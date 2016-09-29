/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
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
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/
/**********************************************************************

     Name:     LTE eNodB Stack Manager 
  
     Type:     C Source file
  
     Desc:     Stack Manager Initialization Functions.

     File:     ve_sm_ve_exms.c

     Sid:      ve_sm_ve_exms.c@@/main/2 - Wed Dec  1 08:44:39 2010

     Prg:      an 

**********************************************************************/

/* header include files (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_inet.h"
#include "cm_tpt.h"
#include "ve_sm_init.h"
#include "lve.h"
#include "ve.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_inet.x"
#include "cm_tpt.x"
#include "ve_sm_init.x"
#include "lve.x"

/*
*
*       Fun:   Activate Task - initialize
*
*       Desc:  Invoked by system services to initialize a task.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smveexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smVeActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smVeActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smVeActvInit)

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(reason);
   smCb.init.region = region;
   smCb.init.pool  = 0;
   RETVALUE(ROK);
}


/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from VE
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ve_sm_ve_exms.c
*
*/

#ifdef ANSI
PUBLIC S16 smVeActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smVeActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;

   TRC2(smVeActvTsk)

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCVEMILVE
      case EVTLVECFGCFM:             /* Config confirm */
         ret = cmUnpkLveCfgCfm(SmMiLveCfgCfm, pst, mBuf);
         break;
      case EVTLVECNTRLCFM:           /* Control confirm */
         ret = cmUnpkLveCntrlCfm(SmMiLveCntrlCfm, pst, mBuf);
         break;
      case EVTLVESTAIND:
         ret = cmUnpkLveStaInd(SmMiLveStaInd, pst,mBuf);
         break;
#endif
      case EVTVESCTPCFG:
      case EVTVES1APCFG:
#ifdef LTE_HO_SUPPORT
      case EVTVEX2APCFG:
#endif
      case EVTVEEGTPCFG:
      case EVTVEAPPCFG:
      case EVTVERRCCFG:
      case EVTVEPDCPCFG:
      case EVTVERLCCFG:
      case EVTVEMACCFG:
      case EVTVECLCFG:
      case EVTVES1APSTKBND:
      case EVTVEEGTPSTKBND:
      case EVTVERRCSTKBND:
      case EVTVEENBALRMS:
      case EVTVEENBDEBUGS:
      case EVTVEINITS1SETUP:
      case EVTVEINITX2SETUP:
      case EVTVECELLCFG:
     
         smVeProcSm(pst->event);
         (Void)SPutMsg(mBuf);
         break;
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
#ifdef DEBUGP
       VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "smVeActvTsk : Received\
         invalid event\n"));
#endif
         break;
   }

   RETVALUE(ret);

} /* end of smVeActvTsk */

/********************************************************************30**
  
         End of file:     ve_sm_ve_exms.c@@/main/2 - Wed Dec  1 08:44:39 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
