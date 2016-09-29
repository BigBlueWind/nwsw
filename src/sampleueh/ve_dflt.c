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

     Name:     LTE RRC

     Type:     C source file

     Desc:     Default Config Values  

     File:     ve_dflt.c

     Sid:      ve_dflt.c@@/main/2 - Wed Dec  1 08:44:23 2010

     Prg:      

*********************************************************************21*/
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

#include "nhu.h"           /* NHU interface defines */
#include "lnh.h"           /* Layer Manager interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */


/* RRM related includes */
#include "ve_eut.h"
#include "ve.h" 
#include "lve.h" 

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */
#include "nhu_asn.x"

/* RRM related includes */
#include "ve_eut.x"
#include "lve.x" 
#include "ve.x" 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ve003.102: CR ID- ccpu00116764-IOT changes are incorporated. */
CONSTANT CtfBandwidth veDfltCellDlBW = CTF_BW_RB_50;                  /* 50 */ 
CONSTANT CtfBandwidth veDfltCellUlBW = CTF_BW_RB_50;                  /* 50 */ 

/* 
 * Master Info Block 
 */
#ifdef MSPD
CONSTANT U16 veDfltBchTbsSz = 4;                 
#else
CONSTANT U16 veDfltBchTbsSz = 3;                 
#endif
CONSTANT U16 veDfltDlschTbsSz = 32;             
CONSTANT U16 veDfltDlSysbw =  0x03;                  /* = n50 */ 
CONSTANT CtfPhichDuration veDfltPhichDur = CTF_PHICH_DUR_NORMAL;                 /* normal */
CONSTANT CtfPhichResource veDfltPhichRes = CTF_PHICH_RSRC_ONESIXTH;                 /* oneSixth */
/* 
 * SystemInfo Blocks 
 */
CONSTANT U16 veDfltCellRsrvdForOperUse = 1;
CONSTANT U16 veDfltCellBarred = 1;            
CONSTANT U16 veDfltIntraFreqResel = 1;       
CONSTANT U16 veDfltCellRsrvExtn = 1;        
CONSTANT S16 veDfltqRxlevmin = -50;        
CONSTANT U16 veDfltqRxlevminoffset = 0x01;
CONSTANT U16 veDfltPMax = 0x00;          
CONSTANT U16 veDfltSubframeAssignment = 0x02;        /* 5ms switch frame   */
CONSTANT U16 veDfltSpecialSubfrmPatterns = 0x08;     /* based on latest system reqs.  */
#ifndef SI_NEW
CONSTANT U16 veDfltSiPeriodicity = 0x01;            /* 16 radioframes  */
#else
CONSTANT U16 veDfltSiPeriodicity[] = {0x01, 0x02, 0x03, 0x03}; 
               /* 16, 32, 64, 64 radioframes for SIB2, 3, 4, 5 respectively */
CONSTANT U16 veDfltSiMapInfoCnt[] = {0, 0x01, 0x01, 0x01}; 
               /* Si0 has SIB2 by default, Si1, 2, 3 has SIB3,4,5 respectively */
CONSTANT U16 veDfltSiMapInfoVal[] = {0, 0, 0x01, 0x02}; 
               /* SIB2, 3, 4, 5 respectively */
CONSTANT U8 veDfltNumSi = 4;
#if 0 /*Sriky Fix*/
CONSTANT U8 veDfltModPrd = 1;
#else
CONSTANT U8 veDfltModPrd = 128;
#endif
#endif
CONSTANT U16 veDfltSiWindowLength = 0x05;           /* 20 ms */
CONSTANT U16 veDfltSystemInfoValueTag = 0x00;        /* Zero */

/* RACH Common Cfg */
#ifdef MSPD
CONSTANT U16 veDfltNumberOfRaPreambles = 0x01;      /* n8 */
#else
CONSTANT U16 veDfltNumberOfRaPreambles = 0x0c;      /* n52 */
#endif
CONSTANT U16 veDflSizeOfRaPreamblesGroupA = 0x01;  /* same as veDfltNumberOfRaPreambles */
CONSTANT U16 veDfltMessageSizeGroupA = 0x01;       
CONSTANT U16 veDfltMessagePowerOffsetGroupB = 0x01;    
CONSTANT U16 veDfltPowerRampingStep = 0x01;          
#ifdef MSPD
CONSTANT U16 veDfltPreambleInitRecvdTgtPwr = 0x0f;
CONSTANT U16 veDfltPreambleTransMax = 0x09;
#else
CONSTANT U16 veDfltPreambleInitRecvdTgtPwr = 0x05;
CONSTANT U16 veDfltPreambleTransMax = 0x03; /* n6 */
#endif
CONSTANT U16 veDfltRaRspWindowSize = 0x06;          /* sf8 */
CONSTANT U16 veDfltMacContentResTmr = 0x04;         /* sf40 */
CONSTANT U16 veDfltMaxHARQMsg3Tx = 0x04;            /* 6 given picochip */

