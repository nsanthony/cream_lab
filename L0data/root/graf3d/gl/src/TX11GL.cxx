// @(#)root/gx11:$Id: TX11GL.cxx 28478 2009-05-07 07:00:20Z couet $
// Author: Timur Pocheptsov (TX11GLManager) / Valeriy Onuchin (TX11GL)

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include <deque>
#include <map>
#include <stdlib.h>
#include <string.h>

#include "TVirtualViewer3D.h"
#include "TVirtualX.h"
#include "TGLViewer.h"
#include "TGLManip.h"
#include "TX11GL.h"
#include "TError.h"
#include "TROOT.h"

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TX11GLManager                                                        //
//                                                                      //
// The TX11GLManager is X11 implementation of TGLManager.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

struct TX11GLManager::TGLContext_t {
   //these are numbers returned by gVirtualX->AddWindow and gVirtualX->AddPixmap
   TGLContext_t() : fWindowIndex(-1), fPixmapIndex(-1), fX11Pixmap(0), fW(0),
                  fH(0), fX(0), fY(0), fGLXContext(0), fDirect(kFALSE),
                  fXImage(0), fNextFreeContext(0), fDirectGC(0), fPixmapGC(0)
   {
   }//FIXME
   Int_t                fWindowIndex;
   Int_t                fPixmapIndex;
   //X11 pixmap
   Pixmap               fX11Pixmap;
   //
   UInt_t               fW;
   UInt_t               fH;
   //
   Int_t                fX;
   Int_t                fY;
   //
   GLXContext           fGLXContext;
   Bool_t               fDirect;
   //GL buffer is read into XImage
   XImage              *fXImage;
   std::vector<UChar_t> fBUBuffer;//gl buffer is read from bottom to top.
   //
   TGLContext_t        *fNextFreeContext;
   GC                   fDirectGC;
   GC                   fPixmapGC;
};

namespace {

   typedef std::deque<TX11GLManager::TGLContext_t> DeviceTable_t;
   typedef DeviceTable_t::size_type SizeType_t;
   typedef std::map<Int_t, XVisualInfo *> WinTable_t;
   XSetWindowAttributes dummyAttr;

   //RAII class for Pixmap
   class TX11PixGuard {
   private:
      Display *fDpy;
      Pixmap   fPix;

   public:
      TX11PixGuard(Display *dpy, Pixmap pix) : fDpy(dpy), fPix(pix) {}
      ~TX11PixGuard(){if (fPix) XFreePixmap(fDpy, fPix);}
      void Stop(){fPix = 0;}

   private:
      TX11PixGuard(const TX11PixGuard &);
      TX11PixGuard &operator = (const TX11PixGuard &);
   };

   //RAII class for GLXContext
   class TGLXCtxGuard {
   private:
      Display    *fDpy;
      GLXContext  fCtx;

   public:
      TGLXCtxGuard(Display *dpy, GLXContext ctx) : fDpy(dpy), fCtx(ctx) {}
      ~TGLXCtxGuard(){if (fCtx) glXDestroyContext(fDpy, fCtx);}
      void Stop(){fCtx = 0;}

   private:
      TGLXCtxGuard(const TGLXCtxGuard &);
      TGLXCtxGuard &operator = (const TGLXCtxGuard &);
   };

   // RAII class for XImage.
   class TXImageGuard {
   private:
      XImage *fImage;

      TXImageGuard(const TXImageGuard &);
      TXImageGuard &operator = (const TXImageGuard &);

   public:
      explicit TXImageGuard(XImage *im) : fImage(im) {}
      ~TXImageGuard(){if (fImage) XDestroyImage(fImage);}
      void Stop(){fImage = 0;}
   };

}

// Attriblist for glXChooseVisual (double-buffered visual).
const Int_t dblBuff[] =
   {
      GLX_DOUBLEBUFFER,
      GLX_RGBA,
      GLX_DEPTH_SIZE, 16,
      GLX_RED_SIZE, 1,
      GLX_GREEN_SIZE, 1,
      GLX_BLUE_SIZE, 1,
      None
   };

// Attriblist for glxChooseVisual (single-buffered visual).
const Int_t *snglBuff = dblBuff + 1;

