/********************************************************************
* graf2d/graf/src/G__Graf.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error graf2d/graf/src/G__Graf.h/C is only for compilation. Abort cint.
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
extern void G__cpp_setup_tagtableG__Graf();
extern void G__cpp_setup_inheritanceG__Graf();
extern void G__cpp_setup_typetableG__Graf();
extern void G__cpp_setup_memvarG__Graf();
extern void G__cpp_setup_globalG__Graf();
extern void G__cpp_setup_memfuncG__Graf();
extern void G__cpp_setup_funcG__Graf();
extern void G__set_cpp_environmentG__Graf();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "TArc.h"
#include "TArrow.h"
#include "TAttImage.h"
#include "TBox.h"
#include "TCrown.h"
#include "TCurlyArc.h"
#include "TCurlyLine.h"
#include "TCutG.h"
#include "TDiamond.h"
#include "TEllipse.h"
#include "TFrame.h"
#include "TGaxis.h"
#include "TGraphPolargram.h"
#include "TGraphPolar.h"
#include "TGraphQQ.h"
#include "TImage.h"
#include "TImagePlugin.h"
#include "TLatex.h"
#include "TLegendEntry.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLink.h"
#include "TMarker.h"
#include "TPave.h"
#include "TPaveLabel.h"
#include "TPaveStats.h"
#include "TPavesText.h"
#include "TPaveText.h"
#include "TPie.h"
#include "TPieSlice.h"
#include "TPoints.h"
#include "TPolyLine.h"
#include "TText.h"
#include "TTF.h"
#include "TWbox.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__GrafLN_TClass;
extern G__linked_taginfo G__G__GrafLN_TBuffer;
extern G__linked_taginfo G__G__GrafLN_TMemberInspector;
extern G__linked_taginfo G__G__GrafLN_TObject;
extern G__linked_taginfo G__G__GrafLN_TNamed;
extern G__linked_taginfo G__G__GrafLN_basic_ostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__GrafLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__G__GrafLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__GrafLN_TList;
extern G__linked_taginfo G__G__GrafLN_TString;
extern G__linked_taginfo G__G__GrafLN_TAttLine;
extern G__linked_taginfo G__G__GrafLN_TAttFill;
extern G__linked_taginfo G__G__GrafLN_TEllipse;
extern G__linked_taginfo G__G__GrafLN_TEllipsecLcLdA;
extern G__linked_taginfo G__G__GrafLN_TArc;
extern G__linked_taginfo G__G__GrafLN_TLine;
extern G__linked_taginfo G__G__GrafLN_TLinecLcLdA;
extern G__linked_taginfo G__G__GrafLN_TArrow;
extern G__linked_taginfo G__G__GrafLN_TAttImage;
extern G__linked_taginfo G__G__GrafLN_TPaletteEditor;
extern G__linked_taginfo G__G__GrafLN_TImagePalette;
extern G__linked_taginfo G__G__GrafLN_TAttImagecLcLEImageQuality;
extern G__linked_taginfo G__G__GrafLN_TBox;
extern G__linked_taginfo G__G__GrafLN_TBoxcLcLdA;
extern G__linked_taginfo G__G__GrafLN_TCrown;
extern G__linked_taginfo G__G__GrafLN_TCollection;
extern G__linked_taginfo G__G__GrafLN_TPolyLine;
extern G__linked_taginfo G__G__GrafLN_TPolyLinecLcLdA;
extern G__linked_taginfo G__G__GrafLN_TCurlyLine;
extern G__linked_taginfo G__G__GrafLN_TCurlyLinecLcLdA;
extern G__linked_taginfo G__G__GrafLN_TCurlyArc;
extern G__linked_taginfo G__G__GrafLN_TAttMarker;
extern G__linked_taginfo G__G__GrafLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__G__GrafLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__G__GrafLN_TAxis;
extern G__linked_taginfo G__G__GrafLN_TH1;
extern G__linked_taginfo G__G__GrafLN_TF1;
extern G__linked_taginfo G__G__GrafLN_TGraph;
extern G__linked_taginfo G__G__GrafLN_TH2;
extern G__linked_taginfo G__G__GrafLN_TCutG;
extern G__linked_taginfo G__G__GrafLN_TPave;
extern G__linked_taginfo G__G__GrafLN_TPavecLcLdA;
extern G__linked_taginfo G__G__GrafLN_TAttText;
extern G__linked_taginfo G__G__GrafLN_TText;
extern G__linked_taginfo G__G__GrafLN_TTextcLcLdA;
extern G__linked_taginfo G__G__GrafLN_TPaveText;
extern G__linked_taginfo G__G__GrafLN_TDiamond;
extern G__linked_taginfo G__G__GrafLN_TWbox;
extern G__linked_taginfo G__G__GrafLN_TFrame;
extern G__linked_taginfo G__G__GrafLN_TGaxis;
extern G__linked_taginfo G__G__GrafLN_TGraphPolargram;
extern G__linked_taginfo G__G__GrafLN_TGraphPolargramcLcLdA;
extern G__linked_taginfo G__G__GrafLN_TGraphErrors;
extern G__linked_taginfo G__G__GrafLN_TGraphPolar;
extern G__linked_taginfo G__G__GrafLN_TGraphQQ;
extern G__linked_taginfo G__G__GrafLN_Segment_t;
extern G__linked_taginfo G__G__GrafLN_TVirtualPad;
extern G__linked_taginfo G__G__GrafLN_TArrayD;
extern G__linked_taginfo G__G__GrafLN_TArrayL;
extern G__linked_taginfo G__G__GrafLN_TPoint;
extern G__linked_taginfo G__G__GrafLN_TImage;
extern G__linked_taginfo G__G__GrafLN_TImagecLcLEImageFileTypes;
extern G__linked_taginfo G__G__GrafLN_TImagecLcLEText3DType;
extern G__linked_taginfo G__G__GrafLN_TImagecLcLECharType;
extern G__linked_taginfo G__G__GrafLN_TImagecLcLETileType;
extern G__linked_taginfo G__G__GrafLN_TImagecLcLECoordMode;
extern G__linked_taginfo G__G__GrafLN_TImagecLcLEColorChan;
extern G__linked_taginfo G__G__GrafLN_TImagePlugin;
extern G__linked_taginfo G__G__GrafLN_FormSize_t;
extern G__linked_taginfo G__G__GrafLN_TextSpec_t;
extern G__linked_taginfo G__G__GrafLN_TLatexFormSize;
extern G__linked_taginfo G__G__GrafLN_TLatex;
extern G__linked_taginfo G__G__GrafLN_TLatexcLcLdA;
extern G__linked_taginfo G__G__GrafLN_TLegendEntry;
extern G__linked_taginfo G__G__GrafLN_TLegend;
extern G__linked_taginfo G__G__GrafLN_TLink;
extern G__linked_taginfo G__G__GrafLN_TLinkcLcLdA;
extern G__linked_taginfo G__G__GrafLN_TMarker;
extern G__linked_taginfo G__G__GrafLN_TMarkercLcLdA;
extern G__linked_taginfo G__G__GrafLN_TPaveLabel;
extern G__linked_taginfo G__G__GrafLN_TPaveStats;
extern G__linked_taginfo G__G__GrafLN_TPavesText;
extern G__linked_taginfo G__G__GrafLN_TPieSlice;
extern G__linked_taginfo G__G__GrafLN_TPie;
extern G__linked_taginfo G__G__GrafLN_TPoints;
extern G__linked_taginfo G__G__GrafLN_FT_BBox;
extern G__linked_taginfo G__G__GrafLN_FT_Matrix;
extern G__linked_taginfo G__G__GrafLN_TTGlyph;
extern G__linked_taginfo G__G__GrafLN_TTF;
extern G__linked_taginfo G__G__GrafLN_TTFcLcLdA;

/* STUB derived class for protected member access */
