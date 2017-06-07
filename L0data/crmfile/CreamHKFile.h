#ifndef CREAMHKFILE_H
#define CREAMHKFILE_H
//
// CreamHKFile.h --- class for reading CREAM housekeeping files
//
// 2005/6/3 SYZ
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
// $Log: CreamHKFile.h,v $
// Revision 1.3  2012/11/16 21:11:55  dja1987
// Fixes to TCD/BCD crmfile. Updated HK format. Improved client playback (no more temporary arrays for missing packet numbers). Updated cprint
//
// Revision 1.2  2012/03/21 20:41:34  dja1987
// T/BCD updates.
//
// Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
// Importing SCDL bug fixes.
//
// Revision 1.2  2011/07/20 17:22:27  dja1987
// SCDL housekeeping code added
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
// Revision 1.25  2007/05/18 19:53:41  pbhoyar
// add 1 reserved for TCD
//
// Revision 1.24  2007/04/02 17:06:51  juneki
// Add tcd hv monitor
//
// Revision 1.23  2007/02/21 00:10:22  juneki
// added CER HK channels
//
// Revision 1.22  2006/08/25 19:46:38  syzinn
// different timer resolution for format 5; total/live times are stored in min.sec format
//
// Revision 1.21  2006/08/25 15:40:22  syzinn
// raw trigger counts
//
// Revision 1.20  2006/06/29 19:51:49  juneki
// *** empty log message ***
//
// Revision 1.19  2006/06/27 20:28:05  juneki
// support cer sparsification with GUI
//
// Revision 1.18  2006/03/24 14:42:38  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.17.2.1  2006/03/14 21:07:38  syzinn
// eight-byte timestamp
//
// Revision 1.17  2005/11/24 02:34:06  syzinn
// private variable _tmconv_fmt1; #pragma for CINT preprocessor
//
// Revision 1.16  2005/11/07 19:14:51  cvs
// cerenkov camera support
//
// Revision 1.15.2.1  2005/09/22 20:16:29  cvs
// added section for Cerenkov Camera
//
// Revision 1.15  2005/08/08 22:56:23  syzinn
// backward compatibility
//
// Revision 1.14  2005/07/16 00:35:02  cvs
// corrected live and total time
//
// Revision 1.13  2005/07/12 20:10:32  cvs
// unpack reduced hk event of fmt 1
//
// Revision 1.12  2005/07/06 21:20:41  cvs
// support for hk ver 3
//
// Revision 1.11  2005/06/16 21:16:25  cvs
// reduced dual SCD buffer to fit
//
// Revision 1.10  2005/06/16 20:44:39  cvs
// select mode to collect hk data from dual SCDs
//
// Revision 1.9  2005/06/11 12:18:01  cvs
// bug fix in hk system
//
// Revision 1.8  2005/06/10 19:52:16  cvs
// new master trigger
//
// Revision 1.7  2005/06/10 17:10:42  cvs
// added flags for buffers
//
// Revision 1.6  2005/06/10 17:05:43  cvs
// took out flag from hk sections
//
// Revision 1.5  2005/06/07 18:39:45  cvs
// updated
//
// Revision 1.4  2005/06/07 12:22:51  cvs
// first working version for new hk system
//
// Revision 1.3  2005/06/07 00:22:08  cvs
// this is enough for today
//
// Revision 1.2  2005/06/07 00:16:16  cvs
// still under modification
//
// Revision 1.1  2005/06/06 21:30:55  cvs
// new
//
//

#include <stdio.h>
#pragma preprocess defs
#include "defs.h"
#include "CreamHKFileHeader.h"

// sections
struct hk_trg {
	/* trigger rate */
	timestamp4_t	trg_ts;

	real4		trg_tot; /* overall trigger rate */
	real4		trg_ext; /* external */
	real4		trg_clb; /* calibration trigger rate in Hz */
	real4		trg_ehi; /* High Energy */
	real4		trg_elo; /* Low Energy */
	real4		trg_zclb; /* ZCALIB */
	real4           trg_tcrd; /*TCD/BCD*/ 

