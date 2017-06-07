// @(#)root/roostats:$Id: MCMCIntervalPlot.h 31276 2009-11-18 15:06:42Z moneta $
// Authors: Kevin Belasco        17/06/2009
// Authors: Kyle Cranmer         17/06/2009
/*************************************************************************
 * Project: RooStats                                                     *
 * Package: RooFit/RooStats                                              *
 *************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOSTATS_MCMCIntervalPlot
#define ROOSTATS_MCMCIntervalPlot

#ifndef ROO_PRINTABLE
#include "RooPrintable.h"
#endif
#ifndef ROO_ARG_SET
#include "RooArgSet.h"
#endif
#ifndef ROOT_TNamed
#include "TNamed.h"
#endif
#ifndef ROOT_TH1
#include "TH1.h"
#endif
#ifndef RooStats_MCMCInterval
#include "RooStats/MCMCInterval.h"
#endif
#ifndef ROO_KEYS_PDF
#include "RooNDKeysPdf.h"
#endif
#ifndef ROO_PRODUCT
#include "RooProduct.h"
#endif

namespace RooStats {

   class MCMCIntervalPlot : public TNamed, public RooPrintable {

   public:
      MCMCIntervalPlot();
      MCMCIntervalPlot(MCMCInterval& interval);

      // Destructor of SamplingDistribution
      virtual ~MCMCIntervalPlot();

      void SetMCMCInterval(MCMCInterval& interval);
      void SetLineColor(Color_t color) {fLineColor = color;}
      void SetLineWidth(Int_t width) {fLineWidth = width;}
      //void SetContourColor(Color_t color) {fContourColor = color;}
      void SetShowBurnIn(Bool_t showBurnIn) { fShowBurnIn = showBurnIn; }

      void Draw(const Option_t* options = NULL);

      void DrawPosterior(const Option_t* options = NULL);
      void DrawPosteriorHist(const Option_t* options = NULL, Bool_t scale = kTRUE);
      void DrawPosteriorKeysPdf(const Option_t* options = NULL);
      void DrawPosteriorKeysProduct(const Option_t* options = NULL);

      void DrawInterval(const Option_t* options = NULL);
      void DrawHistInterval(const Option_t* options = NULL);
      void DrawKeysPdfInterval(const Option_t* options = NULL);

      void DrawChainScatter(RooRealVar& xVar, RooRealVar& yVar);
      void DrawParameterVsTime(RooRealVar& param);
      void DrawNLLVsTime();
      void DrawNLLHist(const Option_t* options = NULL);
      void DrawWeightHist(const Option_t* options = NULL);

   private:

      MCMCInterval *fInterval;
      RooArgSet *fParameters;
      TH1* fPosteriorHist;
      RooNDKeysPdf* fPosteriorKeysPdf;
      RooProduct* fPosteriorKeysProduct;
      TH1* fNLLHist;
      TH1* fWeightHist;
      Int_t fDimension;
      Color_t fLineColor;
      Int_t fLineWidth;
      //Color_t fContourColor;
      Bool_t fShowBurnIn;
      TGraph* fWalk;
      TGraph* fBurnIn;
      TGraph* fFirst;
      TGraph* fParamGraph;
      TGraph* fNLLGraph;

   protected:

      ClassDef(MCMCIntervalPlot,1)  // Class containing the results of the MCMCCalculator
   };
}

#endif
