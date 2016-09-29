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
 
    Name:   RRC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE-MAC
 
    File:   rg.h
 
    Sid:      rg.h@@/main/3 - Sat Jul 30 02:22:00 2011
 
    Prg:    st
 
**********************************************************************/
 
/** @file rg.h
@brief This file contains definitons for Mac.
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
 
#ifndef __RGH__
#define __RGH__

#define RGLAYERNAME     "MAC"
#define RG_MAX_TB_PER_UE 2
#define RG_MAX_BCCH_DLSCH 2
#define RG_INVALID_RNTI   0x0000

#define RG_MAX_NUM_DED_LC 10       /* maximum dedicated logical channels in a UE */
#define RG_MAX_NUM_CMN_LC 5       /* maximum number of common logical 
                                     channels in a cell */
#define RG_MAX_LCG_PER_UE 4
#define RG_CON_RES_ID_SZ  6
#define RG_DELTA_MAC_PHY  1
#define RG_MAX_RA_RNTI   60
#define RG_MAX_CMN_LC_BUF RG_DELTA_MAC_PHY

/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
#define RG_ADDL_SPS_OCC_FOR_EXP_REL 2
#endif

/* This is the delta that MAC works on Schedule's DELTA should be a function of
 * this.
 */

/* Well known RNTIS */
#define RG_SI_RNTI        0xffff
#define RG_P_RNTI         0xfffe

#define RG_MAX_UPPERSAP               3
#define RG_MAX_LOWERSAP               1 
#define RG_TQ_SIZE                    10   /* Timing Queue Size */
#define RG_MAX_TIMER     RG_MAX_LOWERSAP   /* MAX number of MAC timers */
#define RG_NMB_CELL_HASHBIN           10   /* Number of Hash Bins for cell hash
                                              table */
#define RG_NMB_UE_HASHBIN             10   /* Number of Hash Bins for UE hash
                                              table */        
#define RG_BNDREQ_TMR    1    /* Bind Request timer event value */
#define RG_MAX_BNDRETRY  2    /* Max number of Bind Retries for TFU SAP */


#define RG_UE_TQ_SIZE    10   /* Timing Queue Size */

#define RG_INVALID_SCHD_TYPE 255
#define RG_MAX_UE_PER_CELL 0xFFFF /*!< Maximum number of UEs per cell */
/* rg005.201 Changing from 31 to 128 for optimization with more number of UEs */
#define RG_MAX_UE_BIN_PER_CELL 128 /*!< Maximum number of UE bins per cell */
#define RG_MIN_DL_BW       6    /*!< Minimum Downlink bandwidth in RBs */
#define RG_MAX_DL_BW       110  /*!< Maximum Downlink bandwidth in RBs */
#define RG_MIN_UL_BW       6    /*!< Minimum Uplink bandwidth in RBs */
#define RG_MAX_UL_BW       110  /*!< Maximum Uplink bandwidth in RBs */
#define RG_MIN_CFI_VAL     1    /*!< Minimum value for CFI */
#define RG_MAX_CFI_VAL     3    /*!< Maximum value for CFI */
#define RG_QM_BPSK         2    /*!< Qm value for BPSK */
#define RG_QM_QPSK         4    /*!< Qm value for QPSK */
#define RG_QM_64QAM        6    /*!< Qm value for 64QAM */
#define RG_MIN_SRS_SFCFG_IDX 0  /*!< Minimum value for SRS subframe 
                                     configurtion index */
#define RG_MAX_SRS_SFCFG_IDX 15  /*!< Maximum value for SRS subframe 
                                     configurtion index */
#define RG_MAX_SRS_TX_OFFSET 8    /*!< Maximum number of SRS transmission 
                                    offsets per cell */
#define RG_MIN_MAC_RNTI      10   /*!< Minimum value of RNTI to be managed by 
                                    MAC */

/* HARQ related MACROs */
#define RG_NUM_DL_HQ_PROC 8
#define RG_NUM_UL_HQ_PROC 8
#define RG_MIN_HQ_TX 1

/* Group power related MACROs */
#define RG_MAX_GRP_PWR_FMT3_IDX  15   /*!< Maximum index value for group power format 3 */
#define RG_MAX_GRP_PWR_FMT3A_IDX 31   /*!< Maximum index value for group power format 3A */

