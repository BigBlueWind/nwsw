/********************************************************************16**

                         (c) COPYRIGHT 2011 by 
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

                    Email: support@ccpu.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/************************************************************************
 
     Name:     LTE-MAC layer
 
     Type:     C source file
 
     Desc:     C source code for Entry point fucntions
 
     File:     rg_cfg.c
 
     Sid:      gk_cfg.c@@/main/3 - Sat Jul 30 02:21:24 2011
 
     Prg:     sd
 
**********************************************************************/

/** @file rg_cfg.c
@brief This module handles the configuration of MAC by RRC and RRM.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "crg.h"
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "rg.h"
#include "rg_err.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"           /* layer management typedefs for MAC */
#include "rg.x"            /* typedefs for MAC */

/* LTE-MAC Control Block Structure */
PUBLIC RgCb rgCb;

/* local defines */
PRIVATE S16 rgCFGVldtCrgDedLcCfg ARGS((CrgLchCfg *lcCfg, RgCellCb **cell,
         RgUeCb **ue, RgErrInfo *errInfo));
PRIVATE S16 rgCFGVldtCrgCmnLcCfg ARGS((CrgLchCfg *lcCfg, RgCellCb **cell,
         RgErrInfo *errInfo));
PRIVATE S16 rgCFGCrgDedLcCfg ARGS((RgCellCb *cell, RgUeCb *ue,
         CrgLchCfg *lcCfg, RgErrInfo *errInfo));
PRIVATE S16 rgCFGCrgCmnLcCfg ARGS((RgCellCb *cell, CrgLchCfg *lcCfg,
         RgErrInfo *errInfo));

PRIVATE Void rgCFGFreeUeCb ARGS((RgCellCb *cell, RgUeCb *ue));
PRIVATE Void rgCFGFreeCmnLcLst ARGS((RgCellCb *cell));
PRIVATE Void rgCFGFreeUeLst ARGS((RgCellCb *cell));
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
PRIVATE Void rgCFGFreeSpsUeLst ARGS((RgCellCb *cell));
#endif /* LTEMAC_SPS */



/* local typedefs */
 
/* local externs */
 
/* forward references */


#if 0

/**
 * @brief Validates the cell configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgCellCfg
 *
 *     Processing Steps:
 *      - Validate the cell configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  CrgCellCfg  *cellCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgCellCfg
(
CrgCellCfg  *cellCfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgCellCfg(cellCfg, errInfo)
CrgCellCfg  *cellCfg;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgCellCfg);
   RGDBGPRM((rgPBuf, "VALIDATE CRG CELL CONFIG: cellId %d\n", cellCfg->cellId));

   errInfo->errCause = RGERR_CFG_INVALID_CRG_CELL_CFG;

   if ((rgDBMGetCellCb(cellCfg->cellId) != NULLP)
         || rgDBMGetInactvCellCb(cellCfg->cellId) != NULLP)
   {
      RGDBGERR((rgPBuf, "Cell already exists %d\n",cellCfg->cellId));
      RETVALUE(RFAILED);
   }
   if ((cellCfg->bwCfg.dlTotalBw < RG_MIN_DL_BW
            || cellCfg->bwCfg.dlTotalBw > RG_MAX_DL_BW)
         || (cellCfg->bwCfg.ulTotalBw < RG_MIN_UL_BW
            || cellCfg->bwCfg.ulTotalBw > RG_MAX_UL_BW))
   {
      RGDBGERR((rgPBuf, "Invalid Bandwidth configuration: ul %d dl %d",
            cellCfg->bwCfg.ulTotalBw, cellCfg->bwCfg.dlTotalBw));
      RETVALUE(RFAILED);
   }
   if (cellCfg->rachCfg.maxMsg3Tx < RG_MIN_HQ_TX)
   {
      RGDBGERR((rgPBuf, "Invalid RACH configuration: maxMsg3Tx %d",
            cellCfg->rachCfg.maxMsg3Tx));
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG cell config validation done: cellId %d\n",
            cellCfg->cellId));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgCellCfg */


/**
 * @brief Validates the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeCfg
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  CrgUeCfg  *ueCfg
 *  @param[out] RgCellCb  **cell
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgUeCfg
(
CrgUeCfg  *ueCfg,
RgCellCb  **cell,
RgErrInfo *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgUeCfg(ueCfg, cell, errInfo)
CrgUeCfg  *ueCfg;
RgCellCb  **cell;
RgErrInfo *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgUeCfg);
   RGDBGPRM((rgPBuf, "VALIDATE CRG UE CONFIG: cellId %d ueId %d cell %p\n",
         ueCfg->cellId, ueCfg->crnti, (void*)*cell));

   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_CFG;
#ifdef LTEMAC_MIMO
   if ((ueCfg->txMode.pres == PRSNT_NODEF) && 
       (ueCfg->txMode.tm == CRG_UE_TM_5))
   {
      RGDBGERR((rgPBuf, "Transmission Mode=%d not supported\n", ueCfg->txMode.tm));
      RETVALUE(RFAILED);
   }
#endif
   /* Fetch the Active cell */
   if ((*cell = rgDBMGetCellCb(ueCfg->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Active Cell does not exist %d\n", ueCfg->cellId));
      RETVALUE(RFAILED);
   }
   /* Check if Ue already configured */
   if (rgDBMGetUeCb(*cell, ueCfg->crnti) != NULLP)
   {
      RGDBGERR((rgPBuf, "Ue already exists %d\n", ueCfg->crnti));
#ifdef PWAV
	  MSPD_DBG("Previous UE not deleted:%d\n", ueCfg->crnti);
#endif
      RETVALUE(RFAILED);
   }
#ifdef PWAV
   MSPD_DBG("UE configured at MAC:%d\n", ueCfg->crnti);
#endif
   if (ueCfg->ueUlHqCfg.maxUlHqTx < RG_MIN_HQ_TX)
   {
      RGDBGERR((rgPBuf, "Invalid Uplink HARQ config for UE %d\n",
            ueCfg->ueUlHqCfg.maxUlHqTx));
      RETVALUE(RFAILED);
   }
   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG UE config validation done: cellId %d ueId %d\n",
        ueCfg->cellId, ueCfg->crnti));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgUeCfg */


