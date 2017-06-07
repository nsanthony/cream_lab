// @(#)root/io:$Id: TMakeProject.cxx 29360 2009-07-06 21:10:23Z pcanal $
// Author: Rene Brun   12/10/2000

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMakeProject                                                         //
//                                                                      //
// Helper class implementing the TFile::MakeProject.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include "TMakeProject.h"
#include "TClass.h"
#include "TClassEdit.h"
#include "TROOT.h"
#include "TMD5.h"
#include "TStreamerInfo.h"
#include "TStreamerElement.h"

//______________________________________________________________________________
void TMakeProject::AddUniqueStatement(FILE *fp, const char *statement, char *inclist)
{
   // Add an include statement, if it has not already been added.

   if (!strstr(inclist, statement)) {
      strcat(inclist, statement);
      fprintf(fp, "%s", statement);
   }
}

//______________________________________________________________________________
void TMakeProject::AddInclude(FILE *fp, const char *header, Bool_t system, char *inclist)
{
   // Add an include statement, if it has not already been added.

   TString what;
   if (system) {
      what.Form("#include <%s>\n", header);
   } else {
      what.Form("#include \"%s\"\n", header);
   }
   AddUniqueStatement(fp, what.Data(), inclist);
}

//______________________________________________________________________________
void TMakeProject::ChopFileName(TString &name, Int_t limit)
{
   // Chop the name by replacing the ending (before a potential extension) with
   // a md5 summary of the name.
   
   if (name.Length() >= limit) {
      bool has_extension = (strcmp(name.Data() + name.Length() - 2, ".h") == 0);
      if (has_extension) {
         name.Remove(name.Length()-2);
      }
      TMD5 md;
      md.Update((const UChar_t*)name.Data(),name.Length());
      md.Final();
      name.Remove( limit - 32 - 5); // Chop the part longer than 255 and keep space for the md5 and leave space for an extension
      name.Append( md.AsString() );
      if (has_extension) {
         name.Append( ".h" );
      }
   }

}

//______________________________________________________________________________
TString TMakeProject::GetHeaderName(const char *name, Bool_t includeNested)
{
   //Return the header name containing the description of name
   TString result;
   Int_t len = strlen(name);
   Int_t nest = 0;
   for (Int_t i = 0; i < len; ++i) {
      switch (name[i]) {
         case '<':
            ++nest;
            result.Append('_');
            break;
         case '>':
            --nest;
            result.Append('_');
            break;
         case ':':
            if (nest == 0 && name[i+1] == ':') {
               TString nsname(name, i);
               TClass *cl = gROOT->GetClass(nsname);
               if (!includeNested && cl && (cl->Size() != 0 || (cl->Size()==0 && cl->GetClassInfo()==0 /*empty 'base' class on file*/))) {
                  // The requested class is actually nested inside
                  // the class whose name we already 'copied' to
                  // result.  The declaration will be in the same
                  // header file as the outer class.
                  if (strcmp(name + strlen(name) - 2, ".h") == 0) {
                     result.Append(".h");
                  }
                  ChopFileName(result,255);
                  return result;
               }
            }
            result.Append('_');
            break;
         case ',':
         case '*':
         case '[':
         case ']':
         case ' ':
         case '(':
         case ')':
            result.Append('_');
            break;
         case '/':
         case '\\':
         default:
            result.Append(name[i]);
      }
   }
   ChopFileName(result,255);
   return result;
}

