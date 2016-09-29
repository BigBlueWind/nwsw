/********************************************************************16**

                         (c) COPYRIGHT 2009 by
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

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************

    Name:   LTE MAC Convergence layer

    Type:   C include file

    Desc:   Data Structures required by LTE MAC Convergence Layer for MSPD PHY

    File:   ys_ms.x

    Sid:

    Prg:    hs

**********************************************************************/

#ifndef __YS_MS_X__
#define __YS_MS_X__

/***************SEC_CHANGE_START****************/
#if 0
#ifdef CTF_AS_SEC_ENB
#include "spaccdrv.h"
#endif
#endif
/***************SEC_CHANGE_END****************/
/* Forward declarations */
typedef struct ysCb           YsCb;
typedef struct ysCellCb       YsCellCb;
typedef struct ysUeCb         YsUeCb;

/*ys004.102 :  Merged MSPD code with phy 1.7 */
typedef enum sfnType
{
   YS_ALL_SFN,
   YS_EVEN_SFN,
   YS_ODD_SFN
} YsSfnType;

/**
  * Control Block structure for Upper SAPs at CTF and TFU interfaces
  */
typedef struct ysCtfSapCb
{
   Pst           sapPst;          /* Post info associated with SAP */
   SpId          spId;            /* SpId associated with SAP */
   SuId          suId;            /* SuId associated with SAP */
   State         sapState;        /* SAP state. */
   YsCtfSapSts   ctfSts;          /* CTF SAP statistics */
   CtfCfgTransId transId;
}YsCtfSapCb;

typedef struct ysTfuSapCb
{
   YsTfuSapCfg tfuSapCfg;    /* SAP cfg received from LM */
   Pst         sapPst;       /* Post info associated with SAP */
   SpId        spId;         /* SpId associated with SAP */
   SuId        suId;         /* SuId associated with SAP */
   YsCellCb    *cellCb;      /* Cell Control Block */
   State       sapState;     /* SAP state. */
   YsTfuSapType type;        /* type of the SAP */
   YsTfuSapSts tfuSts;       /* TFU SAP statistics */
   YsL1PhySts  phySts;       /* message related statistics */


}YsTfuSapCb;

typedef struct ysDlTxMsgInfo
{
   TfuCntrlReqInfo *cntrlReq;
   TfuDatReqInfo   *datReq;
} YsDlTxMsgInfo;

typedef struct ysPhyTxSdu
{
   CmLList    lnk;
   U16        sduLen;
   PGENMSGDESC   txSduReq;
} YsPhyTxSdu;

typedef struct ysPhyLstCp
{
PMAC2PHY_QUEUE_EL head;
PMAC2PHY_QUEUE_EL tail;
U32               count;
} YsPhyLstCp;

typedef struct ysDlEncL1Msgs
{
   /* HARQ */
  /* PHIADCIULMSGDESC dci0HiVector; */
   YsPhyLstCp       dlData;  /* Linked lisd of DL data */
   YsPhyLstCp       dlCntrl; /* Linked list of DL cntrl */
   YsPhyLstCp       ulCntrl; /* Linked list of UL control: PHICH and DCI 0 */
   YsPhyLstCp       ulDlLst; /* Linked list of DL data and control of this air
                                subframe(n) and UL cntrl of previous subframe (n-1) */
   PGENMSGDESC      txVector;
   U32              numChannels;
   Bool             isdatReqPres;
   /*
    CmLListCp       txSduLst;  */ /* List of TXSDUREQ  */
} YsDlEncL1Msgs;

typedef struct ysUlMsgInfo
{
   TfuRecpReqInfo  *recpReq;
} YsUlMsgInfo;


typedef struct ysSrCfgInfo
{
   U8    srCfgIndex;             /*!< SR Config Index, (0..154) */
   U8    srPeriod;               /*!< SR periodicity */
   U8    srSfOffSet;             /*!< SR subframe ofset */
}YsSrCfgInfo;

typedef struct ysSrsCfgInfo
{
   U16    srsCfgIndex;            /*!< SRS Config Index, (0..636) */
   U16    srsPeriod;              /*!< SRS periodicity */
   U16    srsSfOffSet;            /*!< SRS subframe ofset */
}YsSrsCfgInfo;

typedef struct ysCqiPmiCfgIdxInfo
{
   U8    cqiPmiCfgIndex;         /*!< Cqi-PMI Config Index, (0..315) */
   U8    cqiPeriod;              /*!< Cqi periodicity */
   U8    cqiSfOffSet;            /*!< Cqi subframe ofset */
}YsCqiPmiCfgIdxInfo;


/* MS_FIX: tfu upgrade changes */
#ifndef TFU_UPGRADE
typedef struct ysUeSubFrInfo
{
   Bool                schPres;
   Bool                srPres;
   Bool                srsPres;
   Bool                cqiPres;
   Bool                onPucch;
   U8                  pucchFormat;

   TfuUeRecpReqInfo   *schReqInfo;
   TfuUeRecpReqInfo   *harqReqInfo;

} YsUeSubFrInfo;
#endif

/***************SEC_CHANGE_START****************/
#if 0
#ifdef CTF_AS_SEC_ENB
#if 1
#define YS_SEC_DIR_UL          0
#define YS_SEC_DIR_DL          1
#define LTE_CIPHER_KEY_LEN   16
#define LTE_INTEG_KEY_LEN    16
#define LTE_IV_LEN           16
#define YS_MS_HASH_KEY_SZ    16
#define YS_MS_HMAC_STR_SZ    16
#define YS_MS_KENB_LEN       32
#define YS_MS_KDF_OP_LEN     32
#define YS_MS_STR_S_LEN      7
#define BYTES_IN_WORD        4
#define LTE_IV_WORD_LEN      4
#define YS_MS_MAC_SIZE_BYTES 4
#define MAX_PRE_ALLOC_BUFF   30
#define SPACC_DDT_NODE_SIZE  768