/**
 * @brief Validates the logical channel configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgLcCfg
 *
 *     Processing Steps:
 *      - Validate the logical channel configuration request from RRC to
 *        MAC at CFG: validate if configured values are within the range.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell for common channels. Return
 *          pointer to cell and UE for dedicated logical channels.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  CrgLchCfg  *lcCfg
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgLcCfg
(
CrgLchCfg  *lcCfg,
RgCellCb   **cell,
RgUeCb     **ue,
RgErrInfo  *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgLcCfg(lcCfg, cell, ue, errInfo)
CrgLchCfg  *lcCfg;
RgCellCb   **cell;
RgUeCb     **ue;
RgErrInfo  *errInfo;
#endif
{
   S16     ret;

   TRC2(rgCFGVldtCrgLcCfg);
   RGDBGPRM((rgPBuf, "VALIDATE CRG LC CONFIG: cellId %d ueId %d lcId %d\
            cell %p ue %p\n", lcCfg->cellId, lcCfg->crnti, lcCfg->lcId,
            (void*)*cell, (void*)*ue));

   if (lcCfg->lcType == CM_LTE_LCH_DTCH || lcCfg->lcType == CM_LTE_LCH_DCCH)
   {
      /* Dedicated logical channels */
      if ((ret = rgCFGVldtCrgDedLcCfg(lcCfg, cell, ue, errInfo)) != ROK)
      {
         RGDBGERR((rgPBuf, "Validation for dedicated LC failed\n"));
         RETVALUE(RFAILED);
      }
   }
   else if (lcCfg->lcType == CM_LTE_LCH_BCCH
         || lcCfg->lcType == CM_LTE_LCH_PCCH
         || lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      if ((ret = rgCFGVldtCrgCmnLcCfg(lcCfg, cell, errInfo)) != ROK)
      {
         RGDBGERR((rgPBuf, "Validation for common logical channels failed\n"));
         RETVALUE(RFAILED);
      }
   }
   else
   {
      RGDBGERR((rgPBuf, "Invalid logical channel type %d\n", lcCfg->lcType));
      RETVALUE(RFAILED);
   }
   /*rg002.301:ccpu00120621-ADD-validations for QCI */
#ifdef LTE_L2_MEAS
   if ( lcCfg->qci <  RG_QCI_MIN ||
        lcCfg->qci >  RG_QCI_MAX
      )
   {
      RGDBGERR((rgPBuf, "Invalid qci %x\n", lcCfg->qci));
      RETVALUE(RFAILED);
   }
   /*validate qci */
#endif /*LTE_L2_MEAS */

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG LC config validation done: cellId %d ueId %d\
            lcId %d\n", lcCfg->cellId, lcCfg->crnti, lcCfg->lcId));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgLcCfg */


/**
 * @brief Validates the cell re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgCellRecfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  CrgCellRecfg  *cellRecfg
 *  @param[out] RgCellCb      **cell
 *  @param[out] RgErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgCellRecfg
(
CrgCellRecfg  *cellRecfg,
RgCellCb      **cell,
RgErrInfo     *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgCellRecfg(cellRecfg, cell, errInfo)
CrgCellRecfg  *cellRecfg;
RgCellCb      **cell;
RgErrInfo     *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgCellRecfg);
   RGDBGPRM((rgPBuf, "VALIDATE CRG CELL RECONFIG: cellId %d cell %p\n",
         cellRecfg->cellId, (void*)*cell));

   errInfo->errCause = RGERR_CFG_INVALID_CRG_CELL_RECFG;
   
   if (((*cell = rgDBMGetCellCb(cellRecfg->cellId)) == NULLP)
         && ((*cell = rgDBMGetInactvCellCb(cellRecfg->cellId)) == NULLP))
   {
      RGDBGERR((rgPBuf, "Cell does not exist %d\n",cellRecfg->cellId));
      RETVALUE(RFAILED);
   }
   if (cellRecfg->rachRecfg.maxMsg3Tx < RG_MIN_HQ_TX)
   {
      RGDBGERR((rgPBuf, "Invalid RACH configuration: maxMsg3Tx %d",
            cellRecfg->rachRecfg.maxMsg3Tx));
      RETVALUE(RFAILED);
   }
   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG Cell Reconfig validation done: cellId %d\n",
            cellRecfg->cellId));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgCellRecfg */


/**
 * @brief Validates the UE re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeRecfg
 *
 *     Processing Steps:
 *      - Retrieve the UE control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell and ue.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  CrgUeRecfg *ueRecfg
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgUeRecfg
(
CrgUeRecfg  *ueRecfg,
RgCellCb    **cell,
RgUeCb      **ue,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgUeRecfg(ueRecfg, cell, ue, errInfo)
CrgUeRecfg  *ueRecfg;
RgCellCb    **cell;
RgUeCb      **ue;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgUeRecfg);
   RGDBGPRM((rgPBuf, "VALIDATE CRG UE RECONFIG: cellId %d oldUeId %d\
            cell %p ue %p\n", ueRecfg->cellId, ueRecfg->oldCrnti, (void*)*cell,
            (void*)*ue));

   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_RECFG;
   
#ifdef LTEMAC_MIMO
   if ((ueRecfg->txMode.pres == PRSNT_NODEF) && 
       (ueRecfg->txMode.tm == CRG_UE_TM_5))
   {
      RGDBGERR((rgPBuf, "Transmission Mode=%d not supported\n", ueRecfg->txMode.tm));
      RETVALUE(RFAILED);
   }
#endif
   /* Fetch the Active cell */
   if ((*cell = rgDBMGetCellCb(ueRecfg->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Active Cell does not exist %d\n", ueRecfg->cellId));
      RETVALUE(RFAILED);
   }

   /* Fetch the Old Ue */
   if ((*ue = rgDBMGetUeCb(*cell, ueRecfg->oldCrnti)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Old Ue does not exist %d\n", ueRecfg->oldCrnti));
      RETVALUE(RFAILED);
   }

   if (ueRecfg->oldCrnti != ueRecfg->newCrnti)
   {
      if (rgDBMGetUeCb(*cell, ueRecfg->newCrnti) != NULLP)
      {
         RGDBGERR((rgPBuf, "New Ue already exists %d\n", ueRecfg->newCrnti));
         RETVALUE(RFAILED);
      }
   }
   if (ueRecfg->ueUlHqRecfg.maxUlHqTx < RG_MIN_HQ_TX)
   {
      RGDBGERR((rgPBuf, "Invalid Uplink HARQ config for UE %d\n",
            ueRecfg->ueUlHqRecfg.maxUlHqTx));
      RETVALUE(RFAILED);
   }
   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG Ue reconfig validation done: cellId %d oldUeId %d\n",
         ueRecfg->cellId, ueRecfg->oldCrnti));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgUeRecfg */


/**
 * @brief Validates the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgLcRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  CrgLchRecfg  *lcRecfg
 *  @param[out] RgCellCb     **cell
 *  @param[out] RgUeCb       **ue
 *  @param[out] RgUlLcCb     **ulLc
 *  @param[out] RgDlLcCb     **dlLc
 *  @param[out] RgErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgLcRecfg
(
CrgLchRecfg *lcRecfg,
RgCellCb    **cell,
RgUeCb      **ue,
RgUlLcCb    **ulLc,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgLcRecfg(lcRecfg, cell, ue, ulLc, errInfo)
CrgLchRecfg  *lcRecfg;
RgCellCb     **cell;
RgUeCb       **ue;
RgUlLcCb     **ulLc;
RgErrInfo    *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgLcRecfg);
   RGDBGPRM((rgPBuf, "VALIDATE CRG LC RECONFIG: cellId %d ueId %d \
            lcId %d cell %p ue %p ulLc %p",lcRecfg->cellId,
            lcRecfg->crnti,lcRecfg->lcId, (void*)*cell, (void*)*ue,
            (void*)*ulLc));

   errInfo->errCause = RGERR_CFG_INVALID_CRG_LC_RECFG;

   /* Fetch the cell */
   if ((*cell = rgDBMGetCellCb(lcRecfg->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Active Cell does not exist %d\n", lcRecfg->cellId));
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgDBMGetUeCb(*cell, lcRecfg->crnti)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Ue does not exist for dedicated logical channel %d\n",
               lcRecfg->crnti));
      RETVALUE(RFAILED);
   }

   if ((*ulLc = rgDBMGetUlDedLcCb((*ue), lcRecfg->lcId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Dedicated UL LC does not exist %d\n",
               lcRecfg->lcId));
      RETVALUE(RFAILED);
   }

   if (lcRecfg->ulRecfg.lcgId > (RG_MAX_LCG_PER_UE - 1))
   {
      RGDBGERR((rgPBuf, "Invalid lcgId for uplink logical channel: lcg %d\
               lc %d\n", lcRecfg->ulRecfg.lcgId, lcRecfg->lcId));
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG Lc Reconfig validation done: cellId %d ueId %d\
            lcId %d\n", lcRecfg->cellId, lcRecfg->crnti,lcRecfg->lcId));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgLcRecfg */

/* Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Validates the UE Reset request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeReset
 *
 *     Processing Steps:
 *      - Retrieve the CELL control block.
 *      - If Failue,
 *          - Return RFAILED.
 *      - Retrieve the UE control block.
 *      - If Failue,
 *          - Return RFAILED.
 *
 *  @param[in]  CrgRst     *reset,
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgUeReset
(
CrgRst     *reset,
RgCellCb    **cell,
RgUeCb      **ue,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgUeReset(reset, cell, ue, errInfo)
CrgRst     *reset;
RgCellCb    **cell;
RgUeCb      **ue;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgUeReset);
   RGDBGPRM((rgPBuf, "VALIDATE CRG UE RESET: cellId %d UeId %d\n",
            reset->cellId, reset->crnti));

   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_RESET;
   
   /* Fetch the Active cell */
   if ((*cell = rgDBMGetCellCb(reset->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Active Cell does not exist %d\n", reset->cellId));
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((*ue = rgDBMGetUeCb(*cell, reset->crnti)) == NULLP)
   {
      RGDBGERR((rgPBuf, "UE does not exist %d\n", reset->crnti));
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG UE reset validation done: cellId %d UeId %d\n",
         reset->cellId, reset->crnti));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgUeReset*/
/* End: LTEMAC_2.1_DEV_CFG */



/**
 * @brief Handler for the cell configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellCfg
 *
 *     Processing Steps:
 *      - Allocate and create cell control block.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - Add the control block to hash list of cells.
 *      - Update the statistics.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  CrgCellCfg  *cellCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgCellCfg
(
CrgCellCfg  *cellCfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgCellCfg(cellCfg, errInfo)
CrgCellCfg  *cellCfg;
RgErrInfo   *errInfo;
#endif
{
   S16        ret;
   RgCellCb   *cell = NULLP;
   U8         idx;

   TRC2(rgCFGCrgCellCfg);
   RGDBGPRM((rgPBuf, "APPLYING CRG CELL CONFIG: cellId %d\n",
         cellCfg->cellId));

   errInfo->errCause = RGERR_CFG_CRG_CELL_CFG;
   
   /* Allocate the cell control block */
   if((ret = rgAllocSBuf((Data**)&cell, sizeof(RgCellCb))) != ROK)
   {
      RGDBGERR((rgPBuf, "Memory allocation FAILED for cell %d",
               cellCfg->cellId));
      RETVALUE(RFAILED);
   }
   if (cell == NULLP)
   {
      RGDBGERR((rgPBuf, "Memory allocation FAILED for cell %d",
               cellCfg->cellId));
      RETVALUE(RFAILED);
   }

   /* Initialize the cell */
   cell->cellId  = cellCfg->cellId;
   cell->rachCfg = cellCfg->rachCfg;
   cell->bwCfg   = cellCfg->bwCfg;

   /* Initialize UL and DL CCCH logical channels */
   cell->ulCcchId = RG_INVALID_LC_ID;
   cell->dlCcchId = RG_INVALID_LC_ID;

   
   /* Initialize the lists of the cell */
   ret = rgDBMInitCell(cell);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf, "DBM initialization for cell %d failed\n",
               cellCfg->cellId));
      rgCFGFreeInactvCellCb(cell);
      RETVALUE(RFAILED);
   }

   for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
   {
      cell->subFrms[idx].txDone = TRUE;
   }
   
   /* Insert cell in the incative cell list */
   rgDBMInsInactvCellCb(cell);
#ifdef LTE_L2_MEAS
   cmLListInit(&cell->l2mList);
   for(idx = 0; idx < RG_NUM_UL_SUB_FRAMES; idx++)
   {
      cmMemset((U8 *)&cell->ulSf[idx], 0, sizeof(RgUlSf));
   }
#endif
   /* Update Statistics */
   rgUpdtCellCnt(RG_CFG_ADD);
   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG Cell configuration done %d\n",cellCfg->cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgCellCfg */


/**
 * @brief Handler for the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeCfg
 *
 *     Processing Steps:
 *      - Allocate and create UE control block.
 *      - Update UE control block with the values recieved in the
 *        configuration.
 *      - Invoke RAM, SCH, UHM and DHM with created UE control block, to
 *        update random access, scheduler, uplink harq and downlink harq
 *        specific information respectively.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  RgCellCb  *cell
 *  @param[in]  CrgUeCfg  *ueCfg
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgUeCfg
(
RgCellCb  *cell,
CrgUeCfg  *ueCfg,
RgErrInfo *errInfo
)
#else
PUBLIC S16 rgCFGCrgUeCfg(cell, ueCfg, errInfo)
RgCellCb  *cell;
CrgUeCfg  *ueCfg;
RgErrInfo *errInfo;
#endif
{
   RgUeCb    *ue = NULLP;
   Bool      handover = FALSE;

   TRC2(rgCFGCrgUeCfg);
   RGDBGPRM((rgPBuf, "APPLYING CRG UE CONFIG: cellId %d ueId %d cell %p\n",
            ueCfg->cellId, ueCfg->crnti, (void*)cell));

   errInfo->errCause = RGERR_CFG_CRG_UE_CFG;

/* Start: LTEMAC_2.1_DEV_CFG */
   if ((ue = rgDBMGetUeCbFromRachLst(cell, ueCfg->crnti)) == NULLP)
   {
      /* Create UeCb and Insert in Rach List */
      if((ue=rgRAMCreateUeCb(cell, ueCfg->crnti, FALSE, errInfo)) == NULLP)
      {
         RGDBGERR((rgPBuf, "UeCb creation failed for UE %d\n", ueCfg->crnti));
         RETVALUE(RFAILED);
      }
      /*rg001.301 -MOD- ccpu00119113 Changes for the crash */
      if(rgDHMHqEntInit(&ue->dl.hqEnt, cell->maxDlHqProcPerUe) != ROK)
      {
         RGDBGERR((rgPBuf,"UeCb Harq Entity Initialization failed for HO UE %d \n", ueCfg->crnti));
         RETVALUE(RFAILED);
      }


      handover = TRUE;
   }
/* End: LTEMAC_2.1_DEV_CFG */

   if(handover == FALSE)
   {
      /* Remove from the rachLst */
      rgDBMDelUeCbFromRachLst(cell, ue);
   }

   /* Initialize uplink HARQ related information for UE */
   rgUHMCrgUeCfg(cell, ue, ueCfg);

   rgDBMInsUeCb(cell, ue);

   /* Update satistics */
   rgUpdtUeCnt(RG_CFG_ADD);
   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "Crg Ue configuration done: cellId %d ueId %d\n",
            ueCfg->cellId, ueCfg->crnti));
   RETVALUE(ROK);
}  /* rgCFGCrgUeCfg */


