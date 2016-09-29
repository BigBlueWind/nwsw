#ifndef _PROTOTYPE_H_
#define _PROTOTYPE_H_


#include "ltephy.h"
#include "Debug.h"
#include "tcb.h"

// For bring up of the UL RX Static Task List define here compile switch that enables the UL Static List
// functions
#ifndef _WIN32
#ifdef LIST_BASED
//#define UL_STATIC_LIST
//#define NON_BLOCKING_SCHEDULER
#endif
#endif

#ifdef _WIN32
#ifdef CEVA_INTRINSICS
	#define			myprintf(x)
#else
	#define         myprintf		printf
#endif

#define AUTO_OFF    0
#define AUTO_ON     1

//#define SetDepDiscovery(val)    gDepDiscovery = val
#endif

#ifndef _WIN32

int uart_printf(const char *, ...);

#else

#define uart_printf     printf
#endif


#ifndef SCHEDULER
	#define ExeTSchedList(x)
	#define CreateTaskList(x,y) ((PTASKLIST) 1)
	#define AddSuperTCB         AddSuperTCB_stub
	#define DestroyTaskList(a)
	#define extractTCBIOBuf(x,y) NULL
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

//*****************************************************************************


void * AddSuperTCB_stub(U16 GID, void* GroupHandle, void * Rsrv, U16 numTCB, ...);

//  gp_utils
S16 block_norm(S16 * data_in, S16 *data_out, S32 size, S16 headroom);
void block_shift(S16 *data_in, S16 size, S16 expo);

S16 block_lnorm(S32 * data, S32 size, S16 headroom);

short *svector( long nl, long nh );
void free_svector( short *v, long nl, long nh);
S16  **smatrix               ( S32, S32, S32, S32 );
S16  **smatrix0               ( S32, S32 );

void    free_smatrix         ( S16 **, S32, S32, S32, S32 );
void    free_smatrix0         ( S16 **);

void BPSK_TX_MAPPER( unsigned char *IN, short *OUT, short *pTABLE, short N );
void QPSK_TX_MAPPER( unsigned char *IN, short *OUT, short *pTABLE, short N, short N1 );
void QAM16_TX_MAPPER( unsigned char *IN, short *OUT, short *pTABLE, short N, short N1 );
void QAM64_TX_MAPPER( unsigned char *IN, short *OUT, short *pTABLE, short N );

void BPSK_TX_MAPPER_A(void*, void*, void*, long);
void QPSK_TX_MAPPER_A(void*, void*, void*, long, long);
void QAM16_TX_MAPPER_A(void*, void*, void*, long, long);
void QAM64_TX_MAPPER_A(void*, void*, void*, long);

// diag
void diagnostic_file_open(U32 numchan);
void diagnostic_file_close(U32 numChan);

S32 BsTxChannelCoder(U32 chanId, U32 cwId, U8 in[], S32 nin, U8 out[], S32 G, S16 rvidx, S16 NL, S16 Qm, S16 nRNTI, S16 channelType, S16 slotNumber, S16 Ncell_id, U32 numberofControlChannels,
                     U8* scrOutBufs[], U32 *scrIndex[], U32 _totalsizescr[]);

void bs_tx_broadcast_channel_coding(U8 in[], U8 out[], S32 nin, S16 nant, S32 E);
//void bs_tx_broadcast_channel_coding(U32 chanId, U8 in[], U8 out[], S32 nin, S16 nant, S32 E, U32 numberofControlChannels, U32 cinit_PBCH, U8* scrOutBufs[], U32 *scrIndex[], U32* _totalsizescr);


void Byte2BitStream(U8 bytestream[], U8 bitstream[], U16 length);

void BsTxCrcEncoder(U8 in_bitstream[], U8 out_bitstream[], S32 length);

void AddCodeBlockCRC24B(U8 in_bitstream[], U8 out_bitstream[], S16 length);

S16  BsTxCodeBlkSeg(U8 in_bitstream[], U8 **code_blocks, S16 KIndex[], S16 *FillerBits, S32 B, S16 code_length[], S16 *MM);

#ifdef _WIN32
    S32 BsTxRateMatch(S16 **in_bitstream, S32 G_num_bits_transport,
                      S16 rvidx, S16 num_FillerBits, S16 NL,
                      S16 Qm, S16 CBNum,S16 C_num_code_blocks,
                      S16 dlen, U8 out_bitstream[]);
#endif

PRateMatchParms Compute_Rate_match_in_parms(S16 inlen, S32 G_num_bits_transport, S16 rvidx, S16 NL, S16 Qm, S16 CBNum, S16 C_num_code_blocks);

//S16 LteDivideS(S16 num, S16 den, S16 type);
//S16 LteDivideL(S32 num, S32 den, S16 type);

U16 bitget32(U32 X, S16 b);

////////////////////////////////////////////////////////////////////////////////////////
void PseudoRandomSeqGen(S32 N, U32 cinit,U8 *c);
PTCB PseudoRandomSeqGen_tcb(PTASKLIST pTskLst, S32 length, U32 cinit, U32 cinit1, S32 stepsize, S16 numSym, U8 *c, PTCB pTaskListDependencyTo);
void PseudoRandomSeqGen_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void scrambler(U32 init, S8 bx[], S8 by[], S32 length_bx);
PTCB scrambler_tcb(PTASKLIST pTskLst, U32 cinit, S8 *input, S8 *output, S32 length_s, S32 length_s1, S16 modType, PTCB pTaskListDependencyTo);

void scrambler_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

void scrambler_byte(U32 cinit, S8 bx[], S8 by[], S32 length_bx);

////////////////////////////////////////////////////////////////////////////////////////
void descrambler(U32 init, S8 bx[], S8 by[], S32 length_bx);
PTCB descrambler_tcb(U32 init, S8 bx[], S8 by[], S32 length_bx, S16 modType, PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);
void descrambler_task(PTCB pTCB);

void bs_rx_descrambler(S8 in[], S8 out[], S32 nin, U32 init);

////////////////////////////////////////////////////////////////////////////////////////

void Mapper_fx(U8 b[], S16 Qm, S16  cIQ[], S32 length_b);

void bs_phy_config(S16 Modulation, S16 Profile, S16 CellGroup,
                   S16 CellSector, S16 Ntx_ant);

////////////////////////////////////////////////////////////////////////////////////////
void DLResourceElementMapper(S16 *a, S16 *y, S16 *scIndex, S16 length_y, S16 scale_factor);
PTCB DLResourceElementMapper_tcb(S16 *a, S16 *y, S16 *scIndex, S16 length_y, S16 scale_factor, S16 numrows_a, PTCB pTaskListDependencyTo);
void DLResourceElementMapper_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

void DLResourceElementAssignment_ARM(PDLRESOURCEPARAM InParam, RESELEMINFO *pResInfo, S16 isym, S16 physChanType, S16 *num_RE);

S16  IFFTModSym(PLTEPHYDESC pPhy, S16 a[], S16 iSymInSlot, S16 *s);

void BsTxOfdmModulator(S16 isym, PLTEPHYDESC pPhy, S16 a[], S16 P);

U32 PhyBsTx(PLTEPHYDESC pPhy);
U32 PhyBsTxPrepareTaskList(PLTEPHYDESC pPhy);
U32 PhyBsTxExeTaskList(PLTEPHYDESC pPhy);


void BsTxPhysicalSignals(PLTEPHYDESC pPhy, S16 a[], S16 bs_Ntx_antennas,
                         S16 slotNumber);

void BsTxPDSCH(S16 isym, PLTEPHYDESC pPhy,S16 a[]);

void BsTxPDCCHBkdComp(PLTEPHYDESC pPhy, S16 a[], S16 Ntx_ant, S16 numPDCCHSymbols, S16 slotNumber, S16 scaleFactor);


void BsTxPDCCH(PLTEPHYDESC pPhy,  S16 a[], S16 Ntx_ant, S16 numPDCCHSymbols, S16 slotNumber, S32 stepsize_a, S16 scaleFactor);

void BsTxPBCH(PLTEPHYDESC pPhy,  S16 bitsBcst, S16 a[], S16 Ntx_ant, S16 subframeNumber, S16 scaleFactor);

//void precoder(PRECODERPARAM InParam, S16 *x,S16 *y, S16 *k_l_row0, S16 *k_l_row1, S32 *length_y, S16 *numrows_k_l, S16 *numcols_k_l, S32 Mlayer_symb);




//void BsTxSCH(PLTEPHYDESC pPhy, S16 a[], S16 slotNumber);
//void BsTxPSCH(PLTEPHYDESC pPhy, S16 a[], S16 SyncTxAntennaNum);
//void BsTxSSCH(PLTEPHYDESC pPhy, S16 a[], S16 SyncTxAntennaNum, S16 slotNumber);



void fftcg_c_16t24d( short *xtime_real_img, int m, short *xfreq_real_img, short *Exponent2 );
PTCB ifft_tcb( short *xfreq_real_img, int m, short *xtime_real_img, short *Exponent2);

////////////////////////////////////////////////////////////////////////////////////////
void LteIDFT_16t32d (short *xn, unsigned short N, short *Xk, short *Exp2);
void IDFT(S16 *input, U16 idftSize, S16 *output, S16 *exponent);
PTCB IDFT_tcb(PTASKLIST pTskLst, S16 *input, S16 isym, U16 idftSize, S16 *output, S16 *exponent, PTCB pTaskListDependencyTo);
void IDFT_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

