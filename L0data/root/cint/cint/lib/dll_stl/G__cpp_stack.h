/********************************************************************
* cint/cint/lib/dll_stl/G__cpp_stack.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error cint/cint/lib/dll_stl/G__cpp_stack.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
extern "C" {
extern void G__cpp_setup_tagtable();
extern void G__cpp_setup_inheritance();
extern void G__cpp_setup_typetable();
extern void G__cpp_setup_memvar();
extern void G__cpp_setup_global();
extern void G__cpp_setup_memfunc();
extern void G__cpp_setup_func();
extern void G__set_cpp_environment();
}


#include "cint/cint/lib/dll_stl/stk.h"
#include "cint/cint/inc/cintdictversion.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__LN_string;
extern G__linked_taginfo G__LN_allocatorlEintgR;
extern G__linked_taginfo G__LN_dequelEintcOallocatorlEintgRsPgR;
extern G__linked_taginfo G__LN_dequelEintcOallocatorlEintgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_dequelEintcOallocatorlEintgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_stacklEintcOdequelEintcOallocatorlEintgRsPgRsPgR;
extern G__linked_taginfo G__LN_allocatorlElonggR;
extern G__linked_taginfo G__LN_dequelElongcOallocatorlElonggRsPgR;
extern G__linked_taginfo G__LN_dequelElongcOallocatorlElonggRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_dequelElongcOallocatorlElonggRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_stacklElongcOdequelElongcOallocatorlElonggRsPgRsPgR;
extern G__linked_taginfo G__LN_allocatorlEdoublegR;
extern G__linked_taginfo G__LN_dequelEdoublecOallocatorlEdoublegRsPgR;
extern G__linked_taginfo G__LN_dequelEdoublecOallocatorlEdoublegRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_dequelEdoublecOallocatorlEdoublegRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_stacklEdoublecOdequelEdoublecOallocatorlEdoublegRsPgRsPgR;
extern G__linked_taginfo G__LN_allocatorlEvoidmUgR;
extern G__linked_taginfo G__LN_dequelEvoidmUcOallocatorlEvoidmUgRsPgR;
extern G__linked_taginfo G__LN_dequelEvoidmUcOallocatorlEvoidmUgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_dequelEvoidmUcOallocatorlEvoidmUgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_stacklEvoidmUcOdequelEvoidmUcOallocatorlEvoidmUgRsPgRsPgR;
extern G__linked_taginfo G__LN_allocatorlEcharmUgR;
extern G__linked_taginfo G__LN_dequelEcharmUcOallocatorlEcharmUgRsPgR;
extern G__linked_taginfo G__LN_dequelEcharmUcOallocatorlEcharmUgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_dequelEcharmUcOallocatorlEcharmUgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_stacklEcharmUcOdequelEcharmUcOallocatorlEcharmUgRsPgRsPgR;
extern G__linked_taginfo G__LN_allocatorlEstringgR;
extern G__linked_taginfo G__LN_dequelEstringcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__LN_dequelEstringcOallocatorlEstringgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_dequelEstringcOallocatorlEstringgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_stacklEstringcOdequelEstringcOallocatorlEstringgRsPgRsPgR;

/* STUB derived class for protected member access */
typedef allocator<int> G__allocatorlEintgR;
typedef deque<int,allocator<int> > G__dequelEintcOallocatorlEintgRsPgR;
typedef deque<int,allocator<int> >::iterator G__dequelEintcOallocatorlEintgRsPgRcLcLiterator;
typedef deque<int,allocator<int> >::reverse_iterator G__dequelEintcOallocatorlEintgRsPgRcLcLreverse_iterator;
typedef stack<int,deque<int,allocator<int> > > G__stacklEintcOdequelEintcOallocatorlEintgRsPgRsPgR;
typedef allocator<long> G__allocatorlElonggR;
typedef deque<long,allocator<long> > G__dequelElongcOallocatorlElonggRsPgR;
typedef deque<long,allocator<long> >::iterator G__dequelElongcOallocatorlElonggRsPgRcLcLiterator;
typedef deque<long,allocator<long> >::reverse_iterator G__dequelElongcOallocatorlElonggRsPgRcLcLreverse_iterator;
typedef stack<long,deque<long,allocator<long> > > G__stacklElongcOdequelElongcOallocatorlElonggRsPgRsPgR;
typedef allocator<double> G__allocatorlEdoublegR;
typedef deque<double,allocator<double> > G__dequelEdoublecOallocatorlEdoublegRsPgR;
typedef deque<double,allocator<double> >::iterator G__dequelEdoublecOallocatorlEdoublegRsPgRcLcLiterator;
typedef deque<double,allocator<double> >::reverse_iterator G__dequelEdoublecOallocatorlEdoublegRsPgRcLcLreverse_iterator;
typedef stack<double,deque<double,allocator<double> > > G__stacklEdoublecOdequelEdoublecOallocatorlEdoublegRsPgRsPgR;
typedef allocator<void*> G__allocatorlEvoidmUgR;
typedef deque<void*,allocator<void*> > G__dequelEvoidmUcOallocatorlEvoidmUgRsPgR;
typedef deque<void*,allocator<void*> >::iterator G__dequelEvoidmUcOallocatorlEvoidmUgRsPgRcLcLiterator;
typedef deque<void*,allocator<void*> >::reverse_iterator G__dequelEvoidmUcOallocatorlEvoidmUgRsPgRcLcLreverse_iterator;
typedef stack<void*,deque<void*,allocator<void*> > > G__stacklEvoidmUcOdequelEvoidmUcOallocatorlEvoidmUgRsPgRsPgR;
typedef allocator<char*> G__allocatorlEcharmUgR;
typedef deque<char*,allocator<char*> > G__dequelEcharmUcOallocatorlEcharmUgRsPgR;
typedef deque<char*,allocator<char*> >::iterator G__dequelEcharmUcOallocatorlEcharmUgRsPgRcLcLiterator;
typedef deque<char*,allocator<char*> >::reverse_iterator G__dequelEcharmUcOallocatorlEcharmUgRsPgRcLcLreverse_iterator;
typedef stack<char*,deque<char*,allocator<char*> > > G__stacklEcharmUcOdequelEcharmUcOallocatorlEcharmUgRsPgRsPgR;
typedef allocator<string> G__allocatorlEstringgR;
typedef deque<string,allocator<string> > G__dequelEstringcOallocatorlEstringgRsPgR;
typedef deque<string,allocator<string> >::iterator G__dequelEstringcOallocatorlEstringgRsPgRcLcLiterator;
typedef deque<string,allocator<string> >::reverse_iterator G__dequelEstringcOallocatorlEstringgRsPgRcLcLreverse_iterator;
typedef stack<string,deque<string,allocator<string> > > G__stacklEstringcOdequelEstringcOallocatorlEstringgRsPgRsPgR;
