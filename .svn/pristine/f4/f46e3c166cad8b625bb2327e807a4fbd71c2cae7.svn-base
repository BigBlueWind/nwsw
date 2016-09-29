
#if defined (LOWERARM)
#include "phyprofiler.h"
#endif

#include "ltephy.h"
#include "ltephydef.h"
#include "tcbscheduler.h"

extern S16 MODTAB_BPSK_256[];
extern S16 MODTAB_QPSK_256[];
extern S16 MODTAB_16QAM_256[];
extern S16 MODTAB_64QAM_64[];

extern S16 CA_Constel_BPSK[4], CA_Constel_QPSK[8], CA_Constel_16QAM[32];
extern S16 CA_Constel_64QAM[128];
extern S16 CA_ConstelLevel2[2];
extern S16 CA_ConstelLevel4[4];
extern S16 CA_ConstelLevel8[8];
extern S16 isym_tab[12];

extern S16 CA_CTCInternalInterleaverParams[188][3];
extern S16 CA_BtuNcp[2], CA_BtuNcp0[2];
extern S16 CA_Ncpc[4], CA_NumDLDataCarriersPerResourceBlock[4];
extern S16 CA_PrecCodebookP2V1[16], CA_PrecCodebookP2V2[24];
extern S16 CA_PrecCodebookP4V1W[128], CA_PrecCodebookP4V2W[256];
extern S16 CA_PrecCodebookP4V3W[384], CA_PrecCodebookP4V4W[512];
extern S16 CA_PrecV1U[2];
extern S16 CA_PrecV2U[8], CA_PrecV3U[18],  CA_PrecV4U[32];
extern S16 CA_PrecSTCP2[32], CA_PrecSTCP4[256];
extern S16 CA_WPrecCodebookP2V2[48] ;
extern S16 CA_WPrecCodebookP4V2W[512];
extern S16 CA_WPrecCodebookP4V3W[1152];
extern S16 CA_WPrecCodebookP4V4W[2048];

extern S16 CA_RBIndexMid6Lo[6], CA_RBIndexMid6Hi[6];
extern S16 CA_RMCTC_InterColPerm[32];
extern S16 CA_RMCTC_InterColPermConv[32];
extern S16 CA_Sync2CellIDTable[168][2];
extern S16 CA_ZadoffChu[372];
extern U32 PsuedoRandomGenInitValue[31];
extern S16 PsuedoRandomGenParityTable[256];

extern S16 CE_CCK;
extern U32 CE_CCGenPolyX, CE_CCGenPolyY;
extern S16 CE_CTCParamColf1, CE_CTCParamColf2, CE_CTCParamColKi;
extern S16 CE_DL_OFDMA;
extern S16 CE_DL_SCH;
extern S16 CE_PBCH, CE_PDCCH, CE_PDSCH;
extern S16 CE_FDD;
extern S16 CE_Prof_BW_1_4, CE_Prof_BW_3, CE_Prof_BW_5;
extern S16 CE_Prof_BW_10, CE_Prof_BW_15, CE_Prof_BW_20;
extern S16 CE_ScaleConstel_BPSK, CE_ScaleConstel_QPSK;
extern S16 CE_ScaleConstel_16QAM, CE_ScaleConstel_64QAM;
extern S16 CE_SyncTxAntennaNum;
extern S16 CE_IndexQPSKl;
extern S16 CE_LargeDelayl;
extern S16 CE_m0Col,CE_m1Col;
extern S16 CE_NmaxDL_rb;
extern S16 CE_NormalCP, CE_ExtendedCP;
extern S16 CE_PrsNc, CE_PrsX1;
extern S16 CE_SingleAntenna, CE_SpatMux, CE_Diversity;
extern S16 CE_SmallDelay;
extern S16 CE_TDD;
extern S16 CE_UL_SC_FDMA;
extern S16 CE_ZeroDelay;


extern S16 CE_RACH     ;
extern S16 CE_UL_SCH   ;

extern S16 CE_BCH      ;
extern S16 CE_DL_SCH   ;
extern S16 CE_MCH      ;
extern S16 CE_PCH      ;

extern S16 CE_UCI      ;

extern S16 CE_CFI      ;
extern S16 CE_DCI      ;
extern S16 CE_HI       ;

