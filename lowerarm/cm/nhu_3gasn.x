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

     Name:     LTE RRC Layer

     Type:     structure defintion file

     Desc:     Defines required by RRC

     File:     nhu_3gasn.x

     Sid:      nhu_3gasn.x@@/main/1 - Fri Dec 11 18:45:13 2009

     Prg:      chebli

**********************************************************************/

#ifndef __NHU3GASNX__
#define __NHU3GASNX__ 

/* Element ----- MaxHcContextSpace ------- */
typedef enum enumNhuMaxHcCtxtSpace
{
    NhuMaxHcCtxtSpacedummyEnum,
    NhuMaxHcCtxtSpaceby1024Enum,
    NhuMaxHcCtxtSpaceby2048Enum,
    NhuMaxHcCtxtSpaceby4096Enum,
    NhuMaxHcCtxtSpaceby8192Enum
} EnumNhuMaxHcCtxtSpace;
typedef TknU32 NhuMaxHcCtxtSpace;

/* Element ----- losslessSRNS-RelocationSupport ------- */
typedef TknU8 NhuPDCP_CaplosslessSRNS_RelocationSupport;

typedef struct NhuPDCP_CapsupportForRfc2507
{
    TknU8 choice;
    union {
        NhuMaxHcCtxtSpace supported;
    } val;
} NhuPDCP_CapsupportForRfc2507;

typedef struct NhuPDCP_Cap
{
    TknPres pres;
    NhuPDCP_CaplosslessSRNS_RelocationSupport losslessSRNS_RelocationSupport;
    NhuPDCP_CapsupportForRfc2507 supportForRfc2507;
} NhuPDCP_Cap;

/* Element ----- TotalRLC-AM-BufferSize ------- */
typedef enum enumNhuTotalRLC_AM_BufferSize
{
    NhuTotalRLC_AM_BufferSizedummyEnum,
    NhuTotalRLC_AM_BufferSizekb10Enum,
    NhuTotalRLC_AM_BufferSizekb50Enum,
    NhuTotalRLC_AM_BufferSizekb100Enum,
    NhuTotalRLC_AM_BufferSizekb150Enum,
    NhuTotalRLC_AM_BufferSizekb500Enum,
    NhuTotalRLC_AM_BufferSizekb1000Enum,
    NhuTotalRLC_AM_BufferSizespareEnum
} EnumNhuTotalRLC_AM_BufferSize;
typedef TknU32 NhuTotalRLC_AM_BufferSize;

/* Element ----- MaximumRLC-WindowSize ------- */
typedef enum enumNhuMaxRLC_WindowSize
{
    NhuMaxRLC_WindowSizemws2047Enum,
    NhuMaxRLC_WindowSizemws4095Enum
} EnumNhuMaxRLC_WindowSize;
typedef TknU32 NhuMaxRLC_WindowSize;

/* Element ----- MaximumAM-EntityNumberRLC-Cap ------- */
typedef enum enumNhuMaxAM_EntityNumberRLC_Cap
{
    NhuMaxAM_EntityNumberRLC_CapdummyEnum,
    NhuMaxAM_EntityNumberRLC_Capam4Enum,
    NhuMaxAM_EntityNumberRLC_Capam5Enum,
    NhuMaxAM_EntityNumberRLC_Capam6Enum,
    NhuMaxAM_EntityNumberRLC_Capam8Enum,
    NhuMaxAM_EntityNumberRLC_Capam16Enum,
    NhuMaxAM_EntityNumberRLC_Capam30Enum
} EnumNhuMaxAM_EntityNumberRLC_Cap;
typedef TknU32 NhuMaxAM_EntityNumberRLC_Cap;

typedef struct NhuRLC_Cap
{
    TknPres pres;
    NhuTotalRLC_AM_BufferSize totalRLC_AM_BufferSize;
    NhuMaxRLC_WindowSize maximumRLC_WindowSize;
    NhuMaxAM_EntityNumberRLC_Cap maximumAM_EntityNumber;
} NhuRLC_Cap;

/* Element ----- MaxNoBits ------- */
typedef enum enumNhuMaxNoBits
{
    NhuMaxNoBitsb640Enum,
    NhuMaxNoBitsb1280Enum,
    NhuMaxNoBitsb2560Enum,
    NhuMaxNoBitsb3840Enum,
    NhuMaxNoBitsb5120Enum,
    NhuMaxNoBitsb6400Enum,
    NhuMaxNoBitsb7680Enum,
    NhuMaxNoBitsb8960Enum,
    NhuMaxNoBitsb10240Enum,
    NhuMaxNoBitsb20480Enum,
    NhuMaxNoBitsb40960Enum,
    NhuMaxNoBitsb81920Enum,
    NhuMaxNoBitsb163840Enum
} EnumNhuMaxNoBits;
typedef TknU32 NhuMaxNoBits;

typedef struct NhuTurboSupport
{
    TknU8 choice;
    union {
        NhuMaxNoBits supported;
    } val;
} NhuTurboSupport;

/* Element ----- MaxSimultaneousTransChsDL ------- */
typedef enum enumNhuMaxSimultaneousTransChsDL
{
    NhuMaxSimultaneousTransChsDLe4Enum,
    NhuMaxSimultaneousTransChsDLe8Enum,
    NhuMaxSimultaneousTransChsDLe16Enum,
    NhuMaxSimultaneousTransChsDLe32Enum
} EnumNhuMaxSimultaneousTransChsDL;
typedef TknU32 NhuMaxSimultaneousTransChsDL;

/* Element ----- MaxSimultaneousCCTrCH-Count ------- */
typedef TknU32 NhuMaxSimultaneousCCTrCH_Count;

/* Element ----- MaxTransportBlocksDL ------- */
typedef enum enumNhuMaxTportBlocksDL
{
    NhuMaxTportBlocksDLtb4Enum,
    NhuMaxTportBlocksDLtb8Enum,
    NhuMaxTportBlocksDLtb16Enum,
    NhuMaxTportBlocksDLtb32Enum,
    NhuMaxTportBlocksDLtb48Enum,
    NhuMaxTportBlocksDLtb64Enum,
    NhuMaxTportBlocksDLtb96Enum,
    NhuMaxTportBlocksDLtb128Enum,
    NhuMaxTportBlocksDLtb256Enum,
    NhuMaxTportBlocksDLtb512Enum
} EnumNhuMaxTportBlocksDL;
typedef TknU32 NhuMaxTportBlocksDL;

/* Element ----- MaxNumberOfTFC-DL ------- */
typedef enum enumNhuMaxNumberOfTFC_DL
{
    NhuMaxNumberOfTFC_DLtfc16Enum,
    NhuMaxNumberOfTFC_DLtfc32Enum,
    NhuMaxNumberOfTFC_DLtfc48Enum,
    NhuMaxNumberOfTFC_DLtfc64Enum,
    NhuMaxNumberOfTFC_DLtfc96Enum,
    NhuMaxNumberOfTFC_DLtfc128Enum,
    NhuMaxNumberOfTFC_DLtfc256Enum,
    NhuMaxNumberOfTFC_DLtfc512Enum,
    NhuMaxNumberOfTFC_DLtfc1024Enum
} EnumNhuMaxNumberOfTFC_DL;
typedef TknU32 NhuMaxNumberOfTFC_DL;

/* Element ----- MaxNumberOfTF ------- */
typedef enum enumNhuMaxNumberOfTF
{
    NhuMaxNumberOfTFtf32Enum,
    NhuMaxNumberOfTFtf64Enum,
    NhuMaxNumberOfTFtf128Enum,
    NhuMaxNumberOfTFtf256Enum,
    NhuMaxNumberOfTFtf512Enum,
    NhuMaxNumberOfTFtf1024Enum
} EnumNhuMaxNumberOfTF;
typedef TknU32 NhuMaxNumberOfTF;

typedef struct NhuDL_TransChCap
{
    TknPres pres;
    NhuMaxNoBits maxNoBitsReceived;
    NhuMaxNoBits maxConvCodeBitsReceived;
    NhuTurboSupport turboDecodingSupport;
    NhuMaxSimultaneousTransChsDL maxSimultaneousTransChs;
    NhuMaxSimultaneousCCTrCH_Count maxSimultaneousCCTrCH_Count;
    NhuMaxTportBlocksDL maxReceivedTportBlocks;
    NhuMaxNumberOfTFC_DL maxNumberOfTFC;
    NhuMaxNumberOfTF maxNumberOfTF;
} NhuDL_TransChCap;

