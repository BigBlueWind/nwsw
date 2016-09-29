//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL prototypes_ms.h
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.23 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------

#include "ltephy.h"
#include "ltephy_ms.h"
#include "Debug.h"
#include "tcb.h"

#ifndef _PROTOTYPES_FX_MS_H
#define _PROTOTYPES_FX_MS_H

void LteMs_FEQ_CEVA(PTCB pTcb);
void LteMs_FindMax_CEVA(PTCB pTcb);
void LteMs_ExpEQ32_CEVA(PTCB pTcb);
void LteMs_ExpEQ16_CEVA(PTCB pTcb);
void LteMS_rx_ChEstimator_CEVA(PTCB pTcb);
void diag_UplinkTx_FileOpen();
void diag_UplinkTx_FileClose();
void diag_DownlinkRx_FileOpen();
void diag_DownlinkRx_FileClose();
void diag_UplinkTxChanCoder_FileOpen(int numchan);
void diag_UplinkTxChanCoder_FileClose(int numchan);

//MOBILE_STATION
U32 LteMsPhyMain(U32 phyInstance, S16 *sigOut, S16 **sigIn,
                 U32* inputMessage, U32 inputMessageSize, U32* outputMessage,
                 U32* outputMessageSize);
void LteGenerateMsTxSduConfMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize,
                             U32 status, U32 chanId);
void LteGenerateMsRxSduIndMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize,
                            U32 status, U32 chanId, U32 i);

U32 LteMsTxSduProcessing(U32 phyInstance, U32* ptr, U32 chanId, U32 numBytes,
                         U32 maxBitsperCw);
U32 LteMsPhyRxVectorProcessing(U32 phyInstance, PDLSUBFRDESC ptr, U32 len);
U32 LteMsPhyTxVectorProcessing(U32 phyInstance, PULSUBFRDESC ptr, U32 len);

U32 LteMsParseMsg(U32 phyInstance, U32* inMsg, U32 inMsgSize, U32* outMsg,
                       U32* outMsgSize);
U32 LteMsTx(U32 phyInstance, S16* sigOut, U32* outMsg, U32* outMsgSize);
U32 LteMsRx(U32 phyInstance, S16 **sigIn, U32* outMsg, U32* outMsgSize);
void LteGenerateMsAPIMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize,
                       U32 msgType, U32 status);

U32 LtePhyInitializationMs(U32 phyInstance, PINITPARM parmPtr, U32 Type);
void LteMsInitializeMiMoInfo(U32 phyInstance, U32 test_case);
S16 LteMsDeterminePrachSubcSpacing(S16 prachConfigurationIndex);
U32 LteMsSetupPersReMapsUl(PULCHANDESCMS pChan, PULCHANDESC ptr, U32 nRbSc, U32 nSymb);
U32 LteMsSetupPersReMaps(PDLCHANDESCMS pChan, PDLCHANDESC ptr, U32 nRbSc, U32 nSymb);
U32 LteMsSetupPersReMapsDCI(PDLCHANDESCMS pChan, PDCICHANNELDESC ptr, U32 nRbSc, U32 nSymb);
U32 LteMsFindRbAllocMode(PDLCHANDESCMS pChan);
U32 LteMsDetRbandReforChannel(PDLCHANDESCMS pChan, U32 nRbSc, U32 nSymb, U32 mode);
U32 LteMsDetRbandReforChannelUl(PULCHANDESCMS pChan, U32 nRbSc, U32 nSymb, U32 mode);
U32 LteMsUpdateBitMap(PDLCHANDESCMS pChan, U32 strtRb, U32 endRb);
U32 LteMsValidateTbs(PULCHANDESCMS pChan, U32 tBsize);
U32	LteMsReadTbsTable( U16 mcsIndex, U16 numRb);
S16 LteMsDeterminePrachSubcSpacing(S16 prachConfigurationIndex);


// diag
void diagnostic_file_open_ms(U32 numchan);
void diagnostic_file_close_ms(U32 numChan);

void ms_ul_ConfigFrame_Chan(MS *s, MS_TX *pMsTx, MS_TX_CHAN *ulChan);
void ms_ul_ConfigFrame_Globals(MS *s, MS_TX *pMsTx, MS_SRS *ul_srs, MS_TX_PRACH *ul_prach);

// 16 from CRC and 3 from 1/3 convolutional coder
U32 ms_tx_pucch_channel_coding(U8* ptr, U8 out[],
                   S32 numBytes, S16 Qm, S32 numBitsOut);
