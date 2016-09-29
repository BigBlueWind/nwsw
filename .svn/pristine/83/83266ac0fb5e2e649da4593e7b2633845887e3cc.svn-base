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
 
     Desc:     C source code for Entry point functions.
 
     File:     rg_sch_cfg.c
 
     Sid:      gk_sch_cfg.c@@/main/2 - Sat Jul 30 02:21:35 2011
 
     Prg:     ns
 
**********************************************************************/

/** @file rg_sch_cfg.c
@brief This module handles the configuration of SCH by RRC and RRM.
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
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "rg_sch_cmn.h"

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
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"            /* typedefs for MAC */
#include "rg_sch.x"            /* typedefs for MAC */
/* [ccpu00124018]-MOD- Retrieving CQI value from cell config*/
#include "rg_sch_cmn.x"   

/* LTE-MAC Scheduler instance control block structures */
PUBLIC RgSchCb rgSchCb[RGSCH_MAX_INST];

#ifdef PHY_ERROR_LOGING
PUBLIC RgSchUlAllocCntr rgSchUlAllocCntr;
#endif

/* local defines */
PRIVATE S16 rgSCHCfgVldtUePwrCfg ARGS((RgSchCellCb *cell,
         RgrUeUlPwrCfg *pwrCfg));
PRIVATE S16 rgSCHCfgVldtUeGrpPwrCfg ARGS((RgSchCellCb *cell,
         RgrUeGrpPwrCfg *grpPwrCfg));
#ifdef LTEMAC_SPS
PRIVATE S16 rgSCHCfgVldtUeDlSpsCfg ARGS((RgSchCellCb *cell,
         RgrUeSpsDlCfg *dlSpsCfg));
PRIVATE S16 rgSCHCfgVldtSpsReCfg ARGS ((RgSchCellCb *cell,
         RgSchUeCb *ue, RgrUeRecfg *ueRecfg));
#endif /*LTEMAC_SPS*/
PRIVATE S16 rgSCHCfgVldtUeCqiModeCfg ARGS((RgSchCellCb *cell,RgrUeCfg *ueCfg));
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepCfg ARGS ((RgSchCellCb *cell,
         RgrUeCfg *ueCfg));
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepRecfg ARGS ((RgSchCellCb *cell,
         RgrUeRecfg *ueRecfg));
PRIVATE Void rgSCHCfgFreeDlDedLcCb ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc));
PRIVATE Void rgSCHCfgFreeDlCmnLcCb ARGS((RgSchClcDlLcCb *cmnDlLc));
PRIVATE Void rgSCHCfgFreeUeCb ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
PRIVATE Void rgSCHCfgFreeRgrCfgLst ARGS((RgSchCellCb *cell));
PRIVATE Void rgSCHCfgFreeCmnLcLst ARGS((RgSchCellCb *cell));
PRIVATE Void rgSCHCfgFreeUeLst ARGS((RgSchCellCb *cell));
#ifdef LTEMAC_SPS
PRIVATE Void rgSCHCfgFreeSpsUeLst ARGS((RgSchCellCb *cell));
#endif
PRIVATE S16 rgSCHCfgVldtRgrCmnLcCfg ARGS((Inst inst, RgrCellCfg *cellCfg,
                         RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHCfgVldtRgrCellPwrCfg ARGS((Inst inst, RgrCellCfg *cellCfg,
         RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHCfgVldtRgrCellSchCfg ARGS((Inst inst, RgrCellCfg *cellCfg));
PRIVATE S16 rgSCHCfgVldtRgrCellRACfg ARGS((Inst inst, RgrCellCfg *cellCfg));
PRIVATE Void rgSCHCfgRgrUePhrMsg3 ARGS(( RgSchCellCb *cell,
             RgSchRaCb *raCb,RgSchUeCb *ue, RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHCfgRgrCmnLcCfg ARGS((RgSchCellCb *cell, RgrCmnLchCfg *lcCfg,
                         RgSchErrInfo *errInfo));
/* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
PRIVATE S16 rgSCHCfgVldtRgrCellSiCfg ARGS(( Inst inst, RgrSiCfg  *siCfg));
#endif/*RGR_SI_SCH */

#ifdef LTEMAC_DRX
PRIVATE S16 rgSCHCfgVldtDrxUeCfg ARGS ((RgSchCellCb *cell,
                                       RgrUeDrxCfg *ueDrxCfg));
PRIVATE S16 rgSCHCfgVldtDrxOnDurCfg ARGS((U8 onDurTmr));
PRIVATE S16 rgSCHCfgVldtDrxInActvCfg ARGS((U16 inActvTmr));
PRIVATE S16 rgSCHCfgVldtDrxReTxCfg ARGS((U8 reTxTmr));
PRIVATE S16 rgSCHCfgVldtDrxLngCycCfg ARGS((RgrDrxLongCycleOffst lngCycleOffst));
PRIVATE S16 rgSCHCfgVldtDrxLngCyclTmrs ARGS((U16 val));
PRIVATE S16 rgSCHCfgVldtDrxShrtCycCfg ARGS((RgrDrxShortDrx shrtCycCfg));
#endif /*LTEMAC_DRX*/
#ifdef TFU_UPGRADE

PUBLIC S16 rgSCHCfgACqiUeCfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb  *ueCb, 
                     RgrUeAprdDlCqiCfg *aCqiCfg, CmLteUeCategory ueCat ));

PUBLIC S16 rgSCHCfgAcqiUeReCfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb, 
 RgrUeAprdDlCqiCfg   *acqiCfg, CmLteUeCategory  ueCat ));

PUBLIC S16 rgSCHUtlGetCfgPerOff ARGS(( RgSchPerTbl tbl, U16  cfgIdx, 
         U16 *peri, U16 *offset ));

PUBLIC S16 rgSCHCfgRiUeCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb    *ueCb, 
                   RgrUePrdDlCqiCfg *cqiCfg, CmLteUeCategory ueCat ));

PUBLIC S16 rgSCHCfgPCqiUeCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb    *ueCb, 
                     RgrUePrdDlCqiCfg *cqiCfg,CmLteUeCategory ueCat ));

PUBLIC S16 rgSCHCfgSrsUeCfg ARGS((RgSchCellCb   *cellCb,RgSchUeCb     *ueCb,
                     RgrUeUlSrsCfg *srsCfg ));

PUBLIC S16 rgSCHCfgSrUeCfg ARGS((RgSchCellCb   *cellCb,RgSchUeCb     *ueCb,
                     RgrUeSrCfg    *srCfg));

PUBLIC S16 rgSCHCfgPCqiUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                     RgrUePrdDlCqiCfg *cqiCfg,CmLteUeCategory ueCat));

PUBLIC S16 rgSCHCfgSrsUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                     RgrUeUlSrsCfg *srsCfg));

PUBLIC S16 rgSCHCfgSrUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                      RgrUeSrCfg   *srCfg));

PUBLIC S16 rgSCHCfgVldtRgrTxmodePuschMode ARGS((RgSchCellCb  *cellCb,
         RgrTxMode  txMde, RgrAprdCqiMode puschMode,RgSchErrInfo *errInfo));

PUBLIC S16 rgSCHCfgVldtRgrUeACqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUeAprdDlCqiCfg *acqiCfg, RgrUeTxModeCfg txMode,
         RgSchErrInfo  *errInfo ));

PUBLIC S16 rgSCHCfgVldtRgrTxmodePucchMode ARGS((RgSchCellCb  *cellCb,
         RgrTxMode  txMde, RgrPrdCqiMode pucchMode,RgSchErrInfo *errInfo));

#ifdef LTEMAC_HDFDD
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUePrdDlCqiCfg *cqiCfg, Bool hdFdd,
         RgrUeTxModeCfg txMode, RgSchErrInfo *errInfo));
#else
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUePrdDlCqiCfg *cqiCfg, RgrUeTxModeCfg txMode,
         RgSchErrInfo *errInfo));         
#endif
#ifdef LTEMAC_HDFDD
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg  ARGS ((RgSchCellCb *cellCb,
             CmLteRnti crnti, RgrUeUlSrsCfg *srsCfg, Bool hdFdd, 
             RgSchErrInfo *errInfo));
#else
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg  ARGS ((RgSchCellCb *cellCb,
         CmLteRnti crnti, RgrUeUlSrsCfg *srsCfg, 
         RgSchErrInfo *errInfo));
#endif

#ifdef LTEMAC_HDFDD
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg  ARGS((RgSchCellCb *cellCb, CmLteRnti crnti,  
         RgrUeSrCfg *srCfg, Bool hdFdd, RgSchErrInfo *errInfo));
#else
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg  ARGS((RgSchCellCb *cellCb, CmLteRnti crnti,  
         RgrUeSrCfg *srCfg, RgSchErrInfo *errInfo));
#endif
PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeCfg ARGS (( RgSchCellCb       *cellCb,
                                             RgrUeCfg          *ueCfg,  
                                             RgSchErrInfo      *errInfo));

PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeReCfg ARGS (( RgSchCellCb  *cellCb,
                                               RgSchUeCb    *ueCb,
                                               RgrUeRecfg   *ueReCfg,  
                                               RgSchErrInfo *errInfo ));

PUBLIC S16 rgSCHCfgPCqiSrsSrUeDel ARGS ((RgSchCellCb *cellCb, 
                                         RgSchUeCb *ueCb));

PRIVATE Void rgSCHCfgUtlFetchAcqiBitSz ARGS ((RgSchUeCb* ueCb, U8  numTxAnt,
                     U8* cqiPmiSzR1,U8* cqiPmiSzRn1 ));

/* rg002.301 - ADD - Added the function to be used instead of the 
 *  MACRO RG_SCH_GET_PERIODICITY_TBL */
PRIVATE CONSTANT RgSchUePCqiSrsSrCfgIdxTbl*  rgSCHCfgUtlGetPcqiSrsSrRiTbl ARGS ((
                                           RgSchPerTbl tblType, 
                                           U8 * min, 
                                           U8 * max));

#endif /* TFU_UPGRADE */ 
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
PRIVATE S16 rgSCHCfgUeCqiReptReCfg ARGS ((
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg
));

PRIVATE S16 rgSCHCfgVldtCqiReptReCfg ARGS ((
RgSchCellCb       *cell,
RgrUeRecfg     *ueRecfg
));
#endif

/* local typedefs */
CONSTANT RgSchSrsTxOffst rgSrsTxOffstTbl[RGSCH_MAX_SRS_SFCFG_IDX+1]=
                                  {{1,{0,0,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{2,0,0,0,0,0,0,0}},
                                   {1,{3,0,0,0,0,0,0,0}},
                                   {2,{0,1,0,0,0,0,0,0}},
                                   {2,{2,3,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{2,0,0,0,0,0,0,0}},
                                   {1,{3,0,0,0,0,0,0,0}},
                                   {7,{0,1,2,3,4,6,8,0}},
                                   {8,{0,1,2,3,4,5,6,8}},
                                   {0,{0,0,0,0,0,0,0,0}}};
 
/* local externs */

#ifdef TFU_UPGRADE
/* SRSCfg Table: Ref 36.213, Table: 8.2.1  */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl  
rgSchUeSrsCfgIdxFddTbl[RG_SCH_SRS_ISRS_INDX_MAX_FDD]=
{
 { 0,1,    2,  0 },
 { 2,6,    5,  2 },
 { 7,16,   10, 7 },
 { 17,36,  20, 17},
 { 37,76,  40, 37},
 { 77,156, 80, 77},
 { 157,316,160,157},
 { 317,636,320,317 }
};

/* Reference : 36.213 Table 8.2-2 */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeSrsCfgIdxTddTbl[RG_SCH_SRS_ISRS_INDX_MAX_TDD]=
{
 { 10,14,  5,  10 },
 { 15,24,  10, 15 },
 { 25,44,  20, 25 },
 { 45,84,  40, 45 },
 { 85,164, 80, 85 },
 { 165,324,160, 165 },
 { 325,644,320, 325 }
 /* RESERVED: Configuration Module should not allow Res values */
};

/*Reference: 36.213 Table:7.2.2-1A */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUePCqiCfgIdxFddTbl[RG_SCH_CQIPMI_CFGIDX_MAX_FDD]=
{
 { 0,1,   2, 0 },
 { 2,6,   5, 2 },
 { 7,16,  10,7 },
 {17,36,  20,17},
 {37,76,  40, 37},
 {77,156, 80, 77},
 {157,316,160,157},
 {318,349,32,318}, 
 {350,413,64, 350},
 {414,541,128,414 }
 /* RESERVED: Configuration should not allow Res values */
};

/* Reference: 36.213 Table:7.2.2-1C */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeCqiPmiCfgIdxTddTbl[RG_SCH_CQIPMI_CFGIDX_MAX_TDD]=
{
 { 0,0,   1, 0 },
 { 1,5,   5, 1 },
 { 6,15,  10,6 },
 {16,35,  20,16},
 {36,75,  40,36},
 {76,155, 80,76},
 {156,315,160,156}
 /* RESERVED: Configuration should not allow Res values */
};

/* Note: RI table is same for FDD and TDD */
/*Reference: 36.213 Table:7.2.2-1B */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeRiCfgIdxTbl[RG_SCH_RI_CFGIDX_MAX]=
{
 { 0,160,    1, 0 },
 { 161,321,  2, 161 },
 { 322,482,  4, 322 },
 { 483,643,  8, 483},
 { 644,804,  16,644},
 { 805,965,  32,805 }
 /* RESERVED: Configuration should not allow Res values */
};

/*Reference: 36.213 Table:7.2.2-2 */
CONSTANT RgSchUeBwSubSzBwParts 
rgSchUeBwSubSzBwPartsTbl[RG_SCH_BW_SUBSZ_BWPARTS_MAX]=
{
 {6,7, 0, 0},  /*TODO: 6,7, NA, NA */
 {8, 10, 4,1},
 {11, 26, 4,2},
 {27, 63, 6,3},
 {64, 110, 8,4}
};


/* Reference : 36.213 Table 10.1-5 */
/* Note: SR is same table for TDD and FDD */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl rgSchUeSrCfgIdxTbl[RG_SCH_ISR_INDX_MAX]=
{
 { 0,4,   5, 0 },
 { 5,14,   10, 5 },
 { 15,34,  20,15 },
 { 35,74, 40,35},
 { 75,154, 80, 75}
 /* RESERVED: Configuration should not allow Res values */
};

/*Reference:36.213: Derived from Table: 7.2.1-5 for Label L.
 The Label L is CEIL(log2(BinCoe(N and M)))*/
CONSTANT U8 RgSCHUeAcqi2022LBitWidth[6][28] ={
{0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5},
{0,0,2,3,4,4,5,5,6,6,6,7,7,7,7,7,8,8,8,8,8,8,8,9,9,9,9,9},
{0,0,0,2,4,5,6,6,7,7,8,8,9,9,9,10,10,10,10,11,11,11,11,11,12,12,12,12},
{0,0,0,0,3,4,6,7,7,8,9,9,10,10,11,11,12,12,12,13,13,13,14,14,14,14,15,15},
{0,0,0,0,0,3,5,6,7,8,9,10,11,11,12,13,13,14,14,14,15,15,16,16,16,17,17,17},
{0,0,0,0,0,0,3,5,7,8,9,10,11,12,13,13,14,15,15,16,16,17,17,18,18,18,19,19}
};

#endif 
 
/* forward references */



/**
 * @brief Validates the cell configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellCfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of configured values recieved in
 *          configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *cellCfg
 *  @param[out] RgSchCellCb  **cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrCellCfg
(
Inst         inst,
RgrCellCfg   *cellCfg,
RgSchCellCb  *cell,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrCellCfg(inst, cellCfg, cell, errInfo)
Inst         inst;
RgrCellCfg   *cellCfg;
RgSchCellCb  *cell;
RgSchErrInfo *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrCellCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATE RGR CELL CONFIG:cellId %d \n",
                       cellCfg->cellId));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CELL_CFG;

   /* check if cell exists already */
   if ((U8 *)cell != NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell with Id %d already exists\n",
               cell->cellId));
      RETVALUE(RFAILED);
   }
   
   if(cellCfg->macInst >= RGSCH_INST_START)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid MAC Instance %d \n",
               cellCfg->macInst));
      RETVALUE(RFAILED);
   }
   
   if (cellCfg->macRnti.startRnti < RGSCH_MIN_MAC_RNTI )
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid start RNTI %d for cell \n",
               cellCfg->macRnti.startRnti));
      RETVALUE(RFAILED);
   }

   if ((rgSCHCfgVldtRgrCellSchCfg(inst, cellCfg)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Validation for scheduler related "
                          "config failed\n"));
      RETVALUE(RFAILED);
   }

   if ((cellCfg->dlHqCfg.maxDlHqTx < RGSCH_MIN_HQ_TX) ||
         (cellCfg->dlHqCfg.maxMsg4HqTx < RGSCH_MIN_HQ_TX))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),"Invalid Downlink HARQ configuration:"
               " maxDlHqTx %d maxMsg4HqTx %d\n", cellCfg->dlHqCfg.maxDlHqTx,
               cellCfg->dlHqCfg.maxMsg4HqTx));
      RETVALUE(RFAILED);
   }
   if ((cellCfg->cfiCfg.cfi < RGSCH_MIN_CFI_VAL) ||
         (cellCfg->cfiCfg.cfi > RGSCH_MAX_CFI_VAL))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid CFI configuration %d\n",
           cellCfg->cfiCfg.cfi));
      RETVALUE(RFAILED);
   }
   if (((cellCfg->puschSubBand.subbandStart) + 
        ((cellCfg->puschSubBand.numSubbands -1) * cellCfg->puschSubBand.size))
         > (cellCfg->bwCfg.ulTotalBw - 1))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),"Invalid PUSCH subband configuration:"
               " subBandStart %d numSubBands %d subBandSize %d ulTotBw %d\n",
               cellCfg->puschSubBand.subbandStart, 
               cellCfg->puschSubBand.numSubbands, cellCfg->puschSubBand.size,
               cellCfg->bwCfg.ulTotalBw));
      RETVALUE(RFAILED);
   }

   /* Validation for the cell parameters */
   if ((cellCfg->numTxAntPorts == 0) || (cellCfg->numTxAntPorts > 4))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),"Invalid number of transmit antenna" 
            " ports %d\n", cellCfg->numTxAntPorts));
      RETVALUE(RFAILED);
   }
   if (((cellCfg->bwCfg.dlTotalBw < RGSCH_MIN_DL_BW) ||
            (cellCfg->bwCfg.dlTotalBw > RGSCH_MAX_DL_BW)) ||
         ((cellCfg->bwCfg.ulTotalBw < RGSCH_MIN_UL_BW) || 
          (cellCfg->bwCfg.ulTotalBw > RGSCH_MAX_UL_BW)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid Bandwidth configuration:"
            " ul %d dl %d",cellCfg->bwCfg.ulTotalBw,
            cellCfg->bwCfg.dlTotalBw));
      RETVALUE(RFAILED);
   }
   if (cellCfg->phichCfg.ngEnum > RGR_NG_TWO)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid PHICH Ng configuration %d\n",
               (U8)cellCfg->phichCfg.ngEnum));
      RETVALUE(RFAILED);
   }
   /* Validation for extended PHICH Duration */
   if ((cellCfg->phichCfg.isDurExtend == TRUE) && 
         (cellCfg->bwCfg.dlTotalBw <= 10) && (cellCfg->cfiCfg.cfi < 2))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid cfi value for"
               "Extended PHICH duration cfi:%d dlBw:%d\n",
               (U8)cellCfg->cfiCfg.cfi, cellCfg->bwCfg.dlTotalBw));
      RETVALUE(RFAILED);
   }
   if ((cellCfg->phichCfg.isDurExtend == TRUE) &&
         (cellCfg->bwCfg.dlTotalBw > 10) && (cellCfg->cfiCfg.cfi < 3))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid cfi value for"
               "Extended PHICH duration cfi:%d dlBw:%d\n",
               (U8)cellCfg->cfiCfg.cfi, cellCfg->bwCfg.dlTotalBw));
      RETVALUE(RFAILED);
   }
   if ((cellCfg->pucchCfg.resourceSize == 0) ||
         (cellCfg->pucchCfg.resourceSize >= cellCfg->bwCfg.ulTotalBw/2) ||
         (cellCfg->pucchCfg.n1PucchAn == 0) ||
         (cellCfg->pucchCfg.deltaShift < RGSCH_PUCCH_MINVAL_DS) ||
         (cellCfg->pucchCfg.deltaShift > RGSCH_PUCCH_MAXVAL_DS) ||
         (cellCfg->pucchCfg.cyclicShift > RGSCH_PUCCH_MAXVAL_CS))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid PUCCH configuration:"
              " N2_RB %d N1_PUCCH %d deltaShift %d cyclicShift %d\n",
               cellCfg->pucchCfg.resourceSize,
               cellCfg->pucchCfg.n1PucchAn,
               cellCfg->pucchCfg.deltaShift,
               cellCfg->pucchCfg.cyclicShift));
      RETVALUE(RFAILED);
   }
   if (cellCfg->srsCfg.srsBwEnum > RGR_SRS_BWCFG_7)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid SRS configuration: "
               " srsBw %d\n", (U8)cellCfg->srsCfg.srsBwEnum));
      RETVALUE(RFAILED);
   }

   if ((rgSCHCfgVldtRgrCellRACfg(inst, cellCfg)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Validation for Random access related"
                          "config failed\n"));
      RETVALUE(RFAILED);
   }

   if ((rgSCHCfgVldtRgrCellPwrCfg(inst, cellCfg, errInfo)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Validation for cell power "
                          "config failed\n"));
      RETVALUE(RFAILED);
   }

   /* Validate the common logical channel configuration */
   if( (cellCfg->numCmnLcs < RGR_MIN_CMN_LC_PER_CELL)||
       (cellCfg->numCmnLcs > RGR_MAX_CMN_LC_PER_CELL))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid number(%d) of common logical"
              "channels in cell config \n", cellCfg->numCmnLcs));
      RETVALUE(RFAILED);
   }
   if ((rgSCHCfgVldtRgrCmnLcCfg(inst, cellCfg, errInfo)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Validation for common logical"
                          "channels failed\n"));
      RETVALUE(RFAILED);
   }

   /* rg001.201: Added for sending TTI tick to RRM */
   /* rg002.201: Added 0 as a valid value for number of TICKs RRM
    * 0 implies no ticks i.e. shutting off the feature.*/

   /* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
   if ((rgSCHCfgVldtRgrCellSiCfg(inst, &(cellCfg->siCfg))) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Validation for SI"
                          "configuration failed\n"));
      RETVALUE(RFAILED);
   }
#endif /*RGR_SI_SCH */

   /*ccpu00116923 - ADD - Srs Present support - Start*/
#ifdef TFU_UPGRADE
#ifdef LTE_TDD
   if(cellCfg->srsCfg.srsSubFrameCfg > 13)
#else
      if(cellCfg->srsCfg.srsSubFrameCfg > 14)
#endif
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid"
                  "Subframe configuration \n"));
         RETVALUE(RFAILED);
      }
#endif
   /*ccpu00116923 - ADD - Srs Present support - End*/

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR cell config validation done:"
            " cellId %d\n", cellCfg->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCellCfg */

/**
 * @brief Validates the scheduler related configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellSchCfg
 *
 *     Processing Steps:
 *      - Validate the scheduler related configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[out] RgSchCellCfg  *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrCellSchCfg
(
Inst         inst,
RgrCellCfg   *cellCfg
)
#else
PUBLIC S16 rgSCHCfgVldtRgrCellSchCfg(inst, cellCfg)
Inst         inst;
RgrCellCfg   *cellCfg;
#endif
{
   TRC2(rgSCHCfgVldtRgrCellSchCfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHCfgVldtRgrCellSchCfg:Validating \
               scheduler related Configuration"));
   if (cellCfg->ulSchdType > (RGSCH_NUM_SCHEDULERS - 1))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHCfgVldtRgrCellSchCfg:Invalid \
                  UL scheduler type %d for cell\n", cellCfg->ulSchdType));
      RETVALUE(RFAILED);
   }
   if (cellCfg->dlSchdType > (RGSCH_NUM_SCHEDULERS - 1))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHCfgVldtRgrCellSchCfg:Invalid \
                  DL scheduler type %d for cell\n",cellCfg->dlSchdType));
      RETVALUE(RFAILED);
   }
#if RGSCH_NUM_DLFS_SCHEDULERS
   if (cellCfg->dlfsSchdType > RGSCH_NUM_DLFS_SCHEDULERS - 1)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHCfgVldtRgrCellSchCfg:Invalid \
                  dlfs scheduler type %d for cell\n",cellCfg->dlfsSchdType));
      RETVALUE(RFAILED);
   }
#endif
   RETVALUE(ROK);
}
/**
 * @brief Validates the RACH related configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellRACfg
 *
 *     Processing Steps:
 *      - Validate the RA configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[out] RgSchCellCfg  *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrCellRACfg
(
Inst         inst,
RgrCellCfg   *cellCfg
)
#else
PUBLIC S16 rgSCHCfgVldtRgrCellRACfg(inst, cellCfg)
Inst         inst;
RgrCellCfg   *cellCfg;
#endif
{
   TRC2(rgSCHCfgVldtRgrCellRACfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "Validating RACH related Configuration"));

#ifdef LTE_TDD
   if ((cellCfg->rachCfg.preambleFormat > RGSCH_MAX_TDD_RA_PREAMBLE_FMT) ||
#else
   if ((cellCfg->rachCfg.preambleFormat > RGSCH_MAX_RA_PREAMBLE_FMT) ||
#endif
         (cellCfg->rachCfg.raWinSize < RGSCH_MIN_RA_WINSIZE)  ||
         (cellCfg->rachCfg.raWinSize > RGSCH_MAX_RA_WINSIZE) ||
         (cellCfg->rachCfg.maxMsg3Tx < RGSCH_MIN_HQ_TX) ||
         (cellCfg->rachCfg.numRaPreamble < RGSCH_MIN_NUM_RA_PREAMBLE) ||
         (cellCfg->rachCfg.numRaPreamble > RGSCH_MAX_NUM_RA_PREAMBLE) ||
         (cellCfg->rachCfg.sizeRaPreambleGrpA >
          cellCfg->rachCfg.numRaPreamble) || 
         (cellCfg->rachCfg.prachResource > 
          (cellCfg->bwCfg.ulTotalBw - RGSCH_NUM_RA_RB)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHCfgVldtRgrCellRACfg:Invalid \
            RACH configuration: preamble Fmt %d raWinSize %d maxMsg3Tx %d\
              numRaPreamble %d sizeRaPreambleGrpA %d",
            cellCfg->rachCfg.preambleFormat, cellCfg->rachCfg.raWinSize,
            cellCfg->rachCfg.maxMsg3Tx, cellCfg->rachCfg.numRaPreamble,
            cellCfg->rachCfg.sizeRaPreambleGrpA));
      RETVALUE(RFAILED);
   }
   /* RACHO */
   /* verify that the ded Preambles cfgd for Pdcch Order
    * do not collide with that of non-dedicated  and validates against
    * the configuration index and number of RACH
    * ded-preambles. For non-zero ded preamble cfg, 
    * the config index is expected to be != NA.*/
   if ((cellCfg->macPreambleSet.pres) && 
       ((cellCfg->macPreambleSet.start < cellCfg->rachCfg.numRaPreamble) ||
       (cellCfg->macPreambleSet.start >= RGSCH_MAX_NUM_RA_PREAMBLE) ||
       (cellCfg->macPreambleSet.size < 1) ||
       (cellCfg->macPreambleSet.size > RGSCH_MAX_NUM_RA_PREAMBLE-
                                       cellCfg->rachCfg.numRaPreamble) ||
       (cellCfg->rachCfg.raOccasion.sfnEnum == RGR_SFN_NA)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHCfgVldtRgrCellRACfg:Invalid \
                  RACH Preambleset conf:preambleSet Start %d \
                  preambleSet Size %d",
            cellCfg->macPreambleSet.start, cellCfg->macPreambleSet.size));
      RETVALUE(RFAILED);
   }
#ifdef 0 /*Manu: Removed this incorrect check. timer value is configured, not enum. RGR_V1*/
   if(cellCfg->rachCfg.contResTmr)
   {
      if(RG_SCH_GRD_TMR >= cellCfg->rachCfg.contResTmr)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHCfgVldtRgrCellRACfg:\
                     Contention Resolution timer not greater than the \
                     guard timer confiured Contention REsolution \
                     timer %d Guard timer %d",
                  cellCfg->rachCfg.contResTmr, RG_SCH_GRD_TMR));
         RETVALUE(RFAILED);
      }
   }
#endif

   RETVALUE(ROK);
}

