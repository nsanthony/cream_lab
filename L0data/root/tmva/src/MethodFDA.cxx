// @(#)root/tmva $Id: MethodFDA.cxx 31458 2009-11-30 13:58:20Z stelzer $    
// Author: Andreas Hoecker, Peter Speckmayer, Joerg Stelzer

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : MethodFDA                                                             *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Implementation                                                            *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker  <Andreas.Hocker@cern.ch> - CERN, Switzerland             *
 *      Peter Speckmayer <speckmay@mail.cern.ch>  - CERN, Switzerland             *
 *      Joerg Stelzer    <stelzer@cern.ch>        - DESY, Germany                 *
 *      Maciej Kruk      <mkruk@cern.ch>          - IFJ PAN & AGH, Poland         *
 *                                                                                *
 * Copyright (c) 2005-2006:                                                       *
 *      CERN, Switzerland                                                         *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

//_______________________________________________________________________
//                                                                      
// Function discriminant analysis (FDA). This simple classifier         //
// fits any user-defined TFormula (via option configuration string) to  //
// the training data by requiring a formula response of 1 (0) to signal //
// (background) events. The parameter fitting is done via the abstract  //
// class FitterBase, featuring Monte Carlo sampling, Genetic            //
// Algorithm, Simulated Annealing, MINUIT and combinations of these.    //
//                                                                      //
// Can compute regression value for one dimensional output              //
//_______________________________________________________________________

#include "Riostream.h"
#include "TList.h"
#include "TFormula.h"
#include "TString.h"
#include "TObjString.h"
#include "TRandom3.h"
#include "TMath.h"
#include <sstream>

#include "TMVA/ClassifierFactory.h"
#include "TMVA/MethodFDA.h"
#include "TMVA/Tools.h"
#include "TMVA/Interval.h"
#include "TMVA/Timer.h"
#include "TMVA/GeneticFitter.h"
#include "TMVA/SimulatedAnnealingFitter.h"
#include "TMVA/MinuitFitter.h"
#include "TMVA/MCFitter.h"
#include "TMVA/Config.h"

REGISTER_METHOD(FDA)

ClassImp(TMVA::MethodFDA)

//_______________________________________________________________________
TMVA::MethodFDA::MethodFDA( const TString& jobName,
                            const TString& methodTitle,
                            DataSetInfo& theData, 
                            const TString& theOption,
                            TDirectory* theTargetDir )
   : MethodBase( jobName, Types::kFDA, methodTitle, theData, theOption, theTargetDir ), 
     IFitterTarget   (),
     fFormula        ( 0 ),
     fFitter         ( 0 ),
     fConvergerFitter( 0 )
{
   // standard constructor
}

//_______________________________________________________________________
TMVA::MethodFDA::MethodFDA( DataSetInfo& theData, 
                            const TString& theWeightFile,  
                            TDirectory* theTargetDir )
   : MethodBase( Types::kFDA, theData, theWeightFile, theTargetDir ), 
     IFitterTarget   (),
     fFormula        ( 0 ),
     fFitter         ( 0 ),
     fConvergerFitter( 0 )
{
   // constructor from weight file
}

//_______________________________________________________________________
void TMVA::MethodFDA::Init( void )
{
   // default initialisation
   fNPars    = 0;

   fBestPars.clear();

   fSumOfWeights    = 0;
   fSumOfWeightsSig = 0;
   fSumOfWeightsBkg = 0;

   fFormulaStringP  = "";
   fParRangeStringP = "";
   fFormulaStringT  = "";
   fParRangeStringT = "";

   fFitMethod       = "";
   fConverger       = "";
}

