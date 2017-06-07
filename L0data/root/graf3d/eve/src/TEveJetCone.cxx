// @(#)root/eve:$Id: TEveJetCone.cxx 28214 2009-04-15 11:32:25Z matevz $
// Author: Matevz Tadel, Jochen Thaeder 2009

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TEveJetCone.h"
#include "TEveTrans.h"

#include "TBuffer3D.h"
#include "TBuffer3DTypes.h"
#include "TVirtualPad.h"
#include "TVirtualViewer3D.h"

#include "TMath.h"

//______________________________________________________________________________
//
// Draws a jet cone with leading particle is specified in (eta,phi) and
// cone radius is given.
//
// If Apex is not set, default is (0.,0.,0.)
// In case of cylinder was set, cone is cut at the cylinder edges
// example :
//
//  Float_t coneEta    = r.Uniform(-0.9, 0.9);
//  Float_t conePhi    = r.Uniform(0.0, TwoPi() );
//  Float_t coneRadius = 0.4;
//
//  TEveJetCone* jetCone = new TEveJetCone("JetCone");
//  jetCone->SetCylinder( 250., 250. );
//  if ( (jetCone->AddCone( coneEta, conePhi, coneRadius   ) ) != -1)
//    gEve->AddElement( jetCone );
//

ClassImp(TEveJetCone);

//______________________________________________________________________________
TEveJetCone::TEveJetCone(const Text_t* n, const Text_t* t) :
   TEveElementList(n, t, kTRUE),
   TAttBBox(),
   fApex( TEveVector(0.,0.,0.) ),
   fBasePoints(),
   fCylinderBorder( TEveVector(-1.,0.,-1.) ),
   fThetaC(0.)
{
   // Constructor.

   fColor = kGreen;
}


/******************************************************************************/

//______________________________________________________________________________
void TEveJetCone::ComputeBBox()
{
   // Compute bounding-box of the data.

   BBoxInit();
   BBoxCheckPoint(fApex);
   for (vTEveVector_ci i = fBasePoints.begin(); i != fBasePoints.end(); ++i)
   {
      BBoxCheckPoint(*i);
   }
}

//______________________________________________________________________________
void TEveJetCone::Paint(Option_t*)
{
   // Paint object.
   // This is for direct rendering (using TEveJetConeGL class).

   static const TEveException eh("TEveJetCone::Paint ");

   if (fRnrSelf == kFALSE) return;

   TBuffer3D buff(TBuffer3DTypes::kGeneric);

   // Section kCore
   buff.fID           = this;
   buff.fColor        = GetMainColor();
   buff.fTransparency = GetMainTransparency();
   if (HasMainTrans())
      RefMainTrans().SetBuffer3D(buff);
   buff.SetSectionsValid(TBuffer3D::kCore);

   Int_t reqSections = gPad->GetViewer3D()->AddObject(buff);
   if (reqSections != TBuffer3D::kNone)
      Error(eh, "only direct GL rendering supported.");
}

//______________________________________________________________________________
Int_t TEveJetCone::AddCone(Float_t eta, Float_t phi, Float_t coneRadius, Float_t height )
{
   // Add jet cone
   // parameters are :
   // * (eta,phi)    : of the center/leading particle
   // * coneRadius   : in eta-phi space
   // * height       : height of the cone
   //                  * if cylinder is set and length is adapted to cylinder.
   //                    - if height is given, it will be used as scalar factor
   //                  * if cylinder is not set, height is used as height of the cone
   // Return 0 on sucess

   if ( fCylinderBorder.fZ == -1. && fCylinderBorder.fX == -1. && height == -1 )
      return -1;

   TEveVector coneAxis;
   FillTEveVectorFromEtaPhi( coneAxis, eta, phi );

   Float_t angleRad = 0.;
   for ( Float_t angle = 0.; angle < 360. ; angle+=5. , angleRad=angle*TMath::Pi()/180 ) {

      // -- Get Contour point
      TEveVector contourPoint;
      FillTEveVectorFromEtaPhi( contourPoint,
                                eta + coneRadius * TMath::Cos(angleRad),
                                phi + coneRadius * TMath::Sin(angleRad) );

      // -- Set length of the contourPoint
      if ( fCylinderBorder.fZ != -1. && fCylinderBorder.fX != -1. ) {
         if ( contourPoint.Theta() < fThetaC )
            contourPoint *= fCylinderBorder.fZ / TMath::Cos( contourPoint.Theta() ) ;
         else if ( contourPoint.Theta() > ( TMath::Pi() - fThetaC ) )
            contourPoint *= fCylinderBorder.fZ / TMath::Cos( contourPoint.Theta() - TMath::Pi() ) ;
         else
            contourPoint *= fCylinderBorder.fX / TMath::Sin( contourPoint.Theta() ) ;

         if ( height != -1 ) contourPoint *= height;
      }
      else {
         contourPoint *= height / GetArcCosConeOpeningAngle( coneAxis, contourPoint );
      }

      // -- Add contourPoint
      fBasePoints.push_back( contourPoint );
   }

   return 0;
}

