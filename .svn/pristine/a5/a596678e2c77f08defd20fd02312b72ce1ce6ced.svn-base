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
  
     File:     rg_sch_tom.c 
  
     Sid:      gk_sch_tom.c@@/main/2 - Sat Jul 30 02:21:51 2011
  
     Prg:      sd 
  
**********************************************************************/

/** @file rg_sch_tom.c 
@brief This module does processing related to handling of lower interface APIs 
invoked by PHY towards scheduler.
*/
/* header include files -- defines (.h) */
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
#include "cm_lte.h"       /* common tokens */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "rgr.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"        /* defines and macros for MAC */
#include "rg_sch_err.h"    /* defines and macros for MAC */
#include "rg_sch_inf.h"    /* defines and macros for MAC */
#include "rg_sch.h"        /* defines and macros for MAC */
#include "rg_sch_cmn.h"    /* typedefs for MAC */


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
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for MAC */
#include "rg_sch_cmn.x"    /* typedefs for MAC */

/* local defines */


/* local typedefs */

/* local externs */
/* rg006.201: [ccpu000111706, ccpu00112394]: Separated UL and DL TTI
 * processing */
#ifndef LTE_TDD
PRIVATE S16 rgSCHTomUtlProcDlSf ARGS((
         RgSchDlSf        *dlSf,
         RgSchDlSf        *ulSf,
         RgSchCellCb      *cell,
         RgSchErrInfo     *err));
/*rg002.301 - Add - [ccpu00118352] - UL Scheduling at CRC Indication */
#ifdef RG_ULSCHED_AT_CRC
PRIVATE S16 rgSCHTomUtlProcDlSfAtCrc ARGS((
         RgSchDlSf        *ulSf,
         RgSchCellCb      *cell,
         RgSchErrInfo     *err));
#endif /* RG_ULSCHED_AT_CRC */
#else
PRIVATE S16 rgSCHTomUtlProcDlSf ARGS((
         RgSchDlSf        *dlSf,
         RgSchCellCb      *cell,
         RgSchErrInfo     *err));
#endif

PRIVATE S16 rgSCHTomUtlProcUlSf ARGS((
         RgSchCellCb      *cell,
         RgSchErrInfo     *err));

PRIVATE S16 rgSCHTomUtlFillPhich ARGS((
         TfuCntrlReqInfo  *cntrlInfo,
         RgSchDlSf        *dlSf,
         RgSchErrInfo     *err));

/* rg006.201: [ccpu000111706, ccpu00112394]: Separated UL and DL TTI
 * processing */
#ifdef LTE_TDD
PRIVATE S16 rgSCHTomUtlFillPdcch ARGS((
         TfuCntrlReqInfo  *cntrlInfo,
         RgSchDlSf        *dlSf,
         RgSchErrInfo     *err));
#else
PRIVATE S16 rgSCHTomUtlFillDlPdcch ARGS((
         TfuCntrlReqInfo  *cntrlInfo,
         RgSchDlSf        *dlSf,
         RgSchErrInfo     *err));
PRIVATE S16 rgSCHTomUtlFillUlPdcch ARGS((
         TfuCntrlReqInfo  *cntrlInfo,
         RgSchDlSf        *ulSf,
         RgSchErrInfo     *err));
#endif

PRIVATE S16 rgSCHTomUtlProcTA ARGS((
         RgSchCellCb      *cell));
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         U16              validIdx,
        RgSchErrInfo     *err));
#else
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         RgSchErrInfo     *err));
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
PRIVATE S16 rgSCHTomUtlDecodeRawCqi ARGS
(( 
 RgSchCellCb        *cellCb,
 RgSchUeCb          *ueCb,
 TfuDlCqiRpt        *dlCqiRpt,
 TfuRawCqiRpt       *rawCqiRpt,
 CmLteTimingInfo    timingInfo
));

PRIVATE S16 rgSCHTomUtlDecodeRawAcqi ARGS
(( 
 RgSchCellCb        *cellCb,
 RgSchUeCb          *ueCb,
 TfuDlCqiRpt        *dlCqiRpt,
 TfuRawCqiRpt       *rawCqiRpt,
 U8                 cqiRiIdx
));

PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode31 ARGS
((
 RgSchUeCb          *ueCb,
 U8*                  revArray,
 RgSchCqiRawPuschMode31   *mode31Info,
 TfuCqiPuschMode31   *psMode31,
 U8                  ri,
 U32                 numTxAnt
));
PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode12 ARGS
((
 RgSchUeCb          *ueCb,
 U8*                  revArray,
 RgSchCqiRawPuschMode12   *mode12Info,
 TfuCqiPuschMode12   *psMode12,
 U8                  ri,
 U32                 numTxAnt
));
PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode22 ARGS
((
 RgSchUeCb          *ueCb,
 U8*                  revArray,
 RgSchCqiRawPuschMode22   *mode22Info,
 TfuCqiPuschMode22   *psMode22,
 U8                  ri,
 U32                 numTxAnt
));

PRIVATE Void rgSCHTomUtlDecodeRawPcqiPucchMode21 ARGS
(( 
 U8*                 revArray,
 TfuCqiPucchMode21  *puMode21,
 TfuCqiPucchMode21    *mode21,
 U8*                   ri
));
PRIVATE S16 rgSCHTomUtlDecodeRawPcqi ARGS
(( 
 RgSchCellCb        *cellCb,
 RgSchUeCb          *ueCb,
 TfuDlCqiRpt        *dlCqiRpt,
 TfuRawCqiRpt       *rawCqiRpt,
 U8                 cqiRiIdx
));


PRIVATE S16 rgSCHTomFillOnlySrsRecpReq ARGS
((
 RgSchCellCb       *cell,
 RgSchUlAlloc      *alloc,
 TfuUeRecpReqInfo  *datRecpInfo
 ));

PRIVATE S16 rgSCHTomUtlFillCqiSrsWithSr ARGS
((
 RgSchCellCb       *cell,
 RgSchUeCb         *ue, 
 TfuRecpReqInfo    *recpReqInfo,
 TfuUeRecpReqInfo  *pucchRecpInfo,
 U16               validIdx
 ));

PRIVATE S16 rgSCHTomUtlFillCqiSrSrsWithHq ARGS
((
 RgSchCellCb         *cell,
 TfuRecpReqInfo      *recpReqInfo,
 RgSchUeCb     *ue,
 TfuUeRecpReqInfo    *pucchRecpInfo,
 U16                 validIdx
 ));

PUBLIC S16 rgSCHTomUtlFillRiBitWidthInfo ARGS
((
 RgSchUeCb          *ueCb,
 U8                 recReqType
));

PUBLIC U8 rgSCHTomUtlFetchPcqiBitSz ARGS
((
RgSchUeCb          *ueCb, 
U8                 numTxAnt,
U8                 *ri
));

PUBLIC U8 rgSCHTomUtlFetchPcqiBitSzPucchMode21 ARGS
((
RgSchUeCb          *ueCb, 
TfuCqiPucchMode21   *mode21Info,
U8                 numTxAnt,
U8                 *ri
));

PUBLIC S16 rgSCHTomUtlPcqiSbCalcBpIdx ARGS
((
 CmLteTimingInfo   crntTimInfo,
 RgSchUeCb         *ueCb
));

PRIVATE S16 rgSCHTomUtlMoveNxtOccasion ARGS
((
RgSchCellCb        *cell,
RgSchUeCb          *ueCb,
U16                validIdx
));

PRIVATE S16 rgSCHTomUtlMovePcqiNxtOccasion ARGS
((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

PRIVATE S16 rgSCHTomUtlMovePriNxtOccasion ARGS
((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

PRIVATE S16 rgSCHTomUtlMoveSrNxtOccasion ARGS
((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

PRIVATE S16 rgSCHTomUtlMoveSrsNxtOccasion ARGS
((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

PRIVATE Bool rgSCHTomUtlFillDatHarqRecpReq ARGS
((
 RgSchCellCb       *cell,
 RgSchUlAlloc      *alloc,
 TfuUeRecpReqInfo  *datRecpInfo,
 TfuRecpReqInfo    *recpReqInfo
 ));

PRIVATE S16 rgSCHTomUtlFillDatAperRecpReq ARGS
((
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres,
U16               validIdx
));

PRIVATE S16 rgSCHTomUtlFillDatPriRecpReq ARGS
((
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres,
U16               validIdx
));

PRIVATE S16 rgSCHTomUtlFillDatPCqiRecpReq ARGS
((
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres,
U16               validIdx
));

PRIVATE S16 rgSCHTomUtlFillDatSrsRecpReq ARGS
((
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres
));


PRIVATE S16 rgSCHTomUtlFillSrRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         U16              validIdx,
         RgSchErrInfo     *err));

PRIVATE S16 rgSCHTomUtlWillUeRprtCqiRi ARGS((
         RgSchUeCb        *ue,
         Bool             *willueRprtCqiRii));

PRIVATE S16 rgSCHTomUtlFillRiRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         U16              validIdx,
         RgSchErrInfo     *err));

PRIVATE S16 rgSCHTomUtlFillPcqiRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         U16              validIdx,
         RgSchErrInfo     *err));

PRIVATE S16 rgSCHTomUtlFillSrsRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         U16              validIdx,
         RgSchErrInfo     *err));
         
PRIVATE S16 rgSCHTomUtlGenIndices ARGS((
         U32      label,
         U8        posM,
         U8        valN,
         U8        valK,
         TfuSubbandInfo*    sbInfo));

#endif 
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
PRIVATE S16 rgSCHTomUtlFillDatRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         U16              validIdx,
         RgSchErrInfo     *err));
#else
PRIVATE S16 rgSCHTomUtlFillDatRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         RgSchErrInfo     *err));
#endif

#ifdef LTE_TDD
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
PRIVATE S16 rgSCHTomUtlFillSfRepHqFdbk ARGS((
      TfuRecpReqInfo *recpReqInfo,
      RgSchCellCb    *cell,
      RgSchErrInfo   *err,
      RgSchDlSf      *dlSf,
      U8             noFdbks,
      CmMemListCp    *memCp,
      U8             elemIdx,
      RgSchDlSf      *nxtDlsf,
      U16              validIdx
     ));
#else
PRIVATE S16 rgSCHTomUtlFillSfRepHqFdbk ARGS((
      TfuRecpReqInfo *recpReqInfo,
      RgSchCellCb    *cell,
      RgSchErrInfo   *err,
      RgSchDlSf      *dlSf,
      U8             noFdbks,
      CmMemListCp    *memCp,
      U8             elemIdx,
      RgSchDlSf      *nxtDlsf
     ));
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
PRIVATE S16 rgSCHTomUtlFillSfHqFdbk ARGS((
      TfuRecpReqInfo *recpReqInfo,
      RgSchCellCb    *cell,
      RgSchErrInfo   *err,
      RgSchDlSf      *dlSf,
      U8             noFdbks,
      CmMemListCp    *memCp,
      U8             elemIdx,
      RgSchDlSf      *nxtDlsf,
      U16              validIdx
     ));
#else
PRIVATE S16 rgSCHTomUtlFillSfHqFdbk ARGS((
      TfuRecpReqInfo *recpReqInfo,
      RgSchCellCb    *cell,
      RgSchErrInfo   *err,
      RgSchDlSf      *dlSf,
      U8             noFdbks,
      CmMemListCp    *memCp,
      U8             elemIdx,
      RgSchDlSf      *nxtDlsf
     ));
#endif

PRIVATE S16 rgSCHTomUtlFillSfHqFdbkForOneUe ARGS((
      TfuRecpReqInfo          *recpReqInfo,
      RgSchCellCb             *cellCb,
      RgSchErrInfo            *err,
      RgSchDlSf               *dlSf,
      U8                      noFdbks,
      CmMemListCp             *memCp,
      U8                      elemIdx,
      RgSchDlSf               *nxtDlsf,
      CmLteRnti               rnti,
      RgrTddAckNackMode       ackNackMode,
      RgSchUePucchRecpInfo    **pucchInfoRef,
      RgSchPdcch              *pdcch,
      TknU16                  n1PucchTkn,
      Bool                    *allocRef,
      /* rg002.301 - Add - [ccpu00119458] - Correcting
         HqSz filling */
      U8                      hqSz
      ));
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef LTE_TDD
#ifdef LTEMAC_SPS
#ifdef TFU_UPGRADE
PRIVATE S16 rgSCHTomCnsdrRelPdcch ARGS
((
 RgSchCellCb   *cell,
 RgSchDlSf       *dlSf,
 TfuRecpReqInfo          *recpReqInfo,
 U16 validIdx,
 RgSchErrInfo    *err
 ));
#else
 PRIVATE S16 rgSCHTomCnsdrRelPdcch ARGS
((
 RgSchCellCb   *cell,
 RgSchDlSf       *dlSf,
 TfuRecpReqInfo          *recpReqInfo,
 RgSchErrInfo    *err
 ));
#endif
#endif
#endif

#if 1
PRIVATE Void rgSCHTomPrntDciInfo ARGS
(( 
TfuPdcchInfo *datReq
));
#endif

#ifdef MSPD
extern U32 dlCqiRptCnt[16], totDlCqiRpt;
#endif

/* forward references */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
/*HARQ Feedback interpretation in accordance with Femto Forum.
Note: There is no value as '0' in Femto Forum Spec but in order to retain
the existing usage in MAC (and its Acceptance), its being considered*/
CONSTANT PRIVATE U8 rgSchTomHqFbkMap[8] = {0,1,0,0,4,4,4,4};

CONSTANT PRIVATE U32 rgSCHTomBinCoe[6][28]={
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28},
{0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120,136,153,171,190,210,231,253,276,300,325,351,378},
{0,0,1,4,10,20,35,56,84,120,165,220,286,364,455,560,680,816,969,1140,1330,1540,1771,2024,2300,2600,2925,3276},
{0,0,0,1,5,15,35,70,126,210,330,495,715,1001,1365,1820,2380,3060,3876,4845,5985,7315,8855,10626,12650,14950,17550,20475},
{0,0,0,0,1,6,21,56,126,252,462,792,1287,2002,3003,4368,6188,8568,11628,15504,20349,26334,33649,42504,53130,65780,80730,98280},
{0,0,0,0,0,1,7,28,84,210,462,924,1716,3003,5005,8008,12376,18564,27132,38760,54264,74613,100947,134596,177100,230230,296010,376740}
};


/*ccpu00116923 - ADD - SRS present support*/
/*Tables Derived from 3GPP TS 36.211 Section 5.5.3.3 */
/* Table 5.5.3.3-1 */
#ifndef LTE_TDD
CONSTANT PRIVATE RgSchFddCellSpSrsSubfrmTbl rgSchFddCellSpSrsSubfrmTbl = {
   {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
   {TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE},
   {FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE},
   {TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE},
   {FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE},
   {TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE},
   {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE},
   {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE},
   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}
};
#else
/* Table 5.5.3.3-2 */
CONSTANT PRIVATE RgSchTddCellSpSrsSubfrmTbl rgSchTddCellSpSrsSubfrmTbl = {
   {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE},
   {FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE},
   {FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE},
   {FALSE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
   {FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE},
   {FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE},
   {FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE},
   {FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE},
   {FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE},
   {FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE},
   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}
};
#endif

#endif 

#if 1
PRIVATE Void rgSCHTomPrntDciInfo(TfuPdcchInfo *datReq)

{
   TfuDciInfo      dciInfo;
   Tfudciformat1aPdsch  *pdschInfo;
   TfuDciFormat1aAllocInfo *allocInfo;
   TfuDciFormat2AAllocInfo *alloc2Info;

   TfuDciFormat0Info  *dci0Info;

   dciInfo = datReq->dci;
   if (datReq->rnti == 65535)
   {
      /* KWORK_FIX: Added macro for returning void*/
      RETVOID;
   }
   switch (dciInfo.dciFormat)
   {

      case TFU_DCI_FORMAT_1A:
         pdschInfo = &dciInfo.u.format1aInfo.t.pdschInfo;
         allocInfo = &pdschInfo->allocInfo;
#ifdef MSPD
         MSPD_DBG("FORMAT 1A rnti %d ncce %d aggrL %d isLocal %d, allocType %d riv %d harqId %d mcs %d ndi %d rv %d gapPrs %d ngapVal %d tpcCmd %d\n",
               datReq->rnti, datReq->nCce, datReq->aggrLvl, allocInfo->isLocal, allocInfo->alloc.type, allocInfo->alloc.u.riv,
               allocInfo->harqProcId.val, allocInfo->mcs, allocInfo->ndi,allocInfo->rv,allocInfo->nGap2.pres,
               allocInfo->nGap2.val,pdschInfo->tpcCmd);
#endif
         break;

      case TFU_DCI_FORMAT_2A:

         alloc2Info = &dciInfo.u.format2AInfo.allocInfo;
#ifdef MSPD
         MSPD_DBG("FORMAT 2A rnti %d ncce %d aggrL %d allocType0 %d resourceMap %d harqId %d mcs %d ndi %d rv %d mcs %d ndi %d rv %d tpcCmd %d\n",datReq->rnti,
               datReq->nCce, datReq->aggrLvl,alloc2Info->isAllocType0, *((U32*)(&(alloc2Info->resAllocMap))), alloc2Info->harqProcId, 
               alloc2Info->tbInfo[0].mcs, alloc2Info->tbInfo[0].ndi,
               alloc2Info->tbInfo[0].rv,alloc2Info->tbInfo[1].mcs, alloc2Info->tbInfo[1].ndi,
               alloc2Info->tbInfo[1].rv,dciInfo.u.format2AInfo.tpcCmd);
#endif
         break;
      case TFU_DCI_FORMAT_0:
         dci0Info = &dciInfo.u.format0Info;

#ifdef MSPD
         MSPD_DBG("FORMAT 0 rnti %d ncce %d aggrL %d rbSTart %d, NumRb %d  mcs %d  ndi %d  tpcCmd %d nDmrx %d hqProcId %d txAnt %d\n",
               datReq->rnti, datReq->nCce, datReq->aggrLvl, dci0Info->rbStart, dci0Info->numRb, dci0Info->mcs, dci0Info->ndi, dci0Info->tpcCmd,
               dci0Info->nDmrs, dci0Info->hqProcId, dci0Info->txAntenna);
#endif
      break;
      case TFU_DCI_FORMAT_1:	
      MSPD_DBG("TFU_DCI_FORMAT_1\n");
         break;
      case TFU_DCI_FORMAT_1B:
      MSPD_DBG("TFU_DCI_FORMAT_1B\n");
         break;
      case TFU_DCI_FORMAT_1C:
      MSPD_DBG("TFU_DCI_FORMAT_1C\n");
         break;
      case TFU_DCI_FORMAT_1D:
      MSPD_DBG("TFU_DCI_FORMAT_1D\n");
         break;
      case TFU_DCI_FORMAT_2:					         
      MSPD_DBG("TFU_DCI_FORMAT_2\n");
         break;
      case TFU_DCI_FORMAT_3:					         
      MSPD_DBG("TFU_DCI_FORMAT_3\n");
         break;
      case TFU_DCI_FORMAT_3A:					         
      MSPD_DBG("TFU_DCI_FORMAT_3A\n");
         break;

      default :
         break;
   }
   RETVOID;
}
#endif
/**
 * @brief Handler for processing Random Access request indication 
 * recieved from PHY.
 *
 * @details
 *
 *     Function: rgSCHTomRaReqInd
 *
 *     Handler for processing Random Access request indication recieved from
 *     PHY.
 *
 *     Invoked by: RgLiTfuRaReqInd of LIM
 *
 *     Processing Steps: 
 *      - Validate the information received: cellId value and raRnti values
 *      - Process the request: Call rgSCHRamProcRaReq (cell, raRnti, raReqInd)
 *           
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] TfuRaReqIndInfo *raReqInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomRaReqInd
(
RgSchCellCb     *cell,
TfuRaReqIndInfo *raReqInd
)
#else
PUBLIC S16 rgSCHTomRaReqInd(cell, raReqInd)
RgSchCellCb     *cell; 
TfuRaReqIndInfo *raReqInd;
#endif
{
   S16           ret;
   U8            raRntiCnt;
   RgSchErrInfo  err;
   TRC2(rgSCHTomRaReqInd);

   if(cell->cellId != raReqInd->cellId)
   {
      err.errType    = RGSCHERR_TOM_RAREQIND;
      err.errCause   = RGSCHERR_TOM_INV_CELL_ID;
      RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx), 
                  "rgSCHTomRaReqInd(): No cell found with"
                 " cellId = (%d) errorType (%d) errorCause(%d)\n", 
               raReqInd->cellId, err.errType, err.errCause));
      MSPD_ERR("No cell found with"
                 " cellId = (%d) errorType (%d) errorCause(%d)\n", 
               raReqInd->cellId, err.errType, err.errCause);
      RETVALUE(RFAILED);
   } 

   for (raRntiCnt=0; raRntiCnt < raReqInd->nmbOfRaRnti; raRntiCnt++)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      if(raReqInd->rachInfoArr[raRntiCnt].raRnti > RGSCH_MAX_RA_RNTI)
      {
         RGSCHLOGERROR(cell->instIdx, ERRCLS_INT_PAR, ERG013, 
               (ErrVal)raReqInd->rachInfoArr[raRntiCnt].raRnti, 
               ("rgSCHTomRaReqInd(): raRnti  is out of range\n"));
	  MSPD_ERR("RA-RNTI is out of range \n");
         continue;
      }
#endif
      ret = rgSCHRamProcRaReq(cell, raReqInd->rachInfoArr[raRntiCnt].raRnti, 
            (TfuRachInfo *)&raReqInd->rachInfoArr[raRntiCnt],
            raReqInd->timingInfo, &err);
      if(ret == RFAILED)
      {
         err.errType = RGSCHERR_TOM_RAREQIND;
         RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
            "rgSCHTomRaReqInd(): RAM processing "
           "failed RNTI(%d) errType(%d) errCause(%d)\n", 
           raReqInd->rachInfoArr[raRntiCnt].raRnti, err.errType, err.errCause));
         MSPD_ERR("RAM processing "
           "failed RNTI(%d) errType(%d) errCause(%d)\n", 
           raReqInd->rachInfoArr[raRntiCnt].raRnti, err.errType, err.errCause);
         continue;
      }
   }
   RETVALUE(ROK);
}  /* rgSCHTomRaReqInd */


/**
 * @brief Handler for processing uplink CQI indication recieved from PHY.
 *
 * @details
 *
 *     Function: rgSCHTomUlCqiInd
 *
 *     Handler for processing uplink CQI indication recieved from PHY.
 *
 *     Invoked by: RgLiTfuUlCqiInd 
 *
 *     Processing Steps:
 *     - Gets UE
 *     - Invoke scheduler to push reported CQI info rgSCHUtlUlCqiInd 
 *           
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] TfuUlCqiIndInfo *ulCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomUlCqiInd
( 
RgSchCellCb     *cell,
TfuUlCqiIndInfo *ulCqiInd
)
#else
PUBLIC S16 rgSCHTomUlCqiInd(cell, ulCqiInd)
RgSchCellCb     *cell; 
TfuUlCqiIndInfo *ulCqiInd;
#endif
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuUlCqiRpt  *ulCqiInfo;
   TRC2(rgSCHTomUlCqiInd);

   node =  ulCqiInd->ulCqiRpt.first;
   if(cell->cellId != ulCqiInd->cellId)
   {
      RGSCHDBGERR (cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHTomUlCqiInd() Unable to get the cell with id(%d)\n", 
               ulCqiInd->cellId));
      MSPD_ERR("Unable to get the cell with id(%d)\n", 
               ulCqiInd->cellId);
      RETVALUE(RFAILED);
   }

   for (;node; node=node->next)
   {
      ulCqiInfo = (TfuUlCqiRpt *)node->node;
#if (ERRCLASS & ERRCLS_DEBUG)
      if(ulCqiInfo->numSubband == 0)
      {
         RGSCHLOGERROR(cell->instIdx, ERRCLS_INT_PAR, ERG014, 
               (ErrVal)ulCqiInfo->numSubband, 
               ("rgSCHTomUlCqiInd(): numSubband  is out of range\n"));
	 MSPD_ERR("Num Subband is out of range \n");
         continue;
      }
#endif
      if((ue = rgSCHDbmGetUeCb(cell, ulCqiInfo->rnti)) == NULLP)
      {
         RGSCHDBGERR (cell->instIdx,(rgSchPBuf(cell->instIdx),
         "rgSCHTomUlCqiInd() Unable to get the ue for rnti (%d)\n", 
                  ulCqiInfo->rnti));
         MSPD_ERR("Unable to get the ue for rnti (%d)\n", 
                  ulCqiInfo->rnti);
         continue;
      }
      /* wideband cqi is directly reported now. and also isTxPort0 */
      rgSCHUtlUlCqiInd(cell, ue, ulCqiInfo);
   }
   RETVALUE(ROK);
}  /* rgSCHTomUlCqiInd */

/**
 * @brief Handler for processing PUCCH power adjustment indication
 *
 * @details
 *
 *     Function: rgSCHTomPucchDeltaPwrInd
 *
 *     Handler for processing PUCCH power adjustment indication
 *     received from PHY.
 *
 *     Invoked by: RgLiTfuPucchDeltaPwrInd 
 *
 *     Processing Steps:
 *     - Gets UE
 *     - Invoke scheduler to push reported CQI info rgSCHUtlPucchDeltaPwrInd 
 *           
 *  @param[in] RgSchCellCb             *cell
 *  @param[in] TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomPucchDeltaPwrInd
( 
RgSchCellCb             *cell,
TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
)
#else
PUBLIC S16 rgSCHTomPucchDeltaPwrInd(cell, pucchDeltaPwr)
RgSchCellCb             *cell; 
TfuPucchDeltaPwrIndInfo *pucchDeltaPwr;
#endif
{
   RgSchUeCb       *ue;
   CmLList         *node;
   TfuPucchDeltaPwr  *ueElem;
#ifdef DEBUGP
   Inst            inst = cell->instIdx;
#endif
   TRC2(rgSCHTomPucchDeltaPwrInd);

   if(cell->cellId != pucchDeltaPwr->cellId)
   {
      RGSCHDBGERR (inst, (rgSchPBuf(inst),
               "rgSCHTomPucchDeltaPwrInd() Unable to get the cell with id(%d)\n", 
               pucchDeltaPwr->cellId));
      MSPD_ERR("Unable to get the cell with id(%d)\n", 
               pucchDeltaPwr->cellId);
      RETVALUE(RFAILED);
   }

   node =  pucchDeltaPwr->pucchDeltaPwrLst.first;
   for (;node; node=node->next)
   {
      ueElem = (TfuPucchDeltaPwr *)node->node;
      if((ue = rgSCHDbmGetUeCb(cell, ueElem->rnti)) == NULLP)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomPucchDeltaPwrInd() Unable to get the ue for rnti (%d)\n", 
                  ueElem->rnti));
         MSPD_ERR("Unable to get the ue for rnti (%d)\n", ueElem->rnti);
         continue;
      }
      rgSCHUtlPucchDeltaPwrInd(cell, ue, ueElem->pucchDeltaPwr);
   }
   RETVALUE(ROK);
}  /* rgSCHTomPucchDeltaPwrInd */

/**
 * @brief Handler for processing harq ACK/NACK indication recieved from PHY.
 *
 * @details
 *
 *     Function: rgSCHTomHarqAckInd
 *
 *     Handler for processing harq ACK/NACK indication recieved from PHY.
 *
 *     Invoked by: RgLiTfuHqInd
 *
 *     Processing Steps:
 *     For each HqAckInfo received
 *     - Get UE
 *     - If UE doesnt exist look for a RaCb and invoke rgSCHRamMsg4FdbkInd
 *     - Invoke HARQ module to pass HARQ-ACK info rgSCHDhmHqFdbkInd
 *           
 *  @param[in]  TfuHqIndInfo *harqAckInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomHarqAckInd
(
RgSchCellCb     *cell,
TfuHqIndInfo    *harqAckInd
)
#else
PUBLIC S16 rgSCHTomHarqAckInd(cell, harqAckInd)
RgSchCellCb     *cell; 
TfuHqIndInfo    *harqAckInd;
#endif
{
   RgSchErrInfo    err;
   RgSchUeCb       *ue;
   RgSchRaCb       *raCb;
   CmLList         *node;
   TfuHqInfo       *hqInfo;
   Pst             pst;
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   U8              tbCnt;
#ifdef LTE_TDD
   U8              fbCnt;
#endif
#endif 
#ifdef DEBUGP
   Inst            inst = cell->instIdx;
#endif
   RgInfRlsHqInfo  *rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);

   TRC2(rgSCHTomHarqAckInd);

   if(cell->cellId != harqAckInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomHarqAckInd() Unable to get"
                         " the cell for cellId (%d)\n", harqAckInd->cellId));
      MSPD_ERR ("Unable to get"
                         " the cell for cellId (%d)\n", harqAckInd->cellId);
      err.errType = RGSCHERR_TOM_HARQACKIND;
      err.errCause = RGSCHERR_TOM_INV_CELL_ID;
      RETVALUE(RFAILED);
   }

   rlsHqBufs->numUes = 0;
   node =  harqAckInd->hqIndLst.first;
   for (;node; node=node->next)
   {
      hqInfo = (TfuHqInfo *)node->node;
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
#ifndef LTE_TDD
      for(tbCnt=0; tbCnt<TFU_MAX_TB; tbCnt++)
      {
         hqInfo->isAck[tbCnt]=(TfuHqFdbk)rgSchTomHqFbkMap[hqInfo->isAck[tbCnt]];
      }
#else
      for(tbCnt=0; tbCnt<TFU_MAX_TB; tbCnt++)
      {
         for(fbCnt=0; fbCnt<4; fbCnt++)
         {
            hqInfo->hqFbk[tbCnt][fbCnt]=
               (TfuHqFdbk)rgSchTomHqFbkMap[hqInfo->hqFbk[tbCnt][fbCnt]];
         }
      }
#endif 
#endif 
      raCb = rgSCHDbmGetRaCb (cell, hqInfo->rnti);
      ue = rgSCHDbmGetUeCb (cell, hqInfo->rnti);
      if (ue == NULLP && raCb != NULLP)
      {
#ifdef LTEMAC_SPS 
         ue = rgSCHDbmGetSpsUeCb (cell, hqInfo->rnti);
         if (ue == NULLP)
    {
#endif
         /*rg004.201: ccpu00105882: Made changes in handling ACK for msg4. */
#ifdef RG_UNUSED
         rgSCHRamMsg4FdbkInd (raCb);
#endif
         if ((rgSCHDhmHqFdbkInd (raCb, RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB, 
                     cell, harqAckInd->timingInfo, hqInfo, rlsHqBufs, &err)) != ROK)
         {
            err.errType = RGSCHERR_TOM_HARQACKIND;
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomHarqAckInd() HARQ"
                     " feedback processing failed errType(%d) errCause(%d).\n", 
                     err.errType, err.errCause)); 
            MSPD_ERR ("rgSCHTomHarqAckInd() HARQ"
                     " feedback processing failed errType(%d) errCause(%d).\n", 
                     err.errType, err.errCause); 
            continue;
         }
#ifdef LTEMAC_SPS
         }
         else /* UE found with SPS-RNTI */
         {
            if ((rgSCHDhmHqFdbkInd (ue, RGSCH_HQ_FDB_IND_CB_TYPE_HQ_ENT, 
                        cell, harqAckInd->timingInfo, hqInfo, rlsHqBufs, &err)) != ROK)
            {
               err.errType = RGSCHERR_TOM_HARQACKIND;
               RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomHarqAckInd() HARQ feedback"
                        " processing failed errType(%d) errCause(%d).\n", 
                        err.errType, err.errCause)); 
               MSPD_ERR("HARQ feedback"
                        " processing failed errType(%d) errCause(%d).\n", 
                        err.errType, err.errCause); 
               continue;
            }
         }
#endif /* LTEMAC_SPS */
         continue;
      }
         /*rg004.201: ccpu00105882: Made changes in handling ACK for msg4. */
      else if (ue != NULLP && raCb == NULLP)
      {
         /* Get the Downlink HARQ entity from ue */
         if ((rgSCHDhmHqFdbkInd (ue, RGSCH_HQ_FDB_IND_CB_TYPE_HQ_ENT, 
                     cell, harqAckInd->timingInfo, hqInfo, rlsHqBufs, &err)) != ROK)
         {
            err.errType = RGSCHERR_TOM_HARQACKIND;
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomHarqAckInd() HARQ feedback"
                     " processing failed errType(%d) errCause(%d).\n", 
                     err.errType, err.errCause)); 
            MSPD_ERR ("HARQ feedback"
                     " processing failed errType(%d) errCause(%d).\n", 
                     err.errType, err.errCause); 
            continue;
         }
      }
      else if (ue != NULLP && raCb != NULLP)
      {
         if ((rgSCHDhmHqFdbkInd (raCb, RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB, 
                     cell, harqAckInd->timingInfo, hqInfo, rlsHqBufs, &err)) != ROK)
         {
            err.errType = RGSCHERR_TOM_HARQACKIND;
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomHarqAckInd() HARQ"
                     " feedback processing failed errType(%d) errCause(%d).\n", 
                     err.errType, err.errCause)); 
            MSPD_ERR("HARQ feedback processing failed errType(%d) errCause(%d).\n", 
                     err.errType, err.errCause); 
            continue;
         }
      }
      else
      {
#ifdef LTEMAC_SPS 
         ue = rgSCHDbmGetSpsUeCb (cell, hqInfo->rnti);
         if (ue == NULLP)
         {
#endif /* (LTEMAC_SPS & LTE_TDD) */
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomHarqAckInd() Unable to"
                  " get the UE CB or RA CB for RNTI (%d) \n", hqInfo->rnti));
         MSPD_ERR ("Unable to"
                  " get the UE CB or RA CB for RNTI (%d) \n", hqInfo->rnti);
		 err.errType = RGSCHERR_TOM_HARQACKIND;
         continue;
