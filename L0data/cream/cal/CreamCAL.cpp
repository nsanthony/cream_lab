#include <iostream>
#include <stdlib.h>
#include "CreamCAL.h"

using namespace std;

CreamCAL::CreamCAL()
	{
	
}

CreamCAL::~CreamCAL()
	{
	// nothing to do
}

int CreamCAL::Run(DEvent ev)
{
  event = ev ;
  ReadRaw() ;
  return 0 ;
}

int CreamCAL::ReadRaw()
{
  for(unsigned i=0; i<NCAL; i++) {
    Raw[i] = 0 ;
  }
  unsigned i1 ;
  NRaw = event.CalorimeterGetNumberOfChannel() ; 
  for(unsigned i=0; i<NRaw; i++) {
    i1      = event.CalorimeterGetChannel(i) ;
    Raw[i1] = event.CalorimeterGetADC(i) ;
  }
  return 0 ;
}
