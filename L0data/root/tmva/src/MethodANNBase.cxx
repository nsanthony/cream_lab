// @(#)root/tmva $Id: MethodANNBase.cxx 31458 2009-11-30 13:58:20Z stelzer $
// Author: Andreas Hoecker, Matt Jachowski

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : MethodANNBase                                                         *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Artificial neural network base class for the discrimination of signal     *
 *      from background.                                                          *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker  <Andreas.Hocker@cern.ch>   - CERN, Switzerland           *
 *      Matt Jachowski   <jachowski@stanford.edu>   - Stanford University, USA    *
 *      Joerg Stelzer    <stelzer@cern.ch>          - DESY, Germany               *
 *      Peter Speckmayer <peter.speckmayer@cern.ch> - CERN, Switzerland           *
 *                                                                                *
 * Small changes (regression):                                                    *
 *      Krzysztof Danielowski <danielow@cern.ch>  - IFJ PAN & AGH, Poland         *
 *      Kamil Kraszewski      <kalq@cern.ch>      - IFJ PAN & UJ , Poland         *
 *      Maciej Kruk           <mkruk@cern.ch>     - IFJ PAN & AGH, Poland         *
 *                                                                                *
 *                                                                                *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

//_______________________________________________________________________
//                                                                      
// Base class for all TMVA methods using artificial neural networks      
//                                                                      
//_______________________________________________________________________

#include <vector>
#include <cstdlib>

#include "TString.h"
#include "TTree.h"
#include "TDirectory.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH2F.h"
#include "TH1.h"
#include "TXMLEngine.h"

#include "TMVA/MethodBase.h"
#include "TMVA/MethodANNBase.h"
#include "TMVA/TNeuron.h"
#include "TMVA/TSynapse.h"
#include "TMVA/TActivationChooser.h"
#include "TMVA/Types.h"
#include "TMVA/Tools.h"
#include "TMVA/TNeuronInputChooser.h"
#include "TMVA/Ranking.h"

using std::vector;

ClassImp(TMVA::MethodANNBase)

//______________________________________________________________________________
TMVA::MethodANNBase::MethodANNBase( const TString& jobName,
                                    Types::EMVA methodType,
                                    const TString& methodTitle,
                                    DataSetInfo& theData, 
                                    const TString& theOption,
                                    TDirectory* theTargetDir )
   : TMVA::MethodBase( jobName, methodType, methodTitle, theData, theOption, theTargetDir )
{
   // standard constructor
   // Note: Right now it is an option to choose the neuron input function,
   // but only the input function "sum" leads to weight convergence --
   // otherwise the weights go to nan and lead to an ABORT.   
   InitANNBase();

   DeclareOptions();
}

//______________________________________________________________________________
TMVA::MethodANNBase::MethodANNBase( Types::EMVA methodType,
                                    DataSetInfo& theData, 
                                    const TString& theWeightFile,
                                    TDirectory* theTargetDir )
   : TMVA::MethodBase( methodType, theData, theWeightFile, theTargetDir ) 
{
   // construct the Method from the weight file 
   InitANNBase();

   DeclareOptions();
}

//______________________________________________________________________________
void TMVA::MethodANNBase::DeclareOptions()
{
   // define the options (their key words) that can be set in the option string 
   // here the options valid for ALL MVA methods are declared.
   // know options: NCycles=xx              :the number of training cycles
   //               Normalize=kTRUE,kFALSe  :if normalised in put variables should be used
   //               HiddenLayser="N-1,N-2"  :the specification of the hidden layers
   //               NeuronType=sigmoid,tanh,radial,linar  : the type of activation function
   //                                                       used at the neuronn
   //                

   DeclareOptionRef( fNcycles    = 500,       "NCycles",         "Number of training cycles" );
   DeclareOptionRef( fLayerSpec  = "N,N-1",   "HiddenLayers",    "Specification of hidden layer architecture" );
   DeclareOptionRef( fNeuronType = "sigmoid", "NeuronType",      "Neuron activation function type" );

   TActivationChooser aChooser;
   vector<TString>* names = aChooser.GetAllActivationNames();
   Int_t nTypes = names->size();
   for (Int_t i = 0; i < nTypes; i++)
      AddPreDefVal(names->at(i));
   delete names;

   DeclareOptionRef(fNeuronInputType="sum", "NeuronInputType","Neuron input function type");
   TNeuronInputChooser iChooser;
   names = iChooser.GetAllNeuronInputNames();
   nTypes = names->size();
   for (Int_t i = 0; i < nTypes; i++) AddPreDefVal(names->at(i));
   delete names;
}