/* Element ----- MaxSimultaneousTransChsUL ------- */
typedef enum enumNhuMaxSimultaneousTransChsUL
{
    NhuMaxSimultaneousTransChsULdummyEnum,
    NhuMaxSimultaneousTransChsULe4Enum,
    NhuMaxSimultaneousTransChsULe8Enum,
    NhuMaxSimultaneousTransChsULe16Enum,
    NhuMaxSimultaneousTransChsULe32Enum
} EnumNhuMaxSimultaneousTransChsUL;
typedef TknU32 NhuMaxSimultaneousTransChsUL;

/* Element ----- MaxTransportBlocksUL ------- */
typedef enum enumNhuMaxTportBlocksUL
{
    NhuMaxTportBlocksULdummyEnum,
    NhuMaxTportBlocksULtb4Enum,
    NhuMaxTportBlocksULtb8Enum,
    NhuMaxTportBlocksULtb16Enum,
    NhuMaxTportBlocksULtb32Enum,
    NhuMaxTportBlocksULtb48Enum,
    NhuMaxTportBlocksULtb64Enum,
    NhuMaxTportBlocksULtb96Enum,
    NhuMaxTportBlocksULtb128Enum,
    NhuMaxTportBlocksULtb256Enum,
    NhuMaxTportBlocksULtb512Enum
} EnumNhuMaxTportBlocksUL;
typedef TknU32 NhuMaxTportBlocksUL;

/* Element ----- MaxNumberOfTFC-UL ------- */
typedef enum enumNhuMaxNumberOfTFC_UL
{
    NhuMaxNumberOfTFC_ULdummy1Enum,
    NhuMaxNumberOfTFC_ULdummy2Enum,
    NhuMaxNumberOfTFC_ULtfc16Enum,
    NhuMaxNumberOfTFC_ULtfc32Enum,
    NhuMaxNumberOfTFC_ULtfc48Enum,
    NhuMaxNumberOfTFC_ULtfc64Enum,
    NhuMaxNumberOfTFC_ULtfc96Enum,
    NhuMaxNumberOfTFC_ULtfc128Enum,
    NhuMaxNumberOfTFC_ULtfc256Enum,
    NhuMaxNumberOfTFC_ULtfc512Enum,
    NhuMaxNumberOfTFC_ULtfc1024Enum
} EnumNhuMaxNumberOfTFC_UL;
typedef TknU32 NhuMaxNumberOfTFC_UL;

typedef struct NhuUL_TransChCapmodeSpecificInfotdd
{
    TknPres pres;
    NhuMaxSimultaneousCCTrCH_Count maxSimultaneousCCTrCH_Count;
} NhuUL_TransChCapmodeSpecificInfotdd;

typedef struct NhuUL_TransChCapmodeSpecificInfo
{
    TknU8 choice;
    union {
        NhuUL_TransChCapmodeSpecificInfotdd tdd;
    } val;
} NhuUL_TransChCapmodeSpecificInfo;

typedef struct NhuUL_TransChCap
{
    TknPres pres;
    NhuMaxNoBits maxNoBitsTransmitted;
    NhuMaxNoBits maxConvCodeBitsTransmitted;
    NhuTurboSupport turboEncodingSupport;
    NhuMaxSimultaneousTransChsUL maxSimultaneousTransChs;
    NhuUL_TransChCapmodeSpecificInfo modeSpecificInfo;
    NhuMaxTportBlocksUL maxTransmittedBlocks;
    NhuMaxNumberOfTFC_UL maxNumberOfTFC;
    NhuMaxNumberOfTF maxNumberOfTF;
} NhuUL_TransChCap;

typedef struct NhuTportChannelCap
{
    TknPres pres;
    NhuDL_TransChCap dl_TransChCap;
    NhuUL_TransChCap ul_TransChCap;
} NhuTportChannelCap;

/* Element ----- UE-PowerClass ------- */
typedef TknU32 NhuUE_PowerClass;

/* Element ----- TxRxFrequencySeparation ------- */
typedef enum enumNhuTxRxFrequencySeparation
{
    NhuTxRxFrequencySeparationdefault_TxRx_separationEnum,
    NhuTxRxFrequencySeparationspare2Enum,
    NhuTxRxFrequencySeparationspare1Enum
} EnumNhuTxRxFrequencySeparation;
typedef TknU32 NhuTxRxFrequencySeparation;

/* Element ----- RadioFrequencyBandTDDList ------- */
typedef enum enumNhuRadioFrequencyBandTDDLst
{
    NhuRadioFrequencyBandTDDLstaEnum,
    NhuRadioFrequencyBandTDDLstbEnum,
    NhuRadioFrequencyBandTDDLstcEnum,
    NhuRadioFrequencyBandTDDLstabEnum,
    NhuRadioFrequencyBandTDDLstacEnum,
    NhuRadioFrequencyBandTDDLstbcEnum,
    NhuRadioFrequencyBandTDDLstabcEnum,
    NhuRadioFrequencyBandTDDLstspareEnum
} EnumNhuRadioFrequencyBandTDDLst;
typedef TknU32 NhuRadioFrequencyBandTDDLst;

/* Element ----- ChipRateCapability ------- */
typedef enum enumNhuChipRateCap
{
    NhuChipRateCapmcps3_84Enum,
    NhuChipRateCapmcps1_28Enum
} EnumNhuChipRateCap;
typedef TknU32 NhuChipRateCap;

typedef struct NhuRF_CapfddRF_Cap
{
    TknPres pres;
    NhuUE_PowerClass ue_PowerClass;
    NhuTxRxFrequencySeparation txRxFrequencySeparation;
} NhuRF_CapfddRF_Cap;

typedef struct NhuRF_CaptddRF_Cap
{
    TknPres pres;
    NhuUE_PowerClass ue_PowerClass;
    NhuRadioFrequencyBandTDDLst radioFrequencyTDDBandLst;
    NhuChipRateCap chipRateCap;
} NhuRF_CaptddRF_Cap;

typedef struct NhuRF_Cap
{
    TknPres pres;
    NhuRF_CapfddRF_Cap fddRF_Cap;
    NhuRF_CaptddRF_Cap tddRF_Cap;
} NhuRF_Cap;

/* Element ----- MaxNoPhysChBitsReceived ------- */
typedef enum enumNhuMaxNoPhysChBitsReceived
{
    NhuMaxNoPhysChBitsReceiveddummyEnum,
    NhuMaxNoPhysChBitsReceivedb1200Enum,
    NhuMaxNoPhysChBitsReceivedb2400Enum,
    NhuMaxNoPhysChBitsReceivedb3600Enum,
    NhuMaxNoPhysChBitsReceivedb4800Enum,
    NhuMaxNoPhysChBitsReceivedb7200Enum,
    NhuMaxNoPhysChBitsReceivedb9600Enum,
    NhuMaxNoPhysChBitsReceivedb14400Enum,
    NhuMaxNoPhysChBitsReceivedb19200Enum,
    NhuMaxNoPhysChBitsReceivedb28800Enum,
    NhuMaxNoPhysChBitsReceivedb38400Enum,
    NhuMaxNoPhysChBitsReceivedb48000Enum,
    NhuMaxNoPhysChBitsReceivedb57600Enum,
    NhuMaxNoPhysChBitsReceivedb67200Enum,
    NhuMaxNoPhysChBitsReceivedb76800Enum
} EnumNhuMaxNoPhysChBitsReceived;
typedef TknU32 NhuMaxNoPhysChBitsReceived;

/* Element ----- MaxNoSCCPCH-RL ------- */
typedef enum enumNhuMaxNoSCCPCH_RL
{
    NhuMaxNoSCCPCH_RLrl1Enum
} EnumNhuMaxNoSCCPCH_RL;
typedef TknU32 NhuMaxNoSCCPCH_RL;

/* Element ----- simultaneousSCCPCH-DPCH-DPDCH-Reception ------- */
typedef TknU8 NhuSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception;

typedef struct NhuSimultaneousSCCPCH_DPCH_Receptionsupported
{
    TknPres pres;
    NhuMaxNoSCCPCH_RL maxNoSCCPCH_RL;
    NhuSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception simultaneousSCCPCH_DPCH_DPDCH_Reception;
} NhuSimultaneousSCCPCH_DPCH_Receptionsupported;

typedef struct NhuSimultaneousSCCPCH_DPCH_Reception
{
    TknU8 choice;
    union {
        NhuSimultaneousSCCPCH_DPCH_Receptionsupported supported;
    } val;
} NhuSimultaneousSCCPCH_DPCH_Reception;

