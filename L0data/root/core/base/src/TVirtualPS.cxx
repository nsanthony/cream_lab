// @(#)root/base:$Id: TVirtualPS.cxx 31716 2009-12-09 10:04:45Z rdm $
// Author: Rene Brun   05/09/99

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//______________________________________________________________________________
//
// TVirtualPS is an abstract interface to a Postscript, PDF and SVG drivers
//

#include "Riostream.h"
#include "TVirtualPS.h"

TVirtualPS *gVirtualPS = 0;

const Int_t  kMaxBuffer = 250;

ClassImp(TVirtualPS)


//______________________________________________________________________________
TVirtualPS::TVirtualPS()
{
   // VirtualPS default constructor.

   fStream    = 0;
   fNByte     = 0;
   fSizBuffer = kMaxBuffer;
   fBuffer    = new char[fSizBuffer+1];
   fLenBuffer = 0;
   fPrinted   = kFALSE;
}


//______________________________________________________________________________
TVirtualPS::TVirtualPS(const char *name, Int_t)
          : TNamed(name,"Postscript interface")
{
   // VirtualPS constructor.

   fStream    = 0;
   fNByte     = 0;
   fSizBuffer = kMaxBuffer;
   fBuffer    = new char[fSizBuffer+1];
   fLenBuffer = 0;
   fPrinted   = kFALSE;
}


//______________________________________________________________________________
TVirtualPS::~TVirtualPS()
{
   // VirtualPS destructor

   if (fBuffer) delete [] fBuffer;
}


//______________________________________________________________________________
void TVirtualPS::PrintStr(const char *str)
{
   // Output the string str in the output buffer

   Int_t len = strlen(str);
   if (len == 0) return;
   if( str[0] == '@') {
      if( fLenBuffer ) {
         fStream->write(fBuffer, fLenBuffer);
         fStream->write("\n",1); fNByte++;
      }
      if ( len < 2)  {
         fBuffer[0] = ' ';
      } else {
         strcpy(fBuffer, str+1);
      }
      fLenBuffer = len-1;
      fPrinted = kTRUE;
      fNByte += len-1;
      return;
   }

   if( str[len-1] == '@') {
      if( fLenBuffer ) {
         fStream->write(fBuffer, fLenBuffer);
         fStream->write("\n",1); fNByte++;
      }
      fStream->write(str, len-1);
      fStream->write("\n",1);
      fLenBuffer = 0;
      fNByte += len;
      fPrinted = kTRUE;
      return;
   }

   if( (len + fLenBuffer ) > kMaxBuffer-1) {
      fStream->write(fBuffer, fLenBuffer);
      fStream->write("\n",1); fNByte++;
      strcpy(fBuffer, str);
      fLenBuffer = len;
      fNByte += len;
   } else {
      strcpy(fBuffer + fLenBuffer, str);
      fLenBuffer += len;
      fNByte += len;
   }
   fPrinted = kTRUE;
}


//______________________________________________________________________________
void TVirtualPS::PrintFast(Int_t len, const char *str)
{
   // Fast version of Print

   fNByte += len;
   if( (len + fLenBuffer ) > kMaxBuffer-1) {
      fStream->write(fBuffer, fLenBuffer);
      fStream->write("\n",1); fNByte++;
      while (len > kMaxBuffer-1) {
         fStream->write(str,kMaxBuffer);
         len -= kMaxBuffer;
         str += kMaxBuffer;
      }
      strcpy(fBuffer, str);
      fLenBuffer = len;
   } else {
      strcpy(fBuffer + fLenBuffer, str);
      fLenBuffer += len;
   }
   fPrinted = kTRUE;
}


//______________________________________________________________________________
void TVirtualPS::WriteInteger(Int_t n, Bool_t space )
{
   // Write one Integer to the file
   //
   // n: Integer to be written in the file.
   // space: If TRUE, a space in written before the integer.

   char str[15];
   if (space) {
      sprintf(str," %d", n);
   } else {
      sprintf(str,"%d", n);
   }
   PrintStr(str);
}


//______________________________________________________________________________
void TVirtualPS::WriteReal(Float_t z)
{
   // Write a Real number to the file

   char str[15];
   sprintf(str," %g", z);
   PrintStr(str);
}
