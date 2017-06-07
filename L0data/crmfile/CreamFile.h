#ifndef CREAMFILE_H
#define CREAMFILE_H
//
// CreamFile.h --- class for reading CREAM data files
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
// $Log: CreamFile.h,v $
// Revision 1.3  2012/10/18 14:49:18  dja1987
// Checking in some bug fixes for playback. Playback will now send playback packets event when we aren't dropping packets (i.e. we don't always have to wait for a specific number of packets that need to be playbacked). Also checking in some beginning changes for USB commanding
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
// Revision 1.10  2006/08/09 19:03:21  syzinn
// SBT data format changed
//
// Revision 1.9  2006/05/01 19:39:24  syzinn
// support for silicon beam tracker
//
// Revision 1.8  2006/03/24 14:42:37  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.7.2.2  2006/03/14 22:46:25  syzinn
// sw event number is back
//
// Revision 1.7.2.1  2006/03/14 21:07:37  syzinn
// eight-byte timestamp
//
// Revision 1.7  2005/12/17 07:36:00  syzinn
// keep event length
//
// Revision 1.6  2005/11/07 21:25:32  cvs
// supports Cerenkov Camera data
//
// Revision 1.5  2005/07/31 17:27:55  cvs
// support CVD data section
//
// Revision 1.4  2005/04/26 19:27:38  cvs
// bug fixes
//
// Revision 1.3  2005/04/08 16:33:46  cvs
// supports format 3.1
//
// Revision 1.2  2005/02/28 02:10:17  daq
// changed to format 3.0
//
// Revision 1.1.1.1  2005/02/04 05:02:17  daq
// initial version for CREAM2.
//
// Revision 1.4  2004/06/21 13:22:02  syzinn
// added _nread to check the number of bytes read.
//
// Revision 1.3  2004/04/15 20:59:02  syzinn
// added copyright notice.
//
// Revision 1.2  2004/02/27 19:20:21  syzinn
// Writing run header and tail without giving a file pointer.
//
// Revision 1.1.1.1  2004/02/24 21:46:25  syzinn
// crmfile library
//

#include <stdio.h>
#include "CreamFileHeader.h"
#include "CreamFileTail.h"
#include "DEvent.h"

class CreamFile : public CreamFileHeader, public CreamFileTail {
	// File
	FILE* _fp;
	long _filesz;

	// Event buffer for each data section
	unsigned _trig_len; // 0 or 1
	unsigned _camac_len; // byte count of data stored in  _camac_cbuf
	unsigned _cal_len;
	unsigned _tcrd_len; //YONAS
	unsigned _hds_len;
	unsigned _scd_len;
	unsigned _tcd_len;
	unsigned _bsd_len;
	unsigned _cvd_len;
	unsigned _trd_len;
	unsigned _tscd_len;
	unsigned _bscd_len;
	unsigned _scdl1_len; //Added by David Angelaszek 9/1/2010
	unsigned _scdl2_len; //Added by David Angelaszek 9/1/2010
	unsigned _scdl3_len; //Added by David Angelaszek 9/1/2010
	unsigned _ts_len; // 0 or 1
	unsigned _cer_len;
	unsigned _sbt_len;