/** @brief This enum defines the Hash Table State
 *         for MSPD SPAcc
 **/
typedef enum
{
    YS_MS_HASH_INIT,
    YS_MS_HASH_DEINIT
}YsMsSpaccHashState;

/** @brief This enum defines the Integrity Algorithm
 *         Types
 **/
typedef enum
{
    YS_MS_CIPHERING,
    YS_MS_INTEGRITY,
    YS_MS_KDF
}YsMsSpaccSecOp;

/** @brief This enum defines the Integrity Algorithm
 *         Types
 *  */
typedef enum
{
    YS_MS_SPACC_EIA0,
    YS_MS_SPACC_EIA1,
    YS_MS_SPACC_EIA2
}YsMsSpaccIntAlgo;

/** @brief This enum defines the Ciphering Algorithm
 *         Types
 *  */
typedef enum
{
    YS_MS_SPACC_EEA0,
    YS_MS_SPACC_EEA1,
    YS_MS_SPACC_EEA2
}YsMsSpaccCiphAlgo;

/** @brief This enum defines the key types that are
 *         received from the PDCP Layer and stored
 *         as part of security context
 *  */
typedef enum
{
    YS_MS_SPACC_CP_CIPHER_KEY,
    YS_MS_SPACC_UP_CIPHER_KEY,
    YS_MS_SPACC_INTEG_KEY
}YsMsSpaccKeyType;

/** @brief This structure the security information
 *         received from the PDCP Layer;
 *  */
typedef struct ysMsSecInfo
{
    U32  rbId;
    U8   rbType;
    U8  dir;
    U8  hash_key[YS_MS_KENB_LEN];
    U8  strS[YS_MS_STR_S_LEN];
    U8 count[4];
    U8 fresh[4];
    U8 *pOutBuf;
    
}YsMsSecInfo;

/**
 * @brief This structure contains the security
 *        context of a UE for the MSPD SPAcc
 *        accelerators.
 **/
typedef struct ysUeSecCtxt
{
   CmHashListEnt      ent;
   U16                ueId;
   YsMsSpaccIntAlgo   intAlgoType;
   YsMsSpaccCiphAlgo  ciphAlgoType;
   S32                ciphHandle;
   S32                intHandle;
   U8                 upCiph_key[LTE_CIPHER_KEY_LEN];
   U8                 cpCiph_key[LTE_CIPHER_KEY_LEN];
   U8                 integ_key[LTE_INTEG_KEY_LEN];
}YsUeSecCtxt;

#ifdef SPACC_PROF

#define MAX_REP_SPACC_PROF 100
/**
 * @brief This structure is used only for profiling various 
 *        metrics for SPACC 
 */
typedef struct ysSpaccProfiling
{
  U64 profEEA1CiphPrevTti;
  U64 profEEA1CiphCurrTti;
  U64 profEEA2CiphPrevTti;
  U64 profEEA2CiphCurrTti;

  U64 profEEA1CiphDiff;
  U64 profEEA2CiphDiff;

  U64 profEIA1IntgPrevTti;
  U64 profEIA1IntgCurrTti;
  U64 profEIA2IntgPrevTti;
  U64 profEIA2IntgCurrTti;

  U64 profEIA1IntgDiff;
  U64 profEIA2IntgDiff;

  double profEEA1CiphTotTime;
  double profEEA2CiphTotTime;

  double profEIA1IntgTotTime;
  double profEIA2IntgTotTime;
}YsSpaccProfiling;
#endif

/**
 * @brief This structure contains the security
 *        control block of the UE Security Hash List for
 *        the MSPD SPAcc accelerators.
 **/
typedef struct ysUeMsSecCb
{
    U32 nmbUe;
    YsMsSpaccHashState secHashState;
    CmHashListCp ueSecHLst;
    DDT_LST *paDdtPtLst[MAX_PRE_ALLOC_BUFF];
    DDT_LST *paDdtCtLst[MAX_PRE_ALLOC_BUFF];
    U8 *paDdtBlk[MAX_PRE_ALLOC_BUFF];
}YsUeMsSecCb;

EXTERN YsUeMsSecCb ueSecCb;
#endif
#if 0
#define LTE_CIPHER_KEY_LEN 16
#define LTE_INTEG_KEY_LEN 16
#define LTE_IV_LEN 16
#define YS_MS_HASH_KEY_SZ 16
#define YS_MS_HMAC_STR_SZ 16
#define YS_MS_KDF_OP_LEN 32
#define BYTES_IN_WORD 4
#define LTE_IV_WORD_LEN (LTE_IV_LEN / BYTES_IN_WORD)
#define YS_MS_MAC_SIZE_BYTES 4

/** @brief This enum defines the Integrity Algorithm
 *         Types
 *  */
typedef enum
{
    YS_MS_CIPHERING,
    YS_MS_INTEGRITY
}YsMsSpaccSecOp;

/** @brief This enum defines the Integrity Algorithm
 *         Types
 *  */
typedef enum
{
    YS_MS_SPACC_EIA0,
    YS_MS_SPACC_EIA1,
    YS_MS_SPACC_EIA2
}YsMsSpaccIntAlgo;

/** @brief This enum defines the Ciphering Algorithm
 *         Types
 *  */
typedef enum
{
    YS_MS_SPACC_EEA0,
    YS_MS_SPACC_EEA1,
    YS_MS_SPACC_EEA2
}YsMsSpaccCiphAlgo;

