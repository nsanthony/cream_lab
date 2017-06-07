#ifndef CRMFILE_H
#define CRMFILE_H
/*
 * crmfile.h --- a set of functions to read CREAM data files.
 * 		 This reads earlier format files as well.
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
 * $Log: crmfile.h,v $
 * Revision 1.2  2012/03/21 20:41:34  dja1987
 * T/BCD updates.
 *
 * Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
 * Importing SCDL bug fixes.
 *
 * Revision 1.1.1.1  2011/06/07 19:10:20  dja1987
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
 * Revision 1.1.1.1  2009/01/24 04:30:20  creamop
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
 * Revision 1.1.1.1  2009/01/20 08:12:10  creamop
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

#ifdef __cplusplus
extern "C" {
#endif

/* open CREAM file for reading.
   return 0 if successful, or -1 in case of errors */
int cream_open(char* filename);

/* open CREAM file for writing.
   return 0 if successful, or -1 in case of errors */
int cream_openw(char* filename);

/* get pointer to the run-header information */
cream_run_header* cream_get_run_header(void);

/* fill the run header information */
int cream_fill_run_header(cream_run_header* p);

/* get format version */
unsigned short cream_get_format_version();

/* print header info */
int cream_print_run_header(FILE* fp);

/* write header info */
int cream_write_run_header(FILE* fp, cream_run_header* h);

/* close file.  return 0 always. */
int cream_close(void);

/*
 * Read one event.
 *
 * return -1 upon error or end-of-file condition.  return 0 otherwise.
 *
 * Input:
 *	length[0]	length of trigger array (should be 1).
 * 	length[1]	length of camac array.
 * 	length[2]	length of cal array.
 * 	length[3]	length of scd array.
 * 	length[4]	length of hds array.
 *
 * Output:
 * 	length[0]	length of trigger array from data
 * 	length[1]	length of camac array from data
 * 	length[2]	length of cal array from data
 * 	length[3]	length of scd array from data
 * 	length[4]	length of hds array from data
 *
 * 	Zero length indicates non-existence of corresponding data.
 * 	For example, if  length[1]  is zero, no CAMAC data exists.
 *
 * 	*trig		Trigger value
 *	camac[k]	CAMAC ADC value at channel  k = 0, 1, etc.
 * 	cal[k]		Calorimeter ADC value at channel  k = 0, 1, etc.
 * 	scd[k]		SCD ADC value at channel  k = 0, 1, etc.
 * 	hds[k]		Hodoscope ADC value at channel  k = 0, 1, etc.
 */
int cream_read_event(
	int* length,	/* length of the arrays (INPUT/OUTPUT) */
	unsigned* trig,	/* trigger value (OUTPUT) */
	int* camac,	/* CAMAC values (OUTPUT) */
	unsigned short* cal,	/* Calorimeter ADC values (OUTPUT) */
	unsigned short* tcrd,	/* TCRD ADC values (OUTPUT) */ //YONAS
	unsigned short* scd,	/* SCD ADC values (OUTPUT) */
	unsigned short* hds	/* Hodoscope ADC values (OUTPUT) */
	);


/*
 * Get hardware event numbers from data last read.
 *
 * Output:
 *	evnum[0] contains event number from trigger
 *	evnum[1] contains event number from CAMAC
 *	evnum[2] contains event number from Calorimeter
 *	evnum[3] contains event number from SCD
 *	evnum[4] contains event number from Hodoscope
 *	evnum[5] contains event number from TCD
 *	evnum[6] contains event number from TRD
 */
int cream_get_event_number(unsigned evnum[7]);

/*
 * Get timestamp of event.
 *
 * Output:
 * 	timestamp[0] contains year
 * 	timestamp[1] contains month [1-12]
 * 	timestamp[2] contains day [1-31]
 * 	timestamp[3] contains hour [0-23]
 * 	timestamp[4] contains minute [0-59]
 * 	timestamp[5] contains second [0-59]
 */
int cream_get_time_stamp(unsigned timestamp[6]);

#ifdef __cplusplus
};
#endif

#endif
