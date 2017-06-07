/******************************************************************************/
/*                                                                            */
/*                       X r d O l b C o n f i g . c c                        */
/*                                                                            */
/* (C) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*                DE-AC03-76-SFO0515 with the Deprtment of Energy             */
/******************************************************************************/

//         $Id: XrdOlbConfig.cc 29874 2009-08-21 16:56:04Z ganis $

const char *XrdOlbConfigCVSID = "$Id: XrdOlbConfig.cc 29874 2009-08-21 16:56:04Z ganis $";

/*
   The routines in this file handle olb() initialization. They get the
   configuration values either from configuration file or XrdOlbconfig.h (in that
   order of precedence).

   These routines are thread-safe if compiled with:
   AIX: -D_THREAD_SAFE
   SUN: -D_REENTRANT
*/
  
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <strings.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <dirent.h>

#include "../XrdVersion.hh"
#include "Xrd/XrdJob.hh"
#include "Xrd/XrdScheduler.hh"
#include "XrdOlb/XrdOlbAdmin.hh"
#include "XrdOlb/XrdOlbCache.hh"
#include "XrdOlb/XrdOlbConfig.hh"
#include "XrdOlb/XrdOlbMeter.hh"
#include "XrdOlb/XrdOlbManager.hh"
#include "XrdOlb/XrdOlbManTree.hh"
#include "XrdOlb/XrdOlbPrepare.hh"
#include "XrdOlb/XrdOlbRRQ.hh"
#include "XrdOlb/XrdOlbServer.hh"
#include "XrdOlb/XrdOlbState.hh"
#include "XrdOlb/XrdOlbTrace.hh"
#include "XrdOlb/XrdOlbTypes.hh"
#include "XrdOlb/XrdOlbXmi.hh"
#include "XrdOlb/XrdOlbXmiReq.hh"
#include "XrdNet/XrdNetDNS.hh"
#include "XrdNet/XrdNetLink.hh"
#include "XrdNet/XrdNetWork.hh"
#include "XrdNet/XrdNetSecurity.hh"
#include "XrdNet/XrdNetSocket.hh"
#include "XrdOuc/XrdOuca2x.hh"
#include "XrdOuc/XrdOucEnv.hh"
#include "XrdSys/XrdSysError.hh"
#include "XrdOuc/XrdOucExport.hh"
#include "XrdOuc/XrdOucName2Name.hh"
#include "XrdOuc/XrdOucProg.hh"
#include "XrdOuc/XrdOucTList.hh"
#include "XrdOuc/XrdOucUtils.hh"
#include "XrdSys/XrdSysHeaders.hh"
#include "XrdSys/XrdSysPlatform.hh"
#include "XrdSys/XrdSysPlugin.hh"
#include "XrdSys/XrdSysPthread.hh"
#include "XrdOuc/XrdOucStream.hh"
#include "XrdSys/XrdSysTimer.hh"

using namespace XrdOlb;

/******************************************************************************/
/*                  C o m m a n d   L i n e   O p t i o n s                   */
/******************************************************************************/
/*
   olbd [options] [configfn]

   options: [xopt] [-i] [-m] [-s] [-w]

Where:
    xopt  Are Xrd processed options (some of which we use).

   -i     Immediate start-up (do not wait for a server connection).

   -m     function in manager moede.

   -s     Executes in server  mode.
*/

/******************************************************************************/
/*           G l o b a l   C o n f i g u r a t i o n   O b j e c t            */
/******************************************************************************/

      XrdScheduler    *XrdOlb::Sched = 0;
      XrdOlbConfig     XrdOlb::Config;
      XrdOlbXmi       *XrdOlb::Xmi_Chmod  = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Load   = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Mkdir  = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Mkpath = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Prep   = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Rename = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Remdir = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Remove = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Select = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Space  = 0;
      XrdOlbXmi       *XrdOlb::Xmi_Stat   = 0;

/******************************************************************************/
/*            E x t e r n a l   T h r e a d   I n t e r f a c e s             */
/******************************************************************************/
  
void *XrdOlbStartMonPing(void *carg) { return Manager.MonPing(); }

void *XrdOlbStartMonPerf(void *carg) { return Manager.MonPerf(); }

void *XrdOlbStartMonRefs(void *carg) { return Manager.MonRefs(); }

void *XrdOlbStartMonStat(void *carg) { return OlbState.Monitor(); }

void *XrdOlbStartAdmin(void *carg)
      {XrdOlbAdmin Admin;
       return Admin.Start((XrdNetSocket *)carg);
      }

void *XrdOlbStartAnote(void *carg)
      {XrdOlbAdmin Anote;
       return Anote.Notes((XrdNetSocket *)carg);
      }

void *XrdOlbStartPandering(void *carg)
      {XrdOucTList *tp = (XrdOucTList *)carg;
       return Manager.Pander(tp->text, tp->val);
      }

void *XrdOlbStartPreparing(void *carg)
      {XrdOlbServer::Resume(0);
       return (void *)0;
      }

void *XrdOlbStartSupervising(void *carg)
      {EPNAME("StartSuper");
       XrdNetWork *NetTCPr = (XrdNetWork *)carg;
       XrdNetLink *newlink;
       while(1) if ((newlink = NetTCPr->Accept(XRDNET_NODNTRIM)))
                   {DEBUG("FD " <<newlink->FDnum() <<" connected to " <<newlink->Nick());
                    Manager.Login(newlink);
                   }
       return (void *)0;
      }

/******************************************************************************/
/*                               d e f i n e s                                */
/******************************************************************************/

#define TS_String(x,m) if (!strcmp(x,var)) {free(m); m = strdup(val); return 0;}

#define TS_Xeq(x,m)    if (!strcmp(x,var)) return m(eDest, CFile);

#define TS_Set(x,v)    if (!strcmp(x,var)) {v=1; CFile.Echo(); return 0;}

#define TS_unSet(x,v)  if (!strcmp(x,var)) {v=0; CFile.Echo(); return 0;}

/******************************************************************************/
/*                            C o n f i g u r e 1                             */
/******************************************************************************/
  
int XrdOlbConfig::Configure1(int argc, char **argv, char *cfn)
{
/*
  Function: Establish phase 1 configuration at start up time.

  Input:    argc - argument count
            argv - argument vector
            cfn  - optional configuration file name

  Output:   0 upon success or !0 otherwise.
*/
   int NoGo = 0, immed = 0;
   char c, buff[512];
   extern int opterr, optopt;

// Prohibit this program from executing as superuser
//
   if (geteuid() == 0)
      {Say.Emsg("Config", "Security reasons prohibit olbd running as "
                  "superuser; olbd is terminating.");
       _exit(8);
      }

// Process the options
//
   opterr = 0; optind = 1;
   if (argc > 1 && '-' == *argv[1]) 
      while ((c=getopt(argc,argv,"imsw")) && ((unsigned char)c != 0xff))
     { switch(c)
       {
       case 'i': immed = 1;
                 break;
       case 'm': isManager = 1;
                 break;
       case 's': isServer = 1;
                 break;
       case 'w': immed = -1;   // Backward compatability only
                 break;
       default:  buff[0] = '-'; buff[1] = optopt; buff[2] = '\0';
                 Say.Say("Config warning: unrecognized option,",buff,", ignored.");
       }
     }

// Bail if no configuration file specified
//
   inArgv = argv; inArgc = argc;
   if ((!(ConfigFN = cfn) && !(ConfigFN = getenv("XrdOlbCONFIGFN"))) || !*ConfigFN)
      {Say.Emsg("Config", "Required config file not specified.");
       Usage(1);
      }

// Establish my instance name
//
   sprintf(buff, "%s@%s", (myInsName ? myInsName : "anon"), myName);
   myInstance = strdup(buff);

// Print herald
//
   Say.Say("++++++ ", myInstance, " phase 1 initialization started.");

// If we don't know our role yet then we must find out before processing the
// config file. This means a double scan, sigh.
//
   if (!(isManager || isServer)) 
      if (!(NoGo |= ConfigProc(1)) && !(isManager || isServer))
         {Say.Say("Config warning: role not specified; manager role assumed.");
          isManager = -1;
         }

// Process the configuration file
//
   if (!NoGo) NoGo |= ConfigProc();

// Override the wait/nowait from the command line
//
   if (immed) doWait = (immed > 0 ? 0 : 1);

// Determine the role
//
   if (isManager < 0) isManager = 1;
   if (isPeer    < 0) isPeer    = 1;
   if (isProxy   < 0) isProxy   = 1;
   if (isServer  < 0) isServer  = 1;

// Create a text description of our role for use in messages
//
   if (!myRole)
      {      if (isPeer)                myRole = strdup("peer");
        else if (isProxy)               myRole = strdup("proxy");
        else if (isManager && isServer) myRole = strdup("Supervisor");
        else if (isManager)             myRole = strdup("manager");
        else                            myRole = strdup("server");
      }

// For managers, make sure that we have a well designated port.
// For servers or supervisors, force an ephemeral port to be used.
//
   if (!NoGo)
      {if ((isManager && !isServer) || isPeer)
          {if (PortTCP < 0)
              {Say.Emsg("Config", myRole, "port not specified."); NoGo = 1;}
          }
          else PortTCP = 0;
      }

// Determine how we ended and return status
//
   sprintf(buff, " phase 1 %s initialization %s.", myRole,
                (NoGo ? "failed" : "completed"));
   Say.Say("------ ", myInstance, buff);
   return NoGo;
}

/******************************************************************************/
/*                            C o n f i g u r e 2                             */
/******************************************************************************/
  
