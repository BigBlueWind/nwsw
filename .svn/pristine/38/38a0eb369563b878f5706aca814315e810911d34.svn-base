//-------------------------------------------------------------------------------------------
/** @file LTE_test_config.h
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.35 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------



#ifndef _LTE_TEST_CONFIG_H
#define _LTE_TEST_CONFIG_H

// Define Test structure to reduce the number of parameters to pass to the
// configuration functions
typedef struct ResElmDesc
{
    U8 startRes;
    U8 numRes;
} RESELMINF, *PRESELMINF;


typedef struct MappingDesc
{
    U16 numberofEntries;
    RESELMINF reselmInfo[50];
    U16 pad;
} MAPPING, *PMAPPING;

typedef struct ChannelConfigDescriptor
{
    U8 ChanType;
    U8 N2_RB;
    U8 N1_CS;
    U8 delta_shift_PUCCH;
    U16 n1_pucch;
    U16 n2_pucch;
    U8 format_type;
    S32 chPower;
    U8 Qm;
    S16 PhichGrp;
    S16 PhichSequence;
    U8 PhichBit;
    U32 numBytes;
    U32 mapMode;
	U16 nRNTI;
	U16 numbitsDCI;
	S16 numCCE;
    MAPPING map;

} CHCFG, *PCHCFG;


typedef struct SrsChannelConfigDescriptor
{
   S16 srs_enable;
   S16 srs_cdmEnable;   //1=Code Division Multiplexing enabled. i.e. Multiple SRS are sharing this frequency allocation with different cyclic shifts
   S16 srs_msIndex;     //index to MobileStation (/ MIMO input antenna), that this SRS channel is sent from
   S16 srs_cdmGroupIndex;   //SRS channels in the same CDM group need to be configured in consecutive SRS Channel numbers
    // - within a CDM group, each SRS must have a different cyclic shift
   S16 srs_cyclicShift;     //nCs_srs, [3GPP TS 36.211, Sec.5.5.3.1]
    // - within a CDM group, all the following parameters must be the same
   S16 srs_bandwidth;  //=3 for CAZAC24, =2 for ZadoffChu48 %TTTTTTTTTTT     %B_srs, Tables 5.5.3.2-x [3GPP TS 36.211, Sec.5.5.3]
   S16 srs_hoppingBandwidth;    //b_hop, [3GPP TS 36.211, Sec.5.5.3.2]
   S16 srs_freqDomainPosition; //n_rrc, [3GPP TS 36.211, Sec.5.5.3.2].
   S16 srs_duration;      //Duration, [3GPP TS 36.213, Sec.8.2].
   S16 srs_configIndex;     //I_srs, [3GPP TS 36.213, table8.2-1].
   S16 srs_transmissionComb;  //k_tc, [3GPP TS 36.211, Sec.5.5.3.2]
}SRSCHCFG,*PSRSCHCFG;

typedef struct TestParmsDescriptor
{
    U8 numDataCh;
    U8 numCtlCh;
    U8 numCtlSymbols;
    BIT_FIELD qm :3;
    BIT_FIELD mcs :5;
    BIT_FIELD rvidx :2;
    BIT_FIELD nTxCodewords :2;
    BIT_FIELD nTxLayers :3;
    BIT_FIELD cddType :1;
    U8 codeBookIndex;
    U16 cellID;
    U32 transmissionMode;
    U32 ul_dl_test_indicator;// 0 DL, 1 UL
    U32 test_case;

    U32 number_of_channels;
    S16 Ng;
	U32 number_of_phich_channels;
    S16 pilotPower;
    S16 psyncPower;
    S16 ssyncPower;
    S16 ciPower;
    U16 paprControl;
    U16 paprThreshold;

    S16 srsEnable;
	S16 srsBandwidthConfig;
	S16 srsSubframeConfig;
	S16 srsSimultaneousANandSRS;
    S16 numSrs;

	S16 prachEnable;
	S16 prachConfigurationIndex;
	S16 prachSubcSpacing;
	S16 prachFrequencyOffset;
    S16 prachHighSpeedFlag;
	S16 prachCyclicShiftConfig;
	S16 prachRootSequenceIndex;
    U16 numHalfIterations;
    U32 mapEnable;
	U16 numbitsDCI;
	U8 numCCE;
	U8 LayerCellIdGroup;
    U8 LayerId;

    PCHCFG  pchan[100];
	PSRSCHCFG  pSrschan[100];

} TSTPDESC, *PTSTPDESC;

typedef struct TestConfigDescriptor
{
    U16 UL_nDl; // 1 UL 0 DL
    U16 number_of_channels; //
    CHCFG *pchan[100];
} TSTCFG, *PTSTCFG;

#ifdef MSRX
extern U32 LteMsPhyMain(U32 phyInstance, S16 *sigOut, S16 **sigIn,
                        U32* inputMessage, U32 inputMessageSize,
                        U32* outputMessage, U32* outputMessageSize);
#endif

void LtePhyInitDefault(PINITPARM pParm);
void Configure20MhzBw(PINITPARM pParm);
void Configure15MhzBw(PINITPARM pParm);
void Configure10MhzBw(PINITPARM pParm);
void Configure5MhzBw(PINITPARM pParm);
void Configure3MhzBw(PINITPARM pParm);
void Configure1_4MhzBw(PINITPARM pParm);
void InitializeSubchannel(PDLSUBCHINFO pSch, PTSTPDESC pTst, U32 id, U32 chType);
void InitializeULSubchannel(PULSUBCHINFO pSch, PTSTPDESC pTst, U32 id, U32 chType);
void InitializeDLChannels(PDLCHANDESC pCh, PTSTPDESC pTst, U32 id, U32 sFrType);
void InitializeDCIChannels(PDCICHANNELDESC pCh, PTSTPDESC pTst, U32 id, U32 sFrType);
void InitializeULChannels(PULCHANDESC pCh, PTSTPDESC pTst, U32 id, U32 sFrType);
void InitializeUCIChannels(PULCTRLCHDESC pCh, PTSTPDESC pTst, U32 id, U32 sFrType);
void InitializeCommonULCtlParameters(PULSUBFRDESC pSfr, PTSTPDESC pTst);

void ConfigureULOffsets(PULSUBFRDESC pSfr);

void ConfigureTest1_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest2_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest4_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest5_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest6_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                    U32 sFrType);
void ConfigureTest7_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                    U32 sFrType);
void ConfigureTest8_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                    U32 sFrType);
void ConfigureTest9_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest10_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest11_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest12_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest16_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest30_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest31_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest32_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest33_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);
void ConfigureTest34_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst,
                       U32 sFrType);

void ConfigureTestBS_DL(PINITPARM pParm, PDLSUBFRDESC pSfr, PTSTPDESC pTst,
                        U32 sFrType);
void ConfigureTestBsDlDefault(PINITPARM pParm, PINITPARM pParm1,
							  PDLSUBFRDESC txVector, PDLSUBFRDESC rxMsVector,
							  PTSTPDESC tstpbs, PTSTPDESC tstpms, U32 tstOption);

int CheckResults(U32 tstOption, U32 tstType);
#ifdef FD_TEST
void ReadConfigFile(FILE *config_file, TSTPDESC *tstpbs, TSTPDESC *tstpms, U32 rbAssignMode, PCHCFG *pchan, PCHCFG chancfg);
#else
void ReadConfigFile(FILE *config_file, TSTPDESC *tstpbs, TSTPDESC *tstpms, U32 rbAssignMode);
#endif
void InitializeFilesPerTestOption(U32 tstOption, U32 tstType, FILE **inFile,
                                  FILE **inDFile);
void InitializeFilesPerTestOptionCTL(U32 tstOption, U32 tstType, FILE **inDFile);



#endif /* _LTE_TEST_CONFIG_H */