//______________________________________________________________________________
void TMVA::MethodANNBase::ProcessOptions()
{
   // do nothing specific at this moment
   vector<Int_t>* layout = ParseLayoutString(fLayerSpec);
   BuildNetwork(layout);
}

//______________________________________________________________________________
vector<Int_t>* TMVA::MethodANNBase::ParseLayoutString(TString layerSpec)
{
   // parse layout specification string and return a vector, each entry
   // containing the number of neurons to go in each successive layer
   vector<Int_t>* layout = new vector<Int_t>();
   layout->push_back((Int_t)GetNvar());
   while(layerSpec.Length()>0) {
      TString sToAdd="";
      if (layerSpec.First(',')<0) {
         sToAdd = layerSpec;
         layerSpec = "";
      } 
      else {
         sToAdd = layerSpec(0,layerSpec.First(','));
         layerSpec = layerSpec(layerSpec.First(',')+1,layerSpec.Length());
      }
      int nNodes = 0;
      if (sToAdd.BeginsWith("n") || sToAdd.BeginsWith("N")) { sToAdd.Remove(0,1); nNodes = GetNvar(); }
      nNodes += atoi(sToAdd);
      layout->push_back(nNodes);
   }
   if( DoRegression() )
      layout->push_back( DataInfo().GetNTargets() );  // one output node
   else
      layout->push_back(1);  // one output node (for signal/background classification)

   int n = 0;
   for( std::vector<Int_t>::iterator it = layout->begin(); it != layout->end(); it++ ){
      n++;
   }
  
   return layout;
}

//______________________________________________________________________________
void TMVA::MethodANNBase::InitANNBase()
{
   // initialize ANNBase object
   fNetwork         = NULL;
   frgen            = NULL;
   fActivation      = NULL;
   fIdentity        = NULL;
   fInputCalculator = NULL;
   fSynapses        = NULL;
   fEstimatorHistTrain = NULL;  
   fEstimatorHistTest  = NULL;  

   // reset monitorign histogram vectors
   fEpochMonHistS.clear();
   fEpochMonHistB.clear();
   fEpochMonHistW.clear();

   // these will be set in BuildNetwork()
   fInputLayer = NULL;
   fOutputNeurons.clear();

   if (fgFIXED_SEED) frgen = new TRandom3(1);   // fix output for debugging
   else              frgen = new TRandom3(0);   // seed = 0 means random seed

   fSynapses = new TObjArray();
}

//______________________________________________________________________________
TMVA::MethodANNBase::~MethodANNBase()
{
   // destructor
   DeleteNetwork();
}

//______________________________________________________________________________
void TMVA::MethodANNBase::DeleteNetwork()
{
   // delete/clear network
   if (fNetwork != NULL) {
      TObjArray *layer;
      Int_t numLayers = fNetwork->GetEntriesFast();
      for (Int_t i = 0; i < numLayers; i++) {
         layer = (TObjArray*)fNetwork->At(i);
         DeleteNetworkLayer(layer);
      }
    
      delete fNetwork;
   }

   if (frgen != NULL)            delete frgen;
   if (fActivation != NULL)      delete fActivation;
   if (fIdentity != NULL)        delete fIdentity;
   if (fInputCalculator != NULL) delete fInputCalculator;
   if (fSynapses != NULL)        delete fSynapses;

   fNetwork         = NULL;
   frgen            = NULL;
   fActivation      = NULL;
   fIdentity        = NULL;
   fInputCalculator = NULL;
   fSynapses        = NULL;
}

//______________________________________________________________________________
void TMVA::MethodANNBase::DeleteNetworkLayer( TObjArray*& layer )
{
   // delete a network layer
   TNeuron* neuron;
   Int_t numNeurons = layer->GetEntriesFast();
   for (Int_t i = 0; i < numNeurons; i++) {
      neuron = (TNeuron*)layer->At(i);
      neuron->DeletePreLinks();
      delete neuron;
   }
   delete layer;
}