/**
 * @brief Handler for the logical channel configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcCfg
 *
 *     Processing Steps:
 *      - Check if the configuration is for dedicated or common logical channel.
 *      - For Dedicated logical channels:
 *        - Update the dedicated logical channel Cb with the configured values.
 *        - Invoke SCH will cell, UE and logical channel Cb to update scheduler
 *          specific information.
 *      - For Common logical channels:
 *        - Update the common logical channel Cb with the configured values.
 *        - Move cell to active list of cells if cell becomes ACTIVE.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgCellCb    *cell
 *  @param[in]  RgUeCb      *ue
 *  @param[in]  CrgLchCfg   *lcCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgLcCfg
(
RgCellCb    *cell,
RgUeCb      *ue,
CrgLchCfg   *lcCfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgLcCfg(cell, ue, lcCfg, errInfo)
RgCellCb    *cell;
RgUeCb      *ue;
CrgLchCfg   *lcCfg;
RgErrInfo   *errInfo;
#endif
{
   S16     ret;

   TRC2(rgCFGCrgLcCfg);
   RGDBGPRM((rgPBuf, "APPLYING CRG LC CONFIG: cellId %d ueId %d\
            lcId %d dir %d cell %p ue %p\n", lcCfg->cellId, lcCfg->crnti,
            lcCfg->lcId, lcCfg->dir, (void*)cell, (void*)ue));
   
   /* Handle Config for dedicated/common logical channels */
   if (lcCfg->lcType == CM_LTE_LCH_DTCH || lcCfg->lcType == CM_LTE_LCH_DCCH)
   {
      if ((ret = rgCFGCrgDedLcCfg(cell, ue, lcCfg, errInfo)) != ROK)
      {
         RGDBGERR((rgPBuf, "Dedicated logical channel configuration\
                  failed %d\n", lcCfg->lcId));
         RETVALUE(RFAILED);
      }
   }
   else
   {
      if ((ret = rgCFGCrgCmnLcCfg(cell, lcCfg, errInfo)) != ROK)
      {
         RGDBGERR((rgPBuf, "Common logical channel configuration failed %d\n",
               lcCfg->lcId));
         RETVALUE(RFAILED);
      }
   }

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG LC config done: cellId %d ueId %d lcId %d\n",
         lcCfg->cellId, lcCfg->crnti, lcCfg->lcId));
   RETVALUE(ROK);
}  /* rgCFGCrgLcCfg */


/**
 * @brief Handler for the cell re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellRecfg
 *
 *     Processing Steps:
 *      - Invoke SCH with updated Cell Cb to update scheduler specific
 *        parameters.
 *      - Update the cell Cb with the reconfigured values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgCellCb      *cell
 *  @param[in]  CrgCellRecfg  *cellRecfg
 *  @param[out] RgErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgCellRecfg
(
RgCellCb      *cell,
CrgCellRecfg  *cellRecfg,
RgErrInfo     *errInfo
)
#else
PUBLIC S16 rgCFGCrgCellRecfg(cell, cellRecfg, errInfo)
RgCellCb      *cell;
CrgCellRecfg  *cellRecfg;
RgErrInfo     *errInfo;
#endif
{
   TRC2(rgCFGCrgCellRecfg);
   RGDBGPRM((rgPBuf, "APPLYING CRG CELL RECONFIG: cellId %d cell %p\n",
         cellRecfg->cellId, (void*)cell));

   /* Store the given rachCfg */
   cell->rachCfg = cellRecfg->rachRecfg;

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "CRG Cell reconfiguration done %d\n", cellRecfg->cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgCellRecfg */


/**
 * @brief Handler for the UE re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeRecfg
 *
 *     Processing Steps:
 *      - If rnti changes,
 *        - Invoke RAM for UE reconfiguration.
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *      - Update the UE control block with the reconfigured values.
 *      - Invoke SCH, UHM and DHM with updated UE control block to 
 *        update scheduler, uplink HARQ and downlink HARQ specific
 *        parameters.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgCellCb    *cell
 *  @param[in]  RgUeCb      *ue
 *  @param[in]  CrgUeRecfg  *ueRecfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgUeRecfg
(
RgCellCb    *cell,
RgUeCb      *ue,
CrgUeRecfg  *ueRecfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgUeRecfg(cell, ue, ueRecfg, errInfo)
RgCellCb    *cell;
RgUeCb      *ue;
CrgUeRecfg  *ueRecfg;
RgErrInfo   *errInfo;
#endif
{
   U32 cnt;
   /* RRC ReEst */
   RgContResId   contResId;
   RgUeCb        *newCrntiUeCb=NULLP;
   TRC2(rgCFGCrgUeRecfg);
   RGDBGPRM((rgPBuf, "APPLYING CRG UE RECONFIG: cellId %d oldUeId %d\
            newUeId %d cell %p ue %p\n", ueRecfg->cellId, ueRecfg->oldCrnti,
         ueRecfg->newCrnti, (void*)cell, (void*)ue));

   errInfo->errCause = RGERR_CFG_CRG_UE_RECFG;

   /* Handle CRNTI change in reconfiguration */
   if (ueRecfg->oldCrnti != ueRecfg->newCrnti)
   {
      /* rg004.201: CR ccpu00105882: Releasing HARQ processes of old UE when
       * ue reconfig with new crnti */
      /* Release All HARQ Procs before HqEntity Free */
      for(cnt = 0; cnt < ue->dl.hqEnt.numHqProcs; cnt++)
      {
         rgDHMRlsHqProcTB(&ue->dl.hqEnt.procs[cnt], 1);
         rgDHMRlsHqProcTB(&ue->dl.hqEnt.procs[cnt], 2);
      }
      /* Delete Ue from the ue list */
      rgDBMDelUeCb(cell, ue);

      /* Inititialize Ue control block */
      ue->ueId = ueRecfg->newCrnti;

      /* MS_WORKAROUND for ccpu00121588 */
      newCrntiUeCb = rgDBMGetUeCbFromRachLst(cell, ueRecfg->newCrnti);
      if (newCrntiUeCb == NULLP)
      {
#ifdef MSPD
          MSPD_DBG("rgDBMGetUeCbFromRachLst Failed\n");
#endif /* MSPD */
	  RETVALUE(RFAILED);
      }
      cmMemcpy((U8*)&(ue->contResId), (U8*)&(newCrntiUeCb->contResId), sizeof(newCrntiUeCb->contResId));
       

   /*RE EST MAC FIX End */

      /* Re-insert updated Ue */
      rgDBMInsUeCb(cell, ue);
   }

   /* Update uplink HARQ related information for UE */
   rgUHMCrgUeRecfg(cell, ue, ueRecfg);

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "Ue reconfigured: oldUeId %d newUeId %d cell %d\n",
         ueRecfg->oldCrnti, ueRecfg->newCrnti, ueRecfg->cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgUeRecfg */


/**
 * @brief Handler for the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the re-configured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  RgUlLcCb    *ulLc
 *  @param[in]  RgDlLcCb    *dlLc
 *  @param[in]  CrgLchRecfg *lcRecfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgLcRecfg
(
RgCellCb    *cell,
RgUeCb      *ue,
RgUlLcCb    *ulLc,
CrgLchRecfg *lcRecfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgLcRecfg(cell, ue, ulLc, lcRecfg, errInfo)
RgCellCb    *cell;
RgUeCb      *ue;
RgUlLcCb    *ulLc;
CrgLchRecfg *lcRecfg;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGCrgLcRecfg);
   RGDBGPRM((rgPBuf, "APPLYING CRG LC RECONFIG: cellId %d ueId %d\
            lcId %d cell %p ue %p ulLc %p \n",
            lcRecfg->cellId, lcRecfg->crnti, lcRecfg->lcId, 
            (void*)cell, (void*)ue, (void*)ulLc));

   if (ulLc->lcgId != lcRecfg->ulRecfg.lcgId)
   {
      rgDBMUpdUlDedLcCb(ue, ulLc, lcRecfg->ulRecfg.lcgId);
   }

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "LC %d of Ue %d of cell %d Reconfigured\n", 
            lcRecfg->lcId, ue->ueId, cell->cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgLcRecfg */

/* Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Handler for the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeReset
 *
 *     Processing Steps:
 *
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  CrgRst     *reset
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgUeReset
(
RgCellCb    *cell,
RgUeCb      *ue,
CrgRst     *reset,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgUeReset(cell, ue, reset, errInfo)
RgCellCb    *cell;
RgUeCb      *ue;
CrgRst     *reset;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGCrgUeReset);
   RGDBGPRM((rgPBuf, "APPLYING CRG UE RESET: cellId %d ueId %d\n",
            reset->cellId, reset->crnti));

   rgDHMUeReset(&ue->dl.hqEnt);

   errInfo->errCause = RGERR_NONE;
   
   RGDBGINFO((rgPBuf, "UE %d of cell %d Reset\n", ue->ueId, cell->cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgUeReset */
/* End: LTEMAC_2.1_DEV_CFG */

/**
 * @brief Handler for the cell delete request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellDel
 *
 *     Processing Steps:
 *      - Fetch the cell control block.
 *      - Remove the cell control block from the hash list of cells.
 *      - Free the cell control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  CrgDel      *cellDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgCellDel
(
CrgDel      *cellDelInfo,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgCellDel(cellDelInfo, errInfo)
CrgDel      *cellDelInfo;
RgErrInfo   *errInfo;
#endif
{
   RgCellCb      *cell;
   U8 idx;

   TRC2(rgCFGCrgCellDel);
   RGDBGPRM((rgPBuf, "APPLYING CRG CELL DELETE: cellId %d\n",
            cellDelInfo->u.cellDel.cellId));

   errInfo->errCause = RGERR_CFG_CRG_CELL_DEL;

   if ((cell = rgDBMGetCellCb(cellDelInfo->u.cellDel.cellId)) == NULLP)
   {
      if ((cell = rgDBMGetInactvCellCb(cellDelInfo->u.cellDel.cellId))
            == NULLP)
      {

         RGDBGERR((rgPBuf, "Cell does not exist %d\n",
                  cellDelInfo->u.cellDel.cellId));
         RETVALUE(RFAILED);
      }

      /* Delete cell from inactive list */
      rgDBMDelInactvCellCb(cell);

      /* Free the inactive cell */
      rgCFGFreeInactvCellCb(cell);

      errInfo->errCause = RGERR_NONE;
      RGDBGINFO((rgPBuf, "Inactive cell %d deleted\n", cellDelInfo->u.cellDel.cellId));
      RETVALUE(ROK);
   }

   /* Delete from the cell list */
   rgDBMDelCellCb(cell);

   for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
   {
      rgTOMRlsSf(&cell->subFrms[idx]);
   }

   /* Free the active cell */
   rgCFGFreeCellCb(cell);

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "Cell %d deleted\n", cellDelInfo->u.cellDel.cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgCellDel */


/**
 * @brief Handler for the UE delete request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeDel
 *
 *     Processing Steps:
 *      - Fetch the UE control block.
 *      - Remove the UE control block from the hash list of UEs for the cell.
 *      - Free the UE control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  CrgDel      *ueDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgUeDel
(
CrgDel      *ueDelInfo,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgUeDel(ueDelInfo, errInfo)
CrgDel      *ueDelInfo;
RgErrInfo   *errInfo;
#endif
{
   RgCellCb      *cell;
   RgUeCb        *ue;

   TRC2(rgCFGCrgUeDel);
   RGDBGPRM((rgPBuf, "APPLYING CRG UE DELETE: cellId %d ueId %d\n",
         ueDelInfo->u.ueDel.cellId, ueDelInfo->u.ueDel.crnti));
   
   errInfo->errCause = RGERR_CFG_CRG_UE_DEL;

   if ((cell = rgDBMGetCellCb(ueDelInfo->u.ueDel.cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Cell does not exist %d\n",ueDelInfo->u.ueDel.cellId));
      RETVALUE(RFAILED);
   }
   if ((ue = rgDBMGetUeCb(cell, ueDelInfo->u.ueDel.crnti)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Ue does not exist %d\n",ueDelInfo->u.ueDel.crnti));
      RETVALUE(RFAILED);
   }
#ifdef PWAV
   MSPD_DBG("MAC Deleted:%d\n", ueDelInfo->u.ueDel.crnti);
#endif
   /* Delete Ue from the UE list */
   rgDBMDelUeCb(cell, ue);

   /* Free Ue */
   rgCFGFreeUeCb(cell, ue);

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "Ue %d deleted from cell %d \n",
            ueDelInfo->u.ueDel.crnti, ueDelInfo->u.ueDel.cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgUeDel */


/**
 * @brief Handler for the logical channel delete request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  CrgDel      *lcDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgLcDel
(
CrgDel      *lcDelInfo,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgLcDel(lcDelInfo, errInfo)
CrgDel      *lcDelInfo;
RgErrInfo   *errInfo;
#endif
{
   Bool      dirVld = FALSE;
   RgCellCb  *cell;
   RgUeCb    *ue;
   RgUlLcCb  *ulLc;
   RgDlLcCb  *dlLc;

   TRC2(rgCFGCrgLcDel);
   RGDBGPRM((rgPBuf, "APPLYING CRG LC DELETE: cellId %d ueId %d\
            lcId %d dir %d\n", lcDelInfo->u.lchDel.cellId,
            lcDelInfo->u.lchDel.crnti, lcDelInfo->u.lchDel.lcId,
            lcDelInfo->u.lchDel.dir));

   errInfo->errCause = RGERR_CFG_CRG_LC_DEL;

   /* Fetch the Active cell */
   if ((cell = rgDBMGetCellCb(lcDelInfo->u.lchDel.cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Cell does not exist %d\n",lcDelInfo->u.lchDel.cellId));
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((ue = rgDBMGetUeCb(cell, lcDelInfo->u.lchDel.crnti)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Ue %d does not exist for dedicated logical channel\n",
            lcDelInfo->u.lchDel.crnti));
      RETVALUE(RFAILED);
   }

   /* Validate downlink info */
   if (lcDelInfo->u.lchDel.dir & CRG_DIR_TX)
   {
      if ((dlLc = rgDBMGetDlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
            == NULLP)
      {
         RGDBGERR((rgPBuf, "DL LC %d does not exist for UE %d\n",
               lcDelInfo->u.lchDel.lcId, lcDelInfo->u.lchDel.crnti));
         RETVALUE(RFAILED);
      }
      rgDBMDelDlDedLcCb(ue, dlLc);
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcDelInfo->u.lchDel.dir & CRG_DIR_RX)
   {
      if ((ulLc = rgDBMGetUlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
            == NULLP)
      {
         RGDBGERR((rgPBuf, "UL LC %d does not exist for UE %d\n",
               lcDelInfo->u.lchDel.lcId, lcDelInfo->u.lchDel.crnti));
         RETVALUE(RFAILED);
      }
      rgDBMDelUlDedLcCb(ue, ulLc);
      dirVld = TRUE;
   }

   if (!dirVld)
   {
      RGDBGERR((rgPBuf, "Invalid direction %d for LC Delete\n",
            lcDelInfo->u.lchDel.dir));
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGERR_NONE;
   RGDBGINFO((rgPBuf, "Logical channel %d deleted from Ue %d from cell %d\n",
         lcDelInfo->u.lchDel.lcId, lcDelInfo->u.lchDel.crnti,
         lcDelInfo->u.lchDel.cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgLcDel */

/***********************************************************
 *
 *     Func : rgCFGVldtCrgDedLcCfg
 *
 *
 *     Desc : Validates dedicated logical channel configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgCFGVldtCrgDedLcCfg
(
CrgLchCfg     *lcCfg,
RgCellCb      **cell,
RgUeCb        **ue,
RgErrInfo     *errInfo
)
#else
PRIVATE S16 rgCFGVldtCrgDedLcCfg(lcCfg, cell, ue, errInfo)
CrgLchCfg     *lcCfg;
RgCellCb      **cell;
RgUeCb        **ue;
RgErrInfo     *errInfo;
#endif
{
   U8         dirVld   = FALSE;
   TRC2(rgCFGVldtCrgDedLcCfg);
   RGDBGPRM((rgPBuf, "VALIDATING CRG DEDICATED LC CONFIG"));

   errInfo->errCause = RGERR_CFG_INVALID_CRG_DED_LC_CFG;

   /* Fetch the Active cell */
   if ((*cell = rgDBMGetCellCb(lcCfg->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Active Cell does not exist: Cell %d\n",
               lcCfg->cellId));
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((*ue = rgDBMGetUeCb(*cell, lcCfg->crnti)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Ue %d does not exist for dedicated logical\
               channel %d\n", lcCfg->crnti, lcCfg->lcId));
      RETVALUE(RFAILED);
   }

   /* Validate logical channel Id */
   if ((lcCfg->lcId < RG_DEDLC_MIN_LCID)
            ||(lcCfg->lcId > RG_DEDLC_MAX_LCID))
   {
      RGDBGERR((rgPBuf, "Invalid logical channel Id %d\n",
               lcCfg->lcId));
      RETVALUE(RFAILED);
   }

   /* Validate downlink info */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      if (rgDBMGetDlDedLcCb((*ue), lcCfg->lcId) != NULLP)
      {
         RGDBGERR((rgPBuf, "Dedicated DL LC %d already configured for UE %d\n",
                  lcCfg->lcId, lcCfg->crnti));
         RETVALUE(RFAILED);
      }
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcCfg->dir & CRG_DIR_RX)
   {
      if (lcCfg->ulInfo.lcgId > (RG_MAX_LCG_PER_UE - 1))
      {
         RGDBGERR((rgPBuf, "Invalid lcgId for uplink logical channel %d\n",
                  lcCfg->ulInfo.lcgId));
         RETVALUE(RFAILED);
      }
      if (rgDBMGetUlDedLcCb((*ue), lcCfg->lcId) != NULLP)
      {
         RGDBGERR((rgPBuf, "Dedicated UL LC %d already configured for UE %d\n",
                  lcCfg->lcId, lcCfg->crnti));
         RETVALUE(RFAILED);
      }
      dirVld = TRUE;
   }

   if (!dirVld)
   {
      RGDBGERR((rgPBuf, "Invalid Direction %d \n", lcCfg->dir));
      RETVALUE(RFAILED);
   }

   RGDBGINFO((rgPBuf, "Dedicated logical channel %d validated for ue %d\
            for cell %d\n", lcCfg->lcId, lcCfg->crnti, lcCfg->cellId));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgDedLcCfg */


/***********************************************************
 *
 *     Func : rgCFGVldtCrgCmnLcCfg
 *
 *
 *     Desc : Validates common logical channel configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgCFGVldtCrgCmnLcCfg
(
CrgLchCfg     *lcCfg,
RgCellCb      **cell,
RgErrInfo     *errInfo
)
#else
PRIVATE S16 rgCFGVldtCrgCmnLcCfg(lcCfg, cell, errInfo)
CrgLchCfg     *lcCfg;
RgCellCb      **cell;
RgErrInfo     *errInfo;
#endif
{
   U8         dirVld  = FALSE;

   TRC2(rgCFGVldtCrgCmnLcCfg);
   RGDBGPRM((rgPBuf, "VALIDATING COMMON LC CONFIG"));

   errInfo->errCause = RGERR_CFG_INVALID_CRG_CMN_LC_CFG;
   
   /* Ensure cell is not in the active list */
   if ((*cell = rgDBMGetCellCb(lcCfg->cellId)) != NULLP)
   {
      RGDBGERR((rgPBuf, "Active Cell %d exists for common channels\n",
            lcCfg->cellId));
      RETVALUE(RFAILED);
   }

   /* Fetch the inactive cell for common logical channels */
   if ((*cell = rgDBMGetInactvCellCb(lcCfg->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Inactive Cell %d does not exist for common channels\n",
            lcCfg->cellId));
      RETVALUE(RFAILED);
   }

   /* Validate downlink info */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
      /* rg001.201: Removed the CRG Transport channel types */
         if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            if (rgDBMGetBcchOnDlsch(*cell,lcCfg->lcId) != NULLP)
            {
               RGDBGERR((rgPBuf, "BCCH on DLSCH already configured for\
                        cell %d\n", (*cell)->cellId));
               RETVALUE(RFAILED);
            }
         }
         /* rg001.201: Removed the CRG Transport channel types */
         else if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_BCH)
         {
            if (rgDBMGetBcchOnBch(*cell) != NULLP)
            {
               RGDBGERR((rgPBuf, "BCCH on BCH already configured for cell %d\n",
                     (*cell)->cellId));
               RETVALUE(RFAILED);
            }
         }
         else
         {
            RGDBGERR((rgPBuf, "Invalid transport channel %d for cell %d\n",
                  lcCfg->dlInfo.dlTrchType, (*cell)->cellId));
            RETVALUE(RFAILED);
         }
      }
      /* rg001.101: Reordered 'if' check to remove dependency on order of 
       * configuration */
      else if (lcCfg->lcType == CM_LTE_LCH_PCCH)
      {
         if (rgDBMGetPcch(*cell) != NULLP)
         {
            RGDBGERR((rgPBuf, "PCCH already configured for cell %d\n",
                     (*cell)->cellId));
            RETVALUE(RFAILED);
         }
      }
      else if (RG_DLCCCH_ISCFGD(*cell))
      {
         RGDBGERR((rgPBuf, "DL CCCH already configured for cell %d\n",
               (*cell)->cellId));
         RETVALUE(RFAILED);
      }
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcCfg->dir & CRG_DIR_RX)
   {
      /* Uplink CCCH */
      if (lcCfg->lcType != CM_LTE_LCH_CCCH)
      {
         RGDBGERR((rgPBuf, "Invalid UL common lcType %d for cell %d\n",
                  lcCfg->lcType, (*cell)->cellId));
         RETVALUE(RFAILED);
      }
      if (RG_ULCCCH_ISCFGD(*cell))
      {
         RGDBGERR((rgPBuf, "UL CCCH already configured for cell %d\n",
               (*cell)->cellId));
         RETVALUE(RFAILED);
      }
      dirVld = TRUE;
   }

   /* Invalid direction */
   if (!dirVld)
   {
      RGDBGERR((rgPBuf, "Invalid Direction %d\n", lcCfg->dir));
      RETVALUE(RFAILED);
   }

   RGDBGINFO((rgPBuf, "Common logical channel %d validated for cell %d\n",
         lcCfg->lcId, lcCfg->cellId));
   RETVALUE(ROK);
}  /* rgCFGVldtCrgCmnLcCfg */

/***********************************************************
 *
 *     Func : rgCFGCrgDedLcCfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgCFGCrgDedLcCfg
(
RgCellCb      *cell,
RgUeCb        *ue,
CrgLchCfg     *lcCfg,
RgErrInfo     *errInfo
)
#else
PRIVATE S16 rgCFGCrgDedLcCfg(cell, ue, lcCfg, errInfo)
RgCellCb      *cell;
RgUeCb        *ue;
CrgLchCfg     *lcCfg;
RgErrInfo     *errInfo;
#endif
{
   TRC2(rgCFGCrgDedLcCfg);
   RGDBGPRM((rgPBuf, "APPLYING DEDICATED LC CONFIG"));

   errInfo->errCause = RGERR_CFG_CRG_DED_LC_CFG;

   /* Uplink/Bi-directional logical channel */
   if (lcCfg->dir & CRG_DIR_RX)
   {
#ifdef LTE_L2_MEAS
      rgDBMInsUlDedLcCb(ue, lcCfg->lcId, lcCfg->ulInfo.lcgId, lcCfg->qci);
      cell->qciArray[lcCfg->qci].qci = lcCfg->qci;
#else
      rgDBMInsUlDedLcCb(ue, lcCfg->lcId, lcCfg->ulInfo.lcgId);
#endif
   }

   /* Downlink/Bi-directional logical channel */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      rgDBMInsDlDedLcCb(ue, lcCfg->lcId);
   }
   RGDBGINFO((rgPBuf, "Dedicated LC config done\n"));
   RETVALUE(ROK);
}  /* rgCFGCrgDedLcCfg */


/***********************************************************
 *
 *     Func : rgCFGCrgCmnLcCfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgCFGCrgCmnLcCfg
(
RgCellCb      *cell,
CrgLchCfg     *lcCfg,
RgErrInfo     *errInfo
)
#else
PRIVATE S16 rgCFGCrgCmnLcCfg(cell, lcCfg, errInfo)
RgCellCb      *cell;
CrgLchCfg     *lcCfg;
RgErrInfo     *errInfo;
#endif
{
   TRC2(rgCFGCrgCmnLcCfg);
   RGDBGPRM((rgPBuf, "APPLYING COMMON LC CONFIG"));

   errInfo->errCause = RGERR_CFG_CRG_CMN_LC_CFG;

   /* Handle configuration for CCCH/BCCH/PCCH */
   if (lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      /* UL and DL CCCH configuration */
      if (lcCfg->dir & CRG_DIR_TX)
      {
         cell->dlCcchId = lcCfg->lcId;
         cell->cellActvState |= RG_DL_CCCH_CFG_DONE;
      }

      if (lcCfg->dir & CRG_DIR_RX)
      {
         cell->ulCcchId = lcCfg->lcId;
         cell->cellActvState |= RG_UL_CCCH_CFG_DONE;
      }
   }
   else
   {
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
         /* BCCH on BCH and DLSCH configuration */
         /* rg001.201: Removed the CRG Transport channel types */
         if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            rgDBMInsBcchOnDlsch(cell, lcCfg->lcId);
            
            if(cell->cellActvState & RG_BCCH_DLSCH_CFG1_DONE)
            {
               cell->cellActvState |= RG_BCCH_DLSCH_CFG2_DONE;
            }
            else
            {
               cell->cellActvState |= RG_BCCH_DLSCH_CFG1_DONE;
            }
         }
         else
         {
            rgDBMInsBcchOnBch(cell, lcCfg->lcId);
            cell->cellActvState |= RG_BCCH_BCH_CFG_DONE;
         }
      }
      else  /* PCCH configuration */
      {
         rgDBMInsPcch(cell, lcCfg->lcId);
         cell->cellActvState |= RG_PCCH_CFG_DONE;
      }
   }

   /* Add to active cell list if cell is active */
   if (cell->cellActvState == RG_CELL_ACTIVE)
   {
      /* Delete cell from inactive list */
      rgDBMDelInactvCellCb(cell);
      /* Insert cell in the active list */
      rgDBMInsCellCb(cell);
      RGDBGINFO((rgPBuf, "Cell %d added to active list after common LC %d\
               config\n", cell->cellId, lcCfg->lcId));
   }

   RGDBGINFO((rgPBuf, "Common LC config done: lcId %d cell %d\n", lcCfg->lcId,
         lcCfg->cellId));
   RETVALUE(ROK);
}  /* rgCFGCrgCmnLcCfg */
#ifdef LTE_L2_MEAS
/***********************************************************
 *
 *     Func : rgCFGFreeUeUlAlloc 
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeUeUlAlloc
(
RgCellCb      *cell
)
#else
PRIVATE Void rgCFGFreeUeUlAlloc(cell)
RgCellCb      *cell;
#endif
{
   U8    sfIdx;
   
   TRC2(rgCFGFreeUeUlAlloc);

   for(sfIdx = 0; sfIdx < RG_NUM_UL_SUB_FRAMES; sfIdx++)
   {
      if(cell->ulSf[sfIdx].ueUlAllocInfo != NULLP)
      {
         /*ccpu00117052 - MOD- Passing double pointer for proper
          *                    NULLP assignment */
         rgFreeSBuf((Data **)&(cell->ulSf[sfIdx].ueUlAllocInfo), 
               (cell->ulSf[sfIdx].numUe * sizeof(RgUeUlAlloc)));
      }
   }
}/* rgCFGFreeUeUlAlloc */
#endif
/***********************************************************
 *
 *     Func : rgCFGFreeCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgCFGFreeCellCb
(
RgCellCb      *cell
)
#else
PUBLIC Void rgCFGFreeCellCb(cell)
RgCellCb      *cell;
#endif
{
   TRC2(rgCFGFreeCellCb);

   /* Free lists of the cell */
   rgCFGFreeUeLst(cell);
