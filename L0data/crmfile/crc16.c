/*
 * crc16.c --- computes CRC16 checksum.  adopted from CREAM ICD.
 *
 * 2003/10/9 SYZ
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
 * $Log: crc16.c,v $
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
 * Revision 1.1.1.1  2009/01/24 04:30:19  creamop
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
 * Revision 1.1  2004/07/05 19:50:11  syzinn
 * addition of crc16.c
 *
 * Revision 1.4  2004/04/15 21:03:07  syzinn
 * added copyright notice.
 *
 * Revision 1.3  2004/02/21 15:30:03  syzinn
 * Added memory leak detection
 *
 * Revision 1.2  2003/10/25 21:12:06  syzinn
 * Placed a tab.
 *
 * Revision 1.1  2003/10/09 19:52:17  syzinn
 * Addition of CRC checksum routine.
 *
 */
#ifdef MEMLEAK
#include "memleak.h"
#endif

static const unsigned char lowCRC_ab[] = {
0x00, 0x21, 0x42, 0x63, 0x84, 0xa5, 0xc6, 0xe7, 0x08, 0x29, 0x4a, 0x6b, 0x8c,
0xad, 0xce, 0xef, 0x31, 0x10, 0x73, 0x52, 0xb5, 0x94, 0xf7, 0xd6, 0x39, 0x18,
0x7b, 0x5a, 0xbd, 0x9c, 0xff, 0xde, 0x62, 0x43, 0x20, 0x01, 0xe6, 0xc7, 0xa4,
0x85, 0x6a, 0x4b, 0x28, 0x09, 0xee, 0xcf, 0xac, 0x8d, 0x53, 0x72, 0x11, 0x30,
0xd7, 0xf6, 0x95, 0xb4, 0x5b, 0x7a, 0x19, 0x38, 0xdf, 0xfe, 0x9d, 0xbc, 0xc4,
0xe5, 0x86, 0xa7, 0x40, 0x61, 0x02, 0x23, 0xcc, 0xed, 0x8e, 0xaf, 0x48, 0x69,
0x0a, 0x2b, 0xf5, 0xd4, 0xb7, 0x96, 0x71, 0x50, 0x33, 0x12, 0xfd, 0xdc, 0xbf,
0x9e, 0x79, 0x58, 0x3b, 0x1a, 0xa6, 0x87, 0xe4, 0xc5, 0x22, 0x03, 0x60, 0x41,
0xae, 0x8f, 0xec, 0xcd, 0x2a, 0x0b, 0x68, 0x49, 0x97, 0xb6, 0xd5, 0xf4, 0x13,
0x32, 0x51, 0x70, 0x9f, 0xbe, 0xdd, 0xfc, 0x1b, 0x3a, 0x59, 0x78, 0x88, 0xa9,
0xca, 0xeb, 0x0c, 0x2d, 0x4e, 0x6f, 0x80, 0xa1, 0xc2, 0xe3, 0x04, 0x25, 0x46,
0x67, 0xb9, 0x98, 0xfb, 0xda, 0x3d, 0x1c, 0x7f, 0x5e, 0xb1, 0x90, 0xf3, 0xd2,
0x35, 0x14, 0x77, 0x56, 0xea, 0xcb, 0xa8, 0x89, 0x6e, 0x4f, 0x2c, 0x0d, 0xe2,
0xc3, 0xa0, 0x81, 0x66, 0x47, 0x24, 0x05, 0xdb, 0xfa, 0x99, 0xb8, 0x5f, 0x7e,
0x1d, 0x3c, 0xd3, 0xf2, 0x91, 0xb0, 0x57, 0x76, 0x15, 0x34, 0x4c, 0x6d, 0x0e,
0x2f, 0xc8, 0xe9, 0x8a, 0xab, 0x44, 0x65, 0x06, 0x27, 0xc0, 0xe1, 0x82, 0xa3,
0x7d, 0x5c, 0x3f, 0x1e, 0xf9, 0xd8, 0xbb, 0x9a, 0x75, 0x54, 0x37, 0x16, 0xf1,
0xd0, 0xb3, 0x92, 0x2e, 0x0f, 0x6c, 0x4d, 0xaa, 0x8b, 0xe8, 0xc9, 0x26, 0x07,
0x64, 0x45, 0xa2, 0x83, 0xe0, 0xc1, 0x1f, 0x3e, 0x5d, 0x7c, 0x9b, 0xba, 0xd9,
0xf8, 0x17, 0x36, 0x55, 0x74, 0x93, 0xb2, 0xd1, 0xf0
};

