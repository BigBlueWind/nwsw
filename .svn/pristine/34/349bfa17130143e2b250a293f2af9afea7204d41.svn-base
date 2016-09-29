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
 
     Desc:     C source code for SPS functions
 
     File:     rg_sch_sps.c
 
     Sid:      gk_sch_sps.c@@/main/2 - Sat Jul 30 02:21:50 2011
 
     Prg:     sd
 
**********************************************************************/

/** @file rg_sch_sps.c
@brief This module handles the semi-persistent scheduling functionality in both
downlink and uplink
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_math.h"      /* common math library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "cm_math.h"       /* common MATH */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_math.x"       /* common math library */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "cm_math.x"       /* common MATH */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"

#ifdef LTEMAC_SPS
/* Private functions */
typedef enum rgSchCmnSpsPrdEnum
{
   RG_SCH_CMN_SPS_PRD_2SF,
   RG_SCH_CMN_SPS_PRD_5SF,
   RG_SCH_CMN_SPS_PRD_10SF,
   RG_SCH_CMN_SPS_PRD_20SF,
   RG_SCH_CMN_SPS_PRD_30SF,
   RG_SCH_CMN_SPS_PRD_32SF,
   RG_SCH_CMN_SPS_PRD_40SF,
   RG_SCH_CMN_SPS_PRD_60SF,
   RG_SCH_CMN_SPS_PRD_64SF,
   RG_SCH_CMN_SPS_PRD_80SF,
   RG_SCH_CMN_SPS_PRD_120SF,
   RG_SCH_CMN_SPS_PRD_128SF,
   RG_SCH_CMN_SPS_PRD_160SF,
   RG_SCH_CMN_SPS_PRD_256SF,
   RG_SCH_CMN_SPS_PRD_320SF,
   RG_SCH_CMN_SPS_PRD_512SF,
   RG_SCH_CMN_SPS_PRD_640SF,
   RG_SCH_CMN_SPS_PRD_1024SF,
   RG_SCH_CMN_SPS_PRD_1280SF,
   RG_SCH_CMN_SPS_PRD_2048SF,
   RG_SCH_CMN_SPS_PRD_2560SF,
   RG_SCH_CMN_SPS_PRD_INVALID
} RgSchCmnSpsPrdEnum; 

/* DCI Format options for each Transmission Mode and selected RA TYPE 
 * for UE specific search-space using SPS-RNTI only */

TfuDciFormat rgSchCmnSpsRaTypeToDciFrmt[7][3] = {
   {TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_2A,TFU_DCI_FORMAT_2A, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_2, TFU_DCI_FORMAT_2, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_INVALID,TFU_DCI_FORMAT_INVALID, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_INVALID,TFU_DCI_FORMAT_INVALID, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1A}
};

/* LCM table for the possible combination of periodicities */
PRIVATE U16
 rgSchCmnSpsLcmTbl[RG_SCH_CMN_SPS_MAX_NUM_PRD][RG_SCH_CMN_SPS_MAX_NUM_PRD];

PRIVATE U16 rgSchCmnSpsPrdVal[RG_SCH_CMN_SPS_MAX_NUM_PRD] 
= {2, 5, 10, 20, 30, 32, 40, 60, 64, 80, 
   120, 128, 160, 256, 320, 512, 640, 1024, 1280, 2048, 2560};

#ifdef LTE_TDD
/* Harq interval values for various TDD Configurations for SPS_TDD */
/* Array initialization to 0 */
PRIVATE U8 rgSchHqIntTbl[RGSCH_MAX_TDD_UL_DL_CFG] =
 {0};

#endif /* LTE_TDD */
/* DL SPS private functions */

/* DL SPS Configuration/initialization functions */

PRIVATE Void rgSCHCmnSpsCompLcmTbl ARGS((Void));
PRIVATE U16 rgSCHCmnSpsCompLcm ARGS((
 U16      prd1,
 U16      prd2
));
PRIVATE S16 rgSCHCmnSpsVldtDlCellCfg ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
));
PRIVATE S16 rgSCHCmnSpsDlCellCfg ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
));
PRIVATE S16 rgSCHCmnSpsDlSfInit ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
));
PRIVATE Void rgSCHCmnSpsDlN1PucchInit ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
));
PRIVATE S16 rgSCHCmnSpsVldtDlUeCfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg
));
PRIVATE S16 rgSCHCmnSpsDlUeCfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
));
PRIVATE S16 rgSCHCmnSpsVldtDlUeRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg
));
PRIVATE S16 rgSCHCmnSpsDlUeRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
));
PUBLIC Void rgSCHCmnSpsDlCellDel ARGS((
RgSchCellCb        *cell
));
PRIVATE Void rgSCHCmnSpsDlUeSuspend ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue
));
PRIVATE Void rgSCHCmnSpsDlCellInit ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
));
PRIVATE S16 rgSCHCmnSpsDlUeInit ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeSpsCfg        *ueSpsCfg,
RgrUeMeasGapCfg    *ueMeasGapCfg
));


/* DL SPS Scheduling functions */

PRIVATE RgSchCmnSpsDlN1Pucch *rgSCHCmnSpsDlGetN1PucchCb ARGS((
RgSchCmnSpsDlN1PucchDb  *n1PucchDb,
U16                     n1PucchVal
));
PRIVATE S16 rgSCHCmnSpsGetNextSpsOcc ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
CmLteTimingInfo  crntOcc,
CmLteTimingInfo  *nxtOcc
));
PRIVATE Void rgSCHCmnDlSpsRstMaxSpsBw ARGS((
U8             rbgSize,
U8             dlTotalBw,
U8             *spsBw
));
PRIVATE Void rgSCHCmnSpsDlProcRetxUes ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       frmTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnSpsDlProcToBeSchdSvcs ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE S16 rgSCHCmnSpsDlUpdSf ARGS((
RgSchCellCb        *cell,
U32                prdVal
));
PRIVATE U8 rgSCHCmnSpsDlGetScaledCqi ARGS((
RgSchUeCb             *ue,
U8                    curCqi
));
PRIVATE S16 rgSCHCmnSpsDlUeClearAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
PRIVATE Void rgSCHCmnSpsDlProcActvtdUes ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnSpsDlProcToBeRelUes ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf
));
PRIVATE S16 rgSCHCmnSpsDlProcToBeActvtdUes ARGS((
RgSchCellCb            *cell,
CmLteTimingInfo        crntTime,
RgSchDlSf              *dlSf
));
PRIVATE Bool rgSCHCmnSpsDlChkUeInActv ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        timingInfo
));
PRIVATE S16 rgSCHCmnSpsDlAllocForSvc ARGS((
RgSchCellCb            *cell,
CmLteTimingInfo        schdTime,
RgSchDlSf              *subFrm,
RgSchDlLcCb            *svc,
U32                    bo,
RgSchCmnDlRbAllocInfo  *allocInfo
));
PRIVATE S16 rgSCHCmnSpsDlProcToBeReactvtdUe ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        schdTime,
RgSchDlSf              *dlSf
));
PRIVATE S16 rgSCHCmnSpsDlUeAllocFnlz ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
RgSchDlSf           *dlSf,
U8                  n1PucchIdx,
U8                  isActvOrReactv
));
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInTime ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        timingInfo
));
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSfAllocInfo  *allocInfo,
U8                  *n1PucchIdx
));
PRIVATE S16 rgSCHCmnSpsDlUeAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSf           *dlSf,
RgSchDlSfAllocInfo  *allocInfo
));
PRIVATE Bool rgSCHCmnSpsDlChkMeasGapCollision ARGS((
U8                  spsPrdIdx,
U16                 startIdx,
U16                 measGapPrd,
U16                 spsPrd,
U32                  *measGapMask
));
PRIVATE S16 rgSCHCmnSpsDlUeAllocRbs ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSf           *dlSf,
RgSchDlSfAllocInfo  *allocInfo
));
PRIVATE Void rgSCHCmnSpsDlGetDciFmt ARGS((
RgSchUeCb           *ue,
U8                  raType,
TfuDciFormat        *dciFmt
));
PRIVATE S16 rgSCHCmnSpsDlUpdSfOnAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
RgSchDlSf           *dlSf,
U8                  n1PucchIdx
));
PRIVATE S16 rgSCHCmnSpsDlUpdUeOnAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
U8                  n1PucchIdx
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcch ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchPdcch          *pdcch,
Bool                isSpsOcc,
U8                  hqPId
));
PRIVATE Void rgSCHCmnSpsDlTxAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnSpsDlRetxAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));

#ifdef RG_UNUSED
PRIVATE Bool rgSCHCmnSpsDlChkCollision ARGS((
U8                    spsPrdIdx,
U16                   startIdx,
U16                   prd,
U16                   spsPrd,
U16                   offset
));
#endif /* RG_UNUSED */

PRIVATE Void rgSCHCmnSpsDlFillRelPdcch ARGS((
RgSchUeCb       *ue,
RgSchPdcch      *relPdcch
));
PRIVATE Void rgSCHCmnSpsDlNewTxUeFnlz ARGS((
RgSchCellCb    *cell,
RgSchDlRbAlloc *ueAllocInfo, 
RgSchUeCb      *ue
));
PRIVATE Void rgSCHCmnSpsDlFreeN1Pucch ARGS((
RgSchCmnSpsDlN1PucchDb  *n1PucchDb,
RgSchCmnSpsDlN1Pucch    *n1PucchCb
));
PRIVATE S16 rgSCHCmnSpsDlInitMeasGapMask ARGS((
RgSchCellCb          *cell,
RgrUeMeasGapCfg      *measGapCfg,
RgSchCmnDlUeSpsInfo  *dlSpsUe
));
PRIVATE Void rgSCHCmnSpsDlUpdDlSfAllocWithSps ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf
));
PRIVATE Void rgSCHCmnSpsDlUpdSpsWithDlSfAlloc ARGS((
RgSchDlSf             *dlSf,
RgSchDlSfAllocInfo    *allocInfo
));

/* DL SPS Utility functions */

PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ActvUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue,
CmLteTimingInfo         actvTime
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmActvUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue,
CmLteTimingInfo         actvTime
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeRelUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeRelUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxHqProcs ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlHqProcCb         *hqP
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmRetxHqProcs ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlHqProcCb         *hqP
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlLcCb             *dlSvc
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlLcCb             *dlSvc
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2TxSpsUeLst ARGS((
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchDlHqProcCb      *hqP
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst ARGS((
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchDlHqProcCb      *hqP
));


/* UL SPS functions */

PRIVATE S16 rgSCHCmnSpsUlVldtCellCfg ARGS((
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg
));

PRIVATE S16 rgSCHCmnSpsUlCellCfg ARGS((
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg
));

PRIVATE S16 rgSCHCmnSpsUlCellDel ARGS((
RgSchCellCb    *cell
));

PRIVATE S16 rgSCHCmnSpsUlVldtUeCfg ARGS((
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
));

PRIVATE S16 rgSCHCmnSpsUlUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg
));

PRIVATE S16 rgSCHCmnSpsUlVldtUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
));

PRIVATE S16 rgSCHCmnSpsUlUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
));

PRIVATE Void rgSCHCmnSpsUlUeDel ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));

PRIVATE Void rgSCHCmnSpsUlUeMeasGapMaskInit ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeMeasGapCfg    ueMesGapCfg
));

PRIVATE Void rgSCHCmnSpsUlUeInit ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgrUeSpsCfg     spsCfg,
RgrUeMeasGapCfg measGapCfg
));

PRIVATE S16 rgSCHCmnSpsUlBwMaskInit ARGS((
RgSchCellCb     *cell,
U16             lstSz
));

PRIVATE Void rgSCHCmnSpsUlInitSpsSf ARGS((
RgSchCellCb     *cell
));

PRIVATE S16 rgSCHCmnSpsUlUpdSpsSf ARGS((
RgSchCellCb     *cell,
U16             crntLcm,
U16             nextLcm
));

PRIVATE U8 rgSCHCmnSpsScaleUlCqi ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

PRIVATE U8 rgSCHCmnSpsUlCalcSbReqd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
U8                   cqi,
U8                   *iMcs,
U8                   *iTbs
));

PRIVATE RgSchCmnSpsUlSf* rgSCHCmnSpsGetNextNthSpsUlSf ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo ulSpsActvOcc,
U8              prsntidx
));

PRIVATE U8 rgSCHCmnSpsUlGetITbs ARGS((
RgSchUeCb        *ue,
Bool             isEcp
));

PRIVATE RgSchUlAlloc* rgSCHCmnSpsUlSbAlloc ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *sf,
U8                    numSb,
CmLteTimingInfo       timeToTx
));

PRIVATE S16 rgSCHCmnSpsUlSbAllocForUe ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *sf,
CmLteTimingInfo       timeToTx
));

PRIVATE S16 rgSCHCmnUlSpsClrAlloc ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
RgSchUlAlloc            *alloc
));

PRIVATE S16 rgSCHCmnUlSpsMarkAlloc ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
CmLteTimingInfo         timeToTx
));

PRIVATE S16 rgSCHCmnSpsUlUeAlloc ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
RgSchCmnUlRbAllocInfo   *alloc,
RgSchUlAlloc            *ueSpsAlloc,
CmLteTimingInfo         timeToTx
));

PRIVATE Bool rgSCHCmnSpsUlChkMesGapColl ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
CmLteTimingInfo         timeToTx,
U8                      gapPrd
));

PRIVATE S16 rgSCHCmnSpsUlFetchAllocInUlSf ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *ulSf
));

PRIVATE S16 rgSCHCmnSpsUlProcActvUeInCrntSf ARGS((
RgSchCellCb             *cell
));

PRIVATE S16 rgSCHCmnSpsUlProcActvUeNxtOcc ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
));

PRIVATE S16 rgSCHCmnSpsUlMarkActvUeAllocs ARGS((
RgSchCellCb             *cell
));

PRIVATE S16 rgSCHCmnSpsUlProcActvtdUes ARGS((
RgSchCellCb             *cell,
Bool                    ulSched,
RgSchCmnUlRbAllocInfo   *allocInfo
));

PRIVATE S16 rgSCHCmnSpsUlProcToBeRelUe ARGS((
RgSchCellCb             *cell
));

PRIVATE S16 rgSCHCmnSpsUlProcToBeReactvtdUe ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnUlRbAllocInfo   *alloc
));

PRIVATE S16 rgSCHCmnSpsUlProcToBeActvtdUe ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc
));

PRIVATE Bool rgSCHCmnSpsUlChkUeInActv ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue
));

PRIVATE Void rgSCHCmnSpsUlUtlFreeSpsUe ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

PRIVATE RgSchUlAlloc *rgSCHCmnSpsUtlUlGetSpfcAlloc ARGS((
RgSchUlSf       *sf,
U8              startSb,
U8              numSb
));

PRIVATE S16 rgSCHCmnSpsUlUtlCpyAlloc ARGS((
RgSchUlAlloc         *srcAlloc,
RgSchUlAlloc         *dstAlloc 
));

PRIVATE S16 rgSCHCmnSpsUlUtlFillRelPdcch ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchPdcch              *pdcch
));

PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeRelLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue
));

PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeActvLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue
));

PRIVATE S16 rgSCHCmnSpsUlInsUeInActvLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       spsOcc
));

PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeActvLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue
));

PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeRelLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue
));

PRIVATE S16 rgSCHCmnSpsUlDelUeFrmActvLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       spsOcc
));

PRIVATE S16 rgSCHCmnSpsUlFreeToBeActvUeLst ARGS((
RgSchCellCb           *cell
));

PRIVATE S16 rgSCHCmnSpsUlFreeToBeRelUeLst ARGS((
RgSchCellCb           *cell
));

PRIVATE S16 rgSCHCmnSpsUlFreeActvUeLst ARGS((
RgSchCellCb           *cell
));

PRIVATE Void rgSCHCmnSpsUlFillPdcchWithAlloc ARGS((
RgSchPdcch      *pdcch,
RgSchUlAlloc    *alloc,
RgSchUeCb       *ue
));
PRIVATE Void rgSCHCmnSpsUlUeFillAllocInfo ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2A ARGS((
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1 ARGS((
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2 ARGS((
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1A ARGS((
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
));
/**
 * @brief Initializes the SPS related global data structures
 *
 * @details
 *     Function : rgSCHCmnSpsInit
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this API at initialization.
 *       
 *     Processing Steps:
 *     - Compute LCM table for SPS.
 *     
 *  @param[in] Void
 *
 *  @return Void 
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsInit
(
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsInit () 
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsInit);
   /* Compute the LCM table for SPS operation */
   rgSCHCmnSpsCompLcmTbl();

   RETVOID;
}

/* 
 * SPS specific functions 
 */

/**
 * @brief Initializes the SPS scheduler for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsCellCfg
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this API at cell configuration.
 *       
 *     Processing Steps:
 *     - Invoke rgSCHCmnSpsDlCellCfg for DL SPS
 *     - Invoke rgSCHCmnSpsUlCellCfg for UL SPS
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsCellCfg
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsCellCfg (cell, cellCfg, err) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
   S16             ret       = ROK;
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif /* DEBUGP */
   
   TRC2(rgSCHCmnSpsCellCfg);    
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsCellCfg:DL BW: %d\n", (int)cellCfg->bwCfg.dlTotalBw));

   /* Validate DL SPS configuration for the cell */
   if ((ret = rgSCHCmnSpsVldtDlCellCfg(cell, cellCfg)) != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "rgSCHCmnSpsDlCellCfg: Validation failed for cell: %d\n",
               (int)cell->cellId));
      RETVALUE(RFAILED);
   }

   /* Validate UL SPS configuration for the cell */
   if ((ret = rgSCHCmnSpsUlVldtCellCfg(cell, cellCfg)) != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "rgSCHCmnSpsUlVldtCellCfg: Validation failed for cell: %d\n",
               (int)cell->cellId));
      RETVALUE(RFAILED);
   }
   
   /* Initialize DL SPS specific information for the cell */
   if ((ret = rgSCHCmnSpsDlCellCfg(cell, cellCfg, err)) != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
             "rgSCHCmnSpsDlCellCfg failed for cell: %d\n", (int)cell->cellId));
      RETVALUE(RFAILED);
   }

   /* Initialize UL SPS specific information for the cell */
   if ((ret = rgSCHCmnSpsUlCellCfg(cell, cellCfg)) != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
         "rgSCHCmnSpsUlCellCfg failed for cell: %d\n", (int)cell->cellId));
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}

/**
 * @brief Delete the SPS context for the cell at cell delete
 *
 * @details
 *
 *     Function : rgSCHCmnSpsCellDel
 *     
 *     Invoking Module Processing:
 *      - SCH shall invoke this at cell delete.
 *  
 *
 *     Processing Steps:
 *      - Invoke rgSCHCmnSpsDlCellDel
 *      - Invoke rgSCHCmnSpsUlCellDel
 *      - Return Void
 *
 *  
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsCellDel 
(
RgSchCellCb        *cell
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsCellDel (cell) 
RgSchCellCb        *cell;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif /* DEBUGP */
   TRC2(rgSCHCmnSpsCellDel);
       
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsCellDel: cellId: %d\n", cell->cellId));

   /* Delete DL SPS specific information */
   rgSCHCmnSpsDlCellDel(cell);
   
   /* Delete UL SPS specific information */
   rgSCHCmnSpsUlCellDel(cell);

   RETVOID;
}

/**
 * @brief Initializes UE with SPS specific information.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUeCfg
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this if DL SPS is enabled for the UE
 *        at UE configuration.
 *      - SCH CMN shall validate the range for the configured values
 *      - SCH CMN shall invoke this after the configuration for measurement
 *      gap, DRX, SRS, CQI and SR periodicity and offset has been updated in 
 *      the UeCb.
 *        
 *
 *     Processing Steps:
 *      - If DL SPS is enabled,
 *        - Validate the configuration with respect to SPS.
 *          Call rgSCHCmnSpsVldtDlUeCfg().
 *        - If validation fails
 *           - Return RFAILED.
 *        - else
 *           - Invoke rgSCHCmnSpsDlUeCfg
 *      - If (ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled == TRUE)
 *        - Call rgSCHCmnSpsUlVldtUeCfg()
 *        - if validation fails
 *           - Return RFAILED
 *        - else
 *           - Invoke rgSCHCmnSpsUlUeCfg
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeCfg      *ueCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUeCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUeCfg (cell, ue, ueCfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
   S16             ret       = ROK;
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif 
   TRC2(rgSCHCmnSpsUeCfg);
       
   /* Note: Assumed that the ue is memset to 0 before call to this function */
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsUeCfg: cellId: %d, crnti %d DL SPS enabled: %d "
            "UL SPS enabled: %d\n", cell->cellId, ueCfg->crnti,
            ueCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled, 
            ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled));
   /* ccpu00123905 Updation of ySps for SPS-C-RNTI*/
   rgSCHUtlUpdY( ueCfg->ueSpsCfg.spsRnti, ue->ySps);
   /* Perform validations for DL SPS if DL SPS is enabled */
   if (ueCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled)
   {
      if ((ret = rgSCHCmnSpsVldtDlUeCfg(cell, ue, ueCfg)) != ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "Validation failed for DL SPS UE %d\n", 
                  (int)ueCfg->crnti));
         RETVALUE(RFAILED);
      }
      /* Perform initialization for DL SPS enabled UE */
      rgSCHCmnSpsDlUeCfg(cell, ue, ueCfg, err);
   }

   /* Perform validations for UL SPS if UL SPS is enabled */
   if (ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled)
   {
      if ((ret = rgSCHCmnSpsUlVldtUeCfg(cell, ueCfg)) != ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "Validation failed for UL SPS UE config %d\n", 
                  (int)ueCfg->crnti));
         RETVALUE(RFAILED);
      }
      /* Perform initialization for UL SPS enabled UE */
      if ((ret = rgSCHCmnSpsUlUeCfg(cell, ue, ueCfg)) != ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "Initialization failed for UL SPS UE config %d\n", 
                  (int)ueCfg->crnti));
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief Re-initializes SPS context for the UE at UE reconfiguration.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUeRecfg
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this at UE reconfiguration.
 *         
 *     Processing Steps:
 *      - If DL SPS has been reconfigured, call rgSCHCmnSpsDlUeRecfg.
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
PUBLIC S16 rgSCHCmnSpsUeRecfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUeRecfg (cell, ue, ueRecfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif /* DEBUGP */
   S16             ret;
   
   TRC2(rgSCHCmnSpsUeRecfg);
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsUeRecfg: cellId: %d, ueId %d\n", cell->cellId,
            ue->ueId));
   /* ccpu00123905: Check for SPS-C-RNTI change. If it is changed update Ysps*/
   if(ue->spsRnti != ueRecfg->ueSpsRecfg.spsRnti)
   {
	   rgSCHUtlUpdY( ueRecfg->ueSpsRecfg.spsRnti, ue->ySps);
   }
   /* DL SPS reconfigured */
   ret = rgSCHCmnSpsDlUeRecfg(cell, ue, ueRecfg, err);
   if (ret != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "rgSCHCmnSpsDlUeRecfg failed: cellId: %d, ueId %d\n",
               cell->cellId, ue->ueId));
      RETVALUE(RFAILED);
   }

   /* UL SPS reconfigured */
   if ((rgSCHCmnSpsUlUeRecfg(cell, ue, ueRecfg)) != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "rgSCHCmnSpsUlUeRecfg failed: cellId: %d, ueId %d\n",
               cell->cellId, ue->ueId));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 * @brief Clears SPS context of the UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUeDel
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this at delete of SPS enabled UE.
 *
 *     Processing Steps:
 *     - Check the configured n1Pucch and if this is the last UE on the
 *     configured n1Pucch, free the corresponding index
 *     - Remove UE from the lists, if it exists
 *     - Free Ue specific information for SPS 
 *     - Return ROK
 *
 * @param[in]  RgSchCellCb *cell
 * @param[in]  RgSchUeCb   *ue
 * @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUeDel 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUeDel (cell, ue) 
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                instIdx = cell->instIdx;
#endif /*DEBUGP*/
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);

   TRC2(rgSCHCmnSpsUeDel);
       
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsUeDel: cellId: %d, ueId: %d\n", 
            cell->cellId, ue->ueId));

   /* Clear DL SPS specific information for a DL SPS enabled UE */
   if (ue->dl.dlSpsCfg.isDlSpsEnabled)
   {
      rgSCHCmnSpsDlUeSuspend(cell, ue);
      cmMemset((U8 *)dlSpsUe, 0, sizeof(*dlSpsUe));
   }

   /* Delete UL SPS specific information */
   if (ue->ul.ulSpsCfg.isUlSpsEnabled)
   {
      rgSCHCmnSpsUlUeDel(cell, ue);
   }

   RETVOID;
}

/**
 * @brief Downlink CQI Indication handler 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlCqiIndHndlr
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this if UE is DL SPS enabled upon recieving
 *      DL CQI indication from PHY.
 *        
 *     Processing Steps:
 *     -  If (!ue->isSpsActv), return ROK
 *     - else
 *       - if (reported_cqi != old_cqi) 
 *         - ue->newScaledCqi = RgSchCmnSpsGetDlCqi(reported_cqi)
 *         - Determine number of RBs needed at ue->newScaledCqi for the
 *         configured packet size.
 *         - If ue->isSpsActv
 *            - If new_rbs_needed != ue->spsAllocInfo->rbsAlloc, mark UE to be
 *              reactivated.
 *              - Set ue->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_FREQ
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  TfuDlCqiRpt     *cqiIndInfo
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlCqiIndHndlr
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuDlCqiRpt          *cqiIndInfo,
CmLteTimingInfo      timingInfo
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlCqiIndHndlr(cell, ue, cqiIndInfo, timingInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuDlCqiRpt          *cqiIndInfo;
CmLteTimingInfo      timingInfo;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                instIdx  = cell->instIdx;
#endif /*DEBUGP*/
   RgSchCmnDlUe        *cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U8                  scCqi;
   
   TRC2(rgSCHCmnSpsDlCqiIndHndlr);
   /* Update the values only if ue is SPS active */
   if (dlSpsUe->isSpsActv)
   {
      RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
               "rgSCHCmnSpsDlCqiIndHndlr: cellId: %d, ueId: %d\n",
               cell->cellId, ue->ueId));
      
      /* Note: Assumed CQI is updated before invoking this API */
      scCqi = rgSCHCmnSpsDlGetScaledCqi(ue, cmnUeDl->mimoInfo.cwInfo[0].cqi);

      if (scCqi == RG_SCH_CMN_SPS_DL_INVALIDCQI_VAL)
      {
         scCqi = cmnUeDl->mimoInfo.cwInfo[0].cqi;
      }
      
      /* scaledCqi should be updated value for SPS active UE */
      if (scCqi != dlSpsUe->dlSpsUeSchdInfo.scaledCqi)
      {
         U8 rb, mcs, itbs;
         U32 tbs;
         /* Compute number of RBs needed for new CQI */
         rgSCHCmnClcRbAlloc(cell, dlSpsUe->spsSvc->dlLcSpsCfg.packetSize,
               scCqi, &rb, &tbs, &mcs, &itbs, TRUE);
         if (rb != dlSpsUe->dlSpsUeSchdInfo.spsAllocInfo.rbsAlloc)
         {
            /* Re-activation needed due to change in number of required Rbs */
            /* Mark UE for reactivation */
            dlSpsUe->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_FREQ;
         }
      } 
   }
   RETVOID;
}

/**
 * @brief Configuration for SPS service
 * @details
 *
 *     Function : rgSCHCmnSpsDlLcCfg
 *
 *     Invoking Module Processing:
 *     - Common SCH shall invoke this at a DL LC configuration for sps enabled
 *     service
 *
 *     Processing Steps:
 *      - Do validations:
 *          - Validate if the UE has SPS enabled
 *          - Validate if this is the first SPS service for the UE 
 *          - If validation fails, return RFAILED.
 *      - Update the configured values for the LC
 *      - Initialize SPS specific values
 *      - Assign the SPS LC pointer to the UE
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @param[in]  RgrLchCfg        *lcCfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsDlLcCfg
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchCfg          *lcCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsDlLcCfg(cell, ue, dlLc, lcCfg, err)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlLcCb        *dlLc;
RgrLchCfg          *lcCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
#ifdef DEBUGP
   Inst                 instIdx    = cell->instIdx;
#endif /*DEBUGP*/
   TRC2(rgSCHCmnSpsDlLcCfg);
       
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "LC Cfg for SPS DL LC %d\n", dlLc->lcId));

   /* Check if UE is SPS enabled */
   if (!ue->dl.dlSpsCfg.isDlSpsEnabled)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "SPS LC configuration for UE with SPS not enabled: ueId %d,"
               " lcId %d\n", (int)ue->ueId, (int)lcCfg->lcId));
      RETVALUE(RFAILED);

   }

   /* Check if one SPS service is already existing for the UE */
   if (dlSpsUe->spsSvc)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Only one SPS service is supported: attempt to configure more"
               " than 1 SPS service: ueId %d, lcId: %d\n", (int)ue->ueId, 
               (int)lcCfg->lcId));
      RETVALUE(RFAILED);
   }

   /* Note: Assumed that the variables shall be memset to 0
    * during allocation */

   dlSpsUe->spsSvc = dlLc;

   /* Update the configured values */
   dlLc->dlLcSpsCfg = lcCfg->dlInfo.dlSpsCfg;

   RETVALUE(ROK);
} 

/**
 * @brief Deletion for SPS service
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlLcDel
 *
 *     Processing Steps:
 *     - If LC is SPS enabled,
 *       - if (ue->isSpsActv)
 *         - Add UE to toBeRelUes queue for SPS Release
 *         - Remove UE from active list of UEs 
 *       - Remove UE from activation/reactivation SPS Queues
 *       - Set ue->spsSvc = NULLP
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlLcDel
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlLcDel(cell, ue, dlLc)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlLcCb        *dlLc;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                  instIdx    = cell->instIdx;
#endif /*DEBUGP*/
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc  = RG_SCH_CMN_SPS_GET_DL_SVC(dlLc);

   TRC2(rgSCHCmnSpsDlLcDel);
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsDlLcDel: cellId: %d ueId: %d lcId: %d\n", 
            cell->cellId, ue->ueId, dlLc->lcId));

   if (dlSpsUe->spsSvc != dlLc)
   {
      /* Non-SPS service cannot be deleted by SPS module */
      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx),
               "rgSCHCmnSpsDlLcDel: cellId: %d ueId: %d lcId: %d\n", 
               cell->cellId, ue->ueId, dlLc->lcId));
      RETVOID;
   }

   /* Delete SPS service from DL SPS UE */
   if (dlSpsUe->spsSvc)
   {
      if (dlSpsSvc->toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
      }
      if (dlSpsUe->actvUeLstEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, 
                                dlSpsUe->dlSpsUeSchdInfo.schdKey);
      }
   }

   /* Remove UE from toBeActvtdUes or toBeRelUes list */
   if (dlSpsUe->spsList)
   {
      if (dlSpsUe->spsList == &dlSpsCell->toBeActvtdUes)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
      }
      else
      {
         rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue);
      }
   }

   /* If UE is SPS active, clear the allocation of the UE from the cell-wide
    * subframes */
   if (dlSpsUe->isSpsActv)
   {
      /* rgSCHCmnSpsDlUeClearAlloc(cell, ue); */
      rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell,ue);
   }

   cmMemset((U8 *)dlSpsSvc, 0, sizeof(*dlSpsSvc));
   dlSpsUe->spsSvc = NULLP;

   RETVOID;
} 

/**
 * @brief BO update trigger for SPS
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlDedBoUpd
 *
 *     Invoking Module Processing:
 *     - Common SCH shall invoke this on reception of BO update from the UE
 *     for an SPS enabled service.
 *     - It is expected that the Common SCH shall update the bo in lcCb before
 *     invoking this API
 *     - if (svc->isSpsEnabled), Invoke this API
 *     - else, common SCH shall call bo update on other schedulers
 * 
 *     Processing Steps:
 *     - If (bo)
 *       - If svc does not exist in toBeSchdSvcs list && !(ue->isSpsActv)
 *          - Add to the toBeSchdSvcs list
 *       - If (bo <= svc->packetSize  && !(ue->isSpsActv))
 *          - Add UE to the toBeActvtdUes list
 *     -else
 *        - If svc exist in toBeSchdSvcs list, remove from the list 
 *     
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchUeCb              *ue
 *  @param[in]  RgSchDlLcCb            *dlLc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlDedBoUpd
(
RgSchCellCb              *cell,
RgSchUeCb                *ue,
RgSchDlLcCb              *dlLc
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlDedBoUpd(cell, ue, dlLc)
RgSchCellCb              *cell;
RgSchUeCb                *ue;
RgSchDlLcCb              *dlLc;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlLc);
   RgSchCmnDlUeSpsInfo   *dlSpsUe  = RG_SCH_CMN_SPS_GET_DL_UE(ue);

   /* Check if BO update is recieved for UE's SPS service only: this check is
    * important if SPS is disabled for UE but still enabled for LC */
   TRC2(rgSCHCmnSpsDlDedBoUpd);
       
#if (ERRCLASS & ERRCLS_DEBUG)
   if (dlSpsUe->spsSvc != dlLc)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "Bo update recieved for Non-Sps service %d\n", dlLc->lcId));
      RETVOID;
   }
#endif /*(ERRCLASS & ERRCLS_DEBUG)*/
   
   /* Update effective BO of SPS service */
   dlSpsSvc->effSpsBo = dlLc->bo;
   
   /* Handling for BO != 0 */
   /* Note: Adding to both the lists simultaneously, it shall be removed from 
    * to-be scheduled services list post-activation */
   if (dlSpsSvc->effSpsBo)
   {
      if(!(dlSpsUe->isSpsActv) &&
            !(dlSpsSvc->toBeSchdSvcEnt.node) &&
            (dlSpsSvc->effSpsBo > dlLc->dlLcSpsCfg.packetSize))
      {
         rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlLc); 
      } 
      if(!(dlSpsUe->isSpsActv) && 
            (dlSpsSvc->effSpsBo <= dlLc->dlLcSpsCfg.packetSize))
      {
         dlSpsUe->actionPndng |= RG_SCH_CMN_SPS_DL_ACTV; 
         ue->dl.isSpsHqPInUse = TRUE;
         rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue); 
      } 
   }
   else
   {
      /* Handling for BO == 0 */
      if(dlSpsSvc->toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlLc); 
      } 
   }
   RETVOID;
}

/**
 * @brief UE Reset
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeReset
 *
 *     Processing Steps:
 *     - Clear the configured grant for the UE.
 *     - Reset bo for SPS service of UE.
 *     - Remove UE and SPS service from all queues.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlUeReset
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlUeReset(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif /* ANSI */
{
   /* Note:Invoked for DL SPS enabled UE only and before invoking other
    * schedulers to reset the UE */
   TRC2(rgSCHCmnSpsDlUeReset);
       
   rgSCHCmnSpsDlUeSuspend(cell, ue);
   RETVOID;
}

/**
 * @brief Handler for proc requiring re-transmission.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcAddToRetx
 *
 *     Processing Steps:
 *     - Handles HARQ procs pending for re-transmission with SPS data.
 *     - Add hqP to spsCell->retxHqProcs
 *
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchDlHqProcCb          *hqP
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlProcAddToRetx
(
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlProcAddToRetx(cell, hqP)
RgSchCellCb           *cell;
RgSchDlHqProcCb       *hqP;
#endif /* ANSI */
{

   RgSchCmnDlCellSpsInfo *dlSpsCell;
   TRC2(rgSCHCmnSpsDlProcAddToRetx);
   dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   
   /* Add to the list of re-transmitting HARQ SPS procs in the cell */
   rgSCHCmnSpsUtlDlAdd2RetxHqProcs(dlSpsCell, hqP);
   
   RETVOID;
}

/**
 * @brief Handler for processing DTX recieved for a HARQ process
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcDtx
 *
 *     Processing Steps:
 *     - If (hqP->spsAction |= RG_SCH_CMN_SPS_DL_ACTV ||
 *          hqP->spsAction |= RG_SCH_CMN_SPS_DL_REACTV)
 *        - Mark the UE to be send with PDCCH at next SPS ocassion. Set
 *        spsUe->pdcchPndng = TRUE.
 *        - Free the SPS HARQ proc. Mark hqP->isSpsActv = FALSE;
 *        
 *      
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchDlHqProcCb          *hqP
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlProcDtx
(
RgSchCellCb            *cell,
RgSchDlHqProcCb        *hqP
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlProcDtx(cell, hqP)
RgSchCellCb            *cell;
RgSchDlHqProcCb        *hqP;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchUeCb             *ue = hqP->hqE->ue;
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);
   RgSchCmnDlHqProc      *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
   RgSchDlHqEnt         *hqE;
#ifdef LTEMAC_DRX
   RgSchDrxDlHqProcCb  *drxHq   =NULLP; 
   RgSchDRXCellCb      *drxCell =NULLP;
#endif
   TRC2(rgSCHCmnSpsDlProcDtx);

   hqE = hqP->hqE;
   
   if ((cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_ACTV) ||
         (cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_REACTV))
   {
      /* HARQ proc used for activation/re-activation of DL SPS */
      /* Mark UE to be sent with PDCCH at next SPS ocassion */
      dlSpsUe->dlSpsUeSchdInfo.pdcchPndng = TRUE;
      /* Activation/re-activation PDCCH not recieved, increment effSpsBo by
       * packet size */
      dlSpsSvc->effSpsBo += dlSpsUe->spsSvc->dlLcSpsCfg.packetSize;
   }
   else
   {
#ifdef LTEMAC_DRX
      drxCell = RG_SCH_DRX_GET_CELL(cell);
      drxHq   = RG_SCH_DRX_GET_DL_HQ(hqP);
      
      if((ue != NULLP) && (ue->isDrxEnabled == TRUE))
      {
          if((drxHq->rttIndx) != DRX_INVALID)
          {
               rgSCHDrxStartHarqRTTTmr(cell, hqP, 0);
          }
      }
      else
#endif
      /* Add to the list of re-transmitting HARQ SPS procs in the cell */
      /* Note: Assumed that redundancy version shall be taken care by common
       * code */
      {
      rgSCHCmnSpsUtlDlAdd2RetxHqProcs(dlSpsCell, hqP);
      }
   }

   RETVOID;
}

/**
 * @brief Handler for processing feedback recieved for release PDCCH
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlRelPdcchFbk
 *
 *     Processing Steps:
 *     - If ACK
 *       - Remove UE from toBeRelUes list
 *       - Clear UE specific SPS allocation information
 *        
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue       
 *  @param[in]  Bool          isAck
 *
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlRelPdcchFbk
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
Bool                  isAck
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlRelPdcchFbk(cell, ue, isAck)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
Bool                  isAck;
#endif /* ANSI */
{
   /* Assumed that feedback for release PDCCH shall be sent at index 0. 
    * from PHY */
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   /*rg002.301 ccpu00119494-ADD-added handling to retransmit
    * RelPdcch in case no feedback is received */
   CmLteTimingInfo       relFdbkTime;

   TRC2(rgSCHCmnSpsDlRelPdcchFbk);
       
   if ((isAck == TFU_HQFDB_ACK) ||
           (dlSpsUe->numRelPdcchSent > cell->dlHqCfg.maxDlHqTx))
   {
      /* Release successful for SPS UE */

      /* Clear UE's allocation from the subframes */
      rgSCHCmnSpsDlUeClearAlloc(cell, ue);

      /* Remove UE from to be released UEs list */
      rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue);
     

      /* If Ue's SPS service has BO, add to toBeSchdSvcs list */
      if (dlSpsUe->spsSvc)
      {
         RgSchCmnDlSvc  *spsSvcCmn = RG_SCH_CMN_GET_DL_SVC(dlSpsUe->spsSvc);
         if(spsSvcCmn->dlSvcSpsInfo.effSpsBo)
         {
            rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
         }
      }
      
   }
   else
   {
      /* Note: No special handling needed for NACK and DTX: re-transmission of
       * relPdcch taken care through usual toBeRel UE list */
      dlSpsUe->isRelPdcchSent = FALSE;

   }

  /*rg002.301 ccpu00119494-ADD-added handling to retransmit
   * RelPdcch in case no feedback is received */
   
   /* UE is added to wtngForRelFdbkUeLst at 
    * expected fdb time + TFU_HQFBKIND_ULDELTA time.
    * At this time if the UE still
    * exists in wtngForRelFdbkUeLst, we know for sure
    * UE has not received any feedback
    * crntTime is expected fdbk time + delta that PHY takes
    * to deliver the Harq. However, we add to wtngForRelFdbkUeLst
    * 1 tti after this time. 
    * Therefore,we add 1 to determine the correct index of insertion */
   RGSCHADDTOCRNTTIME(cell->crntTime,relFdbkTime,1);
   cmLListDelFrm(&(dlSpsCell->wtngForRelFdbkUeLst[relFdbkTime.subframe]),
                  &dlSpsUe->wtngForRelFdbkUeEnt);

