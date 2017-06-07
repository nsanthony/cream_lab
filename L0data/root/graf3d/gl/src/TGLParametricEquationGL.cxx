// @(#)root/gl:$Id: TGLParametricEquationGL.cxx 29526 2009-07-20 17:41:53Z matevz $
// Author:  Matevz Tadel, Jun 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TGLParametricEquationGL.h"

#include "TGLParametric.h"
#include "TVirtualPad.h"

#include "TGLSurfacePainter.h"
#include "TGLTF3Painter.h"

#include "TGLRnrCtx.h"
#include "TGLIncludes.h"

//______________________________________________________________________
//
// GL-renderer wrapper for TGLParametricEquation.
// This allows rendering of parametric-equations in standard GL viewer.

ClassImp(TGLParametricEquationGL);

//______________________________________________________________________________
TGLParametricEquationGL::TGLParametricEquationGL() : TGLPlot3D(), fM(0)
{
   // Constructor.
}

//______________________________________________________________________________
TGLParametricEquationGL::~TGLParametricEquationGL()
{
   // Destructor.
}

//______________________________________________________________________________
Bool_t TGLParametricEquationGL::SetModel(TObject* obj, const Option_t* opt)
{
   // Set model object.

   if (SetModelCheckClass(obj, TGLParametricEquation::Class()))
   {
      fM = dynamic_cast<TGLParametricEquation*>(obj);
      SetPainter( new TGLParametricPlot(fM, 0) );
      TString option(opt);
      fPlotPainter->AddOption(option);
      fPlotPainter->InitGeometry();
      return kTRUE;
   }
   return kFALSE;
}

//______________________________________________________________________________
void TGLParametricEquationGL::SetBBox()
{
   // Setup bounding-box.

   fBoundingBox.Set(fPlotPainter->RefBackBox().Get3DBox());
}

//______________________________________________________________________________
void TGLParametricEquationGL::DirectDraw(TGLRnrCtx& /*rnrCtx*/) const
{
   // Render the object.

   fPlotPainter->RefBackBox().FindFrontPoint();
   glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
   glEnable(GL_NORMALIZE);
   fPlotPainter->InitGL();
   fPlotPainter->DrawPlot();
   glPopAttrib();
}
