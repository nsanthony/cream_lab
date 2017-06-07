//
// CreamFile.cpp
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
// $Log: CreamFile.cpp,v $
// Revision 1.4  2012/11/16 21:11:55  dja1987
// Fixes to TCD/BCD crmfile. Updated HK format. Improved client playback (no more temporary arrays for missing packet numbers). Updated cprint
//
// Revision 1.3  2012/10/18 14:49:18  dja1987
// Checking in some bug fixes for playback. Playback will now send playback packets event when we aren't dropping packets (i.e. we don't always have to wait for a specific number of packets that need to be playbacked). Also checking in some beginning changes for USB commanding
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
// Revision 1.1.1.1  2009/02/02 03:01:14  creamop
// hahaha
//
// Revision 5.0  2009/01/24 04:33:24  creamop
// S.L 5.2 Version
//
// Revision 1.1.1.1  2009/01/24 04:30:20  creamop
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
// Revision 1.1.1.1  2009/01/20 08:12:10  creamop
// hahaha
//
// Revision 1.17  2007/02/21 00:38:56  juneki
// revised reading CER crmfile
//
// Revision 1.16  2006/08/09 19:03:22  syzinn
// SBT data format changed
//
// Revision 1.15  2006/05/02 19:53:22  syzinn
// changes for USB SBT data format
//
// Revision 1.14  2006/05/01 19:39:24  syzinn
// support for silicon beam tracker
//
// Revision 1.13  2006/03/24 14:42:37  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.12.2.3  2006/03/15 21:48:00  syzinn
// reader for format 3.3 handles DAT_STAT
//
// Revision 1.12.2.2  2006/03/14 22:46:15  syzinn
// sw event number is back
//
// Revision 1.12.2.1  2006/03/14 21:07:37  syzinn
// eight-byte timestamp
//
// Revision 1.12  2005/12/17 07:36:00  syzinn
// keep event length
//
// Revision 1.11  2005/11/24 22:38:00  syzinn
// bug fix in Rewind() for format 3.2
//
// Revision 1.10  2005/11/07 22:50:49  cvs
// bug fix of read_cer()
//
// Revision 1.9  2005/11/07 21:25:32  cvs
// supports Cerenkov Camera data
//
// Revision 1.8  2005/10/29 00:27:15  cvs
// format 3.2 to implement run number
//
// Revision 1.7  2005/07/31 17:27:55  cvs
// support CVD data section
//
// Revision 1.6  2005/04/26 19:27:37  cvs
// bug fixes
//
// Revision 1.5  2005/04/08 16:33:45  cvs
// supports format 3.1
//
// Revision 1.4  2005/04/07 23:01:59  cvs
// bug fix in Rewind()
//
// Revision 1.3  2005/04/07 22:22:44  cvs
// bug fix
//
// Revision 1.2  2005/02/28 02:10:17  daq
// changed to format 3.0
//
// Revision 1.1.1.1  2005/02/04 05:02:17  daq
// initial version for CREAM2.
//
// Revision 1.4  2004/06/21 13:22:18  syzinn
// added _nread to check the number of bytes read.
//
// Revision 1.3  2004/04/15 20:59:02  syzinn
// added copyright notice.
//
// Revision 1.2  2004/02/27 19:19:18  syzinn
// Reads end of run information for format 2.0
//
// Revision 1.1.1.1  2004/02/24 21:46:25  syzinn
// crmfile library
//

//
#ifdef DEBUG_OUT
#define GCC_VERSION ( __GNUC__ * 10000 \
                      + __GNUC_MINOR__ * 100 \
                      + __GNUC_PATCHLEVEL__ )
#if GCC_VERSION >= 30200
using namespace std;
#include <fstream>
#include <cstring> // strncmp()
#else
#include <fstream.h>
#include <string.h> // strncmp()
#endif
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "CreamFile.h"
#ifdef MEMLEAK
#include "memleak.h"
#endif
#include "fio.c"

