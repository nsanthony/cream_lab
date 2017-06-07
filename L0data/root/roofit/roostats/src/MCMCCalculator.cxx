// @(#)root/roostats:$Id: MCMCCalculator.cxx 31876 2009-12-14 11:11:27Z brun $
// Authors: Kevin Belasco        17/06/2009
// Authors: Kyle Cranmer         17/06/2009
/*************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//_________________________________________________
/*
BEGIN_HTML
<p>
MCMCCalculator is a concrete implementation of IntervalCalculator.
It uses a MetropolisHastings object to construct a Markov Chain of data points in the
parameter space.  From this Markov Chain, this class can generate a MCMCInterval as
per user specification.
</p>

<p>
The interface allows one to pass the model, data, and parameters  or eventually 
specify them with names via the ModelConfig class.
</p>

<p>
After configuring the calculator, one only needs to ask GetInterval(), which will
return an ConfInterval (MCMCInterval in this case).
</p>
END_HTML
*/
//_________________________________________________

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
#ifndef ROO_GLOBAL_FUNC
#include "RooGlobalFunc.h"
#endif
#ifndef ROO_ABS_REAL
#include "RooAbsReal.h"
#endif
#ifndef ROO_ARG_SET
#include "RooArgSet.h"
#endif
#ifndef ROO_ARG_LIST
#include "RooArgList.h"
#endif
#ifndef ROOSTATS_ModelConfig
#include "RooStats/ModelConfig.h"
#endif
#ifndef RooStats_RooStatsUtils
#include "RooStats/RooStatsUtils.h"
#endif
#ifndef ROOSTATS_MCMCCalculator
#include "RooStats/MCMCCalculator.h"
#endif
#ifndef ROOSTATS_MetropolisHastings
#include "RooStats/MetropolisHastings.h"
#endif
#ifndef ROOSTATS_MarkovChain
#include "RooStats/MarkovChain.h"
#endif
#ifndef RooStats_MCMCInterval
#include "RooStats/MCMCInterval.h"
#endif
#ifndef ROOT_TIterator
#include "TIterator.h"
#endif
#ifndef ROOSTATS_UniformProposal
#include "RooStats/UniformProposal.h"
#endif
#ifndef ROOSTATS_PdfProposal
#include "RooStats/PdfProposal.h"
#endif

#include "RooProdPdf.h"

ClassImp(RooStats::MCMCCalculator);

using namespace RooFit;
using namespace RooStats;

// default constructor
MCMCCalculator::MCMCCalculator() : 
   fPropFunc(0), 
   fPdf(0), 
   fPriorPdf(0),
   fData(0),
   fAxes(0)
{
   // default constructor
   fNumIters = 0;
   fNumBurnInSteps = 0;
   fNumBins = 0;
   fUseKeys = kFALSE;
   fUseSparseHist = kFALSE;
}

MCMCCalculator::MCMCCalculator(RooAbsData& data, RooAbsPdf& pdf,
                               const RooArgSet& paramsOfInterest,
                               RooAbsPdf & prior) : 
   fPOI(paramsOfInterest),
   fPropFunc(0), 
   fPdf(&pdf), 
   fPriorPdf(&prior),
   fData(&data),
   fAxes(0)
{
// Constructor for automatic configuration with basic settings.  Uses a
// UniformProposal,10,000 iterations, 40 burn in steps, 50 bins for each
// RooRealVar, determines interval by keys, and turns on sparse histogram
// mode in the MCMCInterval.  Finds a 95% confidence interval.
   SetupBasicUsage();
}

MCMCCalculator::MCMCCalculator(RooAbsData& data, RooAbsPdf& pdf,
                               const RooArgSet& paramsOfInterest) :
   fPOI(paramsOfInterest),
   fPropFunc(0), 
   fPdf(&pdf), 
   fPriorPdf(0),
   fData(&data),
   fAxes(0)
{
// same constructor as before but not passing a prior pdf
   SetupBasicUsage();
}

MCMCCalculator::MCMCCalculator(RooAbsData& data, const ModelConfig & model) :
   fPropFunc(0), 
   fPdf(model.GetPdf()), 
   fPriorPdf(0),
   fData(&data)
{
// Constructor for automatic configuration with basic settings.  Uses a
// UniformProposal,10,000 iterations, 40 burn in steps, 50 bins for each
// RooRealVar, determines interval by keys, and turns on sparse histogram
// mode in the MCMCInterval.  Finds a 95% confidence interval.
   SetModel(model);
   SetupBasicUsage();
}