#ifdef LTEMAC_SPS
   rgCFGFreeSpsUeLst(cell);
#endif /* LTEMAC_SPS */
   rgRAMFreeCell(cell);
   rgCFGFreeCmnLcLst(cell);
#ifdef LTE_L2_MEAS
   rgCFGFreeUeUlAlloc(cell);
#endif
   /* ccpu00117052 - MOD - Passing double pointer for proper NULLP 
                           assignment */
   /* De-allocate the Cell */
   rgFreeSBuf((Data **)&cell, sizeof(*cell));
 
   /* Update satistics */
   rgUpdtCellCnt(RG_CFG_DEL);

   RGDBGINFO((rgPBuf, "Cell freed\n"));

  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeCellCb */


/***********************************************************
 *
 *     Func : rgCFGFreeInactvCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees inactive cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgCFGFreeInactvCellCb
(
RgCellCb      *cell
)
#else
PUBLIC Void rgCFGFreeInactvCellCb(cell)
RgCellCb      *cell;
#endif
{
   TRC2(rgCFGFreeInactvCellCb);
   
   #if 1
   /* De-initialize the Ue list */
   rgDBMDeInitUeCbLst(cell);
   #endif
#ifdef LTEMAC_SPS
   rgDBMDeInitSpsUeCbLst(cell);