//______________________________________________________________________________
void TMVA::MethodANNBase::BuildNetwork( vector<Int_t>* layout, vector<Double_t>* weights, Bool_t fromFile )
{
   // build network given a layout (number of neurons in each layer)
   // and optional weights array

   Log() << kINFO << "Building Network" << Endl;

   DeleteNetwork();
   InitANNBase();

   // set activation and input functions
   TActivationChooser aChooser;
   fActivation = aChooser.CreateActivation(fNeuronType);
   fIdentity   = aChooser.CreateActivation("linear");
   TNeuronInputChooser iChooser;
   fInputCalculator = iChooser.CreateNeuronInput(fNeuronInputType);

   fNetwork = new TObjArray();
   BuildLayers( layout, fromFile );

   // cache input layer and output neuron for fast access
   fInputLayer   = (TObjArray*)fNetwork->At(0);
   TObjArray* outputLayer = (TObjArray*)fNetwork->At(fNetwork->GetEntriesFast()-1);
   fOutputNeurons.clear();
   for (Int_t i = 0; i < outputLayer->GetEntries(); i++) {
      fOutputNeurons.push_back( (TNeuron*)outputLayer->At(i) );
   }

   if (weights == NULL) InitWeights();
   else                 ForceWeights(weights);
}

//______________________________________________________________________________
void TMVA::MethodANNBase::BuildLayers( vector<Int_t>* layout, Bool_t fromFile )
{
   // build the network layers

   TObjArray* curLayer;
   TObjArray* prevLayer = NULL;

   Int_t numLayers = layout->size();

   for (Int_t i = 0; i < numLayers; i++) {
      curLayer = new TObjArray();
      BuildLayer(layout->at(i), curLayer, prevLayer, i, numLayers, fromFile);
      prevLayer = curLayer;
      fNetwork->Add(curLayer);
   }

   // cache pointers to synapses for fast access, the order matters
   for (Int_t i = 0; i < numLayers; i++) {                                       
      TObjArray* layer = (TObjArray*)fNetwork->At(i);                             
      Int_t numNeurons = layer->GetEntriesFast();                                 
      for (Int_t j = 0; j < numNeurons; j++) {                                    
         TNeuron* neuron = (TNeuron*)layer->At(j);                                 
         Int_t numSynapses = neuron->NumPostLinks();                               
         for (Int_t k = 0; k < numSynapses; k++) {                                 
            TSynapse* synapse = neuron->PostLinkAt(k);                              
            fSynapses->Add(synapse);                                   
         }                                                                         
      }                                                                           
   }  
}

//______________________________________________________________________________
void TMVA::MethodANNBase::BuildLayer( Int_t numNeurons, TObjArray* curLayer, 
                                      TObjArray* prevLayer, Int_t layerIndex, 
                                      Int_t numLayers, Bool_t fromFile )
{
   // build a single layer with neurons and synapses connecting this
   // layer to the previous layer

   TNeuron* neuron;
   for (Int_t j = 0; j < numNeurons; j++) {
      if (fromFile && (layerIndex != numLayers-1) && (j==numNeurons-1)){
         neuron = new TNeuron();
         neuron->SetActivationEqn(fIdentity);
         neuron->SetBiasNeuron();
         neuron->ForceValue(1.0);
         curLayer->Add(neuron);  
      }
      else {
         neuron = new TNeuron();
         neuron->SetInputCalculator(fInputCalculator);
         
         // input layer
         if (layerIndex == 0) {
            neuron->SetActivationEqn(fIdentity);
            neuron->SetInputNeuron();
         }
         else {
            // output layer
            if (layerIndex == numLayers-1) {
               neuron->SetOutputNeuron();
               neuron->SetActivationEqn(fIdentity);
         }
            // hidden layers
            else neuron->SetActivationEqn(fActivation);
            AddPreLinks(neuron, prevLayer);
         }
         
         curLayer->Add(neuron);
      }
   }

   // add bias neutron (except to output layer)
   if(!fromFile){
      if (layerIndex != numLayers-1) {
         neuron = new TNeuron();
         neuron->SetActivationEqn(fIdentity);
         neuron->SetBiasNeuron();
         neuron->ForceValue(1.0);
         curLayer->Add(neuron);
      }
   }
}

//______________________________________________________________________________
void TMVA::MethodANNBase::AddPreLinks(TNeuron* neuron, TObjArray* prevLayer)
{
   // add synapses connecting a neuron to its preceding layer

   TSynapse* synapse;
   int numNeurons = prevLayer->GetEntriesFast();
   TNeuron* preNeuron;

   for (Int_t i = 0; i < numNeurons; i++) {
      preNeuron = (TNeuron*)prevLayer->At(i);
      synapse = new TSynapse();
      synapse->SetPreNeuron(preNeuron);
      synapse->SetPostNeuron(neuron);
      preNeuron->AddPostLink(synapse);
      neuron->AddPreLink(synapse);
   }
}

