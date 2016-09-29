//-------------------------------------------------------------------------------------------
/** @file lte_FEC_utils.c
 *
 * @brief This file contains utility functions required to implement the LTE FEC.
 * @author Mindspeed Technologies
 * @version $Revision: 1.106 $
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
#include "global.h"
extern U8 *DataBuff_Bank1;
extern U8 *DataBuff_Bank2;
extern U8 *DataBuff_Bank3;
#else
#include "basic_op_cc.h"
#include "gputil_fx.h"
#include "mcs_tables.h"
#endif

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Downlink FEC Rx Utility Functions used by both ARM and Ceva
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------
/** @brief CC_Encoder:  Tail-biting Convolutional Coding
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param in - INPUT, packed input data bit array (including 16-bit CRC) organized as bytes.  For each byte bit7(msb) is the first
 *              bit and bit0(lsb) is the last bit.
 *  @param out - OUTPUT, packed CC encoder output bit organized as array of 32 bits.
 *  @param n - INPUT, 32bit signed number containing the length of CC encoder input array in bits.
 *  @param nb - INPUT, 32bit signed number containing the position of the last bit in the last byte of in[].
 *  @param nsb - INPUT, 32bit signed number containing the length of sub-block interleaver output arrays in bits.
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length       |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |             |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |in           |         |Input         |ceil(n/8)    |pInterger      |(0:16:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |out          |         |Output        |3*ceil(n/32) |pInteger       |(0:32:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |n            |         |Input         |1            |Integer       |(1:31:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |nb           |         |Input         |1            |Integer       |(1:31:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |nsb          |         |Input         |1            |Integer       |(1:31:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |CC_Encoder                           |
 *  ------------------------------------------------------------
 *  |Input Streams       |in                                   |
 *  ------------------------------------------------------------
 *  |Output Streams      |out                                  |
 *  ------------------------------------------------------------
 *  |Config Parameters   |n, nb, nsb                           |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  CC_Encoder:  Convolutional encoding of dci
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void CC_Encoder(U8 in[], U32 out[], S32 n, S32 nb, S32 nsb)
{
    U8 reg = 0,temp,in_temp;
    S32 i,j,j1,k1,Nbyte,k=5;
    U32 tmp32,temp0,temp1,temp2;
    U32 *out1,*out2;

    // odd/even parity table
    U8 Parity[256] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0,
    0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
    0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1,
    0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
    0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1,
    0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };

    // Index of the last byte location
    Nbyte = n >> 3;

    if(n == (Nbyte<<3))
    {
        Nbyte -= 1;
    }

    // Initialize encoder shift register using last 6 input bits
    for(i = 0;i < 6;i++)
    {
        temp = (in[Nbyte] >> nb) & 1;

        reg |= temp << k;

        k--;

        nb++;

        if(nb == 8)
        {
            nb = 0;
            Nbyte--;
        }
    }

    // initialize index of input (byte) location
    j = 0;

    // initialize index of output (32 bit) location
    j1 = 0;

    k = 7;

    k1 = 31 - (nsb - n);

    out1 = &out[nsb >> 5];

    out2 = &out[nsb >> 4];

    in_temp = in[j];

    temp0 = 0;

    temp1 = 0;

    temp2 = 0;

    for(i = 0;i < n;i++)
    {
        temp = (in_temp >> k) & 1;

        // input bit to the encoder
        reg |= temp << 6;

        // ================================
        // Constituent Encoder 1 Octal 133
        // ================================
        temp = reg & 0x5b;

        // compute d_k^0 (Fig. 5.1.3-1 of 36.212)
        tmp32 = Parity[temp];

        // store the output in 32 bit temp location
        temp0 |= tmp32 << k1;

        // ================================
        // Constituent Encoder 2 Octal 171
        // ================================
        temp = reg & 0x79;

        // compute d_k^1 (Fig. 5.1.3-1 of 36.212)
        tmp32 = Parity[temp];

        // store the output in 32 bit temp location
        temp1 |= tmp32 << k1;

        // ================================
        // Constituent Encoder 3 Octal 165
        // ================================
        temp = reg & 0x75;

        // compute d_k^2 (Fig. 5.1.3-1 of 36.212)
        tmp32 = Parity[temp];

        // store the output in 32 bit temp location
        temp2 |= tmp32 << k1;

        // ================================

        reg = reg >> 1;

        k--;

        if(k < 0) // move to next input byte location once output corresponding to all 8 bits are computed
        {
            k = 7;
            j++;
            in_temp = in[j];
        }

        k1--;

        if(k1 < 0) // move to next output 32-bit location once output corresponding to 32 bits are computed
        {
            k1 = 31;
            out[j1] = temp0;
            out1[j1] = temp1;
            out2[j1] = temp2;
            temp0 = 0;
            temp1 = 0;
            temp2 = 0;
            j1++;
        }
    }

    return;
}

/*********************************************************************************
 DCI_CC_SubBlockInterleaver (LTE sub-block interleaver for conv coded dci channels)
 *********************************************************************************

 DCI_CC_SubBlockInterleaver implements the sub-block interleaver for convolutionally
 coded channels described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 (2008-12) section 5.1.4.2.1.  To fully implement the required rate matching
 function, CC_SubBlockInterleaver must be run three times, once for each of
 three convolutional encoder outputs.  The functional calling format is defined
 below.

 nout = CC_SubBlockInterleaver(in, out, null, n)

 Where:  in =    Input bit stream organized as bytes (Bit0 = data bit).
 out =   Output bit stream organized as bytes (Bit0 = data bit).
 n =  Length of CC encoder input array in bits (including 16 bits for CRC)
 out[] element is valid or an inserted <null>.  0 = valid,
 1 = inserted <null>.
 n =     Length of in.
 E =  desired rate matching output length in bits.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE sub-block interleaver function for convolutionally encoded channels.
 *
 *  <1> Hardware Resource:
 *  FEC
 *
 *  @param     in =    Input bit stream organized as 32bits (lsb=bit).
 *  @param     out =   Output bit stream organized as bytes (lsb=bit).
 *  @param     n =     Total number of bits in the Sub-block interleaver (excluding filler/NULL bits) corresponding to the output from
 *                     each constituent encoder.
 *  @param     nout =  Total number of bits in the Sub-block interleaver (including filler/NULL bits) corresponding to the output from
 *                     each constituent encoder.
 *  @return    E =     desired rate matching output length in bits.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length               |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |                     |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |Input        |3*ceil(n/32)         |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |Output       |ceil(max(3*n,E)/8)   |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |Input        |1                    |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |Input        |1                    |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------------
 *  |E              |               |Input        |1                    |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |DCI_CC_SubBlockInterleaver  |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n, nout, E                  |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * DCI_CC_SubBlockInterleaver implements the sub-block interleaver for convolutionally
 * coded channels described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 * (2008-12) section 5.1.4.2.1.  To fully implement the required rate matching
 * function, CC_SubBlockInterleaver must be run three times, once for each of
 * three convolutional encoder outputs.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.1.4.2.1
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void DCI_CC_SubBlockInterleaver(U32 in[], U8 out[], S32 n, S32 nout, S32 E)
{
    S32 i, j, k, nrow, ND;

    U32 flag,mask,m0,m1,m2;

    // Define inter-column permutation table
    S16 P[32] =
    {  1, 17,  9, 25,  5, 21, 13, 29,  3, 19, 11,
      27,  7, 23, 15, 31,  0, 16,  8, 24,  4, 20,
      12, 28,  2, 18, 10, 26,  6, 22, 14, 30 };

    U32 *in1,*in2;

    U8 T0,T1,T2,BB0,BB1,BB2,flag1;

    T0 = 0x80;

    m1 = n>>3;

    j = n - (m1 << 3);

    T1 = T0 >> j;

    m2 = n>>2;

    j = (n << 1) - (m2 << 3);

    T2 = T0 >> j;

    nrow = nout >> 5;

    in1 = &in[nrow];

    in2 = &in[nrow << 1];

    // Initialize output array index
    m0 = 0;
    BB0 = 0;
    BB1 = 0;
    BB2 = 0;

    // Compute number of null filler bits
    ND = nout - n;

    mask = 0x80000000;

    for(j = 0;j < 32;j++)
    {
        for(i = 0;i < nrow;i++)
        {
            k = P[j];
            if(!(i == 0 && k < ND))
            {
                flag = (in[i] & (mask >> k));
                if(flag)
                    BB0 |= T0;
                T0 = T0 >> 1;

                flag = (in1[i] & (mask >> k));
                if(flag)
                    BB1 |= T1;
                T1 = T1 >> 1;

                flag = (in2[i] & (mask >> k));
                if(flag)
                    BB2 |= T2;
                T2 = T2 >> 1;

                if(T0 == 0)
                {
                    out[m0++] = BB0;
                    BB0 = 0;
                    T0 = 0x80;
                }

                if(T1 == 0)
                {
                    out[m1++] = BB1;
                    BB1 = 0;
                    T1 = 0x80;
                }

                if(T2 == 0)
                {
                    out[m2++] = BB2;
                    BB2 = 0;
                    T2 = 0x80;
                }
            }
        }
    }

    out[m0] |= BB0;

    out[m1] |= BB1;

    k = n + (n << 1);

    T0 = 0x80;

    if(k < E)
        for(i = k,j=0;i < E;i++)
        {
            flag1 = (out[j] & T0);
            if(flag1)
                BB2 |= T2;

            T2 = T2 >> 1;
            T0 = T0 >> 1;

            if(T2 == 0)
            {
                out[m2++] = BB2;
                BB2 = 0;
                T2 = 0x80;
            }

            if(T0 == 0)
            {
                T0 = 0x80;
                j++;
            }
        }

    out[m2] = BB2;

    return;
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Uplink FEC Rx Utility Functions used by both ARM and Ceva
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------
/** @brief LTE PUSCH Multiplex control and data.  Extracts soft decisions from
 *         descrambler output buffer for ACK, RI, and CQI decoding
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param     pFecParams  - FEC input parameter structure containing all FEC parameters for all
 *                           users in current subframe.
 *  @param     pMuxControl - Multiplex control and data input parameter structure for all
 *                           users in current subframe.
 *  @param     scrSeq      - Descrambling sequence.
 *  @param     insft       - Soft decisions, output of Descrambler for all users
 *  @param     chanNumber  - Channel Number
 *  @param     isym        - Current symbol being processed
 *  @param     tmpsftri    - Output array for RI soft decisions
 *  @param     tmpsftack   - Output array for ACK soft decisions
 *  @param     tmpsftmux   - Output array for CQI soft decisions
 *  @param     numRI       - Number of RI constellation points in current symbol
 *  @param     numACK      - Number of ACK constellation points in current symbol
 *  @param     numCQI      - Number of CQI constellation points in current symbol
 *  @param     posRI       - Offset location to RI constellation points
 *  @param     posACK      - Offset location to ACK constellation points
 *  @param     posCQI      - Offset location to CQI constellation points
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name         |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |             |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pFecParams   |               |input        |1              |pStructure    |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pMuxControl  |               |output       |1              |pStructure    |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |scrSeq       |               |input        |1              |Real          |(8:0:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |chanNumber   |               |input        |1              |Real          |(16:0:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |isym         |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |tmpsftri     |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |tmpsftack    |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |tmpsftmux    |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |numRI        |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |numACK       |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |numCQI       |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |posRI        |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |posACK       |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |posCQI       |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------------
 *  |Block Name          |bs_rx_mux_control_extract_softbits_sym |
 *  --------------------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  --------------------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  --------------------------------------------------------------
 *  |Config Parameter(s) |nin, Qm, nbits              |
 *  --------------------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  --------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function ACK_RI_Decoder implements a decoder for the 1-bit or 2-bit ACK
 * or RI fields received on the LTE PUSCH as described in the 3GPP (LTE)
 * standard TS 36.212 Section 5.2.2.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
// PUSCH MUX Control demultiplexing
void bs_rx_mux_control_extract_softbits_sym(PULRXFECPARAMS pFecParams, S8* scrSeq, S8* insft,
                                            S32 nc, S32 CP, U32 isym, S8 *pMuxSoftDec)
{
    U32 *scrSeq32, *insft32;
    U16 *scrSeq16, *insft16;
    U16 *pMuxBuff16;
    U32 *pMuxBuff32;
    U16 noutri, noutack;
    U16 Qm, posNum;
    S32 k, pos;
    U32 ac, offset_ACK, offset_RI, offset_CQI;
    U32 offset_CQI1 = 0;
    U32 numSymCQI0, numSymCQI1, numSymCQI2, numSymCQI3;
    U32 checkPos0 = 0, checkPos1 = 0, checkPos2 = 0, checkPos3 = 0;
    U32 numSymACK, numSymRI, numSymCQI;
    U32 symRI0, symRI1, symRI2, symRI3;

    noutri = pFecParams->nRI[nc];
    noutack = pFecParams->nACK[nc];
    Qm = pFecParams->Qm[nc];

    numSymACK = pFecParams->numSymPosACK[nc][isym];
    numSymRI  = pFecParams->numSymPosRI[nc][isym];
    numSymCQI = pFecParams->numSymPosCQI[nc][isym];

    //Set all 4 symbols which have RI present
    symRI0 = ColSetRI[CP][0];
    symRI1 = ColSetRI[CP][1];
    symRI2 = ColSetRI[CP][2];
    symRI3 = ColSetRI[CP][3];

    //Number of CQI constellation points at the symbols
    //where RI points are present
    numSymCQI0 = pFecParams->numSymPosCQI[nc][symRI0];
    numSymCQI1 = pFecParams->numSymPosCQI[nc][symRI1];
    numSymCQI2 = pFecParams->numSymPosCQI[nc][symRI2];
    numSymCQI3 = pFecParams->numSymPosCQI[nc][symRI3];

    //Setup checks for RI and CQI collisions
    if (isym > symRI0)
    {
        checkPos0 = 1;
        //if first row of interleaver at 1st RI symbol
        //has no CQIs, then we have to minus 1 from
        //the initial write position into Mux CQI
        //soft decision buffer
        if (numSymCQI0 == 0)
            offset_CQI1--;
    }
    if (isym > symRI3)
    {
        checkPos3 = 1;
        //if first row of interleaver at 4th RI symbol
        //has no CQIs, then we have to minus 1 from
        //the initial write position into Mux CQI
        //soft decision buffer
        if (numSymCQI3 == 0)
            offset_CQI1--;
    }
    if (isym > symRI2)
    {
        checkPos2 = 1;
        //if first row of interleaver at 3rd RI symbol
        //has no CQIs, then we have to minus 1 from
        //the initial write position into Mux CQI
        //soft decision buffer
        if (numSymCQI2 == 0)
            offset_CQI1--;
    }
    if (isym > symRI1)
    {
        checkPos1 = 1;
        //if first row of interleaver at 2nd RI symbol
        //has no CQIs, then we have to minus 1 from
        //the initial write position into Mux CQI
        //soft decision buffer
        if (numSymCQI1 == 0)
            offset_CQI1--;
    }

    //Setup pointers for writing to Mux soft decision buffer
    pMuxBuff16 = (U16 *)(pMuxSoftDec);
    pMuxBuff32 = (U32 *)(pMuxSoftDec);

    //Find offset into Mux soft decision buffer for
    //ACK, RI and CQI for this user
    offset_ACK = pFecParams->ACK_User_offset[nc];
    offset_RI  = pFecParams->RI_User_offset[nc]  + pFecParams->TotalACK_Bits;
    offset_CQI = pFecParams->CQI_User_offset[nc] + pFecParams->TotalACK_Bits + pFecParams->TotalRI_Bits;

    if (Qm == 2)
    {
        offset_ACK = offset_ACK >> 1;
        offset_RI  = offset_RI >> 1;
        offset_CQI = offset_CQI >> 1;

        offset_ACK += pFecParams->symPosACK[isym];
        offset_RI  += pFecParams->symPosRI[isym];
        offset_CQI += (pFecParams->symPosCQI[isym] + offset_CQI1);
    }
    else if (Qm == 4)
    {
        offset_ACK = offset_ACK >> 2;
        offset_RI  = offset_RI >> 2;
        offset_CQI = offset_CQI >> 2;

        offset_ACK += pFecParams->symPosACK[isym];
        offset_RI  += pFecParams->symPosRI[isym];
        offset_CQI += (pFecParams->symPosCQI[isym] + offset_CQI1);
    }
    else if (Qm == 6)
    {
        offset_ACK = offset_ACK >> 1;
        offset_RI  = offset_RI >> 1;
        offset_CQI = offset_CQI >> 1;

        offset_ACK += 3*pFecParams->symPosACK[isym];
        offset_RI  += 3*pFecParams->symPosRI[isym];
        offset_CQI += 3*(pFecParams->symPosCQI[isym] + offset_CQI1);
    }

    //All ACK and RI allocation starts from the
    //bottom up of PUSCH allocation.
    pos = pFecParams->Mpusch_sc[nc] - 1;
    if (Qm == 6)
        pos = 3*(pFecParams->Mpusch_sc[nc] - 1);

    k = 0;
    ac = pos;
    scrSeq16 = (U16*) scrSeq;
    insft16  = (U16*) insft;
    scrSeq32 = (U32*) scrSeq;
    insft32 = (U32*) insft;
    if (numSymACK)
    {
        for(posNum = 0; posNum < numSymACK; posNum++)
        {
            // If QPSK
            if (Qm == 2)
            {
                // Run QPSK channel de-interleaver
                ACK_RI_DeInterLeaver_QPSK(scrSeq16, insft16, &pMuxBuff16[offset_ACK], ac, k, noutack);
                k  = k + NUM_SYM_ACK_RI;
                ac = ac - 1;
            }
            else if (Qm == 4)
            {
                // Run QAM16 channel de-interleaver
                ACK_RI_DeInterLeaver_QAM16(scrSeq32, insft32, &pMuxBuff32[offset_ACK], ac, k, noutack);
                k  = k + NUM_SYM_ACK_RI;
                ac = ac - 1;
            }
            else if (Qm == 6)
            {
                // Run QAM64 channel de-interleaver
                ACK_RI_DeInterLeaver_QAM64(scrSeq16, insft16, &pMuxBuff16[offset_ACK], ac, k, noutack);
                k  = k + 3 * NUM_SYM_ACK_RI;
                ac = ac - 3;
            }
        }
    }
    else if (numSymRI)
    {
        for(posNum = 0; posNum < numSymRI; posNum++)
        {
            // If QPSK
            if (Qm == 2)
            {
                // Run QPSK channel de-interleaver
                ACK_RI_DeInterLeaver_QPSK(scrSeq16, insft16, &pMuxBuff16[offset_RI], ac, k, noutri);
                k  = k + NUM_SYM_ACK_RI;
                ac = ac - 1;
            }
            else if (Qm == 4)
            {
                // Run QAM16 channel de-interleaver
                ACK_RI_DeInterLeaver_QAM16(scrSeq32, insft32, &pMuxBuff32[offset_RI], ac, k, noutri);
                k  = k + NUM_SYM_ACK_RI;
                ac = ac - 1;
            }
            else if (Qm == 6)
            {
                // Run QAM64 channel de-interleaver
                ACK_RI_DeInterLeaver_QAM64(scrSeq16, insft16, &pMuxBuff16[offset_RI], ac, k, noutri);
                k  = k + 3 * NUM_SYM_ACK_RI;
                ac = ac - 3;
            }
        }
    }

    // Initialize CQI output array index.
    k = 0;
    pos = 0;
    for(posNum = 0; posNum < numSymCQI; posNum++)
    {
        // If QPSK
        if (Qm == 2)
        {
            //Copy input to output array.
            // Two 8-bit soft decisions are copied at a time.
            pMuxBuff16[k + offset_CQI] = insft16[pos];
            k = k + MAX_PUSCH_DATASYMB_PER_SUBF;

            //Check to see how many RI and CQI collisions
            //happened on this row in de-interleaver buffer
            if (checkPos0++ >= numSymCQI0)
                k = k - 1;
            if (checkPos1++ >= numSymCQI1)
                k = k - 1;
            if (checkPos2++ >= numSymCQI2)
                k = k - 1;
            if (checkPos3++ >= numSymCQI3)
                k = k - 1;

            pos = pos + 1;
        }
        // Else if QAM16
        else if (Qm == 4)
        {
            //Copy input to output array.
            //Four 8-bit soft decisions are copied at a time.
            pMuxBuff32[k + offset_CQI] = insft32[pos];
            k = k + MAX_PUSCH_DATASYMB_PER_SUBF;

            //Check to see how many RI and CQI collisions
            //happened on this row in de-interleaver buffer
            if (checkPos0++ >= numSymCQI0)
                k = k - 1;
            if (checkPos1++ >= numSymCQI1)
                k = k - 1;
            if (checkPos2++ >= numSymCQI2)
                k = k - 1;
            if (checkPos3++ >= numSymCQI3)
                k = k - 1;

            pos = pos + 1;
        }
        // Else if QAM64
        else if (Qm == 6)
        {
            //Copy input to output array.
            //Two 8-bit soft decisions are copied at a time.
            pMuxBuff16[k + offset_CQI]     = insft16[pos];
            pMuxBuff16[k + offset_CQI + 1] = insft16[pos + 1];
            pMuxBuff16[k + offset_CQI + 2] = insft16[pos + 2];
            k = k + 3 * MAX_PUSCH_DATASYMB_PER_SUBF;

            //Check to see how many RI and CQI collisions
            //happened on this row in de-interleaver buffer
            if (checkPos0++ >= numSymCQI0)
                k = k - 3;
            if (checkPos1++ >= numSymCQI1)
                k = k - 3;
            if (checkPos2++ >= numSymCQI2)
                k = k - 3;
            if (checkPos3++ >= numSymCQI3)
                k = k - 3;
            pos = pos + 3;
        }
    }

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE PUSCH Multiplex ACK and RI De-Interleaver for QPSK
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     pScrSeq16 - Descrambler sequence so that we can recover descrambler input
 *  @param     insft16   - Soft decision input
 *  @param     pOutput   - De-interleaver output.
 *  @param     Index     - index into insft16 to be de-interleaved
 *  @param     outInd    - output index.
 *  @param     numBits   - number of RI or ACK bits.
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pScrSeq16      |               |input        |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |insft16        |               |input        |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pOutput        |               |output       |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Index          |               |input        |1              |pReal         |1:31:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |outInd         |               |input        |1              |Real          |1:31:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |numBits        |               |input        |1              |Real          |0:32:0)   |[1,2]          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |ACK_RI_DeInterLeaver_QPSK          |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |pScrSeq16, insft16                 |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |pOutput                            |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |Index, outInd, numBits             |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |none                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function will de-interleave ACK and RI soft decisions for
 * multiplex control in data.  For QPSK modulation.  Will
 * de-interleave 4 soft decisions at one time.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void ACK_RI_DeInterLeaver_QPSK(U16 *pScrSeq16, U16 *insft16, U16 *pOutput, S32 Index, S32 outInd, U32 numBits)
{
    U16 ac4;
    S32 byteIndex, bitOffset, i;
    S8  *pData;
    S8 Byte, pScrSeq8[4];
    U8* pScrInp8 = (U8*) pScrSeq16;

    //Read 2 8-bit soft decisions after de-scrambling.
    ac4 = insft16[Index];
    pData = (S8*) &ac4;
//    ac2 = pScrSeq16[Index];
//    pScrSeq8 = (S8*) &ac2;
    byteIndex = (2*Index) >> 3;
    bitOffset = (2*Index) - (byteIndex << 3);

    Byte = pScrInp8[byteIndex];
    bitOffset = 7 - bitOffset;

    for(i=0; i<2; i++)
    {
        pScrSeq8[i] = (Byte >> bitOffset) & 1;
        bitOffset--;
        if(bitOffset < 0)
        {
            bitOffset = 7;
            Byte = pScrInp8[byteIndex+1];
        }
    }

    // QPSK symbol contains (data bit, y bit).
    if(numBits == 1)
    {
        // If sign bit changed then need to invert inb[] 2nd byte.
        if(pScrSeq8[0] > 0)
        {
            pData[1] = -pData[1];
            if(pScrSeq8[1] > 0)
                pData[1] = -pData[1];
        }
        else
        {
            if(pScrSeq8[1] > 0)
                pData[1] = -pData[1];
        }
    }

    // Two 8-bit soft decisions are copied at a time.
    pOutput[outInd] = ac4;

    // Clear ACK/RI soft decisions in input array.
    insft16[Index] = 0;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE PUSCH Multiplex ACK and RI De-Interleaver for QAM16
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     pScrSeq32 - Descrambler sequence so that we can recover descrambler input
 *  @param     insft32   - Soft decision input
 *  @param     pOutput   - De-interleaver output.
 *  @param     Index     - index into insft16 to be de-interleaved
 *  @param     outInd    - output index.
 *  @param     numBits   - number of RI or ACK bits.
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pScrSeq32      |               |input        |1              |pReal         |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |insft32        |               |input        |1              |pReal         |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pOutput        |               |output       |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Index          |               |input        |1              |pReal         |1:31:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |outInd         |               |input        |1              |Real          |1:31:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |numBits        |               |input        |1              |Real          |0:32:0)   |[1,2]          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |ACK_RI_DeInterLeaver_QAM16         |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |pScrSeq32, insft32                 |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |pOutput                            |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |Index, outInd, numBits             |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |none                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function will de-interleave ACK and RI soft decisions for
 * multiplex control in data.  For QAM16 modulation.  Will
 * de-interleave 8 soft decisions at one time.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void ACK_RI_DeInterLeaver_QAM16(U32 *pScrSeq32, U32 *insft32, U32 *pOutput, S32 Index, S32 outInd, U32 numBits)
{
    U32 ac4, m;
    S32 byteIndex, bitOffset, i;
    S8 *pData;
    S8 Byte, pScrSeq8[4];
    U8* pScrInp8 = (U8*) pScrSeq32;

    // Read 4 8-bit soft decisions after de-scrambling.
    ac4 = insft32[Index];
    pData    = (S8*) &ac4;

    // Read 4 8-bit soft-decisions before de-scrambling.
//    ac2 = pScrSeq32[Index];
//    pScrSeq8 = (S8*) &ac2;
    byteIndex = (4*Index) >> 3;
    bitOffset = (4*Index) - (byteIndex << 3);

    Byte = pScrInp8[byteIndex];
    bitOffset = 7 - bitOffset;

    for(i=0; i<4; i++)
    {
        pScrSeq8[i] = (Byte >> bitOffset) & 1;
        bitOffset--;
        if(bitOffset < 0)
        {
            bitOffset = 7;
            Byte = pScrInp8[byteIndex+1];
        }
    }

    for(m=2; m<4; m++)
    {
        if(pScrSeq8[m] > 0)
            pData[m] = -pData[m];
    }

    // If 1 RI bit, then 16-QAM format is data, y, x, x.
    if(numBits == 1)
    {
        // If sign bit changed then need to invert inb[] 2nd byte.
        if(pScrSeq8[0] > 0)
        {
            pData[1] = -pData[1];
            if(pScrSeq8[1] > 0)
                pData[1] = -pData[1];
        }
        else
        {
            if(pScrSeq8[1] > 0)
                pData[1] = -pData[1];
        }
    }

    // Copy 4-bit RI symbol into output array.
    // Four 8-bit soft decisions are copied at a time.
    pOutput[outInd] = ac4;

    // Clear ACK/RI soft decisions in input array.
    insft32[Index] = 0;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE PUSCH Multiplex ACK and RI De-Interleaver for QAM64
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     pScrSeq16 - Descrambler sequence so that we can recover descrambler input
 *  @param     insft16   - Soft decision input
 *  @param     pOutput   - De-interleaver output.
 *  @param     Index     - index into insft16 to be de-interleaved
 *  @param     outInd    - output index.
 *  @param     numBits   - number of RI or ACK bits.
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pScrSeq16      |               |input        |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |insft16        |               |input        |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pOutput        |               |output       |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Index          |               |input        |1              |pReal         |1:31:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |outInd         |               |input        |1              |Real          |1:31:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |numBits        |               |input        |1              |Real          |0:32:0)   |[1,2]          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |ACK_RI_DeInterLeaver_QAM64         |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |pScrSeq16, insft16                 |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |pOutput                            |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |Index, outInd, numBits             |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |none                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function will de-interleave ACK and RI soft decisions for
 * multiplex control in data.  For QAM64 modulation.  Will
 * de-interleave 4 soft decisions at one time.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void ACK_RI_DeInterLeaver_QAM64(U16 *pScrSeq16, U16 *insft16, U16 *pOutput, S32 Index, S32 outInd, U32 numBits)
    {
    U16 ac4;
    S32 byteIndex, bitOffset, i;
    S8  *pData;
    S8 Byte, pScrSeq8[4];
    U8* pScrInp8 = (U8*) pScrSeq16;

    // Read 2 8-bit soft decisions after de-scrambling.
    ac4      = insft16[Index];
    pData    = (S8*) &ac4;

//    ac2      = pScrSeq16[Index];
//    pScrSeq8 = (S8*) &ac2;
    byteIndex = (2*Index) >> 3;
    bitOffset = (2*Index) - (byteIndex << 3);

    Byte = pScrInp8[byteIndex];
    bitOffset = 7 - bitOffset;

    for(i=0; i<2; i++)
    {
        pScrSeq8[i] = (Byte >> bitOffset) & 1;
        bitOffset--;
        if(bitOffset < 0)
        {
            bitOffset = 7;
            Byte = pScrInp8[byteIndex+1];
        }
    }

    // QPSK symbol contains (data bit, y bit).
    if(numBits == 1)
    {
        // If sign bit changed then need to invert inb[] 2nd byte.
        if(pScrSeq8[0] > 0)
        {
            pData[1] = -pData[1];
            if(pScrSeq8[1] > 0)
                pData[1] = -pData[1];
        }
        else
        {
            if(pScrSeq8[1] > 0)
                pData[1] = -pData[1];
        }
    }

    // Two 8-bit soft decisions are copied at a time.
    pOutput[outInd] = ac4;

    ac4      = insft16[Index + 1];
    pData    = (S8*) &ac4;

//    ac2      = pScrSeq16[Index + 1];
//    pScrSeq8 = (S8*) &ac2;
    byteIndex = (2*(Index+1)) >> 3;
    bitOffset = (2*(Index+1)) - (byteIndex << 3);

    Byte = pScrInp8[byteIndex];
    bitOffset = 7 - bitOffset;

    for(i=0; i<2; i++)
    {
        pScrSeq8[i] = (Byte >> bitOffset) & 1;
        bitOffset--;
        if(bitOffset < 0)
        {
            bitOffset = 7;
            Byte = pScrInp8[byteIndex+1];
        }
    }

    if(pScrSeq8[0] > 0)
        pData[0] = -pData[0];

    if(pScrSeq8[1] > 0)
        pData[1] = -pData[1];

    pOutput[outInd + 1] = ac4;

    ac4      = insft16[Index + 2];
    pData    = (S8*) &ac4;

//    ac2      = pScrSeq16[Index + 2];
//    pScrSeq8 = (S8*) &ac2;
    byteIndex = (2*(Index+2)) >> 3;
    bitOffset = (2*(Index+2)) - (byteIndex << 3);

    Byte = pScrInp8[byteIndex];
    bitOffset = 7 - bitOffset;

    for(i=0; i<2; i++)
    {
        pScrSeq8[i] = (Byte >> bitOffset) & 1;
        bitOffset--;
        if(bitOffset < 0)
        {
            bitOffset = 7;
            Byte = pScrInp8[byteIndex+1];
        }
    }

    if(pScrSeq8[0] > 0)
        pData[0] = -pData[0];

    if(pScrSeq8[1] > 0)
        pData[1] = -pData[1];

    pOutput[outInd + 2] = ac4;

    // Clear ACK/RI soft decisions in input array.
    insft16[Index]     = 0;
    insft16[Index + 1] = 0;
    insft16[Index + 2] = 0;
}

/******************************************************************************
 ACK_RI_Decoder (LTE 1 or 2 bit HARQ ACK or RI soft decision decoder for PUSCH)
 ******************************************************************************

 Function ACK_RI_Decoder implements a soft decision decoder for the 1-bit or
 2-bit ACK (HARQ acknowledgement) and RI (rank indicator) received on the PUSCH
 (Physical Uplink Shared Channel).  For the LTE PUSCH both HARQ acknowledgement
 and rank indicator bit fields are received.  These bit fields can be either 1
 or 2 bits.  The encoding of these bit fields depends on the length and the
 modulation type and is described in the LTE standard document 3GPP TS 36.212
 Section 5.2.2.  The encoding as defined in the standard is shown in the tables
 below.

 ---------------------------
 -----  1-Bit Fields   -----
 ---------------------------
 Qm      Encoded bit vectors
 ---------------------------
 2       [A y]
 4       [A y x x]
 6       [A y x x x x]
 ---------------------------

 ---------------------------------------------
 -----           2-Bit Fields            -----
 ---------------------------------------------
 Qm      Encoded bit vectors
 ---------------------------------------------
 2       [A B C A B C]
 4       [A B x x C A x x B C x x]
 6       [A B x x x x C A x x x x B C x x x x]
 ---------------------------------------------

 Where: Qm =    Number of bits in modulation vector
                (QPSK:2, 16QAM:4, 64QAM:6)
        A =     1st bit-field bit
        B =     2nd bit-field bit
        C =     Computed parity bit = A xor B
        x,y =   Placeholders used to generate random bits to maximize
                Euclidean distance between modulation vectors.  After bit
                scrambling, the x bits will always be 1s and the y bits will
                be the same as the A bits that preceed them.

 From the tables it can be seen that for the 1-bit case only one Qm-bit
 modulation vector is used and for the 2-bit case three Qm-bit vectors are
 used.

 It is important to note, that it is possible for random data to be transmitted
 instead of the RI and ACK bits.  It is the responsibility of the decoder to
 determine if the signal received contains valid ACK or RI bits or is instead
 just random data.  To provide for robust decoding as well as differentiation
 from random data, repetition coding is used whereby the encoded ACK and RI
 bit vectors are repeated many times.  Also, for valid ACK and RI the x bits
 should always be 1 and the y bits the same as the previous bit.  This will
 not be the case if random data is transmitted.

 The inputs to the decoder are the 8-bit 2's complement "soft" bits, stored
 in array in[], for each of the demodulated vectors.  Positive values indicate
 a "1" bit and negative values a "0" bit.  The organization of the in[] array
 elements is shown below.

 ---------------------------
 -----  1-Bit Fields   -----
 ---------------------------
 Qm      Encoded bit vectors
 ---------------------------
 2       [A y]
 4       [A y x x]
 6       [A y x x x x]
 ---------------------------
 in[]     0 1 2 3 4 5
 ---------------------------

 -----------------------------------------------------
 -----               2-Bit Fields                -----
 -----------------------------------------------------
 Qm      Encoded bit vectors
 -----------------------------------------------------
 2       [A B C A B C]
 4       [A B x x C A x x B C  x  x]
 6       [A B x x x x C A x x  x  x  B  C  x  x  x  x]
 -----------------------------------------------------
 in[]     0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
 -----------------------------------------------------

 The functional calling format is defined below.

 ACK_RI_Decoder(in, *out, nin, Qm, nbits)

 Where: in =    8-bit 2's complement soft decision input array for the
                demodulated vector bits.

        out =   Output byte containing decoded bits.
                    Bit7 = A (1st bit).
                    Bit6 = B (2nd bit) if there is one.
                    Bit5 = Valid ACK or RI indication (= no, 1 = yes).

        nin =   Length of input array in bytes.

        Qm =    Number of modulation vector bits (QPSK:2, 16QAM:4, 64QAM:6).

        nbits = Number of output bits (1 or 2).

 It is important to note that ACK_RI_Decoder must be called twice.  Once to
 decode the ACK bits and once to decode the RI bits.

 ******************************************************************************/

