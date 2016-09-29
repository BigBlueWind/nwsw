//-------------------------------------------------------------------------------------------
/** @file ltephy.h
 *
 * @brief This file defines the structures, constants and variables
 *        used in the implementation of an LTE PHY
 * @author Mindspeed Technologies
 * @version $Revision: 1.49 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------
#ifndef LTEPHY_H

#define LTEPHY_H
#include "LtePhyL2Api.h"
#include "ltephydef.h"
#include "tcbscheduler.h"

#define MOD_QPSK  1
#define MOD_16QAM 2
#define MOD_64QAM 3

#define MAX_PROFILE_INFO 1000
#define PBCHDATASIZE 480

//*****************************************************************************
//**********                        UPLINK HARQ                      **********
//*****************************************************************************

typedef struct  harq_structure
{
    // the size 93344 is used here to keep buffer
    // aligned and PREVENT MDMA burst problems!!!

    // Please feel free to change it with macro

    //*************************************************************************
    // Soft decision storage buffers.
    // There are separate buffers for each group within each process.
    // The maximum individual buffer length is equal to the maximum possible
    // soft decisions that can be generated each subframe.
    S8      harq_sft_buf[P_HARQ_PROC][P_HARQ_COMB][93344];


    //*************************************************************************
    // Current HARQ process for subframe (0-7).
    // The process increments every subframe.

    S16     harq_prc;

    //*************************************************************************
    // Current group to use for storing soft decisions.
    // The number of possible groups is equal to the maximum number of allowed
    // transmissions per process.

    S16     harq_grp[P_HARQ_PROC];

    //*************************************************************************
    // Current user index.
    S16     harq_user;

    //*************************************************************************
    // Pointers to buffers  being used to store soft decisions for the user.

    S8      *harq_sft_pos[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB];

    //*************************************************************************
    // Current HARQ iteration for user this subframe.

    S16     harq_iter[P_HARQ_PROC][P_HARQ_USERS];

    //*************************************************************************
    // Rate matching rvidx (0-3) for user for each HARQ iteration.

    S16     harq_rvidx[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB];

    //*************************************************************************

} HARQS, *PHARQS;

//*****************************************************************************

typedef struct PUCCH_Info
{
    S16 pucchFormat;
    S16 betaPUCCH;
    S16 delta_shift_PUCCH;
    S16 n1;
    S16 n2;
    S16 N1_cs;
    S16 N2_rb;
    S16 u[20];

} BS_PUCCH, *PBS_PUCCH;

typedef struct PUSCH_Info
{
    U8  cvec[7*20];         //array of zeroes
    U8  cvec_u[7*20];
	U8  PRSbits[7*20];

} BS_PUSCH, *PBS_PUSCH;

typedef struct SRS_Info {

   S16 srsBandwidthTab;
   S16 srsPilotSymbol;
   S16 srsEnable;
   S16 srsBandwidthConfig;  //C_srs, Tables 5.5.3.2-x [3GPP TS 36.211, Sec.5.5.3]
   S16 srsSubframeConfig;   //Table 5.5.3.3-1 [3GPP TS 36.211, Sec.5.5.3]
   S16 srsSimultaneousANandSRS; //[3GPP TS 36.213, Sec.8.2]
   S16 srsSubframeEnable[10];
   S16 u[20];
} BS_SRS, *PBS_SRS;

typedef struct RxChan_SRS_Info {
   U16 enable;
   U16 cdmEnable;      // 1=Code Division Multiplexing enabled.
                       // i.e. Multiple SRS are sharing this frequency allocation
                       // with different cyclic shifts

   U16 msIndex;        // index to MobileStation (/ MIMO input antenna),
                       // that this SRS channel is sent from

   U16 cdmGroupIndex;   // SRS channels in the same CDM group need to be configured
                        // in consecutive SRS Channel numbers - within a CDM group,
                        // each SRS must have a different cyclic shift

   U16 cyclicShift;     // nCs_srs, [3GPP TS 36.211, Sec.5.5.3.1]
                        // within a CDM group all the following parameters must be the same

   U16 bandwidth;       // =3 for CAZAC24, =2 for ZadoffChu48,
                        // Tables 5.5.3.2-x [3GPP TS 36.211, Sec.5.5.3]

   U16 hoppingBandwidth;     //b_hop, [3GPP TS 36.211, Sec.5.5.3.2]
   U16 freqDomainPosition;   //n_rrc, [3GPP TS 36.211, Sec.5.5.3.2].
   U16 duration;             //Duration, [3GPP TS 36.213, Sec.8.2].
   U16 configIndex;          //I_srs, [3GPP TS 36.213, table8.2-1].
   U16 transmissionComb;     //k_tc, [3GPP TS 36.211, Sec.5.5.3.2]

} BS_RX_CHAN_SRS, *PBS_RX_CHAN_SRS;

typedef struct BS_RX_PRACH_Info {
   S16 bs_prach_enable;
   S16 bs_prach_configurationIndex;
   S16 bs_prach_frequencyOffset;
   S16 bs_prach_rootSequenceIndex;
   S16 bs_prach_highSpeedFlag;
   S16 bs_prach_cyclicShiftConfig;
   S16 bs_prach_Nseq;
   S16 bs_prach_Ncp;
   S16 bs_prach_preambleFormat;
   S16 bs_prach_preambleSeqLength;
   S16 bs_prach_subcSpacing;
   S16 bs_prach_subcOffset;
   S16 bs_prach_subcSpacingRatio;
   S16 bs_prach_nRa_prb;
   S16 bs_prach_k0;
   S16 bs_prach_preambleSubframeEnable[10];
   S16 bs_prach_TseqMicrosec;
} BS_RX_PRACH, *PBS_RX_PRACH;

typedef struct MS_DET_Info{
	S16 num;
	S16 cyclicShift[64];
	S16 tDelMicrosecEst[64];
} MS_DET, *PMS_DET;

typedef struct BS_RX_PRACHDet_Info{
   S16 bs_prachDet_downsampleRate;
   S16 bs_prachDet_lpfDelay;
   S16 bs_prachDet_firLpf_length;
   S16 bs_prachDet_numRootSeq;
   S16 bs_prachDet_numCyclicShiftsPerRootSeq;
   S16 bs_prachDet_cyclicShift;
   S16 bs_prachDet_enable;
   S16 bs_prachDet_uOffset;
   S16 bs_prachDet_Ncs;
   S16 bs_prachDet_u[64];
   S16 bs_prachDet_tsamOffset[64];
   S16 bs_prachDet_rootSeqDet[64];
   S16 bs_prachDet_tsamOffsetEst[64];
   MS_DET bs_prachDet_msDet;
} BS_RX_PRACHDet, *PBS_RX_PRACHDet;

typedef struct BsRxChan_Info
{
    S16 rvidx;
    S16 Qm;
    S16 nRNTI;
    S16 n2_DMRS;
    S16 delta_ss;
    S16 transChanType;
    S16 physChanType;
    S16 pilotPower;
    S16 pilotScaleFactor;
    S16 power;
    S32 numBitsInTransportBlock;
    S32 numBitsInCodeword;
    S16 nPRS[20];
    S16 *RBAssign;
    S16 **RBAssignment;
    U8 *mac_sdu;

    U32 G;
    S16 rowRBAssignment;
    S16 colRBAssignment;
    S16 Mpusch_rb;
    S16 Mpusch_sc;
    S8 *by;
    U32 HalfIterations;

    PBS_PUCCH pBsPucch;
    PBS_PUSCH pBsPusch;
    S16 scaleFactor;


} BS_RX_CHAN, *PBS_RX_CHAN;


typedef struct BS_Rx_Info
{
    S16 groupHoppingEnabled;
    S16 Nrx_antennas;
    S16 numMS;
    S16 Nul_rb;
    S16 Nul_symb;
    S16 Ndata_rb;
	S16 frameNumber;
    S16 subframeNumber;
    S16 slotNumber;
    S16 n1_DMRS;
    S16 numChan;
    S16 numDataChan;
    S16 numControlChan;
    S16 numSrsChan;
    S16 numPUCCHSymbols;
    U16 NumTasksTotalRX;
    S16 **adcbuf;

    U32 rxChcuSf[LTEMXSFCH];
    U32 rxChIdx;
    U32 isExeTaskListReady;
    U32 isExePrachTaskListReady;
    U32 *rs_grid;
    S16 *derotation;
    S16 *expo_mat;
    S16 *aDet;
    S16 *mhcs;
    S16 *adcMhcs;
    S16 *bs_aPilots;
    S16 *ul_pilot_offset;
    S16 *aP;
    S8 *bUserDetSoft;

    S16 *srsChEst;

    /* time stamps */
    U32 rxStartIndTimeStamp;
    U32 rxVectorTimeStampDone;
    U32 rxSduTimeStampDone[MAXCHSUBFRAME];
    U32 updatedTimeStamp;
    U32 switchedTimeStamp;
    U32 processedTimeStamp;

    PTCB pTaskRx1[MAX_NUM_TCB];
    PTCB pTaskRx2[MAX_NUM_TCB];
    PTASKLIST pExeTaskList[4];

    PTASKLIST pExePrachTaskList[1];
    U32 *pBsPrachCram;

    PULPILOTSPARAM ULPilotInput[MAXCHSUBFRAME];
    PBS_RX_CHAN pBsRxChan[LTEMXSFCH];
    PBS_RX_CHAN_SRS pBsRxChanSrs[MAX_UL_SRS];

} BS_RX, *PBS_RX;

