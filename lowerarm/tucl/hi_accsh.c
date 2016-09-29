/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/********************************************************************20**
  
     Name:    TCP UDP Convergence Layer (TUCL)
  
     Type:    C source file
  
     Desc:    This file contains all the incoming primitives to the 
              SH from TCP/UDP Convergence Layer - TUCL
              
              
     File:    hi_accsh.c
  
     Sid:      hi_accsh.c@@/main/2 - Mon Mar  3 20:08:54 2008

     Prg:     mmh
  
*********************************************************************21*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

/* external headers */
#ifdef HI_TLS
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif

#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list */
#include "cm5.h"           /* common timer */
#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* common transport defines */
#ifdef FTHA
#include "sht.h"           /* SHT Interface header files */
#endif
#include "lhi.h"           /* layer management, TUCL  */
#include "hit.h"           /* HIT interface */
#ifdef HI_LKSCTP
#include "sct.h"           /* SCT interface */
#endif
#include "hi.h"            /* TUCL internal defines */
#include "hi_err.h"        /* TUCL error */
#include "hi_accen.h"      /* TUCL environment dependent test defines */
#include "hi_acc.h"        /* TUCL test defines */
#include "ss_task.h"       /* new system services task */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_hash.x"       /* common hash list */
#include "cm_llist.x"      /* common linked list */
#include "cm_lib.x"        /* common library */
#include "cm5.x"           /* common timer */
#include "cm_inet.x"       /* common sockets */
#include "cm_tpt.x"        /* common transport typedefs */
#ifdef FTHA
#include "sht.x"           /* SHT Interface defines */
#endif
#ifdef HI_LKSCTP
#include "sct.x"           /* SCT interface */
#endif
#include "lhi.x"           /* layer management TUCL */
#include "hit.x"           /* HIT interface */
#include "hi.x"            /* TUCL internal typedefs */
#include "hi_acc.x"        /* TUCL test  typedefs */
 
#ifndef WIN32
#include <signal.h> 
#endif


#ifdef FTHA

/* private variable definition */
#define SH_GETMSG(p, m, e) { \
    S16 ret; \
    ret = SGetMsg((p)->region, (p)->pool, &(m)); \
    if (ret != ROK) \
    { \
       HIDBGP(DBGMASK_LYR, (hiCb.init.prntBuf, "hiShDeq(): invalid event\n")); \
       RETVALUE(ret); \
    } \
}

/* forward references */
EXTERN S16 ShMiShtCntrlCfm ARGS((Pst *pst, ShtCntrlCfmEvnt *cfmInfo));


/*
 *
 *       Fun:    Activate Task
 *
 *       Desc:   Processes from the layer and the PSF
 *
 *       Ret:    ROK  - ok
 *
 *       Notes:  None
 *
 *       File:   hi_accsh.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 shActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 shActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{

   TRC3(shActvTsk);

   switch(pst->event)
   {
      case EVTSHTCNTRLCFM:      
         /* hi002.105 (hi026.104) - removed trigraph inside comment */
         /* cmUnpkMiShtCntrlCfm(ShMiShtCntrlCfm, pst, mBuf); (?) We
            dont really need to unpack and pack the data again in the
            ShMiShtCntrlCfm function. */
         CMCHKPKLOG(SPkU8, EVTSHTCNTRLCFM, mBuf, EHI001, pst);

         /* now write the version info in mBuf to shRxQ so that HC can read by
          * DeQueuing it */
         SQueueLast(mBuf, &shRxQ);        
         break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)         
         HIDBGP(DBGMASK_LYR, (hiCb.init.prntBuf,
                    "shActvTsk(): invalid event\n"));
#endif /* ERRCLS_DEBUG */    

         break;
   }
   
   SExitTsk();
   RETVALUE(ROK);
} /* end of shActvTsk */


  
/*
 *
 *      Fun:   Activate - initialize
 *
 *      Desc:  Invoked by system services to initialize a task.
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  hi_accsh.c
 *
 */
#ifdef ANSI
PUBLIC S16 shActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 shActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   TRC3 (shActvInit);
   RETVALUE (TRUE);
} /* shActvInit */

  
/*
 *
 *       Fun:   ShMiShtCntrlCfm 
 *
 *       Desc:  Handle control confirms from TUCL
 *
 *       Ret:   ROK   - ok
 *
 *       Notes : The interface with the SH is always LC. (tcr00018.03)
 *
 *       File:  hi_accsh.c
 *
 */
#ifdef ANSI
PUBLIC S16 ShMiShtCntrlCfm
(
Pst *pst,
ShtCntrlCfmEvnt *cfmInfo
)
#else
PUBLIC S16 ShMiShtCntrlCfm(pst, cfmInfo)
Pst *pst;
ShtCntrlCfmEvnt *cfmInfo;
#endif
{
   Buffer *mBuf;

   TRC3(ShMiShtCntrlCfm);

   SH_GETMSG(pst, mBuf, EHI002);
   cmPkShtCntrlCfmEvnt (cfmInfo, mBuf);
   CMCHKPKLOG(SPkU8, EVTSHTCNTRLCFM, mBuf, EHI003, pst);
   SQueueLast(mBuf, &shRxQ);

   RETVALUE(ROK);
} /* ShMiShtCntrlCfm */


/*
 *
 *       Fun:   hiShDeq
 *
 *       Desc:  - remove the first message from the SH receive queue
 *              - unpack the parameters 
 *       Ret:   ROK     - message is dequeued
 *       Ret:   ROKDNA  - message cannot be dequeued
 *
 *       Notes: 
 *
 *       File:  hi_accsh.c
 *
 */
#ifdef ANSI
PUBLIC S16 hiShDeq
(
HiShMsg *shMsg
)
#else
PUBLIC S16 hiShDeq(shMsg)
HiShMsg *shMsg;
#endif
{
   Buffer *mBuf;

   TRC2 (hiShDeq);

   if (SDequeueFirst(&mBuf, &shRxQ) != ROK)
      RETVALUE(ROKDNA);
   
   SUnpkU8(&shMsg->event, mBuf);
   
   switch (shMsg->event)
   {
      case EVTSHTCNTRLCFM:  /* confirmation message */
         cmUnpkShtCntrlCfmEvnt (&shMsg->t.s.cfmInfo, mBuf);
         break;

      default:
         
         HIDBGP(DBGMASK_LYR, (hiCb.init.prntBuf,
            "hiShDeq(): invalid event\n"));

         /* addition - deallocate mBuf and return ROKDNA */
         SPutMsg(mBuf);
         RETVALUE(ROKDNA);
         break;
   }

   /* addition - deallocating mBuf */
   SPutMsg(mBuf);   
   RETVALUE (ROK);
} /* hiShDeq */

#endif /* FTHA */

/********************************************************************30**
  
         End of file:     hi_accsh.c@@/main/2 - Mon Mar  3 20:08:54 2008
  
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/
  
/********************************************************************50**
  
*********************************************************************51*/
  

/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************90**
 
     ver       pat     init                  description
------------ --------- ---- ---------------------------------------------
1.1          hi009.104 mmh  1. Release with RUG patch.
/main/1      ---       kp   1. Updated for release 1.5.
/main/1+     hi002.105 ss   1. Removed trigraph warning
/main/2      ---       hs   1. Updated for release of 2.1
*********************************************************************91*/