//_______________________________________________________________________
void TMVA::MethodFDA::DeclareOptions() 
{
   // define the options (their key words) that can be set in the option string 
   //
   // format of function string:
   //    "x0*(0)+((1)/x1)**(2)..."
   // where "[i]" are the parameters, and "xi" the input variables
   //
   // format of parameter string:
   //    "(-1.2,3.4);(-2.3,4.55);..."
   // where the numbers in "(a,b)" correspond to the a=min, b=max parameter ranges;
   // each parameter defined in the function string must have a corresponding range
   //
   DeclareOptionRef( fFormulaStringP  = "(0)", "Formula",   "The discrimination formula" );
   DeclareOptionRef( fParRangeStringP = "()", "ParRanges", "Parameter ranges" );

   // fitter
   DeclareOptionRef( fFitMethod = "MINUIT", "FitMethod", "Optimisation Method");
   AddPreDefVal(TString("MC"));
   AddPreDefVal(TString("GA"));
   AddPreDefVal(TString("SA"));
   AddPreDefVal(TString("MINUIT"));

   DeclareOptionRef( fConverger = "None", "Converger", "FitMethod uses Converger to improve result");
   AddPreDefVal(TString("None"));
   AddPreDefVal(TString("MINUIT"));
}

//_______________________________________________________________________
void TMVA::MethodFDA::CreateFormula() 
{
   // translate formula string into TFormula, and parameter string into par ranges

   // process transient strings
   fFormulaStringT  = fFormulaStringP;

   // intepret formula string

   // replace the parameters "(i)" by the TFormula style "[i]"
   for (Int_t ipar=0; ipar<fNPars; ipar++) {
      fFormulaStringT.ReplaceAll( Form("(%i)",ipar), Form("[%i]",ipar) );
   }

   // sanity check, there should be no "(i)", with 'i' a number anymore
   for (Int_t ipar=fNPars; ipar<1000; ipar++) {
      if (fFormulaStringT.Contains( Form("(%i)",ipar) ))
         Log() << kFATAL 
                 << "<CreateFormula> Formula contains expression: \"" << Form("(%i)",ipar) << "\", "
                 << "which cannot be attributed to a parameter; " 
                 << "it may be that the number of variable ranges given via \"ParRanges\" "
                 << "does not match the number of parameters in the formula expression, please verify!"
                 << Endl;
   }

   // write the variables "xi" as additional parameters "[npar+i]"
   for (Int_t ivar=GetNvar()-1; ivar >= 0; ivar--) {
      fFormulaStringT.ReplaceAll( Form("x%i",ivar), Form("[%i]",ivar+fNPars) );
   }

   // sanity check, there should be no "xi", with 'i' a number anymore
   for (UInt_t ivar=GetNvar(); ivar<1000; ivar++) {
      if (fFormulaStringT.Contains( Form("x%i",ivar) ))
         Log() << kFATAL 
                 << "<CreateFormula> Formula contains expression: \"" << Form("x%i",ivar) << "\", "
                 << "which cannot be attributed to an input variable" << Endl;
   }
   
   Log() << "User-defined formula string       : \"" << fFormulaStringP << "\"" << Endl;
   Log() << "TFormula-compatible formula string: \"" << fFormulaStringT << "\"" << Endl;
   Log() << "Creating and compiling formula" << Endl;
   
   // create TF1
   if (fFormula) delete fFormula;
   fFormula = new TFormula( "FDA_Formula", fFormulaStringT );
   
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,2,0)
   fFormula->Optimize();
#endif

   // is formula correct ?
   if (fFormula->Compile() != 0)
      Log() << kFATAL << "<ProcessOptions> Formula expression could not be properly compiled" << Endl;

   // other sanity checks
   if (fFormula->GetNpar() > fNPars + (Int_t)GetNvar())
      Log() << kFATAL << "<ProcessOptions> Dubious number of parameters in formula expression: " 
              << fFormula->GetNpar() << " - compared to maximum allowed: " << fNPars + GetNvar() << Endl;
}

