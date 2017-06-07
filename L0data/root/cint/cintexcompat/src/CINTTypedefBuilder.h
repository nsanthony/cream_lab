// @(#)root/cintex:$Id: CINTTypedefBuilder.h 28488 2009-05-07 15:28:21Z pcanal $
// Author: Pere Mato 2005

// Copyright CERN, CH-1211 Geneva 23, 2004-2005, All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.

#ifndef ROOT_Cintex_CINTTypdefBuilder
#define ROOT_Cintex_CINTTypdefBuilder

#include "Reflex/Type.h"

namespace ROOT {
namespace Cintex {

class CINTTypedefBuilder {
public:
   static int Setup(const ROOT::Reflex::Type&);
   static void Set(const char* name, const char* value);
};

} // namespace Cintex
} // namespace ROOT

#endif // ROOT_Cintex_CINTTypdefBuilder
