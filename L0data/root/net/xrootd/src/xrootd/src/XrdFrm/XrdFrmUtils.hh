#ifndef __FRMUTILS__HH
#define __FRMUTILS__HH
/******************************************************************************/
/*                                                                            */
/*                        X r d F r m U t i l s . h h                         */
/*                                                                            */
/* (c) 2009 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//          $Id: XrdFrmUtils.hh 28902 2009-06-11 12:36:21Z ganis $

#include <stdlib.h>
#include <time.h>

class XrdFrmUtils
{
public:

static       char  Ask(char dflt, const char *Msg1, const char *Msg2="",
                                  const char *Msg3="");

static       int   Utime(const char *Path, time_t tVal);

                   XrdFrmUtils() {}
                  ~XrdFrmUtils() {}
private:
};
#endif
