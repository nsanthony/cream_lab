// @(#)root/roostats:$Id: MCMCCalculator.h 31276 2009-11-18 15:06:42Z moneta $
// Authors: Kevin Belasco        17/06/2009
// Authors: Kyle Cranmer         17/06/2009
/*************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOSTATS_MCMCCalculator
#define ROOSTATS_MCMCCalculator

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef ROO_ABS_PDF
#include "RooAbsPdf.h"
#endif
#ifndef ROO_ABS_DATA
#include "RooAbsData.h"
#endif
#ifndef ROO_ARG_SET
#include "RooArgSet.h"
#endif
#ifndef ROO_ARG_LIST
#include "RooArgList.h"
#endif
// #ifndef ROO_WORKSPACE
// #include "RooWorkspace.h"
// #endif
#ifndef ROOSTATS_ProposalFunction
#include "RooStats/ProposalFunction.h"
#endif
#ifndef ROOSTATS_IntervalCalculator
#include "RooStats/IntervalCalculator.h"
#endif
#ifndef RooStats_MCMCInterval
#include "RooStats/MCMCInterval.h"
#endif

namespace RooStats {

   class ModelConfig;

   class MCMCCalculator : public IntervalCalculator, public TNamed {

   public:
      // default constructor
      MCMCCalculator();

      // This constructor will set up a basic settings package including a
      // ProposalFunction, number of iterations, burn in steps, confidence
      // level, and interval determination method. Any of these basic
      // settings can be overridden by calling one of the Set...() methods.
      // Force to pass the a prior PDF for the parameter of interest 
      MCMCCalculator(RooAbsData& data, RooAbsPdf& pdf, const RooArgSet& paramsOfInterest, RooAbsPdf & priorPdf );

      // Constructor as before but without a  prior.
      // In this case it is assumed the prior is already included in the model
      // This constructor will set up a basic settings package including a
      // ProposalFunction, number of iterations, burn in steps, confidence
      // level, and interval determination method. Any of these basic
      // settings can be overridden by calling one of the Set...() methods.
      MCMCCalculator(RooAbsData& data, RooAbsPdf& pdf, const RooArgSet& paramsOfInterest );

      // Constructor from a ModelConfig class. 
      // This constructor will set up a basic settings package including a
      // ProposalFunction, number of iterations, burn in steps, confidence
      // level, and interval determination method. Any of these basic
      // settings can be overridden by calling one of the Set...() methods.
      MCMCCalculator(RooAbsData& data, const ModelConfig& model);

      // alternate constructor, no automatic basic settings
      MCMCCalculator(RooAbsData& data, const ModelConfig& model, ProposalFunction& proposalFunction,
                     Int_t numIters, RooArgList* axes = NULL, Double_t size = 0.05);
//       MCMCCalculator(RooWorkspace& ws, RooAbsData& data,  RooAbsPdf& pdf,
//          const RooArgSet& paramsOfInterest, ProposalFunction& proposalFunction,
//          Int_t numIters, RooArgList* axes = NULL, Double_t size = 0.05);

      // alternate constructor, no automatic basic settings
      MCMCCalculator(RooAbsData& data, RooAbsPdf& pdf,
                     const RooArgSet& paramsOfInterest, RooAbsPdf & priorPdf, ProposalFunction& proposalFunction,
         Int_t numIters, RooArgList* axes = NULL, Double_t size = 0.05);

      virtual ~MCMCCalculator() {}

      // Main interface to get a ConfInterval
      virtual MCMCInterval* GetInterval() const;

      // Get the size of the test (eg. rate of Type I error)
      virtual Double_t Size() const {return fSize;}
      // Get the Confidence level for the test
      virtual Double_t ConfidenceLevel() const {return 1.-fSize;}

      virtual void SetModel(const ModelConfig & model); 

      // Set the DataSet if not already there
      virtual void SetData(RooAbsData& data) { fData = &data; }

      // Set the Pdf if not already there
      virtual void SetPdf(RooAbsPdf& pdf) { fPdf = &pdf; }

      // Set the Prior Pdf if not already there
      virtual void SetPriorPdf(RooAbsPdf& pdf) { fPriorPdf = &pdf; }

      // specify the parameters of interest in the interval
      virtual void SetParameters(const RooArgSet& set) { fPOI.removeAll(); fPOI.add(set); }

      // specify the nuisance parameters (eg. the rest of the parameters)
      virtual void SetNuisanceParameters(const RooArgSet& set) {fNuisParams.removeAll(); fNuisParams.add(set);}

      // set the size of the test (rate of Type I error) ( Eg. 0.05 for a 95% Confidence Interval)
      virtual void SetTestSize(Double_t size) {fSize = size;}

      // set the confidence level for the interval (eg. 0.95 for a 95% Confidence Interval)
      virtual void SetConfidenceLevel(Double_t cl) {fSize = 1.-cl;}

      // set the proposal function for suggesting new points for the MCMC
      virtual void SetProposalFunction(ProposalFunction& proposalFunction)
      { fPropFunc = &proposalFunction; }

      // set the number of iterations to run the metropolis algorithm
      virtual void SetNumIters(Int_t numIters)
      { fNumIters = numIters; }

      // set the number of steps in the chain to discard as burn-in,
      // starting from the first
      virtual void SetNumBurnInSteps(Int_t numBurnInSteps)
      { fNumBurnInSteps = numBurnInSteps; }

      // set the number of bins to create for each axis when constructing the interval
      virtual void SetNumBins(Int_t numBins) { fNumBins = numBins; }
      // set which variables to put on each axis
      virtual void SetAxes(RooArgList& axes)
      { fAxes = &axes; }
      // set whether to use kernel estimation to determine the interval
      virtual void SetUseKeys(Bool_t useKeys) { fUseKeys = useKeys; }
      // set whether to use sparse histogram (if using histogram at all)
      virtual void SetUseSparseHist(Bool_t useSparseHist)
      { fUseSparseHist = useSparseHist; }

   protected:

      Double_t fSize;          // size of the test (eg. specified rate of Type I error)
      RooArgSet   fPOI;        // parameters of interest for interval
      RooArgSet   fNuisParams; // nuisance parameters for interval (not really used)
      mutable ProposalFunction* fPropFunc; // Proposal function for MCMC integration
      RooAbsPdf * fPdf;        // pointer to common PDF (owned by the workspace) 
      RooAbsPdf * fPriorPdf;   // pointer to prior  PDF (owned by the workspace) 
      RooAbsData * fData;     // pointer to the data (owned by the workspace)
      Int_t fNumIters; // number of iterations to run metropolis algorithm
      Int_t fNumBurnInSteps; // number of iterations to discard as burn-in, starting from the first
      Int_t fNumBins; // set the number of bins to create for each
                      // axis when constructing the interval
      RooArgList * fAxes; // which variables to put on each axis
      Bool_t fUseKeys; // whether to use kernel estimation to determine interval
      Bool_t fUseSparseHist; // whether to use sparse histogram (if using hist at all)

      void SetupBasicUsage();
      void SetBins(const RooAbsCollection& coll, Int_t numBins) const
      {
         TIterator* it = coll.createIterator();
         RooAbsArg* r;
         while ((r = (RooAbsArg*)it->Next()) != NULL)
            if (dynamic_cast<RooRealVar*>(r))
               ((RooRealVar*)r)->setBins(numBins);
         delete it;
      }

      ClassDef(MCMCCalculator,1) // Markov Chain Monte Carlo calculator for Bayesian credible intervals
   };
}


#endif
