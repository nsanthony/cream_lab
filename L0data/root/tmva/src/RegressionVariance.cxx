// @(#)root/tmva $Id: RegressionVariance.cxx 29122 2009-06-22 06:51:30Z brun $   
// Author: Andreas Hoecker, Joerg Stelzer, Helge Voss

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : RegressionVariance                                                    *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description: Calculate the separation critiera useded in regression            *
 *                                                                                *
 *          There are two things: the Separation Index, and the Separation Gain   *
 *          Separation Index:                                                     *
 *          Measure of the "Variance" of a sample.                                *
 *                                                                                *
 *          Separation Gain:                                                      *
 *          the measure of how the quality of separation of the sample increases  *
 *          by splitting the sample e.g. into a "left-node" and a "right-node"    *
 *          (N * Index_parent) - (N_left * Index_left) - (N_right * Index_right)  *
 *          this is then the quality crition which is optimized for when trying   *
 *          to increase the information in the system (making the best selection  *
 *                                                                                *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         * 
 *      U. of Victoria, Canada                                                    * 
 *      Heidelberg U., Germany                                                    * 
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://ttmva.sourceforge.net/LICENSE)                                         *
 **********************************************************************************/
#include <iostream>
#include "TMVA/RegressionVariance.h"

ClassImp(TMVA::RegressionVariance)

//_______________________________________________________________________
Double_t TMVA::RegressionVariance::GetSeparationGain(const Double_t &nLeft, 
                                                     const Double_t& targetLeft , const Double_t& target2Left , 
                                                     const Double_t &nTot, 
                                                     const Double_t& targetTot , const Double_t& target2Tot)
{
   // Separation Gain:                                                     
   // the measure of how the quality of separation of the sample increases 
   // by splitting the sample e.g. into a "left-node" and a "right-node"   
   // (N * Index_parent) - (N_left * Index_left) - (N_right * Index_right) 
   // this is then the quality crition which is optimized for when trying  
   // to increase the information in the system (making the best selection             

   if  ( nTot==nLeft || nLeft==0 ) return 0.;

   Double_t parentIndex = this->GetSeparationIndex(nTot,targetTot,target2Tot);
   Double_t leftIndex   = ( (nTot - nLeft) / nTot * this->GetSeparationIndex(nTot-nLeft,targetTot-targetLeft,target2Tot-target2Left) );
   Double_t rightIndex  =    nLeft / nTot * this->GetSeparationIndex(nLeft,targetLeft,target2Left);
    
   //   return (parentIndex - leftIndex - rightIndex)/(parentIndex);   
   return (parentIndex - leftIndex - rightIndex);   
}

//_______________________________________________________________________
Double_t TMVA::RegressionVariance::GetSeparationIndex(const Double_t& n, 
                                                      const Double_t& target , const Double_t& target2)
{
   // Separation Index:  a simple Variance

   return ( target2 - target*target/n) / n;

}



