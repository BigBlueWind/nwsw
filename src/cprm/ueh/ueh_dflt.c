/**
 * @file    ueh_dflt.c
 * @brief Default config values for User Equipment Handler (Abbrev: UEH)
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         : Amar Ramteke
 * Date           : 16th Sept 2010
 * Change History :
 */
#if 0
#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh_eut.h"
#include "lueh.h" 
#include "ueh_eut_x.h"
#include "lueh.x" 
#include "ueh.h" 
#include "ueh_cfg_table.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 
 * Master Info Block 
 */
U16 uehDfltBchTbsSz = 3;                 
U16 uehDfltDlschTbsSz = 32;             
SINT32 uehDfltPhichDur = CTF_PHICH_DUR_NORMAL;                 /* normal */

/* 
* SystemInfo Blocks 
*/
U16 uehDfltCellBarred = 1;            
U16 uehDfltCellRsrvExtn = 1;        

U8 uehDfltModPrd = 1;
U8 uehDfltModType = 0;
U8 uehDfltDupMode = 2;

U8 ulUmSnFieldLen = 1;
U8 dlUmSnFieldLen = 1;

U16 uehDfltSystemInfoValueTag = 0x00;        /* Zero */

/* RACH Common Cfg */
U8  uehDfltPreambleFormat = 0x01;
U16 uehDfltRaOccasionSize = 0x01;        
U16 uehDfltRaOccasionSfnEnum = 0x01;    
U16 uehDfltRaOccasionSubFrmNum0 = 0x01;
U16 uehDfltPrachResource = 0x01;

/* BCCH Cfg */

/* PCCH Cfg */
SINT32 uehDfltScSpacing = CTF_SC_SPACING_15KHZ;
SINT32 uehDfltCycPfx = CTF_CP_NORMAL;

/* PRACH Config */

/* PDSCH Cfg */
U16 uehDfltPDSCHCfgPB = 0x00;

/* PUSCH cfg */
U16 uehDfltEnable64QAM = FALSE;           /* false */


/* PUCCH Cfg */
U16 uehDfltPUCCHnRBCQI = 0x02;          /* PWAV: As Per LTE PICO eNodeB Software Design of RRC Connection Setup rev 1.1  */
U16 uehDfltPUCCHnCSAN = 0x00;           /* zero */
U8 uehDfltPUCCHFixBw = 0x03;         
U8 uehDfltPUCCHCyclShift = 0x01;    

/* SoundingsRS UL Cfg */
U16 uehDfltSrsMaxUpPts = 0x00;      
U16 uehDfltSrsCfgPrd = 0x00;             /*  SRS period configuration */

/* Uplink Power Control Config */
U16 uehDfltULPwrCtrlAlpha             = 0x05;
U16 uehDfltUlCyclicPrefixLength      = 0x00;

/* UE Timers and Constants */

/* frequencyInformation */

/* Security Cfg */
U16 uehDfltCiphAlgo = 0;           
U16 uehDfltIntgProtAlgo = 0;      

/* Cell Selection/Reselection */
#ifdef FT_CELL_RESELECTION
U16 veDfltnCellChangeMedium = 1;
U16 veDfltnCellChangeHigh = 1;
U16 veDfltsNonIntraSearch = 0;
U16 veDfltThreshServingLow = 0;
U16 veDfltCellReselectionPriority = 4;
S16 veDlftqRxLevMin = -70;
U16 veDfltpMax = 1;
U16 veDfltsIntraSearch = 1;
#endif

/* RRC Dedicated Config */
/* SRB */
U16 uehDfltSrbPollRetrans = 8;   
U16 uehDfltSrbPollPdu = 7;      
U16 uehDfltSrbPollByte = 14;   
U16 uehDfltSrbMaxReTxThresh = 3;  
U16 uehDfltSrbTreordrng = 7;     
U16 uehDfltSrbTStatProhibit = 0;  

U16 uehDfltSrb1LogchcfgPrior = 1;   
U16 uehDfltSrb2LogchcfgPrior = 3;  

U16 uehDfltSrbLogchcfgPriBitRate = 7;   
U16 uehDfltSrbLogchcfgBktSzDur = 0;    
U16 uehDfltSrbLogchcfgGrp = 0;        

/* PUCCH_RESOURCE_MGMT  */
U16 uehDfltMacCfgPeriodicBSRTimer = 0;     /*was 3 now infinity */
U16 uehDfltMacCfgRetxBSRTimer = 0;               /* was sf5120 now sf2560*/

U8 uehDfltMacCfgTtiBundling = FALSE;              

U16 uehDfltMacCfgOnDurationTimer = 5;            
U16 uehDfltMacCfgDrxInactvTmr = 19;             
U16 uehDfltMacCfgDrxReTxTimer = 5;             
U16 uehDfltMacCfgLngDRXCycStrtOffVal = 0;     

U16 uehDfltMacCfgTimeAligTmrDed = 7;               /* Infinity */

U16 uehDfltMacCfgPeriodicPHR_Timer = 5;     
U16 uehDfltMacCfgProhibitPHRTimer = 5;     
U16 uehDfltMacCfgDlPathlossChange = 1;    

/* PUCCH-ConfigDedicated  */
U16 uehDfltTddAckNackFdbckMode = 0;               /*bundling*/

/* Uplink Power Config dedicated */
U16 uehDfltDeltaMCSEnabled = 0;               /* en0 */
U8 uehDfltAccumulationEnabled = TRUE;               /* en0 */

/* PUSCH ConfigDedicated */

/* CQI Reporting Related Information */
U16 uehDfltCqiRprtModeAperiodic = 1;            /* rm20 */
U16 uehDfltCqiNomPDSCHRSEPREoff = 6;            /* 0 */
U16 uehDfltCqiPUCCHResIdx = 0;                  /* 0 */
U16 uehDfltCqiPmiConfigIdx = 25;                /* FDD 25 TDD 24 */
U16 uehDfltRiConfigIdx = 483;                   /* FDD - 483 / TDD - 484 */
U8 uehDfltSimultAckNackAndCQI = TRUE;           /* As Per LTE PICO eNodeB  System Requirement Specifications
                                                LTE PicoBTS System Requirements Spec.doc */