#ifdef LTEMAC_SPS
         }
         else /* UE found with SPS-RNTI */
         {
            if ((rgSCHDhmHqFdbkInd (ue, RGSCH_HQ_FDB_IND_CB_TYPE_HQ_ENT, 
                        cell, harqAckInd->timingInfo, hqInfo, rlsHqBufs, &err)) != ROK)
            {
               err.errType = RGSCHERR_TOM_HARQACKIND;
               RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomHarqAckInd() HARQ feedback"
                        " processing failed errType(%d) errCause(%d).\n", 
                        err.errType, err.errCause)); 
               MSPD_ERR ("HARQ feedback"
                        " processing failed errType(%d) errCause(%d).\n", 
                        err.errType, err.errCause); 
               continue;
            }
         }
#endif /* LTEMAC_SPS */
      }
   }

   /* rg001.201 removed extra comments */
   /* Check with TDD call DHM*/
   if ((rgSCHDhmRlsDlsfHqProc (cell, harqAckInd->timingInfo)) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomHarqAckInd() Unable to Release"
                    " Downlink subframe for cellId (%d))\n", harqAckInd->cellId));
      MSPD_ERR ("Unable to Release"
                    " Downlink subframe for cellId (%d))\n", harqAckInd->cellId);
      err.errType = RGSCHERR_TOM_HARQACKIND;
   }

   if(rlsHqBufs->numUes)
   {
      rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacRlsHq (&pst, rlsHqBufs);
   }
   cell->crntHqIdx++;
   cell->crntHqIdx  = cell->crntHqIdx % RGSCH_NUM_SUB_FRAMES;
   RETVALUE(ROK);
}  /* rgSCHTomHarqAckInd */


/**
 * @brief Handler for processing Scheduling Request indication 
 * recieved from PHY for a list of UEs.
 *
 * @details
 *
 *     Function: rgSCHTomSrInd
 *
 *     Handler for processing Scheduling Request indication recieved from PHY
 *     for UEs.
 *
 *     Invoked by: RgLiTfuSrInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate SR rgSCHUtlSrRcvd 
 *           
 *  @param[in]  TfuSrIndInfo *srInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomSrInd
(
RgSchCellCb     *cell,
TfuSrIndInfo    *srInd
)
#else
PUBLIC S16 rgSCHTomSrInd(cell, srInd)
RgSchCellCb     *cell; 
TfuSrIndInfo    *srInd;
#endif
{
   S16          ret = RFAILED;
   RgSchErrInfo err;
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuSrInfo    *srInfo;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHTomSrInd);

   if(cell->cellId != srInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomSrInd() Unable to get the cell"
                                        " for cellId (%d)\n", srInd->cellId));
      MSPD_ERR ("Unable to get the cell"
                                        " for cellId (%d)\n", srInd->cellId);
      err.errType = RGSCHERR_TOM_SRIND;
      err.errCause = RGSCHERR_TOM_INV_CELL_ID;
      RETVALUE(RFAILED);
   }


   node =  srInd->srLst.first;
   for (;node; node=node->next)
   {
      srInfo = (TfuSrInfo *)node->node;
      ue = rgSCHDbmGetUeCb (cell, srInfo->rnti);
      if (ue == NULLP)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomSrInd() Unable to get the UE"
                             " CB for RNTI (%d) \n", srInfo->rnti));
         MSPD_ERR ("rgSCHTomSrInd: Unable to get the UE"
                             " CB for RNTI (%d) \n", srInfo->rnti);
         continue;
      }
      rgSCHUtlHdlUlTransInd(cell, ue, srInd->timingInfo);
      MSPD_DBG("Received SR from UE %d (%d,%d)\n",srInfo->rnti, srInd->timingInfo.sfn,srInd->timingInfo.subframe);
#ifdef LTEMAC_DRX
      /*Need to activate UE as SR received*/
    if (ue->isDrxEnabled)
    {
      rgSCHDrxSrInd(cell, ue);
    }
#endif
      ret = rgSCHUtlSrRcvd (cell, ue, srInd->timingInfo, &err);
      if (ret != ROK)
      {
         err.errType = RGSCHERR_TOM_SRIND;
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomSrInd() Scheduler processing"
             " failed errType(%d) errCause(%d)\n", err.errType, err.errCause));
         MSPD_ERR ("Scheduler processing"
             " failed errType(%d) errCause(%d)\n", err.errType, err.errCause);
         continue;
      }
   }
   RETVALUE(ret);
}  /* end of rgSCHTomSrInd */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
 * @brief Handler for processing downlink CQI indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function:  rgSCHTomDoaInd  
 *
 *     Handler for processing DOA recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuDoaInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate DOA rgSCHUtlDoaInd
 *           
 *  @param[in]  TfuDoaIndInfo *doaInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomDoaInd
(
RgSchCellCb     *cell,
TfuDoaIndInfo   *doaInd
)
#else
PUBLIC S16 rgSCHTomDoaInd(cell, doaInd )
RgSchCellCb     *cell;
TfuDoaIndInfo   *doaInd;
#endif
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuDoaRpt    *doaInfo;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHTomDoaInd);

   if(cell->cellId != doaInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomDoaInd() Unable to get the"
                               " cell for cellId (%d)\n", doaInd->cellId));
      MSPD_ERR ("Unable to get the"
                               " cell for cellId (%d)\n", doaInd->cellId);
      RETVALUE(RFAILED);
   }


   node =  doaInd->doaRpt.first;
   for (;node; node=node->next)
   {
      doaInfo = (TfuDoaRpt *)node->node;
      ue = rgSCHDbmGetUeCb (cell, doaInfo->rnti);
      if (ue == NULLP)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomDoaInd() Unable to get"
                            " the UE CB for RNTI (%d) \n", doaInfo->rnti));
         MSPD_ERR ("Unable to get"
                            " the UE CB for RNTI (%d) \n", doaInfo->rnti);
         continue;
      }
      rgSCHUtlDoaInd(cell, ue, doaInfo);
   }
   RETVALUE(ROK);
}  /* rgSCHTomDoaInd */
/**
 * @brief Handler for processing downlink CQI indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomDlCqiInd
 *
 *     Handler for processing downlink CQI indication recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuDlCqiInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate DL CQI rgSCHUtlDlCqiInd
 *           
 *  @param[in]  TfuDlCqiIndInfo *dlCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomDlCqiInd
(
RgSchCellCb     *cell,
TfuDlCqiIndInfo *dlCqiInd
)
#else
PUBLIC S16 rgSCHTomDlCqiInd(cell, dlCqiInd)
RgSchCellCb     *cell;
TfuDlCqiIndInfo *dlCqiInd;
#endif
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuDlCqiRpt  *dlCqiInfo;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHTomDlCqiInd);

   if(cell->cellId != dlCqiInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomDlCqiInd() Unable to get the"
                               " cell for cellId (%d)\n", dlCqiInd->cellId));
      MSPD_ERR ("Unable to get the"
                               " cell for cellId (%d)\n", dlCqiInd->cellId);
      RETVALUE(RFAILED);
   }


   node =  dlCqiInd->dlCqiRptsLst.first;
   for (;node; node=node->next)
   {
      dlCqiInfo = (TfuDlCqiRpt *)node->node;
      ue = rgSCHDbmGetUeCb (cell, dlCqiInfo->rnti);
      if (ue == NULLP)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomDlCqiInd() Unable to get"
                            " the UE CB for RNTI (%d) \n", dlCqiInfo->rnti));
         MSPD_ERR ("Unable to get"
                            " the UE CB for RNTI (%d) \n", dlCqiInfo->rnti);
         continue;
      }
      rgSCHUtlDlCqiInd(cell, ue, dlCqiInfo, dlCqiInd->timingInfo);
      rgSCHUtlHdlUlTransInd(cell, ue, dlCqiInd->timingInfo);
   }
   RETVALUE(ROK);
}  /* rgSCHTomDlCqiInd */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE

/**
 * @brief Handler for moving PCQI instance for the next periodic occasion
 *
 * @details
 *
 *     Function: rgSCHTomUtlMovePcqiNxtOccasion
 *
 *     Handler for moving PCQI instance for the next periodic occasion
 *
 *     Invoked by: rgSCHTomUtlFill*
 *
 *     Processing Steps:
 *     - For a UE move its occurence instance to next occasion 
 *        depending on its periodicity 
 *     - Remove it from the current list and insert it to the list 
 *        having the index matching with the derived number. 
 *           
 *  @param[in]  RgSchCellCb     *cell,
 *               [in]        RgSchUeCb      *ue
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlMovePcqiNxtOccasion
(
 RgSchCellCb     *cell,
 RgSchUeCb      *ue
 )
#else
PRIVATE S16 rgSCHTomUtlMovePcqiNxtOccasion(cell, ue)
   RgSchCellCb     *cell;
   RgSchUeCb      *ue;
#endif
{
   U16   cqiIdx;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHTomUtlMovePcqiNxtOccasion);

   /* Compute Next Transmission Instance */ 
   cqiIdx = ue->cqiCb.cqiPeri + ue->cqiCb.nCqiTrIdx;
   cqiIdx = cqiIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  
   /* Delete from current List and move to new list */ 
   if (NULLP == cmLListDelFrm(&cell->pCqiSrsSrLst[ue->cqiCb.nCqiTrIdx].cqiLst,
            &ue->cqiCb.cqiLstEnt))
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlMovePcqiNxtOccasion()" 
               "Unable to remove node for UE ID  (%d) \n", ue->ueId));
      MSPD_ERR ("Unable to remove node for UE ID  (%d) \n", ue->ueId);
   }
   MSPD_DBG("CQIDBG: UE[%d] Updating cqiiTrIdx [%d:%d]\n", ue->ueId, ue->cqiCb.nCqiTrIdx, cqiIdx);
   ue->cqiCb.nCqiTrIdx = cqiIdx;
   cmLListAdd2Tail(&(cell->pCqiSrsSrLst[ue->cqiCb.nCqiTrIdx].cqiLst), 
         &(ue->cqiCb.cqiLstEnt));

   RETVALUE(ROK);
}  /* rgSCHTomUtlMovePcqiNxtOccasion */

/**
 * @brief Handler for moving RI instance for the next periodic occasion
 *
 * @details
 *
 *     Function: rgSCHTomUtlMovePriNxtOccasion
 *
 *     Handler for moving PCQI instance for the next periodic occasion
 *
 *     Invoked by: rgSCHTomUtlFill*
 *
 *     Processing Steps:
 *     - For a UE move its occurence instance to next occasion 
 *        depending on its periodicity 
 *     - Remove it from the current list and insert it to the list 
 *        having the index matching with the derived number. 
 *           
 *  @param[in]  RgSchCellCb     *cell,
 *               [in]        RgSchUeCb      *ue
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlMovePriNxtOccasion
(
 RgSchCellCb     *cell,
 RgSchUeCb      *ue
 )
#else
PRIVATE S16 rgSCHTomUtlMovePriNxtOccasion(cell, ue)
   RgSchCellCb     *cell;
   RgSchUeCb      *ue;
#endif
{
   U16   riIdx;
   U16   riDist;
   U16   effPeriodicity;
   U16   crntTime;
   U16   tempIdx; 
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHTomUtlMovePriNxtOccasion);
   crntTime = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)
      +(cell->crntTime.subframe);

   /* Compute Next Transmission Instance */ 
   if (ue->cqiCb.cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_WB_REP)
   {
      effPeriodicity = ue->cqiCb.cqiPeri * ue->cqiCb.riPeri;
      tempIdx = effPeriodicity + ue->cqiCb.nRiTrIdx;
   }
   else 
   {
      effPeriodicity = (ue->cqiCb.h * ue->cqiCb.cqiPeri * ue->cqiCb.riPeri);
      tempIdx = effPeriodicity + ue->cqiCb.nRiTrIdx;
   }
   riIdx = tempIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
   if (effPeriodicity >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
   {
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception req
       * in FDD*/
#ifdef LTE_TDD
      riDist = rgSCHUtlFindDist((U16)(crntTime + TFU_DELTA), 
      (U16)(crntTime + TFU_DELTA + effPeriodicity));  
#else
      riDist = rgSCHUtlFindDist((U16)(crntTime + TFU_RECPREQ_DLDELTA), 
      (U16)(crntTime + TFU_RECPREQ_DLDELTA + effPeriodicity));  
#endif
   }
   else
   {
      riDist =0; 
   }
   /* Delete from current List and move to new list */ 
   if (NULLP == cmLListDelFrm(&cell->pCqiSrsSrLst[ue->cqiCb.nRiTrIdx].riLst,
            &ue->cqiCb.riLstEnt))
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlMovePriNxtOccasion()"
               "Unable to remove node for UE ID (%d) \n", ue->ueId));
      MSPD_ERR ("Unable to remove node for UE ID (%d) \n", ue->ueId);
   }
   MSPD_DBG("CQIDBG: UE[%d] Updating riTrIdx [%d:%d]\n", ue->ueId, ue->cqiCb.nRiTrIdx, riIdx);
   ue->cqiCb.nRiTrIdx = riIdx;
   ue->cqiCb.riDist = riDist;
   cmLListAdd2Tail(&cell->pCqiSrsSrLst[ue->cqiCb.nRiTrIdx].riLst, 
         &ue->cqiCb.riLstEnt);

   RETVALUE(ROK);
}  /* rgSCHTomUtlMovePriNxtOccasion */

/**
 * @brief Handler for moving SR instance for the next periodic occasion
 *
 * @details
 *
 *     Function: rgSCHTomUtlMoveSrNxtOccasion
 *
 *     Handler for moving SR instance for the next periodic occasion
 *
 *     Invoked by: rgSCHTomUtlFill*
 *
 *     Processing Steps:
 *     - For a UE move its occurence instance to next occasion 
 *        depending on its periodicity 
 *     - Remove it from the current list and insert it to the list 
 *        having the index matching with the derived number. 
 *           
 *  @param[in]  RgSchCellCb     *cell,
 *               [in]        RgSchUeCb      *ue
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlMoveSrNxtOccasion
(
 RgSchCellCb     *cell,
 RgSchUeCb      *ue
 )
#else
PRIVATE S16 rgSCHTomUtlMoveSrNxtOccasion(cell, ue)
   RgSchCellCb     *cell;
   RgSchUeCb      *ue;
#endif
{
   U16   srIdx;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHTomUtlMoveSrNxtOccasion);

   /* Compute Next Transmission Instance */ 
   srIdx = ue->srCb.peri + ue->srCb.nSrTrIdx;
   srIdx = srIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  
   /* Delete from current List and move to new list */ 
   if (NULLP == cmLListDelFrm(&cell->pCqiSrsSrLst[ue->srCb.nSrTrIdx].srLst,
            &ue->srCb.srLstEnt))
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlMoveSrNxtOccasion() "
               "Unable to remove node for UE ID  (%d) \n", ue->ueId));
      MSPD_ERR ("Unable to remove node for UE ID  (%d) \n", ue->ueId);
   }
   ue->srCb.nSrTrIdx = srIdx;
   cmLListAdd2Tail(&cell->pCqiSrsSrLst[ue->srCb.nSrTrIdx].srLst, 
         &ue->srCb.srLstEnt);

   RETVALUE(ROK);
}  /* rgSCHTomUtlMoveSrNxtOccasion */

/**
 * @brief Handler for moving SRS instance for the next periodic occasion
 *
 * @details
 *
 *     Function: rgSCHTomUtlMoveSrsNxtOccasion
 *
 *     Handler for moving SRS instance for the next periodic occasion
 *
 *     Invoked by: rgSCHTomUtlFill*
 *
 *     Processing Steps:
 *     - For a UE move its occurence instance to next occasion 
 *        depending on its periodicity 
 *     - Remove it from the current list and insert it to the list 
 *        having the index matching with the derived number. 
 *           
 *  @param[in]  RgSchCellCb     *cell,
 *               [in]        RgSchUeCb      *ue
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlMoveSrsNxtOccasion
(
 RgSchCellCb     *cell,
 RgSchUeCb      *ue
 )
#else
PRIVATE S16 rgSCHTomUtlMoveSrsNxtOccasion(cell, ue)
   RgSchCellCb     *cell;
   RgSchUeCb      *ue;
#endif
{
   U16   srsIdx;
   U16   srsDist;
   U16   tempIdx;
   U16   crntTime; 
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif

   TRC2(rgSCHTomUtlMoveSrsNxtOccasion);
   crntTime = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)
      +(cell->crntTime.subframe);

   /* Compute Next Transmission Instance */ 
   tempIdx = ue->srsCb.peri + ue->srsCb.nSrsTrIdx;
   srsIdx = tempIdx %RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  
   if (ue->srsCb.peri >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
   {
#ifdef LTE_TDD
      srsDist = rgSCHUtlFindDist((U16)(crntTime + TFU_DELTA), 
      (U16)(crntTime + TFU_DELTA + ue->srsCb.peri));  
#else
      srsDist = rgSCHUtlFindDist((U16)(crntTime + TFU_RECPREQ_DLDELTA), 
      (U16)(crntTime + TFU_RECPREQ_DLDELTA + ue->srsCb.peri));  
#endif
   }
   else
   {
      srsDist =0; 
   }
   /* Delete from current List and move to new list */ 
   if (NULLP == cmLListDelFrm(&cell->pCqiSrsSrLst[ue->srsCb.nSrsTrIdx].srsLst,
            &ue->srsCb.srsLstEnt))
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlMoveSrsNxtOccasion()"
               "Unable to remove node for UE ID  (%d) \n", ue->ueId));
      MSPD_ERR ("Unable to remove node for UE ID  (%d) \n", ue->ueId);
   }
   ue->srsCb.nSrsTrIdx = srsIdx;
   ue->srsCb.srsDist = srsDist; 
   cmLListAdd2Tail(&cell->pCqiSrsSrLst[ue->srsCb.nSrsTrIdx].srsLst,
         &ue->srsCb.srsLstEnt);

   RETVALUE(ROK);
}  /* rgSCHTomUtlMoveSrsNxtOccasion */


/**
 * @brief Handler for processing RAW CQI indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomRawCqiInd
 *
 *     Handler for processing RAW CQI indication recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuRawCqiInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate Raw CQI rgSCHUtlRawCqiInd
 *           
 *  @param[in]  TfuRawCqiIndInfo *rawCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomRawCqiInd
(
 RgSchCellCb     *cell,
 TfuRawCqiIndInfo *rawCqiInd
)
#else
PUBLIC S16 rgSCHTomRawCqiInd(cell, rawCqiInd)
   RgSchCellCb     *cell;
   TfuRawCqiIndInfo *rawCqiInd;
#endif
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuRawCqiRpt* rawCqiInfo;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TfuDlCqiRpt  dlCqiInfo;
   TRC2(rgSCHTomRawCqiInd);

   if(cell->cellId != rawCqiInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomRawCqiInd() Unable to get the"
               " cell for cellId (%d)\n", rawCqiInd->cellId));
      MSPD_ERR ("Unable to get the"
               " cell for cellId (%d)\n", rawCqiInd->cellId);
      RETVALUE(RFAILED);
   }


   node =  rawCqiInd->rawCqiRpt.first;
   for (;node; node=node->next)
   {
      rawCqiInfo = (TfuRawCqiRpt *)node->node;
      ue = rgSCHDbmGetUeCb (cell, rawCqiInfo->crnti);
      if (ue == NULLP)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomRawCqiInd() Unable to get"
                  " the UE CB for RNTI (%d) \n", rawCqiInfo->crnti));
         MSPD_ERR ("Unable to get"
                  " the UE CB for RNTI (%d) \n", rawCqiInfo->crnti);
         continue;
      }
      rgSCHTomUtlDecodeRawCqi(cell, ue, &dlCqiInfo, rawCqiInfo, rawCqiInd->timingInfo);
      rgSCHUtlDlCqiInd(cell, ue, &dlCqiInfo, rawCqiInd->timingInfo);
      rgSCHUtlHdlUlTransInd(cell, ue, rawCqiInd->timingInfo);
   }
   RETVALUE(ROK);
}  /* rgSCHTomRawCqiInd */

/**
 * @brief Handler for processing SRS indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomSrsInd
 *
 *     Handler for SRS indication recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuSrsInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate UL SRS  rgSCHUtlSrsInd
 *           
 *  @param[in]  TfuSrsIndInfo *srsInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomSrsInd
(
 RgSchCellCb     *cell,
 TfuSrsIndInfo *srsInd
 )
#else
PUBLIC S16 rgSCHTomSrsInd(cell, srsInd)
   RgSchCellCb     *cell;
   TfuSrsIndInfo *srsInd;
#endif
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuSrsRpt* srsInfo;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHTomSrsInd);

   if(cell->cellId != srsInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomSrsInd() Unable to get the"
               " cell for cellId (%d)\n", srsInd->cellId));
      MSPD_ERR ("Unable to get the"
               " cell for cellId (%d)\n", srsInd->cellId);
      RETVALUE(RFAILED);
   }

   node =  srsInd->srsRpt.first;
   for (;node; node=node->next)
   {
      srsInfo = (TfuSrsRpt *)node->node;
      ue = rgSCHDbmGetUeCb (cell, srsInfo->ueId);
      if (ue == NULLP)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomSrsInd() Unable to get"
                  " the UE CB for UE ID (%d) \n", srsInfo->ueId));
         MSPD_ERR ("Unable to get"
                  " the UE CB for UE ID (%d) \n", srsInfo->ueId);
         continue;
      }
      rgSCHUtlSrsInd(cell, ue, srsInfo, srsInd->timingInfo);
      rgSCHUtlHdlUlTransInd(cell, ue, srsInd->timingInfo);
   }
   RETVALUE(ROK);
}  /* rgSCHTomSrsInd */

/**
 * @brief Function which decodes Raw PCQI Info and fills the DLCqiRpt.
 *
 * @details
 *
 *     Function: rgSCHTomUtlDecodeRawPcqi
 *
 *     Function which decodes Raw PCQI Info and fills the DLCqiRpt structure.
 *
 *     Invoked by: rgSCHTomUtlDecodeRawCqi 
 *
 *     Processing Steps:
 *     - Depending on the Raw PCQI Info stored during Reception Request
 *        decode the Raw PCQI Report 
 *     - Fill the DL CQI Report structure accordingly for the decoded Raw PCQI
 *           
 *  @param[in] RgSchCellCb     *cellCb
 *  @param[in] RgSchUeCb       *ueCb
 *  @param[out] TfuDlCqiRpt     *dlCqiRpt
 *  @param[in] TfuRawCqiRpt     *rawCqiRpt
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlDecodeRawPcqi
( 
 RgSchCellCb        *cellCb,
 RgSchUeCb          *ueCb,
 TfuDlCqiRpt        *dlCqiRpt,
 TfuRawCqiRpt       *rawCqiRpt,
 U8                 cqiRiIdx
 )
#else
PRIVATE S16 rgSCHTomUtlDecodeRawPcqi(cellCb, ueCb, dlCqiRpt, 
      rawCqiRpt, cqiRiIdx)
RgSchCellCb        *cellCb;
RgSchUeCb          *ueCb;
TfuDlCqiRpt        *dlCqiRpt;
TfuRawCqiRpt       *rawCqiRpt;
U8                 cqiRiIdx;
#endif
{
#ifdef DEBUGP
   Inst    inst = cellCb->instIdx;
#endif
   TfuCqiPucchMode10    *mode10, *puMode10;   
   TfuCqiPucchMode11    *mode11, *puMode11;   
   TfuCqiPucchMode20    *mode20, *puMode20;   
   TfuCqiPucchMode21    *mode21, *puMode21;
   TfuDlCqiPucch        *pucchCqi;

   U8 revArray[TFU_MAX_CQI_BYTES] = { '\0' };
   CONSTANT U8 leastSignBit = 0;

   TRC2(rgSCHTomUtlDecodeRawPcqi);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHTomUtlDecodeRawPcqi \n"));
   /* MS_WORKAROUND for ccpu00123929 : This step looks irrelevant. Need to check */
#if 0
#else
   cmMemcpy((U8 *)revArray, (U8 *)rawCqiRpt->cqiBits, TFU_MAX_CQI_BYTES);
#endif

   dlCqiRpt->rnti = rawCqiRpt->crnti;
   dlCqiRpt->isPucchInfo =TRUE;
   pucchCqi = &dlCqiRpt->dlCqiInfo.pucchCqi;
   switch(ueCb->rawCqiBitW[cqiRiIdx].t.pucchRawCqiBitWidth.mode)
   {
      case TFU_PUCCH_CQI_MODE10:
         {
            pucchCqi->mode = TFU_PUCCH_CQI_MODE10;
            puMode10 = &pucchCqi->u.mode10Info;
            mode10 = &ueCb->rawCqiBitW[cqiRiIdx].t.pucchRawCqiBitWidth.u.mode10Info;
            if (mode10->type == TFU_RPT_CQI)
            {
               puMode10->type = TFU_RPT_CQI;
               puMode10->u.cqi = (U8)rgSCHUtlParse(revArray,
                     leastSignBit,(U8)(leastSignBit+mode10->u.cqi), (U8)TFU_MAX_CQI_BYTES);
#ifdef MSPD
               ++dlCqiRptCnt[puMode10->u.cqi];
			   ++totDlCqiRpt;
#endif
               {
                  RgSchCmnDlUe         *ueDl = RG_SCH_CMN_GET_DL_UE(ueCb);
                  RgSchCmnDlUeCwInfo   *cw0 = &ueDl->mimoInfo.cwInfo[0];
                  RgSchCmnDlUeCwInfo   *cw1 = &ueDl->mimoInfo.cwInfo[1];
                  MSPD_DBG("[%d] DL CQI %d cw0 (%u %u %u) cw1 (%u %u %u)\n",
                        ueCb->ueId, puMode10->u.cqi, cw0->ackCnt, cw0->nackCnt, 
                        cw0->dtxCnt, cw1->ackCnt, cw1->nackCnt, cw1->dtxCnt);
                  cw0->ackCnt = cw0->nackCnt = cw0->dtxCnt = 0;
                  cw1->ackCnt = cw1->nackCnt = cw1->dtxCnt = 0;
               }
            }
            else if (mode10->type == TFU_RPT_RI)
            {
               puMode10->type = TFU_RPT_RI;
               puMode10->u.ri = (U8)(rgSCHUtlParse(&rawCqiRpt->ri,
                      leastSignBit, (U8)(leastSignBit+mode10->u.ri), (U8)1) + 1);
               MSPD_DBG("RI at [%d:%d] is %d UE(%u)\n", 
			   	cellCb->crntTime.sfn, cellCb->crntTime.subframe, puMode10->u.ri,
			   	ueCb->ueId);
            }
         }
         break;

      case TFU_PUCCH_CQI_MODE11:
         {
            pucchCqi->mode = TFU_PUCCH_CQI_MODE11;
            puMode11 = &pucchCqi->u.mode11Info;
            mode11 = &ueCb->rawCqiBitW[cqiRiIdx].t.pucchRawCqiBitWidth.u.mode11Info;
            if (mode11->type == TFU_RPT_CQI)
            {
               puMode11->type = TFU_RPT_CQI;
               puMode11->u.cqi.cqi = (U8)rgSCHUtlParse(revArray,
                     leastSignBit, (U8)(leastSignBit+mode11->u.cqi.cqi), (U8)TFU_MAX_CQI_BYTES);
               if (mode11->u.cqi.wideDiffCqi.pres)
               {
                  puMode11->u.cqi.wideDiffCqi.pres = TRUE;
                  puMode11->u.cqi.wideDiffCqi.val = 
                     (U8 )rgSCHUtlParse(revArray,(U8)(leastSignBit+mode11->u.cqi.cqi),
                      (U8)(leastSignBit+(mode11->u.cqi.cqi+mode11->u.cqi.wideDiffCqi.val)),
                      (U8)TFU_MAX_CQI_BYTES);
                  puMode11->u.cqi.pmi = (U8)rgSCHUtlParse(revArray,
                   (U8)(leastSignBit+(mode11->u.cqi.cqi+mode11->u.cqi.wideDiffCqi.val)),
                   (U8)(leastSignBit+(mode11->u.cqi.cqi+mode11->u.cqi.wideDiffCqi.val+
                   mode11->u.cqi.pmi)),(U8)TFU_MAX_CQI_BYTES);
               }
               else
               {
                  puMode11->u.cqi.wideDiffCqi.pres = FALSE;
                  puMode11->u.cqi.pmi = (U8)rgSCHUtlParse(revArray,
                   (U8)(leastSignBit+mode11->u.cqi.cqi),
                   (U8)(leastSignBit+(mode11->u.cqi.cqi+mode11->u.cqi.pmi)), 
                        (U8)TFU_MAX_CQI_BYTES);
               }
            }
            else if (mode11->type == TFU_RPT_RI)
            {
               puMode11->type = TFU_RPT_RI;
               puMode11->u.ri = (U8)(rgSCHUtlParse(&rawCqiRpt->ri,
                     leastSignBit, (U8)(leastSignBit+mode11->u.ri), 1) + 1);
            }
         }
         break;

      case TFU_PUCCH_CQI_MODE20:
         {
            pucchCqi->mode = TFU_PUCCH_CQI_MODE20;
            puMode20 = &pucchCqi->u.mode20Info;
            mode20 = &ueCb->rawCqiBitW[cqiRiIdx].t.pucchRawCqiBitWidth.u.mode20Info;
            if (mode20->type == TFU_RPT_CQI)
            {
               puMode20->type = TFU_RPT_CQI;
               if (mode20->u.cqi.isWideband)
               {
                  puMode20->u.cqi.isWideband = TRUE; 
                  puMode20->u.cqi.u.wideCqi = 
                   (U8)rgSCHUtlParse(revArray,leastSignBit,
                   (U8)(leastSignBit+mode20->u.cqi.u.wideCqi),(U8)TFU_MAX_CQI_BYTES);
               }
               else
               {
                  puMode20->u.cqi.isWideband = FALSE; 
                  puMode20->u.cqi.u.subCqi.cqi = 
                  rgSCHUtlParse(revArray,leastSignBit,
                  (U8)(leastSignBit+mode20->u.cqi.u.subCqi.cqi), (U8)TFU_MAX_CQI_BYTES);
                  puMode20->u.cqi.u.subCqi.l = 
                  rgSCHUtlParse(revArray,
                    (U8)(leastSignBit+mode20->u.cqi.u.subCqi.cqi), 
                    (U8)(leastSignBit+(mode20->u.cqi.u.subCqi.cqi+ 
                     mode20->u.cqi.u.subCqi.l)), (U8)TFU_MAX_CQI_BYTES);
               }
            }
            else if (mode20->type == TFU_RPT_RI)
            {
               puMode20->type = TFU_RPT_RI;
               puMode20->u.ri = (U8)(rgSCHUtlParse(&rawCqiRpt->ri,
                     leastSignBit, (U8)(leastSignBit +mode20->u.ri), 1)+1);
            }
         }
         break;

      case TFU_PUCCH_CQI_MODE21:
         {
            pucchCqi->mode = TFU_PUCCH_CQI_MODE21;
            puMode21 = &pucchCqi->u.mode21Info;
            mode21 = &ueCb->rawCqiBitW[cqiRiIdx].t.pucchRawCqiBitWidth.u.mode21Info;
            rgSCHTomUtlDecodeRawPcqiPucchMode21(revArray,puMode21,
                    mode21,&rawCqiRpt->ri);
         }
         break;
      default:
         break; 
   }
   RETVALUE(ROK);
}  /* rgSCHTomUtlDecodeRawPcqi */

