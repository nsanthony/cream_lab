/******************************************************************************/
/*                                                                            */
/*                        X r d O u c U t i l s . c c                         */
/*                                                                            */
/* (c) 2005 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//         $Id: XrdOucUtils.cc 30949 2009-11-02 16:37:58Z ganis $

const char *XrdOucUtilsCVSID = "$Id: XrdOucUtils.cc 30949 2009-11-02 16:37:58Z ganis $";

#include <ctype.h>
#include <errno.h>

#ifdef WIN32
#include <direct.h>
#include "XrdSys/XrdWin32.hh"
#endif
#include "XrdNet/XrdNetDNS.hh"
#include "XrdSys/XrdSysError.hh"
#include "XrdOuc/XrdOucStream.hh"
#include "XrdSys/XrdSysPlatform.hh"
#include "XrdOuc/XrdOucUtils.hh"
  
/******************************************************************************/
/*                                 e T e x t                                  */
/******************************************************************************/
  
// eText() returns the text associated with the error, making the first
// character in the text lower case. The text buffer pointer is returned.

char *XrdOucUtils::eText(int rc, char *eBuff, int eBlen, int AsIs)
{
   const char *etP;

// Get error text
//
   if (!(etP = strerror(rc)) || !(*etP)) etP = "reason unknown";

// Copy the text and lower case the first letter
//
   strlcpy(eBuff, etP, eBlen);
   if (!AsIs) *eBuff = tolower(*eBuff);

// All done
//
   return eBuff;
}

/******************************************************************************/
/*                                  d o I f                                   */
/******************************************************************************/
  
// doIf() parses "if [<hostlist>] [<altopt> [&& <altop> [ ... ]]]"
// altop: [exec <pgmlist> [&& named <namelist>]] | [named <namelist>]

// Returning 1 if true (i.e., this machine is one of the named hosts in hostlist 
// and is running one of the programs pgmlist and named by one of the names in 
// namelist).
// Return -1 (negative truth) if an error occured.
// Otherwise, returns false (0). Some combination of hostlist, pgm, and 
// namelist, must be specified.

int XrdOucUtils::doIf(XrdSysError *eDest, XrdOucStream &Config,
                      const char *what,  const char *hname,
                      const char *nname, const char *pname)
{
   static const char *brk[] = {"exec", "named", 0};
   char *val;
   int hostok;

// Make sure that at least one thing appears after the if
//
   if (!(val = Config.GetWord()))
      {if (eDest) eDest->Emsg("Config","Host name missing after 'if' in", what);
       return -1;
      }

// Check if we are one of the listed hosts
//
   if (!is1of(val, brk))
      {do {hostok = XrdNetDNS::isMatch(hname, val);
           val = Config.GetWord();
          } while(!hostok && val && !is1of(val, brk));
      if (hostok)
         { while(val && !is1of(val, brk)) val = Config.GetWord();
           // No more directives
           if (!val) return 1;
         } else return 0;
      }

// Check if we need to compare program names (we are here only if we either
// passed the hostlist test or there was no hostlist present)
//
   if (!strcmp(val, "exec"))
      {if (!(val = Config.GetWord()) || !strcmp(val, "&&"))
          {if (eDest)
              eDest->Emsg("Config","Program name missing after 'if exec' in",what);
              return -1;
          }

       // Check if we are one of the programs.
       //
       if (!pname) return 0;
       while(val && strcmp(val, pname))
            if (!strcmp(val, "&&")) return 0;
               else  val = Config.GetWord();
       if (!val) return 0;
       while(val && strcmp(val, "&&")) val = Config.GetWord();
       if (!val) return 1;

       if (!(val = Config.GetWord()))
          {if (eDest)
              eDest->Emsg("Config","Keyword missing after '&&' in",what);
              return -1;
          }
       if (strcmp(val, "named"))
          {if (eDest)
              eDest->Emsg("Config",val,"is invalid after '&&' in",what);
              return -1;
          }
      }

// Check if we need to compare net names (we are here only if we either
// passed the hostlist test or there was no hostlist present)
//
   if (!(val = Config.GetWord()))
      {if (eDest)
          eDest->Emsg("Config","Instance name missing after 'if named' in", what);
       return -1;
      }

// Check if we are one of the names
//
   if (!nname) return 0;
   while(val && strcmp(val, nname)) val = Config.GetWord();

// All done
//
   return (val != 0);
}

