// @(#)root/tmva $Id: DataSetFactory.h 31466 2009-11-30 16:23:35Z stelzer $
// Author: Andreas Hoecker, Peter Speckmayer, Joerg Stelzer, Eckhard von Toerne, Helge Voss

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : DataSetFactory                                                        *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Contains all the data information                                         *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker <Andreas.Hocker@cern.ch> - CERN, Switzerland              *
 *      Joerg Stelzer   <Joerg.Stelzer@cern.ch>  - CERN, Switzerland              *
 *      Peter Speckmayer <Peter.Speckmayer@cern.ch> - CERN, Switzerland           *
 *      Eckhard von Toerne <evt@physik.uni-bonn.de> - U. of Bonn, Germany         *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *                                                                                *
 * Copyright (c) 2006:                                                            *
 *      CERN, Switzerland                                                         *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#define ALTERNATIVE_EVENT_VECTOR_BUILDING 1

#ifndef ROOT_TMVA_DataSetFactory
#define ROOT_TMVA_DataSetFactory

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// DataSetFactory                                                       //
//                                                                      //
// Class that contains all the data information                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <stdlib.h>

#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef ROOT_TTree
#include "TTree.h"
#endif
#ifndef ROOT_TCut
#include "TCut.h"
#endif
#ifndef ROOT_TTreeFormula
#include "TTreeFormula.h"
#endif
#ifndef ROOT_TMatrixDfwd
#include "TMatrixDfwd.h"
#endif
#ifndef ROOT_TPrincipal
#include "TPrincipal.h"
#endif
#ifndef ROOT_TRandom3
#include "TRandom3.h"
#endif

#ifndef ROOT_TMVA_Types
#include "TMVA/Types.h"
#endif
#ifndef ROOT_TMVA_VariableInfo
#include "TMVA/VariableInfo.h"
#endif
#ifndef ROOT_TMVA_Event
#include "TMVA/Event.h"
#endif

namespace TMVA {
   
   class DataSet;
   class DataSetInfo;
   class DataInputHandler;
   class TreeInfo;
   class MsgLogger;

   typedef std::vector< Event* >                              EventVector;
   typedef std::vector< EventVector >                         EventVectorOfClasses;
   typedef std::map<Types::ETreeType,  EventVectorOfClasses > EventVectorOfClassesOfTreeType;
   typedef std::map<Types::ETreeType,  EventVector >          EventVectorOfTreeType;

   typedef std::vector< Double_t >                            ValuePerClass;
   typedef std::map<Types::ETreeType,  ValuePerClass >        ValuePerClassOfTreeType;

   typedef std::vector< Int_t >                               NumberPerClass;
   typedef std::map<Types::ETreeType,  NumberPerClass >       NumberPerClassOfTreeType;

   // =============== maybe move these elswhere (e.g. into the tools )

   // =============== functors =======================


   class RandomGenerator {
   public:
      RandomGenerator( UInt_t seed ){
         fRandom.SetSeed( seed );
      }
      UInt_t operator() ( UInt_t n ) {
         return fRandom.Integer(n);
      }
   private:
      TRandom3 fRandom; // random generator
   };


   // delete-functor (to be used in e.g. for_each algorithm)
   template<class T>
   struct DeleteFunctor_t
   {
      DeleteFunctor_t& operator()(T* p) {
         delete p;
         return *this;
      }
   };

   template<class T>
   DeleteFunctor_t<T> DeleteFunctor()
   {
      return DeleteFunctor_t<T>();
   }


   template< typename T >
   class Increment {
      T value;
   public:
      Increment( T start ) : value( start ){ }
      T operator()() {
         return value++;
      }
   };



   template <typename F>
   class null_t 
   {
   private:
      // returns argF
   public:
      typedef F argument_type;
      F operator()(const F& argF) const 
      {
         return argF;
      }
   };

   template <typename F>
   inline null_t<F> null() {
      return null_t<F>();
   }


   
   template <typename F, typename G, typename H>
   class compose_binary_t : public std::binary_function<typename G::argument_type,
                                                        typename H::argument_type,
                                                        typename F::result_type>
   {
   private:
      const F& f;    // f(g(argG),h(argH))
      const G& g;
      const H& h;
   public:
      compose_binary_t(const F& _f, const G& _g, const H& _h) : f(_f), g(_g), h(_h) 
      {
      }
      
      typename F::result_type operator()(const typename G::argument_type& argG,
                                         const typename H::argument_type& argH) const 
      {
         return f(g(argG),h(argH));
      }
   };

   template <typename F, typename G, typename H>
   inline compose_binary_t<F,G,H> compose_binary(const F& _f, const G& _g, const H& _h) {
      return compose_binary_t<F,G,H>(_f,_g,_h);
   }




   template <typename F, typename G>
   class compose_unary_t : public std::unary_function<typename G::argument_type,
                                                      typename F::result_type>
   {
   private:
      const F& f;    // f(g(argG))
      const G& g;
   public:
      compose_unary_t(const F& _f, const G& _g) : f(_f), g(_g) 
      {
      }

      typename F::result_type operator()(const typename G::argument_type&  argG) const 
      {
         return f(g(argG));
      }
   };

