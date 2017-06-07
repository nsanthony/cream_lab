#ifndef CREAMCAL_H
#define CREAMCAL_H

#include <stdio.h>
#include "DEvent.h"

#define NCAL       2560

class CreamCAL {

// Raw Data
  DEvent event ;
  unsigned NRaw ;
  unsigned Raw[NCAL] ;

public:
  CreamCAL();
  virtual ~CreamCAL();

  int Init() ;

  int Run(DEvent ev) ;
  int ReadRaw() ;

  unsigned GetADC(unsigned i) { return Raw[i] ; }
} ;

#endif