typedef struct BsTxChan_Info
{
    S16 transChanType;
    S16 physChanType;
    S16 nRNTI;
    S16 rvidx;
    S16 Ntx_codewords;
    S16 Ntx_layers;
    S16 mimoTxMode;
    S16 CDDType;
    S16 CodebookIndex;
    S16 Qm;
    S16 modulationType;
    S16 power;
    S16 scaleFactor;
    S16 G0;
    S16 G1;
    S16 **l_k_Nsc; // number of subcarriers(RE) ocuppied per symbol per channel
    S16 **l_k_Isc; // location of subcarriers(RE) ocuppied per symbol per channel
    S32 **pBit; // Begining location of bits per symbol per channel
    S16 *Q; // number of codewords indicator per channel
    U8 *mac_sdu;
    S16 *RBAssignment;
    U8 *transportBlock0;
    U8 *transportBlock1;
// Pointer to array (k,l) coordinates for frequency index and time_index for
// consecutive complex modulation symbols
//    S16** k_l;
} BS_TX_CHAN, *PBS_TX_CHAN;

typedef struct TxChannelInfo
{
    //----- DL Resource Element Assignments
    S16 numberofEntries;
    S16 channelType;
	//----- tx mapper
    S16 Qm;
    S16 modulation_type;
    S16 Q;
    S16 l_k_Nsc;
    U32 length_b;
    U32 length_pb;
    U32 Nsubc;
    U16 Nskip;
    U32 pbIndex;  // index to the PackedBitBuf
    U32 M0symb;
    U32 M1symb;
	//----- tx layer mapper
    S16 transmode;
    S16 numTxLayers;
    U16 nrow_x;
    U16 ncol_x;
	//----- tx precoder
    S16 codeBookIdx;
    S16 cddType;
    U16 ncol_y;
} TXCHANNELINFO, *PTXCHANNELINFO;


