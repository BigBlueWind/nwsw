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
 
     Desc:     C source code for DLFS functions
 
     File:     rg_sch_dlfs.c
 
     Sid:      gk_sch_dlfs.c@@/main/2 - Sat Jul 30 02:21:40 2011
 
     Prg:     sd
 
**********************************************************************/

/** @file rg_sch_dlfs.c
@brief This module handles the downlink frequency selective scheduler functionality
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rg_sch_dlfs.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"
#include "rg_sch_dlfs.x"        /* typedefs for DLFS Scheduler */

/* Private functions */

/* 
 * DLFS specific functions 
 */

/* DLFS Initialization functions */
PRIVATE S16 rgSCHDlfsInitSf ARGS((RgSchCellCb *, RgSchDlfsSf *));
PRIVATE Void rgSCHDlfsInitSfHoles ARGS((RgSchDlfsSf *));
PRIVATE S16 rgSCHDlfsInitCell ARGS((RgSchCellCb *, RgSchDlfsCellCb *));
PRIVATE S16 rgSCHDlfsCreateUe ARGS((RgSchCellCb *, RgSchUeCb *));
PRIVATE Void rgSCHDlfsInitUeBp ARGS((RgSchDlfsCellCb *, RgSchDlfsUeCb *));
PRIVATE Void rgSCHDlfsInitSfSbs ARGS((RgSchDlfsCellCb *, RgSchDlfsSf *));
PRIVATE S16 rgSCHDlfsInitPrdMode20 ARGS((Inst, RgSchDlfsCellCb *,
         RgSchDlfsUeCb *, RgrUePrdDlCqiCfg *));
PRIVATE S16 rgSCHDlfsUpdPrdModeCfg ARGS((RgSchCellCb *, RgSchUeCb *, 
         RgrUePrdDlCqiCfg *));
PRIVATE Void rgSCHDlfsUpdAprdModeCfg ARGS((RgSchCellCb *, RgSchUeCb *, 
         RgrUeAprdDlCqiCfg *));

/* DLFS Sub-band/hole handling functions */
PRIVATE S16 rgSCHDlfsGetAllocFrmHole ARGS((RgSchDlfsHole *, U8 *, U8 *));
PRIVATE Void rgSCHDlfsRlsAlloc ARGS((RgSchDlfsSf *, U32 ));
PRIVATE Void rgSCHDlfsRlsHole ARGS((RgSchDlfsSf *, RgSchDlfsHole *));
PRIVATE Void rgSCHDlfsAddHole ARGS((RgSchDlfsSf *, U8 pos));
PRIVATE Void rgSCHDlfsRstHoleStart ARGS((RgSchDlfsCellCb *, RgSchDlfsSf *));

/* DLFS CQI updation functions */
PRIVATE Void rgSCHDlfsUpdWbCqi ARGS((RgSchDlfsUeCb *, U8 ));
PRIVATE Void rgSCHDlfsUpdPrdMode20WbCqi ARGS((RgSchDlfsCellCb *, 
         RgSchDlfsUeCb *, TfuCqiPucchMode20 *));
PRIVATE Void rgSCHDlfsUpdPrdMode20SbCqi ARGS((RgSchDlfsCellCb *, 
         RgSchDlfsUeCb *, TfuCqiPucchMode20 *, CmLteTimingInfo ));
PRIVATE Void rgSCHDlfsUpdAprdMode20SbCqi ARGS(( RgSchDlfsCellCb *,
         RgSchDlfsUeCb *, TfuCqiPuschMode20 *));
PRIVATE Void rgSCHDlfsUpdAprdMode30SbCqi ARGS(( RgSchDlfsCellCb *,
         RgSchDlfsUeCb *, TfuCqiPuschMode30 *));
PRIVATE S16 rgSCHDlfsCalcBpIdx ARGS((CmLteTimingInfo , RgSchDlfsUeCb *
         ,U8 *bpIdx));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
PRIVATE Void rgSCHDlfsUpdAprdMode22SbCqi ARGS((RgSchDlfsCellCb *, RgSchDlfsUeCb *, 
          TfuCqiPuschMode22 *));
PRIVATE Void rgSCHDlfsUpdAprdMode31SbCqi ARGS((RgSchDlfsCellCb *, RgSchDlfsUeCb *, 
          TfuCqiPuschMode31 *));

/* DLFS Allocation functions */
#ifdef LTEMAC_SPS
PRIVATE Void rgSCHDlfsTxUeRbAlloc ARGS(( RgSchCellCb *, RgSchDlSf *,
         CmLListCp *, CmLListCp *, CmLListCp *));
#else
PRIVATE Void rgSCHDlfsTxUeRbAlloc ARGS((RgSchCellCb *,
        RgSchCmnUeRbAlloc  *));
#endif
PRIVATE Void rgSCHDlfsSbAllocForTxUe ARGS((RgSchDlSf *, RgSchCellCb *,
         RgSchUeCb *, RgSchDlRbAlloc *));
PRIVATE Void rgSCHDlfsWbAllocForTxUe ARGS((RgSchDlSf *, RgSchCellCb *,
         RgSchUeCb *, RgSchDlRbAlloc *));
#ifdef LTEMAC_SPS
PRIVATE Void rgSCHDlfsRetxUeRbAlloc ARGS(( RgSchCellCb *, RgSchDlSf *,
         CmLListCp *, CmLListCp *, CmLListCp *));
#else
PRIVATE Void rgSCHDlfsRetxUeRbAlloc ARGS((RgSchCellCb *,
         RgSchCmnUeRbAlloc *));
#endif
PRIVATE Void rgSCHDlfsSbAllocForRetxUe ARGS((RgSchDlSf *, RgSchCellCb *,
         RgSchUeCb *, RgSchDlRbAlloc *));
PRIVATE Void rgSCHDlfsWbAllocForRetxUe ARGS((RgSchDlSf *, RgSchCellCb *, 
      RgSchUeCb *, RgSchDlRbAlloc *));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
PRIVATE Void rgSCHDlfsTxRetxUeRbAlloc ARGS((RgSchCellCb *,
         RgSchCmnUeRbAlloc *));
PRIVATE Void rgSCHDlfsSbAllocForTxRetxUe ARGS((RgSchDlSf *, RgSchCellCb *,
         RgSchUeCb *, RgSchDlRbAlloc *));
PRIVATE Void rgSCHDlfsWbAllocForTxRetxUe ARGS((RgSchDlSf *, RgSchCellCb *, 
      RgSchUeCb *, RgSchDlRbAlloc *));
PRIVATE Void rgSCHDlfsTyp2AllocForTxUe ARGS((RgSchDlSf *, RgSchCellCb *,
         RgSchUeCb *, RgSchDlRbAlloc *));
PRIVATE Void rgSCHDlfsTyp2AllocForRetxUe ARGS((RgSchDlSf *, RgSchCellCb *,
         RgSchUeCb *, RgSchDlRbAlloc *));
PRIVATE S16 rgSCHDlfsMsg4Alloc ARGS ((RgSchCellCb *,
         RgSchCmnDlRbAllocInfo *, U8 ));
#ifdef RGR_V1
/*rg009.201 - CR timer changes*/
PRIVATE S16 rgSCHDlfsCcchSduAlloc ARGS ((RgSchCellCb *,
         RgSchCmnDlRbAllocInfo *, U8 ));
#endif
PRIVATE S16 rgSCHDlfsCmnRbAlloc ARGS((RgSchCellCb *, 
         RgSchDlRbAlloc *));
PRIVATE Void rgSCHDlfsAllocSb ARGS((RgSchDlfsSf *, RgSchDlfsHole *, U8 *,
         U8 *));

/* DLFS Utility functions */
PRIVATE Void rgSCHDlfsFreeSf ARGS((Inst , RgSchDlfsSf *));
PRIVATE Void rgSCHDlfsFillImcs ARGS(( RgSchCellCb *, RgSchDlfsUeCb *,
         RgSchDlRbAlloc *, U8 isRetx));
PRIVATE Void rgSCHDlfsFreeUeBp ARGS((Inst , RgSchDlfsUeCb *));
PRIVATE Void rgSCHDlfsSortUeLst ARGS((CmLListCp *, CmLListCp *));
PRIVATE Void rgSCHDlfsSortSbCqiLst ARGS((RgSchDlfsUeCb *));
PRIVATE Void rgSCHDlfsCalcSbCqiDeviation ARGS((RgSchDlfsUeCb *)); 

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
PRIVATE Void rgSCHDlfsUpdPrdMode21WbCqi ARGS((
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode21  *mode21CqiInfo
));
PRIVATE Void rgSCHDlfsUpdPrdMode21SbCqi ARGS((
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode21  *mode21CqiInfo,
CmLteTimingInfo    timingInfo
));

/**
 * @brief This function initializes all the data for the DLFS scheduler
 *
 * @details
 *
 *     Function: rgSCHDlfsInit
 *     Purpose: This function registers the DLFS scheduler with the APIs exposed
 *     by it.
 *     
 *     Invoked by: MAC intialization code.
 *     
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDlfsInit
(
RgDlfsSchdApis *rgDlfsApis
)
#else
PUBLIC Void rgSCHDlfsInit(rgDlfsApis)
RgDlfsSchdApis *rgDlfsApis;
#endif 
{
   /* rg003.201 : Trace added */
   TRC2(rgSCHDlfsInit);
   /* Init the function pointers */
   rgDlfsApis->rgSCHDlfsCellCfg    = rgSCHDlfsCellCfg;
   rgDlfsApis->rgSCHDlfsCellRecfg  = rgSCHDlfsCellRecfg;
   rgDlfsApis->rgSCHDlfsCellDel    = rgSCHDlfsCellDel;
   rgDlfsApis->rgSCHDlfsUeCfg      = rgSCHDlfsUeCfg; 
   rgDlfsApis->rgSCHDlfsUeRecfg    = rgSCHDlfsUeRecfg;
   rgDlfsApis->rgSCHDlfsUeDel      = rgSCHDlfsUeDel;
   rgDlfsApis->rgSCHDlfsDlCqiInd   = rgSCHDlfsDlCqiIndHndlr;
   rgDlfsApis->rgSCHDlfsReinitSf   = rgSCHDlfsReinitSf;
   rgDlfsApis->rgSCHDlfsAllocRb    = rgSCHDlfsRbAlloc;
   /* rg004.201-ADD-Added for BCCH/PCCH handling */
   rgDlfsApis->rgSCHDlfsBcchPcchAllocRb = rgSCHDlfsBcchPcchRbAlloc;

   RETVOID;
}

/**
 * @brief Initializes the DLFS scheduler for the cell.
 *
 * @details
 *     Function : rgSCHDlfsCellCfg
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this API at cell configuration if downlink frequency
 *      selective is enabled.
 *       
 *     Processing Steps:
 *     - No DLFS specific validations are needed.
 *     - Update configured values.
 *     - Initialize the cell. Call rgSCHDlfsInitCell()
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDlfsCellCfg 
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
)
#else
PUBLIC S16 rgSCHDlfsCellCfg (cell, cellCfg, err) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
RgSchErrInfo       *err;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchDlfsCellCb *dlfsCell;
   RgSchCmnCell    *cmnCell  = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHDlfsCellCfg);

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsCellCfg: thresholdCqi: %d, DL BW: %d"
            "\n", (int)cellCfg->dlfsCfg.thresholdCqi, 
            (int)cellCfg->bwCfg.dlTotalBw));

   ret = rgSCHUtlAllocSBuf(instIdx,
         (Data**)&((RgSchCmnCell*)((cell)->sc.sch))->dl.dlfsCell,
         (sizeof(RgSchDlfsCellCb)));
   if(ret != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Memory allocation FAILED for DLFS cell\n"));
      err->errCause = RGSCHERR_DLFS_MEMERR;
      RETVALUE(ret);
   }

   dlfsCell = RG_SCH_DLFS_GET_CELL(cell);

   /* Update DLFS specific configuration */
   dlfsCell->defaultCqi = cmnCell->dl.ccchCqi;
   dlfsCell->thresholdCqi = cellCfg->dlfsCfg.thresholdCqi;
   dlfsCell->totDlBw = cellCfg->bwCfg.dlTotalBw;

   /* Initialize the cell */
   if ((ret = rgSCHDlfsInitCell(cell, dlfsCell))!= ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Memory allocation FAILED for DLFS cell %d\n", cell->cellId));
      err->errCause = RGSCHERR_DLFS_MEMERR;
      RETVALUE(ret);
   }

   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsCellCfg Successful for cell: %d\n", cell->cellId));

   RETVALUE(ret);
}

/**
 * @brief Re-initializes the DLFS scheduler at cell-reconfiguration
 *
 * @details
 *
 *     Function : rgSCHDlfsCellRecfg
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this at cell re-configuration.
 *      
 *     Processing Steps:
 *      - Validate the re-configured values
 *      - If validation failed, return RFAILED
 *      - If validation succeeded,
 *       - if isDlFreqSel variable has been toggled
 *        - if (cell->isDlFreqSel is TRUE) && (isDlFreqSel is FALSE)
 *          - clear all the list per ue and per cell associated with DL FS
 *        - else if (cell->isDlFreqSel is FALSE) && (isDlFreqSel is TRUE)
 *          - Initialize all related variables and lists for cell and UE
 *          - Set cell->isDlFreqSel = isDlFreqSel
 *          - Initialize sub-bands for the cell. Call  rgSCHDlfsInitSf().
 *
 *   @param[in]  RgSchCellCb   *cell,
 *   @param[in]  RgrCellRecfg  *cellRecfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDlfsCellRecfg
(
RgSchCellCb        *cell,
RgrCellRecfg       *cellRecfg,
RgSchErrInfo       *err
)
#else
PUBLIC S16 rgSCHDlfsCellRecfg (cell, cellRecfg, err) 
RgSchCellCb        *cell;
RgrCellRecfg       *cellRecfg;
RgSchErrInfo       *err;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchUeCb       *prvUe    = NULLP;
   RgSchUeCb       *ue       = NULLP;
   RgSchCmnCell    *cmnCell  = RG_SCH_CMN_GET_CELL(cell);
   RgSchDlfsCellCb *dlfsCell = NULLP;
   TRC2(rgSCHDlfsCellRecfg);

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsCellRecfg: thresholdCqi: %d, "
            "isDlFreqSel: %d, isCellFreqSel: %d\n", 
            cellRecfg->dlfsRecfg.thresholdCqi, 
            cellRecfg->dlfsRecfg.isDlFreqSel, cmnCell->dl.isDlFreqSel));

   if (!cellRecfg->dlfsRecfg.isDlFreqSel)
   {
      if (cmnCell->dl.isDlFreqSel) 
      {
         /* Frequency selective has been disabled for the cell 
          * Clear DLFS related information */

         /* Clear DLFS specific information for the UE */
         while ((ue = rgSCHDbmGetNextUeCb(cell, prvUe)) != NULLP)
         {
            rgSCHDlfsUeDel(cell, ue);
            prvUe = ue;
         }

         /* Free DLFS cell */
         rgSCHDlfsCellDel(cell);
         RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
                  "Frequency selective disabled for cell: %d\n", cell->cellId));
      }
      RETVALUE(ROK);
   }

   /* Processing for frequency selective enabled */
   if (!(cmnCell->dl.isDlFreqSel)) 
   {
      /* Frequency selective has been enabled for the cell: create cell
       * specific data strutures */ 
      ret = rgSCHUtlAllocSBuf(instIdx,
            (Data**)&((RgSchCmnCell*)((cell)->sc.sch))->dl.dlfsCell,
            (sizeof(RgSchDlfsCellCb)));
      if(ret != ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "Memory allocation FAILED for DLFS cell\n"));
         err->errCause = RGSCHERR_DLFS_MEMERR;
         RETVALUE(ret);
      }

      dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
      dlfsCell->totDlBw = cell->bwCfg.dlTotalBw; 

      /* Initialize the cell */
      if ((ret = rgSCHDlfsInitCell(cell, dlfsCell))!= ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "Memory allocation FAILED for DLFS cell %d\n", cell->cellId));
         err->errCause = RGSCHERR_DLFS_MEMERR;
         RETVALUE(ret);
      }

      /* Create DLFS UE for the UEs in the cell */
      while ((ue = rgSCHDbmGetNextUeCb(cell, prvUe)) != NULLP)
      {
         ret = rgSCHDlfsCreateUe(cell, ue);
         if (ret != ROK)
         {
            RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                     "DLFS cell reconfig failed for cell%d\n", cell->cellId));
            err->errCause = RGSCHERR_DLFS_MEMERR;
            RETVALUE(RFAILED);
         }
         prvUe = ue;
      }

      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
               "Frequency selective enabled for cell: %d\n", cell->cellId));
   }

   /* Update DLFS specific configuration */
   if (!dlfsCell)
   {
      dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   }

   /* Update default and threshold CQI */
   dlfsCell->defaultCqi = cmnCell->dl.ccchCqi;
   dlfsCell->thresholdCqi = cellRecfg->dlfsRecfg.thresholdCqi;
   
   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "Reconfig for frequency selective cell done: %d\n", cell->cellId));
   RETVALUE (ROK);
}


/**
 * @brief Delete the DLFS scheduler context for the cell at cell delete
 *
 * @details
 *
 *     Function : rgSCHDlfsCellDel
 *     
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *      the cell at cell delete.
 *
 *     Processing Steps:
 *      - Free DLFS related information per DL subframe in the cell. 
 *        Call rgSCHDlfsFreeSf().
 *      - Free cell specific informationfor DLFS, if any.
 *      - Note: UE specific information is not freed over here since it is
 *      assumed that UE delete shall be invoked for all UEs prior to this call.
 *      - Return Void
 *
 *  
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDlfsCellDel 
(
RgSchCellCb        *cell
)
#else
PUBLIC Void rgSCHDlfsCellDel (cell) 
RgSchCellCb        *cell;
#endif
{
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   U8              dlsfIdx   = 0;
   RgSchDlfsSf     *dlfsSf    = NULLP;  
   Inst            instIdx   = cell->instIdx;
   TRC2(rgSCHDlfsCellDel);

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsCellDel: cellId: %d\n", cell->cellId));
   
   /* Free DLFS subframe related information */
   /* rg009.201 [ccpu00115187]: Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
   for (dlsfIdx = 0; dlsfIdx < RGSCH_NUM_DL_SUBFRAMES; ++dlsfIdx)
   {
      dlfsSf = RG_SCH_DLFS_GET_SF(cell->subFrms[dlsfIdx]);
      rgSCHDlfsFreeSf(instIdx, dlfsSf);
      (cell->subFrms[dlsfIdx])->dlfsSf = NULLP;
   }

   if (dlfsCell)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(instIdx, (Data**)(&(((RgSchCmnCell*)\
                     ((cell)->sc.sch))->dl.dlfsCell)),
            sizeof(*dlfsCell));
   }

   RETVOID;
}

/**
 * @brief Initializes UE with the DLFS scheduler at UE Configuration.
 *
 * @details
 *
 *     Function : rgSCHDlfsUeCfg
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell at UE configuration.
 *      - SCH shall validate the configured values
 *         - Validate the transmission mode
 *         - Validate the reporting mode for pucch and pusch (if configured)
 *         - Validate reporting mode against transmission mode.
 *
 *     Processing Steps:
 *      - Set ue->wbCqi = cell->defaultCqi; 
 *      - Initalize DL FS related variables per UE
 *      - Return ROK
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeCfg      *ueCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDlfsUeCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
)
#else
PUBLIC S16 rgSCHDlfsUeCfg (cell, ue, ueCfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
RgSchErrInfo       *err;
#endif
{
   S16             ret       = ROK;
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif
   TRC2(rgSCHDlfsUeCfg);
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsUeCfg: cellId: %d, crnti %d periodic mode pres: %d "
            "aperiodic mode pres: %d\n", cell->cellId, ueCfg->crnti,
            ueCfg->ueDlCqiCfg.prdCqiCfg.pres, 
            ueCfg->ueDlCqiCfg.aprdCqiCfg.pres));
#else
    RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsUeCfg: cellId: %d, crnti %d periodic mode pres: %d "
            "aperiodic mode pres: %d\n", cell->cellId, ueCfg->crnti,
            ueCfg->ueDlCqiCfg.prdCqiCfg.type, 
            ueCfg->ueDlCqiCfg.aprdCqiCfg.pres));
#endif

   ret = rgSCHDlfsCreateUe(cell, ue);
   if (ret != ROK)
   {
      err->errCause = RGSCHERR_DLFS_MEMERR;
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 * @brief Re-initializes UE with the DLFS scheduler at UE re-configuration.
 *
 * @details
 *
 *     Function : rgSCHDlfsUeRecfg
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the UE at UE recfg.
 *      - SCH shall validate the re-configured values
 *         - Validate the transmission mode
 *         - Validate the reporting mode for pucch and pusch (if configured)
 *         - Validate reporting mode against transmission mode.
 *         
 *     Processing Steps:
 *      - If CQI reporting is disabled,
 *        - The old CQIs are retained.
 *      - Return ROK
 *        
 *
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in]     RgSchUeCb      *ue
 *  @param[in]     RgrUeRecfg     *ueRecfg
 *  @param[out]    RgrSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDlfsUeRecfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
)
#else
PUBLIC S16 rgSCHDlfsUeRecfg (cell, ue, ueRecfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
RgSchErrInfo       *err;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchDlfsUeCb   *dlfsUe   = RG_SCH_DLFS_GET_UE(ue);
   TRC2(rgSCHDlfsUeRecfg);

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsUeRecfg: cellId: %d, ueId %d\n", cell->cellId,
            ue->ueId));

   /* Depending on the reporting mode, free/create BP information */

   if (ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG)
   {
/* rg009.201. Added changes of TFU_UPGRADE */       
#ifndef TFU_UPGRADE
      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
               "Reconfiguring periodic reporting mode, pres: %d\n",
               ueRecfg->prdDlCqiRecfg.pres));
      if (ueRecfg->prdDlCqiRecfg.pres)
      {
         ret = rgSCHDlfsUpdPrdModeCfg(cell, ue, &ueRecfg->prdDlCqiRecfg);
#else
      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
               "Reconfiguring periodic reporting mode, pres: %d\n",
               ueRecfg->cqiCfg.type));
      if (ueRecfg->cqiCfg.type)
      {

         ret = rgSCHDlfsUpdPrdModeCfg(cell, ue, &ueRecfg->cqiCfg);

#endif /* TFU_UPGRADE */
         if (ret != ROK)
         {
            err->errCause = RGSCHERR_DLFS_MEMERR;
            RETVALUE(RFAILED);
         }
      }
      else
      {
         /* Free bandwidth parts if allocated for the UE */
         if (dlfsUe->bpCbs)
         {
            rgSCHDlfsFreeUeBp(instIdx, dlfsUe);
         }
         /* Re-setting prdSbSize */
         dlfsUe->prdSbSize = 0;
      }
   }

   /* Update aperiodic sub-band size */
   if (ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG)
   {
      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
               "Reconfiguring aperiodic reporting mode, pres: %d\n",
               ueRecfg->aprdDlCqiRecfg.pres));
      if (ueRecfg->aprdDlCqiRecfg.pres)
      {
         rgSCHDlfsUpdAprdModeCfg(cell, ue, &ueRecfg->aprdDlCqiRecfg);
      }
      else
      {
         /* Re-setting aprdSbSize */
         dlfsUe->aprdSbSize = 0;
      }
   }

   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsUeRecfg: numSbCqi: %d, numBps: %d, prdSbSize: %d,"
            "aprdSbSize %d\n", dlfsUe->numSbCqi, dlfsUe->numBp,
            dlfsUe->prdSbSize, dlfsUe->aprdSbSize));
   RETVALUE(ROK);
}

