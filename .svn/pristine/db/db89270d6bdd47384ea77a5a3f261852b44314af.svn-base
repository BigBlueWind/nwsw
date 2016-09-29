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

/********************************************************************20**
  
     Name:     LTE eNodeB Stack Manager
  
     Type:     C source file
  
     Desc:     Activation function for TCP UDP Convergence Layer (TUCL)
               Stack Manager.
              
     File:     ve_sm_hi_exms.c
  
     Sid:      ve_sm_hi_exms.c@@/main/2 - Wed Dec  1 08:44:32 2010

     Prg:      asa
  
*********************************************************************21*/

/*
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.
*/
  
  
/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_inet.h"       /* common sockets */
#include "lhi.h"           /* stack management- TUCL */
#include "ve.h"           /* stack management- TUCL */

/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_inet.x"       /* common sockets */
#include "lhi.x"           /* stack management- TUCL */

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
*       File:  smhiexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smHiActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smHiActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smHiActvInit)

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   RETVALUE(ROK);
}

  
/* local defines */

/* local typedefs */
  
/* local externs */
  
/* forward references */
  

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from TUCL
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:     ve_sm_hi_exms.c
*
*/
  
#ifdef ANSI
PUBLIC S16 smHiActvTsk
(
Pst    *spst,               /* post structure */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smHiActvTsk(spst, mBuf)
Pst    *spst;               /* post structure */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16           ret = ROK; /* return value from the function */

   /* Added trace macro */
   TRC2(smHiActvTsk)

   switch(spst->event)
   {
#ifdef LCSMHIMILHI
      case EVTLHICFGCFM:             /* Configuration Confirm */
         if ((ret = cmUnpkLhiCfgCfm(SmMiLhiCfgCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;
      
      case EVTLHISTSCFM:             /* Statistics Confirm */
         if ((ret = cmUnpkLhiStsCfm(SmMiLhiStsCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHICNTRLCFM:           /* Conntrol Confirm */
         if ((ret = cmUnpkLhiCntrlCfm(SmMiLhiCntrlCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHISTACFM:             /* Status Confirm */
         if ((ret = cmUnpkLhiStaCfm(SmMiLhiStaCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

     case EVTLHISTAIND:             /* Status Indication */
         if ((ret = cmUnpkLhiStaInd(SmMiLhiStaInd, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHITRCIND:             /* Trace Indication */
         if ((ret = cmUnpkLhiTrcInd(SmMiLhiTrcInd, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;
#endif
      default:
        (Void)SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
        SLogError(spst->dstEnt, spst->dstInst, spst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_DEBUG, (ErrVal)VE_EVE001, 
                   (ErrVal)spst->event, "smHiActvTsk () Failed"); 
#endif
         break;
   }

   SExitTsk();

   RETVALUE(ret);
}/* end of smHiActvTsk() */

/********************************************************************30**
  
         End of file:     ve_sm_hi_exms.c@@/main/2 - Wed Dec  1 08:44:32 2010
  
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
