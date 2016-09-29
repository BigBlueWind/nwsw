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
     Name:    LTE MAC SC1 scheduler 

     Type:    C header file

     Desc:    Defines required by SC1 scheduler

     File:    rg_sch_cmn.h

     Sid:      rg_sch_cmn.h@@/main/2 - Sat Jul 30 02:22:02 2011

     Prg:      

*********************************************************************21*/


#ifndef __RGSCHCMNH__
#define __RGSCHCMNH__ 
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
                         Macro Definitions 
 ***********************************************************************/

/* Scheduler1 tunable params */
#define RG_SCH_CMN_GET_CELL(cell) ((RgSchCmnCell *)((cell)->sc.sch))
#define RG_SCH_CMN_GET_UL_CELL(cell) &(((RgSchCmnCell *)((cell)->sc.sch))->ul)
#define RG_SCH_CMN_GET_DL_CELL(cell) &(((RgSchCmnCell *)((cell)->sc.sch))->dl)
#define RG_SCH_CMN_GET_UE(ue) ((RgSchCmnUe *)((ue)->sch))
#define RG_SCH_CMN_GET_UL_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->ul)
#define RG_SCH_CMN_GET_DL_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->dl)
#define RG_SCH_CMN_GET_CMN_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->cmn)
#define RG_SCH_CMN_GET_DL_HQP(hqP) ((RgSchCmnDlHqProc *)((hqP)->sch))
#define RG_SCH_CMN_GET_DL_SVC(svc) ((RgSchCmnDlSvc *)((svc)->sch))
#define RG_SCH_CMN_GET_UL_LCG(lcg) ((RgSchCmnLcg *)((lcg)->sch))

   /* rg005.201 Changing the check for retransmission - now checking if alloc
    * is non NULL instead of using the isRetx field of the harq Proc. 
    */
#define RG_SCH_CMN_IS_UL_UE_RETX(ue) ((&(ue)->ul.hqEnt.hqProcCb[\
 ((RgSchCmnCell *)(ue->cell->sc.sch))->ul.schdHqProcIdx])->alloc)

   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
#define RGSCH_SPS_ULBW_MASK_LEN 4
#define RGSCH_SPS_MG_MASK_LEN 10
#define RG_SCH_CMN_GET_UL_SPS_CELL(cell) &(((RgSchCmnCell *)((cell)->sc.sch))->ul.ulSpsInfo)
#define RG_SCH_CMN_GET_UL_SPS_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->ul.ulSpsInfo)


#define RGSCH_CMN_MIN_SCALABLE_CQI  RGSCH_SPS_CQI_SCALE_FACTOR+1
/* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for DL control in FDD */
#ifdef LTE_TDD
#define RGSCH_SPS_UL_DELTA (TFU_DELTA + RGSCH_PDCCH_PUSCH_DELTA)
#else
#define RGSCH_SPS_UL_DELTA (TFU_DLCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA)
#endif
#define RGSCH_SPS_UL_LCM   20 /* Default lcm to start the spsSfTbl with */

#define RG_SCH_CMN_IS_SPS_SCHD(ue) (((&(ue)->ul.hqEnt.hqProcCb[\
 ((RgSchCmnCell *)(ue->cell->sc.sch))->ul.schdHqProcIdx])->alloc ? TRUE:FALSE))
#endif /* LTEMAC_SPS */

#define RG_SCH_CMN_IS_UE_SCHDLD(ue) (((RgSchCmnUe *)((ue)->sch))->dl.proc != NULLP)
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
#define RG_SCH_CMN_PROC_SLCTD_FOR_RETX(proc) ((proc->tbInfo[0].txCntr |\
                                               proc->tbInfo[1].txCntr))
#define RG_SCH_CMN_DL_IS_UE_ACTIVE(ue) (ue->dl.dlInactvMask == 0)
#define RG_SCH_CMN_UL_IS_UE_ACTIVE(ue) (ue->ul.ulInactvMask == 0) 
/* ulInactvMask and dlInactvMask are simulataneously set/reset
 * hence check against one suffices */
#define RG_SCH_CMN_IS_UE_PDCCHODR_INACTV(ue) (ue->ul.ulInactvMask & RG_PDCCHODR_INACTIVE) 

#define RG_SCH_CMN_DL_SVC_IS_GBR(svc) ((((RgSchCmnDlSvc*)(svc->sch))->prio >= RG_SCH_CMN_DL_GBR_PRIO_START) && \
                                (((RgSchCmnDlSvc*)(svc->sch))->prio <= RG_SCH_CMN_DL_GBR_PRIO_END))

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */

/* Following #defines are used in calculating the cqi to TBS mapping 
 * In 36.213 table 7.1.7.1-1 defines mapping b/w Itbs and mcs and table 
 * 7.2.3-1 defines mapping b/w CQI to mcs and code rate.
 * The values of these tables are chosen such that average of tbs value for 
 * different nprbs with code rate nearest to the code rate defined in table
 * 7.2.3-1 for particular CQI value.We should also take care of the fact that 
 * mapping defined in table 7.1.7.1-1 is not overruled.*/
/*rg002.301 ccpu00109543 ADD fix for CFI0*/
#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI0 \
    {207, 271, 333, 431, 648, 899, 1161, 1482, 1889, 2103,\
        2905, 3412, 3956, 4474, 4655, 5397}

/*rg002.301 ccpu00109543 MOD fix for CFI1*/
#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI1 \
    {178, 222, 296, 364, 577, 856, 1125, 1411, 1833, 2460,\
        2612, 3453, 4026, 4595, 5090, 5902}

/*rg002.301 ccpu00109543 MOD fix for CFI2*/
#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI2 \
    {175, 255, 326, 400, 636, 944, 1240, 1556, 2021, 2536, 2881,\
        3504, 4116, 4772, 5398, 5616}

/*rg002.301 ccpu00109543 MOD fix for CFI3*/
#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI3 \
    {208, 295, 362, 446, 708, 869, 1207, 1734, 1991, 2540,\
        3212, 3561, 4248, 4590, 5322, 5653}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/* Number of information bits per 1024 Phy bits for PDCCH */
/* This is a customer tunable */
#define RG_SCH_CMN_CQI_TO_PDCCH_EFF \
       {  400,  100,  150,  200,  250,  280, 340, 365,\
         380, 400, 500, 600, 700, 800, 900, 1000}

/*rg002.301 ccpu00109543 ADD fix for CFI0*/
#define RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI0 \
    {188, 226, 309, 366, 412, 539, 662, 859, 1052, \
        1293, 1535, 2585, 2957, 3340, 4775, 5300}

/*rg002.301 ccpu00109543 ADD fix for CFI1*/
#define RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI1 \
    {197, 232, 306, 322, 408, 450, 588, 723, 938,\
        1150, 1413, 1679, 2826, 3233, 5222, 5796}

/*rg002.301 ccpu00109543 ADD fix for CFI2*/
#define RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI2 \
    {166, 207, 233, 307, 343, 408, 422, 495, 586,\
        648, 797, 1035, 1268, 1558, 3117, 3567}

/*rg002.301 ccpu00109543 ADD fix for CFI3*/
#define RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI3 \
    {177, 200, 221, 287, 323, 386, 433, 497, 522, 676,\
        722, 888, 1153, 1413, 1737, 3476}

#define RG_SCH_CMN_SET_FORCE_TD(_ue, _event)\
         {\
            RgSchCmnDlUe *_ueDl = RG_SCH_CMN_GET_DL_UE((_ue));\
            _ueDl->mimoInfo.forceTD |= (_event);\
         }
#define RG_SCH_CMN_UNSET_FORCE_TD(_ue, _event)\
         {\
            RgSchCmnDlUe *_ueDl = RG_SCH_CMN_GET_DL_UE((_ue));\
            _ueDl->mimoInfo.forceTD &= ~(_event);\
         }
#define RG_SCH_CMN_INIT_FORCE_TD(_ue, _val)\
         {\
            RgSchCmnDlUe *_ueDl = RG_SCH_CMN_GET_DL_UE((_ue));\
            _ueDl->mimoInfo.forceTD = (_val);\
         }

#define RG_SCH_CMN_TBS_TO_MCS(x, y) do {\
   if (x <= 9) y = x; \
   else if (x <= 15) y = x + 1; \
   else y = x + 2;\
} while(0)

/* rg004.201 patch - ccpu00109921 fix start */
#define RG_SCH_CMN_MCS_TO_TBS(x, y) do {\
   if (x <= 9) y = x; \
   else if (x <= 16) y = x - 1; \
   else y = x - 2; \
}while(0)
/* rg004.201 patch - ccpu00109921 fix end */    

/* RACHO start */
#define RG_SCH_CMN_DFLT_TATMR 10
/* minimum grant, in bytes, to be given to HO and pdcchOrdered UEs */
#define RG_SCH_MIN_GRNT_HOPO 2
/* maximum dedPrmbls */
#define RG_SCH_MAX_DED_PRMBLS 60
/* is PDCCH order generation supported */
#define RG_SCH_CMN_IS_PO_SPRTD(cell) (cell->rachCfg.raOccasion.sfnEnum != RGR_SFN_NA)
/* Min gap value between crntTime and time of PRACH Oppurtunity */
/* RG_SCH_CMN_DL_DELTA is the number of SFs from crntTime at which
 * UE is expected to recieve PDCCH Order.
 * n+6 as per 213 6.1.1 last para */
