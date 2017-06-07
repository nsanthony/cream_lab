// @(#)root/gl:$Id: TGLOverlay.h 28885 2009-06-10 15:51:12Z matevz $
// Author:  Matevz Tadel, Feb 2007

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGLOverlay_H
#define ROOT_TGLOverlay_H

#include <GuiTypes.h>

class TGLRnrCtx;
class TGLOvlSelectRecord;

#include <list>

class TGLOverlayElement
{
public:
   enum ERole { kUser, kViewer, kAnnotation, kAll };

private:
   TGLOverlayElement(const TGLOverlayElement&);            // Not implemented
   TGLOverlayElement& operator=(const TGLOverlayElement&); // Not implemented

protected:
   ERole   fRole;
    
public:
   TGLOverlayElement(ERole r = kUser):fRole(r) {}
   virtual ~TGLOverlayElement() {}

   virtual Bool_t MouseEnter(TGLOvlSelectRecord& selRec);
   virtual Bool_t MouseStillInside(TGLOvlSelectRecord& selRec);
   virtual Bool_t Handle(TGLRnrCtx& rnrCtx, TGLOvlSelectRecord& selRec,
                         Event_t* event);
   virtual void   MouseLeave();

   virtual void Render(TGLRnrCtx& rnrCtx) = 0;

   ERole   GetRole() const { return fRole; }
   void    SetRole(ERole r) { fRole = r; }

   ClassDef(TGLOverlayElement, 0) // Base class for GL overlay elements.
};


class TGLOverlayList
{
private:
   TGLOverlayList(const TGLOverlayList&);            // Not implemented
   TGLOverlayList& operator=(const TGLOverlayList&); // Not implemented

protected:
   std::list<TGLOverlayElement*> fElements;

public:
   TGLOverlayList() : fElements() {}
   virtual ~TGLOverlayList() {}

   // void AddElement(TGLOverlayElement* element);
   // void RemoveElement(TGLOverlayElement* element);

   // TGLOverlayElement* SelectElement(TGLSelectRecord& selRec, Int_t nameOff);

   ClassDef(TGLOverlayList, 0) // Collection of overlay elements to draw/select together.
};


#endif
