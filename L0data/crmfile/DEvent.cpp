//
// DEvent.cpp
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
// $Log: DEvent.cpp,v $
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
// Revision 1.21  2007/06/22 14:49:38  pbhoyar
// modified formula to calculate channel number
//
// Revision 1.20  2007/02/21 00:29:26  juneki
// added CER pedestal and led runs
//
// Revision 1.19  2006/08/09 19:03:04  syzinn
// SBT data format changed
//
// Revision 1.18  2006/05/02 21:01:29  syzinn
// bug fix while unpacking SBT data
//
// Revision 1.17  2006/05/02 19:53:22  syzinn
// changes for USB SBT data format
//
// Revision 1.16  2006/05/01 19:39:24  syzinn
// support for silicon beam tracker
//
// Revision 1.15  2006/03/24 14:42:38  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.14.2.2  2006/03/14 22:46:42  syzinn
// minor changes in printing
//
// Revision 1.14.2.1  2006/03/14 21:07:42  syzinn
// eight-byte timestamp
//
// Revision 1.14  2005/12/17 07:35:12  syzinn
// GetByteLength() added
//
// Revision 1.13  2005/12/06 05:03:44  syzinn
// PrintEventNumber() includes the software, tSCD, bSCD, and CVD event numbers
//
// Revision 1.12  2005/11/08 18:24:29  cvs
// fixed a compiler error
//
// Revision 1.11  2005/11/08 14:48:15  syzinn
// continuous channel number for Cerenkov camera
//
// Revision 1.10  2005/11/07 22:51:38  cvs
// bug fix in UnpackCER()
//
// Revision 1.9  2005/11/07 21:25:32  cvs
// supports Cerenkov Camera data
//
// Revision 1.8  2005/07/31 17:27:55  cvs
// support CVD data section
//
// Revision 1.7  2005/07/18 05:51:21  cvs
// revised trigger printout
//
// Revision 1.6  2005/07/13 18:05:45  cvs
// prints eight channels per row
//
// Revision 1.5  2005/06/10 18:47:11  cvs
// prints sw event number and checksum
//
// Revision 1.4  2005/04/08 16:33:48  cvs
// supports format 3.1
//
// Revision 1.3  2005/04/07 22:21:43  cvs
// bug fix
//
// Revision 1.2  2005/02/28 02:10:17  daq
// changed to format 3.0
//
// Revision 1.1.1.1  2005/02/04 05:02:17  daq
// initial version for CREAM2.
//
// Revision 1.18  2005/01/27 17:45:57  syzinn
// no masking of trigger when printing
//
// Revision 1.17  2005/01/17 19:19:28  cvs
// bug fix
//
// Revision 1.16  2004/12/16 17:53:36  syzinn
// added printing Z-LOW trigger
//
// Revision 1.15  2004/08/27 21:39:08  syzinn
// TRD PHA format corrected
//
// Revision 1.14  2004/08/27 21:23:43  syzinn
// fixes TCD and TRD print outs
//
// Revision 1.13  2004/07/26 15:12:00  syzinn
// bug fix
//
// Revision 1.12  2004/07/16 20:03:57  syzinn
// _tcd_nitems[] now contains the number of items; bug fix
//
// Revision 1.11  2004/07/16 19:50:50  syzinn
// update by paolo
//
// Revision 1.3  2004/07/05 14:03:29  paolo
// *** empty log message ***
//
// Revision 1.9  2004/07/01 18:34:59  syzinn
// deciphers TCD data
//
// Revision 1.8  2004/06/21 13:21:42  syzinn
// provides functions to check the presence of each data section; event number printing.
//
// Revision 1.7  2004/04/22 13:36:22  syzinn
// Added PrintTCDOriginal()
//
// Revision 1.6  2004/04/22 13:16:23  syzinn
// Bug fix in extracting TRD data.
//
// Revision 1.5  2004/04/15 20:59:03  syzinn
// added copyright notice.
//
// Revision 1.4  2004/04/13 19:12:12  syzinn
// Skip printing when data are not available.
//
// Revision 1.3  2004/04/08 14:40:01  syzinn
// prints TRD data in their original format
//
// Revision 1.2  2004/03/06 15:53:06  syzinn
// Print all AMPLEX data.
//
// Revision 1.1.1.1  2004/02/24 21:46:25  syzinn
// crmfile library
//
//
#include <stdio.h>
#include <string.h>
#include "DEvent.h"
#include "fio.c"

#define MASK_HEXADECIMAL 0x00000008

#ifndef MIN
#define MIN(a,b) (((a)<(b)) ? (a):(b))
#endif

extern "C" unsigned short calc_crc16(unsigned char*, unsigned short);

void DEvent::Clear()
	{
	_triggerQ = _camacQ = _calQ = _hdsQ = _scdQ = _tcdQ = _bsdQ = _trdQ = _tcrdQ = false;  //YONAS
	_bscdQ = _tscdQ = false; 
	_scdl1Q = _scdl2Q = _scdl3Q = false;
	_cvdQ = _cerQ = _sbtQ = false;
	_swevnoQ = _chksumQ = false;
	_raw_nbytes = 0;

	_camac_nch = 0;

	_cal_nch = 0;
	_cal_evnum = 0;

	_tcrd_nch = 0; //YONAS
	_tcrd_evnum = 0; //YONAS

	
	_hds_nch = 0;
	_hds_evnum = 0;

	_scd_nch = 0;
	_scd_evnum = 0;

	_tscd_nch = 0;
	_tscd_evnum = 0;

	_bscd_nch = 0;
	_bscd_evnum = 0;

	_scdl1_nch = 0;   //Added by David Angelaszek 9/2/2010
	_scdl1_evnum = 0;  //Added by David Angelaszek 9/2/2010

	_scdl2_nch = 0;    //Added by David Angelaszek 9/2/2010
	_scdl2_evnum = 0;  //Added by David Angelaszek 9/2/2010
	
	_scdl3_nch = 0;    //Added by David Angelaszek 9/2/2010
	_scdl3_evnum = 0;  //Added by David Angelaszek 9/2/2010


	_tcd_nch = 0;
	_tcd_evnum = 0;
	_tcd_nbytes = _tcd_header = 0;
	_tcd_nitems[0] = _tcd_nitems[1] = _tcd_nitems[2] = _tcd_nitems[3] = _tcd_nitems[4] = 0;
	_tcd_nitems[5] = _tcd_nitems[6] = _tcd_nitems[7] = _tcd_nitems[8] = _tcd_nitems[9] = 0;
	_tcd_fmtcode = _tcd_bitmask = _tcd_bitcount = 0;

	_bsd_nch = 0;
	_bsd_evnum = 0;
	_bsd_nbytes=0;

	_cvd_nch = 0;
	_cvd_evnum = 0;

	_cer_nch = 0;
	_cer_evnum = 0;

	_sbt_nch = 0;
	_sbt_evnum = 0;

	//_trd_pha_nch = 0;
	//_trd_amplex_nch = 0;
	_trd_evnum = 0;

	_timestampQ = false;
}