short my_fread( short signal[], short type, short length,  FILE *file);
short my_fwrite( short signal[], short type, short length,  FILE *file);

////////////////////////////////////////////////////////////////////////////////////////

void bs_rx_channel_coding(S32 nin, S32 nout, S8 in[], U8 out[], S16 *stat,
                          PHARQS pharq, S16 NL, S32 G, S16 Qm, S16 rvidx,
                          S32 NIR, U32 halfit, S32 ncqi, S32 nri, S32 Cmux, S16 CP);

PTCB bs_rx_channel_coding_tcb(S32 nin, S32 nout, S8 descr_in[], S8 in[], U8 out[], S16 *stat,
                          PHARQS pharq, S16 NL, S32 G, S16 Qm, S16 rvidx,
                          S32 NIR, U32 halfit, U32 init, PTCB pTaskListDependencyTo);

void bs_rx_channel_coding_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void FSCALE(short int IN[], short int OUT[], short int N, short int FRC);
void PACKBB(U8 IN[], U8 OUT[], S32 N, S16 POS);
void UNPACKBB(U8 IN[], U8 OUT[], S32 N, S16 POS);
void CLEAR(short int IN[], short int N);
void bitswap(S16 IN, U16 *IN_swp);

#ifdef _WIN32
    S32 BsTxRateMatch2(U8 in[], U8 null[], S16 inlen,
                       S32 G_num_bits_transport, S16 rvidx,
                       S16 NL, S16 Qm, S16 CBNum,
                       S16 C_num_code_blocks, U8 out[]);
#endif

S32  LTE_Data_Cont_Mux(U8 in1[], U8 in2[], U8 out[], S32 n1, S32 n2);

S32  LTE_Chan_Interleaver(U8 in1[], U8 in2[], U8 in3[], U8 out[], S32 n1,
                             S32 n2, S32 n3, S16 Qm, S32 Cmux, S16 CP);

void LTE_Data_Cont_Demux(S8 in[], S8 out1[], S8 out2[], S32 n1, S32 n2);

void LTE_Chan_Deinterleaver(S8 in[], S8 out1[], S8 out2[], S8 out3[],
                            S32 n1, S32 n2, S32 n3, S16 Qm, S32 Cmux, S16 CP);

void LTE_Chan_Deint_QPSK(U16 in[], U16 out1[], U16 out2[], U16 out3[],
                         S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP);

void LTE_Chan_Deint_16QAM(UN32 in[], UN32 out1[], UN32 out2[], UN32 out3[],
                         S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP);

void LTE_Chan_Deint_64QAM(U16 in[], U16 out1[], U16 out2[], U16 out3[],
                          S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP);

S32 tx_Mapper_Pack(U8 in[], U8 out[], S32 nin, S16 modt);

void c_mat_mat_mul_fx( S16 *xmat, S16 *ymat, short xdim1, short xdim2, short ydim1, short ydim2, S16 *out_mat);

void c_mat_vec_mul_fx( S16 *xmat, S16 *yvec, short xdim1, short xdim2, short ydim, S16 *out_vec);

void ceye_fx( S16 *xmat, short dim);

S16 get_angle_fx(S32 real_part, S32 imag_part);

S16 atan_fx(S16 val);

S16 saturate(S32 L_var1);

void  ComputeComplexConjMult(S16 adet[], S16 aPilots[], S16 chEst[], S16 length_krange);
void  ComputeComplexConjMult_A(S16 adet[], S16 aPilots[], S16 chEst[], S16 length_krange);
void noiseAveragingFrequency(S16 *temp_vec1, S16 *chEst,S16 Mpusch_sc, S16 expo_norm1);
void noiseAveragingFrequency_A(S16 *temp_vec1, S16 *chEst,S16 Mpusch_sc, S16 expo_norm1);
void Compute_phase_error_A(S16 chEst[], S32 Err[], S16 Mpusch_rb, S16 kre[], S16 length_kre);
void Compute_Derotation_PartOne(S16 Mpusch_sc,S16 noffset, S16 NegExpoNoffset, S16 temp2,S16 fk[], S16 derotation[]);
void Compute_Derotation_PartOne_A(S16 Mpusch_sc,S16 noffset, S16 NegExpoNoffset, S16 temp2,S16 fk[], S16 derotation[]);

 //BASE_STATION RX
////////////////////////////////////////////////////////////////////////////////////////
void rx_Multichan_DeModulation(S16 *RXIN, S16 *chEst_ul, S16 *chEst_pilot,
					S32 *RXOUT, S32 *CHOUT, S16 *tempVecChest, S16 *derotation,
                    S16 *expo_reEst, S16 *expo_ch, S16 expo_norm,
					S16 *reDet0, S16 *expo_reEst_norm, S16 *expo_reEst1,
					S16 NOUT, S16 BLKSIZ, S16 Nul_symb, S16 MO, S16 MI, S16 isym,
                    S16 K, S16 L, S32 length_b, S16 chanType);

PTCB rx_Multichan_DeModulation_tcb(PTASKLIST pTskLst, S16 *RXIN, U32 *rs_grid, S16 *chEst_ul, S16 *tempVecChest,
                    S16 *derotation, S16 *expo_reEst, S16 *expo_ch, S16 *expo_norm,
					S16 *reDet0, S16* expo_reEst_norm, S16 pilotSym, S16 NOUT, S16 BLKSIZ,
                    S16 Nul_symb, S16 MO, S16 MI, S16 isym, S16 K, S16 L, S16 chanType, S32 stepsize_aDet,
                    U32 *pSubTaskTimer,
					PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);

void rx_Multichan_DeModulation_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
void RX_MRCOM_C(short *RXIN, short *CHIN, long int RXOUT[], long int CHOUT[],
                short int NOUT, short int BLKSIZ, short int NANT, short isym);
PTCB RX_MRCOM_C_tcb(PTASKLIST pTskLst, short *RXIN, U32 *rs_grid, short *CHIN, long *RXOUT, long *CHOUT,
                    short NOUT, short BLKSIZ, short MO, short MI, short isym, short K, short L, PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);
void RX_MRCOM_C_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void rx_Demapper_fx(S16 aEst1_fx[], S16 Qm, S8 *bDetSoft, S16 NusedE, S16 chanType, S16 expo);
PTCB rx_Demapper_fx_tcb(PTASKLIST pTskLst, S16 reEst1[], S16 Qm, S16 *expo_reEst, S16 *expo_idft, S8 *bDetSoft, S16 NusedE, S16 chanType, PTCB pTaskListDependencyTo);
void rx_Demapper_fx_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

void Demod_BPSK(S16 RXIN[], S8 *bDetSoft, S32 iLength);
void Demod_QPSK(S16 RXIN[], S8 *bDetSoft, S32 iLength);
void Demod_QAM16(S16 RXIN[], S8 *bDetSoft, S32 iLength, S16 expo);
void Demod_QAM64(S16 RXIN[], S8 *bDetSoft, S32 iLength, S16 expo);

void DemuxSlot_fx(S16 *adc, S16 *mhcs, S16 *adcMhcs, S16 length);

////////////////////////////////////////////////////////////////////////////////////////
void GenHalfCyclicShift(S16 *mhcs, S16 expo, S16 length);
PTCB GenHalfCyclicShift_tcb(S16 *mhcs, S16 expo, S16 length, PTCB pTaskListDependencyTo, PTASKLIST pTaskListAdd);
void GenHalfCyclicShift_task(PTCB pTCB);
void Lte_GenHCS_CEVA(PTCB pTcb);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void Remove_HalfCyclicShift(S16 *adc, S16 *mhcs, S16 *adcMhcs, S16 length);
void Remove_HalfCyclicShift_A(S16 *adc, S16 *mhcs, S16 *adcMhcs, S16 length);
PTCB Remove_HalfCyclicShift_tcb(S16 *adc, S16 *mhcs, S16 *adcMhcs, S16 length, PTCB pTaskListDependencyTo, PTASKLIST pTaskListAdd);
void Remove_HalfCyclicShift_task(PTCB pTCB);
void Lte_RemoveHCS_CEVA(PTCB pTcb);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void SwapFFT_Out(S16 *input,S16 *output, S32 length, S32 offset, S16 *expo_mat);
PTCB SwapFFT_Out_tcb(S16 *input, S16 *output, S32 length, S32 offset, S16 Nfft, S16 *expo_mat, PTCB pTaskListDependencyTo, PTASKLIST pTaskListAdd);
void SwapFFT_Out_task(PTCB pTCB);
void Lte_SwapFFT_Out_CEVA(PTCB pTcb);
////////////////////////////////////////////////////////////////////////////////////////

void FFTDemodSym_fx(S16 *adc, S16 Nfft, S16 expo_Nfft,
                    S16 *adem, S16 *fft_exp);

////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_DemuxFDMAPhysChan_fx(S16 *RBAssignment, S16 Nsym, S16 Nrb_sc, S32 stepsize_aDet,
								S16 K, S16 L, S16 MO, U32 *rs_grid, S16 ChanType,
                                S16 *expo_mat, S16 *expo_mat_ch, S16 *expo_mrc, S16 *aDet);
PTCB bs_rx_DemuxFDMAPhysChan_fx_tcb(PTASKLIST pTskLst, S16 *RBAssignment, S16 Nsym, S16 Nrb_sc, S32 stepsize_aDet,
                                    S16 ROW, S16 K, S16 L, S16 MO, U32 *rs_grid, S16 NusedE, S16 COL, S16 Channel_Type,
                                    S16 *expo_mat, S16 *expo_mat_ch, S16 *expo_mrc, S16 *aDet,
                                    PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);