//______________________________________________________________________________
UInt_t TMakeProject::GenerateClassPrefix(FILE *fp, const char *clname, Bool_t top, TString &protoname,
      UInt_t *numberOfClasses, Int_t implementEmptyClass, Bool_t needGenericTemplate)
{
   // Write the start of the class (forward) declaration.
   // if 'implementEmptyClass' is 3 then never add a #pragma

   // First open the namespace (if any)
   Int_t numberOfNamespaces = 0;
   const char *fullname = clname;

   Bool_t istemplate = kFALSE;
   if (strchr(clname, ':')) {
      // We might have a namespace in front of the classname.
      Int_t len = strlen(clname);
      const char *name = clname;
      UInt_t nest = 0;
      for (Int_t cur = 0; cur < len; ++cur) {
         switch (clname[cur]) {
            case '<':
               ++nest;
               istemplate = kTRUE;
               break;
            case '>':
               --nest;
               break;
            case ':': {
                  if (nest == 0 && clname[cur+1] == ':') {
                     // We have a scope
                     TString nsname(clname, cur);
                     TClass *cl = gROOT->GetClass(nsname);
                     if (top) {
                        if (cl == 0 || (cl && cl->Size() == 0)) {
                           TString last(name, cur - (name - clname));
                           if ((numberOfClasses == 0 || *numberOfClasses == 0) && strchr(last.Data(), '<') == 0) {
                              fprintf(fp, "namespace %s {\n", last.Data());
                              ++numberOfNamespaces;
                           } else {
                              TString headername(GetHeaderName(last));
                              fprintf(fp, "#ifndef %s_h\n", headername.Data());
                              fprintf(fp, "#define %s_h\n", headername.Data());
                              GenerateClassPrefix(fp, last.Data(), top, protoname, 0);
                              fprintf(fp, "{\n");
                              fprintf(fp, "public:\n");
                              if (numberOfClasses) ++(*numberOfClasses);
                              istemplate = kFALSE;
                           }
                           name = clname + cur + 2;
                        }
                     } else {
                        istemplate = kFALSE;
                        name = clname + cur + 2;
                     }
                  }
                  break;
               }
         }
      }
      clname = name;
   } else {
      istemplate = strstr(clname, "<") != 0;
   }

   protoname = clname;

   if (implementEmptyClass==1) {
      TString headername(GetHeaderName(fullname));
      fprintf(fp, "#ifndef %s_h\n", headername.Data());
      fprintf(fp, "#define %s_h\n", headername.Data());
   }
   if (istemplate) {
      std::vector<const char*> argtype;

      Ssiz_t pos = protoname.First('<');
      UInt_t nparam = 1;
      if (pos != kNPOS) {
         if (isdigit(protoname[pos+1])) {
            argtype.push_back("int");
         } else {
            argtype.push_back("typename");
         }
         UInt_t nest = 0;
         for (Ssiz_t i = pos; i < protoname.Length(); ++i) {
            switch (protoname[i]) {
               case '<':
                  ++nest;
                  break;
               case '>':
                  --nest;
                  break;
               case ',':
                  if (nest == 1) {
                     if (isdigit(protoname[i+1])) {
                        argtype.push_back("int");
                     } else {
                        argtype.push_back("typename");
                     }
                     ++nparam;
                  }
                  break;
            }
         }
         protoname.Remove(pos);
      }
      
      // Forward declaration of template.
      fprintf(fp, "template <");
      for (UInt_t p = 0; p < nparam; ++p) {
         if (p >= argtype.size() ) {
            fprintf(fp, "/* missing */ T%d", p);
         } else {
            fprintf(fp, "%s T%d", argtype[p], p);
         }
         if (p != (nparam - 1)) fprintf(fp, ", ");
      }
      if (needGenericTemplate) {
         fprintf(fp, "> class %s", protoname.Data());
      } else {
         fprintf(fp, "> class %s;\n", protoname.Data());
         fprintf(fp, "template <> ");
      }
   }

   if (implementEmptyClass) {
      if (istemplate) {
         if (!needGenericTemplate) {
            fprintf(fp, "class %s", clname);
         }
         fprintf(fp, " {\n");
         if (numberOfClasses) ++(*numberOfClasses);
         fprintf(fp, "public:\n");
         fprintf(fp, "operator int() { return 0; };\n");
      } else {
         fprintf(fp, "enum %s { kDefault_%s };\n", clname, clname);
         // The nesting space of this class may not be #pragma declared (and without
         // the dictionary is broken), so for now skip those
         if (implementEmptyClass==1) {
            if (strchr(fullname, ':') == 0) {
               // yes this is too aggressive, this needs to be fixed properly by moving the #pragma out of band.
               fprintf(fp, "%s", Form("#ifdef __MAKECINT__\n#pragma link C++ class %s+;\n#endif\n", fullname));
            }
            fprintf(fp, "#endif\n");
         }
      }
   } else {
      if (!(istemplate && needGenericTemplate)) {
          fprintf(fp, "class %s", clname);
      }
   }
   return numberOfNamespaces;
}