int DEvent::UnpackTrigger(unsigned trig, unsigned trig_len)
	{
	if ( trig_len != 0 ) {
		_triggerQ = true;
		_trigger = trig;
	}

	return 0;
}

int DEvent::UnpackCAMAC(char* src, unsigned nbytes)
	{
	unsigned k;
	int* ibuf;

	// read channel number and adjust
	ibuf = (int *) src;
	_camac_nch = get_uint4( (unsigned char *) ibuf ); ibuf++;
	_camac_nch = MIN(MAX_CAMAC_CH, _camac_nch);

	// fill ADC array
	for(k = 0; k < _camac_nch; k++)
		_camac_adc[k] = ibuf[k];

	_camacQ = true;

	return 0;
}

int DEvent::UnpackSparsified(char* src, unsigned nbytes,
	unsigned max_channel, unsigned displ,
	unsigned& nchannel, unsigned& evnum,
	unsigned short* pch, unsigned short* padc)
	{
	unsigned short *sbuf;
	unsigned short channel;  // channel number from raw data
	unsigned short adc; // ADC value from raw data
	unsigned k, i;
	
	// read channel number plus two
	sbuf = (unsigned short*) src;
	nchannel = get_uint2( (unsigned char*) sbuf ); nchannel -= 2;
	sbuf++;
	
	// skip unused
	sbuf++;
       
	// event number
	evnum = sbuf[0] + (sbuf[1] << 16);
	sbuf += 2;

	// fill ADC array
	i = 0;
	for(k = 0; k < nchannel; k++) {
		adc = *sbuf++;
		channel = *sbuf++ - displ;
		if ( channel < max_channel ) {
			pch[i] = channel;
			padc[i] = adc;
			i++;
		}
	}

	// adjust channel count.
	// do not place this before the loop for filling above.
	nchannel = MIN(max_channel, nchannel);

	return 0;
}

int DEvent::UnpackTCD(char* src, unsigned nbytes)
	{
	unsigned short* sbuf;
	unsigned k, kk;
	unsigned nleft = nbytes;

	// copy to raw buffer
	if ( nbytes <= TCD_BUFSIZ ) {
		memcpy(_tcd_raw, src, nbytes);
		_tcd_raw_nbytes = nbytes;
	}
	else
		_tcd_raw_nbytes = 0;

	// number of bytes
	sbuf = (unsigned short *) src;
	_tcd_nbytes = *sbuf++;

	// event number
	_tcd_evnum = sbuf[0] + (sbuf[1] << 16);
	sbuf += 2;

	// header
	_tcd_header = *sbuf++;
	_tcd_nitems[0] = ( (_tcd_header >> 12) & 0x0f) + 1;
	_tcd_fmtcode = (_tcd_header >> 9) & 0x07;
	_tcd_bitmask = _tcd_header & 0x1ff;
	_tcd_bitcount = 0;
	for(k = 0; k < 9; k++)
		if ( _tcd_bitmask & (1L << k) ) _tcd_bitcount++;
	nleft -= 8; // so far four half-words.

	if ( _tcd_bitcount > 1 ) {
		_tcd_halfword4 = *sbuf;
		_tcd_nitems[4] = ((*sbuf >> 12) & 0x0f) + 1;
		_tcd_nitems[3] = ((*sbuf >> 8) & 0x0f) + 1;
		_tcd_nitems[2] = ((*sbuf >> 4) & 0x0f) + 1;
		_tcd_nitems[1] = (*sbuf & 0x0f) + 1;
		sbuf++;
		nleft -= 2;

		if ( _tcd_bitcount > 5 ) {
			_tcd_halfword5 = *sbuf;
			_tcd_nitems[8] = ((*sbuf >> 12) & 0x0f) + 1;
			_tcd_nitems[7] = ((*sbuf >> 8) & 0x0f) + 1;
			_tcd_nitems[6] = ((*sbuf >> 4) & 0x0f) + 1;
			_tcd_nitems[5] = (*sbuf & 0x0f) + 1;
			sbuf++;
			nleft -= 2;
		}
	}

	// channel number
	_tcd_nch = (nleft >> 1) - 1; // last one is the CRC
	_tcd_nch = MIN(MAX_TCD_CH, _tcd_nch);

	k = 0;
	kk = 0;
	for(int i=0; i<9; i++) //loop on concentrators
	{
		if ( (_tcd_bitmask & (1L << i)) ) {
			for(unsigned j=0; j<_tcd_nitems[kk]; j++) //loop on items
			{
				_tcd_ch[k]  =  (*sbuf>>12) + i*16;
				_tcd_adc[k] =  *sbuf & 0x0fff;
				sbuf++;
				k++;
			}
			kk++;
		}
	}

	_tcd_crc16 = *sbuf;
//	_tcd_calc16 = calc_crc16((unsigned char*) src, nbytes - 2);
	_tcdQ = true;

	return 0;
}

#ifdef OLD_METHOD
/*
 * Copy from bsd BSD-event.h
 */
#define BSD_NUM_RAW_BYTES 84 // number of raw bytes from USB
#define NUM_LATE 16
#define NUM_EARLY 2
#define OFFSET_LOW_GAIN 0
#define OFFSET_HIGH_GAIN (OFFSET_LOW_GAIN + NUM_LATE)
#define OFFSET_TRG_LOW_GAIN     (OFFSET_HIGH_GAIN + NUM_LATE)
#define OFFSET_TRG_HIGH_GAIN (OFFSET_TRG_LOW_GAIN + NUM_EARLY)
#endif

