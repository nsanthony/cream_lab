/* @(#)root/main:$Id: setpawc.c 27662 2009-03-01 09:45:17Z brun $ */
/* Author: */
// @(#)root/main:$Id: setpawc.c 27662 2009-03-01 09:45:17Z brun $
// Author: Valery Fine(fine@vxcern.cern.ch)   08/12/96
//*--
//*--  This fortran subroutine is needed to set the right size
//*--  for the /PAWC/ common block for H2Root C++ utility.
//*--  This common is defined as "external" in the H2Root and
//*--  its size is ignored by linker.
int PAWC[4000000];
void setpawc(){}
