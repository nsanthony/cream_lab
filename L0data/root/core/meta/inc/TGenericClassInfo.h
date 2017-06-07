// @(#)root/base:$Id: TGenericClassInfo.h 30815 2009-10-20 13:49:22Z rdm $
// Author: Philippe Canal   23/2/02

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun, Fons Rademakers and al.           *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGenericClassInfo
#define ROOT_TGenericClassInfo

#include <vector>
//#ifndef ROOT_TSchemaHelper
#include "TSchemaHelper.h"
//#endif
//#ifndef ROOT_Rtypes
//#inlcude "Rtypes.h"
//#endif

// Forward declarations
class TVirtualIsAProxy;
//class TClass;
//class TClassStreamer;
//class TVirtualCollectionProxy;
//class TCollectionProxyInfo;


namespace ROOT {

   class TCollectionProxyInfo;

   class TGenericClassInfo {
      // This class in not inlined because it is used is non time critical
      // section (the dictionaries) and inline would lead to too much
      // repetition of the code (once per class!).

      const TInitBehavior        *fAction;
      TClass                     *fClass;
      const char                 *fClassName;
      const char                 *fDeclFileName;
      Int_t                       fDeclFileLine;
      VoidFuncPtr_t               fDictionary;
      const type_info            &fInfo;
      const char                 *fImplFileName;
      Int_t                       fImplFileLine;
      TVirtualIsAProxy           *fIsA;
      void                       *fShowMembers;
      Int_t                       fVersion;
      NewFunc_t                   fNew;
      NewArrFunc_t                fNewArray;
      DelFunc_t                   fDelete;
      DelArrFunc_t                fDeleteArray;
      DesFunc_t                   fDestructor;
      DirAutoAdd_t                fDirAutoAdd;
      TClassStreamer             *fStreamer;
      TVirtualCollectionProxy    *fCollectionProxy;
      Int_t                       fSizeof;
      TCollectionProxyInfo       *fCollectionProxyInfo;
      TCollectionProxyInfo       *fCollectionStreamerInfo;
      std::vector<TSchemaHelper>  fReadRules;
      std::vector<TSchemaHelper>  fReadRawRules;

   public:
      TGenericClassInfo(const char *fullClassname,
                       const char *declFileName, Int_t declFileLine,
                       const type_info &info, const TInitBehavior *action,
                       void *showmembers, VoidFuncPtr_t dictionary,
                       TVirtualIsAProxy *isa, Int_t pragmabits, Int_t sizof);

      TGenericClassInfo(const char *fullClassname, Int_t version,
                       const char *declFileName, Int_t declFileLine,
                       const type_info &info, const TInitBehavior *action,
                       void *showmembers,  VoidFuncPtr_t dictionary,
                       TVirtualIsAProxy *isa, Int_t pragmabits, Int_t sizof);

      TGenericClassInfo(const char *fullClassname, Int_t version,
                       const char *declFileName, Int_t declFileLine,
                       const type_info &info, const TInitBehavior *action,
                       VoidFuncPtr_t dictionary,
                       TVirtualIsAProxy *isa, Int_t pragmabits, Int_t sizof);

      TGenericClassInfo(const char *fullClassname, Int_t version,
                        const char *declFileName, Int_t declFileLine,
                        const TInitBehavior *action,
                        VoidFuncPtr_t dictionary, Int_t pragmabits);

      void Init(Int_t pragmabits);
      ~TGenericClassInfo();

      const TInitBehavior              &GetAction() const;
      TClass                           *GetClass();
      const char                       *GetClassName() const;
      TCollectionProxyInfo             *GetCollectionProxyInfo() const;
      TCollectionProxyInfo             *GetCollectionStreamerInfo() const;
      const char                       *GetDeclFileName() const;
      Int_t                             GetDeclFileLine() const;
      DelFunc_t                         GetDelete() const;
      DelArrFunc_t                      GetDeleteArray() const;
      DesFunc_t                         GetDestructor() const;
      DirAutoAdd_t                      GetDirectoryAutoAdd() const;
      const char                       *GetImplFileName();
      Int_t                             GetImplFileLine();
      const type_info                  &GetInfo() const;
      TVirtualIsAProxy                 *GetIsA() const;
      NewFunc_t                         GetNew() const;
      NewArrFunc_t                      GetNewArray() const;
      const std::vector<TSchemaHelper> &GetReadRawRules() const;
      const std::vector<TSchemaHelper> &GetReadRules() const;
      void                             *GetShowMembers() const;
      Int_t                             GetVersion() const;

      TClass                           *IsA(const void *obj);

      Short_t                           AdoptStreamer(TClassStreamer*);
      Short_t                           AdoptCollectionProxy(TVirtualCollectionProxy*);
      void                              AdoptCollectionProxyInfo(TCollectionProxyInfo*);
      void                              AdoptCollectionStreamerInfo(TCollectionProxyInfo*);
      Int_t                             SetDeclFile(const char *file, Int_t line);
      void                              SetDelete(DelFunc_t deleteFunc);
      void                              SetDeleteArray(DelArrFunc_t deleteArrayFunc);
      void                              SetDestructor(DesFunc_t destructorFunc);
      void                              SetDirectoryAutoAdd(DirAutoAdd_t dirAutoAdd);
      void                              SetFromTemplate();
      Int_t                             SetImplFile(const char *file, Int_t line);
      void                              SetNew(NewFunc_t newFunc);
      void                              SetNewArray(NewArrFunc_t newArrayFunc);
      void                              SetReadRawRules( const std::vector<TSchemaHelper>& rules );
      void                              SetReadRules( const std::vector<TSchemaHelper>& rules );
      Short_t                           SetStreamer(ClassStreamerFunc_t);
      Short_t                           SetVersion(Short_t version);

      //   protected:
   private:
      void CreateRuleSet( std::vector<TSchemaHelper>& vect, Bool_t ProcessReadRules );
      TGenericClassInfo(const TGenericClassInfo&); // Not implemented
      TGenericClassInfo& operator=(const TGenericClassInfo&); // Not implemented

   private:
      TGenericClassInfo();

   };

}

#endif
