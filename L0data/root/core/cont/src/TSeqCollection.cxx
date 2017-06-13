// @(#)root/cont:$Id: TSeqCollection.cxx 28588 2009-05-12 21:43:28Z pcanal $
// Author: Fons Rademakers   04/08/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSeqCollection                                                       //
//                                                                      //
// Sequenceable collection abstract base class. TSeqCollection's have   //
// an ordering relation, i.e. there is a first and last element.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TSeqCollection.h"
#include "TCollection.h"
#include "TVirtualMutex.h"
#include "TClass.h"
#include "TMethodCall.h"

ClassImp(TSeqCollection)

//______________________________________________________________________________
Int_t TSeqCollection::IndexOf(const TObject *obj) const
{
   // Return index of object in collection. Returns -1 when object not found.
   // Uses member IsEqual() to find object.

   Int_t   idx = 0;
   TIter   next(this);
   TObject *ob;

   while ((ob = next())) {
      if (ob->IsEqual(obj)) return idx;
      idx++;
   }
   return -1;
}

//______________________________________________________________________________
Int_t TSeqCollection::ObjCompare(TObject *a, TObject *b)
{
   // Compare to objects in the collection. Use member Compare() of object a.

   if (a == 0 && b == 0) return 0;
   if (a == 0) return 1;
   if (b == 0) return -1;
   return a->Compare(b);
}

//______________________________________________________________________________
void TSeqCollection::QSort(TObject **a, Int_t first, Int_t last)
{
   // Sort array of TObject pointers using a quicksort algorithm.
   // Uses ObjCompare() to compare objects.

   R__LOCKGUARD2(gCollectionMutex);

   static TObject *tmp;
   static int i;           // "static" to save stack space
   int j;

   while (last - first > 1) {
      i = first;
      j = last;
      for (;;) {
         while (++i < last && ObjCompare(a[i], a[first]) < 0)
            ;
         while (--j > first && ObjCompare(a[j], a[first]) > 0)
            ;
         if (i >= j)
            break;

         tmp  = a[i];
         a[i] = a[j];
         a[j] = tmp;
      }
      if (j == first) {
         ++first;
         continue;
      }
      tmp = a[first];
      a[first] = a[j];
      a[j] = tmp;
      if (j - first < last - (j + 1)) {
         QSort(a, first, j);
         first = j + 1;   // QSort(j + 1, last);
      } else {
         QSort(a, j + 1, last);
         last = j;        // QSort(first, j);
      }
   }
}

//______________________________________________________________________________
void TSeqCollection::QSort(TObject **a, TObject **b, Int_t first, Int_t last)
{
   // Sort array a of TObject pointers using a quicksort algorithm.
   // Array b will be sorted just like a (a determines the sort).
   // Uses ObjCompare() to compare objects.

   R__LOCKGUARD2(gCollectionMutex);
   static TObject *tmp1, *tmp2;
   static int i;           // "static" to save stack space
   int j;

   while (last - first > 1) {
      i = first;
      j = last;
      for (;;) {
         while (++i < last && ObjCompare(a[i], a[first]) < 0)
            ;
         while (--j > first && ObjCompare(a[j], a[first]) > 0)
            ;
         if (i >= j)
            break;

         tmp1 = a[i]; tmp2 = b[i];
         a[i] = a[j]; b[i] = b[j];
         a[j] = tmp1; b[j] = tmp2;
      }
      if (j == first) {
         ++first;
         continue;
      }
      tmp1 = a[first]; tmp2 = b[first];
      a[first] = a[j]; b[first] = b[j];
      a[j] = tmp1;     b[j] = tmp2;
      if (j - first < last - (j + 1)) {
         QSort(a, b, first, j);
         first = j + 1;   // QSort(j + 1, last);
      } else {
         QSort(a, b, j + 1, last);
         last = j;        // QSort(first, j);
      }
   }
}

//______________________________________________________________________________
Long64_t TSeqCollection::Merge(TCollection *list)
{
   // Merge this collection with all collections coming in the input list. The
   // input list must contain other collections of objects compatible with the
   // ones in this collection and ordered in the same manner. For example, if this
   // collection contains a TH1 object and a tree, all collections in the input
   // list have to contain a histogram and a tree. In case the list contains
   // collections, the objects in the input lists must also be collections with
   // the same structure and number of objects.
   //
   // Example
   // =========
   //   this                          list
   // ____________                  ---------------------|
   // | A (TH1F) |  __________      | L1 (TSeqCollection)|- [A1, B1(C1,D1,E1)]
   // | B (TList)|-| C (TTree)|     | L1 (TSeqCollection)|- [A2, B2(C2,D2,E2)]
   // |__________| | D (TH1F) |     | ...                |- [...]
   //              | E (TH1F) |     |____________________|
   //              |__________|

   Long64_t nmerged = 0;
   if (IsEmpty() || !list) {
      Warning("Merge", "list is empty - nothing to merge");
      return 0;
   }
   if (list->IsEmpty()) {
      Warning("Merge", "input list is empty - nothing to merge with");
      return 0;
   }
   TIter nextobject(this);
   TIter nextlist(list);
   TObject *object;
   TObject *objtomerge;
   TObject *collcrt;
   TSeqCollection *templist;
   TMethodCall callEnv;
   Int_t indobj = 0;
   while ((object = nextobject())) {   // loop objects in this collection
      // If current object is not mergeable just skip it
      if (!object->IsA()) {
         indobj++;  // current object non-mergeable, go to next object
         continue;
      }
      callEnv.InitWithPrototype(object->IsA(), "Merge", "TCollection*");
      if (!callEnv.IsValid()) {
         indobj++;  // no Merge() interface, go to next object
         continue;
      }
      // Current object mergeable - get corresponding objects in input lists
      templist = (TSeqCollection*)IsA()->New();
      nextlist.Reset();
      while ((collcrt = nextlist())) {      // loop input lists
         if (!collcrt->InheritsFrom(TSeqCollection::Class())) {
            Error("Merge", "some objects in the input list are not collections - merging aborted");
            delete templist;
            return 0;
         }
         // The next object to be merged with is a collection
         // the iterator skips the 'holes' the collections, we also need to do so.
         objtomerge = ((TSeqCollection*)collcrt)->At(indobj);
         while (objtomerge == 0
                && indobj < ((TSeqCollection*)collcrt)->LastIndex() 
               ) {
            ++indobj;
            objtomerge = ((TSeqCollection*)collcrt)->At(indobj);
         }
         if (object->IsA() != objtomerge->IsA()) {
            Error("Merge", "object of type %s at index %d not matching object of type %s in input list",
                  object->ClassName(), indobj, objtomerge->ClassName());
            delete templist;
            return 0;
         }
         // Add object at index indobj in the temporary list
         templist->Add(objtomerge);
         nmerged++;
      }
      // Merge current object with objects in the temporary list
      callEnv.SetParam((Long_t) templist);
      callEnv.Execute(object);
      delete templist;
      indobj++;
   }
   return nmerged;
}