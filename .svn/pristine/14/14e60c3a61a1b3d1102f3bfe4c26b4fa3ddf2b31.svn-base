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
  
     File:     ys_mi.c
  
     Sid:      yw_mi.c@@/main/1 - Thu Dec  2 02:27:26 2010
  
     Prg:      pk
  
**********************************************************************/

/** @file ys_mi.c
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files -- defines (.h) */

/* header/extern include files (.x) */
/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "ctf.h"           /* CTF defines */
#include "lys.h"           /* layer management defines for LTE-CL */
#include "tfu.h"

#ifdef YS_MSPD
#include "LtePhyL2Api.h"
#include "apidefs.h"
#include "ys_ms.h"            /* defines and macros for CL */
#else
#include "ys.h"            /* defines and macros for CL */
#endif /* YS_MSPD */
#include "ys_err.h"        /* YS error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "ctf.x"           /* CTF types */
#include "lys.x"           /* layer management typedefs for CL */
#include "tfu.x"

#ifdef YS_MSPD
#include "ys_ms.x"            /* typedefs for CL */
#endif /* YS_MSPD */

#include "log.h"

/*ys004.102 MSPD merge for PHY 1.7*/
/* local externs */
PRIVATE U16 ysLMMGenCfg ARGS((YsCfg *cfg));
PRIVATE U16 ysLMMSapCfg ARGS((YsCfg *cfg, Elmnt sapType));
PRIVATE Void ysLMMGenCntrl ARGS((
YsMngmt       *cntrl,
YsMngmt       *cfm,
Pst           *cfmPst
));
PRIVATE Void ysLMMSapCntrl ARGS((
YsMngmt       *cntrl,
YsMngmt       *cfm,
Pst           *cfmPst
));
PRIVATE Void ysLMMShutdown ARGS((Void));
PRIVATE S16 ysLMMStopPhy ARGS((CmLteCellId cellId));
PRIVATE Void ysDeleteTfuSap ARGS((
YsTfuSapCb           *tfuSap
));
PRIVATE Void ysLMMFillCfmPst ARGS((
Pst           *reqPst,
Pst           *cfmPst,
YsMngmt       *cfm
));

/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : YsMiLysCfgReq
 *     
 *     This function handles the configuration
 *     request received from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions ysHdlGenCfg() or ysHdlSapCfg().
 *     -# Invokes YsMiLysCfgCfm() to send back the confirmation to the LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  YsMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 YsMiLysCfgReq
(
Pst      *pst,    /* post structure  */
YsMngmt  *cfg     /* config structure  */
)
#else
PUBLIC S16 YsMiLysCfgReq(pst, cfg)
Pst      *pst;    /* post structure  */
YsMngmt  *cfg;    /* config structure  */
#endif    
{
   U16       ret = LCM_PRIM_OK;
   U16       reason = LCM_REASON_NOT_APPL;
   YsMngmt   *cfm = NULLP;
   Pst       cfmPst;

   TRC2(YsMiLysCfgReq)
   
   logInfo("YsMiLysCfgReq(): pst->srcEnt=%d, pst->srcInst=%d", pst->srcEnt, pst->srcInst);
   /* Fill the post structure for sending the confirmation */
   ysLMMFillCfmPst(pst, &cfmPst, cfg);

   /* Initialize the cfg cfm structure */
   if (SGetSBuf(cfmPst.region, cfmPst.pool, (Data **)&cfm, sizeof(YsMngmt))
      != ROK)
   {
      logError("%s", "YsMiLysCfgReq(): Memory Unavailable for Confirmation");
      RETVALUE(ROK);
   }
   cmMemset((U8 *)cfm, 0, sizeof(YsMngmt));

   cfm->hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   cfm->hdr.transId=cfg->hdr.transId;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STGEN:
         reason = ysLMMGenCfg(&cfg->t.cfg); 
         logInfo("YsMiLysCfgReq(): Gen Cfg done with reason=%d.", reason);
         break;
      case STCTFSAP:
      case STTFUSAP:
         reason = ysLMMSapCfg(&cfg->t.cfg, cfg->hdr.elmId.elmnt);
         logInfo("YsMiLysCfgReq(): SAP(Elmnt=%d) Cfg done with reason=%d.", cfg->hdr.elmId.elmnt, reason);
         break;
      default:
         ret = LCM_PRIM_NOK;
         reason = LCM_REASON_INVALID_ELMNT;
         logInfo("YsMiLysCfgReq(): Invalid Elmnt=%d", cfg->hdr.elmId.elmnt);
         break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm->cfm.status = ret;
   cfm->cfm.reason = reason;

   YsMiLysCfgCfm(&cfmPst, cfm);
   
   SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(YsMngmt));
   
   RETVALUE(ROK);
}/*-- YsMiLysCfgReq --*/

