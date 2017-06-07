#ifndef CREAMHKFILEHEADER_H
#define CREAMHKFILEHEADER_H
//
// CreamHKFileHeader.h
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
// $Log: CreamHKFileHeader.h,v $
// Revision 1.3  2012/11/16 21:11:55  dja1987
// Fixes to TCD/BCD crmfile. Updated HK format. Improved client playback (no more temporary arrays for missing packet numbers). Updated cprint
//
// Revision 1.2  2012/10/18 14:49:18  dja1987
// Checking in some bug fixes for playback. Playback will now send playback packets event when we aren't dropping packets (i.e. we don't always have to wait for a specific number of packets that need to be playbacked). Also checking in some beginning changes for USB commanding
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
// Revision 1.2  2006/03/24 14:42:38  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.1.4.1  2006/03/14 21:07:41  syzinn
// eight-byte timestamp
//
// Revision 1.1  2005/06/06 21:30:59  cvs
// new
//
//
//

// File header is available from format 4 introduced on 3 June 2005.
// Formats 0, 1, 2, and 3 can be determined by looking at the format number
// of hk events.  For format 2 and 3 the difference of hk events are
// the order of byte length, type, and event format number.  Prior to format
// 3, type and event format number appears before the byte length.  From event
// format 3 and higher, the byte length proceeds type and event format number.

/* latest file format number */
#define CREAMHKFILE_CUR_MAJOR 8
//#define CREAMHKFILE_CUR_MAJOR 5 CRAM 4
#define CREAMHKFILE_CUR_MINOR 0

/* header length */
#define CREAMHKFILE_HEADER_LENGTH_V0 0
#define CREAMHKFILE_HEADER_LENGTH_V1 0
#define CREAMHKFILE_HEADER_LENGTH_V2 0
#define CREAMHKFILE_HEADER_LENGTH_V3 0
#define CREAMHKFILE_HEADER_LENGTH_V4 32
#define CREAMHKFILE_HEADER_LENGTH_V5 32
#define CREAMHKFILE_HEADER_LENGTH_V6 32
#define CREAMHKFILE_HEADER_LENGTH_V8 32

/* Structure for header for version 4 & 5 */
typedef struct {
	char		id[5];	/* "CRMHK" */
	unsigned char	major;
	unsigned char	minor;
	unsigned short	year;	/* 2003, 2004, etc. */
	unsigned char	month;	/* 1, 2, ..., 12. */
	unsigned char	day;	/* 1, 2, ..., 31. */
	unsigned char	hour;	/* 0, 1, ..., 23. */
	unsigned char	min;	/* 0, 1, ..., 59. */
	unsigned char	sec;	/* 0, 1, ..., 59. */
	unsigned short	ref_year;
} cream_hk_header4;
typedef cream_hk_header4 cream_hk_header5;

/* current version */
typedef cream_hk_header5 cream_hk_header;
#define CREAMHKFILE_HEADER_LENGTH CREAMHKFILE_HEADER_LENGTH_V5

class CreamHKFileHeader {
	// directly from/to file
	char _header_cbuf[128];

protected:
	// unpacked header information
	union {
		struct {
			char		id[5];	/* "CRMHK" */
			unsigned char	major;
			unsigned char	minor;
		} any;
			
		cream_hk_header4   v4;
		cream_hk_header5   v5;
	} _h;
	int _run_date;

	int init_header();
	int read_header(FILE* fp);
	int check_header();
	int read_header_4();
	int print_header_4(FILE* fp);
	int read_header_5() { return read_header_4(); }
	int print_header_5(FILE* fp) { return print_header_4(fp); }

	int print_header_6(FILE* fp) { return print_header_4(fp); }
	int read_header_6() { return read_header_4(); }

public:
	CreamHKFileHeader();
	virtual ~CreamHKFileHeader();

	inline int GetRunDate(void) { return _run_date; }
	inline unsigned short GetFormatVersion(void) { return (_h.any.major << 8) + _h.any.minor; }
	int FillHeader(cream_hk_header*);
	inline int FillHeader(void) { return FillHeader(&_h.v5); }
	int PrintHeader(FILE* fp);
	int WriteHeader(FILE* fp, cream_hk_header* h);
	inline int WriteHeader(FILE* fp) { return WriteHeader(fp, &_h.v5); }
	inline cream_hk_header* GetHeader(void) { return &_h.v5; }
};

#endif
