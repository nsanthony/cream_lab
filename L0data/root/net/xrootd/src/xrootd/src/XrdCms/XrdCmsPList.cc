/******************************************************************************/
/*                                                                            */
/*                        X r d C m s P L i s t . c c                         */
/*                                                                            */
/* (c) 2007 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//         $Id: XrdCmsPList.cc 24468 2008-06-22 16:47:03Z ganis $

// Original Version: 1.8 2007/07/18 01:34:53 abh

const char *XrdCmsPListCVSID = "$Id: XrdCmsPList.cc 24468 2008-06-22 16:47:03Z ganis $";
  
#include "XrdCms/XrdCmsPList.hh"

/******************************************************************************/
/*                                  F i n d                                   */
/******************************************************************************/
  
int XrdCmsPList_Anchor::Find(const char *pname, XrdCmsPInfo &pinfo)
{
   int plen = strlen(pname);

// Lock the anchor and setup for search
//
   Lock();
   XrdCmsPList *p = next;

// Find matching entry
//
   while(p) if (p->pathlen <= plen && !strncmp(p->pathname, pname, p->pathlen)) 
               {pinfo = p->pathmask; break;}
               else p = p->next;

// All done
//
   UnLock();
   return p != 0;
}

/******************************************************************************/
/*                                I n s e r t                                 */
/******************************************************************************/
  
SMask_t XrdCmsPList_Anchor::Insert(const char *pname, XrdCmsPInfo *pinfo)
{
   int rc, plen = strlen(pname);
   XrdCmsPList *p, *pp;
   SMask_t newmask;

// Set up the search
//
   Lock();
   p = next;
   pp = 0;

// Find the proper insertion point. Paths are sorted in decreasin length
// order but within a length in increasing lexical order.
//
   rc = 1;
   while(p && p->pathlen >= plen)
        {if (p->pathlen == plen && !(rc = strcmp(p->pathname,pname))) break;
         pp = p; 
          p = p->next;
        }

// Merge the path masks
//
   if (!rc) {p->pathmask.And(~(pinfo->rovec)); p->pathmask.Or(pinfo);}
      else { p = new XrdCmsPList(pname, pinfo);
             if (pp)
                { p->next = pp->next;
                 pp->next = p;
                } else {
                  p->next = next;
                     next = p;
                }
           }

// All done
//
   newmask = p->pathmask.rovec | p->pathmask.rwvec;
   UnLock();
   return newmask;
}
 
/******************************************************************************/
/*                                R e m o v e                                 */
/******************************************************************************/
  
void XrdCmsPList_Anchor::Remove(SMask_t mask)
{
    SMask_t zmask = ~mask;
    XrdCmsPList *pp = next, *prevp = 0;

// Lock the list
//
   Lock();

// Remove bit from mask. If mask is zero, remove the entry
//
   while(pp)
        {if (!pp->pathmask.And(zmask))
            {if (prevp) {prevp->next = pp->next; delete pp; pp = prevp->next;}
                else    {       next = pp->next; delete pp; pp = next;}
            }
            else {prevp = pp; pp = pp->next;}
        }

// All done
//
   UnLock();
}

/******************************************************************************/
/*                                  F i n d                                   */
/******************************************************************************/
  
const char *XrdCmsPList_Anchor::Type(const char *pname)
{
   int isrw = 0, plen = strlen(pname);

// Lock the anchor and setup for search
//
   Lock();
   XrdCmsPList *p = next;

// Find matching entry
//
   while(p) if (p->pathlen <= plen && !strncmp(p->pathname, pname, p->pathlen)) 
               {isrw = (p->pathmask.rwvec != 0); break;}
               else p = p->next;

// All done
//
   UnLock();
   if (p) return (isrw ? "w" : "r");
   return "?";
}
 
/******************************************************************************/
/*                                 P T y p e                                  */
/******************************************************************************/
  
const char *XrdCmsPList::PType()
{
    if (pathmask.ssvec) return (pathmask.rwvec ? "ws" : "rs");
    return (pathmask.rwvec ? "w" : "r");
}
