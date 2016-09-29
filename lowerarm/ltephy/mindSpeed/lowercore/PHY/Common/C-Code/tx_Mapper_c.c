#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#ifdef CEVAX
#include    <basic_op.h>
#else
#include    "basic_op_cc.h"
#endif

//================================================================================


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

void QPSK_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N, short N1)
{
    int i, k, Nhalf, tail, istart, n1;
    long *pL_OUT;
    long *pL_TABLE_QPSK = (long *) pTABLE;

    n1 = 0;
    if (N1 != 0)
    {
        n1 = 4 - N1;
    }
    Nhalf = (int) shr((short) (N - n1), 2); // 4 constellation points for each byte of input
    tail = N - n1 - shl((short) Nhalf, 2);
    pL_OUT = (long *) OUT;

#ifdef CEVAX
    k = IN[ 0 ] << 2; // 8 shorts = 4 longs
#else
    k = shl((unsigned short) IN[0], 2); // 8 shorts = 4 longs
#endif

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

    istart = 1;
    if (N1 == 0)
    {
        istart = 0;
    }

    for (i = 0; i < Nhalf; i++)
    {
#ifdef CEVAX
        k = IN[ istart + i ] << 2; // 8 shorts = 4 longs
#else
        k = shl((unsigned short) IN[istart + i], 2); // 8 shorts = 4 longs
#endif
        *pL_OUT++ = pL_TABLE_QPSK[k];
        *pL_OUT++ = pL_TABLE_QPSK[k + 1];
        *pL_OUT++ = pL_TABLE_QPSK[k + 2];
        *pL_OUT++ = pL_TABLE_QPSK[k + 3];
    }

#ifdef CEVAX
    k = IN[ istart + i ] << 2; // 8 shorts = 4 longs
#else
    k = shl((unsigned short) IN[istart + i], 2); // 8 shorts = 4 longs
#endif
    for (i = 0; i < tail; i++)
    {
        *pL_OUT++ = pL_TABLE_QPSK[k++];
    }

    return;
}

void QAM16_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N, short N1)
{
    int i, k, Nhalf, tail, istart;
    long *pL_OUT;
    long *pL_TABLE_16QAM = (long *) pTABLE;

    Nhalf = (int) shr((N - N1), 1); // 2 constellation points for each byte of input
    tail = N - N1 - shl(Nhalf, 1);
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

