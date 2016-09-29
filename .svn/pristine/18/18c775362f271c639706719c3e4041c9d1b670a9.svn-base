//-------------------------------------------------------------------------------------------
/** @file bs_rx_channel_coding.c
 *
 * @brief LTE receive channel coding function.
 * This file contains the source code for the functions used to implement the
 * receive channel coding procedures defined in the LTE (3GPP) standard.
 * @author Mindspeed Technologies
 * @version $Revision: 1.123 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

/******************************************************************************
**********          MORE CRITICAL HARDWARE FEC INFORMATION           **********
*******************************************************************************

 It appears that all of the Turner devices have some type of FEC channel
 de-interleaver problem.  This de-interleaver is designed to de-interleave one
 code block prior to de-rate matching and CTC decoding for the PUSCH.  For some
 reason the CTC decoder generates errors after de-interleaving and de-rate
 matching for some of the PUSCH test cases.  Unfortunately, there is no way in
 the Turner device to examine the CTC decoder inputs directly.  Only the decoder
 outputs can be seen.  Therefore, it is impossible to determine exactly what is
 causing the problem.  Bugzilla #66949 has been opened to track the problem.
 The FEC's designer Chuong Vu has investigated but so far has not been able to
 explain the observations.  The following conclusions are based on several
 PUSCH test cases executed on all of the Turner EVM boards.

 1) QPSK (Qm=2) test cases never seem to fail.

 2) All of the failures occur for either QAM16 (Qm=4) or QAM64 (Qm=6).  The
    failures seem to be related to the de-interleaver columns having different
    lengths.  For PUSCH test cases where the de-interleaver buffer columns have
    equal lengths there never are any failures.  The multiplexing of CQI and/or
    RI with the CTC data causes the column lengths to be different.

 3) As an experiment, the RI soft-decisions were all set to 0.  Amazingly, the
    CTC decoder output errors were reduced.  If the de-interleaver was
    operating properly, all of these RI soft decisions should be ignored.  This
    experiment seems to indicate that for some reason some of the RI soft
    decisions are incorrectly being processed during de-interleaving.

 4) Depending on the CTC decoder's programmed number of half iterations,
    sometimes it appears that whatever errors are being generated the decoder
    is capable of correcting them.  For this reason it is important to use only
    2 half iterations to help prevent any errors from being corrected.

 5) All of the PUSCH test case failures can be corrected by modifying the
    channel de-interleaver descriptors so that all of the columns have the same
    length.  Of course, this work-around results in slightly more puncturing
    since the shortest column length must be used for all of the columns and
    therefore some of the soft decisions normally sent to the de-rate matching
    block will be ignored.  This is the only work-around that seems to work.

 6) The work-around can easily be prevented when the extra puncturing
    associated with the work-around is excessive.

 The code segments needed to implement this work-around are surrounded by:

    //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX

******************************************************************************/

// The conditions for executing the work-around are defined below.  These
// conditions ensure that the work-around does not result in excessive
// puncturing.  Of course, if the work-around is prevented because of excessive
// puncturing, then currently there is no guarantee that the FEC hardware will
// work correctly!

//      L = (P_FEC_DEINT_SCF*E) >> 15
//      if(L < K+4) L = K+4
//      if(T > L) execute work-around

//      Where:  E = de-rate matching input size.
//              K = code block size.
//              T = size of de-interleaver after work-around.
//              P_FEC_DEINT_SCF = fractional limit (x32768)

#define P_FEC_DEINT_SCF         29491   // .9

//*****************************************************************************

// Control parameter P_ENB_FEC_DEINT_FIX is used to enable (1) or disable (0)
// the FEC channel de-interleaver work-around.

#define P_ENB_FEC_DEINT_FIX     1   // Enable Turner FEC de-interleaver fix

/******************************************************************************
**********             CRITICAL HARDWARE FEC INFORMATION             **********
*******************************************************************************

 The X6 Turner devices have the following FEC CC decoder issues:

    1) For odd K the FEC will lock up.

    2) For K < 32 the decoder outputs can be wrong depending on the data.

    3) For E > 3K the FEC will lock up.

    4) For K not an integer multiple of 8 the decoder does not output the last
       byte.

    Where:  K = block size = number of bits that are CC encoded.
            E = de-rate matching input size.

 The new X7 Turner devices should fix issue 4) but not issues 1), 2), or 3).

 For the X7 device the following workarounds are proposed:

    1) Convert odd K to even K by doubling K.  This will involve adding soft
       decisions as well as re-mapping the soft decision input array.

    2) Convert K < 32 to K >= 32 by doubling K.  This also will involve adding
       soft decisions as well as re-mapping the soft decision input array.

    3) Convert E > 3K to E = 3K.  This will involve adding some of the soft
       decision input array elements together.

    4) The X7 should fix the K not an integer multiple of 8 problem.

There is a new function DevInfoIsT4Kx7() that returns true if and only if
the device is a X7.  This function is used by the PHY code to conditionally
execute the required code needed to support the X7 device.

The code segments needed to support the X7 device are surrounded by:

    //X7X7X7X7X7X7

The code segments needed to support the X7 device are enabled by defining the
control parameter X7_DEV_ENABLED.

******************************************************************************/

#define X7_DEV_ENABLED      1       // Define to enable X7 specific code

