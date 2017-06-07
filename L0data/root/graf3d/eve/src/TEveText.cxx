// @(#)root/eve:$Id: TEveText.cxx 30702 2009-10-13 13:55:31Z matevz $
// Authors: Alja & Matevz Tadel 2008

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TEveText.h"
#include "TEveTrans.h"

#include "TGLFontManager.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TString.h"
#include "TBuffer3D.h"
#include "TBuffer3DTypes.h"
#include "TVirtualPad.h"
#include "TVirtualViewer3D.h"
#include "TMath.h"

//==============================================================================
//==============================================================================
// TEveText
//==============================================================================

//______________________________________________________________________________
//
// TEveElement class used for displaying FreeType GL fonts. Holds a
// set of parameters to define FTGL font and its rendering style.
//

ClassImp(TEveText);

//______________________________________________________________________________
TEveText::TEveText(const char* txt) :
   TEveElement(fTextColor),
   TNamed("TEveText", ""),
   TAtt3D(),
   TAttBBox(),
   fText(txt),
   fTextColor(0),

   fFontSize(12),
   fFontFile(4),
   fFontMode(-1),
   fExtrude(1.0f),

   fAutoLighting(kTRUE),
   fLighting(kFALSE)
{
   // Constructor.

   fPolygonOffset[0] = 0;
   fPolygonOffset[1] = 0;

   InitMainTrans();
   SetFontMode(TGLFont::kPixmap);
}

//______________________________________________________________________________
void TEveText::SetFontSize(Int_t val, Bool_t validate)
{
   // Set valid font size.

   if (validate) {
      Int_t* fsp = &TGLFontManager::GetFontSizeArray()->front();
      Int_t  ns  = TGLFontManager::GetFontSizeArray()->size();
      Int_t  idx = TMath::BinarySearch(ns, fsp, val);
      fFontSize = fsp[idx];
   } else {
      fFontSize = val;
   }
}

//______________________________________________________________________________
void TEveText::SetFontFile(const char* name)
{
   // Set font file regarding to static TGLFontManager fgFontFileArray.

   TObjArray* fa =TGLFontManager::GetFontFileArray();
   TIter  next_base(fa);
   TObjString* os;
   Int_t idx = 0;
   while ((os = (TObjString*) next_base()) != 0) {
      if (os->GetString() == name) {
         SetFontFile(idx);
         return;
      }
      idx++;
   }
}

//______________________________________________________________________________
void TEveText::SetFontMode( Int_t mode)
{
   // Set FTFont class ID.

   fFontMode = mode;

   Bool_t edit = (fFontMode > TGLFont::kPixmap);
   TEveTrans& t = RefMainTrans();
   t.SetEditRotation(edit);
   t.SetEditScale(edit);
}

//______________________________________________________________________________
void TEveText::SetPolygonOffset(Float_t factor, Float_t units)
{
   // Set the scale and units used to calculate depth values.
   // See glPolygonOffset manual page.

   fPolygonOffset[0] = factor;
   fPolygonOffset[1] = units;
}

//______________________________________________________________________________
void TEveText::Paint(Option_t* )
{
   // Paint this object. Only direct rendering is supported.

   static const TEveException eH("TEveText::Paint ");

   TBuffer3D buff(TBuffer3DTypes::kGeneric);

   // Section kCore
   buff.fID           = this;
   buff.fColor        = GetMainColor();
   buff.fTransparency = GetMainTransparency();
   if (HasMainTrans())
      RefMainTrans().SetBuffer3D(buff);
   buff.SetSectionsValid(TBuffer3D::kCore);

   Int_t reqSections = gPad->GetViewer3D()->AddObject(buff);
   if (reqSections != TBuffer3D::kNone)
      Error(eH, "only direct GL rendering supported.");
}

//______________________________________________________________________________
void TEveText::ComputeBBox()
{
   // Fill bounding-box information. Virtual from TAttBBox.
   // If member 'TEveFrameBox* fFrame' is set, frame's corners are
   // used as bbox.

   BBoxZero();
}

//______________________________________________________________________________
const TGPicture* TEveText::GetListTreeIcon(Bool_t)
{
   // Return TEveText icon.

   return TEveElement::fgListTreeIcons[5];
}
