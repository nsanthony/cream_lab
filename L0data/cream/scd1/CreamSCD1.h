#ifndef CREAMSCD1_H
#define CREAMSCD1_H

#include <stdio.h>
#include "DEvent.h"

#define NSCD1       2944

class CreamSCD1 {

// Raw Data
  DEvent event ;
  unsigned NRaw ;
  unsigned Raw[NSCD1] ;

public:
  CreamSCD1();
  virtual ~CreamSCD1();

  int Init() ;

  int Run(DEvent ev) ;
  int ReadRaw() ;

  unsigned GetADC(unsigned i) { return Raw[i] ; }
} ;

#endif
