// @(#):$Id: TGeoTabManager.cxx 21565 2007-12-28 12:28:46Z brun $
// Author: M.Gheata 

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//______________________________________________________________________________
//  TGeoTabManager  - manager for all editor tabs.                                                    //
//                                                                      
//  TGeoTreeDialog  - Base class for dialog frames for selecting objects 
// with a tree hierarchy. Specific implementations are:
//     TGeoVolumeDialog -  Special tree dialog class for selecting volumes.
//     TGeoShapeDialog  -  Special tree dialog class for selecting shapes. 
//     TGeoMediumDialog -  Special tree dialog class for selecting media.
//     TGeoMaterialDialog - Special tree dialog class for selecting materials.
//     TGeoMatrixDialog -  Special tree dialog class for selecting matrices.                   
//  TGeoTransientPanel - Special transient tab holding TGeo editors.
//______________________________________________________________________________

#include "TROOT.h"
#include "TClass.h"
#include "TVirtualPad.h"
#include "TGeoGedFrame.h"
#include "TGTab.h"
#include "TGLabel.h"
#include "TGComboBox.h"
#include "TGListBox.h"
#include "TGListTree.h"
#include "TGTextEntry.h"
#include "TGCanvas.h"
#include "TGMimeTypes.h"

#include "TGeoManager.h"
#include "TGeoShape.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"

#include "TGedEditor.h"
#include "TGeoTabManager.h"

TMap TGeoTabManager::fgEditorToMgrMap;

ClassImp(TGeoTabManager)

//______________________________________________________________________________
TGeoTabManager::TGeoTabManager(TGedEditor *ged)
{
// Ctor.
   fGedEditor = ged;
   fPad = ged->GetPad();
   fTab = ged->GetTab();
   fVolume = 0;
   fShapePanel = 0;
   fMediumPanel = 0;
   fMaterialPanel = 0;
   fMatrixPanel = 0;
   fVolumeTab = 0;
   fgEditorToMgrMap.Add(ged, this);
}   

//______________________________________________________________________________
TGeoTabManager::~TGeoTabManager()
{
// Dtor.
   fgEditorToMgrMap.Remove(fGedEditor);
   if (fShapePanel) delete fShapePanel;
   if (fMaterialPanel) delete fMaterialPanel;
   if (fMatrixPanel) delete fMatrixPanel;
   if (fMediumPanel) delete fMediumPanel;
}

//______________________________________________________________________________
void TGeoTabManager::Cleanup(TGCompositeFrame *frame)
{
// Static method to cleanup hirarchically all daughters of a composite frame.
// Does not remove the frame itself.
   TGFrameElement *el;
   TList *list = frame->GetList();
   Int_t nframes = list->GetSize();
   TClass *cl;
   for (Int_t i=0; i<nframes; i++) {
      el = (TGFrameElement *)list->At(i);
      cl = el->fFrame->IsA();
      if (cl==TGCompositeFrame::Class() || cl==TGHorizontalFrame::Class() || cl==TGVerticalFrame::Class())
         Cleanup((TGCompositeFrame*)el->fFrame);
   }
   frame->Cleanup();
}   

//______________________________________________________________________________
void TGeoTabManager::GetShapeEditor(TGeoShape *shape)
{
// Get editor for a shape.
   if (!shape) return;
   if (!fShapePanel) fShapePanel = new TGeoTransientPanel(fGedEditor, "Shape", shape);
   else {
      fShapePanel->SetModel(shape);   
      fShapePanel->Show();
   }   
}

//______________________________________________________________________________
void TGeoTabManager::GetVolumeEditor(TGeoVolume *volume)
{
// Get editor for a volume.
   if (!volume || !fVolumeTab) return;
   GetEditors(TAttLine::Class());
   GetEditors(TGeoVolume::Class());
   fVolumeTab->MapSubwindows();
   fVolumeTab->Layout();
   SetModel(volume);
}
   