	/* raw trigger rates */
	real4		raw_zhi; /* Z-HI */
	real4		raw_cal; /* calorimeter */
	//real4		raw_tcrd; /* TCRD */ //YONAS
	real4		raw_zlo; /* Z-LOW */
	//real4		raw_s3hi; /* S3-HI */
	//real4		raw_s3lo; /* S3-LOW */
	real4		raw_clb; /* calibration */
	real4		raw_ext; /* external */
};

struct hk_sw {
	/* network i/o rate meter */
	timestamp4_t	nio_ts;
	real4		nio_tkbps;
	real4		nio_nkbps;

	/* buffer fill level in percentage * 256 */
	uint2		bufl_cmdq; // command queue fill level in format 8.8
	uint2		bufl_hkq; // hk command queue
	uint2		bufl_evtq; // evt command queue
	uint2		bufl_datq; // data queue
	uint2		bufl_datq2; // hk data queue
	uint2		bufl_pkt0; // data packet pool
	uint2		bufl_pkt1; // hk packet pool
	uint2           bufl_pkt2; // send packet pool

	real4           flash1;
	real4		flash2;
	real4		flash3;
	real4		flash4;

	/* disk space in percentage * 256 */
	uint2		disk_space; // data disk space
	uint2		sys_disk_space; //system disk space PGB 15 July 2008
	real4           free_ram;      

	// the following is experimental!
	uint2		cmd_ser; // serial number of the last command received
	uint2		cmd_code; // code of the last command received.

//	uint2		reserved[10]; PGB 15 JUly 2008
	uint2		reserved[9];
};

struct hk_gps {
	/* gps */
	timestamp4_t	gps_ts;
	gps_t		gps;	/* 68 bytes */
};
struct hk_gps_sip {
	/* gps */
	timestamp4_t	gps_ts;
	gps_sip_t		gps;	/* 68 bytes */
};

struct hk_mtt {
	/* master trigger timers */
	char	live[6]; /* 48 bit number */
	char	total[6]; /* 48 bits number */
};

struct hk_cal {
	uint2	temp[8];
	uint2	hv_a[8];
	uint2	hv_b[8];
	uint2	bias[8];
	
	uint2 cal_3o3v;
	uint2 cal_p5o2v;
	uint2 cal_m5o2v;
	uint2 cal_12v;
	uint2 cal_temp;
	uint2 cal_12c[14];
	uint2 cal_p5o2c[14];
	uint2 cal_m5o2c[14];
	uint2 cal_gnd;

	uint2	cm3o6[8]; // -3.6V current
	uint2	c3o6[8]; // 3.6V current
	uint2	c12[8]; // 12V current
	uint2	reserved[4];
	uint2	pwr_logic; // system logic voltage
	uint2	pwr_vm3o6; // system -3.6V monitor
	uint2	pwr_v3o6; // system 3.6V monitor
	uint2	pwr_v12; // system 12V monitor
	uint2	pwr_cm3o6; // system -3.6V current
	uint2	pwr_c3o6; // system 3.6V current
	uint2	pwr_c12; // system 12V current
	uint2	ctrg; // trigger +3.6V current
	uint2	hv[8];
};
struct hk_tcrd {
	uint2 temp[4];
	uint2 tcdbias[4];
	uint2 bcdbias[4];
}; //YONAS
struct hk_hds {
	uint2	pwr_logic; // system logic voltage
	uint2	pwr_vm6; // system -6V monitor
	uint2	pwr_v6; // system 6V monitor
	uint2	pwr_v12; // system 12V monitor
	uint2	reserved[8];

	uint2	temp1[12];
	uint2	temp2[12];
	uint2	temp3[12];
	uint2	hv[12];
	uint2	bias[12];
	uint2	cm6[12]; // -6V current
	uint2	c6[12]; // 6V current
	uint2	c12[12]; // 12V current
};

struct hk_scd {
	uint2	pwr_vlogic;
	uint2	pwr_vm6o5;
	uint2	pwr_v6o5;
	uint2	pwr_v12;
	uint2	mb_cm6o5[2];
	uint2	mb_c6o5[2];
	uint2	mb_c12[2];
	uint2	reserved[2];

	uint2	temp[26];
	uint2	bias[26];
	uint2	vlogic[26];
	uint2	vm5[26];
	uint2	v5[26];
};