void bs_rx_DemuxFDMAPhysChan_fx_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_ChEstimator_part1_fx(S16 *adet, S16 *aPilots, S16 *temp_vec1, S16 *derotation,
                          PCHANESTPARAM pChanEstPar, S16 MI,
                          S16 *chEst,  S16 slot, S16 Msc, S16 Mrb,
                          S16 start_address);
void bs_rx_ChEstimator_part2_fx(S16 *temp_vec1, S16 pilotCarrierSymbol,
                          S16  Nul_symb,
                          S16 *chEstL, S16 *chEstU, S16 *expo_fac, S16 Msc);
void bs_rx_ChEstimator_part3_fx(S16 *adet,  S16 *derotation,  S16 Msc);

void bs_rx_ChEstimator_part4_fx(S16 *temp_vec1,
                          S16 *chEstIn, S16 *chEstOut, S16 Msc, S16 isym);




PTCB bs_rx_ChEstimator_part1_fx_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *aPilots, S16 *derotation,
                          PCHANESTPARAM pChanEstPar, S16 MI, S32 stepsize_aDet,
                          S16 *chEst,  S16 slot, S16 Msc, S16 Mrb, S16 K, S16 L,
                          S16 start_address,  PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);


PTCB bs_rx_ChEstimator_part2_fx_tcb(PTASKLIST pTskLst, S16 *tempVecChEst, S16 pilotCarrierSymbol,
                          S16 Nul_symb,   S16 *chEstL, S16 *chEstU, S16 *expo_fac,  S16 Msc,
                          PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend, S16 blocking);
PTCB bs_rx_ChEstimator_part3_fx_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *derotation,  S16 Msc, S16 K, S16 L, S32 stepsize_aDet,
                          PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);

PTCB bs_rx_ChEstimator_part4_fx_tcb(PTASKLIST pTskLst, S16 *tempVecChEst,
                           S16 *chEstIn, S16 *chEstOut, S16 Msc, S16 isym,
                          PTCB pTaskListDependencyTo);

void bs_rx_ChEstimator_part1_fx_task(PTCB pTCB);
void bs_rx_ChEstimator_part2_fx_task(PTCB pTCB);
void bs_rx_ChEstimator_part3_fx_task(PTCB pTCB);
void bs_rx_ChEstimator_part4_fx_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

void IdftDespreader_fx(S16 *z, S16 Mpusch_sc, S16 Nul_symb,
                            S16 pilotCarrierSymbol, S16 *expo_fac);


////////////////////////////////////////////////////////////////////////////////////////
void ul_Pilot_fx(S16 a[], S16 nPRS[], PULPILOTSPARAM ULPilotInput, S16 u[], S16 v[]);
PTCB ul_Pilot_fx_tcb(PTASKLIST pTskLst, S16 a[], S16 nPRS[], PULPILOTSPARAM ULPilotInput, U8 cvec_u[], U8 PRSbits[], S16 bitRevTable[], PTCB pTaskListDependencyTo);
void ul_Pilot_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

void ULPilotResourceElementAssignment_fx(S16 *RBAssignment, PULPILOTSPARAM ULPilotInput,
										S16 Msymb, S16 *k_l_ul_row0, S16 *k_l_ul_row1, S16 *numrows_k_l, S16 *numcols_k_l);

void UL_ResourceElementMapper_fx(S16 *a, S16 *y, S16 *k_l_ul_row0, S16 *k_l_ul_row1,
								 S16 numrows_k_l, S16 numcols_k_l, S16 numcols_a, S16 length_y);

void ul_ConfigPilots_Pusch(S16 Ncell_id, S16 delta_ss, S16 groupHopping, S16 Nul_symb, S16 Mpusch_sc, S16 Nrb_sc,
                           U8 *cvec_u, S16 *u, U8 *PRSbits, S16 *v, S16 *nPRS, S16 bitRevTable[]);

void phy_bs_rx(PLTEPHYDESC pPhy);

void ul_ConfigFrame_Chan(BS *s, BS_RX *ul, BS_RX_CHAN *ulChan);
void ul_ConfigFrame_Globals(BS *s, BS_RX *pBsRx, BS_SRS *ul_srs, BS_RX_PRACH *ul_prach);

S32 L_divide(S32 L_num, S32 L_denom);

S16  divide_tab_lookup( S16 X);
void  divide_tab_lookup2_A( S16 x1, S16 x2, S16 *pInv2 );

S32 ComputeUpperBound(U16 nFft);

////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_feq32b(S32 reEst1[], S16 reDet0[], S32 ChEst_pwr_fx[], S16 expo_reEst[],
                  S16* expo_reEst_norm, S16 Mpusch_sc);
PTCB bs_rx_feq32b_tcb(PTASKLIST pTskLst, S32 reEst1[], S16 reDet0[], S32 ChEst_pwr_fx[], S16* expo_reEst_norm, S16* Mpusch_sc, PTCB pTaskListDependencyTo);
void bs_rx_feq32b_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void rx_exponent_equalization_16bit(S16 *RXIN, S16 *CHIN, S32 length_b, S16 *expo_reEst, S16 *expo_ch, S16 Nul_symb,
                                    S16 expo_norm, S16 NOUT, S16 MO);
////////////////////////////////////////////////////////////////////////////////////////

void Compute_phase_error(S16 chEst[], S32 Err[], S16 Mpusch_rb, S16 kre[], S16 length_kre);

void Compute_Derotation(S32 Fs, S16 phaseOffset, S16 df, S16 Ndf, S16 Nfft, S16 Mpusch_sc, S16 NusedE, S16 derotation[],  S16 start_adress, S16 islot, S16 *Noffset, S16 *expoNoffset);

void AppendCRC16(U8 in_bitstream[], U8 out_bitstream[], S32 length);

void  TBCCENC_1_3(U8 in[], U8 out[], S32 n);

S32 CC_SubBlockInterleaver(U8 in[], U8 out[], U8 null[], S32 n);

void CC_BitSelection(U8 in[], U8 null[], U8 out[], S32 n, S32 E);

void STC(S16 d0[], S32 M0symb, S16 num_bs_Ntx_antennas, S16 *y);

void lte_encode_20_13(U8 in[2], U8 out[20], U16 n);

void lte_decode_20_13(S8 in[20], U16 *out);

void lte_encode_32_11(U8 in[2], U8 out[32], U16 n);

void lte_decode_32_11(S8 in[32], U16 *out);

void CC_Rx_Rate_Matching(S32 n, S32 E, U16 out[]);

void calc_gcrc8(U8 in[], U8 *out, S32 n);

void cc_decoder_1_3(S8 IN[], U8 OUT[], S32 N);

void bs_rx_cc_cont_chan_coding(S8 in[], U8 out[], S32 nin, S32 nout, U8 *stat);

void calc_gcrc16(U8 in[], U8 out[2], S32 n);

void FSCALEB(S8 in[], S8 out[], S32 n, S16 frc);


////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_ChEstimator_PUCCH(S16 *adet, U32 *rs_grid, S16 *aP_temp, S16 pilotCarrierSymbol[], S16 num_pilots,
							 S16 MI, S16 row_a, S16 col_a, S16 pucch_format,
							 PCHANESTPARAM pChanEstPar, S16 *chEst, S16 *expo_fac,
                             S16 slot, S16 Msc, S16 Mrb, S16 start_address);
PTCB bs_rx_ChEstimator_PUCCH_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *aP_temp, S16 pilotCarrierSymbol[], S16 num_pilots,
							 S16 MI, S16 row_a, S16 col_a, S16 pucch_format,
							 PCHANESTPARAM pChanEstPar, S16 *chEst, S16 *expo_fac,
                             S16 slot, S16 Msc, S16 Mrb, S16 start_address, S32 stepsize_aDet,
                             PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);
void bs_rx_ChEstimator_PUCCH_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

void ULPilotResourceElementAssignment_PUCCH(S16 *RBAssignment, PULPILOTSPARAM ULPilotInput,
										S16 Msymb, S16 *k_l_ul_row0, S16 *k_l_ul_row1, S16 *numrows_k_l, S16 *numcols_k_l);

////////////////////////////////////////////////////////////////////////////////////////
void ul_Pilot_PUCCH(S16 a[], S16 *k_l_ul_row0, S16 *k_l_ul_row1,
				 S16 *RBAssignment,S16 *r_alpha_uv, S16 *nOcBar, ULPILOTSPARAM* ULPilotInput);
PTCB ul_Pilot_PUCCH_tcb(PTASKLIST pTskLst, S16 a[], S16 *k_l_ul_row0, S16 *k_l_ul_row1,
				 S16 *RBAssignment,S16 *r_alpha_uv, S32 length_r_alpha_uv,
				  S16* nOcBar, ULPILOTSPARAM* ULPilotInput, PTCB pTaskListDependencyTo);