#ifdef LTE_TDD
   /* Reset the transmission time of relPdcch */
   ue->relPdcchTxTime.sfn = RGSCH_MAX_SFN + 1;
#endif /*LTE_TDD*/
   /* Reset release PDCCH feedback timing */
   ue->relPdcchFbkTiming.sfn = RGSCH_MAX_SFN + 1;

   RETVOID;
}

#ifdef RG_UNUSED
/* Invoked by HD-FDD module to get list of DL SPS active UEs in a TTI */
/**
 * @brief Determines the list of UEs Active for this sub-frame
 *
 * @details
 *
 *     Function : rgSCHCmnSpsGetDlActvUe
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked to get list of UEs active for DL SPS
 *        
 *     Processing Steps:
 *     - Compute the index (idx) into cell->actvDlSpsUeLsts
 *     - Set *dlSpsActvUeLst = cell->actvDlSpsUeLsts[idx]
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out] CmLListCp    *dlSpsActvUeLst
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsGetDlActvUe
(
RgSchCellCb     *cell,
CmLteTimingInfo *timingInfo,
CmLListCp       *dlSpsActvUeLst
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsGetDlActvUe(cell, timingInfo, dlSpsActvUeLst)
RgSchCellCb     *cell;
CmLteTimingInfo *timingInfo;
CmLListCp       *dlSpsActvUeLst;
#endif /* ANSI */
{
   U16                   idx;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   
   TRC2(rgSCHCmnSpsGetDlActvUe);
   /* Note: Assumed that the list will be used for read-only purpose */

   idx = (timingInfo->sfn * 10 + timingInfo->subframe) % 
      RG_SCH_CMN_SPS_MAX_PRD;

   *dlSpsActvUeLst = dlSpsCell->actvDlSpsUeLsts[idx];

   RETVALUE(ROK);
}
#endif /* RG_UNUSED */

/**
 * @brief Trigger for DL scheduling for SPS UEs.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlSched
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this at TTI for SPS scheduling in DL
 *      sub-frame.
 *      - This API shall be invoked before invoking TTI processing API for
 *      other schedulers
 *        
 *     Processing Steps:
 *     - Process UEs with configured grant for the target DL subframe. Call
 *       rgSCHCmnSpsDlProcActvtdUes()
 *     - Process UEs with release pending for the target DL subframe.Call 
 *       rgSCHCmnSpsDlProcToBeRelUes()
 *     - Process UEs with activation pending for the target DL subframe.Call 
 *       rgSCHCmnSpsDlProcToBeActvtdUes()
 *     - Process UEs with pending re-transmissions. Call
 *       rgSCHCmnSpsDlProcRetxUes()
 *     - Process UEs with services pending for Non-SPS scheduling.Call 
 *       rgSCHCmnSpsDlProcToBeSchdSvcs()
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo,
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlSched
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlSched(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                  instIdx       = cell->instIdx;
#endif /*DEBUGP*/
   CmLteTimingInfo       crntTime;
   RgSchDlSf             *dlSf;
   
   TRC2(rgSCHCmnSpsDlSched);
   /* Get the current time for the cell */ 
   crntTime = cell->crntTime;

   /* Get Downlink Subframe */
   dlSf = allocInfo->dedAlloc.dedDlSf;

   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsDlSched: cellId: %d\n", cell->cellId));

   /* Handle re-transmitting UEs */
   rgSCHCmnSpsDlProcRetxUes(cell, crntTime, dlSf, allocInfo);

   /* Handle already activated UEs*/
   rgSCHCmnSpsDlProcActvtdUes(cell, crntTime, dlSf, allocInfo);

   /* Handle to be released UEs */
   rgSCHCmnSpsDlProcToBeRelUes(cell, crntTime, dlSf);

   /* Handle to be activated UEs */
   rgSCHCmnSpsDlProcToBeActvtdUes(cell, crntTime, dlSf);

   /* Handle to be scheduled services */
   rgSCHCmnSpsDlProcToBeSchdSvcs(cell, crntTime, dlSf, allocInfo);

   /* Update subframe-wide allocation information with SPS allocation */
   rgSCHCmnSpsDlUpdDlSfAllocWithSps(cell, crntTime, dlSf);

   RETVOID;
}

/**
 * @brief Update DL SF with SPS allocation information in the sub-frame
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdDlSfAllocWithSps
 *
 *
 *     Processing Steps:
 *     
 *  @param[in]  RgSchCellCb     *cell
 *  @parama[in] CmLteTimingInfo crntTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlUpdDlSfAllocWithSps
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlUpdDlSfAllocWithSps(cell, crntTime, dlSf)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   CmLteTimingInfo       schdTime;
   U16                   dlSpsSfIdx;   
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnSpsDlSf       *spsSf;
   U8                    idx;   
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif /*DEBUGP*/
   TRC2(rgSCHCmnSpsDlUpdDlSfAllocWithSps);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHCmnSpsDlUpdDlSfAllocWithSps\n"));

   schdTime = crntTime;
   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);

   /* Determine DL SPS subframe index */
   dlSpsSfIdx = (schdTime.sfn * 10 + schdTime.subframe) %
      dlSpsCell->spsPrdLcmVal;
   spsSf = &dlSpsCell->spsSfTbl[dlSpsSfIdx];

   /* Update allocation from DL SPS SF to dlSf */
   dlSf->dlSfAllocInfo.raType0Mask        |= spsSf->spsAllocInfo.raType0Mask;
   for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; idx++)
   {
      dlSf->dlSfAllocInfo.raType1Mask[idx] |= 
         spsSf->spsAllocInfo.raType1Mask[idx];
      dlSf->dlSfAllocInfo.raType1UsedRbs[idx] |= 
         spsSf->spsAllocInfo.raType1UsedRbs[idx];
   }
   dlSf->dlSfAllocInfo.nxtRbgSubset = spsSf->spsAllocInfo.nxtRbgSubset;
   for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; idx++)
   {
      dlSf->dlSfAllocInfo.raType2Mask[idx] |= 
         spsSf->spsAllocInfo.raType2Mask[idx];
   }

   dlSf->spsAllocdBw += spsSf->rbsAlloc;
   RETVOID;
}


/**
 * @brief Handles Retransmitting SPS UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcRetxUes
 *
 *
 *     Processing Steps:
 *     - This API handles retransmitting HARQ SPS procs. Retransmitting SPS HARQ
 *     procs include all HARQ procs with isSpsSvcSchd = TRUE.
 *     - For each hqProc in retxHqProcs,
 *        - if (current_time > hqP->maxRetxTime)
 *           - Delete the HARQ proc from retxHqProcs list
 *           - Free the HARQ proc
 *           - continue
 *        - Determine the RBs needed for the re-transmission based on the tbSize
 *        - Add to the re-transmitting SPS queue in allocInfo 
 *     
 *  @param[in]  RgSchCellCb     *cell
 *  @parama[in] CmLteTimingInfo crntTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlProcRetxUes
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlProcRetxUes(cell, crntTime, dlSf, allocInfo)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   CmLListCp            *retxLst;
   CmLList              *node;
   RgSchDlHqProcCb      *hqP;
   RgSchCmnDlUe         *cmnUeDl;
   RgSchCmnDlHqProc     *cmnHqDl;
   U32                  effBo;
   RgSchUeCb            *ue = NULLP;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   CmLteTimingInfo       schdTime;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif /*DEBUGP*/
   TRC2(rgSCHCmnSpsDlProcRetxUes);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHCmnSpsDlProcRetxUes\n"));

   schdTime = crntTime;
   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);


   retxLst = &dlSpsCell->retxHqProcs;
   /* allocate bw for the retransmission..should be same are previous */
   /* If CQI gets worse, as we cannot find same TB size for another   */
   /* MCS, we just remove this from the retransmission queue          */
   node = retxLst->first;
   while (node != NULLP)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
      node = node->next;
      ue = hqP->hqE->ue;
      
      /* Check if this UE is already scheduled for transmission/
       * active to be scheduled */
      if (rgSCHCmnSpsDlChkUeInActv(cell, ue, schdTime))
      {
         RGSCHDBGINFO(inst, (rgSchPBuf(inst), 
                  "\n UE %d inactive at SPS ocassion: sfn %d subframe %d", 
                  ue->ueId, schdTime.sfn, schdTime.subframe));
         continue;
      }
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);

      /* If HARQ proc corresponds to SPS transmission, check for the 
       * maximum time for re-transmission */
      if (cmnHqDl->isSpsActv)
      {
         S8       ret;
         RGSCH_TIMEINFO_CMP(schdTime, cmnHqDl->maxRetxTime, &ret);
         if (ret >= 0)
         {
            /* Current time is greater than maximum time of re-transmission: 
             * Remove the HARQ proc from retxHqProcs list */
            rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP);
      /* rg001.301 -MOD-ccpu00118350 : Correcting NDI manipulation of Harq */
            rgSCHDhmRlsHqpTb(hqP, 0, TRUE);
            rgSCHDhmRlsHqpTb(hqP, 1, TRUE);
            continue;
         }
      }
      effBo = 0;
      if (rgSCHCmnDlAllocRetxRb(cell, dlSf, ue, 0, &effBo, hqP, allocInfo) 
            != ROK)
      {
         /* SF/RETX Bandwidth expired */
         RETVOID;
      }
      if (effBo == 0)
      {
         continue;
      }
      /* Mark HARQ Proc to have invalid SPS n1Pucch resource */
      hqP->spsN1PucchRes.pres = FALSE;
      cmnUeDl->proc = hqP;
      
      /* Mark UE to be scheduled by SPS module */
      ue->dl.dlAllocCb.spsSchdTime = schdTime;
      rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst(allocInfo, hqP);
   }
   RETVOID;
}


/**
 * @brief Handles Activated SPS UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcActvtdUes
 *
 *
 *     Processing Steps:
 *     - If UE is HD-FDD, 
 *       - Check if the subframe corrsponding to 'frm' is DL subframe 
 *         else return. Invoke API/MACRO on HD-FDD.
 *     - If UE has measurement gap enabled, 
 *       - Check if UE can be scheduled in DL else continue.
 *         Invoke API/MACRO on measurement gap module.
 *     - If UE has DRX enabled, 
 *       - Check if UE can be scheduled in DL else continue.
 *         Invoke API/MACRO on DRX module.
 *     - Determine the index into the cell->actvDlSpsUeLsts using following
 *       - Determine frm for (cell->crntTime + DL_DELTA)
 *       - (frm.sfn * 10 + frm.subframe) % RG_SCH_CMN_SPS_MAX_PRD
 *     - for each Ue in the list,
 *       - if (ue->spsSvc == NULL)
 *        {
 *           - Service might have been deleted and ue should be awaiting
 *             release
 *           - Remove UE from toBeActvtdUes list
 *           - Continue to next UE
 *        }
 *       - if (ue->spsSvc->bo == 0)
 *        {
 *           - ue->spsSvc->zeroBoOcassionCnt++
 *           - if (ue->spsSvc->zeroBoOcassionCnt == svc->explicitRelCnt)
 *             - Add UE to toBeRelUes list
 *             - Remove UE from toBeActvtdUes list
 *           - return
 *        }
 *       - Reset zeroBoOcassionCnt
 *       - if (ue->spsSvc->bo > ue->spsSvc->packetSize)
 *        {
 *           - Schedule UE using C-RNTI
 *           - Call rgSCHCmnSpsDlAllocForSvc()
 *        }
 *       - if (ue->spsSvc->bo <= ue->spsSvc->packetSize)
 *        {
 *           - Schedule UE using SPS-RNTI
 *           - If successful,
 *             - Get SPS harq Proc. Call rgSCHDhmSpsDlGetHqProc()
 *             - Check if (ue->actionPndng  == RG_SCH_CMN_SPS_DL_REACTV_FREQ)
 *             {
 *                - Reactivate UE with change in number and position of RBs
 *                - Call rgSCHCmnSpsDlProcToBeReactvtdUe() 
 *                - continue 
 *             }
 *             else
 *             {
 *                - Check if (ue->pdcchPndng), fetch PDCCH and update the
 *                  information in pdcch
 *             }
 *             - Update DHM with logical channel's data information
 *             - Update the HARQ proc with the allocation information
 *             - Set hqP->isSpsActv = TRUE
 *             - Add HARQ proc to the subframe.
 *             - Update the maximumRetxTime in the hqP
#ifdef LTE_TDD
 *             - increment numCfgGrntTx in the UE's dlSpsInfo
#endif
 *        }
 *  @param[in]  RgSchCellCb           *cell
 *  @parama[in] CmLteTimingInfo       crntTime
 *  @param[in]  RgSchDlSf             *dlSf
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlProcActvtdUes
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlProcActvtdUes(cell, crntTime, dlSf, allocInfo)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo;
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc;
   U32                   spsPrd;
   CmLList               *listNode;
   CmLteTimingInfo       schdTime;
   U16                   actvtdLstIdx;
   RgSchUeCb             *ue;
   U8                    n1PucchIdx;
   Bool                  isActvOrReactv = FALSE;
   TRC2(rgSCHCmnSpsDlProcActvtdUes);
   
   schdTime = crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
   actvtdLstIdx = (schdTime.sfn * 10 + schdTime.subframe) % 
      RG_SCH_CMN_SPS_MAX_PRD;
   listNode = dlSpsCell->actvDlSpsUeLsts[actvtdLstIdx].first;

   while (listNode)
   {
      ue       = (RgSchUeCb *)(listNode->node);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
      dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
      spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
      listNode = listNode->next;


      /* Check if UE can be scheduled in downlink */
      if (rgSCHCmnSpsDlChkUeInActv(cell, ue, schdTime))
      {
         RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), 
                  "\n UE %d inactive at SPS ocassion: sfn %d subframe %d", 
                  ue->ueId, schdTime.sfn, schdTime.subframe));
         rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
         RGSCHADDTOCRNTTIME(schdTime, dlSpsUeSchdInfo->schdKey , spsPrd);
         rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, dlSpsUeSchdInfo->schdKey);
         continue;
      }

#if (ERRCLASS & ERRCLS_DEBUG)
      if (!dlSpsUe->spsSvc)
      {
         /* SPS Service does not exist: this condition might not occur since
          * service deletion should cause removal from actvUesLst as well */
         RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), 
                  "\n UE %d has NULL SPS service at SPS ocassion: "
                  " sfn %d subframe %d", ue->ueId, schdTime.sfn,
                  schdTime.subframe));
         rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
         continue;
      }
#endif /*(ERRCLASS & ERRCLS_DEBUG)*/

      dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);

      /* Zero BO SPS ocassion */
      if (!dlSpsSvc->effSpsBo)
      {
         dlSpsSvc->zeroBoOcassionCnt++;
         if (dlSpsSvc->zeroBoOcassionCnt == 
               dlSpsUe->spsSvc->dlLcSpsCfg.explicitRelCnt)
         {
            RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), 
                  "\n UE %d explict release at SPS ocassion: sfn %d "
                  "subframe %d", ue->ueId, schdTime.sfn, schdTime.subframe));
            /* Remove from toBeActvtdUes, if UE exists in the queue for
             * re-activation */
            rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
            /* Explicit release needed */
            rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell, ue);
            rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
            dlSpsSvc->zeroBoOcassionCnt = 0;
         }
         else
         {
            rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
            RGSCHADDTOCRNTTIME(schdTime, dlSpsUeSchdInfo->schdKey , spsPrd);
            rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, 
                                  dlSpsUeSchdInfo->schdKey);
         }
         continue;
      }

      /* If non-zero BO is recieved, reset zeroBoOcassionCnt */
      dlSpsSvc->zeroBoOcassionCnt = 0;

      /* BO exceeds packet size, schedule using CRNTI */
      if (dlSpsSvc->effSpsBo > dlSpsUe->spsSvc->dlLcSpsCfg.packetSize)
      {
         rgSCHCmnSpsDlAllocForSvc(cell, schdTime, dlSf, dlSpsUe->spsSvc,
               dlSpsSvc->effSpsBo, allocInfo);

         /* Move UE to next SPS ocassion */
         rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
         RGSCHADDTOCRNTTIME(schdTime, dlSpsUeSchdInfo->schdKey , spsPrd);
         rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, 
                        dlSpsUeSchdInfo->schdKey);
         continue;
      }

      /* BO within packet size limits, use SPS ocassion and SPS RBs */
      /* If the UE is been re-activated for change in number of RBs */
      if (dlSpsUe->actionPndng == RG_SCH_CMN_SPS_DL_REACTV_FREQ)
      {
         if ((rgSCHCmnSpsDlProcToBeReactvtdUe(cell, ue, schdTime, dlSf) != ROK))
         {
            RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), 
                     "\n Re-activation failed for UE : %d, re-activation "
                     "time: sfn %d subframe %d", ue->ueId,
                     schdTime.sfn, schdTime.subframe));
            /* Add UE to toBeActvtdUes list for re-activation */
            rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue);
         }
      }
      else
      {
         /* Update allocCb of UE with the scheduled SPS allocation */
         ue->dl.dlAllocCb = dlSpsUeSchdInfo->spsAllocInfo;
         n1PucchIdx = dlSpsUeSchdInfo->allocN1PucchIdx;

         /* Allocation finalization for an SPS UE */
         if (rgSCHCmnSpsDlUeAllocFnlz(cell, ue, schdTime, dlSf,
                  n1PucchIdx, isActvOrReactv) != ROK)
         {
            RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), 
                     "\nRB allocation failed for activated UE: %d, "
                     "SPS ocasssion : sfn %d subframe %d", ue->ueId, schdTime.sfn, 
                     schdTime.subframe));
         }
      }
   } /* End of while (listNode) */

   RETVOID;
}

/**
 * @brief Handles to-be activated SPS UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcToBeActvtdUes
 *
 *
 *     Processing Steps:
 *     - Determine the index into the cell->actvDlSpsUeLsts using following
 *       - Determine frm for (cell->crntTime + DL_DELTA)
 *       - (frm.sfn * 10 + frm.subframe) % RG_SCH_CMN_SPS_MAX_PRD
 *     - For each UE in toBeActvtdUes UE list,
 *       - Check collision in time domain. Call rgSCHCmnSpsDlUeCollidesInTime.
 *       - Check collision in frequency domain or n1Pucch resources. 
 *       Call rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch.
 *       - Determine allocation for the UE. Use resource allocation type 1 and
 *       0 for allocation within maximum SPS bandwidth.Call
 *       rgSCHCmnSpsDlUeAllocRbs. 
 *       - Finalize allocation. Call rgSCHCmnSpsDlUeAllocFnlz.
 *       - If failed,
 *         - Move UE to the end of toBeActvtdUes list
 *         - break from the loop
 *       
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @parama[in] CmLteTimingInfo       crntTime
 *  @param[in]  RgSchDlSf             *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlProcToBeActvtdUes
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf
)
#else/* ANSI */
PRIVATE S16 rgSCHCmnSpsDlProcToBeActvtdUes(cell, crntTime, dlSf)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   CmLList               *listNode  = dlSpsCell->toBeActvtdUes.first;
   RgSchUeCb             *ue;
   RgSchDlSfAllocInfo    allocInfo;
   /* rg009:Changes Added the line here */
   CmLteTimingInfo       schdTime = crntTime;
   Bool                  isActvOrReactv = TRUE;
   U8                    n1PucchIdx;
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc;
#ifdef DEBUGP   
   Inst                  inst = cell->instIdx;
   /*rg009.201 Added the change to Initialize The frm reported by Insure */
   CmLteTimingInfo       frm = schdTime;
#endif

   TRC2(rgSCHCmnSpsDlProcToBeActvtdUes);

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);

   while (listNode)
   {
      ue       = (RgSchUeCb *)(listNode->node);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);

      /* Check if UE is already scheduled by SPS module */
      if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
      {
         RGSCHDBGINFO(inst, (rgSchPBuf(inst),"\nrgSCHCmnSpsDlProcToBeActvtdUes:"
                  "Ue already scheduled by SPS module %d", ue->ueId));
         listNode = listNode->next;
         continue;
      }

      /* Check if UE collides in time domain and if the current activation
       * offset needs to be changed */
      if (rgSCHCmnSpsDlUeCollidesInTime(cell, ue, schdTime))
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), 
                  "\nSPS Collision in time domain for UE: %d, activation "
                  "time: sfn %d subframe %d", ue->ueId, schdTime.sfn, 
                  schdTime.subframe));
         rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
         rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue);
         RETVALUE(RFAILED);
      }

      /* Check if UE collides in frequency domain or in n1Pucch resources: 
       * no resources available at this offset */
      /* Set n1PucchIdx to an invalid value */
      n1PucchIdx = RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE + 1;
      if (rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch(cell, ue, schdTime, 
               &allocInfo, &n1PucchIdx))
      {

         RGSCHDBGERR(inst, (rgSchPBuf(inst), 
             "\nSPS Collision in frequency domain for UE: %d, activation "
             "time: sfn %d subframe %d", ue->ueId, frm.sfn, frm.subframe));
         rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
         rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue);
         RETVALUE(RFAILED);
      }

      /* Attempt allocation for SPS UE */
      if ((rgSCHCmnSpsDlUeAlloc(cell, ue, schdTime, dlSf, &allocInfo)) != ROK)
      {

         RGSCHDBGERR(inst, (rgSchPBuf(inst), 
                  "\nRB allocation failed for UE: %d, "
                  "activation time: sfn %d subframe %d", ue->ueId, frm.sfn, 
                  frm.subframe));
         rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
         rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue);
         RETVALUE(RFAILED);
      }

      /* Allocation finalization for an SPS UE */
      if (rgSCHCmnSpsDlUeAllocFnlz(cell, ue, schdTime, dlSf, 
               n1PucchIdx, isActvOrReactv) != ROK)
      {
        /*As part of purify fixes frm variable is included in case only DEBUGP is enabled */ 
         RGSCHDBGERR(inst, (rgSchPBuf(inst), 
                "\nRB allocation failed for UE: %d, "
                "activation time: sfn %d subframe %d", ue->ueId, frm.sfn, 
                frm.subframe));
         rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
         rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue);
         RETVALUE(RFAILED);
      }

      /* Remove UE from toBeActvtd list of UEs */
      rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);

      /* Remove service from toBeSchdSvcs list, if present */
      dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);
      if(dlSpsSvc->toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
      }

      /* Set UE to be SPS active */ 
      dlSpsUe->isSpsActv = TRUE;
      ue->dl.isSpsHqPInUse = TRUE;

      /* Unmark the pending action */
      dlSpsUe->actionPndng = 0;

      /* Breaking from the loop since activation for only one UE is attempted
       * per TTI */
      break;
   }

   RETVALUE(ROK);
}

/**
 * @brief Handles to-be re-activated SPS UEs for change in number of RBs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcToBeReactvtdUe
 *
 *
 *     Processing Steps:
 *       - Check collision in frequency domain or n1Pucch resources. 
 *       Call rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch.
 *       - Determine allocation for the UE. Use resource allocation type 1 and
 *       0 for allocation within maximum SPS bandwidth.Call
 *       rgSCHCmnSpsDlUeAllocRbs. 
 *       - Finalize allocation. Call rgSCHCmnSpsDlUeAllocFnlz.
 *       
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  CmLteTimingInfo       *schdTime
 *  @param[in]  RgSchDlSf             *dlSf
 *  @return  S16
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlProcToBeReactvtdUe
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf
)
#else/* ANSI */
PRIVATE S16 rgSCHCmnSpsDlProcToBeReactvtdUe(cell, ue, schdTime, dlSf)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
CmLteTimingInfo       schdTime;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   RgSchDlSfAllocInfo    allocInfo;
   U8                    n1PucchIdx;
   Bool                  isActvOrReactv = TRUE;
#ifdef DEBUGP
   Inst                  inst = cell->instIdx;
   CmLteTimingInfo       frm = schdTime;
#endif

   TRC2(rgSCHCmnSpsDlProcToBeReactvtdUe);

   /* Note: It is assumed in this function that dlSpsUe->isSpsActv = TRUE and
    * dlSpsUe has valid SPS scheduling information  */

   /* Check if UE collides in frequency domain or in n1Pucch resources: 
    * no resources available at this offset */
   n1PucchIdx = dlSpsUeSchdInfo->allocN1PucchIdx; 
   if (rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch(cell, ue, schdTime, 
            &allocInfo, &n1PucchIdx))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), 
               "\nSPS Collision in frequency domain for UE: %d, activation "
               "time: sfn %d subframe %d", ue->ueId, frm.sfn, frm.subframe));
      RETVALUE(RFAILED);
   }

   /* Attempt allocation for SPS UE */
   if ((rgSCHCmnSpsDlUeAlloc(cell, ue, schdTime, dlSf, &allocInfo)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), 
               "\nRB allocation failed for UE: %d, "
               "activation time: sfn %d subframe %d", ue->ueId, frm.sfn, 
               frm.subframe));
      RETVALUE(RFAILED);
   }

   /* Allocation finalization for an SPS UE */
   if (rgSCHCmnSpsDlUeAllocFnlz(cell, ue, schdTime, dlSf, 
            n1PucchIdx, isActvOrReactv) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), 
               "\nRB allocation failed for UE: %d, "
               "activation time: sfn %d subframe %d", ue->ueId, frm.sfn, 
               frm.subframe));
      RETVALUE(RFAILED);
   }

   /* Reset the pending action for the UE */
   dlSpsUe->actionPndng = 0;

   RETVALUE(ROK);
}
/**
 * @brief Checks collision of allocation in time domain
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeCollidesInTime
 *
 *
 *     Processing Steps:
 *       - If UE is in HD-FDD mode, check collision with SR, CQI, SRS for the
 *       'frm' for current and future SPS ocassions.
 *       - If UE has measurement gap enabled, check collision with measurement
 *       gap using measGapMask for current and future SPS ocassions.
 *       - If DRX is enabled, the selected offset should lie within the
 *       on-Duration timer value for the UE
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchUeCb             *ueCb
 *  @param[in]  CmLteTimingInfo       timingInfo
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInTime
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        timingInfo
)
#else/* ANSI */
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInTime(cell, ue, timingInfo)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
CmLteTimingInfo        timingInfo;
#endif /* ANSI */
{
    U16              startIdx;
    U16              prd;
    U16              spsPrd;
    U8               spsPrdIdx;
    Bool             collides = FALSE;
    RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);

    TRC2(rgSCHCmnSpsDlUeCollidesInTime);

    spsPrdIdx = dlSpsUe->prdIdx;
    if(spsPrdIdx == RG_SCH_CMN_SPS_PRD_INVALID )
       RETVALUE(collides);

    spsPrd    = (U16)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;

    /* If HD-FDD UE, check collision with SR, SRS and CQI */
#ifdef RG_UNUSED
    startIdx = (timingInfo.sfn * 10 + timingInfo.subframe) % prd;
    if (collides = 
            rgSCHCmnSpsDlChkCollision(spsPrdIdx, startIdx, prd, spsPrd, offset))
    {
        RETVALUE(collides);
    }
    /* For DRX active UE, check if the scheduling offset
     * falls during 'OnDuration' period for the UE */
#endif /*RG_UNUSED*/

   /* If measurement gap enabled, check collision for measurement gap */
   if (ue->measGapCb.isMesGapEnabled)
   {
      prd = ue->measGapCb.gapPrd;
      startIdx = (cell->crntTime.sfn * 10 + cell->crntTime.subframe) % prd; 
      if ((collides =
            rgSCHCmnSpsDlChkMeasGapCollision(spsPrdIdx , startIdx, prd, 
               spsPrd, (U32 *)dlSpsUe->measGapMask)))
      {
         RETVALUE(collides);
      }
   }
      /*  For DRX active UE, check if the scheduling offset
       * falls during 'OnDuration' period for the UE .
       *  we checked for SPS periodicity being multiple of DRX cycle at 
       *  configuration so that SPS occassions fall on on-duration*/
#ifdef LTEMAC_DRX
   if(ue->isDrxEnabled)
   {
       /*if UE is not on onduration then dont schedule it*/
       if(RG_SCH_DRX_DL_IS_UE_ONDUR_INACTIVE(ue->drxCb))
       {
          RETVALUE(TRUE);
       }
   }
#endif

    RETVALUE(collides);
}

/**
 * @brief Checks collision of allocation with measurement gap
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlChkMeasGapCollision
 *
 *
 *     Processing Steps:
 *       - If UE has measurement gap enabled, check collision with measurement
 *       gap using measGapMask for current and future SPS ocassions.
 *       - It checks collision using the measurement gap bitmask for the UE
 *       using the time at which scheduling is attempted.
 *
 *  @param[in]  U8                    spsPrdIdx
 *  @param[in]  U16                   startIdx
 *  @param[in]  U16                   measGapPrd
 *  @param[in]  U16                   spsPrd
 *  @param[in]  U8                    *measGapMask
 *  @return  Bool
 *             TRUE     Collision detected
 *             FALSE    No collision
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlChkMeasGapCollision
(
U8                     spsPrdIdx,
U16                    startIdx,
U16                    measGapPrd,
U16                    spsPrd,
U32                     *measGapMask
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsDlChkMeasGapCollision(spsPrdIdx, startIdx, 
      measGapPrd, spsPrd, measGapMask)
U8                     spsPrdIdx;
U16                    startIdx;
U16                    measGapPrd;
U16                    spsPrd;
U32                     *measGapMask;
#endif /* ANSI */
{
   U8                  measGapPrdIdx;
   U16                 cycleCnt;
   U16                 lcm;
   U8                  idx;
   U8                  maskIdx;
   U8                  posInMask;

   TRC2(rgSCHCmnSpsDlChkMeasGapCollision);

   /* If measurement gap enabled, check collision for measurement gap */
   RG_SCH_CMN_SPS_GET_PRD_IDX (measGapPrd, &measGapPrdIdx);
   lcm = rgSchCmnSpsLcmTbl[spsPrdIdx][measGapPrdIdx];
   cycleCnt = lcm/spsPrd;
   idx = (U8)startIdx;

   while (cycleCnt)
   {
      maskIdx = idx / 32;
      posInMask = idx % 32;
      if ((measGapMask[maskIdx]) & 
            (1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(posInMask)))
      {
         break;
      }
      idx = (idx + spsPrd) % measGapPrd;
      cycleCnt--;
   }
   if (cycleCnt)
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}

#ifdef RG_UNUSED
/* This function to be used after introduction of HD_FDD module */
/**
 * @brief Checks collision of allocation with a given periodicity and offset
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlChkCollision
 *
 *
 *     Processing Steps:
 *       - It checks collision of allocation for given offset and periodicity of
 *       the SPS.
 *
 *  @param[in]  U8                    spsPrdIdx
 *  @param[in]  U16                   startIdx
 *  @param[in]  U16                   prd
 *  @param[in]  U16                   spsPrd
 *  @param[in]  U16                   offset
 *  @return  Bool
 *             TRUE     Collision detected
 *             FALSE    No collision
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlChkCollision
(
U8                     spsPrdIdx,
U16                    startIdx,
U16                    prd,
U16                    spsPrd,
U16                    offset
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsDlChkCollision(spsPrdIdx, startIdx, 
      prd, spsPrd, offset)
U8                     spsPrdIdx;
U16                    startIdx;
U16                    prd;
U16                    spsPrd;
U16                    offset;
#endif /* ANSI */
{
   U8                  prdIdx;
   U16                 cycleCnt;
   U16                 lcm;
   U8                  idx;

   TRC2(rgSCHCmnSpsDlChkCollision);

   /* Get the index for the periodicity of the feature */
   RG_SCH_CMN_SPS_GET_PRD_IDX (prd, &prdIdx);
   lcm = rgSchCmnSpsLcmTbl[spsPrdIdx][prdIdx];
   cycleCnt = lcm/spsPrd;
   idx = startIdx;

   while (cycleCnt)
   {
      if (offset == idx)
         break;
      idx = (idx + spsPrd) % prd;
      cycleCnt--;
   }
   if (cycleCnt)
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}
#endif /*RG_UNUSED*/

/**
 * @brief Checks collision of allocation in frequency domain
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch
 *
 *
 *     Processing Steps:
 *     - Assumed that this function is called with invalid value of n1Pucch if
 *     n1Pucch needs to be determined by the function.
 *     - Check the collision in frequency domain for the computed sfIdx and
 *     returns allocation mask indicating frequency resources available for UE.
 *     - Check the collision in n1Pucch values and returns the n1PucchMask
 *     indicating n1Pucch available resources.
 *
 *  @param[in]  RgSchCellCb        *cellCb
 *  @param[in]  RgSchUeCb          *ueCb
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @param[out] RgSchDlSfAllocInfo *allocInfo
 *  @param[out] U8                 *n1PucchIdx
 *  @return  Bool
 *             TRUE     Collision detected in n1Pucch resources
 *             FALSE    No collision detected in n1Pucch values
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSfAllocInfo  *allocInfo,
U8                  *n1PucchIdx
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch(cell, ue, timingInfo,
      allocInfo, n1PucchIdx)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     timingInfo;
RgSchDlSfAllocInfo  *allocInfo;
U8                  *n1PucchIdx;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                   sfIdx;
   U16                   cycleCnt;
   U16                   spsPrdIdx;  
   U16                   spsPrd;  
   U16                   n1PucchCnt;
   U32                   n1PucchMask[RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE];
   U8                    maskIdx;
   U8                    maskPos;
   U16                   crntLcm = dlSpsCell->spsPrdLcmVal;

   TRC2(rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch);

   /* Memset the input masks to 0 */
   cmMemset((U8 *)allocInfo, 0, sizeof(*allocInfo));
   cmMemset((U8 *)n1PucchMask, 0, 
         (sizeof(U32) * RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE) );

   /* Initialize the variables */
   spsPrdIdx = dlSpsUe->prdIdx;
   spsPrd    = (U16)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   sfIdx = (timingInfo.sfn * 10 + timingInfo.subframe) % crntLcm;
   cycleCnt = crntLcm/spsPrd; 

   /* Determine the allocation mask and n1Pucch mask for the current offset */
   while(cycleCnt)
   {
      RgSchDlSfAllocInfo *spsSfAlloc = 
         &(dlSpsCell->spsSfTbl[sfIdx].spsAllocInfo);
      for (maskIdx = 0; maskIdx < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++maskIdx)
      {
         allocInfo->raType1Mask[maskIdx] |= spsSfAlloc->raType1Mask[maskIdx];
         allocInfo->raType1UsedRbs[maskIdx] += 
            spsSfAlloc->raType1UsedRbs[maskIdx];
      }
      allocInfo->raType0Mask |= spsSfAlloc->raType0Mask;
      for (maskIdx = 0; maskIdx < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++maskIdx)
      {
         allocInfo->raType2Mask[maskIdx] |= spsSfAlloc->raType2Mask[maskIdx];
      }
      for (n1PucchCnt = 0;n1PucchCnt < RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE;
            ++n1PucchCnt)
      {
         n1PucchMask[n1PucchCnt] |= 
            dlSpsCell->spsSfTbl[sfIdx].n1PucchMask[n1PucchCnt];
      } 
      sfIdx = (sfIdx + spsPrd) % crntLcm;
      --cycleCnt;
   }

   /* Check if n1Pucch value is available */
   if (*n1PucchIdx == RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE + 1)
   {
      for (n1PucchCnt = 0; n1PucchCnt < ue->dl.dlSpsCfg.numPucchVal; 
              ++n1PucchCnt)
      {
         maskIdx = dlSpsUe->n1PucchIdx[n1PucchCnt]/32;
         maskPos = dlSpsUe->n1PucchIdx[n1PucchCnt] % 32;
         if (!((n1PucchMask[maskIdx]) & 
               (1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(maskPos))))
         {
            *n1PucchIdx = (U8)n1PucchCnt;
            break;
         }
      }
   }

   /* If n1PucchIdx is invalid value, no n1Pucch value is available at this
    * offset: Collision in n1Pucch resources detected */
   if (*n1PucchIdx == RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE + 1)
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}


/**
 * @brief Performs resource allocation for an SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeAlloc
 *
 *
 *     Processing Steps:
 *       - Determine the scaled CQI value for the UE.Call
 *       rgSCHCmnSpsDlGetScaledCqi.
 *       - Determine the number of RBs needed for the SPS transmission.
 *       - Determine the allocation and allocation type.
 *       - Determine the dci format for transmission.
 *       - Check if the PDCCH for the UE is available.
 *       - Update allocInfo with information.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchDlSfAllocInfo  *allocInfo
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSf           *dlSf,
RgSchDlSfAllocInfo  *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeAlloc(cell, ue, timingInfo, dlSf, allocInfo)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     timingInfo;
RgSchDlSf           *dlSf;
RgSchDlSfAllocInfo  *allocInfo;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                instIdx  = cell->instIdx;
#endif /*DEBUGP*/
   RgSchCmnDlUe        *cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U8                  scCqi;
   RgSchDlRbAlloc      *rbAllocInfo = &(ue->dl.dlAllocCb);
   U32                 spsBo;
   U16                 rlcHdrEst;

   TRC2(rgSCHCmnSpsDlUeAlloc);

   /* Note: Assumed allocation information for UE is initialized to 0 while
    * memset for ueCb */

   /* Determine the scaled CQI value for SPS transmission */
   scCqi = rgSCHCmnSpsDlGetScaledCqi(ue, cmnUeDl->mimoInfo.cwInfo[0].cqi);

   if (scCqi == RG_SCH_CMN_SPS_DL_INVALIDCQI_VAL)
   {
      scCqi = cmnUeDl->mimoInfo.cwInfo[0].cqi;
   }
   /* Determine number of RBs needed by the UE for SPS allocation */
   spsBo = dlSpsUe->spsSvc->dlLcSpsCfg.packetSize;
   RG_SCH_CMN_DL_GET_HDR_EST(dlSpsUe->spsSvc, rlcHdrEst);
   spsBo += rlcHdrEst;

   rgSCHCmnClcRbAlloc(cell, spsBo, scCqi, &rbAllocInfo->rbsReq, \
         &rbAllocInfo->tbInfo[0].bytesReq,\
         &rbAllocInfo->tbInfo[0].imcs, &rbAllocInfo->tbInfo[0].iTbs, TRUE);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (!rbAllocInfo->rbsReq)
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(instIdx), 
               "Zero RBs needed for SPS UE %d\n", ue->ueId));
      RETVALUE(RFAILED);
   }