/**
 * @brief Function which decodes Raw ACQI Info and fills the DLCqiRpt.
 *
 * @details
 *
 *     Function: rgSCHTomUtlDecodeRawACqi
 *
 *     Function which decodes Raw ACQI Info and fills the DLCqiRpt structure.
 *
 *     Invoked by: rgSCHTomUtlDecodeRawCqi 
 *
 *     Processing Steps:
 *     - Depending on the Raw ACQI Info stored during Reception Request
 *        decode the Raw CQI Report 
 *     - Fill the DL CQI Report structure accordingly for the decoded Raw ACQI
 *           
 *  @param[in] RgSchCellCb     *cellCb
 *  @param[in] RgSchUeCb       *ueCb
 *  @param[out] TfuDlCqiRpt     *dlCqiRpt
 *  @param[in] TfuRawCqiRpt     *rawCqiRpt
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlDecodeRawAcqi
( 
 RgSchCellCb        *cellCb,
 RgSchUeCb          *ueCb,
 TfuDlCqiRpt        *dlCqiRpt,
 TfuRawCqiRpt       *rawCqiRpt,
 U8                 cqiRiIdx
 )
#else
PRIVATE S16 rgSCHTomUtlDecodeRawAcqi(cellCb, ueCb, dlCqiRpt, 
      rawCqiRpt, cqiRiIdx)
RgSchCellCb        *cellCb;
RgSchUeCb          *ueCb;
TfuDlCqiRpt        *dlCqiRpt;
TfuRawCqiRpt       *rawCqiRpt;
U8                 cqiRiIdx;
#endif
{
#ifdef DEBUGP
   Inst    inst = cellCb->instIdx;
#endif
   RgSchCqiRawPuschMode12   *mode12Info;    
   RgSchCqiRawPuschMode20   *mode20Info;    
   RgSchCqiRawPuschMode22   *mode22Info;   
   RgSchCqiRawPuschMode30   *mode30Info;    
   RgSchCqiRawPuschMode31   *mode31Info;
   TfuDlCqiPusch          *puschCqi;
   TfuCqiPuschMode12   *psMode12;    
   TfuCqiPuschMode20   *psMode20;    
   TfuCqiPuschMode22   *psMode22;   
   TfuCqiPuschMode30   *psMode30;    
   TfuCqiPuschMode31   *psMode31;    
   U8 ri;
   U8 loop, sbOffst, indx;
   U8 revArray[TFU_MAX_CQI_BYTES];
   CONSTANT U8 leastSignBit = 0;
   U32 label =0, numTxAnt =0;

   TRC2(rgSCHTomUtlDecodeRawAcqi);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHTomUtlDecodeRawAcqi \n"));

   indx = TFU_MAX_CQI_BYTES-1; 
   /*Reversing the Array so as to facilitate bit Parsing */
   for(loop=0;loop<TFU_MAX_CQI_BYTES;loop++)
   {
      revArray[loop]=rawCqiRpt->cqiBits[indx];
      indx--;
   }
   numTxAnt = cellCb->numTxAntPorts;
   dlCqiRpt->rnti = rawCqiRpt->crnti;
   dlCqiRpt->isPucchInfo =FALSE;
   puschCqi = &dlCqiRpt->dlCqiInfo.puschCqi;
   if((ueCb->mimoInfo.txMode != RGR_UE_TM_3) && 
         (ueCb->mimoInfo.txMode != RGR_UE_TM_4))
   {
      ri =1;
   }
   else
   {
      puschCqi->ri.pres = TRUE;
      ri =(rgSCHUtlParse(&rawCqiRpt->ri, 0, ueCb->acqiCb.riNumBits, 1)+1);
      puschCqi->ri.val = ri;
   }      
   switch(ueCb->rawCqiBitW[cqiRiIdx].t.puschRawCqiBitWidth.mode)
   {
      case TFU_PUSCH_CQI_MODE_12:
         {
            puschCqi->mode = TFU_PUSCH_CQI_MODE_12;
            psMode12 = &puschCqi->u.mode12Info;
            mode12Info = &ueCb->rawCqiBitW[cqiRiIdx].t.puschRawCqiBitWidth.u.mode12Info;
            rgSCHTomUtlDecodeRawAcqiPuschMode12(ueCb,revArray,
                             mode12Info,psMode12,ri,numTxAnt);
         }
         break;

      case TFU_PUSCH_CQI_MODE_20:
         {
            puschCqi->mode = TFU_PUSCH_CQI_MODE_20;
            psMode20 = &puschCqi->u.mode20Info;
            psMode20->numSubband = ueCb->acqiCb.M;
            mode20Info = &ueCb->rawCqiBitW[cqiRiIdx].t.puschRawCqiBitWidth.u.mode20Info;
            psMode20->wideBandCqi= rgSCHUtlParse(revArray,
                  leastSignBit, (U8)(leastSignBit+mode20Info->wideBCqiCw), TFU_MAX_CQI_BYTES);
            psMode20->cqiIdx = rgSCHUtlParse(revArray,
                  (U8)(leastSignBit+mode20Info->wideBCqiCw),
                  (U8)(leastSignBit+(mode20Info->wideBCqiCw+mode20Info->subBandDiffCqi)), 
                  TFU_MAX_CQI_BYTES);
            /*Extract and Store Pos of M selected Subbands */
            label = rgSCHUtlParse(revArray,\
                  (U8)(leastSignBit+ (mode20Info->wideBCqiCw+mode20Info->subBandDiffCqi)), 
                  (U8)(leastSignBit+ (mode20Info->wideBCqiCw+mode20Info->subBandDiffCqi+
                         mode20Info->posOfM)), TFU_MAX_CQI_BYTES);
               /*Arg-5 has been not been casted to TfuSubbandInfo as this is  
                * defined only under MIMO */
            rgSCHTomUtlGenIndices(label, ueCb->acqiCb.M, 
                  ueCb->acqiCb.N, ueCb->acqiCb.k, psMode20->subbandArr);
         }
         break;

      case TFU_PUSCH_CQI_MODE_22:
         {
            puschCqi->mode = TFU_PUSCH_CQI_MODE_22;
            psMode22 = &puschCqi->u.mode22Info;
            mode22Info = &ueCb->rawCqiBitW[cqiRiIdx].t.puschRawCqiBitWidth.u.mode22Info;
            rgSCHTomUtlDecodeRawAcqiPuschMode22(ueCb,revArray,
                             mode22Info,psMode22,ri,numTxAnt);
         }
         break;

      case TFU_PUSCH_CQI_MODE_30:
         {
            puschCqi->mode = TFU_PUSCH_CQI_MODE_30;
            psMode30 = &puschCqi->u.mode30Info;
            mode30Info = &ueCb->rawCqiBitW[cqiRiIdx].t.puschRawCqiBitWidth.u.mode30Info;
            psMode30->wideBandCqi = rgSCHUtlParse(revArray,
                  leastSignBit, (U8)(leastSignBit+mode30Info->wideBCqiCw), TFU_MAX_CQI_BYTES);
            psMode30->numSubband = ueCb->acqiCb.N; 
            sbOffst = leastSignBit+mode30Info->wideBCqiCw;
            for(loop =0; loop<mode30Info->totLenSbDiffCqi/2; loop++)
            {
               psMode30->subbandArr[loop].cqi = 
                  rgSCHUtlParse(revArray,sbOffst,
                        (U8)(sbOffst+2), (U8)TFU_MAX_CQI_BYTES);
               sbOffst+=2;
               psMode30->subbandArr[loop].subBand.numRb = ueCb->acqiCb.k;
               psMode30->subbandArr[loop].subBand.rbStart = 
                  ueCb->acqiCb.k * loop;
            }
         }
         break;

      case TFU_PUSCH_CQI_MODE_31:
         {
            puschCqi->mode = TFU_PUSCH_CQI_MODE_31;
            psMode31 = &puschCqi->u.mode31Info;
            mode31Info =  &ueCb->rawCqiBitW[cqiRiIdx].t.puschRawCqiBitWidth.u.mode31Info;
            rgSCHTomUtlDecodeRawAcqiPuschMode31(ueCb,revArray,
                             mode31Info,psMode31,ri,numTxAnt);
            
         }
         break;
      default:
         break; 
   }
   RETVALUE(ROK);
}  /* rgSCHTomUtlDecodeRawAcqi */

/**
 * @brief Function which decodes Raw CQI Info and fills the DLCqiRpt.
 *
 * @details
 *
 *     Function: rgSCHTomUtlDecodeRawCqi
 *
 *     Function which decodes Raw CQI Info and fills the DLCqiRpt structure.
 *
 *     Invoked by: rgSCHTomRawCqiInd 
 *
 *     Processing Steps:
 *     - Depending on the Raw CQI Info stored during Reception Request
 *        decode the Raw CQI Report 
 *     - Fill the DL CQI Report structure accordingly for the decoded Raw CQI
 *           
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] TfuUlCqiIndInfo *ulCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlDecodeRawCqi
( 
 RgSchCellCb        *cellCb,
 RgSchUeCb          *ueCb,
 TfuDlCqiRpt           *dlCqiRpt,
 TfuRawCqiRpt      *rawCqiRpt,
 CmLteTimingInfo    timingInfo
 )
#else
PRIVATE S16 rgSCHTomUtlDecodeRawCqi(cellCb, ueCb, dlCqiRpt, 
rawCqiRpt, timingInfo)
   RgSchCellCb        *cellCb;
   RgSchUeCb          *ueCb;
   TfuDlCqiRpt           *dlCqiRpt;
   TfuRawCqiRpt      *rawCqiRpt;
   CmLteTimingInfo    timingInfo;
#endif
{
#ifdef DEBUGP
   Inst    inst = cellCb->instIdx;
#endif
   U8     cqiRiIdx;
   U8     Indx;
   TRC2(rgSCHTomUtlDecodeRawCqi);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHTomUtlDecodeRawCqi \n"));

   dlCqiRpt->rnti = rawCqiRpt->crnti;
   for(Indx = ueCb->cqiRiReadIdx; 
       Indx < (ueCb->cqiRiReadIdx + MAX_CQI_RI_RPT_BUFF); 
       Indx++)
   {
      cqiRiIdx = Indx % MAX_CQI_RI_RPT_BUFF;
      if((timingInfo.sfn == ueCb->rawCqiBitW[cqiRiIdx].recvTime.sfn) &&
         (timingInfo.subframe == ueCb->rawCqiBitW[cqiRiIdx].recvTime.subframe))
   {
         break;
      }
   }
   if(Indx == (ueCb->cqiRiReadIdx + MAX_CQI_RI_RPT_BUFF))
   {
      MSPD_DBG("ERR: Cqi time not found");
      RETVALUE(ROK);
   }

   if (ueCb->rawCqiBitW[cqiRiIdx].type == TFU_RECP_REQ_PUCCH)
   {
      rgSCHTomUtlDecodeRawPcqi(cellCb, ueCb, dlCqiRpt, rawCqiRpt, cqiRiIdx);
   }
   else  /*PUSCH*/
   {
      rgSCHTomUtlDecodeRawAcqi(cellCb, ueCb, dlCqiRpt, rawCqiRpt, cqiRiIdx);
   }
   
   /* update the read index*/
   RG_SCH_INCR_CQIRI_INDEX(cqiRiIdx);
   ueCb->cqiRiReadIdx = cqiRiIdx;
   RETVALUE(ROK);
}  /* rgSCHTomUtlDecodeRawCqi */

/*
*
*       Fun:   rgSCHTomUtlGenIndices
*
*       Desc:  This function reconstructs the Subband Indices for
*       of M selected Subbands conveyed by the UE for APeriodic Modes
*       2-0 and 2-2. It decodes the Label which uniquely encodes M out
*       of N subbands.
*                   
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  rg_sch_utl.c
*
*/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlGenIndices
(
 U32      label,
 U8        posM,
 U8        valN,
 U8        valK,
 TfuSubbandInfo*    sbInfo
 )
#else
PRIVATE S16 rgSCHTomUtlGenIndices(label, posM, valN, valK, sbInfo)
   U32      label;
   U8        posM;
   U8        valN;
   U8        valK;
   TfuSubbandInfo*    sbInfo;
#endif
{
   U8 idx, kval, xval, xmin;
   U32 binCoe;
   xmin =1;
   for(kval=0; kval<posM;kval++)
   {
      xval = xmin;
      binCoe = rgSCHTomBinCoe[posM-kval-1][valN-xval-1];
      while(binCoe>label)
      {
         xval = xval+1;
         binCoe = rgSCHTomBinCoe[posM-kval-1][valN-xval-1];
      }
      idx = xval;
      sbInfo[kval].numRb = valK;
      sbInfo[kval].rbStart = idx*valK;
      xmin = idx+1;
      label = label-binCoe;
   }
   RETVALUE(ROK);
} /* end of rgSCHTomUtlGenIndices*/
#endif 
/**
 * @brief Handler for processing decode failure indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomCrcInd
 *
 *     Handler for processing decode failure indication recieved from 
 *     PHY for a set of UEs.
 *
 *     Invoked by: RgLiTfuCrcInd of rg_sch.x 
 *
 *     Processing Steps:
 *     - Validate the information received and retrieve cell and ue.
 *     - Process Decode failure Indication: Call rgSCHUhmProcHqFailure().
 *           
 *  @param[in]  TfuCrcIndInfo *crcInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomCrcInd
(
RgSchCellCb       *cell,
TfuCrcIndInfo *crcInd
)
#else
PUBLIC S16 rgSCHTomCrcInd(cell, crcInd)
RgSchCellCb       *cell;
TfuCrcIndInfo *crcInd;
#endif
{
   RgSchUeCb      *ue;
   RgSchRaCb      *raCb = NULLP;
   CmLList        *node;
   TfuCrcInfo *crcInfo;
/*rg002.301 - Add - [ccpu00118352] - UL Scheduling at CRC Indication */
#ifndef LTE_TDD
#ifdef RG_ULSCHED_AT_CRC
   RgSchErrInfo    err;
   RgSchDlSf       *ulSf;
   CmLteTimingInfo crntFrm;
   RgSchCmnUlCell  *cellUl; 
#endif
#endif
#ifdef DEBUGP
   Inst           inst = cell->instIdx;
#endif
   TRC2(rgSCHTomCrcInd);

   if(cell->cellId != crcInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomCrcInd() Unable to get the"
                           " cell for cellId (%d)\n", crcInd->cellId));
      MSPD_ERR ("Unable to get the"
                           " cell for cellId (%d)\n", crcInd->cellId);
      RETVALUE(RFAILED);
   }
#ifdef MSPD
{
    static CmLteTimingInfo lastCrc = {2000,0};
    CmLteTimingInfo crntCrc = cell->crntTime;
    if (RGSCH_TIMEINFO_SAME(lastCrc, crntCrc))
    {

       /*Removed the WA to drop 2nd CRC*/
       MSPD_DBG("Recieved CRC twice per TTI @(%u,%u)\n",
            cell->crntTime.sfn, cell->crntTime.subframe);
    }
    
    lastCrc = crntCrc;
}
#endif

   node =  crcInd->crcLst.first;
   for (;node; node=node->next)
   {
      crcInfo = (TfuCrcInfo*)node->node;
      ue = rgSCHDbmGetUeCb (cell, crcInfo->rnti);
      if (ue == NULLP)
      {
#ifdef LTEMAC_SPS
         /* Fetch from SPS List */
         ue = rgSCHDbmGetSpsUeCb(cell, crcInfo->rnti);
         if (ue == NULLP)         
#endif 
    {
       raCb = rgSCHDbmGetRaCb (cell, crcInfo->rnti);
       if (raCb == NULLP)
       {
           RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), "rgSCHTomCrcInd():"
                     "  Unable to get the UE CB for RNTI (%d) \n",  crcInfo->rnti));
          continue;
       }
    }
      }
      if (crcInfo->isFailure == FALSE)
      {
         if(raCb)
         {
            rgSCHRamMsg3DatInd(raCb);
#ifdef LTE_TDD
            RGSCH_UPD_PHICH(cell->ulDlCfgIdx, crcInd->timingInfo.subframe,
                  raCb->msg3HqProcRef);
#endif
            raCb = NULLP;
         }
         else
         {
            RGSCHCPYTIMEINFO(crcInd->timingInfo, ue->datIndTime);
#ifndef MAC_SCH_STATS
            rgSCHUhmProcDatInd(cell, ue, crcInd->timingInfo);

#else 
   /** Stats update over here 
    */
   {
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ue->sch;
      rgSCHUhmProcDatInd(cell, ue, crcInd->timingInfo, cmnUe->ul.crntUlCqi[0]);
   }
#endif /* MAC_SCH_STATS */

#ifdef LTEMAC_SPS
            rgSCHUtlHdlCrcInd(cell, ue, crcInd->timingInfo);
#endif
         }
      }
      else
      {
         if(raCb)
         {
            rgSCHRamMsg3FailureInd(raCb);
#ifdef LTE_TDD
            RGSCH_UPD_PHICH(cell->ulDlCfgIdx, crcInd->timingInfo.subframe,
                  raCb->msg3HqProcRef);
#endif
            raCb = NULLP;
         }
         else
         {
#ifndef MAC_SCH_STATS
            rgSCHUhmProcHqFailure (cell, ue, crcInd->timingInfo, crcInfo->rv);
#else
	    {
               RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ue->sch;
               rgSCHUhmProcHqFailure (cell, ue, crcInd->timingInfo, crcInfo->rv, cmnUe->ul.crntUlCqi[0]);
	    }
#endif /* MAC_SCH_STATS */
            rgSCHUtlHdlUlTransInd(cell, ue, crcInd->timingInfo);
#ifdef LTEMAC_SPS
            rgSCHUtlHdlCrcFailInd(cell, ue, crcInd->timingInfo);
#endif
         }
      }
   }
/*rg002.301 - Add - [ccpu00118352] - UL Scheduling at CRC Indication */
#ifndef LTE_TDD
#ifdef RG_ULSCHED_AT_CRC
	/* Changes to do uplink scheduling at CRC Indication */
	cellUl = RG_SCH_CMN_GET_UL_CELL(cell);

   /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for UL control */
	RGSCHADDTOCRNTTIME(cell->crntTime, crntFrm, TFU_ULCNTRL_DLDELTA);
	
   /* Append the PHICH information */
	if(cellUl->hqFdbkIdx != RGSCH_INVALID_INFO)
	{   
	   if ((rgSCHUhmAppendPhich(cell, crntFrm))!= ROK)
	   {
	      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomCrcInd() Unable to append"
					 " PHICH cellId (%d))\n", crcInd->cellId));
	      MSPD_ERR ("Unable to append PHICH cellId (%d))\n", crcInd->cellId);
	   }
	 }

	if(cellUl->reTxIdx != RGSCH_INVALID_INFO)
	{
	   rgSCHCmnRlsUlSf(cell);
	}

	rgSCHCmnUlTti(cell, &err);

	/* Also, sending UL DCI and PHICH for just scheduled subframe */
	ulSf = rgSCHUtlSubFrmGet (cell, crntFrm);

	if ((rgSCHTomUtlProcDlSfAtCrc (ulSf, cell, &err)) != ROK)
	{
	  RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomCrcInd() Unable to process"
	              " downlink subframe for cellId (%d))\n", crcInd->cellId));
	  MSPD_ERR ("Unable to process"
	              " downlink subframe for cellId (%d))\n", crcInd->cellId);
	  err.errType = RGSCHERR_TOM_TTIIND;
	  RETVALUE(RFAILED);
	}
#endif
#endif
   RETVALUE(ROK);
}  /* rgSCHTomCrcInd */

/**
 * @brief Handler for processing timing Advance indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomTimingAdvInd
 *
 *     Handler for processing timing advance indication recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuTimingAdvInd
 *
 *     Processing Steps:
 *     - Get UE.
 *     - Call DHM to update value of Timing Advance rgSCHDhmUpdTa.
 *           
 *  @param[in] TfuTimingAdvIndInfo *timingAdvInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomTimingAdvInd
(
RgSchCellCb         *cell,
TfuTimingAdvIndInfo *timingAdvInd
 )
#else
PUBLIC S16 rgSCHTomTimingAdvInd(cell, timingAdvInd)
RgSchCellCb         *cell;
TfuTimingAdvIndInfo *timingAdvInd;
#endif
{
   RgSchUeCb        *ue;
   CmLList          *node;
   TfuTimingAdvInfo *timingAdvInfo;
#ifdef DEBUGP
   Inst             inst = cell->instIdx;
#endif

   TRC2(rgSCHTomTimingAdvInd);

   if(cell->cellId != timingAdvInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomTimingAdvInd() Unable to get"
                        " the cell for cellId (%d)\n", timingAdvInd->cellId));
      MSPD_ERR ("Unable to get"
                        " the cell for cellId (%d)\n", timingAdvInd->cellId);
      RETVALUE(RFAILED);
   }


   node =  timingAdvInd->timingAdvLst.first;
   for (;node; node=node->next)
   {
      timingAdvInfo = (TfuTimingAdvInfo *)node->node;
      ue = rgSCHDbmGetUeCb (cell, timingAdvInfo->rnti);
      if (ue == NULLP)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomTimingAdvInd() Unable to"
                      " get the UE CB for RNTI (%d) \n", timingAdvInfo->rnti));
         MSPD_ERR ("Unable to"
                      " get the UE CB for RNTI (%d) \n", timingAdvInfo->rnti);
         continue;
      }
      rgSCHDhmUpdTa (cell, ue, timingAdvInfo->timingAdv);
   }
   RETVALUE(ROK);
}  /* rgSCHTomTimingAdvInd */

#ifdef LTE_TDD
/**
 * @brief Handler for processing TTI indication recieved from
 * PHY for a cell.
 *
 * @details
 *
 *     Function: rgSCHTomTtiInd
 *
 *     Handler for processing TTI indication recieved from PHY
 *     for a cell.
 *
 *     Invoked by: RgLiTfuTtiInd
 *
 *     Processing Steps:
 *     - Get cell and update the cell's current time with the timing value given
 *     by PHY
 *     - Invoke the cmPrcTmr to process the timing queue.
 *     - Append the PHICH information to the downlink subframe that needs to go
 *       out to PHY in this subframe.
 *     - Invoke DHM to release the downlink subframe that occured earlier
 *     rgSCHDhmRlsDlsfHqProc.
 *     - Invoke the TTI handler of scheduler.
 *     - Invoke the TTI handler of RAM module.
 *     - Get the downlink subframe that has to go out to PHY in this subframe
 *     rgSCHUtlSubFrmGet.
 *     - Invoke rgSCHTomUtlProcTA to perform and timing advance processing.
 *     - Invoke rgSCHTomUtlProcDlSf to do further processing on the downlink
 *     subframe.
 *     - Get the downlink subframe that would occur after RGSCH_DL_DELTA and
 *     invoke rgSCHTomUtlProcDlSfStaInd to send status indications to the higher
 *     layer.
 *     - Invoke GOM's TTI handler rgSCHGomTtiHndlr
 *     - Invoke COM's TTI handler rgCOMTtiHndlr
 *
 *  @param[in] RgSchCellCb   *cell
 *  @param[in] SuId           suId
 *  @param[in] TfuTtiIndInfo *ttiInd
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomTtiInd
(
RgSchCellCb       *cell,
SuId              suId,
TfuTtiIndInfo     *ttiInd
)
#else
PUBLIC S16 rgSCHTomTtiInd(cell, suId, ttiInd)
RgSchCellCb       *cell;
SuId              suId;
TfuTtiIndInfo     *ttiInd;
#endif
{
   S16                  ret;
   RgSchErrInfo         err;
   RgSchDlSf            *dlSf;
   CmLteTimingInfo      crntFrm;
   CmLteTimingInfo      frm;
   U8                   ulDlCfgIdx;
   RgSchCmnUlCell       *cellUl; 
   /* rg001.201: Added for sending TTI tick to RRM */
#ifdef RGR_RRM_TICK
   RgrTtiIndInfo        *rgrTtiInd;
#endif

   TRC2(rgSCHTomTtiInd);

   if (cell->cellId != ttiInd->cellId)
   {
      RGSCHDBGERR (cell->instIdx,(rgSchPBuf(cell->instIdx),"rgSCHTomTtiInd() Unable to get"
                           " the cell for cellId (%d))\n", ttiInd->cellId));
     MSPD_ERR ("Unable to get the cell for cellId (%d))\n", ttiInd->cellId);
      err.errType = RGSCHERR_TOM_TTIIND;
      err.errCause = RGSCHERR_TOM_INV_CELL_ID;
      RETVALUE(RFAILED);
   }

   ulDlCfgIdx = cell->ulDlCfgIdx;
   cellUl = RG_SCH_CMN_GET_UL_CELL(cell);

   /* increment the crntTime in cell */
   RGSCHCPYTIMEINFO(ttiInd->timingInfo, cell->crntTime);

   /* Update the TTI type into Cell */
   cell->isDummyTti = ttiInd->isDummyTti;

   /* rg001.201: Added for sending TTI tick to RRM */
#ifdef RGR_RRM_TICK
   /* TTI to be sent to RRM only once per system frame */
   /* rg002.201: Added support for period = 0 to disable tick to RRM */
   if ((cell->rrmTtiIndPrd != 0) && 
         ((ttiInd->timingInfo.sfn % cell->rrmTtiIndPrd) == 0) && 
         (ttiInd->timingInfo.subframe == 0))
   {
      /* Allocate a TTI indication structure and send to RRM over RGR interface */
      if (rgSCHUtlAllocSBuf (cell->instIdx,
               (Data**)&rgrTtiInd, sizeof(RgrTtiIndInfo)) != ROK)
      {
         RGSCHDBGERR (cell->instIdx,(rgSchPBuf(cell->instIdx),
            "rgSCHTomTtiInd() Unable to allocate memory for RGR TTI \
            indication for cellId (%d))\n", ttiInd->cellId));
         MSPD_ERR ("Unable to allocate memory for RGR TTI \
            indication for cellId (%d))\n", ttiInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
         err.errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(RFAILED);
      }
      rgrTtiInd->cellId = ttiInd->cellId;
      rgrTtiInd->sfn = ttiInd->timingInfo.sfn;
      if (rgSCHUtlRgrTtiInd (cell, rgrTtiInd) != ROK)
      {
         RGSCHDBGERR (cell->instIdx,(rgSchPBuf(cell->instIdx),
            "rgSCHTomTtiInd() Unable to Send RGR TTI indication to RRM\
            for cellId (%d))\n", ttiInd->cellId));
         MSPD_ERR ("Unable to Send RGR TTI indication to RRM\
            for cellId (%d))\n", ttiInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, (Data**)&rgrTtiInd,
               sizeof(RgrTtiIndInfo));
         RETVALUE(RFAILED);
      }
   }
#endif

   cmPrcTmr(&cell->tqCp, cell->tq, (PFV)rgSCHTmrProcTmr);

   /*rg003.301[ccpu00121751] ADD call to measuring gap module */
   rgSCHMeasGapANRepTtiHndl (cell);

   /* We need to fill in the PHICH for the UL Data, first we need to get the UL
    * SF from Scheduler, next we fill in that in the dlSf thats going to go out
    * this TTI. This must be performed prior to any other processing of the TTI
    * so that we do not wrap around and generate feedback prior to reception of
    * UL data.
    */

   RGSCHADDTOCRNTTIME(ttiInd->timingInfo, crntFrm, TFU_DELTA);
   if(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][crntFrm.subframe] != 
                                          RG_SCH_TDD_UL_SUBFRAME)
   {
      if(cellUl->hqFdbkIdx != RGSCH_INVALID_INFO)
      {
         if ((ret = rgSCHUhmAppendPhich(cell, crntFrm))!= ROK)
         {
            RGSCHDBGERR (cell->instIdx, (rgSchPBuf(cell->instIdx),
                     "rgSCHTomTtiInd() Unable to append PHICH cellId (%d))\n",
                     ttiInd->cellId));
            MSPD_ERR ("Unable to append PHICH cellId (%d))\n",
                     ttiInd->cellId);
            err.errType = RGSCHERR_TOM_TTIIND;
         }
         rgSCHCmnRlsUlSf(cell);
         cellUl->hqFdbkIdx = RGSCH_INVALID_INFO;
         cellUl->reTxIdx = RGSCH_INVALID_INFO;
      }
   }

     /*rg002.301 ccpu00119494-ADD-added handling to retransmit
      * release PDCCH in case of DTX
      */
#ifdef LTEMAC_SPS
     rgSCHCmnSpsDlReTxRelPdcch(cell);
#endif

   if(RGSCH_INVALID_INFO !=
      rgSchTddRlsDlSubfrmTbl[cell->ulDlCfgIdx][cell->crntTime.subframe])
   {
      rgSCHDhmTddRlsSubFrm(cell);

      RGSCHSUBFRMCRNTTIME(cell->crntTime, frm, 
            rgSchTddRlsDlSubfrmTbl[cell->ulDlCfgIdx][cell->crntTime.subframe]); 
      if ((ret = rgSCHDhmRlsDlsfHqProc (cell, frm)) != ROK)
      {
         RGSCHDBGERR (cell->instIdx, 
               (rgSchPBuf(cell->instIdx),"rgSCHTomTtiInd() Unable to Release "
                  "Downlink subframe for cellId (%d))\n",
                  ttiInd->cellId));
         MSPD_ERR ("Unable to Release "
                  "Downlink subframe for cellId (%d))\n",
                  ttiInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
      }
   }

#ifdef LTEMAC_DRX
   rgSCHDrxTtiInd(cell);
#endif /*LTEMAC_DRX*/

   /*ccpu00117180 - ADD - Call rgSCHCmnUpdVars in TTI IND */
   rgSCHCmnUpdVars(cell);

   ret = rgSCHUtlTti (cell, &err);
   if (ret != ROK)
   {
      err.errType = RGSCHERR_TOM_TTIIND;
      RGSCHDBGERR (cell->instIdx,
            (rgSchPBuf(cell->instIdx),"rgSCHTomTtiInd() Scheduler processing \
               failed errType(%d) errCause(%d)\n",
               err.errType, err.errCause));
      MSPD_ERR ("Scheduler processing \
               failed errType(%d) errCause(%d)\n",
               err.errType, err.errCause);
   }

   /* Invoke RAM  */
   rgSCHRamTtiHndlr(cell);

   if(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][crntFrm.subframe] != 
                                       RG_SCH_TDD_UL_SUBFRAME)
   {
      /* Processing steps
       * 1. Get the downlink subframe for this tti from DBM.
       * 2. Send BCH Data to phy (inside downlink subframe).
       * 3. Invoke DHM to send scheduled data of all the scheduled HQ Procs
       * (inside downlink subframe) to PHY
       */
      /* Need to get the crnt time PLUS TFU_DELTA */
      RGSCHADDTOCRNTTIME(ttiInd->timingInfo, crntFrm, TFU_DELTA);
      dlSf = rgSCHUtlSubFrmGet (cell, crntFrm);

      if (rgSCHTomUtlProcTA (cell) != ROK)
      {
         RGSCHDBGERR (cell->instIdx,
               (rgSchPBuf(cell->instIdx),"rgSCHTomTtiInd() Unable Process Timing Adjustment\
                  for cellId (%d))\n", ttiInd->cellId));
         MSPD_ERR ("Unable Process Timing Adjustment\
                  for cellId (%d))\n", ttiInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
      }

      if ((ret = rgSCHTomUtlProcDlSf (dlSf, cell, &err)) != ROK)
      {
         RGSCHDBGERR (cell->instIdx,
               (rgSchPBuf(cell->instIdx),"rgSCHTomTtiInd() Unable to \
                  process downlink subframe \
                  for cellId (%d))\n", ttiInd->cellId));
         MSPD_ERR ("Unable to \
                  process downlink subframe \
                  for cellId (%d))\n", ttiInd->cellId);

         err.errType = RGSCHERR_TOM_TTIIND;
      }
      /* Mark this frame as sent */
      dlSf->txDone = TRUE;
   }
   else
   {

      if ((ret = rgSCHTomUtlProcUlSf (cell, &err)) != ROK)
      {
         /* fill in err type and call sta ind */
         RGSCHDBGERR (cell->instIdx, (rgSchPBuf(cell->instIdx),
                  "rgSCHTomTtiInd() Unable to process Uplink subframe\
                  for cellId (%d))\n", ttiInd->cellId));
         MSPD_ERR ("Unable to process Uplink subframe\
                  for cellId (%d))\n", ttiInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
      }
   }

   rgSCHGomTtiHndlr(cell, suId);

   RETVALUE(ret);
}  /* rgSCHTomTtiInd */
#else
/**
 * @brief Handler for processing TTI indication recieved from 
 * PHY for a cell.
 *
 * @details
 *
 *     Function: rgSCHTomTtiInd
 *
 *     Handler for processing TTI indication recieved from PHY
 *     for a cell.
 *
 *     Invoked by: RgLiTfuSchTtiInd
 *
 *     Processing Steps:
 *     - Get cell and update the cell's current time with the timing value given
 *     by PHY
 *     - Invoke the cmPrcTmr to process the timing queue.
 *     - Append the PHICH information to the downlink subframe that needs to go
 *       out to PHY in this subframe.
 *     - Invoke DHM to release the downlink subframe that occured earlier
 *     rgSCHDhmRlsDlsfHqProc.
 *     - Invoke the TTI handler of scheduler. 
 *     - Invoke the TTI handler of RAM module. 
 *     - Get the downlink subframe that has to go out to PHY in this subframe
 *     rgSCHUtlSubFrmGet.
 *     - Invoke rgSCHTomUtlProcTA to perform and timing advance processing. 
 *     - Invoke rgSCHTomUtlProcDlSf to do further processing on the downlink
 *     subframe.
 *     - Invoke GOM's TTI handler rgSCHGomTtiHndlr
 *     - Invoke COM's TTI handler rgCOMTtiHndlr
 *           
 *  @param[in] TfuTtiIndInfo *ttiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHTomTtiInd
(
RgSchCellCb        *cell,
SuId               suId,
TfuTtiIndInfo      *ttiInd
)
#else
PUBLIC S16 rgSCHTomTtiInd(cell, suId, ttiInd)
RgSchCellCb        *cell;
SuId               suId;
TfuTtiIndInfo      *ttiInd;
#endif
{
   S16             ret;
   RgSchErrInfo    err;
   RgSchDlSf       *dlSf;
   /* rg006.201: [ccpu000111706, ccpu00112394]: Separated UL and DL TTI
    * processing */
   RgSchDlSf       *ulSf;
   CmLteTimingInfo crntFrm;
   CmLteTimingInfo frm;
#ifdef DEBUGP
   Inst            inst = cell->instIdx;
#endif
#ifndef RG_ULSCHED_AT_CRC
   RgSchCmnUlCell  *cellUl; 
#endif
   /* rg001.201: Added for sending TTI tick to RRM */
#ifdef RGR_RRM_TICK
   RgrTtiIndInfo        *rgrTtiInd;
#endif
#ifdef MSPD_MLOG_NEW
   volatile U32 t1;
#endif

   TRC2(rgSCHTomTtiInd);

   if (cell->cellId != ttiInd->cellId)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomTtiInd() Unable to get"
                           " the cell for cellId (%d))\n", ttiInd->cellId));
      MSPD_ERR ("Unable to get"
                           " the cell for cellId (%d))\n", ttiInd->cellId);
      err.errType = RGSCHERR_TOM_TTIIND;
      err.errCause = RGSCHERR_TOM_INV_CELL_ID;
      RETVALUE(RFAILED);
   }

#ifndef RG_ULSCHED_AT_CRC
   cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
#endif

   /* increment the crntTime in cell */
   RGSCHCPYTIMEINFO(ttiInd->timingInfo, cell->crntTime);
   /* Update the TTI type into Cell */
   cell->isDummyTti = ttiInd->isDummyTti;

   /* Send TTI Tick to RRM over RGR */
   /* rg001.201: Added for sending TTI tick to RRM */