typedef struct TxSystemInfoInner
{
    U32 numChan;
    U32 numCtlCh;
    S16 bs_Ntx_antennas;
    S16 txSubFrameNumber;
    S16 numCtrlSymbols;
    S16 nDlRb;
    S16 nRbSc;
    S16 nDlSymb;
	S16 *aSymbol[TXANT_FOUR];
} TXSYSTEMINFOINNER, *PTXSYSTEMINFOINNER;

typedef struct TxSystemInfo
{
	TXSYSTEMINFOINNER a;
    TXCHANNELINFO TxChanInfo[200];
} TXSYSTEMINFO, *PTXSYSTEMINFO;


typedef struct BS_Tx_Info
{
    S16 numPDCCHSymbols;
    S16 Ntx_antennas;
    S16 numDataChan;
    S16 numControlChan;
    S16 pilotPower;
    S16 pilotScaleFactor;
    S16 syncPower;
    S16 syncScaleFactor;
// nDLRb is the number of resource elements per slot as a function of BW in the
// downlink direction(Ndl_rb)
    S16 Ndl_rb;
// nDlSymb is the number of Downlink Symbols per slot it is derived from
// cpType and the nRbSc as follows cpType=0 then nDlSymb=7
// cpType=1 and nRbSc=12 then nDlSymb=6 else nDlSymb=3 (Ndl_symb)
    S16 Ndl_symb;
    S16 pilotCarrierRepeatFreq;
    S16 pilotCarrierRepeatTime;
    S16 pilotCarrierSpacing;
    S16 pilotCarrierIndices;
    S16 REinSubfrmSym_Ref;
    S16 NumREinSubfrmSym_Ref;
    S16 Ndata_rb;
    S16 REinSubfrmSym_PDSCH;
    S16 NumREinSubfrmSym_PDSCH;
    S16 RBIndexMid6Lo;
    S16 RBIndexMid6Hi;
    S16 REinSubfrmSym_PBCH;
    S16 NumREinSubfrmSym_PBCH;
    S16 REinSubfrmSym_PDCCH;
    S16 NumREinSubfrmSym_PDCCH;
    U16 TxTaskOffset;
    S16 sync2_d[2][62];
    S16 subframeNumber;
    S16 slotNumber;
    S16 numPDCCH;
    S16 countPDCCH;
    U32 cinitPDCCH;
    S16 sizePDCCH;
    U8* startPDCCH;
    U8* ptrPDCCH;
    U8* startPBCH;
    U8* ptrPBCH;
    S16 *Grp;
    S16 *PHICH_Bit;
    S16 *PHICH_SequenceIndex;
    U32 txChcuSf[LTEMXSFCH];
    U32 txChIdx;
    PBS_TX_CHAN pBsTxChan[LTEMAXCHAN];

    U32 isExeTaskListReady;
    /* time stamps */
    U32 txStartIndTimeStamp;
    U32 txVectorTimeStampDone;
    U32 txSduTimeStampDone[MAXCHSUBFRAME];
    U32 updatedTimeStamp;
    U32 switchedTimeStamp;
    U32 processedTimeStamp;

    PTASKLIST pExeTaskList[4];
    S16 row_a;
    S16 col_a;

//    S16 *ncol_y;
//    S16 *n1_phich;

    S16 *a;

    PTXSYSTEMINFO *pTxSys;
    PDLRESOURCEPARAM pDLResourceInChan;
    S16 *DependencyArray;
    PDLRESOURCEPARAM pDLResourceIn[MAXCHSUBFRAME];

//	U8 *scrOut1[MAXCHSUBFRAME];
//	U8 *scrOut2[MAXCHSUBFRAME];

    U16 NumTasksTotalTX;
    PTCB pTaskTx1[MAX_NUM_TCB];
    PTCB pTaskTx2[MAX_NUM_TCB];

    RESELEMINFO *pReselmInfo[MAXRESOURCEBLOCKS];
} BS_TX, *PBS_TX;


