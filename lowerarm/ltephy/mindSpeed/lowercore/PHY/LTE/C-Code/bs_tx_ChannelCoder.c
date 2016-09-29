//-------------------------------------------------------------------------------------------
/** @file bs_tx_ChannelCoder.c
 *
 * @brief This file implements the LTE base station transmitter forward error correction functions.
 * @author Mindspeed Technologies
 * @version $Revision: 1.116 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

/******************************************************************************
**********             CRITICAL HARDWARE FEC INFORMATION             **********
*******************************************************************************

X7 Device Information:
----------------------
1) For all previous devices, there is a problem with the CTC encoder rate
matching function.  Whenever the specified Ncb < Kw and k0 > 0 and a circular
buffer wrap-around occurs, the next read position after the wrap-around is set
to k0 instead of 0.  For all other conditions the rate matching works properly.
This problem has been fixed in the X7 device.  However, the fix requires that
the PHY code when programming the FEC's CTC rate matching buffer length uses
the value Ncb-32 instead of Ncb.

2) Unfortunately, the X7 fix described above has inadvertently broken the CC
encoder's rate matching.  This occurred because for the CC encoder the rate
matching buffer length is computed internally by the FEC and it was not
correctly modified to compute the now required Ncb-32 instead of Ncb.
Consequently, any CC encoder rate matching that results in a circular buffer
wrap-around must be performed externally by the ARM.

There is a new function DevInfoIsT4Kx7() that returns true if and only if
the device is a X7.  This function is used by the PHY code to conditionally
execute the required code needed to support the X7 device.

The code segments needed to support the X7 device are surrounded by:

    //X7X7X7X7X7X7

The code segments needed to support the X7 device are enabled by defining the
control parameter X7_DEV_ENABLED.

******************************************************************************/

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
#include "tc7000_lte.h"
#include "tc1700.h"
#include "prototypes.h"

#ifndef _WIN32
#include "mdmadrv.h"
#include "mlog.h"
#endif

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#define CEVA_BUILD 1
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#endif

// Globals
//extern U8 *scrOut1;
//extern U8 *scrOut2;
//extern U32 *scrIndex1, *scrIndex2;
//extern U32 _totalsizescr[2];


#ifdef  _WIN32

//*****************************************************************************
//**********              CONSTANT PARAMETER DEFINITIONS             **********
//*****************************************************************************

// Define gCRC24A generator polynomial mask
// gCRC24A(D) = D**24 + D**23 + D**18 + D**17 + D**14 + D**11 + D**10 +
//              D**7 + D**6 + D**5 + D**4 + D**3 + D**1 + 1

#define P_GCRC24A   0xC3267D

//*****************************************************************************

