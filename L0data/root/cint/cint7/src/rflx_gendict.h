/* -*- C++ -*- */
/*************************************************************************
 * Copyright(c) 1995~2005  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * For the licensing terms see the file COPYING
 *
 ************************************************************************/
//$Id: rflx_gendict.h 22729 2008-03-19 10:20:10Z pcanal $

#ifndef RFLX_GENDICT_H
#define RFLX_GENDICT_H 1

namespace Cint {
   namespace Internal {

      void rflx_gendict(const char * linkfilename,
		        const char * sourcefile);

   } // namespace Internal
} // namespace Cint

#endif // RFLX_GENDICT_H
