//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ltephy_ms.h
 *
 * @brief This file defines the structures, constants and variables
 *        used in the implementation of an LTE MS PHY
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------
#ifndef LTEPHY_MS_H
#define LTEPHY_MS_H
#include "LtePhyL2Api.h"
#include "ltephydef.h"
#include "tcbscheduler.h"
#include "ltephy.h"

#define MOD_QPSK  1
#define MOD_16QAM 2
#define MOD_64QAM 3

#define MAX_PROFILE_INFO 1000


typedef struct MS_PUCCH_Info
{
    S16 pucchFormat;
    S16 betaPUCCH;
    S16 delta_shift_PUCCH;
    S16 n1;
    S16 n2;
    S16 N1_cs;
    S16 N2_rb;
    S16 u[20];

} MS_PUCCH, *PMS_PUCCH;

typedef struct MS_PUSCH_Info
{
    S16 u[20];
	S16 v[20];

} MS_PUSCH, *PMS_PUSCH;

typedef struct MS_SRS_Info {

   S16 srsBandwidthTab;
   S16 srsPilotSymbol;
   S16 srsEnable;
   S16 srsBandwidthConfig;  //C_srs, Tables 5.5.3.2-x [3GPP TS 36.211, Sec.5.5.3]
   S16 srsSubframeConfig;   //Table 5.5.3.3-1 [3GPP TS 36.211, Sec.5.5.3]
   S16 srsSimultaneousANandSRS; //[3GPP TS 36.213, Sec.8.2]
   S16 srsSubframeEnable[10];
   S16 u[20];

} MS_SRS, *PMS_SRS;

typedef struct TxChan_SRS_Info {
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

} MS_TX_CHAN_SRS, *PMS_TX_CHAN_SRS;

typedef struct MS_TX_PRACH_Info {
   S16 ms_prach_enable;
   S16 ms_prach_configurationIndex;
   S16 ms_prach_frequencyOffset;
   S16 ms_prach_rootSequenceIndex;
   S16 ms_prach_highSpeedFlag;
   S16 ms_prach_cyclicShiftConfig;
   S16 ms_prach_Nseq;
   S16 ms_prach_Ncp;
   S16 ms_prach_preambleFormat;
   S16 ms_prach_preambleSeqLength;
   S16 ms_prach_subcSpacing;
   S16 ms_prach_subcOffset;
   S16 ms_prach_subcSpacingRatio;
   S16 ms_prach_nRa_prb;
   S16 ms_prach_k0;
   S16 ms_prach_preambleSubframeEnable[10];
   S16 ms_prach_TseqMicrosec;
} MS_TX_PRACH, *PMS_TX_PRACH;

typedef struct MsTxChan_Info
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
    S16 scaleFactor;
    S32 numBitsInTransportBlock;
    S32 numBitsInCodeword;
    S16 nPRS[20];
    S16 **RBAssignment;
    U8 *mac_sdu;

    U32 G;
    S16 rowRBAssignment;
    S16 colRBAssignment;
    S16 Mpusch_rb;
    S16 Mpusch_sc;
    S8 *by;
    U32 HalfIterations;

    PMS_PUCCH pMsPucch;
    PMS_PUSCH pMsPusch;

} MS_TX_CHAN, *PMS_TX_CHAN;

typedef struct MS_Tx_Info
{
    S16 groupHoppingEnabled;
    S16 Ntx_antennas;
    S16 numMS;
    S16 Nul_rb;
    S16 Nul_symb;
    S16 Ndata_rb;
	S16 frameNumber;
    S16 subframeNumber;
    S16 slotNumber;
    S16 **adcbuf;
    S16 n1_DMRS;
	S16 numChan;
    S16 numDataChan;
    S16 numControlChan;
    S16 numSrsChan;
    S16 numPUCCHSymbols;
    U32 txChcuSf[LTEMXSFCH];
    U32 txChIdx;
    PMS_TX_CHAN pMsTxChan[LTEMXSFCH];
    PMS_TX_CHAN_SRS pMsTxSrs[MAX_UL_SRS];

} MS_TX, *PMS_TX;