/**
 * @brief Clears UE context with the DLFS scheduler at UE delete.
 *
 * @details
 *
 *     Function : rgSCHDlfsUeDel
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell at UE delete.
 *
 *     Processing Steps:
 *     - Free Ue specific information 
 *     - Return ROK
 *
 * @param[in]  RgSchCellCb *cell
 * @param[in]  RgSchUeCb   *ue
 * @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDlfsUeDel 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHDlfsUeDel (cell, ue) 
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchDlfsUeCb *dlfsUe = RG_SCH_DLFS_GET_UE(ue);
   Inst          instIdx = cell->instIdx;
   TRC2(rgSCHDlfsUeDel);

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsUeDel: cellId: %d, ueId: %d\n", cell->cellId, ue->ueId));

   /* Clear ue specific DLFS information */
   if (dlfsUe)
   {
      /* Free the list of sub-band CQIs allocated for the UE */
      if (dlfsUe->sbCqiLst)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(instIdx, (Data**)(&(dlfsUe->sbCqiLst)),
               (dlfsUe->numSbCqi * sizeof(RgSchDlfsSbCqi)));
      }

      /* Free bandwidth part information */
      if (dlfsUe->bpCbs)
      {
         rgSCHDlfsFreeUeBp(instIdx, dlfsUe);
      }

         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(instIdx,
            (Data **)(&(((RgSchCmnUe *)((ue)->sch))->dl.dlfsUe)),
            sizeof(RgSchDlfsUeCb));
   }

   RETVOID;
}

/**
 * @brief Downlink CQI Indication handler 
 *
 * @details
 *
 *     Function : rgSCHDlfsDlCqiIndHndlr
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell at downlink CQI indication from PHY for the UE.
 *        
 *     Processing Steps:
 *     - It is assumed that the reporting of the CQI shall be strictly on the
 *     basis of the configured mode of the UE.
 *     - Per reporting mode, 
 *        - Update wideband CQI.
 *        - Update sub-band CQI, if reported.
 *     - If sub-band CQI has been reported, sort the sub-band CQI list in 
 *       desceding order of CQIs. Call rgSCHDlfsSortSbCqiLst().
 *     - Compute the deviation of sub-band CQI from average CQI. Call
 *       rgSCHDlfsCalcSbCqiDeviation().
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  TfuDlCqiRpt     *cqiIndInfo
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHDlfsDlCqiIndHndlr
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuDlCqiRpt          *cqiIndInfo,
CmLteTimingInfo      timingInfo
)
#else
PUBLIC Void rgSCHDlfsDlCqiIndHndlr(cell, ue, cqiIndInfo, timingInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuDlCqiRpt          *cqiIndInfo;
CmLteTimingInfo      timingInfo;
#endif
{
   RgSchDlfsCellCb   *dlfsCell     = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb     *dlfsUe       = RG_SCH_DLFS_GET_UE(ue);
   TfuDlCqiPucch     *pucchCqiInfo = NULLP;
   TfuDlCqiPusch     *puschCqiInfo = NULLP; 
#ifdef DEBUGP
   Inst              instIdx       = cell->instIdx;
#endif
   TRC2(rgSCHDlfsDlCqiIndHndlr);

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsDlCqiIndHndlr: cellId: %d, ueId: %d\n",
            cell->cellId, ue->ueId));

   if (cqiIndInfo->isPucchInfo)
   {
      /* Periodic CQI report recieved */
      pucchCqiInfo = &(cqiIndInfo->dlCqiInfo.pucchCqi);
      switch (pucchCqiInfo->mode)
      {
         case TFU_PUCCH_CQI_MODE10:
            {
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
               if (pucchCqiInfo->u.mode10Info.type != TFU_RPT_CQI)
               {
                  RETVOID;
               }
               /* Only wideband CQI is recieved */
               RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                        "Updating periodic mode 1-0 wideband CQI\n"));
               rgSCHDlfsUpdWbCqi(dlfsUe, pucchCqiInfo->u.mode10Info.u.cqi);
               break;
            }
         case TFU_PUCCH_CQI_MODE20:
            {
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
               if (pucchCqiInfo->u.mode20Info.type != TFU_RPT_CQI)
               {
                  RETVOID;
               }
               if (pucchCqiInfo->u.mode20Info.u.cqi.isWideband)
               {
                  /* Wideband CQI is recieved */
                  RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                           "Updating periodic mode 2-0 wideband CQI\n"));
                  rgSCHDlfsUpdPrdMode20WbCqi(dlfsCell, dlfsUe, 
                        &pucchCqiInfo->u.mode20Info);
               }
               else
               {
                  /* Sub-band CQI received for preferred sub-band in a BP */
                  RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                           "Updating periodic mode 2-0 subband CQI\n"));
                  rgSCHDlfsUpdPrdMode20SbCqi(dlfsCell, dlfsUe, 
                        &pucchCqiInfo->u.mode20Info, timingInfo);
               }
               break;
            }
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
         case TFU_PUCCH_CQI_MODE11:
            {
               U8 avgCqi, cw1Cqi, cw2Cqi;
               if (pucchCqiInfo->u.mode11Info.type != TFU_RPT_CQI)
               {
                  RETVOID;
               }
               /* Only wideband CQI is recieved */
               RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                        "Updating periodic mode 1-1 wideband CQI\n"));
               /* Considering Wideband CQI as an average of both
                * CW1 and CW2 CQIs */
               avgCqi = cw1Cqi = pucchCqiInfo->u.mode11Info.u.cqi.cqi;
               if (pucchCqiInfo->u.mode11Info.u.cqi.wideDiffCqi.pres)
               {
                  cw2Cqi = cw1Cqi - rgSchCmnDlCqiDiffOfst[\
                   pucchCqiInfo->u.mode11Info.u.cqi.wideDiffCqi.val];
                  avgCqi = (cw1Cqi+cw2Cqi)/2;
               }
               rgSCHDlfsUpdWbCqi(dlfsUe, avgCqi);
               break;
            }
         case TFU_PUCCH_CQI_MODE21:
            {
               if (pucchCqiInfo->u.mode21Info.type != TFU_RPT_CQI)
               {
                  RETVOID;
               }
               if (pucchCqiInfo->u.mode21Info.u.cqi.isWideband)
               {
                  /* Wideband CQI is recieved */
                  RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                           "Updating periodic mode 2-1 wideband CQI\n"));
                  rgSCHDlfsUpdPrdMode21WbCqi(dlfsCell, dlfsUe, 
                        &pucchCqiInfo->u.mode21Info);
               }
               else
               {
                  /* Sub-band CQI received for preferred sub-band in a BP */
                  RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                           "Updating periodic mode 2-1 subband CQI\n"));
                  rgSCHDlfsUpdPrdMode21SbCqi(dlfsCell, dlfsUe, 
                        &pucchCqiInfo->u.mode21Info, timingInfo);
               }
               break;
            }
         default:
            {
               RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                        "Invalid periodic CQI reporting mode config %d"
                        ,(int)pucchCqiInfo->mode));
               break;
            }
      }
   }
   else
   {
      /* Aperiodic CQI report recieved */
      puschCqiInfo = &(cqiIndInfo->dlCqiInfo.puschCqi); 
      switch (puschCqiInfo->mode)
      {
         case TFU_PUSCH_CQI_MODE_20:
            {
               RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                        "Updating aperiodic mode 2-0 wideband and "
                        "subband CQI\n"));
               /* Update wideband CQI */
               rgSCHDlfsUpdWbCqi(dlfsUe,
                     puschCqiInfo->u.mode20Info.wideBandCqi);

               /* Update Sub-band CQI recieved for preferred sub-bands */
               rgSCHDlfsUpdAprdMode20SbCqi(dlfsCell, dlfsUe,
                     &(puschCqiInfo->u.mode20Info));
               break;
            }
         case TFU_PUSCH_CQI_MODE_30:
            {
               RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                        "Updating aperiodic mode 3-0 wideband and "
                        "subband CQI\n"));
               /* Update wideband CQI */
               rgSCHDlfsUpdWbCqi(dlfsUe, 
                     puschCqiInfo->u.mode30Info.wideBandCqi);

               /* Update Sub-band CQI recieved for all sub-bands */
               rgSCHDlfsUpdAprdMode30SbCqi(dlfsCell, dlfsUe,
                     &puschCqiInfo->u.mode30Info);
               break;
            }
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
         case TFU_PUSCH_CQI_MODE_12:
            {
               RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                        "Updating aperiodic mode 1-2 wideband and "
                        "subband CQI\n"));
               /* Update wideband CQI as an average of 
                * CW1 and CW2's CQIs */
               rgSCHDlfsUpdWbCqi(dlfsUe, 
                     (U8)((puschCqiInfo->u.mode12Info.cqiIdx[0] +\
                      puschCqiInfo->u.mode12Info.cqiIdx[1])/2));
               break;
            }
         case TFU_PUSCH_CQI_MODE_22:
            {
               RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                        "Updating aperiodic mode 1-2 wideband and "
                        "subband CQI\n"));
               /* Update wideband CQI as an average of 
                * CW1 and CW2's CQIs */
               rgSCHDlfsUpdWbCqi(dlfsUe, 
                     (U8) ((puschCqiInfo->u.mode22Info.cqi[0] +\
                      puschCqiInfo->u.mode22Info.cqi[1])/2));
               /* Update Sub-band CQI recieved for all sub-bands */
               rgSCHDlfsUpdAprdMode22SbCqi(dlfsCell, dlfsUe,
                     &puschCqiInfo->u.mode22Info);
               break;
            }
         case TFU_PUSCH_CQI_MODE_31:
            {
               RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                        "Updating aperiodic mode 1-2 wideband and "
                        "subband CQI\n"));
               /* Update wideband CQI as an average of 
                * CW1 and CW2's CQIs */
               rgSCHDlfsUpdWbCqi(dlfsUe, 
                    (U8) ((puschCqiInfo->u.mode31Info.wideBandCqi[0] +\
                      puschCqiInfo->u.mode31Info.wideBandCqi[1])/2));
               /* Update Sub-band CQI recieved for all sub-bands */
               rgSCHDlfsUpdAprdMode31SbCqi(dlfsCell, dlfsUe,
                     &puschCqiInfo->u.mode31Info);
               break;
            }            
         default:
            {
               RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
                        "Invalid aperiodic CQI reporting mode config %d"
                        ,(int)puschCqiInfo->mode));
               break;
            }
      }
   }

   RETVOID;
}


/**
 * @brief Handles DLFS based RB allocation
 *
 * @details
 *
 *     Function : rgSCHDlfsRbAlloc
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell for RB allocation.
 *      - MAX C/I/PFS/RR shall provide the requiredBytes, required RBs
 *        estimate and subframe for each allocation to be made to SCH.
 *        
 *     Processing Steps:
 *     - Allocate for common channel CCCH,
 *       RaRsp. Call  rgSCHDlfsCmnRbAlloc(). Call rgSCHDlfsMsg4Alloc().
 *     - For transmiting and re-transmiting UE list,
 *       - Call rgSCHDlfsTxUeRbAlloc().
 *       - Call rgSCHDlfsRetxUeRbAlloc().
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHDlfsRbAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHDlfsRbAlloc(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   S16                ret            = ROK;
   U8                 raRspCnt       = 0;
   RgSchDlfsSf        *dlfsSf        = NULLP;
   RgSchDlfsCellCb    *dlfsCell      = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlRbAlloc     *reqAllocInfo;
#ifdef DEBUGP
   Inst               instIdx        = cell->instIdx;
#endif
   TRC2(rgSCHDlfsRbAlloc);

   /* Allocate for MSG4 retransmissions */
   /* Assuming all the nodes in the list need allocations: rbsReq is valid */
   if (allocInfo->msg4Alloc.msg4RetxLst.count)
   {
      ret = rgSCHDlfsMsg4Alloc(cell, allocInfo, RG_SCH_DLFS_RETX_TRUE);
   }

   /* If bandwidth remains, Allocate for MSG4 transmissions */
   /* Assuming all the nodes in the list need allocations: rbsReq is valid */
   if ((ret == ROK) && (allocInfo->msg4Alloc.msg4TxLst.count))
   {
      rgSCHDlfsMsg4Alloc(cell, allocInfo, RG_SCH_DLFS_RETX_FALSE);
   }

   /* Allocate for CCCH SDU (received after guard timer expiry) retransmissions */
#ifdef RGR_V1
   /*rg009.201 - CR timer implementation changes*/
   if (allocInfo->ccchSduAlloc.ccchSduRetxLst.count)
   {
      ret = rgSCHDlfsCcchSduAlloc(cell, allocInfo, RG_SCH_DLFS_RETX_TRUE);
   }
   
   /* If bandwidth remains, Allocate for CCCH SDU transmissions */
   /* Allocate for CCCH SDU (received after guard timer expiry) transmissions */
   if ((ret == ROK) && (allocInfo->ccchSduAlloc.ccchSduTxLst.count))
   {
      rgSCHDlfsCcchSduAlloc(cell, allocInfo, RG_SCH_DLFS_RETX_FALSE);
   }
#endif
   /* Allocate for Random access response */
   for (raRspCnt = 0; raRspCnt < RG_SCH_CMN_MAX_CMN_PDCCH; ++raRspCnt)
   {
      /* Assuming that the requests will be filled in sequentially */
      reqAllocInfo = &(allocInfo->raRspAlloc[raRspCnt]);
      if (!reqAllocInfo->rbsReq)
      {
         break;
      }
      if ((ret = rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo)) != ROK)
      {
         RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                  "RAR Allocation failed for RAR idx: %d, rbsReq: %d,"
                  " rbsAlloc: %d\n", raRspCnt,
                  reqAllocInfo->rbsReq, (int)reqAllocInfo->rbsAlloc));
         break;
      }
   }

   /* Re-set the hole list's start if common allocations are done
    * for the dedicated sub-frame */
   dlfsSf = RG_SCH_DLFS_GET_SF(allocInfo->dedAlloc.dedDlSf);
#ifdef LTEMAC_SPS
   if (dlfsSf->allocDlBw > cell->dlSpsCfg.maxSpsDlBw)
   {
      rgSCHDlfsRstHoleStart(dlfsCell, dlfsSf); 
   }
#else
   if(dlfsSf->allocDlBw)
   {
      rgSCHDlfsRstHoleStart(dlfsCell, dlfsSf); 
   }
#endif
#ifdef LTEMAC_SPS
   /* Allocate for re-transmitting SPS UEs */
   if (allocInfo->dedAlloc.retxSpsUeLst.count)
   {
      rgSCHDlfsRetxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.retxSpsUeLst),
            &(allocInfo->dedAlloc.schdRetxSpsUeLst),
            &(allocInfo->dedAlloc.nonSchdRetxSpsUeLst));
   }

   /* Allocate for transmitting SPS UEs */
   if (allocInfo->dedAlloc.txSpsUeLst.count)
   {
      rgSCHDlfsTxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.txSpsUeLst),
            &(allocInfo->dedAlloc.schdTxSpsUeLst), 
            &(allocInfo->dedAlloc.nonSchdTxSpsUeLst));
   }
#endif


   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   if (allocInfo->dedAlloc.txRetxUeLst.count)
   {
      rgSCHDlfsTxRetxUeRbAlloc(cell, &(allocInfo->dedAlloc));
   }
   /* Allocate for re-transmitting UEs */
   if (allocInfo->dedAlloc.retxUeLst.count)
   {
#ifdef LTEMAC_SPS
      rgSCHDlfsRetxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.retxUeLst),
            &(allocInfo->dedAlloc.schdRetxUeLst), 
            &(allocInfo->dedAlloc.nonSchdRetxUeLst));
#else
      rgSCHDlfsRetxUeRbAlloc(cell, &(allocInfo->dedAlloc));
#endif
   }

   /* Allocate for transmitting UEs */
   if (allocInfo->dedAlloc.txUeLst.count)
   {
#ifdef LTEMAC_SPS
      rgSCHDlfsTxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.txUeLst),
            &(allocInfo->dedAlloc.schdTxUeLst), 
            &(allocInfo->dedAlloc.nonSchdTxUeLst));
#else
      rgSCHDlfsTxUeRbAlloc(cell, &(allocInfo->dedAlloc));
#endif
   }
   RETVOID;
}

/**
 * @brief Updates configuration for periodic mode
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdModeCfg
 *
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @param[in]  RgrUePrdDlCqiCfg  *prdCqiCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsUpdPrdModeCfg
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgrUePrdDlCqiCfg       *prdCqiCfg
)
#else
PRIVATE S16 rgSCHDlfsUpdPrdModeCfg(cell, ue, prdCqiCfg)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgrUePrdDlCqiCfg       *prdCqiCfg;
#endif
{
   S16                 ret         = ROK;
   U32                 mode;
   Inst                instIdx     = cell->instIdx;
   RgSchDlfsCellCb     *dlfsCell   = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb       *dlfsUe     = RG_SCH_DLFS_GET_UE(ue);
   TRC2(rgSCHDlfsUpdPrdModeCfg);

/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
    mode = prdCqiCfg->prdModeEnum;
#else
    mode = prdCqiCfg->cqiSetup.prdModeEnum;
#endif

   switch (mode)
   {
      case RGR_PRD_CQI_MOD20:
         {
            RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                     "Configuring to periodic mode 2-0\n"));

            /* Initialize periodic Mode 20 information */
            ret = rgSCHDlfsInitPrdMode20(instIdx, dlfsCell, dlfsUe, prdCqiCfg);
            if (ret != ROK)
            {
               RETVALUE(ret);
            }
            break;
         }
      default: /* RGR_PRD_CQI_MOD10 */
         {
            RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                     "Configuring to periodic mode 1-0\n"));
            /* If allocated, de-allocate the Bandwidth parts */
            if (dlfsUe->bpCbs)
            {
               rgSCHDlfsFreeUeBp(instIdx, dlfsUe);
            }
            /* Re-setting prdSbSize */
            dlfsUe->prdSbSize = 0;
            break;
         }
   }
   RETVALUE(ROK);
}

/**
 * @brief Updates configuration for aperiodic mode 
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdModeCfg
 *
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgrUeAprdDlCqiCfg  *aprdCqiCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdModeCfg
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgrUeAprdDlCqiCfg      *aprdCqiCfg
)
#else
PRIVATE Void rgSCHDlfsUpdAprdModeCfg(cell, ue, aprdCqiCfg)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgrUeAprdDlCqiCfg      *aprdCqiCfg;
#endif
{
   RgSchDlfsCellCb     *dlfsCell   = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb       *dlfsUe     = RG_SCH_DLFS_GET_UE(ue);
#ifdef DEBUGP
   Inst                instIdx     = cell->instIdx;
#endif
   TRC2(rgSCHDlfsUpdAprdModeCfg);

   switch (aprdCqiCfg->aprdModeEnum)
   {
      case RGR_APRD_CQI_MOD20:
         {
            RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                     "Reconfiguring to aperiodic mode 2-0\n"));
            RG_SCH_DLFS_GET_APRD_MODE20_SBSIZE(&(dlfsUe->aprdSbSize),
                  dlfsCell->totDlBw);
            break;
         }
      default: /* RGR_APRD_CQI_MOD30 */
         {
            RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
                     "Reconfiguring to aperiodic mode 3-0\n"));
            RG_SCH_DLFS_GET_APRD_MODE30_SBSIZE(&(dlfsUe->aprdSbSize),
                  dlfsCell->totDlBw);
            break;
         }
   }
   RETVOID;
}

