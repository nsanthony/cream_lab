#!/bin/sh
# $Id: g4_makedist.sh 21499 2007-12-19 16:50:00Z brun $

# Script to produce source and optionally binary distribution of geant4_vmc.
# Called by main Makefile.
#
# According to: 
# $ROOTSYS/build/unix/makedist.sh
# Author: Fons Rademakers, 29/2/2000
#
# Usage: makedist.sh [gcc_version] [lib]
#
# By I.Hrivnacova, 7/10/2002

CURDIR=`pwd`

# gmake is called from geant4_vmc/source
cd ../..

if [ "x$1" = "xlib" ]; then
   GCC_VERS=""
   MAKELIB="geant4_vmc/include geant4_vmc/lib"
elif [ "x$2" = "xlib" ]; then
   GCC_VERS=$1
   MAKELIB="geant4_vmc/include geant4_vmc/lib"
else
   GCC_VERS=$1
fi
VERSION=`cat geant4_vmc/version_number`
MACHINE=`root-config --arch`
if [ "x$MAKELIB" = "xgeant4_vmc/lib" ]; then
   if [ "x$GCC_VERS" = "x" ]; then  
      TYPE=$MACHINE.
   else
      TYPE=$MACHINE.$GCC_VERS.
   fi
else   
  TYPE=""
fi  
TARFILE=geant4_vmc.$VERSION.$TYPE"tar"

TAR=`which gtar`
dum=`echo $TAR | grep "no gtar"`
stat=$?
if [ "$TAR" = '' ] || [ $stat = 0 ]; then
   TAR="tar cvf"
   rm -f $TARFILE.gz
   EXCLUDE=
else 
   TAR=$TAR" zcvf"
   rm -f $TARFILE.gz
   TARFILE=$TARFILE".gz"
   EXCLUDE="--exclude .svn"
fi

$TAR $TARFILE $EXCLUDE geant4_vmc/README geant4_vmc/LICENSE \
   geant4_vmc/"history" geant4_vmc/Geant4VMC.html geant4_vmc/version_number  \
   geant4_vmc/"source" geant4_vmc/examples $MAKELIB

cd $CURDIR

exit 0
