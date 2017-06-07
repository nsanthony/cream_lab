#ifndef CREAMFILETAIL_H
#define CREAMFILETAIL_H
//
// CreamFileTail.h
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
// $Log: CreamFileTail.h,v $
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
// Revision 1.1.1.1  2005/02/04 05:02:17  daq
// initial version for CREAM2.
//
// Revision 1.3  2004/04/15 20:59:02  syzinn
// added copyright notice.
//
// Revision 1.2  2004/02/27 19:21:56  syzinn
// Read/write end of run information for format 2.0
//
// Revision 1.1.1.1  2004/02/24 21:46:25  syzinn
// crmfile library
//
//

typedef struct {
	unsigned	eor_mark; // end-of-run mark
	unsigned short	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	status;
} cream_run_tail;

#define CREAMFILE_RUN_TAIL_LENGTH 12
class CreamFileTail {
	// directly from/to file
	char _tail_cbuf[CREAMFILE_RUN_TAIL_LENGTH];

protected:
	cream_run_tail _tail;

	int init_tail();
	int read_tail(FILE* fp);
	int print_run_tail(FILE* fp);

public:
	CreamFileTail();
	virtual ~CreamFileTail();

	cream_run_tail* GetRunTail() { return &_tail; }
	int FillRunTail(cream_run_tail *);
	inline int FillRunTail() { return FillRunTail( &_tail ); }

	int WriteRunTail(FILE*, cream_run_tail *);
	inline int WriteRunTail(FILE* fp) { return WriteRunTail(fp, &_tail); }

	inline int PrintRunTail(FILE *fp) { return print_run_tail(fp); }
};

#endif
