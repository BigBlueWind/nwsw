//-------------------------------------------------------------------------------------------
/** @file RxCoder_Util.c
 *
 * @brief LTE receive channel coding auxiliary functions.
 * This file contains the source code for the auxiliary functions used to implement the
 * receive channel coding procedures defined in the LTE (3GPP) standard.
 * @author Mindspeed Technologies
 * @version $Revision: 1.36 $
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
#else
#include "basic_op_cc.h"
#endif

//*****************************************************************************
//**********  CONDITIONAL COMPILATION CONTROL PARAMETER DEFINITIONS  **********
//*****************************************************************************

//*****************************************************************************
//**********             CONSTANT PARAMETER DEFINITIONS              **********
//*****************************************************************************

// Define maximum length of decoder's soft decision input array.
#define P_MAX_SFDEC_LEN     18528   // 3*32*INT(6144 + 4 + 31)/32)
// Define maximum allowable rate matching redundancy factor.  This number
// specifies the maximum number of soft decisions that can be averaged
// together.
#define P_MAX_SFDEC_AVG     5

// Define maximum length of rate matching output array
#define P_MAX_RATEM_LEN     (P_MAX_SFDEC_LEN*P_MAX_SFDEC_AVG)

// Define gCRC24A generator polynomial mask
// gCRC24A(D) = D**24 + D**23 + D**18 + D**17 + D**14 + D**11 + D**10 +
//              D**7 + D**6 + D**5 + D**4 + D**3 + D**1 + 1
#define P_GCRC24A   0xC3267D

// Define maximum number of HARQ processes
#define P_MAX_HARQ_PROC     8

// Define maximum size for each HARQ process
#define P_MAX_HARQ_SIZE     280128

//*****************************************************************************
//**********                         TABLES                          **********
//*****************************************************************************

//*****************************************************************************
//**********                rx_lte_rate_matching function            **********
//*****************************************************************************

//Define interleaver column permutation table

S16 IntlColPermTab[32] =
{ 0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30, 1, 17, 9, 25, 5, 21, 13, 29, 3, 19,
  11, 27, 7, 23, 15, 31 };

//*****************************************************************************
//**********                  MISCELLANEOUS ARRAYS                   **********
//*****************************************************************************

//*****************************************************************************
//**********                rx_lte_rate_matching function            **********
//*****************************************************************************

S16 d[3][6148]; // input streams

S16 tmpa[6176]; // 1st temporary interleaver buffer

S16 tmpb[6176]; // 2nd temporary interleaver buffer

S16 v[3][6176]; // interleaver outputs for each stream

S16 w[18528]; // Circular buffer

/******************************************************************************

 *******************************************************************************
 3GPP (LTE) Standard Section 5.1.4 Rate Matching
 *******************************************************************************

 Function rx_lte_rate_matching implements the rate matching function defined in
 the LTE standard.  The function calling format is defined below.

 nout = rx_lte_rate_matching(nblk, nfil, C, NL, G, Qm, r, rvidx,
 NIR, out[]);

 Where:  nblk =  code block size in bits.
 nfil =  number of filler bits.
 C =     number of code blocks.
 NL =    1 (for transport blocks mapped to 1 layer) or 2 (for transport
 blocks mapped to 2 or 4 layers).
 G =     total number of bits available for the transmission of one
 transport block.
 Qm =    2 for QPSK, 4 for 16QAM, or 6 for 64QAM.
 r =     code block index (0 - C-1).
 rvidx = redundancy version number (0, 1, 2, or 3).
 NIR =   soft buffer size for the transport block.
 Set = 0 for uplink receiver.
 out[] = output indices array.
 nout =  length of out[] array.



 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE channel coding rate matching function.
 *
 *  <1> Hardware Resource:
 *  FEC
 *
 *   @param     nblk =  code block size in bits.
 *   @param     nfil =  number of filler bits.
 *   @param     C =     number of code blocks.
 *   @param     NL =    1 (for transport blocks mapped to 1 layer) or 2 (for transport
 *                      blocks mapped to 2 or 4 layers).
 *   @param     G =     total number of bits available for the transmission of one
 *                      transport block.
 *   @param     Qm =    2 for QPSK, 4 for 16QAM, or 6 for 64QAM.
 *   @param     r =     code block index (0 - C-1).
 *   @param     rvidx = redundancy version number (0, 1, 2, or 3).
 *   @param     NIR =   soft buffer size for the transport block.
 *                      Set = 0 for uplink receiver.
 *   @param     out[] = output indices array.
 *   @return    nout =  length of out[] array.
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
 *  |nfil           |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |C              |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NL             |               |input        |1              |Real          |(1:15:0)  |{1,2}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |G              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(1:15:0)  |{2,4,6}        |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |r              |               |input        |1              |Real          |(1:15:0)  |[0:C-1]        |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |rvidx          |               |input        |1              |Real          |(1:15:0)  |{0,1,2,3}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NIR            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |output       |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------------
 *  |Block Name          |rx_lte_rate_matching                   |
 *  --------------------------------------------------------------
 *  |Input Stream(s)     |none                                   |
 *  --------------------------------------------------------------
 *  |Output Stream(s)    |out                                    |
 *  --------------------------------------------------------------
 *  |Config Parameter(s) |nblk, nfil, C, NL, G, Qm, r, rvidx, NIR|
 *  --------------------------------------------------------------
 *  |Status Parameter(s) |nout                                   |
 *  --------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function rx_lte_rate_matching implements the rate matching function defined in
 * the LTE standard.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0 (2009-03) Section 5.1.4
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
S32 rx_lte_rate_matching(S16 nblk, S16 nfil, S16 C, S16 NL, S32 G, S16 Qm, S16 r, S16 rvidx,
                         S32 NIR, S16 out[])
{

    //*****************************************************************************
    //**********             CONSTANT PARAMETER DEFINITIONS              **********
    //*****************************************************************************

    // Define sub-block interleaver number of columns
#define p_ncol   32

    //*****************************************************************************

    S16 D, nrow, j, i, nintl, nfill, k, p, pp;
    S16 Kw, Ncb, g;
    S32 Gd, E, k0, kin, kout;

    //*************************************************************************
    // The inputs to the rate matching algorithm are 3 streams that are
    // the outputs from the CTC encoder.  These streams are:
    //  d[0][ ], d[1][ ] and d[2][ ].

    // The length of each stream will be the block size (nblk) + 4 extra
    // for the termination of the trellis.  The first step is to load each
    // stream with the corresponding CTC encoder output array index.  Also,
    // as specified in the standard d[0][ ] and d[1][ ] are set to <NULL> for
    // those inputs that were filled.

    // D = length of each rate matching input stream
    D = nblk + 4;
    // Intialize encoder output array index
    j = 0;
    // Load encoder output array indices into streams
    for (i = 0; i < D; i++)
    {
        d[0][i] = j;
        d[1][i] = j + 1;
        d[2][i] = j + 2;
        j = j + 3;
    }
    // Remove indices for outputs corresponding to filler bits
    for (i = 0; i < nfil; i++)
    {
        d[0][i] = -1;
        d[1][i] = -1;
    }
    //*************************************************************************
    //**********                 SUB-BLOCK INTERLEAVING              **********
    //*************************************************************************
    // Each input stream is block interleaved separately.  The interleaver
    // buffers are arranged as p_ncol columns by nrow rows.

    // Compute number of rows in interleaver buffer
    nrow = D / p_ncol;
    if ((D - (nrow * p_ncol)) != 0)
        nrow = nrow + 1;
    // Compute total size of interleaver buffer
    nintl = nrow * p_ncol;
    // Compute number of extra required <NULL> filler bits
    nfill = nintl - D;
    // Do for each input stream
    for (i = 0; i < 3; i++)
    {
        // Initialize interleaver buffer index
        k = 0;
        // Fill with extra <NULL>s if required
        for (j = 0; j < nfill; j++)
        {
            tmpa[k] = -1;
            k = k + 1;
        }
        // Load stream into interleaver buffer
        for (j = 0; j < D; j++)
        {
            tmpa[k] = d[i][j];
            k = k + 1;
        }
        // If first two streams
        if (i < 2)
        {
            // Initialize output index
            p = 0;
            // Intitialize input index
            pp = 0;
            // Do for each interleaver buffer row
            for (j = 0; j < nrow; j++)
            {
                // Do for each interleaver buffer column
                for (k = 0; k < p_ncol; k++)
                {
                    // Select and copy elements for each row
                    tmpb[p] = tmpa[IntlColPermTab[k] + pp];

                    // Increment output index
                    p = p + 1;
                }
                // Increment input index to next row
                pp = pp + p_ncol;
            }
            // Initialize output index
            p = 0;
            // Do for each column in interleaver buffer
            for (j = 0; j < p_ncol; j++)
            {
                // Initialize input index to column
                pp = j;
                // For every row in column
                for (k = 0; k < nrow; k++)
                {
                    // Select and copy element from column
                    v[i][p] = tmpb[pp];
                    // Increment output pointer
                    p = p + 1;
                    // Increment input pointer to next row
                    pp = pp + p_ncol;
                }
            }

        }
        // Else, if 3rd stream
        else
        {
            // Do for each element in interleaver buffer
            for (k = 0; k < nintl; k++)
            {
                // Compute index for column permutation table
                j = k / nrow;
                // Include column permutation
                p = IntlColPermTab[j];
                // Compute remaining index according to standard
                j = k % nrow;
                j = j * p_ncol;
                j = j + 1;
                p = p + j;
                pp = p % nintl;
                // Select and copy element from interleaver buffer
                v[i][k] = tmpa[pp];
            }
        }
    }

    //*************************************************************************
    //**********       BIT COLLECTION, SELECTION AND TRANSMISSION    **********
    //*************************************************************************

    // Load sub-block interleaver outputs for each steam into circular buffer
    for (i = 0; i < nintl; i++)
    {
        w[i] = v[0][i];
        w[nintl + i + i] = v[1][i];
        w[nintl + i + i + 1] = v[2][i];
    }

    // Compute length of circular buffer
    Kw = 3 * nintl;

    // If Turbo Concept CTC decoder is using sub-block de-interleaver
    // Bypass sub-block interleaver for subsequent rate matching

    for (i = 0; i < Kw; i++)
    {
        if (w[i] >= 0)
            w[i] = i;
    }

    // Ncb = circular buffer size
    Ncb = Kw;
    // If downlink, then limit circular buffer size
    if (NIR > 0)
    {
        if ((NIR / C) < Ncb)
        {
            Ncb = (S16) (NIR / C);
        }
    }
    // Compute number of symbols
    Gd = G / (NL * Qm);
    // Compute remainder
    g = Gd % C;
    // If current code block number is small enough
    if (r <= (C - g - 1))
    {
        // Compute number of total outputs
        E = Gd / C;
        E = E * NL * Qm;
    }
    else
    {
        // Compute larger number of total outputs
        E = Gd / C;
        if ((Gd - (E * C)) != 0)
        {
            E = E + 1;
        }
        E = E * NL * Qm;
    }

    // Compute circular buffer starting point
    j = Ncb / (8 * nrow);
    if ((Ncb - (j * 8 * nrow)) != 0)
    {
        j = j + 1;
    }
    k0 = nrow * ((2 * j * rvidx) + 2);

    // Initialize output index
    kout = 0;
    // Initialize input index
    kin = k0;

    laba:
    // Limit circular buffer index
    if (kin >= Ncb)
    {
        kin = kin - Ncb;
    }
    // If not a <NULL> value
    if (w[kin] != -1)
    {
        // Select and output value
        out[kout] = w[kin];
        // Increment output index
        kout = kout + 1;
    }
    // Increment input index
    kin = kin + 1;
    // If not enough outputs generated, then continue
    if (kout < E)
        goto laba;

    return (kout);
}



/******************************************************************************

 *******************************************************************************
 3GPP (LTE) Standard Section 5.1.4 Rate Matching For Hardware Simulation
 *******************************************************************************

 Function rx_lte_rate_matching_hdwr implements the rate matching function
 defined in the LTE standard.  The function calling format is defined below.

 rx_lte_rate_matching_hdwr(nblk, nfil, E, k0, Ncb, out[]);

 Where:  nblk =  code block size in bits.
 nfil =  number of filler bits.
 E =     rate matching parameter E (number of outputs).
 k0 =    rate matching parameter k0 (circular buffer starting index).
 Ncb =   rate matching parameter Ncb (circular buffer size).
 out[] = output indices array.


 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE channel coding rate matching function for hardware simulation.
 *
 *  <1> Hardware Resource:
 *  FEC
 *
 *   @param     nblk =  code block size in bits.
 *   @param     nfil =  number of filler bits.
 *   @param     E =     rate matching parameter E (number of outputs).
 *   @param     k0 =    rate matching parameter k0 (circular buffer starting index).
 *   @param     Ncb =   rate matching parameter Ncb (circular buffer size).
 *   @param     out[] = output indices array.
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nblk           |               |input        |1              |Real          |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nfil           |               |input        |1              |Real          |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |E              |               |input        |1              |Real          |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |k0             |               |input        |1              |Real          |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Ncb            |               |input        |1              |Real          |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal         |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------------
 *  |Block Name          |rx_lte_rate_matching_hdwr              |
 *  --------------------------------------------------------------
 *  |Input Stream(s)     |none                                   |
 *  --------------------------------------------------------------
 *  |Output Stream(s)    |out                                    |
 *  --------------------------------------------------------------
 *  |Config Parameter(s) |nblk, nfil, E, k0, Ncb                 |
 *  --------------------------------------------------------------
 *  |Status Parameter(s) |                                       |
 *  --------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function rx_lte_rate_matching implements the rate matching function defined in
 * the LTE standard.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0 (2009-03) Section 5.1.4
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void rx_lte_rate_matching_hdwr(U16 nblk, U16 nfil, U32 E, U16 k0, U16 Ncb, S16 out[])
{

    //*****************************************************************************
    //**********             CONSTANT PARAMETER DEFINITIONS              **********
    //*****************************************************************************

    // Define sub-block interleaver number of columns
#define p_ncol   32

    //*****************************************************************************

    S16 D, nrow, j, i, nintl, nfill, k, p, pp;
    S16 Kw;
    U32 kin, kout;

    //*************************************************************************
    // The inputs to the rate matching algorithm are 3 streams that are
    // the outputs from the CTC encoder.  These streams are:
    //  d[0][ ], d[1][ ] and d[2][ ].

    // The length of each stream will be the block size (nblk) + 4 extra
    // for the termination of the trellis.  The first step is to load each
    // stream with the corresponding CTC encoder output array index.  Also,
    // as specified in the standard d[0][ ] and d[1][ ] are set to <NULL> for
    // those inputs that were filled.

    // D = length of each rate matching input stream
    D = nblk + 4;

    // Intialize encoder output array index
    j = 0;

    // Load encoder output array indices into streams
    for (i = 0; i < D; i++)
    {
        d[0][i] = j;
        d[1][i] = j + 1;
        d[2][i] = j + 2;
        j = j + 3;
    }

    // Remove indices for outputs corresponding to filler bits
    for (i = 0; i < (S16) nfil; i++)
    {
        d[0][i] = -1;
        d[1][i] = -1;
    }

    //*************************************************************************
    //**********                 SUB-BLOCK INTERLEAVING              **********
    //*************************************************************************

    // Each input stream is block interleaved separately.  The interleaver
    // buffers are arranged as p_ncol columns by nrow rows.

    // Compute number of rows in interleaver buffer
    nrow = D / p_ncol;
    if ((D - (nrow * p_ncol)) != 0)
        nrow = nrow + 1;

    // Compute total size of interleaver buffer
    nintl = nrow * p_ncol;

    // Compute number of extra required <NULL> filler bits
    nfill = nintl - D;

    // Do for each input stream
    for (i = 0; i < 3; i++)
    {
        // Initialize interleaver buffer index
        k = 0;

        // Fill with extra <NULL>s if required
        for (j = 0; j < nfill; j++)
        {
            tmpa[k] = -1;
            k = k + 1;
        }

        // Load stream into interleaver buffer
        for (j = 0; j < D; j++)
        {
            tmpa[k] = d[i][j];
            k = k + 1;
        }

        // If first two streams
        if (i < 2)
        {
            // Initialize output index
            p = 0;

            // Intitialize input index
            pp = 0;

            // Do for each interleaver buffer row
            for (j = 0; j < nrow; j++)
            {
                // Do for each interleaver buffer column
                for (k = 0; k < p_ncol; k++)
                {
                    // Select and copy elements for each row
                    tmpb[p] = tmpa[IntlColPermTab[k] + pp];

                    // Increment output index
                    p = p + 1;
                }

                // Increment input index to next row
                pp = pp + p_ncol;
            }

            // Initialize output index
            p = 0;

            // Do for each column in interleaver buffer
            for (j = 0; j < p_ncol; j++)
            {
                // Initialize input index to column
                pp = j;

                // For every row in column
                for (k = 0; k < nrow; k++)
                {
                    // Select and copy element from column
                    v[i][p] = tmpb[pp];

                    // Increment output pointer
                    p = p + 1;

                    // Increment input pointer to next row
                    pp = pp + p_ncol;
                }
            }

        }
        // Else, if 3rd stream
        else
        {
            // Do for each element in interleaver buffer
            for (k = 0; k < nintl; k++)
            {
                // Compute index for column permutation table
                j = k / nrow;

                // Include column permutation
                p = IntlColPermTab[j];

                // Compute remaining index according to standard
                j = k % nrow;
                j = j * p_ncol;
                j = j + 1;
                p = p + j;
                pp = p % nintl;

                // Select and copy element from interleaver buffer
                v[i][k] = tmpa[pp];
            }
        }
    }

    //*************************************************************************
    //**********       BIT COLLECTION, SELECTION AND TRANSMISSION    **********
    //*************************************************************************

    // Load sub-block interleaver outputs for each steam into circular buffer
    for (i = 0; i < nintl; i++)
    {
        w[i] = v[0][i];
        w[nintl + i + i] = v[1][i];
        w[nintl + i + i + 1] = v[2][i];
    }

    // Compute length of circular buffer
    Kw = 3 * nintl;

    // If Turbo Concept CTC decoder is using sub-block de-interleaver
    // Bypass sub-block interleaver for subsequent rate matching

    for (i = 0; i < Kw; i++)
    {
        if (w[i] >= 0)
            w[i] = i;
    }

    // Initialize output index
    kout = 0;

    // Initialize input index
    kin = k0;

    laba:
    // Limit circular buffer index
    if (kin >= Ncb)
    {
        kin = kin - Ncb;
    }
    // If not a <NULL> value
    if (w[kin] != -1)
    {
        // Select and output value
        out[kout] = w[kin];

        // Increment output index
        kout = kout + 1;
    }
    // Increment input index
    kin = kin + 1;

    // If not enough outputs generated, then continue
    if (kout < E)
        goto laba;

    return;
}



/******************************************************************************

 *******************************************************************************
 3GPP (LTE) Standard Section 5.1.1 CRC Calculation
 *******************************************************************************

 Function ComputeCRC24 implements the 24-bit crc generation function (gCRC24A or
 gCRC24B) defined in LTE standard.  The function calling format is defined
 below.

 ComputeCRC24(in, n, genmsk, crc);

 Where:  in =        input data array organized as bytes.
 n =         input data array length in bytes (32-bit parameter).
 genmsk =    Generator polynomial mask (32-bit parameter).
 crc =       output 24-bit crc organized as bytes.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE 24-bit CRC computation.
 *
 *  <1> Hardware Resource:
 *  MDMA
 *
 *   @param     in =        Packed input data array organized as bytes.
 *   @param     n =         Input data array length in bytes (32-bit parameter).
 *   @param     genmsk =    Generator polynomial mask (32-bit parameter).
 *   @param     crc =       Output 24-bit crc organized as bytes.
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
 *  |in             |               |input        |n              |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |n              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |genmsk         |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |crc            |               |output       |3              |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ComputeCRC24                |
 *  ---------------------------------------------------
 *  |Input Stream(s)     |in                          |
 *  ---------------------------------------------------
 *  |Output Stream(s)    |crc                         |
 *  ---------------------------------------------------
 *  |Config Parameter(s) |n, genmsk                   |
 *  ---------------------------------------------------
 *  |Status Parameter(s) |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function ComputeCRC24 implements the 24-bit crc generation function (gCRC24A or
 * gCRC24B) defined in LTE standard.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0 (2009-03) Section 5.1.1
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------
void ComputeCRC24(U8 in[], S32 n, S32 genmsk, U8 crc[])
{
    S32 i, j, pin, ac;
    S16 bit24, bitin, inbyte = 0, k;

    // Initialize shift register
    ac = 0;
    // Initialize input array pointer
    pin = 0;

    j = 3;
    if (n < j)
    {
        j = n;
    }

    // Initialize input byte shift counter
    k = 16;
    // For 1st 3 input bytes
    for (i = 0; i < j; i++)
    {
        // Shift input byte and load into shift register
        ac = ac | (in[pin] << k);
        // Decrement input byte shift counter
        k = k - 8;
        // Increment input array pointer
        pin = pin + 1;
    }

    // j = input array length in bits
    j = n << 3;
    // For each input array bit
    for (i = 0; i < j; i++)
    {
        // If beginning of next input byte
        if (i % 8 == 0)
        {
            // Initialize next input byte to 0
            inbyte = 0;

            // If still more input bytes in array
            if (pin < n)
            {
                // Read next input array byte
                inbyte = in[pin];
            }
            // Increment input array pointer
            pin = pin + 1;
        }

        // bit24 = bit #24 in shift register
        bit24 = (ac >> 23) & 1;

        // If bit24 = 1, then xor in generator polynomial
        if (bit24 != 0)
        {
            ac = ac ^ genmsk;
        }
        // Shift shift register left 1 bit
        ac = ac << 1;
        // bitin = next input bit
        bitin = (inbyte >> 7) & 1;
        // Shift current input byte left 1 bit
        inbyte = inbyte << 1;
        // Shift next input bit contribution into shift register
        ac = ac | (bitin ^ bit24);
    }
    // Output 24-bit CRC 1st byte
    crc[0] = (ac >> 16) & 255;
    // Output 24-bit CRC 2nd bytes
    crc[1] = (ac >> 8) & 255;
    // Output 24-bit CRC 3rd byte
    crc[2] = ac & 255;

    return;
}

void descrambler_fl(U32 cinit, double bx[], double by[], S32 length_bx)
{

    S32 i;
    U8 *cvec;

    cvec = MemAlloc(length_bx, CRAM_MEM, 0);

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
    MemFree(cvec, CRAM_MEM);

    return;
}
//RateMatchParms rm1;

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Det_rx_lte_rate_matching_parms function.
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *   @param     nblk =  code block size in bits.
 *   @param     nfil =  number of filler bits.
 *   @param     C =     number of code blocks.
 *   @param     NL =    1 (for transport blocks mapped to 1 layer) or 2 (for transport
 *                      blocks mapped to 2 or 4 layers).
 *   @param     G =     total number of bits available for the transmission of one
 *                      transport block.
 *   @param     Qm =    2 for QPSK, 4 for 16QAM, or 6 for 64QAM.
 *   @param     r =     code block index (0 - C-1).
 *   @param     rvidx = redundancy version number (0, 1, 2, or 3).
 *   @param     NIR =   soft buffer size for the transport block.
 *              Set = 0 for uplink receiver.
 *   @return    Pointer to a RateMatchParms structure.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name  |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  ---------------------------------------------------------------------------------------------------
 *  |nblk  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  |nfil  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  |C     |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  |NL    |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  |G     |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  |Qm    |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  |r     |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  |rvidx |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  |NIR   |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------
 *  |Block Name          |Det_rx_lte_rate_matching_parms |
 *  ------------------------------------------------------
 *  |Input Streams       |DOXYGEN_TO_DO                  |
 *  ------------------------------------------------------
 *  |Output Streams      |DOXYGEN_TO_DO                  |
 *  ------------------------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO                  |
 *  ------------------------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO                  |
 *  ------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Function Det_rx_lte_rate_matching_parms determines the parameters required
 * for the HW to execute the rate de-matching functions per LTE 3GPP 36.212
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * [1] LTE 3GPP 36.212
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//-------------------------------------------------------------------------------------------
void Det_rx_lte_rate_matching_parms(S16 nblk, S16 nfil, S16 C, S16 NL, S32 G, S16 Qm, S16 r,
                                    S16 rvidx, S32 NIR, RateMatchParms *pRm)
{
    S16 D, nrow, j, nintl;
    S16 Kw, Ncb, g, k0;
    S32 Gd, E;

    // D = length of each rate matching input stream
    D = nblk + 4;
    // Compute number of rows in interleaver buffer
    nrow = D / p_ncol;
    if ((D - (nrow * p_ncol)) != 0)
    {
        nrow = nrow + 1;
    }
    // Compute total size of interleaver buffer
    nintl = nrow * p_ncol;
    // Compute length of circular buffer
    Kw = 3 * nintl;
    // Ncb = circular buffer size
    Ncb = Kw;
    // If downlink, then limit circular buffer size
    if (NIR > 0)
    {
        if ((NIR / C) < Ncb)
            Ncb = (S16) (NIR / C);
    }
    // Compute number of symbols
    Gd = G / (NL * Qm);
    // Compute remainder
    g = Gd % C;
    // If current code block number is small enough
    if (r <= (C - g - 1))
    {
        // Compute number of total outputs
        E = Gd / C;
        E = E * NL * Qm;
    }
    else
    {
        // Compute larger number of total outputs
        E = Gd / C;
        if ((Gd - (E * C)) != 0)
        {
            E = E + 1;
        }
        E = E * NL * Qm;
    }

    // Compute circular buffer starting point
    j = Ncb / (8 * nrow);

    if ((Ncb - (j * 8 * nrow)) != 0)
    {
        j = j + 1;
    }

    k0 = nrow * ((2 * j * rvidx) + 2);

    pRm->E = E;
    pRm->k0 = k0;
    pRm->Ncb = Ncb;
}
