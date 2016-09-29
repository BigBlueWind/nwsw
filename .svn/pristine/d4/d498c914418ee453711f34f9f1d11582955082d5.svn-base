//-------------------------------------------------------------------------------------------
/** @file prototypes.h
 *
 * @brief PHY Prototypes / Definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.261 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _PROTOTYPE_H_
#define _PROTOTYPE_H_

#include "ltephy.h"
#include "Debug.h"
#include "tcb.h"
#include "LtePhyL2Api.h"

#if !defined(CEVA_INTRINSICS)
// don't need for CEVA and for TestMAC
#include "LteBsPhyStats.h"
#endif

#ifdef CEVA_INTRINSICS
#define         myprintf(x)
#else
#define myprintf    uart_printf
#endif

#if !defined(CEVA_INTRINSICS) || !defined(EVM)
#define         MemCpyFromExt   memcpy
#define         MemCpyToExt     memcpy
#endif

#ifdef _WIN32
#define AUTO_OFF    0
#define AUTO_ON     1

#define stop_printf(...)
#define phy_printf(...) {printf (__VA_ARGS__);}
#define uart_printf(...) {printf (__VA_ARGS__);}
#define _ASSERT_PTR(a)
#endif

#if !defined(_WIN32) && !defined(CEVA_INTRINSICS)
#define phy_printf(...) {uart_printf (__VA_ARGS__);}
#endif


#if !defined (SCHEDULER) && defined (_WIN32)
#define ExeTSchedList(x)
#define ExeTSchedSingle(x)
#define CreateTaskList(x,y) ((PTASKLIST) 1)
#define DestroyTaskList(a)
#endif

#ifdef CEVA_INTRINSICS
#define CEVAEXTERN          __attribute__ ((section (".CSECT cevaboot")))
#define extractTCBIOBuf     EXTRACTTCBIOBUF
#define CEVAINLINE          __inline
#else
#define CEVAEXTERN
#define CEVAINLINE
#endif

//*****************************************************************************
//**********               FUNCTIONAL MACRO DEFINITIONS              **********
//*****************************************************************************

// Function divide_floor(x,y) computes the floor(x/y) where x and y are
// positive integers.

#define divide_floor(x,y)       (x)/(y)

//*****************************************************************************

// Function divide_ceil(x,y) computes the ceil(x/y) where x and y are
// positive integers.

#define divide_ceil(x,y)        ((S32)((x)+(y)-1))/(y)
#define RUP32B(x) (((x)+31)&(~31))
#define RUP8B(x)  (((x)+7)&(~7))
#define RUP4B(x)  (((x)+3)&(~3))

#define TCBIOBUFFILL 0
#define TCBIOBUFREFILL 1

//*****************************************************************************

S16 block_norm(S16 * data_in, S16 *data_out, S32 size, S16 headroom);
void block_shift(S16 *data_in, S16 size, S16 expo);

S16 block_lnorm(S32 * data, S32 size, S16 headroom);

short *svector(long nl, long nh);
void free_svector(short *v, long nl, long nh);
S16 **smatrix(S32, S32, S32, S32);
S16 **smatrix0(S32, S32);

void free_smatrix(S16 **, S32, S32, S32, S32);
void free_smatrix0(S16 **);

void BPSK_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N);
void QPSK_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N, short N1);
void QAM16_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N, short N1);
void QAM64_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N);

// diag
void diagnostic_file_open(U32 numAnt);
void diagnostic_file_close(U32 numAnt);

S32 BsTxChannelCoder(U32 chanId, U32 cwId, U8 in[], S32 nin, U8 out[], S32 G, S16 rvidx, S16 NL,
                     S16 Qm, U16 nRNTI, S16 channelType, S16 slotNumber, S16 Ncell_id,
                     U32 numberofControlChannels, U8* scrOutBufs[], U32 *scrIndex[],
                     U32 _totalsizescr[], S32 NIR);

void bs_tx_broadcast_channel_coding(U8 in[], U8 out[], S32 nin, S16 nant, S32 E);

void Byte2BitStream(U8 bytestream[], U8 bitstream[], U16 length);

void BsTxCrcEncoder(U8 in_bitstream[], U8 out_bitstream[], S32 length);

void AddCodeBlockCRC24B(U8 in_bitstream[], U8 out_bitstream[], S16 length);

S16 BsTxCodeBlkSeg(U8 in_bitstream[], U8 **code_blocks, S16 KIndex[], S16 *FillerBits, S32 B,
                   S16 code_length[], S16 *MM);

#ifdef _WIN32
S32 BsTxRateMatch(S16 **in_bitstream, S32 G_num_bits_transport,
    S16 rvidx, S16 num_FillerBits, S16 NL,
    S16 Qm, S16 CBNum,S16 C_num_code_blocks,
    S16 dlen, U8 out_bitstream[]);
#endif

PRateMatchParms Compute_Rate_match_in_parms(S16 inlen, S32 G_num_bits_transport, S16 rvidx, S16 NL,
                                            S16 Qm, S16 CBNum, S16 C_num_code_blocks);

U16 bitget32(U32 X, S16 b);

////////////////////////////////////////////////////////////////////////////////////////
void PseudoRandomSeqGen(S32 N, U32 cinit, U8 *c);
void PseudoRandomSeqGen2(S32 N, U32 cinit, U8 *c) CEVAEXTERN;
void PseudoRandomSeqGen1(S32 N, U32 *cinit, U32* x1, U8 *c, U32 init);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void scrambler(U32 init, S8 bx[], S8 by[], S32 length_bx);
void scrambler_byte(U32 cinit, S8 bx[], S8 by[], S32 length_bx);
void scrambler_byte1(U32 *cinit, U32 *x1, S8 bx[], S8 by[], S32 length_bx, U32 init);

////////////////////////////////////////////////////////////////////////////////////////
void descrambler(U32 init, S8 bx[], S8 by[], S32 length_bx, U8 *cvec) CEVAEXTERN;
void bs_rx_descrambler(S8 in[], S8 out[], S32 nin, U32 init);

////////////////////////////////////////////////////////////////////////////////////////

void Mapper_fx(U8 b[], S16 Qm, S16 cIQ[], S32 length_b);

////////////////////////////////////////////////////////////////////////////////////////
void DLResourceElementMapper(S16 *a, S16 *y, S16 *scIndex, S16 length_y, S16 scale_factor);
////////////////////////////////////////////////////////////////////////////////////////

U32 PhyBsTx(PLTEPHYDESC pPhy);
U32 PhyBsTxExeTaskList(PLTEPHYDESC pPhy);

void BsTxPDCCH(PLTEPHYDESC pPhy, S16 a[], S16 Ntx_ant, S16 numPDCCHSymbols, S16 slotNumber,
               S32 stepsize_a, S16 scaleFactor);

///////////////////////////////////////////////////////////////////////////////////////

void bs_rx_channel_coding(S32 nin, S32 nout, S8 in[], U8 out[], S16 *stat, PHARQS pharq, S16 NL,
                          S32 G, S16 Qm, S16 rvidx, S32 NIR, U32 halfit, S32 ncqi, S32 nri,
                          S32 Cmux, S16 CP, S16 Itbs, S16 Nprbm1, S16 user);

////////////////////////////////////////////////////////////////////////////////////////

void FSCALE(short int IN[], short int OUT[], short int N, short int FRC);
void PACKBB(U8 IN[], U8 OUT[], S32 N, S16 POS) CEVAEXTERN;
void UNPACKBB(U8 IN[], U8 OUT[], S32 N, S16 POS) CEVAEXTERN;
void CLEAR(short int IN[], short int N);
void bitswap(S16 IN, U16 *IN_swp);

#ifdef _WIN32
S32 BsTxRateMatch2(U8 in[], U8 null[], S16 inlen,
    S32 G_num_bits_transport, S16 rvidx,
    S16 NL, S16 Qm, S16 CBNum,
    S16 C_num_code_blocks, U8 out[]);
#endif

void LTE_Data_Cont_Demux(S8 in[], S8 out1[], S8 out2[], S32 n1, S32 n2);

void LTE_Chan_Deint_QPSK(U16 in[], U16 inb[], U16 out1[], U16 out2[], U16 out3[],
                         S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP,
                         S16 nRI, S16 nACK);
void LTE_Chan_Deint_16QAM(U32 in[], U32 inb[], U32 out1[], U32 out2[], U32 out3[],
                          S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP,
                          S16 nRI, S16 nACK);
void LTE_Chan_Deint_64QAM(U16 in[], U16 inb[], U16 out1[], U16 out2[], U16 out3[],
                          S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP,
                          S16 nRI, S16 nACK);
S32 tx_Mapper_Pack(U8 in[], U8 out[], S32 nin, S16 modt);

void c_mat_mat_mul_fx(S16 *xmat, S16 *ymat, short xdim1, short xdim2, short ydim1, short ydim2,
                      S16 *out_mat) CEVAEXTERN;

void c_mat_vec_mul_fx(S16 *xmat, S16 *yvec, short xdim1, short xdim2, short ydim, S16 *out_vec) CEVAEXTERN;

void ceye_fx(S16 *xmat, short dim);

S16 get_angle_fx(S32 real_part, S32 imag_part);

S16 atan_fx(S16 val);

S16 saturate(S32 L_var1);

void ComputeComplexConjMult(S16 adet[], S16 aPilots[], S16 chEst[], S16 length_krange) CEVAEXTERN;
void ComputeComplexConjMult_A(S16 adet[], S16 aPilots[], S16 chEst[], S16 length_krange);
void noiseAveragingFrequency(S16 *temp_vec1, S16 *chEst, S16 Mpusch_sc, S16 expo_norm1, S16 N, S16 Qm)CEVAEXTERN;
void noiseAveragingFrequency_PART1(S16 *temp_vec1, S16 *chEst, S16 *n_arr, S16 Mpusch_sc, S16 expo_norm1, S16 N, S32 loopCount,S32 flag, S16 Qm)CEVAEXTERN;
void noiseAveragingFrequency_PART2(S16 *chEst,S16 Mpusch_sc, S16 N, S16 NN, S32 loopCount) CEVAEXTERN;
void noiseAveragingFrequency_PART3(S16 *chEst,S16 Mpusch_sc, /*S16 fltLength,*/ S16 N, S16 shift1) CEVAEXTERN;
void noiseAveragingFrequency_PART4(S16 *temp_vec1, S16 *chEst,S16 Mpusch_sc, S16 iE, S16 fltLength) CEVAEXTERN;
void noiseAveragingFrequency_PART1_SRS(S16 *temp_vec1, S16 *chEst, S16 *n_arr, S16 Mpusch_sc, S16 expo_norm1, S16 N) CEVAEXTERN;

void noiseAveragingFrequency_PART1_A(S16 *temp_vec1, S16 *chEst, S16 *n_arr, S16 Mpusch_sc, S16 expo_norm1, S16 N, S32 loopCount,S32 flag)CEVAEXTERN;
void noiseAveragingFrequency_PART2_A(S16 *chEst,S16 Mpusch_sc, S16 N, S16 NN, S32 loopCount)CEVAEXTERN;
void noiseAveragingFrequency_PART3_12_A(S16 *chEst,S16 Mpusch_sc, S16 N, S16 shift1)CEVAEXTERN;
void noiseAveragingFrequency_PART3_24_A(S16 *chEst,S16 Mpusch_sc, S16 N, S16 shift1, S16 eq12)CEVAEXTERN;
void noiseAveragingFrequency_PART4_A(S16 *temp_vec1, S16 *chEst,S16 Mpusch_sc, S16 iE, S16 fltLength)CEVAEXTERN;

void noiseAveragingFrequency_PART1_NA(S16 *temp_vec1, S16 *chEst, S16 *n_arr, S16 Mpusch_sc, S16 expo_norm1, S16 N, S32 loopCount,S32 flag, S16 Qm)CEVAEXTERN;
void noiseAveragingFrequency_PART3_6_A(S16 *chEst,S16 Mpusch_sc, S16 N, S16 shift1, S16 numblks)CEVAEXTERN;
void noiseAveragingFrequency_PART4_6_A(S16 *temp_vec1, S16 *chEst,S16 Mpusch_sc, S16 iE, S16 fltLength)CEVAEXTERN;
void noiseAveragingFrequency_PART1_SRS_A(S16 *temp_vec1, S16 *chEst, S16 *n_arr, S16 Mpusch_sc, S16 expo_norm1, S16 N) CEVAEXTERN;


S32 ComputePhaseDifferencePower(S32 *diffErr, S16 expo, S16 Msc)CEVAEXTERN;
void Compute_phase_errorPUCCH(S16 chEst[], S32 Err[], S16 Mpusch_rb, S16 Mpusch_sc,
                         S16 *pScaleComputeErr_Tab, S32 *ErrSub, S32 *ErrUnWt) CEVAEXTERN;
void Compute_phase_error(S16 chEst[], S32 Err[], S16 Mpusch_rb, S16 Mpusch_sc,
                         S16 *pScaleComputeErr_Tab) CEVAEXTERN;
void Compute_phase_error_A(S16 chEst[], S32 Err[], S16 Mpusch_rb, S16 Mpusch_sc,
                         S16 *pScaleComputeErr_Tab)CEVAEXTERN;