void scrambler_PUCCH(U16 Ncell_id, U8 bx[], S16 nRNTI, S16 q, S16 ns, U8 by[], S32 length_bx);
void Pusch_ResourceElementMapper(S16 *RbAssign, S16 *z, S16 K, S16 num_row, S16 Nrb_sc, S16 symNum, S16 *a);

void phy_ms_tx_ConfigFrame(MS_TX_PRACH *ul_prach);
U32  phy_ms_tx(PLTEPHYMSDESC pPhy);
#ifdef MSRX
void phy_ms_rx(PLTEPHYMSDESC pPhy, S16 **adc);
#else
void phy_ms_rx(PLTEPHYMSDESC pPhy, S16 *adc);
#endif
void rx_exponent_equalization_16bit_ms(S16 reEst1[], S16 expo_reEst, S16 expo_norm_reEst, S16 Mpusch_sc);
void rx_exponent_equalization_16bit_ms_tcb(S16 reEst1[], S16 expo_reEst, S16 expo_norm_reEst, S16 Mpusch_sc);

void rx_find_max_ms(S16 expo_reEst[], S16 *expo_norm_reEst, S16 Mpusch_sc);
void rx_find_max_ms_tcb(S16 expo_reEst[], S16 *expo_norm_reEst, S16 Mpusch_sc);

void rx_exponent_equalization_32bit_ms(S32 reEst1[], S16 expo_reEst[], S16 expo_norm_reEst, S16 Mpusch_sc);
void rx_exponent_equalization_32bit_ms_tcb(S32 reEst1[], S16 expo_reEst[], S16 expo_norm_reEst, S16 Mpusch_sc);

void ms_rx_feq(S32 reEst1[], S32 ChEst_pwr_fx[], S16 expo_reEst[], S16 Mpusch_sc);
void ms_rx_feq_tcb(S32 reEst1[], S32 ChEst_pwr_fx[], S16 expo_reEst[], S16 Mpusch_sc);

void ms_tx_genHalfCyclicShift(S16 Nfft, S16 *mhcs);
void ms_tx_setupIFFT_Bufs(S16* ifft_in, S16* a, S16 row_a, S16 Nfft);
void ms_tx_hcs_addCP(S16 Nfft, S16 Ncpsam, S16 *mhcs, S16 *symIn, S16 *ptr_dac_samp);

void ms_rx_channel_coding(S32 nin, S32 nout, S8 in[], U8 out[],
                          S16 *stat, S16 harq, S16 NL, S32 G, S16 Qm,
                          S16 rvidx, S32 NIR, U32 halfIt);

void ms_rx_broadcast_chan_coding(S8 in[], U8 out[], S32 nin, S32 nout, S16 nant,
                                 S16 scf, U8 *stat);


void ms_ul_PrachPreamble_Config(MS_TX_PRACH *ul_prach, S16 *u, S16 *Cv, S16 *vLen, S16 *Ncs);

void lpfMid6(S16 adcin[], S16 adcin_subsam[], S32 length_adcin,
             S16 *length_adcin_subsam);

void symbolTimingSync(S16 adcin_subsam[], S16 *syncdet, S16 *syncdetIndex,
                      S16 length_adcin_subsam);

void ms_rx_TimeFreqSync(S16 adc[], S16 syncMode, S32 length_adc,
                        S16 NrxAntenna, S16 *syncdet, S16 *syncdetIndex,
                        S16 *slotNumber);

void rx_RGConstellation_ms(PLTEPHYMSDESC pPhy, PDLCHANDESCMS pChan,
                        S16 **RBAssignment, S16 numPDCCHSymbols, S16 P,
                        S16 subframeNumber, S16 **rgQm, S16 Qm, S16 ROW,
                        S16 COL, S16 *numrows_rgQm, S16 *numcols_rgQm);
void rx_Demapper_ms(S32 aEst1_fx[], S32 ChEst_pwr_fx[], S16 **pRgQm,
                    S16 iSymRange, S16 iSymInSlot, S16 Nsym, S16 *bDetHard,
                    S16 *bDetSoft, S16 NShift, S16 NusedE);

void rx_MrcDetector_fx(S16 **aDet_fx, S16 **chEst_fx, S16 MO, S16 MI,
                       S16 **pRgQm, S16 iSymRange, S16 Nsym, S32 *aEst1_fx,
                       S32 *ChEst_pwr_fx, S16 NusedE);


void rx_Demapper_ms(S32 aEst1_fx[], S32 ChEst_pwr_fx[], S16 **pRgQm,
                    S16 iSymRange, S16 iSymInSlot, S16 Nsym, S16 *bDetHard,
                    S16 *bDetSoft, S16 NShift, S16 NusedE);

#if 0
void Demod_QPSK_ms(S32 RXIN[], S16 *bDetSoft, S16 *bDetHard, S16 NShift,
                S16 numbits);

