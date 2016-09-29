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

/********************************************************************20**

     Name:     LTE DEFAULT CONFIG VALUES for MSPD

     Type:     C source file

     Desc:     Default Config Values  

     File:     ve_dflt_ms.x

     Sid:      ve_dflt_ms.x@@/main/2 - Wed Dec  1 08:44:23 2010

     Prg:      

*********************************************************************21*/
#ifndef VE_DFLT_MS_PARAMS 
#define VE_DFLT_MS_PARAMS 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern CONSTANT CtfBandwidth veDfltCellDlBW; 
extern CONSTANT CtfBandwidth veDfltCellUlBW; 

/* 
 * Master Info Block 
 */
extern CONSTANT U16 veDfltBchTbsSz; 
extern CONSTANT U16 veDfltDlschTbsSz; 
extern CONSTANT U16 veDfltDlSysbw;
extern CONSTANT CtfPhichDuration veDfltPhichDur;
extern CONSTANT CtfPhichResource veDfltPhichRes;
extern CONSTANT U16 veDfltPagDrx;
/* 
 * SystemInfo Blocks 
 */
extern CONSTANT U16 veDfltCellRsrvdForOperUse;
extern CONSTANT U16 veDfltCellBarred;
extern CONSTANT U16 veDfltIntraFreqResel;                 /*  picoChip would give approp. val  */
extern CONSTANT U16 veDfltCellRsrvExtn;                 /*  picoChip would give approp. val  */
extern CONSTANT S16 veDfltqRxlevmin;                 /*  picoChip would give approp. val  */
extern CONSTANT U16 veDfltqRxlevminoffset;          /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltPMax;                     /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltSubframeAssignment;        /* 5ms switch frame   */
extern CONSTANT U16 veDfltSpecialSubfrmPatterns;     /* based on latest system reqs.  */
#ifndef SI_NEW
extern CONSTANT U16 veDfltSiPeriodicity;            /* 16 radioframes  */
#else
extern CONSTANT U16 veDfltSiPeriodicity[];          /* Periodicity in radioframes
                                                  * for VE_SI_CNT SIs*/
extern CONSTANT U16 veDfltSiMapInfoCnt[];           /* Count of number of SIBs mapped 
                                                  * to respective SI */
extern CONSTANT U16 veDfltSiMapInfoVal[];           /* SIB type mapped 
                                                  * to respective SI */
extern CONSTANT U8 veDfltNumSi;
extern CONSTANT U8 veDfltModPrd;
#endif
extern CONSTANT U16 veDfltSiWindowLength;           /* 20 ms */
extern CONSTANT U16 veDfltSystemInfoValueTag;       /* Zero */

/* RACH Common Cfg */
extern CONSTANT U16 veDfltNumberOfRaPreambles;      /* n8 */
extern CONSTANT U16 veDflSizeOfRaPreamblesGroupA;  /* same as veDfltNumberOfRaPreambles */
extern CONSTANT U16 veDfltMessageSizeGroupA;          /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltMessagePowerOffsetGroupB;          /*  picoChip would give approp. val */

extern CONSTANT U16 veDfltPowerRampingStep;          /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltPreambleInitRecvdTgtPwr;   /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltPreambleTransMax;          /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltRaRspWindowSize;          /* sf8 */
extern CONSTANT U16 veDfltMacContentResTmr;         /* sf40 */
extern CONSTANT U16 veDfltMaxHARQMsg3Tx;            /* 6 given picochip */

extern CONSTANT U8 veDfltPreambleFormat;             /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltRaOccasionSize;           /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltRaOccasionSfnEnum;        /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltRaOccasionSubFrmNum0;    /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltRACHCfgMsgSizeGrpA;      /*  picoChip would give approp. val */
extern CONSTANT U16 veDfltPrachResource;             /*  picoChip would give approp. val */

