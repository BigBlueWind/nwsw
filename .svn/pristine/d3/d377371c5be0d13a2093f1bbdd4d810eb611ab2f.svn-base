//-------------------------------------------------------------------------------------------
/** @file rx_mrcom_c.c
 *
 * @brief Maximum-ratio combining function for the receiver.
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"

#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
//#include "heaps.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

/******************************************************************************
 RX_MRCOM (Rx Maximum-Ratio-Combining Function)
 ******************************************************************************

 RX_MRCOM implements the maximum-ratio combining function for the
 receiver.  The exact function performed is described below.

 For every complex data symbol that needs to be demodulated:

 For each Rx antenna extract the corresponding complex receive vector from
 the FFT output array (RXIN) and the complex channel estimate from the channel
 estimate array (CHIN).  Table OFFTAB, that contains the relative complex
 elemental offsets, is used to control the extraction.

 Compute and output the average summation of the Rx*complex conjugate[Ch]
 for all of the Rx antennas.

 Compute and output the average summation of the channel estimate magnitudes
 squared for all of the Rx antennas.

 The calling format for RX_MRCOM is shown below.

 RX_MRCOM RXIN, CHIN, RXOUT, CHOUT, NOUT, BLKSIZ, NANT, OFFTAB

 Where:  RXIN = Complex receive vector input array (organized as
 real part, imaginary part).  The first BLKSIZ complex
 samples are for Rx antenna #0, the next BLKSIZ complex
 samples are for Rx antenna #1, ...

 CHIN = Complex channel estimate input array (organized as
 real part, imaginary part).  The first BLKSIZ complex
 samples are for Rx antenna #0, the next BLKSIZ complex
 samples are for Rx antenna #1, ...

 RXOUT = Final maximum-ratio combined receive vector output
 array (organized as real part, imaginary part).

 CHOUT = Average channel estimate power output array.

 NOUT = Number of complex receive vectors to be combined.

 BLKSIZ = Number of complex elements in arrays RXIN and CHIN for
 each Rx antenna.

 NANT = Number of Rx antennas.

 OFFTAB = Table containing complex elemental relative offsets
 used to extract data symbol vectors and channel
 estimates.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Maximum-ratio combining function for the receiver.
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param RXIN     Complex receive vector input array (organized as
 *                  real part, imaginary part).  The first BLKSIZ complex
 *                  samples are for Rx antenna #0, the next BLKSIZ complex
 *                  samples are for Rx antenna #1, ...
 *
 *  @param CHIN     Complex channel estimate input array (organized as
 *                  real part, imaginary part).  The first BLKSIZ complex
 *                  samples are for Rx antenna #0, the next BLKSIZ complex
 *                  samples are for Rx antenna #1, ...
 *
 *  @param RXOUT    Final maximum-ratio combined receive vector output
 *                  array (organized as real part, imaginary part).
 *
 *  @param CHOUT    Average channel estimate power output array.
 *
 *  @param NOUT     Number of complex receive vectors to be combined.
 *
 *  @param BLKSIZ   Number of complex elements in arrays RXIN and CHIN for
 *                  each Rx antenna.
 *
 *  @param NANT     Number of Rx antennas.
 *
 *  @param isym     Current Symbol Number
 *
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in   |Direction  |Length       |Type           |Data Format |Valid Range |Units  |
 *  |         |Standard  |           |             |               |            |            |       |
 *  --------------------------------------------------------------------------------------------------
 *  |RXIN     |          |pInput     |BLKSIZ*NANT  |pComplex Array |(1:0:15)    |            |       |
 *  --------------------------------------------------------------------------------------------------
 *  |CHIN     |          |pInput     |BLKSIZ*NANT  |pComplex Array |(1:0:15)    |            |       |
 *  --------------------------------------------------------------------------------------------------
 *  |RXOUT    |          |pOutput    |NOUT         |pComplex Array |(1:0:31)    |            |       |
 *  --------------------------------------------------------------------------------------------------
 *  |CHOUT    |          |pOutput    |NOUT         |pReal Array    |(1:0:31)    |            |       |
 *  --------------------------------------------------------------------------------------------------
 *  |NOUT     |          |Input      |1            |real           |(1:15:0)    |            |       |
 *  --------------------------------------------------------------------------------------------------
 *  |BLKSIZ   |          |Input      |1            |real           |(1:15:0)    |            |       |
 *  --------------------------------------------------------------------------------------------------
 *  |NANT     |          |Input      |1            |real           |(1:15:0)    |[1,2,4]     |       |
 *  --------------------------------------------------------------------------------------------------
 *  |isym     |          |Input      |1            |signed short   |(1:15:0)    |[0,13]      |       |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------
 *  |Block Name          |RX_MRCOM_C           |
 *  --------------------------------------------
 *  |Input Streams       |RXIN, CHIN           |
 *  --------------------------------------------
 *  |Output Streams      |RXOUT, CHOUT         |
 *  --------------------------------------------
 *  |Config Parameters   |NOUT, NANT, isym     |
 *  --------------------------------------------
 *  |Status Parameters   |none                 |
 *  --------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  For every complex data symbol that needs to be demodulated:
 *
 *    For each Rx antenna extract the corresponding complex receive vector from
 *    the FFT output array (RXIN) and the complex channel estimate from the channel
 *    estimate array (CHIN).  Table OFFTAB, that contains the relative complex
 *    elemental offsets, is used to control the extraction.
 *
 *    Compute and output the average summation of the Rx*complex conjugate[Ch]
 *    for all of the Rx antennas.
 *
 *    Compute and output the average summation of the channel estimate magnitudes
 *    squared for all of the Rx antennas.
 *
 * <5> Cycle Count Formula:
 * Two antennas: 5*(Number-of-subcarriers) + 90 <BR>
 * Four antennas: 9*(Number-of-subcarriers) + 90
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.7, Sec.5.3, "Uplink: Physical Uplink Shared Channel"
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
void rx_mrcom_c(S16 *pRxIn, S16 *pChEstIn, S32 *pRxOut, S32 *pChPow, S32 num_out,
                S32 ant_offset, S32 num_ant)
{
    //*****************************************************************************
    S32 i, j;
    S16 rxR, rxI, chR, chI, scChR, scChI;
    S16 scf;
    S32 sumChMag, sumRxR, sumRxI, offset;

    // Define 1/2N averaging scale factor table (maximum N = 8)
    // Note, the extra 1/2 scaling is to prevent 32-bit summation overflows
    S16 ScaleTab[8] = { 16384, 8192, 5461, 4096, 3276, 2730, 2340, 2048 };

    //printf("!!!!!!!!!!!!!!!!!!!! RX_MRCOM !!!!!!!!!!!!!!!!!!!!\n");
    scf = ScaleTab[num_ant-1];

    // Do for each complex combined Rx output
    for (i = 0; i < num_out; i++)
    {
        //Initialize Channel Power, (Rx input)*(channel) (real and imag part)
        sumChMag = 0;
        sumRxR   = 0;
        sumRxI   = 0;

        // Do for each Rx antenna
        for (j = 0; j < num_ant; j++)
        {
            //Calculate offset into RxIn and ChEst Buffers for each RxAnt
            offset = 2 * j * ant_offset + 2*i;

            rxR = pRxIn[offset];         //RxIn real part
            rxI = pRxIn[offset + 1];     //RxIn imag part
            chR = pChEstIn[offset];      //ChEst real part
            chI = pChEstIn[offset + 1];  //ChEst imag part

            scChR = mult(chR, scf);      //Scale ChEst to divide by num RxAnt
            scChI = mult(chI, scf);      //Scale ChEst to divide by num RxAnt

            // Compute summation of Rx * conj scaled chan est (real part)
            sumRxR = L_mac(sumRxR, rxR, scChR);
            sumRxR = L_mac(sumRxR, rxI, scChI);

            // Compute summation of Rx * conj scaled chan est (real part)
            sumRxI = L_msu(sumRxI, rxR, scChI);
            sumRxI = L_mac(sumRxI, rxI, scChR);
            
            // Compute scaled summation of channel estimate magnitudes ** 2
            sumChMag = L_mac(sumChMag, chR, scChR);
            sumChMag = L_mac(sumChMag, chI, scChI);
        }

        // Output maximum ratio-combined value (real and imag part)
        pRxOut[2 * i]     = sumRxR;
        pRxOut[2 * i + 1] = sumRxI;
        
        // Output average channel estimate power
        pChPow[i] = sumChMag;
    }

    return;
}
