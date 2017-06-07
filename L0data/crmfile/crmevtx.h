#ifndef CRMEVENTX_H
#define CRMEVENTX_H
/*
 * crmevtx.h --- extension to crmevt.h
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
 * $Log: crmevtx.h,v $
 * Revision 1.4  2012/07/19 14:44:47  dja1987
 * Changing all T/BCD channel numbers to 896 and adding new cissnio and sissnio cpp files without ack system
 *
 * Revision 1.3  2012/05/10 13:34:22  dja1987
 * Changed scdl channel number from 3136 to 2944 in all crmevtx.h files. Fixed typos in SEventbuilder.cpp TCRD code that caused compiling error. Removed accidentally placed log message in HTriggerUSB.cpp
 *
 * Revision 1.2  2012/03/21 20:41:34  dja1987
 * T/BCD updates.
 *
 * Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
 * Importing SCDL bug fixes.
 *
 * Revision 1.2  2011/07/20 17:22:27  dja1987
 * SCDL housekeeping code added
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
 * Revision 5.0  2009/02/02 03:05:21  creamop
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
 * Revision 1.1.1.1  2009/01/24 03:44:33  creamop
 * hahaha
 *
 * Revision 4.0  2009/01/20 08:13:49  creamop
 * CREAM IV flight version.
 *
 * Revision 1.1.1.1  2009/01/20 08:12:09  creamop
 * hahaha
 *
 * Revision 1.22  2007/05/18 19:58:43  pbhoyar
 * added DAT_DV for CherCam delay values
 *
 * Revision 1.21  2007/02/21 00:11:28  juneki
 * added CER pedestal and led runs
 *
 * Revision 1.20  2006/08/18 21:00:56  syzinn
 * EVT_TRG_BSP introduced.
 *
 * Revision 1.19  2006/08/09 17:37:34  syzinn
 * adjusted SBT_BUFSIZ
 *
 * Revision 1.18  2006/07/20 16:16:02  syzinn
 * added pedestal and charge calibration run for silicon beam tracker.
 *
 * Revision 1.17  2006/05/02 19:53:16  syzinn
 * changes for USB SBT data format
 *
 * Revision 1.16  2006/05/01 19:39:49  syzinn
 * support for silicon beam tracker
 *
 * Revision 1.15  2006/03/28 18:43:57  syzinn
 * added MAX_CER_MODULE
 *
 * Revision 1.14  2006/03/24 14:42:42  syzinn
 * Merged a branch post_flight2_changes
 *
 * Revision 1.13.2.1  2006/03/15 21:48:26  syzinn
 * added DAT_STAT type
 *
 * Revision 1.13  2005/11/08 19:05:42  cvs
 * adjusted MAX_CER_CH
 *
 * Revision 1.12  2005/11/07 19:20:30  cvs
 * cerenkov camera support
 *
 * Revision 1.11  2005/10/28 14:42:33  syzinn
 * event type 22 is back but marked as obsolete
 *
 * Revision 1.10.2.2  2005/09/22 20:16:53  cvs
 * fixed defining HKSEC_dSCD and HKSEC_CER
 *
 * Revision 1.10.2.1  2005/09/22 18:34:38  cvs
 * support Cerenkov Camera
 *
 * Revision 1.10  2005/09/01 15:07:34  cvs
 * is_triggered() added
 *
 * Revision 1.9  2005/07/21 18:35:36  cvs
 * support for CVD
 *
 * Revision 1.8  2005/06/10 19:52:54  cvs
 * new master trigger
 *
 * Revision 1.7  2005/06/09 19:14:54  cvs
 * fixed channel displacements for top and bottom SCDs
 *
 * Revision 1.6  2005/06/06 15:43:09  cvs
 * removed
 *
 * Revision 1.5  2005/04/08 16:35:57  cvs
 * added sw event number & 16-bit crc
 *
 * Revision 1.4  2005/02/28 02:10:43  daq
 * cleaned up
 *
 * Revision 1.3  2005/02/28 01:32:48  daq
 * added tSCD, bSCD, and CER
 *
 * Revision 1.2  2005/02/28 01:19:11  daq
 * defined more BUFSIZs
 *
 * Revision 1.1  2005/02/26 17:23:37  daq
 * constants moved from crmevt.h
 *
 */

/*
 * data types
 *
 * type number 0 is reserved for handling special packets.
 * when data of type 0 appears in a packet, the rest of the data
 * including type will be ignored.  the length of such zero data
 * expands to the packet boundary.
 *
 * type number 0xff is reserved for run tail.
 */