/* Element ----- maxNoDPCH-PDSCH-Codes ------- */
typedef TknU32 NhuDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes;

/* Element ----- supportForSF-512 ------- */
typedef TknU8 NhuDL_PhysChCapFDDsupportForSF_512;

/* Element ----- dummy ------- */
typedef TknU8 NhuDL_PhysChCapFDDdummy;

typedef struct NhuDL_PhysChCapFDD
{
    TknPres pres;
    NhuDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes maxNoDPCH_PDSCH_Codes;
    NhuMaxNoPhysChBitsReceived maxNoPhysChBitsReceived;
    NhuDL_PhysChCapFDDsupportForSF_512 supportForSF_512;
    NhuDL_PhysChCapFDDdummy dummy;
    NhuSimultaneousSCCPCH_DPCH_Reception dummy2;
} NhuDL_PhysChCapFDD;

/* Element ----- MaxNoDPDCH-BitsTransmitted ------- */
typedef enum enumNhuMaxNoDPDCH_BitsTransmitted
{
    NhuMaxNoDPDCH_BitsTransmittedb600Enum,
    NhuMaxNoDPDCH_BitsTransmittedb1200Enum,
    NhuMaxNoDPDCH_BitsTransmittedb2400Enum,
    NhuMaxNoDPDCH_BitsTransmittedb4800Enum,
    NhuMaxNoDPDCH_BitsTransmittedb9600Enum,
    NhuMaxNoDPDCH_BitsTransmittedb19200Enum,
    NhuMaxNoDPDCH_BitsTransmittedb28800Enum,
    NhuMaxNoDPDCH_BitsTransmittedb38400Enum,
    NhuMaxNoDPDCH_BitsTransmittedb48000Enum,
    NhuMaxNoDPDCH_BitsTransmittedb57600Enum
} EnumNhuMaxNoDPDCH_BitsTransmitted;
typedef TknU32 NhuMaxNoDPDCH_BitsTransmitted;

/* Element ----- dummy ------- */
typedef TknU8 NhuUL_PhysChCapFDDdummy;

typedef struct NhuUL_PhysChCapFDD
{
    TknPres pres;
    NhuMaxNoDPDCH_BitsTransmitted maxNoDPDCH_BitsTransmitted;
    NhuUL_PhysChCapFDDdummy dummy;
} NhuUL_PhysChCapFDD;

/* Element ----- MaxTS-PerFrame ------- */
typedef TknU32 NhuMaxTS_PerFrame;

/* Element ----- MaxPhysChPerFrame ------- */
typedef TknU32 NhuMaxPhysChPerFrame;

/* Element ----- MinimumSF-DL ------- */
typedef enum enumNhuMinimumSF_DL
{
    NhuMinimumSF_DLsf1Enum,
    NhuMinimumSF_DLsf16Enum
} EnumNhuMinimumSF_DL;
typedef TknU32 NhuMinimumSF_DL;

/* Element ----- MaxPhysChPerTS ------- */
typedef TknU32 NhuMaxPhysChPerTS;

/* Element ----- supportOfPDSCH ------- */
typedef TknU8 NhuDL_PhysChCapTDDsupportOfPDSCH;

typedef struct NhuDL_PhysChCapTDD
{
    TknPres pres;
    NhuMaxTS_PerFrame maxTS_PerFrame;
    NhuMaxPhysChPerFrame maxPhysChPerFrame;
    NhuMinimumSF_DL minimumSF;
    NhuDL_PhysChCapTDDsupportOfPDSCH supportOfPDSCH;
    NhuMaxPhysChPerTS maxPhysChPerTS;
} NhuDL_PhysChCapTDD;

/* Element ----- MaxPhysChPerTimeslot ------- */
typedef enum enumNhuMaxPhysChPerTimeslot
{
    NhuMaxPhysChPerTimeslotts1Enum,
    NhuMaxPhysChPerTimeslotts2Enum
} EnumNhuMaxPhysChPerTimeslot;
typedef TknU32 NhuMaxPhysChPerTimeslot;

/* Element ----- MinimumSF-UL ------- */
typedef enum enumNhuMinimumSF_UL
{
    NhuMinimumSF_ULsf1Enum,
    NhuMinimumSF_ULsf2Enum,
    NhuMinimumSF_ULsf4Enum,
    NhuMinimumSF_ULsf8Enum,
    NhuMinimumSF_ULdummyEnum
} EnumNhuMinimumSF_UL;
typedef TknU32 NhuMinimumSF_UL;

/* Element ----- supportOfPUSCH ------- */
typedef TknU8 NhuUL_PhysChCapTDDsupportOfPUSCH;

typedef struct NhuUL_PhysChCapTDD
{
    TknPres pres;
    NhuMaxTS_PerFrame maxTS_PerFrame;
    NhuMaxPhysChPerTimeslot maxPhysChPerTimeslot;
    NhuMinimumSF_UL minimumSF;
    NhuUL_PhysChCapTDDsupportOfPUSCH supportOfPUSCH;
} NhuUL_PhysChCapTDD;

typedef struct NhuPhysicalChannelCapfddPhysChCap
{
    TknPres pres;
    NhuDL_PhysChCapFDD downlinkPhysChCap;
    NhuUL_PhysChCapFDD uplinkPhysChCap;
} NhuPhysicalChannelCapfddPhysChCap;

typedef struct NhuPhysicalChannelCaptddPhysChCap
{
    TknPres pres;
    NhuDL_PhysChCapTDD downlinkPhysChCap;
    NhuUL_PhysChCapTDD uplinkPhysChCap;
} NhuPhysicalChannelCaptddPhysChCap;

typedef struct NhuPhysicalChannelCap
{
    TknPres pres;
    NhuPhysicalChannelCapfddPhysChCap fddPhysChCap;
    NhuPhysicalChannelCaptddPhysChCap tddPhysChCap;
} NhuPhysicalChannelCap;

/* Element ----- supportOfGSM ------- */
typedef TknU8 NhuMultiRAT_CapsupportOfGSM;

/* Element ----- supportOfMulticarrier ------- */
typedef TknU8 NhuMultiRAT_CapsupportOfMulticarrier;

typedef struct NhuMultiRAT_Cap
{
    TknPres pres;
    NhuMultiRAT_CapsupportOfGSM supportOfGSM;
    NhuMultiRAT_CapsupportOfMulticarrier supportOfMulticarrier;
} NhuMultiRAT_Cap;

/* Element ----- MultiModeCapability ------- */
typedef enum enumNhuMultiModeCap
{
    NhuMultiModeCaptddEnum,
    NhuMultiModeCapfddEnum,
    NhuMultiModeCapfdd_tddEnum
} EnumNhuMultiModeCap;
typedef TknU32 NhuMultiModeCap;

typedef struct NhuUE_MultiModeRAT_Cap
{
    TknPres pres;
    NhuMultiRAT_Cap multiRAT_CapLst;
    NhuMultiModeCap multiModeCap;
} NhuUE_MultiModeRAT_Cap;

/* Element ---------- cipheringAlgorithmCap ----------- */
typedef TknStrBSXL NhuSecCapcipheringAlgorithmCap;
/* Element ---------- integrityProtectionAlgorithmCap ----------- */
typedef TknStrBSXL NhuSecCapintegrityProtectionAlgorithmCap;
typedef struct NhuSecCap
{
    TknPres pres;
    NhuSecCapcipheringAlgorithmCap cipheringAlgorithmCap;
    NhuSecCapintegrityProtectionAlgorithmCap integrityProtectionAlgorithmCap;
} NhuSecCap;

/* Element ----- NetworkAssistedGPS-Supported ------- */
typedef enum enumNhuNwAssistedGPS_Supp
{
    NhuNwAssistedGPS_SuppnetworkBasedEnum,
    NhuNwAssistedGPS_Suppue_BasedEnum,
    NhuNwAssistedGPS_SuppbothNwAndUE_BasedEnum,
    NhuNwAssistedGPS_SuppnoNwAssistedGPSEnum
} EnumNhuNwAssistedGPS_Supp;
typedef TknU32 NhuNwAssistedGPS_Supp;

/* Element ----- standaloneLocMethodsSupported ------- */
typedef TknU8 NhuUE_Positioning_CapstandaloneLocMethodsSupp;

/* Element ----- ue-BasedOTDOA-Supported ------- */
typedef TknU8 NhuUE_Positioning_Capue_BasedOTDOA_Supp;