struct hk_tscd {


	uint2 scd1_3o3v;
	uint2 scd1_p6o5v;
	uint2 scd1_m6o5v;
	uint2 scd1_12v;
	uint2 scd1_temp;
	uint2 scd1_12c[14];
	uint2 scd1_p6o5c[14];
	uint2 scd1_m6o5c[14];
	uint2 scd1_gnd;

	uint2	temp[24];
	uint2	bias[24];
	uint2	vlogic[24];
	uint2	vm5[24];
	uint2	v5[24];
	uint2	cbias[24];
	uint2	clogic[24];
	uint2	cm5[24];
	uint2	c5[24];

	uint2	pwr_vlogic;
	uint2	pwr_vm6o5;
	uint2	pwr_v6o5;
	uint2	pwr_v12;
	uint2	mb_cm6o5[2];
	uint2	mb_c6o5[2];
	uint2	mb_c12[2];
	uint2	reserved[6];
	
};

struct hk_bscd {

	uint2 scd1_3o3v;
	uint2 scd1_p6o5v;
	uint2 scd1_m6o5v;
	uint2 scd1_12v;
	uint2 scd1_temp;
	uint2 scd1_12c[14];
	uint2 scd1_p6o5c[14];
	uint2 scd1_m6o5c[14];
	uint2 scd1_gnd;


	uint2	temp[24];
	uint2	bias[24];
	uint2	vlogic[24];
	uint2	vm5[24];
	uint2	v5[24];
	uint2	cbias[24];
	uint2	clogic[24];
	uint2	cm5[24];
	uint2	c5[24];


	uint2	pwr_vlogic;
	uint2	pwr_vm6o5;
	uint2	pwr_v6o5;
	uint2	pwr_v12;
	uint2	mb_cm6o5[2];
	uint2	mb_c6o5[2];
	uint2	mb_c12[2];
	uint2	reserved[6];
};


struct hk_scdl1 {                      // New struct added by David Angelaszek 9/2/2010

	uint2 scd2_3o3v;
	uint2 scd2_p6o5v;
	uint2 scd2_m6o5v;
	uint2 scd2_12v;
	uint2 scd2_temp;
	uint2 scd2_12c[14];
	uint2 scd2_p6o5c[14];
	uint2 scd2_m6o5c[14];
	uint2 scd2_gnd;

	uint2	temp[24]; 
	uint2	bias[24];
	uint2	vlogic[24];
	uint2	vm5[24];
	uint2	v5[24];
	uint2	cbias[24];
	uint2	clogic[24];
	uint2	cm5[24];
	uint2	c5[24];


	uint2	pwr_vlogic;
	uint2	pwr_vm6o5;
	uint2	pwr_v6o5;
	uint2	pwr_v12;
	uint2	mb_cm6o5[2];
	uint2	mb_c6o5[2];
	uint2	mb_c12[2];
	uint2	reserved[6];
};

struct hk_scdl2 {                      // New struct added by David Angelaszek 9/2/2010

	uint2 scd2_3o3v;
	uint2 scd2_p6o5v;
	uint2 scd2_m6o5v;
	uint2 scd2_12v;
	uint2 scd2_temp;
	uint2 scd2_12c[14];
	uint2 scd2_p6o5c[14];
	uint2 scd2_m6o5c[14];
	uint2 scd2_gnd;


	uint2	temp[24];   
	uint2	bias[24];
	uint2	vlogic[24];
	uint2	vm5[24];
	uint2	v5[24];
	uint2	cbias[24];
	uint2	clogic[24];
	uint2	cm5[24];
	uint2	c5[24];

	uint2	pwr_vlogic;
	uint2	pwr_vm6o5;
	uint2	pwr_v6o5;
	uint2	pwr_v12;
	uint2	mb_cm6o5[2];
	uint2	mb_c6o5[2];
	uint2	mb_c12[2];
	uint2	reserved[6];

};