//______________________________________________________________________________
void TMakeProject::GenerateMissingStreamerInfo(TList *extrainfos, const char *clname, Bool_t iscope)
{
   // Generate an empty StreamerInfo for the given type (no recursion) if it is not
   // not known in the list of class.   If the type itself is a template,
   // we mark it with version 1 (a class) otherwise we mark it as version -3 (an enum).

   if (!TClassEdit::IsStdClass(clname) && !TClass::GetClass(clname) && gROOT->GetType(clname) == 0 && !extrainfos->FindObject(clname)) {

      // The class does not exist, let's create it
      TStreamerInfo *newinfo = new TStreamerInfo();
      newinfo->SetName(clname);
      if (clname[strlen(clname)-1]=='>') {
         newinfo->SetTitle("Generated by MakeProject as an empty class");
         newinfo->SetClassVersion(1);
      } else if (iscope) {
         newinfo->SetTitle("Generated by MakeProject as a class/namespace");
         newinfo->SetClassVersion(-4 /*namespace*/);
      } else {
         newinfo->SetTitle("Generated by MakeProject as an enum");
         newinfo->SetClassVersion(-3 /*enum*/);
      }
      extrainfos->Add(newinfo);
   }
}

//______________________________________________________________________________
void TMakeProject::GenerateMissingStreamerInfos(TList *extrainfos, const char *clname)
{
   // Generate an empty StreamerInfo for types that are used in templates parameters
   // but are not known in the list of class.   If the type itself is a template,
   // we mark it with version 1 (a class) otherwise we mark it as version -3 (an enum).
   
   UInt_t len = strlen(clname);
   UInt_t nest = 0;
   UInt_t last = 0;
   Bool_t istemplate = kFALSE; // mark whether the current right most entity is a class template.
   
   for (UInt_t i = 0; i < len; ++i) {
      switch (clname[i]) {
         case ':':
            if (nest == 0 && clname[i+1] == ':') {
               TString incName(clname, i);
               GenerateMissingStreamerInfo(extrainfos, incName.Data(), kTRUE);
               istemplate = kFALSE;
            }
            break;
         case '<':
            ++nest;
            if (nest == 1) last = i + 1;
            break;
         case '>':
            --nest; /* intentional fall throught to the next case */
         case ',':
            if ((clname[i] == ',' && nest == 1) || (clname[i] == '>' && nest == 0)) {
               TString incName(clname + last, i - last);
               incName = TClassEdit::ShortType(incName.Data(), 1);
               if (clname[i] == '>' && nest == 1) incName.Append(">");

               if (isdigit(incName[0])) {
                  // Not a class name, nothing to do.
               } else {
                  GenerateMissingStreamerInfos(extrainfos,incName.Data());
               }
               last = i + 1;
            }
      }
   }
   GenerateMissingStreamerInfo(extrainfos,TClassEdit::ShortType(clname, 1).c_str(),kFALSE);
}