/******************************************************************************
**********             X7 FEC WORKAROUND PROFILING INFO              **********
*******************************************************************************

 All of the X7 FEC workaround code can be found in function
 bs_rx_cc_cont_chan_coding( ):

 25001  Measures time required for any required soft-decision combining needed
        to workaround (E > 3K) issue.

 25002  Measures time required for any required soft-decision remapping needed
        to workaround K < 32 and odd K issues.

 25003  Measures time required for any required decoder output copying needed
        to workaround K < 32 and odd K issues.

 25004  Measures total time required to execute function
        bs_rx_cc_cont_chan_coding( ).

 The profiling results are summarized below.

    Test used:

        rt_ul_test21.bat.  Control file TestConfig_ul_test21.cfg was
        modified for worst-case conditions as shown below.

            betaOffsetCQIIndex:         15
            nr1CQI:                     59

        This configuration results in function bs_rx_cc_cont_chan_coding( )
        nin = 656 and nout = 59.

    EVM platform:

        c5k-alan1

        DDR freq =      120MHz
        FEC UL freq =   450MHz
        ARM freq =      600MHz

    Other conditions:

        All bs_rx_cc_cont_chan_coding( ) inputs and outputs were in ARM
        cachable DDR memory.

    Profiling measurements:

        ------------------------------------------------------------------
        ID      Cycles (150MHz counter)   Time (msec)   Equivalent ARM MHz
        -----   -----------------------   -----------   ------------------
        25001   2968                      .020          11.87
        25002   1118                      .007           4.47
        25003     85                      .001            .34

        Total   4171                      .028          16.68

        25004   7440                      .050
        ------------------------------------------------------------------

 Conclusions:

    Worst-case workaround time (per user):          .028msec
    Worst-case workaround ARM loading (per user):    16.68Mhz
    Worst-case total CC decoding time (per user):   .050msec

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "tc7000_lte.h"
#include "tc1700.h"
#include "basic_op_cc.h"

//RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
// If using Rossetti device.
#if USE_ROSSETTI_DEV

U8 fec_decode_hdwr_sim_rossetti(
    U16 CodingType,             // Type of encoding or decoding (0=CC, 1=CTC)
    U16 BlockSize,              // Code block size
    U16 CrcSelector,            // Type of CRC (0=none, 1=gcrc24A, 2=gcrc24b)
    U16 FillerBits,             // Number of initial filler bits in code block
    U16 CodingRate,             // Coding rate
    U16 HalfIterations,         // Number of decoder 1/2 iterations
    U16 DynamicStopIteration,   // CTC decoder automatic stop select
    U16 BpDecoder,              // Decoder bypass
    U16 EnableCombine,          // HARQ soft-combining enable
    U32 RateMatchingE,          // Rate matching E parameter
    U16 RateMatchingk0,         // Rate matching k0 parameter
    U16 RateMatchingNcb,        // Rate matching Ncb parameter
    U16 Qm,                     // Number of modulation bits used (2,4,6)
    U16 BpChanInt,              // Bypass channel de-interleaver
    U16 ColInt,                 // Number of channel interleaver columns (12 or 14)
    S8 *DescAdr[],              // Channel de-interleaver descriptor starting addresses
    S32 DescLen[],              // Channel de-interleaver descriptor byte counts
    U16 DescN,                  // Channel de-interleaver number of descriptors
    S8 In[],                    // Input buffer pointer
    U8 Out[],                   // Output buffer pointer
    U16 CodeBlockIndex,         // Code block index for debug only
    U8 HarqWeight);             // HARQ combining weight

#endif
//RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR

//*****************************************************************************
//**********             CONSTANT PARAMETER DEFINITIONS              **********
//*****************************************************************************

// Define gCRC24A generator polynomial mask
// gCRC24A(D) = D**24 + D**23 + D**18 + D**17 + D**14 + D**11 + D**10 +
//              D**7 + D**6 + D**5 + D**4 + D**3 + D**1 + 1

#define P_GCRC24A   0xC3267D

//*****************************************************************************

/******************************************************************************

 Function bs_rx_channel_coding implements the channel coding function for the
 3GPP (LTE) uplink receiver.  The functional calling arguments are defined below.

 bs_rx_channel_coding(nin, nout, in[], out[], *stat, pharq, NL, G, Qm, rvidx,
                      NIR, halfit, ncqi, nri, Cmux, CP, Itbs, Nprbm1)

 Where: nin =   length of soft-decision input array in[].
        nout =  length of decoded data output array out[] in bytes.
        in[] =  interleaved soft-decision input array
                (2's complement 8-bit format).
        out[] = decoded data output array arranged as bytes.
        *stat = bit-mapped receiver status as defined below.
                bit0: gCRC24A CRC status: (1=good, 0=bad).
                bit1: gCRC24B CRC status: (1=good, 0=bad).
        pharq = pointer to HARQ control structure.
        halfit = number of CTC decoder 1/2 iterations (1 - 32).
        ncqi =  Number of soft-decisions in input buffer used for CQI.
        nri  =  Number of soft-decisions in input buffer used for RI.
        Cmux =  Channel interleaver number of columns (9-12).
        CP =    Cyclic prefix indication (0=normal, 1=extended).
        Itbs =  Transport block table index (0 - 26).
        Nprbm1 = Transport block table Nprb - 1 (0 - 109).

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 The following parameters are also required to specify the rate matching.
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        NL =    1 (for transport blocks mapped to 1 layer) or 2 (for
                transport blocks mapped to 2 or 4 layers).
        G =     total number of bits available for the transmission of one
                transport block.
        Qm =    2 for QPSK, 4 for 16QAM, or 6 for 64QAM.
        rvidx = redundancy version number (0, 1, 2, or 3).
        NIR =   soft buffer size for the transport block.
                Set = 0 for uplink receiver.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE uplink receiver channel coding function<BR>
 *  Function bs_rx_channel_coding implements the channel coding function for the
 *  3GPP (LTE) uplink receiver as defined in document 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 *  <1> Hardware Resource:
 *  ARM, FEC, MDMA
 *
 *  @param     nin     length of soft-decision input array in[].
 *  @param     nout    length of decoded data output array out[] in bytes.
 *  @param     in[]    interleaved soft-decision input array (8-bit 2's complement format).
 *  @param     out[]   decoded data output array arranged as bytes.
 *  @param     *stat   bit-mapped receiver status:<BR>
 *                     bit0: gCRC24A CRC status: (1=good, 0=bad).<BR>
 *                     bit1: gCRC24B CRC status: (1=good, 0=bad).<BR>
 *  @param     pharq   pointer to HARQ control structure.<BR>
 *  @param     NL      1 (for transport blocks mapped to 1 layer) or 2 (for
 *                     transport blocks mapped to 2 or 4 layers).
 *  @param     G       Total number of bits available for the transmission of one
 *                     transport block.
 *  @param     Qm      2 for QPSK, 4 for 16QAM, or 6 for 64QAM.
 *  @param     rvidx   Redundancy version number (0, 1, 2, or 3).
 *  @param     NIR     soft buffer size for the transport block.<BR>
 *                     Set = 0 for uplink receiver.
 *  @param     halfit  Number of 1/2 iterations to be performed by the CTC decoder<BR>
 *                     (1 - 32).
 *  @param     ncqi    Number of input soft decisions used for CQI.
 *  @param     nri     Number of input soft decisions used for RI.
 *  @param     Cmux =  Channel interleaver number of columns (9-12).
 *  @param     CP =    Cyclic prefix indication (0=normal, 1=extended).
 *  @param     Itbs =  Transport block table index (0 - 26).
 *  @param     Nprbm1 = Transport block table Nprb - 1 (0 - 109).
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
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |stat           |               |output       |1              |pReal         |(1:15:0)  |{0,1,2,3}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pharq          |               |input        |1              |pReal         |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NL             |               |input        |1              |Real          |(1:15:0)  |{1,2}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |G              |               |input        |1              |Real          |(1:31:0)  |[1: maximum]   |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(1:15:0)  |{2,4,6}        |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |rvidx          |               |input        |1              |Real          |(1:15:0)  |{0,1,2,3}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NIR            |               |input        |1              |Real          |(1:31:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |halfit         |               |input        |1              |Real          |(0:31:0)  |[1:32]         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ncqi           |               |input        |1              |Real          |(1:31:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nri            |               |input        |1              |Real          |(1:31:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Cmux           |               |input        |1              |Real          |(1:31:0)  |9,10,11,12}    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CP             |               |input        |1              |Real          |(1:15:0)  |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Itbs           |               |input        |1              |Real          |(1:15:0)  |[0:26]         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Nprbm1         |               |input        |1              |Real          |(1:15:0)  |[0:109]        |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------------------------------------------
 *  |Block Name          |bs_rx_channel_coding                                                |
 *  -------------------------------------------------------------------------------------------
 *  |Input Stream(s)     |in                                                                  |
 *  -------------------------------------------------------------------------------------------
 *  |Output Stream(s)    |out                                                                 |
 *  -------------------------------------------------------------------------------------------
 *  |Config Parameter(s) |nin,nout,pharq,NL,G,Qm,rvidx,NIR,halfit,ncqi,nri,Cmux,CP,Itbs,Nprbm1|
 *  -------------------------------------------------------------------------------------------
 *  |Status Parameter(s) |stat                                                                |
 *  -------------------------------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function bs_rx_channel_coding implements the channel coding function for the
 * 3GPP (LTE) uplink receiver as defined in document 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 *
 * \ingroup group_lte_rx_uplink_fec_decoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------

void bs_rx_channel_coding(S32 nin, S32 nout, S8 in[], U8 out[], S16 *stat, PHARQS pharq, S16 NL,
                          S32 G, S16 Qm, S16 rvidx, S32 NIR, U32 halfit, S32 ncqi, S32 nri,
                          S32 Cmux, S16 CP, S16 Itbs, S16 Nprbm1, S16 user)

{

#ifdef _WIN32

#if 0
// Currently, weighted HARQ combining only supported in floating point version

    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    // If using weighted HARQ combining.
#if USE_WEIGHTED_HARQ_COMB

    U8 sftscf[4];
    S32 ac0, ac1;

#endif
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

#endif

    PHARQ_DESCRIPTOR_T pHarqDescriptor = pharq->pHarqDescriptor;

    //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX
    // If Turner device FEC de-interleaver issue work-around enabled, then.
    #if P_ENB_FEC_DEINT_FIX

    // Required only for Turner FEC de-interleaver work-around
    S32 i, minlen, ncol, len, nlen, lim;

    #endif
    //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX

    S16 C, K, Ncb;
    S32 E;
    S16 r;
    S32 j;
    S32 outp, inp;
    S16 crcstat, crctype;

    S16 prc, iter, citer;
    U16 harqcomb, decbypass;
    S8 *psft;

    U16 dinbypass;
    U8 gen_crc[3];

    // Set bypass hardware channel de-interleaver = 0 as default
    dinbypass = 0;

    // If Cmux=0 then bypass hardware channel de-interleaver
    // Note, this option is required to support the C-based regression tests
    // contained in folder ...\C-Code\Prj\RunBERTests\.
    if(Cmux == 0)dinbypass = 1;

    // Initialize CRC24B status to good.
    crcstat = 1;

    // Use table look-up to compute total number of code blocks (C).
    C = Table_FEC_CodeBlock_C[Itbs][Nprbm1];

    // Use table look-up to compute code block size in bits (K);
    K = Table_FEC_CodeBlock_K[Itbs][Nprbm1];

    // Use table look-up to compute rate matching buffer size in bits (Ncb);
    Ncb = Table_FEC_RateMatching_Ncb[Itbs][Nprbm1];

    // If only one code block then there is no attached CRC24B (crctype = 0).
    // Otherwise, CRC24B is appended to each code block (crctype = 2).
    crctype = 0;
    if (C > 1) crctype = 2;

    //printf("bs_rx_channel_coding: nin=%d, nout=%d, NL=%d\n", nin, nout, NL);
    //printf("bs_rx_channel_coding: G=%d, Qm=%d, rvidx=%d, NIR=%d, halfit=%d\n", G, Qm, rvidx, NIR, halfit);
    //printf("bs_rx_channel_coding: ncqi=%d, nri=%d\n", ncqi, nri);
    //printf("bs_rx_channel_coding: CP=%d, Itbs=%d, Nprbm1=%d, user=%d\n", CP, Itbs, Nprbm1, user);
    //printf("bs_rx_channel_coding: C=%d, K=%d, Ncb=%d\n", C, K, Ncb);

    // Initaialize soft decision input array index.
    inp = 0;

    // Initialize output array index.
    outp = 0;

    // Initialize receiver bit-mapped output status to all good.
    *stat = STAT_SUCCESS;

    // prc = HARQ process.
    prc = pharq->harq_prc;

    // iter = number of total HARQ iterations - 1.
    iter = pharq->harq_iter[prc][user];

    // Limit iter to maximum allowed - 1.
    if(iter >= P_HARQ_COMB) iter = P_HARQ_COMB-1;

    // citer = current HARQ iteration (0 - P_HARQ_COMB-1).
    citer = pharq->harq_iter[prc][user] % P_HARQ_COMB;

#if 0
// Currently, weighted HARQ combining only supported in floating point version

    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    // If using weighted HARQ combining.
#if USE_WEIGHTED_HARQ_COMB

    // Initialize summation of all HARQ weights
    ac0 = 0;

    // For each HARQ iteration
    for(j = 0; j <= iter; j++)
    {
        // Add HARQ weight for iteration to sum
        r = pharq->harq_weight[prc][user][j];
        ac0 = ac0 + r;
    }

    // For each HARQ iteration
    for(j = 0; j <= iter; j++)
    {
        r = pharq->harq_weight[prc][user][j];
        ac1 = (r << 8)/ac0;
        if(ac1 > 255) ac1 = 255;
        sftscf[j] = (U8) ac1;
        printf("Harq Weight[%d] = %d, sftscf[%d] = %d\n", j, r, j, sftscf[j]);
    }

#endif
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

#endif

    //printf("Running bs_rx_channel_coding with HARQ enabled:\n");
    //printf("  harq process = %d, user = %d, iter = %d\n\n", prc, user, iter);

    // Do for each code block.
    for(r = 0; r < C; r++)
    {

        // Compute rate matching parameter E.
        E = Compute_Rate_Matching_E(C, NL, G, Qm, r);

        // Initialize harq combining control for no combining.
        harqcomb = 0;

        // Initialize CTC decoder bypass control for bypass
        decbypass = 1;

        //printf("Code block loop: iteration = %d, size = %d\n", r, K);

        // Do for each HARQ iteration
        for(j = 0; j <= iter; j++)
        {

            // If last iteration, then enable CTC decoder.
            if(j == iter) decbypass = 0;

            // psft = pointer to soft decisions for iteration
            psft = pharq->harq_sft_pos[prc][user][j];

            //printf("  harq iter = %d, rvidx = %d, psft = %d, comb = %d, decbyp = %d\n", j, rvidx, psft, harqcomb, decbypass);

            // If iteration is current HARQ iteration, then.
            if(j == citer)
            {
                // Save rate matching E for code block in HARQ structure.
                pharq->harq_E[prc][user][j][r] = E;

                // rvidx = rate matching rviex for iteration
                rvidx = pharq->harq_rvidx[prc][user][j];

                // Save rate matching k0 for code block in HARQ structure.
                pharq->harq_k0[prc][user][j][r] = Table_FEC_RateMatching_k0[rvidx][Itbs][Nprbm1];

                // Save constellation size in HARQ structure.
                if(r == 0) pharq->harq_Qm[prc][user][j] = Qm;

                // If channel de-interleaver enabled, then generate channel
                // de-interleaver descriptors for code block and save in HARQ
                // structure.
                if(dinbypass == 0)
                {
                    LTE_Chan_Deint_Gen(inp, inp-1+E,
                                       CP, Qm, Cmux,
                                       nin, ncqi, nri,
                                       psft,
                                       &pHarqDescriptor->harq_DescAdr[prc][user][j][r][0],
                                       &pHarqDescriptor->harq_DescLen[prc][user][j][r][0],
                                       &pHarqDescriptor->harq_DescN[prc][user][j][r]);

                    //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX
                    // If Turner device FEC de-interleaver issue work-around enabled, then.
                    #if P_ENB_FEC_DEINT_FIX

                    // If QAM16 (Qm=4) or QAM64 (Qm=6), then.
                    if(Qm > 2)
                    {

                        //printf("!!! FEC De-interleaver work-around: !!!\n     Qm = %d, Number of columns = %d\n",
                        //       Qm, pHarqDescriptor->harq_DescN[prc][user][j][r]);

                        // Compute limit for executing work-around.
                        lim = E*P_FEC_DEINT_SCF;
                        lim = lim >> 15;
                        if(lim < (K+4)) lim = K+4;

                        // Initialize minimum column length.
                        minlen = 1000000;

                        // Initialize number of non-zero lengths.
                        nlen = 0;

                        // Get number of columns.
                        ncol = pHarqDescriptor->harq_DescN[prc][user][j][r];

                        // For each of the other de-interleaver column lengths.
                        for(i = 0; i < ncol; i++)
                        {
                            // get column length.
                            len = pHarqDescriptor->harq_DescLen[prc][user][j][r][i];

                            // If not an empty column, then.
                            if(len != 0)
                            {
                                // If column length is shorter, then use it as minimum length.
                                if(len < minlen) minlen = len;

                                // Increment non-empty column counter.
                                nlen = nlen + 1;
                            }

                            //printf("     Column length[%d] = %d, minlen = %d\n", i, len, minlen);

                        }

                        //printf("nlen=%d, minlen=%d, nlen*minlen=%d, E=%d, K=%d, lim=%d\n", nlen, minlen, nlen*minlen, E, K, lim);

                        // If additional puncturing does not exceed limit, then.
                        if( (minlen*nlen) > lim)
                        {

                            // For each de-interleaver buffer column.
                            for(i = 0; i < ncol; i++)
                            {
                                // Reset longer column lengths to minimum column length.
                                if(pHarqDescriptor->harq_DescLen[prc][user][j][r][i] > minlen)
                                {

                                    //printf("Original %dth column length=%d, new column length=%d\n",
                                    //       i, pHarqDescriptor->harq_DescLen[prc][user][j][r][i], minlen);

                                    pHarqDescriptor->harq_DescLen[prc][user][j][r][i] = minlen;
                                }
                            }

                        }

                    }

                    #endif
                    //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX

                }
            }

            //RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
            // If using Rossetti device.
#if USE_ROSSETTI_DEV

#if 0
// Currently, weighted HARQ combining only supported in floating point version

            //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
            // If using weighted HARQ combining.
//#if USE_WEIGHTED_HARQ_COMB

            // Run simulation for Rossetti FEC hardware decoder
            crcstat = fec_decode_hdwr_sim_rossetti(
                1,                                              // Type of encoding or decoding (0=CC, 1=CTC)
                (U16) K,                                        // Code block size
                (U16) crctype,                                  // Type of CRC
                (U16) 0,                                        // Number of initial filler bits in code block
                0,                                              // Coding rate
                (U16) halfit,                                   // Number of decoder 1/2 iterations
                0,                                              // CTC decoder automatic stop select
                decbypass,                                      // Decoder bypass
                harqcomb,                                       // HARQ soft-combining enable
                (U32) pharq->harq_E[prc][user][j][r],           // Rate matching E parameter
                (U16) pharq->harq_k0[prc][user][j][r],          // Rate matching k0 parameter
                (U16) Ncb,                                      // Rate matching Ncb parameter
                (U16) pharq->harq_Qm[prc][user][j],             // Number of modulation bits used (2,4,6)
                dinbypass,                                      // Bypass channel de-interleaver
                12,                                             // Number of channel interleaver columns
                &pHarqDescriptor->harq_DescAdr[prc][user][j][r][0],       // Channel de-interleaver descriptor starting addresses
                (S32*)&pHarqDescriptor->harq_DescLen[prc][user][j][r][0], // Channel de-interleaver descriptor byte counts
                (U16)pHarqDescriptor->harq_DescN[prc][user][j][r],        // Channel de-interleaver number of descriptors
                (S8*) (psft+inp),                               // Input buffer pointer
                (U8*) (out+outp),                               // Output buffer pointer
                (U16) r,                                        // Code block index for debug only
                sftscf[j]);                                     // HARQ combining weight

#else

            // Run simulation for Rossetti FEC hardware decoder
            crcstat = fec_decode_hdwr_sim_rossetti(
                1,                                              // Type of encoding or decoding (0=CC, 1=CTC)
                (U16) K,                                        // Code block size
                (U16) crctype,                                  // Type of CRC
                (U16) 0,                                        // Number of initial filler bits in code block
                0,                                              // Coding rate
                (U16) halfit,                                   // Number of decoder 1/2 iterations
                0,                                              // CTC decoder automatic stop select
                decbypass,                                      // Decoder bypass
                harqcomb,                                       // HARQ soft-combining enable
                (U32) pharq->harq_E[prc][user][j][r],           // Rate matching E parameter
                (U16) pharq->harq_k0[prc][user][j][r],          // Rate matching k0 parameter
                (U16) Ncb,                                      // Rate matching Ncb parameter
                (U16) pharq->harq_Qm[prc][user][j],             // Number of modulation bits used (2,4,6)
                dinbypass,                                      // Bypass channel de-interleaver
                12,                                             // Number of channel interleaver columns
                &pHarqDescriptor->harq_DescAdr[prc][user][j][r][0],       // Channel de-interleaver descriptor starting addresses
                (S32*)&pHarqDescriptor->harq_DescLen[prc][user][j][r][0], // Channel de-interleaver descriptor byte counts
                (U16)pHarqDescriptor->harq_DescN[prc][user][j][r],        // Channel de-interleaver number of descriptors
                (S8*) (psft+inp),                               // Input buffer pointer
                (U8*) (out+outp),                               // Output buffer pointer
                (U16) r,                                        // Code block index for debug only
                255);                                           // HARQ combining weight

#endif
            //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

#else

            // Run simulation for FEC hardware decoder
            crcstat = fec_decode_hdwr_sim( 1,                   // Type of encoding or decoding (0=CC, 1=CTC)
                (U16) K,                                        // Code block size
                (U16) crctype,                                  // Type of CRC
                (U16) 0,                                        // Number of initial filler bits in code block
                0,                                              // Coding rate
                (U16) halfit,                                   // Number of decoder 1/2 iterations
                0,                                              // CTC decoder automatic stop select
                decbypass,                                      // Decoder bypass
                harqcomb,                                       // HARQ soft-combining enable
                (U32) pharq->harq_E[prc][user][j][r],           // Rate matching E parameter
                (U16) pharq->harq_k0[prc][user][j][r],          // Rate matching k0 parameter
                (U16) Ncb,                                      // Rate matching Ncb parameter
                (U16) pharq->harq_Qm[prc][user][j],             // Number of modulation bits used (2,4,6)
                dinbypass,                                      // Bypass channel de-interleaver
                12,                                             // Number of channel interleaver columns
                &pHarqDescriptor->harq_DescAdr[prc][user][j][r][0],       // Channel de-interleaver descriptor starting addresses
                (S32*)&pHarqDescriptor->harq_DescLen[prc][user][j][r][0], // Channel de-interleaver descriptor byte counts
                (U16)pHarqDescriptor->harq_DescN[prc][user][j][r],        // Channel de-interleaver number of descriptors
                (S8*) (psft+inp),                               // Input buffer pointer
                (U8*) (out+outp),                               // Output buffer pointer
                (U16) r);                                       // Code block index for debug only

#endif
            //RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR

            // Set HARQ combining to combine next time.
            harqcomb = 1;

        }

        // Compute number of decoded data bits.
        j = K;

        // If more than 1 code block
        if(C > 1)
        {
        // Subtract 24 gCRC24B CRC bits from decoded data bits total.
        j = j - 24;

        // If gCRC24B error than clear status bit.
        if(crcstat == 0)
            *stat = *stat & 1;

        }

        // If last code block, then subtract 24 gCRC24A CRC bits from
        // decoded data bits total.
        if(r == (C-1) ) j = j - 24;

        // Increment output array index.
        outp += j/8;

        // Increment input array index.
        inp += E;

    }

    // Compute 24-bit CRC using final received data bytes.
    ComputeCRC24(out, nout, P_GCRC24A, gen_crc);

    // If gCRC24A CRC error than clear status bit.
    if(out[outp] != gen_crc[0])
        *stat = *stat & 2;
    if(out[outp+1] != gen_crc[1])
        *stat = *stat & 2;
    if(out[outp+2] != gen_crc[2])
        *stat = *stat & 2;

#endif  //_WIN32

    return;

}

//X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7
#ifdef  X7_DEV_ENABLED

//*****************************************************************************
//**********                    CONTROL PARAMETERS                   **********
//*****************************************************************************

    // Define smallest allowed CC encoder block size
    #define P_MIN_K     20

    // Define largest allowed CC encoder block size
    #define P_MAX_K     68

    // Define number of allowed CC encoder block sizes
    #define P_NUM_K     (P_MAX_K - P_MIN_K + 1)

    // Based on the LTE channel coding standard requirements alone (refer to
    // 3GPP 36.212 section 5.2.2.6) it is theoretically possible to use all
    // of the allocated resource blocks to send CQI alone.  This could result
    // in repeating the CC encoder output bits hundreds of times.  Because
    // of the CC decoder de-rate matching issue (E > 3K), any soft decision
    // combining must be performed by the ARM to prevent FEC lock-up.  In order
    // to limit the ARM execution time, a limitation is imposed on the de-rate
    // matching input size (nin) as defined below.

    //  if(nin > 3*K*P_MAX_RIN) nin = 3*K*P_MAX_RIN

    //  Where:  nin =       number of input soft decisions
    //          K =         CC encoder input block size
    //          P_MAX_RIN = limiting input repeating scale factor

    // Define maximum soft decision input repeating scale factor
    #define P_MAX_RIN   3

//*****************************************************************************
//**********                          TABLES                         **********
//*****************************************************************************

// CC new block size table

// The X7 Turner device has the following CC decoder block size related
// issues.

//  1)  For K < 32 the decoder outputs can be incorrect.  To fix this problem,
//      at least 32 output bits must be generated.

//  2)  For odd K the decoder locks up.  To fix this problem all odd Ks must
//      be changed to even Ks by doubling K.

// Table CC_Knew_Table[] specifies the new block sizes (Knew) required for
// each of the allowed block sizes (K).  K = Knew means that no re-mapping
// is required.


                                // Knew             // K
                                // ----                --
S16 CC_Knew_Table[P_NUM_K] =    {   40,             // 20
                                    42,             // 21
                                    44,             // 22
                                    46,             // 23
                                    48,             // 24
                                    50,             // 25
                                    52,             // 26
                                    54,             // 27
                                    56,             // 28
                                    58,             // 29
                                    60,             // 30
                                    62,             // 31
                                    32,             // 32
                                    66,             // 33
                                    34,             // 34
                                    70,             // 35
                                    36,             // 36
                                    74,             // 37
                                    38,             // 38
                                    78,             // 39
                                    40,             // 40
                                    82,             // 41
                                    42,             // 42
                                    86,             // 43
                                    44,             // 44
                                    90,             // 45
                                    46,             // 46
                                    94,             // 47
                                    48,             // 48
                                    98,             // 49
                                    50,             // 50
                                   102,             // 51
                                    52,             // 52
                                   106,             // 53
                                    54,             // 54
                                   110,             // 55
                                    56,             // 56
                                   114,             // 57
                                    58,             // 58
                                   118,             // 59
                                    60,             // 60
                                   122,             // 61
                                    62,             // 62
                                   126,             // 63
                                    64,             // 64
                                   130,             // 65
                                    66,             // 66
                                   134,             // 67
                                    68};            // 68

//*****************************************************************************

// CC de-rate matching re-mapping table

// Whenever the CC block size is changed because of X7 Turner issues, the
// decoder soft decision input array has to be modified.  These modifications
// include the insertion of extra soft decsions and the re-mapping of the
// actual input soft decision array.  This re-mapping is required because
// of the differences in the sub-block interleaving that occur for different
// block sizes.  The table below contains the required re-mapping offsets
// for each of the block sizes.

                                                                             // ----------
S16 CC_RM_Remapping_Table[2412] = {  2,  0,  4,  3,  1,  1,  2,  3,          // 20 -> 40
                                     0,  4,  7,  5,  9,  8,  6,  6,
                                     7,  8,  5,  9, 12, 10, 14, 13,
                                    11, 11, 12, 13, 10, 14, 17, 15,
                                    19, 18, 16, 16, 17, 18, 15, 19,
                                    18,  0, 20, 19,  1, 17,  2,  3,          // 21 -> 42
                                    16,  4, 12,  5,  6, 11,  7, 15,
                                    14,  8,  9, 13, 10,  2, 11,  4,
                                     3, 12,  1, 13, 14,  0, 15,  8,
                                    16, 10,  9, 17,  7,  6, 18, 19,
                                     5, 20,
                                     8,  0, 10,  9,  1,  7,  6,  2,          // 22 -> 44
                                     3,  5,  4,  1,  5,  6,  0,  7,
                                     4,  3,  8,  9,  2, 10, 19, 11,
                                    21, 20, 12, 18, 17, 13, 14, 16,
                                    15, 12, 16, 17, 11, 18, 15, 14,
                                    19, 20, 13, 21,
                                    13,  0,  1, 12,  2, 16, 15,  3,          // 23 -> 46
                                     4, 14,  5, 19, 18,  6,  7, 17,
                                     8, 22, 21,  9, 10, 20, 11,  9,
                                    12, 11, 10, 13,  8,  7, 14, 15,
                                     6, 16,  2,  1, 17, 18,  0, 19,
                                     5,  4, 20, 21,  3, 22,
                                     2,  1,  0,  1,  0,  2,  5,  4,          // 24 -> 48
                                     3,  4,  3,  5,  8,  7,  6,  7,
                                     6,  8, 11, 10,  9, 10,  9, 11,
                                    14, 13, 12, 13, 12, 14, 17, 16,
                                    15, 16, 15, 17, 20, 19, 18, 19,
                                    18, 20, 23, 22, 21, 22, 21, 23,
                                    21, 20,  0,  1, 19,  2, 24, 23,          // 25 -> 50
                                     3,  4, 22,  5, 18, 17,  6,  7,
                                    16,  8,  9, 13, 10, 14, 11, 15,
                                    12,  2,  1, 13, 14,  0, 15,  5,
                                     4, 16, 17,  3, 18,  8,  7, 19,
                                    20,  6, 21, 12, 11, 22,  9, 23,
                                    10, 24,
                                     8,  7,  0,  1,  6,  2, 12, 11,          // 26 -> 52
                                     3,  9,  4, 10,  5,  5,  4,  6,
                                     7,  3,  8,  9,  0, 10,  1, 11,
                                     2, 12, 21, 20, 13, 14, 19, 15,
                                    25, 24, 16, 22, 17, 23, 18, 18,
                                    17, 19, 20, 16, 21, 22, 13, 23,
                                    14, 24, 15, 25,
                                    19, 18,  0,  1, 17,  2,  3, 14,          // 27 -> 54
                                     4, 15,  5, 16,  6, 26, 25,  7,
                                    23,  8, 24,  9, 10, 20, 11, 21,
                                    12, 22, 13,  9,  8, 14, 15,  7,
                                    16, 13, 12, 17, 10, 18, 11, 19,
                                     6,  5, 20,  3, 21,  4, 22, 23,
                                     0, 24,  1, 25,  2, 26,
                                     6,  5,  0,  3,  1,  4,  2,  3,          // 28 -> 56
                                     0,  4,  1,  5,  2,  6, 13, 12,
                                     7, 10,  8, 11,  9, 10,  7, 11,
                                     8, 12,  9, 13, 20, 19, 14, 17,
                                    15, 18, 16, 17, 14, 18, 15, 19,
                                    16, 20, 27, 26, 21, 24, 22, 25,
                                    23, 24, 21, 25, 22, 26, 23, 27,
                                    28, 27,  0, 25,  1, 26,  2,  3,          // 29 -> 58
                                    22,  4, 23,  5, 24,  6,  7, 15,
                                     8, 16,  9, 17, 10, 18, 11, 19,
                                    12, 20, 13, 21, 14,  6,  5, 15,
                                     3, 16,  4, 17, 18,  0, 19,  1,
                                    20,  2, 21, 14, 13, 22, 11, 23,
                                    12, 24,  7, 25,  8, 26,  9, 27,
                                    10, 28,
                                    14, 13,  0, 11,  1, 12,  2,  7,          // 30 -> 60
                                     3,  8,  4,  9,  5, 10,  6,  7,
                                     0,  8,  1,  9,  2, 10,  3, 11,
                                     4, 12,  5, 13,  6, 14, 29, 28,
                                    15, 26, 16, 27, 17, 22, 18, 23,
                                    19, 24, 20, 25, 21, 22, 15, 23,
                                    16, 24, 17, 25, 18, 26, 19, 27,
                                    20, 28, 21, 29,
                                     0, 16,  1, 17,  2, 18,  3, 19,          // 31 -> 62
                                     4, 20,  5, 21,  6, 22,  7, 23,
                                     8, 24,  9, 25, 10, 26, 11, 27,
                                    12, 28, 13, 29, 14, 30, 15, 15,
                                    14, 16, 12, 17, 13, 18,  8, 19,
                                     9, 20, 10, 21, 11, 22,  0, 23,
                                     1, 24,  2, 25,  3, 26,  4, 27,
                                     5, 28,  6, 29,  7, 30,
                                    25,  0, 26,  1, 27,  2, 28,  3,          // 33 -> 66
                                    29,  4, 30,  5, 31,  6, 32,  7,
                                    21,  8, 22,  9, 23, 10, 24, 11,
                                    19, 12, 20, 13, 18, 14, 17, 15,
                                    16,  0, 17,  1, 18,  2, 19,  3,
                                    20,  4, 21,  5, 22,  6, 23,  7,
                                    24,  8, 25,  9, 26, 10, 27, 11,
                                    28, 12, 29, 13, 30, 14, 31, 15,
                                    16, 32,
                                    22,  0, 23,  1, 24,  2, 25,  3,          // 35 -> 70
                                    20,  4, 21,  5, 19,  6, 18,  7,
                                     8, 30,  9, 31, 10, 32, 11, 33,
                                    34, 12, 28, 13, 29, 14, 27, 15,
                                    26, 16, 17,  9, 18, 10, 19, 11,
                                    20, 12, 21, 13, 22, 14, 23, 15,
                                    24, 16, 17, 25,  4, 26,  5, 27,
                                     6, 28,  7,  8, 29,  2, 30,  3,
                                    31,  1, 32,  0, 33, 34,
                                    32,  0, 33,  1, 34,  2, 35, 36,          // 37 -> 74
                                     3, 30,  4, 31,  5, 29,  6, 28,
                                     7,  8, 21,  9, 22, 10, 20, 11,
                                    19, 12, 13, 25, 14, 26, 27, 15,
                                    24, 16, 23, 17, 18,  4, 19,  5,
                                    20,  6, 21,  7,  8, 22,  2, 23,
                                     3, 24,  1, 25,  0, 26, 27, 14,
                                    28, 15, 29, 16, 30, 17, 18, 31,
                                    11, 32, 12, 13, 33, 10, 34,  9,
                                    35, 36,
                                    22,  0, 23,  1, 21,  2, 20,  3,          // 39 -> 78
                                     4, 26,  5, 27, 28,  6, 25,  7,
                                    24,  8,  9, 31, 10, 32, 33, 11,
                                    30, 12, 29, 13, 14, 36, 15, 37,
                                    38, 16, 35, 17, 34, 18, 19, 15,
                                    20, 16, 21, 17, 22, 18, 19, 23,
                                    12, 24, 13, 14, 25, 11, 26, 10,
                                    27, 28,  2, 29,  3,  4, 30,  1,
                                    31,  0, 32, 33,  7, 34,  8,  9,
                                    35,  6, 36,  5, 37, 38,
                                    33,  0, 34, 35,  1, 32,  2, 31,          // 41 -> 82
                                     3,  4, 38,  5, 39, 40,  6, 37,
                                     7, 36,  8,  9, 28, 10, 29, 30,
                                    11, 27, 12, 26, 13, 14, 22, 15,
                                    21, 16, 17, 24, 25, 18, 23, 19,
                                    20,  2, 21,  3,  4, 22,  1, 23,
                                     0, 24, 25,  7, 26,  8,  9, 27,
                                     6, 28,  5, 29, 30, 12, 31, 13,
                                    14, 32, 11, 33, 10, 34, 35, 18,
                                    36, 19, 20, 37, 16, 17, 38, 15,
                                    39, 40,
                                    29,  0, 30, 31,  1, 28,  2, 27,          // 43 -> 86
                                     3,  4, 23,  5, 22,  6,  7, 25,
                                    26,  8, 24,  9, 10, 40, 11, 41,
                                    42, 12, 38, 39, 13, 37, 14, 15,
                                    33, 16, 32, 17, 18, 35, 36, 19,
                                    34, 20, 21, 13, 22, 14, 15, 23,
                                    12, 24, 11, 25, 26, 19, 27, 20,
                                    21, 28, 17, 18, 29, 16, 30, 31,
                                     8, 32,  9, 10, 33,  6,  7, 34,
                                     5, 35, 36,  1, 37,  0, 38, 39,
                                     3,  4, 40,  2, 41, 42,
                                    42,  0, 43, 44,  1, 40, 41,  2,          // 45 -> 90
                                    39,  3,  4, 35,  5, 34,  6,  7,
                                    37, 38,  8, 36,  9, 10, 24, 11,
                                    23, 12, 13, 26, 27, 14, 25, 15,
                                    16, 29, 30, 17, 28, 18, 19, 32,
                                    33, 20, 31, 21, 22,  8, 23,  9,
                                    10, 24,  6,  7, 25,  5, 26, 27,
                                     1, 28,  0, 29, 30,  3,  4, 31,
                                     2, 32, 33, 20, 34, 21, 22, 35,
                                    18, 19, 36, 17, 37, 38, 12, 13,
                                    39, 11, 40, 41, 15, 16, 42, 14,
                                    43, 44,
                                    25,  0, 24,  1,  2, 27, 28,  3,          // 47 -> 94
                                    26,  4,  5, 30, 31,  6, 29,  7,
                                     8, 33, 34,  9, 32, 10, 11, 36,
                                    37, 12, 35, 13, 14, 39, 40, 15,
                                    38, 16, 17, 42, 43, 18, 41, 19,
                                    20, 45, 46, 21, 44, 22, 23, 21,
                                    24, 22, 23, 25, 19, 20, 26, 18,
                                    27, 28, 13, 14, 29, 12, 30, 31,
                                    16, 17, 32, 15, 33, 34,  1,  2,
                                    35,  0, 36, 37,  4,  5, 38,  3,
                                    39, 40,  7,  8, 41,  6, 42, 43,
                                    10, 11, 44,  9, 45, 46,
                                    38, 39,  0, 37,  1,  2, 41, 42,          // 49 -> 98
                                     3, 40,  4,  5, 44, 45,  6, 43,
                                     7,  8, 47, 48,  9, 46, 10, 11,
                                    32, 33, 12, 31, 13, 14, 35, 36,
                                    15, 34, 16, 17, 29, 30, 18, 28,
                                    19, 20, 25, 21, 22, 26, 27, 23,
                                    24,  1,  2, 25,  0, 26, 27,  4,
                                     5, 28,  3, 29, 30,  7,  8, 31,
                                     6, 32, 33, 10, 11, 34,  9, 35,
                                    36, 13, 14, 37, 12, 38, 39, 16,
                                    17, 40, 15, 41, 42, 19, 20, 43,
                                    18, 44, 45, 23, 24, 46, 21, 22,
                                    47, 48,
                                    33, 34,  0, 32,  1,  2, 36, 37,          // 51 -> 102
                                     3, 35,  4,  5, 30, 31,  6, 29,
                                     7,  8, 26,  9, 10, 27, 28, 11,
                                    12, 45, 46, 13, 44, 14, 15, 49,
                                    50, 16, 47, 48, 17, 18, 42, 43,
                                    19, 41, 20, 21, 38, 22, 23, 39,
                                    40, 24, 25, 14, 15, 26, 13, 27,
                                    28, 17, 18, 29, 16, 30, 31, 20,
                                    21, 32, 19, 33, 34, 24, 25, 35,
                                    22, 23, 36, 37,  7,  8, 38,  6,
                                    39, 40, 11, 12, 41,  9, 10, 42,
                                    43,  4,  5, 44,  3, 45, 46,  0,
                                    47, 48,  1,  2, 49, 50,
                                    47, 48,  0, 46,  1,  2, 51, 52,          // 53 -> 106
                                     3, 49, 50,  4,  5, 44, 45,  6,
                                    43,  7,  8, 40,  9, 10, 41, 42,
                                    11, 12, 31, 32, 13, 30, 14, 15,
                                    27, 16, 17, 28, 29, 18, 19, 38,
                                    39, 20, 36, 37, 21, 22, 33, 23,
                                    24, 34, 35, 25, 26,  7,  8, 27,
                                     6, 28, 29, 11, 12, 30,  9, 10,
                                    31, 32,  4,  5, 33,  3, 34, 35,
                                     0, 36, 37,  1,  2, 38, 39, 21,
                                    22, 40, 20, 41, 42, 25, 26, 43,
                                    23, 24, 44, 45, 18, 19, 46, 16,
                                    17, 47, 48, 13, 49, 50, 14, 15,
                                    51, 52,
                                    32, 33,  0, 31,  1,  2, 28,  3,          // 55 -> 110
                                     4, 29, 30,  5,  6, 39, 40,  7,
                                    37, 38,  8,  9, 34, 10, 11, 35,
                                    36, 12, 13, 46, 47, 14, 44, 45,
                                    15, 16, 41, 17, 18, 42, 43, 19,
                                    20, 53, 54, 21, 51, 52, 22, 23,
                                    48, 24, 25, 49, 50, 26, 27, 22,
                                    23, 28, 21, 29, 30, 26, 27, 31,
                                    24, 25, 32, 33, 19, 20, 34, 17,
                                    18, 35, 36, 14, 37, 38, 15, 16,
                                    39, 40,  5,  6, 41,  3,  4, 42,
                                    43,  0, 44, 45,  1,  2, 46, 47,
                                    12, 13, 48, 10, 11, 49, 50,  7,
                                    51, 52,  8,  9, 53, 54,
                                    48, 49,  0, 46, 47,  1,  2, 43,          // 57 -> 114
                                     3,  4, 44, 45,  5,  6, 55, 56,
                                     7, 53, 54,  8,  9, 50, 10, 11,
                                    51, 52, 12, 13, 41, 42, 14, 39,
                                    40, 15, 16, 36, 17, 18, 37, 38,
                                    19, 20, 29, 21, 22, 30, 31, 23,
                                    24, 32, 33, 25, 26, 34, 35, 27,
                                    28,  5,  6, 29,  3,  4, 30, 31,
                                     0, 32, 33,  1,  2, 34, 35, 12,
                                    13, 36, 10, 11, 37, 38,  7, 39,
                                    40,  8,  9, 41, 42, 19, 20, 43,
                                    17, 18, 44, 45, 14, 46, 47, 15,
                                    16, 48, 49, 27, 28, 50, 25, 26,
                                    51, 52, 21, 22, 53, 54, 23, 24,
                                    55, 56,
                                    42, 43,  0, 40, 41,  1,  2, 37,          // 59 -> 118
                                     3,  4, 38, 39,  5,  6, 30,  7,
                                     8, 31, 32,  9, 10, 33, 34, 11,
                                    12, 35, 36, 13, 14, 57, 58, 15,
                                    55, 56, 16, 17, 51, 52, 18, 19,
                                    53, 54, 20, 21, 44, 22, 23, 45,
                                    46, 24, 25, 47, 48, 26, 27, 49,
                                    50, 28, 29, 20, 21, 30, 18, 19,
                                    31, 32, 15, 33, 34, 16, 17, 35,
                                    36, 28, 29, 37, 26, 27, 38, 39,
                                    22, 23, 40, 41, 24, 25, 42, 43,
                                    13, 14, 44, 11, 12, 45, 46,  7,
                                     8, 47, 48,  9, 10, 49, 50,  0,
                                    51, 52,  1,  2, 53, 54,  3,  4,
                                    55, 56,  5,  6, 57, 58,
                                    59, 60,  0, 57, 58,  1,  2, 53,          // 61 -> 122
                                    54,  3,  4, 55, 56,  5,  6, 46,
                                     7,  8, 47, 48,  9, 10, 49, 50,
                                    11, 12, 51, 52, 13, 14, 31, 15,
                                    16, 32, 33, 17, 18, 34, 35, 19,
                                    20, 36, 37, 21, 22, 38, 39, 23,
                                    24, 40, 41, 25, 26, 42, 43, 27,
                                    28, 44, 45, 29, 30, 13, 14, 31,
                                    11, 12, 32, 33,  7,  8, 34, 35,
                                     9, 10, 36, 37,  0, 38, 39,  1,
                                     2, 40, 41,  3,  4, 42, 43,  5,
                                     6, 44, 45, 29, 30, 46, 27, 28,
                                    47, 48, 23, 24, 49, 50, 25, 26,
                                    51, 52, 15, 16, 53, 54, 17, 18,
                                    55, 56, 19, 20, 57, 58, 21, 22,
                                    59, 60,
                                    32,  0,  1, 33, 34,  2,  3, 35,          // 63 -> 126
                                    36,  4,  5, 37, 38,  6,  7, 39,
                                    40,  8,  9, 41, 42, 10, 11, 43,
                                    44, 12, 13, 45, 46, 14, 15, 47,
                                    48, 16, 17, 49, 50, 18, 19, 51,
                                    52, 20, 21, 53, 54, 22, 23, 55,
                                    56, 24, 25, 57, 58, 26, 27, 59,
                                    60, 28, 29, 61, 62, 30, 31, 30,
                                    31, 32, 28, 29, 33, 34, 24, 25,
                                    35, 36, 26, 27, 37, 38, 16, 17,
                                    39, 40, 18, 19, 41, 42, 20, 21,
                                    43, 44, 22, 23, 45, 46,  0,  1,
                                    47, 48,  2,  3, 49, 50,  4,  5,
                                    51, 52,  6,  7, 53, 54,  8,  9,
                                    55, 56, 10, 11, 57, 58, 12, 13,
                                    59, 60, 14, 15, 61, 62,
                                    49, 50,  0,  1, 51, 52,  2,  3,          // 65 -> 130
                                    53, 54,  4,  5, 55, 56,  6,  7,
                                    57, 58,  8,  9, 59, 60, 10, 11,
                                    61, 62, 12, 13, 63, 64, 14, 15,
                                    41, 42, 16, 17, 43, 44, 18, 19,
                                    45, 46, 20, 21, 47, 48, 22, 23,
                                    37, 38, 24, 25, 39, 40, 26, 27,
                                    35, 36, 28, 29, 33, 34, 30, 31,
                                    32,  0,  1, 33, 34,  2,  3, 35,
                                    36,  4,  5, 37, 38,  6,  7, 39,
                                    40,  8,  9, 41, 42, 10, 11, 43,
                                    44, 12, 13, 45, 46, 14, 15, 47,
                                    48, 16, 17, 49, 50, 18, 19, 51,
                                    52, 20, 21, 53, 54, 22, 23, 55,
                                    56, 24, 25, 57, 58, 26, 27, 59,
                                    60, 28, 29, 61, 62, 30, 31, 32,
                                    63, 64,
                                    42, 43,  0,  1, 44, 45,  2,  3,          // 67 -> 134
                                    46, 47,  4,  5, 48, 49,  6,  7,
                                    38, 39,  8,  9, 40, 41, 10, 11,
                                    36, 37, 12, 13, 34, 35, 14, 15,
                                    16, 58, 59, 17, 18, 60, 61, 19,
                                    20, 62, 63, 21, 22, 64, 65, 66,
                                    23, 24, 54, 55, 25, 26, 56, 57,
                                    27, 28, 52, 53, 29, 30, 50, 51,
                                    31, 32, 33, 17, 18, 34, 35, 19,
                                    20, 36, 37, 21, 22, 38, 39, 23,
                                    24, 40, 41, 25, 26, 42, 43, 27,
                                    28, 44, 45, 29, 30, 46, 47, 31,
                                    32, 33, 48, 49,  8,  9, 50, 51,
                                    10, 11, 52, 53, 12, 13, 54, 55,
                                    14, 15, 16, 56, 57,  4,  5, 58,
                                    59,  6,  7, 60, 61,  2,  3, 62,
                                    63,  0,  1, 64, 65, 66};

//*****************************************************************************

// CC de-rate matching remapping index table

// This table contains the relative starting positions in the above table
// (CC_RM_Remapping_Table[]) that correspond to each of the block sizes.
// Note, some block sizes are handled correctly by the X7 Turner device and
// therefore do not require soft decision remapping.  For these block sizes
// -1 is used as the starting position.

                                                      // K (Block size)
                                                      // --------------
S16 CC_RM_Remapping_Start_Table[49] = {   0,          // 20
                                         40,          // 21
                                         82,          // 22
                                        126,          // 23
                                        172,          // 24
                                        220,          // 25
                                        270,          // 26
                                        322,          // 27
                                        376,          // 28
                                        432,          // 29
                                        490,          // 30
                                        550,          // 31
                                         -1,          // 32
                                        612,          // 33
                                         -1,          // 34
                                        678,          // 35
                                         -1,          // 36
                                        748,          // 37
                                         -1,          // 38
                                        822,          // 39
                                         -1,          // 40
                                        900,          // 41
                                         -1,          // 42
                                        982,          // 43
                                         -1,          // 44
                                       1068,          // 45
                                         -1,          // 46
                                       1158,          // 47
                                         -1,          // 48
                                       1252,          // 49
                                         -1,          // 50
                                       1350,          // 51
                                         -1,          // 52
                                       1452,          // 53
                                         -1,          // 54
                                       1558,          // 55
                                         -1,          // 56
                                       1668,          // 57
                                         -1,          // 58
                                       1782,          // 59
                                         -1,          // 60
                                       1900,          // 61
                                         -1,          // 62
                                       2022,          // 63
                                         -1,          // 64
                                       2148,          // 65
                                         -1,          // 66
                                       2278,          // 67
                                         -1};         // 68

//*****************************************************************************

//*****************************************************************************
//**********                          ARRAYS                         **********
//*****************************************************************************

S8  tmpsftdeca[3*P_MAX_K];              // Zero-filled soft decision input array

S8  tmpsftdecb[3*P_MAX_K*2];            // Re-mapped soft decision input array

U8  tmpout[(P_MAX_K+P_MAX_K+5) >> 3];   // Decoder output array

//*****************************************************************************

#endif
//X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7

/******************************************************************************
 bs_rx_cc_cont_chan_coding (LTE Receive Channel Coding Processing For
 Convolutionally Encoded Control Channels
 ******************************************************************************

 Function bs_rx_cc_cont_chan_coding implements the receive channel coding
 processing for convolutionally encoded uplink control channels.  The
 functional calling format is shown below.

 bs_rx_cc_cont_chan_coding(in, out, nin, nout, *stat)

 Where:  in =    Input 8-bit 2-s complement soft-decison array organized as
 bytes.
 out =   Output data array organized as bytes.  Data bits are
 packed into bytes.  Bit order is msb first, lsb last.
 Note, out must include an extra element to store 8-bit CRC.
 nin =   Length of soft-decision input array.
 nout =  Length of output data array in bits.
 stat =  Reported CRC status. 0=CRC error.  1=CRC ok.

 The entire channel coding process is defined in the 3GPP (LTE) standard TS
 36.212 Section 5.2.2.6 and is described below.

 1) Compute and append 8-bit CRC to input control bits.

 2) Encode bits using 1/3 rate convolutional encoder to generate 3 output
 bit streams.

 3) Sub-block interleave each of the 3 encoder output bit sterams.

 4) Combine the 3 sub-block interleaved outputs to for one circular buffer
 and then select bits from circular buffer for modulation and transmission.

 Function bs_rx_cc_cont_chan_coding performs the inverse of this processing
 as described below.

 1) Combine and re-map soft-decisions for received data bits to reconstruct
 original transmitted circular buffer organization.

 2) Sub-block de-interleave the soft-decisons for each of the 3 encoded
 streams.

 3) Decode data bits and CRC using 1/3 rate convolutional soft-decision
 decoder.

 4) Re-compute and verify CRC and report results.

 *****************************************************************************/

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// This section defines the function called by the FEC driver when the Rx CC
// decoding operations have been completed.

