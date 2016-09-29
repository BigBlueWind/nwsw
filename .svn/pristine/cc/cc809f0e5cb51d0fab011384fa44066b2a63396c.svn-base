//-------------------------------------------------------------------------------------------
/** @file rx_Demapper.c
 *
 * @brief Receiver, Constellation demapper. Calculate hard and soft decisions
 * @author Mindspeed Technologies
 * @version $Revision: 1.39.8.1 $
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

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#include <asm-insn.h>
#include "global.h"
extern U8 *DataBuff_Bank1;
extern U8 *DataBuff_Bank2;
extern U8 *DataBuff_Bank3;
#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief Receiver, Constellation demapper. Calculate soft decisions
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param aEst1_fx - Pointer to resource element grid after MRC
 *  @param Qm       - Modulation Type: 1:BPSK, 2:QPSK, 4:QAM16, 6:QAM64
 *  @param bDetSoft - Pointer demapped soft decisions
 *  @param NusedE   - Number of I/Q sample to demap
 *  @param chanType - Physical Channel Type
 *  @param expo     - Number of shifts to normalize aEst1_fx.
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction|Length     |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |         |           |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  |aEst1_fx   |         |pInput   |Qm*NusedE  |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |Qm         |         |Input    |1          |Real          |(1:15:0)    |[0,1,2,4,6] |      |
 *  ---------------------------------------------------------------------------------------------
 *  |bDetSoft   |         |pOutput  |Qm*NusedE  |pReal Array   |(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |NusedE     |         |Input    |1          |Real          |(1:15:0)    |[1:8000]    |      |
 *  ---------------------------------------------------------------------------------------------
 *  |chanType   |         |Input    |1          |Real          |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |expo       |         |Input    |1          |Real          |(1:15:0)    |[-16:16]    |      |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------------
 *  |Block Name          |rx_Demapper_fx                          |
 *  ---------------------------------------------------------------
 *  |Input Streams       |aEst1_fx                                |
 *  ---------------------------------------------------------------
 *  |Output Streams      |bDetSoft                                |
 *  ---------------------------------------------------------------
 *  |Config Parameters   |Qm, NusedE, NShift,chanType,expo        |
 *  ---------------------------------------------------------------
 *  |Status Parameters   |none                                    |
 *  ---------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Constellation demapper. Calculate hard and soft decisions. <BR>
 * Will do BPSK, QPSK, 16QAM, or 64QAM DeMapping.<BR>
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.7, Sec.5.3, "Uplink: Physical Uplink Shared Channel"
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
void rx_Demapper_fx(S16 aEst1_fx[], S16 Qm, S8* scrSeq, S8 *bDetSoft, S16 NusedE, S16 chanType, S16 expo)
{
    S32 I;

    if (Qm == 0)
    {
        for (I = 0; I < NusedE; I += 6)
        {
            bDetSoft[I] = 0;
        }
    }
    else if (Qm == 1)
    {
        Demod_BPSK(aEst1_fx, scrSeq, bDetSoft, (S32) (NusedE * 2));
    }
    else if (Qm == 2)
    {
        Demod_QPSK(aEst1_fx, scrSeq, bDetSoft, (S32) (NusedE * 2), expo);
    }
    else if (Qm == 4)
    {
#ifdef CEVA_INTRINSICS
        Demod_QAM16_A(aEst1_fx, scrSeq, bDetSoft, (S32) (NusedE * 2), expo);
#else
        Demod_QAM16(aEst1_fx, scrSeq, bDetSoft, (S32) (NusedE * 2), expo);
#endif
    }
    else if (Qm == 6)
    {
#ifdef CEVA_INTRINSICS
        Demod_QAM64_A(aEst1_fx, scrSeq, bDetSoft, (S32) (NusedE * 2), expo);
#else
        Demod_QAM64(aEst1_fx, scrSeq, bDetSoft, (S32) (NusedE * 2), expo);
#endif
    }
}

/******************************************************************************
 Demod_BPSK (LTE BPSK Demodulator)
 ******************************************************************************

 Demod_BPSK implements the BPSK demodulator for the LTE receiver.
 In addition to determining the hard data bits, the routine also computes a
 quality measure for each data bit.  These quality measures are required for
 optimum soft-decision decoding.  The LTE BPSK constellation is shown below.

 X     Y     B0
 --------------
 A    A     0
 -A    A     1

 Where: X =      X coordinate
 Y =      Y coordinate
 A =      1/SQRT(2)
 B0 =     data bit

 The inputs to the demodulator are an array of complex receive symbol vectors
 (RXIN[]).  It is assumed that the receive symbol vectors have already been
 adjusted using their channel estimates and maximum-ratio combined
 (see routine rx_MrcDetector_fx).  However, the magnitudes of the receive symbol
 vectors are still weighted by the overall combined channel estimate powers.
 The determination of the hard data bits is easy.  For a given receive symbol
 vector, B0 = SIGN[real part + imag part].

 The magnitude of the summation of the real and imaginary parts provides a
 good quality measure for this data bit.  The quality measure is computed as
 a 16-bit value with the sign bit inverted being used as the actual B0 data bit.
 The combined value B0VAL is computed as shown below.

 B0VAL = -RXIN (real part + imag part)/2 (normalized)

 Where:  RXIN =  complex receive symbol vector

 The outputs for the demodulator are an array of B0VAL values.  The calling
 format for Demod_BPSK is shown below.

 Demod_BPSK RXIN, OUT, N, NSHFT

 Where:  RXIN =  Complex receive vector input array (organized as
 real part, imaginary part).
 OUT =   Output data array containing B0VALs.
 N =     Number or receive vectors to demodulate.
 NSHFT = Number of shifts required to normalize RXIN array.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   RXIN[] DOXYGEN_TO_DO
 *  @param   *bDetSoft DOXYGEN_TO_DO
 *  @param   iLength DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |RXIN[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*bDetSoft     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |iLength       |              |              |              |              |              |              |
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
void Demod_BPSK(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength)
{
    S32 I, J, K;
    S16 AC;
    U8 scrBit;
    U8 mask = 0x80;

    K = 0;
    for (I = 0, J = 0; I < iLength; I += 2, J++)
    {
        AC = add(RXIN[I], RXIN[I + 1]);
        AC = negate(AC) >> 9;

        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            AC = negate(AC);
        }
        bDetSoft[J] = (S8) AC;

        if(!mask)
        {
            K++;
            mask = 0x80;
        }
    }
    return;
}

/******************************************************************************
 Demod_QPSK (QPSK Demodulator)
 ******************************************************************************

 Demod_QPSK implements the QPSK demodulator for the LTE receiver.
 In addition to determining the hard data bits, the routine also computes a
 quality measure for each data bit.  These quality measures are required for
 optimum soft-decision decoding.  The LTE QPSK constellation is shown below.

 X     Y     B0     B1
 ---------------------
 A     A     0      0
 A    -A     0      1
 -A     A     1      0
 -A    -A     1      1

 Where: X =      X coordinate
 Y =      Y coordinate
 A =      1/SQRT(2)
 B0 =     1st data bit
 B1 =     2nd data bit

 The inputs to the demodulator are an array of complex receive symbol vectors
 (RXIN[]).  It is assumed that the receive symbol vectors have already been
 adjusted using their channel estimates and maximum-ratio combined
 (see routine ).  However, the magnitudes of the receive symbol
 vectors are still weighted by the overall combined channel estimate powers.
 The determination of the hard data bits is easy.  For a given receive symbol
 vector, B0 = SIGN[real part] and B1 = SIGN[imaginary part].

 The decision boundary for B0 is the Y axis.  Therefore, the magnitude
 of the receive symbol's real part provides a good quality measure for
 this data bit.  The quality measure is computed as a 16-bit value
 with the sign bit inverted being used as the actual B0 data bit.  The combined
 value B0VAL is computed as shown below.

 B0VAL = -RXIN real part (normalized)

 Where:  RXIN =  complex receive symbol vector

 The decision boundary for B1 is the X axis.  Therefore, the magnitude
 of the receive symbol's imaginary part provides a good quality measure for
 this data bit.  The quality measure is computed as a 16-bit value with
 the sign bit inverted being used as the actual B1 data bit.  The combined value
 B1VAL is computed as shown below.

 B1VAL = -RXIN imaginary part (normalized)

 Where:  RXIN =  complex receive symbol vector

 The outputs for the demodulator are an array of B0VAL, B1VAL pairs.  The
 calling format for Demod_QPSK is shown below.

 Demod_QPSK RXIN, OUT, N, NSHFT

 Where:  RXIN =  Complex receive vector input array (organized as
 real part, imaginary part).
 OUT =   Output data array (organized as B0VAL, B1VAL).
 N =     Number or receive vectors to demodulate.
 NSHFT = Number of shifts required to normalize RXIN array.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   RXIN[] DOXYGEN_TO_DO
 *  @param   *bDetSoft DOXYGEN_TO_DO
 *  @param   iLength DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |RXIN[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*bDetSoft     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |iLength       |              |              |              |              |              |              |
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
void Demod_QPSK(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength, S16 expo)
{
    S32 I, J, K;
    S16 expo1;
    S16 AC0, AC1;
    U8 scrBit;
    U8 mask = 0x80;

    K = 0;

    expo1 = 8 - expo;

    for (I = 0, J = 0; I < iLength; I += 2)
    {

        AC0 = shr(negate(RXIN[I]), expo1);
        AC1 = shr(negate(RXIN[I + 1]), expo1);
        
        scrBit = scrSeq[K] & mask;
        mask >>= 1;

        if(scrBit)
        {
            //Add 1 to prevent overflow when trying
            //to negate -128
            AC0 = negate(AC0 + 1);
        }

        scrBit = scrSeq[K] & mask;
        mask >>= 1;

        if(scrBit > 0)
        {
            //Add 1 to prevent overflow when trying
            //to negate -128
            AC1 = negate(AC1 + 1);
        }

        bDetSoft[J++] = (S8) AC0;
        bDetSoft[J++] = (S8) AC1;

        if(!mask)
        {
            K++;
            mask = 0x80;
        }
    }

    return;
}

/******************************************************************************
 Demod_16QAM (LTE 16-QAM Demodulator)
 ******************************************************************************

 Demod_16QAM implements the 16-QAM demodulator for the LTE receiver.
 In addition to determining the hard data bits, the routine also computes a
 quality measure for each data bit.  These quality measures are required for
 optimum soft-decision decoding.  The LTE 16-QAM constellation is shown below.


 X               Y                B0 B1 B2 B3
 --------------------------------------------
 P_16QAM_1       P_16QAM_1        0  0  0  0
 P_16QAM_1       P_16QAM_3        0  0  0  1
 P_16QAM_3       P_16QAM_1        0  0  1  0
 P_16QAM_3       P_16QAM_3        0  0  1  1

 P_16QAM_1      -P_16QAM_1        0  1  0  0
 P_16QAM_1      -P_16QAM_3        0  1  0  1
 P_16QAM_3      -P_16QAM_1        0  1  1  0
 P_16QAM_3      -P_16QAM_3        0  1  1  1

 -P_16QAM_1       P_16QAM_1        1  0  0  0
 -P_16QAM_1       P_16QAM_3        1  0  0  1
 -P_16QAM_3       P_16QAM_1        1  0  1  0
 -P_16QAM_3       P_16QAM_3        1  0  1  1

 -P_16QAM_1      -P_16QAM_1        1  1  0  0
 -P_16QAM_1      -P_16QAM_3        1  1  0  1
 -P_16QAM_3      -P_16QAM_1        1  1  1  0
 -P_16QAM_3      -P_16QAM_3        1  1  1  1

 Where: X =              X coordinate
 Y =              Y coordinate
 P_16QAM_1 =      1/SQRT(10)
 P_16QAM_3 =      3/SQRT(10)
 B0 - B3 =        data bits

 The inputs to the demodulator are an array of complex receive symbol vectors
 (RXIN[]) and their corresponding channel estimate powers (ChEst_pwr).  It is
 assumed that the receive symbol vectors have already been adjusted using
 their channel estimates and maximum-ratio combined (see routine rx_MrcDetector_fx).
 However, the magnitudes of the receive symbol vectors are still weighted by
 the overall combined channel estimate powers.  The determination of the hard
 data bits is shown below.

 B0 = COND( X < 0 )
 B1 = COND( Y < 0 )
 B2 = COND( |X| > P_16QAM_2 )
 B3 = COND( |Y| > P_16QAM_2 )

 Where: COND(X) =    0 (if X is false), 1 (if X is true)
 P_16QAM_2 =  2/SQRT(10)

 The quality measures for each bit are computed using the distances from
 their respective decision boundaries.  The quality measures are computed
 as 16-bit values with the sign bit inverted being used as the actual hard
 data bit.  The quality measures for each of the data bits are shown below.

 ------------------------------------------------------------------------------
 B0
 --

 B0VAL = -X (normalized)

 ------------------------------------------------------------------------------
 B1
 --

 B1VAL = -Y (normalized)

 ------------------------------------------------------------------------------
 B2
 --

 B2VAL = -P_16QAM_2*CHP + |X| (normalized)

 ------------------------------------------------------------------------------
 B3
 --

 B3VAL = -P_16QAM_2*CHP + |Y| (normalized)

 ------------------------------------------------------------------------------

 Where:  X =         complex receive symbol vector (real part)
 Y =         complex receive symbol vector (imaginary part)
 P_16QAM_2 = 2/SQRT(10)
 CHP =       channel power estimate

 ------------------------------------------------------------------------------

 The outputs for the demodulator are an array containing the BnVAL values.
 The calling format for Demod_16QAM is shown below.

 Demod_16QAM RXIN, CHP, OUT, N, NSHFT

 Where:  RXIN =  Complex receive vector input array (organized as
 real part, imaginary part).
 CHP =   Channel power estimate.
 OUT =   Output data array (organized as B0VAL, B1VAL, B2VAL,
 B3VAL).
 N =     Number or receive vectors to demodulate.
 NSHFT = Number of shifts needed to normalize RXIN array.

 *****************************************************************************/