//*****************************************************************************
//**********        ACK_RI_Decoder Control Parameter Defintions      **********
//*****************************************************************************


//*****************************************************************************
//**********                  ACK_RI_Decoder Tables                  **********
//*****************************************************************************

// Table Tab_ACK_RI_NX[] defines the number of "x" bits that occur after the
// data bits.
//
//S16 Tab_ACK_RI_NX[3] = {0, 2, 4};       // Qm=2, Qm=4, Qm=6
//
//*****************************************************************************

// Table Tab_ACK_RI_DET[] defines the fractional scale factor applied to the
// accumulated signal required to declare a valid ACK/RI signal detection.
//
//S16 Tab_ACK_RI_DET[6] = {10640, 4529, 1221,     // 1-bit: Qm=2, Qm=4, Qm=6
//                         13575, 6440, 1658};    // 2-bit: Qm=2, Qm=4, Qm=6
//
//*****************************************************************************

#if 0   // This section is used only to collect test statistics
    double fpmaxsnr = 0.;
    double  fpminsnr = 1000000.;
    double  fpsnr;
#endif

//-------------------------------------------------------------------------------------------
/** @brief LTE PUSCH ACK or RI bit soft decision decoder function.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     in =     8-bit 2's complement soft decision input array.
 *  @param     out =    Output byte.  Bit7 is 1st decoded bit and bit6 is 2nd decoded bit.
 *                      Bit5 is used to indicate valid reception (1=yes, 0=no).
 *  @param     nin =    Length of input array.
 *  @param     Qm =     Modulation vector size in bits (QPSK:2, 16QAM:4, 64QAM:6).
 *  @param     nbits =  Number of output bits decoded (1 or 2).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |1              |pReal         |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(1:15:0)  |{2, 4, 6}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nbits          |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ACK_RI_Decoder              |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |nin, Qm, nbits              |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function ACK_RI_Decoder implements a decoder for the 1-bit or 2-bit ACK
 * or RI fields received on the LTE PUSCH as described in the 3GPP (LTE)
 * standard TS 36.212 Section 5.2.2.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void ACK_RI_Decoder(S8 in[], U8 *out, S32 nin, S16 Qm, S16 nbits)
{
    S32 i;

    S32 indQm, nx, ncnt, ncntd, sums, sumn, detscf, ac, sb;
    S32 sumx[2] = {0, 0};
    S32 sumd[3][2] = {{0, 0}, {0, 0}, {0, 0}};
    S16 Tab_ACK_RI_NX[3] = {0, 2, 4};            // Qm=2, Qm=4, Qm=6

    S16 Tab_ACK_RI_DET[6] = {10486 /*10640*/, 4529, 1221,  // 1-bit: Qm=2, Qm=4, Qm=6
                             13575, 6440, 1658}; // 2-bit: Qm=2, Qm=4, Qm=6
    // Compute table index = Qm/2 - 1.
    indQm = (Qm >> 1) - 1;

    // Compute number of "x" bits after data bits.
    nx = Tab_ACK_RI_NX[indQm];

    // Compute signal detection fractional scale factor.
    detscf = Tab_ACK_RI_DET[indQm + (3*nbits) - 3];

    //printf("Running ACK_RI_Decoder: nin=%d, Qm=%d, nbits=%d\n", nin, Qm, nbits);
    //printf("indQm=%d, scf=%d, nx=%d, detscf=%d\n", indQm, scf, nx, detscf);

    // Initialize data/x bit counter.
    ncnt = 2;

    // Initialize data bit counter.
    ncntd = 0;

    // For each received vector of soft decisions.
    for(i = 0; i < nin; i++)
    {
        // Invert soft decision.
        ac = -in[i];

        // Compute soft decision sign bit.
        sb = (ac >> 8) & 1;

        // Compute absolute value of soft decision.
        if(ac < 0)
            ac = -ac;

        // if input soft decision is for data bit (not x).
        if(ncnt > 0)
        {
            // Decrement bit counter.
            ncnt = ncnt - 1;

            // If counter expired, then reset it to number of x bits.
            if(ncnt == 0)
                ncnt = -nx;

            // If not x bits, then reset bit counter for 2 data bits.
            if(ncnt == 0)
                ncnt = 2;

            // Increment and limit data bit summation.
            ac = ac + sumd[ncntd][sb];
            if(ac > 32767)
                ac = 32767;
            sumd[ncntd][sb] = ac;

            // If 2-bit ACK or RI, then.
            if(nbits > 1)
            {
                // Increment and limit data bit counter.
                ncntd = ncntd + 1;
                if(ncntd > 2)
                    ncntd = 0;
            }

        }
        // Else, input soft decision is for x.
        else
        {
            // Increment bit counter.
            ncnt = ncnt + 1;

            // If counter expired, then reset it for 2 data bits.
            if(ncnt == 0)
                ncnt = 2;

            // Increment and limit x bit summation.
            ac = ac + sumx[sb];
            if(ac > 32767)
                ac = 32767;
            sumx[sb] = ac;
        }
    }

    // Initialize output data bits.
    *out = 0;

    // Initialize signal and noise summations.
    sums = sumx[1];
    sumn = sumx[0];

    // Initialize data bit value;
    sb = 0x80;

    // For each ACK or RI bit.
    for(i = 0; i < nbits; i++)
    {
        // If "1" summation > "0" summation, then.
        if(sumd[i][1] > sumd[i][0])
        {
            // Add "1" summation to signal.
            sums = sums + sumd[i][1];

            // Add "0" summation to noise.
            sumn = sumn + sumd[i][0];

            // Set output data bit to 1.
            *out = (U8) (*out | sb);
        }
        else
        {
            // Add "0" summation to signal.
            sums = sums + sumd[i][0];

            // Add "1" summation to noise.
            sumn = sumn + sumd[i][1];

        }

        // Compute next data bit value.
        sb = sb >> 1;

    }

    // If 2 ACK or RI bits, then.
    if(nbits > 1)
    {

        // If output parity should be 0, then.
        if((*out == 0) || (*out == 0xC0) )
        {
            // Add "0" parity summation to signal.
            sums = sums + sumd[2][0];

            // Add "1" parity summation to noise.
            sumn = sumn + sumd[2][1];
        }
        else
        {
            // Add "1" parity summation to signal.
            sums = sums + sumd[2][1];

            // Add "0" parity summation to noise.
            sumn = sumn + sumd[2][0];
        }

    }

    // Limit signal and noise summations.
    if(sums > 32767)
        sums = 32767;
    if(sumn > 32767)
        sumn = 32767;

    // Scale signal using detection threshold.
    ac = sums*detscf;

    // If scaled signal > noise, then set valid signal bit.
    if(ac >= (sumn << 15))
        *out = *out | 0x20;

#if 0   // This section is used only to collect test statistics

    fpsnr = sums;
    fpsnr = fpsnr/sumn;
    if(fpsnr > fpmaxsnr) fpmaxsnr = fpsnr;
    if(fpsnr < fpminsnr) fpminsnr = fpsnr;
    printf("fpmaxsnr = %f, fpminsnr = %f\n", fpmaxsnr, fpminsnr);

    if( (*out & 0x20) != 0)
    {

        for(i = 0; i < nin; i++)
        {
            printf("in[%d]=%d\n", i, in[i]);
        }
        printf("out=%x\n", *out);
        if(nbits == 1)
        {
            printf("sumd[0][0]=%d, sumd[0][1]=%d\n", sumd[0][0], sumd[0][1]);
        }
        else
        {
            printf("sumd[0][0]=%d, sumd[0][1]=%d\n", sumd[0][0], sumd[0][1]);
            printf("sumd[1][0]=%d, sumd[1][1]=%d\n", sumd[1][0], sumd[1][1]);
            printf("sumd[2][0]=%d, sumd[2][1]=%d\n", sumd[2][0], sumd[2][1]);
        }
        printf("sumx[0]=%d, sumx[1]=%d\n", sumx[0], sumx[1]);
        printf("sums=%d, sumn=%d\n", sums, sumn);

        i = getchar();

    }

#endif

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE PUSCH RI and CQI decoder function.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     pFecParams - FEC Input/Output structure for all users.  RI and CQI bits will be
 *                          written to buffers in structure for each user.
 *  @param     pMuxSoftDec - Soft decision buffer for RI and CQI for all users.
 *  @param     dmemTop     - Beginning address for temporary Ceva memory
 *  @param     tablesRM    - Table of pointers to tables for Reed Muller Decoder.
 *  @param     FHT_MASK_IN - Pointer to table for Fast Hadamard Transform Reed Muller Decoder.
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pFecParams     |               |output       |               |pStructure    |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pMuxSoftDec    |               |input        |               |pReal         |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |dmemTop        |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |tablesRM       |               |input        |11             |pReal         |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |FHT_MASK_IN    |               |input        |               |Real          |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |bs_rx_mux_RI_CQI_decoding          |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |pMuxSoftDec, tablesRM, FHT_MASK_IN |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |pFecParams                         |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |none                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |none                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function will take soft decisions for RI and CQI and decode
 * for all users.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_mux_RI_CQI_decoding(PULRXFECPARAMS pFecParams, S8 *pMuxSoftDec, U32 dmemTop,
                                S8 *tablesRM[], S8 FHT_MASK_IN[])
{
    U8 *outcqi, *outri;
    S8 *tmpcmbcqi;
    S32 nincqi, ninri;
    U16 noutcqi, noutri;
    U8 cqistat[1] = { 0 };
    U16 cqiwrd, Qm;
    S32 nc;
    S8* RM_decTable = NULL, *RM_TableIn = NULL;
    U16 RM_decSize = 0;
    U32 memTop = RUP32B(dmemTop);
    S32 off_RI, off_CQI;

#ifdef CEVA_INTRINSICS
    RM_TableIn = (S8*)(DataBuff_Bank3);

    // Allocate temporary space for CQI combined soft-decision array
    tmpcmbcqi = (S8*) memTop;
    memTop += 32 * sizeof(S8);
#else
    memTop = 0;
    RM_TableIn = (S8*) MemAlloc(MAX_RM_DECODER_TABLE_SIZE * sizeof(S8), DDR_MEM, 0);

    // Allocate temporary space for CQI combined soft-decision array
    tmpcmbcqi = (S8*) MemAlloc(32 * sizeof(S8), DDR_MEM, 0);
#endif  //CEVA_INTRINSICS

    for(nc = 0; nc < pFecParams->numChan; nc++)
    {
        if ((pFecParams->channelType[nc] == PUSCH) &&
            (pFecParams->muxPUSCH_User[nc] == 1))
        {
            outcqi = &pFecParams->cqiStore[nc][0];
            outri  = &pFecParams->riStore[nc];

            nincqi  = pFecParams->numbitsCQI[nc];
            ninri   = pFecParams->numbitsRI[nc];
            noutcqi = pFecParams->nr1CQI[nc] + pFecParams->nrg1CQI[nc];
            noutri  = pFecParams->nRI[nc];
            Qm      = pFecParams->Qm[nc];

            off_RI  = pFecParams->RI_User_offset[nc];
            off_CQI = pFecParams->CQI_User_offset[nc] + pFecParams->TotalRI_Bits;

            pFecParams->rmconf[nc] = 1;               // Good Confidence (default)

            // If there are CQI bits and no more than 11 decoded bits, then
            if ((noutcqi <= 11) && (noutcqi > 0))
            {
                StartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_SFCOMB);
                // Compute CQI soft decisions and combine multiple values
                // if circular repetition is being used
                SFDECCOMB(&pMuxSoftDec[off_CQI], tmpcmbcqi, nincqi, 32);
                // cqiwrd bits 15 through 8 contain CQI bits 0 through 7
                // cqiwrd bits 7 through 5 contain CQI bits 8 through 10
                // Run new Reed-Muller (32, X) decoder
                StopSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_SFCOMB);

                if(noutcqi > 10)
                {
                    StartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_DMA_IN);
                    // Run new Reed-Muller (32, X) decoder
                    RM_decTable = (S8*) tablesRM[noutcqi - 1];
                    RM_decSize =  rm_dectab_32_X_size[noutcqi - 1];

                    MemCpyFromExt((void*) RM_TableIn, (void*) RM_decTable, RM_decSize);

                    StopAndStartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_DMA_IN, SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER);

                    Decode_Reed_Muller(tmpcmbcqi, &cqiwrd, 32, noutcqi, RM_TableIn, rm_32_11_enctab);

                    // Compute Reed Muller decoder output confidence indicators
                    pFecParams->rmconf[nc] = Decode_Reed_Muller_Conf(tmpcmbcqi, cqiwrd, 32, (S16)noutcqi, rm_32_11_enctab,
                                                     &RM_MLOG_CONF_32_X_TAB[noutcqi-1][0], 13, RM_MRDERR_32_X[noutcqi-1]);

                    StopSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER);
                }
                else
                {
                    StartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_FHT_DMA_IN);
                    if(noutcqi > 6)
                    {
                        RM_decSize = 32<<(noutcqi-6);
                        MemCpyFromExt((void*) RM_TableIn, (void*) FHT_MASK_IN, RM_decSize);
                    }

                    StopAndStartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_FHT_DMA_IN, SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_FHT);

                    Decode_Reed_Muller_FHT(tmpcmbcqi, &cqiwrd, 32, noutcqi, RM_TableIn);

                    // Compute Reed Muller decoder output confidence indicators
                    pFecParams->rmconf[nc] = Decode_Reed_Muller_Conf(tmpcmbcqi, cqiwrd, 32, (S16)noutcqi, rm_32_11_enctab,
                                                     &RM_FHT_CONF_32_X_TAB[noutcqi-1][0], 13, RM_MRDERR_32_X[noutcqi-1]);

                    StopSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_FHT);
                }

                // Copy decoded CQI bits into CQI output array
                outcqi[0] = (U8) ((cqiwrd >> 8) & 255);
                outcqi[1] = (U8) cqiwrd & 255;

            }
            // Else if there are more than 11 decoded CQI bits, then
            else if (noutcqi > 0)
            {
                // Decode CQI bits using 1/3 rate CC decoder as described in the
                // 3GPP (LTE) standard TS 36.212 Section 5.2.2.6
                StartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_CC_CHAN_CODING);
                bs_rx_cc_cont_chan_coding(&pMuxSoftDec[off_CQI], outcqi, nincqi, noutcqi, cqistat, memTop);
                StopSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_CC_CHAN_CODING);
            }

            // If there are RI bits, then
            StartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_REED_RI_DEC);
            if (noutri > 0)
            {
                // Run ACK/RI soft-decision decoder to decode RI bits
                ACK_RI_Decoder(&pMuxSoftDec[off_RI], outri, ninri, Qm, noutri);
            }
            StopSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_REED_RI_DEC);
        }
    }

#ifndef CEVA_INTRINSICS
    // De-allocate temporary space
    MemFree(RM_TableIn, DDR_MEM);
    MemFree(tmpcmbcqi, DDR_MEM);
