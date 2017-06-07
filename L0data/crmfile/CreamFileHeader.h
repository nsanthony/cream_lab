#ifndef CREAMFILEHEADER_H
#define CREAMFILEHEADER_H
//
// CreamFileHeader.h
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
// $Log: CreamFileHeader.h,v $
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
// Revision 1.1.1.1  2009/01/20 08:12:10  creamop
// hahaha
//
// Revision 1.9  2007/02/21 00:09:04  juneki
// added CER pedestal and led runs
//
// Revision 1.8  2006/07/20 16:15:55  syzinn
// added pedestal and charge calibration run for silicon beam tracker.
//
// Revision 1.7  2006/03/24 14:42:37  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.6.2.1  2006/03/14 21:07:38  syzinn
// eight-byte timestamp
//
// Revision 1.6  2005/10/29 00:27:15  cvs
// format 3.2 to implement run number
//
// Revision 1.5  2005/04/08 16:33:47  cvs
// supports format 3.1
//
// Revision 1.4  2005/04/07 23:02:29  cvs
// keep track of header length
//
// Revision 1.3  2005/02/28 02:10:17  daq
// changed to format 3.0
//
// Revision 1.2  2005/02/25 22:54:26  syzinn
// added run types 14 and 15
//
// Revision 1.1.1.1  2005/02/04 05:02:17  daq
// initial version for CREAM2.
//
// Revision 1.7  2004/11/19 21:53:56  syzinn
// introduced TRD pedestal run.
//
// Revision 1.6  2004/10/07 12:00:12  syzinn
// added CREAM_ARCHIVE_RUN
//
// Revision 1.5  2004/08/14 08:39:52  syzinn
// Macros for pedestal files are added.
//
// Revision 1.4  2004/04/15 20:59:02  syzinn
// added copyright notice.
//
// Revision 1.3  2004/04/09 17:37:43  syzinn
// Added CAL and HDS LED runs
//
// Revision 1.2  2004/02/27 19:21:09  syzinn
// no message.
//
// Revision 1.1.1.1  2004/02/24 21:46:25  syzinn
// crmfile library
//
//

/* latest file format number */
#define CREAMFILE_CUR_MAJOR 8
#define CREAMFILE_CUR_MINOR 0

/* Event composition for formats 0.xx and 1.xx was stored to the header */
#define FMT1_EVENT_CALORIMETER	0x0001
#define FMT1_EVENT_CAMAC	0x0002
#define FMT1_EVENT_TRIGGER	0x0004
#define FMT1_EVENT_HODOSCOPE	0x0008
#define FMT1_EVENT_SCD		0x0010
#define FMT1_EVENT_TIMESTAMP	0x0020
#define FMT1_EVENT_TCRD 	0x0400 //YONAS
#define FMT1_EVENT_SPECIAL	0x8000

/* CREAM run type and file extensions for formats 0.xx and 1.xx */
#define FMT1_CREAM_NORMAL_RUN		0	/* dat */
#define FMT1_CREAM_PEDESTAL_RUN		1	/* ped */
#define FMT1_CREAM_CAL_ZCAL_RUN		2	/* c.zcal */
#define FMT1_CREAM_HDS_ZCAL_RUN		3	/* h.zcal */
#define FMT1_CREAM_SCD_ZCAL_RUN		4	/* s.zcal */
#define FMT1_CREAM_TRIG_EFF_RUN		5	/* tef */
#define FMT1_CREAM_TRIG_GAIN_RUN	6	/* tgn */
#define FMT1_CREAM_TCRD_ZCAL_RUN	7	/* tcrd.zcal */ //YONAS