#define P_16QAM_2       10362    // 20724 = 2/SQRT(10) in Q15

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   RXIN[] DOXYGEN_TO_DO
 *  @param   *bDetSoft DOXYGEN_TO_DO
 *  @param   iLength DOXYGEN_TO_DO
 *  @param   expo DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |RXIN[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*bDetSoft     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |iLength       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |expo          |              |              |              |              |              |              |
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
#ifndef CEVA_INTRINSICS

void Demod_QAM16(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength, S16 expo)
{
    S32 I, J, K;
    S16 AC1, AC2, AC3, AC4;
    S16 Qam16_2;
    U8 scrBit;
    U8 mask = 0x80;

    K = 0;

    Qam16_2 = shr(P_16QAM_2, expo);

    for (I = 0, J = 0; I < iLength; I += 2, J += 4)
    {
        AC1 = sub(abs_s(RXIN[I]), Qam16_2);
        AC2 = sub(abs_s(RXIN[I + 1]), Qam16_2);

        AC3 = negate(RXIN[I]) >> 8;

        AC4 = negate(RXIN[I + 1]) >> 8;

        AC1 = AC1 >> 8;

        AC2 = AC2 >> 8;

        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            //Add 1 to prevent overflow when trying
            //to negate -128
            AC3 = negate(AC3 + 1);
        }
        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            //Add 1 to prevent overflow when trying
            //to negate -128
            AC4 = negate(AC4 + 1);
        }
        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            AC1 = negate(AC1);
        }
        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            AC2 = negate(AC2);
        }

        // B0
        bDetSoft[J] = (S8) AC3;
        // B1
        bDetSoft[J + 1] = (S8) AC4;
        // B2
        bDetSoft[J + 2] = (S8) AC1;
        // B3
        bDetSoft[J + 3] = (S8) AC2;

        if(!mask)
        {
            K++;
            mask = 0x80;
        }
    }
    return;
}