//______________________________________________________________________________
void TGeoTabManager::GetMatrixEditor(TGeoMatrix *matrix)
{
// Get editor for a matrix.
   if (!matrix) return;
   if (!fMatrixPanel) fMatrixPanel = new TGeoTransientPanel(fGedEditor, "Matrix", matrix);
   else {
      fMatrixPanel->SetModel(matrix);
      fMatrixPanel->Show();
   }   
}

//______________________________________________________________________________
void TGeoTabManager::GetMediumEditor(TGeoMedium *medium)
{
// Get editor for a medium.
   if (!medium) return;
   if (!fMediumPanel) fMediumPanel = new TGeoTransientPanel(fGedEditor, "Medium", medium);
   else {
      fMediumPanel->SetModel(medium);
      fMediumPanel->Show();
   }   
}

//______________________________________________________________________________
void TGeoTabManager::GetMaterialEditor(TGeoMaterial *material)
{
// Get editor for a material.
   if (!material) return;
   TString name = "Material";
   if (material->IsMixture()) name = "Mixture";
   if (!fMaterialPanel) fMaterialPanel = new TGeoTransientPanel(fGedEditor, name.Data(), material);
   else {
      fMaterialPanel->SetModel(material);
      fMaterialPanel->Show();
   }   
}

//______________________________________________________________________________
void TGeoTabManager::GetEditors(TClass *cl)
{
// Get editor for a class.
   // Look in fVolumeTab for any object deriving from TGedFrame,

   TClass *class2 = TClass::GetClass(Form("%sEditor",cl->GetName()));
   if (class2 && class2->InheritsFrom(TGedFrame::Class())) {
      TGFrameElement *fr;
      TIter next(fVolumeTab->GetList());
      while ((fr = (TGFrameElement *) next())) if (fr->fFrame->IsA() == class2) return;
      TGClient *client = fGedEditor->GetClient();
      TGWindow *exroot = (TGWindow*) client->GetRoot();
      client->SetRoot(fVolumeTab);
      TGedEditor::SetFrameCreator(fGedEditor);
      TGedFrame* gfr = reinterpret_cast<TGedFrame*>(class2->New());
      gfr->SetModelClass(cl);
      TGedEditor::SetFrameCreator(0);
      client->SetRoot(exroot);
      fVolumeTab->AddFrame(gfr, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 2, 2));
      gfr->MapSubwindows();
   }
}

//______________________________________________________________________________
TGeoTabManager *TGeoTabManager::GetMakeTabManager(TGedEditor *ged)
{
// Static method to return the tab manager currently appended to the pad or create one 
// if not existing.
   if (!ged) return NULL;
   TPair *pair = (TPair*) fgEditorToMgrMap.FindObject(ged);
   if (pair) {
      return (TGeoTabManager*) pair->Value();
   } else {
      TGeoTabManager *tabmgr = new TGeoTabManager(ged); // added to fgEditorToMgrMap in ctor
      return tabmgr;
   }
}   

//______________________________________________________________________________
Int_t TGeoTabManager::GetTabIndex() const
{
// Get index for a given tab element.
   Int_t ntabs = fTab->GetNumberOfTabs();
   TString tabname = "Volume";
                     
   TGTabElement *tel;
   for (Int_t i=0; i<ntabs; i++) {
      tel = fTab->GetTabTab(i);
      if (tel && !strcmp(tel->GetString(),tabname.Data())) return i;
   }   
   return 0;
}

//______________________________________________________________________________
void TGeoTabManager::MoveFrame(TGCompositeFrame *fr, TGCompositeFrame *p)
{
// Move frame fr at the end of the list of parent p.
   TList *list = p->GetList();
   TIter next(list);
   TGFrameElement *el = 0;
   while ((el=(TGFrameElement*)next())) {
      if (el->fFrame == fr) break;
   }  
   if (el) { 
      list->Remove(el);
      list->Add(el);
   }   
}

