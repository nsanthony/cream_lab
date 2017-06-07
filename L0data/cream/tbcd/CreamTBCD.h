#ifndef CREAMTBCD_H
#define CREAMTBCD_H

#include <stdio.h>
#include "DEvent.h"

#define NTBCD       1280

class CreamTBCD {

// Raw Data
  DEvent event ;
  unsigned NRaw ;
  unsigned Raw[NTBCD] ;

public:
  CreamTBCD();
  virtual ~CreamTBCD();

  int Init() ;
  int ReadGeometry() ;
  int ReadCalibration() ;
  int ReadPedestal() ;

  int Run(DEvent ev) ;
  int ReadRaw() ;

   unsigned GetADC(unsigned i) { return Raw[i] ; }
} ;

#endif