#endif

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE PUSCH ACK decoder function.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     pFecParams - FEC Input/Output structure for all users.  ACK bits will be
 *                          written to buffers in structure for each user.
 *  @param     pMuxSoftDec - Soft decision buffer for ACK for all users.
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pFecParams     |               |output       |               |pStructure    |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pMuxSoftDec    |               |input        |               |pReal         |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |dmemTop        |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |tablesRM       |               |input        |11             |pReal         |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |FHT_MASK_IN    |               |input        |               |Real          |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |bs_rx_mux_ACK_decoding             |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |pMuxSoftDec                        |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |pFecParams                         |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |none                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |none                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function will take soft decisions for ACK and decode
 * for all users.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_mux_ACK_decoding(PULRXFECPARAMS pFecParams, S8 *pMuxSoftDec)
{
    U8 *outack;
    S32 ninack, nc;
    U16 noutack, Qm;
    S32 off_ACK;

    for(nc = 0; nc < pFecParams->numChan; nc++)
    {
        if ((pFecParams->channelType[nc] == PUSCH) &&
            (pFecParams->muxPUSCH_User[nc] == 1))
        {
            outack   = &pFecParams->ackStore[nc];
            noutack  = pFecParams->nACK[nc];

            ninack  = pFecParams->numbitsACK[nc];
            Qm      = pFecParams->Qm[nc];

            off_ACK = pFecParams->ACK_User_offset[nc];
            // If there are ACK bits, then
            if (noutack > 0)
            {
                // Run ACK soft-decision decoder to decode ACK bits
                ACK_RI_Decoder(&pMuxSoftDec[off_ACK], outack, ninack, Qm, noutack);
            }
        }
    }

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE receive CC encoded control channel coding processing.<BR>
 *  Function bs_rx_cc_cont_chan_coding implements the receive channel coding
 *  processing for convolutionally encoded uplink control channels.
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param     in      Input 8-bit 2-s complement soft-decison array organized as
 *                     bytes.
 *  @param     out     Output data array organized as bytes.  Data bits are
 *                     packed into bytes.  Bit order is msb first, lsb last.
 *                     Note, out must include an extra element to store 8-bit CRC.
 *  @param     nin     Length of soft-decision input array.
 *  @param     nout    Length of output data array in bits.
 *  @param     stat    Reported CRC status. 0=CRC error.  1=CRC ok.
 *  @param     dmemTop Temp Buffer to be used by CC Decoder
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout/8         |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |stat           |               |output       |1              |pReal         |(1:15:0)  |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |dmemTop        |               |input        |1              |Real          |(32:0:0)  |full Range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------
 *  |Block Name          |bs_rx_cc_cont_chan_coding      |
 *  ------------------------------------------------------
 *  |Input Stream(s)     |in                             |
 *  ------------------------------------------------------
 *  |Output Stream(s)    |out                            |
 *  ------------------------------------------------------
 *  |Config Parameter(s) |nin, nout, dmemTop             |
 *  ------------------------------------------------------
 *  |Status Parameter(s) |stat                           |
 *  ------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *
 * The entire channel coding process is defined in the 3GPP (LTE) standard TS
 * 36.212 Section 5.2.2.6 and is described below:
 *
 *     - Compute and append 8-bit CRC to input control bits.
 *     - Encode bits using 1/3 rate convolutional encoder to generate 3 output
 *     bit streams.
 *     - Sub-block interleave each of the 3 encoder output bit sterams.
 *     - Combine the 3 sub-block interleaved outputs to for one circular buffer
 *     and then select bits from circular buffer for modulation and transmission.
 *
 *    Function bs_rx_cc_cont_chan_coding performs the inverse of this processing
 *    as described below:
 *
 *     - Combine and re-map soft-decisions for received data bits to reconstruct
 *     original transmitted circular buffer organization.
 *     - Sub-block de-interleave the soft-decisons for each of the 3 encoded
 *    streams.
 *     - Decode data bits and CRC using 1/3 rate convolutional soft-decision
 *    decoder.
 *     - Re-compute and verify CRC and report results.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.6
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_cc_cont_chan_coding(S8 in[], U8 out[], S32 nin, S32 nout, U8 *stat, U32 dmemTop)
{
    U8 gencrc[2] = {0};
    S32 i, j;
    S32 m, n, nencout;
    S8 *decin;
    U8 *decout;
    U16 *decinmap;
    U32 BlockSize;
    U32 memTop = RUP32B(dmemTop);

    BlockSize = nout + 8;

    // Compute 1/3 rate encoder output length in bits
    nencout = 3*BlockSize;

#ifdef CEVA_INTRINSICS
    // Allocate temporary space for decoder input array
    decin = (S8*) memTop;
    memTop += nencout * sizeof(S8);

    // Allocate temporary space for decoder output array
    decout = (U8*) memTop;
    memTop += BlockSize * sizeof(S8);

    // Allocate temporary space for rate-matching mapping output array
    decinmap = (U16*) memTop;
    memTop += nin * sizeof(U16);
#else
    // Allocate temporary space for decoder input array
    decin    = (S8*) MemAlloc(nencout * sizeof(S8), DDR_MEM, 0);

    // Allocate temporary space for decoder output array
    decout   = (U8*) MemAlloc(BlockSize * sizeof(S8), DDR_MEM, 0);

    // Allocate temporary space for rate-matching mapping output array
    decinmap = (U16*) MemAlloc(nin * sizeof(U16), DDR_MEM, 0);
#endif

    // Run rate matching to determine how soft-decisions map into
    // decoder input array
    CC_Rx_Rate_Matching(BlockSize, nin, decinmap, memTop);

    // Clear decoder input soft-decision array
    for(i = 0; i < nencout; i++)
    {
        decin[i] = 0;
    }

    // For each input soft-decision
    for(i = 0; i < nin; i++)
    {
        // Determine which encoder output stream (d0, d1 or d2)
        j = (decinmap[i] >> 14) & 3;

        // Determine relative location within stream
        n = decinmap[i] & 0x3fff;

        // Compute corresponding decoder input array index
        m = n + n + n + j;

        // Add soft-decision to decoder input array element and limit to +-127
        j = decin[m] + in[i];

        if(j > 127)
            j = 127;
        if(j < -127)
            j = -127;

        decin[m] = (S8) (j);
    }

    // Run MindSpeed 1/3 rate CC soft-decision decoder
    cc_decoder_1_3(decin, decout, (S16) BlockSize, memTop);

    // Pack decoder output bits into output bytes
    PACKBB(decout, out, BlockSize, 0);

    // Use received data to re-compute CRC
    calc_gcrc8(out, gencrc, BlockSize);

    // Initialize output status = 0
    *stat = 0;

    // Set bit0 of status if CRC is correct
    if (gencrc[0] == 0)
        *stat = 1;

#ifndef CEVA_INTRINSICS
    // De-allocate temporary arrays
    MemFree(decin, DDR_MEM);
    MemFree(decout, DDR_MEM);
    MemFree(decinmap, DDR_MEM);
#endif

    return;
}

/******************************************************************************
 CC_Rx_Rate_Matching (LTE receive rate-matching function for CC channels)
 *******************************************************************************

 CC_Rx_Rate_Matching implements the rate matching function required for
 receiving CC (convolutionally coded) channels.  First, it is important to
 understand the corresponding processing steps performed by the transmitter
 which are described below.

 1) Encode input bits using 1/3 rate tail-biting convolutional encoder.  The
 encoder outputs three bit streams d0n, d1n and d2n.  This encoder is defined
 in the 3GPP (LTE) standard document TS 36.212 section 5.1.3.1.

 2) Perform sub-block interleaving on each of the three encoder output bit
 streams as described in the 3GPP (LTE) standard document TS 36.212 section
 5.1.4.2.1.

 3) Concatenate the three sub-block interleaver output streams to form one
 circular buffer.  Select elements from this circular buffer as described in the
 3GPP (LTE) standard document TS 36.212 section 5.1.4.2.2.

 The purpose of function CC_Rx_Rate_Matching is to determine the exact encoder
 output that was used for each of the elements read from the circular buffer.
 This is required so that the receiver's soft-decisions can be re-ordered
 properly for the decoder.  The functional calling format is shown below.

 CC_Rx_Rate_Matching(n, E, out)

 Where:  n =     Length of CC encoder input array (same as length of each
 of the encoder's output streams).
 E =     Number of bits read from circular buffer.
 out =   Output array containing encoder output stream indices used
 for each of the cicular buffer outputs.  The values are
 bit-mapped as shown below.

 Bits 0 - 13:    stream index (0 to n-1)
 Bits 14-15:     stream type (0, 1, or 2)

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE receive rate-matching function for CC channels.
 *
 *  <1> Hardware Resource:
 *  FEC
 *  @param     n =     Length of CC encoder input array (same as length of each
 *                     of the encoder's output streams).
 *  @param     E =     Number of bits read from circular buffer.
 *  @param     out =   Output array containing encoder output stream indices used
 *                     for each of the cicular buffer outputs.  The values are
 *                     bit-mapped as shown:
 *                     Bits 0 - 13:    stream index (0 to n-1)
 *                     Bits 14-15:     stream type (0, 1, or 2)
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |E              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |E              |pReal Array   |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |CC_Rx_Rate_Matching         |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |none                        |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n, E                        |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * CC_Rx_Rate_Matching implements the rate matching function required for
 * receiving CC (convolutionally coded) channels.  First, it is important to
 * understand the corresponding processing steps performed by the transmitter
 * which are described below.

 * -# Encode input bits using 1/3 rate tail-biting convolutional encoder.  The
 * encoder outputs three bit streams d0n, d1n and d2n.  This encoder is defined
 * in the 3GPP (LTE) standard document TS 36.212 section 5.1.3.1.
 * -# Perform sub-block interleaving on each of the three encoder output bit
 * streams as described in the 3GPP (LTE) standard document TS 36.212 section
 * 5.1.4.2.1.
 * -# Concatenate the three sub-block interleaver output streams to form one
 * circular buffer.  Select elements from this circular buffer as described in the
 * 3GPP (LTE) standard document TS 36.212 section 5.1.4.2.2.
 *
 * The purpose of function CC_Rx_Rate_Matching is to determine the exact encoder
 * output that was used for each of the elements read from the circular buffer.
 * This is required so that the receiver's soft-decisions can be re-ordered
 * properly for the decoder.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 section 5.1.3.1<BR>
 * [2] 3GPP (LTE) standard document TS 36.212 section 5.1.4.2.1<BR>
 * [3] 3GPP (LTE) standard document TS 36.212 section 5.1.4.2.2
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void CC_Rx_Rate_Matching(S32 n, S32 E, U16 out[], U32 dmemTop)
{
    S32 i, j, k, m;
    S32 nrow, nout, nfill;
#ifdef CEVA_INTRINSICS
    U32 memTop = RUP32B(dmemTop);
#endif
    // Define inter-column permutation table
    S16 P[32] =
    {  1, 17,  9, 25,  5, 21, 13, 29,  3, 19, 11,
      27,  7, 23, 15, 31,  0, 16,  8, 24,  4, 20,
      12, 28,  2, 18, 10, 26,  6, 22, 14, 30 };

    // define temporary buffers
    U16 *tmp1;
    U16 *tmp2;

    //*****************************************************************************
    // Perform sub-block interleaving for one CC encoder output stream.
    // Stream indices are used instead of real encoder outputs.

    // Compute number of rows with 32 columns each
    nrow = (n + 31) >> 5;

    // Compute interleaver buffer size = 32*nrow
    nout = nrow << 5;

#ifdef CEVA_INTRINSICS
    tmp1 = (U16*) memTop;
    memTop += nout * sizeof(U16);
    tmp2 = (U16*) memTop;
    memTop += nout * sizeof(U16);
#else
    // Allocate sub-block interleaver arrays
    tmp1 = (U16*) MemAlloc(nout * sizeof(U16), DDR_MEM, 0);
    tmp2 = (U16*) MemAlloc(nout * sizeof(U16), DDR_MEM, 0);
#endif

    // Compute number of null filler bits
    nfill = nout - n;

    // Initialize temporary input buffer index
    j = 0;

    // If needed, load nulls into sub-block interleaver input array.
    // Use bit15=1 to indicate null
    for (i = 0; i < nfill; i++)
    {
        tmp1[j] = 0x8000;
        j = j + 1;
    }

    // Load encoder output indices into sub-block interleaver input array.
    for (i = 0; i < n; i++)
    {
        tmp1[j] = (U16) i;
        j = j + 1;
    }

    // Initialize sub-block interleaver output array index
    m = 0;

    // Do for each column
    for (i = 0; i < 32; i++)
    {
        // Do for each row
        for (j = 0; j < nrow; j++)
        {
            // Compute input buffer index to use for next output
            k = P[i] + (j << 5);
            // Copy sub-block interleaver input to output array
            tmp2[m] = tmp1[k];
            // Increment output array index
            m = m + 1;
        }
    }

    //*****************************************************************************
    // Perform bit collection, selection and rate matching.

    // Initialize output array index
    i = 0;
    // Initialize input array index
    j = 0;
    // Initialize encoder output stream type (0, 1 or 2)
    k = 0;

    laba:

    // If input is not a null, then
    if ((tmp2[j] & 0x8000) == 0)
    {
        // Use encoder output index as final output
        out[i] = (U16) (tmp2[j] | (k << 14));
        // Increment output array index
        i = i + 1;
        // If all outputs selected, then exit
        if (i == E)
            goto labb;
    }

    // Increment input array index
    j = j + 1;

    // If last element of input array, then
    if (j == nout)
    {
        // Increment encoder output stream type
        k = k + 1;
        if (k == 3)
        {
            k = 0;
        }
        // Reset input pointer to beginning
        j = 0;
    }
    // Continue to select remaining output bits
    goto laba;

    labb:

#ifndef CEVA_INTRINSICS
    // De-allocate temporary arrays
    MemFree(tmp1, DDR_MEM);
    MemFree(tmp2, DDR_MEM);
#endif

    return;
}

/******************************************************************************
 cc_decoder_1_3 (LTE 1/3 Rate Convolutional Decoder)
 ******************************************************************************

 Function cc_decoder_1_3() implements the 1/3 rate convolutional decoder
 required by the LTE standard.  The tail-biting CC decoder is based on
 a circular Viterbi algorithm (CVA).  The basic ideas are that the received
 block of coded symbols with soft decision information from the demodulator
 (and with channel state information for a fading channel) is recorded at the
 receiver.  Then apply the Viterbi algorithm for continuous decoding to a
 sequence of repeated blocks, thus simulating the situation where the
 information is transmitted repeatedly in block form. The decoder can start in
 one arbitrary state (e.g.  zero state) or in all states with the same metric
 start value.

 Stopping Rule: After the first N (block length) Viterbi updates are completed,
 begin comparing  decision words separated by N symbol times. When observe that
 M consecutive decision words have been the same as their predecessors, stop
 the updates, where M is the code memory. If the condition is not met after
 (N+ 2*TRACEBACK_LEN)) Viterbi updates,  then stop and flag a potential block
 error being detected (FAIL).

 For more info, see: "An Efficient Adaptive Circular Viterbi Algorithm for
 Decoding Generalized Tailbiting Convolutional Codes"  by Richard V. Cox, and
 Carl-Erik W. Sundberg IEEE Trans Vehicular Tech Vol.43. No. I , Feb 1994,
 pp. 57-68.

 The calling format for cc_decoder_1_3 is shown below.

 cc_decoder_1_3(IN, OUT, N)

 Where:  IN = input array containing d0, d1, d2 soft decisions
 (organized as bytes).  Note, positive values correspond
 to a "1" and negative values correspond to a "0".
 OUT = output data bit array (organized as bytes, lsb = data).
 N = number of output data bits = number of (d0, d1, d2) inputs.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE 1/3 rate convolutional decoder.
 *
 *  <1> Hardware Resource:
 *  FEC
 *
 *  @param     IN =      Input array containing d0, d1 d2 soft decisions
 *                       (organized as bytes).  Note, positive values correspond
 *                       to a "1" and negative values correspond to a "0".
 *
 *  @param     OUT =     Output data bit array (organized as bytes, lsb = data).
 *  @param     N =       Number of output data bits = number of (d0, d1, d2) inputs.
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |IN             |               |input        |3N             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |OUT            |               |output       |N              |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |N              |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |cc_decoder_1_3              |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |IN                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |OUT                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |N                           |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function implements the 1/3 rate convolutional decoder
 * required by the LTE standard.  The tail-biting CC decoder is based on
 * a circular Viterbi algorithm (CVA).  The basic ideas are that the received
 * block of coded symbols with soft decision information from the demodulator
 * (and with channel state information for a fading channel) is recorded at the
 * receiver.  Then apply the Viterbi algorithm for continuous decoding to a
 * sequence of repeated blocks, thus simulating the situation where the
 * information is transmitted repeatedly in block form. The decoder can start in
 * one arbitrary state (e.g.  zero state) or in all states with the same metric
 * start value.<BR><BR>
 *
 * Stopping Rule: After the first N (block length) Viterbi updates are completed,
 * begin comparing  decision words separated by N symbol times. When observe that
 * M consecutive decision words have been the same as their predecessors, stop
 * the updates, where M is the code memory. If the condition is not met after
 * (N+ 2*TRACEBACK_LEN)) Viterbi updates,  then stop and flag a potential block
 * error being detected (FAIL).<BR><BR>
 *
 * For more info, see: "An Efficient Adaptive Circular Viterbi Algorithm for
 * Decoding Generalized Tailbiting Convolutional Codes"  by Richard V. Cox, and
 * Carl-Erik W. Sundberg IEEE Trans Vehicular Tech Vol.43. No. I , Feb 1994,
 * pp. 57-68.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] An Efficient Adaptive Circular Viterbi Algorithm for Decoding Generalized
 * Tailbiting Convolutional Codes"  by Richard V. Cox, and Carl-Erik W. Sundberg
 * IEEE Trans Vehicular Tech Vol.43. No. I , Feb 1994, pp. 57-68.
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------

void cc_decoder_1_3(S8 IN[], U8 OUT[], S16 N, U32 dmemTop)
{
    //*****************************************************************************
    //**********           OPERATIONAL PARAMETER DEFINITIONS             **********
    //*****************************************************************************

#define P_LN              6             // 64 state trellis
#define P_NSTAT           (1<<P_LN)     // number of states
#define P_TBLEN           P_NSTAT       // decoder trace-back length

    //*****************************************************************************

    S16 min_state = 0, i, ii;
    S16 temp[8], min_metrc;
    S16 chkexit, cntexit;
    S16 x, y, z;
    S16 xp, xm, yp, ym, zp, zm;
    S16 tblen;
    S16 *tb_buf;
    S16 *metric;
    S16 *newmetric;

    S16 adra, adralim, adrb, adrblim, adrc;
    S16 state, oldstate;

#ifdef CEVA_INTRINSICS
    U32 memTop = RUP32B(dmemTop);
#endif

    // Initialize trace-back length
    tblen = P_TBLEN;

    // Be sure trace-back length is not > N
    if (N < tblen)
    {
        tblen = N;
    }

    // Compute counter to check for possible early termination
    chkexit = N + tblen + 1;

    // Initialize early termination counter
    cntexit = 0;

#ifdef CEVA_INTRINSICS
    metric = (S16*) memTop;
    memTop += P_NSTAT * sizeof(S16);
    newmetric = (S16*) memTop;
    memTop += P_NSTAT * sizeof(S16);
    tb_buf = (S16*) memTop;
    memTop += (P_NSTAT + 1) * N  * sizeof(S16);
#else
    // Allocation space for temporary arrays
    metric    = MemAlloc(P_NSTAT * sizeof(S16), DDR_MEM, 0);
    newmetric = MemAlloc(P_NSTAT * sizeof(S16), DDR_MEM, 0);
    tb_buf    = MemAlloc((P_NSTAT + 1) * N * sizeof(S16), DDR_MEM, 0);
#endif

    // Initialize each state metric to 0
    for (i = 0; i < P_NSTAT; i++)
    {
        metric[i] = 0; // start from zero state

    }

    // Initialize input array index and index limit
    adra = 0;
    adralim = 3*N;

    // Initialize traceback buffer index and index limit
    adrb = 0;
    adrblim = (P_NSTAT + 1)*N;

    // For each (d0, d1, d2) + (trace-back)
    for (i = 0; i < N + (tblen * 2); i++)
    {

        // Read d0 soft decision
        x = IN[adra];

        adra = adra + 1;

        // Read d1 soft decision
        y = IN[adra];

        adra = adra + 1;

        // Read d2 soft decision
        z = IN[adra];

        adra = adra + 1;

        // Reset input array address when it hits limit
        if(adra == adralim) adra = 0;

        // Compute bit value confidences
        xp = abs(x + 127);
        xm = abs(x - 127);
        yp = abs(y + 127);
        ym = abs(y - 127);
        zp = abs(z + 127);
        zm = abs(z - 127);

        // Compute possible distances for (zyx = 000, 001, 010, 011, 100, 101, 110, 111)
        temp[0] = zp + yp + xp;
        temp[1] = zp + yp + xm;
        temp[2] = zp + ym + xp;
        temp[3] = zp + ym + xm;
        temp[4] = zm + yp + xp;
        temp[5] = zm + yp + xm;
        temp[6] = zm + ym + xp;
        temp[7] = zm + ym + xm;

        // Initialize minimum metric value
        min_metrc = 32767;

        // Initialize old state
        oldstate = 0;

        // Initialize traceback buffer index
        adrc = adrb;

        // For each possible state
        for (ii = 0; ii < P_NSTAT; ii++)
        {

            // Compute metric for 0 and 1
            x = temp[cc_1_3_dectab[ii][0]] + metric[oldstate];
            y = temp[cc_1_3_dectab[ii][1]] + metric[oldstate + 1];

            // If 0 metric better than 1, then use 0 metric and state
            if (x <= y)
            {
                newmetric[ii] = x;
                tb_buf[adrc] = oldstate;

                adrc = adrc + 1;

                // Update minimum metric and state
                if (min_metrc > x)
                {
                    min_metrc = x;
                    min_state = ii;
                }

            }
            // Else, use 1 metric and state
            else
            {
                newmetric[ii] = y;
                tb_buf[adrc] = oldstate + 1;

                adrc = adrc + 1;

                // Update minimum metric and state
                if (min_metrc > y)
                {
                    min_metrc = y;
                    min_state = ii;
                }

            }

            // Increment and limit old state and old state + 1;
            oldstate = (2 + oldstate) & (P_NSTAT - 1);

        }

        // If check for early termination counter has not expired
        if(chkexit > 0)
        {
            // Decrement counter
            chkexit = chkexit - 1;
        }
        // Else, check for possible early termination
        else
        {
            // If traceback ok, then
            if (tb_buf[P_NSTAT + adrb] == min_state)
            {
                // Increment early termination counter
                cntexit = cntexit + 1;

                // If conditions allow, then terminate early
                if (cntexit > P_LN)
                {
                    //printf("!!!!!!!!!! Early break from cc_decoder_1_3 !!!!!!!!!!\n");

                    break; // early stop
                }
            }
            // Else, traceback not good enough
            else
            {
                // Reset early termination counter
                cntexit = 0;
            }

        }

        // Store minimum/best state in traceback buffer
        tb_buf[P_NSTAT + adrb] = min_state;

        // Update metrics and normalize to prevent overflows
        for (ii = 0; ii < P_NSTAT; ii++)
        {
            //printf("newmetric[%d]=%d, min_metrc=%d\n", state, newmetric[ii], min_metrc);

            metric[ii] = newmetric[ii] - min_metrc;

        }

        // Increment and limit traceback buffer index
        adrb = adrb + (P_NSTAT + 1);
        if(adrb == adrblim) adrb = 0;

    }

    // Initalize traceback buffer index
    adrb = tblen*(P_NSTAT + 1);
    if(adrb == adrblim) adrb = 0;

    // For each decoder output bit
    for (i = 0; i < N; i++)
    {
        // Get surviving state from traceback buffer
        state = tb_buf[P_NSTAT + adrb];

        // Initialize trackback buffer index
        adrc = adrb;

        // Check traceback buffer for best surviving state
        for (ii = 0; ii < tblen; ii++)
        {

            state = tb_buf[state + adrc];

            // Update and limit traceback buffer index
            adrc = adrc - (P_NSTAT + 1);
            if(adrc < 0) adrc = adrc + adrblim;
        }

        // Store next decoder output bit (1 bit per byte)
        OUT[i] = (U8) ((state >> (P_LN - 1)) & 1);

        // Increment and limit traceback buffer index
        adrb = adrb + (P_NSTAT + 1);
        if(adrb == adrblim) adrb = 0;

    }

#ifndef CEVA_INTRINSICS
    // De-allocate space for temporary arrays
    MemFree(metric, DDR_MEM);
    MemFree(newmetric, DDR_MEM);
    MemFree(tb_buf, DDR_MEM);
#endif

    return;
}

//----------------------------------------------------------------------------------------------------
/** @brief PUCCH Rx SDU processing function
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param     chanNum          - Channel Number of User
 *  @param     bUserDetSoft     - Output LLR from Rx DeMapper
 *  @param     pUlRxInputParams - Structure containing Input Parameters for all users
 *  @param     pUlRxSduParams   - RxSdu structure containing input/output parameters for all users
 *  @param     pucch_by         - temp buffer for LLRs
 *  @param     dec_tab20        - Pointer to entire Reed Muller Decoder table
 *  @param     dec_tab20_offset - Array of pointers to point to offset in Reed Muller Decoder table
 *                                for all users
 *  @param     MASK             - Table for new Fast Hadamard Transform Reed Muller Decoder
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |IN             |               |input        |N              |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |OUT            |               |output       |(N+7)/8        |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |N              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |POS            |               |input        |1              |Real          |(1:15:0)  |[0:7]          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------
 *  |Block Name          |BsRx_PucchSduProcessing |
 *  ------------------------------
 *  |Input Stream(s)     |IN     |
 *  ------------------------------
 *  |Output Stream(s)    |OUT    |
 *  ------------------------------
 *  |Config Parameter(s) |N,POS  |
 *  ------------------------------
 *  |Status Parameter(s) |none   |
 *  ------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * PACKBB extracts single bits from byte input array elements and packs the
 * bits into an output array of bytes.  The bit packing order is msb (bit7) first
 * to lsb (bit0) last.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
U32 BsRx_PucchSduProcessing(S32 chanNum, S8 *bUserDetSoft,
                          ULRXINPUTPARAMS *pUlRxInputParams, ULRXSDUPARAMS *pUlRxSduParams,
                          S8 pucch_by[], S8 dec_tab20[], S16 dec_tab20_offset[], S8 MASK[])
{
    S16 cond1, cond2, cond3;
    S16 format_type;
    U32 status = 0;
    S16 stat;
    U8 *pucch_mac_sdu;
    S8 *ptr_tab;
    S16 offset;
    U16 numBits;

#ifdef CEVA_INTRINSICS
    U16 size;
    S8* decTab;
#endif

    format_type   = pUlRxInputParams->pucchFormat[chanNum];
    pucch_mac_sdu = &pUlRxSduParams->pucch_mac_sdu[chanNum][0];

    stat = STAT_SUCCESS;   //PUCCH does not have CRC checking, so just set stat = STAT_SUCCESS
    if ((format_type == FORMAT2) || (format_type == FORMAT2A) || (format_type == FORMAT2B))
    {
        memcpy(&pucch_by[0], bUserDetSoft, 22);
    }
    else
    {
        pucch_by[0] = bUserDetSoft[0];
        pucch_by[1] = bUserDetSoft[1];

    }
    //if dead zone no sdu reporting
    cond1 =  format_type == FORMAT1   && pucch_by[0] == 0;
    cond2 =  format_type == FORMAT1A  && pucch_by[0] == 0;
    cond3 = (format_type == FORMAT1B) &&
            (pucch_by[0] == 0) && (pucch_by[1] == 0);

    if (cond1 == 0 && cond2 == 0 && cond3 == 0)
    {
        numBits = (U16) pUlRxSduParams->dlCqiPmiSizeBits[chanNum];

#ifndef CEVA_INTRINSICS
        ptr_tab = MASK;
        if(numBits > 9 && format_type >= FORMAT2)
        {
            offset = dec_tab20_offset[numBits - 3];
            ptr_tab = dec_tab20 + offset;
        }
#else
        if(numBits > 6 && format_type >= FORMAT2)
        {
            ptr_tab = (S8*) (DataBuff_Bank3);
            if(numBits < 10)
            {
                decTab = MASK;
                size = 32<<(numBits-6);
            }
            else
            {
                offset = rm_dectab_20_X_offset[numBits - 3];
                decTab = dec_tab20 + offset;
                size = rm_dectab_20_X_size[numBits - 2];
            }

            MemCpyFromExt((void*) ptr_tab, (void*) decTab, size);
        }

#endif

        bs_rx_pucch_chan_coding(&pucch_by[0], pucch_mac_sdu, format_type,
                                (U16) pUlRxSduParams->dlCqiPmiSizeBits[chanNum], ptr_tab, &pUlRxInputParams->rmconf[chanNum]);

        // If format 2, 2a or 2b, then
        if(format_type >= FORMAT2)
        {
            // Compute correlation of received soft decisions and Reed-Muller encoded
            // received data.
            //corr_pucch_cqi = Detect_Reed_Muller(&pucch_by[0], pucch_mac_sdu,
            //                                    (S16) 20, (S16) pUlRxSduParams->dlCqiPmiSizeBits[chanNum],
            //                                    ptr_tab);

            //printf("corr_pucch_cqi = %d\n", corr_pucch_cqi);

            // If correlation too small, then a valid Reed-Muller CQI message
            // was not received and the CQI message status must be set to invalid.

            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // Note, the current threshold=48 was determined experimentally. 48
            // was chosen so that a CQI message will never falsely detected when
            // noise only is present.  However, a threshold=48 also causes some
            // valid CQI messages to be rejected under the conditions required by
            // the 36.141 8.3.2 PUCCH tests.  These tests require CQI detection
            // at SNRs of about -4dB.  However, using a threshold of 48, SNRs of
            // about 0dB are required to guarantee than no valid CQI messages are
            // rejected.
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            //if(corr_pucch_cqi <= 48)
            //    stat = STAT_PUCCH_DEADZONE;
        }
    }
    else
    {
        stat = STAT_PUCCH_DEADZONE;    //We are in deadzone, so don't send message
    }

    pUlRxSduParams->stat[chanNum] = (U8)stat;

    return status;
}

/******************************************************************************
 bs_rx__pucch_chan_coding (LTE Receive Channel Coding Processing For
 PUCCH (Physical Uplink Control Channels) )
 ******************************************************************************

 Function bs_rx_pucch_chan_coding implements the receive channel coding
 processing for physical uplink control channels.  The functional calling
 format is shown below.

 bs_rx_pucch_chan_coding(in, out, format)

 Where: in =     Input 8-bit 2-s complement soft-decison array organized as
                 bytes.
        out =    Output data array organized as bytes.  Data bits are
                 packed into bytes.  Bit order is msb first, lsb last.
        format = Format type (0:format 1, 1:format 1a, 2: format 1b,
                 3: format 2, 4: format 2a, 5: format 2b).

 The entire channel coding process for PUCCH is defined in the 3GPP (LTE)
 standard TS 36.212 Section 5.2.3 and is described below.

 There are six possible formats for the physical uplink control channels.  The
 number and type of bits processed and the channel coding type used depends on
 the specified format.  For the physical uplink control channels the following
 types of control bits are transmitted:

 1) CQI (Channel quality indication)
 2) RI (rank indication)
 3) ACK/NACK (HARQ acknowledgement)

 These bits are encoded using a variable (20, A) Reed-Muller code or
 pass-through (no encoding) or both.  The CQI and RI bits are always encoded
 using the Reed-Muller code.  However, the ACK/NACK bits can be combined
 together with the CQI and RI bits and then Reed-Muller encoded or else
 just passed through without any encoding.  The descriptions for the various
 formats are shown below.

 ---------------------------------------------------------------------------
 Format 1
 ---------------------------------------------------------------------------
 Number of CQI and RI bits:      0
 Number of ACK/NACK bits:        0
 Type of encoding:               None
 Number of encoded output bits:  0

 ---------------------------------------------------------------------------
 Format 1a
 ---------------------------------------------------------------------------
 Number of CQI and RI bits:      0
 Number of ACK/NACK bits:        1
 Type of encoding:               Pass-through
 Number of encoded output bits:  1

 ---------------------------------------------------------------------------
 Format 1b
 ---------------------------------------------------------------------------
 Number of CQI and RI bits:      0
 Number of ACK/NACK bits:        2
 Type of encoding:               Pass-through
 Number of encoded output bits:  2

 ---------------------------------------------------------------------------
 Format 2
 ---------------------------------------------------------------------------
 Number of CQI and RI bits:      5 to 11
 Number of ACK/NACK bits:        1 or 2
 Type of encoding:               (20, A) Reed-Muller
 Number of encoded output bits:  20

 ---------------------------------------------------------------------------
 Format 2a
 ---------------------------------------------------------------------------
 Number of CQI and RI bits:      5 to 11
 Number of ACK/NACK bits:        1
 Type of encoding:               (20, A) Reed-Muller for CQI and RI
                                 Pass-through for ACK/NACK
 Number of encoded output bits:  20 + 1

 ---------------------------------------------------------------------------
 Format 2b
 ---------------------------------------------------------------------------
 Number of CQI and RI bits:      5 to 11
 Number of ACK/NACK bits:        2
 Type of encoding:               (20, A) Reed-Muller for CQI and RI
                                 Pass-through for ACK/NACK
 Number of encoded output bits:  20 + 2

 ---------------------------------------------------------------------------

 The interpretation of the output array bits depends on the format as described
 below.

 ----------------------------------------------------------------------------
 Bit:       |7   6   5   4   3   2   1   0 | 7   6   5   4   3   2   1   0  |
            |            out[0]            |             out[1]             |
 ----------------------------------------------------------------------------

 Format 1:   X   X   X   X   X   X   X   X   X   X   X   X   X   X   X   X

 Format 1a:  X   X   X   X   X   X   X   X   X   X   X   X   X   A1  X   X

 Format 1b:  X   X   X   X   X   X   X   X   X   X   X   X   X   A1  A2  X

 Format 2:   C1  C2  C3  C4  C5  C6  C7  C8  C9  C10 C11 C12 C13 X   X   X

 Format 2a:  C1  C2  C3  C4  C5  C6  C7  C8  C9  C10 C11 C12 C13 A1  X   X

 Format 2b:  C1  C2  C3  C4  C5  C6  C7  C8  C9  C10 C11 C12 C13 A1  A2  X
 ----------------------------------------------------------------------------

 Where: Cn = CQI or RI or HARQ ACK/NACK bits.
        An = HARQ ACK/NACK bits.

 Notes: 1) The number of Cn bits can vary between 5 and 13.
        2) For format 2 the location of the HARQ ACK/NACK bits depends on the
           number of CQI and RI bits used.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE receive (PUCCH) physical uplink control channel coding processing.<BR>
 *  Function bs_rx_pucch_chan_coding implements the receive channel coding
 *  processing for physical uplink control channels.  The functional calling
 *  arguments are shown below.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param     in      Input 8-bit 2-s complement soft-decison array organized as
 *                     bytes.
 *  @param     out     Output data array organized as bytes.  Data bits are
 *                     packed into bytes.  Bit order is msb first, lsb last.
 *  @param     format  Format type (0:format 1, 1:format 1a, 2: format 1b,
 *                     3: format 2, 4: format 2a, 5: format 2b).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |22 maximum     |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |2              |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |format         |               |input        |1              |Real          |(1:15:0)  |{0,1,2,3,4,5}  |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------
 *  |Block Name          |bs_rx_pucch_chan_coding |
 *  -----------------------------------------------
 *  |Input Stream(s)     |in                      |
 *  -----------------------------------------------
 *  |Output Stream(s)    |out                     |
 *  -----------------------------------------------
 *  |Config Parameter(s) |format                  |
 *  -----------------------------------------------
 *  |Status Parameter(s) |none                    |
 *  -----------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *  The entire channel coding process for PUCCH is defined in the 3GPP (LTE)
 *  standard TS 36.212 Section 5.2.3 and is described below.<BR><BR>
 *
 *  There are six possible formats for the physical uplink control channels.  The
 *  number and type of bits processed and the channel coding type used depends on
 *  the specified format.  For the physical uplink control channels the following
 *  types of control bits are transmitted:<BR><BR>
 *
 *    - CQI (Channel quality indication)<BR>
 *    - RI (rank indication)<BR>
 *    - ACK/NACK (HARQ acknowledgement)<BR><BR>
 *
 *  These bits are encoded using a variable (20, A) Reed-Muller code or
 *  pass-through (no encoding) or both.  The CQI and RI bits are always encoded
 *  using the Reed-Muller code.  However, the ACK/NACK bits can be combined
 *  together with the CQI and RI bits and then Reed-Muller encoded or else
 *  just passed through without any encoding.  The descriptions for the various
 *  formats are shown below.
 *
 *  TABLE_START
 *
 *  ---------------------------------------------------------------------------------------
 *  |Format Type   |Number of CQI |Number of      |Type of Encoding    |Number of encoded |
 *  |              |and RI bits   |ACK/ NACK bits |                    |output bits       |
 *  ---------------------------------------------------------------------------------------
 *  |Format 1      |0             |0              |None                |0                 |
 *  ---------------------------------------------------------------------------------------
 *  |Format 1a     |0             |1              |Pass-through        |1                 |
 *  ---------------------------------------------------------------------------------------
 *  |Format 1b     |0             |2              |Pass-through        |2                 |
 *  ---------------------------------------------------------------------------------------
 *  |Format 2      |5 to 11       |1 or 2         |(20, A) Reed-Muller |20                |
 *  ---------------------------------------------------------------------------------------
 *  |Format 2a     |5 to 11       |1              |(20, A) Reed-Muller |20 + 1            |
 *  |              |              |               |for CQI and RI      |                  |
 *  |              |              |               |Pass-through for    |                  |
 *  |              |              |               |ACK/NACK            |                  |
 *  ---------------------------------------------------------------------------------------
 *  |Format 2b     |5 to 11       |2              |(20, A) Reed-Muller |20 + 2            |
 *  |              |              |               |for CQI and RI      |                  |
 *  |              |              |               |Pass-through for    |                  |
 *  |              |              |               |ACK/NACK            |                  |
 *  ---------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  The interpretation of the output array bits depends on the format as described below:
 *  TABLE_START
 *  ------------------------------------------------
 *  |Format Type   |Interpretation of Output Bits  |
 *  ------------------------------------------------
 *  |Format 1      |No output bits                 |
 *  ------------------------------------------------
 *  |Format 1a     |A1 = out[1] & 0x04             |
 *  ------------------------------------------------
 *  |Format 1b     |A1 = out[1] & 0x04             |
 *  |              |A2 = out[1] & 0x02             |
 *  ------------------------------------------------
 *  |Format 2      |C1 through C8 = out[0]         |
 *  |              |C9 through C13 = out[1] & 0xf8 |
 *  ------------------------------------------------
 *  |Format 2a     |C1 through C8 = out[0]         |
 *  |              |C9 through C13 = out[1] & 0xf8 |
 *  |              |A1 = out[1] & 0x04             |
 *  |              |                               |
 *  ------------------------------------------------
 *  |Format 2b     |C1 through C8 = out[0]         |
 *  |              |C9 through C13 = out[1] & 0xf8 |
 *  |              |A1 = out[1] & 0x04             |
 *  |              |A2 = out[1] & 0x02             |
 *  ------------------------------------------------
 *  TABLE_END
 *
 * Where:
 *  - Cn : CQI or RI or HARQ ACK/NACK bits.<BR>
 *  - An : HARQ ACK/NACK bits.<BR><BR>
 *
 * Notes:
 *   - The number of Cn bits can vary between 5 and 13.<BR>
 *   - For format 2 the location of the HARQ ACK/NACK bits depends on the
 *     number of CQI and RI bits used.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.3
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_pucch_chan_coding(S8 in[], U8 out[], S16 format, U16 numbitsOut, S8 dec_tab20[], U16 *rmconf)
{
    U16 rmout = 0;

    *rmconf = 1;                               // Good Confidence (default)

    // If PUCCH format 1
    if (format == FORMAT1)
    {
        //printf("bs_rx_pucch_chan_coding format 1 processing\n");
        // Initialize output bits to 0
        out[0] = 0;
        out[1] = in[0];

        // Just return since no channel coding is used
        return;
    }
    // if PUCCH format 1a of 1b
    else if ((format == FORMAT1A) || (format == FORMAT1B))
    {
        //printf("bs_rx_pucch_chan_coding format 1a or 1b processing\n");

        // Initialize output bits to 0
        out[0] = 0;
        out[1] = 0;

        // Set A1 bit using corresponding demodulator output sign bit
        if (in[0] >= 0)
            out[1] = out[1] | 0x04;

        // If format 1b, set A2 bit using corresponding demodulator output sign bit
        if ((format == FORMAT1B) && (in[1] >= 0))
            out[1] = out[1] | 0x02;
    }
    // Else if PUCCH formats 2, 2a or 2b
    else
    {
        //printf("bs_rx_pucch_chan_coding format 2, 2a or 2b processing\n");
        if(numbitsOut > 9)
        {
            // Run new Reed-Muller (20, X) decoder
            Decode_Reed_Muller(in, &rmout, 20, numbitsOut, dec_tab20, rm_20_13_enctab);

            // Compute Reed Muller decoder output confidence indicators
            // !!!!!!!!!! MAY NOT NEED FOR PUCCH !!!!!!!!!!
            *rmconf = Decode_Reed_Muller_Conf(in, rmout, 20, (S16)numbitsOut, rm_20_13_enctab,
                                             &RM_MLOG_CONF_20_X_TAB[numbitsOut-1][0], 9, RM_MRDERR_20_X[numbitsOut-1]);
        }
        else
        {
            S32 i;
            S8 tmp_in[32] = {0};

            for(i = 0;i < 20;i++)
                tmp_in[i] = in[i];

            Decode_Reed_Muller_FHT(tmp_in, &rmout, 32, numbitsOut, dec_tab20);

            // Compute Reed Muller decoder output confidence indicators
            // !!!!!!!!!! MAY NOT NEED FOR PUCCH !!!!!!!!!!
            *rmconf = Decode_Reed_Muller_Conf(tmp_in, rmout, 20, (S16)numbitsOut, rm_20_13_enctab,
                                             &RM_FHT_CONF_20_X_TAB[numbitsOut-1][0], 9, RM_MRDERR_20_X[numbitsOut-1]);

        }

        // Convert 16-bit Reed-Muller decoder output into bytes
        out[0] = (U8) ((rmout >> 8) & (0xFF));
        out[1] = (U8) (rmout & (0xF8));
        // If not format 2, set A1 bit using corresponding demodulator output sign bit
        if ((format != FORMAT2) && (in[20] >= 0))
            out[1] = out[1] | 0x04;

        // If format 2b, set A2 bit using corresponding demodulator output sign bit
        if ((format == FORMAT2B) && (in[21] >= 0))
            out[1] = out[1] | 0x02;

    }

    //printf("bs_rx_pucch_chan_coding out[0] = %x, out[1] = %x\n",out[0], out[1]);

    return;

}

/******************************************************************************
 Description:
 ------------

 Function Decode_Reed_Muller implements a general-purpose Reed-Muller decoder.
 The decoding process uses a decoding table that contains sets of equations
 for each output bit.  The equations use the decoder inputs which are 8-bit
 soft decisions that represent the received Reed-Muller encoded bits.
 The decoder output bits are packed into a 16-bit output word (msb first,
 lsb last).  The functional calling format is shown below.


 Decode_Reed_Muller(in, out, nin, nout, dectab, enctab)

 Where: in =    Input array containing 8-bit 2's complement soft
                decisions. Positive values indicate "1" value bits and
                negative values indicate "0" value bits.
        out =   Output containing packed decoder output bits (bit15 = 1st
                data bit, bit0 = last data bit).
        nin =   Length of input array.
        nout =  Number of output bits to be decoded.
        dectab = Decoding table.
        enctab = Encoding table.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Reed-Muller soft-decision decoder.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param  in =        Input array containing 8-bit 2's complement soft
 *                      decisions.  Positive values indicate "1" value bits and
 *                      negative values indicate "0" value bits.
 *  @param  out =       Output containing packed decoder output bits (bit15 =
 *                      1st data bit, bit0 = last data bit).
 *  @param  nin =       Length of input array.
 *  @param  nout =      Number of output bits to be decoded.
 *  @param  dectab =    Decoding table.
 *  @param  enctab =    Encoding table.
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal         |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |dectab         |               |input        |               |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |enctab         |               |input        |nout           |pReal Array   |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Decode_Reed_Muller          |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |nin, dectab, enctab         |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function Decode_Reed_Muller implements the Reed Muller decoding functions
 * described in the LTE standard (3GPP TS 36.212 V8.6.0).  The decoding process
 * uses 32 8-bit soft decisions and generates 11 output bits.  The inputs are
 * 8-bit 2's complement soft decisions derived from either the BPSK or QPSK
 * receiver.  The decoder output bits are packed into a 16-bit output word
 * (msb first, lsb last).
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void Decode_Reed_Muller(S8 in[], U16 *out, S32 nin, S32 nout, S8 dectab[], U32 enctab[])
{
    S32 ac, ac0, ac1, ac2, ac3;
    S32 i, j;
    S32 ind, nequ;
    U32 decbits, decval;
    U32 u0, u1;

    //*************************************************************************

    // Initialize decoding table index.
    ind = 1;

    // Read length of each equation from decoding table.
    nequ = dectab[0];

    // Initialize packed decoded data bits.
    decbits = 0;

    // Compute decoded data bit value.
    decval = (1 << (16 - nout));

    // Do for each output bit, except 1st one.
    for (i = 1; i < nout; i++)
    {
        // Initialize bit=0 and bit=1 confidence accumulators.
        ac0 = 0;
        ac1 = 0;

        // Shift packed decoded data left 1 bit.
        decbits = decbits << 1;

        laba:

        // Initialize combined soft decision bit value.
        ac2 = 0;

        // Initialize minimum soft decision confidence measure.
        ac3 = 32767;

        // Do for each term in equation.
        for (j = 0; j < nequ; j++)
        {
            // ac = soft decision corresponding to equation term.
            ac = in[dectab[ind]];

            // Combine soft decision into bit value.
            ac2 = ac2 ^ ac;

            // Determine minimum soft decision magnitude.
            ac = abs(ac);
            if (ac < ac3)
                ac3 = ac;

            // Increment decoding table index.
            ind = ind + 1;
        }

        // Combined soft decision sign bit is actual data bit.
        ac = ac2 & 0x80;

        // If bit=0, then add confidence measure to "0" confidence accumulator.
        if (ac == 0)
            ac0 = ac0 + ac3;

        // If bit=1, then add confidence measure to "1" confidence accumulator.
        if (ac != 0)
            ac1 = ac1 + ac3;

        // If still more equations (not=-1), then continue.
        if (dectab[ind] >= 0)
            goto laba;

        // Increment decoding table index.
        ind = ind + 1;

        // If "1" confidence > "0" confidence, then insert 1 into
        // packed decoded data bits.
        ac = ac1 - ac0;
        if (ac > 0)
            decbits = decbits | decval;

    }

    //*************************************************************************

    // Run corresponding encoder to remove decoded bits from input.

    // Initialize encoder output to all 0s.
    u0 = 0;

    // u1 = packed decoded data bits.
    u1 = decbits;

    // Do for each decoded data bit.
    for (i = 1; i < nout; i++)
    {
        // Test next decoded data bit.
        ac = u1 & 0x4000;

        // If  data bit = 1, then xor in corresponding encoding table
        // element.
        if (ac != 0)
            u0 = u0 ^ enctab[i];

        // Shift data left to next data bit.
        u1 = u1 << 1;
    }

    //*************************************************************************

    // After removing decoded bits from input, input should be
    // either all 0s or all 1s.  To determine which convert soft decisions
    // into hard decisions, remove decoded contributions and then count
    // remaining number of 1s.

    // Initialize #1s accumulator = 0.
    ac0 = 0;

    // For each of encoder output bits.
    for (i = 0; i < nin; i++)
    {
        // ac1 = soft decision sign bit = hard data bit inverted.
        ac1 = 1;
        ac2 = in[i];
        ac = ac2 & 0x80;
        if (ac != 0) ac1 = 0;

        // Remove bit contribution using encoder outputs.
        ac = u0 & 1;

        // If next encoder output bit = 1, then
        // invert soft decision hard data bit.
        if (ac != 0) ac1 = ac1 ^ 1;

        // Update #1s accumulator.
        ac0 = ac0 + ac1;

        // Shift encoder output to next bit.
        u0 = u0 >> 1;
    }

    // If more than nin/2 1s, then bit0 = 1 and conf = ac0.
    ac = ac0 + ac0 - nin;
    if (ac > 0) decbits = decbits | 0x8000;

    // Write decoded data bits to output.
    *out = (U16) (decbits & 0xffff);

    return;

}

//-------------------------------------------------------------------------------------------
/** @brief Reed-Muller soft-decision decoder.
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param  in =        Input array containing 8-bit 2's complement soft
 *                      decisions.  Positive values indicate "1" value bits and
 *                      negative values indicate "0" value bits.
 *  @param  out =       Output containing packed decoder output bits (bit15 =
 *                      1st data bit, bit0 = last data bit).
 *  @param  nin =       Length of input array.
 *  @param  nout =      Number of output bits to be decoded.
 *  @param  Mask =      Table with masking bits.
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal         |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Mask           |               |input        |               |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Decode_Reed_Muller          |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |nin, Mask                   |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function Decode_Reed_Muller_FHT implements the Reed Muller decoding functions
 * using Fast Hadamard Transform, described in the LTE standard (3GPP TS 36.212 V8.6.0).  The decoding process
 * uses 32 8-bit soft decisions and generates upto 13 output bits.  The inputs are
 * 8-bit 2's complement soft decisions derived from either the BPSK or QPSK
 * receiver.  The decoder output bits are packed into a 16-bit output word
 * (msb first, lsb last).  The function returns the decoder output confidence.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void Decode_Reed_Muller_FHT(S8 in[], U16 *out, S16 nin, S16 nout, S8 Mask[])
{
    S32 i,j,k,ind,ind1=0;
    S16 Dmax,temp;
    U16 tc,Mask_Size;
    S16 D[32],D1[32],D2[32];

    for(i = 0;i < nin;i++)
        D2[i] = -in[Interleaver[i]];

    if(nout <= 6)
    {
        // 1st stage

        for(j = 0,ind = 16;j < 16;j++,ind++)
        {
            D1[ind] = D2[j] - D2[ind];
            D1[j] = D2[j] + D2[ind];
        }

        // 2nd stage

        for(i = 0;i < 32;i+=16)
            for(j = i,ind = i+8;j < i+8;j++,ind++)
            {
                D[ind] = D1[j] - D1[ind];
                D[j] = D1[j] + D1[ind];
            }

        // 3rd stage

        for(i = 0;i < 32;i+=8)
            for(j = i,ind = i+4;j < i+4;j++,ind++)
            {
                D1[ind] = D[j] - D[ind];
                D1[j] = D[j] + D[ind];
            }


        // 4th stage

        for(i = 0;i < 32;i+=4)
            for(j = i,ind = i+2;j < i+2;j++,ind++)
            {
                D[ind] = D1[j] - D1[ind];
                D[j] = D1[j] + D1[ind];
            }


        // 5th stage

        for(i = 0;i < 32;i+=2)
            for(j = i,ind = i+1;j < i+1;j++,ind++)
            {
                D1[ind] = D[j] - D[ind];
                D1[j] = D[j] + D[ind];
            }


        tc = 1<<(nout-1);

        Dmax = D1[0];

        for(i = 1;i < tc;i++)
            if(abs(Dmax) < abs(D1[i]))
            {
                Dmax = D1[i];
                ind1 = i;
            }
    }
    else
    {
        Mask_Size = (1<<(nout-6));

        Dmax = 0;

        for(k = 0;k < Mask_Size;k++)
        {
            for(i = 0;i < nin;i++)
                D[i] = Mask[(k<<5)+i] == 1 ? -D2[i]:D2[i];

            // 1st stage

        for(j = 0,ind = 16;j < 16;j++,ind++)
        {
            D1[ind] = D[j] - D[ind];
            D1[j] = D[j] + D[ind];
        }

        // 2nd stage

        for(i = 0;i < 32;i+=16)
            for(j = i,ind = i+8;j < i+8;j++,ind++)
            {
                D[ind] = D1[j] - D1[ind];
                D[j] = D1[j] + D1[ind];
            }

        // 3rd stage

        for(i = 0;i < 32;i+=8)
            for(j = i,ind = i+4;j < i+4;j++,ind++)
            {
                D1[ind] = D[j] - D[ind];
                D1[j] = D[j] + D[ind];
            }


        // 4th stage

        for(i = 0;i < 32;i+=4)
            for(j = i,ind = i+2;j < i+2;j++,ind++)
            {
                D[ind] = D1[j] - D1[ind];
                D[j] = D1[j] + D1[ind];
            }


        // 5th stage

        for(i = 0;i < 32;i+=2)
            for(j = i,ind = i+1;j < i+1;j++,ind++)
            {
                D1[ind] = D[j] - D[ind];
                D1[j] = D[j] + D[ind];
            }


            for(i = 0;i < 32;i++)
                if(abs(Dmax) < abs(D1[i]))
                {
                    Dmax = D1[i];
                    ind1 = (k<<5)+i;
                }
        }
    }

    *out = 0;

    temp = 1<<(16-nout);

    if(Dmax < 0)
        *out = temp;

    for(i = 1;i < nout;i++)
    {
        *out = *out << 1;
        *out |= (ind1 & 1) == 0 ? 0:temp;
        ind1 = ind1 >> 1;
    }

    return;

}

/******************************************************************************
 Decode_Reed_Muller_Conf (Reed-Muller decoder output confidence measure)
 ******************************************************************************

 ------------------------------------------------------------------------------
 Description:
 ------------

 For LTE the CQI messages longer than 11 bits have a CRC attached that can be
 used by the receiver to determine the accuracy of the decoded message.
 Unfortunately, for the shorter Reed-Muller encoded CQI messages there is no
 direct means of determining the validity of the decoded message.  LTE encodes
 the shorter CQI messages using the RM(20, X) (0 < X < 14) code for PUCCH and
 the RM(32, X) (0 < X < 12) for PUSCH.

 Function Decode_Reed_Muller_Conf computes confidence measures for these
 Reed-Muller soft decision decoder outputs. The following three confidence
 indicators are computed:

    1) HDQ (Hard decision quality)
    HRD is a single status bit that indicates if the Reed Muller decoder
    outputs are most likely correct or not (0= erroneous, 1=correct).  The
    assumption used for this indicator is that the UE is actually transmitting
    the proper Reed-Muller encoded CQI message with the possibility that
    the transmission is being corrupted by noise, fading or interference.

    2) HDQRD (Hard decision quality assuming possible random data transmission)
    HDQRD is also a single status bit that indicates if the Reed-Muller decoder
    outputs are most likely correct or not (0=erroneous, 1=correct).  However,
    the assumption used for this indicator is that the UE is possibly
    transmitting the proper Reed-Muller encoded CQI message or just random
    data.  This can occur if the UE somehow gets misconfigured.  In this case,
    the base station is expecting and decoding a CQI message but the UE never
    sends one.  This indicator guarantees that the decoder inputs are really
    Reed-Muller encoded and the decoder outputs are correct.  Note, the
    probability of false detection for this indicator is no worse than 1%.

    3) SDQ (Soft decision quality)
    SDQ is a 12-bit unsigned soft decision measure of the decoder's output
    quality.  The range is 0 to 4095 where 0 means the worst quality and
    4095 means the best quality.

 The function returns these threee confidence indicators mapped into a
 single 16-bit word as shown below.

    -----------------------------------------------------------
         MSB                                               LSB
    Bit: 15   14    13 12 11 10  9  8  7  6  5  4  3  2  1   0
         HDQ  HDQRD      |<-------------- SDQ --------------->|
    -----------------------------------------------------------

 ------------------------------------------------------------------------------
 Function prototype:
 -------------------

 The functional calling format for Decode_Reed_Muller_Conf is shown below.

    conf =  Decode_Reed_Muller_Conf(sftin, datin, nsft, ndat, enctab, conftab,
                                    ntab, mrderr)

    Where:  sftin = 8-bit 2's complement received soft decision array.

            datin = Decoder output data bytes.  Data bytes are packed into a
                    single 16-bit word (msb first to lsb last).

            nsft =  Number of received soft decisions (20 or 32).

            ndat =  Number of decoder output bits (Range is 1 to 13).

            enctab = Reed-Muller encoding table.

            conftab = Reed-Muller decoder confidence scale factor table.

            ntab =  Length of table conftab.

            mrderr = Maximum number of soft decision errors allowed to
                     exclude the possibility of random data transmission.

            conf = Returned confidence indicators.  The format for conf is:

                   Bit: 15  14    13 12 11 10 9  8  7  6  5  4  3  2  1  0
                        HDQ HDQRD      |<------------- SDQ -------------->|

 ------------------------------------------------------------------------------
 Algorithm description:
 ----------------------

 The confidence measures are determined by re-encoding the decoder output data
 and then checking the received soft decisions for errors (incorrect sign bits
 compared to the encoder outputs).  If there are too many errors then all of
 the confidence indicators are set to invalid.  Otherwise, the magnitudes of
 the correct and incorrect soft decisions are summed separately.  For HDQ and
 HDQRD the correct soft decision magnitude summation is scaled using table
 conftab[] and then compared to the incorrect magnitude summation.  If the
 comparison is positive, then the confidence is good.  Otherwise it is bad.
 For HDQRD the number of soft decision errors must also be no greater than
 the specified maximum (mrderr).  For SDQ the difference of the correct and
 incorrect soft decision magnitudes is computed and limited to ensure the
 proper 12-bit range.  The exact algorithm steps are summarized below.

        Re-encode decoder output data bytes (datin) using encoding table
        (enctab) and number of decoder output bits (ndat).

        Compare each of the (nsft) soft decisions to the encoder outputs.
        Compute the total number of soft decision sign bit errors (nerr).

        Compute the summation of all of the correct soft decison magnitudes
        (sumcor) and all of the incorrect soft decision magnitudes (sumerr).

        If the number of soft decision sign bit errors is too large (greater
        than ntab), then set all confidence indicators to invalid (=0).

        Otherwise, use the number of soft decision sign bit errors (nerr) to
        index the scale factor table (conftab) and multiply the correct soft
        decision magnitude summation (sumcor) by the selected table entry.

        Compare the scaled correct soft decision magnitude summation
        (sumcor*conftab[nerr]/32768) to the incorrect soft decision magnitude
        summation (sumerr).  If the result is positive then set indicators HDQ
        and HDQRD to good (=1) otherwise set them to bad (=0).

        If the number of soft decision errors are too large to guarantee that
        just random data is not present (greater than mrderr) then set
        indicator HDQRD to invalid (=0).

        Compute the difference of the correct and incorrect soft decision
        magnitude summations (sumcor - sumerr), limit the difference and use
        the result for indicator SDQ.

 ------------------------------------------------------------------------------
 BLER SNR penalty for using hard decision indicators HDQ or HDQRD:
 -----------------------------------------------------------------

 The typical performance measure of the (20, X) and (32, X) Reed Muller
 decoders is the determination of the decoder's block error rate (BLER) under
 various conditions (additive noise, fading, ...).  The potential error
 correcting capability of the decoder's depends on the specific code type
 as well as the actual decoding algorithm used.  Currently two decoder types
 are being used: 1) majority logic soft-decision decoder and 2) fast hadamard
 transform soft-decision decoder.  The hard decision indicators HDQ and HDQRD
 are designed to have false detection rates no greater than 1%.  To guarantee
 this reliability the number and relative magnitudes of the erroneous soft
 decisions has to be constrained.  This unfortunately results in a
 degradation of the decoder's BLER performance.  Simulations were run to
 determine the performance degradation for both indicators using all of the
 RM(20, X) and RM(32, X) codes and both decoder types.  The results are shown
 below:

        HDQ:    Worst-case BLER degradation = 4dB.
        HDQRD:  Worst-case BLER degradation = 7dB.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Reed-Muller decoder output confidence measurement.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param   sftin = 8-bit 2's complement received soft decision array.
 *  @param   datin = Decoder output data bytes.  Data bytes are packed into a
 *                   single 16-bit word (msb first to lsb last).
 *  @param   nsft =  Number of received soft decisions (20 or 32).
 *  @param   ndat =  Number of decoder output bits (Range is 1 to 13).
 *  @param   enctab = Reed-Muller encoding table.
 *  @param   conftab = Reed-Muller decoder confidence scale factor table.
 *  @param   ntab =  Length of table conftab.
 *  @param   mrderr = Maximum number of soft decision errors allowed to
 *                    exclude the possibility of random data transmission.
 *  @return  conf = Returned confidence indicators.  The format for conf is:
 *
 *                  Bit: 15  14    13 12 11 10 9  8  7  6  5  4  3  2  1  0
 *                       HDQ HDQRD      |<------------- SDQ -------------->|
 *
 *                  Where: HDQ = hard decision indicator (1=good, 0=bad)
 *                         HDQRD = hard decision indicator assuming possible
 *                                 random data (1=good, 0=bad).
 *                         SDQ = soft decision indicator (range: 0 - 4095}.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |sftin          |               |input        |nsft           |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |datin          |               |input        |1              |Real          |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nsft           |               |input        |1              |Real          |(1:15:0)  |{20, 32}       |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ndat           |               |input        |1              |Real          |(1:15:0)  |[1:13]         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |enctab         |               |input        |ndat           |pReal Array   |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |conftab        |               |input        |ntab           |pReal Array   |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ntab           |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |mrderr         |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |conf           |               |output       |1              |Real          |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------------
 *  |Block Name          |Decode_Reed_Muller_Conf                  |
 *  ----------------------------------------------------------------
 *  |Input Stream(s)     |sftin, datin                             |
 *  ----------------------------------------------------------------
 *  |Output Stream(s)    |                                         |
 *  ----------------------------------------------------------------
 *  |Config Parameter(s) |nsft, ndat, enctab, conftab, ntab, mrderr|
 *  ----------------------------------------------------------------
 *  |Status Parameter(s) |conf                                     |
 *  ----------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function Decode_Reed_Muller_Conf implements a function that computes
 * confidence measures for the (32, X) and (20, X) Reed Muller decoder
 * outputs.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
U16 Decode_Reed_Muller_Conf(S8 sftin[], U16 datin, S16 nsft, S16 ndat,
                            U32 enctab[], S16 conftab[],
                            S16 ntab, S16 mrderr)
{
    U16 dat;
    U32 encout;
    S16 i;
    S16 sumerr, sumcor, nerr;

    // Compute 16-bit data (data bytes are already packed in datin).
    dat = datin;

    // Initialize encoder output.
    encout = 0;

    // For each input data bit.
    for(i = 0; i < ndat; i++)
    {
        // If next data bit = 1 then xor corresponding encoding table
        // element into encoder output.
        if((dat & 0x8000) != 0) encout = encout ^ enctab[i];

        // Shift data left to next bit.
        dat = dat << 1;
    }

    // Initialize accumulated soft decision magnitudes for input errors.
    sumerr = 0;

    // Initialize accumulated soft decision magnitudes for correct inputs.
    sumcor = 0;

    // Initialize number of input errors.
    nerr = 0;

    // For each soft decision input.
    for(i = 0; i < nsft; i++)
    {

        // if soft decision = 0 or polarity is incorrect, then.
        if( ( (sftin[i] <= 0) && ((encout & 1) != 0) ) ||
            ( (sftin[i] >= 0) && ((encout & 1) == 0) ) )
        {
            // Add soft decision magnitude to error accumulation.
            sumerr = sumerr + abs(sftin[i]);

            // Increment number of input errors.
            nerr = nerr + 1;
        }
        // Else, input soft decision is alright.
        else
        {
            // Add soft decision magnitude to correct accumulation.
            sumcor = sumcor + abs(sftin[i]);

        }

        // Shift encoder output left to next bit.
        encout = encout >> 1;

    }

    // Initialize all inicator outputs to 0 (bad).
    dat = 0;

    // If not too many errors to use comparison table, then.
    if(nerr <= ntab)
    {

        // If accumulated soft decision errors are small enough, then
        // set indicators HDQ and HDQRD to 1 (good).
        if(sumerr < ((sumcor*conftab[nerr]) >> 15) ) dat = 0xC000;

        // If there are too many soft decision errors to guarantee
        // that the decoder inputs are not just random data, then
        // set indicator HDQRD to 0 (bad).
        if(nerr > mrderr) dat = dat & 0x8000;

        // Compute difference of correct and incorrect soft decision summations.
        i = sumcor - sumerr;

        // Limit difference.
        if(i < 0) i = 0;
        if(i > 4095) i = 4095;

        // Use limited difference for indicator SDQ.
        dat = dat | i;

    }

    //printf("Decode_Reed_Muller_Conf: nsft=%d, ndat=%d, ntab=%d, mrderr=%d\n", nsft, ndat, ntab, mrderr);
    //printf("Decode_Reed_Muller_Conf: nerr=%d, sumcor=%d, sumerr=%d, sft=%d, conf=%x\n", nerr, sumcor, sumerr, i, dat);
    //i = getchar();

    // Return confidence indication (0=incorrect, 1=correct).
    return (dat);

}

/******************************************************************************
 SFDECCOMB (SOFT_DECISION COMBINING FUNCTION)
 ******************************************************************************

 SFDECCOMB implements a function that allows multiple soft decisions to be
 combined.  Sometimes, at the transmitter the same bit is repeated multiple
 times.  Therefore, at the receiver the soft decisions for these bits must be
 combined in an equivalent manner.  Soft decisions for identical bits are
 combined by simply adding them together with limiting to prevent overflow.
 The functional calling format is defined below.

 SFDECCOMB(in, out, nin, nout)

 Where:  in =    8-bit 2's complement soft decicion input array.
 out =   8-bit 2's complement soft decision output array.
 nin =   Length of input array.
 nout =  Length of output array.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Soft decision combining function.
 *
 *  <1> Hardware Resource:
 *  ARM, CEVA or FEC
 *
 *  @param     in =     8-bit 2's complement soft decision input array.
 *  @param     out =    8-bit 2's complement soft decision output array.
 *  @param     nin =    Length of input array.
 *  @param     nout =   Length of output array.
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |SFDECCOMB                   |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |nin, nout                   |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * SFDECCOMB implements a function that allows multiple soft decisions to be
 * combined.  Sometimes, at the transmitter the same bit is repeated multiple
 * times.  Therefore, at the receiver the soft decisions for these bits must be
 * combined in an equivalent manner.  Soft decisions for identical bits are
 * combined by simply adding them together with limiting to prevent overflow.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void SFDECCOMB(S8 in[], S8 out[], S32 nin, S32 nout)
{
    S32 i, j, ac;

    //printf("SFDECCOMB nin=%d, nout=%d\n",nin, nout);

    // For each output array element
    for (i = 0; i < nout; i++)
    {
        // Initialize combined sum to 0
        ac = 0;

        // For each identical element in input array
        for (j = i; j < nin; j = j + nout)
        {
            // Add input array element to sum
            ac = ac + in[j];
        }

        //printf("SFDECCOMB i=%d, ac=%d\n",i, ac);

        // Limit sum to range (-128, +127)
        if (ac > 127)
            ac = 127;
        if (ac < -128)
            ac = -128;

        // Store limited sum in output array
        out[i] = (S8) ac;
    }

    return;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//Common FEC Utility Functions for both Downlink and Uplink
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 PACKBB (PACK BITS FROM BYTE ARRAY ELEMENTS INTO BYTE ARRAY ELEMENTS)
 ******************************************************************************

 PACKBB extracts single bits from byte input array elements and packs the bits
 into an output array of bytes.  The bit packing order is msb (bit7) first to
 lsb (bit0) last.  The calling arguments are shown below.

    IN      Input array pointer (bytes)
    OUT     Output array pointer (bytes)
    N       Number of bits to be extracted and packed
    POS     Relative position of bits within byte (7(msb) - 0(lsb))

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Pack bits from byte array elements into byte array elements.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     IN =     Input array pointer (16-bit words).
 *  @param     OUT =    Output array pointer (bytes).
 *  @param     N =      Number of bits to be extracted and packed.
 *  @param     POS =    Relative position of bits within byte (7(msb) - 0(lsb)).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |IN             |               |input        |N              |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |OUT            |               |output       |(N+7)/8        |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |N              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |POS            |               |input        |1              |Real          |(1:15:0)  |[0:7]          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------
 *  |Block Name          |PACKBB |
 *  ------------------------------
 *  |Input Stream(s)     |IN     |
 *  ------------------------------
 *  |Output Stream(s)    |OUT    |
 *  ------------------------------
 *  |Config Parameter(s) |N,POS  |
 *  ------------------------------
 *  |Status Parameter(s) |none   |
 *  ------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * PACKBB extracts single bits from byte input array elements and packs the
 * bits into an output array of bytes.  The bit packing order is msb (bit7) first
 * to lsb (bit0) last.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void PACKBB(U8 IN[], U8 OUT[], S32 N, S16 POS)
{

    S16 K, B, BB;
    S32 I, J;

    // Initialize output array index
    J = 0;
    // Initialize current output byte bit position
    K = 7;
    // Initialize current output byte
    BB = 0;

    //  Do for each bit
    for (I = 0; I < N; I++)
    {
        // Shift and extract bit from input array
        B = 1 & (IN[I] >> POS);
        // Shift bit into correct position and combine with others
        BB = BB | (B << K);
        // Decrement current output byte bit position
        K = K - 1;
        // If last output byte bit finished
        if (K < 0)
        {
            // Reset output byte bit position
            K = 7;
            // Write byte into output array
            OUT[J] = (U8) BB;
            // Increment output array index
            J = J + 1;
            // Clear current output byte
            BB = 0;
        }
    }

    // If not all bits written, then output last byte
    if (K != 7)
        OUT[J] = (U8) BB;

    return;
}

/******************************************************************************
 UNPACKBB (UNPACK BITS FROM BYTE ARRAY ELEMENTS INTO BYTE ARRAY ELEMENTS)
 ******************************************************************************

 UNPACKBB extracts single bits from byte input array elements and unpacks the
 bits into an output array of bytes.  The bit unpacking order is msb (bit7)
 first to lsb (bit0) last.  The calling arguments are shown below.

 IN     Input array pointer (bytes)
 OUT     Output array pointer (bytes)
 N      Number of bits to be extracted and unpacked
 POS     Relative location of output bit within byte (7(msb) - 0(lsb))

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Unpack bits from byte array elements into byte array elements.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     IN =  Input array pointer (bytes).
 *  @param     OUT = Output array pointer (bytes).
 *  @param     N =   Number of bits to be extracted and unpacked.
 *  @param     POS = Relative location of output bit within byte (7(msb) - 0(lsb)).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |IN             |               |input        |(N+7)/8        |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |OUT            |               |output       |N              |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |N              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |POS            |               |input        |1              |Real          |(1:15:0)  |[0:7]          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------
 *  |Block Name          |UNPACKBB |
 *  --------------------------------
 *  |Input Stream(s)     |IN       |
 *  --------------------------------
 *  |Output Stream(s)    |OUT      |
 *  --------------------------------
 *  |Config Parameter(s) |N,POS    |
 *  --------------------------------
 *  |Status Parameter(s) |none     |
 *  --------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * UNPACKBB extracts single bits from byte input array elements and unpacks the
 * bits into an output array of bytes.  The bit unpacking order is msb (bit7)
 * first to lsb (bit0) last.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void UNPACKBB(U8 IN[], U8 OUT[], S32 N, S16 POS)
{
    S16 K, B;
    S32 I, J;

    // Initialize input array index
    J = 0;
    // Initialize current input byte bit position
    K = 7;

    //  Do for each bit
    for (I = 0; I < N; I++)
    {
        // Shift and extract next bit from input array
        B = 1 & (IN[J] >> K);
        // Shift bit into correct position and output
        OUT[I] = (B << POS);
        // Decrement current input byte bit position
        K = K - 1;
        // If last input byte bit finished
        if (K < 0)
        {
            // Reset input byte bit position
            K = 7;
            // Increment input array index
            J = J + 1;
        }
    }
    return;
}

/******************************************************************************
 calc_gcrc8 (LTE gcrc8 CRC calculation)
 *******************************************************************************

 Function calc_gcrc8 calculates the 8-bit CRC gcrc8 as defined in the 3GPP (LTE)
 standard TS 36.212 Section 5.1.1.  The functional calling format is shown below.

 calc_gcrc8(in, out, n)

 Where: in =    Input bit stream organized as bytes.  Bits are packed in the
                bytes with the order msb=first, lsb=last.
        out =   8-bit CRC output.
        n =     Number of input bits to be processed.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE gcrc8 CRC calculation function.
 *
 *  <1> Hardware Resource:
 *  MDMA
 *
 *  @param     in =    Input bit stream organized as bytes.  Bits are packed in the
 *                     bytes with the order msb=first, lsb=last.
 *  @param     out =   8-bit CRC output.
 *  @param     n =     Number of input bits to be processed.
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |(n+7)/8        |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |1              |pReal         |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |calc_gcrc8                  |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n                           |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function calc_gcrc8 calculates the 8-bit CRC gcrc8 as defined in the 3GPP (LTE)
 * standard TS 36.212 Section 5.1.1.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.1.1.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void calc_gcrc8(U8 in[], U8 *out, S32 n)
{

    //*****************************************************************************
    //**********        GENERATOR POLYNOMIAL PARAMETER DEFINITION        **********
    //*****************************************************************************

    // The generator polynomial for the 8-bit gcrc8 is defined in the 3GPP standard
    // TS 36.212 Section 5.1.1 as is shown below.

    //      gcrc8(D) = D**8 + D**7 + D**4 + D**3 + D**1 + 1

    // For this implementation a 16-bit integer is used for the shift register as
    // shown below.

    //      reg bit:    15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //      D exponent:  8  7  6  5  4  3  2  1  0 -1 -2 -3 -4 -5 -6 -7
    //      Polynomial:  *  *        *  *     *  *

    // A representation of the generator polynomial (P_CRC8_POLY) is generated by
    // forming a constant that contains a 1 for each of the *s and zeros elsewhere.

#define     P_CRC8_POLY     0xCD80

    //*****************************************************************************

    U16 reg;
    S32 i, j, k;
    U8  mska, mskb;

    // k = number of input bytes
    k = (n + 7) >> 3;

    // Initialize input byte index
    j = 0;

    // In order to guarantee that the  crc generation algorithm generates the
    // correct results, all unused bits in the last input byte must be 0.  A
    // bit mask is computed and used to ensure this.
    i = n & 7;
    i = 8 - i;
    i = i & 7;
    mska = 0xff;
    mskb = mska << i;
    if (k == 1) mska = mskb;

    // Initialize shift register (b8 - b1) to 1st input byte
    reg = (in[0] & mska) << 8;

    // For each input bit
    for (i = 0; i < n; i++)
    {
        // If bit at a byte boundary, then
        if (i % 8 == 0)
        {
            // Increment input array index
            j = j + 1;
            // If more input bytes, then
            if (j < k)
            {
                // If last byte, then mask off unused bits.
                if(j == (k-1)) mska = mskb;

                // Load shift register (b0 - bn7) with next byte
                reg = reg | (mska & in[j]);
            }

        }
        // If bit D**8 = 1, then xor register with generator polynomial
        if ((reg & 0x8000) != 0)
        {
            reg = reg ^ P_CRC8_POLY;
        }
        // Move shift register to next bit
        reg = reg << 1;
    }

    // Write 8-bit CRC to output
    *out = (U8) ((reg >> 8) & 255);

    return;
}

/******************************************************************************
 3GPP (LTE) Standard Section 5.1.2 Code Block Segmentation and Code Block CRC
 Attachment
 *******************************************************************************

 Function lte_code_blk_seg implements the code block segmentation defined
 in the LTE standard.  The standard requires that any block having more bits
 than the allowed maximum (Z=6144 bits) be subdivided into sub-blocks.  Not all
 sub-block sizes are allowed.  The allowed sub-block sizes are defined in Table
 5.1.3-3 of the standard.  If the original block size cannot be exactly divided
 into sub-blocks having the allowed sizes, then filler bits are added at the
 beginning of the first sub-block to make up the size difference.

 The algorithm used to divide the original block into sub-blocks is clearly
 defined in the standard.  The original block is divided into sub-blocks that
 have one of two possible sizes (Kp and Km).  The algorithm computes the
 following:
 1) Number of sub-blocks (C)
 2) Number of sub-blocks of size Kp (Cp)
 3) Number of sub-blocks of size Km (Cm)
 4) Number of filler bits required (F)

 The functional calling arguments are defined below.

 B   Input block size in  bits.  It is important to note, that the maximum
 possible B value is 201,600 bits.  Therefore, a 32-bit variable must
 be used to represnt B.
 *C  Pointer to the computed number of total sub-blocks.
 *Cp Pointer to the computed number of Kp-bit sub-blocks.
 *Cm Pointer to the computed number of Km-bit sub-blocks.
 *Kp Pointer to computed sub-block size Kp.
 *Km Pointer to computed sub-block size Km.
 *F  Pointer to computed number of required filler bits F.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE channel coding code block segmentation function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *   @param      B =   Input block size in bits.  It is important to note, that the maximum
 *                     possible B value is 201,600 bits.  Therefore, a 32-bit variable must
 *                     be used to represent B.
 *   @param      *C =  Pointer to the computed number of total sub-blocks.
 *   @param      *Cp = Pointer to the computed number of Kp-bit sub-blocks.
 *   @param      *Cm = Pointer to the computed number of Km-bit sub-blocks.
 *   @param      *Kp = Pointer to computed sub-block size Kp.
 *   @param      *Km = Pointer to computed sub-block size Km.
 *   @param      *F =  Pointer to computed number of required filler bits F.
 *   @return     None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |B              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |C              |               |output       |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Cp             |               |output       |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Cm             |               |output       |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Kp             |               |output       |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Km             |               |output       |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |F              |               |output       |1              |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |lte_code_blk_seg            |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |none                        |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |none                        |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |B                           |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |C, Cp, Cm, Kp, Km, F        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function lte_code_blk_seg() implements the code block segmentation defined
 * in the LTE standard.  The standard requires that any block having more bits
 * than the allowed maximum (Z=6144 bits) be subdivided into sub-blocks.  Not all
 * sub-block sizes are allowed.  The allowed sub-block sizes are defined in Table
 * 5.1.3-3 of the standard.  If the original block size cannot be exactly divided
 * into sub-blocks having the allowed sizes, then filler bits are added at the
 * beginning of the first sub-block to make up the size difference.  <BR>
 *
 * The algorithm used to divide the original block into sub-blocks is clearly
 * defined in the standard.  The original block is divided into sub-blocks that
 * have one of two possible sizes (Kp and Km).  The algorithm computes the
 * following:
 *  -# Number of sub-blocks (C)
 *  -# Number of sub-blocks of size Kp (Cp)
 *  -# Number of sub-blocks of size Km (Cm)
 *  -# Number of filler bits required (F)
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0 (2009-03) Section 5.1.2
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------

void lte_code_blk_seg(S32 B, S16 *C, S16 *Cp, S16 *Cm, S16 *Kp, S16 *Km, S16 *F, S16* blkSizeTab)
{

    //*****************************************************************************
    //**********             CONSTANT PARAMETER DEFINITIONS              **********
    //*****************************************************************************

    // Define maximum block size in bits
#define Z   6144

    //*****************************************************************************

    S32 Bp;
    S16 L, Kd, ind;

    // Initialize number of filler bits
    *F = 0;

#if 0

    // If input block size is too small ( < 40 bits)
    if(B < 40)
    {
        // Compute required number of filler bits
        *F = (S16) (40 - B);

        // Set input block size to minimum (40 bits)
        B = 40;
    }

#endif

    // If input block size is not too big
    if (B <= Z)
    {
        // Set number of CRC bits = 0, since CRC is not added for 1 sub-block
        L = 0;

        // Set number of sub-blocks = 1
        *C = 1;

        // Set total required block size = input block size
        Bp = B;
    }
    // Else input block size is too large
    else
    {
        // Set number of extra CRC bits = 24, since a CRC is required for each
        // sub-block
        L = 24;

        // Compute number of sub-blocks
        *C = (S16) (B / (Z - L));
        if (B - ((*C) * (Z - L)) != 0)
            *C = *C + 1;

        // Set total required block size to input block size + extra CRC bits
        Bp = B + ((*C) * L);
    }

    // Determine table index corresponding to smallest allowed sub-block size
    // that would be required
    for (ind = 0; ind < P_BLKSIZ_TAB_LEN; ind++)
    {
        if ((*C) * blkSizeTab[ind] >= Bp)
            goto laba;
    }

    laba:
    // Set Kp = allowable sub-block size in bits
    *Kp = blkSizeTab[ind];

    // If only 1 sub-block is required
    if (*C == 1)
    {
        // Set number of required Kp sub-blocks = 1
        *Cp = 1;

        // Set number of required Km sub-blocks = 0
        *Cm = 0;

        // Set 2nd allowed sub-block size = 0
        *Km = 0;
    }
    // Else, more than 1 sub-block is required
    else
    {
        // Set 2nd sub-block size to maximum allowed size < Kp
        *Km = blkSizeTab[ind - 1];

        // Compute difference between sub-block sizes
        Kd = *Kp - *Km;

        // Compute number of Km size sub-blocks
        *Cm = (S16) ((((*C) * (*Kp)) - Bp) / Kd);

        // Compute number of Kp size sub-blocks
        *Cp = *C - *Cm;
    }

    // Compute number of 1st sub-block filler bits
    *F = (S16) (((*Cp) * (*Kp)) + ((*Cm) * (*Km)) - Bp);

    return;

}
//-------------------------------------------------------------------------------------------
/** @brief LTE compute rate matching parameter E function.<BR>
 * Function Compute_Rate_Matching_E computes the LTE rate matching parameter E.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *   @param     C =     Number of code blocks.
 *   @param     NL =    1 (for transport blocks mapped to 1 layer) or 2 (for transport
 *                      blocks mapped to 2 or 4 layers).
 *   @param     G =     Total number of bits available for the transmission of one
 *                      transport block.
 *   @param     Qm =    2 for QPSK, 4 for 16QAM, or 6 for 64QAM.
 *   @param     r =     Code block index (0 - C-1).
 *   @return    E.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |C              |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NL             |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |G              |               |input        |1              |Real          |(1:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(1:15:0)  |{2, 4, 6}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |r              |               |input        |1              |Real          |(1:15:0)  |[0: C-1]       |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |Compute_Rate_Matching_E          |
 *  --------------------------------------------------------
 *  |Input Stream(s)     |None                             |
 *  --------------------------------------------------------
 *  |Output Stream(s)    |None                             |
 *  --------------------------------------------------------
 *  |Config Parameter(s) |C,NL,G,Qm,r                      |
 *  --------------------------------------------------------
 *  |Status Parameter(s) |None                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Function Compute_Rate_Matching_E computes the LTE rate matching parameter E.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] LTE 3GPP 36.212
 *
 *
 * \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S32 Compute_Rate_Matching_E(S16 C, S16 NL, S32 G, S16 Qm, S16 r)
{

    S16 g;
    S32 Gd, E;

    // Compute number of symbols
    Gd = G / (NL * Qm);

    // Compute remainder
    g = Gd % C;

    // If current code block number is small enough
    if (r <= (C - g - 1))
    {
        // Compute number of total outputs
        E = divide_floor(Gd, C);
    }
    else
    {
        // Compute larger number of total outputs
        E = divide_ceil(Gd, C);
    }

    E = E*NL*Qm;

    return (E);

}

///////////////////////////////////////////////////////////////////////////////
//General FEC Utility Functions
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 div_quo_rem (Divide function with remainder)
 ******************************************************************************

 Function div_quo_rem computes the integer quotient of two 16-bit positive
 integers and the remainder as defined below.

 q = x/y
 r = x%y

 Where:  x = 16-bit positive dividend
 y = 16-bit positive divisor
 q = 16-bit quotient (integer part)
 r = 16-bit remainder

 The functional calling format is defined below.

 div_quo_rem(x, y, yscf, &q, &r)

 Where:  x = 16-bit positive dividend
 y = 16-bit positive divisor
 yscf = 16-bit equivalent fractional scale factor = 1/y = 32768/y
 (rounded up if necessary).
 q = address of quotient (integer part)
 r = address of remainder

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Integer divide function with remainder.<BR>
 *  Function div_quo_rem computes the integer quotient of two 16-bit positive
 *  integers and the remainder.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *   @param     x =     16-bit positive dividend.
 *   @param     y =     16-bit positive divisor.
 *   @param     yscf =  16-bit equivalent fractional scale factor = 1/y = 32768/y
 *                      (rounded up if necessary).
 *   @param     *q =    Pointer to quotient (integer part).
 *   @param     *r =    Pointer to remainder.
 *   @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |x              |               |input        |1              |Real          |(1:31:0)  |[0:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |y              |               |input        |1              |Real          |(1:31:0)  |[0:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |yscf           |               |input        |1              |Real          |(1:31:0)  |[0:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |*q             |               |output       |1              |pReal         |(1:31:0)  |[0:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |*r             |               |output       |1              |pReal         |(1:31:0)  |[0:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |div_quo_rem                      |
 *  --------------------------------------------------------
 *  |Input Stream(s)     |x, y                             |
 *  --------------------------------------------------------
 *  |Output Stream(s)    |*q, *r                           |
 *  --------------------------------------------------------
 *  |Config Parameter(s) |yscf                             |
 *  --------------------------------------------------------
 *  |Status Parameter(s) |None                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  Function div_quo_rem computes the integer quotient of two 16-bit positive
 *  integers and the remainder as defined below.
 *
 *   -# q = x/y
 *   -# r = x%y
 *
 *   -# Where:  x = 16-bit positive dividend
 *   -#         y = 16-bit positive divisor
 *   -#         q = 16-bit quotient (integer part)
 *   -#         r = 16-bit remainder
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void div_quo_rem(S32 x, S32 y, S32 yscf, S32 *q, S32 *r)
{

    S32 ac;
    S32 z;

    // Compute z = (x/y integer part)
    ac = x * yscf;
    z = ac >> 15;

    // Compute ac = remainder of x/y
    ac = x;
    ac = ac - (z * y);

    // If (ac < 0) then yscf rounding caused error
    if (ac < 0)
    {
        // Decrement quotient to remove error
        z = z - 1;

        // Add divisor to remainder to remove error
        ac = ac + y;
    }

    // Return quotient and remainder
    *q = z;
    *r = ac;

    return;
}
//-------------------------------------------------------------------------------------------
/** @brief LTE compute rate matching parameters function.<BR>
 * Function Compute_Rate_Matching_Parameters computes a structure that contains the various
 * LTE rate matching parameters.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *   @param     nblk =  Code block size in bits.
 *   @param     C =     Number of code blocks.
 *   @param     NL =    1 (for transport blocks mapped to 1 layer) or 2 (for transport
 *                      blocks mapped to 2 or 4 layers).
 *   @param     G =     Total number of bits available for the transmission of one
 *                      transport block.
 *   @param     Qm =    2 for QPSK, 4 for 16QAM, or 6 for 64QAM.
 *   @param     r =     Code block index (0 - C-1).
 *   @param     NIR =   Soft buffer size for the transport block.
 *                      Set = 0 for uplink receiver.
 *   @param     *pRm    Rate matching parameter structure pointer.
 *   @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nblk           |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |C              |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NL             |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |G              |               |input        |1              |Real          |(1:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(1:15:0)  |{2, 4, 6}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |r              |               |input        |1              |Real          |(1:15:0)  |[0: C-1]       |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NIR            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |*pRm           |               |output       |1              |pStructure    |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |Compute_Rate_Matching_Parameters |
 *  --------------------------------------------------------
 *  |Input Stream(s)     |None                             |
 *  --------------------------------------------------------
 *  |Output Stream(s)    |*pRm                             |
 *  --------------------------------------------------------
 *  |Config Parameter(s) |nblk,C,NL,G,Qm,r,NIR             |
 *  --------------------------------------------------------
 *  |Status Parameter(s) |None                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Function Compute_Rate_Matching_Parameters computes a structure that contains the various
 * LTE rate matching parameters.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] LTE 3GPP 36.212
 *
 *
 * \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void Compute_Rate_Matching_Parameters(S16 nblk, S16 C, S16 NL, S32 G, S16 Qm, S16 r, S32 NIR,
                                      RMParameters *pRm)
{

    // define sub-block interleaver number of columns
//#define     p_ncol      32
#define     sh_ncol      5

    S16 D, nrow, j, nintl, tmp;
    S16 Ncb, g;
    S32 Gd, E, E2, Ltmp, Ltmp1;

    // D = length of each rate matching input stream
    D = nblk + 4;

    // Compute number of rows in interleaver buffer

//    nrow = divide_ceil(D, p_ncol);
    // Compute total size of interleaver buffer
//    nintl = nrow * p_ncol;

    nrow = D >> sh_ncol;
    tmp = nrow << sh_ncol;
    if(D > tmp)
        nrow += 1;
    nintl = nrow << sh_ncol;

    // Compute length of circular buffer
    Ncb = 3 * nintl;

    // If downlink, then limit circular buffer size
    if (NIR > 0)
    {
        Ltmp = NIR / C;
        if (Ltmp < Ncb)
            Ncb = (S16) Ltmp;
    }

    // Compute number of symbols
    Gd = G / (NL * Qm);
    // If current code block number is small enough
    E = Gd / C;

    // Compute remainder
    Ltmp = C * E;
    g = (S16)(Gd - Ltmp);

	E2 = E;
    if(g > 0)
        E2 += 1;

    // Compute alternate E for r > (C - g - 1)
	E2 = E2 * NL * Qm;

    if (r <= (C - g - 1))
    {
        // Compute number of total outputs
//        E = divide_floor(Gd, C);
        E = E * NL * Qm;
    }
    else
    {
        // Compute larger number of total outputs
//        E = divide_ceil(Gd, C);
        E = E2;
    }


    // Compute circular buffer starting points for rvidx=0, 1, 2, 3
//    j = divide_ceil(Ncb, (8*nrow));
    tmp = nrow << 3;
    j = Ncb / tmp;
    if(Ncb > tmp * j)
        j += 1;

    // Write rate matching parameters into structure
    pRm->E = E;
    Ltmp = nrow * 2;
    Ltmp1 = Ltmp * j;
    pRm->k0[0] = (S16)Ltmp; // rvidx = 0
    pRm->k0[1] = (S16)(Ltmp + Ltmp1); // rvidx = 1
    pRm->k0[2] = (S16)(Ltmp + 2*Ltmp1); // rvidx = 2
    pRm->k0[3] = (S16)(Ltmp + 3*Ltmp1); // rvidx = 3
    pRm->Ncb = Ncb;
	pRm->NumCodeBlockUseE = ( C - g - 1);   // Code block limit for using E
	pRm->E2 = E2;   // Alternate E when code block exceeds limit

    return;

}

//*****************************************************************************
//**********            Function: LTE_Chan_Deint_Gen Tables          **********
//*****************************************************************************

// Table tab_ri_col[i][j] defines the relative columns where the RI symbols
// are located.  The indices are defined below.

//      i = cyclic prefix (CP).  CP = 0 for normal.  CP = 1 for extended.
//      j = number of columns allowed (4).

S16 tab_ri_col[2][4] =
{ {1, 10, 7, 4}, // CP = 0
  {0, 8, 5, 3} // CP = 1
    };

//*****************************************************************************

// Table tab_col_inv[4] defines 1/ncol for the four allowable de-interleaver
// buffer number of columns (9, 10, 11, 12);

S16 tab_col_inv[4] =
{ 3641, // ncol = 9.  32768/9 rounded up
  3277, // ncol = 10.  32768/10 rounded up
  2979, // ncol = 11.  32768/11 rounded up
  2731 }; // ncol = 12.  32768/12 rounded up

//*****************************************************************************

// Table tab_qm_inv[3] defines 1/Qm for the three allowable modulation types
// (Qm = 2, 4, or 6).

S16 tab_qm_inv[4] =
{ 16384, // Qm = 2.  32768/1 rounded up
  8192, // Qm = 4.  32768/4 rounded up
  5462 }; // Qm = 6.  32768/6 rounded up

//*****************************************************************************

// The current FEC hardware de-interleaver when configured for 12 columns
// has a maximum allowed column length of 1536 bytes.  Table tab_max_row[3]
// defines the maximum number of symbol rows per column for Qm=2
// (2-bit symbols), Qm=4 (4-bit symbols and Qm=6 (6-bit symbols).

S16 tab_max_row[3] =
{ 768, // Qm = 2.  Length = 1536/2
  384, // Qm = 4.  Length = 1536/4
  256 }; // Qm = 6.  Length = 1536/6

//*****************************************************************************

/******************************************************************************
 LTE_Chan_Deint_Gen (FEC hardware channel de-interleaver descriptor generator)
 ******************************************************************************

 Function LTE_Chan_Deint_Gen is used to generate the descriptors required to
 control the hardware channel de-interleaver.  For the uplink shared channel
 the multiplexed control and data received soft-decisions must be
 de-interleaved.

 The current de-interleaver hardware is part of the FEC decoder and can only be
 used to de-interleave the CTC encoded soft-decisions before they are
 decoded.  Therefore, the de-interleaver is run every code block just like the
 decoder is.  Even though the actual de-interleaver function can have 9, 10, 11
 or 12 columns (depending on the system configuration), the hardware
 de-interleaver is always configured for 12 columns.  Also, the hardware
 de-interleaver has a maximum of 1536 rows.

 To implement the channel de-interleaver function, the hardware loads the
 soft-decisions into the de-interleaver column by column and then reads them
 out row by row.  The loading of the de-interleaver is controlled by a set
 of descriptors generated by the ARM using this function.  A descriptor
 consists of one starting address and one byte count.  The starting address
 indicates where to start reading the soft-decisions and the byte count
 indicates how many soft-decisions to read.  There is one
 descriptor for each of the 12 columns in the de-interleaver.  For the 9, 10
 and 11 column cases, the unused columns can be skipped by setting the
 corresponding descriptor byte counts to 0.  For those cases requiring more
 than 1536 rows, multiple sets of 12 descriptors can be used.

 For the channel-deinterleaver function the soft-decisions are organized
 as symbols.  The symbol size, specified by parameter Qm, depends on the type
 of modulation used (QPSK: Qm=2, 16-QAM: Qm=4, 64-QAM; Qm=6).

 The functional calling format is defined below.

 LTE_Chan_Deint_Gen(S32 data_start, S32 data_end,
 S16 CP, S16 Qm, S32 ncol,
 S32 ndat, S32 ncqi, S32 nri,
 S8  *inp, S8 *outadr[], S32 outlen[], S32 *nout)

 Where:  data_start =    Relative (non-interleaved) location of code block's
 first data soft-decision.
 data_end =      Relative (non-interleaved) location of code block's
 last data soft decision.
 CP =            Cyclic prefix type (0=normal, 1=extended).
 Qm =            Modulation vector size (2=QPSK, 4=16-QAM, 6=64-QAM).
 ncol =          Number of de-interleaver buffer columns (9, 10, 11, 12).
 ndat =          Number of data soft-decisions in de-interleaver buffer.
 ncqi =          Number of CQI soft-decisions in de-interleaver buffer.
 nri =           Number of RI soft-decisions in de-interleaver buffer.
 *inp =          Soft-decision input array pointer.
 *outadr[] =     Descriptor starting address output array.
 outlen[] =      Descriptor byte count output array.
 *nout =         Number of descriptors generated.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief FEC hardware channel de-interleaver descriptor generation function.<BR>
 *  Function LTE_Chan_Deint_Gen is used to generate the descriptors required to
 *  control the hardware channel de-interleaver.  For the uplink shared channel
 *  the multiplexed control and data received soft-decisions must be
 *  de-interleaved.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param  data_start =    Relative (non-interleaved) location of code block's
 *                          first data soft-decision.
 *  @param  data_end =      Relative (non-interleaved) location of code block's
 *                          last data soft decision.
 *  @param  CP =            Cyclic prefix type (0=normal, 1=extended).
 *  @param  Qm =            Modulation vector size (2=QPSK, 4=16-QAM, 6=64-QAM).
 *  @param  ncol =          Number of de-interleaver buffer columns (9, 10, 11, 12).
 *  @param  ndat =          Number of data soft-decisions in de-interleaver buffer.
 *  @param  ncqi =          Number of CQI soft-decisions in de-interleaver buffer.
 *  @param  nri =           Number of RI soft-decisions in de-interleaver buffer.
 *  @param  *inp =          Soft-decision input array pointer.
 *  @param  *outadr[] =     Descriptor starting address output array.
 *  @param  outlen[] =      Descriptor byte count output array.
 *  @param  *nout =         Number of descriptors generated.
 *  @return None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |data_start     |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |data_end       |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CP             |               |input        |1              |Real          |(1:15:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(1:15:0)  |{2, 4, 6}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ncol           |               |input        |1              |Real          |(1:31:0)  |{9, 10, 11, 12}|              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ndat           |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ncqi           |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nri            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |*inp           |               |input        |1              |pReal Array   |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |*outadr[]      |               |output       |1              |pReal Array   |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |outlen[]       |               |output       |1              |pReal Array   |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |*nout          |               |output       |1              |pReal         |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------------------------
 *  |Block Name          |LTE_Chan_Deint_Gen                                |
 *  -------------------------------------------------------------------------
 *  |Input Stream(s)     |None                                              |
 *  -------------------------------------------------------------------------
 *  |Output Stream(s)    |*outadr[], outlen[], *nout                        |
 *  -------------------------------------------------------------------------
 *  |Config Parameter(s) |data_start,data_end,CP,Qm,ncol,ndat,ncqi,nri,*inp |
 *  -------------------------------------------------------------------------
 *  |Status Parameter(s) |None                                              |
 *  -------------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------

void LTE_Chan_Deint_Gen(S32 data_start, S32 data_end, S16 CP, S16 Qm, S32 ncol, S32 ndat, S32 ncqi,
                        S32 nri, S8 *inp, S8 *outadr[], S32 outlen[], S32 *nout)
{
    S32 hdwr_lim, scfqm, scfcol;
    S32 nsym, nrow, nsymri, sym_start, sym_end;
    S32 nrirow[4];
    S32 i, j, ac, ac1, ac2, q, r;
    S32 start, end, s, n;

    //printf("************************************************************\n");
    //printf("***** Running LTE_Chan_Deint_Gen *****\n");
    //printf(" data_start=%d, data_end=%d\n", data_start, data_end);
    //printf(" CP=%d, Qm=%d, ncol=%d\n", CP, Qm, ncol);
    //printf(" ndat=%d, ncqi=%d, nri=%d\n", ndat, ncqi, nri);

    // Define hardware de-interleaver maximum size in symbols.
    hdwr_lim = ncol * tab_max_row[(Qm >> 1) - 1];

    // Compute 1/Qm using table.
    scfqm = tab_qm_inv[(Qm >> 1) - 1];

    // Compute 1/ncol using table.
    scfcol = tab_col_inv[ncol - 9];

    //printf("hdwr_lim=%d, scfqm=%d, scfcol=%d\n", hdwr_lim, scfqm, scfcol);

    // Compute total number of symbols in de-interleaver buffer.
    ac = ndat + ncqi + nri;
    nsym = (ac * scfqm) >> 15;
    if ((nsym * Qm) > ac)
        nsym = nsym - 1;
    if ((nsym * Qm) > ac)
        nsym = nsym - 1;

    //Compute number of symbol rows in de-interleaver buffer.
    nrow = (nsym * scfcol) >> 15;
    if ((nrow * ncol) > nsym)
        nrow = nrow - 1;

    // Compute 1st data symbol location.  Compensate for any CQI symbols present.
    ac = data_start + ncqi;
    sym_start = (ac * scfqm) >> 15;
    if ((sym_start * Qm) > ac)
        sym_start = sym_start - 1;
    if ((sym_start * Qm) > ac)
        sym_start = sym_start - 1;

    // Compute last data symbol location.  Compensate for any CQI symbols present.
    ac = data_end + ncqi;
    sym_end = (ac * scfqm) >> 15;
    if ((sym_end * Qm) > ac)
        sym_end = sym_end - 1;
    if ((sym_end * Qm) > ac)
        sym_end = sym_end - 1;

    //printf("nsym=%d, nrow=%d\n", nsym, nrow);
    //printf("Before prior RI symbol compensation: sym_start=%d, sym_end=%d\n", sym_start, sym_end);

    // Compute number of RI symbols.
    nsymri = (nri * scfqm) >> 15;
    if ((nsymri * Qm) > nri)
        nsymri = nsymri - 1;
    if ((nsymri * Qm) > nri)
        nsymri = nsymri - 1;

    // Compute number of rows used for RI symbols in each of the four possible columns.
    nrirow[0] = (nsymri + 3) >> 2; // 1st column used
    nrirow[1] = (nsymri + 2) >> 2; // 2nd column used
    nrirow[2] = (nsymri + 1) >> 2; // 3rd column used
    nrirow[3] = (nsymri) >> 2; // 4th column used

    //printf("nsymri=%d, nrirow[0]=%d, nrirow[1]=%d, nrirow[2]=%d, nrirow[3]=%d\n", nsymri,
    //       nrirow[0], nrirow[1], nrirow[2],nrirow[3]);

    // Compute actual 1st data symbol position that accounts for prior RIs.

    // Initialize starting position for data symbol
    s = sym_start;

    labstra:

    // Compute q = row = s/ncol and r = column = s%ncol for 1st data symbol.
    div_quo_rem(s, ncol, scfcol, &q, &r);

    // Compute number of RI symbols in de-interleaver buffer before data symbol.

    // ac = total number of prior RI symbols, initialized to 0.
    ac = 0;

    // For each possible column containing RI symbols.
    for (i = 0; i < 4; i++)
    {
        // ac1 = number of RI symbols in column in prior rows.
        ac1 = nrirow[i] - (nrow - q);

        //  If RI column is prior to or at data symbol, add this row too.
        if (tab_ri_col[CP][i] <= r)
            ac1 = ac1 + 1;

        // Limit number of prior RI symbols in column to 0.
        if (ac1 < 0)
            ac1 = 0;

        // Add RI column value to total.
        ac = ac + ac1;
    }

    //printf("Computing 1st data symbol position: s=%d, row=%d, col=%d, prior RIs=%d\n", s, q, r, ac);

    // If new data symbol position and prior number of RIs are incorrect.
    if ((s - ac) != sym_start)
    {
        // Update new data symbol position and re-iterate
        s = sym_start + ac;
        goto labstra;
    }

    // Update 1st data symbol position to account for prior RIs.
    sym_start = s;

    //printf("After prior RI symbol compensation: sym_start=%d, prior RIs=%d\n", sym_start, ac);

    // Compute actual last data symbol position that accounts for prior RIs.

    // Initialize starting position for data symbol
    s = sym_end;

    labenda:

    // Compute q = row = s/ncol and r = column = s%ncol for 1st data symbol.
    div_quo_rem(s, ncol, scfcol, &q, &r);

    // Compute number of RI symbols in de-interleaver buffer before data symbol.

    // ac = total number of prior RI symbols, initialized to 0.
    ac = 0;

    // For each possible column containing RI symbols.
    for (i = 0; i < 4; i++)
    {
        // ac1 = number of RI symbols in column in prior rows.
        ac1 = nrirow[i] - (nrow - q);

        //  If RI column is prior to or at data symbol, add this row too.
        if (tab_ri_col[CP][i] <= r)
            ac1 = ac1 + 1;

        // Limit number of prior RI symbols in column to 0.
        if (ac1 < 0)
            ac1 = 0;

        // Add RI column value to total.
        ac = ac + ac1;
    }

    //printf("Computing last data symbol position: s=%d, row=%d, col=%d, prior RIs=%d\n", s, q, r, ac);

    // If new data symbol position and prior number of RIs are incorrect.
    if ((s - ac) != sym_end)
    {
        // Update new data symbol position and re-iterate
        s = sym_end + ac;
        goto labenda;
    }

    // Update last data symbol position to account for prior RIs.
    sym_end = s;

    //printf("After prior RI symbol compensation: sym_end=%d, prior RIs=%d\n", sym_end, ac);

    // Initialize symbol range start.
    start = sym_start;

    // Initialize number of outputs.
    n = 0;

    loopsyms:

    // Initialize symbol range end.
    end = sym_end;

    // If specified symbol range is larger than hardware can accommodate,
    // then limit symbol range end.
    if ((end - start + 1) > hdwr_lim)
        end = start + hdwr_lim - 1;

    // Initialize column start symbol.
    s = start;

    //printf("Descriptor generation loop: s=%d, end=%d\n", s, end);

    // For each de-interleaver buffer column.
    for (i = 0; i < ncol; i++)
    {

        // Compute number of data symbols in this column.
        ac = (end - s);
        ac1 = ac * scfcol;
        ac1 = ac1 >> 15;
        if ((ac1 * ncol) > ac)
            ac1 = ac1 - 1;
        ac1 = ac1 + 1;

        // Compute q = row = s/ncol, r = column = s%ncol.
        div_quo_rem(s, ncol, scfcol, &q, &r);

        // Compute location of start symbol within input array.
        // location = Qm*(s/ncol + nrow(s%ncol)).
        ac = Qm * (q + (nrow * r));

        //printf("col=%d, s=%d, srow=%d, scol=%d, nsyms=%d\n", i, s, q, r, ac1);

        // For each of the possible columns containing RI symbols.
        for (j = 0; j < 4; j++)
        {
            // If this is a column that contain RI symbols.
            if (r == tab_ri_col[CP][j])
            {
                // Compute number of rows until RI symbol.
                ac2 = nrow - nrirow[j] - q;
                if (ac2 < 0)
                    ac2 = 0;

                // Limit number of data symbols in column until first RI symbol.
                if (ac2 < ac1)
                    ac1 = ac2;

                //printf("RI column limit = %d, new nsyms=%d\n", ac2, ac1);

                // Exit.
                goto labexa;
            }

        }

        labexa:

        // Store pointer to input array element in output array.
        outadr[n] = inp + ac;

        // Store number of bytes in column in output array.
        outlen[n] = ac1 * Qm;

        //printf("outadr[%d]=%d, outlen[%d]=%d\n", n, outadr[n]-inp, n, outlen[n]);

        // Increment column's start symbol.
        s = s + 1;

        // Increment number of output array elements.
        n = n + 1;

    }

    // For any unused de-interleaver buffer columns
    for (i = ncol; i < P_DEINT_COL; i++)
    {

        // Store pointer to input array element in output array.
        outadr[n] = inp;

        // Store number of bytes in column in output array.
        outlen[n] = 0;

        //printf("outadr[%d]=%d, outlen[%d]=%d\n", n, outadr[n]-inp, n, outlen[n]);

        // Increment number of output array elements.
        n = n + 1;
    }

    // Advance symbol range start.
    start = end + 1;

    // If still more symbols in range, then continue.
    if (start <= sym_end)
        goto loopsyms;

    // Store number of outputs
    *nout = n;

    //printf("nout=%d\n", *nout);
    //printf("************************************************************\n");
    //i = getchar();

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief Compute number of ACK, RI and CQI control symbols for PUSCH.<BR>

 *  Function Num_Cont_Syms_PUSCH computes the number of coded symbols for the ACK,
 *  RI and CQI control fields used for the PUSCH.  The formulas for computing these
 *  values are defined in document 3GPP 36.212 Sections 5.2.2.6 and 5.2.4.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param  nsym       = Current number of SC-FDMA symbols per subframe excluding
 *                       those used for pilots or SRS(9 - 12).
 *  @param  nc         = Channel Number for user.
 *  @param  pFecParams = FEC Structure containing information on all users in current subframe
 *
 *  @return None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range      |Units         |
 *  |               |Standard       |             |               |              |          |                 |              |
 *  --------------------------------------------------------------------------------------------------------------------------
 *  |nsym           |               |input        |1              |Real          |(1:15:0)  |[9:12]           |              |
 *  --------------------------------------------------------------------------------------------------------------------------
 *  |nc             |               |input        |1              |Real          |(1:15:0)  |[0:MAXCHSUBFRAME]|              |
 *  --------------------------------------------------------------------------------------------------------------------------
 *  |pFecParams     |               |input/output |1              |pStructure    |          |                 |              |
 *  --------------------------------------------------------------------------------------------------------------------------
 *
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------------------------------
 *  |Block Name          |Num_Cont_Syms_PUSCH                                          |
 *  ------------------------------------------------------------------------------------
 *  |Input Stream(s)     |None                                                         |
 *  ------------------------------------------------------------------------------------
 *  |Output Stream(s)    |*noutack, *noutri, *noutcqi                                  |
 *  ------------------------------------------------------------------------------------
 *  |Config Parameter(s) |pFecParams, nc, nsym                                         |
 *  ------------------------------------------------------------------------------------
 *  |Status Parameter(s) |None                                                         |
 *  ------------------------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 sections 5.2.2.6 and 5.2.4.<BR>
 * [2] 3GPP (LTE) standard document TS 36.213 section 8.6.3.<BR>
 *
 * \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void Num_Cont_Syms_PUSCH(PULRXFECPARAMS pFecParams, S16 nc, S16 nsym, S16* blkSizeTab)
{
    S16 ninack, ninri, nincqi, iack, iri, icqi,
        nsymi, nrbi, nrb, Qm, C, Cp, Cm, Kp, Km, F;
    S32 *noutack, *noutri, *noutcqi;
    S32 SumKr, nindat, ac0, ac1, ac2, ac3;

    //printf("!!!!!!!!!! Running Num_Cont_Syms_PUSCH !!!!!!!!!!\n");
    //printf("ninack=%d, ninri=%d, nincqi=%d, nindat=%d\n",ninack,ninri,nincqi,nindat);
    //printf("iack=%d, iri=%d, icqi=%d\n",iack,iri,icqi);
    //printf("nsymi=%d, nsym=%d, nrbi=%d, nrb=%d\n",nsymi,nsym,nrbi,nrb);
    ninack = (S16) pFecParams->nACK[nc];
    ninri = (S16) pFecParams->nRI[nc];
    nincqi = (S16) (pFecParams->nr1CQI[nc] + pFecParams->nrg1CQI[nc]);
    nindat = pFecParams->numBytes[nc] * 8;
    iack = pFecParams->betaOffsetACKIndex[nc];
    iri = pFecParams->betaOffsetRIIndex[nc];
    icqi = pFecParams->betaOffsetCQIIndex[nc];
    nsymi = pFecParams->nsymi[nc];
    nrbi = pFecParams->nrbi[nc];
    nrb = pFecParams->Mpusch_rb[nc];
    Qm = pFecParams->Qm[nc];
    noutack = (S32 *)&pFecParams->numbitsACK[nc];
    noutri = (S32 *)&pFecParams->numbitsRI[nc];
    noutcqi = (S32 *)&pFecParams->numbitsCQI[nc];

    // Initialize all outputs to 0.
    *noutack = 0;
    *noutri = 0;
    *noutcqi = 0;

    // If there are some data bits (PUSCH with data and control).
    if (nindat > 0)
    {

        // Compute code block segmentation parameters used by transmitter.
        // C = total number of code blocks.
        // Cp = number of Kp sized code blocks.
        // Cm = number of Km sized code blocks.
        // Kp = K+ code block size in bits.
        // Km = K- code block size in bits.
        // F = number of filler bits.
        lte_code_blk_seg((nindat + 24), &C, &Cp, &Cm, &Kp, &Km, &F, blkSizeTab);

        // Compute summation Kr (total size for all code blocks).
        SumKr = (Cp * Kp) + (Cm * Km);

        //printf("Code block segmentation results:\n");
        //printf("  C=%d, Cp=%d, Cm=%d, Kp=%d, Km=%d, F=%d, SUMKr=%d\n",C,Cp,Cm,Kp,Km,F,SumKr);

        // Compute 4*Mpusch = 4*nrb*12.
        ac1 = 48 * nrb;

        // Compute (Npusch-initial)(Mpusch-initial).
        ac0 = nrbi * nsymi * 12;

        //********** Compute number of ACK code symbols **********

        // Compute O*(Mpusch-initial)(Npusch-initial)(Bpusch).
        ac2 = ninack * ac0 * Boffset_ACK[iack];

        // Shift result right 3 bits to compensate for x8 offset table scaling.
        ac2 = ac2 >> 3;

        // Compute ceil(ac2/SumKr).
        ac2 = ac2 + SumKr - 1;
        ac2 = ac2 / SumKr;

        // Number of coded symbols = min(ac2, ac1).
        if (ac1 < ac2)
            ac2 = ac1;
        *noutack = ac2;

        //********** Compute number of RI code symbols **********

        // Compute O*(Mpusch-initial)(Npusch-initial)(Bpusch).
        ac2 = ninri * ac0 * Boffset_RI[iri];

        // Shift result right 3 bits to compensate for x8 offset table scaling.
        ac2 = ac2 >> 3;

        // Compute ceil(ac2/SumKr).
        ac2 = ac2 + SumKr - 1;
        ac2 = ac2 / SumKr;

        // Number of coded symbols = min(ac2, ac1).
        if (ac1 < ac2)
            ac2 = ac1;
        *noutri = ac2;

        //********** Compute number of CQI code symbols **********

        // Compute number of CRC bits transmitted.
        C = 0;
        if (nincqi > 11)
            C = 8;

        // Compute (O+L)*(Mpusch-initial)(Npusch-initial)(Bpusch).
        ac2 = (nincqi + C) * ac0 * Boffset_CQI[icqi];

        // Shift result right 3 bits to compensate for x8 offset table scaling.
        ac2 = ac2 >> 3;

        // Compute ceil(ac2/SumKr).
        ac2 = ac2 + SumKr - 1;
        ac2 = ac2 / SumKr;

        // Compute (Mpusch*Npusch) - Qri/Qm.
        ac1 = (12 * nrb * nsym) - *noutri;

        // Number of coded symbols = min(ac2, ac1).
        if (ac1 < ac2)
            ac2 = ac1;
        *noutcqi = ac2;

        // Convert fron symbols to bits
        *noutri *= Qm;
        *noutack *= Qm;
        *noutcqi *= Qm;

        //printf("noutack=%d, noutri=%d, noutcqi=%d\n",*noutack,*noutri,*noutcqi);
        //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

        return;
    }

    // Compute 4*Mpusch = 4*nrb*12.
    ac1 = 48 * nrb;

    // Compute (Mpusch)*(Npusch).
    ac0 = nrb * nsym * 12;

    // Compute number of CRC bits transmitted.
    C = 0;
    if (nincqi > 11)
        C = 8;

    // Compute (Ocqi-min)
    ac3 = (C + nincqi);

    // Prevent invalid (Ocqi-min) = 0 case
    if (ac3 == 0)
        ac3 = 1;

    //********** Compute number of ACK code symbols **********

    // Compute O*(Mpusch)(Npusch)(Bpusch).
    ac2 = ninack * ac0 * Boffset_ACK[iack];
    ac2 = ac2 / Boffset_CQI[icqi];

    // Compute ceil(ac2/ac3).
    ac2 = ac2 + ac3 - 1;
    ac2 = ac2 / ac3;

    // Number of coded symbols = min(ac2, ac1).
    if (ac1 < ac2)
        ac2 = ac1;
    *noutack = ac2;

    //********** Compute number of RI code symbols **********
    // Compute O*(Mpusch)(Npusch)(Bpusch).
    ac2 = ninri * ac0 * Boffset_RI[iri];
    ac2 = ac2 / Boffset_CQI[icqi];

    // Compute ceil(ac2/ac3).
    ac2 = ac2 + ac3 - 1;
    ac2 = ac2 / ac3;

    // Number of coded symbols = min(ac2, ac1).
    if (ac1 < ac2)
        ac2 = ac1;
    *noutri = ac2;

    //********** Compute number of CQI code symbols **********
    // Number of coded symbols = (Npusch)(Mpusch) - Qri.
    *noutcqi = ac0 - *noutri;

    //printf("noutack=%d, noutri=%d, noutcqi=%d\n",*noutack,*noutri,*noutcqi);
    //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    // Convert fron symbols to bits
    *noutri *= Qm;
    *noutack *= Qm;
    *noutcqi *= Qm;

    return;
}

//---------------------------------------------------------------------------------------------------------
/** @brief Find bit positions for all ACK, RI, and CQI Multiplexed control in
 *         PUSCH data.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pFecParams - Pointer to FEC Structure containing all FEC parameters for all users
 *
 *  @param Cmux       - Number of columns to use for interleaver.
 *  @param CP         - Cyclic prefix indication (0normal, 1extended).
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |              |            |              |      |
 *  --------------------------------------------------------------------------------------------------
 *  |pFecParams |          |pInput        |1       |pStructure    |            |              |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Cmux       |          |input         |1       |Real          |(1:31:0)    |{9,10,11,12}  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |CP         |          |input         |1       |Real          |(16:0:0)    |{0,1}         |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |bs_rx_mux_control_findpos   |
 *  ---------------------------------------------------
 *  |Input Streams       |pFecParams                  |
 *  ---------------------------------------------------
 *  |Output Streams      |pFecParams                  |
 *  ---------------------------------------------------
 *  |Config Parameters   |Cmux, CP                    |
 *  ---------------------------------------------------
 *  |Status Parameters   |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Function implements finding all of the ACK, RI, and CQI
 * constellation points when there is control information
 * multiplexed in PUSCH data.
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 *  \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//----------------------------------------------------------------------------------------------------------
void bs_rx_mux_control_findpos(PULRXFECPARAMS pFecParams, S32 Cmux, S16 CP)
{
    S32 nincqi, ninri, ninack;
    S32 nincqi_sym, ninri_sym, ninack_sym;

    S32 Qm, num_sym, num_sym_cqi, num_sym_cqi1;
    S32 num_sym_empty, numRI;
    S32 Rmux, Rpmux, RRI, isym;
    S32 CQI_RI_sym, RRI1 = 0;
    S32 i, nc;

    for(nc = 0; nc < pFecParams->numChan; nc++)
    {
        if(pFecParams->channelType[nc] == PUSCH)
        {
            nincqi = pFecParams->numbitsCQI[nc];
            ninri  = pFecParams->numbitsRI[nc];
            ninack = pFecParams->numbitsACK[nc];
            Qm     = pFecParams->Qm[nc];

            nincqi_sym   = nincqi/Qm;
            ninri_sym    = ninri/Qm;
            ninack_sym   = ninack/Qm;

            //Find out the minimum number of CQI constellation
            //points per symbol by dividing by Cmux.  Then
            //find remainder to see how many symbols have extra
            //constellation point
            num_sym_cqi  = nincqi_sym / Cmux;
            num_sym_cqi1 = nincqi_sym % Cmux;

            //Clear all Symbol positions for ACK, RI, and CQI
//            memset(pFecParams->numSymPosACK[nc], 0, MAX_SYM_PER_SUBFRAME * sizeof(U16));
//            memset(pFecParams->numSymPosRI[nc], 0, MAX_SYM_PER_SUBFRAME * sizeof(U16));
//            memset(pFecParams->numSymPosCQI[nc], 0, MAX_SYM_PER_SUBFRAME * sizeof(U16));
            for(i=0; i<MAX_SYM_PER_SUBFRAME; i++) pFecParams->numSymPosACK[nc][i] = 0;
            for(i=0; i<MAX_SYM_PER_SUBFRAME; i++) pFecParams->numSymPosRI[nc][i] = 0;
            for(i=0; i<MAX_SYM_PER_SUBFRAME; i++) pFecParams->numSymPosCQI[nc][i] = 0;

            //Compute number of rows in channel de-interleaver buffer.
            Rmux = pFecParams->G[nc] / Cmux;

            //Compute number of symbol rows in channel de-interleaver buffer.
            Rpmux = Rmux;
            if (Qm == 2)
                Rpmux = Rmux >> 1;  //2 bits per symbol
            else if (Qm == 4)
                Rpmux = Rmux >> 2;  //4 bits per symbol
            else if (Qm == 6)
                Rpmux = Rmux / 6;   //6 bits per symbol

            //Reset number of CQI and RI collisions for each user
            CQI_RI_sym = 0;
            for(i = 0; i < NUM_SYM_ACK_RI; i++)
            {
                //Find number of constellations per symbol for ACK
                isym = ColSetACK[CP][i];
                num_sym = (U16)(ninack_sym >> 2);
                if ((ninack_sym & 0x03) > i)
                    num_sym++;
                pFecParams->numSymPosACK[nc][isym] = (U16)num_sym;

                //Find number of constellations per symbol for RI
                isym = ColSetRI[CP][i];
                num_sym = (U16)(ninri_sym >> 2);
                if ((ninri_sym & 0x03) > i)
                    num_sym++;
                pFecParams->numSymPosRI[nc][isym] = (U16)num_sym;

                //Need to check if we have RI and CQI present in the same symbol
                //If RI allocation collides with CQI allocation, CQI will need to skip
                //over RI constellation points and go to next valid location in the
                //channel de-interleaver buffer
                numRI   = num_sym;
                num_sym = num_sym_cqi;
                if (isym < num_sym_cqi1)
                    num_sym++;

                num_sym_empty = Rpmux - numRI;
                if (num_sym_empty < num_sym)
                    CQI_RI_sym += (num_sym - num_sym_empty);
            }

            //If RI and CQI allocation collide with each other, then we will
            //need to re-calculate the minimum number of CQI constellation points
            //per symbol.  For every 3 constellation points where CQI and RI
            //collide, there will be another RI and CQI collision.
            if (CQI_RI_sym)
            {
                RRI1 = CQI_RI_sym / 3;
                CQI_RI_sym += RRI1;

                //Must now re-check again if these additional collisions will
                //produce more collisions with RI points.
                RRI = (num_sym_cqi1 + RRI1) % Cmux;
                if (RRI > ColSetRI[CP][1])
                    CQI_RI_sym += 4;
                else if (RRI > ColSetRI[CP][2])
                    CQI_RI_sym += 2;
                else if (RRI > ColSetRI[CP][3])
                    CQI_RI_sym += 2;
                else if (RRI > ColSetRI[CP][0])
                    CQI_RI_sym += 1;
            }

            //Recalculate number of CQI constellation points in each symbol
            //after adding in the number of constellation points skipped
            //because of RI and CQI allocation collision.
            nincqi_sym  += CQI_RI_sym;
            num_sym_cqi  = nincqi_sym / Cmux;
            num_sym_cqi1 = nincqi_sym % Cmux;

            //Find number of constellations per symbol for CQI
            for(isym = 0; isym < MAX_PUSCH_DATASYMB_PER_SUBF; isym++)
            {
                num_sym = num_sym_cqi;
                if (isym < num_sym_cqi1)
                    num_sym++;

                numRI = (S32)(pFecParams->numSymPosRI[nc][isym]);
                if (numRI)
                {
                    num_sym_empty = Rpmux - numRI;
                    if (num_sym_empty < num_sym)
                        num_sym = num_sym_empty;
                }
                pFecParams->numSymPosCQI[nc][isym] = (U16)num_sym;
            }
        }
    } // for(nc = 0; nc < pFecParams->numChan; nc++)

    return;
}

#ifndef CEVA_INTRINSICS
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Below are FEC Utility functions needed for C-Code and ARM but not needed by Ceva Code
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//Downlink Tx FEC Utility Functions
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Base Station, Scrambler for input serial bistream bx.
 *         per 3GPP TS 36.211 Rel 8 Sec.6.3.1
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *  @param pPhy      Pointer to PLTEPHYDESC for this Phy Instance
 *  @param bx        Pointer to the input bitstream to be scrambled
 *  @param nRNTI     Radio Network Temporary Identifier value to be
 *                   used for scrambler initial value
 *  @param q         codeword number, one of two codewords transmitted in a
 *                   subframe {0,1}
 *  @param ns        Slot number in use
 *  @param by        Pointer to output bitstream
 *  @param length_bx Length of output bit stream
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name      |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  -------------------------------------------------------------------------------------------------------
 *  |pPhy      |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |bx        |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |nRNTI     |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |q         |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |ns        |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |by        |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |length_bx |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |scrambler                   |
 *  ---------------------------------------------------
 *  |Input Streams       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Output Streams      |DOXYGEN_TO_DO               |
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
 *  This function implements the LTE Base Station scrambler for the input
 *  serial bitstream bx per 3GPP TS 36.211 Rel 8 Sec.6.8.3.1<BR><BR>
 *
 * <B>Input Parameters:</B><BR>
 * - bx  = bx(0), bx(1), ..., bx(N-1)  = input bitstream, bx(0) first in time
 * nRNTI = radio network temporary identifier (= identity of the UE to which
 * the PDSCH is intended)<BR>
 * - q   = codeword number, one of two codewords transmitted in a subframe {0,1}
 * - ns  = slot number within a radio frame {0,1,...,19}<BR>
 *
 * <B>Outputs:</B><BR>
 * - by  = by(0), by(1), ..., by(N-1)  = output bitstream, by(0) first in time
 * - length_bx = length of output bit stream<BR>
 *
 * <B>Note:</B><BR>
 * - This function will be considered part of the FEC block since the input and
 * output is still bitstream memory format. In fact, for WiMAX and LTE, should
 * put everything up to the constellation mapper into the fec block. So the
 * output moved to the DSP modulator code is in terms of the 16-bit complex
 * values, not the bitstream values!
 *
 * <B>Shift Register Implementation:</B><BR>
 * -# by(n) = (bx(n) + c(n))mod2<BR>
 * where the scrambling sequence c(n) is given by 3GPP TS 36.211 Rel 8  Sec.7.2]
 * i.e.= length-31 Gold sequence.
 * -# The output sequence c(n) of length N, where n=0,1,2,...,N-1, is defined by
 *   - c(n) = (x1(n) + x2(n))mod2
 *   - x1(n+31) = (x1(n+3) + x1(n))mod2
 *   - x2(n+31) = (x2(n+3) + x2(i+2) + x2(n+1) + x2(n))mod2 <BR>
 * with initial conditions set at the start of each subframe, given as defined
 * in 3GPP TS 36.211 Rel 8 Sec.5.3.1
 *   - x1(0)=1, x1(n)=0 for n=1,2,...,30
 *   - x2 = nRNTI*2^14 + floor(ns/2)*2^9 + Ncell_id, for PDSCH (=cinit in the
 *   standard document)
 *   - x2   = nRNTI*2^14 + q*2^13 + floor(ns/2)*2^9 + Ncell_id, for PDSCH
 * (=cinit in the standard document)
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel 8 Sec.6.3.1
 *
 *
 * \ingroup DOXYGEN_TO_DO
 *
 **/
//-------------------------------------------------------------------------------------------
void scrambler(U32 cinit, S8 bx[], S8 by[], S32 length_bx)
{
    S32 i;
    U8 *cvec;

    cvec = MemAlloc(length_bx, DDR_MEM, 0);

    //run the pseudo random sequence generator for length_bx bits and scramble
    // the input sequence
    PseudoRandomSeqGen(length_bx, cinit, cvec);

    for (i = 0; i < length_bx; i++)
    {
        //scrambled sequence = input sequence xor'd with pseudorandom sequence
        by[i] = bx[i] ^ cvec[i];
    }
    MemFree(cvec, DDR_MEM);

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Base Station, Scrambler for input serial bistream bx.
 *         per 3GPP TS 36.211 Rel 8 Sec.6.3.1
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *  @param pPhy      Pointer to PLTEPHYDESC for this Phy Instance
 *  @param bx        Pointer to the input bitstream to be scrambled
 *  @param nRNTI     Radio Network Temporary Identifier value to be
 *                   used for scrambler initial value
 *  @param q         codeword number, one of two codewords transmitted in a
 *                   subframe {0,1}
 *  @param ns        Slot number in use
 *  @param by        Pointer to output bitstream
 *  @param length_bx Length of output bit stream
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name      |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  -------------------------------------------------------------------------------------------------------
 *  |pPhy      |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |bx        |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |nRNTI     |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |q         |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |ns        |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |by        |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  |length_bx |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |scrambler                   |
 *  ---------------------------------------------------
 *  |Input Streams       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Output Streams      |DOXYGEN_TO_DO               |
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
 *  This function implements the LTE Base Station scrambler for the input
 *  serial bitstream bx per 3GPP TS 36.211 Rel 8 Sec.6.8.3.1<BR><BR>
 *
 * <B>Input Parameters:</B><BR>
 * - bx  = bx(0), bx(1), ..., bx(N-1)  = input bitstream, bx(0) first in time
 * nRNTI = radio network temporary identifier (= identity of the UE to which
 * the PDSCH is intended)<BR>
 * - q   = codeword number, one of two codewords transmitted in a subframe {0,1}
 * - ns  = slot number within a radio frame {0,1,...,19}<BR>
 *
 * <B>Outputs:</B><BR>
 * - by  = by(0), by(1), ..., by(N-1)  = output bitstream, by(0) first in time
 * - length_bx = length of output bit stream<BR>
 *
 * <B>Note:</B><BR>
 * - This function will be considered part of the FEC block since the input and
 * output is still bitstream memory format. In fact, for WiMAX and LTE, should
 * put everything up to the constellation mapper into the fec block. So the
 * output moved to the DSP modulator code is in terms of the 16-bit complex
 * values, not the bitstream values!
 *
 * <B>Shift Register Implementation:</B><BR>
 * -# by(n) = (bx(n) + c(n))mod2<BR>
 * where the scrambling sequence c(n) is given by 3GPP TS 36.211 Rel 8  Sec.7.2]
 * i.e.= length-31 Gold sequence.
 * -# The output sequence c(n) of length N, where n=0,1,2,...,N-1, is defined by
 *   - c(n) = (x1(n) + x2(n))mod2
 *   - x1(n+31) = (x1(n+3) + x1(n))mod2
 *   - x2(n+31) = (x2(n+3) + x2(i+2) + x2(n+1) + x2(n))mod2 <BR>
 * with initial conditions set at the start of each subframe, given as defined
 * in 3GPP TS 36.211 Rel 8 Sec.5.3.1
 *   - x1(0)=1, x1(n)=0 for n=1,2,...,30
 *   - x2 = nRNTI*2^14 + floor(ns/2)*2^9 + Ncell_id, for PDSCH (=cinit in the
 *   standard document)
 *   - x2   = nRNTI*2^14 + q*2^13 + floor(ns/2)*2^9 + Ncell_id, for PDSCH
 * (=cinit in the standard document)
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel 8 Sec.6.3.1
 *
 *
 * \ingroup DOXYGEN_TO_DO
 *
 **/
//-------------------------------------------------------------------------------------------
void scrambler_byte(U32 cinit, S8 bx[], S8 by[], S32 length_bx)
{
    S32 i;
    U8 *cvec, *cvecByte;

    cvec = MemAlloc(8 * length_bx, DDR_MEM, 0);
    cvecByte = MemAlloc(length_bx, DDR_MEM, 0);

    //run the pseudo random sequence generator for length_bx bits and scramble
    // the input sequence
    PseudoRandomSeqGen(8 * length_bx, cinit, cvec);
    PACKBB(cvec, cvecByte, 8 * length_bx, 0);

    for (i = 0; i < length_bx; i++)
    {
        //scrambled sequence = input sequence xor'd with pseudorandom sequence
        by[i] = bx[i] ^ cvecByte[i];
    }
    MemFree(cvec, DDR_MEM);
    MemFree(cvecByte, DDR_MEM);

    return;
}

void scrambler_byte1(U32 *cinit, U32 *x1, S8 bx[], S8 by[], S32 length_bx, U32 init)
{
    S32 i;
    U8 *cvec, *cvecByte;

    cvec = MemAlloc(8 * length_bx, DDR_MEM, 0);
    cvecByte = MemAlloc(length_bx, DDR_MEM, 0);

    //run the pseudo random sequence generator for length_bx bits and scramble
    // the input sequence
    PseudoRandomSeqGen1(8 * length_bx, cinit, x1, cvec, init);
    PACKBB(cvec, cvecByte, 8 * length_bx, 0);

    for (i = 0; i < length_bx; i++)
    {
        //scrambled sequence = input sequence xor'd with pseudorandom sequence
        by[i] = bx[i] ^ cvecByte[i];
    }
    MemFree(cvec, DDR_MEM);
    MemFree(cvecByte, DDR_MEM);

    return;
}


/******************************************************************************
 tx_Mapper_Pack (PACK BITS FOR TX MAPPER FUNCTION)
 ******************************************************************************

 Function Tx_Mapper_Pack extracts bits from an array of input bytes and packs
 the bits into an output array of bytes that is to be used as the input array
 for the transmit mapper function (tx_Mapper).  The tx mapper function is
 used to map the input bits into vectors for the various modulation types
 (QPSK, 16QAM or 64QAM).  For the byte input array the lsb is the actual data
 bit that is used for the packing.  The type of packing depends on the
 specified modulation type as described below.

 QPSK:   Symbol size in bits:        2
 Symbols per output byte:    4
 Output byte packing order:  bit7 (1st) to bit0 (last)

 16QAM:  Symbol size in bits:        4
 Symbols per output byte:    2
 Output byte packing order:  bit7 (1st) to bit0 (last)


 64QAM:  Symbol size in bits:        6
 Symbols per output byte:    1
 Output byte packing order:  bit5 (1st) to bit0 (last)

 The functional calling format is shown below.

 nout = tx_Mapper_Pack(in, out, nin, modt)

 Where:  in =    input byte array (bit0 of each byte is the data bit).
 out =   output byte array.
 nin =   input array length in bytes.
 modt =  modulation type (1=QPSK, 2=16QAM, 3=64QAM).
 nout =  output array length in bytes.

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Pack bits for Tx mapper function.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param      in =    Input byte array (bit0 of each byte is the data bit).
 *  @param      out =   Output byte array.
 *  @param      nin =   Input array length in bytes.
 *  @param      modt =  Modulation type (1=QPSK, 2=16QAM, 3=64QAM).
 *  @return     nout =  Output array length in bytes.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |modt           |               |input        |1              |Real          |(1:15:0)  |{1,2,3}        |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |output       |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |tx_Mapper_Pack |
 *  --------------------------------------
 *  |Input Stream(s)     |in             |
 *  --------------------------------------
 *  |Output Stream(s)    |out            |
 *  --------------------------------------
 *  |Config Parameter(s) |nin,modt       |
 *  --------------------------------------
 *  |Status Parameter(s) |nout           |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function Tx_Mapper_Pack extracts bits from an array of input bytes and packs
 * the bits into an output array of bytes that is to be used as the input array
 * for the transmit mapper function (tx_Mapper).  The tx mapper function is
 * used to map the input bits into vectors for the various modulation types
 * (QPSK, 16QAM or 64QAM).  For the byte input array the lsb is the actual data
 * bit that is used for the packing.  The type of packing depends on the
 * specified modulation type as described below.
 *
 * TABLE_START
 *  ------------------------------------------------------------------
 *  |Modulation |Symbol Size |Symbols per |Output byte               |
 *  |Type       |(in bits)   |output byte |packing order             |
 *  ------------------------------------------------------------------
 *  |QPSK       |2           |4           |bit7 (1st) to bit0 (last) |
 *  ------------------------------------------------------------------
 *  |16QAM      |4           |2           |bit7 (1st) to bit0 (last) |
 *  ------------------------------------------------------------------
 *  |64QAM      |6           |1           |bit5 (1st) to bit0 (last) |
 *  ------------------------------------------------------------------
 *  TABLE_END
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_tx_downlink_modulator
 *
 *
 **/
//-------------------------------------------------------------------------------------------
S32 tx_Mapper_Pack(U8 in[], U8 out[], S32 nin, S16 modt)
{
    S32 i, j;
    S16 k, kk;
    U8 b, bb;

    // If QPSK
    if (modt == 1)
    {
        // Set 1st output bit location to bit7
        kk = 7;
    }
    // Else if 16QAM
    else if (modt == 2)
    {
        // Set 1st output bit location to bit7
        kk = 7;
    }
    // Else if 64QAM
    else if (modt == 3)
    {
        // Set 1st output bit location to bit5
        kk = 5;
    }
    else
    {
#ifdef _WIN32
        printf("Error! Modulation type must be 1, 2 or 3.\n");
#endif
        return (0);
    }

    // Initialize output array index
    j = 0;

    // Initialize current output byte bit position
    k = kk;

    // Initialize current output byte
    bb = 0;

    //  Do for each input bit
    for (i = 0; i < nin; i++)
    {
        // Shift and extract bit from input array
        b = 1 & (in[i]);

        // Shift bit into correct position and combine with others
        bb = bb | (b << k);

        // Decrement current output byte bit position
        k = k - 1;

        // If last output byte bit finished
        if (k < 0)
        {
            // Reset output byte bit position
            k = kk;

            // Write byte into output array
            out[j] = bb;

            // Increment output array index
            j = j + 1;

            // Clear current output byte
            bb = 0;
        }
    }

    // If partially filled output byte
    if (k != kk)
    {
        // Write byte into output array
        out[j] = bb;

        // Increment output array index
        j = j + 1;
    }

    // Return length of output array
    return (j);
}

/******************************************************************************
 calc_gcrc16 (LTE gcrc16 CRC calculation)
 *******************************************************************************

 Function calc_gcrc16 calculates the 16-bit CRC gcrc16 as defined in the 3GPP
 (LTE) standard TS 36.212 Section 5.1.1.  The functional calling format is shown
 below.

 calc_gcrc16(in, out, n)

 Where:  in =    Input bit stream organized as bytes.  Bits are packed in the
 bytes with the order msb=first, lsb=last.
 out =   16-bit CRC output organized as an array of two bytes.
 n =     Number of input bits to be processed.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE gcrc16 CRC calculation function.
 *
 *  <1> Hardware Resource:
 *  MDMA
 *
 *  @param     in =    Input bit stream organized as bytes.  Bits are packed in the
 *                     bytes with the order msb=first, lsb=last.
 *  @param     out =   16-bit CRC output organized as an array of two bytes.
 *  @param     n =     Number of input bits to be processed.
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |(n+7)/8        |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |2              |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |calc_gcrc16                 |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n                           |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function calc_gcrc16 calculates the 16-bit CRC gcrc16 as defined in the 3GPP (LTE)
 * standard TS 36.212 Section 5.1.1.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.1.1.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void calc_gcrc16(U8 in[], U8 out[2], S32 n)
{

    //*****************************************************************************
    //**********        GENERATOR POLYNOMIAL PARAMETER DEFINITION        **********
    //*****************************************************************************

    // The generator polynomial for the 16-bit gcrc16 is defined in the 3GPP
    // standard TS 36.212 Section 5.1.1 as is shown below.

    //      gcrc16(D) = D**16 + D**12 + D**5 + 1

    // For this implementation a 32-bit integer is used for the shift register as
    // shown below.

    //      reg bit:    15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //      D exponent:  8  7  6  5  4  3  2  1  0 -1 -2 -3 -4 -5 -6 -7
    //      Polynomial:           *              *

    //      reg bit:    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
    //      D exponent:                         16 15 14 13 12 11 10  9
    //      Polynomial:                          *           *

    // A representation of the generator polynomial (P_CRC16_POLY) is generated by
    // forming a constant that contains a 1 for each of the *s and zeros elsewhere.

#define     P_CRC16_POLY     0x00881080

    //*****************************************************************************

    U32 reg;
    S32 i, j, k;

    // k = number of input bytes
    k = (n + 7) >> 3;
    // Initialize input byte index
    j = 1;
    // Initialize shift register (b16 - b9) to 1st input byte
    reg = in[0] << 16;
    // If more than 1 input byte, then
    if (k > 1)
    {
        // Initialize shift register (b8 - b1) to 2nd input byte
        reg = reg | (in[1] << 8);
    }

    // For each input bit
    for (i = 0; i < n; i++)
    {
        // If bit at a byte boundary, then
        if (i % 8 == 0)
        {
            // Increment input array index
            j = j + 1;
            // If more input bytes, then
            if (j < k)
            {
                // Load shift register (b0 - bn7) with next byte
                reg = reg | (0x00ff & in[j]);
            }

        }
        // If bit D**16 = 1, then xor register with generator polynomial
        if ((reg & 0x800000) != 0)
        {
            reg = reg ^ P_CRC16_POLY;
        }
        // Move shift register to next bit
        reg = reg << 1;
    }

    // Write 16-bit CRC to output bytes
    out[0] = (U8) ((reg >> 16) & 255);
    out[1] = (U8) ((reg >> 8) & 255);

    return;
}

/******************************************************************************
 AppendCRC16 (LTE compute and append CRC16 function)
 *******************************************************************************

 AppendCRC16 copies an input to an output bit stream, computes the 16-bit CRC
 (gCRC16) described in the 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12)
 section 5.1.1 and then appends the 16-bit CRC to the output bit stream.  The
 bit streams are organized as arrays of bytes whereby the lsb of each
 byte contains the input bit.  The functional calling format is defined below.

 AppendCRC16(in_bitstream, out_bitstream, length)

 Where:  in_bitstream =  Input bit stream organized as bytes (lsb=bit).
 out_bitstream = Output bit stream organized as bytes (lsb=bit).
 length =        Length of in_bitstream.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE gCRC16 16-bit CRC computation function.
 *
 *  <1> Hardware Resource:
 *  MDMA
 *
 *  @param     in_bitstream =  Input bit stream organized as bytes (lsb=bit).
 *  @param     out_bitstream = Output bit stream organized as bytes (lsb=bit).
 *  @param     length =        Length of in_bitstream.
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in_bitstream   |               |input        |length         |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out_bitstream  |               |output       |length+16      |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |length         |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |AppendCRC16                 |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in_bitstream                |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out_bitstream               |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |length                      |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * AppendCRC16 copies an input to an output bit stream, computes the 16-bit CRC
 * (gCRC16) described in the 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12)
 * section 5.1.1 and then appends the 16-bit CRC to the output bit stream.  The
 * bit streams are organized as arrays of bytes whereby the lsb of each
 * byte contains the input bit.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.1.1
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void AppendCRC16(U8 in_bitstream[], U8 out_bitstream[], S32 length)
{

    S32 i, k;
    U8 ShiftReg[17];

    // Copy input to output bit stream
    for (i = 0; i < length; i++)
    {
        out_bitstream[i] = in_bitstream[i];
    }

    // Append 16 zeros to output bit stream
    for (i = length; i < length + 16; i++)
    {
        out_bitstream[i] = 0;
    }

    // Load 1st 16 input bits into shift register
    for (i = 0; i < 16; i++)
    {
        ShiftReg[16 - i] = out_bitstream[i];
    }

    // Calculate CRC16 1 bit at a time
    for (k = 16; k < length + 16; k++)
    {
        ShiftReg[12] = ShiftReg[12] ^ ShiftReg[16];
        ShiftReg[5] = ShiftReg[5] ^ ShiftReg[16];
        ShiftReg[0] = out_bitstream[k] ^ ShiftReg[16];

        //Shift bits one place
        for (i = 16; i > 0; i--)
        {
            ShiftReg[i] = ShiftReg[i - 1];
        }

    }

    //Overwrite temporary 16-zeros with the 16-bit CRC
    for (i = length; i < length + 16; i++)
    {
        out_bitstream[i] = ShiftReg[16 - i + length];
    }

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   crc[] DOXYGEN_TO_DO
 *  @param   num_bits DOXYGEN_TO_DO
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
 *  |crc[]         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |num_bits      |              |              |              |              |              |              |
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
void PatchCRC16(U8 crc[], S32 num_bits)
{

    S32 k;
    U16 sr;
    U16 bit;

    sr = (crc[0] << 8) | crc[1];
    for (k = num_bits - 1; k >= 0; k--)
    {
        bit = sr & 1;
        sr >>= 1;
        sr |= (bit << 15);
        if (bit)
        {
            sr ^= 0x0810;
        }
    }
    crc[0] = (U8) (sr >> 8);
    crc[1] = (U8) sr;

    return;
}

/******************************************************************************
 TBCCENC_1_3 (LTE 1/3 rate tail biting convolutional encoding function)
 *******************************************************************************

 TBCCENC_1_3 implements the 1/3 rate tail biting convolutional encoder described
 in the 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.1.3.1.
 The functional calling format is defined below.

 TBCCENC_1_3(in, out, n)

 Where:  in =    Input bit stream organized as bytes (lsb=bit).
 out =   Output bit stream organized as bytes (lsb=bit).
 out[0 : n-1] = encoder d(0) outputs.
 out[n : 2n-1] = encoder d(1) outputs.
 out[2n : 3n-1] = encoder d(2) outputs.
 n =     Length of input array (in).

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE 1/3 rate tail-biting convolutional encoding function.
 *
 *  <1> Hardware Resource:
 *  FEC
 *
 *  @param     in =    Input bit stream organized as bytes (lsb=bit).
 *  @param     out =   Output bit stream organized as bytes (lsb=bit).
 *                     out[0 : n-1] = encoder d(0) outputs.
 *                     out[n : 2n-1] = encoder d(1) outputs.
 *                     out[2n : 3n-1] = encoder d(2) outputs.
 *  @param     n =     Length of input array (in).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |n              |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |3n             |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |TBCCENC_1_3                 |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n                           |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * TBCCENC_1_3 implements the 1/3 rate tail biting convolutional encoder described
 * in the 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.1.3.1.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.1.3.1.
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void TBCCENC_1_3(U8 in[], U8 out[], S32 n)
{
    S32 i;
    U8 sreg[6] =
    { 0 };

    // Initialize encoder shift register using last 6 input bits
    for (i = 0; i < 6; i++)
    {
        sreg[i] = in[n - 1 - i];
    }

    // Do for each input bit
    for (i = 0; i < n; i++)
    {
        // Compute d(0) encoder output bits for each input bit
        out[i] = in[i] ^ sreg[1] ^ sreg[2] ^ sreg[4] ^ sreg[5];

        // Compute d(1) encoder output bits for each input bit
        out[i + n] = in[i] ^ sreg[0] ^ sreg[1] ^ sreg[2] ^ sreg[5];

        // Compute d(2) encoder output bits for each input bit
        out[i + n + n] = in[i] ^ sreg[0] ^ sreg[1] ^ sreg[3] ^ sreg[5];

        // Update shift register
        sreg[5] = sreg[4];
        sreg[4] = sreg[3];
        sreg[3] = sreg[2];
        sreg[2] = sreg[1];
        sreg[1] = sreg[0];
        sreg[0] = in[i];
    }

    return;

}

/******************************************************************************
 CC_BitSelection (LTE bit selection for convolutionally coded channels)
 *******************************************************************************

 CC_BitSelection implements the bit selection function for convolutionally
 coded channels described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 (2008-12) section 5.1.4.2.2.  The functional calling format is defined below.

 CC_BitSelection(in, null, n, E)

 Where:  in =    Sub-block interleaver output bit streams v(0), v(1), v(2)
 organized as bytes. Bit0 = data bit.
 null    Byte array indicating if corresponding in[] element is valid
 data or an inserted <null>.  0=valid, 1=<null>.
 out =   Output bit stream organized as bytes (Bit0 = data bit).
 n =     Length of in.
 E =     Length of out.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE rate matching bit selection function for convolutionally encoded channels.
 *
 *  <1> Hardware Resource:
 *  CEVA or ARM
 *
 *  @param     in =    Input bit stream organized as bytes (lsb=bit).
 *  @param     null =  Input byte array indicating if corresponding in[] element
 *                     is valid or an inserted <null>.  0 = valid, 1 = <null>.
 *  @param     out =   Output bit stream organized as bytes (lsb=bit).
 *  @param     n =     Length of input array (in).
 *  @param     E =     Length of output array (out).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |n              |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |null           |               |input        |n              |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |E              |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |E              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |CC_BitSelection             |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in, null                    |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n, E                        |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * CC_BitSelection implements the bit selection function for convolutionally
 * coded channels described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 * (2008-12) section 5.1.4.2.2.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void CC_BitSelection(U8 in[], U8 null[], U8 out[], S32 n, S32 E)
{

    S32 i, j;

    // Initialize output array index
    i = 0;
    // Initialize input array index
    j = 0;

    laba:

    // If input is not a null, then
    if (null[j] == 0)
    {
        // Use data bit as next output bit
        out[i] = in[j];

        // Increment output array index
        i = i + 1;

        // If all outputs selected, then exit
        if (i == E)
            goto labb;
    }

    // Increment input array index
    j = j + 1;
    // If last element of input array, then reset to beginning
    if (j == n)
        j = 0;
    // Continue to select remaining output bits
    goto laba;

    labb:

    return;
}

/******************************************************************************
 CC_SubBlockInterleaver (LTE sub-block interleaver for conv coded channels)
 *******************************************************************************

 CC_SubBlockInterleaver implements the sub-block interleaver for convolutionally
 coded channels described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 (2008-12) section 5.1.4.2.1.  To fully implement the required rate matching
 function, CC_SubBlockInterleaver must be run three times, once for each of
 three convolutional encoder outputs.  The functional calling format is defined
 below.

 nout = CC_SubBlockInterleaver(in, out, null, n)

 Where:  in =    Input bit stream organized as bytes (Bit0 = data bit).
 out =   Output bit stream organized as bytes (Bit0 = data bit).
 null =  Output array organized as bytes indicating if corresponding
 out[] element is valid or an inserted <null>.  0 = valid,
 1 = inserted <null>.
 n =     Length of in.
 nout =  Length of out returned by function.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE sub-block interleaver function for convolutionally encoded channels.
 *
 *  <1> Hardware Resource:
 *  FEC
 *
 *  @param     in =    Input bit stream organized as bytes (lsb=bit).
 *  @param     out =   Output bit stream organized as bytes (lsb=bit).
 *  @param     null =  Output byte array indicating if corresponding in[] element
 *                     is valid or an inserted <null>.  0 = valid, 1 = <null>.
 *  @param     n =     Length of input array (in).
 *  @return    nout =  length of out and null arrays in bytes.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |n              |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |null           |               |output       |nout           |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |output       |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |CC_SubBlockInterleaver      |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out, null                   |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n                           |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |nout                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * CC_SubBlockInterleaver implements the sub-block interleaver for convolutionally
 * coded channels described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 * (2008-12) section 5.1.4.2.1.  To fully implement the required rate matching
 * function, CC_SubBlockInterleaver must be run three times, once for each of
 * three convolutional encoder outputs.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.1.4.2.1
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
S32 CC_SubBlockInterleaver(U8 in[], U8 out[], U8 null[], S32 n)
{

    S32 i, j, k, m;
    S32 nrow, nout, nfill;

    // Define inter-column permutation table
    S16 P[32] =
    { 1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31, 0, 16, 8, 24, 4, 20, 12, 28, 2,
      18, 10, 26, 6, 22, 14, 30 };

    // define temporary buffer
    U8 *tmp;

    // Compute number of rows with 32 columns each
    nrow = (n + 31) >> 5;
    // Compute interleaver buffer size = 32*nrow
    nout = nrow << 5;
    // Allocate temporary buffer
    tmp = (U8*) MemAlloc(nout, DDR_MEM, 0);
    // Compute number of null filler bits
    nfill = nout - n;
    // Initialize temporary buffer index
    j = 0;
    // If needed, load nulls into temporary buffer.  Use bit7=1 to indicate null
    for (i = 0; i < nfill; i++)
    {
        tmp[j] = 0x80;
        j = j + 1;
    }

    // Load inputs into temporary buffer.  Use bit7=0 to indicate valid input.
    for (i = 0; i < n; i++)
    {
        tmp[j] = in[i] & 1;
        j = j + 1;
    }

    // Initialize output array index
    m = 0;

    // Do for each column
    for (i = 0; i < 32; i++)
    {
        // Do for each row
        for (j = 0; j < nrow; j++)
        {
            // Compute temporary buffer index to use for next output
            k = P[i] + (j << 5);
            // Copy temporary buffer data element to output array
            out[m] = tmp[k] & 1;
            // Set corresponding null array element
            null[m] = tmp[k] >> 7;
            // Increment output array index
            m = m + 1;
        }
    }

    // De-allocate temporary buffer
    MemFree(tmp, DDR_MEM);

    // return size of output array
    return (nout);

}

/////////////////////////////////////////////////////////////////////////////////////////////
//Uplink Rx FEC Utility Functions
/////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------
/** @brief Calculates the G value, ModulationType and the number of bytes based on the MCS value and the number
 *  of resource blocks  allocated to specific ChanID.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   ptr     - Pointer to the TxVector/RxVector for specific ChanID
 *  @param   pPhy    - Pointer to LTEPHYDESC structure
 *  @param   num_sym - Number of symbols per subframe
 *  @param   ChanId  - Current Channel ID.
 *  @param   index   - Index for the channel in the LTEPHYDESC structure
 *
 *
 *  @return  TotalBits(G) for this specific ChanID
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------
 *  |Name         |Name in   |Direction |Length  |Type      |Data Format |Valid Range |Units  |
 *  |             |Standard  |          |        |          |            |            |       |
 *  -----------------------------------------------------------------------------------------
 *  |pInt         |          |pInput    |        |pStructure|            |            |       |
 *  -----------------------------------------------------------------------------------------
 *  |pRxFecParams |          |pOutput   |        |pStructure|            |            |       |
 *  -----------------------------------------------------------------------------------------
 *  |num_sym      |          |Input     |1       |short     |(1:15:0)    |14          |       |
 *  -----------------------------------------------------------------------------------------
 *  |ChanIdx      |          |Input     |1       |short     |(1:15:0)    |Full range  |       |
 *  -----------------------------------------------------------------------------------------
 *  |Nrb_sc       |          |Input     |1       |long      |(1:31:0)    |Full range  |       |
 *  -----------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |PuschMCSbasedCalculations   |
 *  ---------------------------------------------------
 *  |Input Stream        |none                        |
 *  ---------------------------------------------------
 *  |Output Stream       |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pInt,Nrb_sc,num_sym,ChanIdx |
 *  ---------------------------------------------------
 *  |Status Parameters   |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Calculates the G value, ModulationType and the number of bytes based on the MCS value and the number of resource blocks
 *  allocated to the specific ChanID.
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
 *  \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//-------------------------------------------------------------------------------------------
void PuschMCSbasedCalculations(PULCHANDESC pInt, ULRXFECPARAMS *pRxFecParams, S16 num_sym, S16 Nrb_sc, U32 ChanIdx)
{
    int i;
    U32 NumREperRB, SymPerSubFrame;
    U32 numRBs, TotalBits;

    U16 mcs = (U16) pInt->ulSubChInfo.mcinfo.mcsType;

    numRBs = 0;
    NumREperRB = Nrb_sc;
    SymPerSubFrame = num_sym - 2; //subtract 2 Pilot Symbols
    for (i = 0; i < pInt->ulSubChInfo.mapInfo.numberofEntries; i++)
        numRBs += pInt->ulSubChInfo.mapInfo.reselmInfo[i].numRes;

    TotalBits = numRBs * NumREperRB;
    TotalBits = TotalBits * 2 * pInt->ulSubChInfo.mcinfo.modulationType;
    pRxFecParams->G[ChanIdx] = TotalBits * SymPerSubFrame;
    pRxFecParams->numBytes[ChanIdx] = TBSizeTable[numRBs - 1][TBSIndexTable[mcs]] / 8;

    // 3GPP TS 36.213 Table 7.1.7.2.1-1 Transport block size table + Itbs and Nprb-1
    pRxFecParams->Itbs[ChanIdx] = (S16) (TBSIndexTable[mcs]);
    pRxFecParams->Nprbm1[ChanIdx] = (S16) (numRBs - 1);
}

/******************************************************************************
 *******************************************************************************
 3GPP (LTE) Standard Section 5.3.1 Descrambling Function
 *******************************************************************************

 Function descrambler implements the inverse of the scrambler function defined
 in the LTE standard.  It is important to note that the descrambling function is
 not properly a part of the actual Rx channel coding section defined in the LTE
 standard.  However, the function is included in this file since in the actual
 system hardware it will be included in the Rx FEC hardware block.  This
 function is essentially identical to the transmitter's scrambler function
 scrambler( ) that can be found in file bs_tx_PhysicalChannels.  The only
 difference is that the input and output arrays are 16-bit 2's complement soft
 decisions instead of actual single data bits.  Since the two functions are
 almost identical, the C code and documentation shown below were derived from
 the original scrambler function.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE base station soft decision descrambler function.
 *
 *  <1> Hardware Resource:
 *  FEC
 *
 *   @param     cinit     = initial seed to the descrambler function
 *   @param     bx        = pointer to the input soft decision array to be descrambled.
 *   @param     by        = pointer to output soft decision array.
 *   @param     length_bx = length of input and output arrays.
 *
 *   @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |cinit          |               |input        |1              |Real          |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |bx             |               |input        |length_bx      |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |by             |               |output       |length_bx      |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |length_bx      |               |input        |1              |Real          |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |descrambler                   |
 *  -----------------------------------------------------
 *  |Input Stream(s)     |bx                            |
 *  -----------------------------------------------------
 *  |Output Stream(s)    |by                            |
 *  -----------------------------------------------------
 *  |Config Parameter(s) |cinit, length_bx              |
 *  -----------------------------------------------------
 *  |Status Parameter(s) |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function descrambler implements the inverse of the scrambler function defined
 * in the LTE standard.  It is important to note that the descrambling function is
 * not properly a part of the actual Rx channel coding section defined in the LTE
 * standard.  However, the function is included in this file since in the actual
 * system hardware it will be included in the Rx FEC hardware block.  This
 * function is essentially identical to the transmitter's scrambler function
 * scrambler( ) that can be found in file bs_tx_PhysicalChannels.  The only
 * difference is that the input and output arrays are 16-bit 2's complement soft
 * decisions instead of actual single data bits.  Since the two functions are
 * almost identical, the C code and documentation shown below were derived from
 * the original scrambler function.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Section 5.3.1
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void descrambler(U32 cinit, S8 bx[], S8 by[], S32 length_bx, U8 *cvec)
{
    S32 i;

    //run the pseudo random sequence generator for length_bx bits and scramble
    // the input sequence
    PseudoRandomSeqGen(length_bx, cinit, cvec);

    for (i = 0; i < length_bx; i++)
    {
        //scrambled sequence = input sequence xor'd with pseudorandom sequence
        by[i] = bx[i];
        if (cvec[i] != 0)
        {
            by[i] = -by[i];
        }
    }

    return;
}

///////////////////////////////////////////////////////////////////////////////
//Multiplex Control and Data Uplink FEC Functions
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 LTE_Chan_Deint_QPSK(LTE transport channel deinterleaving function for QPSK)
 *******************************************************************************

 LTE_Chan_Deint_QPSK implements the deinterleaving function for QPSK encoded
 symbols as described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 (2008-12) section 5.2.2.8.  This function is used to separate a combined
 soft decision input array into the following three separate soft decision
 output arrays:

 out1    soft decisions for coded channel rank (RI) information bits
 out2    soft decisions for multiplexed coded transport block and channel
         quality information bits
 out3    soft decisions for coded HARQ ACK information bits

 Note, it is not required that all output arrays be used.  Any output array can
 be skipped by setting its corresponding array length = 0.  The functional
 calling format is defined below.

 LTE_Chan_Deint_QPSK(in, inb, out1, out2, out3,
                     n1, n2, n3, Cmux, Rmux, CP, nRI, nACK)

 in      combined/interleaved soft decision input array after de-scrambling
 inb     combined/interleaved soft decision input array before de-scrambling
 out1    coded channel rank (RI) soft decision output array
 out2    multiplexed coded transport block and channel quality soft decision
         output array
 out3    coded HARQ ACK soft decision output array
 n1      length of out1 array in bytes
 n2      length of out2 array in bytes
 n3      length of out3 array in bytes
 Cmux    number of columns to use for interleaver
 Rmux    number of rows to use for interleaver
 CP      cyclic prefix (0=normal, 1=extended)
 nRI     number of RI bits encoded (1 or 2)
 nACK    number of ACK bits encoded (1 or 2)

 The LTE_Chan_Deint_QPSK function will not work properly if invalid calling
 arguments are specified.  The required calling argument rules and
 restrictions are as follows:

 1)  n1%2 = 0
 2)  n2%2 = 0
 3)  n3%2 = 0

 Before LTE_Chan_Deint_QPSK executes, the calling arguments are checked.  If
 any of the rules are violated, an error message will be printed and the
 function will return.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE transport channel deinterleaving function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param     in =    Combined/interleaved soft decision input array after
 *                     de-scrambling.
 *  @param     inb =   Combined/interleaved soft decision input array before
 *                     de-scrambling.
 *  @param     out1 =  Coded channel rank (RI) soft decision output array.
 *  @param     out2 =  Multiplexed coded transport block and channel quality
 *                     soft decision output array.
 *  @param     out3 =  Coded HARQ ACK soft decision output array.
 *  @param     n1 =    Length of out1 array in bytes.
 *  @param     n2 =    Length of out2 array in bytes.
 *  @param     n3 =    Length of out3 array in bytes.
 *  @param     Cmux =  Number of columns to use for interleaver.
 *  @param     Rmux =  Number of rows to use for interleaver.
 *  @param     CP =    Cyclic prefix (0=normal, 1=extended).
 *  @param     nRI =   Number of encoded RI bits (1 or 2).
 *  @param     nACK =  Number of encoded ACK bits (1 or 2).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |n1+n2          |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |inb            |               |input        |n1+n2          |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out1           |               |output       |n1             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out2           |               |output       |n2             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out2           |               |output       |n3             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n1             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n2             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n3             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Cmux           |               |input        |1              |Real          |(1:31:0)  |{9,10,11,12}   |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Rmux           |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CP             |               |input        |1              |Real          |(1:15:0)  |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nRI            |               |input        |1              |Real          |(1:15:0)  |{1,2}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nACK           |               |input        |1              |Real          |(1:15:0)  |{1,2}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LTE_Chan_Deint_QPSK           |
 *  -----------------------------------------------------
 *  |Input Stream(s)     |in, inb                       |
 *  -----------------------------------------------------
 *  |Output Stream(s)    |out1,out2,out3                |
 *  -----------------------------------------------------
 *  |Config Parameter(s) |n1,n2,n3,Cmux,Rmux,CP,nRI,nACK|
 *  -----------------------------------------------------
 *  |Status Parameter(s) |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * LTE_Chan_Deint_QPSK implements the deinterleaving function for QPSK encoded
 * symbols as described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 * (2008-12) section 5.2.2.8.  This function is used to separate a combined
 * soft decision input array into the following three separate soft decision
 * output arrays:
 *
 *  - out1    soft decisions for coded channel rank (RI) information bits
 *  - out2    soft decisions for multiplexed coded transport block and channel
 *            quality information bits
 *  - out3    soft decisions for coded HARQ ACK information bits
 *
 * Note, it is not required that all output arrays be used.  Any output array can
 * be skipped by setting its corresponding array length = 0.<BR><BR>
 *
 * The LTE_Chan_Deint_QPSK function will not work properly if invalid calling
 * arguments are specified.  The required calling argument rules and
 * restrictions are as follows:
 *
 *  -#  n1 % 2 = 0
 *  -#  n2 % 2 = 0
 *  -#  n3 % 2 = 0

 * Before LTE_Chan_Deint_QPSK executes, the calling arguments are checked.  If
 * any of the rules are violated, an error message will be printed and the
 * function will return.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.2.2.8
 *
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void LTE_Chan_Deint_QPSK(U16 in[], U16 inb[], U16 out1[], U16 out2[], U16 out3[],
                         S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP,
                         S16 nRI, S16 nACK)
{

    S32 /*ntot, */Rpmux, R, RRI;
    S32 r, i, j, k, c, m, ac;
    U32 ac1, ac2, ac3;
    U16 *tmploc;

#ifdef _WIN32

    // If input argument errors, then.
    if( ((n1%2) != 0) || ((n2%2) != 0) || ((n3%2) != 0) )
    {
        printf("Invalid n1, n2 or n3 error!!! nx%2 must be 0!\n");
        return;
    }

#endif

    //printf("LTE_Chan_Deint_QPSK\n");
    //printf("n1=%d, n2=%d, n3=%d, CP=%d, Cmux=%d, Rmux=%d, nRI=%d, nACK=%d\n",
    //        n1, n2, n3, CP, Cmux, Rmux, nRI, nACK);

    // Compute number of RI QPSK (2-bit) encoded symbols.
    n1 = n1 >> 1;

    // Compute number of combined CQI and data QPSK (2-bit) encoded symbols.
    n2 = n2 >> 1;

    // Compute number of HARQ ACK QPSK (2-bit) encoded symbols
    n3 = n3 >> 1;

    // Rpmux = number of QPSK symbol rows in de-intereleaver buffer;
    Rpmux = Rmux >> 1;

    // ntot = total number of QPSK symbols in de-interleaver buffer.
    //ntot = Rpmux * Cmux;

    //printf("n1=%d, n2=%d, n3=%d, ntot=%d, Rpmux=%d\n",n1, n2, n3, ntot, Rpmux);

    // If there are HARQ ACK soft decisions to be extracted, then.
    if (n3 > 0)
    {

        // Initialize current symbol row to last row.
        r = Rpmux - 1;

        // Initialize ACK symbol extraction table index.
        j = 0;

        // Initialize ACK output array index.
        k = 0;

        // For each 2-bit QPSK HARQ ACK symbol to be extracted
        for (i = 0; i < n3; i++)
        {
            // c = column of HARQ ACK symbol.
            c = ColSetACK[CP][j];

            // ac = input array index for HARQ ACK symbol.
            ac = r + (c * Rpmux);

            // If 2-bit ACK, then QPSK symbol contains data bits only.
            if(nACK == 2)
            {

                // Copy 2-bit QPSK HARQ ACK symbol into output array.
                // Two 8-bit soft decisions are copied at a time.
                out3[k] = in[ac];

            }
            // Else each QPSK symbol contains (data bit, y bit).
            else
            {

                // Read 2-bit QPSK ACK soft decisions after descrambling.
                ac1 = in[ac];

                // Read 2-bit QPSK ACK soft decisions before descrambling.
                ac2 = inb[ac];

                // Need to determine if in[] 1st byte sign bit changed.
                ac3 = ac1 ^ ac2;
                ac3 = ac3 & 0x80;

                // If sign bit changed then need to invert inb[] 2nd byte.
                if(ac3 != 0) ac2 = (~ac2) + 0x100;

                // Combine in[] 1st byte and inb[] 2nd byte and copy into output array.
                ac1 = ac1 & 0x00ff;     // data
                ac2 = ac2 & 0xff00;     // y
                ac1 = ac1 | ac2;        // data, y
                out3[k] = (U16) ac1;
            }

            // Clear HARQ ACK soft decisions in input array.
            in[ac] = 0;

            // Increment HARQ ACK output array index;
            k = k + 1;

            // Advance ACK symbol extraction table index;
            j = (j + 1) & 3;

            // Decrement row after 4 symbols have been extracted for row.
            if (j == 0)
                r = r - 1;

        }

    }

    // If there are RI soft decisions to be extracted, then.
    if (n1 > 0)
    {

        // Initialize current symbol row to last row.
        r = Rpmux - 1;

        // Initialize RI symbol extraction table index.
        j = 0;

        // Initialize RI output array index.
        k = 0;

        // For each 2-bit RI symbol to be extracted.
        for (i = 0; i < n1; i++)
        {
            // c = column of RI symbol.
            c = ColSetRI[CP][j];

            // ac = input array index for RI symbol.
            ac = r + (c * Rpmux);


            // If 2-bit RI, then QPSK symbol contains data bits only.
            if(nRI == 2)
            {

                // Copy 2-bit QPSK HARQ ACK symbol into output array.
                // Two 8-bit soft decisions are copied at a time.
                out1[k] = in[ac];

            }
            // Else each QPSK symbol contains (data bit, y bit).
            else
            {

                // Read 2-bit QPSK RI soft decisions after descrambling.
                ac1 = in[ac];

                // Read 2-bit QPSK RI soft decisions before descrambling.
                ac2 = inb[ac];

                // Need to determine if in[] 1st byte sign bit changed.
                ac3 = ac1 ^ ac2;
                ac3 = ac3 & 0x80;

                // If sign bit changed then need to invert inb[] 2nd byte.
                if(ac3 != 0) ac2 = (~ac2) + 0x100;

                // Combine in[] 1st byte and inb[] 2nd byte and copy into output array.
                ac1 = ac1 & 0x00ff;     // data
                ac2 = ac2 & 0xff00;     // y
                ac1 = ac1 | ac2;        // data, y
                out1[k] = (U16) ac1;

            }

            // Increment RI output array index;
            k = k + 1;

            // Advance RI symbol extraction table index;
            j = (j + 1) & 3;

            // Decrement row after 4 symbols have been extracted for row.
            if (j == 0)
                r = r - 1;

        }

    }

    // If there are multiplexed coded transport block and channel quality
    // soft decisions to be extracted, then.
    if (n2 > 0)
    {

        // Compute number of rows containing RI symbols.
        RRI = (n1 + 3) >> 2;

        // Compute number of rows only containing multiplexed data and channel
        // quality symbols;
        R = Rpmux - RRI;

        //printf("RRI=%d, R=%d\n",RRI, R);

        // Initialize input array index.
        k = 0;

        // Initialize output array index.
        m = 0;

        //  For each row in de-interleaver buffer.
        for (i = 0; i < R; i++)
        {

            // Initialize input array index.
            r = k;

            // For each column in row.
            for (j = 0; j < Cmux; j++)
            {
                // Copy input to output array.
                // Two 8-bit soft decisions are copied at a time.
                out2[m] = in[r];

                // Increment output array index
                m = m + 1;

                // Exit if all symbols copied.
                if (m == n2)
                    goto LTE_Chan_Deint_QPSK_laba;

                // Advance input array index to next column.
                r = r + Rpmux;

            }

            // Increment input array index to next row.
            k = k + 1;

        }

        // If there are rows with RI symbols, then.
        if (RRI > 0)
        {

            // Allocate temporary array to indicate location of RIs.
            tmploc = (U16*) MemAlloc(RRI * sizeof(U16), DDR_MEM, 0);

            // Clear temporary array elements.
            for (i = 0; i < RRI; i++)
            {
                tmploc[i] = 0;
            }

            // Initialize RI row to last one;
            r = RRI - 1;

            // Initialize RI column table index.
            j = 0;

            // For each RI symbol in row.
            for (i = 0; i < n1; i++)
            {
                // c = column of RI symbol.
                c = ColSetRI[CP][j];

                // Compute bit in correct position.
                ac = (1 << c);

                // Update RI location array.
                tmploc[r] = tmploc[r] + (U16) ac;

                // Advance RI symbol column table index.
                j = (j + 1) & 3;

                // Decrement row every 4th RI symbol.
                if (j == 0)
                    r = r - 1;

            }

            // For each row in de-interleaver buffer with RIs.
            for (i = 0; i < RRI; i++)
            {
                // ac = location of RIs in row.
                ac = tmploc[i];

                // Initialize input array index.
                r = k;

                // For each column in row.
                for (j = 0; j < Cmux; j++)
                {
                    // If symbol is not an RI, then.
                    if ((ac & 1) == 0)
                    {
                        // Copy input to output array.
                        // Two 8-bit soft decisions are copied at a time.
                        out2[m] = in[r];

                        // Increment output array index
                        m = m + 1;

                        // Exit if all symbols copied.
                        if (m == n2)
                            goto LTE_Chan_Deint_QPSK_labb;

                    }

                    // Shift RI location indicator to next column.
                    ac = ac >> 1;

                    // Advance input array index to next column.
                    r = r + Rpmux;

                }

                // Increment input array index to next row.
                k = k + 1;

            }

            LTE_Chan_Deint_QPSK_labb:

            // De-allocate temporary array.
            MemFree(tmploc, DDR_MEM);

        }

    }

    LTE_Chan_Deint_QPSK_laba:

    return;

}

