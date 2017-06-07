#ifndef CREAMBSD_H
#define CREAMBSD_H

#include "DEvent.h"

#define NBSD       36

class CreamBSD {
 
// Raw Data
  DEvent event ;
  unsigned NRaw ;
  unsigned Raw[NBSD] ;

        unsigned        fevent_number;
        unsigned        fevent_count;
        unsigned        ftrig_mode;
        unsigned        fconf_mode;
        unsigned        fsync;
        unsigned        ftrig;

public:
  CreamBSD();
  virtual ~CreamBSD();

  int Init() ;

  int Run(DEvent ev) ;
  int ReadRaw() ;

   unsigned GetADC(unsigned i) { return Raw[i] ; }
   unsigned  GetEventNum() {return fevent_number; }
   unsigned  GetEventCnt() {return fevent_count; }
   unsigned  GetTriMode() {return ftrig_mode; }
   unsigned  GetConfMode() {return fconf_mode; }
   unsigned  GetSyncNum() {return fsync; }
   unsigned  GetTriStatus() {return ftrig; }
} ;

#endif