typedef struct BS_Info
{
// CPType corresponds to the cyclic prefix type: 0 normal 1 extended
// and is derived from cyclicPrefixType in the INITPARM info
    S16 CPType;
    S16 cellGroup;
    S16 cellSector;
    S16 profile;
    S16 N1_id;
    S16 N2_id;
    S16 Ncell_id;
// frameDuplexType can be either FDD (Frame Type 1) or TDD (FrameType 2)
    S16 frameDuplexType;
    S16 BW;
// Number of Subcarriers per slot as a function of BW or numberUsedSubcarriers
    S16 Nused;
    S16 n;
// nRbSc number of subcarriers per Resource Block as a function of the
// subcarrier spacing corresponds to physicalResourceBandwidth (Nrb_sc)
    S16 Nrb_sc;
    S16 Nsymb;
// nFft FFT size as a function of Bandwidth corresponds to ffftSize	(Nfft)
    S16 Nfft;
// expoNfft is the power of 2 for the particular FFT base on Bandwidth
// The only deviation is 15 MHz that requires a special algorithm that
// performs decimation in time and splits the process into 3 512 FFT's and
// a radix 3 stage
    S16 expoNfft;
// Ncp number of samples in cyclic prefix, at the fft sample rate for all
// symbols except the first one as a function of bandwidth and Cyclic Prefix
    S16 Ncp;
// Ncp0 number of samples in cyclic prefix, at the fft sample rate for the
// first symbol as a function of bandwidth and Cyclic
    S16 Ncp0;
// nRb corresponds to the number of Resource Blocks as a function of the BW
// so it is a value in the range of 6 to 100 corresponds to nMaxDlRb and
// nMaxUlRb	  (Nrb)
    S16 Nrb;
// Fs sampling frequency as a function of the BW, it can be derived from a
// table lookup from the samplingRateoverMHz (Fs) (In matlab it uses n but it
// is not necessary to avoid division)
    U32 Fs;
// df subcarrier frequency spacing it can be derived from
// nRbSc if 12 then 15 KHz spacing else 7.5 KHz
	S16 baseRB;
    U16 df;
    S16 Tusym;
    S16 Tsam;
    S16 Tcp;
    S16 Tcp0;
    S16 Tsym;
    S16 Tsym0;
    S16 CarrierIndices[256];
    S16 GuardLowCarrierIndices[38];
    S16 GuardHighCarrierIndices[37];
    PBS_SRS pBsSrs;
	PBS_RX_PRACH pBsRxPrach;
	PBS_RX_PRACHDet pBsRxPrachDet;

} BS, *PBS;

typedef struct RateMatch_Info
{
    S32 E;
    S16 k0;
    S16 Ncb;
} RateMatchParms, *PRateMatchParms;

typedef struct RateMatchingParameters
{
    S32 E;
    S16 k0[4];
    S16 Ncb;
} RMParameters, *PRMParameters;

// Information for the processing of the RxSDUs
//typedef struct LtePhyRxSubChInfo
//{
//    U32 rxBits;
//} SUBCHRXINFO, *PSUBCHRXINFO;


// Information for the processing of channel scheduling for those channels with
// persistent scheduling enabled