/******************************************************************************
 LTE_Chan_Deint_16QAM(LTE transport channel deinterleaving function for 16QAM)
 *******************************************************************************

 LTE_Chan_Deint_16QAM implements the deinterleaving function for 16QAM encoded
 symbols as described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 (2008-12) section 5.2.2.8.  This function is used to separate a combined
 soft decision input array into the following three separate soft decision
 output arrays:

 out1    soft decisions for coded channel rank (RI) information bits
 out2    soft decisions for multiplexed coded transport block and channel
 quality information bits
 out3    soft decisions for coded HARQ ACK information bits

 Note, it is not required that all output arrays be used.  Any output array can
 be skipped by setting its corresponding array length = 0.  The functional
 calling format is defined below.

 LTE_Chan_Deint_16QAM(in, inb, out1, out2, out3,
                      n1, n2, n3, Cmux, Rmux, CP, nRI, nACK)

 in      combined/interleaved soft decision input array after de-scrambling
 inb     combined/interleaved soft decision input array before de-scrambling
 out1    coded channel rank (RI) soft decision output array
 out2    multiplexed coded transport block and channel quality soft decision
         output array
 out3    coded HARQ ACK soft decision output array
 n1      length of out1 array in bytes
 n2      length of out2 array in bytes
 n3      length of out3 array in bytes
 Cmux    Number of columns to use for interleaver
 Rmux    Number of rows to use for interleaver
 CP      Cyclic prefix (0=normal, 1=extended)
 nRI     number of RI bits encoded (1 or 2)
 nACK    number of ACK bits encoded (1 or 2)

 The LTE_Chan_Deint_16QAM function will not work properly if invalid calling
 arguments are specified.  The required calling argument rules and
 restrictions are as follows:

 1)  n1%4 = 0
 2)  n2%4 = 0
 3)  n3%4 = 0

 Before LTE_Chan_Deint_16QAM executes, the calling arguments are checked.  If
 any of the rules are violated, an error message will be printed and the
 function will return.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE transport channel deinterleaving function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param     in =    Combined/interleaved soft decision input array after de-scrambling.
 *  @param     inb =   Combined/interleaved soft decision input array before de-scrambling.
 *  @param     out1 =  Coded channel rank (RI) soft decision output array.
 *  @param     out2 =  Multiplexed coded transport block and channel quality
 *                     soft decision output array.
 *  @param     out3 =  Coded HARQ ACK soft decision output array.
 *  @param     n1 =    Length of out1 array in bytes.
 *  @param     n2 =    Length of out2 array in bytes.
 *  @param     n3 =    Length of out3 array in bytes.
 *  @param     Cmux =  Number of columns to use for interleaver.
 *  @param     Rmux =  Number of rows to use for interleaver.
 *  @param     CP =    Cyclic prefix (0=normal, 1=extended).
 *  @param     nRI =   Number of RI bits encoded (1 or 2).
 *  @param     nACK =  Number of ACK bit encoded (1 or 2).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |n1+n2          |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |inb            |               |input        |n1+n2          |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out1           |               |output       |n1             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out2           |               |output       |n2             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out2           |               |output       |n3             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n1             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n2             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n3             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Cmux           |               |input        |1              |Real          |(1:31:0)  |{9,10,11,12}   |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Rmux           |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CP             |               |input        |1              |Real          |(1:15:0)  |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nRI            |               |input        |1              |Real          |(1:15:0)  |{1,2}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nACK           |               |input        |1              |Real          |(1:15:0)  |{1,2}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------
 *  |Block Name          |LTE_Chan_Deint_16QAM           |
 *  ------------------------------------------------------
 *  |Input Stream(s)     |in, inb                        |
 *  ------------------------------------------------------
 *  |Output Stream(s)    |out1,out2,out3                 |
 *  ------------------------------------------------------
 *  |Config Parameter(s) |n1,n2,n3,Cmux,Rmux,CP,nRI,nACK |
 *  ------------------------------------------------------
 *  |Status Parameter(s) |none                           |
 *  ------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * LTE_Chan_Deint_16QAM implements the deinterleaving function for 16QAM encoded
 * symbols as described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 * (2008-12) section 5.2.2.8.  This function is used to separate a combined
 * soft decision input array into the following three separate soft decision
 * output arrays:
 *
 *  - out1    soft decisions for coded channel rank (RI) information bits
 *  - out2    soft decisions for multiplexed coded transport block and channel
 *            quality information bits
 *  - out3    soft decisions for coded HARQ ACK information bits
 *
 * Note, it is not required that all output arrays be used.  Any output array can
 * be skipped by setting its corresponding array length = 0.<BR><BR>
 *
 * The LTE_Chan_Deint_16QAM function will not work properly if invalid calling
 * arguments are specified.  The required calling argument rules and
 * restrictions are as follows:
 *
 *  -#  n1 % 4 = 0
 *  -#  n2 % 4 = 0
 *  -#  n3 % 4 = 0

 * Before LTE_Chan_Deint_16QAM executes, the calling arguments are checked.  If
 * any of the rules are violated, an error message will be printed and the
 * function will return.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.2.2.8
 *
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void LTE_Chan_Deint_16QAM(U32 in[], U32 inb[], U32 out1[], U32 out2[], U32 out3[],
                          S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP,
                          S16 nRI, S16 nACK)
{
    S32 /*ntot,*/ Rpmux, R, RRI;
    S32 r, i, j, k, c, m, ac;
    U32 ac1, ac2, ac4;
    S8 *scr, *data;
    U16 *tmploc;