/* tpc-PDCCH-ConfigPUSCH  */
U16 uehDfltTpcRNTI = 0x01FA;               /* only for PUSCH */
U16 uehDfltIndexOfFormat3 = 1;               /* 1 */

/* Sounding Rs Uplink Configuration */
U16 uehDfltSrsBandwidth = 0;               /* bw0 */
U16 uehDfltSrsHoppingBw = 0;               /* hbw0 */
U16 uehDfltFreqDomainPos = 0;               /* 0 */
U8 uehDfltDuration = TRUE;               /* TRUE */
U16 uehDfltSrsPeriodicity = 10;               /* FDD 20 / TDD 31 */
U16 uehDfltTransmissionComb = 0;               /* 0 */
U16 uehDfltDedCyclicShift = 0;               /* cs0 */

/* Scheduling Request Configuration */
U16 uehDfltSrPUCCHResIdx = 0;             
U16 uehDfltSrConfigIdx = 30;               /* 30 */
U16 uehDfltDsrTransMax = 4;               /* n64 */

/* Antenna Configuration */
U16 uehDfltTransMode = 0;               /* tm1 */

/* DRB PDCP Config  */
U16 uehDfltDiscardTimer = 7;               /* infinity */
U8 uehDfltStatusReportReqd = TRUE;               /* TRUE */

/* DRB RLC-Config */
U16 uehDfltDrbTPollRetx = 25;               /* ms80 */
U16 uehDfltDrbTPollPDU = 2;               /* p128 NhuPollPDUp256Enum */
U16 uehDfltDrbTPollByte = 13;               /*NhuPollBytekB3000Enum kb125 */
U16 uehDfltDrbTMaxRetxThres = 3;               /* t4 */
U16 uehDfltDrbTReordrng = 16;               /* ms80 */
U16 uehDfltDrbTStatProh = 12;               /* ms60 */

/* DRB LogicalChannelConfig */
U16 uehDfltDrbPriority = 6;               /* High 6 */
U16 uehDfltDrbPriBitRate = 0;               /* kBps0   */
U16 uehDfltDrbBktSzDur = 1;               /* ms100 */
U16 uehDfltDrbGrp = 1;               /* High 1*/

/* Ctf Cfg Info */
SINT32 uehDfltUECategory = CM_LTE_UE_CAT_5;
SINT32 uehDfltPdschPA = 4;           /* same as uehDfltDedPhyCfgPa */
SINT32 uehDfltRepFact = CTF_PUCCH_REP_FACTOR_N2;
U16 uehDfltN1PUCCHRep = 0;              
U16 uehDfltFormatInd = 1;               /* 1 */
U16 uehDfltSubBandK = 0;               
SINT32 uehDfltAntInfoTxMode = CTF_UE_TX_ANT_MODE_TM1; /* CTF_UE_TX_ANT_MODE_TM1 */
SINT32 uehDfltAntennaSel = 0;               /*  Closed 0 | Open 1 */

/* Rgr cfg Info  UE Cfg */
U16 uehDfltTxModeEnum = 1;               /* TM1 */
U16 uehDfltAPrdModeEnum = 3;      /* MOD30 */

/* Periodic CQI reporting mode */
U16 uehDfltPrdModeEnum =  1;

/* Periodicity values for CQI */
U16 uehDfltPrdicityEnum = 20;       /* Acc to \02_Engineers_Notebook\0204_Specifications\020401_Internal\Release 1.0\AUSSIE_eNB_RRM_System_Architecture_Rel1.0*/

/* Subframe offset */
S8 uehDfltCqiOffst = 0;         /* Delta^cqi_offset: (actual_value*10) */
S8 uehDfltCqiCfgk  = 1;         /* According to Release 1.0\AUSSIE_eNB_RRM_System_Architecture_Rel1.0*/

/*!< Delta HARQ offset */
U8 uehDfltDeltaHqOffst = 0;    

/* Timer configuration */

/* k value: range [1-4] */
U8 uehDfltUePrdDlCqiK = 1;

/* CQI to aim for during PUSCH power control */
U8 uehDfltUeUlPwrTrgCqi = 9;
U8 uehDfltAckNackRepFactor= 4;


/* RGR Cell Cfg */
U8 uehDfltMacInst = 0;
U8 uehDfltMaxUePerUlSf = 10;
U8 uehDfltMaxUePerDlSf = 10;

/* Configure the below DL/UL bw per UE is as per the:
1. Scheduler at MAC 
2. Bandwidth expected
3. Number of UEs schedulable per TTI against the maximum allowed for a particular bandwidth   
Example:    
1. UL/veDfltMaxUlBwPerUe: For total of more than 1/2 UEs, this shall be set to 20 for RR scheduler.    
2. DL/veDfltMaxDlBwPerUe: For total of more than 1/2 UEs, this shall be set to 30 for RR scheduler. */
U8 uehDfltMaxUlBwPerUe = 100; /*(100/UEH_MAX_UE);*/
U16 uehDfltMaxDlBwPerUe = 100;
U8 uehDfltMaxDlRetxBw = 50;
U8 uehDfltMaxDlUeNwTxPerTti = 10;
U8 uehDfltMaxUlUeNwTxPerTti = 5;
U16 uehDfltNumOfTxAnt = 1;

U16 uehDfltIsCycPrefULExt = FALSE; /* TRUE-extended */
U16 uehDfltIsCycPrefDLExt = FALSE; /* TRUE-extended */
U16 uehDfltCellModSchm = FALSE;    /* TRUE indicates 64QAM */
/* MAC - Different Scheduler Type 
#define RGR_SCH_TYPE_SC1    0
#define RGR_SCH_TYPE_PFS    1
#define RGR_SCH_TYPE_RR     2
#define RGR_SCH_TYPE_MAXCI  3
Configure below the DL and UL scheduler typesas per these values */
/* Changing from 0 to 2 for RR */
U8 uehDfltDlSchdType = 0;  /*  0..(number of schedulers - 1) */ 
U8 uehUlDlCfgIdx = 2;
U8 uehSpclSfCfgIdx =8;     
U8 uehDfltUlSchdType = 0;  /*  0..(number of schedulers - 1) */
U8 uehDfltDLFSSchdTyp = 0;  /* DLFS scheduler; 0..(no. of schedulers - 1) */