//______________________________________________________________________________
void TGeoTabManager::SetVolTabEnabled(Bool_t flag)
{
// Enable/disable tabs
   fTab->SetEnabled(GetTabIndex(), flag);
}

//______________________________________________________________________________
void TGeoTabManager::SetModel(TObject *model)
{
// Send the SetModel signal to all editors in the tab TYPE.
   TGCompositeFrame *tab = fVolumeTab;
   fVolume = (TGeoVolume*)model;
   TGFrameElement *el;
   TIter next(tab->GetList());
   while ((el = (TGFrameElement *) next())) {
      if ((el->fFrame)->InheritsFrom(TGedFrame::Class())) {
         ((TGedFrame *)(el->fFrame))->SetModel(model);
      }   
   }
}      

//______________________________________________________________________________
void TGeoTabManager::SetTab()
{
// Set a given tab element as active one.
   fTab->SetTab(GetTabIndex());
}
   
ClassImp(TGeoTreeDialog)

TObject *TGeoTreeDialog::fgSelectedObj = 0;

//______________________________________________________________________________
TObject *TGeoTreeDialog::GetSelected() 
{
   //static; return selected object
   return fgSelectedObj;
}

//______________________________________________________________________________
TGeoTreeDialog::TGeoTreeDialog(TGFrame *caller, const TGWindow *main, UInt_t w, UInt_t h)
               :TGTransientFrame(main, main, w, h)
{
// Constructor
   fgSelectedObj = 0;
   fCanvas = new TGCanvas(this, 100, 200,  kSunkenFrame | kDoubleBorder);
   fLT = new TGListTree(fCanvas->GetViewPort(), 100, 200);
   fLT->Associate(this);
   fCanvas->SetContainer(fLT);
   AddFrame(fCanvas, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 2,2,2,2));
   f1 = new TGCompositeFrame(this, 100, 10, kHorizontalFrame | kLHintsExpandX);
   fObjLabel = new TGLabel(f1, "Selected: -none-");
   Pixel_t color;
   gClient->GetColorByName("#0000ff", color);
   fObjLabel->SetTextColor(color);
   fObjLabel->ChangeOptions(kSunkenFrame | kDoubleBorder);
   f1->AddFrame(fObjLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 2,2,2,2));
   fClose = new TGTextButton(f1, "&Close");
   fClose->Associate(this);
   f1->AddFrame(fClose, new TGLayoutHints(kLHintsRight, 2,2,2,2)); 
   AddFrame(f1, new TGLayoutHints(kLHintsBottom | kLHintsExpandX, 2,2,2,2));
   
   Int_t ww = caller->GetWidth();
   Window_t wdum;
   Int_t    ax, ay;
   gVirtualX->TranslateCoordinates(caller->GetId(), main->GetId(), 0,0,ax,ay,wdum);
   Move(ax + ww, ay);
   SetWMPosition(ax, ay);
   
}

//______________________________________________________________________________
TGeoTreeDialog::~TGeoTreeDialog()
{
// Destructor
   delete fClose;
   delete fObjLabel;
   delete f1;
   delete fLT;
   delete fCanvas;
}

//______________________________________________________________________________
void TGeoTreeDialog::DoSelect(TGListTreeItem *item)
{
// Update dialog to reflect current clicked object.
   static char name[256];
   if (!item || !item->GetUserData()) {
      fgSelectedObj = 0;
      if (!strcmp(name, "Selected: -none-")) return;
      sprintf(name,"Selected: -none-");
      fObjLabel->SetText(name);
   }
   fgSelectedObj = (TObject *)item->GetUserData();
   if (fgSelectedObj) {
      sprintf(name, "Selected %s", fgSelectedObj->GetName());
      fObjLabel->SetText(name);
   }   
}   
      
ClassImp(TGeoVolumeDialog)