void Demod_BPSK_ms(S32 RXIN[], S16 *bDetSoft, S16 *bDetHard, S16 NShift,
                S16 numbits);

void Demod_QAM16_ms(S32 RXIN[], S32 ChEst_pwr_fx, S16 *bDetSoft, S16 *bDetHard,
                 S16 NShift, S16 numbits);

void Demod_QAM64_ms(S32 RXIN[], S32 ChEst_pwr_fx, S16 *bDetSoft, S16 *bDetHard,
                 S16 NShift, S16 numbits);
#endif



void Demod_BPSK_ms(S32 RXIN[], S16 *bDetSoft, S16 iShift, S32 iLength);
void Demod_QPSK_ms(S32 RXIN[], S16 *bDetSoft, S16 iShift, S32 iLength);
void Demod_QAM16_ms(S32 RXIN[], S16 *bDetSoft, S16 iShift, S32 iLength);
void Demod_QAM64_ms(S32 RXIN[], S16 *bDetSoft, S16 iShift, S32 iLength);

#if 1
void ms_rx_ChEstimator(PMSCHANESTPARAMS pMsChanEstParam, S16 *adet, S16 *aPilots1, S16 *aPilots2
					   , S16 *chEst, S16 *derotation);
#else
void ms_rx_ChEstimator(PLTEPHYMSDESC pPhy, S16 *adet, S16 *aPilots, S16 MI,
                       S16 *chEst, S16 *Noffset, S16 *expoNoffset, S16 row_a, S16 col_a,
                       S16 iSymInSubframe, S16 *expo_fac);
#endif

PTCB ms_rx_ChEstimator_tcb(PMSCHANESTPARAMS pMsChanEstParam, S16 *adet, S16 *aPilots1, S16 *aPilots2
					   , S16 *chEst, S16 *derotation);


void ms_rx_DemuxOFDMSym(PLTEPHYMSDESC pPhy, PDLCHANDESCMS pChan, S16 *bDetHard,
                        S16 *bDetSoft, S16 Qm, S16 **pRgQm, S16 iSymRange,
                        S16 Nsym, S16 numPDCCHSymbols, S16 subframeNumber,
                        S16 slotNumber, S16 *bUserDetHard, S16 *bUserDetSoft,
                        S16 P, S32 *length_bUserDetHard);

void ms_rx_OFDM_Demodulator(PLTEPHYMSDESC pPhy, S16 *adc, S16 mo,
                            S16 iSymInSlot, S16 *adem, S16 *expo_fac);





void ms_rx_DecodeControl(PLTEPHYMSDESC pPhy,  S16 *bDetHard,
                         S16 *bDetSoft, S16 Qm, S16 **pRgQm, S16 **RG_Assigned,
                         S16 numPDCCHSymbols, S16 *bUserDetHard,
                         S16 *bUserDetSoft, S32 *length_bUserDetHard);

void ms_rx_channel_coding(S32 nin, S32 nout, S8 in[], U8 out[],
                          S16 *stat, S16 harq, S16 NL, S32 G, S16 Qm,
                          S16 rvidx, S32 NIR, U32 halfit);

void ms_rx_broadcast_chan_coding(S8 in[], U8 out[], S32 nin, S32 nout, S16 nant,
                                 S16 scf, U8 *stat);

void ms_rx_dci_chan_coding(S8 in[], U8 out[], S32 nin, S32 nout, U16 scram,
                           S16 scf, U8 *stat);

U32 MsTxChannelCoder(U8* in_dat_ptr, U16 num_in_bytes, U8 out[],
                     S32 G, S16 rvidx, S16 NL, S16 Qm, U32 cwId, U32 ChanId );

void ms_tx_cc_cont_chan_coding(U8 in[], U8 out[], S32 nin, S32 E);

S32 ms_tx_pusch_chan_coding(U8 indat[], U8 incqi[], U8 inri[], U8 inack[],
                            U8 out[],
                            U16 nindat, S32 nincqi, S16 ninri, S16 ninack,
                            S32 Cmux, S16 CP, S32 noutcqi,
                            S32 G, S16 rvidx, S16 NL, S16 Qm,
                            U32 cwId, U32 ChanId );

void reshuffle_pdcch_bits(PLTEPHYMSDESC pPhy, S16 *bUserDetHard,
                          S16 *bUserDetSoft, S16 numPDCCHQuadruplets, S16 Qm);