/**
 * @brief Validates the UE configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeCfg
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchCellCb  **cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeCfg
(
Inst         inst,
RgrUeCfg     *ueCfg,
RgSchCellCb  **cell,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrUeCfg(inst, ueCfg, cell, errInfo)
Inst         inst;
RgrUeCfg     *ueCfg;
RgSchCellCb  **cell;
RgSchErrInfo *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrUeCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATE RGR UE CONFIG: cellId %d "
         "ueId %d cell %p\n", ueCfg->cellId, ueCfg->crnti, (void*)*cell));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_CFG;

   if (((*cell) == NULLP) || 
       ((*cell)->cellId != ueCfg->cellId))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist %d\n", 
           ueCfg->cellId));
      MSPD_ERR("Cell does not exist %d\n", ueCfg->cellId);
      RETVALUE(RFAILED);
   }
   /* RACHO: 
    * Check configured preamble id not colliding with non dedicated or PDCCH
    * order preamble sets. When valid preamble id given check that C-RNTI given
    * in configuration is not amongst the C-RNTI'smanaged by scheduler */
   if ((rgSCHRamVldtUeCfg(*cell, ueCfg)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Preamble Id configuration"
               "failed for UE %d\n",ueCfg->crnti));
      MSPD_ERR("Preamble Id configuration failed for UE %d\n",ueCfg->crnti);
      RETVALUE(RFAILED);
   }
   /* Check if Ue already configured */
   if (rgSCHDbmGetUeCb(*cell, ueCfg->crnti) != NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Ue already exists %d\n", 
            ueCfg->crnti));
      MSPD_ERR("Ue already exists %d\n", ueCfg->crnti);
      RETVALUE(RFAILED);
   }
   /* Validate Transmission UE modes  */
   if ((ueCfg->txMode.pres == TRUE) && ((ueCfg->txMode.txModeEnum < RGR_UE_TM_1)
            || (ueCfg->txMode.txModeEnum > RGR_UE_TM_7)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid transmission mode for"
               " UE %d\n", (U8)ueCfg->txMode.txModeEnum));
      MSPD_ERR("Invalid transmission mode for"
               " UE %d\n", (U8)ueCfg->txMode.txModeEnum);
      RETVALUE(RFAILED);
   }
  
   /* Validate UE Category */
   if (ueCfg->ueCatEnum > CM_LTE_UE_CAT_5)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid category for UE %d\n",
               (U8)ueCfg->ueCatEnum));
     MSPD_ERR("Invalid category for UE %d\n",
               (U8)ueCfg->ueCatEnum);
      RETVALUE(RFAILED);
   }
   MSPD_DBG("Configured category %u\n" ,ueCfg->ueCatEnum);
  
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
   if ((*cell)->numTxAntPorts == 1)
   {
      if ((ueCfg->txMode.pres == TRUE) && 
            (ueCfg->txMode.txModeEnum > RGR_UE_TM_1))
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid transmission mode for"
                  " UE (%d) for the configured Cell Antenna Ports",
                  (U8)ueCfg->txMode.txModeEnum));
         MSPD_ERR("Invalid transmission mode for"
                  " UE (%d) for the configured Cell Antenna Ports",
                  (U8)ueCfg->txMode.txModeEnum);
         RETVALUE(RFAILED);
      }
   }
   
   if ((rgSCHCfgVldtUeCqiModeCfg(*cell, ueCfg)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid CQI Mode "
            " configuration\n"));
      MSPD_ERR("Invalid CQI Mode configuration\n");
      RETVALUE(RFAILED);
   }

   /* Validate Max Uplink HARQ transmission value */
   if (ueCfg->ueUlHqCfg.maxUlHqTx < RGSCH_MIN_HQ_TX)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid Uplink HARQ config for "
            "UE %d\n", ueCfg->ueUlHqCfg.maxUlHqTx));
      MSPD_ERR("Invalid Uplink HARQ config for "
            "UE %d\n", ueCfg->ueUlHqCfg.maxUlHqTx);
      RETVALUE(RFAILED);
   }

   if (rgSCHCfgVldtUePwrCfg(*cell, &ueCfg->ueUlPwrCfg) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid PUSCH Group power"
           " configuration\n"));
      MSPD_ERR("Invalid PUSCH Group power configuration\n");
      RETVALUE(RFAILED);
   }
   
#ifdef LTEMAC_SPS
   /* Validating SPS RNTI */
   if ((ueCfg->ueSpsCfg.spsRnti >  (*cell)->rntiDb.rntiStart) && 
       (ueCfg->ueSpsCfg.spsRnti<=((*cell)->rntiDb.rntiStart+(*cell)->rntiDb.maxRntis)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid SPS RNTI "
               " in DL SPS Config\n"));
      MSPD_ERR("Invalid SPS RNTI in DL SPS Config\n");
      RETVALUE(RFAILED);
   }

   if (rgSCHCfgVldtUeDlSpsCfg(*cell, &ueCfg->ueSpsCfg.dlSpsCfg) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid DL SPS configuration"
           " for the UE\n"));
      MSPD_ERR("Invalid DL SPS configuration for the UE\n");
      RETVALUE(RFAILED);
   }
#endif 

   if (rgSCHCfgVldtUeMeasGapAckNakRepCfg(*cell, ueCfg) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid MeasGap/AckNackRep"
            " configuration\n"));
      MSPD_ERR("Invalid MeasGap/AckNackRep configuration\n");
      RETVALUE(RFAILED);
   }
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   /* Validated Periodic CQI/PMI, RI , SRS and SR related UeCfg */
   if ( ROK != rgSCHCfgVldtCqiSrSrsUeCfg(*cell, ueCfg, errInfo))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid Periodic CQI/SR/SRS"
               "configuration\n"));
      MSPD_ERR("Invalid Periodic CQI/SR/SRS configuration\n");
      RETVALUE(RFAILED);
   }
#endif

#ifdef LTEMAC_DRX
   /* Validate DRX specific parameters */
   if ( ROK != rgSCHCfgVldtDrxUeCfg(*cell, &(ueCfg->ueDrxCfg)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid DRX configuration\n"));
      MSPD_ERR("Invalid DRX configuration\n");
      RETVALUE(RFAILED);
   }

#endif /*LTEMAC_DRX*/
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   if (ueCfg->ueCqiReptCfg.numColltdCqiRept > RGR_CQIRPTS_MAXN)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid numColltdCqiRept,"
               "MAX supported %d\n",RGR_CQIRPTS_MAXN));
      MSPD_ERR("Invalid numColltdCqiRept,"
               "MAX supported %d\n",RGR_CQIRPTS_MAXN);
      RETVALUE(RFAILED);
   }
#endif /* End of RGR_CQI_REPT */

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR UE config validation done:"
       " cellId %d ueId %d\n",
        ueCfg->cellId, ueCfg->crnti));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrUeCfg */


/**
 * @brief Validates the cell reconfiguration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellRecfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  RgrCellRecfg  *cellRecfg
 *  @param[out] RgSchCellCb   **cell
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrCellRecfg
(
Inst          inst,
RgrCellRecfg  *cellRecfg,
RgSchCellCb   **cell,
RgSchErrInfo  *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrCellRecfg(inst, cellRecfg, cell, errInfo)
Inst          inst;
RgrCellRecfg  *cellRecfg;
RgSchCellCb   **cell;
RgSchErrInfo  *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrCellRecfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATE RGR CELL RECONFIG: cellId %d"
        " cell %p\n", cellRecfg->cellId, (void*)*cell));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CELL_RECFG;
   
   /* Fetch cell and validate cell Id with the cell control block*/
   if (((*cell) == NULLP) ||
       ((*cell)->cellId != cellRecfg->cellId)) 
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell control block does not "
            "exist %d\n", cellRecfg->cellId));
      RETVALUE(RFAILED);
   }

   /* Validate recieved values */
   if ((cellRecfg->recfgTypes & RGR_CELL_DL_HARQ_RECFG) &&
         ((cellRecfg->dlHqRecfg.maxDlHqTx < RGSCH_MIN_HQ_TX) ||
          (cellRecfg->dlHqRecfg.maxMsg4HqTx < RGSCH_MIN_HQ_TX)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid Downlink HARQ configuration:"
           " maxDlHqTx %d maxMsg4HqTx %d\n", cellRecfg->dlHqRecfg.maxDlHqTx,
            cellRecfg->dlHqRecfg.maxMsg4HqTx));
      RETVALUE(RFAILED);
   }
   if ((cellRecfg->recfgTypes & RGR_CELL_CFI_RECFG) && 
         ((cellRecfg->cfiRecfg.cfi < RGSCH_MIN_CFI_VAL) ||
          (cellRecfg->cfiRecfg.cfi > RGSCH_MAX_CFI_VAL)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid CFI configuration %d\n",
               cellRecfg->cfiRecfg.cfi));
      RETVALUE(RFAILED);
   }
   if (cellRecfg->recfgTypes & RGR_CELL_PUCCH_RECFG)
   {
      if ((cellRecfg->pucchRecfg.resourceSize == 0) || 
          (cellRecfg->pucchRecfg.n1PucchAn == 0) ||
          (cellRecfg->pucchRecfg.resourceSize >= (*cell)->bwCfg.ulTotalBw/2)||
          ((cellRecfg->pucchRecfg.deltaShift < RGSCH_PUCCH_MINVAL_DS) || 
           (cellRecfg->pucchRecfg.deltaShift > RGSCH_PUCCH_MAXVAL_DS)) ||
          (cellRecfg->pucchRecfg.cyclicShift > RGSCH_PUCCH_MAXVAL_CS))
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid PUCCH configuration: "
               "N2_RB %d N1_PUCCH %d deltaShift %d cyclicShift %d\n",
               cellRecfg->pucchRecfg.resourceSize,
               cellRecfg->pucchRecfg.n1PucchAn,
               cellRecfg->pucchRecfg.deltaShift,
               cellRecfg->pucchRecfg.cyclicShift));
         RETVALUE(RFAILED);
      }
   }
   if (cellRecfg->recfgTypes & RGR_CELL_SRS_RECFG)
   {
      if (cellRecfg->srsRecfg.srsBwEnum > RGR_SRS_BWCFG_7)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid SRS configuration: "
               "srsBw %d", (U8)cellRecfg->srsRecfg.srsBwEnum));
         RETVALUE(RFAILED);
      }

      /*ccpu00116923 - ADD - Srs Present support - Start*/
#ifdef TFU_UPGRADE
#ifdef LTE_TDD
      if(cellRecfg->srsRecfg.srsSubFrameCfg > 13)
#else
         if(cellRecfg->srsRecfg.srsSubFrameCfg > 14)
#endif
         {
            RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid"
                     "Subframe configuration \n"));
            RETVALUE(RFAILED);
         }
#endif
      /*ccpu00116923 - ADD - Srs Present support - End*/
   }
   if (cellRecfg->recfgTypes & RGR_CELL_RACH_RECFG)
   {
      if ((cellRecfg->rachRecfg.preambleFormat > RGSCH_MAX_RA_PREAMBLE_FMT) ||
            ((cellRecfg->rachRecfg.raWinSize < RGSCH_MIN_RA_WINSIZE) ||
             (cellRecfg->rachRecfg.raWinSize > RGSCH_MAX_RA_WINSIZE)) ||
            (cellRecfg->rachRecfg.maxMsg3Tx < RGSCH_MIN_HQ_TX) ||
            ((cellRecfg->rachRecfg.numRaPreamble < RGSCH_MIN_NUM_RA_PREAMBLE)
            || (cellRecfg->rachRecfg.numRaPreamble > RGSCH_MAX_NUM_RA_PREAMBLE))
            || (cellRecfg->rachRecfg.sizeRaPreambleGrpA >
               cellRecfg->rachRecfg.numRaPreamble) ||
            (cellRecfg->rachRecfg.prachResource >
               (*cell)->bwCfg.ulTotalBw - RGSCH_NUM_RA_RB))
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid RACH configuration:"
               " preamble Fmt %d raWinSize %d maxMsg3Tx %d numRaPreamble %d"
               " sizeRaPreambleGrpA %d\n",
               cellRecfg->rachRecfg.preambleFormat,
               cellRecfg->rachRecfg.raWinSize,
               cellRecfg->rachRecfg.maxMsg3Tx,
               cellRecfg->rachRecfg.numRaPreamble,
               cellRecfg->rachRecfg.sizeRaPreambleGrpA));
         RETVALUE(RFAILED);
      }
   }

   /* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
   if (cellRecfg->recfgTypes & RGR_CELL_SI_RECFG)
   {
      if ((rgSCHCfgVldtRgrCellSiCfg(inst, &(cellRecfg->siReCfg))) != ROK)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Validation for SI"
                          "Re-configuration failed\n"));
         RETVALUE(RFAILED);
      }
   }
#endif /*RGR_SI_SCH */

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR cell reconfig validation done:"
           " cellId %d\n", cellRecfg->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCellRecfg */

/**
 * @brief Validates the UE reconfiguration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeRecfg
 *
 *     Processing Steps:
 *      - Retrieve the UE control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell and ue.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  RgrUeRecfg    *ueRecfg
 *  @param[out] RgSchCellCb   **cell
 *  @param[out] RgSchUeCb     **ue
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeRecfg
(
Inst           inst,
RgrUeRecfg     *ueRecfg,
RgSchCellCb    **cell,
RgSchUeCb      **ue,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrUeRecfg(inst, ueRecfg, cell, ue, errInfo)
Inst           inst;
RgrUeRecfg     *ueRecfg;
RgSchCellCb    **cell;
RgSchUeCb      **ue;
RgSchErrInfo   *errInfo;
#endif
{

   TRC2(rgSCHCfgVldtRgrUeRecfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATE RGR UE RECONFIG: cellId %d "
            "oldUeId %d cell %p ue %p\n", ueRecfg->cellId, ueRecfg->oldCrnti, 
            (Void*)*cell, (Void*)*ue));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_RECFG;
   
   if (((*cell) == NULLP) ||
       ((*cell)->cellId != ueRecfg->cellId))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist %d\n", 
            ueRecfg->cellId));
      MSPD_ERR("Cell does not exist %d\n", ueRecfg->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Old Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, ueRecfg->oldCrnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Old Ue does not exist %d\n", 
            ueRecfg->oldCrnti));
      MSPD_ERR("Old Ue does not exist %d\n", 
            ueRecfg->oldCrnti);
      RETVALUE(RFAILED);
   }

   if (ueRecfg->oldCrnti != ueRecfg->newCrnti)
   {
      if (rgSCHDbmGetUeCb(*cell, ueRecfg->newCrnti) != NULLP)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "New Ue already exists %d\n", 
              ueRecfg->newCrnti));
         MSPD_ERR("New UE already exists %d\n", ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }
   }

   /* Validate values */
   if ((ueRecfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG) &&
         (ueRecfg->txMode.pres == TRUE) &&
         ((ueRecfg->txMode.txModeEnum < RGR_UE_TM_1) ||
          (ueRecfg->txMode.txModeEnum > RGR_UE_TM_7)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid transmission mode for"
            " UE %d\n", (U8)ueRecfg->txMode.txModeEnum));
      MSPD_ERR("Invalid transmission mode for"
            " UE %d\n", (U8)ueRecfg->txMode.txModeEnum);
      RETVALUE(RFAILED);
   }
#ifndef TFU_UPGRADE
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG) &&
         (((ueRecfg->prdDlCqiRecfg.k < 1) || (ueRecfg->prdDlCqiRecfg.k > 4)) ||
         ((ueRecfg->prdDlCqiRecfg.cqiPmiCfgIdx < 1) || 
         (ueRecfg->prdDlCqiRecfg.cqiPmiCfgIdx > 1024))))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),"Invalid Periodic CQI Info \
                  for UE %d\n",(U8)ueRecfg->oldCrnti));
      MSPD_ERR("Invalid Periodic CQI Info \
                  for UE %d\n",(U8)ueRecfg->oldCrnti);                  
      RETVALUE(RFAILED);
   }
#endif
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ULHARQ_RECFG) &&
         (ueRecfg->ueUlHqRecfg.maxUlHqTx < RGSCH_MIN_HQ_TX))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid Uplink HARQ config for"
          " UE %d\n", ueRecfg->ueUlHqRecfg.maxUlHqTx));
      MSPD_ERR("Invalid Uplink HARQ config for"
          " UE %d\n", ueRecfg->ueUlHqRecfg.maxUlHqTx);
      RETVALUE(RFAILED);
   }
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG) &&
         (ueRecfg->prdDlCqiRecfg.prdModeEnum > RGR_PRD_CQI_MOD21))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid periodic mode config for"
          " DL CQI for UE %d\n", (U8)ueRecfg->prdDlCqiRecfg.prdModeEnum));
      MSPD_ERR("Invalid periodic mode config for"
          " DL CQI for UE %d\n", (U8)ueRecfg->prdDlCqiRecfg.prdModeEnum);
      RETVALUE(RFAILED);
   }
#else
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG) &&
       (ueRecfg->cqiCfg.cqiSetup.prdModeEnum > RGR_PRD_CQI_MOD21))
   {
     RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid periodic mode config for "
            "DL CQI for UE %d\n",(U8)ueRecfg->cqiCfg.cqiSetup.prdModeEnum));
     MSPD_ERR("Invalid periodic mode config for "
            "DL CQI for UE %d\n",(U8)ueRecfg->cqiCfg.cqiSetup.prdModeEnum);
     RETVALUE(RFAILED);
   }
#endif /* TFU_UPGRADE */
   if ((ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG) && 
         (ueRecfg->aprdDlCqiRecfg.aprdModeEnum > RGR_APRD_CQI_MOD31))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid aperiodic mode config for"
          " DL CQI for UE %d\n", (U8)ueRecfg->aprdDlCqiRecfg.aprdModeEnum));
      MSPD_ERR("Invalid aperiodic mode config for"
          " DL CQI for UE %d\n", (U8)ueRecfg->aprdDlCqiRecfg.aprdModeEnum);
      RETVALUE(RFAILED);
   }
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ULPWR_RECFG) &&
         (rgSCHCfgVldtUePwrCfg(*cell, &ueRecfg->ueUlPwrRecfg) != ROK))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid PUSCH Group power"
           " Reconfiguration\n"));
      MSPD_ERR("Invalid PUSCH Group power"
           " Reconfiguration\n");
      RETVALUE(RFAILED);
   }
#ifdef LTEMAC_SPS
   if(rgSCHCfgVldtSpsReCfg(*cell, *ue, ueRecfg)!= ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid SPS"
           " Reconfiguration\n"));
      MSPD_ERR("Invalid SPS Reconfiguration\n");
      RETVALUE(RFAILED);
   }
#endif 
   /* After merging from 2.2 */
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ACKNACK_MEASGAP_RECFG) &&
      (rgSCHCfgVldtUeMeasGapAckNakRepRecfg(*cell, ueRecfg) != ROK))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid MeasGap/AckNackRep"
            " Reconfiguration\n"));
      MSPD_ERR("Invalid MeasGap/AckNackRep Reconfiguration\n");
      RETVALUE(RFAILED);
   }
#ifdef TFU_UPGRADE
   /* Validated Periodic CQI/PMI, RI , SRS and SR related UeCfg */
   if ( ROK != rgSCHCfgVldtCqiSrSrsUeReCfg(*cell, *ue, ueRecfg,  errInfo))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid ACQI, PCQI/SR/SRS "
               "Re-configuration\n"));
      MSPD_ERR("Invalid ACQI, PCQI/SR/SRS "
               "Re-configuration\n");
      RETVALUE(RFAILED);
   }
#endif
#ifdef RGR_V2
#ifdef LTEMAC_DRX
   if ((ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG) &&
      (rgSCHCfgVldtDrxUeCfg(*cell, &(ueRecfg->ueDrxRecfg)) != ROK))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid drxParams"
            " Reconfiguration\n"));
      MSPD_ERR("Invalid drxParams Reconfiguration\n");
      RETVALUE(RFAILED);
   }
#endif /*LTEMAC_DRX*/

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* Validate DL Power Control Config parameters */
   if(rgSCHCfgVldtCqiReptReCfg(*cell, ueRecfg)!= ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid DL Power Control"
           " Reconfiguration\n"));
      MSPD_ERR("Invalid DL Power Control Reconfiguration\n");
      RETVALUE(RFAILED);
   }
#endif /* End of RGR_CQI_REPT */
#endif
   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR Ue reconfig validation done: "
       "cellId %d oldUeId %d\n", ueRecfg->cellId, ueRecfg->oldCrnti));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrUeRecfg */


/**
 * @brief Validates the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrLchRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  RgrLchRecfg     *lcRecfg
 *  @param[out] RgSchCellCb     **cell
 *  @param[out] RgSchUeCb       **ue
 *  @param[out] RgSchUlLcCb     **ulLc
 *  @param[out] RgSchDlLcCb     **dlLc
 *  @param[out] RgSchErrInfo       *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrLchRecfg
(
Inst           inst,
RgrLchRecfg    *lcRecfg,
RgSchCellCb    **cell,
RgSchUeCb      **ue,
RgSchDlLcCb    **dlLc,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrLchRecfg(inst, lcRecfg, cell, ue, dlLc, errInfo)
Inst            inst;
RgrLchRecfg     *lcRecfg;
RgSchCellCb     **cell;
RgSchUeCb       **ue;
RgSchDlLcCb     **dlLc;
RgSchErrInfo    *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrLchRecfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATE RGR LC RECONFIG: cellId %d "
        " ueId %d lcId %d cell %p ue %p dlLc %p ",lcRecfg->cellId,
        lcRecfg->crnti,lcRecfg->lcId, (void*)*cell, (void*)*ue,
        (void*)*dlLc));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_LC_RECFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcRecfg->cellId))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist %d\n", 
           lcRecfg->cellId));
      MSPD_ERR("Cell does not exist %d\n", 
           lcRecfg->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcRecfg->crnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Ue does not exist for dedicated"
         " logical channel %d\n", lcRecfg->crnti));
      MSPD_ERR("Ue does not exist for dedicated"
         " logical channel %d\n", lcRecfg->crnti);
      RETVALUE(RFAILED);
   }
   
   if ((*dlLc = rgSCHDbmGetDlDedLcCb((*ue), lcRecfg->lcId)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Dedicated DL LC does not "
               "exist %d\n", lcRecfg->lcId));
      MSPD_ERR("Dedicated DL LC does not exist %d\n", lcRecfg->lcId);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR Lc Reconfig validation done: "
         "cellId %d ueId %d lcId %d\n", lcRecfg->cellId, lcRecfg->crnti,
         lcRecfg->lcId));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLchRecfg */

/**
 * @brief Validates the UE Reset request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeReset
 *
 *     Processing Steps:
 *      - Retrieve the CELL control block
 *      - If cell does not exist return RFAILED
 *      - Retrieve UE Control block
 *      - If UE does not exist return RFAILED
 *      - Return ROK
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgrRst      *reset
 *  @param[out] RgSchCellCb **cell
 *  @param[out] RgSchUeCb      **ue
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeReset
(
Inst         inst,
RgrRst       *reset,
RgSchCellCb  *cell,
RgSchUeCb    **ue,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrUeReset(inst, reset, cell, ue, errInfo)
Inst         inst;
RgrRst       *reset;
RgSchCellCb  *cell;
RgSchUeCb    **ue;
RgSchErrInfo *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrUeReset);
   
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATE RGR UE RESET: cellId %d"
            "UeId %d\n",reset->cellId, reset->crnti));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_RESET;
   
   if ((cell == NULLP) || (cell->cellId != reset->cellId))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "CELL does not exist CELLID %d\n",
               reset->cellId));
      MSPD_ERR("CELL does not exist CELLID %d\n",
               reset->cellId);
      RETVALUE(RFAILED);
   }
   /* Fetch the Ue  */
   if ((*ue = rgSCHDbmGetUeCb(&(*cell), reset->crnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "UE does not exist %d\n",
               reset->crnti));
      MSPD_ERR("UE does not exist %d\n",
               reset->crnti);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR UE reset validation done: cellId"
            "%d UeId %d\n",reset->cellId, reset->crnti));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrUeReset */
   

/**
 * @brief Validates the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrLcgRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  RgrLchRecfg     *lcRecfg
 *  @param[out] RgSchCellCb     **cell
 *  @param[out] RgSchUeCb       **ue
 *  @param[out] RgSchErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrLcgRecfg
(
Inst           inst,
RgrLcgRecfg    *lcgRecfg,
RgSchCellCb    *cell,
RgSchUeCb      **ue,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrLcgRecfg(inst, lcgRecfg, cell, ue, errInfo)
Inst            inst;
RgrLcgRecfg     *lcgRecfg;
RgSchCellCb     *cell;
RgSchUeCb       **ue;
RgSchErrInfo    *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrLcgRecfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATE RGR LCG RECONFIG: cellId %d "
        " ueId %d lcgId %d cell %p ue %p ",lcgRecfg->cellId, lcgRecfg->crnti,
        lcgRecfg->ulRecfg.lcgId, (void*)cell, (void*)*ue));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_LCG_RECFG;

   if (((cell) == NULLP) ||
       ((cell)->cellId != lcgRecfg->cellId))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist %d\n", 
           lcgRecfg->cellId));
      MSPD_ERR("Cell does not exist %d\n", 
           lcgRecfg->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgSCHDbmGetUeCb(&(*cell), lcgRecfg->crnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Ue does not exist for dedicated"
         " logical channel group %d\n", lcgRecfg->crnti));
      MSPD_ERR("Ue does not exist for dedicated"
         " logical channel group %d\n", lcgRecfg->crnti);
      RETVALUE(RFAILED);
   }
   if (lcgRecfg->ulRecfg.lcgId > (RGSCH_MAX_LCG_PER_UE - 1))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid lcgId for uplink logical"
               " channel: lcg %d \n", lcgRecfg->ulRecfg.lcgId));
      MSPD_ERR("Invalid lcgId for uplink logical"
               " channel: lcg %d \n", lcgRecfg->ulRecfg.lcgId);
      RETVALUE(RFAILED);
   }
   
   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR Lcg Reconfig validation done: "
         "cellId %d ueId %d lcgId %d\n", lcgRecfg->cellId, lcgRecfg->crnti,
         lcgRecfg->ulRecfg.lcgId));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLcgRecfg */


/**
 * @brief Handler for the cell configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellCfg
 *
 *     Processing Steps:
 *      - Invoke SCH with cell control block to update
 *        scheduler specific information.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - Add to the active list of cells if cell becomes ACTIVE.
 *      - If successful, return ROK else RFAILED.
 *      
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgSchCellCfg    *cellCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrCellCfg
(
RgSchCb       *instCb,
SpId          spId,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrCellCfg(instCb, spId, cellCfg, errInfo)
RgSchCb       *instCb;
SpId          spId;
RgrCellCfg    *cellCfg;
RgSchErrInfo  *errInfo;
#endif
{
   S16          ret;
   U8           idx;
   Pst          pst;
   RgInfCellReg cellRegReq;
   RgSchCellCb  *cell = NULLP;
   Inst         inst = instCb->rgSchInit.inst;

   TRC2(rgSCHCfgRgrCellCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR CELL CONFIG: cellId %d "
         "cell %p\n", cellCfg->cellId, (void*)cell));

   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_CFG;
   
   cmMemset((U8*)&pst, (U8)0, sizeof(Pst));
    
   /* Allocate the scheduler's cell control block */
   if((ret = rgSCHUtlAllocSBuf(inst, (Data**)&cell, sizeof(RgSchCellCb))) 
         != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),"Memory allocation FAILED for cell %d",
               cellCfg->cellId));
      MSPD_ERR("Memory allocation FAILED for cell %d",
               cellCfg->cellId);
      RETVALUE(RFAILED);
   }
   if ((U8 *)cell == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),"Memory allocation FAILED for cell %d",
               cellCfg->cellId));
      MSPD_ERR("Memory allocation FAILED for cell %d",
               cellCfg->cellId);
      RETVALUE(RFAILED);
   }


   /* Initialize the lists of the cell */
   ret = rgSCHDbmInitCell(cell);
   if (ret != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "DBM initialization for cell %d"
          " failed\n", cellCfg->cellId));
      MSPD_ERR("DBM initialization for cell %d"
          " failed\n", cellCfg->cellId);
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }

   /* Initialize the cell */
   cell->cellId = cellCfg->cellId;
   cell->instIdx = inst;
   cell->macInst = cellCfg->macInst;
   cell->isCpUlExtend = cellCfg->isCpUlExtend;
   cell->isCpDlExtend = cellCfg->isCpDlExtend;
   cell->numTxAntPorts = cellCfg->numTxAntPorts;
/* rg004.201 patch - ccpu00109921 fix start */
   if(cell->numTxAntPorts == 1)
     cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_ONE_ANT_PORT;
   else if(cell->numTxAntPorts == 2)
     cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_TWO_ANT_PORT;
   else
     cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_FOUR_ANT_PORT;
   /* rg004.201 patch - ccpu00109921 fix end */
   cell->bwCfg = cellCfg->bwCfg;
/* rg004.201 patch - ccpu00109921 fix start */
   cell->pbchRbStart = ((((cell->bwCfg.dlTotalBw * 12)/2) - 36)/12); /* Ref section 6.6 in 36.211 */ 
   cell->pbchRbEnd = cell->pbchRbStart + 5;
   /* rg004.201 patch - ccpu00109921 fix end */
   cell->pucchCfg = cellCfg->pucchCfg;
   cell->rachCfg = cellCfg->rachCfg;
   cell->siCfg = cellCfg->siCfg;
   /* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
   /*Initialize the SI CB in Cell CB */
   cmMemset((U8 *)&cell->siCb, 0, sizeof(RgSchSiCb));
   /*Initialize the linked list */
   cmLListInit(&cell->siCb.siCtxLst);
#endif
   /*MS_FIX: Added Guard Pool for RNTI which will contain RNTIs 
       *for UEs deleted from Scheduler but not yet from MAC*/
   cmLListInit(&cell->rntiDb.rntiGuardPool);
   cell->macPreambleSet = cellCfg->macPreambleSet;
   cell->phichCfg = cellCfg->phichCfg;

   /* Initialize UL and DL CCCH logical channels */
   cell->ulCcchId = RGSCH_INVALID_LC_ID;
   cell->dlCcchId = RGSCH_INVALID_LC_ID;

   /* Update SRS configuration */
   cell->srsCfg.srsCfgPrdEnum = cellCfg->srsCfg.srsCfgPrdEnum;
   cell->srsCfg.srsBwEnum = cellCfg->srsCfg.srsBwEnum;
   cell->srsCfg.srsTxOffst =
      rgSrsTxOffstTbl[cellCfg->srsCfg.srsSubFrameCfg];
   /*ccpu00116923 - ADD - Srs Present support */