int DEvent::UnpackBSD(char* src, unsigned nbytes)
{
	unsigned char *sp;
	int i;

	sp = (unsigned char *)src;
#ifndef N05282014
//      last modified for struct change ; 07012014
//
//
        _bsd_syncnum =(0xFF000000 & ( *sp++ << 24)) ;
        _bsd_syncnum |= (0x00FF0000 & (*sp++ << 16) ) ;
        _bsd_syncnum |= (0x0000FF00 & ( *sp++ << 8));
        _bsd_syncnum |= (0x000000FF & *sp++);
        _bsd_evnum =(0xFF000000 & ( *sp++ << 24)) ;
        _bsd_evnum |= (0x00FF0000 & (*sp++ << 16) ) ;
        _bsd_evnum |= (0x0000FF00 & ( *sp++ << 8));
        _bsd_evnum |= (0x000000FF & *sp++);
        _bsd_evcnt = *sp++ & 0xff ;
        _bsd_tri_mode = *sp++  & 0xff ;
        _bsd_conf_mode = *sp++ & 0xff ;
        _bsd_tri_status = *sp++ & 0xff ;


        for( i = 0; i < ( NUM_LATE * 2 + NUM_EARLY * 2 ); i++ )
        {
                _bsd_adc[i]  = (0xFF00 & ( *sp++ << 8));
                _bsd_adc[i] |= (0x00FF & *sp++);
                _bsd_ch[i] =  i;
        }

#else
	sp += 4;       //skip for header SYNC(0x8f c7 dc dc )4
					// EVTNO 4
					// EVT COUNT 1
				//STATUS 1
	_bsd_evnum =
			(0xFF000000 & ( *sp++ << 24)) | (0x00FF0000 & (*sp++ << 16) ) |
			(0x0000FF00 & ( *sp++ << 8)) | (0x000000FF & *sp++);

	for( i = 0; i < NUM_LATE; i++ )
	{
		_bsd_adc[OFFSET_LOW_GAIN + i]= (0xFF00 & ( *sp++ << 8)) | (0x00FF & *sp++);
		_bsd_ch[OFFSET_LOW_GAIN + i] = OFFSET_LOW_GAIN + i;
		_bsd_adc[OFFSET_HIGH_GAIN + i]= (0xFF00 & ( *sp++ << 8)) | (0x00FF & *sp++);
		_bsd_ch[OFFSET_HIGH_GAIN + i] = OFFSET_HIGH_GAIN + i;
	}

	for( i = 0; i < NUM_EARLY; i++ )
	{
		_bsd_adc[OFFSET_TRG_LOW_GAIN + i]= (0xFF00 & ( *sp++ << 8)) | (0x00FF & *sp++);
		_bsd_ch[OFFSET_TRG_LOW_GAIN + i] = OFFSET_TRG_LOW_GAIN + i;
		_bsd_adc[OFFSET_TRG_HIGH_GAIN + i ]= (0xFF00 & ( *sp++ << 8)) | (0x00FF & *sp++);
		_bsd_ch[OFFSET_TRG_HIGH_GAIN + i] = OFFSET_TRG_HIGH_GAIN + i;
	}
#endif
	_bsd_nch = NUM_LATE * 2 + NUM_EARLY * 2;

	_bsdQ = true;
	return 0;
}

int DEvent::UnpackCVD(char* src, unsigned nbytes)
	{
	unsigned short* sbuf;
	unsigned k, kk;
	unsigned nleft = nbytes;

	// copy to raw buffer
	if ( nbytes <= CVD_BUFSIZ ) {
		memcpy(_cvd_raw, src, nbytes);
		_cvd_raw_nbytes = nbytes;
	}
	else
		_cvd_raw_nbytes = 0;

	// number of bytes
	sbuf = (unsigned short *) src;
	_cvd_nbytes = *sbuf++;

	// event number
	_cvd_evnum = sbuf[0] + (sbuf[1] << 16);
	sbuf += 2;

	// header
	_cvd_header = *sbuf++;
	_cvd_nitems[0] = ( (_cvd_header >> 12) & 0x0f) + 1;
	_cvd_fmtcode = (_cvd_header >> 9) & 0x07;
	_cvd_bitmask = _cvd_header & 0x1ff;
	_cvd_bitcount = 0;
	for(k = 0; k < 9; k++)
		if ( _cvd_bitmask & (1L << k) ) _cvd_bitcount++;
	nleft -= 8; // so far four half-words.

	if ( _cvd_bitcount > 1 ) {
		_cvd_halfword4 = *sbuf;
		_cvd_nitems[4] = ((*sbuf >> 12) & 0x0f) + 1;
		_cvd_nitems[3] = ((*sbuf >> 8) & 0x0f) + 1;
		_cvd_nitems[2] = ((*sbuf >> 4) & 0x0f) + 1;
		_cvd_nitems[1] = (*sbuf & 0x0f) + 1;
		sbuf++;
		nleft -= 2;

		if ( _cvd_bitcount > 5 ) {
			_cvd_halfword5 = *sbuf;
			_cvd_nitems[8] = ((*sbuf >> 12) & 0x0f) + 1;
			_cvd_nitems[7] = ((*sbuf >> 8) & 0x0f) + 1;
			_cvd_nitems[6] = ((*sbuf >> 4) & 0x0f) + 1;
			_cvd_nitems[5] = (*sbuf & 0x0f) + 1;
			sbuf++;
			nleft -= 2;
		}
	}

	// channel number
	_cvd_nch = (nleft >> 1) - 1; // last one is the CRC
	_cvd_nch = MIN(MAX_CVD_CH, _cvd_nch);

	k = 0;
	kk = 0;
	for(int i=0; i<9; i++) //loop on concentrators
	{
		if ( (_cvd_bitmask & (1L << i)) ) {
			for(unsigned j=0; j<_cvd_nitems[kk]; j++) //loop on items
			{
				_cvd_ch[k]  =  (*sbuf>>12) + i*16;
				_cvd_adc[k] =  *sbuf & 0x0fff;
				sbuf++;
				k++;
			}
			kk++;
		}
	}

	_cvd_crc16 = *sbuf;
//	_cvd_calc16 = calc_crc16((unsigned char*) src, nbytes - 2);
	_cvdQ = true;

	return 0;
}

