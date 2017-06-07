// @(#)root/mathcore:$Id: MinimizerOptions.cxx 31625 2009-12-08 10:02:13Z moneta $ 
// Author: L. Moneta Fri Aug 15 2008

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2008  LCG ROOT Math Team, CERN/PH-SFT                *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

#include "Math/MinimizerOptions.h"

// case of using ROOT plug-in manager
#ifndef MATH_NO_PLUGIN_MANAGER
#include "TEnv.h"
#endif 

namespace ROOT { 
   

namespace Math { 

   static std::string gDefaultMinimizer = ""; // take from /etc/system.rootrc in ROOT Fitter
   static std::string gDefaultMinimAlgo = "Migrad";
   static double gDefaultErrorDef = 1.;
   static double gDefaultTolerance = 1.E-4; 
   static double gDefaultPrecision = -1; // value <= 0 means left to minimizer
   static int  gDefaultMaxCalls = 0; // 0 means leave default values Deaf
   static int  gDefaultMaxIter  = 0; 
   static int  gDefaultStrategy  = 1; 
   static int  gDefaultPrintLevel  = 0; 



void MinimizerOptions::SetDefaultMinimizer(const char * type, const char * algo ) {   
   // set the default minimizer type and algorithm
   if (type) gDefaultMinimizer = std::string(type); 
   if (algo) gDefaultMinimAlgo = std::string(algo);
}
void MinimizerOptions::SetDefaultErrorDef(double up) {
   // set the default error definition 
   gDefaultErrorDef = up; 
}
void MinimizerOptions::SetDefaultTolerance(double tol) {
   // set the defult tolerance
   gDefaultTolerance = tol; 
}
void MinimizerOptions::SetDefaultPrecision(double prec) {
   // set the defult precision
   gDefaultPrecision = prec; 
}
void MinimizerOptions::SetDefaultMaxFunctionCalls(int maxcall) {
   // set the default maximum number of function calls
   gDefaultMaxCalls = maxcall; 
}
void MinimizerOptions::SetDefaultMaxIterations(int maxiter) {
   // set the default maximum number of iterations
   gDefaultMaxIter = maxiter; 
}
void MinimizerOptions::SetDefaultStrategy(int stra) {
   // set the default minimization strategy
   gDefaultStrategy = stra; 
}
void MinimizerOptions::SetDefaultPrintLevel(int level) {
   // set the default printing level 
   gDefaultPrintLevel = level; 
}

const std::string & MinimizerOptions::DefaultMinimizerAlgo() { return gDefaultMinimAlgo; }
double MinimizerOptions::DefaultErrorDef()         { return gDefaultErrorDef; }
double MinimizerOptions::DefaultTolerance()        { return gDefaultTolerance; }
double MinimizerOptions::DefaultPrecision()        { return gDefaultPrecision; }
int    MinimizerOptions::DefaultMaxFunctionCalls() { return gDefaultMaxCalls; }
int    MinimizerOptions::DefaultMaxIterations()    { return gDefaultMaxIter; }
int    MinimizerOptions::DefaultStrategy()         { return gDefaultStrategy; }
int    MinimizerOptions::DefaultPrintLevel()       { return gDefaultPrintLevel; }

const std::string & MinimizerOptions::DefaultMinimizerType() 
{ 
   // return default minimizer
   // if is "" (no default is set) read from etc/system.rootrc

   if (gDefaultMinimizer.size() == 0) { 
#ifndef MATH_NO_PLUGIN_MANAGER
   // use value defined in etc/system.rootrc  (if not found Minuit is used) 
      if (gEnv) 
         gDefaultMinimizer = gEnv->GetValue("Root.Fitter","Minuit");   
#else
      gDefaultMinimizer = "Minuit2";  // in case no PM exists 
#endif
   }

   return gDefaultMinimizer; 
}


MinimizerOptions::MinimizerOptions(): 
   fLevel( gDefaultPrintLevel),
   fMaxCalls( gDefaultMaxCalls ), 
   fMaxIter( gDefaultMaxIter ), 
   fStrategy( gDefaultStrategy ), 
   fErrorDef(  gDefaultErrorDef ), 
   fTolerance( gDefaultTolerance ),
   fPrecision( gDefaultPrecision )
{
   // constructor using  the default options

   fMinimType = MinimizerOptions::DefaultMinimizerType();

   fAlgoType =  gDefaultMinimAlgo;

   // case of Fumili2 and TMinuit
   if (fMinimType == "TMinuit") fMinimType = "Minuit";
   else if (fMinimType == "Fumili2") { 
      fMinimType = "Minuit2";
      fAlgoType = "Fumili";
   }   
   else if (fMinimType == "GSLMultiMin" && fAlgoType == "Migrad") 
      fAlgoType = "BFGS2";
}



} // end namespace Math

} // end namespace ROOT

