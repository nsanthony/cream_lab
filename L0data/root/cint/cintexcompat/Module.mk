# Module.mk for reflex module
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

MODNAME      := cintexcompat
MODDIR       := cint/$(MODNAME)
MODDIRS      := $(MODDIR)/src
MODDIRI      := $(MODDIR)/inc

CINTEXDIR    := $(MODDIR)
CINTEXDIRS   := $(CINTEXDIR)/src
CINTEXDIRI   := $(CINTEXDIR)/inc

##### libCintex #####
CINTEXL      := $(CINTEXDIRI)/LinkDef.h
CINTEXH      := $(wildcard $(MODDIRI)/Cintex/*.h)
CINTEXS      := $(wildcard $(MODDIRS)/*.cxx)
CINTEXO      := $(CINTEXS:.cxx=.o)

CINTEXDEP    := $(CINTEXO:.o=.d)

CINTEXLIB    := $(LPATH)/libCintex.$(SOEXT)
CINTEXMAP    := $(CINTEXLIB:.$(SOEXT)=.rootmap)

CINTEXPYS    := $(wildcard $(MODDIR)/python/*.py)
ifeq ($(PLATFORM),win32)
CINTEXPY     := $(subst $(MODDIR)/python,bin,$(CINTEXPYS))
bin/%.py: $(MODDIR)/python/%.py; cp $< $@
else
CINTEXPY     := $(subst $(MODDIR)/python,$(LPATH),$(CINTEXPYS))
$(LPATH)/%.py: $(MODDIR)/python/%.py; cp $< $@
endif
ifneq ($(BUILDPYTHON),no)
CINTEXPYC    := $(CINTEXPY:.py=.pyc)
CINTEXPYO    := $(CINTEXPY:.py=.pyo)
endif

# used in the main Makefile
ALLHDRS      += $(patsubst $(MODDIRI)/Cintex/%.h,include/Cintex/%.h,$(CINTEXH))
ALLLIBS      += $(CINTEXLIB)
ALLMAPS      += $(CINTEXMAP)

# include all dependency files
INCLUDEFILES += $(CINTEXDEP)

# test suite
ifeq ($(PLATFORM),win32)
REFLEXLL = lib/libReflex.lib
CINTEXLL = lib/libCintex.lib
SHEXT    = .bat
DICTEXT  = dll
else
REFLEXLL = -Llib -lReflex
CINTEXLL = -Llib -lCintex
SHEXT    = .sh
ifneq ($(PLATFORM),fbsd)
ifneq ($(PLATFORM),obsd)
REFLEXLL   += -ldl
endif
endif
ifeq ($(PLATFORM),macosx)
DICTEXT  = so
else
DICTEXT  = $(SOEXT)
endif
endif

GENREFLEX_CMD2 = python ../../../../lib/python/genreflex/genreflex.py

CINTEXTESTD     = $(CINTEXDIR)/test
CINTEXTESTDICTD = $(CINTEXTESTD)/dict
CINTEXTESTDICTL = $(CINTEXTESTDICTD)/lib
CINTEXTESTDICTH = $(CINTEXTESTDICTD)/CintexTest.h
CINTEXTESTDICTS = $(subst .h,_rflx.cpp,$(CINTEXTESTDICTH))
CINTEXTESTDICTO = $(subst .cpp,.o,$(CINTEXTESTDICTS))
CINTEXTESTDICT  = $(subst $(CINTEXTESTDICTD)/,$(CINTEXTESTDICTD)/test_,$(subst _rflx.o,Rflx.$(DICTEXT),$(CINTEXTESTDICTO)))

##### local rules #####
.PHONY:         all-$(MODNAME) clean-$(MODNAME) distclean-$(MODNAME) \
                check-$(MODNAME) clean-check-$(MODNAME)

include/Cintex/%.h: $(CINTEXDIRI)/Cintex/%.h
		@(if [ ! -d "include/Cintex" ]; then    \
		   mkdir -p include/Cintex;             \
		fi)
		cp $< $@

%.pyc: %.py;    python -c 'import py_compile; py_compile.compile( "$<" )'
%.pyo: %.py;    python -O -c 'import py_compile; py_compile.compile( "$<" )'

ifeq ($(findstring $(MAKECMDGOALS),distclean maintainer-clean),)
# Avoid conflict with cintex/Module.mk
$(CINTEXLIB):   $(CINTEXO) $(CINTEXPY) $(CINTEXPYC) $(CINTEXPYO) \
                $(ORDER_) $(subst $(CINTEXLIB),,$(MAINLIBS)) $(CINTEXLIBDEP)
		@$(MAKELIB) $(PLATFORM) $(LD) "$(LDFLAGS)"      \
		"$(SOFLAGS)" libCintex.$(SOEXT) $@ "$(CINTEXO)" \
		"$(CINTEXLIBEXTRA)"

$(CINTEXMAP):   $(RLIBMAP) $(MAKEFILEDEP) $(CINTEXL)
		$(RLIBMAP) -o $(CINTEXMAP) -l $(CINTEXLIB) \
		   -d $(CINTEXLIBDEPM) -c $(CINTEXL)
endif

all-$(MODNAME): $(CINTEXLIB) $(CINTEXMAP)

clean-$(MODNAME): clean-check-$(MODNAME)
		@rm -f $(CINTEXO)

clean-check-$(MODNAME):
		@rm -f $(CINTEXTESTDICTS) $(CINTEXTESTDICTO)

clean::         clean-$(MODNAME)

distclean-$(MODNAME): clean-$(MODNAME)
		@rm -f $(CINTEXDEP) $(CINTEXLIB) $(CINTEXMAP) $(CINTEXPY) $(CINTEXPYC) $(CINTEXPYO)
		@rm -rf include/Cintex

distclean::     distclean-$(MODNAME)


#### test suite ####

check-$(MODNAME): $(REFLEXLIB) $(CINTEXLIB) $(CINTEXTESTDICT)
		@echo "Running all Cintex tests"
		@$(CINTEXTESTD)/test_all$(SHEXT)  $(PYTHONINCDIR)

$(CINTEXTESTDICT): $(CINTEXTESTDICTO)
		@mkdir -p $(CINTEXTESTDICTL)
		@$(MAKELIB) $(PLATFORM) $(LD) "$(LDFLAGS)" "$(SOFLAGS)" $(notdir $@) $@ $< "$(REFLEXLL)"

$(CINTEXTESTDICTO): CXXFLAGSPCH =

$(CINTEXTESTDICTS): $(CINTEXTESTDICTH) $(CINTEXTESTDICTD)/selection.xml
		cd $(CINTEXTESTDICTD); $(GENREFLEX_CMD2) CintexTest.h -s selection.xml --rootmap=$(PWD)/$(CINTEXTESTDICT).rootmap --rootmap-lib=$(CINTEXTESTDICT) --comments

##### extra rules ######
$(CINTEXO): CXXFLAGS += -Iinclude/cint
ifeq ($(PLATFORM),macosx)
ifeq ($(ICC_MAJOR),9)
ifeq ($(ICC_MINOR),1)
$(CINTEXDIRS)/ROOTClassEnhancer.o: OPT = $(NOOPT)
endif
endif
endif