void ul_Pilot_PUCCH_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void gen_ul_Pilot_info(PULPILOTSPARAM pULPilot_Pucch, S16* r_alpha_uv, S16* nOc, S16* nOcBar, S16 *Sdescramb, S16 u[]);
PTCB gen_ul_Pilot_info_tcb(PTASKLIST pTskLst, PULPILOTSPARAM pULPilot_Pucch, S16* r_alpha_uv, S32 length_r_alpha_uv, S16* nOc, S16* nOcBar, S16 *Sdescramb, S16 u[],  PTCB pTaskListDependencyTo);
void gen_ul_Pilot_info_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void gen_ul_SequenceGroupNumber_U(S16 CellId, S16 groupHoppingEnabled, S16 u[]);
PTCB gen_ul_SequenceGroupNumber_U_tcb(PTASKLIST pTskLst, S16 CellId, S16 groupHoppingEnabled, S16 u[], PTCB pTaskListDependencyTo);
void gen_ul_SequenceGroupNumber_U_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

void SubBlockDeInterleaver_PDCCH(S16 in_bitstream[], S16 dlen, S16 Qm, S16 out_bitstream[]);

void PDCCHResourceElementAssignment(PLTEPHYDESC pPhy, S16 numPDCCHSymbols,
                                    S16 numPDCCHQuadruplets, S16 Ntx_ant,
                                    S16 *k_l_row0, S16 *k_l_row1,
                                    U16 *numrows_k_l, U16 *numcols_k_l,
                                    S16 *RG_Assigned, U16 numRePerSymb[],PCHANDESC pChan);

void PDCFIResourceElementAssignment(PLTEPHYDESC pPhy,S16 Ntx_ant, S16 *k_l_row0,
                                    S16 *k_l_row1, U16 *numrows_k_l, U16 *numcols_k_l, S16 *RG_Assigned);

////////////////////////////////////////////////////////////////////////////////////////
void DLResourceElementAssignment(PDLRESOURCEPARAM InParam, RESELEMINFO *pResInfo, S16 isym, S16 *num_RE, S16 *loc_RE);
////////////////////////////////////////////////////////////////////////////////////////

void BsTxPCFICH(PLTEPHYDESC pPhy,  S16 a[], S16 Ntx_ant, S16 slotNumber, S32 stepsize_a, S16 scaleFactor);

void TurboBest_CC_Decoder_1_3(signed char in[], unsigned char out[], long int nout,
                              short int sfsize, short int traceback);

void TurboBest_CC_Decoder_Init(void);

void TurboBest_CC_Decoder_Terminate(void);

PTCB fft_tcb( short *xtime_real_img, int m, short *xfreq_real_img, short *Exponent2, PTCB pTaskListDependencyTo, PTASKLIST pTaskListAdd);
PTCB fft_tcb_new(PTASKLIST pTskLst, PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);


void ResourceElementMapper_PS(S16 nDlRb, S16 nRbSc, S16 lIndex, S16 *d, S16 *a, S16 row_a, S16 col_a, S16 SyncTxAntennaNum, S16 scaleFactor);
void SymbolMapper(S16 isym, PLTEPHYDESC pPhy);
void SymbolLayerMapper(S16 isym, PLTEPHYDESC pPhy);
void SymbolPrecoder(S16 isym, PLTEPHYDESC pPhy);
void SymbolDLResourceElementMapper(S16 isym, S16 *a, PLTEPHYDESC pPhy);

void ResourceElementMapperPHICH(S16 *a, S16 *y, S16 *k_l_row0, S16 *k_l_row1, S16 numrows_k_l,
                            S16 numcols_k_l, S16 numcols_a, S32 length_y, S16 offset);
//void PHICHResourceElementAssignment(PBSTXPHICHPARAM PHICHInfo, S16 *RG_Assigned_Tx, S16 *RG_Assigned_PCFIH, S16 *k_l_row0, S16 *k_l_row1, S16 *numrows_k_l, S16 *numcols_k_l, S16 RG_BuffWrk[]);

void LTE_PN_Generation_PHICH(U32 Ncell_id, U32 Nsf_PHICH, S16 *ccvec, S16 ns);
void STC_type1(S16 d0[], S32 M0symb,  S16 *y);
void STC_type2(S16 d0[], S32 M0symb,  S16 *y);


//void BsTxPHICH(S16 numTxAntennas, S16 d0_IQ_wz[], S16 Nsf_PHICH, S16 N_PHICH_Grp, U32 number_of_phich_channels, S16 CPType, S16 m_c,
//               U32 stc_alloc_length, S16 Ndl_rb, S16 Nrb_sc, S16 N_PHICH_Grp_Prime, S16 scaleFactor[], S16 Ncell_id, S16 buffWrk[],
//               S16 *RG_Assigned_Tx, S16 *RG_Assigned_PCFIH, S16 *k_l_row0, S16 *k_l_row1, S16 *EffectiveNumPHICH, S16 y_tilde[],
//               S16 *n1out, S16 *ncol_y);
void BsTxPHICH(S16 numTxAntennas, S16 d0_IQ_wz[], S16 Nsf_PHICH, S16 N_PHICH_Grp, S16 *y, S16 *y_bar, S16 CPType, S16 m_c, U32 M0symb,
               U32 stc_alloc_length, S16 scaleFactor[], S16 buffWrk[], S16 *EffectiveNumPHICH, S16 y_tilde[]);



//PTCB BsTxPHICH_tcb(PBSTXPHICHPARAM PHICHInfo,   S16 d0_IQ_wz[],
//               S16 scaleFactor[], S16 *RG_Assigned_Tx, S16 *RG_Assigned_PCFIH,
//			   S16 *k_l_row0, S16 *k_l_row1, S16 *EffectiveNumPHICH, S16 y_tilde[], S16 *n1out, S16 *ncol_y, PTCB pTaskListDependencyTo);
void BsTxPHICH_task(PTCB pTCB);

void BsTxPHICHMod(PLTEPHYDESC pPhy, U32 ichan, S16 SequenceIndex, S16 cvec[], S16 d0_IQ_wz[]);
S16 Convert_dB_Linear(S16 Power_dB);
void SortMin(S16* array, S16 *array_ind, S16 num);

U32 LteDetermineCpsandSampling(PINITPARM pInit, PLTEPHYDESC pPhy);
void LteDetermineOneOverSamplingFrequency(PINITPARM pInit, BS *bs);
extern U32 LteSetupPersReMaps(PCHANDESC pChan, PDLCHANDESC ptr, U32 nRbSc,
                              U32 nSymb);
extern U32 LteSetupPersReMapsUl(PULCHDESC pChan, PULCHANDESC ptr, U32 nRbSc,
                              U32 nSymb);
extern U32 LteSetupPersReMapsDCI(PCHANDESC pChan, PDCICHANNELDESC ptr, U32 nRbSc,
                              U32 nSymb);
U32 LteDetRbandReforChannel(PCHANDESC pChan, U32 nRbSc, U32 nSymb,
                                           U32 mode);
U32 LteDetRbandReforChannelUl(PULCHDESC pChan, U32 nRbSc, U32 nSymb,
                                           U32 mode);

U32 LteUpdateBitMap(PCHANDESC pChan, U32 strtRb, U32 endRb);
U32 LteFindRbAllocMode(PCHANDESC pChan);
U32 LteValidateTbs(PCHANDESC pChan, U32 tBsize);
U32 LteReadTbsTable( U16 mcsIndex, U16 numRb);

void SchedulerInit(void);
//void SchedulerAddTask(void* pTcb);
void SchedulerLteRun(void);
void SchedulerRunLTETask(U16 taskNum);
int FPGADR_Proc(void* pTCBstruct,int dspnum);

void Lte_mapper_fx_CEVA(PTCB pTcb);
void Lte_demapper_fx_CEVA(PTCB pTcb);
void Lte_MrcDetector(PTCB pTcb);

void *CRAMAlloc(U32 size);
U32 CRAMFree(void *ptr);
void Lte_mapper_fx_CEVA(PTCB pTcb);
void Lte_demapper_fx_CEVA(PTCB pTcb);
void Lte_MrcDetector(PTCB pTcb);
void Lte_Precoder_CEVA(PTCB pTcb);
void Lte_LayerMap_CEVA(PTCB pTcb);
void Lte_DlPilot_CEVA(PTCB pTcb);
void Lte_RcRelmMap_CEVA(PTCB pTcb);
void Lte_RGConstellation_CEVA(PTCB pTcb);
void Lte_DemuxFDMAPhysChan_CEVA(PTCB pTcb);
void Lte_FEQ_CEVA(PTCB pTcb);
void Lte_FindMax_CEVA(PTCB pTcb);
void Lte_ExpEQ32_CEVA(PTCB pTcb);
void Lte_ExpEQ16_CEVA(PTCB pTcb);
void Lte_norm32to16_CEVA(PTCB pTcb);
void Lte_block_denorm_CEVA(PTCB pTcb);
void Lte_DemuxOFDMSym_CEVA(PTCB pTcb);
void Lte_ChanEst_CEVA(PTCB pTcb);
void Lte_ChanEst_PUCCH_CEVA(PTCB pTcb);
void Lte_UlPilots_CEVA(PTCB pTcb);
void Lte_CazacAvg_CEVA(PTCB pTcb);
void Lte_Rotate_CEVA(PTCB pTcb);
void Lte_getDetHard_CEVA(PTCB pTcb);
void LayerMapper_fx(TCB* pTCB);
void Precoder_fx(TCB* pTCB);
void DlPilot_fx(PTCB pTCB);
void ResourceElementMapper_fx(PTCB pTCB);


void LteDFT_16t32d (short *xn, unsigned short N, short *Xk, short *Exp2);
void LteIDFT_16t32d (short *xIn, unsigned short N, short *yOut, short *Exp2);

