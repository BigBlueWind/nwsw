//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ext_gbl_var_fx.h
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.138.2.2.6.2 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#if defined (LOWERARM)
#include "phyprofiler.h"
#endif

#include "ltephy.h"
#include "ltephydef.h"

#if defined (RT_API_PROC)
#include "../Arm/Prj/LtePhyRt/LteBsTxDataSdu.h"
#include "../Arm/Prj/LtePhyRt/LteBsRxDataSdu.h"
#endif

#if !defined (_WIN32) && !defined(CEVA_INTRINSICS)
#include "rexcomp.h"
#endif

extern U32 glbObjStart[512];
extern U32 glbObjSize[512];
extern U32 glbObjNum[512];
extern U32 glbNumObj;
extern U32 glbTotalSize;

extern U32 gCtxDlFec;
extern U8 *gScrInBufs[2][2];
extern U8 *gCRC24A_buf[2];
extern U8 *gScrOutBufs[2][2];
extern U32 *gScrIndex[2][2];
extern U32 g_totalsizescr[2][2];
extern U32 g_scrsize[2][2];
#if defined (RT_API_PROC)
extern PLTE_PHY_SDU_DESC gpSduDesc[2];
#endif

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
extern S16 CA_PrecV2U[8], CA_PrecV3U[18], CA_PrecV4U[32];
extern S16 CA_PrecSTCP2[32], CA_PrecSTCP4[256];
extern S16 CA_WPrecCodebookP2V2[48];
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


extern S16 nVRB_BW_1_4[12];
extern S16 nVRB_BW_3[28];
extern S16 nVRB_BW_5[48];
extern S16 nVRB_BW_10[184];
extern S16 nVRB_BW_15[256];
extern S16 nVRB_BW_20[384];

extern S16 CA_NumREinRBPairsMid6_Bcst_all[3][4];

extern S16 CA_NumREinRBPairsMid6_Sync_BW_1_4[6];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_3[7];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_5[7];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_10[6];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_15[7];
extern S16 CA_NumREinRBPairsMid6_Sync_BW_20[6];

extern S16 CA_NumREinRBPairsMid6_Bcst_BW_1_4[3][6];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_3[3][7];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_5[3][7];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_10[3][6];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_15[3][7];
extern S16 CA_NumREinRBPairsMid6_Bcst_BW_20[3][6];

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
extern S16 CE_m0Col, CE_m1Col;
extern S16 CE_NmaxDL_rb;
extern S16 CE_NormalCP, CE_ExtendedCP;
extern S16 CE_PrsNc, CE_PrsX1;
extern S16 CE_SingleAntenna, CE_SpatMux, CE_Diversity;
extern S16 CE_SmallDelay;
extern S16 CE_TDD;
extern S16 CE_UL_SC_FDMA;
extern S16 CE_ZeroDelay;

extern S16 CE_RACH;
extern S16 CE_UL_SCH;

extern S16 CE_BCH;
extern S16 CE_DL_SCH;
extern S16 CE_MCH;
extern S16 CE_PCH;

extern S16 CE_UCI;

extern S16 CE_CFI;
extern S16 CE_DCI;
extern S16 CE_HI;

extern S16 CE_PRACH;
extern S16 CE_PUSCH;

extern S16 CE_PUCCH;

extern S16 CE_PBCH;
extern S16 CE_PDSCH;
extern S16 CE_PMCH;

extern S16 CE_PCFICH;
extern S16 CE_PHICH;
extern S16 CE_PDCCH;

extern U32 CE_GenPolyCRC24A;
extern U32 CE_GenPolyCRC24B;

extern S16 CE_IndexBPSK;
extern S16 CE_IndexQPSK;
extern S16 CE_Index16QAM;
extern S16 CE_Index64QAM;

