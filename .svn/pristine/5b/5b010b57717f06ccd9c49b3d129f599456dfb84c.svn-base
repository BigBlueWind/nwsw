//-------------------------------------------------------------------------------------------
/** @file TxCoder_util.c
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.48 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/** ATbrief Compute_Rate_match_in_parms.
 *         This function computes the Rate Matching Input Parameters
 *         per 3GPP TS 36.212 Sec.5.1.4.1.
 *
 *  @param inlen is the length of the input arrays.
 *
 *  @param G_num_bits_transport Total Number of bits available for the
 *         transmission of 1 Transport Block.
 *
 *  @param rvidx Redundancy Version number for this Transport Block
 *         per 3GPP 36.212 Sec5.3.3.1.2.
 *
 *  @param NL Number of Layers
 *            = 1, for blocks mapped onto one transmission layer, or
 *            = 2, for blocks mapped onto two or four transmission layers
 *
 *  @param Qm Number of bits mapped per symbol
 *         =1 for BPSK
 *         =2 for QPSK
 *         =4 for 16-QAM
 *         =6 for 64-QAM
 *
 *  @param CBNum corresponds to the Code Block Number (0,1,...,C-1).
 *
 *  @param C_num_code_blocks is the total number of code blocks (C above).
 *
 *
 *  @return outlen Length of the Output Bit stream
 *
 *  This function computes the rate matching input parms for a code block
 *	per 3GPP TS 36.212 Sec.5.1.4.1
 *

 */

//*****************************************************************************
//**********               GLOBAL MEMORY DECLARATIONS                **********
//*****************************************************************************

// Allocate space for structure containing rate matching parameters
RateMatchParms rm;

