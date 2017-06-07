#ifndef DEFS_H
#define DEFS_H
/*
 * defs.h
 *
 * 2005/2/26 SYZ
 *
 * Copyright (C) 2003-2005 by the CREAM project.  All rights reserved.
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
 * $Log: defs.h,v $
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
 * Revision 1.1.1.1  2009/01/24 03:44:33  creamop
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
 * Revision 1.3  2005/11/24 02:30:35  syzinn
 * workaround for CINT, the ROOT C/C++ interpreter
 *
 * Revision 1.2  2005/02/26 17:22:19  daq
 * added disclaimer
 *
 */
#define GCC_VERSION   (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ )
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
 * TYPES OF FIXED BYTE SIZE
 **********************************************************************/
typedef unsigned char uint1;
typedef unsigned short uint2;
typedef unsigned uint4;
typedef unsigned long long uint8;
typedef signed char int1;
typedef short int2;
typedef int int4;
typedef float real4;
typedef double real8;

/* Eight-byte timestamp */
#define TS_LEN		8
#define TS_YEAR0	2000

#if __CINT__
typedef uint8 timestamp8_t;
#else
// G++ compiler builds bit-field structure from the least significant bits.
// So we place  sec  first which occupies bits 0 to 5.
#if defined(__GNUC__) && ( ( __GNUC__ == 3 ) && ( __GNUC_MINOR__ >= 2 ) )
typedef struct _timestamp8_t {
#else
typedef struct {
#endif
	// bit 0 is the least significant.
	//	bits  0 -  1: reserved for future
	//	bits  2 - 11: nanosecond (maybe later)
	//	bits 12 - 21: microsecond 
	//	bits 22 - 31: millisecond 
	//	bits 32 - 37: second 
	//	bits 38 - 43: minute 
	//	bits 44 - 48: hour 
	//	bits 49 - 53: day 
	//	bits 54 - 57: month 
	//	bits 58 - 63: year 

	unsigned reserved	:  2;
	unsigned ns		: 10; /* 0, 1, ..., 999 */
	unsigned us		: 10; /* 0, 1, ..., 999 */
	unsigned ms		: 10; /* 0, 1, ..., 999 */
	unsigned sec		:  6; /* 0, 1, ..., 59 */
	unsigned min		:  6; /* 0, 1, ..., 59 */
	unsigned hour		:  5; /* 0, 1, ..., 23 */
	unsigned day		:  5; /* 1, 2, ..., 31 */
	unsigned month		:  4; /* 1, 2, ..., 12 */
	unsigned year		:  6; /* year - TS_YEAR0; up to TS_YEAR0 + 64 - 1 */
} timestamp8_t;
#endif

/* Four-byte timestamp for keeping backward compatibility */
#if __CINT__
typedef uint4 timestamp4_t;
#else
// G++ compiler builds bit-field structure from the least significant bits.
// So we place  sec  first which occupies bits 0 to 5.
#if defined(__GNUC__) &&  ( ( __GNUC__ == 3 ) && ( __GNUC_MINOR__ >= 2 ) )
typedef struct _timestamp4_t {
#else
typedef struct {
#endif
	// bit 0 is the least significant.
	//	bits  0 -  5: second 
	//	bits  6 - 11: minute 
	//	bits 12 - 16: hour 
	//	bits 17 - 21: day 
	//	bits 22 - 25: month 
	//	bits 26 - 31: year 

	unsigned sec		:  6; /* 0, 1, ..., 59 */
	unsigned min		:  6; /* 0, 1, ..., 59 */
	unsigned hour		:  5; /* 0, 1, ..., 23 */
	unsigned day		:  5; /* 1, 2, ..., 31 */
	unsigned month		:  4; /* 1, 2, ..., 12 */
	unsigned year		:  6; /* year - TS_YEAR0; up to TS_YEAR0 + 64 - 1 */
} timestamp4_t;
#endif

#define GPS_DATSIZ 68
/********************************
typedef struct {
	uint2		pos_hr;
	uint2		pos_min;
	real4		pos_sec;
	real8		latitude;
	real8		longitude;
	real4		altitude;
	real4		vvelocity;
	real4		heading;
	real4		pitch;
	real4		roll;
	real4		course;
	real4		speed;
	real4		pdop;
	real4		phase_rms;
	real4		baseline_rms;
	uint1		nsatellite;
	uint1		reset_flag;
	uint1		padding[2];
} gps_t;
*******************************/
typedef struct {
	uint2		pos_hr;
	uint2		pos_min;
	real4		pos_sec;
	real8		latitude;
	real8		longitude;
	real4		altitude;
	real4		vvelocity;
	real4		heading;
	real4		pitch;
	real4		roll;
	real4		course;
	real4		speed;
	real4		pdop;
	real4		phase_rms;
	real4		baseline_rms;
	uint1		nsatellite;
	uint1		reset_flag;
	uint1		padding[2];
} gps_t;

typedef struct {
	uint4		timeOff;
	uint4		weekNo;
	uint4		weekTime;

	real8		latitude;
	real8		longitude;
	real4		altitude;
	real4		vvelocity;
	real4		heading;
	real4		pitch;
	real4		roll;
//	real4		course;
	real4		speed;

	real4		mksHi; //10
	real4		mksMid; //100
	real4		mksLo; //1000

	uint1		nsatellite;
	uint1		reset_flag;
	uint1		padding[2];
} gps_sip_t;
#ifdef __cplusplus
};
#endif

/* standard macros */
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef safe_delete
#define safe_delete(p) if ( p ) { delete p; p = 0; }
#endif

#endif