int XrdOlbConfig::Configure2()
{
/*
  Function: Establish phase 2 configuration at start up time.

  Input:    None.

  Output:   0 upon success or !0 otherwise.
*/
   int NoGo = 0;
   char *p, buff[512];

// Print herald
//
   sprintf(buff, " phase 2 %s initialization started.", myRole);
   Say.Say("++++++ ", myInstance, buff);

// Determine who we are. If we are a manager or supervisor start the file
// location cache scrubber.
//
   if (isManager) 
      {XrdJob *jp=(XrdJob *)new XrdOlbCache_Scrubber(&Cache, Sched);
       Sched->Schedule(jp, cachelife+time(0));
      }

// Establish the path to be used for admin functions
//
   p = XrdOucUtils::genPath(AdminPath,(strcmp("anon",myInsName)?myInsName:0),".olb");
   free(AdminPath);
   AdminPath = p;

// Setup the admin path (used in all roles)
//
   if (!NoGo) NoGo = !(AdminSock = XrdNetSocket::Create(&Say, AdminPath,
                     (isManager|isPeer ? "olbd.nimda":"olbd.admin"),AdminMode));

// Develop a stable unique identifier for this olbd independent of the port
//
   if (!NoGo)
      {char sidbuf[1024];
       sprintf(sidbuf, "%s%c", AdminPath, (isManager ? 'm' : 's'));
       mySID = strdup(sidbuf);
      }

// If we need a name library, load it now
//
   if ((LocalRoot || RemotRoot) && ConfigN2N()) NoGo = 1;

// Setup manager or server, as needed
//
  if (!NoGo && isManager) NoGo = setupManager();
  if (!NoGo && isServer)  NoGo = setupServer();

// If we are a solo peer then we have no servers and a lot of space
//
   if (isPeer && isSolo) 
      {SUPCount = 0; 
       SUPLevel = 0; 
       XrdOlbServer::setSpace(0x7fffffff, 0);
      }

// Set up the generic message id
//
   MsgGIDL = sprintf(buff, "%d@0 ", MsgTTL);
   MsgGID  = strdup(buff);

// Create the pid file
//
   if (!NoGo) NoGo |= PidFile();

// Load the XMI plugin
//
   if (!NoGo && XmiPath) NoGo = setupXmi();

// All done, check for success or failure
//
   sprintf(buff, " phase 2 %s initialization %s.", myRole,
                 (NoGo ? "failed." : "completed."));
   Say.Say("------ ", myInstance, buff);

// The remainder of the configuration needs to be run in a separate thread
//
   if (!NoGo) Sched->Schedule((XrdJob *)this);

// All done
//
   return NoGo;
}

/******************************************************************************/
/*                             C o n f i g X e q                              */
/******************************************************************************/

int XrdOlbConfig::ConfigXeq(char *var, XrdOucStream &CFile, XrdSysError *eDest)
{
   int dynamic;

   // Determine whether is is dynamic or not
   //
   if (eDest) dynamic = 1;
      else   {dynamic = 0; eDest = &Say;}

   // Process items
   //
   TS_Xeq("delay",         xdelay);  // Manager,     dynamic
   TS_Xeq("fxhold",        xfxhld);  // Manager,     dynamic
   TS_Xeq("ping",          xping);   // Manager,     dynamic
   TS_Xeq("sched",         xsched);  // Any,         dynamic
   TS_Xeq("space",         xspace);  // Any,        dynamic
   TS_Xeq("trace",         xtrace);  // Any,        dynamic

   if (!dynamic)
   {
   TS_Xeq("adminpath",     xapath);  // Any,     non-dynamic
   TS_Xeq("allow",         xallow);  // Manager, non-dynamic
   TS_Xeq("cache",         xcache);  // Server,  non-dynamic
   TS_Xeq("defaults",      xdefs);   // Server,  non-dynamic
   TS_Xeq("export",        xexpo);   // Server,  non-dynamic
   TS_Xeq("fsxeq",         xfsxq);   // Server,  non-dynamic
   TS_Xeq("localroot",     xlclrt);  // Any,     non-dynamic
   TS_Xeq("manager",       xmang);   // Server,  non-dynamic
   TS_Xeq("namelib",       xnml);    // Server,  non-dynamic
   TS_Xeq("path",          xpath);   // Server,  non-dynamic
   TS_Xeq("perf",          xperf);   // Server,  non-dynamic
   TS_Xeq("pidpath",       xpidf);   // Any,     non-dynamic
   TS_Xeq("port",          xport);   // Any,     non-dynamic
   TS_Xeq("prep",          xprep);   // Any,     non-dynamic
   TS_Xeq("prepmsg",       xprepm);  // Any,     non-dynamic
   TS_Xeq("remoteroot",    xrmtrt);  // Any,     non-dynamic
   TS_Xeq("role",          xrole);   // Server,  non-dynamic
   TS_Xeq("subscribe",     xsubs);   // Server,  non-dynamic (deprecated)
   TS_Set("wait",          doWait);  // Server,  non-dynamic (backward compat)
   TS_unSet("nowait",      doWait);  // Server,  non-dynamic
   TS_Xeq("xmilib",        xxmi);    // Any,     non-dynamic
   }

   // No match found, complain.
   //
   eDest->Say("Config warning: ignoring unknown directive '", var, "'.");
   CFile.Echo();
   return 0;
}

/******************************************************************************/
/*                                  D o I t                                   */
/******************************************************************************/
  
void XrdOlbConfig::DoIt()
{
   XrdSysSemaphore SyncUp(0);
   XrdOucTList    *tp;
   pthread_t       tid;
   time_t          eTime = time(0);
   int             wTime;

// Start the notification thread if we need to
//
   if (AnoteSock)
      if (XrdSysThread::Run(&tid, XrdOlbStartAnote, (void *)AnoteSock,
                            0, "Notification handler"))
         Say.Emsg("olbd", errno, "start notification handler");

// Start the prepare handler
//
   if (XrdSysThread::Run(&tid,XrdOlbStartPreparing,
                             (void *)0, 0, "Prep handler"))
      Say.Emsg("olbd", errno, "start prep handler");

// Start the admin thread if we need to, we will not continue until told
// to do so by the admin interface.
//
   if (AdminSock)
      {XrdOlbAdmin::setSync(&SyncUp);
       if (XrdSysThread::Run(&tid, XrdOlbStartAdmin, (void *)AdminSock,
                             0, "Admin traffic"))
          Say.Emsg("olbd", errno, "start admin handler");
       SyncUp.Wait();
      }

// Start the supervisor subsystem
//
   if (NetTCPr)
      {if (XrdSysThread::Run(&tid,XrdOlbStartSupervising, 
                             (void *)NetTCPr, 0, "supervisor"))
          {Say.Emsg("olbd", errno, "start", myRole);
          return;
          }
      }

// For solo peers we do not run disabled
//
   if (isPeer && isSolo) Disabled = 0;

// Start the server subsystem
//
   if (isServer || isPeer)
      {tp = myManagers;
       while(tp)
            {if (!isManager && !tp->next) Manager.Pander(tp->text, tp->val);
                else {if (XrdSysThread::Run(&tid,XrdOlbStartPandering,(void *)tp,
                                            0, tp->text))
                         {Say.Emsg("olbd", errno, "start", myRole);
                          return;
                         }
                      }
             tp = tp->next;
            }
      }

// If we are a manager then we must do a service enable after a service delay
//
   if (isManager || isPeer)
      {wTime = SRVDelay - static_cast<int>((time(0) - eTime));
       if (wTime > 0) XrdSysTimer::Wait(wTime*1000);
       Disabled = 0;
       Say.Emsg("Config", myRole, "service enabled.");
      }
}

/******************************************************************************/
/*                          G e n L o c a l P a t h                           */
/******************************************************************************/
  
/* GenLocalPath() generates the path that a file will have in the local file
   system. The decision is made based on the user-given path (typically what 
   the user thinks is the local file system path). The output buffer where the 
   new path is placed must be at least XrdOlbMAX_PATH_LEN bytes long.
*/
int XrdOlbConfig::GenLocalPath(const char *oldp, char *newp)
{
    if (lcl_N2N) return -(lcl_N2N->lfn2pfn(oldp, newp, XrdOlbMAX_PATH_LEN));
    if (strlen(oldp) >= XrdOlbMAX_PATH_LEN) return -ENAMETOOLONG;
    strcpy(newp, oldp);
    return 0;
}

/******************************************************************************/
/*                              G e n M s g I D                               */
/******************************************************************************/

int XrdOlbConfig::GenMsgID(char *oldmid, char *buff, int blen)
{
   char *ep;
   int msgnum, midlen;

// Find the id separator, if none, allow the message to be forwarded only 
// one additional time (compatability feature)
//
   msgnum = strtol(oldmid, &ep, 10);
   if (*ep != '@') {msgnum = 1; ep = oldmid;}
      else if (msgnum <= 1) return 0;
              else {msgnum--; ep++;}

// Format new msgid
//
   midlen = snprintf(buff, blen, "%d@%s ", msgnum, ep);
   if (midlen < 0 || midlen >= blen) return 0;
   return midlen;
}
  
/******************************************************************************/
/*                             i n N o S t a g e                              */
/******************************************************************************/

int  XrdOlbConfig::inNoStage()
{
   struct stat buff;

   return (!stat(NoStageFile, &buff));
}

/******************************************************************************/
/*                             i n S u s p e n d                              */
/******************************************************************************/
  
int  XrdOlbConfig::inSuspend()
{
   struct stat buff;

   return (!stat(SuspendFile, &buff));
}
  
/******************************************************************************/
/*                     P r i v a t e   F u n c t i o n s                      */
/******************************************************************************/
/******************************************************************************/
/*                        C o n f i g D e f a u l t s                         */
/******************************************************************************/

void XrdOlbConfig::ConfigDefaults(void)
{

// Preset all variables with common defaults
//
   myName   = (char *)"localhost"; // Correctly set in Configure()
   myDomain = 0;
   LUPDelay = 5;
   LUPHold  = 133;
   DRPDelay = 10*60;
   PSDelay  = 0;
   SRVDelay = 90;
   SUPCount = 1;
   SUPLevel = 80;
   SUPDelay = 15;
   SUSDelay = 30;
   MaxLoad  = 0x7fffffff;
   MsgTTL   = 7;
   PortTCP  = 0;
   P_cpu    = 0;
   P_fuzz   = 20;
   P_io     = 0;
   P_load   = 0;
   P_mem    = 0;
   P_pag    = 0;
   AskPerf  = 10;         // Every 10 pings
   AskPing  = 60;         // Every  1 minute
   MaxDelay = -1;
   LogPerf  = 10;         // Every 10 usage requests
   DiskMin  = 10485760LL; // 10737418240/1024 10GB (Min partition space) in KB
   DiskHWM  = 11534336LL; // 11811160064/1024 11GB (High Water Mark SUO) in KB
   DiskAsk  = 12;         // 15 Seconds between space calibrations.
   DiskWT   = 0;          // Do not defer when out of space
   DiskSS   = 0;          // Not a staging server
   PrepOK   = 0;
   ConfigFN = 0;
   sched_RR = 0;
   isManager= 0;
   isPeer   = 0;
   isSolo   = 0;
   isProxy  = 0;
   isServer = 0;
   N2N_Lib  = 0;
   N2N_Parms= 0;
   lcl_N2N  = 0;
   xeq_N2N  = 0;
   LocalRoot= 0;
   RemotRoot= 0;
   myInsName= 0;
   myManagers=0;
   myRole    =0;
   ManList   =0;
   mySID    = 0;
   perfint  = 3*60;
   perfpgm  = 0;
   AdminPath= strdup("/tmp/");
   AdminMode= 0700;
   AdminSock= 0;
   AnoteSock= 0;
   RedirSock= 0;
   pidPath  = strdup("/tmp");
   Police   = 0;
   monPath  = 0;
   monPathP = 0;
   cachelife= 8*60*60;
   pendplife=   60*60*24*7;
   DiskLinger=0;
   ProgCH   = 0;
   ProgMD   = 0;
   ProgMV   = 0;
   ProgRD   = 0;
   ProgRM   = 0;
   doWait   = 1;
   Disabled = 1;
   RefReset = 60*60;
   RefTurn  = 3*XrdOlbManager::STMax*(DiskLinger+1);
   NoStageFile = 0;
   SuspendFile = 0;
   NetTCPr     = 0;
   XmiPath     = 0;
   XmiParms    = 0;
   DirFlags    = 0;
}
  