//______________________________________________________________________________
void TMVA::MethodANNBase::InitWeights()
{
   // initialize the synapse weights randomly
   PrintMessage("initializing weights");
   
   // init synapse weights
   Int_t numSynapses = fSynapses->GetEntriesFast();
   TSynapse* synapse;
   for (Int_t i = 0; i < numSynapses; i++) {
      synapse = (TSynapse*)fSynapses->At(i);
      synapse->SetWeight(4.0*frgen->Rndm() - 2.0);
   }
}

//_______________________________________________________________________
void TMVA::MethodANNBase::ForceWeights(vector<Double_t>* weights)
{
   // force the synapse weights
   PrintMessage("forcing weights");

   Int_t numSynapses = fSynapses->GetEntriesFast();
   TSynapse* synapse;
   for (Int_t i = 0; i < numSynapses; i++) {
      synapse = (TSynapse*)fSynapses->At(i);
      synapse->SetWeight(weights->at(i));
   }
}

//______________________________________________________________________________
void TMVA::MethodANNBase::ForceNetworkInputs( const Event* ev, Int_t ignoreIndex)
{
   // force the input values of the input neurons
   // force the value for each input neuron

   Double_t x;
   TNeuron* neuron;

   //   const Event* ev = GetEvent();
   for (UInt_t j = 0; j < GetNvar(); j++) {

      x = (j != (UInt_t)ignoreIndex)?ev->GetValue(j):0;

      neuron = GetInputNeuron(j);
      neuron->ForceValue(x);
   }
}

//______________________________________________________________________________
void TMVA::MethodANNBase::ForceNetworkCalculations()
{
   // calculate input values to each neuron

   TObjArray* curLayer;
   TNeuron* neuron;
   Int_t numLayers = fNetwork->GetEntriesFast();
   Int_t numNeurons;

   for (Int_t i = 0; i < numLayers; i++) {
      curLayer = (TObjArray*)fNetwork->At(i);
      numNeurons = curLayer->GetEntriesFast();

      for (Int_t j = 0; j < numNeurons; j++) {
         neuron = (TNeuron*) curLayer->At(j);
         neuron->CalculateValue();
         neuron->CalculateActivationValue();

      }
   }
}

//______________________________________________________________________________
void TMVA::MethodANNBase::PrintMessage(TString message, Bool_t force) const
{
   // print messages, turn off printing by setting verbose and debug flag appropriately
   if (Verbose() || Debug() || force) Log() << kINFO << message << Endl;
}

//______________________________________________________________________________
void TMVA::MethodANNBase::WaitForKeyboard()
{
   // wait for keyboard input, for debugging
   string dummy;
   Log() << kINFO << "***Type anything to continue (q to quit): ";
   getline(cin, dummy);
   if (dummy == "q" || dummy == "Q") {
      PrintMessage( "quit" );
      delete this;
      exit(0);
   }
}

//______________________________________________________________________________
void TMVA::MethodANNBase::PrintNetwork() const
{
   // print network representation, for debugging
   if (!Debug()) return;

   Log() << Endl;
   PrintMessage( "printing network " );
   Log() << kINFO << "-------------------------------------------------------------------" << Endl;

   TObjArray* curLayer;
   Int_t numLayers = fNetwork->GetEntriesFast();

   for (Int_t i = 0; i < numLayers; i++) {

      curLayer = (TObjArray*)fNetwork->At(i);
      Int_t numNeurons = curLayer->GetEntriesFast();

      Log() << kINFO << "Layer #" << i << " (" << numNeurons << " neurons):" << Endl;
      PrintLayer( curLayer );
   }
}

//______________________________________________________________________________
void TMVA::MethodANNBase::PrintLayer(TObjArray* layer) const
{
   // print a single layer, for debugging

   Int_t numNeurons = layer->GetEntriesFast();
   TNeuron* neuron;
  
   for (Int_t j = 0; j < numNeurons; j++) {
      neuron = (TNeuron*) layer->At(j);
      Log() << kINFO << "\tNeuron #" << j << " (LinksIn: " << neuron->NumPreLinks() 
              << " , LinksOut: " << neuron->NumPostLinks() << ")" << Endl;
      PrintNeuron( neuron );
   }
}