typedef struct LtePhyChanDescriptor
{
    // Start Resource Block Pair or CCE for this allocation
    S16 startRb;
    // Total Resource Block Pairs used in this allocation
    S16 totalRbUsed;
    // Channel defined flag, set for persistent entries. For dynamic entries
    // set after TXSTART req and clear after TXEND indication
    S16 chDefined;
    // Subframe number that marks the first time the allocation was defined
    S16 refSubFrame;
    // Frame number that together with the subframe number allows us to find
    // out if the allocation has expired or not
    U16 refFrame;
    // Running counter in subframes to determine time out and period
    S16 runSubFrCtr;
    // scTxRbInfo contains the number of pair of RB's used for codeword0 and
    // codeword1 and are updated during txSdu processing
    U16 scTxRbInfo[2];
    // pFecTxOut is an array of pointers for each subchannel Fec Block Output
    U8* pFecTxOut[2];
    // fecOutBits is an array with the number of output bits present
    // in the tx FEC out(Expected ie G0,G1 in Matlab code)
    U32 fecInOutBits[2];
    // Number of bytes received for each codeword from the MAC
    U32 numBytes[2];
    // pFecRxIn is an array of pointers for each subchannel receive Fec In
    U8* pFecRxIn[2];
    // RbMap is used for debugging purposes as it is easy to visualize the
    // Pair of resource blocks mapping
    // This is a bit map for RB's allocation for this channel, it is the result
    // of processing the allocations of both subchannels
    // The bit map is as follows
    // RbMap[0] RB0, RB1,..., RB31
    // RbMap[1] RB32,......., RB63
    // RbMap[2] RB64,.........RB95
    // RbMap[3] RB96.. RB99
    // Although 36.211 and 36.213 have 110 as maximum number of RB's in certain
    // tables, the 36.804 document that specifies the BS Tx & Rx only requires
    // 100 RB's for 20 MHz Bandwith, 110 corresponds to 22 MHz BW which it
    // is not currently listed as a required BW
    U32 RbMap[4];
//    SUBCHRXINFO scRxInfo[2];
    U32 rxBits[2];
    S16 *d0IQ[14];
    S16 *d1IQ[14];
    S32 length_d0IQ[14];
    S32 length_d1IQ[14];
    S32 M0symb[14];
    S32 M1symb[14];
    S16 *x_layer[14];
    S16 *y_precoder[14];
    S32 nrow_x[14];
    S32 ncol_x[14];
    S16 *ncol_y;
    S16 phyChanScale_factor;
    S16 phyChanPowerdB_RS;
    S16 *l_k_Nsc;  //[14];
    S16 *l_k_Isc[14]; // old K_L in reverse
    S32 *pBit;   //[14];
    DLCHANDESC chi;
    S16 Grp;
    S16 SequenceIndex;
    U8  phichBit;
    U16 Nlast[2];
	S16 numCCE;
	U16 nRNTI;

} CHANDESC, *PCHANDESC;


typedef struct LtePhyULChanDescriptor
{
    // Start Resource Block Pair or CCE for this allocation
    U8 startRb;
    // Total Resource Block Pairs used in this allocation
    U8 totalRbUsed;
    // Channel defined flag, set for persistent entries. For dynamic entries
    // set after TXSTART req and clear after TXEND indication
    U8 chDefined;
    // Subframe number that marks the first time the allocation was defined
    U8 refSubFrame;
    // Frame number that together with the subframe number allows us to find
    // out if the allocation has expired or not
    U16 refFrame;
    // Running counter in subframes to determine time out and period
    U8 runSubFrCtr;
    // scTxRbInfo contains the number of pair of RB's used for codeword0 and
    // codeword1 and are updated during txSdu processing
    U16 scTxRbInfo[2];
    // pFecTxOut is an array of pointers for each subchannel Fec Block Output
    U8* pFecTxOut[2];
    // fecOutBits is an array with the number of output bits present
    // in the tx FEC out(Expected ie G0,G1 in Matlab code)
    U32 fecInOutBits[2];
    // Number of bytes received for each codeword from the MAC
    U32 numBytes[2];
    // pFecRxIn is an array of pointers for each subchannel receive Fec In
    U8* pFecRxIn[2];
    // RbMap is used for debugging purposes as it is easy to visualize the
    // Pair of resource blocks mapping
    // This is a bit map for RB's allocation for this channel, it is the result
    // of processing the allocations of both subchannels
    // The bit map is as follows
    // RbMap[0] RB0, RB1,..., RB31
    // RbMap[1] RB32,......., RB63
    // RbMap[2] RB64,.........RB95
    // RbMap[3] RB96.. RB99
    // Although 36.211 and 36.213 have 110 as maximum number of RB's in certain
    // tables, the 36.804 document that specifies the BS Tx & Rx only requires
    // 100 RB's for 20 MHz Bandwith, 110 corresponds to 22 MHz BW which it
    // is not currently listed as a required BW
    U32 RbMap[4];
//    SUBCHRXINFO scRxInfo[2];
    U32 rxBits[2];
    S16 *d0IQ[14];
    S16 *d1IQ[14];
    S32 length_d0IQ[14];
    S32 length_d1IQ[14];
    S32 M0symb[14];
    S32 M1symb[14];
    S16 *x_layer[14];
    S16 *y_precoder[14];
    S32 nrow_x[14];
    S32 ncol_x[14];
    S16 ncol_y[14];
    S16 phyChanScale_factor;
    S16 phyChanPower;
    S16 l_k_Nsc[14];
    S16 *l_k_Isc[14]; // old K_L in reverse
    S32 pBit[14];
    ULCHANDESC chi;

} ULCHDESC, *PULCHDESC;