class TX11GLManager::TX11GLImpl {
public:
   TX11GLImpl();
   ~TX11GLImpl();

   WinTable_t      fGLWindows;
   DeviceTable_t   fGLContexts;
   Display        *fDpy;
   TGLContext_t     *fNextFreeContext;

private:
   TX11GLImpl(const TX11GLImpl &);
   TX11GLImpl &operator = (const TX11GLImpl &);
};


ClassImp(TX11GLManager)


//______________________________________________________________________________
TX11GLManager::TX11GLImpl::TX11GLImpl() : fDpy(0), fNextFreeContext(0)
{
   // Constructor.

   fDpy = reinterpret_cast<Display *>(gVirtualX->GetDisplay());
}


//______________________________________________________________________________
TX11GLManager::TX11GLImpl::~TX11GLImpl()
{
   // Destructor.
   // Destroys only GL contexts and XImages pixmaps and windows must be
   // closed through gVirtualX

   for (SizeType_t i = 0,  e = fGLContexts.size(); i < e; ++i) {
      TGLContext_t &ctx = fGLContexts[i];

      if (ctx.fGLXContext) {
         ::Warning("TX11GLManager::~TX11GLManager", "opengl device with index %d was not destroyed", i);
         glXDestroyContext(fDpy, ctx.fGLXContext);

         if (ctx.fPixmapIndex != -1) {
            gVirtualX->SelectWindow(ctx.fPixmapIndex);
            gVirtualX->ClosePixmap();
            if (ctx.fXImage)
               XDestroyImage(ctx.fXImage);
         }
      }
   }
}


//______________________________________________________________________________
TX11GLManager::TX11GLManager() : fPimpl(new TX11GLImpl)
{
   // Constructor.

   gGLManager = this;
   gROOT->GetListOfSpecials()->Add(this);
}


//______________________________________________________________________________
TX11GLManager::~TX11GLManager()
{
   // Destructor.

   delete fPimpl;
}


//______________________________________________________________________________
Int_t TX11GLManager::InitGLWindow(Window_t winID)
{
   // Try to find correct visual.

   XVisualInfo *visInfo = glXChooseVisual(
                                          fPimpl->fDpy, DefaultScreen(fPimpl->fDpy),
                                          const_cast<Int_t *>(dblBuff)
                                         );

   if (!visInfo) {
      Error("InitGLWindow", "No good visual found!\n");
      return -1;
   }

   Int_t  x = 0, y = 0;
   UInt_t w = 0, h = 0, b = 0, d = 0;
   Window root = 0;
   XGetGeometry(fPimpl->fDpy, winID, &root, &x, &y, &w, &h, &b, &d);

   XSetWindowAttributes attr(dummyAttr);
   attr.colormap = XCreateColormap(fPimpl->fDpy, root, visInfo->visual, AllocNone); // ???
   attr.event_mask = NoEventMask;
   attr.backing_store = Always;
   attr.bit_gravity = NorthWestGravity;

   ULong_t mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask | CWBackingStore | CWBitGravity;

   // Create window with specific visual.
   Window glWin = XCreateWindow(
                                fPimpl->fDpy, winID,
                                x, y, w, h,
                                0, visInfo->depth, InputOutput,
                                visInfo->visual, mask, &attr
                               );

   // Check results.
   XMapWindow(fPimpl->fDpy, glWin);

   // Register window for gVirtualX.
   Int_t x11Ind = gVirtualX->AddWindow(glWin,  w, h);

   // Register this window for GL manager.
   fPimpl->fGLWindows[x11Ind] = visInfo;

   return x11Ind;
}