/** @brief This enum defines the key types that are
 *         received from the PDCP Layer and stored
 *         as part of security context
 *  */
typedef enum
{
    YS_MS_SPACC_CP_CIPHER_KEY,
    YS_MS_SPACC_UP_CIPHER_KEY,
    YS_MS_SPACC_INTEG_KEY
}YsMsSpaccKeyType;

/** @brief This structure the security information
 *         received from the PDCP Layer;
 *  */
typedef struct ysMsSecInfo
{
    U8  rbId;
    U8  rbType;
    U8  dir;
    /* U32 pIvVal[LTE_IV_WORD_LEN]; */
    U8  *pIvVal;
    U8  *hash_key;
    U8  *pOutBuf;
    U8  *strS;
    U32 outLen;
    U32 count;
    U32 fresh;
    
}YsMsSecInfo;

/**
 * @brief This structure contains the security
 *        context of a UE for the MSPD SPAcc
 *        accelerators.
 **/
typedef struct ysUeSecCtxt
{
   CmHashListEnt      ent;
   U16                ueId;
   YsMsSpaccIntAlgo   intAlgoType;
   YsMsSpaccCiphAlgo  ciphAlgoType;
   S32                dlhandle;
   S32                ulCiphhandle;
   S32                ulInthandle;
   U32                *pUpCiph_key;
   U32                *pCpCiph_key;
   U32                *pInteg_key;

}YsUeSecCtxt;



/**
 * @brief This structure contains the security
 *        control block of the UE Security Hash List for
 *        the MSPD SPAcc accelerators.
 **/
typedef struct ysUeMsSecCb
{
    U32 nmbUe; 
    CmHashListCp ueSecHLst;
}YsUeMsSecCb;

EXTERN YsUeMsSecCb ueSecCb;
#endif
#endif
#endif
/***************SEC_CHANGE_END****************/

/** 
  * List of UEId's 
  */
typedef struct ysUeLst
{
   U16         numPusch;
   YsUeCb      *puschLst[YS_NUM_UES];
   U16         numPucch;
   YsUeCb      *pucchLst[YS_NUM_UES];
   U16         numSrs;
   YsUeCb      *srsLst[YS_NUM_UES];
}YsUeLst;

typedef struct ysMsg3Lst
{
   U16                  numMsg3;
   TfuUeRecpReqInfo     msg3RecpLst[YS_NUM_UES];
} YsMsg3Lst;

#ifndef TFU_UPGRADE
typedef struct ysSubFrInfo
{
   Bool             pres;
   Bool             prachPres;
   YsUeLst          ueLst;
   YsMsg3Lst        msg3Lst;
   U8               numCh;
   U8               numCtrl;
   U8               numSrs;
   U8               numRxSduRcvd;
   /* Storing Predefined values */
} YsSubFrInfo;
#endif

typedef struct ysUlEncL1Msgs
{
   TfuDatIndInfo   *tfuDatIndInfo;   /* Tfu Dat Indication Information*/
   TfuCrcIndInfo   *tfuCrcIndInfo;   /* Tfu Crc Indication Information*/
   TfuRaReqIndInfo *rachIndInfo;     /* Tfu Rach Indication Information */
   TfuUlCqiIndInfo *ulCqiIndInfo;    /* Tfu UL CQI Information */
#ifndef TFU_UPGRADE
   TfuDlCqiIndInfo *dlCqiIndInfo;    /* Tfu DL CQI Information */
#else
   TfuRawCqiIndInfo *dlCqiIndInfo;    /* Tfu DL CQI Information */
#endif
   TfuHqIndInfo    *harqIndInfo;     /* Tfu Harq Indication Information */
   TfuSrIndInfo    *srIndInfo;       /* Tfu SR Indication Information */
   TfuTimingAdvIndInfo *tmAdvIndInfo;  /* TFU Timing Adv info structure */
#ifndef TFU_UPGRADE
   YsSubFrInfo     ysSubFrInfo;      /* SubFrame Information */
#endif
   TfuPucchDeltaPwrIndInfo *pucchDeltaPwrIndInfo; /* PUCCH delta power indication */
   PGENMSGDESC     rxVector;         /* RxVector for current subframe */
   Bool            rxSduPres;        /* HARQ: Indicates presence of rxSdu */
} YsUlEncL1Msgs;



typedef struct ysPrachFddCfgInfo
{
   U8    prachCfgIndex;          /*!< Prach Config Index, (0..63) */
   S8    preamFrmt;              /*!< Preamble Format, (0..4) */
   YsSfnType sfn;                /*!< System Frame Number Type */
   U8    numSf;                  /* Number of Subframe Cfg */
   U8    sf[YS_NUM_SUB_FRAMES];  /*!< Subframe Number */
}YsPrachFddCfgInfo;

typedef struct ysPrachCb
{
   YsPrachFddCfgInfo *rachCfgInfo;               /* RACH configurations */
                     /* 0-9 for Even and 10-19 for Odd */
   U32               ysPrachPres;
}YsPrachCb;

#ifndef YS_MS_NO_TA
typedef struct ysTArptInfo
{
   U8 ta;
   CmLteTimingInfo taApplyTime;
   CmLList         lnk;
   CmLteTimingInfo throttleExp;
} YsTArptInfo;
#endif

#ifdef TTI_PROC_FIRST
/* This structure is to store the UL SDU indications and process always at
 * the edge of UL delay budget (2.5ms) */
typedef struct ysUlL1Msg
{
   U8 sduIndCnt;
   /* number of SduIndication possible per subframe(PUCCH || PUSCH) for each UE. */
   PRXSDUIND pRxSduInd[MAX_UL_PHY_MSG_PER_TTI];
   PMSGIND pRxEndInd;

}YsUlL1Msg;
#endif