int CreamFile::clear_lengths()
	{
	_raw_nbytes = 0;
	_trig_len = 0;
	_camac_len = 0;
	_cal_len = 0;
	_tcrd_len = 0; //YONAS
	_hds_len = 0;
	_scd_len = 0;
	_tcd_len = 0;
	_bsd_len = 0;
	_cvd_len = 0;
	_trd_len = 0;
	_tscd_len = 0;
	_bscd_len = 0;
	_scdl1_len = 0; //Added by David Angelaszek
	_scdl2_len = 0; //Added by David Angelaszek
	_scdl3_len = 0; //Added by David Angelaszek
	_cer_len = 0;
	_sbt_len = 0;
	_ts_len = 0;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// For format 0.
//////////////////////////////////////////////////////////////////////////////

int CreamFile::cream_read_0_0()
	{
	// Format 0.0 has only calorimeter information.
	// It contains no trigger, CAMAC, Hodoscope, and SCD information.
	_type = DAT_EVT;
	clear_lengths();

	// only calorimeter infomation
	return read_calorimeter();
}

int CreamFile::read_sparsified(char* cbuf, unsigned len, unsigned& readlen)
	{
	unsigned nchannel; // number of ADC channel from raw data
	unsigned nbytes;
	unsigned nread;

	_nread = 0;

	if ( feof(_fp) ) return -1;

	// read two bytes: channel number plus two
	nread = fread(cbuf, 1, 2, _fp); _nread += nread;
	if ( nread != 2 ) return -1;

	nchannel = get_uint2( (unsigned char*) cbuf ); nchannel -= 2;
	cbuf += 2;
	readlen = 2;

	// TODO: check buffer size.
	// read the rest into memory
	nbytes = 4 * nchannel + 6;
	nread = fread(cbuf, 1, nbytes, _fp); _nread += nread;
	if ( nread != nbytes ) return -1;
	readlen += nbytes;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// For format 1.xx
//////////////////////////////////////////////////////////////////////////////

int CreamFile::cream_read_1_0()
	{
	unsigned type;
	unsigned t; // event type from raw data

	clear_lengths();
	type = _evc_1;
	while ( type ) {
		t = fget_uint2( _fp );
		if ( feof(_fp) ) return -1;

		switch ( t ) {
		case FMT1_EVENT_TRIGGER:
			if ( read_trigger_1() ) return -1;
			break;
		case FMT1_EVENT_CAMAC:
			if ( read_camac() ) return -1;
			break;
		case FMT1_EVENT_CALORIMETER:
			if ( read_calorimeter() ) return -1;
			break;
		case FMT1_EVENT_SCD:
			if ( read_scd() ) return -1;
			break;
		case FMT1_EVENT_HODOSCOPE:
			if ( read_hodoscope() ) return -1;
			break;
		}

		type &= ~t; // clear bit
	}

	_type = _evc_1;
	return 0;
}

int CreamFile::read_trigger_1()
	{
	if ( feof(_fp) ) return -1;
	_trig = fget_uint4( _fp );
	_trig_len = 1;
	return 0;
}

int CreamFile::read_camac()
	{
	unsigned nchannel; // number of camac channels from raw data
	unsigned nbytes;
	unsigned nread;
	char* p = _camac_cbuf;

	_nread = 0;

	if ( feof(_fp) ) return -1;

	// read channel number
	nread = fread(p, 1, 4, _fp); _nread += nread;
	if ( nread != 4 ) return -1;
	nchannel = get_uint4( (unsigned char *) p );
	_camac_len = 4;
	p += 4;

	// read the rest into memory
	nbytes = 4 * nchannel;
	nread = fread(p, 1, nbytes, _fp); _nread += nread;
	if ( nread != nbytes ) return -1;
	_camac_len += nbytes;

	return 0;
}

int CreamFile::cream_read_1_1()
	{
	unsigned type;
	unsigned t; // event type from raw data

	clear_lengths();
	type = _evc_1;
	while ( type ) {
		t = fget_uint2( _fp );
		if ( feof(_fp) ) return -1;

		switch ( t ) {
		case FMT1_EVENT_TRIGGER:
			if ( read_trigger_1() ) return -1;
			break;
		case FMT1_EVENT_CAMAC:
			if ( read_camac() ) return -1;
			break;
		case FMT1_EVENT_CALORIMETER:
			if ( read_calorimeter() ) return -1;
			break;
		case FMT1_EVENT_SCD:
			if ( read_scd() ) return -1;
			break;
		case FMT1_EVENT_HODOSCOPE:               
			if ( read_hodoscope() ) return -1; 
			break;
		case FMT1_EVENT_TIMESTAMP:
			if ( read_timestamp_1() ) return -1;
			break;
	
		type &= ~t; // clear bit
	}

	_type = _evc_1;
	return 0;
}
}

int CreamFile::read_timestamp_1()
	{
	unsigned nbytes;
	char cbuf[8];
	unsigned char* p;

	if ( feof(_fp) ) return -1;

	nbytes = 8;
	if ( fread(cbuf, 1, nbytes, _fp) != nbytes ) return -1;

	p = (unsigned char *) cbuf;
	_timestamp[0] = get_uint2( p ); p += 2; // year
	_timestamp[1] = *p++; // month
	_timestamp[2] = *p++; // day
	_timestamp[3] = *p++; // hour
	_timestamp[4] = *p++; // minute
	_timestamp[5] = *p++; // second
	_ts_len = 6;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// For format version 2.0
//////////////////////////////////////////////////////////////////////////////

/* Format 2 records event composition per event */
#define FMT2_EVENT_TRIGGER	0x80
#define FMT2_EVENT_CAMAC	0x40
#define FMT2_EVENT_CALORIMETER	0x20
#define FMT2_EVENT_TCRD		0x20 //YONAS
#define FMT2_EVENT_HODOSCOPE	0x10
#define FMT2_EVENT_SCD		0x08
#define FMT2_EVENT_TCD		0x04
#define FMT2_EVENT_TRD		0x02

/* Data section order */
#define FMT2_DSI_CAMAC		0
#define FMT2_DSI_CAL		1
#define FMT2_DSI_HDS		2
#define FMT2_DSI_SCD		3
#define FMT2_DSI_TCD		4
#define FMT2_DSI_TRD		5
#define FMT2_DSI_TCRD		6 //YONAS

int CreamFile::cream_read_2_0()
	{
	unsigned char type;
	unsigned char evc;
	int len;
	unsigned tstamp;
	unsigned dsi;
	int nsec, k;
	unsigned sec;
	long len_check;

	// event header
	type = (unsigned char) fgetc( _fp );
	if ( type == 0xff ) { // end of run is detected
		ungetc(type, _fp);
		read_tail( _fp );
		return -1;
	}
	_type = type;
	evc = (unsigned) fgetc( _fp );
	len = (int) fget_uint2( _fp );

	// clear
	clear_lengths();

	// time stamp
	tstamp = (unsigned) fget_uint4( _fp );
	_timestamp[0] =((tstamp >> 26) & 0x0000003F) + _h.v2.ref_year; // year
	_timestamp[1] = (tstamp >> 22) & 0x0000000F; // month
	_timestamp[2] = (tstamp >> 17) & 0x0000001F; // day
	_timestamp[3] = (tstamp >> 12) & 0x0000001F; // hour
	_timestamp[4] = (tstamp >>  6) & 0x0000003F; // min
	_timestamp[5] = (tstamp      ) & 0x0000003F; // sec
	_ts_len = 6;

	// data section index
	dsi = (unsigned) fget_uint4( _fp );

	// trigger info
	_trig = (unsigned) fget_uint2( _fp );
	_trig_len = 1;

	// count data section
	nsec = 0;
	if ( evc & FMT2_EVENT_CAMAC ) nsec++;
	if ( evc & FMT2_EVENT_CALORIMETER ) nsec++;
	if ( evc & FMT2_EVENT_TCRD ) nsec++; //YONAS
	if ( evc & FMT2_EVENT_HODOSCOPE ) nsec++;
	if ( evc & FMT2_EVENT_SCD ) nsec++;
	if ( evc & FMT2_EVENT_TCD ) nsec++;
	if ( evc & FMT2_EVENT_TRD ) nsec++;

	// data section
	len_check = len - 14;
	for(k = 0; k < nsec; k++) {
		if ( feof(_fp) ) return -1;

		sec = (dsi >> (28 - 4 * k)) & 0x0fL;
		switch ( sec ) {
		case FMT2_DSI_CAMAC:
			if ( (evc & FMT2_EVENT_CAMAC) == 0 ) goto dsi_err;
			if ( read_camac() ) return -1;
			len_check -= _nread;
			break;
		case FMT2_DSI_CAL:
			if ( evc & FMT2_EVENT_CALORIMETER == 0 ) goto dsi_err;
			if ( read_calorimeter() ) return -1;
			len_check -= _nread;
			break;
		case FMT2_DSI_SCD:
			if ( evc & FMT2_EVENT_SCD == 0 ) goto dsi_err;
			if ( read_scd() ) return -1;
			len_check -= _nread;
			break;
		case FMT2_DSI_HDS:
			if ( evc & FMT2_EVENT_HODOSCOPE == 0 ) goto dsi_err;
			if ( read_hodoscope() ) return -1;
			len_check -= _nread;
			break;
		case FMT2_DSI_TCD:
			if ( evc & FMT2_EVENT_TCD == 0 ) goto dsi_err;
			if ( read_tcd() ) return -1;
			len_check -= _nread;
			break;
		case FMT2_DSI_TRD:
			if ( evc & FMT2_EVENT_TRD == 0 ) goto dsi_err;
			if ( read_trd() ) return -1;
			len_check -= _nread;
			break;

	}

	if ( len_check != 0 ) {
		fprintf(stderr, "CreamFile2.0: inconsistent data length: nominal %d, actual %ld\n", len, len - len_check);
		fprintf(stderr, "CreamFile2.0: file repositioning by %ld byte(s)\n", len_check);
		fseek(_fp, len_check, SEEK_CUR); 
		// return -1;
	}

	return 0;

dsi_err:
	fprintf(stderr, "CreamFile2.0: corrupt data section info: evc %02X, dsi %08X\n", evc, dsi);
	fprintf(stderr, "CreamFile2.0: file repositioning by %ld byte(s)\n", len_check);
	fseek(_fp, len_check, SEEK_CUR); 
	return 0;
}
}
int CreamFile::read_tcd()
	{
	int nbytes;
	char* p = _tcd_cbuf;
	int nread;


	_nread = 0;

	if ( feof(_fp) ) return -1;

	_tcd_len = nbytes = fget_uint2(_fp); _nread += 2;
	put_uint2(p, nbytes); p += 2; nbytes -= 2;

	//nread = fread(p, 1, nbytecase FMT1_EVENT_SCD:
	//		if ( read_scd() ) return -1;
	//	      break;s, _fp); _nread += nread;

	if ( nread != nbytes ) return -1;

	return 0;
}

/*
 * This function must same as DEventHolder.cpp: unpack_bsd
 *
 */
int CreamFile::read_bsd()
{
	int nread;

	_nread = fread(_bsd_cbuf, 1, BSD_NUM_RAW_BYTES, _fp);
	_bsd_len = _nread;
	if(_nread != BSD_NUM_RAW_BYTES) return -1;
	return 0;
}

int CreamFile::read_trd()
	{
	int nbytes;
	char* p = _trd_cbuf;
	int nread;

	_nread = 0;

	if ( feof(_fp) ) return -1;

	_trd_len = nbytes = fget_uint2(_fp); _nread += 2;
	put_uint2(p, nbytes); p += 2; nbytes -= 2;

	nread = fread(p, 1, nbytes, _fp); _nread += nread;
	if ( nread != nbytes ) return -1;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// For format version 3.0
//////////////////////////////////////////////////////////////////////////////

/* Format 3 records event composition per event */
//#define FMT3_EVC_SCDL1	0x6400 //Added by David Angelaszek 9/1/2010
//#define FMT3_EVC_SCDL2	0x3200 //Added by David Angelaszek 9/1/2010
//#define FMT3_EVC_SCDL3	0x1600 //Added by David Angelaszek 9/1/2010
#define FMT3_EVC_SCDL1	0x8000
#define FMT3_EVC_SCDL2	0x4000
#define FMT3_EVC_SCDL3	0x2000
//#define FMT3_EVC_RSV4	0x1000
#define FMT3_EVC_SBT	0x0800
#define FMT3_EVC_CVD	0x0400
#define FMT3_EVC_tSCD	0x0200
#define FMT3_EVC_bSCD	0x0100
#define FMT3_EVC_TRG	0x0080
#define FMT3_EVC_CMC	0x0040
#define FMT3_EVC_CAL	0x0020
#define FMT3_EVC_TCRD	0x1000 //YONAS
#define FMT3_EVC_HDS	0x0010
#define FMT3_EVC_SCD	0x0008
#define FMT3_EVC_TCD	0x0004
#define FMT3_EVC_BSD	0x0002
#define FMT3_EVC_TRD	0x0000
#define FMT3_EVC_CER	0x0001

/* Data section order */
#define FMT3_DSI_CMC	0
#define FMT3_DSI_CAL	1
#define FMT3_DSI_HDS	2
#define FMT3_DSI_SCD	3
#define FMT3_DSI_TCD	4
#define FMT3_DSI_TRD	5
#define FMT3_DSI_CER	6
#define FMT3_DSI_tSCD	7
#define FMT3_DSI_bSCD	8
#define FMT3_DSI_CVD	9
#define FMT3_DSI_SBT	10
#define FMT3_DSI_SCDL1	11
#define FMT3_DSI_SCDL2	12
#define FMT3_DSI_SCDL3	13
#define FMT3_DSI_TCRD	14
#define FMT3_DSI_BSD	15
//#define FMT3_DSI_TCRD	16 //YONAS
//#define FMT3_DSI_SCDL1	16   //Added by David Angelaszek 9/1/2010
//#define FMT3_DSI_SCDL2	17   //Added by David Angelaszek 9/1/2010
//#define FMT3_DSI_SCDL3	18   //Added by David Angelaszek 9/1/2010

int CreamFile::cream_read_3_0()
	{
	unsigned len;
	unsigned type, subtype;
	unsigned evc;
	unsigned tstamp;
	unsigned dsi[2];
	unsigned nsec, k;
	unsigned sec;
	long len_check;

	// event header
	len = (unsigned) fget_uint2( _fp );
	type = (unsigned) fgetc( _fp );
	if ( type == 0xff ) { // end of run is detected
		ungetc(type, _fp);
		ungetc((len >> 8) & 0x00ff, _fp);
		ungetc(len & 0x00ff, _fp);
		read_tail( _fp );
		return -1;
	}
	_type = type;
	subtype = (unsigned) fgetc( _fp );

	// clear
	clear_lengths();
	_raw_nbytes = len;

	// time stamp
	tstamp = (unsigned) fget_uint4( _fp );
	_timestamp[0] =((tstamp >> 26) & 0x0000003F) + _h.v3.ref_year; // year
	_timestamp[1] = (tstamp >> 22) & 0x0000000F; // month
	_timestamp[2] = (tstamp >> 17) & 0x0000001F; // day
	_timestamp[3] = (tstamp >> 12) & 0x0000001F; // hour
	_timestamp[4] = (tstamp >>  6) & 0x0000003F; // min
	_timestamp[5] = (tstamp      ) & 0x0000003F; // sec
	_ts_len = 6;

	// data section index
	dsi[0] = (unsigned) fget_uint4( _fp );
	dsi[1] = (unsigned) fget_uint4( _fp );

	// event composition
	evc = (unsigned) fget_uint2( _fp );

	// trigger info
	_trig = (unsigned) fget_uint2( _fp );
	_trig_len = 1;

	// obtain number of data section by counting EVC bits.
	nsec = 0;
	for(k = 0; k < 16; k++)
		nsec += (evc & (1L << k)) ? 1 : 0;
	// trigger bit should not be counted.
	if ( evc & FMT3_EVC_TRG ) nsec--;

	// data section
	len_check = len - 20;
	for(k = 0; k < nsec; k++) {
		if ( feof(_fp) ) return -1;

		if ( k < 8 )
			sec = (dsi[0] >> (4 * (7 - k))) & 0x0fL;
		else
			sec = (dsi[1] >> (4 * (15 - k))) & 0x0fL;

		switch ( sec ) {
		case FMT3_DSI_CMC:
			if ( (evc & FMT3_EVC_CMC) == 0 ) goto dsi_err;
			if ( read_camac() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CAL:
			if ( evc & FMT3_EVC_CAL == 0 ) goto dsi_err;
			if ( read_calorimeter() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_SCD:
			if ( evc & FMT3_EVC_SCD == 0 ) goto dsi_err;
			if ( read_scd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_tSCD:
			if ( evc & FMT3_EVC_tSCD == 0 ) goto dsi_err;
			if ( read_tscd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_bSCD:
			if ( evc & FMT3_EVC_bSCD == 0 ) goto dsi_err;
			if ( read_bscd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_HDS:
			if ( evc & FMT3_EVC_HDS == 0 ) goto dsi_err;
			if ( read_hodoscope() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TCD:
			if ( evc & FMT3_EVC_TCD == 0 ) goto dsi_err;
			if ( read_tcd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TRD:
			if ( evc & FMT3_EVC_TRD == 0 ) goto dsi_err;
			if ( read_trd() ) return -1;
			len_check -= _nread;
			break;
		}

	}

	if ( len_check != 0 ) {
		fprintf(stderr, "CreamFile3.0: inconsistent data length: nominal %d, actual %ld\n", len, len - len_check);
		fprintf(stderr, "CreamFile3.0: file repositioning by %ld byte(s)\n", len_check);
		fseek(_fp, len_check, SEEK_CUR); 
		// return -1;
	}

	return 0;

dsi_err:
	fprintf(stderr, "CreamFile3.0: corrupt data section info: evc %02X, dsi %08X\n", evc, dsi);
	fprintf(stderr, "CreamFile3.0: file repositioning by %ld byte(s)\n", len_check);
	fseek(_fp, len_check, SEEK_CUR); 
	return 0;
}

int CreamFile::read_cvd()
	{
	int nbytes;
	char* p = _cvd_cbuf;
	int nread;

	_nread = 0;

	if ( feof(_fp) ) return -1;

	_cvd_len = nbytes = fget_uint2(_fp); _nread += 2;
	put_uint2(p, nbytes); p += 2; nbytes -= 2;

	nread = fread(p, 1, nbytes, _fp); _nread += nread;
	if ( nread != nbytes ) return -1;

	return 0;
}

int CreamFile::read_cer()
	{
	int nbytes, nch;
	unsigned char* p = (unsigned char*) _cer_cbuf;
	int nread;

	_nread = 0;

	if ( feof(_fp) ) return -1;

	// read six bytes
	nbytes = 8;
	nread = fread(p, 1, nbytes, _fp); _nread += nread;
	if ( nread != nbytes ) return -1;
	p += nbytes;

	// obtain length
	nch = p[-2] + (p[-1] << 8L) - 1;
	nbytes = 4 * nch;

	nread = fread(p, 1, nbytes, _fp); _nread += nread;
	if ( nread != nbytes ) return -1;

	_cer_len = nbytes + 8;

	return 0;
}

int CreamFile::cream_read_3_1()
	{
	unsigned len;
	unsigned type, subtype;
	unsigned evc;
	unsigned tstamp;
	unsigned dsi[2];
	unsigned nsec, k;
	unsigned sec;
	long len_check;

	// event header
	len = (unsigned) fget_uint2( _fp );
	type = (unsigned) fgetc( _fp );
	if ( type == 0xff ) { // end of run is detected
		ungetc(type, _fp);
		ungetc((len >> 8) & 0x00ff, _fp);
		ungetc(len & 0x00ff, _fp);
		read_tail( _fp );
		return -1;
	}
	_type = type;
	subtype = (unsigned) fgetc( _fp );

	// clear
	clear_lengths();
	_raw_nbytes = len;

	// time stamp
	tstamp = (unsigned) fget_uint4( _fp );
	_timestamp[0] =((tstamp >> 26) & 0x0000003F) + _h.v3.ref_year; // year
	_timestamp[1] = (tstamp >> 22) & 0x0000000F; // month
	_timestamp[2] = (tstamp >> 17) & 0x0000001F; // day
	_timestamp[3] = (tstamp >> 12) & 0x0000001F; // hour
	_timestamp[4] = (tstamp >>  6) & 0x0000003F; // min
	_timestamp[5] = (tstamp      ) & 0x0000003F; // sec
	_ts_len = 6;

	// data section index
	dsi[0] = (unsigned) fget_uint4( _fp );
	dsi[1] = (unsigned) fget_uint4( _fp );

	// event composition
	evc = (unsigned) fget_uint2( _fp );

	// trigger info
	_trig = (unsigned) fget_uint2( _fp );
	_trig_len = 1;

	// sw event number
	_swevno = (unsigned) fget_uint4( _fp );

	// obtain number of data section by counting EVC bits.
	nsec = 0;
	for(k = 0; k < 16; k++)
		nsec += (evc & (1L << k)) ? 1 : 0;
	// trigger bit should not be counted.
	if ( evc & FMT3_EVC_TRG ) nsec--;

	// data section
	len_check = len - 24;
	for(k = 0; k < nsec; k++) {
		if ( feof(_fp) ) return -1;

		if ( k < 8 )
			sec = (dsi[0] >> (4 * (7 - k))) & 0x0fL;
		else
			sec = (dsi[1] >> (4 * (15 - k))) & 0x0fL;

		switch ( sec ) {
		case FMT3_DSI_CMC:
			if ( (evc & FMT3_EVC_CMC) == 0 ) goto dsi_err;
			if ( read_camac() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CAL:
			if ( evc & FMT3_EVC_CAL == 0 ) goto dsi_err;
			if ( read_calorimeter() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_SCD:
			if ( evc & FMT3_EVC_SCD == 0 ) goto dsi_err;
			if ( read_scd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_tSCD:
			if ( evc & FMT3_EVC_tSCD == 0 ) goto dsi_err;
			if ( read_tscd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_bSCD:
			if ( evc & FMT3_EVC_bSCD == 0 ) goto dsi_err;
			if ( read_bscd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_HDS:
			if ( evc & FMT3_EVC_HDS == 0 ) goto dsi_err;
			if ( read_hodoscope() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TCD:
			if ( evc & FMT3_EVC_TCD == 0 ) goto dsi_err;
			if ( read_tcd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TRD:
			if ( evc & FMT3_EVC_TRD == 0 ) goto dsi_err;
			if ( read_trd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CVD:
			if ( evc & FMT3_EVC_CVD == 0 ) goto dsi_err;
			if ( read_cvd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CER:
			if ( evc & FMT3_EVC_CER == 0 ) goto dsi_err;
			if ( read_cer() ) return -1;
			len_check -= _nread;
			break;
		
		}
	}

	// CRC info
	_crc16 = (unsigned) fget_uint2( _fp );
	len_check -= 2;

	if ( len_check != 0 ) {
		fprintf(stderr, "CreamFile3.1: inconsistent data length: nominal %d, actual %ld\n", len, len - len_check);
		fprintf(stderr, "CreamFile3.1: file repositioning by %ld byte(s)\n", len_check);
		fseek(_fp, len_check, SEEK_CUR); 
		// return -1;
	}

	return 0;

dsi_err:
	fprintf(stderr, "CreamFile3.1: corrupt data section info: evc %02X, dsi %08X\n", evc, dsi);
	fprintf(stderr, "CreamFile3.1: file repositioning by %ld byte(s)\n", len_check);
	fseek(_fp, len_check, SEEK_CUR); 
	return 0;
}

int CreamFile::cream_read_3_3()
	{
	unsigned len;
	unsigned type, subtype;
	unsigned evc;
	unsigned tstamp, tstamp2;
	unsigned dsi[2];
	unsigned nsec, k;
	unsigned sec;
	long len_check;

startover:
	// event header
	len = (unsigned) fget_uint2( _fp );
	_crc16 = (unsigned) fget_uint2( _fp );
	type = (unsigned) fgetc( _fp );
	if ( type == 0xff ) { // end of run is detected
		ungetc(type, _fp);
		ungetc((len >> 8) & 0x00ff, _fp);
		ungetc(len & 0x00ff, _fp);
		read_tail( _fp );
		return -1;
	}
	else if ( type == DAT_STAT ) {
		len -= 5; // so far we read five bytes.
		while ( len-- > 0 ) fgetc( _fp );
		goto startover;
	}
	_type = type;
	subtype = (unsigned) fgetc( _fp );

	// reserved
	fget_uint2( _fp );

	// event serial number
	_serial = (unsigned) fget_uint4( _fp );

	// clear
	clear_lengths();
	_raw_nbytes = len;

	// time stamp
	tstamp2 = (unsigned) fget_uint4( _fp );
	tstamp = (unsigned) fget_uint4( _fp );
	_timestamp[0] =((tstamp >> 26) & 0x0000003F) + _h.v3.ref_year; // year
	_timestamp[1] = (tstamp >> 22) & 0x0000000F; // month
	_timestamp[2] = (tstamp >> 17) & 0x0000001F; // day
	_timestamp[3] = (tstamp >> 12) & 0x0000001F; // hour
	_timestamp[4] = (tstamp >>  6) & 0x0000003F; // min
	_timestamp[5] = (tstamp      ) & 0x0000003F; // sec
	_timestamp[6] = (tstamp2 >> 22L) & 0x000003FF; // msec
	_timestamp[7] = (tstamp2 >> 12L) & 0x000003FF; // usec
	_timestamp[8] = (tstamp2 >>  2L) & 0x000003FF; // nsec
	_ts_len = 9;

	// data section index
	dsi[0] = (unsigned) fget_uint4( _fp );
	dsi[1] = (unsigned) fget_uint4( _fp );

	// event composition
	evc = (unsigned) fget_uint2( _fp );

	// trigger info
	_trig = (unsigned) fget_uint2( _fp );
	_trig_len = 1;

	// sw event number
	_swevno = (unsigned) fget_uint4( _fp );

	// obtain number of data section by counting EVC bits.
	nsec = 0;
	for(k = 0; k < 16; k++)
		nsec += (evc & (1L << k)) ? 1 : 0;
	// trigger bit should not be counted.
	if ( evc & FMT3_EVC_TRG ) nsec--;

	// data section
	len_check = len - 36;
	for(k = 0; k < nsec; k++) {
		if ( feof(_fp) ) return -1;

		if ( k < 8 )
			sec = (dsi[0] >> (4 * (7 - k))) & 0x0fL;
		else
			sec = (dsi[1] >> (4 * (15 - k))) & 0x0fL;

		switch ( sec ) {
		case FMT3_DSI_CMC:
			if ( (evc & FMT3_EVC_CMC) == 0 ) goto dsi_err;
			if ( read_camac() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CAL:
			if ( evc & FMT3_EVC_CAL == 0 ) goto dsi_err;
			if ( read_calorimeter() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_SCD:
			if ( evc & FMT3_EVC_SCD == 0 ) goto dsi_err;
			if ( read_scd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_tSCD:
			if ( evc & FMT3_EVC_tSCD == 0 ) goto dsi_err;
			if ( read_tscd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_bSCD:
			if ( evc & FMT3_EVC_bSCD == 0 ) goto dsi_err;
			if ( read_bscd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_SBT:
			if ( evc & FMT3_EVC_SBT == 0 ) goto dsi_err;
			if ( read_sbt() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_HDS:
			if ( evc & FMT3_EVC_HDS == 0 ) goto dsi_err;
			if ( read_hodoscope() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TCD:
			if ( evc & FMT3_EVC_TCD == 0 ) goto dsi_err;
			if ( read_tcd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TRD:
			if ( evc & FMT3_EVC_TRD == 0 ) goto dsi_err;
			if( read_trd2() ) return -1; // Angelaszek TRD
			//if ( read_trd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CVD:
			if ( evc & FMT3_EVC_CVD == 0 ) goto dsi_err;
			if ( read_cvd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CER:
			if ( evc & FMT3_EVC_CER == 0 ) goto dsi_err;
			if ( read_cer() ) return -1;
			len_check -= _nread;
			break;

		}

	}
	if ( len_check != 0 ) {
		fprintf(stderr, "CreamFile3.3: inconsistent data length: nominal %d, actual %ld\n", len, len - len_check);
		fprintf(stderr, "CreamFile3.3: file repositioning by %ld byte(s)\n", len_check);
		fseek(_fp, len_check, SEEK_CUR); 
		// return -1;
	}

	return 0;

dsi_err:
	fprintf(stderr, "CreamFile3.3: corrupt data section info: evc %02X, dsi %08X\n", evc, dsi);
	fprintf(stderr, "CreamFile3.3: file repositioning by %ld byte(s)\n", len_check);
	fseek(_fp, len_check, SEEK_CUR); 
	return 0;
}

int CreamFile::cream_read_8_0()
	{
	unsigned len;
	unsigned type, subtype;
	unsigned evc;
	unsigned tstamp, tstamp2;
	unsigned dsi[2];
	unsigned nsec, k;
	unsigned sec;
	long len_check;

startover:
	// event header
	len = (unsigned) fget_uint2( _fp );
	_crc16 = (unsigned) fget_uint2( _fp );
	type = (unsigned) fgetc( _fp );
	if ( type == 0xff ) { // end of run is detected
		ungetc(type, _fp);
		ungetc((len >> 8) & 0x00ff, _fp);
		ungetc(len & 0x00ff, _fp);
		read_tail( _fp );
		return -1;
	}
	else if ( type == DAT_STAT ) {
		len -= 5; // so far we read five bytes.
		while ( len-- > 0 ) fgetc( _fp );
		goto startover;
	}
	_type = type;
	subtype = (unsigned) fgetc( _fp );

	// reserved
	fget_uint2( _fp );

	// event serial number
	_serial = (unsigned) fget_uint4( _fp );

	// clear
	clear_lengths();
	_raw_nbytes = len;

	// time stamp
	tstamp2 = (unsigned) fget_uint4( _fp );
	tstamp = (unsigned) fget_uint4( _fp );
	_timestamp[0] =((tstamp >> 26) & 0x0000003F) + _h.v3.ref_year; // year
	_timestamp[1] = (tstamp >> 22) & 0x0000000F; // month
	_timestamp[2] = (tstamp >> 17) & 0x0000001F; // day
	_timestamp[3] = (tstamp >> 12) & 0x0000001F; // hour
	_timestamp[4] = (tstamp >>  6) & 0x0000003F; // min
	_timestamp[5] = (tstamp      ) & 0x0000003F; // sec
	_timestamp[6] = (tstamp2 >> 22L) & 0x000003FF; // msec
	_timestamp[7] = (tstamp2 >> 12L) & 0x000003FF; // usec
	_timestamp[8] = (tstamp2 >>  2L) & 0x000003FF; // nsec
	_ts_len = 9;

	// data section index
	dsi[0] = (unsigned) fget_uint4( _fp );
	dsi[1] = (unsigned) fget_uint4( _fp );

	// event composition
	evc = (unsigned) fget_uint2( _fp );

	// trigger info
	_trig = (unsigned) fget_uint2( _fp );
	_trig_len = 1;

	// sw event number
	_swevno = (unsigned) fget_uint4( _fp );

	// obtain number of data section by counting EVC bits.
	nsec = 0;
	for(k = 0; k < 16; k++)
		nsec += (evc & (1L << k)) ? 1 : 0;
	// trigger bit should not be counted.
	if ( evc & FMT3_EVC_TRG ) nsec--;

	// data section
	len_check = len - 36;
	for(k = 0; k < nsec; k++) {
		if ( feof(_fp) ) return -1;

		if ( k < 8 )
			sec = (dsi[0] >> (4 * (7 - k))) & 0x0fL;
		else
			sec = (dsi[1] >> (4 * (15 - k))) & 0x0fL;

		switch ( sec ) {
		case FMT3_DSI_CMC:
			if ( (evc & FMT3_EVC_CMC) == 0 ) goto dsi_err;
			if ( read_camac() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CAL:
			if ( evc & FMT3_EVC_CAL == 0 ) goto dsi_err;
			if ( read_calorimeter() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TCRD:
			if ( evc & FMT3_EVC_TCRD == 0 ) goto dsi_err;
			if ( read_tcrd() ) return -1;
			len_check -= _nread;
			break; //YONAS
		case FMT3_DSI_SCD:
			if ( evc & FMT3_EVC_SCD == 0 ) goto dsi_err;
			if ( read_scd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_tSCD:
			if ( evc & FMT3_EVC_tSCD == 0 ) goto dsi_err;
			if ( read_tscd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_bSCD:
			if ( evc & FMT3_EVC_bSCD == 0 ) goto dsi_err;
			if ( read_bscd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_SBT:
			if ( evc & FMT3_EVC_SBT == 0 ) goto dsi_err;
			if ( read_sbt() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_HDS:
			if ( evc & FMT3_EVC_HDS == 0 ) goto dsi_err;
			if ( read_hodoscope() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TCD:
			if ( evc & FMT3_EVC_TCD == 0 ) goto dsi_err;
			if ( read_tcd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_BSD:
			if ( evc & FMT3_EVC_BSD == 0 ) goto dsi_err;
			if ( read_bsd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_TRD:
			if ( evc & FMT3_EVC_TRD == 0 ) goto dsi_err;
			if( read_trd2() ) return -1; // Angelaszek TRD
			//if ( read_trd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CVD:
			if ( evc & FMT3_EVC_CVD == 0 ) goto dsi_err;
			if ( read_cvd() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_CER:
			if ( evc & FMT3_EVC_CER == 0 ) goto dsi_err;
			if ( read_cer() ) return -1;
			len_check -= _nread;
			break;
		case FMT3_DSI_SCDL1:                                    //Added by David Angelaszek 9/1/2010
			if ( evc & FMT3_EVC_SCDL1 == 0 ) goto dsi_err;   //Added by David Angelaszek 9/1/2010
			if ( read_scdl1() ) return -1;                    //Added by David Angelaszek 9/1/2010
			len_check -= _nread;                              //Added by David Angelaszek 9/1/2010
			break;                                            //Added by David Angelaszek 9/1/2010
		case FMT3_DSI_SCDL2:                                      //Added by David Angelaszek 9/1/2010
			if ( evc & FMT3_EVC_SCDL2 == 0 ) goto dsi_err;    //Added by David Angelaszek 9/1/2010
			if ( read_scdl2() ) return -1;                    //Added by David Angelaszek 9/1/2010
			len_check -= _nread;                              //Added by David Angelaszek 9/1/2010
			break;  
		case FMT3_DSI_SCDL3:                                      //Added by David Angelaszek 9/1/2010
			if ( evc & FMT3_EVC_SCDL3 == 0 ) goto dsi_err;    //Added by David Angelaszek 9/1/2010
			if ( read_scdl3() ) return -1;                    //Added by David Angelaszek 9/1/2010
			len_check -= _nread;                              //Added by David Angelaszek 9/1/2010
			break;                                            //Added by David Angelaszek 9/1/2010*/
                                          

		}

	}
	if ( len_check != 0 ) {
		fprintf(stderr, "CreamFile8.0: inconsistent data length: nominal %d, actual %ld\n", len, len - len_check);
		fprintf(stderr, "CreamFile8.0: file repositioning by %ld byte(s)\n", len_check);
		fseek(_fp, len_check, SEEK_CUR); 
		// return -1;
	}

	return 0;

dsi_err:
	fprintf(stderr, "CreamFile8.0: corrupt data section info: evc %02X, dsi %08X\n", evc, dsi);
	fprintf(stderr, "CreamFile8.0: file repositioning by %ld byte(s)\n", len_check);
	fseek(_fp, len_check, SEEK_CUR); 
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
// For current version
//////////////////////////////////////////////////////////////////////////////

CreamFile::CreamFile()
	{
	_fp = 0;
	_nread = 0;
}

CreamFile::~CreamFile()
	{
	if ( _fp ) { fclose( _fp ); _fp = 0; }
}

int CreamFile::OpenWrite(char* filename)
	{
	if ( filename == 0 || *filename == 0 ) {
		fprintf(stderr, "null filename.\n");
		return -1;
	}

	if ( _fp ) fclose( _fp );

	if ( (_fp = fopen(filename, "w")) == 0 ) {
		fprintf(stderr, "cannot open file %s for writing.\n", filename);
		return -1;
	}

	init_header();
	init_tail();

	return 0;
}

int CreamFile::OpenRead(char* filename)
	{
	if ( filename == 0 || *filename == 0 ) {
		fprintf(stderr, "null filename.\n");
		return -1;
	}

	if ( _fp ) fclose( _fp );

	if ( (_fp = fopen(filename, "r")) == 0 ) {
		fprintf(stderr, "cannot open file %s for reading.\n", filename);
		return -1;
	}
	else {
		/* get file size */
		struct stat buf;
		if ( fstat(fileno(_fp), &buf) == 0 )
			_filesz = buf.st_size;
		else {
			fprintf(stderr, "cannot obtain file size; setting to 100.\n");
			_filesz = 100;
		}

		/* read header and check header information */
		if ( read_header(_fp) ) {
			fprintf(stderr, "cannot read header info; file closed.\n");
			fclose( _fp ); _fp = 0;
			return -1;
		}
	}

	return 0;
}

int CreamFile::Close(void)
	{
	if ( _fp ) {
		fclose( _fp ); _fp = 0;
	}

	return 0;
}

long CreamFile::GetCurrentFilePosition(void)
	{
	return _fp ? ftell( _fp ) : 0;
}

// Rewind file pointer right after the run header so that
// subsequent call to  ReadEvent()  would read the first event data.
int CreamFile::Rewind(void)
	{
	if ( _fp ) {
		switch ( GetFormatVersion() ) {
		case 0x0000:
			fseek(_fp, CREAMFILE_RUN_HEADER_LENGTH_V0, SEEK_SET);
			break;
		case 0x0100:
		case 0x0101:
			fseek(_fp, CREAMFILE_RUN_HEADER_LENGTH_V1, SEEK_SET);
			break;
		case 0x0200:
			fseek(_fp, CREAMFILE_RUN_HEADER_LENGTH_V2, SEEK_SET);
			break;
		case 0x0300:
		case 0x0301:
		case 0x0302:
		case 0x0303:
		case 0x0800:
			fseek(_fp, CREAMFILE_RUN_HEADER_LENGTH_V3, SEEK_SET);
			break;
		default:
			fprintf(stderr, "crmfile::Rewind: unknown format.\n");
			return -1;
		}
	}

	return 0;
}

/* read an event */
int CreamFile::ReadEvent(DEvent& ev)
	{



	if ( _fp == 0 ) return -1;



	/* select reader function */
	switch ( GetFormatVersion() ) {
	case 0x0000:
		if ( cream_read_0_0() ) return -1;
		break;
	case 0x0100:
		if ( cream_read_1_0() ) return -1;
		break;
	case 0x0101:
		if ( cream_read_1_1() ) return -1;
		break;
	case 0x0200:
		if ( cream_read_2_0() ) return -1;
		break;
	case 0x0300:
		if ( cream_read_3_0() ) return -1;
		break;
	case 0x0301:
	case 0x0302: // 3.1 and 3.2 differ in the header.
		if ( cream_read_3_1() ) return -1;
		break;
	case 0x0303:
		if ( cream_read_3_3() ) return -1;
		break;
	case 0x0800:
		if ( cream_read_8_0() ) return -1;
		break;
	default:
		fprintf(stderr, "crmfile::ReadEvent: unknown format.\n");
		return -1;
	}

	ev.Clear();
	ev.SetType( _type );
	ev.SetByteLength( _raw_nbytes );
	

	
	if ( _trig_len  ) ev.UnpackTrigger( _trig, _trig_len );
	if ( _camac_len ) ev.UnpackCAMAC( _camac_cbuf, _camac_len );
	if ( _cal_len   ) ev.UnpackCalorimeter( _cal_cbuf, _cal_len );
	if ( _tcrd_len   ) ev.UnpackTCRD( _tcrd_cbuf, _tcrd_len ); //YONAS
	if ( _hds_len   ) ev.UnpackHodoscope( _hds_cbuf, _hds_len );
	if ( _scd_len   ) ev.UnpackSCD( _scd_cbuf, _scd_len );
	//if ( _tcd_len   ) ev.UnpackTCD( _tcd_cbuf, _tcd_len );
	if ( _bsd_len   ) ev.UnpackBSD( _bsd_cbuf, _bsd_len );
	//if ( _cvd_len   ) ev.UnpackCVD( _cvd_cbuf, _cvd_len );
	if ( _cer_len   ) ev.UnpackCER( _cer_cbuf, _cer_len );
	if ( _trd_len   ) ev.UnpackTRD2( _trd_cbuf, _trd_len ); //Angelaszek TRD
	//if ( _trd_len   ) ev.UnpackTRD( _trd_cbuf, _trd_len );
	if ( _ts_len    ) ev.UnpackTimeStamp( _timestamp, _ts_len );
	if ( _tscd_len   ) ev.UnpackTopSCD( _tscd_cbuf, _tscd_len );       
	if ( _bscd_len   ) ev.UnpackBottomSCD( _bscd_cbuf, _bscd_len );    
	if ( _sbt_len   ) ev.UnpackSiliconBeamTracker( _sbt_cbuf, _sbt_len );  
	if ( _scdl1_len   ) ev.UnpackSCDL1( _scdl1_cbuf, _scdl1_len );     //Added by David Angelaszek 9/1/2010
	if ( _scdl2_len   ) ev.UnpackSCDL2( _scdl2_cbuf, _scdl2_len );  //Added by David Angelaszek 9/1/2010*/
	if ( _scdl3_len   ) ev.UnpackSCDL3( _scdl3_cbuf, _scdl3_len );  //Added by David Angelaszek 9/1/2010*/
	if ( GetFormatVersion() >= 0x0301 ) {
		ev.SetSWEventNumber( _swevno );
		ev.SetChecksum( _crc16 );
	}
	

	return 0;
}