/* CFI for PDCCH: a value in set {1,2,3} */
/* Target UL CQI to achieve through power control */
U8 uehDfltTrgUlCqi = 9;

/* BCCH on DLSCH, PCH and RARsp coding rate */
U16 uehDfltBcchPchRaCodeRate = 0;

/* PDCCH coding rate (for broadcast RNTIs) */
U16 uehDfltPdcchCodeRate = 0;

/* Default CQI to be used for Msg4 and UE 
 * which has not yet reported any CQI */
U8 uehDfltDlCmnCodRateCcchCqi = 15;

/* Sub-band start */
U8 uehDfltPuschSubBndStrt = 1;

/* Number of equal sized sub-bands */
U8 uehDfltPuschNoSubbnds = 23; /*5 Mhz Configuration*/

/* Size of a sub-band */
U8 uehDfltPuschSubBandSz = 1;

/* DMRS information per sub-band */
U8 uehDfltPuschSubBandDmrs = 1;

/* CCCH CQI index, also used as default initial CQI for UEs */
U8 uehDfltULCmnCodRateCcchCqi = 15;

/* Indicates if resource allocation is frequency selective or not */
U8 uehDfltIsDlFreqSel = FALSE;

/* Threshold CQI value: Threshold CQI value below 
 * which the sub-band is not considered for 
 * allocation for a frequency selective cell */
U8 uehDfltThresholdCqi = FALSE;

/*!< Ng value for PHICH */
U16 uehDfltNgEnum = RGR_NG_ONESIXTH;
/*!< PHICH Duration: TRUE-extended/FALSE-normal */
U8 uehDfltIsDurExt = FALSE;
/*!< PUCCH resource-size or N^(2)_RB (in RBs) */
U8 uehDfltPUCCHResSz = 1;
/*!< SRS configuration period (in subframes).*/
U16 uehDfltSrsCfgPrdEnum = 0;

/*!< RACH Preamble format: a value in set {0,1,2,3} */
U8 uehDfltPreambleFrmt = 0;

/*!< RA Window size */
U8 uehDfltRaWinSz = 8;
/*!< Number of subframe numbers */
U8 uehDfltRaOccSz = 1;
/*!< System Frame Number */
U16 uehDfltRaOccSfnEnum = RGR_SFN_ANY;
/*!< Retransmission count */
U8 uehDfltRetxCnt = 1;

/*!< Aplha, 3-bit cell-specific parameter*/
U16 uehDfltUlPwrCfgAlpha = RGR_PWR_ALPHA0;

U16 uehDfltPucchPwrFmt3RntiSt = 1;
U16 uehDfltPucchPwrFmt3RntiSz = 1;

U16 uehDfltPucchPwrFmt3aRntiSt = 1;
U16 uehDfltPucchPwrFmt3aRntiSz = 1;

U16 uehDfltPuschPwrFmt3RntiSt = 1;
U16 uehDfltPuschPwrFmt3RntiSz = 1;

U16 uehDfltPuschPwrFmt3aRntiSt = 1;
U16 uehDfltPuschPwrFmt3aRntiSz = 1;


/* Changing from 0 to 1 to configure required DRB QoS, instead of qci 0, which allows infinite QoS*/
U32 uehDfltUeBr = 80000000; /* 2048000; */
U32 uehDfltQci = 1; 
U32 uehDfltDlQosGbr = 80000000;/* 2048000; */
U32 uehDfltDlQosMbr = 80000000;/* 2048000; */

/*!< Indicates if other configuration fields are valid */
U8 uehDfltPreambleSetCfgPres = FALSE;
/*!< Start Preamble ID for the range managed by MAC */
U8 uehDfltPreambleSetCfgStart = 0;
/*!< Indicates contiguous range of premables managaed by MAC */
U8 uehDfltPreambleSetCfgSz = 0;
/*!< Number of PRACH resources*/
U8 uehDfltPRACHNumRes = 0;
/*!< Number of common logical channels*/
U8 uehDfltNumCmnLcs = 6;

/* RLC Buffer size as per the UE Category */
U8 ueCatPollByteIdx[5] = {2, 6, 8, 9, 11};
U8 isUeSecEnble = 0;
U8 uehDfltDrbRlcMode = 0;                     /* AM */

/******SIB4******* */
U32 uehDfltS4MaxCellIntra = 1;
U32 uehDfltS4PhysCellId = 1;
U32 uehDfltS4QOffsetCell = 0;
U32 uehDfltS4MaxCellBlack = 1;
U32 uehDfltS4BCStart = 1;
U32 uehDfltS4BCRange = NhuPhysCellIdRangerangen4Enum;
U32 uehDfltS4PCStart = 1;
U32 uehDfltS4PCRange = NhuPhysCellIdRangerangen4Enum;

/* SIB5 defaults */
U32 uehDfltS5DlCarrierFreq = 15;
S32 uehDfltS5QRxLevMin = -65;
U32 uehDfltS5InterFreqtReselection = 0;
U32 uehDfltS5ThreshXHigh = 4;
U32 uehDfltS5ThreshXLow = 2;
U32 uehDfltS5AllowedMeasBandwidth = NhuAllowedMeasBandwidthmbw25Enum;
U32 uehDfltS5PresenceAntennaPort1 = FALSE;
U32 uehDfltS5CellReselectionPriority = 4;
U8 uehDfltS5NeighCellCfg = 1 << 6;
U32 uehDfltS5QOffsetFreq = 0;

/* UEH Timer Values */
U32     uehDfltCrntiRtnTimer;
U32     uehDfltStandaloneSrb1Timer;
U32     uehDfltContextReleaseTimer;
U32     uehDfltRrcConnRelL2AckTimer;
U32     uehDfltInternalConfigTimer;
U32     uehDfltopStopAckTimer;
U32     uehDfltMmeRelIndTimer;
U32     uehDfltInternalUeCfgTimer;