/**
  * UE control block for UE specific information
  */
struct ysUeCb
{
   CmLteRnti            ueId;             /* UE ID */
   CmHashListEnt        ueHlEnt;          /* UeCb Hashlist Entry */
   CmLList              lnkSrLst;         /* Link List Node for SR List */
   CmLList              lnkSrsLst;        /* Link List Node for SRS List */
   CmLList              lnkCqiLst;        /* Link List Node for CQI List */
   CtfDedCfgInfo        ueCfg;            /* UE configuration */
   YsCellCb             *cellCb;          /* Cell Control Block */
   U8                   lastRptdUlCqi;    /* Last reported valid UL CQI */
   U8                   lastRptdDlCqi;    /* Last reported valid DL CQI */
   U32                  ulCqiLastTti;     /* Last TTI when UL CQI was reported */
   U16                  ulTaLastTti;      /* Ta Value in last TTI */
   S32                  lastRptdPucchSnr; /* Last reported PUCCH SNR value */
   U32                  pucchSnrLastTti;  /* Last TTI when PUCCH SNR was reported */
#ifndef TFU_UPGRADE
   YsSrCfgInfo          *srCfgInfo;
   YsSrsCfgInfo         *srsCfgInfo;
   YsCqiPmiCfgIdxInfo   *cqiCfgInfo;
   YsUeSubFrInfo        subFrInfo;
   YsUeSubFrInfo        ueSubFrInfo[YS_NUM_SUB_FRAMES];
   U8                   cwCount[YS_NUM_SUB_FRAMES];
   /* MS_WORKAROUND: needed to
                                         * figure out harq bits, won't be needed
                                         * once TFU_UPGRADE is used */
   /* This is to track the subframes where the
    * TB to CW swap flag is set */
   Bool                 cwSwpflg[YS_NUM_SUB_FRAMES];
#endif /* TFU_UPGRADE */
   TIMADVERRINF         timingAdvErrInfo;
#ifndef YS_MS_NO_TA
   YsTArptInfo          tarptInfo;
#endif
};

typedef struct ysMsL1Predef
{
   RACHCTRL         rachCtrl;
   ULSUBFRCMNCTRL   ulSubFrCmnCtrl;
} YsMsL1Predef;

#ifdef YS_ALL_PERF
typedef struct ysMsAllPerfTtiInfo
{
   CmLteTimingInfo  	timingInfo;
	U32					ttiStartTick;
	U32					ttiEndTick;
   U32               avlMem;

#if YS_FUTURE_PERF
	U32					macTtiStartTick;
	U32					macTtiEndTick;
	U32					schTtiStartTick;
	U32					schTtiEndTick;
	U32					txVectTick;
#endif
} YsMsAllPerfTtiInfo;
#endif



typedef struct ysMsChanRntiMap
{
   CmLteRnti     rnti;
} YsMsChanRntiMap;
typedef struct ysMsCellInfo
{
  U8 ulBw;
  U8 dlBw;
  U8 phichSixNg;
  U8 rbgSize;
  U8 bandId ; /* Adding bandId to automate the ADI ID */
  U8 numAntPorts;
   U8 cmnTransMode; /* transmission mode for common channels, PDCCH etc,
                       function of numAntPorts*/
  U8 sinrToMcs[256];
  U8 mcsToCqi[29];
}YsMsCellInfo;

struct ysCellCb
{
   CmLteCellId     cellId;                      /*!< Cell ID */
   CmHashListEnt   cellHlEnt;                   /* CellCb Hashlist Entry */
   CtfCellCfgInfo  cellCfg;                     /* Cell configuration */
   CmLteTimingInfo timingInfo;                  /* Timing information */
   YsTfuSapCb      *schTfuSap;                  /* TFU SAP Control Block */
   CmHashListCp    ueHLst;                      /* Hash list of UE control
                                                   blocks: YsUeCb */
   YsDlTxMsgInfo   dlTxMsgInfo[YS_NUM_SUB_FRAMES]; /* Used for storing MAC
                                                   CntrlReq/DatReq pointer */
   YsDlEncL1Msgs   dlEncL1Msgs[YS_NUM_SUB_FRAMES]; /* L1 DL encoded message
                                                      buffer */
   YsUlMsgInfo     ulMsgInfo;                   /* Used for storing MAC
                                                   RecpReq pointer */
   YsUlEncL1Msgs   ulEncL1Msgs[YS_NUM_SUB_FRAMES]; /* L1 UL encoded message
                                                      buffer */
   YsPrachCb       prachCb;                     /* PRACH Control Block */
#ifndef TFU_UPGRADE
   CmLListCp       srPeriodList[YS_SR_PERIOD_80];
   CmLListCp       srsPeriodList[YS_SRS_PERIOD_320];
   CmLListCp       cqiPeriodList[YS_CQI_PERIOD_160];
#endif /* TFU_UPGRADE */
   State           phyState;                    /* L1 PHY state */

   /* HARQ_UL:  Added crcInfo for UL subframes */
#ifdef RG_ULSCHED_AT_CRC
   Bool                crcSent[YS_NUM_SUB_FRAMES];
#endif
   Bool                isCrcExptd[YS_NUM_SUB_FRAMES];

  /* To Store Predefined RACH Control */
   YsMsL1Predef      preDefVal;
   U8                gotRACH;
    U32              cmnChScrInit; /*!< Scrambler initialization for common channels */

#ifdef YS_ALL_PERF
	  Bool						capPerfData;
	  U32							ttiIndex;
     float                 maxtimeTaken;
     float                 maxttiDiff;
#if YS_PHY_STOP_AUTO
	  YsMsAllPerfTtiInfo    perfInfo[YS_MS_MAX_PERF_TTI];
#else
	  YsMsAllPerfTtiInfo    perfInfo;
#endif
     U32					      ttiPrevTick;

#endif

