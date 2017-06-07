/*
 * crmfile.cpp --- a set of routines for reading CREAM data file.
 *
 * 2003/10/8 SYZ
 *
 * Copyright (C) 2003-2004 by the CREAM project.  All rights reserved.
 * 
 * This file is part of CDAQ, the CREAM data acquisition and processing system.
 * 
 * This work may not be reproduced, displayed, modified or distributed
 * without the express prior written permission of the copyright holder.
 * For permission, contact Prof. Eun-suk Seo at seo@umd.edu.  The above
 * copyright notice must appear in all copies and in supporting documentation.
 * 
 * The software is provided without any warranty, express or implied, including
 * but not limited to the warranties of merchantability, fitness for a
 * particular purpose and noninfringement.  In no event shall the author or
 * the CREAM project group be liable for any claim, damages or other liability,
 * whether in an action of contract, tort or otherwise, arising from, out of
 * or in connection with the software or the use or other dealings in the
 * software.
 *
 * $Log: crmfile.cpp,v $
 * Revision 1.2  2012/03/21 20:41:34  dja1987
 * T/BCD updates.
 *
 * Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
 * Importing SCDL bug fixes.
 *
 * Revision 1.1.1.1  2011/06/07 19:10:21  dja1987
 * Includes both TRD and SCDL updates
 *
 * Revision 1.1.1.1  2010/06/11 15:14:31  spaced4
 * CREAM6
 *
 * Revision 5.1  2009/03/02 04:42:01  creamop
 * Pcm Encoder and TDRSS w/o SEI
 *
 * Revision 5.0  2009/02/02 03:05:20  creamop
 * S.L 5.2 and FC3 both.
 *
 * Revision 1.1.1.1  2009/02/02 03:01:13  creamop
 * hahaha
 *
 * Revision 5.0  2009/01/24 04:33:24  creamop
 * S.L 5.2 Version
 *
 * Revision 1.1.1.1  2009/01/24 04:30:19  creamop
 * hahaha
 *
 * Revision 5.0  2009/01/24 03:48:22  creamop
 * S.L 5.2 Version.
 *
 * Revision 1.1.1.1  2009/01/24 03:44:34  creamop
 * hahaha
 *
 * Revision 4.0  2009/01/20 08:13:49  creamop
 * CREAM IV flight version.
 *
 * Revision 1.1.1.1  2009/01/20 08:12:09  creamop
 * hahaha
 *
 * Revision 1.1.1.1  2005/02/04 05:02:17  daq
 * initial version for CREAM2.
 *
 * Revision 1.2  2004/04/15 20:59:03  syzinn
 * added copyright notice.
 *
 * Revision 1.1.1.1  2004/02/24 21:46:25  syzinn
 * crmfile library
 *
 */
#include "CreamFile.h"
#ifdef MEMLEAK
#include "memleak.h"
#endif

/* hidden variables */
static CreamFile _cf;

int cream_open(char* filename)
	{
	return _cf.OpenRead(filename);
}

int cream_openw(char* filename)
	{
	return _cf.OpenWrite(filename);
}

cream_run_header* cream_get_run_header(void)
	{
	return _cf.GetRunHeader();
}

int cream_fill_run_header(cream_run_header* p)
	{
	return _cf.FillRunHeader( p );
}

int cream_print_run_header(FILE* fp)
	{
	return _cf.PrintRunHeader(fp);
}

int cream_write_run_header(FILE* fp, cream_run_header* h)
	{
	return _cf.WriteRunHeader(fp, h);
}

int cream_close(void)
	{
	return _cf.Close();
}

/* read an event */
int cream_read_event(int* plen, unsigned* ptrig, int* pcamac,
		unsigned short* pcal, unsigned short* pscd, 
		unsigned short* phds, unsigned short* tcrd)
	{
	return _cf.ReadEvent(plen, ptrig, pcamac, pcal, pscd, phds, ptcrd); 
} //YONAS


/* return event numbers */
int cream_get_event_number(unsigned evnum[5])
	{
	return _cf.GetEventNumber(evnum);
}

/* return version number */
unsigned short cream_get_format_version(void)
	{
	return _cf.GetFormatVersion();
}

/* return time stamp */
int cream_get_time_stamp(unsigned ts[6])
	{
	return _cf.GetTimeStamp(ts);
}

/* return TCD data */
int cream_get_tcd(int* plen, unsigned short *pdata)
	{
	return _cf.GetTCDData(plen, pdata);
}

/* return TRD data */
int cream_get_trd(int* plen, unsigned short *pdata)
	{
	return _cf.GetTRDData(plen, pdata);
}