#ifdef TFU_UPGRADE
      cell->srsCfg.srsSubFrameCfg = cellCfg->srsCfg.srsSubFrameCfg;
#endif

 
   /* Update scheduler type */
   cell->ulSchdType = cellCfg->ulSchdType;
   cell->dlSchdType = cellCfg->dlSchdType;

   /* Configure all the common logical channels for the cell */
   for(idx = 0; idx < cellCfg->numCmnLcs; idx++)
   {
      /* This never returns failure and hence not checked for */
      rgSCHCfgRgrCmnLcCfg(cell, &(cellCfg->cmnLcCfg[idx]), errInfo);
   }

   /* Invoke the MeasGap and ACK NACK Rep handler for cell cfg */

   /* Invoke scheduler to update scheduler specific information */
   ret =  rgSCHUtlRgrCellCfg(cell, cellCfg, errInfo);
   if (ret != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Rgr cell Config failed at Scheduler"
       " for cell %d\n", cell->cellId));
      MSPD_ERR("Rgr cell Config failed at Scheduler"
       " for cell %d\n", cell->cellId);
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }

   /* Invoke DHM to update DHM specific information */
   rgSCHDhmRgrCellCfg(cell, cellCfg, errInfo);
   
   /* Initialize RNTI DB */
   ret = rgSCHDbmRntiDbInit(cell, cellCfg->macRnti.startRnti,
         cellCfg->macRnti.size);
   if (ret != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Rgr Cell Config failed at RNTI DB"
          " init for cell %d\n", cell->cellId));
      MSPD_ERR("Rgr Cell Config failed at RNTI DB"
          " init for cell %d\n", cell->cellId);
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }

   /* Update the cell with recieved configuration */
   cell->dlHqCfg  =  cellCfg->dlHqCfg;

   cell->crntSfIdx = 0; 
   /* Allocate the subframe allocation information */
   if((ret = rgSCHUtlGetSfAlloc(cell)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for "
               "cell %d",cellCfg->cellId));
      MSPD_ERR("Memory allocation FAILED for "
               "cell %d",cellCfg->cellId);
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }
   /* Update RACH Related information 
    * XXX: Below function yet to be written in RAM
    * To store the preambles given in  the configuration for PDCCH order in the
    * scheduler cell control block. Initialize the PRACH Mask Index allocated
    * for these preambles to invalid values */

   cell->crntHqIdx = 0; 
   /* Allocate the subframe allocation information */
   if((ret = rgSCHUtlGetRlsHqAlloc(cell)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for"
               "cell %d",cellCfg->cellId));
      MSPD_ERR("Memory allocation FAILED for"
               "cell %d",cellCfg->cellId);
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }

   /* Associate a pair of upper and lower sapCbs with this cell */
   instCb->rgrSap[spId].cell = cell;
   instCb->tfuSap[spId].cell = cell;
   cell->tfuSap = &(instCb->tfuSap[spId]);
   /* rg001.201: Added for sending TTI tick to RRM */
#if (defined(RGR_RRM_TICK) || defined(RGR_CQI_REPT))
   /* Associate the RGR SAP as well utilized while sending TTI
    * Ticks to RGR User. */
   cell->rgrSap = &(instCb->rgrSap[spId]);
#endif
#ifdef RGR_RRM_TICK
   /* Store the periodicity configured */
   cell->rrmTtiIndPrd = cellCfg->rrmTtiIndPrd;
#endif

#ifdef LTE_L2_MEAS
   cmLListInit(&cell->l2mList);
#endif

#ifdef LTEMAC_DRX
   if (rgSCHDrxCellCfg(cell,cellCfg) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),"Drx Memory allocation FAILED for\
                                  cell %d",cellCfg->cellId));
      MSPD_ERR("DRX Memory allocation FAILED for\
                                  cell %d",cellCfg->cellId);                               
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }
#endif /*LTEMAC_DRX*/

   /* Register the cell with MAC */ 
   rgSCHUtlGetPstToLyr(&pst, instCb, cell->macInst);
   cellRegReq.cellId = cell->cellId;
   cellRegReq.cellSapId = spId;
   /* rg001.201: maxDlHqProcPerUe was incorrectly filled for TDD */
#ifdef LTE_TDD
   cellRegReq.maxDlHqProcPerUe = rgSchTddDlNumHarqProcTbl[cellCfg->ulDlCfgIdx];
#else
   cellRegReq.maxDlHqProcPerUe = RGSCH_MAX_DL_HQ_PROC;
#endif
   RgSchMacCellReg(&pst, &cellRegReq);

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR cell config done: Cell %d\n", 
      cell->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrCellCfg */

/**
 * @brief Handler for the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeCfg
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
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrUeCfg
(
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrUeCfg(cell, ueCfg, errInfo)
RgSchCellCb  *cell;
RgrUeCfg     *ueCfg;
RgSchErrInfo *errInfo;
#endif
{
   S16          ret;
   RgSchRaCb    *raCb=NULLP;
   RgSchUeCb    *ue = NULLP;
   Inst         inst = cell->instIdx;
   U32          lcgCnt;

   TRC2(rgSCHCfgRgrUeCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR UE CONFIG: cellId %d "
      "ueId %d cell %p\n", ueCfg->cellId, ueCfg->crnti, (void*)cell));
#ifdef MSPD
  MSPD_DBG("--------------------------------------------------------------------\n");
  MSPD_DBG("UE Configuration at MAC: rnti (%u) cell(%u)\n", ueCfg->crnti, ueCfg->cellId);
  MSPD_DBG("--------------------------------------------------------------------\n");
#endif
   errInfo->errCause = RGSCHERR_CFG_RGR_UE_CFG;
   /* RACHO : Check for raCb only if preamble Id not provded */
   if (ueCfg->dedPreambleId.pres == NOTPRSNT)
   {
      if ((raCb = rgSCHDbmGetRaCb(cell, ueCfg->crnti)) == NULLP)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "No RaCb exists for the UE %d\n",
                  ueCfg->crnti));
         MSPD_ERR( "No RaCb exists for the UE %d\n", ueCfg->crnti);
         RETVALUE(RFAILED);
      }
   }

   /* Allocate the Ue control block */
   if ((rgSCHUtlAllocSBuf(inst, (Data **)&ue, sizeof(RgSchUeCb))) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for ue %d",
               ueCfg->crnti));
      MSPD_ERR("Memory allocation FAILED for ue %d",
               ueCfg->crnti);
      RETVALUE(RFAILED);
   }
   if ((U8 *)ue == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for ue %d",
               ueCfg->crnti));
      MSPD_ERR("Memory allocation FAILED for ue %d",
               ueCfg->crnti);
      RETVALUE(RFAILED);
   }

   /* Inititialize Ue control block */
   ue->ueId = ueCfg->crnti;
   ue->cell = cell;
   /*ccpu00117778- Initialize Transmission Indices upon UE CB creation */
#ifdef TFU_UPGRADE
   ue->cqiCb.nCqiTrIdx = RG_SCH_INVALID_IDX;
   ue->cqiCb.nRiTrIdx = RG_SCH_INVALID_IDX;
   ue->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
   ue->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
#endif


   /* Initialize the lists of the UE */
   if((rgSCHDbmInitUe(ue)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "DBM initialization for UE %d "
           "failed\n", ueCfg->crnti));
      MSPD_ERR( "DBM initialization for UE %d "
           "failed\n", ueCfg->crnti);
      rgSCHCfgFreeUeCb(cell, ue);
      RETVALUE(RFAILED);
   }
   /* rg001.201: Moved UHM init before RAM processing */
   ret = rgSCHUhmHqEntInit(ue);
   /*rg003.201: TDD flag added  */
   if (ret != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "UHM HARQ Ent Init Failed"
         "for UE %d\n", ueCfg->crnti));
      MSPD_ERR("UHM HARQ Ent Init Failed"
         "for UE %d\n", ueCfg->crnti);
      rgSCHCfgFreeUeCb(cell, ue);
      RETVALUE(RFAILED);
   }
   /* Initialize RAM related information for UE 
    * RACHO: if preamble Id is present in ueCfg then raCb will be NULL
    * so rgSCHRamRgrUeCfg should take care of creating raCb */
   if (ueCfg->dedPreambleId.pres == NOTPRSNT)
   {
      if((rgSCHRamRgrUeCfg(cell, ue, raCb, errInfo)) != ROK)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Random access handling for UE"
           " config failed %d\n", ueCfg->crnti));
         MSPD_ERR("Random access handling for UE"
           " config failed %d\n", ueCfg->crnti);
         rgSCHCfgFreeUeCb(cell, ue);
         RETVALUE(RFAILED);
      }
   }
   else /* if HO Ue */
   {
      ue->dl.hqEnt = rgSCHDhmHqEntInit(cell);
      if (ue->dl.hqEnt == NULLP)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Hq Entity Initialization for UE"
           " config failed %d\n", ueCfg->crnti));
         MSPD_ERR("Hq Entity Initialization for UE"
           " config failed %d\n", ueCfg->crnti);
         rgSCHCfgFreeUeCb(cell, ue);
         RETVALUE(RFAILED);
      }
      ue->dl.hqEnt->ue = ue;
      /* MS_FIX : syed Assign hqEnt to UE only if msg4 is done */
      rgSCHCmnDlInitHqEnt(cell, ue);
   }
#ifdef MSPD
   MSPD_DBG("TFU_HQFBKIND_ULDELTA %u DL Tx Count %u for Ue %u \n", 
   TFU_HQFBKIND_ULDELTA, ue->dl.hqEnt->maxHqTx, ue->ueId);
#endif

   /* Initialize lcgIds to Invalid */
   for (lcgCnt = 0; lcgCnt < RGSCH_MAX_LCG_PER_UE; lcgCnt++)
   {
      ue->ul.lcgArr[lcgCnt].lcgId = RGSCH_INVALID_LCG_ID;
   }
   /* Initialize scheduler related information for UE */
   if(rgSCHUtlRgrUeCfg(cell, ue, ueCfg, errInfo) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Scheduler handling for UE config"
         " failed %d\n", ueCfg->crnti));
      MSPD_ERR("Scheduler handling for UE config"
         " failed %d\n", ueCfg->crnti);
      rgSCHCfgFreeUeCb(cell, ue);
      RETVALUE(RFAILED);
   }
   
   /* rg004.201: ccpu00105914: PHR handling for MSG3 */
   if(raCb != NULLP)
   {
      rgSCHCfgRgrUePhrMsg3(cell,raCb,ue,errInfo);
      /* Moved this code out of rgSCHCfgRgrUePhrMsg3()
       * as it was not the appropriate place to 
       * do this. */
      if (raCb->raState == RGSCH_RA_MSG4_DONE)
      {
         RGSCHDBGINFO(inst, (rgSchPBuf(inst), 
            "rgSCHCfgRgrUeCfg(): RaCb with rnti=%u deleted as Msg4 "
            "transmission is done\n", raCb->tmpCrnti));
         MSPD_DBG("RaCb with rnti=%u deleted as Msg4 "
            "transmission is done\n", raCb->tmpCrnti);		 
         rgSCHRamDelRaCb(cell, raCb, FALSE);
      }
   }
   /* Initialize uplink HARQ related information for UE */
   rgSCHUhmRgrUeCfg(cell, ue, ueCfg);
   /* MS_WORKAROUND for CR FIXME */
   cmInitTimers(&ue->bsrTmr, 1);
#ifdef RGR_V1
   /* rg006.201: [ccpu00112398] Added periodic BSR timer */
   cmInitTimers(&ue->bsrTmr, 1);
#ifndef MSPD
   ue->ul.bsrTmrCfg = ueCfg->ueBsrTmrCfg;
   if ((ue->ul.bsrTmrCfg.isPrdBsrTmrPres) && 
         (ue->ul.bsrTmrCfg.prdBsrTmr == 0xFFFF))
   {
      ue->ul.bsrTmrCfg.isPrdBsrTmrPres = FALSE;
   }
#else
   MSPD_DBG("Configure BSR Timer, pres=%d, prdBsrTmr=%d, retxBsrTmr=%d\n", 
   ueCfg->ueBsrTmrCfg.isPrdBsrTmrPres,
   ueCfg->ueBsrTmrCfg.prdBsrTmr, ueCfg->ueBsrTmrCfg.retxBsrTmr);

   /* MS_FIX - Added proper configuration from U-ARM */
   if(ueCfg->ueBsrTmrCfg.isPrdBsrTmrPres == TRUE)
   {
      ue->ul.bsrTmrCfg.isPrdBsrTmrPres = TRUE;
      ue->ul.bsrTmrCfg.prdBsrTmr       = ueCfg->ueBsrTmrCfg.prdBsrTmr;
      ue->ul.bsrTmrCfg.retxBsrTmr      = ueCfg->ueBsrTmrCfg.retxBsrTmr;
   }
#endif
#endif
   /* Initialize downlink HARQ related information for UE */
   rgSCHDhmRgrUeCfg(cell, ue, ueCfg, errInfo);
   
   /* Initialize MeasureGap and Acknack Rep Information for UE */
   if((rgSCHMeasGapANRepUeCfg(cell, ue, ueCfg)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Measurement Gap and AckNack Rep"
         " Config failed for UE %d\n", ueCfg->crnti));
      MSPD_ERR("Measurement Gap and AckNack Rep"
         " Conig failed for UE %d\n", ueCfg->crnti);
      rgSCHCfgFreeUeCb(cell, ue);
      RETVALUE(RFAILED);
   }


#ifdef LTE_TDD
   if((rgSCHUtlAllocUeANFdbkInfo(ue)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memomy allocation Failed for"
         "UE related Ack Nack Information%d\n", ueCfg->crnti));
      MSPD_ERR("Memomy allocation Failed for"
         "UE related Ack Nack Information%d\n", ueCfg->crnti);
      rgSCHCfgFreeUeCb(cell, ue);
      RETVALUE(RFAILED);
   }
   ue->dl.ackNackMode = ueCfg->ackNackModeEnum;
#endif /* LTE_TDD */

   /* Insert Ue */
   rgSCHDbmInsUeCb(cell, ue);

/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   /*  Intialize APeriodic CQI/PMI/RI Information for UE */
   ret = rgSCHCfgACqiUeCfg(cell, ue, &ueCfg->ueDlCqiCfg.aprdCqiCfg, 
         ue->ueCatEnum);

   ue->cqiRiWritIdx = 0;
   ue->cqiRiReadIdx = 0;
   /*   Initialize Periodic CQI/PMI, RI Information for UE  */
   ret = rgSCHCfgPCqiUeCfg(cell, ue, &ueCfg->ueDlCqiCfg.prdCqiCfg, 
         ue->ueCatEnum);

   /*   Initialize UL SRS Information for UE  */
   ret = rgSCHCfgSrsUeCfg(cell, ue, &ueCfg->srsCfg);

   /*   Initialize SR Information for UE  */
   ret = rgSCHCfgSrUeCfg(cell, ue, &ueCfg->srCfg);
#endif 

#ifdef LTEMAC_HDFDD
   if (rgSCHHdFddUeCfg(cell, ue, ueCfg->isHdFddEnbld) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "Could not do HD-FDD config for UE\n"));
      MSPD_ERR("Could not do HD-FDD config for UE\n");
      rgSCHCfgFreeUeCb(cell, ue);
      RETVALUE(RFAILED);
   }

#endif /* LTEMAC_HDFDD */
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   ue->cqiReptCfgInfo.numColltdCqiRept =
       ueCfg->ueCqiReptCfg.numColltdCqiRept;
#endif /* End of RGR_CQI_REPT */
#ifdef TFU_UPGRADE
   ue->pA.pres = FALSE;
   if (RG_SCH_UE_CFG_ISPAPRSNT(ueCfg->uePdschDedCfg.uepACfg))
   {
      ue->pA.pres = TRUE;
      ue->pA.val  = ueCfg->uePdschDedCfg.uepACfg.pA;
   }
#endif
#ifdef LTEMAC_DRX
   ue->isDrxEnabled = ueCfg->ueDrxCfg.isDrxEnabled;

   if ( ue->isDrxEnabled )
   {
       if((rgSCHDrxUeCfg(cell,ue,ueCfg)) != ROK )
       {
          RGSCHDBGERR(inst, (rgSchPBuf(inst), "DRX configuration failed\n"));
          RETVALUE(RFAILED);
       }
   }
#endif /* LTEMAC_DRX */

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Rgr Ue configuration done: cellId %d"
        " ueId %d\n", ueCfg->cellId, ueCfg->crnti));
#ifdef MSPD
  MSPD_DBG("--------------------------------------------------------------------\n");
  MSPD_DBG("UE Configuration at MAC done: rnti (%u) cell(%u)\n", ueCfg->crnti, ueCfg->cellId);
  MSPD_DBG("--------------------------------------------------------------------\n");
#endif
   RETVALUE(ROK);
}  /* rgSCHCfgRgrUeCfg */

/**
 * @brief Handler for PHR for MSG3.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUePhrMsg3
 *
 *     Processing Steps:
 *       Handle PHR related config for MSG3
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrUeCb     *ueCb
 *  @param[in]  RgSchRaCb    *raCb
 *  @param[out] RgSchErrInfo *errInfo
 **/
#ifdef ANSI
PRIVATE Void rgSCHCfgRgrUePhrMsg3
(
RgSchCellCb  *cell,
RgSchRaCb    *raCb,
RgSchUeCb    *ue,
RgSchErrInfo *errInfo
)
#else
PRIVATE Void rgSCHCfgRgrUePhrMsg3(cell, raCb, ue, errInfo)
RgSchCellCb  *cell;
RgSchRaCb    *raCb;
RgSchUeCb    *ue;
RgSchErrInfo *errInfo;
#endif
{

   TRC2(rgSCHCfgRgrUePhrMsg3);

   /* rg004.201: ccpu00105914: PHR handling for MSG3 */
   /* Record msg3 allocation in the UE */
   rgSCHUtlRecMsg3Alloc(cell, ue, raCb);

   /* If raCb received PHR, update scheduler */
   if(raCb->phr.pres == TRUE)
   {
      ue->macCeRptTime = raCb->msg3AllocTime; 
      rgSCHUtlUpdPhr(cell, ue, raCb->phr.val, errInfo);
   }

   RETVOID;
}

/**
 * @brief Handler for the cell reconfiguration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellRecfg
 *
 *     Processing Steps:
 *      - Invoke SCH with cell control block to update
 *        scheduler specific information.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCellRecfg  *cellRecfg
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrCellRecfg
(
RgSchCellCb   *cell,
RgrCellRecfg  *cellRecfg,
RgSchErrInfo  *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrCellRecfg(cell, cellRecfg, errInfo)
RgSchCellCb   *cell;
RgrCellRecfg  *cellRecfg;
RgSchErrInfo  *errInfo;
#endif
{
   S16      ret;
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   
   TRC2(rgSCHCfgRgrCellRecfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR CELL RECONFIG: cellId %d "
          "cell %p\n", cellRecfg->cellId, (void*)cell));

   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_RECFG;
   
   /* Invoke scheduler to update scheduler specific information */
   ret =  rgSCHUtlRgrCellRecfg(cell, cellRecfg, errInfo);
   if (ret != ROK)
   {
     RGSCHDBGERR(inst, (rgSchPBuf(inst), "RGR Cell re-configuration failed "
          "at Scheduler: cellId %d", cellRecfg->cellId));
     MSPD_ERR("RGR Cell re-configuration failed "
          "at Scheduler: cellId %d", cellRecfg->cellId);
      RETVALUE(RFAILED);
   }
   
   /* Invoke DHM to update DHM specific information */
   rgSCHDhmRgrCellRecfg(cell, cellRecfg, errInfo);

      /* PUCCH Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_PUCCH_RECFG)
   {
      cell->pucchCfg = cellRecfg->pucchRecfg;
   }
 
   /* SRS Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_SRS_RECFG)
   {
      cell->srsCfg.srsCfgPrdEnum = cellRecfg->srsRecfg.srsCfgPrdEnum;
      cell->srsCfg.srsBwEnum = cellRecfg->srsRecfg.srsBwEnum;
      cell->srsCfg.srsTxOffst =
         rgSrsTxOffstTbl[cellRecfg->srsRecfg.srsSubFrameCfg];
      /*ccpu00116923 - ADD - Srs Present support */
#ifdef TFU_UPGRADE
      cell->srsCfg.srsSubFrameCfg = cellRecfg->srsRecfg.srsSubFrameCfg;
#endif
   }

   /* RACH Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_RACH_RECFG)
   {
      cell->rachCfg = cellRecfg->rachRecfg;
   }


   /* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
   /* SI Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_SI_RECFG)
   {
      /*Set the specified SI configuration. */
      cell->siCb.newSiCfg = cellRecfg->siReCfg;
      /* Set the Bit mask for SI re-configuration */
      cell->siCb.siBitMask |= RGSCH_SI_SICFG_UPD; 
   }
#endif /*RGR_SI_SCH */

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR cell reconfig done: Cell %d\n",
                             cell->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrCellRecfg */

/**
 * @brief Handler for the UE reconfiguration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeRecfg
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
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrUeRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrUeRecfg(cell, ue, ueRecfg, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrUeRecfg     *ueRecfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16          ret;
   RgSchRaCb    *raCb;
   RgSchCmnCell *cellSch= (RgSchCmnCell *)(cell->sc.sch);
   Inst         inst = cell->instIdx;
   
   TRC2(rgSCHCfgRgrUeRecfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR UE RECONFIG: cellId %d"
           " oldUeId %d newUeId %d cell %p ue %p\n", ueRecfg->cellId,
           ueRecfg->oldCrnti, ueRecfg->newCrnti, (void*)cell, (void*)ue));

   errInfo->errCause = RGSCHERR_CFG_RGR_UE_RECFG;
#ifdef MSPD
  MSPD_DBG("--------------------------------------------------------------------\n");
  MSPD_DBG("UE Recfonfiguration at MAC : ueRecfg->oldCrnti (%u), ueRecfg->newCrnti (%u)"
  	"RecfgTypes(0x%0x)\n", ueRecfg->oldCrnti, ueRecfg->newCrnti, ueRecfg->ueRecfgTypes);
  MSPD_DBG("--------------------------------------------------------------------\n");
#endif
   if (ueRecfg->ueRecfgTypes)
   {
      /* Update scheduler related information for UE */
      ret = rgSCHUtlRgrUeRecfg(cell, ue, ueRecfg, errInfo);
      if (ret != ROK)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Scheduler handling for UE "
             "reconfig failed: oldUeId %d newUeId %d\n",
                  ueRecfg->oldCrnti, ueRecfg->newCrnti));
         MSPD_ERR("Scheduler handling for UE "
             "reconfig failed: oldUeId %d newUeId %d\n",
                  ueRecfg->oldCrnti, ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }

      /* Update uplink HARQ related information for UE */
      rgSCHUhmRgrUeRecfg(cell, ue, ueRecfg);

      /* Update downlink HARQ related information for UE */
      rgSCHDhmRgrUeRecfg(cell, ue, ueRecfg, errInfo);

      /*Update Measurement Gap and AckNack Details */
      /* After merging from 2.2 */
      if (ueRecfg->ueRecfgTypes & RGR_UE_ACKNACK_MEASGAP_RECFG)
      {
         ret = rgSCHMeasGapANRepUeRecfg(cell, ue, ueRecfg);
         if (ret != ROK)
         {
            RGSCHDBGERR(inst, (rgSchPBuf(inst), "Measurement Gap and"
                     "AckNack Rep Recfg failed: \n"));
            MSPD_ERR("Measurement Gap and"
                     "AckNack Rep Recfg failed: \n");
            RETVALUE(RFAILED);
         }
      }
#ifdef RGR_V1
      if (ueRecfg->ueRecfgTypes & RGR_UE_BSRTMR_RECFG)
      {
         /* rg006.201: [ccpu00112398] Added periodic BSR timer */
         cmInitTimers(&ue->bsrTmr, 1);
         ue->ul.bsrTmrCfg = ueRecfg->ueBsrTmrRecfg;
         if ((ue->ul.bsrTmrCfg.isPrdBsrTmrPres) && 
               (ue->ul.bsrTmrCfg.prdBsrTmr == 0xFFFF))
         {
            ue->ul.bsrTmrCfg.isPrdBsrTmrPres = FALSE;
         }
      }
#endif
   }

   /* Handle CRNTI change in reconfiguration */
   if (ueRecfg->oldCrnti != ueRecfg->newCrnti)
   {
      if ((raCb = rgSCHDbmGetRaCb(cell, ueRecfg->newCrnti)) == NULLP)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "No RaCb exists for UE "
              "Reconfig %d\n", ueRecfg->newCrnti));
         MSPD_ERR("No RaCb exists for UE "
              "Reconfig %d\n", ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }

      /* Release Older rnti */
      /* rntiLnk does not exist for a HandIn UE. Hence this check. */
      if(ue->rntiLnk)
         rgSCHDbmRlsRnti(cell, ue->rntiLnk);
      
      /* MS_FIX : syed Deleting Old DL HqEnt. It would be assigned after
       * reest RACH(msg4) is completed. */
      rgSCHDhmDelHqEnt(cell, &ue->dl.hqEnt);

      /* Initialize RAM related information for UE */
      ret = rgSCHRamRgrUeCfg(cell, ue, raCb, errInfo);
      if (ret != ROK)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "RAM Handling for UE Reconfig "
                "failed: oldUeId %d newUeId %d\n",
                 ueRecfg->oldCrnti, ueRecfg->newCrnti));
         MSPD_ERR("RAM Handling for UE Reconfig "
                "failed: oldUeId %d newUeId %d\n",
                 ueRecfg->oldCrnti, ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }
      /* Delete Ue from the ue list */
      rgSCHDbmDelUeCb(cell, ue);

      /* Inititialize Ue control block */
      ue->ueId = ueRecfg->newCrnti;
#ifdef MSPD
      MSPD_DBG("Changing RNTI from %d to %d\n", ueRecfg->oldCrnti, ueRecfg->newCrnti);
#endif
      rgSCHUtlUpdY(ue->ueId, ue->y);
	   /* MS_FIX ccpu00122631 */
      ue->dl.dlAllocCb.rnti = ueRecfg->newCrnti;

      /* rg004.201: ccpu00105914: PHR handling for MSG3 */
      rgSCHUtlRecMsg3Alloc(cell, ue, raCb);

      /* If raCb received PHR, update scheduler */
      if(raCb->phr.pres == TRUE)
      {
         ue->macCeRptTime = raCb->msg3AllocTime; 
         rgSCHUtlUpdPhr(cell, ue, raCb->phr.val, errInfo);
      }

      /* Re-insert updated Ue */
      rgSCHDbmInsUeCb(cell, ue);

      /* MS_FIX : syed If MSG4 is done, since corresponding ueCb
       * is ready, the raCb should be cleared immediately.
       * Otherwise it would remain in the cell until timed out
       * and till then the hq Feedbacks will be assumed to be
       * for msg4 */
      if (raCb->raState == RGSCH_RA_MSG4_DONE)
      {
         RGSCHDBGINFO(inst, (rgSchPBuf(inst), 
            "rgSCHCfgRgrUeRecfg(): RaCb with rnti=%u deleted as Msg4 "
            "transmission is done\n", raCb->tmpCrnti));
         MSPD_DBG("RaCb with rnti=%u deleted as Msg4 "
            "transmission is done\n", raCb->tmpCrnti);		 
         rgSCHRamDelRaCb(cell, raCb, FALSE);
      }
      /* MS_FIX : syed moving the UL CQI initialization to UERESET */
   }

#ifdef TFU_UPGRADE
   /* Re-Initialize Aperiodic CQI Information for UE*/
   if ( ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG ) 
   {
      ret = rgSCHCfgAcqiUeReCfg(cell, ue, &ueRecfg->aprdDlCqiRecfg, 
            ue->ueCatEnum);
   }
   /*   Re-Initialize Periodic CQI/PMI, RI Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_PCQI_RECFG) 
   {
      ret = rgSCHCfgPCqiUeReCfg(cell, ue, &ueRecfg->cqiCfg, 
            ue->ueCatEnum);
   }
   /*   Re-Initialize UL SRS Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_SRS_RECFG) 
   {
      ret = rgSCHCfgSrsUeReCfg(cell, ue, &ueRecfg->srsCfg);
   }
   /*   Re-Initialize SR Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_SR_RECFG) 
   {
      ret = rgSCHCfgSrUeReCfg(cell, ue, &ueRecfg->srCfg);
   }
#endif 

#ifdef LTEMAC_HDFDD  
   if(ueRecfg->isHdFddEnbld)
   {
        ret = rgSCHHdFddUeCfg(cell, ue, ueRecfg->isHdFddEnbld);
        if (ret != ROK)
        {
           errInfo->errCause = RGSCHERR_HDFDD_SPSCFGRD;
           RETVALUE(ret);
        }
   }
#endif /* LTEMAC_HDFDD */
#ifdef RGR_V2
#ifdef LTEMAC_DRX
  if ( ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG)
  {
     ret = rgSCHDrxUeReCfg(cell,ue,ueRecfg);  

     if ( ret != ROK )
     {
        RGSCHDBGERR(inst, (rgSchPBuf(inst), "UE DRX reconfig failed"));
        MSPD_ERR("UE DRX reconfig failed");
        RETVALUE(RFAILED);
     }
  }
#endif
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* CQI Reporting (N)  Re-configuration */
   if(ueRecfg->ueRecfgTypes & RGR_UE_CQIREPT_RECFG)
   {
      ret = rgSCHCfgUeCqiReptReCfg(cell, ue, ueRecfg);
      if(ret != OK)
      {
         errInfo->errCause = RGSCHERR_CQIREPT;
         RETVALUE(ret);
      }
   }
