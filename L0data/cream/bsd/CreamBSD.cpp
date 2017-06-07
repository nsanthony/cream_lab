#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include "CreamBSD.h"

using namespace std;

CreamBSD::CreamBSD()
	{
	CreamBSD::Init();
}

CreamBSD::~CreamBSD()
	{
	// nothing to do
}

int CreamBSD::Init()
{
  return 0 ;
}

int CreamBSD::Run(DEvent ev)
{
  event = ev ;
  ReadRaw() ;
  return 0 ;
}

int CreamBSD::ReadRaw()
{
  for(unsigned i=0; i<NBSD; i++) {
    Raw[i] = 0 ;
  }
  unsigned i1 ;
  NRaw = event.BSDGetNumberOfChannel() ;

        fevent_number = event.BSDGetEventNum() ;
        fevent_count = event.BSDGetEventCnt()  & 0xff;
        ftrig_mode = event.BSDGetTriMode()  & 0xff;
        fconf_mode = event.BSDGetConfMode()  & 0xff;
        fsync = event.BSDGetSyncNum() ;
        ftrig = event.BSDGetTriStatus() ;
 
  for(unsigned i=0; i<NRaw; i++) {
    i1      = event.BSDGetChannel(i) ;
    Raw[i1] = event.BSDGetADC(i) ;
  }
  return 0 ;
}
