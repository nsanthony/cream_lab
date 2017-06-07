/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: RooAddModel.cxx 28259 2009-04-16 16:21:16Z wouter $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// 
// RooAddModel is an efficient implementation of a sum of PDFs of the form 
//
//  c_1*PDF_1 + c_2*PDF_2 + ... c_n*PDF_n 
//
// or 
//
//  c_1*PDF_1 + c_2*PDF_2 + ... (1-sum(c_1...c_n-1))*PDF_n 
//
// The first form is for extended likelihood fits, where the
// expected number of events is Sum(i) c_i. The coefficients c_i
// can either be explicitly provided, or, if all components support
// extended likelihood fits, they can be calculated the contribution
// of each PDF to the total number of expected events.
//
// In the second form, the sum of the coefficients is enforced to be one,
// and the coefficient of the last PDF is calculated from that condition.
//
// RooAddPdf relies on each component PDF to be normalized and will perform 
// no normalization other than calculating the proper last coefficient c_n, if requested.
// An (enforced) condition for this assuption is that each PDF_i is independent
// of each coefficient_i.
//
// 

#include "RooFit.h"
#include "RooMsgService.h"

#include "TIterator.h"
#include "TIterator.h"
#include "TList.h"
#include "RooAddModel.h"
#include "RooDataSet.h"
#include "RooRealProxy.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooAddGenContext.h"
#include "RooRealConstant.h"
#include "RooNameReg.h"
#include "RooMsgService.h"

#include "Riostream.h"



ClassImp(RooAddModel)
;


//_____________________________________________________________________________
RooAddModel::RooAddModel() :
  _refCoefNorm("!refCoefNorm","Reference coefficient normalization set",this,kFALSE,kFALSE),
  _refCoefRangeName(0),
  _codeReg(10),
  _snormList(0)
{
  _pdfIter   = _pdfList.createIterator() ;
  _coefIter  = _coefList.createIterator() ;

  _coefCache = new Double_t[10] ;
  _coefErrCount = _errorCount ;
}



//_____________________________________________________________________________
RooAddModel::RooAddModel(const char *name, const char *title, const RooArgList& inPdfList, const RooArgList& inCoefList, Bool_t ownPdfList) :
  RooResolutionModel(name,title,((RooResolutionModel*)inPdfList.at(0))->convVar()),
  _refCoefNorm("!refCoefNorm","Reference coefficient normalization set",this,kFALSE,kFALSE),
  _refCoefRangeName(0),
  _projectCoefs(kFALSE),
  _projCacheMgr(this,10),
  _intCacheMgr(this,10),
  _codeReg(10),
  _pdfList("!pdfs","List of PDFs",this),
  _coefList("!coefficients","List of coefficients",this),
  _haveLastCoef(kFALSE),
  _allExtendable(kFALSE)
{ 
  // Generic constructor from list of PDFs and list of coefficients.
  // Each pdf list element (i) is paired with coefficient list element (i).
  // The number of coefficients must be either equal to the number of PDFs,
  // in which case extended MLL fitting is enabled, or be one less.
  //
  // All PDFs must inherit from RooAbsPdf. All coefficients must inherit from RooAbsReal

  if (inPdfList.getSize()>inCoefList.getSize()+1) {
    coutE(InputArguments) << "RooAddModel::RooAddModel(" << GetName() 
			  << ") number of pdfs and coefficients inconsistent, must have Npdf=Ncoef or Npdf=Ncoef+1" << endl ;
    assert(0) ;
  }

  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ;
 
  // Constructor with N PDFs and N or N-1 coefs
  TIterator* pdfIter = inPdfList.createIterator() ;
  TIterator* coefIter = inCoefList.createIterator() ;
  RooAbsPdf* pdf ;
  RooAbsReal* coef ;

  while((coef = (RooAbsPdf*)coefIter->Next())) {
    pdf = (RooAbsPdf*) pdfIter->Next() ;
    if (!pdf) {
      coutE(InputArguments) << "RooAddModel::RooAddModel(" << GetName() 
			    << ") number of pdfs and coefficients inconsistent, must have Npdf=Ncoef or Npdf=Ncoef+1" << endl ;
      assert(0) ;
    }
    if (!dynamic_cast<RooAbsReal*>(coef)) {
      coutE(InputArguments) << "RooAddModel::RooAddModel(" << GetName() << ") coefficient " << coef->GetName() << " is not of type RooAbsReal, ignored" << endl ;
      continue ;
    }
    if (!dynamic_cast<RooAbsReal*>(pdf)) {
      coutE(InputArguments) << "RooAddModel::RooAddModel(" << GetName() << ") pdf " << pdf->GetName() << " is not of type RooAbsPdf, ignored" << endl ;
      continue ;
    }
    _pdfList.add(*pdf) ;
    _coefList.add(*coef) ;    
  }

  pdf = (RooAbsPdf*) pdfIter->Next() ;
  if (pdf) {
    if (!dynamic_cast<RooAbsReal*>(pdf)) {
      coutE(InputArguments) << "RooAddModel::RooAddModel(" << GetName() << ") last pdf " << coef->GetName() << " is not of type RooAbsPdf, fatal error" << endl ;
      assert(0) ;
    }
    _pdfList.add(*pdf) ;  
  } else {
    _haveLastCoef=kTRUE ;
  }

  delete pdfIter ;
  delete coefIter  ;

  _coefCache = new Double_t[_pdfList.getSize()] ;
  _coefErrCount = _errorCount ;

  if (ownPdfList) {
    _ownedComps.addOwned(_pdfList) ;
  }

}