/**
 * @brief Initializes the cell for DLFS
 *
 * @details
 *
 *     Function : rgSCHDlfsInitCell
 *
 *     Processing Steps:
 *     - Compute rbgSize and number of bandwidth parts based on 
 *       DL bandwidth configuration.
 *     - For each downlink subframe, 
 *       - Initialize sub-bands and holes. Call  rgSCHDlfsInitSf().
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsInitCell
(
RgSchCellCb         *cell,
RgSchDlfsCellCb     *dlfsCell
)
#else
PRIVATE S16 rgSCHDlfsInitCell (cell, dlfsCell)
RgSchCellCb         *cell;
RgSchDlfsCellCb     *dlfsCell;
#endif
{
   S16              ret      = ROK;
   Inst             instIdx  = cell->instIdx;
   RgSchCmnCell     *cmnCell = RG_SCH_CMN_GET_CELL(cell);
   U8               dlsfIdx  = 0;
   RgSchDlfsSf      *dlfsSf  = NULLP;
   TRC2(rgSCHDlfsInitCell);

   if (dlfsCell->totDlBw > 63)
   {
      dlfsCell->rbgSize = 4;
      dlfsCell->numBp = 4;
   }
   else if (dlfsCell->totDlBw > 26)
   {
      dlfsCell->rbgSize = 3;
      dlfsCell->numBp = 3;
   }
   else if (dlfsCell->totDlBw > 10)
   {
      dlfsCell->rbgSize = 2;
      dlfsCell->numBp = 2;
   }
   else
   {
      dlfsCell->rbgSize = 1;
      if (dlfsCell->totDlBw > 7)
      {
         dlfsCell->numBp = 1;
      }
      else
      {
         dlfsCell->numBp = 0;
      }
   }

   dlfsCell->numDlSb =  (dlfsCell->totDlBw + dlfsCell->rbgSize - 1)/ 
      (dlfsCell->rbgSize); 
   dlfsCell->maxDlSbPerUe = (cmnCell->dl.maxDlBwPerUe/dlfsCell->rbgSize);


   /* rg009.201 [ccpu00115187]- Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
   /*rg002.301 - Add - [cppu00120694] DLSF Index calculation support for TDD */
#ifndef LTE_TDD
   for (dlsfIdx = 0; dlsfIdx < RGSCH_NUM_DL_SUBFRAMES; ++dlsfIdx)
#else
   for (dlsfIdx = 0; dlsfIdx < cell->numDlSubfrms; ++dlsfIdx)
#endif
   {
      if((ret = rgSCHUtlAllocSBuf(instIdx, 
                  (Data**)&((cell->subFrms[dlsfIdx])->dlfsSf), 
                  (sizeof(RgSchDlfsSf)))) != ROK)
      {
         RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
                  "Memory allocation FAILED for DLFS sub-frame\n"));
         /* Note: Failure shall be handled by invoking cellDel on DLFS by the
          * caller of DLFS */ 
         RETVALUE(ret);
      }

      dlfsSf = RG_SCH_DLFS_GET_SF(cell->subFrms[dlsfIdx]);
      if ((ret = rgSCHDlfsInitSf(cell,dlfsSf)) != ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "Memory allocation FAILED for sub-bands %d\n", dlsfIdx));
         /* Note: Failure shall be handled by invoking cellDel on DLFS by the
          * caller of DLFS */ 
         RETVALUE(ret);
      }
   }

   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsInitCell Successful for cell: %d, rbgSize: %d,"
            "numBp: %d, numDlSb: %d, maxDlSbPerUe: %d\n", 
            cell->cellId, dlfsCell->rbgSize, dlfsCell->numBp,
            dlfsCell->numDlSb, dlfsCell->maxDlSbPerUe));

   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHDlfsCreateUe
 *
 *     Desc : Creates DLFS UE for a UE 
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsCreateUe
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PRIVATE S16 rgSCHDlfsCreateUe(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   U8              numSbCqi  = dlfsCell->numDlSb;
   RgSchCmnDlUe    *cmnDlUe  = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchDlfsUeCb   *dlfsUe;
   TRC2(rgSCHDlfsCreateUe);

   /* Allocate DFLS UE */
   ret = rgSCHUtlAllocSBuf(instIdx, 
         (Data**)&((RgSchCmnUe *)((ue)->sch))->dl.dlfsUe,
         (sizeof(RgSchDlfsUeCb)));
   if(ret != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Memory allocation FAILED for DLFS Ue\n"));
      RETVALUE(ret);
   }

   dlfsUe   = RG_SCH_DLFS_GET_UE(ue);

   /* Allocate the sub-bands for the UE */
   if((ret = rgSCHUtlAllocSBuf(cell->instIdx, 
               (Data**)&(dlfsUe->sbCqiLst), 
               numSbCqi * (sizeof(RgSchDlfsSbCqi)))) != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx), 
               "Allocation for sub-band cqi list failed for the UE %d",
               ue->ueId));
      RETVALUE(ret);
   }

   /* Update the sub-band info for the UE */ 
   dlfsUe->numSbCqi = numSbCqi;

   /* Update WB cqi for DLFS UE */
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   rgSCHDlfsUpdWbCqi(dlfsUe, cmnDlUe->mimoInfo.cwInfo[0].cqi);

#ifndef  TFU_UPGRADE
   if (ue->dl.ueDlCqiCfg.prdCqiCfg.pres)
   {
      ret = rgSCHDlfsUpdPrdModeCfg(cell, ue, &ue->dl.ueDlCqiCfg.prdCqiCfg);
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   }
#else
   if (ue->dl.ueDlCqiCfg.prdCqiCfg.type)
   {
      ret = rgSCHDlfsUpdPrdModeCfg(cell, ue, &ue->dl.ueDlCqiCfg.prdCqiCfg);
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   }
#endif
   if (ue->dl.ueDlCqiCfg.aprdCqiCfg.pres)
   {
      rgSCHDlfsUpdAprdModeCfg(cell, ue, &ue->dl.ueDlCqiCfg.aprdCqiCfg);
   }

   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsUeCfg: numSbCqi: %d, numBps: %d, prdSbSize: %d,"
            "aprdSbSize %d\n", dlfsUe->numSbCqi, dlfsUe->numBp,
            dlfsUe->prdSbSize, dlfsUe->aprdSbSize));

   RETVALUE(ROK);
}

/**
 * @brief Initializes the subframe for DLFS
 *
 * @details
 *
 *     Function : rgSCHDlfsInitSf
 *
 *     Processing Steps:
 *      - Allocate DL sub-bands
 *      - Initialize DL sub-bands for each DL subframe.
 *        Call rgSCHDlfsInitSfSbs().
 *      - Initialize DL holes for each DL subframe. Call rgSCHDlfsInitSfHoles().
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlfsSf     *sf
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsInitSf
(
RgSchCellCb        *cell,
RgSchDlfsSf        *dlfsSf
)
#else
PRIVATE S16 rgSCHDlfsInitSf (cell, dlfsSf)
RgSchCellCb        *cell;
RgSchDlfsSf        *dlfsSf;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   TRC2(rgSCHDlfsInitSf);
   
   dlfsSf->numDlSb = 
      (dlfsCell->totDlBw + dlfsCell->rbgSize - 1)/ (dlfsCell->rbgSize); 

   /* Allocate DFLS Sub-bands */
   if((ret = rgSCHUtlAllocSBuf(instIdx, 
               (Data**)&(dlfsSf->dlSb), 
             dlfsSf->numDlSb * sizeof(RgSchDlfsSb))) != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Memory allocation FAILED for sub-bands\n"));
      RETVALUE(ret);
   }
   
   /* Initialize DL sub-bands */
   rgSCHDlfsInitSfSbs(dlfsCell, dlfsSf);

   /* Initialize Hole control block */
   rgSCHDlfsInitSfHoles(dlfsSf);

   dlfsSf->availDlBw = dlfsCell->totDlBw;
#ifdef LTEMAC_SPS
   /* Setting allocated bandwidth to SPS bandwidth for non-SPS RB allocator */
   dlfsSf->allocDlBw = cell->dlSpsCfg.maxSpsDlBw;
   rgSCHDlfsRstHoleStart(dlfsCell, dlfsSf); 
#else
   dlfsSf->allocDlBw = 0;
#endif
   
   RETVALUE(ret);
}

/**
 * @brief Re-initializes the subframe for dlfs
 *
 * @details
 *
 *     Function : rgSCHDlfsReinitSf
 *
 *     Processing Steps:
 *      - Re-initialize DL holes. Call rgSCHDlfsInitSfHoles().
 *
 *  @param[in]  RgSchDlfsCellCb *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDlfsReinitSf
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf
)
#else
PUBLIC Void rgSCHDlfsReinitSf(cell, dlSf)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
#endif
{
   RgSchDlfsSf     *dlfsSf   = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   TRC2(rgSCHDlfsReinitSf);

   dlSf->bwAlloced   = 0;
   rgSCHDlfsInitSfHoles(dlfsSf);
   dlfsSf->availDlBw = dlfsCell->totDlBw;
#ifdef LTEMAC_SPS
   /* Setting allocated bandwidth to SPS bandwidth for non-SPS RB allocator */
   dlfsSf->allocDlBw = cell->dlSpsCfg.maxSpsDlBw;
   rgSCHDlfsRstHoleStart(dlfsCell, dlfsSf); 
#else
   dlfsSf->allocDlBw = 0;
#endif

   RETVOID;
}

/**
 * @brief Initializes the Bandwidth part information for a UE
 *
 * @details
 *
 *     Function : rgSCHDlfsInitUeBp
 *
 *     Processing Steps:
 *     - Initialize the bandwidth parts for the UE.
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @param[in]  RgSchDlfsUe     *dlfsUe
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHDlfsInitUeBp
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe
)
#else
PRIVATE Void rgSCHDlfsInitUeBp (dlfsCell, dlfsUe)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
#endif
{
   U8              bpIdx             = 0;
   U8              numDlfsSbPerPrdSb = dlfsUe->prdSbSize/dlfsCell->rbgSize;
   U8              numPrdSbPerCell   = ((dlfsCell->totDlBw) +
         (dlfsUe->prdSbSize - 1))/(dlfsUe->prdSbSize);
   U8              numPrdSbPerBp     = numPrdSbPerCell/dlfsUe->numBp;
   U8              numLargeBp        = numPrdSbPerCell % dlfsUe->numBp;
   U8              bpStart           = 0;
   RgSchDlfsUeBpCb *bpCb             = NULLP;
   TRC2(rgSCHDlfsInitUeBp);

   /* Update larger bandwidth parts with 
    * numPrdSbs = cieling(N_DL_RB/prdSbSize/numBps) */
   for (bpIdx = 0; bpIdx < numLargeBp; ++bpIdx)
   {
      bpCb = &(dlfsUe->bpCbs[bpIdx]);
      bpCb->sbStartIdx = bpStart;
      bpCb->sbEndIdx = bpStart + ((numPrdSbPerBp + 1) * numDlfsSbPerPrdSb) - 1;
      bpStart = bpCb->sbEndIdx + 1;
      bpCb->rptCqi = RG_SCH_DLFS_INVALID_CQI;
   }
   
   /* Update smaller bandwidth parts with
    * numPrdSbs = cieling(N_DL_RB/prdSbSize/numBps) - 1 */
   for (bpIdx = numLargeBp; bpIdx < dlfsUe->numBp; ++bpIdx)
   {
      bpCb = &(dlfsUe->bpCbs[bpIdx]);
      bpCb->sbStartIdx = bpStart;
      bpCb->sbEndIdx = bpStart + (numPrdSbPerBp * numDlfsSbPerPrdSb) - 1;
      bpStart = bpCb->sbEndIdx + 1;
      bpCb->rptCqi = RG_SCH_DLFS_INVALID_CQI;
   }

   /* If the last BP goes beyond the last DLFS sub-band, update 
    * bpCb->sbEndIdx appropriately */
   if ( (bpCb) && (bpCb->sbEndIdx > (dlfsCell->numDlSb - 1)) )
   {
      bpCb->sbEndIdx = dlfsCell->numDlSb - 1;
   }

   RETVOID;
}

/**
 * @brief Free the Bandwidth part information for a UE
 *
 * @details
 *
 *     Function : rgSCHDlfsFreeUeBp
 *
 *     Processing Steps:
 *     - Free the bandwidth parts for the UE.
 *
 *  @param[in]  Inst            instIdx
 *  @param[in]  RgSchDlfsUe     *dlfsUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsFreeUeBp
(
Inst               instIdx,
RgSchDlfsUeCb      *dlfsUe
)
#else
PRIVATE Void rgSCHDlfsFreeUeBp (instIdx, dlfsUe)
Inst               instIdx;
RgSchDlfsUeCb      *dlfsUe;
#endif
{
   TRC2(rgSCHDlfsFreeUeBp);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(instIdx, (Data**)(&(dlfsUe->bpCbs)), 
         (dlfsUe->numBp * sizeof(RgSchDlfsUeBpCb)));
   dlfsUe->numBp = 0;

   RETVOID;
}

/**
 * @brief Initializes the subframe for DLFS
 *
 * @details
 *
 *     Function : rgSCHDlfsInitPrdMode20
 *
 *     Processing Steps:
 *
 *  @param[in]  Inst              instIdx
 *  @param[in]  RgSchDlfsCellCb   *dlfsCell
 *  @param[in]  RgSchDlfsUeCb     *dlfsUe
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsInitPrdMode20
(
Inst                   instIdx,
RgSchDlfsCellCb        *dlfsCell,
RgSchDlfsUeCb          *dlfsUe,
RgrUePrdDlCqiCfg       *prdCqiCfg
)
#else
PRIVATE S16 rgSCHDlfsInitPrdMode20(instIdx, dlfsCell, dlfsUe, prdCqiCfg)
Inst                   instIdx;
RgSchDlfsCellCb        *dlfsCell;
RgSchDlfsUeCb          *dlfsUe;
RgrUePrdDlCqiCfg       *prdCqiCfg;
#endif
{
   S16                 ret = ROK;
   TRC2(rgSCHDlfsInitPrdMode20);

   /* Initialize periodic sub-band size for the UE */
   RG_SCH_DLFS_GET_PRD_SBSIZE(&(dlfsUe->prdSbSize), 
         dlfsCell->totDlBw);
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
   /* Update k and I_cqi_pmi value */
   dlfsUe->k           = prdCqiCfg->k;

   /* Get periodicity value */
   RG_SCH_DLFS_GET_PRDICITY_AND_OFFST(prdCqiCfg->cqiPmiCfgIdx,
         &dlfsUe->periodicity, &dlfsUe->cqiRptOffst);
#else
   /* Update k and I_cqi_pmi value */
   dlfsUe->k           = prdCqiCfg->cqiSetup.k;

   /* Get periodicity value */
   RG_SCH_DLFS_GET_PRDICITY_AND_OFFST(prdCqiCfg->cqiSetup.cqiPCfgIdx,
         &dlfsUe->periodicity, &dlfsUe->cqiRptOffst);



#endif
   /* Allocate bandwidth parts for the UE */
   dlfsUe->numBp = dlfsCell->numBp;
   if (dlfsUe->numBp)
   {
      if((ret = rgSCHUtlAllocSBuf(instIdx, 
                  (Data**)&(dlfsUe->bpCbs), 
                  dlfsUe->numBp * (sizeof(RgSchDlfsUeBpCb)))) 
            != ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "Allocation for bandwidth parts failed for"
                  "the UE\n"));
         RETVALUE(ret);
      }
      /* Initialize bandwidth part of the UE */
      rgSCHDlfsInitUeBp(dlfsCell, dlfsUe);
   }
   RETVALUE(ret);
}

/**
 * @brief Initializes the holes for the subframe
 *
 * @details
 *
 *     Function : rgSCHDlfsInitSfHoles
 *
 *     Processing Steps:
 *      - Initializes DL holes for the subframe.
 *
 *  @param[in]  RgSchDlfsSf   *dlfsSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsInitSfHoles
(
RgSchDlfsSf      *dlfsSf
)
#else
PRIVATE Void rgSCHDlfsInitSfHoles (dlfsSf)
RgSchDlfsSf      *dlfsSf;
#endif
{
   RgSchDlfsHole *hole = NULLP;
   U8            sbIdx;
   TRC2(rgSCHDlfsInitSfHoles);

   cmLListInit(&(dlfsSf->holeLst));
   for (sbIdx = 0; sbIdx < dlfsSf->numDlSb; ++sbIdx)
   {
      hole = &dlfsSf->dlSb[sbIdx];
      hole->isAvail = TRUE;
      hole->holeLnk.node = (PTR)hole; 
      cmLListAdd2Tail(&(dlfsSf->holeLst), &(hole->holeLnk)); 
   }

   RETVOID;
}

/**
 * @brief Initializes the sub-bands for the subframe
 *
 * @details
 *
 *     Function : rgSCHDlfsInitSfSbs
 *
 *     Processing Steps:
 *      - Initializes DL sub-bands for the subframe.
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @param[in]  RgSchDlfsSf     *dlfsSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsInitSfSbs
(
RgSchDlfsCellCb *dlfsCell,
RgSchDlfsSf     *dlfsSf
)
#else
PRIVATE Void rgSCHDlfsInitSfSbs (dlfsCell, dlfsSf)
RgSchDlfsCellCb *dlfsCell;
RgSchDlfsSf     *dlfsSf;
#endif
{
   U8           sbIdx = 0;
   TRC2(rgSCHDlfsInitSfSbs);

   for (sbIdx = 0; sbIdx < dlfsSf->numDlSb - 1; ++sbIdx)
   {
      dlfsSf->dlSb[sbIdx].start = sbIdx * dlfsCell->rbgSize;
      dlfsSf->dlSb[sbIdx].sbIdx = sbIdx;
      dlfsSf->dlSb[sbIdx].sbSize = dlfsCell->rbgSize;
   }

   /* Initialize the last sub-band */
   dlfsSf->dlSb[sbIdx].start = sbIdx * dlfsCell->rbgSize;
   dlfsSf->dlSb[sbIdx].sbSize = (dlfsCell->totDlBw % dlfsCell->rbgSize) ? 
      dlfsCell->totDlBw - (dlfsCell->rbgSize * 
            (dlfsCell->totDlBw/dlfsCell->rbgSize)):dlfsCell->rbgSize;
   dlfsSf->dlSb[sbIdx].sbIdx = sbIdx;

   RETVOID;
}

/**
 * @brief Resets the start of hole list for DLFS
 *
 * @details
 *
 *     Function : rgSCHDlfsRstHoleStart
 *
 *     Processing Steps:
 *     - Assumption: All the allocated bandwidth is contiguous and starts from
 *       RB 0.
 *     - Reset the start of the hole list depending on the allocated bandwidth
 *       in the sub-frame.
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @param[in]  RgSchDlfsSf     *dlfsSf
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHDlfsRstHoleStart
(
RgSchDlfsCellCb *dlfsCell,
RgSchDlfsSf     *dlfsSf
)
#else
PRIVATE Void rgSCHDlfsRstHoleStart (dlfsCell, dlfsSf)
RgSchDlfsCellCb *dlfsCell;
RgSchDlfsSf     *dlfsSf;
#endif
{
   U8           numAllocHoles = 0;
   U8           sbIdx = 0;
   TRC2(rgSCHDlfsRstHoleStart);

   numAllocHoles = (dlfsSf->allocDlBw + 
         (dlfsCell->rbgSize - 1))/dlfsCell->rbgSize;
   
   for (sbIdx = 0; sbIdx < numAllocHoles; ++sbIdx)
   {
      /* Set the sub-bands as used: assuming that the allocation for common
       * channels starts from RB 0 */
      if (dlfsSf->dlSb[sbIdx].isAvail)
      {
         dlfsSf->dlSb[sbIdx].isAvail = FALSE;
         rgSCHDlfsRlsHole(dlfsSf, &(dlfsSf->dlSb[sbIdx]));
      }
   }
   
   RETVOID;
}

