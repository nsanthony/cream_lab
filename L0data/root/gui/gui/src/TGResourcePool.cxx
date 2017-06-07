// @(#)root/gui:$Id: TGResourcePool.cxx 23115 2008-04-10 13:35:37Z rdm $
// Author: Fons Rademakers   19/5/2003

/*************************************************************************
 * Copyright (C) 1995-2003, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
/**************************************************************************

    This source is based on Xclass95, a Win95-looking GUI toolkit.
    Copyright (C) 1996, 1997 David Barth, Ricky Ralston, Hector Peraza.

    Xclass95 is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

**************************************************************************/


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGResourcePool                                                       //
//                                                                      //
// This class implements a pool for the default GUI resource set,       //
// like GC's, colors, fonts, etc..                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "RConfigure.h"

#include "TGResourcePool.h"
#include "TGClient.h"
#include "TGWindow.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TEnv.h"
#include "TVirtualX.h"
#include "TGGC.h"
#include "TGPicture.h"
#include "TGFont.h"
#include "TGMimeTypes.h"


const int gray_width  = 8;
const int gray_height = 8;
static unsigned char gray_bits[] = {
   0x55, 0xaa, 0x55, 0xaa,
   0x55, 0xaa, 0x55, 0xaa
};


ClassImp(TGResourcePool)