//______________________________________________________________________________
void TMVA::MethodANNBase::PrintNeuron(TNeuron* neuron) const
{
   // print a neuron, for debugging
   Log() << kINFO 
           << "\t\tValue:\t"     << neuron->GetValue()
           << "\t\tActivation: " << neuron->GetActivationValue()
           << "\t\tDelta: "      << neuron->GetDelta() << Endl;
   Log() << kINFO << "\t\tActivationEquation:\t";
   neuron->PrintActivationEqn();
   Log() << kINFO << "\t\tLinksIn:" << Endl;
   neuron->PrintPreLinks();
   Log() << kINFO << "\t\tLinksOut:" << Endl;
   neuron->PrintPostLinks();
}

//_______________________________________________________________________
Double_t TMVA::MethodANNBase::GetMvaValue( Double_t* err )
{
   // get the mva value generated by the NN
   TNeuron* neuron;

   TObjArray* inputLayer = (TObjArray*)fNetwork->At(0);

   const Event * ev = GetEvent();

   for (UInt_t i = 0; i < GetNvar(); i++) {
      neuron = (TNeuron*)inputLayer->At(i);
      neuron->ForceValue( ev->GetValue(i) );
   }
   ForceNetworkCalculations();

   // check the output of the network
   TObjArray* outputLayer = (TObjArray*)fNetwork->At( fNetwork->GetEntriesFast()-1 );
   neuron = (TNeuron*)outputLayer->At(0);

   // cannot determine error
   if (err != 0) *err = -1;

   return neuron->GetActivationValue();
}

//_______________________________________________________________________
const std::vector<Float_t> &TMVA::MethodANNBase::GetRegressionValues() 
{
   // get the regression value generated by the NN
   TNeuron* neuron;

   TObjArray* inputLayer = (TObjArray*)fNetwork->At(0);

   const Event * ev = GetEvent();

   for (UInt_t i = 0; i < GetNvar(); i++) {
      neuron = (TNeuron*)inputLayer->At(i);
      neuron->ForceValue( ev->GetValue(i) );
   }
   ForceNetworkCalculations();

   // check the output of the network
   TObjArray* outputLayer = (TObjArray*)fNetwork->At( fNetwork->GetEntriesFast()-1 );

   if (fRegressionReturnVal == NULL) fRegressionReturnVal = new std::vector<Float_t>();
   fRegressionReturnVal->clear();

   Event * evT = new Event(*ev);
   UInt_t ntgts = outputLayer->GetEntriesFast();
   for (UInt_t itgt = 0; itgt < ntgts; itgt++) {
      evT->SetTarget(itgt,((TNeuron*)outputLayer->At(itgt))->GetActivationValue());
   }

   const Event* evT2 = GetTransformationHandler().InverseTransform( evT );
   for (UInt_t itgt = 0; itgt < ntgts; itgt++) {
      fRegressionReturnVal->push_back( evT2->GetTarget(itgt) );
   }

   delete evT;

   return *fRegressionReturnVal;
}

//_______________________________________________________________________
void TMVA::MethodANNBase::AddWeightsXMLTo( void* parent ) const 
{
   // create XML description of ANN classifier
   Int_t numLayers = fNetwork->GetEntriesFast();
   void* wght = gTools().xmlengine().NewChild(parent, 0, "Weights");
   gTools().xmlengine().NewAttr(wght, 0, "NLayers", gTools().StringFromInt(fNetwork->GetEntriesFast()) );
   TString weights = "";
   for (Int_t i = 0; i < numLayers; i++) {
      TObjArray* layer = (TObjArray*)fNetwork->At(i);
      Int_t numNeurons = layer->GetEntriesFast();
      void* layerxml = gTools().xmlengine().NewChild(wght, 0, "Layer");
      gTools().xmlengine().NewAttr(layerxml, 0, "Index",    gTools().StringFromInt(i) );
      gTools().xmlengine().NewAttr(layerxml, 0, "NNeurons", gTools().StringFromInt(numNeurons) );
      for (Int_t j = 0; j < numNeurons; j++) {
         TNeuron* neuron = (TNeuron*)layer->At(j);
         Int_t numSynapses = neuron->NumPostLinks();
         void* neuronxml = gTools().xmlengine().NewChild(layerxml, 0, "Neuron");
         gTools().xmlengine().NewAttr(neuronxml, 0, "NSynapses", gTools().StringFromInt(numSynapses) );
         if(numSynapses==0) continue;
         stringstream s("");
         s.precision( 16 );
         for (Int_t k = 0; k < numSynapses; k++) {
            TSynapse* synapse = neuron->PostLinkAt(k);
            s << std::scientific << synapse->GetWeight() << " ";
         }
         gTools().xmlengine().AddRawLine( neuronxml, s.str().c_str() );
      }
   }
}


