//
// CreamFileHeader.cpp
//
// 2004/2/24 SYZ
//
// Copyright (C) 2003-2004 by the CREAM project.  All rights reserved.
// 
// This file is part of CDAQ, the CREAM data acquisition and processing system.
// 
// This work may not be reproduced, displayed, modified or distributed
// without the express prior written permission of the copyright holder.
// For permission, contact Prof. Eun-suk Seo at seo@umd.edu.  The above
// copyright notice must appear in all copies and in supporting documentation.
// 
// The software is provided without any warranty, express or implied, including
// but not limited to the warranties of merchantability, fitness for a
// particular purpose and noninfringement.  In no event shall the author or
// the CREAM project group be liable for any claim, damages or other liability,
// whether in an action of contract, tort or otherwise, arising from, out of
// or in connection with the software or the use or other dealings in the
// software.
//
// $Log: CreamFileHeader.cpp,v $
// Revision 1.3  2012/11/16 21:11:55  dja1987
// Fixes to TCD/BCD crmfile. Updated HK format. Improved client playback (no more temporary arrays for missing packet numbers). Updated cprint
//
// Revision 1.2  2012/03/21 20:41:34  dja1987
// T/BCD updates.
//
// Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
// Importing SCDL bug fixes.
//
// Revision 1.1.1.1  2011/06/07 19:10:20  dja1987
// Includes both TRD and SCDL updates
//
// Revision 1.1.1.1  2010/06/11 15:14:31  spaced4
// CREAM6
//
// Revision 5.1  2009/03/02 04:42:01  creamop
// Pcm Encoder and TDRSS w/o SEI
//
// Revision 5.0  2009/02/02 03:05:20  creamop
// S.L 5.2 and FC3 both.
//
// Revision 1.1.1.1  2009/02/02 03:01:13  creamop
// hahaha
//
// Revision 5.0  2009/01/24 04:33:24  creamop
// S.L 5.2 Version
//
// Revision 1.1.1.1  2009/01/24 04:30:19  creamop
// hahaha
//
// Revision 5.0  2009/01/24 03:48:22  creamop
// S.L 5.2 Version.
//
// Revision 1.1.1.1  2009/01/24 03:44:34  creamop
// hahaha
//
// Revision 4.0  2009/01/20 08:13:49  creamop
// CREAM IV flight version.
//
// Revision 1.1.1.1  2009/01/20 08:12:09  creamop
// hahaha
//
// Revision 1.5  2006/03/24 14:42:37  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.4.2.1  2006/03/14 21:07:37  syzinn
// eight-byte timestamp
//
// Revision 1.4  2005/10/29 00:27:15  cvs
// format 3.2 to implement run number
//
// Revision 1.3  2005/04/08 16:33:47  cvs
// supports format 3.1
//
// Revision 1.2  2005/02/28 02:10:17  daq
// changed to format 3.0
//
// Revision 1.1.1.1  2005/02/04 05:02:17  daq
// initial version for CREAM2.
//
// Revision 1.2  2004/04/15 20:59:02  syzinn
// added copyright notice.
//
// Revision 1.1.1.1  2004/02/24 21:46:25  syzinn
// crmfile library
//
//
#include <stdio.h>
#include <string.h>
#include "CreamFileHeader.h"
#include "fio.c"

CreamFileHeader::CreamFileHeader()
	{
}

CreamFileHeader::~CreamFileHeader()
	{
}

int CreamFileHeader::init_header(void)
	{
	return FillRunHeader( &_h.v3 );
}