int DEvent::UnpackCER(char* src, unsigned nbytes)
	{
	unsigned short *sbuf = (unsigned short *) src;
	int ch, adc, ich, gain;

	// copy to raw buffer
	if ( nbytes <= CER_BUFSIZ ) {
		memcpy(_cer_raw, src, nbytes);
		_cer_raw_nbytes = nbytes;
	}
	else
		_cer_raw_nbytes = 0;

	// six bytes of header
	_cer_evnum = sbuf[1] + (sbuf[0] << 16L);
	sbuf += 3;

	// channel number
	_cer_nch = sbuf[0] - 1; 
	sbuf++;

        unsigned int npm = 0;
        for(unsigned k = 0; k < _cer_nch; k++)
        {
                //ch = ( posflex*10 + flexnum)*16 + pospmt;
	//New formula to caculate channel number
                ch = ( ((sbuf[0] >> 4L) & 0x0F) * 10 + (sbuf[0] & 0x0F) ) * 16
                        + (sbuf[1] >> 12L);
                adc = sbuf[1] & 0x0FFF;
                gain = ((sbuf[0] >> 8L) & 0x0F);
                ich = ch - CDSPL_CER;

                if ( adc!=0 && ich < MAX_CER_CH && ich >= 0 ) {
                        _cer_ch[npm] = ich;
                        _cer_adc[npm] = adc;
                        _cer_gain[npm] = gain;
                        npm++;
                }
                sbuf += 2;
        }
        _cer_nch= npm;
        _cerQ = true;

        return 0;
}

/*int DEvent::UnpackTRD(char* src, unsigned nbytes)
	{
	unsigned short* sbuf;
	unsigned ntotal;
	unsigned k;

	// copy to raw buffer
	if ( nbytes <= TRD_BUFSIZ ) {
		memcpy(_trd_raw, src, nbytes);
		_trd_raw_nbytes = nbytes;
	}
	else
		_trd_raw_nbytes = 0;

	// skip number of bytes
	sbuf = (unsigned short *) src;
	sbuf++;

	// event number
	_trd_evnum = sbuf[0] + (sbuf[1] << 16);
	sbuf += 2;

	// channel number
	ntotal = (nbytes >> 1) - 3;
	_trd_pha_nch = 14; // fixed; what remains after excluding sync patterns
	_trd_pha_nch = MIN(MAX_TRDPHA_CH, _trd_pha_nch);
	_trd_amplex_nch = ntotal - _trd_pha_nch - 6;
	int amplex_count = ntotal - _trd_pha_nch;

	// fill PHA
	_trd_pha_adc[0]  = ((sbuf[0] << 4) & 0xff0) + (sbuf[ 7] & 0x00f);
	_trd_pha_adc[1]  = ((sbuf[1] << 4) & 0xff0) + (sbuf[ 8] & 0x00f);
	_trd_pha_adc[2]  = ((sbuf[2] << 4) & 0xff0) + (sbuf[ 9] & 0x00f);
	_trd_pha_adc[3]  = ((sbuf[3] << 4) & 0xff0) + (sbuf[10] & 0x00f);
	_trd_pha_adc[4]  = ((sbuf[4] << 4) & 0xff0) + (sbuf[11] & 0x00f);
	_trd_pha_adc[5]  = ((sbuf[5] << 4) & 0xff0) + (sbuf[12] & 0x00f);
	_trd_pha_adc[6]  = ((sbuf[6] << 4) & 0xff0) + (sbuf[13] & 0x00f);

	_trd_pha_adc[7]  = ((sbuf[0] >> 4) & 0xff0) + ((sbuf[ 7] >> 4) & 0x00f);
	_trd_pha_adc[8]  = ((sbuf[1] >> 4) & 0xff0) + ((sbuf[ 8] >> 4) & 0x00f);
	_trd_pha_adc[9]  = ((sbuf[2] >> 4) & 0xff0) + ((sbuf[ 9] >> 4) & 0x00f);
	_trd_pha_adc[10] = ((sbuf[3] >> 4) & 0xff0) + ((sbuf[10] >> 4) & 0x00f);
	_trd_pha_adc[11] = ((sbuf[4] >> 4) & 0xff0) + ((sbuf[11] >> 4) & 0x00f);
	_trd_pha_adc[12] = ((sbuf[5] >> 4) & 0xff0) + ((sbuf[12] >> 4) & 0x00f);
	_trd_pha_adc[13] = ((sbuf[6] >> 4) & 0xff0) + ((sbuf[13] >> 4) & 0x00f);

	sbuf += _trd_pha_nch;
	
	//find frame length
	unsigned int synch=0;

	for (int i=0; i<amplex_count; i++)
	  {
	    if ( (sbuf[i]&0xf000) == 0xf000 ) 
	      {
		synch =i;
		break; 
	      }
	  }
	
	unsigned int frame = (sbuf[synch]&0x0fff); 
	unsigned int fifohit[32]={0};
	
	// find hit fifos
	if (frame == synch) 
	  {
	    for (int i=0; i<16; i++)
	      {   
		if ((( sbuf[synch+1] >> i) &0x1) == 0x1)
		  fifohit[i]=1;
		else
		  fifohit[i]=0;
		
		//printf("%d %d\n",i,fifohit[i]);
	      }  
	  
	    for (int i=0; i<16; i++)
	      {               
		if (((sbuf[synch+2] >> i)&0x1) == 0x1)
		  fifohit[i+16]=1;
		else
		  fifohit[i+16]=0;

		//printf("%d %d\n",i,fifohit[i]);
	      }
	  }

	//fill adc
	unsigned int sflag=0, fpoint=0;
	unsigned int waddr, wamp;
	
	k=0;
	for (int i=frame; i>0; i--)
	  {
	    if ((sbuf[synch-i]&0x8000) == 0x8000) //it's first in fifo
	      {   
		if (sflag==0) // first time thorugh -check for fpoint=0
		  sflag=1;
	        else 
		  fpoint++;   // further times - bump-up pointer

		while(fifohit[fpoint]==0)
		  {
		    fpoint++;
		    if (fpoint >= 31)
		      {
			fpoint=31;
			break;
		      }
		  }  // fpoint finds next fifo hit
	      }
	    
	    waddr=((sbuf[synch-i] >> 8)&0x3f);
	    wamp=(sbuf[synch-i]&0xff);
	    //printf("%d %d %d\n",k,waddr, wamp);
 
	    if ( k < 1024) 
	      {
		_trd_amplex_ch[k]   = waddr-1 + 32*fpoint;
		_trd_amplex_adc[k]  = wamp;
		k++;
	      }
	    else
	      {
		printf("fwire exceeds bounds at %i \n", k);
		break;
	      }
	  }
	
	_trd_amplex_nch = frame;
	_trdQ = true;

	return 0;
}*/

int DEvent::UnpackTimeStamp(unsigned* ts, unsigned ts_len)
	{
	if ( ts_len != 0 ) {
		for(unsigned k = 0; k < ts_len; k++)
			_timestamp[k] = ts[k];
		_timestampQ = true;
	}

	return 0;
}

