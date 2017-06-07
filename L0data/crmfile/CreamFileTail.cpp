//
// CreamFileTail.cpp
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
// $Log: CreamFileTail.cpp,v $
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
#include <stdio.h>
#include <string.h>
#include "CreamFileTail.h"
#include "fio.c"

CreamFileTail::CreamFileTail()
	{
	memset(_tail_cbuf, 0, CREAMFILE_RUN_TAIL_LENGTH);
	memset(&_tail, 0, sizeof(cream_run_tail));
}

CreamFileTail::~CreamFileTail()
	{
}

int CreamFileTail::init_tail(void)
	{
	return FillRunTail( &_tail );
}

int CreamFileTail::read_tail(FILE *fp)
	{
	unsigned char *p;

	if ( fread(_tail_cbuf, 1, CREAMFILE_RUN_TAIL_LENGTH, fp) !=
		CREAMFILE_RUN_TAIL_LENGTH ) return -1;

	p = (unsigned char *) _tail_cbuf;
	_tail.eor_mark = get_uint4( p ); p += 4;
	_tail.year = get_uint2( p ); p += 2;
	_tail.month = *p++;
	_tail.day = *p++;
	_tail.hour = *p++;
	_tail.min = *p++;
	_tail.sec = *p++;
	_tail.status = *p++;

	return 0;
}

int CreamFileTail::print_run_tail(FILE* fp)
	{
	char *p;

	fprintf(fp, "========== TAIL INFORMATION ==========\n");
	fprintf(fp, "eof mark = %08X\n", _tail.eor_mark);
	fprintf(fp, "date and time = %04d-%02d-%02d %02d:%02d:%02d\n",
			_tail.year, _tail.month, _tail.day,
			_tail.hour, _tail.min, _tail.sec);
	switch ( _tail.status ) {
	default: p = "unknown"; break;
	case 0: p = "ok"; break;
	case 1: p = "aborted"; break;
	case 2: p = "continued"; break;
	}
	fprintf(fp, "status = %d [%s]\n", _tail.status, p);

	return 0;
}

int CreamFileTail::FillRunTail(cream_run_tail* p)
	{
	if ( p ) {
		p->eor_mark = 0xffffffff;
		p->year = 0;
		p->month = 0;
		p->day = 0;
		p->hour = 0;
		p->min = 0;
		p->sec = 0;
		p->status = 0xff;
	}
	return 0;
}

int CreamFileTail::WriteRunTail(FILE* fp, cream_run_tail* t)
	{
	char *p;

	memset(_tail_cbuf, 0, CREAMFILE_RUN_TAIL_LENGTH);
	p = _tail_cbuf;

	put_uint4(p, t->eor_mark); p += 4;
	put_uint2(p, t->year); p += 2;
	*p++ = t->month;
	*p++ = t->day;
	*p++ = t->hour;
	*p++ = t->min;
	*p++ = t->sec;
	*p++ = t->status;

	if ( fwrite(_tail_cbuf, 1, CREAMFILE_RUN_TAIL_LENGTH, fp) !=
		CREAMFILE_RUN_TAIL_LENGTH ) {
		fprintf(stderr, "write_run_tail: write failure; disk problem?");
		return -1;
	}

	return 0;
}
