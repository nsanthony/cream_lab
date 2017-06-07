// @(#)root/io:$Id: TGenCollectionStreamer.h 23122 2008-04-10 14:56:30Z rdm $
// Author: Markus Frank  28/10/04

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#ifndef ROOT_TGenCollectionStreamer
#define ROOT_TGenCollectionStreamer

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGenCollectionStreamer
//
// Streamer around an arbitrary STL like container, which implements basic
// container functionality.
//
//////////////////////////////////////////////////////////////////////////

#include "TGenCollectionProxy.h"

class TGenCollectionStreamer : public TGenCollectionProxy {

protected:
   void ReadMapHelper(StreamHelper *i, Value *v, Bool_t vsn3,  TBuffer &b);
   void ReadMap(int nElements, TBuffer &b);
   void ReadPairFromMap(int nElements, TBuffer &b);
   void ReadObjects(int nElements, TBuffer &b);
   void ReadPrimitives(int nElements, TBuffer &b);
   void WriteMap(int nElements, TBuffer &b);
   void WriteObjects(int nElements, TBuffer &b);
   void WritePrimitives(int nElements, TBuffer &b);

public:
   // Virtual copy constructor
   virtual TVirtualCollectionProxy* Generate() const;

   // Copy constructor
   TGenCollectionStreamer(const TGenCollectionStreamer& copy);

   // Initializing constructor
   TGenCollectionStreamer(Info_t typ, size_t iter_size);
   TGenCollectionStreamer(const ROOT::TCollectionProxyInfo &info, TClass *cl);

   // Standard destructor
   virtual ~TGenCollectionStreamer();

   // Streamer I/O overload
   virtual void Streamer(TBuffer &refBuffer);

   // Streamer I/O overload
   virtual void StreamerAsMap(TBuffer &refBuffer);

   // Streamer I/O overload
   virtual void Streamer(TBuffer &buff, void *pObj, int siz)  {
      TGenCollectionProxy::Streamer(buff, pObj, siz);
   }
};

template <typename T>
struct AnyCollectionStreamer : public TGenCollectionStreamer  {
   AnyCollectionStreamer()
      : TGenCollectionStreamer(typeid(T::Cont_t),sizeof(T::Iter_t))  {
      fValDiff        = sizeof(T::Value_t);
      fValOffset      = T::value_offset();
      fSize.call      = T::size;
      fFirst.call     = T::first;
      fNext.call      = T::next;
      fClear.call     = T::clear;
      fResize.call    = T::resize;
      fCollect.call   = T::collect;
      fConstruct.call = T::construct;
      fDestruct.call  = T::destruct;
      fFeed.call      = T::feed;
      CheckFunctions();
   }
   virtual ~AnyCollectionStreamer() {  }
};

// Forward declaration in the event of later seperation
typedef TGenCollectionStreamer TGenMapStreamer;

#endif