//_______________________________________________________________________
void TMVA::MethodANNBase::ReadWeightsFromXML( void* wghtnode ) 
{
   // read MLP from xml weight file

   // build the layout first
   Bool_t fromFile = kTRUE;
   vector<Int_t>* layout = new vector<Int_t>();

   UInt_t nLayers;
   gTools().ReadAttr( wghtnode, "NLayers", nLayers );
   layout->resize( nLayers );

   void* ch = gTools().xmlengine().GetChild(wghtnode);
   UInt_t index;
   UInt_t nNeurons;
   while (ch) {
      gTools().ReadAttr( ch, "Index",   index   );
      gTools().ReadAttr( ch, "NNeurons", nNeurons );
      layout->at(index) = nNeurons;
      ch = gTools().xmlengine().GetNext(ch);
   }

   BuildNetwork( layout, NULL, fromFile );
   // fill the weights of the synapses
   UInt_t nSyn;
   Float_t weight;
   ch = gTools().xmlengine().GetChild(wghtnode);
   UInt_t iLayer = 0;
   while (ch) {  // layers
      TObjArray* layer = (TObjArray*)fNetwork->At(iLayer);
      gTools().ReadAttr( ch, "Index",   index   );
      gTools().ReadAttr( ch, "NNeurons", nNeurons );

      void* nodeN = gTools().xmlengine().GetChild(ch);
      UInt_t iNeuron = 0;
      while( nodeN ){ // neurons
         TNeuron *neuron = (TNeuron*)layer->At(iNeuron);
         gTools().ReadAttr( nodeN, "NSynapses", nSyn );
         if( nSyn > 0 ){
            const char* content = gTools().xmlengine().GetNodeContent(nodeN);
            std::stringstream s(content);
            for (UInt_t iSyn = 0; iSyn<nSyn; iSyn++) { // synapses
               
               TSynapse* synapse = neuron->PostLinkAt(iSyn);
               s >> weight;
               //Log() << kWARNING << neuron << " " << weight <<  Endl;
               synapse->SetWeight(weight);
            }
         }
         nodeN = gTools().xmlengine().GetNext(nodeN);
         iNeuron++;
      }
      ch = gTools().xmlengine().GetNext(ch);
      iLayer++;
   }
}


//_______________________________________________________________________
void TMVA::MethodANNBase::ReadWeightsFromStream( istream & istr)
{
   // destroy/clear the network then read it back in from the weights file

   // delete network so we can reconstruct network from scratch

   TString dummy;

   // synapse weights
   Double_t weight;
   vector<Double_t>* weights = new vector<Double_t>();
   istr>> dummy;
   while (istr>> dummy >> weight) weights->push_back(weight); // use w/ slower write-out

   ForceWeights(weights);
   

   delete weights;
}

//_______________________________________________________________________
const TMVA::Ranking* TMVA::MethodANNBase::CreateRanking()
{
   // compute ranking of input variables by summing function of weights

   // create the ranking object
   fRanking = new Ranking( GetName(), "Importance" );

   TNeuron*  neuron;
   TSynapse* synapse;
   Double_t  importance, avgVal;
   TString varName;

   for (UInt_t ivar = 0; ivar < GetNvar(); ivar++) {

      neuron = GetInputNeuron(ivar);
      Int_t numSynapses = neuron->NumPostLinks();
      importance = 0;
      varName = GetInputVar(ivar); // fix this line

      // figure out average value of variable i
      Double_t meanS, meanB, rmsS, rmsB, xmin, xmax;
      Statistics( TMVA::Types::kTraining, varName, 
                  meanS, meanB, rmsS, rmsB, xmin, xmax );

      avgVal = (meanS + meanB) / 2.0; // change this into a real weighted average
      if (IsNormalised()) avgVal = 0.5*(1 + gTools().NormVariable( avgVal, GetXmin( ivar ), GetXmax( ivar )));

      for (Int_t j = 0; j < numSynapses; j++) {
         synapse = neuron->PostLinkAt(j);
         importance += synapse->GetWeight() * synapse->GetWeight();
      }
      
      importance *= avgVal * avgVal;

      fRanking->AddRank( Rank( varName, importance ) );
   }

   return fRanking;
}