#define RG_SCH_CMN_MIN_PRACH_OPPR_GAP (6+RG_SCH_CMN_DL_DELTA)
/* Idle time threshold in terms of subframes, implies
 * the max duration between a TA expiry and latest UL
 * data/Signal transmission time */
#define RG_SCH_CMN_UE_IDLE_THRSLD(ue) ((ue->dl.taCb.cfgTaTmr > 0)?\
 (RG_SCH_CMN_UE_IDLETIME_FCTR * ue->dl.taCb.cfgTaTmr) :\
 (RG_SCH_CMN_UE_IDLETIME_FCTR * RG_SCH_CMN_DFLT_TATMR))
/* R8 Upgrade */
#define RG_SCH_CMN_GET_BI_VAL(prevVal,numUe) ( prevVal + ( numUe * RG_SCH_CMN_BI_NUMUE_FACTOR ))
#define RG_SCH_CMN_NUMUE_FACTOR    1
#define RG_SCH_CMN_BITBL_INDEX(x) ((x/RG_SCH_CMN_NUMUE_FACTOR)>=12)? 12:(x/RG_SCH_CMN_NUMUE_FACTOR)
#define RG_SCH_CMN_GET_BI(numUe)  rgSchCmnBiTbl[RG_SCH_CMN_BITBL_INDEX((numUe))]
/* RACHO end */

#define RG_SCH_CMN_SVC_IS_GBR(svc) ((((RgSchCmnDlSvc*)(svc->sch))->prio >= RG_SCH_CMN_DL_GBR_PRIO_START) && \
                                (((RgSchCmnDlSvc*)(svc->sch))->prio <= RG_SCH_CMN_DL_GBR_PRIO_END))

#define RG_SCH_CMN_SVC_IS_AMBR(svc) ((((RgSchCmnDlSvc*)(svc->sch))->prio > RG_SCH_CMN_DL_GBR_PRIO_END) && \
                                (((RgSchCmnDlSvc*)(svc->sch))->prio <= RG_SCH_CMN_MAX_PRIO))

#define RG_SCH_CMN_TBS_TO_MODODR(x, y) do {\
   if (x <= 9) y = 2; \
   else if (x <= 15) y = 4; \
   else y = 6;\
} while(0)

/* rg001.201:107387: To include the length and ModOrder in DataRecp Req. */
#define RG_SCH_UL_MCS_TO_MODODR(x, y) do {\
   y = (TfuModScheme)rgUlIMcsTbl[x].qm;\
} while(0)

#define RG_SCH_CMN_ITBS_TO_RETX_IMCS(iTbs, iMcs) do {\
   if ((iTbs) <= 9) (iMcs) = 29; \
   else if ((iTbs) <= 15) (iMcs) = 30; \
   else (iMcs) = 31;\
} while(0)

/* MS_FIX for ccpu00123919: In case of RETX TB scheduling avoiding recomputation of RB
 * and Tbs. Set all parameters same as Init TX except RV(only for NACKED) and
 * MCS.  */
#define RG_SCH_CMN_GET_MCS_FOR_RETX(tb, retxMcs) do {\
      if ((tb->isAckNackDtx == TFU_HQFDB_DTX)) { \
         retxMcs = tb->dlGrnt.iMcs; \
      } \
      else { \
	 if (tb->dlGrnt.iMcs < 29) {\
            U8 _iTbs;\
            RG_SCH_CMN_MCS_TO_TBS(tb->dlGrnt.iMcs, _iTbs);\
            RG_SCH_CMN_ITBS_TO_RETX_IMCS(_iTbs, retxMcs); \
         } \
	 else {\
            retxMcs = tb->dlGrnt.iMcs; \
	 }\
      }\
}while(0)

   /* rg004.201 patch - ccpu00110457 DTX Change */
#define RG_SCH_CMN_TBS_TO_MCS_DTX(proc, iTbs, imcs) do {\
      if ((proc->isAckNackDtx == TFU_HQFDB_DTX)) { \
         RG_SCH_CMN_TBS_TO_MCS(iTbs, imcs); \
      } \
      else { \
         RG_SCH_CMN_ITBS_TO_RETX_IMCS(iTbs, imcs); \
      } \
}while(0)
   /* rg004.201 patch - ccpu00110457 DTX end */

#define RG_SCH_CMN_UL_IS_CQI_VALID(cqi) ((cqi) > 0 && (cqi) < RG_SCH_CMN_UL_NUM_CQI)