/******************************************************************************
 Demod_64QAM (LTE 64-QAM Demodulator)
 ******************************************************************************

 Demod_64QAM implements the 64-QAM demodulator for the LTE receiver.
 In addition to determining the hard data bits, the routine also computes a
 quality measure for each data bit.  These quality measures are required for
 optimum soft-decision decoding.  The LTE 64-QAM constellation is shown below.


 X              Y            B0 B1 B2 B3 B4 B5
 ---------------------------------------------
 P_64QAM_3      P_64QAM_3    0  0  0  0  0  0
 P_64QAM_3      P_64QAM_1    0  0  0  0  0  1
 P_64QAM_1      P_64QAM_3    0  0  0  0  1  0
 P_64QAM_1      P_64QAM_1    0  0  0  0  1  1
 P_64QAM_3      P_64QAM_5    0  0  0  1  0  0
 P_64QAM_3      P_64QAM_7    0  0  0  1  0  1
 P_64QAM_1      P_64QAM_5    0  0  0  1  1  0
 P_64QAM_1      P_64QAM_7    0  0  0  1  1  1

 P_64QAM_5      P_64QAM_3    0  0  1  0  0  0
 P_64QAM_5      P_64QAM_1    0  0  1  0  0  1
 P_64QAM_7      P_64QAM_3    0  0  1  0  1  0
 P_64QAM_7      P_64QAM_1    0  0  1  0  1  1
 P_64QAM_5      P_64QAM_5    0  0  1  1  0  0
 P_64QAM_5      P_64QAM_7    0  0  1  1  0  1
 P_64QAM_7      P_64QAM_5    0  0  1  1  1  0
 P_64QAM_7      P_64QAM_7    0  0  1  1  1  1

 P_64QAM_3     -P_64QAM_3    0  1  0  0  0  0
 P_64QAM_3     -P_64QAM_1    0  1  0  0  0  1
 P_64QAM_1     -P_64QAM_3    0  1  0  0  1  0
 P_64QAM_1     -P_64QAM_1    0  1  0  0  1  1
 P_64QAM_3     -P_64QAM_5    0  1  0  1  0  0
 P_64QAM_3     -P_64QAM_7    0  1  0  1  0  1
 P_64QAM_1     -P_64QAM_5    0  1  0  1  1  0
 P_64QAM_1     -P_64QAM_7    0  1  0  1  1  1

 P_64QAM_5     -P_64QAM_3    0  1  1  0  0  0
 P_64QAM_5     -P_64QAM_1    0  1  1  0  0  1
 P_64QAM_7     -P_64QAM_3    0  1  1  0  1  0
 P_64QAM_7     -P_64QAM_1    0  1  1  0  1  1
 P_64QAM_5     -P_64QAM_5    0  1  1  1  0  0
 P_64QAM_5     -P_64QAM_7    0  1  1  1  0  1
 P_64QAM_7     -P_64QAM_5    0  1  1  1  1  0
 P_64QAM_7     -P_64QAM_7    0  1  1  1  1  1

 -P_64QAM_3      P_64QAM_3    1  0  0  0  0  0
 -P_64QAM_3      P_64QAM_1    1  0  0  0  0  1
 -P_64QAM_1      P_64QAM_3    1  0  0  0  1  0
 -P_64QAM_1      P_64QAM_1    1  0  0  0  1  1
 -P_64QAM_3      P_64QAM_5    1  0  0  1  0  0
 -P_64QAM_3      P_64QAM_7    1  0  0  1  0  1
 -P_64QAM_1      P_64QAM_5    1  0  0  1  1  0
 -P_64QAM_1      P_64QAM_7    1  0  0  1  1  1

 -P_64QAM_5      P_64QAM_3    1  0  1  0  0  0
 -P_64QAM_5      P_64QAM_1    1  0  1  0  0  1
 -P_64QAM_7      P_64QAM_3    1  0  1  0  1  0
 -P_64QAM_7      P_64QAM_1    1  0  1  0  1  1
 -P_64QAM_5      P_64QAM_5    1  0  1  1  0  0
 -P_64QAM_5      P_64QAM_7    1  0  1  1  0  1
 -P_64QAM_7      P_64QAM_5    1  0  1  1  1  0
 -P_64QAM_7      P_64QAM_7    1  0  1  1  1  1

 -P_64QAM_3     -P_64QAM_3    1  1  0  0  0  0
 -P_64QAM_3     -P_64QAM_1    1  1  0  0  0  1
 -P_64QAM_1     -P_64QAM_3    1  1  0  0  1  0
 -P_64QAM_1     -P_64QAM_1    1  1  0  0  1  1
 -P_64QAM_3     -P_64QAM_5    1  1  0  1  0  0
 -P_64QAM_3     -P_64QAM_7    1  1  0  1  0  1
 -P_64QAM_1     -P_64QAM_5    1  1  0  1  1  0
 -P_64QAM_1     -P_64QAM_7    1  1  0  1  1  1

 -P_64QAM_5     -P_64QAM_3    1  1  1  0  0  0
 -P_64QAM_5     -P_64QAM_1    1  1  1  0  0  1
 -P_64QAM_7     -P_64QAM_3    1  1  1  0  1  0
 -P_64QAM_7     -P_64QAM_1    1  1  1  0  1  1
 -P_64QAM_5     -P_64QAM_5    1  1  1  1  0  0
 -P_64QAM_5     -P_64QAM_7    1  1  1  1  0  1
 -P_64QAM_7     -P_64QAM_5    1  1  1  1  1  0
 -P_64QAM_7     -P_64QAM_7    1  1  1  1  1  1

 Where: X =              X coordinate
 Y =              Y coordinate
 P_64QAM_1 =      1/SQRT(42)
 P_64QAM_3 =      3/SQRT(42)
 P_64QAM_5 =      5/SQRT(42)
 P_64QAM_7 =      7/SQRT(42)
 B0 - B5 =        data bits

 The inputs to the demodulator are an array of complex receive symbol vectors
 (RXIN[]) and their corresponding channel estimate powers (ChEst_pwr).  It is
 assumed that the receive symbol vectors have already been adjusted using
 their channel estimates and maximum-ratio combined (see routine rx_MrcDetector_fx).
 However, the magnitudes of the receive symbol vectors are still weighted by
 the overall combined channel estimate powers.  The determination of the hard
 data bits is shown below.

 B0 = COND( X < 0 )
 B1 = COND( Y < 0 )
 B2 = COND( |X| > P_64QAM_4 )
 B3 = COND( |Y| > P_64QAM_4 )
 B4 = COND( ( |X| < P_64QAM_2 ) OR (|X| > P_64QAM_6) )
 B5 = COND( ( |Y| < P_64QAM_2 ) OR (|Y| > P_64QAM_6) )

 Where:  COND(X)     =   0 (if X is false), 1 (if X is true)
 P_64QAM_2   =   2/SQRT(42)
 P_64QAM_4   =   4/SQRT(42)
 P_64QAM_6   =   6/SQRT(42)

 The quality measures for each bit are computed using the distances from
 their respective decision boundaries.  The quality measures are computed as
 16-bit values with the sign bit inverted being used as the actual hard data
 bit.  The quality measures for each of the data bits are shown below.

 ------------------------------------------------------------------------------
 B0
 --

 B0VAL = -X (normalized)

 ------------------------------------------------------------------------------
 B1
 --

 B1VAL = -Y (normalized)

 ------------------------------------------------------------------------------
 B2
 --

 B2VAL = -P_64QAM_4*CHP + |X| (normalized)

 ------------------------------------------------------------------------------
 B3
 --

 B3VAL = -P_64QAM_4*CHP + |Y| (normalized)

 ------------------------------------------------------------------------------
 B4
 --

 B4VAL = -P_64QAM_2*CHP + |P_64QAM_4*CHP - |X|| (normalized)

 ------------------------------------------------------------------------------
 B5
 --

 B5VAL = -P_64QAM_2*CHP + |P_64QAM_4*CHP - |Y|| (normalized)

 ------------------------------------------------------------------------------
 Where:  X =         complex receive symbol vector (real part)
 Y =         complex receive symbol vector (imaginary part)
 P_64QAM_2 = 2/SQRT(42)
 P_64QAM_4 = 4/SQRT(42)
 CHP =       channel power estimate

 ------------------------------------------------------------------------------

 The outputs for the demodulator are an array containing the BnVAL values.
 The calling format for Demod_64QAM is shown below.

 Demod_64QAM RXIN, CHP, OUT, N, NSHFT

 Where:  RXIN =  Complex receive vector input array (organized as
 real part, imaginary part).
 CHP =   Channel power estimate
 OUT =   Output data array (organized as B0VAL, B1VAL, B2VAL,
 B3VAL, B4VAL, B5VAL).
 N =     Number or receive vectors to demodulate.
 NSHFT = Number of shifts needed to normalize RXIn array.

 *****************************************************************************/