void Compute_phase_errorPUCCH_A(S16 chEst[], S32 Err[], S16 Mpusch_rb, S16 Mpusch_sc,
                         S16 *pScaleComputeErr_Tab, S32 *ErrSub, S32 *ErrUnWt) CEVAEXTERN;
void Compute_Derotation_PartOne(S16 Mpusch_sc, S16 noffset, S16 NegExpoNoffset, S16 temp2,
                                S16 fk[], S16 derotation[])CEVAEXTERN;
void Compute_Derotation_PartOne_A(S16 Mpusch_sc, S16 noffset, S16 NegExpoNoffset, S16 temp2,
                                S16 fk[], S16 derotation[])CEVAEXTERN;
S32 ComputePhaseDifferencePower_A(S32 *diffErr, S16 expo, S16 Msc)CEVAEXTERN;
void preProcessingNoiseSmoothing(S16 chEst[], S16 Mpusch_sc, S16 Nrb_sc) CEVAEXTERN;
void preProcessingNoiseSmoothing_A(S16 chEst[], S16 Mpusch_sc, S16 Nrb_sc)CEVAEXTERN;

void preProcessingNoiseSmoothing2(S16 chEstIn[], S16 chEstOut[], S16 Mpusch_sc, S16 Nrb_sc, S16 avgScale)CEVAEXTERN;

void PucchRbNoisePower(S16 pucch_format, S16 SrsEnable, S16 nPRB_slot, S32 *rxInPwrAvg_PerRbPilot, S32 *rxInPwrAvg_PerRbDataSym,
					   S32 *pAllPucchPwrAvg_AntSlot, S32 *pPucchNoisePwrAvg, U8  *rbUsedFlag)CEVAEXTERN;
void rbPowerAveraging(S16 *adet, S16 Nul_sym, S16 *expo_mat, S16 max_expo, S16 Nul_rb, S16 pilotCarrierSymbol, S32 step_aDet, S32 *rbPwrAvg, U8 *rbUsedFlag) CEVAEXTERN;
void bs_rx_RbPwrAvg(ULRXINPUTPARAMS *pRxInPar, S16 *adet, S32 *rxInRbPwrAvg_AntSlot, U8 *rbUsedFlag, S32 step_aDet, S16 srsPilot, S16 *expo_fac) CEVAEXTERN;
void RipSubframe(S32 *rxInRbPwrAvg_AntSlot, S16 *expo_fac, S32 *rxInRIP, U16 *rxInRIP_dB, S16 MO, S16 Nul_rb, S16 Nfft, S16 *initFlag) CEVAEXTERN;
void RssiSubframe(ULRXINPUTPARAMS *pRxInPar, S32 *rssi_AntSlot, S16 *expo_mat, S16 *max_expo_AntSlot, S32 chan_num) CEVAEXTERN;
void ComputeThermalNoise(S16 numRB, S16 *Temperature, S16 numAntenna, S16 *ThermalNoisePwr)CEVAEXTERN;


////////////////////////////////////////////////////////////////////////////////////////
//BASE_STATION Uplink RX
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//FFT Functions
////////////////////////////////////////////////////////////////////////////////////////
void GenHalfCyclicShift(S16 *mhcs, S16 expo, S16 length) CEVAEXTERN;
void Remove_HalfCyclicShift(S16 *adc, S16 *mhcs, S16 *adcMhcs, S16 length) CEVAEXTERN;
void Remove_HalfCyclicShift_A(S16 *adc, S16 *mhcs, S16 *adcMhcs, S16 length);
void SwapFFT_Out(S16 *input, S16 *output, S32 length, S32 offset, S16 *expo_mat) CEVAEXTERN;
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
//Uplink Rx Demodulation Function Prototypes
////////////////////////////////////////////////////////////////////////////////////////
void ul_Pilot_fx(S16 a[], S16 nPRS[], ULRXINPUTPARAMS *pULPilotInput, S16 u[], S16 v[],
                 S16 pUlPilots_QthRootTable[], S32 ChanNum)CEVAEXTERN;
void ul_Pilot_fx_part1_A(S16 Nrs_zc, S16 q, S16 Nrs_zcDiv, S16 expo, S16 x_q[])CEVAEXTERN;
void ul_Pilot_fx_part2_A(S16 Mrs_sc, S16 Nrb_sc, S16 uSlot, S16 x_q[])CEVAEXTERN;

void ul_Pilot_fx_part3_A(S16 Nrs_zc, S16 Mrs_sc, S16 length_rPUSCH, S16 ncsSlot, S32 slot,
                         S16 x_q[], S16 a[])CEVAEXTERN;
void gen_ul_Pilot_info2_part1_A(S16 Mrs_sc, S16 *rbar_uv, S32 uSlotNum)CEVAEXTERN;
void gen_ul_Pilot_info2_part2_A(S16 Mrs_sc, S16 Nul_symb, S16 *nCs, S16 *rbar_uv, S16* r_alpha_uv)CEVAEXTERN;

void  AccumAllPucchChanPower_PartOne(S16 pScale, S16 dAckScale, S16 dCqiScale, S16 pucch_format, S16 *pChEst, S16 *dEstAck, S16 *dEstCqi,
                                  S32 *allChanPwr_AntSlot, S32 *pPucchPwrAvg)CEVAEXTERN;
void  AccumAllPucchChanPower_PartOne_A(S16 pScale, S16 dAckScale, S16 dCqiScale, S16 pucch_format, S16 *pChEst, S16 *dEstAck, S16 *dEstCqi,
                                  S32 *allChanPwr_AntSlot, S32 *pPucchPwrAvg)CEVAEXTERN;
void ul_Pilot_PUCCH_PartOne(S16 num_Pilots_slot,
                                   S16 Nrb_sc,
                                   S16 Nul_symb,
                                   S16 indexPilot,
                                   S16 ns,
                                   S16 *r_alpha_uv,
                                   S16  a[],
                                   S16 *w_bar_ptr)CEVAEXTERN;
void ul_Pilot_PUCCH_PartOne_A(S16 num_Pilots_slot, S16 Nrb_sc, S16 Nul_symb, S16 indexPilot, S16 ns,
                                   S16 *r_alpha_uv, S16  a[], S16 *w_bar_ptr)CEVAEXTERN;
void ComputeChestAvg_PerRbPilot_A(S16 *chEst, S16 Mpucch_sc, S16 *temp_vec1)CEVAEXTERN;


void bs_rx_DemuxFDMAPhysChan_fx(ULRXINPUTPARAMS *pRxInParams, S32 stepsize_aDet, U32 *rs_grid,
                                S32 ChanNum)CEVAEXTERN;

void gen_ul_Pilot_info(ULRXINPUTPARAMS *pULPilot_Pucch) CEVAEXTERN;

void gen_ul_Pilot_info1(ULRXINPUTPARAMS *pULPilot_Pucch, S16* nOc, S16* nOcBar, S16 *Sdescramb,
                       S16 *nCs, S16 *ncs_cell, S32 ChanNum, S16 SRequest) CEVAEXTERN;
void gen_ul_Pilot_info2(ULRXINPUTPARAMS *pULPilot_Pucch, S16* r_alpha_uv, S16 *nCs,
                        S16 u[]) CEVAEXTERN;

void gen_ul_Pilot_info2_part1(S16 Mrs_sc, S16 *rbar_uv, S32 uSlotNum)CEVAEXTERN;
void gen_ul_Pilot_info2_part2(S16 Mrs_sc, S16 Nul_symb, S16 *nCs, S16 *rbar_uv, S16* r_alpha_uv)CEVAEXTERN;

void ul_Pilot_PUCCH(S16 a[], S16 *r_alpha_uv, S16 *nOcBar, ULRXINPUTPARAMS *pULPilotInput,
                    S32 ChanNum) CEVAEXTERN;

void bs_rx_ChEstimator_part0_fx(S16 *adet, S16 *aPilots, ULRXINPUTPARAMS *pChanEstPar, S16 *chEst,
                                S16 *pScaleComputeErr_Tab, S32 ErrAvg[], S16 mo,
                                S32 ChanNum) CEVAEXTERN;
void bs_rx_ChEstimator_part1_fx(S16 *temp_vec1, S16 *derotation,
                                ULRXINPUTPARAMS *pChanEstPar, S16 *chEst,
                                S32 ErrAvg[], S16 mo,
                                S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar, S32 ChanNum,
                                S16 mErrExpoTemp, S32 *mErrTemp,
                                S32 *rbPwrAvg_AntSlot, U8 *rbUsedFlag, S32 *RxRssi_AntSlot) CEVAEXTERN;

void bs_rx_ChEstimator_part1_fx_1(S16 *chEst, S16 *temp_vec1, S16 Mpusch_sc,
                                   S16 *expo_norm1, S16 *derotation) CEVAEXTERN;
void bs_rx_ChEstimator_part1_fx_1_A(S16 *chEst, S16 *temp_vec1, S16 Mpusch_sc,
                                   S16 *expo_norm1, S16 *derotation);

void Compute_Derotation(S32 Fs, S16 phaseOffset, S16 df, S16 Nfft, S16 Mpusch_sc,
                        S16 NusedE, S16 derotation[], S16 start_adress, S16 islot, S16 *Noffset,
                        S16 *expoNoffset)CEVAEXTERN;
void bs_rx_ChEstimator_PUCCH_part0(S16 *adet, S16 *aPilots, ULRXINPUTPARAMS *pChanEstPar, S16 *chEst,
                             S16 *expo_fac, S16 slot,
                             S32 ChanNum,  S32 *ErrAvg, S16 *pScaleComputeErr_Tab, S32 *ErrSub, S16 *ErrSubExpo, S32 *ErrVec)CEVAEXTERN;

void bs_rx_ChEstimator_PUCCH(S16 *adet,  ULRXINPUTPARAMS *pChanEstPar, S16 *chEst,
                             S16 *expo_fac, S16 slot, S16 *temp_vec1,
                             S32 ChanNum, S16 *r_alpha_uv, S16 *Sdescramb,
                             S16 *nOc, S16 srsPilotSym, S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI,
                             S16 *pChEst_pucch, S32 *pRxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                             S32 *pRxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo, S32 *Err, S16 StartRB)CEVAEXTERN;
void RunningAverage0(ULRXINPUTPARAMS *pRxInParams, S32 *Err, S16 *ErrExpoPusch, S16 *ErrExpo, S32 *ErrAvg,
                    S16 *mErrExpoTemp,  S32 *mErrTemp, S16 Ns, S32 ChanNum) CEVAEXTERN;
void RunningAveragePUCCH(ULRXINPUTPARAMS *pRxInParams, S32 *Err, S32 *ErrAvg,
                         S32 ChanNum, S16 *max_expo_vec, S16 max_expo, S32 *ErrSub_pucch, S16 *ErrSubExpo_pucch);
void RunningAverage(ULRXINPUTPARAMS *pRxInParams, S32 *Err, S16 *ErrExpoPusch, S16 *ErrExpoPucch,
                    S16 Ns, S32 ChanNum) CEVAEXTERN;
void SnrSubframe(ULRXINPUTPARAMS *pRxInPar, S32 *snrPerRb, S32 *snrPilot,
                 S32 ChanNum) CEVAEXTERN;

void rx_Multichan_DeModulation(S16 *RxIn, S16 *chEst_ul, S16 *chEst_pilot, S32 *RxOut, S32 *ChOut,
                               S16 *derotation, S16 *expo_reEst, S16 *expo_pilot,
                               S16 *reDet0, S16 *expo_reEst_norm, S16 *expo_reEst1, S16 expo_norm,
                               S32 *noiseVar, S16 MO, S16 Msc, S16 L, S16 isym, S16 Nul_symb, S16 Qm)CEVAEXTERN;
void bs_rx_ChEstimator_part3_fx(S16 *adet, S16 *derotation, S16 Msc) CEVAEXTERN;
void bs_rx_ChEstimator_part4_fx(S16 *chEstIn, S16 *chEstOut, S16 Msc, S16 isym, S16 *expo_norm) CEVAEXTERN;
void bs_rx_ChEstimator_part4_fx_A(S16 *chEstIn, S16 *chEstOut, S16 Msc, S16 isym, S16 *expo_norm);
void rx_exponent_equalization_16bit(S16 *RXIN, S16 *CHIN, S32 length_b, S16 *expo_reEst,
                                    S16 *expo_ch, S16 Nul_symb, S16 expo_norm, S16 NOUT, S16 MO) CEVAEXTERN;
void rx_exponent_equalization_16bit_upper_A(S16 *RXIN, S16 *CHIN, S32 length_b, S16 *expo_reEst,
                                            S16 *expo_ch, S16 Nul_symb, S16 expo_norm, S16 NOUT,
                                            S16 MO);
void rx_mrcom_c(S16 *pRxIn, S16 *pChEstIn, S32 *pRxOut, S32 *pChPow, S32 num_out,
                S32 ant_offset, S32 num_ant) CEVAEXTERN;
void rx_mrcom_c_A(S16 *pRxIn, S16 *pChEstIn, S32 *pRxOut, S32 *pChPow, S32 num_out,
                S32 ant_offset, S32 num_ant);

void RX_MRCOM_A2(short *RXIN, short *CHIN, long RXOUT[], long CHOUT[], short NOUT, short BLKSIZ,
                 short NANT, short OFFTAB[]);
