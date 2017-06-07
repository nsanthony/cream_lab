/*
 * fio.c
 *
 * 2004/4/15 SYZ
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
 * $Log: fio.c,v $
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
 * Revision 1.2  2004/04/15 20:59:03  syzinn
 * added copyright notice.
 *
 */

/* uint2 and uint4 readers */
static unsigned short fget_uint2(FILE* fp)
	{
	unsigned short v;

	v = fgetc( fp );
	v += fgetc( fp ) << 8;
	return v;
}

static unsigned fget_uint4(FILE* fp)
	{
	unsigned v;

	v = fgetc( fp );
	v += fgetc( fp ) << 8;
	v += fgetc( fp ) << 16;
	v += fgetc( fp ) << 24;
	return v;
}

static unsigned short get_uint2(unsigned char* buff)
	{
	unsigned short v;

	v = (buff[1] << 8) + buff[0];
	return v;
}

static unsigned get_uint4(unsigned char* buff)
	{
	unsigned v;

	v = buff[0];
       	v += (buff[1] << 8);
       	v += (buff[2] << 16);
       	v += (buff[3] << 24);
	return v;
}

static float get_real4(unsigned char* buff)
	{
	union {
		float f;
		char c[4];
	} v;

	v.c[0] = buff[0];
	v.c[1] = buff[1];
	v.c[2] = buff[2];
	v.c[3] = buff[3];
	return v.f;
}

#define put_uint2(p, v) {\
	(p)[0] = (v) & 0xff;\
	(p)[1] = ((v) >> 8) & 0xff;\
}

#define put_uint4(p, v) {\
	(p)[0] = (v) & 0xff;\
	(p)[1] = ((v) >>  8) & 0xff;\
	(p)[2] = ((v) >> 16) & 0xff;\
	(p)[3] = ((v) >> 24) & 0xff;\
}

static void put_real4(void *vp, float f)
	{
	union {
		float f;
		char c[4];
	} v;
	char *p = (char *) vp;

	v.f = f;
	p[0] = v.c[0];
	p[1] = v.c[1];
	p[2] = v.c[2];
	p[3] = v.c[3];
}