/* return event numbers */
// TODO: extend this to top and bottom SCDs.
int DEvent::GetEventNumber(unsigned evnum[16])
	{
	// from trigger
	evnum[0] = 0;

	// from CAMAC interface
	evnum[1] = 0;

	// from CAL sparsification box
	evnum[2] = _cal_evnum;

	// from SCD
	evnum[3] = _scd_evnum;

	// from HODOSCOPE
	evnum[4] = _hds_evnum;

	// from TCD
	evnum[5] = _tcd_evnum;

	// from BSD
	evnum[5] = _bsd_evnum;

	// from TRD
	evnum[6] = _trd_evnum;

	// from tSCD           // Added by YS 2007.8.21
	evnum[7] = _tscd_evnum;

	// from bSCD           // Added by YS 2007.8.21
	evnum[8] = _bscd_evnum;

	 //from SCDL1             added by David Angelaszek 9/2/2010	
	evnum[9] = _scdl1_evnum;

	// from SCDL2             added by David Angelaszek 9/2/2010	
	evnum[10] = _scdl2_evnum;

	// from SCDL3             added by David Angelaszek 9/2/2010	
	evnum[11] = _scdl3_evnum;


	// from CVD            // Added by YS 2007.8.21
	evnum[12] = _cvd_evnum;

	// from CherCam        // Added by YS 2007.8.21
	evnum[13] = _cer_evnum;

	// from SBT            // Added by YS 2007.8.21
	evnum[14] = _sbt_evnum;

	// from TCRD sparsification box
	evnum[15] = _tcrd_evnum; //YONAS

	return 0;
}

int DEvent::GetTimeStamp(unsigned ts[6])
	{
	int k;
	for(k = 0; k < 6; k++)
		ts[k] = _timestamp[k];
	return 0;
}

void DEvent::Print(FILE* fp, int opt)
	{
	PrintHeader(fp, opt);

	if ( CAMACQ() )
		PrintCAMAC(fp, opt);
	if ( SiliconBeamTrackerQ() )
		PrintSiliconBeamTracker(fp, opt);
	if ( TCDQ() )
		PrintTCD(fp, opt);
	if ( BSDQ() )
		PrintBSD(fp, opt);
	if ( TRDQ() )
		PrintTRD(fp, opt);
	if ( CalorimeterQ() )
		PrintCalorimeter(fp, opt);
	if ( TCRDQ() ) //YONAS
		PrintTCRD(fp, opt);
	if ( HodoscopeQ() )
		PrintHodoscope(fp, opt);
	if ( SCDQ() )
		PrintSCD(fp, opt);
	if ( TopSCDQ() )
		PrintTopSCD(fp, opt);
	if ( BottomSCDQ() )
		PrintBottomSCD(fp, opt);
	if ( SCDL1Q() )                 //Added by David Angelaszek 9/2/2010
		PrintSCDL1(fp, opt);      //Added by David Angelaszek 9/2/2010
	if ( SCDL2Q() )               //Added by David Angelaszek 9/2/2010
		PrintSCDL2(fp, opt);  //Added by David Angelaszek 9/2/2010
	if ( SCDL3Q() )               //Added by David Angelaszek 9/2/2010
		PrintSCDL3(fp, opt);  //Added by David Angelaszek 9/2/2010
	if ( CVDQ() )
		PrintCVD(fp, opt);
	if ( CERQ() )
		PrintCER(fp, opt);
//		PrintRawCER(fp, opt);
}

void DEvent::PrintHeader(FILE* fp, int opt)
	{
	fprintf(fp, "[header]\n");

	// type
	if ( opt & MASK_HEXADECIMAL )
		fprintf(fp, "type %02X [", GetType());
	else
		fprintf(fp, "type %d [", GetType());
	switch ( GetType() ) {
	case DAT_EVT: fprintf(fp, "EVT"); break;
	case DAT_CPDS: fprintf(fp, "CPDS"); break;
	case DAT_TCRDPDS: fprintf(fp, "TCRDPDS"); break; //YONAS
	case DAT_HPDS: fprintf(fp, "HPDS"); break;
	case DAT_SPDS: fprintf(fp, "SPDS"); break;
	case DAT_SLPDS1: fprintf(fp, "SLPDS1"); break;
	case DAT_SLPDS2: fprintf(fp, "SLPDS2"); break;
	case DAT_SLPDS3: fprintf(fp, "SLPDS3"); break;
	case DAT_TPDS: fprintf(fp, "TPDS"); break;
	case DAT_tSPDS: fprintf(fp, "tSPDS"); break;
	case DAT_bSPDS: fprintf(fp, "bSPDS"); break;
	case DAT_BPDS: fprintf(fp, "BPDS"); break;
	case DAT_KPDS: fprintf(fp, "KPDS"); break;   // YS 2007.9.30
	case DAT_CZCL: fprintf(fp, "CZCL"); break;
	case DAT_TCRDZCL: fprintf(fp, "TCRDZCL"); break; //YONAS
	case DAT_HZCL: fprintf(fp, "HZCL"); break;
	case DAT_SZCL: fprintf(fp, "SZCL"); break;
	case DAT_tSZCL: fprintf(fp, "tSZCL"); break;
	case DAT_bSZCL: fprintf(fp, "bSZCL"); break;
	case DAT_SLZCL1: fprintf(fp, "SLZCL1"); break;
	case DAT_SLZCL2: fprintf(fp, "SLZCL2"); break;
	case DAT_SLZCL3: fprintf(fp, "SLZCL3"); break;
	case DAT_BZCL: fprintf(fp, "BZCL"); break;
	case DAT_CLED: fprintf(fp, "CLED"); break;
	case DAT_BSDLED: fprintf(fp, "BSDLED"); break;
	case DAT_TCRDLED: fprintf(fp, "TCRDLED"); break; //YONAS
	case DAT_HLED: fprintf(fp, "HLED"); break;
	case DAT_KLED: fprintf(fp, "KLED"); break;   // YS 2007.9.30
	default: fprintf(fp, "UNKNOWN"); break;
	}
	fprintf(fp, "]\n\n");
	fprintf(fp, "sw event number %d, chksum %04X\n", _swevno, _chksum);

	// others
	PrintTimeStamp(fp, opt);
	PrintTrigger(fp, opt);
}

void DEvent::PrintTimeStamp(FILE* fp, int mode)
	{
	if ( _timestampQ ) {
		fprintf(fp, "[time stamp]\n");

		fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d.%03d.%03d\n\n",
				_timestamp[0],
				_timestamp[1],
				_timestamp[2],
				_timestamp[3],
				_timestamp[4],
				_timestamp[5],
				_timestamp[6],
				_timestamp[7]
		);
	}
}

