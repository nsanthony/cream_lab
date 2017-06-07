// DHKEventConverter.cpp --- implements class DHKEventConverter.
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
// $Log: DHKEventConverter.cpp,v $
// Revision 1.2  2012/11/16 21:11:55  dja1987
// Fixes to TCD/BCD crmfile. Updated HK format. Improved client playback (no more temporary arrays for missing packet numbers). Updated cprint
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
// Revision 1.2  2005/06/07 12:22:53  cvs
// first working version for new hk system
//
// Revision 1.1  2005/06/06 21:31:22  cvs
// new
//
//
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include "defs.h"
#include "timez.h"
#include "DHKEventConverter.h"
#ifdef MEMLEAK
#include "memleak.h"
#endif

DHKEventConverter::DHKEventConverter()
	{
	int k;

	for(k = 0; k < DHK_NCH; k++) {
		_ctbl[k].offset = 0.0;
		// A channel is considered disabled when factor is zero.
		// For such channel, status counts (_red_count etc.) will not be updated.
		_ctbl[k].factor = 0.0;
	}
}

DHKEventConverter::~DHKEventConverter()
	{
	// nothing to do
}

int DHKEventConverter::load(char* path, char* fname)
	{
	char* fullpath;
	int len;
	int iret;

	len = strlen(path) + strlen(fname) + 10;
	fullpath = new char[len];
	if ( fullpath ) {
		sprintf(fullpath, "%s/%s", path, fname);
		iret = load( fullpath );
	}
	else
		iret = -1;

	delete [] fullpath;
	return iret;
}

int DHKEventConverter::load(char* fname)
	{
	FILE* fp;
	int iret;

	if ( fname == 0 || *fname == 0 ) {
		fprintf(stderr, "null file name.\n");
		return -1;
	}

	fp = fopen(fname, "r");
	if ( fp ) {
		iret = load(fp);
		fclose(fp);
	}
	else {
		fprintf(stderr, "cannot open file %s.\n", fname);
		iret = -1;
	}

	return iret;
}

// lineno starts from 0.
char* get_line(char* cbuf, int len, FILE* fp, int lineno)
	{
	char *p = cbuf;
	int k;
	int c;

	// len - 1 needed to store '\0'.
	for(k = 0; k < len - 1; k++) {
		c = getc(fp);
		if ( c == '\n' || c == EOF ) {
			*p = 0;
			return (k == 0) ? 0 : cbuf;
		}
		*p++ = (char) c;
	}

	fprintf(stderr, "line %d: too long; truncated.\n", lineno + 1);
	*p = 0;

	do c = getc(fp); while ( c != '\n' && c != EOF );
	return cbuf;
}

int DHKEventConverter::load(FILE* fp)
	{
	int ch;
	char name[256];
	double offset;
	double factor;
	double therm_quad=0;
	double min0, max0, min1, max1;
	char cbuf[256];
	int line;
	int nerr = 0;
	bool processing_therm = false;

	line = 0;
	while ( get_line(cbuf, 255, fp, line) != 0 ) {
		line++;

		// skip comment line
		if ( cbuf[0] == '#' && processing_therm) { processing_therm = false; continue;}
		else if ( cbuf[0] == '#' ) continue;
		else if(cbuf[0] == '!') { processing_therm = true; continue;}

		if(processing_therm && sscanf(cbuf, "%d%s%lf%lf%lf%lf%lf%lf%lf", &ch, name, &therm_quad, &factor, &offset,
		     &min0, &max0, &min1, &max1) != 9 ) {
			fprintf(stderr, "line %d: invalid format.\n", line);
			nerr++;
			processing_therm = false;
			continue;
		}

		if ( !processing_therm && sscanf(cbuf, "%d%s%lf%lf%lf%lf%lf%lf", &ch, name, &factor, &offset,
		     &min0, &max0, &min1, &max1) != 8 ) {
			fprintf(stderr, "line %d: invalid format.\n", line);
			nerr++;
			continue;
		}

		if ( ch >= 0 && ch < DHK_NCH ) {
			SetEnabled( ch );
			SetName( ch, name );
			_ctbl[ch].factor = factor;
			_ctbl[ch].offset = offset;
			_ctbl[ch].quad_therm = therm_quad;
			_ctbl[ch].normal_min = min0;
			_ctbl[ch].normal_max = max0;
			_ctbl[ch].critical_min = min1;
			_ctbl[ch].critical_max = max1;
		}
		else {
			fprintf(stderr, "line %d: invalid channel %d.\n", line, ch);
			nerr++;
		}
	}

	return (nerr > 0) ? -1 : 0;
}

// convert 12-bit signed integer to 32-bit integer with the same value.
int DHKEventConverter::conv_12bit(int a)
	{
	int b;

	if ( a & 0x0800 ) {
		a = ~a & 0xFFF;
		b = -1 - a;
	}
	else {
		b = a;
	}

	return b;
}