	unsigned _type;
	unsigned _raw_nbytes; // byte size of an event
	unsigned _trig;
	char _camac_cbuf[CAMAC_BUFSIZ]; // large enough to hold 17 integers.
	char _cal_cbuf[CAL_BUFSIZ]; // 16 kB
	char _tcrd_cbuf[TCRD_BUFSIZ]; // 16 kB YONAS
	char _scd_cbuf[SCD_BUFSIZ]; // 16 kB
	char _hds_cbuf[HDS_BUFSIZ]; // 16 kB
	char _tcd_cbuf[TCD_BUFSIZ]; // 16 kB
	char _bsd_cbuf[BSD_BUFSIZ]; // 16 kB
	char _cvd_cbuf[CVD_BUFSIZ]; // 1 kB
	char _trd_cbuf[TRD_BUFSIZ]; // 16 kB
	char _tscd_cbuf[tSCD_BUFSIZ]; // 16 kB
	char _bscd_cbuf[bSCD_BUFSIZ]; // 16 kB
	char _cer_cbuf[CER_BUFSIZ];
	char _sbt_cbuf[SBT_BUFSIZ];
	char _scdl1_cbuf[SCDL1_BUFSIZ]; // 16 kB  Added by David Angelaszek 9/1/2010
	char _scdl2_cbuf[SCDL2_BUFSIZ]; // 16 kB  Added by David Angelaszek 9/1/2010
	char _scdl3_cbuf[SCDL3_BUFSIZ]; // 16 kB  Added by David Angelaszek 9/1/2010
	unsigned _timestamp[10];
	unsigned _serial; // event serial number applicable to all types of events
	unsigned _swevno; // to supplement hw event number
	unsigned _crc16;

	// the following routines record the number of bytes to _nread.
	unsigned _nread;
	int read_trigger_1();
	int read_timestamp_1();
	int read_camac();
	int read_sparsified(char* buf, unsigned nbytes, unsigned& rdlen);
	inline int read_calorimeter() { return read_sparsified( _cal_cbuf, CAL_BUFSIZ, _cal_len ); }
	inline int read_tcrd() { return read_sparsified( _tcrd_cbuf, TCRD_BUFSIZ, _tcrd_len ); }
	inline int read_hodoscope() { return read_sparsified( _hds_cbuf, HDS_BUFSIZ, _hds_len ); }
	inline int read_scd() { return read_sparsified( _scd_cbuf, SCD_BUFSIZ, _scd_len ); }
	inline int read_tscd() { return read_sparsified( _tscd_cbuf, tSCD_BUFSIZ, _tscd_len ); }
	inline int read_bscd() { return read_sparsified( _bscd_cbuf, bSCD_BUFSIZ, _bscd_len ); }
	inline int read_scdl1() { return read_sparsified( _scdl1_cbuf, SCDL1_BUFSIZ, _scdl1_len ); }//David Angelaszek 9/1/10
	inline int read_scdl2() { return read_sparsified( _scdl2_cbuf, SCDL2_BUFSIZ, _scdl2_len ); }//David Angelaszek 9/1/10
	inline int read_scdl3() { return read_sparsified( _scdl3_cbuf, SCDL3_BUFSIZ, _scdl3_len ); }//David Angelaszek 9/1/10
	inline int read_sbt() { return read_sparsified( _sbt_cbuf, SBT_BUFSIZ, _sbt_len );}
	inline int read_trd2() { return read_sparsified( _trd_cbuf, TRD_BUFSIZ, _trd_len ); }// Angelaszek TRD
  
	int read_tcd();
	int read_bsd();
	int read_cvd();
	int read_trd();
	int read_cer();

	int clear_lengths(void);

	int cream_read_0_0(void);
	int cream_read_1_0(void);
	int cream_read_1_1(void);
	int cream_read_2_0(void);
	int cream_read_3_0(void);
	int cream_read_3_1(void); // for 3.1 and 3.2
	int cream_read_3_3(void);
	int cream_read_8_0(void);

public:
	CreamFile();
	virtual ~CreamFile();

	int OpenRead(char* filename); // reading only; reads header info.
	int OpenWrite(char* filename); // writing only; open an empty file.
	inline int Open(char* filename) { return OpenRead(filename); }
	int Close(void);
	int Rewind(void);

	inline long GetFileSize(void) { return _filesz; }
	long GetCurrentFilePosition(void);

	int ReadEvent(DEvent& ev);

	inline int WriteRunHeader() { return CreamFileHeader::WriteRunHeader(_fp); }
	inline int WriteRunTail() { return CreamFileTail::WriteRunTail(_fp); }
};

#endif