#ifndef _WIN32

static volatile U32 fec_rx_cc_stat; // Completion status flag

void FecRxCCDone(LPVOID a) // Function
{
    fec_rx_cc_stat = 1; // Set status flag
    //printf("Running FecRxCCDone\n");
    return;
}

#endif  //ifndef _WIN32

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//-------------------------------------------------------------------------------------------
/** @brief LTE receive CC encoded control channel coding processing.<BR>
 *  Function bs_rx_cc_cont_chan_coding implements the receive channel coding
 *  processing for convolutionally encoded uplink control channels.
 *
 *  <1> Hardware Resource:
 *  ARM, FEC, MDMA
 *
 *  @param     in      Input 8-bit 2-s complement soft-decison array organized as
 *                     bytes.
 *  @param     out     Output data array organized as bytes.  Data bits are
 *                     packed into bytes.  Bit order is msb first, lsb last.
 *                     Note, out must include an extra element to store 8-bit CRC.
 *  @param     nin     Length of soft-decision input array.
 *  @param     nout    Length of output data array in bits.
 *  @param     stat    Reported CRC status. 0=CRC error.  1=CRC ok.
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
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------
 *  |Block Name          |bs_rx_cc_cont_chan_coding |
 *  -------------------------------------------------
 *  |Input Stream(s)     |in                        |
 *  -------------------------------------------------
 *  |Output Stream(s)    |out                       |
 *  -------------------------------------------------
 *  |Config Parameter(s) |nin,nout                  |
 *  -------------------------------------------------
 *  |Status Parameter(s) |stat                      |
 *  -------------------------------------------------
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
void bs_rx_cc_cont_chan_coding_EVM(S8 in[], U8 out[], S32 nin, S32 nout, U8 *stat)
{

//X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7
#ifndef X7_DEV_ENABLED

    S32 Ncb;
    U8 gencrc;

#ifdef _WIN32
    S8 *DescAdr[1];
    S32 DescLen[1];
#endif

#ifndef _WIN32

    PFEC_UL_FDESC pFecUL;
    volatile PFEC_CONFIG_REGS pFecConfigRegs;
    ULHWCTX hw_ctx =
    { 0 };
    S8 *DescAdr[1];
    S32 DescLen[1];

#endif  //ifndef _WIN32

    //printf("bs_rx_cc_cont_chan_coding nin=%d, nout=%d\n", nin, nout);

    // Compute size of rate matching circular buffer.
    Ncb = 96 * ((nout + 8 + 31) >> 5);

    //WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS
#ifdef _WIN32

    // Run simulation for FEC hardware decoder
    gencrc = fec_decode_hdwr_sim( 0, // Type of encoding or decoding (0=CC, 1=CTC)
        (U16) (nout+8), // Code block size
        0, // Type of CRC
        0, // Number of initial filler bits in code block
        0, // Coding rate
        0, // Number of decoder 1/2 iterations
        0, // CTC decoder automatic stop select
        0, // Decoder bypass
        0, // HARQ soft-combining enable
        (U32) nin, // Rate matching E parameter
        0, // Rate matching k0 parameter
        (U16) Ncb, // Rate matching Ncb parameter
        2, // Number of modulation bits used (2,4,6)
        1, // Bypass channel de-interleaver
        12, // Number of channel interleaver columns
        DescAdr, // Channel de-interleaver descriptor starting addresses
        (S32*)DescLen, // Channel de-interleaver descriptor byte counts
        0, // Channel de-interleaver number of descriptors
        (S8*) in, // Input buffer pointer
        (U8*) out, // Output buffer pointer
        0); // Code block index for debug only

#else   //_WIN32

    // Configure FEC CTC decoder via driver.

    // Allocate temporary space for descriptors via driver
    pFecUL = FecUlAllocDesc(1);

    // Configure FEC control registers.
    pFecConfigRegs = (PFEC_CONFIG_REGS) FEC_CONFIG_REGS_BASE;
    pFecConfigRegs->bp_channel_interleaver = 1;

    // set hardware context
    memset(&hw_ctx, 0, sizeof(hw_ctx));

    hw_ctx.CTC_CCn = 0;
    hw_ctx.CRC_Sel = 0;
    hw_ctx.Modulation = 1;
    hw_ctx.HalfInteration = 8;
    hw_ctx.Bypass_Rate_Match = 0;
    hw_ctx.Rate_Index = 0;
    hw_ctx.Enable_Combine = 0;
    hw_ctx.Bypass_Decode_Enable = 0;
    hw_ctx.Dynamic_Stop_Iteration = 0;
    hw_ctx.BlockSize = nout + 8;
    hw_ctx.Filler = 0;
    hw_ctx.E_Param = nin;
    hw_ctx.LTERateMaching_K0 = 0;

    // Create FEC CTC decoder control descriptor via driver.
    DescAdr[0] = in;
    DescLen[0] = nin;

    FecUlSetHwCtx(pFecUL, hw_ctx);
    FecUlSetData(pFecUL, DescAdr, DescLen, out, ((nout + 15) >> 3));

    // Inform FEC driver to enable CC decoder finished interrupt.
    // FEC driver will call function FecRxCCDone when decoder is finished.
    FecUlSetLastDesc(pFecUL, (LPVOID) FecRxCCDone, (LPVOID) NULL);

    // Initialize decoder finished status flag.
    // Function FecRxCCDone will set status flag to 1 when decoder has finished.
    fec_rx_cc_stat = 0;

    // Run FEC CC decoder via driver and wait until finished.
    FecUlRun(pFecUL);
    while (fec_rx_cc_stat == 0);

    // Set FEC control register back to default setting.
    pFecConfigRegs->bp_channel_interleaver = 0;

#endif //_WIN32
    //WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS*WINDOWS

    // Use received data to re-compute CRC
    calc_gcrc8(out, &gencrc, nout + 8);

    // Initialize output status = 0
    *stat = 0;

    // Set bit0 of status if CRC is correct
    if (gencrc == 0)
        *stat = 1;

    return;

#endif
//X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7

//X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7
#ifdef X7_DEV_ENABLED

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!                      X7 WORKAROUNDS                     !!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    S32 ac, i, j, m, n;
    S32 K, Knew;
    S8  *psft;
    U8  *pout;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    U8 gencrc;

    S8 *DescAdr[1];
    S32 DescLen[1];

#ifndef _WIN32
    PFEC_UL_FDESC pFecUL;
    volatile PFEC_CONFIG_REGS pFecConfigRegs;
    ULHWCTX hw_ctx = { 0 };
#else
    S32 Ncb;
#endif

#ifndef _WIN32
    //uart_printf("in=%x, out=%x, tmpsftdeca=%x, tmpsftdecb=%x, tmpout=%x\n", in, out, tmpsftdeca, tmpsftdecb, tmpout);
#endif

    //printf("bs_rx_cc_cont_chan_coding nin=%d, nout=%d\n", nin, nout);

    // Initialize soft decision input pointer to use original inputs
    psft = in;

    // Initialize decoder output pointer to use original outputs
    pout = out;

    // Compute actual block size = data size + 8 bits parity
    K = nout + 8;

#if 1

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!                      X7 WORKAROUNDS                     !!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// ****************************************************************************
// For function bs_rx_cc_cont_chan_coding parameter nin specifies the number
// of de-rate matching soft decsion inputs (E).  nout specifies the number of
// control field data bits.  However, since an 8-bit parity is always appended
// to the end of the data bits, the actual block size K = nout+8.

// If nin > 3*K, the device will lock up.  These cases require the de-rate
// matching circular buffer to wrap-around and the previously stored soft
// decisions to be combined with the newer ones.  To fix the problem, nin must
// be limited to 3*K and any required soft decision combining must be done
// externally.
// ****************************************************************************

    // If K out of range, then print error message
    if((K > P_MAX_K) || (K < P_MIN_K) )
    {
        printf("bs_rx_cc_cont_chan_coding error! K=%d.  Allowed range is: %d to %d\n", K, P_MIN_K, P_MAX_K);
        return;
    }

    // Using block size (K) read required new block size from table
    Knew = CC_Knew_Table[K-P_MIN_K];

    // Compute 3*K = normal total CC encoder output size
    m = 3*K;

    // Limit soft decision input size to prevent excessive soft decision combining
    if(nin > P_MAX_RIN*m) nin = P_MAX_RIN*m;

    // Compute number of extra inputs that would require soft decision combining
    n = nin - m;

    // If there are any extra inputs, then
    if(n > 0)
    {
        // Initialize soft decision input array index
        j = 0;

        // For each extra input
        for(i = 0; i < n; i++)
        {

            //if( ((in[j] < 0) && (in[m+i] > 0)) || ((in[j] > 0) && (in[m+i] < 0)) )  printf("X7 workaround: soft decision combining error!\n");

            // Combine previous value with new value
            ac = in[j] + in[m+i];

            // Limit summation to 8 bits
            if(ac > 127) ac = 127;
            if(ac < -128) ac = -128;

            // Update previous value
            in[j] = (S8) ac;

            // Increment soft decision input array index
            j = j + 1;
            if(j == m) j = 0;

        }

        //printf("X7 workaround: input soft decisions combined. nin=%d > 3K=%d.  nin has been limited to %d\n", nin, m, m);

        // Limit nin to 3*K
        nin = m;

    }
    // Else if the block size must be increased and there are not enough inputs, then
    else if( (n < 0) && (K != Knew) )
    {
        // Copy input soft decisions to temporary buffer
        for(i = 0; i < nin; i++)
        {
            tmpsftdeca[i] = in[i];
        }
        // Copy 0s into any remaining temporary buffer inputs
        for(i = nin; i < m; i++)
        {
            tmpsftdeca[i] = 0;
        }

        // Set soft decision input pointer to use zero-filled input buffer
        psft = tmpsftdeca;

        //printf("X7 workaround: input soft decisions zero filled. nin=%d, 3K=%d.  %d 0s have been inserted\n", nin, m, m-nin);

    }

// ****************************************************************************
// If K (block size) < 32, then the CC decoder outputs can be wrong.  For
// the LTE CC code the decoder requires 32 additional soft decisions after the
// actual soft decision input array.  These additional inputs are supposed to be
// generated automatically by the FEC hardware.  However, because of a hardware
// bug only K additional inputs are generated, resulting in improper decoding
// in some cases depending on the data.  To fix the problem, K < 32 must be
// changed to 32.

// If K is odd, the device will lock up.  To fix the problem, K=K+1 is
// used for any odd K.

// In both of these cases, the decoder will generate additional output bits and
// will require additional input soft decisions which must be generated
// externally.  Because of the sub-block interleaver/de-interleaver, any
// change in the block size (K) also requires a re-mapping of the input
// soft decision array.  This re-mapping is performed using look-up tables.

// The sub-block interleaving procedure is defined in the LTE standard 3GPP
// TS 36.212.  Some important details are summarized below.

//      1) The CC encoder generates 3 separate output bit streams, each of
//         length K bits, where K is the number of encoder input bits.

//      2) Each of the encoder output streams is interleaved seperately.

//      3) The 3 interleaved output streams are combined sequentially and
//         then loaded into a circular rate matching buffer.

//      4) The final outputs, which are sent to the transmitter, are generated
//         by extracting the required number of bits from this circular buffer
//         starting from the beginning of the buffer.
// ****************************************************************************

    // If new block size is not the same as the original one, then
    if(Knew != K)
    {

        // Read re-mapping table starting index for block size
        n = CC_RM_Remapping_Start_Table[K-P_MIN_K];

        // For each output soft decision
        for(i = 0; i < Knew; i++)
        {
            // Use next table entry to re-map soft decision to correct output element
            j = CC_RM_Remapping_Table[n + i];

            // Generate 1st Knew element group by re-mapping 1st K element input group
            tmpsftdecb[i] = psft[j];

            // Generate 2nd Knew element group by re-mapping 2nd K element input group
            tmpsftdecb[i+Knew] = psft[j+K];

            // Generate 3rd Knew element group by re-mapping 3rd K element input group
            tmpsftdecb[i+Knew+Knew] = psft[j+K+K];
        }

        //printf("X7 workaround: input soft decisions remapped. K=%d changed.  New K=%d, nin=%d\n", K, Knew, 3*Knew);

        // Change K and nin to use new block size
        K = Knew;
        nin = 3*Knew;

        // Change soft decision input pointer to use new re-mapped buffer
        psft = tmpsftdecb;

        // Change decoder output pointer to accommodate extra output bits
        pout = tmpout;

    }

#ifndef _WIN32
    // If ARM touched input buffer, then flush it out of ARM cache
    if( (n > 0) || (Knew != K) ) MxCacheFlush(psft, nin);
#endif


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif

#ifdef _WIN32

    // Compute size of rate matching circular buffer.
    Ncb = 96 * ((K + 31) >> 5);

    // Run simulation for FEC hardware decoder
    gencrc = fec_decode_hdwr_sim( 0, // Type of encoding or decoding (0=CC, 1=CTC)
        (U16) (K), // Code block size
        0, // Type of CRC
        0, // Number of initial filler bits in code block
        0, // Coding rate
        0, // Number of decoder 1/2 iterations
        0, // CTC decoder automatic stop select
        0, // Decoder bypass
        0, // HARQ soft-combining enable
        (U32) nin, // Rate matching E parameter
        0, // Rate matching k0 parameter
        (U16) Ncb, // Rate matching Ncb parameter
        2, // Number of modulation bits used (2,4,6)
        1, // Bypass channel de-interleaver
        12, // Number of channel interleaver columns
        DescAdr, // Channel de-interleaver descriptor starting addresses
        (S32*)DescLen, // Channel de-interleaver descriptor byte counts
        0, // Channel de-interleaver number of descriptors
        (S8*) psft, // Input buffer pointer
        (U8*) pout, // Output buffer pointer
        0); // Code block index for debug only

#else   //_WIN32

    // Configure FEC CTC decoder via driver.

    // Allocate temporary space for descriptors via driver
    pFecUL = FecUlAllocDesc(1);

    // Configure FEC control registers.
    pFecConfigRegs = (PFEC_CONFIG_REGS) FEC_CONFIG_REGS_BASE;
    pFecConfigRegs->bp_channel_interleaver = 1;

    // set hardware context
    memset(&hw_ctx, 0, sizeof(hw_ctx));

    hw_ctx.CTC_CCn = 0;
    hw_ctx.CRC_Sel = 0;
    hw_ctx.Modulation = 1;
    hw_ctx.HalfInteration = 8;
    hw_ctx.Bypass_Rate_Match = 0;
    hw_ctx.Rate_Index = 0;
    hw_ctx.Enable_Combine = 0;
    hw_ctx.Bypass_Decode_Enable = 0;
    hw_ctx.Dynamic_Stop_Iteration = 0;
    hw_ctx.BlockSize = K;
    hw_ctx.Filler = 0;
    hw_ctx.E_Param = nin;
    hw_ctx.LTERateMaching_K0 = 0;

    // Create FEC CTC decoder control descriptor via driver.
    DescAdr[0] = psft;
    DescLen[0] = nin;

    FecUlSetHwCtx(pFecUL, hw_ctx);
    FecUlSetData(pFecUL, (LPVOID *)DescAdr, (UINT32 *)DescLen, pout, ((K + 7) >> 3));

    // Inform FEC driver to enable CC decoder finished interrupt.
    // FEC driver will call function FecRxCCDone when decoder is finished.
    FecUlSetLastDesc(pFecUL, FecRxCCDone, (LPVOID) NULL);

    // Initialize decoder finished status flag.
    // Function FecRxCCDone will set status flag to 1 when decoder has finished.
    fec_rx_cc_stat = 0;

    // Run FEC CC decoder via driver and wait until finished.
    FecUlRun(pFecUL);
    while (fec_rx_cc_stat == 0);

    // Set FEC control register back to default setting.
    pFecConfigRegs->bp_channel_interleaver = 0;

#endif //_WIN32

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!                      X7 WORKAROUNDS                     !!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#if 1

    // If decoder was re-programmed to generate extra outputs
    if(pout != out)
    {
        // For each required output byte
        for(i = 0; i < ((nout+15) >> 3); i++)
        {

            // Copy decoder output array to real output array
            out[i] = pout[i];

        }
    }

#endif

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Use received data to re-compute CRC
    calc_gcrc8(out, &gencrc, nout + 8);

    // Initialize output status = 0
    *stat = 0;

    // Set bit0 of status if CRC is correct
    if (gencrc == 0) *stat = 1;

    //printf("bs_rx_cc_cont_chan_coding: crc status = %d\n", *stat);

    //printf("bs_rx_cc_cont_chan_coding: out[]: %x %x %x %x %x %x %x %x\n", out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);

    //if(*stat == 0)i = getchar();

    return;

#endif
//X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7X7

}

#ifdef _WIN32

/******************************************************************************
 **********      Turner FEC Decoder Hardware Simulation Function      **********
 *******************************************************************************

 Function fec_decode_hdwr_sim simulates the FEC decoder hardware in the Turner
 device.

 ******************************************************************************/

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

