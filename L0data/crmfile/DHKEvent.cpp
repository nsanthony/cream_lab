// DHKEvent.cpp --- implements class DHKEvent.
//
// 2005/6/6 SYZ
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
// $Log: DHKEvent.cpp,v $
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
// Revision 1.8  2005/11/24 02:39:17  syzinn
// get timestamp in type double
//
// Revision 1.7  2005/11/08 19:23:59  cvs
// bug fix in counting the number of lines in PrintText() and PrintHTML()
//
// Revision 1.6  2005/10/28 18:01:39  cvs
// polished HTML output
//
// Revision 1.5  2005/10/28 16:44:18  cvs
// bug fixes in printing in HTML
//
// Revision 1.4  2005/10/28 15:12:41  syzinn
// refined printing of housekeeping data
//
// Revision 1.3  2005/08/04 02:48:41  syzinn
// print text version of the housekeeping data
//
// Revision 1.2  2005/06/07 12:22:52  cvs
// first working version for new hk system
//
// Revision 1.1  2005/06/06 21:31:00  cvs
// new
//
//
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "timez.h"
#include "DHKEvent.h"
#ifdef MEMLEAK
#include "memleak.h"
#endif

DHKEvent::DHKEvent()
	{
	int k;

	for(k = 0; k < DHK_NCH; k++) {
		_data[k].info.enabled = 0;
		_data[k].info.filled = 0;
		_data[k].info.status = 0;
		_data[k].info.type = 0;
		sprintf(_data[k].name, "CH%04d", k);
	}

	// reset counters
	_green_count = _orange_count = _red_count = 0;
}

DHKEvent::~DHKEvent()
	{
	// nothing to do
}

int DHKEvent::Clear(void)
	{
	int k;

	for(k = 0; k < DHK_NCH; k++) {
		_data[k].info.filled = 0;
		_data[k].info.status = 0;
		_data[k].info.type = 0;
	}

	return 0;
}

void DHKEvent::PrintText(FILE* fp, int ch_sel)
	{
	int ncol = 5; // number of columns per row.
	int nrow;
	int i, j, k;
	int ch;
	char cbuf[80];
	unsigned short flist[DHK_NCH]; // list of filled channels.
	int flist_count = 0; // number of entries in the list

	// list up filled channels
	for(ch = 0; ch < DHK_NCH; ch++)
		if ( IsFilled(ch) ) {
			if(ch_sel == -1 || ch == ch_sel)
				flist[flist_count++] = ch;
		}

	if(ch_sel == -1){
	fprintf(fp, "CREAM HOUSEKEEPING DATA\n");
	fprintf(fp, "%3d RED/%3d ORANGE/%3d GREEN\n\n",
		GetNumberOfRedChannel(),
		GetNumberOfOrangeChannel(),
		GetNumberOfGreenChannel());
	}
	nrow = (flist_count + ncol - 1) / ncol;
	for(j = 0; j < nrow; j++) {
		for(i = 0; i < ncol; i++) {
			k = i * nrow + j;
			if ( k < flist_count ) {
				ch = flist[k];

				fprintf(fp, "%8s ", GetName(ch));

				switch( GetType(ch) ) {
				case DHKEvent::typeReal:
					fprintf(fp, "%8.2f ", GetDouble(ch));
					break;
				case DHKEvent::typeInteger:
					fprintf(fp, "%8d ", GetInteger(ch));
					break;
				case DHKEvent::typeLongInteger:
					fprintf(fp, "%8lld ", GetLongInteger(ch));
					break;
				case DHKEvent::typeTimestamp:
					// skip month and day
					sprint_timestamp(cbuf, GetTimestamp(ch));
					fprintf(fp, "%8s ", cbuf + 7);
					break;
				}
			}
		}
		fprintf(fp, "\n");
	}
}

void DHKEvent::PrintHTML(FILE* fp)
	{
	int ncol = 5; // number of columns per row.
	int nrow;
	int i, j, k;
	int ch;
	char cbuf[80];
	unsigned short flist[DHK_NCH]; // list of filled channels.
	int flist_count = 0; // number of entries in the list

	// list up filled channels
	for(ch = 0; ch < DHK_NCH; ch++)
		if ( IsFilled(ch) ) {
			flist[flist_count++] = ch;
		}

	fprintf(fp, "<html> <body>\n");

	fprintf(fp, "<h1> CREAM HOUSEKEEPING DATA </h1>\n");
	fprintf(fp, "<p> %3d RED/%3d ORANGE/%3d GREEN\n",
		GetNumberOfRedChannel(),
		GetNumberOfOrangeChannel(),
		GetNumberOfGreenChannel());

	fprintf(fp, "<font face=Courier> <p> <table border=1 cellpadding=1 cellspacing=1>\n");
 	nrow = (flist_count + ncol - 1) / ncol;
	for(j = 0; j < nrow; j++) {
		fprintf(fp, "<tr>\n");
		for(i = 0; i < ncol; i++) {
			k = i * nrow + j;
			if ( k < flist_count ) {
				ch = flist[k];

				fprintf(fp, "<td> %8s: </td> <td align=right>", GetName(ch));

				switch( GetStatus(ch) ) {
				case DHKEvent::statusGreen: fprintf(fp, "<font color=black>"); break;
				case DHKEvent::statusOrange: fprintf(fp, "<font color=orange>"); break;
				case DHKEvent::statusRed: fprintf(fp, "<font color=red>"); break;
				}

				switch( GetType(ch) ) {
				case DHKEvent::typeReal:
					fprintf(fp, "%8.2f ", GetDouble(ch));
					break;
				case DHKEvent::typeInteger:
					fprintf(fp, "%8d ", GetInteger(ch));
					break;
				case DHKEvent::typeLongInteger:
					fprintf(fp, "%8lld ", GetLongInteger(ch));
					break;
				case DHKEvent::typeTimestamp:
					// skip month and day
					sprint_timestamp(cbuf, GetTimestamp(ch));
					fprintf(fp, "%8s ", cbuf + 7);
					break;
				}
				fprintf(fp, "</font> </td>");
 			}
		}
		fprintf(fp, "</tr>\n");
	}
	fprintf(fp, "</table> </face>\n");
	fprintf(fp, "</body> </html>\n");
}

double DHKEvent::GetEventTimeInSecond(void)
	{
	return (double) epoch_time( GetEventTimestamp() );
}

double DHKEvent::GetNIOTimeInSecond(void)
	{
	return (double) epoch_time( GetNIOTimestamp() );
}

double DHKEvent::GetTRGTimeInSecond(void)
	{
	return (double) epoch_time( GetTRGTimestamp() );
}

double DHKEvent::GetGPSTimeInSecond(void)
	{
	return (double) epoch_time( GetGPSTimestamp() );
}
