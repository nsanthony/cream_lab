// @(#)root/pyroot:$Id: MethodProxy.h 28890 2009-06-11 00:37:35Z wlav $
// Author: Wim Lavrijsen, Jan 2005

#ifndef PYROOT_METHODPROXY_H
#define PYROOT_METHODPROXY_H

// ROOT
#include "DllImport.h"

// Bindings
#include "PyCallable.h"

// Standard
#include <map>
#include <string>
#include <vector>


namespace PyROOT {

/** Method proxy object to hold ROOT methods
      @author  WLAV
      @date    01/07/2005
      @version 1.0
 */

   class MethodProxy {
   public:
      typedef std::map< Long_t, Int_t >  DispatchMap_t;
      typedef std::vector< PyCallable* > Methods_t;

      struct MethodInfo_t {
         MethodInfo_t() : fFlags( kNone ) { fRefCount = new int(1); }
         ~MethodInfo_t();

         enum EMethodInfoFlags {
            kNone            = 0,
            kIsSorted        = 1,       // if method overload priority determined
            kIsCreator       = 2,       // if method creates new objects
            kIsHeuristics    = 4,       // if method requires heuristics memory policy
            kIsStrict        = 8        // if method requires strict memory policy
         };

         std::string                 fName;
         MethodProxy::DispatchMap_t  fDispatchMap;
         MethodProxy::Methods_t      fMethods;
         UInt_t                      fFlags;

         int* fRefCount;
      };

   public:
      void Set( const std::string& name, std::vector< PyCallable* >& methods );

      const std::string& GetName() const { return fMethodInfo->fName; }
      void AddMethod( PyCallable* pc ) {
         fMethodInfo->fFlags &= ~MethodInfo_t::kIsSorted;
         fMethodInfo->fMethods.push_back( pc );
      }

   public:               // public, as the python C-API works with C structs
      PyObject_HEAD
      ObjectProxy*   fSelf;        // The instance it is bound to, or NULL
      MethodInfo_t*  fMethodInfo;

   private:              // private, as the python C-API will handle creation
      MethodProxy() {}
   };


//- method proxy type and type verification ----------------------------------
   R__EXTERN PyTypeObject MethodProxy_Type;

   template< typename T >
   inline Bool_t MethodProxy_Check( T* object )
   {
      return object && PyObject_TypeCheck( object, &MethodProxy_Type );
   }

   template< typename T >
   inline Bool_t MethodProxy_CheckExact( T* object )
   {
      return object && object->ob_type == &MethodProxy_Type;
   }

//- creation -----------------------------------------------------------------
   inline MethodProxy* MethodProxy_New(
         const std::string& name, std::vector< PyCallable* >& methods )
   {
      MethodProxy* pymeth = (MethodProxy*)MethodProxy_Type.tp_new( &MethodProxy_Type, 0, 0 );
      pymeth->Set( name, methods );
      return pymeth;
   }

   inline MethodProxy* MethodProxy_New( const std::string& name, PyCallable* method )
   {
      std::vector< PyCallable* > p;
      p.push_back( method );
      return MethodProxy_New( name, p );
   }

} // namespace PyROOT

#endif // !PYROOT_METHODPROXY_H