//_____________________________________________________________________________
RooAddModel::RooAddModel(const RooAddModel& other, const char* name) :
  RooResolutionModel(other,name),
  _refCoefNorm("!refCoefNorm",this,other._refCoefNorm),
  _refCoefRangeName((TNamed*)other._refCoefRangeName),
  _projectCoefs(other._projectCoefs),
  _projCacheMgr(other._projCacheMgr,this),
  _intCacheMgr(other._intCacheMgr,this),
  _codeReg(other._codeReg),
  _pdfList("!pdfs",this,other._pdfList),
  _coefList("!coefficients",this,other._coefList),
  _haveLastCoef(other._haveLastCoef),
  _allExtendable(other._allExtendable)
{
  // Copy constructor

  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ;
  _coefCache = new Double_t[_pdfList.getSize()] ;
  _coefErrCount = _errorCount ;
}



//_____________________________________________________________________________
RooAddModel::~RooAddModel()
{
  // Destructor

  delete _pdfIter ;
  delete _coefIter ;

  if (_coefCache) delete[] _coefCache ;
}



//_____________________________________________________________________________
void RooAddModel::fixCoefNormalization(const RooArgSet& refCoefNorm) 
{
  // By default the interpretation of the fraction coefficients is
  // performed in the contextual choice of observables. This makes the
  // shape of the p.d.f explicitly dependent on the choice of
  // observables. This method instructs RooAddPdf to freeze the
  // interpretation of the coefficients to be done in the given set of
  // observables. If frozen, fractions are automatically transformed
  // from the reference normalization set to the contextual normalization
  // set by ratios of integrals

  if (refCoefNorm.getSize()==0) {
    _projectCoefs = kFALSE ;
    return ;
  }
  _projectCoefs = kTRUE ;  

  _refCoefNorm.removeAll() ;
  _refCoefNorm.add(refCoefNorm) ;

  _projCacheMgr.reset() ;
}



//_____________________________________________________________________________
void RooAddModel::fixCoefRange(const char* rangeName)
{
  // By default the interpretation of the fraction coefficients is
  // performed in the default range. This make the shape of a RooAddPdf
  // explicitly dependent on the range of the observables. To allow
  // a range independent definition of the fraction this function
  // instructs RooAddPdf to freeze its interpretation in the given
  // named range. If the current normalization range is different
  // from the reference range, the appropriate fraction coefficients
  // are automically calculation from the reference fractions using
  // ratios if integrals

  _refCoefRangeName = (TNamed*)RooNameReg::ptr(rangeName) ;
  if (_refCoefRangeName) _projectCoefs = kTRUE ;
}