#endif

   /* Determine the allocation for the UE */
   rgSCHCmnSpsDlUeAllocRbs(cell, ue, timingInfo, dlSf, allocInfo);
   if (!rbAllocInfo->rbsAlloc)
   {
      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), 
               "RB allocation failed for SPS ue: %d at CQI %d\n", ue->ueId, 
               scCqi));
      RETVALUE(RFAILED);
   }

   /* Determine dciFormat for the UE for SPS transmission */
   rgSCHCmnSpsDlGetDciFmt(ue, rbAllocInfo->raType, 
         &rbAllocInfo->dciFormat);


   /* Update scaled CQI value in rbAllocInfo */
   rbAllocInfo->cqiForTx = scCqi;

   RETVALUE(ROK);
}

/**
 * @brief Checks if UE is active for DL scheduling 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlChkUeInActv
 *
 *
 *     Processing Steps:
 *       - If UE is in HD-FDD mode, check collision with SR, CQI, SRS for the
 *       scheduling time.
 *       - If UE has measurement gap enabled, check collision with measurement
 *       gap.
 *       - If DRX is enabled, check if DL scheduling can be done
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchUeCb             *ueCb
 *  @param[in]  CmLteTimingInfo       timingInfo
 *  @return  Bool
 *             TRUE     inactive
 *             FALSE    active
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlChkUeInActv
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        timingInfo
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsDlChkUeInActv(cell, ue, timingInfo)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
CmLteTimingInfo        timingInfo;
#endif /* ANSI */
{
#ifdef DEBUGP
    Inst inst = cell->instIdx;
#endif
    TRC2(rgSCHCmnSpsDlChkUeInActv);

   /* Call APIs exposed by measurement gap, DRX and HD-FDD module */
   if (ue->measGapCb.isMesGapEnabled)
   {
      if(ue->dl.dlInactvMask)
      {
         RETVALUE(TRUE);
      }
   }
   if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, timingInfo))
   {
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "\n rgSCHCmnSpsDlChkUeInActv: "
               "Ue already scheduled by SPS module %d", ue->ueId));
      RETVALUE(TRUE);
   }

         /*Check if UE is on DRX Active period*/
#ifdef LTEMAC_DRX
    if(ue->isDrxEnabled)
    {
       if(!RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
       {
            RGSCHDBGINFO(inst, (rgSchPBuf(inst), 
            "\n UE %d is inactive : sfn %d subframe %d", 
            ue->ueId, timingInfo.sfn, timingInfo.subframe));

            RETVALUE(TRUE);
        }
    }
#endif
   RETVALUE(FALSE);
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlGetDciFmt
 *
 *
 *     Desc :Determines DCI format based on the resource allocation type for 
 *     SPS UE
 *
 *     Ret  : DCI format
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlGetDciFmt
(
RgSchUeCb    *ue,
U8           raType,
TfuDciFormat *dciFmt
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlGetDciFmt(ue, raType, dciFmt) 
RgSchUeCb    *ue;
U8           raType;
TfuDciFormat *dciFmt;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsDlGetDciFmt);
   *dciFmt =  rgSchCmnSpsRaTypeToDciFrmt[ue->mimoInfo.txMode - 1][raType];
   RETVOID;
}

/**
 * @brief Update allocation mask for SPS with DL SF allocation 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdSpsWithDlSfAlloc
 *
 *
 *     Processing Steps:
 *     
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[out]  RgSchDlSfAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlUpdSpsWithDlSfAlloc
(
RgSchDlSf             *dlSf,
RgSchDlSfAllocInfo    *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlUpdSpsWithDlSfAlloc(dlSf, allocInfo)
RgSchDlSf             *dlSf;
RgSchDlSfAllocInfo    *allocInfo;
#endif /* ANSI */
{
   U8                 idx;

   TRC2(rgSCHCmnSpsDlUpdSpsWithDlSfAlloc);

   /* Update allocation mask with DL SF allocation  */
   for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx)
   {
      allocInfo->raType1Mask[idx] |= dlSf->dlSfAllocInfo.raType1Mask[idx];
      allocInfo->raType1UsedRbs[idx] += 
         dlSf->dlSfAllocInfo.raType1UsedRbs[idx];
   }
   allocInfo->raType0Mask |= dlSf->dlSfAllocInfo.raType0Mask;
   for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; idx++)
   {
      allocInfo->raType2Mask[idx] |= dlSf->dlSfAllocInfo.raType2Mask[idx];
   }

   RETVOID;
}

/**
 * @brief Performs resource allocation for an SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeAllocRbs
 *
 *
 *     Processing Steps:
 *       - Determine allocation for the UE. Use resource allocation type 1 and
 *       0 for allocation within maximum SPS bandwidth.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchDlSfAllocInfo  *allocInfo
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeAllocRbs
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSf           *dlSf,
RgSchDlSfAllocInfo  *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeAllocRbs(cell, ue, timingInfo, dlSf, allocInfo)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     timingInfo;
RgSchDlSf           *dlSf;
RgSchDlSfAllocInfo  *allocInfo;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                instIdx  = cell->instIdx;
#endif /*DEBUGP*/
   U16                 frmIdx;
   U8                  rbgSize = cell->rbgSize;
   U8                  numAllocRbs = 0;
   U8                  numAllocRbgs = 0;
   U8                  rbStart=0;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchDlRbAlloc      *rbAllocInfo = &(ue->dl.dlAllocCb);
   RgSchCmnSpsDlSf     *dlSpsSf = NULLP;
   RgSchBwRbgInfo      *spsRbgInfo = &(cell->spsBwRbgInfo);
   U8                  temprbg;


   TRC2(rgSCHCmnSpsDlUeAllocRbs);

   /* Note: This function atttempts allocation only if RBs sufficient for SPS
    * allocation are available */

   /* Memeset the resAllocInfo to 0 */
   cmMemset((U8 *)&rbAllocInfo->resAllocInfo, 0, sizeof(RgSchDlSfAllocInfo));

   /* Determine the DL SPS sub-frame for which allocation is being done */
   frmIdx = (timingInfo.sfn * 10 + timingInfo.subframe) % 
      dlSpsCell->spsPrdLcmVal;
   dlSpsSf = &dlSpsCell->spsSfTbl[frmIdx];

   /* Update allocMask with DL SF allocation */
   rgSCHCmnSpsDlUpdSpsWithDlSfAlloc(dlSf, allocInfo);

   /* Perform allocation for RA type 0 if rbsReq is multiple of RBG size (also
    * if RBG size = 1) */
   if ((ue->mimoInfo.txMode != RGR_UE_TM_5) && 
         (ue->mimoInfo.txMode != RGR_UE_TM_6))
   {
      if (rbAllocInfo->rbsReq % rbgSize == 0)
      {
         numAllocRbgs = rgSCHCmnDlRaType0Alloc(allocInfo, 
               rbAllocInfo->rbsReq, spsRbgInfo, &numAllocRbs,
               &rbAllocInfo->resAllocInfo, FALSE);
         if (numAllocRbs)
         {
            rbAllocInfo->raType = RG_SCH_CMN_RA_TYPE0;
         }
      }
      /* If no RBS could be allocated, attempt RA TYPE 1 */
      if (!numAllocRbs)
      {
         numAllocRbs = rgSCHCmnDlRaType1Alloc(allocInfo, rbAllocInfo->rbsReq,
               spsRbgInfo, (U8)dlSpsSf->spsAllocInfo.nxtRbgSubset,
               &rbAllocInfo->allocInfo.raType1.rbgSubset,
               &rbAllocInfo->resAllocInfo, FALSE);

         if(numAllocRbs)
         {
            rbAllocInfo->raType = RG_SCH_CMN_RA_TYPE1;
            dlSpsSf->spsAllocInfo.nxtRbgSubset = 
               (rbAllocInfo->allocInfo.raType1.rbgSubset + 1 ) % rbgSize;
         }
      }
   }

   /* RA type 2 allocation for SPS UE */
   if (!numAllocRbs)
   {
      /* RA type 2 needs to be attempted for SPS UE */
      numAllocRbs = rgSCHCmnDlRaType2Alloc(allocInfo,rbAllocInfo->rbsReq,
            spsRbgInfo, &rbStart, &rbAllocInfo->resAllocInfo, FALSE);
      if (numAllocRbs)
      {
         rbAllocInfo->raType = RG_SCH_CMN_RA_TYPE2;
      }
   }
   if (!numAllocRbs)
   {
      RGSCHDBGINFO(instIdx, (rgSchPBuf(instIdx), "\nRB allocation failed for"
               " SPS UE %d", ue->ueId));
      RETVALUE(RFAILED);
   }


   /* Update rbAllocInfo with the allocation information */
   rbAllocInfo->rnti = ue->spsRnti;

   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
      rbAllocInfo->allocInfo.raType0.dlAllocBitMask = 
         rbAllocInfo->resAllocInfo.raType0Mask;
      rbAllocInfo->allocInfo.raType0.numDlAlloc = numAllocRbgs;
   }
   else if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE1)
   {
      temprbg = rbAllocInfo->allocInfo.raType1.rbgSubset;

      rbAllocInfo->allocInfo.raType1.dlAllocBitMask =
       rbAllocInfo->resAllocInfo.raType1Mask[temprbg];

      rbAllocInfo->allocInfo.raType1.numDlAlloc = numAllocRbs;
      /* Note: shift currently set to 0 since SPS BW is not more than half of
       * total BW, need to determine based on the allocations */
      rbAllocInfo->allocInfo.raType1.shift = 0;
   }
   else if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE2)
   {
      rbAllocInfo->allocInfo.raType2.isLocal = TRUE;
      rbAllocInfo->allocInfo.raType2.rbStart = rbStart;
      rbAllocInfo->allocInfo.raType2.numRb = numAllocRbs;
   }

   rbAllocInfo->rbsAlloc = numAllocRbs;
   rbAllocInfo->tbInfo[0].bytesAlloc = rbAllocInfo->tbInfo[0].bytesReq;
   rbAllocInfo->tbInfo[0].schdlngForTb = TRUE;


   RETVALUE(ROK);
}


/**
 * @brief Performs allocation finalization for active UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeAllocFnlz
 *
 *
 *     Processing Steps:
 *      - Get SPS harq Proc. Call rgSCHDhmSpsDlGetHqProc()
 *        - If HqP not available
 *          - return ROK
 *      - Update DHM with logical channel's data information
 *      - Update the HARQ proc with the allocation information
 *      - Set hqP->isSpsActv = TRUE
 *      - Add HARQ proc to the subframe.
 *      - If successful, 
 *        - if (ue->actionPndng | RG_SCH_CMN_SPS_DL_REACTV)
 *        {
 *          - Call rgSCHCmnSpsDlUeClearAlloc
 *        }
 *        - Update UE's new allocation information in cell-wide sub-frames.
 *          Call rgSCHCmnSpsDlUpdSfOnAlloc.
 *        - Update UE's new allocation information in dlSpsUe.
 *          Call rgSCHCmnSpsDlUpdUeOnAlloc.
 *        - Mark HARQ proc as activation/reactivation HARQ proc 
 *           - hqP->spsAction = ue->actionPndng
 *        - ue->actionPndng = 0
 *        - Remove UE from toBeActvtdUes list
 *        - Remove SPS service from toBeSchdSvcs list 
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo schdTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              n1PucchIdx
 *  @param[in]  U8              isActvOrReactv
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeAllocFnlz
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
RgSchDlSf           *dlSf,
U8                  n1PucchIdx,
U8                  isActvOrReactv
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeAllocFnlz(cell, ue, schdTime, dlSf, n1PucchIdx,
      isActvOrReactv)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     schdTime;
RgSchDlSf           *dlSf;
U8                  n1PucchIdx;
U8                  isActvOrReactv;
#endif /* ANSI */
{
   RgSchCmnDlHqProc        *cmnHqDl;
   RgSchDlHqProcCb         *spsProc;
   RgSchCmnDlUe            *ueDl;
#ifdef DEBUGP
   Inst                    inst = cell->instIdx;
#endif /*DEBUGP*/
   RgSchLchAllocInfo       lchSchdData;
   RgSchDlRbAlloc          *rbAllocInfo = &ue->dl.dlAllocCb;
   RgSchCmnDlCellSpsInfo   *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo     *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlSvcSpsInfo    *dlSpsSvc;
   U32                     y;

   TRC2(rgSCHCmnSpsDlUeAllocFnlz);
   /* Retrieve PDCCH */
   rbAllocInfo->pdcch = NULLP;
   if ((isActvOrReactv) ||
         ((!isActvOrReactv) && (dlSpsUe->dlSpsUeSchdInfo.pdcchPndng)))
   {
      y = rgSCHUtlGetYsps(ue, dlSf->sfNum);
      if (!(rbAllocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, 
                  rbAllocInfo->cqiForTx, rbAllocInfo->dciFormat, FALSE)))
      {
         RGSCHDBGINFO(inst, (rgSchPBuf(inst), 
                  "PDCCH exhausted for SPS ue: %d, cqi: %d\n",
                  ue->ueId, rbAllocInfo->cqiForTx))
         RETVALUE(RFAILED);
      }
   }

   /* Fetch SPS HARQ proc for activation */
   if (!(spsProc = rgSCHDhmSpsDlGetHqProc(ue, schdTime)))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "\nrgSCHCmnSpsDlProcToBeActvtdUe: "
               "No SPS Hq Proc Avl."));
      if (rbAllocInfo->pdcch)
      {
         rgSCHUtlPdcchPut(cell, &dlSf->pdcchInfo, rbAllocInfo->pdcch);
         rbAllocInfo->pdcch = NULLP;
      }
      RETVALUE(RFAILED);
   }

   /* If the UE is been re-activated, clear the old allocation for the UE */
   if ((isActvOrReactv) && (dlSpsUe->actionPndng & RG_SCH_CMN_SPS_DL_REACTV))
   {
      rgSCHCmnSpsDlUeClearAlloc(cell, ue);
   }

   /* Update UE allocation in cell-wide subframes: replicate the allocation across
    * sub-frames, if activation/re-activation */
   if (isActvOrReactv)
   {
      rgSCHCmnSpsDlUpdSfOnAlloc(cell, ue, schdTime, dlSf, n1PucchIdx);
   }

   /* Update allocation information in dlSpsUe */
   rgSCHCmnSpsDlUpdUeOnAlloc(cell, ue, schdTime, n1PucchIdx);

   /* Update values in the hqProc */
   /* Note: SPS Service data shall not be multiplexed with any other lc Data */
   if (!rbAllocInfo->pdcch)
   {
      /* Allocation without PDCCH, set the n1Pucch resource */
      U8 n1PucchIdxVal = (U8)dlSpsUe->n1PucchIdx[n1PucchIdx];

      spsProc->spsN1PucchRes.pres = TRUE;
      spsProc->spsN1PucchRes.val =
         dlSpsCell->n1PucchDb.n1PucchLst[n1PucchIdxVal].n1PucchVal;
   }
   cmnHqDl = RG_SCH_CMN_GET_DL_HQP(spsProc);
   ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   ueDl->proc = spsProc;
   cmnHqDl->totBytes = rbAllocInfo->tbInfo[0].bytesAlloc;
   cmnHqDl->isSpsSvcSchd = TRUE;
   cmnHqDl->isSpsActv = TRUE;
   cmnHqDl->spsAction = dlSpsUe->actionPndng; 
   rbAllocInfo->tbInfo[0].tbCb = &spsProc->tbInfo[0];
   rbAllocInfo->tbInfo[0].noLyr = 1;
   rbAllocInfo->dlSf = dlSf;


   /* Update maximum re-transmission time for this SPS HARQ proc */
   RGSCHADDTOCRNTTIME(schdTime, cmnHqDl->maxRetxTime , 
         (ue->dl.dlSpsCfg.dlSpsPrdctyEnum * ue->dl.dlSpsCfg.numSpsHqProc));

   /* Update DHM for the sps service*/
   lchSchdData.lcId     = dlSpsUe->spsSvc->lcId;
   lchSchdData.schdData = rbAllocInfo->tbInfo[0].bytesAlloc;
   rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, &spsProc->tbInfo[0]);

   /* Update effSpsBo appropriately */
   /* Note: Assumed that bytes alloc shall always be greater than packet size*/
   dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);
   dlSpsSvc->effSpsBo = 
      (dlSpsSvc->effSpsBo > rbAllocInfo->tbInfo[0].bytesAlloc) ?
      (dlSpsSvc->effSpsBo - dlSpsUe->spsSvc->dlLcSpsCfg.packetSize) : 0;

   if (rbAllocInfo->pdcch)
   {
      /* Fill Activation/re-activation PDCCH and add the HARQ proc to sub-frame
       * for transmission */
      rgSCHCmnSpsDlFillActvPdcch(cell, ue, rbAllocInfo->pdcch, FALSE, 0);
   }
   else
   {
      /* Fill up the TB of HARQ process */
      /* Add the HARQ process to the sub-frame */

      rgSCHCmnSpsDlFillActvPdcch(cell, ue, &ue->dl.spsOccPdcch, TRUE,
            spsProc->procId);
   }

   /* Reset UE but update spsSchdTime correctly */
   rgSCHCmnDlUeResetTemp(ue, spsProc);
   rbAllocInfo->spsSchdTime = schdTime;

   RETVALUE(ROK);

}

/**
 * @brief Clear allocation of a particular SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeClearAlloc
 *
 *
 *     Processing Steps:
 *     - Remove UE from the old index in actvDlSpsUeLsts
 *     - Remove UE's old allocation from the subframes
 *     - Remove n1Pucch allocation corresponding to the UE from the
 *       subframes
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo schdTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              n1PucchIdx
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeClearAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeClearAlloc(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                   lcmVal = dlSpsCell->spsPrdLcmVal;
   U16                   dlSpsSfIdx;
   U16                   repCnt; 
   U16                   sfIdx; 
   U16                   idx, idx1;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   U32                   spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   RgSchCmnSpsDlSf       *dlSpsSfArr = NULLP;
   RgSchDlRbAlloc        *spsAllocInfo = &dlSpsUeSchdInfo->spsAllocInfo;
   RgSchDlSfAllocInfo    *sfResAllocInfo;
   U8                    maskIdx;
   U8                    maskPos;
   U16                   n1PucchIdxVal;
   RgSchCmnSpsDlN1Pucch  *n1PucchCb;
   U32                   n1PucchMask;

   TRC2(rgSCHCmnSpsDlUeClearAlloc);

   /* Compute n1Pucch mask idx and mask position */
   n1PucchIdxVal = dlSpsUe->n1PucchIdx[dlSpsUeSchdInfo->allocN1PucchIdx];
   maskIdx = n1PucchIdxVal/32;
   maskPos = n1PucchIdxVal % 32;
   n1PucchMask = 1 << (31 - maskPos);
   n1PucchCb = &dlSpsCell->n1PucchDb.n1PucchLst[n1PucchIdxVal];

   /* Compute the indices to be updated */
   dlSpsSfIdx = dlSpsUeSchdInfo->actvSfTblIdx;
   dlSpsSfArr = dlSpsCell->spsSfTbl;
   repCnt =  lcmVal/spsPrd;

   sfIdx = dlSpsSfIdx;

   /* Clear allocation information from SPS sub-frames occuring at the UE's
    * periodicity */
   for (idx = 0; idx < repCnt; ++idx)
   {
      sfResAllocInfo = &(dlSpsSfArr[sfIdx].spsAllocInfo);
      sfResAllocInfo->raType0Mask ^=
         spsAllocInfo->resAllocInfo.raType0Mask;
      for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx1)
      {
         sfResAllocInfo->raType1Mask[idx1] ^= 
            spsAllocInfo->resAllocInfo.raType1Mask[idx1];
         sfResAllocInfo->raType1UsedRbs[idx1] -= 
            spsAllocInfo->resAllocInfo.raType1UsedRbs[idx1];
      }
      for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx1)
      {
         sfResAllocInfo->raType2Mask[idx1] ^= 
            spsAllocInfo->resAllocInfo.raType2Mask[idx1];
      }
      dlSpsSfArr[sfIdx].n1PucchMask[maskIdx] ^= n1PucchMask;
      dlSpsSfArr[sfIdx].rbsAlloc -= spsAllocInfo->rbsAlloc;
      sfIdx = (sfIdx + spsPrd) % lcmVal;
   }

   /* Remove UE from active UE list */
   rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, dlSpsUeSchdInfo->schdKey);

   /* Free reserved n1Pucch value */
   rgSCHCmnSpsDlFreeN1Pucch(&dlSpsCell->n1PucchDb, n1PucchCb);

   /* Clear the scheduling information of the UE */
   cmMemset((U8 *)(dlSpsUeSchdInfo), 0, sizeof(*dlSpsUeSchdInfo));

   /* Mark UE to have invalid SPS scheduling information */
   dlSpsUe->isSpsActv = FALSE;
   ue->dl.isSpsHqPInUse = FALSE;

   RETVALUE(ROK);
}

/**
 * @brief Update allocation of a UE in the cell-wide subframes.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdSfOnAlloc
 *
 *
 *     Processing Steps:
 *        - Replicate the n1Pucch and frequency domain information for the SPS
 *          periodicity over the sub-frames
 *        - Mark UE as ue->isSpsActv = TRUE
 *        - Add UE to actvDlSpsUeLsts at computed index.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo schdTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              n1PucchIdx
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUpdSfOnAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
RgSchDlSf           *dlSf,
U8                  n1PucchIdx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUpdSfOnAlloc(cell, ue, schdTime, dlSf, n1PucchIdx)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     schdTime;
RgSchDlSf           *dlSf;
U8                  n1PucchIdx;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                   lcmVal = dlSpsCell->spsPrdLcmVal;
   U16                   dlSpsSfIdx;
   U16                   repCnt; 
   U16                   sfIdx; 
   U16                   idx, idx1;
   U32                   spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   RgSchCmnSpsDlSf       *dlSpsSfArr = NULLP;
   RgSchDlRbAlloc        *rbAllocInfo = &ue->dl.dlAllocCb;
   U8                    maskIdx;
   U8                    maskPos;
   U16                   n1PucchIdxVal;
   U32                   n1PucchMask;
   RgSchDlSfAllocInfo    *sfResAllocInfo;

   TRC2(rgSCHCmnSpsDlUpdSfOnAlloc);

   /* Compute n1Pucch mask idx and mask position */
   n1PucchIdxVal = dlSpsUe->n1PucchIdx[n1PucchIdx];
   maskIdx = n1PucchIdxVal/32;
   maskPos = n1PucchIdxVal % 32;
   n1PucchMask = 1 << (31 - maskPos);

   /* Compute the indices to be updated */
   dlSpsSfIdx = (schdTime.sfn * 10 + schdTime.subframe) % lcmVal;
   dlSpsSfArr = dlSpsCell->spsSfTbl;
   repCnt =  lcmVal/spsPrd;

   sfIdx = dlSpsSfIdx; 
   for (idx = 0; idx < repCnt; ++idx)
   {
      sfResAllocInfo = &(dlSpsSfArr[sfIdx].spsAllocInfo);
      sfResAllocInfo->raType0Mask |=
         rbAllocInfo->resAllocInfo.raType0Mask;
      for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx1)
      {
         sfResAllocInfo->raType1Mask[idx1] |= 
            rbAllocInfo->resAllocInfo.raType1Mask[idx1];
         sfResAllocInfo->raType1UsedRbs[idx1] += 
            rbAllocInfo->resAllocInfo.raType1UsedRbs[idx1];
      }
      for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx1)
      {
         sfResAllocInfo->raType2Mask[idx1] |= 
            rbAllocInfo->resAllocInfo.raType2Mask[idx1];
      }
      dlSpsSfArr[sfIdx].n1PucchMask[maskIdx] |= n1PucchMask;
      dlSpsSfArr[sfIdx].rbsAlloc += rbAllocInfo->rbsAlloc;
      sfIdx = (sfIdx + spsPrd) % lcmVal;
   }

   RETVALUE(ROK);
}

/**
 * @brief Update scheduling information in DL SPS UE on allocation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdUeOnAlloc
 *
 *
 *     Processing Steps:
 *     - Update SPS scheduling information in the UE from  rbAllocInfo
 *     - Mark UE as ue->isSpsActv = TRUE
 *     - Add UE to actvDlSpsUeLsts at computed index.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo schdTime
 *  @param[in]  U8              n1PucchIdx
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUpdUeOnAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
U8                  n1PucchIdx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUpdUeOnAlloc(cell, ue, schdTime, n1PucchIdx)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     schdTime;
U8                  n1PucchIdx;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                   dlSpsSfIdx;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   U32                   spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   RgSchDlRbAlloc        *rbAllocInfo = &ue->dl.dlAllocCb;
   U16                   lcmVal = dlSpsCell->spsPrdLcmVal;

   TRC2(rgSCHCmnSpsDlUpdUeOnAlloc);

   /* Compute the indices to be updated */
   dlSpsSfIdx = (schdTime.sfn * 10 + schdTime.subframe) % lcmVal;

   /* Update the scheduling information in the UE */
   dlSpsUeSchdInfo->spsAllocInfo = *rbAllocInfo;
   dlSpsUeSchdInfo->allocN1PucchIdx = n1PucchIdx; 
   dlSpsUeSchdInfo->actvSfTblIdx =  dlSpsSfIdx;

   /* If UE exists in active list, remove from current index */
   if (dlSpsUe->actvUeLstEnt.node)
   {
      rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, dlSpsUeSchdInfo->schdKey);
   }

   /* Add UE to the active list of UEs at new index */
   RGSCHADDTOCRNTTIME(schdTime, dlSpsUeSchdInfo->schdKey , spsPrd);
   rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, dlSpsUeSchdInfo->schdKey);

   /* Update scaledCqi and pdcchPndg variable */
   dlSpsUeSchdInfo->scaledCqi = rbAllocInfo->cqiForTx;
   dlSpsUeSchdInfo->pdcchPndng = FALSE;

   /* Mark UE to have valid SPS scheduling information */
   dlSpsUe->isSpsActv = TRUE;
   ue->dl.isSpsHqPInUse = TRUE;

   RETVALUE(ROK);
}

/**
 * @brief Handles services to be scheduled at non-SPS ocassions
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcToBeSchdSvcs
 *
 *
 *     Processing Steps:
 *     - For each service in the toBeSchdSvcs,
 *      - Schedule the service using CRNTI
 *      - Call rgSCHCmnSpsDlAllocForSvc()
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  CmLteTimingInfo crntTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlProcToBeSchdSvcs
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlProcToBeSchdSvcs(cell, crntTime, dlSf, allocInfo)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchDlLcCb           *dlLcCb;
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc;
   CmLList               *schdSvcNode;
   CmLteTimingInfo       schdTime;

   TRC2(rgSCHCmnSpsDlProcToBeSchdSvcs);

   schdTime = crntTime;
   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);

   dlSpsCell->toBeSchdSvcs.crnt = NULLP;
   CM_LLIST_NEXT_NODE(&dlSpsCell->toBeSchdSvcs, schdSvcNode);

   /* Note: Currently, all pending services are attempted to be scheduled */
   while (schdSvcNode)
   {
      dlLcCb = (RgSchDlLcCb *)schdSvcNode->node;
      dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlLcCb);
      /* rg003.301[ccpu00122038]-MOD- Check for return value */
      if (rgSCHCmnSpsDlAllocForSvc(cell, schdTime, dlSf, dlLcCb,
            dlSpsSvc->effSpsBo, allocInfo) != ROK)
      {
         /* Bandwidth expired */
         RETVOID;
      }
      CM_LLIST_NEXT_NODE(&dlSpsCell->toBeSchdSvcs, schdSvcNode);
   }

   RETVOID;
}

/**
 * @brief Handles to be released SPS UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcToBeRelUes
 *
 *
 *     Processing Steps:
 *     -For  UE in the list of toBeRelUes,
 *       - Fetch PDCCH for the UE
 *       - If successful, 
 *          - Set the bits according to the one needed for releasing
 *          - Update the feedback timing information in the PDCCH
 *          - Add PDCCH to the subframe to the relPdcchs list 
 *          - set ue->isRelPdcchSent = TRUE
 *        else
 *          - Move UE to the end of the queue
 *
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @parama[in] CmLteTimingInfo       crntTime
 *  @param[in]  RgSchDlSf             *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlProcToBeRelUes
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlProcToBeRelUes(cell, crntTime, dlSf)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   RgSchCmnDlUe          *cmnUeDl = NULLP;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   CmLList               *listNode  = dlSpsCell->toBeRelUes.first;
   RgSchUeCb             *ue;
   CmLteTimingInfo       schdTime;
   U8                    scCqi;
   U32                   y;
   RgSchPdcch            *relPdcch;
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
#ifdef DEBUGP
   Inst                   inst  = cell->instIdx; 
#endif
   /*rg002.301 ccpu00119494-ADD-added handling to retransmit
    * RelPdcch in case no feedback is received */
   CmLteTimingInfo       relFdbkTime = {0};
   
   TRC2(rgSCHCmnSpsDlProcToBeRelUes);
   
   schdTime = crntTime;
   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);

   while (listNode)
   {
      ue       = (RgSchUeCb *)(listNode->node);
      cmnUeDl  = RG_SCH_CMN_GET_DL_UE(ue);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);

      if (!dlSpsUe->isRelPdcchSent)
      {
         /* Check if UE can be scheduled in DL */
         if (rgSCHCmnSpsDlChkUeInActv(cell, ue, schdTime))
         {
            
            listNode = listNode->next;
             RGSCHDBGINFO(inst, (rgSchPBuf(inst), 
                     "\n UE %d inactive for release: sfn %d subframe %d", 
                     ue->ueId, schdTime.sfn, schdTime.subframe));
            continue;
         }

         /* Determine the scaled CQI value for SPS transmission */
         scCqi = 
             rgSCHCmnSpsDlGetScaledCqi(ue, cmnUeDl->mimoInfo.cwInfo[0].cqi);
            
         if (scCqi == RG_SCH_CMN_SPS_DL_INVALIDCQI_VAL)
         {
            scCqi = cmnUeDl->mimoInfo.cwInfo[0].cqi;
         }
         /* Retrieve PDCCH using DCI format 1A */
         y = rgSCHUtlGetYsps(ue, dlSf->sfNum);
         if (!(relPdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, scCqi,\
                     TFU_DCI_FORMAT_1A, FALSE)))
         {
            RGSCHDBGINFO(inst, (rgSchPBuf(inst), 
                     "PDCCH exhausted for SPS ue: %d, cqi: %d\n",
                     ue->ueId, scCqi));
           
            listNode = listNode->next;
            continue;
         }

#ifdef LTE_TDD
         /* Update the transmission time of relPdcch */
         ue->relPdcchTxTime = schdTime;
#endif /*LTE_TDD*/

         /* Update the expected feedback timing for release PDCCH */
#ifndef LTE_TDD
         RGSCHADDTOCRNTTIME(schdTime, ue->relPdcchFbkTiming, 
               RG_SCH_CMN_HARQ_INTERVAL);
#else
         /* Doing equivalent of what is done in rgSCHCmnUpdHqAndDai() */
          /* for timingInfo in tbCb, equivalent of which should be use in place
          * of 'schdTime' below. */
         ue->relPdcchFbkTiming.sfn = (schdTime.sfn + \
               dlSf->dlFdbkInfo.sfnOffset) % RGSCH_MAX_SFN;
         ue->relPdcchFbkTiming.subframe = dlSf->dlFdbkInfo.subframe;
         ue->relPdcchValm = dlSf->dlFdbkInfo.m;
         rgSCHCmnUpdDai(ue, &ue->relPdcchFbkTiming, ue->relPdcchValm, TRUE);
#endif

         /*rg002.301 ccpu00119494-ADD-added handling to retransmit RelPdcch 
          * incase no feedback is received */
         
         /* UE is added to wtngForRelFdbkUeLst at 
          * expected fdb time + TFU_HQFBKIND_ULDELTA tti.
          * At fdb+HQ_IND_DELTA  tti if the UE still
          * exists in wtngForRelFdbkUeLst, we know for sure
          * UE has not received any feedback*/
         RGSCHADDTOCRNTTIME(ue->relPdcchFbkTiming,relFdbkTime,
                            TFU_HQFBKIND_ULDELTA ); 
         cmLListAdd2Tail(&(dlSpsCell->wtngForRelFdbkUeLst[relFdbkTime.subframe]),
                         &(dlSpsUe->wtngForRelFdbkUeEnt));
         dlSpsUe->wtngForRelFdbkUeEnt.node = (PTR)ue;

         /* Note: Add UE's normal RNTI so that the feedback for the same  is
          * recieved with normal RNTI itself and UE can be fetched using the
          * same */
         /* Fill up release PDCCH info */
         rgSCHCmnSpsDlFillRelPdcch(ue, relPdcch);

         /* Note: Currently no list maintained in dlSf since only one release 
          * PDCCH is scheduled per sub-frame. Add release PDCCH to the subframe */
         dlSf->relPdcch                 = relPdcch;
#ifdef LTE_TDD
         dlSf->relPdcchInfo.crnti       = ue->ueId;
         dlSf->relPdcchInfo.ackNackMode = ue->dl.ackNackMode;
#endif /*LTE_TDD*/

         /* Mark UE to have release PDCCH sent */
         dlSpsUe->isRelPdcchSent = TRUE;
         dlSpsUe->numRelPdcchSent++;

         /* Mark UE to be scheduled by SPS */
         ue->dl.dlAllocCb.spsSchdTime = schdTime;

         /* Breaking from the loop since only 1 Release PDCCH to be scheduled per
          * sub-frame */
         break;
      }
      listNode = listNode->next;
   }

   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested. 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsDlAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested. 
 *               1. Loop through scheduled TX and RETX lists.
 *                  Fill in the corresponding PDCCH and HqProcs.
 *                  Remove the satisfied services from the toBeSchdSvcs list
 *                  Remove the satisfied retx Hq Procs from retxHqProcs list
 *               2. Loop through UnScheduled RETX Lists.
 *                  Release grabbed HqProcs.
 *               
 *     
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif /*DEBUGP*/
   TRC2(rgSCHCmnSpsDlAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHCmnSpsDlAllocFnlz\n"));

   
   rgSCHCmnSpsDlRetxAllocFnlz(cell, allocInfo);

   /* Tx Allocations would include allocation for SPS service during 
    * non-SPS ocassions  and SPS ocassions ( when BO does not satisfy packet
    * size constraints) */
   rgSCHCmnSpsDlTxAllocFnlz(cell, allocInfo);


   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX allocations. 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsDlRetxAllocFnlz 
 *     Purpose : Remove the Retx Hq Proc from the Cell's
 *     Retx list, if allocation is successful.
 *     Fill the HqProc and PDCCH and append it to the SubFrm.
 *     
 *               
 *     
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlRetxAllocFnlz 
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlRetxAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   CmLList          *node;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchUeCb        *ue;
   RgSchCmnDlUe     *ueDl;
   RgSchDlHqProcCb  *hqP;
   RgSchCmnDlHqProc *cmnHqDl;

#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif /*DEBUGP*/
   TRC2(rgSCHCmnSpsDlRetxAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHCmnSpsDlRetxAllocFnlz\n"));

   node = allocInfo->dedAlloc.schdRetxSpsUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      node = node->next;
      ueDl = RG_SCH_CMN_GET_DL_UE(ue);
      /* Fill PDCCH and assign it to HqP */
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
      if (cmnHqDl->isSpsActv)
      {
         /* If HARQ proc used for SPS ocassion, set NDI = 1, use SPS rnti for
          * retx */
         ue->dl.dlAllocCb.tbInfo[0].tbCb->ndi = 1;
         ue->dl.dlAllocCb.rnti = ue->spsRnti;
      }
      rgSCHCmnFillHqPPdcch(cell, &ue->dl.dlAllocCb, hqP);
      /* Extra Check: Retain the hqProc in the RETX Queue if one/more
       * TBs of the HqProc are yet to be scheduled for RETX.
       * Note: Here we are not tracking at TB Level, the priority Q
       * to which it belongs to. The retx prio of transmission is still
       * being maintained at hqProc level, rather than at TB level */
      if ((hqP->tbInfo[0].state != HQ_TB_NACKED) && 
          (hqP->tbInfo[1].state != HQ_TB_NACKED)) {
         rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP);
      }
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxUeLst and reset the UE allocation Info */
   node = allocInfo->dedAlloc.nonSchdRetxSpsUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      ueDl = RG_SCH_CMN_GET_DL_UE(ue);
      node = node->next;
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        New TX allocations for SPS services. 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsDlTxAllocFnlz 
 *     Purpose :
 *     Fill pdcch and HqP for UEs will allocations.
 *     Release HqP for UE with no allocation.
 *
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlTxAllocFnlz 
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlTxAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   CmLList          *node;
   RgSchUeCb        *ue;
   RgSchDlHqProcCb  *hqP;
   RgSchDlRbAlloc   *ueAllocInfo; 
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif /*DEBUGP*/
   TRC2(rgSCHCmnSpsDlTxAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHCmnSpsDlTxAllocFnlz\n"));

   node = allocInfo->dedAlloc.schdTxSpsUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      node = node->next;
      ueAllocInfo = &ue->dl.dlAllocCb;
      rgSCHCmnSpsDlNewTxUeFnlz(cell, ueAllocInfo, ue);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxUeLst and reset the UE allocation Info */
   node = allocInfo->dedAlloc.nonSchdTxSpsUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      node = node->next;

      /* Release HqProc */
      /* rg001.301 -MOD-ccpu00118350 : Correcting NDI manipulation of Harq */
      rgSCHDhmRlsHqpTb(hqP, 0, FALSE);
      rgSCHDhmRlsHqpTb(hqP, 1, FALSE);
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlNewTxUeFnlz 
 *        
 *     Desc : Perform UE finalization for allocated bytes.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlNewTxUeFnlz 
(
RgSchCellCb    *cell,
RgSchDlRbAlloc *ueAllocInfo, 
RgSchUeCb      *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlNewTxUeFnlz(cell, ueAllocInfo, ue)
RgSchCellCb    *cell;
RgSchDlRbAlloc *ueAllocInfo;
RgSchUeCb      *ue;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUe        *cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U32                 bytesAlloc = ueAllocInfo->tbInfo[0].bytesAlloc;
   RgSchCmnDlSvc       *spsSvcCmn = RG_SCH_CMN_GET_DL_SVC(dlSpsUe->spsSvc);
   U32                 remBytes;
   U32                 effBo = spsSvcCmn->dlSvcSpsInfo.effSpsBo;
   RgSchLchAllocInfo   lchSchdData;

   TRC2(rgSCHCmnSpsDlNewTxUeFnlz);

   /* Handle SPS service */
   if (bytesAlloc)
   {
      /* Determine how much BO is satisfied */
      if (bytesAlloc <= spsSvcCmn->dlSvcSpsInfo.hdrEst)
      {
         /* Allocation such that Nothing could be satisfied */
         /* Return the grabbed PDCCH */
         rgSCHUtlPdcchPut(cell, &ueAllocInfo->dlSf->pdcchInfo, 
               ueAllocInfo->pdcch);
         /* rg001.301 -MOD-ccpu00118350 : Correcting NDI manipulation of Harq */
         rgSCHDhmRlsHqpTb(cmnUeDl->proc, 0, FALSE);
         rgSCHDhmRlsHqpTb(cmnUeDl->proc, 1, FALSE);
         RETVOID;
      }
      remBytes = bytesAlloc - spsSvcCmn->dlSvcSpsInfo.hdrEst;
      effBo =  effBo > remBytes ? (effBo - remBytes) : 0;
      if (!effBo)
      {
         /* Remove UE from toBeSchdSvcs list, if present */
         if (spsSvcCmn->dlSvcSpsInfo.toBeSchdSvcEnt.node)
         {
            rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
         }
      }
      spsSvcCmn->dlSvcSpsInfo.effSpsBo = effBo;

      /* Update DHM for this SVC */
      lchSchdData.lcId     = dlSpsUe->spsSvc->lcId;
      lchSchdData.schdData = bytesAlloc;
      rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, 
            &(cmnUeDl->proc->tbInfo[0]));
      /* Fill PDCCH and assign it to HqP */
      rgSCHCmnFillHqPPdcch(cell, ueAllocInfo, cmnUeDl->proc);
   }

   RETVOID;
}

/**
 * @brief Handles allocation for an SPS service of a UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlAllocForSvc
 *
 *
 *     Processing Steps:
 *     - Get non-SPS harq Proc. Call rgSCHDhmGetAvlHqProc()
 *     - Determine header size estimate for the service.
 *     - Determine RB requirement for the service's BO. Call rgSCHCmnDlAllocRb()
 *     - Update the HARQ proc with the allocation
 *     - Add UE to the txSpsUeLst in allocInfo structure. 
 *       Call rgSCHCmnDlRbInfoAddSpsUeTx().
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  CmLteTimingInfo        schdTime
 *  @param[in]  RgSchDlSf              *subFrm
 *  @param[in]  RgSchDlLcCb            *svc
 *  @param[in]  U32                    bo
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  S16 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlAllocForSvc
(
RgSchCellCb                *cell,
CmLteTimingInfo            schdTime,
RgSchDlSf                  *subFrm,
RgSchDlLcCb                *svc,
U32                        bo,
RgSchCmnDlRbAllocInfo      *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlAllocForSvc(cell, schdTime, subFrm, svc, bo,
      allocInfo)
RgSchCellCb                *cell;
CmLteTimingInfo            schdTime;
RgSchDlSf                  *subFrm;
RgSchDlLcCb                *svc;
U32                        bo;
RgSchCmnDlRbAllocInfo      *allocInfo;
#endif /* ANSI */
{
   RgSchUeCb               *ue;
   RgSchDlHqProcCb         *proc;
   U16                     rlcHdrEstmt;
   U32                     effBo;
   RgSchCmnDlCell          *cmnCellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnDlSvc           *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
   RgSchCmnDlUe            *ueDl;
   RgSchCmnDlHqProc        *cmnHqDl;
#ifdef DEBUGP
   Inst                    inst = cell->instIdx;
#endif /*DEBUGP*/
   S16                     ret;
   TRC2(rgSCHCmnSpsDlAllocForSvc);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHCmnSpsDlAllocForSvc\n"));

   /* Get the UE to which this service belongs to */
   ue = svc->ue;
   ueDl = RG_SCH_CMN_GET_DL_UE(ue);

   /* Check if this UE is already scheduled for transmission/
    * active to be scheduled */
   if (rgSCHCmnSpsDlChkUeInActv(cell, ue, schdTime))
   {
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), 
               "\n UE %d inactive at SPS ocassion: sfn %d subframe %d", 
               ue->ueId, schdTime.sfn, schdTime.subframe));
      RETVALUE(ROK);
   }

   if (rgSCHDhmGetAvlHqProc(ue, cmnCellDl->time, &proc) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "\nrgSCHCmnSpsDlAllocForSvc: "
               "No Hq Proc Avl."));
      RETVALUE(ROK);
   }
   proc->spsN1PucchRes.pres = FALSE;
   cmnHqDl = RG_SCH_CMN_GET_DL_HQP(proc);
   cmnHqDl->isSpsSvcSchd = TRUE;
   cmnHqDl->isSpsActv = FALSE;
   cmnHqDl->spsAction = 0;

   /* Including each SDU's header size */
   RG_SCH_CMN_DL_GET_HDR_EST(svc, rlcHdrEstmt);
   bo += rlcHdrEstmt;
   effBo = 0;
   /* rg003.301[ccpu00122038]-MOD- Check for return value */
   ret = rgSCHCmnDlAllocTxRb(cell, subFrm, ue, bo, &effBo, proc, allocInfo); 
   /* rg003.301[ccpu00122038]-DEL- cmnHqDl->isSpsSvcSchd */
   if ((ret != ROK) || (effBo == 0))
   {
      /* If no allocations so far, meaning proc obtained now */
      if (cmnHqDl->totBytes == 0)
      {
         /* rg001.301 -MOD-ccpu00118350 : Correcting NDI manipulation of Harq */
         rgSCHDhmRlsHqpTb(proc, 0, FALSE);
         rgSCHDhmRlsHqpTb(proc, 1, FALSE);
         /* rg003.301[ccpu00122038]-ADD- initialize the flag to false */
         cmnHqDl->isSpsSvcSchd = FALSE;
      }
      /* rg003.301[ccpu00122038]-MOD- Return the value returned by DlAllocTxRb */
      RETVALUE(ret);
   }

   ueDl->proc = proc;
   cmnHqDl->totBytes = effBo;
   svcCmn->dlSvcSpsInfo.bytesReq = effBo;
   svcCmn->dlSvcSpsInfo.hdrEst = (U8)rlcHdrEstmt;
   ue->dl.dlAllocCb.spsSchdTime = schdTime;

   rgSCHCmnSpsUtlDlAdd2TxSpsUeLst(allocInfo, proc);
   RETVALUE(ROK);
}