void  SubBlockInterleaver_PDCCH2(U8 in_bytes[], S16 dlen, S16 NilBytes, U8 out_bytes[],U8 out_bytesShadow[], U8 bufferWrk[]);

///////////////////////////////////////////////////////////////////////////////////////
void gen_ul_Pilot_srs_info(S16 Nrb_sc, S16 groupHoppingEnabled, S16 m_srs, S16 N, S16 nCs_srs, U16 *u, S16 *rSrs);
void bs_rx_Pilot_srs(S16 subframeNumber, S16 Nrb_sc, U32 Ncell_id, S16 groupHoppingEnabled, U16 *u, S16 *rSrs, S16 *m_srs, S16 *N, S16 *nCs_srs,
                     S16 *bandwidth, S16 numChan);
PTCB bs_rx_Pilot_srs_tcb(PTASKLIST pTskLst, S16 subframeNumber, S16 Nrb_sc, U32 Ncell_id, S16 groupHoppingEnabled, U16 *u, S16 *rSrs,
                         S16 *m_srs, S16 *N, S16 *nCs_srs, S16 *bandwidth, S16 numChan, S16 Nused, PTCB pTaskListDependencyTo[], S16 *DependArray,
                         S16 numDepend);
void rx_DemuxResourceElements(S16 *a, S16 *k_l_ul_row0, S16 *k_l_ul_row1, S16 length_k_l, S32 K,  S16 *aChan);
void ul_SrsResourceElementAllocation(S16 Nul_rb, S16 Nrb_sc, S16 *k_l_ul_row0, S16 *k_l_ul_row1, S16 *length_kl,
                                     S16 pilotSymbol, S16 bandwidth, S16 *m_srs_ptr, S16 *N_srs_ptr, S16 transmissionComb,
                                     S16 freqDomainPosition, S16 hoppingBandwidth);
void  bs_rx_ChEstimatorSrs(S16 iSig_Start, S16 numSig, S16 *cyclicShift, S16 *aEstSig,
                           S16 *rSrs, S16 df, U32 Fs, S16 Nul_symb, S16 Nrb_sc, S16 NusedE, S16 Nfft,
	                       S16 start_address, S16 islot, S16 length_aEstSig, S16 length_rSrs,
                           S16 *srsChEst, S16 *chEst, S16 *rawSrsChEst);
void SRSReceiverProcessing(S16 df, U32 Fs, S16 Nul_symb, S16 Nrb_sc, S16 NusedE, S16 Nfft, S16 *aDet, S16 numSrs, S16 mo, S16 *k_l_ul_row0, S16 *k_l_ul_row1,
                           S16 K, S16 L, S16 Nul_rb, S16 srsPilotSymbol, S16 *srsChEst, S16 *noisySrsChEst, S16 *rawSrsChEst, S16 *srsPilots,
                           S16 *length_srsPilots, S32 stepsize_aDet, S16 *cyclicShift, S16 *bandwidth, S16 *m_srs, S16 *N, S16 *nCs_srs, S16 *transmissionComb,
                           S16 *freqDomainPosition, S16 *hoppingBandwidth, S16 *cdmEnable, S16 *cdmGroupIndex);
PTCB SRSReceiverProcessing_tcb(PTASKLIST pTskLst, S16 df, U32 Fs, S16 Nul_symb, S16 Nrb_sc, S16 NusedE, S16 Nfft, S16 *aDet, S16 numSrs, S16 mo,
                           S16 *k_l_ul_row0, S16 *k_l_ul_row1, S16 K, S16 L, S16 Nul_rb, S16 srsPilotSymbol, S16 *srsChEst, S16 *noisySrsChEst,
                           S16 *rawSrsChEst, S16 *srsPilots, S16 *srsInBuf, S32 stepsize_aDet, S16 *m_srs, S16 *N, PTCB pTaskDependency);
void SRSReceiverProcessing_task(PTCB pTask);
void bs_rx_Pilot_srs_task(PTCB pTask);
///////////////////////////////////////////////////////////////////////////////////////

void SymbolCtlMapper(S16 isym, U8 *b0, U8 *b0_Shadow, U32 length_b0, U16 offset, U32 TotalNsubc, PLTEPHYDESC pPhy);
void SymbolCtlSTC(S16 isym, PLTEPHYDESC pPhy);
void SymbolCtlDLResourceElementMapper(S16 isym, S16 *a, S16 stepsize_a, PLTEPHYDESC pPhy);

///////////////////////////////////////////////////////////////////////////////////////

void bs_rx_pucch_chan_coding(S8 in[], U8 out[], S16 format);
//void bs_rx_pucch_chan_coding(S8 in[], U8 out[], S16 format);
//PTCB bs_rx_pucch_chan_coding_tcb(S8 in[], U8 out[], S16 format, PTCB pTaskListDependencyTo);
//void bs_rx_pucch_chan_coding_task(PTCB pTCB);

///////////////////////////////////////////////////////////////////////////////////////

void* MemAlloc(U32 size, U32 memId, U32 InitFlag);
void MemFree(void* ptr, U32 memId);


void FirDownsample(S16 in[], S16 firLpfMid6[], S16 USR, S16 delay, S32 L, S16 Nf, S16 outd[], S16 *length_outd);
void ComputePrach_x_u (S16 u, S16 Nzc, S16 x_u[]);
void ComputePrach_IDFT_839(S16 k, S16 Nzc, S16 eVec[]);
void ComputePrachEvec( S16 start, S16 end, S16 phi, S16 K, S16 k0, S16 N, S16 *eVec);
void ul_PrachPreamble_Config(BS_RX_PRACH *ul_prach, BS_RX_PRACHDet *ulChan_prach, S16 *u, S16 *Cv, S16 *vLen, S16 *Ncs);
void phy_bs_rx_ConfigFrame(BS_RX_PRACH *ul_prach, BS_RX_PRACHDet *ul_prachDet);
void PRACHReceiverProcessing(BS *bs, BS_RX *bsRx, BS_RX_CHAN *bsRxChan, BS_RX_PRACH *bsRxPrach,
							 BS_RX_PRACHDet *bsRxChanPrach, S16 MO, S16 **adcbufPrach, S32 length_adcPrach);
void mult_complex_vec(S16 Vec1[], S16 Vec2[], S16 VecOut[], S32 N);
void computeCorrelation(S16 Vec1[], S16 Vec2[], S16 VecOut[], U16 N);
void computePwr(S16 Vec[], S16 VecOut[], U16 N);
S16 ratioCompute(S16 num, S16 den, S16 Qfac);
void mac_complex_vec(S16 Vec1[], S16 Vec2[], S16 out[], U16 Nzc);
S16 computeAvgPwr(S16 xDetPwrProfile[], U16 N);
void PdschMCSIndexProcedures(PCHANDESC pChan, PBS_TX pBsTx);
void PbchMCSIndexProcedures(PCHANDESC pChan, PBS_TX pBsTx);
U32  PuschMCSIndexProcedures(PULSUBFRDESC ptr, S16 Nrb_sc, S16 num_sym, S16 ChanId);

void Pack_FEC_Output_Bits_to_Bytes2(PLTEPHYDESC pPhy, PCHANDESC pChan, U8 *scrOut1, U8 *scrOut2);
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

void UpdateProfiling(PTCB pTCB);
void UpdateCustomProfiling(U32 TaskID, U32 ExecTicks);
U32 GetTick(void);
U32 GetExecTicks(U32 start_tick);
U32 GetLocalTimerTick(void);
U32 GetLocalTimerExecTicks(U32 start_tick);

int CheckDataIntegrity(PLTEPHYDESC pPhy, U32 numChan, S16 col_a);


void ACK_RI_Decoder(S8 in[], U8 *out, S16 Qm, S16 nbits);

void SFDECCOMB(S8 in[], S8 out[], S32 nin, S32 nout);

void bs_rx_pusch_chan_coding(S8 insft[],
                             U8 outdat[], U8 outcqi[], U8 outri[], U8 outacq[],
                             S32 nindat, S32 nincqi, S32 ninri, S32 ninacq,
                             S32 noutdat, S32 noutcqi, S16 noutri, S16 noutacq,
                             S32 Cmux, S16 CP,
                             S16 *stat, PHARQS pharq, S16 NL, S32 G, S16 Qm,
                             S16 rvidx, S32 NIR, U32 halfit);

void ACK_RI_Encoder(U8 in[], U8 out[], S16 Qm, S16 nin);

void bs_tx_dci_channel_coding(U8 in[], U8 out[], S32 nin, U16 scram, S32 E);

void lte_code_blk_seg(S32 B, S16 *C, S16 *Cp, S16 *Cm, S16 *Kp, S16 *Km, S16 *F);

void ComputeCRC24(U8 in[], S32 n, S32 genmsk, U8 crc[]);

void CTC_Encoder_Rate_Matching(U8 in[], U8 out[], S16 nin, S16 nfil, S16 crcsel,
                               S32 E, S16 k0, S16 Ncb, S16 Cblk);

S8 Format1Detection(S16 reDet0[], S16 Msc);

