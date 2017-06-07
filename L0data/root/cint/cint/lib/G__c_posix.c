/********************************************************
* cint/cint/lib/G__c_posix.c
********************************************************/
#include "cint/cint/lib/G__c_posix.h"
void G__c_reset_tagtable();
void G__set_c_environment() {
  G__add_compiledheader("cint/cint/lib/posix/exten.h");
  G__add_compiledheader("cint/cint/inc/cintdictversion.h");
  G__add_compiledheader("cint/cint/include/sys/types.h");
  G__add_compiledheader("cint/cint/lib/posix/posix.h");
  G__c_reset_tagtable();
}
int G__c_dllrev() { return(30051515); }

/* Setting up global function */
static int G__posix__0_0(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) open((char*) G__int(libp->para[0]), (int) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fcntl((int) G__int(libp->para[0]), (int) G__int(libp->para[1])
, (long) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) umask((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) opendir((char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) telldir((DIR*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fileno((FILE*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) readdir((DIR*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      seekdir((DIR*) G__int(libp->para[0]), (long) G__int(libp->para[1]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      rewinddir((DIR*) G__int(libp->para[0]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) closedir((DIR*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) stat((const char*) G__int(libp->para[0]), (struct stat*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) S_ISREG((mode_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) S_ISDIR((mode_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) S_ISCHR((mode_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) S_ISBLK((mode_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) S_ISFIFO((mode_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) uname((struct utsname*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) close((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) read((int) G__int(libp->para[0]), (void*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) write((int) G__int(libp->para[0]), (const void*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) dup((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_21(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) dup2((int) G__int(libp->para[0]), (int) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_22(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) pipe((int*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_23(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) alarm((unsigned int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_24(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) sleep((unsigned int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_25(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      usleep((unsigned long) G__int(libp->para[0]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_26(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) pause());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_27(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) chown((const char*) G__int(libp->para[0]), (uid_t) G__int(libp->para[1])
, (gid_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_28(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) chdir((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_29(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) getcwd((char*) G__int(libp->para[0]), (size_t) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_30(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 108, (long) sysconf((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_31(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) putenv((char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_32(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) getpid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_33(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) getppid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_34(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) setpgid((pid_t) G__int(libp->para[0]), (pid_t) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_35(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) getpgrp());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_36(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 107, (long) getuid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_37(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 107, (long) geteuid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_38(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 107, (long) getgid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_39(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 107, (long) getegid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_40(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) setuid((uid_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_41(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) cuserid((char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_42(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) getlogin());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_43(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) ctermid((char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_44(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) ttyname((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_45(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) link((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_46(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) unlink((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_47(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) rmdir((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_48(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) mkdir((const char*) G__int(libp->para[0]), (mode_t) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_49(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fork());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_50(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 108, (long) time((time_t*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_51(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) S_ISLNK((mode_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_52(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) S_ISSOCK((mode_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_53(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fchown((int) G__int(libp->para[0]), (uid_t) G__int(libp->para[1])
, (gid_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_54(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fchdir((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_55(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) get_current_dir_name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_56(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) getpgid((pid_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_57(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) setpgrp());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_58(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) symlink((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_59(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) vfork());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__posix__0_60(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isDirectory((struct dirent*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}


/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* typedef information setup/
*********************************************************/
void G__c_setup_typetable() {

   /* Setting up typedef entry */
   G__search_typename2("pid_t",105,-1,0,-1);
   G__setnewtype(-2,NULL,0);
   G__search_typename2("gid_t",107,-1,0,-1);
   G__setnewtype(-2,NULL,0);
   G__search_typename2("uid_t",107,-1,0,-1);
   G__setnewtype(-2,NULL,0);
   G__search_typename2("mode_t",107,-1,0,-1);
   G__setnewtype(-2,NULL,0);
   G__search_typename2("time_t",108,-1,0,-1);
   G__setnewtype(-2,NULL,0);
   G__search_typename2("umode_t",107,-1,0,-1);
   G__setnewtype(-2,NULL,0);
   G__search_typename2("DIR",117,G__get_linked_tagnum(&G__LN___dirstream),0,-1);
   G__setnewtype(-2,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* struct dirent */
static void G__setup_memvardirent(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__LN_dirent));
   { struct dirent *p; p=(struct dirent*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->d_ino)-(long)(p)),108,0,0,-1,-1,-1,1,"d_ino=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->d_reclen)-(long)(p)),114,0,0,-1,-1,-1,1,"d_reclen=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->d_name)-(long)(p)),99,0,0,-1,-1,-1,1,"d_name[129]=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* struct stat */
static void G__setup_memvarstat(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__LN_stat));
   { struct stat *p; p=(struct stat*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->st_dev)-(long)(p)),117,0,0,G__get_linked_tagnum(&G__LN_dev_t),G__defined_typename("dev_t"),-1,1,"st_dev=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_ino)-(long)(p)),107,0,0,-1,G__defined_typename("ino_t"),-1,1,"st_ino=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_mode)-(long)(p)),107,0,0,-1,G__defined_typename("umode_t"),-1,1,"st_mode=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_nlink)-(long)(p)),107,0,0,-1,G__defined_typename("nlink_t"),-1,1,"st_nlink=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_uid)-(long)(p)),107,0,0,-1,G__defined_typename("uid_t"),-1,1,"st_uid=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_gid)-(long)(p)),107,0,0,-1,G__defined_typename("gid_t"),-1,1,"st_gid=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_rdev)-(long)(p)),117,0,0,G__get_linked_tagnum(&G__LN_dev_t),G__defined_typename("dev_t"),-1,1,"st_rdev=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_size)-(long)(p)),108,0,0,-1,G__defined_typename("off_t"),-1,1,"st_size=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_blksize)-(long)(p)),107,0,0,-1,-1,-1,1,"st_blksize=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_blocks)-(long)(p)),107,0,0,-1,-1,-1,1,"st_blocks=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_atime)-(long)(p)),108,0,0,-1,G__defined_typename("time_t"),-1,1,"st_atime=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_mtime)-(long)(p)),108,0,0,-1,G__defined_typename("time_t"),-1,1,"st_mtime=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->st_ctime)-(long)(p)),108,0,0,-1,G__defined_typename("time_t"),-1,1,"st_ctime=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* struct utsname */
static void G__setup_memvarutsname(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__LN_utsname));
   { struct utsname *p; p=(struct utsname*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->sysname)-(long)(p)),99,0,0,-1,-1,-1,1,"sysname[65]=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->nodename)-(long)(p)),99,0,0,-1,-1,-1,1,"nodename[65]=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->release)-(long)(p)),99,0,0,-1,-1,-1,1,"release[65]=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->version)-(long)(p)),99,0,0,-1,-1,-1,1,"version[65]=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->machine)-(long)(p)),99,0,0,-1,-1,-1,1,"machine[65]=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

void G__c_setup_memvar() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"__GNUC__=4",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"__GNUC_MINOR__=4",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__EXTEN_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__POSIX_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"NAME_MAX=128",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"SYS_NMLN=65",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__GLIBC_=212",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"INCLUDE_CINTDICTVERSION=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CINTDICTVERSION=20090608",1,(char*)NULL);

   G__resetglobalenv();
}
void G__c_setup_global() {
  G__cpp_setup_global0();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

   G__memfunc_setup("open", 434, G__posix__0_0, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 0 - - i - - 0 - flags", (char*) NULL
#ifndef open
, (void*) open, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fcntl", 535, G__posix__0_1, 105, -1, -1, 0, 3, 1, 1, 0, 
"i - - 0 - fd i - - 0 - cmd l - - 0 - arg", (char*) NULL
#ifndef fcntl
, (void*) fcntl, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("umask", 545, G__posix__0_2, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - mask", (char*) NULL
#ifndef umask
, (void*) umask, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("opendir", 753, G__posix__0_3, 85, G__get_linked_tagnum(&G__LN___dirstream), G__defined_typename("DIR"), 0, 1, 1, 1, 0, "C - - 0 - name", (char*) NULL
#ifndef opendir
, (void*) opendir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("telldir", 752, G__posix__0_4, 105, -1, -1, 0, 1, 1, 1, 0, "U '__dirstream' 'DIR' 0 - dir", (char*) NULL
#ifndef telldir
, (void*) telldir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fileno", 637, G__posix__0_5, 105, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - stream", (char*) NULL
#ifndef fileno
, (void*) fileno, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("readdir", 731, G__posix__0_6, 85, G__get_linked_tagnum(&G__LN_dirent), -1, 0, 1, 1, 1, 0, "U '__dirstream' 'DIR' 0 - dir", (char*) NULL
#ifndef readdir
, (void*) readdir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("seekdir", 743, G__posix__0_7, 121, -1, -1, 0, 2, 1, 1, 0, 
"U '__dirstream' 'DIR' 0 - dir l - - 0 - loc", (char*) NULL
#ifndef seekdir
, (void*) seekdir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("rewinddir", 968, G__posix__0_8, 121, -1, -1, 0, 1, 1, 1, 0, "U '__dirstream' 'DIR' 0 - dir", (char*) NULL
#ifndef rewinddir
, (void*) rewinddir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("closedir", 853, G__posix__0_9, 105, -1, -1, 0, 1, 1, 1, 0, "U '__dirstream' 'DIR' 0 - dirp", (char*) NULL
#ifndef closedir
, (void*) closedir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("stat", 444, G__posix__0_10, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - filename U 'stat' - 0 - buf", (char*) NULL
#ifndef stat
, (void*) stat, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("S_ISREG", 556, G__posix__0_11, 105, -1, -1, 0, 1, 1, 1, 0, "k - 'mode_t' 0 - m", (char*) NULL
#ifndef S_ISREG
, (void*) S_ISREG, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("S_ISDIR", 557, G__posix__0_12, 105, -1, -1, 0, 1, 1, 1, 0, "k - 'mode_t' 0 - m", (char*) NULL
#ifndef S_ISDIR
, (void*) S_ISDIR, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("S_ISCHR", 555, G__posix__0_13, 105, -1, -1, 0, 1, 1, 1, 0, "k - 'mode_t' 0 - m", (char*) NULL
#ifndef S_ISCHR
, (void*) S_ISCHR, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("S_ISBLK", 551, G__posix__0_14, 105, -1, -1, 0, 1, 1, 1, 0, "k - 'mode_t' 0 - m", (char*) NULL
#ifndef S_ISBLK
, (void*) S_ISBLK, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("S_ISFIFO", 626, G__posix__0_15, 105, -1, -1, 0, 1, 1, 1, 0, "k - 'mode_t' 0 - m", (char*) NULL
#ifndef S_ISFIFO
, (void*) S_ISFIFO, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("uname", 534, G__posix__0_16, 105, -1, -1, 0, 1, 1, 1, 0, "U 'utsname' - 0 - buf", (char*) NULL
#ifndef uname
, (void*) uname, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("close", 534, G__posix__0_17, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - fd", (char*) NULL
#ifndef close
, (void*) close, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("read", 412, G__posix__0_18, 105, -1, G__defined_typename("ssize_t"), 0, 3, 1, 1, 0, 
"i - - 0 - fd Y - - 0 - buf h - 'size_t' 0 - nbytes", (char*) NULL
#ifndef read
, (void*) read, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("write", 555, G__posix__0_19, 105, -1, G__defined_typename("ssize_t"), 0, 3, 1, 1, 0, 
"i - - 0 - fd Y - - 10 - buf h - 'size_t' 0 - n", (char*) NULL
#ifndef write
, (void*) write, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("dup", 329, G__posix__0_20, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - oldfd", (char*) NULL
#ifndef dup
, (void*) dup, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("dup2", 379, G__posix__0_21, 105, -1, -1, 0, 2, 1, 1, 0, 
"i - - 0 - oldfd i - - 0 - newfd", (char*) NULL
#ifndef dup2
, (void*) dup2, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("pipe", 430, G__posix__0_22, 105, -1, -1, 0, 1, 1, 1, 0, "I - - 0 - filedes", (char*) NULL
#ifndef pipe
, (void*) pipe, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("alarm", 525, G__posix__0_23, 104, -1, -1, 0, 1, 1, 1, 0, "h - - 0 - seconds", (char*) NULL
#ifndef alarm
, (void*) alarm, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("sleep", 537, G__posix__0_24, 104, -1, -1, 0, 1, 1, 1, 0, "h - - 0 - seconds", (char*) NULL
#ifndef sleep
, (void*) sleep, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("usleep", 654, G__posix__0_25, 121, -1, -1, 0, 1, 1, 1, 0, "k - - 0 - usec", (char*) NULL
#ifndef usleep
, (void*) usleep, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("pause", 542, G__posix__0_26, 105, -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef pause
, (void*) pause, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("chown", 543, G__posix__0_27, 105, -1, -1, 0, 3, 1, 1, 0, 
"C - - 10 - path k - 'uid_t' 0 - owner k - 'gid_t' 0 - group", (char*) NULL
#ifndef chown
, (void*) chown, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("chdir", 522, G__posix__0_28, 105, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - path", (char*) NULL
#ifndef chdir
, (void*) chdir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getcwd", 638, G__posix__0_29, 67, -1, -1, 0, 2, 1, 1, 0, 
"C - - 0 - buf h - 'size_t' 0 - size", (char*) NULL
#ifndef getcwd
, (void*) getcwd, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("sysconf", 773, G__posix__0_30, 108, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - name", (char*) NULL
#ifndef sysconf
, (void*) sysconf, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("putenv", 674, G__posix__0_31, 105, -1, -1, 0, 1, 1, 1, 0, "C - - 0 - string", (char*) NULL
#ifndef putenv
, (void*) putenv, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getpid", 637, G__posix__0_32, 105, -1, G__defined_typename("pid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef getpid
, (void*) getpid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getppid", 749, G__posix__0_33, 105, -1, G__defined_typename("pid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef getppid
, (void*) getppid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("setpgid", 752, G__posix__0_34, 105, -1, -1, 0, 2, 1, 1, 0, 
"i - 'pid_t' 0 - pid i - 'pid_t' 0 - pgid", (char*) NULL
#ifndef setpgid
, (void*) setpgid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getpgrp", 761, G__posix__0_35, 105, -1, G__defined_typename("pid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef getpgrp
, (void*) getpgrp, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getuid", 642, G__posix__0_36, 107, -1, G__defined_typename("uid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef getuid
, (void*) getuid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("geteuid", 743, G__posix__0_37, 107, -1, G__defined_typename("uid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef geteuid
, (void*) geteuid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getgid", 628, G__posix__0_38, 107, -1, G__defined_typename("gid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef getgid
, (void*) getgid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getegid", 729, G__posix__0_39, 107, -1, G__defined_typename("gid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef getegid
, (void*) getegid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("setuid", 654, G__posix__0_40, 105, -1, -1, 0, 1, 1, 1, 0, "k - 'uid_t' 0 - uid", (char*) NULL
#ifndef setuid
, (void*) setuid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("cuserid", 751, G__posix__0_41, 67, -1, -1, 0, 1, 1, 1, 0, "C - - 0 - string", (char*) NULL
#ifndef cuserid
, (void*) cuserid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getlogin", 857, G__posix__0_42, 67, -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef getlogin
, (void*) getlogin, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ctermid", 744, G__posix__0_43, 67, -1, -1, 0, 1, 1, 1, 0, "C - - 0 - s", (char*) NULL
#ifndef ctermid
, (void*) ctermid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ttyname", 770, G__posix__0_44, 67, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - desc", (char*) NULL
#ifndef ttyname
, (void*) ttyname, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("link", 430, G__posix__0_45, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - oldpath C - - 10 - newpath", (char*) NULL
#ifndef link
, (void*) link, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("unlink", 657, G__posix__0_46, 105, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - pathname", (char*) NULL
#ifndef unlink
, (void*) unlink, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("rmdir", 542, G__posix__0_47, 105, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - path", (char*) NULL
#ifndef rmdir
, (void*) rmdir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("mkdir", 535, G__posix__0_48, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - pathname k - 'mode_t' 0 - mode", (char*) NULL
#ifndef mkdir
, (void*) mkdir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fork", 434, G__posix__0_49, 105, -1, G__defined_typename("pid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef fork
, (void*) fork, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("time", 431, G__posix__0_50, 108, -1, G__defined_typename("time_t"), 0, 1, 1, 1, 0, "L - 'time_t' 0 - t", (char*) NULL
#ifndef time
, (void*) time, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("S_ISLNK", 563, G__posix__0_51, 105, -1, -1, 0, 1, 1, 1, 0, "k - 'mode_t' 0 - m", (char*) NULL
#ifndef S_ISLNK
, (void*) S_ISLNK, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("S_ISSOCK", 638, G__posix__0_52, 105, -1, -1, 0, 1, 1, 1, 0, "k - 'mode_t' 0 - m", (char*) NULL
#ifndef S_ISSOCK
, (void*) S_ISSOCK, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fchown", 645, G__posix__0_53, 105, -1, -1, 0, 3, 1, 1, 0, 
"i - - 0 - fd k - 'uid_t' 0 - owner k - 'gid_t' 0 - group", (char*) NULL
#ifndef fchown
, (void*) fchown, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fchdir", 624, G__posix__0_54, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - fd", (char*) NULL
#ifndef fchdir
, (void*) fchdir, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("get_current_dir_name", 2112, G__posix__0_55, 67, -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef get_current_dir_name
, (void*) get_current_dir_name, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getpgid", 740, G__posix__0_56, 105, -1, G__defined_typename("pid_t"), 0, 1, 1, 1, 0, "i - 'pid_t' 0 - pid", (char*) NULL
#ifndef getpgid
, (void*) getpgid, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("setpgrp", 773, G__posix__0_57, 105, -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef setpgrp
, (void*) setpgrp, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("symlink", 775, G__posix__0_58, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - oldpath C - - 10 - newpath", (char*) NULL
#ifndef symlink
, (void*) symlink, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("vfork", 552, G__posix__0_59, 105, -1, G__defined_typename("pid_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef vfork
, (void*) vfork, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isDirectory", 1169, G__posix__0_60, 105, -1, -1, 0, 1, 1, 1, 0, "U 'dirent' - 0 - pd", (char*) NULL
#ifndef isDirectory
, (void*) isDirectory, 0);
#else
, (void*) NULL, 0);
#endif

   G__resetifuncposition();
}

void G__c_setup_func() {
  G__cpp_setup_func0();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__LN_dev_t = { "dev_t" , 115 , -1 };
G__linked_taginfo G__LN___dirstream = { "__dirstream" , 115 , -1 };
G__linked_taginfo G__LN_dirent = { "dirent" , 115 , -1 };
G__linked_taginfo G__LN_stat = { "stat" , 115 , -1 };
G__linked_taginfo G__LN_utsname = { "utsname" , 115 , -1 };

/* Reset class/struct taginfo */
void G__c_reset_tagtable() {
  G__LN_dev_t.tagnum = -1 ;
  G__LN___dirstream.tagnum = -1 ;
  G__LN_dirent.tagnum = -1 ;
  G__LN_stat.tagnum = -1 ;
  G__LN_utsname.tagnum = -1 ;
}


void G__c_setup_tagtable() {

   /* Setting up class,struct,union tag entry */
   G__tagtable_setup(G__get_linked_tagnum(&G__LN___dirstream),0,-2,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__LN_dirent),sizeof(struct dirent),-2,0,(char*)NULL,G__setup_memvardirent,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__LN_stat),sizeof(struct stat),-2,0,(char*)NULL,G__setup_memvarstat,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__LN_utsname),sizeof(struct utsname),-2,0,(char*)NULL,G__setup_memvarutsname,NULL);
}
void G__c_setup() {
  G__check_setup_version(30051515,"G__c_setup()");
  G__set_c_environment();
  G__c_setup_tagtable();

  G__c_setup_typetable();

  G__c_setup_memvar();

  G__c_setup_global();
  G__c_setup_func();
  return;
}