/**
 * @brief Validates the UE configuration for SPS UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsVldtDlCellCfg
 *
 *
 *     Processing Steps:
 *      - Validate the configuration with respect to SPS.
 *      - Validate that SPS BW <= 50% total DL BW
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgrCellCfg    *cellCfg
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsVldtDlCellCfg 
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsVldtDlCellCfg (cell, cellCfg) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsVldtDlCellCfg);
   
   UNUSED(cell);
   UNUSED(cellCfg);

   RETVALUE(ROK);
}

/**
 * @brief Validates the UE configuration for SPS UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsVldtDlUeCfg
 *
 *
 *     Processing Steps:
 *      - Validate the configuration with respect to SPS.
 *      - If DL SPS is eanbled, 
 *        - If ack-nack Repetion is enabled, return RFAILED.
 *        - If measGap is enabled, 
 *          - If SPS periodicity is multiple of measGap period (40/80 ms),
 *          numOfAvailSpsOffset = SPS periodicity - (periodicity/MGP * 10); 
 *        - If DRX is enabled,
 *          - Validate that SPS periodicity is multiple of long_drx_cylce
 *          - Validate that maxTxCount * HARQ_RTT < (numOfSpsHarqProc * periodicity)
 *        - If DL SPS periodicity is multiple of 8, the activation is done
 *        such that it never collides with UL retransmissions of UEs for HD
 *        FDD UE.
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeCfg      *ueCfg
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsVldtDlUeCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsVldtDlUeCfg (cell, ue, ueCfg) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif /*DEBUGP*/
   U16             prdIdx;

   TRC2(rgSCHCmnSpsVldtDlUeCfg);
   
   /* Validate that ack-nack repetition is not enabled for DL SPS UE */
   if (ueCfg->ueAckNackCfg.isAckNackEnabled)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Ack-nack repetition not supported DL SPS enabled UE %d\n", 
               (int)ue->ueId));
      RETVALUE(RFAILED);
   }
   RG_SCH_CMN_SPS_GET_PRD_IDX ((int)ueCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum, 
&prdIdx);

   /* Note: prdIdx shall not be invalid since all possible periodicity
    * configurations have been taken care of */
   if (prdIdx == RG_SCH_CMN_SPS_PRD_INVALID)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Invalid SPS periodicity for ue %d, periodicity %d\n", 
               (int)ue->ueId, (int)ueCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum));
      RETVALUE(RFAILED);
   }

#ifdef LTEMAC_DRX
   /* Validate if DRX is enabled for the UE, SPS periodicity should be multiple
    * of long DRX cycle */
   if ((ueCfg->ueDrxCfg.isDrxEnabled) &&
         (ueCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum %
           ueCfg->ueDrxCfg.drxLongCycleOffst.longDrxCycle))
   {

      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Invalid configuration for DRX long cycle/DL SPS periodicity"
               ": UE %d, DRX long cycle: %d, DL SPS Prd: %d \n", 
               (int)ue->ueId,
               (int)ueCfg->ueDrxCfg.drxLongCycleOffst.longDrxCycle,
               (int)ueCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum));
      RETVALUE(RFAILED);
   }
#endif
   
   RETVALUE(ROK);

}

/**
 * @brief  UE reconfiguration for DL SPS
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeRecfg
 *
 *
 *     Processing Steps:
 *        -  If (ue->dlSpsEnabled && ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsEnabled)
 *           - If both the configurations do not match, return RFAILED
 *           - if (ue->isSpsActv) and collision of the offset is 
 *              detected with new configuration,
 *              - Set (ue->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_TIME)
 *        - If (ue->dlSpsEnabled && !ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsEnabled)
 *           - if (ue->isSpsActv) 
 *             - Add UE to toBeRelUes queue for SPS Release
 *           - Remove it from the SPS Queues
 *        -  If (!ue->dlSpsEnabled && ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsEnabled)
 *           - Perform validations. Call rgSCHCmnSpsVldtDlUeRecfg()
 *           - Update cell-wide DL SF data structure based on the configured
 *             periodicity of DL SPS. Call rgSCHCmnSpsDlUpdSf.
 *           - Perform initialization of UE specifc information for 
 *             DL SPS enabled UE. Call rgSCHCmnSpsDlUeInit.
 *           - Compute Y table for SPS-RNTI. Call rgSCHUtlUpdY().
 *           - Return ROK. 
 *     - Return ROK. 
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeRecfg    *ueRecfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeRecfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeRecfg (cell, ue, ueRecfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                  instIdx   = cell->instIdx;
#endif /*DEBUGP*/
   S16                   ret;
   RgrUeSpsDlCfg         *dlSpsCfg = &ueRecfg->ueSpsRecfg.dlSpsCfg;
   RgrUeSpsDlCfg         *ueSpsCfg = &ue->dl.dlSpsCfg;
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   Bool                  isDlSpsEnabled;
   Bool                  isSpsRecfg = FALSE;

   TRC2(rgSCHCmnSpsDlUeRecfg);
   /* Validate reconfigured values if either DL SPS is getting enabled or is
    * already enabled */
   if (ueRecfg->ueRecfgTypes & RGR_UE_DLSPS_RECFG)
   {
      isDlSpsEnabled = dlSpsCfg->isDlSpsEnabled;
      isSpsRecfg     = TRUE;
   }
   else
   {
      isDlSpsEnabled = ueSpsCfg->isDlSpsEnabled;
   }

   if (isDlSpsEnabled)
   {
      /* Validate the re-configured values of measGap and DRX */
      if ((ret = rgSCHCmnSpsVldtDlUeRecfg(cell, ue, ueRecfg)) != ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "DL SPS sub-frame update failed %d\n", 
                  (int)dlSpsCfg->dlSpsPrdctyEnum));
         RETVALUE(RFAILED);
      }

      /* Update measurement Gap mask of the UE if measurement gap is
       * re-configured, if UE was previously SPS enabled */
      if (ueSpsCfg->isDlSpsEnabled)
      {
         if (ueRecfg->ueRecfgTypes & RGR_UE_MEASGAP_RECFG)
         {
            /* Note: Latest Measurement gap configuration being read from the 
             * ueRecfg rather than from ueCb */
            rgSCHCmnSpsDlInitMeasGapMask(cell, &ueRecfg->ueMeasGapRecfg,
                  dlSpsUe);
         } 

         if (dlSpsUe->isSpsActv)
         {
            /* Check collision with existing activation */
            if (rgSCHCmnSpsDlUeCollidesInTime(cell, ue, 
                     dlSpsUe->dlSpsUeSchdInfo.schdKey))
            {
               /* Existing activation collides, mark UE to be reacitvated */
               dlSpsUe->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_TIME;
               rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue);
            }
         }
      }
   }

   if (isSpsRecfg)
   {
      if (dlSpsCfg->isDlSpsEnabled && ueSpsCfg->isDlSpsEnabled)
      {
         /* Note: Not handling any re-configured values here */
         /* Currently re-configuration of values is not supported for SPS enabled
          * UE */
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "DL SPS values' reconfiguration for UE not supported:"
                  " UE %d\n", (int)ue->ueId));

         /* Could think of re-activation here - design needs to be thought
          * through, if done then UE shall never have temporary disabling of the
          * SPS service */
      }
      else if (!dlSpsCfg->isDlSpsEnabled && ueSpsCfg->isDlSpsEnabled)
      {
         /* DL SPS getting disabled */
         rgSCHCmnSpsDlUeSuspend(cell, ue);
         cmMemset((U8 *)dlSpsUe, 0, sizeof(*dlSpsUe));
         ue->dl.dlSpsCfg = *dlSpsCfg;

         /* Note: Not sending explicit release for this  UE since SPS is getting
          * disabled for the UE, it shall release implicitly [36.321, Sec 5.10] */
         /* Note: If temporarily getting disabled, then it shall be re-configured
          * by RRC for enabling SPS and UE shall expect data for very short period
          * of time for SPS ocassions which could be ignored */
      }
      else if (dlSpsCfg->isDlSpsEnabled && !ueSpsCfg->isDlSpsEnabled)
      {
         /* DL SPS enabled right now */

         /* Update the subframe information based on the configured periodicity value */
         if ((ret = rgSCHCmnSpsDlUpdSf(cell, dlSpsCfg->dlSpsPrdctyEnum)) != ROK)
         {
            RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                     "DL SPS sub-frame update failed %d\n", 
                     (int)dlSpsCfg->dlSpsPrdctyEnum));
            RETVALUE(RFAILED);
         }

         /* Initialize SPS related information in UeCb */
         if ((ret = rgSCHCmnSpsDlUeInit(cell, ue, &ueRecfg->ueSpsRecfg, 
                     &ueRecfg->ueMeasGapRecfg)) != ROK)
         {
            RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                     "Ue Initialization failed: n1Pucch not available\n"));
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief  UE configuration for DL SPS enabled UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeCfg
 *
 *
 *     Processing Steps:
 *     - Update cell-wide DL SF data structure based on the configured
      periodicity of DL SPS. Call rgSCHCmnSpsDlUpdSf.
 *     - Perform initialization of UE specifc information for DL SPS enabled UE.
 *     Call rgSCHCmnSpsDlUeInit.
 *     - Compute Y table for SPS-RNTI. Call rgSCHUtlUpdY().
 *     - Return ROK. 
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeCfg      *ueCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeCfg (cell, ue, ueCfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif /*DEBUGP*/
   S16             ret;
   RgrUeSpsDlCfg   *dlSpsCfg = &ueCfg->ueSpsCfg.dlSpsCfg;

   TRC2(rgSCHCmnSpsDlUeCfg);
   /* Update the subframe information based on the configured periodicity value */
   if ((ret = rgSCHCmnSpsDlUpdSf(cell, dlSpsCfg->dlSpsPrdctyEnum)) != ROK)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "DL SPS sub-frame update failed %d\n", 
               (int)dlSpsCfg->dlSpsPrdctyEnum));
      RETVALUE(RFAILED);
   }

   /* Initialize SPS related information in UeCb */
  if ((ret = rgSCHCmnSpsDlUeInit(cell, ue, &ueCfg->ueSpsCfg,
              &ueCfg->ueMesGapCfg)) != ROK)
  {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
              "Ue Initialization failed: n1Pucch not available\n"));
      RETVALUE(RFAILED);
  }

   RETVALUE(ROK);

}

/**
 * @brief Validates the UE re-configuration for SPS UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsVldtDlUeRecfg
 *
 *
 *     Processing Steps:
 *      - Validate the re-configuration with respect to SPS. 
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeRecfg    *ueRecfg
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsVldtDlUeRecfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsVldtDlUeRecfg (cell, ue, ueRecfg) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
#endif /* ANSI */
{
#ifdef DEBUGP 
   Inst                instIdx = cell->instIdx;
#endif /*DEBUGP*/
   Bool                isAckNackEnabled;
#ifdef RG_UNUSED
   /* Enable this if DRX implementation is completed */
   Bool                isDrxEnabled;
   U16                 longDrxCycle;
   U16                 dlSpsPrd = 0;
#endif
   TRC2(rgSCHCmnSpsVldtDlUeRecfg);
   /* Check if ack-nack repetition is reconfigured and use the re-configured
    * value */
   if (ueRecfg->ueRecfgTypes & RGR_UE_ACKNACK_RECFG)
   {
      isAckNackEnabled = ueRecfg->ueAckNackRecfg.isAckNackEnabled;  
   }
   else
   {
      isAckNackEnabled = ue->ackNakRepCb.isAckNackEnabled;
   }
   /* Validate that ack-nack repetition is not enabled for DL SPS UE */
   if (isAckNackEnabled)
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Ack-nack repetition not supported DL SPS enabled UE %d\n", 
               (int)ue->ueId));
      RETVALUE(RFAILED);
   }

#ifdef RG_UNUSED
   /* Enable this if DRX implementation is completed */
   /* Get SPS periodicity for the new configuration */
   if (ueRecfg->recfgTypes & RGR_UE_DLSPS_RECFG)
   {
      if (ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)
      {
         dlSpsPrd = ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsPrdctyEnum;
      }
   }
   else
   {
      if (ue->dl.dlSpsCfg.isDlSpsEnabled)
      { 
         dlSpsPrd = ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
      }
   }
   
   /* Get DRX values post re-configuration */
   if (ueRecfg->recfgTypes & RGR_UE_DRX_RECFG)
   {
      isDrxEnabled = ueRecfg->ueDrxRecfg.isDrxEnabled;  
      longDrxCycle  = ueRecfg->ueDrxRecfg.drxLongCycleOffst.longDrxCycle;
   }
   else
   {
      isDrxEnabled = ue->drxCb->isDrxEnabled;
      longDrxCycle = ue->drxCb->longDrxCycle;
   }

   /* Validate if DRX is enabled for the UE, SPS periodicity should be multiple
    * of long DRX cycle */
   if ((isDrxEnabled) && ((longDrxCycle > dlSpsPrd) ||
            (dlSpsPrd % longDrxCycle)))
   {

      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Invalid configuration for DRX long cycle/DL SPS periodicity"
               ": UE %d, DRX long cycle: %d, DL SPS Prd: %d \n", 
               (int)ue->ueId, (int)longDrxCycle, (int)dlSpsPrd));
      RETVALUE(RFAILED);
   }
#endif /*ifdef RG_UNUSED */
   
   RETVALUE(ROK);
}


/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlFillRelPdcch
 *
 *
 *     Desc : Fills up the release PDCCH for SPS UE.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillRelPdcch
(
RgSchUeCb       *ue,
RgSchPdcch      *relPdcch
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillRelPdcch(ue, relPdcch) 
RgSchUeCb       *ue;
RgSchPdcch      *relPdcch;
#endif /* ANSI */
{
   Tfudciformat1aRelPdcch *relPdcchDciInfo;
#ifdef LTE_TDD
   RgSchTddANInfo         *anInfo;
#endif
   TRC2(rgSCHCmnSpsDlFillRelPdcch);
   
   relPdcch->rnti = ue->ueId;

   relPdcchDciInfo = 
      &(relPdcch->dci.u.format1aInfo.t.relPdcchInfo);
   relPdcch->dci.u.format1aInfo.dciInfo = TFU_DCIFMT1A_REL;

   /* Release PDCCH is sent using DCI format 1A */
   relPdcchDciInfo->mcs = 0x1F;
   relPdcchDciInfo->harqProcId.pres = TRUE;
   relPdcchDciInfo->harqProcId.val = 0;
   relPdcchDciInfo->ndi = 0;
   relPdcchDciInfo->rv = 0;

#ifdef LTE_TDD
   anInfo = rgSCHUtlGetUeANFdbkInfo(ue, 
                     &ue->relPdcchFbkTiming);
   if(anInfo)
   {
      relPdcchDciInfo->dai = anInfo->dlDai;
   }
   else
   {
      relPdcchDciInfo->dai = RG_SCH_MAX_DAI_IDX;
   }
#endif

   /* Setting allocation information to all 1's */
   relPdcchDciInfo->alloc = 0xFFFFFFFF;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlFillActvPdcch
 *
 *
 *     Desc :Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcch
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcch(cell, ue, pdcch, isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchDlRbAlloc      *rbAllocInfo = &ue->dl.dlAllocCb;
   TRC2(rgSCHCmnSpsDlFillActvPdcch);

   pdcch->rnti                       = rbAllocInfo->rnti;
   pdcch->dci.dciFormat              = rbAllocInfo->dciFormat;
  
   /* Update subframe and pdcch info in HqTb control block */
   switch(rbAllocInfo->dciFormat)
   {
      case TFU_DCI_FORMAT_1:
         {
            rgSCHCmnSpsDlFillActvPdcchDciFrmt1(cell,dlSpsUe,rbAllocInfo,\
                    pdcch,isSpsOcc,hqProcId);
            break;
         }
      case TFU_DCI_FORMAT_1A:
         {
            rgSCHCmnSpsDlFillActvPdcchDciFrmt1A(cell,dlSpsUe,rbAllocInfo,\
                    pdcch,isSpsOcc,hqProcId);
            break;
         }
      case TFU_DCI_FORMAT_2:
         {
            rgSCHCmnSpsDlFillActvPdcchDciFrmt2(cell,dlSpsUe,rbAllocInfo,\
                    pdcch,isSpsOcc,hqProcId);
            break;
         }
      case TFU_DCI_FORMAT_2A:
         {
            rgSCHCmnSpsDlFillActvPdcchDciFrmt2A(cell,dlSpsUe,rbAllocInfo,\
                    pdcch,isSpsOcc,hqProcId);
            break;
         }
      default:
         {
            RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), "Allocator's" \
                     " incorrect dciForamt Fill\n"));
            break;
         }
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsCompLcmTbl
 *
 *
 *     Desc :Computes the LCM table for the feasible set of periodicity values
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsCompLcmTbl
(
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsCompLcmTbl() 
#endif /* ANSI */
{
   U8 idx1, idx2;

   TRC2(rgSCHCmnSpsCompLcmTbl);
   /* Memset the memory */
   cmMemset((Data *)rgSchCmnSpsLcmTbl, 0, 
         RG_SCH_CMN_SPS_MAX_NUM_PRD * RG_SCH_CMN_SPS_MAX_NUM_PRD * 
         sizeof(U16));

   /* Compute LCM values */
   for (idx1 = 0; idx1 <= RG_SCH_CMN_SPS_MAX_NUM_PRD - 1; ++idx1)
   {
      for (idx2 = 0; idx2 <= RG_SCH_CMN_SPS_MAX_NUM_PRD - 1; ++idx2)
      {
         U16 prd1 = rgSchCmnSpsPrdVal[idx1];
         U16 prd2 = rgSchCmnSpsPrdVal[idx2];
         rgSchCmnSpsLcmTbl[idx1][idx2] = rgSCHCmnSpsCompLcm(prd1, prd2);
      }
   }

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsCompLcm
 *
 *
 *     Desc :Computes the LCM of 2 given periodicity values
 *           Assumption: Both the periodicities can be represented as 
 *           2^a * 3^b * 5^c
 *
 *     Ret  : U16
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 rgSCHCmnSpsCompLcm
(
 U16      prd1,
 U16      prd2
)
#else /* ANSI */
PRIVATE  U16 rgSCHCmnSpsCompLcm(prd1, prd2) 
U16       prd1;
U16       prd2;
#endif /* ANSI */
{
   U8     cntFactors = 0;
   U16    tmpN1      = prd1;
   U16    tmpN2      = prd2;
   U16    lcm        = 1;
   U8     num        = 2;
   U8     incr       = 0;

   TRC2(rgSCHCmnSpsCompLcm);
   /* Return same value if periodicities are equal */
   if (prd1 == prd2)
   {
      RETVALUE(prd1);
   }

   /* If periodicities are multiple of each other, return the greater value */
   if ((prd1 > prd2) && !(prd1 % prd2))
   {
      RETVALUE(prd1);
   }
   else if ((prd2 > prd1) && !(prd2 % prd1))
   {
      RETVALUE(prd2);
   }

   /* Compute LCM by computing factors for 2, 3 and 5 */
   do
   {
      while (!(tmpN1 % num) && !(tmpN2 % num))
      {
         tmpN1 = tmpN1/num;
         tmpN2 = tmpN2/num;
         ++cntFactors;
      }
      while (!(tmpN1 % num))
      {
         tmpN1 = tmpN1/num;
         ++cntFactors;
      }
      while (!(tmpN2 % num))
      {
         tmpN2 = tmpN2/num;
         ++cntFactors;
      }

      lcm *= (U32)cmPow((F64)num, (F64)cntFactors);
      cntFactors = 0;
      ++incr;
      num = num + incr;
   } while (num <= 5);
   
   RETVALUE(lcm);
}

/**
 * @brief Initializes the DL SPS specific data structures for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsDlCellCfg
 *
 *       
 *     Processing Steps:
 *     - Initialize the SPS lists maintained at the cell level.
 *     - Initialize the subframe information for the cell. Call 
 *       rgSCHCmnSpsDlSfInit 
 *     - Initialize the cell-wide n1Pucch data base. Call 
 *       rgSCHCmnSpsDlN1PucchInit
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlCellCfg
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlCellCfg (cell, cellCfg, err) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif /*DEBUGP*/
   S16             ret       = ROK;

   TRC2(rgSCHCmnSpsDlCellCfg);
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
        "rgSCHCmnSpsDlCellCfg:DL BW: %d\n", (int)cellCfg->bwCfg.dlTotalBw));
   
   /* Initialize the SPS lists maintained at the cell level: Assumed the cell
    * shall be memset to zero by the caller */

   /* Initialize the cell */
   rgSCHCmnSpsDlCellInit(cell, cellCfg);

   /* Initialize subframe related information per cell for DL SPS */
   if ((ret = rgSCHCmnSpsDlSfInit(cell, cellCfg)) != ROK)
   {
        RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                 "Memory allocation FAILED for SPS sub-frame table\n"));
        RETVALUE(RFAILED);
   }
   
   /* Initialize n1Pucch database per cell for DL SPS */
   rgSCHCmnSpsDlN1PucchInit(cell, cellCfg);
   
   RETVALUE(ROK);
}

/**
 * @brief Initializes DL SPS specific information for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsDlCellInit
 *
 *       
 *     Processing Steps:
 *     - Compute rbgSubset sizes for SPS 
 *     - Update the configured values in CellCb
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlCellInit
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlCellInit (cell, cellCfg) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
#endif /* ANSI */
{
   U8              maxRaType1SpsBw;

   TRC2(rgSCHCmnSpsDlCellInit);
   /* Note: Assumed that the bwRbgInfo has been updated in the cell before call to
    * this API for configured total DL bandwidth */
   /* Note: Also assumed that SPS bandwidth starts from lowest frequency i.e.
    * RB0 onwards */
   /* Note: Assuming that cell->rbgSize shall be set correctly by common
    * module */
   /* Setting maxSpsDlBw to a value such that offset=1 is not needed for SPS BW 
    * for RA type 1 */
   maxRaType1SpsBw = cellCfg->dlSpsCfg.maxSpsDlBw;
   rgSCHCmnDlSpsRstMaxSpsBw(cell->rbgSize, cell->bwCfg.dlTotalBw, 
         &maxRaType1SpsBw);

   /* Determine the RBG information for the configured SPS BW */
   rgSCHCmnDlGetRbgInfo (cell->bwCfg.dlTotalBw, cellCfg->dlSpsCfg.maxSpsDlBw,
         maxRaType1SpsBw, cell->rbgSize, &cell->spsBwRbgInfo); 

   cell->dlSpsCfg.maxSpsDlBw =  cell->spsBwRbgInfo.numRbs;

   RETVOID;
}


/**
 * @brief Initializes subframe related information for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsDlSfInit
 *
 *       
 *     Processing Steps:
 *     - Set spsPrdLcm  = default value
 *     - Initialize the subframes for DL SPS 
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlSfInit
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlSfInit (cell, cellCfg) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
#endif /* ANSI */
{
   S16                   ret        = ROK;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);

   TRC2(rgSCHCmnSpsDlSfInit);
   dlSpsCell->spsPrdLcmVal =  RG_SCH_SPS_DFLT_PRD;
   RG_SCH_CMN_SPS_GET_PRD_IDX (dlSpsCell->spsPrdLcmVal, &dlSpsCell->lcmIdx);
   dlSpsCell->spsSfTbl = NULLP;
  
   if (dlSpsCell->spsPrdLcmVal > 1)
   {
      /* Allocate DL SPS subframes for the LCM value */
      ret = rgSCHUtlAllocSBuf(cell->instIdx, 
           (Data**)(&dlSpsCell->spsSfTbl),
           (dlSpsCell->spsPrdLcmVal * sizeof(RgSchCmnSpsDlSf)));
      if(ret != ROK)
      {
         RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
                 "Memory allocation FAILED for SPS sub-frame table\n"));
         RETVALUE(ret);
      }
   } 

   RETVALUE(ret);
}

/**
 * @brief Resets the maximum SPS bandwidth to ensure that offset=1 is not needed
 * for RA type1 allocations
 *
 * @details
 *     Function : rgSCHCmnDlSpsRstMaxSpsBw
 *
 *       
 *     Processing Steps:
 *     - Reset SPS BW to avoid offset = 1 allocations for RA type 1
 *
 *   @param[in]  U8            rbgSize
 *   @param[in]  U8            dlTotalBw
 *   @param[out] U8            *spsBw  
 *  @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlSpsRstMaxSpsBw
(
U8             rbgSize,
U8             dlTotalBw,
U8             *spsBw
)
#else /* ANSI */
PRIVATE Void rgSCHCmnDlSpsRstMaxSpsBw(rbgSize, dlTotalBw, spsBw) 
U8             rbgSize;
U8             dlTotalBw;
U8             *spsBw;
#endif /* ANSI */
{
  
   U8       maxRaType1Offset0Bw = dlTotalBw - (rbgSize * rbgSize);
   U8       maxSpsBw = ((*spsBw + rbgSize - 1)/rbgSize) * rbgSize;

   TRC2(rgSCHCmnDlSpsRstMaxSpsBw);
   
   if (maxSpsBw > dlTotalBw)
   {
      maxSpsBw = dlTotalBw;
   }
   if (maxRaType1Offset0Bw % rbgSize)
   {
      maxRaType1Offset0Bw -= maxRaType1Offset0Bw % rbgSize; 
   }
   if (maxSpsBw > maxRaType1Offset0Bw)
   {
      *spsBw = maxRaType1Offset0Bw;
   }

   RETVOID;
}

/**
 * @brief Initializes the DL SPS specific data structures for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsDlN1PucchInit
 *
 *       
 *     Processing Steps:
 *     - Initialize the cell-wide n1Pucch data base
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlN1PucchInit
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlN1PucchInit (cell, cellCfg) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                  instIdx   = cell->instIdx;
#endif /*DEBUGP*/
   U16                   sfIdx;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);

   TRC2(rgSCHCmnSpsDlN1PucchInit);
   
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsDlN1PucchInit:cell %d\n", (int)cell->cellId));

   dlSpsCell->n1PucchDb.numFreeN1Pucch = RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF;
   dlSpsCell->n1PucchDb.numInUseN1Pucch = 0;
   dlSpsCell->n1PucchDb.freeN1PucchStart = dlSpsCell->n1PucchDb.n1PucchLst;
   dlSpsCell->n1PucchDb.inUseN1PucchStart = NULLP;

   for (sfIdx = 0; sfIdx <  RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF - 1; ++sfIdx)
   {
      RgSchCmnSpsDlN1Pucch *n1Pucch = &(dlSpsCell->n1PucchDb.n1PucchLst[sfIdx]);
      n1Pucch->idx  = sfIdx;
      n1Pucch->next = sfIdx + 1;
   }
   dlSpsCell->n1PucchDb.n1PucchLst[sfIdx].idx = sfIdx;

   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlGetN1PucchCb
 *
 *
 *     Desc : Determines the n1Pucch Cb for a given n1Pucch value
 *             - It tries to fetch an already existing CB for given value 
 *               from inUse list
 *             - If not available, it fetches from free list
 *
 *     Ret  : RgSchCmnSpsDlN1Pucch control block
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE RgSchCmnSpsDlN1Pucch *rgSCHCmnSpsDlGetN1PucchCb
(
RgSchCmnSpsDlN1PucchDb  *n1PucchDb,
U16                     n1PucchVal
)
#else /* ANSI */
PRIVATE  RgSchCmnSpsDlN1Pucch *rgSCHCmnSpsDlGetN1PucchCb(n1PucchDb, n1PucchVal) 
RgSchCmnSpsDlN1PucchDb  *n1PucchDb;
U16                     n1PucchVal;
#endif /* ANSI */
{
   U16                  cnt;
   RgSchCmnSpsDlN1Pucch *tmpCb = n1PucchDb->inUseN1PucchStart;

   TRC2(rgSCHCmnSpsDlGetN1PucchCb);
   /* Check if n1PucchCb for the n1Pucch value already exists */
   for (cnt = 0; cnt < n1PucchDb->numInUseN1Pucch; ++cnt)
   {
      if (tmpCb->n1PucchVal == n1PucchVal)
      {
         tmpCb->numUes++;
         RETVALUE(tmpCb);
      }
      tmpCb = &n1PucchDb->n1PucchLst[tmpCb->next];
   }

   /* Fetch new n1PucchCb from the free list and update the values */
   if (!n1PucchDb->numFreeN1Pucch)
   {
      RETVALUE(NULLP);
   }

   /* Remove from free list */
   tmpCb = n1PucchDb->freeN1PucchStart;
   tmpCb->n1PucchVal = n1PucchVal;
   tmpCb->numUes++;
   n1PucchDb->freeN1PucchStart = &n1PucchDb->n1PucchLst[tmpCb->next];
   n1PucchDb->numFreeN1Pucch--;

   /* Add to the beginning of inUse list */
   if (n1PucchDb->inUseN1PucchStart)
   {
      tmpCb->next = n1PucchDb->inUseN1PucchStart->idx;
   }
   n1PucchDb->inUseN1PucchStart = tmpCb;
   n1PucchDb->numInUseN1Pucch++;

   RETVALUE(tmpCb);
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlFreeN1Pucch
 *
 *
 *     Desc : Frees the n1Pucch value
 *            - Decrements the count for the number of UEs using the value
 *            - If count == 0, 
 *                - Remove from inUse list and 
 *                - Add the corresponding n1PucchCb to free list
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFreeN1Pucch
(
RgSchCmnSpsDlN1PucchDb  *n1PucchDb,
RgSchCmnSpsDlN1Pucch    *n1PucchCb
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFreeN1Pucch(n1PucchDb, n1PucchCb) 
RgSchCmnSpsDlN1PucchDb  *n1PucchDb;
RgSchCmnSpsDlN1Pucch    *n1PucchCb;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsDlFreeN1Pucch);
   /* Decrement number of UEs using the n1Pucch vaue */
   --n1PucchCb->numUes;

   if (!n1PucchCb->numUes)
   {
      /* Remove from beginning of inUse list */
      if (n1PucchCb == n1PucchDb->inUseN1PucchStart)
      {
         n1PucchDb->inUseN1PucchStart = \
                            &n1PucchDb->n1PucchLst[n1PucchCb->next];
      }
      else
      {
         RgSchCmnSpsDlN1Pucch *tmpCb;
         RgSchCmnSpsDlN1Pucch *tmpCb1;
         U16                  cnt = n1PucchDb->numInUseN1Pucch - 1;
         tmpCb =  n1PucchDb->inUseN1PucchStart;
         tmpCb1 = &n1PucchDb->n1PucchLst[tmpCb->next];
         while (cnt && tmpCb1 != n1PucchCb)
         {
            tmpCb = &n1PucchDb->n1PucchLst[tmpCb->next];
            tmpCb1 = &n1PucchDb->n1PucchLst[tmpCb->next];
            cnt--;
         }
         /* Note: Assuming that the n1PucchCb definitely exists in inUse
          * list: Not checkining if cnt == 0 */
         /* Control reaches here only if tmpCb == n1PucchCb */
         /* Note: Could think of removing following check */
         if (tmpCb1 == n1PucchCb)
         {
            tmpCb->next = tmpCb1->next;
         }
      }
      n1PucchDb->numInUseN1Pucch--;

      /* Add to the beginning of the free list */
      if (n1PucchDb->freeN1PucchStart)
      {
         n1PucchCb->next = n1PucchDb->freeN1PucchStart->idx;
      }
      n1PucchDb->freeN1PucchStart = n1PucchCb;
      n1PucchDb->numFreeN1Pucch++;
   }

   RETVOID;
}

/**
 * @brief Delete the SPS context for the cell at cell delete for DL SPS
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlCellDel
 *     
 *     Processing Steps:
 *      - Free the DL SPS related data structures at cell level 
 *      - Return Void
 *
 *  
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlCellDel 
(
RgSchCellCb        *cell
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlCellDel (cell) 
RgSchCellCb        *cell;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                  instIdx    = cell->instIdx;
#endif   
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);

   TRC2(rgSCHCmnSpsDlCellDel);
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsDlCellDel: cellId: %d\n", cell->cellId));

   /* Free the sub-frame information for DL SPS */
   if (dlSpsCell->spsSfTbl)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, 
              (Data**)(&(dlSpsCell->spsSfTbl)),
              sizeof(*dlSpsCell->spsSfTbl));
   }

   cmMemset((U8 *)dlSpsCell, 0, sizeof(*dlSpsCell));

   RETVOID;
}

/**
 * @brief Initializes DL SPS specific information for the UE.
 *
 * @details
 *     Function : rgSCHCmnSpsDlUeInit
 *
 *       
 *     Processing Steps:
 *     - Determine the index for the configured n1Pucch value.Update the
 *     index in the UeCb
 *     - Initialize ueCb->measGapMask with the measurement gap ocassions.
 *     - Update the configured values in UeCb
 *     - Update ue->spsRnti
 *
 *   @param[in]  RgSchCellCb     *cell
 *   @param[in]  RgSchUeCb       *ue
 *   @param[in]  RgrUeSpsCfg     *ueSpsCfg
 *   @param[in]  RgrUeMeasGapCfg *ueMeasGapCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeInit
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeSpsCfg        *ueSpsCfg,
RgrUeMeasGapCfg    *ueMeasGapCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeInit (cell, ue, ueSpsCfg, ueMeasGapCfg) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeSpsCfg        *ueSpsCfg;
RgrUeMeasGapCfg    *ueMeasGapCfg;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                  instIdx    = cell->instIdx;
#endif /*DEBUGP*/
   S16                   ret        = ROK;
   U8                    n1PucchIdx;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnSpsDlN1Pucch  *n1PucchCb = NULLP;
   RgrUeSpsDlCfg         *dlSpsUeCfg  = &ueSpsCfg->dlSpsCfg;

   TRC2(rgSCHCmnSpsDlUeInit);
   /* Update the indices for n1Pucch values */
   for (n1PucchIdx = 0; n1PucchIdx < dlSpsUeCfg->numPucchVal; ++n1PucchIdx)
   {
      n1PucchCb = rgSCHCmnSpsDlGetN1PucchCb(&dlSpsCell->n1PucchDb, 
            (U16)dlSpsUeCfg->n1PucchVal[n1PucchIdx]);
      if (n1PucchCb == NULLP)
      {
         /* No free n1PucchCb is available */
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "No free n1PucchCb available: in Use n1Pucch Count: %d\n",
                  dlSpsCell->n1PucchDb.numInUseN1Pucch));
         RETVALUE(RFAILED);
      }
      dlSpsUe->n1PucchIdx[n1PucchIdx] = n1PucchCb->idx;
   }

   /* If measurement gap is enabled for the UE, update the mask */
   /* Indices corresponding to the positions at which scheduling 
      cannot be done due to meas gap are set 1*/
   if (ueMeasGapCfg->isMesGapEnabled)
   {
      rgSCHCmnSpsDlInitMeasGapMask(cell, ueMeasGapCfg, dlSpsUe);
   }

   /* Initialize DL SPS UE pointer */
   RG_SCH_CMN_SPS_GET_PRD_IDX (dlSpsUeCfg->dlSpsPrdctyEnum, &dlSpsUe->prdIdx);

   /* Update the configuration information in the ueCb */
   /* Note: Not validating the uniqueness of SPS RNTI at scheduler */
   ue->spsRnti = ueSpsCfg->spsRnti;
   ue->dl.dlSpsCfg = *dlSpsUeCfg;

