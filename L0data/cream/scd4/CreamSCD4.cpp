#include <iostream>
#include <stdlib.h>
#include "CreamSCD4.h"

using namespace std;

CreamSCD4::CreamSCD4()
	{
	CreamSCD4::Init();
}

CreamSCD4::~CreamSCD4()
	{
	// nothing to do
}

int CreamSCD4::Init()
{
  return 0 ;
}

int CreamSCD4::Run(DEvent ev)
{
  event = ev ;
  ReadRaw() ;
  return 0 ;
}

int CreamSCD4::ReadRaw()
{
  for(unsigned i=0; i<NSCD4; i++) {
    Raw[i] = 0 ;
  }
  unsigned i1 ;
  NRaw = event.SCDL2GetNumberOfChannel() ; 
  for(unsigned i=0; i<NRaw; i++) {
    i1      = event.SCDL2GetChannel(i) ;
    Raw[i1] = event.SCDL2GetADC(i) ;
  }
  return 0 ;
}
