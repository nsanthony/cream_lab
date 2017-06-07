#ifndef DEVENT_H
#define DEVENT_H
//
// DEvent.h
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
// $Log: DEvent.h,v $
// Revision 1.3  2012/11/16 21:11:55  dja1987
// Fixes to TCD/BCD crmfile. Updated HK format. Improved client playback (no more temporary arrays for missing packet numbers). Updated cprint
//
// Revision 1.2  2012/03/21 20:41:34  dja1987
// T/BCD updates.
//
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
// Revision 1.13  2007/06/22 14:50:27  pbhoyar
// added function CERGetGain
//
// Revision 1.12  2006/08/09 19:03:03  syzinn
// SBT data format changed
//
// Revision 1.11  2006/05/01 19:39:24  syzinn
// support for silicon beam tracker
//
// Revision 1.10  2006/03/24 14:42:38  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.9.2.1  2006/03/14 21:07:44  syzinn
// eight-byte timestamp
//
// Revision 1.9  2005/12/17 07:35:12  syzinn
// GetByteLength() added
//
// Revision 1.8  2005/11/07 22:51:54  cvs
// added PrintRawCER()
//
// Revision 1.7  2005/11/07 21:25:32  cvs
// supports Cerenkov Camera data
//
// Revision 1.6  2005/08/31 22:56:26  cvs
// Added method GetSWEventNumber()
//
// Revision 1.5  2005/07/31 17:27:55  cvs
// support CVD data section
//
// Revision 1.4  2005/06/10 19:52:16  cvs
// new master trigger
//
// Revision 1.3  2005/04/08 16:33:49  cvs
// supports format 3.1
//
// Revision 1.2  2005/02/28 02:10:17  daq
// changed to format 3.0
//
// Revision 1.1.1.1  2005/02/04 05:02:17  daq
// initial version for CREAM2.
//
// Revision 1.12  2004/12/16 17:53:36  syzinn
// added printing Z-LOW trigger
//
// Revision 1.11  2004/08/27 21:23:44  syzinn
// fixes TCD and TRD print outs
//
// Revision 1.10  2004/07/26 15:12:00  syzinn
// bug fix
//
// Revision 1.9  2004/07/16 20:04:19  syzinn
// no message
//
// Revision 1.8  2004/07/16 19:50:50  syzinn
// update by paolo
//
// Revision 1.3  2004/07/05 14:03:30  paolo
// *** empty log message ***
//
// Revision 1.6  2004/07/01 18:35:00  syzinn
// deciphers TCD data
//
// Revision 1.5  2004/06/21 13:21:13  syzinn
// provides functions to check the presence of each data section; enlarged buffer size for TRD amplex data.
//
// Revision 1.4  2004/04/22 13:35:39  syzinn
// Added PrintTCDOriginal()
//
// Revision 1.3  2004/04/15 20:59:03  syzinn
// added copyright notice.
//
// Revision 1.2  2004/04/08 14:40:01  syzinn
// prints TRD data in their original format
//
// Revision 1.1.1.1  2004/02/24 21:46:25  syzinn
// crmfile library
//
//

#include "crmevtx.h"
#ifndef GCC_VERSION
#define GCC_VERSION ( __GNUC__ * 10000 \
                      + __GNUC_MINOR__ * 100 \
                      + __GNUC_PATCHLEVEL__ )
#endif
#if GCC_VERSION >= 30200
#include <cstring>
using namespace std;
#else
#include <string.h>
#endif


class DEvent {
	unsigned _type;
	unsigned _raw_nbytes; // byte size of raw data

	// Trigger
	bool _triggerQ;
	unsigned _trigger;

	// CAMAC
	unsigned _camac_nch;
	int _camac_adc[MAX_CAMAC_CH];

	// Calorimeter
	unsigned _cal_nch;
	unsigned _cal_evnum;
	unsigned short _cal_ch[MAX_CAL_CH];
	unsigned short _cal_adc[MAX_CAL_CH];
	// TCRD
	unsigned _tcrd_nch;
	unsigned _tcrd_evnum;
	unsigned short _tcrd_ch[MAX_TCRD_CH];
	unsigned short _tcrd_adc[MAX_TCRD_CH]; //YONAS

	// Hodoscope
	unsigned _hds_nch;
	unsigned _hds_evnum;
	unsigned short _hds_ch[MAX_HDS_CH];
	unsigned short _hds_adc[MAX_HDS_CH];