#ifdef CCPU_OPT
#define RG_SCH_CMN_DL_GET_HDR_EST(svc, hdrEst) do {\
   hdrEst  = svc->estRlcHdrSz;\
   hdrEst += RG_SCH_CMN_DED_MAX_HDRSIZE * RG_SCH_CMN_MAX_DED_SDU;\
   if (svc->staPduPrsnt) \
   {\
      hdrEst += RG_SCH_CMN_DED_MAX_HDRSIZE;\
   }\
} while(0)
#else
#define RG_SCH_CMN_DL_GET_HDR_EST(svc, hdrEst) do {\
   hdrEst = RG_SCH_CMN_DED_MAX_HDRSIZE * RG_SCH_CMN_MAX_DED_SDU;\
} while(0)
#endif

#define RG_SCH_CMN_MAX_UL_CONTRES_GRNT   4
#define RG_SCH_CMN_DL_PRIOS              RG_SCH_CMN_MAX_PRIO + 1
#define RG_SCH_CMN_MAX_ALLOC_TRACK       10
/* rg006.201:ccpu00112396 Removed dependency on TFU_DELTA */
/* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for UL control in FDD*/
#ifdef LTE_TDD
#define RG_SCH_CMN_MIN_BSR_RECP_INTRVL   ((4 - (RG_UL_DELTA + 1)) + 4)
#else
#define RG_SCH_CMN_MIN_BSR_RECP_INTRVL   (TFU_ULCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA)
#endif
/* rg004.201: ccpu00105914: PHR handling for MSG3 */
#define RG_SCH_CMN_MIN_MSG3_RECP_INTRVL  RG_SCH_CMN_DL_DELTA + RGSCH_RARSP_MSG3_DELTA
/* rg002.301 -MOD-[ccpu00118351] Introduced timing delta for DL control in FDD */
/* This interval RG_SCH_CMN_MIN_RETXMSG3_RECP_INTRVL is used in FDD only */
#ifndef LTE_TDD
#define RG_SCH_CMN_MIN_RETXMSG3_RECP_INTRVL (TFU_DLCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA)
#endif
/*rg002.301: ccpu00120621-MOD-fixing the priority table to be more in line
 *with the spec 23.203,table 6.1.7 */
#define RG_SCH_CMN_QCI_TO_PRIO           {1, 2, 1, 2, 0, 3, 3, 4, 5}
/* rg003.301[ccpu00121749] -ADD-Not validating DlQoc for DCCH */
#define RG_SCH_CMN_DCCH_PRIO             0
#define RG_SCH_CMN_DL_GBR_PRIO_START     1
#define RG_SCH_CMN_DL_GBR_PRIO_END       2
/*rg002:ccpu00120621-MOD-introduced min & max qci
 * for validation of qci */
#define RG_SCH_CMN_MIN_QCI               1
#define RG_SCH_CMN_MAX_QCI               9
#define RG_SCH_CMN_NUM_QCI               9
#define RG_SCH_CMN_MAX_CP                2
#define RG_SCH_CMN_NOR_CP                0
#define RG_SCH_CMN_EXT_CP                1
#define RG_SCH_CMN_MAX_CFI               4
#define RG_SCH_CMN_NUM_TBS               27
#define RG_SCH_CMN_MAX_CQI               16
#define RB_SCH_CMN_NUM_SCS_PER_RB        12
#define RG_SCH_CMN_NUM_RBS               110
#define RG_SCH_CMN_UL_NUM_SF             RGSCH_NUM_UL_HQ_PROC+8
#define RG_SCH_CMN_UL_NUM_RE_PER_RB(cell) ((cell)->ulNumRePerRb)  
#ifdef LTE_TDD
#define RG_SCH_CMN_MAX_CMN_PDCCH 6
#else
#define RG_SCH_CMN_MAX_CMN_PDCCH 4
#endif
#define RG_SCH_CMN_UL_MAX_CQI            16
#define RG_SCH_CMN_UL_SR_BYTES           1
/* Refresh Timer Defines */
/* MS_WORKAROUND : syed tuning refresh time to 100ms for PFS */
#define RG_SCH_CMN_REFRESH_TIME 32 /* Refresh time/cycle in frames (10ms) */
/* MS_FIX: syed align multiple UEs to refresh at same time */
#define RG_SCH_CMN_REFRESH_TIMERES 10
#define RG_SCH_CMN_NUM_REFRESH_Q    16
#define RG_SCH_CMN_EVNT_UE_REFRESH  1

#define RG_SCH_CMN_TPC_ACC_DEC_THRESHOLD  1
#define RG_SCH_CMN_TPC_ACC_INC_1DB_THRESHOLD  1
#define RG_SCH_CMN_TPC_ACC_INC_3DB_THRESHOLD  3
#define RG_SCH_CMN_TPC_ABS_DEC_4DB_THRESHOLD  4
#define RG_SCH_CMN_TPC_ABS_DEC_1DB_THRESHOLD  1
#define RG_SCH_CMN_TPC_ABS_INC_1DB_THRESHOLD  4
#define RG_SCH_CMN_TPC_ABS_INC_4DB_THRESHOLD  4