#ifdef LTE_TDD
   /* In case of TDD, SPS periodicity shall be rounded off to an integer. For
    * ex: 128 is considered to be 120 */
   RGSCH_SPS_GET_PRDCTY (ue->dl.dlSpsCfg.dlSpsPrdctyEnum,
                                   ue->dl.dlSpsCfg.dlSpsPrdctyEnum);
#endif /*LTE_TDD*/

   dlSpsUe->numRelPdcchSent = 0;
   /* Initialize feedback timing with invalid value */
   ue->relPdcchFbkTiming.sfn = RGSCH_MAX_SFN + 1;

   RETVALUE(ret);
}

/**
 * @brief Clear the SPS context for the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeSuspend
 *     
 *     Processing Steps:
 *      - Free the DL SPS related data structures at UE level 
 *      - Return Void
 *
 *  
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb   *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlUeSuspend 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlUeSuspend (cell, ue) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst                  instIdx    = cell->instIdx;
#endif /*DEBUGP*/
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc;
   U8                    hqPIdx;
   RgSchDlHqProcCb       *hqP;
   /*rg002.301 ccpu00119494-ADD-delete UE from wtngForFdbk list */
   CmLteTimingInfo fdbkTime;

   TRC2(rgSCHCmnSpsDlUeSuspend);
   RGSCHDBGPRM(instIdx, (rgSchPBuf(instIdx),
            "rgSCHCmnSpsDlUeSuspend: cellId: %d ueId: %d\n", 
            cell->cellId, ue->ueId));
   
   /*rg002.301 ccpu00119494-ADD-Delete UE from wtngForFeedback list */
   /* Remove from wtngForRelFdbkUeLst in case Release PDCCH is sent but
    * UE DelReq is received before feedback could arrive */
   if ( dlSpsUe->isRelPdcchSent )
   {
     RGSCHADDTOCRNTTIME(ue->relPdcchFbkTiming,fdbkTime,TFU_HQFBKIND_ULDELTA); 
     cmLListDelFrm(&(dlSpsCell->wtngForRelFdbkUeLst[fdbkTime.subframe]),
                     &(dlSpsUe->wtngForRelFdbkUeEnt));

   }

   /* Remove UE from toBeActvtdUes or toBeRelUes list */
   if (dlSpsUe->spsList)
   {
      if (dlSpsUe->spsList == &dlSpsCell->toBeActvtdUes)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
      }
      else
      {
         rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue);
      }
   }

     

   /* If UE is SPS active, clear the allocation of the UE from the cell-wide
    * subframes */
   if (dlSpsUe->isSpsActv)
   {
      rgSCHCmnSpsDlUeClearAlloc(cell, ue);
   }

   if (dlSpsUe->spsSvc)
   {
      dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);
      if(dlSpsSvc->toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
      }
      cmMemset((U8 *)dlSpsSvc, 0, sizeof(*dlSpsSvc));
   }

   /* Remove HARQ Proc from re-transmitting queues, if present */
   for(hqPIdx = 0; hqPIdx < ue->dl.hqEnt->numHqPrcs; hqPIdx++)
   {
      hqP = &ue->dl.hqEnt->procs[hqPIdx];
      rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP);
   }
   
   ue->dl.isSpsHqPInUse = FALSE;
   RETVOID;
}

/**
 * @brief Update the cell-wide sub-frame information for DL SPS.
 *
 * @details
 *     Function : rgSCHCmnSpsDlUpdSf
 *
 *       
 *     Processing Steps:
 *     - Compute the new LCM value with the DL SPS periodicity value
 *     - if (!cell->spsSfTbl)
 *       -  Allocate for spsSfTbl with new_lcm size
 *     - else if (cell->spsSfTbl && new_lcm > old_lcm) 
 *       -  Re-allocate for spsSfTbl with new_lcm
 *       -  Copy the old data in newly allocated memory. Replicate it for
 *         (new_lcm/old_Lcm) times in newly allocated memory.
 *
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  U32           prdVal
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUpdSf
(
RgSchCellCb        *cell,
U32                prdVal
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUpdSf (cell, prdVal) 
RgSchCellCb        *cell;
U32                prdVal;
#endif /* ANSI */
{
   S16                   ret        = ROK;
#ifdef DEBUGP 
   Inst                  instIdx    = cell->instIdx;
#endif
   U8                    prdIdx;
   U8                    lcmIdx=0;
   U16                   oldLcm     = 1;
   U16                   newLcm     = 1;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnSpsDlSf       *tmpSpsSfTbl = NULLP;

   TRC2(rgSCHCmnSpsDlUpdSf);
   
   oldLcm = dlSpsCell->spsPrdLcmVal;
   lcmIdx = dlSpsCell->lcmIdx;

   /* Get index for new periodicity */
   RG_SCH_CMN_SPS_GET_PRD_IDX (prdVal, &prdIdx);

   /* Note: prdIdx shall not be invalid since all possible periodicity
    * configurations have been taken care of */
   if (lcmIdx >= RG_SCH_CMN_SPS_PRD_INVALID)
   {
      newLcm = rgSCHCmnSpsCompLcm(oldLcm, (U16)prdVal);
   }
   else
   {
      newLcm = rgSchCmnSpsLcmTbl[lcmIdx][prdIdx];
   }

   /* Replicate the data structure information if the newLcm > oldLcm for
    * newLcm/oldLcm number of times */
   /* Note: newLcm/oldLcm is always an integer */
   if (newLcm > oldLcm)
   {
      ret = rgSCHUtlAllocSBuf(cell->instIdx, 
            (Data**)(&tmpSpsSfTbl),
            (newLcm * sizeof(RgSchCmnSpsDlSf)));
      if(ret != ROK)
      {
         RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
                  "Memory allocation FAILED for SPS sub-frame table: "
                  "lcmVal: %d\n", (int)newLcm));
         RETVALUE(ret);
      }

      if (oldLcm > 1)
      {
         /* If old subframe info exists, replicate old sub-frame info across 
          * new subframes */
         U16   iterCntr = newLcm/oldLcm;
         U16   idx;
         for (idx = 0; idx < iterCntr; ++idx)
         {
            cmMemcpy((U8 *)(tmpSpsSfTbl + (idx * oldLcm)),
                  (U8 *)dlSpsCell->spsSfTbl, oldLcm * sizeof(RgSchCmnSpsDlSf));
         }
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, (Data**)(&(dlSpsCell->spsSfTbl)),
               oldLcm * sizeof(RgSchCmnSpsDlSf));
      }

      /* Update the cell-wide data structure with new information */
      dlSpsCell->spsSfTbl =  tmpSpsSfTbl;
      dlSpsCell->spsPrdLcmVal =  newLcm;
      RG_SCH_CMN_SPS_GET_PRD_IDX (dlSpsCell->spsPrdLcmVal, &dlSpsCell->lcmIdx);
   }

   RETVALUE(ret);
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlInitMeasGapMask
 *
 *
 *     Desc : Initializes measurement gap mask of the UE based on the
 *     measurement gap configuration 
 *
 *     Ret  : U8
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlInitMeasGapMask
(
RgSchCellCb          *cell,
RgrUeMeasGapCfg      *measGapCfg,
RgSchCmnDlUeSpsInfo  *dlSpsUe
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlInitMeasGapMask (cell, measGapCfg, dlSpsUe)
RgSchCellCb          *cell;
RgrUeMeasGapCfg      *measGapCfg;
RgSchCmnDlUeSpsInfo  *dlSpsUe;
#endif /* ANSI */
{
   U8 idx = 0;
   S8 startOffset = measGapCfg->gapOffst - RG_DL_DELTA;
   U8 hqFbkOffst;
   U8 maskIdx;
   U8 posInMask;
   S8 tempOff;
   
   TRC2(rgSCHCmnSpsDlInitMeasGapMask);
   
#ifndef LTE_TDD
   UNUSED(cell);
#endif /*LTE_TDD*/
   /* If measurement gap is enabled for the UE, update the mask */
   /* Indices corresponding to the positions at which scheduling 
      cannot be done are set 1*/
   if (measGapCfg->isMesGapEnabled)
   {

#ifdef LTE_TDD
      hqFbkOffst =  rgSchHqIntTbl[cell->ulDlCfgIdx];
#else /* LTE_TDD */
      hqFbkOffst = RG_SCH_CMN_HARQ_INTERVAL;
#endif /*LTE_TDD*/
      startOffset -= hqFbkOffst;
      if (startOffset < 0)
      {
         startOffset = startOffset + measGapCfg->gapPrd;
      } 

      for (;idx < RG_SCH_MEAS_GAP_LEN + hqFbkOffst; ++idx)
      {
         maskIdx = startOffset/32;
         posInMask = startOffset%32;
         dlSpsUe->measGapMask[maskIdx] |= 
            1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(posInMask);
         tempOff = ++startOffset % measGapCfg->gapPrd;
         startOffset = tempOff;
      }
   }

   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlGetScaledCqi
 *
 *
 *     Desc : Determines a scaled down CQI value for SPS transmission
 *
 *     Ret  : U8
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U8 rgSCHCmnSpsDlGetScaledCqi
(
RgSchUeCb             *ue,
U8                    curCqi
)
#else /* ANSI */
PRIVATE  U8 rgSCHCmnSpsDlGetScaledCqi(ue, curCqi) 
RgSchUeCb             *ue;
U8                    curCqi;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsDlGetScaledCqi);
   
   if (curCqi <= RG_SCH_SPS_CQI_DECR_VAL)
   {
      RETVALUE(RG_SCH_CMN_SPS_DL_INVALIDCQI_VAL);
   }
   RETVALUE  (curCqi - RG_SCH_SPS_CQI_DECR_VAL);
}

/***************************************************************
                         DL SPS Utility Functions Start
****************************************************************/


/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes
 *
 *
 *     Desc : Adds UE to the toBeActvtdUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes);
   
   if (!dlSpsUe->pndngUeLstEnt.node)
   {
      dlSpsUe->pndngUeLstEnt.node = (PTR)ue;
      cmLListAdd2Tail(&dlSpsCell->toBeActvtdUes, &dlSpsUe->pndngUeLstEnt);
      dlSpsUe->spsList = &dlSpsCell->toBeActvtdUes;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes
 *
 *
 *     Desc : Deletes UE from toBeActvtdUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes);
   
   if (dlSpsUe->pndngUeLstEnt.node)
   {
      cmLListDelFrm(&dlSpsCell->toBeActvtdUes, &dlSpsUe->pndngUeLstEnt);
   }
   dlSpsUe->pndngUeLstEnt.node = NULLP;
   dlSpsUe->spsList = NULLP;
   RETVOID;
}

/***********************************************************
 *     
 *     Func : rgSCHCmnSpsUtlDlAdd2ActvUes
 *     
 *     
 *     Desc : Adds UE to the actvDlSpsUeLsts at the index corresponding to the
 *     time of activation
 *  
 *     Ret  : Void 
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ActvUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue,
CmLteTimingInfo         actvTime
)
#else/* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, actvTime)
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
CmLteTimingInfo         actvTime;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                  idx;

   TRC2(rgSCHCmnSpsUtlDlAdd2ActvUes);
   
   idx =   (actvTime.sfn * 10 + actvTime.subframe) % RG_SCH_CMN_SPS_MAX_PRD;

   if (!dlSpsUe->actvUeLstEnt.node)
   {
      dlSpsUe->actvUeLstEnt.node = (PTR)ue;
      cmLListAdd2Tail(&dlSpsCell->actvDlSpsUeLsts[idx], 
              &dlSpsUe->actvUeLstEnt);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmActvUes
 *
 *
 *     Desc : Deletes UE from actvDlSpsUeLsts list
 *     Ret  : Void
 *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmActvUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue,
CmLteTimingInfo         actvTime
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, actvTime)
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
CmLteTimingInfo         actvTime;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16   idx = (actvTime.sfn * 10 + actvTime.subframe) % RG_SCH_CMN_SPS_MAX_PRD;

   TRC2(rgSCHCmnSpsUtlDlDelFrmActvUes);
   
   if (dlSpsUe->actvUeLstEnt.node)
   { 
      cmLListDelFrm(&dlSpsCell->actvDlSpsUeLsts[idx], &dlSpsUe->actvUeLstEnt);
   }
   dlSpsUe->actvUeLstEnt.node = NULLP;

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2ToBeRelUes
 *
 *
 *     Desc : Adds UE to the toBeRelUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeRelUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlAdd2ToBeRelUes);
   
   if (!dlSpsUe->pndngUeLstEnt.node)
   {
      dlSpsUe->pndngUeLstEnt.node = (PTR)ue;
      cmLListAdd2Tail(&dlSpsCell->toBeRelUes, &dlSpsUe->pndngUeLstEnt);
      dlSpsUe->spsList = &dlSpsCell->toBeRelUes;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmToBeRelUes
 *
 *
 *     Desc : Deletes UE from toBeRelUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeRelUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlDelFrmToBeRelUes);
   
   if (dlSpsUe->pndngUeLstEnt.node)
   { 
      cmLListDelFrm(&dlSpsCell->toBeRelUes, &dlSpsUe->pndngUeLstEnt);
   }
   dlSpsUe->pndngUeLstEnt.node = NULLP;
   dlSpsUe->spsList = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2RetxHqProcs
 *
 *
 *     Desc : Adds HARQ proc to the list of re-transmitting SPS harq procs.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxHqProcs
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlHqProcCb         *hqP
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2RetxHqProcs(dlSpsCell, hqP) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchDlHqProcCb         *hqP;
#endif /* ANSI */
{
   RgSchCmnDlHqProc     *cmnHqProcDl = RG_SCH_CMN_GET_DL_HQP(hqP);

   TRC2(rgSCHCmnSpsUtlDlAdd2RetxHqProcs);
   
   cmnHqProcDl->retxLnk.node = (PTR)hqP;

   cmLListAdd2Tail(&dlSpsCell->retxHqProcs, &cmnHqProcDl->retxLnk);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmRetxHqProcs
 *
 *
 *     Desc : Deletes HARQ proc from the list of re-transmitting SPS harq 
 *     procs.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmRetxHqProcs
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlHqProcCb         *hqP
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchDlHqProcCb         *hqP;
#endif /* ANSI */
{
   RgSchCmnDlHqProc     *cmnHqProcDl = RG_SCH_CMN_GET_DL_HQP(hqP);

   TRC2(rgSCHCmnSpsUtlDlDelFrmRetxHqProcs);
   if (cmnHqProcDl->retxLnk.node)
   {
      cmLListDelFrm(&dlSpsCell->retxHqProcs, &cmnHqProcDl->retxLnk);
      cmnHqProcDl->retxLnk.node = NULLP;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs
 *
 *
 *     Desc : Adds a service to toBeSchdSvcs list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlLcCb             *dlSvc
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSvc) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchDlLcCb             *dlSvc;
#endif /* ANSI */
{
   RgSchCmnDlSvcSpsInfo *dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSvc);
   
   TRC2(rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs);
   
   if (!dlSpsSvc->toBeSchdSvcEnt.node)
   {
      dlSpsSvc->toBeSchdSvcEnt.node = (PTR)dlSvc;

      cmLListAdd2Tail(&dlSpsCell->toBeSchdSvcs, &dlSpsSvc->toBeSchdSvcEnt);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs
 *
 *
 *     Desc : Deletes a service from toBeSchdSvcs list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlLcCb             *dlSvc
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSvc) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchDlLcCb             *dlSvc;
#endif /* ANSI */
{
   RgSchCmnDlSvcSpsInfo *dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSvc);

   TRC2(rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs);
   
   if (dlSpsSvc->toBeSchdSvcEnt.node)
   {
      cmLListDelFrm(&dlSpsCell->toBeSchdSvcs, &dlSpsSvc->toBeSchdSvcEnt);
      dlSpsSvc->toBeSchdSvcEnt.node = NULLP;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2TxSpsUeLst
 *
 *
 *     Desc : This function adds the UE to alloc info's TX SPS UE list.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2TxSpsUeLst
(
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchDlHqProcCb      *hqP
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUtlDlAdd2TxSpsUeLst(allocInfo, hqP)
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchDlHqProcCb      *hqP;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUtlDlAdd2TxSpsUeLst);

   cmLListAdd2Tail(&allocInfo->dedAlloc.txSpsUeLst, &hqP->reqLnk);
   hqP->reqLnk.node = (PTR)hqP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst
 *
 *
 *     Desc : This function adds the UE to alloc info's Retx SPS UE list.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst
(
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchDlHqProcCb      *hqP
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst(allocInfo, hqP)
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchDlHqProcCb      *hqP;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst);
   cmLListAdd2Tail(&allocInfo->dedAlloc.retxSpsUeLst, &hqP->reqLnk);
   hqP->reqLnk.node = (PTR)hqP;

   RETVOID;
}

/***************************************************************
                         DL SPS Utility Functions End
****************************************************************/

/*!< Changes needed in other modules and functions 
 *  tfu.x: 
 *
 *  Change the bitmask used for allocation from an array to a U32 value
 *  Corresponding changes in the code and the test engine also needs to be
 *  taken care of
 *  
 *  rg_sch_dhm.c :
 *  
 *  On reception of positive feedback, a trigger could be possibly sent to SPS
 *  to unset isSpsActv variable on SPS HARQ proc.
 * */
/***************************************************************
                         UL SPS Functions
****************************************************************/

/**
 * @brief Handler for validating Cell configuration request for UL SPS
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlVldtCellCfg
 *
 *     Invoked by: 
 *         rgSCHCmnSpsCellCfg as part of Cell configuration handling
 *
 *     Processing Steps for UL SPS:
 *       if(cellCfg->puschCfg.hopOffst == cellUl->puschBW)
 *         No bandwidth left for SPS
 *         return RFAILED
 *       return ROK
 *           
 *  @param[in] RgSchCellCb    *cell
 *  @param[in] RgrCellCfg     *cellCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlVldtCellCfg
(
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlVldtCellCfg(cell, cellCfg)
RgSchCellCb    *cell;
RgrCellCfg     *cellCfg;
#endif /* ANSI */
{
   RgSchCmnUlCell        *cellUl    = RG_SCH_CMN_GET_UL_CELL(cell);
   TRC2(rgSCHCmnSpsUlVldtCellCfg);
  
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
           "rgSCHCmnSpsUlVldtCellCfg:PUSCH BW: %d, PUSCH hopping offset: %d\n",
            (int)cellUl->puschBw, (int)cellCfg->puschCfg.hopOffst));

   if (cellCfg->puschCfg.hopOffst == cellUl->puschBw)
   {
      RGSCHDBGERR (cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlVldtCellCfg:No bandwidth for SPS allocations\n"));
      return RFAILED;
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlVldtCellCfg */

/**
 * @brief Handler for processing Cell configuration request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlCellCfg
 *
 *     Invoked by: 
 *         rgSCHCmnSpsCellCfg as part of Cell configuration handling
 *
 *     Processing Steps for UL SPS:
 *       Store the puschCfg in rgSchCellCb
 *       Calculate SPS subband information using the pusch hopping offset
 *       if(cellCfg->puschCfg.hopOffst%2)
 *         spsRbs = cellCfg->bwCfg.ulTotalBw - cellCfg->puschCfg.hopOffst - 1
 *       else
 *         spsRbs = cellCfg->bwCfg.ulTotalBw - cellCfg->puschCfg.hopOffst
 *       cmnCell->ul.ulSpsInfo.spsSbStart = 
 *                     CEIL((cellCfg->bwCfg.ulTotalBw - spsRbs)/2/numSbs)
 *       cmnCell->ul.ulSpsInfo.spsSbEnd = 
 *                     (cmnCell->ul.ulSpsInfo.spsSbStart + (spsRbs)/sbSize) -1
 *       cmnCell->ul.ulSpsInfo.spsPrdLcmVal = 1
 *       cmnCell->ul.ulSpsInfo.spsSfArr = NULL
 *       Initialize actvUlSpsUeLsts, toBeActvtdUeLst, 
 *       toBeRelUeLst
 *           
 *  @param[in] RgSchCellCb    *cell
 *  @param[in] RgrCellCfg     *cellCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlCellCfg
(
RgSchCellCb    *cell,
RgrCellCfg    *cellCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlCellCfg(cell, cellCfg)
RgSchCellCb    *cell;
RgrCellCfg    *cellCfg;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlCell        *cellUl    = RG_SCH_CMN_GET_UL_CELL(cell);
   U8                    spsRbStart; /* Starting RB for SPS */
   U16                   idx;
   U32 numSpsSb=0;

   TRC2(rgSCHCmnSpsUlCellCfg);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlCellCfg:UL BW: %d, PUSCH hopping offset: %d\n",
            (int)cellCfg->bwCfg.ulTotalBw, (int)cellCfg->puschCfg.hopOffst));

   /* Calculate SPS subband information using the pusch hopping offset */
   if (!cellCfg->puschCfg.hopOffst)
   {
      /* Entire bandwidth is available for SPS */
      ulSpsCell->spsSbStart = cellUl->sbStartIdx;
      ulSpsCell->numSpsSb = cellUl->numSb;
   }
   else
   {
      U8   hopOffst;
      U8   puschRbStart;
      /* SPS BW lies in between the total BW with few subbands lying on either
       * sides for non-SPS scheduling */
      if ((cell->bwCfg.ulTotalBw - cellCfg->puschCfg.hopOffst) > 
              cellUl->puschBw)
      {
         hopOffst = cell->bwCfg.ulTotalBw - cellUl->puschBw;
      }
      else
      {
         hopOffst = cellCfg->puschCfg.hopOffst;
      }

      spsRbStart = RGSCH_CEIL(hopOffst,2);
      puschRbStart = (cell->bwCfg.ulTotalBw - cellUl->puschBw)/2;
      ulSpsCell->spsSbStart = RGSCH_CEIL(spsRbStart - puschRbStart, cellCfg->puschSubBand.size);
      numSpsSb =
             (cell->bwCfg.ulTotalBw - spsRbStart - puschRbStart) / cellCfg->puschSubBand.size;
      if(numSpsSb)
      {
      ulSpsCell->numSpsSb=numSpsSb- ulSpsCell->spsSbStart;
      }
       
   }

   rgSCHCmnSpsUlInitSpsSf (cell);

   for(idx=0; idx < RG_SCH_CMN_SPS_MAX_PRD; idx++)
   {
      cmLListInit (&(ulSpsCell->actvUlSpsUeLsts[idx]));
   }
   cmLListInit (&ulSpsCell->toBeActvtdUeLst);
   cmLListInit (&ulSpsCell->toBeRelUeLst);

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlCellCfg */


/**
 * @brief Handler for processing Cell deletion request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlCellDel
 *
 *     Invoked by: 
 *         rgSCHCmnSpsCellDel as part of Cell deletion handling
 *
 *     Processing Steps for UL SPS:
 *       Deallocate cell->ulSpsInfo.spsSfLst
 *       toBeRelUeLst
 *           
 *  @param[in] RgSchCellCb    *cell
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlCellDel
(
RgSchCellCb    *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlCellDel(cell)
RgSchCellCb    *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);

   TRC2(rgSCHCmnSpsUlCellDel);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlCellDel:cellId : %d\n", cell->cellId));

   /* Free cell specific sps information */
   if (ulSpsCell->spsSfLst)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(ulSpsCell->spsSfLst)),
                    (ulSpsCell->spsPrdLcmVal* sizeof(RgSchCmnSpsUlSf)));
   }

   /* Free SPS lists */
   rgSCHCmnSpsUlFreeToBeActvUeLst(cell);
   rgSCHCmnSpsUlFreeToBeRelUeLst(cell);
   rgSCHCmnSpsUlFreeActvUeLst(cell);

   /* CellDel shall invoke ueDel which will take care of SPS UE deletion */

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlCellDel */


/**
 * @brief Handler for validating UL SPS configuration received as part of UE
 * configuration request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlVldtUeCfg
 *
 *     Handler for validating SPS configuration that comes as part of 
 *     UE Configuration request 
 *
 *     Invoked by: 
 *         rgSCHCmnSpsUeCfg as part of UE configuration handling
 *
 *     Processing Steps: 
 *       if ((ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled == TRUE) && 
 *           (ueCfg->ueAckNackCfg.isAckNackEnabled == TRUE))
 *            return RFAILED as both are not supported for a UE simultaneously
 *       return ROK
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgrUeCfg     *ueCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlVldtUeCfg
(
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlVldtUeCfg(cell, ueCfg)
RgSchCellCb  *cell;
RgrUeCfg     *ueCfg;
#endif /* ANSI */
{
#ifdef DEBUGP
   Inst            instIdx   = cell->instIdx;
#endif /* DEBUGP */

   TRC2(rgSCHCmnSpsUlVldtUeCfg);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlVldtUeCfg:cellId : %d\n", cell->cellId));

   if (ueCfg->ueAckNackCfg.isAckNackEnabled == TRUE)
   {
      /* SPS is not supported when ACK-NACK repetition is enabled for a UE */
      RGSCHDBGERR (cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlVldtUeCfg:SPS is not supported when ACK-NACK"
            " repetition is enabled for a UE\n"));
      return RFAILED;
   }

#ifdef LTEMAC_DRX
   /* Validate if DRX is enabled for the UE, SPS periodicity should be multiple
    * of long DRX cycle */
   if ((ueCfg->ueDrxCfg.isDrxEnabled) &&
         (ueCfg->ueSpsCfg.ulSpsCfg.ulSpsPrdctyEnum %
           ueCfg->ueDrxCfg.drxLongCycleOffst.longDrxCycle))
   {
      RGSCHDBGERR(instIdx, (rgSchPBuf(instIdx),
               "Invalid configuration for DRX long cycle/DL SPS periodicity"
               ": UE %d, DRX long cycle: %d, DL SPS Prd: %d \n", 
               (int)ueCfg->crnti,
               (int)ueCfg->ueDrxCfg.drxLongCycleOffst.longDrxCycle,
               (int)ueCfg->ueSpsCfg.ulSpsCfg.ulSpsPrdctyEnum));
      RETVALUE(RFAILED);
   }
#endif
   
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlVldtUeCfg */

/**
 * @brief Handler for processing UL SPS configuration of a UE 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlUeCfg
 *
 *     Handler for processing UL SPS configuration that comes as part of
 *     UE Configuration request 
 *
 *     Invoked by: 
 *         rgSCHCmnSpsUeCfg 
 *
 *     Processing Steps: 
 *        - Store the given configuration in ue->ul.ulSpsInfo
 *        - for TDD
 *          ue->ul.ulSpsCfg.spsPrdcty = ueCfg->spsPrdcty - (ueCfg->spsPrdcty % 10)
 *        - for FDD
 *          ue->ul.ulSpsCfg.spsPrdcty = ueCfg->spsPrdcty
 *        - Compute newLCM value using oldLCM value and the SPS
 *          periodicity given in the configuration.
 *            - if (newLcm > oldLcm) 
 *                 if (spsSfLst == NULLP)
 *                    allocate spsSfLst upto newLcm value
 *                    initialize spsAllocDb and spsHoleDb according to SPS BW
 *                 if (spsSfLst)
 *                    reallocate the spsSfLst
 *                    replicate the subframe information till the newLcm
 *        - if (ueCfg->ueMesGapCfg.isMesGapEnabled == TRUE)
 *            - allocate ueCmnUlCb->ulSpsInfo.measGapMask with
 *                        size= (((ueCfg->ueMesGapCfg.gapPrd)) * sizeof(U8))
 *            - set indices in ueUlCb->ulSpsInfo.measGapMask based on the
 *              inactivity subframes due to measurement gap.
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgSchUeCb    *ue
 *  @param[in] RgrUeCfg     *ueCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUeCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUeCfg(cell, ue, ueCfg)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeCfg     *ueCfg;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   U16                   crntLcm = ulSpsCell->spsPrdLcmVal;
   U16                   newLcm;

   TRC2(rgSCHCmnSpsUlUeCfg);
 
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlUeCfg:cellId : %d, ueId : %d\n", 
            cell->cellId, ueCfg->crnti));

   /* Init Ue with given configuration */
   rgSCHCmnSpsUlUeInit(cell, ue, ueCfg->ueSpsCfg,
                             ueCfg->ueMesGapCfg);

   /* Calculate new LCM using the existing value and the UE's periodicity */
   newLcm = rgSCHCmnSpsCompLcm (crntLcm, ue->ul.spsPrdcty);
   if (newLcm > crntLcm)
   {
      /* Create or update the cell-wide spsSfLst */
      if ((rgSCHCmnSpsUlUpdSpsSf(cell, crntLcm, newLcm)) != ROK)
      {
         RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
               "SPS subframe update failed while configuring Ue %d\n", 
               ue->ueId));
         RETVALUE(RFAILED);
      }
      /* Store the new LCM as the current LCM */
      ulSpsCell->spsPrdLcmVal = newLcm;
   } /* end of newLcm > crntLcm */
   
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUeCfg */

/**
 * @brief Handler for validating UL SPS configuration received as part of UE
 * reconfiguration request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlVldtUeRecfg
 *
 *     Handler for validating SPS configuration that comes as part of 
 *     UE Reconfiguration request 
 *
 *     Invoked by: 
 *         rgSCHCmnSpsUeRecfg as part of UE reconfiguration handling
 *
 *     Processing Steps: 
 *       if ((ueRecfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled == TRUE) && 
 *           (ueRecfg->ueAckNackCfg.isAckNackEnabled == TRUE))
 *            return RFAILED as both are not supported for a UE simultaneously
 *       if ((ue->isUlSpsActv == TRUE) && 
 *           (ueRecfg->ueAckNackCfg.isAckNackEnabled == TRUE))
 *            return RFAILED
 *       return ROK
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgSchUeCb    *ue
 *  @param[in] RgrUeCfg     *ueCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlVldtUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlVldtUeRecfg(cell, ue, ueRecfg)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsUlVldtUeRecfg);
 
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlVldtUeRecfg:cellId : %d, ueId : %d\n", 
            cell->cellId, ueRecfg->oldCrnti));

   if (((ue->ackNakRepCb.isAckNackEnabled == TRUE) &&
        (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE)) ||
       ((ueRecfg->ueAckNackRecfg.isAckNackEnabled == TRUE) &&
        ((ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE) ||
         (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE))))
   {
      /* SPS is not supported when ACK-NACK repetition is enabled for a UE */
      RGSCHDBGERR (cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlVldtUeCfg:SPS is not supported when ACK-NACK"
            " repetition is enabled for a UE\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlVldtUeRecfg */

/**
 * @brief Handler for processing UE reconfiguration request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlUeRecfg
 *
 *     Handler for processing UL SPS configuration/reconfiguration that 
 *     comes as part of UE reconfiguration request 
 *
 *     Invoked by: 
 *         rgSCHCmnRgrUeRecfg as part of UE reconfiguration handling
 *
 *     Processing Steps:
 *        -  invoke rgSCHCmnSpsUlVldtUeRecfg
 *        - if validation is unsuccessful,
 *             return RFAILED 
 *        -  if (ue->isUlSpsEnabled == TRUE) 
 *              if (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE)
 *                 return RFAILED
 *              if (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == FALSE))
 *                 if (ue->isUlSpsActv) 
 *                   add UE to toBeRelUes queue for UL SPS Release
 *        -  If (ue->isUlSpsEnabled ==FALSE)
 *              if (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE)
 *                 perform the same actions as in ueCfg handling
 *                 return ROK. 
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgSchUeCb    *ue
 *  @param[in] RgrUeRecfg   *ueRecfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUeRecfg(cell, ue, ueRecfg)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U16                   newLcm;

   TRC2(rgSCHCmnSpsUlUeRecfg);
   
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlUeRecfg:cellId : %d, ueId : %d\n", 
            cell->cellId, ueRecfg->oldCrnti));

   /* Validate the input reconfiguration parameters */
   if (rgSCHCmnSpsUlVldtUeRecfg(cell, ue, ueRecfg) != ROK)
   {
      RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
            "UE recfg validation failed for Ue %d\n", ueRecfg->oldCrnti));
      RETVALUE(RFAILED);
   }

   /* Reconfiguration of UL SPS params is not supported. However, config/ 
    * reconfig of measurement gap shall be considered */ 
   if ((ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE) &&
       (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE))
   {
     /* Update measurement Gap mask of the UE if measurement gap is
       * re-configured */
      if (ueRecfg->ueRecfgTypes & RGR_UE_MEASGAP_RECFG)
      {
         rgSCHCmnSpsUlUeMeasGapMaskInit(cell, ue, ueRecfg->ueMeasGapRecfg);

         if (ulSpsUe->isUlSpsActv)
         {
           /* Check collision with existing activation */
            if (TRUE == rgSCHCmnSpsUlChkMesGapColl(cell, ue,
                ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc,
                ueRecfg->ueMeasGapRecfg.gapPrd))
            {
              /* Existing activation collides, mark UE to be reacitvated */
               ulSpsUe->state = RGSCH_SPS_TO_BE_REACTV;
               rgSCHCmnSpsUlUtlInsUeInToBeActvLst(cell, ue);
            }
         }

         RETVALUE(ROK);
      }
      /* Reconfiguration of the rest is not supported. so return rfailed */
      RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
              "Reconfiguration of UL SPS params is not supported, ueId: %d\n", 
              ueRecfg->oldCrnti));
      RETVALUE(RFAILED);
   }
   else  if ((ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE) &&
       (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == FALSE))
   {
      /* SPS is getting disabled for the UE. Free UE's SPS resources */
      rgSCHCmnSpsUlUtlFreeSpsUe (cell, ue);
   }
   else if ((ue->ul.ulSpsCfg.isUlSpsEnabled == FALSE) &&
       (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE))
   {
      /* UL SPS is getting enabled here. Init Ue with given configuration */
      rgSCHCmnSpsUlUeInit(cell, ue, ueRecfg->ueSpsRecfg,
                                ueRecfg->ueMeasGapRecfg);

      newLcm = rgSCHCmnSpsCompLcm (ulSpsCell->spsPrdLcmVal, ue->ul.spsPrdcty);
      if (newLcm > ulSpsCell->spsPrdLcmVal)
      {
         
         /* Update the cell-wide spsSfLst */
         if ((rgSCHCmnSpsUlUpdSpsSf(cell, ulSpsCell->spsPrdLcmVal, newLcm)) 
                                                                      != ROK)
         {
            RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
                "SPS subframe update failed while configuring Ue %d\n", 
                     ue->ueId));
            RETVALUE(RFAILED);
         }
        
        /* OK first update ulSpsCell with the new LCM that we just computed in the case of reconfiguring the UE */
        ulSpsCell->spsPrdLcmVal = newLcm;
      }
   }
   
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUeRecfg */


/**
 * @brief  Handler for processing UE delete request
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUeDel
 *
 *     Invoked by:
 *      rgSCHCmnUeDel if(ue->isUlSpsEnabled == TRUE)
 *
 *     Processing Steps:
 *       mark the UE allocation in the corr subframes to be free
 *       free ue->measGapMask if it exists
 *       remove UE from its existing lists
 *       return ROK
 *
 * @param[in]  RgSchCellCb *cell
 * @param[in]  RgSchUeCb   *ue
 * @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUeDel 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUeDel (cell, ue) 
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUlUeDel); 
 
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlUeDel:cellId : %d, ueId : %d\n", 
            cell->cellId, ue->ueId));

   /* Free SPS specific information of the UE */
   rgSCHCmnSpsUlUtlFreeSpsUe (cell, ue);
  
   /* NOTE: No dereg Req to MAC as Ue shall be deleted in MAC too */

   RETVOID;
} /* end of rgSCHCmnSpsUlUeDel */

/**
 * @brief  Handler for processing SPS LCG delete request
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlLcgDel
 *
 *     Invoked by:
 *      rgSCHCmnLcgDel for UL SPS enabled UE 
 *
 *     Processing Steps:
 *       free the allocation of the UE at the scheduled offset and
 *       in the subframes corr to the UE's UL SPS periodicity
 *       remove UE from the actvUlSpsUeLsts
 *       Deregister with MAC
 *       return ROK
 *
 * @param[in]  RgSchCellCb *cell
 * @param[in]  RgSchUeCb   *ue
 * @param[in]  RgSchLcgCb  *lcg
 * @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUlLcgDel 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUlLcgDel (cell, ue, lcg) 
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *lcg;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
  
   TRC2(rgSCHCmnSpsUlLcgDel); 

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlLcgDel:cell: %d, ue :%d, lcg: %d\n",
            cell->cellId, ue->ueId, lcg->lcgId));

   if (lcg->lcgId != RG_SCH_SPS_LCG_ID)
   {
      /* need not do anything */
      RETVOID;
   }

   /* Send an explicit release and when release is successful, then delete
    * the SPS information of the UE */ 
   ulSpsUe->state = RGSCH_SPS_TO_BE_REL;
   rgSCHCmnSpsUlUtlInsUeInToBeRelLst (cell, ue);

   RETVOID;
} /* end of rgSCHCmnSpsUlLcgDel */


/**
 * @brief Handler for processing UE reset request
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUeReset
 *
 *     Invoked By:
 *       rgSCHCmnUlUeReset if (ue->isUlSpsEnabled == TRUE)
 *
 *     Processing Steps:
 *       if (ue->isUlSpsActv == TRUE)
 *          remove the configured grants allocated for the UE.
 *       remove Ue from lists if it exists
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUlUeReset
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUlUeReset(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif /* ANSI */
{
    RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
    RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
    U16 ulSfIdx = (ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc.sfn * RGSCH_NUM_SUB_FRAMES + ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc.subframe) % (RG_SCH_CMN_UL_NUM_SF);

    TRC2(rgSCHCmnSpsUlUeReset);

    RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
                "rgSCHCmnSpsUlUeReset:cell: %d, ue :%d\n",
                cell->cellId, ue->ueId));

    if (ulSpsUe->state == RGSCH_SPS_TO_BE_ACTV)
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
        cmMemset((U8 *)ulSpsUe, 0, sizeof(*ulSpsUe));
        RETVOID;
    }

    if(ulSpsUe->ulSpsSchdInfo.crntAlloc)
    {
        rgSCHCmnUlFreeAllocation(
                &cellUl->ulSfArr[ulSfIdx], 
                ulSpsUe->ulSpsSchdInfo.crntAlloc);
    }
    /* Remove UE from actvtdUeLst */
    rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);

    /* Remove Ue from other lists, if it exists */
    if (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV)
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
    }
    else if (ulSpsUe->state == RGSCH_SPS_TO_BE_REL)
    {
        rgSCHCmnSpsUlDelUeFrmToBeRelLst(cell, ue);
    }

    RETVOID;
} /* end of rgSCHCmnSpsUlUeReset */