//_______________________________________________________________________
void TMVA::MethodFDA::ProcessOptions() 
{
   // the option string is decoded, for availabel options see "DeclareOptions"

   // process transient strings
   fParRangeStringT = fParRangeStringP;

   // interpret parameter string   
   fParRangeStringT.ReplaceAll( " ", "" );
   fNPars = fParRangeStringT.CountChar( ')' );

   TList* parList = gTools().ParseFormatLine( fParRangeStringT, ";" );
   if (parList->GetSize() != fNPars) {
      Log() << kFATAL << "<ProcessOptions> Mismatch in parameter string: " 
              << "the number of parameters: " << fNPars << " != ranges defined: " 
              << parList->GetSize() << "; the format of the \"ParRanges\" string "
              << "must be: \"(-1.2,3.4);(-2.3,4.55);...\", "
              << "where the numbers in \"(a,b)\" correspond to the a=min, b=max parameter ranges; "
              << "each parameter defined in the function string must have a corresponding rang."
              << Endl;
   }

   fParRange.resize( fNPars );
   for (Int_t ipar=0; ipar<fNPars; ipar++) fParRange[ipar] = 0;

   for (Int_t ipar=0; ipar<fNPars; ipar++) {
      // parse (a,b)
      TString str = ((TObjString*)parList->At(ipar))->GetString();
      Ssiz_t istr = str.First( ',' );
      TString pminS(str(1,istr-1));
      TString pmaxS(str(istr+1,str.Length()-2-istr));      

      stringstream stmin; Float_t pmin; stmin << pminS.Data(); stmin >> pmin;       
      stringstream stmax; Float_t pmax; stmax << pmaxS.Data(); stmax >> pmax;

      // sanity check
      if (TMath::Abs(pmax-pmin) < 1.e-30) pmax = pmin;
      if (pmin > pmax) Log() << kFATAL << "<ProcessOptions> max > min in interval for parameter: [" 
                               << ipar << "] : [" << pmin  << ", " << pmax << "] " << Endl;

      fParRange[ipar] = new Interval( pmin, pmax );
   }
   delete parList;

   // create formula
   CreateFormula();

   // create minimiser
   fConvergerFitter = (IFitterTarget*)this;
   if (fConverger == "MINUIT") {
      fConvergerFitter = new MinuitFitter( *this, Form("%s_Converger_Minuit", GetName()), fParRange, GetOptions() );
      SetOptions(dynamic_cast<Configurable*>(fConvergerFitter)->GetOptions());
   }

   if      (fFitMethod == "MC")     
      fFitter = new MCFitter( *fConvergerFitter, Form("%s_Fitter_MC", GetName()), fParRange, GetOptions() );
   else if (fFitMethod == "GA")     
      fFitter = new GeneticFitter( *fConvergerFitter, Form("%s_Fitter_GA", GetName()), fParRange, GetOptions() );
   else if (fFitMethod == "SA")     
      fFitter = new SimulatedAnnealingFitter( *fConvergerFitter, Form("%s_Fitter_SA", GetName()), fParRange, GetOptions() );
   else if (fFitMethod == "MINUIT") 
      fFitter = new MinuitFitter( *fConvergerFitter, Form("%s_Fitter_Minuit", GetName()), fParRange, GetOptions() );
   else {
      Log() << kFATAL << "<Train> Do not understand fit method:" << fFitMethod << Endl;
   }
   
   fFitter->CheckForUnusedOptions();
}

//_______________________________________________________________________
TMVA::MethodFDA::~MethodFDA( void )
{
   // destructor
   ClearAll();
}

//_______________________________________________________________________
Bool_t TMVA::MethodFDA::HasAnalysisType( Types::EAnalysisType type, UInt_t numberClasses, UInt_t numberTargets )
{
   // FDA can handle classification with 2 classes and regression with one regression-target
   if (type == Types::kClassification && numberClasses == 2) return kTRUE;
   if (type == Types::kRegression     && numberTargets == 1) return kTRUE;
   return kFALSE;
}


//_______________________________________________________________________
void TMVA::MethodFDA::ClearAll( void )
{
   // delete and clear all class members
   for (UInt_t ipar=0; ipar<fParRange.size(); ipar++) {
      if (fParRange[ipar] != 0) { delete fParRange[ipar]; fParRange[ipar] = 0; }
   }
   fParRange.clear(); 
   
   if (fFormula  != 0) { delete fFormula; fFormula = 0; }
   fBestPars.clear();
}