extern S16 CE_PRACH    ;
extern S16 CE_PUSCH    ;

extern S16 CE_PUCCH    ;

extern S16 CE_PBCH     ;
extern S16 CE_PDSCH    ;
extern S16 CE_PMCH     ;

extern S16 CE_PCFICH   ;
extern S16 CE_PHICH    ;
extern S16 CE_PDCCH    ;


extern U32 CE_GenPolyCRC24A ;
extern U32 CE_GenPolyCRC24B ;

extern S16 CE_IndexBPSK ;
extern S16 CE_IndexQPSK ;
extern S16 CE_Index16QAM ;
extern S16 CE_Index64QAM ;


//extern S16 CarrierIndices[2048];
//extern S16 PilotCarrierIndices[4][4][3];
extern S16 REinSubfrmSym_Ref[4][14][2];
extern S16 NumREInSubfrmSym_Ref[4][14][14];
extern S16 REinSubfrmSym_PDSCH_ROT0[3*7*12];
extern S16 REinSubfrmSym_PDSCH_ROT1[3*7*12];
extern S16 REinSubfrmSym_PDSCH_ROT2[3*7*12];
extern S16 REinSubfrmSym_PDSCH_ROT3[3*7*12];
extern S16 REinSubfrmSym_PDSCH_ROT4[3*7*12];
extern S16 REinSubfrmSym_PDSCH_ROT5[3*7*12];
extern S16 NumREinSubfrmSym_PDSCH[3][14];
extern S16 REinSubfrmSym_PBCH_ROT0[4*12];
extern S16 REinSubfrmSym_PBCH_ROT1[4*12];
extern S16 REinSubfrmSym_PBCH_ROT2[4*12];
extern S16 NumREinSubfrmSym_PBCH[2][14];
extern S16 REinSubfrmSym_PDCCH_ROT0[2*3*12];
extern S16 REinSubfrmSym_PDCCH_ROT1[2*3*12];
extern S16 REinSubfrmSym_PDCCH_ROT2[2*3*12];
extern S16 NumREinSubfrmSym_PDCCH[2][14];
//extern S16 sync2_d[2][62];
extern U32 SAMPLINGFREQ[6];
extern U16 CEXPFFT[6];
extern U16 CYCLICPRE0_NRML[7];
extern U16 CYCLICPRE0_EXT[7];
extern U16 CYCLICPRE_NRML[7];
extern U16 CYCLICPRE_EXT[7];
extern S16 CA_PrimeNumberTable[220];

extern S16 CA_ULPilotCarrierSymbol[2];
extern S16 CA_NumULDataCarriersPerResourceBlock[2];

extern S16 UlPilots_Div_Nrs_zc_Table[440];
extern S16 Bit_Reversal[256];

extern short Table_FFT16b_4096[];
extern S16 Tab_Cos_Sin[4002];
extern S16 Inv_Tab[257];
extern S16 Div_AB_Tab[130];
extern S16 SCFTAB[8];

extern S16 Tab_Cos_Base2[2];
extern S16 Tab_Cos_Sin_Base3[6];
extern S16 inverse_nrb[108];
extern S16 inverse_nrb_expo[108];

extern S16 Orthog_Seq_PUCCH_fm1[18];
extern S16 Orthog_Seq_PUCCHData_fm1[24];
extern S16 Orthog_Seq_PUCCH_fm2[4];

extern S16 Pilot_symbol_loc[5];
extern S16 Num_Pilots_Slot[6];
extern S16 Num_Pilots_Slot_TX[6];

extern S16 assignment_1_2_antennas[];
extern S16 assignment_4_antennas[];

//extern U8 CFI_Bits[32*5];
extern U32 CFI_DW[5];

extern S16 TabN_PHICH_GRP[24];
//extern S16 W_PHICH[4][4];
extern S16 W_PHICH[8][4];
extern U8 HARQ_Bits[2*8];

extern S16 NgTable[4];

extern S16 cc_1_3_dectab[64][2];
extern U32 rm_20_13_enctab[13];
extern U32 rm_32_11_enctab[11];
extern S8 rm_20_13_dectab[786];
//extern S8 rm_20_13_dectab[156];
//extern S8 rm_20_13_dectab[300];
//extern S8 rm_20_13_dectab[660];
extern S8 rm_32_11_dectab[1494];
//extern S8  rm_32_11_dectab[290];
//extern S8  rm_32_11_dectab[170];