//_____________________________________________________________________________
RooResolutionModel* RooAddModel::convolution(RooFormulaVar* inBasis, RooAbsArg* owner) const
{
  // Instantiate a clone of this resolution model representing a convolution with given
  // basis function. The owners object name is incorporated in the clones name
  // to avoid multiple convolution objects with the same name in complex PDF structures.
  //
  // RooAddModel will clone all the component models to create a composite convolution object

  // Check that primary variable of basis functions is our convolution variable  
  if (inBasis->findServer(0) != x.absArg()) {
    coutE(InputArguments) << "RooAddModel::convolution(" << GetName() 
			  << ") convolution parameter of basis function and PDF don't match" << endl ;
    ccoutE(InputArguments) << "basis->findServer(0) = " << inBasis->findServer(0) << " " << inBasis->findServer(0)->GetName() << endl ;
    ccoutE(InputArguments) << "x.absArg()           = " << x.absArg() << " " << x.absArg()->GetName() << endl ;
    inBasis->Print("v") ;
    return 0 ;
  }

  TString newName(GetName()) ;
  newName.Append("_conv_") ;
  newName.Append(inBasis->GetName()) ;
  newName.Append("_[") ;
  newName.Append(owner->GetName()) ;
  newName.Append("]") ;

  TString newTitle(GetTitle()) ;
  newTitle.Append(" convoluted with basis function ") ;
  newTitle.Append(inBasis->GetName()) ;

  _pdfIter->Reset() ;
  RooResolutionModel* model ;
  RooArgList modelList ;
  while((model = (RooResolutionModel*)_pdfIter->Next())) {       
    // Create component convolution
    RooResolutionModel* conv = model->convolution(inBasis,owner) ;    
    modelList.add(*conv) ;
  }

  _coefIter->Reset() ;
  RooAbsReal* coef ;
  RooArgList theCoefList ;  
  while((coef = (RooAbsReal*)_coefIter->Next())) {
    theCoefList.add(*coef) ;
  }
    
  RooAddModel* convSum = new RooAddModel(newName,newTitle,modelList,theCoefList,kTRUE) ;
  convSum->changeBasis(inBasis) ;
  return convSum ;
}



//_____________________________________________________________________________
Int_t RooAddModel::basisCode(const char* name) const 
{
  // Return code for basis function representing by 'name' string.
  // The basis code of the first component model will be returned,
  // if the basis is supported by all components. Otherwise 0
  // is returned

  TIterator* mIter = _pdfList.createIterator() ;
  RooResolutionModel* model ;
  Bool_t first(kTRUE), code(0) ;
    while((model = (RooResolutionModel*)mIter->Next())) {
      Int_t subCode = model->basisCode(name) ;
      if (first) {
	code = subCode ;
      } else if (subCode==0) {
	code = 0 ;
      }
  }
  delete mIter ;

  return code ;
}