/**
 * @brief Layer Manager Statistics request handler. 
 *
 * @details
 *
 *     Function : YsMiLysStsReq
 *     
 *     This function handles the statistics
 *     request received from the Layer Manager.
 *      -# Based on sts->hdr.elmId.elmnt, it retrieves either general or SAP
 *      statistics from the ysCb global control block.
 *      -# If action=ARST, it will reset the statistics parameters in ysCb to 0.
 *      -# Invokes the YsMiLysStsCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  YsMngmt *sts, the statistics parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 YsMiLysStsReq
(
Pst      *pst,    /* post structure  */
YsMngmt  *sts     /* statistics structure  */
)
#else
PUBLIC S16 YsMiLysStsReq(pst, sts)
Pst      *pst;    /* post structure  */
YsMngmt  *sts;    /* statistics structure  */
#endif    
{
   Pst            cfmPst;
   YsMngmt        *cfm    = NULLP;
   YsTfuSapCb     *tfuSap = NULLP;

   TRC2(YsMiLysStsReq)
   
   YS_DBG_INFO((_ysp, "\nYsMiLysStsReq(): stsReq for elmnt= %d pst->srcEnt=%d, \
                    action=%d", sts->hdr.elmId.elmnt, 
                    pst->srcEnt, sts->t.sts.action));
   /* Fill the post structure for sending the confirmation */
   ysLMMFillCfmPst(pst, &cfmPst, sts);

   /* Initialize the cfg cfm structure */
   if (SGetSBuf(cfmPst.region, cfmPst.pool, (Data **)&cfm, 
                                       sizeof(YsMngmt)) != ROK)
   {
      YS_DBG_ERR((_ysp, "\nYsMiLysStsReq(): Memory Unavailable for \
               Confirmation"));
      SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(YsMngmt));
      RETVALUE(ROK);
   }
   cmMemset((U8 *)cfm, 0, sizeof(YsMngmt));

   SGetDateTime(&cfm->t.sts.dt);
   cfm->cfm.status = LCM_PRIM_OK;
   cfm->cfm.reason = LCM_REASON_NOT_APPL;
   cfm->hdr.elmId.elmnt = sts->hdr.elmId.elmnt;
   cfm->t.sts.action = sts->t.sts.action;

   /* Check if General Config Done */
   if(ysCb.ysInit.cfgDone != TRUE) 
   {
      cfm->cfm.status = LCM_PRIM_NOK;
      cfm->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      YsMiLysStsCfm(&cfmPst,cfm);
      YS_DBG_ERR((_ysp, "\nYsMiLysStsReq(): Gen Cfg not done"));
      SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(YsMngmt));
      RETVALUE(ROK);
   }

   switch(sts->hdr.elmId.elmnt)
   {
      case STCTFSAP:
         cmMemcpy((U8 *)&cfm->t.sts.s.ctfSts, (U8 *)&ysCb.ctfSap.ctfSts,
                  sizeof(YsCtfSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            cmMemset((U8 *)&ysCb.ctfSap.ctfSts, 0, sizeof(YsCtfSapSts));

         break;

      case STTFUSAP:
         if (sts->t.sts.sapInst >= ysCb.genCfg.maxTfuSaps)
         {
            YS_DBG_ERR((_ysp, "\nYsMiLysCfgReq(): Invalid SSAP ID %d ",
                              sts->t.sts.sapInst));
            cfm->cfm.status = LCM_PRIM_NOK;
            cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         }
         else
         {
            tfuSap = ysCb.tfuSapLst[sts->t.sts.sapInst];

            if(tfuSap != NULLP)
            {
               cmMemcpy((U8 *)&cfm->t.sts.s.tfuSts, (U8 *)&tfuSap->tfuSts,
                     sizeof(YsTfuSapSts));

               /* check if action is read and reset */
               if(sts->t.sts.action == ARST)
                  cmMemset((U8 *)&tfuSap->tfuSts, 0, sizeof(YsTfuSapSts));
            }
            else
            {
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
            }
         }
         break;

      case STL1PHY:
         if (sts->t.sts.sapInst >= ysCb.genCfg.maxTfuSaps)
         {
            YS_DBG_ERR((_ysp, "\nYsMiLysCfgReq(): Invalid SSAP ID %d ",
                           sts->t.sts.sapInst));
            cfm->cfm.status = LCM_PRIM_NOK;
            cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         }
         else
         {
            tfuSap = ysCb.tfuSapLst[sts->t.sts.sapInst];

            if(tfuSap != NULLP)
            {
               cmMemcpy((U8 *)&cfm->t.sts.s.phySts, (U8 *)&tfuSap->phySts,
                     sizeof(YsL1PhySts));

               /* check if action is read and reset */
               if(sts->t.sts.action == ARST)
                  cmMemset((U8 *)&tfuSap->phySts, 0, sizeof(YsL1PhySts));
            }
            else
            {
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
            }
         }
         break;

      default:
         cfm->cfm.status = LCM_PRIM_NOK;
         cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         YS_DBG_ERR((_ysp, "\nYsMiLysStsReq(): Invalid Elmnt = %d",
                           sts->hdr.elmId.elmnt));
         break;     
   }
   YsMiLysStsCfm(&cfmPst,cfm);
   SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(YsMngmt));
   RETVALUE(ROK);
}/*-- YsMiLysStsReq --*/

