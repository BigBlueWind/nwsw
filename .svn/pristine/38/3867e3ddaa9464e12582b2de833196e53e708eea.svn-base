//-------------------------------------------------------------------------------------------
/** @file bs_rx_SCFDMA_Demodulator.c
 *
 * @brief Base Station Rx, SC-FDMA demodulator for one symbol, CP already removed
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
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
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#endif

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Generate minus-half-carrier frequency shift to this symbol before FFT Demodulator input, with CP already removed
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *  @param adc(t) = half-carrier shifted modulator output<BR>
 *
 * \f$adc[t] = \{sum_{k=-floor(NusedE/2)}^{ceil(NUsedE/2)-1}{a[k+floor(NusedE/2)]}\}\;\;*\;\;e^{\frac{2j\pi df}{2(t-Tcpf)}}\f$
 *
 *where
 * - t     = time elapsed since the beginning of this OFDM symbol, with 0 < t < Tsym
 * - Tsym  = (Ncpsam + Nfft).Tsam = time duration of symbol, including cyclic prefix
 * - Tcpf  = time duration of cyclic prefix fragment remaining after CP removal, prior to calling this function
 * - NusedE = Nused - 1 = Nrb . Nrb_sc = subcarriers used/occupied with energy, i.e. not including the dc subcarrier 
 * - a(k)  = complex number; the data to be transmitted on the subcarrier whose frequency offset index is
 *         k, during the subject OFDM symbol. It specifies a point in a QAM constellation. In subchannelized
 *         transmissions, a(k) is zero for all unallocated subcarriers
 *
 *  @param mhcs(t) = exp(-j.2.pi.df/2.t) = minus-half-carrier shift array<BR>
 *
 *  \f$mhcs[t] = \{sum_{k=-floor(NusedE/2)}^{ceil(NUsedE/2)-1}{a[k+floor(NusedE/2)]}\;\;*\;\;e^{2j\pi kdf(t-Tcpf)}\}\f$
 *
 *  @return adcMhcs(t)  = adc(t).mhcs(t)  = minus-half-carrier shifted demodulator input
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name    |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  -----------------------------------------------------------------------------------------------------
 *  |mhcs    |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  |expo    |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  |length  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |GenHalfCyclicShift          |
 *  ---------------------------------------------------
 *  |Inputs Stream       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Generate minus-half-carrier frequency shift to this symbol before FFT Demodulator input, with CP already removed
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * DOXYGEN_TO_DO
 * 
 * \ingroup DOXYGEN_TO_DO
 *
 **/