CONSTANT U8 veDfltPreambleFormat = 0x01;          
CONSTANT U16 veDfltRaOccasionSize = 0x01;        
CONSTANT U16 veDfltRaOccasionSfnEnum = 0x01;    
CONSTANT U16 veDfltRaOccasionSubFrmNum0 = 0x01;
CONSTANT U16 veDfltRACHCfgMsgSizeGrpA   = 11; 
CONSTANT U16 veDfltPrachResource = 0x01;     

/* BCCH Cfg */
#ifdef MSPD
CONSTANT U16 veDfltModPeriodCoeff = 0x00;            /* n2  */
#else
CONSTANT U16 veDfltModPeriodCoeff = 0x01;            /* n4  */
#endif
/* PCCH Cfg */
CONSTANT U16 veDfltDefaultPagCycle = 0x02;           /* rf128 */
CONSTANT U16 veDfltPCCHCfgNB = 0x02;               /* oneT */

CONSTANT U16 veDfltPagDrx   = 1;


CONSTANT CtfSCSpacing veDfltScSpacing = CTF_SC_SPACING_15KHZ; 
CONSTANT CtfCPCfg veDfltCycPfx = CTF_CP_NORMAL;              
/* PRACH Config */
#ifdef MSPD
CONSTANT U16 veDfltRootSequenceIdx = 22;           /* 0 */
#else
CONSTANT U16 veDfltRootSequenceIdx = 419; /*22*/	           /* 0 */
#endif
#ifdef LTE_TDD
CONSTANT U16 veDfltPrachCfgIdx = 51;                /* 51 */
#else
#ifdef MSPD
CONSTANT U16 veDfltPrachCfgIdx = 12;                /* 3*/
#else
CONSTANT U16 veDfltPrachCfgIdx = 0;                /* 3*/
#endif
#endif
CONSTANT U16 veDfltHighSpeedFlag = FALSE;            /* FALSE */
#ifdef LTE_TDD
CONSTANT U16 veDfltZeroCorrelZoneCfg = 0x04;         /* 0x04 */
#else
#ifdef MSPD
CONSTANT U16 veDfltZeroCorrelZoneCfg = 0x01;         /* 0x05 */
#else
CONSTANT U16 veDfltZeroCorrelZoneCfg = 0x05;         /* 0x05 */
#endif
#endif
#ifdef MSPD
CONSTANT U16 veDfltPrachFreqOffset = 0x00;          /* (assuming 10MHz Channel bandwidth) changed from 4 in mspd  */
#else
CONSTANT U16 veDfltPrachFreqOffset = 0x04;          /* assuming 10MHz Channel bandwidth  */
#endif

/* PDSCH Cfg */
#ifdef MSPD
CONSTANT S16 veDfltRefSignalPower = 18;  
#else
CONSTANT U16 veDfltRefSignalPower = 0;  /* Eariler 50 TODO */
#endif
CONSTANT U16 veDfltPDSCHCfgPB = 0x00;    
/* PUSCH cfg */
CONSTANT U16 veDfltNSB = 0x1;                    /* 2 */
CONSTANT CtfPuschHoppingMode veDfltHoppingMode = CTF_PUSCH_HOPMODE_INTER_SUBFRM;            /* interSubFrame */
CONSTANT U16 veDfltPuschHoppingOff = 0x08;       /* Assuming 10Mhz channel bandwidth*/
CONSTANT U16 veDfltEnable64QAM = FALSE;           /* false */
CONSTANT U16 veDfltGroupHoppingEnabled = FALSE;   /* false */
CONSTANT U16 veDfltSeqHoppingEnabled = FALSE;     /* since group hopping disabled */
#ifdef LTE_TDD
CONSTANT U16 veDfltGroupAssignPUSCH = 0x00;       /* since group hopping disabled */
CONSTANT U16 veDfltCyclicShift = 0x00;            /* since group hopping disabled */
#else
CONSTANT U16 veDfltGroupAssignPUSCH = 0x0;       /* since group hopping disabled */
CONSTANT U16 veDfltCyclicShift = 0x00;            /* since group hopping disabled */
#endif
/* PUCCH Cfg */
CONSTANT CtfPucchDeltaShift veDfltPUCCHDeltaShift = CTF_PUCCH_DELTASHIFT1;       /* DS1 */
CONSTANT U16 veDfltPUCCHnRBCQI = 0x01;          /* Should have been 1 as per doc */
CONSTANT U16 veDfltPUCCHnCSAN = 0x00;           /* zero */
CONSTANT U16 veDfltNlPUCCHAn = 0x01;             /* Should have been 1 as per doc*/

CONSTANT U8 veDfltPUCCHFixBw = 0x03;         
CONSTANT U8 veDfltPUCCHCyclShift = 0x01;    
/* SoundingsRS UL Cfg */
CONSTANT U16 veDfltSrsBandwidthCfg = 0x02;    
CONSTANT U16 veDfltSrsSubfrmCfg = 0x00;        
CONSTANT U16 veDfltAckNackSrsSimTrns = FALSE;
CONSTANT U16 veDfltSrsMaxUpPts = 0x00;      
#if 0
#ifdef VE_PICO
CONSTANT U16 veDfltulCarrierFreq=23230; /*PicoChip Specific UL 782MHz[23230] */
#endif
#endif

