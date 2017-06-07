// @(#)root/tmva $Id: DecisionTree.h 29122 2009-06-22 06:51:30Z brun $
// Author: Andreas Hoecker, Joerg Stelzer, Helge Voss, Kai Voss 

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : DecisionTree                                                          *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Implementation of a Decision Tree                                         *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker <Andreas.Hocker@cern.ch> - CERN, Switzerland              *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *      Kai Voss        <Kai.Voss@cern.ch>       - U. of Victoria, Canada         *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         * 
 *      U. of Victoria, Canada                                                    * 
 *      MPI-K Heidelberg, Germany                                                 * 
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://mva.sourceforge.net/license.txt)                                       *
 *                                                                                *
 **********************************************************************************/

#ifndef ROOT_TMVA_DecisionTree
#define ROOT_TMVA_DecisionTree

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// DecisionTree                                                         //
//                                                                      //
// Implementation of a Decision Tree                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TH2
#include "TH2.h"
#endif

#ifndef ROOT_TMVA_Types
#include "TMVA/Types.h"
#endif
#ifndef ROOT_TMVA_DecisionTreeNode
#include "TMVA/DecisionTreeNode.h"
#endif
#ifndef ROOT_TMVA_BinaryTree
#include "TMVA/BinaryTree.h"
#endif
#ifndef ROOT_TMVA_BinarySearchTree
#include "TMVA/BinarySearchTree.h"
#endif
#ifndef ROOT_TMVA_SeparationBase
#include "TMVA/SeparationBase.h"
#endif
#ifndef ROOT_TMVA_RegressionVariance
#include "TMVA/RegressionVariance.h"
#endif

class TRandom3;

namespace TMVA {
  
   class Event;
  
   class DecisionTree : public BinaryTree {

   private:

      static const Int_t fgRandomSeed; // set nonzero for debugging and zero for random seeds
    
   public:

      typedef std::vector<TMVA::Event*> EventList;
    
      // the constructur needed for the "reading" of the decision tree from weight files
      DecisionTree( void );
    
      // the constructur needed for constructing the decision tree via training with events
      DecisionTree( SeparationBase *sepType, Int_t minSize, 
                    Int_t nCuts,
                    Bool_t randomisedTree=kFALSE, Int_t useNvars=0, 
                    UInt_t nNodesMax=999999, UInt_t nMaxDepth=9999999, 
                    Int_t iSeed=fgRandomSeed, Float_t purityLimit=0.5,
                    Int_t treeID = 0);
    
      // copy constructor
      DecisionTree (const DecisionTree &d);
    
      virtual ~DecisionTree( void );

      virtual Node * CreateNode(UInt_t) const { return new DecisionTreeNode(); }
      virtual BinaryTree* CreateTree() const { return new DecisionTree(); }
      virtual const char* ClassName() const { return "DecisionTree"; }
  
      // building of a tree by recursivly splitting the nodes 

      UInt_t BuildTree( const EventList & eventSample, 
                       DecisionTreeNode *node = NULL);
      // determine the way how a node is split (which variable, which cut value)

      Float_t TrainNode( const EventList & eventSample,  DecisionTreeNode *node ) { return TrainNodeFast( eventSample, node ); }
      Float_t TrainNodeFast( const EventList & eventSample,  DecisionTreeNode *node );
      Float_t TrainNodeFull( const EventList & eventSample,  DecisionTreeNode *node );
    
      // fill at tree with a given structure already (just see how many signa/bkgr
      // events end up in each node 

      void FillTree( EventList & eventSample);

      // fill the existing the decision tree structure by filling event
      // in from the top node and see where they happen to end up
      void FillEvent( TMVA::Event & event,  
                      TMVA::DecisionTreeNode *node  );
    
      // returns: 1 = Signal (right),  -1 = Bkg (left)

      Double_t CheckEvent( const TMVA::Event & , Bool_t UseYesNoLeaf = kFALSE ) const;     
      TMVA::DecisionTreeNode* GetEventNode(const TMVA::Event & e) const;

      // return the individual relative variable importance 
      std::vector< Double_t > GetVariableImportance();

      Double_t GetVariableImportance(UInt_t ivar);
    
      // clear the tree nodes (their S/N, Nevents etc), just keep the structure of the tree

      void ClearTree();
    
      // set pruning method
      enum EPruneMethod { kExpectedErrorPruning=0, kCostComplexityPruning, kNoPruning };
      void SetPruneMethod( EPruneMethod m = kCostComplexityPruning ) { fPruneMethod = m; }
    
