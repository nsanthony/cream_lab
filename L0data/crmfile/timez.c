/*
 * timez.c --- time funtions
 *
 * 2003/9/17 SYZ
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
 * $Log: timez.c,v $
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
 * Revision 1.1.1.1  2009/02/02 03:01:12  creamop
 * hahaha
 *
 * Revision 5.0  2009/01/24 04:33:23  creamop
 * S.L 5.2 Version
 *
 * Revision 1.1.1.1  2009/01/24 04:30:18  creamop
 * hahaha
 *
 * Revision 5.0  2009/01/24 03:48:22  creamop
 * S.L 5.2 Version.
 *
 * Revision 1.1.1.1  2009/01/24 03:44:32  creamop
 * hahaha
 *
 * Revision 4.0  2009/01/20 08:13:48  creamop
 * CREAM IV flight version.
 *
 * Revision 1.1.1.1  2009/01/20 08:12:08  creamop
 * hahaha
 *
 * Revision 1.4  2006/03/24 14:42:33  syzinn
 * Merged a branch post_flight2_changes
 *
 * Revision 1.3.2.1  2006/03/14 21:07:37  syzinn
 * eight-byte timestamp
 *
 * Revision 1.3  2006/01/14 02:58:57  syzinn
 * commented out unused include
 *
 * Revision 1.2  2005/10/28 14:42:05  syzinn
 * new format for date and time printing
 *
 * Revision 1.1.1.1  2005/02/04 05:02:17  daq
 * initial version for CREAM2.
 *
 * Revision 1.12  2004/10/15 20:52:14  syzinn
 * usec-resolution timestamping.
 *
 * Revision 1.11  2004/08/27 19:37:44  syzinn
 * added epoch_time() and epoch_time2() to get from time in secods since 2003/1/1.
 *
 * Revision 1.10  2004/05/28 15:38:12  syzinn
 * bug fix
 *
 * Revision 1.9  2004/05/20 18:23:38  syzinn
 * cputime() added.
 *
 * Revision 1.8  2004/04/15 21:04:45  syzinn
 * added copyright notice.
 *
 * Revision 1.7  2004/02/21 15:31:36  syzinn
 * added memory leak detection
 *
 * Revision 1.6  2004/02/09 16:34:42  syzinn
 * Merged unstable_flight_x.
 *
 * Revision 1.5.2.2  2004/02/03 20:36:31  syzinn
 * Bug fix.
 *
 * Revision 1.5.2.1  2004/01/23 17:34:07  syzinn
 * New data structure  timestamp_t  for time stamp; replaces older format of 8-byte chars.
 *
 * Revision 1.5  2003/12/26 19:17:34  syzinn
 * Removed checking new timestamp.
 *
 * Revision 1.4  2003/12/26 19:06:06  syzinn
 * Support for new timestamp of 8-byte string type.
 *
 * Revision 1.3  2003/10/28 03:25:27  syzinn
 * New functions that prints date and time to a string buffer.
 *
 * Revision 1.2  2003/10/25 21:09:53  syzinn
 * Placed a tab.
 *
 * Revision 1.1.1.1  2003/10/06 21:15:58  syzinn
 * Alpha version.
 *
 */
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

/* #include "fsio.h" */
#include "timez.h"
#ifdef MEMLEAK
#include "memleak.h"
#endif

#define TS_YEAR0	2000
typedef struct {
	unsigned unused : 2;
	unsigned nsec	:10; /* 0, 1, ..., 999 */
	unsigned usec	:10; /* 0, 1, ..., 999 */
	unsigned msec	:10; /* 0, 1, ..., 999 */
	unsigned sec	: 6; /* 0, 1, ..., 59 */
	unsigned min	: 6; /* 0, 1, ..., 59 */
	unsigned hour	: 5; /* 0, 1, ..., 23 */
	unsigned day	: 5; /* 1, 2, ..., 31 */
	unsigned month	: 4; /* 1, 2, ..., 12 */
	unsigned year	: 6; /* year - TS_YEAR0; up to TS_YEAR0 + 63 */
} timestamp8_t;

