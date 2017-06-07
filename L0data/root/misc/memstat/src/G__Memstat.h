
/* Includes added by #pragma extra_include */
#include "vector"
/********************************************************************
* misc/memstat/src/G__Memstat.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error misc/memstat/src/G__Memstat.h/C is only for compilation. Abort cint.
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
extern void G__cpp_setup_tagtableG__Memstat();
extern void G__cpp_setup_inheritanceG__Memstat();
extern void G__cpp_setup_typetableG__Memstat();
extern void G__cpp_setup_memvarG__Memstat();
extern void G__cpp_setup_globalG__Memstat();
extern void G__cpp_setup_memfuncG__Memstat();
extern void G__cpp_setup_funcG__Memstat();
extern void G__set_cpp_environmentG__Memstat();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "TMemStatHelpers.h"
#include "TMemStatDepend.h"
#include "TMemStat.h"
#include "TMemStatManager.h"
#include "TMemStatInfo.h"

#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__MemstatLN_TClass;
extern G__linked_taginfo G__G__MemstatLN_TBuffer;
extern G__linked_taginfo G__G__MemstatLN_TMemberInspector;
extern G__linked_taginfo G__G__MemstatLN_TObject;
extern G__linked_taginfo G__G__MemstatLN_string;
extern G__linked_taginfo G__G__MemstatLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__G__MemstatLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MemstatLN_TObjArray;
extern G__linked_taginfo G__G__MemstatLN_TString;
extern G__linked_taginfo G__G__MemstatLN_TObjString;
extern G__linked_taginfo G__G__MemstatLN_Memstat;
extern G__linked_taginfo G__G__MemstatLN_TMemStatDepend;
extern G__linked_taginfo G__G__MemstatLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__G__MemstatLN_TBits;
extern G__linked_taginfo G__G__MemstatLN_TTree;
extern G__linked_taginfo G__G__MemstatLN_TMemStatManager;
extern G__linked_taginfo G__G__MemstatLN_TGraph;
extern G__linked_taginfo G__G__MemstatLN_TMemStatCodeInfo;
extern G__linked_taginfo G__G__MemstatLN_TMemStatInfoStamp;
extern G__linked_taginfo G__G__MemstatLN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__G__MemstatLN_reverse_iteratorlEvectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MemstatLN_vectorlEintcOallocatorlEintgRsPgR;
extern G__linked_taginfo G__G__MemstatLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MemstatLN_auto_ptrlETFilegR;
extern G__linked_taginfo G__G__MemstatLN_TMemStat;
extern G__linked_taginfo G__G__MemstatLN_setlEstringcOlesslEstringgRcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__G__MemstatLN_TMemStatcLcLESelection;
extern G__linked_taginfo G__G__MemstatLN_TMemStatcLcLStatType;
extern G__linked_taginfo G__G__MemstatLN_TMemStatcLcLStampType;
extern G__linked_taginfo G__G__MemstatLN_TMemStatcLcLOperType;
extern G__linked_taginfo G__G__MemstatLN_TMemStatStackInfo;
extern G__linked_taginfo G__G__MemstatLN_TMemStatInfoStampcLcLEStampType;
extern G__linked_taginfo G__G__MemstatLN_TMemStatStackInfocLcLdA;
extern G__linked_taginfo G__G__MemstatLN_TMemStatManagercLcLTMemTable_t;
extern G__linked_taginfo G__G__MemstatLN_TMemStatManagercLcLTDeleteTable_t;
extern G__linked_taginfo G__G__MemstatLN_vectorlETMemStatCodeInfocOallocatorlETMemStatCodeInfogRsPgR;
extern G__linked_taginfo G__G__MemstatLN_reverse_iteratorlEvectorlETMemStatCodeInfocOallocatorlETMemStatCodeInfogRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MemstatLN_TMemStatManagercLcLEStatusBits;
extern G__linked_taginfo G__G__MemstatLN_TMemStatManagercLcLEDumpTo;
extern G__linked_taginfo G__G__MemstatLN_vectorlETMemStatStackInfocOallocatorlETMemStatStackInfogRsPgR;
extern G__linked_taginfo G__G__MemstatLN_reverse_iteratorlEvectorlETMemStatStackInfocOallocatorlETMemStatStackInfogRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MemstatLN_vectorlETMemStatInfoStampcOallocatorlETMemStatInfoStampgRsPgR;
extern G__linked_taginfo G__G__MemstatLN_reverse_iteratorlEvectorlETMemStatInfoStampcOallocatorlETMemStatInfoStampgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MemstatLN_vectorlETTimeStampcOallocatorlETTimeStampgRsPgR;
extern G__linked_taginfo G__G__MemstatLN_reverse_iteratorlEvectorlETTimeStampcOallocatorlETTimeStampgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MemstatLN_maplEconstsPvoidmUcOunsignedsPintcOlesslEconstsPvoidmUgRcOallocatorlEpairlEconstsPvoidmUsPconstcOunsignedsPintgRsPgRsPgR;

/* STUB derived class for protected member access */
