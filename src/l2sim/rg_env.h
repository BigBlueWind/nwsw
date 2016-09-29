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
 
    Name:   LTE MAC layer
 
    Type:   H include file
 
    Desc:   Defines required by LTE MAC
 
    File:   rg_env.h
 
    Sid:      rg_env.h@@/main/3 - Sat Jul 30 02:21:58 2011
 
    Prg:    apany
 
**********************************************************************/

 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE MAC
*
*/
 
#ifndef __RGENVH__
#define __RGENVH__

/* Number of Unserved UE's Facotr for BI value calculation */
/* R8 Upgrade */
#define RG_SCH_CMN_BI_NUMUE_FACTOR    1
/* The number of schedulers supported currently */ 
#ifdef RG_PHASE2_SCHED
#define RGSCH_NUM_SCHEDULERS    4
#define RGSCH_NUM_DLFS_SCHEDULERS    1
#else
#define RGSCH_NUM_SCHEDULERS    1
#define RGSCH_NUM_DLFS_SCHEDULERS    0
#endif
/*rg008.201 - Added support for SPS*/

#ifdef LTEMAC_SPS
#define RG_SCH_NUM_SPS_OCC_AFTR_EXP_REL 2 /* Number of empty SPS Occasions
                                             after sending explicit release after
                                             which SPS resources of the UE shall 
                                             be deallocated */
#define RGSCH_SPS_CQI_SCALE_FACTOR 1
#endif

#ifdef LTE_TDD
/* The number of frames ahead that RLC may require a STA IND */
#define RG_DL_DELTA (TFU_DELTA + RGU_DELTA)

/* The number of frames after PHY delivers the frame */
/* rg009.201- ccpu00116499- Define RG_UL_DELTA only if not defined */
#ifndef RG_UL_DELTA
#define RG_UL_DELTA  1
#endif

#else /* LTE_TDD */

/* The number of frames ahead that RLC may require a STA IND */
/* rg002.301 -MOD-[ccpu00118351] Seperated DL control and DL data timing deltas
 * in FDD */
#if (TFU_DLDATA_DLDELTA > TFU_DLCNTRL_DLDELTA)
#define RG_DL_DELTA (TFU_DLDATA_DLDELTA + RGU_DELTA)
#else
#define RG_DL_DELTA (TFU_DLCNTRL_DLDELTA + RGU_DELTA)
#endif

/* 
 * rg002.301 -MOD-[ccpu00118351] RG_UL_DELTA is replaced with TFU_CRCIND_ULDELTA
 * and moved into envopt.h. Applicable to FDD only. 
 */
#endif /* LTE_TDD */

/* List of scheduler init functions to initialise rgSchSchdInits[] array with */
#ifdef RG_PHASE2_SCHED
#define RGSCH_ULSCHED_INITS  {rgSCHSc1UlInit, rgSCHPfsUlInit, rgSCHRrUlInit, rgSCHMaxciUlInit}
#define RGSCH_DLSCHED_INITS  {rgSCHSc1DlInit, rgSCHPfsDlInit, rgSCHRrDlInit, rgSCHMaxciDlInit}
#define RGSCH_DLFSSCHED_INITS {rgSCHDlfsInit}
#else
#define RGSCH_ULSCHED_INITS  {rgSCHSc1UlInit}
#define RGSCH_DLSCHED_INITS  {rgSCHSc1DlInit}
#define RGSCH_DLFSSCHED_INITS {}
#endif


#define RG_UL_SCHED_DELTA 2 /* <= RGU_DELTA, DL sched to happen first */
/* RG_UL_SCHED_DELTA + 2 * TFU_DELTA should be <= 3 */

/* Maximum number of system frames for the completion of a random access procedure */
/*ccpu00121664 */
#define RG_MAX_RA_PRC_FRM  10

/* Common Scheduler Tunable Parameters */
#define RG_SCH_CMN_UE_IDLETIME_FCTR        3
#define RG_SCH_CMN_MAX_BITS_RATIO          14
#define RG_SCH_CMN_UL_COM_DENOM            16 
#define RG_SCH_CMN_UL_NUM_CQI              16
#define RG_SCH_CMN_MAX_UE_PER_UL_SF        8
/* rg006.201: [ccpu00112789] Added configuration for maximum number of
   MSG3s */
#define RG_SCH_CMN_MAX_MSG3_PER_UL_SF      4
#define RG_SCH_CMN_MAX_UL_BW_PER_UE        100
#define RG_SCH_CMN_DL_DELTA                RG_DL_DELTA  /* This parameter should not be less than RG_DL_DELTA */
#define RG_SCH_CMN_MAX_DL_RETX_BW          100 
#define RG_SCH_CMN_MAX_DL_BW_PERUE         100 
#define RG_SCH_CMN_DEF_BCCHPCCH_CODERATE   512
#define RG_SCH_CMN_MAX_DL_AMBR             0xFFFFFFFF 
#define RG_SCH_CMN_MAX_UL_UEBR             0xFFFFFFFF 
#define RG_SCH_CMN_DED_MAX_HDRSIZE         3
#define RG_SCH_CMN_MAX_DED_SDU             5
/* GBR priorities occupy a set of contiguous priorities      */
/* starting always at 1.                                     */
/* RG_SCH_CMN_MAX_PRIO is the total number of priority queues    */
/* defined in the scheduler. This variable can have affect   */
/* on the performance of the scheduler and should be chosen  */
/* based on the same.                                        */
#define RG_SCH_CMN_MAX_PRIO              8