/******************************************************************************/
/*                             C o n f i g N 2 N                              */
/******************************************************************************/

int XrdOlbConfig::ConfigN2N()
{
   XrdSysPlugin    *myLib;
   XrdOucName2Name *(*ep)(XrdOucgetName2NameArgs);

// If we have no library path then use the default method (this will always
// succeed).
//
   if (!N2N_Lib)
      {if (LocalRoot || (RemotRoot && XmiPath))
          {xeq_N2N = XrdOucgetName2Name(&Say,ConfigFN,"",LocalRoot,RemotRoot);
           if (LocalRoot) lcl_N2N = xeq_N2N;
          }
       PrepQ.setParms(xeq_N2N);
       return 0;
      }

// Create a pluin object (we will throw this away without deletion because
// the library must stay open but we never want to reference it again).
//
   if (!(myLib = new XrdSysPlugin(&Say, N2N_Lib))) return 1;

// Now get the entry point of the object creator
//
   ep = (XrdOucName2Name *(*)(XrdOucgetName2NameArgs))(myLib->getPlugin("XrdOucgetName2Name"));
   if (!ep) return 1;

// Get the Object now
//
   lcl_N2N = ep(&Say,ConfigFN,(N2N_Parms ? N2N_Parms : ""),LocalRoot,RemotRoot);
   PrepQ.setParms(lcl_N2N);
   return lcl_N2N == 0;
}

/******************************************************************************/
/*                            C o n f i g P r o c                             */
/******************************************************************************/
  
int XrdOlbConfig::ConfigProc(int getrole)
{
  char *var;
  int  cfgFD, retc, NoGo = 0;
  XrdOucEnv myEnv;
  XrdOucStream CFile(&Say, myInstance, &myEnv, "=====> ");

// Try to open the configuration file.
//
   if ( (cfgFD = open(ConfigFN, O_RDONLY, 0)) < 0)
      {Say.Emsg("Config", errno, "open config file", ConfigFN);
       return 1;
      }
   CFile.Attach(cfgFD);

// Turn off echoing if we are doing a pre-scan
//
   if (getrole) CFile.SetEroute(0);

// Now start reading records until eof.
//
   while((var = CFile.GetMyFirstWord()))
        if (getrole)
           {if (!strcmp("olb.role", var) || !strcmp("all.role", var))
               if (xrole(&Say, CFile))
                  {CFile.SetEroute(&Say); CFile.Echo(); NoGo = 1;
                   CFile.SetEroute(0);
                  }
           }
           else if (!strncmp(var, "olb.", 4)
                ||  !strcmp(var, "oss.cache")
                ||  !strcmp(var, "oss.defaults")
                ||  !strcmp(var, "oss.localroot")
                ||  !strcmp(var, "oss.remoteroot")
                ||  !strcmp(var, "oss.namelib")
                ||  !strcmp(var, "all.adminpath")
                ||  !strcmp(var, "all.export")
                ||  !strcmp(var, "all.manager")
                ||  !strcmp(var, "all.role"))
                   {if (ConfigXeq(var+4, CFile, 0)) {CFile.Echo(); NoGo = 1;}}
                   else if (!strcmp(var, "oss.stagecmd")) DiskSS |= 2;

// Now check if any errors occured during file i/o
//
   if ((retc = CFile.LastError()))
      NoGo = Say.Emsg("Config", retc, "read config file", ConfigFN);
   CFile.Close();

// Merge Paths as needed
//
   if (!getrole && isServer) NoGo |= MergeP();

// Return final return code
//
   return NoGo;
}
 
/******************************************************************************/
/*                                i s E x e c                                 */
/******************************************************************************/
  
int XrdOlbConfig::isExec(XrdSysError *eDest, const char *ptype, char *prog)
{
  char buff[512], pp, *mp = prog;

// Isolate the program name
//
   while(*mp && *mp != ' ') mp++;
   pp = *mp; *mp ='\0';

// Make sure the program is executable by us
//
   if (access(prog, X_OK))
      {sprintf(buff, "find %s execuatble", ptype);
       eDest->Emsg("Config", errno, buff, prog);
       *mp = pp;
       return 0;
      }

// All is well
//
   *mp = pp;
   return 1;
}

/******************************************************************************/
/*                                M e r g e P                                 */
/******************************************************************************/
  
int XrdOlbConfig::MergeP()
{
   XrdOucPList *plp = PexpList.First();
   XrdOlbPList *pp;
   XrdOlbPInfo opinfo, npinfo;
   const char *ptype;
   char buff[80];
   unsigned long long Opts;
   int NoGo = 0;
   npinfo.rovec = 1;

// First, add nostage to all exported path if nothing was specified and stagecmd
// was not specified in the oss section (this is really how it works)
//
   if (DiskSS < 2)
      {while(plp)
            {Opts = plp->Flag();
             if (!(Opts & XRDEXP_STAGE_X)) plp->Set(Opts | XRDEXP_NOSTAGE);
             plp = plp->Next();
            }
       plp = PexpList.First();
      }

// For each path in the export list merge it into the path list
//
   while(plp)
        {Opts = plp->Flag();
         npinfo.rwvec = (Opts & (XRDEXP_GLBLRO | XRDEXP_NOTRW) ? 0 : 1);
         npinfo.ssvec = (Opts & XRDEXP_NOSTAGE ? 0 : 1);
         if (!PathList.Find(plp->Path(), opinfo))
            {if (!(Opts & XRDEXP_LOCAL))
                {PathList.Insert(plp->Path(), &npinfo);
                 if (npinfo.ssvec) DiskSS = 1;
                 if (npinfo.rwvec || npinfo.ssvec)
                    monPathP = new XrdOucTList(plp->Path(), 0, monPathP);
                }
            }
            else if (opinfo.rwvec != npinfo.rwvec
                 ||  opinfo.ssvec != npinfo.ssvec
                 ||  Opts & XRDEXP_LOCAL)
                 {Say.Emsg("Config","all.export conflicts with olb.path ...");
                  sprintf(buff, " %s %s%s", (npinfo.rwvec ? "r/w" : "r/o"),
                         (npinfo.ssvec ? "stage" : "nostage"),
                         (Opts & XRDEXP_LOCAL ? " local" : ""));
                  Say.Say("all.export  ", plp->Path(), buff);
                  buff[0] = (opinfo.rwvec ? 'w' : 'r');
                  buff[1] = (opinfo.ssvec ? 's' : ' '); 
                  buff[2] = ' '; buff[3] = '\0';
                  Say.Say("olb.path ", buff, plp->Path());
                  NoGo = 1;
                 } 
          plp = plp->Next();
         }

// Document what we will be declaring as available
//
   if (!NoGo)
      {Say.Say("The following paths are available to the redirector:");
       if (!(pp = PathList.First())) Say.Say("r  /");
        else while(pp)
             {ptype = pp->PType();
              Say.Say(ptype, (strlen(ptype) > 1 ? " " : "  "), pp->Path());
              pp = pp->Next();
             }
       Say.Say(" ");
      }

// All done
//
   return NoGo;
}

/******************************************************************************/
/*                               P i d F i l e                                */
/******************************************************************************/
  
int XrdOlbConfig::PidFile()
{
    int rc, xfd;
    char buff[1024];
    char pidFN[1200], *ppath=XrdOucUtils::genPath(pidPath,
                                        (strcmp("anon",myInsName)?myInsName:0));
    const char *xop = 0;

    if ((rc = XrdOucUtils::makePath(ppath, XrdOucUtils::pathMode)))
       {Say.Emsg("Config", rc, "create pid file path", ppath);
        free(ppath);
        return 1;
       }

         if (isManager && isServer)
            snprintf(pidFN, sizeof(pidFN), "%s/olbd.super.pid", ppath);
    else if (isServer)
            snprintf(pidFN, sizeof(pidFN), "%s/olbd.pid", ppath);
    else    snprintf(pidFN, sizeof(pidFN), "%s/olbd.mangr.pid", ppath);

    if ((xfd = open(pidFN, O_WRONLY|O_CREAT|O_TRUNC,0644)) < 0) xop = "open";
       else {if ((write(xfd, buff, snprintf(buff,sizeof(buff),"%d",
                                   static_cast<int>(getpid()))) < 0)
             || (LocalRoot && (write(xfd,(void *)"\n&pfx=",6)  < 0 ||
                               write(xfd,(void *)LocalRoot,strlen(LocalRoot)) < 0))
             || (AdminPath && (write(xfd,(void *)"\n&ap=", 5)  < 0 ||
                               write(xfd,(void *)AdminPath,strlen(AdminPath)) < 0))
                ) xop = "write";
             close(xfd);
            }

     if (xop) Say.Emsg("Config", errno, xop, pidFN);
     return xop != 0;
}

/******************************************************************************/
/*                          s e t u p M a n a g e r                           */
/******************************************************************************/
  