/* BCCH Cfg */
extern CONSTANT U16 veDfltModPeriodCoeff;            /* n2  */
/* PCCH Cfg */
extern CONSTANT U16 veDfltDefaultPagCycle;           /* rf128 */
extern CONSTANT U16 veDfltPCCHCfgNB;               /* oneT */



extern CONSTANT U16 ve_dflt_numOfTxAnt;                    /* One */
extern CONSTANT CtfSCSpacing veDfltScSpacing;                    
extern CONSTANT CtfCPCfg veDfltCycPfx;                    
/* PRACH Config */
extern CONSTANT U16 veDfltRootSequenceIdx;           /* 0 */
extern CONSTANT U16 veDfltPrachCfgIdx;                /* 51 */
extern CONSTANT U16 veDfltHighSpeedFlag;            /* FALSE */
extern CONSTANT U16 veDfltZeroCorrelZoneCfg;         /* 0x04 */
extern CONSTANT U16 veDfltPrachFreqOffset;          /* 0 */

/* PDSCH Cfg */
extern CONSTANT S16 veDfltRefSignalPower;         /*  picoChip would give approp. val */
extern CONSTANT S16 veDfltPriSignalPower;
extern CONSTANT S16 veDfltSecSignalPower;
extern CONSTANT U16 veDfltPDSCHCfgPB;          /*  picoChip would give approp. val */
/* PUSCH cfg */
extern CONSTANT U16 veDfltNSB;                    /* 2 */
extern CONSTANT CtfPuschHoppingMode veDfltHoppingMode;            /* interSubFrame */
extern CONSTANT U16 veDfltPuschHoppingOff;       /* 0 */
extern CONSTANT U16 veDfltEnable64QAM;           /* false */
extern CONSTANT U16 veDfltGroupHoppingEnabled;   /* false */
extern CONSTANT U16 veDfltSeqHoppingEnabled;     /* since group hopping disabled */
extern CONSTANT U16 veDfltGroupAssignPUSCH;       /* since group hopping disabled */
extern CONSTANT U16 veDfltCyclicShift;            /* since group hopping disabled */
/* PUCCH Cfg */
extern CONSTANT CtfPucchDeltaShift veDfltPUCCHDeltaShift;       /* DS1 */
extern CONSTANT U16 veDfltPUCCHnRBCQI;          /* Should have been 1 as per doc */
extern CONSTANT U16 veDfltPUCCHnCSAN;           /* zero */
extern CONSTANT U16 veDfltNlPUCCHAn;             /* Should have been 1 as per doc*/

extern CONSTANT U8 veDfltPUCCHFixBw;             
extern CONSTANT U8 veDfltPUCCHCyclShift;        
/* SoundingsRS UL Cfg */
extern CONSTANT U16 veDfltSrsBandwidthCfg;        
extern CONSTANT U16 veDfltSrsSubfrmCfg;           
extern CONSTANT U16 veDfltAckNackSrsSimTrns;     
extern CONSTANT U16 veDfltSrsMaxUpPts;           

extern CONSTANT U16 veDfltSrsCfgPrd;             /*  SRS period configuration */
/* Uplink Power Control Config */
extern CONSTANT U16 veDfltULPwrCtrlAlpha;            /*  PicoChip will give val*/
extern CONSTANT S16 veDfltP0NominalPUCCH;            /*  PicoChip will give val*/
extern CONSTANT S16 veDfltP0NominalPUSCH;            /*  PicoChip will give val*/
extern CONSTANT U16 veDfltDeltaFPUCCHFormat1;       /*  PicoChip will give val*/
extern CONSTANT U16 veDfltDeltaFPUCCHFormat1b;      /*  PicoChip will give val*/
extern CONSTANT U16 veDfltDeltaFPUCCHFormat2;       /*  PicoChip will give val*/
extern CONSTANT U16 veDfltDeltaFPUCCHFormat2a;      /*  PicoChip will give val*/
extern CONSTANT U16 veDfltDeltaFPUCCHFormat2b;      /*  PicoChip will give val*/
extern CONSTANT S16 veDfltUlDeltaPreambleMsg3;          /*  PicoChip will give val*/

