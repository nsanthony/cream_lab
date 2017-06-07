#ifndef CREAMSCD3_H
#define CREAMSCD3_H

#include <stdio.h>
#include "DEvent.h"

#define NSCD3       2944

class CreamSCD3 {

// Raw Data
  DEvent event ;
  unsigned NRaw ;
  unsigned Raw[NSCD3] ;

public:
  CreamSCD3();
  virtual ~CreamSCD3();

  int Init() ;

  int Run(DEvent ev) ;
  int ReadRaw() ;

  unsigned GetADC(unsigned i) { return Raw[i] ; }
} ;

#endif