/**
 * @brief Handler for processing SPS release for a UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcRelInd
 *
 *     Invoked By:
 *       rgSCHCmnUlSpsRelInd
 *
 *     Processing Steps:
 *       rgSCHCmnUlSpsRelInd
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  Bool               isExplRel
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUlProcRelInd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                isExplRel
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUlProcRelInd(cell, ue, isExplRel)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
Bool                isExplRel;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2(rgSCHCmnSpsUlProcRelInd);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcRelInd:cell: %d, ue :%d\n",
            cell->cellId, ue->ueId));

   if (ulSpsUe->state == RGSCH_SPS_TO_BE_REL)
   {
      /* Release already handled */
      RETVOID;
   }

   if (isExplRel == TRUE)
   {
      /* Send an explicit release and when release is successful, then delete
       * the SPS information of the UE */ 
      ulSpsUe->state = RGSCH_SPS_TO_BE_REL;
      rgSCHCmnSpsUlUtlInsUeInToBeRelLst (cell, ue);
   }
   else
   {
      /* Handle implicit release of the UE */
      rgSCHCmnSpsUlUtlFreeSpsUe(cell, ue);
   }

   RETVOID;
} /* end of rgSCHCmnSpsUlProcRelInd */

/**
 * @brief Handler to initialize the measurement gap mask of the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUeMeasGapMaskInit
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUeCfg
 *        
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  RgrUeCfg        *ueCfg
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUeMeasGapMaskInit
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeMeasGapCfg    ueMesGapCfg
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUeMeasGapMaskInit(cell, ue, ueMesGapCfg)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeMeasGapCfg    ueMesGapCfg;
#endif /* ANSI */
{
    RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
    U8                    idx;
    S8                    startPos;   

    TRC2(rgSCHCmnSpsUlUeMeasGapMaskInit);

    RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
                "rgSCHCmnSpsUlUeMeasGapMaskInit:cell: %d, ue :%d\n",
                cell->cellId, ue->ueId));

    /* Reset the existing mask before initializing */
    for (idx=0; idx < RGSCH_SPS_MG_MASK_LEN; idx++)
    {
        ulSpsUe->measGapMask[idx] = 0;
    }

    /* Mark UL Subframes in which Measurement takes place */
    /* Calculate the starting position to be set and set n indices from there
     * starting pos =  ue's measurement offset - pdcch_pusch_gap.
     * n = no of subframes UE remains inactive in UL due to MG */
    /* We need not account for harq feedback in the mask
     * so mark only 6 subframes*/
    startPos = ueMesGapCfg.gapOffst; 
    for (idx=0; idx < (RG_SCH_MEAS_GAP_LEN); idx++)
    {
        /* Divide by 8*/
        ulSpsUe->measGapMask[((startPos + idx) >> 3 )] |= 
            (1 << ((startPos + idx) % 8));
    }
    RETVOID;
} /* end of rgSCHCmnSpsUlUeMeasGapMaskInit */

   /**
    * @brief Handler to initialize the SPS info of a UE at config/reconfig
    *
    * @details
    *
    *     Function : rgSCHCmnSpsUlUeInit
    *
    *     Invoked by:
    *      -rgSCHCmnSpsUlUeCfg
    *      -rgSCHCmnSpsUlUeRecfg
    *        
    *     Processing Steps:
    *           return 
    *
    *  @param[in]  RgSchUeCb      *ue
    *  @return  Void
    **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUeInit
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgrUeSpsCfg     spsCfg,
RgrUeMeasGapCfg measGapCfg
)
#else
PRIVATE Void rgSCHCmnSpsUlUeInit(cell, ue, spsCfg, measGapCfg)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
RgrUeSpsCfg     spsCfg;
RgrUeMeasGapCfg measGapCfg;
#endif
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2(rgSCHCmnSpsUlUeInit);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
          "rgSCHCmnSpsUlUeInit:cell: %d, ue :%d\n",
          cell->cellId, ue->ueId));
   
   ue->spsRnti = spsCfg.spsRnti;
   ue->ul.ulSpsCfg = spsCfg.ulSpsCfg;
   ue->ul.spsPrdcty= spsCfg.ulSpsCfg.ulSpsPrdctyEnum;
#ifdef LTE_TDD
   /* In case of TDD, SPS periodicity shall be rounded off to an integer. For
    * ex: 128 is considered to be 120 */
   RGSCH_SPS_GET_PRDCTY(ue->ul.ulSpsCfg.ulSpsPrdctyEnum, ue->ul.spsPrdcty);
#endif /* LTE_TDD */
   /* Calculate scaled Cqi based on the current UL Cqi */
   ulSpsUe->ulSpsSchdInfo.scaledCqi = rgSCHCmnSpsScaleUlCqi(cell, ue);
   ulSpsUe->state = RGSCH_SPS_IDLE;

   if (measGapCfg.isMesGapEnabled == TRUE)
   {
      rgSCHCmnSpsUlUeMeasGapMaskInit(cell, ue, measGapCfg);
   }
   RETVOID;

} /* end of rgSCHCmnSpsUlUeInit */

/**
 * @brief Handler to initialize the UL bandwidth bitmask in SPS subframes 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlBwMaskInit
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUpdSpsSf
 *        
 *     Processing Steps:
 *           for each subframe in cell->spsSfLst
 *              if (non-SPS bandwidth != 0)
 *                 Mark non-SPS bandwidth bits as 1
 *           return 
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlBwMaskInit
(
RgSchCellCb     *cell,
U16             lstSz
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlBwMaskInit(cell, lstSz)
RgSchCellCb     *cell;
U16             lstSz;
#endif /* ANSI */
{
   U16                   idx;
   U16                   idx1;
   U8                    maskIdx;
   RgSchCmnSpsUlSf       *spsSf;
   RgSchCmnUlCell        *cellUl    = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       *ulSpsSfLst = ulSpsCell->spsSfLst;
   U8 i=0;

   TRC2(rgSCHCmnSpsUlBwMaskInit);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlBwMaskInit:cell: %d\n", cell->cellId));

   for (idx=0; idx < lstSz; idx++)
   {
      spsSf = &ulSpsSfLst[idx];
      spsSf->maskLen = RGSCH_CEIL(cellUl->numSb,32);
      /* Mark the bits corr to non-SPS BW, if exists, as occupied */
      if (ulSpsCell->numSpsSb != cellUl->numSb)
      {
         U8  nonSpsSbToSet;
         U8  numSpsSb;
         U32 tmpMask;

    if(!ulSpsCell->numSpsSb)
    {
       for (i=0; i < RGSCH_SPS_ULBW_MASK_LEN; i++)
       {
          spsSf->ulBwBitMask[i] = 0xffffffff;
       }
       RETVALUE(RFAILED);
    }
         nonSpsSbToSet = (cellUl->numSb - ulSpsCell->numSpsSb)/2;

         /* Mark non-SPS BW at the end of the BW as occupied */
         spsSf->ulBwBitMask[0] = (U32)(((1<<nonSpsSbToSet))-1);
         if (nonSpsSbToSet > 32)
         {
            /* nonSpsSbToSet at one end of the bandwidth can't go beyond
             * 49 and hence lie within Mask[1] */
            spsSf->ulBwBitMask[1] = spsSf->ulBwBitMask[0] | 
                                        (U32)((1<<(nonSpsSbToSet-32))-1);
         }
         /* Mark non-SPS BW at the start of the BW as occupied */
         maskIdx = (ulSpsCell->numSpsSb + nonSpsSbToSet) / 32;
         numSpsSb = ((ulSpsCell->numSpsSb + nonSpsSbToSet)
                                                    - (32 * maskIdx));
         tmpMask = spsSf->ulBwBitMask[maskIdx];
         tmpMask = tmpMask >> numSpsSb;
         tmpMask = tmpMask | (U32)(((1<<nonSpsSbToSet))-1);
         tmpMask = tmpMask << numSpsSb;
         spsSf->ulBwBitMask[maskIdx] = spsSf->ulBwBitMask[maskIdx]|tmpMask;
         if ((32 - numSpsSb) < nonSpsSbToSet)
         {
            spsSf->ulBwBitMask[maskIdx+1] = spsSf->ulBwBitMask[maskIdx+1]|
                            (U32)((1<<(nonSpsSbToSet-(32 - numSpsSb)))-1);
         }
      } /* end of numSpsSb != numSb */ 
      
      /* Also mark the subbands with non-zero DMRS as occupied for SPS
       * allocations */
      for (idx1=cellUl->ulSpsInfo.spsSbStart; 
                        idx1 < cellUl->ulSpsInfo.numSpsSb; idx1++)
      {
         if (cellUl->dmrsArr[idx1] != 0)
         {
            /* Set the bit corr. to the subband as 1 */
            maskIdx = idx1 >> 5;
            spsSf->ulBwBitMask[maskIdx] |= (1 << (idx1 % 32));
         }
      }
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlBwMaskInit */


/**
 * @brief Handler to create cell wide sps subframe list
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlInitSpsSf
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlCellCfg
 *        
 *     Processing Steps:
 *              create spsSfLst of default size
 *           return 
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlInitSpsSf
(
RgSchCellCb     *cell
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlInitSpsSf(cell)
RgSchCellCb     *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       **ulSpsSfLst = &ulSpsCell->spsSfLst;

   TRC2(rgSCHCmnSpsUlInitSpsSf);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsUlInitSpsSf:cell: %d\n", cell->cellId));

   /* Allocate sps sf table */
   if ((rgSCHUtlAllocSBuf (cell->instIdx, 
                         (Data **)(ulSpsSfLst), 
                         (sizeof(RgSchCmnSpsUlSf) * RGSCH_SPS_UL_LCM))) !=ROK)
   {
      RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
                 "Memory allocation FAILED for ulSpsSfLst"));
      RETVOID;
   }

   /* Store the new LCM as the current LCM */
   ulSpsCell->spsPrdLcmVal = RGSCH_SPS_UL_LCM;

   /* Initialize the BW bitmask in the subframes */
   if ((rgSCHCmnSpsUlBwMaskInit(cell, RGSCH_SPS_UL_LCM)) == RFAILED)
   {
      /*rgSCHUtlFreeSBuf (cell->instIdx, (Data *)(*ulSpsSfLst),
                        (sizeof(RgSchCmnSpsUlSf) * RGSCH_SPS_UL_LCM));*/
      RETVOID;
   }
   

   RETVOID;
} /* end of rgSCHCmnSpsUlInitSpsSf */

/**
 * @brief Handler to create or update the cell wide sps subframe list
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUpdSpsSf
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUeCfg
 *        
 *     Processing Steps:
 *           if(cell->spsSfLst == NULLP)
 *              create spsSfLst of size = Ue's periodicity
 *           else
 *              realloc spsSflst
 *              replicate the sps information across the newly allocated memory
 *           return 
 *
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUpdSpsSf
(
RgSchCellCb     *cell,
U16             crntLcm,
U16             newLcm
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUpdSpsSf(cell, crntLcm, newLcm)
RgSchCellCb     *cell;
U16             crntLcm;
U16             newLcm;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       **ulSpsSfLst = &ulSpsCell->spsSfLst;
   RgSchCmnSpsUlSf       *spsSf = NULLP;
   RgSchCmnSpsUlSf       *tmpUlSpsSfLst;
   U8                    idx;

   TRC2(rgSCHCmnSpsUlUpdSpsSf);

   /* Reallocate spsSfLst to the new LCM size */
   if ((rgSCHUtlAllocSBuf (cell->instIdx, 
                            (Data **)(&tmpUlSpsSfLst), 
                            (sizeof(RgSchCmnSpsUlSf) * newLcm))) !=ROK)
   {
      RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
                 "Memory reallocation FAILED for ulSpsSfLst"));
      RETVALUE(RFAILED);
   }
   /* Replicate the sps subframe information upto newLcm */
   for(idx=0; idx < (newLcm/crntLcm); idx++)
   {
      spsSf = &tmpUlSpsSfLst[(idx * crntLcm)];
      cmMemcpy((U8 *)spsSf, (U8 *)(*ulSpsSfLst),
                                 (sizeof(RgSchCmnSpsUlSf) * crntLcm));
   }
   /* Deallocate old spsSf list and assign the newl created list */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(ulSpsSfLst),
                              (sizeof(RgSchCmnSpsUlSf) * crntLcm));
   *ulSpsSfLst = tmpUlSpsSfLst;

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUpdSpsSf */

/**
 * @brief Function to handle BSR report 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsBsrRpt
 *
 *     Handler for processing BSR reported by UE 
 *
 *     Invoked by: 
 *         - rgSCHCmnUpdBsrShort
 *         - rgSCHCmnUpdBsrTrunc
 *         - rgSCHCmnUpdBsrLong 
 *       
 *
 *     Processing Steps: 
 *         if(ulLcg->lcgId == RG_SCH_SPS_LCG_ID) && (ue->isUlSpsEnabled == TRUE)
 *            if(ue->isUlSpsActv == FALSE)
 *               add to the "tobeActvtd" SPS UE List
 *         else
 *            return
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgSchUeCb    *ue
 *  @param[in] RgSchLcgCb   *ulLcg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsBsrRpt
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsBsrRpt(cell, ue, ulLcg)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb   *ulLcg;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2(rgSCHCmnSpsBsrRpt);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsBsrRpt:cell: %d, ue :%d, lcg :%d\n",
                cell->cellId, ue->ueId, ulLcg->lcgId));

   if (ulLcg->lcgId != RG_SCH_SPS_LCG_ID)
   {
      /* need not do anything */
      RETVALUE(ROK);
   }
   /* Add to toBeActvtd Q, if not already added, if SPS is not yet activated */
   if (ulSpsUe->state == RGSCH_SPS_IDLE)
   {
      ulSpsUe->state = RGSCH_SPS_TO_BE_ACTV;
      ulSpsUe->pndngUeLstEnt.node = (PTR)ue;
      /* add UE to the "tobeActvtd" SPS UE List */
      rgSCHCmnSpsUlUtlInsUeInToBeActvLst (cell, ue);
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsBsrRpt */

/**
 * @brief Handler to process Uplink CQI Indication for a UL-SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsScaleUlCqi
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlCqiInd
 *      -rgSCHCmnSpsUlUeCfg 
 *        
 *     Processing Steps:
 *           ue->scaledCqi = cqi - RG_SCH_SPS_CQI_SCALE_FACTOR
 *           return 
 *
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  U8              cqi
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE U8 rgSCHCmnSpsScaleUlCqi
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else /* ANSI */
PRIVATE U8 rgSCHCmnSpsScaleUlCqi(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif /* ANSI */
{
   RgSchCmnUlUe      *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   U8                cqi;

   TRC2(rgSchCmnSpsScaleUlCqi);
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
  cqi = ueUl->validUlCqi;
#else
  cqi = ueUl->crntUlCqi[0];
#endif

   if (cqi > RGSCH_CMN_MIN_SCALABLE_CQI)
   {
      RETVALUE(cqi - RGSCH_SPS_CQI_SCALE_FACTOR);
   }

   RETVALUE(cqi);
} /* end of rgSCHCmnSpsScaleUlCqi */

/**
 * @brief Handler to calculate the number of subbands required for given CQI and
 *     packet size
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlCalcSbReqd
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlCqiInd
 *      -rgSCHCmnSpsUlSbAllocForUe
 *        
 *     Processing Steps:
 *       Calculate the number of subbands required for UE taking the scaled CQI
 *       into consideration  
 *
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  U8              cqi
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE U8 rgSCHCmnSpsUlCalcSbReqd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
U8                   cqi,
U8                   *iMcs,
U8                   *iTbs
)
#else /* ANSI */
PRIVATE U8 rgSCHCmnSpsUlCalcSbReqd(cell, ue, cqi, iMcs, iTbs)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
U8                   cqi;
U8                   *iMcs;
U8                   *iTbs;
#endif /* ANSI */
{
   RgSchCmnUlCell    *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe      *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   U32               maxBits = ueUl->maxBytesPerUePerTti*8; 
   U32               bits;
   U32               nPrb;
   U8                sbSize  = cellUl->sbSize;
   U8                maxRb;
   U32               eff;
   U8                numSb;
   Bool              limit = FALSE;
   /*rg001.301- ADD - [ccpu00117270]to determine UE category */
   CmLteUeCategory   ueCtg = (RG_SCH_CMN_GET_UE_CTGY(ue));

   TRC2(rgSCHCmnSpsUlCalcSbReqd);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsUlCalcSbReqd:cell: %d, ue :%d\n",
                                     cell->cellId, ue->ueId));

   *iTbs  = rgSCHCmnSpsUlGetITbs(ue, cell->isCpUlExtend);

   if (!cell->isCpUlExtend)
   {
       /*rg003.301[ccpu00119901] -MOD- changed to UL eff table*/
      eff   = rgSchCmnNorUlEff[0][*iTbs]; 
   }
   else
   {
       /*rg003.301[ccpu00119901] -MOD- changed to UL eff table*/
      eff   = rgSchCmnExtUlEff[0][*iTbs]; 
   }
   bits = ue->ul.ulSpsCfg.packetSize * 8;
   /* rg001.301- MOD- sending ueCtg to enabled correct iMcs derivation */
   *iMcs  = rgSCHCmnUlGetIMcsFrmITbs(*iTbs,ueCtg);
   maxRb = rgSCHPwrGetMaxUlRb(cell, ue);

   if (bits < rgSCHCmnUlMinTbBitsForITbs(cellUl, *iTbs))
   {
      if (!maxRb)
      {
         limit = TRUE;
         numSb = 0;
      }
      else 
      {
         numSb = 1;
      }
   }
   else
   {
      if (bits > maxBits)
      {
         bits  = maxBits;
         nPrb  = bits * 1024 / eff / RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl);
         if (nPrb > maxRb)
         {
            limit = TRUE;
            nPrb = maxRb;
         }
         numSb = nPrb / sbSize;
      }
      else
      {
         nPrb = RGSCH_CEIL((RGSCH_CEIL(bits * 1024, eff)),
               RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl));
         if (nPrb > maxRb)
         {
            limit = TRUE;
            nPrb = maxRb;
         }
         numSb = RGSCH_CEIL(nPrb, sbSize);
      }
   }
   if (!numSb)
   {
      if (limit)
      {
         RETVALUE(numSb);
      }
      else
      {
         ++numSb;
      }
   }

   RETVALUE(numSb);
} /* end of rgSCHCmnSpsUlCalcSbReqd */

/**
 * @brief Handler to process Uplink CQI Indication for a UL-SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlCqiInd
 *
 *     Invoked by:
 *      rgSCHCmnUlCqiInd when (ue->isUlSpsEnabled == TRUE)
 *        
 *     Processing Steps:
 *       if (ue->isUlSpsActv == FALSE),
 *           invoke rgSchCmnSpsScaleUlCqi(ulCqi)
 *           return ROK
 *       else
 *           newScaledCqi = rgSchCmnSpsScaleUlCqi(ulCqi)
 *           calculate number of RBs needed at ue->newScaledCqi for the
 *           configured packet size.
 *             if rbsReqd > ue->rbsAllocd 
 *                ue->sbsReqd = rbsReqd/sbSize
 *                change ue state to RGSCH_SPS_TO_BE_REACTV
 *             else do nothing
 *
 *  Notes: CQI is assumed to be updated in ue before invoking this function
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlCqiInd(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U8                    iMcs;
   U8                    iTbs;
   U8                    newSb;
   
   TRC2(rgSCHCmnSpsUlCqiInd);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsUlCqiInd:cell: %d, ue :%d\n",
                                     cell->cellId, ue->ueId));

   /* Scale CQI and store irrespective of whether UE is SPS active or not */
   ulSpsUe->ulSpsSchdInfo.scaledCqi = rgSCHCmnSpsScaleUlCqi(cell, ue);

   if (ulSpsUe->isUlSpsActv == TRUE) 
   {
      /* Calculate the new SBs required and add it to reactvUeLst if needed */
      newSb = rgSCHCmnSpsUlCalcSbReqd(cell, ue, 
                  ulSpsUe->ulSpsSchdInfo.scaledCqi, &iMcs, &iTbs);
      if (newSb != ulSpsUe->ulSpsSchdInfo.allocInfo.numSb)
      {
         /* change the state of the UE to "TO_BE_REACTV" so that reactivation
          * can be attempted in frequency domain */
         ulSpsUe->state = RGSCH_SPS_TO_BE_REACTV;
      } 
   }
   
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlCqiInd */

/**
 * @brief Handler to process the DTX indication received for a SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcDtxInd
 *
 *     Invoked by:
 *       rgSCHCmnUlCrcFailInd for a UE whose UL SPS is enabled
 *        
 *     Processing Steps:
 *       if ((ue->isUlSpsActv == FALSE) || (dtxTime != ue->actvUlSpsLstIdx))
 *          return ROK;
 *       else if (ue->ulSpsInfo.state == RGSCH_ACTV_REACTV_SENT)
 *          set ue->pdcchToRetx = TRUE;
 *         
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  CmLteTimingInfo    dtxTime
 *  @return  S16
 *
 *  Notes: Right now, dtxInd is invoked on receiving CRC failure for UL data.
 *         Can also be invoked from DtxInd, if introduced on TFU.
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlProcDtxInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      dtxTime
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlProcDtxInd(cell, ue, dtxTime)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      dtxTime;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2(rgSCHCmnSpsUlProcDtxInd);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcDtxInd:cellId : %d, ueId : %d\n", 
            cell->cellId, ue->ueId));

   /* Act only if UL SPS is active and dtxTime matches Ue's SPS occasion */
   if ((ulSpsUe->isUlSpsActv != TRUE) ||
       (!(RGSCH_TIMEINFO_SAME(ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc,
                                                                dtxTime))))
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "Either UL SPS is not active for the UE or the DTX time doesnt match "
         "with SPS occasion of the UE\n"));
      RETVALUE(ROK);
   }
   else if (ulSpsUe->state == RGSCH_SPS_ACTV_PDCCH_SENT)
   {
      /* Activation PDCCH not received by the UE. So mark to resend */
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "UL SPS (Re)Activation PDCCH not received by the UE\n"));
      ulSpsUe->pdcchToRetx = TRUE;
   }
   else if (ulSpsUe->state == RGSCH_SPS_REL_SENT)
   {
      ue->ul.explicitRelCnt++;
      if( ue->ul.explicitRelCnt == RG_SCH_NUM_SPS_OCC_AFTR_EXP_REL)
      {
      /* Explicit release is successful. Release SPS resources */
      rgSCHCmnSpsUlUtlFreeSpsUe(cell, ue);
      }
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcDtxInd */

/**
 * @brief Handler to process the CRC indication received for a SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcCrcInd
 *
 *     Invoked by:
 *       rgSCHCmnUlCrcFailInd for a UE whose UL SPS is enabled
 *        
 *     Processing Steps:
 *       if ((ue->isUlSpsActv == FALSE) || (dtxTime != ue->actvUlSpsLstIdx))
 *          return ROK;
 *       else if (ue->ulSpsInfo.state == RGSCH_SPS_REL_SENT)
 *          Add Ue to TobeRel List
 *         
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  CmLteTimingInfo    dtxTime
 *  @return  S16
 *
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlProcCrcInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      dtxTime
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlProcCrcInd(cell, ue, dtxTime)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      dtxTime;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2(rgSCHCmnSpsUlProcCrcInd);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcCrcInd:cellId : %d, ueId : %d\n", 
            cell->cellId, ue->ueId));

   /* Act only if UL SPS is active and dtxTime matches Ue's SPS occasion */
   if ((ulSpsUe->isUlSpsActv != TRUE) ||
       (!(RGSCH_TIMEINFO_SAME(ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc,
                                                                dtxTime))))
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "Either UL SPS is not active for the UE or the DTX time doesnt match "
         "with SPS occasion of the UE\n"));
      RETVALUE(ROK);
   }
   else if (ulSpsUe->state == RGSCH_SPS_REL_SENT)
   {
      ue->ul.explicitRelCnt=0;
      rgSCHCmnSpsUlUtlInsUeInToBeRelLst (cell, ue);
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcCrcInd */

/**
 * @brief Handler to get the next SPS occasion based on current sps occasion
 *
 * @details
 *
 *     Function : rgSCHCmnSpsGetNextSpsOcc
 *
 *     Invoked by:
 *        rgSCHCmnSpsUlProcActvUeInCrntSf
 *        rgSCHCmnSpsUlChkMesGapColl
 *        
 *     Processing Steps:
#ifdef LTE_TDD
 *           if(ue->twoIntervalCfg == TRUE)
 *              sfOffset = rgSchTddSfOffTbl[cell->UlDlCfgIdx][(cell->crntTime +
 *              UL_SCHD_DELTA)]
 *              nxtOffst = ((crntTime * RGSCH_NUM_SUB_FRAMES) + 
 *                   crntTime.subframe + ue->ul.spsPrdcty + 
 *                   sfOffset * (n % 2)) % 10240;
 *           else
#endif
 *              nxtOffst = ((crntTime * RGSCH_NUM_SUB_FRAMES) + 
 *                   crntTime.subframe + ue->ul.spsPrdcty) % 10240;
 *          
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo crntOcc
 *  @param[in]  CmLteTimingInfo *nxtOcc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsGetNextSpsOcc
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
CmLteTimingInfo  crntOcc,
CmLteTimingInfo  *nxtOcc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsGetNextSpsOcc(cell, ue, crntOcc, nxtOcc)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
CmLteTimingInfo  crntOcc;
CmLteTimingInfo  *nxtOcc;
#endif /* ANSI */
{
   U16                   nxtOffst;
#ifdef LTE_TDD
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
#endif /* LTE_TDD */

   TRC2(rgSCHCmnSpsGetNextSpsOcc);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsGetNextSpsOcc:cell: %d, ue :%d",
                                     cell->cellId, ue->ueId));

#ifdef LTE_TDD
   if(ue->ul.ulSpsCfg.twoIntervalCfg == TRUE)
   {
      S8                sfOffset;
      U8                nMod2 = (ulSpsUe->nMod2 ^ 1);

      sfOffset = rgSchTddSfOffTbl[cell->ulDlCfgIdx][crntOcc.subframe];
      nxtOffst = ((crntOcc.sfn * RGSCH_NUM_SUB_FRAMES) + crntOcc.subframe + 
                ue->ul.spsPrdcty + (sfOffset * nMod2)) % 10240;
   }
   else
#endif /*LTE_TDD*/
   {
      nxtOffst = ((crntOcc.sfn * RGSCH_NUM_SUB_FRAMES) + crntOcc.subframe + 
                       ue->ul.spsPrdcty) % 10240;
   }
   nxtOcc->sfn = (nxtOffst / RGSCH_NUM_SUB_FRAMES);
   nxtOcc->subframe = (nxtOffst % RGSCH_NUM_SUB_FRAMES);

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsGetNextSpsOcc */

/**
 * @brief Handler to get the next nth SPS occasion's subframe
 *
 * @details
 *
 *     Function : rgSCHCmnSpsGetNextNthSpsUlSf
 *
 *     Invoked by:
 *        rgSCHCmnSpsUlSbAlloc
 *        rgSCHCmnUlSpsMarkAlloc
 *        
 *     Processing Steps:
#ifdef LTE_TDD
 *           if(ue->twoIntervalCfg == TRUE)
 *              sfOffset = rgSchTddSfOffTbl[cell->UlDlCfgIdx][(cell->crntTime +
 *              UL_SCHD_DELTA)]
 *              futureSpsUlSf = spsSfLst[(actvOffst + (prsntidx * ue->spsPrdcty)+
 *                                          sfOffset) % cell->spsPrdLcmVal];
 *           else
 *              futureSpsUlSf = spsSfLst[(actvOffst + (prsntidx * ue->spsPrdcty)) %
 *                                                       cell->spsPrdLcmVal];
#else
 *           futureSpsUlSf = spsSfLst[(actvOffst + (prsntidx * ue->spsPrdcty)) %
 *                                                       cell->spsPrdLcmVal]
#endif
 *          
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  CmLteTimingInfo ulSpsActvOcc
 *  @param[in]  U8            prsntidx
 *  @return  RgSchCmnSpsUlSf*
 **/

#ifdef ANSI
PRIVATE RgSchCmnSpsUlSf* rgSCHCmnSpsGetNextNthSpsUlSf
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo ulSpsActvOcc,
U8              prsntidx
)
#else /* ANSI */
PRIVATE RgSchCmnSpsUlSf* rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, ulSpsActvOcc, prsntidx)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo ulSpsActvOcc;
U8              prsntidx;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
#ifdef LTE_TDD
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
#endif /* LTE_TDD */
   RgSchCmnSpsUlSf       *futSpsSf  = NULLP;
   U8                    actvOffst;

   TRC2(rgSCHCmnSpsGetNextNthSpsUlSf);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsGetNextNthSpsUlSf:cell: %d, ue :%d\n",
                                     cell->cellId, ue->ueId));
   actvOffst = (ulSpsActvOcc.sfn*RGSCH_NUM_SUB_FRAMES) + ulSpsActvOcc.subframe;

#ifdef LTE_TDD
   if(ue->ul.ulSpsCfg.twoIntervalCfg == TRUE)
   {
      S8                sfOffset;
      U8                nMod2 = (ulSpsUe->nMod2 ^ (prsntidx%2));

      sfOffset = rgSchTddSfOffTbl[cell->ulDlCfgIdx][ulSpsActvOcc.subframe];
      futSpsSf = &(ulSpsCell->spsSfLst[(actvOffst + 
                     (prsntidx * ue->ul.spsPrdcty)+ (sfOffset * nMod2)) %
                                            ulSpsCell->spsPrdLcmVal]);
   }
   else
#endif /*LTE_TDD*/
   {
      futSpsSf = &(ulSpsCell->spsSfLst[(actvOffst + 
                     (prsntidx * ue->ul.spsPrdcty)) % ulSpsCell->spsPrdLcmVal]);
   }

   RETVALUE(futSpsSf);
} /* end of rgSCHCmnSpsGetNextNthSpsUlSf */


/**
 * @brief Returns the "Itbs" for a given UE.
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlGetITbs
 *     Purpose:  This function returns the "Itbs" for a given UE.
 *
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchUeCb        *ue
 *  @return     U8 
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnSpsUlGetITbs 
(
RgSchUeCb        *ue,
Bool             isEcp
)
#else /* ANSI */
PRIVATE U8 rgSCHCmnSpsUlGetITbs(ue, isEcp)
RgSchUeCb        *ue;
Bool             isEcp;
#endif /* ANSI */
{
   RgSchCmnUlUe *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   /*rg001.301-ADD-[ccpu00117270]scaledCqi would be capped to maxUlCqi for 16qam UEs */
   U8               cqi    = 0;
   CmLteUeCategory  ueCtgy = (RG_SCH_CMN_GET_UE_CTGY(ue));

   TRC2(rgSCHCmnSpsUlGetITbs);

   /*rg001.301-ADD-[ccpu00117270]cap cqi if scaledCqi > maxUlCqi */
   if ( (ueCtgy != CM_LTE_UE_CAT_5) && 
        (ueUl->ulSpsInfo.ulSpsSchdInfo.scaledCqi > ueUl->maxUlCqi)
      )
   {
      cqi = ueUl->maxUlCqi;
   }
   else
   {
      cqi = ueUl->ulSpsInfo.ulSpsSchdInfo.scaledCqi;
   }

   /*rg001.301-MOD-[ccpu00117270]use cqi to determine iTbs */
   /* Consider the scaled CQI in case of SPS activated UEs */
   RETVALUE(rgSchCmnUlCqiToTbsTbl[(U8)isEcp][cqi]);
} /* end of rgSchCmnSpsUlGetITbs */

/**
 * @brief Handler for SPS SB allocation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsSbAlloc
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlSbAllocForUe 
 *        
 *     Processing Steps:
 *        fetch the spsUlSf using actvOffst as index into spsSfLst
 *        allocMask = spsUlSf->ulBwBitMask
 *        for (i=0; i <  ((spsPrdLcmVal/ue->spsPrdcty) -1); i++)
 *           futureSpsUlSf = rgSCHCmnSpsGetNextNthSpsUlSf (ue, actvOffst, i);
 *           allocMask = allocMask|futureSpsUlSf->ulBwbitMask 
 *        fetch an allocation that can accommodate UE's subband requirement
 *        using allocMask
 *        return allocation
 *          
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  U8            actvOffst
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE RgSchUlAlloc* rgSCHCmnSpsUlSbAlloc
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *sf,
U8                    numSb,
CmLteTimingInfo       timeToTx
)
#else /* ANSI */
PRIVATE RgSchUlAlloc* rgSCHCmnSpsUlSbAlloc(cell, ue, sf, numSb, timeToTx)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
RgSchUlSf             *sf;
U8                    numSb;
CmLteTimingInfo       timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlUe           *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   RgSchCmnUlCellSpsInfo  *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf        *spsSf = NULLP;
   U32                    allocMask[4] = {0}; /* to store the
                                       previous allocations across sps Sfs */
   U32                    tmpMask;
   U8                     idx;
   U8                     idx1;
   U8                     startSb = ulSpsCell->spsSbStart;
   U8                     numAvlSb=0;
   U16                    spsPrdcty = (U16)ue->ul.spsPrdcty;
   Bool                   bwAvl = FALSE;
   RgSchUlAlloc    *alloc = NULLP;

   TRC2(rgSCHCmnSpsUlSbAlloc);
  
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsUlSbAlloc:cellId : %d ue: %d\n", cell->cellId, ue->ueId));

   /* Or the BW bitmasks of UE's SPS occasions */
   for (idx=0; idx < (ulSpsCell->spsPrdLcmVal/spsPrdcty); idx++)
   {
      spsSf = rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                    timeToTx, idx);
      for(idx1=0; idx1<spsSf->maskLen; idx1++)
      {
         allocMask[idx1] = allocMask[idx1]|spsSf->ulBwBitMask[idx1];
      }
   }
   
   /* Fetch an allocation based on allocMask now */
   for (idx=0; idx < spsSf->maskLen; idx++)
   {
      tmpMask = allocMask[idx];
      for (idx1=0; idx1<32; idx1++)
      {
         if ((tmpMask & 0x01) == 0)
         {
            if (numAvlSb == 0)
               startSb = ((idx << 5) + idx1);
            numAvlSb++;
            if (numAvlSb >= numSb)
            {
               bwAvl = TRUE;
               break;
            }
         }
         else
         {
            numAvlSb = 0;
         }
         tmpMask = tmpMask >> 1;            
      }
   }
   if (bwAvl == TRUE)
   {
      /* get the alloc as numSbs are available */
      if((alloc = rgSCHCmnSpsUtlUlGetSpfcAlloc(sf, startSb, numSb)) != NULLP)
      {
         /* If reactivation, clear the previous alloc*/
         spsSf = rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                    ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc, 0);
         rgSCHCmnUlSpsClrAlloc (cell, ue, spsSf,
          &ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo);
         /* Mark the allocation in Ue's allocBwMask */
         for (idx=0; idx < RGSCH_SPS_ULBW_MASK_LEN; idx++)
         {
       /* Reset before marking. Needed if it comes for reactv here */
            ueUl->ulSpsInfo.ulSpsSchdInfo.allocBwMask[idx] = 0;
         }

         for(idx=0; idx < numSb; idx++)
         {
            ueUl->ulSpsInfo.ulSpsSchdInfo.allocBwMask[((startSb+idx)>>5)] |=
              (1 << ((startSb+idx) % 32));
         }
      }
      RETVALUE(alloc);
   } 

   RETVALUE(NULLP);
} /* end of rgSCHCmnSpsUlSbAlloc */


/**
 * @brief Handler for SPS RB allocation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlSbAllocForUe
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcToBeActvtdUe when a UE needs to be
 *        activated/reactivated. This shall pick the subbands based on the UE's
 *        requirement and the available bandwidth in the considered subframe.
 *        This shall also take care of the collision in the future subframes
 *        based on the UE's periodicity.
 *        
 *     Processing Steps:
 *        fetch the spsUlSf using actvOffst as index into spsSfLst
 *        fetch a hole that can accommodate UE's subband requirement
 *        if (hole == NULLP)
 *           return RFAILED
 *        set as many LSBs as the number of subbands allocated in a temporary U32
 *        for (i=0; i <  ((spsPrdLcmVal/ue->spsPrdcty) -1); i++)
 *           futureSpsUlSf = rgSCHCmnSpsGetNextNthSpsUlSf (ue, actvOffst, i);
 *           mask = futureSpsUlSf->ulBwBitmask >> (totalSubbands - allocation's
 *           end subband)
 *           if (mask & allocU32) 
 *              return the allocation to holeDb of spsUlSf
 *              return RFAILED
 *          
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  U8            actvOffst
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlSbAllocForUe
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *sf,
CmLteTimingInfo       timeToTx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlSbAllocForUe(cell, ue, sf, timeToTx)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
RgSchUlSf             *sf;
CmLteTimingInfo       timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe          *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchUlAlloc          *alloc=NULLP;
   RgSchPdcch            *pdcch;
   U8                    iTbs;
   U8                    iMcs;
   U8                    numSb;

   TRC2(rgSCHCmnSpsUlSbAllocForUe);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsUlSbAllocForUe:cellId : %d ue: %d\n", 
              cell->cellId, ue->ueId));
   pdcch = rgSCHCmnPdcchAllocCrntSf(cell, ue);
   if(pdcch == NULLP)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHCmnUlRbAllocForUe(): Could not get PDCCH\n"));
      RETVALUE(RFAILED);
   }

   numSb = rgSCHCmnSpsUlCalcSbReqd(cell, ue, ulSpsUe->ulSpsSchdInfo.scaledCqi,
                           &iMcs, &iTbs);

   if(numSb)
   {
   alloc = rgSCHCmnSpsUlSbAlloc(cell, ue, sf, 
                      (U8)RGSCH_MIN(numSb, cellUl->maxSbPerUe), timeToTx);
   }
                             
   if (alloc == NULLP)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHCmnUlRbAllocForUe(): Could not get UlAlloc\n"));
      rgSCHCmnPdcchRlsCrntSf(cell, pdcch);
      RETVALUE(RFAILED);
   }
   rgSCHCmnUlAllocFillRbInfo(cellUl, alloc);
   alloc->pdcch = pdcch;
   alloc->grnt.iMcs = iMcs;
   alloc->grnt.iMcsCrnt = iMcs;
   alloc->grnt.hop = 0;
   alloc->forMsg3 = FALSE;
   ueUl->alloc.allocdBytes = rgTbSzTbl[0][iTbs][alloc->grnt.numRb-1] / 8;
   alloc->grnt.datSz = ueUl->alloc.allocdBytes;
   RG_SCH_UL_MCS_TO_MODODR(iMcs, alloc->grnt.modOdr);
   alloc->grnt.isRtx = FALSE;
   alloc->hqProc = &ue->ul.hqEnt.hqProcCb[cellUl->schdHqProcIdx];
   if(alloc->hqProc->alloc)
   {
      rgSCHUtlUlAllocRls(sf,alloc->hqProc->alloc);
   }
   alloc->hqProc->alloc = alloc; 
   /*alloc->hqProc->remTx = alloc; */
   alloc->hqProc->ulSfIdx = cellUl->schdIdx;
   alloc->ue = ue;
   alloc->rnti = ue->spsRnti;
   ueUl->alloc.alloc = alloc;
   ulSpsUe->ulSpsSchdInfo.crntAlloc = alloc;
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlSbAllocForUe */

/**
 * @brief Handler to clear SPS allocations of the UE
 *
 * @details
 *
 *     Function : rgSCHCmnUlSpsClrAlloc
 *
 *     Invoked by:
 *            rgSCHCmnSpsUlProcToBeReactvtdUe
 *        
 *     Processing Steps:
 *      for (idx=0; idx < spsPrdLcmVal/ue->spsPrdcty; idx++) 
 *        fetch spsSf corr to the "idx"th SPS occasion of the UE
 *        unmark the allocation in the spsSf's bandwidth mask
 *      return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @param[in]  RgSchCmnSpsUlSf         *spsSf
 *  @param[in]  RgSchUlAlloc            *alloc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnUlSpsClrAlloc
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
RgSchUlAlloc            *alloc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnUlSpsClrAlloc(cell, ue, spsSf, alloc)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnSpsUlSf         *spsSf;
RgSchUlAlloc            *alloc;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       *crntSpsSf = spsSf;
   RgSchCmnUlUe          *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   U8                    idx;
   U8                    idx1;
   U16                   spsPrdcty = ue->ul.spsPrdcty;

   TRC2(rgSCHCmnUlSpsClrAlloc);
   
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnUlSpsClrAlloc:cellId : %d, ueId: %d\n", cell->cellId,
            ue->ueId));
  
   /* Reset bits corr to allocated subbands in all spsSfs corr to UE's 
    * SPS transmissions */

   for (idx=1; idx <= (ulSpsCell->spsPrdLcmVal/spsPrdcty); idx++)
   {
      for(idx1=0; idx1 < RGSCH_SPS_ULBW_MASK_LEN; idx1++)
      {
         crntSpsSf->ulBwBitMask[idx1] ^=
               ueUl->ulSpsInfo.ulSpsSchdInfo.allocBwMask[idx1];
      }
      crntSpsSf = rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                    ueUl->ulSpsInfo.ulSpsSchdInfo.ulSpsActvOcc, idx);
   } /* End of for loop for spsSf */

   RETVALUE(ROK);
} /* end of rgSCHCmnUlSpsClrAlloc */

/**
 * @brief Handler to mark SPS allocation of the UE across SPS subframes
 *
 * @details
 *
 *     Function : rgSCHCmnUlSpsMarkAlloc
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlUeAlloc
 *        
 *     Processing Steps:
 *      for (idx=0; idx < spsPrdLcmVal/ue->spsPrdcty; idx++) 
 *        fetch spsSf corr to the "idx"th SPS occasion of the UE
 *        if (ue's alloc collides with spsSf's existing BW allocations)
 *            return RFAILED
 *      mark the allocation in all the spsSfs that corr to UE's activation and
 *      further SPS occasions
 *      return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @param[in]  RgSchCmnSpsUlSf         *spsSf
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnUlSpsMarkAlloc
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
CmLteTimingInfo         timeToTx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnUlSpsMarkAlloc(cell, ue, spsSf, timeToTx)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnSpsUlSf         *spsSf;
CmLteTimingInfo         timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUe          *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnSpsUlSf       *crntSpsSf = spsSf;
   U8                    idx;
   U8                    idx1;
   U16                   spsPrdcty = ue->ul.spsPrdcty;

   TRC2(rgSCHCmnUlSpsMarkAlloc);
   
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnUlSpsMarkAlloc:cellId : %d, ueId: %d\n", cell->cellId,
            ue->ueId));
   /* Set bits corr to allocated subbands in all spsSfs corr to UE's activation
    * and further SPS transmissions */
   for (idx=1; idx <= (ulSpsCell->spsPrdLcmVal/spsPrdcty); idx++)
   {
      for (idx1=0; idx1 < RGSCH_SPS_ULBW_MASK_LEN; idx1++)
      {
         crntSpsSf->ulBwBitMask[idx1] |=
               ueUl->ulSpsInfo.ulSpsSchdInfo.allocBwMask[idx1];
      }
      crntSpsSf = rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                    timeToTx, idx);
   } /* End of for loop for spsSf */

   RETVALUE(ROK);
} /* end of rgSCHCmnUlSpsMarkAlloc */

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUlFillPdcchWithAlloc
 *        
 *     Desc : Fills a PDCCH with format 0 SPS Activation 
 *     information.
 *
 *     Ret  : 
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlFillPdcchWithAlloc
(
RgSchPdcch      *pdcch,
RgSchUlAlloc    *alloc,
RgSchUeCb       *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlFillPdcchWithAlloc(pdcch, alloc, ue)
RgSchPdcch      *pdcch;
RgSchUlAlloc    *alloc;
RgSchUeCb       *ue;
#endif /* ANSI */
{
#ifdef LTE_TDD
   RgSchTddANInfo    *anInfo;
   RgSchDlSf         *dlsf;
   CmLteTimingInfo   timeInfo = ue->cell->crntTime;
#endif /*LTE_TDD*/
   TRC2(rgSCHCmnSpsUlFillPdcchWithAlloc);

   pdcch->rnti = ue->spsRnti;
   pdcch->dci.dciFormat                  = TFU_DCI_FORMAT_0;
   pdcch->dci.u.format0Info.hoppingEnbld = alloc->grnt.hop;
   pdcch->dci.u.format0Info.rbStart      = alloc->grnt.rbStart;
   pdcch->dci.u.format0Info.numRb        = alloc->grnt.numRb;
   pdcch->dci.u.format0Info.mcs          = alloc->grnt.iMcsCrnt;
   pdcch->dci.u.format0Info.ndi          = alloc->hqProc->ndi;
   pdcch->dci.u.format0Info.cqiReq       = ue->dl.reqForCqi;
   /* SPS RNTI in pdcch */
   pdcch->dci.u.format0Info.tpcCmd = 0;
   pdcch->dci.u.format0Info.nDmrs = 0;
   /* MSB of mcs is set to 0 */
   pdcch->dci.u.format0Info.mcs &= 0xEF;
   /* Indicating aper Cqi has been requested */
   ue->dl.reqForCqi = FALSE;
#ifdef LTE_TDD
   /* Adjust with the scheduled time */
   if((timeInfo.subframe + TFU_DELTA) > RGSCH_NUM_SUB_FRAMES)
   {
      timeInfo.subframe = 
         (timeInfo.subframe + TFU_DELTA) % RGSCH_NUM_SUB_FRAMES;
      timeInfo.sfn = (timeInfo.sfn + 1) % RGSCH_MAX_SFN; 
   }

   /* Get DL subframe for the current time*/
   dlsf = rgSCHUtlSubFrmGet(ue->cell, timeInfo);

   /* Get ACK/NACK feedback time with respect to this current time */

   if( (ue->cell->ulDlCfgIdx != 0) || 
               (dlsf->ulAscInfo.subframe == RGSCH_INVALID_INFO))
   {
      timeInfo.sfn = 
         (timeInfo.sfn + dlsf->dlFdbkInfo.sfnOffset) % RGSCH_MAX_SFN;
      timeInfo.subframe = dlsf->dlFdbkInfo.subframe;
   }
   else
   {
      timeInfo.sfn = 
         (timeInfo.sfn + dlsf->ulAscInfo.sfnOffset) % RGSCH_MAX_SFN;
      timeInfo.subframe = dlsf->ulAscInfo.subframe;
   }
#ifdef TFU_TDD
   if(ue->cell->ulDlCfgIdx == 0)
   {
      pdcch->dci.u.format0Info.ulIdx = RG_SCH_ULIDX_MSB;
   }
   else
   {
      anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &timeInfo); 
      if(anInfo != NULLP)
      {
         pdcch->dci.u.format0Info.dai = (anInfo->ulDai) % RG_SCH_MAX_DAI_IDX + 1;
      }
      else
      {
         /* Fixing DAI value - ccpu00109162 */
         /* Set the no of DL subframes to zero with  DAI=3 */
         pdcch->dci.u.format0Info.dai = RG_SCH_MAX_DAI_IDX;
      }
   }
#endif /*TFU_TDD*/
#endif /*LTE_TDD*/
   RETVOID;
}


/**
 * @brief To fill the RgSchCmnUeUlAlloc structure of UeCb.
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlUeFillAllocInfo 
 *     Purpose:  Specific scheduler to call this API to fill the alloc
 *               information.
 *
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb      *cell
 *  @param[out] RgSchUeCb        *ue
 *  @return   Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUeFillAllocInfo 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUeFillAllocInfo(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif /* ANSI */
{
   RgSchCmnUeUlAlloc  *ulAllocInfo;
   RgSchCmnUlUe       *ueUl;

   TRC2(rgSCHCmnSpsUlUeFillAllocInfo);

   ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   ulAllocInfo = &ueUl->alloc;

   /* Fill alloc structure */
   /* Fill PDCCH */
   rgSCHCmnSpsUlFillPdcchWithAlloc(ulAllocInfo->alloc->pdcch, 
                                                   ulAllocInfo->alloc, ue);
   /* invoked after filling pdcch so that in case of adaptive retx of SPS, ndi
    * can be overwritten as 1 */
   rgSCHCmnUlAllocLnkHqProc(ue, ulAllocInfo->alloc, ulAllocInfo->alloc->hqProc, 
                              ulAllocInfo->alloc->hqProc->isRetx);

   /* Recording information about this allocation */
   rgSCHCmnUlRecordUeAlloc(cell, ue);

   /* Update the UE's outstanding allocation */
   if (!ulAllocInfo->alloc->hqProc->isRetx)
   {
      rgSCHCmnUlUpdOutStndAlloc(cell, ue, ulAllocInfo->allocdBytes);
   }

   RETVOID;
}

/**
 * @brief Handler to allocate UE during UL SPS activation of the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUeAlloc
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlProcToBeActvtdUe
 *       rgSCHCmnSpsUlProcToBeReactvtdUe
 *        
 *     Processing Steps:
 *      if (ue has pending retransmission)
 *         return RFAILED
 *      if(hole satisfying UE's requirement doesn't exist in the subframe)
 *         return RFIALED
 *      else
 *         Check for collision of SPS occasions across sps subframes 
 *         reserve hole 
 *         grab pdcch
 *         mark bits corr to the reserved subbands in ulBwMask maintained in
 *         spsSf
 *      return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @param[in]  RgSchCmnSpsUlSf         *spsSf
 *  @param[in]  RgSchCmnUlRbAllocInfo   *alloc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUeAlloc
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
RgSchCmnUlRbAllocInfo   *alloc,
RgSchUlAlloc            *ueSpsAlloc,
CmLteTimingInfo         timeToTx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUeAlloc(cell, ue, spsSf, alloc, ueSpsAlloc, timeToTx)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnSpsUlSf         *spsSf;
RgSchCmnUlRbAllocInfo   *alloc;
RgSchUlAlloc            *ueSpsAlloc;
CmLteTimingInfo         timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe          *cmnUlUe;
   RgSchUlHqProcCb       *hqProc;
   S16                   ret;

   TRC2(rgSCHCmnSpsUlUeAlloc);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlUeAlloc:cellId : %d, ueId: %d\n", cell->cellId,
            ue->ueId));

   hqProc = &ue->ul.hqEnt.hqProcCb[cellUl->schdHqProcIdx];

   /* Check if Ue's hqProc is awaiting retransmission, if so, no SPS 
    * (re)activation till retransmission completes */
   if (hqProc->isRetx)
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "Retransmission is pending for the UE(%d), hence not "
         "(re)activated now\n", ue->ueId));
      RETVALUE(RFAILED);
   }

   cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue);
   cmnUlUe->alloc.reqBytes = ue->ul.ulSpsCfg.packetSize;

   ret = rgSCHCmnSpsUlSbAllocForUe(cell, ue, alloc->sf, 
                              timeToTx);
   if (ret == ROK)
   {
      /* Mark this allocation for further sps occasions */
      rgSCHCmnUlSpsMarkAlloc(cell, ue, spsSf, timeToTx);

      /* Store scheduled alloc information for the UE */
      rgSCHCmnSpsUlUtlCpyAlloc(cmnUlUe->alloc.alloc, ueSpsAlloc);
                                            
      /* Add UE to the scheduled list of UEs in the subframe */
      rgSCHCmnUlRbAllocAddUeToLst(cell, ue, &alloc->schdUeLst);
      rgSCHCmnSpsUlUeFillAllocInfo(cell, ue);
      /* Fill activation specific params in pdcch */
   } /* Allocation for a UE succeeded */
   else
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "No allocation found for UE (%d) in subframe (%d),"
         " hence not (re)activated now\n", ue->ueId, alloc->sf->idx));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUeAlloc */

/**
 * @brief Handler to check collision of SPS occasions with measurement gap.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlChkMesGapColl
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlUeRecfg
 *       rgSCHCmnSpsUlChkUeInActv
 *        
 *     Processing Steps:
 *      for (i=0; i<lcmVal/spsPrd; i++)
 *         get the index in measGap mask corr to i'th SPS occasion
 *         if mask is set
 *         return TRUE
 *      return FALSE
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @return  Bool
 **/

#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsUlChkMesGapColl
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
CmLteTimingInfo         timeToTx,
U8                      gapPrd
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsUlChkMesGapColl(cell, ue, timeToTx, gapPrd)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
CmLteTimingInfo         timeToTx;
U8                      gapPrd;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U16                   spsPrdcty = ue->ul.spsPrdcty;
   U8                    tmpIdx;
   U8                    idx;
   U16                   lcm;
   CmLteTimingInfo       timeToTx1 = timeToTx;

   TRC2(rgSCHCmnSpsUlChkMesGapColl);
   
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsUlChkMesGapColl:cellId : %d ue: %d\n", 
              cell->cellId, ue->ueId));
   /* Calculate the starting bit position to be checked for */
   tmpIdx = ((timeToTx1.sfn * RGSCH_NUM_SUB_FRAMES) + 
                   (timeToTx1.subframe)) % (gapPrd);

   lcm = rgSCHCmnSpsCompLcm(spsPrdcty, gapPrd);

   for (idx=0; idx < (lcm/spsPrdcty); idx++)
   {
      if (ulSpsUe->measGapMask[tmpIdx >> 3] & (1 << (tmpIdx % 8)))
      {
         RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "(%d)th transmission is colliding due to measurement gap for UE"
            " (%d)\n", idx+1, ue->ueId)); 
         RETVALUE(TRUE);
      }
      rgSCHCmnSpsGetNextSpsOcc(cell, ue, timeToTx1, &timeToTx1);
      tmpIdx = ((timeToTx1.sfn * RGSCH_NUM_SUB_FRAMES) + 
                   (timeToTx1.subframe)) % (gapPrd);
   }
   RETVALUE(FALSE);
} /* end of rgSCHCmnSpsUlChkMesGapColl */

/**
 * @brief Handler to fetch an allocation for an active UE in the uplink subframe
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFetchAllocInUlSf
 *
 *     Invoked by:
 *         rgSCHCmnSpsUlMarkActvUeAllocs
 *         rgSCHCmnSpsUlProcToBeActvtdUe
 *         rgSCHCmnSpsUlProcToBeReactvtdUe
 *        
 *     Processing Steps:
 *            Fetch an uplink allocation in the given subframe based on
 *             the SPS grant allocated for the UE
 *            Fill the fetched allocation with SPS grant information
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFetchAllocInUlSf
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *ulSf
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFetchAllocInUlSf(cell, ue, ulSf)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
RgSchUlSf             *ulSf;
#endif /* ANSI */
{
   RgSchCmnUlUe          *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchUlAlloc          *alloc;
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   TRC2(rgSCHCmnSpsUlFetchAllocInUlSf);
   
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsUlFetchAllocInUlSf:cellId : %d \n", 
              cell->cellId));

   /* create allocation for the configured grant of the UE in the ulSf. No
    * need to check for collision as this will be the first marking on the
    * subframe */
   alloc = rgSCHCmnSpsUtlUlGetSpfcAlloc(ulSf, 
                       ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo.sbStart,
                       ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo.numSb);
#if (ERRCLASS & ERRCLS_DEBUG)
   /* should never come here */
   if (alloc == NULLP)
   {
      RGSCHLOGERROR(cell->instIdx, ERRCLS_DEBUG, ERG012, 0, "Unable to find alloc"
                                " for the UE's configured UL grant");
      RETVALUE(RFAILED);
   }
#endif /*(ERRCLASS & ERRCLS_DEBUG)*/
   /* Copy previous allocation information into this and store */
   {
       RgSchUlAlloc    *prvAlloc = &ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo;

       /*rgSCHCmnSpsUlUtlCpyAlloc (prvAlloc, alloc);*/
       ueUl->ulSpsInfo.ulSpsSchdInfo.crntAlloc = alloc;
       rgSCHCmnUlAllocFillRbInfo(cellUl, alloc);
       alloc->grnt.hop = 0;
       alloc->forMsg3 = FALSE;
       alloc->grnt.isRtx = FALSE;
       alloc->grnt.iMcs = prvAlloc->grnt.iMcs;
       alloc->grnt.iMcsCrnt = prvAlloc->grnt.iMcsCrnt;
       alloc->grnt.modOdr = prvAlloc->grnt.modOdr;
       alloc->grnt.datSz = prvAlloc->grnt.datSz;
       /*   RG_SCH_UL_MCS_TO_MODODR(alloc->grnt.iMcs, alloc->grnt.modOdr);*/
       alloc->ue = ue;
       alloc->rnti = ue->spsRnti;
       /* No pdcch for configured UL grant */
       alloc->pdcch = NULLP;
       alloc->hqProc=NULLP;
       rgSCHCmnSpsUlUtlCpyAlloc (alloc, prvAlloc);
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFetchAllocInUlSf */

/**
 * @brief Handler to process UL SPS activated UEs whose SPS occasion is in
 *        the current subframe 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcActvUeInCrntSf
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcActvtdUes
 *        
 *     Processing Steps:
 *      Calculate crntIndex into the spsSfLst maintained in ulCellCb as
 *      (10*crntSfn + crntSubframe)%(cell->ul.ulSpsInfo.spsPrdLcmVal)
 *      fetch the list of UEs whose SPS Occasion is the current subframe
 *       (crntIndex - 1)%(RG_SCH_CMN_SPS_MAX_PRD) from actvUlSpsUeLsts
 *      for each UE
 *       calculate its next SPS Occasion and move it in actvLst
#ifdef LTE_TDD
 *       toggle nMod2 value
#endif
 *      return ROK              
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcActvUeInCrntSf
(
RgSchCellCb             *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcActvUeInCrntSf(cell)
RgSchCellCb             *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe;
   U16                   actvUeLstIdx;
   CmLListCp             *actvUeLstCp;
   RgSchUeCb             *ue;
   RgSchCmnUlUe          *ueUl;
   CmLList               *ueLnk;

   TRC2(rgSCHCmnSpsUlProcActvUeInCrntSf);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcActvUeInCrntSf:cellId : %d\n", cell->cellId));

   /* Fetch the list of UEs whose SPS occasion is the current subframe */
   actvUeLstIdx = (((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES) + 
              cell->crntTime.subframe)) % (RG_SCH_CMN_SPS_MAX_PRD);

   actvUeLstCp = &ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx];

   ueLnk = actvUeLstCp->first;

   for(;ueLnk; ueLnk=actvUeLstCp->first)
   {
      ue = (RgSchUeCb *)(ueLnk->node);
      ueUl = RG_SCH_CMN_GET_UL_UE(ue);
      ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
#ifdef LTE_TDD
      /* Toggle the nMod2 value of the UE */
      ulSpsUe->nMod2 ^= 1;
#endif /* LTE_TDD */
      /*The current allocation is used up at the SPS allocation
       * so set it it to NULLP*/
      ulSpsUe->ulSpsSchdInfo.crntAlloc = NULLP;
      /* compute the UE's next SPS occasion and move UE to the corr
         offset in actvdUeLst. Delete from previous occasion and insert
         in next SPS occasion */

      rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, 
                                     ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);

      rgSCHCmnSpsGetNextSpsOcc(cell, ue, ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc,
                                     &ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc); 
      rgSCHCmnSpsUlInsUeInActvLst (cell, ue, 
                                     ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcActvUeInCrntSf */

/**
 * @brief Handler to process  the next SPS occasion of UL SPS activated UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcActvUeNxtOcc
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcActvtdUes
 *        
 *     Processing Steps:
 *      Calculate crntIndex into the spsSfLst maintained in ulCellCb as
 *      (10*crntSfn + crntSubframe)%(cell->ul.ulSpsInfo.spsPrdLcmVal)
 *      fetch the list of UEs whose SPS occasion is at the current calculated 
 *      index + UL_SCHD_DELTA by indexing actvUeLst with
 *            ((crntIndex + UL_SCHD_DELTA) % (RG_SCH_CMN_SPS_MAX_PRD))
 *      for each UE in the list
 *          if (ue->state == REACTV)
 *             invoke rgSCHCmnSpsUlProcToBeReactvtdUe
 *          if (ue->pdcchToRetx == TRUE)
 *          invoke rgSCHCmnPdcchAllocCrntSf to get a pdcch 
 *          fill the allocation information same as the previous one sent
 *          set the pdcch members as per activation/reactivation 
 *          set ue->pdcchToRetx = FALSE
 *          ue->state = RGSCH_ULSPS_ACTV
 *      return               
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo   *allocInfo
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcActvUeNxtOcc
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcActvUeNxtOcc(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe;
   U16                   actvUeLstIdx;
   CmLListCp             actvUeLstCp;
   RgSchUeCb             *ue;
   RgSchCmnUlUe          *ueUl;
   CmLList               *ueLnk;
   CmLteTimingInfo       timeInfo;
   U8              hqProcIdx;
   RgSchUlAlloc    *alloc;    /*!< Allocation information of the UE */
   RgSchUlSf      *reTxSf;
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   
   TRC2(rgSCHCmnSpsUlProcActvUeNxtOcc);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcActvUeNxtOcc:cellId : %d\n", cell->cellId));

   /* Fetch the list of UEs whose SPS occasion is at crntTime + uplink 
    * scheduling delta */
#ifndef LTE_TDD
   actvUeLstIdx = (((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES) + 
              cell->crntTime.subframe + RGSCH_SPS_UL_DELTA))
                                                   % (RG_SCH_CMN_SPS_MAX_PRD);
#else
   actvUeLstIdx = cellUl->schdTti % RG_SCH_CMN_SPS_MAX_PRD;
#endif

   actvUeLstCp = ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx];

   ueLnk = actvUeLstCp.first;
   RGSCHCMNADDTOCRNTTIME(cell->crntTime, timeInfo, RGSCH_SPS_UL_DELTA)

   for(; ueLnk; ueLnk=ueLnk->next)
   {
       ue = (RgSchUeCb *)(ueLnk->node);
       ueUl = RG_SCH_CMN_GET_UL_UE(ue);
       ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
       /*Link the Harq Proc Start*/
       alloc = ulSpsUe->ulSpsSchdInfo.crntAlloc;
       hqProcIdx = cellUl->spsUlRsrvHqProcIdx; 
       alloc->hqProc =  &ue->ul.hqEnt.hqProcCb[hqProcIdx];
       if(alloc->hqProc->isRetx == TRUE)
       {
           reTxSf = &cellUl->ulSfArr[cellUl->schdIdx]; 
           cmLListDelFrm(&reTxSf->reTxLst, &alloc->hqProc->reTxLnk);
           alloc->hqProc->reTxLnk.node = NULLP;

       }
       rgSCHCmnUlAllocLnkHqProc(ue, alloc, alloc->hqProc, 
               FALSE);
       /*alloc->hqProc->alloc = alloc;*/
       alloc->hqProc->ulSfIdx = (cellUl->schdIdx);/* % (RG_SCH_CMN_UL_NUM_SF);*/
       ulSpsUe->ulSpsSchdInfo.allocInfo.hqProc = alloc->hqProc;
       ueUl->alloc.alloc = alloc;
       ueUl->alloc.allocdBytes = alloc->grnt.datSz;
       /*Link the Harq Proc end*/
       ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc = 
           ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc;
       if ((ulSpsUe->pdcchToRetx == FALSE) &&
               (ulSpsUe->state == RGSCH_SPS_ACTV_PDCCH_SENT))
       {
           ulSpsUe->state = RGSCH_SPS_ACTV;

       }

       /* Do nothing if Ue is inactive at the next SPS occasion. This can happen
        * because of reconfiguration in between the SPS occasions of the UE */
       if ((rgSCHCmnSpsUlChkUeInActv(cell, ue)) == TRUE)
       {
           RGSCHDBGINFO (cell->instIdx, (rgSchPBuf(cell->instIdx),
                       "Ue (%d) not active for the next sps occasion\n", 
                       ue->ueId));
           continue;
       }

       ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
       if (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV)
       {
           rgSCHCmnSpsUlProcToBeReactvtdUe (cell, ue, allocInfo);
       }
       else if (ulSpsUe->pdcchToRetx == TRUE)
       {
           /* Fetch a pdcch and send out the release */
           RgSchPdcch *pdcch = rgSCHCmnPdcchAllocCrntSf(cell, ue);
           if (pdcch == NULLP)
           {
               RGSCHDBGERR (cell->instIdx, (rgSchPBuf(cell->instIdx),
                           "rgSCHCmnSpsUlProcActvtdUes:PDCCH unavailable to retransmit"
                           " for UE(%d)\n", ue->ueId));
               RETVALUE(RFAILED);
           }
           /* Fill PDCCH params same as the ones sent before */
           rgSCHCmnUlFillPdcchWithAlloc (pdcch, 
                   &ulSpsUe->ulSpsSchdInfo.allocInfo, ue);
           ueUl->alloc.alloc->pdcch = pdcch;
           ueUl->alloc.alloc->forMsg3 = FALSE; 
           ulSpsUe->pdcchToRetx = FALSE;
       }

   } /* end of while */

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcActvUeNxtOcc */

/**
 * @brief Handler to mark SPS allocations of the activated UEs in uplink
 * subframe to be scheduled
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlMarkActvUeAllocs
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcActvtdUes
 *        
 *     Processing Steps:
 *      Calculate crntIndex into the spsSfLst maintained in ulCellCb as
 *      (10*crntSfn + crntSubframe)%(cell->ul.ulSpsInfo.spsPrdLcmVal)
 *      fetch the list of UEs whose SPS Occasion is at the current calculated
 *      index + TFU_DELTA + PDCCH_PUSCH_DELTA +DL_DELTA + 1 by indexing  
 *            actvUeLst with ((crntIndex + 10) % (RG_SCH_CMN_SPS_MAX_PRD))
 *      for each UE in the list
 *          create allocation for the configured grant of the UE in the ulSf
 *          This shall take care of the collision of msg3 allocations/non-adaptive
 *          retx with configured grants, if any. This shall also take care 
 *          of the reception requests being sent to PHY at the expected time
 *           for the configured UL grants
 *      return        
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlMarkActvUeAllocs
(
RgSchCellCb             *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlMarkActvUeAllocs(cell)
RgSchCellCb             *cell;
#endif /* ANSI */
{
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   U16                   actvUeLstIdx;
   CmLListCp             actvUeLstCp;
   RgSchUeCb             *ue;
   RgSchCmnUlUe          *ueUl;
   CmLList               *ueLnk;
   RgSchUlSf             *ulSf;
#ifdef LTE_TDD
   U8                    offst;
   CmLteTimingInfo       timeInfo;
#else /*LTE_TDD*/
   U8                    ulSfIdx;
#endif/*LTE_TDD*/

   TRC2(rgSCHCmnSpsUlMarkActvUeAllocs);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlMarkActvUeAllocs:cellId : %d\n", cell->cellId));

#ifndef LTE_TDD
   /* Fetch the list of UEs whose SPS Occasion is at current time +
      TFU_DELTA + PDCCH_PUSCH_DELTA +DL_DELTA + 1,i.e., 10 subframes ahead so
      that collision of msg3 allocations/non-adaptive retx with configured 
      grants, if any and the reception requests to be sent to PHY for the
      configured grants can be taken care of */
   actvUeLstIdx = ((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES) + 
              cell->crntTime.subframe + RGSCH_SPS_UL_DELTA
               + RG_SCH_CMN_DL_DELTA + 1) % RG_SCH_CMN_SPS_MAX_PRD;
#else /* LTE_TDD */
   RGSCHADDTOCRNTTIME(cell->crntTime, timeInfo, RG_SCH_CMN_DL_DELTA);
   if (!(offst = rgSchTddSpsUlRsrvTbl[cell->ulDlCfgIdx][timeInfo.subframe]))
   {
      RETVALUE(ROK);
   }
   actvUeLstIdx = ((timeInfo.sfn * RGSCH_NUM_SUB_FRAMES) + 
              timeInfo.subframe + offst) % RG_SCH_CMN_SPS_MAX_PRD;
#endif /* LTE_TDD */

   actvUeLstCp = ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx];

   ueLnk = actvUeLstCp.first;

   for(; ueLnk; ueLnk = ueLnk->next)
   {
      ue = (RgSchUeCb *)(ueLnk->node);
      ueUl = RG_SCH_CMN_GET_UL_UE(ue); 

      /* later remove this to use sps idx computed every TTI once
       * it works fine */
#ifndef LTE_TDD
      /* Fetch the UL subframe that shall be scheduled 10 subframes later */
      ulSfIdx = (cellUl->schdIdx + (U8)4) % (RG_SCH_CMN_UL_NUM_SF);

      ulSf = &cellUl->ulSfArr[ulSfIdx];
#else /*LTE_TDD*/

      ulSf = &cellUl->ulSfArr[cellUl->spsUlRsrvIdx];
#endif /*LTE_TDD*/

      rgSCHCmnSpsUlFetchAllocInUlSf(cell, ue, ulSf);

   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlMarkActvUeAllocs */

/**
 * @brief Handler to process UL SPS activated UEs at the time of TTI
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcActvtdUes
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlTti
 *        
 *     Processing Steps:
 *        Invoke rgSCHCmnSpsUlProcActvUeInCrntSf to process the UEs whose 
 *        SPS occasion is in current subframe
 *        Invoke rgSCHCmnSpsUlProcActvUeNxtOcc to process the next SPS 
 *        occasion of the UL SPS activated UEs
 *        Invoke rgSCHCmnSpsUlMarkActvUeAllocs to mark the allocations in 
 *        ulSf for the activated UEs
 *        return       
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcActvtdUes
(
RgSchCellCb             *cell,
Bool                    ulSched,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcActvtdUes(cell, ulSched, allocInfo)
RgSchCellCb             *cell;
Bool                    ulSched;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUlProcActvtdUes);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcActvtdUes:cellId : %d\n", cell->cellId));

   /* Process the UEs whose SPS occasion is in current subframe */
   rgSCHCmnSpsUlProcActvUeInCrntSf(cell);

   /* Process the next SPS occasion of the UL SPS activated UEs */
   if (ulSched)
   {
      rgSCHCmnSpsUlProcActvUeNxtOcc(cell, allocInfo);
   }

   /* Mark the allocations in ulSf for the activated UEs */
   rgSCHCmnSpsUlMarkActvUeAllocs(cell);

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcActvtdUes */


/**
 * @brief Handler to process UEs which are waiting for UL SPS release
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcToBeRelUe
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlTti
 *        
 *     Processing Steps:
 *       fetch the first UE in the "toBeRelUeLst"
 *       invoke rgSCHCmnPdcchAllocCrntSf to fetch a PDCCH to send release
 *       if (pdcch == NULLP)
 *          return RFAILED;
 *       else
 *          fill the pdcch with release specific information
 *          set ue->ulSpsInfo.state = REL_SENT
 *       
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcToBeRelUe
(
RgSchCellCb             *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcToBeRelUe(cell)
RgSchCellCb             *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe;
   CmLList               *ueLnk;
   RgSchUeCb             *ue;
   RgSchPdcch            *pdcch;
   RgSchCmnUlUe          *ueUl; 

   TRC2(rgSCHCmnSpsUlProcToBeRelUe);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcToBeRelUes:cellId : %d\n", cell->cellId));

   /* Fetch the first UE waiting for UL SPS activation */
   ueLnk = cmLListFirst(&ulSpsCell->toBeRelUeLst);
   if ((ueLnk == NULLP) || ((ue = (RgSchUeCb *)(ueLnk->node)) == NULLP))
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "No Ue is pending for UL SPS release in cell %d\n", cell->cellId));
      RETVALUE(ROK);
   }

   RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), "UE(%d) considered "
               "for UL SPS release in cell %d\n", ue->ueId, cell->cellId));

   for(;ueLnk; ueLnk= ueLnk->next)
   {
      /* Check if Ue is inactive for UL scheduling */
      if (ue->ul.ulInactvMask)
      {
         RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "Ue (%d) inactive mask is set\n", ue->ueId));
         continue;
      } /* end of (ue->inactvmask) */

      /* Fetch a pdcch and send out the release */
      pdcch = rgSCHCmnPdcchAllocCrntSf(cell, ue);
      if (pdcch == NULLP)
      {
         ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
         RGSCHDBGERR (cell->instIdx, (rgSchPBuf(cell->instIdx),
           "rgSCHCmnSpsUlProcToBeRelUe:PDCCH unavailable to send SPS release"
           " for UE(%d)\n", ue->ueId));
         /* Move Ue to the end of the Q */
         rgSCHCmnSpsUlUtlInsUeInToBeRelLst (cell, ue);
         RETVALUE(RFAILED);
      }
      /* Fill release PDCCH */
      rgSCHCmnSpsUlUtlFillRelPdcch(cell, ue, pdcch);

      ueUl = RG_SCH_CMN_GET_UL_UE(ue);
      ueUl->ulSpsInfo.state = RGSCH_SPS_REL_SENT;
      rgSCHCmnSpsUlDelUeFrmToBeRelLst(cell, ue);
      /*Breaking here to process only one UE per TTI*/
      break;
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcToBeRelUe */

/**
 * @brief Handler to process UEs which are waiting for UL SPS reactivation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcToBeReactvtdUe
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlTti
 *        
 *     Processing Steps:
 *      fetch the subframe information from the "spsSfLst" with index as 
 *      ((crntSfn*10)+crntSubframe + UL_SCHD_DELTA)%spsPrdLcmVal
 *      invoke rgSCHCmnRbAllocForUe to fetch  an allocation from subframe's 
 *      spsAllocDb for ueUlSpsInfo.sbsReqd
 *      if (alloc == NULLP)
 *         ue->state = TO_BE_REACTV
 *         add Ue to cellUlSpsInfo.toBeActvtdUeLst
 *         return RFAILED
 *      ue->rbsallocd = alloc->numRbs
 *      remove UE's  old allocation from the subframes
 *      copy the allocation in all the subframes corresponding to the UE's SPS
 *      periodicity
 *      return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo   *alloc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcToBeReactvtdUe
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnUlRbAllocInfo   *alloc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcToBeReactvtdUe(cell, ue, alloc)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnUlRbAllocInfo   *alloc;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   CmLteTimingInfo       timeToTx;
   U16                   spsSfIdx; /* Index into the SPS Subframes Array */
   RgSchCmnSpsUlSf       *spsSf;   /* UL SPS information of a subframe */
   RgSchUlAlloc          tmpUeAlloc;

   TRC2(rgSCHCmnSpsUlProcToBeReactvtdUe);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcToBeReactvtdUe:cellId : %d ueId: %d\n",
            cell->cellId, ue->ueId));

   /* Calculate timingInfo that UE needs to transmit in, if Ue reactivation
    * succeeds */
   RGSCHADDTOCRNTTIME(cell->crntTime, timeToTx, RGSCH_SPS_UL_DELTA);

   /* Fetch the sps subframe corresponding to the UE's SPS occasion which is
    * away by 6 (UL scheduling delta) subframes from the current subframe */
   spsSfIdx = ((timeToTx.sfn * RGSCH_NUM_SUB_FRAMES) + (timeToTx.subframe)) %
                                         ulSpsCell->spsPrdLcmVal;
   spsSf = &(ulSpsCell->spsSfLst[spsSfIdx]); 

   /* Fetch an allocation for the new RB requirement of Ue in the subframe */
   if ((rgSCHCmnSpsUlUeAlloc (cell, ue, spsSf, alloc, &tmpUeAlloc, timeToTx))!=
                                                         ROK)
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "Ue (%d) allocation failed and hence reactivation at the same offset "
         "failed\n", ue->ueId));
      /* New RB requirement of the UE not satisfied. Move it to activation Q.
       * State is already set to TO_BE_REACTV */
      rgSCHCmnSpsUlUtlInsUeInToBeActvLst(cell, ue);
      RETVALUE(RFAILED);
   }     

   ulSpsUe->state = RGSCH_SPS_ACTV_PDCCH_SENT;
   /* Remove the previous existing allocation of the Ue from the SPS subframe
    * and the subframe which is getting scheduled in this tti */
   /*rgSCHCmnUlSpsClrAlloc (cell, ue, spsSf,&ulSpsUe->ulSpsSchdInfo.allocInfo);*/
   /* Release previous allocation from the scheduling subframe's allocDb */
  /*rgSCHUtlUlAllocRls(alloc->sf,ulSpsUe->ulSpsSchdInfo.allocInfo.hqProc->alloc);*/
   /* Update the allocation into ueCb */
   rgSCHCmnSpsUlUtlCpyAlloc (&tmpUeAlloc, &ulSpsUe->ulSpsSchdInfo.allocInfo);

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcToBeReactvtdUe */

/**
 * @brief Handler to process UEs which are waiting for UL SPS activation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcToBeActvtdUe
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlTti. For TDD, this shall be invoked only when the subframe
 *       to be scheduled is an UL subframe
 *        
 *     Processing Steps:
 *       fetch the first UE in toBeActvtdUeLst of cell
 *         if Ue is inactive due to DRX, 
 *             add UE to the end of toBeActvtdUes list
 *             return RFAILED
 *         if (ue->isHdFdd == TRUE)
 *           query HD-FDD module to check if UE is eligible for UL transmission
 *           at subframe = crntTime + UL_SCHD_DELTA
 *           if (retVal == FALSE)
 *             add UE to the end of toBeActvtdUes list
 *             return RFAILED
 *       fetch the subframe information from the "spsSfLst" with index as 
 *       ((crntSfn*10) + crntSubframe + UL_SCHD_DELTA)%spsPrdLcmVal
 *         if (ue->is measGapEnabled == TRUE)
 *           check collision with measurement gap using measGapMask for 
 *           current and future SPS ocassions
 *           if (collision == TRUE)
 *             add UE to the end of toBeActvtdUes list
 *             return RFAILED
 *       invoke rgSCHCmnSpsUlUeAlloc to fetch an allocation from subframe's
 *       spsAllocDb for ueUlSpsInfo.sbsReqd
 *       if (alloc == NULLP)
 *          add UE to the end of toBeActvtdUes list
 *          return RFAILED 
 *       Mark the allocation in the current and future SPS occaisons using the
 *       UE's SPS periodicity
 *       ue->isUlSpsActv = TRUE
 *       ue->state = RGSCH_ACTV_REACTV_SENT
 *       add UE to actvUlSpsUeLsts at the selected index.
 *       remove UE from toBeActvtdUes list
 *       if (ue->state == TO_BE_REACTV)
 *         remove UE from the old index in actvUlSpsUeLsts
 *         remove UE's old allocation from the subframes corr to its SPS
 *         occasions
 *       else
 *         register the SPS-Rnti and the set of lcIds given as SPS LCs with
 *         MAC. Invoke RgSchMacSpsLcRegReq() for this
 *       return ROK
 *
 *  Notes: Only one UE's activation shall be attempted per TTI
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return     S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcToBeActvtdUe
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcToBeActvtdUe(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *alloc;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe;
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   CmLList               *ueLnk;
   RgSchCmnUlUe          *ueUl; 
   RgSchUeCb             *ue;
   RgSchCmnUlUe          *cmnUlUe; 
   RgSchCmnSpsUlSf       *spsSf;   /* UL SPS information of a subframe */
   CmLteTimingInfo       timeToTx; /* initial UL SPS transmission time */
   U16                   spsSfIdx; /* Index into the SPS Subframes Array */
   U8                    idx;      /* Counter variable */
   Pst                   pst;
   RgInfSpsLcInfo        lcInfo;
   RgSchUlAlloc          tmpUeAlloc;

   TRC2(rgSCHCmnSpsUlProcToBeActvtdUe);

   cmMemset((U8*)&tmpUeAlloc, 0, sizeof(RgSchUlAlloc));
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlProcToBeActvtdUe:cellId : %d\n", cell->cellId));

   /* Fetch the first UE waiting for UL SPS activation */
   ueLnk = cmLListFirst(&ulSpsCell->toBeActvtdUeLst);
   if ((ueLnk == NULLP) || ((ue = (RgSchUeCb *)(ueLnk->node)) == NULLP))
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "No Ue is pending for UL SPS activation in cell %d\n", cell->cellId));
      RETVALUE(ROK);
   }
   for(;ueLnk; ueLnk= ueLnk->next)
   {

      cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue);
      ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
      ueUl = RG_SCH_CMN_GET_UL_UE(ue);

      /* calculate the time to Transmit */
      RGSCHADDTOCRNTTIME(cell->crntTime, timeToTx, RGSCH_SPS_UL_DELTA);

      /* Check if UE is inactive for UL scheduling */
      if ((rgSCHCmnSpsUlChkUeInActv (cell, ue)) == TRUE)
      {
         RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
           "Ue (%d) is inactive for uplink scheduling\n", ue->ueId));
         continue;
      }