// convert 16-bit signed integer to 32-bit integer with the same value.---ISS-CREAM
int DHKEventConverter::conv_16bit(int a)
	{
	int b;

	if ( a & 0x8000 ) {
		a = ~a & 0xFFFF;
		b = -1 - a;
	}
	else {
		b = a;
	}

	return b;
}

int DHKEventConverter::CheckStatus()
	{
	int k;

	_green_count = _orange_count = _red_count = 0;

	for(k = 0; k < DHK_NCH; k++) {
		if ( IsFilled(k) && IsEnabled(k) ) {
			switch( GetType(k) ) {
			case typeReal:
				if ( GetDouble(k) >= _ctbl[k].normal_min &&
				     GetDouble(k) <= _ctbl[k].normal_max ) {
					// normal range (green range)
					SetStatus(k, statusGreen);
					_green_count++;
				}
				else if ( GetDouble(k) >= _ctbl[k].critical_min &&
					  GetDouble(k) <= _ctbl[k].critical_max ) {
					// critical range (orange range)
					SetStatus(k, statusOrange);
					_orange_count++;
				}
				else {
					// red range
					SetStatus(k, statusRed);
					_red_count++;
				}
				break;
			case typeInteger:
				if ( GetInteger(k) >= _ctbl[k].normal_min &&
				     GetInteger(k) <= _ctbl[k].normal_max ) {
					// normal range (green range)
					SetStatus(k, statusGreen);
					_green_count++;
				}
				else if ( GetInteger(k) >= _ctbl[k].critical_min &&
					  GetInteger(k) <= _ctbl[k].critical_max ) {
					// critical range (orange range)
					SetStatus(k, statusOrange);
					_orange_count++;
				}
				else {
					// red range
					SetStatus(k, statusRed);
					_red_count++;
				}
				break;
			case typeLongInteger:
				if ( GetLongInteger(k) >= _ctbl[k].normal_min &&
				     GetLongInteger(k) <= _ctbl[k].normal_max ) {
					// normal range (green range)
					SetStatus(k, statusGreen);
					_green_count++;
				}
				else if ( GetLongInteger(k) >= _ctbl[k].critical_min &&
					  GetLongInteger(k) <= _ctbl[k].critical_max ) {
					// critical range (orange range)
					SetStatus(k, statusOrange);
					_orange_count++;
				}
				else {
					// red range
					SetStatus(k, statusRed);
					_red_count++;
				}
				break;
			case typeTimestamp:
				// timestamp for trigger rate
				{
					time_t event_timestamp = epoch_time( GetEventTimestamp() );
					time_t this_timestamp = epoch_time( GetTimestamp(k) );
					if ( event_timestamp > this_timestamp + 20 ) {
						SetStatus(k, statusRed);
						_red_count++;
					}
					else if ( event_timestamp > this_timestamp + 10 ) {
						SetStatus(k, statusOrange);
						_orange_count++;
					}
					else {
						SetStatus(k, statusGreen);
						_green_count++;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int DHKEventConverter::Convert()
	{
	int k;
	float resistance;

	for(k = 0; k < DHK_NCH; k++) {
		if ( IsEnabled(k) && _ctbl[k].flag.set ) {
			
			if(k >= 196 && k <= 213){ // ISS-Thermistors are different 196-213 are channel numbers in hkconviss_v3.dat!
						  // Must update these if hk-channels change.
				resistance = 10000.0f *(_ctbl[k].raw / (3.3f - _ctbl[k].raw));
			
				SetDouble(k, pow( pow(log(resistance),3) * _ctbl[k].quad_therm +
					log(resistance) * _ctbl[k].factor + _ctbl[k].offset, -1) -273.15);
			}
			else if( k >= 72 && k <= 79) // correcting for ground voltage for SFC-related items. Update date channel
						      //numbers if hkconv changes
				SetDouble(k, (_ctbl[k].raw - _ctbl[80].raw) * _ctbl[k].factor + _ctbl[k].offset);

			else if( k >= 214 && k <= 260)
				SetDouble(k, (_ctbl[k].raw - _ctbl[261].raw) * _ctbl[k].factor + _ctbl[k].offset);

			else if( k >= 262 && k <= 308)
				SetDouble(k, (_ctbl[k].raw - _ctbl[309].raw) * _ctbl[k].factor + _ctbl[k].offset);

			else if( k >= 310 && k <= 356)
				SetDouble(k, (_ctbl[k].raw - _ctbl[357].raw) * _ctbl[k].factor + _ctbl[k].offset);

			else
				SetDouble(k, _ctbl[k].raw * _ctbl[k].factor + _ctbl[k].offset);
		}
	}
	return 0;
}

int DHKEventConverter::Clear()
	{
	_cbuf_count = 0;

	for(int k = 0; k < DHK_NCH; k++)
		_ctbl[k].flag.set = 0;

	return DHKEvent::Clear();
}
