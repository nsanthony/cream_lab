/********************************************************
* cint/cint/lib/G__c_stdfunc.c
********************************************************/
#include "cint/cint/lib/G__c_stdfunc.h"
void G__c_reset_tagtable();
void G__set_c_environment() {
  G__add_compiledheader("cint/cint/lib/stdstrct/stdfunc.h");
  G__add_compiledheader("cint/cint/inc/cintdictversion.h");
  G__c_reset_tagtable();
}
int G__c_dllrev() { return(30051515); }

/* Setting up global function */
static int G__stdfunc__0_0(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      abort();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) abs((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) acos((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) asin((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) asctime((struct tm*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) atan((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) atan2((double) G__double(libp->para[0]), (double) G__double(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) atof((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) atoi((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 108, (long) atol((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 89, (long) calloc((size_t) G__int(libp->para[0]), (size_t) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ceil((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      clearerr((FILE*) G__int(libp->para[0]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 107, (long) clock());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) cos((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) cosh((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) ctime((time_t*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) difftime((time_t) G__int(libp->para[0]), (time_t) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__alloc_tempobject_val(result7);
      result7->obj.i = G__gettempbufpointer();
      result7->ref = G__gettempbufpointer();
      *((div_t *) result7->obj.i) = div((int) G__int(libp->para[0]), (int) G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) exp((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) fabs((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_21(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fclose((FILE*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_22(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) feof((FILE*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_23(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) ferror((FILE*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_24(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fflush((FILE*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_25(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fgetc((FILE*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_26(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) fgets((char*) G__int(libp->para[0]), (int) G__int(libp->para[1])
, (FILE*) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_27(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) floor((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_28(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) fmod((double) G__double(libp->para[0]), (double) G__double(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_29(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 69, (long) fopen((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_30(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fputc((int) G__int(libp->para[0]), (FILE*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_31(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fputs((const char*) G__int(libp->para[0]), (FILE*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_32(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) fread((void*) G__int(libp->para[0]), (size_t) G__int(libp->para[1])
, (size_t) G__int(libp->para[2]), (FILE*) G__int(libp->para[3])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_33(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      free((void*) G__int(libp->para[0]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_34(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 69, (long) freopen((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])
, (FILE*) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_35(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) frexp((double) G__double(libp->para[0]), (int*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_36(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) fseek((FILE*) G__int(libp->para[0]), (long) G__int(libp->para[1])
, (int) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_37(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 108, (long) ftell((FILE*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_38(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) fwrite((void*) G__int(libp->para[0]), (size_t) G__int(libp->para[1])
, (size_t) G__int(libp->para[2]), (FILE*) G__int(libp->para[3])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_39(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) getc((FILE*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_40(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) getchar());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_41(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) getenv((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_42(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) gets((char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_43(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) gmtime((time_t*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_44(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isalnum((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_45(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isalpha((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_46(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) iscntrl((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_47(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isdigit((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_48(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isgraph((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_49(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) islower((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_50(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isprint((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_51(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) ispunct((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_52(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isspace((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_53(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isupper((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_54(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) isxdigit((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_55(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 108, (long) labs((long) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_56(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ldexp((double) G__double(libp->para[0]), (int) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_57(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__alloc_tempobject_val(result7);
      result7->obj.i = G__gettempbufpointer();
      result7->ref = G__gettempbufpointer();
      *((ldiv_t *) result7->obj.i) = ldiv((long) G__int(libp->para[0]), (long) G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_58(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) localeconv());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_59(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) localtime((time_t*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_60(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) log((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_61(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) log10((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_62(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 89, (long) malloc((size_t) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_63(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) mblen((const char*) G__int(libp->para[0]), (size_t) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_64(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) mbstowcs((wchar_t*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_65(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) mbtowc((wchar_t*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_66(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 89, (long) memchr((void*) G__int(libp->para[0]), (int) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_67(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) memcmp((void*) G__int(libp->para[0]), (void*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_68(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 89, (long) memcpy((void*) G__int(libp->para[0]), (void*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_69(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 89, (long) memmove((void*) G__int(libp->para[0]), (void*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_70(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 89, (long) memset((void*) G__int(libp->para[0]), (int) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_71(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 108, (long) mktime((struct tm*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_72(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) modf((double) G__double(libp->para[0]), (double*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_73(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      perror((const char*) G__int(libp->para[0]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_74(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) pow((double) G__double(libp->para[0]), (double) G__double(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_75(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) putc((int) G__int(libp->para[0]), (FILE*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_76(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) putchar((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_77(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) puts((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_78(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) raise((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_79(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) rand());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_80(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 89, (long) realloc((void*) G__int(libp->para[0]), (size_t) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_81(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) remove((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_82(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) rename((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_83(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      rewind((FILE*) G__int(libp->para[0]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_84(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      setbuf((FILE*) G__int(libp->para[0]), (char*) G__int(libp->para[1]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_85(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) setlocale((int) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_86(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) setvbuf((FILE*) G__int(libp->para[0]), (char*) G__int(libp->para[1])
, (int) G__int(libp->para[2]), (size_t) G__int(libp->para[3])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_87(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) sin((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_88(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) sinh((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_89(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) sqrt((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_90(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      srand((unsigned int) G__int(libp->para[0]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_91(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strcat((char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_92(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strchr((const char*) G__int(libp->para[0]), (int) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_93(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) strcmp((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_94(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) strcoll((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_95(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strcpy((char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_96(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) strcspn((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_97(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strerror((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_98(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) strftime((char*) G__int(libp->para[0]), (size_t) G__int(libp->para[1])
, (const char*) G__int(libp->para[2]), (struct tm*) G__int(libp->para[3])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_99(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) strlen((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_100(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strncat((char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_101(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) strncmp((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_102(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strncpy((char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_103(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strpbrk((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_104(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strrchr((const char*) G__int(libp->para[0]), (int) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_105(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) strspn((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_106(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strstr((const char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_107(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) strtod((const char*) G__int(libp->para[0]), (char**) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_108(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) strtok((char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_109(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 108, (long) strtol((const char*) G__int(libp->para[0]), (char**) G__int(libp->para[1])
, (int) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_110(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 107, (long) strtoul((const char*) G__int(libp->para[0]), (char**) G__int(libp->para[1])
, (int) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_111(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) strxfrm((char*) G__int(libp->para[0]), (const char*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_112(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) system((const char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_113(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) tan((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_114(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) tanh((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_115(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 108, (long) time((time_t*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_116(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 69, (long) tmpfile());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_117(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) tmpnam((char*) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_118(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) tolower((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_119(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) toupper((int) G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_120(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) ungetc((int) G__int(libp->para[0]), (FILE*) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_121(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 104, (long) wcstombs((char*) G__int(libp->para[0]), (wchar_t*) G__int(libp->para[1])
, (size_t) G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_122(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) wctomb((char*) G__int(libp->para[0]), (wchar_t) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__stdfunc__0_123(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      exit((int) G__int(libp->para[0]));
      G__setnull(result7);
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
   G__search_typename2("wchar_t",104,-1,0,-1);
   G__setnewtype(-2,NULL,0);
   G__search_typename2("clock_t",107,-1,0,-1);
   G__setnewtype(-2,NULL,0);
   G__search_typename2("time_t",108,-1,0,-1);
   G__setnewtype(-2,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */
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
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__STDFUNC=0",1,(char*)NULL);
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

   G__memfunc_setup("abort", 536, G__stdfunc__0_0, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef abort
, (void*) abort, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("abs", 310, G__stdfunc__0_1, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - n", (char*) NULL
#ifndef abs
, (void*) abs, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("acos", 422, G__stdfunc__0_2, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - arg", (char*) NULL
#ifndef acos
, (void*) acos, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("asin", 427, G__stdfunc__0_3, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - arg", (char*) NULL
#ifndef asin
, (void*) asin, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("asctime", 742, G__stdfunc__0_4, 67, -1, -1, 0, 1, 1, 1, 0, "U 'tm' - 0 - timestruct", (char*) NULL
#ifndef asctime
, (void*) asctime, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("atan", 420, G__stdfunc__0_5, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - arg", (char*) NULL
#ifndef atan
, (void*) atan, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("atan2", 470, G__stdfunc__0_6, 100, -1, -1, 0, 2, 1, 1, 0, 
"d - - 0 - num d - - 0 - den", (char*) NULL
#ifndef atan2
, (void*) atan2, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("atof", 426, G__stdfunc__0_7, 100, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - string", (char*) NULL
#ifndef atof
, (void*) atof, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("atoi", 429, G__stdfunc__0_8, 105, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - string", (char*) NULL
#ifndef atoi
, (void*) atoi, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("atol", 432, G__stdfunc__0_9, 108, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - string", (char*) NULL
#ifndef atol
, (void*) atol, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("calloc", 622, G__stdfunc__0_10, 89, -1, -1, 0, 2, 1, 1, 0, 
"h - 'size_t' 0 - count h - 'size_t' 0 - size", (char*) NULL
#ifndef calloc
, (void*) calloc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ceil", 413, G__stdfunc__0_11, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - z", (char*) NULL
#ifndef ceil
, (void*) ceil, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("clearerr", 848, G__stdfunc__0_12, 121, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef clearerr
, (void*) clearerr, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("clock", 524, G__stdfunc__0_13, 107, -1, G__defined_typename("clock_t"), 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef clock
, (void*) clock, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("cos", 325, G__stdfunc__0_14, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - radian", (char*) NULL
#ifndef cos
, (void*) cos, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("cosh", 429, G__stdfunc__0_15, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - value", (char*) NULL
#ifndef cosh
, (void*) cosh, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ctime", 530, G__stdfunc__0_16, 67, -1, -1, 0, 1, 1, 1, 0, "L - 'time_t' 0 - timeptr", (char*) NULL
#ifndef ctime
, (void*) ctime, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("difftime", 840, G__stdfunc__0_17, 100, -1, -1, 0, 2, 1, 1, 0, 
"l - 'time_t' 0 - newtime l - 'time_t' 0 - oldtime", (char*) NULL
#ifndef difftime
, (void*) difftime, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("div", 323, G__stdfunc__0_18, 117, G__get_linked_tagnum(&G__LN_dAdiv_t), G__defined_typename("div_t"), 0, 2, 1, 1, 0, 
"i - - 0 - numerator i - - 0 - denominator", (char*) NULL
#ifndef div
, (void*) div, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("exp", 333, G__stdfunc__0_19, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - z", (char*) NULL
#ifndef exp
, (void*) exp, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fabs", 412, G__stdfunc__0_20, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - z", (char*) NULL
#ifndef fabs
, (void*) fabs, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fclose", 636, G__stdfunc__0_21, 105, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef fclose
, (void*) fclose, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("feof", 416, G__stdfunc__0_22, 105, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef feof
, (void*) feof, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ferror", 656, G__stdfunc__0_23, 105, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef ferror
, (void*) ferror, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fflush", 648, G__stdfunc__0_24, 105, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef fflush
, (void*) fflush, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fgetc", 521, G__stdfunc__0_25, 105, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef fgetc
, (void*) fgetc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fgets", 537, G__stdfunc__0_26, 67, -1, -1, 0, 3, 1, 1, 0, 
"C - - 0 - string i - - 0 - n E - - 0 - fp", (char*) NULL
#ifndef fgets
, (void*) fgets, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("floor", 546, G__stdfunc__0_27, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - z", (char*) NULL
#ifndef floor
, (void*) floor, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fmod", 422, G__stdfunc__0_28, 100, -1, -1, 0, 2, 1, 1, 0, 
"d - - 0 - number d - - 0 - divisor", (char*) NULL
#ifndef fmod
, (void*) fmod, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fopen", 536, G__stdfunc__0_29, 69, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - file C - - 10 - mode", (char*) NULL
#ifndef fopen
, (void*) fopen, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fputc", 546, G__stdfunc__0_30, 105, -1, -1, 0, 2, 1, 1, 0, 
"i - - 0 - character E - - 0 - fp", (char*) NULL
#ifndef fputc
, (void*) fputc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fputs", 562, G__stdfunc__0_31, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - string E - - 0 - fp", (char*) NULL
#ifndef fputs
, (void*) fputs, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fread", 514, G__stdfunc__0_32, 104, -1, G__defined_typename("size_t"), 0, 4, 1, 1, 0, 
"Y - - 0 - buffer h - 'size_t' 0 - size h - 'size_t' 0 - n E - - 0 - fp", (char*) NULL
#ifndef fread
, (void*) fread, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("free", 418, G__stdfunc__0_33, 121, -1, -1, 0, 1, 1, 1, 0, "Y - - 0 - ptr", (char*) NULL
#ifndef free
, (void*) free, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("freopen", 751, G__stdfunc__0_34, 69, -1, -1, 0, 3, 1, 1, 0, 
"C - - 10 - file C - - 10 - mode E - - 0 - fp", (char*) NULL
#ifndef freopen
, (void*) freopen, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("frexp", 549, G__stdfunc__0_35, 100, -1, -1, 0, 2, 1, 1, 0, 
"d - - 0 - real I - - 0 - exp1", (char*) NULL
#ifndef frexp
, (void*) frexp, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fseek", 526, G__stdfunc__0_36, 105, -1, -1, 0, 3, 1, 1, 0, 
"E - - 0 - fp l - - 0 - offset i - - 0 - whence", (char*) NULL
#ifndef fseek
, (void*) fseek, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ftell", 535, G__stdfunc__0_37, 108, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef ftell
, (void*) ftell, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("fwrite", 657, G__stdfunc__0_38, 104, -1, G__defined_typename("size_t"), 0, 4, 1, 1, 0, 
"Y - - 0 - buffer h - 'size_t' 0 - size h - 'size_t' 0 - n E - - 0 - fp", (char*) NULL
#ifndef fwrite
, (void*) fwrite, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getc", 419, G__stdfunc__0_39, 105, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef getc
, (void*) getc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getchar", 734, G__stdfunc__0_40, 105, -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef getchar
, (void*) getchar, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("getenv", 649, G__stdfunc__0_41, 67, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - variable", (char*) NULL
#ifndef getenv
, (void*) getenv, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("gets", 435, G__stdfunc__0_42, 67, -1, -1, 0, 1, 1, 1, 0, "C - - 0 - buffer", (char*) NULL
#ifndef gets
, (void*) gets, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("gmtime", 643, G__stdfunc__0_43, 85, G__get_linked_tagnum(&G__LN_tm), -1, 0, 1, 1, 1, 0, "L - 'time_t' 0 - caltime", (char*) NULL
#ifndef gmtime
, (void*) gmtime, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isalnum", 761, G__stdfunc__0_44, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef isalnum
, (void*) isalnum, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isalpha", 738, G__stdfunc__0_45, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef isalpha
, (void*) isalpha, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("iscntrl", 767, G__stdfunc__0_46, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef iscntrl
, (void*) iscntrl, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isdigit", 749, G__stdfunc__0_47, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef isdigit
, (void*) isdigit, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isgraph", 750, G__stdfunc__0_48, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef isgraph
, (void*) isgraph, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("islower", 773, G__stdfunc__0_49, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef islower
, (void*) islower, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isprint", 777, G__stdfunc__0_50, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef isprint
, (void*) isprint, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ispunct", 774, G__stdfunc__0_51, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef ispunct
, (void*) ispunct, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isspace", 744, G__stdfunc__0_52, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef isspace
, (void*) isspace, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isupper", 776, G__stdfunc__0_53, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef isupper
, (void*) isupper, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("isxdigit", 869, G__stdfunc__0_54, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef isxdigit
, (void*) isxdigit, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("labs", 418, G__stdfunc__0_55, 108, -1, -1, 0, 1, 1, 1, 0, "l - - 0 - n", (char*) NULL
#ifndef labs
, (void*) labs, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ldexp", 541, G__stdfunc__0_56, 100, -1, -1, 0, 2, 1, 1, 0, 
"d - - 0 - number i - - 0 - n", (char*) NULL
#ifndef ldexp
, (void*) ldexp, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ldiv", 431, G__stdfunc__0_57, 117, G__get_linked_tagnum(&G__LN_dAldiv_t), G__defined_typename("ldiv_t"), 0, 2, 1, 1, 0, 
"l - - 0 - numerator l - - 0 - denominator", (char*) NULL
#ifndef ldiv
, (void*) ldiv, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("localeconv", 1062, G__stdfunc__0_58, 85, G__get_linked_tagnum(&G__LN_lconv), -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef localeconv
, (void*) localeconv, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("localtime", 954, G__stdfunc__0_59, 85, G__get_linked_tagnum(&G__LN_tm), -1, 0, 1, 1, 1, 0, "L - 'time_t' 0 - timeptr", (char*) NULL
#ifndef localtime
, (void*) localtime, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("log", 322, G__stdfunc__0_60, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - z", (char*) NULL
#ifndef log
, (void*) log, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("log10", 419, G__stdfunc__0_61, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - z", (char*) NULL
#ifndef log10
, (void*) log10, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("malloc", 632, G__stdfunc__0_62, 89, -1, -1, 0, 1, 1, 1, 0, "h - 'size_t' 0 - size", (char*) NULL
#ifndef malloc
, (void*) malloc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("mblen", 526, G__stdfunc__0_63, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - address h - 'size_t' 0 - number", (char*) NULL
#ifndef mblen
, (void*) mblen, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("mbstowcs", 882, G__stdfunc__0_64, 104, -1, G__defined_typename("size_t"), 0, 3, 1, 1, 0, 
"H - 'wchar_t' 0 - widechar C - - 10 - multibyte h - 'size_t' 0 - number", (char*) NULL
#ifndef mbstowcs
, (void*) mbstowcs, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("mbtowc", 652, G__stdfunc__0_65, 105, -1, -1, 0, 3, 1, 1, 0, 
"H - 'wchar_t' 0 - charptr C - - 10 - address h - 'size_t' 0 - number", (char*) NULL
#ifndef mbtowc
, (void*) mbtowc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("memchr", 636, G__stdfunc__0_66, 89, -1, -1, 0, 3, 1, 1, 0, 
"Y - - 0 - region i - - 0 - character h - 'size_t' 0 - n", (char*) NULL
#ifndef memchr
, (void*) memchr, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("memcmp", 639, G__stdfunc__0_67, 105, -1, -1, 0, 3, 1, 1, 0, 
"Y - - 0 - region1 Y - - 0 - region2 h - 'size_t' 0 - count", (char*) NULL
#ifndef memcmp
, (void*) memcmp, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("memcpy", 651, G__stdfunc__0_68, 89, -1, -1, 0, 3, 1, 1, 0, 
"Y - - 0 - region1 Y - - 0 - region2 h - 'size_t' 0 - n", (char*) NULL
#ifndef memcpy
, (void*) memcpy, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("memmove", 758, G__stdfunc__0_69, 89, -1, -1, 0, 3, 1, 1, 0, 
"Y - - 0 - region1 Y - - 0 - region2 h - 'size_t' 0 - count", (char*) NULL
#ifndef memmove
, (void*) memmove, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("memset", 651, G__stdfunc__0_70, 89, -1, -1, 0, 3, 1, 1, 0, 
"Y - - 0 - buffer i - - 0 - character h - 'size_t' 0 - n", (char*) NULL
#ifndef memset
, (void*) memset, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("mktime", 647, G__stdfunc__0_71, 108, -1, G__defined_typename("time_t"), 0, 1, 1, 1, 0, "U 'tm' - 0 - timeptr", (char*) NULL
#ifndef mktime
, (void*) mktime, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("modf", 422, G__stdfunc__0_72, 100, -1, -1, 0, 2, 1, 1, 0, 
"d - - 0 - real D - - 0 - ip", (char*) NULL
#ifndef modf
, (void*) modf, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("perror", 666, G__stdfunc__0_73, 121, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - string", (char*) NULL
#ifndef perror
, (void*) perror, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("pow", 342, G__stdfunc__0_74, 100, -1, -1, 0, 2, 1, 1, 0, 
"d - - 0 - z d - - 0 - x", (char*) NULL
#ifndef pow
, (void*) pow, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("putc", 444, G__stdfunc__0_75, 105, -1, -1, 0, 2, 1, 1, 0, 
"i - - 0 - character E - - 0 - fp", (char*) NULL
#ifndef putc
, (void*) putc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("putchar", 759, G__stdfunc__0_76, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - character", (char*) NULL
#ifndef putchar
, (void*) putchar, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("puts", 460, G__stdfunc__0_77, 105, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - string", (char*) NULL
#ifndef puts
, (void*) puts, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("raise", 532, G__stdfunc__0_78, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - signal", (char*) NULL
#ifndef raise
, (void*) raise, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("rand", 421, G__stdfunc__0_79, 105, -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef rand
, (void*) rand, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("realloc", 738, G__stdfunc__0_80, 89, -1, -1, 0, 2, 1, 1, 0, 
"Y - - 0 - ptr h - 'size_t' 0 - size", (char*) NULL
#ifndef realloc
, (void*) realloc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("remove", 654, G__stdfunc__0_81, 105, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - filename", (char*) NULL
#ifndef remove
, (void*) remove, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("rename", 632, G__stdfunc__0_82, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - old C - - 10 - new", (char*) NULL
#ifndef rename
, (void*) rename, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("rewind", 649, G__stdfunc__0_83, 121, -1, -1, 0, 1, 1, 1, 0, "E - - 0 - fp", (char*) NULL
#ifndef rewind
, (void*) rewind, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("setbuf", 649, G__stdfunc__0_84, 121, -1, -1, 0, 2, 1, 1, 0, 
"E - - 0 - fp C - - 0 - buffer", (char*) NULL
#ifndef setbuf
, (void*) setbuf, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("setlocale", 956, G__stdfunc__0_85, 67, -1, -1, 0, 2, 1, 1, 0, 
"i - - 0 - position C - - 10 - locale", (char*) NULL
#ifndef setlocale
, (void*) setlocale, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("setvbuf", 767, G__stdfunc__0_86, 105, -1, -1, 0, 4, 1, 1, 0, 
"E - - 0 - fp C - - 0 - buffer i - - 0 - mode h - 'size_t' 0 - size", (char*) NULL
#ifndef setvbuf
, (void*) setvbuf, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("sin", 330, G__stdfunc__0_87, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - radian", (char*) NULL
#ifndef sin
, (void*) sin, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("sinh", 434, G__stdfunc__0_88, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - value", (char*) NULL
#ifndef sinh
, (void*) sinh, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("sqrt", 458, G__stdfunc__0_89, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - z", (char*) NULL
#ifndef sqrt
, (void*) sqrt, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("srand", 536, G__stdfunc__0_90, 121, -1, -1, 0, 1, 1, 1, 0, "h - - 0 - seed", (char*) NULL
#ifndef srand
, (void*) srand, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strcat", 657, G__stdfunc__0_91, 67, -1, -1, 0, 2, 1, 1, 0, 
"C - - 0 - string1 C - - 10 - string2", (char*) NULL
#ifndef strcat
, (void*) strcat, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strchr", 662, G__stdfunc__0_92, 67, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - string i - - 0 - character", (char*) NULL
#ifndef strchr
, (void*) strchr, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strcmp", 665, G__stdfunc__0_93, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - string1 C - - 10 - string2", (char*) NULL
#ifndef strcmp
, (void*) strcmp, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strcoll", 771, G__stdfunc__0_94, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - string1 C - - 10 - string2", (char*) NULL
#ifndef strcoll
, (void*) strcoll, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strcpy", 677, G__stdfunc__0_95, 67, -1, -1, 0, 2, 1, 1, 0, 
"C - - 0 - string1 C - - 10 - string2", (char*) NULL
#ifndef strcpy
, (void*) strcpy, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strcspn", 781, G__stdfunc__0_96, 104, -1, G__defined_typename("size_t"), 0, 2, 1, 1, 0, 
"C - - 10 - string1 C - - 10 - string2", (char*) NULL
#ifndef strcspn
, (void*) strcspn, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strerror", 899, G__stdfunc__0_97, 67, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - error", (char*) NULL
#ifndef strerror
, (void*) strerror, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strftime", 878, G__stdfunc__0_98, 104, -1, G__defined_typename("size_t"), 0, 4, 1, 1, 0, 
"C - - 0 - string h - 'size_t' 0 - maximum C - - 10 - format U 'tm' - 0 - brokentime", (char*) NULL
#ifndef strftime
, (void*) strftime, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strlen", 664, G__stdfunc__0_99, 104, -1, G__defined_typename("size_t"), 0, 1, 1, 1, 0, "C - - 10 - string", (char*) NULL
#ifndef strlen
, (void*) strlen, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strncat", 767, G__stdfunc__0_100, 67, -1, -1, 0, 3, 1, 1, 0, 
"C - - 0 - string1 C - - 10 - string2 h - 'size_t' 0 - n", (char*) NULL
#ifndef strncat
, (void*) strncat, 0);
#else
, (void*) NULL, 0);
#endif
}

static void G__cpp_setup_func1() {
   G__memfunc_setup("strncmp", 775, G__stdfunc__0_101, 105, -1, -1, 0, 3, 1, 1, 0, 
"C - - 10 - string1 C - - 10 - string2 h - 'size_t' 0 - n", (char*) NULL
#ifndef strncmp
, (void*) strncmp, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strncpy", 787, G__stdfunc__0_102, 67, -1, -1, 0, 3, 1, 1, 0, 
"C - - 0 - string1 C - - 10 - string2 h - 'size_t' 0 - n", (char*) NULL
#ifndef strncpy
, (void*) strncpy, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strpbrk", 776, G__stdfunc__0_103, 67, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - string1 C - - 10 - string2", (char*) NULL
#ifndef strpbrk
, (void*) strpbrk, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strrchr", 776, G__stdfunc__0_104, 67, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - string i - - 0 - character", (char*) NULL
#ifndef strrchr
, (void*) strrchr, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strspn", 682, G__stdfunc__0_105, 104, -1, G__defined_typename("size_t"), 0, 2, 1, 1, 0, 
"C - - 10 - string1 C - - 10 - string2", (char*) NULL
#ifndef strspn
, (void*) strspn, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strstr", 690, G__stdfunc__0_106, 67, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - string1 C - - 10 - string2", (char*) NULL
#ifndef strstr
, (void*) strstr, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strtod", 672, G__stdfunc__0_107, 100, -1, -1, 0, 2, 1, 1, 0, 
"C - - 10 - string C - - 2 - tailptr", (char*) NULL
#ifndef strtod
, (void*) strtod, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strtok", 679, G__stdfunc__0_108, 67, -1, -1, 0, 2, 1, 1, 0, 
"C - - 0 - string1 C - - 10 - string2", (char*) NULL
#ifndef strtok
, (void*) strtok, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strtol", 680, G__stdfunc__0_109, 108, -1, -1, 0, 3, 1, 1, 0, 
"C - - 10 - sprt C - - 2 - tailptr i - - 0 - base", (char*) NULL
#ifndef strtol
, (void*) strtol, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strtoul", 797, G__stdfunc__0_110, 107, -1, -1, 0, 3, 1, 1, 0, 
"C - - 10 - sprt C - - 2 - tailptr i - - 0 - base", (char*) NULL
#ifndef strtoul
, (void*) strtoul, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("strxfrm", 790, G__stdfunc__0_111, 104, -1, G__defined_typename("size_t"), 0, 3, 1, 1, 0, 
"C - - 0 - string1 C - - 10 - string2 h - 'size_t' 0 - n", (char*) NULL
#ifndef strxfrm
, (void*) strxfrm, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("system", 677, G__stdfunc__0_112, 105, -1, -1, 0, 1, 1, 1, 0, "C - - 10 - program", (char*) NULL
#ifndef system
, (void*) system, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("tan", 323, G__stdfunc__0_113, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - radian", (char*) NULL
#ifndef tan
, (void*) tan, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("tanh", 427, G__stdfunc__0_114, 100, -1, -1, 0, 1, 1, 1, 0, "d - - 0 - value", (char*) NULL
#ifndef tanh
, (void*) tanh, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("time", 431, G__stdfunc__0_115, 108, -1, G__defined_typename("time_t"), 0, 1, 1, 1, 0, "L - 'time_t' 0 - tp", (char*) NULL
#ifndef time
, (void*) time, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("tmpfile", 753, G__stdfunc__0_116, 69, -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL
#ifndef tmpfile
, (void*) tmpfile, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("tmpnam", 653, G__stdfunc__0_117, 67, -1, -1, 0, 1, 1, 1, 0, "C - - 0 - name", (char*) NULL
#ifndef tmpnam
, (void*) tmpnam, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("tolower", 780, G__stdfunc__0_118, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef tolower
, (void*) tolower, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("toupper", 783, G__stdfunc__0_119, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - c", (char*) NULL
#ifndef toupper
, (void*) toupper, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("ungetc", 646, G__stdfunc__0_120, 105, -1, -1, 0, 2, 1, 1, 0, 
"i - - 0 - character E - - 0 - fp", (char*) NULL
#ifndef ungetc
, (void*) ungetc, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("wcstombs", 882, G__stdfunc__0_121, 104, -1, G__defined_typename("size_t"), 0, 3, 1, 1, 0, 
"C - - 0 - multibyte H - 'wchar_t' 0 - widechar h - 'size_t' 0 - number", (char*) NULL
#ifndef wcstombs
, (void*) wcstombs, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("wctomb", 652, G__stdfunc__0_122, 105, -1, -1, 0, 2, 1, 1, 0, 
"C - - 0 - string h - 'wchar_t' 0 - widecharacter", (char*) NULL
#ifndef wctomb
, (void*) wctomb, 0);
#else
, (void*) NULL, 0);
#endif
   G__memfunc_setup("exit", 442, G__stdfunc__0_123, 121, -1, -1, 0, 1, 1, 1, 0, "i - - 0 - status", (char*) NULL
#ifndef exit
, (void*) exit, 0);
#else
, (void*) NULL, 0);
#endif

   G__resetifuncposition();
}

void G__c_setup_func() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__LN_lconv = { "lconv" , 115 , -1 };
G__linked_taginfo G__LN_tm = { "tm" , 115 , -1 };
G__linked_taginfo G__LN_dAdiv_t = { "$div_t" , 115 , -1 };
G__linked_taginfo G__LN_dAldiv_t = { "$ldiv_t" , 115 , -1 };

/* Reset class/struct taginfo */
void G__c_reset_tagtable() {
  G__LN_lconv.tagnum = -1 ;
  G__LN_tm.tagnum = -1 ;
  G__LN_dAdiv_t.tagnum = -1 ;
  G__LN_dAldiv_t.tagnum = -1 ;
}


void G__c_setup_tagtable() {

   /* Setting up class,struct,union tag entry */
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