/*
 * _t0 is set by init_wallclock() to compute wall clock time.
 */
static struct timeval _t0;

static char* mon_str = "JAN\0FEB\0MAR\0APR\0MAY\0JUN\0JUL\0AUG\0SEP\0OCT\0NOV\0DEC\0UNK";

void fprint_date_time(FILE* fp, time_t t)
	{
	struct tm date_time;

	/* convert */
	localtime_r(&t, &date_time);

	/* print date and time */
	fprintf(fp, "%s %02d %02d:%02d:%02d",
		mon_str + 4 * date_time.tm_mon,
		date_time.tm_mday,
		date_time.tm_hour,
		date_time.tm_min,
		date_time.tm_sec
	);
}

void sprint_date_time(char* s, time_t t)
	{
	struct tm date_time;

	/* convert */
	localtime_r(&t, &date_time);

	/* print date and time */
	sprintf(s, "%s %02d %02d:%02d:%02d",
		mon_str + 4 * date_time.tm_mon,
		date_time.tm_mday,
		date_time.tm_hour,
		date_time.tm_min,
		date_time.tm_sec
	);
}

void fprint_cur_date_time(FILE* fp)
	{
	time_t t;

	/* get time */
	t = time( 0 );

	/* print */
	fprint_date_time(fp, t);
}

void sprint_cur_date_time(char* s)
	{
	time_t t;

	/* get time */
	t = time( 0 );

	/* print */
	sprint_date_time(s, t);
}

void init_wallclock()
	{
	struct timezone z;
	gettimeofday(&_t0, &z);
}

double wallclock()
	{
	struct timeval t;
	struct timezone z;
	double dt;

	gettimeofday(&t, &z);

	dt = (t.tv_sec - _t0.tv_sec) + 0.000001 * (t.tv_usec - _t0.tv_usec);
	return dt;
}

double cputime()
{
	static int firstQ = 1;
	static double inv_clock_tick;
	static struct tms buf;
	static clock_t sum;

	if ( firstQ ) {
		long clock_tick = sysconf( _SC_CLK_TCK );
		inv_clock_tick = (double) 1. / clock_tick;
		firstQ = 0;
	}

	times( &buf );

	sum = buf.tms_utime + buf.tms_stime + buf.tms_cutime + buf.tms_cstime;
	return (double) sum * inv_clock_tick;
	//return (double) sum;
}

/* return value: -1 upon error, otherwise the byte length of time stamp */
int timestamp(void* s)
	{
	static struct timeval tv;
	static struct timezone tz;
	static struct tm date_time;
	static timestamp8_t* p;
	unsigned usec;

	if ( s == 0 )
		return -1;
       
	gettimeofday(&tv, &tz);
	localtime_r(&(tv.tv_sec), &date_time);
	usec = tv.tv_usec;

	p = (timestamp8_t*) s;
	p->year = date_time.tm_year + 1900 - TS_YEAR0;
	p->month = date_time.tm_mon + 1;
	p->day = date_time.tm_mday;
	p->hour = date_time.tm_hour;
	p->min = date_time.tm_min;
	p->sec = date_time.tm_sec;
	p->msec = usec / 1000;
	p->usec = usec - p->msec * 1000;
	p->nsec = 0;
	p->unused = 0;

	return 8;
}

time_t epoch_time(void* p0)
	{
	struct tm date_time;
	time_t t;
	timestamp8_t* p;

	if ( p0 == 0 )
		return 0;

	p = (timestamp8_t *) p0;
	date_time.tm_year = p->year + TS_YEAR0 - 1900;
	date_time.tm_mon = p->month - 1;
	date_time.tm_mday = p->day;
	date_time.tm_hour = p->hour;
	date_time.tm_min = p->min;
	date_time.tm_sec = p->sec;
	date_time.tm_isdst = 0; // no daylight saving time.

	// Our reference is 2003 JAN 1 00:00:00
	// which is 1041397200.
	t = mktime( &date_time ) - 1041397200;
	return t;
}