MCMCCalculator::MCMCCalculator(RooAbsData& data, const ModelConfig & model,
                               ProposalFunction& proposalFunction, Int_t numIters,
                               RooArgList* axes, Double_t size) : 
   fPropFunc(&proposalFunction), 
   fPdf(model.GetPdf()), 
   fPriorPdf(0),
   fData(&data), 
   fAxes(axes)
{
// alternate constructor, specifying many arguments
   SetModel(model);
   SetTestSize(size);
   fNumIters = numIters;
   fNumBurnInSteps = 0;
   fNumBins = 0;
   fUseKeys = kFALSE;
   fUseSparseHist = kFALSE;
}

void MCMCCalculator::SetModel(const ModelConfig & model) { 
   // set the model
   fPdf = model.GetPdf();  
   fPriorPdf = model.GetPriorPdf();
   fPOI.removeAll();
   fNuisParams.removeAll();
   if (model.GetParametersOfInterest() ) fPOI.add(*model.GetParametersOfInterest());
   if (model.GetNuisanceParameters() )   fNuisParams.add(*model.GetNuisanceParameters());
}

// alternate constructor, specifying many arguments
MCMCCalculator::MCMCCalculator(RooAbsData& data, RooAbsPdf& pdf,
                               const RooArgSet& paramsOfInterest, RooAbsPdf & prior, ProposalFunction& proposalFunction,
                               Int_t numIters, RooArgList* axes, Double_t size) : 
   fPOI(paramsOfInterest),
   fPropFunc(&proposalFunction), 
   fPdf(&pdf), 
   fPriorPdf(&prior),
   fData(&data), 
   fAxes(axes)
{
   SetTestSize(size);
   fNumIters = numIters;
   fNumBurnInSteps = 0;
   fNumBins = 0;
   fAxes = axes;
   fUseKeys = kFALSE;
   fUseSparseHist = kFALSE;
}

void MCMCCalculator::SetupBasicUsage()
{
// Setting automatic configuration with basic settings.  Uses a
// UniformProposal,10,000 iterations, 40 burn in steps, 50 bins for each
// RooRealVar, determines interval by keys, and turns on sparse histogram
// mode in the MCMCInterval.  Finds a 95% confidence interval.
   fPropFunc = 0;
   fNumIters = 10000;
   fNumBurnInSteps = 40;
   //fNumBins = 0;
   fNumBins = 50;
   fUseKeys = kTRUE;
   fUseSparseHist = kTRUE;
   SetTestSize(0.05);
}

MCMCInterval* MCMCCalculator::GetInterval() const
{
   // Main interface to get a RooStats::ConfInterval.  

   if (!fData || !fPdf   ) return 0;
   if (fPOI.getSize() == 0) return 0;

   // if a proposal function has not been specified create a default one
   bool useDefaultPropFunc = (fPropFunc == 0); 
   bool usePriorPdf = (fPriorPdf != 0);
   if (useDefaultPropFunc) fPropFunc = new UniformProposal(); 

   // if prior is given create product 
   RooAbsPdf * prodPdf = fPdf;
   if (usePriorPdf) { 
      TString prodName = TString("product_") + TString(fPdf->GetName()) + TString("_") + TString(fPriorPdf->GetName() );   
      prodPdf = new RooProdPdf(prodName,prodName,RooArgList(*fPdf,*fPriorPdf) );
   }

   RooArgSet* constrainedParams = prodPdf->getParameters(*fData);
   RooAbsReal* nll = prodPdf->createNLL(*fData, Constrain(*constrainedParams));
   delete constrainedParams;

   RooArgSet* params = nll->getParameters(*fData);
   RemoveConstantParameters(params);
   if (fNumBins > 0) {
      SetBins(*params, fNumBins);
      SetBins(fPOI, fNumBins);
      if (dynamic_cast<PdfProposal*>(fPropFunc)) {
         RooArgSet* proposalVars = ((PdfProposal*)fPropFunc)->GetPdf()->
                                               getParameters((RooAbsData*)NULL);
         SetBins(*proposalVars, fNumBins);
      }
   }

   MetropolisHastings mh;
   mh.SetFunction(*nll);
   mh.SetType(MetropolisHastings::kLog);
   mh.SetSign(MetropolisHastings::kNegative);
   mh.SetParameters(*params);
   mh.SetProposalFunction(*fPropFunc);
   mh.SetNumIters(fNumIters);

   MarkovChain* chain = mh.ConstructChain();

   TString name = TString("MCMCInterval_") + TString(GetName() ); 
   MCMCInterval* interval = new MCMCInterval(name, fPOI, *chain);
   if (fAxes != NULL)
      interval->SetAxes(*fAxes);
   if (fNumBurnInSteps > 0)
      interval->SetNumBurnInSteps(fNumBurnInSteps);
   interval->SetUseKeys(fUseKeys);
   interval->SetUseSparseHist(fUseSparseHist);
   interval->SetConfidenceLevel(1.0 - fSize);

   if (useDefaultPropFunc) delete fPropFunc; 
   if (usePriorPdf) delete prodPdf; 
   delete nll; 
   delete params;
   
   return interval;
}
