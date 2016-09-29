//-------------------------------------------------------------------------------------------
/** @file tx_Mapper.c
 *
 * @brief Mapper function for mapping data bits to constellation points.
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

//-------------------------------------------------------------------------------------------

#include "tx_Mapper.h"

/*
 ARM-Ceva interface for TX mapper function

 ARM and Ceva use the data structure struct tTCB for passing parameters and data.
 The parameters for the TX mapper function are described in the followings.

 When calling the TX mapper function, ARM should create the data structure and
 allocate memories to pointers. The usage of the data structure is described in
 the followings.

 (1) TaskID
 TASKID_TX_MAPPER

 (2) ContextPtr

 --------------+---------+-----+-------------------------
 Byte offset   |  Date   |     |
 from pointer  |  type   | I/O |  Description
 --------------+---------+-----+-------------------------
 0        |  short  |  I  |  Modulation type
 |         |     |  1: QPSk
 |         |     |  2: 16 QAM
 |         |     |  3: 64 QAM
 |         |     |
 --------------+---------+-----+-------------------------
 2        |  short  |  O  |  Number of 16-bit words
 |         |     |  in the output buffer.
 |         |     |
 --------------+---------+-----+-------------------------

 (3) InputDataLen
 Number of bytes in the input buffer to be processed.

 (4) InputDataPtr
 Input buffer Pointer. ARM should allocate enough memories for this pointer.

 (5) OutputDataLen
 Number of bytes in the output buffer. Depends on the modulation type, the
 relationship between InputDataLen and OutputDataLen is given below.

 -----------+--------------------------------
 Modulation |
 type       |  OutputDataLen (in bytes)
 -----------+--------------------------------
 1      |  InputDataLen * 4 * 4
 -----------+--------------------------------
 2      |  InputDataLen * 2 * 4
 -----------+--------------------------------
 3      |  InputDataLen * 1 * 4
 -----------+--------------------------------

 (6) OutputDataPtr
 Output buffer Pointer. ARM should allocate enough memories for this pointer.
 output buffer is larger than input buffer. Please refer to the parameter
 OutputDataLen.

 */

/*
 File requirements for the following cases

 (1) MSC WiMAX
 tx_Mapper.c
 tx_Mapper_c.c
 WiMax16e_tx_Mapper_table.c

 (2) MSC LTE
 tx_Mapper.c
 tx_Mapper_c.c
 lte_tx_Mapper_table.c

 (3) Ceva WiMAX
 tx_Mapper.c
 tx_Mapper_a.asm
 WiMax16e_tx_Mapper_table.c

 (4) Ceva LTE
 tx_Mapper.c
 tx_Mapper_a.asm
 lte_tx_Mapper_table.c
 */

