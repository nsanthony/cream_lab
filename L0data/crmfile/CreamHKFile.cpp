// CreamHKFile.cpp
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
// $Log: CreamHKFile.cpp,v $
// Revision 1.3  2012/11/16 21:11:55  dja1987
// Fixes to TCD/BCD crmfile. Updated HK format. Improved client playback (no more temporary arrays for missing packet numbers). Updated cprint
//
// Revision 1.2  2012/03/21 20:41:34  dja1987
// T/BCD updates.
//
// Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
// Importing SCDL bug fixes.
//
// Revision 1.2  2011/07/20 17:22:27  dja1987
// SCDL housekeeping code added
//
// Revision 1.1.1.1  2011/06/07 19:10:21  dja1987
// Includes both TRD and SCDL updates
//
// Revision 1.2  2010/07/10 14:40:12  spaced4
// CAL HV2a HV2b
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
// Revision 1.33  2007/04/02 17:06:51  juneki
// Add tcd hv monitor
//
// Revision 1.32  2007/02/21 00:10:31  juneki
// added CER HK channels
//
// Revision 1.31  2006/08/29 20:00:03  syzinn
// shows differential live/total times
//
// Revision 1.30  2006/08/25 19:46:38  syzinn
// different timer resolution for format 5; total/live times are stored in min.sec format
//
// Revision 1.29  2006/08/25 15:40:22  syzinn
// raw trigger counts
//
// Revision 1.28  2006/06/29 19:52:09  juneki
// *** empty log message ***
//
// Revision 1.27  2006/06/29 17:50:55  juneki
// cer hk channel assigned
//
// Revision 1.26  2006/06/27 20:28:04  juneki
// support cer sparsification with GUI
//
// Revision 1.25  2006/03/24 14:42:37  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.24.2.3  2006/03/23 23:12:09  syzinn
// unpack_fmt_5_full() and unpack_fmt_2_reduced() now assumes the data pointer starts from the beginning of an event
//
// Revision 1.24.2.2  2006/03/23 22:24:50  syzinn
// preventing an infinite loop
//
// Revision 1.24.2.1  2006/03/14 21:07:38  syzinn
// eight-byte timestamp
//
// Revision 1.24  2006/01/13 22:11:31  syzinn
// fixed latitude and longitude
//
// Revision 1.23  2005/11/24 02:32:45  syzinn
// Fixed channel mapping to prevent overwriting valid channels by spare; conversion of total and live times in minutes
//
// Revision 1.22  2005/11/07 19:14:51  cvs
// cerenkov camera support
//
// Revision 1.21  2005/10/28 21:05:44  cvs
// total and live times use min.
//
// Revision 1.20.2.1  2005/09/22 20:16:29  cvs
// added section for Cerenkov Camera
//
// Revision 1.20  2005/08/26 17:29:29  cvs
// fixed latitude and longitude conversion
//
// Revision 1.19  2005/08/23 21:35:35  cvs
// corrected longitute and latitude
//
// Revision 1.18  2005/08/12 21:12:40  cvs
// added CVD HV monitors
//
// Revision 1.17  2005/08/08 22:56:51  syzinn
// backward compatibility
//
// Revision 1.16  2005/07/16 00:35:54  cvs
// bug fix
//
// Revision 1.15  2005/07/16 00:34:14  cvs
// converts live and total time to seconds
//
// Revision 1.14  2005/07/12 20:30:24  cvs
// bug fix in unpacking reduced hk event
//
// Revision 1.13  2005/07/12 20:10:36  cvs
// unpack reduced hk event of fmt 1
//
// Revision 1.12  2005/07/06 21:17:25  cvs
// support hk file ver 3
//
// Revision 1.11  2005/06/16 21:16:25  cvs
// reduced dual SCD buffer to fit
//
// Revision 1.10  2005/06/16 20:44:39  cvs
// select mode to collect hk data from dual SCDs
//
// Revision 1.9  2005/06/11 12:18:01  cvs
// bug fix in hk system
//
// Revision 1.8  2005/06/10 20:33:11  cvs
// minor change in format 4.0
//
// Revision 1.7  2005/06/10 19:52:16  cvs
// new master trigger
//
// Revision 1.6  2005/06/10 17:15:42  cvs
// included trigger timestamp
//
// Revision 1.5  2005/06/10 17:10:42  cvs
// added flags for buffers
//
// Revision 1.4  2005/06/07 12:22:51  cvs
// first working version for new hk system
//
// Revision 1.3  2005/06/07 00:22:08  cvs
// this is enough for today
//
// Revision 1.2  2005/06/07 00:16:15  cvs
// still under modification
//
// Revision 1.1  2005/06/06 21:30:53  cvs
// new
//

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> // strncmp()
#include "defs.h"
#include "crmevtx.h"
#include "DHKEventConverter.h"
#include "CreamHKFile.h"
#ifdef MEMLEAK
#include "memleak.h"
#endif
#include "fio.c"

int CreamHKFile::clear()
	{
	_nread = 0; _crc16 = 0;
	_trg_flag = 0;
	_sw_flag = 0;
	_gps_flag = 0;
	_mtt_flag = 0;
	_cal_flag = 0;
	_tcrd_flag = 0; //YONAS
	_hds_flag = 0;
	_scd_flag = 0;
	_tscd_flag = 0;
	_bscd_flag = 0;
	_trd_flag = 0;
	_tcd_flag = 0;
	_bsd_flag = 0;
	_inst_flag = 0;
	_dscd_flag = 0;
	_cer_flag = 0;
	_scdl1_flag = 0; // Added by David Angelaszek 9/1/2010
	_scdl2_flag = 0; // Added by David Angelaszek 9/1/2010
	_scdl3_flag = 0; // Added by David Angelaszek 9/1/2010
	return 0;
}


// channels for formats earlier than Ver 4.0
#define HKCH_CNT	386	/* number of channels produced by old HKS */
#define HKCH_BYTES	579	/* number of bytes of packed 12-bit signed integers */

struct hk_allch {
	uint2		adc[HKCH_CNT]; /* 12-bit integer values */
};

double fix_latitude(double a)
	{
	int h;
	h = (int) (a * 0.01);
	return (h + (a - h * 100) / 60.);
}

double fix_longitude(double a)
	{
	return fix_latitude( a );
}

double convert_timer(char* pbuf, int len, double cf)
	{
	uint8 a, sum;
	int k;

	sum = 0;
	for(k = 0; k < len; k++) {
		a = pbuf[k] & 0x00ff;
		sum += a << ((len - 1 - k) * 8LL);
	}
	
	return (cf * sum); // in minute
}

// eg, 5.50 to 5.30
double format_min_sec(double v)
	{
	double fix;

	// change to minute.second
	fix = (int) v;
	fix += (v - fix) * 60 / 100;
	return fix;
}

//////////////////////////////////////////////////////////////////////////////
// For format version 0.xx
//////////////////////////////////////////////////////////////////////////////

