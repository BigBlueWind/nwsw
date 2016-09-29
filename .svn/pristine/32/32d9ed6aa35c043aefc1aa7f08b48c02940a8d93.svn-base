//-------------------------------------------------------------------------------------------
/** @file bs_rx_MultiChannels.c
 *
 * @brief LTE BS Transmitter, Physical Channel processing from input
 *        codewords to output resource elements
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#if !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#ifdef _WIN32
#define WIN_BLD 1
#else
#define WIN_BLD 0
#endif

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "math_ext32.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
extern void DmaFromExtDram(void *p_ext_addr, void *p_int_addr, unsigned size);
extern U8 *DataBuff_Bank1, *DataBuff_Bank2_Off;

#define CEVA_BUILD 1
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#define round(x)    round_s(x)
#endif

//-------------------------------------------------------------------------------------------
/** @brief Groups several Demodulation functions into a single call per symbol PUSCH
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   *RXIN            - Input IQ samples from FFT.
 *  @param   *CHIN            - For PUCCH, this is the channel estimate, for PUSCH used as temp buffer for ChanEst
 *  @param   *chEst_pilot     - For PUSCH, contains the channel estimate of the 2 pilot symbols
 *  @param   *RXOUT           - Temp buffer used as the output from MRC for RxIn
 *  @param   *CHOUT           - Temp buffer used as the output from MRC for ChIn
 *  @param   *VecChEst        - Contains the slops for ChanEst calculated in ChanEstP2.
 *  @param   *derotation      - Contains the Derotation calculated from ChanEstP1.
 *  @param   *expo_reEst      - Exponents from FFT
 *  @param   *expo_ch         - Exponents from ChanEstP1
 *  @param   expo_norm        - Normalization exponent
 *  @param   *reDet0          - Final output from FEQ, which is the final output from rx_Multichan_DeModulation.
 *  @param   *expo_reEst_norm - Temp buffer used to store new exponent from FEQ
 *  @param   *expo_reEst1     - Temp buffer used to store final exponent from FEQ
 *  @param   NOUT             - Number of IQ samples to process for this symbol
 *  @param   BLKSIZ           - Number of IQ samples to process for this symbol
 *  @param   Nul_symb         - Number of symbols per slot
 *  @param   MO               - Number of Rx Ant
 *  @param   MI               - Number of Tx Ant
 *  @param   isym             - Current symbol number being processed
 *  @param   K                - Total number of SubCarriers in symbol
 *  @param   L                - Total number of symbols in subframe
 *  @param   length_b         - Total number of samples
 *  @param   chanType         - Channel Type
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name             |Name in  |Direction|Length    |Type          |Data Format |Valid Range |Units |
 *  |                 |Standard |         |          |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*RXIN            |         |pInput   |MO*NOUT   |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*CHIN            |         |pInput   |MO*NOUT   |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*chEst_pilot     |         |pInput   |MO*NOUT   |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*RXOUT           |         |pInput   |MO*NOUT   |pComplex long |(1:0:32)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*CHOUT           |         |pInput   |MO*NOUT   |pComplex long |(1:0:32)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*VecChEst        |         |pInput   |MO*NOUT   |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*derotation      |         |pInput   |MO*NOUT   |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_reEst      |         |pInput   |MO        |short         |(1:15:0)    |[-32:31]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_ch         |         |pInput   |MO        |short         |(1:15:0)    |[-16:16]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |expo_norm        |         |Input    |1         |short         |(1:15:0)    |[-16:16]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*reDet0          |         |pOutput  |MO*NOUT   |pComplex      |(1:0:15)    |rull range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_reEst_norm |         |pInput   |MO        |short         |(1:15:0)    |[-32:31]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_reEst1     |         |pInput   |MO        |short         |(1:15:0)    |[-32:31]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |NOUT             |         |Input    |1         |short         |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |BLKSIZ           |         |Input    |1         |short         |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Nul_symb         |         |Input    |1         |short         |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |MO               |         |Input    |1         |short         |(1:15:0)    |[1,2,4]     |      |
 *  --------------------------------------------------------------------------------------------------
 *  |MI               |         |Input    |1         |short         |(1:15:0)    |[1,2,4]     |      |
 *  --------------------------------------------------------------------------------------------------
 *  |isym             |         |Input    |1         |short         |(1:15:0)    |[0:13]      |      |
 *  --------------------------------------------------------------------------------------------------
 *  |K                |         |Input    |1         |short         |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |L                |         |Input    |1         |short         |(1:15:0)    |[0:13]      |      |
 *  --------------------------------------------------------------------------------------------------
 *  |length_b         |         |Input    |1         |short         |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |chanType         |         |Input    |1         |short         |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------------------
 *  |Block Name          |rx_Multichan_DeModulation                    |
 *  --------------------------------------------------------------------
 *  |Input Stream        |RXIN,CHIN,chEst_pilot,RXOUT,CHOUT,           |
 *  |                    |VecChEst,derotation                          |
 *  --------------------------------------------------------------------
 *  |Output Stream       |reDet0                                       |
 *  --------------------------------------------------------------------
 *  |Config Parameters   |expo_reEst,expo_ch,expo_norm,expo_reEst_norm,|
 *  |                    |expo_reEst1,NOUT,BLKSIZ,Nul_symb,MO,MI,isym, |
 *  |                    |K,L,length_b,chanType                        |
 *  --------------------------------------------------------------------
 *  |Status Parameters   |none                                         |
 *  --------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function groups ChanEstP3, ChanEstP3, rx_expo_eq16, MRC and FEQ into a single
 *  call per symbol.   This reduces the number of interrupts from Ceva to ARM.  For
 *  more detailed explanation of functions grouped here, please refer to:
 *
 *  -# bs_rx_ChEstimator_part3_fx()
 *  -# bs_rx_ChEstimator_part4_fx()
 *  -# rx_exponent_equalization_16bit()
 *  -# RX_MRCOM_C()
 *  -# bs_rx_feq32b()
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void rx_Multichan_DeModulation(S16 *RxIn, S16 *chEst_ul, S16 *chEst_pilot, S32 *RxOut, S32 *ChOut,
                               S16 *derotation, S16 *expo_reEst, S16 *expo_pilot,
                               S16 *reDet0, S16 *expo_reEst_norm, S16 *expo_reEst1, S16 expo_norm,
                               S32 *noiseVar, S16 MO, S16 Msc, S16 L, S16 isym, S16 Nul_symb, S16 Qm)
{
    S32 i, j;
    S32 length_b, length_b1;
    S32 noiseVarAvg = 0;
    S16 shft, temp1;
    S16 shift2;
    S32 noiseVar1[NUM_SLOTS_SUBFRM * MAX_NUM_ANT];
    S16 expo_pilot_norm[MAX_NUM_ANT], expo0, expo1;
    S16 expo_ch[MAX_SYM_PER_SUBFRAME * MAX_NUM_ANT];

    length_b = 2 * Msc;
    length_b1 = 4 * Msc;
    //Channel Estimate for PUSCH
    //----------------------------
    for (i = 0; i < MO; i++)
    {
        expo_pilot_norm[i] = expo_pilot[2 * i];
        if(expo_pilot_norm[i] < expo_pilot[2 * i + 1])
            expo_pilot_norm[i] = expo_pilot[2 * i + 1];

        expo0 = expo_pilot[2 * i]     - expo_pilot_norm[i];
        expo1 = expo_pilot[2 * i + 1] - expo_pilot_norm[i];

        for(j = 0; j < L; j++)
            expo_ch[i * L + j] = expo_pilot_norm[i];

        noiseVar1[2*i]   = L_shl(noiseVar[2*i], (S16) (2*expo0));
        noiseVar1[2*i+1] = L_shl(noiseVar[2*i+1], (S16) (2*expo1));

        StartSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_CHANEST_P3);
        bs_rx_ChEstimator_part3_fx(&RxIn[i * length_b],
                        &derotation[0], Msc);
        StopAndStartSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_CHANEST_P3, SUBTASKID_RX_MULTICHAN_DEMODULATION_CHANEST_P4);

#if 0//def 0//CEVA_INTRINSICS
        bs_rx_ChEstimator_part4_fx_A(&chEst_pilot[i*length_b1], &chEst_ul[i*length_b],
                                   Msc, isym, &expo_pilot[2*i]);
#else
        bs_rx_ChEstimator_part4_fx(&chEst_pilot[i*length_b1], &chEst_ul[i*length_b],
                                    Msc, isym, &expo_pilot[2*i]);
#endif

        StopSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_CHANEST_P4);

#ifdef DEBUG_PRINT
        filePrint(fChanEst_p3, &RxIn[i*length_b], Msc, "S16", "Complex", (double)1.0, LOG_BS_CHANESTP3_PUSCH_FX);
        filePrint(fChanEst_p4, &chEst_ul[i*length_b], Msc, "S16", "Complex", (double)1.0, LOG_BS_CHANESTP4_PUSCH_FX);
#endif
    }
     for (i = 0; i < MO; i++)
    {
        shift2 = 2*(expo_ch[i * 2 * Nul_symb] - expo_norm);
        noiseVarAvg = L_add(noiseVarAvg,L_shl(noiseVar1[2 * i],shift2));
        noiseVarAvg = L_add(noiseVarAvg,L_shl(noiseVar1[2 * i + 1],shift2));
    }

    shft = 1; // 1 antenna
    if (MO == 2)
        shft = 2;
    else if (MO == 4)
        shft = 3;

    temp1 = 0;
    if (Qm > 2)
        temp1 = 1;
    noiseVarAvg  = L_shr(noiseVarAvg, 2*shft+temp1); //First shift is for averaging , Need to add extra shft to right since ChPwr has same shift in MRC

    // rx_exponent_equalization_16bit
    //----------------------------
    StartSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_EXP_EQ16);
#ifdef CEVA_INTRINSICS
    rx_exponent_equalization_16bit_upper_A(RxIn, chEst_ul, length_b, expo_reEst, &expo_ch[isym],
                        L, expo_norm, Msc, MO);
#else
    rx_exponent_equalization_16bit(RxIn, chEst_ul, length_b, expo_reEst, &expo_ch[isym],
                                   L, expo_norm, Msc, MO);
#endif

    StopAndStartSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_EXP_EQ16, SUBTASKID_RX_MULTICHAN_DEMODULATION_MRCOM);

    // RX_MRCOM
    //-----------
#ifdef CEVA_INTRINSICS

    rx_mrcom_c_A(RxIn, chEst_ul, RxOut, ChOut, Msc, Msc, MO);

#else // CEVA_INTRINSICS

    rx_mrcom_c(RxIn, chEst_ul, RxOut, ChOut, Msc, Msc, MO);
#ifdef DEBUG_PRINT
    for (i = 0; i < MO; i++)
    {
        debugExpo[isym][i][3] = expo_reEst[i] - expo_norm;
        debugExpo[isym][i][4] = expo_ch[isym + i*L] - expo_norm;
    }

    {
        double max = 0.0;

        max = pow(2., (double)debugExpo[isym][0][2]);
        filePrint(fMrc_Out, RxOut, Msc, "S32", "Complex", (double)(pow(2., 2.*11.)*max), LOG_BS_MRCOUT_FX);
        filePrint(fMrc_ChanPower, ChOut, Msc, "S32", "real", (double)(pow(2., 2.*11.)*max), LOG_BS_MRC_CHANPOW_FX);
    }
#endif  // DEBUG_PRINT
#endif  // CEVA_INTRINSICS

    StopAndStartSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_MRCOM, SUBTASKID_RX_MULTICHAN_DEMODULATION_FEQ);

#ifdef CEVA_INTRINSICS
    noiseVarAvgPlus_A(ChOut,Msc, noiseVarAvg);
    bs_rx_feq32b_A(RxOut, reDet0, ChOut, expo_reEst1, expo_reEst_norm, Msc);
#else
    bs_rx_feq32b(RxOut, reDet0, ChOut, expo_reEst1, expo_reEst_norm, noiseVarAvg, Msc);

#ifdef DEBUG_PRINT
        for (i = 0; i < MO; i++)
        {
            debugExpo[isym][i][5] = expo_reEst_norm[isym];
        }
#endif // DEBUG_PRINT
#endif // CEVA_INTRINSICS

    StopSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_FEQ);

}

//-------------------------------------------------------------------------------------------
/** @brief Groups several Demodulation functions into a single call per symbol for PUCCH
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   *pRxIn           - Input IQ samples from FFT.
 *  @param   *pChEstIn        - For PUCCH, this is the channel estimate, for PUSCH used as temp buffer for ChanEst
 *  @param   *pRxOut          - Temp buffer used as the output from MRC for RxIn
 *  @param   *pChEstPow       - Temp buffer used as the output from MRC for ChIn
 *  @param   *expo_reEst      - Exponents from FFT
 *  @param   *expo_ch         - Exponents from ChanEstPUCCH
 *  @param   *expo_norm       - Normalization exponents
 *  @param   *reDet0          - Final output from FEQ, which is the final output from rx_Multichan_DeModulation.
 *  @param   *expo_reEst_norm - Temp buffer used to store new exponent from FEQ
 *  @param   *expo_temp       - Temp buffer used to store final exponent from FEQ
 *  @param   *pSnr            - SNR calculated
 *  @param   *r_alpha_uvf     - pointer to Reference Signal Sequence
 *  @param   *bDetSoft        - Soft decisions
 *  @param   *Sdescramb       - Scrambling flag for PUCCH 1, 1A, 1B
 *  @param   *nOc             - Orthogonal Sequence Index (DATA)
 *  @param   srsPilotSym      - SRS Pilot Symbol
 *  @param   *pRxInPar        - Pointer to input structure containing all Channel information
 *  @param   ChanNum          - Channel Number
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name             |Name in  |Direction|Length    |Type          |Data Format |Valid Range |Units |
 *  |                 |Standard |         |          |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*pRxIn            |         |pInput   |MO*NOUT   |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*pChEstIn         |         |pInput   |MO*NOUT   |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*pRxOut           |         |pInput   |MO*NOUT   |pComplex long |(1:0:32)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*pChEstPow        |         |pInput   |MO*NOUT   |pComplex long |(1:0:32)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_reEst       |         |pInput   |MO        |short         |(1:15:0)    |[-32:31]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_ch          |         |pInput   |MO        |short         |(1:15:0)    |[-16:16]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_norm        |         |Input    |1         |short         |(1:15:0)    |[-16:16]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*reDet0           |         |pOutput  |MO*NOUT   |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_reEst_norm  |         |pInput   |MO        |short         |(1:15:0)    |[-32:31]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*expo_temp        |         |pInput   |MO        |short         |(1:15:0)    |[-32:31]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*pSnr             |         |pInput   |MO        |short         |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*r_alpha_uvf      |         |pInput   |          |pComplex      |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*bDetSoft         |         |pOutput  |24        |S8            |(1:7:0)     |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*Sdescramb        |         |pInput   |20        |short         |(1:15:0)    |[0,1]       |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*nOc              |         |pInput   |20        |short         |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |srsPilotSym       |         |pInput   |1         |short         |(1:15:0)    |[0,13]      |      |
 *  --------------------------------------------------------------------------------------------------
 *  |*pRxInPar         |         |pInput   |1         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ChanNum           |         |pInput   |1         |short         |(1:15:0)    |[0:32767]   |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------------------
 *  |Block Name          |rx_Multichan_DeModulation_Pucch              |
 *  --------------------------------------------------------------------
 *  |Input Stream        |pRxIn,pChEstIn,pRxOut,pChEstPow, pSNR,       |
 *  |                    |r_alpha_uvf, nOc, Sdescramb                  |
 *  --------------------------------------------------------------------
 *  |Output Stream       |bDetSoft                                     |
 *  --------------------------------------------------------------------
 *  |Config Parameters   |expo_reEst,expo_ch,expo_norm,expo_reEst_norm,|
 *  |                    |expo_reEst1, srsPilotSym, pRxInPar, ChanNum  |
 *  --------------------------------------------------------------------
 *  |Status Parameters   |none                                         |
 *  --------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function groups rx_expo_eq16, MRC, FEQ, Rotate, CazacAvg, DeMapper and updateBits into a single
 *  call per symbol.   This reduces the number of interrupts from Ceva to ARM.  For
 *  more detailed explanation of functions grouped here, please refer to:
 *
 *  -# rx_exponent_equalization_16bit()
 *  -# RX_MRCOM_C()
 *  -# bs_rx_feq32b()
 *  -# CazacAvg()
 *  -# Rotate()
 *  -# rx_Demapper_fx()
 *  -# updateBits()
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void rx_Multichan_DeModulation_Pucch(S16 *pRxInACK, S16 *pRxInCQI, S16 *pChEstIn, S32 *pRxOut,
                                     S32 *pChEstPow, S16 *pReDet0,
                                     S16 *expo_ch, S16 *expo_temp,
                                     S16 *expo_reEst_norm, S8* scrSeq,
                                     S8 *bDetSoft, ULRXINPUTPARAMS *pRxInPar,
                                     S32 *pRxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                                     S32 *pRxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo,
                                     S32 ChanNum,  S16 StartRB, S32 *snrPerRb, S32 *snrPilot,
                                     S32 *pAllPucchPwrAvg_AntSlot, S16 *chEstVec, S32 *pPucchPwrAvg)
{
    S32 i, j;
    S16 Qm, QmTmp, ChanFormat, MO, Nul_symb;
    S16 L;
    S32 softInd = 0;
    S16 max_expo;

    S16 temp[2] = { 0 }, temp2x0;
    S32 L_temp;

    S16 temp1, temp2;
    S16 expo1;
    S16 ind2W, ind1W, indW, ind2R, ind1R, indR;
    S16 D_CqiSlot, isym, mo, islot;
    S32 noisePwrAvg, SNR;
    S8 temp8;
    S8 zeroScrSeq[24];

#if 0
    static FILE *fmyAVG1;
    static short first_open_avg1 =1;

    if(first_open_avg1)
    {
        fmyAVG1 = fopen("MYAVG1.txt","wt");
        first_open_avg1 = 0;
    }
#endif

    L = pRxInPar->L;
    Nul_symb = pRxInPar->Nul_symb;
    MO = pRxInPar->MO;
    Qm = pRxInPar->Qm[ChanNum];
    ChanFormat = pRxInPar->pucchFormat[ChanNum];

    memset(zeroScrSeq, 0, 24);

    //Find common exponent across slots and antennas
    max_expo = -32768;
    for(i = 0; i < MO; i++)
    {
        //slot 0
        if (max_expo < expo_ch[i*L])
            max_expo = expo_ch[i*L];
        //slot 1
        if (max_expo < expo_ch[i*L + Nul_symb])
            max_expo = expo_ch[i*L + Nul_symb];
    }

    //Bring all channels  to common exponent
    for(i = 0; i < MO; i++)
    {
        //slot 0
        pChEstIn[4*i]   = shl(pChEstIn[4*i], (S16)(expo_ch[i*L]-max_expo));
        pChEstIn[4*i+1] = shl(pChEstIn[4*i+1], (S16)(expo_ch[i*L]-max_expo));

        //slot 1
        pChEstIn[4*i+2] = shl(pChEstIn[4*i+2], (S16)(expo_ch[i*L + Nul_symb]-max_expo));
        pChEstIn[4*i+3] = shl(pChEstIn[4*i+3], (S16)(expo_ch[i*L + Nul_symb]-max_expo));
    }

    if (ChanFormat >= FORMAT2)
    {
        //Bring all CQI  to common exponent
        for(i = 0; i < MO; i++)
        {
            for(j=0; j < NUM_CQI_DATASYMB_PER_SUBF/2; j++)
            {
                //slot 0
                pRxInCQI[2*j + NUM_CQI_DATASYMB_PER_SUBF*2*i]   = shl(pRxInCQI[2*j + NUM_CQI_DATASYMB_PER_SUBF*2*i], (S16)(expo_ch[i*L]-max_expo));
                pRxInCQI[2*j + NUM_CQI_DATASYMB_PER_SUBF*2*i + 1] = shl(pRxInCQI[2*j + NUM_CQI_DATASYMB_PER_SUBF*2*i + 1], (S16)(expo_ch[i*L]-max_expo));

                //slot 1
                pRxInCQI[2*j + NUM_CQI_DATASYMB_PER_SUBF + NUM_CQI_DATASYMB_PER_SUBF*2*i] = shl(pRxInCQI[2*j + NUM_CQI_DATASYMB_PER_SUBF + NUM_CQI_DATASYMB_PER_SUBF*2*i], (S16)(expo_ch[i*L + Nul_symb]-max_expo));
                pRxInCQI[2*j + NUM_CQI_DATASYMB_PER_SUBF + NUM_CQI_DATASYMB_PER_SUBF*2*i + 1] = shl(pRxInCQI[2*j + NUM_CQI_DATASYMB_PER_SUBF + NUM_CQI_DATASYMB_PER_SUBF*2*i + 1], (S16)(expo_ch[i*L + Nul_symb]-max_expo));
            }
        }
    }

    if (ChanFormat != FORMAT2)
    {
        //Bring all ACK  to common exponent
        for(i = 0; i < MO; i++)
        {
            //slot 0
            pRxInACK[4*i]   = shl(pRxInACK[4*i], (S16)(expo_ch[i*L]-max_expo));
            pRxInACK[4*i+1] = shl(pRxInACK[4*i+1], (S16)(expo_ch[i*L]-max_expo));

            //slot 1
            pRxInACK[4*i+2] = shl(pRxInACK[4*i+2], (S16)(expo_ch[i*L + Nul_symb]-max_expo));
            pRxInACK[4*i+3] = shl(pRxInACK[4*i+3], (S16)(expo_ch[i*L + Nul_symb]-max_expo));
        }
        ///////////////////////////////////////////////////
        // RX_MRCOM, across slot and antenna
        ///////////////////////////////////////////////////
        rx_mrcom_c(&pRxInACK[0], &pChEstIn[0],
                pRxOut, pChEstPow, 1, 1, 2*MO);

        ///////////////////////////////////////////////////
        // FEQ
        ///////////////////////////////////////////////////
        if (pChEstPow[0] == 0)
        {
            temp[0] = 0;
            expo_temp[0] = 0;
        }
        else
        {
            expo_temp[0] = norm_l(pChEstPow[0]);
            L_temp = L_shl(pChEstPow[0], expo_temp[0]);

            //divide_tab: temp is in Q(29+16),Q(45)
            //29 from Tab, 16 from round_s before tab
            temp2x0 = round(L_temp);

            temp[0] = divide_tab_lookup(temp2x0);
        }

        // Output maximum ratio-combined value (real part)
        pRxOut[0] = L_mpy_ls(pRxOut[0], temp[0]);
        // Q(45 + 1 -16 - expo_temp[0]), 1 from the inherent shift left of multiply, 16 from the extract_h in L_mpy_ls
        // Output maximum ratio-combined value (imaginary part)
        pRxOut[1] = L_mpy_ls(pRxOut[1], temp[0]);

        temp[0] = norm32to16(pRxOut, pReDet0, 2 * 1, 0);
        expo_reEst_norm[0] = expo_temp[0] - temp[0];

        //Normalizing with respect to max of I/Q
        temp1 = abs_s(pReDet0[0]);
        temp2 = abs_s(pReDet0[1]);
        temp[1] = temp1;
        if(temp2 > temp[1])
            temp[1] = temp2;
        expo1 = norm_s(temp[1]);

        pReDet0[0] = shl(pReDet0[0], expo1);
        pReDet0[1] = shl(pReDet0[1], expo1);

        ////////////////////////////////////////////////
        //Demapper
        ////////////////////////////////////////////////
        Rotate(&pReDet0[0], 1, (S16) 23170);
        if (ChanFormat >= FORMAT2)
            softInd = NUM_CQI_DATASYMB_PER_SUBF*2;

        QmTmp = Qm;
        if (ChanFormat == FORMAT2A)
            QmTmp = 1;

        rx_Demapper_fx(&pReDet0[0], QmTmp, zeroScrSeq, &bDetSoft[softInd], 1, PUCCH, 0);

        bs_rx_SNRPucch(ChanFormat, MO, pRxInPwrAvg_PerRbPilot, pPwrAvg_Pilot_expo, pRxInPwrAvg_PerRbDataSym, pPwrAvg_Data_expo,
                     pAllPucchPwrAvg_AntSlot, pPucchPwrAvg, &noisePwrAvg, StartRB, snrPerRb, &SNR, &max_expo);

        //since we are computing 1 SNR averaged accross all antennas and slots
        //we put that average in locations (antennas and slots)
        for(i = 0; i < NUM_SLOTS_SUBFRM*MO; i++)
            snrPilot[i] = SNR;

        if (ChanFormat < FORMAT2) // do this only for Format 1s
        {
            pReDet0[0] = shl(pReDet0[0], expo_reEst_norm[0]-expo1);
            pReDet0[1] = shl(pReDet0[1], expo_reEst_norm[0]-expo1);

            bs_rx_SignalDetectionPucch(&pChEstIn[0], &pRxInACK[0], ChanFormat, MO,
                           pReDet0, bDetSoft, noisePwrAvg, max_expo);

            if (ChanFormat == FORMAT1)
            {
                temp8 = bDetSoft[0];
                bDetSoft[0] = 1;
                if(temp8 >= 0) //dead zone
                    bDetSoft[0] = 0;
            }
        }
    }

    if (ChanFormat >= FORMAT2)
    {
        softInd = 0; //point beginning of buffer
        D_CqiSlot = NUM_CQI_DATASYMB_PER_SUBF/2;  //number of data symbols per slot of CQI for all Format 2s
        //Extend the channel estimate vector to the same length as the IQ samples vector, to use the existing MRCOM function
        for (mo = 0; mo < MO; mo++)
        {
            ind2W = mo * NUM_CQI_DATASYMB_PER_SUBF * 2;
            ind2R = mo * NUM_SLOTS_PER_SUBF * 2;
            for (islot = 0; islot < NUM_SLOTS_PER_SUBF; islot++)
            {
                ind1W = islot * D_CqiSlot * 2;
                ind1R = islot * 2;

                for (isym = 0; isym < D_CqiSlot; isym++)
                {
                    indW = ind2W + ind1W + isym*2;
                    indR = ind2R + ind1R;

                    chEstVec[indW]    = pChEstIn[indR];       //Re
                    chEstVec[indW +1] = pChEstIn[indR +1];    //Im
                }
            }
        }

        ///////////////////////////////////////////////////
        // RX_MRCOM
        ///////////////////////////////////////////////////
        //Since NUM_CQI_DATASYMB_PER_SUBF is not power of 4, we cannot use the Ceva version of MRC
        rx_mrcom_c(&pRxInCQI[0], &chEstVec[0], pRxOut, pChEstPow,
                    NUM_CQI_DATASYMB_PER_SUBF, NUM_CQI_DATASYMB_PER_SUBF, MO);

        ///////////////////////////////////////////////////
        // FEQ /DEMAPPER
        ///////////////////////////////////////////////////
        for (i = 0; i < NUM_CQI_DATASYMB_PER_SUBF; i++)
        {
            expo_temp[i] = 0;

            temp[0] = norm32to16(&pRxOut[2*i], &pReDet0[2*i], 2 * 1, 0);

            expo_reEst_norm[i] = expo_temp[i] - temp[0];
        }

        //we bring inputs to Demapper to same exponent
        max_expo = -32768;
        for(i = 0; i < NUM_CQI_DATASYMB_PER_SUBF; i++)
        {
            if (max_expo < expo_reEst_norm[i])
                max_expo = expo_reEst_norm[i];
        }
        for(i = 0; i < NUM_CQI_DATASYMB_PER_SUBF; i++)
        {
            pReDet0[2*i] = shl(pReDet0[2*i], expo_reEst_norm[i] - max_expo);
            pReDet0[2*i + 1] = shl(pReDet0[2*i + 1], expo_reEst_norm[i] - max_expo);
        }

        rx_Demapper_fx(&pReDet0[0], Qm, scrSeq, &bDetSoft[softInd], NUM_CQI_DATASYMB_PER_SUBF, PUCCH, 0);

        if (ChanFormat == FORMAT2)// For the other formats we call it above
        {
            bs_rx_SNRPucch(ChanFormat, MO, pRxInPwrAvg_PerRbPilot, pPwrAvg_Pilot_expo, pRxInPwrAvg_PerRbDataSym, pPwrAvg_Data_expo,
                     pAllPucchPwrAvg_AntSlot, pPucchPwrAvg, &noisePwrAvg, StartRB, snrPerRb, &SNR, &max_expo);
            //since we are computing 1 SNR averaged accross all antennas and slots
            //we put that average in locations (antennas and slots)
            for(i = 0; i < NUM_SLOTS_SUBFRM*MO; i++)
                snrPilot[i] = SNR;
        }
        softInd += Qm*NUM_CQI_DATASYMB_PER_SUBF;
    }
}