//-------------------------------------------------------------------------------------------
/** @brief Mapper function for mapping data bits to constellation points.
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param ctxdata  (INPUT) Pointer to the input data to be mapped to constellation points.
 *                  The bit stream from the FEC should be packed into bytes as followings.
 *                  1: QPSk
 *                     Every 8 bits are packed into a byte with the MSB first.
 *                     MSB of byte-0 is the bit 1, and LSB of byte-0 is the bit 8.
 *                     MSB of byte-1 is bit 9, and so on.
 *                     For QPSK, every byte will be mapped to 4 constellation points.
 *
 *                  2: 16-QAM
 *                     Every 8 bits are packed into a byte with the MSB first.
 *                     MSB of byte-0 is the bit 1, and LSB of byte-0 is the bit 8.
 *                     MSB of byte-1 is bit 9, and so on.
 *                     For 16-QAM, every byte will be mapped to 2 constellation points.
 *
 *                  3: 64-QAM
 *                     Every 6 bits are packed into a byte with the most significant 2 bits
 *                     always cleared to 0. The packing is shown in the followings.
 *                     Byte-1 (from MSB to LSB) :  0,  0,  bit-1, bit-2, bit-3, bit-4, bit-5, bit-6
 *                     Byte-2 (from MSB to LSB) :  0,  0,  bit-7, bit-8, bit-9, bit-10, bit-11, bit-12
 *                     ...
 *                     ...
 *                     For 64-QAM, every byte will be mapped to 1 constellation point.
 *
 *   @param Nsubcarriers     (INPUT) Number of sub-carriers
 *   @param skip             (INPUT) This is skip lenght to fix hardware issue
 *
 *   @param modType (INPUT) Modulation type
 *                     1: QPSk
 *                     2: 16-QAM
 *                     3: 64-QAM
 *
 *   @param MappingGainOut   (OUTPUT) Pointer to the output data. The data in this array is arranged as
 *                           I(1),Q(1), I(2),Q(2), I(3),Q(3), ....
 *                           where I(n) is the I-value of the nth constellation point,
 *                           and Q(n) is the Q-value of the nth constellation point,
 *
 *  @return   Number of values in the output array MappingGainOut[]. Number of constellation points
 *            is half of the returned value.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name           |Name in  |Direction|Length  |Type          |Data Format |Valid Range |Units |
 *  |               |Standard |         |        |              |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |ctxdata        |         |pInput   |len     |pReal         |(0:8:0)     |[0:255]     |      |
 *  ----------------------------------------------------------------------------------------------
 *  |Nsubcarriers   |         |Input    |1       |Real          |(0:32:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  |skip           |         |Input    |1       |Real          |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  |modType        |         |Input    |1       |Real          |(0:16:0)    |[0:4]       |      |
 *  ----------------------------------------------------------------------------------------------
 *  |MappingGainOut |         |pOutput  |1       |pComplex Array|(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------------
 *  |Block Name          |tx_Mapper                             |
 *  -------------------------------------------------------------
 *  |Input Streams       |ctxdata                               |
 *  -------------------------------------------------------------
 *  |Output Streams      |MappingGainOut                        |
 *  -------------------------------------------------------------
 *  |Config Parameters   |Nsubcarriers,modType,skip             |
 *  -------------------------------------------------------------
 *  |Status Parameters   |2*Number_of_constellation_points      |
 *  -------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Every byte of the input array ctxdata[] will be used as an index to the mapping table pointed by the
 * pointer (S16*) MODTAB. Depending on the selected mapping, the table should be set up accordingly as
 * described in the followings.
 *
 * - 1: BPSK <BR>
 *      For BPSK, the mapping table should be constructed as the C code fragment shown below. <BR>
 *
 *         \#define B0   0x7FFF  //     P_BPSK_1 <BR>
 *         \#define B1   0x8001  //    -P_BPSK_1 <BR>
 *         const unsigned short MODTAB_BPSK_256[2048] = { <BR>
 *            B0, B0, B0, B0, B0, B0, B0, B0,             //  0  0  0  0  0  0  0  0 <BR>
 *            B0, B0, B0, B0, B0, B0, B0, B1,             //  0  0  0  0  0  0  0  1 <BR>
 *           ... <BR>
 *           ... <BR>
 *            B1, B1, B1, B1, B1, B1, B1, B0,             //  1  1  1  1  1  1  1  0 <BR>
 *            B1, B1, B1, B1, B1, B1, B1, B1              //  1  1  1  1  1  1  1  1 <BR>
 *         }; <BR>
 *
 * - 2: QPSK <BR>
 *      For QPSK, the mapping table should be constructed as the C code fragment shown below. <BR>
 *
 *         \#define LTEP0        11585 <BR>
 *         \#define LTEP1       -11585 <BR>
 *         const short MODTAB_QPSK_256[2048] = { <BR>
 *           LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0,    //  0  0  0  0  0  0  0  0 <BR>
 *           LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1,    //  0  0  0  0  0  0  0  1 <BR>
 *           ... <BR>
 *           ... <BR>
 *           LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0,     //  1  1  1  1  1  1  1  0 <BR>
 *           LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1      //  1  1  1  1  1  1  1  1 <BR>
 *         }; <BR>
 *
 *
 * - 3: 16-QAM <BR>
 *      For 16-QAM, the mapping table should be constructed as the C code fragment shown below. <BR>
 *
 *         \#define P_LTE16QAM_3       15543 <BR>
 *         \#define P_LTE16QAM_1        5181 <BR>
 *         \#define N_LTE16QAM_1       -5181 <BR>
 *         \#define N_LTE16QAM_3      -15543 <BR>
 *         const short MODTAB_16QAM_256[1024] = { <BR>
 *            P_LTE16QAM_1,  P_LTE16QAM_1,   P_LTE16QAM_1,  P_LTE16QAM_1,    // 0000 0000	  (1,  1) <BR>
 *            P_LTE16QAM_1,  P_LTE16QAM_1,   P_LTE16QAM_1,  P_LTE16QAM_3,    // 0000 0001	  (1,  3) <BR>
 *           ... <BR>
 *           ... <BR>
 *            N_LTE16QAM_3,  N_LTE16QAM_3,   N_LTE16QAM_3,  N_LTE16QAM_1,    // 1111 1110 <BR>
 *            N_LTE16QAM_3,  N_LTE16QAM_3,   N_LTE16QAM_3,  N_LTE16QAM_3     // 1111 1111 <BR>
 *         }; <BR>
 *
 * - 4: 64-QAM <BR>
 *      For 64-QAM, the mapping table should be constructed as the C code fragment shown below. <BR>
 *
 *         \#define P_LTE64QAM_7    17696 <BR>
 *         \#define P_LTE64QAM_5    12640 <BR>
 *         \#define P_LTE64QAM_3    7584 <BR>
 *         \#define P_LTE64QAM_1    2528 <BR>
 *         \#define N_LTE64QAM_7   -17696 <BR>
 *         \#define N_LTE64QAM_5   -12640 <BR>
 *         \#define N_LTE64QAM_3   -7584 <BR>
 *         \#define N_LTE64QAM_1   -2528 <BR>
 *
 *         const short MODTAB_64QAM_64[128] = { <BR>
 *             P_LTE64QAM_3, P_LTE64QAM_3,      //  0  0  0  0  0  0      (3,  3) <BR>
 *             P_LTE64QAM_3, P_LTE64QAM_1,      //  0  0  0  0  0  1      (3,  1) <BR>
 *           ... <BR>
 *           ... <BR>
 *             N_LTE64QAM_7, N_LTE64QAM_5,      //  1  1  1  1  1  0     (-7, -5) <BR>
 *             N_LTE64QAM_7, N_LTE64QAM_7       //  1  1  1  1  1  1     (-7, -7) <BR>
 *         }; <BR>
 *
 *
 * <5> Cycle Count Formula:
 *     - For BPSK, the cycle count is 0.5 cycles/subcarrier <BR>
 *     - For QPSK, the cycle count is 0.625 cycles/subcarrier <BR>
 *     - For QAM16, the cycle count is 0.625 cycles/subcarrier <BR>
 *     - For QAM64, the cycle count is 1.0 cycles/subcarrier <BR>
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.2.0, "Physical Channels and Modulation", March 2008.
 *
 * \ingroup group_lte_tx_downlink_modulator
 **/