/* ccpu00117606 - ADD - Include CRS REs while calculating Efficiency */
#define RG_SCH_CMN_MAX_ANT_CONF               3
#define RG_SCH_CMN_NUM_SLOTS_PER_SF           2
/* ccpu00117606 - ADD - Defines for Effective Cell RS for different Tx Ant Ports */
#define RG_SCH_CMN_EFF_CRS_ONE_ANT_PORT       6
#define RG_SCH_CMN_EFF_CRS_TWO_ANT_PORT       12
#define RG_SCH_CMN_EFF_CRS_FOUR_ANT_PORT      16

/*rg002.301:ccpu00120274-ADD-new defines for Min & Max RI values */
#define RG_SCH_CMN_MIN_RI 1 
#define RG_SCH_CMN_MAX_RI 4 

#define RG_SCH_CMN_IS_RI_VALID(ri) \
   (ri >= RG_SCH_CMN_MIN_RI && ri <= RG_SCH_CMN_MAX_RI)


#define RGSCHCMNADDTOCRNTTIME(crntTime, toFill, incr) \
   if ((crntTime.subframe + incr) > (RGSCH_NUM_SUB_FRAMES - 1))\
      toFill.sfn = (crntTime.sfn + 1) % RGSCH_MAX_SFN;\
   else \
      toFill.sfn = crntTime.sfn; \
   toFill.subframe = (crntTime.subframe + incr) % RGSCH_NUM_SUB_FRAMES;

#define RG_SCH_CMN_RARSP_WAIT_PRD   3

#define RG_SCH_CMN_MAX_SPL_CFI      2
#define RG_SCH_CMN_INVALID_INFO     0xff
#define RG_SCH_CMN_NUM_SUBCAR       12

#define RG_SCH_CMN_SUBFRM_0      0
#define RG_SCH_CMN_SPL_SUBFRM_1  1
#define RG_SCH_CMN_SUBFRM_5      5
#define RG_SCH_CMN_SPL_SUBFRM_6  6

#define RG_SCH_CMN_VALUE_ONE 1

#define RG_SCH_CMN_IS_ODD(x) ((x) & 0x01)

#define RG_SCH_CMN_MAX_NUM_OF_SFN 10240
#define RG_SCH_CMN_MAX_SFN_NUM 1023

#define RG_SCH_CMN_NUM_DL_AT_SWTCHPT   2

#define RG_SCH_CMN_10_MS_PRD      10
#define RG_SCH_CMN_5_MS_PRD       5
/* Interval between Transmission and feedback */
#define RG_SCH_CMN_HARQ_INTERVAL  4   

#define RG_SCH_CMN_DECR_FRAME(_x, _y) {\
   S16 _tmpNo = (_x) - (_y); \
   if(_tmpNo < 0) { \
      (_x) = _tmpNo + RGSCH_MAX_SFN; \
   }\
   else {\
      (_x) = _tmpNo; \
   }\
}
/*rg002.301:ccpu00120703-ADD-new hash define for max msg3 mcs val */
#define RG_SCH_CMN_MAX_MSG3_IMCS 15

/* Based on RG_SCH_CMN_DL_DELTA this should be updated */
#define RG_SCH_CMN_AN_FDBK_TBLSZ_INIT(cell) {\
   switch(RG_SCH_CMN_DL_DELTA) \
   { \
      case 1: \
      case 2: \
      case 3: \
         rgSchTddANFdbkMapTbl[cell->ulDlCfgIdx] += 1; \
         break; \
      default: \
         break; \
   } \
}

#define RG_SCH_CMN_CALC_RARSPLST_SIZE(cell, raArrsz) {\
   S16 _sfNum=0; \
   /* Get the last UL subframe no */ \
   for(_sfNum=RGSCH_NUM_SUB_FRAMES-1; _sfNum >= 0; _sfNum--) \
   { \
      if(rgSchTddUlDlSubfrmTbl[(cell)->ulDlCfgIdx][_sfNum] == \
            RG_SCH_TDD_UL_SUBFRAME) \
      { \
         break; \
      } \
   } \
   (raArrSz) = (_sfNum + ((RgSchCmnCell *)(cell)->sc.sch)->dl.numRaSubFrms \
                + RG_SCH_CMN_RARSP_WAIT_PRD +  \
                (cell)->rachCfg.raWinSize - 1) / RGSCH_NUM_SUB_FRAMES + 1; \
}