int XrdOlbConfig::setupManager()
{
   pthread_t tid;
   int rc;

// Setup a local connection when in supervisor role
//
   if (isServer)
      {char spbuff[1024];
       if (!XrdNetSocket::socketPath(&Say, spbuff, AdminPath,
            "olbd.super", AdminMode | S_IFSOCK)) return 1;
       if (!(NetTCPr = new XrdNetWork(&Say)))
          {Say.Emsg("Config","Unable to create supervisor interface.");
           return 1;
          }
       if (myDomain) NetTCPr->setDomain(myDomain);
       if (NetTCPr->Bind(spbuff, "tcp")) return 1;
      }

// Compute the scheduling policy
//
   sched_RR = (100 == P_fuzz) || !AskPerf
              || !(P_cpu || P_io || P_load || P_mem || P_pag);
   if (sched_RR)
      Say.Say("Config round robin scheduling in effect.");

// Create statistical monitoring thread
//
   if ((rc = XrdSysThread::Run(&tid, XrdOlbStartMonPerf, (void *)0,
                               0, "Performance monitor")))
      {Say.Emsg("Config", rc, "create perf monitor thread");
       return 1;
      }

// Create reference monitoring thread
//
   RefTurn  = 3*XrdOlbManager::STMax*(DiskLinger+1);
   if (RefReset)
      {if ((rc = XrdSysThread::Run(&tid, XrdOlbStartMonRefs, (void *)0,
                                   0, "Refcount monitor")))
          {Say.Emsg("Config", rc, "create refcount monitor thread");
           return 1;
          }
      }

// Create state monitoring thread
//
   if ((rc = XrdSysThread::Run(&tid, XrdOlbStartMonStat, (void *)0,
                               0, "State monitor")))
      {Say.Emsg("Config", rc, "create state monitor thread");
       return 1;
      }

// Initialize the fast redirect queue
//
   RRQ.Init(LUPHold, LUPDelay);

// All done
//
   return 0;
}

/******************************************************************************/
/*                           s e t u p S e r v e r                            */
/******************************************************************************/
  
int XrdOlbConfig::setupServer()
{
   XrdOucTList *tp;
   XrdNetWork *Net;
   XrdNetLink *Relay;
   pthread_t tid;
   int n = 0, rc;

// Make sure we have enough info to be a server
//
   if (!myManagers && !(myManagers = ManList))
      {Say.Emsg("Config", "Manager node not specified for", myRole, "role");
       return 1;
      }

// Count the number of managers we have and tell ManTree about it
//
   tp = myManagers;
   while(tp) {n++; tp = tp->next;}
   if (n > XrdOlbManager::MTMax)
      {Say.Emsg("Config", "Too many managers have been specified"); return 1;}
   ManTree.setMaxCon(n);

// Setup TCP outgoing network connections
//
   if (!(Net = new XrdNetWork(&Say, 0)))
      {Say.Emsg("Config","Unable to create", myRole, "network interface.");
       return 1;
      }
   if (myDomain) Net->setDomain(myDomain);
   Manager.setNet(Net);

// Setup a UDP relay for the server
//
   if (!(Relay = Net->Relay(0, XRDNET_SENDONLY))) return 1;
   XrdOlbServer::setRelay(Relay);

// Calculate overload delay time
//
   if (MaxDelay < 0) MaxDelay = AskPerf*AskPing+30;
   if (DiskWT   < 0) DiskWT   = AskPerf*AskPing+30;

// Create manager monitoring thread
//
   if ((rc = XrdSysThread::Run(&tid, XrdOlbStartMonPing, (void *)0,
                               0, "Ping monitor")))
      {Say.Emsg("Config", rc, "create ping monitor thread");
       return 1;
      }

// If this is a staging server then set up the Prepq object
//
   if (DiskSS) 
      {PrepQ.setParms(Sched);
       PrepOK = PrepQ.Reset();
       Sched->Schedule((XrdJob *)&PrepQ,pendplife+time(0));
      }

// Setup notification path
//
   if (!(AnoteSock = XrdNetSocket::Create(&Say, AdminPath,
                             (isManager|isPeer ? "olbd.seton":"olbd.notes"),
                             AdminMode, XRDNET_UDPSOCKET))) return 1;

// Construct the nostage/suspend file path names
//
  {char fnbuff[1048];
   int i;

   i = strlen(AdminPath);
   strcpy(fnbuff, AdminPath);
   if (AdminPath[i-1] != '/') fnbuff[i++] = '/';
   strcpy(fnbuff+i, "NOSTAGE");
   NoStageFile = strdup(fnbuff);
   strcpy(fnbuff+i, "SUSPEND");
   SuspendFile = strdup(fnbuff);
  }

// Determine if we are in nostage and/or suspend state
//
   if (inNoStage())
      {Say.Say("Config starting in NOSTAGE state.");
       Manager.Stage(0, 0);
      }
   if (inSuspend())
      {Say.Say("Config starting in SUSPEND state.");
       Manager.Suspend(0);
      }

// We have data only if we are a pure data server (the default is noData)
// If we have no data, then we are done (the rest is for pure servers)
//
   if (isManager || isPeer || isProxy) return 0;
   Manager.hasData = 1;

// If no cache has been specified but paths exist get the pfn for each path
// in the list for monitoring purposes
//
   if (!monPath && monPathP && lcl_N2N)
      {XrdOucTList *tlp = monPathP;
       char pbuff[2048];
       while(tlp)
            {if ((rc = lcl_N2N->lfn2pfn(tlp->text, pbuff, sizeof(pbuff))))
                Say.Emsg("Config",rc,"determine pfn for lfn",tlp->text);
                else {free(tlp->text);
                      tlp->text = strdup(pbuff);
                     }
             tlp = tlp->next;
            }
       }

// Setup file system metering (skip it for peers)
//
   Meter.setParms(monPath ? monPath : monPathP, monPath != 0);
   if (perfpgm && Meter.Monitor(perfpgm, perfint))
      Say.Say("Config warning: load based scheduling disabled.");

// If this is a staging server then we better have a disk cache. We ignore this
// restriction if an XMI plugin will be used and we are a peer.
//
   if (!(isPeer || XmiPath) && DiskSS && !(monPath || monPathP))
      {Say.Emsg("Config","Staging paths present but no disk cache specified.");
       return 1;
      }

// All done
//
   return 0;
}

/******************************************************************************/
/*                              s e t u p X m i                               */
/******************************************************************************/
  
int XrdOlbConfig::setupXmi()
{
   EPNAME("setupXmi");
   static XrdOlbXmiEnv XmiEnv;
   XrdSysPlugin       *xmiLib;
   XrdOlbXmi          *(*ep)(int, char **, XrdOlbXmiEnv *);
   unsigned int isNormal, isDirect;
   XrdOlbXmi          *XMI, *myXMI;
   const char         *theMode;
   int i;

   struct {unsigned int   theMask;
           XrdOlbXmi    **theAddr;
           const char    *theName;} XmiTab[] =
          {{XMI_CHMOD,  &Xmi_Chmod,  "chmod"},
           {XMI_LOAD,   &Xmi_Load,   "load"},
           {XMI_MKDIR,  &Xmi_Mkdir,  "mkdir"},
           {XMI_MKPATH, &Xmi_Mkpath, "mkpath"},
           {XMI_PREP,   &Xmi_Prep,   "prep"},
           {XMI_RENAME, &Xmi_Rename, "rename"},
           {XMI_REMDIR, &Xmi_Remdir, "remdir"},
           {XMI_REMOVE, &Xmi_Remove, "remove"},
           {XMI_SELECT, &Xmi_Select, "select"},
           {XMI_SPACE,  &Xmi_Space,  "space"},
           {XMI_STAT,   &Xmi_Stat,   "stat"}};
   int numintab = sizeof(XmiTab)/sizeof(XmiTab[0]);

// Fill out the rest of the XmiEnv structure
//
   XmiEnv.Role     = myRole;
   XmiEnv.ConfigFN = ConfigFN;
   XmiEnv.Parms    = XmiParms;
   XmiEnv.eDest    = &Say;
   XmiEnv.iNet     = NetTCP;
   XmiEnv.Sched    = Sched;
   XmiEnv.Trace    = &Trace;
   XmiEnv.Name2Name= xeq_N2N;

// Create a pluin object (we will throw this away without deletion because
// the library must stay open but we never want to reference it again).
//
   if (!(xmiLib = new XrdSysPlugin(&Say, XmiPath))) return 1;

// Now get the entry point of the object creator
//
   ep = (XrdOlbXmi *(*)(int, char **, XrdOlbXmiEnv *))(xmiLib->getPlugin("XrdOlbgetXmi"));
   if (!ep) return 1;

// Get the Object now
//
   if (!(XMI = ep(inArgc, inArgv, &XmiEnv))) return 1;
   DEBUG("xmi library loaded; path=" <<XmiPath);

// Obtain the execution mode
//
   XMI->XeqMode(isNormal, isDirect);

// Check if we need to create an indirect XMI interface
//
   if ((isDirect & XMI_ALL) == XMI_ALL) myXMI = 0;
      else myXMI = (XrdOlbXmi *)new XrdOlbXmiReq(XMI);

// Now run throw all of the possibilities setting the execution mode as needed
//
   for (i = 0; i < numintab; i++)
       {if (!(isNormal & XmiTab[i].theMask))
           if (isDirect & XmiTab[i].theMask)
                   {*XmiTab[i].theAddr =   XMI; theMode = "direct";}
              else {*XmiTab[i].theAddr = myXMI; theMode = "queued";}
           else theMode = "normal";
        DEBUG(XmiTab[i].theName <<" is " <<theMode);
       }
   return 0;
}

/******************************************************************************/
/*                                 U s a g e                                  */
/******************************************************************************/
  
void XrdOlbConfig::Usage(int rc)
{
cerr <<"\nUsage: olbd [xrdopts] [-i] [-m] [-s] -c <cfile>" <<endl;
exit(rc);
}
  
/******************************************************************************/
/*                                x a l l o w                                 */
/******************************************************************************/

/* Function: xallow

   Purpose:  To parse the directive: allow {host | netgroup} <name>

             <name> The dns name of the host that is allowed to connect or the
                    netgroup name the host must be a member of. For DNS names,
                    a single asterisk may be specified anywhere in the name.

   Type: Manager only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xallow(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val;
    int ishost;

    if (!isManager) return 0;

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "allow type not specified"); return 1;}

    if (!strcmp(val, "host")) ishost = 1;
       else if (!strcmp(val, "netgroup")) ishost = 0;
               else {eDest->Emsg("Config", "invalid allow type -", val);
                     return 1;
                    }

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "allow target name not specified"); return 1;}

    if (!Police) Police = new XrdNetSecurity();
    if (ishost)  Police->AddHost(val);
       else      Police->AddNetGroup(val);

    return 0;
}

/******************************************************************************/
/*                                x a p a t h                                 */
/******************************************************************************/