/**
 * @brief Layer Manager Status request handler. 
 *
 * @details
 *
 *     Function : YsMiLysStaReq
 *     
 *     This function handles the solicited status
 *     request received from the Layer Manager.
 *      -# Based on sta->hdr.elmId.elmnt, it retrieves the status of a
 *      particular SAP from the ysCb global control block.
 *      -# Invokes the YsMiLysStaCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  YsMngmt *sta, the status parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 YsMiLysStaReq
(
Pst      *pst,    /* post structure  */
YsMngmt  *sta     /* status structure  */
)
#else
PUBLIC S16 YsMiLysStaReq(pst, sta)
Pst      *pst;    /* post structure  */
YsMngmt  *sta;    /* status structure  */
#endif    
{
   Pst            cfmPst;
   YsMngmt        *cfm    = NULLP;
   YsTfuSapCb     *tfuSap = NULLP;

   TRC2(YsMiLysStaReq)
   
   YS_DBG_INFO((_ysp, "\nYsMiLysStaReq(): staReq for elmnt= %d pst->srcEnt=%d",
                  sta->hdr.elmId.elmnt, pst->srcEnt));
   /* Fill the post structure for sending the confirmation */
   ysLMMFillCfmPst(pst, &cfmPst, sta);


   /* Initialize the cfg cfm structure */
   if (SGetSBuf(cfmPst.region, cfmPst.pool, (Data **)&cfm, sizeof(YsMngmt))
      != ROK)
   {
      YS_DBG_ERR((_ysp, "\nYsMiLysStaReq(): Memory Unavailable for \
               Confirmation"));
      SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(YsMngmt));
      RETVALUE(ROK);
   }
   cmMemset((U8 *)cfm, 0, sizeof(YsMngmt));
   cfm->hdr.elmId.elmnt = sta->hdr.elmId.elmnt;

   /* Check if General Config Done */
   if(ysCb.ysInit.cfgDone != TRUE) 
   {
      SGetDateTime(&cfm->t.ssta.dt);
      cfm->cfm.status = LCM_PRIM_NOK;
      cfm->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm->hdr.elmId.elmnt = sta->hdr.elmId.elmnt;
      YsMiLysStaCfm(&cfmPst, cfm);
      YS_DBG_ERR((_ysp, "\nYsMiLysStaReq(): Gen Cfg not done"));
      SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(YsMngmt));
      RETVALUE(ROK);
   }

   switch(sta->hdr.elmId.elmnt)
   {
#ifdef YS_MSPD
      case STSID:
#endif
         SGetDateTime(&cfm->t.ssta.dt);
         if (SGetSBuf(cfmPst.region, cfmPst.pool, 
             (Data **)&cfm->t.ssta.s.sysId.ptNmb, LYS_PT_NUM_SIZE)
            != ROK)
         {
            YS_DBG_ERR((_ysp, "\nYsMiLysStaReq(): Memory Unavailable \
                     for Confirmation"));
            SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(YsMngmt));
            RETVALUE(ROK);
         } 
         cmMemset((U8 *)cfm->t.ssta.s.sysId.ptNmb, 0, LYS_PT_NUM_SIZE);
         ysGetSId(&cfm->t.ssta.s.sysId);
         cfm->cfm.status = LCM_PRIM_OK;
         cfm->cfm.reason = LCM_REASON_NOT_APPL;
         YsMiLysStaCfm(&cfmPst, cfm);
         break;
      case STCTFSAP:
         cfm->cfm.status = LCM_PRIM_OK;
         cfm->cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm->t.ssta.dt);
         cfm->t.ssta.s.ctfSapSta.sapState = ysCb.ctfSap.sapState;
         YsMiLysStaCfm(&cfmPst, cfm);
         break;

      case STTFUSAP:
         cfm->cfm.status = LCM_PRIM_OK;
         cfm->cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm->t.ssta.dt);

         if (sta->t.ssta.sapInst >= ysCb.genCfg.maxTfuSaps)
         {
            YS_DBG_ERR((_ysp, "\nYsMiLysStaReq(): Invalid SSAP ID %d ",
                           sta->t.ssta.sapInst));
            cfm->cfm.status = LCM_PRIM_NOK;
            cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         }
         else
         {
            tfuSap = ysCb.tfuSapLst[sta->t.ssta.sapInst];

            if(tfuSap != NULLP)
            {
               cfm->t.ssta.s.tfuSapSta.sapState = tfuSap->sapState;
            }
            else
            {
               YS_DBG_ERR((_ysp, "\nYsMiLysStaReq(): Invalid SSAP ID %d ",
                              sta->t.ssta.sapInst));
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
            }
         }
         YsMiLysStaCfm(&cfmPst, cfm);
         break;

      case STL1PHY:
         cfm->cfm.status = LCM_PRIM_OK;
         cfm->cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm->t.ssta.dt);

         if (sta->t.ssta.sapInst >= ysCb.genCfg.maxTfuSaps)
         {
            YS_DBG_ERR((_ysp, "\nYsMiLysStaReq(): Invalid SSAP ID %d ",
                           sta->t.ssta.sapInst));
            cfm->cfm.status = LCM_PRIM_NOK;
            cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         }
         else
         {
            tfuSap = ysCb.tfuSapLst[sta->t.ssta.sapInst];

            if((tfuSap != NULLP) && (tfuSap->cellCb != NULLP))
            {
               cfm->t.ssta.s.phyState = tfuSap->cellCb->phyState;
            }
            else
            {
               YS_DBG_ERR((_ysp, "\nYsMiLysStaReq(): Invalid SSAP ID %d ",
                              sta->t.ssta.sapInst));
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
            }
         }
         YsMiLysStaCfm(&cfmPst, cfm);
         break;

      default:
         cfm->cfm.status = LCM_PRIM_NOK;
         cfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         YsMiLysStaCfm(&cfmPst, cfm);
         YS_DBG_ERR((_ysp, "\nYsMiLysStaReq(): Invalid elmnt=%d",
                        sta->hdr.elmId.elmnt));
         break;     
   }
   SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(YsMngmt));
   RETVALUE(ROK);
}/*-- YsMiLysStaReq --*/