//______________________________________________________________________________
Int_t TEveJetCone::AddEllipticCone(Float_t eta, Float_t phi, Float_t reta, Float_t rphi, Float_t height)
{
   // Add jet cone
   // parameters are :
   // * (eta,phi)    : of the center/leading particle
   // * (reta, rphi) : radius of cone in eta-phi space
   // * height       : height of the cone
   //                  * if cylinder is set and length is adapted to cylinder.
   //                    - if height is given, it will be used as scalar factor
   //                  * if cylinder is not set, height is used as height of the cone
   // Return 0 on sucess

   if ( fCylinderBorder.fZ == -1. && fCylinderBorder.fX == -1. && height == -1 )
      return -1;

   TEveVector coneAxis;
   FillTEveVectorFromEtaPhi( coneAxis, eta, phi );

   Float_t angleRad = 0.;
   for ( Float_t angle = 0.; angle < 360. ; angle+=5. , angleRad=angle*TMath::Pi()/180 ) {

      // -- Get Contour point
      TEveVector contourPoint;
      FillTEveVectorFromEtaPhi( contourPoint,
                                eta + reta * TMath::Cos(angleRad),
                                phi + rphi * TMath::Sin(angleRad) );

      // -- Set length of the contourPoint
      if ( fCylinderBorder.fZ != -1. && fCylinderBorder.fX != -1. ) {
         if ( contourPoint.Theta() < fThetaC )
            contourPoint *= fCylinderBorder.fZ / TMath::Cos( contourPoint.Theta() ) ;
         else if ( contourPoint.Theta() > ( TMath::Pi() - fThetaC ) )
            contourPoint *= fCylinderBorder.fZ / TMath::Cos( contourPoint.Theta() - TMath::Pi() ) ;
         else
            contourPoint *= fCylinderBorder.fX / TMath::Sin( contourPoint.Theta() ) ;

         if ( height != -1 ) contourPoint *= height;
      }
      else {
         contourPoint *= height / GetArcCosConeOpeningAngle( coneAxis, contourPoint );
      }

      // -- Add contourPoint
      fBasePoints.push_back( contourPoint );
   }

   return 0;
}

//______________________________________________________________________________
void TEveJetCone::FillTEveVectorFromEtaPhi( TEveVector &vec, const Float_t& eta, const Float_t& phi )
{
   // Fill TEveVector with eta and phi, with magnitude 1.

   vec.Set( TMath::Cos(phi) / TMath::CosH(eta),
            TMath::Sin(phi) / TMath::CosH(eta),
            TMath::TanH(eta) );
   return;
}

//______________________________________________________________________________
Float_t TEveJetCone::GetArcCosConeOpeningAngle( const TEveVector& axis, const TEveVector& contour )
{
   // Return the arccos of the opening angle between two eve vectors

   Float_t arcCos = 0.;

   Float_t tot2 = axis.Mag2() * contour.Mag2();
   if( tot2 > 0. ) {
      arcCos = axis.Dot( contour )/ TMath::Sqrt( tot2 );
      if (arcCos >  1.0) arcCos =  1.0;
      if (arcCos < -1.0) arcCos = -1.0;
   }

   return arcCos;
}