#endif /* End of RGR_CQI_REPT */
#ifdef TFU_UPGRADE
   /* pA Re-configuration */
   if((ueRecfg->ueRecfgTypes & RGR_UE_PA_RECFG) &&
         RG_SCH_UE_CFG_ISPAPRSNT(ueRecfg->uePdschDedCfg.uepACfg))
   {
      ue->pA.pres = TRUE;
      ue->pA.val  = ueRecfg->uePdschDedCfg.uepACfg.pA;
   }
#endif
#endif
   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Ue reconfigured: oldUeId %d"
        " newUeId %d cell %d\n",
         ueRecfg->oldCrnti, ueRecfg->newCrnti, ueRecfg->cellId));
#ifdef MSPD
   MSPD_DBG("--------------------------------------------------------------------\n");
   MSPD_DBG("UE Reconfiguration at MAC done: oldUeId %d"
        " newUeId %d \n", ueRecfg->oldCrnti, ueRecfg->newCrnti);
   MSPD_DBG("--------------------------------------------------------------------\n");
#endif
   RETVALUE(ROK);
}  /* rgSCHCfgRgrUeRecfg */


/**
 * @brief Handler for the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLchRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the reconfigured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  RgSchUlLcCb    *ulLc
 *  @param[in]  RgSchDlLcCb    *dlLc
 *  @param[in]  RgrLchRecfg *lcRecfg
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLchRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlLcCb    *dlLc,
RgrLchRecfg    *lcRecfg,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLchRecfg(cell, ue, dlLc, lcRecfg, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlLcCb    *dlLc;
RgrLchRecfg    *lcRecfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16     ret = ROK;
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgRgrLchRecfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR LC RECONFIG: cellId %d"
            " ueId %d lcId %d cell %p ue %p dlLc %p\n",
            lcRecfg->cellId, lcRecfg->crnti, lcRecfg->lcId, (void*)cell,
            (void*)ue, (void*)dlLc));

   errInfo->errCause = RGSCHERR_CFG_RGR_LC_RECFG;
   /* Invoke Scheduler to update the new configuration */
   ret = rgSCHUtlRgrLcRecfg(cell, ue, dlLc, lcRecfg, errInfo);
   if (ret != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Scheduler handling for LC Recfg"
              " failed: lcId %d\n", lcRecfg->lcId));
      MSPD_ERR("Scheduler handling for LC Recfg"
              " failed: lcId %d\n", lcRecfg->lcId);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "LC %d of Ue %d of cell %d "
             "Reconfigured\n", lcRecfg->lcId, ue->ueId, cell->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLchRecfg */
/**
 * @brief Handler for the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcgRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the re-configured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgUlCellCb   *cell
 *  @param[in]  RgUlUeCb     *ue
 *  @param[in]  RgrLcgRecfg  *lcgRecfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLcgRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrLcgRecfg    *lcgRecfg,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLcgRecfg(cell, ue, lcgRecfg, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrLcgRecfg    *lcgRecfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16     ret = ROK;
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgRgrLcgRecfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR LCG RECONFIG: cellId %d"
            " ueId %d lcgId %d cell %p ue %p \n",
            lcgRecfg->cellId, lcgRecfg->crnti, lcgRecfg->ulRecfg.lcgId,
            (void*)cell, (void*)ue));

   errInfo->errCause = RGSCHERR_CFG_RGR_LCG_RECFG;
   /* Invoke Scheduler to update the new configuration */
   ret = rgSCHUtlRgrLcgRecfg(cell, ue, lcgRecfg, errInfo);
   if (ret != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Scheduler handling for LCG Recfg"
              " failed: lcgId %d\n", lcgRecfg->ulRecfg.lcgId));
     MSPD_ERR( "Scheduler handling for LCG Recfg"
              " failed: lcgId %d\n", lcgRecfg->ulRecfg.lcgId);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "LCG %d of Ue %d of cell %d "
             "Reconfigured\n", lcgRecfg->ulRecfg.lcgId, ue->ueId, cell->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLcgRecfg */

/**
 * @brief Handler for the UE Reset request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeReset
 *
 *     Processing Steps:
 *     - Call Measument Gap Module and Ack/Nack Module for resetting UE.
 *     - Call Common Schduler UE rest API which inturn will call scheduler
 *     specific UE Reset APis to reset UE.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgSchUeCb      *ue
 *  @param[in]  RgrRst         *reset
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrUeReset
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrRst         *reset,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrUeReset(cell, ue, reset, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrRst         *reset;
RgSchErrInfo   *errInfo;
#endif
{
   U32     idx;
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgRgrUeReset);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR UE RESET: cellId %d"
            " ueId %d cell %p ue %p \n",reset->cellId, reset->crnti,
            (void*)cell, (void*)ue));

   errInfo->errCause = RGSCHERR_CFG_RGR_UE_RESET;
  
   /* Setting BO of Each Logical Channel of the UE to 0 */
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; idx++)
   {
      if(ue->dl.lcCb[idx] != NULLP)
         ue->dl.lcCb[idx]->bo = 0;
   }
#ifdef LTEMAC_DRX
      /* Call DRX module to stop all DRX timers
       */
      (Void)rgSCHDrxUeDel(cell,ue);
#endif /*LTEMAC_DRX*/

   /* Call Common scheduler which in turn will call specific scheduler for UE
    * Reset*/
   rgSCHUtlUeReset(cell, ue);
  
   rgSCHDhmHqEntReset(ue->dl.hqEnt);
   /* MS_FIX : syed set UE inactive in DL until UE is reinitialization completed */
   ue->dl.dlInactvMask |= RG_HQENT_INACTIVE;
   ue->ul.ulInactvMask |= RG_HQENT_INACTIVE;
   /* [ccpu00127141] Resetting TA related parameters */
   ue->dl.taCb.ta = RGSCH_NO_TA_RQD;
   ue->dl.taCb.state = RGSCH_TA_IDLE;
   if (ue->dl.taCb.cfgTaTmr)
   {
      rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_TA, ue->dl.taCb.cfgTaTmr);
   }

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Ue %d of cell %d "
             "RESET\n", ue->ueId, cell->cellId));
   MSPD_DBG("Ue %d of cell %d "
             "RESET\n", ue->ueId, cell->cellId);
   RETVALUE(ROK);
}  /* rgSCHCfgRgrUeReset */

/**
 * @brief Handler for the cell delete request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellDel
 *
 *     Processing Steps:
 *      - Fetch the cell control block.
 *      - Remove the cell control block from the hash list of cells.
 *      - Free the cell control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgrDel         *cellDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrCellDel
(
RgSchCellCb    *cell,
RgrDel         *cellDelInfo,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrCellDel(cell, cellDelInfo, errInfo)
RgSchCellCb    *cell;
RgrDel         *cellDelInfo;
RgSchErrInfo   *errInfo;
#endif
{ 
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   TRC2(rgSCHCfgRgrCellDel);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR CELL DELETE: cellId %d\n",
            cellDelInfo->u.cellDel.cellId));

   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_DEL;

   if (cell->cellId != cellDelInfo->u.cellDel.cellId)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist %d\n",
               cellDelInfo->u.cellDel.cellId));
      RETVALUE(RFAILED);
   }

   /* Free the active cell */
   rgSCHCfgFreeCellCb(cell);

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Cell %d deleted\n", 
                      cellDelInfo->u.cellDel.cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrCellDel */


/**
 * @brief Handler for the UE delete request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeDel
 *
 *     Processing Steps:
 *      - Fetch the UE control block.
 *      - Remove the UE control block from the hash list of UEs for the cell.
 *      - Free the UE control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgrDel         *ueDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrUeDel
(
RgSchCellCb         *cell,
RgrDel              *ueDelInfo,
RgSchErrInfo        *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrUeDel(cell, ueDelInfo, errInfo)
RgSchCellCb         *cell;
RgrDel              *ueDelInfo;
RgSchErrInfo        *errInfo;
#endif
{
   RgSchUeCb        *ue;
   RgSchRaCb        *raCb;
#ifdef DEBUGP
   Inst             inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgRgrUeDel);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR UE DELETE: cellId %d "
        "ueId %d\n", ueDelInfo->u.ueDel.cellId, ueDelInfo->u.ueDel.crnti));
   
   errInfo->errCause = RGSCHERR_CFG_RGR_UE_DEL;

   if (cell->cellId != ueDelInfo->u.ueDel.cellId)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist %d\n",
                ueDelInfo->u.ueDel.cellId));
      MSPD_ERR("Cell does not exist %d\n", ueDelInfo->u.ueDel.cellId);
      RETVALUE(RFAILED);
   }
#ifdef PWAV
   MSPD_DBG("Received RGR UE delete for UE:%d @ (%d, %d)",ueDelInfo->u.ueDel.crnti, cell->crntTime.sfn, cell->crntTime.subframe);
#endif
   if ((ue = rgSCHDbmGetUeCb(cell, ueDelInfo->u.ueDel.crnti)) == NULLP)
   {
      if((raCb = rgSCHDbmGetRaCb(cell, ueDelInfo->u.ueDel.crnti)) == NULLP)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Ue does not exist %d\n",
             ueDelInfo->u.ueDel.crnti));
         MSPD_ERR("Ue does not exist %d\n", ueDelInfo->u.ueDel.crnti);
         RETVALUE(RFAILED);
      }
      else
      {
         /* This happens in case of Msg4 rejection */
         raCb->toDel = TRUE;
         RETVALUE(ROK);
      }
   }
   else
   {
      /* Delete Ue from the UE list of CELL*/
#ifdef PWAV
	  MSPD_DBG("RACB and UE exists, DEleing UE:%d\n", ue->ueId);
#endif
      rgSCHDbmDelUeCb(cell, ue);

      /* Call MeasGap and AckNakRep processing module */
      rgSCHMeasGapANRepUeDel(cell, ue);

#ifdef LTEMAC_DRX
      /* Call DRX module to remove UEs from various
       * lists it maintain 
       */
      (Void)rgSCHDrxUeDel(cell,ue);
#endif /*LTEMAC_DRX*/
      /* Free Ue */
      
      /*MS_FIX: If RA CB exists, delete it*/
      if((raCb = rgSCHDbmGetRaCb(cell, ueDelInfo->u.ueDel.crnti)) != NULLP)
      {
         /* MS_FIX : syed RNTI was getting released twice, once by racb del 
	  * and subsequently by ueDel. Let it get released by ueDel alone */	      
         rgSCHRamDelRaCb(cell, raCb, FALSE);
      }
	  
      rgSCHCfgFreeUeCb(cell, ue);

      errInfo->errCause = RGSCHERR_NONE;

      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Ue %d deleted from cell %d \n",
               ueDelInfo->u.ueDel.crnti, ueDelInfo->u.ueDel.cellId));
      RETVALUE(ROK);
   }
}  /* rgSCHCfgRgrUeDel */


/**
 * @brief Handler for the logical channel delete request from
 * RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgrDel      *lcDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLcDel
(
RgSchCellCb    *cell,
RgrDel         *lcDelInfo,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLcDel(cell, lcDelInfo, errInfo)
RgSchCellCb    *cell;
RgrDel         *lcDelInfo;
RgSchErrInfo   *errInfo;
#endif
{
   RgSchUeCb    *ue;
   RgSchDlLcCb  *dlLc;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgRgrLcDel);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR LC DELETE: cellId %d"
            " ueId %d lcId %d \n", lcDelInfo->u.lchDel.cellId,
            lcDelInfo->u.lchDel.crnti, lcDelInfo->u.lchDel.lcId));

   errInfo->errCause = RGSCHERR_CFG_RGR_LC_DEL;

   /* Fetch the Active cell */
   if (cell->cellId != lcDelInfo->u.lchDel.cellId)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist %d\n",
                  lcDelInfo->u.lchDel.cellId));
     MSPD_ERR("Cell does not exist %d\n",
                  lcDelInfo->u.lchDel.cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((ue = rgSCHDbmGetUeCb(cell, lcDelInfo->u.lchDel.crnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Ue %d does not exist for dedicated "
           "logical channel\n", lcDelInfo->u.lchDel.crnti));
      MSPD_ERR("Ue %d does not exist for dedicated "
           "logical channel\n", lcDelInfo->u.lchDel.crnti);
      RETVALUE(RFAILED);
   }
   if ((dlLc = rgSCHDbmGetDlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
         == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "DL LC %d does not exist for"
               " UE %d\n", lcDelInfo->u.lchDel.lcId, lcDelInfo->u.lchDel.crnti));
      MSPD_ERR("DL LC %d does not exist for"
               " UE %d\n", lcDelInfo->u.lchDel.lcId, lcDelInfo->u.lchDel.crnti);
      RETVALUE(RFAILED);
   }
   rgSCHDbmDelDlDedLcCb(ue, dlLc);
   rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);

   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Logical channel %d deleted from Ue %d"
      " from cell %d\n", lcDelInfo->u.lchDel.lcId, lcDelInfo->u.lchDel.crnti,
         lcDelInfo->u.lchDel.cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLcDel */



/**
 * @brief Handler for the logical channel delete request from
 * RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcgDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgrDel      *lcDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLcgDel
(
RgSchCellCb    *cell,
RgrDel         *lcDelInfo,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLcgDel(cell, lcDelInfo, errInfo)
RgSchCellCb    *cell;
RgrDel         *lcDelInfo;
RgSchErrInfo   *errInfo;
#endif
{
   RgSchUeCb    *ue = NULLP;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgRgrLcgDel);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING RGR LCG DELETE: cellId %d"
            " ueId %d lcgId %d \n", lcDelInfo->u.lcgDel.cellId,
            lcDelInfo->u.lcgDel.crnti, lcDelInfo->u.lcgDel.lcgId));

   errInfo->errCause = RGSCHERR_CFG_RGR_LCG_DEL;

   /* Fetch the Active cell */
   if (cell->cellId != lcDelInfo->u.lcgDel.cellId)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist %d\n",
                  lcDelInfo->u.lcgDel.cellId));
      MSPD_ERR("Cell does not exist %d\n",
                  lcDelInfo->u.lcgDel.cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((ue = rgSCHDbmGetUeCb(cell, lcDelInfo->u.lcgDel.crnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Ue %d does not exist for dedicated "
           "logical channel\n", lcDelInfo->u.lcgDel.crnti));
      MSPD_ERR("Ue %d does not exist for dedicated "
           "logical channel\n", lcDelInfo->u.lcgDel.crnti);
      RETVALUE(RFAILED);
   }

   /* set lcgId in UEs lcg cntrl blk to invalid */
   rgSCHUtlRgrLcgDel(cell, ue, lcDelInfo->u.lcgDel.lcgId);
   ue->ul.lcgArr[lcDelInfo->u.lcgDel.lcgId].lcgId = RGSCH_INVALID_LCG_ID;
   
   errInfo->errCause = RGSCHERR_NONE;
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Logical channel Group %d deleted from Ue %d"
      " from cell %d\n", lcDelInfo->u.lcgDel.lcgId, lcDelInfo->u.lcgDel.crnti,
         lcDelInfo->u.lcgDel.cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLcgDel */



/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrLcCfg
 *
 *
 *     Desc : Validates dedicated logical channel configuration recieved from RRM.
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
PUBLIC S16 rgSCHCfgVldtRgrLcCfg
(
Inst             inst,
RgrLchCfg     *lcCfg,
RgSchCellCb      **cell,
RgSchUeCb        **ue,
RgSchErrInfo     *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrLcCfg(inst, lcCfg, cell, ue, errInfo)
Inst             inst;
RgrLchCfg     *lcCfg;
RgSchCellCb      **cell;
RgSchUeCb        **ue;
RgSchErrInfo     *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrLcCfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATING RGR DEDICATED LC CONFIG"));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_DED_LC_CFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcCfg->cellId))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist: Cell %d\n",
               lcCfg->cellId));
      MSPD_ERR("Cell does not exist: Cell %d\n",
               lcCfg->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcCfg->crnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Ue %d does not exist for dedicated"
         " logical channel %d\n", lcCfg->crnti, lcCfg->lcId));
      MSPD_ERR("Ue %d does not exist for dedicated"
         " logical channel %d\n", lcCfg->crnti, lcCfg->lcId);
      RETVALUE(RFAILED);
   }

   /* Validate logical channel Id */
   if ((lcCfg->lcId < RGSCH_DEDLC_MIN_LCID)
            ||(lcCfg->lcId > RGSCH_DEDLC_MAX_LCID))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid logical channel Id %d\n",
               lcCfg->lcId));
      MSPD_ERR("Invalid logical channel Id %d\n",
               lcCfg->lcId);
      RETVALUE(RFAILED);
   }

   if (lcCfg->lcType != CM_LTE_LCH_DTCH && lcCfg->lcType != CM_LTE_LCH_DCCH)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid logical channel Type %d\n",
               lcCfg->lcType));
      MSPD_ERR("Invalid logical channel Type %d\n",
               lcCfg->lcType);
      RETVALUE(RFAILED);
   }

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Dedicated logical channel %d validated"
      " for ue %d for cell %d\n", lcCfg->lcId, lcCfg->crnti, lcCfg->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLcCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrLcgCfg
 *
 *
 *     Desc : Validates dedicated logical channel group configuration recieved from RRM.
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
PUBLIC S16 rgSCHCfgVldtRgrLcgCfg
(
Inst             inst,
RgrLcgCfg        *lcgCfg,
RgSchCellCb      **cell,
RgSchUeCb        **ue,
RgSchErrInfo     *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrLcgCfg(inst, lcgCfg, cell, ue, errInfo)
Inst             inst;
RgrLcgCfg        *lcgCfg;
RgSchCellCb      **cell;
RgSchUeCb        **ue;
RgSchErrInfo     *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrLcgCfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATING RGR DEDICATED LCG CONFIG"));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_DED_LCG_CFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcgCfg->cellId))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell does not exist: Cell %d\n",
               lcgCfg->cellId));
      MSPD_ERR("Cell does not exist: Cell %d\n",
               lcgCfg->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcgCfg->crnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Ue %d does not exist for dedicated"
         " logical channel %d\n", lcgCfg->crnti, lcgCfg->ulInfo.lcgId));
      MSPD_ERR("Ue %d does not exist for dedicated"
         " logical channel %d\n", lcgCfg->crnti, lcgCfg->ulInfo.lcgId);
      RETVALUE(RFAILED);
   }

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Dedicated Logical Group %d validated"
      " for ue %d for cell %d\n", lcgCfg->ulInfo.lcgId, lcgCfg->crnti, lcgCfg->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLcgCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellPwrCfg
 *
 *     Desc : Validates cell power configuration.
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
PRIVATE S16 rgSCHCfgVldtRgrCellPwrCfg
(
Inst             inst,
RgrCellCfg       *cellCfg,
RgSchErrInfo     *errInfo
)
#else
PRIVATE S16 rgSCHCfgVldtRgrCellPwrCfg(inst, cellCfg, errInfo)
Inst             inst;
RgrCellCfg       *cellCfg;
RgSchErrInfo     *errInfo;
#endif
{
   UNUSED(inst);
   UNUSED(cellCfg);
   UNUSED(errInfo);

   TRC2(rgSCHCfgVldtRgrCellPwrCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATING COMMON LC CONFIG"));

   /* This function does nothing now, placeholder for
    * subsequent power config validations that may be needed */

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Power configuration"
             " validated for cell %d\n", cellCfg->cellId));

   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCellPwrCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCmnLcCfg
 *
 *
 *     Desc : Validates common logical channel configuration recieved from RRM.
 *
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *cellCfg
 *  @param[out] RgSchErrInfo *errInfo
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
PRIVATE S16 rgSCHCfgVldtRgrCmnLcCfg
(
Inst             inst,
RgrCellCfg       *cellCfg,
RgSchErrInfo     *errInfo
)
#else
PRIVATE S16 rgSCHCfgVldtRgrCmnLcCfg(inst, cellCfg, errInfo)
Inst             inst;
RgrCellCfg       *cellCfg;
RgSchErrInfo     *errInfo;
#endif
{
   U8            idx;
   RgrCmnLchCfg  *lcCfg; 
   U8            dirVld  = FALSE;
   U8            bitMask = 0x00;
   U8            cnt=0;

   TRC2(rgSCHCfgVldtRgrCmnLcCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "VALIDATING COMMON LC CONFIG"));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CMN_LC_CFG;
   
   for (idx = 0; idx < cellCfg->numCmnLcs; idx++)
   {
      lcCfg = &(cellCfg->cmnLcCfg[idx]);
      /* Validate downlink info */
      if (lcCfg->dir & RGR_DIR_TX)
      {
         if (lcCfg->lcType == CM_LTE_LCH_BCCH)
         {
            if (lcCfg->dlTrchType == CM_LTE_TRCH_DL_SCH)
            {
               if(cnt == 0)
               {
                  bitMask |= RGSCH_BCCH_DLSCH_CFG1;
                  cnt++;
               }
               else
               {

                  if((
                     (cellCfg->siCfg.siWinSize == 1)  ||
                     (cellCfg->siCfg.siWinSize == 2)  ||
                     (cellCfg->siCfg.siWinSize == 5)  ||
                     (cellCfg->siCfg.siWinSize == 10) ||
                     (cellCfg->siCfg.siWinSize == 15) ||
                     (cellCfg->siCfg.siWinSize == 20) ||
                     (cellCfg->siCfg.siWinSize == 40)) &&
                     (cellCfg->siCfg.retxCnt>0)
                    )
                  {
                     bitMask |= RGSCH_BCCH_DLSCH_CFG2;
                  }
                  else
                  {
                     RGSCHDBGERR(inst, (rgSchPBuf(inst),"Invalid si config "
                       " for cell %d\n", cellCfg->cellId));
                     RETVALUE(RFAILED);
                  }
               }
            }
            else if (lcCfg->dlTrchType == CM_LTE_TRCH_BCH)
            {
               bitMask |= RGSCH_BCCH_BCH_CFG;
            }
            else
            {
               RGSCHDBGERR(inst, (rgSchPBuf(inst),"Invalid transport channel %d"
                " for cell %d\n", lcCfg->dlTrchType, cellCfg->cellId));
               RETVALUE(RFAILED);
            }
         }
         /* rg001.101: Reordered 'if' check to remove dependency on order of 
          * configuration */
         else if (lcCfg->lcType == CM_LTE_LCH_PCCH)
         {
            bitMask |= RGSCH_PCCH_CFG;
         }
         else if (lcCfg->lcType == CM_LTE_LCH_CCCH)
         {
            bitMask |= RGSCH_DL_CCCH_CFG;
         }
         dirVld = TRUE;
      }

      /* Validate uplink info */
      if (lcCfg->dir & RGR_DIR_RX)
      {
         /* Uplink CCCH */
         if (lcCfg->lcType != CM_LTE_LCH_CCCH)
         {
            RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid UL common lcType %d "
             "for cell %d\n", lcCfg->lcType, cellCfg->cellId));
            RETVALUE(RFAILED);
         }
         else
         {
            bitMask |= RGSCH_UL_CCCH_CFG;
         }
         dirVld = TRUE;
      }

      /* Invalid direction */
      if (!dirVld)
      {
         RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid Direction %d\n", 
            lcCfg->dir));
         RETVALUE(RFAILED);
      }
   }
   if (bitMask != RGSCH_CELL_ACTIVE_CFG)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid Common channel config for"
              " cell %d\n", cellCfg->cellId));
      RETVALUE(RFAILED);
   }

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Common logical channel configuration"
             " validated for cell %d\n", cellCfg->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCmnLcCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeCqiModeCfg
 *
 *
 *     Desc : Validates UE CQI modes Configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtUeCqiModeCfg
(
RgSchCellCb       *cell,
RgrUeCfg          *ueCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeCqiModeCfg(cell, ueCfg)
RgSchCellCb       *cell;
RgrUeCfg          *ueCfg;
#endif
{
   
    TRC2(rgSCHCfgVldtUeCqiModeCfg)

#ifndef TFU_UPGRADE
   /* Validate K value in periodic CQI Config */
   if(((ueCfg->ueDlCqiCfg.prdCqiCfg.k < 1)||
            (ueCfg->ueDlCqiCfg.prdCqiCfg.k > 4)) ||
         ((ueCfg->ueDlCqiCfg.prdCqiCfg.cqiPmiCfgIdx < 1) || 
         (ueCfg->ueDlCqiCfg.prdCqiCfg.cqiPmiCfgIdx > 1024)))
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),\
                  "rgSCHCfgVldtUeCqiModeCfg: \
                  Invalid Periodic CQI Info for UE %d\n",
               (U8)ueCfg->crnti));
      RETVALUE(RFAILED);
   }
#endif
   /* Validate UE Aperiodic CQI mode */
   if (ueCfg->ueDlCqiCfg.aprdCqiCfg.aprdModeEnum > RGR_APRD_CQI_MOD31)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),\
                  "rgSCHCfgVldtUeCqiModeCfg:\
                  Invalid Aperiodic mode config for DL CQI for UE %d\n",
            ueCfg->ueDlCqiCfg.aprdCqiCfg.aprdModeEnum));
      RETVALUE(RFAILED);
   }
#ifndef TFU_UPGRADE
   /* Validate UE Periodic CQI mode */ 
   if (ueCfg->ueDlCqiCfg.prdCqiCfg.prdModeEnum > RGR_PRD_CQI_MOD21)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),\
                  "rgSCHCfgVldtUeCqiModeCfg:\
                  Invalid periodic mode config for DL CQI for UE %d\n",
            ueCfg->ueDlCqiCfg.prdCqiCfg.prdModeEnum));
      RETVALUE(RFAILED);
   }
#else
   if (ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.prdModeEnum > RGR_PRD_CQI_MOD21)
   {
     RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),\
                 "rgSCHCfgVldtUeCqiModeCfg: Invalid \
                 periodic mode config for DL CQI for UE %d\n",
                 ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.prdModeEnum));
     RETVALUE(RFAILED);
   }
   if (ueCfg->ueDlCqiCfg.prdCqiCfg.type == 1)
   {
#ifdef MSPD
      MSPD_DBG("Periodic CQI config: n2Pucch(%u) cqiPmiCfgIdx (%u) cqiRepType(%u)"
	  	"k (%u) riEna(%u) riCfgIdx(%u) sANCQI (%u) prdModeEnum (%u)\n", 
	  	ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiPResIdx,
	  	ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiPCfgIdx,
	  	ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiRepType,
	  	ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.k,
	  	ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.riEna,
	  	ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.riCfgIdx,
	  	ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.sANCQI,
	  	ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.prdModeEnum
	  	);
#endif
	  	
   }

#endif
   
   RETVALUE(ROK);
      
}
/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeMeasGapAckNakRepCfg
 *
 *
 *     Desc : Validates UE Measurement Gap and Ack Nack Repetition Configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepCfg
(
RgSchCellCb       *cell,
RgrUeCfg          *ueCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepCfg(cell, ueCfg)
RgSchCellCb       *cell;
RgrUeCfg          *ueCfg;
#endif
{
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   TRC2(rgSCHCfgVldtUeMeasGapAckNakRepCfg)
      
#ifdef LTE_TDD
   if ((ueCfg->ackNackModeEnum == RGR_TDD_ACKNACK_MODE_MULT) &&
         (ueCfg->ueAckNackCfg.isAckNackEnabled == TRUE))
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "TDD ACK NACK Multiplexing Mode\n"
      "is not allowed when Ack/Nack is Enabled: %d ",ueCfg->ueAckNackCfg.ackNackRepFactor));
      RETVALUE(RFAILED);
   }
#endif /* LTE_TDD */
   /* Validate AckNackRep Factor */
   /*rg003.301[ccpu00121749]-ADD-Validating ACK/NACK is Enabled */ 
   if((ueCfg->ueAckNackCfg.isAckNackEnabled == FALSE) &&
         (!ueCfg->ueMesGapCfg.isMesGapEnabled))
   {
      RETVALUE(ROK);
   }

   if(ueCfg->ueAckNackCfg.isAckNackEnabled)
   {
   if ( (ueCfg->ueAckNackCfg.ackNackRepFactor < RGR_ACKNACK_REPFACT_N2)
         || (ueCfg->ueAckNackCfg.ackNackRepFactor > RGR_ACKNACK_REPFACT_N6))
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid ACK NACK REP Factor (%d)\n"
               ,ueCfg->ueAckNackCfg.ackNackRepFactor));
      RETVALUE(RFAILED);
   }
   }
   if(ueCfg->ueMesGapCfg.isMesGapEnabled)
   {
   switch(ueCfg->ueMesGapCfg.gapPrd)
   {
      case RG_MEAS_GAPPRD_40:
         if(ueCfg->ueMesGapCfg.gapOffst >= RG_MEAS_GAPPRD_40)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid GAP Offset (%d)\n"
                     ,ueCfg->ueMesGapCfg.gapOffst));
            RETVALUE(RFAILED);
         }
         break;
      case RG_MEAS_GAPPRD_80:
         if(ueCfg->ueMesGapCfg.gapOffst >= RG_MEAS_GAPPRD_80)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid GAP Offset (%d)\n"
                     ,ueCfg->ueMesGapCfg.gapOffst));
            RETVALUE(RFAILED);
         }
         break;
            /*rg003.301[ccpu00121749] -ADD-Validating ACK/NACK is Enabled */ 
         default:
         { 
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid GAP Periodicity Settings (%d)\n"
                        ,ueCfg->ueMesGapCfg.gapPrd));
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtUeMeasGapAckNakRepCfg*/


/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeMeasGapAckNakRepRecfg
 *
 *
 *     Desc : Validates UE Measurement Gap and Ack Nack Repetition Configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepRecfg
(
RgSchCellCb       *cell,
RgrUeRecfg        *ueRecfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepRecfg(cell, ueRecfg)
RgSchCellCb       *cell;
RgrUeRecfg        *ueRecfg;
#endif
{
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   TRC2(rgSCHCfgVldtUeMeasGapAckNakRepRecfg)
   /*rg003.301[ccpu00121749] -ADD-Validating ACK/NACK is Enabled */
   if((ueRecfg->ueAckNackRecfg.isAckNackEnabled == FALSE) &&
          (!ueRecfg->ueMeasGapRecfg.isMesGapEnabled))
   {
      RETVALUE(ROK);
   }

   if(ueRecfg->ueAckNackRecfg.isAckNackEnabled )
   {
   /* Validate AckNackRep Factor */
   if ( (ueRecfg->ueAckNackRecfg.ackNackRepFactor < RGR_ACKNACK_REPFACT_N2)
         || (ueRecfg->ueAckNackRecfg.ackNackRepFactor > RGR_ACKNACK_REPFACT_N6))
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid ACK NACK REP Factor (%d)\n"
               ,ueRecfg->ueAckNackRecfg.ackNackRepFactor));
      RETVALUE(RFAILED);
   }
   }
   if(ueRecfg->ueMeasGapRecfg.isMesGapEnabled)
   {
   switch(ueRecfg->ueMeasGapRecfg.gapPrd)
   {
      case RG_MEAS_GAPPRD_40:
         if(ueRecfg->ueMeasGapRecfg.gapOffst >= RG_MEAS_GAPPRD_40)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid GAP Offset (%d)\n"
                     ,ueRecfg->ueMeasGapRecfg.gapOffst));
            RETVALUE(RFAILED);
         }
         break;
      case RG_MEAS_GAPPRD_80:
         if(ueRecfg->ueMeasGapRecfg.gapOffst >= RG_MEAS_GAPPRD_80)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid GAP Offset (%d)\n"
                     ,ueRecfg->ueMeasGapRecfg.gapOffst));
            RETVALUE(RFAILED);
         }
         break;
            /*rg003.301[ccpu00121749] -ADD-Validating ACK/NACK is Enabled */ 
         default:
         { 
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid GAP Periodicity Settings (%d)\n"
                        ,ueRecfg->ueMeasGapRecfg.gapPrd));
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtUeMeasGapAckNakRepRecfg*/

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeDlSpsCfg
 *
 *
 *     Desc : Validates UE's DL SPS configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtUeDlSpsCfg
(
RgSchCellCb       *cell,
RgrUeSpsDlCfg     *dlSpsCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeDlSpsCfg(cell, dlSpsCfg)
RgSchCellCb       *cell;
RgrUeSpsDlCfg     *dlSpsCfg;
#endif
{
#ifdef DEBUGP
   Inst               inst = cell->instIdx;
#endif
   U8                 idx = 0;

   TRC2(rgSCHCfgVldtUeDlSpsCfg);

   if (dlSpsCfg->dlSpsPrdctyEnum == RGR_SPS_PRD_INVALID)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid periodicity value for DL"
               " SPS\n"));
      RETVALUE(RFAILED);
   }
   if (dlSpsCfg->numPucchVal > RG_SCH_MAX_NUM_N1PUCCH_PER_UE)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid number of n1Pucch values"
               " in DL SPS Config\n"));
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < dlSpsCfg->numPucchVal; ++idx)
   {
      if (dlSpsCfg->n1PucchVal[idx] > RG_SCH_MAX_N1PUCCH_VAL)
      {
#ifdef ALIGN_64BIT
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid N1Pucch value"
                  " in DL SPS Config %u\n", dlSpsCfg->n1PucchVal[idx]));
#else
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid N1Pucch value"
                  " in DL SPS Config %lu\n", dlSpsCfg->n1PucchVal[idx]));
#endif
         RETVALUE(RFAILED);
      }
   }

   if (dlSpsCfg->numSpsHqProc > RGSCH_MAX_DL_HQ_PROC)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid number of SPS HARQ procs"
               " in DL SPS Config\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtDlSpsCfg */
#endif /* LTEMAC_SPS */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtUePwrCfg
 *
 *
 *     Desc : Validates UE Group power configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtUePwrCfg
(
RgSchCellCb       *cell,
RgrUeUlPwrCfg     *pwrCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUePwrCfg(cell, pwrCfg)
RgSchCellCb       *cell;
RgrUeUlPwrCfg     *pwrCfg;
#endif
{
#ifdef DEBUGP
   Inst               inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgVldtUePwrCfg);

#ifdef MSPD
   MSPD_DBG("Power Configuration isAccumulated(%u) deltaMcsEnbld(%u)"
   	"trgCqi(%u) p0UePusch(%u) p0UePucch(%u) grpPwrPucch(%u %u %u) grpPwrPusch(%u %u %u)\n", 
   	pwrCfg->isAccumulated, pwrCfg->isDeltaMCSEnabled, pwrCfg->trgCqi,
   	pwrCfg->p0UePusch, pwrCfg->p0UePucch,
    pwrCfg->uePucchPwr.pres, pwrCfg->uePucchPwr.idx, pwrCfg->uePucchPwr.tpcRnti,
    pwrCfg->uePuschPwr.pres, pwrCfg->uePuschPwr.idx, pwrCfg->uePuschPwr.tpcRnti);
#endif
   /* Group power control works only in accumulated mode */
   if (!pwrCfg->isAccumulated)
   {
      /* MS_FIX */
      if (pwrCfg->uePuschPwr.pres)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Accumulation configutation"
              " not in sync with group power configuration\n"));
         RETVALUE(RFAILED);
      }
   }

   if (rgSCHCfgVldtUeGrpPwrCfg(cell, &pwrCfg->uePuschPwr) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid PUSCH Group power"
           " configuration\n"));
      RETVALUE(RFAILED);
   }
   if (rgSCHCfgVldtUeGrpPwrCfg(cell, &pwrCfg->uePucchPwr) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid PUSCH Group power"
           " configuration\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtUePwrCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeGrpPwrCfg
 *
 *
 *     Desc : Validates UE Group power configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtUeGrpPwrCfg
(
RgSchCellCb       *cell,
RgrUeGrpPwrCfg *grpPwrCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeGrpPwrCfg(cell, grpPwrCfg)
RgSchCellCb       *cell;
RgrUeGrpPwrCfg *grpPwrCfg;
#endif
{
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   TRC2(rgSCHCfgVldtUeGrpPwrCfg);

   if ((grpPwrCfg->pres) &&
       (((grpPwrCfg->tpcRnti > cell->rntiDb.rntiStart) &&
        ((grpPwrCfg->tpcRnti <
                     (cell->rntiDb.rntiStart + cell->rntiDb.maxRntis))))))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid Uplink Group power " 
             "configuration\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtUeGrpPwrCfg */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgVldtSpsReCfg
 *
 *
 *     Desc : Validates UE SPS and other SPS dependent 
 *     configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtSpsReCfg
(
RgSchCellCb       *cell,
RgSchUeCb         *ue,
RgrUeRecfg        *ueRecfg
)
#else
PRIVATE S16 rgSCHCfgVldtSpsReCfg(cell, ue, ueRecfg)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
RgrUeRecfg     *ueRecfg;
#endif
{
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   TRC2(rgSCHCfgVldtSpsReCfg);
   if (ueRecfg->ueRecfgTypes & RGR_UE_DLSPS_RECFG)
   {
      if (rgSCHCfgVldtUeDlSpsCfg(cell, &ueRecfg->ueSpsRecfg.dlSpsCfg) != ROK)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid DL SPS configuration"
                  " for the UE\n"));
         RETVALUE(RFAILED);
      }
   }
#ifdef RGR_V2
#ifdef LTEMAC_DRX 
   if(ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)
   {
     if (ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG)
     {
     /* ccpu00117035 - MOD - changed instIdx to inst */
     /* ccpu00117035 - MOD - changed ueID to oldCrnti*/
      RGSCHDBGERR(inst, (rgSchPBuf(inst),
               " DRX reconfig not supported DL SPS enabled UE %d\n", 
               (U16)ueRecfg->oldCrnti));
      RETVALUE(RFAILED);
     }
   }
#endif
#endif
/* ccpu00117627 - ADD - SPS recfg validation against HDFDD */
#ifdef LTEMAC_HDFDD
   if(ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)
   {
      if(ue->hdFddEnbld == TRUE)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst),
                  "SPS is not supported for HDFDD enabled UE %d\n",
                  (U16)ueRecfg->oldCrnti));
         RETVALUE(RFAILED);
      }
   }
   if(ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled)
   {
      if(ue->hdFddEnbld == TRUE)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst),
                  "SPS is not supported for HDFDD enabled UE %d\n",
                  (U16)ueRecfg->oldCrnti));
         RETVALUE(RFAILED);
      }
   }
#endif

   RETVALUE(ROK);
}  /*rgSCHCfgVldtSpsReCfg*/
#endif

#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
/***********************************************************
 *
 *     Func : rgSCHCfgVldtCqiReptReCfg
 *
 *
 *     Desc : Validates UE CQI report for DL Power control
 *     configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtCqiReptReCfg
(
RgSchCellCb       *cell,
RgrUeRecfg     *ueRecfg
)
#else
PRIVATE S16 rgSCHCfgVldtCqiReptReCfg(cell, ueRecfg)
RgSchCellCb       *cell;
RgrUeRecfg     *ueRecfg;
#endif
{
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   TRC2(rgSCHCfgVldtCqiReptReCfg);
   /* Validate DL Power Control Config parameters */
   if (ueRecfg->ueCqiReptCfg.numColltdCqiRept > RGR_CQIRPTS_MAXN)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalide numColltdCqiRept,"
               "MAX supported %d\n",RGR_CQIRPTS_MAXN));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /*rgSCHCfgVldtCqiReptReCfg*/
#endif

/***********************************************************
 *
 *     Func : rgSCHCfgRgrLcChfg
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
PUBLIC S16 rgSCHCfgRgrLchCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchCfg     *lcCfg,
RgSchErrInfo     *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLchCfg(cell, ue, lcCfg, errInfo)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrLchCfg     *lcCfg;
RgSchErrInfo     *errInfo;
#endif
{
   S16       ret;
   RgSchDlLcCb  *dlLc = NULLP;
   Inst    inst = cell->instIdx;

   TRC2(rgSCHCfgRgrLchCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING DEDICATED LC CONFIG"));

   errInfo->errCause = RGSCHERR_CFG_RGR_DED_LC_CFG;

   /* Allocate the downlink logical channel control block */
   if((ret = rgSCHUtlAllocSBuf(inst, (Data**)&dlLc, 
               sizeof(RgSchDlLcCb))) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for "
               "Downlink LC %d\n", lcCfg->lcId));
      RETVALUE(RFAILED);
   }
   if ((U8 *)dlLc == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for "
               "Downlink LC %d\n", lcCfg->lcId));
      RETVALUE(RFAILED);
   }
   dlLc->lcId   = lcCfg->lcId;

   rgSCHDbmInsDlDedLcCb(ue, dlLc);
   
   ret = rgSCHUtlRgrLcCfg(cell, ue, dlLc, lcCfg, errInfo);

   if (ret != ROK)
   {
      /* ROLLBACK */
      if (dlLc)
      {
         rgSCHDbmDelDlDedLcCb(ue, dlLc);
         rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);
      }

      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Dedicated logical channel "
        "configuration failed at SCH: cellId %d ueId %d lcId %d\n",
        cell->cellId, ue->ueId, lcCfg->lcId));
      RETVALUE(RFAILED);
   }
#ifdef LTE_L2_MEAS
   dlLc->qciCb = &(cell->qciArray[lcCfg->dlInfo.dlQos.qci]);
   dlLc->qciCb->qci = lcCfg->dlInfo.dlQos.qci;
#endif /* LTE_L2_MEAS */

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Dedicated LC config done\n"));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLchCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgRgrLcgCfg
 *
 *
 *     Desc : Handles dedicated logical channel group configuration
 *     recieved from RRM.
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
PUBLIC S16 rgSCHCfgRgrLcgCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLcgCfg        *lcgCfg,
RgSchErrInfo     *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLcgCfg(cell, ue, lcgCfg, errInfo)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrLcgCfg        *lcgCfg;
RgSchErrInfo     *errInfo;
#endif
{
   S16          ret = ROK;
#ifdef DEBUGP
   Inst inst = cell->instIdx;
#endif

#ifdef LTE_L2_MEAS
   U32          idx;
   RgSchUlLcCb  *ulLc; 
#endif
   TRC2(rgSCHCfgRgrLcgCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING DEDICATED LCG CONFIG"));

   errInfo->errCause = RGSCHERR_CFG_RGR_DED_LCG_CFG;

   ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcgId = lcgCfg->ulInfo.lcgId;

   ret = rgSCHUtlRgrLcgCfg(cell, ue, lcgCfg, errInfo);
   if (ret != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Dedicated logical channel "
        "configuration failed at SCH: cellId %d ueId %d lcgId %d\n",
        cell->cellId, ue->ueId, lcgCfg->ulInfo.lcgId));
      /* Roll back lcgCfg */
      ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcgId = RGSCH_INVALID_LCG_ID;
      rgSCHUtlRgrLcgDel(cell, ue, lcgCfg->ulInfo.lcgId);
      RETVALUE(RFAILED);
   }
#ifdef LTE_L2_MEAS
   /* Copy all info of UL LCH in cfg to ulLcgCb */
   for (idx = 0; idx < lcgCfg->ulInfo.numLch; idx++)
   {
      /* Allocate the uplink logical channel control block */
      if((ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data**)&ulLc, 
                  sizeof(RgSchUlLcCb))) != ROK)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for "));
         RETVALUE(RFAILED);
      }
      if ((U8 *)ulLc == NULLP)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for "));
         RETVALUE(RFAILED);
      }
      ulLc->lcId = lcgCfg->ulInfo.lchUlCfg[idx].lcId;
      ulLc->qciCb = &(cell->qciArray[lcgCfg->ulInfo.lchUlCfg[idx].qci]);
      ulLc->qciCb->qci = lcgCfg->ulInfo.lchUlCfg[idx].qci;
      ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcArray[idx] = ulLc;
   }
   ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].numLch = lcgCfg->ulInfo.numLch;
#endif /* LTE_L2_MEAS */

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Dedicated LC config done\n"));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLcgCfg */



/***********************************************************
 *
 *     Func : rgSCHCfgRgrCmnLcCfg
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
PRIVATE S16 rgSCHCfgRgrCmnLcCfg
(
RgSchCellCb           *cell,
RgrCmnLchCfg          *lcCfg,
RgSchErrInfo          *errInfo
)
#else
PRIVATE S16 rgSCHCfgRgrCmnLcCfg(cell, lcCfg, errInfo)
RgSchCellCb         *cell;
RgrCmnLchCfg        *lcCfg;
RgSchErrInfo        *errInfo;
#endif
{
   RgSchClcDlLcCb cmnLcCb;
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   TRC2(rgSCHCfgRgrCmnLcCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "APPLYING COMMON LC CONFIG"));

   errInfo->errCause = RGSCHERR_CFG_RGR_CMN_LC_CFG;

   cmMemset((U8 *)&cmnLcCb, 0, sizeof(cmnLcCb));

   /* Handle configuration for CCCH/BCCH/PCCH */
   if (lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      /* UL and DL CCCH configuration */
      if (lcCfg->dir & RGR_DIR_TX)
      {
         cell->dlCcchId = lcCfg->lcId;
      }

      if (lcCfg->dir & RGR_DIR_RX)
      {
         cell->ulCcchId = lcCfg->lcId;
      }
   }
   else
   {
      cmnLcCb.lcId = lcCfg->lcId;
      rgSCHDbmInitCmnLcBoLst(&cmnLcCb);
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
         /* BCCH on BCH and DLSCH configuration */
         if (lcCfg->dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            rgSCHDbmInsBcchOnDlsch(cell, &cmnLcCb);
         }
         else
         {
            rgSCHDbmInsBcchOnBch(cell, &cmnLcCb);
         }
      }
      else  /* PCCH configuration */
      {
         rgSCHDbmInsPcch(cell, &cmnLcCb);
      }
   }

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Common LC config done: lcId %d"
        " cell %d\n", lcCfg->lcId, cell->cellId));
   RETVALUE(ROK);
}  /* rgSCHCfgRgrCmnLcCfg */



/***********************************************************
 *
 *     Func : rgSCHCfgFreeDlDedLcCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees downlink dedicated logical channel control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeDlDedLcCb
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc
)
#else
PRIVATE Void rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *dlLc;
#endif
{
   Inst    inst = cell->instIdx;
   TRC2(rgSCHCfgFreeDlDedLcCb);

   rgSCHUtlFreeDlLc(cell, ue, dlLc);

   /* De-allocate the Cb */
   /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, (Data **)&dlLc, sizeof(*dlLc));

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "DL dedicated LC freed\n"));

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeDlDedLcCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeDlCmnLcCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees downlink common logical channel control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeDlCmnLcCb
(
RgSchClcDlLcCb   *cmnDlLc
)
#else
PRIVATE Void rgSCHCfgFreeDlCmnLcCb(cmnDlLc)
RgSchClcDlLcCb      *cmnDlLc;
#endif
{
   TRC2(rgSCHCfgFreeDlCmnLcCb);
 
   cmMemset((U8*)cmnDlLc, 0, sizeof(*cmnDlLc));
   cmnDlLc->lcId = RGSCH_INVALID_LC_ID;
   RETVOID;
}  /* rgSCHCfgFreeDlCmnLcCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees scheduler cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCfgFreeCellCb
(
RgSchCellCb      *cell
)
#else
PUBLIC Void rgSCHCfgFreeCellCb(cell)
RgSchCellCb      *cell;
#endif
{
   Inst    inst = cell->instIdx;
   TRC2(rgSCHCfgFreeCellCb);

   /* Free lists of the cell */
   rgSCHCfgFreeUeLst(cell);
#ifdef LTEMAC_SPS
   rgSCHCfgFreeSpsUeLst(cell);
#endif /* LTEMAC_SPS */
   rgSCHRamFreeCell(cell);

   rgSCHDbmRntiDbDeInit(cell);
   /* Deallocate the subframe allocation information */
   rgSCHUtlPutSfAlloc(cell);
   rgSCHUtlFreeCell(cell);

   rgSCHCfgFreeRgrCfgLst(cell);
   rgSCHCfgFreeCmnLcLst(cell);

   rgSCHUtlPutRlsHqAlloc(cell);

#ifdef LTE_TDD
   rgSCHDbmDeInitUeTfuPendLst(cell);
#endif /* LTE_TDD */

   /* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
   rgSCHUtlPutSiInfo(cell);
#endif/*RGR_SI_SCH*/

#ifdef LTEMAC_DRX
   (Void)rgSCHDrxCellDel(cell);
#endif /*LTEMAC_DRX*/
   /* De-allocate the Cell */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, (Data **)&cell, sizeof(*cell));
 
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Cell freed\n"));

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeCellCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeUeCb
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
PRIVATE Void rgSCHCfgFreeUeCb
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PRIVATE Void rgSCHCfgFreeUeCb(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   RgSchDlLcCb       *dlLc;
   Inst    inst = cell->instIdx;

   TRC2(rgSCHCfgFreeUeCb);

   /* Free all logical channel info per UE */
   while((dlLc = rgSCHDbmGetNextDlDedLcCb(ue, NULLP)) != NULLP)
   {
      rgSCHDbmDelDlDedLcCb(ue, dlLc);
      rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);
   }
   /* Free Scheduler specific information per UE */
   rgSCHUtlFreeUe(cell, ue);
   /* Free Uplink HARQ specific information per UE */
   rgSCHUhmFreeUe(ue);
#ifdef LTEMAC_DRX
   if ( ue->drxCb != NULLP)
   {
      /* free drxCb */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
            (Data**)(&(ue->drxCb)),
            sizeof(RgSchDrxUeCb));
   }

   ue->drxCb = (RgSchDrxUeCb *)NULLP;
#endif
   /* Free Downlink HARQ specific information per UE */
   rgSCHDhmFreeUe(ue);
   /* Release the RNTI */
   if (ue->rntiLnk)
   {
#ifdef PWAV
      MSPD_DBG("Releasing the RNTI back to the pool:%d\n", ue->ueId);
#endif
      /*MS_FIX: Release RNTI only if already deleted at MAC*/
      if(ue->dl.dlInactvMask & RG_MACUEDEL_INACTIVE)
      {
         rgSCHDbmRlsRnti(cell, ue->rntiLnk);
      }
	  else
	  {
	     /*Add to Guard Pool*/
		 cmLListAdd2Tail(&cell->rntiDb.rntiGuardPool,
                  &ue->rntiLnk->rntiGrdPoolLnk);
         ue->rntiLnk->rntiGrdPoolLnk.node = (PTR)ue->rntiLnk;
	  }
   }
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
    rgSCHCfgPCqiSrsSrUeDel(cell,ue); 
#endif 
#ifdef LTEMAC_HDFDD
    rgSCHHdFddUeDel(cell, ue);
#endif
   /* De-allocate the Ue */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, (Data **)&ue, sizeof(*ue));


   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Ue freed\n"));

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeUeCb */

/***********************************************************
 *
 *     Func : rgSCHCfgFreeRgrCfgLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees configuration lists in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeRgrCfgLst
(
RgSchCellCb      *cell
)
#else
PRIVATE Void rgSCHCfgFreeRgrCfgLst(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchCfgElem  *rgCfgElem;
   Inst    inst = cell->instIdx;

   TRC2(rgSCHCfgFreeRgrCfgLst);

   /* Free CURRENT RGR cfg list */
   while ((rgCfgElem = rgSCHDbmGetNextCrntRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelCrntRgrCfgElem(cell, rgCfgElem);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&rgCfgElem, sizeof(*rgCfgElem));
   }

   /* Free PENDING RGR cfg list */
   while ((rgCfgElem = rgSCHDbmGetNextPndngRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelPndngRgrCfgElem(cell, rgCfgElem);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&rgCfgElem, sizeof(*rgCfgElem));
   }

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Config lists of the cell %d freed\n", 
           cell->cellId));

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeRgrCfgLst */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeCmnLcLst
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
PRIVATE Void rgSCHCfgFreeCmnLcLst
(
RgSchCellCb      *cell
)
#else
PRIVATE Void rgSCHCfgFreeCmnLcLst(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchClcDlLcCb *dlCmnLc;
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgFreeCmnLcLst);

   if ((dlCmnLc = rgSCHDbmGetBcchOnBch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetFirstBcchOnDlsch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetSecondBcchOnDlsch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetPcch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Common LC list of the cell %d freed\n",
              cell->cellId));

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeCmnLcLst */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeUeLst
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
PRIVATE Void rgSCHCfgFreeUeLst
(
RgSchCellCb      *cell
)
#else
PRIVATE Void rgSCHCfgFreeUeLst(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchUeCb     *ue;
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
   TRC2(rgSCHCfgFreeUeLst);

   /* Free Ues in the list */
   while ((ue = rgSCHDbmGetNextUeCb(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelUeCb(cell, ue);
      
      /* Call MeasGap and AckNakRep processing module */
      rgSCHMeasGapANRepUeDel(cell, ue);

      rgSCHCfgFreeUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgSCHDbmDeInitUeCbLst(cell);

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Ue list of the cell %d freed\n",
          cell->cellId));

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeUeLst */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgFreeSpsUeLst
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
PRIVATE Void rgSCHCfgFreeSpsUeLst
(
RgSchCellCb      *cell
)
#else
PRIVATE Void rgSCHCfgFreeSpsUeLst(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchUeCb     *ue;
#ifdef DEBUGP
   Inst          inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgFreeSpsUeLst);

   /* Free Ues in the list */
   while ((ue = rgSCHDbmGetNextSpsUeCb(cell, NULLP)))
   {
      rgSCHDbmDelSpsUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgSCHDbmDeInitSpsUeCbLst(cell);

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Sps Ue list of the cell %d freed\n",
                                               cell->cellId));
}  /* rgSCHCfgFreeSpsUeLst */

#endif /* LTEMAC_SPS */

/* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellSiCfg
 *
 *     Desc : Validates SI Configuration for SI
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
PRIVATE S16 rgSCHCfgVldtRgrCellSiCfg
(
Inst             inst,
RgrSiCfg         *siCfg
)
#else
PRIVATE S16 rgSCHCfgVldtRgrCellSiCfg(inst, siCfg)
Inst             inst;
RgrSiCfg       *siCfg;
#endif
{
   U8   idx; /* idx for iteration */

   UNUSED(inst);

   TRC2(rgSCHCfgVldtRgrCellSiCfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "Validating SI Configuration"));

#ifndef LTE_TDD
   /* Check that retxCnt value should be <= value of siWinSize.
   This validation is only applicable for FDD mode. */
   if(siCfg->retxCnt > siCfg->siWinSize) 
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "retxCnt is greater than "
                         "siWinSize, validation failed\n"));
      RETVALUE(RFAILED);
   }
#endif

   /* Validate that a valid value for numSi has been specified */
   if(siCfg->numSi > RGR_MAX_NUM_SI) 
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Validation for numSi in "
                          "SI CFG failed\n"));
      RETVALUE(RFAILED);
   }

   /*Validate the value of periodicity specified for SIs */
   for(idx = 0;idx < siCfg->numSi;idx++)
   {
      switch(siCfg->siPeriodicity[idx])
      {
         case RGR_SI_PERD_8:
         case RGR_SI_PERD_16:
         case RGR_SI_PERD_32:
         case RGR_SI_PERD_64:
         case RGR_SI_PERD_128:
         case RGR_SI_PERD_256:
         case RGR_SI_PERD_512:
            continue;

         default:
            RGSCHDBGERR(inst, (rgSchPBuf(inst),"Validation for SI "
                        "Periodicity in SI-CFG failed\n"));
            RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCellSiCfg */


/**
 * @brief Validates the SI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrSiCfg
 *
 *     Processing Steps:
 *        - Validate the range of configured values recieved in
 *          configuration request.
 *        - If validated successfully,
 *          - Return ROK
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *siCfg
 *  @param[out] RgSchCellCb  *cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrSiCfg
(
Inst         inst,
RgrSiCfgReqInfo *siCfg,
RgSchCellCb      *cell,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrSiCfg(inst, siCfg, cell, errInfo)
Inst         inst;
RgrSiCfgReqInfo *siCfg;
RgSchCellCb      *cell;
RgSchErrInfo *errInfo;
#endif
{
   MsgLen  msgLen = 0;
   U8      numSi;

   TRC2(rgSCHCfgVldtRgrCellCfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "Validate RGR SI Config\n"));

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_SI_CFG;

   /*Validate the cfgType parameter */
   switch(siCfg->cfgType)
   {
      case RGR_SI_CFG_TYPE_MIB:   /* SI CFG Type MIB */
      case RGR_SI_CFG_TYPE_SIB1: /* SI CFG TYPE SIB1 */
         break;

      case RGR_SI_CFG_TYPE_SI:    /* SI CFG TYPE SI */
         /*Check that value of SI should be less than equal 
           to configured numSi parameter value */
         /*rg009.lpr-ccpu00116647 -  Added siId validation for lower limit */
         /* MS_FIX ccpu00123909 */
         numSi = (cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD) ? 
            cell->siCb.newSiCfg.numSi : cell->siCfg.numSi;
         if((siCfg->siId < RGSCH_SI_SIID_LOWER_LMT) || 
               (siCfg->siId > numSi))
         {
            RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid SI Id value"
                     " specified\n"));
            RETVALUE(RFAILED);
         }
         break;

      default:
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid cfgType "
                      "parameter value\n"));
         RETVALUE(RFAILED);
   }

   /*Validate the specified pdu */
   if(NULLP == siCfg->pdu)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid NULLP pdu "
                   "specified\n"));
      RETVALUE(RFAILED);
   }

   /*Check if PDU is of 0 length*/
   SFndLenMsg(siCfg->pdu, &msgLen);
   if(0 == msgLen)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid pdu "
                   "specified\n"));
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR SI config "
                   "validation done\n"));

   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrSiCfg */
#endif /*RGR_SI_SCH*/

/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE

/********************************************************************
 *   UE ACQI, PCQI, RI, SRS and SR Re/Configuration  Validation Functions  *
 *                                                                   *
 *********************************************************************/

 /*
 * @brief Validates the Tx Mode and PUSCH Mode configuration.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrTxmodePuschMode  
 *
 *     Processing Steps:
 *      - Validate whether the configured PUSCH Mode and the
 *         Configured Tx Mode are in the right combination
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgrTxMode     txMde
 *  @param[in]  RgrAprdCqiMode puschMode
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrTxmodePuschMode 
(
 RgSchCellCb     *cellCb,
 RgrTxMode       txMde,
 RgrAprdCqiMode  puschMode,
 RgSchErrInfo    *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrTxmodePuschMode(cellCb, txMde, puschMode,errInfo)
   RgSchCellCb     *cellCb;
   RgrTxMode       txMde;
   RgrAprdCqiMode  puschMode;
   RgSchErrInfo    *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrTxmodePuschMode);

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
            "\n rgSCHCfgVldtRgrTxmodePuschMode(): CellID=%d",
            cellCb->cellId));

   if (txMde == RGR_UE_TM_1 ||  txMde == RGR_UE_TM_2 ||
         txMde == RGR_UE_TM_3 || txMde == RGR_UE_TM_7)
   {
      if (puschMode == RGR_APRD_CQI_MOD12 ||
            puschMode == RGR_APRD_CQI_MOD22  ||
            puschMode == RGR_APRD_CQI_MOD31)
      {
         RETVALUE(RFAILED);
      }
   }

   if (txMde == RGR_UE_TM_4 ||  txMde == RGR_UE_TM_6)
   {
      if (puschMode == RGR_APRD_CQI_MOD20 ||
            puschMode == RGR_APRD_CQI_MOD30)
      {
         RETVALUE(RFAILED);
      }
   }

   if (txMde == RGR_UE_TM_5 )
   {
      if (puschMode != RGR_APRD_CQI_MOD31)
      {
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/*rgSCHCfgVldtRgrTxmodePuschMode ends*/

 /*
 * @brief Validates the UE ACQI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeACqiCfg  
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the ACQI Configuration 
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb        *cellCb,
 *  @param[in] CmLteRnti          crnti,  
 *  @param[in] RgrUeAprdDlCqiCfg   *acqiCfg,  
 *  @param[in] RgrUeTxModeCfg     txMode,
 *  @param[out] RgSchErrInfo       *errInfo

 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeACqiCfg 
(
 RgSchCellCb        *cellCb,
 CmLteRnti          crnti,  
 RgrUeAprdDlCqiCfg  *acqiCfg,  
 RgrUeTxModeCfg     txMode,
 RgSchErrInfo       *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeACqiCfg(cellCb, crnti, acqiCfg,txMode, errInfo)
   RgSchCellCb        *cellCb;
   CmLteRnti          crnti; 
   RgrUeAprdDlCqiCfg  *acqiCfg;
   RgrUeTxModeCfg     txMode;
   RgSchErrInfo       *errInfo;
#endif
{
   RgrTxMode      txMde;
   RgrAprdCqiMode puschMode; 
   TRC2(rgSCHCfgVldtRgrUeACqiCfg);

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
            "\n rgSCHCfgVldtRgrUeACqiCfg(): CellID=%d  UeId =%d CqiCfgType=%d ",
            cellCb->cellId, crnti,  acqiCfg->aprdModeEnum));

   if(acqiCfg->pres)
   {
      if(txMode.pres)
      {
         txMde = txMode.txModeEnum;
         puschMode = acqiCfg->aprdModeEnum;
         if ( ROK != rgSCHCfgVldtRgrTxmodePuschMode(cellCb, txMde,
                  puschMode, errInfo))
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), \
                     "Invalid Aperiodic CQI configuration\n"));
            RETVALUE(RFAILED);
         } 
      }
   }
   RETVALUE(ROK);
}

 /*
 * @brief Validates the Tx Mode and PUCCH Mode configuration.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrTxmodePucchMode  
 *
 *     Processing Steps:
 *      - Validate whether the configured PUCCH Mode and the
 *         Configured Tx Mode are in the right combination
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgrTxMode     txMde
 *  @param[in]  RgrPrdCqiMode     pucchMode
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrTxmodePucchMode 
(
   RgSchCellCb     *cellCb,
   RgrTxMode       txMde,
   RgrPrdCqiMode   pucchMode,
   RgSchErrInfo    *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrTxmodePucchMode(cellCb, txMde, pucchMode,errInfo)
   RgSchCellCb     *cellCb;
   RgrTxMode       txMde;
   RgrPrdCqiMode   pucchMode;
   RgSchErrInfo    *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrTxmodePucchMode);
   
   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
            "\n rgSCHCfgVldtRgrTxmodePucchMode(): CellID=%d",
            cellCb->cellId));

  if (pucchMode == RGR_PRD_CQI_MOD10 || pucchMode == RGR_PRD_CQI_MOD20 )
  {
   if (txMde ==RGR_UE_TM_4 || txMde ==RGR_UE_TM_5 || txMde ==RGR_UE_TM_6)
   {
      RETVALUE(RFAILED);
   }
  }
  else if (pucchMode == RGR_PRD_CQI_MOD11 || pucchMode == RGR_PRD_CQI_MOD21)
  {
     if (txMde ==RGR_UE_TM_1 || txMde ==RGR_UE_TM_2 || txMde ==RGR_UE_TM_3 \
             || txMde ==RGR_UE_TM_7)
   {
      RETVALUE(RFAILED);
   }
  }
   RETVALUE(ROK);
}

/*
 * @brief Validates the UE Periodic CQI, PMI, RI, re/configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUePCqiCfg  
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for Periodic CQI, PMI, RI values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb        *cellCb,
 *  @param[in] CmLteRnti          crnti,  
 *  @param[in] RgrUePrdDlCqiCfg   *cqiCfg,  
 *  @param[in] RgrUeTxModeCfg     txMode,
 *  @param[out] RgSchErrInfo       *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg 
(
 RgSchCellCb        *cellCb,
 CmLteRnti          crnti,  
 RgrUePrdDlCqiCfg   *cqiCfg,
 Bool               hdFdd,
 RgrUeTxModeCfg     txMode,
 RgSchErrInfo       *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg(cellCb, crnti, cqiCfg, hdFdd, 
       txMode, errInfo)
   RgSchCellCb        *cellCb;
   CmLteRnti          crnti; 
   RgrUePrdDlCqiCfg   *cqiCfg;
   Bool               hdFdd;
   RgrUeTxModeCfg     txMode;
   RgSchErrInfo       *errInfo;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg 
   (
    RgSchCellCb        *cellCb,
    CmLteRnti          crnti,  
    RgrUePrdDlCqiCfg   *cqiCfg,  
    RgrUeTxModeCfg     txMode,
    RgSchErrInfo       *errInfo
   )
#else
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg(cellCb, crnti, cqiCfg,  txMode, errInfo)
   RgSchCellCb        *cellCb;
   CmLteRnti          crnti; 
   RgrUePrdDlCqiCfg   *cqiCfg;
   RgrUeTxModeCfg     txMode;
   RgSchErrInfo       *errInfo;
#endif
#endif
{
    /*rg002.301 - MOD - [ccpu00119118] Assigned the txMde with RGR_UE_TM_1 to remove the
     * warning arising during cross compilation*/
   RgrTxMode     txMde; 
   RgrPrdCqiMode pucchMode; 

   TRC2(rgSCHCfgVldtRgrUePCqiCfg);
   
   txMde = RGR_UE_TM_1;
   pucchMode = RGR_PRD_CQI_MOD20;
   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
  "\n rgSCHCfgVldtRgrUePCqiCfg(): CellID=%d  UeId =%d CqiCfgType=%d ",
            cellCb->cellId, crnti,  cqiCfg->type));
#ifdef MSPD
    MSPD_DBG("Periodic CQI Config: type (%u) Rnti(%u) txMode(%u) cqiResIDx(%u) cqi-pmi-CfgIdx(%u)"
	  	" CQIRepType(%u) k(%u), riEna(%u), riCfgIdx(%u), sANCqi(%u), mode(%u)\n",
	  	cqiCfg->type, crnti, txMode.txModeEnum, cqiCfg->cqiSetup.cqiPResIdx, cqiCfg->cqiSetup.cqiPCfgIdx,
         cqiCfg->cqiSetup.cqiRepType,cqiCfg->cqiSetup.k, cqiCfg->cqiSetup.riEna,
         cqiCfg->cqiSetup.riCfgIdx, cqiCfg->cqiSetup.sANCQI, 
         cqiCfg->cqiSetup.prdModeEnum);  
#endif
   if ( RGR_SCH_PCQI_SETUP == cqiCfg->type )
   {
      /*1. Validate for Tx Mode and PUCCH Mode combination*/
      if(txMode.pres)
      {
         txMde = txMode.txModeEnum;
         pucchMode = cqiCfg->cqiSetup.prdModeEnum;
         if ( ROK != rgSCHCfgVldtRgrTxmodePucchMode(cellCb, txMde,
                  pucchMode, errInfo))
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), \
                     "Invalid Tx Mode-PUCCH Mode combination \n"));
            RETVALUE(RFAILED);
         } 
      }

      /*2. Validate for PCQI Reporting Type and PUCCH Mode combination*/
      if((cqiCfg->cqiSetup.cqiRepType==1) && 
              ((pucchMode == RGR_PRD_CQI_MOD20) ||
               (pucchMode == RGR_PRD_CQI_MOD21)))
      {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), \
                     "Invalid Reporting Type-PUCCH Mode combination \n"));
            RETVALUE(RFAILED);
      }

      if((cqiCfg->cqiSetup.cqiRepType==2) && 
              ((pucchMode == RGR_PRD_CQI_MOD10) ||
               (pucchMode == RGR_PRD_CQI_MOD11)))
      {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), \
                     "Invalid Reporting Type-PUCCH Mode combination \n"));
            RETVALUE(RFAILED);
      }

      /*3. Validate CQI/PMI and RI Configuration related parameter values */
      /*TODO- To be compared with configured n2Pucch Index*/
      if (cqiCfg->cqiSetup.cqiPResIdx > RG_SCH_PUCCH_RES_MAX_SUPP ) 
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                  "rgSCHCfgVldtRgrUePCqiCfg : Not Supported or Invalid "
                  "CQI-PUCCH resourceIndex=%d Cfg Val=%d \n", 
                  RG_SCH_PUCCH_RES_MAX_SUPP, cqiCfg->type));
         RETVALUE(RFAILED);
      } 

#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
               (cqiCfg->cqiSetup.cqiPCfgIdx < 7) ||
               (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                     "rgSCHCfgVldtRgrUePCqiCfg : Not Supported or "
                     "Invalid Cfg CQI Min Index Sup =%d "
                     "Max Index Sup=%d  Cfg Val=%d\n", RG_SCH_ICQI_MIN_SUPP, 
                     RG_SCH_ICQI_MAX_SUPP, cqiCfg->type));
            RETVALUE(RFAILED);
         }
      }
      else 
      {
         if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
               (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                     "rgSCHCfgVldtRgrUePCqiCfg : Not Supported or "
                     "Invalid Cfg CQI Min Index Sup =%d "
                     "Max Index Sup=%d  Cfg Val=%d\n", RG_SCH_ICQI_MIN_SUPP, 
                     RG_SCH_ICQI_MAX_SUPP, cqiCfg->type));
            RETVALUE(RFAILED);
         }
      }
#else
      if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
            (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                  "rgSCHCfgVldtRgrUePCqiCfg : Not Supported or "
                  "Invalid Cfg CQI Min Index Sup =%d "
                  "Max Index Sup=%d  Cfg Val=%d\n", RG_SCH_ICQI_MIN_SUPP, 
                  RG_SCH_ICQI_MAX_SUPP, cqiCfg->type));
         RETVALUE(RFAILED);
      }
#endif

      if((cqiCfg->cqiSetup.cqiRepType < RGR_UE_PCQI_WB_REP) ||
            (cqiCfg->cqiSetup.cqiRepType > RGR_UE_PCQI_SB_REP))
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                  "rgSCHCfgVldtRgrUePCqiCfg : Invalid Cfg CQI Report"
                  "ModeCfg Val=%d\n", cqiCfg->type));
         RETVALUE(RFAILED);
      }

      if((cqiCfg->cqiSetup.k < RG_SCH_CQI_K_MIN) ||
            (cqiCfg->cqiSetup.k > RG_SCH_CQI_K_MAX))
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                  "rgSCHCfgVldtRgrUePCqiCfg : Invalid CQI Cfg "
                  "K Cfg Val=%d\n", cqiCfg->type));
         RETVALUE(RFAILED);
      }

#ifndef LTE_TDD
      if (cqiCfg->cqiSetup.cqiPCfgIdx == RG_SCH_ICQI_RESV_FDD ) 
      {  
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                  "rgSCHCfgVldtRgrUePCqiCfg: Reserved value Cfg =%d\n", 
                  cqiCfg->cqiSetup.cqiPResIdx));
         RETVALUE(RFAILED);
      }
#endif

      /* 4. Check RI Configuration values */
       if(cqiCfg->cqiSetup.riEna == TRUE)
      {
         if(txMode.pres)
         {
            if(((txMde != RGR_UE_TM_3)  && (txMde != RGR_UE_TM_4)))
              {
                  RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                  "rgSCHCfgVldtRgrUePCqiCfg: Invalid Transmission Mode =%d\n",
                  txMde));
                  RETVALUE(RFAILED);
              }
         }
         
         if(cqiCfg->cqiSetup.riCfgIdx > RG_SCH_IRI_MAX_SUPP)
         {
          RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                  "rgSCHCfgVldtRgrUePCqiCfg: Invalid Index RI value Cfg =%d\n",
                  cqiCfg->cqiSetup.riCfgIdx));
          RETVALUE(RFAILED);
         }
      }
   }

  
   RETVALUE(ROK);
}

/*
 * @brief Validates the UE SRS Re/Configuation request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeUlSrsCfg 
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for SRS values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb       *cellCb,
 *  @param[in] CmLteRnti         crnti,  
 *  @param[in] RgrUeUlSrsCfg     *srsCfg,  
 *  @param[out] RgSchErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg 
(
 RgSchCellCb       *cellCb,
 CmLteRnti         crnti,  
 RgrUeUlSrsCfg     *srsCfg,
 Bool              hdFdd,
 RgSchErrInfo      *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, crnti, srsCfg, hdFdd, errInfo)
   RgSchCellCb     *cellCb;
   CmLteRnti       crnti; 
   RgrUeUlSrsCfg   *srsCfg;
   Bool            hdFdd;
   RgSchErrInfo    *errInfo;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg 
(
 RgSchCellCb       *cellCb,
 CmLteRnti         crnti,  
 RgrUeUlSrsCfg     *srsCfg,  
 RgSchErrInfo      *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, crnti, srsCfg,  errInfo)
   RgSchCellCb     *cellCb;
   CmLteRnti       crnti; 
   RgrUeUlSrsCfg   *srsCfg;
   RgSchErrInfo    *errInfo;
#endif
#endif
{
   TRC2(rgSCHCfgVldtRgrUeUlSrsCfg);
   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
   "\n rgSCHCfgVldtRgrUeUlSrsCfg(): CellID=%d  UeId =%d SrsCfgType=%d ", 
            cellCb->cellId, crnti,  srsCfg->type));


   if ( RGR_SCH_SRS_SETUP == srsCfg->type )
   {
      /*  1. Validate SRS Configuration related parameter values  */
      /* 1.1 iSRS should be 0-636; Ref: 36.213. Table 8.2-1 */
#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if ( (srsCfg->srsSetup.srsCfgIdx < 7) || 
               (srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP) )
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                     "rgSCHCfgVldtRgrUeUlSrsCfg: Not Supported or Invalid Cfg "
                     "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d\n",
                     RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                     srsCfg->srsSetup.srsCfgIdx));
            RETVALUE(RFAILED);
         }
      }
      else
      {
         if ( srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP )
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                     "rgSCHCfgVldtRgrUeUlSrsCfg: Not Supported or Invalid Cfg"
                     "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d\n",
                     RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                     srsCfg->srsSetup.srsCfgIdx));
            RETVALUE(RFAILED);
         }

      }
#else
      if ( srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP )
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                  "rgSCHCfgVldtRgrUeUlSrsCfg: Not Supported or Invalid Cfg" 
                  "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d\n",
                  RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                  srsCfg->srsSetup.srsCfgIdx));
         RETVALUE(RFAILED);
      }
#endif

      if ( srsCfg->srsSetup.fDomPosi > RG_SCH_SRS_FREQDOM_POS_MAX )
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                  "rgSCHCfgVldtRgrUeUlSrsCfg: Not Supported or Invalid Cfg" 
                  "SRS Min Freq Domain Position =%d"   
                  "Max Freq Domain Position =%d  Cfg Val=%d\n",
                  RG_SCH_SRS_FREQDOM_POS_MIN, RG_SCH_SRS_FREQDOM_POS_MAX,
                  srsCfg->srsSetup.srsCfgIdx));
         RETVALUE(RFAILED);
      }

      if ( srsCfg->srsSetup.txComb > RG_SCH_SRS_TXCOMB_MAX )
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                  "rgSCHCfgVldtRgrUeUlSrsCfg: Not Supported or Invalid Cfg" 
                  "SRS Min TX Comb =%d   Max TX Comb =%d  Cfg Val=%d\n",
                  RG_SCH_SRS_TXCOMB_MIN, RG_SCH_SRS_TXCOMB_MAX,
                  srsCfg->srsSetup.srsCfgIdx));
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/*
 * @brief Validates the UE SR Re/configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeSrCfg 
 *
 *     Processing Steps:
 *      - Validate the UE re/configuration request from RRC to MAC at CFG:
 *        validate the value range for SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb      *cellCb,
 *  @param[in] CmLteRnti        crnti,  
 *  @param[in] RgrUeSrCfg       *srCfg,  
 *  @param[out] RgSchErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD 
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg 
(
 RgSchCellCb      *cellCb,
 CmLteRnti        crnti,  
 RgrUeSrCfg       *srCfg,
 Bool             hdFdd,
 RgSchErrInfo     *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg(cellCb, crnti, srCfg, hdFdd, errInfo)
   RgSchCellCb    *cellCb;
   CmLteRnti      crnti; 
   RgrUeSrCfg     *srCfg;
   Bool           hdFdd;
   RgSchErrInfo   *errInfo;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg 
(
 RgSchCellCb      *cellCb,
 CmLteRnti        crnti,  
 RgrUeSrCfg       *srCfg,  
 RgSchErrInfo     *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg(cellCb, crnti, srCfg, errInfo)
   RgSchCellCb    *cellCb;
   CmLteRnti      crnti; 
   RgrUeSrCfg     *srCfg;
   RgSchErrInfo   *errInfo;
#endif
#endif
{

   TRC2(rgSCHCfgVldtRgrUeSrCfg);

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
            "\n rgSCHCfgVldtRgrUeSrCfg(): CellID=%d  UeId =%d CqiCfgType=%d ", 
            cellCb->cellId, crnti,  srCfg->type));


   if ( RGR_SCH_SR_SETUP == srCfg->type )
   {
      /* 1. Validate SR Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
               ( srCfg->srSetup.srCfgIdx < 5 ) ||
               ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                     "rgSCHCfgVldtRgrUeSrCfg Invalid SR Index Cfg =%d \
                      Max Supp=%d,Min Supp=%d\n", srCfg->srSetup.srCfgIdx, 
                     RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP));

            RETVALUE(RFAILED);
         }
      }
      else
      {
         if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
               ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                     "rgSCHCfgVldtRgrUeSrCfg Invalid SR Index Cfg =%d \
                      Max Supp=%d,Min Supp=%d\n", srCfg->srSetup.srCfgIdx, 
                     RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP));

            RETVALUE(RFAILED);
         }
      }
#else
      if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
            ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                  "rgSCHCfgVldtRgrUeSrCfg Invalid SR Index Cfg =%d \
                  Max Supp=%d,Min Supp=%d\n", srCfg->srSetup.srCfgIdx, 
                  RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP));

         RETVALUE(RFAILED);
      }
#endif
   } 
   RETVALUE(ROK);
}


/*
 * @brief Validates the UE Aperiodic & Periodic  CQI, PMI, RI, SRS and SR Configuration 
 *             request from RRM to MAC.
 *
 * @details
 *
 *     Function :rgSCHCfgVldtCqiSrSrsUeCfg 
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for Aperiodic & Periodic CQI, PMI, RI , SRS and SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeCfg 
(
 RgSchCellCb       *cellCb,  
 RgrUeCfg          *ueCfg,  
 RgSchErrInfo      *errInfo
 )
#else
PRIVATE S16  rgSCHCfgVldtCqiSrSrsUeCfg(cellCb, ueCfg,  errInfo)
   RgSchCellCb     *cellCb;  
   RgrUeCfg        *ueCfg;
   RgSchErrInfo    *errInfo;
#endif
{

   TRC2(rgSCHCfgVldtCqiSrSrsUeCfg);

   RGSCHDBGPRM(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
            "rgSCHCfgVldtCqiSrSrsUeCfg cellId = %d ueId=%d\n", 
            cellCb->cellId, ueCfg->crnti));

   /*  1. Validate UE Aperiodic CQI related parameters */
   if ( ROK != rgSCHCfgVldtRgrUeACqiCfg (cellCb, ueCfg->crnti,
            &ueCfg->ueDlCqiCfg.aprdCqiCfg, ueCfg->txMode, errInfo ))
   {
      RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
       "rgSCHCfgVldtCqiSrSrsUeCfg: Invalid Aperiodic CQI configuration\n"));
      RETVALUE(RFAILED);
   }

   /*  1. Validate UE Periodic CQI/PMI, SRS and SR related parameters */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueCfg->crnti,
            &ueCfg->ueDlCqiCfg.prdCqiCfg, ueCfg->isHdFddEnbld, 
            ueCfg->txMode, errInfo ))
#else
      if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueCfg->crnti,
             &ueCfg->ueDlCqiCfg.prdCqiCfg, ueCfg->txMode, errInfo ))
#endif
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
          "rgSCHCfgVldtCqiSrSrsUeCfg: Invalid Periodic CQI configuration\n"));
         RETVALUE(RFAILED);
      }

   /*  2. Validate SRS Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueCfg->crnti, &ueCfg->srsCfg,
            ueCfg->isHdFddEnbld, errInfo ))
#else
      if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueCfg->crnti, 
                  &ueCfg->srsCfg, errInfo ))
#endif
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
           "rgSCHCfgVldtCqiSrSrsUeCfg: Invalid SRS  configuration\n"));
         RETVALUE(RFAILED);
      } 

   /*  3. Validate SR Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueCfg->crnti, &ueCfg->srCfg,
            ueCfg->isHdFddEnbld, errInfo))
#else
      if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueCfg->crnti, 
                  &ueCfg->srCfg, errInfo))
#endif
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
            "rgSCHCfgVldtCqiSrSrsUeCfg: Invalid SR  configuration\n"));
         RETVALUE(RFAILED);
      }

   RETVALUE(ROK);

}



/*****************************************************************
 *   UE PCQI, RI, SRS and SR Re Configuration  Validation Functions  *
 *                                                                *
 ******************************************************************/
/*
 * @brief Validates the UE Periodic CQI, PMI, RI, SRS and SR 
 * Re-configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtCqiSrSrsUeReCfg 
 *
 *     Processing Steps:
 *      - Validate the UE Re configuration request from RRC to MAC at CFG:
 *        validate the value range for Periodic CQI, PMI, RI, SRS and SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]   RgSchCellCb *cellCb
 *  @param[in]   RgSchUeCb   *ueCb
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeReCfg 
(
 RgSchCellCb          *cellCb,
 RgSchUeCb            *ueCb,
 RgrUeRecfg           *ueReCfg,  
 RgSchErrInfo         *errInfo
 )
#else
PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeReCfg(cellCb, ueCb, ueReCfg, errInfo)
   RgSchCellCb          *cellCb;  
   RgSchUeCb            *ueCb;
   RgrUeRecfg           *ueReCfg;
   RgSchErrInfo         *errInfo;
#endif
{
   RgrUeTxModeCfg     txMode;
   TRC3(rgSCHCfgVldtCqiSrSrsUeReCfg);

   txMode.pres=TRUE;
   txMode.tmTrnstnState = RGR_TXMODE_RECFG_CMPLT;
   if ((ueReCfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG) &&
         (ueReCfg->txMode.pres == TRUE))
   {
      txMode.txModeEnum = ueReCfg->txMode.txModeEnum;
   }
   else
   {
      txMode.txModeEnum = ueCb->mimoInfo.txMode;
   }

   /*    1. Validate UE CQI/PMI, SRS and SR related parameters */

   if ( ueReCfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG ) 
   { 
      if ( ROK != rgSCHCfgVldtRgrUeACqiCfg (cellCb, ueReCfg->oldCrnti,     
               &ueReCfg->aprdDlCqiRecfg, txMode, errInfo ))
      {
         RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
         "rgSCHCfgVldtCqiSrSrsUeReCfg : Invalid Aperiodic \
           CQI configuration\n"));
         RETVALUE(RFAILED);
      }
   }


   /*    2. Validate UE CQI/PMI, SRS and SR related parameters */

   if ( ueReCfg->ueRecfgTypes & RGR_UE_PCQI_RECFG ) 
   { 
#ifdef LTEMAC_HDFDD   
      if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueReCfg->oldCrnti,     
               &ueReCfg->cqiCfg, ueReCfg->isHdFddEnbld, txMode, errInfo ))
#else
         if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueReCfg->oldCrnti,     
                  &ueReCfg->cqiCfg, txMode, errInfo ))
#endif

         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
            "rgSCHCfgVldtCqiSrSrsUeReCfg : Invalid Periodic \
                                  CQI configuration\n"));
            RETVALUE(RFAILED);
         }
   }

   if(ueReCfg->ueRecfgTypes & RGR_UE_SRS_RECFG )
   {
#ifdef LTEMAC_HDFDD
      if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueReCfg->oldCrnti,
               &ueReCfg->srsCfg, ueReCfg->isHdFddEnbld, errInfo ))
#else
         if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueReCfg->oldCrnti,
                  &ueReCfg->srsCfg, errInfo ))
#endif
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
              "rgSCHCfgVldtCqiSrSrsUeReCfg: Invalid SRS  configuration\n"));
            RETVALUE(RFAILED);
         } 

   }

   if ( ueReCfg->ueRecfgTypes & RGR_UE_SR_RECFG )
   {
#ifdef LTEMAC_HDFDD
      if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueReCfg->oldCrnti, 
            &ueReCfg->srCfg, ueReCfg->isHdFddEnbld, errInfo))
#else
         if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueReCfg->oldCrnti, 
            &ueReCfg->srCfg, errInfo))
#endif
         {
            RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
                     "rgSCHCfgVldtCqiSrSrsUeCfg:Invalid SR configuration\n"));
            RETVALUE(RFAILED);
         }
   }

   RETVALUE(ROK);
}


/*****************************************************************
 *   UE ACQI, PCQI, RI, SRS SR Configuration Functions      *
 *                                                                *
 ******************************************************************/
   /**
    * @brief Handles Aperiodic CQI , PMI, RI  configuration for a UE.
    *
    * @details
    *
    *     Function : rgSCHCfgACqiUeCfg 
    *
    *     Invoking Module Processing:
    *      - This shall be invoked by SCH_GOM at UE configuration. It 
    *      shall do the validations for the spec-defined values.
    *        
    *     Processing Steps:
    *     
    *     - Return ROK
    *
    *  @param[in]  RgSchCellCb  *cell
    *  @param[in]  RgSchUeCb    *ue
    *  @param[in]  RgrUeAprdDlCqiCfg *aCqiCfg
    *  @param[in]  CmLteUeCategory        ueCat
    *  @return  S16
    *      -# ROK
    *
    **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgACqiUeCfg
(
 RgSchCellCb         *cellCb, 
 RgSchUeCb           *ueCb, 
 RgrUeAprdDlCqiCfg   *aCqiCfg,
 CmLteUeCategory     ueCat
 )
#else
PUBLIC S16 rgSCHCfgACqiUeCfg(cellCb, ueCb, aCqiCfg, ueCat)
   RgSchCellCb       *cellCb; 
   RgSchUeCb         *ueCb; 
   RgrUeAprdDlCqiCfg *aCqiCfg;
   CmLteUeCategory   ueCat;
#endif
{
   U8     M;  /*Num of Subbands -- Applicable only for Mode 2-0 and 2-2*/
   U8     k;  /*SubBand Size (RB)  -- 
              Holds different values depending on Mode*/
   /*rg002.301 - MOD -[ccpu00119118] Initialized the variables to zero */
   U8     cqiPmiSzR1;   /*CQIPMI Size for Rank =1*/
   U8     cqiPmiSzRn1; /*CQIPMI Size for Rank > 1*/

   TRC3(rgSCHCfgACqiUeCfg);

   cqiPmiSzR1 = 0;
   cqiPmiSzRn1 = 0;
   
   RGSCHDBGPRM(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
            "rgSCHCfgACqiUeCfg cellId =%d, ueId = %d, Config Presence =%d\n",
            cellCb->cellId,  ueCb->ueId, aCqiCfg->pres));

   ueCb->acqiCb.aCqiCfg.pres = aCqiCfg->pres;
   ueCb->acqiCb.aCqiCfg.aprdModeEnum = aCqiCfg->aprdModeEnum;

   if(aCqiCfg->pres)
   {           
      switch(aCqiCfg->aprdModeEnum)
      {
         case RGR_APRD_CQI_MOD12:
         case RGR_APRD_CQI_MOD30:
         case RGR_APRD_CQI_MOD31:
            RG_SCH_GET_CQI_K_VAL(cellCb->bwCfg.dlTotalBw, k);
            ueCb->acqiCb.N = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, k);
            ueCb->acqiCb.k = k;
            break;

         case RGR_APRD_CQI_MOD20:
         case RGR_APRD_CQI_MOD22:
            RG_SCH_GET_SBCQI_M_K_VAL(cellCb->bwCfg.dlTotalBw, M, k);
            ueCb->acqiCb.N = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, k);
            ueCb->acqiCb.k = k;
            ueCb->acqiCb.M = M;
            ueCb->acqiCb.L = RgSCHUeAcqi2022LBitWidth[M-1][ueCb->acqiCb.N-1];
            break;

         default:
            break; 
      }
      if(ueCb->mimoInfo.txMode == RGR_UE_TM_3 || 
            ueCb->mimoInfo.txMode == RGR_UE_TM_4)
      {
         if(cellCb->numTxAntPorts ==2)
         {
            ueCb->acqiCb.riNumBits = 1;
         }
         else if(cellCb->numTxAntPorts ==4)
         {
            if(ueCat == CM_LTE_UE_CAT_5)
            {
               ueCb->acqiCb.riNumBits = 2;
            }
            else
            {
               ueCb->acqiCb.riNumBits = 1;
            }
         }
      }
      rgSCHCfgUtlFetchAcqiBitSz(ueCb, cellCb->numTxAntPorts, 
            &cqiPmiSzR1, &cqiPmiSzRn1);
      ueCb->acqiCb.cqiPmiSzR1 = cqiPmiSzR1;
      ueCb->acqiCb.cqiPmiSzRn1 = cqiPmiSzRn1;
   }
   RETVALUE(ROK);
}