time_t epoch_time2(unsigned* p)
	{
	struct tm date_time;
	time_t t;

	if ( p == 0 )
		return 0;

	date_time.tm_year = p[0] - 1900;
	date_time.tm_mon = p[1] - 1;
	date_time.tm_mday = p[2];
	date_time.tm_hour = p[3];
	date_time.tm_min = p[4];
	date_time.tm_sec = p[5];
	date_time.tm_isdst = 0; // no daylight saving time.

	// Our reference is 2003 JAN 1 00:00:00
	// which is 1041397200.
	t = mktime( &date_time ) - 1041397200;
	return t;
}

void filename_timestamp(char* s, void* t)
	{
	int year, month, day;
	int hour, min, sec;
	timestamp8_t* p;

	p = (timestamp8_t*) t;
	year = p->year + TS_YEAR0;
	month = p->month;
	day = p->day;
	hour = p->hour;
	min = p->min;
	sec = p->sec;

	sprintf(s, "%04d%02d%02d-%02d%02d%02d",
		year, month, day,
		hour, min, sec);
}

void fprint_timestamp(FILE* fp, void* t)
	{
	int year, month, day;
	timestamp8_t* p;

	p = (timestamp8_t*) t;
	year = p->year + TS_YEAR0;
	month = p->month;
	day = p->day;

	/* print date and time */
	month--;
	if ( month < 0 || month >= 12 ) month = 12; // UNK
	fprintf(fp, "%s %02d %02d:%02d:%02d.%03d.%03d",
		mon_str + 4 * month, day,
		p->hour, p->min, p->sec, p->msec, p->usec);
}

void sprint_timestamp(char* s, void* t)
	{
	int year, month, day;
	timestamp8_t* p;

	p = (timestamp8_t*) t;
	year = p->year + TS_YEAR0;
	month = p->month;
	day = p->day;

	/* print date and time */
	month--;
	if ( month < 0 || month >= 12 ) month = 12; // UNK
	sprintf(s, "%s %02d %02d:%02d:%02d.%03d.%03d",
		mon_str + 4 * month, day,
		p->hour, p->min, p->sec, p->msec, p->usec);
}

unsigned long ComputeNumSecs(int year, int month, int day, int hour, int min , int sec)
{
	int yearDiff = year - 2014; // number of years in between 2014 and timestamp year
	int monthDays; // number of days up to the timestamp month
	int yearDays = yearDiff*365; // number of days up to the timestamp year
	int leapYearEst=0; // number of leap years in between 2014 and the timestamp year
	int timeSec = hour*3600 + min*60 + sec; // number of seconds since midnight of the timestamp day
	
	//compute number of leap years
	if(yearDiff -2 >= 0)
	{
		leapYearEst = (yearDiff -2) /4  +1;

		if( (year % 4) == 0 &&  month <= 2)
			leapYearEst -= 1;
	}
	
	
	// computer monthDays
	switch (month){

	case 1:
		monthDays = day -1;
		break;
	case 2:
		monthDays = 31 + day -1;
		break;
	case 3: 
		monthDays = 31+28 + day -1;
		break;
	case 4:
		monthDays = 31+28+31+ day -1;
		break;
	case 5:
		monthDays = 31+28+31+30 + day -1;
		break;
	case 6:
		monthDays = 31+28+31+30+31 + day -1;
		break;
	case 7:
		monthDays = 31+28+31+30+31+30 + day -1;
		break;
	case 8:
		monthDays = 31+28+31+30+31+30+31 + day -1;
		break;
	case 9:
		monthDays = 31+28+31+30+31+30+31+31 + day -1;
		break;
	case 10:
		monthDays = 31+28+31+30+31+30+31+31+30 + day -1;
		break;
	case 11:
		monthDays = 31+28+31+30+31+30+31+31+30+31 + day -1;
		break;
	case 12:
		monthDays = 31+28+31+30+31+30+31+31+30+31+30 + day -1;
		break;
	default:
		return -1;
		break;
	}
	
	//add in extra days for all leap years and return the number of days
	monthDays += leapYearEst;
	return ( (yearDays+monthDays)*86400 + timeSec);
}