int CreamFileHeader::read_header(FILE* fp)
	{
	char* p;

	// read id and version numbers.
	if ( fread(_header_cbuf, 1, 7, fp) != 7 ) return -1;

	// check id and version numbers.
	p = _header_cbuf;
	strncpy(_h.any.id, p, 5); p += 5;
	_h.any.major = *p++;
	_h.any.minor = *p++;
	if ( check_header() != 0 )
		return -1;

	switch ( GetFormatVersion() ) {
	case 0x0000: // format 0.0
	case 0x0100: // format 1.0
	case 0x0101: // format 1.1
		// header of 128 bytes.
		if ( fread(_header_cbuf + 7, 1, 121, fp) != 121 ) return -1;
		read_header_1();
		_run_date = 1000 * _h.v1.year + 100 * _h.v1.month + _h.v1.day;
		break;
	case 0x0200: // format 2.0
		// header of 128 bytes.
		if ( fread(_header_cbuf + 7, 1, 121, fp) != 121 ) return -1;
		read_header_2();
		_run_date = 1000 * _h.v2.year + 100 * _h.v2.month + _h.v2.day;
		break;
	case 0x0300: // format 3.0
	case 0x0301: // format 3.1
	case 0x0302: // format 3.2
	case 0x0303: // format 3.3
	case 0x0800: // format 8.0
		// header of 32 bytes.
		if ( fread(_header_cbuf + 7, 1, 25, fp) != 25 ) return -1;
		read_header_3();
		_run_date = 1000 * _h.v3.year + 100 * _h.v3.month + _h.v3.day;
		break;
	}

	return 0;
}

/* check header information */
/* return 0 if okay, -1 if not a cream file, -2 if version mismatches */
int CreamFileHeader::check_header(void)
	{
	if ( _h.any.major > CREAMFILE_CUR_MAJOR ||
	     (_h.any.major == CREAMFILE_CUR_MAJOR &&
	       _h.any.minor > CREAMFILE_CUR_MINOR) ) {
		fprintf(stderr, "unsupported file format %d.%d (program supports up to %d.%d)\n",
			_h.any.major, _h.any.minor,
			CREAMFILE_CUR_MAJOR, CREAMFILE_CUR_MINOR);
		return -2;
	}

	/* check id string for format version 1 or higher */
	if ( _h.any.major >= 1 && strncmp(_h.any.id, "CREAM", 5) != 0 ) { /* not a CREAM file */
		fprintf(stderr, "missing CREAM id string\n");
		return -1;
	}

	return 0;
}

int CreamFileHeader::read_header_1()
	{
	char* p;

	p = _header_cbuf;
	p += 5; // skip id.  already read.
	p++; // skip major.  already read.
	p++; // skip minor.  already read.
	_h.v1.year = get_uint2( (unsigned char*) p ); p += 2;
	_h.v1.month = *p++;
	_h.v1.day = *p++;
	_h.v1.hour = *p++;
	_h.v1.min = *p++;
	_h.v1.sec = *p++;
	_evc_1 = get_uint2( (unsigned char*) p ); p += 2;
	_h.v1.run = *p++;

	return 0;
}

int CreamFileHeader::print_run_header_1(FILE* fp)
	{
	int k;

	fprintf(fp, "========== HEADER INFORMATION ==========\n");
	fprintf(fp, "format id string = ");
	for(k = 0; k < 5; k++) fprintf(fp, "%c", _h.v1.id[k]);
	fprintf(fp, "\n");
	fprintf(fp, "format version = %d.%d\n", _h.v1.major, _h.v1.minor);
	fprintf(fp, "date and time = %04d-%02d-%02d %02d:%02d:%02d\n",
			_h.v1.year, _h.v1.month, _h.v1.day, _h.v1.hour, _h.v1.min, _h.v1.sec);

	// event type is somewhat longer
	fprintf(fp, "event composition = %04x [", _evc_1);
	k = 0;
	if ( _evc_1 & FMT1_EVENT_TRIGGER ) {
		fprintf(fp, "trigger");
		k++;
	}
	if ( _evc_1 & FMT1_EVENT_CALORIMETER ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "calorimeter");
		k++;
	}
	if ( _evc_1 & FMT1_EVENT_TCRD ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "tcrd");
		k++;
	} //YONAS
	if ( _evc_1 & FMT1_EVENT_CAMAC ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "camac");
		k++;
	}
	if ( _evc_1 & FMT1_EVENT_HODOSCOPE ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "hodoscope");
		k++;
	}
	if ( _evc_1 & FMT1_EVENT_SCD ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "scd");
		k++;
	}
	if ( _evc_1 & FMT1_EVENT_TIMESTAMP ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "timestamp");
		k++;
	}
	fprintf(fp, "]\n");

	fprintf(fp, "run type = %d\n", _h.v1.run);

	return 0;
}

