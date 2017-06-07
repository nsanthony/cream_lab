#ifndef __CMS_NOTIFY_HH
#define __CMS_NOTIFY_HH
/******************************************************************************/
/*                                                                            */
/*                       X r d C m s N o t i f y . h h                        */
/*                                                                            */
/* (c) 2009 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//       $Id: XrdCmsNotify.hh 28902 2009-06-11 12:36:21Z ganis $

  
class XrdNetMsg;
class XrdSysError;

class XrdCmsNotify
{
public:

int  Gone(const char *Path, int isPfn=1);

int  Have(const char *Path, int isPfn=1);

static const int isServ = 0x0001;
static const int noPace = 0x0002;

             XrdCmsNotify(XrdSysError *erp, const char *aPath,
                                            const char *iName, int Opts=0);
            ~XrdCmsNotify();

private:
int  Send(const char *Buff, int Blen);

XrdSysError *eDest;
XrdNetMsg   *xMsg;
char        *destPath;
int          Pace;
};
#endif
