#include <iostream>
#include <stdlib.h>
#include "CreamSCD2.h"

using namespace std;

CreamSCD2::CreamSCD2()
	{
	CreamSCD2::Init();
}

CreamSCD2::~CreamSCD2()
	{
	// nothing to do
}

int CreamSCD2::Init()
{
  return 0 ;
}

int CreamSCD2::Run(DEvent ev)
{
  event = ev ;
  ReadRaw() ;
  return 0 ;
}

int CreamSCD2::ReadRaw()
{
  for(unsigned i=0; i<NSCD2; i++) {
    Raw[i] = 0 ;
  }
  unsigned i1 ;
  NRaw = event.BottomSCDGetNumberOfChannel() ; 
  for(unsigned i=0; i<NRaw; i++) {
    i1      = event.BottomSCDGetChannel(i) ;
    Raw[i1] = event.BottomSCDGetADC(i) ;
  }
  return 0 ;
}