/* Resource allocation type MACROs */
#define RG_SCH_CMN_RA_TYPE0 0
#define RG_SCH_CMN_RA_TYPE1 1
#define RG_SCH_CMN_RA_TYPE2 2
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
/* RBG subset MACROs for RA type 1 */
#define RG_SCH_CMN_RBG_SUBSET0 0
#define RG_SCH_CMN_RBG_SUBSET1 1
#define RG_SCH_CMN_RBG_SUBSET2 2
#define RG_SCH_CMN_RBG_SUBSET3 3
#define RG_SCH_CMN_DL_NUM_ALLOCMASK 9
#define RG_SCH_CMN_SPS_MAX_PRD 640
#define RG_SCH_CMN_SPS_DL_ACTV   (1 << 0)
#define RG_SCH_CMN_SPS_DL_REACTV_FREQ (1 << 1)
#define RG_SCH_CMN_SPS_DL_REACTV_TIME (1 << 2)
#define RG_SCH_CMN_SPS_DL_REACTV \
(RG_SCH_CMN_SPS_DL_REACTV_FREQ | RG_SCH_CMN_SPS_DL_REACTV_TIME) 
#define RG_SCH_CMN_SPS_DL_REL    (1 << 3)
#define RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE 4
/* Number of 32 bit bitmasks for marking measurement gap for SPS */
#define RG_SCH_CMN_SPS_DL_MEASGAP_32BITMASK_SIZE 3  

/* 32 Bit mask size for n1Pucch: RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF/32 */
#define RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE \
   ((RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF + 31)/32)
#define RG_SCH_CMN_SPS_DL_INVALIDCQI_VAL 20

/* Maximum number of feasible periodicity values for SPS, SRS, CQI and SR */
#define RG_SCH_CMN_SPS_MAX_NUM_PRD 21

/* Maximum value of iMcs for SPS UE */
#define RG_SCH_CMN_SPS_DL_MAX_MCS 15

/* DL SPS function MACROs */

#define RG_SCH_CMN_SPS_GET_DL_CELL(cell) &(((RgSchCmnCell *)((cell)->sc.sch))->dl.dlSpsInfo)
#define RG_SCH_CMN_SPS_GET_DL_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->dl.dlSpsInfo)
#define RG_SCH_CMN_SPS_GET_DL_SVC(svc) &(((RgSchCmnDlSvc *)((svc)->sch))->dlSvcSpsInfo)
#define RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP) (((RgSchCmnDlHqProc *)((hqP)->sch))->isSpsSvcSchd)
#define RG_SCH_CMN_IS_UE_SPS_SCHDLD(_ue, _schdTime)\
(((_ue)->dl.dlAllocCb.spsSchdTime.sfn == _schdTime.sfn) &&\
 ((_ue)->dl.dlAllocCb.spsSchdTime.subframe == _schdTime.subframe))

#ifdef LTE_TDD
#define RG_SCH_CMN_INCR_DAI(_dai, _incr) \
      ((_dai  + _incr - 1) % RG_SCH_MAX_DAI_IDX + 1)
#endif

#define RG_SCH_CMN_DL_COUNT_ONES(_bitMask, _size, _numOnes)\
{\
   U8 _pos = 0;\
   *_numOnes = 0;\
   for (_pos = 0; _pos < _size; ++_pos)\
   {\
      *_numOnes += (_bitMask & (1 << (31 - _pos))) ? 1: 0;\
   }\
}

#define RG_SCH_CMN_DL_GET_START_POS(_allocedBitmask, _size, _startPos)\
{\
   U8 _pos = 0;\
   for (_pos = 0; _pos < _size; ++_pos)\
   {\
      if ((_allocedBitmask & (1 << (31 -_pos))))\
      {\
         continue;\
      }\
      else\
      {\
         *_startPos = _pos;\
         break;\
      }\
   }\
}

/* This macros returns position of idx in a 32 bit bitmask from LSB */
#define RG_SCH_CMN_DL_GET_POS_FRM_LSB(_idx) (31 - (_idx)) 
   