//______________________________________________________________________________
TGeoVolumeDialog::TGeoVolumeDialog(TGFrame *caller, const TGWindow *main, UInt_t w, UInt_t h)
                 :TGeoTreeDialog(caller, main, w, h)
{
// Ctor.
   BuildListTree();   
   ConnectSignalsToSlots();
   MapSubwindows();
   Layout();
   SetWindowName("Volume dialog");
   MapWindow();
   gClient->WaitForUnmap(this);
}

//______________________________________________________________________________
void TGeoVolumeDialog::BuildListTree()
{
// Build volume specific list tree.
   const TGPicture *pic_fld = gClient->GetPicture("folder_t.xpm");
   const TGPicture *pic_fldo = gClient->GetPicture("ofolder_t.xpm");
   const TGPicture *pic_file = gClient->GetPicture("mdi_default.xpm");
   const TGPicture *pic_fileo = gClient->GetPicture("fileopen.xpm");
   TGListTreeItem *parent_item=0;
   TGeoVolume *parent_vol = gGeoManager->GetMasterVolume();
   TGeoVolume *vol;
   // Existing volume hierarchy
   parent_item = fLT->AddItem(parent_item, "Volume hierarchy", pic_fldo, pic_fld);
   parent_item->SetTipText("Select a volume from the existing hierarchy");
   fLT->OpenItem(parent_item);
   if (parent_vol) {
      if (!parent_vol->GetNdaughters()) {
         parent_item = fLT->AddItem(parent_item, parent_vol->GetName(), parent_vol, pic_fileo, pic_file);
         parent_item->SetTipText("Master volume");
         fLT->SetSelected(parent_item);
      } else {
         parent_item = fLT->AddItem(parent_item, parent_vol->GetName(), parent_vol, pic_fldo, pic_fld);
         parent_item->SetTipText("Master volume");
         fLT->SetSelected(parent_item);
      }
   }
   parent_item = fLT->AddItem(NULL, "Other volumes", pic_fldo, pic_fld);
   parent_item->SetTipText("Select a volume from the list of unconnected volumes");
   TIter next1(gGeoManager->GetListOfVolumes());
   Bool_t found = kFALSE;
   while ((vol=(TGeoVolume*)next1())) {
      if (vol->IsAdded()) continue;
      fLT->AddItem(parent_item, vol->GetName(), vol, pic_fileo, pic_file);
      found = kTRUE;
   }
   if (found) {    
//      fLT->OpenItem(parent_item);   
      if (!parent_vol) fLT->SetSelected(parent_item->GetFirstChild());
   }        
}

//______________________________________________________________________________
void TGeoVolumeDialog::DoClose()
{
// Handle close button.
   DeleteWindow();
}   

//______________________________________________________________________________
void TGeoVolumeDialog::DoItemClick(TGListTreeItem *item, Int_t btn)
{
// Handle item click.
   // Iterate daughters
   if (btn!=kButton1) return;
   DoSelect(item);   
   if (!item || !item->GetUserData()) return;
   const TGPicture *pic_fld = gClient->GetPicture("folder_t.xpm");
   const TGPicture *pic_fldo = gClient->GetPicture("ofolder_t.xpm");
   const TGPicture *pic_file = gClient->GetPicture("mdi_default.xpm");
   const TGPicture *pic_fileo = gClient->GetPicture("fileopen.xpm");
   TGeoVolume *parent_vol = (TGeoVolume*)item->GetUserData();
   TGeoVolume *vol;
   TGeoNode *crtnode;
   TGListTreeItem *daughter_item;
   Int_t i,j,ind,icopy;
   Int_t nd = parent_vol->GetNdaughters();
   for (i=0; i<nd; i++) {
      icopy = 0;
      crtnode = parent_vol->GetNode(i);
      vol = crtnode->GetVolume();
      // check if the volume is replicated in the parent
      ind = parent_vol->GetIndex(crtnode);
      for (j=0; j<ind; j++) if (parent_vol->GetNode(j)->GetVolume() == vol) break;
      if (i<ind) continue;
      icopy++;
      for (j=ind+1; j<nd; j++) if (parent_vol->GetNode(j)->GetVolume() == vol) icopy++;
      daughter_item = fLT->AddItem(item, ((icopy>1)?Form("%s (%i)",vol->GetName(),icopy):vol->GetName()), 
                    vol,((vol->GetNdaughters())?pic_fldo:pic_fileo), ((vol->GetNdaughters())?pic_fld:pic_file));
      if (strlen(vol->GetTitle())) daughter_item->SetTipText(vol->GetTitle());            
   }
   if (nd) gClient->NeedRedraw(fLT);
}   