#ifdef _WIN32

    // If input argument errors, then.
    if( ((n1%4) != 0) || ((n2%4) != 0) || ((n3%4) != 0) )
    {
        printf("Invalid n1, n2 or n3 error!!! nx%4 must be 0!\n");
        return;
    }

#endif

    //printf("LTE_Chan_Deint_16QAM\n");
    //printf("n1=%d, n2=%d, n3=%d, CP=%d, Cmux=%d, Rmux=%d, nRI=%d, nACK=%d\n",
    //        n1, n2, n3, CP, Cmux, Rmux, nRI, nACK);

    // Compute number of RI 16QAM (4-bit) encoded symbols.
    n1 = n1 >> 2;

    // Compute number of combined CQI and data 16QAM (4-bit) encoded symbols.
    n2 = n2 >> 2;

    // Compute number of HARQ ACK 16QAM (4-bit) encoded symbols
    n3 = n3 >> 2;

    // Rpmux = number of 16QAM symbol rows in de-intereleaver buffer;
    Rpmux = Rmux >> 2;

    // ntot = total number of 16QAM symbols in de-interleaver buffer.
    //ntot = Rpmux * Cmux;

    //printf("n1=%d, n2=%d, n3=%d, ntot=%d, Rpmux=%d\n",n1, n2, n3, ntot, Rpmux);

    // If there are HARQ ACK soft decisions to be extracted, then.
    if (n3 > 0)
    {

        // Initialize current symbol row to last row.
        r = Rpmux - 1;

        // Initialize ACK symbol extraction table index.
        j = 0;

        // Initialize ACK output array index.
        k = 0;

        // For each 4-bit 16-QAM HARQ ACK symbol to be extracted
        for (i = 0; i < n3; i++)
        {
            // c = column of HARQ ACK symbol.
            c = ColSetACK[CP][j];

            // ac = input array index for HARQ ACK symbol.
            ac = r + (c * Rpmux);

            // Read 4 8-bit soft decisions after de-scrambling.
            ac4 = in[ac];

            // Read 4 8-bit soft-decisions before de-scrambling.
            ac2 = inb[ac];
            scr = (S8*) &ac2;
            data = (S8*) &ac4;
            for(m=2; m<4; m++)
            {
                if(scr[m] != 0) data[m] = -data[m];
            }

            // If 1 ACK bit, then 16-QAM format is data, y, x, x.
            if(nACK == 1)
            {
                // If sign bit changed then need to invert inb[] 2nd byte.
                if(scr[0] != 0)
                {
                    data[1] = -data[1];
                    if(scr[1] != 0)
                    {
                        data[1] = -data[1];
                    }
            }
            else
            {
                    if(scr[1] != 0)
                    {
                        data[1] = -data[1];
                    }
            }
            }
            ac1 = ac4;      // data, y, x, x
            // Copy 4-bit 16-QAM HARQ ACK symbol into output array.
            // Four 8-bit soft decisions are copied at a time.
            out3[k] = ac1;

            // Clear HARQ ACK soft decisions in input array.
//            in[ac] = 0;

            // Increment HARQ ACK output array index;
            k = k + 1;

            // Advance ACK symbol extraction table index;
            j = (j + 1) & 3;

            // Decrement row after 4 symbols have been extracted for row.
            if (j == 0)
                r = r - 1;

        }

    }

    // If there are RI soft decisions to be extracted, then.
    if (n1 > 0)
    {

        // Initialize current symbol row to last row.
        r = Rpmux - 1;

        // Initialize RI symbol extraction table index.
        j = 0;

        // Initialize RI output array index.
        k = 0;

        // For each 4-bit RI symbol to be extracted.
        for (i = 0; i < n1; i++)
        {
            // c = column of RI symbol.
            c = ColSetRI[CP][j];

            // ac = input array index for RI symbol.
            ac = r + (c * Rpmux);

            // ac = input array index for HARQ ACK symbol.
            ac = r + (c * Rpmux);

            // Read 4 8-bit soft decisions after de-scrambling.
            ac4 = in[ac];

            // Read 4 8-bit soft-decisions before de-scrambling.
            ac2 = inb[ac];
            scr = (S8*) &ac2;
            data = (S8*) &ac4;
            for(m=2; m<4; m++)
            {
                if(scr[m] != 0) data[m] = -data[m];
            }

            // If 1 RI bit, then 16-QAM format is data, y, x, x.
            if(nRI == 1)
            {
                // If sign bit changed then need to invert inb[] 2nd byte.
                if(scr[0] != 0)
                {
                    data[1] = -data[1];
                    if(scr[1] != 0)
                    {
                        data[1] = -data[1];
                    }
            }
            else
            {
                    if(scr[1] != 0)
                    {
                        data[1] = -data[1];
                    }
            }
            }
            ac1 = ac4;      // data, y, x, x

            // Copy 4-bit RI symbol into output array.
            // Four 8-bit soft decisions are copied at a time.
            out1[k] = ac1;

            // Increment RI output array index;
            k = k + 1;

            // Advance RI symbol extraction table index;
            j = (j + 1) & 3;

            // Decrement row after 4 symbols have been extracted for row.
            if (j == 0)
                r = r - 1;

        }

    }

    // If there are multiplexed coded transport block and channel quality
    // soft decisions to be extracted, then.
    if (n2 > 0)
    {

        // Compute number of rows containing RI symbols.
        RRI = (n1 + 3) >> 2;

        // Compute number of rows only containing multiplexed data and channel
        // quality symbols;
        R = Rpmux - RRI;

        //printf("RRI=%d, R=%d\n",RRI, R);

        // Initialize input array index.
        k = 0;

        // Initialize output array index.
        m = 0;

        //  For each row in de-interleaver buffer.
        for (i = 0; i < R; i++)
        {

            // Initialize input array index.
            r = k;

            // For each column in row.
            for (j = 0; j < Cmux; j++)
            {
                // Copy input to output array.
                // Four 8-bit soft decisions are copied at a time.
                out2[m] = in[r];

                // Increment output array index
                m = m + 1;

                // Exit if all symbols copied.
                if (m == n2)
                    goto LTE_Chan_Deint_16QAM_laba;

                // Advance input array index to next column.
                r = r + Rpmux;

            }

            // Increment input array index to next row.
            k = k + 1;

        }

        // If there are rows with RI symbols, then.
        if (RRI > 0)
        {

            // Allocate temporary array to indicate location of RIs.
            tmploc = (U16*) MemAlloc(RRI * sizeof(U16), DDR1_NCNB_MEM, 0);

            // Clear temporary array elements.
            for (i = 0; i < RRI; i++)
            {
                tmploc[i] = 0;
            }

            // Initialize RI row to last one;
            r = RRI - 1;

            // Initialize RI column table index.
            j = 0;

            // For each RI symbol in row.
            for (i = 0; i < n1; i++)
            {
                // c = column of RI symbol.
                c = ColSetRI[CP][j];

                // Compute bit in correct position.
                ac = (1 << c);

                // Update RI location array.
                tmploc[r] = tmploc[r] + (U16) ac;

                // Advance RI symbol column table index.
                j = (j + 1) & 3;

                // Decrement row every 4th RI symbol.
                if (j == 0)
                    r = r - 1;

            }

            // For each row in de-interleaver buffer with RIs.
            for (i = 0; i < RRI; i++)
            {
                // ac = location of RIs in row.
                ac = tmploc[i];

                // Initialize input array index.
                r = k;

                // For each column in row.
                for (j = 0; j < Cmux; j++)
                {
                    // If symbol is not an RI, then.
                    if ((ac & 1) == 0)
                    {
                        // Copy input to output array.
                        // Four 8-bit soft decisions are copied at a time.
                        out2[m] = in[r];

                        // Increment output array index
                        m = m + 1;

                        // Exit if all symbols copied.
                        if (m == n2)
                            goto LTE_Chan_Deint_16QAM_labb;

                    }

                    // Shift RI location indicator to next column.
                    ac = ac >> 1;

                    // Advance input array index to next column.
                    r = r + Rpmux;

                }

                // Increment input array index to next row.
                k = k + 1;

            }

            LTE_Chan_Deint_16QAM_labb:

            // De-allocate temporary array.
            MemFree(tmploc, DDR1_NCNB_MEM);

        }

    }

    LTE_Chan_Deint_16QAM_laba:

    return;

}