//-------------------------------------------------------------------------------------------
/** @brief LTE downlink transmit channel coding function for MAC transport blocks.
 *
 *  <1> Hardware Resource:
 *  ARM, FEC, MDMA
 *
 *  @param      chanId       Channel ID.
 *  @param      cwId         Codeword ID.
 *  @param      in           Input data array organized as bytes.  Data bits are
 *                           packed into bytes.  Bit order is msb first, lsb last.
 *  @param      nin          Length of input array in bytes.
 *  @param      out          Output data array organized as bytes.  Data is stored one
 *                           bit per byte (lsb = data bit).
 *  @param      G            Total number of bits available for the transmission of one
 *                           transport block.
 *  @param      rvidx        Redundancy version number (0, 1, 2, or 3).
 *  @param      NL           1 (for transport blocks mapped to 1 layer) or 2 (for transport
 *                           blocks mapped to 2 or 4 layers).
 *  @param      Qm           Modulation index (QPSK: Qm=2, 16QAM: Qm=4, 64QAM: QM=6).
 *  @param      nRNTI        Radio netword temporary identifier.
 *  @param      channelType  Channel type.
 *  @param      slotNumber   Slot number.
 *  @param      Ncell_id     Physical layer cell identity.
 *  @param      numberofCont Number of control channels.
 *  @param      scrOutBufs   Scrambler output buffer array.
 *  @param      scrIndex     Scrambler index buffer array.
 *  @param      _totalsizescr Scrambler sizes.
 *  @param      NIR          Rate matching buffer size limit.
 *  @return     nout         Length of output array in bytes.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |chanId         |               |input        |1              |Real          |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |cwId           |               |input        |1              |Real          |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |nout           |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |G              |               |input        |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |rvidx          |               |input        |1              |Real          |(1:15:0)  |{0,1,2,3}      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NL             |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Qm             |               |input        |1              |Real          |(1:15:0)  |{2,4,6}        |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nRNTI          |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |channelType    |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |slotNumber     |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NCell_id       |               |input        |1              |Real          |(1:15:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |numberofCont   |               |input        |1              |Real          |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |scrOutBufs     |               |input        |2              |pReal Array   |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |scrIndex       |               |input        |2              |pReal Array   |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |_totalsizescr  |               |input        |2              |pReal Array   |(0:32:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |NIR            |               |input        |2              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nout           |               |output       |1              |Real          |(1:31:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------------------------------------------------------------
 *  |Block Name          |BsTxChannelCoder                                                                           |
 *  ------------------------------------------------------------------------------------------------------------------
 *  |Input Stream(s)     |in                                                                                         |
 *  ------------------------------------------------------------------------------------------------------------------
 *  |Output Stream(s)    |out                                                                                        |
 *  ------------------------------------------------------------------------------------------------------------------
 *  |Config Parameter(s) |chanId,cwId,nin,G,rvidx,NL,Qm,nRNTI,channelType,slotNumber,NCell_id,numberofCont,scrOutBufs|
 *  |                    |scrIndex,_totalsizescr                                                                     |
 *  ------------------------------------------------------------------------------------------------------------------
 *  |Status Parameter(s) |nout                                                                                       |
 *  ------------------------------------------------------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function BsTxChannelCoder implements the channel coding function for a
 * downlink shared channel as defined in document 3GPP TS 36.212 V8.6.0
 * (2009-03) Section 5.3.2.  Specifically, the following processing steps are
 * performed:
 *
 *    - Unpack input data byte array into a data bit array (1 bit per byte).
 *    - Compute and append 24-bit CRC to data bit array.
 *    - Perform code block segmentation and CRC attachment.
 *    - Encode data and CRC using 1/3 rate CTC encoder.
 *    - Perform rate matching (sub-block interleaving, bit collection and
 *      bit selection of CTC encoder outputs.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212 Section 5.3.2
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------

S32 BsTxChannelCoder(U32 chanId, U32 cwId, U8* in_dat_ptr, S32 num_in_bytes, U8* out,
                     S32 G, S16 rvidx, S16 NL, S16 Qm, U16 nRNTI, S16 channelType,
                     S16 slotNumber, S16 Ncell_id, U32 numberofControlChannels,
                     U8* scrOutBufs[], U32 *scrIndex[], U32 _totalsizescr[], S32 NIR)
{
    S16 k;
    S16 C_num_code_blocks, FillerBits;
    U8 *CRC24A_output_array, *tc_encout;

    U8 *pInp, *pOut, *pOutFEC, *pBitsTmp;
    U32 noutbytes[40], noutbits1[40], numBits[40], noutbits, ninbytes[40], totalOutSize;
    U32 c0[40], c1[40], bit, currBits, startByte;
    S16 posBit, i;
    S16 K, Kplus, Kminus, Cplus, Cminus, crcsel, remBits, flagRepack;

    RMParameters *prm;
    RMParameters prm_obj;

    U8 *scrOut_local, *pScrOut;
    U16 utemp16;
    U32 cinit, cinit1, scrInit, *scrIndex_local;
    S32 length_s, length_s1;
    U32 _totalsizescr_local;

    U32 CB;

    //printf("BsTxChannelCoder: (num_in_bytes=%d, G=%d, rvidx=%d, NL=%d, Qm=%d, nRNTI=%d\n", num_in_bytes,G,rvidx,NL,Qm,nRNTI);

    // Read code block segmentation table using number of bytes -1 as index.
    // The number of code blocks (C_num_code_blocks = Cplus) are in the lower 16 bits.
    // The code block size (Kplus) is in the upper 16 bits.
    // For all valid LTE transport block sizes Cminus=0, Kminus=0 and FillerBits=0.
    // Invalid transport block sizes are indicated by a 0 table entry.
    CB = Table_DL_FEC_CTC_CB[num_in_bytes-1];

    // If table indicates transport block size is legal, then.
    if(CB != 0)
    {
        // Use the lower 16-bits as number of total code blocks and Cplus.
        C_num_code_blocks = CB & 65535;
        Cplus = C_num_code_blocks;

        // Use the upper 16 bits as code block size Kplus.
        Kplus = CB >> 16;

        // For LTE valid transport block sizes all other parameters are 0.
        Cminus = 0;
        Kminus = 0;
        FillerBits = 0;

        //printf("C=%d, C+=%d, C-=%d, K+=%d, K-=%d, F=%d\n", C_num_code_blocks, Cplus, Cminus, Kplus, Kminus, FillerBits);
        //i = getchar();

    }
    // Else this is an invalid LTE transport block size.
    else
    {

        // Segment bitstream block into small sub-block streams according to LTE Standard rules
        // Compute code block segmentation parameters.
        lte_code_blk_seg(num_in_bytes*8+NUM_PARITY, &C_num_code_blocks, &Cplus, &Cminus, &Kplus, &Kminus, &FillerBits, BlkSizTab);

    }

    tc_encout = MemAlloc(12*(num_in_bytes+NUM_PARITY/8), DDR_MEM, 0);
    CRC24A_output_array = MemAlloc(num_in_bytes + NUM_PARITY/8, CRAM_MEM, 0);
    pBitsTmp = MemAlloc(8*12*(num_in_bytes+NUM_PARITY/8), DDR_MEM, 0);

    memcpy(CRC24A_output_array, in_dat_ptr, num_in_bytes);

    // Compute 3 byte CRC24A and append to end of input array.
    ComputeCRC24(CRC24A_output_array, num_in_bytes, P_GCRC24A, &CRC24A_output_array[num_in_bytes]);

    crcsel = 0;
    if(C_num_code_blocks > 1)
    {
        crcsel = 2;
    }

    // Initialize number of output bytes
    noutbits = 0;
    pInp = CRC24A_output_array;
    pOut = tc_encout;

    // Allocate temporary space for rate matching parameters
    //prm = MemAlloc(sizeof (RMParameters), CRAM_MEM, 0);
    prm = &prm_obj;

    flagRepack = 0;
    totalOutSize = 0;

    for(k = 0; k < C_num_code_blocks; k++)
    {
        // Compute code block size in bits.
        K = Kplus;
        if (k < Cminus)
        {
            K = Kminus;
        }

        Compute_Rate_Matching_Parameters(K, C_num_code_blocks, NL, G, Qm, k, NIR, prm);

        // Compute rate matching parameters E, k0 and Ncb.
        noutbytes[k] = (7+prm->E) >> 3;
        ninbytes[k] = (K-FillerBits) >> 3;

        // Run CTC encoder with rate matching.
        CTC_Encoder_Rate_Matching(pInp, pOut, K, FillerBits, crcsel, prm->E, prm->k0[rvidx], prm->Ncb, k);

        //printf("BsTxChannelCoder: K=%d, E=%d, k0=%d, Ncb=%d\n", K, prm->E,prm->k0[rvidx],prm->Ncb);
        //i = getchar();

        pInp += ninbytes[k] - NUM_PARITY/8;
        pOut += noutbytes[k];
        noutbits += prm->E;
        noutbits1[k] = prm->E;
        totalOutSize += noutbytes[k];

        numBits[k] = (noutbytes[k] << 3) - noutbits1[k];

        if(numBits[k] != 0)
        {
            flagRepack = 1;
        }
        // Set number of filler bits = 0;
        FillerBits = 0;

    }

    if(C_num_code_blocks == 1)
    {
        flagRepack = 0;
    }

    // If not 64QAM or running FEC unit tests without scrambling (cwId > 1)
    if( ((Qm>>1) != QAM64) || (cwId > 1) )
    {
        pOut = out;
        pOutFEC = tc_encout;
        if(flagRepack)
        {
            remBits = 0;
            for(k=0; k< C_num_code_blocks; k++)
            {
                if(numBits[k] != 0 || remBits != 0)
                {
                    if(remBits == 0)
                    {
                        // No need to repack this block
                        memcpy(pOut, pOutFEC, noutbytes[k]);
                        remBits = (S16)numBits[k];
                        pOut += noutbytes[k];
                    }
                    else
                    {
                        // Need to repack this block
                        RepackBytes(&pOut, pOutFEC, &remBits, noutbits1[k]);
                    }
                }
                else
                {
                    memcpy(pOut, pOutFEC, noutbytes[k]);
                    pOut += noutbytes[k];
                }
                pOutFEC += noutbytes[k];
            }
        }
        else
        {
            memcpy(pOut, pOutFEC, totalOutSize);
        }

        // Bypass scrambling for FEC unit testing (cwId > 1)
        if( cwId > 1) goto skipscram;

        // Scramble the output
        if (cwId == 0)
        {
            scrOut_local = scrOutBufs[0];
            scrIndex_local = scrIndex[0];
            _totalsizescr_local = _totalsizescr[0];
        }
        else
        {
            scrOut_local = scrOutBufs[1];
            scrIndex_local = scrIndex[1];
            _totalsizescr_local = _totalsizescr[1];
        }
        scrOut_local += _totalsizescr_local;

        // Init scrambler state, to prevent interference between users, base station sectors etc.
        // cinit = nRNTI*2^14 + q*2^13 + floor(ns/2)*2^9 + Ncell_id;
        // initial values depends on user, cell, codeword, slot number
        utemp16 = slotNumber>>1;
        cinit = (nRNTI << 14) + (utemp16 << 9) + Ncell_id;
        if (cwId != 0)
        {
            cinit = cinit + (1<<13);
        }

        length_s = (noutbits + 7) >> 3;

        scrambler_byte(cinit, out, tc_encout, length_s);

        if((Qm>>1) == QAM64)
        {
            UNPACKBB(tc_encout, pBitsTmp, noutbits, 0);
            length_s = tx_Mapper_Pack(pBitsTmp, scrOut_local, noutbits, QAM64);
        }
        else
        {
            memcpy(scrOut_local, tc_encout, totalOutSize);
        }

        scrIndex_local[chanId] = _totalsizescr_local;
        _totalsizescr_local += RUP32B(length_s);

        if (cwId == 0)
        {
            _totalsizescr[0] = _totalsizescr_local;
        }
        else
        {
            _totalsizescr[1] = _totalsizescr_local;
        }
    }
    else
    {
        // Scramble the output
        if (cwId == 0)
        {
            scrOut_local = scrOutBufs[0];
            scrIndex_local = scrIndex[0];
            _totalsizescr_local = _totalsizescr[0];
        }
        else
        {
            scrOut_local = scrOutBufs[1];
            scrIndex_local = scrIndex[1];
            _totalsizescr_local = _totalsizescr[1];
        }
        scrOut_local += _totalsizescr_local;

        utemp16 = slotNumber>>1;
        cinit = (nRNTI << 14) + (utemp16 << 9) + Ncell_id;
        if (cwId != 0)
        {
            cinit = cinit + (1<<13);
        }
        //        cinit1 = 1;   // For hardware
        cinit1 = 1581799488;

        pOut = out;
        pOutFEC = tc_encout;
        pScrOut = scrOut_local;

        length_s = 0;
        if(flagRepack)
        {
            U8 *zero, *cinit_out, *cinit1_out;
            U32 tmp1, tmp2, Tmp;
            zero = MemAlloc(4*(num_in_bytes+NUM_PARITY/8), DDR_MEM, 0);
            cinit_out = MemAlloc(4*(num_in_bytes+NUM_PARITY/8), DDR_MEM, 0);
            cinit1_out = MemAlloc(4*(num_in_bytes+NUM_PARITY/8), DDR_MEM, 0);
            memset(zero, 0, 4*(num_in_bytes+NUM_PARITY/8));

            remBits = 0;
            tmp1 = cinit;
            tmp2 = cinit1;
            Tmp = 0;
            scrInit = 1;
            scrambler_byte1(&tmp1, &Tmp, zero, cinit_out, totalOutSize, scrInit);
            Tmp = 0;
            scrInit = 1;
            scrambler_byte1(&Tmp, &tmp2, zero, cinit1_out, totalOutSize, scrInit);

            currBits = 0;
            for(k=1; k< C_num_code_blocks; k++)
            {
                currBits += noutbits1[k-1];
                startByte = currBits >> 3;
                posBit = (S16)(currBits - (startByte << 3));
                posBit = 7 - posBit;
                tmp1 = 0;
                tmp2 = 0;
                for(i=0; i<32; i++)
                {
                    bit = (cinit_out[startByte] >> posBit) & 0x1;
                    tmp1 |= (bit << i);
                    bit = (cinit1_out[startByte] >> posBit) & 0x1;
                    tmp2 |= (bit << i);
                    posBit -= 1;
                    if(posBit < 0)
                    {
                        posBit = 7;
                        startByte += 1;
                    }
                }
                c0[k-1] = tmp1;
                c1[k-1] = tmp2;
            }

            scrInit = 1;
            tmp1 = cinit;
            tmp2 = 1581799488;
            scrambler_byte1(&tmp1, &tmp2, pOutFEC, pOut, noutbytes[0], scrInit);
            if((Qm>>1) == QAM64)
            {
                UNPACKBB(pOut, pBitsTmp, noutbits1[0], 0);
                length_s1 = tx_Mapper_Pack(pBitsTmp, pScrOut, noutbits1[0], QAM64);
            }
            else
            {
                memcpy(pScrOut, pOut, noutbytes[0]);
                length_s1 = noutbytes[0];
            }
            pScrOut += length_s1;
            pOut += noutbytes[0];
            pOutFEC += noutbytes[0];
            length_s += length_s1;
            scrInit = 0;
            for(k=1; k< C_num_code_blocks; k++)
            {
                tmp1 = c0[k-1];
                tmp2 = c1[k-1];
                scrambler_byte1(&tmp1, &tmp2, pOutFEC, pOut, noutbytes[k], scrInit);

                if((Qm>>1) == QAM64)
                {
                    UNPACKBB(pOut, pBitsTmp, noutbits1[k], 0);
                    length_s1 = tx_Mapper_Pack(pBitsTmp, pScrOut, noutbits1[k], QAM64);
                }
                else
                {
                    memcpy(pScrOut, pOut, noutbytes[k]);
                    length_s1 = noutbytes[k];
                }
                pScrOut += length_s1;
                pOut += noutbytes[k];
                pOutFEC += noutbytes[k];
                length_s += length_s1;
            }

            MemFree(zero, DDR_MEM);
            MemFree(cinit_out, DDR_MEM);
            MemFree(cinit1_out, DDR_MEM);
        }
        else
        {
            //        scrambler_byte(cinit, tc_encout, out, length_s);
            length_s = (noutbits + 7) >> 3;

//            scrambler_byte1(&cinit, &cinit1, pOutFEC, pOut, length_s, 1);
            scrambler_byte(cinit, pOutFEC, pOut, length_s);
            if((Qm>>1) == QAM64)
            {
                UNPACKBB(pOut, pBitsTmp, noutbits, 0);
                length_s = tx_Mapper_Pack(pBitsTmp, pScrOut, noutbits, QAM64);
            }
            else
            {
                memcpy(pScrOut, pOut, totalOutSize);
            }
        }

        //  Recover FEC out for printing
        memcpy(out, tc_encout, totalOutSize);

        scrIndex_local[chanId] = _totalsizescr_local;
        _totalsizescr_local += RUP32B(length_s);

        if (cwId == 0)
        {
            _totalsizescr[0] = _totalsizescr_local;
        }
        else
        {
            _totalsizescr[1] = _totalsizescr_local;
        }
    }

    skipscram:
    MemFree(CRC24A_output_array, CRAM_MEM);
    MemFree(tc_encout, DDR_MEM);
    MemFree(pBitsTmp, DDR_MEM);
    return (noutbits);

}

//*****************************************************************************
//**********              CONSTANT PARAMETER DEFINITIONS             **********
//*****************************************************************************

// Parameter P_ENCOUT_SIZ specifies the maximum size of the encoder output
// array in bits assuming internal sub-block interleaving.

#define P_ENCOUT_SIZ    18528   // 3*32*((6144 + 4 + 31)/32)
//*****************************************************************************

// Parameter P_ENCIN_SIZ specifies the maximum size of the encoder input
// array in bits.

#define P_ENCIN_SIZ     6144

//*****************************************************************************
//**********              TURBO CONCEPT ENCODER ARRAYS               **********
//*****************************************************************************

U8 tc_encin[P_ENCIN_SIZ], tc_encinwithcrc[P_ENCIN_SIZ];
U8 tc_encout[P_ENCOUT_SIZ], tc_qfdb[P_ENCOUT_SIZ];

//*****************************************************************************

/******************************************************************************
 *****     Simulation of Turner Hardware CTC Encoder With Rate Matching    *****
 *******************************************************************************

 Funcion CTC_Encoder_Rate_Matching simulates the Turner hardware LTE CTC
 encoder with built-in rate matching.  The functional calling format is
 defined below.

 CTC_Encoder_Rate_Matching(in[], out, nin, nfil, crcsel, E, k0, Ncb, Cblk)

 Where: in =     Packed encoder input data bit array organized as bytes.
 For each byte bit7(msb) is the first bit and bit0(lsb) is
 the last bit.
 out =    Packed rate matching output data bit array organized as
 bytes.  For each byte bit7(msb) is the first bit and
 bit0(lsb) is the last bit.
 nin =    CTC encoder code block size in bits.
 nfil =   Number of initial filler bits included in CTC code block.
 crcsel = Type of CRC to be appended to end of data bits.
 (0 = no CRC appended.  2 = append 24-bit CRC24B).
 E =      Rate matching number of output bits.
 k0 =     Rate matching circular buffer starting position.
 Ncb =    Rate matching circular buffer size in bits.
 out =   output data bit array organized as 1 bit per byte (bit0 =
 data bit).
 nin =   length of input array (in) in bytes.
 nant =  number of transmit antenna ports (1, 2 or 4).  This value
 is used to control the CRC scrambling.
 E =     desired rate matching output length in bits.
 Cblk =  Code block relative index (range = 0 - N-1).

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief Simulation of Turner hardware LTE CTC encoder with rate matching<BR>
 *  Function CTC_Encoder_Rate_Matching simulates the Turner hardware LTE CTC
 *  encoder with built-in rate matching.
 *
 *  <1> Hardware Resource:
 *  ARM FEC
 *
 *
 *  @param  in =    Packed encoder input data bit array organized as bytes.
 *                  For each byte bit7(msb) is the first bit and bit0(lsb) is
 *                  the last bit.
 *  @param  out =   Packed rate matching output data bit array organized as
 *                  bytes.  For each byte bit7(msb) is the first bit and
 *                  bit0(lsb) is the last bit.
 *  @param  nin =   CTC encoder code block size in bits.
 *  @param  nfil =  Number of initial filler bits included in CTC code block.
 *  @param  crcsel =Type of CRC to be appended to end of data bits.
 *                  (0 = no CRC appended.  2 = append 24-bit CRC24B).
 *  @param  E =     Rate matching number of output bits.
 *  @param  k0 =    Rate matching circular buffer starting position.
 *  @param  Ncb =   Rate matching circular buffer size in bits.
 *  @param  Cblk =  Code block relative index (range = 0 - N-1).
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
 *  |in             |               |input        |nin            |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |E              |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:15:0)  |[1:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nfil           |               |input        |1              |Real          |(1:15:0)  |[0:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |crcsel         |               |input        |1              |Real          |(1:15:0)  |{0, 2}         |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |E              |               |input        |1              |Real          |(1:31:0)  |[1: max pos]   |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |k0             |               |input        |1              |Real          |(1:15:0)  |[0: Ncb-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Ncb            |               |input        |1              |Real          |(1:15:0)  |[1:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Cblk           |               |input        |1              |Real          |(1:15:0)  |[0:32767]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |CTC_Encoder_Rate_Matching     |
 *  -----------------------------------------------------
 *  |Input Stream(s)     |in                            |
 *  -----------------------------------------------------
 *  |Output Stream(s)    |out                           |
 *  -----------------------------------------------------
 *  |Config Parameter(s) |nin,nfil,crcsel,E,k0,Ncb,Cblk |
 *  -----------------------------------------------------
 *  |Status Parameter(s) |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function CTC_Encoder_Rate_Matching simulates the Turner hardware LTE CTC
 *  encoder with built-in rate matching.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBC
 *
 *
 * <6> References:
 * [1] 3GPP (LTE) standard TS 36.212
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 **/
//-------------------------------------------------------------------------------------------