//*****************************************************************************
//**********                  MISCELLANEOUS ARRAYS                   **********
//*****************************************************************************

//*****************************************************************************
//**********            TURBO CONCEPT CTC DECODER ARRAYS             **********
//*****************************************************************************

S32 tc_dechalfit_done, tc_deccrc_ok, tc_nout, tc_decber;

S32 tc_decneb[33]; // Decoder number of bit errors per
// each 1/2 iteration

U8 tc_decnullB[1]; // Decoder null byte array

S16 tc_decin[P_MAX_SFDEC_LEN]; // Input soft decision array

U8 tc_decout[6144]; // Output bit array

//*****************************************************************************

//-------------------------------------------------------------------------------------------
/** @brief Turner FEC decoder hardware simulation<BR>
 *  Function fec_decode_hdwr_sim simulates the FEC decoder hardware in the Turner device.
 *
 *  <1> Hardware Resource:
 *  FEC, ARM
 *
 *  @param   CodingType =   Type of decoding (0=CC, 1=CTC).
 *  @param   BlockSize  =   Code block size.
 *  @param   CrcSelector =  Type of CRC (0=none, 1=gcrc24A, 2=gcrc24b).
 *  @param   FillerBits =   Number of initial filler bits in code block.
 *  @param   CodingRate =   Coding rate (not currently used).
 *  @param   HalfIterations = Number of CTC decoder 1/2 iterations.
 *  @param   DynamicStop =  CTC decoder auto stop select.
 *  @param   BpDecoder  =   Bypass decoder (0=no, 1=yes).
 *  @param   EnableCombine = Enable HARQ combining (0=no, 1=yes).
 *  @param   RateMatchingE = Rate matching E parameter.
 *  @param   RateMatchingk0 = Rate matching k0 parameter.
 *  @param   RateMatchingNcb = Rate matching Ncb parameter.
 *  @param   Qm =           Number of modulation bits (2, 4, or 6).
 *  @param   BpChanInt =    Bypass channel de-interleaver (0=no, 1=yes).
 *  @param   ColInt =       Number of channel de-interleaver columns (12 or 14).
 *  @param   *DescAdr[] =   Channel de-interleaver descriptor array of starting addresses.
 *  @param   DescLen[] =    Channel de-interleaver descriptor array of byte counts.
 *  @param   DescN =        Channel de-interleaver number of descriptors.
 *  @param   In[] =         Input array.
 *  @param   Out[] =        Output array.
 *  @return  None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CodingType     |               |input        |1              |Real          |(0:16:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Blocksize      |               |input        |1              |Real          |(0:16:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CrcSelector    |               |input        |1              |Real          |(0:16:0)  |{0, 1, 2}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |FillerBits     |               |input        |1              |Real          |(0:16:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CodingRate     |               |input        |1              |Real          |(0:16:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |HalfIterations |               |input        |1              |Real          |(0:16:0)  |[1:32]         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |DynamicStop    |               |input        |1              |Real          |(0:16:0)  |{0}            |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |BpDecoder      |               |input        |1              |Real          |(0:16:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |EnableCombine  |               |input        |1              |Real          |(0:16:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |RateMatchingE  |               |input        |1              |Real          |(0:32:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |RateMatchingk0 |               |input        |1              |Real          |(0:16:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |RateMatchingNcb|               |input        |1              |Real          |(0:16:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(0:16:0)  |{2, 4, 6}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |BpChanInt      |               |input        |1              |Real          |(0:16:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ColInt         |               |input        |1              |Real          |(0:16:0)  |{12, 14}       |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |*DescAdr[]     |               |input        |DescN          |pReal Array   |(0:32:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |DescLen[]      |               |input        |DescN          |pReal Array   |(0:32:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |DescN          |               |input        |1              |Real          |(0:16:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |In[]           |               |input        |RateMatchingE  |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Out[]          |               |output       |               |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------------------------------------------------------------------
 *  |Block Name          |fec_decode_hdwr_sim                                                                          |
 *  --------------------------------------------------------------------------------------------------------------------
 *  |Input Stream(s)     |In                                                                                           |
 *  --------------------------------------------------------------------------------------------------------------------
 *  |Output Stream(s)    |Out                                                                                          |
 *  --------------------------------------------------------------------------------------------------------------------
 *  |Config Parameter(s) |CodingType,Blocksize, CrcSelector,FillerBits,CodingRate,HalfIterations,DynamicStop,BpDecoder,|
 *  |                    |EnableCombine,RateMatchingE,RateMatchingk0,RateMatchingNcb,Qm,BpChanInt,DescAdr,DescLen,DescN|
 *  ------------------------------------------------------------------------------------------------------------------ -
 *  |Status Parameter(s) |none                                                                                         |
 *  ------------------------------------------------------------------------------------------------------------------ -
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function fec_decode_hdwr_sim simulates the FEC decoder hardware in the Turner device.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBC
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.212
 *
 *
 *  \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//-------------------------------------------------------------------------------------------

U8 fec_decode_hdwr_sim( U16 CodingType, // Type of encoding or decoding (0=CC, 1=CTC)
    U16 BlockSize, // Code block size
    U16 CrcSelector, // Type of CRC (0=none, 1=gcrc24A, 2=gcrc24b)
    U16 FillerBits, // Number of initial filler bits in code block
    U16 CodingRate, // Coding rate
    U16 HalfIterations, // Number of decoder 1/2 iterations
    U16 DynamicStopIteration, // CTC decoder automatic stop select
    U16 BpDecoder, // Decoder bypass
    U16 EnableCombine, // HARQ soft-combining enable
    U32 RateMatchingE, // Rate matching E parameter
    U16 RateMatchingk0, // Rate matching k0 parameter
    U16 RateMatchingNcb,// Rate matching Ncb parameter
    U16 Qm, // Number of modulation bits used (2,4,6)
    U16 BpChanInt, // Bypass channel de-interleaver
    U16 ColInt, // Number of channel interleaver columns (12 or 14)
    S8 *DescAdr[], // Channel de-interleaver descriptor starting addresses
    S32 DescLen[], // Channel de-interleaver descriptor byte counts
    U16 DescN, // Channel de-interleaver number of descriptors
    S8 In[], // Input buffer pointer
    U8 Out[], // Output buffer pointer
    U16 CodeBlockIndex) // Code block index for debug only

{

    S16 ndec, k;
    S16 *rateout;
    S32 i, j;

    S32 m, n, nencout;
    S8 *decin;
    U8 *decout;
    U16 *decinmap;

    S8 *dintout;

    //*************************************************************************

    // If channel de-interleaver is not bypassed, then.
    if(BpChanInt == 0)
    {
        S32 nrows, nbytes, adr;
        S8 *dintin, *dintsts;

        //printf("**************************************************************\n");
        //printf("***** Running fec_decode_hdwr_sim channel de-interleaver *****\n");
        //printf(" ColInt=%d, DescN=%d\n", ColInt, DescN);

        // Define number of rows in hardware channel de-interleaver.
        nrows = 1312;
        if(ColInt == 12)nrows = 1536;

        // Define size of hardware channel de-interleaver.
        nbytes = nrows*ColInt;

        //printf(" nrows=%d, nbtyes=%d\n", nrows, nbytes);

        // If invalid number of descriptors, then print error message.
        if( (DescN%ColInt) != 0)
        {
            printf("fec_decode_hdwr_sim channel de-interleaver error!\n");
            printf("  number of descriptors = %d, number of columns = %d\n", DescN, ColInt);
        }

        // Allocate temporary storage for de-interleaver inputs, status and outputs.
        dintin = (S8*) MemAlloc(nbytes*sizeof(S8), DDR_MEM, 0);
        dintsts = (S8*) MemAlloc(nbytes*sizeof(S8), DDR_MEM, 0);
        dintout = (S8*) MemAlloc(RateMatchingE*sizeof(S8), DDR_MEM, 0);

        // Initialize de-interleaver outputs to 0.
        for(i = 0; i < (S32)RateMatchingE; i++)
        {
            dintout[i] = 0;
        }

        // Initialize descriptor index.
        k = 0;

        // Initialize de-interleaver output index.
        m = 0;

        descloop:

        // Initialize de-interleaver element status to empty.
        for(i = 0; i < nbytes; i++)
        {
            dintsts[i] = 0;
        }

        // For each de-interleaver column.
        for(i = 0; i < ColInt; i++)
        {
            // Initialize de-interleaver element index.
            n = i*Qm;

            // Read next descriptor input starting address.
            decin = DescAdr[k];

            // If descriptor byte count is too big, then print error message.
            if(DescLen[k] > nrows)
            {
                printf("fec_decode_hdwr_sim channel de-interleaver error!\n");
                printf("DescLen[%d] = %d, maximum = %d\n", k, DescLen[k], nrows);
            }

            // For each input soft-decision.
            for(j = 0; j < DescLen[k]; j++)
            {
                // Compute channel de-interleaver buffer address.
                adr = n;
                adr = adr + (j%Qm);
                adr = adr + ((j/Qm)*ColInt*Qm);

                // Load soft-decision into de-interleaver buffer column.
                dintin[adr] = *decin;

                // Set corresponding element status to indicate not empty.
                dintsts[adr] = 1;

                // Increment soft-decision input address.
                decin = decin + 1;

            }

            // Increment descriptor index.
            k = k + 1;

        }

        // For each de-interleaver buffer element.
        for(i = 0; i < nbytes; i++)
        {
            // If element is not empty, then.
            if(dintsts[i] != 0)
            {
                // Copy element into de-interleaver output array.
                dintout[m] = dintin[i];

                // Increment de-interleaver output index.
                m = m + 1;
            }

        }

        // If another set of descriptors, then continue.
        if(k < DescN) goto descloop;


        // If FEC de-interleaver work-around is not enabled, then
        #if !P_ENB_FEC_DEINT_FIX

        // If incorrect number of de-interleaver outputs, then print error message.
        if(m != (S32)RateMatchingE)
        {
            printf("fec_decode_hdwr_sim channel de-interleaver error!\n");
            printf("De-interleaver outputs = %d, required rate matching E = %d\n", m, RateMatchingE);
        }

        #endif

        //for(i = 0; i < m; i++)
        //{
        //    if(In[i] != dintout[i])
        //    {
        //       printf(" Channel de-int output error! In[%d]=%d, dintout[%d]=%d\n", i, In[i], i, dintout[i]);
        //    }
        //}

        // De-allocate de-interleaver input and status storage.
        MemFree(dintin, DDR_MEM);
        MemFree(dintsts, DDR_MEM);

    }

    //*************************************************************************
    // If CTC decoding selected
    if(CodingType == 1)
    {

        // Allocate temporary space for rate matching output array
        rateout = (S16*) MemAlloc(P_MAX_RATEM_LEN*sizeof(S16), DDR_MEM, 0);

        // Determine rate matching used by transmitter
        rx_lte_rate_matching_hdwr(BlockSize, FillerBits, RateMatchingE,
            RateMatchingk0, RateMatchingNcb, rateout);

        // Compute number of decoder inputs
        ndec = 3*32*((BlockSize + 4 + 31)/32);

        // If HARQ combining disabled
        if(EnableCombine == 0)
        {

            // Initialize decoder's soft decision input array
            CLEAR(tc_decin, ndec);

        }

        // Do for each input soft decision
        for(i = 0; i < (S32)RateMatchingE; i++)
        {
            // Read next soft decision array index from rate matching array
            j = rateout[i];

            // If array index is invalid, then output error message
            if((j > ndec) || (j < 0))
            {
                printf("!!!!! ERROR: INVALID RATE MATCHING INDEX = %d, MAX ALLOWED = %d !!!!!\n",
                    j, ndec);
            }
            // Else, array index is valid

            else
            {
                // Add old and new values
                // If bypassing channel de-interleaver, then use input array
                if(BpChanInt != 0)
                {
                    k = tc_decin[j] + In[i];
                }
                // Else, use channel de-interleaver outputs

                else
                {
                    k = tc_decin[j] + dintout[i];
                }

                // Limit sum to 8-bit 2-s complement limits
                if(k > 127) k = 127;
                if(k < -128)k = -128;

                // Update soft decision
                tc_decin[j] = k;

            }

        }

        // De-allocate temporary array space
        MemFree(rateout, DDR_MEM);

        // If channel de-interleaver enabled, then de-allocate output space
        if(BpChanInt == 0)
        {
            MemFree(dintout, DDR_MEM);
        }

        // Return if bypass decoder selected
        if(BpDecoder != 0) return (0);

        // Configure other Turbo Concepts decoder parameters
        tc_nout = BlockSize; // output frame size in bits

        // Run Turbo Concepts LTE decoder with sub-block de-interleaver

        tc1700decode(8, // speed (selects throughput level = 4 or 8)
            8, // d_width (number of soft-decision bits. Range = 4:8)
            0, // wimax_use_rate13 (0:1 disables:enables WiMAX 1/3 rate)
            1, // mode (0=WiMAX, 1=LTE 3GPP)
            tc_nout, // k (frame size in bits)
            ndec, // nb_bits_encoded (encoder output frame size in bits)
            CrcSelector, // crc_select (0=none, 1=gcrc24A, 2=gcrc24b)
            0, // rate_index (0=1/3, 2=1/2, 3=2/3, 4=3/4, 6=5/6)
            2, // modulation_index (2=QPSK, 4=16QAM, 6=64QAM.  For WiMAX only)
            HalfIterations, // halfit (number of 1/2 iterations. Range = 1:32)
            DynamicStopIteration, // stop_criterion (0=none, 1=genie, 2=dynamic)
            0, // threshold (contact Turbo Concept for details)
            tc_decin, // *dec_in (soft-decision input array)
            tc_decout, // *dec_out (output data bit array)
            tc_decnullB, // *genie_enc_in (encoder input bit array)
            tc_decneb, // *genie_neb (1/2 iteration bit errors detected array)
            &tc_decber, // *ber (returns BER estimate)
            &tc_dechalfit_done, // *halfit_done (returns last 1/2 iteration performed when
            // stop_criterion > 0)
            &tc_deccrc_ok, // *crc_error (0=no CRC error, 1=CRC error)
            0); // debug_output (0=debug off, 1=generate debug files)

        // Invert CRC24B error status so that 1 means CRC24B check passed
        tc_deccrc_ok = tc_deccrc_ok ^ 1;

        // Compute number of decoded data bits - filler bits
        j = tc_nout - FillerBits;

        // If CRC24B attached to code block, then remove its bits
        if(CrcSelector != 0)j = j - 24;

        // Pack decoder output bits into output byte array
        PACKBB(&tc_decout[FillerBits], Out, j, 0);

        // Return CRC24B status
        return (U8) tc_deccrc_ok;

    }

    //*************************************************************************
    // If CC decoding selected
    if(CodingType == 0)
    {

        // Compute 1/3 rate encoder output length in bits
        nencout = 3*BlockSize;

        // Allocate temporary space for decoder input array
        decin = (S8*) MemAlloc(nencout*sizeof(S8), DDR_MEM, 0);

        // Allocate temporary space for decoder output array
        decout = (U8*) MemAlloc(BlockSize*sizeof(U8), DDR_MEM, 0);

        // Allocate temporary space for rate-matching mapping output array
        decinmap = (U16*) MemAlloc(RateMatchingE*sizeof(U16), DDR_MEM, 0);

        // Run rate matching to determine how soft-decisions map into
        // decoder input array
        CC_Rx_Rate_Matching(BlockSize, RateMatchingE, decinmap, NULL);

        // Clear decoder input soft-decision array
        for(i = 0; i < nencout; i++)
        {
            decin[i] = 0;
        }

        // For each input soft-decision
        for(i = 0; i < (S32)RateMatchingE; i++)
        {
            // Determine which encoder output stream (d0, d1 or d2)
            j = (decinmap[i] >> 14) & 3;

            // Determine relative location within stream
            n = decinmap[i] & 0x3fff;

            // Compute corresponding decoder input array index
            m = n + n + n + j;

            // Add soft-decision to decoder input array element and limit to +-127
            // If bypassing channel de-interleaver, then use input array
            if(BpChanInt != 0)
            {
                j = decin[m] + In[i];
            }
            // Else use channel de-interleaver outputs

            else
            {
                j = decin[m] + dintout[i];
            }
            if(j > 127) j = 127;
            if(j < -127) j = -127;
            decin[m] = (S8) (j);

        }

        // If using Turbo Best CC decoder function (included in Matlab DLL)
#ifdef USETURBOBESTCCDECODER

        // Run Turbo Best CC decoder
        TurboBest_CC_Decoder_1_3(decin, decout, BlockSize, 8, 96);

#else

        // Run MindSpeed 1/3 rate CC soft-decision decoder
        cc_decoder_1_3(decin, decout, (S16) BlockSize, NULL);

#endif

        // Pack decoder output bits into output bytes
        PACKBB(decout, Out, BlockSize, 0);

        // De-allocate temporary space
        MemFree(decin, DDR_MEM);
        MemFree(decout, DDR_MEM);
        MemFree(decinmap, DDR_MEM);

        // If channel de-interleaver enabled, then de-allocate output space
        if(BpChanInt == 0)
        {
            MemFree(dintout, DDR_MEM);
        }

    }

    //*************************************************************************

    return (0);

}

//RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
// If using Rossetti device.
#if USE_ROSSETTI_DEV

// Define HARQ combining decoder input array.
S8 harq_decin[P_MAX_SFDEC_LEN]; // HARQ combined soft decision array

//-------------------------------------------------------------------------------------------
/** @brief Turner FEC decoder hardware simulation<BR>
 *  Function fec_decode_hdwr_sim simulates the FEC decoder hardware in the Turner device.
 *
 *  <1> Hardware Resource:
 *  FEC, ARM
 *
 *  @param   CodingType =   Type of decoding (0=CC, 1=CTC).
 *  @param   BlockSize  =   Code block size.
 *  @param   CrcSelector =  Type of CRC (0=none, 1=gcrc24A, 2=gcrc24b).
 *  @param   FillerBits =   Number of initial filler bits in code block.
 *  @param   CodingRate =   Coding rate (not currently used).
 *  @param   HalfIterations = Number of CTC decoder 1/2 iterations.
 *  @param   DynamicStop =  CTC decoder auto stop select.
 *  @param   BpDecoder  =   Bypass decoder (0=no, 1=yes).
 *  @param   EnableCombine = Enable HARQ combining (0=no, 1=yes).
 *  @param   RateMatchingE = Rate matching E parameter.
 *  @param   RateMatchingk0 = Rate matching k0 parameter.
 *  @param   RateMatchingNcb = Rate matching Ncb parameter.
 *  @param   Qm =           Number of modulation bits (2, 4, or 6).
 *  @param   BpChanInt =    Bypass channel de-interleaver (0=no, 1=yes).
 *  @param   ColInt =       Number of channel de-interleaver columns (12 or 14).
 *  @param   *DescAdr[] =   Channel de-interleaver descriptor array of starting addresses.
 *  @param   DescLen[] =    Channel de-interleaver descriptor array of byte counts.
 *  @param   DescN =        Channel de-interleaver number of descriptors.
 *  @param   In[] =         Input array.
 *  @param   Out[] =        Output array.
 *  @return  None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CodingType     |               |input        |1              |Real          |(0:16:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Blocksize      |               |input        |1              |Real          |(0:16:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CrcSelector    |               |input        |1              |Real          |(0:16:0)  |{0, 1, 2}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |FillerBits     |               |input        |1              |Real          |(0:16:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |CodingRate     |               |input        |1              |Real          |(0:16:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |HalfIterations |               |input        |1              |Real          |(0:16:0)  |[1:32]         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |DynamicStop    |               |input        |1              |Real          |(0:16:0)  |{0}            |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |BpDecoder      |               |input        |1              |Real          |(0:16:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |EnableCombine  |               |input        |1              |Real          |(0:16:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |RateMatchingE  |               |input        |1              |Real          |(0:32:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |RateMatchingk0 |               |input        |1              |Real          |(0:16:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |RateMatchingNcb|               |input        |1              |Real          |(0:16:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(0:16:0)  |{2, 4, 6}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |BpChanInt      |               |input        |1              |Real          |(0:16:0)  |{0, 1}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ColInt         |               |input        |1              |Real          |(0:16:0)  |{12, 14}       |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |*DescAdr[]     |               |input        |DescN          |pReal Array   |(0:32:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |DescLen[]      |               |input        |DescN          |pReal Array   |(0:32:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |DescN          |               |input        |1              |Real          |(0:16:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |In[]           |               |input        |RateMatchingE  |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Out[]          |               |output       |               |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------------------------------------------------------------------
 *  |Block Name          |fec_decode_hdwr_sim                                                                          |
 *  --------------------------------------------------------------------------------------------------------------------
 *  |Input Stream(s)     |In                                                                                           |
 *  --------------------------------------------------------------------------------------------------------------------
 *  |Output Stream(s)    |Out                                                                                          |
 *  --------------------------------------------------------------------------------------------------------------------
 *  |Config Parameter(s) |CodingType,Blocksize, CrcSelector,FillerBits,CodingRate,HalfIterations,DynamicStop,BpDecoder,|
 *  |                    |EnableCombine,RateMatchingE,RateMatchingk0,RateMatchingNcb,Qm,BpChanInt,DescAdr,DescLen,DescN|
 *  ------------------------------------------------------------------------------------------------------------------ -
 *  |Status Parameter(s) |none                                                                                         |
 *  ------------------------------------------------------------------------------------------------------------------ -
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function fec_decode_hdwr_sim_rossetti simulates the FEC decoder hardware in the Rossetti device.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBC
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.212
 *
 *
 *  \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//-------------------------------------------------------------------------------------------

U8 fec_decode_hdwr_sim_rossetti(
    U16 CodingType, // Type of encoding or decoding (0=CC, 1=CTC)
    U16 BlockSize, // Code block size
    U16 CrcSelector, // Type of CRC (0=none, 1=gcrc24A, 2=gcrc24b)
    U16 FillerBits, // Number of initial filler bits in code block
    U16 CodingRate, // Coding rate
    U16 HalfIterations, // Number of decoder 1/2 iterations
    U16 DynamicStopIteration, // CTC decoder automatic stop select
    U16 BpDecoder, // Decoder bypass
    U16 EnableCombine, // HARQ soft-combining enable
    U32 RateMatchingE, // Rate matching E parameter
    U16 RateMatchingk0, // Rate matching k0 parameter
    U16 RateMatchingNcb,// Rate matching Ncb parameter
    U16 Qm, // Number of modulation bits used (2,4,6)
    U16 BpChanInt, // Bypass channel de-interleaver
    U16 ColInt, // Number of channel interleaver columns (12 or 14)
    S8 *DescAdr[], // Channel de-interleaver descriptor starting addresses
    S32 DescLen[], // Channel de-interleaver descriptor byte counts
    U16 DescN, // Channel de-interleaver number of descriptors
    S8 In[], // Input buffer pointer
    U8 Out[], // Output buffer pointer
    U16 CodeBlockIndex, // Code block index for debug only
    U8 HarqWeight) // HARQ combining weight

{

    S16 ndec, k;
    S16 *rateout;
    S32 i, j, ac;

    S32 m, n, nencout;
    S8 *decin;
    U8 *decout;
    U16 *decinmap;

    S8 *dintout;

    //*************************************************************************

    // If channel de-interleaver is not bypassed, then.
    if(BpChanInt == 0)
    {
        S32 nrows, nbytes, adr;
        S8 *dintin, *dintsts;

        //printf("**************************************************************\n");
        //printf("***** Running fec_decode_hdwr_sim channel de-interleaver *****\n");
        //printf(" ColInt=%d, DescN=%d\n", ColInt, DescN);

        // Define number of rows in hardware channel de-interleaver.
        nrows = 1312;
        if(ColInt == 12)nrows = 1536;

        // Define size of hardware channel de-interleaver.
        nbytes = nrows*ColInt;

        //printf(" nrows=%d, nbtyes=%d\n", nrows, nbytes);

        // If invalid number of descriptors, then print error message.
        if( (DescN%ColInt) != 0)
        {
            printf("fec_decode_hdwr_sim channel de-interleaver error!\n");
            printf("  number of descriptors = %d, number of columns = %d\n", DescN, ColInt);
        }

        // Allocate temporary storage for de-interleaver inputs, status and outputs.
        dintin = (S8*) MemAlloc(nbytes*sizeof(S8), DDR_MEM, 0);
        dintsts = (S8*) MemAlloc(nbytes*sizeof(S8), DDR_MEM, 0);
        dintout = (S8*) MemAlloc(RateMatchingE*sizeof(S8), DDR_MEM, 0);

        // Initialize descriptor index.
        k = 0;

        // Initialize de-interleaver output index.
        m = 0;

        descloop:

        // Initialize de-interleaver element status to empty.
        for(i = 0; i < nbytes; i++)
        {
            dintsts[i] = 0;
        }

        // For each de-interleaver column.
        for(i = 0; i < ColInt; i++)
        {
            // Initialize de-interleaver element index.
            n = i*Qm;

            // Read next descriptor input starting address.
            decin = DescAdr[k];

            // If descriptor byte count is too big, then print error message.
            if(DescLen[k] > nrows)
            {
                printf("fec_decode_hdwr_sim channel de-interleaver error!\n");
                printf("DescLen[%d] = %d, maximum = %d\n", k, DescLen[k], nrows);
            }

            // For each input soft-decision.
            for(j = 0; j < DescLen[k]; j++)
            {
                // Compute channel de-interleaver buffer address.
                adr = n;
                adr = adr + (j%Qm);
                adr = adr + ((j/Qm)*ColInt*Qm);

                // Load soft-decision into de-interleaver buffer column.
                dintin[adr] = *decin;

                // Set corresponding element status to indicate not empty.
                dintsts[adr] = 1;

                // Increment soft-decision input address.
                decin = decin + 1;

            }

            // Increment descriptor index.
            k = k + 1;

        }

        // For each de-interleaver buffer element.
        for(i = 0; i < nbytes; i++)
        {
            // If element is not empty, then.
            if(dintsts[i] != 0)
            {
                // Copy element into de-interleaver output array.
                dintout[m] = dintin[i];

                // Increment de-interleaver output index.
                m = m + 1;
            }

        }

        // If another set of descriptors, then continue.
        if(k < DescN) goto descloop;

        // If incorrect number of de-interleaver outputs, then print error message.
        if(m != (S32)RateMatchingE)
        {
            printf("fec_decode_hdwr_sim channel de-interleaver error!\n");
            printf("De-interleaver outputs = %d, required rate matching E = %d\n", m, RateMatchingE);
        }

        //for(i = 0; i < m; i++)
        //{
        //    if(In[i] != dintout[i])
        //    {
        //       printf(" Channel de-int output error! In[%d]=%d, dintout[%d]=%d\n", i, In[i], i, dintout[i]);
        //    }
        //}

        // De-allocate de-interleaver input and status storage.
        MemFree(dintin, DDR_MEM);
        MemFree(dintsts, DDR_MEM);

    }

    //*************************************************************************
    // If CTC decoding selected
    if(CodingType == 1)
    {

        // Allocate temporary space for rate matching output array
        rateout = (S16*) MemAlloc(P_MAX_RATEM_LEN*sizeof(S16), DDR_MEM, 0);

        // Determine rate matching used by transmitter
        rx_lte_rate_matching_hdwr(BlockSize, FillerBits, RateMatchingE,
            RateMatchingk0, RateMatchingNcb, rateout);

        // Compute number of decoder inputs
        ndec = 3*32*((BlockSize + 4 + 31)/32);

        // If HARQ combining disabled
        if(EnableCombine == 0)
        {

            // Initialize HARQ combined soft decision array to 0
            for(i = 0; i < ndec; i++)
            {
                harq_decin[i] = 0;
            }

        }

        // Do for each input soft decision
        for(i = 0; i < (S32)RateMatchingE; i++)
        {
            // Read next soft decision array index from rate matching array
            j = rateout[i];

            // If array index is invalid, then output error message
            if((j > ndec) || (j < 0))
            {
                printf("!!!!! ERROR: INVALID RATE MATCHING INDEX = %d, MAX ALLOWED = %d !!!!!\n",
                    j, ndec);
            }
            // Else, array index is valid

            else
            {
                // Add old and new values
                // If bypassing channel de-interleaver, then use input array
                if(BpChanInt != 0)
                {
                    ac = (harq_decin[j] << 8) + (HarqWeight*In[i]) + 128;
                }
                // Else, use channel de-interleaver outputs

                else
                {
                    ac = (harq_decin[j] << 8) + (HarqWeight*dintout[i]) + 128;
                }

                // Limit sum to 16-bit 2-s complement limits
                if(ac > 32767) ac = 32767;
                if(ac < -32768)ac = -32768;

                // Update 8-bit HARQ combined soft decisions
                harq_decin[j] = (S8) (ac >> 8);

            }

        }

        // De-allocate temporary array space
        MemFree(rateout, DDR_MEM);

        // If channel de-interleaver enabled, then de-allocate output space
        if(BpChanInt == 0)
        {
            MemFree(dintout, DDR_MEM);
        }

        // Return if bypass decoder selected
        if(BpDecoder != 0) return (0);

        // Copy HARQ combined soft decision array into decoder input array
        for(i = 0; i < ndec; i++)
        {
            tc_decin[i] = harq_decin[i];
        }

        // Configure other Turbo Concepts decoder parameters
        tc_nout = BlockSize; // output frame size in bits

        // Run Turbo Concepts LTE decoder with sub-block de-interleaver

        tc1700decode(8, // speed (selects throughput level = 4 or 8)
            8, // d_width (number of soft-decision bits. Range = 4:8)
            0, // wimax_use_rate13 (0:1 disables:enables WiMAX 1/3 rate)
            1, // mode (0=WiMAX, 1=LTE 3GPP)
            tc_nout, // k (frame size in bits)
            ndec, // nb_bits_encoded (encoder output frame size in bits)
            CrcSelector, // crc_select (0=none, 1=gcrc24A, 2=gcrc24b)
            0, // rate_index (0=1/3, 2=1/2, 3=2/3, 4=3/4, 6=5/6)
            2, // modulation_index (2=QPSK, 4=16QAM, 6=64QAM.  For WiMAX only)
            HalfIterations, // halfit (number of 1/2 iterations. Range = 1:32)
            DynamicStopIteration, // stop_criterion (0=none, 1=genie, 2=dynamic)
            0, // threshold (contact Turbo Concept for details)
            tc_decin, // *dec_in (soft-decision input array)
            tc_decout, // *dec_out (output data bit array)
            tc_decnullB, // *genie_enc_in (encoder input bit array)
            tc_decneb, // *genie_neb (1/2 iteration bit errors detected array)
            &tc_decber, // *ber (returns BER estimate)
            &tc_dechalfit_done, // *halfit_done (returns last 1/2 iteration performed when
            // stop_criterion > 0)
            &tc_deccrc_ok, // *crc_error (0=no CRC error, 1=CRC error)
            0); // debug_output (0=debug off, 1=generate debug files)

        // Invert CRC24B error status so that 1 means CRC24B check passed
        tc_deccrc_ok = tc_deccrc_ok ^ 1;

        // Compute number of decoded data bits - filler bits
        j = tc_nout - FillerBits;

        // If CRC24B attached to code block, then remove its bits
        if(CrcSelector != 0)j = j - 24;

        // Pack decoder output bits into output byte array
        PACKBB(&tc_decout[FillerBits], Out, j, 0);

        // Return CRC24B status
        return (U8) tc_deccrc_ok;

    }

    //*************************************************************************
    // If CC decoding selected
    if(CodingType == 0)
    {

        // Compute 1/3 rate encoder output length in bits
        nencout = 3*BlockSize;

        // Allocate temporary space for decoder input array
        decin = (S8*) MemAlloc(nencout*sizeof(S8), DDR_MEM, 0);

        // Allocate temporary space for decoder output array
        decout = (U8*) MemAlloc(BlockSize*sizeof(U8), DDR_MEM, 0);

        // Allocate temporary space for rate-matching mapping output array
        decinmap = (U16*) MemAlloc(RateMatchingE*sizeof(U16), DDR_MEM, 0);

        // Run rate matching to determine how soft-decisions map into
        // decoder input array
        CC_Rx_Rate_Matching(BlockSize, RateMatchingE, decinmap, NULL);

        // Clear decoder input soft-decision array
        for(i = 0; i < nencout; i++)
        {
            decin[i] = 0;
        }

        // For each input soft-decision
        for(i = 0; i < (S32)RateMatchingE; i++)
        {
            // Determine which encoder output stream (d0, d1 or d2)
            j = (decinmap[i] >> 14) & 3;

            // Determine relative location within stream
            n = decinmap[i] & 0x3fff;

            // Compute corresponding decoder input array index
            m = n + n + n + j;

            // Add soft-decision to decoder input array element and limit to +-127
            // If bypassing channel de-interleaver, then use input array
            if(BpChanInt != 0)
            {
                j = decin[m] + In[i];
            }
            // Else use channel de-interleaver outputs

            else
            {
                j = decin[m] + dintout[i];
            }
            if(j > 127) j = 127;
            if(j < -127) j = -127;
            decin[m] = (S8) (j);

        }

        // If using Turbo Best CC decoder function (included in Matlab DLL)
#ifdef USETURBOBESTCCDECODER

        // Run Turbo Best CC decoder
        TurboBest_CC_Decoder_1_3(decin, decout, BlockSize, 8, 96);

#else

        // Run MindSpeed 1/3 rate CC soft-decision decoder
        cc_decoder_1_3(decin, decout, (S16) BlockSize, NULL);

#endif

        // Pack decoder output bits into output bytes
        PACKBB(decout, Out, BlockSize, 0);

        // De-allocate temporary space
        MemFree(decin, DDR_MEM);
        MemFree(decout, DDR_MEM);
        MemFree(decinmap, DDR_MEM);

        // If channel de-interleaver enabled, then de-allocate output space
        if(BpChanInt == 0)
        {
            MemFree(dintout, DDR_MEM);
        }

    }

    //*************************************************************************

    return (0);

}

#endif
//RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR

#endif  //_WIN32

/******************************************************************************
 **********                    Descrambler Function                   **********
 *******************************************************************************

 Function bs_rx_descrambler implements the LTE descrambler function.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE descrambler function<BR>
 *  Function bs_rx_descrambler implements the LTE descrambler function.
 *
 *  <1> Hardware Resource:
 *  ARM, MDMA
 *
 *
 *  @param      in[] =  Input bit array.
 *  @param      out[] = Output bit array.
 *  @param      nin =   Length of arrays.
 *  @param      init =  Initialization request.
 *  @return     None.
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
 *  |out            |               |output       |nin            |pReal Array   |(1:7:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |init           |               |input        |1              |Real          |(0:32:0)  |[0:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------
 *  |Block Name          |bs_rx_descrambler  |
 *  ------------------------------------------
 *  |Input Stream        |in                 |
 *  ------------------------------------------
 *  |Output Stream       |out                |
 *  ------------------------------------------
 *  |Config Parameters   |nin, init          |
 *  ------------------------------------------
 *  |Status Parameters   |none               |
 *  ------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function bs_rx_descrambler implements the LTE descrambler function.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBC
 *
 *
 *  <6> References:
 *
 *
 *  \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//-------------------------------------------------------------------------------------------

void bs_rx_descrambler(S8 in[], S8 out[], S32 nin, U32 init)
{
#ifndef _WIN32

    PMDMA_FDESC pMdma;
    U32 numDesc;

    pMdma = MdmaAllocDesc (MDMA_HW_SYS, 1);
    _ASSERT_PTR (pMdma);
    numDesc = MdmaSetDataMultiple(pMdma, in, out, nin);
    MdmaSetMultiple4GDSCR(pMdma, init, 1, 0, numDesc);
    MdmaRunEx (MDMA_HW_SYS, pMdma, NULL, NULL);

#else
    {
        U8 *cvec;

        cvec = MemAlloc(nin, DDR1_NCNB_MEM, 0);

        descrambler(init, in, out, nin, cvec);

        MemFree(cvec, DDR1_NCNB_MEM);
    }
#endif

    return;

}


#if 0
#ifndef _WIN32
//-------------------------------------------------------------
// CC Decoder Test
//-----------------

// Reference Inputs
const UINT32 CcDecoderInLen = 80;
const UINT32 CcDecoderOutLen = 3;

const UINT8 CcDecoderIn[] = {
0x29, 0xd8, 0x28, 0x28, 0x79, 0x7a, 0x28, 0x29,
0x79, 0x7a, 0xd8, 0x29, 0x7a, 0xd8, 0xd8, 0x29,
0x28, 0x28, 0xd7, 0xd7, 0x29, 0xd7, 0x28, 0x29,
0x28, 0x7a, 0x29, 0x29, 0xd7, 0x87, 0xd8, 0x27,
0x7a, 0x28, 0x28, 0x29, 0xd8, 0x79, 0x2a, 0xd8,
0x7a, 0xd8, 0x28, 0x29, 0x86, 0x79, 0x28, 0x28,
0xd7, 0x29, 0xd7, 0x29, 0x79, 0x86, 0xd8, 0x28,
0x86, 0x7a, 0xd8, 0x28, 0x28, 0x87, 0x29, 0x28,
0x7a, 0x28, 0x28, 0x29, 0x79, 0x79, 0xd8, 0x28,
0x28, 0x87, 0xd7, 0x28, 0x79, 0x79, 0xd8, 0xd7,
};

const UINT8 CcDecoderOut[] = {
0xff, 0xfa, 0x90, 0x00,
};


// Call Back Function and Variable
VUINT32 CcDecoderTestDone = 0;
void CcDecoderTestDoneCb(LPVOID CbContext)
{
    CcDecoderTestDone = 1;
    //uart_printf("CcDecoderTestDoneCb:Running CcDecoderTestDoneCb\n");
    return;
}

void DebugCCDecoder(U8 *in, U8 *out, U32 nin, U32 nout)
{
    PFEC_UL_FDESC pFecUL;
    volatile PFEC_CONFIG_REGS pFecConfigRegs;
    UINT32 t;
    ULHWCTX hw_ctx=
    { 0 };
    S8 *DescAdr[1];
    S32 DescLen[1];

     // Configure FEC CTC decoder via driver.

    // Allocate temporary space for descriptors via driver
    pFecUL = FecUlAllocDesc(1);
    _ASSERT_PTR(pFecUL);

    // Configure FEC control registers.
    pFecConfigRegs = (PFEC_CONFIG_REGS) FEC_CONFIG_REGS_BASE;
    pFecConfigRegs->bp_channel_interleaver = 1;

    // set hardware context
    memset(&hw_ctx, 0, sizeof(hw_ctx));

    hw_ctx.CTC_CCn = 0;
    hw_ctx.CRC_Sel = 0;
    hw_ctx.Modulation = 0;
    hw_ctx.HalfInteration = 0;
    hw_ctx.Bypass_Rate_Match = 0;
    hw_ctx.Rate_Index = 0;
    hw_ctx.Enable_Combine = 0;
    hw_ctx.Bypass_Decode_Enable = 0;
    hw_ctx.Dynamic_Stop_Iteration = 0;
    hw_ctx.BlockSize = nout + 8;
    hw_ctx.Filler = 0;
    hw_ctx.E_Param = nin;
    hw_ctx.LTERateMaching_K0 = 0;

    // Create FEC CTC decoder control descriptor via driver.
    DescAdr[0] = in;
    DescLen[0] = nin;

    FecUlSetHwCtx(pFecUL, hw_ctx);
    FecUlSetData(pFecUL, DescAdr, DescLen, out, ((nout + 15) >> 3));

    // Inform FEC driver to enable CC decoder finished interrupt.
    // FEC driver will call function FecRxCCDone when decoder is finished.
    FecUlSetLastDesc(pFecUL, (LPVOID) CcDecoderTestDoneCb, (LPVOID) NULL);

    // Initialize decoder finished status flag.
    // Function FecRxCCDone will set status flag to 1 when decoder has finished.
    CcDecoderTestDone = 0;

    // Run FEC CC decoder via driver and wait until finished.
    t = GET_TICKS;

    FecUlRun(pFecUL);
    while ((CcDecoderTestDone == 0)); // && ((GET_TICKS - t) > 150000)

    // Set FEC control register back to default setting.
    pFecConfigRegs->bp_channel_interleaver = 0;

    MxCacheInvalidate(out, ((nout + 15) >> 3));

    return;
}

int TestDecoderCombo(U8* in, U8*out)
{
    U32 ii, jj, flag = 0, flag1 = 0;

    // Copy Reference Input and initialize Output
    for (ii = 0; ii < CcDecoderInLen; ii++)
        in[ii] = CcDecoderIn[ii];
    memset(out, 0xBC, CcDecoderOutLen);

    // Call DebugCCDecoder Function
    DebugCCDecoder(in, out, CcDecoderInLen, 12);

    // Compare Outputs
    for(ii = 0 ;ii < CcDecoderOutLen; ii++)
    {
        if (out[ii] != CcDecoderOut[ii])
        {
            flag = 1;
            uart_printf("Output[%d]: %d != Ref Output[%d]: %d\r\n", ii, out[ii], ii, CcDecoderOut[ii]);
            if (ii != CcDecoderOutLen - 1)
                flag1 = 1;
        }
    }

    if (flag != 1)
        uart_printf("Output == Ref Output\r\n");

    return flag1;
}

void TestDecoder()
{
    U8 *in, *out;
    int ii, jj;
    char memoryname[3][10] = {"DDR_MEM \0", "CRAM_MEM\0", "NCNB_MEM\0"};
    int memory[3] = {DDR_MEM, CRAM_MEM, NCNB_MEM};
    int passfail[3][3];

    for (ii = 0; ii < 3; ii++)
        for (jj = 0; jj < 3; jj++)
        {
            uart_printf("Testing: IN[%s], OUT[%s]\r\n", memoryname[ii], memoryname[jj]);
            passfail[ii][jj] = 0;

            in = (U8*) MemAlloc(CcDecoderInLen, memory[ii], 0);
            out = (U8*) MemAlloc(CcDecoderOutLen, memory[jj], 0);

            passfail[ii][jj] = TestDecoderCombo(in, out);

            MemFree(in, memory[ii]);
            MemFree(out, memory[jj]);

            uart_printf("\r\n");
        }

        uart_printf("IN  OUT %s  %s  %s\r\n", memoryname[0], memoryname[1], memoryname[2]);
        for (ii = 0; ii < 3; ii++)
        {
            uart_printf("%s  ", memoryname[ii]);
            uart_printf("%s  ", (passfail[ii][0] ? "FAIL    ":"PASS    "));
            uart_printf("%s  ", (passfail[ii][1] ? "FAIL    ":"PASS    "));
            uart_printf("%s  ", (passfail[ii][2] ? "FAIL    ":"PASS    "));
            uart_printf("\r\n");
        }

}

#endif
#endif