/******************************************************************************/
/*                               g e n P a t h                                */
/******************************************************************************/

char *XrdOucUtils::genPath(const char *p_path, const char *inst, 
                           const char *s_path)
{
   char buff[2048];
   int i = strlcpy(buff, p_path, sizeof(buff));

   if (buff[i-1] != '/') {buff[i++] = '/'; buff[i] = '\0';}
   if (inst) {strcpy(buff+i, inst); strcat(buff, "/");}
   if (s_path) strcat(buff, s_path);

   i = strlen(buff);
   if (buff[i-1] != '/') {buff[i++] = '/'; buff[i] = '\0';}

   return strdup(buff);
}

/******************************************************************************/
  
int XrdOucUtils::genPath(char *buff, int blen, const char *path, const char *psfx)
{
    int i, j;

    i = strlen(path);
    j = (psfx ? strlen(psfx) : 0);
    if (i+j+3 > blen) return -ENAMETOOLONG;

     strcpy(buff, path);
     if (psfx)
        {if (buff[i-1] != '/') buff[i++] = '/';
         strcpy(&buff[i], psfx);
         if (psfx[j-1] != '/') strcat(buff, "/");
        }
    return 0;
}

/******************************************************************************/
/*                                 i s 1 o f                                  */
/******************************************************************************/
  
int XrdOucUtils::is1of(char *val, const char **clist)
{
   int i = 0;
   while(clist[i]) if (!strcmp(val, clist[i])) return 1;
                      else i++;
   return 0;
}

/******************************************************************************/
/*                              m a k e H o m e                               */
/******************************************************************************/
  
void XrdOucUtils::makeHome(XrdSysError &eDest, const char *inst)
{
   char buff[1024];

   if (!inst || !getcwd(buff, sizeof(buff))) return;

   strcat(buff, "/"); strcat(buff, inst);
   if (MAKEDIR(buff, pathMode) && errno != EEXIST)
      {eDest.Emsg("Config", errno, "create home directory", buff);
       return;
      }

   if (chdir(buff) < 0)
      eDest.Emsg("Config", errno, "chdir to home directory", buff);
}

/******************************************************************************/
/*                              m a k e P a t h                               */
/******************************************************************************/
  
int XrdOucUtils::makePath(char *path, mode_t mode)
{
    char *next_path = path+1;
    struct stat buf;

// Typically, the path exists. So, do a quick check before launching into it
//
   if (!stat(path, &buf)) return 0;

// Start creating directories starting with the root
//
   while((next_path = index(next_path, int('/'))))
        {*next_path = '\0';
         if (MAKEDIR(path, mode))
            if (errno != EEXIST) return -errno;
         *next_path = '/';
         next_path = next_path+1;
        }

// All done
//
   return 0;
}
 
/******************************************************************************/
/*                              s u b L o g f n                               */
/******************************************************************************/
  
char *XrdOucUtils::subLogfn(XrdSysError &eDest, const char *inst, char *logfn)
{
   const mode_t lfm = S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
   char buff[2048], *sp;
   int rc;

   if (!inst || !*inst) return logfn;
   if (!(sp = rindex(logfn, '/'))) strcpy(buff, "./");
      else {*sp = '\0'; strcpy(buff, logfn); strcat(buff, "/");}

   strcat(buff, inst); strcat(buff, "/");

   if ((rc = XrdOucUtils::makePath(buff, lfm)))
      {eDest.Emsg("Config", rc, "create log file path", buff);
       return 0;
      }

   if (sp) {*sp = '/'; strcat(buff, sp+1);}
      else strcat(buff, logfn);

   free(logfn);
   return strdup(buff);
}