///////////////////////////////////////////////////////////////////////////////////////
void CazacAvg(S16* reDet0, S16 *r_alpha_uv, S16 Msc, S16 isym, S16 slotNum, S16 NumUlSymEffPerSlot, S16 *Sdescramb, S16 pucchFormat, S16 *nOc);
PTCB CazacAvg_tcb(PTASKLIST pTskLst, S16* reDet0, S16 *r_alpha_uv, S16 Msc, S16 isym, S16 slotNum, S16 NumUlSymEffPerSlot, S16 *Sdescramb, S16 pucchFormat, S16 *nOc, PTCB pTaskListDependencyTo);
void CazacAvg_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void Rotate(S16* reDet, S16 Msc, S16 coeff);
PTCB Rotate_tcb(PTASKLIST pTskLst, S16* reDet, S16 Msc, S16 coeff, PTCB pTaskListDependencyTo);
void Rotate_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void updateBits(S16 *reDet0, S8* bUserDetSoft, S8* by, S16 Channel_Format, S16 Msc, S32 length_bUserDetHard);
PTCB updateBits_tcb(PTASKLIST pTskLst, S16 *reDet0, S8* bUserDetSoft, S8* by, S16 Channel_Format, S16 Msc, S32 length_bUserDetHard, PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);
void updateBits_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void getDetHard(S8* bSoft, S8* bHard, S8* by, S32 size);
PTCB getDetHard_tcb(S8* bSoft, S8* bHard, S8* by, S32 size,PTCB pTaskListDependencyTo);
void getDetHard_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
S16 norm32to16(S32 *data, S16 *out, S32 size, S16 headroom);
PTCB norm32to16_tcb(PTASKLIST pTskLst, S32 *data, S16 *out, S32 size, S16 headroom, S16* shft, PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend);
void norm32to16_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void block_denorm(S16 * data, S16 size, S16 scnt);
PTCB block_denorm_tcb(PTASKLIST pTskLst, S16* data, S16 size, S16* expo_mrc, S16* expo_norm_reEst,  PTCB pTaskListDependencyTo);
void block_denorm_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
void CopyData(S16 *input, S16 *output, S32 length);
PTCB CopyData_tcb(S16 *input, S16 *output, S32 length, PTCB pTaskListDependencyTo);
void CopyData_task(PTCB pTCB);
////////////////////////////////////////////////////////////////////////////////////////
#if !defined(_WIN32 ) && !defined(USE_LINUX) && !defined(CEVA_INTRINSICS)

#define FEC_FDESC_ENABLED
#define MDMA_FDESC_ENABLED

#include "FECdrv.h"
#include "mdmadrv.h"

#endif

#ifdef FEC_FDESC_ENABLED

#define _BuildFecTcb_TX(_pFecDL, _CodingType, _BlockSize, _CrcSelector,\
                 _FillerBits,            \
                 _Modulation,            \
                 _CodingRate,            \
                 _HalfIterations,        \
                 _DynamicStopIteration,  \
                 _ScrambleEnable,        \
                 _ScrambleMask,          \
                 _SubpacketId,           \
                 _BpBlockInterleaving,   \
                 _BpEncoder,             \
                 _BpDecoder,             \
                 _EnableCombine,         \
                 _BpRateMatching,        \
                 _RateMatchingE,         \
                 _RateMatchingk0,        \
                 _RateMatchingNcb,       \
                 _BpChannelInterleaving, \
                 _CiSymbolNumber,        \
                 _In,                    \
                 _InLen,                 \
                 _Out,                   \
                 _OutLen)                \
{                                       \
    UINT32 * p, x1, x2, x3;                 \
    (_pFecDL) = FecDlAllocOneDesc();         \
                                        \
    _ASSERT_PTR ((_pFecDL));               \
                                            \
    p = (PUINT32)&(_pFecDL)->HwContext;        \
                                            \
    x1 = (1 << 4);                          \
                                            \
    x2 = (_CodingType)                   |   \
         ((_CrcSelector) << 1)             |   \
         (((_RateMatchingk0) >> 1) << 4)   |   \
         ((_BlockSize) << 18);                 \
                                            \
    x3 = (((_Modulation) >> 1) << 2)   |       \
         ((_BpEncoder) << 6)           |       \
         ((_BpBlockInterleaving) << 7)|        \
         ((_CodingRate) << 8)          |       \
         ((_BpRateMatching) << 14)     |       \
         ((_FillerBits) << 18);                \
                                            \
    if ((_CodingType) == 0)                    \
    {                                       \
        x3 |= (((_ScrambleEnable) & 1) << 13) | ((((_ScrambleMask) >> 14) & 3) << 11);    \
        x2 |= ((_ScrambleMask) & 0x3FFF) << 4;                                         \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        (_pFecDL)->IoBdesc[1].BCtrl |= ((_RateMatchingNcb) & 0x1FFF) << 18;               \
        x3  |= (((_RateMatchingNcb) >> 13) & 3) << 4;                                  \
    }                                                                               \
                                                                                    \
    if ((_BpRateMatching))                                                             \
    {                                                                               \
        (_pFecDL)->IoBdesc[1].BCtrl |= (((_pFecDL)->IoBdesc[0].BCtrl & 0xFFFF) << 3) << 18;   \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        x1 |= ((_RateMatchingE) & 0x1FFFF) << 15;                                      \
    }                                                                               \
    FecDlSetData ((_pFecDL), (_In), (_InLen), (_Out), (_OutLen));                                \
    p[0] = x1;                                              \
    p[1] = x2;                                              \
    p[2] = x3;                                              \
}

PFEC_DL_FDESC BuildFecTcb_TX(
                 U16 CodingType,                // Type of encoding or decoding (0=CC, 1=CTC)
                 U16 BlockSize,                 // Code block size
                 U16 CrcSelector,               // Type of CRC
                 U16 FillerBits,                // Number of initial filler bits in code block
                 U16 Modulation,                // Number of modulation bits used (1,2,4,6)
                 U16 CodingRate,                // Coding rate
                 U16 HalfIterations,            // Number of decoder 1/2 iterations
                 U16 DynamicStopIteration,      // CTC decoder automatic stop select
                 U16 ScrambleEnable,            // Scrambling enable
                 U16 ScrambleMask,              // Scrambling mask
                 U16 SubpacketId,               // Sub-packet ID for the itial transmission
                 U16 BpBlockInterleaving,       // Block interleaver bypass
                 U16 BpEncoder,                 // Encoder bypass
                 U16 BpDecoder,                 // Decoder bypass
                 U16 EnableCombine,             // HARQ soft-combining enable
                 U16 BpRateMatching,            // Rate matching bypass
                 U32 RateMatchingE,             // Rate matching E parameter
                 U16 RateMatchingk0,            // Rate matching k0 parameter
                 U16 RateMatchingNcb,           // Rate matching Ncb parameter
                 U16 BpChannelInterleaving,     // Channel interleaver bypass
                 U16 CiSymbolNumber,            // Number of channel interleaver columns (12 or 14)
                 U8  In[],                      // Input buffer pointer
                 U32 InLen,                     // Input buffer length
                 U8  Out[],                     // Output buffer pointer
                 U32 OutLen,                    // Output buffer length
                 U32 pDlCbCtx,                  // Pointer to callback context
                 U32 FecDlCbDone);              // Pointer to callback function

PFEC_UL_FDESC BuildFecTcb_RX(
                 U16 CodingType,                // Type of encoding or decoding (0=CC, 1=CTC)
                 U16 BlockSize,                 // Code block size
                 U16 CrcSelector,               // Type of CRC
                 U16 FillerBits,                // Number of initial filler bits in code block
                 U16 Modulation,                // Number of modulation bits used (1,2,4,6)
                 U16 CodingRate,                // Coding rate
                 U16 HalfIterations,            // Number of decoder 1/2 iterations
                 U16 DynamicStopIteration,      // CTC decoder automatic stop select
                 U16 ScrambleEnable,            // Scrambling enable
                 U16 ScrambleMask,              // Scrambling mask
                 U16 SubpacketId,               // Sub-packet ID for the itial transmission
                 U16 BpBlockInterleaving,       // Block interleaver bypass
                 U16 BpEncoder,                 // Encoder bypass
                 U16 BpDecoder,                 // Decoder bypass
                 U16 EnableCombine,             // HARQ soft-combining enable
                 U16 BpRateMatching,            // Rate matching bypass
                 U32 RateMatchingE,             // Rate matching E parameter
                 U16 RateMatchingk0,            // Rate matching k0 parameter
                 U16 RateMatchingNcb,           // Rate matching Ncb parameter
                 U16 BpChannelInterleaving,     // Channel interleaver bypass
                 U16 CiSymbolNumber,            // Number of channel interleaver columns (12 or 14)
                 U8  In[],                      // Input buffer pointer
                 U32 InLen,                     // Input buffer length
                 U8  Out[],                     // Output buffer pointer
                 U32 OutLen,                    // Output buffer length
                 S8** DescAdr,                  // Channel de-interleaver descriptor starting addresses
                 U32* DescLen,                  // Channel de-interleaver descriptor byte counts
                 U32 DescN,                     // Channel de-interleaver number of descriptors
                 U32 pDlCbCtx,                  // Pointer to callback context
                 U32 FecDlCbDone);              // Pointer to callback function

#endif