extern S16 DACSCALE[6];

// Tables for Rb Mapping
extern U32 LteDeltaBitMap[32];

extern U32 LteDel1BitMap[32];

extern U16 McsQm_Table[29];

extern U16 McsTbi_Table[29];

extern U16 TbsRbLu01_10[27][10];

extern U16 TbsRbLu11_20[27][10];

extern U16 TbsRbLu21_30[27][10];

extern U16 TbsRbLu31_40[27][20];

extern U16 TbsRbLu41_50[27][20];

extern U16 TbsRbLu51_60[27][20];

extern U16 TbsRbLu61_70[27][20];

extern U16 TbsRbLu71_80[27][20];

extern U32 TbsRbLu81_90[27][20];

extern U32 TbsRbLu91_100[27][20];

extern U32 TbsRbLu101_110[27][20];

extern S16 **RBAssignment_ul;
extern S16 ROW_RBAssignment_ul;
extern S16 COL_RBAssignment_ul;
extern S16 **rgQm_ul;
extern S16 numrows_rgQm_ul, numcols_rgQm_ul;
//extern S16 bUserDetUncoded[MAX_OUTPUT_TRANSPORT_BIT_SIZE];
extern U8 bUserDetCoded[2*MAX_INPUT_TRANSPORT_BYTE_SIZE];
extern U32 SavedLengthDecodedRxData;

extern short W16t0003[];
extern short W16t0004[];
extern short W16t0005[];
extern short W16t0008[];
extern short W16t0009[];
extern short W16t0012[];
extern short W16t0015[];
extern short W16t0016[];
extern short W16t0024[];
extern short W16t0025[];
extern short W16t0027[];
extern short W16t0030[];
extern short W16t0032[];
extern short W16t0036[];
extern short W16t0045[];
extern short W16t0048[];

extern short W16t0720[];
extern short W16t0768[];
extern short W16t0864[];
extern short W16t0900[];
extern short W16t0960[];
extern short W16t0972[];
extern short W16t1080[];
extern short W16t1152[];
extern short W16t1200[];
extern short W16t1296[];
extern short W16t0839[];

extern S8 Phi_mat[360];
extern S8 Phi_mat_24[720];

extern S16 CA_srsBandwidthConfigTable[];
extern S16 CA_srsSubframeConfigTable[];
extern S16 papr_flag;
extern S16 papr_power_thrsh;
extern S16 invTab[8];

extern S16 CA_prachFrameStructureTable[];
extern S16 CA_prachPreambleGenTable_Ncs[];
extern S16 CA_prachPreambleGenTable_numV[];
extern S16 CA_prachPreambleGenTable_numU[];
extern S16 CA_prachRootSeqNumberTable0_3[];
extern S16 prachDet_firLpfMid6[8];

extern U8 Mask[8];
extern U8 MaskCC[8];
extern U32 Mask32[8];
extern U32 Mask32_1[8];

extern SCHEDSTRC gInitScheduler;
extern LTEPHYDESC Inst0[MAX_NUM_OF_SF_CTX];
extern BS  bs[ MAX_NUM_OF_SF_CTX];
extern BS_TX  bsTx[MAX_NUM_OF_SF_CTX];
extern BS_RX  bsRx[MAX_NUM_OF_SF_CTX];
extern BS_RX_CHAN BsRxChan[LTEMXSFCH][MAX_NUM_OF_SF_CTX];
extern BS_PUCCH bsPucch[MAX_NUM_OF_SF_CTX];
extern BS_SRS bsSrs[MAX_NUM_OF_SF_CTX];
extern BS_RX_PRACH bsRxPrach[MAX_NUM_OF_SF_CTX];
extern BS_RX_PRACHDet bsRxPrachDet[MAX_NUM_OF_SF_CTX];

extern double max_scale;

extern S16 CA_NumREinRBPairsMid6_Sync_BW_1_4[];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_3[];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_5[];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_10[];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_15[];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_20[];

extern S16 CA_NumREinRBPairsMid6_Bcst_BW_1_4[3][6];
extern S16	CA_NumREinRBPairsMid6_Bcst_BW_3[3][7];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_5[3][7];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_10[3][6];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_15[3][7];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_20[3][6];

