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


/**********************************************************************
 
    Name:   LTE MAC SC1 scheduler
 
    Type:   C include file
 
    Desc:   Defines required by SC1 scheduler
 
    File:   rg_sch_sc1.x
 
    Sid:      rg_sch_sc1.x@@/main/2 - Sat Jul 30 02:22:11 2011
 
    Prg:    sm
 
**********************************************************************/
/** @file rg_sch_sc1.x
@brief This file contains data structures for the SC1 scheduler.
*/
 

/*
*     The defines in this file correspond to structures and variables
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/

#ifndef __RGSCHSC1X__
#define __RGSCHSC1X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef Void (*RgSchSc1SvcMngFunc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *svc));

/**
  * @brief
  * Cell specific downlink scheduling information for Scheduler type 1.
  */
typedef struct rgSc1DlCell
{
   CmLListCp          prioLst[RG_SC1_DL_NUM_Q];   /*!< Priority queues */
   CmLListCp          retxLst[RG_SC1_DL_NUM_Q];   /*!< Retransmission queues */
   RgSchSc1SvcMngFunc svcMngFunc[RG_SC1_DL_NUM_Q]; /*!< Function for managing
                                                        services */
} RgSchSc1DlCell;

/**
  * @brief
  * Cell specific uplink scheduling information for Scheduler type 1.
  */
typedef struct rgSc1UlCell
{
   CmLListCp     contResLst;/*!< UEs with pending UL alloc for msg4 */
   CmLListCp     ueTxLst[2];/*!< Queue for UEs Ded Data Trans
                             *   ueTxLst[0] for signalling and
                             *   ueTxLst[1] for other data */
} RgSchSc1UlCell;

/**
  * @brief
  * Uplink information for scheduler per UE.
  */
typedef struct rgSc1UlUe
{
   CmLList      txLnk;      /* Lnk to one of Transmission Queues */
   CmLList      contResLnk; /* Lnk to one of Cont Res Queue */
   Bool         srRcvd;     /* TRUE if SR reported is yet to be satisfied */
   U8           qId;        /* Id of the Tx Q to which this UE belongs */
} RgSchSc1UlUe;

/**
  * @brief
  * Downlink information for scheduler per UE.
  */
typedef struct rgSc1DlUe
{
   CmLList           prioLnk; /*!< To link UE into priority queues */
   CmLListCp         gbrSvcs; /*!< List of GBR services */
   U8                prio;    /*!< Current priority of UE for DL */
   CmLListCp         ambrLst; /*!< To maintain services per priority for a UE */
   RgSchDlLcCb       *ambrSvc;/*!< Points to the current AMBR service */
   U32               ambr;    /*!< UE's Remaining AMBR */
   U32               effAmbr; /*!< min(svc->bo, ambr)*/
   CmLListCp         schdSvcs; /*!< List of services for which 
                                 allocation requested */
   U8                taReqBytes;/*!< Set to the Number of bytes Requested for TA
                                  allocation */
   CmLListCp         retxHqProcs; /*!< List of RETX Hq Procs in suspension */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   CmLListCp         lcsWithData; /*!< List of services with non-zero effBo */
} RgSchSc1DlUe;

/**
  * @brief
  * Downlink HARQ information for SC1 scheduler per UE.
  */
typedef struct rgSc1DlHqProc {
   CmLList        retxLnkUe; /*!< To link retransmitting HARQ processes in ue */
   U8             cqi;     /*!< CQI at which the allocation was made */
   U8             prio;    /*!< Priority of the allocation */
}RgSchSc1DlHqProc;

/**
  * @brief
  * Downlink service information for SC1 scheduler per UE.
  */