extern CONSTANT U16 veDfltUlCyclicPrefixLength;         /*  PicoChip will give val*/
/* UE Timers and Constants */
extern CONSTANT U16 veDfltTmrsAndConstsT300;            /*  UE & CCPU will give val*/
extern CONSTANT U16 veDfltTmrsAndConstsT301;            /*  UE & CCPU will give val*/
extern CONSTANT U16 veDfltTmrsAndConstsT310;            /*  UE & CCPU will give val*/
extern CONSTANT U16 veDfltTmrsAndConstsN310;            /*  UE & CCPU will give val*/
extern CONSTANT U16 veDfltTmrsAndConstsT311;            /*  UE & CCPU will give val*/
extern CONSTANT U16 veDfltTmrsAndConstsN311;            /*  UE & CCPU will give val*/
/* frequencyInformation */
extern CONSTANT U16 veDfltUlCarrierFreq;                
extern CONSTANT U16 veDfltUlBandwidth;                  /* 50 RB */
extern CONSTANT U16 veDfltAddSpectEmn;                   

extern CONSTANT U16 veDfltTimeAlignTmrCmn;               /* infinity */


/* RGR Cell Cfg */
extern CONSTANT U8 veDfltMacInst;
extern CONSTANT U8 veDfltMaxUePerUlSf;
extern CONSTANT U8 veDfltMaxUePerDlSf;
extern CONSTANT U8 veDfltMaxUlBwPerUe;
extern CONSTANT U16 veDfltMaxDlBwPerUe;
extern CONSTANT U8 veDfltMaxDlRetxBw;
extern CONSTANT U8 veDfltMaxDlUeNwTxPerTti;
extern CONSTANT U8 veDfltMaxUlUeNwTxPerTti;
extern CONSTANT U16 veDfltNumOfTxAnt;
extern CONSTANT U16 veDfltIsCycPrefULExt;
extern CONSTANT U16 veDfltIsCycPrefDLExt;
extern CONSTANT U16 veDfltCellModSchm;