typedef struct MsRxChan_Info
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
    U32 G0;
    U32 G1;
    S16 **l_k_Nsc; // number of subcarriers(RE) ocuppied per symbol per channel
    S16 **l_k_Isc; // location of subcarriers(RE) ocuppied per symbol per channel
    S32 **pBit; // Begining location of bits per symbol per channel
    S16 *Q; // number of codewords indicator per channel
    U8 *mac_sdu;
    S16 **RBAssignment;
	S16 Row_RBAssignment;
    S16 Col_RBAssignment;
    U8 *transportBlock0;
    U8 *transportBlock1;
	S8 *by;
    U8 *dec_out;
    U32 HalfIterations;
	S16 *bUserDetUncoded;
	S8 *bUserDetUncodedSoft;
	S32 offset_bit;
// Pointer to array (k,l) coordinates for frequency index and time_index for
// consecutive complex modulation symbols
//    S16** k_l;
} MS_RX_CHAN, *PMS_RX_CHAN;

typedef struct RxChannelInfo
{
	//----- tx mapper
    S16 Qm;
    S16 modulation_type;
    S16 Q;
    S16 l_k_Nsc;
    U32 length_b;
    U32 length_pb;
    U32 Nsubc;
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
} RXCHANNELINFO, *PRXCHANNELINFO;

typedef struct MS_Rx_Info
{
    S16 USR;
    S16 firSyncDelay;

    S16 numPDCCHSymbols;
    S16 Ntx_antennas;
	S16 Nrx_antennas;
	S16 numChan;
    S16 numDataChan;
    S16 numControlChan;
    S16 pilotPower;
    S16 pilotScaleFactor;
    S16 syncPower;
    S16 syncScaleFactor;
// nDLRb is the number of resource elements per slot as a function of BW in the
// downlink direction(Ndl_rb)
    S16 nDlRb;
// nDlSymb is the number of Downlink Symbols per slot it is derived from
// cpType and the nRbSc as follows cpType=0 then nDlSymb=7
// cpType=1 and nRbSc=12 then nDlSymb=6 else nDlSymb=3 (Ndl_symb)
    S16 nDlSymb;
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
    S16 sync2_d[2][62];
    S16 subframeNumber;
	S16 rxSubFrameNumber;
    S16 slotNumber;
    U32 rxChcuSf[LTEMXSFCH];
    U32 rxChIdx;
    PMS_RX_CHAN pMsRxChan[LTEMAXCHAN];

} MS_RX, *PMS_RX;

typedef struct RxSystemInfo
{
    U32 numChan;
    U32 numCtlCh;
    S16 bs_Ntx_antennas;
    S16 rxSubFrameNumber;
    S16 numCtrlSymbols;
    S16 nDlRb;
    S16 nRbSc;
    S16 nDlSymb;
    RXCHANNELINFO RxChanInfo0[100];
    RXCHANNELINFO RxChanInfo1[100];
} RXSYSTEMINFO, *PRXSYSTEMINFO;

typedef struct MS_Info
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
    PMS_SRS pMsSrs;
	PMS_TX_PRACH pMsTxPrach;


} MS, *PMS;



typedef struct RateMatchMs_Info
{
    S32 E;
    S16 k0;
    S16 Ncb;
} RateMatchParmsMs, *PRateMatchParmsMs;

// Information for the processing of the RxSDUs
//typedef struct LtePhyRxSubChInfo
//{
//    U32 rxBits;
//} SUBCHRXINFO, *PSUBCHRXINFO;


// Information for the processing of channel scheduling for those channels with
// persistent scheduling enabled