/* Element ----- supportForUE-GPS-TimingOfCellFrames ------- */
typedef TknU8 NhuUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames;

/* Element ----- supportForIPDL ------- */
typedef TknU8 NhuUE_Positioning_CapsupportForIPDL;

typedef struct NhuUE_Positioning_Cap
{
    TknPres pres;
    NhuUE_Positioning_CapstandaloneLocMethodsSupp standaloneLocMethodsSupp;
    NhuUE_Positioning_Capue_BasedOTDOA_Supp ue_BasedOTDOA_Supp;
    NhuNwAssistedGPS_Supp networkAssistedGPS_Supp;
    NhuUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames supportForUE_GPS_TimingOfCellFrames;
    NhuUE_Positioning_CapsupportForIPDL supportForIPDL;
} NhuUE_Positioning_Cap;

/* Element ----- gsm900 ------- */
typedef TknU8 NhuGSM_Measurementsgsm900;

/* Element ----- dcs1800 ------- */
typedef TknU8 NhuGSM_Measurementsdcs1800;

/* Element ----- gsm1900 ------- */
typedef TknU8 NhuGSM_Measurementsgsm1900;

typedef struct NhuGSM_Measurements
{
    TknPres pres;
    NhuGSM_Measurementsgsm900 gsm900;
    NhuGSM_Measurementsdcs1800 dcs1800;
    NhuGSM_Measurementsgsm1900 gsm1900;
} NhuGSM_Measurements;

/* Element ----- fdd-Measurements ------- */
typedef TknU8 NhuCompressedModeMeasCapfdd_Measurements;

/* Element ----- tdd-Measurements ------- */
typedef TknU8 NhuCompressedModeMeasCaptdd_Measurements;

/* Element ----- multiCarrierMeasurements ------- */
typedef TknU8 NhuCompressedModeMeasCapmultiCarrierMeasurements;

typedef struct NhuCompressedModeMeasCap
{
    TknPres pres;
    NhuCompressedModeMeasCapfdd_Measurements fdd_Measurements;
    NhuCompressedModeMeasCaptdd_Measurements tdd_Measurements;
    NhuGSM_Measurements gsm_Measurements;
    NhuCompressedModeMeasCapmultiCarrierMeasurements multiCarrierMeasurements;
} NhuCompressedModeMeasCap;

typedef struct NhuMeasurementCap
{
    TknPres pres;
    NhuCompressedModeMeasCap downlinkCompressedMode;
    NhuCompressedModeMeasCap uplinkCompressedMode;
} NhuMeasurementCap;

typedef struct NhuUE_RadioAccessCap
{
    TknPres pres;
    NhuPDCP_Cap pdcp_Cap;
    NhuRLC_Cap rlc_Cap;
    NhuTportChannelCap transportChannelCap;
    NhuRF_Cap rf_Cap;
    NhuPhysicalChannelCap physicalChannelCap;
    NhuUE_MultiModeRAT_Cap ue_MultiModeRAT_Cap;
    NhuSecCap securityCap;
    NhuUE_Positioning_Cap ue_positioning_Cap;
    NhuMeasurementCap measurementCap;
} NhuUE_RadioAccessCap;

/* Element ----- RadioFrequencyBandFDD ------- */
typedef enum enumNhuRadioFrequencyBandFDD
{
    NhuRadioFrequencyBandFDDfdd2100Enum,
    NhuRadioFrequencyBandFDDfdd1900Enum,
    NhuRadioFrequencyBandFDDfdd1800Enum,
    NhuRadioFrequencyBandFDDbandVIEnum,
    NhuRadioFrequencyBandFDDbandIVEnum,
    NhuRadioFrequencyBandFDDbandVEnum,
    NhuRadioFrequencyBandFDDbandVIIEnum,
    NhuRadioFrequencyBandFDDextension_indicatorEnum
} EnumNhuRadioFrequencyBandFDD;
typedef TknU32 NhuRadioFrequencyBandFDD;

/* Element ----- UE-PowerClassExt ------- */
typedef enum enumNhuUE_PowerClassExt
{
    NhuUE_PowerClassExtclass1Enum,
    NhuUE_PowerClassExtclass2Enum,
    NhuUE_PowerClassExtclass3Enum,
    NhuUE_PowerClassExtclass4Enum,
    NhuUE_PowerClassExtspare4Enum,
    NhuUE_PowerClassExtspare3Enum,
    NhuUE_PowerClassExtspare2Enum,
    NhuUE_PowerClassExtspare1Enum
} EnumNhuUE_PowerClassExt;
typedef TknU32 NhuUE_PowerClassExt;

/* Element ----- dl-MeasurementsFDD ------- */
typedef TknU8 NhuCompressedModeMeasCapabFDDdl_MeasurementsFDD;

/* Element ----- ul-MeasurementsFDD ------- */
typedef TknU8 NhuCompressedModeMeasCapabFDDul_MeasurementsFDD;

typedef struct NhuCompressedModeMeasCapabFDD
{
    TknPres pres;
    NhuRadioFrequencyBandFDD radioFrequencyBandFDD;
    NhuCompressedModeMeasCapabFDDdl_MeasurementsFDD dl_MeasurementsFDD;
    NhuCompressedModeMeasCapabFDDul_MeasurementsFDD ul_MeasurementsFDD;
} NhuCompressedModeMeasCapabFDD;

typedef struct NhuCompressedModeMeasCapabFDDLst
{
    TknU16 noComp;
    NhuCompressedModeMeasCapabFDD *member;
}NhuCompressedModeMeasCapabFDDLst;

/* Element ----- RadioFrequencyBandTDD ------- */
typedef enum enumNhuRadioFrequencyBandTDD
{
    NhuRadioFrequencyBandTDDaEnum,
    NhuRadioFrequencyBandTDDbEnum,
    NhuRadioFrequencyBandTDDcEnum,
    NhuRadioFrequencyBandTDDdEnum
} EnumNhuRadioFrequencyBandTDD;
typedef TknU32 NhuRadioFrequencyBandTDD;

/* Element ----- dl-MeasurementsTDD ------- */
typedef TknU8 NhuCompressedModeMeasCapabTDDdl_MeasurementsTDD;

/* Element ----- ul-MeasurementsTDD ------- */
typedef TknU8 NhuCompressedModeMeasCapabTDDul_MeasurementsTDD;

typedef struct NhuCompressedModeMeasCapabTDD
{
    TknPres pres;
    NhuRadioFrequencyBandTDD radioFrequencyBandTDD;
    NhuCompressedModeMeasCapabTDDdl_MeasurementsTDD dl_MeasurementsTDD;
    NhuCompressedModeMeasCapabTDDul_MeasurementsTDD ul_MeasurementsTDD;
} NhuCompressedModeMeasCapabTDD;

typedef struct NhuCompressedModeMeasCapabTDDLst
{
    TknU16 noComp;
    NhuCompressedModeMeasCapabTDD *member;
}NhuCompressedModeMeasCapabTDDLst;

/* Element ----- RadioFrequencyBandGSM ------- */
typedef enum enumNhuRadioFrequencyBandGSM
{
    NhuRadioFrequencyBandGSMgsm450Enum,
    NhuRadioFrequencyBandGSMgsm480Enum,
    NhuRadioFrequencyBandGSMgsm850Enum,
    NhuRadioFrequencyBandGSMgsm900PEnum,
    NhuRadioFrequencyBandGSMgsm900EEnum,
    NhuRadioFrequencyBandGSMgsm1800Enum,
    NhuRadioFrequencyBandGSMgsm1900Enum,
    NhuRadioFrequencyBandGSMspare9Enum,
    NhuRadioFrequencyBandGSMspare8Enum,
    NhuRadioFrequencyBandGSMspare7Enum,
    NhuRadioFrequencyBandGSMspare6Enum,
    NhuRadioFrequencyBandGSMspare5Enum,
    NhuRadioFrequencyBandGSMspare4Enum,
    NhuRadioFrequencyBandGSMspare3Enum,
    NhuRadioFrequencyBandGSMspare2Enum,
    NhuRadioFrequencyBandGSMspare1Enum
} EnumNhuRadioFrequencyBandGSM;
typedef TknU32 NhuRadioFrequencyBandGSM;

/* Element ----- dl-MeasurementsGSM ------- */
typedef TknU8 NhuCompressedModeMeasCapabGSMdl_MeasurementsGSM;

/* Element ----- ul-MeasurementsGSM ------- */
typedef TknU8 NhuCompressedModeMeasCapabGSMul_MeasurementsGSM;