//*****************************************************************************


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   inlen DOXYGEN_TO_DO
 *  @param   G_num_bits_transport DOXYGEN_TO_DO
 *  @param   rvidx DOXYGEN_TO_DO
 *  @param   NL DOXYGEN_TO_DO
 *  @param   Qm DOXYGEN_TO_DO
 *  @param   CBNum DOXYGEN_TO_DO
 *  @param   C_num_code_blocks DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -----------------------------------------------------------------------------------------------------------------
 *  |Name                 |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |inlen                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |G_num_bits_transport |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |rvidx                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |NL                   |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |Qm                   |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |CBNum                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |C_num_code_blocks    |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
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
PRateMatchParms Compute_Rate_match_in_parms(S16 inlen, S32 G_num_bits_transport, S16 rvidx, S16 NL,
                                            S16 Qm, S16 CBNum, S16 C_num_code_blocks)
{
    S16 temp, k0, RowTCSubblock;
    S16 gamma, Ncb;
    S32 NIR, Gdash, E_var;

    //Step (3) Bit selection and pruning [3GPP TS 36.212 v8.2.0 (2008-03)]
    //!!!!!!!!!!! need to add input parameter

    // NIR = Nsoft/(Kmimo*min(DL_HARQ, 8)), where Kmimo = 1 or 2
    // Nsoft is defined in 36.306 and depends on specified UE category
    // The largest Nsoft = 3667200
    // For now, use NIR = 3667200/8
    NIR = 458400;

    NIR = NIR / C_num_code_blocks;

    Ncb = inlen;

    if (NIR < Ncb)
        Ncb = (S16) NIR;

    RowTCSubblock = inlen / 3 / 32;
    Gdash = G_num_bits_transport / (NL * Qm);
    gamma = Gdash % C_num_code_blocks;
    if (CBNum <= C_num_code_blocks - gamma - 1)
    {
        E_var = divide_floor(Gdash, C_num_code_blocks);
        E_var = NL * Qm * E_var;
    }
    else
    {
        E_var = divide_ceil(Gdash, C_num_code_blocks);
        E_var = NL * Qm * E_var;
    }
    temp = divide_ceil(Ncb, 8*RowTCSubblock);
    k0 = RowTCSubblock * (2 * temp * rvidx + 2);
    rm.E = E_var;
    rm.k0 = k0;
    rm.Ncb = Ncb;

    return &rm;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL This function converts a byte to a bit stream.
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param bytestream Pointer to bytestream
 *  @param bitstream  Pointer to output bitstream
 *  @param length	  Length in bytes
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
 *  |bytestream[]  |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |bitstream[]   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length        |              |              |              |              |              |              |
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
 *  The Byte2BitStream function gets an input byte stream and converts it
 *  to a bit stream at the location given by the bitstream pointer for the
 *  number of input bytes specified by the length parameter
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
void Byte2BitStream(U8 bytestream[], U8 bitstream[], U16 length)
{

    U16 i;
    S16 j;
    S32 k;

    k = 0;

    for (i = 0; i < length; i++)
    {
        for (j = 7; j >= 0; j--)
        {
            bitstream[k] = (U8) ((bytestream[i] >> j) & 1);
            k++;
        }
    }

}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/** ATbrief DOX_INTERNAL BsTxCrcEncoder
 *         This function computes the CRC24A per 3GPP TS 36.212, Sec 5.1.1
 *
 *  @param in_bitstream Pointer to the input bit stream
 *  @param out_bitstream Pointer to the output bit stream
 *  @param length Length of the input bit stream
 *
 *  The BsTxCrcEncoder function computes the CRC24A checksum according to
 *  3GPP TS 36.212 Sec.5.1.1
 *  The in_bistream pointer points to a bit stream which is in the following
 *  order
 *
 *  a(0),a(1),...,a(A-1) = input bits in transport block, a(0) first in time
 *
 *  This function computes 24 CRC parity bits according to the following
 *  generator polynomial
 *
 *  gCRC24A(D) = [D^24 + D^23 + D^18 + D^17 + D^14 +D^11 + D^10 + D^7 + D^6 +
 *                D^5 + D^4 +  D^3 + D + 1]
 *
 *  The computed parity bits p(x)  are then appended to the output stream
 *  p(0),p(1),...,p(L-1) = 24 CRC parity bits
 *
 *  So the out_bitstream corresponds to:
 *
 *  b(0),b(1),...,b(A+L-1) = a(0),a(1),...,a(A-1),p(0),p(1),...,p(L-1)
 *
 *
 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   in_bitstream[] DOXYGEN_TO_DO
 *  @param   out_bitstream[] DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ------------------------------------------------------------------------------------------------------------
 *  |Name            |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ------------------------------------------------------------------------------------------------------------
 *  |in_bitstream[]  |              |              |              |              |              |              |
 *  ------------------------------------------------------------------------------------------------------------
 *  |out_bitstream[] |              |              |              |              |              |              |
 *  ------------------------------------------------------------------------------------------------------------
 *  |length          |              |              |              |              |              |              |
 *  ------------------------------------------------------------------------------------------------------------
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
void BsTxCrcEncoder(U8 in_bitstream[], U8 out_bitstream[], S32 length)
{

    S32 i, k;
    U8 ShiftReg[25];

    // Load input bitstream
    for (i = 0; i < length; i++)
    {
        out_bitstream[i] = in_bitstream[i];
    }

    // Append 24 zeros to bit sequence to initialize CRC encoder
    for (i = length; i < length + 24; i++)
    {
        out_bitstream[i] = 0;
    }

    // Load 1st 24 bits into shift register
    for (i = 0; i < 25; i++)
    {
        ShiftReg[24 - i] = in_bitstream[i];
    }

    // Calculate CRC24A
    for (k = 24; k < length + 24; k++)
    {
        //do 1 bit at a time

        //calculate xor bit combinations

        ShiftReg[23] = ShiftReg[23] ^ ShiftReg[24];
        ShiftReg[18] = ShiftReg[18] ^ ShiftReg[24];
        ShiftReg[17] = ShiftReg[17] ^ ShiftReg[24];
        ShiftReg[14] = ShiftReg[14] ^ ShiftReg[24];
        ShiftReg[11] = ShiftReg[11] ^ ShiftReg[24];
        ShiftReg[10] = ShiftReg[10] ^ ShiftReg[24];
        ShiftReg[7] = ShiftReg[7] ^ ShiftReg[24];
        ShiftReg[6] = ShiftReg[6] ^ ShiftReg[24];
        ShiftReg[5] = ShiftReg[5] ^ ShiftReg[24];
        ShiftReg[4] = ShiftReg[4] ^ ShiftReg[24];
        ShiftReg[3] = ShiftReg[3] ^ ShiftReg[24];
        ShiftReg[1] = ShiftReg[1] ^ ShiftReg[24];
        ShiftReg[0] = out_bitstream[k] ^ ShiftReg[24];

        //shift bits one place
        for (i = 24; i > 0; i--)
        {
            ShiftReg[i] = ShiftReg[i - 1];
        }

    }

    //overwrite temporary 24-zeros with the 24- bit CRC

    for (i = length; i < length + 24; i++)
    {
        out_bitstream[i] = ShiftReg[24 - i + length];
    }

}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/** ATbrief DOX_INTERNAL AddCodeBlockCRC24B
 *         This function computes the CRC24B per 3GPP TS 36.212, Sec 5.1.1
 *
 *  @param in_bitstream Pointer to the input bit stream
 *  @param out_bitstream Pointer to the output bit stream
 *  @param length Length of the input bit stream
 *
 *  The AddCodeBlockCRC24B function computes the CRC24B checksum according to
 *  3GPP TS 36.212 Sec.5.1.1
 *
 *  The in_bistream pointer points to a bit stream which is in the following
 *  order
 *
 *  a(0),a(1),...,a(A-1) = input bits in transport block, a(0) first in time
 *
 *  This function computes 24 CRC parity bits according to the following
 *  generator polynomial
 *
 *  gCRC24B(D) = [D^24 + D^23 + D^6 + D^5 + D + 1]
 *
 *  The computed parity bits p(x)  are then appended to the output stream
 *  p(0),p(1),...,p(L-1) = 24 CRC parity bits
 *
 *  So the out_bitstream corresponds to:
 *
 *  b(0),b(1),...,b(A+L-1) = a(0),a(1),...,a(A-1),p(0),p(1),...,p(L-1)
 *
 *
 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   in_bitstream[] DOXYGEN_TO_DO
 *  @param   out_bitstream[] DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ------------------------------------------------------------------------------------------------------------
 *  |Name            |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ------------------------------------------------------------------------------------------------------------
 *  |in_bitstream[]  |              |              |              |              |              |              |
 *  ------------------------------------------------------------------------------------------------------------
 *  |out_bitstream[] |              |              |              |              |              |              |
 *  ------------------------------------------------------------------------------------------------------------
 *  |length          |              |              |              |              |              |              |
 *  ------------------------------------------------------------------------------------------------------------
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
void AddCodeBlockCRC24B(U8 in_bitstream[], U8 out_bitstream[], S16 length)
{

    short i, k;
    U8 ShiftReg[25];

    // Load input bitstream
    for (i = 0; i < length; i++)
    {
        out_bitstream[i] = in_bitstream[i];
    }

    // Append 24 zeros to bit sequence to initialize CRC encoder
    for (i = length; i < length + 24; i++)
    {
        out_bitstream[i] = 0;
    }

    // Load 1st 24 bits into shift register
    for (i = 0; i < 25; i++)
    {
        ShiftReg[24 - i] = in_bitstream[i];
    }

    // Calculate CRC24A
    for (k = 24; k < length + 24; k++)
    {

        //do 1 bit at a time

        //calculate xor bit combinations

        ShiftReg[23] = ShiftReg[23] ^ ShiftReg[24];
        ShiftReg[6] = ShiftReg[6] ^ ShiftReg[24];
        ShiftReg[5] = ShiftReg[5] ^ ShiftReg[24];
        ShiftReg[1] = ShiftReg[1] ^ ShiftReg[24];
        ShiftReg[0] = out_bitstream[k] ^ ShiftReg[24];

        //shift bits one place
        for (i = 24; i > 0; i--)
        {
            ShiftReg[i] = ShiftReg[i - 1];
        }

    }

    //overwrite temporary 24-zeros with the 24- bit CRC

    for (i = length; i < length + 24; i++)
    {
        out_bitstream[i] = ShiftReg[24 - i + length];
    }

}

#ifdef _WIN32

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/** ATbrief DOX_INTERNAL BsTxCodeBlkSeg
 *         This function implements code block segmentation per 3GPP TS 36.212
 *         Sec.5.1.2
 *
 *  @param in_bitstream Pointer to input bitstream
 *  @param code_blocks Pointer to an array for the code blocks
 *  @param KIndex  Index into [3GPP TS 36.212, Table 5.1.3-3], for value of Ki
 *                 used for	 codeBlock
 *  @param FillerBits Number of filler bits <NULL> added to the start of the
 *                    first output code block
 *  @param B number of input bits
 *  @param K Number of bits in block[r]
 *  @param MM corresponds to a pointer to the Maximum Subblock Length info
 *         The actual value is written by this function
 *
 *  The function BsTxCodeBlkSeg implements the code block segmentation for
 *  LTE per 3GPP TS 36.212 Sec.5.1.2 as follows
 *
 *  If the number of bits in code block {in_bitstream} is bigger than the
 *  maximum allowed, the single sequence {in_bitstream} is segmented into
 *  multiple smaller sequences {code_blocks} and a CRC24B appended to each
 *  sequence per 3GPP TS 36.212 Sec.5.1.2.
 *
 *  For the LTE Turbo coder Z=6144 corresponds to the maximum code block
 *  size in bits including CRC
 *
 *  The input is in_bitstream
 *  in_bitstream(0),in_bitstream(1),...,in_bitstream(B-1) = input bits in
 *  code block,
 *
 *  The output code blocks are defined by the pointer to an array code_blocks
 *  as follows
 *
 *  code_blocks(0,0),code_blocks(0,1),...,code_blocks(0,K(0)-1) = output bits
 *  in code block 0
 *
 *  where K(0) = number of bits in code block number 0
 *
 *  code_blocks(1,0),code_blocks(1,1),...,code_blocks(1,K(1)-1) = output bits
 *  in code block 1
 *
 *  where K(1) = number of bits in code block number 1
 *  ...
 *
 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   in_bitstream[] DOXYGEN_TO_DO
 *  @param   **code_blocks DOXYGEN_TO_DO
 *  @param   KIndex[] DOXYGEN_TO_DO
 *  @param   *FillerBits DOXYGEN_TO_DO
 *  @param   B DOXYGEN_TO_DO
 *  @param   K[] DOXYGEN_TO_DO
 *  @param   *MM DOXYGEN_TO_DO
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
 *  |in_bitstream[]|              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |**code_blocks |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |KIndex[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*FillerBits   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |B             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |K[]           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*MM           |              |              |              |              |              |              |
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
S16 BsTxCodeBlkSeg(U8 in_bitstream[], U8 **code_blocks, S16 KIndex[],
    S16 *FillerBits, S32 B, S16 K[], S16 *MM)
{

    S16 i, CBNum, fb;
    S16 L, k;
    S16 num_code_blocks;
    S16 irow, len, deltaK;
    S16 Cplus, Kplus, Kminus, Cminus, KplusIndex;
    S32 F, Bdash, s;
    U8 *cvec;

#define Z 6144

    cvec = MemAlloc(B+2*NUM_PARITY, CRAM_MEM, 0);

    CE_CTCParamColKi =1;

    // Step 1: If B < 40, add <NULL>-valued filler bits at the beginning of the
    //block
    //Note: Since the CRC appended to the TranportBlock (to change {a} to
    //{b}) is calculate d as a division function, then it is clear, that adding
    //zeros to the beginning of {b} does not change the CRC that was appended to
    //the transport block!

    if( B < 40)
    {
        //=number of filler bits (also must get num_code_blocks=1 in this case!)
        F = 40-B;
        //=40, so block length is now considered to be 40! Filler bits will be
        //added later, in step (6).
        B = B + F;
    }

    // Step 2: Calculate number of code blocks, num_code_blocks
    if (B <= Z)
    {
        L = 0;
        //number of code blocks
        num_code_blocks = 1;
        //B-dash
        Bdash = B;
    }
    else
    {
        L = 24;

        num_code_blocks = (S16)((B+Z-L-1)/(Z-L));

        Bdash = B + (num_code_blocks*L);
    }

    // Step 3: Calculate number of bits in each code block with First
    // Segmentation Size (applicable to num_code_blocks != 0 only)
    // Kplus = min(K in table 5.1.3-3, such that num_code_blocks*K >= Bdash),
    //  i.e. Kplus = min(Ki in
    //  CA_CTCInternalInterleaverParams(CE_CTCParamColKi,:,:), such that
    //   num_code_blocks*K >= Bdash
    //
    // Note, K increases as the index of Ki down the column in the table
    // increases. So, in searching for the min(Ki...) that satisfies the
    // equation, start with row1 and increase until satisfied!

    //number of rows in the table, i.e. number of Ki terms
    len = 188;
    irow = 0;
    Kplus = CA_CTCInternalInterleaverParams[irow][CE_CTCParamColKi-1];

    while(num_code_blocks*Kplus < Bdash)
    {
        irow = irow+1; //increment pointer to check next Ki
        Kplus = CA_CTCInternalInterleaverParams[irow][CE_CTCParamColKi-1];
        if (irow > len-1)
        {
            printf(" Error in CodeBlockSegmentation():Cannot find valid Ki\n");
        }
    }
    //keep for Kminus calculation below
    KplusIndex = irow;

    //Step 4 Calculate number of code blocks with length Kplus. Also,
    //Calculate number of bits in block with Second Segmentation Size, Kminus,
    //and number of blocks with length Kminus where,
    //   Kminus = max(K in table 5.1.3-3, such that K < Kplus), i.e.
    //   Kminus = max(Ki in
    //CA_CTCInternalInterleaverParams(CE_CTCParamColKi,:,:), such that K<Kplus
    //Note, K increases as the index of Ki down the column in the table
    //increases. So, in searching for the max(Ki...) that satisfies the
    //equation, simply read the value from the table, in the row before the
    //Kplus value.

    if( num_code_blocks == 1)
    {
        //number of code blocks with length Kplus
        Cplus = 1;
        //no other code blocks!
        Kminus = 0;
        Cminus = 0;
    }
    else
    {
        //num_code_blocks > 1. i.e. num_code_blocks cannot be <=0 at this point!

        //i.e. the biggest value in the table smaller than Kplus
        Kminus = CA_CTCInternalInterleaverParams[KplusIndex-1]
        [ CE_CTCParamColKi-1];
        deltaK = Kplus - Kminus;
        Cminus = (S16)((num_code_blocks*Kplus - Bdash)/deltaK);
        //number of seqments of size Kplus
        Cplus = num_code_blocks - Cminus;
    }

    //Step 5 Start the first code block with the required number of filler bits

    fb = (S16)((Cplus * Kplus) + (Cminus * Kminus) - Bdash); //number of filler bits
    if( fb > 0)
    {
        for(i=0; i< fb; i++)
        {
            code_blocks[0][i] = 0;
        }
    }
    *FillerBits = fb;

    k = fb;
    s = 0;

    //Step 6 Now, move the bits {b} from the input code block into the
    //segmented code blocks {c}

    for( CBNum = 0; CBNum < num_code_blocks; CBNum++ )
    {
        if (CBNum < Cminus)
        {
            K[CBNum] = Kminus;
            //i.e. Kminus index
            KIndex[CBNum] = KplusIndex-1;
        }
        else
        {
            K[CBNum] = Kplus;
            KIndex[CBNum] = KplusIndex;
        }
        while (k < (K[CBNum] -L))
        {
            code_blocks[CBNum][k] = in_bitstream[s];
            k = k+1;
            s = s+1;
        }
        if (num_code_blocks > 1)
        {
            for(i=0; i< K[CBNum] -L; i++)
            {
                //move the bits from the rectangular codeBlock array to a
                //vector, for the CRC function below
                cvec[i] = code_blocks[CBNum][i];
            }
            AddCodeBlockCRC24B(cvec, cvec,(S16)(K[CBNum] - L));
            for(i=0; i< K[CBNum]; i++)
            {
                //move the final codeBlock, with appended CRC24B back into
                //the codeBlock array
                code_blocks[CBNum][i] = cvec[i];
            }
        }
        k=0;
    }

    // Get Maximum sub-block length. for printing convenience
    *MM = K[0];
    for (i=1; i< num_code_blocks; i++)
    {
        if(K[i] > *MM)
        {
            *MM = K[i];
        }
    }

    MemFree(cvec, CRAM_MEM);

    return(num_code_blocks);
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/** ATbrief DOX_INTERNAL BsTxRateMatch.
 *         This function implements the rate matching for each code block
 *         per 3GPP TS 36.212 Sec.5.1.4.1
 *
 *  @param in_bitstream Pointer to array containing the input bit streams
 *         associated with each code block
 *  @param G_num_bits_transport Total Number of bits available for the
 *         transmission of 1 Transport Block
 *  @param rvidx Redundancy Version number for this Transport Block
 *         per 3GPP 36.212 Sec5.3.3.1.2
 *  @param num_FillerBits  Number of filler (Null) bits
 *  @param NL Number of Layers
 *            = 1, for blocks mapped onto one transmission layer, or
 *            = 2, for blocks mapped onto two or four transmission layers
 *  @param Qm Number of bits mapped per symbol
 *         =1 for BPSK
 *         =2 for QPSK
 *         =4 for 16-QAM
 *         =6 for 64-QAM
 *  @param CBNum corresponds to the Code Block Number (0,1,...,C-1)
 *  @param C_num_code_blocks is the total number of code blocks (C above)
 *  @param dlen is the number of input bits in each CTC parallel stream
 *         output
 *  @param out_bitstream Pointer to the output bit stream that has been
 *         rate matched
 *  @return Len_out_bitstream Length of the Output Bit stream
 *
 *  This function implements the rate matching for each code block
 *	per 3GPP TS 36.212 Sec.5.1.4.1
 *
 *  The in_bitstream points to bit sequence array as follows
 *  d(0,0),d(0,1),...,d(0,dlen-1) = input bits in code block,
 *  d(0,0) being the first sample in time of the first parallel stream
 *  d(1,0),d(1,1),...,d(1,dlen-1) = input bits in code block,
 *  d(1,0) being the first sample in time of the second parallel stream
 *  d(2,0),d(2,1),...,d(2,dlen-1) = input bits in code block,
 *  d(2,0) being the first sample in time of the third parallel stream
 *

 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   **in_bitstream DOXYGEN_TO_DO
 *  @param   G_num_bits_transport DOXYGEN_TO_DO
 *  @param   rvidx DOXYGEN_TO_DO
 *  @param   num_FillerBits DOXYGEN_TO_DO
 *  @param   NL DOXYGEN_TO_DO
 *  @param   Qm DOXYGEN_TO_DO
 *  @param   CBNum DOXYGEN_TO_DO
 *  @param   C_num_code_blocks DOXYGEN_TO_DO
 *  @param   dlen DOXYGEN_TO_DO
 *  @param   out_bitstream[] DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -----------------------------------------------------------------------------------------------------------------
 *  |Name                 |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |**in_bitstream       |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |G_num_bits_transport |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |rvidx                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |num_FillerBits       |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |NL                   |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |Qm                   |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |CBNum                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |C_num_code_blocks    |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |dlen                 |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |out_bitstream[]      |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
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
S32 BsTxRateMatch(S16 **in_bitstream, S32 G_num_bits_transport,
    S16 rvidx, S16 num_FillerBits, S16 NL,
    S16 Qm, S16 CBNum, S16 C_num_code_blocks,
    S16 dlen, U8 out_bitstream[])
{

#define MAX_SIZE_RATE_MATCHING 8000
#define MAX_ND 200

    S16 i,j, k;
    S16 isubblock, irow, icol, yPtr;
    S16 NumSubblocks, ND, iv, temp;
    S16 ColTCSubblock, RowTCSubblock;
    S16 Ksubv, Kw;

    S16 gamma, k0, Ncb, CE_NULLBit;
    S32 Gdash, E_var, lk;
    S32 NIR;

    S16 **ymat;
    S16 **ymatNull;
    S16 **ypmat;
    S16 **ypmatNull;
    S16 **v_arry;
    S16 **vNull_arry;
    S16 *w_arry;
    S16 *wNull_arry;
    S16 *y;
    S16 *yNull;
    S16 *pi2;
    S16 *dNull;
    S16 *dummy;
    S16 *dummyNull;

    //Define constants, internal to this function
    CE_NULLBit = 0;

    //Step (1.1) Define number of columns in matrix
    ColTCSubblock = 32;

    //Step (1.2) Define number of rows in matrix
    RowTCSubblock = 0;
    while (dlen >= RowTCSubblock * ColTCSubblock)
    {
        RowTCSubblock = RowTCSubblock +1;
    }

    ymat = smatrix (0, RowTCSubblock-1, 0, ColTCSubblock-1);
    ymatNull = smatrix (0, RowTCSubblock-1, 0, ColTCSubblock-1);
    ypmat = smatrix (0, RowTCSubblock-1, 0, ColTCSubblock-1);
    ypmatNull = smatrix (0, RowTCSubblock-1, 0, ColTCSubblock-1);
    v_arry = smatrix (0, NUM_TC_STREAMS-1, 0, MAX_SIZE_RATE_MATCHING-1);
    vNull_arry = smatrix (0, NUM_TC_STREAMS-1, 0, MAX_SIZE_RATE_MATCHING-1);
    w_arry = svector (0, NUM_TC_STREAMS*MAX_SIZE_RATE_MATCHING-1);
    wNull_arry = svector (0, NUM_TC_STREAMS*MAX_SIZE_RATE_MATCHING-1);
    y = svector (0, MAX_SIZE_RATE_MATCHING-1);
    yNull = svector (0, MAX_SIZE_RATE_MATCHING-1);
    pi2 = svector (0, MAX_SIZE_RATE_MATCHING-1);
    dNull = svector (0, MAX_SIZE_RATE_MATCHING-1);
    dummy = svector (0, MAX_ND-1);
    dummyNull = svector (0, MAX_ND-1);

    //Step (1.3) Copy sub-blocks to rectangular matrix
    if (RowTCSubblock * ColTCSubblock > dlen)
    {
        ND = RowTCSubblock * ColTCSubblock - dlen;

        for(i=0; i< ND; i++)
        {
            dummy[i] = -1;
            dummyNull[i] = CE_NULLBit;
        }
    }

    //create output array memory for indexing below.
    //Initialized with invalid values -1, for easy checking
    NumSubblocks = 3;
    for(i=0; i< NumSubblocks; i++)
    {
        for(j=0; j< RowTCSubblock*ColTCSubblock; j++)
        {
            v_arry[i][j] = -1;
        }
    }

    for( isubblock= 0; isubblock < NumSubblocks; isubblock++)
    {

        //Create a parallel NullMatrix, to track the interleaving of <NULL>
        // bits from this point on
        for(j=0; j< dlen; j++)
        {
            dNull[j] = 1;
        }
        if (isubblock <= 1)
        {
            for(j=0; j< num_FillerBits; j++)
            {
                //0=<NULL>, NullMatrix version of d
                dNull[j] = 0;
            }
        }

        //first, copy to vector (length Row x Col), starting with ND
        //dummy <NULL> bits if necessary to pad to this length
        for(j=0; j< ND; j++)
        {
            y[j] = dummy[j];
        }
        for(j=0; j< dlen; j++)
        {
            y[j+ND] = in_bitstream[isubblock][j];
        }

        //0=<NULL>, NullMatrix version of y
        for(j=0; j< ND; j++)
        {
            yNull[j] = dummyNull[j];
        }
        for(j=0; j< dlen; j++)
        {
            yNull[j+ND] = dNull[j];
        }

        //copy vector elements to rectangular matrix, one row at a time
        yPtr = 0;
        for( irow = 0; irow < RowTCSubblock; irow++)
        {
            for( icol = 0; icol < ColTCSubblock; icol++)
            {
                ymat[irow][icol] = y[yPtr+icol];
                ymatNull[irow][icol] = yNull[yPtr+icol];
            }
            yPtr = yPtr + ColTCSubblock;
        }
        if (isubblock <= 1)
        {

            //Step (1.4) do 1st kind of inter-column permutation

            //create array memory for indexing below. Initialized with invalid
            //values -1, for easy checking
            for(i=0; i< RowTCSubblock; i++)
            {
                for(j=0; j< ColTCSubblock; j++)
                {
                    ypmat[i][j] = ypmatNull[i][j] = -1;
                }
            }

            for(irow=0; irow< RowTCSubblock; irow++)
            {
                for(icol=0; icol< ColTCSubblock; icol++)
                {
                    ypmat[irow][icol] =
                    ymat[irow][CA_RMCTC_InterColPerm[icol]];
                    ypmatNull[irow][icol] =
                    ymatNull[irow][CA_RMCTC_InterColPerm[icol]];
                }
            }

            //Step (1.5) read sub-block interleaver output bits from columns
            iv = 0; //initial index to output bit
            //vector of bits, v, = consecutive columns of bits from yp (transposed
            //for row vector)
            for (icol = 0; icol <ColTCSubblock; icol++)
            {
                for(irow=0; irow< RowTCSubblock; irow++)
                {
                    v_arry[isubblock][ iv+irow] = ypmat[irow][icol];
                    vNull_arry[isubblock][ iv+irow] = ypmatNull[irow][icol];
                }
                iv = iv + RowTCSubblock;
            }
        }
        else
        {
            //(isubblock == 2)

            // Step (1.4) read sub-block interleaver output bits based on 2nd
            // kind of permutation
            for (k=0; k<RowTCSubblock*ColTCSubblock; k++)
            {
                temp = divide_floor(k, RowTCSubblock);
                temp = CA_RMCTC_InterColPerm[temp];
                temp = temp + ColTCSubblock * (k%RowTCSubblock) +1;
                pi2[k]= temp% (RowTCSubblock*ColTCSubblock);

                v_arry[isubblock][ k] = y[pi2[k]];
                vNull_arry[isubblock][ k] = yNull[pi2[k]];
            }
        }
    }

    //Step (2) Bit collection

    //=length of each sub-block of v
    Ksubv = RowTCSubblock*ColTCSubblock;
    //=length of circular buffer w, where w is linear concatenation of
    //sub-blocks of v
    Kw = 3*Ksubv;
    for (k=0; k<Ksubv; k++)
    {
        //circular buffer, starts as vector v(0,:)
        w_arry[k] = v_arry[0][k];
        //0=<NULL>, NullMatrix version of w
        wNull_arry[k] = vNull_arry[0][k];
    }
    for (k=0; k<Ksubv; k++)
    {
        //interleave v(1,:) and v(2,:) samples as odd and even samples of w
        w_arry[Ksubv+2*k ] = v_arry[1][k];
        w_arry[Ksubv+2*k+1] = v_arry[2][k];
        wNull_arry[Ksubv+2*k] = vNull_arry[1][k];
        wNull_arry[Ksubv+2*k+1] = vNull_arry[2][k];
    }

    //Step (3) Bit selection and pruning [3GPP TS 36.212 v8.2.0 (2008-03)]
    //!!!!!!!!!!! need to add input parameter

    // NIR = Nsoft/(Kmimo*min(DL_HARQ, 8)), where Kmimo = 1 or 2
    // Nsoft is defined in 36.306 and depends on specified UE category
    // The largest Nsoft = 3667200
    // For now, use NIR = 3667200/8
    NIR = 458400;

    NIR = NIR/C_num_code_blocks;

    Ncb = Kw;

    if(NIR < Ncb) Ncb = (S16)NIR;

    Gdash = G_num_bits_transport/(NL*Qm);
    gamma = Gdash%C_num_code_blocks;
    if (CBNum <= C_num_code_blocks - gamma - 1)
    {
        E_var = Gdash/C_num_code_blocks;
        E_var = NL*Qm*E_var;

    }
    else
    {
        E_var = (Gdash+C_num_code_blocks-1)/C_num_code_blocks;
        E_var = NL*Qm*E_var;
    }
    temp = divide_ceil(Ncb, 8*RowTCSubblock);
    k0 = RowTCSubblock*(2*temp*rvidx+2);

    lk=0;
    j=0;
    while (lk < E_var)
    {
        if (wNull_arry[(k0+j)%Ncb] != CE_NULLBit)
        {
            //=<NULL>
            //=1
            out_bitstream[lk] = (U8)w_arry[(k0+j)%Ncb];
            lk = lk + 1;
        }
        j = j + 1;
    }

    free_smatrix(ymat, 0, RowTCSubblock-1, 0, ColTCSubblock-1);
    free_smatrix(ymatNull, 0, RowTCSubblock-1, 0, ColTCSubblock-1);
    free_smatrix(ypmat, 0, RowTCSubblock-1, 0, ColTCSubblock-1);
    free_smatrix(ypmatNull, 0, RowTCSubblock-1, 0, ColTCSubblock-1);
    free_smatrix(v_arry, 0, NUM_TC_STREAMS-1, 0, MAX_SIZE_RATE_MATCHING-1);
    free_smatrix(vNull_arry, 0, NUM_TC_STREAMS-1, 0, MAX_SIZE_RATE_MATCHING-1);
    free_svector(w_arry, 0, NUM_TC_STREAMS*MAX_SIZE_RATE_MATCHING-1);
    free_svector(wNull_arry, 0, NUM_TC_STREAMS*MAX_SIZE_RATE_MATCHING-1);
    free_svector(y, 0, MAX_SIZE_RATE_MATCHING-1);
    free_svector(yNull ,0, MAX_SIZE_RATE_MATCHING-1);
    free_svector(pi2, 0, MAX_SIZE_RATE_MATCHING-1);
    free_svector(dNull, 0, MAX_SIZE_RATE_MATCHING-1);
    free_svector(dummy, 0, MAX_ND-1);
    free_svector(dummyNull, 0, MAX_ND-1);

    //printf("!!!!! TX RATE MATCHING(%d): Kw=%d, k0=%d, E_var=%d, Ncb=%d\n",CBNum,Kw,k0,E_var,Ncb);

    return(E_var);
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/** ATbrief DOX_INTERNAL BsTxRateMatch2.
 *         This function implements the rate matching for each code block
 *         per 3GPP TS 36.212 Sec.5.1.4.1.  This function assumes that the
 *         sub-block interleaving function has already been performed on
 *         the input array.
 *
 *  @param in Pointer to array containing the input bit stream
 *         which is the sub-block interleaver output array (w).
 *
 *  @param null Pointer to array containing the indicators for each
 *         input bit. (0=valid input bit, 1=input bit is NULL).
 *
 *  @param inlen is the length of the input arrays.
 *
 *  @param G_num_bits_transport Total Number of bits available for the
 *         transmission of 1 Transport Block.
 *
 *  @param rvidx Redundancy Version number for this Transport Block
 *         per 3GPP 36.212 Sec5.3.3.1.2.
 *
 *  @param NL Number of Layers
 *            = 1, for blocks mapped onto one transmission layer, or
 *            = 2, for blocks mapped onto two or four transmission layers
 *
 *  @param Qm Number of bits mapped per symbol
 *         =1 for BPSK
 *         =2 for QPSK
 *         =4 for 16-QAM
 *         =6 for 64-QAM
 *
 *  @param CBNum corresponds to the Code Block Number (0,1,...,C-1).
 *
 *  @param C_num_code_blocks is the total number of code blocks (C above).
 *
 *  @param out Pointer to the output bit stream that has been
 *         rate matched
 *
 *  @return outlen Length of the Output Bit stream
 *
 *  This function implements the rate matching for each code block
 *	per 3GPP TS 36.212 Sec.5.1.4.1
 *

 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   in[] DOXYGEN_TO_DO
 *  @param   null[] DOXYGEN_TO_DO
 *  @param   inlen DOXYGEN_TO_DO
 *  @param   G_num_bits_transport DOXYGEN_TO_DO
 *  @param   rvidx DOXYGEN_TO_DO
 *  @param   NL DOXYGEN_TO_DO
 *  @param   Qm DOXYGEN_TO_DO
 *  @param   CBNum DOXYGEN_TO_DO
 *  @param   C_num_code_blocks DOXYGEN_TO_DO
 *  @param   out[] DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -----------------------------------------------------------------------------------------------------------------
 *  |Name                 |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |in[]                 |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |null[]               |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |inlen                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |G_num_bits_transport |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |rvidx                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |NL                   |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |Qm                   |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |CBNum                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |C_num_code_blocks    |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |out[]                |              |              |              |              |              |              |
 *  -----------------------------------------------------------------------------------------------------------------
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
S32 BsTxRateMatch2(U8 in[], U8 null[], S16 inlen,
    S32 G_num_bits_transport, S16 rvidx,
    S16 NL, S16 Qm, S16 CBNum,
    S16 C_num_code_blocks, U8 out[])
{

    S16 j, temp, k0, RowTCSubblock;
    S16 gamma, Ncb;
    S32 NIR;
    S32 Gdash, E_var, k;

    //Step (3) Bit selection and pruning [3GPP TS 36.212 v8.2.0 (2008-03)]
    //!!!!!!!!!!! need to add input parameter

    // NIR = Nsoft/(Kmimo*min(DL_HARQ, 8)), where Kmimo = 1 or 2
    // Nsoft is defined in 36.306 and depends on specified UE category
    // The largest Nsoft = 3667200
    // For now, use NIR = 3667200/8
    NIR = 458400;

    NIR = NIR/C_num_code_blocks;

    Ncb = inlen;

    if(NIR < Ncb) Ncb = (S16)NIR;

    RowTCSubblock = inlen/3/32;
    Gdash = G_num_bits_transport/(NL*Qm);
    gamma = Gdash%C_num_code_blocks;
    if (CBNum <= C_num_code_blocks - gamma - 1)
    {
        E_var = Gdash/C_num_code_blocks;
        E_var = NL*Qm*E_var;

    }
    else
    {
        E_var = (Gdash+C_num_code_blocks-1)/C_num_code_blocks;
        E_var = NL*Qm*E_var;
    }
    temp = divide_ceil(Ncb, 8*RowTCSubblock);
    k0 = RowTCSubblock*(2*temp*rvidx+2);

    k=0;
    j=0;
    while (k < E_var)
    {
        if (null[(k0+j)%Ncb] != 1)
        {
            out[k] = in[(k0+j)%Ncb];
            k = k + 1;
        }
        j = j + 1;
    }

    //printf("!!!!! TX RATE MATCHING(%d): Kw=%d, k0=%d, E_var=%d, Ncb=%d\n",CBNum,inlen,k0,E_var,Ncb);

    return (E_var);
}

#else   // FPGA

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/** ATbrief BsTxCodeBlkSeg
 *         This function implements code block segmentation per 3GPP TS 36.212
 *         Sec.5.1.2
 *
 *  @param in_bitstream Pointer to input bitstream
 *  @param code_blocks Pointer to an array for the code blocks
 *  @param KIndex  Index into [3GPP TS 36.212, Table 5.1.3-3], for value of Ki
 *                 used for	 codeBlock
 *  @param FillerBits Number of filler bits <NULL> added to the start of the
 *                    first output code block
 *  @param B number of input bits
 *  @param K Number of bits in block[r]
 *  @param MM corresponds to a pointer to the Maximum Subblock Length info
 *         The actual value is written by this function
 *
 *  The function BsTxCodeBlkSeg implements the code block segmentation for
 *  LTE per 3GPP TS 36.212 Sec.5.1.2 as follows
 *
 *  If the number of bits in code block {in_bitstream} is bigger than the
 *  maximum allowed, the single sequence {in_bitstream} is segmented into
 *  multiple smaller sequences {code_blocks} and a CRC24B appended to each
 *  sequence per 3GPP TS 36.212 Sec.5.1.2.
 *
 *  For the LTE Turbo coder Z=6144 corresponds to the maximum code block
 *  size in bits including CRC
 *
 *  The input is in_bitstream
 *  in_bitstream(0),in_bitstream(1),...,in_bitstream(B-1) = input bits in
 *  code block,
 *
 *  The output code blocks are defined by the pointer to an array code_blocks
 *  as follows
 *
 *  code_blocks(0,0),code_blocks(0,1),...,code_blocks(0,K(0)-1) = output bits
 *  in code block 0
 *
 *  where K(0) = number of bits in code block number 0
 *
 *  code_blocks(1,0),code_blocks(1,1),...,code_blocks(1,K(1)-1) = output bits
 *  in code block 1
 *
 *  where K(1) = number of bits in code block number 1
 *  ...
 *
 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   in_bitstream[] DOXYGEN_TO_DO
 *  @param   **code_blocks DOXYGEN_TO_DO
 *  @param   KIndex[] DOXYGEN_TO_DO
 *  @param   *FillerBits DOXYGEN_TO_DO
 *  @param   B  DOXYGEN_TO_DO
 *  @param   K[] DOXYGEN_TO_DO
 *  @param   *MM DOXYGEN_TO_DO
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
 *  |in_bitstream[]|              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |**code_blocks |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |KIndex[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*FillerBits   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |B             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |K[]           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*MM           |              |              |              |              |              |              |
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
S16 BsTxCodeBlkSeg(U8 in_bitstream[], U8 **code_blocks, S16 KIndex[], S16 *FillerBits, S32 B,
                   S16 K[], S16 *MM)
{

    S16 i, CBNum, UpperLimit, fb;
    S16 L, k;
    S16 num_code_blocks;
    S16 irow, deltaK;
    S16 Cplus, Kplus, Kminus, Cminus, KplusIndex;
    S32 F, Bdash, s;
    U8 *cvec;

#define Z 6144

    cvec = MemAlloc(B + 2 * NUM_PARITY, CRAM_MEM, 0);

    CE_CTCParamColKi = 1;

    // Step 1: If B < 40, add <NULL>-valued filler bits at the beginning of the
    //block
    //Note: Since the CRC appended to the TranportBlock (to change {a} to
    //{b}) is calculate d as a division function, then it is clear, that adding
    //zeros to the beginning of {b} does not change the CRC that was appended to
    //the transport block!

    if (B < 40)
    {
        //=number of filler bits (also must get num_code_blocks=1 in this case!)
        F = 40 - B;
        //=40, so block length is now considered to be 40! Filler bits will be
        //added later, in step (6).
        B = B + F;
    }

    // Step 2: Calculate number of code blocks, num_code_blocks
    if (B <= Z)
    {
        L = 0;
        //number of code blocks
        num_code_blocks = 1;
        //B-dash
        Bdash = B;
    }
    else
    {
        L = 24;

        num_code_blocks = (S16) ((B + Z - L - 1) / (Z - L));

        Bdash = B + (num_code_blocks * L);
    }

    // Step 3: Calculate number of bits in each code block with First
    // Segmentation Size (applicable to num_code_blocks != 0 only)
    // Kplus = min(K in table 5.1.3-3, such that num_code_blocks*K >= Bdash),
    //  i.e. Kplus = min(Ki in
    //  CA_CTCInternalInterleaverParams(CE_CTCParamColKi,:,:), such that
    //   num_code_blocks*K >= Bdash
    //
    // Note, K increases as the index of Ki down the column in the table
    // increases. So, in searching for the min(Ki...) that satisfies the
    // equation, start with row1 and increase until satisfied!

    //number of rows in the table, i.e. number of Ki terms
    irow = 0;
    Kplus = CA_CTCInternalInterleaverParams[irow][CE_CTCParamColKi - 1];

    while (num_code_blocks * Kplus < Bdash)
    {
        irow = irow + 1; //increment pointer to check next Ki
        Kplus = CA_CTCInternalInterleaverParams[irow][CE_CTCParamColKi - 1];
    }
    //keep for Kminus calculation below
    KplusIndex = irow;

    //Step 4 Calculate number of code blocks with length Kplus. Also,
    //Calculate number of bits in block with Second Segmentation Size, Kminus,
    //and number of blocks with length Kminus where,
    //   Kminus = max(K in table 5.1.3-3, such that K < Kplus), i.e.
    //   Kminus = max(Ki in
    //CA_CTCInternalInterleaverParams(CE_CTCParamColKi,:,:), such that K<Kplus
    //Note, K increases as the index of Ki down the column in the table
    //increases. So, in searching for the max(Ki...) that satisfies the
    //equation, simply read the value from the table, in the row before the
    //Kplus value.

    if (num_code_blocks == 1)
    {
        //number of code blocks with length Kplus
        Cplus = 1;
        //no other code blocks!
        Kminus = 0;
        Cminus = 0;
    }
    else
    {
        //num_code_blocks > 1. i.e. num_code_blocks cannot be <=0 at this point!

        //i.e. the biggest value in the table smaller than Kplus
        Kminus = CA_CTCInternalInterleaverParams[KplusIndex - 1][CE_CTCParamColKi - 1];
        deltaK = Kplus - Kminus;
        Cminus = (S16) ((num_code_blocks * Kplus - Bdash) / deltaK);
        //number of seqments of size Kplus
        Cplus = num_code_blocks - Cminus;
    }

    //Step 5 Start the first code block with the required number of filler bits

    fb = (S16) (Cplus * Kplus + Cminus * Kminus - Bdash); //number of filler bits

    k = 0;

    *FillerBits = fb;

    s = 0;

    //Step 6 Now, move the bits {b} from the input code block into the
    //segmented code blocks {c}

    for (CBNum = 0; CBNum < num_code_blocks; CBNum++)
    {
        if (CBNum < Cminus)
        {
            K[CBNum] = Kminus;
            //i.e. Kminus index
            KIndex[CBNum] = KplusIndex - 1;
        }
        else
        {
            K[CBNum] = Kplus;
            KIndex[CBNum] = KplusIndex;
        }
        if (CBNum == 0)
        {
            UpperLimit = K[CBNum] - L - fb; // To account for Filler Bits being placed by FPGA
        }
        else
        {
            UpperLimit = K[CBNum] - L;
        }
        while (k < UpperLimit)
        {
            code_blocks[CBNum][k] = in_bitstream[s];
            k = k + 1;
            s = s + 1;
        }

        if (num_code_blocks > 1)
        {
            for (i = 0; i < UpperLimit; i++)
            {
                //move the bits from the rectangular codeBlock array to a
                //vector, for the CRC function below
                cvec[i] = code_blocks[CBNum][i];
            }
            AddCodeBlockCRC24B(cvec, cvec, (S16) UpperLimit);
            for (i = 0; i < K[CBNum]; i++)
            {
                //move the final codeBlock, with appended CRC24B back into
                //the codeBlock array
                code_blocks[CBNum][i] = cvec[i];
            }
        }
        k = 0;
    }

    // Get Maximum sub-block length. for printing convenience
    *MM = K[0];
    for (i = 1; i < num_code_blocks; i++)
    {
        if (K[i] > *MM)
        {
            *MM = K[i];
        }
    }

    MemFree(cvec, CRAM_MEM);

    return (num_code_blocks);
}

#endif

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   pOutBuf DOXYGEN_TO_DO
 *  @param   tc_encout DOXYGEN_TO_DO
 *  @param   remBits DOXYGEN_TO_DO
 *  @param   nBits DOXYGEN_TO_DO
 *  @param   noutbytes DOXYGEN_TO_DO
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
 *  |pOutBuf       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |tc_encout     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |remBits       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nBits         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |noutbytes     |              |              |              |              |              |              |
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
void RepackBytes(U8** pOutBuf, U8* pIn, S16* remBits, U32 curBits)
{
    U8 msBits, mask, shift8, shift24;
    U16 shift, prevBits;
    U8 *pOut;

    U32 *pIn32, *pOut32;
    U32 mask32, mask32_1, lsBits32, msBits32, tail, tmp, Word;

    prevBits = *remBits;
    pOut = *pOutBuf - 1;
    shift = 8 - prevBits;
    shift8 = shift + 8;
    shift24 = 24 - shift;

    mask = MaskCC[shift];
    mask32 = Mask32[prevBits];
    mask32_1 = Mask32_1[prevBits];

    //  Update last byte from previous block
    msBits = *pIn >> shift;
    *pOut++ |= msBits;
    curBits -= prevBits;

    if (((U32) pIn & 0x3) == 0)
    {
        if (((U32) pOut & 0x3) == 0)
        {
            pIn32 = (U32*) pIn;
            pOut32 = (U32*) pOut;
            Word = *pIn32;

            while (curBits >= 256)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                curBits -= 256;
            }

            while (curBits >= 64)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                curBits -= 64;
            }

            while (curBits >= 32)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                *pOut32++ = msBits32 | tail;
                curBits -= 32;
            }
            pIn = (U8*) pIn32;
            pOut = (U8*) pOut32;
        }
        else if (((U32) pOut & 0x1) == 0)
        {
            pIn32 = (U32*) pIn;
            Word = *pIn32;

            while (curBits >= 256)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                curBits -= 256;
            }

            while (curBits >= 64)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                curBits -= 64;
            }

            while (curBits >= 32)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *(U16*) pOut = (U16) tmp;
                pOut += 2;
                *(U16*) pOut = tmp >> 16;
                pOut += 2;
                curBits -= 32;
            }
            pIn = (U8*) pIn32;
        }
        else
        {
            pIn32 = (U32*) pIn;
            Word = *pIn32;

            while (curBits >= 256)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                curBits -= 256;
            }

            while (curBits >= 64)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;

                curBits -= 64;
            }

            while (curBits >= 32)
            {
                msBits32 = (Word & mask32) << prevBits;
                lsBits32 = (Word >> shift8) & mask32_1;
                msBits32 |= lsBits32;
                Word = *++pIn32;
                tail = (Word & mask) << shift24;
                tmp = msBits32 | tail;
                *pOut++ = (U8) tmp;
                *(U16*) pOut = (U16) (tmp >> 8);
                pOut += 2;
                *pOut++ = tmp >> 24;
                curBits -= 32;
            }
            pIn = (U8*) pIn32;
        }
    }
    else if (((U32) pOut & 0x3) == 0)
    {
        pOut32 = (U32*) pOut;

        while (curBits >= 256)
        {
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            curBits -= 256;
        }

        while (curBits >= 32)
        {
            tmp = *pIn++;
            tmp |= (*pIn++ << 8);
            tmp |= (*pIn++ << 16);
            tmp |= (*pIn++ << 24);
            msBits32 = (tmp & mask32) << prevBits;
            lsBits32 = (tmp >> shift8) & mask32_1;
            msBits32 |= lsBits32;
            tail = (*pIn & mask) << shift24;
            *pOut32++ = msBits32 | tail;
            curBits -= 32;
        }
        pOut = (U8*) pOut32;
    }

    while (curBits >= 8)
    {
        msBits = *pIn++;
        *pOut++ = (msBits << prevBits) | (*pIn >> shift);
        curBits -= 8;
    }

    if (curBits != 0)
    {
        *remBits = (S16) (8 - curBits);

        msBits = *pIn++;
        *pOut = (msBits << prevBits) | (*pIn >> shift);
        *pOutBuf = pOut + 1;
    }
    else
    {
        *remBits = 0;
        *pOutBuf = pOut;
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
 *  @param   mcsIndex DOXYGEN_TO_DO
 *  @param   numRb DOXYGEN_TO_DO
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
 *  |mcsIndex      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |numRb         |              |              |              |              |              |              |
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
U32 LteReadTbsTable(U16 mcsIndex, U16 numRb)
{
    U32 tbsSize = 0;
    U16 tbsIndex;

    tbsIndex = McsTbi_Table[mcsIndex];

    if (numRb < 10)
    {
        tbsSize = (U32) TbsRbLu01_10[tbsIndex][numRb];
    }
    else if (numRb < 20)
    {
        tbsSize = (U32) TbsRbLu11_20[tbsIndex][numRb - 10];
    }
    else if (numRb < 30)
    {
        tbsSize = (U32) TbsRbLu21_30[tbsIndex][numRb - 20];
    }
    else if (numRb < 40)
    {
        tbsSize = (U32) TbsRbLu31_40[tbsIndex][numRb - 30];
    }
    else if (numRb < 50)
    {
        tbsSize = (U32) TbsRbLu41_50[tbsIndex][numRb - 40];
    }
    else if (numRb < 60)
    {
        tbsSize = (U32) TbsRbLu51_60[tbsIndex][numRb - 50];
    }
    else if (numRb < 70)
    {
        tbsSize = (U32) TbsRbLu61_70[tbsIndex][numRb - 60];
    }
    else if (numRb < 80)
    {
        tbsSize = (U32) TbsRbLu71_80[tbsIndex][numRb - 70];
    }
    else if (numRb < 90)
    {
        tbsSize = (U32) TbsRbLu81_90[(U32) tbsIndex][(U32) (numRb - 80)];
    }
    else if (numRb < 100)
    {
        tbsSize = (U32) TbsRbLu91_100[(U32) tbsIndex][(U32) (numRb - 90)];
    }
    else if (numRb < 110)
    {
        tbsSize = (U32) TbsRbLu101_110[(U32) tbsIndex][(U32) (numRb - 100)];
    }
    return (tbsSize);
}