#ifdef RGR_RRM_TICK
   /* TTI to be sent to RRM only once per system frame */
   /* rg002.201: Added support for period = 0 to disable tick to RRM */
   if ((cell->rrmTtiIndPrd != 0) && 
         ((ttiInd->timingInfo.sfn % cell->rrmTtiIndPrd) == 0) && 
         (ttiInd->timingInfo.subframe == 0))
   {
      /* Allocate a TTI indication structure and send to RRM over RGR interface */
      if (rgSCHUtlAllocSBuf (cell->instIdx,
               (Data**)&rgrTtiInd, sizeof(RgrTtiIndInfo)) != ROK)
      {
         RGSCHDBGERR (cell->instIdx,(rgSchPBuf(cell->instIdx),
            "rgSCHTomTtiInd() Unable to allocate memory for RGR TTI indication\
            for cellId (%d))\n", ttiInd->cellId));
         MSPD_ERR ("Unable to allocate memory for RGR TTI indication\
            for cellId (%d))\n", ttiInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
         err.errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(RFAILED);
      }
      rgrTtiInd->cellId = ttiInd->cellId;
      rgrTtiInd->sfn = ttiInd->timingInfo.sfn;

      if (rgSCHUtlRgrTtiInd (cell, rgrTtiInd) != ROK)
      {
         RGSCHDBGERR (cell->instIdx,(rgSchPBuf(cell->instIdx),
            "rgSCHTomTtiInd() Unable to Send RGR TTI indication to RRM\
            for cellId (%d))\n", ttiInd->cellId));
         MSPD_ERR ("Unable to Send RGR TTI indication to RRM\
            for cellId (%d))\n", ttiInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, (Data**)&rgrTtiInd,
               sizeof(RgrTtiIndInfo));
         RETVALUE(RFAILED);
      }
   }
#endif

   cmPrcTmr(&cell->tqCp, cell->tq, (PFV)rgSCHTmrProcTmr);
   rgSCHMeasGapANRepTtiHndl (cell);
   /* We need to fill in the PHICH for the UL Data, first we need to get the UL
    * SF from Scheduler, next we fill in that in the dlSf thats going to go out
    * this TTI. This must be performed prior to any other processing of the TTI
    * so that we do not wrap around and generate feedback prior to reception of
    * UL data.
    */
/*rg002.301 - Add - [ccpu00118352] - UL Scheduling at CRC Indication */
#ifdef RG_ULSCHED_AT_CRC
   /* Moved UL scheduling at CRC indication */
   /* This function performs all scheduler cell Info updations */
   rgSCHCmnUpdVars(cell);
#endif
#ifndef RG_ULSCHED_AT_CRC
   if(cellUl->hqFdbkIdx != RGSCH_INVALID_INFO)
   {   
      /* rg006.201: [ccpu000111706, ccpu00112394]: TFU_DELTA dependency removed 
       * and added RG_UL_DELTA dependency */
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for UL control */
      RGSCHADDTOCRNTTIME(cell->crntTime, crntFrm, TFU_ULCNTRL_DLDELTA);
      if ((ret = rgSCHUhmAppendPhich(cell, crntFrm))!= ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomTtiInd() Unable to append"
                  " PHICH cellId (%d))\n", ttiInd->cellId));
         MSPD_ERR ("Unable to append"
                  " PHICH cellId (%d))\n", ttiInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
      }
   }
#endif 

/*rg002.301 - Add - [ccpu00118352] - UL Scheduling at CRC Indication */
#ifndef RG_ULSCHED_AT_CRC
   if(cellUl->reTxIdx != RGSCH_INVALID_INFO)
   {
      rgSCHCmnRlsUlSf(cell);
   }
#endif

   /* Fetch the current timing info. Modify it to Last sf to be rlsd.*/
   /* UL_DELTA + 1 subframe because TTI is the first primitive from PHY.
    * This is still needed as there is a possibility of No HARQ Feedback at all
    * from PHY. We must still clear of the hqP(s) and mark for retrans.
    */
    /* rg006.201: [ccpu000111706, ccpu00112394]: TFU_DELTA dependency removed 
     * and added RG_UL_DELTA dependency */
    /* rg002.301 -MOD-[ccpu00118351] RG_TFU_HQ_IND_DELTA is replaced with
     * TFU_HQFBKIND_ULDELTA */
   RGSCHSUBFRMCRNTTIME(ttiInd->timingInfo, frm, TFU_HQFBKIND_ULDELTA); 

   /* call DHM*/
   if ((ret = rgSCHDhmRlsDlsfHqProc (cell, frm)) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomTtiInd() Unable to Release"
                    " Downlink subframe for cellId (%d))\n", ttiInd->cellId));
      MSPD_ERR ("Unable to Release"
                    " Downlink subframe for cellId (%d))\n", ttiInd->cellId);
      err.errType = RGSCHERR_TOM_TTIIND;
   }
     /*rg002.301 ccpu00119494-ADD-added handling to retransmit
      * release PDCCH in case of DTX
      */
#ifdef LTEMAC_SPS
     rgSCHCmnSpsDlReTxRelPdcch(cell);
#endif
   /* rg006.201: [ccpu000111706, ccpu00112394]: TFU_DELTA dependency removed 
    * and added RG_UL_DELTA dependency */
   RGSCHSUBFRMCRNTTIME(ttiInd->timingInfo, frm, RGSCH_RLS_SF_IDX); 

   /* rg005.201 Removing unnecessary assignment for optimization */

   /* Re-Init the subframe */
   rgSCHUtlDlRlsSubFrm(cell, frm);

#ifdef LTEMAC_HDFDD
#ifdef TFU_UPGRADE
    /* Mark HD-FDD UEs sf(n+10) as UPLINK if CQI/RI or SRS or RI expecting */
    rgSCHCmnHdFddPtUlMrk(cell);
#endif
#endif

#ifndef RG_ULSCHED_AT_CRC
   /*ccpu00117180 - ADD - Call rgSCHCmnUpdVars in TTI IND */
   rgSCHCmnUpdVars(cell);
#endif


#ifdef LTEMAC_DRX
   /*Check for DRX every TTI*/
   rgSCHDrxTtiInd(cell);
#endif /*LTEMAC_DRX*/
/*rg002.301 - Add - [ccpu00118352] - UL Scheduling at CRC Indication */
#ifndef RG_ULSCHED_AT_CRC
   /* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
    * UL and DL */
   /* rg008.201: Not doing this now due to SPS */
   rgSCHCmnUlTti(cell, &err);
#endif
   /* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
    * UL and DL */

   /*Moving the SCH processing before DL/UL SF transmission since the
    * SCH will do the scheduling TFU_DELTA SFs in advance.*/  
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
#ifdef MSPD_MLOG_NEW
   t1 = MacGetTick();
#endif
   ret = rgSCHUtlTti (cell, &err);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_DL_SCHD, RESOURCE_LARM, t1, MacGetTick());
#endif
#endif

   /* Processing steps
    * 1. Get the downlink subframe for this tti from DBM.
    * 2. Send BCH Data to phy (inside downlink subframe).
    * 3. Invoke DHM to send scheduled data of all the scheduled HQ Procs 
    * (inside downlink subframe) to PHY
    */
   /* Need to get the crnt time PLUS TFU_DELTA */
   /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for DL control */
   RGSCHADDTOCRNTTIME(ttiInd->timingInfo, crntFrm, TFU_DLCNTRL_DLDELTA);
   dlSf = rgSCHUtlSubFrmGet (cell, crntFrm);
   /* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
    * UL and DL */
   /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for UL control */
   RGSCHADDTOCRNTTIME(ttiInd->timingInfo, crntFrm, TFU_ULCNTRL_DLDELTA);
   ulSf = rgSCHUtlSubFrmGet (cell, crntFrm);

   if (rgSCHTomUtlProcTA (cell) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomTtiInd() Unable Process "
           "Timing Adjustment for cellId (%d))\n", ttiInd->cellId));
      MSPD_ERR ("Unable Process "
           "Timing Adjustment for cellId (%d))\n", ttiInd->cellId);
      err.errType = RGSCHERR_TOM_TTIIND;
   }

   /* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
    * UL and DL */
   if ((ret = rgSCHTomUtlProcDlSf (dlSf, ulSf, cell, &err)) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomTtiInd() Unable to process"
                  " downlink subframe for cellId (%d))\n", ttiInd->cellId));
      MSPD_ERR ("Unable to process"
                  " downlink subframe for cellId (%d))\n", ttiInd->cellId);
      err.errType = RGSCHERR_TOM_TTIIND;
   }
   /* Mark this frame as sent */
   dlSf->txDone = TRUE;

   if ((ret = rgSCHTomUtlProcUlSf (cell, &err)) != ROK)
   {
      /* fill in err type and call sta ind */
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomTtiInd() Unable to process"
             " Uplink subframe for cellId (%d))\n", ttiInd->cellId));
      MSPD_ERR ("Unable to process"
             " Uplink subframe for cellId (%d))\n", ttiInd->cellId);
      err.errType = RGSCHERR_TOM_TTIIND;
   }
   /* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
    * UL and DL */
   /* Not using this due to SPS */
/* ADD Changes for Downlink UE Timing Optimization */
#ifndef LTEMAC_DLUE_TMGOPTMZ
#ifdef MSPD_MLOG_NEW
   t1 = MacGetTick();
#endif
   ret = rgSCHUtlTti (cell, &err);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_DL_SCHD, RESOURCE_LARM, t1, MacGetTick());
#endif
#endif
#ifdef LTEMAC_DRX
   cell->sc.apis->rgSCHDrxStrtInActvTmrInUl(cell);
#endif

   /* Invoke RAM  */
   rgSCHRamTtiHndlr(cell);
   rgSCHGomTtiHndlr(cell, suId);

   RETVALUE(ret);
}  /* rgSCHTomTtiInd */
#endif

/** @brief This function does the TTI processin for the uplink subframe,
 * already populated by the scheduler.
 *
 * @details
 *
 *     Function: rgSCHTomUtlProcUlSf
 *
 *         Processing steps:
 *         - Loop through the Uplink allocations present in the uplink subframe.
 *         - For each allocation Fill a data reception request to be sent to PHY
 *         - Also fills the harq reception requests for the expected HQ feedbacks.
 *
 *
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo  *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlProcUlSf 
(
RgSchCellCb        *cell,
RgSchErrInfo       *err
)
#else
PRIVATE S16 rgSCHTomUtlProcUlSf (cell, err)
RgSchCellCb        *cell;
RgSchErrInfo       *err;
#endif
{
   S16             ret;
   TfuRecpReqInfo  *recpReqInfo;
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   U16             validIdx; /* Index computed from recreq's timing info*/
#endif 
   Inst            inst = cell->instIdx;

   TRC2(rgSCHTomUtlProcUlSf)

   if ((ret = rgSCHUtlAllocEventMem(inst, (Ptr *)&recpReqInfo, 
                            sizeof(TfuRecpReqInfo))) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcUlSf() Unable to "
               "Allocate TfuRecpReqInfo for cellId=%d \n", cell->cellId));
      MSPD_ERR ("Unable to "
               "Allocate TfuRecpReqInfo for cellId=%d \n", cell->cellId);
      err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
      RETVALUE(ret);
   }    
   recpReqInfo->cellId = cell->cellId;
   cmLListInit(&recpReqInfo->ueRecpReqLst);
   /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception req */
#ifdef LTE_TDD
   RGSCHADDTOCRNTTIME(cell->crntTime, recpReqInfo->timingInfo, TFU_DELTA);
#else
   RGSCHADDTOCRNTTIME(cell->crntTime, recpReqInfo->timingInfo,
                      TFU_RECPREQ_DLDELTA);
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   RG_SCH_GET_IDX_PCQISRSSR(recpReqInfo->timingInfo, validIdx);
   /*ccpu00116923 - ADD - SRS present support - Start*/
#ifdef LTE_TDD
   recpReqInfo->srsPres = rgSchTddCellSpSrsSubfrmTbl[cell->srsCfg.srsSubFrameCfg][recpReqInfo->timingInfo.subframe];
#else
   recpReqInfo->srsPres = rgSchFddCellSpSrsSubfrmTbl[cell->srsCfg.srsSubFrameCfg][recpReqInfo->timingInfo.subframe];
#endif
   /*ccpu00116923 - ADD - SRS present support - End*/
#endif 
   /* rg009.201. Added changes of TFU_UPGRADE */
   /* Filling data Reception requests */
   ret = rgSCHTomUtlFillDatRecpReq(recpReqInfo, cell, 
#ifdef TFU_UPGRADE
         validIdx,
#endif 
         err);
   if (ret != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcUlSf() Unable to fill"
            " Data recption requests for cellId (%d))\n", cell->cellId));
      MSPD_ERR ("Unable to fill"
            " Data recption requests for cellId (%d))\n", cell->cellId);
      cmFreeMem((Ptr)recpReqInfo);
      RETVALUE(ret);
   } 
/* rg009.201. Added changes of TFU_UPGRADE */
   /* Filling HARQ Reception requests */
#ifdef TFU_UPGRADE
   ret = rgSCHTomUtlFillHqFdbkRecpReq (recpReqInfo, cell, validIdx,err);
#else
 ret = rgSCHTomUtlFillHqFdbkRecpReq (recpReqInfo, cell, err);
#endif

   if (ret != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcUlSf() Unable to fill"
       " Harq Feedback reception requests for cellId (%d))\n", cell->cellId));
      MSPD_ERR ("Unable to fill"
       " Harq Feedback reception requests for cellId (%d))\n", cell->cellId);
      cmFreeMem((Ptr)recpReqInfo);
      RETVALUE(ret);
   }
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   /* Filling SR Reception requests */
   ret = rgSCHTomUtlFillSrRecpReq (recpReqInfo, cell, validIdx, err);
   if (ret != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcUlSf() Unable to fill"
          " SR reception requests for cellId (%d))\n", cell->cellId));
      MSPD_ERR ("Unable to fill"
          " SR reception requests for cellId (%d))\n", cell->cellId);
      cmFreeMem((Ptr)recpReqInfo);
      RETVALUE(ret);
   }

   /* Filling RI Reception requests */
   ret = rgSCHTomUtlFillRiRecpReq (recpReqInfo, cell, validIdx, err);
   if (ret != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcUlSf() Unable to fill"
        " RI reception requests for cellId (%d))\n", cell->cellId));
      MSPD_ERR ("Unable to fill"
        " RI reception requests for cellId (%d))\n", cell->cellId);
      cmFreeMem((Ptr)recpReqInfo);
      RETVALUE(ret);
   }

   /* Filling PCQI Reception requests */
   ret = rgSCHTomUtlFillPcqiRecpReq (recpReqInfo, cell, validIdx, err);
   if (ret != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcUlSf() Unable to fill"
         " PCQI reception requests for cellId (%d))\n", cell->cellId));
      MSPD_ERR ("Unable to fill"
         " PCQI reception requests for cellId (%d))\n", cell->cellId);
      cmFreeMem((Ptr)recpReqInfo);
      RETVALUE(ret);
   }

   /* Filling SRS Reception requests */
   ret = rgSCHTomUtlFillSrsRecpReq (recpReqInfo, cell, validIdx, err);
   if (ret != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcUlSf() Unable to fill"
            " Harq Feedback recption requests for cellId (%d))\n", cell->cellId));
      MSPD_ERR ("Unable to fill"
            " Harq Feedback recption requests for cellId (%d))\n", cell->cellId);
      cmFreeMem((Ptr)recpReqInfo);
      RETVALUE(ret);
   }
#endif

   /* rg001.201: Now always sending down a recp req */
   /* sending the RecpReq to Phy */
   if (rgSCHUtlTfuRecpReq(inst, cell->tfuSap->sapCfg.suId, recpReqInfo) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcUlSf() Unable to send "
                               "Cntrl info for cellId (%d)\n", cell->cellId));
      MSPD_ERR ("Unable to send "
                               "Cntrl info for cellId (%d)\n", cell->cellId);
   }
   RETVALUE(ROK);
} /* end of rgSCHTomUtlProcUlSf */ 

/* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
 * DL and UL */
#ifdef LTE_TDD 
/** @brief This function does all the processing related to a single downlink
 * subframe.
 *
 * @details 
 *
 *     Function: rgSCHTomUtlProcDlSf
 *
 *         Processing steps:
 *         - collate control data for all UEs and send to PHY 
 *         - collate data buffers for all UEs and send to PHY 
 *
 * @param  [in] RgSchDlSf     *dlSf
 * @param  [in] RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 * @return S16
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlProcDlSf
(
RgSchDlSf            *dlSf,
RgSchCellCb          *cell,
RgSchErrInfo         *err
)
#else
PRIVATE S16 rgSCHTomUtlProcDlSf (dlSf, cell, err)
RgSchDlSf            *dlSf;
RgSchCellCb          *cell;
RgSchErrInfo         *err;
#endif
{
   TfuCntrlReqInfo   *cntrlInfo;
   Inst              inst = cell->instIdx;
   S16               ret;

   TRC2(rgSCHTomUtlProcDlSf);

   /* Fill control data from scheduler to PHY */   
   if ((ret = rgSCHUtlAllocEventMem(inst, (Ptr *)&cntrlInfo, 
                            sizeof(TfuCntrlReqInfo))) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
         "Allocate TfuCntrlReqInfo for cellId=%d \n", cell->cellId));
      MSPD_ERR ("Unable to "
         "Allocate TfuCntrlReqInfo for cellId=%d \n", cell->cellId);
      RETVALUE(ret);
   }
   else
   {
      cmLListInit(&cntrlInfo->phichLst);
      cmLListInit(&cntrlInfo->dlPdcchLst);
      cmLListInit(&cntrlInfo->ulPdcchLst);
      RGSCHADDTOCRNTTIME(cell->crntTime, cntrlInfo->ulTiming, TFU_DELTA);
      RGSCHADDTOCRNTTIME(cell->crntTime, cntrlInfo->dlTiming, TFU_DELTA);
      cntrlInfo->cellId = cell->cellId;
      /* Fill PCFICH info */
      cntrlInfo->cfi = (U8)dlSf->pcfichInfo;
      /* Fill PHICH info */
      if ((ret = rgSCHTomUtlFillPhich (cntrlInfo, dlSf, err)) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
           "send PHICH info for cellId (%d)\n", cell->cellId));
         MSPD_ERR ("Unable to "
           "send PHICH info for cellId (%d)\n", cell->cellId);
         cmFreeMem((Ptr)cntrlInfo);
         RETVALUE(ret);
      }
      /* Fill PDCCH info */
      if ((ret = rgSCHTomUtlFillPdcch (cntrlInfo, dlSf, err)) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
           "send PDCCH info for cellId (%d)\n", cell->cellId));
         MSPD_ERR ("Unable to "
           "send PDCCH info for cellId (%d)\n", cell->cellId);
         cmFreeMem((Ptr)cntrlInfo);
         RETVALUE(ret);
      }

      /* rg001.201: Now always sending down a cntrl req */
      /* sending the cntrl data to Phy */
      if (rgSCHUtlTfuCntrlReq(inst, cell->tfuSap->sapCfg.suId, cntrlInfo) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to"
            " send Cntrl info for cellId (%d)\n", cell->cellId));
         MSPD_ERR ("Unable to"
            " send Cntrl info for cellId (%d)\n", cell->cellId);
      }
   }

   RETVALUE(ROK);
} /* end of */ 
#else
/** @brief This function does all the processing related to a single downlink
 * subframe.
 *
 * @details 
 *
 *     Function: rgSCHTomUtlProcDlSf
 *
 *         Processing steps:
 *         - collate control data for all UEs and send to PHY 
 *         - collate data buffers for all UEs and send to PHY 
 *
 * @param  [in] RgSchDlSf     *dlSf
 * @param  [in] RgSchDlSf     *ulSf
 * @param  [in] RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 * @return S16
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlProcDlSf
(
RgSchDlSf            *dlSf,
RgSchDlSf            *ulSf,
RgSchCellCb          *cell,
RgSchErrInfo         *err
)
#else
PRIVATE S16 rgSCHTomUtlProcDlSf (dlSf, ulSf, cell, err)
RgSchDlSf            *dlSf;
RgSchDlSf            *ulSf;
RgSchCellCb          *cell;
RgSchErrInfo         *err;
#endif
{
   TfuCntrlReqInfo   *cntrlInfo;
   Inst              inst = cell->instIdx;
   S16               ret;

   TRC2(rgSCHTomUtlProcDlSf);

   /* Fill control data from scheduler to PHY */   
   if ((ret = rgSCHUtlAllocEventMem(inst, (Ptr *)&cntrlInfo, 
                            sizeof(TfuCntrlReqInfo))) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
         "Allocate TfuCntrlReqInfo for cellId=%d \n", cell->cellId));
      MSPD_ERR ("Unable to "
         "Allocate TfuCntrlReqInfo for cellId=%d \n", cell->cellId);
      RETVALUE(ret);
   }
   else
   {
      cmLListInit(&cntrlInfo->phichLst);
      cmLListInit(&cntrlInfo->dlPdcchLst);
      cmLListInit(&cntrlInfo->ulPdcchLst);
      /* rg006.201: [ccpu000111706, ccpu00112394]: TFU_DELTA dependency
       * removed */ 
#ifndef RG_ULSCHED_AT_CRC
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for UL control */
      RGSCHADDTOCRNTTIME(cell->crntTime, cntrlInfo->ulTiming,
                         TFU_ULCNTRL_DLDELTA);
#endif
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for DL control */
      RGSCHADDTOCRNTTIME(cell->crntTime, cntrlInfo->dlTiming, 
                         TFU_DLCNTRL_DLDELTA);
      cntrlInfo->cellId = cell->cellId;
      /* Fill PCFICH info */
      cntrlInfo->cfi = (U8)dlSf->pcfichInfo;
#ifndef RG_ULSCHED_AT_CRC
      /* Fill PHICH info */
      if ((ret = rgSCHTomUtlFillPhich (cntrlInfo, ulSf, err)) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
           "send PHICH info for cellId (%d)\n", cell->cellId));
         MSPD_ERR ("Unable to "
           "send PHICH info for cellId (%d)\n", cell->cellId);
         cmFreeMem((Ptr)cntrlInfo);
         RETVALUE(ret);
      }
#endif
      /* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
       * DL and UL */
      /* Fill PDCCH info */
      if ((ret = rgSCHTomUtlFillDlPdcch (cntrlInfo, dlSf, err)) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
           "send PDCCH info for cellId (%d)\n", cell->cellId));
         MSPD_ERR ("Unable to "
           "send PDCCH info for cellId (%d)\n", cell->cellId);
         cmFreeMem((Ptr)cntrlInfo);
         RETVALUE(ret);
      }
#ifndef RG_ULSCHED_AT_CRC
      if ((ret = rgSCHTomUtlFillUlPdcch (cntrlInfo, ulSf, err)) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
           "send PDCCH info for cellId (%d)\n", cell->cellId));
         MSPD_ERR ("Unable to "
           "send PDCCH info for cellId (%d)\n", cell->cellId);
         cmFreeMem((Ptr)cntrlInfo);
         RETVALUE(ret);
      }
#endif

      /* rg001.201: Now always sending down a cntrl req */
      /* sending the cntrl data to Phy */
      if (rgSCHUtlTfuCntrlReq(inst, cell->tfuSap->sapCfg.suId, cntrlInfo) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to"
            " send Cntrl info for cellId (%d)\n", cell->cellId));
         MSPD_ERR ("Unable to"
            " send Cntrl info for cellId (%d)\n", cell->cellId);
      }
   }

   RETVALUE(ROK);
} /* end of */ 
#endif /* LTE_TDD */


/** @brief This function handles sending of the PHICH information for the
 * downlink subframe to be sent in the next TTI.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through the PHICH information present in the downlink
 *         subframe and fill the information in cntrlInfo.
 *
 * @param  [out] TfuCntrlReqInfo *cntrlInfo 
 * @param  [in]  RgSchDlSf   *dlSf
 * @param  [out] RgSchErrInfo *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillPhich
(
TfuCntrlReqInfo    *cntrlInfo,
RgSchDlSf          *dlSf,
RgSchErrInfo       *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillPhich(cntrlInfo, dlSf, err)
TfuCntrlReqInfo    *cntrlInfo;
RgSchDlSf          *dlSf;
RgSchErrInfo       *err;
#endif
{
   S16             ret;
   CmLList         *node;
   RgSchPhich      *phich;
   TfuPhichInfo    *harqAck;

   TRC2(rgSCHTomUtlFillPhich)
   ret = ROK;
   /* Traversing the list of Phichs */
   node =  dlSf->phichInfo.phichs.first;
   while (node)
   {
      phich = (RgSchPhich*)node->node;
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&harqAck, sizeof(TfuPhichInfo), 
                                &(cntrlInfo->memCp))) != ROK)
      {
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      /* fill in the tfu structure from the information present in the
       * phich node */
      harqAck->rbStart     = phich->rbStart;
      harqAck->nDmrs       = phich->nDmrs;
      harqAck->isAck       = phich->hqFeedBack;
#ifdef LTE_TDD
      /*rg009.201:[ccpu00116309]: Changes for passing iPhich at TFU interface*/
      harqAck->iPhich      = phich->iPhich;
#endif
      cmLListAdd2Tail(&cntrlInfo->phichLst, &(harqAck->lnk));
      harqAck->lnk.node = (PTR)harqAck;
      node = node->next;
   } /* end of while */
   RETVALUE(ret);
} /* end of */ 

/* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
 * DL and UL */
#ifdef LTE_TDD
/** @brief This function will send all the PDCCH's for the given downlink
 * subframe.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through all the scheduled HARQ processes and fill
 *           the PDCCH information in cntrlInfo.
 *
 * @param  [out] TfuCntrlReqInfo *cntrlInfo 
 * @param  [in]  RgSchDlSf   *dlSf
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillPdcch
(
TfuCntrlReqInfo *cntrlInfo,
RgSchDlSf       *dlSf,
RgSchErrInfo    *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillPdcch(cntrlInfo, dlSf, err)
TfuCntrlReqInfo *cntrlInfo;
RgSchDlSf       *dlSf;
RgSchErrInfo    *err;
#endif
{
   S16          ret;
   CmLList      *node;
   RgSchPdcch   *pdcch;
   TfuPdcchInfo *tfuPdcch;

   TRC2(rgSCHTomUtlFillPdcch)
   ret = ROK;
   /* Traversing the scheduled Harq processes */
   node =  dlSf->pdcchInfo.pdcchs.first;
   while (node)
   {
      pdcch = (RgSchPdcch*)node->node;
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&tfuPdcch, sizeof(TfuPdcchInfo),
                                &(cntrlInfo->memCp))) != ROK)
      {
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      tfuPdcch->rnti       = pdcch->rnti;
      tfuPdcch->nCce       = pdcch->nCce;
      tfuPdcch->aggrLvl    = pdcch->aggrLvl;
      tfuPdcch->dci        = pdcch->dci;
      /* rg001.201 removed extra comments */
      /* To be enhanced later for 2.1 */
      if(pdcch->dci.dciFormat == TFU_DCI_FORMAT_0)
      {
         cmLListAdd2Tail(&cntrlInfo->ulPdcchLst, &(tfuPdcch->lnk));
      }
      else
      {
         cmLListAdd2Tail(&cntrlInfo->dlPdcchLst, &(tfuPdcch->lnk));
      }
      tfuPdcch->lnk.node = (PTR)tfuPdcch;
      node = node->next;
   } /* end of while */
   RETVALUE(ret);
} /* end of rgSCHTomUtlFillPdcch*/ 
#else
/** @brief This function will send all the PDCCH's for the given downlink
 * subframe.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through all the scheduled HARQ processes and fill
 *           the PDCCH information in cntrlInfo.
 *
 * @param  [out] TfuCntrlReqInfo *cntrlInfo 
 * @param  [in]  RgSchDlSf   *dlSf
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillDlPdcch
(
TfuCntrlReqInfo *cntrlInfo,
RgSchDlSf       *dlSf,
RgSchErrInfo    *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillDlPdcch(cntrlInfo, dlSf, err)
TfuCntrlReqInfo *cntrlInfo;
RgSchDlSf       *dlSf;
RgSchErrInfo    *err;
#endif
{
   S16          ret;
   CmLList      *node;
   RgSchPdcch   *pdcch;
   TfuPdcchInfo *tfuPdcch;

   TRC2(rgSCHTomUtlFillDlPdcch)
   ret = ROK;
   /* Traversing the scheduled Harq processes */
   node =  dlSf->pdcchInfo.pdcchs.first;
   while (node)
   {
      pdcch = (RgSchPdcch*)node->node;
      /*ccpu00117179 - ADD - Build only non DCI format-0 messages */
      if(pdcch->dci.dciFormat == TFU_DCI_FORMAT_0)
      {
         node = node->next;
         continue;
      }
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&tfuPdcch, sizeof(TfuPdcchInfo),
                                &(cntrlInfo->memCp))) != ROK)
      {
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      tfuPdcch->rnti       = pdcch->rnti;
      tfuPdcch->nCce       = pdcch->nCce;
      tfuPdcch->aggrLvl    = pdcch->aggrLvl;
      tfuPdcch->dci        = pdcch->dci;
      /* rg001.201 removed extra comments */
      /* To be enhanced later for 2.1 */
      /*ccpu00117179 - REM - Check for Non DCI-0 removed */
#if 1
	  if (pdcch->rnti != 65535)
      {
	     MSPD_DBG("SCH: UEID [%d] format %d Time Air[%d %d]\n",
	  	          pdcch->rnti,pdcch->dci.dciFormat,cntrlInfo->dlTiming.sfn, 
                  cntrlInfo->dlTiming.subframe);
  	     rgSCHTomPrntDciInfo(tfuPdcch);
	  }
#endif

      cmLListAdd2Tail(&cntrlInfo->dlPdcchLst, &(tfuPdcch->lnk));
      tfuPdcch->lnk.node = (PTR)tfuPdcch;
      node = node->next;
   } /* end of while */
   RETVALUE(ret);
} /* end of rgSCHTomUtlFillDlPdcch*/ 

/* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for 
 * DL and UL */
/** @brief This function will send all the UL PDCCH's for the given
 * subframe.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through all the scheduled HARQ processes and fill
 *           the PDCCH information in cntrlInfo.
 *
 * @param  [out] TfuCntrlReqInfo *cntrlInfo 
 * @param  [in]  RgSchDlSf   *dlSf
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillUlPdcch
(
TfuCntrlReqInfo *cntrlInfo,
RgSchDlSf       *dlSf,
RgSchErrInfo    *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillUlPdcch(cntrlInfo, dlSf, err)
TfuCntrlReqInfo *cntrlInfo;
RgSchDlSf       *dlSf;
RgSchErrInfo    *err;
#endif
{
   S16          ret;
   CmLList      *node;
   RgSchPdcch   *pdcch;
   TfuPdcchInfo *tfuPdcch;

   TRC2(rgSCHTomUtlFillUlPdcch)
   ret = ROK;
   /* Traversing the scheduled Harq processes */
   node =  dlSf->pdcchInfo.pdcchs.first;
   while (node)
   {
      pdcch = (RgSchPdcch*)node->node;
      node = node->next;
      /*ccpu00116712- Function should pick only UL allocation related control
       * info- start */
      if(pdcch->dci.dciFormat != TFU_DCI_FORMAT_0) 
      {    
         continue;
      }    
      /*ccpu00116712- Function should pick only UL allocation related control
       * info- end */
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&tfuPdcch, sizeof(TfuPdcchInfo),
                  &(cntrlInfo->memCp))) != ROK)
      {
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      tfuPdcch->rnti       = pdcch->rnti;
      tfuPdcch->nCce       = pdcch->nCce;
      tfuPdcch->aggrLvl    = pdcch->aggrLvl;
      tfuPdcch->dci        = pdcch->dci;
      rgSCHTomPrntDciInfo(tfuPdcch);
      /* rg001.201 removed extra comments */
      /* To be enhanced later for 2.1 */
      cmLListAdd2Tail(&cntrlInfo->ulPdcchLst, &(tfuPdcch->lnk));
      tfuPdcch->lnk.node = (PTR)tfuPdcch;
   } /* end of while */
   RETVALUE(ret);
} /* end of rgSCHTomUtlFillUlPdcch*/ 
#endif

/** @brief This function does the processing for Timing adjustment.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through the ue present ueTimeLst, decrement the remaining
 *         frames left. 
 *
 *
 * @param  [in] RgSchCellCb   *cell
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlProcTA
(
RgSchCellCb    *cell
)
#else
PRIVATE S16 rgSCHTomUtlProcTA (cell)
RgSchCellCb    *cell;
#endif
{
   CmLList     *node;
   RgSchUeCb   *ue;

   TRC2(rgSCHTomUtlProcTA);

   node =  cell->taUeLst.first;
   /* rg006.201: [ccpu00113186] Corrected logic to determine TA application at ue after n+6 */
   while (node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      if (ue->dl.taCb.numRemSf == 0)
      {
         /* We need to reset state and also value of TA, then we start the timer */
         ue->dl.taCb.ta = 31;
         ue->dl.taCb.state = RGSCH_TA_IDLE;
         /* Start the timer only if TA is cfgd as FINITE value */
         if (ue->dl.taCb.cfgTaTmr)
         {
            rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_TA, ue->dl.taCb.cfgTaTmr);
         }
         /* need to delete from the link list */
         cmLListDelFrm(&(cell->taUeLst), &(ue->taLnk));
         /*rg002.301:ccpu001120284:-ADD-seeting taLnk to NULLP*/
         ue->taLnk.node = NULLP;
      }
      else
      {
         ue->dl.taCb.numRemSf--;
      }
   } /* end of taUeLst */

   /* rg006.201: [ccpu00113186] Corrected logic for TA handling */
   RETVALUE(ROK);
} /* end of rgSCHTomUtlProcTA */ 

#ifdef LTE_TDD
/** @brief This function handles filling of HARQ feedback reception request to
 * PHY.
 *
 * @details
 *
 *     Function:
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo  *recpReqInfo
 * @param  [in]  RgSchCellCb     *cellCb
 * @param  [out] RgSchErrInfo    *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq
(
 TfuRecpReqInfo   *recpReqInfo,
 RgSchCellCb      *cellCb,
 U16              validIdx,
 RgSchErrInfo     *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq (recpReqInfo, cellCb, validIdx, err)
   TfuRecpReqInfo   *recpReqInfo;
   RgSchCellCb      *cellCb;
   U16              validIdx; 
   RgSchErrInfo     *err;
#endif
#else
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq
(
 TfuRecpReqInfo   *recpReqInfo,
 RgSchCellCb      *cellCb,
 RgSchErrInfo     *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq (recpReqInfo, cellCb, err)
   TfuRecpReqInfo   *recpReqInfo;
   RgSchCellCb      *cellCb;
   RgSchErrInfo     *err;
#endif
#endif
{
   CmLteTimingInfo         futTime;
   CmLteTimingInfo         fdbkTime;
   CmLteTimingInfo         nxtfrm;
   RgSchDlSf               *dlSf;
   /*rg004.201: Memory Fix */
   RgSchDlSf               *nxtDlsf=NULLP;
   S16                     ret;
   /* rg009.201. Added changes of TFU_UPGRADE */
   U8                      loop;
   U8                      ulDlCfgIdx = cellCb->ulDlCfgIdx;
   U8                      noFdbks;
   RgSchTddDlAscSetIdxK    ascIdx;
   S16                     subfrm;
   CmMemListCp             *memCp;
   RgSchUePucchRecpInfo    *pucchInfo = NULLP;

   TRC2(rgSCHTomUtlFillHqFdbkRecpReq);