typedef struct NhuCompressedModeMeasCapabGSM
{
    TknPres pres;
    NhuRadioFrequencyBandGSM radioFrequencyBandGSM;
    NhuCompressedModeMeasCapabGSMdl_MeasurementsGSM dl_MeasurementsGSM;
    NhuCompressedModeMeasCapabGSMul_MeasurementsGSM ul_MeasurementsGSM;
} NhuCompressedModeMeasCapabGSM;

typedef struct NhuCompressedModeMeasCapabGSMLst
{
    TknU16 noComp;
    NhuCompressedModeMeasCapabGSM *member;
}NhuCompressedModeMeasCapabGSMLst;

/* Element ----- dl-MeasurementsMC ------- */
typedef TknU8 NhuCompressedModeMeasCapabMCdl_MeasurementsMC;

/* Element ----- ul-MeasurementsMC ------- */
typedef TknU8 NhuCompressedModeMeasCapabMCul_MeasurementsMC;

typedef struct NhuCompressedModeMeasCapabMC
{
    TknPres pres;
    NhuCompressedModeMeasCapabMCdl_MeasurementsMC dl_MeasurementsMC;
    NhuCompressedModeMeasCapabMCul_MeasurementsMC ul_MeasurementsMC;
} NhuCompressedModeMeasCapabMC;

typedef struct NhuMeasurementCapExt
{
    TknPres pres;
    NhuCompressedModeMeasCapabFDDLst compressedModeMeasCapabFDDLst;
    NhuCompressedModeMeasCapabTDDLst compressedModeMeasCapabTDDLst;
    NhuCompressedModeMeasCapabGSMLst compressedModeMeasCapabGSMLst;
    NhuCompressedModeMeasCapabMC compressedModeMeasCapabMC;
} NhuMeasurementCapExt;

typedef struct NhuUE_RadioAccessCapabBandFDDfddRF_Cap
{
    TknPres pres;
    NhuUE_PowerClassExt ue_PowerClass;
    NhuTxRxFrequencySeparation txRxFrequencySeparation;
} NhuUE_RadioAccessCapabBandFDDfddRF_Cap;

typedef struct NhuUE_RadioAccessCapabBandFDD
{
    TknPres pres;
    NhuRadioFrequencyBandFDD radioFrequencyBandFDD;
    NhuUE_RadioAccessCapabBandFDDfddRF_Cap fddRF_Cap;
    NhuMeasurementCapExt measurementCap;
} NhuUE_RadioAccessCapabBandFDD;

typedef struct NhuUE_RadioAccessCapabBandFDDLst
{
    TknU16 noComp;
    NhuUE_RadioAccessCapabBandFDD *member;
}NhuUE_RadioAccessCapabBandFDDLst;

typedef struct NhuUE_RadioAccessCap_v370ext
{
    TknPres pres;
    NhuUE_RadioAccessCapabBandFDDLst ue_RadioAccessCapabBandFDDLst;
} NhuUE_RadioAccessCap_v370ext;

typedef struct NhuUTRANUE_RadioAccessCapInfo
{
    TknPres pres;
    NhuUE_RadioAccessCap ue_RadioAccessCap;
    NhuUE_RadioAccessCap_v370ext ue_RadioAccessCap_v370ext;
} NhuUTRANUE_RadioAccessCapInfo;

/* Element ----- PredefinedConfigValueTag ------- */
typedef TknU32 NhuPredefinedConfigValueTag;

typedef struct NhuPredefinedConfigStatusInfoother
{
    TknU8 choice;
    union {
        NhuPredefinedConfigValueTag storedWithDifferentValueTag;
    } val;
} NhuPredefinedConfigStatusInfoother;

typedef struct NhuPredefinedConfigStatusInfo
{
    TknU8 choice;
    union {
        NhuPredefinedConfigStatusInfoother other;
    } val;
} NhuPredefinedConfigStatusInfo;

typedef struct NhuPredefinedConfigStatusLst
{
    TknU16 noComp;
    NhuPredefinedConfigStatusInfo *member;
}NhuPredefinedConfigStatusLst;

/* Element ---------- START-Value ----------- */
typedef TknStrBSXL NhuSTART_Value;
typedef struct NhuUE_SecInfo
{
    TknPres pres;
    NhuSTART_Value start_CS;
} NhuUE_SecInfo;

/* Element ----- rx-tx-TimeDifferenceType2Capable ------- */
typedef TknU8 NhuUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable;

typedef struct NhuUE_PositioningCapExt_v380
{
    TknPres pres;
    NhuUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable rx_tx_TimeDifferenceTyp2Capable;
} NhuUE_PositioningCapExt_v380;

typedef struct NhuUE_RadioAccessCap_v380ext
{
    TknPres pres;
    NhuUE_PositioningCapExt_v380 ue_PositioningCapExt_v380;
} NhuUE_RadioAccessCap_v380ext;

/* Element ----- SupportOfDedicatedPilotsForChEstimation ------- */
typedef enum enumNhuSupportOfDedicatedPilotsForChEstimation
{
    NhuSupportOfDedicatedPilotsForChEstimationtrueEnum
} EnumNhuSupportOfDedicatedPilotsForChEstimation;
typedef TknU32 NhuSupportOfDedicatedPilotsForChEstimation;

typedef struct NhuDL_PhysChCapFDD_v380ext
{
    TknPres pres;
    NhuSupportOfDedicatedPilotsForChEstimation dummy;
} NhuDL_PhysChCapFDD_v380ext;

typedef struct NhuInterRATHovrInfo_v390ext_IEs
{
    TknPres pres;
    NhuUE_RadioAccessCap_v380ext ue_RadioAccessCap_v380ext;
    NhuDL_PhysChCapFDD_v380ext dl_PhysChCapFDD_v380ext;
} NhuInterRATHovrInfo_v390ext_IEs;

/* Element ----- validity-CellPCH-UraPCH ------- */
typedef enum enumNhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH
{
    NhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCHtrueEnum
} EnumNhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH;
typedef TknU32 NhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH;

typedef struct NhuUE_PositioningCapExt_v3a0
{
    TknPres pres;
    NhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH validity_CellPCH_UraPCH;
} NhuUE_PositioningCapExt_v3a0;

typedef struct NhuUE_RadioAccessCap_v3a0ext
{
    TknPres pres;
    NhuUE_PositioningCapExt_v3a0 ue_PositioningCapExt_v3a0;
} NhuUE_RadioAccessCap_v3a0ext;

typedef struct NhuInterRATHovrInfo_v3a0ext_IEs
{
    TknPres pres;
    NhuUE_RadioAccessCap_v3a0ext ue_RadioAccessCap_v3a0ext;
} NhuInterRATHovrInfo_v3a0ext_IEs;

/* Element ---------- UESpecificBehaviourInformation1interRAT ----------- */
typedef TknStrBSXL NhuUESpecificBehaviourInfo1interRAT;
typedef struct NhuInterRATHovrInfo_v3d0ext_IEs
{
    TknPres pres;
    NhuUESpecificBehaviourInfo1interRAT uESpecificBehaviourInfo1interRAT;
} NhuInterRATHovrInfo_v3d0ext_IEs;

/* Element ----- sfn-sfnType2Capability ------- */
typedef enum enumNhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap
{
    NhuUE_PositioningCapExt_v3g0sfn_sfnTyp2CaptrueEnum
} EnumNhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap;
typedef TknU32 NhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap;

typedef struct NhuUE_PositioningCapExt_v3g0
{
    TknPres pres;
    NhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap sfn_sfnTyp2Cap;
} NhuUE_PositioningCapExt_v3g0;

typedef struct NhuUE_RadioAccessCap_v3g0ext
{
    TknPres pres;
    NhuUE_PositioningCapExt_v3g0 ue_PositioningCapExt_v3g0;
} NhuUE_RadioAccessCap_v3g0ext;

typedef struct NhuInterRATHovrInfo_v3g0ext_IEs
{
    TknPres pres;
    NhuUE_RadioAccessCap_v3g0ext ue_RadioAccessCap_v3g0ext;
} NhuInterRATHovrInfo_v3g0ext_IEs;