void bs_rx_feq32b(S32 reEst1[], S16 reDet0[], S32 ChEst_pwr_fx[], S16 expo_reEst[],
                                   S16* expo_reEst_norm, S32 noiseVarAvg, S16 Mpusch_sc) CEVAEXTERN;
void bs_rx_feq32b_A(S32 reEst1[], S16 reDet1[], S32 ChEst_pwr_fx[], S16 expo_reEst[],
                    S16* expo_norm, S16 Mpusch_sc)CEVAEXTERN;
void noiseVarAvgPlus_A(S32 ChEst_pwr_fx[], S16 Mpusch_sc, S32 noiseVarAvg)CEVAEXTERN;
S16 norm32to16(S32 *data, S16 *out, S32 size, S16 headroom);

void bs_rx_SignalDetectionPucch_PartOne_A(S16 * dataSymbolScale,S16 *pRxIn, S16 *pChEstIn,  S16 H_est[],
                                        S16 MO, S16 pScale, S16 dScale)CEVAEXTERN;
void bs_rx_SignalDetectionPucch_PartTwo_A(S16 H_est[], S16 *pRxIn, S32 *pTempI32, S32 *pTempQ32, S32 *pChanPow32, S16 divFactor, S16 MO)CEVAEXTERN;

void rx_Multichan_DeModulation_Pucch(S16 *pRxInACK, S16 *pRxInCQI, S16 *pChEstIn, S32 *pRxOut,
                                     S32 *pChEstPow, S16 *pReDet0,
                                     S16 *expo_ch, S16 *expo_temp,
                                     S16 *expo_reEst_norm, S8* scrSeq,
                                     S8 *bDetSoft, ULRXINPUTPARAMS *pRxInPar,
                                     S32 *rxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                                     S32 *rxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo,
                                     S32 ChanNum, S16 StartRB, S32 *snrPerRb, S32 *snrPilot,
                                     S32 *pAllPucchPwrAvg_AntSlot, S16 *chEstVec, S32 *pPucchPwrAvg) CEVAEXTERN;
void Rotate(S16* reDet, S16 Msc, S16 coeff) CEVAEXTERN;
void CazacCorr(S16 *reDet0, S16 *r_alpha_uv,  S16 Msc, S16 Nul_symb,S16 *Sdescramb, S16 slot,
               S16 *nOc, S16 SrsEnable, S16 expo_fac[], S16 max_expo, S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI ,
               S32 *pRxInPwrAvg_PerRbDataSym, S16 pucch_format) CEVAEXTERN;
void CazacCorr_part1(S16 expo_fac[], S16 max_expo, S16 num_dsym, S16 InvNumSym,  S16 Msc,
               S32 *AvgAcrossSc, S16 *reDet0, S16 *r_alpha_uv, S32 *pRxInPwrAvg_PerRbDataSym, S16 *pdsym) CEVAEXTERN;
void CazacCorr_part1_A(S16 expo_fac[], S16 max_expo, S16 num_dsym, S16 InvNumSym,  S16 Msc,
               S32 *AvgAcrossSc, S16 *reDet0, S16 *r_alpha_uv, S32 *pRxInPwrAvg_PerRbDataSym, S16 *pdsym) CEVAEXTERN;
void CazacCorr_part2(S16 num_dsym, S32 *pAvgAcrossSc, S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI) CEVAEXTERN;
void CazacCorr_part3(S16 num_dsym, S32 *pAvgAcrossSc, S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI) CEVAEXTERN;
void CazacCorr_part4(S16 num_dsym, S16 InvNumSym, S32 *pAvgAcrossSc, S16 *pAvgReDet_pucchACK, S16 *w_ptr) CEVAEXTERN;
void CazacCorr_part4_A(S16 num_dsym, S16 InvNumSym, S32 *pAvgAcrossSc, S16 *pAvgReDet_pucchACK, S16 *w_ptr) CEVAEXTERN;

void bs_rx_SignalDetectionPucch(S16 *pChEstIn, S16 *pRxIn, S16 pucch_format, S16 MO,
                                S16 *pReDet0, S8 *bDetSoft, S32 noisePwrAvg, S16 max_expo) CEVAEXTERN;
void bs_rx_SignalDetectionPucch_PartOne(S16 * dataSymbolScale,S16 *pRxIn, S16 *pChEstIn,  S16 H_est[],
                                        S16 MO, S16 pScale, S16 dScale) CEVAEXTERN;
void bs_rx_SignalDetectionPucch_PartTwo(S16 H_est[], S16 *pRxIn, S32 *pTempI32, S32 *pTempQ32, S32 *pChanPow32, S16  divFactor, S16  MO) CEVAEXTERN;


void bs_rx_RBassignment_pucch(ULRXINPUTPARAMS *pChanEstPar, S32 ChanNum, S16 *nPRB, S16 n1Pucch) CEVAEXTERN;

void AccumAllPucchChanPower(S16 pucch_format, S16 *pChEst, S16 *dEstAck, S16 *dEstCqi, S32 *allChanPwr_AntSlot, S32 *pPucchPwrAvg) CEVAEXTERN;
void bs_rx_SNRPucch(S16 pucch_format, S16 MO, S32 *rxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                    S32 *rxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo, S32 *allPucchPwrAvg_AntSlot,
                    S32 *pPucchPwrAvg, S32 *noisePwrAvg, S16 StartRB, S32 *snrPerRb, S32 *SNR, S16 *max_expo) CEVAEXTERN;
U32 BsRx_PucchSduProcessing(S32 chanNum, S8 *descrInBufs,
                            ULRXINPUTPARAMS *pUlRxInputParams, ULRXSDUPARAMS *pUlRxSduParams,
                            S8 pucch_by[], S8 dec_tab20[], S16 dec_tab20_offset[], S8 fht_mask[]) CEVAEXTERN;

void bs_rx_pucch_chan_coding(S8 in[], U8 out[], S16 format, U16 numbitsOut, S8 dec_tab20[], U16 *rmconf) CEVAEXTERN;

void Decode_Reed_Muller(S8 in[], U16 *out, S32 nin, S32 nout, S8 dectab[], U32 enctab[]) CEVAEXTERN;
void Decode_Reed_Muller_FHT(S8 in[], U16 *out, S16 nin, S16 nout, S8 Mask[]) CEVAEXTERN;

U16 Decode_Reed_Muller_Conf(S8 sftin[], U16 datin, S16 nsft, S16 ndat,
                            U32 enctab[], S16 conftab[], S16 ntab, S16 mrderr);

S32 ComputePwrAvg_PerRbPilot(S16 *adet, S16 Mpucch_sc) CEVAEXTERN;
void ComputeChestAvg_PerRbPilot(S16 *chEst, S16 Mpucch_sc, S16 *temp_vec1) CEVAEXTERN;

void IDFT(S16 *input, U16 idftSize, S16 *output, S16 *exponent);

void rx_Demapper_fx(S16 aEst1_fx[], S16 Qm, S8* scrSeq, S8 *bDetSoft, S16 NusedE, S16 chanType, S16 expo);
void Demod_BPSK(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength);
void Demod_QPSK(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength, S16 expo);
void Demod_QAM16(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength, S16 expo) CEVAEXTERN;
void Demod_QAM64(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength, S16 expo) CEVAEXTERN;

void Demod_QAM16_A(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength, S16 expo);
void Demod_QAM64_A(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength, S16 expo);


////////////////////////////////////////////////////////////////////////////////////////
// Uplink Rx Demodulation TCB Function Prototypes
////////////////////////////////////////////////////////////////////////////////////////
PTCB ul_signal_power_tcb(PTASKLIST pTskLst, S16 *adcbuf0, S16 *adcbuf1, ULRXINPUTPARAMS *pULPilotInput);

PTCB ul_Pilot_PUCCH_tcb(PTASKLIST pTskLst, S16 a[], U32 *rs_grid, S16 *r_alpha_uv,
                        S32 len_r_alpha_uv, S32 step_aDet, S16 *nOcBar, S16 *Sdescramb,
                        ULRXINPUTPARAMS *pULPilotInput,
                        PTCB pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill);

PTCB bs_rx_ChEstimator_PUCCH_part0_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *aPilots,
                                 ULRXINPUTPARAMS *pChanEstPar, S16 slot, S16 mo,
                                 S16 *expo_ch_pucch, S32 step_aDet, S32 *ErrAvg,
                                 S16 *pScaleComputeErr_Tab, S16 *chEst, S32 *ErrSub, S16 *ErrSubExpo,
                                 PTCB pTaskListDependencyTo[], S16 *DependArray,
                                 S16 numDepend, U16 RefillFlag, PTCB pTcbRefill);

PTCB bs_rx_ChEstimator_PUCCH_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *chEst,
                                 ULRXINPUTPARAMS *pChanEstPar, S16 slot, S16 mo,
                                 S16 *expo_ch_pucch, S32 step_aDet, S16 *r_alpha_uv,
                                 S32 len_alpha_uv, S16 *Sdescramb, S16 *nOc, S16 srsPilotSym,
                                 S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI, S16 *pChEst_pucch,
                                 S32 *pRxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                                 S32 *pRxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo,
                                 S32 *pAllPucchPwrAvg_AntSlot, S32 *pPucchPwrAvg, S32 *pRxInRbPwrAvg_AntSlot, U8 *pRbUsedFlag, S32 *pRxRssi_AntSlot,
                                 S32 Err[], PTCB pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill);



PTCB rx_Multichan_DeModulation_Pucch_tcb(PTASKLIST pTskLst, S16 *pRxIn, S16 *expo_ch,
                                   ULRXINPUTPARAMS *pRxInputPars, ULRXSDUPARAMS *pUlRxSduParams,
                                   S16 *pucchChan, S16 numPucchRun,
                                   S8  *scrSeq, U32 *pDescrIndex,
                                   S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI,
                                   S32 *snrPerRb, S32 *snrPilot, S16 *pChEst_pucch,
                                   S32 *pRxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                                   S32 *pRxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo,
                                   S32 *pAllPucchPwrAvg_AntSlot, S32 *pPucchPwrAvg,
                                   S8  *prm_dectab_20_X, S16 *prm_dectab_20_Offset,
                                   PTCB pTaskListDependencyTo[],
                                   S16 *DependArray, S16 numDepend, U16 RefillFlag, PTCB pTcbRefill);

PTCB ul_Pilot_fx_tcb(PTASKLIST pTskLst, S16 a[], ULRXINPUTPARAMS *pULPilotInput, U8 cvec_u[],
                     U8 PRSbits[], S16 bitRevTable[], S16 pUlPilots_QthRootTable[],
                     U32 *rs_grid, U32 stepsize_aDet, PTCB pTaskListDependencyTo,
                     U16 RefillFlag, PTCB pTcbRefill);

PTCB bs_rx_ChEstimator_part0_fx_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *aPilots,
                                    ULRXINPUTPARAMS *pChanEstPar, S32 stepsize_aDet,
                                    S16 *chEst, S16 *pScaleComputeErr_Tab, S32 Err[],
                                    S16 antenna, S16 slot,
                                    S16 *expo_mat, PTCB pTaskListDependencyTo[], S16 *DependArray,
                                    S16 numDepend, U16 RefillFlag, PTCB pTcbRefill);
PTCB bs_rx_ChEstimator_part1_fx_tcb(PTASKLIST pTskLst,
                                    S16 *derotation, ULRXINPUTPARAMS *pChanEstPar, S32 stepsize_aDet,
                                    S16 *chEst, S32 Err[],
                                    S16 antenna, S16 slot, S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar,
                                    S16 *mErrExpoTemp,  S32 *mErrTemp, S32 *rbPwrAvg_AntSlot, U8 *rbUsedFlag, S32 *rssi_AntSlot,
                                    PTCB pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill);
PTCB RunningAverage_tcb(PTASKLIST pTskLst, ULRXINPUTPARAMS *pRxInParams, S32 *ErrAvgIn, S16 *ErrExpoPusch,
                        S16 *ErrExpo,  S32 *ErrAvgOut, S16 *mErrExpoTemp,  S32 *mErrTemp, S16 Ns, PTCB pTaskListDependencyTo[], S16 *DependArray,
                        S16 numDepend, U16 RefillFlag, PTCB pTcbRefill);
PTCB RunningAveragePUCCH_tcb(PTASKLIST pTskLst, ULRXINPUTPARAMS *pRxInParams, S32 *ErrAvgIn, S16 *ErrExpoPucch,
                        S32 *ErrAvgOut,  S32 *ErrSub_pucch, S16 *ErrSubExpo_pucch, PTCB pTaskListDependencyTo[], S16 *DependArray,
                        S16 numDepend, U16 RefillFlag, PTCB pTcbRefill);
PTCB SnrSubframe_tcb(PTASKLIST pTskLst, ULRXINPUTPARAMS *pRxInPar, S32 *snrPerRb, S32 *snrPilot,
                     PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend,
                     U16 RefillFlag, PTCB pTcbRefill);
PTCB RipRssiSubframe_tcb(PTASKLIST pTskLst, ULRXINPUTPARAMS *pRxInPar,
                         S16 *aDet, S32 *rxInRbPwrAvg_AntSlot, U8 *rbUsedFlag, S32 *rxInRIP, S32 step_aDet, S16 srsPilot, S16 *InitFlag,
                         S16 *expo_mat, S32 *rssi_AntSlot,
                         PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend,
                         U16 RefillFlag, PTCB pTcbRefill);