/* rg009.201. Added changes of TFU_UPGRADE */
   ret = rgSCHUtlAllocEventMem(cellCb->instIdx, (Ptr *)&memCp,
               sizeof(CmMemListCp));
   if (ret != ROK)            
   {
      RGSCHDBGERR (cellCb->instIdx, 
            (rgSchPBuf(cellCb->instIdx),"rgSCHTomUtlFillHqFdbkRecpReq() "
             "Unable to Allocate \
             TfuRecpReqInfo for cellId=%d \n", cellCb->cellId));
      MSPD_ERR ("Unable to Allocate \
             TfuRecpReqInfo for cellId=%d \n", cellCb->cellId);
      err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
      RETVALUE(ret);
   }

   RGSCHADDTOCRNTTIME(cellCb->crntTime, futTime, TFU_DELTA)

   ascIdx = rgSchTddDlAscSetIdxKTbl[ulDlCfgIdx][futTime.subframe];
   noFdbks = ascIdx.numFdbkSubfrms;

   for(loop=0; loop < noFdbks; loop++)
   {
      /* Get the subframe and sfn for which HARQ Ack/Nack
       * has to be sent */
      fdbkTime = futTime;
      subfrm = (futTime.sfn * RGSCH_NUM_SUB_FRAMES) + futTime.subframe;

      /* Subtract the subframe offset */
      subfrm = subfrm - ascIdx.subfrmNum[loop];

      if(subfrm < 0)
      {
         subfrm = (RGSCH_MAX_SFN * RGSCH_NUM_SUB_FRAMES) + subfrm;
         fdbkTime.sfn = subfrm / RGSCH_NUM_SUB_FRAMES;
         fdbkTime.subframe = subfrm % RGSCH_NUM_SUB_FRAMES;
      }
      else
      {
         fdbkTime.sfn = subfrm / RGSCH_NUM_SUB_FRAMES;
         fdbkTime.subframe = subfrm % RGSCH_NUM_SUB_FRAMES;
      }

      dlSf = rgSCHUtlSubFrmGet (cellCb, fdbkTime);
      if(cellCb->ulDlCfgIdx != 5)
      {
         rgSCHUtlGetNxtDlSfInfo(fdbkTime, cellCb, dlSf, &nxtDlsf, &nxtfrm);
      }

      /* Fills Harq Ack/Nack Repetition Feedback reception request 
       * for a subframe */
#ifdef TFU_UPGRADE
      ret = rgSCHTomUtlFillSfRepHqFdbk(recpReqInfo, cellCb, err, 
            dlSf, noFdbks, memCp, loop, nxtDlsf, validIdx);
#else
      ret = rgSCHTomUtlFillSfRepHqFdbk(recpReqInfo, cellCb, err, 
            dlSf, noFdbks, memCp, loop, nxtDlsf);
#endif

      if(ret != ROK)
      {
         RETVALUE(RFAILED);
      }

      /* Fills Harq Ack/Nack Feedback reception request 
       * for a subframe */
#ifdef TFU_UPGRADE
      ret = rgSCHTomUtlFillSfHqFdbk(recpReqInfo, cellCb, err, 
            dlSf, noFdbks, memCp, loop, nxtDlsf, validIdx);
#else
      ret = rgSCHTomUtlFillSfHqFdbk(recpReqInfo, cellCb, err, 
            dlSf, noFdbks, memCp, loop, nxtDlsf);
#endif

      if(ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   } /* end of for */

   /* For remaining UEs for which TFU request was not sent */
   for(;;)
   {
      ret = cmHashListGetNext(&cellCb->ueTfuPendLst, NULLP,
            (PTR *) &pucchInfo);
      if(ret != ROK)
      {
         break;
      }
#ifndef TFU_UPGRADE
      pucchInfo->pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
#else
      pucchInfo->pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
#endif
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, 
            &(pucchInfo->pucchRecpInfo->lnk));
      pucchInfo->pucchRecpInfo->lnk.node = (PTR)pucchInfo->pucchRecpInfo;
      cmHashListDelete(&cellCb->ueTfuPendLst, (PTR) pucchInfo);
   }

   cmFreeMem(memCp);

   RETVALUE(ROK);
} /* end of rgSCHTomUtlFillHqFdbkRecpReq */

#else
#ifdef LTEMAC_SPS
/** @brief This function will prepare the HQ Reception request for 
 * for a UE with a release-PDCCH 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Allocates PUCCH RR and sends a HQ reception request
 *           
 *
 * @param  [in] RgSchCellCb   *cell,
 * @param  [in] RgSchDlSf       *dlSf,
 * @param  [in] TfuRecpReqInfo          *recpReqInfo,
 * @param  [in] U16 validIdx,
 * @param  [in] RgSchErrInfo    *err

 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef TFU_UPGRADE
#ifdef ANSI
PRIVATE S16 rgSCHTomCnsdrRelPdcch
(
 RgSchCellCb   *cell,
 RgSchDlSf       *dlSf,
 TfuRecpReqInfo          *recpReqInfo,
 U16 validIdx,
 RgSchErrInfo    *err
 )
#else
PRIVATE S16 rgSCHTomCnsdrRelPdcch(cell, dlSf, recpReqInfo, validIdx, err)
  RgSchCellCb    *cell;
   RgSchDlSf       *dlSf;
   TfuRecpReqInfo          *recpReqInfo;
   U16   validIdx;
   RgSchErrInfo    *err;
#endif
#else
#ifdef ANSI
PRIVATE S16 rgSCHTomCnsdrRelPdcch
(
 RgSchCellCb   *cell,
 RgSchDlSf       *dlSf,
 TfuRecpReqInfo          *recpReqInfo,
 RgSchErrInfo    *err
 )
#else
PRIVATE S16 rgSCHTomCnsdrRelPdcch(cell, dlSf, recpReqInfo, err)
  RgSchCellCb    *cell;
   RgSchDlSf       *dlSf;
   TfuRecpReqInfo          *recpReqInfo;
   RgSchErrInfo    *err;
#endif
#endif
{
   S16          ret;
   TfuUeRecpReqInfo     *pucchRecpInfo;
#ifdef TFU_UPGRADE   
   RgSchUeCb *ue;
#endif
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   RgSchPdcch *relPdcch = dlSf->relPdcch;
   TRC2(rgSCHTomCnsdrRelPdcch)
   ret = ROK;
      /* A feedback reception request needs to be sent for release PDCCH */
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                  sizeof(TfuUeRecpReqInfo), 
                  &(recpReqInfo->memCp))) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomCnsdrRelPdcch()"
                  " Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                  cell->cellId));
         MSPD_ERR (" Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                  cell->cellId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
      /* Assumed UE will not be NULLP for a release PDCCH */
      pucchRecpInfo->rnti = relPdcch->rnti;
#ifndef TFU_UPGRADE
#ifndef TFU_TDD         
      pucchRecpInfo->t.pucchRecpReq.hqType = TFU_HQ_RECP_REQ_NORMAL;
      pucchRecpInfo->t.pucchRecpReq.t.nCce = relPdcch->nCce;
#endif
      /* Handling of other types */
      pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
#else   /* TFU_UPGRADE */
      pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
      pucchRecpInfo->t.pucchRecpReq.hqInfo.pucchIdx = (relPdcch->nCce +
            cell->pucchCfg.n1PucchAn);
      pucchRecpInfo->t.pucchRecpReq.hqInfo.hqSz = 1; /* NON-MIMO */ 
      ue = rgSCHDbmGetUeCb (cell, dlSf->relPdcch->rnti);
      rgSCHTomUtlFillCqiSrSrsWithHq(cell,recpReqInfo, ue, 
                  pucchRecpInfo, validIdx);  
#endif /* TFU_UPGRADE */
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
      pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;   
   RETVALUE(ret);
} /* end of rgSCHTomCnsdrRelPdcch*/ 
#endif
/** @brief This function handles filling of HARQ feedback recption request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq
(
 TfuRecpReqInfo          *recpReqInfo,
 RgSchCellCb             *cell,
 U16                  validIdx,
 RgSchErrInfo            *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq (recpReqInfo, cell, validIdx, err)
   TfuRecpReqInfo          *recpReqInfo;
   RgSchCellCb             *cell;
   U16                  validIdx; 
   RgSchErrInfo            *err;
#endif
#else
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
RgSchErrInfo            *err
)
#else
PRIVATE S16 rgSCHTomUtlFillHqFdbkRecpReq (recpReqInfo, cell, err)
TfuRecpReqInfo          *recpReqInfo;
RgSchCellCb             *cell;
RgSchErrInfo            *err;
#endif
#endif 
{
   CmLList              *node;
   RgSchDlHqProcCb      *hqCb;
   CmLteTimingInfo      futTime;
   RgSchDlSf            *dlSf;
   RgSchDlSf            *nxtDlsf;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   RgSchUeCb            *ue;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchDlHqTbCb        *tbCb;
   RgSchDlHqProcCb      *prvHqCb=NULLP;

   TRC2(rgSCHTomUtlFillHqFdbkRecpReq);

   /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for reception req */
   RGSCHSUBFRMCRNTTIME(cell->crntTime, futTime, (RG_SCH_CMN_HARQ_INTERVAL - TFU_RECPREQ_DLDELTA));
   dlSf = rgSCHUtlSubFrmGet (cell, futTime);
   /* Get the next dlsf as well */
   RGSCHADDTOCRNTTIME(futTime, futTime, 1)
   nxtDlsf = rgSCHUtlSubFrmGet (cell, futTime);

   /* FOR ACK NACK REP */
   node = dlSf->ackNakRepQ.first;
   while (node)
   {
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      tbCb = (RgSchDlHqTbCb *)(node->node);
      hqCb = tbCb->hqP;
      ue = hqCb->hqE->ue;
      /* FOR ACK NAK REP */
      if ((tbCb->fbkRecpRepCntr) && 
            (--tbCb->fbkRecpRepCntr))
      {
         /* Add to next subfarme */
         /* Add this hqCb to the next dlSf's ackNakRepQ */
         cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
               &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
         tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
         tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
      }
      /* In case the HARQ feedback comes with PUSCH */
      /* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
      /*rg002.301 -Add - [ccpu00120233] - TB Count; DEL - rcptDone flag*/
      if (hqCb->tbCnt) 
      {
         hqCb->tbCnt--;
         /* Go to the next node */
         node = node->next;
         continue;
      }
#endif
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      if ((hqCb->hqE->ue != NULLP) &&
            (hqCb->hqE->ue->measGapCb.isMeasuring != TRUE)
            && (hqCb != prvHqCb)
         )
      {
         /* We need to add the recp request to be sent on the pucchANRep
          * value.
          */
        /* rg009.201. Added changes of TFU_UPGRADE */
          ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                     sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
         if (ret != ROK)
         {
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillHqFdbkRecpReq()"
                     " Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                     cell->cellId));
            MSPD_ERR (" Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                     cell->cellId);
            err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
            RETVALUE(ret);
         }
         pucchRecpInfo->rnti    =  ue->ueId;
         /*ccpu00116711- Missing TFU_RECP_REQ_PUCCH field assignment*/
         pucchRecpInfo->type    =  TFU_RECP_REQ_PUCCH;
#ifndef TFU_UPGRADE
         pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
#else
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
         if ((hqCb->tbInfo[0].sfLnk.node != NULLP) && 
               (hqCb->tbInfo[1].sfLnk.node != NULLP))
         {
            pucchRecpInfo->t.pucchRecpReq.hqInfo.hqSz = 2; /* MIMO */
         }
         else
         {
            pucchRecpInfo->t.pucchRecpReq.hqInfo.hqSz = 1; /* NON MIMO */
         }
         pucchRecpInfo->t.pucchRecpReq.hqInfo.pucchIdx = 
            ue->ackNakRepCb.pucchRes; 
#endif /* TFU_UPGRADE */

#ifndef TFU_UPGRADE
         pucchRecpInfo->t.pucchRecpReq.hqType = TFU_HQ_RECP_REQ_N1PUCCH;
         pucchRecpInfo->t.pucchRecpReq.t.n1Pucch = ue->ackNakRepCb.pucchRes;
#endif /* TFU_UPGRADE */
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
         pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
      }
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* In a given dlSf, if there is 2 TBs context 
       * stored for a given harq, then they are added
       * adjacent to each other in the subframe. To avoid
       * adding duplicate recpnInfo for each TB, store this
       * hqCb in prvHqCb. If nextHqCb is same as prvHqCb then
       * do not add reception req info.*/
      prvHqCb = hqCb;
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
      if(hqCb->hqE->ue)
      {
         rgSCHTomUtlMoveNxtOccasion(cell, hqCb->hqE->ue, validIdx);
      }
#endif 
      /* Go to the next node */
      node = node->next;
   }

 /*rg001.301-ADD-[ccpu00118844]reset prvHqCb */
   prvHqCb = NULLP;

   if (dlSf->tbs.count != 0)
   {
      node =  dlSf->tbs.first;
      while (node)
      {
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependency on MIMO compile-time flag */
         tbCb = (RgSchDlHqTbCb*)node->node;
         hqCb = tbCb->hqP;

         /* FOR ACK NAK REP */
         if ((hqCb->hqE->ue != NULLP) &&
               (hqCb->hqE->ue->measGapCb.isMeasuring == TRUE))
         {
            if ((tbCb->fbkRecpRepCntr) && 
                  (--tbCb->fbkRecpRepCntr))
            {
               /* Add to next subfarme */
               /* Add this hqCb to the next dlSf's ackNakRepQ */
               cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
                     &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
               tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
               tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
            }
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
            rgSCHTomUtlMoveNxtOccasion(cell, hqCb->hqE->ue, validIdx);
#endif 
            /* No need to send hqRecpReq */
            node = node->next;
            continue;
         }
#ifdef TFU_UPGRADE
      /*rg002.301 -Add - [ccpu00120233] - TB Count; DEL - rcptDone flag*/
      if (hqCb->tbCnt) 
      {
         hqCb->tbCnt--;
         /* Go to the next node */
         node = node->next;
         continue;
      }
#endif

         if (hqCb != prvHqCb) {
            ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                        sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
            if (ret != ROK)
            {
               RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillHqFdbkRecpReq()"
                        " Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                        cell->cellId));
               MSPD_ERR (" Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                        cell->cellId);
               err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
               RETVALUE(ret);
            }
            pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
            /* rg007.201 - Changes for MIMO feature addition */
            /* rg008.201 - Removed dependency on MIMO compile-time flag */
            /*rg009.201 - Changes as a result of CR timer implementation*/
            if ((hqCb->hqE->ue != NULLP) /*&& 
                  ((tbCb->lchSchdData[0].lcId != 0) || (tbCb->taSnt ==
                  TRUE))*/
               )
            {
                /*rg002.301 - MOD - [ccpu00119223] Assigned pucchRecpInfo->rnti with the ueId 
                 * instead of rntiLnk->rnti */
               pucchRecpInfo->rnti = hqCb->hqE->ue->ueId;
            }
            else
            {
               if (hqCb->hqE->raCb)
               {
                  pucchRecpInfo->rnti    =  hqCb->hqE->raCb->tmpCrnti;
               }
            }
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
#ifndef TFU_TDD         
#ifdef LTEMAC_SPS
         if (!hqCb->spsN1PucchRes.pres)
#endif
    {
            pucchRecpInfo->t.pucchRecpReq.hqType = TFU_HQ_RECP_REQ_NORMAL;
            /* rg007.201 - Changes for MIMO feature addition */
            /* rg008.201 - Removed dependency on MIMO compile-time flag */
            pucchRecpInfo->t.pucchRecpReq.t.nCce = tbCb->pdcch->nCce;
    }
#ifdef LTEMAC_SPS
    else
    {
       pucchRecpInfo->t.pucchRecpReq.hqType = TFU_HQ_RECP_REQ_N1PUCCH;
       pucchRecpInfo->t.pucchRecpReq.t.n1Pucch = hqCb->spsN1PucchRes.val;
    }
#endif
#endif
            /* Handling of other types */
            pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
#else   /* TFU_UPGRADE */
            pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
#ifdef LTEMAC_SPS
            if (!hqCb->spsN1PucchRes.pres)
#endif
            {
               pucchRecpInfo->t.pucchRecpReq.hqInfo.pucchIdx = (tbCb->pdcch->nCce +
                     cell->pucchCfg.n1PucchAn);
            }
#ifdef LTEMAC_SPS
            /* PucchRecpReq needs to be filled up for n1Pucch resource for SPS
             * ocassions */
            if (hqCb->spsN1PucchRes.pres)
            {
               pucchRecpInfo->t.pucchRecpReq.hqInfo.pucchIdx = hqCb->spsN1PucchRes.val;
            }
#endif /* LTEMAC_SPS */

            if ((hqCb->tbInfo[0].sfLnk.node != NULLP) && 
                  (hqCb->tbInfo[1].sfLnk.node != NULLP))
            {
               pucchRecpInfo->t.pucchRecpReq.hqInfo.hqSz = 2; /* MIMO */
            }
            else
            {
               pucchRecpInfo->t.pucchRecpReq.hqInfo.hqSz = 1; /* NON-MIMO */ 
            }

#endif

#ifdef TFU_UPGRADE
            rgSCHTomUtlFillCqiSrSrsWithHq(cell,recpReqInfo, hqCb->hqE->ue, 
                  pucchRecpInfo, validIdx);    
#endif 

            cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
            pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
            /* rg007.201 - Changes for MIMO feature addition */
            /* rg008.201 - Removed dependency on MIMO compile-time flag */
         }
        /*rg001.301-ADD-[ccpu00118844]set prvHqCb */
         prvHqCb = hqCb; 
         
         
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependency on MIMO compile-time flag */
         if ((tbCb->fbkRecpRepCntr) && 
               (--tbCb->fbkRecpRepCntr))
         {
            /* Add to next subfarme */
            /* Add this hqCb to the next dlSf's ackNakRepQ */
            cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
                  &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
            tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
            tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
         }
         node = node->next;
      } /* end of while */
   } /* If hq is expected */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   if (dlSf->relPdcch)
   {
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
      rgSCHTomCnsdrRelPdcch(cell, dlSf, recpReqInfo, validIdx, err);
#else
      rgSCHTomCnsdrRelPdcch(cell, dlSf, recpReqInfo, err);
#endif
   }
#endif /* LTEMAC_SPS */

   /* rg001.201 removed extra comments */
   /* Check with TDD Code */
   /* FOR ACK NACK REP */
   RETVALUE(ROK);
} /* end of rgSCHTomUtlFillHqFdbkRecpReq */ 
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE 
/** @brief This function handles filling of SR reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillSrRecpReq
(
 TfuRecpReqInfo          *recpReqInfo,
 RgSchCellCb             *cell,
 U16                  validIdx,
 RgSchErrInfo            *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillSrRecpReq (recpReqInfo, cell, validIdx, err)
   TfuRecpReqInfo          *recpReqInfo;
   RgSchCellCb             *cell;
   U16                  validIdx; 
   RgSchErrInfo            *err;
#endif
{
   CmLList              *node;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   RgSchUeCb            *ue;

   TRC2(rgSCHTomUtlFillSrRecpReq);

   node = cell->pCqiSrsSrLst[validIdx].srLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
	  /* MS_FIX: ccpu00124011: Fix for missing reception request for UE with same offset */
      node = node->next;
      if(ue == NULLP)
      {
         continue;
      }
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                  sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillSrRecpReq()"
                  " Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                  cell->cellId));
         MSPD_ERR (" Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                  cell->cellId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      /*Fill SR params*/
      pucchRecpInfo->rnti =  ue->ueId; 
      pucchRecpInfo->t.pucchRecpReq.srInfo.n1PucchIdx = 
      ue->srCb.srCfg.srSetup.srResIdx;
      pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR;
      rgSCHTomUtlMoveSrNxtOccasion(cell, ue);
      rgSCHTomUtlFillCqiSrsWithSr(cell, ue, recpReqInfo,  
                  pucchRecpInfo, validIdx); 
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
      pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
   }
   RETVALUE(ROK);
}/* end of rgSCHTomUtlFillSrRecpReq */


/** @brief This function tells will the UE has a periodic CQI/PMI/RI 
 *  reporting
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 *  @param  [in]   RgSchUeCb        *ue 
 *  @param  [out]  Bool             *willueRprtCqiRi  
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

#ifdef ANSI
PRIVATE S16 rgSCHTomUtlWillUeRprtCqiRi
(
 RgSchUeCb        *ue,
 Bool             *willueRprtCqiRi
 )
#else
PRIVATE S16 rgSCHTomUtlWillUeRprtCqiRi ( ue, willueRprtCqiRi)
 RgSchUeCb        *ue;
 Bool             *willueRprtCqiRi;
#endif
{
   TRC2(rgSCHTomUtlWillUeRprtCqiRi);
  
   /* Intialising Reporting probability as TRUE */ 
   *willueRprtCqiRi = TRUE; 

   /* Checking the cases in which UE will not report CQIPMI/RI */
#ifdef LTEMAC_DRX
   if(ue->isDrxEnabled && ue->drxCb)
   {
#ifdef LTEMAC_R9
      if(ue->drxCb->cqiMask.pres && ue->drxCb->cqiMask.val == RGR_DRX_SETUP)
      {/*cqiMask is setup by upper layers */                                
         if((ue->drxCb->drxUlInactvMask & RG_SCH_DRX_ONDUR_BITMASK) ==
               RG_SCH_DRX_ONDUR_BITMASK)
         {/*onDuration NOT running, do not expect cqi/pmi/ri*/
            *willueRprtCqiRi = FALSE;
         }
         RETVALUE(ROK);
      }
#endif /*end of LTEMAC_R9*/
      if ( !RG_SCH_DRX_UL_IS_UE_ACTIVE(ue->drxCb) ) 
      {/*UE is not active, do not expect cqi/pmi/ri*/                                  
         *willueRprtCqiRi = FALSE; 
      }
   }/*ue->isDrxEnabled*/
#endif /*End of LTEMAC_DRX*/
   
   RETVALUE(ROK);
} /*End of rgSCHTomUtlWillUeRprtCqiRi*/

/** @brief This function handles filling of RI reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  U16           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillRiRecpReq
(
 TfuRecpReqInfo       *recpReqInfo,
 RgSchCellCb          *cell,
 U16                  validIdx,
 RgSchErrInfo         *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillRiRecpReq (recpReqInfo, cell, validIdx, err)
   TfuRecpReqInfo       *recpReqInfo;
   RgSchCellCb          *cell;
   U16                  validIdx; 
   RgSchErrInfo         *err;
#endif
{
   CmLList              *node;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   RgSchUeCb            *ue;
   Bool                  willUeRprtCqi; /* Flag set due to CQI Mask
                                       and UE inactive state (DRX) */

   TRC2(rgSCHTomUtlFillRiRecpReq);

   node = cell->pCqiSrsSrLst[validIdx].riLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
	  /* MS_FIX: ccpu00124011: Fix for missing reception request for UE with same offset */
	  node = node->next;
      if(ue->cqiCb.riDist ==0)
      {
         rgSCHTomUtlWillUeRprtCqiRi(ue, &willUeRprtCqi);
         if(willUeRprtCqi == FALSE)
         {    
            rgSCHTomUtlMovePriNxtOccasion(cell, ue);
			/* MS_FIX */
            continue;
         }  
         if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                     sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
         {
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillRiRecpReq()"
                     " Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                     cell->cellId));
            MSPD_ERR (" Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                     cell->cellId);
            err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
            RETVALUE(ret);
         }
         /*Fill RI params*/
         pucchRecpInfo->rnti =  ue->ueId; 
         pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx = 
            ue->cqiCb.cqiCfg.cqiSetup.cqiPResIdx; 
         pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz = ue->cqiCb.riNumBits; 
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_CQI;
         ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
         rgSCHTomUtlFillRiBitWidthInfo(ue, TFU_RECP_REQ_PUCCH);
         rgSCHTomUtlMovePriNxtOccasion(cell, ue);
         if (ue->cqiCb.nCqiTrIdx == validIdx)
         {
            rgSCHTomUtlMovePcqiNxtOccasion(cell, ue); 
         }
         if((ue->srsCb.nSrsTrIdx == validIdx) && (ue->srsCb.srsDist ==0))
         {
            rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
         }
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
         pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
      }
      else
      {
         ue->cqiCb.riDist--; 
      }     
	  /* MS_FIX */
   }
   RETVALUE(ROK);
}/* end of rgSCHTomUtlFillRiRecpReq */


/** @brief This function handles filling of PCQI reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  U16           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillPcqiRecpReq
(
 TfuRecpReqInfo       *recpReqInfo,
 RgSchCellCb          *cell,
 U16                  validIdx, 
 RgSchErrInfo         *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillPcqiRecpReq (recpReqInfo, cell, validIdx, err)
   TfuRecpReqInfo     *recpReqInfo;
   RgSchCellCb        *cell;
   U16                validIdx; 
   RgSchErrInfo       *err;
#endif
{
   CmLList              *node;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   RgSchUeCb            *ue;
   U8                    ri; /*RI value*/
   Bool                 willUeRprtCqi;   /* Flag set due to CQI Mask 
                                        and UE Inactive state (DRX)*/
   U8                   cqiPmiSz; 

   TRC2(rgSCHTomUtlFillPcqiRecpReq);

   node = cell->pCqiSrsSrLst[validIdx].cqiLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
	  /* MS_FIX: ccpu00124011: Fix for missing reception request for UE with same offset */
      node = node->next;
      rgSCHTomUtlWillUeRprtCqiRi(ue, &willUeRprtCqi);
      if(willUeRprtCqi == FALSE)
      {
         rgSCHTomUtlMovePcqiNxtOccasion(cell, ue);  
         continue;
      }
      ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
      cqiPmiSz = 
         rgSCHTomUtlFetchPcqiBitSz(ue, cell->numTxAntPorts, &ri); 
      if(!cqiPmiSz)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillPcqiRecpReq()"
                  " Unable to Fill CqiPmi size for ue %d \n", ue->ueId));
         MSPD_ERR (" Unable to Fill CqiPmi size for ue %d \n", ue->ueId);
         continue;
      }

      if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                  sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillPcqiRecpReq()"
                  " Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                  cell->cellId));
         MSPD_ERR (" Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                  cell->cellId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      /*Fill PCQI params*/
      pucchRecpInfo->rnti =  ue->ueId; 
      pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx = 
         ue->cqiCb.cqiCfg.cqiSetup.cqiPResIdx; 
      if(ue->cqiCb.cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP)
      {
         rgSCHTomUtlPcqiSbCalcBpIdx(recpReqInfo->timingInfo,ue); 
      }
      pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz = cqiPmiSz; 
      pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_CQI;
      rgSCHTomUtlMovePcqiNxtOccasion(cell, ue);  
      if((ue->srsCb.nSrsTrIdx == validIdx) && (ue->srsCb.srsDist ==0))
      {
         rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
      }
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
      pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
   }
   RETVALUE(ROK);
}/* end of rgSCHTomUtlFillPcqiRecpReq */

/** @brief This function handles filling of SRS reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  U16           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillSrsRecpReq
(
 TfuRecpReqInfo       *recpReqInfo,
 RgSchCellCb          *cell,
 U16                  validIdx,
 RgSchErrInfo         *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillSrsRecpReq (recpReqInfo, cell, validIdx, err)
   TfuRecpReqInfo       *recpReqInfo;
   RgSchCellCb          *cell;
   U16                  validIdx; 
   RgSchErrInfo         *err;
#endif
{
   CmLList              *node;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   RgSchUeCb            *ue;

   TRC2(rgSCHTomUtlFillSrsRecpReq);

   node = cell->pCqiSrsSrLst[validIdx].srsLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
	  /* MS_FIX: ccpu00124011: Fix for missing reception request for UE with same offset */
      node = node->next;
      if(ue->srsCb.srsDist ==0)
      {
         /* We need to add the recp request to be sent on the pucchANRep value. */
         if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                     sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
         {
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillSrsRecpReq()"
                     " Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                     cell->cellId));
            MSPD_ERR (" Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n",
                     cell->cellId);
            err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
            RETVALUE(ret);
         }
         /*Fill SRS params*/
         pucchRecpInfo->rnti = ue->ueId;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.srsBw = 
            (TfuUlSrsBwInfo)ue->srsCb.srsCfg.srsSetup.srsBw;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.nRrc = 
            ue->srsCb.srsCfg.srsSetup.fDomPosi;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.srsHopBw = 
            (TfuUlSrsHoBwInfo)ue->srsCb.srsCfg.srsSetup.srsHopBw;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.transComb = 
            ue->srsCb.srsCfg.srsSetup.txComb;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCfgIdx = 
            ue->srsCb.srsCfg.srsSetup.srsCfgIdx;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCyclicShft = 
            (TfuUlSrsCycShiftInfo)ue->srsCb.srsCfg.srsSetup.cycShift;
         
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SRS;
         rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
         pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
      }
      else
      {
         ue->srsCb.srsDist--; 
      }
   }
   RETVALUE(ROK);
}/* end of rgSCHTomUtlFillSrsRecpReq */
#endif 
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
/** @brief This function handles filling of data reception requests for
 *  PUSCH and MSG3.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillDatRecpReq
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
RgSchErrInfo         *err
)
#else
PRIVATE S16 rgSCHTomUtlFillDatRecpReq (recpReqInfo, cell, err)
TfuRecpReqInfo       *recpReqInfo;
RgSchCellCb          *cell;
RgSchErrInfo         *err;
#endif
{
   S16               ret;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;
   U8                msg3UnusedRv;
   U8                msg3UnusedNdi;
   U8                msg3UnusedHqpId;
   U8                msg3UnusedNdmrs;
#ifdef DEBUGP
   Inst              inst = cell->instIdx;
#endif
   TRC2(rgSCHTomUtlFillDatRecpReq)

   /* processing steps are 
    * - Run through the UL allocations going out in this subframe.
    * - Run through the UL receptions expected the next subframe.
    */
   alloc = rgSCHUtlFirstRcptnReq (cell);
   while(alloc)
   {
      /* FOR ACK NACK REP */
      if (NULLP != alloc->ue)
      {
         /* If measuring or ackNakRep we shall not send dat RecpReq */
         if ((alloc->ue->measGapCb.isMeasuring == TRUE) ||
               (alloc->ue->ackNakRepCb.isAckNakRep == TRUE))
         {
            alloc = rgSCHUtlNextRcptnReq (cell, alloc);
            continue;
         }

      }
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&datRecpInfo,
                                sizeof(TfuUeRecpReqInfo), 
                                &(recpReqInfo->memCp))) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillDatRecpReq() Unable"
             " to Allocate TfuUeRecpReqInfo for cellId=%d \n", cell->cellId));
         MSPD_ERR ("Unable"
             " to Allocate TfuUeRecpReqInfo for cellId=%d \n", cell->cellId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      /* rg005.201 Placing likely condition before, for optimization */
      if (!alloc->forMsg3)
      {
         datRecpInfo->type = TFU_RECP_REQ_PUSCH;
         /*rg001.201:107387: To include the length 
                         and ModOrder in DataRecp Req.*/
         /* rg006.201: [ccpu00111718] Updating NDI and HARQ proc Id */
         rgSCHUtlAllocRcptInfo (alloc, 
            &datRecpInfo->rnti,
            &datRecpInfo->t.puschRecpReq.mcs,
            &datRecpInfo->t.puschRecpReq.rbStart,
            &datRecpInfo->t.puschRecpReq.numRb,
            &datRecpInfo->t.puschRecpReq.rv,
            &datRecpInfo->t.puschRecpReq.size,
            &datRecpInfo->t.puschRecpReq.modType,
            &datRecpInfo->t.puschRecpReq.isRtx,
            &datRecpInfo->t.puschRecpReq.nDmrs,
            &datRecpInfo->t.puschRecpReq.ndi,
            &datRecpInfo->t.puschRecpReq.harqProcId
            );
         /* rg004.201: Updating isRtx and nDmrs from alloc info - ccpu00109165*/
      }
      else
      {
         datRecpInfo->type = TFU_RECP_REQ_MSG3;
         /* rg001.201:107387: To include the length 
                         and ModOrder in DataRecp Req.*/
         /* rg006.201: [ccpu00111718] Updating NDI and HARQ proc Id */
         rgSCHUtlAllocRcptInfo (alloc, 
            &datRecpInfo->rnti,
            &datRecpInfo->t.msg3RecpReq.mcs,
            &datRecpInfo->t.msg3RecpReq.rbStart,
            &datRecpInfo->t.msg3RecpReq.numRb,
            &msg3UnusedRv,
            &datRecpInfo->t.msg3RecpReq.size,
            &datRecpInfo->t.msg3RecpReq.modType,
            &datRecpInfo->t.msg3RecpReq.isRtx,
            &msg3UnusedNdmrs,
            &msg3UnusedNdi,
            &msg3UnusedHqpId
            );
         
         /* rg003.201: Updating isRtx from alloc info*/
      }
      /* rg001.201 removed extra comments */
      /* Other fields of datRecpInfo shall be filled 
       * here for new features */
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(datRecpInfo->lnk));
      datRecpInfo->lnk.node = (PTR)datRecpInfo;
      
      alloc = rgSCHUtlNextRcptnReq (cell, alloc);
   } /* end of while */
   RETVALUE(ROK); 
} /* end of rgSCHTomUtlFillDatRecpReq */