CONSTANT U16 veDfltSrsCfgPrd = 0x00;             /*  SRS period configuration */
/* Uplink Power Control Config */
#if 0 /* antz - updated at pC */
CONSTANT U16 veDfltULPwrCtrlAlpha = 0x00;           
CONSTANT S16 veDfltP0NominalPUCCH = -127;          
CONSTANT S16 veDfltP0NominalPUSCH = -126;         
CONSTANT U16 veDfltDeltaFPUCCHFormat1 = 0x00;    
CONSTANT U16 veDfltDeltaFPUCCHFormat1b = 0x00;     
CONSTANT U16 veDfltDeltaFPUCCHFormat2 = 0x00;     
CONSTANT U16 veDfltDeltaFPUCCHFormat2a = 0x00;   
CONSTANT U16 veDfltDeltaFPUCCHFormat2b = 0x00;  
CONSTANT S16 veDfltUlDeltaPreambleMsg3 = 0x00;   
#else
CONSTANT U16 veDfltULPwrCtrlAlpha = 0x05;
CONSTANT S16 veDfltP0NominalPUCCH = -96;
CONSTANT S16 veDfltP0NominalPUSCH = -85;
CONSTANT U16 veDfltDeltaFPUCCHFormat1 = 0x01;
CONSTANT U16 veDfltDeltaFPUCCHFormat1b = 0x01;
CONSTANT U16 veDfltDeltaFPUCCHFormat2 = 0x01;
CONSTANT U16 veDfltDeltaFPUCCHFormat2a = 0x01;
CONSTANT U16 veDfltDeltaFPUCCHFormat2b = 0x01;
CONSTANT S16 veDfltUlDeltaPreambleMsg3 = 0x04;
#endif

CONSTANT U16 veDfltUlCyclicPrefixLength      = 0x00;
#ifdef LTEMAC_MIMO
CONSTANT U32 veDfltAntCmnInfoPorts = NhuAntennaInfoCmnantennaPortsCountan2Enum;
#endif
/* UE Timers and CONSTANTs */
CONSTANT U16 veDfltTmrsAndConstsT300 = 0x00;       
CONSTANT U16 veDfltTmrsAndConstsT301 = 0x00;      
CONSTANT U16 veDfltTmrsAndConstsT310 = 0x00;     
CONSTANT U16 veDfltTmrsAndConstsN310 = 0x00;    
CONSTANT U16 veDfltTmrsAndConstsT311 = 0x00;   
CONSTANT U16 veDfltTmrsAndConstsN311 = 0x00;  
/* frequencyInformation */
CONSTANT U16 veDfltUlCarrierFreq = 0x00;     
CONSTANT U16 veDfltUlBandwidth = 0x03;                  /* 50 RB */
CONSTANT U16 veDfltAddSpectEmn = 0x01;      

#ifdef TA_NEW
CONSTANT U16 veDfltTimeAlignTmrCmn = 0x06;               /* 10240 SubFrms */
#else
CONSTANT U16 veDfltTimeAlignTmrCmn = 0x07;               /* infinity */
#endif

/* Security Cfg */
CONSTANT U16 veDfltCiphAlgo = 0;           
CONSTANT U16 veDfltIntgProtAlgo = 0;      
/* TODO Atul Cell Selection and Reselection Related Parameters */
CONSTANT U16 veDfltnCellChangeMedium = 1;
CONSTANT U16 veDfltnCellChangeHigh = 1;
CONSTANT U16 veDfltsNonIntraSearch = 0;
CONSTANT U16 veDfltThreshServingLow = 0;
CONSTANT U16 veDfltCellReselectionPriority = 4;
CONSTANT S16 veDlftqRxLevMin = -70;
CONSTANT U16 veDfltpMax = 1; 
CONSTANT U16 veDfltsIntraSearch = 1;



/* RRC Dedicated Config */
/* SRB */
CONSTANT U16 veDfltSrbPollRetrans = 8;   
CONSTANT U16 veDfltSrbPollPdu = 7;      
CONSTANT U16 veDfltSrbPollByte = 14;   
CONSTANT U16 veDfltSrbMaxReTxThresh = 3;  
CONSTANT U16 veDfltSrbTreordrng = 7;     
#ifdef MSPD
CONSTANT U16 veDfltSrbTStatProhibit = 40;  
#else
CONSTANT U16 veDfltSrbTStatProhibit = 0;  
#endif

CONSTANT U16 veDfltSrb1LogchcfgPrior = 1;   
CONSTANT U16 veDfltSrb2LogchcfgPrior = 3;  

CONSTANT U16 veDfltSrbLogchcfgPriBitRate = 7;   
CONSTANT U16 veDfltSrbLogchcfgBktSzDur = 0;    
CONSTANT U16 veDfltSrbLogchcfgGrp = 0;        

#ifdef MSPD
CONSTANT U16 veDfltMacCfgMaxHARQTx = 3;               /* 2 */
#else
CONSTANT U16 veDfltMacCfgMaxHARQTx = 2;               /* 6 */
#endif
CONSTANT U16 veDfltMacCfgPeriodicBSRTimer = 0;   
CONSTANT U16 veDfltMacCfgRetxBSRTimer = 0;               /* sf320 */
CONSTANT U8 veDfltMacCfgTtiBundling = FALSE;              