// The following structures are partly based on information contained in
// matlab file phy_bs_config.m (The matlab variables are stated in
// parenthesis in the comments

typedef struct LtePhyDescriptor
{
    // Pointer to Tx IQ Samples storage indexed by layer
    S16* sigOut[MAXNUMLAYERS];
    // Pointer to Rx IQ Samples storage indexed by layer
    S16* sigIn[MAXNUMLAYERS];
    // txFrameNumber
    U16 txFrameNumber;
    // radioAccessMode
    S16 radioAccessMode;
    // rxFrameNumber
    U16 rxFrameNumber;
    //  dacTxScale corresponds to the scale factor for the Tx IQ Samples
    S16 dacTxScale;
    // Pointer to array (k,l) coordinates for frequency index and time_index for
    // consecutive complex modulation symbols
    // Need to move at channel context whenever we go to multi-channel types
//    S16** k_l;

    // Auto generate TX I/Q samples
    U32 autoTx;
    // Expected number of TXSDUs
    S32 expectedTxSdus;
    // Pointers to TXVECTOR and RXVECTOR information for current subframe
    PDLSUBFRDESC txVector;
    PULSUBFRDESC rxVector;
    // Keep an array of existing channels including type, persistent scheduling,
    // subframe number, length of the persistent schedule
    CHANDESC txChan[LTEMAXCHAN];
    ULCHDESC rxChan[LTEMAXCHAN];
    // For New PBCH Mode save PBCH channel info to be reused
    CHANDESC savedPbch;
    U8 savedPbchScrOut[4][PBCHDATASIZE];
    // Array of channel id's to be used in the subfrane processing, entries are
    // updated during txSdu processing and an index is updated (Points to next
    // entry).Once the TXEND indication is issued the index is reset and the
    // number of channels to be processed is also cleared.
    // API Control
    U32 confEnableMask;     // API confirmations (TXSDU, TXSTART, RXSTART)

    PBS pBs;
    PBS_TX pBsTx;
    PBS_RX pBsRx;

//    S16 *RG_Assigned_Tx;
    S16 **RG_Assigned_Rx;
//    S16 RG_Assignment[11];

//    U16 numPDCCHQuadruplets;
    U32 numberofControlChannels;
    S16 pilotScaleFactor;
    S16 primarySyncScaleFactor;
    S16 secondarySyncScaleFactor;
    S16 controlIndicatorScaleFactor;

    S16 pilotPowerdB_EPRE; //pilotPower;
    S16 primarySyncPowerdB_RS; //primarySyncPower;
    S16 secondarySyncPowerdB_RS; //secondarySyncPower;
    S16 controlIndicatorPowerdB_RS; //controlIndicatorPower;
    U16 paprControl;
    U16 paprThreshold;
    U32 number_of_phich_channels;

    S16 numPHICH;
    S16 Ng;

    S16 *r_alpha_uv;
	S16 *nOc;
    S16 *nOcBar;
	S16 *Sdescramb;
    U32 cinit_PBCH;

    S16 *rs_grid[TXANT_FOUR*14];
    S16 *rs_grid_ctrl[3 * TXANT_FOUR];
    U8 *mallocVar1;
    U8 *mallocVar2;

    U8 *scrOutBufs[2];
    U32 *scrIndex[2];
    U32 _totalsizescr[2];

    S8 *descrInBufs;
    U32 *descrIndex;
    U32 *demapOutSize;
    U32 _totalsizedescr;

    TXMODULATIONVARS TxModulationVars;

    PHARQS  pharq;
} LTEPHYDESC, *PLTEPHYDESC;

// The following structure is used for testing the LTE PHY and it is based on
// the matlab files test_lte_dl.m
typedef struct LtePhyTestDescriptor
{
    // PHYProfile in matlab	test_lte_dl.m
    U32 phyProfile;
    // PHYModulation in matlab test_lte_dl.m
    U32 phyModulation;
    //  TransChanType in matlab test_lte_dl.m
    U32 transChanType;
    //  numPDCCHSymbols corresponds to the number of Physical Downlink Control
    //  channel symbols
    U32 numPDCCHSSymbols;

} LTETSTDESC, *PLTETSTDESC;

// Array of Pointers to LTEPHYDESC indexed by the InstanceID
//extern PLTEPHYDESC PhyInst[MAXNUMPHYINSTANCES];

typedef struct ChEntry
{
    U32 fecInOutBits[2];
    U32 numBytes[2];
    U32 numRxAnts;
} CHENTRY, *PCHENTRY;

typedef struct RcvrIODescriptor
{
    CHENTRY chentry[LTEMAXCHAN];
} RXIODESC, *PRXIODESC;

typedef struct tagProfilInfo
{
    U32 TaskID;
    U32 Ticks;

} ProfilInfo, *PProfilInfo;