	// SCD
	unsigned _scd_nch;
	unsigned _scd_evnum;
	unsigned short _scd_ch[MAX_SCD_CH];
	unsigned short _scd_adc[MAX_SCD_CH];

	// tSCD
	unsigned _tscd_nch;
	unsigned _tscd_evnum;
	unsigned short _tscd_ch[MAX_tSCD_CH];
	unsigned short _tscd_adc[MAX_tSCD_CH];

	// bSCD
	unsigned _bscd_nch;
	unsigned _bscd_evnum;
	unsigned short _bscd_ch[MAX_bSCD_CH];
	unsigned short _bscd_adc[MAX_bSCD_CH];

	 //SCDL1                                        //Added by David Angelaszek 9/2/2010
	unsigned _scdl1_nch;
	unsigned _scdl1_evnum;
	unsigned short _scdl1_ch[MAX_SCDL1_CH];
	unsigned short _scdl1_adc[MAX_SCDL1_CH];

	// SCDL2                                          //Added by David Angelaszek 9/2/2010
	unsigned _scdl2_nch;
	unsigned _scdl2_evnum;
	unsigned short _scdl2_ch[MAX_SCDL2_CH];
	unsigned short _scdl2_adc[MAX_SCDL2_CH];

	// SCDL3                                        //Added by David Angelaszek 9/2/2010
	unsigned _scdl3_nch;
	unsigned _scdl3_evnum;
	unsigned short _scdl3_ch[MAX_SCDL3_CH];
	unsigned short _scdl3_adc[MAX_SCDL3_CH];

	// Silicon Beam Tracker
	unsigned _sbt_nch;
	unsigned _sbt_evnum;
	unsigned short _sbt_ch[MAX_SBT_CH];
	unsigned short _sbt_adc[MAX_SBT_CH];

	// TCD
	char _tcd_raw[TCD_BUFSIZ]; // 16 kB
	unsigned _tcd_raw_nbytes; // byte count of data in _tcd_raw[].

	unsigned _tcd_nch;
	unsigned _tcd_evnum;
	unsigned _tcd_nbytes, _tcd_header;
	unsigned _tcd_nitems[10];
	unsigned _tcd_fmtcode, _tcd_bitmask, _tcd_bitcount;
	unsigned _tcd_halfword4, _tcd_halfword5;
	unsigned _tcd_crc16, _tcd_calc16; // CRC from data and from calculation
	unsigned short _tcd_ch[MAX_TCD_CH];
	unsigned short _tcd_adc[MAX_TCD_CH];

   	// BSD
   	 char _bsd_raw[BSD_BUFSIZ]; // 16 kB
   	 unsigned _bsd_raw_nbytes; // byte count of data in _bsd_raw[].

   	 unsigned _bsd_nch;
   	 unsigned _bsd_evnum;
         unsigned _bsd_syncnum;
         unsigned _bsd_nbytes;
         unsigned short _bsd_ch[MAX_BSD_CH];
         unsigned short _bsd_adc[MAX_BSD_CH];

        unsigned _bsd_evcnt;
        unsigned _bsd_tri_mode;
        unsigned _bsd_conf_mode;
        unsigned _bsd_tri_status;
#define BSD_TRIGER_T2   0x04
#define BSD_TRIGER_T1   0x02
#define BSD_TRIGER_EX   0x01


	// CVD
	char _cvd_raw[CVD_BUFSIZ]; // 1 kB
	unsigned _cvd_raw_nbytes;
	
	unsigned _cvd_nch;
	unsigned _cvd_evnum;
	unsigned _cvd_nbytes, _cvd_header;
	unsigned _cvd_nitems[10];
	unsigned _cvd_fmtcode, _cvd_bitmask, _cvd_bitcount;
	unsigned _cvd_halfword4, _cvd_halfword5;
	unsigned _cvd_crc16, _cvd_calc16; // CRC from data and from calculation
	unsigned short _cvd_ch[MAX_CVD_CH];
	unsigned short _cvd_adc[MAX_CVD_CH];

	// Cerenkov Camera
	char _cer_raw[CER_BUFSIZ]; // 1 kB
	unsigned _cer_raw_nbytes;
	
	unsigned _cer_nch;
	unsigned _cer_evnum;
	unsigned short _cer_ch[MAX_CER_CH];
	unsigned short _cer_adc[MAX_CER_CH];
	unsigned short _cer_gain[MAX_CER_CH];

