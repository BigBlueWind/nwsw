//-------------------------------------------------------------------------------------------
/** @file bs_rx_ChEstimator.c
 *
 * @brief LTE Base Station Receiver, Channel Estimation
 * @author Mindspeed Technologies
 * @version $Revision: 1.48.8.1 $
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
#define CEVA_BUILD 1
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#endif

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver, Channel Estimation for PUSCH (Uplink Shared Channel)
 *         part 0 of 4
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param adet          = Pointer to received Resource Grid for signal group to be processed
 *  @param aPilots       = Pointer to locally  generated pilot reference signal
 *  @param temp_vec1     = Pointer to array that contains the slopes
 *  @param derotation    = Pointer to array that contains constants to derotate adet
 *  @param pChanEstPar   = Structure containing PUSCH ChEst Configuration Parameters
 *  @param MI            = Number of Transmit Antennas
 *  @param ChEst         = Pointer to Channel Estimate
 *  @param slot          = slot number (0 to 20)
 *  @param Msc           = Number of sub-carriers
 *  @param Mrb           = Number of resource blocks
 *  @param start_address = Flag used to determine start of processing
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction      |Length        |Type          |Data Format |Valid Range |Units |
 *  |              |Standard |               |              |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |adet          |         |pInput/pOutput |Msc           |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |aPilots       |         |pInput         |Msc           |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |temp_vec1     |         |pInput         |              |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |derotation    |         |pInput/pOutput |Msc * 2       |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |pChanEstPar   |         |pInput         |              |pStructure    |            |            |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |MI            |         |input          |1             |signed short  |(1:15:0)    |[1,2,4]     |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |ChEst         |         |pOutput        |Msc * 2       |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |slot          |         |input          |1             |signed short  |(1:15:0)    |[1,20]      |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |Msc           |         |input          |1             |signed short  |(1:15:0)    |[1,1200]    |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |Mrb           |         |input          |1             |signed short  |(1:15:0)    |[1,100]     |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |start_address |         |input          |1             |signed short  |(1:15:0)    |[1,2,4]     |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_part1_fx        |
 *  ---------------------------------------------------------
 *  |Input Streams       |adet,aPilots,temp_vec1,derotation,|
 *  |                    |pChanEstPar                       |
 *  ---------------------------------------------------------
 *  |Output Streams      |adet,derotation,ChEst             |
 *  ---------------------------------------------------------
 *  |Config Parameters   |MI,slot,Msc,Mrb,start_address     |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * PUSCH channel estimation algorithm part 1 of 4;<BR>
 *
 *<B>Detailed Description:</B><BR>
 * <I>Channel Estimation for PUSCH</I><BR>
 * 1) For the frequency band allocated to this user, do direct calculation of channel estimate at each pilot
 *    point in the two pilotSymbols of the subframe. This channel estimate is calculated by dividing the received IQ sample
 *    by the known pilot data symbol value.<BR>
 * 2) Perform a least-squares noise averaging in the frequency domain. Note, there is a pilot value
 *    for every frequency point in the pilot symbols.<BR>
 * 3) Do fine timing adjustment to remove the phase rotation with frequency.<BR>
 * 4) Do linear interpolation and extrapolation of these channel estimates at the two pilot symbols
 *    across all symbols, and every subcarrier across the complete subframe.<BR>
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.53, "Physical Uplink Shared Channel"<BR>
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
#ifdef DEBUG_PRINT
static int symbNum = 3;
#endif

void bs_rx_ChEstimator_part0_fx(S16 *adet, S16 *aPilots, ULRXINPUTPARAMS *pChanEstPar, S16 *chEst,
                                S16 *pScaleComputeErr_Tab, S32 ErrAvg[], S16 mo,
                                S32 ChanNum)
{

    S16 length_krange;


    S16 Mpusch_rb, Mpusch_sc;
    //S16 Nrb_sc;

#if 0
    static FILE *fmyBE;
    static short first_open_BE =1;

    if(first_open_BE)
    {
        fmyBE = fopen("MYBE.txt","wt");
        first_open_BE = 0;
    }
#endif

#ifdef DEBUG_PRINT
    double max = 0.0;
#endif


    Mpusch_rb = pChanEstPar->NumRBs[ChanNum];
    Mpusch_sc = pChanEstPar->Mpusch_sc[ChanNum];

    //Nrb_sc = pChanEstPar->Nrb_sc;



    // Step 1) Channel Estimation at pilot locations. i.e. initial channel estimation by division of raw detected values with pilots
    if (pChanEstPar->MI != 1)
    {
#ifdef _WIN32
        myprintf("error in bs_rx_ChEstimator(): only MI=1 is supported at this time");
        exit(1);
#endif
    }

    //do pilotCarrierSymbol
    //array of frequency index to pilots in first reference symbol %!!!!!!! need to support BSId offset index
    // Step 2) Do fine timing adjustment to remove the phase rotation with frequency.
    // !!!!!!!!!!!!!!move to function FineTimeAdjust(), since re-useable
    //calculate time offset
    //Err = 0;  %init error. Accumulate below, %!!!!!!!independently for all
    //Resource Blocks, for now. Whether consecutive in frequency or not...
    //kre= [0:Nrb_sc-1];

    length_krange = Mpusch_sc;

#ifdef DEBUG_PRINT
    filePrint(fChanEstIn, adet, Mpusch_sc, "S16", "Fix2Fl_Complex",
            (double)(pow(2., 11.)/pow(2., (double)debugExpo[symbNum][mo][0])), LOG_BS_CHANESTIN_FX);
#endif

#ifdef CEVA_INTRINSICS

    ComputeComplexConjMult_A(adet, aPilots, chEst, length_krange);
    Compute_phase_error_A(chEst, ErrAvg, Mpusch_rb, length_krange, pScaleComputeErr_Tab);

#else   //CEVA_INTRINSICS
    ComputeComplexConjMult(adet, aPilots, chEst, length_krange);
    Compute_phase_error(chEst, ErrAvg, Mpusch_rb, length_krange, pScaleComputeErr_Tab);

#endif //CEVA_INTRINSICS
#ifdef DEBUG_PRINT
    filePrint(fChanEstStage1, chEst, Mpusch_sc, "S16", "Fix2Fl_Complex",
            (double)(pow(2., 10.)/pow(2., (double)debugExpo[symbNum][mo][0])), LOG_BS_CHANEST_STAGE1_FX);
#endif
 }

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver, Channel Estimation for PUSCH (Uplink Shared Channel)
 *         part 1 of 4
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param adet          = Pointer to received Resource Grid for signal group to be processed
 *  @param aPilots       = Pointer to locally  generated pilot reference signal
 *  @param temp_vec1     = Pointer to array that contains the slopes
 *  @param derotation    = Pointer to array that contains constants to derotate adet
 *  @param pChanEstPar   = Structure containing PUSCH ChEst Configuration Parameters
 *  @param MI            = Number of Transmit Antennas
 *  @param ChEst         = Pointer to Channel Estimate
 *  @param slot          = slot number (0 to 20)
 *  @param Msc           = Number of sub-carriers
 *  @param Mrb           = Number of resource blocks
 *  @param start_address = Flag used to determine start of processing
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction      |Length        |Type          |Data Format |Valid Range |Units |
 *  |              |Standard |               |              |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |adet          |         |pInput/pOutput |Msc           |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |aPilots       |         |pInput         |Msc           |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |temp_vec1     |         |pInput         |              |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |derotation    |         |pInput/pOutput |Msc * 2       |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |pChanEstPar   |         |pInput         |              |pStructure    |            |            |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |MI            |         |input          |1             |signed short  |(1:15:0)    |[1,2,4]     |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |ChEst         |         |pOutput        |Msc * 2       |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |slot          |         |input          |1             |signed short  |(1:15:0)    |[1,20]      |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |Msc           |         |input          |1             |signed short  |(1:15:0)    |[1,1200]    |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |Mrb           |         |input          |1             |signed short  |(1:15:0)    |[1,100]     |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |start_address |         |input          |1             |signed short  |(1:15:0)    |[1,2,4]     |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_part1_fx        |
 *  ---------------------------------------------------------
 *  |Input Streams       |adet,aPilots,temp_vec1,derotation,|
 *  |                    |pChanEstPar                       |
 *  ---------------------------------------------------------
 *  |Output Streams      |adet,derotation,ChEst             |
 *  ---------------------------------------------------------
 *  |Config Parameters   |MI,slot,Msc,Mrb,start_address     |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * PUSCH channel estimation algorithm part 1 of 4;<BR>
 *
 *<B>Detailed Description:</B><BR>
 * <I>Channel Estimation for PUSCH</I><BR>
 * 1) For the frequency band allocated to this user, do direct calculation of channel estimate at each pilot
 *    point in the two pilotSymbols of the subframe. This channel estimate is calculated by dividing the received IQ sample
 *    by the known pilot data symbol value.<BR>
 * 2) Perform a least-squares noise averaging in the frequency domain. Note, there is a pilot value
 *    for every frequency point in the pilot symbols.<BR>
 * 3) Do fine timing adjustment to remove the phase rotation with frequency.<BR>
 * 4) Do linear interpolation and extrapolation of these channel estimates at the two pilot symbols
 *    across all symbols, and every subcarrier across the complete subframe.<BR>
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.53, "Physical Uplink Shared Channel"<BR>
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_ChEstimator_part1_fx(S16 *temp_vec1, S16 *derotation,
                                ULRXINPUTPARAMS *pChanEstPar, S16 *chEst,
                                S32 ErrAvg[], S16 mo,
                                S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar, S32 ChanNum, S16 mErrExpoTemp, S32 *mErrTemp,
                                S32 *rbPwrAvg_AntSlot, U8 *rbUsedFlag, S32 *rssi_AntSlot)
{
    S32 i;
    S16 length_krange;
    S16 phaseOffset, RbLocation;

    S16 expo_norm1;
    S16 Mpusch_rb, Mpusch_sc;
    S16 df, Nrb_sc, NusedE, Nfft;
    U32 Fs;
    S16 islot, start_address, Qm;

    S16 dummyNoffset, dummy_expo;
    S16 timeAdjust, index;

    S16 N;
    S16 *rawChEst = NULL;
    S16 DelayOffset;

#ifdef DEBUG_PRINT
    double max = 0.0;
#endif

#if 0
    static FILE *fmyBE;
    static short first_open_BE =1;

    if(first_open_BE)
    {
        fmyBE = fopen("MYBE.txt","wt");
        first_open_BE = 0;
    }
#endif



    //Copy parameters back out of structure so I don't have to
    //change the code below.
    

    //K  = pChanEstPar->K;
    Qm = pChanEstPar->Qm[ChanNum];
    RbLocation = pChanEstPar->RBStart[ChanNum];
    Mpusch_rb = pChanEstPar->NumRBs[ChanNum];
    Mpusch_sc = pChanEstPar->Mpusch_sc[ChanNum];
    df = pChanEstPar->df;
    Fs = pChanEstPar->Fs;
    Nrb_sc = pChanEstPar->Nrb_sc;
    NusedE = pChanEstPar->NusedE;
    Nfft = pChanEstPar->Nfft;

    islot = -1;     //for PUSCH, islot is set to -1 for Compute_Derotation

    start_address = pChanEstPar->RBStart[ChanNum] * Nrb_sc;
    rawChEst = (S16 *) &(temp_vec1[2 * Mpusch_sc]);

    // Step 1) Channel Estimation at pilot locations. i.e. initial channel estimation by division of raw detected values with pilots
    if (pChanEstPar->MI != 1)
    {
#ifdef _WIN32
        myprintf("error in bs_rx_ChEstimator(): only MI=1 is supported at this time");
        exit(1);
#endif
    }

    //do pilotCarrierSymbol
    //array of frequency index to pilots in first reference symbol %!!!!!!! need to support BSId offset index
    // Step 2) Do fine timing adjustment to remove the phase rotation with frequency.
    // !!!!!!!!!!!!!!move to function FineTimeAdjust(), since re-useable
    //calculate time offset
    //Err = 0;  %init error. Accumulate below, %!!!!!!!independently for all
    //Resource Blocks, for now. Whether consecutive in frequency or not...
    //kre= [0:Nrb_sc-1];

    length_krange = Mpusch_sc;

    if(mErrExpoTemp == -32768) // indicator that this is default value
        phaseOffset = get_angle_fx(ErrAvg[0], ErrAvg[1]);
    else
        phaseOffset = get_angle_fx(mErrTemp[0], mErrTemp[1]);

    Compute_Derotation(Fs, phaseOffset, df, Nfft, Mpusch_sc, NusedE, derotation,
                       start_address, islot, &dummyNoffset, &dummy_expo);

#ifdef DEBUG_PRINT
    filePrint(fDerotation, derotation, Mpusch_sc, "S16", "Fix2Fl_Complex", (double)(pow(2., 15.)), LOG_BS_DEROTATION_FX);
#endif

    index = 0;
    switch (pChanEstPar->Nul_rb)
    {
        case 6:
            index = 0;
            break;
        case 15:
            index = 1;
            break;
        case 25:
            index = 2;
            break;
        case 50:
            index = 3;
            break;
        case 75:
            index = 4;
            break;
        case 100:
            index = 5;
            break;
    }

#ifndef CEVA_INTRINSICS
    DelayOffset = shift_r(dummyNoffset, negate(dummy_expo));
#else
    DelayOffset = shr_r(dummyNoffset , dummy_expo);
#endif

    //ratio_fs = Fs / 30720000.0;
    //timeAdjust = (S16) (-ceil(DelayOffset / (16 * ratio_fs)));
    
    timeAdjust = divide_ceil(DelayOffset,timeAdjust_TAB1[index]);
    if(DelayOffset <=0)
    {
      timeAdjust = shr(negate(DelayOffset),timeAdjust_TAB2[index]);
      if(index == 4)
        timeAdjust = mult(timeAdjust,10923);
    }

    if(DelayOffset >0)
       timeAdjust = negate(timeAdjust);
        
    timeAdjust += 31;
    if (timeAdjust > 63)
        timeAdjust = 63; //upper bound
    else if (timeAdjust < 0)
        timeAdjust = 0; //lower bound
    pChanEstPar->BsRxChMeas[ChanNum].TimingAdvance = timeAdjust;

    // apply rotation to the channel estimates already calculated at the pilot locations
    // (really only applying to pilots every 6 frequencies. Others are zero)
    // chEst(mi+CE_MI1).symbol(isym+CE_MI1).element = derotation .* chEst(mi+CE_MI1).symbol(isym+CE_MI1).element;

    //Since chEst have been already multiplied by 0.5 due to Pilots, see
    //phy_bs_rx.c, we don't need to shift right by 1 as for adet
#ifdef CEVA_INTRINSICS
    Remove_HalfCyclicShift_A(derotation, chEst, chEst, Mpusch_sc);
    // Save the raw channel estimate for later SNR calculation
    for (i = 0; i < 2 * length_krange; i++)
    {
        rawChEst[i] = chEst[i];
    }

    bs_rx_ChEstimator_part1_fx_1_A(chEst, temp_vec1, Mpusch_sc, &expo_norm1, derotation);

#else

    Remove_HalfCyclicShift(derotation, chEst, chEst, Mpusch_sc);
    // Save the raw channel estimate for later SNR calculation
    for (i = 0; i < 2 * length_krange; i++)
    {
        rawChEst[i] = chEst[i];
    }

#ifdef DEBUG_PRINT
    filePrint(fChanEstStage2, chEst, Mpusch_sc, "S16", "Fix2Fl_Complex",
                (double)(pow(2., 10.)/pow(2., (double)debugExpo[symbNum][mo][0])), LOG_BS_CHANEST_STAGE2_FX);
#endif

    bs_rx_ChEstimator_part1_fx_1(chEst, temp_vec1, Mpusch_sc, &expo_norm1, derotation);

#endif

    // 2b) Assume pilots lie on a straight line and do least squares straight line fit to these pilot channel
    //      estimates to do some noise averaging. Calc some constants
#ifdef DEBUG_PRINT
    filePrint(fChanEst_BeforeNoiseAvg, chEst, Mpusch_sc, "S16", "Complex", (double)(pow(2., 12.)), LOG_BS_BEFORE_NOISEAVG_FX);
#endif

    N = 12;
    if (Mpusch_rb > 1 && Qm == 2) //QPSK
        N = 24;

    noiseAveragingFrequency(temp_vec1, chEst, Mpusch_sc, expo_norm1, N, Qm);

    // 3b) Estimate noise variance as difference between raw chEst at pilot symbols and noise-averaged chEstimates
    //
    //mean square value of the difference between the raw and smooth chanEst vectors, for both pilot symbols
    //
    // 3c) Estimate relative channel power
    //
    //mean square value of the smooth chanEst vectors, for both pilot symbols
    //
	memset(&snrPerRb_Pilot[RbLocation], 0, 4 * Mpusch_rb);
#ifdef DEBUG_PRINT
    phy_fprintf(fSnr_Calc, LOG_BS_SNR_CALC_FX,
        "%%Logging PUSCH SNR\n");
#endif
        ComputePuschNoisePwr(rawChEst, chEst, Mpusch_sc, &rbPwrAvg_AntSlot[RbLocation], &rbUsedFlag[RbLocation], rssi_AntSlot);

	ComputeSnrReportingInfo(rawChEst, chEst,  1, Mpusch_sc, &snrPerRb_Pilot[RbLocation], snrPilot, noiseVar, Nrb_sc, 1, temp_vec1, Mpusch_rb);

#if 0
	for (i=0; i<  Mpusch_sc; i++)
		 fprintf(fmyBE, "DATA %hd %hd %hd %hd\n", rawChEst[2*i], rawChEst[2*i+1], chEst[2*i], chEst[2*i+1]);
   for (i=0; i<  25; i++)
	   fprintf(fmyBE,"SNROUT %ld\n", snrPerRb_Pilot[i]);

   fprintf(fmyBE,"SNRPILOT %ld\n", *snrPilot);
#endif

	 return;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver, Channel Estimation for PUSCH (Uplink Shared Channel)
 *         part 3 of 4
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param adet          = Pointer to received Resource Grid for signal group to be processed
 *  @param derotation    = Pointer to array that contains constants to derotate adet
 *  @param Mpusch_sc     = Length of inputs and outputs (number of subcarriers)
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction      |Length        |Type          |Data Format |Valid Range |Units |
 *  |              |Standard |               |              |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |adet          |         |pInput/pOutput |Msc           |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |derotation    |         |pInput/pOutput |Msc * 2       |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |Mpusch_sc     |         |input          |1             |signed short  |(1:15:0)    |[1,1200]    |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_part3_fx        |
 *  ---------------------------------------------------------
 *  |Input Streams       |adet,derotation                   |
 *  ---------------------------------------------------------
 *  |Output Streams      |adet,derotation                   |
 *  ---------------------------------------------------------
 *  |Config Parameters   |Mpusch_sc                         |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * PUSCH channel estimation algorithm part 3 of 4;<BR>
 *
 *<B>Detailed Description:</B><BR>
 * <I>Channel Estimation for PUSCH</I><BR>
 * 1) For the frequency band allocated to this user, do direct calculation of channel estimate at each pilot
 *    point in the two pilotSymbols of the subframe. This channel estimate is calculated by dividing the received IQ sample
 *    by the known pilot data symbol value.<BR>
 * 2) Perform a least-squares noise averaging in the frequency domain. Note, there is a pilot value
 *    for every frequency point in the pilot symbols.<BR>
 * 3) Do fine timing adjustment to remove the phase rotation with frequency.<BR>
 * 4) Do linear interpolation and extrapolation of these channel estimates at the two pilot symbols
 *    across all symbols, and every subcarrier across the complete subframe.<BR>
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.53, "Physical Uplink Shared Channel"<BR>
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_ChEstimator_part3_fx(S16 *adet, S16 *derotation, S16 Mpusch_sc)
{

#ifdef CEVA_INTRINSICS
    Remove_HalfCyclicShift_A(derotation, adet, adet, Mpusch_sc);
#else
    Remove_HalfCyclicShift(derotation, adet, adet, Mpusch_sc);
#endif

}

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver, Channel Estimation for PUSCH (Uplink Shared Channel)
 *         part 4 of 4
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param temp_vec1  = Pointer to array that contains the slopes
 *  @param chEstIn    =
 *  @param chEstOut   =
 *  @param Msc        = Number of sub-carriers
 *  @param isym       =
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction      |Length        |Type          |Data Format |Valid Range |Units |
 *  |              |Standard |               |              |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |temp_vec1     |         |pInput         |              |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |chEstIn       |         |pInput         |              |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |chEstOut      |         |pOutput        |              |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |Msc           |         |input          |1             |signed short  |(1:15:0)    |[1,1200]    |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |isym          |         |pInput         |1             |signed short  |(1:15:0)    |3,10        |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_part4_fx        |
 *  ---------------------------------------------------------
 *  |Input Streams       |temp_vec1,chEstIn                 |
 *  ---------------------------------------------------------
 *  |Output Streams      |chEstOut                          |
 *  ---------------------------------------------------------
 *  |Config Parameters   |Msc,isym                          |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * PUSCH channel estimation algorithm part 4 of 4;<BR>
 *
 *<B>Detailed Description:</B><BR>
 * <I>Channel Estimation for PUSCH</I><BR>
 * 1) For the frequency band allocated to this user, do direct calculation of channel estimate at each pilot
 *    point in the two pilotSymbols of the subframe. This channel estimate is calculated by dividing the received IQ sample
 *    by the known pilot data symbol value.<BR>
 * 2) Perform a least-squares noise averaging in the frequency domain. Note, there is a pilot value
 *    for every frequency point in the pilot symbols.<BR>
 * 3) Do fine timing adjustment to remove the phase rotation with frequency.<BR>
 * 4) Do linear interpolation and extrapolation of these channel estimates at the two pilot symbols
 *    across all symbols, and every subcarrier across the complete subframe.<BR>
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.53, "Physical Uplink Shared Channel"<BR>
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_ChEstimator_part4_fx(S16 *chEstIn, S16 *chEstOut, S16 Msc, S16 isym, S16 *expo_norm)
{
    S32 i, I;
    S16 temp;
    S16 Mpusch_sc;
    S16 expo_norm1;
    S16 *chEstU, *chEstL;
    S16 chEstUR, chEstLR[4] = {0};
    S16 chEstUI, chEstLI[4] = {0};

    S16 expo0, expo1;

    S16 tmpR0, tmpI0;
    S16 tmpR1, tmpI1;
    S16 tmpR2, tmpI2;
    S16 tmpR3, tmpI3;

    S16 temp1 = 4681; // 1/7 (1/Nul_symb)
    S16 temp_vec[8] = {0};

    //Copy parameters back out of structure so I don't have to
    //change the code below.
    Mpusch_sc = Msc;
    temp = isym - 3;

    expo_norm1 = expo_norm[0];
    if(expo_norm1 < expo_norm[1])
        expo_norm1 = expo_norm[1];

    expo0 = expo_norm[0] - expo_norm1;
    expo1 = expo_norm[1] - expo_norm1;

    chEstL = (S16*)(&chEstIn[0]);
    chEstU = (S16*)(&chEstIn[2*Msc]);
    //interpolate all symbols between first and second pilot symbol
    for (i = 0; i < Mpusch_sc / 4; i++)
    {
        I = i << 3;

        chEstLR[0] = shl(chEstL[I + 0], expo0);
        chEstLI[0] = shl(chEstL[I + 1], expo0);
        chEstUR    = shl(chEstU[I + 0], expo1);
        chEstUI    = shl(chEstU[I + 1], expo1);

        tmpR0       = sub(chEstUR, chEstLR[0]);
        temp_vec[0] = mult(tmpR0, temp1);
        tmpI0       = sub(chEstUI, chEstLI[0]);
        temp_vec[1] = mult(tmpI0, temp1);

        chEstLR[1] = shl(chEstL[I + 2], expo0);
        chEstLI[1] = shl(chEstL[I + 3], expo0);
        chEstUR    = shl(chEstU[I + 2], expo1);
        chEstUI    = shl(chEstU[I + 3], expo1);

        tmpR1       = sub(chEstUR, chEstLR[1]);
        temp_vec[2] = mult(tmpR1, temp1);
        tmpI1       = sub(chEstUI, chEstLI[1]);
        temp_vec[3] = mult(tmpI1, temp1);

        chEstLR[2] = shl(chEstL[I + 4], expo0);
        chEstLI[2] = shl(chEstL[I + 5], expo0);
        chEstUR    = shl(chEstU[I + 4], expo1);
        chEstUI    = shl(chEstU[I + 5], expo1);

        tmpR2       = sub(chEstUR, chEstLR[2]);
        temp_vec[4] = mult(tmpR2, temp1);
        tmpI2       = sub(chEstUI, chEstLI[2]);
        temp_vec[5] = mult(tmpI2, temp1);

        chEstLR[3] = shl(chEstL[I + 6], expo0);
        chEstLI[3] = shl(chEstL[I + 7], expo0);
        chEstUR    = shl(chEstU[I + 6], expo1);
        chEstUI    = shl(chEstU[I + 7], expo1);

        tmpR3       = sub(chEstUR, chEstLR[3]);
        temp_vec[6] = mult(tmpR3, temp1);
        tmpI3       = sub(chEstUI, chEstLI[3]);
        temp_vec[7] = mult(tmpI3, temp1);

        tmpR0 = (S16) add(chEstLR[0], (S16) (temp * temp_vec[0]));
        tmpI0 = (S16) add(chEstLI[0], (S16) (temp * temp_vec[1]));

        tmpR1 = (S16) add(chEstLR[1], (S16) (temp * temp_vec[2]));
        tmpI1 = (S16) add(chEstLI[1], (S16) (temp * temp_vec[3]));

        tmpR2 = (S16) add(chEstLR[2], (S16) (temp * temp_vec[4]));
        tmpI2 = (S16) add(chEstLI[2], (S16) (temp * temp_vec[5]));

        tmpR3 = (S16) add(chEstLR[3], (S16) (temp * temp_vec[6]));
        tmpI3 = (S16) add(chEstLI[3], (S16) (temp * temp_vec[7]));

        chEstOut[I]     = tmpR0;
        chEstOut[I + 1] = tmpI0;
        chEstOut[I + 2] = tmpR1;
        chEstOut[I + 3] = tmpI1;
        chEstOut[I + 4] = tmpR2;
        chEstOut[I + 5] = tmpI2;
        chEstOut[I + 6] = tmpR3;
        chEstOut[I + 7] = tmpI3;
    }

    return;
}

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver, Channel Estimation for PUSCH (Uplink Shared Channel)
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param temp_vec1  = Pointer to array that contains the slopes
 *  @param chEstIn    =
 *  @param chEstOut   =
 *  @param Msc        = Number of sub-carriers
 *  @param isym       =
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction      |Length        |Type          |Data Format |Valid Range |Units |
 *  |              |Standard |               |              |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |chEst         |         |pInput         |              |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |temp_vec      |         |pOutput        |              |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |derotation    |         |pOutput        |              |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |Mpusch_sc     |         |input          |1             |signed short  |(1:15:0)    |[1,1200]    |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |expo_norm1    |         |pOutput        |1             |signed short  |(1:15:0)    |[0-15]      |      |
 *  ---------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_part1_fx_1c     |
 *  ---------------------------------------------------------
 *  |Input Streams       |chEst                             |
 *  ---------------------------------------------------------
 *  |Output Streams      |temp_vec,derotation               |
 *  ---------------------------------------------------------
 *  |Config Parameters   |Mpusch_sc,expo_norm1              |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * Part of PUSCH channel estimation ;<BR>
 *
 *<B>Detailed Description:</B><BR>
 * <I>Channel Estimation for PUSCH</I><BR>
 * 1) Scale Derotation vector to match aDet.<BR>
 * 2) Block normalization of Channel Estimate at Pilots.<BR>
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.53, "Physical Uplink Shared Channel"<BR>
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_ChEstimator_part1_fx_1(S16 *chEst, S16 *temp_vec1, S16 Mpusch_sc, S16 *expo_norm1, S16 *derotation)
{
    S32 i, I;
    S16 tmpR0, tmpR1, tmpR2, tmpR3;
    S16 tmpI0, tmpI1, tmpI2, tmpI3;
    S32 Ltmp0, Ltmp1, Ltmp2, Ltmp3;
    S32 Ltmp_0, Ltmp_1, Ltmp_2, Ltmp_3;

    //Scale down derotation for derotation processing of adet in bs_rx_ChEstimator_part3_fx
    for (i = 0; i < (S32)(Mpusch_sc / 4); i++)
    {
        I = i << 1;

        tmpR0 = (S16) mult(derotation[4 * I], 16384);
        tmpI0 = (S16) mult(derotation[4 * I + 1], 16384);

        tmpR1 = (S16) mult(derotation[4 * I + 2], 16384);
        tmpI1 = (S16) mult(derotation[4 * I + 3], 16384);

        tmpR2 = (S16) mult(derotation[4 * I + 4], 16384);
        tmpI2 = (S16) mult(derotation[4 * I + 5], 16384);

        tmpR3 = (S16) mult(derotation[4 * I + 6], 16384);
        tmpI3 = (S16) mult(derotation[4 * I + 7], 16384);

        derotation[4 * I] = tmpR0;
        derotation[4 * I + 1] = tmpI0;
        derotation[4 * I + 2] = tmpR1;
        derotation[4 * I + 3] = tmpI1;
        derotation[4 * I + 4] = tmpR2;
        derotation[4 * I + 5] = tmpI2;
        derotation[4 * I + 6] = tmpR3;
        derotation[4 * I + 7] = tmpI3;
    }

    // Block normalization of Channel at Pilots
    Ltmp0 = 0;
    Ltmp1 = 0;
    Ltmp2 = 0;
    Ltmp3 = 0;
    for (i = 0; i < 2 * Mpusch_sc; i += 4)
    {
        Ltmp_0 = L_mult(chEst[i], chEst[i]);
        Ltmp_1 = L_mult(chEst[i + 1], chEst[i + 1]);
        Ltmp_2 = L_mult(chEst[i + 2], chEst[i + 2]);
        Ltmp_3 = L_mult(chEst[i + 3], chEst[i + 3]);
        Ltmp0 |= Ltmp_0;
        Ltmp1 |= Ltmp_1;
        Ltmp2 |= Ltmp_2;
        Ltmp3 |= Ltmp_3;
    }
    Ltmp0 |= Ltmp1;
    Ltmp0 |= Ltmp2;
    Ltmp0 |= Ltmp3;

    if (Ltmp0 == 0)
        *expo_norm1 = 15;
    else
        *expo_norm1 = norm_l(Ltmp0) >> 1;

    for (i = 0; i < (S32)(2 * Mpusch_sc); i++)
        temp_vec1[i] = shift_r(chEst[i], *expo_norm1);
}
#endif
