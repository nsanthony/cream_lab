// @(#)root/bonjour:$Id: TBonjourRecord.cxx 28781 2009-06-02 16:27:00Z rdm $
// Author: Fons Rademakers   29/05/2009

/*************************************************************************
 * Copyright (C) 1995-2009, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBonjourRecord                                                       //
//                                                                      //
// Contains all information concerning a Bonjour entry.                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TBonjourRecord.h"
#include "Riostream.h"


ClassImp(TBonjourRecord)

//______________________________________________________________________________
void TBonjourRecord::Print(Option_t *) const
{
   // Print TBonjourRecord.

   cout << "TBonjourRecord:" << "\t" << GetServiceName()
        << "\t" << GetRegisteredType() << "\t" << GetReplyDomain() << endl;
}
