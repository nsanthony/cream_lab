// @(#)root/geom:$Id: TVirtualMagField.h 27106 2009-01-09 07:55:28Z brun $

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TVirtualMagField
#define ROOT_TVirtualMagField

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// TVirtualMagField - ABC for magnetic field. Derived classes must        //
// implement the method: Field(const Double_t *x, Double_t *B)            //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

class TVirtualMagField : public TNamed
{
public:
   TVirtualMagField()                 : TNamed() {}
   TVirtualMagField(const char *name) : TNamed(name,"") {}
   virtual ~TVirtualMagField();
   
   virtual void Field(const Double_t *x, Double_t *B) = 0;
   
   ClassDef(TVirtualMagField, 1)              // Abstract base field class
};


////////////////////////////////////////////////////////////////////////////
//                                                                        //
// TGeoUniformMagField - Uniform magnetic field class.                       //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

class TGeoUniformMagField : public TVirtualMagField
{
private:
   Double_t                fB[3]; // Magnetic field vector

protected:
   TGeoUniformMagField(const TGeoUniformMagField&);
   TGeoUniformMagField& operator=(const TGeoUniformMagField&);
   
public:
   TGeoUniformMagField();
   TGeoUniformMagField(Double_t Bx, Double_t By, Double_t Bz);
   virtual ~TGeoUniformMagField() {}
   
   void            Field(const Double_t * /*x*/, Double_t *B) {B[0]=fB[0]; B[1]=fB[1]; B[2]=fB[2];}

   const Double_t *GetFieldValue() const { return &fB[0]; }
   void            SetFieldValue(Double_t Bx, Double_t By, Double_t Bz) {fB[0]=Bx; fB[1]=By; fB[2]=Bz;}
   
   ClassDef(TGeoUniformMagField, 1)  // Uniform magnetic field        
};

#endif