PTCB rx_Multichan_DeModulation_tcb(PTASKLIST pTskLst, S16 *aDet, U32 *rs_grid, S16 *chEst_ul,
                                   S16 *derotation, S16 *reDet0, S16* expo_reEst_norm, S32 step_aDet,
                                   S32 step_reDet, S16 isym, ULRXINPUTPARAMS *pRxInParams,
                                   S16 srsPilot, S32 *noiseVar, PTCB pTaskListDependencyTo[],
                                   S16 *DependArray, S16 numDepend, U16 RefillFlag, PTCB pTcbRefill);
PTCB IDFT_tcb(PTASKLIST pTskLst, S16 *input, S16 *output, S16 isym, S16 srsPilot,
                 PMAP_SETUP_DESCRIPTOR pMapSetupDesc, S32 step_reDet, U16 *idftSize, S16 numPusch, U32 totalIdftSize,
                 PTCB *pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill);
PTCB rx_Demapper_fx_tcb(PTASKLIST pTskLst, S16 reEst1[], S16 *expo_reEst, PULRXFECPARAMS pFecParams,
                        ULRXINPUTPARAMS *pRxInParams, S32 isym, S16 srsPilot, U32 step_reDet,
                        S8 *scrSeq, S32 *descrIndex, S8 *pMuxSoftDec, PTCB pTaskListDependencyTo, U16 RefillFlag,
                        PTCB pTcbRefill);
PTCB muxCntlRI_CQI_Dec_tcb(PTASKLIST pTskLst, PULRXFECPARAMS pFecParams, S8 *pMuxSoftDec,
                                    PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend, U16 RefillFlag, PTCB pTcbRefill);
PTCB muxCntlACK_Dec_tcb(PTASKLIST pTskLst, PULRXFECPARAMS pFecParams, S8 *pMuxSoftDec, PTCB pTaskListDependencyTo[],
                                   S16 *DependArray, S16 numDepend, U16 RefillFlag, PTCB pTcbRefill);


////////////////////////////////////////////////////////////////////////////////////////
// Uplink Rx Demodulation TCB Task Function for running on ARM
////////////////////////////////////////////////////////////////////////////////////////
CEVAINLINE void ul_signal_power_task(PTCB pTCB) CEVAEXTERN;
CEVAINLINE void ul_Pilot_task(PTCB pTCB);
CEVAINLINE void ul_Pilot_PUCCH_task(PTCB pTCB);
CEVAINLINE void bs_rx_ChEstimator_part0_fx_task(PTCB pTCB);
CEVAINLINE void bs_rx_ChEstimator_part1_fx_task(PTCB pTCB);
CEVAINLINE void bs_rx_ChEstimator_PUCCH_part0_task(PTCB pTCB);
CEVAINLINE void bs_rx_ChEstimator_PUCCH_task(PTCB pTCB);
CEVAINLINE void RunningAverage_task(PTCB pTCB);
CEVAINLINE void RunningAveragePUCCH_task(PTCB pTCB);
CEVAINLINE void SnrSubframe_task(PTCB pTCB);
CEVAINLINE void rx_Multichan_DeModulation_task(PTCB pTCB);
CEVAINLINE void rx_Multichan_DeModulation_Pucch_task(PTCB pTCB);
CEVAINLINE void IDFT_task(PTCB pTCB);
CEVAINLINE U32 MapSetupDescriptorsTask(U32 idftSetupDescriptor);
CEVAINLINE void rx_Demapper_fx_task(PTCB pTCB);
CEVAINLINE void muxCntlRI_CQI_Dec_task(PTCB pTCB);
CEVAINLINE U32 muxCntlRI_CQI_Dec_task_wrap(U32 pTCB);
CEVAINLINE void muxCntlACK_Dec_task(PTCB pTCB);
CEVAINLINE void RipRssiSubframe_task(PTCB pTCB) CEVAEXTERN;


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// END UpLink
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////////

void UL_ResourceElementMapper_fx(S16 *a, S16 *y, S16 *k_l_ul_row0, S16 *k_l_ul_row1,
                                 S16 numrows_k_l, S16 numcols_k_l, S16 numcols_a, S16 length_y);

void ul_ConfigPilots_Pusch(ULRXINPUTPARAMS *pULPilotInput, U8 *cvec_u, S16 *u,
                           U8 *PRSbits, S16 *v, S16 *nPRS, S16 bitRevTable[], S32 ChanNum)CEVAEXTERN;

U32 phy_bs_rx(PLTEPHYDESC pPhy);

U32 bs_rx_rnti_get_id(U32 RNTI, PBS_RX_RNTI_MAPPING pRntiMapping, S32 currFrameNum);
void bs_rx_rnti_flush_id(S32 currFrameNum, PBS_RX_RNTI_MAPPING pRntiMapping);

void ul_ConfigFrame_Chan(BS *s, BS_RX *ul)CEVAEXTERN;
void ul_ConfigFrame_SRS_Globals(BS *s, BS_RX *pBsRx, BS_SRS *ul_srs, S16 subframeNum)CEVAEXTERN;
void ul_ConfigFrame_PRACH_Globals(BS *s, BS_RX *pBsRx, BS_RX_PRACH *ul_prach, S16 subframeNum)CEVAEXTERN;

S32 L_divide(S32 L_num, S32 L_denom);

S16 divide_tab_lookup(S16 X) CEVAEXTERN;
void divide_tab_lookup2_A(S16 x1, S16 x2, S16 *pInv2);
S16 divide_s(S16 var1, S16 var2);
S32 L_shift_r(S32 L_var1, S16 var2);
void ComputePuschNoisePwr(S16 *rawChEst, S16 *chEst, S16 Mpusch_sc, S32 *rbPwrAvg, U8 *rbUsedFlag, S32 *RxRssi_AntSlot);

S32 ComputeUpperBound(U16 nFft);
void Compute_phase_errorProfile(S16 chEst[], S32 Err[], S16 Mpusch_sc) CEVAEXTERN;
void ComputeSnrReportingInfo(S16 *rawChEst, S16 *chEst,  S16 numPilots, S16 Mpusch_sc,
                             S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar, S16 lSeg, S16 numRbavg, S16 temp_vec1[], S16 N_rb) CEVAEXTERN;

void ComputeSnrReportingInfoSRS(S16 *rawChEst, S16 *chEst,  S16 numPilots, S16 Mpusch_sc,
                             S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar, S16 lSeg, S16 numRbavg, S16 temp_vec1[], S16 N_rb, S32 noise_power, S16 noise_power_shift) CEVAEXTERN;

S16 ComputeSnrReportingInfo_part1(S16 *rawChEst, S16 *chEst, S16 Mpusch_sc, S16 headroom)CEVAEXTERN;
S16 ComputeSnrReportingInfo_part2(S16 chEstIn[], S16 chEstOut[], S16 Mpusch_sc, S16 headroom, S16 Nrb_sc, S16 avgScale)CEVAEXTERN;
void ComputeSnrReportingInfo_part3(S16 *rawChEst, S16 Mpusch_sc, S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar,
                             S16 numRBavg, S16 temp_vec1[], S32 num_avg, S16 headroom2, S16 expo_num, S16 expo_den, S16 invscale, S16 invexpo)CEVAEXTERN;


void ComputeSnrReportingInfoSRS_part3(S16 *rawChEst, S16 Mpusch_sc, S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar,
                             S16 numRBavg, S16 temp_vec1[], S32 num_avg, S16 headroom2, S16 expo_num, S16 expo_den, S16 invscale, S16 invexpo, S32 noise_power, S16 noise_power_shift)CEVAEXTERN;

S16 ComputeSnrReportingInfo_Part1_A(S16 *rawChEst, S16 *chEst, S16 Mpusch_sc, S16 headroom)CEVAEXTERN;
S16 ComputeSnrReportingInfo_Part2_A(S16 chEstIn[], S16 chEstOut[], S16 Mpusch_sc, S16 headroom, S16 Nrb_sc, S16 avgScale)CEVAEXTERN;
void ComputeSnrReportingInfo_Part3_A(S16 *rawChEst, S16 Mpusch_sc,  S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar, S16 numRBavg,
                                   S16 temp_vec1[],  S32 num_avg, S16 headroom2,  S16 expo_num, S16 expo_den,   S16 invscale,  S16 invexpo)CEVAEXTERN;

S16 log_fx(S32 mant_32, S16 expo) CEVAEXTERN;
S32 fnLog2(S32 L_Input) CEVAEXTERN;
S32 fnLog10(S32 L_Input) CEVAEXTERN;

void AppendCRC16(U8 in_bitstream[], U8 out_bitstream[], S32 length);

void TBCCENC_1_3(U8 in[], U8 out[], S32 n) CEVAEXTERN;
U32 ProcCRC(U32 * pRemainder, U32 polynomial, U32 width, U8 message[], U32 nBytes, U32 nBits) CEVAEXTERN;


S32 CC_SubBlockInterleaver(U8 in[], U8 out[], U8 null[], S32 n) CEVAEXTERN;

void CC_BitSelection(U8 in[], U8 null[], U8 out[], S32 n, S32 E) CEVAEXTERN;

void STC(S16 d0[], S32 M0symb, S16 num_bs_Ntx_antennas, S16 *y);

void CC_Rx_Rate_Matching(S32 n, S32 E, U16 out[], U32 mem) CEVAEXTERN;

void calc_gcrc16(U8 in[], U8 out[2], S32 n);
void calc_gcrc8(U8 in[], U8 *out, S32 n) CEVAEXTERN;

void cc_decoder_1_3(S8 IN[], U8 OUT[], S16 N, U32 mem) CEVAEXTERN;

void bs_rx_cc_cont_chan_coding_EVM(S8 in[], U8 out[], S32 nin, S32 nout, U8 *stat);
void bs_rx_cc_cont_chan_coding(S8 in[], U8 out[], S32 nin, S32 nout, U8 *stat, U32 mem) CEVAEXTERN;

void FSCALEB(S8 in[], S8 out[], S32 n, S16 frc);

////////////////////////////////////////////////////////////////////////////////////////
void PDCFIResourceElementAssignment(PLTEPHYDESC pPhy, S16 Ntx_ant, S16 *k_l_row0, S16 *k_l_row1,
                                    U16 *numrows_k_l, U16 *numcols_k_l, S16 *RG_Assigned);

void BsTxPCFICH(PLTEPHYDESC pPhy, S16 a[], S16 Ntx_ant, S16 slotNumber, S32 stepsize_a,
                S16 scaleFactor);

void TurboBest_CC_Decoder_1_3(signed char in[], unsigned char out[], long int nout,
                              short int sfsize, short int traceback);

void TurboBest_CC_Decoder_Init(void);

void TurboBest_CC_Decoder_Terminate(void);

PTCB fft_tcb_hw(PTASKLIST pTskLst, PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);
void fftcg_c_16t24d(short *xtime_real_img, int m, short *xfreq_real_img, short *Exponent2);
void fftcg_c_16t24d_task(PTCB pTask);

void fft_Win32(S16 expo, S16 length,
                  S16 **adcbuf, S16* aDet,
                  S16 Nul_symb, S16 MO, S32 step_aDet, S16 L, S16 K,
                  S16 Ncp, S16 Ncp0);

void ResourceElementMapper_PS(S16 nDlRb, S16 nRbSc, S16 lIndex, S16 *d, S16 *a, S16 row_a,
                              S16 col_a, S16 SyncTxAntennaNum, S16 scaleFactor);
void SymbolMapper(S16 isym, PLTEPHYDESC pPhy);
void SymbolLayerMapper(S16 isym, PLTEPHYDESC pPhy);
void SymbolPrecoder(S16 isym, PLTEPHYDESC pPhy);
void SymbolDLResourceElementMapper(S16 isym, S16 *a, PLTEPHYDESC pPhy);

void ResourceElementMapperPHICH(S16 *a, S16 *y, S16 *k_l_row0, S16 *k_l_row1, S16 numrows_k_l,
                                S16 numcols_k_l, S16 numcols_a, S32 length_y, S16 offset);

void LTE_PN_Generation_PHICH(U32 Ncell_id, U32 Nsf_PHICH, S16 *ccvec, S16 ns);
void STC_type1(S16 d0[], S32 M0symb, S16 *y);
void STC_type2(S16 d0[], S32 M0symb, S16 *y);

void BsTxPHICH(S16 numTxAntennas, S16 d0_IQ_wz[], S16 Nsf_PHICH, S16 N_PHICH_Grp, S16 *y,
               S16 *y_bar, S16 CPType, S16 m_c, U32 M0symb, U32 stc_alloc_length,
               S16 scaleFactor[], S16 buffWrk[], S16 *EffectiveNumPHICH, S16 y_tilde[]);

void BsTxPHICHMod(PLTEPHYDESC pPhy, U32 ichan, S16 SequenceIndex, S16 cvec[], S16 d0_IQ_wz[]);
S16 Convert_dB_Linear(S16 Power_dB);
void SortMin(S16* array, S16 *array_ind, S16 num);