CONSTANT U16 veDfltMacCfgOnDurationTimer = 5;            
CONSTANT U16 veDfltMacCfgDrxInactvTmr = 19;             
CONSTANT U16 veDfltMacCfgDrxReTxTimer = 5;             
CONSTANT U16 veDfltMacCfgLngDRXCycStrtOffVal = 0;     

CONSTANT U16 veDfltMacCfgTimeAligTmrDed = 7;               /* Infinity */

CONSTANT U16 veDfltMacCfgPeriodicPHR_Timer = 5;     
CONSTANT U16 veDfltMacCfgProhibitPHRTimer = 5;     
CONSTANT U16 veDfltMacCfgDlPathlossChange = 1;    

CONSTANT U16 veDfltDedPhyCfgPa = 4;               /* dB0 */
/* PUCCH-ConfigDedicated  */
CONSTANT U16 veDfltTddAckNackFdbckMode = 0;               /* bundling */
/* Uplink Power Config dedicated */
CONSTANT S16 veDfltP0UEPUSCH = 0;               /* 0 */
CONSTANT U16 veDfltDeltaMCSEnabled = 0;               /* en0 */
CONSTANT U8 veDfltAccumulationEnabled = FALSE;               /* en0 */
CONSTANT S16 veDfltP0UEPUCCH = 0;               /* 0 */
#ifdef MSPD
CONSTANT U16 veDfltpSRSOffset = 7;               /* 3(-6dB) */
#else
CONSTANT U16 veDfltpSRSOffset = 3;               /* 3(-6dB) */
#endif
CONSTANT U16 veDfltFilterCoef = 4;               /* fc4 */
/* PUSCH ConfigDedicated */
CONSTANT U16 veDfltBetaOffACKIdx = 10;               /* 9 */
CONSTANT U16 veDfltBetaOffRIIdx =  12;               /* 6 */
CONSTANT U16 veDfltBetaOffCQIIdx = 15;               /* 6 */
/* CQI Reporting Related Information */
#if 0 /* antz - 6 is incorrect, rm30 => 3, using rm20 instead now */
CONSTANT U16 veDfltCqiRprtModeAperiodic = 6;               /* rm30 */
#else
CONSTANT U16 veDfltCqiRprtModeAperiodic = 3;               /* rm20 */
#endif
CONSTANT U16 veDfltCqiNomPDSCHRSEPREoff = 0;               /* 0 */
CONSTANT U16 veDfltCqiPUCCHResIdx = 0;               /* 0 */
#ifdef LTE_TDD
CONSTANT U16 veDfltCqiPmiConfigIdx = 24;               /* FDD 25 TDD 24 */
CONSTANT U16 veDfltRiConfigIdx = 484;               /* FDD - 483 / TDD - 484 */
CONSTANT U8 veDfltSimultAckNackAndCQI = FALSE;               /* FALSE */
#else
CONSTANT U8 veDfltSimultAckNackAndCQI = FALSE;               /* FALSE */
#ifndef LTEMAC_MIMO
CONSTANT U16 veDfltCqiPmiConfigIdx = 25;               /* FDD 25 TDD 24 */
CONSTANT U16 veDfltRiConfigIdx = 483;               /* FDD - 483 / TDD - 484 */
#else
CONSTANT U16 veDfltCqiPmiConfigIdx = 38;               /* FDD 25 TDD 24 */
CONSTANT U16 veDfltRiConfigIdx = 163;               /* FDD - 483 / TDD - 484 */
#endif
#endif
/* tpc-PDCCH-ConfigPUSCH  */
CONSTANT U16 veDfltTpcRNTI = 0x01FA;               /* only for PUSCH */
CONSTANT U16 veDfltIndexOfFormat3 = 1;               /* 1 */
/* Sounding Rs Uplink Configuration */
CONSTANT U16 veDfltSrsBandwidth = 0;               /* bw0 */
CONSTANT U16 veDfltSrsHoppingBw = 0;               /* hbw0 */
CONSTANT U16 veDfltFreqDomainPos = 0;               /* 0 */
CONSTANT U8 veDfltDuration = TRUE;               /* TRUE */
#ifdef LTE_TDD
CONSTANT U16 veDfltSrsConfigIdx = 31;               /* FDD 20 / TDD 31 */
#else
CONSTANT U16 veDfltSrsConfigIdx = 20;               /* FDD 20 / TDD 31 */
#endif
CONSTANT U16 veDfltTransmissionComb = 0;               /* 0 */
CONSTANT U16 veDfltDedCyclicShift = 0;               /* cs0 */
/* Scheduling Request Configuration */
CONSTANT U16 veDfltSrPUCCHResIdx = 0;             
CONSTANT U16 veDfltSrConfigIdx = 30;               /* 30 */
CONSTANT U16 veDfltDsrTransMax = 4;               /* n64 */
/* Antenna Configuration */
#ifdef LTEMAC_MIMO /* syed: Bydefault setting Transmission mode 3 for MIMO testing */
CONSTANT U16 veDfltTransMode = NhuAntennaInfoDedicatedtransmissionModetm3Enum;               /* tm3 */
#else
CONSTANT U16 veDfltTransMode = NhuAntennaInfoDedicatedtransmissionModetm1Enum;               /* tm1 */
#endif