extern CONSTANT U8 veDfltDlSchdType;
extern CONSTANT U8 veUlDlCfgIdx;
extern CONSTANT U8 veSpclSfCfgIdx;
extern CONSTANT U8 veDfltUlSchdType;
extern CONSTANT U8 veDfltDLFSSchdTyp;
extern CONSTANT U8 veDfltMaxDlHqTx;
extern CONSTANT U8 veDfltMaxMsg4HqTx;
extern CONSTANT U16 veDfltStartRnti;
extern CONSTANT U16 veDfltMacRntisize;
extern CONSTANT U8 veDfltCfiCfg;
extern CONSTANT U8 veDfltTrgUlCqi;
extern CONSTANT U16 veDfltBcchPchRaCodeRate;
extern CONSTANT U16 veDfltPdcchCodeRate;
extern CONSTANT U8 veDfltDlCmnCodRateCcchCqi;
extern CONSTANT U8 veDfltPuschSubBndStrt;
extern CONSTANT U8 veDfltPuschNoSubbnds;
extern CONSTANT U8 veDfltPuschSubBandSz;
extern CONSTANT U8 veDfltPuschSubBandDmrs;
extern CONSTANT U8 veDfltULCmnCodRateCcchCqi;
extern CONSTANT U8 veDfltIsDlFreqSel;
extern CONSTANT U8 veDfltThresholdCqi;
extern CONSTANT U8 veDfltDLTotalBw;
extern CONSTANT U8 veDfltULTotalBw;
extern CONSTANT U16 veDfltNgEnum;
extern CONSTANT U8 veDfltIsDurExt;
extern CONSTANT U8 veDfltPUCCHResSz;
extern CONSTANT U8 veDfltN1PucchAn;
extern CONSTANT U8 veDfltDeltaShift;
extern CONSTANT U8 veDfltPUCCHCycShift;
extern CONSTANT U16 veDfltSrsCfgPrdEnum;
extern CONSTANT U16 veDfltSrsBwEnum;
extern CONSTANT U8 veDfltSrsSubFrameCfg;
extern CONSTANT U8 veDfltPreambleFrmt;
extern CONSTANT U8 veDfltRaWinSz;
extern CONSTANT U8 veDfltRaOccSz;
extern CONSTANT U16 veDfltRaOccSfnEnum;
extern CONSTANT U8 veDfltMaxMsg3Tx;
extern CONSTANT U8 veDfltNumRaPreamble;
extern CONSTANT U8 veDfltSzRaPreambleGrpA;
extern CONSTANT U16 veDfltMsgSzGrpA;
/*extern CONSTANT U8 veDfltPrachResource; */
extern CONSTANT U8 veDfltSiWinSize;
extern CONSTANT U8 veDfltRetxCnt;
extern CONSTANT S8 veDfltP0NominalPusch;
extern CONSTANT U16 veDfltUlPwrCfgAlpha;
extern CONSTANT S8 veDfltP0NominalPucch; 
extern CONSTANT S8 veDfltDeltaPreambleMsg3;
extern CONSTANT U16 veDfltPucchPwrFmt3RntiSt;
extern CONSTANT U16 veDfltPucchPwrFmt3RntiSz;
extern CONSTANT U16 veDfltPucchPwrFmt3aRntiSt;
extern CONSTANT U16 veDfltPucchPwrFmt3aRntiSz;
extern CONSTANT U16 veDfltPuschPwrFmt3RntiSt;
extern CONSTANT U16 veDfltPuschPwrFmt3RntiSz;
extern CONSTANT U16 veDfltPuschPwrFmt3aRntiSt;
extern CONSTANT U16 veDfltPuschPwrFmt3aRntiSz;
extern CONSTANT U8 veDfltPuschCfgNoSubBands;
extern CONSTANT U8 veDfltPuschCfgIsIntraHop;
extern CONSTANT U8 veDfltPuschCfgHopOffst;
extern CONSTANT U8 veDfltPreambleSetCfgPres;
extern CONSTANT U8 veDfltPreambleSetCfgStart;
extern CONSTANT U8 veDfltPreambleSetCfgSz;
extern CONSTANT U8 veDfltPRACHNumRes;
extern CONSTANT U8 veDfltNumCmnLcs;

/* Security Cfg */
extern CONSTANT U16 veDfltCiphAlgo;                   
extern CONSTANT U16 veDfltIntgProtAlgo;               


/* RRC Dedicated Config */
/* SRB */
extern CONSTANT U16 veDfltSrbPollRetrans;               /*   45 */
extern CONSTANT U16 veDfltSrbPollPdu;               /*   Infinity  */
extern CONSTANT U16 veDfltSrbPollByte;               /*   Infinity  */
extern CONSTANT U16 veDfltSrbMaxReTxThresh;               /*    */
extern CONSTANT U16 veDfltSrbTreordrng;               /*    */
extern CONSTANT U16 veDfltSrbTStatProhibit;               /*    */

extern CONSTANT U16 veDfltSrb1LogchcfgPrior;               /*    */
extern CONSTANT U16 veDfltSrb2LogchcfgPrior;               /*    */

extern CONSTANT U16 veDfltSrbLogchcfgPriBitRate;               /*  infinity   */
extern CONSTANT U16 veDfltSrbLogchcfgBktSzDur;               
extern CONSTANT U16 veDfltSrbLogchcfgGrp;               

extern CONSTANT U16 veDfltMacCfgMaxHARQTx;               /* 6 */
extern CONSTANT U16 veDfltMacCfgPeriodicBSRTimer;               
extern CONSTANT U16 veDfltMacCfgRetxBSRTimer;               /* sf5120 */
extern CONSTANT U8 veDfltMacCfgTtiBundling;               