U32 LteDetermineCpsandSampling(PINITPARM pInit, PLTEPHYDESC pPhy);
void LteDetermineOneOverSamplingFrequency(PINITPARM pInit, BS *bs);
S16 LteDeterminePrachSubcSpacing(S16 prachConfigurationIndex);

U32 LteReadTbsTable(U16 mcsIndex, U16 numRb);
U32 LteDetRbandReforChannelUl(PULCHDESC pChan, U32 nRbSc, U32 nSymb, U32 mode);
extern U32 LteSetupPersReMapsUl(PULCHDESC pChan, PULCHANDESC ptr, U32 nRbSc, U32 nSymb);


U32 LteBsPhyInit(U32 phyInstance, PINITPARM parmPtr);
void LteBsPhyDisplayFreeSpace(char *ptr, int printAntInfo);
U32 LteBsPhyDestroy(U32 phyInstance);
U32 LteBsPhyStop(void);
U32 LteMsPhyInit(U32 phyInstance, PINITPARM parmPtr);
S16 LteDetermineSymbolsPerSlot(S16 cpType, S16 nRbSc);
void LteGenerateAPIMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize, U32 msgType, U32 status);
U32 LteBsPhyTxVectorProcData(PDLSUBFRDESC pDlSubFrDesc, PDLTXINPUTPARAMS pTxInputParams, U32 isInternalBch) CEVAEXTERN;
U32 LteBsPhyTxVectorProcControl(PDLSUBFRDESC pDlSubFrDesc, PIOParams_DlControl pDlCntrl_Pars, PDLTXINPUTPARAMS pTxInputParams) CEVAEXTERN;
U32 LteBsPhyRxVectorProcessing(U32 phyInstance, PULSUBFRDESC ptr, U32 len);
void LteGenerateTxSduConfMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize, U32 status,
                                    U32 chanId);
U32 LteBsTxSduProcessing(U32 phyInstance, U32* ptr, U32 chanId, PTXSDUREQ ptrSDU, U32 cwId);
U32 LteBsTxHiSduProcessing(U32 phyInstance, PTXSDUREQ ptr, U32 chanId, U32 numBytes, U32 cwId);
U32 LteBsTxDciSduReqProcessing(U32 phyInstance, PDCIULSDUMSG ptr, U32 chanId, U16 numBytes);
U32 LteBsTxHiSduReqProcessing(U32 phyInstance, PHIINFOMSGDESC ptr, U32 chanId);

U32 LteSendMsgToMac(U32* outMsg, U32 outMsgSize);
U32 LteSendMsgToUpperNmm(U32* outMsg, U32 outMsgSize);
U32 LteParseMsg(U32 phyInstance, U32* inMsg, U32 inMsgSize, U32* outMsg, U32* outMsgSize);
U32 LteBsTx(U32 phyInstance, S16* sigOut, U32* outMsg, U32* outMsgSize);
U32 LteBsRx(U32 phyInstance, S16* sigIn, U32* outMsg, U32* outMsgSize);

#ifdef _USRDLL
__declspec(dllexport)
#else
extern
#endif /* _USRDLL */
U32 LteBsPhyMain(U32 phyInstance, S16* sigOut, S16* sigIn, U32* inMsg, U32 inMsgSize, U32* outMsg,
                 U32* outMsgSize);
#ifdef _USRDLL
__declspec(dllexport)
#else
extern
#endif /* _USRDLL */
U32 LteBsPhyDllMain(U32 phyInstance, S16* sigOut, S16* sigIn, U32* inputMessage,
                    U32 inputMessageSize, U32* outputMessage, U32* outputMessageSize);


void SchedulerInit(void);
//void SchedulerAddTask(void* pTcb);
void SchedulerLteRun(void);
void SchedulerRunLTETask(U16 taskNum);
int FPGADR_Proc(void* pTCBstruct, int dspnum);

void *CRAMAlloc(U32 size);
U32 CRAMFree(void *ptr);

void LteDFT_16t32d(short *xn, unsigned short N, short *Xk, short *Exp2);
void LteIDFT_16t32d(short *xIn, unsigned short N, short *yOut, short *Exp2);

void SubBlockInterleaver_PDCCH2(U8 in_bytes[], S16 dlen, S16 NilBytes, U8 out_bytes[],
                                U8 out_bytesShadow[], U8 bufferWrk[]);

///////////////////////////////////////////////////////////////////////////////////////
void gen_ul_Pilot_srs_info(S16 Nrb_sc,  S16 m_srs, S16 nCs_srs,
                           S16 *u, S16 *v, S16 *rSrs, S16 *UlPilots_QthRootTable) CEVAEXTERN;
void bs_rx_Pilot_srs(S16 subframeNumber, S16 Nrb_sc, U32 Ncell_id, S16 groupHoppingEnabled, S16 sequenceHopping, S16 *u, S16 *v, S16 delta_ss,
                     S16 *rSrs, S16 *m_srs, S16 *N, S16 *nCs_srs, S16 *bandwidth, S16 numChan,
                     S16 UlPilots_QthRootTable[], U8 cvec[], S16 fgh[]) CEVAEXTERN;
PTCB bs_rx_Pilot_srs_tcb(PTASKLIST pTskLst, S16 subframeNumber, S16 Nrb_sc, U32 Ncell_id,
                         S16 groupHoppingEnabled, S16 sequenceHopping, S16 delta_ss, S16 *u, S16 *v, S16 *rSrs, S16 *m_srs, S16 *N,
                         S16 *nCs_srs, S16 *bandwidth, S16 numChan, S16 Nused, S16 UlPilots_QthRootTable[],
                         PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend,
                         U16 RefillFlag, PTCB pTcbRefill) CEVAEXTERN;
void rx_DemuxResourceElements(S16 *a, S16 *k_l_ul_row0, S16 *k_l_ul_row1, S16 length_k_l, S32 K,
                              S16 *aChan) CEVAEXTERN;
void Compute_DerotationSRS(S32 Fs, S16 phaseOffset, S16 df,  S16 Nfft, S16 Mpusch_sc,
                        S16 NusedE, S16 start_address, S16 islot, S16 *Noffset,
                        S16 *expoNoffset)CEVAEXTERN;

void ul_SrsResourceElementAllocation(S16 Nul_rb, S16 Nrb_sc,  S16 frameNumber,S16 subframeNumber, S16 *startRE,
                                     S16 *numRE, S16 bandwidth,
                                     S16 *m_srs_ptr, S16 *N_srs_ptr, S16 transmissionComb,
                                     S16 freqDomainPosition, S16 hoppingBandwidth, S16 srsConfigIndex) CEVAEXTERN;
void bs_rx_ChEstimatorSrs(S16 iSig_Start, S16 numSig, S16 *cyclicShift, S16 *aEstSig, S16 *rSrs,
                          S16 df, U32 Fs, S16 Nrb_sc, S16 NusedE, S16 Nfft,
                          S16 start_address, S16 islot, S16 Nul_rb, S16 length_rSrs,
                          S16 *chEst, S16 *rawSrsChEst, S16 *pScaleComputeErr_Tab,
                          PBSRXCHMeas pSrsChMeas, S16 *IDFT_Table_Array) CEVAEXTERN;
void SRSReceiverProcessing(S16 df, U32 Fs, S16 Nrb_sc, S16 NusedE, S16 Nfft,
                           S16 srsBandwidthTab, S16 srsBandwidthConfig, S16 *psrsBandwidthConfigTable,
                           S16 *aDet, S16 numSrs, S16 mo,
                           S16 K, S16 L, S16 Nul_rb, S16 srsPilotSymbol, S16 *srsChEst,
                           S16 *rawSrsChEst, S16 *srsPilots,
                           S16 *length_srsPilots, S32 stepsize_aDet, S16 *cyclicShift,
                           S16 *bandwidth, S16 *transmissionComb,
                           S16 *freqDomainPosition, S16 *hoppingBandwidth, S16 *cdmEnable,
                           S16 *cdmGroupIndex, S16 *temp_vec, S16 *pScaleComputeErr_Tab,
                           S32 *snrPerRb, S16 *StartRb, S16 *EndRb, PBSRXCHMeas pSrsChMeas,
                           S16 **pTable_DFT, S16 *srsConfigIndex, S16 frameNumber,S16 subframeNumber) CEVAEXTERN;
PTCB SRSReceiverProcessing_tcb(PTASKLIST pTskLst, S16 df, U32 Fs, S16 Nrb_sc,
                               S16 NusedE, S16 Nfft, S16 srsBandwidthTab, S16 srsBandwidthConfig, S16 *psrsBandwidthConfigTable,
                               S16 *aDet, S16 numSrs, S16 mo, S16 MO,
                               S16 K, S16 L, S16 Nul_rb,
                               S16 srsPilotSymbol, PBS_RX_POINTERS pRxPtr, PBS_RX_CHAN_SRS pBsRxChanSrs,
                               S32 stepsize_aDet,
                               S16 *pScaleComputeErr_Tab, S32 *pSnrPerRb,
                               PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend,
                               PTCB pTaskListDependencyTo1, U16 RefillFlag, PTCB pTcbRefill,S16 **pTable_DFT,S16 frameNumber,S16 subframeNumber);
void SRSSnrReporting(S16 MO, S16 numSrs, S16 Mrb, S16 *StartRb,
                            S16 *EndRb, S32 *snrPerRb, U8 *rxV_snrPerRB) CEVAEXTERN;
PTCB SRSSnrReporting_tcb(PTASKLIST pTskLst, S16 MO, S16 numSrs, S16 Mrb,
                                 S16 *StartRb, S16 *EndRb, S32 *snrPerRb, U8 *rxV_snrPerRB,
                                 PTCB *pTaskDependency, U16 RefillFlag, PTCB pTcbRefill);

void SRSReceiverProcessing_task(PTCB pTask) CEVAEXTERN;
void bs_rx_Pilot_srs_task(PTCB pTask) CEVAEXTERN;
void SRSSnrReporting_task(PTCB pTask) CEVAEXTERN;


