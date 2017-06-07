//
// CreamHKFileHeader.cpp
//
// 2005/6/4 SYZ
//
// Copyright (C) 2003-2005 by the CREAM project.  All rights reserved.
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
// $Log: CreamHKFileHeader.cpp,v $
// Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
// Importing SCDL bug fixes.
//
// Revision 1.1.1.1  2011/06/07 19:10:21  dja1987
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
// Revision 1.6  2006/03/24 14:42:38  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.5.4.1  2006/03/14 21:07:39  syzinn
// eight-byte timestamp
//
// Revision 1.5  2005/08/08 22:57:24  syzinn
// backward compatibility
//
// Revision 1.4  2005/07/06 21:21:42  cvs
// support for hk ver 3
//
// Revision 1.3  2005/06/07 18:40:45  cvs
// updated
//
// Revision 1.2  2005/06/07 18:39:46  cvs
// updated
//
// Revision 1.1  2005/06/06 21:30:57  cvs
// new
//
//
#include <stdio.h>
#include <string.h>
#include "CreamHKFileHeader.h"
#include "fio.c"

CreamHKFileHeader::CreamHKFileHeader()
	{
}

CreamHKFileHeader::~CreamHKFileHeader()
	{
}

int CreamHKFileHeader::init_header(void)
	{
	return FillHeader( &_h.v5 );
}

int CreamHKFileHeader::read_header(FILE* fp)
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
//	case 0x0000: // format 0.0
	case 0x0100: // format 1.0
	case 0x0200: // format 2.0
	case 0x0300: // format 3.0
	case 0x0301: // format 3.1
	case 0x0302: // format 3.2
	case 0x0303: // format 3.3
	case 0x0304: // format 3.4
	case 0x0305: // format 3.5
		// no header so rewind
		rewind( fp );
		_run_date = 0;
		break;
	case 0x0400: // format 4.0
		// header of 32 bytes.
		if ( fread(_header_cbuf + 7, 1, 25, fp) != 25 ) return -1;
		read_header_4();
		_run_date = 1000 * _h.v4.year + 100 * _h.v4.month + _h.v4.day;
		break;
	case 0x0500: // format 5.0
		// header of 32 bytes.
		if ( fread(_header_cbuf + 7, 1, 25, fp) != 25 ) return -1;
		read_header_5();
		_run_date = 1000 * _h.v5.year + 100 * _h.v5.month + _h.v5.day;
		break;
	case 0x0600: // format 6.0
		// header of 32 bytes.
		if ( fread(_header_cbuf + 7, 1, 25, fp) != 25 ) return -1;
		read_header_6();
		_run_date = 1000 * _h.v5.year + 100 * _h.v5.month + _h.v5.day;
		break;
	case 0x0800: // format 8.0
		// header of 32 bytes.
		if ( fread(_header_cbuf + 7, 1, 25, fp) != 25 ) return -1;
		read_header_5();
		_run_date = 1000 * _h.v5.year + 100 * _h.v5.month + _h.v5.day;
		break;
	}

	return 0;
}

/* check header information */
/* return 0 if okay, -1 if not a cream file, -2 if version mismatches */
int CreamHKFileHeader::check_header(void)
	{
	if ( strncmp(_h.any.id, "CRMHK", 5) == 0 ) {
		// valid ID for version 4.0 and higher.

		// check for version number
		if ( _h.any.major > CREAMHKFILE_CUR_MAJOR ||
		     (_h.any.major == CREAMHKFILE_CUR_MAJOR &&
		       _h.any.minor > CREAMHKFILE_CUR_MINOR) ) {
			fprintf(stderr, "unsupported file format %d.%d (program supports up to %d.%d)\n",
				_h.any.major, _h.any.minor,
				CREAMHKFILE_CUR_MAJOR, CREAMHKFILE_CUR_MINOR);
			return -2;
		}
	}
	else {
		// no header information for earlier versions.
		strcpy(_h.any.id, "CRMHK");

		// read the first 4 bytes
		unsigned x;
		x = ((unsigned) _header_cbuf[0] & 0x00ff) << 24L;
		x += ((unsigned) _header_cbuf[1] & 0x00ff) << 16L;
		x += ((unsigned) _header_cbuf[2] & 0x00ff) << 8L;
		x += ((unsigned) _header_cbuf[3] & 0x00ff);

		// try to determine version number
		switch ( x ) {
//		0x0300????:
//			// format 0
//			break;
		case 0x0301C302:
			// format 1.0
			_h.any.major = 1;
			_h.any.minor = 0;
			break;
		case 0x0302C302:
			// format 2.0
			_h.any.major = 2;
			_h.any.minor = 0;
			break;
		case 0x0303F102:
			// format 3.0
			_h.any.major = 3;
			_h.any.minor = 0;
			break;
		case 0x0303EE02:
			// format 3.1 (10/15/2004)
			// format 3.2 (10/19/2004) : CREAM-I flight
			// format 3.3 (02/18/2005)
			_h.any.major = 3;
			_h.any.minor = 3;
			break;
		case 0xEE020303:
			// format 3.4 (02/26/2005)
			// event length comes first.
			_h.any.major = 3;
			_h.any.minor = 4;
			break;
		case 0xEB020303:
			// format 3.5 (04/07/2005)
			// packet uses two-bytes CRC instead of one.
			_h.any.major = 3;
			_h.any.minor = 5;
			break;
		default:
			fprintf(stderr, "not a CREAM HK file %08x\n", x);
			fprintf(stderr, "Perhaps, you might want to ask Sonny or Joonhyuk?\n");
			return -1;
			break;
		}
	}

	return 0;
}

