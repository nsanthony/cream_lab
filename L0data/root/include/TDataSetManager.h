// @(#)root/proof:$Id: TDataSetManager.h 30421 2009-09-24 17:20:09Z ganis $
// Author: Jan Fiete Grosse-Oetringhaus, 08.08.07

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TDataSetManager
#define ROOT_TDataSetManager

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDataSetManager                                                 //
//                                                                      //
// This class contains functions to handle datasets in PROOF            //
// It is the layer between TProofServ and the file system that stores   //
// the datasets.                                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef ROOT_TMap
#include "TMap.h"
#endif
#ifndef ROOT_TUri
#include "TUri.h"
#endif


class TFileCollection;
class TMD5;
class TVirtualMonitoringWriter;


class TDataSetManager : public TObject {

private:
   TDataSetManager(const TDataSetManager&);             // not implemented
   TDataSetManager& operator=(const TDataSetManager&);  // not implemented

protected:
   TString  fGroup;         // Group to which the owner of this session belongs
   TString  fUser;          // Owner of the session
   TString  fCommonUser;    // User that stores the COMMON datasets
   TString  fCommonGroup;   // Group that stores the COMMON datasets

   TUri     fBase;          // Base URI used to parse dataset names

   TMap     fGroupQuota;    // Group quotas (read from config file)
   TMap     fGroupUsed;     // <group> --> <used bytes> (TParameter)
   TMap     fUserUsed;      // <group> --> <map of users> --> <value>

   Long64_t fAvgFileSize;   // Average file size to be used to estimate the dataset size (in MB)

   Int_t    fNTouchedFiles; // Number of files touched in the last ScanDataSet operation
   Int_t    fNOpenedFiles;  // Number of files opened in the last ScanDataSet operation
   Int_t    fNDisappearedFiles; // Number of files disappared in the last ScanDataSet operation

   TString  fGroupConfigFile;  // Path to the group config file
   Long_t   fMTimeGroupConfig; // Last modification of the group config file

   static TString fgCommonDataSetTag;  // Name for common datasets, default: COMMON

   virtual TMap *GetGroupUsedMap() { return &fGroupUsed; }
   virtual TMap *GetUserUsedMap() { return &fUserUsed; }
   Int_t    GetNTouchedFiles() const { return fNTouchedFiles; }
   Int_t    GetNOpenedFiles() const { return fNOpenedFiles; }
   Int_t    GetNDisapparedFiles() const { return fNDisappearedFiles; }
   void     GetQuota(const char *group, const char *user, const char *dsName, TFileCollection *dataset);
   void     PrintDataSet(TFileCollection *fc, Int_t popt = 0);
   void     PrintUsedSpace();
   Bool_t   ReadGroupConfig(const char *cf = 0);
   virtual void UpdateUsedSpace();

   static Long64_t ToBytes(const char *size = 0);

public:
   enum EDataSetStatusBits {
      kCheckQuota    = BIT(15),   // quota checking enabled
      kAllowRegister = BIT(16),   // allow registration of a new dataset
      kAllowVerify   = BIT(17),   // allow verification of a dataset (requires registration permit)
      kTrustInfo     = BIT(18),   // during registration, trust the available information provided by the user
      kIsSandbox     = BIT(19)    // dataset dir is in the user sandbox (simplified naming)
   };

   enum EDataSetWorkOpts { // General (bits 1-8)
                           kDebug = 1, kShowDefault = 2, kPrint = 4, kExport = 8,
                           kQuotaUpdate = 16, kSetDefaultTree = 32,
                           // File-based specific (bits 9-16)
                           kReopen = 256, kTouch = 512, kMaxFiles = 1024, kReadShort = 2048,
                           kFileMustExist = 4096};

   TDataSetManager(const char *group = 0, const char *user = 0, const char *options = 0);
   virtual ~TDataSetManager();

   virtual TFileCollection *GetDataSet(const char *uri, const char *server = 0);
   virtual TMap            *GetDataSets(const char *uri, UInt_t /*option*/ = 0);
   virtual TMap            *GetSubDataSets(const char *uri, const char *excludeservers);

   virtual Long64_t         GetGroupQuota(const char *group);
   virtual TMap            *GetGroupQuotaMap() { return &fGroupQuota; }
   virtual Long64_t         GetGroupUsed(const char *group);
   virtual Bool_t           ExistsDataSet(const char *uri);
   virtual void             MonitorUsedSpace(TVirtualMonitoringWriter *monitoring);
   Bool_t                   ParseUri(const char *uri, TString *dsGroup = 0, TString *dsUser = 0,
                                     TString *dsName = 0, TString *dsTree = 0,
                                     Bool_t onlyCurrent = kFALSE, Bool_t wildcards = kFALSE);
   virtual void             ParseInitOpts(const char *opts);
   virtual Bool_t           RemoveDataSet(const char *uri);
   virtual Int_t            RegisterDataSet(const char *uri, TFileCollection *dataSet, const char *opt);
   virtual Int_t            ScanDataSet(const char *uri, UInt_t option = 0);
   virtual void             ShowQuota(const char *opt);

   virtual void             ShowDataSets(const char *uri = "*", const char *opt = "");

   static TString           CreateUri(const char *dsGroup = 0, const char *dsUser = 0,
                                      const char *dsName = 0, const char *dsTree = 0);

   ClassDef(TDataSetManager, 0)  // Abstract data set manager class
};

#endif