/* Element ----- AccessStratumReleaseIndicator ------- */
typedef enum enumNhuAccessStratumRlsIndicator
{
    NhuAccessStratumRlsIndicatorrel_4Enum,
    NhuAccessStratumRlsIndicatorrel_5Enum,
    NhuAccessStratumRlsIndicatorrel_6Enum,
    NhuAccessStratumRlsIndicatorrel_7Enum,
    NhuAccessStratumRlsIndicatorspare12Enum,
    NhuAccessStratumRlsIndicatorspare11Enum,
    NhuAccessStratumRlsIndicatorspare10Enum,
    NhuAccessStratumRlsIndicatorspare9Enum,
    NhuAccessStratumRlsIndicatorspare8Enum,
    NhuAccessStratumRlsIndicatorspare7Enum,
    NhuAccessStratumRlsIndicatorspare6Enum,
    NhuAccessStratumRlsIndicatorspare5Enum,
    NhuAccessStratumRlsIndicatorspare4Enum,
    NhuAccessStratumRlsIndicatorspare3Enum,
    NhuAccessStratumRlsIndicatorspare2Enum,
    NhuAccessStratumRlsIndicatorspare1Enum
} EnumNhuAccessStratumRlsIndicator;
typedef TknU32 NhuAccessStratumRlsIndicator;

typedef struct NhuInterRATHovrInfo_v4b0ext_IEs
{
    TknPres pres;
    NhuAccessStratumRlsIndicator accessStratumRlsIndicator;
} NhuInterRATHovrInfo_v4b0ext_IEs;

typedef struct NhuInterRATHovrInfo_v4d0ext_IEs
{
    TknPres pres;
    NhuRadioFrequencyBandTDDLst tdd128_RF_Cap;
} NhuInterRATHovrInfo_v4d0ext_IEs;

typedef struct NhuPredefinedConfigValueTagLst
{
    TknU16 noComp;
    NhuPredefinedConfigValueTag *member;
}NhuPredefinedConfigValueTagLst;

/* Element ----- startPosition ------- */
typedef TknU32 NhuPredefinedConfigSetWithDifferentValueTagstartPosition;

typedef struct NhuPredefinedConfigSetWithDifferentValueTag
{
    TknPres pres;
    NhuPredefinedConfigSetWithDifferentValueTagstartPosition startPosition;    /* DEFAULT 0 */ 
    NhuPredefinedConfigValueTagLst valueTagLst;
} NhuPredefinedConfigSetWithDifferentValueTag;

typedef struct NhuPredefinedConfigSetsWithDifferentValueTag
{
    TknU16 noComp;
    NhuPredefinedConfigSetWithDifferentValueTag *member;
}NhuPredefinedConfigSetsWithDifferentValueTag;

typedef struct NhuPredefinedConfigStatusLstVarSz
{
    TknU16 noComp;
    NhuPredefinedConfigStatusInfo *member;
}NhuPredefinedConfigStatusLstVarSz;

typedef struct NhuPredefinedConfigStatusLstComp
{
    TknPres pres;
    NhuPredefinedConfigSetsWithDifferentValueTag setsWithDifferentValueTag;
    NhuPredefinedConfigStatusLstVarSz otherEntries;
} NhuPredefinedConfigStatusLstComp;

/* Element ----- RF-CapabBandFDDComp ------- */
typedef enum enumNhuRF_CapabBandFDDComp
{
    NhuRF_CapabBandFDDCompnotSuppEnum,
    NhuRF_CapabBandFDDCompdefault_TxRx_separationEnum,
    NhuRF_CapabBandFDDCompspare2Enum,
    NhuRF_CapabBandFDDCompspare1Enum
} EnumNhuRF_CapabBandFDDComp;
typedef TknU32 NhuRF_CapabBandFDDComp;

typedef struct NhuRF_CapabBandLstFDDComp
{
    TknU16 noComp;
    NhuRF_CapabBandFDDComp *member;
}NhuRF_CapabBandLstFDDComp;

typedef struct NhuRF_CapCompfdd
{
    TknU8 choice;
    union {
        NhuRF_CapabBandLstFDDComp supported;
    } val;
} NhuRF_CapCompfdd;

typedef struct NhuRF_CapComptdd384_RF_Cap
{
    TknU8 choice;
    union {
        NhuRadioFrequencyBandTDDLst supported;
    } val;
} NhuRF_CapComptdd384_RF_Cap;

typedef struct NhuRF_CapComptdd128_RF_Cap
{
    TknU8 choice;
    union {
        NhuRadioFrequencyBandTDDLst supported;
    } val;
} NhuRF_CapComptdd128_RF_Cap;

typedef struct NhuRF_CapComp
{
    TknPres pres;
    NhuRF_CapCompfdd fdd;
    NhuRF_CapComptdd384_RF_Cap tdd384_RF_Cap;
    NhuRF_CapComptdd128_RF_Cap tdd128_RF_Cap;
} NhuRF_CapComp;

/* Element ----- totalAM-RLCMemoryExceeds10kB ------- */
typedef TknU8 NhuUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB;

typedef struct NhuUE_RadioAccessCapComp
{
    TknPres pres;
    NhuUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB totalAM_RLCMemoryExceeds10kB;
    NhuRF_CapComp rf_CapComp;
} NhuUE_RadioAccessCapComp;

typedef struct NhuInterRATHovrInfo_v590ext_IEs
{
    TknPres pres;
    NhuPredefinedConfigStatusLstComp predefinedConfigStatusLstComp;
    NhuUE_RadioAccessCapComp ue_RadioAccessCapComp;
} NhuInterRATHovrInfo_v590ext_IEs;

typedef struct NhuUE_SecInfo2
{
    TknPres pres;
    NhuSTART_Value start_PS;
} NhuUE_SecInfo2;

typedef struct NhuRF_CapabBandLstFDDComp_ext
{
    TknU16 noComp;
    NhuRF_CapabBandFDDComp *member;
}NhuRF_CapabBandLstFDDComp_ext;

typedef struct NhuUE_RadioAccessCapComp_ext
{
    TknPres pres;
    NhuRF_CapabBandLstFDDComp_ext rf_CapFDDComp;
} NhuUE_RadioAccessCapComp_ext;

/* Element ----- DL-CapabilityWithSimultaneousHS-DSCHConfig ------- */
typedef enum enumNhuDL_CapWithSimultaneousHS_DSCHConfig
{
    NhuDL_CapWithSimultaneousHS_DSCHConfigkbps32Enum,
    NhuDL_CapWithSimultaneousHS_DSCHConfigkbps64Enum,
    NhuDL_CapWithSimultaneousHS_DSCHConfigkbps128Enum,
    NhuDL_CapWithSimultaneousHS_DSCHConfigkbps384Enum
} EnumNhuDL_CapWithSimultaneousHS_DSCHConfig;
typedef TknU32 NhuDL_CapWithSimultaneousHS_DSCHConfig;

/* Element ----- HSDSCH-physical-layer-category ------- */
typedef TknU32 NhuHSDSCH_physical_layer_category;

/* Element ----- edch-PhysicalLayerCategory ------- */
typedef TknU32 NhuPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory;

typedef struct NhuPhysicalChannelCap_edch_r6fdd_edchsupported
{
    TknPres pres;
    NhuPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory edch_PhysicalLyrCategory;
} NhuPhysicalChannelCap_edch_r6fdd_edchsupported;

typedef struct NhuPhysicalChannelCap_edch_r6fdd_edch
{
    TknU8 choice;
    union {
        NhuPhysicalChannelCap_edch_r6fdd_edchsupported supported;
    } val;
} NhuPhysicalChannelCap_edch_r6fdd_edch;

typedef struct NhuPhysicalChannelCap_edch_r6
{
    TknPres pres;
    NhuPhysicalChannelCap_edch_r6fdd_edch fdd_edch;
} NhuPhysicalChannelCap_edch_r6;

typedef struct NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch
{
    TknPres pres;
    NhuDL_CapWithSimultaneousHS_DSCHConfig dl_CapWithSimultaneousHS_DSCHConfig;
    NhuHSDSCH_physical_layer_category physicalChannelCapabComp_hspdsch_r6;
    NhuPhysicalChannelCap_edch_r6 physicalChannelCap_edch_r6;
} NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch;

typedef struct NhuUE_RadioAccessCapComp2
{
    TknPres pres;
    NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch fddPhysicalChannelCapab_hspdsch_edch;
} NhuUE_RadioAccessCapComp2;

typedef struct NhuInterRATHovrInfo_v690ext_IEs
{
    TknPres pres;
    NhuUE_SecInfo2 ue_SecInfo2;
    NhuUE_RadioAccessCapComp_ext ue_RadioAccessCapComp;
    NhuUE_RadioAccessCapComp2 ue_RadioAccessCapComp2;
} NhuInterRATHovrInfo_v690ext_IEs;