	// TRD trd_selee//////////////////
	unsigned _trd_nch;
	unsigned _trd_evnum;
	unsigned short _trd_ch[MAX_TRD_CH];
	unsigned short _trd_adc[MAX_TRD_CH];
	/**************old Dr.L********************/

	// TRD
	//char _trd_raw[TRD_BUFSIZ]; // 16 kB
	//unsigned _trd_raw_nbytes; // byte count of data in _trd_raw[].

	//unsigned _trd_evnum;
	//unsigned _trd_pha_nch;
	//unsigned _trd_amplex_nch;
	//unsigned short _trd_pha_adc[MAX_TRDPHA_CH];
	//unsigned short _trd_amplex_ch[MAX_TRDAMPLEX_CH];
	//unsigned short _trd_amplex_adc[MAX_TRDAMPLEX_CH];
 /////////////////////////////////////////////////////////////
	// TimeStamp
	bool _timestampQ;
	unsigned _timestamp[9];

	// misc
	unsigned _swevno;
	unsigned _chksum;

	int UnpackSparsified(char* src, unsigned nbytes, unsigned maxch, unsigned displ,
		unsigned& nch, unsigned& evnum, unsigned short* pch, unsigned short* padc);

	// flags to denote the presence of data
	bool _camacQ;
	bool _calQ;
	bool _tcrdQ; //YONAS
	bool _hdsQ;
	bool _scdQ;
	bool _tscdQ;
	bool _bscdQ;
	bool _tcdQ;
	bool _bsdQ;
	bool _cvdQ;
	bool _trdQ;
	bool _cerQ;
	bool _sbtQ;
	bool _swevnoQ;
	bool _chksumQ;
	bool _scdl1Q;
	bool _scdl2Q;
	bool _scdl3Q;

public:
	void Clear();
	int UnpackTrigger(unsigned trig, unsigned trig_len);
	int UnpackCAMAC(char* src, unsigned nbytes);
	int UnpackCalorimeter(char* src, unsigned nbytes) {
		_calQ = true;
		return UnpackSparsified(src, nbytes, MAX_CAL_CH, CDSPL_CAL,
			_cal_nch, _cal_evnum, _cal_ch, _cal_adc); }
	int UnpackTCRD(char* src, unsigned nbytes) {
		_tcrdQ = true;
		return UnpackSparsified(src, nbytes, MAX_TCRD_CH, CDSPL_TCRD,
			_tcrd_nch, _tcrd_evnum, _tcrd_ch, _tcrd_adc); } //YONAS
	int UnpackTRD2(char* src, unsigned nbytes) {
		_trdQ = true;
		return UnpackSparsified(src, nbytes, MAX_TRD_CH, CDSPL_TRD,
			_trd_nch, _trd_evnum, _trd_ch, _trd_adc); }
	int UnpackHodoscope(char* src, unsigned nbytes) {
		_hdsQ = true;
		return UnpackSparsified(src, nbytes, MAX_HDS_CH, CDSPL_HDS,
			_hds_nch, _hds_evnum, _hds_ch, _hds_adc); }
	int UnpackSCD(char* src, unsigned nbytes) {
		_scdQ = true;
		return UnpackSparsified(src, nbytes, MAX_SCD_CH, CDSPL_SCD,
			_scd_nch, _scd_evnum, _scd_ch, _scd_adc); }
	int UnpackTopSCD(char* src, unsigned nbytes) {
		_tscdQ = true;
		return UnpackSparsified(src, nbytes, MAX_tSCD_CH, CDSPL_tSCD,
			_tscd_nch, _tscd_evnum, _tscd_ch, _tscd_adc); }
	int UnpackBottomSCD(char* src, unsigned nbytes) {
		_bscdQ = true;
		return UnpackSparsified(src, nbytes, MAX_bSCD_CH, CDSPL_bSCD,
			_bscd_nch, _bscd_evnum, _bscd_ch, _bscd_adc); }
	int UnpackSCDL1(char* src, unsigned nbytes) {                         //Added by David Angelaszek 9/2/2010
		_scdl1Q = true;                                                  //Added by David Angelaszek 9/2/2010
		return UnpackSparsified(src, nbytes, MAX_SCDL1_CH, CDSPL_SCDL1,  //Added by David Angelaszek 9/2/2010
			_scdl1_nch, _scdl1_evnum, _scdl1_ch, _scdl1_adc); }      //Added by David Angelaszek 9/2/2010
	int UnpackSCDL2(char* src, unsigned nbytes) {                       //Added by David Angelaszek 9/2/2010
		_scdl2Q = true;                                                  //Added by David Angelaszek 9/2/2010
		return UnpackSparsified(src, nbytes, MAX_SCDL2_CH, CDSPL_SCDL2,  //Added by David Angelaszek 9/2/2010
			_scdl2_nch, _scdl2_evnum, _scdl2_ch, _scdl2_adc); }         //Added by David Angelaszek 9/2/2010
	int UnpackSCDL3(char* src, unsigned nbytes) {                         //Added by David Angelaszek 9/2/2010
		_scdl3Q = true;                                                  //Added by David Angelaszek 9/2/2010
		return UnpackSparsified(src, nbytes, MAX_SCDL3_CH, CDSPL_SCDL3,  //Added by David Angelaszek 9/2/2010
			_scdl3_nch, _scdl3_evnum, _scdl3_ch, _scdl3_adc); }      //Added by David Angelaszek 9/2/2010
	int UnpackTCD(char* src, unsigned nbytes);
	int UnpackBSD(char* src, unsigned nbytes);
	int UnpackCVD(char* src, unsigned nbytes);
	int UnpackCER(char* src, unsigned nbytes);
	int UnpackSiliconBeamTracker(char* src, unsigned nbytes) {
		_sbtQ = true;
		return UnpackSparsified(src, nbytes, MAX_SBT_CH, CDSPL_SBT,
			_sbt_nch, _sbt_evnum, _sbt_ch, _sbt_adc); }
//	int UnpackTRD(char* src, unsigned nbytes); trd selee
	int UnpackTRD(char* src, unsigned nbytes) {
		_trdQ = true;
		return UnpackSparsified(src, nbytes, MAX_TRD_CH, CDSPL_TRD,
			_trd_nch, _trd_evnum, _trd_ch, _trd_adc); }
	int UnpackTimeStamp(unsigned *, unsigned ts_len);
	inline void SetSWEventNumber(unsigned evno) { _swevnoQ = true; _swevno = evno; }
	inline void SetChecksum(unsigned chk) { _chksumQ = true; _chksum = chk; }
	inline void SetType(unsigned t) { _type = t; }
	inline void SetByteLength(unsigned n) { _raw_nbytes = n; }
	inline unsigned GetType() { return _type; }
	inline unsigned GetByteLength() { return _raw_nbytes; }