//_______________________________________________________________________
void TMVA::MethodANNBase::CreateWeightMonitoringHists( const TString& bulkname, 
                                                       std::vector<TH1*>* hv ) const
{
   TH2F* hist;
   Int_t numLayers = fNetwork->GetEntriesFast();

   for (Int_t i = 0; i < numLayers-1; i++) {
      
      TObjArray* layer1 = (TObjArray*)fNetwork->At(i);
      TObjArray* layer2 = (TObjArray*)fNetwork->At(i+1);
      Int_t numNeurons1 = layer1->GetEntriesFast();
      Int_t numNeurons2 = layer2->GetEntriesFast();
      
      TString name = Form("%s%i%i", bulkname.Data(), i, i+1);
      hist = new TH2F(name + "", name + "", 
                      numNeurons1, 0, numNeurons1, numNeurons2, 0, numNeurons2);
      
      for (Int_t j = 0; j < numNeurons1; j++) {
         
         TNeuron* neuron = (TNeuron*)layer1->At(j);
         Int_t numSynapses = neuron->NumPostLinks();

         for (Int_t k = 0; k < numSynapses; k++) {
            
            TSynapse* synapse = neuron->PostLinkAt(k);
            hist->SetBinContent(j+1, k+1, synapse->GetWeight());
            
         }
      }
      
      if (hv) hv->push_back( hist );
      else {
         hist->Write();
         delete hist;
      }
   }
}

//_______________________________________________________________________
void TMVA::MethodANNBase::WriteMonitoringHistosToFile() const
{
   // write histograms to file
   PrintMessage(Form("write special histos to file: %s", BaseDir()->GetPath()), kTRUE);

   if (fEstimatorHistTrain) fEstimatorHistTrain->Write();
   if (fEstimatorHistTest ) fEstimatorHistTest ->Write();

   // histograms containing weights for architecture plotting (used in macro "network.C")
   CreateWeightMonitoringHists( "weights_hist" );

   // now save all the epoch-wise monitoring information
   TDirectory* epochdir = BaseDir()->mkdir( "EpochMonitoring" );
   epochdir->cd();
   for (std::vector<TH1*>::const_iterator it = fEpochMonHistS.begin(); it != fEpochMonHistS.end(); it++) {
      (*it)->Write();
      delete (*it);
   }
   for (std::vector<TH1*>::const_iterator it = fEpochMonHistB.begin(); it != fEpochMonHistB.end(); it++) {
      (*it)->Write();
      delete (*it);
   }
   for (std::vector<TH1*>::const_iterator it = fEpochMonHistW.begin(); it != fEpochMonHistW.end(); it++) {
      (*it)->Write();
      delete (*it);
   }
   BaseDir()->cd();
}