void uehCfgGlobalVarInit (UehCfgTable *RrmDftCfg)
{
    /*CtfBandwidth uehDfltCellDlBW = CTF_BW_RB_100; */                 /* 50 */ 
    /*CtfBandwidth uehDfltCellUlBW = CTF_BW_RB_100;      */            /* 50 */ 
    
    /* 
     * Master Info Block 
     */
    uehDfltBchTbsSz = RrmDftCfg->uehDfltBchTbsSz;
    uehDfltDlschTbsSz = RrmDftCfg->uehDfltDlschTbsSz;
    *  uehDfltDlSysbw =  RrmDftCfg->uehDfltDlSysbw; */
    uehDfltPhichDur = RrmDftCfg->uehDfltPhichDur;                 /* normal */
    /*CtfPhichResource uehDfltPhichRes = CTF_PHICH_RSRC_ONE;  */               /* one */
    /* 
    * SystemInfo Blocks 
    */
    /*U16 uehDfltCellRsrvdForOperUse = 1;    */
    uehDfltCellBarred = RrmDftCfg->uehDfltCellBarred;
    /*U16 uehDfltIntraFreqResel = 1;       */
    uehDfltCellRsrvExtn = RrmDftCfg->uehDfltCellRsrvExtn;
    /*S16 uehDfltqRxlevmin = -50;        */
  /*  uehDfltqRxlevminoffset = RrmDftCfg->uehDfltqRxlevminoffset; */
    /*U16 uehDfltPMax = 0x00;*/          

    uehDfltModPrd = RrmDftCfg->uehDfltModPrd;
    uehDfltModType = RrmDftCfg->uehDfltModType;
    uehDfltDupMode = RrmDftCfg->uehDfltDupMode;

    ulUmSnFieldLen = RrmDftCfg->ulUmSnFieldLen;
    dlUmSnFieldLen = RrmDftCfg->dlUmSnFieldLen;

    /*U16 uehDfltSiWindowLength = 0x05;  */         /* 20 ms */
    uehDfltSystemInfoValueTag = RrmDftCfg->uehDfltSystemInfoValueTag;
    
    /* RACH Common Cfg */
    /*U16 uehDfltNumberOfRaPreambles = 0x01;*/      /* n8 */
    /*U16 uehDflSizeOfRaPreamblesGroupA = 0x01;*/  /* same as uehDfltNumberOfRaPreambles */
    /*U16 uehDfltMessageSizeGroupA = 0x01;       */
    /*U16 uehDfltMessagePowerOffsetGroupB = 0x01;*/    
    /*U16 uehDfltPowerRampingStep = 0x01;*/          
    /*U16 uehDfltPreambleInitRecvdTgtPwr = 0x0f; */  
    /*U16 uehDfltPreambleTransMax = 0x00;*/        
    /*U16 uehDfltRaRspWindowSize = 0x06; */         /* sf8 */
    /*U16 uehDfltMacContentResTmr = 0x04;   */      /* sf40 */
    /*U16 uehDfltMaxHARQMsg3Tx = 0x04;*/            /* 6 given picochip */
    
    uehDfltPreambleFormat = RrmDftCfg->uehDfltPreambleFormat;
    uehDfltRaOccasionSize = RrmDftCfg->uehDfltRaOccasionSize;
    uehDfltRaOccasionSfnEnum = RrmDftCfg->uehDfltRaOccasionSfnEnum;
    uehDfltRaOccasionSubFrmNum0 = RrmDftCfg->uehDfltRaOccasionSubFrmNum0;
 /*   uehDfltRACHCfgMsgSizeGrpA   = RrmDftCfg->uehDfltRACHCfgMsgSizeGrpA; */
    uehDfltPrachResource = RrmDftCfg->uehDfltPrachResource;
    
    /* BCCH Cfg */
    /*U16 uehDfltModPeriodCoeff = 0x00;*/            /* n2  */
    /* PCCH Cfg */
    /*U16 uehDfltDefaultPagCycle = 0x02; */          /* rf128 */
    /*U16 uehDfltPCCHCfgNB = 0x02;            */   /* oneT */
    
    
    
    uehDfltScSpacing = RrmDftCfg->uehDfltScSpacing; 
    uehDfltCycPfx = RrmDftCfg->uehDfltCycPfx;
    /* PRACH Config */
    /*U16 uehDfltRootSequenceIdx = 22;  */         /* 0 */
    
    
    /* PDSCH Cfg */
    /*U16 uehDfltRefSignalPower = 18; */ 
    uehDfltPDSCHCfgPB = RrmDftCfg->uehDfltPDSCHCfgPB;
    /* PUSCH cfg */
    /*U16 uehDfltNSB = 0x1;*/                    /* 2 */
    /*CtfPuschHoppingMode uehDfltHoppingMode = CTF_PUSCH_HOPMODE_INTER_SUBFRM; */           /* interSubFrame */
    /*U16 uehDfltPuschHoppingOff = 0x08;    */   /* 0 */
    uehDfltEnable64QAM = RrmDftCfg->uehDfltEnable64QAM;

    /* PUCCH Cfg */
    /*CtfPucchDeltaShift uehDfltPUCCHDeltaShift = CTF_PUCCH_DELTASHIFT1;    */   /* DS1 */
    uehDfltPUCCHnRBCQI = RrmDftCfg->uehDfltPUCCHnRBCQI;
    uehDfltPUCCHnCSAN = RrmDftCfg->uehDfltPUCCHnCSAN;
    /*U16 uehDfltNlPUCCHAn = 0x01; */            /* Should have been 1 as per doc*/
    
    uehDfltPUCCHFixBw = RrmDftCfg->uehDfltPUCCHFixBw;
    uehDfltPUCCHCyclShift = RrmDftCfg->uehDfltPUCCHCyclShift;
    /* SoundingsRS UL Cfg */
    /*U16 uehDfltSrsBandwidthCfg = 0x00;    
    uehDfltSrsSubfrmCfg = RrmDftCfg->uehDfltSrsSubfrmCfg;
    uehDfltAckNackSrsSimTrns = TRUE;
    */
    uehDfltSrsMaxUpPts = RrmDftCfg->uehDfltSrsMaxUpPts;
    
    uehDfltSrsCfgPrd = RrmDftCfg->uehDfltSrsCfgPrd;
    /* Uplink Power Control Config */
    uehDfltULPwrCtrlAlpha = RrmDftCfg->uehDfltULPwrCtrlAlpha;
    /* uehDfltP0NominalPUSCH = RrmDftCfg->uehDfltP0NominalPUSCH; */
    /*U16 uehDfltDeltaFPUCCHFormat1 = 0x01;
    uehDfltDeltaFPUCCHFormat1b = RrmDftCfg->uehDfltDeltaFPUCCHFormat1b;
    uehDfltDeltaFPUCCHFormat2 = RrmDftCfg->uehDfltDeltaFPUCCHFormat2;
    uehDfltDeltaFPUCCHFormat2a = RrmDftCfg->uehDfltDeltaFPUCCHFormat2a;
    uehDfltDeltaFPUCCHFormat2b = RrmDftCfg->uehDfltDeltaFPUCCHFormat2b;
    uehDfltUlDeltaPreambleMsg3 = 0x04;*/
    
    uehDfltUlCyclicPrefixLength      = RrmDftCfg->uehDfltUlCyclicPrefixLength;
    /* UE Timers and Constants */
    
    /* frequencyInformation */
    /* uehDfltUlCarrierFreq = RrmDftCfg->uehDfltUlCarrierFreq; */
    /* uehDfltUlBandwidth = RrmDftCfg->uehDfltUlBandwidth; */

    
    /* Security Cfg */
    uehDfltCiphAlgo = RrmDftCfg->uehDfltCiphAlgo;
    uehDfltIntgProtAlgo = RrmDftCfg->uehDfltIntgProtAlgo;
    
    
    /* RRC Dedicated Config */
    /* SRB */
    uehDfltSrbPollRetrans = RrmDftCfg->uehDfltSrbPollRetrans;
    uehDfltSrbPollPdu = RrmDftCfg->uehDfltSrbPollPdu;
    uehDfltSrbPollByte = RrmDftCfg->uehDfltSrbPollByte;
    uehDfltSrbMaxReTxThresh = RrmDftCfg->uehDfltSrbMaxReTxThresh;
    uehDfltSrbTreordrng = RrmDftCfg->uehDfltSrbTreordrng;
    uehDfltSrbTStatProhibit = RrmDftCfg->uehDfltSrbTStatProhibit;
    
    uehDfltSrb1LogchcfgPrior = RrmDftCfg->uehDfltSrb1LogchcfgPrior;
    uehDfltSrb2LogchcfgPrior = RrmDftCfg->uehDfltSrb2LogchcfgPrior;
    
    uehDfltSrbLogchcfgPriBitRate = RrmDftCfg->uehDfltSrbLogchcfgPriBitRate;
    uehDfltSrbLogchcfgBktSzDur = RrmDftCfg->uehDfltSrbLogchcfgBktSzDur;
    uehDfltSrbLogchcfgGrp = RrmDftCfg->uehDfltSrbLogchcfgGrp;
    
    /* uehDfltMacCfgMaxHARQTx = RrmDftCfg->uehDfltMacCfgMaxHARQTx; */
    #if 1 /* def PUCCH_RESOURCE_MGMT  */
    uehDfltMacCfgPeriodicBSRTimer = RrmDftCfg->uehDfltMacCfgPeriodicBSRTimer;
    uehDfltMacCfgRetxBSRTimer = RrmDftCfg->uehDfltMacCfgRetxBSRTimer;
    
    #endif
    
    
    uehDfltMacCfgTtiBundling = RrmDftCfg->uehDfltMacCfgTtiBundling;
    
    uehDfltMacCfgOnDurationTimer = RrmDftCfg->uehDfltMacCfgOnDurationTimer;
    uehDfltMacCfgDrxInactvTmr = RrmDftCfg->uehDfltMacCfgDrxInactvTmr;
    uehDfltMacCfgDrxReTxTimer = RrmDftCfg->uehDfltMacCfgDrxReTxTimer;
    uehDfltMacCfgLngDRXCycStrtOffVal = RrmDftCfg->uehDfltMacCfgLngDRXCycStrtOffVal;
    
    uehDfltMacCfgTimeAligTmrDed = RrmDftCfg->uehDfltMacCfgTimeAligTmrDed;
    
    uehDfltMacCfgPeriodicPHR_Timer = RrmDftCfg->uehDfltMacCfgPeriodicPHR_Timer;
    uehDfltMacCfgProhibitPHRTimer = RrmDftCfg->uehDfltMacCfgProhibitPHRTimer;
    uehDfltMacCfgDlPathlossChange = RrmDftCfg->uehDfltMacCfgDlPathlossChange;
    
    /* uehDfltDedPhyCfgPa = RrmDftCfg->uehDfltDedPhyCfgPa; */
    /* PUCCH-ConfigDedicated  */

    uehDfltTddAckNackFdbckMode = RrmDftCfg->uehDfltTddAckNackFdbckMode;
    
    /* Uplink Power Config dedicated */
    /* uehDfltP0UEPUSCH = RrmDftCfg->uehDfltP0UEPUSCH;    */           /* 0 */
    uehDfltDeltaMCSEnabled = RrmDftCfg->uehDfltDeltaMCSEnabled;
    uehDfltAccumulationEnabled = RrmDftCfg->uehDfltAccumulationEnabled;
    /* uehDfltP0UEPUCCH = RrmDftCfg->uehDfltP0UEPUCCH;    */           /* 0 */
    /* uehDfltpSRSOffset = RrmDftCfg->uehDfltpSRSOffset; */
    /* uehDfltFilterCoef = RrmDftCfg->uehDfltFilterCoef; */
    /* PUSCH ConfigDedicated */
    /* uehDfltBetaOffACKIdx = RrmDftCfg->uehDfltBetaOffACKIdx; */
    /* uehDfltBetaOffRIIdx =  RrmDftCfg->uehDfltBetaOffRIIdx; */
    /* uehDfltBetaOffCQIIdx = RrmDftCfg->uehDfltBetaOffCQIIdx; */
    /* CQI Reporting Related Information */

    uehDfltCqiRprtModeAperiodic = RrmDftCfg->uehDfltCqiRprtModeAperiodic;

    uehDfltCqiNomPDSCHRSEPREoff = RrmDftCfg->uehDfltCqiNomPDSCHRSEPREoff;
    uehDfltCqiPUCCHResIdx = RrmDftCfg->uehDfltCqiPUCCHResIdx;
    uehDfltCqiPmiConfigIdx = RrmDftCfg->uehDfltCqiPmiConfigIdx;
    uehDfltRiConfigIdx = RrmDftCfg->uehDfltRiConfigIdx;
    uehDfltSimultAckNackAndCQI = RrmDftCfg->uehDfltSimultAckNackAndCQI;               /* As Per LTE PICO eNodeB  System Requirement Specifications

    http://sectrac2/svn/SEC/02_Engineers_Notebook/0204_Specifications/020401_Internal/D042-071829 LTE PicoBTS System Requirements Spec.doc */
    /* tpc-PDCCH-ConfigPUSCH  */
    uehDfltTpcRNTI = RrmDftCfg->uehDfltTpcRNTI;
    uehDfltIndexOfFormat3 = RrmDftCfg->uehDfltIndexOfFormat3;
    /* Sounding Rs Uplink Configuration */
    uehDfltSrsBandwidth = RrmDftCfg->uehDfltSrsBandwidth;
    uehDfltSrsHoppingBw = RrmDftCfg->uehDfltSrsHoppingBw;
    uehDfltFreqDomainPos = RrmDftCfg->uehDfltFreqDomainPos;
    uehDfltDuration = RrmDftCfg->uehDfltDuration;               /* TRUE */
    uehDfltSrsPeriodicity = RrmDftCfg->uehDfltSrsPeriodicity;
    uehDfltTransmissionComb = RrmDftCfg->uehDfltTransmissionComb;
    uehDfltDedCyclicShift = RrmDftCfg->uehDfltDedCyclicShift;
    /* Scheduling Request Configuration */
    uehDfltSrPUCCHResIdx = RrmDftCfg->uehDfltSrPUCCHResIdx;
    uehDfltSrConfigIdx = RrmDftCfg->uehDfltSrConfigIdx;
    uehDfltDsrTransMax = RrmDftCfg->uehDfltDsrTransMax;

    /* Antenna Configuration */

    uehDfltTransMode = RrmDftCfg->uehDfltTransMode;
    

    /* DRB PDCP Config  */
    uehDfltDiscardTimer = RrmDftCfg->uehDfltDiscardTimer;
    uehDfltStatusReportReqd = RrmDftCfg->uehDfltStatusReportReqd;               /* TRUE */
    /* DRB RLC-Config */
    uehDfltDrbTPollRetx = RrmDftCfg->uehDfltDrbTPollRetx;
    uehDfltDrbTPollPDU = RrmDftCfg->uehDfltDrbTPollPDU;
    uehDfltDrbTPollByte = RrmDftCfg->uehDfltDrbTPollByte;
    uehDfltDrbTMaxRetxThres = RrmDftCfg->uehDfltDrbTMaxRetxThres;
    uehDfltDrbTReordrng = RrmDftCfg->uehDfltDrbTReordrng;
    uehDfltDrbTStatProh = RrmDftCfg->uehDfltDrbTStatProh;
    /* DRB LogicalChannelConfig */
    uehDfltDrbPriority = RrmDftCfg->uehDfltDrbPriority;
    uehDfltDrbPriBitRate = RrmDftCfg->uehDfltDrbPriBitRate;
    uehDfltDrbBktSzDur = RrmDftCfg->uehDfltDrbBktSzDur;
    uehDfltDrbGrp = RrmDftCfg->uehDfltDrbGrp;
    
    /* Ctf Cfg Info */
    uehDfltUECategory = RrmDftCfg->uehDfltUECategory; 
    uehDfltPdschPA = RrmDftCfg->uehDfltPdschPA;           /* same as uehDfltDedPhyCfgPa */

    uehDfltRepFact = RrmDftCfg->uehDfltRepFact;
    uehDfltN1PUCCHRep = RrmDftCfg->uehDfltN1PUCCHRep;
    uehDfltFormatInd = RrmDftCfg->uehDfltFormatInd;
    uehDfltSubBandK = RrmDftCfg->uehDfltSubBandK;
    uehDfltAntInfoTxMode = RrmDftCfg->uehDfltAntInfoTxMode; /* CTF_UE_TX_ANT_MODE_TM1 */
    uehDfltAntennaSel = RrmDftCfg->uehDfltAntennaSel;               /*  Closed 0 | Open 1 */
    
    /* Rgr cfg Info  UE Cfg */
    uehDfltTxModeEnum = RrmDftCfg->uehDfltTxModeEnum;
    uehDfltAPrdModeEnum = RrmDftCfg->uehDfltAPrdModeEnum;
    /* Periodic CQI reporting mode */
    uehDfltPrdModeEnum =  RrmDftCfg->uehDfltPrdModeEnum;
    /* Periodicity values for CQI */
    uehDfltPrdicityEnum = RrmDftCfg->uehDfltPrdicityEnum;
    /* Subframe offset */
    /* uehDfltSubframeOffst = RrmDftCfg->uehDfltSubframeOffst; */
    /* Delta^cqi_offset: (actual_value*10) */
    uehDfltCqiOffst = RrmDftCfg->uehDfltCqiOffst;
    uehDfltCqiCfgk  = RrmDftCfg->uehDfltCqiCfgk;
    /* uehDfltCqiPmiCfgIdx = RrmDftCfg->uehDfltCqiPmiCfgIdx; */
    /*!< Maximum number of UL HARQ transmissions */
    /*!< Delta HARQ offset */
    uehDfltDeltaHqOffst = RrmDftCfg->uehDfltDeltaHqOffst;
    /* Timer configuration */

    /* k value: range [1-4] */
    uehDfltUePrdDlCqiK = RrmDftCfg->uehDfltUePrdDlCqiK;
    /* CQI to aim for during PUSCH power control */
    uehDfltUeUlPwrTrgCqi = RrmDftCfg->uehDfltUeUlPwrTrgCqi;
    uehDfltAckNackRepFactor= RrmDftCfg->uehDfltAckNackRepFactor;
    
    
    /* RGR Cell Cfg */
    uehDfltMacInst = RrmDftCfg->uehDfltMacInst;
    uehDfltMaxUePerUlSf = RrmDftCfg->uehDfltMaxUePerUlSf;
    uehDfltMaxUePerDlSf = RrmDftCfg->uehDfltMaxUePerDlSf;
    
    /* Configure the below DL/UL bw per UE is as per the:
    1. Scheduler at MAC 
    2. Bandwidth expected
    3. Number of UEs schedulable per TTI against the maximum allowed for a particular bandwidth   
    Example:    
    1. UL/veDfltMaxUlBwPerUe: For total of more than 1/2 UEs, this shall be set to 20 for RR scheduler.    
    2. DL/veDfltMaxDlBwPerUe: For total of more than 1/2 UEs, this shall be set to 30 for RR scheduler. */
    
    uehDfltMaxUlBwPerUe = RrmDftCfg->uehDfltMaxUlBwPerUe;
    uehDfltMaxDlBwPerUe = RrmDftCfg->uehDfltMaxDlBwPerUe;
    uehDfltMaxDlRetxBw = RrmDftCfg->uehDfltMaxDlRetxBw;
    uehDfltMaxDlUeNwTxPerTti = RrmDftCfg->uehDfltMaxDlUeNwTxPerTti;
    uehDfltMaxUlUeNwTxPerTti = RrmDftCfg->uehDfltMaxUlUeNwTxPerTti;
    uehDfltNumOfTxAnt = RrmDftCfg->uehDfltNumOfTxAnt;
    
    uehDfltIsCycPrefULExt = RrmDftCfg->uehDfltIsCycPrefULExt;
    uehDfltIsCycPrefDLExt = RrmDftCfg->uehDfltIsCycPrefDLExt;
    uehDfltCellModSchm = RrmDftCfg->uehDfltCellModSchm;
    /* MAC - Different Scheduler Type 
    #define RGR_SCH_TYPE_SC1    0
    #define RGR_SCH_TYPE_PFS    1
    #define RGR_SCH_TYPE_RR     2
    #define RGR_SCH_TYPE_MAXCI  3
    Configure below the DL and UL scheduler typesas per these values */
    /* Changing from 0 to 2 for RR */
    uehDfltDlSchdType = RrmDftCfg->uehDfltDlSchdType;
    uehUlDlCfgIdx = RrmDftCfg->uehUlDlCfgIdx;
    uehSpclSfCfgIdx =RrmDftCfg->uehSpclSfCfgIdx;
    uehDfltUlSchdType = RrmDftCfg->uehDfltUlSchdType;
    uehDfltDLFSSchdTyp = RrmDftCfg->uehDfltDLFSSchdTyp;
    /*U8 uehDfltMaxDlHqTx = 1;*/   /* Maximum number of DL HARQ Transmissions */
    /*U8 uehDfltMaxMsg4HqTx = 1; */ /* Maximum msg4 HARQ Transmissions */
    /*U16 uehDfltStartRnti = 104;*/ /* Start RNTI for the range managed by MAC */
    /*U16 uehDfltMacRntisize = 10;*/   /* Indicates contiguous range of RNTI */
    /* CFI for PDCCH: a value in set {1,2,3} */
    /*U8 uehDfltCfiCfg = 1;*/
    /* Target UL CQI to achieve through power control */
    uehDfltTrgUlCqi = RrmDftCfg->uehDfltTrgUlCqi;
    
    /* BCCH on DLSCH, PCH and RARsp coding rate */
    uehDfltBcchPchRaCodeRate = RrmDftCfg->uehDfltBcchPchRaCodeRate;
    /* PDCCH coding rate (for broadcast RNTIs) */
    uehDfltPdcchCodeRate = RrmDftCfg->uehDfltPdcchCodeRate;
    /* Default CQI to be used for Msg4 and UE 
    * which has not yet reported any CQI */
    uehDfltDlCmnCodRateCcchCqi = RrmDftCfg->uehDfltDlCmnCodRateCcchCqi;
    /* Sub-band start */
    uehDfltPuschSubBndStrt = RrmDftCfg->uehDfltPuschSubBndStrt;
    /* Number of equal sized sub-bands */
    uehDfltPuschNoSubbnds = RrmDftCfg->uehDfltPuschNoSubbnds;
    /* Size of a sub-band */
    uehDfltPuschSubBandSz = RrmDftCfg->uehDfltPuschSubBandSz;
    /* DMRS information per sub-band */
    uehDfltPuschSubBandDmrs = RrmDftCfg->uehDfltPuschSubBandDmrs;

    /* CCCH CQI index, also used as default initial CQI for UEs */
    uehDfltULCmnCodRateCcchCqi = RrmDftCfg->uehDfltULCmnCodRateCcchCqi;
    /* Indicates if resource allocation is frequency selective or not */
    uehDfltIsDlFreqSel = RrmDftCfg->uehDfltIsDlFreqSel;
    /* Threshold CQI value: Threshold CQI value below 
    * which the sub-band is not considered for 
    * allocation for a frequency selective cell */
    uehDfltThresholdCqi = RrmDftCfg->uehDfltThresholdCqi;
    /*!< Total Dowlink Bandwidth */
    /*U8 uehDfltDLTotalBw = 100;*/
    /*!< Total Uplink Bandwidth  */
    /*U8 uehDfltULTotalBw = 100;*/
    /*!< Ng value for PHICH */
    uehDfltNgEnum = RrmDftCfg->uehDfltNgEnum;
    /*!< PHICH Duration: TRUE-extended/FALSE-normal */
    uehDfltIsDurExt = RrmDftCfg->uehDfltIsDurExt;
    /*!< PUCCH resource-size or N^(2)_RB (in RBs) */
    uehDfltPUCCHResSz = RrmDftCfg->uehDfltPUCCHResSz;
    /*!< N^(1)_PUCCH */
    /*U8 uehDfltN1PucchAn = 1;*/
    /*!< Delta Shift for PUCCH: a value in set {1,2,3} */
    /*U8 uehDfltDeltaShift = 1;*/
    /*!< Cyclic Shift for PUCCH (N^(1)_CS): a value in range [0-7] */
    /*U8 uehDfltPUCCHCycShift = 1;*/
    /*!< SRS configuration period (in subframes).*/
    uehDfltSrsCfgPrdEnum = RrmDftCfg->uehDfltSrsCfgPrdEnum;
    /*!< SRS Bandwidth configuration per cell Range - [0-7] */
    /*U16 uehDfltSrsBwEnum = 0;*/
    /*!< SRS subframe configuration index per cell Range - [0-15] */
    /*U8 uehDfltSrsSubFrameCfg = 1;*/
    
    
    /*!< RACH Preamble format: a value in set {0,1,2,3} */
    #ifdef LTE_TDD
    uehDfltPreambleFrmt = RrmDftCfg->uehDfltPreambleFrmt;
    #else
    uehDfltPreambleFrmt = RrmDftCfg->uehDfltPreambleFrmt;
    #endif
    /*!< RA Window size */
    uehDfltRaWinSz = RrmDftCfg->uehDfltRaWinSz;
    /*!< Number of subframe numbers */
    uehDfltRaOccSz = RrmDftCfg->uehDfltRaOccSz;
    /*!< System Frame Number */
    uehDfltRaOccSfnEnum = RrmDftCfg->uehDfltRaOccSfnEnum;
    /*!< Maximum number of message 3 transmissions */
    /*U8 uehDfltMaxMsg3Tx = 1;*/
    /*!< Number of RA Preambles */
    /*U8 uehDfltNumRaPreamble = 6;*/
    /*!< Size of RA Preamble in Group A */
    /*U8 uehDfltSzRaPreambleGrpA = 5;*/
    /*!< MESSAGE_SIZE_GROUP_A */
    /*U16 uehDfltMsgSzGrpA = 340;*/
    /*!< N^RA_PRB: PRACH resource for random access */
    /*U8 uehDfltPrachResource = 1;*/
    /*!< SI window size */
    /* uehDfltSiWinSize = RrmDftCfg->uehDfltSiWinSize; */
    /*!< Retransmission count */
    uehDfltRetxCnt = RrmDftCfg->uehDfltRetxCnt;
    
    /*!< P0_NOMINAL_PUSCH*/
    /* uehDfltP0NominalPusch = RrmDftCfg->uehDfltP0NominalPusch; */
    /*!< Aplha, 3-bit cell-specific parameter*/
    uehDfltUlPwrCfgAlpha = RrmDftCfg->uehDfltUlPwrCfgAlpha;
    /*!< P0_NOMINAL_PUCCH*/
    /*S8 uehDfltP0NominalPucch = 1; */
    /*!< Delta_PREAMBLE_MSG3*/
    /*S8 uehDfltDeltaPreambleMsg3 = 1;*/
    
    uehDfltPucchPwrFmt3RntiSt = RrmDftCfg->uehDfltPucchPwrFmt3RntiSt;
    uehDfltPucchPwrFmt3RntiSz = RrmDftCfg->uehDfltPucchPwrFmt3RntiSz;
    
    uehDfltPucchPwrFmt3aRntiSt = RrmDftCfg->uehDfltPucchPwrFmt3aRntiSt;
    uehDfltPucchPwrFmt3aRntiSz = RrmDftCfg->uehDfltPucchPwrFmt3aRntiSz;
    
    uehDfltPuschPwrFmt3RntiSt = RrmDftCfg->uehDfltPuschPwrFmt3RntiSt;
    uehDfltPuschPwrFmt3RntiSz = RrmDftCfg->uehDfltPuschPwrFmt3RntiSz;
    
    uehDfltPuschPwrFmt3aRntiSt = RrmDftCfg->uehDfltPuschPwrFmt3aRntiSt;
    uehDfltPuschPwrFmt3aRntiSz = RrmDftCfg->uehDfltPuschPwrFmt3aRntiSz;
    
    
    /* Changing from 0 to 1 to configure required DRB QoS, instead of qci 0, which allows infinite QoS*/
    uehDfltUeBr = RrmDftCfg->uehDfltUeBr;
    uehDfltQci = RrmDftCfg->uehDfltQci;
    uehDfltDlQosGbr = RrmDftCfg->uehDfltDlQosGbr;
    uehDfltDlQosMbr = RrmDftCfg->uehDfltDlQosMbr;
    
    /*!< Number of sub-bands */
    /*U8 uehDfltPuschCfgNoSubBands = 1;*/
    /*!< Hopping mode inter/intra subframe */
    /*U8 uehDfltPuschCfgIsIntraHop = FALSE;*/
    /*!< Hopping offset {0 ... 98} */
    /*U8 uehDfltPuschCfgHopOffst = 0;*/
    /*!< Indicates if other configuration fields are valid */
    uehDfltPreambleSetCfgPres = RrmDftCfg->uehDfltPreambleSetCfgPres;
    /*!< Start Preamble ID for the range managed by MAC */
    uehDfltPreambleSetCfgStart = RrmDftCfg->uehDfltPreambleSetCfgStart;
    /*!< Indicates contiguous range of premables managaed by MAC */
    uehDfltPreambleSetCfgSz = RrmDftCfg->uehDfltPreambleSetCfgSz;
    /*!< Number of PRACH resources*/
    uehDfltPRACHNumRes = RrmDftCfg->uehDfltPRACHNumRes;
    /*!< Number of common logical channels*/
    uehDfltNumCmnLcs = RrmDftCfg->uehDfltNumCmnLcs;
    /*!< Crnti Retention timer */
    memcpy(ueCatPollByteIdx, RrmDftCfg->ueCatPollByteIdx, 5);
    isUeSecEnble = RrmDftCfg->isUeSecEnble;

    uehDfltDrbRlcMode = RrmDftCfg->uehDfltDrbRlcMode;

    /* Initialize the timer Values */
    uehDfltCrntiRtnTimer = uehGetTimerValFrmEnum(RrmDftCfg->uehDfltCrntiRtnTimer);
    uehDfltStandaloneSrb1Timer  = uehGetTimerValFrmEnum(RrmDftCfg->standaloneSrb1Timer);
    uehDfltContextReleaseTimer  = uehGetTimerValFrmEnum(RrmDftCfg->contextReleaseTimer);
    uehDfltMmeRelIndTimer       = uehGetTimerValFrmEnum(RrmDftCfg->mmeRelIndTimer);
    uehDfltRrcConnRelL2AckTimer = uehGetTimerValFrmEnum(RrmDftCfg->RrcConnRelL2AckTimer);
    uehDfltInternalConfigTimer  = uehGetTimerValFrmEnum(RrmDftCfg->internalConfigTimer);
    uehDfltopStopAckTimer       = uehGetTimerValFrmEnum(RrmDftCfg->opStopAckTimer);
    uehDfltInternalUeCfgTimer   = uehGetTimerValFrmEnum(RrmDftCfg->internalUeConfigTimer);
}

extern  S16                msArgc;
extern  S8                 **msArgv;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
