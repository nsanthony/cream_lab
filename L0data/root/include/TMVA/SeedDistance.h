// @(#)root/tmva $Id: SeedDistance.h 29122 2009-06-22 06:51:30Z brun $ 
// Author: Peter Speckmayer

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : SeedDistance                                                          *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *       Searches for the nearest seed                                            *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Peter Speckmayer <speckmay@mail.cern.ch>  - CERN, Switzerland             *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         * 
 *      MPI-K Heidelberg, Germany                                                 * 
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_SeedDistance
#define ROOT_TMVA_SeedDistance

#include <vector>

#ifndef ROOT_TMVA_IMetric
#ifndef ROOT_IMetric
#include "IMetric.h"
#endif
#endif


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// SeedDistance                                                          //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


namespace TMVA {

   class IMetric;

   class SeedDistance {

   public:

      SeedDistance( IMetric& metric, std::vector< std::vector<Double_t> >& seeds );
      virtual ~SeedDistance() {}

      std::vector<Double_t>& GetDistances( std::vector<Double_t>& point );

   protected:

      std::vector< std::vector<Double_t> >& fSeeds;
      std::vector<Double_t> fDistances;
      IMetric& fMetric; 

   private:

      ClassDef(SeedDistance,0) // 
   };

} // namespace TMVA

#endif