#define RG_SCH_CMN_SPS_GET_PRD_IDX(_prdVal, _prdIdx)\
{\
   switch (_prdVal)\
   {\
      case 2: *_prdIdx = RG_SCH_CMN_SPS_PRD_2SF; break;\
      case 5: *_prdIdx = RG_SCH_CMN_SPS_PRD_5SF; break;\
      case 10: *_prdIdx = RG_SCH_CMN_SPS_PRD_10SF; break;\
      case 20: *_prdIdx = RG_SCH_CMN_SPS_PRD_20SF; break;\
      case 30: *_prdIdx = RG_SCH_CMN_SPS_PRD_30SF; break;\
      case 32: *_prdIdx = RG_SCH_CMN_SPS_PRD_32SF; break;\
      case 40: *_prdIdx = RG_SCH_CMN_SPS_PRD_40SF; break;\
      case 60: *_prdIdx = RG_SCH_CMN_SPS_PRD_60SF; break;\
      case 64: *_prdIdx = RG_SCH_CMN_SPS_PRD_64SF; break;\
      case 80: *_prdIdx = RG_SCH_CMN_SPS_PRD_80SF; break;\
      case 120: *_prdIdx = RG_SCH_CMN_SPS_PRD_120SF; break;\
      case 128: *_prdIdx = RG_SCH_CMN_SPS_PRD_128SF; break;\
      case 160: *_prdIdx = RG_SCH_CMN_SPS_PRD_160SF; break;\
      case 256: *_prdIdx = RG_SCH_CMN_SPS_PRD_256SF; break;\
      case 320: *_prdIdx = RG_SCH_CMN_SPS_PRD_320SF; break;\
      case 512: *_prdIdx = RG_SCH_CMN_SPS_PRD_512SF; break;\
      case 640: *_prdIdx = RG_SCH_CMN_SPS_PRD_640SF; break;\
      case 1024: *_prdIdx = RG_SCH_CMN_SPS_PRD_1024SF; break;\
      case 1280: *_prdIdx = RG_SCH_CMN_SPS_PRD_1280SF; break;\
      case 2048: *_prdIdx = RG_SCH_CMN_SPS_PRD_2048SF; break;\
      case 2560: *_prdIdx = RG_SCH_CMN_SPS_PRD_2560SF; break;\
      default:   *_prdIdx = RG_SCH_CMN_SPS_PRD_INVALID;break;\
   }\
}
/* To be part of rg_env.h */
/* Maximum n1Pucch values per sub-frame: multiple of 32 */
#define RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF  96 
#define RG_SCH_SPS_DFLT_PRD 20
#define RG_SCH_SPS_CQI_DECR_VAL 2

#ifdef LTE_TDD
/*rg009.201- added RgrSpsPrd casting to overcome G++ compilation warning*/
#define RGSCH_SPS_GET_PRDCTY(_prdEnum, _prd) \
{ \
   switch((_prdEnum)) \
   { \
      case RGR_SPS_PRD_10SF: \
         (_prd) = (RgrSpsPrd)10; \
         break; \
      case RGR_SPS_PRD_20SF: \
         (_prd) = (RgrSpsPrd)20; \
         break; \
      case RGR_SPS_PRD_32SF: \
         (_prd) = (RgrSpsPrd)30; \
         break; \
      case RGR_SPS_PRD_40SF: \
         (_prd) = (RgrSpsPrd)40; \
         break; \
      case RGR_SPS_PRD_64SF: \
         (_prd) =(RgrSpsPrd)60; \
         break; \
      case RGR_SPS_PRD_80SF: \
         (_prd) = (RgrSpsPrd)80; \
         break; \
      case RGR_SPS_PRD_128SF: \
         (_prd) = (RgrSpsPrd)120; \
         break; \
      case RGR_SPS_PRD_160SF: \
         (_prd) = (RgrSpsPrd)160; \
         break; \
      case RGR_SPS_PRD_320SF: \
         (_prd) = (RgrSpsPrd)320; \
         break; \
      case RGR_SPS_PRD_640SF: \
         (_prd) = (RgrSpsPrd)640; \
         break; \
      default:\
         (_prd) = RGR_SPS_PRD_INVALID;\
   } \
}
#endif /*LTE_TDD*/
/*rg002.301 ccpu00119494-ADD-hash define for actual transmission time */
/* Feedback for RelPdcch should be received at MAC by HARQ_INTERVAL+
 * RG_TFU_HQ_IND_DELTA time.*/
#define RG_SCH_CMN_SPS_TX_TIME (RG_SCH_CMN_HARQ_INTERVAL + TFU_HQFBKIND_ULDELTA)
#endif /* LTEMAC_SPS */