//______________________________________________________________________________
void TGeoVolumeDialog::ConnectSignalsToSlots()
{
// Connect signals to slots.
   fClose->Connect("Clicked()", "TGeoVolumeDialog", this, "DoClose()");
   fLT->Connect("Clicked(TGListTreeItem *, Int_t)", "TGeoVolumeDialog", this, 
                "DoItemClick(TGListTreeItem *, Int_t)");
}

ClassImp(TGeoShapeDialog)

//______________________________________________________________________________
TGeoShapeDialog::TGeoShapeDialog(TGFrame *caller, const TGWindow *main, UInt_t w, UInt_t h)
                 :TGeoTreeDialog(caller, main, w, h)
{
// Ctor.
   BuildListTree();   
   ConnectSignalsToSlots();
   MapSubwindows();
   Layout();
   SetWindowName("Shape dialog");
   MapWindow();
   gClient->WaitForUnmap(this);
}

//______________________________________________________________________________
void TGeoShapeDialog::BuildListTree()
{
// Build shape specific list tree.
   const TGPicture *pic_fld = gClient->GetPicture("folder_t.xpm");
   const TGPicture *pic_fldo = gClient->GetPicture("ofolder_t.xpm");
   const TGPicture *pic_shape;
   TGListTreeItem *parent_item=0;
   TGeoShape *shape;
   const char *shapename;
   TString fld_name;
   Int_t nshapes = gGeoManager->GetListOfShapes()->GetEntriesFast();
   if (!nshapes) return;
   // Existing shapes
   for (Int_t i=0; i<nshapes; i++) {
      shape = (TGeoShape*)gGeoManager->GetListOfShapes()->At(i);
      shapename = shape->IsA()->GetName();
      pic_shape = fClient->GetMimeTypeList()->GetIcon(shapename, kTRUE);
      fld_name = shapename;  // e.g. "TGeoBBox"
      fld_name.Remove(0,4); // remove "TGeo" part -> "BBox"
      fld_name += " Shapes";
      parent_item = fLT->FindChildByName(NULL, fld_name.Data());
      if (!parent_item) {
         parent_item = fLT->AddItem(NULL, fld_name.Data(), pic_fldo, pic_fld);
         parent_item->SetTipText(Form("List of %s shapes",fld_name.Data()));
      }
      fLT->AddItem(parent_item, shape->GetName(), shape, pic_shape, pic_shape);
   }   
}

//______________________________________________________________________________
void TGeoShapeDialog::DoClose()
{
// Handle close button.
   DeleteWindow();
}   

//______________________________________________________________________________
void TGeoShapeDialog::DoItemClick(TGListTreeItem *item, Int_t btn)
{
// Handle item click.
   // Iterate daughters
   if (btn!=kButton1) return;
   DoSelect(item);   
   if (!item || !item->GetUserData()) return;
}   

//______________________________________________________________________________
void TGeoShapeDialog::ConnectSignalsToSlots()
{
// Connect signals to slots.
   fClose->Connect("Clicked()", "TGeoShapeDialog", this, "DoClose()");
   fLT->Connect("Clicked(TGListTreeItem *, Int_t)", "TGeoShapeDialog", this, 
                "DoItemClick(TGListTreeItem *, Int_t)");
}

ClassImp(TGeoMediumDialog)