//______________________________________________________________________________
Int_t TX11GLManager::CreateGLContext(Int_t winInd)
{
   // Context creation requires Display * and XVisualInfo
   // (was saved for such winInd).
   GLXContext glxCtx = glXCreateContext(fPimpl->fDpy, fPimpl->fGLWindows[winInd], None, True);

   if (!glxCtx) {
      Error("CreateContext", "glXCreateContext failed\n");
      return -1;
   }

   // Register new context now.
   if (TGLContext_t *ctx = fPimpl->fNextFreeContext) {
      Int_t ind = ctx->fWindowIndex;
      ctx->fWindowIndex = winInd;
      ctx->fGLXContext = glxCtx;
      fPimpl->fNextFreeContext = fPimpl->fNextFreeContext->fNextFreeContext;
      return ind;
   } else {
      TGLXCtxGuard glxCtxGuard(fPimpl->fDpy, glxCtx);
      TGLContext_t newDev;
      newDev.fWindowIndex = winInd;
      newDev.fGLXContext = glxCtx;

      fPimpl->fGLContexts.push_back(newDev);
      glxCtxGuard.Stop();

      return Int_t(fPimpl->fGLContexts.size()) - 1;
   }
}


//______________________________________________________________________________
Bool_t TX11GLManager::MakeCurrent(Int_t ctxInd)
{
   // Make GL context current.
   TGLContext_t &ctx = fPimpl->fGLContexts[ctxInd];
   return glXMakeCurrent(fPimpl->fDpy, gVirtualX->GetWindowID(ctx.fWindowIndex), ctx.fGLXContext);
}


//______________________________________________________________________________
void TX11GLManager::Flush(Int_t ctxInd)
{
   // Swaps buffers or copy pixmap.

   TGLContext_t &ctx = fPimpl->fGLContexts[ctxInd];
   Window winID = gVirtualX->GetWindowID(ctx.fWindowIndex);

   if (ctx.fPixmapIndex == -1)
      glXSwapBuffers(fPimpl->fDpy, winID);
   else if (ctx.fXImage && ctx.fDirect) {
      if (!ctx.fDirectGC)
         ctx.fDirectGC = XCreateGC(fPimpl->fDpy, winID, 0, 0);

      if (!ctx.fDirectGC) {
         Error("Flush", "XCreateGC failed while copying pixmap\n");
         ctx.fDirect = kFALSE;
         return;
      }

      XCopyArea(fPimpl->fDpy, ctx.fX11Pixmap, winID, ctx.fDirectGC, 0, 0, ctx.fW, ctx.fH, ctx.fX, ctx.fY);
   }
}


//______________________________________________________________________________
Bool_t TX11GLManager::CreateGLPixmap(TGLContext_t &ctx)
{
   // Create GL pixmap.

   // Create new x11 pixmap and XImage.
   Pixmap x11Pix = XCreatePixmap(fPimpl->fDpy, gVirtualX->GetWindowID(ctx.fWindowIndex), ctx.fW,
                                 ctx.fH, fPimpl->fGLWindows[ctx.fWindowIndex]->depth);

   if (!x11Pix) {
      Error("CreateGLPixmap", "XCreatePixmap failed\n");
      return kFALSE;
   }

   TX11PixGuard pixGuard(fPimpl->fDpy, x11Pix);

   // XImage part here.
   XVisualInfo *visInfo = fPimpl->fGLWindows[ctx.fWindowIndex];
   XImage *testIm = XCreateImage(fPimpl->fDpy, visInfo->visual, visInfo->depth, ZPixmap, 0, 0, ctx.fW, ctx.fH, 32, 0);

   if (testIm) {
      TXImageGuard imGuard(testIm);
      testIm->data = static_cast<Char_t *>(malloc(testIm->bytes_per_line * testIm->height));

      if (!testIm->data) {
         Error("CreateGLPixmap", "Cannot malloc XImage data\n");
         return kFALSE;
      }

      if (XInitImage(testIm)) {
         ctx.fPixmapIndex = gVirtualX->AddPixmap(x11Pix, ctx.fW, ctx.fH);
         ctx.fBUBuffer.resize(testIm->bytes_per_line * testIm->height);
         ctx.fX11Pixmap = x11Pix;
         ctx.fXImage = testIm;
         pixGuard.Stop();
         imGuard.Stop();
         return kTRUE;
      } else
         Error("CreateGLPixmap", "XInitImage error!\n");
   } else
      Error("CreateGLPixmap", "XCreateImage error!\n");

   return kFALSE;
}


