#ifndef DHKEVENTCONVERTER_H
#define DHKEVENTCONVERTER_H
//
// DHKEventConverter.h --- defines class DHKEventConverter.
//
// 2005/6/4 SYZ
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
// $Log: DHKEventConverter.h,v $
// Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
// Importing SCDL bug fixes.
//
// Revision 1.2  2011/07/20 17:22:27  dja1987
// SCDL housekeeping code added
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
// Revision 1.5  2007/03/13 21:13:57  juneki
// increased DHK_NCH
//
// Revision 1.4  2006/03/24 14:42:38  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.3.4.1  2006/03/14 21:07:49  syzinn
// eight-byte timestamp
//
// Revision 1.3  2005/06/07 12:22:54  cvs
// first working version for new hk system
//
// Revision 1.2  2005/06/07 00:16:18  cvs
// still under modification
//
// Revision 1.1  2005/06/06 21:31:04  cvs
// new
//
//

#include "DHKEvent.h"

//#define DHK_CBUFSZ 1024
//#define DHK_CBUFSZ 1200
#define DHK_CBUFSZ 2700 // should be the same size as  DHK_NCH David Angelaszek 18 July 2011

// Convert the current hk event to DHKEvent. 
class DHKEventConverter : public DHKEvent {
private:
	char	_cbuf[DHK_CBUFSZ]; // raw hk event
	int	_cbuf_count;
	int	_ev_type;
	int	_ev_format;
	unsigned _ev_crc16;
	unsigned _ev_serial;

	struct {
		struct {
			unsigned set : 1; // raw value is set;
		} flag;
		float raw; // value before conversion
		float offset; // offset for conversion
		float factor; // factor for conversion
		float quad_therm;
		float normal_min; // normal operation range
		float normal_max;
		float critical_min; // critical operation range
		float critical_max;
	} _ctbl[DHK_NCH];

	int conv_12bit(int i); // convert 12-bit signed integer to integer.
	int conv_16bit(int i); // convert 16-bit signed integer to integer.

	// Conversion of integer to voltage is the same for all channels.
	// -32768 corresponds to -10 volt.
	inline double itov(int i) { return (i * (10. / 32768)); }

public:
	DHKEventConverter();
	~DHKEventConverter();

	int load(FILE* fp);
	int load(char* fname); // load conversion factor and offset file.
	int load(char* path, char* fname);

	int Clear();
	inline void SetEventType(int t) { _ev_type = t; }
	inline int GetEventType() { return _ev_type; }
	inline void SetEventFormat(int t) { _ev_format = t; }
	inline int GetEventFormat() { return _ev_format; }
	inline void SetEventLength(int t) { _cbuf_count = t; }
	inline int GetEventLength() { return _cbuf_count; }
	inline void SetEventCRC16(unsigned t) { _ev_crc16 = t; }
	inline unsigned GetEventCRC16() { return _ev_crc16; }
	inline void SetEventSerial(unsigned t) { _ev_serial = t; }
	inline unsigned GetEventSerial() { return _ev_serial; }
	inline char* GetEventBufferPtr() { return _cbuf; }

	inline void SetADC(unsigned k, int v) {
		//_ctbl[k].raw = itov(conv_12bit(v));
		_ctbl[k].raw = itov(conv_16bit(v)); // ISS-CREAM
		_ctbl[k].flag.set = 1;
	}

	inline void SetUnsignedADC(unsigned k, int v){
          _ctbl[k].raw = 1.0 * v;
          _ctbl[k].flag.set = 1;
        }


	int Convert();
	int CheckStatus();
};

#endif