/* DRB PDCP Config  */
CONSTANT U16 veDfltDiscardTimer = 7;               /* infinity */
CONSTANT U8 veDfltStatusReportReqd = TRUE;               /* TRUE */
/* DRB RLC-Config */
#ifdef LTE_TDD
CONSTANT U16 veDfltDrbTPollRetx = 40;               /* ms80 */
#else
CONSTANT U16 veDfltDrbTPollRetx = 15;
#endif
CONSTANT U16 veDfltDrbTPollPDU = 2;               /* Enum{4,8,16,32,64,128,256,Inf}*/
CONSTANT U16 veDfltDrbTPollByte = 13;               /*NhuPollBytekB3000Enum kb125 */
CONSTANT U16 veDfltDrbTMaxRetxThres = 5;               /* t4 */
CONSTANT U16 veDfltDrbTReordrng = 16;               /* ms80 */
CONSTANT U16 veDfltDrbTStatProh = 12;               /* ms60 */
/* DRB LogicalChannelConfig */
CONSTANT U16 veDfltDrbPriority = 6;               /* High 6 */
CONSTANT U16 veDfltDrbPriBitRate = 0;               /* kBps0   */
CONSTANT U16 veDfltDrbBktSzDur = 1;               /* ms100 */
CONSTANT U16 veDfltDrbGrp = 1;               /* High 1*/

/* Ctf Cfg Info */
CONSTANT CmLteUeCategory veDfltUECategory = CM_LTE_UE_CAT_2;  
CONSTANT CtfPdschPaParam veDfltPdschPA = 4;           /* same as veDfltDedPhyCfgPa */
CONSTANT CtfPucchRepFactor veDfltRepFact = CTF_PUCCH_REP_FACTOR_N2;   
CONSTANT U16 veDfltN1PUCCHRep = 0;              
CONSTANT U16 veDfltFormatInd = 1;               /* 1 */
CONSTANT U16 veDfltSubBandK = 0;               
#ifdef LTEMAC_MIMO
CONSTANT Bool veDfltRiCfgPres = TRUE;
CONSTANT U32 veDfltRiCfgIndx = 163;
CONSTANT CtfUeTxAntMode veDfltAntInfoTxMode = CTF_UE_TX_ANT_MODE_TM3; /* CTF_UE_TX_ANT_MODE_TM1 */
#else
CONSTANT CtfUeTxAntMode veDfltAntInfoTxMode = CTF_UE_TX_ANT_MODE_TM1; /* CTF_UE_TX_ANT_MODE_TM3 */
#endif
CONSTANT CtfUeTxAntSelectType veDfltAntennaSel = 0;               /*  Closed 0 | Open 1 */

/* Rgr cfg Info  UE Cfg */
#ifdef LTEMAC_MIMO 
CONSTANT U16 veDfltTxModeEnum = 3;               /* TM3 */
#else
CONSTANT U16 veDfltTxModeEnum = 1;               /* TM1 */
#endif
#ifdef LTEMAC_MIMO
CONSTANT U16 veDfltAPrdModeEnum = 3;      /* MOD30 */
#else
CONSTANT U16 veDfltAPrdModeEnum = 3;      /* MOD30 */
#endif
#ifdef LTEMAC_MIMO
/* Periodic CQI reporting mode */
CONSTANT U16 veDfltPrdModeEnum =  0;      /* MODE 1-0 in case of TM 1,2,3,7 and MODE1-1 
                                        * in case of TM 4,5,6 */    
/* Periodicity values for CQI */
CONSTANT U16 veDfltPrdicityEnum = 2;      
#else
/* Periodic CQI reporting mode */
CONSTANT U16 veDfltPrdModeEnum =  1;      
/* Periodicity values for CQI */
CONSTANT U16 veDfltPrdicityEnum = 2;      
#endif
/* Subframe offset */
CONSTANT U8 veDfltSubframeOffst = 0;      
/* Delta^cqi_offset: (actual_value*10) */
CONSTANT S8 veDfltCqiOffst = 0;      
CONSTANT S8 veDfltCqiCfgk  = 2;      
#ifdef LTEMAC_MIMO
CONSTANT U16 veDfltCqiPmiCfgIdx = 512;
#else
CONSTANT U16 veDfltCqiPmiCfgIdx = 512;      
#endif

#ifdef LTEMAC_MIMO
CONSTANT Bool veCodeBookRstnPres = TRUE;
#else
CONSTANT Bool veCodeBookRstnPres = FALSE;
#endif
CONSTANT U32 vePmiBitMap0 = 0xc0000000; /* corresponds to n2TxAntenna-tm3 */
CONSTANT U32 vePmiBitMap1 = 0;

/*!< Maximum number of UL HARQ transmissions */
/* Reducing from 2 to 1 to limit the retransmissions.
   MAC GPR 4 supports nonadaptive transmissions in the uplink 
   CR fix - ccpu00111695 */ 