/**
 * @brief Layer Manager Control request handler. 
 *
 * @details
 *
 *     Function : YsMiLysCntrlReq
 *     
 *     This function handles the control
 *     request received from the Layer Manager.
 *      -# Based on cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action
 *      and cntrl->t.cntrl.subAction, it performs the appropriate control action
 *      of SAP (enable/disable), Debug (enable/disable), Trace (enable/disable)
 *      and layer shutdown.
 *      -# Invokes the YsMiLysCntrlCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  YsMngmt *cntrl, the control parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 YsMiLysCntrlReq
(
Pst      *pst,    /* post structure  */
YsMngmt  *cntrl   /* control structure  */
)
#else
PUBLIC S16 YsMiLysCntrlReq(pst, cntrl)
Pst      *pst;    /* post structure  */
YsMngmt  *cntrl;  /* control structure  */
#endif    
{
   S16       ret = ROK;            /* return value */
   Pst       cfmPst;
   YsMngmt   *cfm = NULLP;
   
   TRC2(YsMiLysCntrlReq)
   
   YS_DBG_INFO((_ysp, "\nYsMiLysCntrlReq():cntrlReq for elmnt= %d, action=%d, \
                   subaction=%d", cntrl->hdr.elmId.elmnt,
                   cntrl->t.cntrl.action, cntrl->t.cntrl.subAction));
   /* Fill the post structure for sending the confirmation */
   ysLMMFillCfmPst(pst, &cfmPst, cntrl);

   /* Initialize the cfg cfm structure */
   if (SGetSBuf(cfmPst.region, cfmPst.pool, (Data **)&cfm, sizeof(YsMngmt))
      != ROK)
   {
      YS_DBG_ERR((_ysp, "\nYsMiLysCntrlReq(): Memory Unavailable for \
               Confirmation"));
      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(YsMngmt));
      RETVALUE(ROK);
   }
   cmMemset((U8 *)cfm, 0, sizeof(YsMngmt));
   cfm->hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
   cfm->t.cntrl.action = cntrl->t.cntrl.action;
   cfm->t.cntrl.subAction = cntrl->t.cntrl.subAction;
#ifdef PWAV
   cfm->hdr.transId       = cntrl->hdr.transId;
#endif

   /* Check if General Config Done*/
   if(ysCb.ysInit.cfgDone != TRUE)
   {
      cfm->cfm.status = LCM_PRIM_NOK;
      cfm->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm->hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
      YsMiLysCntrlCfm(&cfmPst, cfm);
      YS_DBG_ERR((_ysp, "\nYsMiLysCntrlReq(): Gen Cfg not done."));
      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(YsMngmt));
      RETVALUE(ROK);
   }
 
   /* General Config done, process the Control request */   
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STGEN:
         ysLMMGenCntrl(cntrl, cfm, &cfmPst);
         break;
      case STTFUSAP:
      case STCTFSAP:
         ysLMMSapCntrl(cntrl, cfm, &cfmPst);
         break;
      default:
         cfm->cfm.status = LCM_PRIM_NOK;
         cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
         YsMiLysCntrlCfm(&cfmPst, cfm);
         YS_DBG_ERR((_ysp, "\nYsMiLysCntrlReq(): invalid elmnt=%d",
         cntrl->hdr.elmId.elmnt));
         break;
   }
   SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(YsMngmt));
   RETVALUE(ret);
}/*-- YsMiLysCntrlReq --*/

/**
 * @brief General Configuration Handler. 
 *
 * @details
 *
 *     Function : ysLMMGenCfg
 *     
 *     This function in called by YsMiLysCfgReq(). It handles the
 *     general configuration of the CL layer. It initializes 
 *     the YsCb. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  YsCfg *cfg, the Configuaration info 
 *  @return  U16
 *      -# LCM_REASON_NOT_APPL 
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_MEM_NOAVAIL
 **/
#ifdef ANSI
PRIVATE U16 ysLMMGenCfg
(
YsCfg *cfg            /* Configuaration info */
)
#else
PRIVATE U16 ysLMMGenCfg(cfg)
YsCfg *cfg;            /* Configuaration info */
#endif
{
   U16    ret = LCM_REASON_NOT_APPL;
   Size   memSize;           /* amount of static memory required */
   YsGenCfg   *genCfg;
   U16    idx;
   S16    retVal;

   TRC2(ysLMMGenCfg)

   logInfo("%s", "ysLMMGenCfg(): General Configuration Request");
   /* Check if General Configuration is done already */
   if (ysCb.ysInit.cfgDone == TRUE)
   {
      RETVALUE(LCM_REASON_INVALID_MSGTYPE);
   }

   genCfg = &cfg->s.genCfg;

   /* copy the general configuration structure */
   cmMemcpy((U8 *)&ysCb.genCfg, (U8 *)genCfg, sizeof(YsGenCfg));

   /* Update the Pst structure for LM interface */
   cmMemcpy((U8 *)&ysCb.ysInit.lmPst, (U8 *)&genCfg->lmPst,
             sizeof(Pst));

   ysCb.ysInit.lmPst.srcProcId = ysCb.ysInit.procId;
   /* ysCb.ysInit.lmPst.srcEnt = ysCb.ysInit.ent; */
   ysCb.ysInit.lmPst.srcEnt = ENTTF;
   ysCb.ysInit.lmPst.srcInst = ysCb.ysInit.inst;
   ysCb.ysInit.lmPst.event = EVTNONE;

   /* Calculate the memory requirement */
   memSize = genCfg->maxTfuSaps * (sizeof(YsTfuSapCfg) + sizeof(YsTfuSapCfg *));

   /* reserve static memory */
   if ((retVal = SGetSMem(ysCb.ysInit.region, memSize, &ysCb.ysInit.pool)) 
         != ROK)
      RETVALUE(LCM_REASON_MEM_NOAVAIL);

   memSize = genCfg->maxTfuSaps * sizeof(YsTfuSapCfg *);

   if ((ysCb.tfuSapLst = (YsTfuSapCb **)ysUtlMalloc(memSize)) == NULLP)
   {
      (Void) SPutSMem(ysCb.ysInit.region, ysCb.ysInit.pool);
      RETVALUE(LCM_CAUSE_MEM_ALLOC_FAIL);
   }

   for (idx = 0;  idx < genCfg->maxTfuSaps;  idx++)
   {
      *(ysCb.tfuSapLst + idx) = NULLP;
   }


   /* Set Config done in TskInit */
   ysCb.ysInit.cfgDone = TRUE;
#ifdef MSPD
   ysInitMainThreadPst();
   ysInitMSFramework();

#endif

   RETVALUE(ret);
}