int CreamFileHeader::read_header_2()
	{
	char* p;

	p = _header_cbuf;
	p += 5; // skip id.  already read.
	p++; // skip major.  already read.
	p++; // skip minor.  already read.
	_h.v2.year = get_uint2( (unsigned char*) p ); p += 2;
	_h.v2.month = *p++;
	_h.v2.day = *p++;
	_h.v2.hour = *p++;
	_h.v2.min = *p++;
	_h.v2.sec = *p++;
	_h.v2.ref_year = get_uint2( (unsigned char*) p ); p += 2;
	_h.v2.run = *p++;
	_h.v2.beam = *p++;
	_h.v2.momentum = get_real4( (unsigned char*) p ); p += 4;
	_h.v2.trig_rate = get_real4( (unsigned char*) p ); p += 4;
	_h.v2.tbl_xpos = get_real4( (unsigned char*) p ); p += 4;
	_h.v2.tbl_ypos = get_real4( (unsigned char*) p ); p += 4;
	for(int k = 0; k < 20; k++)
		_h.v2.high_vol[k] = get_real4( (unsigned char*) p ); p += 4;
	_h.v2.bias_status = get_uint4( (unsigned char*) p ); p += 4;
	_h.v2.sprs_status = *p++;

	return 0;
}

int CreamFileHeader::print_run_header_2(FILE* fp)
	{
	int k;

	fprintf(fp, "========== HEADER INFORMATION ==========\n");
	fprintf(fp, "format id string = ");
	for(k = 0; k < 5; k++) fprintf(fp, "%c", _h.v2.id[k]);
	fprintf(fp, "\n");
	fprintf(fp, "format version = %d.%d\n", _h.v2.major, _h.v2.minor);

	fprintf(fp, "date and time = %04d-%02d-%02d %02d:%02d:%02d\n",
			_h.v2.year, _h.v2.month, _h.v2.day, _h.v2.hour, _h.v2.min, _h.v2.sec);
	fprintf(fp, "event reference year = %04d\n", _h.v2.ref_year);

	fprintf(fp, "run type = %d\n", _h.v2.run);
	fprintf(fp, "beam type = %d\n", _h.v2.beam);
	fprintf(fp, "momentum = %g\n", _h.v2.momentum);
	fprintf(fp, "trigger rate = %g\n", _h.v2.trig_rate);
	fprintf(fp, "table position = %g, %g\n", _h.v2.tbl_xpos, _h.v2.tbl_ypos);
	fprintf(fp, "high voltage array = %g, %g, ..., %g\n",
		_h.v2.high_vol[0], _h.v2.high_vol[1], _h.v2.high_vol[19]);
	fprintf(fp, "bias status = 0x%08X\n", _h.v2.bias_status);
	fprintf(fp, "sparsification status = 0x%02X\n", _h.v2.sprs_status);

	return 0;
}

int CreamFileHeader::read_header_3()
	{
	char* p;

	p = _header_cbuf;
	p += 5; // skip id.  already read.
	p++; // skip major.  already read.
	p++; // skip minor.  already read.
	_h.v3.year = get_uint2( (unsigned char*) p ); p += 2;
	_h.v3.month = *p++;
	_h.v3.day = *p++;
	_h.v3.hour = *p++;
	_h.v3.min = *p++;
	_h.v3.sec = *p++;
	_h.v3.ref_year = get_uint2( (unsigned char*) p ); p += 2;
	_h.v3.run = *p++;
	_h.v3.runno = get_uint2( (unsigned char*) p ); p += 2;

	return 0;
}