/******************************************************************************
 LTE_Chan_Deint_64QAM(LTE transport channel deinterleaving function for 64QAM)
 *******************************************************************************

 LTE_Chan_Deint_64QAM implements the deinterleaving function for 64QAM encoded
 symbols as described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 (2008-12) section 5.2.2.8.  This function is used to separate a combined
 soft decision input array into the following three separate soft decision
 output arrays:

 out1    soft decisions for coded channel rank (RI) information bits
 out2    soft decisions for multiplexed coded transport block and channel
         quality information bits
 out3    soft decisions for coded HARQ ACK information bits

 Note, it is not required that all output arrays be used.  Any output array can
 be skipped by setting its corresponding array length = 0.  The functional
 calling format is defined below.

 LTE_Chan_Deint_64QAM(in, inb, out1, out2, out3,
                      n1, n2, n3, Cmux, Rmux, CP, nRI, nACK)

 in      combined/interleaved soft decision input array after de-scrambling
 inb     combined/interleaved soft decision input array before de-scrambling
 out1    coded channel rank (RI) soft decision output array
 out2    multiplexed coded transport block and channel quality soft decision
         output array
 out3    coded HARQ ACK soft decision output array
 n1      length of out1 array in bytes
 n2      length of out2 array in bytes
 n3      length of out3 array in bytes
 Cmux    number of columns to use for interleaver
 Rmux    number of rows to use for interleaver
 CP      nyclic prefix (0=normal, 1=extended)
 nRI     number of encoded RI bits (1 or 2)
 nACK    number of encoded ACK bits (1 or 2)

 The LTE_Chan_Deint_64QAM function will not work properly if invalid calling
 arguments are specified.  The required calling argument rules and
 restrictions are as follows:

 1)  n1%6 = 0
 2)  n2%6 = 0
 3)  n3%6 = 0

 Before LTE_Chan_Deint_64QAM executes, the calling arguments are checked.  If
 any of the rules are violated, an error message will be printed and the
 function will return.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE transport channel deinterleaving function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param     in =    Combined/interleaved soft decision input array after de-scrambling.
 *  @param     inb =   Combined/interleaved soft decision input array before de-scrambling.
 *  @param     out1 =  Coded channel rank (RI) soft decision output array.
 *  @param     out2 =  Multiplexed coded transport block and channel quality
 *                     soft decision output array.
 *  @param     out3 =  Coded HARQ ACK soft decision output array.
 *  @param     n1 =    Length of out1 array in bytes.
 *  @param     n2 =    Length of out2 array in bytes.
 *  @param     n3 =    Length of out3 array in bytes.
 *  @param     Cmux =  Number of columns to use for interleaver.
 *  @param     Rmux =  Number of rows to use for interleaver.
 *  @param     CP =    Cyclic prefix (0=normal, 1=extended).
 *  @param     nRI=    Number of encoded RI bits (1 or 2).
 *  @param     nACK=   Number of encoded ACK bits (1 or 2).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |n1+n2          |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |inb            |               |input        |n1+n2          |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out1           |               |output       |n1             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out2           |               |output       |n2             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out2           |               |output       |n3             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n1             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n2             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n3             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Cmux           |               |input        |1              |Real          |(1:15:0)  |{9,10,11,12}   |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Rmux           |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CP             |               |input        |1              |Real          |(1:15:0)  |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nRI            |               |input        |1              |Real          |(1:15:0)  |{1,2}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nACK           |               |input        |1              |Real          |(1:15:0)  |{1,2}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------
 *  |Block Name          |LTE_Chan_Deint_64QAM           |
 *  ------------------------------------------------------
 *  |Input Stream(s)     |in, inb                        |
 *  ------------------------------------------------------
 *  |Output Stream(s)    |out1,out2,out3                 |
 *  ------------------------------------------------------
 *  |Config Parameter(s) |n1,n2,n3,Cmux,Rmux,CP,nRI,nACK |
 *  ------------------------------------------------------
 *  |Status Parameter(s) |none                           |
 *  ------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * LTE_Chan_Deint_64QAM implements the deinterleaving function for 64QAM encoded
 * symbols as described in the 3GPP (LTE) standard document TS 36.212 V8.5.0
 * (2008-12) section 5.2.2.8.  This function is used to separate a combined
 * soft decision input array into the following three separate soft decision
 * output arrays:
 *
 *  - out1    soft decisions for coded channel rank (RI) information bits
 *  - out2    soft decisions for multiplexed coded transport block and channel
 *            quality information bits
 *  - out3    soft decisions for coded HARQ ACK information bits
 *
 * Note, it is not required that all output arrays be used.  Any output array can
 * be skipped by setting its corresponding array length = 0.<BR><BR>
 *
 * The LTE_Chan_Deint_64QAM function will not work properly if invalid calling
 * arguments are specified.  The required calling argument rules and
 * restrictions are as follows:
 *
 *  -#  n1 % 6 = 0
 *  -#  n2 % 6 = 0
 *  -#  n3 % 6 = 0

 * Before LTE_Chan_Deint_64QAM executes, the calling arguments are checked.  If
 * any of the rules are violated, an error message will be printed and the
 * function will return.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.2.2.8
 *
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void LTE_Chan_Deint_64QAM(U16 in[], U16 inb[], U16 out1[], U16 out2[], U16 out3[],
                          S32 n1, S32 n2, S32 n3, S32 Cmux, S32 Rmux, S16 CP,
                          S16 nRI, S16 nACK)
{

    S32 /*ntot, */Rpmux, R, RRI, Rpmux3, n23;
    S32 r, i, j, k, c, m, ac;
    U32 ac1, ac2, ac3;
    U16 *tmploc;