struct hk_scdl3 {                      // New struct added by David Angelaszek 9/2/2010
	uint2	pwr_vlogic;
	uint2	pwr_vm6o5;
	uint2	pwr_v6o5;
	uint2	pwr_v12;
	uint2	mb_cm6o5[2];
	uint2	mb_c6o5[2];
	uint2	mb_c12[2];
	uint2	reserved[6];

	uint2	temp[26];   
	uint2	bias[26];
	uint2	vlogic[26];
	uint2	vm5[26];
	uint2	v5[26];
	uint2	cbias[26];
	uint2	clogic[26];
	uint2	cm5[26];
	uint2	c5[26];
};

struct hk_trd {
	uint2	cm3o6;
	uint2	c3o6;
	uint2	spare[8];
	uint2	hv[3];
	uint2	hv_pwr[3];
	uint2	temp[8];
	uint2	pressure[11];
};

struct hk_tcd {
	uint2	vm2;
	uint2	vm5;
	uint2	v3o3;
	uint2	v5analog;
	uint2	v5;
	uint2	v12;
	uint2	temp[5];
//	uint2	hv[9];PGB 05 July 2008
	uint2   hv[10];
	uint2	reserved[6];
};

/*
 *  BSD House keeping data
 */
struct hk_bsd {
/*#ifdef VERSION0
#define BSD_HK_ITEMS_CNT	32
#define BSD_HK_OFFSET_AT_HKCONV	148
	enum hk_bsd_index {
		PMT1= 0, PMT2 = 1,PMT3 = 2, PMT4 = 3,
		SCINTIL	= 4,BSD_DET_BOX = 5, FPGA = 6, BSD_ELE_BOX = 7,
		PS1 = 0, PS2 = 1, PS3 = 2,	// PMT1_T
		DC_PLUS50v= 0, DC_MINU_50V = 1, DC_PLUS_33v = 2, DC_MINU_15v = 3
		, DC_PLUS_120v = 4, DC_MINU_12v= 5,
	};
#else
/*
 *  BSD position is replaced by  hodoscope
 */
/*#define BSD_HK_ITEMS_CNT	32
#define BSD_HK_OFFSET_AT_HKCONV	148 // HDS offset
	enum hk_bsd_index { // 02/20/2014
		PMT1= 0, PMT2 = 1,PMT3 = 2, PMT4 = 3,
		SCINTIL	= 4,BSD_DET_BOX = 5, BSD_ELE_BOX = 6,
		HVMON11 = 7,HVMON21 = 8,HVMON31 = 9,
		HVMON12 = 10,HVMON22 = 11,HVMON32 = 12,
		HVMON13 = 13,HVMON23 = 14,HVMON33 = 15,
		HVMON14 = 16,HVMON24 = 17,HVMON34 = 18,
		HVMON1 = 19, HVMON2 = 20,
        VM12V = 21, VP33V = 22, VP15V = 23,
        IP12V1 = 24, IP12V2 = 25, IP12V3 = 26, IP12V4 = 27,
        IP12VT1 = 28, IP12VT2 = 29
	};
#endif
 uint2 data[BSD_HK_ITEMS_CNT];
*/
#define BSD_HK_ITEMS_CNT	32
	uint2 bsd_return[2];
	uint2 bsd_Q[4];

	uint2 bsd_curT1;
	uint2 bsd_p3o3V;
	uint2 bsd_curT2;
	uint2 bsd_m12V;
	uint2 bsd_m1o5V;

	uint2 bsd_sciTemp;
	uint2 bsd_detEnclT;
	uint2 bsd_eleEnclT;

	uint2 bsd_VHV11;
	uint2 bsd_IHV11;
	uint2 bsd_VHV12;
	uint2 bsd_IHV12;
	uint2 bsd_VHV13;
	uint2 bsd_IHV13;
	uint2 bsd_IHV14;
	uint2 bsd_VHV21;
	uint2 bsd_VHV22;
	uint2 bsd_VHV23;
	uint2 bsd_VHV31;
	uint2 bsd_VHV32;
	uint2 bsd_VHV33;
	uint2 bsd_VHV41;
	uint2 bsd_VHV42;
	uint2 bsd_VHV43;
	
	uint2 bsd_HVTr1;
	uint2 bsd_HVTr2;
};