int CreamFileHeader::print_run_header_3(FILE* fp)
	{
	int k;

	fprintf(fp, "========== HEADER INFORMATION ==========\n"); // cprint starts here David Angelaszek April 7, 2011
	fprintf(fp, "format id string = ");
	for(k = 0; k < 5; k++) fprintf(fp, "%c", _h.v3.id[k]);
	fprintf(fp, "\n");
	fprintf(fp, "format version = %d.%d\n", _h.v3.major, _h.v3.minor);

	fprintf(fp, "date and time = %04d-%02d-%02d %02d:%02d:%02d\n",
			_h.v3.year, _h.v3.month, _h.v3.day, _h.v3.hour, _h.v3.min, _h.v3.sec);
	fprintf(fp, "event reference year = %04d\n", _h.v3.ref_year);

	fprintf(fp, "run type = %d, number = %d\n", _h.v3.run, _h.v3.runno);

	return 0;
}

int CreamFileHeader::FillRunHeader(cream_run_header* h)
	{
	if ( h == 0 ) return -1;

	memset((void*) h, 0, CREAMFILE_RUN_HEADER_LENGTH); // fill zeros

	h->id[0] = 'C';
	h->id[1] = 'R';
	h->id[2] = 'E';
	h->id[3] = 'A';
	h->id[4] = 'M';
	h->major = CREAMFILE_CUR_MAJOR;
	h->minor = CREAMFILE_CUR_MINOR;

	// date and time must be set by user before writing
	h->year = 0;
	h->month = 0;
	h->day = 0;
	h->hour = 0;
	h->min = 0;
	h->sec = 0;
	h->ref_year = 2000;

	h->run = CREAM_NORMAL_RUN;
	h->runno = 0; // should be set by user before writing.

	return 0;
}

int CreamFileHeader::PrintRunHeader(FILE* fp)
	{
	switch ( GetFormatVersion() ) {
	case 0x0000: // format 0.0
	case 0x0100: // format 1.0
	case 0x0101: // format 1.1
		return print_run_header_1( fp );
		break;
	case 0x0200: // format 2.0
		return print_run_header_2( fp );
		break;
	case 0x0300: // format 3.0
	case 0x0301: // format 3.1
	case 0x0302: // format 3.2
	case 0x0303: // format 3.3
	case 0x0800: // format 8.0
		return print_run_header_3( fp );
		break;
	}
	return -1;
}

int CreamFileHeader::WriteRunHeader(FILE* fp, cream_run_header* h)
	{
	char *p;

	// write run header of 32 bytes.
	memset((void*) _header_cbuf, 0, CREAMFILE_RUN_HEADER_LENGTH);
	p = _header_cbuf;
	// id string
	*p++ = h->id[0];
	*p++ = h->id[1];
	*p++ = h->id[2];
	*p++ = h->id[3];
	*p++ = h->id[4];
	// data format version
	*p++ = h->major; // major
	*p++ = h->minor; // minor
	// date and time
	put_uint2(p, h->year); p += 2;
	*p++ = h->month;
	*p++ = h->day;
	*p++ = h->hour;
	*p++ = h->min;
	*p++ = h->sec;
	put_uint2(p, h->ref_year); p += 2;
	// data type
	*p++ = h->run;
	put_uint2(p, h->runno); p += 2;

	// write
	if ( fwrite((const void*) _header_cbuf, sizeof(char), CREAMFILE_RUN_HEADER_LENGTH, fp) != CREAMFILE_RUN_HEADER_LENGTH ) {
		fprintf(stderr, "write_run_header: write failure; disk problem?");
		return -1;
	}

	return 0;
}