#ifdef _WIN32

    // If input argument errors, then.
    if( ((n1%6) != 0) || ((n2%6) != 0) || ((n3%6) != 0) )
    {
        printf("Invalid n1, n2 or n3 error!!! nx%2 must be 0!\n");
        return;
    }

#endif

    //printf("LTE_Chan_Deint_64QAM\n");
    //printf("n1=%d, n2=%d, n3=%d, CP=%d, Cmux=%d, Rmux=%d, nRI=%d, nACK=%d\n",
    //        n1, n2, n3, CP, Cmux, Rmux, nRI, nACK);

    // Compute number of RI 64QAM (6-bit) encoded symbols.
    n1 = n1 / 6;

    // Compute number of combined CQI and data 64QAM (6-bit) encoded symbols.
    n2 = n2 / 6;

    // Compute number of HARQ ACK 64QAM (6-bit) encoded symbols
    n3 = n3 / 6;

    // Rpmux = number of 64QAM symbol rows in de-interleaver buffer.
    Rpmux = Rmux / 6;

    // ntot = total number of 64QAM symbols in de-interleaver buffer.
    //ntot = Rpmux * Cmux;

    // Compute 3*Rpmux.
    Rpmux3 = 3 * Rpmux;

    // Compute 3*n2.
    n23 = 3 * n2;

    //printf("n1=%d, n2=%d, n3=%d, ntot=%d, Rpmux=%d\n",n1, n2, n3, ntot, Rpmux);

    // If there are HARQ ACK soft decisions to be extracted, then.
    if (n3 > 0)
    {

        // Initialize current symbol row to last row.
        r = Rpmux3 - 3;

        // Initialize ACK symbol extraction table index.
        j = 0;

        // Initialize ACK output array index.
        k = 0;

        // For each 2-bit HARQ ACK symbol to be extracted
        for (i = 0; i < n3; i++)
        {
            // c = column of HARQ ACK symbol.
            c = ColSetACK[CP][j];

            // ac = input array index for HARQ ACK symbol.
            ac = r + (c * Rpmux3);

            // If one encoded ACK bit, then format is: data, y, x, x, x, x.
            if(nACK == 1)
            {

                // Read 2-bit QPSK ACK soft decisions after descrambling.
                ac1 = in[ac];

                // Read 2-bit QPSK ACK soft decisions before descrambling.
                ac2 = inb[ac];

                // Need to determine if in[] 1st byte sign bit changed.
                ac3 = ac1 ^ ac2;
                ac3 = ac3 & 0x80;

                // If sign bit changed then need to invert inb[] 2nd byte.
                if(ac3 != 0) ac2 = (~ac2) + 0x100;

                // Combine in[] 1st byte and inb[] 2nd byte and copy into output array.
                ac1 = ac1 & 0x00ff;     // data
                ac2 = ac2 & 0xff00;     // y
                ac1 = ac1 | ac2;        // data, y
                out3[k] = (U16) ac1;

            }
            // Else two encoded ACK bits, format is: data, data, x, x, x, x.
            else
            {
                // Copy 2-bit HARQ ACK symbol into output array.
                // two 8-bit soft decisions are copied at a time.
                out3[k] = in[ac];               // data, data
            }

            out3[k + 1] = inb[ac + 1];      // x, x
            out3[k + 2] = inb[ac + 2];      // x, x

            // Clear HARQ ACK soft decisions in input array.
            in[ac] = 0;
            in[ac + 1] = 0;
            in[ac + 2] = 0;

            // Increment HARQ ACK output array index;
            k = k + 3;

            // Advance ACK symbol extraction table index;
            j = (j + 1) & 3;

            // Decrement row after 4 symbols have been extracted for row.
            if (j == 0)
                r = r - 3;

        }

    }

    // If there are RI soft decisions to be extracted, then.
    if (n1 > 0)
    {

        // Initialize current symbol row to last row.
        r = Rpmux3 - 3;

        // Initialize RI symbol extraction table index.
        j = 0;

        // Initialize RI output array index.
        k = 0;

        // For each 2-bit RI symbol to be extracted.
        for (i = 0; i < n1; i++)
        {
            // c = column of RI symbol.
            c = ColSetRI[CP][j];

            // ac = input array index for RI symbol.
            ac = r + (c * Rpmux3);

            // If one encoded RI bit, then format is: data, y, x, x, x, x.
            if(nRI == 1)
            {

                // Read 2-bit QPSK ACK soft decisions after descrambling.
                ac1 = in[ac];

                // Read 2-bit QPSK ACK soft decisions before descrambling.
                ac2 = inb[ac];

                // Need to determine if in[] 1st byte sign bit changed.
                ac3 = ac1 ^ ac2;
                ac3 = ac3 & 0x80;

                // If sign bit changed then need to invert inb[] 2nd byte.
                if(ac3 != 0) ac2 = (~ac2) + 0x100;

                // Combine in[] 1st byte and inb[] 2nd byte and copy into output array.
                ac1 = ac1 & 0x00ff;     // data
                ac2 = ac2 & 0xff00;     // y
                ac1 = ac1 | ac2;        // data, y
                out1[k] = (U16) ac1;

            }
            // Else two encoded ACK bits, format is: data, data, x, x, x, x.
            else
            {

                // Copy 2-bit HARQ ACK symbol into output array.
                // two 8-bit soft decisions are copied at a time.
                out1[k] = in[ac];               // data, data

            }

            out1[k + 1] = inb[ac + 1];
            out1[k + 2] = inb[ac + 2];

            // Increment RI output array index;
            k = k + 3;

            // Advance RI symbol extraction table index;
            j = (j + 1) & 3;

            // Decrement row after 4 symbols have been extracted for row.
            if (j == 0)
                r = r - 3;

        }

    }

    // If there are multiplexed coded transport block and channel quality
    // soft decisions to be extracted, then.
    if (n2 > 0)
    {

        // Compute number of rows containing RI symbols.
        RRI = (n1 + 3) >> 2;

        // Compute number of rows only containing multiplexed data and channel
        // quality symbols;
        R = Rpmux - RRI;

        //printf("RRI=%d, R=%d\n",RRI, R);

        // Initialize input array index.
        k = 0;

        // Initialize output array index.
        m = 0;

        //  For each row in de-interleaver buffer.
        for (i = 0; i < R; i++)
        {

            // Initialize input array index.
            r = k;

            // For each column in row.
            for (j = 0; j < Cmux; j++)
            {
                // Copy input to output array.
                // Six 8-bit soft decisions are copied at a time.
                out2[m] = in[r];
                out2[m + 1] = in[r + 1];
                out2[m + 2] = in[r + 2];

                // Increment output array index.
                m = m + 3;

                // Exit if all symbols copied.
                if (m == n23)
                    goto LTE_Chan_Deint_64QAM_laba;

                // Advance input array index to next column.
                r = r + Rpmux3;

            }

            // Increment input array index to next row.
            k = k + 3;

        }

        // If there are rows with RI symbols, then.
        if (RRI > 0)
        {

            // Allocate temporary array to indicate location of RIs.
            tmploc = (U16*) MemAlloc(RRI * sizeof(U16), DDR_MEM, 0);

            // Clear temporary array elements.
            for (i = 0; i < RRI; i++)
            {
                tmploc[i] = 0;
            }

            // Initialize RI row to last one;
            r = RRI - 1;

            // Initialize RI column table index.
            j = 0;

            // For each RI symbol in row.
            for (i = 0; i < n1; i++)
            {
                // c = column of RI symbol.
                c = ColSetRI[CP][j];

                // Compute bit in correct position.
                ac = (1 << c);

                // Update RI location array.
                tmploc[r] = tmploc[r] + (U16) ac;

                // Advance RI symbol column table index.
                j = (j + 1) & 3;

                // Decrement row every 4th RI symbol.
                if (j == 0)
                    r = r - 1;

            }

            // For each row in de-interleaver buffer with RIs.
            for (i = 0; i < RRI; i++)
            {
                // ac = location of RIs in row.
                ac = tmploc[i];

                // Initialize input array index.
                r = k;

                // For each column in row.
                for (j = 0; j < Cmux; j++)
                {
                    // If symbol is not an RI, then.
                    if ((ac & 1) == 0)
                    {
                        // Copy input to output array.
                        // Six 8-bit soft decisions are copied at a time.
                        out2[m] = in[r];
                        out2[m + 1] = in[r + 1];
                        out2[m + 2] = in[r + 2];

                        // Increment output array index
                        m = m + 3;

                        // Exit if all symbols copied.
                        if (m == n23)
                            goto LTE_Chan_Deint_64QAM_labb;

                    }

                    // Shift RI location indicator to next column.
                    ac = ac >> 1;

                    // Advance input array index to next column.
                    r = r + Rpmux3;

                }

                // Increment input array index to next row.
                k = k + 3;

            }

            LTE_Chan_Deint_64QAM_labb:

            // De-allocate temporary array.
            MemFree(tmploc, DDR_MEM);

        }

    }

    LTE_Chan_Deint_64QAM_laba:

    return;

}