void CTC_Encoder_Rate_Matching(U8 in[], U8 out[], S16 nin, S16 nfil, S16 crcsel,
    S32 E, S16 k0, S16 Ncb, S16 Cblk)
{

    S32 tc_nout;

    S16 i, ncrc, datcnt;
    S32 ii, jj, kk, mm;
    U8 dat;

    //*****************************************************************************

    // Compute number of CRC bits to be appended for code block
    ncrc = 0;
    if(crcsel != 0)ncrc = 24;

    // Insert filler bits into encoder input array, if required
    for(i = 0; i < nfil; i++)
    {
        tc_encin[i] = 0;
    }

    // Unpack input data bytes and copy them into encoder input array.
    UNPACKBB(in, &tc_encin[nfil], (nin-ncrc), 0);

    // If code block has CRC, then encoder requires additional 0 bits
    // appended to end of data bits.
    for(i = 0; i < ncrc; i++)
    {
        tc_encin[nin-ncrc+i] = 0;
    }

    // Run Turbo Concept CTC encoder.
    tc7000_lte_encode(0, // fec_mode (0=CTC, 1=CC)
        nin, // encoder input size in bits
        crcsel, // crc select (0=none, 2=CRC24B)
        nfil, // number of filler bits
        0, // scramble enable (0=no scramble applied)
        0, // scramble mask
        0, // bypass_interleaver (0=no, 1=yes)
        0, // bypass encoder (0=no, 1=yes)
        &tc_nout, // encoder output size in bits
        tc_encin, // encoder input array (1 bit per element)
        tc_encout, // encoder output array (1 bit per element)
        tc_qfdb, // encoder output filler bit array
        // (1 bit per element) (0=data, 1=filler bit)
        tc_encinwithcrc); // encoder input with appended CRC

    // Initialize rate matching variables.
    kk = 0;
    jj = 0;

    // Initialize packed output bit array index.
    mm = 0;

    // Initialize packed bit output.
    dat = 0;

    // Initialize packed bit position.
    datcnt = 7;

    // Perform rate matching.
    laba:

    // Compute rate matching buffer index.
    ii = (k0 + jj) % Ncb;

    // If data is not a null.
    if(tc_qfdb[ii] != 1)
    {
        // Combine data bit into packed byte.
        dat = dat | ( (tc_encout[ii]&1) << datcnt);

        // Decrement bit position counter.
        datcnt = datcnt - 1;

        // If bit position counter is past end.
        if(datcnt < 0)
        {
            // Reset bit position counter.
            datcnt = 7;

            // Store packed data into output array.
            out[mm] = dat;

            // Clear output byte.
            dat = 0;

            // Increment output array pointer.
            mm = mm + 1;

        }

        // Increment output bit counter.
        kk = kk + 1;
    }

    // Increment input bit counter.
    jj = jj + 1;

    // If more bits to output, then continue.
    if(kk < E)goto laba;

    // If last output byte was not filled and saved, then do it.
    if(datcnt < 7)out[mm] = dat;

    return;

}

