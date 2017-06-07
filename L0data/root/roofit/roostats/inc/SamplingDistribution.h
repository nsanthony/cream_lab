// @(#)root/roostats:$Id: SamplingDistribution.h 31276 2009-11-18 15:06:42Z moneta $

/*************************************************************************
 * Project: RooStats                                                     *
 * Package: RooFit/RooStats                                              *
 * Authors:                                                              *
 *   Kyle Cranmer, Lorenzo Moneta, Gregory Schott, Wouter Verkerke       *
 *************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOSTATS_SamplingDistribution
#define ROOSTATS_SamplingDistribution

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

#include "Rtypes.h"

#include <vector>


namespace RooStats {

 class SamplingDistribution : public TNamed {

   public:

    // Constructor for SamplingDistribution
    SamplingDistribution(const char *name,const char *title, std::vector<Double_t>& samplingDist, const TString varName = "");
    SamplingDistribution(const char *name,const char *title,
			 std::vector<Double_t>& samplingDist, std::vector<Double_t>& sampleWeights, const TString varName = "");


    SamplingDistribution(const char *name,const char *title, const TString varName = "");

    // Default constructor for SamplingDistribution
    SamplingDistribution();
    
    // Destructor of SamplingDistribution
    virtual ~SamplingDistribution();
    
    // get the inverse of the Cumulative distribution function
    Double_t InverseCDF(Double_t pvalue);

    // get the inverse of the Cumulative distribution function
    Double_t InverseCDFInterpolate(Double_t pvalue);

    // get the inverse of the Cumulative distribution function
    // together with the inverse based on sampling variation
    Double_t InverseCDF(Double_t pvalue, Double_t sigmaVariaton, Double_t& inverseVariation);
  
    // merge two sampling distributions
    void Add(SamplingDistribution* other);
    
    // size of samples
    Int_t GetSize() const{return fSamplingDist.size();}

    // Get test statistics values
    std::vector<Double_t> GetSamplingDistribution() const {return fSamplingDist;}
    // Get the sampling weights 
    std::vector<Double_t> GetSampleWeights() const {return fSampleWeights;}

    const TString GetVarName() const {return fVarName;}
    
  private:
    std::vector<Double_t> fSamplingDist; // vector of points for the sampling distribution
    std::vector<Double_t> fSampleWeights; // vector of weights for the samples
    // store a RooRealVar that this distribution corresponds to?

    TString fVarName;
    
  protected:
    
    ClassDef(SamplingDistribution,1)  // Class containing the results of the HybridCalculator
  };
}

#endif