	// trigger
	inline unsigned GetTrigger() { return _trigger; }
	inline bool TriggerQ() { return ( _trigger & EVT_TRG_TRG == 0 ) ? true : false; }
	inline bool CalibrationTriggerQ() { return ( _trigger & EVT_TRG_CLB ) ? true : false; }
	inline bool ExternalTriggerQ() { return ( _trigger & EVT_TRG_EXT ) ? true : false; }
	inline bool CalorimeterTriggerQ() { return ( _trigger & EVT_TRG_CAL ) ? true : false; }
	inline bool CDTRG1TriggerQ() { return ( _trigger & EVT_TRG_CDTRG1 ) ? true : false; } //YONAS
	inline bool CDTRG2TriggerQ() { return ( _trigger & EVT_TRG_CDTRG2 ) ? true : false; } //YONAS
	inline bool ZHiTriggerQ() { return ( _trigger & EVT_TRG_ZHI ) ? true : false; }
	inline bool ZLowTriggerQ() { return ( _trigger & EVT_TRG_ZLO ) ? true : false; }
	inline bool ZCalibrationTriggerQ() { return ( _trigger & EVT_TRG_ZCLB ) ? true : false; }
	inline bool EHiTriggerQ() { return ( _trigger & EVT_TRG_EHI ) ? true : false; }
	inline bool ELowTriggerQ() { return ( _trigger & EVT_TRG_ELO ) ? true : false; }

	// CAMAC
	inline bool CAMACQ() { return _camacQ; }
	inline unsigned CAMACGetNumberOfChannel() { return _camac_nch; }
	inline unsigned CAMACGetChannel(unsigned k) { return k; }
	inline unsigned CAMACGetADC(unsigned k) { return _camac_adc[k]; }
	void CAMACGetBeamPosition(double& x, double& y);

