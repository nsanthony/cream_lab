# Module.mk for rfio module
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

MODNAME      := rfio
MODDIR       := io/$(MODNAME)
MODDIRS      := $(MODDIR)/src
MODDIRI      := $(MODDIR)/inc

RFIODIR      := $(MODDIR)
RFIODIRS     := $(RFIODIR)/src
RFIODIRI     := $(RFIODIR)/inc

##### libRFIO #####
RFIOL        := $(MODDIRI)/LinkDef.h
RFIODS       := $(MODDIRS)/G__RFIO.cxx
RFIODO       := $(RFIODS:.cxx=.o)
RFIODH       := $(RFIODS:.cxx=.h)

RFIOH        := $(filter-out $(MODDIRI)/LinkDef%,$(wildcard $(MODDIRI)/*.h))
RFIOS        := $(filter-out $(MODDIRS)/G__%,$(wildcard $(MODDIRS)/*.cxx))
RFIOO        := $(RFIOS:.cxx=.o)

RFIODEP      := $(RFIOO:.o=.d) $(RFIODO:.o=.d)

RFIOLIB      := $(LPATH)/libRFIO.$(SOEXT)
RFIOMAP      := $(RFIOLIB:.$(SOEXT)=.rootmap)

# used in the main Makefile
ALLHDRS     += $(patsubst $(MODDIRI)/%.h,include/%.h,$(RFIOH))
ALLLIBS     += $(RFIOLIB)
ALLMAPS     += $(RFIOMAP)

# include all dependency files
INCLUDEFILES += $(RFIODEP)

##### local rules #####
.PHONY:         all-$(MODNAME) clean-$(MODNAME) distclean-$(MODNAME)

include/%.h:    $(RFIODIRI)/%.h
		cp $< $@

$(RFIOLIB):     $(RFIOO) $(RFIODO) $(ORDER_) $(MAINLIBS) $(RFIOLIBDEP)
		@$(MAKELIB) $(PLATFORM) $(LD) "$(LDFLAGS)" \
		   "$(SOFLAGS)" libRFIO.$(SOEXT) $@ "$(RFIOO) $(RFIODO)" \
		   "$(SHIFTLIBDIR) $(SHIFTLIB) $(RFIOLIBEXTRA)"

$(RFIODS):      $(RFIOH) $(RFIOL) $(ROOTCINTTMPDEP)
		@echo "Generating dictionary $@..."
		$(ROOTCINTTMP) -f $@ -c $(RFIOH) $(RFIOL)

$(RFIOMAP):     $(RLIBMAP) $(MAKEFILEDEP) $(RFIOL)
		$(RLIBMAP) -o $(RFIOMAP) -l $(RFIOLIB) \
		   -d $(RFIOLIBDEPM) -c $(RFIOL)

all-$(MODNAME): $(RFIOLIB) $(RFIOMAP)

clean-$(MODNAME):
		@rm -f $(RFIOO) $(RFIODO)

clean::         clean-$(MODNAME)

distclean-$(MODNAME): clean-$(MODNAME)
		@rm -f $(RFIODEP) $(RFIODS) $(RFIODH) $(RFIOLIB) $(RFIOMAP)

distclean::     distclean-$(MODNAME)

##### extra rules ######
ifeq ($(PLATFORM),win32)
$(RFIOO): CXXFLAGS += $(SHIFTCFLAGS) $(SHIFTINCDIR:%=-I%) -DNOGDI -D__INSIDE_CYGWIN__
else
$(RFIOO): CXXFLAGS += $(SHIFTCFLAGS) $(SHIFTINCDIR:%=-I%)
endif