static const unsigned char highCRC_ab[] = {
0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x81, 0x91, 0xa1, 0xb1, 0xc1,
0xd1, 0xe1, 0xf1, 0x12, 0x02, 0x32, 0x22, 0x52, 0x42, 0x72, 0x62, 0x93, 0x83,
0xb3, 0xa3, 0xd3, 0xc3, 0xf3, 0xe3, 0x24, 0x34, 0x04, 0x14, 0x64, 0x74, 0x44,
0x54, 0xa5, 0xb5, 0x85, 0x95, 0xe5, 0xf5, 0xc5, 0xd5, 0x36, 0x26, 0x16, 0x06,
0x76, 0x66, 0x56, 0x46, 0xb7, 0xa7, 0x97, 0x87, 0xf7, 0xe7, 0xd7, 0xc7, 0x48,
0x58, 0x68, 0x78, 0x08, 0x18, 0x28, 0x38, 0xc9, 0xd9, 0xe9, 0xf9, 0x89, 0x99,
0xa9, 0xb9, 0x5a, 0x4a, 0x7a, 0x6a, 0x1a, 0x0a, 0x3a, 0x2a, 0xdb, 0xcb, 0xfb,
0xeb, 0x9b, 0x8b, 0xbb, 0xab, 0x6c, 0x7c, 0x4c, 0x5c, 0x2c, 0x3c, 0x0c, 0x1c,
0xed, 0xfd, 0xcd, 0xdd, 0xad, 0xbd, 0x8d, 0x9d, 0x7e, 0x6e, 0x5e, 0x4e, 0x3e,
0x2e, 0x1e, 0x0e, 0xff, 0xef, 0xdf, 0xcf, 0xbf, 0xaf, 0x9f, 0x8f, 0x91, 0x81,
0xb1, 0xa1, 0xd1, 0xc1, 0xf1, 0xe1, 0x10, 0x00, 0x30, 0x20, 0x50, 0x40, 0x70,
0x60, 0x83, 0x93, 0xa3, 0xb3, 0xc3, 0xd3, 0xe3, 0xf3, 0x02, 0x12, 0x22, 0x32,
0x42, 0x52, 0x62, 0x72, 0xb5, 0xa5, 0x95, 0x85, 0xf5, 0xe5, 0xd5, 0xc5, 0x34,
0x24, 0x14, 0x04, 0x74, 0x64, 0x54, 0x44, 0xa7, 0xb7, 0x87, 0x97, 0xe7, 0xf7,
0xc7, 0xd7, 0x26, 0x36, 0x06, 0x16, 0x66, 0x76, 0x46, 0x56, 0xd9, 0xc9, 0xf9,
0xe9, 0x99, 0x89, 0xb9, 0xa9, 0x58, 0x48, 0x78, 0x68, 0x18, 0x08, 0x38, 0x28,
0xcb, 0xdb, 0xeb, 0xfb, 0x8b, 0x9b, 0xab, 0xbb, 0x4a, 0x5a, 0x6a, 0x7a, 0x0a,
0x1a, 0x2a, 0x3a, 0xfd, 0xed, 0xdd, 0xcd, 0xbd, 0xad, 0x9d, 0x8d, 0x7c, 0x6c,
0x5c, 0x4c, 0x3c, 0x2c, 0x1c, 0x0c, 0xef, 0xff, 0xcf, 0xdf, 0xaf, 0xbf, 0x8f,
0x9f, 0x6e, 0x7e, 0x4e, 0x5e, 0x2e, 0x3e, 0x0e, 0x1e
};

unsigned short calc_crc16(unsigned char *_data_pb, unsigned short _numBytes_t)
    {
    unsigned short k_t, index_t;
    unsigned short highParity_t = 255;
    unsigned short lowParity_t = 255;

    for (index_t = 0; index_t < _numBytes_t; index_t++) {
	k_t = (unsigned short) (*_data_pb++ ^ highParity_t);

	highParity_t =
	    (unsigned short) (lowParity_t ^ highCRC_ab[k_t]);

	lowParity_t = lowCRC_ab[k_t];
    }

    return (((unsigned short) (highParity_t << 8 | lowParity_t)));
}