// @(#)root/eve:$Id: TEveLineGL.cxx 24214 2008-06-11 14:48:35Z matevz $
// Authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TEveLineGL.h"
#include "TEveLine.h"

#include "TGLRnrCtx.h"
#include "TGLIncludes.h"

//==============================================================================
//==============================================================================
// TEveLineGL
//==============================================================================

//______________________________________________________________________________
//
// GL-renderer for TEveLine class.

ClassImp(TEveLineGL);

//______________________________________________________________________________
TEveLineGL::TEveLineGL() : TPointSet3DGL(), fM(0)
{
   // Constructor.

   // fDLCache = false; // Disable display list.
   fMultiColor = kTRUE;
}

/******************************************************************************/

//______________________________________________________________________________
Bool_t TEveLineGL::SetModel(TObject* obj, const Option_t* /*opt*/)
{
   // Set model object.

   if (SetModelCheckClass(obj, TEveLine::Class())) {
      fM = dynamic_cast<TEveLine*>(obj);
      return kTRUE;
   }
   return kFALSE;
}

/******************************************************************************/

//______________________________________________________________________________
void TEveLineGL::DirectDraw(TGLRnrCtx & rnrCtx) const
{
   // Direct GL rendering for TEveLine.

   // printf("TEveLineGL::DirectDraw Style %d, LOD %d\n", rnrCtx.Style(), rnrCtx.LOD());

   TEveLine& q = *fM;
   if (q.Size() <= 0) return;

   TGLUtil::LockColor(); // Keep color from TGLPhysicalShape.
   if (q.fRnrLine) {
      TGLCapabilitySwitch sw_smooth(GL_LINE_SMOOTH, q.fSmooth);
      TGLCapabilitySwitch sw_blend(GL_BLEND, q.fSmooth);
      TGLUtil::RenderPolyLine(q, q.GetP(), q.Size());
   }
   TGLUtil::UnlockColor();
   if (q.fRnrPoints)
      TGLUtil::RenderPolyMarkers(q, q.GetP(), q.Size(),
                                 rnrCtx.GetPickRadius(),
                                 rnrCtx.Selection());
}