/* Function: xapath

   Purpose:  To parse the directive: adminpath <path>

             <path>    the path of the named socket to use for admin requests.

   Type: Manager and Server, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xapath(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *pval, *val;
    mode_t mode = S_IRWXU;
    struct sockaddr_un USock;

// Get the path
//
   pval = CFile.GetWord();
   if (!pval || !pval[0])
      {eDest->Emsg("Config", "adminpath not specified"); return 1;}

// Make sure it's an absolute path
//
   if (*pval != '/')
      {eDest->Emsg("Config", "adminpath not absolute"); return 1;}

// Make sure path is not too long (account for "/olbd.admin")
//                                              12345678901
   if (strlen(pval) > sizeof(USock.sun_path) - 11)
      {eDest->Emsg("Config", "admin path", pval, "is too long");
       return 1;
      }
   pval = strdup(pval);

// Get the optional access rights
//
   if ((val = CFile.GetWord()) && val[0])
      {if (!strcmp("group", val)) mode |= S_IRWXG;
          else {eDest->Emsg("Config", "invalid admin path modifier -", val);
                free(pval); return 1;
               }
      }

// Record the path
//
   if (AdminPath) free(AdminPath);
   AdminPath = pval;
   AdminMode = mode;
   return 0;
}

/******************************************************************************/
/*                                x c a c h e                                 */
/******************************************************************************/

/* Function: xcache

   Purpose:  To parse the directive: cache <group> <path>[*]

             <group>   the cache group (ignored for olbd)
             <path>    the full path of the filesystem the server will handle.

   Type: Server only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xcache(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val, *pfxdir, *sfxdir, fn[XrdOlbMAX_PATH_LEN+1];
    int i, k, rc, pfxln, cnum = 0;
    struct dirent *dir;
    DIR *DFD;

    if (!isServer) return 0;

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "cache group not specified"); return 1;}

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "cache path not specified"); return 1;}

    k = strlen(val);
    if (k >= (int)(sizeof(fn)-1) || val[0] != '/' || k < 2)
       {eDest->Emsg("Config", "invalid cache path - ", val); return 1;}

    if (val[k-1] != '*') return !Fsysadd(eDest, 0, val);

    for (i = k-1; i; i--) if (val[i] == '/') break;
    i++; strncpy(fn, val, i); fn[i] = '\0';
    sfxdir = &fn[i]; pfxdir = &val[i]; pfxln = strlen(pfxdir)-1;
    if (!(DFD = opendir(fn)))
       {eDest->Emsg("Config", errno, "open cache directory", fn); return 1;}

    errno = 0; rc = 0;
    while((dir = readdir(DFD)))
         {if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")
          || (pfxln && strncmp(dir->d_name, pfxdir, pfxln)))
             continue;
          strcpy(sfxdir, dir->d_name);
          if ((rc = Fsysadd(eDest, 1, fn))  < 0) break;
          cnum += rc;
         }

    if (errno)
       {if (rc >= 0) 
           {rc = -1; eDest->Emsg("Config", errno, "process cache directory", fn);}
       }
       else if (!cnum) eDest->Say("Config warning: no cache directories found in ",val);

    closedir(DFD);
    return rc < 0;
}

int XrdOlbConfig::Fsysadd(XrdSysError *eDest, int chk, char *fn)
{
    struct stat buff;

    if (stat(fn, &buff))
       {if (!chk) eDest->Emsg("Config", errno, "process r/w path", fn);
        return -1;
       }

    if ((chk > 0) && !(buff.st_mode & S_IFDIR)) return 0;

    monPath = new XrdOucTList(fn, 0, monPath);
    return 1;
}

/******************************************************************************/
/*                                x d e l a y                                 */
/******************************************************************************/

/* Function: xdelay

   Purpose:  To parse the directive: delay [lookup <sec>] [overload <sec>]
                                           [startup <sec>] [servers <cnt>[%]]
                                           [full <sec>] [discard <cnt>]
                                           [suspend <sec>] [drop <sec>]
                                           [service <sec>] [hold <msec>]
                                           [peer <sec>]

   discard   <cnt>     maximum number a message may be forwarded.
   drop      <sec>     seconds to delay a drop of an offline server.
   full      <sec>     seconds to delay client when no servers have space.
   hold      <msec>    millseconds to optimistically hold requests.
   lookup    <sec>     seconds to delay client when finding a file.
   overload  <sec>     seconds to delay client when all servers overloaded.
   peer      <sec>     maximum seconds client may be delayed before peer
                       selection is triggered.
   servers   <cnt>     minimum number of servers we need.
   service   <sec>     seconds to delay client when waiting for servers.
   startup   <sec>     seconds to delay enabling our service
   suspend   <sec>     seconds to delay client when all servers suspended.

   Type: Manager only, dynamic.

   Output: 0 upon success or !0 upon failure.
*/
int XrdOlbConfig::xdelay(XrdSysError *eDest, XrdOucStream &CFile)
{   char *val;
    const char *etxt = "invalid delay option";
    int  i, ppp, ispercent = 0;
    static struct delayopts {const char *opname; int *oploc; int istime;}
           dyopts[] =
       {
        {"discard",  &MsgTTL,   0},
        {"drop",     &DRPDelay, 1},
        {"full",     &DiskWT,  -1},
        {"hold",     &LUPHold,  0},
        {"lookup",   &LUPDelay, 1},
        {"overload", &MaxDelay,-1},
        {"peer",     &PSDelay,  1},
        {"servers",  &SUPCount, 0},
        {"service",  &SUPDelay, 1},
        {"startup",  &SRVDelay, 1},
        {"suspend",  &SUSDelay, 1}
       };
    int numopts = sizeof(dyopts)/sizeof(struct delayopts);

    if (!isManager && !isPeer) return 0;

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "delay arguments not specified"); return 1;}

    while (val)
          {for (i = 0; i < numopts; i++)
               if (!strcmp(val, dyopts[i].opname))
                  {if (!(val = CFile.GetWord()))
                      {eDest->Emsg("Config", "delay ", dyopts[i].opname,
                                   " argument not specified.");
                       return 1;
                      }
                   if (dyopts[i].istime < 0 && !strcmp(val, "*")) ppp = -1;
                      else if (dyopts[i].istime)
                              {if (XrdOuca2x::a2tm(*eDest,etxt,val,&ppp,1))
                                  return 1;
                              } else {
                               if (*dyopts[i].opname == 's')
                                  {ppp = strlen(val); SUPLevel = 0;
                                   if (val[ppp-1] == '%')
                                      {ispercent = 1; val[ppp-1] = '\0';}
                                  }
                               if (XrdOuca2x::a2i( *eDest,etxt,val,&ppp,1))
                                  return 1;
                              }
                   if (!ispercent) *dyopts[i].oploc = ppp;
                      else {ispercent = 0; SUPCount = 1; SUPLevel = ppp;}
                   break;
                  }
           if (i >= numopts) 
              eDest->Say("Config warning: ignoring invalid delay option '",val,"'.");
           val = CFile.GetWord();
          }
     return 0;
}

/******************************************************************************/
/*                                 x d e f s                                  */
/******************************************************************************/

/* Function: xdefs

   Purpose:  Parse: oss.defaults <default options>
                              
   Notes: See the oss configuration manual for the meaning of each option.
          The actual implementation is defined in XrdOucExport.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xdefs(XrdSysError *eDest, XrdOucStream &CFile)
{
   if (!isServer) return 0;
   DirFlags = XrdOucExport::ParseDefs(CFile, *eDest, DirFlags);
   return 0;
}
  
/******************************************************************************/
/*                                 x e x p o                                  */
/******************************************************************************/

/* Function: xexpo

   Purpose:  To parse the directive: all.export <path> [<options>]

             <path>    the full path that resides in a remote system.
             <options> a blank separated list of options (see XrdOucExport)

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xexpo(XrdSysError *eDest, XrdOucStream &CFile)
{
   XrdOucPList *plp, *olp;
   unsigned long long Opts = DirFlags & XRDEXP_SETTINGS;

   if (!isServer) return 0;

// Parse the arguments
//
   if (!(plp = XrdOucExport::ParsePath(CFile, *eDest, Opts))) return 1;

// Check if this path is being modified or added. For modifications, turn off
// all bitsin the old path specified in the new path and then set the new bits.
//
   if (!(olp = PexpList.Match(plp->Path()))) PexpList.Insert(plp);
      else {Opts = plp->Flag() >> XRDEXP_MASKSHIFT;
            Opts = olp->Flag() & ~Opts;
            olp->Set(Opts | plp->Flag());
            delete plp;
           }
   return 0;
}
  
/******************************************************************************/
/*                                 x f s x q                                  */
/******************************************************************************/
  
/* Function: xfsxq

   Purpose:  To parse the directive: fsxeq <types> <prog>

             <types>   what operations the program performs (one or more of):
                       chmod mkdir mkpath mv rm rmdir
             <prog>    the program to execute when doing a forwarded fs op.

   Type: Server only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xfsxq(XrdSysError *eDest, XrdOucStream &CFile)
{
    struct xeqopts {const char *opname; int doset; XrdOucProg **pgm;} xqopts[] =
       {
        {"chmod",    0, &ProgCH},
        {"mkdir",    0, &ProgMD},
        {"mkpath",   0, &ProgMP},
        {"mv",       0, &ProgMV},
        {"rm",       0, &ProgRM},
        {"rmdir",    0, &ProgRD}
       };
    int i, xtval = 0, numopts = sizeof(xqopts)/sizeof(struct xeqopts);
    char *val;

// If we are a manager, ignore this option
//
   if (!isServer) return 0;

// Get the operation types
//
    val = CFile.GetWord();
    while (val && *val != '/')
          {for (i = 0; i < numopts; i++)
               if (!strcmp(val, xqopts[i].opname))
                  {xqopts[i].doset = 1;
                   xtval = 1;
                   break;
                  }
           if (i >= numopts)
              eDest->Say("Config warning: ignoring invalid fsxeq type option '",val,"'.");
           val = CFile.GetWord();
          }

// Make sure some type was specified
//
   if (!xtval)
      {eDest->Emsg("Config", "fsxeq type option not specified"); return 1;}

// Make sure a program was specified
//
   if (!val)
      {eDest->Emsg("Config", "fsxeq program not specified"); return 1;}

// Get the program
//
   CFile.RetToken();

// Set the program for each type
//
   for (i = 0; i < numopts; i++)
       if (xqopts[i].doset)
          {if (!*xqopts[i].pgm) *(xqopts[i].pgm) = new XrdOucProg(0);
           if ((*(xqopts[i].pgm))->Setup(val, eDest)) return 1;
          }

// All done
//
   return 0;
}

/******************************************************************************/
/*                                x f x h l d                                 */
/******************************************************************************/

/* Function: xfxhld

   Purpose:  To parse the directive: fxhold <sec>

             <sec>  number of seconds (or M, H, etc) to cache file existence

   Type: Manager only, dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xfxhld(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val;
    int ct;

    if (!isManager) return 0;

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "fxhold value not specified."); return 1;}

    if (XrdOuca2x::a2tm(*eDest, "fxhold value", val, &ct, 60)) return 1;

    cachelife = ct;
    Cache.setLifetime(ct);
    return 0;
}

/******************************************************************************/
/*                                x l c l r t                                 */
/******************************************************************************/

