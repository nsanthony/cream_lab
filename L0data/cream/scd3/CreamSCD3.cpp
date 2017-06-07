#include <iostream>
#include <stdlib.h>
#include "CreamSCD3.h"

using namespace std;

CreamSCD3::CreamSCD3()
	{
	CreamSCD3::Init();
}

CreamSCD3::~CreamSCD3()
	{
	// nothing to do
}

int CreamSCD3::Init()
{
  return 0 ;
}

int CreamSCD3::Run(DEvent ev)
{
  event = ev ;
  ReadRaw() ;
  return 0 ;
}

int CreamSCD3::ReadRaw()
{
  for(unsigned i=0; i<NSCD3; i++) {
    Raw[i] = 0 ;
  }
  unsigned i1 ;
  NRaw = event.SCDL1GetNumberOfChannel() ; 
  for(unsigned i=0; i<NRaw; i++) {
    i1      = event.SCDL1GetChannel(i) ;
    Raw[i1] = event.SCDL1GetADC(i) ;
  }
  return 0 ;
}