#ifdef MSPD
CONSTANT U8 veDfltMaxUlHqTx = 4;      
#else
CONSTANT U8 veDfltMaxUlHqTx = 2;      
#endif
/*!< Delta HARQ offset */
CONSTANT U8 veDfltDeltaHqOffst = 0;    
/* Timer configuration */
#ifdef TA_NEW
CONSTANT U16 veDfltTaTmr = 10240;       /* 10240 Subfrms */
CONSTANT Bool veDfltTaTmrPres = TRUE;       /* 10240 Subfrms */
#else
CONSTANT Bool veDfltTaTmrPres = FALSE;       /* Infinity */
CONSTANT U16 veDfltTaTmr = 0;      /* infinity */
#endif


/* k value: range [1-4] */
CONSTANT U8 veDfltUePrdDlCqiK = 1;
/* CQI to aim for during PUSCH power control */
CONSTANT U8 veDfltUeUlPwrTrgCqi = 9;
CONSTANT U8 veDfltAckNackRepFactor= 4;


/* RGR Cell Cfg */
CONSTANT U8 veDfltMacInst = 0;
#ifdef MSPD
CONSTANT U8 veDfltMaxUePerUlSf = 2;
CONSTANT U8 veDfltMaxUePerDlSf = 2;
#else
CONSTANT U8 veDfltMaxUePerUlSf = 2;
CONSTANT U8 veDfltMaxUePerDlSf = 2;
#endif
/* Configure the below DL/UL bw per UE is as per the:
1. Scheduler at MAC 
2. Bandwidth expected
3. Number of UEs schedulable per TTI against 
   the maximum allowed for a particular bandwidth
   Example: 
   1. UL/veDfltMaxUlBwPerUe: For total of more than 
      1/2 UEs, this shall be set to 20 for RR scheduler. 
   2. DL/veDfltMaxDlBwPerUe: For total of more than
      1/2 UEs, this shall be set to 30 for RR scheduler. 
*/
CONSTANT U8 veDfltMaxUlBwPerUe = 50;
CONSTANT U16 veDfltMaxDlBwPerUe = 50;
CONSTANT U8 veDfltMaxDlRetxBw = 50;
CONSTANT U8 veDfltMaxDlUeNwTxPerTti = 2;
CONSTANT U8 veDfltMaxUlUeNwTxPerTti = 2;

CONSTANT U16 veDfltIsCycPrefULExt = FALSE; /* TRUE-extended */
CONSTANT U16 veDfltIsCycPrefDLExt = FALSE; /* TRUE-extended */
CONSTANT U16 veDfltCellModSchm = FALSE;    /* TRUE indicates 64QAM */
#ifdef LTEMAC_MIMO
CONSTANT U16 veDfltNumOfTxAnt = 2;                    /* Two */
#else
CONSTANT U16 veDfltNumOfTxAnt = 1;                    /* One */
#endif
/* MAC - Different Scheduler Type 
#define RGR_SCH_TYPE_SC1    0
#define RGR_SCH_TYPE_PFS    1
#define RGR_SCH_TYPE_RR     2
#define RGR_SCH_TYPE_MAXCI  3
Configure below the DL and UL scheduler types
as per these values 
*/

#ifdef MSPD
/* Changing from 0 to 2 for RR */
CONSTANT U8 veDfltDlSchdType = 2;  /*  0..(number of schedulers - 1) */ 
#else
CONSTANT U8 veDfltDlSchdType = 0;  /*  0..(number of schedulers - 1) */
#endif
CONSTANT U8 veUlDlCfgIdx = 2;
CONSTANT U8 veSpclSfCfgIdx = 8;     
#ifdef MSPD
CONSTANT U8 veDfltUlSchdType = 2;  /*  0..(number of schedulers - 1) */
CONSTANT U8 veDfltDLFSSchdTyp = 0;  /* DLFS scheduler; 0..(no. of schedulers - 1) */
CONSTANT U8 veDfltMaxDlHqTx = 1;   /* Maximum number of DL HARQ Transmissions */

CONSTANT U8 veDfltMaxMsg4HqTx = 1; /* Maximum msg4 HARQ Transmissions */
const U16 veDfltMacRntisize = 40;   /* Indicates contiguous range of RNTI */
CONSTANT U8 veDfltCfiCfg = 2;    /* CFI for PDCCH: a value in set {1,2,3} */
#else
CONSTANT U8 veDfltUlSchdType = 0;  /*  0..(number of schedulers - 1) */
CONSTANT U8 veDfltDLFSSchdTyp = 0;  /* DLFS scheduler; 0..(no. of schedulers - 1) */
CONSTANT U8 veDfltMaxDlHqTx = 2; /*1;*/   /* Maximum number of DL HARQ Transmissions */
CONSTANT U8 veDfltMaxMsg4HqTx = 1; /* Maximum msg4 HARQ Transmissions */

CONSTANT U16 veDfltMacRntisize = 10;   /* Indicates contiguous range of RNTI */
/* CFI for PDCCH: a value in set {1,2,3} */
CONSTANT U8 veDfltCfiCfg = 1;
#endif
/* Target UL CQI to achieve through power control */
CONSTANT U8 veDfltTrgUlCqi = 9;