//_______________________________________________________________________
void TMVA::MethodFDA::Train( void )
{
   // FDA training 

   // cache training events
   fSumOfWeights    = 0;
   fSumOfWeightsSig = 0;
   fSumOfWeightsBkg = 0;

   for (UInt_t ievt=0; ievt<GetNEvents(); ievt++) {

      // read the training event 
      const Event* ev = GetEvent(ievt);

      // true event copy
      Float_t w  = GetTWeight(ev);

      if (!DoRegression()) {
         if (ev->IsSignal()) { fSumOfWeightsSig += w; }
         else                { fSumOfWeightsBkg += w; }
      }
      fSumOfWeights += w;
   }

   // sanity check
   if (!DoRegression()) {
      if (fSumOfWeightsSig <= 0 || fSumOfWeightsBkg <= 0) {
         Log() << kFATAL << "<Train> Troubles in sum of weights: " 
                 << fSumOfWeightsSig << " (S) : " << fSumOfWeightsBkg << " (B)" << Endl;
      }
   }
   else if (fSumOfWeights <= 0) {
      Log() << kFATAL << "<Train> Troubles in sum of weights: " 
              << fSumOfWeights << Endl;
   }

   // starting values (not used by all fitters)
   fBestPars.clear();
   for (std::vector<Interval*>::const_iterator parIt = fParRange.begin(); parIt != fParRange.end(); parIt++) {
      fBestPars.push_back( (*parIt)->GetMean() );
   }

   // execute the fit
   Double_t estimator = fFitter->Run( fBestPars );
      
   // print results
   PrintResults( fFitMethod, fBestPars, estimator );

   delete fFitter; fFitter = 0;
   if (fConvergerFitter!=0 && fConvergerFitter!=(IFitterTarget*)this) {
      delete fConvergerFitter;
      fConvergerFitter = 0;
   }
}

//_______________________________________________________________________
void TMVA::MethodFDA::PrintResults( const TString& fitter, std::vector<Double_t>& pars, const Double_t estimator ) const
{
   // display fit parameters
   // check maximum length of variable name
   Log() << kINFO;
   Log() << "Results for parameter fit using \"" << fitter << "\" fitter:" << Endl;
   vector<TString>  parNames;
   for (UInt_t ipar=0; ipar<pars.size(); ipar++) parNames.push_back( Form("Par(%i)",ipar ) );
   gTools().FormattedOutput( pars, parNames, "Parameter" , "Fit result", Log(), "%g" );   
   Log() << "Discriminator expression: \"" << fFormulaStringP << "\"" << Endl;
   Log() << "Value of estimator at minimum: " << estimator << Endl;
}


//_______________________________________________________________________
Double_t TMVA::MethodFDA::EstimatorFunction( std::vector<Double_t>& pars )
{
   // compute estimator for given parameter set (to be minimised)
   //   const Double_t sumOfWeights[]                = { fSumOfWeightsSig, fSumOfWeightsBkg, fSumOfWeights };
   const Double_t sumOfWeights[]                = { fSumOfWeightsBkg, fSumOfWeightsSig, fSumOfWeights };
   Double_t estimator[]                         = { 0, 0, 0 };

   Double_t result, deviation;
   Double_t desired = 0.0;
   for (UInt_t ievt=0; ievt<GetNEvents(); ievt++) {

      // read the training event 
      const Event* ev = GetEvent(ievt);

      // calculate the deviation from the desired value

      if (!DoRegression()) desired = (ev->IsSignal() ? 1.0 : 0.0);
      else                 desired = ev->GetTarget( 0 );

      result    = InterpretFormula( ev, pars );
      deviation = TMath::Power(result - desired, 2);

      if (!DoRegression())  estimator[Int_t(desired)] += deviation * ev->GetWeight();
      else                  estimator[2]              += deviation * ev->GetWeight();

   }
   estimator[0] /= sumOfWeights[0];
   estimator[1] /= sumOfWeights[1];
   if (DoRegression()) estimator[2] /= sumOfWeights[2];
   // return value is sum over normalised signal and background contributions

   if (!DoRegression()) return estimator[0] + estimator[1];
   else                 return estimator[2];
}

//_______________________________________________________________________
Double_t TMVA::MethodFDA::InterpretFormula( const Event* event, std::vector<Double_t>& pars )
{
   // formula interpretation
   for (UInt_t ipar=0; ipar<pars.size(); ipar++) fFormula->SetParameter( ipar, pars[ipar] );
   for (UInt_t ivar=0;  ivar<GetNvar();  ivar++) fFormula->SetParameter( fNPars+ivar, event->GetValue(ivar) );

   return fFormula->Eval( 0 );
}