int CreamHKFile::read_fmt_0()
	{
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
// For format version 1.0 and version 2.0
//////////////////////////////////////////////////////////////////////////////

// structure for format version 1.0 and 2.0
// the same structure, but different channel assignment
struct hk_fmt1 {
	timestamp4_t	ts;
	timestamp4_t	nio_ts;
	real4		nio_tkbps;
	real4		nio_nkbps;
	timestamp4_t	gps_ts;
	gps_t		gps;
	timestamp4_t	trg_ts;
	real4		trg_tot;
	real4		trg_clb;
	real4		trg_zhi;
	real4		trg_cal;
	real4		trg_tcrd; //YONAS
	real4		trg_ext;
	char		total[6];
	char		live[6];
	char		cbuf[579];
};

int CreamHKFile::unpack_fmt_1_full(unsigned char* p, DHKEventConverter& ev)
	{
	int k;
	static hk_fmt1* fmt1_ptr;
	static hk_allch allch;
	// note that spare channels are mapped to -1.
	static int map1to4[] = {
		90, 82, 74, 91, 83, 75, 92, 84, 76, 93, // 0 ...
		85, 77, 94, 86, 78, 95, 87, 79, 96, 88, // 10 ...
		80, 97, 89, 81, 182, 170, 134, 146, 158, 183, // 20 ...
		171, 135, 147, 159, 184, 172, 136, 148, 160, 185, // 30 ...
		173, 137, 149, 161, 186, 174, 138, 150, 162, 187, // 40 ...
		175, 139, 151, 163, 188, 176, 140, 152, 164, 189, // 50 ...
		177, 141, 153, 165, 190, 178, 142, 154, 166, 191, // 60 ...
		179, 143, 155, 167, 192, 180, 144, 156, 168, 193, // 70 ...
		181, 145, 157, 169, -1, -1, -1, -1, 64, 62, // 80 ...
		106, 98, 114, 107, 99, 115, 108, 100, 116, 109, // 90 ...
		101, 117, 110, 102, 118, 111, 103, 119, 112, 104, // 100 ...
		120, -1, 63, 65, 113, 105, 121, 67, 66, 68, // 110 ...
		69, 373, 372, -1, -1, -1, -1, -1, 124, 122, // 120 ...
		206, 194, 218, 207, 195, 219, 208, 196, 220, 209,
		197, 221, 210, 198, 222, 211, 199, 223, 212, 200,
		224, -1, 123, 125, 213, 201, 225, 214, 202, 226, // 150 ..
		215, 203, 227, 216, 204, 228, 217, 205, 229, -1, // 160
		-1, -1, -1, -1, -1, -1, 232, 230, 236, 234,
		238, 237, 235, 239, 231, 233, -1, -1, -1, -1,
		-1, -1, 388, 389, 390, 391, 392, 393, 394, 395, // 190
		396, 397, 398, 399, 400, 401, 402, 403, 404, 405,
		406, -1, -1, -1, -1, -1, 412, 411, 410, 408, // 210
		409, 407, 413, 414, 415, 416, 417, -1, -1, -1, // 220
		-1, -1, 434, 423, 425, 429, 432, 430, -1, -1, // 230
		426, 424, 428, 431, 427, 433, -1, -1, -1, -1, // 240
		-1, -1, -1, -1, -1, -1, 346, 320, 294, 268, // 250
		242, 347, 321, 295, 269, 243, 348, 322, 296, 270,
		244, 349, 323, 297, 271, 245, 350, 324, 298, 272,
		246, 351, 325, 299, 273, 247, 352, 326, 300, 274,
		248, 353, 327, 301, 275, 249, 354, 328, 302, 276,
		250, 355, 329, 303, 277, 251, 356, 330, 304, 278,
		252, 357, 331, 305, 279, 253, 358, 332, 306, 280,
		254, 359, 333, 307, 281, 255, 360, 334, 308, 282,
		256, 361, 335, 309, 283, 257, 362, 336, 310, 284,
		258, 363, 337, 311, 285, 259, 364, 338, 312, 286,
		260, 365, 339, 313, 287, 261, 366, 340, 314, 288,
		262, 367, 341, 315, 289, 263, 368, 342, 316, 290,
		264, 369, 343, 317, 291, 265, 370, 344, 318, 292,
		266, 371, 345, 319, 293, 267};
	// note that spare channels are mapped to -1.
	static int map2to4[] = {
		62, 64, 106, 114, 98, 107, 115, 99, 108, 116,
		100, 109, 117, 101, 110, 118, 102, 111, 119, 103,
		112, 120, 104, -1, 65, 63, 113, 121, 105, 67,
		68, 66, 69, 373, 372, -1, -1, -1, -1, -1,
		122, 124, 206, 218, 194, 207, 219, 195, 208, 220,
		196, 209, 221, 197, 210, 222, 198, 211, 223, 199,
		212, 224, 200, -1, 125, 123, 213, 225, 201, 214,
		226, 202, 215, 227, 203, 216, 228, 204, 217, 229,
		205, 382, 385, 383, 386, 384, 387, -1, 396, 397,
		398, 399, 400, 401, 402, 403, 404, 405, 406, 388,
		389, 390, 391, 392, 393, 394, 395, -1, -1, 412,
		409, 407, 408, 410, 411, 413, 414, 415, 416, 417,
		78, 86, 94, 79, 87, 95, -1, -1, 74, 82,
		90, 75, 83, 91, -1, -1, 80, 88, 96, 81,
		89, 97, -1, -1, 76, 84, 92, 77, 85, 93,
		-1, -1, 134, 146, 170, 182, 158, 135, 147, 171,
		183, 159, 136, 148, 172, 184, 160, -1, 137, 149,
		173, 185, 161, -1, -1, -1, 141, 153, 177, 189,
		165, 142, 154, 178, 190, 166, 143, 155, 179, 191,
		167, -1, 138, 150, 174, 186, 162, 139, 151, 175,
		187, 163, 140, 152, 176, 188, 164, -1, 144, 156,
		180, 192, 168, 145, 157, 181, 193, 169, 238, 232,
		230, 236, 234, -1, 239, 231, 233, 237, 235, -1,
		-1, -1, 434, 423, 425, 429, 432, 430, -1, -1,
		426, 424, 428, 431, 427, 433, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, 346, 320, 294, 268,
		242, 347, 321, 295, 269, 243, 348, 322, 296, 270,
		244, 349, 323, 297, 271, 245, 350, 324, 298, 272,
		246, 351, 325, 299, 273, 247, 352, 326, 300, 274,
		248, 353, 327, 301, 275, 249, 354, 328, 302, 276,
		250, 355, 329, 303, 277, 251, 356, 330, 304, 278,
		252, 357, 331, 305, 279, 253, 358, 332, 306, 280,
		254, 359, 333, 307, 281, 255, 360, 334, 308, 282,
		256, 361, 335, 309, 283, 257, 362, 336, 310, 284,
		258, 363, 337, 311, 285, 259, 364, 338, 312, 286,
		260, 365, 339, 313, 287, 261, 366, 340, 314, 288,
		262, 367, 341, 315, 289, 263, 368, 342, 316, 290,
		264, 369, 343, 317, 291, 265, 370, 344, 318, 292,
		266, 371, 345, 319, 293, 267};
	
	// type, fmt and length are already done.
	p += 4;

	fmt1_ptr = (hk_fmt1 *) p; // copy
	
	// event timestamp
	ev.SetTimestamp(0, (timestamp4_t *) p);

	// network i/o rates
	ev.SetTimestamp(20, fmt1_ptr->nio_ts);
	ev.SetReal(21, fmt1_ptr->nio_tkbps);
	ev.SetReal(22, fmt1_ptr->nio_nkbps);

	// gps
	ev.SetTimestamp(42, fmt1_ptr->gps_ts);
	ev.SetInteger(43, fmt1_ptr->gps.pos_hr);
	ev.SetInteger(44, fmt1_ptr->gps.pos_min);
	ev.SetReal(45, fmt1_ptr->gps.pos_sec);
	ev.SetReal(46, fix_latitude(fmt1_ptr->gps.latitude));
	ev.SetReal(47, fix_longitude(fmt1_ptr->gps.longitude));
	ev.SetReal(48, fmt1_ptr->gps.altitude);
	ev.SetReal(49, fmt1_ptr->gps.vvelocity);
	ev.SetReal(50, fmt1_ptr->gps.heading);
	ev.SetReal(51, fmt1_ptr->gps.pitch);
	ev.SetReal(52, fmt1_ptr->gps.roll);
	ev.SetReal(53, fmt1_ptr->gps.course);
	ev.SetReal(54, fmt1_ptr->gps.speed);
	ev.SetReal(55, fmt1_ptr->gps.pdop);
	ev.SetReal(56, fmt1_ptr->gps.phase_rms);
	ev.SetReal(57, fmt1_ptr->gps.baseline_rms);
	ev.SetInteger(58, fmt1_ptr->gps.nsatellite);
	ev.SetInteger(59, fmt1_ptr->gps.reset_flag);

	// trigger
	ev.SetTimestamp(10, fmt1_ptr->trg_ts);
	ev.SetReal(11, fmt1_ptr->trg_tot);
	ev.SetReal(13, fmt1_ptr->trg_clb);
	ev.SetReal(17, fmt1_ptr->trg_zhi);
	ev.SetReal(18, fmt1_ptr->trg_cal);
	ev.SetReal(12, fmt1_ptr->trg_ext);
	
	// master trigger timers
	{
		uint8 a, sum;

		sum = 0;
		for(k = 0; k < 6; k++) {
			a = ((unsigned char) fmt1_ptr->total[k]) & 0x00ff;
			sum += a << ((5-k)*8LL);
		}
		//ev.SetLongInteger(60, sum);
		ev.SetDouble(60, sum * _tmconv_fmt1);

		sum = 0;
		for(k = 0; k < 6; k++) {
			a = ((unsigned char) fmt1_ptr->live[k]) & 0x00ff;
			sum += a << ((5-k)*8LL);
		}
		//ev.SetLongInteger(61, sum);
		ev.SetDouble(61, sum * _tmconv_fmt1);
	}
	
	// psrc = fmt1_ptr->cbuf, pdest = &allch
	// nbytes = 579, number of channels (12-bit integers) = 386
	p = (unsigned char *) fmt1_ptr->cbuf;
	unpack_12bit_array(p, &allch, HKCH_BYTES);
	
	// ev.SetADC(map[k], allch.adc[k]); k=ch_old, map[k]=ch_new
	for (k = 0; k < HKCH_CNT; k++) {
		/* select channel assignment map */
		switch ( GetFormatVersion() ) {
		case 0x0100:
			if ( map1to4[k] >= 0 )
				ev.SetADC(map1to4[k], allch.adc[k]);
			break;
		case 0x0200:
			if ( map2to4[k] >= 0 )
				ev.SetADC(map2to4[k], allch.adc[k]);
			break;
		}
	}

	ev.Convert();
	ev.CheckStatus();
	return 0;
}

int CreamHKFile::read_fmt_1(DHKEventConverter& ev)
	{
	int c;
	struct _hdr {
		unsigned char type;
		unsigned char fmt;
		unsigned short length;
	} *p;

	/* check EOF */
	if ( (c = fgetc(_fp)) == EOF ) return -1;
	else ungetc(c, _fp);

	/* read four bytes to get type, format and length */
	fread(ev.GetEventBufferPtr(), sizeof(char), 4, _fp);
	p = (struct _hdr *) ev.GetEventBufferPtr();
	fread(ev.GetEventBufferPtr() + 4, sizeof(char), p->length - 4, _fp);
	ev.SetEventType( p->type );
	ev.SetEventFormat( p->fmt );
	ev.SetEventLength( p->length );

	switch ( p->type ) {
	case DAT_HK: // full hk
		unpack_fmt_1_full((unsigned char *) ev.GetEventBufferPtr(), ev);
		break;
	case DAT_HK2: // reduced hk
		unpack_fmt_1_reduced((unsigned char *) ev.GetEventBufferPtr(), ev);
		break;
	case DAT_TCF: // TCD config
		// nothing todo
		break;
	default:
		fprintf(stderr, "File contains events non-related to housekeeping.\n");
		return -1;
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// For format version 3.xx
//////////////////////////////////////////////////////////////////////////////

// This format appends buffer fill levels and padding to format 2.0.
// After format 3.3, it also appends disk space, serial number and code for the last command.
struct hk_fmt3 {
	timestamp4_t	ts;
	timestamp4_t	nio_ts;
	real4		nio_tkbps;
	real4		nio_nkbps;
	timestamp4_t	gps_ts;
	gps_t		gps;
	timestamp4_t	trg_ts;
	real4		trg_tot;
	real4		trg_clb;
	real4		trg_zhi;
	real4		trg_cal;
	real4		trg_ext;
	char		total[6];
	char		live[6];
	char		cbuf[579];
	uint2		bufl_cmdq;
	uint2		bufl_hkq;
	uint2		bufl_evtq;
	uint2		bufl_datq;
	uint2		bufl_datq2;
	uint2		bufl_pkt0;
	uint2		bufl_pkt1;
	uint2		disk_space;
	uint2		sys_disk_space;//PGB 15 July 2008
	uint2		cmd_ser;
	uint2		cmd_code;
};

int CreamHKFile::unpack_fmt_3_full(unsigned char* p, DHKEventConverter& ev)
	{
	int k;
	static hk_fmt3* fmt3_ptr;
	static hk_allch allch;
	// note that spare channels are mapped to -1.
	static int map[] = {
		62, 64, 106, 114, 98, 107, 115, 99, 108, 116,
		100, 109, 117, 101, 110, 118, 102, 111, 119, 103,
		112, 120, 104, -1, 65, 63, 113, 121, 105, 67,
		68, 66, 69, 373, 372, -1, -1, -1, -1, -1,
		122, 124, 206, 218, 194, 207, 219, 195, 208, 220,
		196, 209, 221, 197, 210, 222, 198, 211, 223, 199,
		212, 224, 200, -1, 125, 123, 213, 225, 201, 214,
		226, 202, 215, 227, 203, 216, 228, 204, 217, 229,
		205, 382, 385, 383, 386, 384, 387, -1, 396, 397,
		398, 399, 400, 401, 402, 403, 404, 405, 406, 388,
		389, 390, 391, 392, 393, 394, 395, -1, -1, 412,
		409, 407, 408, 410, 411, 413, 414, 415, 416, 417,
		78, 86, 94, 79, 87, 95, -1, -1, 74, 82,
		90, 75, 83, 91, -1, -1, 80, 88, 96, 81,
		89, 97, -1, -1, 76, 84, 92, 77, 85, 93,
		-1, -1, 134, 146, 170, 182, 158, 135, 147, 171,
		183, 159, 136, 148, 172, 184, 160, -1, 137, 149,
		173, 185, 161, -1, -1, -1, 141, 153, 177, 189,
		165, 142, 154, 178, 190, 166, 143, 155, 179, 191,
		167, -1, 138, 150, 174, 186, 162, 139, 151, 175,
		187, 163, 140, 152, 176, 188, 164, -1, 144, 156,
		180, 192, 168, 145, 157, 181, 193, 169, 238, 232,
		230, 236, 234, -1, 239, 231, 233, 237, 235, -1,
		-1, -1, 434, 423, 425, 429, 432, 430, -1, -1,
		426, 424, 428, 431, 427, 433, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, 346, 320, 294, 268,
		242, 347, 321, 295, 269, 243, 348, 322, 296, 270,
		244, 349, 323, 297, 271, 245, 350, 324, 298, 272,
		246, 351, 325, 299, 273, 247, 352, 326, 300, 274,
		248, 353, 327, 301, 275, 249, 354, 328, 302, 276,
		250, 355, 329, 303, 277, 251, 356, 330, 304, 278,
		252, 357, 331, 305, 279, 253, 358, 332, 306, 280,
		254, 359, 333, 307, 281, 255, 360, 334, 308, 282,
		256, 361, 335, 309, 283, 257, 362, 336, 310, 284,
		258, 363, 337, 311, 285, 259, 364, 338, 312, 286,
		260, 365, 339, 313, 287, 261, 366, 340, 314, 288,
		262, 367, 341, 315, 289, 263, 368, 342, 316, 290,
		264, 369, 343, 317, 291, 265, 370, 344, 318, 292,
		266, 371, 345, 319, 293, 267};
	
	// length, type, and fmt is already done.
	p += 4;

	fmt3_ptr = (hk_fmt3 *) p; // copy
	
	// event timestamp
	ev.SetTimestamp(0, (timestamp4_t *) p);

	// network i/o rates
	ev.SetTimestamp(20, fmt3_ptr->nio_ts);
	ev.SetReal(21, fmt3_ptr->nio_tkbps);
	ev.SetReal(22, fmt3_ptr->nio_nkbps);

	// gps
	ev.SetTimestamp(42, fmt3_ptr->gps_ts);
	ev.SetInteger(43, fmt3_ptr->gps.pos_hr);
	ev.SetInteger(44, fmt3_ptr->gps.pos_min);
	ev.SetReal(45, fmt3_ptr->gps.pos_sec);
	ev.SetReal(46, fix_latitude(fmt3_ptr->gps.latitude));
	ev.SetReal(47, fix_longitude(fmt3_ptr->gps.longitude));
	ev.SetReal(48, fmt3_ptr->gps.altitude);
	ev.SetReal(49, fmt3_ptr->gps.vvelocity);
	ev.SetReal(50, fmt3_ptr->gps.heading);
	ev.SetReal(51, fmt3_ptr->gps.pitch);
	ev.SetReal(52, fmt3_ptr->gps.roll);
	ev.SetReal(53, fmt3_ptr->gps.course);
	ev.SetReal(54, fmt3_ptr->gps.speed);
	ev.SetReal(55, fmt3_ptr->gps.pdop);
	ev.SetReal(56, fmt3_ptr->gps.phase_rms);
	ev.SetReal(57, fmt3_ptr->gps.baseline_rms);
	ev.SetInteger(58, fmt3_ptr->gps.nsatellite);
	ev.SetInteger(59, fmt3_ptr->gps.reset_flag);

	// trigger
	ev.SetTimestamp(10, fmt3_ptr->trg_ts);
	ev.SetReal(11, fmt3_ptr->trg_tot);
	ev.SetReal(13, fmt3_ptr->trg_clb);
	ev.SetReal(17, fmt3_ptr->trg_zhi);
	ev.SetReal(18, fmt3_ptr->trg_cal);
	ev.SetReal(12, fmt3_ptr->trg_ext);
	
	// master trigger timers
	{
		uint8 a, sum;

		sum = 0;
		for(k = 0; k < 6; k++) {
			a = ((unsigned char) fmt3_ptr->total[k]) & 0x00ff;
			sum += a << ((5-k)*8LL);
		}
		//ev.SetLongInteger(60, sum);
		ev.SetDouble(60, sum * _tmconv_fmt1);

		sum = 0;
		for(k = 0; k < 6; k++) {
			a = ((unsigned char) fmt3_ptr->live[k]) & 0x00ff;
			sum += a << ((5-k)*8LL);
		}
		//ev.SetLongInteger(61, sum);
		ev.SetDouble(61, sum * _tmconv_fmt1);
	}
	
	// psrc = fmt3_ptr->cbuf, pdest = &allch
	// nbytes = 579, number of channels (12-bit integers) = 386
	p = (unsigned char *) fmt3_ptr->cbuf;
	unpack_12bit_array(p, &allch, HKCH_BYTES);
	
	// ev.SetADC(map[k], allch.adc[k]); k=ch_old, map[k]=ch_new
	for (k = 0; k < HKCH_CNT; k++) {
		if ( map[k] >= 0 )
			ev.SetADC(map[k], allch.adc[k]);
	}

	// buffer fill level	
	ev.SetReal(23, fmt3_ptr->bufl_cmdq / 256.);
	ev.SetReal(24, fmt3_ptr->bufl_hkq / 256.);
	ev.SetReal(25, fmt3_ptr->bufl_evtq / 256.);
	ev.SetReal(26, fmt3_ptr->bufl_datq / 256.);
	ev.SetReal(27, fmt3_ptr->bufl_datq2 / 256.);
	ev.SetReal(28, fmt3_ptr->bufl_pkt0 / 256.);
	ev.SetReal(29, fmt3_ptr->bufl_pkt1 / 256.);
	
	// disk space
	ev.SetReal(30, fmt3_ptr->disk_space / 256.);
	ev.SetReal( 34, fmt3_ptr->sys_disk_space / 256.); //PGB 15 July 2008

	// serial number and code of last command
	ev.SetInteger(31, fmt3_ptr->cmd_ser);
	ev.SetInteger(32, fmt3_ptr->cmd_code);
	
	ev.Convert();
	ev.CheckStatus();
	return 0;
}

int CreamHKFile::unpack_fmt_3_reduced(unsigned char* p, DHKEventConverter& ev)
	{
	return -1;
}

int CreamHKFile::read_fmt_3_typefirst(DHKEventConverter& ev)
	{
	int c;
	struct _hdr {
		unsigned char type;
		unsigned char fmt;
		unsigned short length;
	} *p;

	/* check EOF */
	if ( (c = fgetc(_fp)) == EOF ) return -1;
	else ungetc(c, _fp);

	/* read four bytes to get type, format and length */
	fread(ev.GetEventBufferPtr(), sizeof(char), 4, _fp);
	p = (struct _hdr *) ev.GetEventBufferPtr();
	fread(ev.GetEventBufferPtr() + 4, sizeof(char), p->length - 4, _fp);
	ev.SetEventType( p->type );
	ev.SetEventFormat( p->fmt );
	ev.SetEventLength( p->length );

	switch ( p->type ) {
	case DAT_HK: // full hk
		unpack_fmt_3_full((unsigned char *) ev.GetEventBufferPtr(), ev);
		break;
	case DAT_HK2: // reduced hk
		unpack_fmt_3_reduced((unsigned char *) ev.GetEventBufferPtr(), ev);
		break;
	case DAT_TCF: // TCD config
		// nothing todo
		break;
	default:
		fprintf(stderr, "File contains events non-related to housekeeping.\n");
		return -1;
		break;
	}
	return 0;
}

int CreamHKFile::read_fmt_3_lengthfirst(DHKEventConverter& ev)
	{
	int c;
	struct _hdr {
		unsigned short length;
		unsigned char type;
		unsigned char fmt;
	} *p;

	/* check EOF */
	if ( (c = fgetc(_fp)) == EOF ) return -1;
	else ungetc(c, _fp);

	/* read four bytes to get length, type, and format */
	fread(ev.GetEventBufferPtr(), sizeof(char), 4, _fp);
	p = (struct _hdr *) ev.GetEventBufferPtr();
	fread(ev.GetEventBufferPtr() + 4, sizeof(char), p->length - 4, _fp);
	ev.SetEventLength( p->length );
	ev.SetEventType( p->type );
	ev.SetEventFormat( p->fmt );

	switch ( p->type ) {
	case DAT_HK: // full hk
		unpack_fmt_3_full((unsigned char *) ev.GetEventBufferPtr(), ev);
		break;
	case DAT_HK2: // reduced hk
		unpack_fmt_3_reduced((unsigned char *) ev.GetEventBufferPtr(), ev);
		break;
	case DAT_TCF: // TCD config
		// nothing todo
		break;
	default:
		fprintf(stderr, "File contains events non-related to housekeeping.\n");
		return -1;
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// For format version 4.xx
//////////////////////////////////////////////////////////////////////////////
int CreamHKFile::unpack_12bit_array(unsigned char *psrc, void* pdest, int nbytes)
	{
	uint2* p = (uint2 *) pdest;
	int status = 0;
	int j = 0;

	// AB CD EF
	for(int k = 0; k < nbytes; k++) {
		switch ( status ) {
		case 0: p[j] = psrc[k] << 4L; status = 1; break; // AB0
		case 1: p[j++] += psrc[k] >> 4L; p[j] = (psrc[k] & 0x0f) << 8L; status = 2; break; // AB0 + 00C, D00
		case 2: p[j++] += psrc[k]; status = 0; break; // D00 + 0EF
		}
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////////
// For ISS-CREAM
//////////////////////////////////////////////////////////////////////////////
int CreamHKFile::unpack_16bit_array(unsigned char *psrc, void* pdest, int nbytes)
{
	uint2* p = (uint2 *) pdest;
	int j = 0;
	uint2 val;
	
	for(int k = 0; k < nbytes; k = k+2) {
		val = (psrc[k] << 8L) + psrc[k+1];
		p[j] = val;
		j++;
	}
	return 0;
}

int CreamHKFile::unpack_fmt_4_full(unsigned char* p, DHKEventConverter& ev)
	{
	int sec, sec_len, k;
	static hk_trg* trg_ptr;
	static hk_sw* sw_ptr;
	static hk_gps* gps_ptr;
	static hk_mtt* mtt_ptr;
	static hk_cal cal;
	static hk_hds hds;
	static hk_scd scd;
	static hk_tscd tscd;
	static hk_bscd bscd;
	static hk_tcd tcd;
	static hk_trd trd;
	static hk_inst inst;
	static hk_dscd dscd;
	static hk_cer cer;


	// length, type, and fmt is already done.
	p += 4;

	// event timestamp
	ev.SetTimestamp(0, (timestamp4_t *) p);
	p += sizeof(timestamp4_t);

	// skip serial number and reserved field.
	p += 4; 

	// go over sections
	while ( 1 ) {
		sec = ((*p) >> 4L) & 0x0f;
		sec_len = ((*p) & 0x0f) << 8L; p++;
		sec_len += (*p++) - 2;
		switch ( sec ) {
		case HKSEC_EOS:
			goto jumpout;
			break;
		case HKSEC_TRG:
			trg_ptr = (hk_trg *) p; // copy
			ev.SetTimestamp(10, trg_ptr->trg_ts);
			ev.SetReal(11, trg_ptr->trg_tot);
			ev.SetReal(12, trg_ptr->trg_ext);
			ev.SetReal(13, trg_ptr->trg_clb);
			ev.SetReal(14, trg_ptr->trg_ehi);
			ev.SetReal(15, trg_ptr->trg_elo);
			ev.SetReal(16, trg_ptr->trg_zclb);

			ev.SetReal(17, trg_ptr->raw_zhi);
			ev.SetReal(18, trg_ptr->raw_cal);
			ev.SetReal(19, trg_ptr->raw_zlo);
			//ev.SetReal( 6, trg_ptr->raw_s3hi);
			//ev.SetReal( 7, trg_ptr->raw_s3lo);
			ev.SetReal( 8, trg_ptr->raw_clb);
			ev.SetReal( 9, trg_ptr->raw_ext);
			break;
		case HKSEC_SW:
			sw_ptr = (hk_sw *) p; // copy
			ev.SetTimestamp(20, sw_ptr->nio_ts);
			ev.SetReal(21, sw_ptr->nio_tkbps);
			ev.SetReal(22, sw_ptr->nio_nkbps);
			ev.SetReal(23, sw_ptr->bufl_cmdq / 256.);
			ev.SetReal(24, sw_ptr->bufl_hkq / 256.);
			ev.SetReal(25, sw_ptr->bufl_evtq / 256.);
			ev.SetReal(26, sw_ptr->bufl_datq / 256.);
			ev.SetReal(27, sw_ptr->bufl_datq2 / 256.);
			ev.SetReal(28, sw_ptr->bufl_pkt0 / 256.);
			ev.SetReal(29, sw_ptr->bufl_pkt1 / 256.);
			ev.SetReal(30, sw_ptr->disk_space / 256.);
			ev.SetReal(34, sw_ptr->sys_disk_space / 256.); //PGB 15 July 2008
			ev.SetInteger(31, sw_ptr->cmd_ser);
			ev.SetInteger(32, sw_ptr->cmd_code);
			break;
		case HKSEC_GPS:
			gps_ptr = (hk_gps *) p; // copy
			ev.SetTimestamp(42, gps_ptr->gps_ts);
			ev.SetInteger(43, gps_ptr->gps.pos_hr);
			ev.SetInteger(44, gps_ptr->gps.pos_min);
			ev.SetReal(45, gps_ptr->gps.pos_sec);
			ev.SetReal(46, fix_latitude(gps_ptr->gps.latitude));
			ev.SetReal(47, fix_longitude(gps_ptr->gps.longitude));
			ev.SetReal(48, gps_ptr->gps.altitude);
			ev.SetReal(49, gps_ptr->gps.vvelocity);
			ev.SetReal(50, gps_ptr->gps.heading);
			ev.SetReal(51, gps_ptr->gps.pitch);
			ev.SetReal(52, gps_ptr->gps.roll);
			ev.SetReal(53, gps_ptr->gps.course);
			ev.SetReal(54, gps_ptr->gps.speed);
			ev.SetReal(55, gps_ptr->gps.pdop);
			ev.SetReal(56, gps_ptr->gps.phase_rms);
			ev.SetReal(57, gps_ptr->gps.baseline_rms);
			ev.SetInteger(58, gps_ptr->gps.nsatellite);
			ev.SetInteger(59, gps_ptr->gps.reset_flag);
			break;
		case HKSEC_MTT:
			mtt_ptr = (hk_mtt *) p; // copy
			{
				uint8 a, sum;

				sum = 0;
				for(k = 0; k < 6; k++) {
					a = ((unsigned char) mtt_ptr->total[k]) & 0x00ff;
					sum += a << ((5-k)*8LL);
				}
				//ev.SetLongInteger(60, sum);
				ev.SetDouble(60, _tmconv_fmt1 * sum);

				sum = 0;
				for(k = 0; k < 6; k++) {
					a = ((unsigned char) mtt_ptr->live[k]) & 0x00ff;
					sum += a << ((5-k)*8LL);
				}
				//ev.SetLongInteger(61, sum);
				ev.SetDouble(61, _tmconv_fmt1 * sum);
			}
			break;
		case HKSEC_CAL:
			unpack_12bit_array(p, &cal, sec_len);
			ev.SetADC(62, cal.pwr_logic);
			ev.SetADC(63, cal.pwr_vm3o6);
			ev.SetADC(64, cal.pwr_v3o6);
			ev.SetADC(65, cal.pwr_v12);
			ev.SetADC(66, cal.pwr_cm3o6);
			ev.SetADC(67, cal.pwr_c3o6);
			ev.SetADC(68, cal.pwr_c12);
			ev.SetADC(69, cal.ctrg);

			for(k = 0; k < 8; k++) {
				ev.SetADC(74 + k, cal.temp[k]);
				ev.SetADC(82 + k, cal.hv[k]);
				ev.SetADC(90 + k, cal.bias[k]);
				ev.SetADC(98 + k, cal.cm3o6[k]);
				ev.SetADC(106 + k, cal.c3o6[k]);
				ev.SetADC(114 + k, cal.c12[k]);
			}
			break;
		/*case HKSEC_HDS:
			unpack_12bit_array(p, &hds, sec_len);
			ev.SetADC(122, hds.pwr_logic);
			ev.SetADC(123, hds.pwr_vm6);
			ev.SetADC(124, hds.pwr_v6);
			ev.SetADC(125, hds.pwr_v12);
			for(k = 0; k < 8; k++) {
				ev.SetADC(134 + k, hds.temp1[k]);
				ev.SetADC(146 + k, hds.temp2[k]);
				ev.SetADC(158 + k, hds.temp3[k]);
				ev.SetADC(170 + k, hds.hv[k]);
				ev.SetADC(182 + k, hds.bias[k]);
				ev.SetADC(194 + k, hds.cm6[k]);
				ev.SetADC(206 + k, hds.c6[k]);
				ev.SetADC(218 + k, hds.c12[k]);
			}
			break;
		case HKSEC_SCD:
			unpack_12bit_array(p, &scd, sec_len);
			ev.SetADC(230, scd.pwr_vlogic);
			ev.SetADC(231, scd.pwr_vm6o5);
			ev.SetADC(232, scd.pwr_v6o5);
			ev.SetADC(233, scd.pwr_v12);
			ev.SetADC(234, scd.mb_cm6o5[0]);
			ev.SetADC(235, scd.mb_cm6o5[1]);
			ev.SetADC(236, scd.mb_c6o5[0]);
			ev.SetADC(237, scd.mb_c6o5[1]);
			ev.SetADC(238, scd.mb_c12[0]);
			ev.SetADC(239, scd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(242 + k, scd.temp[k]);
				ev.SetADC(268 + k, scd.bias[k]);
				ev.SetADC(294 + k, scd.vlogic[k]);
				ev.SetADC(320 + k, scd.vm5[k]);
				ev.SetADC(346 + k, scd.v5[k]);
			}
			break;*/
		case HKSEC_tSCD:
			unpack_12bit_array(p, &tscd, sec_len);
			ev.SetADC(439, tscd.pwr_vlogic);
			ev.SetADC(440, tscd.pwr_vm6o5);
			ev.SetADC(441, tscd.pwr_v6o5);
			ev.SetADC(442, tscd.pwr_v12);
			ev.SetADC(443, tscd.mb_cm6o5[0]);
			ev.SetADC(444, tscd.mb_cm6o5[1]);
			ev.SetADC(445, tscd.mb_c6o5[0]);
			ev.SetADC(446, tscd.mb_c6o5[1]);
			ev.SetADC(447, tscd.mb_c12[0]);
			ev.SetADC(448, tscd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(455 + k, tscd.temp[k]);
				ev.SetADC(481 + k, tscd.bias[k]);
				ev.SetADC(507 + k, tscd.vlogic[k]);
				ev.SetADC(533 + k, tscd.vm5[k]);
				ev.SetADC(559 + k, tscd.v5[k]);
				ev.SetADC(585 + k, tscd.cbias[k]);
				ev.SetADC(611 + k, tscd.clogic[k]);
				ev.SetADC(637 + k, tscd.cm5[k]);
				ev.SetADC(663 + k, tscd.c5[k]);
			}
			break;

		case HKSEC_bSCD:
			unpack_12bit_array(p, &bscd, sec_len);
			ev.SetADC(689, bscd.pwr_vlogic);
			ev.SetADC(690, bscd.pwr_vm6o5);
			ev.SetADC(691, bscd.pwr_v6o5);
			ev.SetADC(692, bscd.pwr_v12);
			ev.SetADC(693, bscd.mb_cm6o5[0]);
			ev.SetADC(694, bscd.mb_cm6o5[1]);
			ev.SetADC(695, bscd.mb_c6o5[0]);
			ev.SetADC(696, bscd.mb_c6o5[1]);
			ev.SetADC(697, bscd.mb_c12[0]);
			ev.SetADC(698, bscd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(705 + k, bscd.temp[k]);
				ev.SetADC(731 + k, bscd.bias[k]);
				ev.SetADC(757 + k, bscd.vlogic[k]);
				ev.SetADC(783 + k, bscd.vm5[k]);
				ev.SetADC(809 + k, bscd.v5[k]);
				ev.SetADC(835 + k, bscd.cbias[k]);
				ev.SetADC(861 + k, bscd.clogic[k]);
				ev.SetADC(887 + k, bscd.cm5[k]);
				ev.SetADC(913 + k, bscd.c5[k]);
			}
			break;
		case HKSEC_TRD:
			unpack_12bit_array(p, &trd, sec_len);
			ev.SetADC(372, trd.cm3o6);
			ev.SetADC(373, trd.c3o6);
			for(k = 0; k < 3; k++) {
			;	ev.SetADC(382 + k, trd.hv[k]);
				ev.SetADC(385 + k, trd.hv_pwr[k]);
			}
			for(k = 0; k < 8; k++)
				ev.SetADC(388 + k, trd.temp[k]);
			for(k = 0; k < 11; k++)
				ev.SetADC(396 + k, trd.pressure[k]);
			break;
		case HKSEC_TCD:
			unpack_12bit_array(p, &tcd, sec_len);
			ev.SetADC(407, tcd.vm2);
			ev.SetADC(408, tcd.vm5);
			ev.SetADC(409, tcd.v3o3);
			ev.SetADC(410, tcd.v5analog);
			ev.SetADC(411, tcd.v5);
			ev.SetADC(412, tcd.v12);
			for(k = 0; k < 5; k++)
				ev.SetADC(413 + k, tcd.temp[k]);
			ev.SetADC(418, tcd.reserved[0]); // CVD HV1
			ev.SetADC(419, tcd.reserved[1]); // CVD HV2
			// tcd hv monitors
//			for(k = 0; k < 9; k++) PGB 05 July 2008
			for(k = 0; k < 10; k++)
				ev.SetADC(1079 + k, tcd.hv[k]);
			break;
		case HKSEC_INST:
			unpack_12bit_array(p, &inst, sec_len);
			for(k = 0; k < 12; k++)
				ev.SetADC(423 + k, inst.temp[k]);
			break;
		/*case HKSEC_dSCD:
			unpack_12bit_array(p, &dscd, sec_len);
			// top scd
			ev.SetADC(439, dscd.top_pwr_vlogic);
			ev.SetADC(440, dscd.top_pwr_vm6o5);
			ev.SetADC(441, dscd.top_pwr_v6o5);
			ev.SetADC(442, dscd.top_pwr_v12);
			for(k = 0; k < 26; k++) {
				ev.SetADC(455 + k, dscd.top_temp[k]);
				ev.SetADC(481 + k, dscd.top_bias[k]);
				ev.SetADC(611 + k, dscd.top_clogic[k]);
				ev.SetADC(637 + k, dscd.top_cm5[k]);
				ev.SetADC(663 + k, dscd.top_c5[k]);
			}
			// bottom scd
			ev.SetADC(689, dscd.bot_pwr_vlogic);
			ev.SetADC(690, dscd.bot_pwr_vm6o5);
			ev.SetADC(691, dscd.bot_pwr_v6o5);
			ev.SetADC(692, dscd.bot_pwr_v12);
			for(k = 0; k < 26; k++) {
				ev.SetADC(705 + k, dscd.bot_temp[k]);
				ev.SetADC(731 + k, dscd.bot_bias[k]);
				ev.SetADC(861 + k, dscd.bot_clogic[k]);
				ev.SetADC(887 + k, dscd.bot_cm5[k]);
				ev.SetADC(913 + k, dscd.bot_c5[k]);
			}
			break;*/
		case HKSEC_CER:
			unpack_12bit_array(p, &cer, sec_len);

			for(k = 0; k < 100; k++)
				ev.SetADC(939 + k, cer.hv[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1039 + k, cer.hv_c28[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1041 + k, cer.hv_cm3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1043 + k, cer.hv_ctrl_c12[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1045 + k, cer.hv_ctrl_cm3o3[k]);
			ev.SetADC(1047, cer.ps_v5_analog);
			ev.SetADC(1048, cer.ps_v5_digital);
			ev.SetADC(1049, cer.ps_vm3o3);
			ev.SetADC(1050, cer.ps_v12);
			ev.SetADC(1051, cer.ps_v28);
			ev.SetADC(1052, cer.sprs_c5);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1053 + k, cer.merger_c3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1055 + k, cer.merger_cm3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1057 + k, cer.merger_c2o5[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1059 + k, cer.merger_c5[k]);
			ev.SetADC(1061, cer.hk_c12);
			ev.SetADC(1062, cer.hk_cm3o3);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1063 + k, cer.merger_temp[k]);
			for(k = 0; k < 14; k ++)
				ev.SetADC(1065 + k, cer.temp[k]);
				
			break;
		}
		p += sec_len;
	}

jumpout:
	ev.Convert();
	ev.CheckStatus();
	return 0;
}

int CreamHKFile::unpack_fmt_1_reduced(unsigned char* p, DHKEventConverter& ev)
	{
	uint2* h;
	uint2 a[139];
	int k;

	// length, type, and fmt is already done.
	p += 4;

	// event timestamp
	ev.SetTimestamp(0, (timestamp4_t *) p);
	p += sizeof(timestamp4_t);

	// trigger, network speed, disk space, buffer levels.
	h = (uint2 *) p;
	ev.SetReal(11, (*h++) / 256.);
	ev.SetReal(16, (*h++) / 256.);
	ev.SetReal(14, (*h++) / 256.);
	ev.SetReal(21, (*h++) / 256.);
	ev.SetReal(30, (*h++) / 256.);
	ev.SetReal(23, (*h++) / 256.);
	ev.SetReal(24, (*h++) / 256.);
	ev.SetReal(25, (*h++) / 256.);
	ev.SetReal(26, (*h++) / 256.);
	ev.SetReal(27, (*h++) / 256.);
	ev.SetReal(28, (*h++) / 256.);
	ev.SetReal(29, (*h++) / 256.);

	// gps hour, min, sec.
	ev.SetInteger(43, (*h++));
	ev.SetInteger(44, (*h++));
	ev.SetReal(45, *((real4 *) h)); h += 2;

	// unpack
	unpack_12bit_array((unsigned char *) h, a, 210);

	// cal
	for(k = 0; k < 8; k++) ev.SetADC(90 + k, a[k]);
	for(k = 0; k < 8; k++) ev.SetADC(82 + k, a[k + 8]);
	ev.SetADC(62, a[16]);
	ev.SetADC(63, a[17]);
	ev.SetADC(64, a[18]);
	ev.SetADC(65, a[19]);
	ev.SetADC(66, a[20]);
	ev.SetADC(67, a[21]);
	ev.SetADC(68, a[22]);
	ev.SetADC(69, a[23]);

	// tscd
	for(k = 0; k < 26; k++) ev.SetADC(481 + k, a[k + 24]);
	ev.SetADC(439, a[50]);
	ev.SetADC(440, a[51]);
	ev.SetADC(441, a[52]);
	ev.SetADC(442, a[53]);
	ev.SetADC(443, a[54]);
	ev.SetADC(444, a[55]);
	ev.SetADC(445, a[56]);
	ev.SetADC(446, a[57]);
	ev.SetADC(447, a[58]);
	ev.SetADC(448, a[59]);

	// bscd
	for(k = 0; k < 26; k++) ev.SetADC(731 + k, a[k + 60]);
	ev.SetADC(689, a[86]);
	ev.SetADC(690, a[87]);
	ev.SetADC(691, a[88]);
	ev.SetADC(692, a[89]);
	ev.SetADC(693, a[90]);
	ev.SetADC(694, a[91]);
	ev.SetADC(695, a[92]);
	ev.SetADC(696, a[93]);
	ev.SetADC(697, a[94]);
	ev.SetADC(698, a[95]);

	// tcd
	ev.SetADC(407, a[96]);
	ev.SetADC(408, a[97]);
	ev.SetADC(409, a[98]);
	ev.SetADC(410, a[99]);
	ev.SetADC(411, a[100]);
	ev.SetADC(412, a[101]);
	for(k = 0; k < 5; k++)
		ev.SetADC(413 + k, a[102 + k]);

	// instrument
	for(k = 0; k < 12; k++)
		ev.SetADC(423 + k, a[107 + k]);
	ev.SetADC(418, a[119]);
	ev.SetADC(419, a[120]);

	ev.Convert();
	ev.CheckStatus();
	return 0;
}

int CreamHKFile::read_fmt_4(DHKEventConverter& ev)
	{
	int c;
	struct _hdr {
		unsigned short length;
		unsigned char type;
		unsigned char fmt;
	} *p;

	/* check EOF */
	if ( (c = fgetc(_fp)) == EOF ) return -1;
	else ungetc(c, _fp);

	/* read four bytes to get length, type, and format */
	fread(ev.GetEventBufferPtr(), sizeof(char), 4, _fp);
	p = (struct _hdr *) ev.GetEventBufferPtr();
	fread(ev.GetEventBufferPtr() + 4, sizeof(char), p->length - 4, _fp);
	ev.SetEventType( p->type );
	ev.SetEventFormat( p->fmt );
	ev.SetEventLength( p->length );

	switch ( p->type ) {
	case DAT_HK: // full hk
		unpack_fmt_4_full((unsigned char *) ev.GetEventBufferPtr(), ev);
		break;
	case DAT_HK2: // reduced hk
		unpack_fmt_1_reduced((unsigned char *) ev.GetEventBufferPtr(), ev);
		break;
	case DAT_TCF: // TCD config
		// nothing todo
		break;
	default:
		fprintf(stderr, "File contains events non-related to housekeeping.\n");
		return -1;
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// For format version 5.xx
//////////////////////////////////////////////////////////////////////////////

struct _fmt5_hdr {
	unsigned short length;
	unsigned short crc16;
	unsigned char type;
	unsigned char fmt;
	unsigned short reserved;
	unsigned serial;
	timestamp8_t ts;
}; /* 20 bytes */

int CreamHKFile::unpack_fmt_5_full(unsigned char* p, DHKEventConverter& ev)
	{
	int sec, sec_len, k, nsec = 0;
	static hk_trg* trg_ptr;
	static hk_sw* sw_ptr;
	static hk_gps* gps_ptr;
	static hk_mtt* mtt_ptr;
	static hk_cal cal;
	static hk_hds hds;
	static hk_scd scd;
	static hk_tscd tscd;
	static hk_bscd bscd;
	static hk_tcd tcd;
	static hk_trd trd;
	static hk_inst inst;
	static hk_dscd dscd;
	static hk_cer cer;
	struct _fmt5_hdr *hdr;

	// Do the header.
	hdr = (struct _fmt5_hdr *) p;
	ev.SetEventFormat( hdr->fmt );
	ev.SetEventSerial( hdr->serial );
	ev.SetTimestamp(0, &(hdr->ts));
	p += 20; 

	// go over sections
	while ( nsec++ < 16 ) {
		sec = ((*p) >> 4L) & 0x0f;
		sec_len = ((*p) & 0x0f) << 8L; p++;
		sec_len += (*p++) - 2;
		switch ( sec ) {
		default:
			// This is an error!!!
			goto jumpout;
		case HKSEC_EOS:
			goto jumpout;
			break;
		case HKSEC_TRG:
			trg_ptr = (hk_trg *) p; // copy
			ev.SetTimestamp(10, trg_ptr->trg_ts);
			ev.SetReal(11, trg_ptr->trg_tot);
			ev.SetReal(12, trg_ptr->trg_ext);
			ev.SetReal(13, trg_ptr->trg_clb);
			ev.SetReal(14, trg_ptr->trg_ehi);
			ev.SetReal(15, trg_ptr->trg_elo);
			ev.SetReal(16, trg_ptr->trg_zclb);

			ev.SetReal(17, trg_ptr->raw_zhi);
			ev.SetReal(18, trg_ptr->raw_cal);
			ev.SetReal(19, trg_ptr->raw_zlo);
			//ev.SetReal( 6, trg_ptr->raw_s3hi);
			//ev.SetReal( 7, trg_ptr->raw_s3lo);
			ev.SetReal( 8, trg_ptr->raw_clb);
			ev.SetReal( 9, trg_ptr->raw_ext);
			break;
		case HKSEC_SW:
			sw_ptr = (hk_sw *) p; // copy
			ev.SetTimestamp(20, sw_ptr->nio_ts);
			ev.SetReal(21, sw_ptr->nio_tkbps);
			ev.SetReal(22, sw_ptr->nio_nkbps);
			ev.SetReal(23, sw_ptr->bufl_cmdq / 256.);
			ev.SetReal(24, sw_ptr->bufl_hkq / 256.);
			ev.SetReal(25, sw_ptr->bufl_evtq / 256.);
			ev.SetReal(26, sw_ptr->bufl_datq / 256.);
			ev.SetReal(27, sw_ptr->bufl_datq2 / 256.);
			ev.SetReal(28, sw_ptr->bufl_pkt0 / 256.);
			ev.SetReal(29, sw_ptr->bufl_pkt1 / 256.);
			ev.SetReal(30, sw_ptr->disk_space / 256.);
			ev.SetReal(34, sw_ptr->sys_disk_space / 256.); //PGB 15 July 2008
			ev.SetInteger(31, sw_ptr->cmd_ser);
			ev.SetInteger(32, sw_ptr->cmd_code);
			break;
		case HKSEC_GPS:
			gps_ptr = (hk_gps *) p; // copy
			ev.SetTimestamp(42, gps_ptr->gps_ts);
			ev.SetInteger(43, gps_ptr->gps.pos_hr);
			ev.SetInteger(44, gps_ptr->gps.pos_min);
			ev.SetReal(45, gps_ptr->gps.pos_sec);
			ev.SetReal(46, fix_latitude(gps_ptr->gps.latitude));
			ev.SetReal(47, fix_longitude(gps_ptr->gps.longitude));
			ev.SetReal(48, gps_ptr->gps.altitude);
			ev.SetReal(49, gps_ptr->gps.vvelocity);
			ev.SetReal(50, gps_ptr->gps.heading);
			ev.SetReal(51, gps_ptr->gps.pitch);
			ev.SetReal(52, gps_ptr->gps.roll);
			ev.SetReal(53, gps_ptr->gps.course);
			ev.SetReal(54, gps_ptr->gps.speed);
			ev.SetReal(55, gps_ptr->gps.pdop);
			ev.SetReal(56, gps_ptr->gps.phase_rms);
			ev.SetReal(57, gps_ptr->gps.baseline_rms);
			ev.SetInteger(58, gps_ptr->gps.nsatellite);
			ev.SetInteger(59, gps_ptr->gps.reset_flag);
			break;
		case HKSEC_MTT:
			mtt_ptr = (hk_mtt *) p; // copy
			{
				static double live_tm_prev = 0, tot_tm_prev = 0;
				double live_tm, tot_tm;

				tot_tm = convert_timer(mtt_ptr->total, 6, _tmconv_fmt5);
				ev.SetDouble(60, format_min_sec(tot_tm));

				live_tm = convert_timer(mtt_ptr->live, 6, _tmconv_fmt5);
				ev.SetDouble(61, format_min_sec(live_tm));

				// compute differential ratio
				if ( (tot_tm - tot_tm_prev) != 0 )
					ev.SetDouble(33,
						((live_tm - live_tm_prev) /
						(tot_tm - tot_tm_prev)) * 100.);
				else
					ev.SetDouble(33, 999.0);

				tot_tm_prev = tot_tm;
				live_tm_prev = live_tm;
			}
			
			break;
		case HKSEC_CAL:
			unpack_12bit_array(p, &cal, sec_len);
			ev.SetADC(62, cal.pwr_logic);
			ev.SetADC(63, cal.pwr_vm3o6);
			ev.SetADC(64, cal.pwr_v3o6);
			ev.SetADC(65, cal.pwr_v12);
			ev.SetADC(66, cal.pwr_cm3o6);
			ev.SetADC(67, cal.pwr_c3o6);
			ev.SetADC(68, cal.pwr_c12);
			ev.SetADC(69, cal.ctrg);
			for(k = 0; k < 8; k++) {
				ev.SetADC(74 + k, cal.temp[k]);
				ev.SetADC(82 + k, cal.hv[k]);
				ev.SetADC(90 + k, cal.bias[k]);
				ev.SetADC(98 + k, cal.cm3o6[k]);
				ev.SetADC(106 + k, cal.c3o6[k]);
				ev.SetADC(114 + k, cal.c12[k]);
			}
			break;
		/*case HKSEC_HDS:
			unpack_12bit_array(p, &hds, sec_len);
			ev.SetADC(122, hds.pwr_logic);
			ev.SetADC(123, hds.pwr_vm6);
			ev.SetADC(124, hds.pwr_v6);
			ev.SetADC(125, hds.pwr_v12);
			for(k = 0; k < 8; k++) {
				ev.SetADC(134 + k, hds.temp1[k]);
				ev.SetADC(146 + k, hds.temp2[k]);
				ev.SetADC(158 + k, hds.temp3[k]);
				ev.SetADC(170 + k, hds.hv[k]);
				ev.SetADC(182 + k, hds.bias[k]);
				ev.SetADC(194 + k, hds.cm6[k]);
				ev.SetADC(206 + k, hds.c6[k]);
				ev.SetADC(218 + k, hds.c12[k]);
			}
			break;
		case HKSEC_SCD:
			unpack_12bit_array(p, &scd, sec_len);
			ev.SetADC(230, scd.pwr_vlogic);
			ev.SetADC(231, scd.pwr_vm6o5);
			ev.SetADC(232, scd.pwr_v6o5);
			ev.SetADC(233, scd.pwr_v12);
			ev.SetADC(234, scd.mb_cm6o5[0]);
			ev.SetADC(235, scd.mb_cm6o5[1]);
			ev.SetADC(236, scd.mb_c6o5[0]);
			ev.SetADC(237, scd.mb_c6o5[1]);
			ev.SetADC(238, scd.mb_c12[0]);
			ev.SetADC(239, scd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(242 + k, scd.temp[k]);
				ev.SetADC(268 + k, scd.bias[k]);
				ev.SetADC(294 + k, scd.vlogic[k]);
				ev.SetADC(320 + k, scd.vm5[k]);
				ev.SetADC(346 + k, scd.v5[k]);
			}
			break;*/
		case HKSEC_tSCD:
			unpack_12bit_array(p, &tscd, sec_len);
			ev.SetADC(439, tscd.pwr_vlogic);
			ev.SetADC(440, tscd.pwr_vm6o5);
			ev.SetADC(441, tscd.pwr_v6o5);
			ev.SetADC(442, tscd.pwr_v12);
			ev.SetADC(443, tscd.mb_cm6o5[0]);
			ev.SetADC(444, tscd.mb_cm6o5[1]);
			ev.SetADC(445, tscd.mb_c6o5[0]);
			ev.SetADC(446, tscd.mb_c6o5[1]);
			ev.SetADC(447, tscd.mb_c12[0]);
			ev.SetADC(448, tscd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(455 + k, tscd.temp[k]);
				ev.SetADC(481 + k, tscd.bias[k]);
				ev.SetADC(507 + k, tscd.vlogic[k]);
				ev.SetADC(533 + k, tscd.vm5[k]);
				ev.SetADC(559 + k, tscd.v5[k]);
				ev.SetADC(585 + k, tscd.cbias[k]);
				ev.SetADC(611 + k, tscd.clogic[k]);
				ev.SetADC(637 + k, tscd.cm5[k]);
				ev.SetADC(663 + k, tscd.c5[k]);
			}
			break;
		case HKSEC_bSCD:
			unpack_12bit_array(p, &bscd, sec_len);
			ev.SetADC(689, bscd.pwr_vlogic);
			ev.SetADC(690, bscd.pwr_vm6o5);
			ev.SetADC(691, bscd.pwr_v6o5);
			ev.SetADC(692, bscd.pwr_v12);
			ev.SetADC(693, bscd.mb_cm6o5[0]);
			ev.SetADC(694, bscd.mb_cm6o5[1]);
			ev.SetADC(695, bscd.mb_c6o5[0]);
			ev.SetADC(696, bscd.mb_c6o5[1]);
			ev.SetADC(697, bscd.mb_c12[0]);
			ev.SetADC(698, bscd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(705 + k, bscd.temp[k]);
				ev.SetADC(731 + k, bscd.bias[k]);
				ev.SetADC(757 + k, bscd.vlogic[k]);
				ev.SetADC(783 + k, bscd.vm5[k]);
				ev.SetADC(809 + k, bscd.v5[k]);
				ev.SetADC(835 + k, bscd.cbias[k]);
				ev.SetADC(861 + k, bscd.clogic[k]);
				ev.SetADC(887 + k, bscd.cm5[k]);
				ev.SetADC(913 + k, bscd.c5[k]);
			}
			break;
		case HKSEC_TRD:
			unpack_12bit_array(p, &trd, sec_len);
			ev.SetADC(372, trd.cm3o6);
			ev.SetADC(373, trd.c3o6);
			for(k = 0; k < 3; k++) {
				ev.SetADC(382 + k, trd.hv[k]);
				ev.SetADC(385 + k, trd.hv_pwr[k]);
			}
			for(k = 0; k < 8; k++)
				ev.SetADC(388 + k, trd.temp[k]);
			for(k = 0; k < 11; k++)
				ev.SetADC(396 + k, trd.pressure[k]);
			break;
		case HKSEC_TCD:
			unpack_12bit_array(p, &tcd, sec_len);
			ev.SetADC(407, tcd.vm2);
			ev.SetADC(408, tcd.vm5);
			ev.SetADC(409, tcd.v3o3);
			ev.SetADC(410, tcd.v5analog);
			ev.SetADC(411, tcd.v5);
			ev.SetADC(412, tcd.v12);
			for(k = 0; k < 5; k++)
				ev.SetADC(413 + k, tcd.temp[k]);
			ev.SetADC(418, tcd.reserved[0]); // CVD HV1
			ev.SetADC(419, tcd.reserved[1]); // CVD HV2
			// tcd hv monitors
//			for(k = 0; k < 9; k++)PGB 05 July 2008
			for(k = 0; k < 10; k++)
				ev.SetADC(1079 + k, tcd.hv[k]);
			break;
		case HKSEC_INST:
			unpack_12bit_array(p, &inst, sec_len);
			for(k = 0; k < 12; k++)
				ev.SetADC(423 + k, inst.temp[k]);
			break;
		/*case HKSEC_dSCD:                                 
			unpack_12bit_array(p, &dscd, sec_len);   
			// top scd             
			ev.SetADC(439, dscd.top_pwr_vlogic);     
			ev.SetADC(440, dscd.top_pwr_vm6o5);      
			ev.SetADC(441, dscd.top_pwr_v6o5);       
			ev.SetADC(442, dscd.top_pwr_v12);         
			for(k = 0; k < 26; k++) {                 
				ev.SetADC(455 + k, dscd.top_temp[k]);  
				ev.SetADC(481 + k, dscd.top_bias[k]);   
				ev.SetADC(611 + k, dscd.top_clogic[k]);  
				ev.SetADC(637 + k, dscd.top_cm5[k]);     
				ev.SetADC(663 + k, dscd.top_c5[k]);     
			}
			// bottom scd
			ev.SetADC(689, dscd.bot_pwr_vlogic);        
			ev.SetADC(690, dscd.bot_pwr_vm6o5);         
			ev.SetADC(691, dscd.bot_pwr_v6o5);          
			ev.SetADC(692, dscd.bot_pwr_v12);           
			for(k = 0; k < 26; k++) {
				ev.SetADC(705 + k, dscd.bot_temp[k]);  
				ev.SetADC(731 + k, dscd.bot_bias[k]);   
				ev.SetADC(861 + k, dscd.bot_clogic[k]); 
				ev.SetADC(887 + k, dscd.bot_cm5[k]);     
				ev.SetADC(913 + k, dscd.bot_c5[k]);     
			}                                                 
			break;*/
		case HKSEC_CER:
			unpack_12bit_array(p, &cer, sec_len);

			for(k = 0; k < 100; k++)
				ev.SetADC(939 + k, cer.hv[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1039 + k, cer.hv_c28[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1041 + k, cer.hv_cm3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1043 + k, cer.hv_ctrl_c12[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1045 + k, cer.hv_ctrl_cm3o3[k]);
			ev.SetADC(1047, cer.ps_v5_analog);
			ev.SetADC(1048, cer.ps_v5_digital);
			ev.SetADC(1049, cer.ps_vm3o3);
			ev.SetADC(1050, cer.ps_v12);
			ev.SetADC(1051, cer.ps_v28);
			ev.SetADC(1052, cer.sprs_c5);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1053 + k, cer.merger_c3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1055 + k, cer.merger_cm3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1057 + k, cer.merger_c2o5[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1059 + k, cer.merger_c5[k]);
			ev.SetADC(1061, cer.hk_c12);
			ev.SetADC(1062, cer.hk_cm3o3);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1063 + k, cer.merger_temp[k]);
			for(k = 0; k < 14; k ++)
				ev.SetADC(1065 + k, cer.temp[k]);
				
			break;
		}
		p += sec_len;
	}

jumpout:
	ev.Convert();
	ev.CheckStatus();
	return 0;
}
//For SIP GPS
int CreamHKFile::unpack_fmt_6_full(unsigned char* p, DHKEventConverter& ev)
	{
	int sec, sec_len, k, nsec = 0;
	static hk_trg* trg_ptr;
	static hk_sw* sw_ptr;
	static hk_gps_sip* gps_ptr;
//	static hk_gps* gps_ptr;
	static hk_mtt* mtt_ptr;
	static hk_cal cal;
	static hk_hds hds;
	static hk_scd scd;
	static hk_tscd tscd;
	static hk_bscd bscd;
	static hk_tcd tcd;
	static hk_trd trd;
	static hk_inst inst;
	static hk_dscd dscd;
	static hk_cer cer;
	struct _fmt5_hdr *hdr;

	// Do the header.
	hdr = (struct _fmt5_hdr *) p;
	ev.SetEventFormat( hdr->fmt );
	ev.SetEventSerial( hdr->serial );
	ev.SetTimestamp(0, &(hdr->ts));
	p += 20; 

	// go over sections
	while ( nsec++ < 16 ) {
		sec = ((*p) >> 4L) & 0x0f;
		sec_len = ((*p) & 0x0f) << 8L; p++;
		sec_len += (*p++) - 2;
		switch ( sec ) {
		default:
			// This is an error!!!
			goto jumpout;
		case HKSEC_EOS:
			goto jumpout;
			break;
		case HKSEC_TRG:
			trg_ptr = (hk_trg *) p; // copy
			ev.SetTimestamp(10, trg_ptr->trg_ts);
			ev.SetReal(11, trg_ptr->trg_tot);
			ev.SetReal(12, trg_ptr->trg_ext);
			ev.SetReal(13, trg_ptr->trg_clb);
			ev.SetReal(14, trg_ptr->trg_ehi);
			ev.SetReal(15, trg_ptr->trg_elo);
			ev.SetReal(16, trg_ptr->trg_zclb);

			ev.SetReal(17, trg_ptr->raw_zhi);
			ev.SetReal(18, trg_ptr->raw_cal);
			ev.SetReal(19, trg_ptr->raw_zlo);
			//ev.SetReal( 6, trg_ptr->raw_s3hi);
			//ev.SetReal( 7, trg_ptr->raw_s3lo);
			ev.SetReal( 8, trg_ptr->raw_clb);
			ev.SetReal( 9, trg_ptr->raw_ext);
			break;
		case HKSEC_SW:
			sw_ptr = (hk_sw *) p; // copy
			ev.SetTimestamp(20, sw_ptr->nio_ts);
			ev.SetReal(21, sw_ptr->nio_tkbps);
			ev.SetReal(22, sw_ptr->nio_nkbps);
			ev.SetReal(23, sw_ptr->bufl_cmdq / 256.);
			ev.SetReal(24, sw_ptr->bufl_hkq / 256.);
			ev.SetReal(25, sw_ptr->bufl_evtq / 256.);
			ev.SetReal(26, sw_ptr->bufl_datq / 256.);
			ev.SetReal(27, sw_ptr->bufl_datq2 / 256.);
			ev.SetReal(28, sw_ptr->bufl_pkt0 / 256.);
			ev.SetReal(29, sw_ptr->bufl_pkt1 / 256.);
			ev.SetReal(30, sw_ptr->disk_space / 256.);
			ev.SetReal(34, sw_ptr->sys_disk_space / 256.); //PGB 15 July 2008
			ev.SetInteger(31, sw_ptr->cmd_ser);
			ev.SetInteger(32, sw_ptr->cmd_code);
			break;
		case HKSEC_GPS:
			gps_ptr = (hk_gps_sip *) p; // copy
			ev.SetTimestamp(42, gps_ptr->gps_ts);
			ev.SetInteger(43, gps_ptr->gps.timeOff);
//			ev.SetInteger(43, gps_ptr->gps.pos_hr);
			ev.SetInteger(44, gps_ptr->gps.weekNo);
//			ev.SetInteger(44, gps_ptr->gps.pos_min);
			ev.SetReal(45, gps_ptr->gps.weekTime);
//			ev.SetReal(45, gps_ptr->gps.pos_sec);
//			ev.SetReal(46, fix_latitude(gps_ptr->gps.latitude));
//			ev.SetReal(47, fix_longitude(gps_ptr->gps.longitude));
			ev.SetReal(46, gps_ptr->gps.latitude);  //SIP Dr.L
			ev.SetReal(47, gps_ptr->gps.longitude); //SIP Dr.L
			ev.SetReal(48, gps_ptr->gps.altitude);
			ev.SetReal(49, gps_ptr->gps.vvelocity);
			ev.SetReal(50, gps_ptr->gps.heading);
			ev.SetReal(51, gps_ptr->gps.pitch);
			ev.SetReal(52, gps_ptr->gps.roll);
//			ev.SetReal(53, gps_ptr->gps.course);
			ev.SetReal(54, gps_ptr->gps.speed);

			ev.SetReal(55, gps_ptr->gps.mksHi);
//			ev.SetReal(55, gps_ptr->gps.pdop);
			ev.SetReal(56, gps_ptr->gps.mksMid);
//			ev.SetReal(56, gps_ptr->gps.phase_rms);
			ev.SetReal(57, gps_ptr->gps.mksLo);
//			ev.SetReal(57, gps_ptr->gps.baseline_rms);
			ev.SetInteger(58, gps_ptr->gps.nsatellite);
			ev.SetInteger(59, gps_ptr->gps.reset_flag);
			break;
		case HKSEC_MTT:
			mtt_ptr = (hk_mtt *) p; // copy
			{
				static double live_tm_prev = 0, tot_tm_prev = 0;
				double live_tm, tot_tm;

				tot_tm = convert_timer(mtt_ptr->total, 6, _tmconv_fmt5);
				ev.SetDouble(60, format_min_sec(tot_tm));

				live_tm = convert_timer(mtt_ptr->live, 6, _tmconv_fmt5);
				ev.SetDouble(61, format_min_sec(live_tm));

				// compute differential ratio
				if ( (tot_tm - tot_tm_prev) != 0 )
					ev.SetDouble(33,
						((live_tm - live_tm_prev) /
						(tot_tm - tot_tm_prev)) * 100.);
				else
					ev.SetDouble(33, 999.0);

				tot_tm_prev = tot_tm;
				live_tm_prev = live_tm;
			}
			
			break;
		case HKSEC_CAL:
			unpack_12bit_array(p, &cal, sec_len);
			ev.SetADC(62, cal.pwr_logic);
			ev.SetADC(63, cal.pwr_vm3o6);
			ev.SetADC(64, cal.pwr_v3o6);
			ev.SetADC(65, cal.pwr_v12);
			ev.SetADC(66, cal.pwr_cm3o6);
			ev.SetADC(67, cal.pwr_c3o6);
			ev.SetADC(68, cal.pwr_c12);
			ev.SetADC(69, cal.ctrg);

			ev.SetADC(89, cal.reserved[0]);

			for(k = 0; k < 8; k++) {
				ev.SetADC(73 + k, cal.temp[k]);
				ev.SetADC(81 + k, cal.hv[k]);
				ev.SetADC(90 + k, cal.bias[k]);
				ev.SetADC(98 + k, cal.cm3o6[k]);
				ev.SetADC(106 + k, cal.c3o6[k]);
				ev.SetADC(114 + k, cal.c12[k]);
			}
			break;
		/*case HKSEC_HDS:
			unpack_12bit_array(p, &hds, sec_len);
			ev.SetADC(122, hds.pwr_logic);
			ev.SetADC(123, hds.pwr_vm6);
			ev.SetADC(124, hds.pwr_v6);
			ev.SetADC(125, hds.pwr_v12);
			for(k = 0; k < 8; k++) {
				ev.SetADC(134 + k, hds.temp1[k]);
				ev.SetADC(146 + k, hds.temp2[k]);
				ev.SetADC(158 + k, hds.temp3[k]);
				ev.SetADC(170 + k, hds.hv[k]);
				ev.SetADC(182 + k, hds.bias[k]);
				ev.SetADC(194 + k, hds.cm6[k]);
				ev.SetADC(206 + k, hds.c6[k]);
				ev.SetADC(218 + k, hds.c12[k]);
			}
			break;
		case HKSEC_SCD:
			unpack_12bit_array(p, &scd, sec_len);
			ev.SetADC(230, scd.pwr_vlogic);
			ev.SetADC(231, scd.pwr_vm6o5);
			ev.SetADC(232, scd.pwr_v6o5);
			ev.SetADC(233, scd.pwr_v12);
			ev.SetADC(234, scd.mb_cm6o5[0]);
			ev.SetADC(235, scd.mb_cm6o5[1]);
			ev.SetADC(236, scd.mb_c6o5[0]);
			ev.SetADC(237, scd.mb_c6o5[1]);
			ev.SetADC(238, scd.mb_c12[0]);
			ev.SetADC(239, scd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(242 + k, scd.temp[k]);
				ev.SetADC(268 + k, scd.bias[k]);
				ev.SetADC(294 + k, scd.vlogic[k]);
				ev.SetADC(320 + k, scd.vm5[k]);
				ev.SetADC(346 + k, scd.v5[k]);
			}
			break;*/
		case HKSEC_tSCD:
			unpack_12bit_array(p, &tscd, sec_len);
			ev.SetADC(439, tscd.pwr_vlogic);
			ev.SetADC(440, tscd.pwr_vm6o5);
			ev.SetADC(441, tscd.pwr_v6o5);
			ev.SetADC(442, tscd.pwr_v12);
			ev.SetADC(443, tscd.mb_cm6o5[0]);
			ev.SetADC(444, tscd.mb_cm6o5[1]);
			ev.SetADC(445, tscd.mb_c6o5[0]);
			ev.SetADC(446, tscd.mb_c6o5[1]);
			ev.SetADC(447, tscd.mb_c12[0]);
			ev.SetADC(448, tscd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(455 + k, tscd.temp[k]);
				ev.SetADC(481 + k, tscd.bias[k]);
				ev.SetADC(507 + k, tscd.vlogic[k]);
				ev.SetADC(533 + k, tscd.vm5[k]);
				ev.SetADC(559 + k, tscd.v5[k]);
				ev.SetADC(585 + k, tscd.cbias[k]);
				ev.SetADC(611 + k, tscd.clogic[k]);
				ev.SetADC(637 + k, tscd.cm5[k]);
				ev.SetADC(663 + k, tscd.c5[k]);
			}
			break;
		case HKSEC_bSCD:
			unpack_12bit_array(p, &bscd, sec_len);
			ev.SetADC(689, bscd.pwr_vlogic);
			ev.SetADC(690, bscd.pwr_vm6o5);
			ev.SetADC(691, bscd.pwr_v6o5);
			ev.SetADC(692, bscd.pwr_v12);
			ev.SetADC(693, bscd.mb_cm6o5[0]);
			ev.SetADC(694, bscd.mb_cm6o5[1]);
			ev.SetADC(695, bscd.mb_c6o5[0]);
			ev.SetADC(696, bscd.mb_c6o5[1]);
			ev.SetADC(697, bscd.mb_c12[0]);
			ev.SetADC(698, bscd.mb_c12[1]);
			for(k = 0; k < 26; k++) {
				ev.SetADC(705 + k, bscd.temp[k]);
				ev.SetADC(731 + k, bscd.bias[k]);
				ev.SetADC(757 + k, bscd.vlogic[k]);
				ev.SetADC(783 + k, bscd.vm5[k]);
				ev.SetADC(809 + k, bscd.v5[k]);
				ev.SetADC(835 + k, bscd.cbias[k]);
				ev.SetADC(861 + k, bscd.clogic[k]);
				ev.SetADC(887 + k, bscd.cm5[k]);
				ev.SetADC(913 + k, bscd.c5[k]);
			}
			break;
		case HKSEC_TRD:
			unpack_12bit_array(p, &trd, sec_len);
			ev.SetADC(372, trd.cm3o6);
			ev.SetADC(373, trd.c3o6);
			for(k = 0; k < 3; k++) {
				ev.SetADC(382 + k, trd.hv[k]);
				ev.SetADC(385 + k, trd.hv_pwr[k]);
			}
			for(k = 0; k < 8; k++)
				ev.SetADC(388 + k, trd.temp[k]);
			for(k = 0; k < 11; k++)
				ev.SetADC(396 + k, trd.pressure[k]);
			break;
		case HKSEC_TCD:
			unpack_12bit_array(p, &tcd, sec_len);
			ev.SetADC(407, tcd.vm2);
			ev.SetADC(408, tcd.vm5);
			ev.SetADC(409, tcd.v3o3);
			ev.SetADC(410, tcd.v5analog);
			ev.SetADC(411, tcd.v5);
			ev.SetADC(412, tcd.v12);
			for(k = 0; k < 5; k++)
				ev.SetADC(413 + k, tcd.temp[k]);
			ev.SetADC(418, tcd.reserved[0]); // CVD HV1
			ev.SetADC(419, tcd.reserved[1]); // CVD HV2
			// tcd hv monitors
//			for(k = 0; k < 9; k++)PGB 05 July 2008
			for(k = 0; k < 10; k++)
				ev.SetADC(1079 + k, tcd.hv[k]);
			break;
		case HKSEC_INST:
			unpack_12bit_array(p, &inst, sec_len);
			for(k = 0; k < 12; k++)
				ev.SetADC(423 + k, inst.temp[k]);
			break;
		/*case HKSEC_dSCD:                                    
			unpack_12bit_array(p, &dscd, sec_len);          
			// top scd                         
			ev.SetADC(439, dscd.top_pwr_vlogic);            
			ev.SetADC(440, dscd.top_pwr_vm6o5);        
			ev.SetADC(441, dscd.top_pwr_v6o5);            
			ev.SetADC(442, dscd.top_pwr_v12);             
			for(k = 0; k < 26; k++) {                      
				ev.SetADC(455 + k, dscd.top_temp[k]);      
				ev.SetADC(481 + k, dscd.top_bias[k]);      
				ev.SetADC(611 + k, dscd.top_clogic[k]);    
				ev.SetADC(637 + k, dscd.top_cm5[k]);      
				ev.SetADC(663 + k, dscd.top_c5[k]);
			}
			// bottom scd
			ev.SetADC(689, dscd.bot_pwr_vlogic);
			ev.SetADC(690, dscd.bot_pwr_vm6o5);
			ev.SetADC(691, dscd.bot_pwr_v6o5);
			ev.SetADC(692, dscd.bot_pwr_v12);
			for(k = 0; k < 26; k++) {
				ev.SetADC(705 + k, dscd.bot_temp[k]);
				ev.SetADC(731 + k, dscd.bot_bias[k]);
				ev.SetADC(861 + k, dscd.bot_clogic[k]);
				ev.SetADC(887 + k, dscd.bot_cm5[k]);
				ev.SetADC(913 + k, dscd.bot_c5[k]);
			}
			break;*/
		case HKSEC_CER:
			unpack_12bit_array(p, &cer, sec_len);

			for(k = 0; k < 100; k++)
				ev.SetADC(939 + k, cer.hv[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1039 + k, cer.hv_c28[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1041 + k, cer.hv_cm3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1043 + k, cer.hv_ctrl_c12[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1045 + k, cer.hv_ctrl_cm3o3[k]);
			ev.SetADC(1047, cer.ps_v5_analog);
			ev.SetADC(1048, cer.ps_v5_digital);
			ev.SetADC(1049, cer.ps_vm3o3);
			ev.SetADC(1050, cer.ps_v12);
			ev.SetADC(1051, cer.ps_v28);
			ev.SetADC(1052, cer.sprs_c5);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1053 + k, cer.merger_c3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1055 + k, cer.merger_cm3o3[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1057 + k, cer.merger_c2o5[k]);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1059 + k, cer.merger_c5[k]);
			ev.SetADC(1061, cer.hk_c12);
			ev.SetADC(1062, cer.hk_cm3o3);
			for(k = 0; k < 2; k ++)
				ev.SetADC(1063 + k, cer.merger_temp[k]);
			for(k = 0; k < 14; k ++)
				ev.SetADC(1065 + k, cer.temp[k]);
				
			break;
		}
		p += sec_len;
	}

jumpout:
	ev.Convert();
	ev.CheckStatus();
	return 0;
}

int CreamHKFile::unpack_bad(unsigned char* bad_p, DHKEventConverter& ev)
{
	unsigned char bad_arr[148];
	struct _fmt5_hdr *hdr;
	static bad_data_hk* bad_ptr;

	double x_pos_iner;
	double y_pos_iner;
	double z_pos_iner;
	double vx_pos_ecf;
	double vy_pos_ecf;
	double vz_pos_ecf;
	double q1_lvlh;
	double q2_lvlh;
	double q3_lvlh;
	double q4_lvlh;
	
	//memcpy(bad_arr, bad_p, 148);

	
	// Do the header.
	hdr = (struct _fmt5_hdr *) bad_p;
	ev.SetEventFormat( hdr->fmt );
	ev.SetEventSerial( hdr->serial );
	ev.SetTimestamp(0, &(hdr->ts));
	bad_p += 20;

	
	/*myfile << hdr->ts.sec << std::endl;
	myfile << hdr->ts.min << std::endl;
	myfile << hdr->ts.hour << std::endl;
	myfile << hdr->ts.day << std::endl;
	myfile << hdr->ts.month << std::endl;
	myfile << hdr->ts.year << std::endl << std::endl;*/

	for(int i = 0; i < 40; i++)
	{
		*bad_p = 67; 
		bad_p++;
	}
	for(int i= 0; i < 40; i++)
		bad_p--;

	bad_ptr = (bad_data_hk *) bad_p; // copy

	/*memcpy(bad_arr, bad_p, 148);*/

	if(hdr->ts.year >= 2014){

			ev.SetReal(43, bad_ptr->x_pos_iner);  
			ev.SetReal(44, bad_ptr->y_pos_iner); 
			ev.SetReal(45, bad_ptr->z_pos_iner);
			ev.SetReal(46, bad_ptr->vx_pos_iner);  
			ev.SetReal(47, bad_ptr->vy_pos_iner); 
			ev.SetReal(48, bad_ptr->vz_pos_iner);
			ev.SetReal(49, bad_ptr->q1_iner);  
			ev.SetReal(50, bad_ptr->q2_iner); 
			ev.SetReal(51, bad_ptr->q3_iner);
			ev.SetReal(52, bad_ptr->q4_iner);
			ev.SetReal(53, bad_ptr->x_pos_ecf);  
			ev.SetReal(54, bad_ptr->y_pos_ecf); 
			ev.SetReal(55, bad_ptr->z_pos_ecf);
			ev.SetReal(56, bad_ptr->x_pos_ecf);  
			ev.SetReal(57, bad_ptr->y_pos_ecf); 
			ev.SetReal(58, bad_ptr->z_pos_ecf);
			ev.SetReal(59, bad_ptr->q1_ecf); 
			ev.SetReal(60, bad_ptr->q2_ecf);
			ev.SetReal(61, bad_ptr->q3_ecf);  
			ev.SetReal(62, bad_ptr->q4_ecf);
			ev.SetReal(63, bad_ptr->q1_lvlh);
			ev.SetReal(64, bad_ptr->q2_lvlh);
			ev.SetReal(65, bad_ptr->q3_lvlh);
			ev.SetReal(66, bad_ptr->q4_lvlh);
	}

	ev.Convert();
	ev.CheckStatus();
			
}

int CreamHKFile::unpack_fmt_8_full(unsigned char* p, DHKEventConverter& ev)
	{
	int sec, sec_len, k, nsec = 0;
	static hk_trg* trg_ptr;
	static hk_sw* sw_ptr;
	static hk_gps_sip* gps_ptr;
//	static hk_gps* gps_ptr;
	static hk_mtt* mtt_ptr;
	static hk_cal cal;
	static hk_tcrd tcrd; //YONAS
	static hk_hds hds;
	static hk_scd scd;
	static hk_tscd tscd;
	static hk_bscd bscd;
	static hk_tcd tcd;
	static hk_bsd bsd;
	static hk_trd trd;
	static hk_inst inst;
	static hk_dscd dscd;
	static hk_cer cer;
	struct _fmt5_hdr *hdr;
	static hk_scdl1 scdl1;   //Added by David Angelaszek 9/1/2010
	static hk_scdl2 scdl2;   //Added by David Angelaszek 9/1/2010
	static hk_scdl3 scdl3;     //Added by David Angelaszek 9/1/2010
	static hk_issnew iss;

	// Do the header.
	hdr = (struct _fmt5_hdr *) p;
	ev.SetEventFormat( hdr->fmt );
	ev.SetEventSerial( hdr->serial );
	ev.SetTimestamp(0, &(hdr->ts));
	p += 20; 

	int q =0;

	// go over sections
	while ( nsec++ < 16 ) {
		sec = ((*p) >> 4L) & 0x0f;
		sec_len = ((*p) & 0x0f) << 8L; p++;
		sec_len += (*p++) - 2;
		switch ( sec ) {
		default:
			// This is an error!!!
			goto jumpout;
		case HKSEC_EOS:
			goto jumpout;
			break;
		case HKSEC_TRG:
			trg_ptr = (hk_trg *) p; // copy
			ev.SetTimestamp(10, trg_ptr->trg_ts);
			ev.SetReal(11, trg_ptr->trg_tot);
			ev.SetReal(12, trg_ptr->trg_ext);
			ev.SetReal(13, trg_ptr->trg_clb);
			ev.SetReal(14, trg_ptr->trg_ehi);
			ev.SetReal(15, trg_ptr->trg_elo);
			ev.SetReal(16, trg_ptr->trg_zclb);

			ev.SetReal(17, trg_ptr->raw_zhi);
			ev.SetReal(18, trg_ptr->raw_cal);
			ev.SetReal(19, trg_ptr->raw_zlo);
			//ev.SetReal( 6, trg_ptr->raw_s3hi);
			//ev.SetReal( 7, trg_ptr->raw_s3lo);
			ev.SetReal( 8, trg_ptr->raw_clb);
			ev.SetReal( 9, trg_ptr->raw_ext);
			//ev.SetReal(18, trg_ptr->raw_tcrd); //YONAS // NEEDS TO BE CHANGED
			break;
		case HKSEC_SW:
			sw_ptr = (hk_sw *) p; // copy
			ev.SetTimestamp(20, sw_ptr->nio_ts);
			ev.SetReal(21, sw_ptr->nio_tkbps);
			ev.SetReal(22, sw_ptr->nio_nkbps);
			ev.SetReal(23, sw_ptr->bufl_cmdq / 256.);
			ev.SetReal(24, sw_ptr->bufl_hkq / 256.);
			ev.SetReal(25, sw_ptr->bufl_evtq / 256.);
			ev.SetReal(26, sw_ptr->bufl_datq / 256.);
			ev.SetReal(27, sw_ptr->bufl_datq2 / 256.);
			ev.SetReal(28, sw_ptr->bufl_pkt0 / 256.);
			ev.SetReal(29, sw_ptr->bufl_pkt1 / 256.);
			ev.SetReal(30, sw_ptr->bufl_pkt2 / 256.);
			//ev.SetReal(31, sw_ptr->disk_space / 256.);
			ev.SetReal(34, sw_ptr->sys_disk_space / 256.); //PGB 15 July 2008
			ev.SetInteger(31, sw_ptr->cmd_ser);
			ev.SetInteger(32, sw_ptr->cmd_code);
			ev.SetReal(35, sw_ptr->flash1);
			ev.SetReal(36, sw_ptr->flash2);
			ev.SetReal(37, sw_ptr->flash3);
			ev.SetReal(38, sw_ptr->flash4);
			ev.SetReal(39, sw_ptr->free_ram);
			break;
			//ev.SetReal(43, sw_ptr->bufl_pkt2 / 256.);
			//ev.SetReal(30, sw_ptr->disk_space / 256.);
			//ev.SetReal(34, sw_ptr->sys_disk_space / 256.); //PGB 15 July 2008
			//ev.SetInteger(31, sw_ptr->cmd_ser);
			//ev.SetInteger(32, sw_ptr->cmd_code);
			//break;
		case HKSEC_MTT:
			mtt_ptr = (hk_mtt *) p; // copy
			{
				static double live_tm_prev = 0, tot_tm_prev = 0;
				double live_tm, tot_tm;

				tot_tm = convert_timer(mtt_ptr->total, 6, _tmconv_fmt5);
				ev.SetDouble(67, format_min_sec(tot_tm));

				live_tm = convert_timer(mtt_ptr->live, 6, _tmconv_fmt5);
				ev.SetDouble(68, format_min_sec(live_tm));

				// compute differential ratio
				/*if ( (tot_tm - tot_tm_prev) != 0 )
					ev.SetDouble(33,
						((live_tm - live_tm_prev) /
						(tot_tm - tot_tm_prev)) * 100.);
				else
					ev.SetDouble(33, 999.0);*/

				if ( (tot_tm - tot_tm_prev) != 0 )
					ev.SetDouble(33,
						((live_tm - live_tm_prev) /
						(tot_tm - tot_tm_prev)) * 100.);
				else
					ev.SetDouble(33, 999.0);

				tot_tm_prev = tot_tm;
				live_tm_prev = live_tm;
			}
			
			break;
		case HKSEC_ISS:
			unpack_16bit_array(p, &iss, sec_len);
			for( k =0; k < 18; k++)
				ev.SetADC(196+k, iss.thermistor[k]);
			ev.SetADC(72, iss.sfcAcurrent);
			ev.SetADC(74, iss.sfcBcurrent);
			ev.SetADC(77, iss.sfcAtemp);
			ev.SetADC(76, iss.sfcBtemp);
			ev.SetADC(73, iss.hcAtemp);
			ev.SetADC(75, iss.hcBtemp);
			ev.SetADC(78, iss.iiuCurrent);
			ev.SetADC(79, iss.sfc_intercon_current);
			ev.SetADC(80, iss.sfcAret);

			break;
		case HKSEC_CAL:
			unpack_16bit_array(p, &cal, sec_len);
			//ev.SetADC(62, cal.pwr_logic);
			//ev.SetADC(63, cal.pwr_vm3o6);
			//ev.SetADC(64, cal.pwr_v3o6);
			//ev.SetADC(65, cal.pwr_v12);
			//ev.SetADC(66, cal.pwr_cm3o6);
			//ev.SetADC(67, cal.pwr_c3o6);
			//ev.SetADC(68, cal.pwr_c12);
			//ev.SetADC(69, cal.ctrg);

			//ev.SetADC(89, cal.reserved[0]);

			ev.SetADC(214, cal.cal_3o3v);
			ev.SetADC(215, cal.cal_p5o2v);
			ev.SetADC(216, cal.cal_12v);
			ev.SetADC(217, cal.cal_m5o2v);
			ev.SetADC(218, cal.cal_temp);
			for(int x =0; x <14; x++)
			{
				ev.SetADC(219+x, cal.cal_p5o2c[x]);
				ev.SetADC(233+x, cal.cal_m5o2c[x]);
				ev.SetADC(247+x, cal.cal_12c[x]);
			}
			ev.SetADC(261, cal.cal_gnd);


			for(k = 0; k < 8; k++) {
				ev.SetADC(92 + k, cal.temp[k]);
				ev.SetADC(100 + q, cal.hv_a[k]);
				ev.SetADC(100 + q +1, cal.hv_b[k]);
				ev.SetADC(116 + k, cal.bias[k]);
				//ev.SetADC(124 + k, cal.cm3o6[k]);
				//ev.SetADC(132 + k, cal.c3o6[k]);
				//ev.SetADC(140 + k, cal.c12[k]);
				q = q+2;
			}
			q=0;
			break;
		case HKSEC_TCRD:
			unpack_16bit_array(p, &tcrd, sec_len);

			for(k = 0; k < 4; k++) {
				//ev.SetADC(180, tcrd.temp[0]);
				ev.SetADC(180 + k, tcrd.temp[k]);
				ev.SetADC(184 + k, tcrd.tcdbias[k]);
				ev.SetADC(188 + k, tcrd.bcdbias[k]);
			} //YONAS
			break;
		
		case HKSEC_tSCD:
			unpack_16bit_array(p, &tscd, sec_len);
			//ev.SetADC(439, tscd.pwr_vlogic);
			//ev.SetADC(440, tscd.pwr_vm6o5);
			//ev.SetADC(441, tscd.pwr_v6o5);
			//ev.SetADC(442, tscd.pwr_v12);
			//ev.SetADC(443, tscd.mb_cm6o5[0]);
			//ev.SetADC(444, tscd.mb_cm6o5[1]);
			//ev.SetADC(445, tscd.mb_c6o5[0]);
			//ev.SetADC(446, tscd.mb_c6o5[1]);
			//ev.SetADC(447, tscd.mb_c12[0]);
			//ev.SetADC(448, tscd.mb_c12[1]);
			for(k = 0; k < 24; k++) {
				ev.SetADC(455 + k, tscd.temp[k]);
				ev.SetADC(481 + k, tscd.bias[k]);
				ev.SetADC(507 + k, tscd.vlogic[k]);
				ev.SetADC(533 + k, tscd.vm5[k]);
				ev.SetADC(559 + k, tscd.v5[k]);
				ev.SetADC(585 + k, tscd.cbias[k]);
				ev.SetADC(611 + k, tscd.clogic[k]);
				ev.SetADC(637 + k, tscd.cm5[k]);
				ev.SetADC(663 + k, tscd.c5[k]);
			}
			ev.SetADC(214+48, tscd.scd1_3o3v);
			ev.SetADC(215+48, tscd.scd1_p6o5v);
			ev.SetADC(216+48, tscd.scd1_12v);
			ev.SetADC(217+48, tscd.scd1_m6o5v);
			ev.SetADC(218+48, tscd.scd1_temp);
			for(int k =0; k <14; k++)
			{
				ev.SetADC(219+k+48, tscd.scd1_p6o5c[k]);
				ev.SetADC(233+k+48, tscd.scd1_m6o5c[k]);
				ev.SetADC(247+k+48, tscd.scd1_12c[k]);
			}
			ev.SetADC(261+48, tscd.scd1_gnd);
			break;
		case HKSEC_SCDL1: // David Angelaszek 9/1/2010
			unpack_16bit_array(p, &scdl1, sec_len);
			//ev.SetADC(1118, scdl1.pwr_vlogic);
			//ev.SetADC(1119, scdl1.pwr_vm6o5);
			//ev.SetADC(1120, scdl1.pwr_v6o5);
			//ev.SetADC(1121, scdl1.pwr_v12);
			//ev.SetADC(1122, scdl1.mb_cm6o5[0]);
			//ev.SetADC(1123, scdl1.mb_cm6o5[1]);
			//ev.SetADC(1124, scdl1.mb_c6o5[0]);
			//ev.SetADC(1125, scdl1.mb_c6o5[1]);
			//ev.SetADC(1126, scdl1.mb_c12[0]);
			//ev.SetADC(1127, scdl1.mb_c12[1]);
			for(k = 0; k < 24; k++) {
				ev.SetADC(1134 + k, scdl1.temp[k]);
				ev.SetADC(1160 + k, scdl1.bias[k]);
				ev.SetADC(1186 + k, scdl1.vlogic[k]);
				ev.SetADC(1212 + k, scdl1.vm5[k]);
				ev.SetADC(1238 + k, scdl1.v5[k]);
				ev.SetADC(1264 + k, scdl1.cbias[k]);
				ev.SetADC(1290 + k, scdl1.clogic[k]);
				ev.SetADC(1316 + k, scdl1.cm5[k]);
				ev.SetADC(1342 + k, scdl1.c5[k]);
			}
			ev.SetADC(214+48*2, scdl1.scd2_3o3v);
			ev.SetADC(215+48*2, scdl1.scd2_p6o5v);
			ev.SetADC(216+48*2, scdl1.scd2_12v);
			ev.SetADC(217+48*2, scdl1.scd2_m6o5v);
			ev.SetADC(218+48*2, scdl1.scd2_temp);
			for( k =0; k <14; k++)
			{
				ev.SetADC(219+k+48*2, scdl1.scd2_p6o5c[k]);
				ev.SetADC(233+k+48*2, scdl1.scd2_m6o5c[k]);
				ev.SetADC(247+k+48*2, scdl1.scd2_12c[k]);
			}
			ev.SetADC(261+48*2, scdl1.scd2_gnd);
			break;
		case HKSEC_SCDL2: // David Angelaszek 9/1/2010
			unpack_16bit_array(p, &scdl2, sec_len);
			//ev.SetADC(1368, scdl2.pwr_vlogic);
			//ev.SetADC(1369, scdl2.pwr_vm6o5);
			//ev.SetADC(1370, scdl2.pwr_v6o5);
			//ev.SetADC(1371, scdl2.pwr_v12);
			//ev.SetADC(1372, scdl2.mb_cm6o5[0]);
			//ev.SetADC(1373, scdl2.mb_cm6o5[1]);
			//ev.SetADC(1374, scdl2.mb_c6o5[0]);
			//ev.SetADC(1375, scdl2.mb_c6o5[1]);
			//ev.SetADC(1376, scdl2.mb_c12[0]);
			//ev.SetADC(1377, scdl2.mb_c12[1]);
			for(k = 0; k < 24; k++) {
				ev.SetADC(1384 + k, scdl2.temp[k]);
				ev.SetADC(1410 + k, scdl2.bias[k]);
				ev.SetADC(1436 + k, scdl2.vlogic[k]);
				ev.SetADC(1462 + k, scdl2.vm5[k]);
				ev.SetADC(1488 + k, scdl2.v5[k]);
				ev.SetADC(1514 + k, scdl2.cbias[k]);
				ev.SetADC(1540 + k, scdl2.clogic[k]);
				ev.SetADC(1566 + k, scdl2.cm5[k]);
				ev.SetADC(1592 + k, scdl2.c5[k]);
			}
			ev.SetADC(214+48*2, scdl2.scd2_3o3v);
			ev.SetADC(215+48*2, scdl2.scd2_p6o5v);
			ev.SetADC(216+48*2, scdl2.scd2_12v);
			ev.SetADC(217+48*2, scdl2.scd2_m6o5v);
			ev.SetADC(218+48*2, scdl2.scd2_temp);
			for( k =0; k <14; k++)
			{
				ev.SetADC(219+k+48*2, scdl2.scd2_p6o5c[k]);
				ev.SetADC(233+k+48*2, scdl2.scd2_m6o5c[k]);
				ev.SetADC(247+k+48*2, scdl2.scd2_12c[k]);
			}
			ev.SetADC(261+48*2, scdl2.scd2_gnd);
			break;
		case HKSEC_bSCD:
			unpack_16bit_array(p, &bscd, sec_len);
			//ev.SetADC(689, bscd.pwr_vlogic);
			//ev.SetADC(690, bscd.pwr_vm6o5);
			//ev.SetADC(691, bscd.pwr_v6o5);
			//ev.SetADC(692, bscd.pwr_v12);
			//ev.SetADC(693, bscd.mb_cm6o5[0]);
			//ev.SetADC(694, bscd.mb_cm6o5[1]);
			//ev.SetADC(695, bscd.mb_c6o5[0]);
			//ev.SetADC(696, bscd.mb_c6o5[1]);
			//ev.SetADC(697, bscd.mb_c12[0]);
			//ev.SetADC(698, bscd.mb_c12[1]);
			for(k = 0; k < 24; k++) {
				ev.SetADC(705 + k, bscd.temp[k]);
				ev.SetADC(731 + k, bscd.bias[k]);
				ev.SetADC(757 + k, bscd.vlogic[k]);
				ev.SetADC(783 + k, bscd.vm5[k]);
				ev.SetADC(809 + k, bscd.v5[k]);
				ev.SetADC(835 + k, bscd.cbias[k]);
				ev.SetADC(861 + k, bscd.clogic[k]);
				ev.SetADC(887 + k, bscd.cm5[k]);
				ev.SetADC(913 + k, bscd.c5[k]);
			}
			ev.SetADC(214+48, bscd.scd1_3o3v);
			ev.SetADC(215+48, bscd.scd1_p6o5v);
			ev.SetADC(216+48, bscd.scd1_12v);
			ev.SetADC(217+48, bscd.scd1_m6o5v);
			ev.SetADC(218+48, bscd.scd1_temp);
			for(int k =0; k <14; k++)
			{
				ev.SetADC(219+k+48, bscd.scd1_p6o5c[k]);
				ev.SetADC(233+k+48, bscd.scd1_m6o5c[k]);
				ev.SetADC(247+k+48, bscd.scd1_12c[k]);
			}
			ev.SetADC(261+48, bscd.scd1_gnd);
			break;
		
		case HKSEC_BSD:
			{
				unpack_16bit_array(p, &bsd, sec_len);
				for(k =0; k < 2; k++){
					ev.SetADC(148+k,bsd.bsd_return[k]);
				}
				for(k=0; k<4; k++){
					ev.SetADC(150+k,bsd.bsd_Q[k]);
				}
				ev.SetADC(154,bsd.bsd_curT1);
				ev.SetADC(155,bsd.bsd_p3o3V);
				ev.SetADC(156,bsd.bsd_curT2);
				ev.SetADC(157,bsd.bsd_m12V);
				ev.SetADC(158,bsd.bsd_m1o5V);

				ev.SetADC(159, bsd.bsd_sciTemp);
				ev.SetADC(160, bsd.bsd_detEnclT);
				ev.SetADC(161, bsd.bsd_eleEnclT);

				ev.SetADC(162, bsd.bsd_VHV11);
				ev.SetADC(163, bsd.bsd_IHV11);
				ev.SetADC(164, bsd.bsd_VHV12);
				ev.SetADC(165, bsd.bsd_IHV12);
				ev.SetADC(166, bsd.bsd_VHV13);
				ev.SetADC(167, bsd.bsd_IHV13);
				ev.SetADC(168, bsd.bsd_IHV14);
				ev.SetADC(169, bsd.bsd_VHV21);
				ev.SetADC(170, bsd.bsd_VHV22);
				ev.SetADC(171, bsd.bsd_VHV23);
				ev.SetADC(172, bsd.bsd_VHV31);
				ev.SetADC(173, bsd.bsd_VHV32);
				ev.SetADC(174, bsd.bsd_VHV33);
				ev.SetADC(175, bsd.bsd_VHV41);
				ev.SetADC(176, bsd.bsd_VHV42);
				ev.SetADC(177, bsd.bsd_VHV43);

				ev.SetADC(178, bsd.bsd_HVTr1);
				ev.SetADC(179, bsd.bsd_HVTr2);
				/*uint2 *pUint2p = (uint2 *)&bsd.data[0];
				unpack_16bit_array(p, &bsd.data[0], sec_len);
				for(k = 0; k < BSD_HK_ITEMS_CNT; k++)
				ev.SetADC(BSD_HK_OFFSET_AT_HKCONV + k,*pUint2p++);*/
			}
			break;				
		case HKSEC_INST:
			unpack_16bit_array(p, &inst, sec_len);
			for(k = 0; k < 12; k++)
				ev.SetADC( 358 + k, inst.temp[k]);
			break;
		
		}
		p += sec_len;
	}

jumpout:
	ev.Convert();
	ev.CheckStatus();
	return 0;
}

int CreamHKFile::unpack_fmt_2_reduced(unsigned char* p, DHKEventConverter& ev)
	{
	uint2* h;
	uint2 a[139];
	int k;
	struct _fmt5_hdr *hdr;

	// Do the header.
	hdr = (struct _fmt5_hdr *) p;
	ev.SetEventFormat( hdr->fmt );
	ev.SetEventSerial( hdr->serial );
	ev.SetTimestamp(0, &(hdr->ts));
	p += 20;

	// trigger, network speed, disk space, buffer levels.
	h = (uint2 *) p;
	ev.SetReal(11, (*h++) / 256.);
	ev.SetReal(16, (*h++) / 256.);
	ev.SetReal(14, (*h++) / 256.);
	ev.SetReal(21, (*h++) / 256.);
	ev.SetReal(30, (*h++) / 256.);
	ev.SetReal(23, (*h++) / 256.);
	ev.SetReal(24, (*h++) / 256.);
	ev.SetReal(25, (*h++) / 256.);
	ev.SetReal(26, (*h++) / 256.);
	ev.SetReal(27, (*h++) / 256.);
	ev.SetReal(28, (*h++) / 256.);
	ev.SetReal(29, (*h++) / 256.);

	// gps hour, min, sec.
	ev.SetInteger(43, (*h++));
	ev.SetInteger(44, (*h++));
	ev.SetReal(45, *((real4 *) h)); h += 2;

	// unpack
	unpack_12bit_array((unsigned char *) h, a, 210);

        if ( a[117] != 98 ) {
//      if ( ( count % 2 ) != 0 ) //PGB 25 July 2008

	// cal
	for(k = 0; k < 8; k++) ev.SetADC(90 + k, a[k]);
	for(k = 0; k < 8; k++) ev.SetADC(82 + k, a[k + 8]);
	ev.SetADC(62, a[16]);
	ev.SetADC(63, a[17]);
	ev.SetADC(64, a[18]);
	ev.SetADC(65, a[19]);
	ev.SetADC(66, a[20]);
	ev.SetADC(67, a[21]);
	ev.SetADC(68, a[22]);
	ev.SetADC(69, a[23]);
	
	// tscd
	for(k = 0; k < 26; k++) ev.SetADC(481 + k, a[k + 24]);
	ev.SetADC(439, a[50]);
	ev.SetADC(440, a[51]);
	ev.SetADC(441, a[52]);
	ev.SetADC(442, a[53]);
	/*	ev.SetADC(443, a[54]);
	ev.SetADC(444, a[55]);
	ev.SetADC(445, a[56]);
	ev.SetADC(446, a[57]);
	ev.SetADC(447, a[58]);
	ev.SetADC(448, a[59]); */
	ev.SetADC(559, a[54]);
        ev.SetADC(533, a[55]);
        ev.SetADC(507, a[56]);
        ev.SetADC(663, a[57]);
        ev.SetADC(637, a[58]);
        ev.SetADC(611, a[59]);
	


	

	// bscd
	for(k = 0; k < 26; k++) ev.SetADC(731 + k, a[k + 60]);
	ev.SetADC(689, a[86]);    
	ev.SetADC(690, a[87]);    
	ev.SetADC(691, a[88]);   
	ev.SetADC(692, a[89]);       
/*	ev.SetADC(693, a[90]);
	ev.SetADC(694, a[91]);
	ev.SetADC(695, a[92]);
	ev.SetADC(696, a[93]);
	ev.SetADC(697, a[94]);
	ev.SetADC(698, a[95]); */

	ev.SetADC(809, a[90]);     
        ev.SetADC(783, a[91]);
        ev.SetADC(757, a[92]);
        ev.SetADC(913, a[93]);
        ev.SetADC(887, a[94]);
        ev.SetADC(861, a[95]);

	// tcd
	ev.SetADC(407, a[96]);
	ev.SetADC(408, a[97]);
	ev.SetADC(409, a[98]);
	ev.SetADC(410, a[99]);
	ev.SetADC(411, a[100]);
	ev.SetADC(412, a[101]);
	for(k = 0; k < 5; k++)
		ev.SetADC(413 + k, a[102 + k]);

	// instrument
	for(k = 0; k < 12; k++)
		ev.SetADC(423 + k, a[107 + k]);
	ev.SetADC(418, a[119]);
	ev.SetADC(419, a[120]);

	}//PGB 25 July 2008
                                                                                                                             
   	else {// PGB 25 July 2008
                                                                                                                             
        // tcd high voltages
                for(k =0; k < 10; k++)
                        ev.SetADC(1079 + k, a[k]);
        // chercam
                for(k = 0; k < 100; k++)
                        ev.SetADC(939 + k, a[k+10]);
               for(k = 0; k < 4; k++)
                        ev.SetADC(1066 + k, a[k+110]);
        // sfc system disk space
//        ev.SetReal(34,  (a[114]/ 256.) );
	  ev.SetReal(34, (( a[114] ) /256. ) );// PGB 21 Nov 2008
//	  ev.SetDouble(34, a[114] );
        // live time and total time
        ev.SetDouble(60, a[115]);
        ev.SetDouble(61, a[116]);
        //Special Value
        ev.SetADC(1065, a[117]);
                                                                                                                             
	}
//        count++;//PGB 25 July 2008

	ev.Convert();
	ev.CheckStatus();
	return 0;
}
int CreamHKFile::unpack_fmt_6_reduced(unsigned char* p, DHKEventConverter& ev)
	{
	uint2* h;
	uint2 a[139];
	int k;
	struct _fmt5_hdr *hdr; //header format of fmt5_hdr the same

	// Do the header.
	hdr = (struct _fmt5_hdr *) p;
	ev.SetEventFormat( hdr->fmt );
	ev.SetEventSerial( hdr->serial );
	ev.SetTimestamp(0, &(hdr->ts));
	p += 20;

	// trigger, network speed, disk space, buffer levels.
	h = (uint2 *) p;
	ev.SetReal(11, (*h++) / 256.);
	ev.SetReal(16, (*h++) / 256.);
	ev.SetReal(14, (*h++) / 256.);
	ev.SetReal(21, (*h++) / 256.);
	ev.SetReal(30, (*h++) / 256.);
	ev.SetReal(23, (*h++) / 256.);
	ev.SetReal(24, (*h++) / 256.);
	ev.SetReal(25, (*h++) / 256.);
	ev.SetReal(26, (*h++) / 256.);
	ev.SetReal(27, (*h++) / 256.);
	ev.SetReal(28, (*h++) / 256.);
	ev.SetReal(29, (*h++) / 256.);

	// gps hour, min, sec.
	// gps timeOff, weekNo, weekTime.
	ev.SetInteger(43, *((uint4 *) h)); h +=2;
	ev.SetInteger(44, *((uint4 *) h)); h +=2;
	ev.SetInteger(45, *((uint4 *) h)); h +=2;
	//ev.SetReal(45, *((real4 *) h)); h += 2;

	// unpack
	unpack_12bit_array((unsigned char *) h, a, 210);

        if ( a[117] != 98 ) {
//      if ( ( count % 2 ) != 0 ) //PGB 25 July 2008

	// cal
	for(k = 0; k < 8; k++) ev.SetADC(90 + k, a[k]);
	for(k = 0; k < 8; k++) ev.SetADC(82 + k, a[k + 8]);
	ev.SetADC(62, a[16]);
	ev.SetADC(63, a[17]);
	ev.SetADC(64, a[18]);
	ev.SetADC(65, a[19]);
	ev.SetADC(66, a[20]);
	ev.SetADC(67, a[21]);
	ev.SetADC(68, a[22]);
	ev.SetADC(69, a[23]);
	
	// tscd
	for(k = 0; k < 26; k++) ev.SetADC(481 + k, a[k + 24]);
	ev.SetADC(439, a[50]);
	ev.SetADC(440, a[51]);
	ev.SetADC(441, a[52]);
	ev.SetADC(442, a[53]);
/*	ev.SetADC(443, a[54]);
	ev.SetADC(444, a[55]);
	ev.SetADC(445, a[56]);
	ev.SetADC(446, a[57]);
	ev.SetADC(447, a[58]);
	ev.SetADC(448, a[59]); */

	ev.SetADC(559, a[54]);
        ev.SetADC(533, a[55]);
        ev.SetADC(507, a[56]);
        ev.SetADC(663, a[57]);
        ev.SetADC(637, a[58]);
        ev.SetADC(611, a[59]);

	// bscd
	for(k = 0; k < 26; k++) ev.SetADC(731 + k, a[k + 60]);
	ev.SetADC(689, a[86]);
	ev.SetADC(690, a[87]);
	ev.SetADC(691, a[88]);
	ev.SetADC(692, a[89]);
/*	ev.SetADC(693, a[90]);
	ev.SetADC(694, a[91]);
	ev.SetADC(695, a[92]);
	ev.SetADC(696, a[93]);
	ev.SetADC(697, a[94]);
	ev.SetADC(698, a[95]); */

	ev.SetADC(809, a[90]);
        ev.SetADC(783, a[91]);
        ev.SetADC(757, a[92]);
        ev.SetADC(913, a[93]);
        ev.SetADC(887, a[94]);
        ev.SetADC(861, a[95]);

	ev.SetADC(5590, a[54]);   //Added by David Angelaszek 9/1/2010
        ev.SetADC(5330, a[55]);   //Added by David Angelaszek 9/1/2010
        ev.SetADC(5070, a[56]);   //Added by David Angelaszek 9/1/2010
        ev.SetADC(6630, a[57]);   //Added by David Angelaszek 9/1/2010
        ev.SetADC(6370, a[58]);   //Added by David Angelaszek 9/1/2010
        ev.SetADC(6110, a[59]);   //Added by David Angelaszek 9/1/2010

	// tcd
	ev.SetADC(407, a[96]);
	ev.SetADC(408, a[97]);
	ev.SetADC(409, a[98]);
	ev.SetADC(410, a[99]);
	ev.SetADC(411, a[100]);
	ev.SetADC(412, a[101]);
	for(k = 0; k < 5; k++)
		ev.SetADC(413 + k, a[102 + k]);

	// instrument
	for(k = 0; k < 12; k++)
		ev.SetADC(423 + k, a[107 + k]);
	ev.SetADC(418, a[119]);
	ev.SetADC(419, a[120]);

	}//PGB 25 July 2008
                                                                                                                             
   	else {// PGB 25 July 2008
                                                                                                                             
        // tcd high voltages
                for(k =0; k < 10; k++)
                        ev.SetADC(1079 + k, a[k]);
        // chercam
                for(k = 0; k < 100; k++)
                        ev.SetADC(939 + k, a[k+10]);
               for(k = 0; k < 4; k++)
                        ev.SetADC(1066 + k, a[k+110]);
        // sfc system disk space
//        ev.SetReal(34,  (a[114]/ 256.) );
	  ev.SetReal(34, (( a[114] ) /256. ) );// PGB 21 Nov 2008
//	  ev.SetDouble(34, a[114] );
        // live time and total time
        ev.SetDouble(60, a[115]);
        ev.SetDouble(61, a[116]);
        //Special Value
        ev.SetADC(1065, a[117]);
                                                                                                                             
	}
//        count++;//PGB 25 July 2008

	ev.Convert();
	ev.CheckStatus();
	return 0;
}

int CreamHKFile::unpack_fmt_8_reduced(unsigned char* p, DHKEventConverter& ev)
	{
	uint2* h;
	uint2 a[242];
	int k;
	struct _fmt5_hdr *hdr; //header format of fmt5_hdr the same

	// Do the header.
	hdr = (struct _fmt5_hdr *) p;
	ev.SetEventFormat( hdr->fmt );
	ev.SetEventSerial( hdr->serial );
	ev.SetTimestamp(0, &(hdr->ts));
	p += 20;

	// trigger, network speed, disk space, buffer levels.
	h = (uint2 *) p;
	ev.SetReal(11, (*h++) / 256.);//
	ev.SetReal(16, (*h++) / 256.);//
	ev.SetReal(14, (*h++) / 256.);//
	ev.SetReal(21, (*h++) / 256.);//
	ev.SetReal(23, (*h++) / 256.); 
	ev.SetReal(24, (*h++) / 256.);
	ev.SetReal(25, (*h++) / 256.);
	ev.SetReal(26, (*h++) / 256.);
	ev.SetReal(27, (*h++) / 256.);
	ev.SetReal(28, (*h++) / 256.);
	ev.SetReal(29, (*h++) / 256.);
        ev.SetReal(30, (*h++) / 256.);
	ev.SetReal(35, (*h++) / 256.);
	ev.SetReal(36, (*h++) / 256.);
	ev.SetReal(37, (*h++) / 256.);
	ev.SetReal(38, (*h++) / 256.);

	// gps hour, min, sec.
	// gps timeOff, weekNo, weekTime.
	//ev.SetInteger(43, *((uint4 *) h)); h +=2;
	//ev.SetInteger(44, *((uint4 *) h)); h +=2;
	//ev.SetInteger(45, *((uint4 *) h)); h +=2;
	//ev.SetReal(45, *((real4 *) h)); h += 2;

	// unpack
	unpack_12bit_array((unsigned char *) h, a, 210);
	int q =0;

        if ( a[117] != 98 ) {
//      if ( ( count % 2 ) != 0 ) //PGB 25 July 2008

	// cal
	//for(k = 0; k < 8; k++) ev.SetADC(90 + k, a[k]);
	//for(k = 0; k < 8; k++) ev.SetADC(82 + k, a[k + 8]);
	for(k = 0; k < 8; k++) ev.SetADC(92 + k, a[k]);
	for(k = 0; k < 8; k++){ ev.SetADC(100 + q, a[k+8]);
				ev.SetADC(100 + q +1, a[k+16]);
				q = q+2;
	}
	q =0;
 
	//tcd-bcd
	ev.SetADC(214, a[24]);
	ev.SetADC(215, a[25]);
	ev.SetADC(216, a[26]);
	ev.SetADC(217, a[27]);
	ev.SetADC(218, a[28]);
	for(k = 0; k < 4; k++) ev.SetADC(184+k, a[29 +k]);
	for(k = 0; k < 4; k++) ev.SetADC(188+k, a[33+k]);
	for(k = 0; k < 4; k++) ev.SetADC(180+k, a[37 +k]);
	
	// tscd
	for(k = 0; k < 24; k++) ev.SetADC(481 + k, a[41+k]);
	ev.SetADC(262, a[65]);
	ev.SetADC(263, a[66]);
	ev.SetADC(264, a[67]);
	ev.SetADC(265, a[68]);
	ev.SetADC(266, a[69]);
/*	ev.SetADC(443, a[54]);
	ev.SetADC(444, a[55]);
	ev.SetADC(445, a[56]);
	ev.SetADC(446, a[57]);
	ev.SetADC(447, a[58]);
	ev.SetADC(448, a[59]); */

	ev.SetADC(559, a[70]);
        ev.SetADC(533, a[71]);
        ev.SetADC(507, a[72]);
        ev.SetADC(663, a[73]);
        ev.SetADC(637, a[74]);
        ev.SetADC(611, a[75]);

	// bscd
	for(k = 0; k < 24; k++) ev.SetADC(731 + k, a[k + 76]);
	ev.SetADC(262, a[65]);
	ev.SetADC(263, a[66]);
	ev.SetADC(264, a[67]);
	ev.SetADC(265, a[68]);
	ev.SetADC(266, a[69]);
/*	ev.SetADC(693, a[90]);
	ev.SetADC(694, a[91]);
	ev.SetADC(695, a[92]);
	ev.SetADC(696, a[93]);
	ev.SetADC(697, a[94]);
	ev.SetADC(698, a[95]); */

	ev.SetADC(809, a[100]);
        ev.SetADC(783, a[101]);
        ev.SetADC(757, a[102]);
        ev.SetADC(913, a[103]);
        ev.SetADC(887, a[104]);
        ev.SetADC(861, a[105]);

	//scdl1
	for(k = 0; k < 24; k++) ev.SetADC(1160 + k, a[k + 106]);
	ev.SetADC(310, a[130]);
	ev.SetADC(311, a[131]);
	ev.SetADC(312, a[132]);
	ev.SetADC(313, a[133]);
	ev.SetADC(314, a[134]);
/*	ev.SetADC(693, a[90]);
	ev.SetADC(694, a[91]);
	ev.SetADC(695, a[92]);
	ev.SetADC(696, a[93]);
	ev.SetADC(697, a[94]);
	ev.SetADC(698, a[95]); */

	ev.SetADC(1238, a[135]);
        ev.SetADC(1212, a[136]);
        ev.SetADC(1186, a[137]);
        ev.SetADC(1342, a[138]);
        ev.SetADC(1316, a[139]);
        ev.SetADC(1290, a[140]);

	//scdl2
	for(k = 0; k < 24; k++) ev.SetADC(1410 + k, a[k + 141]);
	ev.SetADC(310, a[130]);
	ev.SetADC(311, a[131]);
	ev.SetADC(312, a[132]);
	ev.SetADC(313, a[133]);
	ev.SetADC(314, a[134]);
/*	ev.SetADC(693, a[90]);
	ev.SetADC(694, a[91]);
	ev.SetADC(695, a[92]);
	ev.SetADC(696, a[93]);
	ev.SetADC(697, a[94]);
	ev.SetADC(698, a[95]); */

	ev.SetADC(1488, a[165]);
        ev.SetADC(1462, a[166]);
        ev.SetADC(1186, a[167]);
        ev.SetADC(1592, a[168]);
        ev.SetADC(1566, a[169]);
        ev.SetADC(1540, a[170]);

	// instrument
	for(k = 0; k < 12; k++)
		ev.SetADC(358 + k, a[171 + k]);
	for(k = 0; k < 18; k++)
		ev.SetADC(196 + k, a[183 + k]);

	ev.SetADC(72, a[201]);
	ev.SetADC(73, a[202]);
	ev.SetADC(74, a[203]);
	ev.SetADC(75, a[204]);
	ev.SetADC(76, a[205]);
	ev.SetADC(77, a[206]);

	// bsd
	ev.SetADC(155, a[207]);
	ev.SetADC(157, a[208]);
	ev.SetADC(158, a[209]);

	ev.SetADC(162, a[210]);
	ev.SetADC(163, a[211]);
	ev.SetADC(164, a[212]);
	ev.SetADC(165, a[213]);
	ev.SetADC(166, a[214]);
	ev.SetADC(167, a[215]);
	ev.SetADC(168, a[216]);
	ev.SetADC(169, a[217]);
	ev.SetADC(170, a[218]);
	ev.SetADC(171, a[219]);
	ev.SetADC(172, a[220]);
	ev.SetADC(173, a[221]);
	ev.SetADC(174, a[222]);
	ev.SetADC(175, a[223]);
	ev.SetADC(176, a[224]);
	ev.SetADC(177, a[225]);

	
	ev.SetADC(418, a[119]);
	ev.SetADC(419, a[120]);

	}//PGB 25 July 2008
                                                                                                                             
   	else {// PGB 25 July 2008
                                                                                                                             
        // tcd high voltages
                for(k =0; k < 10; k++)
                        ev.SetADC(1079 + k, a[k]);
        // chercam
                for(k = 0; k < 100; k++)
                        ev.SetADC(939 + k, a[k+10]);
               for(k = 0; k < 4; k++)
                        ev.SetADC(1066 + k, a[k+110]);
        // sfc system disk space
//        ev.SetReal(34,  (a[114]/ 256.) );
	  ev.SetReal(34, (( a[114] ) /256. ) );// PGB 21 Nov 2008
//	  ev.SetDouble(34, a[114] );
        // live time and total time
        ev.SetDouble(60, a[115]);
        ev.SetDouble(61, a[116]);
        //Special Value
        ev.SetADC(1065, a[117]);
                                                                                                                             
	}
//        count++;//PGB 25 July 2008

	ev.Convert();
	ev.CheckStatus();
	return 0;
}

int CreamHKFile::read_fmt_5(DHKEventConverter& ev)
	{
	int c;
	struct _fmt5_hdr *p; // 20 bytes

	/* check EOF */
	if ( (c = fgetc(_fp)) == EOF ) return -1;
	else ungetc(c, _fp);

	/* read eight bytes to get the header information */
	fread(ev.GetEventBufferPtr(), sizeof(char), 8, _fp);
	p = (struct _fmt5_hdr *) ev.GetEventBufferPtr();
	fread(ev.GetEventBufferPtr() + 8, sizeof(char), p->length - 8, _fp);
	ev.SetEventLength( p->length );
	ev.SetEventCRC16( p->crc16 );
	ev.SetEventType( p->type );

	switch ( p->type ) {
	case DAT_HK: // full hk
		unpack_fmt_5_full(((unsigned char *) ev.GetEventBufferPtr()), ev);
		break;
	case DAT_HK2: // reduced hk
		unpack_fmt_2_reduced(((unsigned char *) ev.GetEventBufferPtr()), ev);
		break;
	case DAT_TCF: // TCD config
		// nothing todo
		break;
	default:
		fprintf(stderr, "File contains events non-related to housekeeping.\n");
		return -1;
		break;
	}
	return 0;
}
int CreamHKFile::read_fmt_6(DHKEventConverter& ev)
	{
	int c;
	struct _fmt5_hdr *p; // 20 bytes

	/* check EOF */
	if ( (c = fgetc(_fp)) == EOF ) return -1;
	else ungetc(c, _fp);

	/* read eight bytes to get the header information */
	fread(ev.GetEventBufferPtr(), sizeof(char), 8, _fp);
	p = (struct _fmt5_hdr *) ev.GetEventBufferPtr();
	fread(ev.GetEventBufferPtr() + 8, sizeof(char), p->length - 8, _fp);
	ev.SetEventLength( p->length );
	ev.SetEventCRC16( p->crc16 );
	ev.SetEventType( p->type );

	switch ( p->type ) {
	case DAT_HK: // full hk
		unpack_fmt_6_full(((unsigned char *) ev.GetEventBufferPtr()), ev);//Dr.L
		break;
	case DAT_HK2: // reduced hk
		unpack_fmt_6_reduced(((unsigned char *) ev.GetEventBufferPtr()), ev);
//		unpack_fmt_2_reduced(((unsigned char *) ev.GetEventBufferPtr()), ev);
		break;
	case DAT_TCF: // TCD config
		// nothing todo
		break;
	default:
		fprintf(stderr, "File contains events non-related to housekeeping.\n");
		return -1;
		break;
	}
	return 0;
}

int CreamHKFile::read_fmt_8(DHKEventConverter& ev)
	{
	int c;
	struct _fmt5_hdr *p; // 20 bytes

	/* check EOF */
	if ( (c = fgetc(_fp)) == EOF ) return -1;
	else ungetc(c, _fp);

	/* read eight bytes to get the header information */
	fread(ev.GetEventBufferPtr(), sizeof(char), 8, _fp);
	p = (struct _fmt5_hdr *) ev.GetEventBufferPtr();
	fread(ev.GetEventBufferPtr() + 8, sizeof(char), p->length - 8, _fp);
	ev.SetEventLength( p->length );
	ev.SetEventCRC16( p->crc16 );
	ev.SetEventType( p->type );

	//printf("p %x p->type %x\n", p, p->type);
	switch ( p->type ) {
	case DAT_HK: // full hk
		unpack_fmt_8_full(((unsigned char *) ev.GetEventBufferPtr()), ev);//Dr.L
		break;
	case DAT_HK2: // reduced hk
		unpack_fmt_8_reduced(((unsigned char *) ev.GetEventBufferPtr()), ev);
//		unpack_fmt_2_reduced(((unsigned char *) ev.GetEventBufferPtr()), ev);
		break;
	case DAT_TCF: // TCD config
		// nothing todo
		break;
	default:
		fprintf(stderr, "File contains events non-related to housekeeping.\n");
		return -1;
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// For current version
//////////////////////////////////////////////////////////////////////////////

CreamHKFile::CreamHKFile()
	{
	_fp = 0;
	_filesz = 0;

	// total and live timer conversion factor
	_tmconv_fmt1 = 12.0 / (3.6864e6 * 60.0); // in min.

	// 25 Aug 2006
	// The USB master trigger uses a quarter microsecond per tick.
	_tmconv_fmt5 = 0.25e-6 / 60.0; // in min.
}

CreamHKFile::~CreamHKFile()
	{
	CreamHKFile::Close();
}

int CreamHKFile::OpenRead(char* filename)
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
			fclose( _fp ); _fp = 0;//Added by David Angelaszek 9/1/2010;
			return -1;
		}
	}

	return 0;
}

int CreamHKFile::Close(void)
	{
	if ( _fp ) {
		fclose( _fp ); _fp = 0;
	}

	return 0;
}

long CreamHKFile::GetCurrentFilePosition(void)
	{
	return _fp ? ftell( _fp ) : 0;
}

// Rewind file pointer so that
// subsequent call to  ReadEvent()  would read the first event data.
int CreamHKFile::Rewind(void)
	{
	if ( _fp ) {
		switch ( GetFormatVersion() ) {
//		case 0x0000: fseek(_fp, CREAMHKFILE_HEADER_LENGTH_V0, SEEK_SET); break;
		case 0x0100: fseek(_fp, CREAMHKFILE_HEADER_LENGTH_V1, SEEK_SET); break;
		case 0x0200: fseek(_fp, CREAMHKFILE_HEADER_LENGTH_V2, SEEK_SET); break;
		case 0x0300:
		case 0x0303:
		case 0x0304:
		case 0x0305:
			     fseek(_fp, CREAMHKFILE_HEADER_LENGTH_V3, SEEK_SET); break;
		case 0x0400: fseek(_fp, CREAMHKFILE_HEADER_LENGTH_V4, SEEK_SET); break;
		case 0x0500: fseek(_fp, CREAMHKFILE_HEADER_LENGTH_V5, SEEK_SET); break;
		case 0x0600: fseek(_fp, CREAMHKFILE_HEADER_LENGTH_V6, SEEK_SET); break;
		case 0x0800: fseek(_fp, CREAMHKFILE_HEADER_LENGTH_V8, SEEK_SET); break;
		default: return -1;
		}
	}

	return 0;
}

/* read an event */
int CreamHKFile::ReadEvent(DHKEventConverter& ev)
	{
	if ( _fp == 0 ) return -1;

	/* select reader function */
	switch ( GetFormatVersion() ) {
//	case 0x0000: if ( read_fmt_0() ) return -1; break;
	case 0x0100: if ( read_fmt_1(ev) ) return -1; break;
	case 0x0200: if ( read_fmt_2(ev) ) return -1; break;
	case 0x0300:
	case 0x0303: if ( read_fmt_3_typefirst(ev) ) return -1; break;
	case 0x0304:
	case 0x0305: if ( read_fmt_3_lengthfirst(ev) ) return -1; break;
	case 0x0400: if ( read_fmt_4(ev) ) return -1; break;
	case 0x0500: if ( read_fmt_5(ev) ) return -1; break;
	case 0x0600: if ( read_fmt_6(ev) ) return -1; break;
	case 0x0800: if ( read_fmt_8(ev) ) return -1; break;
	default: return -1; break;
	}
	
	// TODO
	// copy to assigned channels.
	return 0;
}