#define DAT_NULL	0 /* null (NULL) */
#define DAT_EVT		1 /* event (EVT) */
#define DAT_CTRL	2 /* control data (CTRL) */
#define DAT_HK		3 /* housekeeping (HK) */
#define DAT_MSG		4
#define DAT_STV		5
#define DAT_MET		6 /* event for monitoring (MET); use same structure as DAT_EVT */
#define DAT_AUX		7 /* auxiliary string data */
#define DAT_TEG		8 /* trigger efficiency or gain data */
#define DAT_SPI		9 /* sparsification information (part 1) */
#define DAT_SPI2       10 /* sparsification information (part 2) */
#define DAT_SYS        11 /* system command output file */
#define DAT_GTB        12 /* gain table from charge or LED calib. runs */
#define DAT_TCF        13 /* TCD configuration data (part of housekeeping) */
#define DAT_CPDS       14 /* Calorimeter pedestal data */
#define DAT_HPDS       15 /* Hodoscope pedestal data */
#define DAT_SPDS       16 /* SCD pedestal data */
#define DAT_CZCL       17 /* Calorimeter charge calibration data */
#define DAT_HZCL       18 /* Hodoscope charge calibration data */
#define DAT_SZCL       19 /* SCD charge calibration data */
#define DAT_CLED       20 /* Calorimeter LED data */
#define DAT_HLED       21 /* Hodoscope LED data */
#define DAT_ARC	       22 /* obsolete */
#define DAT_SPI3       23 /* sparsification information (part 3) */
#define DAT_HK2	       24 /* reduced housekeeping event */
#define DAT_TPDS       25 /* TRD pedestal data */
#define DAT_tSPDS      26 /* SCD pedestal data */
#define DAT_bSPDS      27 /* SCD pedestal data */
#define DAT_tSZCL      28 /* SCD charge calibration data */
#define DAT_bSZCL      29 /* SCD charge calibration data */
#define DAT_STAT       30 /* status event; or just echoes back a command */
#define DAT_BPDS       31 /* SBT pedestal data */
#define DAT_BZCL       32 /* SBT charge calibration data */
#define DAT_KPDS       33 /* CER pedestal data */
#define DAT_KLED       34 /* CER LED data */
#define DAT_DV	       35 /* CER DV data */
#define DAT_EOF	      255 /* reserved to denote end of file */
#define DAT_SLPDS1      36 /* SCDL pedestal data */    //Added by David Angelaszek 9/3/2010 Problem
#define DAT_SLPDS2      37 /* SCDL pedestal data */   //Added by David Angelaszek 9/3/2010 Problem
#define DAT_SLPDS3      38 /* SCDL pedestal data */   //Added by David Angelaszek 9/3/2010 Problem
#define DAT_SLZCL1      39 /* SCDL charge calibration data */   //Added by David Angelaszek 9/3/2010 Problem
#define DAT_SLZCL2      40 /* SCDL charge calibration data */  //Added by David Angelaszek 9/3/2010 Problem
#define DAT_SLZCL3      41 /* SCDL charge calibration data */  //Added by David Angelaszek 9/3/2010 Problem
#define DAT_TCRDPDS       42 /* TCRD pedestal data */ //YONAS
#define DAT_TCRDZCL       43 /* TCRD charge calibration data */ //YONAS
#define DAT_TCRDLED       44 /* TCRD LED data */ //YONAS
#define DAT_TEG_TCRD	  45 /* trigger efficiency or gain data for TCD/BCD */
#define DAT_TCF_TCD	46	// tcd configuration
#define DAT_TCF_CVD	47	// cd configuration
#define DAT_TCF_BSD	48	// bsd configuration#define DAT_HK2_STELLA	       49 /* reduced housekeeping event */
#define DAT_SFC          50 /*SFC interconnect diagnostic data*/
#define DAT_IIU          51 /*IIU Diagnostic Data*/
#define DAT_BAD          52 /*BAD Data*/
#define DAT_BSDLED       53 /* Calorimeter LED data */

//#define DAT_BSDF        49 /* BSD configuration data (part of housekeeping) */ // Added by Raym


/* test if an event is trigger initiated */
//#define is_triggered(t) (t == DAT_EVT || (t >= DAT_CPDS && t <= DAT_HLED) || (t >= DAT_TPDS && t <= DAT_bSZCL))

#define is_triggered(t) (t == DAT_EVT || (t >= DAT_CPDS && t <= DAT_HLED) || (t >= DAT_TPDS && t <= DAT_bSZCL) || (t >= DAT_KPDS && t <= DAT_DV))//PGB 2 Oct 2007