	// Calorimeter
	inline bool CalorimeterQ() { return _calQ; }
	inline unsigned CalorimeterGetNumberOfChannel() { return _cal_nch; }
	inline unsigned CalorimeterGetChannel(unsigned k) { return _cal_ch[k]; }
	inline unsigned CalorimeterGetADC(unsigned k) { return _cal_adc[k]; }
	// TCRD
	inline bool TCRDQ() { return _tcrdQ; }
	inline unsigned TCRDGetNumberOfChannel() { return _tcrd_nch; }
	inline unsigned TCRDGetChannel(unsigned k) { return _tcrd_ch[k]; }
	inline unsigned TCRDGetADC(unsigned k) { return _tcrd_adc[k]; } //YONAS

	// Hodoscope
	inline bool HodoscopeQ() { return _hdsQ; }
	inline unsigned HodoscopeGetNumberOfChannel() { return _hds_nch; }
	inline unsigned HodoscopeGetChannel(unsigned k) { return _hds_ch[k]; }
	inline unsigned HodoscopeGetADC(unsigned k) { return _hds_adc[k]; }

	// SCD
	inline bool SCDQ() { return _scdQ; }
	inline unsigned SCDGetNumberOfChannel() { return _scd_nch; }
	inline unsigned SCDGetChannel(unsigned k) { return _scd_ch[k]; }
	inline unsigned SCDGetADC(unsigned k) { return _scd_adc[k]; }

	// tSCD
	inline bool TopSCDQ() { return _tscdQ; }
	inline unsigned TopSCDGetNumberOfChannel() { return _tscd_nch; }
	inline unsigned TopSCDGetChannel(unsigned k) { return _tscd_ch[k]; }
	inline unsigned TopSCDGetADC(unsigned k) { return _tscd_adc[k]; }

	// bSCD
	inline bool BottomSCDQ() { return _bscdQ; }
	inline unsigned BottomSCDGetNumberOfChannel() { return _bscd_nch; }
	inline unsigned BottomSCDGetChannel(unsigned k) { return _bscd_ch[k]; }
	inline unsigned BottomSCDGetADC(unsigned k) { return _bscd_adc[k]; }

	 //SCDL1
	inline bool SCDL1Q() { return _scdl1Q; }                              //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL1GetNumberOfChannel() { return _scdl1_nch; }        //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL1GetChannel(unsigned k) { return _scdl1_ch[k]; }   //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL1GetADC(unsigned k) { return _scdl1_adc[k]; }    //Added by David Angelaszek 9/2/2010

	// scdl2
	inline bool SCDL2Q() { return _scdl2Q; }                          //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL2GetNumberOfChannel() { return _scdl2_nch; }   //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL2GetChannel(unsigned k) { return _scdl2_ch[k]; }  //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL2GetADC(unsigned k) { return _scdl2_adc[k]; }    //Added by David Angelaszek 9/2/2010

	// SCDL3
	inline bool SCDL3Q() { return _scdl3Q; }                              //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL3GetNumberOfChannel() { return _scdl3_nch; }        //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL3GetChannel(unsigned k) { return _scdl3_ch[k]; }   //Added by David Angelaszek 9/2/2010
	inline unsigned SCDL3GetADC(unsigned k) { return _scdl3_adc[k]; }    //Added by David Angelaszek 9/2/2010


	// Silicon Beam Tracker
	inline bool SiliconBeamTrackerQ() { return _sbtQ; }
	inline unsigned SiliconBeamTrackerGetNumberOfChannel() { return _sbt_nch; }
	inline unsigned SiliconBeamTrackerGetChannel(unsigned k) { return _sbt_ch[k]; }
	inline unsigned SiliconBeamTrackerGetADC(unsigned k) { return _sbt_adc[k]; }

	// TCD
	inline bool TCDQ() { return _tcdQ; }
	inline unsigned TCDGetNumberOfChannel() { return _tcd_nch; }
	inline unsigned TCDGetChannel(unsigned k) { return _tcd_ch[k]; }
	inline unsigned TCDGetADC(unsigned k) { return _tcd_adc[k]; }