//-------------------------------------------------------------------------------------------
void GenHalfCyclicShift(S16 *mhcs, S16 expo, S16 length)
{
    S32 i, k, l;
    S16 scale;

    scale = shr((S16) (4096), (S16) (expo + 1));
    // Init array used for minus-half-carrier shift
    for (i = 0; i < length; i++)
    {
        //half carrier shift array, for every symbol in slot, where the CP has already been removed
        //exp(-j*PI*phase*it*Tsam) where phase = PI*df becomes exp(-j*2*PI*it/(2*Nfft))

        k = i;
        if (i >= 2 * length)
            k = i % (2 * length);

        l = k * scale;
        if (l < 2048)
        {
            mhcs[2 * i] = Table_FFT16b_4096[2 * l];
            mhcs[2 * i + 1] = Table_FFT16b_4096[2 * l + 1];
        }
        else
        {
            l = l - 2048;
            mhcs[2 * i] = negate(Table_FFT16b_4096[2 * l]);
            mhcs[2 * i + 1] = negate(Table_FFT16b_4096[2 * l + 1]);
        }
    }
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Apply minus-half-carrier frequency shift to this symbol before FFT Demodulator input, with CP already removed
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *  @param adc(t) = half-carrier shifted modulator output<BR>
 *
 * \f$adc[t] = \{sum_{k=-floor(NusedE/2)}^{ceil(NUsedE/2)-1}{a[k+floor(NusedE/2)]}\}\;\;*\;\;e^{\frac{2j\pi df}{2(t-Tcpf)}}\f$
 *
 *where
 * - t     = time elapsed since the beginning of this OFDM symbol, with 0 < t < Tsym
 * - Tsym  = (Ncpsam + Nfft).Tsam = time duration of symbol, including cyclic prefix
 * - Tcpf  = time duration of cyclic prefix fragment remaining after CP removal, prior to calling this function
 * - NusedE = Nused - 1 = Nrb . Nrb_sc = subcarriers used/occupied with energy, i.e. not including the dc subcarrier
 * - a(k)  = complex number; the data to be transmitted on the subcarrier whose frequency offset index is
 *         k, during the subject OFDM symbol. It specifies a point in a QAM constellation. In subchannelized
 *         transmissions, a(k) is zero for all unallocated subcarriers
 *
 *  @param mhcs(t) = exp(-j.2.pi.df/2.t) = minus-half-carrier shift array<BR>
 *
 *  \f$mhcs[t] = \{sum_{k=-floor(NusedE/2)}^{ceil(NUsedE/2)-1}{a[k+floor(NusedE/2)]}\;\;*\;\;e^{2j\pi kdf(t-Tcpf)}\}\f$
 *
 *  @return adcMhcs(t)  = adc(t).mhcs(t)  = minus-half-carrier shifted demodulator input
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name    |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  -----------------------------------------------------------------------------------------------------
 *  |adc     |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  |mhcs    |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  |adcMhcs |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Remove_HalfCyclicShift      |
 *  ---------------------------------------------------
 *  |Inputs Stream       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Apply minus-half-carrier frequency shift to this symbol before FFT Demodulator input, with CP already removed
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * DOXYGEN_TO_DO
 *
 * \ingroup DOXYGEN_TO_DO
 *
 **/
//-------------------------------------------------------------------------------------------
void Remove_HalfCyclicShift(S16 *adc, S16 *mhcs, S16 *adcMhcs, S16 length)
{
    S32 i;
    S32 L_temp1, L_temp2;
    S32 L_temp3, L_temp4;
    S16 temp1, temp2, temp3, temp4;
    S16 temp5, temp6, temp7, temp8;

    //Apply minus-half-carrier shift
    for (i = 0; i < (S32) (length >> 1); i++)
    {
        temp1 = adc[4 * i];
        temp2 = mhcs[4 * i];
        temp3 = mhcs[4 * i + 1];
        temp4 = adc[4 * i + 1];
        temp5 = adc[4 * i + 2];
        temp6 = mhcs[4 * i + 2];
        temp7 = mhcs[4 * i + 3];
        temp8 = adc[4 * i + 3];

        L_temp1 = L_mult(temp1, temp2);
        L_temp2 = L_mult(temp1, temp3);

        L_temp3 = L_mult(temp5, temp6);
        L_temp4 = L_mult(temp5, temp7);

        adcMhcs[4 * i] = msu_r(L_temp1, temp4, temp3);
        adcMhcs[4 * i + 1] = mac_r(L_temp2, temp4, temp2);

        adcMhcs[4 * i + 2] = msu_r(L_temp3, temp8, temp7);
        adcMhcs[4 * i + 3] = mac_r(L_temp4, temp8, temp6);
    }

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Swap FFT output so that high and low frequencies are in their proper places
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name    |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  -----------------------------------------------------------------------------------------------------
 *  |input   |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |output  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  |length  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  |offset  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |SwapFFT_Out                 |
 *  ---------------------------------------------------
 *  |Inputs Stream       |input                       |
 *  ---------------------------------------------------
 *  |Output Stream       |output                      |
 *  ---------------------------------------------------
 *  |Config Parameters   |length, offset              |
 *  ---------------------------------------------------
 *  |Status Parameters   |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Swap FFT output so that high and low frequencies are in their proper places
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * DOXYGEN_TO_DO
 *
 * \ingroup DOXYGEN_TO_DO
 *
 **/
//-------------------------------------------------------------------------------------------
void SwapFFT_Out(S16 *input, S16 *output, S32 length, S32 offset, S16 *expo_mat)
{
    S32 k;

    //store detected resource grid from selected raw demodulated values
    for (k = 0; k < length; k++)
    {
        // Get low frequencies
        output[2 * k] = input[2 * k + offset];
        output[2 * k + 1] = input[2 * k + offset + 1];

        // Get Upper frequencies
        output[2 * k + 2 * length] = input[2 * k];
        output[2 * k + 2 * length + 1] = input[2 * k + 1];
    }

    output[4 * length] = *expo_mat;
}