    YsMsChanRntiMap chan2Rnti[YS_NUM_SUB_FRAMES][YS_NUM_UES];
    YsMsCellInfo    cellInfo;
#ifdef TTI_PROC_FIRST
   /* This structure is to store the UL SDU indications and process always at
    * the edge of UL delay budget (2.5ms) */
   YsUlL1Msg            ulRxSduInds[YS_NUM_SUB_FRAMES];
#endif
#ifndef YS_MS_NO_TA
   CmLListCp            taUeLst;
#endif

};
#ifdef PWAV
typedef struct _dStatsCfg {
  U32          cmd;
  U32          interval_ms; /* reporting interval */
}DStatsCfg;
#endif
/*********************************************/
/**
  * Global Control block for Convergence Layer
  */
struct ysCb
{
   TskInit         ysInit;                   /* Task Init info */
   S16             trcLen;                   /* trace length */
   YsTfuSapCb      *macTfuSap;                  /* TFU SAP Control Block */
   YsTfuSapCb      **tfuSapLst;              /* TFU SAP Control Block */
   YsCtfSapCb      ctfSap;                   /* CTF SAP Control Block */
   CmHashListCp    cellCfgLst;               /* Cell configuration list */
   Txt             apb[10000];               /* print buf         */
   YsGenCfg        genCfg;                   /* general config */
   Pst             clPst;
   CmLteCellId     cellId;                      /*!< ccpu00118146: Cell ID */
#ifdef PWAV
   DStatsCfg             dStatsCfg;
#endif
};

EXTERN YsCb ysCb;

EXTERN YsPrachFddCfgInfo  ysPrachFddCfgDb[YS_NUM_PRACH_FDD_CFG];
EXTERN Bool               ysPrachPresDb[YS_NUM_PRACH_PRES_ARR];
EXTERN YsSrCfgInfo        ysSrCfgDb[YS_NUM_SR_CFG];
EXTERN YsSrsCfgInfo       ysSrsCfgDb[YS_NUM_SRS_CFG];
EXTERN YsCqiPmiCfgIdxInfo ysCqiPmiCfgDb[YS_NUM_CQI_CFG];
EXTERN U8                 MSeq[20][13];
EXTERN U8                 cqiTo20BitMap[16][2];

EXTERN volatile U32       prev_sf;
EXTERN volatile U32       prev_sfn;


EXTERN S16 ysActvInit ARGS((Ent entity, Inst inst, Region
       region, Reason reason));
EXTERN S16 ysActvTsk ARGS((Pst *pst, Buffer *mBuf));

EXTERN Void ysGetSId ARGS((SystemId *s));

#ifndef TFU_UPGRADE
EXTERN S16 ysMsUlmPrcUeCfg ARGS((
YsCellCb        *cellCb,
YsUeCb          *ueCb
));

EXTERN S16 ysMsUlmPrcDelUeCfg ARGS((
YsCellCb        *cellCb,
YsUeCb          *ueCb
));
#endif /* TFU_UPGRADE */

EXTERN S16 ysMsUlmPrcCellCfg ARGS((
YsCellCb        *cellCb
));

EXTERN S16 ysMsUlmPrcDelCellCfg ARGS((
YsCellCb        *cellCb
));

EXTERN S16 ysMsUlmPrcCellRecfg ARGS((
YsCellCb        *cellCb
));

EXTERN S16 ysMsPrcRecpReq ARGS((
YsCellCb        *cellCb,
TfuRecpReqInfo  *recpReq
));

#ifndef TFU_UPGRADE
/* HARQ_UL: Added timing information for UL HARQ handling */
EXTERN S16 ysMsUtlMapRxVector ARGS((
YsCellCb         *cellCb,
YsSubFrInfo      *subFrInfo,
PULSUBFRDESC     *ulSubFrDesc,
CmLteTimingInfo   timingInfo
));
#else /* TFU_UPGRADE */

/* HARQ_UL: Added timing information for UL HARQ handling */
EXTERN S16 ysMsUtlMapRxVector ARGS((
YsCellCb         *cellCb,
TfuRecpReqInfo   *recpReq,
PULSUBFRDESC     ulSubFrDesc,
CmLteTimingInfo   timingInfo
));
#endif /* TFU_UPGRADE */


EXTERN Void ysMsUlmRxStartCnf ARGS((
PINITIND        rxStartCnf,
YsCellCb        *cellCb
));

EXTERN Void ysMsUtlPrntErrInfo ARGS((
U8         errCode
));

EXTERN Void ysMsUlmTtiInd ARGS((
CmLteTimingInfo  *timingInfo,
YsCellCb         *cellCb
));

EXTERN S16 ysMsUlmRxStartInd ARGS((
PMSGIND        rxStartInd,
YsCellCb        *cellCb
));

EXTERN S16 ysUtlAllocEventMem ARGS((
Ptr       *memPtr,
Size      memSize
));

EXTERN S16 ysMsUtlFillTxVector ARGS ((
TfuCntrlReqInfo *tfuCntrlReq,
YsCellCb        *cellCb,
PDLSUBFRDESC    pTxVect
));
/* HARQ: Added timing info */
EXTERN S16 ysMsUtlFillDCIChanDesc ARGS ((
TfuPdcchInfo    *pdcchInfo,
YsCellCb        *cellCb,
PDCICHANNELDESC dciCh,
U16             channelId,
CmLteTimingInfo timingInfo
));