   template <typename F, typename G>
   inline compose_unary_t<F,G> compose_unary(const F& _f, const G& _g) {
      return compose_unary_t<F,G>(_f,_g);
   }

   // =============== functors =======================


   // =========================================================


   class DataSetFactory {

   public:



      // singleton class
      static DataSetFactory& Instance() { if (!fgInstance) fgInstance = new DataSetFactory(); return *fgInstance; } 
      static void destroyInstance() { if (fgInstance) { delete fgInstance; fgInstance=0; } }

      DataSet* CreateDataSet( DataSetInfo &, DataInputHandler& );

   protected:

      ~DataSetFactory();
      
      DataSetFactory();
      static DataSetFactory *fgInstance;

      DataSet*  BuildInitialDataSet( DataSetInfo&, TMVA::DataInputHandler& );
      DataSet*  BuildDynamicDataSet( DataSetInfo& );

#ifndef ALTERNATIVE_EVENT_VECTOR_BUILDING

      void      BuildEventVector   ( DataSetInfo& dsi, 
                                     DataInputHandler& dataInput, 
                                     std::vector< std::vector< Event* > >& tmpEventVector, 
                                     std::vector<Double_t>& sumOfWeights, 
                                     std::vector<Double_t>& nTempEvents, 
                                     std::vector<Double_t>& renormFactor,
                                     std::vector< std::vector< std::pair< Long64_t, Types::ETreeType > > >& userDefinedEventTypes );
      
      DataSet*  MixEvents          ( DataSetInfo& dsi, 
                                     std::vector< std::vector< Event* > >& tmpEventVector, 
                                     std::vector< std::pair< Int_t, Int_t > >& nTrainTestEvents, 
                                     const TString& splitMode, UInt_t splitSeed, 
                                     std::vector<Double_t>& renormFactor,
                                     std::vector< std::vector< std::pair< Long64_t, Types::ETreeType > > >& userDefinedEventTypes );

      void      InitOptions        ( DataSetInfo& dsi, 
                                     std::vector< std::pair< Int_t, Int_t > >& nTrainTestEvents, 
                                     TString& normMode, UInt_t& splitSeed, TString& splitMode );
      

#else
      // ---------- new versions
      void      BuildEventVector    ( DataSetInfo& dsi, 
                                      DataInputHandler& dataInput, 
                                      EventVectorOfClassesOfTreeType& tmpEventVector);
      
      DataSet*  MixEvents           ( DataSetInfo& dsi, 
                                      EventVectorOfClassesOfTreeType& tmpEventVector, 
                                      NumberPerClassOfTreeType& nTrainTestEvents,
                                      const TString& splitMode,
                                      const TString& mixMode, 
                                      const TString& normMode, 
                                      UInt_t splitSeed);

      void      RenormEvents        ( DataSetInfo& dsi, 
                                      EventVectorOfClassesOfTreeType& tmpEventVector,
                                      const TString& normMode );

      void      InitOptions         ( DataSetInfo& dsi, 
                                      NumberPerClassOfTreeType& nTrainTestEvents, 
                                      TString& normMode, UInt_t& splitSeed, TString& splitMode, TString& mixMode );
      

      // ------------------------
#endif

      // auxiliary functions to compute correlations
      TMatrixD* CalcCorrelationMatrix( DataSet*, const UInt_t classNumber );
      TMatrixD* CalcCovarianceMatrix ( DataSet*, const UInt_t classNumber );
      void      CalcMinMax           ( DataSet*, DataSetInfo& dsi );

      // resets branch addresses to current event
      void   ResetBranchAndEventAddresses( TTree* );
      void   ResetCurrentTree() { fCurrentTree = 0; }
      void   ChangeToNewTree( TreeInfo&, const DataSetInfo & );
      Bool_t CheckTTreeFormula( TTreeFormula* ttf, const TString& expression, Bool_t& hasDollar );

      // verbosity
      Bool_t Verbose() { return fVerbose; }

      // data members

      // verbosity
      Bool_t                     fVerbose;           //! Verbosity
      TString                    fVerboseLevel;      //! VerboseLevel

      // the event 
      mutable TTree*             fCurrentTree;       //! the tree, events are currently read from
      mutable UInt_t             fCurrentEvtIdx;     //! the current event (to avoid reading of the same event)

      // the formulas for reading the original tree
      std::vector<TTreeFormula*> fInputFormulas;   //! input variables
      std::vector<TTreeFormula*> fTargetFormulas;  //! targets
      std::vector<TTreeFormula*> fCutFormulas;     //! cuts
      std::vector<TTreeFormula*> fWeightFormula;   //! weights
      std::vector<TTreeFormula*> fSpectatorFormulas; //! spectators

      mutable MsgLogger*         fLogger;          //! message logger
      MsgLogger& Log() const { return *fLogger; }
   };
}

#endif