	// BSD
	inline bool BSDQ() { return _bsdQ; }
	inline unsigned BSDGetNumberOfChannel() { return _bsd_nch; }
	inline unsigned BSDGetChannel(unsigned k) { return _bsd_ch[k]; }
	inline unsigned BSDGetADC(unsigned k) { return _bsd_adc[k]; }
	inline unsigned BSDGetEventNum() { return _bsd_evnum; }
        inline unsigned BSDGetSyncNum() { return _bsd_syncnum; }
	inline unsigned BSDGetEventCnt() { return _bsd_evcnt; }
	inline unsigned BSDGetTriMode() { return _bsd_tri_mode; }
	inline unsigned BSDGetConfMode() { return _bsd_conf_mode; }
        inline unsigned BSDGetTriStatus() { return _bsd_tri_status; };
	inline unsigned BSDGetRaw(unsigned char *k){
		memcpy(k,_bsd_raw, _bsd_raw_nbytes);
		return _bsd_raw_nbytes;
	}

	// CVD
	inline bool CVDQ() { return _cvdQ; }
	inline unsigned CVDGetNumberOfChannel() { return _cvd_nch; }
	inline unsigned CVDGetChannel(unsigned k) { return _cvd_ch[k]; }
	inline unsigned CVDGetADC(unsigned k) { return _cvd_adc[k]; }

	// Cerenkov Camera
	inline bool CERQ() { return _cerQ; }
	inline unsigned CERGetNumberOfChannel() { return _cer_nch; }
	inline unsigned CERGetChannel(unsigned k) { return _cer_ch[k]; }
	inline unsigned CERGetADC(unsigned k) { return _cer_adc[k]; }
	inline unsigned CERGetGain(unsigned k) { return _cer_gain[k]; }

	// TRD
	inline bool TRDQ() { return _trdQ; }
	inline unsigned TrdGetNumberOfChannel() { return _trd_nch; }
	inline unsigned TrdGetChannel(unsigned k) { return _trd_ch[k]; }
	inline unsigned TrdGetADC(unsigned k) { return _trd_adc[k]; }
	/********************************Dr.L***************************
	inline bool TRDQ() { return _trdQ; }
	inline unsigned TRDPHAGetNumberOfChannel() { return _trd_pha_nch; }
	inline unsigned TRDPHAGetChannel(unsigned k) { return k; }
	inline unsigned TRDPHAGetADC(unsigned k) { return _trd_pha_adc[k]; }
	inline unsigned TRDAmplexGetNumberOfChannel() { return _trd_amplex_nch; }
	inline unsigned TRDAmplexGetChannel(unsigned k) { return _trd_amplex_ch[k]; }
	inline unsigned TRDAmplexGetADC(unsigned k) { return _trd_amplex_adc[k]; }
	********************************************/

	// Misc.
	int GetEventNumber(unsigned* evnum);
	inline unsigned GetSWEventNumber() { return _swevno; }
	int GetTimeStamp(unsigned* ts);

	// Printing
	void Print(FILE* fp, int opt = 0);
	void PrintHeader(FILE* fp, int opt = 0);
	void PrintTimeStamp(FILE* fp, int opt = 0);
	void PrintTrigger(FILE* fp, int opt = 0);
	void PrintCAMAC(FILE* fp, int opt = 0);
	void PrintCalorimeter(FILE* fp, int opt = 0);
	void PrintTCRD(FILE* fp, int opt = 0); //YONAS
	void PrintHodoscope(FILE* fp, int opt = 0);
	void PrintSCD(FILE* fp, int opt = 0);
	void PrintTopSCD(FILE* fp, int opt = 0);
	void PrintBottomSCD(FILE* fp, int opt = 0);
	void PrintTCD(FILE* fp, int opt = 0);
	void PrintTCDOriginal(FILE* fp, int opt = 0);
	void PrintBSD(FILE* fp, int opt = 0);
	void PrintBSDOriginal(FILE* fp, int opt = 0);
	void PrintTRD(FILE* fp, int opt = 0);
	void PrintTRDOriginal(FILE* fp, int opt = 0);
	void PrintCVD(FILE* fp, int opt = 0);
	void PrintCVDOriginal(FILE* fp, int opt = 0);
	void PrintEventNumber(FILE* fp, int opt = 0);
	void PrintCER(FILE* fp, int opt = 0);
	void PrintRawCER(FILE* fp, int opt = 0);
	void PrintSiliconBeamTracker(FILE* fp, int opt = 0);
	void PrintSCDL1(FILE* fp, int opt = 0);  //Added by David Angelaszek 9/2/2010
	void PrintSCDL2(FILE* fp, int opt = 0);  //Added by David Angelaszek 9/2/2010
	void PrintSCDL3(FILE* fp, int opt = 0);  //Added by David Angelaszek 9/2/2010
};

#endif
