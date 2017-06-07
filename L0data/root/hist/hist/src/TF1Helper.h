// @(#)root/hist:$Id: TF1Helper.h 29775 2009-08-13 15:07:28Z moneta $
// Author: Lorenzo Moneta 12/06/07

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2007  LCG ROOT Math Team, CERN/PH-SFT                *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// helper functions used internally by TF1

#ifndef ROOT_TF1Helper
#define ROOT_TF1Helper 

#include "TF1.h"

namespace ROOT { 

   namespace TF1Helper { 

      double IntegralError(TF1 * func, int dim, const double * a, const double * b, double eps); 

  /**
     function class representing the derivative with respect a parameter of a given TF1  
  */
      class TGradientParFunction { 

      public: 

         TGradientParFunction(int ipar, TF1 * f)  : 
            fPar(ipar),
            fFunc(f)
         {}
         
         double operator() (double * x, double *) const 
         { 
            // evaluate gradient vector of functions at point x
            return fFunc->GradientPar(fPar,x);  
         }
         
      private: 
         
         unsigned int fPar; 
         mutable TF1 * fFunc;
      }; 


   } // end namespace TF1Helper

} // end namespace TF1

#endif