//IDFT for SRS
void IDFT_Ceva_fx( short *xF16, short N, short *xT16, short *Exp2, S16 *IDFT_Table_Array, S32 *IDFT_temp) CEVAEXTERN;
void DFT_Ceva_fx(short *xIn, short N, short *yOut, short *Exp2, S16 *IDFT_Table_Array, S32 *IDFT_temp) CEVAEXTERN;
void IDFT_Ceva_TA( short *pTableArray, short *xF16, short N, short *xT16, short *Exp2, S32 *IDFT_temp ) CEVAEXTERN;
void IDFT_Ceva_Setup_Tables(S16 * IDFT_Table_Array,S16 **  pTable_DFT, S16 srsPilotLength) CEVAEXTERN;
void IDFT_Ceva_0024( IDFTparam *pIDFTinfo ) CEVAEXTERN;
void IDFT_Ceva_0048( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0060( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0072( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0096( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0108( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0120( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0144( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0180( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0192( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0240( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0216( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_0288( IDFTparam *pIDFTinfo )CEVAEXTERN;
void IDFT_Ceva_4ptShf11( IDFTparam *pIDFTinfo ) CEVAEXTERN;
void IDFT_Ceva_xTwdd_A(short TwddNptInEachCol, short TwddQCol, int *pxTbuf,int   TwddOffset[], short *pTwdd[5]) CEVAEXTERN;
void IDFT_Ceva_xTwdd( IDFTparam *pIDFTinfo ) CEVAEXTERN;
void IDFT_Ceva_2pt( int *xTbuf, int NCol, int ColOffset, int N2ptIn1Col, int Offset2pt ) CEVAEXTERN;
void IDFT_Ceva_3ptSqrN( IDFTparam *pIDFTinfo ) CEVAEXTERN;
void IDFT_Ceva_Normalize(  IDFTparam *pIDFTinfo, short *xT16, short *Exp2 ) CEVAEXTERN;
void IDFT_Ceva_Normalize_A(  short *xT16, short *Exp2, int *pxTbuf,short *pShffIdx, short Exp2S, short N ) CEVAEXTERN;
void IDFT_Ceva_xTwdd_nGrp( IDFTparam *pIDFTinfo, int nGrp, int GrpOffset, int nColInGrp, int ColOffset ) CEVAEXTERN;
void IDFT_Ceva_xTwdd_nGrp_A( int nGrp, int GrpOffset, int nColInGrp, int ColOffset, short N, int *pxTbuf, int TwddOffset[5], short *pTwdd[5]) CEVAEXTERN;

void IDFT_Ceva_3pt_A( int *xTbuf, int NCol, int ColOffset, int N3ptIn1Col, int Offset3pt ) CEVAEXTERN;
void IDFT_Ceva_5pt_A( int *xTbuf, int NCol, int ColOffset, int N5ptIn1Col, int Offset5pt )CEVAEXTERN;
void IDFT_Ceva_5pt_0( int *xTbuf, int NCol, int ColOffset, int N5ptIn1Col, int Offset5pt ) CEVAEXTERN;
void IDFT_Ceva_3ptSqrN_A( int *pxTbuf, short *p3ptSqrN, int ColOffset3ptSqrN, int N3ptInEachCol, int Offset3ptSqrN, short* pExp2, short E3ptSqrN )CEVAEXTERN;
void IDFT_Ceva_3pt_A_0( int *xTbuf, int NCol, int ColOffset, int N3ptIn1Col, int Offset3pt )CEVAEXTERN;
void IDFT_Ceva_4pt( int *xTbuf, int NCol, int ColOffset, int N4ptIn1Col, int Offset4pt ) CEVAEXTERN;
void IDFT_Ceva_2pt_A( int *xTbuf, int NCol, int ColOffset, int N2ptIn1Col, int Offset2pt )CEVAEXTERN;
void IDFT_Ceva_4ptShf11_A( short *xF16, int *xTbuf, short N, short *Exp2 )CEVAEXTERN;

void IDFT_Ceva_3pt( int *xTbuf, int NCol, int ColOffset, int N3ptIn1Col, int Offset3pt )CEVAEXTERN;
void IDFT_Ceva_5pt( int *xTbuf, int NCol, int ColOffset, int N5ptIn1Col, int Offset5pt )CEVAEXTERN;
///////////////////////////////////////////////////////////////////////////////////////


void PRACH_Rx_Init(BS_PRACH *pbs_prach, BS_RX_PRACH *bsRxPrach, BS_RX_PRACHDet *bsRxPrachDet,
                   S32 length_adcPrach, S16 MO) CEVAEXTERN;

void Prach_DownSampling_NF_33(BS_PRACH *pbs_prach, S16 *pInBuf, S16 *pOut) CEVAEXTERN;

void PRACH_Rx(BS_RX *bsRx, BS_RX_PRACH *bsRxPrach, BS_RX_PRACHDet *bsRxPrachDet, S16 MO,
              S16 **adcbufPrach, S32 length_adcPrach, S16 *pTablesInCRAM, BS_PRACH *pbs_prach_cram, S16 *zczLUT,
              U16 prachDetThreashold) CEVAEXTERN;

PTCB PRACH_Rx_tcb(PBS_RX pBsRx, PBS_RX_PRACH pBsRxPrach, PBS_RX_PRACHDet pBsRxPrachDet, S16 MO,
                  S16 **adcbufPrach, S32 length_adcPrach, S16 *pTablesInCRAM,
                  BS_PRACH *pBsPrachCram, PTASKLIST pTaskList, U16 RefillFlag, PTCB pTcbRefill, S16 *zczLUT) CEVAEXTERN;
CEVAINLINE void bs_rx_prach_task(PTCB pTask) CEVAEXTERN;

///////////////////////////////////////////////////////////////////////////////////////

void SymbolCtlMapper(S16 isym, U8 *b0, U8 *b0_Shadow, U32 length_b0, U16 offset, U32 TotalNsubc,
                     PLTEPHYDESC pPhy);
void SymbolCtlSTC(S16 isym, PLTEPHYDESC pPhy);
void SymbolCtlDLResourceElementMapper(S16 isym, S16 *a, S16 stepsize_a, PLTEPHYDESC pPhy);

///////////////////////////////////////////////////////////////////////////////////////

void* MemAlloc(U32 size, U32 memId, U32 InitFlag);
void MemFree(void* ptr, U32 memId);
void *MemAllocFrom(U8* p, U32 *offset, S32 length);
void delay(S32 i);

void FirDownsample(S16 in[], S16 firLpfMid6[], S16 USR, S16 delay, S32 L, S16 Nf, S16 outd[],
                   S16 *length_outd);
void ComputePrach_x_u(S16 u, S16 Nzc, S16 x_u[]);
void ComputePrach_IDFT_839(S16 k, S16 Nzc, S16 eVec[]);
void ComputePrachEvec(S16 start, S16 end, S16 phi, S16 K, S16 k0, S16 N, S16 *eVec);
void ul_PrachPreamble_Config(BS_RX_PRACH *ul_prach, BS_RX_PRACHDet *ulChan_prach, S16 *u, S16 *Cv,
                             S16 *vLen, S16 *Ncs);
void phy_bs_rx_ConfigFrame(BS_RX_PRACH *ul_prach, BS_RX_PRACHDet *ul_prachDet);
void PRACHReceiverProcessing(BS *bs, BS_RX *bsRx, BS_RX_PRACH *bsRxPrach,
                             BS_RX_PRACHDet *bsRxChanPrach, S16 MO, S16 **adcbufPrach,
                             S32 length_adcPrach);
void mult_complex_vec(S16 Vec1[], S16 Vec2[], S16 VecOut[], S32 N);
void computeCorrelation(S16 Vec1[], S16 Vec2[], S16 VecOut[], U16 N);
void computePwr(S16 Vec[], S16 VecOut[], U16 N);
S16 ratioCompute(S16 num, S16 den, S16 Qfac);
void mac_complex_vec(S16 Vec1[], S16 Vec2[], S16 out[], U16 Nzc);
S16 computeAvgPwr(S16 xDetPwrProfile[], U16 N);

S16* Determine_Sync_Re_alloc(S16 Ndl_rb) CEVAEXTERN;
S16* Determine_Bcst_Re_alloc(S16 Ndl_rb, S16 AntIdx) CEVAEXTERN;
void PdschMCSIndexProcedures(PDLTXINPUTPARAMS pTxInputParams, U32 i) CEVAEXTERN;
S16 ComputePRSAssignment(PDLTXINPUTPARAMS pTxInputParams) CEVAEXTERN;
U32 PuschMCSIndexProcedures(PULSUBFRDESC ptr, S16 Nrb_sc, S16 num_sym, S16 ChanId);
void PuschMCSbasedCalculations(PULCHANDESC pInt, ULRXFECPARAMS *pRxFecParams, S16 num_sym, S16 Nrb_sc, U32 chanIdx);

void SymbolMapperMultiChan(S16 isym, PLTEPHYDESC pPhy);
void SymbolLayerMapperMultiChan(S16 isym, PLTEPHYDESC pPhy);
void SymbolPrecoderMultiChan(S16 isym, PLTEPHYDESC pPhy);
void SymbolDLResElemMapperMultiChan(S16 isym, S16 *a, PLTEPHYDESC pPhy);

void Lte_mapper_multichan_fx_CEVA(PTCB pTcb);
void Lte_layermapper_multichan_fx_CEVA(PTCB pTcb);
void Lte_precoder_multichan_fx_CEVA(PTCB pTcb);
void Lte_dlreselemmapper_multichan_fx_CEVA(PTCB pTcb);

////////////////////////////////////////////
//  New task list                         //
////////////////////////////////////////////
U32 GetTick(void);
U32 GetExecTicks(U32 start_tick);
U32 GetLocalTimerTick(void);
U32 GetLocalTimerExecTicks(U32 start_tick);

int CheckDataIntegrity(PLTEPHYDESC pPhy, U32 numChan, S16 col_a);

void SFDECCOMB(S8 in[], S8 out[], S32 nin, S32 nout) CEVAEXTERN;

void ACK_RI_DeInterLeaver_QPSK (U16 *pScrSeq16, U16 *insft16, U16 *pOutput,
                                S32 Index, S32 outInd, U32 numBits) CEVAEXTERN;
void ACK_RI_DeInterLeaver_QAM16(U32 *pScrSeq16, U32 *insft16, U32 *pOutput,
                                S32 Index, S32 outInd, U32 numBits) CEVAEXTERN;
void ACK_RI_DeInterLeaver_QAM64(U16 *pScrSeq16, U16 *insft16, U16 *pOutput,
                                S32 Index, S32 outInd, U32 numBits) CEVAEXTERN;

void ACK_RI_Decoder(S8 in[], U8 *out, S32 nin, S16 Qm, S16 nbits) CEVAEXTERN;

void bs_rx_mux_control_findpos(PULRXFECPARAMS pFecParams, S32 Cmux, S16 CP) CEVAEXTERN;
void bs_rx_mux_control_extract_softbits_sym(PULRXFECPARAMS pFecParams, S8* scrSeq, S8* insft,
                                            S32 nc, S32 CP, U32 isym,
                                            S8 *pMuxSoftDec) CEVAEXTERN;
void bs_rx_mux_RI_CQI_decoding(PULRXFECPARAMS pFecParams, S8 *pMuxSoftDec, U32 dmemTop,
                                S8 *tablesRM[], S8 FHT_MASK[]) CEVAEXTERN;

void bs_rx_mux_ACK_decoding(PULRXFECPARAMS pFecParams, S8 *pMuxSoftDec) CEVAEXTERN;

void ACK_RI_Encoder(U8 in[], U8 out[], S16 Qm, S16 nin);

void lte_code_blk_seg(S32 B, S16 *C, S16 *Cp, S16 *Cm, S16 *Kp, S16 *Km, S16 *F, S16* BlkSizTab) CEVAEXTERN;

void ComputeCRC24(U8 in[], S32 n, S32 genmsk, U8 crc[]);

void CTC_Encoder_Rate_Matching(U8 in[], U8 out[], S16 nin, S16 nfil, S16 crcsel, S32 E, S16 k0,
                               S16 Ncb, S16 Cblk);

#if !defined(_WIN32 ) && !defined(USE_LINUX) && !defined(CEVA_INTRINSICS)

#include "FECdrv.h"
#include "mdmadrv.h"

#endif

#if !defined(_WIN32 ) && !defined(USE_LINUX) && !defined(CEVA_INTRINSICS)
PFEC_DL_FDESC BuildFecTcb_TX(U16 CodingType, // Type of encoding or decoding (0=CC, 1=CTC)
                             U16 BlockSize, // Code block size
                             U16 CrcSelector, // Type of CRC
                             U16 FillerBits, // Number of initial filler bits in code block
                             U16 Modulation, // Number of modulation bits used (1,2,4,6)
                             U16 CodingRate, // Coding rate
                             U16 HalfIterations, // Number of decoder 1/2 iterations
                             U16 DynamicStopIteration, // CTC decoder automatic stop select
                             U16 ScrambleEnable, // Scrambling enable
                             U16 ScrambleMask, // Scrambling mask
                             U16 SubpacketId, // Sub-packet ID for the itial transmission
                             U16 BpBlockInterleaving, // Block interleaver bypass
                             U16 BpEncoder, // Encoder bypass
                             U16 BpDecoder, // Decoder bypass
                             U16 EnableCombine, // HARQ soft-combining enable
                             U16 BpRateMatching, // Rate matching bypass
                             U32 RateMatchingE, // Rate matching E parameter
                             U16 RateMatchingk0, // Rate matching k0 parameter
                             U16 RateMatchingNcb, // Rate matching Ncb parameter
                             U16 BpChannelInterleaving, // Channel interleaver bypass
                             U16 CiSymbolNumber, // Number of channel interleaver columns (12 or 14)
                             U8 In[], // Input buffer pointer
                             U32 InLen, // Input buffer length
                             U8 Out[], // Output buffer pointer
                             U32 OutLen, // Output buffer length
                             U32 pDlCbCtx, // Pointer to callback context
                             void *FecDlCbDone); // Pointer to callback function
#endif

void RepackBytes(U8** pOut, U8* pIn, S16* remBits, U32 nBits);

//----------------------------------------------------------------------------------------
// Debug Print Functions
//----------------------------------------------------------------------------------------
void filePrint(FILE *fInput, void *inputArray, int size, char *datatype, char *type,
               double scale, int printEnable);
void filePrintSingle(FILE *fInput, char *datatype, char *type, double scale, int printEnable,
                     U16 numargs, ...);
void phy_fprintf(void *fStream, int printEnable, const char *format, ...);

#ifdef _WIN32
void LogRxVector_Main(PULSUBFRDESC ptr);
void LogRxVector_Pusch(PULSUBFRDESC ptr, S32 index, ULRXFECPARAMS *pRxFecParams);
void LogRxVector_Pucch(PULSUBFRDESC ptr, S32 index, S16 startRB);
void LogRxVector_Srs(PULSUBFRDESC ptr, S32 index);
void LogRxVector_Prach(PULSUBFRDESC ptr, S32 index);
#else
#define LogRxVector_Main(x)
#define LogRxVector_Pusch(x, y, z)
#define LogRxVector_Pucch(x, y, z)
#define LogRxVector_Srs(x, y)
#define LogRxVector_Prach(x, y)
#endif

//----------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// DownLink Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////
void DlPilot(S16 Ncell_id, S16 CPType, S16 Ndl_rb, S16 Ndl_symb, S16 Nrb_sc, S16 Ntx_antennas,
             S16 pilotScaleFactor, S16 isym, S16 slotNumber, S16* a[]) CEVAEXTERN;
void DlPilot_part1_A(S16 Ndl_rb, S16 scaleFactor, U8* c, S16* a) CEVAEXTERN;

void DlPilotPosition(S16 Ncell_id, S16 CPType, S16 Ndl_rb, S16 Ndl_rbPrs, S16 Ndl_symb, S16 Nrb_sc, S16 Ntx_antennas,
             S16 pilotScaleFactor, S16 isym, S16 slotNumber, S16* a[]);

////////////////////////////////////////////////////////////////////////////////////////
void BsTxSyncCH(S16 cellSector, S16 Ndl_rb, S16 Nrb_sc, S16 Ndl_symb, S16 scaleP, S16 scaleS,
                S16 SyncTxAntennaNum, S16 slotNumber, S16 isym, S16 *a[], S16 d0[], S16 d1[]) CEVAEXTERN;
////////////////////////////////////////////////////////////////////////////////////////
void SetupSymbDataInpBufs(U32 isymstart, U32 isymstop, U32 ichanStart, U32 numChan,
                          U16 Ntx_antennas, TXSYMBUFS *pTxSymBufs, TXSYMBUF *pTxSymBuf,
                          TXSYMBUFS *pTxSymBufsCeva, TXSYMBUF *pTxSymBufCeva,
                          DLTXINPUTPARAMS *pTxInPars)CEVAEXTERN;

PTCB SetupSymbDataInpBufs_tcb(PTASKLIST pTskLst, PLTEPHYDESC pPhy,
                          TXSYMBUFS *pTxSymBufs, TXSYMBUF *pTxSymBuf,
                          DLTXINPUTPARAMS *pTxInPars, U16 RefillFlag,
                          PTCB pTaskListDependencyTo, PTCB pTcbRefill);

CEVAINLINE void SetupSymbDataInpBufs_task(PTCB pTCB);
U32 SetupSymbDataInpBufs_task_wrap(U32 pTCB) CEVAEXTERN;

void SetupSymbDataInpBufs_part1_A(S16 RBIndexMid6Lo, S16 RBIndexMid6Hi, S16 numEntries,
                    S16 nRbSc, S16 cond1x, S16 cond2x, S16 cond3, U16* *locRE,
                    RESELEMINFO* pResInfo, S16 *tab_ptrPdschCurr, S16 *currNumRE,
                    S16 NumREinSubfrmSym) CEVAEXTERN;

void SetupSymbDataInpBufs_part2_A(S16 RBIndexMid6Lo, S16 RBIndexMid6Hi, S16 numEntries,
                    S16 nRbSc, S16 cond1x, S16 cond2x, S16 cond3, U16* *locRE,
                    RESELEMINFO* pResInfo, S16 *tab_ptrPbchCurr, S16 *currNumRE,
                    S16 NumREinSubfrmSym0, S16 NumREinSubfrmSym1) CEVAEXTERN;

////////////////////////////////////////////////////////////////////////////////////////

void tx_Mapper_MultiChan(TXSYSTEMINFO *pTxSys, DLTXINPUTPARAMS *pTxInPars,
                         TXMAPPERMULTICHANIN *pIn, TXMAPPERMULTICHANOUT *pOut, U32 isym)CEVAEXTERN;
void LayerMapper_MultiChan(TXSYSTEMINFO *pTxSys, DLTXINPUTPARAMS *pTxInPars, TXMAPPERMULTICHANOUT *pIn,
                           TXLAYERMAPPERMULTICHANOUT *pOut, U32 isym) CEVAEXTERN;
void Precoder_MultiChan(TXSYSTEMINFO *pTxSys, DLTXINPUTPARAMS *pTxInPars, TXLAYERMAPPERMULTICHANOUT *pIn,
                        TXPRECODERMULTICHANOUT *pOut, U32 isym) CEVAEXTERN;
void DLResourceElementMapper_MultiChan(TXSYSTEMINFO *pTxSys, DLTXINPUTPARAMS *pTxInPars, TXPRECODERMULTICHANOUT *pIn,
                                       DLRESELEMMAPPERMULTICHANOUT *pOut, U32 isym) CEVAEXTERN;
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
PTCB tx_MultiChan_Modulation_tcb(PTASKLIST pTskLst, PDLTXINPUTPARAMS pTxModVars,
                                 TXSYSTEMINFO *pTxSys, U32 *_totalsizescr, U32 *scrIndex[2],
                                 U8 *scrOut[2], TXSYMBUFS *pTxSymBufs, TXSYMBUF *pTxSymBuf,
                                 S16* sync[2], U32 isym, U16 RefillFlag,
                                 PTCB pTaskListDependencyTo, PTCB pTcbRefill);

void tx_MultiChan_Modulation(PDLTXINPUTPARAMS pVars, PTXMODULATIONPARAMS pIn,
                             TXSYSTEMINFO *pTxSys, TXSYMBUFS *pTxSymBufs, TXSYMBUF *pTxSymBuf,
                             S16 *pSigOut[], S16 *syncP, S16 *syncS, S32 isym) CEVAEXTERN;

CEVAINLINE void tx_MultiChan_Modulation_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

PTCB ifft_tcb_hw(PTASKLIST pTskList, PTCB pTaskListDependencyTo);

PTCB ifft_tcb_sim(PTASKLIST pTskLst, S16 *pRS_Grid, S16 **pIFFTOut, S16 slotNum, S16 fftExpo,
                  S16 num_sym, S16 numSubCar, S16 numTxAnt, S16 CPType, S16 Ncp0, S16 Ncp,
                  U16 RefillFlag, PTCB pTaskListDependencyTo, PTCB pTcbRefill);

void SetupIFFT_Bufs(S16* ifft_in, S16* a, S16 row_a, S16 col_a, S16 isym, S16 mi, S16 Nfft,
                    S16 Ncpsam);
void ifftcg_c_16t24d(short *xfreq_real_img, int m, short *xtime_real_img, short *Exponent2);
void AddCP(S16* ptr_dac_samp, S16* ifft_out, S16 Nfft, S16 Ncpsam);

void ifftcg_c_16t24d_task(PTCB pTask);

///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void tx_Mapper(unsigned char ctxdata[], unsigned long len, short N1, unsigned short modType,
               short MappingGainOut[])CEVAEXTERN;
////////////////////////////////////////////////////////////////////////////////////////

void ResourceElementMapperSymb(S16 *a, S16 *y, S16 *k_l_row0, S16 *k_l_row1, S16 numrows_k_l,
                               S16 numcols_k_l, S16 numrows_a, S32 length_y, S16 scaleFactor) CEVAEXTERN;

////////////////////////////////////////////////////////////////////////////////////////

void LayerMapper(S16 d0[], S16 d1[], S32 M0symb, S32 M1symb, S16 V, S16 *x, U8 transmode,
                 S32 *num_row_x, S32 *num_col_x) CEVAEXTERN;
void LayerMapper_Q1_V2_A(S16 *x, S16 d0[], S32 Mlayer_symb);
void LayerMapper_Q2_V3_A(S16 *x, S16 d1[], S32 Mlayer_symb);
void LayerMapper_Q2_V4_A(S16 *x, S16 d0[], S16 d1[], S32 Mlayer_symb);

////////////////////////////////////////////////////////////////////////////////////////

void precoder(PRECODERPARAM InParam, S16 *x, S16 *y, S32 *length_y, S32 Mlayer_symb) CEVAEXTERN;
void Precoder_Mat_x_Vec(S16 w_dim1, S16 w_dim2, S32 Map_symb, S16 *W_ptr, S16 *x, S16 *y);
void Precoder_LargeCDD_Mat_x_Vec(S16 *pInput, S16 *pOutput, S16 *pWPtr[],
                                  S32 Map_symb, S16 wdu_dim1, S16 wdu_dim2,
                                  S16 x_dim2, S16 y_dim1, S16 CodebookIndex) CEVAEXTERN;
void Precoder_Mat_x_Vec_2_1_A(S32 Map_symb, S16 w_dim1, S16 w_dim2, S16 * W_ptr, S16 * x, S16 * y);
void Precoder_Mat_x_Vec_2_2_A(S32 Map_symb, S16 w_dim1, S16 w_dim2, S16 * W_ptr, S16 * x, S16 * y);
void Precoder_Mat_x_Vec_4_1_A(S32 Map_symb, S16 w_dim1, S16 w_dim2, S16 * W_ptr, S16 * x, S16 * y);
void Precoder_Mat_x_Vec_4_2_A(S32 Map_symb, S16 w_dim1, S16 w_dim2, S16 * W_ptr, S16 * x, S16 * y);
void Precoder_Mat_x_Vec_4_3_A(S32 Map_symb, S16 w_dim1, S16 w_dim2, S16 * W_ptr, S16 * x, S16 * y);
void Precoder_Mat_x_Vec_4_4_A(S32 Map_symb, S16 w_dim1, S16 w_dim2, S16 * W_ptr, S16 * x, S16 * y);

////////////////////////////////////////////////////////////////////////////////////////

void rx_lte_rate_matching_hdwr(U16 nblk, U16 nfil, U32 E, U16 k0, U16 Ncb, S16 out[]);

U8 fec_decode_hdwr_sim(U16 CodingType, // Type of encoding or decoding (0=CC, 1=CTC)
                       U16 BlockSize, // Code block size
                       U16 CrcSelector, // Type of CRC
                       U16 FillerBits, // Number of initial filler bits in code block
                       U16 CodingRate, // Coding rate
                       U16 HalfIterations, // Number of decoder 1/2 iterations
                       U16 DynamicStopIteration, // CTC decoder automatic stop select
                       U16 BpDecoder, // Decoder bypass
                       U16 EnableCombine, // HARQ soft-combining enable
                       U32 RateMatchingE, // Rate matching E parameter
                       U16 RateMatchingk0, // Rate matching k0 parameter
                       U16 RateMatchingNcb,// Rate matching Ncb parameter
                       U16 Qm, // Number of modulation bits used (2,4,6)
                       U16 BpChanInt, // Bypass channel de-interleaver
                       U16 ColInt, // Number of channel interleaver columns (9, 10, 11 or 12)
                       S8 *DescAdr[], // Channel de-interleaver descriptor starting addresses
                       S32 DescLen[], // Channel de-interleaver descriptor byte counts
                       U16 DescN, // Channel de-interleaver number of descriptors
                       S8 In[], // Input buffer pointer
                       U8 Out[], // Output buffer pointer
                       U16 CodeBlockIndex);// Code block index for debug only

void Compute_Rate_Matching_Parameters(S16 nblk, S16 C, S16 NL, S32 G, S16 Qm, S16 r, S32 NIR,
                                      RMParameters *pRm) CEVAEXTERN;

S32 Compute_Rate_Matching_E(S16 C, S16 NL, S32 G, S16 Qm, S16 r) CEVAEXTERN;

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
// Downlink Tx Control (PDCCH, PHICH, PCFICH)
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void LteBsPhyDLControlProc(PBSDLINITMEMBUF pBsDlInitMemBuf,
                           PIOParams_DlControl pIOParamsDlControl,
                           S16 computeOffset,
                           S16 *StartCCECtl, S16 *NumCCECtl,
                           S16 *pScratchMem,
                           S16 *pBsTxAntPortOut[]) CEVAEXTERN;

PTCB phy_bs_tx_control_tcb(PTASKLIST pTskLst, S16* rs_grid[], U8* startPDCCH,
                           PBSDLINITMEMBUF pBsDlInitMemBuf, PIOParams_DlControl pDlCntrl_Pars,
                           U16 RefillFlag, PTCB pTaskListDependencyTo, PTCB pTcbRefill);

CEVAINLINE void phy_bs_tx_control_task(PTCB pTask);

/* For initializing all DL tables */
void LteBsPhyDLCommonInit ( PBSDLINITPARAM  pBsDlInitParam,
                            PBSDLINITMEMBUF pBsDlInitMemBuf );


/* Individual functions to run the control functions */
/* To compute offset to PCFICH buffer */
void computeOffsetToPCFICHBuffer ( S16 numCtrlSymbols, S16 numBsTxAntPort,
                                   S16 subFrameNum, S16 *pOffset);

void LteBsPhyPCFICHProc(S16 *pCFIMemBuf, S16 *pCfiREMapTable,
                        S16 numCtrlSymbols, S16 numBsTxAntPort,
                        S16 subFrameNum, S16 pcfichScale,
                        S16 computeOffset, S16 *pBsTxAntPortOut[]) CEVAEXTERN;

void computeOffsetToPHICHBuffer ( S16 numBsTxAntPort, S16 subFrameNum,
                                  S16 *pOffset);

void LteBsPhyPHICHProc( S16 numPHICHChannels, S16 *pPhichHarqBits,
                        S16 *pPhichSeqIndx, S16 *pPhichGrpNum,
                        S16 numPHICHGrp, S16 *pHISymBuf,
                        S16 *pHiREMapTable, S16 numBsTxAntPort,
                        S16 subFrameNum, S16 computeOffset,
                        S16 *pPhichScale, S16 *pBsTxAntPortOut[]) CEVAEXTERN;

void LteBsPhyPDCCHProc(S16 numPDCCHChannels, U8 *pPdcchBytes,
                       S16 *pNumCCE, S16 *pStartCCE,
                       S16 numCtrlSymbols, U32 transmissionMode,
                       S16 *pPdcchPermuteTable, S16 *pPdcchREMapTable,
                       S16 *pPdcchREGLen, S16 numBsTxAntPort,
                       S16 *pPdcchScale, S16 *pBsTxAntPortOut[]) CEVAEXTERN;

void computeOffsetToDLRefSeqBuffer ( S16 numBsTxAntPort, S16 slotNumber,
                                     S16 symbolNum, S16 numDlRb, S16 *pOffset);

void LteBsPhyDLRefSeqProc(S16 *pDlRefSeqSymBuffer, S16 cellId, S16 symbolNum,
                          S16 numBsTxAntPort, S16 slotNumber, S16 numDlRb,
                          S16 pilotScaleFactor, S16 computeOffset,
                          S16 *pBsTxAntPortOut[]) CEVAEXTERN;

U32 LtePhyControlSduEnc(PIOParams_DlControl pDlCntrl_Pars) CEVAEXTERN;
U32 bsTxDciChannelCoding(U8 in[], U8 out[], S32 nin, U16 scram, S32 E) CEVAEXTERN;
void CC_Encoder(U8 in[], U32 out[], S32 n, S32 nb, S32 nsb) CEVAEXTERN;
void DCI_CC_SubBlockInterleaver(U32 in[], U8 out[], S32 n, S32 nout, S32 E) CEVAEXTERN;


//----------------------------------------------------------------------------------------
//End of Downlink Tx Control (PDCCH, PHICH, PCFICH)
//----------------------------------------------------------------------------------------

////
U32 UpdateRxChanVars(PLTEPHYDESC pPhy);
U32 LteBsRxSduProcessing(PLTEPHYDESC pPhy);
U32 LteBsRxSduProcessingRT(U32 RxSduCtx, U32 startTime);
U32 LteBsRxSduPreProcessingRT(U32 RxSduCtx, PLTEPHYDESC pPhy);

U32 CreateDlTxTaskList(PLTEPHYDESC pPhy);

U32 UpdateDlControlTaskList(PLTEPHYDESC pPhy);
U32 UpdateDlTxTaskList_p1(PLTEPHYDESC pPhy);

U32 UpdateDlTxTaskListRT(PLTEPHYDESC pPhy);
U32 InsertPBCH(PLTEPHYDESC pPhy);
U32 UpdateDlControlTaskListRT(PLTEPHYDESC pPhy);
U32 UpdateDlTxTaskList_p1_RT(PLTEPHYDESC pPhy);

U32 UpdateDlBufSetupTaskList(PLTEPHYDESC pPhy);
U32 DLMemAlloc(PLTEPHYDESC pPhy);

U32 PhyBsTxPreExec(PLTEPHYDESC pPhy);

U32 PhyBsTxPostExec(PLTEPHYDESC pPhy);
PTASKLIST PhyBsTxGetNextTaskList(PLTEPHYDESC pPhy, U32 idx);

// PUSCH / PUCCH Lists
U32 CreateUlRxTaskList(PLTEPHYDESC pPhy);
U32 UpdateUlRxTaskList(PLTEPHYDESC pPhy);
U32 PhyBsRxPreExec(PLTEPHYDESC pPhy);
U32 PhyBsRxExeTaskList(PLTEPHYDESC pPhy);
U32 PhyBsRxPostExec(PLTEPHYDESC pPhy);
void setupFpPointers(PLTEPHYDESC pPhy);

// PRACH List
U32 PhyBsRxPrachPreExec(PLTEPHYDESC pPhy);
U32 PhyBsRxPrachExeTaskList(PLTEPHYDESC pPhy, U32 Num);
U32 PhyBsRxPrachPostExec(PLTEPHYDESC pPhy, U32 Num);

// MUX PUSCH FEC List
U32 PhyBsRxMuxControlExeTaskList(PLTEPHYDESC pPhy);

// SRS List
U32 UpdateUlRxSrsTaskList(PLTEPHYDESC pPhy);
U32 PhyBsRxSrsPreExec(PLTEPHYDESC pPhy);
U32 PhyBsRxSRSExeTaskList(PLTEPHYDESC pPhy);
U32 PhyBsRxSrsPostExec(PLTEPHYDESC pPhy);


// Phy context api
void setPhyCtx(U32 phyInstance, U32 sfCxtNum, PLTEPHYDESC pPhy);
U8 getPhyCtxListIndx(U32 phyInstance);
PLTEPHYDESC getPhyCtxApiProc(U32 phyInstance);
PLTEPHYDESC getPhyCtxListProc(U32 phyInstance);
PLTEPHYDESC getPhyCtxByNum(U32 phyInstance, U32 sfCxtNum);

void swapPhyCtx(U32 phyInstance);


void LTE_Chan_Deint_Gen(S32 data_start, S32 data_end, S16 CP, S16 Qm, S32 ncol, S32 ndat, S32 ncqi,
                        S32 nri, S8 *inp, S8 *outadr[], S32 outlen[], S32 *nout) CEVAEXTERN;

void div_quo_rem(S32 x, S32 y, S32 yscf, S32 *q, S32 *r) CEVAEXTERN;

void fec_hdwr_chan_deint_sim(U16 ColInt, // Number of channel interleaver columns (12 or 14)
                             U16 Qm, // Number of modulation bits used (2,4,6)
                             U32 E, // Rate matching E parameter
                             S8 *DescAdr[], // Channel de-interleaver descriptor starting addresses
                             S32 DescLen[], // Channel de-interleaver descriptor byte counts
                             U16 DescN); // Channel de-interleaver number of descriptors

void PatchCRC16(U8 crc[], S32 num_bits);
U8* Phy2MacIfAllocBuffer(U32 size);
U8* Phy2MacIfAllocNcNbBuffer(U32 size);
U8* Phy2MacIfApiBuffer(U32 size);

U32 LteStart(U32 phyInstance, PSTARTREQ pParam, U32 logIqEnable);
U32 LteStop(U32 phyInstance);


void Num_Cont_Syms_PUSCH(PULRXFECPARAMS pFecParams, S16 nc, S16 nsym, S16* BlkSizTab) CEVAEXTERN;

void LteGenerateRxSduIndMsg(void *Context, U32* outMsg, U32* outMsgSize, U32 status, U32 chanId,
                            PULRXINPUTPARAMS pUlRxInputParams, PULRXFECPARAMS pRxFecParams,
                            U32 ctxState);

void LteGenerateRxStatusCqiRiHi(void *Context, U32 phyInstance, U32* outMsg,
                                U32* outMsgSize, PULRXFECPARAMS pCh, U32 chanId, U8 sf, U32 ctxState);
void LteGenerateRxStatusHi(void *Context, U32 phyInstance, U32* outMsg,
                           U32* outMsgSize, PULRXFECPARAMS pCh, U32 chanId, U8 sf, U32 ctxState);
void LteGenerateRxStatusCqiRi(void *Context, U32 phyInstance, U32* outMsg,
                              U32* outMsgSize, PULRXFECPARAMS pCh, U32 chanId, U8 sf, U32 ctxState);

void LteGenerateRxPrachStatusIndMsg(PLTEPHYDESC pPhy, U32* outMsg, U32* outMsgSize, MS_DET* pDetEvent, U8 sf, U16 sfn);

void LteBsRxGenerateRipIndMsg(U16 *RipPerRB_dB, U32 numRbs, U8 sf, U16 sfn, U32 *outMsg, U32 *outMsgSize);
void LteBsRxGenerateThermalNoiseIndMsg(S16 ThermalNoisePwr, U16 numRbs, U8 sf, U16 sfn, U32 *outMsg, U32 *outMsgSize);

void LteGenerateRxSrsStatusIndMsg(PLTEPHYDESC pPhy, U32* outMsg, U32* outMsgSize, U8 sf, U16 sfn);

S32 Detect_Reed_Muller(S8 insft[], U8 indat[], S16 ninsft, S16 nindat, U32 enctab[]);


#if !defined(_WIN32) && !defined(CEVA_INTRINSICS)

U8* LteRxSduBuildRxEndMessage(U32 frameNum, U32 subframeNum);

void SendRxSduList(U32 listType, U32 Last, U32 rxSduCtx, U8* pRxEnd);

void LteRxSduAdd2List(void *Context, U32 status, U32 listType, U32 rxSduCtx, void *listElement);

void LteRxStatusCqiRiHiAdd2List(void *Context, U32 typeCqiRiHi, U32 listType, U32 rxSduCtx, void *listElement);

void LteDelayedApiIndAdd2List(U32 listType, U32 rxSduCtx, void *listElement);

void SNRDone(TCB* pTCB, void* param);
void RipRssiMeasDone(TCB* pTCB, void* param);


MXRC LtePhyMessagePush(PMAC2PHY_QUEUE_EL ListHeader, UINT32 fifoBufferType, UINT32 fifoCleanType, UINT32 msgLen);
LPVOID LtePhyMessagePop(UINT32 fifoBufferType, PUINT32 pFifoCleanType, PUINT32 pMsgLen);
LPVOID LtePhyMessagePopSilent(UINT32 fifoBufferType, PUINT32 pFifoCleanType, PUINT32 pMsgLen);
#endif

#if defined (EVM) && defined (CEVA_INTRINSICS)

void InitSubTaskLogger(void* pTimer);
void StartSubTask(U32 taskId);
void StopSubTask(U32 taskId);
void StopAndStartSubTask(U32 taskIdStop, U32 taskIdStart);
void SetSubTaskLoggerCount(U32 taskIdStart, U32 taskIdStop);
void FinalizeSubtaskLogger(PTCB pTcb);

#else

#define InitSubTaskLogger(a)
#define StartSubTask(a)
#define StopSubTask(a)
#define StopAndStartSubTask(a, b)
#define SetSubTaskLoggerCount(a, b)
#define FinalizeSubtaskLogger(a)

#endif

S32 LtePhyRxSduAverageUlCqi(U8 cqi, U32 isPusch, PUSED_INDEX_VARS pUsedIndexVars, U32 CurrTti,
                            U32 Multi_PUCCH_PerRB) CEVAEXTERN;
S16 LtePhyRxSduAverageUlTa(S16 currTa, U8 currCqi, U32 numRbs,
                           PUSED_INDEX_VARS pUsedIndexVars) CEVAEXTERN;

void addObj(U8* start, U32 size, U32 num);

#if !defined(EVM)

#define GetBusTick()        0
#define GetExecBusTicks(a)  0

#endif

CEVAINLINE U32 LteBsStartTtiProcessing(U32 LteBsApiErrorCheckCtxLocal, U32 BsTxSduDesc) CEVAEXTERN;
CEVAINLINE U32 LteBsApiErrorCheck(U32 LteBsApiErrorCheckCtxLocal) CEVAEXTERN;
CEVAINLINE U32 LteBsTxSduProcessingRT(U32 pSduDescIn) CEVAEXTERN;
CEVAINLINE void AddPdcchInpBuf(PDLTXINPUTPARAMS pTxInputParams, PIOParams_DlControl pDlCntrl_TxInPars, PTXSDUREQ *pdcchSdu, U32 numPdcchSdu, U32 gUseTxSduPointer) CEVAEXTERN;
CEVAINLINE void AddDci0InpBuf(PIOParams_DlControl pDlCntrl_TxInPars, PDCIULSDUMSG *dciSdu, U32 numDciSdu) CEVAEXTERN;
CEVAINLINE U32 LtePhyHiDciParse(U32 LteBsApiErrorCheck, PIOParams_DlControl pDlCntrl_TxInPars, PIOParams_DlControl pDlCntrl_TxInPars_List, PDLTXINPUTPARAMS pTxInputParams, U32 gUseTxSduPointer) CEVAEXTERN;
CEVAINLINE U32 LteBsTxStartCRC_RT(U32 pSduDescIn) CEVAEXTERN;
CEVAINLINE U32 InsertPBCH_RT(U32 pSduDescIn) CEVAEXTERN;
CEVAINLINE U32 LteBsRxSduCevaPreProcessingRT(U32 ulRxInputParams, U32 ulFecParams, U32 ulShareParams, U32 ulHarq,
                                  U32 Table_C, U32 Table_K, U32 Table_k0) CEVAEXTERN;
CEVAINLINE U32 LteBsRxVectorProcessingRT(U32 ceva_rxvectorproc_params) CEVAEXTERN;
U32 bsTxBchChannelCoding(U8 in[], U8 out[], S32 nin, U16 scram, S32 E) CEVAEXTERN;


#ifndef CEVA_INTRINSICS
#define InitializeDriver(a)     0

#if defined(RT_API_PROC)
#define MdmaDrvAllocDesc(a, b)              MdmaAllocDesc(a, b)
#define MdmaDrvSet4GSCR(a, b, c, d, e)      MdmaSet4GSCR(a, b, c, d, e)
#define MdmaDrvSetData(a, b, c, d, e)       MdmaSetData(a, b, c, d, e)
#define FecUlDrvAllocDesc(a)                FecUlAllocDesc(a)
#define FecUlDrvSetHwCtx(a, b)              FecUlSetHwCtx(a, b)
#define FecUlDrvSetData(a, b, c, d, e)      FecUlSetDataOpt(a, b, c, d, e)
#define MdmaDrvSetDataEx(a, b, c, d, e)     MdmaSetDataEx(a, b, c, d, e, MDMA_CACHE_PROC_NONE)
#define MdmaDrvSetCRC(a, b, c, d)           MdmaSetCRC(a, b, c, d)
#define MdmaDrvSetCopy(a)                   MdmaSetCopy(a)
#define MdmaDrvSetLastDescArm(a, b, c)
#define MdmaDrvRun(a, b)
#define MdmaDrvRunQ(a, b, c)
#define ceva_uart_printf(...)               stop_printf(__VA_ARGS__)
#endif  // EVM

#else

#define MdmaRunEx(a, b, c, d)
#define MdmaRunExQ(a, b, c, d, e)
#define MdmaDrvSetDataEx(a, b, c, d, e)     MdmaDrvSetData(a, b, c, d, e)

#endif  // CEVA_INTRINSICS


#endif // _PROTOTYPE_H_
