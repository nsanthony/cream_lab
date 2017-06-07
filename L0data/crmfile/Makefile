#
# Makefile --- builds library files.
#
# 2004/4/15 SYZ
#
# Copyright (C) 2003-2004 by the CREAM project.  All rights reserved.
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

# Machine dependency
include ./Makefile.inc

ifndef $(INSTOPT)
INSTOPT = -o $(USER) -g $(GROUP)
endif

#TARGET= libcrmfile.a crmfile.so
TARGET= libcrmfile.a

INSTDIR= $(CDAQROOT)/lib

FILES=\
	Makefile Makefile.inc\
	crmfile.h crmfile.cpp\
	CreamFile.h CreamFile.cpp\
	CreamFileHeader.h CreamFileHeader.cpp\
	CreamFileTail.h CreamFileTail.cpp\
	DEvent.h DEvent.cpp\
	fio.c crc16.c\
	CreamHKFile.h CreamHKFile.cpp\
	CreamHKFileHeader.h CreamHKFileHeader.cpp\
	DHKEvent.h DHKEvent.cpp\
	DHKEventConverter.h DHKEventConverter.cpp\
	LinkDefs.h

IMPORTS=\
	defs.h crmevtx.h timez.h timez.c

SRCS=\
	crmfile.cpp\
	CreamFile.cpp\
	CreamFileHeader.cpp\
	CreamFileTail.cpp\
	CreamHKFile.cpp\
	CreamHKFileHeader.cpp\
	DEvent.cpp\
	DHKEvent.cpp\
	DHKEventConverter.cpp\
	fio.c\
	crc16.c

OBJS=\
	CreamFileHeader.o CreamFile.o DEvent.o CreamFileTail.o\
	CreamHKFile.o DHKEvent.o DHKEventConverter.o CreamHKFileHeader.o

SOBJS=\
	crmfileint.d\
	CreamFileHeader.d CreamFile.d DEvent.d CreamFileTail.d\
	CreamHKFile.d DHKEvent.d DHKEventConverter.d CreamHKFileHeader.d\
	crc16.d timez.d

HDRS=\
	CreamFile.h CreamFileHeader.h CreamFileTail.h DEvent.h\
	CreamHKFile.h CreamHKFileHeader.h DHKEvent.h DHKEventConverter.h

%.d: %.cpp
	$(CXX) -fPIC $(CXXFLAGS) -I$(ROOTSYS)/include -o $*.d -c $<

%.d: %.c
	$(CC) -fPIC $(CFLAGS) -I../base -I$(ROOTSYS)/include -o $*.d -c $<

build: $(IMPORTS) $(TARGET)

crmfileint.d: crmfileint.cpp
                                                                                
crmfileint.cpp: LinkDefs.h $(HDRS)
	rootcint -f crmfileint.cpp -c $(HDRS) defs.h LinkDefs.h

crmevtx.h:
	@if [ -r ../evt/crmevtx.h ]; then\
		cp -f ../evt/crmevtx.h crmevtx.h;\
	fi

defs.h:
	@if [ -r ../base/defs.h ]; then\
		cp -f ../base/defs.h defs.h;\
	fi

timez.h:
	@if [ -r ../base/timez.h ]; then\
		cp -f ../base/timez.h timez.h;\
	fi

timez.c:
	@if [ -r ../base/timez.c ]; then\
		cp -f ../base/timez.c timez.c;\
	fi

libcrmfile.a: $(OBJS)
	rm -f libcrmfile.a
	ar q libcrmfile.a $(OBJS)
	ranlib libcrmfile.a

crmfile.so: $(SOBJS)
	rm -f crmfile.so
	$(CXX) -shared -Wl,-soname,crmfile.so -o crmfile.so $(SOBJS)

sample: sample.o
	$(CXX) -o sample sample.o -L. -lcrmfile

clean:
	rm -f *.o *.d core core.* $(TARGET)
	rm -f crmfileint.h crmfileint.cpp
	rm -f defs.h timez.h timez.c
#Testing removing crmevtx.h
depend:
	makedepend -Y -o.o -- $(CFLAGS) -- $(SRCS)

install: $(TARGET)
	@if [ ! -r "$(INSTDIR)" ]; then\
		mkdir $(INSTDIR);\
		chown $(OWNER).$(GROUP) $(INSTDIR);\
	fi
	install -m 0755 $(INSTOPT) $(TARGET) $(INSTDIR)

tarball: $(FILES) $(IMPORTS)
	tar cf crmfile.tar $(FILES) $(IMPORTS)

makeso: crmfile.so
	cp crmfile.so $(CDAQROOT)/lib

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