//------------------------------------------------------------------------------
void tx_Mapper(unsigned char ctxdata[], unsigned long Nsubcarriers, short skip,
               unsigned short modType, short MappingGainOut[])
{
    switch (modType)
    {
        //#if 0
#ifdef CEVAX
        // Use Ceva assembly code
        case MOD_QPSK:
        QPSK_TX_MAPPER_A(ctxdata, MappingGainOut, MODTAB_QPSK_256, Nsubcarriers, (long)skip);
        break;
        case MOD_16QAM:
        QAM16_TX_MAPPER_A(ctxdata, MappingGainOut, MODTAB_16QAM_256, Nsubcarriers, (long)skip);
        break;
        case MOD_64QAM:
        QAM64_TX_MAPPER_A(ctxdata, MappingGainOut, MODTAB_64QAM_64, Nsubcarriers);
        break;
#else
        // Use C code
        case MOD_QPSK:
            QPSK_TX_MAPPER(ctxdata, MappingGainOut, MODTAB_QPSK_256, (short) Nsubcarriers, skip);
            break;
        case MOD_16QAM:
            QAM16_TX_MAPPER(ctxdata, MappingGainOut, MODTAB_16QAM_256, (short) Nsubcarriers, skip);
            break;
        case MOD_64QAM:
            QAM64_TX_MAPPER(ctxdata, MappingGainOut, MODTAB_64QAM_64, (short) Nsubcarriers);
            break;

#endif

        default:
            break;
    }
    return;
}