//_______________________________________________________________________
Double_t TMVA::MethodFDA::GetMvaValue( Double_t* err )
{
   // returns MVA value for given event
   const Event* ev = GetEvent();

   // cannot determine error
   if (err != 0) *err = -1;
   
   return InterpretFormula( ev, fBestPars );
}

//_______________________________________________________________________
std::vector<Float_t>& TMVA::MethodFDA::GetRegressionValues()
{
   if (fRegressionReturnVal == NULL) fRegressionReturnVal = new std::vector<Float_t>();
   fRegressionReturnVal->clear();

   const Event* ev = GetEvent();

   Event* evT = new Event(*ev);
   evT->SetTarget(0,InterpretFormula( ev, fBestPars ));
   const Event* evT2 = GetTransformationHandler().InverseTransform( evT );
   fRegressionReturnVal->push_back(evT2->GetTarget(0));

   delete evT;

   return (*fRegressionReturnVal);
}
  
//_______________________________________________________________________
void  TMVA::MethodFDA::ReadWeightsFromStream( istream& istr )
{
   // read back the training results from a file (stream)

   // retrieve best function parameters
   istr >> fNPars;

   fBestPars.clear();
   fBestPars.resize( fNPars );
   for (Int_t ipar=0; ipar<fNPars; ipar++) istr >> fBestPars[ipar];
}

//_______________________________________________________________________
void TMVA::MethodFDA::AddWeightsXMLTo( void* parent ) const 
{
   // create XML description for LD classification and regression 
   // (for arbitrary number of output classes/targets)

   void* wght = gTools().xmlengine().NewChild(parent, 0, "Weights");
   gTools().AddAttr( wght, "NPars",  fNPars );
   for (Int_t ipar=0; ipar<fNPars; ipar++) {
      void* coeffxml = gTools().xmlengine().NewChild( wght, 0, "Parameter" );
      gTools().AddAttr( coeffxml, "Index", ipar   );
      gTools().AddAttr( coeffxml, "Value", fBestPars[ipar] );
   }

   // write formula
   gTools().AddAttr( wght, "Formula", fFormulaStringP );
}
  
//_______________________________________________________________________
void TMVA::MethodFDA::ReadWeightsFromXML( void* wghtnode ) 
{
   // read coefficients from xml weight file
   gTools().ReadAttr( wghtnode, "NPars", fNPars );

   fBestPars.clear();
   fBestPars.resize( fNPars );
   
   void* ch = gTools().xmlengine().GetChild(wghtnode);
   Double_t par;
   Int_t    ipar;
   while (ch) {
      gTools().ReadAttr( ch, "Index", ipar );
      gTools().ReadAttr( ch, "Value", par  );

      // sanity check
      if (ipar >= fNPars) Log() << kFATAL << "<ReadWeightsFromXML> index out of range: "
                                  << ipar << " >= " << fNPars << Endl;
      fBestPars[ipar] = par;

      ch = gTools().xmlengine().GetNext(ch);
   }

   // read formula
   gTools().ReadAttr( wghtnode, "Formula", fFormulaStringP );

   // create the TFormula
   CreateFormula();
}

//_______________________________________________________________________
void TMVA::MethodFDA::MakeClassSpecific( std::ostream& fout, const TString& className ) const
{
   // write FDA-specific classifier response
   fout << "   double              fParameter[" << fNPars << "];" << endl;
   fout << "};" << endl;
   fout << "" << endl;
   fout << "inline void " << className << "::Initialize() " << endl;
   fout << "{" << endl;
   for (Int_t ipar=0; ipar<fNPars; ipar++) {
      fout << "   fParameter[" << ipar << "] = " << fBestPars[ipar] << ";" << endl;
   }
   fout << "}" << endl;
   fout << endl;
   fout << "inline double " << className << "::GetMvaValue__( const std::vector<double>& inputValues ) const" << endl;
   fout << "{" << endl;
   fout << "   // interpret the formula" << endl;

   // replace parameters
   TString str = fFormulaStringT;
   for (Int_t ipar=0; ipar<fNPars; ipar++) {
      str.ReplaceAll( Form("[%i]", ipar), Form("fParameter[%i]", ipar) );
   }
   
   // replace input variables
   for (UInt_t ivar=0; ivar<GetNvar(); ivar++) {
      str.ReplaceAll( Form("[%i]", ivar+fNPars), Form("inputValues[%i]", ivar) );
   }

   fout << "   double retval = " << str << ";" << endl;
   fout << endl;
   fout << "   return retval; " << endl;
   fout << "}" << endl;
   fout << endl;
   fout << "// Clean up" << endl;
   fout << "inline void " << className << "::Clear() " << endl;
   fout << "{" << endl;
   fout << "   // nothing to clear" << endl;
   fout << "}" << endl;
}