int CreamHKFileHeader::read_header_4()
	{
	char* p;

	p = _header_cbuf;
	p += 5; // skip id.  already read.
	p++; // skip major.  already read.
	p++; // skip minor.  already read.
	_h.v4.year = get_uint2( (unsigned char*) p ); p += 2;
	_h.v4.month = *p++;
	_h.v4.day = *p++;
	_h.v4.hour = *p++;
	_h.v4.min = *p++;
	_h.v4.sec = *p++;
	_h.v4.ref_year = get_uint2( (unsigned char*) p ); p += 2;

	return 0;
}

int CreamHKFileHeader::print_header_4(FILE* fp)
	{
	int k;

	fprintf(fp, "========== HEADER INFORMATION ==========\n");
	fprintf(fp, "format id string = ");
	for(k = 0; k < 5; k++) fprintf(fp, "%c", _h.v4.id[k]);
	fprintf(fp, "\n");
	fprintf(fp, "format version = %d.%d\n", _h.v4.major, _h.v4.minor);

	fprintf(fp, "date and time = %04d-%02d-%02d %02d:%02d:%02d\n",
			_h.v4.year, _h.v4.month, _h.v4.day, _h.v4.hour, _h.v4.min, _h.v4.sec);
	fprintf(fp, "event reference year = %04d\n", _h.v4.ref_year);

	return 0;
}

int CreamHKFileHeader::FillHeader(cream_hk_header* h)
	{
	if ( h == 0 ) return -1;

	memset((void*) h, 0, CREAMHKFILE_HEADER_LENGTH); // fill zeros

	h->id[0] = 'C';
	h->id[1] = 'R';
	h->id[2] = 'M';
	h->id[3] = 'H';
	h->id[4] = 'K';
	h->major = CREAMHKFILE_CUR_MAJOR;
	h->minor = CREAMHKFILE_CUR_MINOR;

	// date and time must be set by user before writing
	h->year = 0;
	h->month = 0;
	h->day = 0;
	h->hour = 0;
	h->min = 0;
	h->sec = 0;
	h->ref_year = 2000;

	return 0;
}

int CreamHKFileHeader::PrintHeader(FILE* fp)
	{
	switch ( GetFormatVersion() ) {
//	case 0x0000: // format 0.0
	case 0x0100: // format 1.0
	case 0x0200: // format 2.0
	case 0x0300: // format 3.0
	case 0x0301: // format 3.1
	case 0x0302: // format 3.2
	case 0x0303: // format 3.3
	case 0x0304: // format 3.4
	case 0x0305: // format 3.5
		// nothing to do
		break;
	case 0x0400: // format 4.0
		return print_header_4( fp );
		break;
	case 0x0500: // format 5.0
		return print_header_5( fp );
		break;
	case 0x0600: // format 6.0
		return print_header_6( fp );
		break;
	case 0x0800: // format 8.0
		return print_header_5( fp );
		break;
	}
	return -1;
}

int CreamHKFileHeader::WriteHeader(FILE* fp, cream_hk_header* h)
	{
	char *p;

	// write run header of 32 bytes.
	memset((void*) _header_cbuf, 0, CREAMHKFILE_HEADER_LENGTH);
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

	// write
	if ( fwrite((const void*) _header_cbuf, sizeof(char), CREAMHKFILE_HEADER_LENGTH, fp) != CREAMHKFILE_HEADER_LENGTH ) {
		fprintf(stderr, "write_run_header: write failure; disk problem?");
		return -1;
	}

	return 0;
}
