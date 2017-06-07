#include <iostream>
#include <stdlib.h>
#include "CreamSCD1.h"

using namespace std;

CreamSCD1::CreamSCD1()
	{
	CreamSCD1::Init();
}

CreamSCD1::~CreamSCD1()
	{
	// nothing to do
}

int CreamSCD1::Init()
{
  return 0 ;
}

int CreamSCD1::Run(DEvent ev)
{
  event = ev ;
  ReadRaw() ;
  return 0 ;
}

int CreamSCD1::ReadRaw()
{
  for(unsigned i=0; i<NSCD1; i++) {
    Raw[i] = 0 ;
  }
  unsigned i1 ;
  NRaw = event.TopSCDGetNumberOfChannel() ; 
  for(unsigned i=0; i<NRaw; i++) {
    i1      = event.TopSCDGetChannel(i) ;
    Raw[i1] = event.TopSCDGetADC(i) ;
  }
  return 0 ;
}