void DEvent::PrintTrigger(FILE* fp, int mode)
	{
	int k;

	fprintf(fp, "[trigger]\n");

	if ( !_triggerQ ) return; // no trigger info

	if ( mode & MASK_HEXADECIMAL )
		fprintf(fp, "%08X [", GetTrigger());
	else
		fprintf(fp, "%d [", GetTrigger());

	k = 0;
	if ( TriggerQ() ) { // zero is active
		fprintf(fp, "TRIGG"); k++;
	}
	if ( EHiTriggerQ() ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "E-HI"); k++;
	}
	if ( ZCalibrationTriggerQ() ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "ZCLB"); k++;
	}
	if ( ELowTriggerQ() ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "E-LOW"); k++;
	}
	if ( CalibrationTriggerQ() ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "CALIB"); k++;
	}
	if ( ExternalTriggerQ() ) {
		if ( k != 0 ) fprintf(fp, ", ");
		fprintf(fp, "EXTRN"); k++;
	}

	// flags
	if ( _trigger & EVT_TRG_S3HI ) {
		if ( k != 0 ) fprintf(fp, "; ");
		fprintf(fp, "S3HI"); k++;
	}
	if ( _trigger & EVT_TRG_S3LO ) {
		if ( k != 0 ) fprintf(fp, "; ");
		fprintf(fp, "S3LO"); k++;
	}
	if ( CalorimeterTriggerQ() ) { //YONAS
		if ( k != 0 ) fprintf(fp, "; ");
		fprintf(fp, "CALOR"); k++;
	}
	if ( CDTRG1TriggerQ() ) {
		if ( k != 0 ) fprintf(fp, "; ");
		fprintf(fp, "CDTRG1"); k++;
	}
	if ( CDTRG2TriggerQ() ) {
		if ( k != 0 ) fprintf(fp, "; ");
		fprintf(fp, "CDTRG2"); k++;
	}
	if ( ZHiTriggerQ() ) {
		if ( k != 0 ) fprintf(fp, "; ");
		fprintf(fp, "Z-HI"); k++;
	}
	if ( ZLowTriggerQ() ) {
		if ( k != 0 ) fprintf(fp, "; ");
		fprintf(fp, "Z-LOW"); k++;
	}

	fprintf(fp, "]\n\n");
}