/**
 * @brief Handles Periodic CQI , PMI, RI  configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic CQI  related configuration, 
 *      - If Periodic CQI/PMI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of CQI/PMI or RI Tranmission instance.
 - Update the CQI offset and CQI perodicity information
 - Add Ue to cell's list  
 *        
 *
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of RI Tranmission instance.
 - Update the  RI offset and RI perodicity information
 *      
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUePrdDlCqiCfg *cqiCfg
 *  @param[in]  CmLteUeCategory   ueCat
 *  @return  S16
 *      -# ROK
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgPCqiUeCfg
(
 RgSchCellCb         *cellCb, 
 RgSchUeCb           *ueCb, 
 RgrUePrdDlCqiCfg    *cqiCfg,
 CmLteUeCategory     ueCat
 )
#else
PUBLIC S16 rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat)
   RgSchCellCb       *cellCb; 
   RgSchUeCb         *ueCb; 
   RgrUePrdDlCqiCfg  *cqiCfg;
   CmLteUeCategory   ueCat;
#endif
{  
   U16    crntTime;           
   U16    cqiTrInstTime; 
   U8     j;  /*Bandwidth Parts*/
   U8     temp; 
   U8     loop;
   TRC3(rgSCHCfgPCqiUeCfg);

   RGSCHDBGINFO(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
            "rgSCHCfgPCqiUeCfg cellId =%d, ueId = %d, CfgType =%d\n",
            cellCb->cellId,  ueCb->ueId, cqiCfg->type));
   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)+
                  (cellCb->crntTime.subframe);
   /* Periodic CQI is setup  */
   if (cqiCfg->type == RGR_SCH_PCQI_SETUP)
   {   
      for(loop = 0; loop < MAX_CQI_RI_RPT_BUFF;loop++)
      {
         ueCb->rawCqiBitW[loop].type = 2; /* setting invalid  type*/
      }
      /*  1. Copy the Received CQI Cfg parameters to ueCb  */
      cmMemcpy((U8 *)&ueCb->cqiCb.cqiCfg, (U8 *)cqiCfg, 
            sizeof(RgrUePrdDlCqiCfg));

      /*  2. Compute Periodic CQI Periodicity and subframe offset   */
#ifndef LTE_TDD           
      rgSCHUtlGetCfgPerOff(RG_SCH_FDD_PCQI_TBL, cqiCfg->cqiSetup.cqiPCfgIdx,
            &ueCb->cqiCb.cqiPeri, &ueCb->cqiCb.cqiOffset);      
#else
      rgSCHUtlGetCfgPerOff( RG_SCH_TDD_PCQI_TBL, 
            cqiCfg->cqiSetup.cqiPCfgIdx,
            &ueCb->cqiCb.cqiPeri, &ueCb->cqiCb.cqiOffset);    
#endif


      RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
               "\n rgSCHCfgPCqiUeCfg(): CQI Peri=%d, CQI Offset=%d", 
               ueCb->cqiCb.cqiPeri,ueCb->cqiCb.cqiOffset));

      cqiTrInstTime = ((ueCb->cqiCb.cqiPeri+crntTime) - ueCb->cqiCb.cqiOffset)
         %ueCb->cqiCb.cqiPeri;
      ueCb->cqiCb.nCqiTrIdx = (crntTime + 
            (ueCb->cqiCb.cqiPeri - cqiTrInstTime));
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception req
       * in FDD*/
#ifdef LTE_TDD
      if(ueCb->cqiCb.nCqiTrIdx  <= (crntTime + TFU_DELTA))
#else
      if(ueCb->cqiCb.nCqiTrIdx  <= (crntTime + TFU_RECPREQ_DLDELTA))
#endif
      {
         ueCb->cqiCb.nCqiTrIdx = ueCb->cqiCb.nCqiTrIdx + ueCb->cqiCb.cqiPeri;
      }
      ueCb->cqiCb.nCqiTrIdx = ueCb->cqiCb.nCqiTrIdx
         %RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
#ifdef MSPD
      MSPD_DBG("CQI Config: idx(%u) Periodicity %u Offset %u uePosInQ (%u)\n", 
      cqiCfg->cqiSetup.cqiPCfgIdx, ueCb->cqiCb.cqiPeri, ueCb->cqiCb.cqiOffset,ueCb->cqiCb.nCqiTrIdx);
#if 0 /*THIS PWAV CHNAGE IT IS HAS TO BE TAKEN FOR EVERY RELEASE*/
      if (ueCb->cqiCb.cqiPeri != 40)
      {
stop_printf("CQIDBG: Incorrect Periodicity for UE[%d] cqi[%d] ri[%d]\n", 
               ueCb->ueId, ueCb->cqiCb.cqiPeri);
      }
#endif
#endif
      if(RGR_UE_PCQI_SB_REP == cqiCfg->cqiSetup.cqiRepType)
      {
         U8     k;  /*SubBand Size (RB) */
         RG_SCH_GET_CQI_J_VAL(cellCb->bwCfg.dlTotalBw, j);
         RG_SCH_GET_CQI_K_VAL(cellCb->bwCfg.dlTotalBw, k);
         ueCb->cqiCb.J = j; /*Number of Bandwidth Parts*/
         /*h: reporting instances required for a complete CQI/PMI report */
         /*j:Number of bandwidth parts; k: Subband Size*/
         ueCb->cqiCb.h = (ueCb->cqiCb.cqiCfg.cqiSetup.k *j )+1; 
         temp = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, k);
          ueCb->cqiCb.label = 
            RGSCH_CEILING(rgSCHUtlLog32bitNbase2
                  (RGSCH_CEIL(temp, j)));
      }
      /*    Place the UE in cellCb->tIUeLstCp  */
      ueCb->cqiCb.cqiLstEnt.node=(PTR) ueCb;


      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nCqiTrIdx].cqiLst,
            &ueCb->cqiCb.cqiLstEnt);  

      RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
               "\n rgSCHCfgPCqiUeCfg():"
               "  CrntTime=%d  Next CqiTrInstTime=%d  Index Stored at=%d  ",
               crntTime, cqiTrInstTime, ueCb->cqiCb.nCqiTrIdx));

      /* 4. Rank Indicator Cfg handler */
      rgSCHCfgRiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }
   else
   { 
      ueCb->cqiCb.cqiCfg.type =  RGR_SCH_PCQI_REL;
      if(ueCb->cqiCb.nCqiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nCqiTrIdx].cqiLst,\
               &ueCb->cqiCb.cqiLstEnt);
      } 
      if(ueCb->cqiCb.nRiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst, \
               &ueCb->cqiCb.riLstEnt);
      }
      ueCb->cqiCb.nCqiTrIdx = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.nRiTrIdx   = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.riDist   = RG_SCH_INVALID_IDX; 
   } 
   RETVALUE(ROK);
}

/**
 * @brief Handles Periodic  RI  configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgRiUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of RI Tranmission instance.
 - Update the  RI offset and RI perodicity information
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUePrdDlCqiCfg *cqiCfg
 *  @param[in]  CmLteUeCategory   ueCat
 *  @return  S16
 *      -# ROK
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRiUeCfg
(
 RgSchCellCb        *cellCb, 
 RgSchUeCb          *ueCb, 
 RgrUePrdDlCqiCfg   *cqiCfg,
 CmLteUeCategory    ueCat
 )
#else
PUBLIC S16 rgSCHCfgRiUeCfg(cellCb, ueCb, cqiCfg, ueCat)
   RgSchCellCb       *cellCb; 
   RgSchUeCb         *ueCb; 
   RgrUePrdDlCqiCfg  *cqiCfg;
   CmLteUeCategory   ueCat;
#endif
{
   U16     crntTime;  
   U16     riTrInsTime;  
   U8      j; /*Bandwidth parts. Valid for Modes 2-0, 2-1*/         
   U16     periodicity; 
   U16    tempIdx; 

   TRC3(rgSCHCfgRiUeCfg);

   RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
            "\n rgSCHPRiUeCfg(): CellID=%d  UeId =%d  RI CfgEna =%d", 
            cellCb->cellId, ueCb->ueId, cqiCfg->cqiSetup.riEna));


   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)
      +(cellCb->crntTime.subframe);
   /* 1. Rank Indicator is enabled  */
   if(cqiCfg->cqiSetup.riEna)
   {

      rgSCHUtlGetCfgPerOff(RG_SCH_RI_TBL, 
            cqiCfg->cqiSetup.riCfgIdx,
            &ueCb->cqiCb.riPeri, &ueCb->cqiCb.riOffset);

      RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
               "\n rgSCHCfgRiUeCfg(): RI Peri=%d, RI Offset=%d", 
               ueCb->cqiCb.riPeri,ueCb->cqiCb.riOffset));

      if(RGR_UE_PCQI_WB_REP == cqiCfg->cqiSetup.cqiRepType)
      {
         /* 
            1. wideband RI reporting is configured 
            (Mode 1-0 or 1-1)
            (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )Mod(NCqiperiod
          *MriPeriod)=0  
          */ 
         periodicity = ueCb->cqiCb.cqiPeri * ueCb->cqiCb.riPeri; 
#ifdef MSPD
#if 0 /*THIS PWAV CHNAGE IT IS HAS TO BE TAKEN FOR EVERY RELEASE*/
         if (periodicity != 80)
         {
            stop_printf("CQIDBG: Incorrect Periodicity for UE[%d] cqi[%d] ri[%d]\n", 
                  ueCb->ueId, ueCb->cqiCb.cqiPeri, ueCb->cqiCb.riPeri);
         }
#endif
#endif
         riTrInsTime = ((periodicity +crntTime )- \
               ueCb->cqiCb.cqiOffset + ueCb->cqiCb.riOffset)\
                       % periodicity;
         tempIdx = (crntTime + (periodicity -riTrInsTime));
         /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception 
          * req in FDD*/
#ifdef LTE_TDD
         if (tempIdx  <= (crntTime+ TFU_DELTA))
#else
         if (tempIdx  <= (crntTime + TFU_RECPREQ_DLDELTA))
#endif
         {
            tempIdx = tempIdx + periodicity;
         }
         ueCb->cqiCb.nRiTrIdx = tempIdx 
            % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
         if(periodicity >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
         {
         /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception 
          * req in FDD*/
#ifdef LTE_TDD
          ueCb->cqiCb.riDist = rgSCHUtlFindDist((U16)(crntTime+TFU_DELTA),
                                        (U16) tempIdx);
#else
          ueCb->cqiCb.riDist = rgSCHUtlFindDist((U16)(crntTime + TFU_RECPREQ_DLDELTA),
                                        (U16) tempIdx);
#endif
         }
         else
         {
            ueCb->cqiCb.riDist =0; 
         }

      }  
      else
      {
         /*
         *  Where Widesband and Subband RI reporting is configured
         *   (Mode 2-0 or 2-1 )
         *   (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )
         *   Mod(H. NCqiperiod *MriPeriod )=0 
         *   where H= J * K +1;  J=Number of bandwidth parts(BW/subsize). 
         *   K is RGR interf input 
         */

         RG_SCH_GET_CQI_J_VAL(cellCb->bwCfg.dlTotalBw, j);
         ueCb->cqiCb.h = (ueCb->cqiCb.cqiCfg.cqiSetup.k *j )+1;  
         periodicity = ueCb->cqiCb.h * ueCb->cqiCb.cqiPeri * 
                                  ueCb->cqiCb.riPeri; 
         riTrInsTime = ((periodicity +crntTime)\
               - ueCb->cqiCb.cqiOffset + ueCb->cqiCb.riOffset) %periodicity;
         tempIdx = (crntTime + ((ueCb->cqiCb.h * \
             ueCb->cqiCb.cqiPeri * ueCb->cqiCb.riPeri)-riTrInsTime));
         /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception 
          * req in FDD*/
#ifdef LTE_TDD
         if (tempIdx <= (crntTime+ TFU_DELTA))
#else
         if (tempIdx <= (crntTime + TFU_RECPREQ_DLDELTA))
#endif
         {
            tempIdx = tempIdx + periodicity; 
         }
         ueCb->cqiCb.nRiTrIdx = tempIdx 
            % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
         if(periodicity >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
         {  
         /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception 
          * req in FDD*/
#ifdef LTE_TDD
          ueCb->cqiCb.riDist = rgSCHUtlFindDist((U16)(crntTime+TFU_DELTA),
                                                (U16) tempIdx);
#else
          ueCb->cqiCb.riDist = rgSCHUtlFindDist((U16)(crntTime + TFU_RECPREQ_DLDELTA),
                                                (U16) tempIdx);
#endif
         }
         else
         {
            ueCb->cqiCb.riDist =0; 
         }
      }
      if(ueCb->mimoInfo.txMode == RGR_UE_TM_3 
            || ueCb->mimoInfo.txMode == RGR_UE_TM_4)
      {
         if (cellCb->numTxAntPorts ==2)
         {
            ueCb->cqiCb.riNumBits = 1;
         }
         else if(cellCb->numTxAntPorts ==4)
         {
            if(ueCat == CM_LTE_UE_CAT_5)
            {
               ueCb->cqiCb.riNumBits = 2;
            }
            else
            {
               ueCb->cqiCb.riNumBits = 1;
            }
         }
      }
      /*    Place the UE in cellCb->tIUeLstCp  */
      ueCb->cqiCb.riLstEnt.node=(PTR) ueCb;

      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst,
            &ueCb->cqiCb.riLstEnt);  
	  
#ifdef MSPD   
	  MSPD_DBG("RI config: idx %u period %u Offset %u posInQ(%u) riDist(%u)"
	  "lst count %u\n", 
	  cqiCfg->cqiSetup.riCfgIdx, ueCb->cqiCb.riPeri, ueCb->cqiCb.riOffset, 
	  ueCb->cqiCb.nRiTrIdx, ueCb->cqiCb.riDist, cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst.count);
#endif

      RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
               "\n rgSCHCfgRiUeCfg(): CrntTime=%d  Next RiTrInstTime=%d"
               "Index Stored at=%d riDis=%d ",
              crntTime, riTrInsTime, ueCb->cqiCb.nRiTrIdx, ueCb->cqiCb.riDist));
   }
   else
   {
      ueCb->cqiCb.nRiTrIdx = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.riDist   = RG_SCH_INVALID_IDX; 
   }

   RETVALUE(ROK);

}

/* @brief Handles SRS configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrsUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SRS  related configuration, 
 *      - If SRS is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of SRS Tranmission instance.
 - Update the  SRS offset and SRS perodicity information
 - Add Ue to cell's srs list
 *      - else
 *        - next occurance transmission instance of SRS  = RG_SCH_INVALID_IDX
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeUlSrsCfg   *srsCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgSrsUeCfg
(
 RgSchCellCb     *cellCb,
 RgSchUeCb       *ueCb,
 RgrUeUlSrsCfg   *srsCfg 
 )
#else
PUBLIC S16 rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg)
   RgSchCellCb     *cellCb;
   RgSchUeCb       *ueCb;
   RgrUeUlSrsCfg   *srsCfg;
#endif
{
   U16      srsTrInsTime;
   U16      crntTime;  
   U16      tempIdx;

   TRC3(rgSCHCfgSrsUeCfg);

   RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
            "\n rgSCHCfgSrsUeCfg(): CellID=%d  UeId =%d  CfgType=%d", 
            cellCb->cellId, ueCb->ueId, srsCfg->type));

   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)
      +(cellCb->crntTime.subframe);

   if(RGR_SCH_SRS_SETUP == srsCfg->type) 
   {
      /*  1. Copy the Received Cfg parameters to local cb  */
      cmMemcpy((U8 *)&ueCb->srsCb.srsCfg, (U8 *)srsCfg, sizeof(RgrUeUlSrsCfg));

#ifndef LTE_TDD
      /* 2.  Compute SRS Offset and Periodicity */
      rgSCHUtlGetCfgPerOff( RG_SCH_FDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &ueCb->srsCb.peri, &ueCb->srsCb.offset);      
#else
      rgSCHUtlGetCfgPerOff( RG_SCH_TDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &ueCb->srsCb.peri, &ueCb->srsCb.offset);      
#endif       

      RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
               "\n rgSCHCfgSrsUeCfg(): SRS Peri=%d, SRS Offset=%d", 
               ueCb->srsCb.peri,ueCb->srsCb.offset));


      /*  3. Compute next Tranmission index for SRS */ 
      /*     Referenence: 36.213 Section:8.2
             i. SRS transmission instances for TDD with  period > 2 and for FDD are 
             ((10*sfn +Ksrs-suframeoffset))/mod(periodicity))  
             FDD: Ksrs is 0...9
             TDD: Ksrs Table 8.2-3: 
             ii.The SRS transmission instances for TDD (periodicity == 2)
             (Ksrs-Toffset)mod(5)==0.  Note: This is not supported now  
      */ 

      srsTrInsTime = ((ueCb->srsCb.peri+crntTime) - ueCb->srsCb.offset)
         %ueCb->srsCb.peri;
     tempIdx = (crntTime + (ueCb->srsCb.peri - srsTrInsTime));
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception 
       * req in FDD*/
#ifdef LTE_TDD
      if (tempIdx  <=  (crntTime + TFU_DELTA))
#else
      if (tempIdx  <=  (crntTime + TFU_RECPREQ_DLDELTA))
#endif
      {
         tempIdx = tempIdx + ueCb->srsCb.peri;
      }
      ueCb->srsCb.nSrsTrIdx =(U16) (tempIdx 
         % RG_SCH_PCQI_SRS_SR_TRINS_SIZE);
      if(ueCb->srsCb.peri >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
      {
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception 
       * req in FDD*/
#ifdef LTE_TDD
         ueCb->srsCb.srsDist = rgSCHUtlFindDist((U8)(crntTime+TFU_DELTA), 
                                                    (U16)tempIdx); 
#else
         ueCb->srsCb.srsDist = rgSCHUtlFindDist((U8)(crntTime + TFU_RECPREQ_DLDELTA), 
                                                    (U16)tempIdx); 
#endif
      }
      else
      {
         ueCb->srsCb.srsDist = 0; 
      }

      /*UE Tx Antenna Selection - START*/
      if(ueCb->ul.ulTxAntSel.pres == TRUE )
      {
          /*for both partial and full sounding bandwidth, 
           and when frequency hopping is disabled */
         ueCb->srsCb.selectedAnt = (crntTime/ueCb->srsCb.peri)%2; 
      }
      else
      {
         /* TS 36.213 specifies that if Tx Antenna Selection is 
            disabled/not supported then its Port 0*/
         ueCb->srsCb.selectedAnt=0; 
      }
      ueCb->validTxAnt = ueCb->srsCb.selectedAnt; 
      /*UE Tx Antenna Selection - ENDS*/

      ueCb->srsCb.srsLstEnt.node=(PTR)ueCb;
      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
            &ueCb->srsCb.srsLstEnt);  

      RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
               "\n rgSCHCfgRiUeCfg(): "
               "CrntTime=%d  Next CqiTxInstTime=%d  Index Stored at=%d  ", 
               crntTime, srsTrInsTime, ueCb->srsCb.nSrsTrIdx));

   }
   else
   {
      /*  SRS Release / Not configured */
      ueCb->srsCb.srsCfg.type = RGR_SCH_SRS_REL;
      if(ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
               &ueCb->srsCb.srsLstEnt);
      }
      ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX; 
      ueCb->srsCb.srsLstEnt.node =(PTR) NULLP;
   }

   RETVALUE(ROK);
}


/* * @brief Handles SR configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *      - If SR is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of SR Tranmission instance.
 - Update the  SR offset and SR perodicity information
 - Add Ue to cell->tIUeLstCp[n]  
 *      - else
 *        - next occurance transmission instance of SR = RG_INVALID_SR_ID
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeSrCfg       *srCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgSrUeCfg
(
 RgSchCellCb      *cellCb,
 RgSchUeCb        *ueCb,
 RgrUeSrCfg       *srCfg
 )
#else
PUBLIC S16 rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg)
   RgSchCellCb    *cellCb;
   RgSchUeCb      *ueCb;
   RgrUeSrCfg     *srCfg;
#endif
{
   U16     srTrInsTime;
   U16     crntTime;

   TRC3(rgSCHCfgSrUeCfg);

   RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
            "\n rgSCHCfgSrUeCfg(): CellID=%d  UeId =%d  CfgType=%d", 
            cellCb->cellId, ueCb->ueId, srCfg->type));

   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)
      +(cellCb->crntTime.subframe);
   if(srCfg->type == RGR_SCH_SR_SETUP) 
   {
      /*  1. Copy the Received Cfg parameters to local cb  */
      cmMemcpy((U8 *)&ueCb->srCb.srCfg, (U8 *)srCfg, sizeof(RgrUeSrCfg));


      /*  2. Compute SR periodicity and offset */
      rgSCHUtlGetCfgPerOff( RG_SCH_SR_TBL, 
            srCfg->srSetup.srCfgIdx,
            &ueCb->srCb.peri, &ueCb->srCb.offset);
	  
#ifdef TFU_UPGRADE
	  MSPD_DBG("SR Config: idx(%u), period (%u) offset (%u)\n",
	  	srCfg->srSetup.srCfgIdx,ueCb->srCb.peri, ueCb->srCb.offset);
#endif
      RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
               "\n rgSCHCfgSrUeCfg(): SR Peri=%d, SR Offset=%d", 
               ueCb->srCb.peri,ueCb->srCb.offset));

      /*   3. Compute Next Transmission Instance */

      srTrInsTime = ((ueCb->srCb.peri+crntTime) - ueCb->srCb.offset)
         %ueCb->srCb.peri;
      ueCb->srCb.nSrTrIdx = (crntTime + (ueCb->srCb.peri- srTrInsTime)); 
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception 
       * req in FDD*/
#ifdef LTE_TDD
      if (ueCb->srCb.nSrTrIdx  <= (crntTime + TFU_DELTA))
#else
      if (ueCb->srCb.nSrTrIdx  <= (crntTime + TFU_RECPREQ_DLDELTA))
#endif
      {
         ueCb->srCb.nSrTrIdx = ueCb->srCb.nSrTrIdx +  ueCb->srCb.peri;
      }
      ueCb->srCb.nSrTrIdx = ueCb->srCb.nSrTrIdx 
         % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
      ueCb->srCb.srLstEnt.node= (PTR) ueCb;

      /* 4. Place UE in Cell SR Tranmisison Instance List  */
      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
            &ueCb->srCb.srLstEnt);  
   }
   else
   {
      ueCb->srCb.srCfg.type =  RGR_SCH_SR_REL;

      if(ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
               &ueCb->srCb.srLstEnt);
      }
      ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX; 
      ueCb->srCb.srLstEnt.node = (PTR)NULLP;
   }

   RETVALUE(ROK);
}


/*****************************************************************
 *    UE PCQI, RI, SRS and SR       Re Configuration Functions    *
 *                                                                *
 ******************************************************************/


/* * @brief Handles Periodic CQI, PMI, RI  Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic CQI  related configuration, 
 *      - If Periodic CQI/PMI is re configured(first time enabled),
 *          - Update UE with the configured values.
 *         - Compute and Update next occurance of CQI/PMI or RI Tranmission 
 *         instance.
 *         - Update the CQI offset and CQI perodicity information
 *         - Add Ue to cell's list 
 *      - If Periodic CQI/PMI is re configured(modify),
 *         - Update UE with the configured values.
 *         - Del Ue from cell->tIUeLstCp list 
 *          - Compute and Update next occurance of CQI/PMI or RI Tranmission
 *          instance.
 *         - Update the CQI offset and CQI perodicity information
 *         - Add Ue to cell's list
 *      - If Periodic CQI/PMI is re configured(disabled),
 *        - Update UE with the configured values.
 *         - Del Ue from cell's list  
 *         - Update next occurance of CQI/PMI or RI Tranmission instance.
 *        - next occurance of CQI/PMI = RG_INVALID_CQIPMI_ID
 *
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured(first time enabled),
 *        - Update UE with the configured values.
 *          - Compute and Update next occurance of RI Tranmission instance.
 *         - Update the  RI offset and RI perodicity information
 *      - If Periodic RI is configured(modify),
 *        - Update UE with the configured values.
 *         - Compute and Update next occurance of RI Tranmission instance.
 *         - Update the  RI offset and RI perodicity information
 *      - else
 *        - next occurance of RI  = RG_INVALID_CQIPMI_ID
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *ueCfg
 *
 *  @return  S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgPCqiUeReCfg
(
 RgSchCellCb        *cellCb, 
 RgSchUeCb          *ueCb, 
 RgrUePrdDlCqiCfg   *cqiCfg,
 CmLteUeCategory      ueCat
 )
#else
PUBLIC S16  rgSCHCfgPCqiUeReCfg(cellCb, ueCb, cqiCfg, ueCat)
   RgSchCellCb       *cellCb; 
   RgSchUeCb         *ueCb; 
   RgrUePrdDlCqiCfg  *cqiCfg;
   CmLteUeCategory  ueCat;
#endif
{

   TRC3(rgSCHCfgPCqiUeReCfg);

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
            "\n rgSCHCfgPCqiUeReCfg(): CellID=%d  UeId =%d  cqiCfg Type =%d", 
            cellCb->cellId, ueCb->ueId, cqiCfg->type));
   /* MS_FIX: ccpu00124008 Fix for incorrect check causing missed CQI reception instance */
   if((cqiCfg->type == RGR_SCH_PCQI_SETUP) &&
         (ueCb->cqiCb.cqiCfg.type != RGR_SCH_PCQI_SETUP))
   {
      /* 1. cqi is in Release (Disable) state,  Recfg is allowing Setup (Enable)
      */
      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);

   }
   else if((cqiCfg->type == RGR_SCH_PCQI_SETUP) &&
         (ueCb->cqiCb.cqiCfg.type == RGR_SCH_PCQI_SETUP ))
   {

      /*
         2. Present is SETUP(Enable) state,  Recfg is modifying SETUP(Enable)

         2.1 Delete UE from the cqiList 
         2.2 Set next occurance Transmission instace to "INVALID"
         2.3 Compute Next occurance Transmission instace 
         2.4 Placed ue in Transmission instance list.
         */    
      if(ueCb->cqiCb.nCqiTrIdx != RG_SCH_INVALID_IDX )
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nCqiTrIdx].cqiLst,
               &ueCb->cqiCb.cqiLstEnt); 

      if(ueCb->cqiCb.nRiTrIdx != RG_SCH_INVALID_IDX )
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst,
               &ueCb->cqiCb.riLstEnt); 


      ueCb->cqiCb.cqiLstEnt.next = NULLP;
      ueCb->cqiCb.cqiLstEnt.prev  = NULLP;
      ueCb->cqiCb.nCqiTrIdx = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.nRiTrIdx  = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.riDist    = RG_SCH_INVALID_IDX;

      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }
   else
   {
      /* Present is SETUP(Enable) state,  Recfg is Release(Disable)  */
      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }

   RETVALUE(ROK);
}


/* * @brief Handles SRS  Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrsUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SRS  related re configuration, 
 *      - If SRS is configured modified(First time Enabled),
 *        - Update UE with the configured values.
 *         - Compute and Update next occurance of SRS Tranmission instance.
 *        - Update the  SRS offset and SRS perodicity information
 *        - Add Ue to cell's list  
 *      - If SRS is configured modified(Changed offset or index ),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *      - Update UE with the configured values.
 *      - Compute and Update next occurance of SRS Tranmission instance.
 *      - Update the  SRS offset and SRS perodicity information
 *      - Add Ue to cell's list  
 *      - If SRS is configured modified(disabled),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *        - Update next occurance of SRS Tranmission instance to "INVALID".
 *        - Update the  SRS offset and SRS perodicity information "INVALID"
 *      - else
 *        - ROK 
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeUlSrsCfg    *srsCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgSrsUeReCfg
(
 RgSchCellCb        *cellCb, 
 RgSchUeCb          *ueCb, 
 RgrUeUlSrsCfg      *srsCfg
 )
#else
PUBLIC S16 rgSCHCfgSrsUeReCfg(cellCb, ueCb, srsCfg)
   RgSchCellCb      *cellCb; 
   RgSchUeCb        *ueCb; 
   RgrUeUlSrsCfg    *srsCfg;
#endif
{

   TRC3(rgSCHCfgSrsUeReCfg);

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
            "\n rgSCHCfgSrsUeReCfg(): CellID=%d  UeId =%d  srsCfg Type =%d", 
            cellCb->cellId, ueCb->ueId, srsCfg->type));


   if(( RGR_SCH_SRS_SETUP == srsCfg->type) ||
         ( RGR_SCH_SRS_SETUP != ueCb->srsCb.srsCfg.type ))
   {
      /*     1. Present is Release(Disable) state,  Recfg is allowing
       *     SETUP(Enable)  */
      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   } 
   else if (( RGR_SCH_SRS_SETUP == srsCfg->type ) &&
         ( RGR_SCH_SRS_SETUP  == ueCb->srsCb.srsCfg.type))
   {

      /*     2. Present is SETUP(Eanble) state,  Recfg is modifying SETUP(Enable) 

             2.1 Delete UE from the cqiList 
             2.2 Set next occurance Transmission instance to "INVALID"
             2.3 Compute Next occurance Transmission instance 
             2.4 Placed ue in Transmission instance list.
             */ 
      if (ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
               &ueCb->srsCb.srsLstEnt);
         ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
      }

      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   }
   else
   {
      /* 3. Present is SETUP(Enable) state,  Recfg is Release(Disable)*/ 
      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   }
   RETVALUE(ROK);
}