#ifdef RGR_V1
/*rg009.201 - CR timer implementation changes*/
/**
 * @brief Performs RB allocation for MSG4 list
 * cell
 *
 * @details
 *
 *     Function : rgSCHDlfsCcchSduAlloc
 *
 *     Processing Steps:
 *      - Perform allocations in consecutive holes.
 *
 *  @param[in]      RgSchCellCb         *cell
 *  @param[in, out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]      U8                    isRetx
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsCcchSduAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo,
U8                    isRetx
)
#else
PRIVATE S16 rgSCHDlfsCcchSduAlloc(cell, allocInfo, isRetx) 
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
U8                    isRetx;
#endif
{
   S16                ret            = ROK;
   CmLList            *ccchSduAllocNode = NULLP;
   CmLListCp          *ccchSduLst;
   CmLListCp          *allocLst;
   CmLListCp          *nonAllocLst;
   RgSchDlRbAlloc     *reqAllocInfo;
   RgSchDlHqProcCb    *hqP = NULLP;
#ifdef DEBUGP
   Inst               instIdx       = cell->instIdx;
#endif
   TRC2(rgSCHDlfsCcchSduAlloc);

   if (isRetx)
   {
      ccchSduLst = &(allocInfo->ccchSduAlloc.ccchSduRetxLst);
      allocLst = &(allocInfo->ccchSduAlloc.schdCcchSduRetxLst);
      nonAllocLst = &(allocInfo->ccchSduAlloc.nonSchdCcchSduRetxLst);
   }
   else
   {
      ccchSduLst = &(allocInfo->ccchSduAlloc.ccchSduTxLst);
      allocLst = &(allocInfo->ccchSduAlloc.schdCcchSduTxLst);
      nonAllocLst = &(allocInfo->ccchSduAlloc.nonSchdCcchSduTxLst);
   }
   
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsCcchSduAlloc: cell: %d, isRetx: %d, reqLst Count: %d\n",
            cell->cellId, isRetx, (int)ccchSduLst->count));

   ccchSduAllocNode = cmLListFirst(ccchSduLst);
   while (ccchSduAllocNode)
   {
      hqP = ((RgSchDlHqProcCb *)(ccchSduAllocNode->node));	   
      reqAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE
         (hqP->hqE->ue);
      RG_SCH_CMN_INIT_SCHD_LNK(&(hqP->schdLstLnk), hqP);
      reqAllocInfo->dlSf = allocInfo->ccchSduAlloc.ccchSduDlSf;
      if ((ret = rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo)) != ROK)
      {
         break;
      }
      cmLListAdd2Tail(allocLst, &(hqP->schdLstLnk));
      ccchSduAllocNode = ccchSduAllocNode->next;
   }

   /* Add the remaining ones to non-scheduled list */
   while (ccchSduAllocNode)
   { 
      hqP = ((RgSchDlHqProcCb *)(ccchSduAllocNode->node));	   
      reqAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(hqP->hqE->ue);
      RG_SCH_CMN_INIT_SCHD_LNK(&(hqP->schdLstLnk), hqP);
      cmLListAdd2Tail(nonAllocLst, &(hqP->schdLstLnk));
      ccchSduAllocNode = ccchSduAllocNode->next;
   }

   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsCcchSduAlloc done: cellId : %d, allocCount: %d "
            "nonAllocCount: %d\n", cell->cellId, (int)allocLst->count,
            (int)nonAllocLst->count));
   RETVALUE(ret);
}
#endif
/**
 * @brief Performs RB allocation for MSG4 list
 * cell
 *
 * @details
 *
 *     Function : rgSCHDlfsMsg4Alloc
 *
 *     Processing Steps:
 *      - Perform allocations in consecutive holes.
 *
 *  @param[in]      RgSchCellCb         *cell
 *  @param[in, out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]      U8                    isRetx
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsMsg4Alloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo,
U8                    isRetx
)
#else
PRIVATE S16 rgSCHDlfsMsg4Alloc(cell, allocInfo, isRetx) 
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
U8                    isRetx;
#endif
{
   S16                ret            = ROK;
   CmLList            *msg4AllocNode = NULLP;
   CmLListCp          *msg4Lst;
   CmLListCp          *allocLst;
   CmLListCp          *nonAllocLst;
   RgSchDlRbAlloc     *reqAllocInfo;
   RgSchDlHqProcCb    *hqP = NULLP;
#ifdef DEBUGP
   Inst               instIdx       = cell->instIdx;
#endif
   TRC2(rgSCHDlfsMsg4Alloc);

   if (isRetx)
   {
      msg4Lst = &(allocInfo->msg4Alloc.msg4RetxLst);
      allocLst = &(allocInfo->msg4Alloc.schdMsg4RetxLst);
      nonAllocLst = &(allocInfo->msg4Alloc.nonSchdMsg4RetxLst);
   }
   else
   {
      msg4Lst = &(allocInfo->msg4Alloc.msg4TxLst);
      allocLst = &(allocInfo->msg4Alloc.schdMsg4TxLst);
      nonAllocLst = &(allocInfo->msg4Alloc.nonSchdMsg4TxLst);
   }
   
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsMsg4Alloc: cell: %d, isRetx: %d, reqLst Count: %d\n",
            cell->cellId, isRetx, (int)msg4Lst->count));

   msg4AllocNode = cmLListFirst(msg4Lst);
   while (msg4AllocNode)
   {
      hqP = (RgSchDlHqProcCb *)(msg4AllocNode->node);	   
      reqAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_RACB
         (hqP->hqE->raCb);
      RG_SCH_CMN_INIT_SCHD_LNK(&(hqP->schdLstLnk), hqP);
      reqAllocInfo->dlSf = allocInfo->msg4Alloc.msg4DlSf;
      if ((ret = rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo)) != ROK)
      {
         break;
      }
      cmLListAdd2Tail(allocLst, &(hqP->schdLstLnk));
      msg4AllocNode = msg4AllocNode->next;
   }

   /* Add the remaining ones to non-scheduled list */
   while (msg4AllocNode)
   { 
      hqP = (RgSchDlHqProcCb *)(msg4AllocNode->node);	   
      reqAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_RACB
         (hqP->hqE->raCb);
      RG_SCH_CMN_INIT_SCHD_LNK(&(hqP->schdLstLnk), hqP);
      cmLListAdd2Tail(nonAllocLst, &(hqP->schdLstLnk));
      msg4AllocNode = msg4AllocNode->next;
   }

   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsMsg4Alloc done: cellId : %d, allocCount: %d "
            "nonAllocCount: %d\n", cell->cellId, (int)allocLst->count,
            (int)nonAllocLst->count));
   RETVALUE(ret);
}

/**
 * @brief Performs RB allocation for common channels for a frequency selective
 * cell
 *
 * @details
 *
 *     Function : rgSCHDlfsCmnRbAlloc
 *
 *     Processing Steps:
 *      - Allocate consecutively available holes.
 *
 *  @param[in]      RgSchCellCb *cell
 *  @param[in, out] RgSchDlRbAlloc  *cmnAllocInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsCmnRbAlloc
(
RgSchCellCb     *cell,
RgSchDlRbAlloc  *cmnAllocInfo
)
#else
PRIVATE S16 rgSCHDlfsCmnRbAlloc(cell, cmnAllocInfo)
RgSchCellCb     *cell;
RgSchDlRbAlloc  *cmnAllocInfo;
#endif
{
   RgSchDlfsSf  *dlfsSf  = RG_SCH_DLFS_GET_SF(cmnAllocInfo->dlSf); 
#ifdef LTEMAC_SPS
   U8                  rbStart;
   U8                  rbsAlloc = 0;
   U8                  spsRbsAlloc = 0;
   RgSchDlSfAllocInfo  *dlSfAlloc = &cmnAllocInfo->dlSf->dlSfAllocInfo;
#endif
#ifdef DEBUGP
   Inst         instIdx  = cell->instIdx;
#endif
   TRC2(rgSCHDlfsCmnRbAlloc);

   if (cmnAllocInfo->rbsReq > dlfsSf->availDlBw)
   {
#ifdef LTEMAC_SPS
      /* Attempt RA Type 2 allocation in SPS Bandwidth */
      spsRbsAlloc = 
         rgSCHCmnDlRaType2Alloc(dlSfAlloc,
               cmnAllocInfo->rbsReq, &cell->spsBwRbgInfo, &rbStart, 
               &cmnAllocInfo->resAllocInfo, FALSE);
      if (!spsRbsAlloc)
#endif
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),"DL BW exhausted for cell:" 
                  " %d\n", cell->cellId));
         RETVALUE(RFAILED);
      }
#ifdef LTEMAC_SPS
      rbsAlloc = spsRbsAlloc;
#endif
   }
#ifdef LTEMAC_SPS
   else
   {
      rbStart = dlfsSf->allocDlBw;
      rbsAlloc = cmnAllocInfo->rbsReq;
   }
#endif

   /* Update allocation information */
   /* Retrieve PDCCH */
   cmnAllocInfo->pdcch = rgSCHCmnCmnPdcchAlloc(cell, cmnAllocInfo->dlSf);
   if (cmnAllocInfo->pdcch == NULLP)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),"Commom PDCCH exhausted for cell:"
               " %d\n", cell->cellId));
      RETVALUE(RFAILED);
   }

   cmnAllocInfo->dciFormat = TFU_DCI_FORMAT_1A;
   cmnAllocInfo->raType = RG_SCH_CMN_RA_TYPE2;
   cmnAllocInfo->allocInfo.raType2.isLocal = TRUE;
#ifdef LTEMAC_SPS
   cmnAllocInfo->allocInfo.raType2.rbStart = rbStart;
   cmnAllocInfo->allocInfo.raType2.numRb = rbsAlloc;
   cmnAllocInfo->rbsAlloc = rbsAlloc;
#else
   cmnAllocInfo->allocInfo.raType2.rbStart = dlfsSf->allocDlBw;
   cmnAllocInfo->allocInfo.raType2.numRb = cmnAllocInfo->rbsReq;
   cmnAllocInfo->rbsAlloc = cmnAllocInfo->rbsReq;
#endif
   cmnAllocInfo->tbInfo[0].bytesAlloc = cmnAllocInfo->tbInfo[0].bytesReq;
#ifndef LTEMAC_SPS
   /* Update the sub-frame with new allocation */
   cmnAllocInfo->dlSf->bwAlloced += cmnAllocInfo->rbsReq;
   dlfsSf->allocDlBw += cmnAllocInfo->rbsReq;
   dlfsSf->availDlBw -= cmnAllocInfo->rbsReq;
#endif

#ifdef LTEMAC_SPS
   if (!spsRbsAlloc)
   {
      /* Update the sub-frame with new allocation */
      cmnAllocInfo->dlSf->bwAlloced += rbsAlloc;
      dlfsSf->allocDlBw += rbsAlloc;
      dlfsSf->availDlBw -= rbsAlloc;
   }
   else
   {
      U8    idx;
      /* Update type 0, 1 and 2 masks in DL SF */
      dlSfAlloc->raType0Mask    |= cmnAllocInfo->resAllocInfo.raType0Mask;
      for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx)
      {
         dlSfAlloc->raType1Mask[idx] |=
            cmnAllocInfo->resAllocInfo.raType1Mask[idx];
         dlSfAlloc->raType1UsedRbs[idx] +=
            cmnAllocInfo->resAllocInfo.raType1UsedRbs[idx];
      }
      for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx)
      {
         dlSfAlloc->raType2Mask[idx] |=
            cmnAllocInfo->resAllocInfo.raType2Mask[idx];
      }
   }
#endif

   RETVALUE(ROK);
}

/**
 * @brief Performs RB allocation for dedicated channels for a frequency 
 * selective
 * cell for transmitting Ues 
 *
 * @details
 *
 *     Function : rgSCHDlfsTxUeRbAlloc
 *
 *     Processing Steps:
 *     - Assumption: Each UE has one entry in transmitting or re-transmitting
 *       UE list.
 *     - If common channel allocations exist in the sub-frame, 
 *       re-set the hole list's start.
 *     - Sort the UE list in descending order of 'average CQI devaition' of the
 *       UE. 
 *     - For each UE,
 *      - if (avgCqiDeviation != 0)
 *         - Perform sub-band allocations for UE. Call rgSCHDlfsSbAllocForUe().
 *      - else
 *         - Perform wide-band allocations for UE with 'avgCqiDeviation == 0'. 
 *         Call rgSCHDlfsWbAllocForTxUe().
 *     - MIMO: Allocate as much as requested by TbInfo[0], tbInfo[1]'s allocation is
 *             a result of tbInfo[0]'s allocation.
 *
 *  @param[in]      RgSchCellCb        *cell
 *  @param[in, out] RgSchCmnUeRbAlloc  *allocInfo
 *  @return  Void
 **/
#ifdef LTEMAC_SPS
#ifdef ANSI
PRIVATE Void rgSCHDlfsTxUeRbAlloc
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
CmLListCp          *ueLst,
CmLListCp          *schdUeLst,
CmLListCp          *nonSchdUeLst
)
#else
PRIVATE Void rgSCHDlfsTxUeRbAlloc(cell, dlSf, ueLst, schdUeLst, nonSchdUeLst)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
CmLListCp          *ueLst;
CmLListCp          *schdUeLst;
CmLListCp          *nonSchdUeLst;
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHDlfsTxUeRbAlloc
(
RgSchCellCb        *cell,
RgSchCmnUeRbAlloc  *allocInfo
)
#else
PRIVATE Void rgSCHDlfsTxUeRbAlloc(cell, allocInfo)
RgSchCellCb        *cell;
RgSchCmnUeRbAlloc  *allocInfo;
#endif
#endif /* LTEMAC_SPS */
{
   CmLList           *lstPtr       = NULLP;
   CmLList           *prevLstPtr   = NULLP;
#ifdef LTEMAC_SPS
   CmLList         *nonSchdLstPtr = NULLP;
   Bool            isSpsBwAvail   = TRUE;
#endif
#ifndef LTEMAC_SPS
   CmLListCp         *ueLst        = NULLP;
   CmLListCp         *schdUeLst    = NULLP;
   CmLListCp         *nonSchdUeLst = NULLP;
#endif
   RgSchDlfsUeCb     *dlfsUe       = NULLP;
   RgSchDlRbAlloc    *dlAllocCb    = NULLP;
   RgSchUeCb         *ue           = NULLP;
#ifndef LTEMAC_SPS
   RgSchDlSf         *dlSf         = allocInfo->dedDlSf;
#endif
   RgSchDlfsSf       *dlfsSf       = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchDlfsCellCb   *dlfsCell     = RG_SCH_DLFS_GET_CELL(cell);
#ifdef DEBUGP
   Inst              instIdx       = cell->instIdx;
#endif
   RgSchDlHqProcCb    *hqP = NULLP;
   TRC2(rgSCHDlfsTxUeRbAlloc);

#ifndef LTEMAC_SPS
/* Initialize transmitting lists */
   ueLst = &(allocInfo->txUeLst);
   schdUeLst = &(allocInfo->schdTxUeLst); 
   nonSchdUeLst = &(allocInfo->nonSchdTxUeLst);
#endif

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsTxUeRbAlloc: cell: %d, reqLst Count: %d\n",
            cell->cellId, (int)ueLst->count));

   if (dlfsSf->holeLst.count)
   {
      /* Sort UE */
      rgSCHDlfsSortUeLst(ueLst, schdUeLst); 

      /* Perform allocation for the reported sub-bands */
      lstPtr = cmLListFirst(schdUeLst);
      while (dlfsSf->holeLst.count && lstPtr)
      {
         hqP = (RgSchDlHqProcCb *)(lstPtr->node);
         ue = hqP->hqE->ue;
         dlfsUe = RG_SCH_DLFS_GET_UE(ue);
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (!dlfsUe->avgCqiDeviation)
         {
            break;
         }
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
         if (dlAllocCb->raType == RG_SCH_CMN_RA_TYPE2)
         {
            rgSCHDlfsTyp2AllocForTxUe(dlSf, cell, ue, dlAllocCb);
         }
         else /* Type 0 */
         {
            rgSCHDlfsSbAllocForTxUe(dlSf, cell, ue, dlAllocCb);
         }
         prevLstPtr = lstPtr;
         lstPtr = lstPtr->next;
         if (!dlAllocCb->rbsAlloc)
         {
            /* Remove the UE from the scheduled list */
            cmLListDelFrm(schdUeLst, prevLstPtr);
            /* Add UE to not scheduled UE list */
            cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
         }
      }

      /* If the holes remain, allocate using avgCqi */
      while (dlfsSf->holeLst.count && lstPtr)
      {
         hqP = (RgSchDlHqProcCb *)(lstPtr->node);
         ue = hqP->hqE->ue;
         dlfsUe = RG_SCH_DLFS_GET_UE(ue);
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (dlfsUe->avgSbCqi >= dlfsCell->thresholdCqi)
         {
            /* rg007.201 - Changes for MIMO feature addition */
            /* rg008.201 - Removed dependency on MIMO compile-time flag */
            if (dlAllocCb->raType == RG_SCH_CMN_RA_TYPE2)
            {
               rgSCHDlfsTyp2AllocForTxUe(dlSf, cell, ue, dlAllocCb);
            }
            else /* Type 0 */
            {
               rgSCHDlfsWbAllocForTxUe(dlSf, cell, ue, dlAllocCb);
            }
         }
         prevLstPtr = lstPtr; 
         lstPtr = lstPtr->next;
         if (!dlAllocCb->rbsAlloc)
         {
            /* Remove the UE from the scheduled list */
            cmLListDelFrm(schdUeLst, prevLstPtr);
            /* Add UE to not scheduled UE list */
            cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
         }
      }

      /* Add remaining UEs to non-scheduled list */
      while (lstPtr)
      {
         prevLstPtr = lstPtr; 
         lstPtr = lstPtr->next;
         cmLListDelFrm(schdUeLst, prevLstPtr);
         cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
      }
   }
   
#ifdef LTEMAC_SPS
      /* Attempt Scheduling for non-scheduled UEs in SPS BW */
      nonSchdLstPtr = cmLListFirst(nonSchdUeLst);
      while (isSpsBwAvail && nonSchdLstPtr)
      {
         hqP = (RgSchDlHqProcCb *)(nonSchdLstPtr->node);
         ue = hqP->hqE->ue;
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (!(isSpsBwAvail = 
                  rgSCHCmnAllocUeInSpsBw(dlSf, cell, ue, dlAllocCb, TRUE)))
         {
            break;
         }
         prevLstPtr = nonSchdLstPtr;
         nonSchdLstPtr = nonSchdLstPtr->next;
         if (dlAllocCb->rbsAlloc)
         {
            /* Remove the UE from the non-scheduled list */
            cmLListDelFrm(nonSchdUeLst, prevLstPtr);
            /* Add UE to scheduled UE list */
            cmLListAdd2Tail(schdUeLst, prevLstPtr);
         }
      }
#endif
   
   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsTxUeRbAlloc done: cell: %d, allocLst Count: %d" 
            " nonallocLst count %d\n",
            cell->cellId,(int)schdUeLst->count, (int)nonSchdUeLst->count));

   RETVOID;
}

/**
 * @brief Performs RB allocation for dedicated channels for a frequency 
 * selective cell for re-transmitting Ues 
 *
 * @details
 *
 *     Function : rgSCHDlfsRetxUeRbAlloc
 *
 *     Processing Steps:
 *     - Assumption: Each UE has one entry in transmitting or re-transmitting
 *       UE list.
 *     - If common channel allocations exist in the sub-frame, 
 *       re-set the hole list's start.
 *     - Sort the UE list in descending order of 'average CQI devaition' of the
 *       UE. 
 *     - For each UE,
 *      - if (avgCqiDeviation != 0)
 *         - Perform sub-band allocations for UE. Call rgSCHDlfsSbAllocForUe().
 *      - else
 *         - Perform wide-band allocations for UE with 'avgCqiDeviation == 0'.
 *         Call rgSCHDlfsWbAllocForTxUe().
 *
 *  @param[in]      RgSchCellCb    *cell
 *  @param[in, out] RgSchCmnUeRbAlloc  *allocInfo
 *  @return  Void
 **/
#ifdef LTEMAC_SPS
#ifdef ANSI
PRIVATE Void rgSCHDlfsRetxUeRbAlloc
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
CmLListCp          *ueLst,
CmLListCp          *schdUeLst,
CmLListCp          *nonSchdUeLst
)
#else
PRIVATE Void rgSCHDlfsRetxUeRbAlloc(cell, dlSf, ueLst, schdUeLst, nonSchdUeLst)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
CmLListCp          *ueLst;
CmLListCp          *schdUeLst;
CmLListCp          *nonSchdUeLst;
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHDlfsRetxUeRbAlloc
(
RgSchCellCb        *cell,
RgSchCmnUeRbAlloc  *allocInfo
)
#else
PRIVATE Void rgSCHDlfsRetxUeRbAlloc(cell, allocInfo)
RgSchCellCb        *cell;
RgSchCmnUeRbAlloc  *allocInfo;
#endif
#endif /* LTEMAC_SPS */
{
   CmLList         *lstPtr       = NULLP;
   CmLList         *prevLstPtr   = NULLP;
#ifndef LTEMAC_SPS
   CmLListCp       *ueLst        = NULLP;
   CmLListCp       *schdUeLst    = NULLP;
   CmLListCp       *nonSchdUeLst = NULLP;
#endif /* LTEMAC_SPS */
   RgSchDlfsUeCb   *dlfsUe       = NULLP;
   RgSchDlRbAlloc  *dlAllocCb    = NULLP;
   RgSchUeCb       *ue           = NULLP;
#ifndef LTEMAC_SPS
   RgSchDlSf       *dlSf         = allocInfo->dedDlSf;
#endif
   RgSchDlfsSf     *dlfsSf       = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchDlfsCellCb *dlfsCell     = RG_SCH_DLFS_GET_CELL(cell);
#ifdef DEBUGP
   Inst            instIdx       = cell->instIdx;
#endif
   RgSchDlHqProcCb    *hqP = NULLP;
   TRC2(rgSCHDlfsRetxUeRbAlloc);

   /* Initialize re-transmitting lists */
#ifndef LTEMAC_SPS
   ueLst = &(allocInfo->retxUeLst);
   schdUeLst = &(allocInfo->schdRetxUeLst); 
   nonSchdUeLst = &(allocInfo->nonSchdRetxUeLst);
#endif

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsRetxUeRbAlloc: cell: %d, reqLst Count: %d\n",
            cell->cellId,(int)ueLst->count));

   if (dlfsSf->holeLst.count)
   {
      /* Sort UE */
      rgSCHDlfsSortUeLst(ueLst, schdUeLst); 

      /* Perform allocation for the reported sub-bands */
      lstPtr = cmLListFirst(schdUeLst);
      while (dlfsSf->holeLst.count && lstPtr)
      {
         hqP = (RgSchDlHqProcCb *)(lstPtr->node);
         ue = hqP->hqE->ue;
         dlfsUe = RG_SCH_DLFS_GET_UE(ue);
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (!dlfsUe->avgCqiDeviation)
         {
            /* Break if average CQI deviation = 0 */
            break;
         }
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
         if (dlAllocCb->raType == RG_SCH_CMN_RA_TYPE2)
         {
            rgSCHDlfsTyp2AllocForRetxUe(dlSf, cell, ue, dlAllocCb);
         }
         else /* Type 0 */
         {
            rgSCHDlfsSbAllocForRetxUe(dlSf, cell, ue, dlAllocCb);
         }
         prevLstPtr = lstPtr;
         lstPtr = lstPtr->next;
         if (!dlAllocCb->rbsAlloc)
         {
            /* Remove the UE from the scheduled list */
            cmLListDelFrm(schdUeLst, prevLstPtr);
            /* Add UE to not scheduled UE list */
            cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
         }
      }

      /* If the holes remain, allocate using avgCqi */
      while (dlfsSf->holeLst.count && lstPtr)
      {
         hqP = (RgSchDlHqProcCb *)(lstPtr->node);
         ue = hqP->hqE->ue;
         dlfsUe = RG_SCH_DLFS_GET_UE(ue);
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (dlfsUe->avgSbCqi >= dlfsCell->thresholdCqi)
         {
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
            if (dlAllocCb->raType == RG_SCH_CMN_RA_TYPE2)
            {
               rgSCHDlfsTyp2AllocForRetxUe(dlSf, cell, ue, dlAllocCb);
            }
            else /* Type 0 */
            {
               rgSCHDlfsWbAllocForRetxUe(dlSf, cell, ue, dlAllocCb);
            }
         }
         prevLstPtr = lstPtr; 
         lstPtr = lstPtr->next;
         if (!dlAllocCb->rbsAlloc)
         {
            /* Remove the UE from the scheduled list */
            cmLListDelFrm(schdUeLst, prevLstPtr);
            /* Add UE to not scheduled UE list */
            cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
         }
      }

      /* Add remaining UEs to non-scheduled list */
      while (lstPtr)
      {
         prevLstPtr = lstPtr; 
         lstPtr = lstPtr->next;
         cmLListDelFrm(schdUeLst, prevLstPtr);
         cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
      }
   }

   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsRetxUeRbAlloc done: cell: %d, allocLst Count: %d"
            " nonallocLst count %d\n",
            cell->cellId, (int)schdUeLst->count, (int)nonSchdUeLst->count));

   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
 * @brief Performs RB allocation for dedicated channels for a frequency 
 * selective cell for RETX+TX Ues 
 *
 * @details
 *
 *     Function : rgSCHDlfsTxRetxUeRbAlloc
 *
 *     Processing Steps:
 *     - Assumption: Each UE has one entry in transmitting or re-transmitting
 *       UE list.
 *     - If common channel allocations exist in the sub-frame, 
 *       re-set the hole list's start.
 *     - Sort the UE list in descending order of 'average CQI devaition' of the
 *       UE. 
 *     - For each UE,
 *      - if (avgCqiDeviation != 0)
 *         - Perform sub-band allocations for UE. Call rgSCHDlfsSbAllocForUe().
 *      - else
 *         - Perform wide-band allocations for UE with 'avgCqiDeviation == 0'.
 *         Call rgSCHDlfsWbAllocForTxUe().
 *     - MIMO: Perform allocation for tbInfo[0]'s request, i.e the request for RETX
 *       allocation. Assign as a result the resources available on other codeword 
 *       for tbInfo[1]'s request. i.e the request for TX.
 *
 *  @param[in]      RgSchCellCb    *cell
 *  @param[in, out] RgSchCmnUeRbAlloc  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsTxRetxUeRbAlloc
(
RgSchCellCb        *cell,
RgSchCmnUeRbAlloc  *allocInfo
)
#else
PRIVATE Void rgSCHDlfsTxRetxUeRbAlloc(cell, allocInfo)
RgSchCellCb        *cell;
RgSchCmnUeRbAlloc  *allocInfo;
#endif
{
   CmLList         *lstPtr       = NULLP;
   CmLList         *prevLstPtr   = NULLP;
   CmLListCp       *ueLst        = NULLP;
   CmLListCp       *schdUeLst    = NULLP;
   CmLListCp       *nonSchdUeLst = NULLP;
   RgSchDlfsUeCb   *dlfsUe       = NULLP;
   RgSchDlRbAlloc  *dlAllocCb    = NULLP;
   RgSchUeCb       *ue           = NULLP;
   RgSchDlSf       *dlSf         = allocInfo->dedDlSf;
   RgSchDlfsSf     *dlfsSf       = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchDlfsCellCb *dlfsCell     = RG_SCH_DLFS_GET_CELL(cell);
#ifdef LTEMAC_SPS
   CmLList         *nonSchdLstPtr = NULLP;
   Bool            isSpsBwAvail   = TRUE;
#endif
#ifdef DEBUGP
   Inst            instIdx       = cell->instIdx;
#endif
   RgSchDlHqProcCb    *hqP = NULLP;
   TRC2(rgSCHDlfsTxRetxUeRbAlloc);

   /* Initialize re-transmitting lists */
   ueLst = &(allocInfo->txRetxUeLst);
   schdUeLst = &(allocInfo->schdTxRetxUeLst); 
   nonSchdUeLst = &(allocInfo->nonSchdTxRetxUeLst);

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsTxRetxUeRbAlloc: cell: %d, reqLst Count: %d\n",
            cell->cellId,(int)ueLst->count));

   if (dlfsSf->holeLst.count)
   {
      /* Sort UE */
      rgSCHDlfsSortUeLst(ueLst, schdUeLst); 

      /* Perform allocation for the reported sub-bands */
      lstPtr = cmLListFirst(schdUeLst);
      while (dlfsSf->holeLst.count && lstPtr)
      {
         hqP = (RgSchDlHqProcCb *)(lstPtr->node);
         ue = hqP->hqE->ue;
         dlfsUe = RG_SCH_DLFS_GET_UE(ue);
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (!dlfsUe->avgCqiDeviation)
         {
            /* Break if average CQI deviation = 0 */
            break;
         }
         rgSCHDlfsSbAllocForTxRetxUe(dlSf, cell, ue, dlAllocCb);
         prevLstPtr = lstPtr;
         lstPtr = lstPtr->next;
         if (!dlAllocCb->rbsAlloc)
         {
            /* Remove the UE from the scheduled list */
            cmLListDelFrm(schdUeLst, prevLstPtr);
            /* Add UE to not scheduled UE list */
            cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
         }
      }

      /* If the holes remain, allocate using avgCqi */
      while (dlfsSf->holeLst.count && lstPtr)
      {
         hqP = (RgSchDlHqProcCb *)(lstPtr->node);
         ue = hqP->hqE->ue;
         dlfsUe = RG_SCH_DLFS_GET_UE(ue);
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (dlfsUe->avgSbCqi >= dlfsCell->thresholdCqi)
         {
            rgSCHDlfsWbAllocForTxRetxUe(dlSf, cell, ue, dlAllocCb);
         }
         prevLstPtr = lstPtr; 
         lstPtr = lstPtr->next;
         if (!dlAllocCb->rbsAlloc)
         {
            /* Remove the UE from the scheduled list */
            cmLListDelFrm(schdUeLst, prevLstPtr);
            /* Add UE to not scheduled UE list */
            cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
         }
      }

      /* Add remaining UEs to non-scheduled list */
      while (lstPtr)
      {
         prevLstPtr = lstPtr; 
         lstPtr = lstPtr->next;
         cmLListDelFrm(schdUeLst, prevLstPtr);
         cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
      }
   }

#ifdef LTEMAC_SPS
      /* Attempt Scheduling for non-scheduled UEs in SPS BW */
      nonSchdLstPtr = cmLListFirst(nonSchdUeLst);
      while (isSpsBwAvail && nonSchdLstPtr)
      {
         ue = (RgSchUeCb *)(nonSchdLstPtr->node);
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (!(isSpsBwAvail = 
                  rgSCHCmnAllocUeInSpsBw(dlSf, cell, ue, dlAllocCb, TRUE)))
         {
            break;
         }
         prevLstPtr = nonSchdLstPtr;
         nonSchdLstPtr = nonSchdLstPtr->next;
         if (dlAllocCb->rbsAlloc)
         {
            /* Remove the UE from the non-scheduled list */
            cmLListDelFrm(nonSchdUeLst, prevLstPtr);
            /* Add UE to scheduled UE list */
            cmLListAdd2Tail(schdUeLst, prevLstPtr);
         }
      }
#endif

   RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
            "rgSCHDlfsTxRetxUeRbAlloc done: cell: %d, allocLst Count: %d"
            " nonallocLst count %d\n",
            cell->cellId, (int)schdUeLst->count, (int)nonSchdUeLst->count));

   RETVOID;
}

/**
 * @brief Calculates the average sub-band CQI deviation from the average 
 *        CQI
 *
 * @details
 *
 *     Function : rgSCHDlfsCalcSbCqiDeviation
 *
 *     Processing Steps:
 *      - Calculates the average sub-band CQI deviation from the average CQI for
 *      the UEs to be scheduled.
 *
 *  @param[in] RgSchDlfsUeCb *dlfsUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsCalcSbCqiDeviation
(
RgSchDlfsUeCb *dlfsUe
)
#else
PRIVATE Void rgSCHDlfsCalcSbCqiDeviation(dlfsUe)
RgSchDlfsUeCb *dlfsUe;
#endif
{
   U32        avgCqi = 0;
   U32        cqiDev = 0;
   U8         sbCnt  = 0;
   TRC2(rgSCHDlfsCalcSbCqiDeviation);
   
   /* Compute Average CQI for the UE */
   for (sbCnt = 0; sbCnt < dlfsUe->numSbCqi; ++sbCnt)
   {
      avgCqi += dlfsUe->sbCqiLst[sbCnt].cqi; 
   }
   avgCqi /= dlfsUe->numSbCqi;
   dlfsUe->avgSbCqi = avgCqi;

   /* Compute sum of CQI deviation from average CQI for the sub-bands */
   for (sbCnt = 0; sbCnt < dlfsUe->numSbCqi; ++sbCnt)
   {
      cqiDev += (dlfsUe->sbCqiLst[sbCnt].cqi > avgCqi) ? 
         (dlfsUe->sbCqiLst[sbCnt].cqi - avgCqi):
            (avgCqi - dlfsUe->sbCqiLst[sbCnt].cqi);
   }
   dlfsUe->avgCqiDeviation = cqiDev;

   RETVOID;
}

/**
 * @brief Sort the sub-band CQIs in descending order of CQIs for a UE 
 *
 * @details
 *
 *     Function : rgSCHDlfsSortSbCqiLst
 *
 *     Processing Steps:
 *      - Sorts the sub-band CQI list in descending order of CQIs for a UE
 *
 *  @param[in]  RgSchDlfsUeCb   *dlfsUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsSortSbCqiLst 
(
RgSchDlfsUeCb     *dlfsUe
)
#else
PRIVATE Void rgSCHDlfsSortSbCqiLst(dlfsUe)
RgSchDlfsUeCb     *dlfsUe;
#endif
{
   U8             sbCqiIdx;
   RgSchDlfsSbCqi *nextSbCqi = NULLP;
   TRC2(rgSCHDlfsSortSbCqiLst);

   dlfsUe->sortSbCqiLst =  &dlfsUe->sbCqiLst[0];
   dlfsUe->sortSbCqiLst->nxtSbCqi = NULLP;

   for(sbCqiIdx = 1; sbCqiIdx < dlfsUe->numSbCqi; ++sbCqiIdx)
   {
      nextSbCqi = dlfsUe->sortSbCqiLst;
      if (dlfsUe->sbCqiLst[sbCqiIdx].cqi >= dlfsUe->sortSbCqiLst->cqi)
      {
         /* Insert at the beginning of the list */
         (dlfsUe->sbCqiLst[sbCqiIdx]).nxtSbCqi = dlfsUe->sortSbCqiLst;
         dlfsUe->sortSbCqiLst = &(dlfsUe->sbCqiLst[sbCqiIdx]);
         continue;
      }
      while (nextSbCqi->nxtSbCqi)
      {
         /* Insert at the correct position in the list */
         if (dlfsUe->sbCqiLst[sbCqiIdx].cqi <= nextSbCqi->cqi &&
               dlfsUe->sbCqiLst[sbCqiIdx].cqi > nextSbCqi->nxtSbCqi->cqi)
         {
            (dlfsUe->sbCqiLst[sbCqiIdx]).nxtSbCqi = nextSbCqi->nxtSbCqi;
            nextSbCqi->nxtSbCqi = &(dlfsUe->sbCqiLst[sbCqiIdx]);
            break;
         }
         nextSbCqi = nextSbCqi->nxtSbCqi; 
      }
      if (!(nextSbCqi->nxtSbCqi))
      {
         /* Insert at the end of the list */
         (dlfsUe->sbCqiLst[sbCqiIdx]).nxtSbCqi = NULLP;
         nextSbCqi->nxtSbCqi = &(dlfsUe->sbCqiLst[sbCqiIdx]);
      }
   }

   RETVOID;
}

/**
 * @brief Updates the reported Wideband CQI for the UE.
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdWbCqi
 *
 *     Processing Steps:
 *      - Update the wideband CQI value for the UE.
 *
 *  @param[in]  RgSchDlfsUeCb   *dlfsUe
 *  @param[in]  U8              wbCqi
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdWbCqi
(
RgSchDlfsUeCb   *dlfsUe,
U8              wbCqi
)
#else
PRIVATE Void rgSCHDlfsUpdWbCqi(dlfsUe, wbCqi)
RgSchDlfsUeCb   *dlfsUe;
U8              wbCqi;
#endif
{
   U8           sbCqiIdx = 0; 
   U8           numSbCqi = dlfsUe->numSbCqi;
   TRC2(rgSCHDlfsUpdWbCqi);
   
   dlfsUe->wbCqi = wbCqi;

   for (sbCqiIdx = 0; sbCqiIdx < (numSbCqi - 1); sbCqiIdx++)
   {
      dlfsUe->sbCqiLst[sbCqiIdx].cqi = wbCqi; 
      dlfsUe->sbCqiLst[sbCqiIdx].sbIdx = sbCqiIdx;
      dlfsUe->sbCqiLst[sbCqiIdx].nxtSbCqi = &(dlfsUe->sbCqiLst[sbCqiIdx+1]);
   }
   
   /* Update the last entry */
   dlfsUe->sbCqiLst[sbCqiIdx].cqi = wbCqi; 
   dlfsUe->sbCqiLst[sbCqiIdx].sbIdx = sbCqiIdx;
   dlfsUe->sbCqiLst[sbCqiIdx].nxtSbCqi = NULLP;

   /* Update the sorted list pointer */
   dlfsUe->sortSbCqiLst = &(dlfsUe->sbCqiLst[0]);
   
   /* Update average CQI and CQI deviation for the UE */
   dlfsUe->avgSbCqi = wbCqi;
   dlfsUe->avgCqiDeviation = 0;

   RETVOID;
}

/**
 * @brief Updates the aperiodic CQI report for Mode 2-0
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdMode20SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-0.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPuschMode20  *mode20CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdMode20SbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPuschMode20  *mode20CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdAprdMode20SbCqi(dlfsCell, dlfsUe, mode20CqiInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPuschMode20  *mode20CqiInfo;
#endif
{
   U8              sbIdx    = 0;
   U8              startIdx = 0;
   U8              remRbs   = 0;
   U8              sbCqi    = mode20CqiInfo->cqiIdx;
   TRC2(rgSCHDlfsUpdAprdMode20SbCqi);

   if (!dlfsUe->aprdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   for (sbIdx = 0; sbIdx < mode20CqiInfo->numSubband; ++sbIdx)
   {
      startIdx = 
         ((mode20CqiInfo->subbandArr[sbIdx].rbStart)/dlfsCell->rbgSize);
      remRbs = mode20CqiInfo->subbandArr[sbIdx].numRb;
      while (remRbs >= dlfsCell->rbgSize)
      {
         dlfsUe->sbCqiLst[startIdx].cqi = sbCqi;         
         remRbs -= dlfsCell->rbgSize;
         startIdx += 1; 
      }
      /* Updation for the last small-sized sub-band */
      if (remRbs)
      {
         dlfsUe->sbCqiLst[startIdx].cqi = sbCqi;         
      }
   }

   /* Arrange the sub-bands of the UE in descending order of CQI */
   rgSCHDlfsSortSbCqiLst(dlfsUe);

   /* Calculate sub-band CQI deviation from average for the UE */
   rgSCHDlfsCalcSbCqiDeviation(dlfsUe);

   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */

/**
 * @brief Updates the aperiodic CQI report for Mode 3-1
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdMode31SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the aperiodic reporting mode 3-1.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPuschMode31  *mode31CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdMode31SbCqi
(
RgSchDlfsCellCb     *dlfsCell,
RgSchDlfsUeCb       *dlfsUe,
TfuCqiPuschMode31   *mode31CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdAprdMode31SbCqi(dlfsCell, dlfsUe, mode31CqiInfo)
RgSchDlfsCellCb     *dlfsCell;
RgSchDlfsUeCb       *dlfsUe;
TfuCqiPuschMode31   *mode31CqiInfo;
#endif
{
   U8               sbIdx      = 0;
   U8               startIdx   = 0;
   U8               remRbs     = 0;
   U8               sbCqi;
   TfuSubbandDlCqiInfo *sbCqiInfo = NULLP;
   TRC2(rgSCHDlfsUpdAprdMode31SbCqi);

   if (!dlfsUe->aprdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   for (sbIdx = 0; sbIdx < mode31CqiInfo->numSubband; ++sbIdx)
   {
      sbCqiInfo = &(mode31CqiInfo->subbandCqiArr[sbIdx]);
      startIdx = ((sbCqiInfo->subband.rbStart)/dlfsCell->rbgSize);
      remRbs = sbCqiInfo->subband.numRb;
      sbCqi = (sbCqiInfo->cqiIdx[0] + sbCqiInfo->cqiIdx[1])/2;
      while (remRbs >= dlfsCell->rbgSize)
      {
         dlfsUe->sbCqiLst[startIdx].cqi = sbCqi;         
         remRbs -= dlfsCell->rbgSize;
         startIdx += 1; 
      }
      /* Updation for the last small-sized sub-band */
      if (remRbs)
      {
         dlfsUe->sbCqiLst[startIdx].cqi = sbCqi;         
      }
   }

   /* Arrange the sub-bands of the UE in descending order of CQI */
   rgSCHDlfsSortSbCqiLst(dlfsUe);

   /* Calculate sub-band CQI deviation from average for the UE */
   rgSCHDlfsCalcSbCqiDeviation(dlfsUe);

   RETVOID;
}

/**
 * @brief Updates the aperiodic CQI report for Mode 2-2 
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdMode22SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the aperiodic reporting mode 22.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPuschMode22  *mode22CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdMode22SbCqi
(
RgSchDlfsCellCb     *dlfsCell,
RgSchDlfsUeCb       *dlfsUe,
TfuCqiPuschMode22   *mode22CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdAprdMode22SbCqi(dlfsCell, dlfsUe, mode22CqiInfo)
RgSchDlfsCellCb     *dlfsCell;
RgSchDlfsUeCb       *dlfsUe;
TfuCqiPuschMode22   *mode22CqiInfo;
#endif
{
   U8               sbIdx      = 0;
   U8               startIdx   = 0;
   U8               remRbs     = 0;
   U8               sbCqi;
   TfuSubbandInfo   *sbCqiInfo = NULLP;
   TRC2(rgSCHDlfsUpdAprdMode22SbCqi);

   if (!dlfsUe->aprdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   /* subband CQI is considered as the average of the two CW
    * CQIs reported over M-preferred subbands. */
   sbCqi = (mode22CqiInfo->cqi[0] + mode22CqiInfo->cqi[1])/2;
   for (sbIdx = 0; sbIdx < mode22CqiInfo->numSubband; ++sbIdx)
   {
       sbCqiInfo =(TfuSubbandInfo*) &(mode22CqiInfo->subbandArr[sbIdx]);
      startIdx = ((sbCqiInfo->rbStart)/dlfsCell->rbgSize);
      remRbs = sbCqiInfo->numRb;
      while (remRbs >= dlfsCell->rbgSize)
      {
         dlfsUe->sbCqiLst[startIdx].cqi = sbCqi;         
         remRbs -= dlfsCell->rbgSize;
         startIdx += 1; 
      }
      /* Updation for the last small-sized sub-band */
      if (remRbs)
      {
         dlfsUe->sbCqiLst[startIdx].cqi = sbCqi;         
      }
   }

   /* Arrange the sub-bands of the UE in descending order of CQI */
   rgSCHDlfsSortSbCqiLst(dlfsUe);

   /* Calculate sub-band CQI deviation from average for the UE */
   rgSCHDlfsCalcSbCqiDeviation(dlfsUe);

   RETVOID;
}
/**
 * @brief Updates the aperiodic CQI report for Mode 3-0
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdMode30SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the aperiodic reporting mode 3-0.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPuschMode30  *mode30CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdMode30SbCqi
(
RgSchDlfsCellCb     *dlfsCell,
RgSchDlfsUeCb       *dlfsUe,
TfuCqiPuschMode30   *mode30CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdAprdMode30SbCqi(dlfsCell, dlfsUe, mode30CqiInfo)
RgSchDlfsCellCb     *dlfsCell;
RgSchDlfsUeCb       *dlfsUe;
TfuCqiPuschMode30   *mode30CqiInfo;
#endif
{
   U8               sbIdx      = 0;
   U8               startIdx   = 0;
   U8               remRbs     = 0;
   U8               sbCqi;
   TfuSubbandMode30 *sbCqiInfo = NULLP;
   TRC2(rgSCHDlfsUpdAprdMode30SbCqi);

   if (!dlfsUe->aprdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   for (sbIdx = 0; sbIdx < mode30CqiInfo->numSubband; ++sbIdx)
   {
      sbCqiInfo = &(mode30CqiInfo->subbandArr[sbIdx]);
      startIdx = ((sbCqiInfo->subBand.rbStart)/dlfsCell->rbgSize);
      remRbs = sbCqiInfo->subBand.numRb;
      sbCqi = sbCqiInfo->cqi;
      while (remRbs >= dlfsCell->rbgSize)
      {
         dlfsUe->sbCqiLst[startIdx].cqi = sbCqi;         
         remRbs -= dlfsCell->rbgSize;
         startIdx += 1; 
      }
      /* Updation for the last small-sized sub-band */
      if (remRbs)
      {
         dlfsUe->sbCqiLst[startIdx].cqi = sbCqi;         
      }
   }

   /* Arrange the sub-bands of the UE in descending order of CQI */
   rgSCHDlfsSortSbCqiLst(dlfsUe);

   /* Calculate sub-band CQI deviation from average for the UE */
   rgSCHDlfsCalcSbCqiDeviation(dlfsUe);

   RETVOID;
}