/* MACROs to indicate cell specific config for cell to be active */
#define RG_BCCH_BCH_CFG_DONE      (1<<0)
#define RG_BCCH_DLSCH_CFG1_DONE    (1<<1)
#define RG_BCCH_DLSCH_CFG2_DONE    (1<<2)
#define RG_PCCH_CFG_DONE          (1<<3)
#define RG_UL_CCCH_CFG_DONE       (1<<4)
#define RG_DL_CCCH_CFG_DONE       (1<<5)
#define RG_SCHD_CFG_DONE          (1<<6)
#define RG_CELL_ACTIVE (RG_BCCH_BCH_CFG_DONE | RG_BCCH_DLSCH_CFG1_DONE |  RG_BCCH_DLSCH_CFG2_DONE | RG_PCCH_CFG_DONE | RG_UL_CCCH_CFG_DONE | RG_DL_CCCH_CFG_DONE)
/* Logical channel realated MACROs */
#define RG_INVALID_LCG_ID  255
#define RG_INVALID_LC_ID   255
#define RG_BCCH_BCH_IDX   0
#define RG_BCCH_DLSCH_IDX 1
#define RG_PCCH_IDX 2

/*rg002.301:ccpu0010621-ADD-constants for MIN & MAX qci*/
#define RG_QCI_MIN 1
#define RG_QCI_MAX 9

/* PUCCH related macros */
#define RG_PUCCH_MAXVAL_CS    7   /*!< Maximum value for cyclic shift of PUCCH */
#define RG_PUCCH_MINVAL_DS    1   /*!< Mininmum value for delta shift of PUCCH */
#define RG_PUCCH_MAXVAL_DS    3   /*!< Maximum value for delta shift of PUCCH */

/* DUX related macros */
#define RG_LCID_MASK 0x1F
#define RG_LCID_LEN 0x5
#define RG_CCCH_LCID 0x00
#define RG_DEDLC_MIN_LCID 0x01
#define RG_DEDLC_MAX_LCID 0x0A
#define RG_RES_MIN_LCID 0x0B
#define RG_RES_MAX_LCID 0x19
#define RG_PHR_LCID 0x1A
#define RG_CRNTI_LCID 0X1B
#define RG_TRUNC_BSR_LCID 0X1C
#define RG_SHORT_BSR_LCID 0X1D
#define RG_LONG_BSR_LCID  0X1E
#define RG_PAD_LCID 0x1F
#define RG_MAX_EXTN_PAD_SUBHDRS 0x02

#define RG_CCCH_SDU_PRSNT     (1<<0)
#define RG_CRNTI_CE_PRSNT     (1<<1)
#define RG_PHR_CE_PRSNT       (1<<2)
#define RG_TRUNC_BSR_CE_PRSNT (1<<3)
#define RG_SHORT_BSR_CE_PRSNT (1<<4)
#define RG_LONG_BSR_CE_PRSNT  (1<<5)

/* LOGICAL CHANNEL */
#define RG_MAX_LC_PER_UE   10
/* Maximum number of common logical channel control blocks */
#define RG_MAX_CMN_LC_CB   3
#define RG_MAX_BCCH   2

#define RG_OPTM_NUM_DED_LC    3

/* Random access related MACROs */
#define RG_MAX_RA_PREAMBLE_FMT 3 /*!< Maximun value of Random access preamble 
                                      format */
#define RG_MAX_RA_WINSIZE    10  /*!< Maximum size of Random access response 
                                      window in subframes */
#define RG_MIN_RA_WINSIZE    2   /*!< Minimum size of Random access response 
                                      window in subframes */
#define RG_MIN_NUM_RA_PREAMBLE 4 /*!< Minimum number of Random access 
                                      preambles */
#define RG_MAX_NUM_RA_PREAMBLE 64 /*!< Maximim number of Random access 
                                      preambles */
#define RG_NUM_RA_RB     6

#define RG_MAX_RA_RSP_ALLOC    4 /*!< Maximum number of Random access
                                      allocations */
#define RG_RARSP_MSG3_DELTA    6
#define RG_PDCCH_PUSCH_DELTA   4


#define RG_CRG_CFG 1          /* CRG configuration element */
#define RG_RGR_CFG 2          /* RGR configuration element */

#define RG_NUM_ITBS         27
#define RG_MAX_NUM_RB       110
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
#define RG_MAX_LYR_PERCW    2

