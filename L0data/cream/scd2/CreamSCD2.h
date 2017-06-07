#ifndef CREAMSCD2_H
#define CREAMSCD2_H

#include <stdio.h>
#include "DEvent.h"

#define NSCD2       2944

class CreamSCD2 {

// Raw Data
  DEvent event ;
  unsigned NRaw ;
  unsigned Raw[NSCD2] ;

public:
  CreamSCD2();
  virtual ~CreamSCD2();

  int Init() ;

  int Run(DEvent ev) ;
  int ReadRaw() ;

  unsigned GetADC(unsigned i) { return Raw[i] ; }
} ;

#endif