typedef struct PreLayerMapTaskInfo
{
    U32 transmode;
    U32 M0symb;
    U32 M1symb;
    U32 Q;
    U32 numTxLayers;
    U32 numTxantennas;
    U32 length_dIQ;
} PRELAYMPTSK, *PPRELAYMPTSK;

typedef struct LayerMapTaskInfo
{
    U32 M0symb;
    U32 M1symb;
    U32 numTxLayers;
    U32 transmode;
} LAYERMAPTSK, *PLAYERMAPTSK;

typedef struct PostLayerMapTaskInfo
{
    U32 nrow_x;
    U32 ncol_x;
} POSTLAYMAPTSK, *PPOSTLAYMAPTSK;

typedef struct TxTaskInfo
{
    PRELAYMPTSK preLayMpTsk;
    LAYERMAPTSK layerMpTsk;
    POSTLAYMAPTSK postLayMpTsk;
    S16 *x;
    S16 *y;
    S32 length_d0_IQ;
    S32 length_b0;
    S32 length_d1IQ;
    S32 M0symb;
    S32 M1symb;
    S32 length_y;
    S32 Mlayer_symb;
    S32 ncol_x;
    S32 nrow_x;
    S32 alloc_length;
    S16 numrows_k_l;
    S16 numcols_k_l;
    S16 *w;
    S16 *wbar;
} TRANSMITTSKINFO, *PTRANSMITTSKINFO;

typedef struct ArrayPtr
{
    S16 *strt_add;
} ARRAYPTR, *PARRAY;

typedef struct PostSCFMDADemParams
{
    S16 islot;
    S16 iSymInSlot;
    S16 Nul_symb;
    S16 iSymInSubframe;
    S16 Nfft;
    S16 Nrb_sc;
    S16 Nul_rb;
    S16 expo_fac;
    S16 mo;
    S16 MO;
    S16 MI;
} POSTSCFDMADEMPARAM, *PPOSTSCFDMADEMPARAM;

typedef struct RxEpilogueParams
{
    S16 *bDetHard_fx;
    S16 *bDetSoft_fx;
    S16 *RBAssign;
    S16 *rgQm_ul_ptr;
    S16 *ademod;
    S16 *NfftInSamples;
    S16 *aP;
    S16 **aDet;
    S32 *reEst1;
    S16 *reDet;
    S32 *ChEst_pwr_fx;
    S16 *bUserDetHard;
    S16 *bUserDetSoft;
    S16 **expo_mat;
    S16 **expo_mat_ch;
    S16 expo_mrc;
    S16 *expo_norm_reEst;
    S16 *expo_reEst;
    S32 *reEst1_temp;
    S16 *reDet_temp;
    S16 *aP_temp;
    S16 *chEst_ul_temp;
} RXEPILOGUEPARM, *PRXEPILOGUEPARM;

//	FFT IO params
typedef struct tag_IO_FFTParams
{
	S16 Exp;
	U16 FFT_Order;
	S16* ExpAddr;
    S16 isym;

} IOParams_FFT, *PIOParams_FFT;

typedef struct tagFEC_LteContext
{
// FEC
	unsigned short CodingType;   // fec_mode (0=CTC, 1=CC to the HW interface)
    unsigned short Blocksize;    // In bits, minimum 40 bits, maximum 6144 bits for Enc,
    unsigned short CRC_Sel;      // (0:disabled, 1 crc24a. 2 crc24b) (Enc and dec)
    unsigned short Filler;       // Number of filler bits(Enc in)
    unsigned short ScrambEnable; // Scrambler Enable 0 disable (Enc)
    unsigned short ScrambMask;   // Scrambler Mask(Enc)
    unsigned short BypassIntl;   // Bypass Inter/Dintleaver: 1 bypass (Enc)(dec)
    unsigned short BypasEnc;     // Bypass Encoder: 1 bypass (Enc)
    unsigned short BypasRateMatching; // Bypass Rate Matching: 1 bypass
    unsigned short tc_rate;      // tc7000 (tc_rate goes to rate index in HW
    unsigned short tc_modi;      // tc7000 (tc_modi / 2 goes to modulation in Wimax, 0 always for LTE)
    unsigned short tc_nout;      // tc7000 Encoder Output Ptr (Bit array)(Enc),(Dec)Frame Size in bits

} FEC_LteContext, *PFEC_LteContext;

////////////////////////////////////////////
//  New task list                         //
////////////////////////////////////////////
/*
typedef struct tag_IO_InitTX_Subfr
{
	U32* pPhy;
	S16* a;

} IOParams_InitTX_Subfr, *PIOParams_InitTX_Subfr;
*/
typedef struct tag_IO_SetupIFFT_Bufs
{
    S16* ifft_in;
    S16* a;
    S16 row_a;
    S16 col_a;
    S16 isym;
    S16 mi;
    S16 Nfft;
    S16 Ncpsam;

} IOParams_SetupIFFT_Bufs, *PIOParams_SetupIFFT_Bufs;

