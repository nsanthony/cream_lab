# @(#)root/mathcore:$Id: Makefile.am 20882 2007-11-19 11:31:26Z rdm $
# Author: L. Moneta    2005  
#
## Process this file with automake to produce Makefile.in .
#
#
# The following is set, otherwise it would follow GNU conventions.
AUTOMAKE_OPTIONS = foreign



# A trick to add options to the aclocal command.
ACLOCAL = aclocal -I config/m4

SUBDIRS =   src inc doc test
#tests 

EXTRA_DIST = autogen config





## Make the documentation in the build directory
docs:
	( cd doc; $(MAKE) docs )
