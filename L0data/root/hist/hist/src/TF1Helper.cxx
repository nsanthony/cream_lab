// @(#)root/hist:$Id: TF1Helper.cxx 29775 2009-08-13 15:07:28Z moneta $
// Author: Lorenzo Moneta 12/06/07

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2006  LCG ROOT Math Team, CERN/PH-SFT                *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// helper functions and classes used internally by TF1 

#include "TF1Helper.h"
#include "TVirtualFitter.h"
#include "TError.h"
#include <vector>
#include <cmath>
#include <cassert>


namespace ROOT { 



   namespace TF1Helper{ 


      


double IntegralError(TF1 * func, Int_t ndim, const double * a, const double * b, double epsilon) { 

   // calculate the eror on an integral from a to b of a parametetric function f when the parameters 
   // are estimated from a fit and have an error represented by the covariance matrix of the fit. 
   // The latest fit result is used 

   // need to create the gradient functions w.r.t to the parameters 

   // loop on all parameters 
   bool onedim = ndim == 1; 
   int npar = func->GetNpar();
   int nfreepar = func->GetNumberFreeParameters();

   double * epar = func->GetParErrors();

   // get covariance matrix from latest fit 
   TVirtualFitter * fitter = TVirtualFitter::GetFitter();
   if (fitter == 0) { 
      Error("TF1Helper","No existing fitter");
      return 0;
   } 
   double * covMatrix = fitter->GetCovarianceMatrix(); 
   if (covMatrix == 0) { 
      Error("TF1Helper","Fitter has no covariance matrix");
      return 0;
   }
   // check that fitter and function are in sync
   if (fitter->GetNumberFreeParameters() != nfreepar) { 
      Error("TF1Helper","Wrong Fitter for TF1");
      return 0;
   }

   std::vector<double> ig;
   ig.reserve(nfreepar);
   for (int i=0; i < npar; ++i) { 

      // check also the parameter values
      if (fitter->GetParameter(i) != func->GetParameter(i) ) { 
         Error("TF1Helper","Wrong Fitter for TF1");
         return 0;
      }
      
      // check that parameter is not fixed  
      if (! fitter->IsFixed(i) ) {
         // should check the limits 
         double integral  = 0;
         if (epar[i] > 0 ) {          
            TF1 gradFunc("gradFunc",TGradientParFunction(i,func),0,0,0);
            if (onedim) 
               integral = gradFunc.Integral(*a,*b,(double*)0,epsilon);
            else { 
               double relerr;
               integral = gradFunc.IntegralMultiple(ndim,a,b,epsilon,relerr);
            }
         }
         ig.push_back(integral);
      } 
   }
   // number of free parameters and size of corr matrix
   assert( int(ig.size()) == nfreepar); 

   double err2 = 0; 
   for (int i = 0; i < nfreepar; ++i) { 
      double s = 0;
      for (int j =0; j < nfreepar; ++j) 
         s += ig[j] * covMatrix[i*nfreepar + j] ;
      

      err2 += ig[i] * s; 
   }


   return std::sqrt(err2);

}   


} // end namespace TF1Helper


} // end namespace ROOT