//_______________________________________________________________________
void TMVA::MethodANNBase::MakeClassSpecific( std::ostream& fout, const TString& className ) const
{
   // write specific classifier response
   Int_t numLayers = fNetwork->GetEntries();

   fout << endl;
   fout << "   double ActivationFnc(double x) const;" << endl;
   fout << endl;
   fout << "   int fLayers;" << endl;
   fout << "   int fLayerSize["<<numLayers<<"];" << endl;
   int numNodesFrom = -1;
   for (Int_t lIdx = 0; lIdx < numLayers; lIdx++) {
      int numNodesTo = ((TObjArray*)fNetwork->At(lIdx))->GetEntries();
      if (numNodesFrom<0) { numNodesFrom=numNodesTo; continue; }
      fout << "   double fWeightMatrix" << lIdx-1  << "to" << lIdx << "[" << numNodesTo << "][" << numNodesFrom << "];";
      fout << "   // weight matrix from layer " << lIdx-1  << " to " << lIdx << endl;
      numNodesFrom = numNodesTo;
   }
   fout << endl;
   fout << "   double * fWeights["<<numLayers<<"];" << endl;
   fout << "};" << endl;

   fout << endl;

   fout << "inline void " << className << "::Initialize()" << endl;
   fout << "{" << endl;
   fout << "   // build network structure" << endl;
   fout << "   fLayers = " << numLayers << ";" << endl;
   for (Int_t lIdx = 0; lIdx < numLayers; lIdx++) {
      TObjArray* layer = (TObjArray*)fNetwork->At(lIdx);
      int numNodes = layer->GetEntries();
      fout << "   fLayerSize[" << lIdx << "] = " << numNodes << "; fWeights["<<lIdx<<"] = new double["<<numNodes<<"]; " << endl;
   }

   for (Int_t i = 0; i < numLayers-1; i++) {
      fout << "   // weight matrix from layer " << i  << " to " << i+1 << endl;
      TObjArray* layer = (TObjArray*)fNetwork->At(i);
      Int_t numNeurons = layer->GetEntriesFast();
      for (Int_t j = 0; j < numNeurons; j++) {
         TNeuron* neuron = (TNeuron*)layer->At(j);
         Int_t numSynapses = neuron->NumPostLinks();
         for (Int_t k = 0; k < numSynapses; k++) {
            TSynapse* synapse = neuron->PostLinkAt(k);
            fout << "   fWeightMatrix" << i  << "to" << i+1 << "[" << k << "][" << j << "] = " << synapse->GetWeight() << ";" << endl;
         }
      }
   }

   fout << "}" << endl;
   fout << endl;

   // writing of the GetMvaValue__ method
   fout << "inline double " << className << "::GetMvaValue__( const std::vector<double>& inputValues ) const" << endl;
   fout << "{" << endl;
   fout << "   if (inputValues.size() != (unsigned int)fLayerSize[0]-1) {" << endl;
   fout << "      std::cout << \"Input vector needs to be of size \" << fLayerSize[0]-1 << std::endl;" << endl;
   fout << "      return 0;" << endl;
   fout << "   }" << endl;
   fout << endl;
   fout << "   for (int l=0; l<fLayers; l++)" << endl;
   fout << "      for (int i=0; i<fLayerSize[l]; i++) fWeights[l][i]=0;" << endl;
   fout << endl;
   fout << "   for (int l=0; l<fLayers-1; l++)" << endl;
   fout << "      fWeights[l][fLayerSize[l]-1]=1;" << endl;
   fout << endl;
   fout << "   for (int i=0; i<fLayerSize[0]-1; i++)" << endl;
   fout << "      fWeights[0][i]=inputValues[i];" << endl;
   fout << endl;
   for (Int_t i = 0; i < numLayers-1; i++) {
      fout << "   // layer " << i << " to " << i+1 << endl;
      if (i+1 == numLayers-1) {
         fout << "   for (int o=0; o<fLayerSize[" << i+1 << "]; o++) {" << endl;
      } 
      else {
         fout << "   for (int o=0; o<fLayerSize[" << i+1 << "]-1; o++) {" << endl;
      }
      fout << "      for (int i=0; i<fLayerSize[" << i << "]; i++) {" << endl;
      fout << "         double inputVal = fWeightMatrix" << i << "to" << i+1 << "[o][i] * fWeights[" << i << "][i];" << endl;

      if ( fNeuronInputType == "sum") {
         fout << "         fWeights[" << i+1 << "][o] += inputVal;" << endl;
      } 
      else if ( fNeuronInputType == "sqsum") {
         fout << "         fWeights[" << i+1 << "][o] += inputVal*inputVal;" << endl;
      } 
      else { // fNeuronInputType == TNeuronInputChooser::kAbsSum
         fout << "         fWeights[" << i+1 << "][o] += fabs(inputVal);" << endl;
      }
      fout << "      }" << endl;
      if (i+1 != numLayers-1) // in the last layer no activation function is applied
         fout << "      fWeights[" << i+1 << "][o] = ActivationFnc(fWeights[" << i+1 << "][o]);" << endl;
      fout << "   }" << endl;
   }
   fout << endl;
   fout << "   return fWeights[" << numLayers-1 << "][0];" << endl;   
   fout << "}" << endl;

   fout << endl;
   TString fncName = className+"::ActivationFnc";
   fActivation->MakeFunction(fout, fncName);

   fout << "   " << endl;
   fout << "// Clean up" << endl;
   fout << "inline void " << className << "::Clear() " << endl;
   fout << "{" << endl;
   fout << "   // nothing to clear" << endl;
   fout << "}" << endl;
}