void DEvent::PrintCAMAC(FILE* fp, int mode)
	{
	fprintf(fp, "[camac]\n");
	fprintf(fp, "channel count = %d\n", CAMACGetNumberOfChannel());

	for(unsigned k = 0; k < CAMACGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 7 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X", CAMACGetChannel(k), CAMACGetADC(k));
		else
			fprintf(fp, " %4d %4d", CAMACGetChannel(k), CAMACGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintCalorimeter(FILE* fp, int mode)
	{
	fprintf(fp, "[calorimeter]\n");
	fprintf(fp, "channel count = %d\n", CalorimeterGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _cal_evnum, _cal_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _cal_evnum, _cal_evnum);  // Long int output by YS

	for(unsigned k = 0; k < CalorimeterGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				CalorimeterGetChannel(k), CalorimeterGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				CalorimeterGetChannel(k), CalorimeterGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintTCRD(FILE* fp, int mode) //YONAS
	{
	fprintf(fp, "[tcrd]\n");
	fprintf(fp, "channel count = %d\n", TCRDGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _cal_evnum, _cal_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _tcrd_evnum, _tcrd_evnum);  // Long int output by YS

	for(unsigned k = 0; k < TCRDGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				TCRDGetChannel(k), TCRDGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				TCRDGetChannel(k), TCRDGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintHodoscope(FILE* fp, int mode)
	{
	fprintf(fp, "[hodoscope]\n");
	fprintf(fp, "channel count = %d\n", HodoscopeGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _hds_evnum, _hds_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _hds_evnum, _hds_evnum);  // Long int output by YS

	for(unsigned k = 0; k < HodoscopeGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				HodoscopeGetChannel(k), HodoscopeGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				HodoscopeGetChannel(k), HodoscopeGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintSCD(FILE* fp, int mode)
	{
	fprintf(fp, "[scd]\n");
	fprintf(fp, "channel count = %d\n", SCDGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _scd_evnum, _scd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _scd_evnum, _scd_evnum);  // Long int output by YS

	for(unsigned k = 0; k < SCDGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				SCDGetChannel(k), SCDGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				SCDGetChannel(k), SCDGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintTopSCD(FILE* fp, int mode)
	{
	fprintf(fp, "[scd1]\n");
	fprintf(fp, "channel count = %d\n", TopSCDGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _tscd_evnum, _tscd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _tscd_evnum, _tscd_evnum);  // Long int output by YS

	for(unsigned k = 0; k < TopSCDGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				TopSCDGetChannel(k), TopSCDGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				TopSCDGetChannel(k), TopSCDGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintBottomSCD(FILE* fp, int mode)
	{
	fprintf(fp, "[scd2]\n");
	fprintf(fp, "channel count = %d\n", BottomSCDGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _bscd_evnum, _bscd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _bscd_evnum, _bscd_evnum);  // Long int output by YS

	for(unsigned k = 0; k < BottomSCDGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				BottomSCDGetChannel(k), BottomSCDGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				BottomSCDGetChannel(k), BottomSCDGetADC(k));
	}
	fprintf(fp, "\n\n");
}


void DEvent::PrintSCDL1(FILE* fp, int mode)    //Function added by David Angelaszek 9/2/2010
	{
	fprintf(fp, "[scd3]\n");
	fprintf(fp, "channel count = %d\n", SCDL1GetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _tscd_evnum, _tscd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _scdl1_evnum, _scdl1_evnum);  

	for(unsigned k = 0; k < SCDL1GetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				SCDL1GetChannel(k), SCDL1GetADC(k));
		else
			fprintf(fp, " %4d %4d",
				SCDL1GetChannel(k), SCDL1GetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintSCDL2(FILE* fp, int mode)   //Function added by David Angelaszek 9/2/2010
	{
	fprintf(fp, "[scd4]\n");
	fprintf(fp, "channel count = %d\n", SCDL2GetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _bscd_evnum, _bscd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _scdl2_evnum, _scdl2_evnum); 

	for(unsigned k = 0; k < SCDL2GetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				SCDL2GetChannel(k), SCDL2GetADC(k));
		else
			fprintf(fp, " %4d %4d",
				SCDL2GetChannel(k), SCDL2GetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintSCDL3(FILE* fp, int mode)   //Function added by David Angelaszek 9/2/2010
	{
	fprintf(fp, "[scdl3]\n");
	fprintf(fp, "channel count = %d\n", SCDL3GetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _bscd_evnum, _bscd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _scdl3_evnum, _scdl3_evnum); 

	for(unsigned k = 0; k < SCDL3GetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				SCDL3GetChannel(k), SCDL3GetADC(k));
		else
			fprintf(fp, " %4d %4d",
				SCDL3GetChannel(k), SCDL3GetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintSiliconBeamTracker(FILE* fp, int mode)
	{
	fprintf(fp, "[sbt]\n");
	fprintf(fp, "channel count = %d\n", SiliconBeamTrackerGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _sbt_evnum, _sbt_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _sbt_evnum, _sbt_evnum);  // Long int output by YS

	for(unsigned k = 0; k < SiliconBeamTrackerGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				SiliconBeamTrackerGetChannel(k), SiliconBeamTrackerGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				SiliconBeamTrackerGetChannel(k), SiliconBeamTrackerGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintRawCER(FILE* fp, int mode)
	{
	unsigned k;
	fprintf(fp, "[cerenkov camera]\n");

	for(k = 0; k < _cer_raw_nbytes; k++) {
		if ( k != 0 && k % 16 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %02X", _cer_raw[k]);
		else
			fprintf(fp, " %03d", _cer_raw[k]);
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintCER(FILE* fp, int mode)
	{
	unsigned k;

	fprintf(fp, "[cerenkov camera]\n");
	fprintf(fp, "channel count = %d\n", CERGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _cer_evnum, _cer_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _cer_evnum, _cer_evnum);  // Long int output by YS

	for(k = 0; k < CERGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				CERGetChannel(k), CERGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				CERGetChannel(k), CERGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintTCD(FILE* fp, int mode)
	{
	unsigned k;

	fprintf(fp, "[tcd]\n");
	fprintf(fp, "byte count = %d (%04X)\n", _tcd_nbytes, _tcd_nbytes);
//	fprintf(fp, "hw event number = %d (%08X)\n", _tcd_evnum, _tcd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _tcd_evnum, _tcd_evnum);  // Long int output by YS
	fprintf(fp, "header = %04X [fmt %d, mask %03X, bit count %d]\n", _tcd_header, _tcd_fmtcode, _tcd_bitmask, _tcd_bitcount);
	fprintf(fp, "items = ");
	for(k = 0; k < _tcd_bitcount; k++)
		fprintf(fp, "%s%d", (k == 0) ? "" : ", ", _tcd_nitems[k]);
	fprintf(fp, "\n");
	fprintf(fp, "crc16 = %04X (calc %04X)\n", _tcd_crc16, _tcd_calc16);

	fprintf(fp, "channel count = %d\n", TCDGetNumberOfChannel());
	for(k = 0; k < TCDGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 14 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X", TCDGetADC(k));
		else
			fprintf(fp, " %4d", TCDGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintBSD(FILE* fp, int mode)
        {
        unsigned k;

        fprintf(fp, "[bsd] ");
#ifndef N05282014
       unsigned char tstatus;
        tstatus = (unsigned char )(BSDGetTriStatus() & 0xff);
        fprintf(fp, "hw event number = %u (0x%08X) Sync num %u (0x%08X) event cnt %u (%02X) \n trimode(0x%02X), Conf(0x%02X), TRIGER STATUS: %s, %s, %s,\n",
                        _bsd_evnum, _bsd_evnum,
                        BSDGetSyncNum(), BSDGetSyncNum(),
                        BSDGetEventCnt(), BSDGetEventCnt(),
                        BSDGetTriMode(), BSDGetConfMode(),
                        (tstatus &  BSD_TRIGER_T2) ? "T2":"",
                        (tstatus &  BSD_TRIGER_T1) ? "T1":"",
                        (tstatus &  BSD_TRIGER_EX) ? "EXT":""
                        );
#else
        fprintf(fp, "hw event number = %u (%08X)\n", _bsd_evnum, _bsd_evnum);
#endif
        for( k = 0; k < BSDGetNumberOfChannel() ; k++)
        {
        	if(!( k % 8) ) fprintf(fp,"\n");
        	//fprintf(fp,"0x%04x ",BSDGetADC(k));

		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, "0x%04x ",BSDGetADC(k) );
		else
			fprintf(fp, " %4d ", BSDGetADC(k));
        }
        fprintf(fp, "\n\n");
}

void DEvent::PrintCVD(FILE* fp, int mode)
	{
	unsigned k;

	fprintf(fp, "[cvd]\n");
	fprintf(fp, "byte count = %d (%04X)\n", _cvd_nbytes, _cvd_nbytes);
//	fprintf(fp, "hw event number = %d (%08X)\n", _cvd_evnum, _cvd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _cvd_evnum, _cvd_evnum);  // Long int output by YS
	fprintf(fp, "header = %04X [fmt %d, mask %03X, bit count %d]\n", _cvd_header, _cvd_fmtcode, _cvd_bitmask, _cvd_bitcount);
	fprintf(fp, "items = ");
	for(k = 0; k < _cvd_bitcount; k++)
		fprintf(fp, "%s%d", (k == 0) ? "" : ", ", _cvd_nitems[k]);
	fprintf(fp, "\n");
	fprintf(fp, "crc16 = %04X (calc %04X)\n", _cvd_crc16, _cvd_calc16);

	fprintf(fp, "channel count = %d\n", CVDGetNumberOfChannel());
	for(k = 0; k < CVDGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 14 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X", CVDGetADC(k));
		else
			fprintf(fp, " %4d", CVDGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintTCDOriginal(FILE* fp, int mode)
	{
	if ( TCDQ() && _tcd_raw_nbytes > 0 )
		fwrite(_tcd_raw, sizeof(char), _tcd_raw_nbytes, fp);
}

void DEvent::PrintBSDOriginal(FILE* fp, int mode)
        {
        if ( BSDQ() && _bsd_raw_nbytes > 0 )
                fwrite(_bsd_raw, sizeof(char), _bsd_raw_nbytes, fp);
}

void DEvent::PrintTRD(FILE* fp, int mode)
	{
	fprintf(fp, "[trd]\n");
	fprintf(fp, "channel count = %d\n", TrdGetNumberOfChannel());
//	fprintf(fp, "hw event number = %d (%08X)\n", _cal_evnum, _cal_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _trd_evnum, _trd_evnum);

	for(unsigned k = 0; k < TrdGetNumberOfChannel(); k++) {
		if ( k != 0 && k % 8 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X %04X",
				TrdGetChannel(k), TrdGetADC(k));
		else
			fprintf(fp, " %4d %4d",
				TrdGetChannel(k), TrdGetADC(k));
	}
	fprintf(fp, "\n\n");
}

/**************************old Dr.L *****************
void DEvent::PrintTRD(FILE* fp, int mode)
	{
	unsigned j, k;

	fprintf(fp, "[trd]\n");
//	fprintf(fp, "hw event number = %d (%08X)\n", _trd_evnum, _trd_evnum);
	fprintf(fp, "hw event number = %u (%08X)\n", _trd_evnum, _trd_evnum);  // Long int output by YS

	fprintf(fp, "PHA count = %d\n", TRDPHAGetNumberOfChannel());
	for(j = k = 0; k < TRDPHAGetNumberOfChannel(); k++, j++) {
		if ( j != 0 && j % 16 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X", TRDPHAGetADC(k));
		else
			fprintf(fp, " %4d", TRDPHAGetADC(k));
	}
	fprintf(fp, "\n");

	fprintf(fp, "AMPLEX count = %d\n", TRDAmplexGetNumberOfChannel());
	for(j = k = 0; k < TRDAmplexGetNumberOfChannel(); k++, j++) {
		if ( j != 0 && j % 16 == 0 )
			fprintf(fp, "\n");
		if ( mode & MASK_HEXADECIMAL )
			fprintf(fp, " %04X", TRDAmplexGetADC(k));
		else
			fprintf(fp, " %4d", TRDAmplexGetADC(k));
	}
	fprintf(fp, "\n\n");
}

void DEvent::PrintTRDOriginal(FILE* fp, int mode)
	{
	unsigned k, i;
	unsigned short *sbuf;
	unsigned fifo1_count;

	if ( !(TRDQ() && _trd_raw_nbytes > 0) )
		return;
	else {
		sbuf = (unsigned short *) _trd_raw;
		i = 1; // skip the first half-word.
	}

	// sync pattern
	fprintf(fp, " face\n");

	// event number
	fprintf(fp, " %04x\n", sbuf[i++]);
	fprintf(fp, " %04x\n", sbuf[i++]);

	// pha info
	fprintf(fp, " ff00\n");
	for(k = 0; k < 7; k++)
		fprintf(fp, " %04x\n", sbuf[i++]);
	for(k = 0; k < 7; k++)
		fprintf(fp, " ffff\n");
	for(k = 0; k < 7; k++)
		fprintf(fp, " %04x\n", sbuf[i++]);
	for(k = 0; k < 6; k++)
		fprintf(fp, " ffff\n");
	fprintf(fp, " ef00\n");

	// fifo1 count
	fifo1_count = (_trd_raw_nbytes - i * sizeof(unsigned short)) / 4;
	if ( 4 * fifo1_count + i * sizeof(unsigned short) != _trd_raw_nbytes ) {
		fprintf(stderr, "CPrint::TRD: incorrect fifo1_count!\n");
	}

	// amplex info
	fprintf(fp, " %04x\n", fifo1_count);
	for(k = 0; k < 2 * fifo1_count; k++) {
		fprintf(fp, " %04x\n", sbuf[i++]);
	}
//	if ( (c & 1) != 0 ) { // odd number
//		fprintf(stderr, "odd channel count!\n");
//		// print an additional line in case of odd number
//		fprintf(fp, " 0000\n");
//	}
}
*******************************************/
void DEvent::PrintEventNumber(FILE* fp, int opt)
	{
	// software event number
	fprintf(fp, "%08X", _swevno);

	// print CAL, HDS, SCD, TCD, TRD event numbers
	// EMPTY will be printed when the corresponding data are absent.
	if ( CalorimeterQ() ) fprintf(fp, " %08X", _cal_evnum);
	else fprintf(fp, " %8s", "EMPTY");

	if ( TCRDQ() ) fprintf(fp, " %08X", _tcrd_evnum); //YONAS
	else fprintf(fp, " %8s", "EMPTY");

//   Temporarily disabled for CREAM 3 by YS 2007.8.21
//	if ( HodoscopeQ() ) fprintf(fp, " %08X", _hds_evnum);
//	else fprintf(fp, " %8s", "EMPTY");

//   Temporarily disabled for CREAM 3 by YS 2007.8.21
//	if ( SCDQ() ) fprintf(fp, " %08X", _scd_evnum);
//	else fprintf(fp, " %8s", "EMPTY");

	if ( TCDQ() ) fprintf(fp, " %08X", _tcd_evnum);
	else fprintf(fp, " %8s", "EMPTY");

	if ( BSDQ() ) fprintf(fp, " %08X", _bsd_evnum);
	else fprintf(fp, " %8s", "EMPTY");

//   Temporarily disabled for CREAM 3 by YS 2007.8.21
//	if ( TRDQ() ) fprintf(fp, " %08X", _trd_evnum);
//	else fprintf(fp, " %8s", "EMPTY");

	if ( TopSCDQ() ) fprintf(fp, " %08X", _tscd_evnum);
	else fprintf(fp, " %8s", "EMPTY");

	if ( BottomSCDQ() ) fprintf(fp, " %08X", _bscd_evnum);
	else fprintf(fp, " %8s", "EMPTY");

	if ( SCDL1Q() ) fprintf(fp, " %08X", _scdl1_evnum);     //Added by David Angelaszek 9/2/2010
	else fprintf(fp, " %8s", "EMPTY");                        //Added by David Angelaszek 9/2/2010

	if ( SCDL2Q() ) fprintf(fp, " %08X", _scdl2_evnum);  //Added by David Angelaszek 9/2/2010
	else fprintf(fp, " %8s", "EMPTY");                        //Added by David Angelaszek 9/2/2010

	if ( SCDL3Q() ) fprintf(fp, " %08X", _scdl3_evnum);  //Added by David Angelaszek 9/2/2010
	else fprintf(fp, " %8s", "EMPTY");                        //Added by David Angelaszek 9/2/2010

	if ( CVDQ() ) fprintf(fp, " %08X", _cvd_evnum);
	else fprintf(fp, " %8s", "EMPTY");

	if ( CERQ() ) fprintf(fp, " %08X", _cer_evnum);  
	else fprintf(fp, " %8s", "EMPTY");

//   Temporarily disabled for CREAM 3 by YS 2007.8.21
//	if ( SiliconBeamTrackerQ() ) fprintf(fp, " %08X", _sbt_evnum);
//	else fprintf(fp, " %8s", "EMPTY");

	fputc('\n', fp);
}