/* CREAM run type and file extensions for formats 2.xx and 3.xx */
#define CREAM_NORMAL_RUN	0	/* dat */
#define CREAM_PEDESTAL_RUN	1	/* ped (or pds) */
#define CREAM_CAL_ZCAL_RUN	2	/* c.zcal */
#define CREAM_HDS_ZCAL_RUN	3	/* h.zcal */
#define CREAM_SCD_ZCAL_RUN	4	/* s.zcal */
#define CREAM_TRIG_EFF_RUN	5	/* tef */
#define CREAM_TRIG_GAIN_RUN	6	/* tgn */
#define CREAM_CAL_LED_RUN	7	/* c.led */
#define CREAM_HDS_LED_RUN	8	/*SCD h.led */
#define CREAM_CAL_PEDESTAL_RUN	9	/* c.pds */
#define CREAM_HDS_PEDESTAL_RUN	10	/* h.pds */
#define CREAM_SCD_PEDESTAL_RUN	11	/* s.pds */
#define CREAM_ARCHIVE_RUN 	12	/* arc */
#define CREAM_TRD_PEDESTAL_RUN	13	/* t.pds */
#define CREAM_RAW_RUN		14	/* raw */
#define CREAM_HK_RUN		15	/* hk */
#define CREAM_tSCD_ZCAL_RUN	16	/* ts.zcal */
#define CREAM_bSCD_ZCAL_RUN	17	/* bs.zcal */
#define CREAM_tSCD_PEDESTAL_RUN	18	/* ts.pds */
#define CREAM_bSCD_PEDESTAL_RUN	19	/* bs.pds */
#define CREAM_SBT_PEDESTAL_RUN	20	/* b.pds */
#define CREAM_SBT_ZCAL_RUN	21	/* b.zcal */
#define CREAM_CER_PEDESTAL_RUN	22	/* k.pds */
#define CREAM_CER_LED_RUN	23	/* k.led */
#define CREAM_SCDL1_ZCAL_RUN	24                  //Added by David Angelaszek 9/1/2010
#define CREAM_SCDL2_ZCAL_RUN	25	            //Added by David Angelaszek 9/1/2010
#define CREAM_SCDL3_ZCAL_RUN	26	            //Added by David Angelaszek 9/1/2010
#define CREAM_SCDL1_PEDESTAL_RUN 27	            //Added by David Angelaszek 9/1/2010
#define CREAM_SCDL2_PEDESTAL_RUN 28                 //Added by David Angelaszek 9/1/2010
#define CREAM_SCDL3_PEDESTAL_RUN 29                 //Added by David Angelaszek 9/1/2010
#define CREAM_TCRD_ZCAL_RUN	30	/* tcrd.zcal */ //YONAS 
#define CREAM_TCRD_LED_RUN	31	/* tcrd.led */ //YONAS
#define CREAM_TCRD_PEDESTAL_RUN	32	/* tcrd.pds */ //YONAS
#define CREAM_TRIG_EFF_RUN_TCRD	33	/* tef_tcrd */
#define CREAM_TRIG_GAIN_RUN_TCRD	34	/* tgn_tcrd */
#define CREAM_BSD_LED_RUN	35	/* bsd.led */

/* structure for header of versions 0, 1, and 2. */
typedef struct {
	char		id[5];	/* "CREAM" */
	unsigned char	major;
	unsigned char	minor;
	unsigned short	year;	/* 2003, 2004, etc. */
	unsigned char	month;	/* 1, 2, ..., 12. */
	unsigned char	day;	/* 1, 2, ..., 31. */
	unsigned char	hour;	/* 0, 1, ..., 23. */
	unsigned char	min;	/* 0, 1, ..., 59. */
	unsigned char	sec;	/* 0, 1, ..., 59. */
	unsigned short	ref_year;
	unsigned char	run;	/* run type */
	unsigned char	beam;
	float		momentum;
	float		trig_rate;
	float		tbl_xpos;
	float		tbl_ypos;
	float		high_vol[20];
	unsigned	bias_status;
	unsigned char	sprs_status;
} cream_run_header_2;

/* Structure for header for version 3 */
typedef struct {
	char		id[5];	/* "CREAM" */
	unsigned char	major;
	unsigned char	minor;
	unsigned short	year;	/* 2003, 2004, etc. */
	unsigned char	month;	/* 1, 2, ..., 12. */
	unsigned char	day;	/* 1, 2, ..., 31. */
	unsigned char	hour;	/* 0, 1, ..., 23. */
	unsigned char	min;	/* 0, 1, ..., 59. */
	unsigned char	sec;	/* 0, 1, ..., 59. */
	unsigned short	ref_year;
	unsigned char	run;	/* run type */
	unsigned short	runno;	/* run number */
} cream_run_header;

#define CREAMFILE_RUN_HEADER_LENGTH_V0 128
#define CREAMFILE_RUN_HEADER_LENGTH_V1 128
#define CREAMFILE_RUN_HEADER_LENGTH_V2 128
#define CREAMFILE_RUN_HEADER_LENGTH_V3 32
#define CREAMFILE_RUN_HEADER_LENGTH 32 /* current version */
class CreamFileHeader {
	// directly from/to file
	char _header_cbuf[128];

protected:
	// unpacked header information
	union {
		struct {
			char		id[5];	/* "CREAM" */
			unsigned char	major;
			unsigned char	minor;
		} any;
			
		cream_run_header_2 v1;
		cream_run_header_2 v2;
		cream_run_header   v3;
	} _h;
	unsigned char _evc_1; // format 1 only
	int _run_date;

	int init_header();
	int read_header(FILE* fp);
	int check_header();
	int read_header_1();
	int print_run_header_1(FILE* fp);
	int read_header_2();
	int print_run_header_2(FILE* fp);
	int read_header_3();
	int print_run_header_3(FILE* fp);

public:
	CreamFileHeader();
	virtual ~CreamFileHeader();

	inline int GetRunDate(void) { return _run_date; }
	inline unsigned short GetFormatVersion(void) { return (_h.any.major << 8) + _h.any.minor; }
	int FillRunHeader(cream_run_header*);
	inline int FillRunHeader(void) { return FillRunHeader(&_h.v3); }
	int PrintRunHeader(FILE* fp);
	int WriteRunHeader(FILE* fp, cream_run_header* h);
	inline int WriteRunHeader(FILE* fp) { return WriteRunHeader(fp, &_h.v3); }
	inline cream_run_header* GetRunHeader(void) { return &_h.v3; }
};

#endif
