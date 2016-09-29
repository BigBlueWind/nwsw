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

     Name:     smsbexms.c - stack manager - interface with SCTP layer

     Type:     C source file

     Desc:     Sample C source code for the stack manager.
               - SCTP layer interface primitives.
               - Functions required for unpacking layer management
                 service provider primitives in loosely coupled systems.

     File:     ve_sm_sb_exms.c

     Sid:      ve_sm_sb_exms.c@@/main/2 - Wed Dec  1 08:44:38 2010

     Prg:      an

*********************************************************************21*/

/*

  smsbexms.c -

Following functions are provided in this file:
*/

/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000163                      SCTP layer
 */

/* header include files (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_hash.h"       /* common hashing */
#include "cm_llist.h"      /* common linked list */
#include "cm5.h"           /* timer defines */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"        /* common dns defines */
#ifdef SB_FTHA
#include "sht.h"           /* SHT Interface header file */
#endif /* SB_FTHA */
#include "sct.h"           /* SCT interface */
#include "lsb.h"           /* layer management defines for SCTP layer */
#include "ve_sm_init.h"
#include "ve.h"

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm_hash.x"       /* common hashing */
#include "cm_llist.x"      /* common linked list */
#include "cm_lib.x"        /* common library */
#include "cm5.x"           /* timer typedefs */
#include "cm_tpt.x"              /* common transport address */
#include "cm_dns.x"              /* common dns address */
#ifdef SB_FTHA
#include "sht.x"           /* SHT Interface header file */
#endif /* SB_FTHA */
#include "sct.x"           /* SCT interface */
#include "lsb.x"           /* layer management typedefs for SCTP layer */
#include "ve_sm_init.x"


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
*       File:  smsbexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smSbActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smSbActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smSbActvInit)

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   RETVALUE(ROK);
} /* end of smSbActvInit */



/* local defines */

/* local typedefs */

/* forward references */
EXTERN  S16 smSbActvTsk     ARGS((Pst *pst, Buffer *mBuf));


/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from SCTP layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:     ve_sm_sb_exms.c
*
*/

#ifdef ANSI
PUBLIC S16 smSbActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smSbActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;
   TRC3(smSbActvTsk)
   ret = ROK;
  
   switch(pst->event)
   {
#ifdef LCSMSBMILSB
    
      case LSB_EVTCFGCFM:             /* Config confirm */
         ret = cmUnpkLsbCfgCfm(SmMiLsbCfgCfm, pst, mBuf);
         break;
      case LSB_EVTCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLsbCntrlCfm(SmMiLsbCntrlCfm, pst, mBuf);
         break;
      case LSB_EVTSTACFM:             /* Status Confirm */
         ret = cmUnpkLsbStaCfm(SmMiLsbStaCfm, pst, mBuf);
         break;
      case LSB_EVTSTSCFM:             /* Statistics Confirm */
         ret = cmUnpkLsbStsCfm(SmMiLsbStsCfm, pst, mBuf);
         break;
      case LSB_EVTSTAIND:             /* Status Indication */
           /* Removed the debug prints*/

            ret = cmUnpkLsbStaInd(SmMiLsbStaInd, pst, mBuf);
         break;
      case LSB_EVTTRCIND:             /* Trace Indication */
         ret = cmUnpkLsbTrcInd(SmMiLsbTrcInd, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
#ifdef DEBUGP
       VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "smSbActvTsk : Received\
         invalid event\n"));
#endif
         break;
   }
   SExitTsk();
   RETVALUE(ret);
} /* end of smSbActvTsk */

/********************************************************************30**
  
         End of file:     ve_sm_sb_exms.c@@/main/2 - Wed Dec  1 08:44:38 2010
  
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