//______________________________________________________________________________
void TMakeProject::GenerateMissingStreamerInfos(TList *extrainfos, TStreamerElement *element)
{
   // Generate an empty StreamerInfo for types that are used in templates parameters
   // but are not known in the list of class.   If the type itself is a template,
   // we mark it with version 1 (a class) otherwise we mark it as version -3 (an enum).
   
   if (element->IsBase()) {
      GenerateMissingStreamerInfos(extrainfos,element->GetClassPointer()->GetName());
   } else {
      GenerateMissingStreamerInfos(extrainfos,element->GetTypeName());
   }

}

//______________________________________________________________________________
UInt_t TMakeProject::GenerateForwardDeclaration(FILE *fp, const char *clname, char *inclist, Bool_t implementEmptyClass, Bool_t needGenericTemplate)
{
   // Insert a (complete) forward declaration for the class 'clname'

   UInt_t ninc = 0;

   if (strchr(clname, '<')) {
      ninc += GenerateIncludeForTemplate(fp, clname, inclist, kTRUE);
   }
   TString protoname;
   UInt_t numberOfClasses = 0;
   UInt_t numberOfNamespaces = GenerateClassPrefix(fp, clname, kTRUE, protoname, &numberOfClasses, implementEmptyClass, needGenericTemplate);

   fprintf(fp, ";\n");
   for (UInt_t i = 0;i < numberOfClasses;++i) {
      fprintf(fp, "}; // end of class.\n");
      fprintf(fp, "#endif\n");
   }
   for (UInt_t i = 0;i < numberOfNamespaces;++i) {
      fprintf(fp, "} // end of namespace.\n");
   }

   return ninc;
}

//______________________________________________________________________________
UInt_t TMakeProject::GenerateIncludeForTemplate(FILE *fp, const char *clname, char *inclist, Bool_t forward)
{
   // Add to the header file, the #include needed for the argument of
   // this template.

   UInt_t ninc = 0;
   UInt_t len = strlen(clname);
   UInt_t nest = 0;
   UInt_t last = 0;


   for (UInt_t i = 0; i < len; ++i) {
      switch (clname[i]) {
         case '<':
            ++nest;
            if (nest == 1) last = i + 1;
            break;
         case '>':
            --nest; /* intentional fall throught to the next case */
         case ',':
            if ((clname[i] == ',' && nest == 1) || (clname[i] == '>' && nest == 0)) {
               TString incName(clname + last, i - last);
               incName = TClassEdit::ShortType(incName.Data(), 1);
               if (clname[i] == '>' && nest == 1) incName.Append(">");
               Int_t stlType;
               if (isdigit(incName[0])) {
                  // Not a class name, nothing to do.
               } else if ((stlType = TClassEdit::IsSTLCont(incName))) {
                  const char *what = "";
                  switch (TMath::Abs(stlType))  {
                     case TClassEdit::kVector:
                        what = "vector";
                        break;
                     case TClassEdit::kList:
                        what = "list";
                        break;
                     case TClassEdit::kDeque:
                        what = "deque";
                        break;
                     case TClassEdit::kMap:
                        what = "map";
                        break;
                     case TClassEdit::kMultiMap:
                        what = "map";
                        break;
                     case TClassEdit::kSet:
                        what = "set";
                        break;
                     case TClassEdit::kMultiSet:
                        what = "set";
                        break;
                     case TClassEdit::kBitSet:
                        what = "bitset";
                        break;
                     default:
                        what = "undetermined_stl_container";
                        break;
                  }
                  AddInclude(fp, what, kTRUE, inclist);
                  fprintf(fp, "namespace std {} using namespace std;\n");
                  ninc += GenerateIncludeForTemplate(fp, incName, inclist, forward);
               } else if (strncmp(incName.Data(), "pair<", strlen("pair<")) == 0) {
                  AddInclude(fp, "utility", kTRUE, inclist);
                  ninc += GenerateIncludeForTemplate(fp, incName, inclist, forward);
               } else if (strncmp(incName.Data(), "auto_ptr<", strlen("auto_ptr<")) == 0) {
                  AddInclude(fp, "memory", kTRUE, inclist);
                  ninc += GenerateIncludeForTemplate(fp, incName, inclist, forward);
               } else if (TClassEdit::IsStdClass(incName)) {
                  // Do nothing.
               } else {
                  TClass *cl = gROOT->GetClass(incName);
                  if (!forward && cl) {
                     if (cl->GetClassInfo()) {
                        // We have the real dictionary for this class.

                        const char *include = cl->GetDeclFileName();
                        if (include && strlen(include) != 0) {

                           if (strncmp(include, "include/", 8) == 0) {
                              include += 8;
                           }
                           if (strncmp(include, "include\\", 9) == 0) {
                              include += 9;
                           }
                           TMakeProject::AddInclude(fp, include, kFALSE, inclist);
                        }
                        GenerateIncludeForTemplate(fp, incName, inclist, forward);
                     } else {
                        incName = GetHeaderName(incName);
                        incName.Append(".h");
                        AddInclude(fp, incName, kFALSE, inclist);
                     }
                  } else if (incName.Length() && incName[0] != ' ' && gROOT->GetType(incName) == 0) {
                     GenerateForwardDeclaration(fp, incName, inclist, !cl);
                  }
               }
               last = i + 1;
            }
      }
   }

   Int_t stlType = TClassEdit::IsSTLCont(clname);
   if (stlType) {
      std::vector<std::string> inside;
      int nestedLoc;
      TClassEdit::GetSplit(clname, inside, nestedLoc);
      Int_t stlkind =  TClassEdit::STLKind(inside[0].c_str());
      TClass *key = TClass::GetClass(inside[1].c_str());
      if (key) {
         std::string what;
         switch (stlkind)  {
            case TClassEdit::kMap:
            case TClassEdit::kMultiMap: {
                  what = "pair<";
                  what += UpdateAssociativeToVector( inside[1].c_str() );
                  what += ",";
                  what += UpdateAssociativeToVector( inside[2].c_str() );
                  what += " >";
                  AddUniqueStatement(fp, Form("#ifdef __MAKECINT__\n#pragma link C++ class %s+;\n#endif\n", what.c_str()), inclist);
                  break;
               }
         }
      }
   }

   return ninc;
}