//______________________________________________________________________________
TGeoMediumDialog::TGeoMediumDialog(TGFrame *caller, const TGWindow *main, UInt_t w, UInt_t h)
                 :TGeoTreeDialog(caller, main, w, h)
{
// Ctor.
   BuildListTree();   
   ConnectSignalsToSlots();
   MapSubwindows();
   Layout();
   SetWindowName("Medium dialog");
   MapWindow();
   gClient->WaitForUnmap(this);
}

//______________________________________________________________________________
void TGeoMediumDialog::BuildListTree()
{
// Build volume specific list tree.
   const TGPicture *pic_med = gClient->GetPicture("geomedium_t.xpm");;
   TGeoMedium *med;
   Int_t nmed = gGeoManager->GetListOfMedia()->GetSize();
   if (!nmed) return;
   // Existing media
   for (Int_t i=0; i<nmed; i++) {
      med = (TGeoMedium*)gGeoManager->GetListOfMedia()->At(i);
      fLT->AddItem(NULL, med->GetName(), med, pic_med, pic_med);
   }   
}

//______________________________________________________________________________
void TGeoMediumDialog::DoClose()
{
// Handle close button.
   DeleteWindow();
}   

//______________________________________________________________________________
void TGeoMediumDialog::DoItemClick(TGListTreeItem *item, Int_t btn)
{
// Handle item click.
   // Iterate daughters
   if (btn!=kButton1) return;
   DoSelect(item);   
   if (!item || !item->GetUserData()) return;
   //gClient->NeedRedraw(fLT);
}   

//______________________________________________________________________________
void TGeoMediumDialog::ConnectSignalsToSlots()
{
// Connect signals to slots.
   fClose->Connect("Clicked()", "TGeoMediumDialog", this, "DoClose()");
   fLT->Connect("Clicked(TGListTreeItem *, Int_t)", "TGeoMediumDialog", this, 
                "DoItemClick(TGListTreeItem *, Int_t)");
}

ClassImp(TGeoMaterialDialog)

//______________________________________________________________________________
TGeoMaterialDialog::TGeoMaterialDialog(TGFrame *caller, const TGWindow *main, UInt_t w, UInt_t h)
                 :TGeoTreeDialog(caller, main, w, h)
{
// Ctor.
   BuildListTree();   
   ConnectSignalsToSlots();
   MapSubwindows();
   Layout();
   SetWindowName("Material dialog");
   MapWindow();
   gClient->WaitForUnmap(this);
}

//______________________________________________________________________________
void TGeoMaterialDialog::BuildListTree()
{
// Build volume specific list tree.
   const TGPicture *pic_mat = gClient->GetPicture("geomaterial_t.xpm");;
   TGeoMaterial *mat;
   Int_t nmat = gGeoManager->GetListOfMaterials()->GetSize();
   if (!nmat) return;
   // Existing materials
   for (Int_t i=0; i<nmat; i++) {
      mat = (TGeoMaterial*)gGeoManager->GetListOfMaterials()->At(i);
      fLT->AddItem(NULL, mat->GetName(), mat, pic_mat, pic_mat);
   }   
}

//______________________________________________________________________________
void TGeoMaterialDialog::DoClose()
{
// Handle close button.
   DeleteWindow();
}   

//______________________________________________________________________________
void TGeoMaterialDialog::DoItemClick(TGListTreeItem *item, Int_t btn)
{
// Handle item click.
   // Iterate daughters
   if (btn!=kButton1) return;
   DoSelect(item);   
   if (!item || !item->GetUserData()) return;
   //gClient->NeedRedraw(fLT);
}   

//______________________________________________________________________________
void TGeoMaterialDialog::ConnectSignalsToSlots()
{
// Connect signals to slots.
   fClose->Connect("Clicked()", "TGeoMaterialDialog", this, "DoClose()");
   fLT->Connect("Clicked(TGListTreeItem *, Int_t)", "TGeoMaterialDialog", this, 
                "DoItemClick(TGListTreeItem *, Int_t)");
}