struct hk_issnew
{
	uint2 thermistor[18];
	uint2 sfcAcurrent;
	uint2 sfcBcurrent;
	uint2 sfcAtemp;
	uint2 sfcBtemp;
	uint2 hcBtemp;
	uint2 hcAtemp;
	uint2 iiuCurrent;
	uint2 sfc_intercon_current;
	uint2 sfcAret;

};


struct hk_inst {
	uint2	temp[12];
	uint2	reserved[4];
};

struct hk_dscd {
	uint2	top_pwr_vlogic;
	uint2	top_pwr_vm6o5;
	uint2	top_pwr_v6o5;
	uint2	top_pwr_v12;

	uint2	bot_pwr_vlogic;
	uint2	bot_pwr_vm6o5;
	uint2	bot_pwr_v6o5;
	uint2	bot_pwr_v12;

	uint2	top_temp[26];
	uint2	top_bias[26];
	uint2	top_clogic[26];
	uint2	top_cm5[26];
	uint2	top_c5[26];

	uint2	bot_temp[26];
	uint2	bot_bias[26];
	uint2	bot_clogic[26];
	uint2	bot_cm5[26];
	uint2	bot_c5[26];
};



struct hk_cer {
	uint2	hv[100];
	uint2	hv_c28[2];
	uint2	hv_cm3o3[2];
	uint2	hv_ctrl_c12[2];
	uint2	hv_ctrl_cm3o3[2];
	uint2	ps_v5_analog;
	uint2	ps_v5_digital;
	uint2	ps_vm3o3;
	uint2	ps_v12;
	uint2	ps_v28;
	uint2	sprs_c5;
	uint2	merger_c3o3[2];
	uint2	merger_cm3o3[2];
	uint2	merger_c2o5[2];
	uint2	merger_c5[2];
	uint2	hk_c12;
	uint2	hk_cm3o3;
	uint2	merger_temp[2];
	uint2	temp[14];
};

struct bad_data_hk {
	real4 x_pos_iner;
	real4 y_pos_iner;
	real4 z_pos_iner;
	real4 vx_pos_iner;
	real4 vy_pos_iner;
	real4 vz_pos_iner;
	real4 q1_iner;
	real4 q2_iner;
	real4 q3_iner;
	real4 q4_iner;
	real4 x_pos_ecf;
	real4 y_pos_ecf;
	real4 z_pos_ecf;
	real4 vx_pos_ecf;
	real4 vy_pos_ecf;
	real4 vz_pos_ecf;
	real4 q1_ecf;
	real4 q2_ecf;
	real4 q3_ecf;
	real4 q4_ecf;
	real4 q1_lvlh;
	real4 q2_lvlh;
	real4 q3_lvlh;
	real4 q4_lvlh;
};
	

class DHKEvent;
class DHKEventConverter;
class CreamHKFile : public CreamHKFileHeader {
	// File
	FILE* _fp;
	long _filesz;
	int _fmt; // -1: not determined yet, 0 or higher are valid.

	// per event
	int clear(void);
	unsigned _nread;
	unsigned _crc16;
	unsigned _serial;

	// buffer for each section
	hk_trg	_trg_buf;
	hk_sw	_sw_buf;
	hk_gps	_gps_buf;
	hk_gps_sip _gps_buf_sip;
	hk_mtt	_mtt_buf;
	hk_cal	_cal_buf;
	hk_tcrd	_tcrd_buf; //YONAS
	hk_hds	_hds_buf;
	hk_scd	_scd_buf;
	hk_tscd	_tscd_buf;
	hk_bscd	_bscd_buf;
	hk_trd	_trd_buf;
	hk_tcd	_tcd_buf;
	hk_bsd	_bsd_buf;
	hk_inst	_inst_buf;
	hk_dscd _dscd_buf;
	hk_cer	_cer_buf;
	hk_scdl1 _scdl1_buf;  //Added by David Angelaszek 9/2/2010
	hk_scdl2 _scdl2_buf;   //Added by David Angelaszek 9/2/2010
	hk_scdl3 _scdl3_buf;    //Added by David Angelaszek 9/2/2010
	bad_data_hk _bad_buf;
 