/* Function: xlclrt

   Purpose:  To parse the directive: localroot <path>

             <path>    the path that the server will prefix to all local paths.

   Type: Server only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xlclrt(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val;
    int i;

// If we are a manager, ignore this option
//
   if (!isServer) return 0;

// Get path type
//
   val = CFile.GetWord();
   if (!val || !val[0])
      {eDest->Emsg("Config", "localroot path not specified"); return 1;}
   if (*val != '/')
      {eDest->Emsg("Config", "localroot path not absolute"); return 1;}

// Cleanup the path
//
   i = strlen(val)-1;
   while (i && val[i] == '/') val[i--] = '\0';

// Assign new path prefix
//
   if (i)
      {if (LocalRoot) free(LocalRoot);
       LocalRoot = strdup(val);
      }
   return 0;
}

/******************************************************************************/
/*                                 x m a n g                                  */
/******************************************************************************/

/* Function: xmang

   Purpose:  Parse: manager [peer | proxy] [all|any] <host>[+][:<port>|<port>] 
                                                     [if ...]

             peer   For olbd:   Specified the manager when running as a peer
                    For xrootd: The directive is ignored.
             proxy  For olbd:   This directive is ignored.
                    For xrootd: Specifies the odc-proxy service manager
             all    Ignored (useful only to the odc)
             any    Ignored (useful only to the odc)
             <host> The dns name of the host that is the cache manager.
                    If the host name ends with a plus, all addresses that are
                    associated with the host are treated as managers.
             <port> The port number to use for this host.
             if     Apply the manager directive if "if" is true. See
                    XrdOucUtils:doIf() for "if" syntax.

   Notes:   Any number of manager directives can be given. When niether peer nor
            proxy is specified, then regardless of role the following occurs:
            olbd:   Subscribes to each manager whens role is not peer.
            xrootd: Logins in as a redirector to each manager when role is not 
                    proxy or server.
            The subscribe directive also provides this information but is now 
            deprecated.

   Type: Remote server only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xmang(XrdSysError *eDest, XrdOucStream &CFile)
{
    struct sockaddr InetAddr[8];
    XrdOucTList *tp = 0;
    char *val, *bval = 0, *mval = 0;
    int j, i, port = 0, xPeer = 0, xProxy = 0;

//  Process the optional "peer" or "proxy"
//
    if ((val = CFile.GetWord()))
       {if ((xPeer = !strcmp("peer", val)) || (xProxy = !strcmp("proxy", val)))
           {if (xProxy || (xPeer && !isPeer)) return 0;
            val = CFile.GetWord();
           } else if (isPeer) return 0;
       }

//  We can accept this manager. Skip the optional "all" or "any"
//
    if (val)
       if (!strcmp("any", val) || !strcmp("all", val)) val = CFile.GetWord();

//  Get the actual host name
//
    if (!val)
       {eDest->Emsg("Config","manager host name not specified"); return 1;}
    mval = strdup(val);

//  Grab the port number
//
    if ((val = index(mval, ':'))) {*val = '\0'; val++;}
       else val = CFile.GetWord();

    if (val)
       {if (isdigit(*val))
           {if (XrdOuca2x::a2i(*eDest,"manager port",val,&port,1,65535))
               port = 0;
           }
           else if (!(port = XrdNetDNS::getPort(val, "tcp")))
                   {eDest->Emsg("Config", "Unable to find tcp service '",val,"'.");
                    port = 0;
                   }
       }
       else if (!(port = PortTCP))
               eDest->Emsg("Config","manager port not specified for",mval);

    if (!port) {free(mval); return 1;}

    if ((val = CFile.GetWord()))
       {if (strcmp(val, "if"))
           {eDest->Emsg("Config","expecting manager 'if' but",val,"found");
            return 1;
           }
        if ((i=XrdOucUtils::doIf(eDest,CFile,"manager directive",
                            myName,myInsName,myProg))<=0) return i < 0;
       }

    i = strlen(mval);
    if (mval[i-1] != '+') 
       {i = 0;
        if (!XrdNetDNS::getHostAddr(mval, InetAddr))
           {eDest->Emsg("CFile","Manager host", mval, "not found");
            free(mval); return 1;
           }
       }
        else {bval = strdup(mval); mval[i-1] = '\0';
              if (!(i = XrdNetDNS::getHostAddr(mval, InetAddr, 8)))
                 {eDest->Emsg("CFile","Manager host", mval, "not found");
                  free(bval); free(mval); return 1;
                 }
             }

    if (isManager && !isServer)
       for (j = 0; j <= i; j++)
           if (!memcmp(&InetAddr[j], &myAddr, sizeof(struct sockaddr)))
              {PortTCP = port; break;}

    do {if (i)
           {i--; free(mval);
            char mvBuff[1024];
            sprintf(mvBuff, "%s -> all.manager ", bval);
            mval = XrdNetDNS::getHostName(InetAddr[i]);
            eDest->Say("Config ", mvBuff, mval);
           }
        tp = ManList;
        while(tp) 
             if (strcmp(tp->text, mval) || tp->val != port) tp = tp->next;
                else {eDest->Say("Config warning: duplicate manager ",mval);
                      break;
                     }
        if (tp) break;
        ManList = new XrdOucTList(mval, port, ManList);
       } while(i);

    if (bval) free(bval);
    free(mval);
    return tp != 0;
}
  
/******************************************************************************/
/*                                  x n m l                                   */
/******************************************************************************/

/* Function: xnml

   Purpose:  To parse the directive: namelib <path> [<parms>]

             <path>    the path of the filesystem library to be used.
             <parms>   optional parms to be passed

  Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xnml(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val, parms[1024];

// Get the path
//
   if (!(val = CFile.GetWord()) || !val[0])
      {eDest->Emsg("Config", "namelib not specified"); return 1;}

// Record the path
//
   if (N2N_Lib) free(N2N_Lib);
   N2N_Lib = strdup(val);

// Record any parms
//
   if (!CFile.GetRest(parms, sizeof(parms)))
      {eDest->Emsg("Config", "namelib parameters too long"); return 1;}
   if (N2N_Parms) free(N2N_Parms);
   N2N_Parms = (*parms ? strdup(parms) : 0);
   return 0;
}

/******************************************************************************/
/*                                 x p a t h                                  */
/******************************************************************************/

/* Function: xpath

   Purpose:  To parse the directive: path {r | w | rw}[s] <path>

             <path>    the full path that the server will hanlde. This is

   Type: Server only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xpath(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val, *path;
    int i = -1;
    XrdOlbPInfo  pmask;

// If we are a manager, ignore this option
//
   if (!isServer) return 0;

// Get path type
//
   val = CFile.GetWord();
   if (!val || !val[0])
      {eDest->Emsg("Config", "path type not specified"); return 1;}

// Translate path type
//
   while(val[++i])
              if ('r' == val[i])  pmask.rovec = 1;
         else if ('w' == val[i])  pmask.rovec = pmask.rwvec = 1;
         else if ('s' == val[i]) {pmask.rovec = pmask.ssvec = 1; DiskSS |= 1;}
         else {eDest->Emsg("Config", "invalid path type", val); return 1;}

// Get the path
//
   path = CFile.GetWord();
   if (!path || !path[0])
      {eDest->Emsg("Config", "path not specified"); return 1;}

// Add the path to the list of paths
//
   PathList.Insert(path, &pmask);

// If the path is writable, add it to the potential cache list. This list gets
// used if no cache directives have been specified.
//
   if (pmask.rwvec || pmask.ssvec) monPathP = new XrdOucTList(path,0,monPathP);
   return 0;
}
  
/******************************************************************************/
/*                                 x p e r f                                  */
/******************************************************************************/

/* Function: xperf

   Purpose:  To parse the directive: perf [key <num>] [int <sec>] [pgm <pgm>]

         int <time>    estimated time (seconds, M, H) between reports by <pgm>
         key <num>     This is no longer documented but kept for compatability.
         pgm <pgm>     program to start that will write perf values to standard
                       out. It must be the last option.

   Type: Server only, non-dynamic.

   Output: 0 upon success or !0 upon failure. Ignored by manager.
*/
int XrdOlbConfig::xperf(XrdSysError *eDest, XrdOucStream &CFile)
{   int   ival = 3*60;
    char *pgm=0, *val, rest[2048];

    if (!isServer) return 0;

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "perf options not specified"); return 1;}

    do {     if (!strcmp("int", val))
                {if (!(val = CFile.GetWord()))
                    {eDest->Emsg("Config", "perf int value not specified");
                     return 1;
                    }
                 if (XrdOuca2x::a2tm(*eDest,"perf int",val,&ival,0)) return 1;
                }
        else if (!strcmp("pgm",  val))
                {if (!CFile.GetRest(rest, sizeof(rest)))
                    {eDest->Emsg("Config", "perf pgm parameters too long"); return 1;}
                 if (!*rest)
                    {eDest->Emsg("Config", "perf prog value not specified");
                     return 1;
                    }
                 pgm = rest;
                 break;
                }
        else eDest->Say("Config warning: ignoring invalid perf option '",val,"'.");
       } while((val = CFile.GetWord()));

// Make sure that the perf program is here
//
   if (perfpgm) {free(perfpgm); perfpgm = 0;}
   if (pgm) {if (!isExec(eDest, "perf", pgm)) return 1;
                else perfpgm = strdup(pgm);
            }

// Set remaining values
//
    perfint = ival;
    return 0;
}

  
/******************************************************************************/
/*                                 x p i d f                                  */
/******************************************************************************/

/* Function: xpidf

   Purpose:  To parse the directive: pidpath <path>

             <path>    the path where the pid file is to be created.

  Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xpidf(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val;

// Get the path
//
   val = CFile.GetWord();
   if (!val || !val[0])
      {eDest->Emsg("Config", "pidpath not specified"); return 1;}

// Record the path
//
   if (pidPath) free(pidPath);
   pidPath = strdup(val);
   return 0;
}
  
/******************************************************************************/
/*                                 x p i n g                                  */
/******************************************************************************/