/**
 * @brief SAP Configuration Handler. 
 *
 * @details
 *
 *     Function : ysLMMSapCfg
 *     
 *     This function in called by YsMiLysCfgReq(). It handles the
 *     interface SAP configuration of the CL layer. It 
 *     initializes the sapState to LYS_UNBND. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  YsCfg *cfg, the Configuaration info 
 *  @return  U16
 *      -# LCM_REASON_GENCFG_NOT_DONE
 *      -# LCM_REASON_INVALID_SAP
 *      -# LCM_REASON_NOT_APPL
 **/
#ifdef ANSI
PRIVATE U16 ysLMMSapCfg
(
YsCfg *cfg,            /* Configuaration info */
Elmnt sapType             /* Sap Type */
)
#else
PRIVATE U16 ysLMMSapCfg(cfg,sapType)
YsCfg *cfg;            /* Configuaration info */
Elmnt sapType;            /* Sap Type */
#endif
{
   U16               ret = LCM_REASON_NOT_APPL;
   YsTfuSapCb        *tfuSap = NULLP;
   YsCtfSapCb        *ctfSap = NULLP;

   TRC2(ysLMMSapCfg)

   logInfo("\nysLMMSapCfg(): SAP Config Requested for sapType=%d", sapType);
   /* Check if Gen Config has been done */
   if(ysCb.ysInit.cfgDone != TRUE)
      RETVALUE(LCM_REASON_GENCFG_NOT_DONE);

   switch(sapType)
   {   
      case STCTFSAP:
         ctfSap = &ysCb.ctfSap;
         if(ctfSap->sapState == LYS_NOT_CFG)
         { 
            ctfSap->sapState = LYS_UNBND;
            ctfSap->sapPst.dstEnt = cfg->s.ctfSap.ent;
            ctfSap->sapPst.dstInst = cfg->s.ctfSap.inst;
            ctfSap->sapPst.dstProcId = cfg->s.ctfSap.procId;
            /* ctfSap->sapPst.srcEnt = ysCb.ysInit.ent; */
            ctfSap->sapPst.srcEnt = ENTTF;
            ctfSap->sapPst.srcInst = ysCb.ysInit.inst;
            ctfSap->sapPst.srcProcId = ysCb.ysInit.procId;

           /* With multicore support layer shall use the assigned region
              and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
            ctfSap->sapPst.region = ysCb.ysInit.region;
            ctfSap->sapPst.pool = ysCb.ysInit.pool;
#else
            ctfSap->sapPst.region = cfg->s.ctfSap.mem.region;
            ctfSap->sapPst.pool = cfg->s.ctfSap.mem.pool;
#endif
            ctfSap->sapPst.selector = cfg->s.ctfSap.selector;
            ctfSap->sapPst.route = cfg->s.ctfSap.route;
            ctfSap->sapPst.prior = cfg->s.ctfSap.prior;
            ctfSap->suId = cfg->s.ctfSap.suId;
            ctfSap->spId = cfg->s.ctfSap.spId;
         }
         break;

      case STTFUSAP:

         /* get out the pointer to the SSAP from the SSAP list */
         if (cfg->s.tfuSap.spId >= ysCb.genCfg.maxTfuSaps)
         {
            logError("\nYsMiLysCfgReq(): Invalid SSAP ID %d ", cfg->s.tfuSap.spId);
            RETVALUE(LCM_REASON_INVALID_PAR_VAL);
         }

         /* Get Sap control block */
         tfuSap = ysCb.tfuSapLst[cfg->s.tfuSap.spId];
   
         if(tfuSap != NULLP)
         {
            RETVALUE(ROK);
         }

         /* allocate the SSAP and put it in the SSAP list */
         if ((tfuSap = (YsTfuSapCb *)ysUtlMalloc(sizeof(YsTfuSapCb))) == NULLP)
         {
            RETVALUE(LCM_REASON_MEM_NOAVAIL);
         }

         cmMemcpy((U8 *)&tfuSap->tfuSapCfg, (U8 *)&cfg->s.tfuSap,
                  sizeof(YsTfuSapCfg));

         tfuSap->sapState = LYS_UNBND;
         tfuSap->sapPst.dstEnt = cfg->s.tfuSap.ent;
         tfuSap->sapPst.dstInst = cfg->s.tfuSap.inst;
         tfuSap->sapPst.dstProcId = cfg->s.tfuSap.procId;
         /* tfuSap->sapPst.srcEnt = ysCb.ysInit.ent; */
         tfuSap->sapPst.srcEnt = ENTTF;
         tfuSap->sapPst.srcInst = ysCb.ysInit.inst;
         tfuSap->sapPst.srcProcId = ysCb.ysInit.procId;

         /* With multicore support layer shall use the assigned region
           and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
         tfuSap->sapPst.region = ysCb.ysInit.region;
         tfuSap->sapPst.pool = ysCb.ysInit.pool;
#else
         tfuSap->sapPst.region = cfg->s.tfuSap.mem.region;
         tfuSap->sapPst.pool = cfg->s.tfuSap.mem.pool;
#endif
         tfuSap->sapPst.selector = cfg->s.tfuSap.selector;
         tfuSap->sapPst.route = cfg->s.tfuSap.route;
         tfuSap->sapPst.prior = cfg->s.tfuSap.prior;
         tfuSap->suId = cfg->s.tfuSap.suId;
         tfuSap->spId = cfg->s.tfuSap.spId;
#ifndef YS_MSPD
#else
         tfuSap->type = cfg->s.tfuSap.type;
         tfuSap->cellCb = NULLP;
        /* Place tfuSapCb in ysCb's TfuSap list */
         ysCb.tfuSapLst[cfg->s.tfuSap.spId] = tfuSap;     

         if(cfg->s.tfuSap.type == LYS_TFU_USR_SAP)
            ysCb.macTfuSap = tfuSap;
#if 0
         else
            tfuSap->cellCb = ysMsCfgGetCellCfg(cfg->s.tfuSap.cellId);
#endif
#endif /* YS_MSPD */
         break;

      default:
         /* would never reach here */
         break;
   }
   RETVALUE(ret);
}

/***********************************************************
 *
 *     Func : ysLMMGenCntrl 
 *        
 *
 *     Desc : Processes the LM control request for STGEN elmnt.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : ys_mi.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void ysLMMGenCntrl 
(
YsMngmt       *cntrl,
YsMngmt       *cfm,
Pst           *cfmPst
)
#else
PRIVATE Void ysLMMGenCntrl(cntrl, cfm, cfmPst)
YsMngmt       *cntrl;
YsMngmt       *cfm;
Pst           *cfmPst;
#endif
{
   S16 ret;

   TRC2(ysLMMGenCntrl)

   YS_DBG_INFO((_ysp, "\nysLMMGenCntrl(): Control Req for STGEN"));
   cfm->cfm.status = LCM_PRIM_OK;
   cfm->cfm.reason = LCM_REASON_NOT_APPL;
   
   switch(cntrl->t.cntrl.action)
   {
      case AENA:
         /* Action is Enable */
         switch(cntrl->t.cntrl.subAction)
         {
            case SATRC:
            /* Enable Traces */
               ysCb.ysInit.trc = TRUE;
               ysCb.trcLen = cntrl->t.cntrl.s.trcLen;
               break;

            case SAUSTA:   
            /* Enable Unsolicited Status (alarms) */
               ysCb.ysInit.usta = TRUE;
               break;

            case SADBG:
            /* Enable Debug Printing */
#ifdef DEBUGP
#ifdef MSPD
/*
               ysCb.ysInit.dbgMask |= cntrl->t.cntrl.s.ysDbgCntrl.dbgMask;
               */
#else
               ysCb.ysInit.dbgMask |= cntrl->t.cntrl.s.ysDbgCntrl.dbgMask;
#endif
#endif
               break;

            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               YS_DBG_ERR((_ysp, "\nysLMMGenCntrl(): invalid subaction=%d",
               cntrl->t.cntrl.subAction));
               break;
         }
         break;
      case ADISIMM:
         /* Action is Diable immidiately */
         switch(cntrl->t.cntrl.subAction)
         {
            case SATRC:
            /* Disable Traces */
               ysCb.ysInit.trc = FALSE;
               break;
            case SAUSTA:
            /* Disable Unsolicited Status (alarms) */
               ysCb.ysInit.usta = FALSE;
               break;
            case SADBG:
            /* Disable Debug Printing */
#ifdef DEBUGP
               ysCb.ysInit.dbgMask &=~cntrl->t.cntrl.s.ysDbgCntrl.dbgMask;
#endif
               break;
#if 0
            case SASTOPL1:
               ret = ysLMMStopPhy(cntrl->t.cntrl.s.cellId);
               if(ret != ROK)
               {
                  cfm->cfm.status = LCM_PRIM_NOK;
                  cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               }
               break;
#endif

            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               YS_DBG_ERR((_ysp, "\nysLMMGenCntrl(): invalid subaction=%d",
                            cntrl->t.cntrl.subAction));
               break;
         }
         break;
      case ASHUTDOWN:
         /* Free all the memory dynamically allocated by CL */
         ysLMMShutdown();
         break;
      default:
         cfm->cfm.status = LCM_PRIM_NOK;
         cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
         YS_DBG_ERR((_ysp, "\nysLMMGenCntrl(): invalid action=%d",
                        cntrl->t.cntrl.action));
         break;
   }

   YsMiLysCntrlCfm(cfmPst, cfm);
   RETVOID;
}

