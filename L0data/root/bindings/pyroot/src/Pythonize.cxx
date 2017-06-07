// @(#)root/pyroot:$Id: Pythonize.cxx 31373 2009-11-23 06:08:42Z wlav $
// Author: Wim Lavrijsen, Jul 2004

// Bindings
#include "PyROOT.h"
#include "PyStrings.h"
#include "Pythonize.h"
#include "ObjectProxy.h"
#include "MethodProxy.h"
#include "RootWrapper.h"
#include "Utility.h"
#include "PyCallable.h"
#include "PyBufferFactory.h"
#include "FunctionHolder.h"
#include "Converters.h"
#include "MemoryRegulator.h"
#include "Adapters.h"
#include "Utility.h"

// ROOT
#include "TClass.h"
#include "TMethod.h"
#include "TCollection.h"
#include "TDirectory.h"
#include "TSeqCollection.h"
#include "TClonesArray.h"
#include "TObject.h"
#include "TFunction.h"
#include "TError.h"

#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"

// CINT
#include "Api.h"

// Standard
#include <stdexcept>
#include <string>
#include <stdio.h>
#include <utility>


namespace {

// for convenience
   using namespace PyROOT;

//____________________________________________________________________________
   Bool_t HasAttrDirect( PyObject* pyclass, PyObject* pyname, Bool_t mustBePyROOT = kFALSE ) {
   // prevents calls to pyclass->ob_type->tp_getattr, which is unnecessary for our
   // purposes here and could tickle problems w/ spurious lookups into ROOT meta
      PyObject* attr = PyType_Type.tp_getattro( pyclass, pyname );
      if ( attr != 0 && ( ! mustBePyROOT || MethodProxy_Check( attr ) ) ) {
         Py_DECREF( attr );
         return kTRUE;
      }

      PyErr_Clear();
      return kFALSE;
   }

//____________________________________________________________________________
   inline Bool_t IsTemplatedSTLClass( const std::string& name, const std::string& klass ) {
      const int nsize = (int)name.size();
      const int ksize = (int)klass.size();

      return ( ( ksize   < nsize && name.substr(0,ksize) == klass ) ||
               ( ksize+5 < nsize && name.substr(5,ksize) == klass ) ) &&
             name.find( "::", name.find( ">" ) ) == std::string::npos;
   }

// to prevent compiler warnings about const char* -> char*
   inline PyObject* CallPyObjMethod( PyObject* obj, const char* meth )
   {
      return PyObject_CallMethod( obj, const_cast< char* >( meth ), const_cast< char* >( "" ) );
   }

//____________________________________________________________________________
   inline PyObject* CallPyObjMethod( PyObject* obj, const char* meth, PyObject* arg1 )
   {
      return PyObject_CallMethod(
         obj, const_cast< char* >( meth ), const_cast< char* >( "O" ), arg1 );
   }

//____________________________________________________________________________
   inline PyObject* CallPyObjMethod(
      PyObject* obj, const char* meth, PyObject* arg1, PyObject* arg2 )
   {
      return PyObject_CallMethod(
         obj, const_cast< char* >( meth ), const_cast< char* >( "OO" ), arg1, arg2 );
   }

//____________________________________________________________________________
   inline PyObject* CallPyObjMethod( PyObject* obj, const char* meth, PyObject* arg1, int arg2 )
   {
      return PyObject_CallMethod(
         obj, const_cast< char* >( meth ), const_cast< char* >( "Oi" ), arg1, arg2 );
   }


//- helpers --------------------------------------------------------------------
   PyObject* PyStyleIndex( PyObject* self, PyObject* index )
   {
      Py_ssize_t idx = PyInt_AsSsize_t( index );
      if ( idx == (Py_ssize_t)-1 && PyErr_Occurred() )
         return 0;

      Py_ssize_t size = PySequence_Size( self );
      if ( idx >= size || ( idx < 0 && idx < -size ) ) {
         PyErr_SetString( PyExc_IndexError, "index out of range" );
         return 0;
      }

      PyObject* pyindex = 0;
      if ( idx >= 0 ) {
         Py_INCREF( index );
         pyindex = index;
      } else
         pyindex = PyLong_FromLong( size + idx );

      return pyindex;
   }

//____________________________________________________________________________
   inline PyObject* CallSelfIndex( ObjectProxy* self, PyObject* idx, const char* meth )
   {
      PyObject* pyindex = PyStyleIndex( (PyObject*)self, idx );
      if ( ! pyindex )
         return 0;

      PyObject* result = CallPyObjMethod( (PyObject*)self, meth, pyindex );
      Py_DECREF( pyindex );
      return result;
   }

//- "smart pointer" behavior ---------------------------------------------------
   PyObject* DeRefGetAttr( PyObject* self, PyObject* name )
   {
      if ( ! PyString_Check( name ) )
         PyErr_SetString( PyExc_TypeError, "getattr(): attribute name must be string" );

      PyObject* pyptr = CallPyObjMethod( self, "__deref__" );
      if ( ! pyptr )
         return 0;

   // prevent a potential infinite loop
      if ( pyptr->ob_type == self->ob_type ) {
         PyObject* val1 = PyObject_Str( self );
         PyObject* val2 = PyObject_Str( name );
         PyErr_Format( PyExc_AttributeError, "%s has no attribute \'%s\'",
            PyString_AsString( val1 ), PyString_AsString( val2 ) );
         Py_DECREF( val2 );
         Py_DECREF( val1 );

         Py_DECREF( pyptr );
         return 0;
      }

      PyObject* result = PyObject_GetAttr( pyptr, name );
      Py_DECREF( pyptr );
      return result;
   }

//____________________________________________________________________________
   PyObject* FollowGetAttr( PyObject* self, PyObject* name )
   {
      if ( ! PyString_Check( name ) )
         PyErr_SetString( PyExc_TypeError, "getattr(): attribute name must be string" );

      PyObject* pyptr = CallPyObjMethod( self, "__follow__" );
      if ( ! pyptr )
         return 0;

      PyObject* result = PyObject_GetAttr( pyptr, name );
      Py_DECREF( pyptr );
      return result;
   }

//- TObject behavior -----------------------------------------------------------
   PyObject* TObjectContains( PyObject* self, PyObject* obj )
   {
      if ( ! ( ObjectProxy_Check( obj ) || PyString_Check( obj ) ) )
         return PyInt_FromLong( 0l );

      PyObject* found = CallPyObjMethod( self, "FindObject", obj );
      PyObject* result = PyInt_FromLong( PyObject_IsTrue( found ) );
      Py_DECREF( found );
      return result;
   }

//____________________________________________________________________________
   PyObject* TObjectCompare( PyObject* self, PyObject* obj )
   {
      if ( ! ObjectProxy_Check( obj ) )
         return PyInt_FromLong( -1l );

      return CallPyObjMethod( self, "Compare", obj );
   }

//____________________________________________________________________________
   PyObject* TObjectIsEqual( PyObject* self, PyObject* obj )
   {
      if ( ! ObjectProxy_Check( obj ) || ! ((ObjectProxy*)obj)->fObject )
         return ObjectProxy_Type.tp_richcompare( self, obj, Py_EQ );

      return CallPyObjMethod( self, "IsEqual", obj );
   }

//____________________________________________________________________________
   PyObject* GenObjectIsEqual( PyObject* self, PyObject* obj )
   {
   // Contrary to TObjectIsEqual, it can now not be relied upon that the only
   // non-ObjectProxy obj is None, as any operator==(), taking any object (e.g.
   // an enum) can be implemented. However, those cases will yield an exception
   // if presented with None.
      PyObject* result = CallPyObjMethod( self, "__cpp_eq__", obj );
      if ( ! result ) {
         PyErr_Clear();
         result = ObjectProxy_Type.tp_richcompare( self, obj, Py_EQ );
      }

      return result;
   }

//____________________________________________________________________________
   PyObject* GenObjectIsNotEqual( PyObject* self, PyObject* obj )
   {
   // Reverse of GenObjectIsEqual, if operator!= defined
      PyObject* result = CallPyObjMethod( self, "__cpp_ne__", obj );
      if ( ! result ) {
         PyErr_Clear();
         result = ObjectProxy_Type.tp_richcompare( self, obj, Py_NE );
      }

      return result;
   }

//- TClass behavior ------------------------------------------------------------
   PyObject* TClassStaticCast( ObjectProxy* self, PyObject* args )
   {
   // Implemented somewhat different than TClass::DynamicClass, in that "up" is
   // chosen automatically based on the relationship between self and arg pyclass.
      ObjectProxy* pyclass = 0; PyObject* pyobject = 0;
      if ( ! PyArg_ParseTuple( args, const_cast< char* >( "O!O:StaticCast" ),
              &ObjectProxy_Type, &pyclass, &pyobject ) )
         return 0;

   // check the given arguments (dcasts are necessary b/c of could be a TQClass
      TClass* from =
         (TClass*)self->ObjectIsA()->DynamicCast( TClass::Class(), self->GetObject() );
      TClass* to   =
         (TClass*)pyclass->ObjectIsA()->DynamicCast( TClass::Class(), pyclass->GetObject() );

      if ( ! from ) {
         PyErr_SetString( PyExc_TypeError, "unbound method TClass::StaticCast "
            "must be called with a TClass instance as first argument" );
         return 0;
      }

      if ( ! to ) {
         PyErr_SetString( PyExc_TypeError, "could not convert argument 1 (TClass* expected)" );
         return 0;
      }

   // retrieve object address
      void* address = 0;
      if ( ObjectProxy_Check( pyobject ) ) address = ((ObjectProxy*)pyobject)->GetObject();
      else if ( PyInt_Check( pyobject ) ) address = (void*)PyInt_AS_LONG( pyobject );
      else Utility::GetBuffer( pyobject, '*', 1, address, kFALSE );

      if ( ! address ) {
         PyErr_SetString( PyExc_TypeError, "could not convert argument 2 (void* expected)" );
         return 0;
      }

   // determine direction of cast
      int up = -1;
      if ( from->InheritsFrom( to ) ) up = 1;
      else if ( to->InheritsFrom( from ) ) {
         TClass* tmp = to; to = from; from = tmp;
         up = 0;
      }

      if ( up == -1 ) {
         PyErr_Format( PyExc_TypeError, "unable to cast %s to %s", from->GetName(), to->GetName() );
         return 0;
      }

   // perform actual cast
      void* result = from->DynamicCast( to, address, (Bool_t)up );

   // at this point, "result" can't be null (but is still safe if it is)
      return BindRootObjectNoCast( result, to );
   }

//____________________________________________________________________________
   PyObject* TClassDynamicCast( ObjectProxy* self, PyObject* args )
   {
      ObjectProxy* pyclass = 0; PyObject* pyobject = 0;
      long up = 1;
      if ( ! PyArg_ParseTuple( args, const_cast< char* >( "O!O|l:DynamicCast" ),
              &ObjectProxy_Type, &pyclass, &pyobject, &up ) )
         return 0;

   // perform actual cast
      PyObject* meth = PyObject_GetAttr( (PyObject*)self, PyStrings::gTClassDynCast );
      PyObject* ptr = meth ? PyObject_Call(
         meth, PyTuple_GetSlice( args, 1, PyTuple_GET_SIZE( args ) ), 0 ) : 0;

   // simply forward in case of call failure
      if ( ! ptr )
         return ptr;

   // supposed to be an int or long ...
      long address = PyLong_AsLong( ptr );
      if ( address == -1 && PyErr_Occurred() ) {
         PyErr_Clear();
         return ptr;
      }

   // now use binding to return a usable class
      TClass* klass = 0;
      if ( up ) {                  // up-cast: result is a base
         klass = (TClass*)pyclass->ObjectIsA()->DynamicCast( TClass::Class(), pyclass->GetObject() );
      } else {                     // down-cast: result is a derived
         klass = (TClass*)self->ObjectIsA()->DynamicCast( TClass::Class(), self->GetObject() );
      }

      PyObject* result = BindRootObjectNoCast( (void*)address, klass );
      Py_DECREF( ptr );
      return result;
   }

//- TCollection behavior -------------------------------------------------------
   PyObject* TCollectionExtend( PyObject* self, PyObject* obj )
   {
      for ( Py_ssize_t i = 0; i < PySequence_Size( obj ); ++i ) {
         PyObject* item = PySequence_GetItem( obj, i );
         PyObject* result = CallPyObjMethod( self, "Add", item );
         Py_XDECREF( result );
         Py_DECREF( item );
      }

      Py_INCREF( Py_None );
      return Py_None;
   }

//____________________________________________________________________________
   PyObject* TCollectionRemove( PyObject* self, PyObject* obj )
   {
      PyObject* result = CallPyObjMethod( self, "Remove", obj );
      if ( ! result )
         return 0;

      if ( ! PyObject_IsTrue( result ) ) {
         Py_DECREF( result );
         PyErr_SetString( PyExc_ValueError, "list.remove(x): x not in list" );
         return 0;
      }

      Py_DECREF( result );
      Py_INCREF( Py_None );
      return Py_None;
   }

//____________________________________________________________________________
   PyObject* TCollectionAdd( PyObject* self, PyObject* other )
   {
      PyObject* l = CallPyObjMethod( self, "Clone" );
      if ( ! l )
         return 0;

      PyObject* result = CallPyObjMethod( l, "extend", other );
      if ( ! result ) {
         Py_DECREF( l );
         return 0;
      }

      return l;
   }

//____________________________________________________________________________
   PyObject* TCollectionMul( ObjectProxy* self, PyObject* pymul )
   {
      Long_t imul = PyLong_AsLong( pymul );
      if ( imul == -1 && PyErr_Occurred() )
         return 0;

      if ( ! self->GetObject() ) {
         PyErr_SetString( PyExc_TypeError, "unsubscriptable object" );
         return 0;
      }

      PyObject* nseq = BindRootObject( self->ObjectIsA()->New(), self->ObjectIsA() );

      for ( Long_t i = 0; i < imul; ++i ) {
         PyObject* result = CallPyObjMethod( nseq, "extend", (PyObject*)self );
         Py_DECREF( result );
      }

      return nseq;
   }

//____________________________________________________________________________
   PyObject* TCollectionIMul( PyObject* self, PyObject* pymul )
   {
      Long_t imul = PyLong_AsLong( pymul );
      if ( imul == -1 && PyErr_Occurred() )
         return 0;

      PyObject* l = PySequence_List( self );

      for ( Long_t i = 0; i < imul - 1; ++i ) {
         CallPyObjMethod( self, "extend", l );
      }

      Py_INCREF( self );
      return self;
   }

//____________________________________________________________________________
   PyObject* TCollectionCount( PyObject* self, PyObject* obj )
   {
      Py_ssize_t count = 0;
      for ( Py_ssize_t i = 0; i < PySequence_Size( self ); ++i ) {
         PyObject* item = PySequence_GetItem( self, i );
         PyObject* found = PyObject_RichCompare( item, obj, Py_EQ );

         Py_DECREF( item );

         if ( ! found )
            return 0;                        // internal problem

         if ( PyObject_IsTrue( found ) )
            count += 1;
         Py_DECREF( found );
      }

      return PyInt_FromSsize_t( count );
   }

//____________________________________________________________________________
   PyObject* TCollectionIter( ObjectProxy* self ) {
      if ( ! self->GetObject() ) {
         PyErr_SetString( PyExc_TypeError, "iteration over non-sequence" );
         return 0;
      }

      TCollection* col =
         (TCollection*)self->ObjectIsA()->DynamicCast( TCollection::Class(), self->GetObject() );

      PyObject* pyobject = BindRootObject( (void*) new TIter( col ), TIter::Class() );
      ((ObjectProxy*)pyobject)->HoldOn();
      return pyobject;
   }


//- TSeqCollection behavior ----------------------------------------------------
   PyObject* TSeqCollectionGetItem( ObjectProxy* self, PySliceObject* index )
   {
      if ( PySlice_Check( index ) ) {
         if ( ! self->GetObject() ) {
            PyErr_SetString( PyExc_TypeError, "unsubscriptable object" );
            return 0;
         }

         TClass* clSeq = self->ObjectIsA();
         TSeqCollection* oseq =
            (TSeqCollection*)clSeq->DynamicCast( TSeqCollection::Class(), self->GetObject() );
         TSeqCollection* nseq = (TSeqCollection*)clSeq->New();

         Py_ssize_t start, stop, step;
         PySlice_GetIndices( index, oseq->GetSize(), &start, &stop, &step );
         for ( Py_ssize_t i = start; i < stop; i += step ) {
            nseq->Add( oseq->At( (Int_t)i ) );
         }

         return BindRootObject( (void*) nseq, clSeq );
      }

      return CallSelfIndex( self, (PyObject*)index, "At" );
   }

//____________________________________________________________________________
   PyObject* TSeqCollectionSetItem( ObjectProxy* self, PyObject* args )
   {
      PyObject* index = 0, *obj = 0;
      if ( ! PyArg_ParseTuple( args,
                const_cast< char* >( "OO:__setitem__" ), &index, &obj ) )
         return 0;

      if ( PySlice_Check( index ) ) {
         if ( ! self->GetObject() ) {
            PyErr_SetString( PyExc_TypeError, "unsubscriptable object" );
            return 0;
         }

         TSeqCollection* oseq = (TSeqCollection*)self->ObjectIsA()->DynamicCast(
            TSeqCollection::Class(), self->GetObject() );

         Py_ssize_t start, stop, step;
         PySlice_GetIndices( (PySliceObject*) index, oseq->GetSize(), &start, &stop, &step );
         for ( Py_ssize_t i = stop - step; i >= start; i -= step ) {
            oseq->RemoveAt( (Int_t)i );
         }

         for ( Py_ssize_t i = 0; i < PySequence_Size( obj ); ++i ) {
            ObjectProxy* item = (ObjectProxy*)PySequence_GetItem( obj, i );
            item->Release();
            oseq->AddAt( (TObject*) item->GetObject(), (Int_t)(i + start) );
            Py_DECREF( item );
         }

         Py_INCREF( Py_None );
         return Py_None;
      }

      PyObject* pyindex = PyStyleIndex( (PyObject*)self, index );
      if ( ! pyindex )
         return 0;

      PyObject* result  = CallPyObjMethod( (PyObject*)self, "RemoveAt", pyindex );
      if ( ! result ) {
         Py_DECREF( pyindex );
         return 0;
      }

      Py_DECREF( result );
      result = CallPyObjMethod( (PyObject*)self, "AddAt", obj, pyindex );
      Py_DECREF( pyindex );
      return result;
   }

//____________________________________________________________________________
   PyObject* TSeqCollectionDelItem( ObjectProxy* self, PySliceObject* index )
   {
      if ( PySlice_Check( index ) ) {
         if ( ! self->GetObject() ) {
            PyErr_SetString( PyExc_TypeError, "unsubscriptable object" );
            return 0;
         }

         TSeqCollection* oseq = (TSeqCollection*)self->ObjectIsA()->DynamicCast(
            TSeqCollection::Class(), self->GetObject() );

         Py_ssize_t start, stop, step;
         PySlice_GetIndices( index, oseq->GetSize(), &start, &stop, &step );
         for ( Py_ssize_t i = stop - step; i >= start; i -= step ) {
            oseq->RemoveAt( (Int_t)i );
         }

         Py_INCREF( Py_None );
         return Py_None;
      }

      PyObject* result = CallSelfIndex( self, (PyObject*)index, "RemoveAt" );
      if ( ! result )
         return 0;

      Py_DECREF( result );
      Py_INCREF( Py_None );
      return Py_None;
   }

//____________________________________________________________________________
   PyObject* TSeqCollectionInsert( PyObject* self, PyObject* args )
   {
      PyObject* obj = 0; Long_t idx = 0;
      if ( ! PyArg_ParseTuple( args, const_cast< char* >( "lO:insert" ), &idx, &obj ) )
         return 0;

      Py_ssize_t size = PySequence_Size( self );
      if ( idx < 0 )
         idx = 0;
      else if ( size < idx )
         idx = size;

      return CallPyObjMethod( self, "AddAt", obj, idx );
   }

//____________________________________________________________________________
   PyObject* TSeqCollectionPop( ObjectProxy* self, PyObject* args )
   {
      int nArgs = PyTuple_GET_SIZE( args );
      if ( nArgs == 0 ) {
      // create the default argument 'end of sequence'
         PyObject* index = PyInt_FromSsize_t( PySequence_Size( (PyObject*)self ) - 1 );
         PyObject* result = CallSelfIndex( self, index, "RemoveAt" );
         Py_DECREF( index );
         return result;
      } else if ( nArgs != 1 ) {
         PyErr_Format( PyExc_TypeError,
            "pop() takes at most 1 argument (%d given)", nArgs );
         return 0;
      }

      return CallSelfIndex( self, PyTuple_GET_ITEM( args, 0 ), "RemoveAt" );
   }

//____________________________________________________________________________
   PyObject* TSeqCollectionReverse( PyObject* self )
   {
      PyObject* tup = PySequence_Tuple( self );
      if ( ! tup )
         return 0;

      PyObject* result = CallPyObjMethod( self, "Clear" );
      Py_XDECREF( result );

      for ( Py_ssize_t i = 0; i < PySequence_Size( tup ); ++i ) {
         PyObject* retval = CallPyObjMethod( self, "AddAt", PyTuple_GET_ITEM( tup, i ), 0 );
         Py_XDECREF( retval );
      }

      Py_INCREF( Py_None );
      return Py_None;
   }

//____________________________________________________________________________
   PyObject* TSeqCollectionSort( PyObject* self, PyObject* args )
   {
      if ( PyTuple_GET_SIZE( args ) == 0 ) {
      // no specialized sort, use ROOT one
         return CallPyObjMethod( self, "Sort" );
      } else {
      // sort in a python list copy
         PyObject* l = PySequence_List( self );
         PyObject* result = CallPyObjMethod( l, "sort", PyTuple_GET_ITEM( args, 0 ) );
         Py_XDECREF( result );
         if ( PyErr_Occurred() ) {
            Py_DECREF( l );
            return 0;
         }

         result = CallPyObjMethod( self, "Clear" );
         Py_XDECREF( result );
         result = CallPyObjMethod( self, "extend", l );
         Py_XDECREF( result );
         Py_DECREF( l );

         Py_INCREF( Py_None );
         return Py_None;
      }
   }

//____________________________________________________________________________
   PyObject* TSeqCollectionIndex( PyObject* self, PyObject* obj )
   {
      PyObject* index = CallPyObjMethod( self, "IndexOf", obj );
      if ( ! index )
         return 0;

      if ( PyLong_AsLong( index ) < 0 ) {
         Py_DECREF( index );
         PyErr_SetString( PyExc_ValueError, "list.index(x): x not in list" );
         return 0;
      }

      return index;
   }

//- TClonesArray behavior ------------------------------------------------------
   PyObject* TClonesArraySetItem( ObjectProxy* self, PyObject* args )
   {
   // TClonesArray sets objects by constructing them in-place; which is impossible
   // to support as the python object given as value must exist a priori. It can,
   // however, be memcpy'd and stolen, caveat emptor.
      ObjectProxy* pyobj = 0; PyObject* idx = 0;
      if ( ! PyArg_ParseTuple( args,
               const_cast< char* >( "OO!:__setitem__" ), &idx, &ObjectProxy_Type, &pyobj ) )
         return 0;

      if ( ! self->GetObject() ) {
         PyErr_SetString( PyExc_TypeError, "unsubscriptable object" );
         return 0;
      }

      PyObject* pyindex = PyStyleIndex( (PyObject*)self, idx );
      if ( ! pyindex )
         return 0;
      int index = (int)PyLong_AsLong( pyindex );
      Py_DECREF( pyindex );

   // get hold of the actual TClonesArray
      TClonesArray* cla =
         (TClonesArray*)self->ObjectIsA()->DynamicCast( TClonesArray::Class(), self->GetObject() );

      if ( ! cla ) {
         PyErr_SetString( PyExc_TypeError, "attempt to call with null object" );
         return 0;
      }

      if ( cla->GetClass() != pyobj->ObjectIsA() ) {
         PyErr_Format( PyExc_TypeError, "require object of type %s, but %s given",
            cla->GetClass()->GetName(), pyobj->ObjectIsA()->GetName() );
      }

   // destroy old stuff, if applicable
      if ( ((const TClonesArray&)*cla)[index] ) {
         cla->RemoveAt( index );
      }

      if ( pyobj->GetObject() ) {
      // accessing an entry will result in new, unitialized memory (if properly used)
         TObject* object = (*cla)[index];
         pyobj->Release();
         TMemoryRegulator::RegisterObject( pyobj, object );
         memcpy( (void*)object, pyobj->GetObject(), cla->GetClass()->Size() );
      }

      Py_INCREF( Py_None );
      return Py_None;
   }

//- vector behavior as primitives ----------------------------------------------
   PyObject* VectorGetItem( ObjectProxy* self, PySliceObject* index )
   {
      if ( PySlice_Check( index ) ) {
         if ( ! self->GetObject() ) {
            PyErr_SetString( PyExc_TypeError, "unsubscriptable object" );
            return 0;
         }

         PyObject* pyclass = PyObject_GetAttr( (PyObject*)self, PyStrings::gClass );
         PyObject* nseq = PyObject_CallObject( pyclass, NULL );
         Py_DECREF( pyclass );
 
         Py_ssize_t start, stop, step;
         PySlice_GetIndices( index, PyObject_Length( (PyObject*)self ), &start, &stop, &step );
         for ( Py_ssize_t i = start; i < stop; i += step ) {
            PyObject* pyidx = PyInt_FromSsize_t( i );
            CallPyObjMethod( nseq, "push_back", CallPyObjMethod( (PyObject*)self, "_vector__at", pyidx ) );
            Py_DECREF( pyidx );
         }

         return nseq;
      }

      return CallSelfIndex( self, (PyObject*)index, "_vector__at" );
   }

//- map behavior as primitives ------------------------------------------------
   PyObject* MapContains( PyObject* self, PyObject* obj )
   {
      PyObject* result = 0;

      PyObject* iter = CallPyObjMethod( self, "find", obj );
      if ( ObjectProxy_Check( iter ) ) {
         PyObject* end = CallPyObjMethod( self, "end" );
         if ( ObjectProxy_Check( end ) ) {
            if ( ! PyObject_RichCompareBool( iter, end, Py_EQ ) ) {
               Py_INCREF( Py_True );
               result = Py_True;
            }
         }
         Py_XDECREF( end );
      }
      Py_XDECREF( iter );

      if ( ! result ) {
         PyErr_Clear();            // e.g. wrong argument type, which should always lead to False
         Py_INCREF( Py_False );
         result = Py_False;
      }

      return result;
   }

//- STL container iterator support --------------------------------------------
   PyObject* StlSequenceIter( PyObject* self )
   {
      PyObject* iter = CallPyObjMethod( self, "begin" );
      if ( iter ) {
         PyObject* end = CallPyObjMethod( self, "end" );
         if ( end )
            PyObject_SetAttr( iter, PyStrings::gEnd, end );
         Py_XDECREF( end );
      }
      return iter;
   }

//- safe indexing for STL-like vector w/o iterator dictionaries ---------------
   PyObject* CheckedGetItem( PyObject* self, PyObject* obj )
   {
      Bool_t inbounds = kFALSE;
      Py_ssize_t size = PySequence_Size( self );
      Py_ssize_t idx  = PyInt_AsSsize_t( obj );
      if ( 0 <= idx && 0 <= size && idx < size )
         inbounds = kTRUE;

      if ( inbounds ) {
         return CallPyObjMethod( self, "_getitem__unchecked", obj );
      } else if ( PyErr_Occurred() ) {
      // argument conversion problem: let method itself resolve anew and report
         PyErr_Clear();
         return CallPyObjMethod( self, "_getitem__unchecked", obj );
      } else {
         PyErr_SetString( PyExc_IndexError, "index out of range" );
      }

      return 0;
   }

//- pair as sequence to allow tuple unpacking ---------------------------------
   PyObject* PairUnpack( PyObject* self, PyObject* pyindex )
   {
      Long_t idx = PyLong_AsLong( pyindex );
      if ( idx == -1 && PyErr_Occurred() )
         return 0;

      if ( ! ObjectProxy_Check( self ) || ! ((ObjectProxy*)self)->GetObject() ) {
         PyErr_SetString( PyExc_TypeError, "unsubscriptable object" );
         return 0;
      }

      if ( (int)idx == 0 )
         return PyObject_GetAttr( self, PyStrings::gFirst );
      else if ( (int)idx == 1 )
         return PyObject_GetAttr( self, PyStrings::gSecond );

   // still here? Trigger stop iteration
      PyErr_SetString( PyExc_IndexError, "out of bounds" );
      return 0;
   }

//- string behavior as primitives ----------------------------------------------
#define PYROOT_IMPLEMENT_STRING_PYTHONIZATION( name, func )                   \
   PyObject* name##StringRepr( PyObject* self )                               \
   {                                                                          \
      PyObject* data = CallPyObjMethod( self, #func );                        \
      PyObject* repr = PyString_FromFormat( "\'%s\'", PyString_AsString( data ) ); \
      Py_DECREF( data );                                                      \
      return repr;                                                            \
   }                                                                          \
                                                                              \
   PyObject* name##StringCompare( PyObject* self, PyObject* obj )             \
   {                                                                          \
      PyObject* data = CallPyObjMethod( self, #func );                        \
      int result = PyObject_Compare( data, obj );                             \
      Py_DECREF( data );                                                      \
      if ( PyErr_Occurred() )                                                 \
         return 0;                                                            \
      return PyInt_FromLong( result );                                        \
   }                                                                          \
                                                                              \
   PyObject* name##StringIsequal( PyObject* self, PyObject* obj )             \
   {                                                                          \
      PyObject* data = CallPyObjMethod( self, #func );                        \
      PyObject* result = PyObject_RichCompare( data, obj, Py_EQ );            \
      Py_DECREF( data );                                                      \
      if ( ! result )                                                         \
         return 0;                                                            \
      return result;                                                          \
   }

   PYROOT_IMPLEMENT_STRING_PYTHONIZATION( Stl, c_str )
   PYROOT_IMPLEMENT_STRING_PYTHONIZATION(   T, Data )


//- TObjString behavior --------------------------------------------------------
   PYROOT_IMPLEMENT_STRING_PYTHONIZATION( TObj, GetName )

//____________________________________________________________________________
   PyObject* TObjStringLength( PyObject* self )
   {
      PyObject* data = CallPyObjMethod( self, "GetName" );
      Py_ssize_t size = PySequence_Size( data );
      Py_DECREF( data );
      return PyInt_FromSsize_t( size );
   }


//- TIter behavior -------------------------------------------------------------
   PyObject* TIterIter( PyObject* self )
   {
      Py_INCREF( self );
      return self;
   }

//____________________________________________________________________________
   PyObject* TIterNext( PyObject* self )
   {
      PyObject* next = CallPyObjMethod( self, "Next" );

      if ( ! next )
         return 0;

      if ( ! PyObject_IsTrue( next ) ) {
         Py_DECREF( next );
         PyErr_SetString( PyExc_StopIteration, "" );
         return 0;
      }

      return next;
   }


//- STL iterator behavior ------------------------------------------------------
   PyObject* StlIterNext( PyObject* self )
   {
      PyObject* next = 0;
      PyObject* last = PyObject_GetAttr( self, PyStrings::gEnd );

      if ( last != 0 ) {
      // handle special case of empty container (i.e. self is end)
         if ( PyObject_RichCompareBool( last, self, Py_EQ ) ) {
            PyErr_SetString( PyExc_StopIteration, "" );
         } else {
            PyObject* dummy = PyInt_FromLong( 1l );
            PyObject* iter = CallPyObjMethod( self, "__postinc__", dummy );
            Py_DECREF( dummy );
            if ( iter != 0 ) {
               if ( PyObject_RichCompareBool( last, iter, Py_EQ ) )
                  PyErr_SetString( PyExc_StopIteration, "" );
               else
                  next = CallPyObjMethod( iter, "__deref__" );
            } else {
               PyErr_SetString( PyExc_StopIteration, "" );
            }
            Py_XDECREF( iter );
         }
      } else {
         PyErr_SetString( PyExc_StopIteration, "" );
      }

      Py_XDECREF( last );
      return next;
   }


//- TDirectory member templates ----------------------------------------------
   PyObject* TDirectoryGetObject( ObjectProxy* self, PyObject* args )
   {
      PyObject* name = 0; ObjectProxy* ptr = 0;
      if ( ! PyArg_ParseTuple( args, const_cast< char* >( "SO!:TDirectory::GetObject" ),
               &name, &ObjectProxy_Type, &ptr ) )
         return 0;

      TDirectory* dir =
         (TDirectory*)self->ObjectIsA()->DynamicCast( TDirectory::Class(), self->GetObject() );

      if ( ! dir ) {
         PyErr_SetString( PyExc_TypeError,
           "TDirectory::GetObject must be called with a TDirectory instance as first argument" );
         return 0;
      }

      void* address = dir->GetObjectChecked( PyString_AS_STRING( name ), ptr->ObjectIsA() );
      if ( address ) {
         ptr->Set( address );

         Py_INCREF( Py_None );
         return Py_None;
      }

      PyErr_Format( PyExc_LookupError, "no such object, \"%s\"", PyString_AS_STRING( name ) );
      return 0;
   }

//____________________________________________________________________________
   PyObject* TDirectoryWriteObject( ObjectProxy* self, PyObject* args )
   {
      ObjectProxy *wrt = 0; PyObject *name = 0, *option = 0;
      if ( ! PyArg_ParseTuple( args, const_cast< char* >( "O!OS|S:TDirectory::WriteObject" ),
               &ObjectProxy_Type, &wrt, &name, &option ) )
         return 0;

      TDirectory* dir =
         (TDirectory*)self->ObjectIsA()->DynamicCast( TDirectory::Class(), self->GetObject() );

      if ( ! dir ) {
         PyErr_SetString( PyExc_TypeError,
           "TDirectory::WriteObject must be called with a TDirectory instance as first argument" );
         return 0;
      }

      Int_t result = 0;
      if ( option != 0 ) {
         result = dir->WriteObjectAny( wrt->GetObject(), wrt->ObjectIsA(),
            PyString_AS_STRING( name ), PyString_AS_STRING( option ) );
      } else {
         result = dir->WriteObjectAny(
            wrt->GetObject(), wrt->ObjectIsA(), PyString_AS_STRING( name ) );
      }

      return PyInt_FromLong( (Long_t)result );
   }

}


namespace PyROOT {      // workaround for Intel icc on Linux

//- TTree behavior ------------------------------------------------------------
   PyObject* TTreeGetAttr( ObjectProxy* self, PyObject* pyname )
   {
   // allow access to branches/leaves as if they are data members
      const char* name = PyString_AsString( pyname );
      if ( ! name )
         return 0;

   // get hold of actual tree
      TTree* tree =
         (TTree*)self->ObjectIsA()->DynamicCast( TTree::Class(), self->GetObject() );

      if ( ! tree ) {
         PyErr_SetString( PyExc_ReferenceError, "attempt to access a null-pointer" );
         return 0;
      }

   // search for branch first (typical for objects)
      TBranch* branch = tree->GetBranch( name );
      if ( ! branch ) {
      // for benefit of naming of sub-branches, the actual name may have a trailing '.'
         branch = tree->GetBranch( (std::string( name ) + '.' ).c_str() );
      }

      if ( branch ) {
      // found a branched object, wrap its address for the object it represents
         TClass* klass = TClass::GetClass( branch->GetClassName() );
         if ( klass && branch->GetAddress() )
            return BindRootObjectNoCast( *(char**)branch->GetAddress(), klass );
      }

   // if not, try leaf
      TLeaf* leaf = tree->GetLeaf( name );
      if ( branch && ! leaf ) {
         leaf = branch->GetLeaf( name );
         if ( ! leaf ) {
            TObjArray* leaves = branch->GetListOfLeaves();
     	    if ( leaves->GetSize() && ( leaves->First() == leaves->Last() ) ) {
            // i.e., if unambiguously only this one
               leaf = (TLeaf*)leaves->At( 0 );
            }
         }
      }

      if ( leaf ) {
      // found a leaf, extract value and wrap
         if ( 1 < leaf->GetLenStatic() || leaf->GetLeafCount() ) {
         // array types
            std::string typeName = leaf->GetTypeName();
            TConverter* pcnv = CreateConverter( typeName + '*', leaf->GetNdata() );
            void* address = (void*)leaf->GetValuePointer();
            PyObject* value = pcnv->FromMemory( &address );
            delete pcnv;

            return value;
         } else {
         // value types
            TConverter* pcnv = CreateConverter( leaf->GetTypeName() );
            PyObject* value = pcnv->FromMemory( (void*)leaf->GetValuePointer() );
            delete pcnv;

            return value;
         }
      }

   // confused
      PyErr_Format( PyExc_AttributeError,
          "\'%s\' object has no attribute \'%s\'", tree->IsA()->GetName(), name );
      return 0;
   }

//____________________________________________________________________________
   class TTreeMemberFunction : public PyCallable {
   protected:
      TTreeMemberFunction( MethodProxy* org ) { Py_INCREF( org ); fOrg = org; }
      TTreeMemberFunction( const TTreeMemberFunction& t ) : PyCallable( t )
      {
         Py_INCREF( t.fOrg );
         fOrg = t.fOrg;
      }
      TTreeMemberFunction& operator=( const TTreeMemberFunction& t )
      {
         if ( &t != this ) {
            Py_INCREF( t.fOrg );
            fOrg = t.fOrg;
         }
         return *this;
      }
      ~TTreeMemberFunction() { Py_DECREF( fOrg ); fOrg = 0; }

   public:
      virtual PyObject* GetSignature() { return PyString_FromString( "(...)" ); }
      virtual PyObject* GetPrototype() { return PyObject_GetAttrString( (PyObject*)fOrg, (char*)"__doc__" ); }
      virtual PyObject* GetScope()
      {
         return MakeRootClassFromString< TScopeAdapter, TBaseAdapter, TMemberAdapter >( "TTree" );
      }

   protected:
      MethodProxy* fOrg;
   };

//____________________________________________________________________________
   class TTreeBranch : public TTreeMemberFunction {
   public:
      TTreeBranch( MethodProxy* org ) : TTreeMemberFunction( org ) {}

   public:
      virtual PyObject* operator()( ObjectProxy* self, PyObject* args, PyObject* kwds, Long_t )
      {
      // acceptable signatures:
      //   ( const char*, void*, const char*, Int_t = 32000 )
      //   ( const char*, const char*, T**, Int_t = 32000, Int_t = 99 )
      //   ( const char*, T**, Int_t = 32000, Int_t = 99 ) 
         int argc = PyTuple_GET_SIZE( args );

         if ( 2 <= argc ) {
            TTree* tree =
               (TTree*)self->ObjectIsA()->DynamicCast( TTree::Class(), self->GetObject() );

            if ( ! tree ) {
               PyErr_SetString( PyExc_TypeError,
                  "TTree::Branch must be called with a TTree instance as first argument" );
               return 0;
            }

            PyObject *name = 0, *clName = 0, *leaflist = 0;
            PyObject *address = 0;
            PyObject *bufsize = 0, *splitlevel = 0;

         // try: ( const char*, void*, const char*, Int_t = 32000 )
            if ( PyArg_ParseTuple( args, const_cast< char* >( "SOS|O!:Branch" ),
                    &name, &address, &leaflist, &PyInt_Type, &bufsize ) ) {

               void* buf = 0;
               if ( ObjectProxy_Check( address ) )
                  buf = (void*)((ObjectProxy*)address)->GetObject();
               else
                  Utility::GetBuffer( address, '*', 1, buf, kFALSE );
 
               if ( buf != 0 ) {
                  TBranch* branch = 0;
                  if ( argc == 4 ) {
                     branch = tree->Branch( PyString_AS_STRING( name ), buf,
                        PyString_AS_STRING( leaflist ), PyInt_AS_LONG( bufsize ) );
                  } else {
                     branch = tree->Branch(
                        PyString_AS_STRING( name ), buf, PyString_AS_STRING( leaflist ) );
                  }

                  return BindRootObject( branch, TBranch::Class() );
               }

            }
            PyErr_Clear();

         // try: ( const char*, const char*, T**, Int_t = 32000, Int_t = 99 )
         //  or: ( const char*,              T**, Int_t = 32000, Int_t = 99 ) 
            Bool_t bIsMatch = kFALSE;
            if ( PyArg_ParseTuple( args, const_cast< char* >( "SSO|O!O!:Branch" ),
                    &name, &clName, &address, &PyInt_Type, &bufsize, &PyInt_Type, &splitlevel ) ) {
               bIsMatch = kTRUE;
            } else {
               PyErr_Clear(); clName = 0;    // clName no longer used
               if ( PyArg_ParseTuple( args, const_cast< char* >( "SO|O!O!" ),
                       &name, &address, &PyInt_Type, &bufsize, &PyInt_Type, &splitlevel ) )
                  bIsMatch = kTRUE;
               else
                  PyErr_Clear();
            }

            if ( bIsMatch == kTRUE ) {
               std::string klName = clName ? PyString_AS_STRING( clName ) : "";
               void* buf = 0;

               if ( ObjectProxy_Check( address ) ) {
                  if ( ((ObjectProxy*)address)->fFlags & ObjectProxy::kIsReference )
                     buf = (void*)((ObjectProxy*)address)->fObject;
                  else
                     buf = (void*)&((ObjectProxy*)address)->fObject;

                  if ( ! clName ) {
                     klName = ((ObjectProxy*)address)->ObjectIsA()->GetName();
                     argc += 1;
                  }
               } else
                  Utility::GetBuffer( address, '*', 1, buf, kFALSE );

               if ( buf != 0 && klName != "" ) {
                  TBranch* branch = 0;
                  if ( argc == 3 ) {
                     branch = tree->Branch( PyString_AS_STRING( name ), klName.c_str(), buf );
                  } else if ( argc == 4 ) {
                     branch = tree->Branch( PyString_AS_STRING( name ), klName.c_str(), buf,
                        PyInt_AS_LONG( bufsize ) );
                  } else if ( argc == 5 ) {
                     branch = tree->Branch( PyString_AS_STRING( name ), klName.c_str(), buf,
                        PyInt_AS_LONG( bufsize ), PyInt_AS_LONG( splitlevel ) );
                  }

                  return BindRootObject( branch, TBranch::Class() );
               }
            }
         }

      // still here? Then call original Branch() to reach the other overloads:
         Py_INCREF( (PyObject*)self );
         fOrg->fSelf = self;
         PyObject* result = PyObject_Call( (PyObject*)fOrg, args, kwds );
         fOrg->fSelf = 0;
         Py_DECREF( (PyObject*)self );

         return result;
      }
   };

//____________________________________________________________________________
   class TTreeSetBranchAddress : public TTreeMemberFunction {
   public:
      TTreeSetBranchAddress( MethodProxy* org ) : TTreeMemberFunction( org ) {}

   public:
      virtual PyObject* GetPrototype()
      {
         return PyString_FromString( "TBranch* TTree::SetBranchAddress( ... )" );
      }

      virtual PyObject* operator()( ObjectProxy* self, PyObject* args, PyObject* kwds, Long_t )
      {
      // acceptable signature:
      //   ( const char*, void* )
         int argc = PyTuple_GET_SIZE( args );

         if ( 2 == argc ) {
            TTree* tree =
               (TTree*)self->ObjectIsA()->DynamicCast( TTree::Class(), self->GetObject() );

            if ( ! tree ) {
               PyErr_SetString( PyExc_TypeError,
                  "TTree::SetBranchAddress must be called with a TTree instance as first argument" );
               return 0;
            }

            PyObject *name = 0, *address = 0;

         // try: ( const char*, void* )
            if ( PyArg_ParseTuple( args, const_cast< char* >( "SO:SetBranchAddress" ),
                    &name, &address ) ) {

               void* buf = 0;
               if ( ObjectProxy_Check( address ) ) {
                  if ( ((ObjectProxy*)address)->fFlags & ObjectProxy::kIsReference )
                     buf = (void*)((ObjectProxy*)address)->fObject;
                  else
                     buf = (void*)&((ObjectProxy*)address)->fObject;
               } else
                  Utility::GetBuffer( address, '*', 1, buf, kFALSE );

               if ( buf != 0 ) {
                  tree->SetBranchAddress( PyString_AS_STRING( name ), buf );

                  Py_INCREF( Py_None );
                  return Py_None;
               }
            }
         }

      // still here? Then call original Branch() to reach the other overloads:
         Py_INCREF( (PyObject*)self );
         fOrg->fSelf = self;
         PyObject* result = PyObject_Call( (PyObject*)fOrg, args, kwds );
         fOrg->fSelf = 0;
         Py_DECREF( (PyObject*)self );

         return result;
      }
   };

} // namespace PyROOT


namespace {

// for convenience
   using namespace PyROOT;

//- TFN behavior --------------------------------------------------------------
   int TFNPyCallback( G__value* res, G__CONST char*, struct G__param* libp, int hash )
   {
      PyObject* result = 0;

   // retrieve function information
      Long_t npar = 0;
      PyObject* pyfunc = PyROOT::Utility::GetInstalledMethod( G__value_get_tagnum(res), &npar );
      if ( ! pyfunc )
         return 0;

   // prepare arguments and call
      PyObject* arg1 = BufFac_t::Instance()->PyBuffer_FromMemory(
         (double*)G__int(libp->para[0]), 4 );

      if ( npar != 0 ) {
         PyObject* arg2 = BufFac_t::Instance()->PyBuffer_FromMemory(
            (double*)G__int(libp->para[1]), npar );
         result = PyObject_CallFunction( pyfunc, (char*)"OO", arg1, arg2 );
         Py_DECREF( arg2 );
      } else
         result = PyObject_CallFunction( pyfunc, (char*)"O", arg1 );

      Py_DECREF( arg1 );

   // translate result, throw if an error has occurred
      double d = 0.;
      if ( ! result ) {
         PyErr_Print();
         throw std::runtime_error( "TFN python function call failed" );
      } else {
         d = PyFloat_AsDouble( result );
         Py_DECREF( result );
      }

      G__letdouble( res, 100, d );
      return ( 1 || hash || res || libp );
   }

//- TMinuit behavior ----------------------------------------------------------
   int TMinuitPyCallback( G__value* res, G__CONST char*, struct G__param* libp, int hash )
   {
      PyObject* result = 0;

   // retrieve function information
      PyObject* pyfunc = PyROOT::Utility::GetInstalledMethod( G__value_get_tagnum(res) );
      if ( ! pyfunc )
         return 0;

   // prepare arguments
      PyObject* arg1 = BufFac_t::Instance()->PyBuffer_FromMemory(
         (Int_t*)G__int(libp->para[0]), 1 );
      int npar = G__int(libp->para[0]);
 
      PyObject* arg2 = BufFac_t::Instance()->PyBuffer_FromMemory(
         (Double_t*)G__int(libp->para[1]), npar );

      PyObject* arg3 = PyList_New( 1 );
      PyList_SetItem( arg3, 0, PyFloat_FromDouble( G__double(libp->para[2]) ) );

      PyObject* arg4 = BufFac_t::Instance()->PyBuffer_FromMemory(
         (Double_t*)G__int(libp->para[3]), npar );

   // perform actual call
      result = PyObject_CallFunction( pyfunc, (char*)"OOOOi",
         arg1, arg2, arg3, arg4, (int)G__int(libp->para[4]) );
      *(Double_t*)G__Doubleref(&libp->para[2]) = PyFloat_AsDouble( PyList_GetItem( arg3, 0 ) );

      Py_DECREF( arg4 ); Py_DECREF( arg3 ); Py_DECREF( arg2 ); Py_DECREF( arg1 );

      if ( ! result ) {
         PyErr_Print();
         throw std::runtime_error( "TMinuit python fit function call failed" );
      }

      Py_XDECREF( result );

      G__setnull( res );
      return ( 1 || hash || res || libp );
   }

//____________________________________________________________________________
   class TPretendInterpreted: public PyCallable {
   public:
      TPretendInterpreted( int nArgs ) : fNArgs( nArgs ) {}

   public:
      Int_t GetNArgs() { return fNArgs; }

      Bool_t IsCallable( PyObject* pyobject )
      {
         if ( ! pyobject || ! PyCallable_Check( pyobject ) ) {
            PyObject* str = pyobject ? PyObject_Str( pyobject ) : PyString_FromString( "null pointer" );
            PyErr_Format( PyExc_ValueError,
               "\"%s\" is not a valid python callable", PyString_AS_STRING( str ) );
            Py_DECREF( str );
            return kFALSE;
         }

         return kTRUE;
      }

   private:
      Int_t fNArgs;
   };

//____________________________________________________________________________
   class TF1InitWithPyFunc : public TPretendInterpreted {
      typedef std::pair< PyObject*, int > pairPyObjInt_t;

   public:
      TF1InitWithPyFunc( int ntf = 1 ) : TPretendInterpreted( 2 + 2*ntf ) {}

   public:
      virtual PyObject* GetSignature() { return PyString_FromString( "(...)" ); }
      virtual PyObject* GetPrototype()
      {
         return PyString_FromString(
            "TF1::TF1(const char* name, PyObject* callable, "
            "Double_t xmin, Double_t xmax, Int_t npar = 0)" );
      }

      virtual PyObject* operator()( ObjectProxy* self, PyObject* args, PyObject*, Long_t )
      {
      // expected signature: ( char* name, pyfunc, double xmin, double xmax, int npar = 0 )
         int argc = PyTuple_GET_SIZE( args );
         const int reqNArgs = GetNArgs();
         if ( ! ( argc == reqNArgs || argc == reqNArgs+1 ) ) {
            PyErr_Format( PyExc_TypeError,
               "TFN::TFN(const char*, PyObject* callable, ...) =>\n"
               "    takes at least %d and at most %d arguments (%d given)",
               reqNArgs, reqNArgs+1, argc );
            return 0;              // reported as an overload failure
         }

         PyObject* pyfunc = PyTuple_GET_ITEM( args, 1 );
         if ( ! IsCallable( pyfunc ) )
            return 0;

      // use requested function name as identifier
         const char* name = PyString_AsString( PyTuple_GET_ITEM( args, 0 ) );
         if ( PyErr_Occurred() )
            return 0;

      // verify/setup the callback parameters
         Long_t npar = 0;             // default value if not given
         if ( argc == reqNArgs+1 )
            npar = PyInt_AsLong( PyTuple_GET_ITEM( args, reqNArgs ) );

      // registration with CINT (note: CINT style signature for free functions)
         Long_t fid = Utility::InstallMethod(
            0, pyfunc, name, "D - - 0 - - D - - 0 - -", (void*)TFNPyCallback, 2, npar );

      // get constructor
         MethodProxy* method =
            (MethodProxy*)PyObject_GetAttr( (PyObject*)self, PyStrings::gInit );

      // build new argument array
         PyObject* newArgs = PyTuple_New( reqNArgs + 1 );

         for ( int iarg = 0; iarg < argc; ++iarg ) {
            PyObject* item = PyTuple_GET_ITEM( args, iarg );
            if ( iarg != 1 ) {
               Py_INCREF( item );
               PyTuple_SET_ITEM( newArgs, iarg, item );
            } else {
               PyTuple_SET_ITEM( newArgs, iarg, PyCObject_FromVoidPtr( (void*)fid, NULL ) );
            }
         }

         if ( argc == reqNArgs )             // meaning: use default for last value
            PyTuple_SET_ITEM( newArgs, reqNArgs, PyInt_FromLong( 0l ) );

      // re-run
         PyObject* result = PyObject_CallObject( (PyObject*)method, newArgs );

      // done, may have worked, if not: 0 is returned
         Py_DECREF( newArgs );
         Py_DECREF( method );
         return result;
      }
   };

//____________________________________________________________________________
   class TF2InitWithPyFunc : public TF1InitWithPyFunc {
   public:
      TF2InitWithPyFunc() : TF1InitWithPyFunc( 2 ) {}

   public:
      virtual PyObject* GetPrototype()
      {
         return PyString_FromString(
            "TF2::TF2(const char* name, PyObject* callable, "
            "Double_t xmin, Double_t xmax, "
            "Double_t ymin, Double_t ymax, Int_t npar = 0)" );
      }
   };

//____________________________________________________________________________
   class TF3InitWithPyFunc : public TF1InitWithPyFunc {
   public:
      TF3InitWithPyFunc() : TF1InitWithPyFunc( 3 ) {}

   public:
      virtual PyObject* GetPrototype()
      {
         return PyString_FromString(
            "TF3::TF3(const char* name, PyObject* callable, "
            "Double_t xmin, Double_t xmax, "
            "Double_t ymin, Double_t ymax, "
            "Double_t zmin, Double_t zmax, Int_t npar = 0)" );
      }
   };


//- TFunction behavior ---------------------------------------------------------
   PyObject* TFunctionCall( ObjectProxy* self, PyObject* args ) {
      return TFunctionHolder< TScopeAdapter, TMemberAdapter >( (TFunction*)self->GetObject() )( self, args, 0 );
   }


//- TMinuit behavior -----------------------------------------------------------
   class TMinuitSetFCN : public TPretendInterpreted {
   public:
      TMinuitSetFCN() : TPretendInterpreted( 1 ) {}

   public:
      virtual PyObject* GetSignature() { return PyString_FromString( "(PyObject* callable)" ); }
      virtual PyObject* GetPrototype()
      {
         return PyString_FromString(
            "TMinuit::SetFCN(PyObject* callable)" );
      }

      virtual PyObject* operator()( ObjectProxy* self, PyObject* args, PyObject*, Long_t )
      {
      // expected signature: ( pyfunc )
         int argc = PyTuple_GET_SIZE( args );
         if ( argc != 1 ) {
            PyErr_Format( PyExc_TypeError,
               "TMinuit::SetFCN(PyObject* callable, ...) =>\n"
               "    takes exactly 1 argument (%d given)", argc );
            return 0;              // reported as an overload failure
         }

         PyObject* pyfunc = PyTuple_GET_ITEM( args, 0 );
         if ( ! IsCallable( pyfunc ) )
            return 0;

      // use callable name (if available) as identifier
         PyObject* pyname = PyObject_GetAttr( pyfunc, PyStrings::gName );
         const char* name = "dummy";
         if ( pyname != 0 )
            name = PyString_AsString( pyname );

      // registration with CINT (note: CINT style signature for free functions)
         Long_t fid = Utility::InstallMethod( 0, pyfunc, name,
            "i - - 1 - - D - - 0 - - d - - 1 - - D - - 0 - - i - - 0 - -",
            (void*)TMinuitPyCallback, 5 );

      // get function
         MethodProxy* method =
            (MethodProxy*)PyObject_GetAttr( (PyObject*)self, PyStrings::gSetFCN );

      // build new argument array
         PyObject* newArgs = PyTuple_New( 1 );
         PyTuple_SET_ITEM( newArgs, 0, PyCObject_FromVoidPtr( (void*)fid, NULL ) );

      // re-run
         PyObject* result = PyObject_CallObject( (PyObject*)method, newArgs );

      // done, may have worked, if not: 0 is returned
         Py_DECREF( newArgs );
         Py_DECREF( method );
         return result;
      }
   };

//- TMinuit behavior -----------------------------------------------------------


} // unnamed namespace


//- public functions -----------------------------------------------------------
Bool_t PyROOT::Pythonize( PyObject* pyclass, const std::string& name )
{
   if ( pyclass == 0 )
      return kFALSE;


//- method name based pythonization --------------------------------------------

// for smart pointer style classes (note fall-through)
   if ( HasAttrDirect( pyclass, PyStrings::gDeref ) ) {
      Utility::AddToClass( pyclass, "__getattr__", (PyCFunction) DeRefGetAttr, METH_O );
   } else if ( HasAttrDirect( pyclass, PyStrings::gFollow ) ) {
      Utility::AddToClass( pyclass, "__getattr__", (PyCFunction) FollowGetAttr, METH_O );
   }

// for STL containers, and user classes modeled after them
   if ( HasAttrDirect( pyclass, PyStrings::gSize ) )
      Utility::AddToClass( pyclass, "__len__", "size" );

   if ( HasAttrDirect( pyclass, PyStrings::gBegin ) && HasAttrDirect( pyclass, PyStrings::gEnd ) ) {
   // some classes may not have dicts for their iterators, making begin/end useless
      PyObject* pyfullname = PyObject_GetAttr( pyclass, PyStrings::gName );
      TClass* klass = TClass::GetClass( PyString_AS_STRING( pyfullname ) );
      Py_DECREF( pyfullname );
      TMethod* meth = klass->GetMethodAllAny( "begin" );

      TClass* iklass = 0;
      if ( meth ) {
         Int_t oldl = gErrorIgnoreLevel; gErrorIgnoreLevel = 3000;
         iklass = TClass::GetClass( meth->GetReturnTypeName() );
         gErrorIgnoreLevel = oldl;
      }

      if ( iklass && iklass->GetClassInfo() ) {
         Utility::AddToClass( pyclass, "__iter__", (PyCFunction) StlSequenceIter, METH_NOARGS );
      } else if ( HasAttrDirect( pyclass, PyStrings::gGetItem ) && HasAttrDirect( pyclass, PyStrings::gLen ) ) {
         Utility::AddToClass( pyclass, "_getitem__unchecked", "__getitem__" );
         Utility::AddToClass( pyclass, "__getitem__", (PyCFunction) CheckedGetItem, METH_O );
      }
   }

// map operator==() through GenObjectIsEqual to allow comparison to None
   if ( HasAttrDirect( pyclass, PyStrings::gEq, kTRUE ) ) {
      Utility::AddToClass( pyclass, "__cpp_eq__",  "__eq__" );
      Utility::AddToClass( pyclass, "__eq__",  (PyCFunction) GenObjectIsEqual, METH_O );
   }

// map operator!=() through GenObjectIsNotEqual to allow comparison to None
   if ( HasAttrDirect( pyclass, PyStrings::gNe, kTRUE ) ) {
      Utility::AddToClass( pyclass, "__cpp_ne__",  "__ne__" );
      Utility::AddToClass( pyclass, "__ne__",  (PyCFunction) GenObjectIsNotEqual, METH_O );
   }


//- class name based pythonization ---------------------------------------------

   if ( name == "TObject" ) {
   // support for the 'in' operator
      Utility::AddToClass( pyclass, "__contains__", (PyCFunction) TObjectContains, METH_O );

   // comparing for lists
      Utility::AddToClass( pyclass, "__cmp__", (PyCFunction) TObjectCompare, METH_O );
      Utility::AddToClass( pyclass, "__eq__",  (PyCFunction) TObjectIsEqual, METH_O );

      return kTRUE;
   }

   if ( name == "TClass" ) {
   // make DynamicCast return a usable python object, rather than void*
      Utility::AddToClass( pyclass, "_TClass__DynamicCast", "DynamicCast" );
      Utility::AddToClass( pyclass, "DynamicCast", (PyCFunction) TClassDynamicCast );

   // the following cast is easier to use (reads both ways)
      Utility::AddToClass( pyclass, "StaticCast", (PyCFunction) TClassStaticCast );

      return kTRUE;
   }

   if ( name == "TCollection" ) {
      Utility::AddToClass( pyclass, "append",   "Add" );
      Utility::AddToClass( pyclass, "extend",   (PyCFunction) TCollectionExtend, METH_O );
      Utility::AddToClass( pyclass, "remove",   (PyCFunction) TCollectionRemove, METH_O );
      Utility::AddToClass( pyclass, "__add__",  (PyCFunction) TCollectionAdd, METH_O );
      Utility::AddToClass( pyclass, "__imul__", (PyCFunction) TCollectionIMul, METH_O );
      Utility::AddToClass( pyclass, "__mul__",  (PyCFunction) TCollectionMul, METH_O );
      Utility::AddToClass( pyclass, "__rmul__", (PyCFunction) TCollectionMul, METH_O );

      Utility::AddToClass( pyclass, "count", (PyCFunction) TCollectionCount, METH_O );

      Utility::AddToClass( pyclass, "__len__",  "GetSize" );
      Utility::AddToClass( pyclass, "__iter__", (PyCFunction) TCollectionIter, METH_NOARGS );

      return kTRUE;
   }

   if ( name == "TSeqCollection" ) {
      Utility::AddToClass( pyclass, "__getitem__", (PyCFunction) TSeqCollectionGetItem, METH_O );
      Utility::AddToClass( pyclass, "__setitem__", (PyCFunction) TSeqCollectionSetItem );
      Utility::AddToClass( pyclass, "__delitem__", (PyCFunction) TSeqCollectionDelItem, METH_O );

      Utility::AddToClass( pyclass, "insert",  (PyCFunction) TSeqCollectionInsert );
      Utility::AddToClass( pyclass, "pop",     (PyCFunction) TSeqCollectionPop );
      Utility::AddToClass( pyclass, "reverse", (PyCFunction) TSeqCollectionReverse, METH_NOARGS );
      Utility::AddToClass( pyclass, "sort",    (PyCFunction) TSeqCollectionSort );

      Utility::AddToClass( pyclass, "index", (PyCFunction) TSeqCollectionIndex, METH_O );

      return kTRUE;
   }

   if ( name == "TClonesArray" ) {
   // restore base TSeqCollection operator[] to prevent random object creation (it's
   // functionality is equivalent to the operator[](int) const of TClonesArray, but
   // there's no guarantee it'll be selected over the non-const version)
      Utility::AddToClass( pyclass, "__getitem__", (PyCFunction) TSeqCollectionGetItem, METH_O );

   // this setitem should be used with as much care as the C++ one
      Utility::AddToClass( pyclass, "__setitem__", (PyCFunction) TClonesArraySetItem );

      return kTRUE;
   }

   if ( IsTemplatedSTLClass( name, "vector" ) ) {

      if ( HasAttrDirect( pyclass, PyStrings::gLen ) ) {
         Utility::AddToClass( pyclass, "_vector__at", "at" );
      // remove iterator that was set earlier (checked __getitem__ will do the trick)
         if ( HasAttrDirect( pyclass, PyStrings::gIter ) )
            PyObject_DelAttr( pyclass, PyStrings::gIter );
      } else if ( HasAttrDirect( pyclass, PyStrings::gGetItem ) ) {
         Utility::AddToClass( pyclass, "_vector__at", "__getitem__" );   // unchecked!
      // if unchecked getitem, use checked iterator protocol (was set above if begin/end)
      }

   // provide a slice-able __getitem__, if possible
      if ( HasAttrDirect( pyclass, PyStrings::gVectorAt ) )
         Utility::AddToClass( pyclass, "__getitem__", (PyCFunction) VectorGetItem, METH_O );

      return kTRUE;
   }

   if ( IsTemplatedSTLClass( name, "map" ) ) {
      Utility::AddToClass( pyclass, "__contains__", (PyCFunction) MapContains, METH_O );

      return kTRUE;
   }

   if ( IsTemplatedSTLClass( name, "pair" ) ) {
      Utility::AddToClass( pyclass, "__getitem__", (PyCFunction) PairUnpack, METH_O );

      return kTRUE;
   }

   if ( name.find( "iterator" ) != std::string::npos ) {
      Utility::AddToClass( pyclass, "next", (PyCFunction) StlIterNext, METH_NOARGS );

      return kTRUE;
   }

   if ( name == "string" || name == "std::string" ) {
      Utility::AddToClass( pyclass, "__repr__", (PyCFunction) StlStringRepr, METH_NOARGS );
      Utility::AddToClass( pyclass, "__str__", "c_str" );
      Utility::AddToClass( pyclass, "__cmp__", (PyCFunction) StlStringCompare, METH_O );
      Utility::AddToClass( pyclass, "__eq__",  (PyCFunction) StlStringIsequal, METH_O );

      return kTRUE;
   }

   if ( name == "TString" ) {
      Utility::AddToClass( pyclass, "__repr__", (PyCFunction) TStringRepr, METH_NOARGS );
      Utility::AddToClass( pyclass, "__str__", "Data" );
      Utility::AddToClass( pyclass, "__len__", "Length" );

      Utility::AddToClass( pyclass, "__cmp__", "CompareTo" );
      Utility::AddToClass( pyclass, "__eq__",  (PyCFunction) TStringIsequal, METH_O );

      return kTRUE;
   }

   if ( name == "TObjString" ) {
      Utility::AddToClass( pyclass, "__repr__", (PyCFunction) TObjStringRepr, METH_NOARGS );
      Utility::AddToClass( pyclass, "__str__",  "GetName" );
      Utility::AddToClass( pyclass, "__len__",  (PyCFunction) TObjStringLength, METH_NOARGS );

      Utility::AddToClass( pyclass, "__cmp__", (PyCFunction) TObjStringCompare, METH_O );
      Utility::AddToClass( pyclass, "__eq__",  (PyCFunction) TObjStringIsequal, METH_O );

      return kTRUE;
   }

   if ( name == "TIter" ) {
      Utility::AddToClass( pyclass, "__iter__", (PyCFunction) TIterIter, METH_NOARGS );
      Utility::AddToClass( pyclass, "next",     (PyCFunction) TIterNext, METH_NOARGS );

      return kTRUE;
   }

   if ( name == "TDirectory" ) {
   // note: this replaces the already existing TDirectory::GetObject()
      Utility::AddToClass( pyclass, "GetObject", (PyCFunction) TDirectoryGetObject );

   // note: this replaces the already existing TDirectory::WriteObject()
      Utility::AddToClass( pyclass, "WriteObject", (PyCFunction) TDirectoryWriteObject );

      return kTRUE;
   }

   if ( name == "TTree" ) {
   // allow direct browsing of the tree
      Utility::AddToClass( pyclass, "__getattr__", (PyCFunction) TTreeGetAttr, METH_O );

   // workaround for templated member Branch()
      MethodProxy* original = (MethodProxy*)PyObject_GetAttr( pyclass, PyStrings::gBranch );
      MethodProxy* method = MethodProxy_New( "Branch", new TTreeBranch( original ) );
      Py_DECREF( original ); original = 0;

      PyObject_SetAttrString(
         pyclass, const_cast< char* >( method->GetName().c_str() ), (PyObject*)method );
      Py_DECREF( method ); method = 0;

   // workaround for templated member SetBranchAddress()
      original = (MethodProxy*)PyObject_GetAttr( pyclass, PyStrings::gSetBranchAddress );
      method = MethodProxy_New( "SetBranchAddress", new TTreeSetBranchAddress( original ) );
      Py_DECREF( original ); original = 0;

      PyObject_SetAttrString(
         pyclass, const_cast< char* >( method->GetName().c_str() ), (PyObject*)method );
      Py_DECREF( method ); method = 0;

      return kTRUE;
   }

   if ( name == "TF1" )       // allow instantiation with python callable
      return Utility::AddToClass( pyclass, "__init__", new TF1InitWithPyFunc );

   if ( name == "TF2" )       // allow instantiation with python callable
      return Utility::AddToClass( pyclass, "__init__", new TF2InitWithPyFunc );

   if ( name == "TF3" )       // allow instantiation with python callable
      return Utility::AddToClass( pyclass, "__init__", new TF3InitWithPyFunc );

   if ( name == "TFunction" ) // allow direct call
      return Utility::AddToClass( pyclass, "__call__", (PyCFunction) TFunctionCall );

   if ( name == "TMinuit" )   // allow call with python callable
      return Utility::AddToClass( pyclass, "SetFCN", new TMinuitSetFCN );

   if ( name == "TFile" )     // allow member-style access to entries in file
      return Utility::AddToClass( pyclass, "__getattr__", "Get" );

   if ( name.substr(0,8) == "TVectorT" ) {  // allow proper iteration
      Utility::AddToClass( pyclass, "__len__", "GetNoElements" );
      Utility::AddToClass( pyclass, "_getitem__unchecked", "__getitem__" );
      Utility::AddToClass( pyclass, "__getitem__", (PyCFunction) CheckedGetItem, METH_O );
   }

// default (no pythonization) is by definition ok
   return kTRUE;
}