#define P_64QAM_2       5055 //= 2/SQRT(42) in Q15
#define P_64QAM_4       10112 //= 4/SQRT(42) in Q15

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   RXIN[] DOXYGEN_TO_DO
 *  @param   *bDetSoft DOXYGEN_TO_DO
 *  @param   iLength DOXYGEN_TO_DO
 *  @param   expo DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |RXIN[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*bDetSoft     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |iLength       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |expo          |              |              |              |              |              |              |
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
void Demod_QAM64(S16 RXIN[], S8* scrSeq, S8 *bDetSoft, S32 iLength, S16 expo)
{
    S16 AC1, AC2, AC3, AC4, AC5, AC6;
    S16 Qam64_2, Qam64_4;
    S32 I, J, K;
    U8 scrBit;
    U8 mask = 0x80;

    K = 0;

    Qam64_2 = shr(P_64QAM_2, expo);
    Qam64_4 = shr(P_64QAM_4, expo);

    for (I = 0, J = 0; I < iLength; I += 2, J += 6)
    {
        AC1 = sub(abs_s(RXIN[I]), Qam64_4);
        AC2 = sub(abs_s(RXIN[I + 1]), Qam64_4);
        AC3 = negate(RXIN[I]) >> 8;
        AC4 = negate(RXIN[I + 1]) >> 8;
        AC5 = AC1 >> 8;
        AC6 = AC2 >> 8;

        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            //Add 1 to prevent overflow when trying
            //to negate -128
            AC3 = negate(AC3 + 1);
        }
        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            //Add 1 to prevent overflow when trying
            //to negate -128
            AC4 = negate(AC4 + 1);
        }

        if(!mask)
        {
            K++;
            mask = 0x80;
        }

        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            AC5 = negate(AC5);
        }
        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            AC6 = negate(AC6);
        }

        // B0
        bDetSoft[J] = (S8) AC3;
        // B1
        bDetSoft[J + 1] = (S8) AC4;
        // B2
        bDetSoft[J + 2] = (S8) AC5;
        // B3
        bDetSoft[J + 3] = (S8) AC6;

        if(!mask)
        {
            K++;
            mask = 0x80;
        }

        AC1 = sub(abs_s(AC1), Qam64_2);
        AC2 = sub(abs_s(AC2), Qam64_2);

        AC1 = AC1 >> 8;
        AC2 = AC2 >> 8;

        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            AC1 = negate(AC1);
        }
        scrBit = scrSeq[K] & mask;
        mask >>= 1;
        if(scrBit)
        {
            AC2 = negate(AC2);
        }

        // B4VAL
        bDetSoft[J + 4] = (S8) AC1;
        // B5VAL
        bDetSoft[J + 5] = (S8) AC2;

        if(!mask)
        {
            K++;
            mask = 0x80;
        }
    }
    return;
}
#endif