EXTERN S16 ysMsUtlFillDlChanDesc ARGS ((
TfuDatReqPduInfo *tfuDatReqPdu,
TfuPhichInfo    *phichInfo,
YsCellCb        *cellCb,
PDLCHANDESC     dlCh,
U8              channelType,
U16             channelId,
CmLteTimingInfo timingInfo
));

EXTERN S16 ysMsUtlFillTxSdu ARGS ((
YsPhyTxSdu     *phyTxSdu,
TfuDatReqInfo *bchDatReq,
TfuDatReqPduInfo *datReq,
TfuPhichInfo  *phichInfo,
TfuPdcchInfo  *pdcchInfo,
U8            channelType,
U16           channelId,
YsCellCb      *cellCb,
U8            tbIndex
)) ;

EXTERN Void ysMsUtlFillMappingInfo ARGS ((
YsCellCb        *cellCb,
U32             riv,
U8              *startRb,
U8              *numRb
));

/* HARQ: DCI0_PHICH */
EXTERN Void ysMsUlmRxEndIndAtRxSdu ARGS((
CmLteTimingInfo rxEndTimingInfo,
YsCellCb        *cellCb,
Bool            fakeCrc
));

EXTERN S16 ysMsUtlFillDci0HiSdu ARGS ((
YsPhyTxSdu     *phyTxSdu,
TfuPhichInfo  *phichInfo,
TfuPdcchInfo  *pdcchInfo,
U8            channelType,
U16           channelId,
YsCellCb      *cellCb
));

EXTERN S16 ysMsUtlFillAndAddDci0HiInfo ARGS((
TfuCntrlReqInfo  *tfuCntrlReq,
YsCellCb         *cellCb,
PHIADCIULMSGDESC pDci0HiVector
));

/* HARQ: PHY_LST */
EXTERN S16 ysMsUtlFreePhyList ARGS((
YsPhyLstCp        *lstCp
));

EXTERN S16 ysMsUtlAddToPhyList ARGS((
YsPhyLstCp        *lstCp,
PMAC2PHY_QUEUE_EL lstElem
));
EXTERN S16 ysMsUtlCatPhyList ARGS((
YsPhyLstCp        *lstCp1,
YsPhyLstCp        *lstCp2
));
EXTERN PMAC2PHY_QUEUE_EL ysMsUtlGetPhyListElem ARGS((
Void));

EXTERN Void ysMsMapRntiToChan ARGS((YsCellCb * cellCb, CmLteRnti rnti, \
         U8 subframe, U16 channelId));
EXTERN Void ysMsGetRntiFrmChanId ARGS((YsCellCb * cellCb, CmLteRnti *rnti, \
         U8 subframe, U16 channelId));





/* Function prototypes for upper interface */
EXTERN S16 YsUiCtfBndReq ARGS((
         Pst *pst,
         SuId suId,
         SpId spId
         ));

EXTERN S16 YsUiCtfUbndReq ARGS((
         Pst *pst,
         SuId suId,
         Reason reason
         ));

EXTERN S16 YsUiCtfCfgReq ARGS((
         Pst *pst,
         SpId spId,
         CtfCfgTransId transId,
         CtfCfgReqInfo* cfgReqInfo
         ));

/***************SEC_CHANGE_START****************/
#if 0
#ifdef CTF_AS_SEC_ENB
EXTERN S16 YsUiCtfDatReq ARGS((
         Pst*                 pst,
         SpId                 spId,
         CtfDatReqSdus*       CtfDatReqInfo
         ));

EXTERN S16 ysUiCtfDatRsp ARGS((
         Pst      *pst,
         SpId     spId,
         CtfDatRspSdus *ctfDatRspSdus
         ));
#endif
#endif
/***************SEC_CHANGE_END****************/

EXTERN S16 YsUiTfuBndReq ARGS((
         Pst  *pst,
         SuId suId,
         SpId spId
         ));

EXTERN S16 YsUiTfuUbndReq ARGS((
         Pst    *pst,
         SpId   spId,
         Reason reason
         ));

EXTERN S16 YsUiTfuSchBndReq ARGS((
         Pst  *pst,
         SuId suId,
         SpId spId
         ));

EXTERN S16 YsUiTfuSchUbndReq ARGS((
         Pst    *pst,
         SpId   spId,
         Reason reason
         ));

EXTERN S16 YsUiTfuCntrlReq ARGS((
         Pst             *pst,
         SpId            spId,
         TfuCntrlReqInfo *cntrlReq
         ));

EXTERN S16 YsUiTfuDatReq ARGS((
         Pst             *pst,
         SpId            spId,
         TfuDatReqInfo   *datReq
         ));

EXTERN S16 YsUiTfuRecpReq ARGS((
         Pst             *pst,
         SpId            spId,
         TfuRecpReqInfo  *recpReq
         ));

/* Function prototypes for lower interface */
EXTERN S16 ysMsLimRegisterCallback ARGS((
         Void
         ));

EXTERN S16 ysMsLimRecvMsgFromPhy ARGS((
         U32   size,
         Ptr   l1Msg
         ));

EXTERN S16 ysMsLimSendToPhy ARGS((
         U32         size,
         PGENMSGDESC  pMsgDesc
         ));

EXTERN Data *ysUtlMalloc ARGS((Size size));

EXTERN Void ysUtlDeAlloc ARGS((Data *buf, Size size));

EXTERN S16 ysUtlFreeEventMem ARGS((Ptr memPtr));

EXTERN Void ysUtlFreeDatReq ARGS((TfuDatReqInfo  *datReq));

EXTERN S16 ysMsCfgSm ARGS((
         YsCellCb   *cellCb,
         U8         event,
         Void       *status
         ));