/***********************************************************
 *
 *     Func : ysLMMSapCntrl 
 *        
 *
 *     Desc : Processes the LM control request for STxxxSAP elmnt.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : ys_mi.c
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void ysLMMSapCntrl 
(
YsMngmt       *cntrl,
YsMngmt       *cfm,
Pst           *cfmPst
)
#else
PRIVATE Void ysLMMSapCntrl(cntrl, cfm, cfmPst)
YsMngmt       *cntrl;
YsMngmt       *cfm;
Pst           *cfmPst;
#endif
{
   TRC2(ysLMMSapCntrl)

   YS_DBG_INFO((_ysp, "\nysLMMSapCntrl(): Control Req for SAP (%d)",
                  cntrl->hdr.elmId.elmnt));
   /* Only TFU Sap can be controlled by LM */
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STCTFSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               cmMemset((U8 *)&ysCb.ctfSap, 0, sizeof(YsCtfSapCb));
               ysCb.ctfSap.sapState = LYS_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               YS_DBG_ERR((_ysp, "\nysLMMSapCntrl(): invalid action=%d",
                           cntrl->t.cntrl.action));
               break;
         }
         break;

      case STTFUSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               if(ysCb.tfuSapLst[cntrl->t.cntrl.s.ysSapCntrl.spId] != NULLP)
               {
                  ysDeleteTfuSap(ysCb.tfuSapLst
                           [cntrl->t.cntrl.s.ysSapCntrl.spId]);
                  cfm->cfm.status = LCM_PRIM_OK;
                  cfm->cfm.reason = LCM_REASON_NOT_APPL;
               }
               else
               {
                  cfm->cfm.status = LCM_PRIM_NOK;
                  cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
                  YS_DBG_ERR((_ysp, "\nysLMMSapCntrl(): invalid sapId=%d",
                           cntrl->t.cntrl.s.ysSapCntrl.spId));
               }
               break;

            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               YS_DBG_ERR((_ysp, "\nysLMMSapCntrl(): invalid action=%d",
                              cntrl->t.cntrl.action));
               break;
         }
         break;
      default:
         /* Would never here. */
         RETVOID;
   }
   YsMiLysCntrlCfm(cfmPst, cfm);
   RETVOID;
}