/**
 * @brief Updates the periodic CQI sub-band report for Mode 2-0
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdMode20SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-0.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *     - Update only the sub-bands of the reported bandwidth part.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPucchMode20  *mode20CqiInfo
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdPrdMode20SbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode20  *mode20CqiInfo,
CmLteTimingInfo    timingInfo
)
#else
PRIVATE Void rgSCHDlfsUpdPrdMode20SbCqi(dlfsCell, dlfsUe, mode20CqiInfo, timingInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPucchMode20  *mode20CqiInfo;
CmLteTimingInfo    timingInfo;
#endif
{
   U8              sbLabel;
   U8              bpSbIdx;
   U8              newRptIdx;
   U8              oldRptIdx;
   U8              tmpIdx;
   U8              sbCnt;
   U8              numDlfsSbPerPrdSb = (dlfsUe->prdSbSize/dlfsCell->rbgSize);
   U8              sbCqi             = mode20CqiInfo->u.cqi.u.subCqi.cqi;
   U8              rcvdBpIdx;
   RgSchDlfsUeBpCb *bpCb; 
#ifndef NO_ERRCLS
   S16             ret = ROK;
#endif   
   TRC2(rgSCHDlfsUpdPrdMode20SbCqi);

   if (!dlfsUe->prdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

#if (ERRCLASS & ERRCLS_DEBUG)
   /* Determine the BP index for which the report is recieved */
   ret = rgSCHDlfsCalcBpIdx(timingInfo, dlfsUe, &rcvdBpIdx);
   
   if ( ret != ROK)
   {
      /* Report recieved at wrong timing */
      RETVOID;
   }
#else 
   /* Determine the BP index for which the report is recieved */
   rgSCHDlfsCalcBpIdx(timingInfo, dlfsUe, &rcvdBpIdx);
#endif 

   sbLabel = mode20CqiInfo->u.cqi.u.subCqi.l;
   /* Update the value for reported BP */
   bpCb = &(dlfsUe->bpCbs[rcvdBpIdx]);

   newRptIdx = bpCb->sbStartIdx + (sbLabel * numDlfsSbPerPrdSb);

   if (newRptIdx > bpCb->sbEndIdx)
   {
      RETVOID;
   }

   /* If reported value is less than wideband CQI, 
    * update all the sub-bands of the BP with new CQI */
   if (sbCqi < dlfsUe->wbCqi)
   {
      for (bpSbIdx = bpCb->sbStartIdx; bpSbIdx <= bpCb->sbEndIdx; ++bpSbIdx)
      {
         dlfsUe->sbCqiLst[bpSbIdx].cqi = sbCqi;
      }
   }
   else
   {
      /* Overwrite the previously reported values with wideband CQI */
      if (bpCb->rptCqi != RG_SCH_DLFS_INVALID_CQI)
      {
         oldRptIdx = bpCb->rptSbIdx;
         for (sbCnt = 0; (sbCnt < numDlfsSbPerPrdSb) && 
               (oldRptIdx <= dlfsUe->numSbCqi - 1); ++sbCnt)
         {
            dlfsUe->sbCqiLst[oldRptIdx++].cqi = dlfsUe->wbCqi;

         }
      }
      /* Update the new reported periodic sub-band CQI with the new value */
      tmpIdx = newRptIdx;
      for (sbCnt = 0; (sbCnt < numDlfsSbPerPrdSb) && 
            (tmpIdx <= dlfsUe->numSbCqi - 1); ++sbCnt)
      {
         dlfsUe->sbCqiLst[tmpIdx++].cqi = sbCqi;
      }
   }

   /* Update reported values in bpCb */
   bpCb->rptSbIdx = newRptIdx;
   bpCb->rptCqi = sbCqi;


   /* Arrange the sub-bands of the UE in descending order of CQI */
   rgSCHDlfsSortSbCqiLst(dlfsUe);

   /* Calculate sub-band CQI deviation from average for the UE */
   rgSCHDlfsCalcSbCqiDeviation(dlfsUe);

   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
 * @brief Updates the periodic CQI sub-band report for Mode 2-1
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdMode21SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-1.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *     - Update only the sub-bands of the reported bandwidth part.
 *     - In case of 2 CW CQI reporting. Consider the average of both CQIs
 *       as sbCqi.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPucchMode21  *mode21CqiInfo
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdPrdMode21SbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode21  *mode21CqiInfo,
CmLteTimingInfo    timingInfo
)
#else
PRIVATE Void rgSCHDlfsUpdPrdMode21SbCqi(dlfsCell, dlfsUe, mode21CqiInfo, timingInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPucchMode21  *mode21CqiInfo;
CmLteTimingInfo    timingInfo;
#endif
{
   U8              sbLabel;
   U8              bpSbIdx;
   U8              newRptIdx;
   U8              oldRptIdx;
   U8              tmpIdx;
   U8              sbCnt;
   U8              numDlfsSbPerPrdSb = (dlfsUe->prdSbSize/dlfsCell->rbgSize);
   U8              sbCqiCw1          = mode21CqiInfo->u.cqi.u.subCqi.cqi;
   U8              sbCqiCw2, avgSbCqi = sbCqiCw1;
   U8              rcvdBpIdx;
   RgSchDlfsUeBpCb *bpCb; 
#ifndef NO_ERRCLS
   S16             ret = ROK;
#endif
   if (!dlfsUe->prdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   if (mode21CqiInfo->u.cqi.u.subCqi.diffCqi.pres)
   {
      sbCqiCw2 = sbCqiCw1 - rgSchCmnDlCqiDiffOfst[\
                            mode21CqiInfo->u.cqi.u.subCqi.diffCqi.val]; 
      avgSbCqi = (sbCqiCw1 + sbCqiCw2)/2;
   }
#if (ERRCLASS & ERRCLS_DEBUG)
   /* Determine the BP index for which the report is recieved */
   ret = rgSCHDlfsCalcBpIdx(timingInfo, dlfsUe, &rcvdBpIdx);
   
   if ( ret != ROK)
   {
      /* Report recieved at wrong timing */
      RETVOID;
   }
#else 
   /* Determine the BP index for which the report is recieved */
   rgSCHDlfsCalcBpIdx(timingInfo, dlfsUe, &rcvdBpIdx);
#endif 

   sbLabel = mode21CqiInfo->u.cqi.u.subCqi.l;
   /* Update the value for reported BP */
   bpCb = &(dlfsUe->bpCbs[rcvdBpIdx]);

   newRptIdx = bpCb->sbStartIdx + (sbLabel * numDlfsSbPerPrdSb);

   if (newRptIdx > bpCb->sbEndIdx)
   {
      RETVOID;
   }

   /* If reported value is less than wideband CQI, 
    * update all the sub-bands of the BP with new CQI */
   if (avgSbCqi < dlfsUe->wbCqi)
   {
      for (bpSbIdx = bpCb->sbStartIdx; bpSbIdx <= bpCb->sbEndIdx; ++bpSbIdx)
      {
         dlfsUe->sbCqiLst[bpSbIdx].cqi = avgSbCqi;
      }
   }
   else
   {
      /* Overwrite the previously reported values with wideband CQI */
      if (bpCb->rptCqi != RG_SCH_DLFS_INVALID_CQI)
      {
         oldRptIdx = bpCb->rptSbIdx;
         for (sbCnt = 0; (sbCnt < numDlfsSbPerPrdSb) && 
               (oldRptIdx <= dlfsUe->numSbCqi - 1); ++sbCnt)
         {
            dlfsUe->sbCqiLst[oldRptIdx++].cqi = dlfsUe->wbCqi;

         }
      }
      /* Update the new reported periodic sub-band CQI with the new value */
      tmpIdx = newRptIdx;
      for (sbCnt = 0; (sbCnt < numDlfsSbPerPrdSb) && 
            (tmpIdx <= dlfsUe->numSbCqi - 1); ++sbCnt)
      {
         dlfsUe->sbCqiLst[tmpIdx++].cqi = avgSbCqi;
      }
   }

   /* Update reported values in bpCb */
   bpCb->rptSbIdx = newRptIdx;
   bpCb->rptCqi = avgSbCqi;


   /* Arrange the sub-bands of the UE in descending order of CQI */
   rgSCHDlfsSortSbCqiLst(dlfsUe);

   /* Calculate sub-band CQI deviation from average for the UE */
   rgSCHDlfsCalcSbCqiDeviation(dlfsUe);

   RETVOID;
}

/**
 * @brief Updates the periodic CQI wideband report for Mode 2-0
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdMode20WbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-0.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *     - Update only the sub-bands of the reported bandwidth part.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPucchMode20  *mode20CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdPrdMode20WbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode20  *mode20CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdPrdMode20WbCqi(dlfsCell, dlfsUe, mode20CqiInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPucchMode20  *mode20CqiInfo;
#endif
{
   U8              sbIdx;
   U8              bpIdx;
   U8              sbCnt;
   U8              wbCqi  = mode20CqiInfo->u.cqi.u.wideCqi;
   U8              sortSb = FALSE;
   U8              numDlfsSbPerPrdSb = (dlfsUe->prdSbSize/dlfsCell->rbgSize); 
   RgSchDlfsUeBpCb *bpCb  = NULLP;
   TRC2(rgSCHDlfsUpdPrdMode20WbCqi);

   /* Update wideband CQI for all the bands */
   rgSCHDlfsUpdWbCqi(dlfsUe, wbCqi);

   for (bpIdx = 0; bpIdx < dlfsUe->numBp; ++bpIdx)
   {
      bpCb = &(dlfsUe->bpCbs[bpIdx]);
      /* If previously reported CQI is lower than reported wbCqi, 
       * update the reported CQI value of BP else restore the old value */
      if (bpCb->rptCqi != RG_SCH_DLFS_INVALID_CQI)
      {
         if (bpCb->rptCqi < dlfsUe->wbCqi)
         {
            bpCb->rptCqi = wbCqi;
         }
         else
         {
            sortSb = TRUE;
            sbIdx = bpCb->rptSbIdx;
            for (sbCnt = 0; (sbCnt < numDlfsSbPerPrdSb) && 
                  (sbIdx <= dlfsUe->numSbCqi - 1); ++sbCnt)
            {
               dlfsUe->sbCqiLst[sbIdx++].cqi = bpCb->rptCqi;
            }
         }
      }
   }

   if (sortSb)
   {
      /* Arrange the sub-bands of the UE in descending order of CQI */
      rgSCHDlfsSortSbCqiLst(dlfsUe);

      /* Calculate sub-band CQI deviation from average for the UE */
      rgSCHDlfsCalcSbCqiDeviation(dlfsUe);
   }

   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
 * @brief Updates the periodic CQI wideband report for Mode 2-1
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdMode21WbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-1.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *     - Update only the sub-bands of the reported bandwidth part.
 *     - In case of 2 CW CQI reporting. Consider the average of both CQIs
 *       as wbCqi.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPucchMode21  *mode21CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdPrdMode21WbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode21  *mode21CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdPrdMode21WbCqi(dlfsCell, dlfsUe, mode21CqiInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPucchMode21  *mode21CqiInfo;
#endif
{
   U8              sbIdx;
   U8              bpIdx;
   U8              sbCnt;
   U8              cw1Cqi = mode21CqiInfo->u.cqi.u.wideCqi.cqi;
   U8              cw2Cqi, avgCqi = cw1Cqi;
   U8              sortSb = FALSE;
   U8              numDlfsSbPerPrdSb = (dlfsUe->prdSbSize/dlfsCell->rbgSize); 
   RgSchDlfsUeBpCb *bpCb  = NULLP;

   if (mode21CqiInfo->u.cqi.u.wideCqi.diffCqi.pres)
   {
      cw2Cqi = cw1Cqi - rgSchCmnDlCqiDiffOfst[\
                         mode21CqiInfo->u.cqi.u.wideCqi.diffCqi.val]; 
      avgCqi = (cw1Cqi + cw2Cqi)/2;
   }
   /* Update wideband CQI for all the bands */
   rgSCHDlfsUpdWbCqi(dlfsUe, avgCqi);

   for (bpIdx = 0; bpIdx < dlfsUe->numBp; ++bpIdx)
   {
      bpCb = &(dlfsUe->bpCbs[bpIdx]);
      /* If previously reported CQI is lower than reported wbCqi, 
       * update the reported CQI value of BP else restore the old value */
      if (bpCb->rptCqi != RG_SCH_DLFS_INVALID_CQI)
      {
         if (bpCb->rptCqi < dlfsUe->wbCqi)
         {
            bpCb->rptCqi = avgCqi;
         }
         else
         {
            sortSb = TRUE;
            sbIdx = bpCb->rptSbIdx;
            for (sbCnt = 0; (sbCnt < numDlfsSbPerPrdSb) && 
                  (sbIdx <= dlfsUe->numSbCqi - 1); ++sbCnt)
            {
               dlfsUe->sbCqiLst[sbIdx++].cqi = bpCb->rptCqi;
            }
         }
      }
   }

   if (sortSb)
   {
      /* Arrange the sub-bands of the UE in descending order of CQI */
      rgSCHDlfsSortSbCqiLst(dlfsUe);

      /* Calculate sub-band CQI deviation from average for the UE */
      rgSCHDlfsCalcSbCqiDeviation(dlfsUe);
   }

   RETVOID;
}

/**
 * @brief Sort the UEs in descending order of average sub-band CQI deviation 
 *
 * @details
 *
 *     Function : rgSCHDlfsSortUeLst
 *
 *     Processing Steps:
 *      - Sorts the UEs to be scheduled in descending order of average sub-band
 *      CQI deviation and creates a new sorted list.
 *      - This function assumes sortedUeLst to be initialized appropriately.
 *
 *  @param[in]   CmLListCp  *usUeLst 
 *  @param[out]  CmLListCp  *sUeLst
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsSortUeLst 
(
CmLListCp        *usUeLst,
CmLListCp        *sUeLst
)
#else
PRIVATE Void rgSCHDlfsSortUeLst(usUeLst, sUeLst)
CmLListCp        *usUeLst;
CmLListCp        *sUeLst;
#endif
{
   RgSchDlfsUeCb *prvDlfsUe;
   RgSchDlfsUeCb *usDlfsUe;
   RgSchUeCb     *usUe;
   CmLList       *sLnkNode = NULLP;
   CmLList       *usLnkNode = NULLP;
   CmLList       *prvLnkNode = NULLP;
   RgSchDlHqProcCb *hqP = NULLP;
   TRC2(rgSCHDlfsSortUeLst);

   /* Initialize unsorted list pointer */
   usLnkNode = cmLListFirst(usUeLst);
   while (usLnkNode)
   {
      hqP = (RgSchDlHqProcCb *)usLnkNode->node;
      usUe = hqP->hqE->ue;
      usDlfsUe = RG_SCH_DLFS_GET_UE(usUe);

      /* Initialize the link pointer for UE Cb to be inserted */
      sLnkNode = &(hqP->schdLstLnk);
      RG_SCH_CMN_INIT_SCHD_LNK(sLnkNode, hqP);

      /* Initialize sorted list pointer */
      prvLnkNode = cmLListFirst(sUeLst);

      /* Insert the UE in already sorted list */
      while ((prvLnkNode) &&
            ((prvDlfsUe = RG_SCH_DLFS_GET_UE(((RgSchDlHqProcCb*)prvLnkNode->node)->hqE->ue)) != NULLP) &&
            (usDlfsUe->avgCqiDeviation < prvDlfsUe->avgCqiDeviation))
      {
         prvLnkNode = prvLnkNode->next;
      }
      if (!prvLnkNode)
      {
         cmLListAdd2Tail(sUeLst, sLnkNode);
      }
      else
      {
         sUeLst->crnt = prvLnkNode;
         cmLListInsCrnt(sUeLst, sLnkNode);
      }
      usLnkNode = usLnkNode->next;
   }

   RETVOID;
}

/**
 * @brief  RB allocation for the UE
 *
 * @details
 *
 *     Function : rgSCHDlfsAllocSb
 *
 *     Processing Steps:
 *      - RB allocation for the UE from the hole
 *
 *  @param[in]   RgSchDlfsSf     *dlfsSf
 *  @param[in]   RgSchDlfsHole   *hole
 *  @param[out]  U8              *allocIdx 
 *  @param[out]  U8              *numAllocRbs 
 *  
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsAllocSb
(
RgSchDlfsSf     *dlfsSf,
RgSchDlfsHole   *hole,
U8              *allocIdx,
U8              *numAllocRbs
)
#else
PRIVATE Void rgSCHDlfsAllocSb(dlfsSf, hole, allocIdx, numAllocRbs)
RgSchDlfsSf     *dlfsSf;
RgSchDlfsHole   *hole;
U8              *allocIdx;
U8              *numAllocRbs;
#endif
{
   TRC2(rgSCHDlfsAllocSb);

   rgSCHDlfsGetAllocFrmHole(hole, allocIdx, numAllocRbs);
   rgSCHDlfsRlsHole(dlfsSf, hole);
   RETVOID;
}

/**
 * @brief  RB allocation for the UE for wideband CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsWbAllocForTxUe
 *
 *     Processing Steps:
 *     - Traverse through the list of holes and allocate to the UE.
 *     - If allocation is successful,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *       - Determine imcs
 *       - Determine Resource allocation type
 *       - Determine DCI format
 *     
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsWbAllocForTxUe
(
RgSchDlSf          *dlSf,
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlRbAlloc     *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsWbAllocForTxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf          *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U8              remRbsReq      = 0;
   U8              rbAlloc        = 0;
   U8              numSbAlloc     = 0;
   U8              totAllocRbs    = 0;
   U8              allocIdx       = 0;
   U32             dlAllocBitMask = 0;
   RgSchDlfsSb     *dlSb          = NULLP;
   RgSchDlfsHole   *hole          = NULLP;
   CmLList         *holeLnk       = NULLP;
   RgSchDlfsCellCb *dlfsCell      = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb   *dlfsUe        = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsSf     *dlfsSf        = RG_SCH_DLFS_GET_SF(dlSf);
   TRC2(rgSCHDlfsWbAllocForTxUe);

   /* Note: It is assumed that the avgCqi = wbCqi when avgCqiDeviation 
    * = 0. Therefore, number of RBs computed by invoking scheduler shall be
    * targeted for assignment */
   holeLnk = dlfsSf->holeLst.first;
   remRbsReq = dlRbAlloc->rbsReq;
   while (remRbsReq && holeLnk && (numSbAlloc < dlfsCell->maxDlSbPerUe))
   {
      hole = (RgSchDlfsHole *)(holeLnk->node);
      holeLnk = holeLnk->next;
      dlSb = hole;
      if (numSbAlloc)
      {
         if(remRbsReq > (dlSb->sbSize/2))
         {
            /* Allocate the sub-band only if rbReq for UE exceeds half the
             * size of the sub-band: we dont want the entire sub-band to be
             * allocated if the UE requirement is less than/equal to 
             * half of the sub-band */
            rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
            totAllocRbs += rbAlloc; 
            dlAllocBitMask |= 1 << (31 - allocIdx); 
            numSbAlloc++;
            remRbsReq = (remRbsReq > rbAlloc) ? 
               (remRbsReq - rbAlloc):0;
            continue;
         }
         break;
      }

      /* First allocation for the UE */
      rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
      totAllocRbs += rbAlloc; 
      dlAllocBitMask |= 1 << (31 - allocIdx); 
      numSbAlloc++;
      remRbsReq = (remRbsReq > rbAlloc) ? 
         (remRbsReq - rbAlloc):0;
   }


   /* Update allocation Info */
   if (totAllocRbs) 
   {
      U32      y;

      /* Take floor of the computed average */
      dlfsUe->imcsCqi = dlfsUe->avgSbCqi;

      /* Retrieve the PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* rg004.201 patch - ccpu00110457 DTX Changes Start */
      /* DTX Change : One Variable is passed to check DTX or not */
      dlRbAlloc->pdcch = 
         rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi, \
               dlRbAlloc->dciFormat, FALSE);
      if (!(dlRbAlloc->pdcch))
      {
         rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
         RETVOID;
      }
      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

      /* Allocated RB info */
      dlRbAlloc->rbsAlloc = totAllocRbs;

      /* Determine Imcs */
      /* Note: It is possible that this computation is done in invoking
       * scheduler. If needed, we can use rbsReq for allocation and maintain the
       * Imcs as it is, assuming we should be always able to satisfy the
       * wideband requirement of a UE */
      rgSCHDlfsFillImcs(cell, dlfsUe, dlRbAlloc, RG_SCH_DLFS_RETX_FALSE);

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->allocInfo.raType0.numDlAlloc += numSbAlloc; 
      dlRbAlloc->allocInfo.raType0.dlAllocBitMask |= dlAllocBitMask;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
   }

   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */

/***********************************************************
 *
 *     Func : rgSCHDlfsAreHolesAdjcnt
 *
 *     Desc : Returns TRUE if hole and nxtHole are adjacent 
 *            to each other else returns FALSE.
 *
 *     Ret  : Bool 
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/

#ifdef ANSI
PRIVATE Bool rgSCHDlfsAreHolesAdjcnt
(
RgSchDlfsHole   *hole,
RgSchDlfsHole   *nxtHole
)
#else
PRIVATE Bool rgSCHDlfsAreHolesAdjcnt(hole, nxtHole)
RgSchDlfsHole   *hole;
RgSchDlfsHole   *nxtHole;
#endif
{
   TRC2(rgSCHDlfsAreHolesAdjcnt)

   if ((nxtHole->start) == (hole->start + hole->sbSize))
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}
/**
 * @brief  Type 2 Localized RB allocation for the UE for wideband CQI 
 *
 * @details
 *
 *     Function :rgSCHDlfsTyp2AllocForTxUe
 *
 *     Processing Steps:
 *     - Traverse through the list of holes and allocate to the 
 *       the set of holes consecutive to each other limited by the
 *       RBs requested by UE.
 *     - If allocation is successful,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *       - Determine imcs
 *       - Determine Resource allocation type
 *       - Determine DCI format
 *     
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsTyp2AllocForTxUe
(
RgSchDlSf          *dlSf,
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlRbAlloc     *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsTyp2AllocForTxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf          *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U8              remRbsReq      = 0;
   U8              rbAlloc        = 0;
   U8              numSbAlloc     = 0;
   U8              totAllocRbs    = 0;
   U8              allocIdx       = 0;
   U32             dlAllocBitMask = 0;
   RgSchDlfsHole   *hole          = NULLP;
   RgSchDlfsHole   *nxtHole       = NULLP;
   CmLList         *holeLnk       = NULLP;
   RgSchDlfsCellCb *dlfsCell      = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb   *dlfsUe        = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsSf     *dlfsSf        = RG_SCH_DLFS_GET_SF(dlSf);
   TRC2(rgSCHDlfsTyp2AllocForTxUe);

   /* Note: It is assumed that the avgCqi = wbCqi when avgCqiDeviation 
    * = 0. Therefore, number of RBs computed by invoking scheduler shall be
    * targeted for assignment */
   holeLnk = dlfsSf->holeLst.first;
   remRbsReq = dlRbAlloc->rbsReq;
   while (remRbsReq && (numSbAlloc < dlfsCell->maxDlSbPerUe))
   {
      hole = (RgSchDlfsHole *)(holeLnk->node);
      holeLnk = holeLnk->next;
      /* Allocate the sub-band only if rbReq for UE exceeds half the
       * size of the sub-band: we dont want the entire sub-band to be
       * allocated if the UE requirement is less than/equal to 
       * half of the sub-band */
      rgSCHDlfsAllocSb (dlfsSf, hole, &allocIdx, &rbAlloc);
      totAllocRbs += rbAlloc; 
      dlAllocBitMask |= 1 << (31 - allocIdx); 
      numSbAlloc++;
      remRbsReq = (remRbsReq > rbAlloc) ? 
         (remRbsReq - rbAlloc):0;

      if (holeLnk)
      {
         nxtHole = (RgSchDlfsHole *)(holeLnk->node);
         if (rgSCHDlfsAreHolesAdjcnt(hole, nxtHole) == FALSE)
         {
            /* If hole and nxtHole are not adjacent to each other
             * then stop allocating further */
            break;
         }
      }
      else
      {
         break;
      }
   }

   /* Update allocation Info */
   if (totAllocRbs) 
   {
      U32      y;

      /* Take floor of the computed average */
      dlfsUe->imcsCqi = dlfsUe->avgSbCqi;

      /* Retrieve the PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* rg004.201 patch - ccpu00110457 DTX Changes Start */
      /* DTX Change : One Variable is passed to check DTX or not */
      if (NULLP == (dlRbAlloc->pdcch = 
               rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi, \
                        dlRbAlloc->dciFormat, FALSE)))
      {
         rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
         RETVOID;
      }
      /* Allocated RB info */
      dlRbAlloc->rbsAlloc = totAllocRbs;

      /* Determine Imcs */
      /* Note: It is possible that this computation is done in invoking
       * scheduler. If needed, we can use rbsReq for allocation and maintain the
       * Imcs as it is, assuming we should be always able to satisfy the
       * wideband requirement of a UE */
      rgSCHDlfsFillImcs(cell, dlfsUe, dlRbAlloc, RG_SCH_DLFS_RETX_FALSE);

      dlRbAlloc->allocInfo.raType2.isLocal = TRUE;
      dlRbAlloc->allocInfo.raType2.rbStart = (U8)dlSf->bwAlloced;
      dlRbAlloc->allocInfo.raType2.numRb = totAllocRbs;

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;
   }

   RETVOID;
}