typedef struct tag_IO_PAPR_RCF
{
    S16 PAPR_mode;
    S16 Nfft;
    S16* ifft_out;
    S16 papr_power_thrsh;
    S16* xbar;

} IOParams_PAPR_RCF, *PIOParams_PAPR_RCF;

typedef struct tag_IO_AddCP
{
    S16* ptr_dac_samp;
    S16* ifft_out;
    S16 Nfft;
    S16 Ncpsam;

} IOParams_AddCP, *PIOParams_AddCP;

typedef struct tag_IO_LoadSymbol
{
    S16* pa1;
    S16* pa;
    S16 Ntx_antennas;
    S16 row_a;
    S16 col_a;
    S16 isym;
} IOParams_LoadSymbol, *PIOParams_LoadSymbol;

typedef struct tag_IO_SetupSymbDataInpBufs
{
    U32 isymstart, isymstop;
	U32 ichanStart, numChan;
    U16 Ntx_antennas;
    S16 RBIndexMid6Lo;
    S16 RBIndexMid6Hi;
	PCHANDESC pChan1;
	U32 *index;
    RESELEMINFO *pResInfo;
	PTXSYSTEMINFO *pTxSysIn;
} IOParams_SetupSymbDataInpBufs, *PIOParams_SetupSymbDataInpBufs;

typedef struct DlPilotsInputParams
{
    S16 Ncell_id;
    S16 CPType;
    S16 Ndl_rb;
    S16 Ndl_symb;
    U16 Nrb_sc;
    S16 Ntx_antennas;
    S16 slotNumber;
    S16 pilotScaleFactor;
    S16 isym;
    S32 stepsize;
    S32 stepsize_a;
} IOParams_DlPilots, *PIOParams_DlPilots;

typedef struct DlSyncParams
{
    S16 cellSector;
    S16 Ndl_rb;
    S16 Nrb_sc;
    S16 Ndl_symb;
    S16 scaleP;
    S16 scaleS;
    S16 SyncTxAntennaNum;
    S16 Ntx_antennas;
    S16 slotNumber;
    S32 stepsize_a;
    S16 isym;
    S16* d0;
    S16* d1;
} IOParams_DlSync, *PIOParams_DlSync;


typedef struct DlControlParams
{
    S16* a;
    S16* PHICH_Bit;
    S16* PHICH_SequenceIndex;
    U8* startPDCCH;
    S16* Grp;
    S16 numTxAntennas;
    S16 numPDCCHSymbols;
    S16 numcols_a;
    U32 number_of_phich_channels;
    S16 CPType;
    U32 Ncell_id;
    S16 controlIndicatorScaleFactor;
    S16 N_PHICH_Grp;
    S16 sizePDCCH;
    S16 scalefactorPDCCH;
    S16 scalefactorPHICH;
    S16 pilotScaleFactor;
    S16 numGroups;
    S16 Ndl_rb;
    S16 Nrb_sc;
    S16 col_a;
    S16 row_a;
    S16 slotNumber;
    S16 stepsize_a;
    U32 transmode;
    S16 numCtrlSymbols;
} IOParams_DlControl, *PIOParams_DlControl;

typedef struct SRS_Params
{
    S16 df;
    U32 Fs;
    S16 Nul_symb;
    S16 Nrb_sc;
    S16 NusedE;
    S16 Nfft;
    S16 numSrs;
    S16 mo;
    S16 K;
    S16 L;
    S16 Nul_rb;
    S16 srsPilotSymbol;
    S32 stepsize_aDet;
} IOParams_SRS, *PIOParams_SRS;


typedef struct SRS_PilotsParams
{
    S16 subframeNumber;
    S16 Nrb_sc;
    U32 Ncell_id;
    S16 groupHoppingEnabled;
    S16 numSrs;
} IOParams_SRS_Pilots, *PIOParams_SRS_Pilots;

// Temporary
//extern PTCB pTaskTx[MAX_NUM_TCB];
//extern PTASKLIST pTaskListTx;
//extern PTASKLIST pTaskListTx1;
//extern PTASKLIST pTaskListTx2;
//extern U16 NumTasksTotalRX;
//extern U16 NumNodesRX;
//extern U16 NumTasksDoneRX;
//extern PTASKLIST pTaskListRx;
//extern PTASKLIST pTaskListRx1;
//extern PTASKLIST pTaskListRx2;
extern void SchedulerRunNextTX(void);

////////////////////////////////////////////

//extern PLTEPHYDESC PhyInst[MAXNUMPHYINSTANCES];
//extern ULSUBFRDESC rxVector;
//extern DLSUBFRDESC txVector;

#endif /* LTEPHY_H */