//extern S16 CarrierIndices[2048];
//extern S16 PilotCarrierIndices[4][4][3];
extern S16 REinSubfrmSym_Ref[4][14][2];
extern S16 NumREInSubfrmSym_Ref[4][14][14];
extern S16 REinSubfrmSym_PDSCH_ROT0[3 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_ROT1[3 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_ROT2[3 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_ROT3[3 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_ROT4[3 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_ROT5[3 * 7 * 12];
extern S16 NumREinSubfrmSym_PDSCH[3][14];
extern S16 REinSubfrmSym_PBCH_ROT0[4 * 12];
extern S16 REinSubfrmSym_PBCH_ROT1[4 * 12];
extern S16 REinSubfrmSym_PBCH_ROT2[4 * 12];
extern S16 NumREinSubfrmSym_PBCH[2][14];
extern S16 REinSubfrmSym_PDCCH_ROT0[2 * 3 * 12];
extern S16 REinSubfrmSym_PDCCH_ROT1[2 * 3 * 12];
extern S16 REinSubfrmSym_PDCCH_ROT2[2 * 3 * 12];
extern S16 NumREinSubfrmSym_PDCCH[2][14];
extern U32 SAMPLINGFREQ[6];
extern U16 CEXPFFT[6];
extern U16 CYCLICPRE0_NRML[7];
extern U16 CYCLICPRE0_EXT[7];
extern U16 CYCLICPRE_NRML[7];
extern U16 CYCLICPRE_EXT[7];
extern S16 CA_PrimeNumberTable[];
extern S16 UlPilots_QthRootTable[];
extern S16 CA_ULPilotCarrierSymbol[2];
extern S16 CA_NumULDataCarriersPerResourceBlock[2];

extern S16 UlPilots_Div_Nrs_zc_Table[];
extern S16 Bit_Reversal[256];

extern short Table_FFT16b_4096[];

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

extern S16 Pilot_symbol_loc[6];
extern S16 Num_Pilots_Slot[6];
extern S16 pucchF1_data_symbol_loc[4];
extern S16 pucchF2_data_symbol_loc[5];
extern S16 pucchF2ab_data_symbol_loc[6];

extern S16 Num_Pilots_Slot_TX[6];

extern S16 assignment_1_2_antennas[];
extern S16 assignment_4_antennas[];

//extern U8 CFI_Bits[32*5];
extern U32 CFI_DW[5];

extern U16 max_num_PDCCH[6];

extern S16 TabN_PHICH_GRP[24];

//extern S16 W_PHICH[4][4];
extern S16 W_PHICH[8][4];
extern U8 HARQ_Bits[2 * 8];

extern S16 NgTable[4];

extern S16 cc_1_3_dectab[64][2];

extern U32 rm_20_13_enctab[13];
extern U32 rm_32_11_enctab[11];

extern S16 rm_dectab_20_Offset[11];
extern S8  rm_dectab_20_X[9522];

/*extern S8 rm_dectab_20_2[202];
extern S8 rm_dectab_20_3[203];
extern S8 rm_dectab_20_4[138];
extern S8 rm_dectab_20_5[97];
extern S8 rm_dectab_20_6[64];
extern S8 rm_dectab_20_7[315];
extern S8 rm_dectab_20_8[356];
extern S8 rm_dectab_20_9[349];
extern S8 rm_dectab_20_10[338];
extern S8 rm_dectab_20_11[287];
extern S8 rm_dectab_20_12[306];
extern S8 rm_dectab_20_13[307];
extern S8* rm_dectab_20_X[13];*/

extern S8 rm_dectab_32_2[376];
extern S8 rm_dectab_32_3[375];
extern S8 rm_dectab_32_4[376];
extern S8 rm_dectab_32_5[261];
extern S8 rm_dectab_32_6[166];
extern S8 rm_dectab_32_7[287];
extern S8 rm_dectab_32_8[360];
extern S8 rm_dectab_32_9[293];
extern S8 rm_dectab_32_10[358];
extern S8 rm_dectab_32_11[347];
extern S8* rm_dectab_32_X[11];

extern S8 FHT_MASK[4096];

extern S32 Interleaver[32];

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
extern U8 macSdu[5000];

#if defined (RT_API_PROC)
extern volatile unsigned int _my_sfn;
extern volatile unsigned int gPhyRobustControl;
extern volatile unsigned int gPhyInternalBch;
extern volatile unsigned int gUseTxSduPointer;
extern volatile unsigned int gTwoRxMuxControlLists;
extern volatile unsigned int gSendSecondTTIIndication;
extern volatile unsigned int gMuteRxSduForSrRach;
extern volatile unsigned int gLowLatencyFlag;
extern volatile PMAC2PHY_QUEUE_EL pRxSduRxEnd[NUM_UL_SDU_CONTEXT];
extern volatile PLTE_PHY_UL_SDU_DESC pRxSduDesc[NUM_UL_SDU_CONTEXT][MAXCHSUBFRAME];
extern volatile LTE_PHY_UL_PUSCH_SDU_DESC SduPuschDesc[NUM_UL_SDU_CONTEXT];
extern volatile LTE_PHY_UL_SHARED_PARAMS UlSharedParams[NUM_UL_SDU_CONTEXT];
extern volatile PLTE_PHY_UL_SDU_DESC pRxSduFirstPusch[NUM_UL_SDU_CONTEXT];
extern volatile PLTE_PHY_UL_SDU_DESC pRxSduFirstPucch[NUM_UL_SDU_CONTEXT];
extern volatile PLTE_PHY_UL_SDU_DESC pRxSduFirstMuxCtrlAckNack[NUM_UL_SDU_CONTEXT];
extern volatile PLTE_PHY_UL_SDU_DESC pRxSduFirstMuxCtrlCqiRi[NUM_UL_SDU_CONTEXT];
extern volatile U32 *pRxRipMeas[NUM_UL_SDU_CONTEXT];
extern volatile U32 *pRxThermalMeas[NUM_UL_SDU_CONTEXT];
extern volatile U32 gMissedApiTable[P_HARQ_PROC];
extern volatile U32 gRxSduCtx;
extern volatile U32 gRxSduListState[NUM_UL_SDU_CONTEXT];
extern volatile U32 RxControlSduSyncCtrl[NUM_UL_SDU_CONTEXT];
extern volatile U32 stopPhy;
extern volatile U32 gStartSubfrTimeMark;
extern volatile U32 gPhyLogsEnableMask;
extern volatile U32 gDefaultApiLoggingMask;
extern volatile U32 gDefaultApiLoggingResetCounter;
extern volatile U32 gDefaultApiLoggingTxSduMaxSize;
extern volatile U32 gDefaultApiLoggingRxSduMaxSize;
extern volatile U32 gDefaultApiLoggingCore;
extern volatile U32 gDefaultIqEnableMask;
extern volatile U32 gDefaultPhyStatsEnableMask;
extern volatile INITPARM gStoredInitParm;
extern volatile U32 gMode;
extern volatile U32 gRunFftSlot0, gNumSlotIrq;
extern volatile U32 rxVectorProcDone;

extern U32 gTTIPauseResumeCount;
extern U32 gFirstMessageArrived;
extern U32 gLastTxSfn, gLastRxSfn;

#ifndef CEVA_INTRINSICS
extern U8  CevaDataSectionBase[0x15000];
extern U8 *DataBuff_Bank1, *DataBuff_Bank2, *DataBuff_Bank3;
extern U8 *DataBuff_Bank1a, *DataBuff_Bank3a;
extern U8 *DataBuff_Bank1_Off, *DataBuff_Bank2_Off, *DataBuff_Bank3_Off;
#endif  //CEVA_INTRINSICS

#else   //RT_API_PROC

extern U32 gLastTxSfn, gLastRxSfn;
extern U32 gPhyRobustControl;
extern U32 gPhyInternalBch;
extern U32 gMissedApiTable[P_HARQ_PROC];
extern U32 gUseTxSduPointer;
extern U32 gTwoRxMuxControlLists;
extern U32 gSendSecondTTIIndication;
extern U32 gMuteRxSduForSrRach;
extern U32 gLowLatencyFlag;
extern U32 gMode;

#ifndef CEVA_INTRINSICS
extern U8  CevaDataSectionBase[0x15000];
extern U8 *DataBuff_Bank1, *DataBuff_Bank2, *DataBuff_Bank3;
extern U8 *DataBuff_Bank1a, *DataBuff_Bank3a;
extern U8 *DataBuff_Bank1_Off, *DataBuff_Bank2_Off, *DataBuff_Bank3_Off;
#endif  //CEVA_INTRINSICS

#endif  //RT_API_PROC

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
extern S16* CA_srsBandwidthConfigTable_cram;
extern S16 CA_srsSubframeConfigTable[];

extern S32 count_samp; // total number of samples per antenna
extern S16 papr_flag;
extern S16 papr_power_thrsh;
extern S16 invTab[8];

extern S16 CA_prachFrameStructureTable[];
extern S16 CA_prachPreambleGenTable_Ncs[];
extern S16 CA_prachPreambleGenTable_numV[];
extern S16 CA_prachPreambleGenTable_numU[];
extern S16 CA_prachRootSeqNumberTable0_3[];
extern S16 prachDet_firLpfMid6[8];
extern S16 prachDet_firLpfMid6_15[15];
extern S16 prachDet_firLpfMid6_31[31];
extern S16 prachDet_firLpfMid6_61[61];
extern S16 prachDet_firLpfMid6_61b[61];
//extern S16 prach_HBFIR[27];
extern S16 prach_HBFIR[15];
//extern S16 prach_HBFIR_1[7];
extern S16 prach_HBFIR_1[5];
extern S16 prach_fir33_D3[33];

extern S16 zeroCorrZone13[65];
extern S16 zeroCorrZone15[56];
extern S16 zeroCorrZone18[47];
extern S16 zeroCorrZone22[39];
extern S16 zeroCorrZone26[33];
extern S16 zeroCorrZone32[27];
extern S16 zeroCorrZone38[23];
extern S16 zeroCorrZone46[19];
extern S16 zeroCorrZone59[15];
extern S16 zeroCorrZone76[12];
extern S16 zeroCorrZone93[10];
extern S16 zeroCorrZone119[8];
extern S16 zeroCorrZone167[6];
extern S16 zeroCorrZone279[4];
extern S16 zeroCorrZone419[3];
extern S16 zeroCorrZone0[2];
extern U8 Mask[8];
extern U8 MaskCC[8];
extern U32 Mask32[8];
extern U32 Mask32_1[8];

extern SCHEDSTRC gInitScheduler;
extern LTEPHYDESC Inst0[MAX_NUM_OF_SF_CTX];
extern BS bs[MAX_NUM_OF_SF_CTX];
extern BS_TX bsTx[MAX_NUM_OF_SF_CTX];
extern BS_RX bsRx[MAX_NUM_OF_SF_CTX];
extern BS_SRS bsSrs[MAX_NUM_OF_SF_CTX];
extern BS_RX_PRACH bsRxPrach[2*MAX_NUM_OF_SF_CTX];
extern BS_RX_PRACHDet bsRxPrachDet[2*MAX_NUM_OF_SF_CTX];

extern BS_DL_INIT_MEMBUF glb_bsDlInitMemBuf[MAX_NUM_OF_SF_CTX];
extern BS_DL_INIT_PARAM  glb_bsDlInitParam[MAX_NUM_OF_SF_CTX];


extern double max_scale;

extern S16 timeAdjust_TAB1[6];
extern S16 timeAdjust_TAB2[6];

extern S16 Boffset_ACK[16];
extern S16 Boffset_RI[16];
extern S16 Boffset_CQI[16];
extern S16 ColSetACK[2][NUM_SYM_ACK_RI];
extern S16 ColSetRI[2][NUM_SYM_ACK_RI];

extern S16 CyclicShiftTab[8];

extern S16 ScaleComputeErr_Tab[100];

extern S32 UE_NIR_Table[5];
extern S16 PRACH_Tables[];

extern S16 Table_FEC_CodeBlock_C[27][110];
extern S16 Table_FEC_CodeBlock_K[27][110];
extern S16 Table_FEC_RateMatching_Ncb[27][110];
extern S16 Table_FEC_RateMatching_k0[4][27][110];

extern U32 Table_DL_FEC_CTC_CB[9422];

extern S16 RbRa_ratio[8];
extern U16 rm_dectab_32_X_size[11];
extern U16 rm_dectab_20_X_offset[11];
extern U16 rm_dectab_20_X_size[12];


extern short IDFT_Twdd4C_0030_q4[];
extern short IDFT_Twdd4C_0720_q1[];
extern short IDFT_Twdd4C_0768_q1[];
extern short IDFT_Twdd4C_0864_q1[];
extern short IDFT_Twdd4C_0900_q1[];
extern short IDFT_Twdd4C_0960_q1[];
extern short IDFT_Twdd4C_0972_q1[];
extern short IDFT_Twdd4C_1080_q1[];
extern short IDFT_Twdd4C_1152_q1[];
extern short IDFT_Twdd4C_1200_q1[];
extern short IDFT_Twdd4C_1296_q1[];

extern short IDFT_Twdd4C_0720_q2[];
extern short IDFT_Twdd4C_0768_q2[];
extern short IDFT_Twdd4C_0864_q2[];
extern short IDFT_Twdd4C_0900_q2[];
extern short IDFT_Twdd4C_0960_q2[];
extern short IDFT_Twdd4C_0972_q2[];
extern short IDFT_Twdd4C_1080_q2[];
extern short IDFT_Twdd4C_1152_q2[];
extern short IDFT_Twdd4C_1200_q2[];
extern short IDFT_Twdd4C_1296_q2[];

extern short IDFT_Twdd4C_0720_q3[];
extern short IDFT_Twdd4C_0768_q3[];
extern short IDFT_Twdd4C_0864_q3[];
extern short IDFT_Twdd4C_0900_q3[];
extern short IDFT_Twdd4C_0960_q3[];
extern short IDFT_Twdd4C_0972_q3[];
extern short IDFT_Twdd4C_1080_q3[];
extern short IDFT_Twdd4C_1152_q3[];
extern short IDFT_Twdd4C_1200_q3[];
extern short IDFT_Twdd4C_1296_q3[];

extern short IDFT_3ptSqrN_0360[];
extern short IDFT_3ptSqrN_0384[];
extern short IDFT_3ptSqrN_0432[];
extern short IDFT_3ptSqrN_0480[];
extern short IDFT_3ptSqrN_0540[];
extern short IDFT_3ptSqrN_0576[];
extern short IDFT_3ptSqrN_0600[];
extern short IDFT_3ptSqrN_0648[];
extern short IDFT_3ptSqrN_0720[];
extern short IDFT_3ptSqrN_0768[];
extern short IDFT_3ptSqrN_0864[];
extern short IDFT_3ptSqrN_0900[];
extern short IDFT_3ptSqrN_0960[];
extern short IDFT_3ptSqrN_0972[];
extern short IDFT_3ptSqrN_1080[];
extern short IDFT_3ptSqrN_1152[];
extern short IDFT_3ptSqrN_1200[];
extern short IDFT_3ptSqrN_1296[];

extern short IDFT_ShffIdx_0012[];
extern short IDFT_ShffIdx_0024[];
extern short IDFT_ShffIdx_0036[];
extern short IDFT_ShffIdx_0048[];
extern short IDFT_ShffIdx_0060[];
extern short IDFT_ShffIdx_0072[];
extern short IDFT_ShffIdx_0096[];
extern short IDFT_ShffIdx_0108[];
extern short IDFT_ShffIdx_0120[];
extern short IDFT_ShffIdx_0144[];
extern short IDFT_ShffIdx_0180[];
extern short IDFT_ShffIdx_0192[];
extern short IDFT_ShffIdx_0216[];
extern short IDFT_ShffIdx_0240[];
extern short IDFT_ShffIdx_0288[];

extern short IDFT_ShffIdx_0300[];
extern short IDFT_ShffIdx_0324[];
extern short IDFT_ShffIdx_0360[];
extern short IDFT_ShffIdx_0384[];
extern short IDFT_ShffIdx_0432[];
extern short IDFT_ShffIdx_0480[];
extern short IDFT_ShffIdx_0540[];
extern short IDFT_ShffIdx_0576[];
extern short IDFT_ShffIdx_0600[];
extern short IDFT_ShffIdx_0648[];
extern short IDFT_ShffIdx_0720[];
extern short IDFT_ShffIdx_0768[];
extern short IDFT_ShffIdx_0864[];
extern short IDFT_ShffIdx_0900[];
extern short IDFT_ShffIdx_0960[];
extern short IDFT_ShffIdx_0972[];
extern short IDFT_ShffIdx_1080[];
extern short IDFT_ShffIdx_1152[];
extern short IDFT_ShffIdx_1200[];
extern short IDFT_ShffIdx_1296[];

extern short *pIDFT_Twdd4C[];
extern short szIDFT_Twdd4C[];
extern short *pIDFT_3ptSqrN[];
extern short szIDFT_3ptSqrN[];
extern short *pIDFT_ShffIdx[];
extern short szIDFT_ShffIdx[];

extern S16 REinSubfrmSym_PDSCH_PRS_ROT0[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT1[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT2[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT3[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT4[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT5[3 * 2 * 7 * 12];

extern S16 REinSubfrmSym_PDSCH_PRS_ROT0_Type1[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT0_Type2[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT1_Type1[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT1_Type2[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT2_Type1[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT2_Type2[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT3_Type1[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT3_Type2[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT4_Type1[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT4_Type2[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT5_Type1[3 * 2 * 7 * 12];
extern S16 REinSubfrmSym_PDSCH_PRS_ROT5_Type2[3 * 2 * 7 * 12];


extern S16 NumREinSubfrmSym_PDSCH_PRS[3][14];
extern S16 NumREinSubfrmSym_PDSCH_PRS_Type1_2[3][14];

extern S16 nVRB_BW_1_4[12];
extern S16 nVRB_BW_3[28];
extern S16 nVRB_BW_5[48];
extern S16 nVRB_BW_10[184];
extern S16 nVRB_BW_15[256];
extern S16 nVRB_BW_20[384];

extern U32 ModulationOrderTable [32];
extern U32 TBSIndexTable [32];
extern U32 RedundVersionTable [32];
extern U32 TBSizeTable  [110][27];
extern U32 TBS_L1_L2_Table[112][2];
extern U32 TBS_dci_format_1c[32];

// Decode_Reed_Muller_Conf() function tables
extern S16 RM_FHT_CONF_32_X_TAB[11][14];
extern S16 RM_MLOG_CONF_32_X_TAB[11][14];
extern S16 RM_FHT_CONF_20_X_TAB[13][10];
extern S16 RM_MLOG_CONF_20_X_TAB[13][10];
extern S16 RM_MRDERR_20_X[13];
extern S16 RM_MRDERR_32_X[11];
extern S16 BlkSizTab[P_BLKSIZ_TAB_LEN];