/***********************************************************
 *
 *     Func : ysLMMShutdown
 *        
 *
 *     Desc : Handles the CL layer shutdown request.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : ys_mi.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void ysLMMShutdown
(
Void
)
#else
PRIVATE Void ysLMMShutdown()
#endif
{
   U16   idx;
#ifdef YS_MSPD
   YsCellCb  *pCellCb;
   YsCellCb  *nCellCb;
   S16       ret;

   pCellCb = NULLP;
   nCellCb = NULLP;
   ret = ROK;

#endif /* YS_MSPD */
   
   TRC2(ysLMMShutdown)

   YS_DBG_INFO((_ysp, "\nysLMMShutdown(): Layer Shutdown Requested"));

   /* remove the tfusap list memory */
   if (ysCb.tfuSapLst != NULLP)
   {
      for (idx=0; idx < ysCb.genCfg.maxTfuSaps; ++idx)
      {
         if (ysCb.tfuSapLst[idx])
         {
            ysDeleteTfuSap(ysCb.tfuSapLst[idx]);
            ysCb.tfuSapLst[idx] = NULLP;
         }
      }

      /* Free Memory for the TSAP List */
      ysUtlDeAlloc((Data *)ysCb.tfuSapLst, 
                (ysCb.genCfg.maxTfuSaps * sizeof(YsTfuSapCfg *)));

      ysCb.tfuSapLst = NULLP;
   }
#if 0   
#ifdef YS_MSPD
   ret = cmHashListGetNext(&ysCb.cellCfgLst, (PTR)pCellCb, (PTR *)&nCellCb);
   for (;;)
   {
      if (ret != ROK)
      {
         break;
      }
      else
      {
         pCellCb = nCellCb;
         ret = cmHashListGetNext(&ysCb.cellCfgLst, (PTR)pCellCb,
                     (PTR *)&nCellCb);
         ysMsCfgDelCellCfg(pCellCb->cellId); 
      }
   }

   cmHashListDeinit(&ysCb.cellCfgLst);
#else
   for (idx=0; idx < YS_NUM_LOG_CELLS; ++idx)
   {
      if(ysCb.cellCfgLst[idx] != NULLP)
      {
         ysUtlDelCellCfg(ysCb.cellCfgLst[idx]->cellId);
      }
   }


#endif /* YS_MSPD */
#endif

   /* call back the task initialization function to intialize
    * the global YsCb Struct */
   ysActvInit(ysCb.ysInit.ent, ysCb.ysInit.inst, ysCb.ysInit.region, 
              ysCb.ysInit.reason);

   RETVOID;
}

/*
*
*       Fun:   ysDeleteTfuSap
*
*       Desc:  This function implements TFU SAP control.
*
*       Ret:   void
*
*       Notes: None
*
*       File : ys_mi.c 
*
*/
#ifdef ANSI
PRIVATE Void ysDeleteTfuSap
(
YsTfuSapCb           *tfuSap               /* SSAP Control Block */
)
#else
PRIVATE Void ysDeleteTfuSap(tfuSap)
YsTfuSapCb           *tfuSap;              /* SSAP Control Block */
#endif
{

   TRC2(ysDeleteTfuSap)

   YS_DBG_INFO((_ysp, "\nysDeleteTfuSap(): SAP delete Requested"));

   /* remove the TFUSAP from the TFUSAP list */
   *(ysCb.tfuSapLst + tfuSap->spId) = NULLP;
#ifdef YS_MSPD
   if(ysCb.macTfuSap == tfuSap)
   {
      ysCb.macTfuSap = NULLP;
   }
#endif /* YS_MSPD */
   ysUtlDeAlloc((Data *)tfuSap, sizeof(YsTfuSapCb));
   
   RETVOID;

} /* end of ysDeleteTfuSap() */ 

