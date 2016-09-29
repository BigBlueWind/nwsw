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

/************************************************************************
 
     Name:     LTE MAC Convergence Layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     ys_ex_ms.c
  
     Sid:      yw_ex_ms.c@@/main/2 - Thu Dec  2 02:27:25 2010
  
     Prg:      pk
  
**********************************************************************/

/** @file ys_ex_ms.c
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* Trillium Includes */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list defines  */
#include "cm_hash.h"       /* Common hashlist defines */
#include "cm_lte.h"        /* Common LTEE defines */
#include "ctf.h"           /* CTF defines */
#include "lys.h"           /* layer management defines for LTE-CL */
#include "tfu.h"


#ifdef YS_MSPD
#include "LtePhyL2Api.h"
#include "resultcodes.h"
#include "ys_ms.h"            /* defines and macros for CL */
#else
#include <sys/time.h>
#include "ys.h"
#endif /* YS_MSPD*/
#include "ys_ms_err.h"        /* CL error header file */

#ifdef TENB_AS_SECURITY
#include "spaccdrv.h" /*sumanth*/
#endif



#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "ctf.x"
#include "cm_lib.x"
#include "tfu.x"
#include "lys.x"


#ifdef YS_MSPD
/* Silicon Includes */
#include "ys_ms.x"
#else
#include "ys.x"            /* typedefs for CL */
#endif /* YS_MSPD */

#ifdef YS_MSPD
PUBLIC RESULTCODE ysReceivePHYMsg ( U32   size, PTR   l1Msg);
#endif

/*ys004.102 MSPD merge for PHY 1.7*/
/** @brief This is the activation task of the CL layer. 
 * @param pst pointer to the Pst structure
 * @param mBuf pointer to the Buffer. 
 * @return ROK/RFAILED
 */
#ifdef ANSI
PUBLIC S16 ysActvTsk
(
Pst     *pst,
Buffer  *mBuf
)
#else
PUBLIC S16 ysActvTsk (pst, mBuf)
Pst     *pst;
Buffer  *mBuf;
#endif
{
   S16       ret   = ROK;

   TRC3(ysActvTsk)

   ret = ROK;

   switch(pst->srcEnt)
   {
#if defined(YS_MSPD) || defined(YS_PICO)
      /* The originator of this message is the stack manager,
       * unpack and go to the respective primitive processing function */
      case ENTSM:
         switch(pst->event)
         {
#ifdef LCYSMILYS
            case EVTLYSCFGREQ:
               /* Process a config. request */
               cmUnpkLysCfgReq(YsMiLysCfgReq, pst, mBuf);
               break;
            case EVTLYSCNTRLREQ:
               /* Process a control request */
               cmUnpkLysCntrlReq(YsMiLysCntrlReq, pst, mBuf);
               break;
            case EVTLYSSSTAREQ:
               /* Process a status request  */
               cmUnpkLysStaReq(YsMiLysStaReq, pst, mBuf);
               break;
            case EVTLYSSTSREQ:
               /* Process a statistics request */
               cmUnpkLysStsReq(YsMiLysStsReq, pst, mBuf);
               break;
#endif /* LCYSMILYS */
            default:
               YS_DBG_ERR((_ysp, "ysActvTsk(): Invalid event (%d)! \n",
                           pst->event));
               SPutMsg(mBuf);
               break;
         }
         break;

      case ENTRG:
         switch (pst->event)
         {
#if  (defined(LCYSUITFU) || defined(LWLCYSUITFU))
            case EVTTFUBNDREQ:
               ret = cmUnpkTfuBndReq (YsUiTfuBndReq, pst, mBuf);
               break;
#ifdef MSPD
            case EVTTFUSCHBNDREQ:
               ret = cmUnpkTfuBndReq (YsUiTfuSchBndReq, pst, mBuf);
               break;
#endif
            case EVTTFUUBNDREQ:
               ret = cmUnpkTfuUbndReq (YsUiTfuUbndReq, pst, mBuf);
               break;
            case EVTTFUCNTRLREQ:
               ret = cmUnpkTfuCntrlReq (YsUiTfuCntrlReq, pst, mBuf);
               break;
            case EVTTFUDATREQ:
               ret = cmUnpkTfuDatReq (YsUiTfuDatReq, pst, mBuf);
               break;
            case EVTTFURECPREQ:
               ret = cmUnpkTfuRecpReq (YsUiTfuRecpReq, pst, mBuf);
               break;
#endif
            default:
               YS_DBG_ERR((_ysp, "ysActvTsk(): Invalid event (%d)! \n",
                           pst->event));
               SPutMsg(mBuf);
               break;

         } /* end of switch (evnt) */
         break;
#endif /* YS_MSPD or YS_PICO */

#if (defined(YS_PICO) || (defined(YS_LTE_PAL) && defined (LTE_PAL_ENB)) || \
      defined(YS_MSPD))
      case ENTVE:
      case ENTNH:
         switch (pst->event)
         {
#ifdef LCYSUICTF
            case EVTCTFBNDREQ:
               ret = cmUnpkCtfBndReq (YsUiCtfBndReq, pst, mBuf);
               break;
            case EVTCTFUBNDREQ:
               ret = cmUnpkCtfUbndReq (YsUiCtfUbndReq, pst, mBuf);
               break;
            case EVTCTFCFGREQ:
               ret = cmUnpkCtfCfgReq (YsUiCtfCfgReq, pst, mBuf);
               break;
#ifdef TENB_AS_SECURITY
            case EVTCTFKDFREQ:
            ret = cmUnpkCtfKdfReq(YsUiCtfKdfReq, pst, mBuf);
            break;
#endif
#endif
            default:
               YS_DBG_ERR((_ysp, "ysActvTsk(): Invalid event (%d)! \n",
                           pst->event));
               SPutMsg(mBuf);
               break;

         } /* end of switch (evnt) */
         break;
#endif

      case ENTTF:
#ifdef MSPD
         /* self post: there can be only one event coming from the receiver 
          * thread
          * which is the uplink data
          */
         ret = ysReceivePHYMsg(0, (PTR) mBuf);
         break;
#else
         switch(pst->event)
         {
#ifdef LTE_PAL_ENB
#else /* LTE_PAL_ENB */
            case EVTTFUCNTRLREQ:
               cmUnpkTfuCntrlReq(YsUiTfuCntrlReq, pst, mBuf);
               break;
            case EVTTFUDATREQ:
               cmUnpkTfuDatReq(YsUiTfuDatReq, pst, mBuf);
               break;
            case EVTTFURECPREQ:
               ret = cmUnpkTfuRecpReq(YsUiTfuRecpReq, pst, mBuf);
               break;
#endif /* LTE_PAL_ENB */
            default:
               YS_DBG_ERR((_ysp, "ysActvTsk(): Invalid event (%d)! \n",pst->event));
               SPutMsg(mBuf);
               break;
         }
         break;
#endif

      default:
         YS_DBG_ERR((_ysp, "ysActvTsk(): Message from an invalid \
                              entity (%d)! \n", pst->srcEnt));
         SPutMsg(mBuf);
         ret = RFAILED;

   } /* end of switch */

   SExitTsk();
   RETVALUE (ret);
} /* end of ysActvTsk */

/********************************************************************30**
  
         End of file:     yw_ex_ms.c@@/main/2 - Thu Dec  2 02:27:25 2010
  
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

     ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/2      ---     sgm                   1. eNodeB 1.2 release
/main/1      ys004.102     vr              1. MSPD merge for PHY 1.7
*********************************************************************91*/
