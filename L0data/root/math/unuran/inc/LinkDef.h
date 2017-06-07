/* @(#)root/minuit:$Id: LinkDef.h 20882 2007-11-19 11:31:26Z rdm $ */

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;


#pragma link C++ class TUnuran+;
#pragma link C++ class TUnuranBaseDist+;
#pragma link C++ class TUnuranContDist+;
#pragma link C++ class TUnuranMultiContDist+;
#pragma link C++ class TUnuranDiscrDist+;
#pragma link C++ class TUnuranEmpDist+;

#pragma link C++ function  TUnuranDiscrDist::TUnuranDiscrDist (double *, double*);
#pragma link C++ function  TUnuranEmpDist::TUnuranEmpDist (double *, double*,unsigned int);


#endif
