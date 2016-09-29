//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL bs_tx_OFDM_Modulator.c
 *
 * @brief LTE Base Station Transmit OFDM Modulator Implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.37 $
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

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "papr_rcf.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   ifft_in DOXYGEN_TO_DO
 *  @param   a DOXYGEN_TO_DO
 *  @param   row_a DOXYGEN_TO_DO
 *  @param   col_a DOXYGEN_TO_DO
 *  @param   isym DOXYGEN_TO_DO
 *  @param   mi DOXYGEN_TO_DO
 *  @param   Nfft DOXYGEN_TO_DO
 *  @param   Ncpsam DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ifft_in       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |row_a         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |col_a         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |isym          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |mi            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Nfft          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Ncpsam        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void SetupIFFT_Bufs(S16* ifft_in, S16* a, S16 row_a, S16 col_a, S16 isym, S16 mi, S16 Nfft,
                    S16 Ncpsam)
{
    S32 i;

    for (i = 0; i < 2 * Nfft; i++)
    {
        ifft_in[i] = 0;
    }

    // The continous time domain reconstruction of an OFDM symbol using IDFT
    // will be simultated in digital domain using IFFT
    // First, we need to construct the input to the IFFT
    // Skip DC , Put positive freq, upper part of a first
    for (i = 0; i < row_a / 2; i++)
    {
        // Real part
        ifft_in[2 * i + 2] = a[row_a + 2 * i];
        // Imag Part
        ifft_in[2 * i + 3] = a[row_a + 2 * i + 1];
    }

    // Put Negative freq, lower part of a last
    for (i = 0; i < row_a / 2; i++)
    {
        // Real part
        ifft_in[2 * i + 2 * (Nfft - row_a / 2)] = a[2 * i];
        // Imag Part
        ifft_in[2 * i + 2 * (Nfft - row_a / 2) + 1] = a[2 * i + 1];

        //  DEBUG PRINT
        // printf("input_fft  %f %f\n", \
// ifft_in[2*i+2*(Nfft-length_a/2)]/8192.0, \
// ifft_in[2*i+1+2*(Nfft-length_a/2)]/8192.0);
        // getchar();
    }

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   ptr_dac_samp DOXYGEN_TO_DO
 *  @param   ifft_out DOXYGEN_TO_DO
 *  @param   Nfft DOXYGEN_TO_DO
 *  @param   Ncpsam DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ptr_dac_samp  |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ifft_out      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Nfft          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Ncpsam        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void AddCP(S16* ptr_dac_samp, S16* ifft_out, S16 Nfft, S16 Ncpsam)
{
    S32 i;

    for (i = 0; i < 2 * Nfft; i++)
    {
        ptr_dac_samp[i + 2 * Ncpsam] = ifft_out[i];
        //  DEBUG PRINT
        //printf("OUTPUT %hd %f\n", i/2, ifft_out[i]*pow(2.0, -14.0+ifft_exp));
    }

    // Add cyclic prefix
    // copy last Ncpsam samples of Nfft FFT samples to beginning of sequence
    for (i = 0; i < 2 * Ncpsam; i++)
    {
        ptr_dac_samp[i] = ptr_dac_samp[2 * Nfft + i];
    }

    //  DEBUG PRINT
    //printf("samp %hd %f %f\n", isym, dac[2*(li+mi*count_samp/P)]/16384.0,
    //dac[2*(li+mi*count_samp/P)+1]/16384.0);

    return;
}

