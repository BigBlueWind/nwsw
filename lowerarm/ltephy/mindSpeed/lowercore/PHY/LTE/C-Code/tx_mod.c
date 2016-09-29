//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL tx_mod.c
 *
 * @brief Mapper function for mapping data bits to constellation points.
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "basic_op_cc.h"

//================================================================================


//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 * 
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   char *IN DOXYGEN_TO_DO
 *  @param   *OUT DOXYGEN_TO_DO
 *  @param   *pTABLE DOXYGEN_TO_DO
 *  @param   N  DOXYGEN_TO_DO
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
 *  |char *IN      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*OUT          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*pTABLE       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |N             |              |              |              |              |              |              |
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
void BPSK_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N)
{
    int i, k, Nhalf;
    long *pL_OUT;
    unsigned short *pS_TABLE_QPSK = (unsigned short *) pTABLE;

    Nhalf = (int) shr(N, 3); // 8 constellation points for each byte of input
    pL_OUT = (long *) OUT;

    for (i = 0; i < Nhalf; i++)
    {
        k = shl((unsigned short) IN[i], 3); // 8 shorts
        *pL_OUT++ = (long) pS_TABLE_QPSK[k];
        *pL_OUT++ = (long) pS_TABLE_QPSK[k + 1];
        *pL_OUT++ = (long) pS_TABLE_QPSK[k + 2];
        *pL_OUT++ = (long) pS_TABLE_QPSK[k + 3];
        *pL_OUT++ = (long) pS_TABLE_QPSK[k + 4];
        *pL_OUT++ = (long) pS_TABLE_QPSK[k + 5];
        *pL_OUT++ = (long) pS_TABLE_QPSK[k + 6];
        *pL_OUT++ = (long) pS_TABLE_QPSK[k + 7];
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
 *  @param   char *IN DOXYGEN_TO_DO
 *  @param   *OUT DOXYGEN_TO_DO
 *  @param   *pTABLE DOXYGEN_TO_DO
 *  @param   N DOXYGEN_TO_DO
 *  @param   N1 DOXYGEN_TO_DO
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
 *  |char *IN      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*OUT          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*pTABLE       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |N             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |N1            |              |              |              |              |              |              |
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
void QPSK_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N, short N1)
{
    int i, k, Nhalf, tail, istart; //, n1;
    long *pL_OUT;
    long *pL_TABLE_QPSK = (long *) pTABLE;

#if 0 // OLD
    n1 = 0;
    if (N1 != 0)
    {
        n1 = 4 - N1;
    }
    Nhalf = (int) shr((short) (N - n1), 2); // 4 constellation points for each byte of input
    tail = N - n1 - shl((short) Nhalf, 2);
#else // NEW
    Nhalf = (int) shr((short) (N - N1), 2); // 4 constellation points for each byte of input
    tail =  N - N1 - shl((short) Nhalf, 2);
#endif

    pL_OUT = (long *) OUT;

    k = shl((unsigned short) IN[0], 2); // 8 shorts = 4 longs

#if 0//OLD
    if (N1 == 1)
    {
        *pL_OUT++ = pL_TABLE_QPSK[k + 1];
        *pL_OUT++ = pL_TABLE_QPSK[k + 2];
        *pL_OUT++ = pL_TABLE_QPSK[k + 3];
    }
    else if (N1 == 2)
    {
        *pL_OUT++ = pL_TABLE_QPSK[k + 2];
        *pL_OUT++ = pL_TABLE_QPSK[k + 3];
    }
    else if (N1 == 3)
    {
        *pL_OUT++ = pL_TABLE_QPSK[k + 3];
    }
#else //NEW
    if (N1 == 1)
    {
        
        *pL_OUT++ = pL_TABLE_QPSK[k + 3];
    }
    else if (N1 == 2)
    {
        *pL_OUT++ = pL_TABLE_QPSK[k + 2];
        *pL_OUT++ = pL_TABLE_QPSK[k + 3];
    }
    else if (N1 == 3)
    {
        *pL_OUT++ = pL_TABLE_QPSK[k + 1];
        *pL_OUT++ = pL_TABLE_QPSK[k + 2];
        *pL_OUT++ = pL_TABLE_QPSK[k + 3];
    }
#endif

    istart = 1;
    if (N1 == 0)
    {
        istart = 0;
    }

    for (i = 0; i < Nhalf; i++)
    {
        k = shl((unsigned short) IN[istart + i], 2); // 8 shorts = 4 longs
        *pL_OUT++ = pL_TABLE_QPSK[k];
        *pL_OUT++ = pL_TABLE_QPSK[k + 1];
        *pL_OUT++ = pL_TABLE_QPSK[k + 2];
        *pL_OUT++ = pL_TABLE_QPSK[k + 3];
    }

    k = shl((unsigned short) IN[istart + i], 2); // 8 shorts = 4 longs
    for (i = 0; i < tail; i++)
    {
        *pL_OUT++ = pL_TABLE_QPSK[k++];
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
 *  @param   char *IN DOXYGEN_TO_DO
 *  @param   *OUT DOXYGEN_TO_DO
 *  @param   *pTABLE DOXYGEN_TO_DO
 *  @param   N DOXYGEN_TO_DO
 *  @param   N1  DOXYGEN_TO_DO
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
 *  |char *IN      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*OUT          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*pTABLE       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |N             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |N1            |              |              |              |              |              |              |
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
void QAM16_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N, short N1)
{
    int i, k, Nhalf, tail, istart;
    long *pL_OUT;
    long *pL_TABLE_16QAM = (long *) pTABLE;

    Nhalf = (int) shr((short) (N - N1), 1); // 2 constellation points for each byte of input
    tail = N - N1 - shl((short) Nhalf, 1);
    pL_OUT = (long *) OUT;

#ifdef CEVAX
    k = IN[ 0 ] << 1;
#else
    k = shl((unsigned short) IN[0], 1);
#endif

    if (N1 == 1)
    {
        *pL_OUT++ = pL_TABLE_16QAM[k + 1];
    }

    istart = 1;
    if (N1 == 0)
    {
        istart = 0;
    }

    for (i = 0; i < Nhalf; i++)
    {
#ifdef CEVAX
        k = IN[ istart + i ] << 1;
#else
        k = shl((unsigned short) IN[istart + i], 1);
#endif
        *pL_OUT++ = pL_TABLE_16QAM[k];
        *pL_OUT++ = pL_TABLE_16QAM[k + 1];
    }

#ifdef CEVAX
    k = IN[ istart + i ] << 1;
#else
    k = shl((unsigned short) IN[istart + i], 1); // 8 shorts = 4 longs
#endif

    if (tail != 0)
    {
        *pL_OUT = pL_TABLE_16QAM[k];
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
 *  @param   char *IN DOXYGEN_TO_DO
 *  @param   *OUT DOXYGEN_TO_DO
 *  @param   *pTABLE DOXYGEN_TO_DO
 *  @param   N  DOXYGEN_TO_DO
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
 *  |char *IN      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*OUT          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*pTABLE       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |N             |              |              |              |              |              |              |
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
void QAM64_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N)
{
    int i;
    long *pL_OUT;
    long *pL_TABLE_64QAM = (long *) pTABLE;

    pL_OUT = (long *) OUT;

    for (i = 0; i < N; i++)
    {
        *pL_OUT++ = pL_TABLE_64QAM[IN[i]];
    }
    return;
}