//_____________________________________________________________________________
RooAddModel::CacheElem* RooAddModel::getProjCache(const RooArgSet* nset, const RooArgSet* iset, const char* rangeName) const
{
  // Retrieve cache element with for calculation of p.d.f value with normalization set nset and integrated over iset
  // in range 'rangeName'. If cache element does not exist, create and fill it on the fly. The cache contains
  // suplemental normalization terms (in case not all added p.d.f.s have the same observables), projection
  // integrals to calculated transformed fraction coefficients when a frozen reference frame is provided
  // and projection integrals for similar transformations when a frozen reference range is provided.

  // Check if cache already exists 
  CacheElem* cache = (CacheElem*) _projCacheMgr.getObj(nset,iset,0,RooNameReg::ptr(rangeName)) ;
  if (cache) {
    return cache ;
  }

  //Create new cache 
  cache = new CacheElem ;

  // *** PART 1 : Create supplemental normalization list ***

  // Retrieve the combined set of dependents of this PDF ;
  RooArgSet *fullDepList = getObservables(nset) ;
  if (iset) {
    fullDepList->remove(*iset,kTRUE,kTRUE) ;
  }    

  // Fill with dummy unit RRVs for now
  _pdfIter->Reset() ;
  _coefIter->Reset() ;
  RooAbsPdf* pdf ;
  RooAbsReal* coef ;
  while((pdf=(RooAbsPdf*)_pdfIter->Next())) {    
    coef=(RooAbsPdf*)_coefIter->Next() ;

    // Start with full list of dependents
    RooArgSet supNSet(*fullDepList) ;

    // Remove PDF dependents
    RooArgSet* pdfDeps = pdf->getObservables(nset) ;
    if (pdfDeps) {
      supNSet.remove(*pdfDeps,kTRUE,kTRUE) ;
      delete pdfDeps ; 
    }

    // Remove coef dependents
    RooArgSet* coefDeps = coef ? coef->getObservables(nset) : 0 ;
    if (coefDeps) {
      supNSet.remove(*coefDeps,kTRUE,kTRUE) ;
      delete coefDeps ;
    }
    
    RooAbsReal* snorm ;
    TString name(GetName()) ;
    name.Append("_") ;
    name.Append(pdf->GetName()) ;
    name.Append("_SupNorm") ;
    if (supNSet.getSize()>0) {
      snorm = new RooRealIntegral(name,"Supplemental normalization integral",RooRealConstant::value(1.0),supNSet) ;
    } else {
      snorm = new RooRealVar(name,"Unit Supplemental normalization integral",1.0) ;
    }
    cache->_suppNormList.addOwned(*snorm) ;
  }

  delete fullDepList ;
    
  if (_verboseEval>1) {
    cxcoutD(Caching) << "RooAddModel::syncSuppNormList(" << GetName() << ") synching supplemental normalization list for norm" << (nset?*nset:RooArgSet()) << endl ;
    if (dologD(Caching)) {
      cache->_suppNormList.Print("v") ;
    }
  }


  // *** PART 2 : Create projection coefficients ***

  // If no projections required stop here
  if (!_projectCoefs || _basis!=0 ) {
    _projCacheMgr.setObj(nset,iset,cache,RooNameReg::ptr(rangeName)) ;
    return cache ;
  }


  // Reduce iset/nset to actual dependents of this PDF
  RooArgSet* nset2 = nset ? getObservables(nset) : new RooArgSet() ;

  // Check if requested transformation is not identity 
  if (!nset2->equals(_refCoefNorm) || _refCoefRangeName !=0 || rangeName !=0 ) {
    
    coutI(Caching)  << "RooAddModel::syncCoefProjList(" << GetName() << ") creating coefficient projection integrals" << endl ;
    ccoutI(Caching) << "  from current normalization: "  ; nset2->Print("1") ;
    ccoutI(Caching) << "          with current range: " << (rangeName?rangeName:"<none>") << endl ;
    ccoutI(Caching) << "  to reference normalization: "  ; _refCoefNorm.Print("1") ; 
    ccoutI(Caching) << "        with reference range: " << (_refCoefRangeName?RooNameReg::str(_refCoefRangeName):"<none>") << endl ;
    
    // Recalculate projection integrals of PDFs 
    _pdfIter->Reset() ;
    RooAbsPdf* thePdf ;

    while((thePdf=(RooAbsPdf*)_pdfIter->Next())) {

      // Calculate projection integral
      RooAbsReal* pdfProj ;
      if (!nset2->equals(_refCoefNorm)) {
	pdfProj = thePdf->createIntegral(*nset2,_refCoefNorm) ;
	pdfProj->setOperMode(operMode()) ;
      } else {
	TString name(GetName()) ;
	name.Append("_") ;
	name.Append(thePdf->GetName()) ;
	name.Append("_ProjectNorm") ;
	pdfProj = new RooRealVar(name,"Unit Projection normalization integral",1.0) ;
      }

      cache->_projList.addOwned(*pdfProj) ;

      // Calculation optional supplemental normalization term
      RooArgSet supNormSet(_refCoefNorm) ;
      RooArgSet* deps = thePdf->getParameters(RooArgSet()) ;
      supNormSet.remove(*deps,kTRUE,kTRUE) ;
      delete deps ;

      RooAbsReal* snorm ;
      TString name(GetName()) ;
      name.Append("_") ;
      name.Append(thePdf->GetName()) ;
      name.Append("_ProjSupNorm") ;
      if (supNormSet.getSize()>0) {
	snorm = new RooRealIntegral(name,"Projection Supplemental normalization integral",
				    RooRealConstant::value(1.0),supNormSet) ;
      } else {
	snorm = new RooRealVar(name,"Unit Projection Supplemental normalization integral",1.0) ;
      }
      cache->_suppProjList.addOwned(*snorm) ;

      // Calculate reference range adjusted projection integral
      RooAbsReal* rangeProj1 ;
      if (_refCoefRangeName && _refCoefNorm.getSize()>0) {
	rangeProj1 = thePdf->createIntegral(_refCoefNorm,_refCoefNorm,RooNameReg::str(_refCoefRangeName)) ;
	rangeProj1->setOperMode(operMode()) ;
      } else {
	TString theName(GetName()) ;
	theName.Append("_") ;
	theName.Append(thePdf->GetName()) ;
	theName.Append("_RangeNorm1") ;
	rangeProj1 = new RooRealVar(theName,"Unit range normalization integral",1.0) ;
      }
      cache->_refRangeProjList.addOwned(*rangeProj1) ;
      

      // Calculate range adjusted projection integral
      RooAbsReal* rangeProj2 ;
      if (rangeName && _refCoefNorm.getSize()>0) {
	rangeProj2 = thePdf->createIntegral(_refCoefNorm,_refCoefNorm,rangeName) ;
	rangeProj2->setOperMode(operMode()) ;
      } else {
	TString theName(GetName()) ;
	theName.Append("_") ;
	theName.Append(thePdf->GetName()) ;
	theName.Append("_RangeNorm2") ;
	rangeProj2 = new RooRealVar(theName,"Unit range normalization integral",1.0) ;
      }
      cache->_rangeProjList.addOwned(*rangeProj2) ;

    }               

  }

  delete nset2 ;

  _projCacheMgr.setObj(nset,iset,cache,RooNameReg::ptr(rangeName)) ;

  return cache ;
}