#define RG_LCG_ISCFGD(lcg) ((lcg)->lcgId != RG_INVALID_LCG_ID)
/* rg001.101: Corrected the check for dlCcchId */
#define RG_DLCCCH_ISCFGD(cell) ((cell)->dlCcchId != RG_INVALID_LC_ID)
#define RG_ULCCCH_ISCFGD(cell) ((cell)->ulCcchId != RG_INVALID_LC_ID)
/* After merging from 2.1 to 2.2 */
#define RG_CALC_SF_DIFF(_time1, _time2)\
   (_time1.sfn*10+_time1.subframe) < (_time2.sfn*10+_time2.subframe)?\
     ((_time1.sfn+RG_MAX_SFN)*10+_time1.subframe) -\
       (_time2.sfn*10+_time2.subframe) : \
     (_time1.sfn*10+_time1.subframe) - (_time2.sfn*10+_time2.subframe)

#define RGADDTOCRNTTIME(crntTime, toFill, incr)          \
   if ((crntTime.subframe + incr) > (RG_NUM_SUB_FRAMES - 1))   \
      toFill.sfn = (crntTime.sfn + 1) % RG_MAX_SFN;      \
   else                                                  \
      toFill.sfn = crntTime.sfn;                              \
   toFill.subframe = (crntTime.subframe + incr) % RG_NUM_SUB_FRAMES;

#define RGSUBFRMCRNTTIME(crntTime, toFill, dcr)    \
{                                                  \
   if (crntTime.sfn == 0)                          \
{                                                  \
   if ((crntTime.subframe - (dcr)) < 0)              \
   {                                               \
      toFill.sfn = RG_MAX_SFN - 1;                 \
   }                                               \
   else                                            \
   {                                               \
   toFill.sfn = crntTime.sfn;                      \
   }                                               \
}                                                  \
else                                               \
{                                                  \
   if ((crntTime.subframe - (dcr)) < 0)              \
   {                                               \
   toFill.sfn = crntTime.sfn - 1;                  \
   }                                               \
   else                                            \
   toFill.sfn = crntTime.sfn;                      \
}                                                  \
toFill.subframe = (RG_NUM_SUB_FRAMES + crntTime.subframe - (dcr)) % (RG_NUM_SUB_FRAMES);                                   \
}

#define RGCPYTIMEINFO(src, dst)  \
   dst.sfn        = src.sfn;     \
   dst.subframe   = src.subframe;
#define RG_TIMEINFO_SAME(x, y) ((x.sfn == y.sfn) && (x.subframe == y.subframe))


#define rgPBuf  rgCb.rgInit.prntBuf

/* Debug Prints for MAC */
#ifdef DEBUGP
#define RGDBGPRM(_args)          \
                  DBGP(&rgCb.rgInit, RGLAYERNAME, DBGMASK_PRM, _args)
#define RGDBGERR(_args)          \
                  DBGP(&rgCb.rgInit, RGLAYERNAME, DBGMASK_ERR, _args)
#define RGDBGINFO(_args)         \
                  DBGP(&rgCb.rgInit, RGLAYERNAME, DBGMASK_INFO, _args)
#else
#define RGDBGPRM(_args) 
#define RGDBGERR(_args) 
#define RGDBGINFO(_args)
#endif /* #ifdef DEBUGP */


/* Macro to free the message buffer and initialize it to zero */
/***********************************************************
 *
 *     Name : RG_FREE_MSG
 *
 *     Desc : Macro to free the message buffer and initialize it to zero
 *            
 *     Input  : mBuf - message buffer pointer to be retunrned
 *
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/
#define RG_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      SPutMsg((_buf)); \
      _buf = NULLP; \
   } \
}
/***********************************************************
 *
 *     Name : RG_DROP_RGUDDATREQ_MBUF
 *
 *     Desc : Macro to free the message buffers and initialize them to zero
 *            
 *     Input  : _datreq - Dedicated Data Request pointer which has mBufs
 *              to be freed
 *     
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/

#define RG_DROP_RGUDDATREQ_MBUF(_datReq)\
{\
   U32 idx1,idx2,idx3;\
   if (_datReq != NULLP)\
   {\
      for (idx1=0; idx1 < _datReq->nmbOfTbs; idx1++)\
      {\
         for (idx2=0; idx2 < _datReq->datReqTb[idx1].nmbLch; idx2++)\
         {\
            for (idx3=0; \
               idx3 < _datReq->datReqTb[idx1].lchData[idx2].pdu.numPdu; \
               idx3++)\
            {\
               RG_FREE_MSG(_datReq->datReqTb[idx1].\
                        lchData[idx2].pdu.mBuf[idx3]);\
            }\
         }\
      }\
   }\
}
/***********************************************************
 *
 *     Name : RG_DROP_RGUCDATREQ_MBUF
 *
 *     Desc : Macro to free the message buffers and initialize them to zero
 *            
 *     Input  : _datreq - Common Data Request pointer which has mBufs
 *              to be freed
 *     
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/
#define RG_DROP_RGUCDATREQ_MBUF(_datReq)\
{\
   if (_datReq != NULLP)\
   {\
      RG_FREE_MSG(_datReq->pdu);\
   }\
}


/* Macros for memory region and pool determination */
#define RG_GET_MEM_REGION(rgCb)  (rgCb.rgInit.region)
#define RG_GET_MEM_POOL(rgCb)    (rgCb.rgInit.pool)