S16 LteDetermineSymbolsPerSlotMs(S16 cpType, S16 nRbSc);
U32 LteDetermineCpsandSamplingMs(PINITPARM pInit, PLTEPHYMSDESC pPhy);
void PDCFIResourceElementAssignmentMs(PLTEPHYMSDESC pPhy,S16 Ntx_ant, S16 *k_l_row0, S16 *k_l_row1,
                                    U16 *numrows_k_l, U16 *numcols_k_l, S16 **RG_Assigned);

void PDCCHResourceElementAssignmentMs(PLTEPHYMSDESC pPhy,  S16 numPDCCHSymbols,
                                    S16 numPDCCHQuadruplets, S16 Ntx_ant, S16 *k_l_row0,
                                    S16 *k_l_row1, U16 *numrows_k_l, U16 *numcols_k_l,
                                    S16 **RG_Assigned, U16 numRePerSymb[],PDLCHANDESCMS pChan);
void PdschMCSIndexProceduresMs(PDLCHANDESCMS pChan, PMS_RX pMsRx);
void PbchMCSIndexProceduresMs(PDLCHANDESCMS pChan, PMS_RX pMsRx);

void MsRxCtlDataInit(U32 phyInstance, U32* inCtlSz, U32 index);

void MsRxDataInit(U32 phyInstance, U32* inDatSz, U32 index, U32 numCtlCh);
void descrambler_PBCH(U32 *cinit_PBCH, U32 Ncell_id, S8 bx[], U32 nf, S8 by[],
                 U32 length_bx);
void descrambler_PCFIH(U32 Ncell_id, S16 bx_h[], S16 bx_s[], S16 ns,
                 U32 length_bx);
void descrambler_PDCCH(U32 Ncell_id, S16 bx_h[], S16 bx_s[], S16 ns,
                 U32 length_bx);
void descrambler_pucch(U32 Ncell_id, S16 bx_h[], S16 bx_s[], S16 nRNTI,  S16 ns,
                 U32 length_bx);
void descrambler_ms(U32 Ncell_id, S8 bx[], S16 nRNTI, S16 q, S16 ns, S8 by[], U32 length_bx);

void scrambler_ms(U32 Ncell_id, U8 bx[], S16 nRNTI, S16 q, S16 ns, U8 by[], S32 length_bx);

void ComputePHICHParamMs(PLTEPHYMSDESC pPhy, S16 Ntx_ant);
void PHICHResourceElementAssignmentMs(PLTEPHYMSDESC pPhy,S16 Ntx_ant, S16 *k_l_row0, S16 *k_l_row1,
                                    S16 *numrows_k_l, S16 *numcols_k_l, S16 **RG_Assigned);

void convertSoftHardBit(S16 *pSoft, S16 *pHard, S32 L);
void ms_rx_feq(S32 reEst1[], S32 ChEst_pwr_fx[], S16 expo_reEst[], S16 Mpusch_sc);

void RBassignment_pucch(PULPILOTSPARAM pULPilot_Pucch, S16 slotNumber, S16 nPRB[]);
void ControlInfoResourceElementAssignment_PUCCH(S16 *RBAssignment, PULPILOTSPARAM ULPilotInput,
										S16 Msymb, S16 *k_l_ul_row0, S16 *k_l_ul_row1,
                                        S16 *numrows_k_l, S16 *numcols_k_l);
void ul_Pilot_PUCCH_MS(S16 a[], S16 *k_l_ul_row0, S16 *k_l_ul_row1, S16 *RBAssignment,
                    S16 *r_alpha_uv, S16 *nOcBar, PULPILOTSPARAM ULPilotInput, S16 zFlag) ;
void ULPilotResourceElementAssignment_PUCCH_ms(S16 *RBAssignment, PULPILOTSPARAM ULPilotInput,
										S16 Msymb, S16 *k_l_ul_row0, S16 *k_l_ul_row1,
                                        S16 *numrows_k_l, S16 *numcols_k_l);
//void gen_ul_Pilot_info_ms(PULPILOTSPARAM pULPilot_Pucch, S16* r_alpha_uv);
void gen_ul_Pilot_info_ms(PULPILOTSPARAM pULPilot_Pucch, S16* r_alpha_uv, S16* nOc, S16* nOcBar, S16 *Sdescramb, S16 u[]);
void gen_ul_SequenceGroupNumber_U_ms(S16 CellId, S16 groupHoppingEnabled, S16 u[]);
void rx_EVM_ms(S32 aEst1_fx[], S32 ChEst_pwr_fx[], S16 **pRgQm, S16 iSymRange,
                    S16 iSymInSlot, S16 Nsym, S16 NusedE,
                    S32 *pEVM_QPSK, S32 *pEVM_QAM16, S32 *pEVM_QAM64 );

#endif /* _PROTOTYPES_FX_MS_H */