//______________________________________________________________________________
TGResourcePool::TGResourcePool(TGClient *client)
{
   // Create the global GUI resource pool manager.

   fClient = client;
   fDefaultColormap = gVirtualX->GetColormap();

   // Get GUI defaults from [system].rootrc
   char default_font[256];
   char menu_font[256];
   char menu_hi_font[256];
   char doc_fixed_font[256];
   char doc_prop_font[256];
   char icon_font[256];
   char status_font[256];
   char backcolor[256];
   char forecolor[256];
   char selbackcolor[256];
   char selforecolor[256];
   char docbackcolor[256];
   char docforecolor[256];
   char tipbackcolor[256];
   char tipforecolor[256];
   char framebgpixmap[2048], docbgpixmap[2048];
   char icon_path[2048], mime_file[256], line[2048];

   strcpy(default_font,   gEnv->GetValue("Gui.DefaultFont",  "-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1"));
   strcpy(menu_font,      gEnv->GetValue("Gui.MenuFont",     "-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1"));
   strcpy(menu_hi_font,   gEnv->GetValue("Gui.MenuHiFont",   "-*-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1"));
   strcpy(doc_fixed_font, gEnv->GetValue("Gui.DocFixedFont", "-*-courier-medium-r-*-*-12-*-*-*-*-*-iso8859-1"));
   strcpy(doc_prop_font,  gEnv->GetValue("Gui.DocPropFont",  "-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1"));
   strcpy(icon_font,      gEnv->GetValue("Gui.IconFont",     "-*-helvetica-medium-r-*-*-10-*-*-*-*-*-iso8859-1"));
   strcpy(status_font,    gEnv->GetValue("Gui.StatusFont",   "-*-helvetica-medium-r-*-*-10-*-*-*-*-*-iso8859-1"));
   strcpy(backcolor,      gEnv->GetValue("Gui.BackgroundColor",         "#c0c0c0"));
   strcpy(forecolor,      gEnv->GetValue("Gui.ForegroundColor",         "black"));
   strcpy(selbackcolor,   gEnv->GetValue("Gui.SelectBackgroundColor",   "#000080"));
   strcpy(selforecolor,   gEnv->GetValue("Gui.SelectForegroundColor",   "white"));
   strcpy(docbackcolor,   gEnv->GetValue("Gui.DocumentBackgroundColor", "white"));
   strcpy(docforecolor,   gEnv->GetValue("Gui.DocumentForegroundColor", "black"));
   strcpy(tipbackcolor,   gEnv->GetValue("Gui.TooltipBackgroundColor",  "LightYellow"));
   strcpy(tipforecolor,   gEnv->GetValue("Gui.TooltipForegroundColor",  "black"));
   strcpy(framebgpixmap,  gEnv->GetValue("Gui.FrameBackgroundPixmap", ""));
   strcpy(docbgpixmap,    gEnv->GetValue("Gui.DocumentBackgroundPixmap", ""));

#ifndef R__WIN32
#ifndef R__VMS
# ifdef ROOTICONPATH
   sprintf(icon_path, "%s/icons:%s:.:",
           gSystem->HomeDirectory(),
           ROOTICONPATH);
#  ifdef EXTRAICONPATH
   strcat(icon_path, gEnv->GetValue("Gui.IconPath", EXTRAICONPATH));
#  else
   strcat(icon_path, gEnv->GetValue("Gui.IconPath", ""));
#  endif
# else
   sprintf(icon_path, "%s/icons:%s/icons:.:", gSystem->HomeDirectory(),
                                              gSystem->Getenv("ROOTSYS"));
   strcat(icon_path, gEnv->GetValue("Gui.IconPath", ""));
# endif
   sprintf(line, "%s/.root.mimes", gSystem->HomeDirectory());
#else
   sprintf(line,"[%s.ICONS]",gSystem->Getenv("ROOTSYS"));
   strcpy(icon_path, gEnv->GetValue("Gui.IconPath",line));
   sprintf(line,"%sroot.mimes",gSystem->HomeDirectory());
#endif

   strcpy(mime_file, gEnv->GetValue("Gui.MimeTypeFile", line));
   char *mf = gSystem->ExpandPathName(mime_file);
   if (mf) {
      strcpy(mime_file, mf);
      delete [] mf;
   }
   if (gSystem->AccessPathName(mime_file, kReadPermission))
#ifdef R__VMS
      sprintf(mime_file,"[%s.ETC]root.mimes",gSystem->Getenv("ROOTSYS"));
#else
# ifdef ROOTETCDIR
      sprintf(mime_file, "%s/root.mimes", ROOTETCDIR);
# else
      sprintf(mime_file, "%s/etc/root.mimes", gSystem->Getenv("ROOTSYS"));
# endif
#endif
#else // R__WIN32
   sprintf(icon_path, "%s\\icons:.:\\", gSystem->Getenv("ROOTSYS"));
   strcat(icon_path, gEnv->GetValue("Gui.IconPath", ""));
   sprintf(line, "%s\\root.mimes", gSystem->HomeDirectory());
   strcpy(mime_file, gEnv->GetValue("Gui.MimeTypeFile", line));
   if (gSystem->AccessPathName(mime_file, kReadPermission))
      sprintf(mime_file, "%s\\etc\\root.mimes", gSystem->Getenv("ROOTSYS"));
#endif

   // Setup colors...
   fClient->GetColorByName("white", fWhite);  // white and black always exist
   fClient->GetColorByName("black", fBlack);
   if (!fClient->GetColorByName(backcolor, fBackColor))
      fBackColor = fWhite;
   if (!fClient->GetColorByName(forecolor, fForeColor))
      fForeColor = fBlack;
   fHilite = fClient->GetHilite(fBackColor);
   fShadow = fClient->GetShadow(fBackColor);
   if (!fClient->GetColorByName(selbackcolor, fSelBackColor))
      fSelBackColor = fBlack;
   if (!fClient->GetColorByName(selforecolor, fSelForeColor))
      fSelForeColor = fWhite;
   if (!fClient->GetColorByName(docbackcolor, fDocBackColor))
      fDocBackColor = fWhite;
   if (!fClient->GetColorByName(docforecolor, fDocForeColor))
      fDocForeColor = fBlack;
   if (!fClient->GetColorByName(tipbackcolor, fTipBackColor))
      fTipBackColor = fWhite;
   if (!fClient->GetColorByName(tipforecolor, fTipForeColor))
      fTipForeColor = fBlack;

   // Setup checkered pix/bit-maps...
   fCheckered = gVirtualX->CreatePixmap(fClient->GetDefaultRoot()->GetId(),
                                        (const char *)gray_bits,
                                        gray_width, gray_height,
                                        fBackColor, fWhite /*fHilite*/,
                                        gVirtualX->GetDepth());
   fCheckeredBitmap = gVirtualX->CreatePixmap(fClient->GetDefaultRoot()->GetId(),
                                              (const char *)gray_bits,
                                              gray_width, gray_height,
                                              1, 0, 1);

   // Create picture pool, GC pool, font pool, mime type list, etc.

   // Create picture pool and pre-load some pictures...
   fPicturePool = new TGPicturePool(fClient, icon_path);

   fDefaultBackPicture    = 0;
   fDefaultDocBackPicture = 0;
   if (strlen(framebgpixmap))
      fDefaultBackPicture = fPicturePool->GetPicture(framebgpixmap);
   if (strlen(docbgpixmap))
      fDefaultDocBackPicture = fPicturePool->GetPicture(docbgpixmap);

   // Create font pool and pre-load some fonts...
   fFontPool = new TGFontPool(fClient);
   fDefaultFont  = fFontPool->GetFont(default_font);
   fMenuFont     = fFontPool->GetFont(menu_font);
   fMenuHiFont   = fFontPool->GetFont(menu_hi_font);
   fDocFixedFont = fFontPool->GetFont(doc_fixed_font);
   fDocPropFont  = fFontPool->GetFont(doc_prop_font);
   fIconFont     = fFontPool->GetFont(icon_font);
   fStatusFont   = fFontPool->GetFont(status_font);

   // Create GC pool and pre-load some GCs...
   fGCPool = new TGGCPool(fClient);

   GCValues_t gval;
   gval.fMask = kGCForeground | kGCBackground | kGCFont |
                kGCFillStyle  | kGCGraphicsExposures;
   gval.fGraphicsExposures = kFALSE;
   gval.fFillStyle  = kFillSolid;
   gval.fFont       = fDefaultFont->GetFontHandle();
   gval.fBackground = fBackColor;
   gval.fForeground = fBlack;
   fBlackGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fForeground = fWhite;
   fWhiteGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fForeground = fHilite;
   fHiliteGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fForeground = fShadow;
   fShadowGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fForeground = fBackColor;
   fBckgndGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fForeground = fForeColor;
   fFrameGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fForeground = fSelBackColor;
   fSelbgndGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fForeground = fSelForeColor;
   gval.fBackground = fSelBackColor;
   fSelGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fFont       = fDocPropFont->GetFontHandle();
   gval.fForeground = fDocForeColor;
   gval.fBackground = fDocBackColor;
   fDocGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fForeground = fDocBackColor;
   fDocbgndGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fFont       = fStatusFont->GetFontHandle();
   gval.fForeground = fTipForeColor;
   gval.fBackground = fTipBackColor;
   fTipGC = fGCPool->GetGC(&gval, kTRUE);

   gval.fMask = kGCForeground | kGCBackground | kGCFillStyle |
                kGCLineWidth  | kGCSubwindowMode |
                kGCGraphicsExposures | kGCStipple;
   gval.fForeground = fBlack;
   gval.fBackground = fSelBackColor;
   gval.fLineWidth  = 0;
   gval.fFillStyle  = kFillStippled;
   gval.fSubwindowMode = kIncludeInferiors;
   gval.fGraphicsExposures = kFALSE;
   gval.fStipple = fCheckeredBitmap;
   fFocusGC = fGCPool->GetGC(&gval, kTRUE);

   // Setup cursors...
   fDefaultCursor = kNone;
   fGrabCursor    = gVirtualX->CreateCursor(kArrowRight);
   fTextCursor    = gVirtualX->CreateCursor(kCaret);
   fWaitCursor    = gVirtualX->CreateCursor(kWatch);

   // Read in mime type...
   fMimeTypeList  = new TGMimeTypes(fClient, mime_file);

   // Clipboard handle...
#ifndef R__WIN32
   fClipboardAtom = gVirtualX->InternAtom("_ROOT_CLIPBOARD", kFALSE);
#else
   fClipboardAtom = gVirtualX->InternAtom("CLIPBOARD", kFALSE);
#endif
}

//______________________________________________________________________________
TGResourcePool::~TGResourcePool()
{
   // Cleanup the resource pool...

   delete fPicturePool;
   delete fFontPool;
   delete fGCPool;
   delete fMimeTypeList;

   gVirtualX->DeletePixmap(fCheckered);
   gVirtualX->DeletePixmap(fCheckeredBitmap);
}
