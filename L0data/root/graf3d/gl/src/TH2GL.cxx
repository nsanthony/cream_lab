// @(#)root/gl:$Id: TH2GL.cxx 29526 2009-07-20 17:41:53Z matevz $
// Author:  Matevz Tadel, Jun 2007

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TH2GL.h"
#include "TH2.h"
#include "TVirtualPad.h"
#include "TAxis.h"
#include "TMath.h"

#include "TGLSurfacePainter.h"
#include "TGLHistPainter.h"
#include "TGLLegoPainter.h"
#include "TGLBoxPainter.h"
#include "TGLTF3Painter.h"
#include "TGLAxisPainter.h"
#include "TGLCamera.h"

#include "TGLRnrCtx.h"

#include "TGLIncludes.h"

//______________________________________________________________________________
//
// Rendering of TH2 and derived classes.
// Interface to plot-painters also used for gl-in-pad.

ClassImp(TH2GL);

//______________________________________________________________________________
TH2GL::TH2GL() :
   TGLPlot3D(), fM(0)
{
   // Constructor.
}

//______________________________________________________________________________
TH2GL::~TH2GL()
{
   // Destructor.
}

//______________________________________________________________________________
Bool_t TH2GL::SetModel(TObject* obj, const Option_t* opt)
{
   // Set model object.

   TString option(opt);
   option.ToLower();

   if (SetModelCheckClass(obj, TH2::Class()))
   {
      fM = dynamic_cast<TH2*>(obj);

      // Plot type
      if (option.Index("surf") != kNPOS)
         SetPainter( new TGLSurfacePainter(fM, 0, &fCoord) );
      else
         SetPainter( new TGLLegoPainter(fM, 0, &fCoord) );

      if (option.Index("sph") != kNPOS)
         fCoord.SetCoordType(kGLSpherical);
      else if (option.Index("pol") != kNPOS)
         fCoord.SetCoordType(kGLPolar);
      else if (option.Index("cyl") != kNPOS)
         fCoord.SetCoordType(kGLCylindrical);

      fPlotPainter->AddOption(option);
      fPlotPainter->InitGeometry();
      return kTRUE;
   }
   return kFALSE;
}

//______________________________________________________________________________
void TH2GL::SetBBox()
{
   // Setup bounding-box.

   fBoundingBox.Set(fPlotPainter->RefBackBox().Get3DBox());
}

//______________________________________________________________________________
void TH2GL::DirectDraw(TGLRnrCtx & rnrCtx) const
{
   // Render the object.

   fPlotPainter->RefBackBox().FindFrontPoint();

   glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);

   glEnable(GL_NORMALIZE);
   glDisable(GL_COLOR_MATERIAL);

   fPlotPainter->InitGL();
   fPlotPainter->DrawPlot();

   glDisable(GL_CULL_FACE);
   glPopAttrib();

   // Axes
   TGLAxisPainterBox axe_painter;
   axe_painter.SetUseAxisColors(kFALSE);
   axe_painter.SetFontMode(TGLFont::kPixmap);
   axe_painter.PlotStandard(rnrCtx, fM, fBoundingBox);
}