#else
/** @brief This function handles filling of data reception requests for
 *  PUSCH and MSG3.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  U16           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillDatRecpReq
(
 TfuRecpReqInfo       *recpReqInfo,
 RgSchCellCb          *cell,
 U16                  validIdx,
 RgSchErrInfo         *err
 )
#else
PRIVATE S16 rgSCHTomUtlFillDatRecpReq (recpReqInfo, cell, validIdx, err)
   TfuRecpReqInfo       *recpReqInfo;
   RgSchCellCb          *cell;
   U16                  validIdx;
   RgSchErrInfo         *err;
#endif
{
   S16               ret;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;
#ifdef DEBUGP
   Inst              inst = cell->instIdx;
#endif
   CmLteTimingInfo   timeInfo;
   Bool              hqPres = FALSE;   /*Set when HARQ Rec Req is present*/
   Bool              isAperiodic = FALSE; /*Set when Aperiodic CQI is expected */

   TRC2(rgSCHTomUtlFillDatRecpReq);

   /* processing steps are 
    * - Run through the UL allocations going out in this subframe.
    * - Run through the UL receptions expected the next subframe.
    */

   /* Timing of this feedback must be for a transmission 4 subframes before.
   */
   RGSCHSUBFRMCRNTTIME(recpReqInfo->timingInfo, timeInfo, 4);
   alloc = rgSCHUtlFirstRcptnReq (cell);
   while(alloc)
   {
      /* FOR ACK NACK REP */
      if (NULLP != alloc->ue)
      {
         /* If measuring or ackNakRep we shall not send dat RecpReq */
         if ((alloc->ue->measGapCb.isMeasuring == TRUE) ||
               (alloc->ue->ackNakRepCb.isAckNakRep == TRUE))
         {
            rgSCHTomUtlMoveNxtOccasion(cell, alloc->ue, validIdx);
            alloc = rgSCHUtlNextRcptnReq (cell, alloc);
            continue;
         }

      }
      ret = rgSCHUtlGetEventMem((Ptr *)&datRecpInfo,
            sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
      if(ret != ROK)            
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlFillDatRecpReq() Unable"
            " to Allocate TfuUeRecpReqInfo for cellId=%d \n", cell->cellId));
         MSPD_ERR ("Unable"
            " to Allocate TfuUeRecpReqInfo for cellId=%d \n", cell->cellId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }

      datRecpInfo->type = TFU_RECP_REQ_PUSCH;
      /* Check if this if for MSG3 - no scope for feedback along with it. */
      if ((FALSE == alloc->forMsg3))
      {
         /*  Check if any DL HARQ processes has a feedback coming at the time of
          *  this reception request. 
          */
         if(NULLP != alloc->ue)
         {
            if ( NULLP != alloc->ue->dl.hqEnt->inUse.first)
            {
               hqPres = rgSCHTomUtlFillDatHarqRecpReq(cell,alloc, 
                     datRecpInfo, recpReqInfo);
            }       

            if(alloc->pdcch && alloc->pdcch->dci.u.format0Info.cqiReq) 
            {
               /* Aperiodic CQI has to be received */
               rgSCHTomUtlFillDatAperRecpReq(cell,alloc, datRecpInfo,
                     &timeInfo, hqPres, validIdx);
               isAperiodic = TRUE; 
            }
            else if((alloc->ue->cqiCb.nRiTrIdx == validIdx) && 
                    (alloc->ue->cqiCb.riDist == 0))
            {
               /* Periodic RI has to be received */
               rgSCHTomUtlFillDatPriRecpReq(cell,alloc, datRecpInfo, 
                     &timeInfo, hqPres, validIdx);
            }
            else if(alloc->ue->cqiCb.nCqiTrIdx == validIdx)
            {
               /* Periodic CQI has to be received */
               rgSCHTomUtlFillDatPCqiRecpReq(cell,alloc, datRecpInfo, &timeInfo,
                     hqPres, validIdx);
            }
            else if((alloc->ue->srsCb.nSrsTrIdx == validIdx) && 
                  (alloc->ue->srsCb.srsDist ==0))
            {
               /* SRS has to be received */
               rgSCHTomUtlFillDatSrsRecpReq(cell,alloc, datRecpInfo, 
                     &timeInfo, hqPres);  
            }
            else
            {
               if(hqPres)
               {
                  datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_HARQ;
               }
               else
               {
                  datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA;
               }
               datRecpInfo->rnti = alloc->rnti;
               rgSCHUtlAllocRcptInfo (cell,alloc, &timeInfo,
                        &datRecpInfo->t.puschRecpReq.ulSchInfo);
            }
            rgSCHTomUtlMoveNxtOccasion(cell, alloc->ue, validIdx);
         }
      }
      else /*Enters for Msg3 == TRUE condition*/
      {
         /* Only DATA is expected */
         datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA;
         datRecpInfo->rnti = alloc->rnti;
         rgSCHUtlAllocRcptInfo (cell,alloc, &timeInfo,
               &datRecpInfo->t.puschRecpReq.ulSchInfo);
      }
      /* rg001.301- ADD - [ccpu00118643]Initial Num RBs support for UCI on PUSCH - START */
      if(alloc->ue)
      {
         if(datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA &&
               datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA_SRS &&
               isAperiodic == FALSE)
         {
            datRecpInfo->t.puschRecpReq.initialNumRbs.pres = TRUE;
            datRecpInfo->t.puschRecpReq.initialNumRbs.val = alloc->ue->initNumRbs;
         }
         else
         {
            datRecpInfo->t.puschRecpReq.initialNumRbs.pres = FALSE;
         }
      }
      /* rg001.301- ADD - [ccpu00118643]Initial Num RBs support for UCI on PUSCH - END */
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(datRecpInfo->lnk));
      datRecpInfo->lnk.node = (PTR)datRecpInfo;
      alloc = rgSCHUtlNextRcptnReq (cell, alloc);
   } /* end of while */
   RETVALUE(ROK); 
} /* end of rgSCHTomUtlFillDatRecpReq */
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
/***********************************************************
 *
 *     Func : rgSCHTomUtlFillRiBitWidthInfo
 *
 *
 *     Desc : Fills the RI BitWidth and stores it for decoding.
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
PUBLIC S16 rgSCHTomUtlFillRiBitWidthInfo
(
 RgSchUeCb     *ueCb,
 U8            recReqType
 )
#else
PUBLIC S16 rgSCHTomUtlFillRiBitWidthInfo(ueCb, recReqType)
   RgSchUeCb   *ueCb;
   U8          recReqType;
#endif
{
   TRC2(rgSCHTomUtlFillRiBitWidthInfo);
   
   if (ueCb->mimoInfo.txMode != RGR_UE_TM_3 &&
             ueCb->mimoInfo.txMode != RGR_UE_TM_4)
   {
      RETVALUE(RFAILED);
   }

   if(recReqType == TFU_RECP_REQ_PUCCH)
   {
      ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].type = TFU_RECP_REQ_PUCCH;
      ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.mode= 
         (TfuDlCqiPucchMode)ueCb->cqiCb.cqiCfg.cqiSetup.prdModeEnum;
      switch(ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.mode)
      {
         case TFU_PUCCH_CQI_MODE10:
            ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode10Info.type = TFU_RPT_RI;
            ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode10Info.u.ri = 
               ueCb->cqiCb.riNumBits;
            break;
         case TFU_PUCCH_CQI_MODE11:
            ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode11Info.type = TFU_RPT_RI;
            ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode11Info.u.ri = 
               ueCb->cqiCb.riNumBits;
            break;
         case TFU_PUCCH_CQI_MODE20:
            ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode20Info.type = TFU_RPT_RI;
            ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode20Info.u.ri = 
               ueCb->cqiCb.riNumBits;
            break;
         case TFU_PUCCH_CQI_MODE21:
            ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode21Info.type = TFU_RPT_RI;
            ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode21Info.u.ri = 
               ueCb->cqiCb.riNumBits;
            break;
         default:
            break;
      }
   }
   else /*PUSCH*/
   {
      ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].type = TFU_RECP_REQ_PUSCH;
      ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.puschRawCqiBitWidth.ri.pres= TRUE;
      ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.puschRawCqiBitWidth.ri.val = ueCb->acqiCb.riNumBits;
   }
   
   RG_SCH_INCR_CQIRI_INDEX(ueCb->cqiRiWritIdx);
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHTomUtlFetchPcqiBitSz
 *
 *
 *     Desc : Fetch the CQI/PMI bits for a UE based on the mode, periodicity.
 *
 *     Ret  : U8
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC U8 rgSCHTomUtlFetchPcqiBitSz
(
 RgSchUeCb    *ueCb, 
 U8           numTxAnt,
 U8           *ri
 )
#else
PUBLIC U8 rgSCHTomUtlFetchPcqiBitSz(ueCb, numTxAnt, ri)
   RgSchUeCb     *ueCb;
   U8            numTxAnt;
   U8            *ri;
#endif
{
   U8 confRepMode;
   U8 pcqiSz;
   RgSchCmnDlUe       *ueCmnDl = RG_SCH_CMN_GET_DL_UE(ueCb);
   TfuCqiPucchMode10   *mode10Info;    
   TfuCqiPucchMode11   *mode11Info;    
   TfuCqiPucchMode20   *mode20Info;   
   TfuCqiPucchMode21   *mode21Info;    
   TRC3(rgSCHTomUtlFetchPcqiBitSz);
   confRepMode = ueCb->cqiCb.cqiCfg.cqiSetup.prdModeEnum;
   if((ueCb->mimoInfo.txMode != RGR_UE_TM_3) && 
         (ueCb->mimoInfo.txMode != RGR_UE_TM_4))
   {
      *ri =1;
   }
   else
   {
      *ri = ueCmnDl->mimoInfo.ri;
   }
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].type = TFU_RECP_REQ_PUCCH;
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.mode= 
       (TfuDlCqiPucchMode)confRepMode;
   switch(confRepMode)
   {
      case RGR_PRD_CQI_MOD10:
         {
            mode10Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode10Info;
            pcqiSz = 4;
            mode10Info->type = TFU_RPT_CQI;
            mode10Info->u.cqi = 4;
         }
         break;

      case RGR_PRD_CQI_MOD11:
         {
            mode11Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode11Info;
            mode11Info->type = TFU_RPT_CQI;
            if(numTxAnt == 2)
            {
               if (*ri ==1)
               {
                  pcqiSz = 6;
                  mode11Info->u.cqi.cqi = 4;
                  mode11Info->u.cqi.wideDiffCqi.pres = FALSE;
                  mode11Info->u.cqi.pmi = 2;
               }
               else
               {
                  pcqiSz = 8;
                  mode11Info->u.cqi.cqi = 4;
                  mode11Info->u.cqi.wideDiffCqi.pres = TRUE;
                  mode11Info->u.cqi.wideDiffCqi.val = 3;
                  mode11Info->u.cqi.pmi = 1;
               }
            }
            else if(numTxAnt == 4)
            {
               if (*ri ==1)
               {
                  pcqiSz = 8;
                  mode11Info->u.cqi.cqi = 4;
                  mode11Info->u.cqi.wideDiffCqi.pres = FALSE;
                  mode11Info->u.cqi.pmi = 4;
               }
               else
               {
                  pcqiSz = 11;
                  mode11Info->u.cqi.cqi = 4;
                  mode11Info->u.cqi.wideDiffCqi.pres = TRUE;
                  mode11Info->u.cqi.wideDiffCqi.val = 3;
                  mode11Info->u.cqi.pmi = 4;
               }
            }
            else
            {
               /* This is number of antenna case 1.
                * This is not applicable for Mode 1-1. 
                * So setting it to invalid value */
               pcqiSz = 0;
            }
         }
         break;

      case RGR_PRD_CQI_MOD20:
         {
            mode20Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode20Info;
            mode20Info->type = TFU_RPT_CQI;
            if(ueCb->cqiCb.isWb)
            {
               pcqiSz = 4;
               mode20Info->u.cqi.isWideband = TRUE;
               mode20Info->u.cqi.u.wideCqi = 4;
            }
            else
            {
               pcqiSz = 4 + ueCb->cqiCb.label;
               mode20Info->u.cqi.isWideband = FALSE;
               mode20Info->u.cqi.u.subCqi.cqi = 4;
               mode20Info->u.cqi.u.subCqi.l = ueCb->cqiCb.label;
            }
         }
         break;

      case RGR_PRD_CQI_MOD21:
         {
            mode21Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].t.pucchRawCqiBitWidth.u.mode21Info;
            mode21Info->type = TFU_RPT_CQI;
            pcqiSz = rgSCHTomUtlFetchPcqiBitSzPucchMode21(ueCb, 
                                 mode21Info, numTxAnt, ri);
         }
         break;
      default:
          pcqiSz = 0;
          break;
   }
   
   RG_SCH_INCR_CQIRI_INDEX(ueCb->cqiRiWritIdx);
   RETVALUE(pcqiSz);
}


/***********************************************************
 *
 *     Func : rgSCHTomUtlPcqiSbCalcBpIdx
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
PUBLIC S16 rgSCHTomUtlPcqiSbCalcBpIdx
(
 CmLteTimingInfo    crntTimInfo,
 RgSchUeCb          *ueCb
 )
#else
PUBLIC S16 rgSCHTomUtlPcqiSbCalcBpIdx(crntTimInfo, ueCb)
   CmLteTimingInfo  crntTimInfo;
   RgSchUeCb        *ueCb;
#endif
{
   U16 tti = (crntTimInfo.sfn * RGSCH_NUM_SUB_FRAMES + crntTimInfo.subframe);
   U16  prdNum = tti/ueCb->cqiCb.cqiPeri;
   TRC2(rgSCHTomUtlPcqiSbCalcBpIdx);
   if((prdNum % ueCb->cqiCb.h) == 0)
   {
      ueCb->cqiCb.isWb = TRUE;
   }
   else
   {
      ueCb->cqiCb.isWb = FALSE;
      ueCb->cqiCb.bpIdx = ((prdNum % ueCb->cqiCb.h) - 1) % ueCb->cqiCb.J;  
   }
   RETVALUE(ROK);
}


/**
 * @brief Function which moves PCQI, RI, SR and SRS to next periodicity
 *           Occasions as that needs to be done in case of Ack/Nack repetition
 *           reception request occasions or during Measurement Gap occasions.
 *
 * @details
 *
 *     Function: rgSCHTomUtlMoveNxtOccasion
 *
 *       Function which moves PCQI, RI, SR and SRS to next perodicity
 *      Occasions as that needs to be done in case of Ack/Nack repetition
 *      reception request occasions or during Measurement Gap occasions.
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Check whether the current Tx Instance matches with the rec req time
 *     - If true, then move them to their next Tx Instance
 *           
 *  @param[in]   RgSchCellCb    *cell,
 *               RgSchUeCb      *ue,
 *               U16            validIdx
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlMoveNxtOccasion
(
 RgSchCellCb    *cell,
 RgSchUeCb      *ue,
 U16            validIdx
 )
#else
PRIVATE S16 rgSCHTomUtlMoveNxtOccasion(cell, ue, validIdx)
   RgSchCellCb    *cell;
   RgSchUeCb      *ue;
   U16            validIdx;
#endif
{
   TRC2(rgSCHTomUtlMoveNxtOccasion);

   if (ue->cqiCb.nRiTrIdx == validIdx)
   {
      if(ue->cqiCb.riDist ==0)
      {
         rgSCHTomUtlMovePriNxtOccasion(cell, ue); 
      }
      else
      {
         ue->cqiCb.riDist--; 
      }
   }
   if (ue->cqiCb.nCqiTrIdx == validIdx)
   {
      rgSCHTomUtlMovePcqiNxtOccasion(cell, ue); 
   }
   if (ue->srsCb.nSrsTrIdx == validIdx)
   {
      if(ue->srsCb.srsDist ==0)
      {
         rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);  
      }
      else
      {
         ue->srsCb.srsDist--; 
      }
   }
   if (ue->srCb.nSrTrIdx == validIdx)
   {
      rgSCHTomUtlMoveSrNxtOccasion(cell, ue); 
   }
   RETVALUE(ROK);
}  /* rgSCHTomUtlMoveNxtOccasion */


/**
 * @brief Function which handles the filling of HARQ reception
 * request values which arrives along with UL Data on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatHarqRecpReq
 *
 *    Function which handles the filling of HARQ reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:

 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the HARQ
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *                      RgSchUlAlloc      *alloc,
 *                      CmLteTimingInfo         *timeInfo,
 *                      U8                     tbCount
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/

#ifdef ANSI
PRIVATE Bool rgSCHTomUtlFillDatHarqRecpReq
(
 RgSchCellCb       *cell,
 RgSchUlAlloc      *alloc,
 TfuUeRecpReqInfo  *datRecpInfo,
 TfuRecpReqInfo    *recpReqInfo
 )
#else
PRIVATE Bool rgSCHTomUtlFillDatHarqRecpReq(cell, alloc, datRecpInfo, recpReqInfo)
   RgSchCellCb       *cell;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;
   TfuRecpReqInfo    *recpReqInfo;
#endif
{
   U8                tbIdx;
   U8                tbCount = 0;
   U8                itr;
   RgSchDlHqTbCb     *tbCb;
   RgSchDlHqProcCb   *dlHq;
   TfuUePuschHqRecpInfo   *hqRecpReqInfo;
   CmLteTimingInfo         timeInfo;
   Bool hqPres= FALSE; 
   CmLList           *node;
   Bool               found = FALSE;
   U32                nodeCnt = 0;

   TRC2(rgSCHTomUtlFillDatHarqRecpReq);

   RGSCHSUBFRMCRNTTIME(recpReqInfo->timingInfo, timeInfo, 4);

   /*ccpu00118540-MOD-new variables introduced */
   dlHq      = (RgSchDlHqProcCb*)(alloc->ue->dl.hqEnt->inUse.first->node);
   node      = (alloc->ue->dl.hqEnt->inUse.first);
   nodeCnt   = (alloc->ue->dl.hqEnt->inUse.count); 

   /*ccpu00118540-MOD-modified the logic to match timinInfo with
    * fdbkTime. Assume, DL data for a UE is sent at (x,8). 
    * Reception request will generated at (x+1,0). At (x+1,1),
    * assume, we send DL data + UL Pdcch for the same UE. 
    * At (x+1,3),we expect Harq feedack for the first DL transmission 
    * and Recp Req for ULData + Harq for second DL transmission. However,
    * it is possible at the time of generation RecpReq, Feedback
    * has not been received for first DL transmission (ergo Hq proc
    * is still in use). Therefore, we now compare timingInfo 
    * 4+TFU_HQFBKIND_ULDELTA deep in the link list 
    */
   for ( itr=0; nodeCnt && itr < (4 + TFU_HQFBKIND_ULDELTA) ; itr++,nodeCnt--)
   {
      dlHq = (RgSchDlHqProcCb*)(node->node);
      
      if ( dlHq == (RgSchDlHqProcCb *)NULLP )
      {
        continue;
      }

     for (tbIdx = 0; tbIdx < 2; tbIdx++)
     {
        tbCb = &(dlHq->tbInfo[tbIdx]);
#ifndef LTE_TDD
      if ((tbCb->state == HQ_TB_WAITING) &&
            (RGSCH_TIMEINFO_SAME(tbCb->timingInfo, timeInfo)))
#else
         if ((tbCb->state == HQ_TB_WAITING) &&
           (RGSCH_TIMEINFO_SAME(tbCb->fdbkTime, recpReqInfo->timingInfo)))
#endif
         {
            /* atleast one is TB's feedback is coming. */
            tbCount++;
            hqPres = TRUE;
            found = TRUE;
	         
         }
     }/*for(tbIdx...*/

     if ( found == TRUE)
     {
       break;
     }
   
     node = node->next;

   }/*for(itr=0...*/


   if (hqPres)
   {
      /* Fill the harq related information */
      hqRecpReqInfo = &datRecpInfo->t.puschRecpReq.hqInfo;
      hqRecpReqInfo->hqBetaOff = alloc->ue->ul.betaHqOffst;
#ifdef LTE_TDD
      /* Though different enums however values are same i.e.
       * MULTIPLEXING = 0
       * BUNDLING = 1 
       */
      hqRecpReqInfo->hqFdbkMode = (TfuAckNackMode)alloc->ue->dl.ackNackMode;
      if (alloc->ue->dl.ackNackMode == RGR_TDD_ACKNACK_MODE_BUNDL)
      {
         /* for TX mode 3 and 4 UE shall generate 1 bit per CW */
         hqRecpReqInfo->hqSz = tbCount;
      }
      else
      {
         /* ACK NACK multiplexing */
         hqRecpReqInfo->hqSz = alloc->pdcch->dci.u.format0Info.dai;
      }
#else /* TFU_TDD */
      /* Number of HARQ bits to be expected */
      hqRecpReqInfo->hqSz = tbCount;


#endif /* TFU_TDD */
      /*rg002.301 -Add - [ccpu00120233] - TB Count; DEL - rcptDone flag*/
      dlHq->tbCnt = tbCount;
   }
   RETVALUE(hqPres);
}  /* rgSCHTomUtlFillDatHarqRecpReq */



/**
 * @brief Function which handles the filling of Aperiodic CQI/RI reception
 * request values 
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatAperRecpReq
 *
 *    Function which handles the filling of Aperiodic CQI/RI reception
 *    request values
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the Aperiodic CQI/PMI/RI
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *              CmLteTimingInfo   *timeInfo,
 *              Bool              hqPres
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillDatAperRecpReq
(
 RgSchCellCb       *cell,
 RgSchUlAlloc      *alloc,
 TfuUeRecpReqInfo  *datRecpInfo,
 CmLteTimingInfo   *timeInfo,
 Bool              hqPres,
 U16                validIdx
 )
#else
PRIVATE S16 rgSCHTomUtlFillDatAperRecpReq(cell, alloc, datRecpInfo,
timeInfo, hqPres, validIdx)
   RgSchCellCb       *cell;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;
   CmLteTimingInfo   *timeInfo;
   Bool              hqPres;
   U16                validIdx; 
#endif
{
   TfuUePuschCqiRecpInfo  *cqiRecpReqInfo;
   CmLteTimingInfo         cqiTimeInfo;

   TRC2(rgSCHTomUtlFillDatAperRecpReq);

   /*Fill RI Reception Params*/
   cqiRecpReqInfo = &datRecpInfo->t.puschRecpReq.cqiRiInfo;   
   cqiRecpReqInfo->riBetaOff = alloc->ue->ul.betaRiOffst;
   cqiRecpReqInfo->cqiBetaOff = alloc->ue->ul.betaCqiOffst;
   cqiRecpReqInfo->cqiPmiSzR1 = alloc->ue->acqiCb.cqiPmiSzR1;
   cqiRecpReqInfo->cqiPmiSzRn1 = alloc->ue->acqiCb.cqiPmiSzRn1; 
   cqiRecpReqInfo->riSz.pres = TRUE;
   cqiRecpReqInfo->riSz.val = alloc->ue->acqiCb.riNumBits; 
   //alloc->ue->rawCqiBitW[alloc->ue->cqiRiWritIdx].recvTime = *timeInfo;
   RGSCHADDTOCRNTTIME((*timeInfo), cqiTimeInfo, 4);
   alloc->ue->rawCqiBitW[alloc->ue->cqiRiWritIdx].recvTime = cqiTimeInfo;
   rgSCHTomUtlFillRiBitWidthInfo(alloc->ue, TFU_RECP_REQ_PUSCH);
   if((alloc->ue->srsCb.nSrsTrIdx == validIdx) && (alloc->ue->srsCb.srsDist ==0))
   {
         rgSCHTomFillOnlySrsRecpReq(cell,alloc, datRecpInfo); 
         datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_SRS; 
         
   }
   if(hqPres && 
   (datRecpInfo->t.puschRecpReq.rcpInfo == TFU_PUSCH_DATA_CQI_SRS))
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ_SRS;
   }
   else if (hqPres)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ;
   }
   else if(datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA_CQI_SRS)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI;
   }
   datRecpInfo->rnti = alloc->rnti;
   rgSCHUtlAllocRcptInfo (cell, alloc, timeInfo,
            &datRecpInfo->t.puschRecpReq.ulSchInfo);
   RETVALUE(ROK);
}  /* rgSCHTomUtlFillDatAperRecpReq */



/**
 * @brief Function which handles the filling of Periodic RI reception
 * request values which arrives along with UL Data on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatPriRecpReq
 *
      *    Function which handles the filling of Periodic RI reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the Periodic RI
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *              CmLteTimingInfo   *timeInfo,
 *              Bool              hqPres
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillDatPriRecpReq
(
 RgSchCellCb       *cell,
 RgSchUlAlloc      *alloc,
 TfuUeRecpReqInfo  *datRecpInfo,
 CmLteTimingInfo   *timeInfo,
 Bool              hqPres,
 U16               validIdx
 )
#else
PRIVATE S16 rgSCHTomUtlFillDatPriRecpReq(cell, alloc, datRecpInfo, timeInfo, 
hqPres, validIdx)
   RgSchCellCb       *cell;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;
   CmLteTimingInfo   *timeInfo;
   Bool              hqPres; 
   U16                validIdx;
#endif
{
   TfuUePuschCqiRecpInfo  *cqiRecpReqInfo;
   TRC2(rgSCHTomUtlFillDatPriRecpReq);

   /*Fill RI Reception Params*/
   cqiRecpReqInfo = &datRecpInfo->t.puschRecpReq.cqiRiInfo;   
   cqiRecpReqInfo->riBetaOff = alloc->ue->ul.betaRiOffst;
   cqiRecpReqInfo->riSz.pres= TRUE;
   cqiRecpReqInfo->riSz.val = alloc->ue->cqiCb.riNumBits;
    /*Other params*/
   alloc->ue->rawCqiBitW[alloc->ue->cqiRiWritIdx].recvTime = *timeInfo;
   rgSCHTomUtlFillRiBitWidthInfo(alloc->ue, TFU_RECP_REQ_PUCCH);
    if((alloc->ue->srsCb.nSrsTrIdx == validIdx) && (alloc->ue->srsCb.srsDist ==0))
   {
         rgSCHTomFillOnlySrsRecpReq(cell,alloc, datRecpInfo);
         datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_SRS; 

   }
   if(hqPres && 
   (datRecpInfo->t.puschRecpReq.rcpInfo == TFU_PUSCH_DATA_CQI_SRS))
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ_SRS;
   }
   else if (hqPres)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ;
   }
   else if(datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA_CQI_SRS)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI;
   }
   datRecpInfo->rnti = alloc->rnti;
   rgSCHUtlAllocRcptInfo (cell, alloc, timeInfo,
         &datRecpInfo->t.puschRecpReq.ulSchInfo);
   RETVALUE(ROK);
}  /* rgSCHTomUtlFillDatPriRecpReq */


/**
 * @brief Function which handles the filling of Periodic CQI/PMI reception
 * request values which arrives along with UL Data on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatPCqiRecpReq
 *
 *    Function which handles the filling of Periodic CQI/PMI reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the Periodic CQI/PMI
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *              CmLteTimingInfo         *timeInfo,
 *              Bool hqPres
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/

#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillDatPCqiRecpReq
(
 RgSchCellCb       *cell,
 RgSchUlAlloc      *alloc,
 TfuUeRecpReqInfo  *datRecpInfo,
 CmLteTimingInfo   *timeInfo,
 Bool              hqPres, 
 U16                validIdx
 )
#else
PRIVATE S16 rgSCHTomUtlFillDatPCqiRecpReq(cell, alloc, datRecpInfo, 
      timeInfo, hqPres, validIdx)
   RgSchCellCb       *cell;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;
   CmLteTimingInfo   *timeInfo;
   Bool              hqPres; 
   U16                validIdx;
#endif
{
   TfuUePuschCqiRecpInfo  *cqiRecpReqInfo;
   CmLteTimingInfo         recreqTimeInfo;
   U8                 cqiPmiSz; /*Raw CQI/PMI Size*/
   U8                 ri;
   CmLteTimingInfo    cqiTimeInfo;

   TRC2(rgSCHTomUtlFillDatPCqiRecpReq);
   RGSCHADDTOCRNTTIME(cell->crntTime, recreqTimeInfo, 2);

   /*Fill CQI Reception Params*/
   cqiRecpReqInfo = &datRecpInfo->t.puschRecpReq.cqiRiInfo;
   cqiRecpReqInfo->cqiBetaOff = alloc->ue->ul.betaCqiOffst;
   if(alloc->ue->cqiCb.cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP)
   {
      rgSCHTomUtlPcqiSbCalcBpIdx(recreqTimeInfo, alloc->ue); 
   }
   //alloc->ue->rawCqiBitW[alloc->ue->cqiRiWritIdx].recvTime = *timeInfo;
   RGSCHADDTOCRNTTIME((*timeInfo), cqiTimeInfo, 4);
   alloc->ue->rawCqiBitW[alloc->ue->cqiRiWritIdx].recvTime = cqiTimeInfo;
   cqiPmiSz = rgSCHTomUtlFetchPcqiBitSz(alloc->ue, cell->numTxAntPorts, &ri);
   if(0 == cqiPmiSz)
   {
      RGSCHDBGERR ( cell->instIdx,(rgSchPBuf( cell->instIdx),
               "rgSCHTomUtlFillPcqiRecpReq()"
               " Unable to Fill CqiPmi size  \n"));
      MSPD_ERR (" Unable to Fill CqiPmi size  \n");
      RETVALUE(RFAILED);
   }
   if (ri ==1)
   {
      cqiRecpReqInfo->cqiPmiSzR1 = cqiPmiSz;
      cqiRecpReqInfo->cqiPmiSzRn1= 0;
   }
   else
   {
      cqiRecpReqInfo->cqiPmiSzRn1= cqiPmiSz;
      cqiRecpReqInfo->cqiPmiSzR1 = 0;
   }
   cqiRecpReqInfo->riSz.pres = FALSE; 

   if((alloc->ue->srsCb.nSrsTrIdx == validIdx) && (alloc->ue->srsCb.srsDist ==0))
   {
         rgSCHTomFillOnlySrsRecpReq(cell,alloc, datRecpInfo); 
         datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_SRS; 
   }
   if(hqPres && 
   (datRecpInfo->t.puschRecpReq.rcpInfo == TFU_PUSCH_DATA_CQI_SRS))
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ_SRS;
   }
   else if (hqPres)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ;
   }
   else if(datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA_CQI_SRS)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI;
   }
   datRecpInfo->rnti = alloc->rnti;
   rgSCHUtlAllocRcptInfo (cell, alloc, timeInfo,
            &datRecpInfo->t.puschRecpReq.ulSchInfo);
   RETVALUE(ROK);
}  /* rgSCHTomUtlFillDatPCqiRecpReq */

/**
 * @brief Function which handles the filling of SRS reception
 * request values which arrives along with UL Data on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatSrsRecpReq
 *
 *    Function which handles the filling of SRS reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the SRS
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *              CmLteTimingInfo   *timeInfo,
 *              Bool              hqPres
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillDatSrsRecpReq
(
 RgSchCellCb       *cell,
 RgSchUlAlloc      *alloc,
 TfuUeRecpReqInfo  *datRecpInfo,
 CmLteTimingInfo   *timeInfo,
 Bool              hqPres
 )
#else
PRIVATE S16 rgSCHTomUtlFillDatSrsRecpReq(cell, alloc, datRecpInfo, timeInfo, 
      hqPres)
   RgSchCellCb       *cell;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;
   CmLteTimingInfo   *timeInfo;
   Bool              hqPres; 
#endif
{
   TRC2(rgSCHTomUtlFillDatSrsRecpReq);
   datRecpInfo->rnti = alloc->rnti;
   rgSCHTomFillOnlySrsRecpReq(cell,alloc, datRecpInfo); 
   if(hqPres)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_HARQ_SRS;
   }
   else
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_SRS;
   }
   datRecpInfo->rnti = alloc->rnti;
   rgSCHUtlAllocRcptInfo (cell, alloc, timeInfo, 
            &datRecpInfo->t.puschRecpReq.ulSchInfo);
   RETVALUE(ROK);
}  /* rgSCHTomUtlFillDatSrsRecpReq */

/**
 * @brief Function which handles the filling of only SRS reception
 * request values on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomFillOnlySrsRecpReq
 *
 *    Function which handles the filling of SRS reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatSrsRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the SRS
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomFillOnlySrsRecpReq
(
 RgSchCellCb       *cell,
 RgSchUlAlloc      *alloc,
 TfuUeRecpReqInfo  *datRecpInfo
 )
#else
PRIVATE S16 rgSCHTomFillOnlySrsRecpReq(cell, alloc, datRecpInfo)  
   RgSchCellCb       *cell;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;
#endif
{
   TfuUePuschSrsRecpInfo  *srsRecpReqInfo;
   TRC2(rgSCHTomFillOnlySrsRecpReq);

   srsRecpReqInfo = &datRecpInfo->t.puschRecpReq.srsInfo;
   srsRecpReqInfo->srsBw = (TfuUlSrsBwInfo)alloc->ue->srsCb.srsCfg.srsSetup.srsBw;
   srsRecpReqInfo->nRrc = alloc->ue->srsCb.srsCfg.srsSetup.fDomPosi;
   srsRecpReqInfo->srsHopBw = (TfuUlSrsHoBwInfo)alloc->ue->srsCb.srsCfg.srsSetup.srsHopBw;
   srsRecpReqInfo->transComb = alloc->ue->srsCb.srsCfg.srsSetup.txComb;
   srsRecpReqInfo->srsCfgIdx = alloc->ue->srsCb.srsCfg.srsSetup.srsCfgIdx;
   srsRecpReqInfo->srsCyclicShft = (TfuUlSrsCycShiftInfo)alloc->ue->srsCb.srsCfg.srsSetup.cycShift;

   /* ccpu00117050 - ADD - nSrs setting 
    * Refer Section 5.2.2.6 of TS 36.212 V8.5.0*/
   datRecpInfo->t.puschRecpReq.ulSchInfo.nSrs = 1;

   RETVALUE(ROK);
}  /* rgSCHTomFillOnlySrsRecpReq */