#endif

#ifndef _WIN32   //  FPGA
void FecDlBcCbDone(void);
static volatile U32 status_bc;

void FecDlBcCbDone(void)
{
    status_bc = 1;
    return;
}
#endif

/******************************************************************************
 ********** Channel Coding Processing for Downlink Broadcast Channel  **********
 *******************************************************************************

 Function bs_tx_broadcast_channel_coding implements the channel coding function
 for a downlink broadcast channel as defined in document 3GPP TS 36.212 V8.6.0
 (2009-03) Section 5.3.1.  Specifically, the following processing steps are
 performed:

 1) Unpack input data byte array into a data bit array (1 bit per byte).
 2) Compute and append 16-bit CRC to data bit array.
 3) Scramble CRC bits according to antenna configuration.
 4) Encode data and CRC using 1/3 rate tail-biting convolutional encoder.
 5) Perform rate matching (sub-block interleaving, bit collection and
 bit selection of convolutional encoder outputs.

 The functional calling format is defined below.

 bs_tx_broadcast_channel_coding(in, out, nin, nant, E)

 Where:  in =    packed input data bit array organized as bytes.  For each
 byte bit7(msb) is the first bit and bit0(lsb) is the last
 bit.
 out =   output data bit array organized as 1 bit per byte (bit0 =
 data bit).
 nin =   length of input array (in) in bytes.
 nant =  number of transmit antenna ports (1, 2 or 4).  This value
 is used to control the CRC scrambling.
 E =     desired rate matching output length in bits.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief LTE downlink transmit channel coding function for broadcast channels.
 *
 *  <1> Hardware Resource:
 *  ARM, FEC
 *
 *  @param     in      packed input data bit array organized as bytes.  For each
 *                     byte bit7(msb) is the first bit and bit0(lsb) is the last
 *                     bit.
 *  @param     out     output data bit array organized as 1 bit per byte (bit0 =
 *                     data bit).
 *  @param     nin     length of input array in bytes.
 *  @param     nant    number of transmit antenna ports (1, 2 or 4).  This value
 *                     is used to control the CRC scrambling.
 *  @param     E       desired rate matching output length in bits.
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
 *  |in             |               |input        |nin            |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |E              |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nant           |               |input        |1              |Real          |(1:15:0)  |{1,2,4}        |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |E              |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------
 *  |Block Name          |bs_tx_broadcast_channel_coding |
 *  ------------------------------------------------------
 *  |Input Stream(s)     |in                             |
 *  ------------------------------------------------------
 *  |Output Stream(s)    |out                            |
 *  ------------------------------------------------------
 *  |Config Parameter(s) |nin,nant,E                     |
 *  ------------------------------------------------------
 *  |Status Parameter(s) |none                           |
 *  ------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function bs_tx_broadcast_channel_coding implements the channel coding function
 * for a downlink broadcast channel as defined in document 3GPP TS 36.212 V8.6.0
 * (2009-03) Section 5.3.1.  Specifically, the following processing steps are
 * performed:
 *
 *    - Unpack input data byte array into a data bit array (1 bit per byte).
 *    - Compute and append 16-bit CRC to data bit array.
 *    - Scramble CRC bits according to antenna configuration.
 *    - Encode data and CRC using 1/3 rate tail-biting convolutional encoder.
 *    - Perform rate matching (sub-block interleaving, bit collection and
 *      bit selection of convolutional encoder outputs.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.6.0 (2009-03) Section 5.3.1
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------

void bs_tx_broadcast_channel_coding(U8 in[], U8 out[], S32 nin, S16 nant, S32 E)
{

    //*****************************************************************************
    //**********  CONDITIONAL COMPILATION CONTROL PARAMETER DEFINITIONS  **********
    //*****************************************************************************

    // This simulation can be compiled using the following three options:

    //      Option 1:   Use MindSpeed C coded functions.

    //      Option 2:   Use functions provided by Turbo Concept in their DLLs.

    //      Option 3:   Use functions provided by T4K hardware.

    //      Option 4:   Use functions provided by Turbo Concept in their DLLs and
    //                  also use MindSpeed C coded functions to verify that the
    //                  Turbo Concept functions are correct.

    // Parameter P_USE_MSPD is used to control the use of the MindSpeed functions
    // (0 = no, 1 = yes).

    // Parameter P_USE_TCON is used to control the use of the Turbo Concept
    // functions (0 = no, 1 = yes).

#ifdef  _WIN32
#define P_USE_MSPD  0
#define P_USE_TCON  (1-P_USE_MSPD)
#endif

#ifndef _WIN32

    S32 Kr = 0, Er = 0, nout, ind, scnt, i;
    U32 ac, msk;

    PFEC_DL_FDESC pFecDL;
    PMDMA_FDESC pMdma;
    S32 ninbits, nencbits, nintlbits, Ncb;
    U16 smask;
    U8 *tmpin;
    U8 tmp8;

    // Compute CRC scrambling mask based on number of antennas.
    smask = 0x0000;
    if (nant == 2)
    {
        smask = 0xffff;
    }
    else if (nant == 4)
    {
        smask = 0xaaaa;
    }

    if (smask == 0)
    {
        // Allocate memory for temporary encoder inputs.
        tmpin = (U8*) MemAlloc(nin * sizeof(U8), CRAM_MEM, 0);

        // Copy input array to temporary encoder input array.
        memcpy(tmpin, in, nin);

        // Compute length of input array in bits.
        ninbits = nin << 3;

        // Compute length of CC encoder input array in bits (add 16 for CRC).
        nencbits = ninbits + 16;

        // Compute length of sub-block interleaver output arrays in bits.
        nintlbits = (nencbits + 31) >> 5;
        nintlbits = nintlbits << 5;

        // Compute length of rate matching buffer in bits.
        Ncb = 3 * nintlbits;

        // If X7 device, then implement CC encoder rate matching work-around
        if(DevInfoIsT4Kx7())
        {

            // Compute number of CC encoder output bits.
            // This is the number of bits that can be read from the rate matching
            // buffer prior to wrap-around.
            Kr = 3*nencbits;

            // Compute number of extra bits that must be generated externally
            // because of rate matching buffer wrap-around issue.
            Er = E - Kr;

            // If there are any extra bits then limit number of rate matching
            // outputs sent to FEC hardware.
            //if(Er > 0) E = Kr;

            // If E is odd then add 1 since FEC cannot generate odd number of outputs.
            //if( (E & 1) != 0) E = E + 1;

            //printf("Kr=%d, Er=%d, E=%d\n", Kr, Er, E);

        }

        pFecDL = BuildFecTcb_TX(0, // Type of encoding or decoding (0=CC, 1=CTC)
                                (U16) nencbits, // Code block size
                                4, // Type of CRC (4 means append 16-bit CRC)
                                0, // Number of initial filler bits in code block
                                0, // Number of modulation bits used (1,2,4,6)
                                0, // Coding rate
                                16, // Number of decoder 1/2 iterations
                                0, // CTC decoder automatic stop select
                                1, // Scrambling enable
                                (U16) smask, // Scrambling mask
                                0, // Sub-packet ID for the initial transmission
                                0, // Block interleaver bypass
                                0, // Encoder bypass
                                0, // Decoder bypass
                                0, // HARQ soft-combining enable
                                0, // Rate matching bypass
                                E, // Rate matching E parameter
                                0, // Rate matching k0 parameter
                                (U16) Ncb, // Rate matching Ncb parameter
                                1, // Channel interleaver bypass
                                12, // Number of channel interleaver columns (12 or 14)
                                tmpin, // Input data pointer
                                nin + 2, // Input data length
                                out, // Output buffer pointer
                                (7 + E) / 8, // Output buffer length
                                NULL, NULL);

        FecDlSetLastDesc(pFecDL, (void(*)(LPVOID)) FecDlBcCbDone, (LPVOID) NULL);

        FecDLReset();

        status_bc = 0;

        FecDlRun(pFecDL);

        while (status_bc == 0);

        // If X7 device, then implement CC encoder rate matching work-around
        if(DevInfoIsT4Kx7())
        {
            // If there are any extra bits that must be generated after FEC runs.
            if(Er > 0)
            {
                // Computer number of required extra bytes.
                nout = (Er + 7) >> 3;

                // Compute starting index for extra output buts.
                ind = Kr >> 3;

                // Compute shift count for generating extra bits.  Starting
                // bit of 1st extra output bit will be (7-scnt).
                scnt = Kr & 7;

                // If last rate matching output bit did not entirely fill last
                // output byte, then.
                if(scnt > 0)
                {
                    // Compute mask required to partially fill 1st byte.
                    msk = (255 << (8-scnt)) & 255;

                    //printf("nout=%d, ind=%d, scnt=%d, msk=%x\n", nout, ind, scnt, msk);

                    // Read 1st rate matching output byte.
                    ac = out[0];

                    // Write extra bits to 1st byte.
                    out[ind] = (U8) ( (out[ind] & msk) | (ac >> scnt) );

                    // For each remaining extra byte.
                    for(i = 1; i < nout; i++)
                    {
                        // Append next rate matching output byte to previous one.
                        ac = (ac << 8) | out[i];

                        // Write extra bits.
                        out[i+ind] = (U8) ( (ac >> scnt) & 255 );
                    }

                }
                // Else, last rate matching output entirely filled last byte.
                else
                {

                    //printf("nout=%d, ind=%d, scnt=%d\n", nout, ind, scnt);

                    // For each extra output byte.
                    for(i = 0; i < nout; i++)
                    {
                        // Write extra bits
                        out[i+ind] = out[i];
                    }

                }
            }

        }

        // De-allocate memory temporary encoder inputs.
        MemFree(tmpin, CRAM_MEM);

        return;
    }
    else
    {
        // Allocate memory for temporary encoder inputs.
        tmpin = (U8*) MemAlloc(nin * sizeof(U8), CRAM_MEM, 0);

        // Copy input array to temporary encoder input array.
        memcpy(tmpin, in, nin);

        // Compute length of input array in bits
        ninbits = nin << 3;

        // Compute length of CC encoder input array in bits (add 16 for CRC)
        nencbits = ninbits + 16;

        // Compute length of sub-block interleaver output arrays in bits
        nintlbits = (nencbits + 31) >> 5;
        nintlbits = nintlbits << 5;

        // Compute length of rate matching buffer in bits.
        Ncb = 3 * nintlbits;

        // Compute CRC for bytes on MDMA
        pMdma = MdmaAllocDesc (MDMA_HW_SYS, 1);
        _ASSERT_PTR (pMdma);
        MdmaSetCRC(pMdma, 0, 0x1021, 2);
        MdmaSetData(pMdma, tmpin, nin, tmpin, nin + 2);
        MdmaRunEx (MDMA_HW_SYS, pMdma, NULL, NULL);

        // Put CRC16 to temporary array
        tmp8 = tmpin[nin + 1];
        tmpin[nin + 1] = tmpin[nin];
        tmpin[nin] = tmp8;

        // Scramble CRC16
        tmp8 = smask & 0xff;
        tmpin[nin + 1] ^= tmp8;
        tmp8 = (smask >> 8) & 0xff;
        tmpin[nin] ^= tmp8;

        // If X7 device, then implement CC encoder rate matching work-around
        if(DevInfoIsT4Kx7())
        {

            // Compute number of CC encoder output bits.
            // This is the number of bits that can be read from the rate matching
            // buffer prior to wrap-around.
            Kr = 3*nencbits;

            // Compute number of extra bits that must be generated externally
            // because of rate matching buffer wrap-around issue.
            Er = E - Kr;

            // If there are any extra bits then limit number of rate matching
            // outputs sent to FEC hardware.
            //if(Er > 0) E = Kr;

            // If E is odd then add 1 since FEC cannot generate odd number of outputs.
            //if( (E & 1) != 0) E = E + 1;

            //printf("Kr=%d, Er=%d, E=%d\n", Kr, Er, E);

        }

        pFecDL = BuildFecTcb_TX(0, // Type of encoding or decoding (0=CC, 1=CTC)
                                (U16) nencbits, // Code block size
                                0, // Type of CRC (4 means append 16-bit CRC)
                                0, // Number of initial filler bits in code block
                                0, // Number of modulation bits used (1,2,4,6)
                                0, // Coding rate
                                16, // Number of decoder 1/2 iterations
                                0, // CTC decoder automatic stop select
                                0, // Scrambling enable
                                0, // Scrambling mask
                                0, // Sub-packet ID for the initial transmission
                                0, // Block interleaver bypass
                                0, // Encoder bypass
                                0, // Decoder bypass
                                0, // HARQ soft-combining enable
                                0, // Rate matching bypass
                                E, // Rate matching E parameter
                                0, // Rate matching k0 parameter
                                (U16) Ncb, // Rate matching Ncb parameter
                                1, // Channel interleaver bypass
                                12, // Number of channel interleaver columns (12 or 14)
                                tmpin, // Input data pointer
                                nin + 2, // Input data length
                                out, // Output buffer pointer
                                (7 + E) / 8, // Output buffer length
                                NULL, NULL);

        FecDlSetLastDesc(pFecDL, (void(*)(LPVOID)) FecDlBcCbDone, (LPVOID) NULL);

        FecDLReset();

        status_bc = 0;

        FecDlRun(pFecDL);

        while (status_bc == 0);

        // If X7 device, then implement CC encoder rate matching work-around
        if(DevInfoIsT4Kx7())
        {
            // If there are any extra bits that must be generated after FEC runs.
            if(Er > 0)
            {
                // Computer number of required extra bytes.
                nout = (Er + 7) >> 3;

                // Compute starting index for extra output buts.
                ind = Kr >> 3;

                // Compute shift count for generating extra bits.  Starting
                // bit of 1st extra output bit will be (7-scnt).
                scnt = Kr & 7;

                // If last rate matching output bit did not entirely fill last
                // output byte, then.
                if(scnt > 0)
                {
                    // Compute mask required to partially fill 1st byte.
                    msk = (255 << (8-scnt)) & 255;

                    //printf("nout=%d, ind=%d, scnt=%d, msk=%x\n", nout, ind, scnt, msk);

                    // Read 1st rate matching output byte.
                    ac = out[0];

                    // Write extra bits to 1st byte.
                    out[ind] = (U8) ( (out[ind] & msk) | (ac >> scnt) );

                    // For each remaining extra byte.
                    for(i = 1; i < nout; i++)
                    {
                        // Append next rate matching output byte to previous one.
                        ac = (ac << 8) | out[i];

                        // Write extra bits.
                        out[i+ind] = (U8) ( (ac >> scnt) & 255 );
                    }

                }
                // Else, last rate matching output entirely filled last byte.
                else
                {

                    //printf("nout=%d, ind=%d, scnt=%d\n", nout, ind, scnt);

                    // For each extra output byte.
                    for(i = 0; i < nout; i++)
                    {
                        // Write extra bits
                        out[i+ind] = out[i];
                    }

                }
            }
        }

        MemFree(tmpin, CRAM_MEM);

        return;

    }

#else   // _WIN32

    // WINDOWS
    S32 ninbits, nencbits, nintlbits;
    S32 i;
    U8 *inbits;
    U8 *outbits;

    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

#if (P_USE_MSPD != 0)   // If using MindSpeed functions
    S32 j, k;
    U8 *encin;
    U8 *encout;
    U8 *intlout, *intlnull;

#endif

    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM


#if (P_USE_TCON != 0)   // If using Turbo Concept functions
    S32 tc_nout, tc_smask;
    U8 *tc_encin, *tc_encout, *tc_encinwithcrc, *tc_qfdb;

#endif


    // Compute length of input array in bits
    ninbits = nin << 3;

    // Compute length of CC encoder input array in bits (add 16 for CRC)
    nencbits = ninbits + 16;

    // Compute length of sub-block interleaver output arrays in bits
    nintlbits = (nencbits + 31) >> 5;
    nintlbits = nintlbits << 5;

    // Allocate temporary space for unpacked input arrays
    inbits = (U8*)MemAlloc(ninbits, CRAM_MEM, 0);
    outbits = (U8*)MemAlloc(E, CRAM_MEM, 0);

    // Unpack input byte array into bit array (1 bit per byte)
    UNPACKBB(in, inbits, ninbits, 0);

    //printf("bs_tx_broadcast_channel_coding: nin=%d nant=%d E=%d ninbits=%d nencbits=%d nintlbits=%d\n", nin,nant,E,ninbits,nencbits,nintlbits);
    //i = getchar();

    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

#if (P_USE_MSPD != 0)   // If using MindSpeed functions
    // Allocate temporary space for CC encoder input array
    encin = (U8*) MemAlloc(nencbits, CRAM_MEM, 0);

    // Allocate temporary space for CC encoder output array
    encout = (U8*) MemAlloc(3 * nencbits, CRAM_MEM, 0);

    // Allocate temporary space for sub-block interleaver output arrays
    intlout = (U8*) MemAlloc(3 * nintlbits, CRAM_MEM, 0);
    intlnull = (U8*) MemAlloc(3 * nintlbits, CRAM_MEM, 0);

    // Create CC encoder input array by appending 16-bit CRC to input array
    AppendCRC16(inbits, encin, ninbits);

    // Scramble CRC bits according to antenna configuration
    // If 2 antenna ports, then scramble using 0xFFFF
    if(nant == 2)
    {
        for(i = ninbits; i < nencbits; i++)
        {
            encin[i] = encin[i] ^ 1;
        }

    }
    // Else if 4 antenna ports, then scramble using 0x5555

    else if(nant == 4)
    {
        for(i = ninbits+1; i < nencbits; i += 2)
        {
            encin[i] = encin[i] ^ 1;
        }
    }

    // Perform 1/3 rate CC encoding
    TBCCENC_1_3(encin, encout, nencbits);

    // Perform sub-block interleaving on d(0) encoder outputs
    j = 0;
    k = 0;
    i = CC_SubBlockInterleaver(&encout[j], &intlout[k], &intlnull[k], nencbits);

    // Perform sub-block interleaving on d(1) encoder outputs
    j = j + nencbits;
    k = k + nintlbits;
    i = CC_SubBlockInterleaver(&encout[j], &intlout[k], &intlnull[k], nencbits);

    // Perform sub-block interleaving on d(2) encoder outputs
    j = j + nencbits;
    k = k + nintlbits;
    i = CC_SubBlockInterleaver(&encout[j], &intlout[k], &intlnull[k], nencbits);

#if (P_USE_TCON == 0)   // If not also using Turbo Concept functions
    // Combine sub-block interleaver outputs and select bits for rate-matching
    //    CC_BitSelection(intlout, intlnull, out, (3*nintlbits), E);
    CC_BitSelection(intlout, intlnull, outbits, (3*nintlbits), E);
    // Pack output bits into output byte array
    PACKBB(outbits, out, E, 0);

#endif

#endif

    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM


#if (P_USE_TCON != 0)   // If using Turbo Concept functions
    tc_encin = (U8*)MemAlloc(nencbits, CRAM_MEM, 0);
    tc_encinwithcrc = (U8*)MemAlloc(nencbits, CRAM_MEM, 0);
    tc_encout = (U8*)MemAlloc(3*nintlbits, CRAM_MEM, 0);
    tc_qfdb = (U8*)MemAlloc(3*nintlbits, CRAM_MEM, 0);

    for(i = 0; i < ninbits; i++)
    {
        tc_encin[i] = inbits[i];
    }
    for(i = ninbits; i < nencbits; i++)
    {
        tc_encin[i] = 0;
    }

    tc_smask = 0;
    if(nant == 2) tc_smask = 0xffff;
    if(nant == 4) tc_smask = 0xaaaa;

    // Run Turbo Concept CTC encoder
    tc7000_lte_encode(1, // fec_mode (0=CTC, 1=CC)
        nencbits, // nb_bits_syst (frame size in bits)
        4, // crc_select (16-bit CRC generated)
        0, // nfiller_bits (number of filler bits in enc_in)
        1, // scramble_en (1=scramble applied)
        tc_smask, // scramble_mask (scramble mask applied)
        0, // bypass_interleaver (0=no, 1=yes)
        0, // bypass encoder (0=no, 1=yes)
        &tc_nout, // *nb_bits_encoded (enc_out length)
        tc_encin, // *enc_in (encoder input bit array)
        tc_encout, // *enc_out (encoder output bit array)
        tc_qfdb, // *qfdb (filler bit flag array)
        tc_encinwithcrc); // *enc_in_withcrc (input array with CRC included)

    //CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

#if (P_USE_MSPD != 0)   // If also using MindSpeed functions for verification
    printf("\n!!!!! Comparing MindSpeed and Turbo Concept Functional Outputs !!!!!\n\n");

    printf("nin=%d, nant=%d, E=%d, ninbits=%d, nencbits=%d, nintlbits=%d\n",
        nin, nant, E, ninbits, nencbits, nintlbits);

    printf("Checking encoder inputs with CRC (type CR to continue)\n");

    for(i = 0; i < nencbits; i++)
    {
        if(encin[i] != tc_encinwithcrc[i])
        printf("Encoder input error: encin[%d]=%d, tc_encinwithcrc[%d]=%d\n",
            i,encin[i],i,tc_encinwithcrc[i]);
    }
    i = getchar();

#if 0       // Set to 1 to compare outputs without sub-block interleaving
    printf("Checking encoder outputs without sub-block interleaving (type CR to continue)\n");

    j = 0;
    for(i = 0; i < (3*nencbits); i++)
    {
        if(encout[i] != tc_encout[j])
        printf("Encoder output error: encout[%d]=%d, tc_encout[%d]=%d\n",
            i,encout[i],j,tc_encout[j]);
        j = j + 3;
        if(j >= (3*nencbits) ) j = j - (3*nencbits) + 1;
    }
    i = getchar();

#else

    printf("Checking sub-block interleaved encoder outputs (type CR to continue)\n");

    for(i = 0; i < (3*nintlbits); i++)
    {
        if(intlout[i] != tc_encout[i])
        printf("Interleaver output error: intlout[%d]=%d, tc_encout[%d]=%d\n",
            i,intlout[i],i,tc_encout[i]);

        if(intlnull[i] != tc_qfdb[i])
        printf("Interleaver null output error: intlnull[%d]=%d, tc_qfdb[%d]=%d\n",
            i,intlnull[i],i,tc_qfdb[i]);
    }
    i = getchar();

#endif

#endif

    //CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

    // Combine sub-block interleaver outputs and select bits for rate-matching
    //    CC_BitSelection(tc_encout, tc_qfdb, out, (3*nintlbits), E);
    CC_BitSelection(tc_encout, tc_qfdb, outbits, (3*nintlbits), E);
    // Pack output bits into output byte array
    PACKBB(outbits, out, E, 0);

    MemFree(tc_encin, CRAM_MEM);
    MemFree(tc_encinwithcrc, CRAM_MEM);
    MemFree(tc_encout, CRAM_MEM);
    MemFree(tc_qfdb, CRAM_MEM);

#endif

    // De-allocate temporary arrays
    MemFree(inbits, CRAM_MEM);
    MemFree(outbits, CRAM_MEM);

    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

#if (P_USE_MSPD != 0)   // If using MindSpeed functions
    MemFree(encin, CRAM_MEM);
    MemFree(encout, CRAM_MEM);
    MemFree(intlout, CRAM_MEM);
    MemFree(intlnull, CRAM_MEM);

#endif

    return;

#endif

}