#ifdef LTEMAC_DRX
      if(ue->isDrxEnabled)
      {
        /*if UE is not on onduration then dont schedule it*/
         if(RG_SCH_DRX_UL_IS_UE_ONDUR_INACTIVE(ue->drxCb))
         {
           RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
           "Ue (%d) is inactive for uplink scheduling due to DRX\n", ue->ueId));
           continue;
         }
      }
#endif

      /* Fetch the SPS information corresponding to the subframe in which UL
       * transmission shall happen for the UE, if activation succeeds */
      spsSfIdx = ((timeToTx.sfn * RGSCH_NUM_SUB_FRAMES) + (timeToTx.subframe)) %
                     ulSpsCell->spsPrdLcmVal;
      spsSf = &(ulSpsCell->spsSfLst[spsSfIdx]);

      /* Try to allocate UE in this subframe */
      if ((rgSCHCmnSpsUlUeAlloc (cell, 
                      ue, spsSf, alloc, &tmpUeAlloc, timeToTx)) != ROK)
      {
         RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
           "Ue (%d) allocation failed and hence activation failed\n", 
           ue->ueId));
         /* rgSCHCmnSpsUlUtlInsUeInToBeActvLst(cell, ue);*/
         RETVALUE(RFAILED);
      }

      /* Handle the case where UE is reactivated in time domain */ 
      if (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV)
      {
    /* Remove UE from the old index in actvUlSpsUeLsts */
         rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, 
          ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);
    /* Remove the previous existing allocation of the Ue from the SPS subframe
     * and the subframe which is getting scheduled in this tti */
         rgSCHCmnUlSpsClrAlloc (cell, ue,
          &(ulSpsCell->spsSfLst[ulSpsUe->ulSpsSchdInfo.actvSfLstIdx]),
          &ulSpsUe->ulSpsSchdInfo.allocInfo);
    /* Release previous allocation from the scheduling subframe's allocDb */ 
         if(ulSpsUe->ulSpsSchdInfo.allocInfo.hqProc->alloc)
         {
            rgSCHUtlUlAllocRls(
             &cellUl->ulSfArr[ulSpsUe->ulSpsSchdInfo.allocInfo.hqProc->ulSfIdx],
                ulSpsUe->ulSpsSchdInfo.allocInfo.hqProc->alloc);
         }
      }

      /* Mark the state of the UE to ACTV_SENT so that if no data comes, pdcch 
       * can be retransmitted at the next SPS occasion */
      ulSpsUe->state = RGSCH_SPS_ACTV_PDCCH_SENT;
      ulSpsUe->ulSpsSchdInfo.actvSfLstIdx = spsSfIdx;
      ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc = timeToTx;
      ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc = timeToTx;
      ulSpsUe->isUlSpsActv = TRUE;
      rgSCHCmnSpsUlUtlCpyAlloc(&tmpUeAlloc, &ulSpsUe->ulSpsSchdInfo.allocInfo);

      /* Add UE to the cell's sps List */
      rgSCHDbmInsSpsUeCb(cell, ue);

      /* calculate the time to Transmit */
      /* Add UE to actvUlSpsUeLsts */
      rgSCHCmnSpsUlInsUeInActvLst(cell, ue, timeToTx);
      rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);

      /* Register the SPS-Rnti and the set of lcIds given as SPS LCs with MAC */
      lcInfo.cellId = cell->cellId;
      lcInfo.crnti = ue->ueId;
      lcInfo.spsRnti = ue->spsRnti;
      lcInfo.spsLcCnt = ue->ul.ulSpsCfg.lcCnt;
      for (idx=0; idx<lcInfo.spsLcCnt; idx++)
      {
         lcInfo.spsLcId[idx] = ue->ul.ulSpsCfg.lcId[idx];
      }
      lcInfo.implRelCnt = ue->ul.ulSpsCfg.implicitRelCnt;
      lcInfo.spsPrd = ue->ul.spsPrdcty;
      rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
      /* code Coverage portion of the test case */ 
      RgSchMacSpsLcReg(&pst, &lcInfo); 

      /*breaking here currently to process only one UE per TTI*/
      break;
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcToBeActvtdUe */

/**
 * @brief Handler to check if Ue is inactive for UL scheduling
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlChkUeInActv
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcToBeActvtdUe
 *        
 *     Processing Steps:
 *           if (ue->ulInactvMask)
 *              return TRUE
 *           if (ue->hdfdd)
 *              invoke isUlSchedUE() to check if UE can be scheduled in UL
 *           invoke ChkMeasGapColl if ue->measGap is enabled
 *              return TRUE if collides
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Bool
 *       TRUE if UE is inactive
 *       FALSE otherwise
 **/

#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsUlChkUeInActv
(
RgSchCellCb             *cell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsUlChkUeInActv(cell, ue)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   CmLteTimingInfo timeToTx;

   TRC2(rgSCHCmnSpsUlChkUeInActv);

   RGSCHDBGPRM (cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlChkUeInActv:cellId %d, ueId :%d\n", 
              (int)cell->cellId, (int)ue->ueId));

   /* calculate the time to Transmit */
   RGSCHADDTOCRNTTIME(cell->crntTime, timeToTx, RGSCH_SPS_UL_DELTA);

   /* Check if Ue is inactive for UL scheduling */
   if (ue->ul.ulInactvMask)
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "Ue (%d) inactive mask is set\n", ue->ueId));
      RETVALUE(TRUE);
   } /* end of (ue->inactvmask) */
 
   
   /* Check if measuring collides with UL SPS transmissions if activated */
   if ((ue->measGapCb.isMesGapEnabled == TRUE) &&
       (rgSCHCmnSpsUlChkMesGapColl(cell, ue, timeToTx,
               ue->measGapCb.gapPrd) == TRUE))
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
        "Ue (%d) is not eligible for UL transmission due to measurement gap\n", 
        ue->ueId));
      RETVALUE(TRUE);
   }

   RETVALUE(FALSE);   
} /* end of rgSCHCmnSpsUlChkUeInActv */

/**
 * @brief Handler to process TTI for  Uplink SPS 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlTti
 *
 *     Invoked by:
 *      - rgSCHCmnTti before calling normal uplink scheduling 
 *        
 *     Processing Steps:
 *      invoke rgSCHCmnSpsUlProcActvtdUes to process all the UL-SPS activated
 *        UEs
 *      invoke rgSCHCmnSpsUlProcToBeRelUe to process the UE in the
 *        toBeRelUeLst
 *      invoke rgSCHCmnSpsUlProcToBeActvtdUe to process the UE in the
 *        toBeActvtdUeLst
 *      return ROK
 *  @param[in]  RgSchCellCb     *cell
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlTti
(
RgSchCellCb             *cell,
Bool                    dlSched,
Bool                    ulSched,
RgSchCmnUlRbAllocInfo   *alloc
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlTti(cell, dlSched, ulSched, alloc)
RgSchCellCb             *cell;
Bool                    dlSched;
Bool                    ulSched;
RgSchCmnUlRbAllocInfo   *alloc;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUlTti);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlTti:cellId %d\n", (int)cell->cellId));

   /* Process UL SPS activated UEs */ 
   rgSCHCmnSpsUlProcActvtdUes(cell, ulSched, alloc);

   /* Process UE waiting for UL SPS Release */ 
   if (dlSched)
   {
      rgSCHCmnSpsUlProcToBeRelUe(cell);
   }

   /* Process UE waiting for UL SPS Activation */ 
   if (ulSched)
   {
      rgSCHCmnSpsUlProcToBeActvtdUe(cell, alloc);
   }


   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlTti */

#ifdef RG_UNUSED
/**
 * @brief Handler to send the list of UL SPS active UEs for a given time
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlGetActvUeLst
 *
 *     Invoked by:
 *      - HD-FDD module 
 *        
 *     Processing Steps:
 *       Fetch the actvUeLstIdx for the given timingInfo
 *          return (actvUeLst = &actvueLsts[actvUeLstIdx])
 *       return ROK
 *  @param[in]  RgSchCellCb     *cell
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlGetActvUeLst 
(
RgSchCellCb     *cell,
CmLteTimingInfo timingInfo,
CmLListCp       *ulSpsActvUeLst
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlGetActvUeLst(cell, timingInfo, ulSpsActvUeLst)
RgSchCellCb     *cell;
CmLteTimingInfo timingInfo;
CmLListCp       *ulSpsActvUeLst;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   U16                   actvUeLstIdx;

   TRC2(rgSCHCmnSpsUlGetActvUeLst);
  
   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
         "rgSCHCmnSpsUlGetActvUeLst:cellId : %d, sfn: %d, subframe: %d\n", 
              cell->cellId, timingInfo.sfn, timingInfo.subframe));

   /* Fetch the list of UL SPS active UEs for the given timingInfo */
   actvUeLstIdx = ((timingInfo.sfn * RGSCH_NUM_SUB_FRAMES) + 
                      timingInfo.subframe) % RG_SCH_CMN_SPS_MAX_PRD;

   ulSpsActvUeLst = &ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx];

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlGetActvUeLst */
#endif /*RG_UNUSED*/

/************** UL SPS Utility Functions *************/

/**
 * @brief Handler to remove the SPS specific information in the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlFreeSpsUe
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUeDel
 *      -rgSCHCmnSpsUlLcgDel
 *        
 *     Processing Steps: 
 *        Free measGapMask if allocated
 *        clear sps allocations of the UE
 *        Delete from lists
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUtlFreeSpsUe
(
RgSchCellCb     *cell,
RgSchUeCb       *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUtlFreeSpsUe(cell, ue)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
#endif /* ANSI */
{
    RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
    RgSchCmnUlUeSpsInfo   *ulSpsUe   = RG_SCH_CMN_GET_UL_SPS_UE(ue);
    RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
#ifdef LTE_TDD
    U8                    maxSubfrms = cellUl->numUlSubfrms;
#else
    U8                    maxSubfrms = RG_SCH_CMN_UL_NUM_SF;
#endif
    U16                   actvOffst  = ulSpsUe->ulSpsSchdInfo.actvSfLstIdx;
    U16 ulSfIdx = (ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc.sfn * RGSCH_NUM_SUB_FRAMES + ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc.subframe) % maxSubfrms;
    Pst                   pst;

    TRC2(rgSCHCmnSpsUlUtlFreeSpsUe);

    RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
                "rgSCHCmnSpsUlUtlFreeSpsUe:cellId : %d, ue: %d\n", 
                cell->cellId, ue->ueId));

    if (ulSpsUe->state == RGSCH_SPS_IDLE)
    {
        /* Nothing needs to be done */
        ue->ul.ulSpsCfg.isUlSpsEnabled = FALSE;
        RETVOID;
    }

    if (ulSpsUe->state == RGSCH_SPS_TO_BE_ACTV)
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
        ue->ul.ulSpsCfg.isUlSpsEnabled = FALSE;
        RETVOID;
    }
    if(ulSpsUe->ulSpsSchdInfo.crntAlloc)
    {
        rgSCHCmnUlFreeAllocation(
                &cellUl->ulSfArr[ulSfIdx], 
                ulSpsUe->ulSpsSchdInfo.crntAlloc);
    }
    /* Free the allocations of the UE in cell wide sps sf tbl */
    rgSCHCmnUlSpsClrAlloc (cell, ue, &(ulSpsCell->spsSfLst[actvOffst]),
            &ulSpsUe->ulSpsSchdInfo.allocInfo);

    /* Remove Ue from sps list */
    rgSCHDbmDelSpsUeCb(cell, ue);
    /* Remove UE from actvtdUeLst */
    rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);

    /* Remove Ue from other lists, if it exists */
    if (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV)
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
    }
    else if (ulSpsUe->state == RGSCH_SPS_TO_BE_REL)
    {
        rgSCHCmnSpsUlDelUeFrmToBeRelLst(cell, ue);
    }
    ue->ul.ulSpsCfg.isUlSpsEnabled = FALSE;
    /* deregister the SPS LCs with MAC */
    rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
    RgSchMacSpsLcDereg(&pst, cell->cellId, ue->ueId);
    RETVOID;
} /* end of rgSCHCmnSpsUlUtlFreeSpsUe */

/**
 * @brief Get an alloc from the specified position in the BW. 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUtlUlGetSpfcAlloc
 *
 *      - Return an alloc from the specified position in the BW.
 *
 *  @param[in]  RgSchUlSf     *sf
 *  @param[in]  U8            startSb
 *  @param[in]  U8            numSb
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PRIVATE RgSchUlAlloc *rgSCHCmnSpsUtlUlGetSpfcAlloc
(
RgSchUlSf       *sf,
U8              startSb,
U8              numSb
)
#else /* ANSI */
PRIVATE RgSchUlAlloc *rgSCHCmnSpsUtlUlGetSpfcAlloc(sf, startSb, numSb)
RgSchUlSf       *sf;
U8              startSb;
U8              numSb;
#endif /* ANSI */
{
   RgSchUlHole     *hole, *nxtHole;
   RgSchUlAlloc    *alloc = NULLP;
   TRC2(rgSCHCmnSpsUtlUlGetSpfcAlloc);

   if (!(hole = rgSCHUtlUlHoleFirst(sf)))
   {
      RETVALUE(NULLP);
   }
   do
   {
      nxtHole = rgSCHUtlUlHoleNxt(sf, hole);
      if ((startSb >= hole->start) &&
          (startSb+numSb <= hole->start+hole->num))
      {
         if (startSb != hole->start)
         {
            /* Create a new hole to accomodate Subbands between
             * hole start and req alloc start */
            RgSchUlHole *newHole = rgSCHUtlUlHoleMemGet(&(sf->holeDb->mem));

#if (ERRCLASS & ERRCLS_DEBUG)
            if (newHole == NULLP )
               RETVALUE(NULLP);
#endif
            newHole->start = hole->start;
            newHole->num = startSb - hole->start;
            hole->num -= (startSb - hole->start);
            hole->start = startSb;
            ++(sf->holeDb->count);
            rgSCHUtlUlHoleIns(sf->holeDb, newHole);
            newHole->prvAlloc = hole->prvAlloc;
            hole->prvAlloc = NULLP;
            if (newHole->prvAlloc) 
            {
               newHole->prvAlloc->nxtHole = newHole;
            }
            alloc = rgSCHCmnUlSbAlloc(sf, numSb, hole, 0);
            alloc->prvHole = newHole;
            newHole->nxtAlloc = alloc;
         }
         else /* Hole start and req alloc start are same */
         {
            alloc = rgSCHCmnUlSbAlloc(sf, numSb, hole, 0);
         }
         break;
      }
   } while ((hole = nxtHole));
   RETVALUE(alloc);
} /* end of rgSCHCmnSpsUtlUlGetSpfcAlloc */

/**
 * @brief Handler to copy source allocation to destination allocation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlCpyAlloc
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlProcActvtdUes
 *       rgSCHCmnSpsUlProcToBeReactvtdUe 
 *       rgSCHCmnSpsUlProcToBeActvtdUe 
 *        
 *     Processing Steps:
 *        Copy the entire source allocation to destination allocation
 *         
 *
 *  @param[in]  RgSchUlAlloc         *srcAlloc
 *  @param[in]  RgSchUlAlloc         *dstAlloc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUtlCpyAlloc
(
RgSchUlAlloc         *srcAlloc,
RgSchUlAlloc         *dstAlloc 
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUtlCpyAlloc(srcAlloc, dstAlloc)
RgSchUlAlloc         *srcAlloc;
RgSchUlAlloc         *dstAlloc;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsUlUtlCpyAlloc);
   cmMemcpy((U8*)dstAlloc, (U8*)srcAlloc, sizeof(RgSchUlAlloc));
   dstAlloc->pdcch          = NULLP;
  
   RETVALUE(ROK); 
} /* end of rgSCHCmnSpsUlUtlCpyAlloc */


/**
 * @brief Handler to fill release pdcch to be sent for a UE 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlFillRelPdcch
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlProcToBeRelUe
 *        
 *     Processing Steps:
 *       Fill pdcch to reflect SPS release
 *       return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchPdcch              *pdcch
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUtlFillRelPdcch
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchPdcch              *pdcch
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUtlFillRelPdcch(cell, ue, pdcch)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchPdcch              *pdcch;
#endif /* ANSI */
{
   
   TRC2(rgSCHCmnSpsUlUtlFillRelPdcch);

   RGSCHDBGPRM(cell->instIdx, (rgSchPBuf(cell->instIdx),
            "rgSCHCmnSpsUlUtlFillRelPdcch:cellId : %d, ueId: %d\n", 
            cell->cellId, pdcch->rnti));

   /* SPS RNTI in pdcch */
   pdcch->rnti = ue->spsRnti;
   pdcch->dci.u.format0Info.tpcCmd = 0;
   pdcch->dci.u.format0Info.nDmrs = 0;
   /* MCS set to all 1's */
   pdcch->dci.u.format0Info.mcs = 0x1F;
   /* Set all bits of RIV to 1's */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   pdcch->dci.u.format0Info.riv = 0xFFFFFFFF;
#endif /*TFU_UPGRADE*/

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUtlFillRelPdcch */

/**
 * @brief Handler to insert Ue into ToBeRelUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlInsUeInToBeRelLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlProcActvtdUes
 *           rgSCHCmnSpsUlProcToBeRelUe
 *        
 *     Processing Steps:
 *       Insert UE into tobeRelUeLst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeRelLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeRelLst(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe  = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   
   TRC2(rgSCHCmnSpsUlUtlInsUeInToBeRelLst);
   
   ulSpsUe->pndngUeLstEnt.node = (PTR)ue;
   cmLListAdd2Tail(&ulSpsCell->toBeRelUeLst, &ulSpsUe->pndngUeLstEnt);

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUtlInsUeInToBeRelLst */

/**
 * @brief Handler to insert Ue into ToBeActvUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlInsUeInToBeActvLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsBsrRpt
 *           rgSCHCmnSpsUlProcToBeReactvtdUe
 *           rgSCHCmnSpsUlProcToBeActvtdUe
 *        
 *     Processing Steps:
 *       Insert UE into tobeActvUelst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeActvLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeActvLst(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe  = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2 (rgSCHCmnSpsUlUtlInsUeInToBeActvLst);

   ulSpsUe->pndngUeLstEnt.node = (PTR)ue;
   cmLListAdd2Tail (&ulSpsCell->toBeActvtdUeLst, &ulSpsUe->pndngUeLstEnt); 

   RETVALUE(ROK);   
} /* end of rgSCHCmnSpsUlUtlInsUeInToBeActvLst */

/**
 * @brief Handler to insert Ue into ActvUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlInsUeInActvLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlProcActvtdUes
 *           rgSCHCmnSpsUlProcToBeActvtdUe
 *        
 *     Processing Steps:
 *       Insert UE into ActvUelst based on time
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  CmLteTimingInfo  spsOcc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlInsUeInActvLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       spsOcc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlInsUeInActvLst(cell, ue, spsOcc)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
CmLteTimingInfo       spsOcc;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U16                   actvUeLstIdx;

   TRC2 (rgSCHCmnSpsUlInsUeInActvLst);

   /* Compute actvlstidx and insert */
   actvUeLstIdx = (((spsOcc.sfn * RGSCH_NUM_SUB_FRAMES) + spsOcc.subframe)) % 
                                           (RG_SCH_CMN_SPS_MAX_PRD);

   ulSpsUe->actvUeLstEnt.node = (PTR)ue;
   /* Add UE to actvUlSpsUeLsts at the selected index */
   cmLListAdd2Tail (&ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx],
                                                &ulSpsUe->actvUeLstEnt);
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlInsUeInActvLst */

/**
 * @brief Handler to delete Ue from ToBeActvUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlDelUeFrmToBeActvLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlUtlFreeSpsUe
 *        
 *     Processing Steps:
 *       Delete UE from  ToBeActvUelst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeActvLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   
   TRC2(rgSCHCmnSpsUlDelUeFrmToBeActvLst);

   cmLListDelFrm (&ulSpsCell->toBeActvtdUeLst, &ulSpsUe->pndngUeLstEnt);
   ulSpsUe->pndngUeLstEnt.node = NULLP;

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlDelUeFrmToBeActvLst */

/**
 * @brief Handler to delete Ue from ToBeRelUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlDelUeFrmToBeRelLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlUtlFreeSpsUe
 *        
 *     Processing Steps:
 *       Delete UE from  ToBeRelUelst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeRelLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeRelLst(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   
   TRC2(rgSCHCmnSpsUlDelUeFrmToBeRelLst);

   cmLListDelFrm (&ulSpsCell->toBeRelUeLst, &ulSpsUe->pndngUeLstEnt);
   ulSpsUe->pndngUeLstEnt.node = NULLP;

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlDelUeFrmToBeRelLst */

/**
 * @brief Handler to delete Ue from actvLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlDelUeFrmActvLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlProcActvtdUes
 *           rgSCHCmnSpsUlProcToBeActvtdUe
 *           rgSCHCmnSpsUlUtlFreeSpsUe
 *        
 *     Processing Steps:
 *       Delete UE from ActvUelst based on time
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  CmLteTimingInfo  spsOcc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmActvLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       spsOcc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, spsOcc)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
CmLteTimingInfo       spsOcc;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U16                   actvUeLstIdx;

   TRC2 (rgSCHCmnSpsUlDelUeFrmActvLst);

   /* Compute actvlstidx and delete */
   actvUeLstIdx = (((spsOcc.sfn * RGSCH_NUM_SUB_FRAMES) + spsOcc.subframe)) % 
                                           (RG_SCH_CMN_SPS_MAX_PRD);

/*   ulSpsUe->actvUeLstEnt.node = (PTR)ue;*/
   /* Delete UE from actvUlSpsUeLsts at the selected index */
   cmLListDelFrm (&ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx], 
                                              &ulSpsUe->actvUeLstEnt);
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlDelUeFrmActvLst */

/**
 * @brief Handler to free tobeActvUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFreeToBeActvUeLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlCellDel
 *        
 *     Processing Steps:
 *       Delete  all the UEs from ToBeActvUelst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFreeToBeActvUeLst
(
RgSchCellCb           *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFreeToBeActvUeLst(cell)
RgSchCellCb           *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   CmLList               *ueLnk;

   TRC2 (rgSCHCmnSpsUlFreeToBeActvUeLst);

   /* Delete all the UEs from toBeActvtdueLst */
   while ((ueLnk = cmLListFirst(&ulSpsCell->toBeActvtdUeLst)) != NULLP)
   {
      cmLListDelFrm(&ulSpsCell->toBeActvtdUeLst, ueLnk);
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFreeToBeActvUeLst */


/**
 * @brief Handler to free tobeRelUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFreeToBeRelUeLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlCellDel
 *        
 *     Processing Steps:
 *       Delete all the UEs from ToBeRelUeLst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFreeToBeRelUeLst
(
RgSchCellCb           *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFreeToBeRelUeLst(cell)
RgSchCellCb           *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   CmLList               *ueLnk;

   TRC2 (rgSCHCmnSpsUlFreeToBeRelUeLst);

   /* delete all the UEs from toBeRelUeLst */
   while ((ueLnk = cmLListFirst(&ulSpsCell->toBeRelUeLst)) != NULLP)
   {
      cmLListDelFrm(&ulSpsCell->toBeRelUeLst, ueLnk);
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFreeToBeRelUeLst */

/**
 * @brief Handler to free actvUeLsts
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFreeActvUeLst
 *
 *     Invoked by:
 *            rgSCHCmnSpsUlCellDel
 *        
 *     Processing Steps:
 *       Delete all the UEs from all ActvUeLsts in the cell
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFreeActvUeLst
(
RgSchCellCb           *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFreeActvUeLst(cell)
RgSchCellCb           *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   CmLList               *ueLnk;
   U16                   idx;

   TRC2 (rgSCHCmnSpsUlFreeActvUeLst);

   /* Delete all the Ues from actvUlSpsUeLsts */
   for (idx=0; idx<RG_SCH_CMN_SPS_MAX_PRD; idx++)
   {
      while ((ueLnk = cmLListFirst(&ulSpsCell->actvUlSpsUeLsts[idx])) != NULLP)
      {
         cmLListDelFrm (&ulSpsCell->actvUlSpsUeLsts[idx], ueLnk);
      }
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFreeActvUeLst */

/** @brief This function is called to fill activation PDCCH for DCI format 1
 *  
 *  @details
 *  Invoked by - rgSCHCmnSpsDlFillActvPdcch
 *  
 *  Function:  rgSCHCmnSpsDlFillActvPdcchDciFrmt1
 *       
 *  Processing steps:
 *  Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *  @param  RgSchCellCb         *cellCb
 *  @param  RgSchCmnDlUeSpsInfo *dlSpsUe    
 *  @param  RgSchDlRbAlloc      *rbAllocInfo
 *  @param  RgSchPdcch           *pdcch
 *  @param  Bool                 isSpsOcc
 *  @param  U8                   hqProcId
 *  @return Void 
 *  
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1
(
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1(cell, dlSpsUe, rbAllocInfo,
        pdcch, isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchCmnDlUeSpsInfo        *dlSpsUe;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   U8 tpc;
   U32 allocBitMask;
   
   tpc = dlSpsUe->dlSpsUeSchdInfo.allocN1PucchIdx;
   allocBitMask = 0;

   TRC2(rgSCHCmnSpsDlFillActvPdcchDciFrmt1)
   if (!isSpsOcc)
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
   }
   else
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, NULLP);
   }

   pdcch->dci.u.format1Info.tpcCmd = tpc;
   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
       /* Fill-up for resource allocation type 0 */
       pdcch->dci.u.format1Info.allocInfo.isAllocType0 = TRUE;
       allocBitMask =  rbAllocInfo->allocInfo.raType0.dlAllocBitMask;
   }
   else
   {
       /* Fill-up for resource allocation type 1 */
       pdcch->dci.u.format1Info.allocInfo.isAllocType0 = FALSE;
       allocBitMask =  rbAllocInfo->allocInfo.raType1.dlAllocBitMask;
   }
   pdcch->dci.u.format1Info.allocInfo.resAllocMap[0] = 
               ((allocBitMask >> 24) & 0xff);
   pdcch->dci.u.format1Info.allocInfo.resAllocMap[1] = 
               ((allocBitMask >> 16) & 0x00ff);
   pdcch->dci.u.format1Info.allocInfo.resAllocMap[2] = 
               ((allocBitMask >> 8) & 0x0000ff);
   pdcch->dci.u.format1Info.allocInfo.resAllocMap[3] = 
               ((allocBitMask & 0x000000ff));
   pdcch->dci.u.format1Info.allocInfo.harqProcId = hqProcId;
   pdcch->dci.u.format1Info.allocInfo.ndi = 0;
   pdcch->dci.u.format1Info.allocInfo.mcs =
                                rbAllocInfo->tbInfo[0].tbCb->dlGrnt.iMcs;
   pdcch->dci.u.format1Info.allocInfo.rv = 0;

   RETVOID;
}
/** @brief This function is called to fill activation PDCCH for DCI format 1A
 *  
 *  @details
 *  Invoked by - rgSCHCmnSpsDlFillActvPdcch
 *  
 *  Function:  rgSCHCmnSpsDlFillActvPdcchDciFrmt1A
 *       
 *  Processing steps:
 *  Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *  @param  RgSchCellCb         *cellCb
 *  @param  RgSchCmnDlUeSpsInfo *dlSpsUe    
 *  @param  RgSchDlRbAlloc      *rbAllocInfo
 *  @param  RgSchPdcch           *pdcch
 *  @param  Bool                 isSpsOcc
 *  @param  U8                   HqProcId
 *   @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1A
(
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1A(cell, dlSpsUe, rbAllocInfo,
        pdcch,isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchCmnDlUeSpsInfo        *dlSpsUe;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   U8 tpc;

   tpc = dlSpsUe->dlSpsUeSchdInfo.allocN1PucchIdx;

   TRC2(rgSCHCmnSpsDlFillActvPdcchDciFrmt1A)
    if (!isSpsOcc)
    {
        rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
    }
    else
    {
        rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, NULLP);
    }
   pdcch->dci.u.format1aInfo.dciInfo = TFU_DCIFMT1A_PDSCHINFO;
   pdcch->dci.u.format1aInfo.t.pdschInfo.tpcCmd  = tpc;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.mcs     = \
                         rbAllocInfo->tbInfo[0].tbCb->dlGrnt.iMcs;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.pres = \
                         TRUE;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.val = \
                         hqProcId;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.ndi = 0;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.rv = 0;
   /* As of now, we do not support Distributed allocations */
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.isLocal = TRUE;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.nGap2.pres = \
                        NOTPRSNT;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.type = \
               TFU_ALLOC_TYPE_RIV;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.u.riv = \
    rgSCHCmnCalcRiv (cell->bwCfg.dlTotalBw, 
                     rbAllocInfo->allocInfo.raType2.rbStart,
                     rbAllocInfo->allocInfo.raType2.numRb);

   RETVOID;
}
/** @brief This function is called to fill activation PDCCH for DCI format 2
 *  
 *  @details
 *  Invoked by - rgSCHCmnSpsDlFillActvPdcch
 *  
 *  Function:  rgSCHCmnSpsDlFillActvPdcchDciFrmt2
 *       
 *  Processing steps:
 *  Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *  @param  RgSchCellCb         *cellCb
 *  @param  RgSchCmnDlUeSpsInfo *dlSpsUe    
 *  @param  RgSchDlRbAlloc      *rbAllocInfo
 *  @param  RgSchPdcch           *pdcch
 *  @param  Bool                 isSpsOcc
 *  @param  U8                   HqProcId
 *   @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2
(
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2(cell, dlSpsUe, rbAllocInfo,
        pdcch, isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchCmnDlUeSpsInfo        *dlSpsUe;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   U8 tpc;
   U32 allocBitMask;

   tpc = dlSpsUe->dlSpsUeSchdInfo.allocN1PucchIdx;
   allocBitMask = 0;

   TRC2(rgSCHCmnSpsDlFillActvPdcchDciFrmt2)

   if (!isSpsOcc)
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
   }
   else
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, NULLP);
   }
   
   pdcch->dci.u.format2Info.tpcCmd = tpc;
    /*ccpu00117078 - MOD - typo correction for format2Info*/
   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
        /* Fill-up for resource allocation type 0 */
       pdcch->dci.u.format2Info.allocInfo.isAllocType0 = TRUE;
       allocBitMask =  rbAllocInfo->allocInfo.raType0.dlAllocBitMask;
   }
   else
   {
       /* Fill-up for resource allocation type 1 */
       pdcch->dci.u.format2Info.allocInfo.isAllocType0 = FALSE;
       allocBitMask =  rbAllocInfo->allocInfo.raType1.dlAllocBitMask;
   }
   pdcch->dci.u.format2Info.allocInfo.resAllocMap[0] = \
               ((allocBitMask >> 24) & 0xff);
   pdcch->dci.u.format2Info.allocInfo.resAllocMap[1] = \
               ((allocBitMask >> 16) & 0x00ff);
   pdcch->dci.u.format2Info.allocInfo.resAllocMap[2] = \
               ((allocBitMask >> 8) & 0x0000ff);
   pdcch->dci.u.format2Info.allocInfo.resAllocMap[3] = \
               ((allocBitMask & 0x000000ff));
   
   pdcch->dci.u.format2Info.allocInfo.harqProcId = hqProcId;
   pdcch->dci.u.format2Info.allocInfo.precoding  = 0;
   pdcch->dci.u.format2Info.allocInfo.transSwap = 0;
   pdcch->dci.u.format2Info.allocInfo.tbInfo[0].mcs = 0;
   pdcch->dci.u.format2Info.allocInfo.tbInfo[0].rv  = 1;

   /* Fill tbInfo for scheduled TBs */

   pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].ndi = 0;
   pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].mcs = rbAllocInfo->tbInfo[0].tbCb->dlGrnt.iMcs;
   pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].rv = 0;

   RETVOID;
}
/** @brief This function is called to fill activation PDCCH for DCI format 2A
 *  
 *  @details
 *  Invoked by - rgSCHCmnSpsDlFillActvPdcch
 *  
 *  Function:  rgSCHCmnSpsDlFillActvPdcchDciFrmt2A
 *       
 *  Processing steps:
 *  Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *  @param  RgSchCellCb         *cellCb
 *  @param  RgSchCmnDlUeSpsInfo *dlSpsUe    
 *  @param  RgSchDlRbAlloc      *rbAllocInfo
 *  @param  RgSchPdcch           *pdcch
 *  @param  Bool                 isSpsOcc
 *  @param  U8                   HqProcId
 *   @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2A
(
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2A(cell, dlSpsUe, rbAllocInfo,
        pdcch, isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchCmnDlUeSpsInfo        *dlSpsUe;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   U8 tpc;
   U32 allocBitMask;
   
   tpc = dlSpsUe->dlSpsUeSchdInfo.allocN1PucchIdx;
   allocBitMask = 0;

   TRC2(rgSCHCmnSpsDlFillActvPdcchDciFrmt2A)

   if (!isSpsOcc)
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
   }
   else
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, NULLP);
   }
   pdcch->dci.u.format2AInfo.tpcCmd = tpc;
   /*ccpu00117078 - MOD - typo correction for format2AInfo*/
   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
       /* Fill-up for resource allocation type 0 */
       pdcch->dci.u.format2AInfo.allocInfo.isAllocType0 = TRUE;
       allocBitMask =  rbAllocInfo->allocInfo.raType0.dlAllocBitMask;
   }
   else
   {
       /* Fill-up for resource allocation type 1 */
       pdcch->dci.u.format2AInfo.allocInfo.isAllocType0 = FALSE;
       allocBitMask =  rbAllocInfo->allocInfo.raType1.dlAllocBitMask;
   }
   pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[0] = 
               ((allocBitMask >> 24) & 0xff);
   pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[1] = 
               ((allocBitMask >> 16) & 0x00ff);
   pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[2] = 
               ((allocBitMask >> 8) & 0x0000ff);
   pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[3] = 
               ((allocBitMask & 0x000000ff));
   pdcch->dci.u.format2AInfo.allocInfo.harqProcId = hqProcId;
   pdcch->dci.u.format2AInfo.allocInfo.precoding  = 0;
   pdcch->dci.u.format2AInfo.allocInfo.transSwap  = 0;
   /* Initialize the TB info for TB 0 only */
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[0].mcs = 0;
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[0].rv  = 1;
   /* Fill tbInfo for scheduled TBs */
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].ndi = 0;
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].mcs = rbAllocInfo->tbInfo[0].tbCb->dlGrnt.iMcs;
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].rv = 0;

   RETVOID;
}

/*rg002.301 ccpu00119494-ADD-handler function to handle UE's who 
 * were expecting a ACK for Release PDCCH but received
 * no feedback
 */
/** @brief Handler to retransmit Release PDCCHs 
 *  
 *  @details
 *  Invoked by - rgSCHTomTtiInd
 *  
 *  Function:  rgSCHCmnSpsDlRetransRelPdcch
 *       
 *  Processing steps:
 *  a) Taking current subframe as index, traverse
 *     through wtngForRelFdbkUeLst.
 *  b) If any entry is found, a retranmission is
 *     scheduled
 *
 *  @param  RgSchCellCb         *cellCb
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlReTxRelPdcch
(
RgSchCellCb                *cell
)
#else /* ANSI */
PUBLIC  Void rgSCHCmnSpsDlReTxRelPdcch(cell)
RgSchCellCb                *cell;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell;
   U8                     idx;
   CmLList               *listNode;

   RgSchUeCb             *ue;
   RgSchCmnDlUe          *cmnUeDl;
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   RgSchDlSf             *dlSf;
   CmLteTimingInfo       actulTxTime; /*the timingInfo of feedback */
#ifdef LTE_TDD
   CmLteTimingInfo       actulRelTime;
#endif
   


   dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   idx       = cell->crntTime.subframe;

   /*initialize listNode */ 
   CM_LLIST_FIRST_NODE(&(dlSpsCell->wtngForRelFdbkUeLst[idx]),listNode);
   
   while ( listNode )
   {
      ue      = (RgSchUeCb *)(listNode->node);
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
#ifdef LTE_TDD
      /* wtngForRelFdbkUeLst is an array of CmLListCp of size
       * RGSCH_NUM_SUB_FRAMES and the subframe acts as an idx
       * to this array.However, in TDD for some cfgs like 5,
       * a feedback can be expected 13 subframes after RelPDCCH Tx
       * which would cause a cycle in the array. To ensure, we 
       * don't delete the UE too early from this queue, we
       * compare the Release time with the current time - TFU_HQFBKIND_ULDELTA 
       * . If they do not match, we do not delete
       * */
      RGSCHSUBFRMCRNTTIME(cell->crntTime,actulRelTime,(TFU_HQFBKIND_ULDELTA));
      if ( !(RGSCH_TIMEINFO_SAME(actulRelTime, ue->relPdcchFbkTiming)) )
      {
         CM_LLIST_NEXT_NODE(&(dlSpsCell->wtngForRelFdbkUeLst[idx]),listNode);
         continue;
      }
#endif
      dlSpsUe->isRelPdcchSent = FALSE;

      cmLListDelFrm(&(dlSpsCell->wtngForRelFdbkUeLst[idx]),
                      &(dlSpsUe->wtngForRelFdbkUeEnt));

      /*reset relPdcch in dlSf */
#ifdef LTE_TDD
      actulTxTime = ue->relPdcchTxTime;
#else
      RGSCHSUBFRMCRNTTIME(cell->crntTime,actulTxTime,RG_SCH_CMN_SPS_TX_TIME);
#endif
      dlSf = rgSCHUtlSubFrmGet(cell,actulTxTime);

      rgSCHUtlPdcchPut(cell,&dlSf->pdcchInfo, dlSf->relPdcch);
      dlSf->relPdcch = NULLP;
#ifdef LTE_TDD
      ue->relPdcchTxTime.sfn    = RGSCH_MAX_SFN + 1;
#endif
      ue->relPdcchFbkTiming.sfn = RGSCH_MAX_SFN + 1;

      CM_LLIST_NEXT_NODE(&(dlSpsCell->wtngForRelFdbkUeLst[idx]),listNode);
   }/*while (listNode) */

   RETVOID;

}/*rgSCHCmnSpsDlReTxRelPdcch*/
#endif /* LTEMAC_SPS */

/**********************************************************************
 
         End of file:     gk_sch_sps.c@@/main/2 - Sat Jul 30 02:21:50 2011
 
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
/main/1      ---     sd   1. LTE MAC 3.1 release
/main/1   rg009.201  rsharon   1. [ccpu00115756]:Removed TFU_UPGRADE flag for
                                   DTX.
                    nudupi 2.Pending Insure fixes accomodated
/main/1    3. Added changes of TFU_UPGRADE. 
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
  rg001.301  nudupi  1.Added the changes for the CR ccpu00117270 
                     2. ccpu00118350:Correcting NDI manipulation of Harq.
          rg002.301 asehgal 1. ccpu00119494: added code to handle retx
                               of release PDCCH.
*********************************************************************91*/