/* BCCH on DLSCH, PCH and RARsp coding rate */
CONSTANT U16 veDfltBcchPchRaCodeRate = 0;
/* PDCCH coding rate (for broadcast RNTIs) */
CONSTANT U16 veDfltPdcchCodeRate = 0;
/* Default CQI to be used for Msg4 and UE 
 * which has not yet reported any CQI */
CONSTANT U8 veDfltDlCmnCodRateCcchCqi = 6;
/* Sub-band start */
CONSTANT U8 veDfltPuschSubBndStrt = 1;
 /* Number of equal sized sub-bands */
CONSTANT U8 veDfltPuschNoSubbnds = 8;
/* Size of a sub-band */
CONSTANT U8 veDfltPuschSubBandSz = 6;
/* DMRS information per sub-band */
CONSTANT U8 veDfltPuschSubBandDmrs = 1;
/* CCCH CQI index, also used as default initial CQI for UEs */
#ifdef MSPD
CONSTANT U8 veDfltULCmnCodRateCcchCqi = 11;
#else
CONSTANT U8 veDfltULCmnCodRateCcchCqi = 6;
#endif
/* Indicates if resource allocation is frequency selective or not */
CONSTANT U8 veDfltIsDlFreqSel = FALSE;
/* Threshold CQI value: Threshold CQI value below 
 * which the sub-band is not considered for 
 * allocation for a frequency selective cell */
CONSTANT U8 veDfltThresholdCqi = TRUE;
/*!< Total Dowlink Bandwidth */
CONSTANT U8 veDfltDLTotalBw = 50;
/*!< Total Uplink Bandwidth  */
CONSTANT U8 veDfltULTotalBw = 50;
/*!< Ng value for PHICH */
CONSTANT U16 veDfltNgEnum = RGR_NG_ONESIXTH;
/*!< PHICH Duration: TRUE-extended/FALSE-normal */
CONSTANT U8 veDfltIsDurExt = FALSE;
/*!< PUCCH resource-size or N^(2)_RB (in RBs) */
CONSTANT U8 veDfltPUCCHResSz = 1;
/*!< N^(1)_PUCCH */
CONSTANT U8 veDfltN1PucchAn = 1;
/*!< Delta Shift for PUCCH: a value in set {1,2,3} */
CONSTANT U8 veDfltDeltaShift = 1;
/*!< Cyclic Shift for PUCCH (N^(1)_CS): a value in range [0-7] */
#ifdef MSPD
CONSTANT U8 veDfltPUCCHCycShift = 0;
#else
CONSTANT U8 veDfltPUCCHCycShift = 1;
#endif
/*!< SRS configuration period (in subframes).*/
CONSTANT U16 veDfltSrsCfgPrdEnum = 0;
/*!< SRS Bandwidth configuration per cell Range - [0-7] */
CONSTANT U16 veDfltSrsBwEnum = 0;
/*!< SRS subframe configuration index per cell Range - [0-15] */
CONSTANT U8 veDfltSrsSubFrameCfg = 1;


 /*!< RACH Preamble format: a value in set {0,1,2,3} */
#ifdef LTE_TDD
CONSTANT U8 veDfltPreambleFrmt = 1;
#else
CONSTANT U8 veDfltPreambleFrmt = 0;
#endif
 /*!< RA Window size */
#ifdef MSPD
CONSTANT U8 veDfltRaWinSz = 10;
#else
CONSTANT U8 veDfltRaWinSz = 8;
#endif
/*!< Number of subframe numbers */
CONSTANT U8 veDfltRaOccSz = 1;
 /*!< System Frame Number */
CONSTANT U16 veDfltRaOccSfnEnum = RGR_SFN_ANY;
/*!< Maximum number of message 3 transmissions */
CONSTANT U8 veDfltMaxMsg3Tx = 1;
/*!< Number of RA Preambles */
#ifdef MSPD
CONSTANT U8 veDfltNumRaPreamble = 45;
#else
CONSTANT U8 veDfltNumRaPreamble = 6;
#endif
/*!< Size of RA Preamble in Group A */
CONSTANT U8 veDfltSzRaPreambleGrpA = 5;
 /*!< MESSAGE_SIZE_GROUP_A */
#ifdef MSPD
CONSTANT U16 veDfltMsgSzGrpA = 56;
#else
CONSTANT U16 veDfltMsgSzGrpA = 340;
#endif
/*!< N^RA_PRB: PRACH resource for random access */
/*CONSTANT U8 veDfltPrachResource = 1;*/
/*!< SI window size */
CONSTANT U8 veDfltSiWinSize = 20;  /* In sync with veDfltSiWindowLength */
/*!< Retransmission count */
CONSTANT U8 veDfltRetxCnt = 1;

/*!< P0_NOMINAL_PUSCH*/
CONSTANT S8 veDfltP0NominalPusch = 1;
/*!< Aplha, 3-bit cell-specific parameter*/
CONSTANT U16 veDfltUlPwrCfgAlpha = RGR_PWR_ALPHA0;
/*!< P0_NOMINAL_PUCCH*/
CONSTANT S8 veDfltP0NominalPucch = 1;
/*!< Delta_PREAMBLE_MSG3*/
CONSTANT S8 veDfltDeltaPreambleMsg3 = 1;