/**
 * @brief Layer Manager Unsolicited Status Indication generation. 
 *
 * @details
 *
 *     Function : ysLMMStaInd 
 *     
 *     This API is used by the other modules of CL to send a unsolicited
 *     status indication to the Layer Manager.
 *     
 *  @param[in]  U16 category, the Alarm category.
 *  @param[in]  U16 event, the Alarm event.
 *  @param[in]  U16 cause, the cause of the Alarm.
 *  @param[in]  YsUstaDgn *dgn, Alarm Diagonostics.
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 ysLMMStaInd
(
U16 category,
U16 event,
U16 cause,
YsUstaDgn *dgn
)
#else
PUBLIC S16 ysLMMStaInd(category, event, cause, dgn) 
U16 category;
U16 event;
U16 cause;
YsUstaDgn *dgn;
#endif
{
   YsMngmt    usta;

   TRC2(ysLMMStaInd)

   YS_DBG_INFO((_ysp, "\nysLMMStaInd(): category=%d, event=%d, cause=%d",
                        category, event, cause));
   
   if(ysCb.ysInit.usta == FALSE)
   {
      YS_DBG_INFO((_ysp, "\nysLMMStaInd(): USTA Ind Disabled."));
      RETVALUE(ROK);
   }

   cmMemset((U8 *)&usta, 0, sizeof(YsMngmt));

   /* fill in the category, event and cause */
   usta.t.usta.cmAlarm.category = category;
   usta.t.usta.cmAlarm.event = event;
   usta.t.usta.cmAlarm.cause = cause;

   SGetDateTime(&usta.t.usta.cmAlarm.dt);
   if (dgn != NULLP)
   {
      cmMemcpy((U8 *)&usta.t.usta.dgn, (U8 *)dgn, sizeof(YsUstaDgn));
   }

   RETVALUE(YsMiLysStaInd(&ysCb.ysInit.lmPst, &usta));
}

/***********************************************************
 *
 *     Func : ysLMMFillCfmPst 
 *        
 *
 *     Desc : Fills the Confirmation Post Structure cfmPst using the reqPst 
 *            and the cfm->hdr.response.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : ys_mi.c
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void ysLMMFillCfmPst
(
Pst           *reqPst,
Pst           *cfmPst,
YsMngmt       *cfm
)
#else
PRIVATE Void ysLMMFillCfmPst(reqPst, cfmPst, cfm)
Pst           *reqPst;
Pst           *cfmPst;
YsMngmt       *cfm;
#endif
{
   TRC2(ysLMMFillCfmPst)

   /* cfmPst->srcEnt    = ysCb.ysInit.ent; */
   cfmPst->srcEnt = ENTTF;
   cfmPst->srcInst   = ysCb.ysInit.inst;
   cfmPst->srcProcId = ysCb.ysInit.procId;
   cfmPst->dstEnt    = reqPst->srcEnt;
   cfmPst->dstInst   = reqPst->srcInst;
   cfmPst->dstProcId = reqPst->srcProcId;

   cfmPst->selector  = cfm->hdr.response.selector;
   cfmPst->prior     = cfm->hdr.response.prior;
   cfmPst->route     = cfm->hdr.response.route;
   cfmPst->region    = cfm->hdr.response.mem.region;
   cfmPst->pool      = cfm->hdr.response.mem.pool;

   RETVOID;
}

/***********************************************************
 *
 *     Func : ysLMMStopPhy
 *        
 *
 *     Desc : Handles the L1 layer shutdown request.
 *            
 *
 *     Ret  : S16
 *
 *     Notes: 
 *
 *     File : ys_mi.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 ysLMMStopPhy
(
CmLteCellId cellId
)
#else
PRIVATE S16 ysLMMStopPhy(cellId)
CmLteCellId cellId;
#endif
{
   S16 ret;
   YsCellCb        *cellCb;
   
   TRC2(ysLMMStopPhy)

   YS_DBG_INFO((_ysp, "\nysLMMStopPhy(): L1 Layer Shutdown Requested"));

   ret = ROK;
#if 0
#ifdef YS_MSPD
   if((cellCb = ysMsCfgGetCellCfg(cellId)) == NULLP) 
   {
      YS_DBG_ERR((_ysp, "Cell(%d) doesn't exist \n", cellId));
      RETVALUE(RFAILED);
   }

   ret = ysMsCfgSm(cellCb, YS_MS_EVENT_STOP, NULLP);
   if (ret != ROK)
   {
      YS_DBG_ERR((_ysp, "sending stop req to PHY failed\n"));
   }

#else
   if((cellCb = ysUtlGetCellCfg(cellId)) == NULLP) 
   {
      YS_DBG_ERR((_ysp, "Cell(%d) doesn't exist \n", cellId));
      RETVALUE(RFAILED);
   }


   if (ret != ROK)
   {
      YS_DBG_ERR((_ysp, "ysPcEncStopReqAndSend2Phy(): failed\n"));
   }
#endif /* YS_MSPD*/
#endif   

   RETVALUE(ret);
}

/********************************************************************30**
  
         End of file:     yw_mi.c@@/main/1 - Thu Dec  2 02:27:26 2010
  
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
/main/1      ---     sgm                   1. eNodeB 1.2 release
/main/1      ys004.102     vr              1. MSPD merge for PHY 1.7
*********************************************************************91*/