ClassImp(TGeoMatrixDialog)

//______________________________________________________________________________
TGeoMatrixDialog::TGeoMatrixDialog(TGFrame *caller, const TGWindow *main, UInt_t w, UInt_t h)
                 :TGeoTreeDialog(caller, main, w, h)
{
// Ctor.
   BuildListTree();   
   ConnectSignalsToSlots();
   MapSubwindows();
   Layout();
   SetWindowName("Matrix dialog");
   MapWindow();
   gClient->WaitForUnmap(this);
}

//______________________________________________________________________________
void TGeoMatrixDialog::BuildListTree()
{
// Build matrix specific list tree.
   const TGPicture *pic_tr = gClient->GetPicture("geotranslation_t.xpm");
   const TGPicture *pic_rot = gClient->GetPicture("georotation_t.xpm");
   const TGPicture *pic_combi = gClient->GetPicture("geocombi_t.xpm");
   const TGPicture *pic;
   TGListTreeItem *parent_item=0;
   TGeoMatrix *matrix;
   Int_t nmat = gGeoManager->GetListOfMatrices()->GetEntriesFast();
   if (!nmat) return;
   // Existing matrices
   for (Int_t i=0; i<nmat; i++) {
      matrix = (TGeoMatrix*)gGeoManager->GetListOfMatrices()->At(i);
      if (matrix->IsIdentity()) continue;
      if (!strcmp(matrix->IsA()->GetName(),"TGeoTranslation")) {
         pic = pic_tr;
         parent_item = fLT->FindChildByName(NULL, "Translations");
         if (!parent_item) {
            parent_item = fLT->AddItem(NULL, "Translations", pic, pic);
            parent_item->SetTipText("List of translations");
         }            
      } else if (!strcmp(matrix->IsA()->GetName(),"TGeoRotation")) {
         pic = pic_rot;
         parent_item = fLT->FindChildByName(NULL, "Rotations");
         if (!parent_item) {
            parent_item = fLT->AddItem(NULL, "Rotations", pic, pic);
            parent_item->SetTipText("List of rotations");
         } 
      } else if (!strcmp(matrix->IsA()->GetName(),"TGeoCombiTrans") ||
                  !strcmp(matrix->IsA()->GetName(),"TGeoHMatrix")) {
         pic = pic_combi;
         parent_item = fLT->FindChildByName(NULL, "Combined");
         if (!parent_item) {
            parent_item = fLT->AddItem(NULL, "Combined", pic, pic);
            parent_item->SetTipText("List of combined transformations");
         } 
      } else continue;
      fLT->AddItem(parent_item, matrix->GetName(), matrix, pic, pic);
   }   
}

//______________________________________________________________________________
void TGeoMatrixDialog::DoClose()
{
// Handle close button.
   DeleteWindow();
}   

//______________________________________________________________________________
void TGeoMatrixDialog::DoItemClick(TGListTreeItem *item, Int_t btn)
{
// Handle item click.
   // Iterate daughters
   if (btn!=kButton1) return;
   DoSelect(item);   
   if (!item || !item->GetUserData()) return;
   //gClient->NeedRedraw(fLT);
}   

//______________________________________________________________________________
void TGeoMatrixDialog::ConnectSignalsToSlots()
{
// Connect signals to slots.
   fClose->Connect("Clicked()", "TGeoMatrixDialog", this, "DoClose()");
   fLT->Connect("Clicked(TGListTreeItem *, Int_t)", "TGeoMatrixDialog", this, 
                "DoItemClick(TGListTreeItem *, Int_t)");
}

ClassImp(TGeoTransientPanel)