      // recursive pruning of the tree, validation sample required for automatic pruning
      Double_t PruneTree( EventList* validationSample = NULL );
    
      // manage the pruning strength parameter (iff < 0 -> automate the pruning process)
      void SetPruneStrength( Double_t p ) { fPruneStrength = p; }
      Double_t GetPruneStrength( ) const { return fPruneStrength; }

      // apply pruning validation sample to a decision tree
      void ApplyValidationSample( const EventList* validationSample ) const;
    
      // return the misclassification rate of a pruned tree
      Double_t TestPrunedTreeQuality( const DecisionTreeNode* dt = NULL, Int_t mode=0 ) const;
    
      // pass a single validation event throught a pruned decision tree
      void CheckEventWithPrunedTree( const TMVA::Event& ) const;

      // calculate the normalization factor for a pruning validation sample
      Float_t GetSumWeights( const EventList* validationSample ) const;
    
      void SetNodePurityLimit( Double_t p ) { fNodePurityLimit = p; }
      Float_t GetNodePurityLimit( ) const { return fNodePurityLimit; }

      void DescendTree( DecisionTreeNode *n = NULL );
      void SetParentTreeInNodes( DecisionTreeNode *n = NULL );
    
      DecisionTreeNode* GetLeftDaughter( DecisionTreeNode *n );
    
      DecisionTreeNode* GetRightDaughter( DecisionTreeNode *n );
    
      // retrieve node from the tree. Its position (up to a maximal tree depth of 64)
      // is coded as a sequence of left-right moves starting from the root, coded as
      // 0-1 bit patterns stored in the "long-integer" together with the depth
      DecisionTreeNode* GetNode( ULong_t sequence, UInt_t depth );
    
      UInt_t CleanTree(DecisionTreeNode *node=NULL);
     
      void PruneNode(TMVA::DecisionTreeNode *node);    
    
      // prune a node from the tree without deleting its descendants; allows one to
      // effectively prune a tree many times without making deep copies
      void PruneNodeInPlace( TMVA::DecisionTreeNode* node );
    

      UInt_t CountLeafNodes(TMVA::DecisionTreeNode *n = NULL);

      void  SetTreeID(Int_t treeID){fTreeID = treeID;};
      Int_t GetTreeID(){return fTreeID;};

      Bool_t DoRegression() const { return fAnalysisType == Types::kRegression; }
      void SetAnalysisType (Types::EAnalysisType t) { fAnalysisType = t;}
      Types::EAnalysisType GetAnalysisType ( void ) { return fAnalysisType;}

   private:
      // utility functions
     
      // calculate the Purity out of the number of sig and bkg events collected
      // from individual samples.
    
      // calculates the purity S/(S+B) of a given event sample
      Float_t SamplePurity(EventList eventSample);

      UInt_t    fNvars;          // number of variables used to separate S and B
      Int_t     fNCuts;          // number of grid point in variable cut scans
      SeparationBase *fSepType;  // the separation crition
      RegressionVariance *fRegType;  // the separation crition used in Regression
    
      Float_t   fMinSize;        // min number of events in node
      Float_t   fMinSepGain;     // min number of separation gain to perform node splitting
    
      Bool_t    fUseSearchTree;  // cut scan done with binary trees or simple event loop.
      Double_t  fPruneStrength;  // a parameter to set the "amount" of pruning..needs to be adjusted 
    
      EPruneMethod fPruneMethod; // method used for prunig 
    
      Float_t   fNodePurityLimit;// purity limit to decide whether a node is signal
    
      Bool_t    fRandomisedTree; // choose at each node splitting a random set of variables 
      Int_t     fUseNvars;       // the number of variables used in randomised trees;
    
      TRandom3  *fMyTrandom;     // random number generator for randomised trees
    
      std::vector< Double_t > fVariableImportance; // the relative importance of the different variables 

      UInt_t     fNNodesMax;     // max # of nodes
      UInt_t     fMaxDepth;      // max depth

      static const Int_t  fgDebugLevel = 0;     // debug level determining some printout/control plots etc.
      Int_t     fTreeID;        // just an ID number given to the tree.. makes debugging easier as tree knows who he is.

      Types::EAnalysisType  fAnalysisType;   // kClassification(=0=false) or kRegression(=1=true)

      ClassDef(DecisionTree,0)               // implementation of a Decision Tree
   };
  
} // namespace TMVA

#endif 