extern CONSTANT U16 veDfltMacCfgOnDurationTimer;               
extern CONSTANT U16 veDfltMacCfgDrxInactvTmr;               
extern CONSTANT U16 veDfltMacCfgDrxReTxTimer;               
extern CONSTANT U16 veDfltMacCfgLngDRXCycStrtOffVal;               

extern CONSTANT U16 veDfltMacCfgTimeAligTmrDed;               /* Infinity */

extern CONSTANT U16 veDfltMacCfgPeriodicPHR_Timer;               
extern CONSTANT U16 veDfltMacCfgProhibitPHRTimer;               
extern CONSTANT U16 veDfltMacCfgDlPathlossChange;               

extern CONSTANT U16 veDfltDedPhyCfgPa;               /* dB0 */
/* PUCCH-ConfigDedicated  */
extern CONSTANT U16 veDfltTddAckNackFdbckMode;               /* multiplexing */
/* Uplink Power Config dedicated */
extern CONSTANT S16 veDfltP0UEPUSCH;               /* 0 */
extern CONSTANT U16 veDfltDeltaMCSEnabled;               /* en0 */
extern CONSTANT U8 veDfltAccumulationEnabled;               /* en0 */
extern CONSTANT S16 veDfltP0UEPUCCH;               /* 0 */
extern CONSTANT U16 veDfltpSRSOffset;               /* 3(-6dB) */
extern CONSTANT U16 veDfltFilterCoef;               /* fc4 */
/* PUSCH ConfigDedicated */
extern CONSTANT U16 veDfltBetaOffACKIdx;               /* 9 */
extern CONSTANT U16 veDfltBetaOffRIIdx;               /* 6 */
extern CONSTANT U16 veDfltBetaOffCQIIdx;               /* 6 */
/* CQI Reporting Related Information */
extern CONSTANT U16 veDfltCqiRprtModeAperiodic;               /* rm30 */
extern CONSTANT U16 veDfltCqiNomPDSCHRSEPREoff;               /* 0 */
extern CONSTANT U16 veDfltCqiPUCCHResIdx;               /* 0 */
extern CONSTANT U16 veDfltCqiPmiConfigIdx;               /* FDD 25 TDD 24 */
extern CONSTANT U16 veDfltRiConfigIdx;               /* FDD - 483 / TDD - 484 */
extern CONSTANT U8 veDfltSimultAckNackAndCQI;               /* TRUE */
/* tpc-PDCCH-ConfigPUSCH  */
extern CONSTANT U16 veDfltTpcRNTI;               /* only for PUSCH */
extern CONSTANT U16 veDfltIndexOfFormat3;               /* 1 */
/* Sounding Rs Uplink Configuration */
extern CONSTANT U16 veDfltSrsBandwidth;               /* bw0 */
extern CONSTANT U16 veDfltSrsHoppingBw;               /* hbw0 */
extern CONSTANT U16 veDfltFreqDomainPos;               /* 0 */
extern CONSTANT U8 veDfltDuration;               /* TRUE */
extern CONSTANT U16 veDfltSrsConfigIdx;               /* FDD 20 / TDD 31 */
extern CONSTANT U16 veDfltTransmissionComb;               /* 0 */
extern CONSTANT U16 veDfltDedCyclicShift;               /* cs0 */
/* Scheduling Request Configuration */
extern CONSTANT U16 veDfltSrPUCCHResIdx;               
extern CONSTANT U16 veDfltSrConfigIdx;               /* 30 */
extern CONSTANT U16 veDfltDsrTransMax;               /* n16 */
/* Antenna Configuration */
extern CONSTANT U16 veDfltTransMode;               /* tm2 */