#endif

   rgCFGFreeCmnLcLst(cell);

   /*ccpu00117052 - MOD- Passing double pointer for proper
                        NULLP assignment */
   /* De-allocate the Cell */
   rgFreeSBuf((Data **)&cell, sizeof(*cell));

   /* Update satistics */
   rgUpdtCellCnt(RG_CFG_DEL);

   RGDBGINFO((rgPBuf, "Cell freed\n"));

  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeInactvCellCb */


/***********************************************************
 *
 *     Func : rgCFGFreeUeCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeUeCb
(
RgCellCb    *cell,
RgUeCb      *ue
)
#else
PRIVATE Void rgCFGFreeUeCb(cell, ue)
RgCellCb    *cell;
RgUeCb      *ue;
#endif
{

   TRC2(rgCFGFreeUeCb);

   rgDHMFreeUe(&ue->dl.hqEnt);

   /* ccpu00117052 - MOD - Passing double pointer for proper NULLP
                          assignment */
   /* De-allocate the Ue */
   rgFreeSBuf((Data **)&ue, sizeof(*ue));

   /* Update Statistics */
   rgUpdtUeCnt(RG_CFG_DEL);

   RGDBGINFO((rgPBuf, "Ue freed\n"));

  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeUeCb */

/***********************************************************
 *
 *     Func : rgCFGFreeCmnLcLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees common logical channels in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeCmnLcLst
(
RgCellCb      *cell
)
#else
PRIVATE Void rgCFGFreeCmnLcLst(cell)
RgCellCb      *cell;
#endif
{
   TRC2(rgCFGFreeCmnLcLst);

   rgDBMFreeCmnLcLst(cell);

   RGDBGINFO((rgPBuf, "Common LC list of the cell %d freed\n", cell->cellId));

  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeCmnLcLst */


/***********************************************************
 *
 *     Func : rgCFGFreeUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeUeLst
(
RgCellCb      *cell
)
#else
PRIVATE Void rgCFGFreeUeLst(cell)
RgCellCb      *cell;
#endif
{
   RgUeCb     *ue;

   TRC2(rgCFGFreeUeLst);

   /* Free Ues in the list */
   while ((ue = rgDBMGetNextUeCb(cell, NULLP)))
   {
      rgDBMDelUeCb(cell, ue);
      rgCFGFreeUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgDBMDeInitUeCbLst(cell);

   RGDBGINFO((rgPBuf, "Ue list of the cell %d freed\n", cell->cellId));

  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeUeLst */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgCFGFreeSpsUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees Sps UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeSpsUeLst
(
RgCellCb      *cell
)
#else
PRIVATE Void rgCFGFreeSpsUeLst(cell)
RgCellCb      *cell;
#endif
{
   RgUeCb     *ue;

   TRC2(rgCFGFreeSpsUeLst);

   /* Free Ues in the list */
   while ((ue = rgDBMGetNextSpsUeCb(cell, NULLP)))
   {
      rgDBMDelSpsUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgDBMDeInitSpsUeCbLst(cell);

   RGDBGINFO((rgPBuf, "Sps Ue list of the cell %d freed\n",
                                               cell->cellId));
}  /* rgCFGFreeSpsUeLst */

#endif /* LTEMAC_SPS */

/**
 * @brief Function for registering cell- scheduler instance mapping
 *
 * @details
 *
 *     Function : RgSchMacCellRegReq
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     cell configuration successfully.
 *     This shall create a mapping of the cell, scheduler instance that
 *     is serving the cell and the unique identifier of the cell on the 
 *     scheduler at MAC. This mapping shall be used for further 
 *     communication to the scheduler instance for this cell.
 *     
 *           
 *  @param[in] Pst*                pst,
 *  @param[in] CmLteCellId         cellId,
 *  @param[in] RaRespReqInfo       raRespReq
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgSchMacCellRegReq
(
Pst*                pst,
RgInfCellReg*       regReq 
)
#else
PUBLIC S16 RgSchMacCellRegReq(pst, regReq)
Pst*                pst;
RgInfCellReg*       regReq;
#endif
{
   RgCellCb *cell;

   /*rg003.201: Changed TRC2 to TRC3 */
   TRC3(RgSchMacCellRegReq)

   if(NULLP == regReq)
   {
      RETVALUE(RFAILED);
   }
      
   if((cell = rgDBMGetCellCb(regReq->cellId)) == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Initialize */
   cell->schInstMap.cellSapId = regReq->cellSapId;
   cell->schInstMap.schInst   = pst->srcInst;
   cell->maxDlHqProcPerUe = regReq->maxDlHqProcPerUe;

   RETVALUE(ROK);

} /* end of RgSchMacCellRegReq */
#endif
/**********************************************************************
 
         End of file:     gk_cfg.c@@/main/3 - Sat Jul 30 02:21:24 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       sd   1. Initial Release.
/main/2      ---       sd   1. LTE MAC 2.1 release
           rg001.201   sm          1. Removed Transport channel 
                                   types from CRG
           rg003.201   ns   1. Trace added for functions.
           rg004.201   ns   1. ccpu00105882: Releasing HARQ processes of old
                               UE when ue reconfig with new crnti 
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependncy on MIMO compile-time flag
                           2. Added support for SPS.
/main/3      ---         gvj 1. Updated for LTE MAC Rel 3.1
           rg001.301   nudupi  1.Changes for the CR ccpu00119113. 
           rg002.301  asehgal 1. ccpu00120621:validations for qci introduced.
*********************************************************************91*/