EXTERN S16 ysMsCfgAddCellCfg ARGS((
         CtfCellCfgInfo *cellCfg
         ));

EXTERN S16 ysMsCfgModCellCfg ARGS((
         CtfCellRecfgInfo  *cellRecfg
         ));

EXTERN S16 ysMsCfgDelCellCfg ARGS((
         CmLteCellId    cellId
         ));

EXTERN S16 ysMsCfgAddUeCfg ARGS((
         CtfDedCfgInfo  *dedCfg
         ));

EXTERN S16 ysMsCfgModUeCfg ARGS((
         CtfDedRecfgInfo   *dedRecfg
         ));

EXTERN S16 ysMsCfgDelUeCfg ARGS((
         CtfDedReleaseInfo   *dedRel
         ));

EXTERN YsCellCb *ysMsCfgGetCellCfg ARGS((
         CmLteCellId    cellId
         ));

EXTERN YsUeCb *ysMsCfgGetUe ARGS((
         YsCellCb        *cellCb,
         CmLteRnti       ueId
         ));

EXTERN S16 ysMsCfgDeAllocUeCb ARGS((
         YsCellCb        *cellCb,
         YsUeCb          *ueCb
         ));

EXTERN S16 ysLMMStaInd ARGS((
         U16 category,
         U16 event,
         U16 cause,
         YsUstaDgn *dgn
         ));

EXTERN S16 ysMsUtlFillCfgReq  ARGS((
         INITPARM      *msCfgReq,
         YsCellCb      *cellCb
         ));

EXTERN S16 ysMsUtlFillStartReq ARGS((
         PSTARTREQ      startReq,
         YsCellCb       *cellCb
         ));

EXTERN S16 ysMsUtlFillStopReq ARGS((
         PGENMSGDESC    stopReq,
         YsCellCb       *cellCb
         ));

EXTERN S16 ysMsDlmSndTxSdu ARGS((
         YsCellCb        *cellCb
         ));

EXTERN S16 ysMsDlmPrcDatReq ARGS((
         YsCellCb        *cellCb,
         TfuDatReqInfo   *datReq
         ));

EXTERN S16 ysMsDlmPrcCntrlReq ARGS((
         YsCellCb        *cellCb,
         TfuCntrlReqInfo *cntrlReq
         ));

EXTERN S16 ysMsDlmPrcTtiInd ARGS((
         PMSGIND         txStartInd,
         YsCellCb        *cellCb,
         Bool            isDummyTti
         ));

EXTERN S16 ysMsDlmPrcTxStartCfm ARGS((
         PINITIND        txStartCfm,
         YsCellCb        *cellCb
         ));

EXTERN S16 ysMsUlmPrcRecpReq ARGS((
         YsCellCb        *cellCb,
         TfuRecpReqInfo  *recpReq
         ));

EXTERN S16 ysMsUlmSduInd ARGS((
         PRXSDUIND       rxSduInd,
         YsCellCb        *cellCb
         ));

EXTERN Void ysMsUlmRxEndInd ARGS((
         PMSGIND        rxEndInd,
         YsCellCb        *cellCb
         ));

EXTERN Void ysMsUlmRachRxEndInd ARGS((
         PMSGIND        rxEndInd,
         YsCellCb        *cellCb
         ));

/* Function prototypes */
EXTERN S16 YsUiCtfBndCfm ARGS((
         Pst* pst,
         SuId suId,
         U8 status
         ));

EXTERN S16 YsUiCtfCfgCfm ARGS((
         Pst* pst,
         SuId suId,
         CtfCfgTransId transId,
         U8 status
         ));

EXTERN S16 YsUiTfuBndCfm ARGS((
         Pst* pst,
         SuId suId,
         U8 status
         ));

EXTERN S16 YsUiTfuSchBndCfm ARGS((
         Pst* pst,
         SuId suId,
         U8 status
         ));

EXTERN S16 YsUiTfuRaReqInd ARGS((
         Pst * pst,
         SuId suId,
         TfuRaReqIndInfo * raReqInd
         ));

EXTERN S16 YsUiTfuUlCqiInd ARGS((
         Pst * pst,
         SuId suId,
         TfuUlCqiIndInfo * ulCqiInd
         ));

EXTERN S16 YsUiTfuHqInd ARGS((
         Pst * pst,
         SuId suId,
         TfuHqIndInfo      *hqInd
         ));

EXTERN S16 YsUiTfuSrInd ARGS((
         Pst * pst,
         SuId suId,
         TfuSrIndInfo * srInd
         ));

EXTERN S16 YsUiTfuDlCqiInd ARGS((
         Pst * pst,
         SuId suId,
         TfuDlCqiIndInfo * dlCqiInd
         ));

#ifdef TFU_UPGRADE
EXTERN S16 YsUiTfuRawCqiInd ARGS((
         Pst * pst,
         SuId suId,
         TfuRawCqiIndInfo * dlCqiInd
         ));
#endif

EXTERN S16 YsUiTfuDatInd ARGS((
         Pst * pst,
         SuId suId,
         TfuDatIndInfo * datInd
         ));

EXTERN S16 YsUiTfuCrcInd ARGS((
         Pst * pst,
         SuId suId,
         TfuCrcIndInfo * crcInd
         ));


EXTERN S16 YsUiTfuTimingAdvInd ARGS((
         Pst * pst,
         SuId suId,
         TfuTimingAdvIndInfo * timingAdvInd
         ));

EXTERN S16 YsUiTfuTtiInd ARGS((
         Pst * pst,
         SuId suId,
         TfuTtiIndInfo * ttiInd
         ));

EXTERN S16 YsUiTfuSchTtiInd ARGS((
         Pst * pst,
         SuId suId,
         TfuTtiIndInfo * ttiInd
         ));