/**
 * @brief  Type 2 Localized RB allocation for the Retx UE for wideband CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsTyp2AllocForRetxUe
 *
 *     Processing Steps:
 *     - Traverse through the list of holes and allocate to the UE.
 *     - If the tbSize cannot be satisfied, free the allocation.
 *     - If allocation is successful,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *       - Determine Resource allocation type
 *       - Determine DCI format
 *     
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsTyp2AllocForRetxUe
(
RgSchDlSf           *dlSf,
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlRbAlloc      *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsTyp2AllocForRetxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf        *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U8              remRbsReq      = 0;
   U8              rbAlloc        = 0;
   U8              numSbAlloc     = 0;
   U8              totAllocRbs    = 0;
   U8              allocIdx       = 0;
   U32             dlAllocBitMask = 0;
   RgSchDlfsHole   *hole          = NULLP;
   RgSchDlfsHole   *nxtHole       = NULLP;
   CmLList         *holeLnk       = NULLP;
   RgSchDlfsCellCb *dlfsCell      = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb   *dlfsUe        = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsSf     *dlfsSf        = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchCmnDlUe    *dlUe;
   TRC2(rgSCHDlfsTyp2AllocForRetxUe);

   /* Note: It is assumed that the avgCqi = wbCqi when avgCqiDeviation 
    * = 0. Therefore, number of RBs computed by invoking scheduler shall be
    * targeted for assignment */

   remRbsReq = dlRbAlloc->rbsReq;

   holeLnk = dlfsSf->holeLst.first;

   while (remRbsReq && (numSbAlloc < dlfsCell->maxDlSbPerUe))
   {
      hole = (RgSchDlfsHole *)(holeLnk->node);
      holeLnk = holeLnk->next;
      rgSCHDlfsAllocSb (dlfsSf, hole, &allocIdx, &rbAlloc);
      totAllocRbs += rbAlloc; 
      dlAllocBitMask |= 1 << (31 - allocIdx); 
      numSbAlloc++;
      remRbsReq = (remRbsReq > rbAlloc) ? 
         (remRbsReq - rbAlloc):0;

      if (holeLnk)
      {
         nxtHole = (RgSchDlfsHole *)(holeLnk->node);
         if (rgSCHDlfsAreHolesAdjcnt(hole, nxtHole) == FALSE)
         {
            /* If hole and nxtHole are not adjacent to each other
             * then stop allocating further */
            break;
         }
      }
      else
      {
         break;
      }
   }

   /* If entire RB requirement cannot be satisfied, add allocated holes
    * back to the list */
   if (remRbsReq && dlAllocBitMask)
   {
      rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
      RETVOID;
   }

   /* Update allocation Info */
   if (totAllocRbs) 
   {
      U32       y;

      /* Take floor of the computed average */
      dlfsUe->imcsCqi = dlfsUe->avgSbCqi;

      /* Allocate PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* rg004.201 patch - ccpu00110457 DTX Changes Start */
      /* DTX Change : One Variable is passed to check DTX or not */
      dlUe = RG_SCH_CMN_GET_DL_UE(ue);
      if ((dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX) || (dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX))
      {
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
                                   dlRbAlloc->dciFormat, TRUE);
      }
      else{
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
                                   dlRbAlloc->dciFormat, FALSE);
      }
      if (!(dlRbAlloc->pdcch ))
      {
         rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
         RETVOID;
      }

      /* Allocated RB info */
      dlRbAlloc->rbsAlloc = totAllocRbs;
      dlRbAlloc->tbInfo[0].bytesAlloc = dlRbAlloc->tbInfo[0].bytesReq;
      dlRbAlloc->tbInfo[1].bytesAlloc = dlRbAlloc->tbInfo[1].bytesReq;

      /* Determination of  Imcs for Retx UE for imcsCqi */
      rgSCHDlfsFillImcs(cell, dlfsUe, dlRbAlloc, RG_SCH_DLFS_RETX_TRUE);

      dlRbAlloc->allocInfo.raType2.isLocal = TRUE;
      dlRbAlloc->allocInfo.raType2.rbStart = (U8)dlSf->bwAlloced;
      dlRbAlloc->allocInfo.raType2.numRb = totAllocRbs;

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

   }

   RETVOID;
} /* rgSCHDlfsTyp2AllocForRetxUe */

/**
 * @brief  RB allocation for the Retx UE for wideband CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsWbAllocForRetxUe
 *
 *     Processing Steps:
 *     - Traverse through the list of holes and allocate to the UE.
 *     - If the tbSize cannot be satisfied, free the allocation.
 *     - If allocation is successful,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *       - Determine Resource allocation type
 *       - Determine DCI format
 *     
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsWbAllocForRetxUe
(
RgSchDlSf           *dlSf,
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlRbAlloc      *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsWbAllocForRetxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf        *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U8              remRbsReq      = 0;
   U8              rbAlloc        = 0;
   U8              numSbAlloc     = 0;
   U8              totAllocRbs    = 0;
   U8              allocIdx       = 0;
   U32             dlAllocBitMask = 0;
   RgSchDlfsSb     *dlSb          = NULLP;
   RgSchDlfsHole   *hole          = NULLP;
   CmLList         *holeLnk       = NULLP;
   RgSchDlfsCellCb *dlfsCell      = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb   *dlfsUe        = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsSf     *dlfsSf        = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchCmnDlUe    *dlUe;
   TRC2(rgSCHDlfsWbAllocForRetxUe);

   /* Note: It is assumed that the avgCqi = wbCqi when avgCqiDeviation 
    * = 0. Therefore, number of RBs computed by invoking scheduler shall be
    * targeted for assignment */

   remRbsReq = dlRbAlloc->rbsReq;

   holeLnk = dlfsSf->holeLst.first;

   while (remRbsReq && holeLnk && (numSbAlloc < dlfsCell->maxDlSbPerUe))
   {
      hole = (RgSchDlfsHole *)(holeLnk->node);
      holeLnk = holeLnk->next;
      dlSb = hole;
      rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
      totAllocRbs += rbAlloc; 
      dlAllocBitMask |= 1 << (31 - allocIdx); 
      numSbAlloc++;
      remRbsReq = (remRbsReq > rbAlloc) ? 
         (remRbsReq - rbAlloc):0;
   }

   /* If entire RB requirement cannot be satisfied, add allocated holes
    * back to the list */
   if (remRbsReq && dlAllocBitMask)
   {
      rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
      RETVOID;
   }

   /* Update allocation Info */
   if (totAllocRbs) 
   {
      U32       y;

      /* Take floor of the computed average */
      dlfsUe->imcsCqi = dlfsUe->avgSbCqi;

      /* Allocate PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* rg004.201 patch - ccpu00110457 DTX Changes Start */
      /* DTX Change : One Variable is passed to check DTX or not */
      dlUe = RG_SCH_CMN_GET_DL_UE(ue);
      if ((dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX) || (dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX))
      {
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
               dlRbAlloc->dciFormat, TRUE);
      }
      else{
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
               dlRbAlloc->dciFormat, FALSE);
      }
      if (!(dlRbAlloc->pdcch))
      {
         rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
         RETVOID;
      }

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

      /* Allocated RB info */
      dlRbAlloc->rbsAlloc = totAllocRbs;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->tbInfo[0].bytesAlloc = dlRbAlloc->tbInfo[0].bytesReq;
      dlRbAlloc->tbInfo[1].bytesAlloc = dlRbAlloc->tbInfo[1].bytesReq;

      /* Determination of  Imcs for Retx UE for imcsCqi */
      rgSCHDlfsFillImcs(cell, dlfsUe, dlRbAlloc, RG_SCH_DLFS_RETX_TRUE);

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->allocInfo.raType0.numDlAlloc += numSbAlloc; 
      dlRbAlloc->allocInfo.raType0.dlAllocBitMask |= dlAllocBitMask;

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
   }

   RETVOID;
} /* rgSCHDlfsWbAllocForRetxUe */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
 * @brief  RB allocation for the RETX+TX UE for wideband CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsWbAllocForTxRetxUe
 *
 *     Processing Steps:
 *     - Traverse through the list of holes and allocate to the UE.
 *     - If the tbSize cannot be satisfied, free the allocation.
 *     - If allocation is successful,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *       - Determine Resource allocation type
 *       - Determine DCI format
 *     
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsWbAllocForTxRetxUe
(
RgSchDlSf           *dlSf,
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlRbAlloc      *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsWbAllocForTxRetxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf        *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U8              remRbsReq      = 0;
   U8              rbAlloc        = 0;
   U8              numSbAlloc     = 0;
   U8              totAllocRbs    = 0;
   U8              allocIdx       = 0;
   U32             dlAllocBitMask = 0;
   RgSchDlfsSb     *dlSb          = NULLP;
   RgSchDlfsHole   *hole          = NULLP;
   CmLList         *holeLnk       = NULLP;
   RgSchDlfsCellCb *dlfsCell      = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb   *dlfsUe        = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsSf     *dlfsSf        = RG_SCH_DLFS_GET_SF(dlSf);
   U8              iTbs2;
   U8              noLyr2         = dlRbAlloc->tbInfo[1].noLyr;
   U8              mod;
   RgSchCmnCell    *cmnCell = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlUe    *dlUe;

   TRC2(rgSCHDlfsWbAllocForTxRetxUe);

   /* Note: It is assumed that the avgCqi = wbCqi when avgCqiDeviation 
    * = 0. Therefore, number of RBs computed by invoking scheduler shall be
    * targeted for assignment */

   remRbsReq = dlRbAlloc->rbsReq;

   holeLnk = dlfsSf->holeLst.first;

   while (remRbsReq && holeLnk && (numSbAlloc < dlfsCell->maxDlSbPerUe))
   {
      hole = (RgSchDlfsHole *)(holeLnk->node);
      holeLnk = holeLnk->next;
      dlSb = hole;
      rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
      totAllocRbs += rbAlloc; 
      dlAllocBitMask |= 1 << (31 - allocIdx); 
      numSbAlloc++;
      remRbsReq = (remRbsReq > rbAlloc) ? 
         (remRbsReq - rbAlloc):0;
   }

   /* If entire RB requirement cannot be satisfied, add allocated holes
    * back to the list */
   if (remRbsReq && dlAllocBitMask)
   {
      rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
      RETVOID;
   }

   /* Update allocation Info */
   if (totAllocRbs) 
   {
      U32       y;

      /* Take floor of the computed average */
      dlfsUe->imcsCqi = dlfsUe->avgSbCqi;

      /* Allocate PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* rg004.201 patch - ccpu00110457 DTX Changes Start */
      /* DTX Change : One Variable is passed to check DTX or not */
      dlUe = RG_SCH_CMN_GET_DL_UE(ue);
      if (dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX)
      {
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
                                   dlRbAlloc->dciFormat, TRUE);
      }
      else{
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
                                   dlRbAlloc->dciFormat, FALSE);
      }
      if (!(dlRbAlloc->pdcch))
      {
         rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
         RETVOID;
      }

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

      /* Allocated RB info */
      dlRbAlloc->rbsAlloc = totAllocRbs;
      dlRbAlloc->tbInfo[0].bytesAlloc = dlRbAlloc->tbInfo[0].bytesReq;
      dlRbAlloc->tbInfo[1].bytesAlloc = dlRbAlloc->tbInfo[1].bytesReq;

      /* Update imcs for re-transmission in range: 29-31 */
      RG_SCH_DLFS_CQI_TO_MODODR(dlfsUe->imcsCqi, &mod);
      RG_SCH_DLFS_MODODR_TO_RETX_MCS(mod, &dlRbAlloc->tbInfo[0].imcs);

      if ((dlRbAlloc->tbInfo[1].schdlngForTb) && (dlRbAlloc->rbsAlloc != 0))
      {
         iTbs2 = (*(RgSchCmnCqiToTbs *)(cmnCell->dl.cqiToTbsTbl[noLyr2 - 1]))[dlfsUe->imcsCqi];
         RG_SCH_CMN_TBS_TO_MCS(iTbs2, dlRbAlloc->tbInfo[1].imcs);
         dlRbAlloc->tbInfo[1].bytesAlloc = 
            rgTbSzTbl[noLyr2 - 1][iTbs2][dlRbAlloc->rbsAlloc - 1]/8;
      }

      dlRbAlloc->allocInfo.raType0.numDlAlloc += numSbAlloc; 
      dlRbAlloc->allocInfo.raType0.dlAllocBitMask |= dlAllocBitMask;
   }

   RETVOID;
}


/**
 * @brief  RB allocation for the UE based on sub-band CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsSbAllocForTxUe
 *
 *     Processing Steps:
 *     - Traverse through the list of sub-band CQIs. For each sub-band,
 *       - If available, allocate to the UE.
 *       - else, go to the next sub-band.
 *     - If allocation is successful,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *       - Determine imcs
 *       - Determine Resource allocation type
 *       - Determine DCI format
 *
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsSbAllocForTxUe
(
RgSchDlSf          *dlSf,
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlRbAlloc     *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsSbAllocForTxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf          *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U32             remBytesReq;
   U32             bytesAlloc;
   U8              allocIdx;
   U8              rbAlloc;
   U8              numSbAlloc     = 0;
   U8              totAllocRbs    = 0;
   U32             totAllocBytes  = 0;
   U32             imcsCqi        = 0;
   U32             dlAllocBitMask = 0;
   RgSchDlfsSbCqi  *sbCqiPtr;
   RgSchDlfsSb     *dlSb;
   RgSchDlfsCellCb *dlfsCell      = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb   *dlfsUe        = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsSf     *dlfsSf        = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchCmnCell    *cmnCell       = RG_SCH_CMN_GET_CELL(cell);
   U8              iTbs;
   TRC2(rgSCHDlfsSbAllocForTxUe);

   sbCqiPtr = dlfsUe->sortSbCqiLst;
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   remBytesReq = dlRbAlloc->tbInfo[0].bytesReq;

   while(remBytesReq && sbCqiPtr && (numSbAlloc < dlfsCell->maxDlSbPerUe))
   {
      if (sbCqiPtr->cqi >= dlfsCell->thresholdCqi)
      {
         dlSb = &(dlfsSf->dlSb[sbCqiPtr->sbIdx]);
         /* Note: Determining bytes that  can be satisfied by the sub-band size
          *  at reported CQI value. This method has inaccuracy especially for 
          *  low CQI, but works quite OK for high CQI. This solution helps in 
          *  reducing the CQI cycles. */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
         /* rg001.201:106531: Inappropriate name of CQI to ITbs table for DL. */
         iTbs = (*(RgSchCmnCqiToTbs *)(cmnCell->dl.cqiToTbsTbl[0]))[sbCqiPtr->cqi];
         bytesAlloc = (rgTbSzTbl[0][iTbs][dlSb->sbSize - 1])/8;
         if (numSbAlloc)
         {
            if(remBytesReq > (bytesAlloc/2))
            {
               /* Allocate the sub-band only if rbReq for UE exceeds half the
                * size of the sub-band: we dont want the entire sub-band to be
                * allocated if the UE requirement is less than/equal to 
                * half of the sub-band */
               if (dlSb->isAvail)
               {
                  /* Allocate the available sub-band */
                  rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
                  imcsCqi += (sbCqiPtr->cqi * rbAlloc);
                  totAllocRbs += rbAlloc; 
                  totAllocBytes += bytesAlloc;
                  dlAllocBitMask |= 1 << (31 - allocIdx); 
                  numSbAlloc++;
                  remBytesReq = (remBytesReq > bytesAlloc) ?
                     (remBytesReq - bytesAlloc) : 0;
               }
               sbCqiPtr = sbCqiPtr->nxtSbCqi;
               continue;
            }
            /* Since most of the sub-bands will be of equal size, no point
             * further assigning */
            break;
         }
         if (dlSb->isAvail)
         {
            /* Allocate the available sub-band */
            rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
            imcsCqi += (sbCqiPtr->cqi * rbAlloc);
            totAllocRbs += rbAlloc; 
            totAllocBytes += bytesAlloc;
            dlAllocBitMask |= 1 << (31 - allocIdx); 
            numSbAlloc++;
            remBytesReq = (remBytesReq > bytesAlloc) ?
               (remBytesReq - bytesAlloc) : 0;
         }
         sbCqiPtr = sbCqiPtr->nxtSbCqi;
         continue;
      }
      /* No more good sub-bands exist */
      break;
   }

   /* Update allocation Info */
   if (totAllocRbs) 
   {
      U32     y;

      /* Take floor of the computed average */
      dlfsUe->imcsCqi = (U8)(imcsCqi/totAllocRbs);

      /* Retrieve the PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->pdcch = 
         rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
               dlRbAlloc->dciFormat, FALSE);
      if (!(dlRbAlloc->pdcch)) 
      {
         rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
         RETVOID;
      }

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

      dlRbAlloc->rbsAlloc = totAllocRbs;

      /* Determine Imcs */
      rgSCHDlfsFillImcs(cell, dlfsUe, dlRbAlloc, RG_SCH_DLFS_RETX_FALSE);

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->allocInfo.raType0.numDlAlloc += numSbAlloc; 
      dlRbAlloc->allocInfo.raType0.dlAllocBitMask |= dlAllocBitMask;

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
   }

   RETVOID;
}

