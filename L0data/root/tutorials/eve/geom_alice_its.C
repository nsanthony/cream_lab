// @(#)root/eve:$Id: geom_alice_its.C 26876 2008-12-12 14:45:40Z matevz $
// Author: Matevz Tadel

// Shows geometry of ALICE ITS.

void geom_alice_its()
{
   TEveManager::Create();

   gGeoManager = gEve->GetGeometry("http://root.cern.ch/files/alice.root");

   TGeoNode* node = gGeoManager->GetTopVolume()->FindNode("ITSV_1");
   TEveGeoTopNode* its = new TEveGeoTopNode(gGeoManager, node);
   gEve->AddGlobalElement(its);

   gEve->Redraw3D(kTRUE);
}
