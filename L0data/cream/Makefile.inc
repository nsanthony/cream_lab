#
# Makefile.inc --- machine dependent declarations.
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
# $Log: Makefile.inc,v $
# Revision 1.1.1.1  2006/02/02 13:02:37  umd
# initial checkin.
#
#

CC=gcc
CXX=g++

DEBUG= -g -O

# Include directory other than the standard ones
CRMROOT=/Users/nsanthony/cream_lab/L0data
INCDIR= -I$(CRMROOT)/crmfile
#INCDIR= -I/usr/local/root/include

CFLAGS= -Wall $(INCDIR) $(DEBUG)
CXXFLAGS= -Wall $(INCDIR) $(DEBUG)

# Additional translational rules
.SUFFIXES: .cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.d: %.cpp
	$(CXX) -fPIC $(CXXFLAGS) -o $*.d -c $<

%.d: %.c
	$(CC) -fPIC $(CFLAGS) -o $*.d -c $<