//_____________________________________________________________________________
void RooAddModel::updateCoefficients(CacheElem& cache, const RooArgSet* nset) const 
{
  // Update the coefficient values in the given cache element: calculate new remainder
  // fraction, normalize fractions obtained from extended ML terms to unity and
  // multiply these the various range and dimensional corrections needed in the
  // current use context

  // cxcoutD(ChangeTracking) << "RooAddModel::updateCoefficients(" << GetName() << ") update coefficients" << endl ;
  
  Int_t i ;

  // Straight coefficients
  if (_allExtendable) {
    
    // coef[i] = expectedEvents[i] / SUM(expectedEvents)
    Double_t coefSum(0) ;
    for (i=0 ; i<_pdfList.getSize() ; i++) {
      _coefCache[i] = ((RooAbsPdf*)_pdfList.at(i))->expectedEvents(_refCoefNorm.getSize()>0?&_refCoefNorm:nset) ;
      coefSum += _coefCache[i] ;
    }
    if (coefSum==0.) {
      coutW(Eval) << "RooAddModel::updateCoefCache(" << GetName() << ") WARNING: total number of expected events is 0" << endl ;
    } else {
      for (i=0 ; i<_pdfList.getSize() ; i++) {
	_coefCache[i] /= coefSum ;
      }			            
    }
    
  } else {
    if (_haveLastCoef) {
      
      // coef[i] = coef[i] / SUM(coef)
      Double_t coefSum(0) ;
      for (i=0 ; i<_coefList.getSize() ; i++) {
	_coefCache[i] = ((RooAbsPdf*)_coefList.at(i))->getVal(nset) ;
	coefSum += _coefCache[i] ;
      }		
      for (i=0 ; i<_coefList.getSize() ; i++) {
	_coefCache[i] /= coefSum ;
      }			
    } else {
      
      // coef[i] = coef[i] ; coef[n] = 1-SUM(coef[0...n-1])
      Double_t lastCoef(1) ;
      for (i=0 ; i<_coefList.getSize() ; i++) {
	_coefCache[i] = ((RooAbsPdf*)_coefList.at(i))->getVal(nset) ;
 	cxcoutD(Caching) << "SYNC: orig coef[" << i << "] = " << _coefCache[i] << endl ;
	lastCoef -= _coefCache[i] ;
      }			
      _coefCache[_coefList.getSize()] = lastCoef ;
      cxcoutD(Caching) << "SYNC: orig coef[" << _coefList.getSize() << "] = " << _coefCache[_coefList.getSize()] << endl ;
      
      
      // Warn about coefficient degeneration
      if ((lastCoef<-1e-05 || (lastCoef-1)>1e-5) && _coefErrCount-->0) {
	coutW(Eval) << "RooAddModel::updateCoefCache(" << GetName() 
		    << " WARNING: sum of PDF coefficients not in range [0-1], value=" 
		    << 1-lastCoef << endl ;
	if (_coefErrCount==0) {
	  coutW(Eval) << " (no more will be printed)" << endl  ;
	}
      } 
    }
  }

  

  // Stop here if not projection is required or needed
  if ((!_projectCoefs) || cache._projList.getSize()==0) {
    //     cout << "SYNC no projection required rangeName = " << (rangeName?rangeName:"<none>") << endl ;
    return ;
  }

  // Adjust coefficients for given projection
  Double_t coefSum(0) ;
  for (i=0 ; i<_pdfList.getSize() ; i++) {
    RooAbsPdf::globalSelectComp(kTRUE) ;    

    RooAbsReal* pp = ((RooAbsReal*)cache._projList.at(i)) ; 
    RooAbsReal* sn = ((RooAbsReal*)cache._suppProjList.at(i)) ; 
    RooAbsReal* r1 = ((RooAbsReal*)cache._refRangeProjList.at(i)) ;
    RooAbsReal* r2 = ((RooAbsReal*)cache._rangeProjList.at(i)) ;
    
    if (dologD(Eval)) {
      cxcoutD(Eval) << "pp = " << pp->GetName() << endl 
		    << "sn = " << sn->GetName() << endl 
		    << "r1 = " << r1->GetName() << endl 
		    << "r2 = " << r2->GetName() << endl ;
      r1->printStream(ccoutD(Eval),kName|kArgs|kValue,kSingleLine) ;
      r1->printCompactTree(ccoutD(Eval)) ;
    }

    Double_t proj = pp->getVal()/sn->getVal()*(r2->getVal()/r1->getVal()) ;  
    
    RooAbsPdf::globalSelectComp(kFALSE) ;

    _coefCache[i] *= proj ;
    coefSum += _coefCache[i] ;
  }
  for (i=0 ; i<_pdfList.getSize() ; i++) {
    _coefCache[i] /= coefSum ;
//     cout << "POST-SYNC coef[" << i << "] = " << _coefCache[i] << endl ;
  }
   

  
}