/* Element ----- supportForSIB11bis ------- */
typedef enum enumNhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis
{
    NhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bistrueEnum
} EnumNhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis;
typedef TknU32 NhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis;

typedef struct NhuInterRATHovrInfo_v6b0ext_IEs
{
    TknPres pres;
    NhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis supportForSIB11bis;
} NhuInterRATHovrInfo_v6b0ext_IEs;

typedef struct NhuUTRANInterRATHovrInfopredefinedConfigStatusLst
{
    TknU8 choice;
    union {
        NhuPredefinedConfigStatusLst present;
    } val;
} NhuUTRANInterRATHovrInfopredefinedConfigStatusLst;

typedef struct NhuUTRANInterRATHovrInfouE_SecInfo
{
    TknU8 choice;
    union {
        NhuUE_SecInfo present;
    } val;
} NhuUTRANInterRATHovrInfouE_SecInfo;

/* Element ---------- present ----------- */
typedef TknStrOSXL NhuUTRANInterRATHovrInfoue_CapContpresent;
typedef struct NhuUTRANInterRATHovrInfoue_CapCont
{
    TknU8 choice;
    union {
        NhuUTRANInterRATHovrInfoue_CapContpresent present;
    } val;
} NhuUTRANInterRATHovrInfoue_CapCont;