/* @brief Handles  SR Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. 
 *      It shall do the validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SR  related re configuration, 
 *      - If SR is configured modified(First time Enabled),
 *        - Update UE with the configured values.
 *          - Compute and Update next occurance of SR Tranmission instance.
 *         - Update the  SR offset and SR perodicity information
 *         - Add Ue to cell->tIUeLstCp[n]  
 *      - If SR is configured modified(Changed offset or index ),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *       - Update UE with the configured values.
 *       - Compute and Update next occurance of SR Tranmission instance.
 *       - Update the  SR offset and SR perodicity information
 *       - Add Ue to cell->tIUeLstCp[n]  
 *      - If SR is configured modified(disabled),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *         - Update next occurance of SR Tranmission instance to "INVALID".
 *         - Update the  SR offset and SR perodicity information "INVALID"
 *      - else
 *        - ROK 
 *
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *ueCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgSrUeReCfg
(
 RgSchCellCb    *cellCb, 
 RgSchUeCb      *ueCb, 
 RgrUeSrCfg     *srCfg
 )
#else
PUBLIC S16 rgSCHCfgSrUeReCfg(cellCb, ueCb, srCfg)
   RgSchCellCb   *cellCb; 
   RgSchUeCb     *ueCb; 
   RgrUeSrCfg    *srCfg;
#endif
{
   TRC3(rgSCHCfgSrUeReCfg);

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
            "\n rgSCHCfgSrUeReCfg(): CellID=%d  UeId =%d  SrCfg Type =%d",
            cellCb->cellId, ueCb->ueId, srCfg->type));

   if((srCfg->type == RGR_SCH_SR_SETUP) ||
         (ueCb->srCb.srCfg.type != RGR_SCH_SR_SETUP))
   {
      /*
         1. Present is Release(Disable) state,  Recfg is allowing SETUP(Enable) 
         */
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);
   } 
   else if((srCfg->type == RGR_SCH_SR_SETUP) &&
         (ueCb->srCb.srCfg.type == RGR_SCH_SR_SETUP))
   {

      /*   2. Present is SETUP(Eanble) state,  Recfg is modifying SETUP(Enable) 

           2.1 Delete UE from the cqiList 
           2.2 Compute Next occurance Transmission instace 
           */      
      if(ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX )
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst, 
               &ueCb->srCb.srLstEnt);
         ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
      }
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);

   }
   else
   {
      /* 3. Present is SETUP(Enable) state,  Recfg is Release(Disable) */
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);
   }

   RETVALUE(ROK);
}

/* @brief Handles  ACQI Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgAcqiUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. 
 *      It shall do the validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific ACQI  related re configuration, 
 *          - Check if the ACQI Mode has been changed from the
 *             existing Configuration.
 *     - If the configuration has been changed,
 *          - Call Aperiodic Config function to change the config
 *      - else
 *        - ROK 
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeAprdDlCqiCfg   *acqiCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgAcqiUeReCfg
(
 RgSchCellCb    *cellCb, 
 RgSchUeCb      *ueCb, 
 RgrUeAprdDlCqiCfg   *acqiCfg,
 CmLteUeCategory   ueCat
 )
#else
PUBLIC S16 rgSCHCfgAcqiUeReCfg(cellCb, ueCb, acqiCfg, ueCat)
   RgSchCellCb   *cellCb; 
   RgSchUeCb     *ueCb; 
   RgrUeAprdDlCqiCfg   *acqiCfg;
   CmLteUeCategory   ueCat;
#endif
{
   TRC3(rgSCHCfgAcqiUeReCfg);

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
            "\n rgSCHCfgAcqiUeReCfg(): CellID=%d  UeId =%d  AcqiCfg Pres =%d",
            cellCb->cellId, ueCb->ueId, acqiCfg->pres));

     rgSCHCfgACqiUeCfg(cellCb, ueCb, acqiCfg, ueCat);

   RETVALUE(ROK);
}

/*****************************************************************
 *   UE PCQI, RI, SRS and SR Configuration  Delete                *
 *                                                                *
 *****************************************************************/

/* @brief Free Periodic CQI/PMI/RI, SRS and SR transmission instance 
 * related data structures of this UE from CellCb 
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiSrsSrUeDel
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at Ue deletion.
 *
 *     Processing Steps:
 *      - For SRS Transmission Instance
 *       - if (srsTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srsTxInst*3+2]
 *       - else
 *        - Nothing to do
 *      - For SR Transmission Instance
 *       - if (srTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srTxInst*3+1]
 *       - else
 *        - Nothing to do
 *      - For Periodic CQI/PMI RI  Transmission Instance
 *       - if (pCqiTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srTxInst*3+0]
 *       - else
 *        - Nothing to do
 *      - Return ROK
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @return  S16
 *      -# ROK
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgPCqiSrsSrUeDel 
(
 RgSchCellCb      *cellCb,
 RgSchUeCb        *ueCb
 )
#else
PUBLIC S16 rgSCHCfgPCqiSrsSrUeDel(cellCb, ueCb)
   RgSchCellCb    *cellCb;
   RgSchUeCb      *ueCb;
#endif
{

   TRC3(rgSCHCfgPCqiSrsSrUeDel);

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),\
            "\n rgSCHCfgPCqiSrsSrUeDel(): CellID=%d  UeId =%d ", 
            cellCb->cellId, ueCb->ueId));

   RGSCHDBGPRM(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
            "\n rgSCHCfgPCqiSrsSrUeDel(): SrsTxInst=%d  SrTxinst=%d"  
            "CqiTxInst=%d,Ri TxInst=%d", ueCb->srsCb.nSrsTrIdx, 
            ueCb->srCb.nSrTrIdx,ueCb->cqiCb.nCqiTrIdx, ueCb->cqiCb.nRiTrIdx));


   /* Delete SRS Transmission Instance  */
   if (ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
            &ueCb->srsCb.srsLstEnt); 
      ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
   }

   /* Delete SR Transmission Instance  */
   if (ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
            &ueCb->srCb.srLstEnt); 
      ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
   }


   /* Delete Periodic CQI/PMI  Transmission Instance  */
   if (ueCb->cqiCb.nCqiTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nCqiTrIdx].cqiLst, 
            &ueCb->cqiCb.cqiLstEnt); 
      ueCb->cqiCb.nCqiTrIdx = RG_SCH_INVALID_IDX;

      /* Delete Periodic  RI  Transmission Instance  */

      if (ueCb->cqiCb.nRiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst, 
               &ueCb->cqiCb.riLstEnt); 
         ueCb->cqiCb.nRiTrIdx = RG_SCH_INVALID_IDX;
      }
   }

   RETVALUE(ROK);
}


/* @brief Search the cfgIdx in given table and retrive periodicity & offset
 * @details
 *
 *     Function : rgSCHUtlGetCfgPerOff 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by Cfg Module 
 *
 *     Processing Steps:
 *          binary search for given entry in table
 *          find the periodicty, offset for a given config index from the table
 *      - Return ROK
 *  @param[in]  RgSchPerTbl tbl
 *  @param[in]  U16      cfgIdx
 *  @param[out] U16     *peri 
 *  @param[out] U16     *offset 
 *
 *  @return  S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetCfgPerOff
(
 RgSchPerTbl     tbl,
 U16             cfgIdx, 
 U16             *peri, 
 U16             *offset
 )
#else
PUBLIC S16 rgSCHUtlGetCfgPerOff ( tbl, cfgIdx, peri, offset)
   RgSchPerTbl   tbl;
   U16           cfgIdx;
   U16           *peri; 
   U16           *offset;
#endif
{
   U8     mid;
   U8     min;
   U8     max;
   CONSTANT RgSchUePCqiSrsSrCfgIdxTbl* table;
   TRC3(rgSCHUtlGetCfgPerOff);
  
   /* rg002.301 - ADD - Added the function instead of the MACRO to get the 
    * periodicity table */
   table = rgSCHCfgUtlGetPcqiSrsSrRiTbl ( tbl,&min,&max );
   do{
      mid = (min+max)/2;
      if (( cfgIdx >= table[mid].min) &&
            ( cfgIdx <= table[mid].max))
      {
         *peri = table[mid].peri;
         *offset = cfgIdx - table[mid].offset;
         break;
      } 

      if ( cfgIdx > table[mid].min)
      {
         min = mid+1;
      }
      else
      {
         max = mid-1;
      }

   }while( min <= max );

   RETVALUE(ROK);
} 


/***********************************************************
 *
 *     Func : rgSCHCfgUtlFetchAcqiBitSz
 *
 *
 *     Desc : Fetch the CQI/PMI bits for a UE based on the mode and store them 
 *                for decoding. 
 *
 *     Ret  : Void
 *            ROK - RETVOID
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgUtlFetchAcqiBitSz
(
 RgSchUeCb         *ueCb, 
 U8                       numTxAnt,
 U8*                    cqiPmiSzR1,
 U8*                    cqiPmiSzRn1
 )
#else
PRIVATE  Void rgSCHCfgUtlFetchAcqiBitSz(ueCb, numTxAnt, cqiPmiSzR1, cqiPmiSzRn1)
   RgSchUeCb         *ueCb;
   U8                       numTxAnt;
   U8*                    cqiPmiSzR1;
   U8*                    cqiPmiSzRn1;
#endif
{
   U8 confRepMode;
   RgSchCqiRawPuschMode12   *mode12Info;   
   RgSchCqiRawPuschMode20   *mode20Info;    
   RgSchCqiRawPuschMode22   *mode22Info;    
   RgSchCqiRawPuschMode30   *mode30Info;   
   RgSchCqiRawPuschMode31   *mode31Info;    


   TRC3(rgSCHCfgUtlFetchAcqiBitSz);
   confRepMode = ueCb->dl.ueDlCqiCfg.aprdCqiCfg.aprdModeEnum;
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].type = TFU_RECP_REQ_PUSCH;
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.puschRawCqiBitWidth.mode = (TfuDlCqiPuschMode)confRepMode;

   switch(confRepMode)
   {
      case RGR_APRD_CQI_MOD12:
         {
            mode12Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.puschRawCqiBitWidth.u.mode12Info;
            mode12Info->wideBCqiCw0 = 4;
            mode12Info->r1WideBCqiCw1 = 0;
            mode12Info->rg1WideBCqiCw1 = 4;
            if(numTxAnt == 2)
            {
               mode12Info->r1TotalPmiBitLen = 2*ueCb->acqiCb.N;
               mode12Info->rg1TotalPmiBitLen = ueCb->acqiCb.N;
               *cqiPmiSzR1 = 4 + 2*ueCb->acqiCb.N;
               *cqiPmiSzRn1 = 8+ ueCb->acqiCb.N;
            }
            else if(numTxAnt == 4)
            {
               mode12Info->r1TotalPmiBitLen = 4*ueCb->acqiCb.N;
               mode12Info->rg1TotalPmiBitLen = 4*ueCb->acqiCb.N;
               *cqiPmiSzR1 = 4 + 4*ueCb->acqiCb.N;
               *cqiPmiSzRn1 = 8 + 4*ueCb->acqiCb.N;
            }
         }
         break;

      case RGR_APRD_CQI_MOD20:
         {
            mode20Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.puschRawCqiBitWidth.u.mode20Info;
            mode20Info->wideBCqiCw = 4;
            mode20Info->subBandDiffCqi = 2;
            mode20Info->posOfM = ueCb->acqiCb.L;
            *cqiPmiSzR1 = 6 + ueCb->acqiCb.L;
            *cqiPmiSzRn1 = 0;
         }
         break;

      case RGR_APRD_CQI_MOD22:
         {
            mode22Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.puschRawCqiBitWidth.u.mode22Info;
            mode22Info->wideBCqiCw0 = 4;
            mode22Info->sBDiffCqiCw0 = 2;
            mode22Info->r1WideBCqiCw1 = 0;
            mode22Info->r1SbDiffCqiCw1 = 0;
            mode22Info->rg1WideBCqiCw1 = 4;
            mode22Info->rg1SbDiffCqiCw1 = 2;
            mode22Info->posOfM = ueCb->acqiCb.L;
            if(numTxAnt == 2)
            {
               mode22Info->r1PmiBitLen = 4;
               mode22Info->rg1PmiBitLen = 2;
               *cqiPmiSzR1 = 10 + ueCb->acqiCb.L;
               *cqiPmiSzRn1 = 14 + ueCb->acqiCb.L;
            }
            else if(numTxAnt == 4)
            {
               mode22Info->r1PmiBitLen = 8;
               mode22Info->rg1PmiBitLen = 8;
               *cqiPmiSzR1 = 14 + ueCb->acqiCb.L;
               *cqiPmiSzRn1 = 20 + ueCb->acqiCb.L;
            }
         }
         break;

      case RGR_APRD_CQI_MOD30:
         {
            mode30Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.puschRawCqiBitWidth.u.mode30Info;
            mode30Info->wideBCqiCw = 4;
            mode30Info->totLenSbDiffCqi = 2*ueCb->acqiCb.N;
            *cqiPmiSzR1 = 4 + 2*ueCb->acqiCb.N;
            *cqiPmiSzRn1 = 0;
         }
         break;

      case RGR_APRD_CQI_MOD31:
         {
            mode31Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.puschRawCqiBitWidth.u.mode31Info;
            mode31Info->wideBCqiCw0 = 4;
            mode31Info->totLenSbDiffCqiCw0 = 2*ueCb->acqiCb.N;
            mode31Info->r1WideBCqiCw1 = 0;
            mode31Info->r1TotLenSbDiffCqiCw1 =0;
            mode31Info->rg1WideBCqiCw1 = 4;
            mode31Info->rg1TotLenSbDiffCqiCw1 = 2*ueCb->acqiCb.N;
            if(numTxAnt == 2)
            {
               mode31Info->r1PmiBitLen = 2;
               mode31Info->rg1PmiBitLen = 1;
               *cqiPmiSzR1 = 6 + 2*ueCb->acqiCb.N;
               *cqiPmiSzRn1 = 9 + 4*ueCb->acqiCb.N;
            }
            else if(numTxAnt == 4)
            {
               mode31Info->r1PmiBitLen = 4;
               mode31Info->rg1PmiBitLen = 4;
               *cqiPmiSzR1 = 8 + 2*ueCb->acqiCb.N;
               *cqiPmiSzRn1 = 12 + 4*ueCb->acqiCb.N;
            }
         }
         break;
      default:
         break;
   }
   RETVOID;
}
/* rg002.301 - ADD - Added the function rgSCHCfgUtlGetPcqiCrsSrRiTbl to be used
 * instead of the MACRO RG_SCH_GET_PERIODICITY_TBL */
/***********************************************************
 *
 *     Func : rgSCHCfgUtlGetPcqiCrsSrRiTbl
 *
 *
 *     Desc :  Get the Srs Cqi Crs Ri Table 
 *
 *     Ret  : Void
 *            ROK - RETVOID
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/

#ifdef ANSI
PRIVATE CONSTANT RgSchUePCqiSrsSrCfgIdxTbl *  rgSCHCfgUtlGetPcqiSrsSrRiTbl
(
   RgSchPerTbl tblType, 
   U8* min, 
   U8* max
)
#else
PRIVATE CONSTANT RgSchUePCqiSrxSrCfgIdxTbl *  rgSCHCfgUtlGetPcqiSrsSrRiTbl(tblType, min, max)
   RgSchPerTbl tblType; 
   U8* min; 
   U8* max;
#endif
{
    CONSTANT RgSchUePCqiSrsSrCfgIdxTbl * table;
    TRC3(rgSCHCfgUtlGetPcqiCrsSrRiTbl);
    
    switch (tblType)
    {

        case RG_SCH_FDD_PCQI_TBL:
        {
            table = rgSchUePCqiCfgIdxFddTbl;
            * min = 0;
            * max=RG_SCH_CQIPMI_CFGIDX_MAX_FDD;
            break;
        }
        case RG_SCH_TDD_PCQI_TBL:
        {
            table = rgSchUeCqiPmiCfgIdxTddTbl;
            * min = 0;
            * max=RG_SCH_CQIPMI_CFGIDX_MAX_TDD;
            break;
        }
        case RG_SCH_RI_TBL:
        {
            table = rgSchUeRiCfgIdxTbl;
            * min = 0;
            * max=RG_SCH_RI_CFGIDX_MAX;
            break;
        }
        case RG_SCH_FDD_SRS_TBL:
        {
            table = rgSchUeSrsCfgIdxFddTbl;
            * min = 0;
            * max=RG_SCH_SRS_ISRS_INDX_MAX_FDD;
            break;
        }
        case RG_SCH_TDD_SRS_TBL:
        {
            table = rgSchUeSrsCfgIdxTddTbl;
            * min = 0;
            * max=RG_SCH_SRS_ISRS_INDX_MAX_TDD;
            break;
        }
        case RG_SCH_SR_TBL:
        {
            table = rgSchUeSrCfgIdxTbl;
            * min = 0;
            * max=RG_SCH_ISR_INDX_MAX;
            break;
        }
        default:
        {
            table = (CONSTANT RgSchUePCqiSrsSrCfgIdxTbl *) 0;
            * min = 0 ;
            * max = 0 ;
            break;
        }

    }
    RETVALUE ( table );
}
/* #endif */
#endif /* TFU_UPGRADE */

#ifdef LTEMAC_DRX
/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxUeCfg
 *
 *
 *     Desc : Validates UE DRX Timers Configuration recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtDrxUeCfg
(
RgSchCellCb       *cell,
RgrUeDrxCfg       *ueDrxCfg
)
#else
PRIVATE S16 rgSCHCfgVldtDrxUeCfg(cell, ueDrxCfg)
RgSchCellCb       *cell;
RgrUeDrxCfg       *ueDrxCfg;
#endif
{
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif

   TRC2(rgSCHCfgVldtDrxUeCfg)


   if (ueDrxCfg->isDrxEnabled == FALSE)
   {
      RETVALUE(ROK);
   }

#ifdef LTEMAC_R9
   if ( ueDrxCfg->cqiMask.pres )
   {
      if ( ueDrxCfg->cqiMask.val != RGR_DRX_SETUP )
      {
#ifdef ALIGN_64BIT
         RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid cqiMask configuration,\
                  (%d)",ueDrxCfg->cqiMask.val));
#else
         RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid cqiMask configuration,\
                  (%ld)",ueDrxCfg->cqiMask.val));
#endif
         RETVALUE(RFAILED);
      }
   }
#endif /*LTEMAC_R9*/

   if ( rgSCHCfgVldtDrxOnDurCfg(ueDrxCfg->drxOnDurTmr) != ROK )
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid onDurTimer configuration,\
               (%d)",ueDrxCfg->drxOnDurTmr));
      RETVALUE(RFAILED);
   }

   if ( rgSCHCfgVldtDrxInActvCfg(ueDrxCfg->drxInactvTmr) != ROK )
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid Inactivity configuration,\
               (%d)",ueDrxCfg->drxInactvTmr));
      RETVALUE(RFAILED);
   }

   if ( rgSCHCfgVldtDrxReTxCfg(ueDrxCfg->drxRetxTmr) != ROK )
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid DrxReTX configuration,\
               (%d)",ueDrxCfg->drxRetxTmr));
      RETVALUE(RFAILED);
   }

   if ( rgSCHCfgVldtDrxLngCycCfg(ueDrxCfg->drxLongCycleOffst) != ROK )
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid LongCycle configuration"));
      RETVALUE(RFAILED);
   }

   if ( ueDrxCfg->drxLongCycleOffst.longDrxCycle < ueDrxCfg->drxOnDurTmr ) 
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid combination of Long DRX Cycle " 
              " and onDuration timer values"));
      RETVALUE(RFAILED);
   }
  
   if( TRUE == ueDrxCfg->drxShortDrx.pres )
   { 
      if ( ueDrxCfg->drxShortDrx.shortDrxCycle < ueDrxCfg->drxOnDurTmr )
      {
         RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid combination of short DRX "
                 "Cycle and onDuration timer values"));
         RETVALUE(RFAILED);
      }

      if ( (ueDrxCfg->drxLongCycleOffst.longDrxCycle %
               ueDrxCfg->drxShortDrx.shortDrxCycle) != 0)
      {
         RGSCHDBGERR(inst,(rgSchPBuf(inst)," Long DRX cycle is not multiple of "
                 "short DRX cycle"));
         RETVALUE(RFAILED);
      }

      if ( rgSCHCfgVldtDrxShrtCycCfg(ueDrxCfg->drxShortDrx) != ROK )
      {
         RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid Short Cycle configuration"));
         RETVALUE(RFAILED);
      }
   }
   
   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxUeCfg*/
      
/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxOnDurCfg
 *
 *
 *     Desc : Validates UE DRX On Duration configuration 
 *            recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtDrxOnDurCfg
(
U8      onDurTmr 
)
#else
PRIVATE S16 rgSCHCfgVldtDrxOnDurCfg(onDurTmr)
U8      onDurTmr;
#endif
{
   TRC2(rgSCHCfgVldtDrxOnDurCfg)

   switch ( onDurTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_3PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_5PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_10PSF:
      case RGR_DRX_PRD_20PSF:
      case RGR_DRX_PRD_30PSF:
      case RGR_DRX_PRD_40PSF:
      case RGR_DRX_PRD_50PSF:
      case RGR_DRX_PRD_60PSF:
      case RGR_DRX_PRD_80PSF:
      case RGR_DRX_PRD_100PSF:
      case RGR_DRX_PRD_200PSF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }
   }/*switch(onDurTmr)*/

   RETVALUE(ROK);
}/*rgSCHCfgVldtOnDurCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxInActvCfg
 *
 *
 *     Desc : Validates UE DRX InActivity configuration 
 *            recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtDrxInActvCfg
(
U16      inActvTmr
)
#else
PRIVATE S16 rgSCHCfgVldtDrxInActvCfg(inActvTmr)
U16      inActvTmr;
#endif
{
   TRC2(rgSCHCfgVldtDrxInActvCfg)

   switch ( inActvTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_3PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_5PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_10PSF:
      case RGR_DRX_PRD_20PSF:
      case RGR_DRX_PRD_30PSF:
      case RGR_DRX_PRD_40PSF:
      case RGR_DRX_PRD_50PSF:
      case RGR_DRX_PRD_60PSF:
      case RGR_DRX_PRD_80PSF:
      case RGR_DRX_PRD_100PSF:
      case RGR_DRX_PRD_200PSF:
      case RGR_DRX_PRD_300PSF:
      case RGR_DRX_PRD_500PSF:
      case RGR_DRX_PRD_750PSF:
      case RGR_DRX_PRD_1280PSF:
      case RGR_DRX_PRD_1920PSF:
      case RGR_DRX_PRD_2560PSF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }
   }/*switch(InactvTmr)*/

   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxInActvCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxReTxCfg
 *
 *
 *     Desc : Validates DRX ReTx timer  configuration 
 *            recieved from RRC.
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
PRIVATE S16 rgSCHCfgVldtDrxReTxCfg
(
U8       reTxTmr
)
#else
PRIVATE S16 rgSCHCfgVldtDrxReTxCfg(reTxTmr)
U8       reTxTmr;
#endif
{
   TRC2(rgSCHCfgVldtDrxReTxCfg)

   switch ( reTxTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_16PSF:
      case RGR_DRX_PRD_24PSF:
      case RGR_DRX_PRD_33PSF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }
   }/*switch(drxRetxTmr)*/

   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxReTxCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxShrtCycCfg
 *
 *
 *     Desc : Validates DRX Short Cycle timer  configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxLngCycCfg
(
RgrDrxLongCycleOffst       lngCycleOffst
)
#else
PRIVATE S16 rgSCHCfgVldtDrxLngCycCfg(lngCycleOffst)
RgrDrxLongCycleOffst       lngCycleOffst;
#endif
{
   TRC2(rgSCHCfgVldtDrxLngCycCfg)

   if ( rgSCHCfgVldtDrxLngCyclTmrs(lngCycleOffst.longDrxCycle) != ROK )
   {
      RETVALUE(RFAILED);
   }

   if ( lngCycleOffst.drxStartOffst >= lngCycleOffst.longDrxCycle ) 
   {
      RETVALUE(RFAILED);  
   }

   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxLngCycCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxLngCyclTmrs
 *
 *
 *     Desc : Validates DRX Long Cycle timer values 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxLngCyclTmrs
(
U16      val 
)
#else
PRIVATE S16 rgSCHCfgVldtDrxLngCyclTmrs(val)
U16      val;
#endif
{
   TRC2(rgSCHCfgVldtDrxLngCyclTmrs)

   switch ( val )
   {
      case RGR_DRX_PRD_10SF:
      case RGR_DRX_PRD_20SF:
      case RGR_DRX_PRD_32SF:
      case RGR_DRX_PRD_40SF:
      case RGR_DRX_PRD_64SF:
      case RGR_DRX_PRD_80SF:
      case RGR_DRX_PRD_128SF:
      case RGR_DRX_PRD_160SF:
      case RGR_DRX_PRD_256SF:
      case RGR_DRX_PRD_320SF:
      case RGR_DRX_PRD_512SF:
      case RGR_DRX_PRD_640SF:
      case RGR_DRX_PRD_1024SF:
      case RGR_DRX_PRD_1280SF:
      case RGR_DRX_PRD_2048SF:
      case RGR_DRX_PRD_2560SF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }
   }/*switch(longDrxCycle)*/

   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxLngCyclTmrs*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxShrtCycCfg
 *
 *
 *     Desc : Validates DRX Short Cycle timer  configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxShrtCycCfg
(
RgrDrxShortDrx      shrtCycCfg 
)
#else
PRIVATE S16 rgSCHCfgVldtDrxShrtCycCfg(shrtCycCfg)
RgrDrxShortDrx      shrtCycCfg;
#endif
{
   TRC2(rgSCHCfgVldtDrxShrtCycCfg)

   switch(shrtCycCfg.shortDrxCycle)
   {
      case RGR_DRX_PRD_2SF:
      case RGR_DRX_PRD_5SF:
      case RGR_DRX_PRD_8SF:
      case RGR_DRX_PRD_10SF:
      case RGR_DRX_PRD_16SF:
      case RGR_DRX_PRD_20SF:
      case RGR_DRX_PRD_32SF:
      case RGR_DRX_PRD_40SF:
      case RGR_DRX_PRD_64SF:
      case RGR_DRX_PRD_80SF:
      case RGR_DRX_PRD_128SF:
      case RGR_DRX_PRD_160SF:
      case RGR_DRX_PRD_256SF:
      case RGR_DRX_PRD_320SF:
      case RGR_DRX_PRD_640SF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }

   }/*switch(shortDrxCycle)*/

   if ( (shrtCycCfg.drxShortCycleTmr < RGR_DRX_SHRTCYCLE_MIN) ||
         (shrtCycCfg.drxShortCycleTmr > RGR_DRX_SHRTCYCLE_MAX)
      )
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
#endif /*LTEMAC_DRX*/
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
/***********************************************************
 *
 *     Func : rgSCHCfgUeCqiReptReCfg
 *
 *
 *     Desc : Reconfiguration of PUSH N CQI Reporting 
 *
 *     Ret  : RFAILED in case of failure
 *            ROK if success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgUeCqiReptReCfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg
)
#else
PRIVATE  S16 rgSCHCfgUeCqiReptReCfg(cell, ue, ueRecfg)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrUeRecfg     *ueRecfg;
#endif
{
   S16 retVal;
   TRC3(rgSCHCfgUeCqiReptReCfg)

   /* Check has it been disabled */
   if(ueRecfg->ueCqiReptCfg.numColltdCqiRept)
   {
    /* Check if we need to send CQI reports collagted so far and send if so */
      if(ue->schCqiInfo.cqiCount >=
              ueRecfg->ueCqiReptCfg.numColltdCqiRept)
      {
         RgrStaIndInfo *staInfo;
      /* if yes, Send StaInd to RRM */
         retVal = rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&staInfo,
                  sizeof(RgrStaIndInfo));
         if(retVal != ROK)
         {
            RGSCHDBGERR (cell->instIdx,(rgSchPBuf(cell->instIdx),\
               "Could not allocate memory for sending StaInd\n"));
            RETVALUE(retVal);
         }

      /* Fill StaInd for sending collated N CQI rpeorts */
         rgSCHUtlFillSndStaInd(cell, ue, staInfo,
               ueRecfg->ueCqiReptCfg.numColltdCqiRept);
      }
   }
   else
   {
      ue->schCqiInfo.cqiCount = 0;
   }

   ue->cqiReptCfgInfo.numColltdCqiRept =
      ueRecfg->ueCqiReptCfg.numColltdCqiRept;
   RETVALUE(ROK);
} /* End of rgSCHCfgUeCqiReptReCfg */
#endif /* End of RGR_CQI_REPT */
/**********************************************************************
 
         End of file:     gk_sch_cfg.c@@/main/2 - Sat Jul 30 02:21:35 2011
 
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
/main/1      ---       sd   1. LTE MAC 2.1 release
             rg001.201 sm   1. Added TTI indication towards 
                                        RGR user.
             rg002.201 sm   1. Added 0 as a valid value for number of TICKs
                               RRM.0 implies no ticks i.e. shutting off the 
                               feature.
             rg003.201 sm   1. TDD flag added  
             rg004.201 sd   1. ccpu00105914: PHR handling for MSG3
                       pk   2. ccpu00109921: PBCH allocation      
                       rt   3. Added for SI Enhancement
             rg006.201 sd  1. ccpu00112398: Added periodic BSR timer
             rg007.201 ap  1. Added support for MIMO
             rg008.201 sd  1. Removed dependncy on MIMO compile-time flag
             rg009.201 rsharon  1.[ccpu00112372]Changes for CR timer.
             rg009.201 gj 1. [ccpu00116647] - Added siId validation for lower limit
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
           rg002.301 nudupi 1. [ccpu00119118 ] - Warning removal for cross 
                                   compilation
                            2. Added the function rgSCHCfgUtlGetPcqiSrsSrRiTbl
                               and removed the MACRO RG_SCH_GET_PERIODICITY_TBL
                               as a part of coding standard improvement.
                            3.ccpu00118351: seperate UL & DL deltas.
*********************************************************************91*/
