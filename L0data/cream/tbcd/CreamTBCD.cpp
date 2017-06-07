#include <iostream>
#include <stdlib.h>
#include "CreamTBCD.h"

using namespace std;

CreamTBCD::CreamTBCD()
	{
	CreamTBCD::Init();
}

CreamTBCD::~CreamTBCD()
	{
	// nothing to do
}

int CreamTBCD::Init()
{
  return 0 ;
}

int CreamTBCD::Run(DEvent ev)
{
  event = ev ;
  ReadRaw() ;
  return 0 ;
}

int CreamTBCD::ReadRaw()
{
  for(unsigned i=0; i<NTBCD; i++) {
    Raw[i] = 0 ;
  }
  unsigned i1 ;
  NRaw = event.TCRDGetNumberOfChannel() ; 
  for(unsigned i=0; i<NRaw; i++) {
    i1      = event.TCRDGetChannel(i) ;
    Raw[i1] = event.TCRDGetADC(i) ;
  }
  return 0 ;
}