/* bit-mask for the trigger */
/* bit value 1 implies that corresponding trigger is active */
/* only exception is EVT_TRG_TRG where bit value 0 implies active */
#define EVT_TRG_RSV9	0x8000
#define EVT_TRG_CDTRG2	0x4000
#define EVT_TRG_CDTRG1	0x2000
#define EVT_TRG_EHI	0x1000
#define EVT_TRG_ELO	0x0800
#define EVT_TRG_ZCLB	0x0400
#define EVT_TRG_S3LO	0x0200
#define EVT_TRG_S3HI	0x0100
#define EVT_TRG_TRG	0x0080
#define EVT_TRG_CAL	0x0040
#define EVT_TRG_CLB	0x0020
#define EVT_TRG_EXT	0x0010
#define EVT_TRG_ZHI	0x0008
#define EVT_TRG_ZLO	0x0004
#define EVT_TRG_TMO	0x0002
#define EVT_TRG_BSP	0x0001	/* beam-spill */

/* bitmasks for event composition */
#define EVT_EVC_NON	0x0000
#define EVT_EVC_SCDL1	0x8000
#define EVT_EVC_SCDL2	0x4000
#define EVT_EVC_SCDL3   0x2000
#define EVT_EVC_TCRD	0x1000 // Reserved for future detectors---TCRD YONAS
#define EVT_EVC_SBT	0x0800 /* Silicon beam tracker */
#define EVT_EVC_CVD	0x0400
#define EVT_EVC_tSCD	0x0200
#define EVT_EVC_bSCD	0x0100
#define EVT_EVC_TRG	0x0080
#define EVT_EVC_CMC	0x0040
#define EVT_EVC_CAL	0x0020
#define EVT_EVC_HDS	0x0010
#define EVT_EVC_SCD	0x0008
#define EVT_EVC_TCD	0x0004
#define EVT_EVC_BSD     0x0002 // Added by Raymond Guerci
#define EVT_EVC_TRD	0x0000
#define EVT_EVC_CER	0x0001
//#define EVT_EVC_SCDL1	0x10000  //Added by David Angelaszek 9/3/2010 
//#define EVT_EVC_SCDL2	0x20000  //Added by David Angelaszek 9/3/2010 
//#define EVT_EVC_SCDL3	0x40000 //Added by David Angelaszek 9/3/2010 


/* indices for data section information */
#define EVT_DSI_CAMAC	0
#define EVT_DSI_CAL	1
#define EVT_DSI_HDS	2
#define EVT_DSI_SCD	3
#define EVT_DSI_TCD	4
#define EVT_DSI_TRD	5
#define EVT_DSI_CER	6
#define EVT_DSI_tSCD	7
#define EVT_DSI_bSCD	8
#define EVT_DSI_CVD	9
#define EVT_DSI_SBT	10
#define EVT_DSI_SCDL1	11
#define EVT_DSI_SCDL2	12
#define EVT_DSI_SCDL3	13
#define EVT_DSI_TCRD	14 // Reserved for future detectors---TCRD YONAS
#define EVT_DSI_BSD	15 // Reserved for future detectors


/* Number of devices producing event number */
#define MAX_EVNUM_DEV	19

/* max number of channels */
#define MAX_CAMAC_CH	16
#define MAX_CAL_CH	2560
#define MAX_TCRD_CH	1280 //YONAS
#define MAX_HDS_CH	2944	/* only 2304 points are valid */
#define VLD_HDS_CH	2304
#define MAX_SCD_CH	2944
#define MAX_TCD_CH	256
#define MAX_TRD_CH	1792
#define MAX_BSD_CH 36  // Added by Raymond Guerci (32 backgroud + 32 Prompt Signal + 32 neutron
//#define MAX_TRDPHA_CH		14
//#define MAX_TRDAMPLEX_CH	2048 // with 1024, it crashed sometimes.
//#define MAX_TRD_CH	(MAX_TRDPHA_CH + MAX_TRDAMPLEX_CH + 24)
			/* fifo1 (2048 + 6) + fifo2 (2) + fifo3 (30) */
#define MAX_tSCD_CH	2944
#define MAX_bSCD_CH	2944
#define MAX_SCDL1_CH	2944  //Added by David Angelaszek 9/3/2010 Problem
#define MAX_SCDL2_CH	2944  //Added by David Angelaszek 9/3/2010 Problem
#define MAX_SCDL3_CH	2944  //Added by David Angelaszek 9/3/2010 Problem
#define MAX_CER_CH	1600
#define MAX_CER_MODULE	100
#define MAX_CVD_CH	8
#define MAX_SBT_CH	256