//_____________________________________________________________________________
Double_t RooAddModel::evaluate() const 
{
  // Calculate the current value
  const RooArgSet* nset = _normSet ; 
  CacheElem* cache = getProjCache(nset) ;

  updateCoefficients(*cache,nset) ;

  
  // Do running sum of coef/pdf pairs, calculate lastCoef.
  _pdfIter->Reset() ;
  _coefIter->Reset() ;
  RooAbsPdf* pdf ;

  Double_t snormVal ;
  Double_t value(0) ;
  Int_t i(0) ;
  while((pdf = (RooAbsPdf*)_pdfIter->Next())) {
    if (_coefCache[i]!=0.) {
      snormVal = nset ? ((RooAbsReal*)cache->_suppNormList.at(i))->getVal() : 1.0 ;
      Double_t pdfVal = pdf->getVal(nset) ;
      // Double_t pdfNorm = pdf->getNorm(nset) ;
      if (pdf->isSelectedComp()) {
	value += pdfVal*_coefCache[i]/snormVal ;
 	cxcoutD(Eval) << "RooAddModel::evaluate(" << GetName() << ")  value += [" 
 			<< pdf->GetName() << "] " << pdfVal << " * " << _coefCache[i] << " / " << snormVal << endl ;
      }
    }
    i++ ;
  }

  return value ;
}



//_____________________________________________________________________________
void RooAddModel::resetErrorCounters(Int_t resetValue)
{
  // Reset error counter to given value, limiting the number
  // of future error messages for this pdf to 'resetValue'
  RooAbsPdf::resetErrorCounters(resetValue) ;
  _coefErrCount = resetValue ;
}



//_____________________________________________________________________________
Bool_t RooAddModel::checkObservables(const RooArgSet* nset) const 
{
  // Check if PDF is valid for given normalization set.
  // Coeffient and PDF must be non-overlapping, but pdf-coefficient 
  // pairs may overlap each other

  Bool_t ret(kFALSE) ;

  _pdfIter->Reset() ;
  _coefIter->Reset() ;
  RooAbsReal* coef ;
  RooAbsReal* pdf ;
  while((coef=(RooAbsReal*)_coefIter->Next())) {
    pdf = (RooAbsReal*)_pdfIter->Next() ;
    if (pdf->observableOverlaps(nset,*coef)) {
      coutE(InputArguments) << "RooAddModel::checkObservables(" << GetName() << "): ERROR: coefficient " << coef->GetName() 
			    << " and PDF " << pdf->GetName() << " have one or more dependents in common" << endl ;
      ret = kTRUE ;
    }
  }
  
  return ret ;
}