//______________________________________________________________________________
TGeoTransientPanel::TGeoTransientPanel(TGedEditor* ged, const char *name, TObject *obj)
                   :TGMainFrame(gClient->GetRoot(),175,20)
{
// Transient panel ctor.
   fGedEditor = ged;
   fModel = obj;
   fCan = new TGCanvas(this, 170, 100);
   fTab = new TGTab(fCan->GetViewPort(), 10, 10);
   fCan->SetContainer(fTab);
   AddFrame(fCan, new TGLayoutHints(kLHintsExpandY | kLHintsExpandX));
   fTab->Associate(fCan);
   fTabContainer = fTab->AddTab(name);
   fStyle = new TGCompositeFrame(fTabContainer, 110, 30, kVerticalFrame);
   fTabContainer->AddFrame(fStyle, new TGLayoutHints(kLHintsTop | kLHintsExpandX,\
                                                     5, 0, 2, 2));
   TString wname = name;
   wname += " Editor";
   SetWindowName(wname.Data());
   SetModel(fModel);
   fClose = new TGTextButton(this, "Close");
   AddFrame(fClose, new TGLayoutHints(kLHintsBottom | kLHintsRight, 0,10,5,5));
   MapSubwindows();
   Layout();
   Resize(fTabContainer->GetDefaultWidth()+30, fTabContainer->GetDefaultHeight()+65);
   MapWindow();
   gROOT->GetListOfCleanups()->Add(this);
   fClose->Connect("Clicked()", "TGeoTransientPanel", this, "Hide()");
} 

//______________________________________________________________________________
TGeoTransientPanel::~TGeoTransientPanel()
{
// Destructor.
   DeleteEditors();
   delete fTab;
   delete fCan;
   gROOT->GetListOfCleanups()->Remove(this);
}

//______________________________________________________________________________
void TGeoTransientPanel::CloseWindow()
{
   // When closed via WM close button, just unmap (i.e. hide) editor
   // for later use.
   UnmapWindow();
   gROOT->GetListOfCleanups()->Remove(this);
}

//______________________________________________________________________________
void TGeoTransientPanel::GetEditors(TClass *cl)
{
// Get editor for a class.
   // Look in fStyle for any object deriving from TGedFrame,

   TClass *class2 = TClass::GetClass(Form("%sEditor",cl->GetName()));
   if (class2 && class2->InheritsFrom(TGedFrame::Class())) {
      TGFrameElement *fr;
      TIter next(fStyle->GetList());
      while ((fr = (TGFrameElement *) next()))
         if (fr->fFrame->IsA() == class2) return;
      TGClient *client = fGedEditor->GetClient();
      TGWindow *exroot = (TGWindow*) client->GetRoot();
      client->SetRoot(fStyle);
      TGedEditor::SetFrameCreator(fGedEditor);
      TGedFrame* gfr = reinterpret_cast<TGedFrame*>(class2->New());
      gfr->SetModelClass(cl);
      TGedEditor::SetFrameCreator(0);
      client->SetRoot(exroot);
      fStyle->AddFrame(gfr, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 2, 2));
      gfr->MapSubwindows();
   }
}

//______________________________________________________________________________
void TGeoTransientPanel::SetModel(TObject *model)
{
// Update the editors in the main tab to reflect the selected object.
   if (!model) return;
   fModel = model;
   GetEditors(model->IsA());
   TGFrameElement *el;
   TIter next(fStyle->GetList());
   while ((el = (TGFrameElement *) next())) {
      if ((el->fFrame)->InheritsFrom(TGedFrame::Class())) {
         ((TGedFrame *)(el->fFrame))->SetModel(model);
      }   
   }
   Resize(fTabContainer->GetDefaultWidth()+30, fTabContainer->GetDefaultHeight()+65);
}

//______________________________________________________________________________
void TGeoTransientPanel::Hide()
{
// Hide the transient frame
   UnmapWindow();
}

//______________________________________________________________________________
void TGeoTransientPanel::Show()
{
// Hide the transient frame
   MapWindow();
}

//______________________________________________________________________________
void TGeoTransientPanel::DeleteEditors()
{
// Delete editors.
   fStyle->Cleanup();
}

   