/* DRB PDCP Config  */
extern CONSTANT U16 veDfltDiscardTimer;               /* infinity */
extern CONSTANT U8 veDfltStatusReportReqd;               /* TRUE */
/* DRB RLC-Config */
extern CONSTANT U16 veDfltDrbTPollRetx;               /* ms80 */
extern CONSTANT U16 veDfltDrbTPollPDU;               /* p128 */
extern CONSTANT U16 veDfltDrbTPollByte;               /* kb125 */
extern CONSTANT U16 veDfltDrbTMaxRetxThres;               /* t4 */
extern CONSTANT U16 veDfltDrbTReordrng;               /* ms80 */
extern CONSTANT U16 veDfltDrbTStatProh;               /* ms60 */
/* DRB LogicalChannelConfig */
extern CONSTANT U16 veDfltDrbPriority;               /* High 6 */
extern CONSTANT U16 veDfltDrbPriBitRate;               /* kBps0   */
extern CONSTANT U16 veDfltDrbBktSzDur;               /* ms100 */
extern CONSTANT U16 veDfltDrbGrp;               /* High 1*/

/* Ctf Cfg Info */
extern CONSTANT CmLteUeCategory veDfltUECategory;               
extern CONSTANT CtfPdschPaParam veDfltPdschPA;               
extern CONSTANT CtfPucchRepFactor veDfltRepFact;               
extern CONSTANT U16 veDfltN1PUCCHRep;               
extern CONSTANT U16 veDfltFormatInd;               
extern CONSTANT U16 veDfltSubBandK;  
extern CONSTANT CtfUeTxAntMode veDfltAntInfoTxMode; /* CTF_UE_TX_ANT_MODE_TM1 */

extern CONSTANT CtfUeTxAntSelectType veDfltAntennaSel;               /*  Closed 0 | Open 1 */

/* Rgr cfg Info  UE Cfg */
/* Rgr cfg Info  UE Cfg */
/* MS_FIX */
extern CONSTANT RgrTxMode veDfltTxModeEnum;               /* TM2 */
extern CONSTANT U16 veDfltAPrdModeEnum;      /* MOD30 */
extern CONSTANT U16 veDfltPrdModeEnum;      
extern CONSTANT U16 veDfltPrdicityEnum;      
extern CONSTANT U8 veDfltSubframeOffst;      
extern CONSTANT S8 veDfltCqiOffst;      
extern CONSTANT S8 veDfltCqiCfgk;      
extern CONSTANT U16 veDfltCqiPmiCfgIdx;      
extern CONSTANT U8 veDfltMaxUlHqTx;      
extern CONSTANT U8 veDfltDeltaHqOffst;      
extern CONSTANT U16 veDfltTaTmr;      
extern CONSTANT Bool veDfltTaTmrPres;      
extern CONSTANT U8 veDfltUePrdDlCqiK;
extern CONSTANT U16 veDfltCqiPmiCfgIdx;
extern CONSTANT U8 veDfltUeUlPwrTrgCqi;
extern CONSTANT U8 veDfltAckNackRepFactor;

extern CONSTANT U32 veDfltUeBr;
extern CONSTANT U32 veDfltQci;
extern CONSTANT U32 veDfltDlQosGbr;
extern CONSTANT U32 veDfltDlQosMbr;
#ifdef LTEMAC_MIMO
extern CONSTANT Bool veDfltRiCfgPres;
extern CONSTANT U32 veDfltRiCfgIndx;
extern CONSTANT U32 veDfltAntCmnInfoPorts;
#endif
/* MS_FIX */
extern CONSTANT CtfAntennaPortsCount veDfltCtfAntPort;
extern CONSTANT Bool veCodeBookRstnPres;      
extern CONSTANT U32 vePmiBitMap0;
extern CONSTANT U32 vePmiBitMap1;