//______________________________________________________________________________
Bool_t TX11GLManager::AttachOffScreenDevice(Int_t ctxInd, Int_t x, Int_t y, UInt_t w, UInt_t h)
{
   // Attach off screen device.

   // Create pixmap and XImage for GL context ctxInd.
   TGLContext_t &ctx = fPimpl->fGLContexts[ctxInd];
   TGLContext_t newCtx;
   newCtx.fWindowIndex = ctx.fWindowIndex;
   newCtx.fW = w, newCtx.fH = h, newCtx.fX = x, newCtx.fY = y;
   newCtx.fGLXContext = ctx.fGLXContext;

   if (CreateGLPixmap(newCtx)) {
      ctx.fPixmapIndex = newCtx.fPixmapIndex;
      ctx.fX11Pixmap = newCtx.fX11Pixmap;
      ctx.fW = w, ctx.fH = h, ctx.fX = x, ctx.fY = y;
      ctx.fDirect = kFALSE;
      ctx.fXImage = newCtx.fXImage;
      ctx.fBUBuffer.swap(newCtx.fBUBuffer);
      return kTRUE;
   }

   return kFALSE;
}


//______________________________________________________________________________
Bool_t TX11GLManager::ResizeOffScreenDevice(Int_t ctxInd, Int_t x, Int_t y, UInt_t w, UInt_t h)
{
   // Resize off screen devive.

   // Create a new pixmap and a new XImage if needed.
   TGLContext_t &ctx = fPimpl->fGLContexts[ctxInd];

   if (ctx.fPixmapIndex != -1) {
      if (TMath::Abs(Int_t(w) - Int_t(ctx.fW)) > 1 || TMath::Abs(Int_t(h) - Int_t(ctx.fH)) > 1) {
         TGLContext_t newCtx;
         newCtx.fWindowIndex = ctx.fWindowIndex;
         newCtx.fW = w, newCtx.fH = h, newCtx.fX = x, newCtx.fY = y;
         newCtx.fGLXContext = ctx.fGLXContext;

         if (CreateGLPixmap(newCtx)) {
            gVirtualX->SelectWindow(ctx.fPixmapIndex);
            gVirtualX->ClosePixmap();
            ctx.fPixmapIndex = newCtx.fPixmapIndex;
            ctx.fX11Pixmap = newCtx.fX11Pixmap;
            ctx.fW = w, ctx.fH = h, ctx.fX = x, ctx.fY = y;
            ctx.fDirect = kFALSE;
            if (ctx.fXImage) XDestroyImage(ctx.fXImage);
            ctx.fXImage = newCtx.fXImage;
            ctx.fBUBuffer.swap(newCtx.fBUBuffer);
            return kTRUE;
         } else
            Error("ResizeOffScreenDevice", "Resize failed\n");
      } else {
         ctx.fX = x;
         ctx.fY = y;
      }
   }

   return kFALSE;
}


//______________________________________________________________________________
void TX11GLManager::SelectOffScreenDevice(Int_t ctxInd)
{
   // Selects off-screen device to make it accessible by gVirtualX.

   gVirtualX->SelectWindow(fPimpl->fGLContexts[ctxInd].fPixmapIndex);
}


//______________________________________________________________________________
void TX11GLManager::MarkForDirectCopy(Int_t ctxInd, Bool_t dir)
{
   // Selection-rotation support for TPad/TCanvas.

   if (fPimpl->fGLContexts[ctxInd].fPixmapIndex != -1)
      fPimpl->fGLContexts[ctxInd].fDirect = dir;
}


//______________________________________________________________________________
void TX11GLManager::ReadGLBuffer(Int_t ctxInd)
{
   // GL buffer is read info buffer, after that lines are reordered
   // into XImage, XImage copied into pixmap.

   TGLContext_t &ctx = fPimpl->fGLContexts[ctxInd];

   if (ctx.fPixmapIndex != -1 && ctx.fXImage) {
      // Read GL buffer.
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glReadBuffer(GL_BACK);
      glReadPixels(0, 0, ctx.fW, ctx.fH, GL_BGRA, GL_UNSIGNED_BYTE, &ctx.fBUBuffer[0]);

      if (!ctx.fPixmapGC)
         ctx.fPixmapGC = XCreateGC(fPimpl->fDpy, ctx.fX11Pixmap, 0, 0);
      if (ctx.fPixmapGC) {
         // GL buffer read operation gives bottom-up order of pixels, but XImage
	      // require top-down. So, change RGB lines first.
         char *dest = ctx.fXImage->data;
         const UChar_t *src = &ctx.fBUBuffer[ctx.fW * 4 * (ctx.fH - 1)];
         for (UInt_t i = 0, e = ctx.fH; i < e; ++i) {
            memcpy(dest, src, ctx.fW * 4);
            dest += ctx.fW * 4;
            src -= ctx.fW * 4;
         }
         XPutImage(fPimpl->fDpy, ctx.fX11Pixmap, ctx.fPixmapGC, ctx.fXImage, 0, 0, 0, 0, ctx.fW, ctx.fH);
      } else
         Error("ReadGLBuffer", "XCreateGC error while attempt to copy XImage\n");
   }
}