PTCB BuildFecTcb(U16 CodingType,                // Type of encoding or decoding (0=CC, 1=CTC)
                 U16 BlockSize,                 // Code block size
                 U16 CrcSelector,               // Type of CRC
                 U16 FillerBits,                // Number of initial filler bits in code block
                 U16 Modulation,                // Number of modulation bits used (1,2,4,6)
                 U16 CodingRate,                // Coding rate
                 U16 HalfIterations,            // Number of decoder 1/2 iterations
                 U16 DynamicStopIteration,      // CTC decoder automatic stop select
                 U16 ScrambleEnable,            // Scrambling enable
                 U16 ScrambleMask,              // Scrambling mask
                 U16 SubpacketId,               // Sub-packet ID for the itial transmission
                 U16 BpBlockInterleaving,       // Block interleaver bypass
                 U16 BpEncoder,                 // Encoder bypass
                 U16 BpDecoder,                 // Decoder bypass
                 U16 EnableCombine,             // HARQ soft-combining enable
                 U16 BpRateMatching,            // Rate matching bypass
                 UN32 RateMatchingE,            // Rate matching E parameter
                 U16 RateMatchingk0,            // Rate matching k0 parameter
                 U16 RateMatchingNcb,           // Rate matching Ncb parameter
                 U16 BpChannelInterleaving,     // Channel interleaver bypass
                 U16 CiSymbolNumber,            // Number of channel interleaver columns (12 or 14)
                 U8  In[],                      // Input buffer pointer
                 UN32 InLen,                    // Input buffer length
                 U8  Out[],                     // Output buffer pointer
                 UN32 OutLen,                   // Output buffer length
                 U16 Tx_Rx);                    // Tx or Rx select (0=Rx, 1=Tx)

void RepackBytes(U8** pOut, U8* tc_encout, S16* reminder, S32 nBits, S32 noutbytes);

//----------------------------------------------------------------------------------------
// Debug Print Functions
//----------------------------------------------------------------------------------------
void filePrint(char *fileName, void *inputArray, int size, char *type, char *mode);
PTCB filePrint_tcb(PTASKLIST pTskLst, char *fileName, void *inputArray, int size, char *type, char *mode,
				   PTCB pTaskListDependencyTo);
void filePrint_task(PTCB pTCB);

//----------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// DownLink Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////
//void InitTX_Subfr(PLTEPHYDESC pPhy, S16* a);
//PTCB InitTX_Subfr_tcb(PLTEPHYDESC pPhy, S16* a, PTCB pTaskListDependencyTo);
//void InitTX_Subfr_task(PTCB pTask);

////////////////////////////////////////////////////////////////////////////////////////

void DlPilot(S16 Ncell_id, S16 CPType, S16 Ndl_rb, S16 Ndl_symb, S16 Nrb_sc, S16 Ntx_antennas,
            S16 pilotScaleFactor, S16 isym, S16 slotNumber, S16* a[]);
PTCB DlPilot_tcb(PTASKLIST pTskLst, S16 Ncell_id, S16 CPType, S16 Ndl_rb, S16 Ndl_symb, S16 Nrb_sc,
                 S16 Ntx_antennas, S16 pilotScaleFactor, S32 stepsize_a, S16 isym, S16 slotNumber, S16* a[],
				 PTCB pTaskListDependencyTo);
void DlPilot_task(PTCB pTask);

////////////////////////////////////////////////////////////////////////////////////////

void BsTxSyncCH(S16 cellSector, S16 Ndl_rb, S16 Nrb_sc, S16 Ndl_symb, S16 scaleP, S16 scaleS,
                S16 SyncTxAntennaNum, S16 slotNumber, S16 isym, S16 *a[], S16 d0[], S16 d1[]);

PTCB BsTxSyncCH_tcb(PTASKLIST pTskLst, S16 cellSector, S16 Ndl_rb, S16 Nrb_sc, S16 Ndl_symb, S16 scaleP, S16 scaleS,
                S16 SyncTxAntennaNum, S16 slotNumber, S32 stepsize_a, S16 isym, S16 *a[], S16 d0[], S16 d1[], S16 Ntx_antennas,
                PTCB pTaskListDependencyTo);


void BsTxSyncCH_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void SetupSymbDataInpBufs(
		U32 isymstart, U32 isymstop, U32 ichanStart, U32 numChan, U16 Ntx_antennas, S16 RBIndexMid6Lo, S16 RBIndexMid6Hi,
		PCHANDESC pChan1, U32 *index, RESELEMINFO *pResInfo, PTXSYSTEMINFO *pTxSysIn);
/*PTCB SetupSymbDataInpBufs_tcb(PTASKLIST pTskLst,
		U32 isymstart, U32 isymstop, U32 ichanStart, U32 numChan, U16 Ntx_antennas, S16 RBIndexMid6Lo, S16 RBIndexMid6Hi,
		PCHANDESC pChan1, U32 *index, RESELEMINFO *pResInfo, PTXSYSTEMINFO *pTxSysIn,
		PTCB pTaskListDependencyTo);
void SetupSymbDataInpBufs_task(PTCB pTask);*/

////////////////////////////////////////////////////////////////////////////////////////

void LoadSymbol(S16* pa1, S16* pa, U16 Ntx_antennas, S16 row_a, S16 col_a);
PTCB LoadSymbol_tcb(S16* pa1, S16* pa, U16 Ntx_antennas, S16 row_a, S16 col_a, S16 isym,
					PTCB pTaskListDependencyTo);
void LoadSymbol_task(PTCB pTask);

////////////////////////////////////////////////////////////////////////////////////////

void tx_Mapper_MultiChan(TXSYSTEMINFO *pTxSys, TXMAPPERMULTICHANIN *pIn, TXMAPPERMULTICHANOUT *pOut);
PTCB tx_Mapper_MultiChan_tcb(TXSYSTEMINFO *pTxSys, TXMAPPERMULTICHANIN *pIn1, TXMAPPERMULTICHANOUT *pOut,
							      PTCB pTaskListDependencyTo);
void tx_mapper_multichan_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void LayerMapper_MultiChan(TXSYSTEMINFO *pTxSys, TXMAPPERMULTICHANOUT *pIn, TXLAYERMAPPERMULTICHANOUT *pOut);
PTCB LayerMapper_MultiChan_tcb(TXSYSTEMINFO *pTxSys, TXMAPPERMULTICHANOUT *pIn, TXLAYERMAPPERMULTICHANOUT *pOut,
							   PTCB pTaskListDependencyTo);
void LayerMapper_MultiChan_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void Precoder_MultiChan(TXSYSTEMINFO *pTxSys, TXLAYERMAPPERMULTICHANOUT *pIn, TXPRECODERMULTICHANOUT *pOut);
PTCB Precoder_MultiChan_tcb(TXSYSTEMINFO *pTxSys, TXLAYERMAPPERMULTICHANOUT *pIn, TXPRECODERMULTICHANOUT *pOut,
							PTCB pTaskListDependencyTo);
void Precoder_multichan_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void DLResourceElementMapper_MultiChan(TXSYSTEMINFO *pTxSys, TXPRECODERMULTICHANOUT *pIn,
                                       DLRESELEMMAPPERMULTICHANOUT *pOut);
PTCB DLResourceElementMapper_MultiChan_tcb(TXSYSTEMINFO *pTxSys, TXPRECODERMULTICHANOUT *pIn,
                                           DLRESELEMMAPPERMULTICHANOUT *pOut, U16 N, S16* pSigOut, U32 len,
										   PTCB pTaskListDependencyTo);
void DLResourceElementMapper_MultiChan_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

PTCB tx_MultiChan_Modulation_tcb(PTASKLIST pTskLst,
		PTXMODULATIONVARS pTxModVars, TXSYSTEMINFO *pTxSys, U32 *_totalsizescr, RESELEMINFO *reselmInfo,
		U32 *scrIndex[2], U8 *scrOut[2], S16 *pl_k_Isc, S16 *phyChanScale_factor,
		S32 *pBit, S16 *pncol_y, S16 sync[2][62],
		U32 *pSubTaskTimer,
		PTCB pTaskListDependencyTo);

void tx_MultiChan_Modulation(PTXMODULATIONVARS pVars, PTXMODULATIONPARAMS pIn, TXSYSTEMINFO *pTxSys, S16 *pSigOut[], S16 *syncP, S16 *syncS);

void tx_MultiChan_Modulation_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////


void SetupIFFT_Bufs(S16* ifft_in, S16* a, S16 row_a, S16 col_a, S16 isym,
                    S16 mi, S16 Nfft, S16 Ncpsam);
PTCB SetupIFFT_Bufs_tcb(PTASKLIST pTskLst, S16* ifft_in, S16* a, S16 row_a, S16 col_a, S16 isym,
                    S16 mi, S16 Nfft, S16 Ncpsam, PTCB pTaskListDependencyTo);
void SetupIFFT_Bufs_task(PTCB pTask);

////////////////////////////////////////////////////////////////////////////////////////


PTCB ifft_tcb_tx_new(PTASKLIST pTskList, PTCB pTaskListDependencyTo);

PTCB ifft_tcb_tx(PTASKLIST pTskLst, short *xfreq_real_img, int m, short isym, short *xtime_real_img,
              short *Exponent2, PTCB pTaskListDependencyTo);

void ifftcg_c_16t24d( short *xfreq_real_img, int m, short *xtime_real_img, short *Exponent2);
void ifftcg_c_16t24d_task(PTCB pTask);
void fftcg_c_16t24d_task(PTCB pTask);


////////////////////////////////////////////////////////////////////////////////////////

PTCB PAPR_RCF_tcb(PTASKLIST pTskLst,S16 PAPR_mode, S16 Nfft, S16* ifft_out, S16 papr_power_thrsh, S16* xbar,
				       PTCB pTaskListDependencyTo);
void PAPR_RCF_task(PTCB pTask);

////////////////////////////////////////////////////////////////////////////////////////

