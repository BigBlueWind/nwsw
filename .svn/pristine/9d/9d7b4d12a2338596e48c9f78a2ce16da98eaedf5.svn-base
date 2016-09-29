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
 
    Name:   LTE MAC PFS scheduler
 
    Type:   C include file
 
    Desc:   Defines required by PFS scheduler
 
    File:   rg_sch_pfs.x
 
    Sid:      rg_sch_pfs.x@@/main/2 - Sat Jul 30 02:22:08 2011
 
    Prg:    apany
 
**********************************************************************/
/** @file rg_sch_pfs.x
@brief This file contains data structures for the Propotional fair scheduler.
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

#ifndef __RGPFSX__
#define __RGPFSX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* MS_WORKAROUND for ccpu00122892  : Temp fix for erroeneous RETX Harq release by rgSCHCmnDlAllocRetxRb */
EXTERN Void rgSCHPfsDlProcRmvFrmRetx ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlHqProcCb *hqP));
   /* MS_FIX : Updating PFS to start prio with highest possible value
               and diminishing prio val updates from there on */
/**
  @brief Data structure holding info of frac Prio updates */
typedef struct rgSchPfsFracPrioInfo { 
   U32  fracDataRate;/*!< Amount of data rate to be serviced
                          to move one notch in fracPrio.
                          Precomputed for a given LC Cfg.*/
   U32  achvdFracDataRate; /*!< Amount of data rate serviced
                                till now.
                                achvdFracDataRate < fracDataRate.
                                Reset at every refresh cycle. */ 
   U32  fracPrioLvls; /*!< Number of fracDataRates serviced so far
                           in this refresh cycle. Reset at every
                           refresh cycle. */
}RgSchPfsFracPrioInfo;

/**
  @brief Downlink Logical channel control block for PFS. */
typedef struct rgSchPfsDlLc
{
   U8              lcType;     /*!< LC type - SRB/DRB0/GRB/AMBR */
   U32             gbr;        /*!< Configured GBR */
   U32             deltaMbr;   /*!< Configured MBR in excess of configured GBR */
   U32             remGbr;     /*!< GBR remaining for current refresh cycle */
   U32             remDeltaMbr;/*!< MBR remaining in excess of GBR */
   U32             bo;         /*!< Buffer occupancy */
   U32             effBo;      /*!< Effective BO */
   U32             hdrSz;      /*!< Est header size, valid during scheduling */
   U32             reqBytes;   /*!< effBo + hdrSz, valid during scheduling */
   CmLListCp      *prioLstCp;
   CmLList         prioLnk;     /*!< To link service into priority queues */
   CmLList         schedSvcLnk;  /*!< To link service into UE's list of allocated
                                  * services in a TTI */
   CmLList         ambrLnk;     /*!< To link AMBR svcs in UE */
   U32             pfsPrio;     /*!< PSF priority */
   U32             svcPrio;     /*!< Service priority */
   U32             svccqiPrio;  /*!< svcPrio * cqiPrio */
   U8              qId;         /*!< Queue ID */
   /* MS_FIX : Updating PFS to start prio with highest possible value
      and diminishing prio val updates from there on. */
   RgSchPfsFracPrioInfo  fracPrioInfo; /*!< Data structure holding 
                                          info of frac Prio updates */
#if 0
   U32             remRate;    /*!< Remaining rate, as a fraction on 16;
                                *  this is always 100% for non-GBR services
                                *  with no AMBR configured */
   U32             remRatePrio; /*!< Priority corresponding to remaining rate */
#endif
   CmLList         cfgLcLnk;    /*!< To link cfgd GBR/AMBR services */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   CmLList         svcPrioLnk;  /*!< To link service into UE->svcPrioLst */
} RgSchPfsDlLc;


/**
  @brief Downlink HARQ control block for PFS. */
typedef struct rgSchPfsDlHqProc
{
   CmLList         retxLnk;  /*!< To link together retx procs in cell lists */
   CmLListCp      *retxLstCp; /* This is to hold the retx queue pointer
                               * as soon as HARQ proc has been scheduled. This
                               * is to avoid checks to determine which queue
                               * to add proc to for retx. */
   CmLList         ueRetxLnk;  /*!< To link together retx procs in UE's
                                * list of retx procs */
} RgSchPfsDlHqProc;