/* Function: xping

   Purpose:  To parse the directive: ping <ptm> [log <num>] [usage <cnt>]

             <ptm>     Time (seconds, M, H. etc) between keepalive pings.
                       The default is 60 seconds.
             log       values are logged to the log every <num> usage
                       requests (zero, the default, suppresses logging).
             usage     The number of pings between resource usage requests.
                       The default is 10. Zero suppresses usage requests.

   Type: Server for ping value and Manager for all values, dynamic.

   Output: 0 upon success or !0 upon failure.
*/
int XrdOlbConfig::xping(XrdSysError *eDest, XrdOucStream &CFile)
{   int pnum = AskPerf, lnum = LogPerf, ping;
    char *val;

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "ping value not specified"); return 1;}
    if (XrdOuca2x::a2tm(*eDest, "ping interval",val,&ping,0)) return 1;


    while((val = CFile.GetWord()))
        {     if (!strcmp("log", val))
                 {if (!(val = CFile.GetWord()))
                     {eDest->Emsg("Config", "ping log value not specified");
                      return 1;
                     }
                  if (XrdOuca2x::a2i(*eDest,"ping log",val,&lnum,0)) return 1;
                 }
         else if (!strcmp("usage", val))
                 {if (!(val = CFile.GetWord()))
                    {eDest->Emsg("Config", "ping usage value not specified");
                     return 1;
                    }
                  if (XrdOuca2x::a2i(*eDest,"ping usage",val,&pnum,1)) return 1;
                 }
        }
    AskPerf = pnum;
    AskPing = ping;
    LogPerf = lnum;
    return 0;
}

/******************************************************************************/
/*                                 x p o r t                                  */
/******************************************************************************/

/* Function: xport

   Purpose:  To parse the directive: port <tcpnum> [if ...]

             <tcpnum>   number of the tcp port for incomming requests
             if         Apply the manager directive if "if" is true. See
                        XrdOucUtils:doIf() for "if" syntax.

   Type: Manager or Server, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/
int XrdOlbConfig::xport(XrdSysError *eDest, XrdOucStream &CFile)
{   int rc, pnum = 0;
    char *val;

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "tcp port not specified"); return 1;}
    if (isdigit(*val))
       {if (XrdOuca2x::a2i(*eDest,"tcp port",val,&pnum,1,65535)) return 1;}
       else if (!(pnum = XrdNetDNS::getPort(val, "tcp")))
               {eDest->Emsg("Config", "Unable to find tcp service", val);
                return 1;
               }

    if ((val = CFile.GetWord()) && !strcmp("if", val))
       if ((rc = XrdOucUtils::doIf(eDest,CFile,"role directive",
                              myName,myInsName,myProg)) <= 0) return (rc < 0);

    PortTCP = pnum;

    return 0;
}
  
/******************************************************************************/
/*                                 x p r e p                                  */
/******************************************************************************/

/* Function: xprep

   Purpose:  To parse the directive: prep  [echo]
                                           [reset <cnt>] [scrub <sec>] 
                                           [ifpgm <pgm>]

         echo          display list of pending prepares during resets.
         reset <cnt>   number of scrubs after which a full reset is done.
         scrub <sec>   time (seconds, M, H) between pendq scrubs.
         ifpgm <pgm>   program that adds, deletes, and lists prepare queue
                       entries. It must be specified as the last option
                       on the line.

   Type: Any, non-dynamic. Note that the Manager only need the "batch" option
         while slacves need the remaining options.

   Output: 0 upon success or !0 upon failure. Ignored by manager.
*/
int XrdOlbConfig::xprep(XrdSysError *eDest, XrdOucStream &CFile)
{   int   reset=0, scrub=0, echo = 0, doset = 0;
    char  *prepif=0, *val, rest[2048];

    if (!isServer) return 0;

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "prep options not specified"); return 1;}

    do {     if (!strcmp("echo", val)) doset = echo = 1;
        else if (!strcmp("reset", val))
                {if (!(val = CFile.GetWord()))
                    {eDest->Emsg("Config", "prep reset value not specified");
                     return 1;
                    }
                 if (XrdOuca2x::a2i(*eDest,"prep reset int",val,&reset,1)) return 1;
                 doset = 1;
                }
        else if (!strcmp("scrub", val))
                {if (!(val = CFile.GetWord()))
                    {eDest->Emsg("Config", "prep scrub value not specified");
                     return 1;
                    }
                 if (XrdOuca2x::a2tm(*eDest,"prep scrub",val,&scrub,0)) return 1;
                 doset = 1;
                }
        else if (!strcmp("ifpgm",  val))
                {if (!CFile.GetRest(rest, sizeof(rest)))
                    {eDest->Emsg("Config", "prep ifpgm parameters too long"); return 1;}
                 if (!*rest)
                    {eDest->Emsg("Config", "prep ifpgm value not specified");
                     return 1;
                    }
                 prepif = rest;
                 break;
                }
        else eDest->Say("Config warning: ignoring invalid prep option '",val,"'.");
       } while((val = CFile.GetWord()));



// Set the values
//
   if (scrub) pendplife = scrub;
   if (doset) PrepQ.setParms(reset, scrub, echo);
   if (prepif) {if (!isExec(eDest, "prep", prepif)) return 1;
                   else return PrepQ.setParms(prepif);
               }
   return 0;
}

/******************************************************************************/
/*                                x p r e p m                                 */
/******************************************************************************/

/* Function: xprepm

   Purpose:  To parse the directive: prepmsg <msg>

             <msg>     the message to be sent to the prep ifpgm (see prep).

   Type: Manager only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xprepm(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val, buff[2048];
    XrdOucEnv *myEnv = CFile.SetEnv(0);

   // At this point, make sure we have a value
   //
   if (!(val = CFile.GetWord()))
      {eDest->Emsg("Config", "no value for prepmsg directive");
       CFile.SetEnv(myEnv);
       return 1;
      }

   // We need to suck all the tokens to the end of the line for remaining
   // options. Do so, until we run out of space in the buffer.
   //
   CFile.RetToken();
   if (!CFile.GetRest(buff, sizeof(buff)))
      {eDest->Emsg("Config", "prepmsg arguments too long");
       CFile.SetEnv(myEnv);
       return 1;
      }

   // Restore substitutions and parse the message
   //
   CFile.SetEnv(myEnv);
   return PrepQ.setParms(0, buff);
}
  
/******************************************************************************/
/*                                x r m t r t                                 */
/******************************************************************************/

/* Function: xrmtrt

   Purpose:  To parse the directive: remoteroot <path>

             <path>    the path that the server will prefix to all remote paths.

   Type: Manager only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xrmtrt(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val, *colon, *slash;
    int i;

// If we are a manager, ignore this option
//
   if (isManager) return 0;

// Get path type
//
   val = CFile.GetWord();
   if (!val || !val[0])
      {eDest->Emsg("Config", "remoteroot path not specified"); return 1;}

// For remote roots we allow a url-type specification o/w path must be absolute
//
   if (*val != '/')
      {colon = index(val, ':'); slash = index(val, '/');
       if ((colon+1) != slash)
          {eDest->Emsg("Config", "remoteroot path not absolute"); return 1;}
      }

// Cleanup the path
//
   i = strlen(val)-1;
   while (i && val[i] == '/') val[i--] = '\0';

// Assign new path prefix
//
   if (i)
      {if (RemotRoot) free(RemotRoot);
       RemotRoot = strdup(val);
      }
   return 0;
}

/******************************************************************************/
/*                                 x r o l e                                  */
/******************************************************************************/

/* Function: xrole
   Purpose:  Parse: role {[peer] [proxy] manager | peer | proxy | [proxy] server
                          | [proxy] supervisor} [if ...]

             manager    xrootd: act as a manager (redirecting server). Prefix
                                modifications are ignored.
                        olbd:   accept server subscribes and redirectors. Prefix
                                modifiers do the following:
                                peer  - subscribe to other managers as a peer
                                proxy - manage a cluster of proxy servers

             peer       xrootd: same as "peer manager"
                        olbd:   same as "peer manager" but no server subscribers
                                are required to function (i.e., run stand-alone).

             proxy      xrootd: act as a server but supply data from another 
                                server. No local olbd is present or required.
                        olbd:   Generates an error as this makes no sense.

             server     xrootd: act as a server (supply local data). Prefix
                                modifications do the following:
                                proxy - server is part of a cluster. A local
                                        olbd is required.
                        olbd:   subscribe to a manager, possibly as a proxy.

             supervisor xrootd: equivalent to manager.
                        olbd:   equivalent to manager but also subscribe to a
                                manager. When proxy is specified, subscribe as
                                a proxy and only accept proxy servers.


             if         Apply the manager directive if "if" is true. See
                        XrdOucUtils:doIf() for "if" syntax.


   Type: Server only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xrole(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val, role[64];
    int rc, xPeer=0, xProxy=0, xServ=0, xMan=0, xSolo=0, xSup=0;

    *role = '\0';
    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "role not specified"); return 1;}


// First screen for "peer"
//
   if (!strcmp("peer", val))
      {xPeer = -1;
       strcpy(role, val);
       val = CFile.GetWord();
      }

// Now scan for "proxy"
//
   if (val && !strcmp("proxy", val))
      {xProxy = -1;
       if (xPeer) strcat(role, " ");
       strcat(role, val);
       val = CFile.GetWord();
      }

// Scan for other possible alternatives
//
   if (val && strcmp("if", val))
      {     if (!strcmp("manager",    val)) {xMan = -1;}
       else if (!strcmp("server",     val)) {           xServ = -1;}
       else if (!strcmp("supervisor", val)) {xMan = -1; xServ = -1; xSup = -1;}
       else    {eDest->Emsg("Config", "invalid role -", val); return 1;}

       if (xPeer || xProxy) strcat(role, " ");
       strcat(role, val);
       val = CFile.GetWord();
      }

// Scan for invalid roles: peer proxy | peer server | peer supervisor
//
   if (((xPeer && xProxy) && !(xMan || xServ))
   ||  (xPeer && xServ)
   ||  (xPeer && xSup))
      {eDest->Emsg("Config", "invalid role -", role); return 1;}
   if (!(xMan || xServ) && xProxy)
      {eDest->Emsg("Config", "pure proxy role is not supported"); return 1;}

// Make sure a role was specified
//
    if (!(xPeer || xProxy || xServ || xMan))
       {eDest->Emsg("Config", "role not specified"); return 1;}

// Check if this is a solo peer
//
    if (xPeer) if (!xMan) {xSolo = 1; xServ = -1;}

// Handle optional "if"
//
    if (val && !strcmp("if", val))
       if ((rc = XrdOucUtils::doIf(eDest,CFile,"role directive",
                              myName,myInsName,myProg)) <= 0) return (rc < 0);

    if (isServer > 0 || isManager > 0 || isProxy > 0 || isPeer > 0)
       eDest->Say("Config warning: role directive over-ridden by command line options.");
       else {isServer = xServ; isManager = xMan; isProxy = xProxy;
             isPeer   = xPeer; isSolo    = xSolo;
             if (myRole) free(myRole); myRole = strdup(role);
            }
    return 0;
}

/******************************************************************************/
/*                                x s c h e d                                 */
/******************************************************************************/