/* Element ---------- interRATHandoverInfo-r3-add-ext ----------- */
typedef TknStrBSXL NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext;
typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns
{
    TknPres pres;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns
{
    TknPres pres;
    NhuInterRATHovrInfo_v6b0ext_IEs interRATHovrInfo_v6b0ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns nonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns
{
    TknPres pres;
    NhuInterRATHovrInfo_v690ext_IEs interRATHovrInfo_v690ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns v6b0NonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns
{
    TknPres pres;
    NhuInterRATHovrInfo_v590ext_IEs interRATHovrInfo_v590ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns v690NonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns
{
    TknPres pres;
    NhuInterRATHovrInfo_v4d0ext_IEs interRATHovrInfo_v4d0ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns v590NonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns
{
    TknPres pres;
    NhuInterRATHovrInfo_v4b0ext_IEs interRATHovrInfo_v4b0ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns v4d0NonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns
{
    TknPres pres;
    NhuInterRATHovrInfo_v3g0ext_IEs interRATHovrInfo_v3g0ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns v4b0NonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns
{
    TknPres pres;
    NhuInterRATHovrInfo_v3d0ext_IEs interRATHovrInfo_v3d0ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext interRATHovrInfo_r3_add_ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns v3g0NonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns
{
    TknPres pres;
    NhuInterRATHovrInfo_v3a0ext_IEs interRATHovrInfo_v3a0ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns laterNonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent
{
    TknPres pres;
    NhuInterRATHovrInfo_v390ext_IEs interRATHovrInfo_v390ext;
    NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns v3a0NonCriticalExtns;
} NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent;

typedef struct NhuUTRANInterRATHovrInfov390NonCriticalExtns
{
    TknU8 choice;
    union {
        NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent present;
    } val;
} NhuUTRANInterRATHovrInfov390NonCriticalExtns;

typedef struct NhuUTRANInterRATHovrInfo
{
    TknPres pres;
    NhuUTRANInterRATHovrInfopredefinedConfigStatusLst predefinedConfigStatusLst;
    NhuUTRANInterRATHovrInfouE_SecInfo uE_SecInfo;
    NhuUTRANInterRATHovrInfoue_CapCont ue_CapCont;
    NhuUTRANInterRATHovrInfov390NonCriticalExtns v390NonCriticalExtns;
} NhuUTRANInterRATHovrInfo;

/* Element ----- RadioFrequencyBandFDD2 ------- */
typedef enum enumNhuRadioFrequencyBandFDD2
{
    NhuRadioFrequencyBandFDD2bandVIIIEnum,
    NhuRadioFrequencyBandFDD2bandIXEnum,
    NhuRadioFrequencyBandFDD2bandXEnum,
    NhuRadioFrequencyBandFDD2bandXIEnum,
    NhuRadioFrequencyBandFDD2bandXIIEnum,
    NhuRadioFrequencyBandFDD2bandXIIIEnum,
    NhuRadioFrequencyBandFDD2bandXIVEnum,
    NhuRadioFrequencyBandFDD2bandXVEnum,
    NhuRadioFrequencyBandFDD2bandXVIEnum,
    NhuRadioFrequencyBandFDD2bandXVIIEnum,
    NhuRadioFrequencyBandFDD2bandXVIIIEnum,
    NhuRadioFrequencyBandFDD2bandXIXEnum,
    NhuRadioFrequencyBandFDD2bandXXEnum,
    NhuRadioFrequencyBandFDD2bandXXIEnum,
    NhuRadioFrequencyBandFDD2bandXXIIEnum,
    NhuRadioFrequencyBandFDD2extension_indicatorEnum
} EnumNhuRadioFrequencyBandFDD2;
typedef TknU32 NhuRadioFrequencyBandFDD2;

/* Element ----- dl-MeasurementsFDD ------- */
typedef TknU8 NhuCompressedModeMeasCapabFDD2dl_MeasurementsFDD;

/* Element ----- ul-MeasurementsFDD ------- */
typedef TknU8 NhuCompressedModeMeasCapabFDD2ul_MeasurementsFDD;

typedef struct NhuCompressedModeMeasCapabFDD2
{
    TknPres pres;
    NhuRadioFrequencyBandFDD radioFrequencyBandFDD;
    NhuRadioFrequencyBandFDD2 radioFrequencyBandFDD2;
    NhuCompressedModeMeasCapabFDD2dl_MeasurementsFDD dl_MeasurementsFDD;
    NhuCompressedModeMeasCapabFDD2ul_MeasurementsFDD ul_MeasurementsFDD;
} NhuCompressedModeMeasCapabFDD2;

typedef struct NhuCompressedModeMeasCapabFDDLst2
{
    TknU16 noComp;
    NhuCompressedModeMeasCapabFDD2 *member;
}NhuCompressedModeMeasCapabFDDLst2;

typedef struct NhuMeasurementCapExt2
{
    TknPres pres;
    NhuCompressedModeMeasCapabFDDLst2 compressedModeMeasCapabFDDLst;
    NhuCompressedModeMeasCapabTDDLst compressedModeMeasCapabTDDLst;
    NhuCompressedModeMeasCapabGSMLst compressedModeMeasCapabGSMLst;
    NhuCompressedModeMeasCapabMC compressedModeMeasCapabMC;
} NhuMeasurementCapExt2;

typedef struct NhuUE_RadioAccessCapabBandFDD2fddRF_Cap
{
    TknPres pres;
    NhuUE_PowerClassExt ue_PowerClass;
    NhuTxRxFrequencySeparation txRxFrequencySeparation;
} NhuUE_RadioAccessCapabBandFDD2fddRF_Cap;

typedef struct NhuUE_RadioAccessCapabBandFDD2
{
    TknPres pres;
    NhuRadioFrequencyBandFDD2 radioFrequencyBandFDD2;
    NhuUE_RadioAccessCapabBandFDD2fddRF_Cap fddRF_Cap;
    NhuMeasurementCapExt2 measurementCap2;
} NhuUE_RadioAccessCapabBandFDD2;

typedef struct NhuUE_RadioAccessCapabBandFDDLst2
{
    TknU16 noComp;
    NhuUE_RadioAccessCapabBandFDD2 *member;
}NhuUE_RadioAccessCapabBandFDDLst2;

/* Element ----- dl-MeasurementsFDD ------- */
typedef TknU8 NhuCompressedModeMeasCapabFDD_extdl_MeasurementsFDD;

/* Element ----- ul-MeasurementsFDD ------- */
typedef TknU8 NhuCompressedModeMeasCapabFDD_extul_MeasurementsFDD;

typedef struct NhuCompressedModeMeasCapabFDD_ext
{
    TknPres pres;
    NhuRadioFrequencyBandFDD2 radioFrequencyBandFDD2;
    NhuCompressedModeMeasCapabFDD_extdl_MeasurementsFDD dl_MeasurementsFDD;
    NhuCompressedModeMeasCapabFDD_extul_MeasurementsFDD ul_MeasurementsFDD;
} NhuCompressedModeMeasCapabFDD_ext;

typedef struct NhuCompressedModeMeasCapabFDDLst_ext
{
    TknU16 noComp;
    NhuCompressedModeMeasCapabFDD_ext *member;
}NhuCompressedModeMeasCapabFDDLst_ext;

typedef struct NhuUE_RadioAccessCapabBandFDD_ext
{
    TknPres pres;
    NhuRadioFrequencyBandFDD radioFrequencyBandFDD;
    NhuCompressedModeMeasCapabFDDLst_ext compressedModeMeasCapabFDDLst_ext;
} NhuUE_RadioAccessCapabBandFDD_ext;

typedef struct NhuUE_RadioAccessCapabBandFDDLst_ext
{
    TknU16 noComp;
    NhuUE_RadioAccessCapabBandFDD_ext *member;
}NhuUE_RadioAccessCapabBandFDDLst_ext;

typedef struct NhuUE_RadioAccessCap_v650ext
{
    TknPres pres;
    NhuUE_RadioAccessCapabBandFDDLst2 ue_RadioAccessCapabBandFDDLst2;
    NhuUE_RadioAccessCapabBandFDDLst_ext ue_RadioAccessCapabBandFDDLst_ext;
} NhuUE_RadioAccessCap_v650ext;

typedef struct NhuInterRATHovrInfo_v690ext1_IEs
{
    TknPres pres;
    NhuUE_RadioAccessCap_v650ext ue_RadioAccessCap_v650ext;
} NhuInterRATHovrInfo_v690ext1_IEs;

typedef struct NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns
{
    TknPres pres;
} NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns;

typedef struct NhuInterRATHovrInfo_r3_add_ext_IEs
{
    TknPres pres;
    NhuInterRATHovrInfo_v690ext1_IEs interRATHovrInfo_v690ext1;
    NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns nonCriticalExtns;
} NhuInterRATHovrInfo_r3_add_ext_IEs;

/* Element ----- RadioFrequencyBandTDDList-r7 ------- */
typedef enum enumNhuRadioFrequencyBandTDDLst_r7
{
    NhuRadioFrequencyBandTDDLst_r7aEnum,
    NhuRadioFrequencyBandTDDLst_r7bEnum,
    NhuRadioFrequencyBandTDDLst_r7cEnum,
    NhuRadioFrequencyBandTDDLst_r7dEnum,
    NhuRadioFrequencyBandTDDLst_r7abEnum,
    NhuRadioFrequencyBandTDDLst_r7acEnum,
    NhuRadioFrequencyBandTDDLst_r7adEnum,
    NhuRadioFrequencyBandTDDLst_r7bcEnum,
    NhuRadioFrequencyBandTDDLst_r7bdEnum,
    NhuRadioFrequencyBandTDDLst_r7cdEnum,
    NhuRadioFrequencyBandTDDLst_r7abcEnum,
    NhuRadioFrequencyBandTDDLst_r7abdEnum,
    NhuRadioFrequencyBandTDDLst_r7acdEnum,
    NhuRadioFrequencyBandTDDLst_r7bcdEnum,
    NhuRadioFrequencyBandTDDLst_r7abcdEnum,
    NhuRadioFrequencyBandTDDLst_r7spareEnum
} EnumNhuRadioFrequencyBandTDDLst_r7;
typedef TknU32 NhuRadioFrequencyBandTDDLst_r7;

/* Element ----- ChipRateCapability-r7 ------- */
typedef enum enumNhuChipRateCap_r7
{
    NhuChipRateCap_r7mcps3_84Enum,
    NhuChipRateCap_r7mcps7_68Enum,
    NhuChipRateCap_r7mcps1_28Enum
} EnumNhuChipRateCap_r7;
typedef TknU32 NhuChipRateCap_r7;

typedef struct NhuRF_Cap_r7fddRF_Cap
{
    TknPres pres;
    NhuUE_PowerClass ue_PowerClass;
    NhuTxRxFrequencySeparation txRxFrequencySeparation;
} NhuRF_Cap_r7fddRF_Cap;

typedef struct NhuRF_Cap_r7tddRF_Cap
{
    TknPres pres;
    NhuUE_PowerClass ue_PowerClass;
    NhuRadioFrequencyBandTDDLst_r7 radioFrequencyTDDBandLst;
    NhuChipRateCap_r7 chipRateCap;
} NhuRF_Cap_r7tddRF_Cap;

typedef struct NhuRF_Cap_r7
{
    TknPres pres;
    NhuRF_Cap_r7fddRF_Cap fddRF_Cap;
    NhuRF_Cap_r7tddRF_Cap tddRF_Cap;
} NhuRF_Cap_r7;

/* Element ----- MaxPhysChPerFrame-768 ------- */
typedef TknU32 NhuMaxPhysChPerFrame_768;

/* Element ----- MinimumSF-DL-768 ------- */
typedef enum enumNhuMinimumSF_DL_768
{
    NhuMinimumSF_DL_768sf1Enum,
    NhuMinimumSF_DL_768sf32Enum
} EnumNhuMinimumSF_DL_768;
typedef TknU32 NhuMinimumSF_DL_768;

/* Element ----- MaxPhysChPerTS-768 ------- */
typedef TknU32 NhuMaxPhysChPerTS_768;

/* Element ----- supportOfPDSCH ------- */
typedef TknU8 NhuDL_PhysChCapTDD_768supportOfPDSCH;

typedef struct NhuDL_PhysChCapTDD_768
{
    TknPres pres;
    NhuMaxTS_PerFrame maxTS_PerFrame;
    NhuMaxPhysChPerFrame_768 maxPhysChPerFrame;
    NhuMinimumSF_DL_768 minimumSF;
    NhuDL_PhysChCapTDD_768supportOfPDSCH supportOfPDSCH;
    NhuMaxPhysChPerTS_768 maxPhysChPerTS;
} NhuDL_PhysChCapTDD_768;

typedef struct NhuPhysicalChannelCap_r7fddPhysChCap
{
    TknPres pres;
    NhuDL_PhysChCapFDD downlinkPhysChCap;
    NhuUL_PhysChCapFDD uplinkPhysChCap;
} NhuPhysicalChannelCap_r7fddPhysChCap;

typedef struct NhuPhysicalChannelCap_r7tddPhysChCap
{
    TknPres pres;
    NhuDL_PhysChCapTDD downlinkPhysChCap;
    NhuUL_PhysChCapTDD uplinkPhysChCap;
} NhuPhysicalChannelCap_r7tddPhysChCap;

typedef struct NhuPhysicalChannelCap_r7tddPhysChCap_768
{
    TknPres pres;
    NhuDL_PhysChCapTDD_768 downlinkPhysChCap;
    NhuUL_PhysChCapTDD uplinkPhysChCap;
} NhuPhysicalChannelCap_r7tddPhysChCap_768;

typedef struct NhuPhysicalChannelCap_r7
{
    TknPres pres;
    NhuPhysicalChannelCap_r7fddPhysChCap fddPhysChCap;
    NhuPhysicalChannelCap_r7tddPhysChCap tddPhysChCap;
    NhuPhysicalChannelCap_r7tddPhysChCap_768 tddPhysChCap_768;
} NhuPhysicalChannelCap_r7;

/* Element ----- tdd-edch-PhysicalLayerCategory ------- */
typedef TknU32 NhuPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory;

typedef struct NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported
{
    TknPres pres;
    NhuPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory tdd_edch_PhysicalLyrCategory;
} NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported;

typedef struct NhuPhysicalChannelCap_tddedch_r7tdd_edch
{
    TknU8 choice;
    union {
        NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported supported;
    } val;
} NhuPhysicalChannelCap_tddedch_r7tdd_edch;

typedef struct NhuPhysicalChannelCap_tddedch_r7
{
    TknPres pres;
    NhuPhysicalChannelCap_tddedch_r7tdd_edch tdd_edch;
} NhuPhysicalChannelCap_tddedch_r7;

typedef struct NhuUE_RadioAccessCap_v7xyext
{
    TknPres pres;
    NhuRF_Cap_r7 rf_Cap;
    NhuPhysicalChannelCap_r7 physicalChannelCap_r7;
    NhuPhysicalChannelCap_tddedch_r7 physicalchannelcapability_tddedch;
} NhuUE_RadioAccessCap_v7xyext;


#endif

/**********************************************************************
         End of file:     nhu_3gasn.x@@/main/1 - Fri Dec 11 18:45:13 2009
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---    ch        1. RRC LTE Initial Release.
*********************************************************************91*/