EXTERN S16 YsUiTfuPucchDeltaPwrInd ARGS((
	     Pst * pst,
	     SuId suId,
	     TfuPucchDeltaPwrIndInfo *pucchDeltaPwrInd
	     ));

EXTERN S16 YsMiLysCfgCfm ARGS((
         Pst      *pst,    /* post structure  */
         YsMngmt  *cfm     /* config confirm structure  */
         ));

EXTERN S16 YsMiLysStsCfm ARGS((
         Pst      *pst,    /* post structure  */
         YsMngmt  *cfm     /* statistics confirm structure  */
         ));

EXTERN S16 YsMiLysStaCfm ARGS((
         Pst      *pst,    /* post structure  */
         YsMngmt  *cfm     /* status confirm structure  */
         ));

EXTERN S16 YsMiLysCntrlCfm ARGS((
         Pst      *pst,    /* post structure  */
         YsMngmt  *cfm     /* control confirm structure  */
         ));

EXTERN S16 YsMiLysStaInd ARGS((
         Pst      *pst,    /* post structure  */
         YsMngmt  *usta    /* status indication structure  */
         ));

#ifndef TFU_UPGRADE
/* HARQ_UL: Added timing information for UL HARQ handling */
EXTERN S16 ysMsMapUlSubFrDesc ARGS((
         YsCellCb         *cellCb,
         YsSubFrInfo      *ysSubFrInfo,
         ULSUBFRDESC      *ulSubFrDesc,
         MsgLen           *msgLen,
         CmLteTimingInfo timingInfo
         ));
#endif

EXTERN PGENMSGDESC ysMsUtlAllocSvsrMsg ARGS((
         Bool isSdu));


EXTERN S16 ysMsgDispatchInit ARGS((
         Void
         ));
EXTERN S16 ysMsPrcRxStatusInd ARGS((
PRXSTATUSIND       pRxStatusInd,
YsCellCb           *cellCb
));

EXTERN Void ysMsPrcMissedVectors ARGS((
YsCellCb       *cellCb,
U8             missedSf
));

/***************SEC_CHANGE_START****************/
#if 0
#ifdef CTF_AS_SEC_ENB
/* Function Prototypes */

#if 1
EXTERN PUBLIC S16 ysMsSpaccCreateUeCtxt(U16 UeId);
EXTERN PUBLIC S16 ysMsSpaccUpdUeCtxt(U16 UeId, U8  Key[LTE_CIPHER_KEY_LEN], 
                    YsMsSpaccKeyType KeyType, YsMsSpaccCiphAlgo ciphAlgoType, 
                    YsMsSpaccIntAlgo intgAlgo);
EXTERN PUBLIC S16 ysMsSpaccDelUeCtxt(U16 ueId);

EXTERN PUBLIC S16 ysMsKDF(YsMsSecInfo *pSecInfo);
EXTERN PUBLIC U32 ysMsSpaccCipherPkt(YsMsSecInfo *pSecInfo,U16 UeId, 
                          Buffer *mBuf, Buffer **opSdu);
EXTERN PUBLIC U32 ysMsSpaccIntegProtPkt(YsMsSecInfo *pSecInfo,U16 UeId, 
                          Buffer *mBuf, U32 macI);
/***************SEC_CHANGE_END****************/
EXTERN PUBLIC S16 ysMsCryptoTst(Pst *pst);
EXTERN PUBLIC PCRYPTO_VECTOR_DATA ysSPAccDiagCreateCryptVec(YsUeSecCtxt *pUeSecCtxt,
                                   YsMsSecInfo *pSecInfo, YsMsSpaccSecOp secOp, Buffer *mBuf); 
#endif
#if 0
EXTERN PUBLIC S16 ysMsSpaccDrvInit();
EXTERN PUBLIC S16 ysMsSpaccCreateUeCtxt(U16 UeId, U8  Key[LTE_INTEG_KEY_LEN], 
              YsMsSpaccKeyType KeyType, YsMsSpaccIntAlgo intAlgoType);
EXTERN PUBLIC S16 ysMsSpaccUpdUeCtxt(U16 UeId, U8  Key[LTE_CIPHER_KEY_LEN], 
                    YsMsSpaccKeyType KeyType, YsMsSpaccCiphAlgo ciphAlgoType);
EXTERN PUBLIC S16 ysMsSpaccDelUeCtxt(U16 ueId);
EXTERN PUBLIC S16 ysMsSpaccClose(S32 handle);
EXTERN PUBLIC S16 ysMsSpaccShtDwn();
EXTERN PUBLIC S16 ysMsKDF(YsMsSecInfo *pSecInfo);
EXTERN PUBLIC U32 ysMsSpaccCipherPkt(YsMsSecInfo *pSecInfo,U16 UeId, U8 *dataBuf, 
                            U32 dataLen);
EXTERN PUBLIC U32 ysMsSpaccIntegProtPkt(YsMsSecInfo *pSecInfo,U16 UeId, U8 *dataBuf,
                               U32 dataLen);
EXTERN PUBLIC S16 ysMsCryptoTst();
#endif
#endif
#endif
/***************SEC_CHANGE_END****************/

#ifdef PWAV
EXTERN S16 sendMsgToFrm ARGS((
    Ent     srcEnt,
    Ent     dstEnt,
    Event   event,
    U16     len,
    U8     *data
    ));
#endif
#endif /* __YS_MS_X__ */
/********************************************************************30**

         End of file:     ys_ms.x@@/main/1 - Sun Mar 28 21:19:19 2010

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
----------- -------- ---- -----------------------------------------------
/main/1      ---      pk   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/