/* MUX related macros */
#define RG_RAR_SHDR_LEN                1
#define RG_RAR_ELEM_LEN                6
#define RG_MAX_SDU_SUB_HDR_LEN         3
#define RG_MAX_PAD_ARR_SZ              128 /* Changing from 100 to 128 for optimization */
#define RG_PAD_BYTE                    0x00

#define RG_HDR_TYPE_CRES                1
#define RG_HDR_TYPE_TA                  2

#define RG_SDU_SHDR_LEN 1
#define RG_CE_SHDR_LEN 1
#define RG_CRES_LEN    6
#define RG_TA_LEN      1
#define RG_CRES_ELM_LEN (RG_CE_SHDR_LEN+RG_CRES_LEN)
#define RG_TA_ELM_LEN   (RG_CE_SHDR_LEN+RG_TA_LEN)

#define RG_CRES_LCID_IDX               0x1C
#define RG_TA_LCID_IDX                 0x1D

/* Structure member offset computation macro    */
/*rg009.201- ccpu00116700- changed from U32 to PTR */
#define OffsetOf(type, field)                                                 \
         (PTR) (&(((type *) NULLP)->field))

#define RG_MAX_SUBFRAMES_IN_SFN        9
#define RG_MAX_SFN                     1024
#define RG_NUM_SUB_FRAMES              10

#ifdef LTE_L2_MEAS
#define RG_NUM_UL_SUB_FRAMES              16
#define RG_MAX_QCI_VALUE                  10
#ifdef LTEMAC_R9
#define RG_CALC_SF_DIFF(_time1, _time2)\
      (_time1.sfn*10+_time1.subframe) < (_time2.sfn*10+_time2.subframe)?\
     ((_time1.sfn+RG_MAX_SFN)*10+_time1.subframe) -\
       (_time2.sfn*10+_time2.subframe) : \
     (_time1.sfn*10+_time1.subframe) - (_time2.sfn*10+_time2.subframe)
#endif
#endif /* LTE_L2_MEAS */

/* Define for the block size for memory allocation */
/*rg009.201- ccpu00116498- RG_BLKSZ changed from 2048 to 1500*/
#define RG_BLKSZ                       1500

/* Defines for RGU Statistics types */
#define RG_RGU_SDU_DROP 1
#define RG_RGU_SDU_RCVD 2

/* MACROS for General Statistics */
#define RG_CFG_ADD      1
#define RG_CFG_DEL      2

#define RG_HQ_FDB_IND_CB_TYPE_HQ_ENT      1
#define RG_HQ_FDB_IND_CB_TYPE_RA_CB       2

/* 
 * rg002.301 -DEL-[ccpu00118351] Removed unused hash-define which defines the
 * index for releasing the subframe.
 */

/* Value used to set nDmrs in uplink grant if nDmrs is not applicable */
#define RG_INVALID_NDMRS  10

#define RG_SEND_TRC_IND(_mBuf, _event) rgLMMTrcInd(_mBuf, _event)

#endif /* __RGH__ */

/**********************************************************************
         End of file:     rg.h@@/main/3 - Sat Jul 30 02:22:00 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       st  1. Initial Release
/main/2      ---       st  1. LTE MAC 2.1 release
             rg005.201 sm  1. Modified values for optimization.
             rg007.201 gvj/ap  1. Added support for MIMO
             rg008.201 sd  1.Removed dependency on MIMO compile-time flag
                       rsharon 2.Added support for SPS.
             rg009.201 dv  1.ccpu00116498- Change in RG_BLKSZ
                           2.ccpu00116700- changed from U32 to PTR
/main/3      ---     gj                1. LTE MAC 3.1 Release.                           
           rg002.301  asehgal 1.ccpu0010621:hash defines for min & max qci 
                               defined.
                              2. ccpu00118351:seperate deltas for UL & DL.
*********************************************************************91*/