CONSTANT U16 veDfltPucchPwrFmt3RntiSt = 1;
CONSTANT U16 veDfltPucchPwrFmt3RntiSz = 1;

CONSTANT U16 veDfltPucchPwrFmt3aRntiSt = 1;
CONSTANT U16 veDfltPucchPwrFmt3aRntiSz = 1;

CONSTANT U16 veDfltPuschPwrFmt3RntiSt = 1;
CONSTANT U16 veDfltPuschPwrFmt3RntiSz = 1;

CONSTANT U16 veDfltPuschPwrFmt3aRntiSt = 1;
CONSTANT U16 veDfltPuschPwrFmt3aRntiSz = 1;

/* Changing from 0 to 1 to configure required DRB QoS,
   instead of qci 0, which allows infinite QoS*/
CONSTANT U32 veDfltUeBr = 80000000; /* 2048000; */
#ifdef MSPD
CONSTANT U32 veDfltQci = 0; 
#else
CONSTANT U32 veDfltQci = 1; 
#endif
CONSTANT U32 veDfltDlQosGbr = 80000000;/* 2048000; */
CONSTANT U32 veDfltDlQosMbr = 80000000;/* 2048000; */

/*!< Number of sub-bands */
CONSTANT U8 veDfltPuschCfgNoSubBands = 1;
/*!< Hopping mode inter/intra subframe */
CONSTANT U8 veDfltPuschCfgIsIntraHop = FALSE;
/*!< Hopping offset {0 ... 98} */
CONSTANT U8 veDfltPuschCfgHopOffst = 0;
/*!< Indicates if other configuration fields are valid */
CONSTANT U8 veDfltPreambleSetCfgPres = FALSE;
/*!< Start Preamble ID for the range managed by MAC */
CONSTANT U8 veDfltPreambleSetCfgStart = 0;
/*!< Indicates contiguous range of premables managaed by MAC */
CONSTANT U8 veDfltPreambleSetCfgSz = 0;
/*!< Number of PRACH resources*/
CONSTANT U8 veDfltPRACHNumRes = 0;
/*!< Number of common logical channels*/
CONSTANT U8 veDfltNumCmnLcs = 6;

/* Cell Selection and Reselection Related Parameters */
/* SIB3 defaults */
CONSTANT U32 veDfltS3nCellChangeMedium = 1;
CONSTANT U32 veDfltS3nCellChangeHigh = 1;
CONSTANT U32 veDfltS3sNonIntraSearch = 0;
CONSTANT U32 veDfltS3ThreshServingLow = 0;
CONSTANT U32 veDfltS3CellReselectionPriority = 4;
CONSTANT U32 veDfltS3qRxLevMin = -70;
CONSTANT U32 veDfltS3pMax = 1; 
CONSTANT U32 veDfltS3sIntraSearch = 1;
CONSTANT U32 veDfltS3AllowedMeasBandwidth = 0;
CONSTANT U32 veDfltS3PresenceAntennaPort1 = FALSE;
CONSTANT U8 veDfltS3NeighCellCfg = 1 << 6;               
CONSTANT U32 veDfltS3tReselectionEUTRA = 0;
/* SIB4 defaults */
CONSTANT U32 veDfltS4MaxCellIntra = 1; 
CONSTANT U32 veDfltS4PhysCellId = 1;
CONSTANT U32 veDfltS4QOffsetCell = 0;
CONSTANT U32 veDfltS4MaxCellBlack = 1;
CONSTANT U32 veDfltS4BCStart = 1;
CONSTANT U32 veDfltS4BCRange = NhuPhysCellIdRangerangen4Enum;
CONSTANT U32 veDfltS4PCStart = 1;
CONSTANT U32 veDfltS4PCRange = NhuPhysCellIdRangerangen4Enum;
/* SIB5 defaults */
CONSTANT U32 veDfltS5DlCarrierFreq = 15;
CONSTANT S32 veDfltS5QRxLevMin = -65;
CONSTANT U32 veDfltS5InterFreqtReselection = 0;
CONSTANT U32 veDfltS5ThreshXHigh = 4;
CONSTANT U32 veDfltS5ThreshXLow = 2;
CONSTANT U32 veDfltS5AllowedMeasBandwidth = NhuAllowedMeasBandwidthmbw25Enum;
CONSTANT U32 veDfltS5PresenceAntennaPort1 = FALSE;
CONSTANT U32 veDfltS5CellReselectionPriority = 4;
CONSTANT U8 veDfltS5NeighCellCfg = 1 << 6;
CONSTANT U32 veDfltS5QOffsetFreq = 0;
#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**

         End of file:     ve_dflt.c@@/main/2 - Wed Dec  1 08:44:23 2010

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
/main/2      ----       rk        1. eNodeb 1.2 Release
/main/2      ve003.102  pkd       1. CR ID- ccpu00116764-IOT changes are incorporated.
*********************************************************************91*/