#ifdef RG_SC1
/* UL Scheduler1 tunable params */
#define RG_SC1_BSR_BS         4
#define RG_SC1_SR_BS          RG_SC1_BSR_BS
#define RG_SC1_UL_RATIO       14  /* UL_RATIO/COM_DENOM of bs for LCGs other than */
                                /* highest priority LCG used */

/* Priority 0 is used as a FIFO service where the priority   */
/* of UEs within the priority Q is based purely on time at   */
/* which the service is reported to the scheduler. This      */
/* priority queue is suitable for DCCH services.             */


/* This is the maximum known QCIs to the scheduler.          */
/* RG_SC1_QCI_TO_PRIO maps QCIs to respective priorities and */
/* there by behavior of the service                          */

/* This table is used to translate a CQI to an applicable */
/* aggregation level to be used for a UE when allocating  */
/* a PDCCH.                                               */
/* This table is used to translate a CQI to an applicable  */
/* coding rate for dedicated PDSCH allocation. The number  */
/* in the table represents the number of bits to allocated */
/* per 1024 REs.                                           */

#endif
/* rg005.201 Fix for CR: ccpu00111069 Increasing from 10 to 12 
 * to accomodate the TA for more number of UEs */
#define RGSCH_MAX_UE_PER_DL_SF 12
#define RGSCH_MAX_RARNTI_PER_DL_SF 4
#define RGSCH_MAX_RNTI_PER_RARNTI 10 /* rg001.201:  be looked at */
#define  RGSCH_INST_START 1
#define RGSCH_MAX_INST 1


/* Twice of difference in power levels between successive uplink
 * CQIs (in dB) */
#define RG_SCH_UL_CQI_DB_STEP_2   2

#define RG_SCH_CMN_MAX_NUM_TPC_PUCCH_RNTI 100
#define RG_SCH_CMN_MAX_NUM_TPC_PUSCH_RNTI 100
/*rg009.201 - Guard timer for CR timer implementation*/
/*Guard timer is the number of subframes before contention resolution timer
 * expiry at which the contention resultion CE will be scheduled. The value for
 * guard timer should take of the TFU_DELTA for processing the CE as well as the
 * feedback and RTT for the number of re-transmissions desired before contention
 * resolution timer expiry*/
#define RG_SCH_GRD_TMR 8 
/*rg001.301-ADD-:  [ccpu00117270]moving rgSchCmnUlCqiTbl values here to enable customer to 
 *                   fine tune these values 
 */
/*rg001.301-ADD-: [ccpu00117270]adding modulation order & efficiency hash defines for
 *                   UL. This can be tuned by customer. These values are used
 *                   in rgSchCmnUlCqiTbl in rg_sch_cmn.c
 */

/* for CQI 1 */
#define RGSCH_CMN_QM_CQI_1      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_1      156

/* for CQI 2 */
#define RGSCH_CMN_QM_CQI_2      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_2      240 

/* for CQI 3 */
#define RGSCH_CMN_QM_CQI_3      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_3      386 

/* for CQI 4 */
#define RGSCH_CMN_QM_CQI_4      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_4      616

/* for CQI 5 */
#define RGSCH_CMN_QM_CQI_5      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_5      898

/* for CQI 6 */
#define RGSCH_CMN_QM_CQI_6      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_6      1204 

/* for CQI 7 */
#define RGSCH_CMN_QM_CQI_7      RGSCH_QM_QPSK
#define RGSCH_CMN_UL_EFF_CQI_7      1512 

/* for CQI 8 */
#define RGSCH_CMN_QM_CQI_8      RGSCH_QM_QPSK
#define RGSCH_CMN_UL_EFF_CQI_8      1960 

/* for CQI 9 */
#define RGSCH_CMN_QM_CQI_9      RGSCH_QM_QPSK
#define RGSCH_CMN_UL_EFF_CQI_9      2464 

/* for CQI 10 */
#define RGSCH_CMN_QM_CQI_10      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_10      2796 

/* for CQI 11 */
#define RGSCH_CMN_QM_CQI_11      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_11      3402 

/* for CQI 12 */
#define RGSCH_CMN_QM_CQI_12      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_12      3996 

/* for CQI 13 */
#define RGSCH_CMN_QM_CQI_13      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_13      4632 

/* for CQI 14 */
#define RGSCH_CMN_QM_CQI_14      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_14      5238 

/* for CQI 15 */
#define RGSCH_CMN_QM_CQI_15      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_15      5728 

/* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
#define RG_SCH_MAX_RETX_ALLOC_FAIL  10 

#endif /* __RGENVH__ */


/**********************************************************************
         End of file:     rg_env.h@@/main/3 - Sat Jul 30 02:21:58 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---   apany 1. Initial Release.
/main/2      ---   apany 1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
             rg005.201 sm 1. Fix for CR ccpu00111069
             rg006.201 sd 1. ccpu00112789: Added MACRO for default maximum
                             MSG3 per UL SF.
             rg008.201 rsharon 1.Added support for SPS.
             rg009.201 rsharon 1.[ccpu00112372]Changes for CR timer.
                       dv      2.[ccpu00116499]RG_UL_DELTA defined only if not defined
/main/3      ---         gvj 1. Updated for LTE MAC Rel 3.1
             rg001.301 asehgal 1.Changes for the ticket ccpu00117270 .
             rg002.301 asehgal 1.ccpu00118351-seperate deltas for UL & DL.
             rg003.301 hsingh  1. ccpu00120794-Freeing up the HARQ proc blocked 
                                  for indefinite time in case of Retx
*********************************************************************91*/