	// flags
	int	_trg_flag;
	int	_sw_flag;
	int	_gps_flag;
	int	_mtt_flag;
	int	_cal_flag;
	int	_tcrd_flag; //YONAS
	int	_hds_flag;
	int	_scd_flag;
	int	_tscd_flag;
	int	_bscd_flag;
	int	_trd_flag;
	int	_tcd_flag;
	int	_bsd_flag;
	int	_inst_flag;
	int	_dscd_flag;
	int	_cer_flag;
	int	_scdl1_flag;  //Added by David Angelaszek 9/2/2010
	int	_scdl2_flag;  //Added by David Angelaszek 9/2/2010
	int	_scdl3_flag;  //Added by David Angelaszek 9/2/2010
	int     _bad_flag;

	// the following routines record the number of bytes to _nread.
	int read_fmt_0(void); // format 0.0 - 0.5
	int unpack_fmt_1_full(unsigned char *, DHKEventConverter&);
	int read_fmt_1(DHKEventConverter& ev); // format 1.0
	int read_fmt_2(DHKEventConverter& ev) { return read_fmt_1(ev); } // format 2.0
	int unpack_fmt_3_full(unsigned char *, DHKEventConverter&);
	int unpack_fmt_3_reduced(unsigned char *, DHKEventConverter&);
	int read_fmt_3_typefirst(DHKEventConverter& ev); // format 3.0 - 3.3
	int read_fmt_3_lengthfirst(DHKEventConverter& ev); // format 3.4 and 3.5
	int unpack_12bit_array(unsigned char *psrc, void* pdest, int nbytes);
	int unpack_16bit_array(unsigned char *psrc, void* pdest, int nbytes);// ISS-CREAM
	int unpack_fmt_4_full(unsigned char *, DHKEventConverter&);
	int unpack_fmt_1_reduced(unsigned char *, DHKEventConverter&);
	int read_fmt_4(DHKEventConverter& ev); // current format (full ver. 4, reduced ver. 1)

	int unpack_fmt_5_full(unsigned char *, DHKEventConverter&);
	int unpack_fmt_2_reduced(unsigned char *, DHKEventConverter&);
	int read_fmt_5(DHKEventConverter& ev); // current format (full ver. 5, reduced ver. 2)

	int unpack_fmt_6_full(unsigned char *, DHKEventConverter&);
	int unpack_fmt_6_reduced(unsigned char *, DHKEventConverter&);
	int read_fmt_6(DHKEventConverter& ev); // current format (full ver. 6, reduced ver. 2)

	int unpack_fmt_8_full(unsigned char *, DHKEventConverter&);
	int unpack_fmt_8_reduced(unsigned char *, DHKEventConverter&);
	int unpack_bad(unsigned char *, DHKEventConverter&);
	int read_fmt_8(DHKEventConverter& ev); // current format (full ver. 8, reduced ver. 2)

	// total and live timer conversion factor
	double _tmconv_fmt1;
	double _tmconv_fmt5;

public:
	CreamHKFile();
	virtual ~CreamHKFile();

	int OpenRead(char* filename); // reading only
	inline int Open(char* filename) { return OpenRead(filename); }
	int Close(void);
	int Rewind(void);

	inline long GetFileSize(void) { return _filesz; }
	long GetCurrentFilePosition(void);

	int ReadEvent(DHKEventConverter& ev);

	// hook to unpacking methods for the latest format.
	int UnpackFull(unsigned char *p, DHKEventConverter& ev){
//		 return unpack_fmt_5_full(p, ev); 
		if( CREAMHKFILE_CUR_MAJOR == 8){ return unpack_fmt_8_full(p, ev);}
		else{ return unpack_fmt_5_full(p, ev);}
	}
	int UnpackReduced(unsigned char *p, DHKEventConverter& ev){
		if( CREAMHKFILE_CUR_MAJOR == 8 ){ return unpack_fmt_8_reduced(p, ev);}
		else{ return unpack_fmt_2_reduced(p, ev); }
	}
	int UnpackBad(unsigned char *bad_p, DHKEventConverter& ev){
		if( CREAMHKFILE_CUR_MAJOR == 8 ){ return unpack_bad(bad_p, ev);}
	}
};

#endif