/* Cell Selection and Reselection Related Parameters */
/* SIB3 defaults */
extern CONSTANT U32 veDfltS3nCellChangeMedium;
extern CONSTANT U32 veDfltS3nCellChangeHigh;
extern CONSTANT U32 veDfltS3sNonIntraSearch;
extern CONSTANT U32 veDfltS3ThreshServingLow;
extern CONSTANT U32 veDfltS3CellReselectionPriority;
extern CONSTANT U32 veDfltS3qRxLevMin;
extern CONSTANT U32 veDfltS3pMax; 
extern CONSTANT U32 veDfltS3sIntraSearch;
extern CONSTANT U32 veDfltS3AllowedMeasBandwidth;
extern CONSTANT U32 veDfltS3PresenceAntennaPort1;
extern CONSTANT U8 veDfltS3NeighCellCfg;               
extern CONSTANT U32 veDfltS3tReselectionEUTRA;
/* SIB4 defaults */
extern CONSTANT U32 veDfltS4MaxCellIntra; 
extern CONSTANT U32 veDfltS4PhysCellId;
extern CONSTANT U32 veDfltS4QOffsetCell;
extern CONSTANT U32 veDfltS4MaxCellBlack;
extern CONSTANT U32 veDfltS4BCStart;
extern CONSTANT U32 veDfltS4BCRange;
extern CONSTANT U32 veDfltS4PCStart;
CONSTANT U32 veDfltS4PCRange;
/* SIB5 defaults */
extern CONSTANT U32 veDfltS5DlCarrierFreq;
extern CONSTANT S32 veDfltS5QRxLevMin;
extern CONSTANT U32 veDfltS5InterFreqtReselection;
extern CONSTANT U32 veDfltS5ThreshXHigh;
extern CONSTANT U32 veDfltS5ThreshXLow;
extern CONSTANT U32 veDfltS5AllowedMeasBandwidth;
extern CONSTANT U32 veDfltS5PresenceAntennaPort1;
extern CONSTANT U32 veDfltS5CellReselectionPriority;
extern CONSTANT U8 veDfltS5NeighCellCfg;
extern CONSTANT U32 veDfltS5QOffsetFreq;

#ifdef LTE_HO_SUPPORT
extern CONSTANT U32 veDfltMeasObjIdOne;
extern CONSTANT U32 veDfltMeasObjIdTwo;
extern CONSTANT U32 veDfltMeasRprtCfgId;
extern CONSTANT U32 veDfltMeasIdOne;
extern CONSTANT U32 veDfltMeasIdTwo;
extern CONSTANT S32 veDfltMeasA3Offset;
extern CONSTANT U32 veDfltMeasHysteresis;
extern CONSTANT U32 veDfltMeasTTT;
extern CONSTANT U32 veDfltMeasL3FilterCoeff;
extern CONSTANT U8  veDfltMeasEvntA3;
extern CONSTANT U32 veDfltMeaseutraFreqOffset;
extern CONSTANT U32 veDfltMeasTrgrQnty;
extern CONSTANT U32 veDfltMeasRprtQnty;
extern CONSTANT U32 veDfltMeasMaxRprtCells;
extern CONSTANT U32 veDfltMeasIntrvl;
extern CONSTANT U32 veDfltMeasRprtAmnt;
extern CONSTANT U32 veDfltMeasGapOffset;
extern CONSTANT U32 veDfltMeasServCellDlFreq;
extern CONSTANT U16 veDfltMeasServCellDlBwdth;
extern CONSTANT U32 veDfltMeasNghCellDlFreq;
extern CONSTANT U16 veDfltMeasNghCellDlBwdth;
extern CONSTANT U16 veDfltTmrsAndConstsT304;
extern CONSTANT U16 veDfltPrachMskIdx;                
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DFLT_MS_PARAMS */
/********************************************************************30**

         End of file:     ve_dflt_ms.x@@/main/2 - Wed Dec  1 08:44:23 2010

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
/main/2      ve004.102       rk        1. eNodeb 1.2 Release
*********************************************************************91*/