//_____________________________________________________________________________
Int_t RooAddModel::getAnalyticalIntegralWN(RooArgSet& allVars, RooArgSet& analVars, 
					 const RooArgSet* normSet, const char* rangeName) const 
{

  if (_forceNumInt) return 0 ;

  // Declare that we can analytically integrate all requested observables
  analVars.add(allVars) ;

  // Retrieve (or create) the required component integral list
  Int_t code ;
  RooArgList *cilist ;
  getCompIntList(normSet,&allVars,cilist,code,rangeName) ;
  
  return code+1 ;
  
}



//_____________________________________________________________________________
void RooAddModel::getCompIntList(const RooArgSet* nset, const RooArgSet* iset, pRooArgList& compIntList, Int_t& code, const char* isetRangeName) const 
{
  // Check if this configuration was created before
  Int_t sterileIdx(-1) ;

  IntCacheElem* cache = (IntCacheElem*) _intCacheMgr.getObj(nset,iset,&sterileIdx,RooNameReg::ptr(isetRangeName)) ;
  if (cache) {
    code = _intCacheMgr.lastIndex() ;
    compIntList = &cache->_intList ;
    
    return ;
  }

  // Create containers for partial integral components to be generated
  cache = new IntCacheElem ;

  // Fill Cache
  _pdfIter->Reset() ;
  RooResolutionModel* model ;
  while ((model=(RooResolutionModel*)_pdfIter->Next())) {
    RooAbsReal* intPdf = model->createIntegral(*iset,nset,0,isetRangeName) ;
    cache->_intList.addOwned(*intPdf) ;
  }

  // Store the partial integral list and return the assigned code ;
  code = _intCacheMgr.setObj(nset,iset,(RooAbsCacheElement*)cache,RooNameReg::ptr(isetRangeName)) ;

  // Fill references to be returned
  compIntList = &cache->_intList ;
}



//_____________________________________________________________________________
Double_t RooAddModel::analyticalIntegralWN(Int_t code, const RooArgSet* normSet, const char* rangeName) const 
{
  // Return analytical integral defined by given scenario code

  // No integration scenario
  if (code==0) {
    return getVal(normSet) ;
  }

  // Partial integration scenarios
  IntCacheElem* cache = (IntCacheElem*) _intCacheMgr.getObjByIndex(code-1) ;
  
  RooArgList* compIntList ;

  // If cache has been sterilized, revive this slot
  if (cache==0) {
    RooArgSet* vars = getParameters(RooArgSet()) ;
    RooArgSet* nset = _intCacheMgr.nameSet1ByIndex(code-1)->select(*vars) ;
    RooArgSet* iset = _intCacheMgr.nameSet2ByIndex(code-1)->select(*vars) ;

    Int_t code2(-1) ;
    getCompIntList(nset,iset,compIntList,code2,rangeName) ;

    delete vars ;
    delete nset ;
    delete iset ;
  } else {

    compIntList = &cache->_intList ;

  }

  // Calculate the current value
  const RooArgSet* nset = _normSet ; 
  CacheElem* pcache = getProjCache(nset) ;

  updateCoefficients(*pcache,nset) ;
  
  // Do running sum of coef/pdf pairs, calculate lastCoef.
  TIterator* compIntIter = compIntList->createIterator() ;
  _coefIter->Reset() ;
  RooAbsReal* pdfInt ;

  Double_t snormVal ;
  Double_t value(0) ;
  Int_t i(0) ;
  while((pdfInt = (RooAbsReal*)compIntIter->Next())) {
    if (_coefCache[i]!=0.) {
      snormVal = nset ? ((RooAbsReal*)pcache->_suppNormList.at(i))->getVal() : 1.0 ;
      Double_t intVal = pdfInt->getVal(nset) ;
      value += intVal*_coefCache[i]/snormVal ;
      cxcoutD(Eval) << "RooAddModel::evaluate(" << GetName() << ")  value += [" 
		      << pdfInt->GetName() << "] " << intVal << " * " << _coefCache[i] << " / " << snormVal << endl ;
    }
    i++ ;
  }

  delete compIntIter ;
  
  return value ;
  
}



//_____________________________________________________________________________
Double_t RooAddModel::expectedEvents(const RooArgSet* nset) const 
{  
  // Return the number of expected events, which is either the sum of all coefficients
  // or the sum of the components extended terms

  Double_t expectedTotal(0.0);
  RooAbsPdf* pdf ;
    
  if (_allExtendable) {
    
    // Sum of the extended terms
    _pdfIter->Reset() ;
    while((pdf = (RooAbsPdf*)_pdfIter->Next())) {      
      expectedTotal += pdf->expectedEvents(nset) ;
    }   
    
  } else {
    
    // Sum the coefficients
    _coefIter->Reset() ;
    RooAbsReal* coef ;
    while((coef=(RooAbsReal*)_coefIter->Next())) {
      expectedTotal += coef->getVal() ;
    }   
  }

  return expectedTotal;
}