/**
 * @brief Function which handles the filling of PCQI/RI, SRS and SR
 *             Reception Request Information along
 *             with the HARQ reception Request
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillCqiSrSrsWithHq
 *
 *              Function which handles the filling of PCQI/RI, SRS ans SR
 *             Reception Request Information along
 *             with the HARQ reception Request
 *
 *
 *     Invoked by: rgSCHTomUtlFillHqFdbkRecpReq & 
 *          rgSCHTomUtlFillSfHqFdbk of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the Control Info arriving on the PUCCH
 *     - Fill the reception request information for the SR, RI, CQI, SRS
 *           
 *  @param[in]   RgSchCellCb       *cell,
 *               TfuRecpReqInfo    *recpReqInfo,
 *               RgSchDlHqProcCb   *hqCb,
 *  @param[out]  TfuUeRecpReqInfo  *pucchRecpInfo
 *  @param[in]   U16               validIdx
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillCqiSrSrsWithHq
(
 RgSchCellCb       *cell,
 TfuRecpReqInfo    *recpReqInfo,
 RgSchUeCb   *ue,
 TfuUeRecpReqInfo  *pucchRecpInfo,
 U16               validIdx
 )
#else
PRIVATE S16 rgSCHTomUtlFillCqiSrSrsWithHq(cell, recpReqInfo, ue, 
      pucchRecpInfo, validIdx)
RgSchCellCb       *cell;
TfuRecpReqInfo    *recpReqInfo;
RgSchUeCb   *ue;
TfuUeRecpReqInfo  *pucchRecpInfo;
U16               validIdx;
#endif
{
   U8                 ri; /*To fetch RI value*/
   Bool               willUeRprtCqi;   /* Flag set due to CQI Mask and 
                                      UE Inactive state (DRX)*/
  

   TRC2(rgSCHTomUtlFillCqiSrSrsWithHq);
    
   if(ue)
   {
      rgSCHTomUtlWillUeRprtCqiRi(ue, &willUeRprtCqi);
      if(ue->srCb.nSrTrIdx == validIdx)
      {
         /*Fill SR params*/
         pucchRecpInfo->t.pucchRecpReq.srInfo.n1PucchIdx = 
         ue->srCb.srCfg.srSetup.srResIdx; 
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR;
         rgSCHTomUtlMoveSrNxtOccasion(cell, ue);
      }
      if(ue->cqiCb.nRiTrIdx == validIdx)
      {
         if(ue->cqiCb.riDist == 0)
         {
            if((ue->cqiCb.cqiCfg.cqiSetup.sANCQI == TRUE) && (willUeRprtCqi == TRUE))
            {
               /*Fill RI params*/
               pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx =
                  ue->cqiCb.cqiCfg.cqiSetup.cqiPResIdx; 
               pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz = 
                  ue->cqiCb.riNumBits; 
               if(pucchRecpInfo->t.pucchRecpReq.uciInfo == TFU_PUCCH_HARQ_SR)
               {
                  pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_CQI;
               }
               else
               {
                  pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_CQI;
               }
               ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
               rgSCHTomUtlFillRiBitWidthInfo(ue, TFU_RECP_REQ_PUCCH);
               if (ue->cqiCb.nCqiTrIdx == validIdx)
               {
                  rgSCHTomUtlMovePcqiNxtOccasion(cell, ue); 
               } 
            }
            rgSCHTomUtlMovePriNxtOccasion(cell, ue);
         }
         else
         {
            ue->cqiCb.riDist--;
         }
      }
      if(ue->cqiCb.nCqiTrIdx == validIdx)
      {
         if((ue->cqiCb.cqiCfg.cqiSetup.sANCQI == TRUE) && (willUeRprtCqi == TRUE))
         {
            /*Fill CQI Params*/
            pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx =
               ue->cqiCb.cqiCfg.cqiSetup.cqiPResIdx; 

            if(ue->cqiCb.cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP)
            {
               rgSCHTomUtlPcqiSbCalcBpIdx(recpReqInfo->timingInfo, ue); 
            }
            ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
            pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz =
               rgSCHTomUtlFetchPcqiBitSz(ue, cell->numTxAntPorts, &ri); 
            if(0 == pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz)
            {
               RGSCHDBGERR ( cell->instIdx,(rgSchPBuf( cell->instIdx),
                      "rgSCHTomUtlFillPcqiRecpReq()"
                      " Unable to Fill CqiPmi size for ue %d \n", ue->ueId));
               MSPD_ERR (" Unable to Fill CqiPmi size for ue %d \n", ue->ueId);
               RETVALUE(RFAILED);
            }
            if(pucchRecpInfo->t.pucchRecpReq.uciInfo == TFU_PUCCH_HARQ_SR)
            {
               pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_CQI;
            }
            else
            {
               pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_CQI;
            }
         }
         rgSCHTomUtlMovePcqiNxtOccasion(cell, ue);           
      }
      if(ue->srsCb.nSrsTrIdx == validIdx)
      {
         if(ue->srsCb.srsDist ==0 )
         {
            if((pucchRecpInfo->t.pucchRecpReq.uciInfo != TFU_PUCCH_HARQ_CQI) 
              && (ue->srsCb.srsCfg.srsSetup.sANSrs))
            {
               /*Fill SRS params*/
               pucchRecpInfo->t.pucchRecpReq.srsInfo.srsBw = 
                  (TfuUlSrsBwInfo)ue->srsCb.srsCfg.srsSetup.srsBw;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.nRrc = 
                  ue->srsCb.srsCfg.srsSetup.fDomPosi;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.srsHopBw = 
                  (TfuUlSrsHoBwInfo)ue->srsCb.srsCfg.srsSetup.srsHopBw;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.transComb = 
                  ue->srsCb.srsCfg.srsSetup.txComb;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCfgIdx = 
                  ue->srsCb.srsCfg.srsSetup.srsCfgIdx;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCyclicShft = 
                  (TfuUlSrsCycShiftInfo)ue->srsCb.srsCfg.srsSetup.cycShift;
               /* ccpu00116923 - ADD - New Reception Request types for CQI and SRS with SR */
               switch(pucchRecpInfo->t.pucchRecpReq.uciInfo)
               {
                  case TFU_PUCCH_HARQ_SR:
                  pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_SRS;
                  break;
                  case TFU_PUCCH_HARQ_SR_CQI:
                  pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_CQI_SRS;
                  break;
                  default:
                  pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SRS;
                  break;
               }
            }
            rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
         }
         else
         {
            ue->srsCb.srsDist--; 
         }
      }
   }            

   RETVALUE(ROK);
}  /* rgSCHTomUtlFillCqiSrSrsWithHq */

/**
 * @brief  Function which handles the filling of PCQI/RI, SRS 
 *             Reception Request Information along with SR reception 
 *              Request 
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillCqiSrsWithSr
 *
 *              Function which handles the filling of PCQI/RI, SRS 
 *             Reception Request Information along
 *             with the SR reception Request
 *
 *
 *     Invoked by: rgSCHTomUtlFillSrRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for CQI/RI, SRS if they occur
 *       in the same instance as of SR. 
 *           
 *  @param[in]   RgSchCellCb       *cell,
 *               RgSchUeCb         *ue
 *               TfuRecpReqInfo    *recpReqInfo,
 *  @param[out]  TfuUeRecpReqInfo  *pucchRecpInfo
 *  @param[in]   U16               validIdx
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillCqiSrsWithSr
(
 RgSchCellCb       *cell,
 RgSchUeCb         *ue, 
 TfuRecpReqInfo    *recpReqInfo,
 TfuUeRecpReqInfo  *pucchRecpInfo,
 U16               validIdx
 )
#else
PRIVATE S16 rgSCHTomUtlFillCqiSrsWithSr(cell, ue, recpReqInfo,  
      pucchRecpInfo, validIdx)
RgSchCellCb       *cell;
RgSchUeCb         *ue; 
TfuRecpReqInfo    *recpReqInfo;
TfuUeRecpReqInfo  *pucchRecpInfo;
U16               validIdx;
#endif
{
   U8                 ri; /*To fetch RI value*/
   Bool               willUeRprtCqi;   /* Flag set due to CQI Mask and 
                                      UE Inactive state (DRX)*/
   TRC2(rgSCHTomUtlFillCqiSrsWithSr);
   
   rgSCHTomUtlWillUeRprtCqiRi(ue, &willUeRprtCqi);
   if(ue->cqiCb.nRiTrIdx == validIdx)
   {
         if(ue->cqiCb.riDist == 0)
         {
            if(willUeRprtCqi == TRUE)
            {
               /*Fill RI params*/
               pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx =
                  ue->cqiCb.cqiCfg.cqiSetup.cqiPResIdx; 
               pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz = 
                  ue->cqiCb.riNumBits; 
               pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR_CQI;
               ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
               rgSCHTomUtlFillRiBitWidthInfo(ue, TFU_RECP_REQ_PUCCH);
	       /* MSPD_TODO: syed Shouldn't this be done outside this if condition */
               if (ue->cqiCb.nCqiTrIdx == validIdx)
               {
                  rgSCHTomUtlMovePcqiNxtOccasion(cell, ue); 
               }       
            }
            rgSCHTomUtlMovePriNxtOccasion(cell, ue);
         }
         else
         {
            ue->cqiCb.riDist--;
         }
   }
      if(ue->cqiCb.nCqiTrIdx == validIdx)
      {
         if(willUeRprtCqi == TRUE)
         {
            /*Fill CQI Params*/
            pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx =
               ue->cqiCb.cqiCfg.cqiSetup.cqiPResIdx; 

            if(ue->cqiCb.cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP)
            {
               rgSCHTomUtlPcqiSbCalcBpIdx(recpReqInfo->timingInfo, ue); 
            }
            ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
            pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz =
               rgSCHTomUtlFetchPcqiBitSz(ue, cell->numTxAntPorts, &ri); 
            if(0 == pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz)
            {
               RGSCHDBGERR ( cell->instIdx,(rgSchPBuf( cell->instIdx),
                      "rgSCHTomUtlFillPcqiRecpReq()"
                      " Unable to Fill CqiPmi size for ue %d \n", ue->ueId));
               MSPD_ERR ("rgSCHTomUtlFillPcqiRecpReq()"
                      " Unable to Fill CqiPmi size for ue %d \n", ue->ueId);
               RETVALUE(RFAILED);
            }
            pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR_CQI;
         }
         rgSCHTomUtlMovePcqiNxtOccasion(cell, ue);  
      }
      if(ue->srsCb.nSrsTrIdx == validIdx)
      {
         if(ue->srsCb.srsDist ==0 )
         {
            if(ue->srsCb.srsCfg.srsSetup.sANSrs) 
            {
               /*Fill SRS params*/
               pucchRecpInfo->t.pucchRecpReq.srsInfo.srsBw = 
                  (TfuUlSrsBwInfo)ue->srsCb.srsCfg.srsSetup.srsBw;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.nRrc = 
                  ue->srsCb.srsCfg.srsSetup.fDomPosi;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.srsHopBw = 
                  (TfuUlSrsHoBwInfo)ue->srsCb.srsCfg.srsSetup.srsHopBw;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.transComb = 
                  ue->srsCb.srsCfg.srsSetup.txComb;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCfgIdx = 
                  ue->srsCb.srsCfg.srsSetup.srsCfgIdx;
               pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCyclicShft = 
                  (TfuUlSrsCycShiftInfo)ue->srsCb.srsCfg.srsSetup.cycShift;
               /* ccpu00116923 - ADD - New Reception Request types for CQI and
                * SRS with SR */
               if(pucchRecpInfo->t.pucchRecpReq.uciInfo == TFU_PUCCH_SR_CQI)
               {
                  pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR_CQI_SRS;
               }
               else
               {
                  pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR_SRS;
               }

            }
            rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
         }
         else
         {
            ue->srsCb.srsDist--; 
         }
      }
   RETVALUE(ROK);
}  /* rgSCHTomUtlFillCqiSrsWithSr */

/**
 * @brief Function which decodes Raw PCQI Info and fills the DLCqiRpt.
 *
 * @details
 *
 *     Function: rgSCHTomUtlDecodeRawPcqiPucchMode21
 *
 *     Function which decodes Raw PCQI Info and fills the DLCqiRpt structure.
 *
 *     Invoked by: rgSCHTomUtlDecodeRawPcqi 
 *
 *     Processing Steps:
 *     - Depending on the Raw PCQI Info stored during Reception Request
 *        decode the Raw PCQI Report 
 *     - Fill the DL CQI Report structure accordingly for the decoded Raw PCQI
 *           
 *  @param[in] U8*                    ri
 *  @param[in] U8*                    revArray
 *  @param[out] TfuCqiPucchMode21     *puMode21
 *  @param[out] TfuCqiPucchMode21     *mode21
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHTomUtlDecodeRawPcqiPucchMode21
( 
 U8*                 revArray,
 TfuCqiPucchMode21  *puMode21,
 TfuCqiPucchMode21    *mode21,
 U8*                   ri
 )
#else
PRIVATE Void rgSCHTomUtlDecodeRawPcqiPucchMode21(revArray,
               puMode21,mode21,ri)
U8*                 revArray,
TfuCqiPucchMode21  *puMode21;
TfuCqiPucchMode21    *mode21;
U8*                  ri;
#endif
{
   CONSTANT U8 leastSignBit = 0;
   TRC2(rgSCHTomUtlDecodeRawPcqiPucchMode21);

   if (mode21->type == TFU_RPT_CQI)
   {
      puMode21->type = TFU_RPT_CQI;
      if (mode21->u.cqi.isWideband)
      {
         puMode21->u.cqi.isWideband = TRUE; 
         puMode21->u.cqi.u.wideCqi.cqi = 
            (U8)rgSCHUtlParse(revArray, leastSignBit, 
            (U8)(leastSignBit+mode21->u.cqi.u.wideCqi.cqi), 
            (U8)TFU_MAX_CQI_BYTES);
         if (mode21->u.cqi.u.wideCqi.diffCqi.pres)
         {
             puMode21->u.cqi.u.wideCqi.diffCqi.pres = TRUE;
             puMode21->u.cqi.u.wideCqi.diffCqi.val = 
                rgSCHUtlParse(revArray,
                 (U8)(leastSignBit+mode21->u.cqi.u.wideCqi.cqi), 
                 (U8)(leastSignBit+(mode21->u.cqi.u.wideCqi.cqi+
                     mode21->u.cqi.u.wideCqi.diffCqi.val)),
                 (U8)TFU_MAX_CQI_BYTES);
             puMode21->u.cqi.u.wideCqi.pmi = 
             rgSCHUtlParse(revArray,
                 (U8)(leastSignBit+(mode21->u.cqi.u.wideCqi.cqi+
                         mode21->u.cqi.u.wideCqi.diffCqi.val)),
                 (U8)(leastSignBit+(mode21->u.cqi.u.wideCqi.cqi+
                    mode21->u.cqi.u.wideCqi.diffCqi.val+
                    mode21->u.cqi.u.wideCqi.pmi)), (U8)TFU_MAX_CQI_BYTES);
         }
         else
         {
             puMode21->u.cqi.u.wideCqi.diffCqi.pres = FALSE;
             puMode21->u.cqi.u.wideCqi.pmi = 
             rgSCHUtlParse(revArray,
                     (U8)(leastSignBit + mode21->u.cqi.u.wideCqi.cqi), 
                     (U8)(leastSignBit+(mode21->u.cqi.u.wideCqi.cqi+
                     mode21->u.cqi.u.wideCqi.pmi)),(U8)TFU_MAX_CQI_BYTES);
         }
      }
      else
      {
          puMode21->u.cqi.isWideband = FALSE; 
          puMode21->u.cqi.u.subCqi.cqi = 
                rgSCHUtlParse(revArray,
               leastSignBit, (U8)(leastSignBit+mode21->u.cqi.u.subCqi.cqi), 
                (U8)TFU_MAX_CQI_BYTES);
          if(mode21->u.cqi.u.subCqi.diffCqi.pres)
          {
              puMode21->u.cqi.u.subCqi.diffCqi.pres = TRUE;
              puMode21->u.cqi.u.subCqi.diffCqi.val = 
              rgSCHUtlParse(revArray,
                 (U8)(leastSignBit+mode21->u.cqi.u.subCqi.cqi), 
                 (U8)(leastSignBit+(mode21->u.cqi.u.subCqi.cqi+
                 mode21->u.cqi.u.subCqi.diffCqi.val)),(U8)TFU_MAX_CQI_BYTES);
              puMode21->u.cqi.u.subCqi.l = 
                 rgSCHUtlParse(revArray,
                 (U8)(leastSignBit+(mode21->u.cqi.u.subCqi.cqi+
                       mode21->u.cqi.u.subCqi.diffCqi.val)),
                       (U8)(leastSignBit+(mode21->u.cqi.u.subCqi.cqi+
                        mode21->u.cqi.u.subCqi.diffCqi.val+
                        mode21->u.cqi.u.subCqi.l)),(U8) TFU_MAX_CQI_BYTES);
          }
          else
          {
              puMode21->u.cqi.u.subCqi.diffCqi.pres = FALSE;
              puMode21->u.cqi.u.subCqi.l = 
                        rgSCHUtlParse(revArray,
                        (U8)(leastSignBit+(mode21->u.cqi.u.subCqi.cqi)),
                        (U8)(leastSignBit+(mode21->u.cqi.u.subCqi.cqi+
                        mode21->u.cqi.u.subCqi.l)), (U8)TFU_MAX_CQI_BYTES);
          }
      }
   }
   else if (mode21->type == TFU_RPT_RI)
   {
       puMode21->type = TFU_RPT_RI;
       puMode21->u.ri = (rgSCHUtlParse(ri,
                     leastSignBit, (U8)(leastSignBit+mode21->u.ri), 1)+1);
   }
   RETVOID;
}  /* rgSCHTomUtlDecodeRawPcqiPucchMode21 */

/**
 * @brief Function which decodes Raw ACQI Info and fills the DLCqiRpt.
 *
 * @details
 *
 *     Function: rgSCHTomUtlDecodeRawAcqiPuschMode31
 *
 *     Function which decodes Raw ACQI Info and fills the DLCqiRpt structure.
 *
 *     Invoked by: rgSCHTomUtlDecodeRawACqi
 *
 *     Processing Steps:
 *     - Depending on the Raw ACQI Info stored during Reception Request
 *        decode the Raw CQI Report 
 *     - Fill the DL CQI Report structure accordingly for the decoded Raw ACQI
 *           
 *  @param[in] RgSchUeCb       *ueCb
 *  @param[in] U8*             revArray
 *  @param[out] RgSchCqiRawPuschMode31     *mode31Info
 *  @param[in] TfuCqiPuschMode31     *psMode31
 *  @param[in] U8     ri
 *  @param[in] U32    numTxAnt
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode31
( 
 RgSchUeCb          *ueCb,
 U8*                  revArray,
 RgSchCqiRawPuschMode31   *mode31Info,
 TfuCqiPuschMode31   *psMode31,
 U8                  ri,
 U32                 numTxAnt
)
#else
PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode31(ueCb,revArray,
         mode31Info,psMode31,ri,numTxAnt)
RgSchUeCb          *ueCb;
U8*                  revArray,
RgSchCqiRawPuschMode31   *mode31Info;   
TfuCqiPuschMode31   *psMode31;   
U8                  ri;
U32                 numTxAnt;
#endif
{
   CONSTANT U8 leastSignBit = 0;
   U8 loop, sbOffst;

   TRC2(rgSCHTomUtlDecodeRawAcqiPuschMode31);
    
   psMode31->numSubband = ueCb->acqiCb.N;
   psMode31->wideBandCqi[0] = rgSCHUtlParse(revArray,
        leastSignBit, (U8)(leastSignBit+mode31Info->wideBCqiCw0), 
              TFU_MAX_CQI_BYTES);
   sbOffst = leastSignBit + mode31Info->wideBCqiCw0;         
   for(loop =0; loop<mode31Info->totLenSbDiffCqiCw0/2; loop++)
   {
       psMode31->subbandCqiArr[loop].cqiIdx[0] = 
       rgSCHUtlParse(revArray,sbOffst,
                        (U8)(sbOffst+2), (U8)TFU_MAX_CQI_BYTES);
       sbOffst+=2;
       psMode31->subbandCqiArr[loop].subband.numRb = ueCb->acqiCb.k;
       psMode31->subbandCqiArr[loop].subband.rbStart = 
                  ueCb->acqiCb.k*loop;
   }
   if(ri ==1)
   {
       psMode31->wideBandCqi[1] = 0;
       for(loop =0; loop<mode31Info->totLenSbDiffCqiCw0/2; loop++)
       {
           psMode31->subbandCqiArr[loop].cqiIdx[1] = 0;
       }
       psMode31->pmi = rgSCHUtlParse(revArray,
        (U8)(leastSignBit+(mode31Info->wideBCqiCw0+
                mode31Info->totLenSbDiffCqiCw0)),
        (U8)(leastSignBit+(mode31Info->wideBCqiCw0+
                mode31Info->totLenSbDiffCqiCw0+
                       mode31Info->r1PmiBitLen)), TFU_MAX_CQI_BYTES);
   }
   else if (ri > 1)
   {
       psMode31->wideBandCqi[1] = rgSCHUtlParse(revArray,
        (U8)(leastSignBit+(mode31Info->wideBCqiCw0+
                        mode31Info->totLenSbDiffCqiCw0)), 
        (U8)(leastSignBit+(mode31Info->wideBCqiCw0+
                mode31Info->totLenSbDiffCqiCw0+mode31Info->rg1WideBCqiCw1)), 
                           TFU_MAX_CQI_BYTES);
      sbOffst = leastSignBit + (mode31Info->wideBCqiCw0+
                mode31Info->totLenSbDiffCqiCw0+ mode31Info->rg1WideBCqiCw1); 
      for(loop =0; loop<mode31Info->rg1TotLenSbDiffCqiCw1/2; loop++)
      {
          psMode31->subbandCqiArr[loop].cqiIdx[1] = 
          rgSCHUtlParse(revArray,sbOffst,
                           (U8)(sbOffst+2), (U8)TFU_MAX_CQI_BYTES);
          sbOffst+=2;
      }
      psMode31->pmi = rgSCHUtlParse(revArray,
            (U8)(leastSignBit+(mode31Info->wideBCqiCw0+
                mode31Info->totLenSbDiffCqiCw0+mode31Info->rg1WideBCqiCw1 + 
                mode31Info->rg1TotLenSbDiffCqiCw1)),
            (U8)(leastSignBit+(mode31Info->wideBCqiCw0+
                    mode31Info->totLenSbDiffCqiCw0+mode31Info->rg1WideBCqiCw1 + 
                    mode31Info->rg1TotLenSbDiffCqiCw1+mode31Info->rg1PmiBitLen)), 
                 TFU_MAX_CQI_BYTES);
   }
   RETVOID;
}  /* rgSCHTomUtlDecodeRawAcqiPuschMode31 */

/**
 * @brief Function which decodes Raw ACQI Info and fills the DLCqiRpt.
 *
 * @details
 *
 *     Function: rgSCHTomUtlDecodeRawAcqiPuschMode22
 *
 *     Function which decodes Raw ACQI Info and fills the DLCqiRpt structure.
 *
 *     Invoked by: rgSCHTomUtlDecodeRawACqi
 *
 *     Processing Steps:
 *     - Depending on the Raw ACQI Info stored during Reception Request
 *        decode the Raw CQI Report 
 *     - Fill the DL CQI Report structure accordingly for the decoded Raw ACQI
 *           
 *  @param[in] RgSchUeCb       *ueCb
 *  @param[in] U8*             revArray
 *  @param[out] RgSchCqiRawPuschMode22     *mode22Info
 *  @param[in] TfuCqiPuschMode22     *psMode22
 *  @param[in] U8     ri
 *  @param[in] U32    numTxAnt
 *  @return  Void
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode22
( 
 RgSchUeCb          *ueCb,
 U8*                  revArray,
 RgSchCqiRawPuschMode22   *mode22Info,
 TfuCqiPuschMode22   *psMode22,
 U8                  ri,
 U32                 numTxAnt
)
#else
PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode22(ueCb,revArray,
         mode22Info,psMode22,ri,numTxAnt)
RgSchUeCb          *ueCb;
U8*                  revArray,
RgSchCqiRawPuschMode22   *mode22Info;   
TfuCqiPuschMode22   *psMode22;   
U8                  ri;
U32                 numTxAnt;
#endif
{
   CONSTANT U8 leastSignBit = 0;
   U32 label =0;

   TRC2(rgSCHTomUtlDecodeRawAcqiPuschMode22);

   psMode22->numSubband = ueCb->acqiCb.M;
   psMode22->wideBandCqi[0]=rgSCHUtlParse(revArray,
        leastSignBit, (U8)(leastSignBit+mode22Info->wideBCqiCw0), 
                   (U8)TFU_MAX_CQI_BYTES);
   psMode22->cqi[0]=rgSCHUtlParse(revArray,
        (U8)(leastSignBit+mode22Info->wideBCqiCw0), 
        (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
            mode22Info->sBDiffCqiCw0)) , TFU_MAX_CQI_BYTES);
    psMode22->numSubband = ueCb->acqiCb.N;
    if(ri ==1)
    {
        psMode22->wideBandCqi[1] = 0;
        psMode22->cqi[1] = 0;
        /* Extract and Store Position of M Selected SubBands*/
        label =   rgSCHUtlParse(revArray,\
         (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                 mode22Info->sBDiffCqiCw0)),(U8)(leastSignBit+
                 (mode22Info->wideBCqiCw0 + mode22Info->sBDiffCqiCw0 + 
                   mode22Info->posOfM)),TFU_MAX_CQI_BYTES); 
        /*Arg-5 has been not been casted to TfuSubbandInfo as this is  
                * defined only under MIMO */
/* MS_FIX: tfu upgrade changes */
#ifndef TFU_UPGRADE
        rgSCHTomUtlGenIndices(label, ueCb->acqiCb.M, 
                     ueCb->acqiCb.N, ueCb->acqiCb.k,psMode22->subbandArr);
#endif /* TFU_UPGRADE */
        psMode22->wideBandPmi =  rgSCHUtlParse(revArray,
                (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                        mode22Info->sBDiffCqiCw0 + mode22Info->posOfM)),
                (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                        mode22Info->sBDiffCqiCw0 + mode22Info->posOfM + 
                        mode22Info->r1PmiBitLen)),TFU_MAX_CQI_BYTES);
        psMode22->pmi = psMode22->wideBandPmi;
    }
    else
    {
        psMode22->wideBandCqi[1] =  rgSCHUtlParse(revArray,
        (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                mode22Info->sBDiffCqiCw0)),
        (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                mode22Info->sBDiffCqiCw0 + mode22Info->rg1WideBCqiCw1)), 
                    TFU_MAX_CQI_BYTES);
        psMode22->cqi[1] = rgSCHUtlParse(revArray,
        (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + mode22Info->sBDiffCqiCw0 +
              mode22Info->rg1WideBCqiCw1)), (U8)(leastSignBit+
              (mode22Info->wideBCqiCw0+mode22Info->sBDiffCqiCw0 +
               mode22Info->rg1WideBCqiCw1 + mode22Info->rg1SbDiffCqiCw1)), 
                          TFU_MAX_CQI_BYTES);
               /*Extract and Store Position of M Selected SubBands */
        label =  rgSCHUtlParse(revArray,\
                (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                        mode22Info->sBDiffCqiCw0+mode22Info->rg1WideBCqiCw1 +
                                mode22Info->rg1SbDiffCqiCw1)), 
                (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                        mode22Info->sBDiffCqiCw0+mode22Info->rg1WideBCqiCw1 +
                        mode22Info->rg1SbDiffCqiCw1+ mode22Info->posOfM)),
                        TFU_MAX_CQI_BYTES); 
        rgSCHTomUtlGenIndices(label, ueCb->acqiCb.M, 
                     ueCb->acqiCb.N, ueCb->acqiCb.k,
                     (TfuSubbandInfo * )psMode22->subbandArr);
        psMode22->wideBandPmi = rgSCHUtlParse(revArray,
            (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                mode22Info->sBDiffCqiCw0+mode22Info->rg1WideBCqiCw1 +
                mode22Info->rg1SbDiffCqiCw1+mode22Info->posOfM)),
            (U8)(leastSignBit+(mode22Info->wideBCqiCw0 + 
                 mode22Info->sBDiffCqiCw0+
                 mode22Info->rg1WideBCqiCw1+mode22Info->rg1SbDiffCqiCw1 +
                 mode22Info->posOfM + mode22Info->rg1PmiBitLen)),
                 TFU_MAX_CQI_BYTES);
        psMode22->pmi = psMode22->wideBandPmi;
    }
    RETVOID;
}  /* rgSCHTomUtlDecodeRawAcqiPuschMode22 */

/**
 * @brief Function which decodes Raw ACQI Info and fills the DLCqiRpt.
 *
 * @details
 *
 *     Function: rgSCHTomUtlDecodeRawAcqiPuschMode12
 *
 *     Function which decodes Raw ACQI Info and fills the DLCqiRpt structure.
 *
 *     Invoked by: rgSCHTomUtlDecodeRawACqi
 *
 *     Processing Steps:
 *     - Depending on the Raw ACQI Info stored during Reception Request
 *        decode the Raw CQI Report 
 *     - Fill the DL CQI Report structure accordingly for the decoded Raw ACQI
 *           
 *  @param[in] RgSchUeCb       *ueCb
 *  @param[in] U8*             revArray
 *  @param[out] RgSchCqiRawPuschMode12     *mode12Info
 *  @param[in] TfuCqiPuschMode12     *psMode12
 *  @param[in] U8     ri
 *  @param[in] U32    numTxAnt
 *  @return  Void
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode12
( 
 RgSchUeCb          *ueCb,
 U8*                  revArray,
 RgSchCqiRawPuschMode12   *mode12Info,
 TfuCqiPuschMode12   *psMode12,
 U8                  ri,
 U32                 numTxAnt
)
#else
PRIVATE Void rgSCHTomUtlDecodeRawAcqiPuschMode12(ueCb,revArray,
         mode12Info,psMode12,ri,numTxAnt)
RgSchUeCb          *ueCb;
U8*                  revArray,
RgSchCqiRawPuschMode12   *mode12Info;   
TfuCqiPuschMode12   *psMode12;   
U8                  ri;
U32                 numTxAnt;
#endif
{
   CONSTANT U8 leastSignBit = 0;
   U8 sbOffst;
   U8 bitLen = 0;

   TRC2(rgSCHTomUtlDecodeRawAcqiPuschMode12);
   
   if(ri ==1)
   {
      psMode12->cqiIdx[0] = rgSCHUtlParse(revArray,
            leastSignBit,(U8)(leastSignBit+mode12Info->wideBCqiCw0), 
                 TFU_MAX_CQI_BYTES);
      psMode12->cqiIdx[1] = 0;
      psMode12->numSubband =ueCb->acqiCb.N;
      sbOffst = leastSignBit+mode12Info->wideBCqiCw0;
      if(numTxAnt ==2)
      {
         bitLen = 2;
         RG_SCH_PARSE_MULTI_PMI(bitLen, mode12Info->r1TotalPmiBitLen,
                        psMode12, ueCb, revArray, sbOffst);
      }
      else if(numTxAnt ==4)
      {
          bitLen = 4;
          RG_SCH_PARSE_MULTI_PMI(bitLen, mode12Info->r1TotalPmiBitLen,
                        psMode12, ueCb, revArray, sbOffst);
      }                  
   }
   else
   {
      psMode12->cqiIdx[0] = rgSCHUtlParse(revArray,
            leastSignBit,(U8)(leastSignBit+mode12Info->wideBCqiCw0), 
                           TFU_MAX_CQI_BYTES);
      psMode12->cqiIdx[1] = rgSCHUtlParse(revArray,
            (U8)(leastSignBit+mode12Info->wideBCqiCw0),(U8)(leastSignBit+
                (mode12Info->wideBCqiCw0+mode12Info->rg1WideBCqiCw1)), 
                           TFU_MAX_CQI_BYTES);
      psMode12->numSubband =ueCb->acqiCb.N;
      sbOffst = leastSignBit+(mode12Info->wideBCqiCw0+
                     mode12Info->rg1WideBCqiCw1);
      if(numTxAnt ==2)
      {
          bitLen = 1;
          RG_SCH_PARSE_MULTI_PMI(bitLen, mode12Info->rg1TotalPmiBitLen,
                        psMode12, ueCb, revArray, sbOffst);
      }
      else if(numTxAnt ==4)
      {
          bitLen = 4;
          RG_SCH_PARSE_MULTI_PMI(bitLen, mode12Info->rg1TotalPmiBitLen,
                        psMode12, ueCb, revArray, sbOffst);
      }       
   }
   
   RETVOID;
}/*rgSCHTomUtlDecodeRawAcqiPuschMode12*/


/***********************************************************
 *
 *     Func : rgSCHTomUtlFetchPcqiBitSzPucchMode21
 *
 *
 *     Desc : Fetch the CQI/PMI bits for a UE with Mode 21 configured
 *
 *     Ret  : U8
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC U8 rgSCHTomUtlFetchPcqiBitSzPucchMode21
(
 RgSchUeCb    *ueCb,
 TfuCqiPucchMode21   *mode21Info,
 U8           numTxAnt,
 U8           *ri
)
#else
PUBLIC U8 rgSCHTomUtlFetchPcqiBitSzPucchMode21(ueCb, mode21Info, numTxAnt, ri)
   RgSchUeCb     *ueCb;
   TfuCqiPucchMode21   *mode21Info;
   U8            numTxAnt;
   U8            *ri;
#endif
{
   U8 pcqiSz;
   
   TRC3(rgSCHTomUtlFetchPcqiBitSzPucchMode21);
   
   if(ueCb->cqiCb.isWb)
   {
       mode21Info->u.cqi.isWideband = TRUE;
       if(numTxAnt == 2)
       {
          if (*ri ==1)
          {
              pcqiSz = 6;
              mode21Info->u.cqi.u.wideCqi.cqi = 4;
              mode21Info->u.cqi.u.wideCqi.diffCqi.pres = FALSE;
              mode21Info->u.cqi.u.wideCqi.pmi = 2;
          }
          else
          {
              pcqiSz = 8;
              mode21Info->u.cqi.u.wideCqi.cqi = 4;
              mode21Info->u.cqi.u.wideCqi.diffCqi.pres = TRUE;
              mode21Info->u.cqi.u.wideCqi.diffCqi.val  = 3;
              mode21Info->u.cqi.u.wideCqi.pmi = 1;
          }
       }
       else if(numTxAnt == 4)
       {
          if (*ri ==1)
          {
              pcqiSz = 8;
              mode21Info->u.cqi.u.wideCqi.cqi = 4;
              mode21Info->u.cqi.u.wideCqi.diffCqi.pres = FALSE;
              mode21Info->u.cqi.u.wideCqi.pmi = 4;
          }
          else
          {
              pcqiSz = 11;
              mode21Info->u.cqi.u.wideCqi.cqi = 4;
              mode21Info->u.cqi.u.wideCqi.diffCqi.pres  = TRUE;
              mode21Info->u.cqi.u.wideCqi.diffCqi.val =3;
              mode21Info->u.cqi.u.wideCqi.pmi = 4;
          }
       }
       else
       {
          /* This might be number of antenna case 1.
           * For mode 2-1 wideband case only antenna port 2 or 4 is supported.
           * So setting invalid value.*/
          pcqiSz = 0;
       }
    }
    else
    {
       mode21Info->u.cqi.isWideband = FALSE;
       if (*ri ==1)
       {
           pcqiSz = 4 + ueCb->cqiCb.label;
           mode21Info->u.cqi.u.subCqi.cqi = 4;
           mode21Info->u.cqi.u.subCqi.diffCqi.pres = FALSE;
           mode21Info->u.cqi.u.subCqi.l = ueCb->cqiCb.label;
       }
       else
       {
           pcqiSz = 7 + ueCb->cqiCb.label;
           mode21Info->u.cqi.u.subCqi.cqi = 4;
           mode21Info->u.cqi.u.subCqi.diffCqi.pres = TRUE; 
           mode21Info->u.cqi.u.subCqi.diffCqi.val = 3;
           mode21Info->u.cqi.u.subCqi.l = ueCb->cqiCb.label;
       }
    }
    
    RETVALUE(pcqiSz);
}/*rgSCHTomUtlFetchPcqiBitSzPucchMode21*/