typedef struct LtePhyChanMsDescriptor
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
    U32 numBits[2];
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
    S16 phyChanPowerdB_RS;
    S16 l_k_Nsc[14];
    S16 *l_k_Isc[14]; // old K_L in reverse
    S32 pBit[14];
    DLCHANDESC chi;
    S16 Grp;
    S16 SequenceIndex;
    U8  phichBit;

} DLCHANDESCMS, *PDLCHANDESCMS;

typedef struct LteMsPhyULChanDescriptor
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
    U16 scRxRbInfo[2];
    // pFecTxOut is an array of pointers for each subchannel Fec Block Output
    U8* pFecTxOut[2];
    // fecOutBits is an array with the number of output bits present
    // in the tx FEC out(Expected ie G0,G1 in Matlab code)
    U32 fecInOutBits[2];
    // Number of bytes received for each codeword from the MAC
    U32 numBits[2];
    // Number of bits received for each codeword from the MAC
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
	S16 phyChanPilotScale_factor;
    S16 l_k_Nsc[14];
    S16 *l_k_Isc[14]; // old K_L in reverse
    S32 pBit[14];
    ULCHANDESC chi;

} ULCHANDESCMS, *PULCHANDESCMS;

// The following structures are partly based on information contained in
// matlab file phy_bs_config.m (The matlab variables are stated in
// parenthesis in the comments
typedef struct LtePhyMsDescriptor
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
    S16** k_l;

    // Pointers to TXVECTOR and RXVECTOR information for current subframe
    PDLSUBFRDESC rxVector;
    PULSUBFRDESC txVector;
    // Keep an array of existing channels including type, persistent scheduling,
    // subframe number, length of the persistent schedule
    ULCHANDESCMS txChan[LTEMAXCHAN];
    DLCHANDESCMS rxChan[LTEMAXCHAN];
    // Array of channel id's to be used in the subfrane processing, entries are
    // updated during txSdu processing and an index is updated (Points to next
    // entry).Once the TXEND indication is issued the index is reset and the
    // number of channels to be processed is also cleared.
    // API Control
    S16 txSduConfEnable;

    PMS pMs;
    PMS_TX pMsTx;
    PMS_RX pMsRx;

    S16 **RG_Assigned_Tx;
    S16 **RG_Assigned_Rx;
    S16 RG_Assignment[11];

    U16 numPDCCHQuadruplets;
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

    S16 Nsf_PHICH;
    S16 N_PHICH_Grp;
    S16 N_PHICH_Grp_Prime;
    S16 numPHICH;
    S16 Ng;
    S16 RG_Assigned_PCFIH[4];
    S16 *EffectiveNumPHICH;
    S16 Grp[25]; // Maximum number of groups that we can have
    S16 m_c;


    S16 *r_alpha_uv;
	S16 *nOc;
    S16 *nOcBar;
	S16 *Sdescramb;
    U32 cinit_PBCH;

} LTEPHYMSDESC, *PLTEPHYMSDESC;

// Array of Pointers to LTEPHYDESC indexed by the InstanceID
extern PLTEPHYMSDESC PhyInstMs[MAXNUMPHYINSTANCES];
extern DLSUBFRDESC rxMsVector;
extern ULSUBFRDESC txMsVector;

// Temporary
extern unsigned short NumTasksTotalTX;
extern unsigned short NumTasksDoneTX;
extern PTCB pTaskTx[MAX_NUM_TCB];
extern PTASKLIST pTaskListTx;
extern PTASKLIST pTaskListTx1;
extern PTASKLIST pTaskListTx2;
extern U16 NumTasksTotalRX;
extern U16 NumNodesRX;
extern U16 NumTasksDoneRX;
extern PTASKLIST pTaskListRx;
extern PTASKLIST pTaskListRx1;
extern PTASKLIST pTaskListRx2;
extern void SchedulerRunNextTX(void);

////////////////////////////////////////////

#endif /* LTEPHY_MS_H */