/******************************************************************************
 ACK_RI_Encoder (LTE 1 or 2 bit HARQ ACK or RI encoder for PUSCH)
 *******************************************************************************

 Function ACK_RI_Encoder implements the encoder for the 1-bit or 2-bit ACK
 (HARQ acknowledgement) and RI (rank indicator) transmitted on the PUSCH
 (Physical Uplink Shared Channel).  For the LTE PUSCH both HARQ acknowledgement
 and rank indicator bit fields are transmitted.  These bit fields can be either
 1 or 2 bits.  The encoding of these bit fields depends on the length and the
 modulation type and is described in the LTE standard document 3GPP TS 36.212
 Section 5.2.2.  The encoding as defined in the standard is shown in the tables
 below.

 ---------------------------
 -----  1-Bit Fields   -----
 ---------------------------
 Qm      Encoded bit vectors
 ---------------------------
 2       [A y]
 4       [A y x x]
 6       [A y x x x x]
 ---------------------------

 ---------------------------------------------
 -----           2-Bit Fields            -----
 ---------------------------------------------
 Qm      Encoded bit vectors
 ---------------------------------------------
 2       [A B C A B C]
 4       [A B x x C A x x B C x x]
 6       [A B x x x x C A x x x x B C x x x x]
 ---------------------------------------------

 Where:  Qm =    Number of bits in modulation vector
 (QPSK:2, 16QAM:4, 64QAM:6)
 A =    1st bit-field bit
 B =    2nd bit-field bit
 C =    Computed parity bit = A xor B
 x,y =  Placeholders used to generate random bits to maximize
 Euclidean distance between modulation vectors

 From the tables it can be seen that for the 1-bit case only one Qm-bit
 modulation vector is used and for the 2-bit case three Qm-bit vectors are
 used.  The encoder outputs are stored in array out[].  Array out[] elements are
 stored as bytes where the lsb (bit0) of each byte is the actual data.  Bits 1-2
 of each input byte are used to indicate the type of output as shown below.

 ---------------------------------------------------------------------------
 Bit 2 1     Description
 ---------------------------------------------------------------------------
 0 0:    Output is just normal data.
 0 1:    Output is a "X" placeholder used to scramble RI and ACK bits.
 1 0:    Output is a "Y" placeholder used to scramble RI and ACK bits.
 ---------------------------------------------------------------------------

 The organization of the out[] array elements is shown below.

 ---------------------------
 -----  1-Bit Fields   -----
 ---------------------------
 Qm      Encoded bit vectors
 ---------------------------
 2       [A y]
 4       [A y x x]
 6       [A y x x x x]
 ---------------------------
 out[]    0 1 2 3 4 5
 ---------------------------

 -----------------------------------------------------
 -----               2-Bit Fields                -----
 -----------------------------------------------------
 Qm      Encoded bit vectors
 -----------------------------------------------------
 2       [A B C A B C]
 4       [A B x x C A x x B C x x]
 6       [A B x x x x C A x x x  x  B  C  x  x  x  x ]
 -----------------------------------------------------
 out[]    0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
 -----------------------------------------------------

 The functional calling format is defined below.

 ACK_RI_Encoder(in, out, Qm, nin)

 Where:  in =    Input byte.  Bit7 = A (1st bit).
 Bit6 = B (2nd bit) if there is one.
 out =   Output byte array containing encoded bits.
 Bit0 of each byte is actual encoded bit.  Bits2-1 indicate
 type of output (00:normal, 01:x placeholder,
 10:y placeholder)
 Qm =    Number of modulation vector bits (QPSK:2, 16QAM:4, 64QAM:6).
 nin =   Number of input bits (1 or 2).

 It is important to note that ACK_RI_Encoder must be called twice.  Once to
 encode the ACK bits and once to encode the RI bits.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE PUSCH ACK or RI bit encoder function.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param  in =    Input byte array.  Bit7 = A (1st bit).
 *                  Bit6 = B (2nd bit) if there is one.
 *  @param  out =   Output byte array containing encoded bits.
 *                  Bit0 of each byte is actual encoded bit.  Bits2-1 indicate
 *                  type of output (00:normal, 01:x placeholder,
 *                  10:y placeholder)
 *  @param  Qm =    Number of modulation vector bits (QPSK:2, 16QAM:4, 64QAM:6).
 *  @param  nin =   Number of input bits (1 or 2).
 *  @return         None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |1              |pReal         |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |18 maximum     |pReal         |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(1:15:0)  |{2, 4, 6}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:15:0)  |{1, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ACK_RI_Encoder              |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |Qm, nin                     |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function ACK_RI_Encoder implements an encoder for the 1-bit or 2-bit ACK
 * or RI fields transmitted on the LTE PUSCH as described in the 3GPP
 * (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.2.2.
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void ACK_RI_Encoder(U8 in[], U8 out[], S16 Qm, S16 nin)
{

    // Define "x" placeholder value
#define     P_X     2

    // Define "y" placeholder value
#define     P_Y     4

    U8 A, B, C;
    S16 i;

    // Compute 1st input bit (A)
    A = (in[0] >> 7) & 1;

    // If only 1 input bit, then
    if (nin == 1)
    {

        // 1st output bit = A
        out[0] = A;

        // 2nd output bit = "y" placeholder
        out[1] = P_Y;

        // For remaining output bits, use "x" placeholder
        for (i = 0; i < Qm - 2; i++)
        {
            out[i + 2] = P_X;
        }

        //printf("ACK_RI_Encoder in=%x, Qm=%d, nin=%d\n",in[0], Qm, nin);
        //for(i = 0; i < Qm; i++)
        //    printf("out[%d] = %x\n",i, out[i]);

        return;

    }

    // Compute 2nd input bit (B)
    B = (in[0] >> 6) & 1;

    // Compute parity bit (C)
    C = A ^ B;

    // For out[2] through out[3*Qm-1]
    for (i = 2; i < 3 * Qm; i++)
    {
        // Initialize output to "x" placeholder
        out[i] = P_X;
    }

    // Output bits for 1st vector
    out[0] = A;
    out[1] = B;

    // Output bits for 2nd vector
    out[Qm] = C;
    out[Qm + 1] = A;

    // Output bits for 3rd vector
    out[Qm + Qm] = B;
    out[Qm + Qm + 1] = C;

    //printf("ACK_RI_Encoder in=%x, Qm=%d, nin=%d\n",in[0], Qm, nin);
    //for(i = 0; i < 3*Qm; i++)
    //    printf("out[%d] = %x\n",i, out[i]);

    return;

}

/******************************************************************************
 FSCALEB (BYTE ARRAY FRACTIONAL SCALING FUNCTION)
 ******************************************************************************

 FSCALEB generates an output byte array which is the product of an input byte
 array and a fractional scale factor.  The calling parameters are defined below.

 in     Input array pointer
 out        Output array pointer
 n      Number of array elements
 frc        Fractional scale factor

 *****************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Byte array fractional scaling function.
 *
 *  <1> Hardware Resource:
 *  ARM or CEVA
 *
 *  @param     in =     Input array pointer.
 *  @param     out =    Output array pointer.
 *  @param     n =      Number of array elements.
 *  @param     frc =    Fractional scale factor.
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |n              |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |n              |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |scf            |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |FSCALEB                     |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n, frc                      |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * FSCALEB generates an output byte array which is the product of an input byte
 * array and a fractional scale factor.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 *
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void FSCALEB(S8 in[], S8 out[], S32 n, S16 frc)
{
    S32 i, ac;

    for (i = 0; i < n; i++)
    {
        ac = in[i] * frc;
        ac = ac >> 15;
        if (ac > 127)
        {
            ac = 127;
        }
        if (ac < -127)
        {
            ac = -127;
        }
        out[i] = (S8) (ac);
    }
    return;
}

#if 0       // No longer used

/******************************************************************************
 LTE_Data_Cont_Demux (LTE transport channel data and control de-multiplexing
 function)
 *******************************************************************************

 LTE_Data_Cont_Demux implements the data and control de-multiplexing function
 described in the 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12)
 section 5.2.2.7.  This function is used to separate a combined soft-decision
 input array into the following two separate output soft-decision arrays:

 out1     soft decisions for coded channel quality information bits
 out2     soft decisions for coded transport block bits

 The functional calling format is defined below.

 LTE_Data_Cont_Demax(in, out1, out2, n1, n2)

 in      combined soft decision input array
 out1    coded channel quality soft decision output array
 out2    coded transport block soft decision output array
 n1      length of out1 array
 n2      length of out2 array

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE transport channel data and control de-multiplexing function.
 *
 *  <1> Hardware Resource:
 *  ARM, CEVA or MDMA
 *
 *  @param     in =    Combined soft decision input array.
 *  @param     out1 =  Coded channel quality soft decision output array.
 *  @param     out2 =  Coded transport block soft decision output array.
 *  @param     n1 =    Length of out1 array.
 *  @param     n2 =    Length of out2 array.
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |n1+n2          |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out1           |               |output       |n1             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out2           |               |output       |n2             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n1             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n2             |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LTE_Data_Cont_Demux         |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out1,out2                   |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n1,n2                       |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * LTE_Data_Cont_Demux implements the data and control de-multiplexing function
 * described in the 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section
 * 5.2.2.7.  This function is used to separate a combined soft-decision input
 * array into the following two separate output soft-decision arrays:
 *  - out1     soft decisions for coded channel quality information bits
 *  - out2     soft decisions for coded transport block bits
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard document TS 36.212 V8.5.0 (2008-12) section 5.2.2.7.
 *
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void LTE_Data_Cont_Demux(S8 in[], S8 out1[], S8 out2[], S32 n1, S32 n2)
{

    S32 i, j;

    // Initialize input array index
    j = 0;

    // For each out1 array soft decision
    for (i = 0; i < n1; i++)
    {
        // Copy input into output array
        out1[i] = in[j];
        // Increment input array index
        j = j + 1;
    }

    // For each out2 array soft decision
    for (i = 0; i < n2; i++)
    {
        // Copy input into output array
        out2[i] = in[j];
        // Increment input array index
        j = j + 1;
    }

    return;

}

/******************************************************************************

 Description:
 ------------

 Function lte_decode_20_13 implements the decoding function described in the
 LTE standard (3GPP TS 36.212 V8.6.0 Section 5.2.3.3).  The decoding process
 uses 20 8-bit soft decisions and generates 13 output bits.  The inputs are 20
 8-bit 2's complement soft decisions derived from either the BPSK or QPSK
 receiver.  The 13 encoder output bits are packed into a 16-bit output word
 (msb first, lsb last).  The functional calling format is shown below.


 lte_decode_20_13(in, out)

 Where:  in = 20 byte input array containing 20 2's complement soft
 decisions.
 out = output containing packed decoder output bits (bit15 = 1st
 data bit, bit3 = 13th data bit).

 Execution on ARM:
 -----------------

 Originally, the Reed-Muller decoding function was supposed to be implemented
 in dedicated hardware.  However, for some reason, this did not happen.  The
 current plan is to execute the lte_decode_20_13 function on the lower ARM.
 500 lower ARM mips have been reserved for this purpose.  Although written in
 C, the lte_decode_20_13 function is written in such a way that almost every
 C code line should correspond to one ARM instruction.  This way, it is very
 easy to estimate the execution time assuming that the function is written in
 ARM assembly code.

 An estimate of the execution time for lte_decode_20_13, implemented in
 assembly code and running on the lower ARM is shown below.

 texe = 394 + 39totequ

 Where:  texe = total execution time in cycles.
 totequ = total number of equations specified in dectab table.

 Unfortunately, this execution time is only valid if all of the data and tables
 are resident in the ARM's data cache.  Most likely the data and tables will
 reside in the DDR memory.  Therefore, additional time will be required to
 load the data and tables from DDR memory into the cache.  For our system the
 DDR memory can be accessed 8 bytes at a time.  In general, the time to load
 the data cache from DDR is shown below.

 tload = 10 + (n-8)/8

 Where:  tload = time to load data cache in cycles.
 n = number of bytes in DDR memory to be read/written.

 The time required to load the data cache for the required lte_decode_20_13
 data and tables is shown below.

 in:     10 + (20-8)/8
 out:    10
 dectab: 10 + (totequ*6 + 13 - 8)/8
 enctab: 10 + (4*13 - 8)/8

 total:  49 + .75totequ

 The combined execution and cache loading time is shown below.

 tcom = 443 + 39.75totequ

 Where:  tcom = total combined execution time in cycles

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Reed-Muller (RM)(20, 13) soft-decision decoder.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param     in =    20 byte input array containing 20 2's complement soft
 *                     decisions.
 *  @param     out =   Output containing packed decoder output bits (bit15 =
 *                     1st data bit, bit3 = 13th data bit).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |20             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |1              |pReal         |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |lte_decode_20_13            |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function lte_decode_20_13 implements the decoding function described in the
 * LTE standard (3GPP TS 36.212 V8.6.0 Section 5.2.3.3).  The decoding process
 * uses 20 8-bit soft decisions and generates 13 output bits.  The inputs are 20
 * 8-bit 2's complement soft decisions derived from either the BPSK or QPSK
 * receiver.  The 13 encoder output bits are packed into a 16-bit output word
 * (msb first, lsb last).
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0 Section 5.2.3.3
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void lte_decode_20_13(S8 in[20], U16 *out)
{
    S32 ac, ac0, ac1;
    S32 i, i0, i1, i2, i3, i4, i5;
    S32 r0, r1, r2, r3, r4, r5, r6;
    S32 ind;
    U32 decbits;
    U32 u0, u1, u2;

    //*************************************************************************
    // Initialize decoding table index
    ind = 0;
    // Initialize packed decoded data bits
    decbits = 0;

    // Do for output bits 1 through 12
    for (i = 1; i < 13; i++)
    {
        // Initialize bit=0 and bit=1 confidence accumulators
        ac0 = 0;
        ac1 = 0;

        // Shift packed decoded data left 1 bit
        decbits = decbits << 1;

        // Read 6 soft decision input indices for next logic equation
        i0 = rm_20_13_dectab[ind];

        laba:

        i1 = rm_20_13_dectab[ind + 1];
        i2 = rm_20_13_dectab[ind + 2];
        i3 = rm_20_13_dectab[ind + 3];
        i4 = rm_20_13_dectab[ind + 4];
        i5 = rm_20_13_dectab[ind + 5];

        // Increment decoding table index
        ind = ind + 6;

        // Read 6 soft decisions for next logic equation
        r0 = in[i0];
        r1 = in[i1];
        r2 = in[i2];
        r3 = in[i3];
        r4 = in[i4];
        r5 = in[i5];

        // Compute confidence measure for equation using minimum soft
        // decision magnitude
        ac = r0;
        if (ac < 0)
            ac = -ac;
        r6 = ac;
        ac = r1;
        if (ac < 0)
            ac = -ac;
        if (ac < r6)
            r6 = ac;
        ac = r2;
        if (ac < 0)
            ac = -ac;
        if (ac < r6)
            r6 = ac;
        ac = r3;
        if (ac < 0)
            ac = -ac;
        if (ac < r6)
            r6 = ac;
        ac = r4;
        if (ac < 0)
            ac = -ac;
        if (ac < r6)
            r6 = ac;
        ac = r5;
        if (ac < 0)
            ac = -ac;
        if (ac < r6)
            r6 = ac;

        // Determine bit value for equation by xoring terms
        r0 = r0 ^ r1;
        r0 = r0 ^ r2;
        r0 = r0 ^ r3;
        r0 = r0 ^ r4;
        r0 = r0 ^ r5;

        ac = r0 & 0x80;
        // If bit=0, then add confidence measure to "0" confidence accumulator
        if (ac == 0)
            ac0 = ac0 + r6;
        // If bit=1, then add confidence measure to "1" confidence accumulator
        if (ac != 0)
            ac1 = ac1 + r6;
        // Read next soft decision index from decoding table
        i0 = rm_20_13_dectab[ind];
        // If still more equations (not=-1), then continue
        if (i0 >= 0)
            goto laba;
        // Increment decoding table index
        ind = ind + 1;
        // If "1" confidence > "0" confidence, then insert 1 into
        // packed decoded data bits
        ac = ac1 - ac0;
        if (ac > 0)
            decbits = decbits | 0x08;
    }

    //*************************************************************************

    // Run corresponding encoder to remove bits 1-12 from input
    // Initialize 20-bit encoder output to all 0s
    u0 = 0;
    // u1 = packed decoded data bits (1-12)
    u1 = decbits;
    // Initialize encoding table index
    ind = 1;

    // Do for each decoded data bit
    for (i = 0; i < 12; i++)
    {
        // Read next encoding table entry
        u2 = rm_20_13_enctab[ind];
        // Increment encoding table index
        ind = ind + 1;
        // Test next decoded data bit
        ac = u1 & 0x4000;
        // If  data bit = 1, then xor in corresponding encoding table element
        if (ac != 0)
            u0 = u0 ^ u2;
        // Shift data left to next data bit;
        u1 = u1 << 1;
    }

    //*************************************************************************

    // After removing bits 1-12 from input, input should be
    // either 0x00000000 or 0x000fffff.  To determine which
    // convert soft decisions into hard decisions, remove bit 1-12
    // contributions and then count remaining number of 1s.

    // Initialize #1s accumulator = 0
    ac0 = 0;
    // Initialize soft decision input array index
    ind = 0;

    // For each of 20 encoder output bits
    for (i = 0; i < 20; i++)
    {
        // ac1 = soft decision sign bit = hard data bit
        ac1 = 1;
        ac = in[ind];
        ac = ac & 0x80;
        if (ac != 0)
        {
            ac1 = 0;
        }

        // Remove bit 1-12 contribution using encoder output
        ac = u0 & 1;
        if (ac != 0)
        {
            ac1 = ac1 ^ 1;
        }

        // Update #1s accumulator
        ac0 = ac0 + ac1;
        // Increment soft decision input array index
        ind = ind + 1;
        // Shift encoder output to next bit
        u0 = u0 >> 1;
    }

    //printf("Decoder bit0 number of 1s = %d\n",ac0);

    // If more than 10 1s, then bit0 = 1
    ac = ac0 - 10;
    if (ac > 0)
    {
        decbits = decbits | 0x8000;
    }

    // Write decoded data bits to output
    *out = (U16) (decbits & 0xffff);

    return;
}

/******************************************************************************

 Description:
 ------------

 Function lte_decode_32_11 implements the decoding function described in the
 LTE standard (3GPP TS 36.212 V8.6.0 Section 5.2.2.6.4).  The decoding process
 uses 32 8-bit soft decisions and generates 11 output bits.  The inputs are 32
 8-bit 2's complement soft decisions derived from either the BPSK or QPSK
 receiver.  The 11 decoder output bits are packed into a 16-bit output word
 (msb first, lsb last).  The functional calling format is shown below.


 lte_decode_32_11(in, out)

 Where:  in = 32 byte input array containing 32 2's complement soft
 decisions.
 out = output containing packed decoder output bits (bit15 =
 1st data bit, bit5 = 11th data bit).

 Execution on ARM:
 -----------------

 Originally, the Reed-Muller decoding function was supposed to be implemented
 in dedicated hardware.  However, for some reason, this did not happen.  The
 current plan is to execute the lte_decode_32_11 function on the lower ARM.
 500 lower ARM mips have been reserved for this purpose.  Although written in
 C, the lte_decode_32_11 function is written in such a way that almost every
 C code line should correspond to one ARM instruction.  This way, it is very
 easy to estimate the execution time assuming that the function is written in
 ARM assembly code.

 An estimate of the execution time for lte_decode_32_11, implemented in
 assembly code and running on the lower ARM is shown below.

 texe = 484 + 27totequ

 Where:  texe = total execution time in cycles.
 totequ = total number of equations specified in dectab table.

 Unfortunately, this execution time is only valid if all of the data and tables
 are resident in the ARM's data cache.  Most likely the data and tables will
 reside in the DDR memory.  Therefore, additional time will be required to
 load the data and tables from DDR memory into the cache.  For our system the
 DDR memory can be accessed 8 bytes at a time.  In general, the time to load
 the data cache from DDR is shown below.

 tload = 10 + (n-8)/8

 Where:  tload = time to load data cache in cycles.
 n = number of bytes in DDR memory to be read/written.

 The time required to load the data cache for the required lte_decode_20_13
 data and tables is shown below.

 in:     10 + (32-8)/8
 out:    10
 dectab: 10 + (totequ*4 + 11 - 8)/8
 enctab: 10 + (4*11 - 8)/8

 total:  49 + .5totequ

 The combined execution and cache loading time is shown below.

 tcom = 533 + 27.5totequ

 Where:  tcom = total combined execution time in cycles

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Reed-Muller (RM)(32, 11) soft-decision decoder.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param     in =    32 byte input array containing 32 2's complement soft
 *                     decisions.
 *  @param     out =   Output containing packed decoder output bits (bit15 =
 *                     1st data bit, bit5 = 11th data bit).
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |32             |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |1              |pReal         |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |lte_decode_32_11            |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |out                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function lte_decode_32_11 implements the decoding function described in the
 * LTE standard (3GPP TS 36.212 V8.6.0 Section 5.2.2.6.4).  The decoding process
 * uses 32 8-bit soft decisions and generates 11 output bits.  The inputs are 32
 * 8-bit 2's complement soft decisions derived from either the BPSK or QPSK
 * receiver.  The 11 decoder output bits are packed into a 16-bit output word
 * (msb first, lsb last).
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0 Section 5.2.2.6.4
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void lte_decode_32_11(S8 in[32], U16 *out)
{

    S32 ac, ac0, ac1;
    S32 i, i0, i1, i2, i3;
    S32 ind;
    S32 r0, r1, r2, r3, r4;
    U32 decbits;
    U32 u0, u1, u2;

    //*************************************************************************

    // Initialize decoding table index
    ind = 0;
    // Initialize packed decoded data bits
    decbits = 0;

    // Do for output bits (2nd through 11th)
    for (i = 0; i < 10; i++)
    {
        // Initialize bit=0 and bit=1 confidence accumulators
        ac0 = 0;
        ac1 = 0;
        // Shift packed decoded data left 1 bit
        decbits = decbits << 1;
        // Read 4 soft decision input indices for next logic equation
        i0 = rm_dectab_32_11[ind];

        laba:

        i1 = rm_dectab_32_11[ind + 1];
        i2 = rm_dectab_32_11[ind + 2];
        i3 = rm_dectab_32_11[ind + 3];
        // Increment decoding table index
        ind = ind + 4;
        // Read 4 soft decisions for next logic equation
        r0 = in[i0];
        r1 = in[i1];
        r2 = in[i2];
        r3 = in[i3];
        // Compute confidence measure for equation using minimum soft decision magnitude
        ac = r0;
        if (ac < 0)
            ac = -ac;
        r4 = ac;
        ac = r1;
        if (ac < 0)
            ac = -ac;
        if (ac < r4)
            r4 = ac;
        ac = r2;
        if (ac < 0)
            ac = -ac;
        if (ac < r4)
            r4 = ac;
        ac = r3;
        if (ac < 0)
            ac = -ac;
        if (ac < r4)
            r4 = ac;

        // Determine bit value for equation by xoring terms
        r0 = r0 ^ r1;
        r0 = r0 ^ r2;
        r0 = r0 ^ r3;

        ac = r0 & 0x80;

        // If bit=0, then add confidence measure to "0" confidence accumulator
        if (ac == 0)
        {
            ac0 = ac0 + r4;
        }
        // If bit=1, then add confidence measure to "1" confidence accumulator
        if (ac != 0)
        {
            ac1 = ac1 + r4;
        }
        // Read next soft decision index from decoding table
        i0 = rm_dectab_32_11[ind];
        // If still more equations (not=-1), then continue
        if (i0 >= 0)
            goto laba;

        // Increment decoding table index
        ind = ind + 1;
        // If "1" confidence > "0" confidence, then insert 1 into
        // packed decoded data bits
        ac = ac1 - ac0;
        if (ac > 0)
        {
            decbits = decbits | 0x20;
        }
    }

    //*************************************************************************

    // Run corresponding encoder to remove 2nd through 11th bits from input
    // Initialize 32-bit encoder output to all 0s
    u0 = 0;
    // u1 = packed decoded data bits (2nd through 11th)
    u1 = decbits;
    // Initialize encoding table index
    ind = 1;

    // Do for each decoded data bit
    for (i = 0; i < 10; i++)
    {
        // Read next encoding table entry
        u2 = rm_32_11_enctab[ind];
        // Increment encoding table index
        ind = ind + 1;
        // Test next decoded data bit
        ac = u1 & 0x4000;
        // If  data bit = 1, then xor in corresponding encoding table element
        if (ac != 0)
        {
            u0 = u0 ^ u2;
        }
        // Shift data left to next data bit;
        u1 = u1 << 1;
    }

    //*************************************************************************

    // After removing 2nd through 11th bits from input, input should be
    // either 0x00000000 or 0xffffffff.  To determine which
    // convert soft decisions into hard decisions, remove bit 1-10
    // contributions and then count remaining number of 1s.

    // Initialize #1s accumulator = 0
    ac0 = 0;
    // Initialize soft decision input array index
    ind = 0;

    // For each of 32 encoder output bits
    for (i = 0; i < 32; i++)
    {
        // ac1 = soft decision sign bit = hard data bit
        ac1 = 1;
        ac = in[ind];
        ac = ac & 0x80;
        if (ac != 0)
            ac1 = 0;
        // Remove 2nd through 11th bit contribution using encoder output
        ac = u0 & 1;
        if (ac != 0)
        {
            ac1 = ac1 ^ 1;
        }
        // Update #1s accumulator
        ac0 = ac0 + ac1;
        // Increment soft decision input array index
        ind = ind + 1;
        // Shift encoder output to next bit
        u0 = u0 >> 1;
    }

    // If more than 16 1s, then bit0 = 1
    ac = ac0 - 16;
    if (ac > 0)
    {
        decbits = decbits | 0x8000;
    }

    // Write decoded data bits to output
    *out = (U16) (decbits & 0xffff);

    return;
}

#endif  //#if 0

#endif  //Not needed for Ceva
