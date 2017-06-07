#ifndef CREAMSCD4_H
#define CREAMSCD4_H

#include <stdio.h>
#include "DEvent.h"

#define NSCD4       2944

class CreamSCD4 {

// Raw Data
  DEvent event ;
  unsigned NRaw ;
  unsigned Raw[NSCD4] ;

public:
  CreamSCD4();
  virtual ~CreamSCD4();

  int Init() ;

  int Run(DEvent ev) ;
  int ReadRaw() ;

  unsigned GetADC(unsigned i) { return Raw[i] ; }
} ;

#endif