/* Function: xsched

   Purpose:  To parse directive: sched [cpu <p>] [io <p>] [runq <p>]
                                       [mem <p>] [pag <p>] [fuzz <p>]
                                       [maxload <p>] [refreset <sec>]

             <p>      is the percentage to include in the load as a value
                      between 0 and 100. For fuzz this is the largest
                      difference two load values may have to be treated equal.
                      maxload is the largest load allowed before server is
                      not selected. refreset is the minimum number of seconds
                      between reference counter resets.

   Type: Any, dynamic.

   Output: retc upon success or -EINVAL upon failure.
*/

int XrdOlbConfig::xsched(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val;
    int  i, ppp;
    static struct schedopts {const char *opname; int maxv; int *oploc;}
           scopts[] =
       {
        {"cpu",      100, &P_cpu},
        {"fuzz",     100, &P_fuzz},
        {"io",       100, &P_io},
        {"runq",     100, &P_load}, // Actually load, runq to avoid confusion
        {"mem",      100, &P_mem},
        {"pag",      100, &P_pag},
        {"maxload",  100, &MaxLoad},
        {"refreset", -1,  &RefReset}
       };
    int numopts = sizeof(scopts)/sizeof(struct schedopts);

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "sched option not specified"); return 1;}

    while (val)
          {for (i = 0; i < numopts; i++)
               if (!strcmp(val, scopts[i].opname))
                  {if (!(val = CFile.GetWord()))
                      {eDest->Emsg("Config", "sched ", scopts[i].opname,
                                   "argument not specified.");
                       return 1;
                      }
                   if (scopts[i].maxv < 0)
                      {if (XrdOuca2x::a2tm(*eDest,"sched value", val, &ppp, 0)) 
                          return 1;
                      }
                      else if (XrdOuca2x::a2i(*eDest,"sched value", val, &ppp,
                                              0, scopts[i].maxv)) return 1;
                   *scopts[i].oploc = ppp;
                   break;
                  }
           if (i >= numopts)
              eDest->Say("Config warning: ignoring invalid sched option '",val,"'.");
           val = CFile.GetWord();
          }

    return 0;
}

/******************************************************************************/
/*                                x s p a c e                                 */
/******************************************************************************/

/* Function: xspace

   Purpose:  To parse the directive: space [linger <num>] [[min] <min> [<hwm>]]
                                           [recalc <sec>]

             <num> Maximum number of times a server may be reselected without
                   a break. The default is 0.

             <min> Minimum free space need in bytes (or K, M, G) in a partition.
                   The default is 10G.

             <hwm> Bytes (or K, M,G) of free space needed when bytes falls below
                   <min> to requalify a server for selection.
                   The default is 11G.

             <sec> Number of seconds that must elapse before a disk free space
                   calculation will occur.

   Notes:   This is used by the manager and the server.

   Type: All, dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xspace(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val;
    int alinger = -1, arecalc = -1;
    long long minf = -1, hwm = -1;

    while((val = CFile.GetWord()))
      {    if (!strcmp("linger", val))
              {if (!(val = CFile.GetWord()))
                  {eDest->Emsg("Config", "linger value not specified"); return 1;}
               if (XrdOuca2x::a2i(*eDest,"linger",val,&alinger,0)) return 1;
              }
      else if (!strcmp("recalc", val))
              {if (!(val = CFile.GetWord()))
                  {eDest->Emsg("Config", "recalc value not specified"); return 1;}
               if (XrdOuca2x::a2i(*eDest,"recalc",val,&arecalc,1)) return 1;
              }
      else if (isdigit(*val) || (!strcmp("min", val) && (val = CFile.GetWord())) )
              {if (XrdOuca2x::a2sz(*eDest,"space minfree",val,&minf,0)) return 1;
               if ((val = CFile.GetWord()))
                  {if (isdigit(*val))
                       {if (XrdOuca2x::a2sz(*eDest,"space high watermark",
                                            val,&hwm,0)) return 1;
                       }
                      else CFile.RetToken();
                  } else break;
              }
       else {eDest->Emsg("Config", "invalid space parameters"); return 1;}
       }
    
    if (alinger < 0 && arecalc < 0 && minf < 0)
       {eDest->Emsg("Config", "no space values specified"); return 1;}

    if (alinger >= 0) DiskLinger = alinger;
    if (arecalc >= 0) DiskAsk    = arecalc;

    if (minf >= 0)
       {if (hwm < 0) DiskHWM = minf+1073741824;
           else if (hwm < minf) DiskHWM = minf + hwm;
                   else DiskHWM = hwm;
        DiskMin = minf / 1024;
        DiskHWM /= 1024;
       }
    return 0;
}

/******************************************************************************/
/*                                 x s u b s                                  */
/******************************************************************************/

/* Function: xsubs

   Purpose:  Parse the directive: subscribe <host>[+] [<port>] [if ...]

             <host> The dns name of the host that is the manager of this host.
                    If the host name ends with a plus, all addresses that are
                    associated with the hosts are subscribed to.

             <port> The port number to use for this host. The default comes
                    from the port directive.

             if     Apply the manager directive if "if" is true. See
                    XrdOucUtils:doIf() for "if" syntax.

   Notes:   Any number of subscribe directives can be given. The server will
            subscribe to all of the managers.

            The subscribe directive is depricated. The manager directive should
            be used as it applies to all components.

   Type: Server only, non-dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xsubs(XrdSysError *eDest, XrdOucStream &CFile)
{
    struct sockaddr InetAddr[8];
    XrdOucTList *tp = 0;
    char *val, *bval = 0, mbuff[1024];
    int i, port = 0;

    if (!isServer) return 0;
    eDest->Say("Config warning: 'subscribe' directive is deprecated; use 'all.manager' instead");

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("Config", "subscribe host not specified"); return 1;}
    strlcpy(mbuff, val, sizeof(mbuff));

    if ((val = CFile.GetWord()) && strcmp(val, "if"))
       {if (isdigit(*val))
           {if (XrdOuca2x::a2i(*eDest,"subscribe port",val,&port,1,65535))
               return 1;
           }
           else if (!(port = XrdNetDNS::getPort(val, "tcp")))
                   {eDest->Emsg("Config", "unable to find tcp service", val);
                    return 1;
                   }
       val = CFile.GetWord();
       }
       else if (!(port = PortTCP))
               {eDest->Emsg("Config","subscribe port not specified for",mbuff);
                return 1;
               }

    if (val)
       {if (strcmp(val, "if"))
           {eDest->Emsg("Config","expecting subscribe 'if' but",val,"found");
            return 1;
           }
        if ((i=XrdOucUtils::doIf(eDest,CFile,"subscribe directive",
                            myName,myInsName,myProg))<=0) return i < 0;
       }

    i = strlen(mbuff);
    if (mbuff[i-1] != '+') i = 0;
        else {bval = strdup(mbuff); mbuff[i-1] = '\0';
              if (!(i = XrdNetDNS::getHostAddr(mbuff, InetAddr, 8)))
                 {eDest->Emsg("Config","Subscribe host", mbuff, "not found");
                  free(bval); return 1;
                 }
             }

    do {if (i)
           {char *mp, mvBuff[1024];
            i--; //Greg had it right
            mp = XrdNetDNS::getHostName(InetAddr[i]);
            strlcpy(mbuff, mp, sizeof(mbuff)); free(mp);
            sprintf(mvBuff, "%s -> olb.subscribe ", bval);
            eDest->Emsg("Config", mvBuff, mbuff);
            if (isdigit(*mbuff))
               eDest->Say("Config warning: unable to reverse lookup ", mbuff);
           }
        tp = myManagers;
        while(tp) 
             if (strcmp(tp->text, mbuff) || tp->val != port) tp = tp->next;
                else {eDest->Say("Config warning: duplicate subscription to ",mbuff);
                      break;
                     }
        if (tp) break;
        myManagers = new XrdOucTList(mbuff, port, myManagers);
       } while(i);

    if (bval) free(bval);
    return tp != 0;
}
  
/******************************************************************************/
/*                                x t r a c e                                 */
/******************************************************************************/

/* Function: xtrace

   Purpose:  To parse the directive: trace <options>

   Type: Manager or Server, dynamic.

   Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xtrace(XrdSysError *eDest, XrdOucStream &CFile)
{
    char  *val;
    static struct traceopts {const char *opname; int opval;} tropts[] =
       {
        {"all",      TRACE_ALL},
        {"debug",    TRACE_Debug},
        {"defer",    TRACE_Defer},
        {"stage",    TRACE_Stage}
       };
    int i, neg, trval = 0, numopts = sizeof(tropts)/sizeof(struct traceopts);

    if (!(val = CFile.GetWord()))
       {eDest->Emsg("config", "trace option not specified"); return 1;}
    while (val)
         {if (!strcmp(val, "off")) trval = 0;
             else {if ((neg = (val[0] == '-' && val[1]))) val++;
                   for (i = 0; i < numopts; i++)
                       {if (!strcmp(val, tropts[i].opname))
                           {if (neg) trval &= ~tropts[i].opval;
                               else  trval |=  tropts[i].opval;
                            break;
                           }
                       }
                   if (i >= numopts)
                      eDest->Say("Config warning: ignoring invalid trace option '",val,"'.");
                  }
          val = CFile.GetWord();
         }

    Trace.What = trval;
    return 0;
}
  
/******************************************************************************/
/*                                  x x m i                                   */
/******************************************************************************/

/* Function: xxmi

   Purpose:  To parse the directive: xmilib <path> [<parms>]

             <path>    the SO path for the XrdOlbXmi plugin.
             <parms>   optional parms to be passed to the Xmi object

  Output: 0 upon success or !0 upon failure.
*/

int XrdOlbConfig::xxmi(XrdSysError *eDest, XrdOucStream &CFile)
{
    char *val, parms[1024];

// Get the path
//
   if (!(val = CFile.GetWord()) || !val[0])
      {eDest->Emsg("Config", "xmilib path not specified"); return 1;}

// Record the path
//
   if (XmiPath) free(XmiPath);
   XmiPath = strdup(val);

// Record any parms
//
   if (!CFile.GetRest(parms, sizeof(parms)))
      {eDest->Emsg("Config", "xmilib parameters too long"); return 1;}
   if (XmiParms) free(XmiParms);
   XmiParms = (*parms ? strdup(parms) : 0);

   return 0;
}
