#
# Makefile --- builds library files.
#
# 2006/2/2 SYZ
#
# Copyright (C) 2003-2006 by the CREAM project.  All rights reserved.
# 
# This file is part of CDAQ, the CREAM data acquisition and processing system.
# 
# This work may not be reproduced, displayed, modified or distributed
# without the express prior written permission of the copyright holder.
# For permission, contact Prof. Eun-suk Seo at seo@umd.edu.  The above
# copyright notice must appear in all copies and in supporting documentation.
# 
# The software is provided without any warranty, express or implied, including
# but not limited to the warranties of merchantability, fitness for a
# particular purpose and noninfringement.  In no event shall the author or
# the CREAM project group be liable for any claim, damages or other liability,
# whether in an action of contract, tort or otherwise, arising from, out of
# or in connection with the software or the use or other dealings in the
# software.
#
# $Log: Makefile,v $
# Revision 1.2  2007/01/11 08:48:57  ysy
# Make libscd3.a
#
# Revision 1.1.1.1  2006/02/02 13:02:37  umd
# initial checkin.
#

# Machine dependency
include ../Makefile.inc

TARGET= libscd3.a

SRCS=\
	CreamSCD3.cpp\

OBJS=\
	CreamSCD3.o\

build: $(TARGET)

libscd3.a: $(OBJS)
	rm -f libscd3.a
	ar q libscd3.a $(OBJS)
	ranlib libscd3.a

clean:
	rm -f *.o core.* $(TARGET)

depend:
	makedepend -Y -o.o -- $(CFLAGS) -- $(SRCS)

# DO NOT DELETE

crmfile.o: CreamFile.h CreamFileHeader.h CreamFileTail.h DEvent.h crmevtx.h
CreamFile.o: CreamFile.h CreamFileHeader.h CreamFileTail.h DEvent.h crmevtx.h
CreamFile.o: fio.c
CreamFileHeader.o: CreamFileHeader.h fio.c
CreamFileTail.o: CreamFileTail.h fio.c
CreamHKFile.o: defs.h crmevtx.h DHKEventConverter.h DHKEvent.h CreamHKFile.h
CreamHKFile.o: CreamHKFileHeader.h fio.c
CreamHKFileHeader.o: CreamHKFileHeader.h fio.c
DEvent.o: DEvent.h crmevtx.h fio.c
DHKEvent.o: defs.h timez.h DHKEvent.h
DHKEventConverter.o: defs.h timez.h DHKEventConverter.h DHKEvent.h
