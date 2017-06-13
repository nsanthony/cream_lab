//
// File generated by core/utils/src/rootcint_tmp at Mon Apr 13 13:39:27 2015

// Do NOT change. Changes will be lost next time file is generated
//

#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "rootcint_deque.h"

#include "TCollectionProxyInfo.h"
#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"

// START OF SHADOWS

namespace ROOT {
   namespace Shadow {
   } // of namespace Shadow
} // of namespace ROOT
// END OF SHADOWS

namespace ROOT {
   void dequelEcharmUgR_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void dequelEcharmUgR_Dictionary();
   static void *new_dequelEcharmUgR(void *p = 0);
   static void *newArray_dequelEcharmUgR(Long_t size, void *p);
   static void delete_dequelEcharmUgR(void *p);
   static void deleteArray_dequelEcharmUgR(void *p);
   static void destruct_dequelEcharmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<char*>*)
   {
      deque<char*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<char*>),0);
      static ::ROOT::TGenericClassInfo 
         instance("deque<char*>", -2, "prec_stl/deque", 42,
                  typeid(deque<char*>), DefineBehavior(ptr, ptr),
                  0, &dequelEcharmUgR_Dictionary, isa_proxy, 0,
                  sizeof(deque<char*>) );
      instance.SetNew(&new_dequelEcharmUgR);
      instance.SetNewArray(&newArray_dequelEcharmUgR);
      instance.SetDelete(&delete_dequelEcharmUgR);
      instance.SetDeleteArray(&deleteArray_dequelEcharmUgR);
      instance.SetDestructor(&destruct_dequelEcharmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<char*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const deque<char*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void dequelEcharmUgR_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const deque<char*>*)0x0)->GetClass();
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelEcharmUgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<char*> : new deque<char*>;
   }
   static void *newArray_dequelEcharmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<char*>[nElements] : new deque<char*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelEcharmUgR(void *p) {
      delete ((deque<char*>*)p);
   }
   static void deleteArray_dequelEcharmUgR(void *p) {
      delete [] ((deque<char*>*)p);
   }
   static void destruct_dequelEcharmUgR(void *p) {
      typedef deque<char*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<char*>

namespace ROOT {
   void dequelEdoublegR_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void dequelEdoublegR_Dictionary();
   static void *new_dequelEdoublegR(void *p = 0);
   static void *newArray_dequelEdoublegR(Long_t size, void *p);
   static void delete_dequelEdoublegR(void *p);
   static void deleteArray_dequelEdoublegR(void *p);
   static void destruct_dequelEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<double>*)
   {
      deque<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<double>),0);
      static ::ROOT::TGenericClassInfo 
         instance("deque<double>", -2, "prec_stl/deque", 42,
                  typeid(deque<double>), DefineBehavior(ptr, ptr),
                  0, &dequelEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(deque<double>) );
      instance.SetNew(&new_dequelEdoublegR);
      instance.SetNewArray(&newArray_dequelEdoublegR);
      instance.SetDelete(&delete_dequelEdoublegR);
      instance.SetDeleteArray(&deleteArray_dequelEdoublegR);
      instance.SetDestructor(&destruct_dequelEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const deque<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void dequelEdoublegR_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const deque<double>*)0x0)->GetClass();
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelEdoublegR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<double> : new deque<double>;
   }
   static void *newArray_dequelEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<double>[nElements] : new deque<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelEdoublegR(void *p) {
      delete ((deque<double>*)p);
   }
   static void deleteArray_dequelEdoublegR(void *p) {
      delete [] ((deque<double>*)p);
   }
   static void destruct_dequelEdoublegR(void *p) {
      typedef deque<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<double>

namespace ROOT {
   void dequelEfloatgR_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void dequelEfloatgR_Dictionary();
   static void *new_dequelEfloatgR(void *p = 0);
   static void *newArray_dequelEfloatgR(Long_t size, void *p);
   static void delete_dequelEfloatgR(void *p);
   static void deleteArray_dequelEfloatgR(void *p);
   static void destruct_dequelEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<float>*)
   {
      deque<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<float>),0);
      static ::ROOT::TGenericClassInfo 
         instance("deque<float>", -2, "prec_stl/deque", 42,
                  typeid(deque<float>), DefineBehavior(ptr, ptr),
                  0, &dequelEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(deque<float>) );
      instance.SetNew(&new_dequelEfloatgR);
      instance.SetNewArray(&newArray_dequelEfloatgR);
      instance.SetDelete(&delete_dequelEfloatgR);
      instance.SetDeleteArray(&deleteArray_dequelEfloatgR);
      instance.SetDestructor(&destruct_dequelEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const deque<float>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void dequelEfloatgR_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const deque<float>*)0x0)->GetClass();
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelEfloatgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<float> : new deque<float>;
   }
   static void *newArray_dequelEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<float>[nElements] : new deque<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelEfloatgR(void *p) {
      delete ((deque<float>*)p);
   }
   static void deleteArray_dequelEfloatgR(void *p) {
      delete [] ((deque<float>*)p);
   }
   static void destruct_dequelEfloatgR(void *p) {
      typedef deque<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<float>

namespace ROOT {
   void dequelEintgR_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void dequelEintgR_Dictionary();
   static void *new_dequelEintgR(void *p = 0);
   static void *newArray_dequelEintgR(Long_t size, void *p);
   static void delete_dequelEintgR(void *p);
   static void deleteArray_dequelEintgR(void *p);
   static void destruct_dequelEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<int>*)
   {
      deque<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<int>),0);
      static ::ROOT::TGenericClassInfo 
         instance("deque<int>", -2, "prec_stl/deque", 42,
                  typeid(deque<int>), DefineBehavior(ptr, ptr),
                  0, &dequelEintgR_Dictionary, isa_proxy, 0,
                  sizeof(deque<int>) );
      instance.SetNew(&new_dequelEintgR);
      instance.SetNewArray(&newArray_dequelEintgR);
      instance.SetDelete(&delete_dequelEintgR);
      instance.SetDeleteArray(&deleteArray_dequelEintgR);
      instance.SetDestructor(&destruct_dequelEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const deque<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void dequelEintgR_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const deque<int>*)0x0)->GetClass();
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelEintgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<int> : new deque<int>;
   }
   static void *newArray_dequelEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<int>[nElements] : new deque<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelEintgR(void *p) {
      delete ((deque<int>*)p);
   }
   static void deleteArray_dequelEintgR(void *p) {
      delete [] ((deque<int>*)p);
   }
   static void destruct_dequelEintgR(void *p) {
      typedef deque<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<int>

namespace ROOT {
   void dequelElonggR_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void dequelElonggR_Dictionary();
   static void *new_dequelElonggR(void *p = 0);
   static void *newArray_dequelElonggR(Long_t size, void *p);
   static void delete_dequelElonggR(void *p);
   static void deleteArray_dequelElonggR(void *p);
   static void destruct_dequelElonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<long>*)
   {
      deque<long> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<long>),0);
      static ::ROOT::TGenericClassInfo 
         instance("deque<long>", -2, "prec_stl/deque", 42,
                  typeid(deque<long>), DefineBehavior(ptr, ptr),
                  0, &dequelElonggR_Dictionary, isa_proxy, 0,
                  sizeof(deque<long>) );
      instance.SetNew(&new_dequelElonggR);
      instance.SetNewArray(&newArray_dequelElonggR);
      instance.SetDelete(&delete_dequelElonggR);
      instance.SetDeleteArray(&deleteArray_dequelElonggR);
      instance.SetDestructor(&destruct_dequelElonggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<long> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const deque<long>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void dequelElonggR_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const deque<long>*)0x0)->GetClass();
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelElonggR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<long> : new deque<long>;
   }
   static void *newArray_dequelElonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<long>[nElements] : new deque<long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelElonggR(void *p) {
      delete ((deque<long>*)p);
   }
   static void deleteArray_dequelElonggR(void *p) {
      delete [] ((deque<long>*)p);
   }
   static void destruct_dequelElonggR(void *p) {
      typedef deque<long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<long>

namespace ROOT {
   void dequelEvoidmUgR_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void dequelEvoidmUgR_Dictionary();
   static void *new_dequelEvoidmUgR(void *p = 0);
   static void *newArray_dequelEvoidmUgR(Long_t size, void *p);
   static void delete_dequelEvoidmUgR(void *p);
   static void deleteArray_dequelEvoidmUgR(void *p);
   static void destruct_dequelEvoidmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<void*>*)
   {
      deque<void*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<void*>),0);
      static ::ROOT::TGenericClassInfo 
         instance("deque<void*>", -2, "prec_stl/deque", 42,
                  typeid(deque<void*>), DefineBehavior(ptr, ptr),
                  0, &dequelEvoidmUgR_Dictionary, isa_proxy, 0,
                  sizeof(deque<void*>) );
      instance.SetNew(&new_dequelEvoidmUgR);
      instance.SetNewArray(&newArray_dequelEvoidmUgR);
      instance.SetDelete(&delete_dequelEvoidmUgR);
      instance.SetDeleteArray(&deleteArray_dequelEvoidmUgR);
      instance.SetDestructor(&destruct_dequelEvoidmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<void*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const deque<void*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void dequelEvoidmUgR_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const deque<void*>*)0x0)->GetClass();
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelEvoidmUgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<void*> : new deque<void*>;
   }
   static void *newArray_dequelEvoidmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) deque<void*>[nElements] : new deque<void*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelEvoidmUgR(void *p) {
      delete ((deque<void*>*)p);
   }
   static void deleteArray_dequelEvoidmUgR(void *p) {
      delete [] ((deque<void*>*)p);
   }
   static void destruct_dequelEvoidmUgR(void *p) {
      typedef deque<void*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<void*>

/********************************************************
* cint/cint/lib/dll_stl/rootcint_deque.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

extern "C" void G__cpp_reset_tagtablerootcint_deque();

extern "C" void G__set_cpp_environmentrootcint_deque() {
  G__add_compiledheader("TObject.h");
  G__add_compiledheader("TMemberInspector.h");
  G__add_compiledheader("deque");
  G__cpp_reset_tagtablerootcint_deque();
}
#include <new>
extern "C" int G__cpp_dllrevrootcint_deque() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncrootcint_deque {
 public:
  G__Sizep2memfuncrootcint_deque(): p(&G__Sizep2memfuncrootcint_deque::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncrootcint_deque::*p)();
};

size_t G__get_sizep2memfuncrootcint_deque()
{
  G__Sizep2memfuncrootcint_deque a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritancerootcint_deque() {

   /* Setting up class inheritance */
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetablerootcint_deque() {

   /* Setting up typedef entry */
   G__search_typename2("vector<TSchemaHelper>",117,G__get_linked_tagnum(&G__rootcint_dequeLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__rootcint_dequeLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__rootcint_dequeLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__rootcint_dequeLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__rootcint_dequeLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__rootcint_dequeLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */
extern "C" void G__cpp_setup_memvarrootcint_deque() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/

/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncrootcint_deque() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalrootcint_deque() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcrootcint_deque() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__rootcint_dequeLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR = { "vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >" , 99 , -1 };
G__linked_taginfo G__rootcint_dequeLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtablerootcint_deque() {
  G__rootcint_dequeLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR.tagnum = -1 ;
  G__rootcint_dequeLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtablerootcint_deque() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__rootcint_dequeLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR);
   G__get_linked_tagnum_fwd(&G__rootcint_dequeLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR);
}
extern "C" void G__cpp_setuprootcint_deque(void) {
  G__check_setup_version(30051515,"G__cpp_setuprootcint_deque()");
  G__set_cpp_environmentrootcint_deque();
  G__cpp_setup_tagtablerootcint_deque();

  G__cpp_setup_inheritancerootcint_deque();

  G__cpp_setup_typetablerootcint_deque();

  G__cpp_setup_memvarrootcint_deque();

  G__cpp_setup_memfuncrootcint_deque();
  G__cpp_setup_globalrootcint_deque();
  G__cpp_setup_funcrootcint_deque();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncrootcint_deque();
  return;
}
class G__cpp_setup_initrootcint_deque {
  public:
    G__cpp_setup_initrootcint_deque() { G__add_setup_func("rootcint_deque",(G__incsetup)(&G__cpp_setuprootcint_deque)); G__call_setup_funcs(); }
   ~G__cpp_setup_initrootcint_deque() { G__remove_setup_func("rootcint_deque"); }
};
G__cpp_setup_initrootcint_deque G__cpp_setup_initializerrootcint_deque;