/**
  @brief UE DL control block for PFS. */
typedef struct rgSchPfsDlUe
{
   U32            remAmbr;   /*!< Remaining AMBR in current refresh cycle */
   U8             cqi;       /*!< DL CQI (wideband) */
   U32            cqiPrio;   /*!< CQI priority */
   CmLList        retxLnk;   /*!< To link UE into priority queues, for retx */
   CmLListCp     *retxLstCp; /*!< Cp for retx priority queue */
   CmLListCp      cfgGbrLst;   /*!< Configured GBR services */
   CmLListCp      cfgAmbrLst;  /*!< Configured AMBR services (exluding DRB0) */
   CmLListCp      ambrLst;   /*!< AMBR (except DRB0) services with BO
                              * in priority order */
   RgSchDlLcCb   *ambrSvc;   /*!< Points to the current AMBR service */
   CmLListCp      retxProcs; /*!< List of procs in retx queues */
   /* The list in HARQ proc cannot be used easily, data struct corresponds to
    * one that is sent over RGU. So having a list of them separately as
    * schedSvcs. */
   CmLListCp      schedSvcs;  /*!< Scratchpad to hold LCs scheduled by PFS,
                                * in decr order of priority. This is
                                * maintained to possibly change (reduce)
                                * byte alloc to LCs after final RB alloc. */
   Bool           taReq;      /*!< While scheduling during a TTI,
                                * this indicates if TA was scheduled */
   /* Mentaing services at the UE level */
   CmLListCp               svcPriorLst;                           
   /* MS_FIX : Updating PFS to start prio with highest possible value
      and diminishing prio val updates from there on. */
   RgSchPfsFracPrioInfo  fracPrioInfo; /*!< Data structure holding 
                                          info of frac Prio updates
                                          for all AMBR services. */
} RgSchPfsDlUe;

/**
  @brief UE UL control block for PFS. */
typedef struct rgSchPfsUlUe
{
   U8             cqi;     /*!< UL CQI (wideband) */
   U32            cqiPrio;   /*!< CQI priority */
   U32            lcgPrio;   /*!< LCG priority */
   U32            fracPrio;  /*!< Remaining fraction priority */
   U32            pfsPrio;   /*!< PFS priority */
   U8             qId;       /*!< PFS queue ID - not applicable when in LCG0
                               *  or contention resolution queue */
   CmLListCp     *lstCp;     /*!< Priority Q that the UE is in */
   CmLList        prioLnk;   /*!< To link into priority queues */
   Bool           srRcvd;    /*!< Set to TRUE when SR is rcvd, set
                              *   to FALSE once allocation has been made */
   /*rg002.301_LPR2-ADD-Added the new parameter below to maintain a context
    * that whether the UE has been scheduled in current UL SF.*/
   Bool           schedInCrntUlSf; /*!< Set to TRUE if UE is scheduled
                                    in the current UL SF scheduled.*/
   /* MS_FIX : Updating PFS to start prio with highest possible value
      and diminishing prio val updates from there on. */
   RgSchPfsFracPrioInfo  fracPrioInfo; /*!< Data structure holding 
                                          info of frac Prio updates */
} RgSchPfsUlUe;

/**
  @brief PFS Queues. */
typedef struct rgSchPfsDlQueues
{
   CmLListCp     srbLst;            /*!< List of DCCHs [to be moved to common part] */
   CmLListCp     drb0Lst;           /*!< List of priority 0 svcs */
   CmLListCp     gbrLst[RG_SCH_PFS_DL_NUM_GBR_QUEUES]; /*!< List of GBR svcs for which
                                             * GBR not yet satisfied */
   CmLListCp     mbrLst[RG_SCH_PFS_DL_NUM_MBR_QUEUES]; /*!< List of GBR svcs for which
                                             * GBR already satisfied */
   CmLListCp     ambrLst[RG_SCH_PFS_DL_NUM_AMBR_QUEUES];/*!< List of other non-GBR svcs */
} RgSchPfsDlQueues;

/**
  @brief PFS Queues for new transmission - each list is a list of LCs. */
typedef RgSchPfsDlQueues RgSchPfsDlTxQueues;

/**
  @brief PFS Queues for retransmission - each list is a list of UEs. */