//_____________________________________________________________________________
void RooAddModel::selectNormalization(const RooArgSet* depSet, Bool_t force) 
{
  // Interface function used by test statistics to freeze choice of observables
  // for interpretation of fraction coefficients

  if (!force && _refCoefNorm.getSize()!=0) {
    return ;
  }

  if (!depSet) {
    fixCoefNormalization(RooArgSet()) ;
    return ;
  }

  RooArgSet* myDepSet = getObservables(depSet) ;
  fixCoefNormalization(*myDepSet) ;
  delete myDepSet ;
}



//_____________________________________________________________________________
void RooAddModel::selectNormalizationRange(const char* rangeName, Bool_t force) 
{
  // Interface function used by test statistics to freeze choice of range
  // for interpretation of fraction coefficients

  if (!force && _refCoefRangeName) {
    return ;
  }

  fixCoefRange(rangeName) ;
}



//_____________________________________________________________________________
RooAbsGenContext* RooAddModel::genContext(const RooArgSet &vars, const RooDataSet *prototype, 
					const RooArgSet* auxProto, Bool_t verbose) const 
{
  // Return specialized context to efficiently generate toy events from RooAddPdfs

  return new RooAddGenContext(*this,vars,prototype,auxProto,verbose) ;
}



//_____________________________________________________________________________
Bool_t RooAddModel::isDirectGenSafe(const RooAbsArg& arg) const 
{
  // Direct generation is safe if all components say so
  _pdfIter->Reset() ;
  RooAbsPdf* pdf ;
  while((pdf=(RooAbsPdf*)_pdfIter->Next())) {
    if (!pdf->isDirectGenSafe(arg)) {
      return kFALSE ;
    }
  }
  return kTRUE ;
}



//_____________________________________________________________________________
Int_t RooAddModel::getGenerator(const RooArgSet& directVars, RooArgSet &/*generateVars*/, Bool_t /*staticInitOK*/) const
{
  // Return pseud-code that indicates if all components can do internal generation (1) or not (0)

  _pdfIter->Reset() ;
  RooAbsPdf* pdf ;
  while((pdf=(RooAbsPdf*)_pdfIter->Next())) {
    RooArgSet tmp ;
    if (pdf->getGenerator(directVars,tmp)==0) {
      return 0 ;
    }
  }
  return 1 ;  
}




//_____________________________________________________________________________
void RooAddModel::generateEvent(Int_t /*code*/)
{
  // This function should never be called as RooAddModel implements a custom generator context
  assert(0) ;
}




//_____________________________________________________________________________
RooArgList RooAddModel::CacheElem::containedArgs(Action) 
{
  // List all RooAbsArg derived contents in this cache element

  RooArgList allNodes;
  allNodes.add(_projList) ;
  allNodes.add(_suppProjList) ;
  allNodes.add(_refRangeProjList) ;
  allNodes.add(_rangeProjList) ;

  return allNodes ;
}



//_____________________________________________________________________________
RooArgList RooAddModel::IntCacheElem::containedArgs(Action) 
{
  // List all RooAbsArg derived contents in this cache element

  RooArgList allNodes(_intList) ;
  return allNodes ;
}


//_____________________________________________________________________________
void RooAddModel::printMetaArgs(ostream& os) const 
{
  // Customized printing of arguments of a RooAddModel to more intuitively reflect the contents of the
  // product operator construction

  _pdfIter->Reset() ;
  _coefIter->Reset() ;

  Bool_t first(kTRUE) ;
    
  os << "(" ;
  RooAbsArg* coef, *pdf ;
  while((coef=(RooAbsArg*)_coefIter->Next())) {
    if (!first) {
      os << " + " ;
    } else {
      first = kFALSE ;
    }
    pdf=(RooAbsArg*)_pdfIter->Next() ;
    os << coef->GetName() << " * " << pdf->GetName() ;
  }
  pdf = (RooAbsArg*) _pdfIter->Next() ;
  if (pdf) {
    os << " + [%] * " << pdf->GetName() ;
  }
  os << ") " ;    
}

