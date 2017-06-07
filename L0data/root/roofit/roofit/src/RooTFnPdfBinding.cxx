/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here...

#include "Riostream.h" 

#include "RooTFnPdfBinding.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include "TF3.h"

ClassImp(RooTFnPdfBinding) 

RooTFnPdfBinding::RooTFnPdfBinding(const char *name, const char *title, TF1* _func, const RooArgList& _list) :
  RooAbsPdf(name,title), 
  list("params","params",this),
  func(_func)
{ 
  list.add(_list) ;
} 


RooTFnPdfBinding::RooTFnPdfBinding(const RooTFnPdfBinding& other, const char* name) :  
  RooAbsPdf(other,name), 
  list("params",this,other.list),
  func(other.func)
{ 
} 



Double_t RooTFnPdfBinding::evaluate() const 
{ 
  Double_t x = list.at(0) ? ((RooAbsReal*)list.at(0))->getVal() : 0 ;
  Double_t y = list.at(1) ? ((RooAbsReal*)list.at(1))->getVal() : 0 ;
  Double_t z = list.at(2) ? ((RooAbsReal*)list.at(2))->getVal() : 0 ;
  return func->Eval(x,y,z) ;
} 



void RooTFnPdfBinding::printArgs(ostream& os) const 
{
  // Print object arguments and name/address of function pointer
  os << "[ TFn={" << func->GetName() << "=" << func->GetTitle() << "} " ;    
  for (Int_t i=0 ; i<numProxies() ; i++) {
    RooAbsProxy* p = getProxy(i) ;
    if (!TString(p->name()).BeginsWith("!")) {
      p->print(os) ;
      os << " " ;
    }
  }    
  os << "]" ;  
}




namespace RooFit {

  RooAbsPdf* bindPdf(TF1* func,RooAbsReal& x) {
    return new RooTFnPdfBinding(func->GetName(),func->GetName(),func,x) ;
  }

  RooAbsPdf* bindPdf(TF2* func,RooAbsReal& x, RooAbsReal& y) {
    return new RooTFnPdfBinding(func->GetName(),func->GetName(),func,RooArgList(x,y)) ;
  }

  RooAbsPdf* bindPdf(TF3* func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) {
    return new RooTFnPdfBinding(func->GetName(),func->GetName(),func,RooArgList(x,y,z)) ;
  }

}