#define CAMAC_BUFSIZ	 1024 //  1 kB
#define CAL_BUFSIZ	16384 // 16 kB
#define TCRD_BUFSIZ	16384 // 16 kB //YONAS
#define HDS_BUFSIZ	16384 // 16 kB
#define SCD_BUFSIZ	16384 // 16 kB
#define tSCD_BUFSIZ	16384
#define bSCD_BUFSIZ	16384
#define SCDL1_BUFSIZ	16384 //Added by David Angelaszek 9/3/2010 Problem
#define SCDL2_BUFSIZ	16384 //Added by David Angelaszek 9/3/2010 Problem
#define SCDL3_BUFSIZ	16384 //Added by David Angelaszek 9/3/2010 Problem
#define TCD_BUFSIZ	16384
#define TRD_BUFSIZ	16384
#define CER_BUFSIZ	16384
#define CVD_BUFSIZ	  256 // 0.25 kB
#define SBT_BUFSIZ	 1032
#define BSD_BUFSIZ 128  // Added by Raymond Guerci

/* Channel number displacements */
#define CDSPL_CAL	0
#define CDSPL_TCRD	0 //TCRD
#define CDSPL_TRD	0
#define CDSPL_HDS	4096	/* Hodoscope ch starts from 4096 */
#define CDSPL_SCD      0//	8192	/* SCD ch starts from 8192 */
#define CDSPL_tSCD     0//		4096	/* top SCD equiv. to SCD */
#define CDSPL_bSCD	0	/* bottom SCD equiv. to hodosceop */
#define CDSPL_CER	0
#define CDSPL_SBT	0
#define CDSPL_SCDL1	0//4096	/* SCDL1 */  //Added by David Angelaszek 9/3/2010
#define CDSPL_SCDL2	0//4096	/*  SCDL2 equiv */  //Added by David Angelaszek 9/3/2010 
#define CDSPL_SCDL3	4096	/* SCDL3 */  //Added by David Angelaszek 9/3/2010 


/* values of gtb_data.table */
#define GTB_CZCAL	0	/* Calorimeter charge calibration */
#define GTB_HZCAL	1	/* Hodoscope charge calibration */
#define GTB_SZCAL	3	/* SCD charge calibration */
#define GTB_CLED	4	/* Calorimeter LED */
#define GTB_HLED	5	/* Hodoscope LED */
#define GTB_tSZCAL	6	/* Top SCD */
#define GTB_bSZCAL	7	/* Bottom SCD */
#define GTB_BZCAL	8	/* Silicon Beam Tracker */
#define GTB_KLED	9	/* CER LED */
#define GTB_SLZCAL1	10      /* SCDL1 */	                  //Added by David Angelaszek 9/3/2010 Problem
#define GTB_SLZCAL2	11	/* SCDL2*/             //Added by David Angelaszek 9/3/2010 Problem
#define GTB_SLZCAL3	12	/* SCDL3 */             //Added by David Angelaszek 9/3/2010 Problem
#define GTB_TCRDZCAL	13	/* TCRD charge calibration */
#define GTB_BSDLED	14	/* BSD LED */

/* values of hk sections */
#define HKSEC_TRG 0
#define HKSEC_SW  1
#define HKSEC_GPS 2
#define HKSEC_MTT 3
#define HKSEC_CAL 4
#define HKSEC_SCDL1 5
//#define HKSEC_HDS 5 Obsolete and changed to SCDL1 David Angelaszek 7/1/2011
//#define HKSEC_SCD 6 Obsolete and changed to SCDL2 David Angelaszek 7/1/2011
#define HKSEC_SCDL2 6
#define HKSEC_tSCD 7
#define HKSEC_bSCD 8
#define HKSEC_TRD 16 // NOT USED
#define HKSEC_BSD 9 // Added by Raymond Guerci
#define HKSEC_TCD 10 // A
#define HKSEC_INST 11 // B
#define HKSEC_ISS 12
//#define HKSEC_dSCD 12 // C Obsolete and changed to SCDL3 David Angelaszek 7/1/2011
#define HKSEC_CER 13 // D
#define HKSEC_TCRD 14 // YONAS
#define HKSEC_EOS 15 // F
#define HKSEC_SCDL3 16// not used
#endif


/*
 * Copy from bsd BSD-event.h
 */
#define BSD_NUM_RAW_BYTES 84 // number of raw bytes from USB
#define BSD_SKIP_TO_DATA	10  // skip 12 bytes
#define NUM_LATE 16
#define NUM_EARLY 2
#define OFFSET_LOW_GAIN 0
#define OFFSET_HIGH_GAIN (OFFSET_LOW_GAIN + NUM_LATE)
#define OFFSET_TRG_LOW_GAIN     (OFFSET_HIGH_GAIN + NUM_LATE)
#define OFFSET_TRG_HIGH_GAIN (OFFSET_TRG_LOW_GAIN + NUM_EARLY)