void AddCP(S16* ptr_dac_samp, S16* ifft_out, S16 Nfft, S16 Ncpsam);
PTCB AddCP_tcb(PTASKLIST pTskLst, S16* ptr_dac_samp, S16* ifft_out, S16 Nfft, S16 Ncpsam,
			PTCB pTaskListDependencyTo);

void AddCP_task(PTCB pTask);

////////////////////////////////////////////////////////////////////////////////////////

void tx_Mapper(unsigned char ctxdata[], unsigned long len, short N1, unsigned short modType, short MappingGainOut[]);
PTCB tx_Mapper_tcb(PTASKLIST pTskLst, U8 ctxdata[], U32 Nsubc, S16 skip, U16 offset, U32 TotalNsubc, U16 modType, S16 MappingGainOut[], PTCB pTaskListDependencyTo);

void tx_mapper_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void ResourceElementMapperSymb(S16 *a, S16 *y, S16 *k_l_row0, S16 *k_l_row1, S16 numrows_k_l,
                           S16 numcols_k_l, S16 numrows_a, S32 length_y, S16 scaleFactor);

PTCB ResourceElementMapper_tcb(PTASKLIST pTskLst, S16 *a, S16 *y, S16 *k_l_row0, S16 *k_l_row1,
                               S16 numrows_k_l, S16 numcols_k_l, S16 numcols_a,
                               S32 ncol_y, U16 numrows_a, S16 mi,
                               S16 scaleFactor, S16 maxSymbolTaken, PTCB pTaskListDependencyTo);
void ResourceElementMapper_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void LayerMapper(S16 d0[], S16 d1[], S32 M0symb, S32 M1symb, S16 V, S16 *x,
                 U8 transmode, S32 *num_row_x, S32 *num_col_x );
PTCB LayerMapper_tcb(S16 d0[], S16 d1[], S32 M0symb, S32 M1symb, S16 V, S16 *x,
                 U8 transmode, S32 *num_row_x, S32 *num_col_x, U32 alloc_length, U32 length_b0,
				 U32 length_b1, S16 Qm, PTCB pTaskListDependencyTo);
void LayerMap_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void precoder(PRECODERPARAM InParam, S16 *x, S16 *y, S32 *length_y, S32 Mlayer_symb);
PTCB precoder_tcb(PRECODERPARAM InParam, S16 *x,S16 *y, S32 *length_y,  S32 Mlayer_symb, U32 alloc_length, PTCB pTaskListDependencyTo);
void precoder_task(PTCB pTCB);

////////////////////////////////////////////////////////////////////////////////////////

void rx_lte_rate_matching_hdwr(U16 nblk, U16 nfil, U32 E, U16 k0, U16 Ncb, S16 out[]);

U8 fec_decode_hdwr_sim( U16 CodingType,     // Type of encoding or decoding (0=CC, 1=CTC)
                        U16 BlockSize,      // Code block size
                        U16 CrcSelector,    // Type of CRC
                        U16 FillerBits,     // Number of initial filler bits in code block
                        U16 CodingRate,     // Coding rate
                        U16 HalfIterations, // Number of decoder 1/2 iterations
                        U16 DynamicStopIteration, // CTC decoder automatic stop select
                        U16 BpDecoder,      // Decoder bypass
                        U16 EnableCombine,  // HARQ soft-combining enable
                        U32 RateMatchingE,  // Rate matching E parameter
                        U16 RateMatchingk0, // Rate matching k0 parameter
                        U16 RateMatchingNcb,// Rate matching Ncb parameter
                        U16 Qm,             // Number of modulation bits used (2,4,6)
                        U16 BpChanInt,      // Bypass channel de-interleaver
                        U16 ColInt,         // Number of channel interleaver columns (9, 10, 11 or 12)
                        S8  *DescAdr[],     // Channel de-interleaver descriptor starting addresses
                        S32 DescLen[],      // Channel de-interleaver descriptor byte counts
                        U16 DescN,          // Channel de-interleaver number of descriptors
                        S8  In[],           // Input buffer pointer
                        U8  Out[],          // Output buffer pointer
                        U16 CodeBlockIndex);// Code block index for debug only

void Compute_Rate_Matching_Parameters(S16 nblk, S16 C, S16 NL, S32 G, S16 Qm,
                                      S16 r, S32 NIR, RMParameters *pRm);

//----------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////

PTCB Clearbuf_tcb(PTASKLIST pTskLst, void *pIn, U32 size, PTCB pTaskListDependencyTo);

void Clearbuf_task(PTCB pTask);

///////////////////////////////////////////////////////////////////////////


#if !defined EVM
#define AddSubTask(a)
#endif

void phy_bs_tx_control(S16* rs_grid[], S16 numTxAntennas, S16 numPDCCHSymbols, S16 numcols_a, U32 number_of_phich_channels, S16 CPType, U32 Ncell_id,
                       S16 controlIndicatorScaleFactor, S16 N_PHICH_Grp, S16* PHICH_Bit, S16* PHICH_SequenceIndex, S16 sizePDCCH, U8* startPDCCH,
                       S16* Grp, S16 scalefactorPDCCH, S16 scalefactorPHICH, S16 pilotScaleFactor, S16 numGroups, S16 Ndl_rb, S16 Nrb_sc, S16 col_a, S16 row_a,
                       S16 slotNumber, S16 stepsize_a, S16 numCtrlSymbols, U32 transmode);

PTCB phy_bs_tx_control_tcb(PTASKLIST pTskLst, S16* rs_grid[], S16 numTxAntennas, S16 numPDCCHSymbols, S16 numcols_a, U32 number_of_phich_channels, S16 CPType, U32 Ncell_id,
                       S16 controlIndicatorScaleFactor, S16 N_PHICH_Grp, S16* PHICH_Bit, S16* PHICH_SequenceIndex, S16 sizePDCCH, U8* startPDCCH, S16* Grp,
                       S16 scalefactorPDCCH, S16 scalefactorPHICH, S16 pilotScaleFactor, S16 numGroups, S16 Ndl_rb, S16 Nrb_sc, S16 col_a, S16 row_a, S16 slotNumber, S16 stepsize_a,
                       U32 transmode, S16 numCtrlSymbols, PTCB pTaskListDependencyTo);

void phy_bs_tx_control_task(PTCB pTcb);

U32 LteBsRxSduProcessing(PLTEPHYDESC pPhy);


U32 CreateDlTxTaskList(PLTEPHYDESC pPhy);
U32 UpdateDlTxTaskList(PLTEPHYDESC pPhy);
U32 PhyBsTxPreExec(PLTEPHYDESC pPhy);


U32 PhyBsTxPostExec(PLTEPHYDESC pPhy);
PTASKLIST PhyBsTxGetNextTaskList(PLTEPHYDESC pPhy, U32 idx);
#ifdef NON_BLOCKING_SCHEDULER
U32 PhyBsTxCB(PLTEPHYDESC pPhy);
U32 PhyBsRxCB(PLTEPHYDESC pPhy);
#endif

#ifdef UL_STATIC_LIST
U32 CreateUlRxTaskList(PLTEPHYDESC pPhy);
U32 UpdateUlRxTaskList(PLTEPHYDESC pPhy);
U32 UpdateChannelSpecificUlRxTaskList(PLTEPHYDESC pPhy, S16 srsPilotSymbol);
U32 PhyBsRxPreExec(PLTEPHYDESC pPhy);
U32 PhyBsRxExeTaskList(PLTEPHYDESC pPhy);


U32 PhyBsRxPostExec(PLTEPHYDESC pPhy);
PTASKLIST PhyBsRxGetNextTaskList(PLTEPHYDESC pPhy, U32 idx);
U32 CreatePrachTaskList(PLTEPHYDESC pPhy);
U32 UpdatePrachTaskList(PLTEPHYDESC pPhy);

#endif

// Phy context api
void setPhyCtx(U32 phyInstance, U32 sfCxtNum, PLTEPHYDESC pPhy);
PLTEPHYDESC getPhyCtxApiProc(U32 phyInstance);
PLTEPHYDESC getPhyCtxListProc(U32 phyInstance);
PLTEPHYDESC getPhyCtxByNum(U32 phyInstance, U32 sfCxtNum);
PULSUBFRDESC getPhyCtxRxVector(U32 phyInstance);
PDLSUBFRDESC getPhyCtxTxVector(U32 phyInstance);

void swapPhyCtx(U32 phyInstance);
U8 getPhyCtxIndx(U32 phyInstance);

void setPhyCtxDemodDone(U32 phyInstance, U32 sfCxtNum);
U8 getPhyCtxDemodDone(U32 phyInstance);

void LTE_Chan_Deint_Gen(S32 data_start, S32 data_end,
                        S16 CP, S16 Qm, S32 ncol,
                        S32 ndat, S32 ncqi, S32 nri,
                        S8  *inp, S8 *outadr[], S32 outlen[], S32 *nout);

void div_quo_rem(S32 x, S32 y, S32 yscf, S32 *q, S32 *r);

void fec_hdwr_chan_deint_sim(U16 ColInt,        // Number of channel interleaver columns (12 or 14)
                             U16 Qm,            // Number of modulation bits used (2,4,6)
                             U32 E,             // Rate matching E parameter
                             S8  *DescAdr[],    // Channel de-interleaver descriptor starting addresses
                             S32 DescLen[],     // Channel de-interleaver descriptor byte counts
                             U16 DescN);        // Channel de-interleaver number of descriptors

void PatchCRC16(U8 crc[], S32 num_bits);


#endif // _PROTOTYPE_H_

