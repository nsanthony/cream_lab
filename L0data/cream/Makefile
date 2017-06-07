# Resolve machine dependence
include ./Makefile.inc

MOREINC = -I./cal -I./tbcd -I./scd1 -I./scd2 -I./scd3 -I./scd4 -I./bsd $(shell root-config --cflags)

LIBS=   -L$(CRMROOT)/crmfile -lcrmfile -lm \
	-L./cal -lcal \
        -L./tbcd -ltbcd \
	-L./scd1 -lscd1 \
        -L./scd2 -lscd2 \
        -L./scd3 -lscd3 \
        -L./scd4 -lscd4 \
	-L./bsd -lbsd \
	$(shell root-config --glibs) 

DIRS= cal tbcd scd1 scd2 scd3 scd4 bsd

DEPS= cal/libcal.a tbcd/libtbcd.a scd1/libscd1.a scd2/libscd2.a scd3/libscd3.a scd4/libscd4.a bsd/libbsd.a

build: all adc

all:
	for dir in $(DIRS); do \
	  (cd $$dir; make); \
	done;

clean:
	for dir in $(DIRS); do \
	  (cd $$dir; make clean); \
	done;
	rm -f *.o core.* go

adc: adc.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(MOREINC) -o adc adc.cpp -I/usr/lib -I${ROOTSYS}/include/ -L${ROOTSYS}/lib -lCore -lCint  -lTree -lvectorDict -lMatrix $(LIBS)