typedef RgSchPfsDlQueues RgSchPfsDlRetxQueues;

/**
  @brief PFS UL Queues. */
typedef struct rgSchPfsUlQueues
{
   CmLListCp     contResLst;     /*!< UEs with pending UL alloc for msg4
                                  *  [to be moved to common part] */
   CmLListCp     sigLst;        /*!< UEs with buffer pending on LCG0 */
   CmLListCp     dataLst[RG_SCH_PFS_UL_NUM_QUEUES];  /*!< UEs with buffer pending
                                                * on LCGs other than LCG0 */
} RgSchPfsUlQueues;

/**
  @brief PFS DL tables. */
typedef struct rgSchPfsDlTbls
{
   U32         cqiPrios[RGR_PFS_CQI_PRIO_LVLS]; /*!< CQI priorities */
   U32         svcPrioPrios[RGR_PFS_SVC_PRIO_LVLS]; /*!< QCI priorities mapped to priorities
                                                   * to be used in PFS */
   U32         fracPrios[RGR_PFS_FRAC_PRIO_LVLS]; /*!< BSR priorities */
} RgSchPfsDlTbls;


/**
  @brief Priority value range and distribution. */
typedef struct rgSchPfsPrioRange
{
   U32      min;         /*!< Minimum value for queue class */
#if (ERRCLASS & ERRCLS_DEBUG)
   U32      max;         /*!< Maximum value for queue class */
#endif
   /* MS_FIX : syed For small range and large number of Queues
    * precalculated numElemPerQ can be highly inaccurate. It
    * was leading to a crash.  */
   U32      numQs;       /*!< Number of Prio Queues for this class */
   U32      maxDiff;     /*!< maxPrio - minPrio + 1 */
} RgSchPfsPrioRange;

/**
  @brief Indicates priority ranges for each class. */
typedef struct rgSchPfsDlPrioRanges
{
   RgSchPfsPrioRange  gbrRange;
   RgSchPfsPrioRange  mbrRange;
   RgSchPfsPrioRange  ambrRange;
} RgSchPfsDlPrioRanges;


/**
  @brief Cell DL control block for PFS. */
typedef struct rgSchPfsDlCell
{
   RgSchPfsDlTxQueues   txQueues;   /*!< Queues for new transmission */
   RgSchPfsDlRetxQueues retxQueues; /*!< Queues for retransmission */
   RgSchPfsDlTbls       tbls;     /*!< Tables for mapping individual
                                   * values to priorities */
   RgSchPfsDlPrioRanges ranges;   /*!< PFS priority ranges for various classes */
   /* MS_FIX : syed MAX Initial Priority Value to be used for prio initialization. */
   U32                  initPrioVal; /* Initial prio value assigned to the 
                                        schedulable entity */
} RgSchPfsDlCell;

/**
  @brief Holder for uplink tables. */
typedef struct rgSchPfsUlTbls
{
   U32         cqiPrios[RGR_PFS_CQI_PRIO_LVLS]; /*!< CQI priorities */
   U32         fracPrios[RGR_PFS_FRAC_PRIO_LVLS]; /*!< BSR priorities */
   U32         lcgPrios[RGR_PFS_LCG_PRIO_LVLS];  /*!< Priority value corresponding to
                              * LCGs 1, 2 and 3 respectively */
} RgSchPfsUlTbls;

/**
  @brief Cell UL control block for PFS. */
typedef struct rgSchPfsUlCell
{
   RgSchPfsUlQueues     txQueues; /*!< Queues for new transmission */
   RgSchPfsUlTbls       tbls;     /*!< Tables to map PFS params to priorities */
   RgSchPfsPrioRange    range;    /*!< PFS priority range information */
   /* MS_FIX : syed MAX Initial Priority Value to be used for prio initialization. */
   U32                  initPrioVal; /*!< Initial prio value assigned to 
                                          the schedulable entity */
} RgSchPfsUlCell;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGPFSX__ */


/**********************************************************************
         End of file:     rg_sch_pfs.x@@/main/2 - Sat Jul 30 02:22:08 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     apany   1. LTE MAC 2.1 release
              rg007.201   ap  1. Added support for MIMO
           rg008.201      sd  1.Removed dependency on MIMO compile-time flag
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