/**
 * @brief  RB allocation for the re-transmitting UE based on sub-band CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsSbAllocForRetxUe
 *
 *     Processing Steps:
 *     - Traverse through the list of sub-band CQIs. For each sub-band,
 *       - If available, allocate to the UE.
 *       - else, go to the next sub-band.
 *     - If the tbSize cannot be satisfied, free the allocation
 *     - else if allocation is successful,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *       - Determine Resource allocation type
 *       - Determine DCI format
 *     - MIMO: Satisfy TbInfo[0]'s request, and then try to see if TbInfo[1]'s
 *             request is satisfied eventually.
 *
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsSbAllocForRetxUe
(
RgSchDlSf          *dlSf,
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlRbAlloc     *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsSbAllocForRetxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf          *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U8              allocIdx;
   U8              numSbAlloc     = 0;
   U8              totAllocRbs    = 0;
   U8              rbAlloc        = 0;
   U32             tbSize;
   U32             remBytesReq;
   U32             bytesAlloc     = 0;
   U32             totAllocBytes  = 0;
   U32             imcsCqi        = 0;
   U32             dlAllocBitMask = 0;
   RgSchDlfsSbCqi  *sbCqiPtr;
   RgSchDlfsSb     *dlSb;
   RgSchDlfsCellCb *dlfsCell     = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb   *dlfsUe       = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsSf     *dlfsSf       = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchCmnCell    *cmnCell      = RG_SCH_CMN_GET_CELL(cell);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8              iTbs1, iTbs2;
   U8              noLyr1        = dlRbAlloc->tbInfo[0].noLyr;
   U8              noLyr2        = dlRbAlloc->tbInfo[1].noLyr;
   U32             totalAllocOthrTb = 0;
   /* rg007.201 - DTX Change */
   RgSchCmnDlUe    *dlUe; 

   TRC2(rgSCHDlfsSbAllocForRetxUe);


   sbCqiPtr = dlfsUe->sortSbCqiLst;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   remBytesReq = dlRbAlloc->tbInfo[0].bytesReq;
   tbSize      = dlRbAlloc->tbInfo[0].bytesReq;

   while(remBytesReq && sbCqiPtr && (numSbAlloc < dlfsCell->maxDlSbPerUe))
   {
      if (sbCqiPtr->cqi >= dlfsCell->thresholdCqi)
      {
         dlSb = &(dlfsSf->dlSb[sbCqiPtr->sbIdx]);
         if (dlSb->isAvail)
         {
            /* Note: Determining bytes that can be satisfied by the sub-band
             * size at reported CQI value. This method has inaccuracy 
             * especially for low CQI, but works quite OK for high CQI.
             * This solution helps in reducing the CQI cycles. */
            /* rg007.201 - Changes for MIMO feature addition */
            /* rg008.201 - Removed dependency on MIMO compile-time flag */
            /* rg001.201:106531: Inappropriate name of CQI to ITbs 
             * table for DL. */
            iTbs1 = (*(RgSchCmnCqiToTbs *)(cmnCell->dl.cqiToTbsTbl[noLyr1 - 1]))
               [sbCqiPtr->cqi];
            bytesAlloc = (rgTbSzTbl[noLyr1 - 1][iTbs1][dlSb->sbSize - 1])/8;
            if (dlRbAlloc->tbInfo[1].schdlngForTb)
            {
               iTbs2 = (*(RgSchCmnCqiToTbs *)(cmnCell->dl.cqiToTbsTbl[noLyr2 - 1]))
                  [sbCqiPtr->cqi];
               totalAllocOthrTb += (rgTbSzTbl[noLyr2 - 1][iTbs2][dlSb->sbSize - 1])/8;
            }

            /* Allocate the available sub-band */
            rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
            imcsCqi += (sbCqiPtr->cqi * rbAlloc);
            totAllocRbs += rbAlloc; 
            totAllocBytes += bytesAlloc;
            dlAllocBitMask |= 1 << (31 - allocIdx); 
            numSbAlloc++;
            remBytesReq = (remBytesReq > bytesAlloc) ? 
               (remBytesReq - bytesAlloc) : 0;
         }
         sbCqiPtr = sbCqiPtr->nxtSbCqi;
         continue;
      }
      /* No more good sub-bands exist */
      break;
   }

   /* If entire TB size cannot be satisfied, add allocated holes back to the
    * list */
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   if ((remBytesReq && dlAllocBitMask) 
         ||
         (dlRbAlloc->tbInfo[1].schdlngForTb && 
          (dlRbAlloc->tbInfo[1].bytesReq > totalAllocOthrTb))
      )
   {
      rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
      RETVOID;
   }

   /* Update allocation Info if the entire TB size could be satisfied */
   if (totAllocRbs) 
   {
      U32 y;

      /* Take floor of the computed average */
      dlfsUe->imcsCqi = imcsCqi/totAllocRbs;

      /* Retrieve the PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlUe = RG_SCH_CMN_GET_DL_UE(ue);
      if ((dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX) || \
            (dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX))
      {
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
               dlRbAlloc->dciFormat, TRUE);
      }
      else{
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
               dlRbAlloc->dciFormat, FALSE);
      }
      if (!(dlRbAlloc->pdcch))
      {
         rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
         RETVOID;
      }

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

      dlRbAlloc->rbsAlloc = totAllocRbs;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->tbInfo[0].bytesAlloc = tbSize; 
      dlRbAlloc->tbInfo[1].bytesAlloc = dlRbAlloc->tbInfo[1].bytesReq; 

      /* Determination of Imcs for Retx UE for imcsCqi */
      rgSCHDlfsFillImcs(cell, dlfsUe, dlRbAlloc, RG_SCH_DLFS_RETX_TRUE);

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->allocInfo.raType0.numDlAlloc += numSbAlloc; 
      dlRbAlloc->allocInfo.raType0.dlAllocBitMask |= dlAllocBitMask;

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
   }
   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
 * @brief  RB allocation for the RETX+TX UE based on sub-band CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsSbAllocForTxRetxUe
 *
 *     Processing Steps:
 *     - Traverse through the list of sub-band CQIs. For each sub-band,
 *       - If available, allocate to the UE.
 *       - else, go to the next sub-band.
 *     - If the tbSize cannot be satisfied, free the allocation
 *     - else if allocation is successful,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *       - Determine Resource allocation type
 *       - Determine DCI format
 *     - MIMO: Satisfy TbInfo[0]'s request, and then TbInfo[1]'s
 *             request is served as a result of tbInfo[0]'s allocation. 
 *
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsSbAllocForTxRetxUe
(
RgSchDlSf          *dlSf,
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlRbAlloc     *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsSbAllocForTxRetxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf          *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U8              allocIdx;
   U8              numSbAlloc     = 0;
   U8              totAllocRbs    = 0;
   U8              rbAlloc        = 0;
   U32             tbSize;
   U32             remBytesReq;
   U32             bytesAlloc     = 0;
   U32             totAllocBytes  = 0;
   U32             imcsCqi        = 0;
   U32             dlAllocBitMask = 0;
   RgSchDlfsSbCqi  *sbCqiPtr;
   RgSchDlfsSb     *dlSb;
   RgSchDlfsCellCb *dlfsCell     = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb   *dlfsUe       = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsSf     *dlfsSf       = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchCmnCell    *cmnCell      = RG_SCH_CMN_GET_CELL(cell);
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8              mod;
   U8              iTbs1, iTbs2;
   U8              noLyr1        = dlRbAlloc->tbInfo[0].noLyr;
   U8              noLyr2        = dlRbAlloc->tbInfo[1].noLyr;
   RgSchCmnDlUe    *dlUe;
   TRC2(rgSCHDlfsSbAllocForRetxUe);
   

   sbCqiPtr = dlfsUe->sortSbCqiLst;
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   remBytesReq = dlRbAlloc->tbInfo[0].bytesReq;
   tbSize      = dlRbAlloc->tbInfo[0].bytesReq;

   while(remBytesReq && sbCqiPtr && (numSbAlloc < dlfsCell->maxDlSbPerUe))
   {
      if (sbCqiPtr->cqi >= dlfsCell->thresholdCqi)
      {
         dlSb = &(dlfsSf->dlSb[sbCqiPtr->sbIdx]);
         if (dlSb->isAvail)
         {
            /* Note: Determining bytes that can be satisfied by the sub-band
             * size at reported CQI value. This method has inaccuracy 
             * especially for low CQI, but works quite OK for high CQI.
             * This solution helps in reducing the CQI cycles. */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
            /* rg001.201:106531: Inappropriate name of CQI to ITbs table for DL. */
            iTbs1 = (*(RgSchCmnCqiToTbs *)(cmnCell->dl.cqiToTbsTbl[noLyr1 - 1]))
               [sbCqiPtr->cqi];
            bytesAlloc = (rgTbSzTbl[noLyr1 - 1][iTbs1][dlSb->sbSize - 1])/8;

            /* Allocate the available sub-band */
            rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
            imcsCqi += (sbCqiPtr->cqi * rbAlloc);
            totAllocRbs += rbAlloc; 
            totAllocBytes += bytesAlloc;
            dlAllocBitMask |= 1 << (31 - allocIdx); 
            numSbAlloc++;
            remBytesReq = (remBytesReq > bytesAlloc) ? 
               (remBytesReq - bytesAlloc) : 0;
         }
         sbCqiPtr = sbCqiPtr->nxtSbCqi;
         continue;
      }
      /* No more good sub-bands exist */
      break;
   }

   /* If entire TB size cannot be satisfied, add allocated holes back to the
    * list */
   if (remBytesReq && dlAllocBitMask) 
   {
      rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
      RETVOID;
   }

   /* Update allocation Info if the entire TB size could be satisfied */
   if (totAllocRbs) 
   {
      U32 y;

      /* Take floor of the computed average */
      dlfsUe->imcsCqi = (U8)(imcsCqi/totAllocRbs);

      /* Retrieve the PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg004.201 patch - ccpu00110457 DTX Changes Start */
      /* DTX Change : One Variable is passed to check DTX or not */
      dlUe = RG_SCH_CMN_GET_DL_UE(ue);
      if (dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX)
      {
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
               dlRbAlloc->dciFormat, TRUE);
      }
      else{
         dlRbAlloc->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlfsUe->imcsCqi,\
               dlRbAlloc->dciFormat, FALSE);
      }
      if (!(dlRbAlloc->pdcch))
      {
         rgSCHDlfsRlsAlloc(dlfsSf, dlAllocBitMask);
         RETVOID;
      }

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

      dlRbAlloc->rbsAlloc = totAllocRbs;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->tbInfo[0].bytesAlloc = tbSize; 
      dlRbAlloc->tbInfo[1].bytesAlloc = dlRbAlloc->tbInfo[1].bytesReq; 

      /* Update imcs for re-transmission in range: 29-31 */
      RG_SCH_DLFS_CQI_TO_MODODR(dlfsUe->imcsCqi, &mod);
      RG_SCH_DLFS_MODODR_TO_RETX_MCS(mod, &dlRbAlloc->tbInfo[0].imcs);

      if (dlRbAlloc->tbInfo[1].schdlngForTb)
      {
         iTbs2 = (*(RgSchCmnCqiToTbs *)(cmnCell->dl.cqiToTbsTbl[noLyr2 - 1]))[dlfsUe->imcsCqi];
         RG_SCH_CMN_TBS_TO_MCS(iTbs2, dlRbAlloc->tbInfo[1].imcs);
         dlRbAlloc->tbInfo[1].bytesAlloc = 
            rgTbSzTbl[noLyr2 - 1][iTbs2][dlRbAlloc->rbsAlloc - 1]/8;
      }

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      dlRbAlloc->allocInfo.raType0.numDlAlloc += numSbAlloc; 
      dlRbAlloc->allocInfo.raType0.dlAllocBitMask |= dlAllocBitMask;

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
   }
   RETVOID;
}
/**
 * @brief  Determines the Imcs for the allocation.
 *
 * @details
 *
 *     Function :rgSCHDlfsFillImcs
 *
 *     Processing Steps:
 *     - Determine the Imcs for allocation based on CQI.
 *
 *  @param[in]       RgSchCellCb     *cell
 *  @param[in]       RgSchDlfsUeCb   *dlfsUe
 *  @param[in, out]  RgSchDlRbAlloc  *alloc
 *  @param[in]       U8              isRetx
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsFillImcs
(
RgSchCellCb        *cell,
RgSchDlfsUeCb      *dlfsUe,
RgSchDlRbAlloc     *alloc,
U8                 isRetx
)
#else
PRIVATE Void rgSCHDlfsFillImcs(cell, dlfsUe, alloc, isRetx)
RgSchCellCb        *cell;
RgSchDlfsUeCb      *dlfsUe;
RgSchDlRbAlloc     *alloc;
U8                 isRetx;
#endif
{
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8              mod;
   RgSchCmnCell    *cmnCell = RG_SCH_CMN_GET_CELL(cell);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8              noLyr1        = alloc->tbInfo[0].noLyr;
   U8              noLyr2        = alloc->tbInfo[1].noLyr;
   U8              iTbs1;
   U8              iTbs2;
   TRC2(rgSCHDlfsFillImcs);

   /* Determine iTbs from CQI */
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   if (!isRetx)
   {
      /* Update tbSize and imcs for transmission in range: 1-28*/
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* rg001.201:106531: Inappropriate name of CQI to ITbs table for DL. */
      iTbs1 = (*(RgSchCmnCqiToTbs *)(cmnCell->dl.cqiToTbsTbl[noLyr1 - 1]))[dlfsUe->imcsCqi];
      RG_SCH_CMN_TBS_TO_MCS(iTbs1, alloc->tbInfo[0].imcs);
      alloc->tbInfo[0].bytesAlloc = rgTbSzTbl[noLyr1 - 1][iTbs1][alloc->rbsAlloc - 1]/8;
      if (alloc->tbInfo[1].schdlngForTb)
      {
         iTbs2 = (*(RgSchCmnCqiToTbs *)(cmnCell->dl.cqiToTbsTbl[noLyr2 - 1]))
            [dlfsUe->imcsCqi];
         RG_SCH_CMN_TBS_TO_MCS(iTbs2, alloc->tbInfo[1].imcs);
         alloc->tbInfo[1].bytesAlloc = 
            rgTbSzTbl[noLyr2 - 1][iTbs2][alloc->rbsAlloc - 1]/8;
      }
   }
   else
   {
      /* Update imcs for re-transmission in range: 29-31 */
      RG_SCH_DLFS_CQI_TO_MODODR(dlfsUe->imcsCqi, &mod);
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      RG_SCH_DLFS_MODODR_TO_RETX_MCS(mod, &alloc->tbInfo[0].imcs);
      RG_SCH_DLFS_MODODR_TO_RETX_MCS(mod, &alloc->tbInfo[1].imcs);
   }

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHDlfsCalcBpIdx
 *
 *
 *     Desc : Determines the BP index from the timing info
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsCalcBpIdx
(
CmLteTimingInfo timingInfo,
RgSchDlfsUeCb   *dlfsUe,
U8              *bpIdx
)
#else
PRIVATE S16 rgSCHDlfsCalcBpIdx(timingInfo, dlfsUe, bpIdx)
CmLteTimingInfo timingInfo;
RgSchDlfsUeCb   *dlfsUe;
U8              *bpIdx;
#endif
{
   U16 sfNum = (timingInfo.sfn * 10 + timingInfo.subframe);
   U8  hVal   = (dlfsUe->numBp * dlfsUe->k) + 1;
   U8  prdNum = sfNum/dlfsUe->periodicity;
   TRC2(rgSCHDlfsCalcBpIdx);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (!(prdNum % hVal))
   {
      /* Report recieved at wrong timing */
      RETVALUE(RFAILED);
   }
#endif

   *bpIdx = ((prdNum % hVal) - 1) % dlfsUe->numBp;  

   RETVALUE(ROK);
}
/***********************************************************
 *
 *     Func : rgSCHDlfsAddHole
 *
 *
 *     Desc : Add downlink hole at the specified position
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHDlfsAddHole
(
RgSchDlfsSf    *dlfsSf,
U8             pos
)
#else
PRIVATE Void rgSCHDlfsAddHole(dlfsSf, pos)
RgSchDlfsSf    *dlfsSf;
U8             pos;
#endif
{
   TRC2(rgSCHDlfsAddHole);
   dlfsSf->dlSb[pos].isAvail = TRUE;
   cmLListAdd2Tail(&dlfsSf->holeLst, &(dlfsSf->dlSb[pos].holeLnk));

   RETVOID;
}
/***********************************************************
 *
 *     Func : rgSCHDlfsRlsHole
 *
 *
 *     Desc : Release downlink hole
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHDlfsRlsHole
(
RgSchDlfsSf    *dlfsSf,
RgSchDlfsHole  *hole
)
#else
PRIVATE Void rgSCHDlfsRlsHole(dlfsSf, hole)
RgSchDlfsSf    *dlfsSf;
RgSchDlfsHole  *hole;
#endif
{
   TRC2(rgSCHDlfsRlsHole);
   cmLListDelFrm (&(dlfsSf->holeLst), &(hole->holeLnk));
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHDlfsGetAllocFrmHole
 *
 *
 *     Desc : Gets a sub-band allocation from the hole
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsGetAllocFrmHole
(
RgSchDlfsHole   *hole,
U8              *allocIdx,
U8              *numAllocRbs
)
#else
PRIVATE S16 rgSCHDlfsGetAllocFrmHole(hole, allocIdx, numAllocRbs)
RgSchDlfsHole   *hole;
U8              *allocIdx;
U8              *numAllocRbs;
#endif
{
   TRC2(rgSCHDlfsGetAllocFrmHole);
   /* Convert a hole into allocation */
   *allocIdx = hole->sbIdx;
   *numAllocRbs = hole->sbSize;
   hole->isAvail = FALSE;

   RETVALUE(ROK);
}

/**
 * @brief  Releases allocation made through allocation bitmask
 *
 * @details
 *
 *     Function :rgSCHDlfsRlsAlloc
 *
 *     Processing Steps:
 *     - Create holes at the positions specified by the bitmask
 *
 *  @param[in]  RgSchDlfsSf *dlfsSf
 *  @param[in]  U32         allocBitMask 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsRlsAlloc
(
RgSchDlfsSf  *dlfsSf,
U32          allocBitMask
)
#else
PRIVATE Void rgSCHDlfsRlsAlloc(dlfsSf, allocBitMask)
RgSchDlfsSf  *dlfsSf;
U32          allocBitMask;
#endif
{
   U8 allocIdx = 0;
   TRC2(rgSCHDlfsRlsAlloc);

   while (allocIdx < (dlfsSf->numDlSb))
   {
      if (allocBitMask & (1 << (31 - allocIdx)))
      {
         rgSCHDlfsAddHole(dlfsSf, allocIdx);
      } 
      allocIdx++;
   }
   RETVOID;
}

/**
 * @brief  Frees subframe related information at DLFS
 *
 * @details
 *
 *     Function :rgSCHDlfsFreeSf
 *
 *     Processing Steps:
 *     - Free the sub-bands allocated for the sub-frame.
 *     - Re-initialize the hole list.
 *
 *  @param[in]  Inst        instIdx  
 *  @param[in]  RgSchDlfsSf *dlfsSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsFreeSf
(
Inst         instIdx,
RgSchDlfsSf  *dlfsSf
)
#else
PRIVATE Void rgSCHDlfsFreeSf(instIdx, dlfsSf)
Inst         instIdx;
RgSchDlfsSf  *dlfsSf;
#endif
{
   TRC2(rgSCHDlfsFreeSf);
   /* Deallocate the sub-bands */
   if (dlfsSf)
   {
      if (dlfsSf->dlSb)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(instIdx, (Data **)(&(dlfsSf->dlSb)),
               (dlfsSf->numDlSb * sizeof(RgSchDlfsSb)));
      }
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(instIdx, (Data **)&dlfsSf, sizeof(RgSchDlfsSf));
   }

   RETVOID;
}

/* rg004.201-ADD-Added for BCCH/PCCH handling */
/**
 * @brief Handles DLFS based RB allocation for BCCH,PCCH
 *
 * @details
 *
 *     Function : rgSCHDlfsBcchPcchRbAlloc
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell for RB allocation.
 *      - MAX C/I/PFS/RR shall provide the requiredBytes, required RBs
 *        estimate and subframe for each allocation to be made to SCH.
 *        
 *     Processing Steps:
 *     - Allocate for common channels BCCH on DLSCH, PCCH. 
 *       Call  rgSCHDlfsCmnRbAlloc().
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHDlfsBcchPcchRbAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHDlfsBcchPcchRbAlloc(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchDlRbAlloc     *reqAllocInfo;
#ifdef DEBUGP
   Inst               instIdx        = cell->instIdx;
#endif

   TRC2(rgSCHDlfsBcchPcchRbAlloc);

   /* Allocate for BCCH on DLSCH */
   reqAllocInfo = &(allocInfo->bcchAlloc);
   if (reqAllocInfo->rbsReq)
   {
      rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo);
      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
               "DLFS BCCH Allocation done: rbsReq: %d, rbsAlloc: %d\n",
               reqAllocInfo->rbsReq, reqAllocInfo->rbsAlloc));
   }

   /* Allocate for PCCH */
   reqAllocInfo = &(allocInfo->pcchAlloc);
   if (reqAllocInfo->rbsReq)
   {
      rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo);
      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
               "PCCH Allocation done: rbsReq: %d, rbsAlloc: %d\n",
               reqAllocInfo->rbsReq, reqAllocInfo->rbsAlloc));
   }

   RETVOID;
}

/**********************************************************************
 
         End of file:     gk_sch_dlfs.c@@/main/2 - Sat Jul 30 02:21:40 2011
 
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
/main/1      ---     sd   1. LTE MAC 2.1 release
           rg001.201 sm   1. Inappropriate name 
                             of CQI to ITbs table for DL has been 
                             corrected.
           rg003.201 ns   1. Trace added for functions.
           rg004.201 ms   1. DL DTX Handling
           rg004.201 rnt  1. Modified for SI Enhancement
           rg005.201 sm   1. Moved the variable declaration.
           rg007.201   ap  1. Added support for MIMO
           rg008.201 sd  1. Removed dependency on MIMO compile-time flag
           rg009.201 rsharon  1.[ccpu00112372]:Changes for CR timer.
                     gj       2. Changing loop limit from RGSCH_NUM_SUB_FRAMES
                                 to RGSCH_NUM_DL_SUBFRAMES
           rg009.201 dv 1. Added Changes of TFU_UPGRADE
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
           rg002.301 dvasisht 1.[cppu00120694]: DLSF Index calculation support for TDD
*********************************************************************91*/