//______________________________________________________________________________
void TX11GLManager::DeleteGLContext(Int_t ctxInd)
{
   // Deletes GLX context and frees pixmap and image (if any).

   TGLContext_t &ctx = fPimpl->fGLContexts[ctxInd];

   // Free GL context.
   glXDestroyContext(fPimpl->fDpy, ctx.fGLXContext);
   ctx.fGLXContext = 0;

   // If the pixmap exists it is destroyed.
   if (ctx.fPixmapIndex != -1) {
      gVirtualX->SelectWindow(ctx.fPixmapIndex);
      gVirtualX->ClosePixmap();
      ctx.fPixmapIndex = -1;
      if(ctx.fXImage) {
         XDestroyImage(ctx.fXImage);
         ctx.fXImage = 0;
      }
      if (ctx.fDirectGC)
         XFreeGC(fPimpl->fDpy, ctx.fDirectGC), ctx.fDirectGC = 0;
      if (ctx.fPixmapGC)
         XFreeGC(fPimpl->fDpy, ctx.fPixmapGC), ctx.fPixmapGC = 0;
   }

   ctx.fNextFreeContext = fPimpl->fNextFreeContext;
   fPimpl->fNextFreeContext = &ctx;
   ctx.fWindowIndex = ctxInd;
}


//______________________________________________________________________________
Int_t TX11GLManager::GetVirtualXInd(Int_t ctxInd)
{
   // Returns an index suitable for gVirtualX.

   return fPimpl->fGLContexts[ctxInd].fPixmapIndex;
}


//______________________________________________________________________________
void TX11GLManager::ExtractViewport(Int_t ctxInd, Int_t *viewport)
{
   // Returns the current dimensions of a GL pixmap.

   TGLContext_t &ctx = fPimpl->fGLContexts[ctxInd];

   if (ctx.fPixmapIndex != -1) {
      viewport[0] = 0;
      viewport[1] = 0;
      viewport[2] = ctx.fW;
      viewport[3] = ctx.fH;
   }
}


//______________________________________________________________________________
void TX11GLManager::PaintSingleObject(TVirtualGLPainter *p)
{
   // Paint a single object.

   p->Paint();
}


//______________________________________________________________________________
void TX11GLManager::PrintViewer(TVirtualViewer3D *vv)
{
   // Print viewer.

   vv->PrintObjects();
}

//______________________________________________________________________________
Bool_t TX11GLManager::SelectManip(TVirtualGLManip *manip, const TGLCamera * camera, const TGLRect * rect, const TGLBoundingBox * sceneBox)
{
   // Select manipulator.

   return manip->Select(*camera, *rect, *sceneBox);
}


//______________________________________________________________________________
void TX11GLManager::PanObject(TVirtualGLPainter *o, Int_t x, Int_t y)
{
   // Pan objects.

   return o->Pan(x, y);
}

//______________________________________________________________________________
Bool_t TX11GLManager::PlotSelected(TVirtualGLPainter *plot, Int_t px, Int_t py)
{
   //Analog of TObject::DistancetoPrimitive
   return plot->PlotSelected(px, py);
}

//______________________________________________________________________________
char *TX11GLManager::GetPlotInfo(TVirtualGLPainter *plot, Int_t px, Int_t py)
{
   //Analog of TObject::GetObjectInfo
   return plot->GetPlotInfo(px, py);
}