typedef  struct rgSc1DlSvc {
   U32          gbr;     /*!< Pending GBR to be satisfied     */
   U32          mbr;     /*!< Pending MBR to be satisfied     */
   U32          bo;      /*!< BO yet to be satisfied */
   U32          effGbr;  /*!< GBR/BO, lower of the two        */
   U32          effMbr;  /*!< MBR/BO, lower of the two        */
   CmLList      gbrLnk;  /*!< used to maintain svcs to be refreshed */
   CmLList      prioLnk; /*!< Used to queue up services on UE */
   CmLList      schdSvcLnk; /*!< Used to queue up services on UE 
                             * scheduled svcs list */
   U32          reqBytes; /*!< Set to number of bytes Req for Allocation */
   U16          hdrEstimate; /*!< RLC Hdr est computed during SVC allocn */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   CmLList      lcWithDataLnk;/*!< Used to maintain svc in ue's 
                               *   lcsWithData List */
} RgSchSc1DlSvc;


EXTERN Void rgSCHSc1DlLcRmv ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
EXTERN Void rgSCHSc1DlLcBoUpd ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
EXTERN Void rgSCHSc1DlProcAddToCellRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
EXTERN Void rgSCHSc1DlAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
EXTERN Void rgSCHSc1UlAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
));
EXTERN Void rgSCHSc1UlCqiInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
));
EXTERN Void rgSCHSc1DlCqiInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt
));
EXTERN Void rgSCHSc1UlUeRefresh ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
EXTERN Void rgSCHSc1UlUeReset ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
EXTERN Void rgSCHSc1DlUeRefresh ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
EXTERN Void rgSCHSc1DlHndlInActUes ARGS((
RgSchCellCb           *cell,
CmLListCp             *inactvLst
));
EXTERN Void rgSCHSc1UlHndlInActUes ARGS((
RgSchCellCb           *cell,
CmLListCp             *inactvLst
));
EXTERN Void rgSCHSc1UlActvtUe ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
EXTERN Void rgSCHSc1DlActvtUe ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
EXTERN Void rgSCHSc1DlInit ARGS((
RgDlSchdApis *apis
));
EXTERN Void rgSCHSc1UlInit ARGS((
RgUlSchdApis *apis
));
EXTERN S16 rgSCHSc1RgrUlCellCfg ARGS((
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
));
EXTERN S16 rgSCHSc1RgrDlCellCfg ARGS((
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
));
EXTERN Void rgSCHSc1UlCellDel ARGS((
RgSchCellCb     *cell
));
EXTERN Void rgSCHSc1DlCellDel ARGS((
RgSchCellCb     *cell
));
EXTERN S16 rgSCHSc1RgrUlUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHSc1RgrDlUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHSc1RgrUlUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHSc1RgrDlUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));
EXTERN Void rgSCHSc1UlUeDel ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN Void rgSCHSc1DlUeDel ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN S16 rgSCHSc1RgrLcCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchCfg *lcCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHSc1RgrLcgCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgCfg    *lcgCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHSc1RgrLcRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchRecfg  *lcRecfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHSc1RgrLcgRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgRecfg  *reCfg,
RgSchErrInfo *err
));
EXTERN Void rgSCHSc1LcgDel ARGS((
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgSchLcgCb    *lcg
));
EXTERN Void rgSCHSc1UpdBsrShort ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
U8           bsr
));
EXTERN Void rgSCHSc1UpdBsrTrunc ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
U8           bsr
));
EXTERN Void rgSCHSc1UpdBsrLong ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8 bsArr[]
));
EXTERN Void rgSCHSc1ContResUlGrant ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN Void rgSCHSc1SrRcvd ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN Void rgSCHSc1UlSched ARGS((
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
));
EXTERN Void rgSCHSc1DlSched ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
EXTERN S16 rgSCHSc1RgrUlCellRecfg ARGS((
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
));
EXTERN Void rgSCHSc1DlUeReset ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));

EXTERN S16 rgSCHSc1DlUeHqEntInit ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));

EXTERN S16 rgSCHSc1DlUeHqEntDeInit ARGS((
RgSchCellCb      *cell,
RgSchDlHqEnt     *hqE
));
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGSCHSC1X__ */


/**********************************************************************
         End of file:     rg_sch_sc1.x@@/main/2 - Sat Jul 30 02:22:11 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sm               1. LTE MAC 2.1 release
           rg007.201   ap  1. Added support for MIMO
           rg008.201      sd  1.Removed dependency on MIMO compile-time flag
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
