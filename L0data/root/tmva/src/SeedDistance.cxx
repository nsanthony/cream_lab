// @(#)root/tmva $Id: SeedDistance.cxx 29122 2009-06-22 06:51:30Z brun $ 
// Author: Andreas Hoecker, Peter Speckmayer

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : SeedDistance                                                         *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Implementation                                                            *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Peter Speckmayer <speckmay@mail.cern.ch> - CERN, Switzerland              *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         * 
 *      MPI-K Heidelberg, Germany                                                 * 
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

//_______________________________________________________________________
//                                                                      
// SeedDistance
//
//_______________________________________________________________________

#include "TMVA/SeedDistance.h"

ClassImp(TMVA::SeedDistance)

//_______________________________________________________________________
TMVA::SeedDistance::SeedDistance( IMetric& metric, std::vector< std::vector<Double_t> >& seeds ) 
   : fSeeds( seeds ),
     fMetric( metric )
{
   // constructor
}            



//_______________________________________________________________________
std::vector<Double_t>& TMVA::SeedDistance::GetDistances( std::vector<Double_t>& point )
{
   // calculates distances of all seeds to a point and stores the result in a distance vector
   fDistances.clear();
   Double_t val = 0.0;
   for( std::vector< std::vector<Double_t> >::iterator itSeed = fSeeds.begin(); itSeed != fSeeds.end(); itSeed++ ){
      val = fMetric.Distance( (*itSeed), point );
      fDistances.push_back( val );
   }
   return fDistances;
}