//______________________________________________________________________________
TString TMakeProject::UpdateAssociativeToVector(const char *name)
{
   // If we have a map, multimap, set or multiset,
   // and the key is a class, we need to replace the
   // container by a vector since we don't have the
   // comparator function.
   // The 'name' is modified to return the change in the name,
   // if any.
   TString newname( name );
   
   if (strchr(name,'<')!=0) {
      std::vector<std::string> inside;
      int nestedLoc;
      TClassEdit::GetSplit( name, inside, nestedLoc );
      for(unsigned int i = 1; i<inside.size(); ++i) {
         inside[i] = UpdateAssociativeToVector( inside[i].c_str() );
      }
      Int_t stlkind =  TMath::Abs(TClassEdit::STLKind(inside[0].c_str()));
      if (stlkind!=0) {
         TClass *key = TClass::GetClass(inside[1].c_str());
      
         if (key) {
            std::string what;
            switch ( stlkind )  {
               case TClassEdit::kMap:
               case TClassEdit::kMultiMap: {
                  what = "pair<";
                  what += inside[1];
                  what += ",";
                  what += inside[2];
                  what += " >";
                  inside.clear();
                  inside.push_back("vector");
                  inside.push_back(what);
                  break;
               }
               case TClassEdit::kSet:
               case TClassEdit::kMultiSet:
                  inside[0] = "vector";
                  break;
            }
         }
      }
      newname = inside[0];
      newname.Append("<");
      newname.Append(inside[1]);
      for(unsigned int j=2; j<inside.size(); ++j) {
         newname.Append(",");
         newname.Append(inside[j]);
      }
      if (newname[newname.Length()-1]=='>') {
         newname.Append(" >");
      } else {
         newname.Append(">");
      }
   }
   return newname;
}