#endif 


#ifdef LTE_TDD
/** @brief This function handles filling of HARQ feedback repetition
 * recption request for each subframe
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillSfRepHqFdbk 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo  *recpReqInfo
 * @param  [in]  RgSchCellCb     *cell
 * @param  [out] RgSchErrInfo    *err
 * @param  [in]  RgSchDlSf       *dlSf
 * @param  [in]  U8              noFdbks
 * @param  [in]  CmMemListCp     *memCp
 * @param  [in]  U8              elemIdx
 * @param  [in]  RgSchDlSf       *nxtDlsf
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillSfRepHqFdbk
(
 TfuRecpReqInfo          *recpReqInfo,
 RgSchCellCb             *cellCb,
 RgSchErrInfo            *err,
 RgSchDlSf               *dlSf,
 U8                      noFdbks,
 CmMemListCp             *memCp,
 U8                      elemIdx,
 RgSchDlSf               *nxtDlsf,
 U16                     validIdx
 )
#else
PRIVATE S16 rgSCHTomUtlFillSfRepHqFdbk (recpReqInfo, cellCb, err, dlSf, 
noFdbks, memCp, elemIdx, nxtDlsf, validIdx)
   TfuRecpReqInfo          *recpReqInfo;
   RgSchCellCb             *cellCb;
   RgSchErrInfo            *err;
   RgSchDlSf               *dlSf;
   U8                      noFdbks;
   CmMemListCp             *memCp;
   U8                      elemIdx;
   RgSchDlSf               *nxtDlsf;
   U16                     validIdx; 
#endif
#else
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillSfRepHqFdbk
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cellCb,
RgSchErrInfo            *err,
RgSchDlSf               *dlSf,
U8                      noFdbks,
CmMemListCp             *memCp,
U8                      elemIdx,
RgSchDlSf               *nxtDlsf
)
#else
PRIVATE S16 rgSCHTomUtlFillSfRepHqFdbk (recpReqInfo, cellCb, err, dlSf, 
noFdbks, memCp, elemIdx, nxtDlsf)
   TfuRecpReqInfo          *recpReqInfo;
   RgSchCellCb             *cellCb;
   RgSchErrInfo            *err;
   RgSchDlSf               *dlSf;
   U8                      noFdbks;
   CmMemListCp             *memCp;
   U8                      elemIdx;
   RgSchDlSf               *nxtDlsf;
#endif
#endif 
{
   RgSchDlHqProcCb         *hqCb;
   CmLList                 *node;
   S16                     ret;
   RgSchUeCb               *ueCb;
   TfuUeRecpReqInfo        *pucchRecpInfo;
#ifdef TFU_UPGRADE
   TfuUePucchHqRecpInfo    *hqRecpReq;  
#endif
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchDlHqTbCb           *tbCb;
   RgSchDlHqProcCb         *prvHqCb = NULLP;

   TRC2(rgSCHTomUtlFillSfRepHqFdbk)

   node =  dlSf->ackNakRepQ.first;
   while (node)
   {
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      tbCb = (RgSchDlHqTbCb *)(node->node);
      hqCb = tbCb->hqP;
      ueCb = hqCb->hqE->ue;

      if (--tbCb->fbkRecpRepCntr)
      {            
         /* Add to next subfarme */
         /* Add this hqCb to the next dlSf's ackNakRepQ */
         cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
               &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
         tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
         tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
      }

      /* rg002.301 - Add - [ccpu00119456]- Do not proceed if PUSCH
         reception req is already filled*/
#ifdef TFU_UPGRADE
      /*rg002.301 -Add - [ccpu00120233] - TB Count; DEL - rcptDone flag*/
      if (hqCb->tbCnt) 
      {
         hqCb->tbCnt--;
         /* Go to the next node */
         node = node->next;
         continue;
      }
#endif
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      if ((hqCb->hqE->ue != NULLP) &&
            (hqCb->hqE->ue->measGapCb.isMeasuring != TRUE)
            && (hqCb != prvHqCb)
         )
      {
         /* We need to add the recp request to be sent on the pucchANRep
          * value.
          */
          /* rg009.201. Added changes of TFU_UPGRADE */
         ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                     sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
         if (ret != ROK)            
         {
            RGSCHDBGERR (cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
             "rgSCHTomUtlFillSfRepHqFdbk() Unable to Allocate TfuUeRecpReqInfo"
             "for cellId=%d \n", cellCb->cellId));
            MSPD_ERR ("Unable to Allocate TfuUeRecpReqInfo"
             "for cellId=%d \n", cellCb->cellId);
            err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
            RETVALUE(ret);
         }
         pucchRecpInfo->rnti    =  ueCb->ueId;
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
         pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
#else
         pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
#endif

         /* FOR repetition Feedback shall come on n1PucchAnRep Configured per
          * UE.
          */
/* rg009.201. Added changes of TFU_UPGRADE */         
#ifndef TFU_UPGRADE
         pucchRecpInfo->t.pucchRecpReq.hqType = TFU_HQ_RECP_REQ_N1PUCCH;
         pucchRecpInfo->t.pucchRecpReq.t.n1Pucch = ueCb->ackNakRepCb.pucchRes;
#else
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
         hqRecpReq = &(pucchRecpInfo->t.pucchRecpReq.hqInfo);
         /* ACK NACK rep works only in bundling mode . */
         hqRecpReq->hqFdbkMode = (TfuAckNackMode)RGR_TDD_ACKNACK_MODE_BUNDL;
         if ((hqCb->tbInfo[0].sfLnk.node != NULLP) && 
               (hqCb->tbInfo[1].sfLnk.node != NULLP))
         {

            hqRecpReq->hqSz = 2;
         }
         else
         {
            hqRecpReq->hqSz = 1;
         }
         hqRecpReq->pucchResCnt = 1; 
         hqRecpReq->hqRes[0] = ueCb->ackNakRepCb.pucchRes; 
#endif
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
         pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
      }
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* In a given dlSf, if there is 2 TBs context 
       * stored for a given harq, then they are added
       * adjacent to each other in the subframe. To avoid
       * adding duplicate recpnInfo for each TB, store this
       * hqCb in prvHqCb. If nextHqCb is same as prvHqCb then
       * do not add reception req info.*/
      prvHqCb = hqCb;
#ifdef TFU_UPGRADE
      rgSCHTomUtlMoveNxtOccasion(cellCb, hqCb->hqE->ue, validIdx);
#endif 
      /* Go to the next node */
      node = node->next;
   }

   RETVALUE(ROK);
}

/** @brief This function handles filling of HARQ feedback recption request
 * for each subframe 
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillSfHqFdbk 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo  *recpReqInfo
 * @param  [in]  RgSchCellCb     *cell
 * @param  [out] RgSchErrInfo    *err
 * @param  [in]  RgSchDlSf       *dlSf
 * @param  [in]  U8              noFdbks
 * @param  [in]  CmMemListCp     *memCp
 * @param  [in]  U8              elemIdx
 * @param  [in]  RgSchDlSf       *nxtDlsf
*  @param  [in]  U16             validIdx; 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef TFU_UPGRADE
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillSfHqFdbk
(
 TfuRecpReqInfo          *recpReqInfo,
 RgSchCellCb             *cellCb,
 RgSchErrInfo            *err,
 RgSchDlSf               *dlSf,
 U8                      noFdbks,
 CmMemListCp             *memCp,
 U8                      elemIdx,
 RgSchDlSf               *nxtDlsf,
 U16                     validIdx
 )
#else
PRIVATE S16 rgSCHTomUtlFillSfHqFdbk (recpReqInfo, cellCb, err, dlSf, 
      noFdbks, memCp, elemIdx, nxtDlsf, validIdx)
TfuRecpReqInfo          *recpReqInfo;
RgSchCellCb             *cellCb;
RgSchErrInfo            *err;
RgSchDlSf               *dlSf;
U8                      noFdbks;
CmMemListCp             *memCp;
U8                      elemIdx;
RgSchDlSf               *nxtDlsf;
U16                     validIdx; 
#endif
#else
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillSfHqFdbk
(
 TfuRecpReqInfo          *recpReqInfo,
 RgSchCellCb             *cellCb,
 RgSchErrInfo            *err,
 RgSchDlSf               *dlSf,
 U8                      noFdbks,
 CmMemListCp             *memCp,
 U8                      elemIdx,
 RgSchDlSf               *nxtDlsf
 )
#else
PRIVATE S16 rgSCHTomUtlFillSfHqFdbk (recpReqInfo, cellCb, err, dlSf, 
      noFdbks, memCp, elemIdx, nxtDlsf)
TfuRecpReqInfo          *recpReqInfo;
RgSchCellCb             *cellCb;
RgSchErrInfo            *err;
RgSchDlSf               *dlSf;
U8                      noFdbks;
CmMemListCp             *memCp;
U8                      elemIdx;
RgSchDlSf               *nxtDlsf;
#endif
#endif
{
   CmLList                 *node;
   RgSchDlHqProcCb         *hqCb;
   CmLteTimingInfo         futTime;
   S16                     ret;
   RgSchUeCb               *ueCb;
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
   RgSchTddANInfo          *anInfo;
#else
#ifdef LTEMAC_SPS
   RgSchUeCb               *ue;
#endif 
#endif
   RgSchUePucchRecpInfo    *pucchInfo = NULLP;
   Bool                    alloc = FALSE;
   RgrTddAckNackMode       ackNackMode;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchDlHqTbCb           *tbCb;
   RgSchDlHqProcCb         *prvHqCb = NULLP;
   CmLteRnti         rnti;
   /* rg002.301 - Add - [ccpu00119456] Correcting HqSz filling */
   U8            hqSz = 0;

   TRC2(rgSCHTomUtlFillSfHqFdbk)

      RGSCHADDTOCRNTTIME(cellCb->crntTime, futTime, TFU_DELTA)

   node =  dlSf->tbs.first;
   while (node)
   {
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      tbCb = (RgSchDlHqTbCb *)(node->node);
      hqCb = tbCb->hqP;
      ueCb = hqCb->hqE->ue;
      if(ueCb != NULLP)
      {
         rnti = ueCb->ueId;
         ackNackMode = ueCb->dl.ackNackMode;
#ifndef TFU_UPGRADE
         if(ackNackMode == RGR_TDD_ACKNACK_MODE_BUNDL)
         {
            anInfo = rgSCHUtlGetUeANFdbkInfo(ueCb, &futTime);
            /* Only the last scheduled TB for the UE is for HARQ
             * ACK/NACK reception in Bundling case */
            if((anInfo == NULLP) || 
                  (anInfo->latestMIdx != dlSf->dlFdbkInfo.m))
            {
               node = node->next;
               continue;
            }
         }
         else
         {
            /* Get the TFU reception request pointer, if present */
            cmHashListFind(&cellCb->ueTfuPendLst, (U8*) &ueCb->ueId,
                  sizeof(ueCb->ueId), 0, (PTR *) &pucchInfo);
         }
#else
         /* For upgrade we shall use the existing logic of pending list. */
         cmHashListFind(&cellCb->ueTfuPendLst, (U8*) &ueCb->ueId,
               sizeof(ueCb->ueId), 0, (PTR *) &pucchInfo);
#endif
      }
      else if(hqCb->hqE->raCb != NULLP)
      {
         /* For RACH it is set to Bundling */
         ackNackMode = RGR_TDD_ACKNACK_MODE_BUNDL;
         rnti = hqCb->hqE->raCb->tmpCrnti;
      }
      else
      {
         RGSCHDBGERR (cellCb->instIdx,
               (rgSchPBuf(cellCb->instIdx),"rgSCHTomUtlFillSfHqFdbk()"
                "Invalid HARQ process for cellId=%d \n", cellCb->cellId));
         MSPD_ERR ("Invalid HARQ process for cellId=%d \n", cellCb->cellId);
         node = node->next;
         continue;
      }

      /* rg002.301 - Add - [ccpu00119456/ccpu00119455] - do not proceed if PUSCH
         reception req is already filled*/
#ifdef TFU_UPGRADE
      /*rg002.301 -Add - [ccpu00120233] - TB Count; DEL - rcptDone flag*/
      if (hqCb->tbCnt) 
      {
         hqCb->tbCnt--;
         /* Go to the next node */
         node = node->next;
         continue;
      }
#endif
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      if(((ueCb == NULLP) || (ueCb->measGapCb.isMeasuring != TRUE))
            &&(hqCb != prvHqCb)
        )
      {
         TknU16       n1PucchTkn = {FALSE, 0};
         RgSchPdcch   *pdcch;
         /* rg002.301 - Add - [ccpu00119456/ccpu00119458] - Correcting HqSz filling */
         U8            tbIndx;
         pdcch = tbCb->pdcch;
#ifdef LTEMAC_SPS
         n1PucchTkn = hqCb->spsN1PucchRes;
#endif
         /* rg002.301 - Add - [ccpu00119456/ccpu00119458] - Correcting HqSz
            filling */
         for (tbIndx = 0; tbIndx < TFU_MAX_TB; tbIndx++)
         {
            if (hqCb->tbInfo[tbIndx].state == HQ_TB_WAITING && 
                  (RGSCH_TIMEINFO_SAME(hqCb->tbInfo[tbIndx].fdbkTime,
                      recpReqInfo->timingInfo)))
            {
               hqSz++;
            }
         }

         ret = rgSCHTomUtlFillSfHqFdbkForOneUe(recpReqInfo, cellCb, err, dlSf, noFdbks,
               memCp, elemIdx, nxtDlsf, rnti, ackNackMode, &pucchInfo, pdcch,
               n1PucchTkn, &alloc, hqSz);
         if (ret != ROK)
         {
            RETVALUE(ret);
         }
/* rg009.201. Added changes of TFU_UPGRADE */         
#ifdef TFU_UPGRADE
               rgSCHTomUtlFillCqiSrSrsWithHq(cellCb,recpReqInfo, hqCb->hqE->ue, 
                     pucchInfo->pucchRecpInfo, validIdx);  
#endif 

         /* TODO antz - pushing the following code (under TFU_UPGRADE)
          * into the above function (...ForOneUe) did not work (caused
          * two additional TCs to fail). Don't know why. If this
          * is done later, make sure that the code branch
          * for relPdcch (later in this func) is also modified appropriately.
          */
         /* Now add to the recp request or pending list */
 /* rg009.201. Added changes of TFU_UPGRADE */              
#ifdef TFU_UPGRADE
         if((elemIdx != (noFdbks - 1)))
         {
            cmHashListInsert(&cellCb->ueTfuPendLst, (PTR) pucchInfo,
                  (U8 *)&rnti ,(U16) sizeof(CmLteRnti));
            alloc = FALSE;
         }
         else
         {
            /*rg003.301 [ccpu00120751]-MOD-avoid overriding latest uci value */
            cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, 
                  &(pucchInfo->pucchRecpInfo->lnk));
            pucchInfo->pucchRecpInfo->lnk.node = 
               (PTR)pucchInfo->pucchRecpInfo;
            /* Delete the entry after addition to the list */
            cmHashListDelete(&cellCb->ueTfuPendLst, (PTR) pucchInfo);
         }
#endif

      } /* If measuring */
      /* Go to the next node */
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      if ((tbCb->fbkRecpRepCntr) && (--tbCb->fbkRecpRepCntr))
      {
         /* Add to next subfarme */
         /* Add this hqCb to the next dlSf's ackNakRepQ */
         cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
               &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
         tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
         tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
      }
      /* In a given dlSf, if there is 2 TBs context 
       * stored for a given harq, then they are added
       * adjacent to each other in the subframe. To avoid
       * adding duplicate recpnInfo for each TB, store this
       * hqCb in prvHqCb. If nextHqCb is same as prvHqCb then
       * do not add reception req info.*/
      prvHqCb = hqCb;
      node = node->next;
      pucchInfo = NULLP;
   } /* end of while */
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   if (dlSf->relPdcch)
   {
      TknU16       n1PucchTkn = {FALSE, 0};
      /* TODO antz - check if rnti on pdcch is SPS or regular. If sps,
       * the regular one may have to be added to pdcch. */
      /* rg002.301 - Add - [ccpu00119456/ccpu00119458] - Correcting HqSz
         filling */
      ret = rgSCHTomUtlFillSfHqFdbkForOneUe(recpReqInfo, cellCb, err, dlSf, noFdbks,
            memCp, elemIdx, nxtDlsf, dlSf->relPdcchInfo.crnti,
            dlSf->relPdcchInfo.ackNackMode, &pucchInfo, dlSf->relPdcch,
            n1PucchTkn, &alloc, hqSz);
      if (ret != ROK)
      {
         RETVALUE(ret);
      }
#ifdef TFU_UPGRADE
       ue = rgSCHDbmGetUeCb (cellCb, dlSf->relPdcch->rnti);
     rgSCHTomUtlFillCqiSrSrsWithHq(cellCb,recpReqInfo, ue, 
            pucchInfo->pucchRecpInfo, validIdx); 
#endif 
      /* Now add to the recp request or pending list */
#ifdef TFU_UPGRADE
      if((elemIdx != (noFdbks - 1)))
      {
         cmHashListInsert(&cellCb->ueTfuPendLst, (PTR) pucchInfo,
               (U8 *)&dlSf->relPdcch->rnti ,(U16) sizeof(CmLteRnti));
         alloc = FALSE;
      }
      else
      {
         pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, 
               &(pucchInfo->pucchRecpInfo->lnk));
         pucchInfo->pucchRecpInfo->lnk.node = 
            (PTR)pucchInfo->pucchRecpInfo;
         /* Delete the entry after addition to the list */
         cmHashListDelete(&cellCb->ueTfuPendLst, (PTR) pucchInfo);
      }
#endif
   }
#endif /* LTEMAC_SPS */
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHTomUtlFillSfHqFdbkForOneUe
 *
 *     Desc : Fill HARQ feedback info for one UE/entry
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlFillSfHqFdbkForOneUe
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cellCb,
RgSchErrInfo            *err,
RgSchDlSf               *dlSf,
U8                      noFdbks,
CmMemListCp             *memCp,
U8                      elemIdx,
RgSchDlSf               *nxtDlsf,
CmLteRnti               rnti,
RgrTddAckNackMode       ackNackMode,
RgSchUePucchRecpInfo    **pucchInfoRef,
RgSchPdcch              *pdcch,
TknU16                  n1PucchTkn,
Bool                    *allocRef,
U8                      hqSz /* rg002.301 Add - [ccpu00119456/ccpu00119458] - 
                                -Correcting HqSz filling */
)
#else
PRIVATE S16 rgSCHTomUtlFillSfHqFdbkForOneUe(recpReqInfo, cellCb, err, dlSf,
      noFdbks, memCp, elemIdx, nxtDlsf, rnti, ackNackMode, pucchInfoRef,
      pdcch, n1PucchTkn, allocRef, hqSz)
TfuRecpReqInfo          *recpReqInfo;
RgSchCellCb             *cellCb;
RgSchErrInfo            *err;
RgSchDlSf               *dlSf;
U8                      noFdbks;
CmMemListCp             *memCp;
U8                      elemIdx;
RgSchDlSf               *nxtDlsf;
CmLteRnti               rnti;
RgrTddAckNackMode       ackNackMode;
RgSchUePucchRecpInfo    **pucchInfoRef;
RgSchPdcch              *pdcch;
TknU16                  n1PucchTkn;
Bool                    *allocRef;
U8                      hqSz;
#endif
{
   RgSchUePucchRecpInfo    *pucchInfo = *pucchInfoRef;
   Bool                    alloc = FALSE;
   S16                     ret;
   TfuUePucchRecpReq       *hqRecpReq;
#ifdef TFU_UPGRADE
   U8                      M;
   U8                      P;
   U8                      m;
   U8                      nP;
   U8                      nPlusOne;
   U16                     pucchRes;
   U8                      resIdx;
   U8                     nCce;
 /* rg003.301 [ccpu00120738] ADD Correcting HqSz filling */
   U8                      prevHqSize;
#else
   U8                      multCnt;
#endif

   if(pucchInfo == NULLP)
   {
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchInfo,
                  sizeof(RgSchUePucchRecpInfo), memCp)) != ROK)
      {
         RGSCHDBGERR (cellCb->instIdx,
               (rgSchPBuf(cellCb->instIdx),"rgSCHTomUtlFillSfHqFdbk() Unable to Allocate \
                 TfuUeRecpReqInfo for cellId=%d \n", cellCb->cellId));
         MSPD_ERR ("Unable to Allocate \
                 TfuUeRecpReqInfo for cellId=%d \n", cellCb->cellId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      alloc = TRUE;

      if ((ret = rgSCHUtlGetEventMem((Ptr *)&(pucchInfo->pucchRecpInfo),
                  sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp))) != ROK)
      {
         RGSCHDBGERR (cellCb->instIdx,
               (rgSchPBuf(cellCb->instIdx),"rgSCHTomUtlFillSfHqFdbk() Unable to Allocate \
                 TfuUeRecpReqInfo for cellId=%d \n", cellCb->cellId));
         MSPD_ERR ("Unable to Allocate \
                 TfuUeRecpReqInfo for cellId=%d \n", cellCb->cellId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         RETVALUE(ret);
      }
      pucchInfo->pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
      pucchInfo->pucchRecpInfo->rnti = rnti;
#ifdef TFU_UPGRADE
      /* rg003.301 [ccpu00120751]-ADD- set initial uci value in first time */
      pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo=TFU_PUCCH_HARQ;
#endif
   }
   /* Fill m, M, P */
   /* Calculation of resources same for both bundling and muxing for M = 1
    * */
   if((ackNackMode == RGR_TDD_ACKNACK_MODE_BUNDL) || 
         ((noFdbks == 1) && (ackNackMode == RGR_TDD_ACKNACK_MODE_MULT)))
   {
      hqRecpReq = &(pucchInfo->pucchRecpInfo->t.pucchRecpReq);
#ifdef TFU_UPGRADE
      prevHqSize =  hqRecpReq->hqInfo.hqSz;
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
      /* Only one index for bundling case */
      hqRecpReq->M = noFdbks;
      hqRecpReq->hqType =
         TFU_HQ_RECP_REQ_NORMAL;
      /* rg002.201: Setting multCnt = 1 for ack-nack bundling */
      hqRecpReq->multCnt = 1;
      hqRecpReq->t.nCce[0] = 
         pdcch->nCce;
      hqRecpReq->m[0] = 
         dlSf->dlFdbkInfo.m;
      hqRecpReq->p[0] =
         rgSCHCmnGetPValFrmCCE(cellCb, pdcch->nCce);

      hqRecpReq->type = TFU_UCI_HARQ;

#else /* TFU_UPGRADE */
      /*rg003.301 [ccpu00120751]-MOD-avoid overriding latest uci value */
      hqRecpReq->hqInfo.hqFdbkMode = (TfuAckNackMode)ackNackMode;
      M = noFdbks;
      P = rgSCHCmnGetPValFrmCCE(cellCb, pdcch->nCce);
      nP = cellCb->rgSchTddNpValTbl[P];
      nPlusOne = cellCb->rgSchTddNpValTbl[P + 1];
      m = dlSf->dlFdbkInfo.m;
      /* In case of no UE */
      pucchRes = (M - m - 1)* nP + (m * nPlusOne) + pdcch->nCce + 
         cellCb->pucchCfg.n1PucchAn;
      hqRecpReq->hqInfo.hqRes[hqRecpReq->hqInfo.pucchResCnt] = pucchRes;
      hqRecpReq->hqInfo.pucchResCnt++;

      /* rg003.301 [ccpu00120738] MOD Correcting HqSz filling */
      if((ackNackMode == RGR_TDD_ACKNACK_MODE_BUNDL) && (hqSz > prevHqSize))
          hqRecpReq->hqInfo.hqSz = hqSz;
      else if (ackNackMode == RGR_TDD_ACKNACK_MODE_MULT)
          hqRecpReq->hqInfo.hqSz = hqSz;
      else
          hqRecpReq->hqInfo.hqSz = prevHqSize;

#endif /* TFU_UPGRADE */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, 
            &(pucchInfo->pucchRecpInfo->lnk));
      pucchInfo->pucchRecpInfo->lnk.node = 
         (PTR)pucchInfo->pucchRecpInfo;
#endif
   }
   else /* Multiplexing */
   {
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
      pucchInfo->pucchRecpInfo->t.pucchRecpReq.M = noFdbks;
#ifdef LTEMAC_SPS
      if (n1PucchTkn.pres == TRUE)
      {
         pucchInfo->pucchRecpInfo->t.pucchRecpReq.hqType =
            TFU_HQ_RECP_REQ_N1PUCCH;
         pucchInfo->pucchRecpInfo->t.pucchRecpReq.t.n1Pucch = n1PucchTkn.val;
      }
      else
#endif
      {
         pucchInfo->pucchRecpInfo->t.pucchRecpReq.hqType =
            TFU_HQ_RECP_REQ_NORMAL;
         multCnt = pucchInfo->pucchRecpInfo->t.pucchRecpReq.multCnt;
         pucchInfo->pucchRecpInfo->t.pucchRecpReq.t.nCce[multCnt] = 
            pdcch->nCce;
         pucchInfo->pucchRecpInfo->t.pucchRecpReq.m[multCnt] =
            dlSf->dlFdbkInfo.m;
         pucchInfo->pucchRecpInfo->t.pucchRecpReq.p[multCnt] =
            rgSCHCmnGetPValFrmCCE(cellCb, pdcch->nCce);

         pucchInfo->pucchRecpInfo->t.pucchRecpReq.multCnt++;
      }
#else /* TFU_UPGRADE */

      hqRecpReq = &(pucchInfo->pucchRecpInfo->t.pucchRecpReq);
      hqRecpReq->hqInfo.hqFdbkMode = (TfuAckNackMode)RGR_TDD_ACKNACK_MODE_MULT;
      hqRecpReq->hqInfo.hqSz = noFdbks;

      resIdx = hqRecpReq->hqInfo.pucchResCnt;
      hqRecpReq->hqInfo.pucchResCnt++;

#ifdef LTEMAC_SPS
      if (n1PucchTkn.pres == TRUE)
      {
         hqRecpReq->hqInfo.hqRes[resIdx] = n1PucchTkn.val;
      }
      else
#endif
      {
         M = noFdbks;
         m = dlSf->dlFdbkInfo.m;
         nCce = pdcch->nCce;
         P = rgSCHCmnGetPValFrmCCE(cellCb, nCce);
         nP = cellCb->rgSchTddNpValTbl[P];
         nPlusOne = cellCb->rgSchTddNpValTbl[P + 1];
         hqRecpReq->hqInfo.hqRes[resIdx] = (M - m - 1)* nP + 
            (m * nPlusOne) + pdcch->nCce + 
            cellCb->pucchCfg.n1PucchAn;
      }
#endif /* TFU_UPGRADE */
      /* If all the DL subframes are scanned, then
       * send TFU request*/
#ifndef TFU_UPGRADE
      if((elemIdx != noFdbks) && alloc)
      {
         cmHashListInsert(&cellCb->ueTfuPendLst, (PTR) pucchInfo,
               (U8 *)&rnti, (U16) sizeof(rnti));
         alloc = FALSE;
      }
      else
      {
         pucchInfo->pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, 
               &(pucchInfo->pucchRecpInfo->lnk));
         pucchInfo->pucchRecpInfo->lnk.node = 
            (PTR)pucchInfo->pucchRecpInfo;
         /* Delete the entry after addition to the list */
         cmHashListDelete(&cellCb->ueTfuPendLst, (PTR) pucchInfo);
      }
#endif
   }

   *pucchInfoRef = pucchInfo;
   *allocRef     = alloc;
   RETVALUE(ROK);
}
#endif

/*rg002.301 - Add - [ccpu00118352] - UL Scheduling at CRC Indication */
#ifndef LTE_TDD
#ifdef RG_ULSCHED_AT_CRC
/** @brief This function does all the processing related to a single downlink
 * subframe.
 *
 * @details 
 *
 *     Function: rgSCHTomUtlProcDlSfAtCrc
 *
 *         Processing steps:
 *         - collate control data for all UEs and send to PHY 
 *         - collate data buffers for all UEs and send to PHY 
 *
 * @param  [in] RgSchDlSf     *ulSf
 * @param  [in] RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 * @return S16
 */
#ifdef ANSI
PRIVATE S16 rgSCHTomUtlProcDlSfAtCrc
(
RgSchDlSf            *ulSf,
RgSchCellCb          *cell,
RgSchErrInfo         *err
)
#else
PRIVATE S16 rgSCHTomUtlProcDlSfAtCrc (ulSf, cell, err)
RgSchDlSf            *ulSf;
RgSchCellCb          *cell;
RgSchErrInfo         *err;
#endif
{
   TfuCntrlReqInfo   *cntrlInfo;
   Inst              inst = cell->instIdx;
   S16               ret;

   TRC2(rgSCHTomUtlProcDlSfAtCrc);
  
   /* Fill control data from scheduler to PHY */   
   if ((ret = rgSCHUtlAllocEventMem(inst, (Ptr *)&cntrlInfo, 
                            sizeof(TfuCntrlReqInfo))) != ROK)
   {
      RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
         "Allocate TfuCntrlReqInfo for cellId=%d \n", cell->cellId));
      MSPD_ERR ("Unable to "
         "Allocate TfuCntrlReqInfo for cellId=%d \n", cell->cellId);
      RETVALUE(ret);
   }
   else
   {
      cmLListInit(&cntrlInfo->phichLst);
      cmLListInit(&cntrlInfo->dlPdcchLst);
      cmLListInit(&cntrlInfo->ulPdcchLst);
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for UL control */  
      RGSCHADDTOCRNTTIME(cell->crntTime, cntrlInfo->ulTiming,
                         TFU_ULCNTRL_DLDELTA);
      /* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for DL control */  
      RGSCHADDTOCRNTTIME(cell->crntTime, cntrlInfo->dlTiming,
                        TFU_DLCNTRL_DLDELTA);
           
      cntrlInfo->cellId = cell->cellId;
      /* Fill PHICH info */
      if ((ret = rgSCHTomUtlFillPhich (cntrlInfo, ulSf, err)) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
           "send PHICH info for cellId (%d)\n", cell->cellId));
         MSPD_ERR (" Unable to "
           "send PHICH info for cellId (%d)\n", cell->cellId);
         cmFreeMem((Ptr)cntrlInfo);
         RETVALUE(ret);
      }

      /* Fill UL Pdcch */
      if ((ret = rgSCHTomUtlFillUlPdcch (cntrlInfo, ulSf, err)) != ROK)
      {
         RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to "
           "send PDCCH info for cellId (%d)\n", cell->cellId));
         MSPD_ERR ("Unable to "
           "send PDCCH info for cellId (%d)\n", cell->cellId);
         cmFreeMem((Ptr)cntrlInfo);
         RETVALUE(ret);
      }

      if (cntrlInfo->ulPdcchLst.count || cntrlInfo->phichLst.count)
      {
         if (rgSCHUtlTfuCntrlReq(inst, cell->tfuSap->sapCfg.suId, cntrlInfo) 
               != ROK)
         {
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomUtlProcDlSf() Unable to"
                     " send Cntrl info for cellId (%d)\n", cell->cellId));
            MSPD_ERR ("Unable to"
                     " send Cntrl info for cellId (%d)\n", cell->cellId);
	  }
      }
	  else
	  {
         cmFreeMem((Ptr)cntrlInfo);
	  }
   }

   RETVALUE(ROK);
} /* end of */ 
#endif /* #ifdef RG_ULSCHED_AT_CRC*/
#endif 


/**********************************************************************
 
         End of file:     gk_sch_tom.c@@/main/2 - Sat Jul 30 02:21:51 2011
 
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
             rg001.201 apany 1. Always sending down TfuCntrlReq in DL TTIs
                            2. Always sending down TfuRecpReq in UL TTIs
                            3. Removed extra comments.
/main/1+     rg002.201 sd   1. Setting multCnt = 1 for ack-nack bundling.
                            2. Added support for period 0 to disable tick to RRM.
             rg003.201 sm   1. corrected the statement in the line 2032.
             rg004.201 ns   1. ccpu00105882: Made changes in handling ACK for
                               msg4.
                            2. ccpu00109165:Filling isRtx in puschRecpReq
             rg005.201 sm   1. Changes for optimization
             rg006.201 sd   1. ccpu00111718: Updating NDI and HARQ proc Id
                            2. Changed prototype for function 
                               'rgSCHUtlAllocRcptInfo'. 
                            3. ccpu000111706, ccpu00112394: 
                               TFU_DELTA dependency removed and added 
                               dependency on RG_UL_DELTA dependency
                            4. [ccpu000111706, ccpu00112394]: Separated TTI
                            5. [ccpu00113186]: Corrected TA handling
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependency on MIMO compile-time flag
               rsharon 2.Added support for SPS.
           rg009.201 rsharon 1.[ccpu00112372]:Changes for CR timer.
           rg009.201 dv  1. Added changes of TFU_UPGRADE.
                         2.Changes for passing iPhich at TFU interface.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
           rg001.301 asehgal 1.Added the changes corresponding to the 
                               CR ccpu00118844 to Change the revHqCb 
                             2. Changes for the CR ccpu00118643.
           rg002.301 asehgal 1.Added the changes corresponding to the 
                               CR ccpu00118844 to Change the revHqCb.
                             2. Changes for the CR ccpu00118643.
                             3. [ccpu00119223]Added the changes for the rnti 
                               assignment with UeId instead of rntiLnk->rnti
                             4. Added changes for ccpu00118352- ULSCH at CRC
                             5. ccpu00119456 and ccpu00119458 - Correcting HqSz 
                             6. ccpu00120233/332 - Handling for PUSCH and PUCCH 
                                RR for same UE issue.
                             7. ccpu00118351:seperate UL & DL deltas.
                             8. ccpu001120284:setting taLnk->node to NULL
*********************************************************************91*/