//_______________________________________________________________________
void TMVA::MethodFDA::GetHelpMessage() const
{
   // get help message text
   //
   // typical length of text line: 
   //         "|--------------------------------------------------------------|"
   Log() << Endl;
   Log() << gTools().Color("bold") << "--- Short description:" << gTools().Color("reset") << Endl;
   Log() << Endl;
   Log() << "The function discriminant analysis (FDA) is a classifier suitable " << Endl;
   Log() << "to solve linear or simple nonlinear discrimination problems." << Endl; 
   Log() << Endl;
   Log() << "The user provides the desired function with adjustable parameters" << Endl;
   Log() << "via the configuration option string, and FDA fits the parameters to" << Endl;
   Log() << "it, requiring the signal (background) function value to be as close" << Endl;
   Log() << "as possible to 1 (0). Its advantage over the more involved and" << Endl;
   Log() << "automatic nonlinear discriminators is the simplicity and transparency " << Endl;
   Log() << "of the discrimination expression. A shortcoming is that FDA will" << Endl;
   Log() << "underperform for involved problems with complicated, phase space" << Endl;
   Log() << "dependent nonlinear correlations." << Endl;
   Log() << Endl;
   Log() << "Please consult the Users Guide for the format of the formula string" << Endl;
   Log() << "and the allowed parameter ranges:" << Endl;
   if (gConfig().WriteOptionsReference()) {
      Log() << "<a href=\"http://tmva.sourceforge.net/docu/TMVAUsersGuide.pdf\">" 
              << "http://tmva.sourceforge.net/docu/TMVAUsersGuide.pdf</a>" << Endl;
   }
   else Log() << "http://tmva.sourceforge.net/docu/TMVAUsersGuide.pdf" << Endl;
   Log() << Endl;
   Log() << gTools().Color("bold") << "--- Performance optimisation:" << gTools().Color("reset") << Endl;
   Log() << Endl;
   Log() << "The FDA performance depends on the complexity and fidelity of the" << Endl;
   Log() << "user-defined discriminator function. As a general rule, it should" << Endl;
   Log() << "be able to reproduce the discrimination power of any linear" << Endl;
   Log() << "discriminant analysis. To reach into the nonlinear domain, it is" << Endl;
   Log() << "useful to inspect the correlation profiles of the input variables," << Endl;
   Log() << "and add quadratic and higher polynomial terms between variables as" << Endl;
   Log() << "necessary. Comparison with more involved nonlinear classifiers can" << Endl;
   Log() << "be used as a guide." << Endl;
   Log() << Endl;
   Log() << gTools().Color("bold") << "--- Performance tuning via configuration options:" << gTools().Color("reset") << Endl;
   Log() << Endl;
   Log() << "Depending on the function used, the choice of \"FitMethod\" is" << Endl;
   Log() << "crucial for getting valuable solutions with FDA. As a guideline it" << Endl;
   Log() << "is recommended to start with \"FitMethod=MINUIT\". When more complex" << Endl;
   Log() << "functions are used where MINUIT does not converge to reasonable" << Endl;
   Log() << "results, the user should switch to non-gradient FitMethods such" << Endl;
   Log() << "as GeneticAlgorithm (GA) or Monte Carlo (MC). It might prove to be" << Endl;
   Log() << "useful to combine GA (or MC) with MINUIT by setting the option" << Endl;
   Log() << "\"Converger=MINUIT\". GA (MC) will then set the starting parameters" << Endl;
   Log() << "for MINUIT such that the basic quality of GA (MC) of finding global" << Endl;
   Log() << "minima is combined with the efficacy of MINUIT of finding local" << Endl;
   Log() << "minima." << Endl;
}