/* DL allocation MACROs */
#define RG_SCH_CMN_GET_ALLOCCB_FRM_UE(_ue) &((_ue)->dl.dlAllocCb); 
#define RG_SCH_CMN_GET_ALLOCCB_FRM_RACB(_raCb) &((_raCb)->rbAllocInfo); 
#define RG_SCH_CMN_INIT_SCHD_LNK(_schdLstLnk, _node)\
{\
      (_schdLstLnk)->node = (PTR)_node;\
      (_schdLstLnk)->prev = NULLP;\
      (_schdLstLnk)->next = NULLP;\
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
#define RG_SCH_CMN_FILL_DL_TXINFO(_allocInfo, _rb, _sFlg, _prcInf, _numLyr, _sf)\
{\
   _allocInfo->rbsReq                    = _rb;\
   _allocInfo->mimoAllocInfo.swpFlg      = _sFlg;\
   _allocInfo->mimoAllocInfo.precIdxInfo = _prcInf;\
   _allocInfo->mimoAllocInfo.numTxLyrs   = _numLyr;\
   _allocInfo->dlSf                      = _sf;\
}

#define RG_SCH_CMN_FILL_DL_TBINFO(_tbInfo, _bytsReq, _iTbs, _imcs, _tbCb, _noLyr)\
{\
   (_tbInfo)->schdlngForTb = TRUE;\
   (_tbInfo)->bytesReq     = _bytsReq;\
   (_tbInfo)->iTbs         = _iTbs;\
   (_tbInfo)->imcs         = _imcs;\
   (_tbInfo)->tbCb         = _tbCb;\
   (_tbInfo)->noLyr        = _noLyr;\
}

/* Bit Masks to Force Transmit diversity scheme */
#define RG_SCH_CMN_TD_RI_1         0x01 /* Transmit Diversity due to RI==1 in case
                                           of TM3 */
#define RG_SCH_CMN_TD_NO_PMI       0x02 /* Transmit Diversity due to No PMI */
#define RG_SCH_CMN_TD_TXMODE_RECFG 0x04 /* Transmit Diversity due to TXMODE ReCfg */


#define RG_SCH_MAX_UL_TX_ANT 2

/*rg001.301-ADD-[ccpu00117270]START */
/*Maximum achievable code rate for non 64QAM UEs.
 *Value should NEVER be > than 93. Refer to 36.213, Table 7.2.3-1*/
#define RG_SCH_CMN_MAX_CODE_RATE_16QAM    85  /* 85% code rate*/
#define RG_SCH_CMN_MAX_EFF_BITS         4096

/*New macro to determine UE Category. We use the stored "ueCat" to 
 * index a UE category array. Therefore, the stored ueCat is 1 less
 * than actual UE cateogry.*/
#define RG_SCH_CMN_GET_UE_CTGY(ue) ((RG_SCH_CMN_GET_UE(ue))->cmn.ueCat + 1)
/*ccpu00117270-ADD-END*/
   
#define RG_SCH_CMN_UPD_RBS_TO_ADD(_CELL,_DLSF,_ALLCINFO,_RBSREQ,_RBSTOADD) \
{\
          U8  addtlRbsAvl;\
          addtlRbsAvl = rgSCHCmnFindNumAddtlRbsAvl(_CELL,_DLSF,_ALLCINFO);\
          if(_RBSREQ > addtlRbsAvl)\
          {\
              _RBSTOADD = addtlRbsAvl;\
          }\
          else\
          {\
              _RBSTOADD = _RBSREQ;\
          }\
}
   /* ccpu00126002 ADD macro added to check wrap around when index is reached 
   MAX_CQI_RI_RPT_BUFF*/
#define RG_SCH_INCR_CQIRI_INDEX(index)\
{\
   (index)++;\
   if(MAX_CQI_RI_RPT_BUFF == index)\
   {\
      index = 0;\
   }\
}

#ifdef __cplusplus
}
#endif
#endif /* __RGSCHCMNH__ */

/********************************************************************30**

         End of file:     rg_sch_cmn.h@@/main/2 - Sat Jul 30 02:22:02 2011

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
------------ -------- ---- ----------------------------------------------
/main/1      ---       sm          1. LTE MAC 2.1 release
           rg001.201   sm          1. ccpu00106531: fixing incorrect 
                                      Imcs derivation. Inappropriate name 
                                      of CQI to ITbs table for DL has been 
                                      corrected.
                                   2. ccpu00107387: To include the length 
                                      and ModOrder in DataRecp Req.   
           rg004.201   sd          1. ccpu00105914: PHR handling for MSG3
           rg005.201   sm          1. Changed the check for uplink harq proc
                                      reTx. 
           rg006.201   sd          1. ccpu00112396: Removed dependency on 
                                      TFU_DELTA
           rg007.201   ap          1. Added support for MIMO
           rg008.201   sd          1. Removed dependency on MIMO compile-time flag
                       rsharon     2. Added support for SPS.
           rg009.201   avivek      1. Added RgrSpsPrd casting to overcome G++ 
                                      compilation warning      
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
           rg001.301   asehgal      1.Added the definitions for maximum achievable non 64QAM UEs.
           rg002.301   avivek       1.changes for ccpu00109543
           rg001.301   asehgal      1.Added the definitions for maximum 
                                      achievable non 64QAM UEs. 
                                    2. ccpu00119494: added a define for delta 
                                       of actual TX time in SPS.
                                    3. ccpu00120274: added new hash defines 
                                       for min & max values of RI.
                                    4. ccpu00120703: added new hash define
                                       for max mcs for msg3
                                    5. ccpu00120621-MOD-fixing the priority 
                                       table to be more in line with the spec.
                                    6. ccpu00120621-hash defines for max & min
                                       values of qci.
                                    7. ccpu00118351-seperate deltas for UL & DL.
*********************************************************************91*/
