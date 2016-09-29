//-------------------------------------------------------------------------------------------
/** @file bs_rx_ChEstimatorSrs.c
 *
 * @brief LTE Base Station Receiver, Channel Estimation for SRS (Sounding Reference Signals)
 * @author Mindspeed Technologies
 * @version $Revision: 1.32.8.1 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

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

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#define CEVA_BUILD 1
#include "global.h"
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#endif

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver, Channel Estimation for SRS (Sounding Reference Signals)
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param iSig_Start     - First SRS Channel number
 *  @param numSig         - Number of SRS Channels to be processed
 *  @param cyclicShift    - nCs_srs, in [3GPP TS 36.211, Sec.5.5.3.1]
 *  @param aEstSig        - Pointer to received Resource Grid for signal group to be processed
 *  @param rSrs           - Pointer to the SRS Pilot Reference Signal
 *  @param df             - frequency spacing between subcarriers
 *  @param Fs             - sampling frequency
 *  @param Nul_symb       - Number of symbols per slot
 *  @param Nrb_sc         - Number of sub-carriers per resource block
 *  @param NusedE         - Total number of Sub-Carriers available
 *  @param Nfft           - Size of FFT
 *  @param start_address  - First Resource Block used by SRS allocation
 *  @param islot          - slot number
 *  @param Nul_rb         - Numberof Resource Blocks
 *  @param length_rSrs    - length of SRS signal
 *  @param srsChEst       - Pointer to final SRS Channel Estimate
 *  @param chEst          - Pointer to intermediate SRS ChanEst, used for Debugging
 *  @param rawSrsChEst    - Pointer to intermediate SRS ChanEst, used for Debugging

 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction|Length        |Type          |Data Format |Valid Range |Units |
 *  |              |Standard |         |              |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |iSig_Start    |         |input    |1             |Real          |(1:15:0)    |[0:100]     |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |numSig        |         |input    |1             |Real          |(1:15:0)    |[0:100]     |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |cyclicShift   |         |input    |1             |Real          |(1:15:0)    |[0:100]     |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |aEstSig       |         |pInput   |ChanEstPar.   |pComplex Array|(1:0:15)    |Full Range  |      |
 *  |              |         |         |length_aEstSig|              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |rSrs          |         |pInput   |ChanEstPar.   |pComplex Array|(1:0:15)    |Full Range  |      |
 *  |              |         |         |length_rSrs   |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |df            |         |input    |1             |Real          |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Fs            |         |input    |1             |Real          |(0:32:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Nul_symb      |         |input    |1             |Real          |(1:15:0)    |7           |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Nrb_sc        |         |input    |1             |Real          |(1:15:0)    |12          |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |NusedE        |         |input    |1             |Real          |(1:15:0)    |[0:1200]    |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Nfft          |         |input    |1             |Real          |(1:15:0)    |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |start_address |         |input    |1             |Real          |(1:15:0)    |[0:100]     |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |islot         |         |input    |1             |Real          |(1:15:0)    |[0:19]      |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Nul_rb        |         |input    |1             |Real          |(1:15:0)    |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |length_rSrs   |         |input    |1             |Real          |(1:15:0)    |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |srsChEst      |         |pOutput  |ChanEstPar.   |pComplex Array|(1:0:15)    |Full Range  |      |
 *  |              |         |         |length_rSrs   |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |chEst         |         |pOutput  |ChanEstPar.   |pComplex Array|(1:0:15)    |Full Range  |      |
 *  |              |         |         |length_rSrs   |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |rawSrsChEst   |         |pOutput  |ChanEstPar.   |pComplex Array|(1:0:15)    |Full Range  |      |
 *  |              |         |         |length_rSrs   |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimatorSrs              |
 *  ---------------------------------------------------------
 *  |Input Streams       |aEstSig,rSrs                      |
 *  ---------------------------------------------------------
 *  |Output Streams      |srsChEst,chEst,rawSrsChEst        |
 *  ---------------------------------------------------------
 *  |Config Parameters   |iSig_Start,numSig,ChanEstPar,     |
 *  |                    |cyclicShift,df,Fs,Nul_symb,Nrb_sc,|
 *  |                    |NusedE,Nfft,start_address,islot,  |
 *  |                    |length_rSRS                       |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * SRS channel estimation algorithm, per user processing;<BR>
 *
 * 1) Get raw channel estimate. i.e. <BR>
 *    - IF single user (numSig==1) <BR>
 *     - For the frequency band allocated to this user, do direct calculation of channel estimate at each pilot
 *       point. This channel estimate is calculated by dividing the received IQ sample
 *       by the known pilot data symbol value.<BR>
 *    - ELSE if CDM group of users<BR>
 *     - For the frequency band allocated to this group of users, calculate the initial channel estimate at each pilot
 *       point by dividing the received IQ sample by the known pilot data symbol values for this group with
 *       cyclic shift = 0<BR>
 *     - Separate the channel estimates for each user in the time domain<BR>
 *    - END
 * 2) Do fine timing adjustment to remove the phase rotation with frequency.<BR>
 * 3) Then perform a least squares noise averaging and estimate the SNR in these channel estimates<BR>
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.5.3, "Sounding Reference Signal"<BR>
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_ChEstimatorSrs(S16 iSig_Start, S16 numSig, S16 *cyclicShift, S16 *aEstSig, S16 *rSrs,
                          S16 df, U32 Fs, S16 Nrb_sc, S16 NusedE, S16 Nfft,
                          S16 start_address, S16 islot, S16 Nul_rb, S16 length_rSrs,
                          S16 *chEst, S16 *rawSrsChEst, S16 *pScaleComputeErr_Tab,
                          PBSRXCHMeas pSrsChMeas, S16 *IDFT_Table_Array)
{
    S16 i;
    S16 timeAdjust, index;

    S16 phaseOffset;
    S32 Err[2]; // Real and Imag

//    S16 N;
//    S16 expo_norm1;
    S16 Ndft;
    //S16 Ntime;
    S16 Nsrsrb_sc;
    S16 Msrs_rb;
    S16 Msrs_sc;
    S16 isig; //, n0;
    S16 dummyNoffset, dummy_expo;


    S16 *idftRawSrsChEst;
    S16 *sigImpResp;
    S16 ifft_exp, fft_exp;
    S16 n0;
    S16 Ntime;

    S32 *IDFT_temp;
    S16 DelayOffset;


    //init variables
    Ndft = length_rSrs;
    Ntime = shr(Ndft,3); //since Ndft is always an integer multiple of 24, Ntime is always an integer
    Nsrsrb_sc = Nrb_sc / 2; //Number of subcarriers in a ResourceBlock is divided by 2 since SRS is sent on interleaved carriers
    Msrs_rb = Ndft / Nsrsrb_sc; //number of resource blocks covered by this SRS.
    Msrs_sc = Msrs_rb * Nsrsrb_sc; //number of subcarriers covered by this SRS

    // allocate memory
    IDFT_temp = (S32 *) &(IDFT_Table_Array[3000]);
    idftRawSrsChEst = (S16 *) &(IDFT_temp[1000]);
    sigImpResp = (S16 *) &(idftRawSrsChEst[2 * Msrs_sc]);

    // 1) Get raw channel estimate
    // - Correlate with conjugate of cazac sequence to get raw channel estimate
    //  rawSrsChEst = aEstSig .* conj(rSrs);


#ifdef CEVA_INTRINSICS

    ComputeComplexConjMult_A(aEstSig, rSrs, rawSrsChEst, length_rSrs);

    // Save the raw channel estimate for later SNR calculation
//   memcpy(chEst, rawSrsChEst, 2*length_rSrs*sizeof(S16));

#else   //CEVA_INTRINSICS
    ComputeComplexConjMult(aEstSig, rSrs, rawSrsChEst, length_rSrs);

    // Save the raw channel estimate for later SNR calculation
//    memcpy(chEst, rawSrsChEst, 2*length_rSrs*sizeof(S16));

#endif //CEVA_INTRINSICS


    // CDM SRS with time-domain channel estimator

    // - Do idft (once for all users); transfer channel to time domain
    IDFT_Ceva_fx(rawSrsChEst, Ndft, idftRawSrsChEst, &ifft_exp, IDFT_Table_Array,IDFT_temp);
    
    // Save the time domain channel estimate for later noise calculation
   // memcpy(timeDomainNoise, idftRawSrsChEst, 2*length_rSrs*sizeof(S16));

    for (isig = 0; isig < numSig; isig++)
    {

        // - Demux each user by the relevant segment in the time domain corresponding to the cyclic shift value
        // - shift the samples for each user to the left hand side, clear all other elements to zero
        //if(numSig > 1)
        //    n0 = (Ndft - Ntime * cyclicShift[iSig_Start + isig]) % Ndft; //starting time sample for this signal
        //else
        //    n0 = 0;

        n0 = (Ndft - Ntime * cyclicShift[iSig_Start + isig]) % Ndft; //starting time sample for this signal
        //for (i = 0; i < 2 * Ndft; i++)
        //{
        //    sigImpResp[i] = 0; //init vector to zeros before adding the time samples
        //}

        memset(sigImpResp, 0, 4 * Ndft);

        for (i = 0; i < 2 * Ntime; i++)
        {
            sigImpResp[i] = idftRawSrsChEst[2 * n0 + i];
        }

        // - dft (separately for all users)
        DFT_Ceva_fx(sigImpResp, Ndft, chEst + 2 * ((isig)* Msrs_sc), &fft_exp, IDFT_Table_Array,IDFT_temp);

        // scale back the data in frequency domain
        for(i = 0; i< 2*length_rSrs; i++)
        {
            #ifndef CEVA_INTRINSICS
                chEst[2 * ((isig) * Msrs_sc)+i] = shift_r(chEst[2 * ((isig)  * Msrs_sc)+i], fft_exp + ifft_exp);
            #else
                chEst[2 * ((isig)  * Msrs_sc)+i] = shr_r(chEst[2 * ((isig)  * Msrs_sc)+i], -(fft_exp + ifft_exp));
            #endif
        }

    }

  // debug output to files
    #if 0
    {
        FILE *fp;
        fp = (FILE *) fopen( "srs_data_after_conj.dat", "at+");
        for ( i=0; i < length_rSrs; i++ )
        {
             fprintf( fp, "%d %d\n",  rawSrsChEst[2 * i], rawSrsChEst[2 * i+1]);
        }
        fclose(fp);

        fp = (FILE *) fopen( "srs_data_ch_in_time_domain.dat", "at+");
        for ( i=0; i < length_rSrs; i++ )
        {
             fprintf( fp, "%d %d\n",  idftRawSrsChEst[2 * i], idftRawSrsChEst[2 * i+1]);
        }
        fclose(fp);

        fp = (FILE *) fopen( "srs_data_ch_in_freq_domain.dat", "at+");
        for ( i=0; i < length_rSrs*numSig; i++ )
        {
             fprintf( fp, "%d %d\n",  chEst[2 * i], chEst[2 * i+1]);
        }
        fclose(fp);

        
        fp = (FILE *) fopen( "srs_data_input.dat", "at+");
        for ( i=0; i < length_rSrs; i++ )
        {
             fprintf( fp, "%d %d\n",  aEstSig[2 * i], aEstSig[2 * i+1]);
        }
        fclose(fp);

        
        fp = (FILE *) fopen( "srs_data_pilot.dat", "at+");
        for ( i=0; i < length_rSrs; i++ )
        {
             fprintf( fp, "%d %d\n",  rSrs[2 * i], rSrs[2 * i+1]);
        }
        fclose(fp);


    }
    #endif
 
    // timing advance calculation
    for (isig = 0; isig < numSig; isig++)
    {
#ifdef CEVA_INTRINSICS
        Compute_phase_error_A(chEst+2*((isig) *Msrs_sc), Err, Msrs_rb, Msrs_sc, pScaleComputeErr_Tab);
#else
        Compute_phase_error(chEst+2*((isig) *Msrs_sc), Err, Msrs_rb, Msrs_sc, pScaleComputeErr_Tab);
#endif

        phaseOffset = get_angle_fx(Err[0], Err[1]);

        Compute_DerotationSRS(Fs, phaseOffset, df, Nfft, Msrs_sc, NusedE,
                           start_address, islot, &dummyNoffset, &dummy_expo);

        index = 0;
        switch (Nul_rb)
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
        pSrsChMeas[iSig_Start+isig].TimingAdvance = timeAdjust;

    }

    // for DFT channel estimation, the edge subcarriers are not accurate; so we replace the left edge RB and right edge RB with center RBs
    for (isig = 0; isig < numSig; isig++)
    {
        memcpy(chEst + 2 * (isig)  * Msrs_sc, chEst + 2 * (isig)  * Msrs_sc + 12, 12*sizeof(S16));
        memcpy(chEst + 2 * (isig)  * Msrs_sc + (Msrs_sc-6)*2, chEst + 2 * (isig)  * Msrs_sc - 12 + (Msrs_sc-6)*2, 12*sizeof(S16));
    }
    
    return;
}

