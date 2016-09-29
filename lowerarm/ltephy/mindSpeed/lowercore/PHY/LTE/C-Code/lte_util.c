//-------------------------------------------------------------------------------------------
/** @file lte_util.c
 *
 * @brief LTE General Purpose Utiliy Functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.164 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef _WIN32
#include <crtdbg.h>
#endif
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
#include "math_ext32.h"
#include "math_adv.h"

#ifdef CEVA_INTRINSICS
#define CEVA_BUILD 1
#include "global.h"
#include <basic_op.h>
#include <asm-insn.h>
extern U8 *DataBuff_Bank3;

#define round_s(x)    round(x)

#else
#define CEVA_BUILD 0
#include "basic_op_cc.h"

#define round(x)    round_s(x)

#endif

#ifndef _WIN32
#ifndef CEVA_INTRINSICS
#include "heaps.h"
#include "4gmx_serv.h"
#include "svsrlogger.h"
#include "mlog.h"
#endif
#endif


void *MemAllocFrom(U8* p, U32 *offset, S32 length)
{
    void *ret;

    ret = (void *) &p[*offset];
    *offset += length;

    return ret;
}

#ifndef CEVA_INTRINSICS

//----------------------------------------------------------------------------------------
/** @brief MemAlloc  This function allocates memory from either Common memory heap
 *                   the DDR heap or the CRAM heap
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param size     Size in bytes of memory to be allocated
 *
 *  @param memId    Indicates the type of memory to be used for the allocatiom
 *                  0: Common Memory 1 DDR memory 3 CRAM memory
 *
 *  @param InitFlag When set initializes the allocated memory to zero
 *
 *  @return ptr     Pointer to memory allocated if null indicates failure
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------
 *  |Name       |Name In  |Direction  |Length |Type          |Format  |Valid Range|Units |
 *  |           |Standard |           |       |              |        |           |      |
 *  --------------------------------------------------------------------------------------
 *  |size       |         |input      |  1    | Unsigned int |(1:31:0)|(0,2^32)   |      |
 *  --------------------------------------------------------------------------------------
 *  |memId      |          |input      |     1    |    Unsigned int |(1:31:0)|[0,1,3]    |      |
 *  --------------------------------------------------------------------------------------
 *  |InitFlag   |         |input      |  1    |    Unsigned int |(1:31:0)|[0,1]      |      |
 *  --------------------------------------------------------------------------------------
 *  |ptr        |         |pOutput    |  1    | pvoid        |(1:31:0)|[0,2^32]   |      |
 *  --------------------------------------------------------------------------------------
 *
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------
 *  |Block Name          |MemAlloc             |
 *  --------------------------------------------
 *  |Input Stream(s)     |size, memid, InitFlag|
 *  --------------------------------------------
 *  |Output Stream(s)    |ptr                  |
 *  --------------------------------------------
 *  |Config Parameter(s) |                     |
 *  --------------------------------------------
 *  |Status Parameter(s) |                     |
 *  --------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * This function allocates memory from either Common memory heap
 * the DDR heap or the CRAM heap. It allows for the memory to be
 * zero initialized with the Initflag parameter
 *
 *
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//------------------------------------------------------------------------------------------
void* MemAlloc(U32 size, U32 memId, U32 InitFlag)
{
    void* ptr = 0;

#if WIN_BLD || CEVA_BUILD

    ptr = malloc(size); // Should be memory type dependent

#else
    int addr = __return_address() - 4;
#ifdef MX_HEAP_LEAKAGE_CONTROL
#define MX_HEAP_ALLOC(heap, size)   MxHeapAllocEx((heap), (size), _LR)
#else
#define MX_HEAP_ALLOC(heap, size)   MxHeapAlloc((heap), (size))
#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL
    UINT32 _LR = MX_GET_RADDR;
#endif

    switch (memId)
    {
        case COMMON_MEM:
            ptr = malloc(size);
            break;

        case DDR_MEM:
            ptr = MX_HEAP_ALLOC(AppHeap, size);
            break;

        case DDR1_MEM:
            ptr = MX_HEAP_ALLOC(DDR1Heap, size);
            break;

        case DDR1_NCNB_MEM:
            ptr = MX_HEAP_ALLOC(DDR1NcnbHeap, size);
            break;

        case CRAM_MEM:
            ptr = MX_HEAP_ALLOC(DspHeap, size);
            break;

        case NCNB_MEM:
            ptr = MX_HEAP_ALLOC(NcnbHeap, size);
            break;

        case IRAM_MEM:
            ptr = MX_HEAP_ALLOC(IramHeap, size);
            break;

        case PHY_MEM:
            ptr = MX_HEAP_ALLOC(PRV_CORE_HEAP, size);
            break;

        default:
        {
            printf("Scheduler: unknown Memory type !!!\r");
        }
    }

    if (ptr == NULL)
    {
        PHEAP pHeapObj;
        ICHEAP * pHeap;
        HANDLE HeapHandle = DspHeap;

        switch (memId)
        {
            case DDR_MEM:
                HeapHandle = AppHeap;
                break;

            case DDR1_MEM:
                HeapHandle = DDR1Heap;
                break;

            case DDR1_NCNB_MEM:
                HeapHandle = DDR1NcnbHeap;
                break;

            case CRAM_MEM:
                HeapHandle = DspHeap;
                break;

            case NCNB_MEM:
                HeapHandle = NcnbHeap;
                break;

            case IRAM_MEM:
                HeapHandle = IramHeap;
                break;

            case PHY_MEM:
                HeapHandle = PRV_CORE_HEAP;
                break;
        }

        pHeapObj = MxiGetHeapDescr(MxInqContext(), HeapHandle);
        pHeap = (ICHEAP*) pHeapObj->HeapStorage;

        uart_printf("Can't allocate %d bytes: memId = %d (Free %d) LR=%X\n", size, memId,
                    MxHeapGetFreeSpace(HeapHandle), addr);

        uart_printf("Can only provide %d bytes (%X)\r\n", MxHeapGetMaxFreeBlockSize(HeapHandle),
                    pHeap->FreeBlock);

        if(SvsrLoggerGetMask())
            SvsrLoggerFlush();

        if (MLogGetMask() != 0)
            MLogPrint();

        while (1);
    }

#endif

#ifdef _WIN32
    {
        int leakChk;
        if (ptr == NULL)
            leakChk = _CrtDumpMemoryLeaks();
        _ASSERT(ptr != NULL);
    }
#endif

#if 0
    if (ptr == 0)
    {
#if WIN_BLD || CEVA_BUILD
#ifdef _WIN32
        printf("Can't allocate memory !!!\n");
        //     exit(1);
#else
        //     exit(1);
#endif

#else
        printf("Can't allocate memory !!!\n");
        i = 1;
        while(i)
        {
            tmp = tmp;
        }
        return 0;
#endif
    }
#endif

    if (InitFlag == 1)
    {
        memset(ptr, 0, size);
    }

#if 0
#ifndef _WIN32
    MxiRestInt(irq);
#endif
#endif
    return ptr;
}

//----------------------------------------------------------------------------------------
/** @brief MemFree   This function frees memory from either Common memory heap
 *                   the DDR heap or the CRAM heap
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param memId    Indicates the type of memory to be free
 *                  0: Common Memory 1 DDR memory 3 CRAM memory
 *
 *  @param  ptr     Pointer to memory to be free
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------
 *  |Name       |Name In  |Direction  |Length |Type          |Format  |Valid Range|Units |
 *  |           |Standard |           |       |              |        |           |      |
 *  --------------------------------------------------------------------------------------
 *  |memId      |          |input      |     1    |    Unsigned int |(1:31:0)|[0,1,3]    |      |
 *  --------------------------------------------------------------------------------------
 *  |ptr        |         |pInput     |  1    | pvoid        |(1:31:0)|[0,2^32]   |      |
 *  --------------------------------------------------------------------------------------
 *
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------
 *  |Block Name          |MemFree              |
 *  --------------------------------------------
 *  |Input Stream(s)     |memid, ptr           |
 *  --------------------------------------------
 *  |Output Stream(s)    |                     |
 *  --------------------------------------------
 *  |Config Parameter(s) |                     |
 *  --------------------------------------------
 *  |Status Parameter(s) |                     |
 *  --------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * This function frees  memory from either Common memory heap
 * the DDR heap or the CRAM heap.  *
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_utilities
 *
 *
 **/
//------------------------------------------------------------------------------------------
void MemFree(void* ptr, U32 memId)
{

#if WIN_BLD || CEVA_BUILD

    free(ptr);

#else
    MXRC rc = 0;
    UINT32 lr = MX_GET_RADDR;

#if 0
    switch (memId)
    {
        case COMMON_MEM:
            free(ptr);
            break;

        case DDR_MEM:
            rc = MxHeapFree(AppHeap, ptr);
            break;

        case DDR1_MEM:
            rc = MxHeapFree(DDR1Heap, ptr);
            break;

        case DDR1_NCNB_MEM:
            rc = MxHeapFree(DDR1NcnbHeap, ptr);
            break;

        case CRAM_MEM:
            rc = MxHeapFree(DspHeap, ptr);
            break;

        case NCNB_MEM:
            rc = MxHeapFree(NcnbHeap, ptr);
            break;

        case IRAM_MEM:
            rc = MxHeapFree(IramHeap, ptr);
            break;

        case PHY_MEM:
            rc = MxHeapFree(PRV_CORE_HEAP, ptr);
            break;

        default:
        {
            S32 i = 1;
            while (i)
            {
                printf("Scheduler: unknown Memory type !!!\r");
            }
        }
    }
#else
    rc = HeapFreeByPtr(ptr);
#endif
    if (FAILED(rc))
    {
        stop_printf ("Heap free returned error(rc=0x%x): lr=0x%x, ptr=0x%x, heap-id=%d\r\n",
                    rc,
                    lr,
                    ptr,
                    memId);

    }

#endif

    return;
}
#endif  //CEVA_INTRINSICS
#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   num DOXYGEN_TO_DO
 *  @param   den DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
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
 *  |num           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |den           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 LteDivideS(S16 num, S16 den, S16 type)
{

    S16 quotient;
    S16 abs_num, abs_den;
    S16 sign;
    S16 temp;

    sign = 0;
    //    if( mult(num,den) < 0)
    if (L_mpy_ll(num, den) < 0)
        sign = -1;

    if (num == 0 || den == 0)
        return (0);

    abs_num = abs_s(num);
    abs_den = abs_s(den);
    if (abs_num < abs_den)
    {
        if (type == 0 && sign == 0) // floor
            return (0);
        else if (type == 0 && sign == -1) // floor
            return (-1);
        else if (type == 1 && sign == 0) // ceil
            return (1);
        else
            //ceil
            return (0);
    }

    quotient = 1;
    temp = sub(abs_num, abs_den);
    while (temp >= abs_den)
    {

        quotient = add(quotient, 1);
        temp = sub(temp, abs_den);
    }

    if (temp == 0)
        return (quotient);
    else
    {
        if (type == 0 && sign == 0) // floor
            return (quotient);
        else if (type == 0 && sign == -1) // floor
            return (negate(add(quotient, 1)));
        else if (type == 1 && sign == 0) // ceil
            return (add(quotient, 1));
        else
            //ceil
            return (negate(quotient));
    }
}
#endif   //Don't need for CEVA

#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   IN DOXYGEN_TO_DO
 *  @param   *IN_swp DOXYGEN_TO_DO
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
 *  |IN            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*IN_swp       |              |              |              |              |              |              |
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
void bitswap(S16 IN, U16 *IN_swp)
{

    S16 K, B;
    U16 BB;
    S32 I;
    S16 temp1[16], temp2[16];

    // Initialize current input byte bit position
    K = 15;

    //  Do for each bit
    for (I = 0; I < 16; I++)
    {
        // Shift and extract next bit from input array
        B = 1 & (IN >> K);
        // Shift bit into correct position and output
        temp1[I] = (B << 0);
        // Decrement current input byte bit position
        K = K - 1;
    }

    //Swap bits
    for (I = 0; I < 16; I++)
    {
        temp2[I] = temp1[15 - I];
    }

    K = 15;
    BB = 0;
    for (I = 0; I < 16; I++)
    {
        // Shift and extract bit from input array
        B = 1 & (temp2[I] >> 0);
        // Shift bit into correct position and combine with others
        BB = BB | (B << K);
        // Decrement current output byte bit position
        K = K - 1;
        // If last output byte bit finished
        if (K < 0)
        {
            // Write byte into output array
            *IN_swp = (U16) BB;
        }
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
 *  @param   X DOXYGEN_TO_DO
 *  @param   b DOXYGEN_TO_DO
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
 *  |X             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
U16 bitget32(U32 X, S16 b)
{
    U16 bit;

    U32 mask;
    U32 L_temp;

    mask = 1 << (b - 1);
    L_temp = (S16) ((X & mask) >> (b - 1));
    // printf(" %u %u %u %ld %hd\n",X,mask, X&mask, (X & mask) >> b-1, b-1);
    bit = (S16) (L_temp);

    return (bit);
}
#endif //Don't need for CEVA
#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   num DOXYGEN_TO_DO
 *  @param   den DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
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
 *  |num           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |den           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 LteDivideL(S32 num, S32 den, S16 type)
{

    S16 quotient;
    S32 abs_num, abs_den;
    S16 sign;
    S32 temp;

    sign = 0;
    if (mult(extract_h(num), extract_h(den)) < 0)
        sign = -1;

    if (num == 0 || den == 0)
        return (0);

    abs_num = L_abs(num);
    abs_den = L_abs(den);
    if (abs_num < abs_den)
    {
        if (type == 0 && sign == 0) // floor
            return (0);
        else if (type == 0 && sign == -1) // floor
            return (-1);
        else if (type == 1 && sign == 0) // ceil
            return (1);
        else
            //ceil
            return (0);
    }

    quotient = 1;
    temp = L_sub(abs_num, abs_den);
    while (temp >= abs_den)
    {

        quotient = add(quotient, 1);
        temp = L_sub(temp, abs_den);
    }

    if (temp == 0)
        return (quotient);
    else
    {
        if (type == 0 && sign == 0) // floor
            return (quotient);
        else if (type == 0 && sign == -1) // floor
            return (negate(add(quotient, 1)));
        else if (type == 1 && sign == 0) // ceil
            return (add(quotient, 1));
        else
            //ceil
            return (negate(quotient));
    }
}

#endif  //Don't need for CEVA
/******************************************************************************
 FSCALE (ARRAY FRACTIONAL SCALING FUNCTION)
 ******************************************************************************

 FSCALE generates an output array which is the product of an input array
 and a fractional scale factor.  The calling parameters are defined below.

 IN    Input array pointer
 OUT    Output array pointer
 N    Number of array elements
 FRC    Fractional scale factor

 *****************************************************************************/

#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   int IN[] DOXYGEN_TO_DO
 *  @param   int OUT[] DOXYGEN_TO_DO
 *  @param   int N DOXYGEN_TO_DO
 *  @param   int FRC DOXYGEN_TO_DO
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
 *  |int IN[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |int OUT[]     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |int N         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |int FRC       |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void FSCALE(short int IN[], short int OUT[], short int N, short int FRC)
{
    short int I;

    for (I = 0; I < N; I++)
    {
        OUT[I] = mult(IN[I], FRC);
    }

    return;
}
#endif //Don't need for CEVA


/******************************************************************************
 CLEAR (CLEAR ARRAY ELEMENTS)
 ******************************************************************************

 CLEAR clears the elements of an array.  The calling arguments are shown below.

 IN    Input array pointer
 N    Number of input array elements

 *****************************************************************************/

#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   int IN[] DOXYGEN_TO_DO
 *  @param   int N DOXYGEN_TO_DO
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
 *  |int IN[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |int N         |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void CLEAR(short int IN[], short int N)
{
    short int I;

    for (I = 0; I < N; I++)
        IN[I] = 0;

    return;
}
#endif //Don't need for CEVA


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xmat DOXYGEN_TO_DO
 *  @param   *yvec DOXYGEN_TO_DO
 *  @param   xdim1 DOXYGEN_TO_DO
 *  @param   xdim2 DOXYGEN_TO_DO
 *  @param   ydim DOXYGEN_TO_DO
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
 *  |*xmat         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*yvec         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |xdim1         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |xdim2         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ydim          |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void c_mat_vec_mul_fx(S16 *xmat, S16 *yvec, short xdim1, short xdim2, short ydim, S16 *out_vec)
{
    S16 i, j, dimxi;
    S32 Ltemp_R, Ltemp_I;

    if (xdim2 != ydim)
    {
#ifdef _WIN32
        myprintf((" c_mat_vec_mul_fx not possible, xdim2 has to be equal to  ydim\n"));
        exit(-1);
#endif
    }

    for (i = 0; i < xdim1; i++)
    {
        Ltemp_R = 0L;
        Ltemp_I = 0L;
        dimxi = xdim2 * 2 * i;
        for (j = 0; j < xdim2; j++)
        {
            Ltemp_R = L_mac(Ltemp_R, xmat[dimxi + 2 * j], yvec[2 * j]);
            Ltemp_R = L_msu(Ltemp_R, xmat[dimxi + 2 * j + 1], yvec[2 * j + 1]);

            Ltemp_I = L_mac(Ltemp_I, xmat[dimxi + 2 * j + 1], yvec[2 * j]);
            Ltemp_I = L_mac(Ltemp_I, xmat[dimxi + 2 * j], yvec[2 * j + 1]);
        }
        out_vec[2 * i] = round(Ltemp_R);
        out_vec[2 * i + 1] = round(Ltemp_I);
    }
}

/*
 * Multiplication of a complex Matrix with a complex Matrix
 * Matrix is storage of row by row with Real and Imaginary entries
 * alternating.
 *
 * For a 2 by 3 matrix
 *   X(0)  X(1)  X(2)
 *   X(3)  X(4)  X(5)
 * Single dimension array storage is assumed to be:
 *
 * First row:
 * mat[0] = real(X(0))
 * mat[1] = imag(X(0))
 * mat[2] = real(X(1))
 * mat[3] = imag(X(1))
 * mat[4] = real(X(2))
 * mat[5] = imag(X(2))
 *
 * Second row:
 * mat[6] = real(X(3))
 * mat[7] = imag(X(3))
 * mat[8] = real(X(4))
 * mat[9] = imag(X(4))
 * mat[10] = real(X(5))
 * mat[11] = imag(X(5))
 *
 * Here xdim1 = 2, xdim2 = 3, and ydim = 3;
 *
 */
#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xmat DOXYGEN_TO_DO
 *  @param   *ymat DOXYGEN_TO_DO
 *  @param   xdim1 DOXYGEN_TO_DO
 *  @param   xdim2 DOXYGEN_TO_DO
 *  @param   ydim1 DOXYGEN_TO_DO
 *  @param   ydim2 DOXYGEN_TO_DO
 *  @param   *out_mat DOXYGEN_TO_DO
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
 *  |*xmat         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*ymat         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |xdim1         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |xdim2         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ydim1         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ydim2         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*out_mat      |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void c_mat_mat_mul_fx(S16 *xmat, S16 *ymat, short xdim1, short xdim2, short ydim1, short ydim2,
                      S16 *out_mat)
{
    S16 i, j, k, dimxi, dimyi;
    S32 Ltemp_R, Ltemp_I;

    if (xdim2 != ydim1)
    {
#ifdef _WIN32
        myprintf(" c_mat_mat_mul_fx not possible, xdim2 has to be equal to  ydim1\n");
        exit(-1);
#endif
    }
    /* for each row of output matrix */
    for (i = 0; i < xdim1; i++)
    {
        dimxi = xdim2 * 2 * i;
        dimyi = ydim2 * 2 * i;

        /* for each column of output matrix */
        for (j = 0; j < ydim2; j++)
        {
            Ltemp_R = 0L;
            Ltemp_I = 0L;
            for (k = 0; k < ydim1; k++)
            {
                Ltemp_R = L_mac(Ltemp_R, xmat[dimxi + 2 * k], ymat[ydim2 * k * 2 + 2 * j]);
                Ltemp_R = L_msu(Ltemp_R, xmat[dimxi + 2 * k + 1], ymat[ydim2 * k * 2 + 2 * j + 1]);
                Ltemp_I = L_mac(Ltemp_I, xmat[dimxi + 2 * k + 1], ymat[ydim2 * k * 2 + 2 * j]);
                Ltemp_I = L_mac(Ltemp_I, xmat[dimxi + 2 * k], ymat[ydim2 * k * 2 + 2 * j + 1]);
            }

            out_mat[dimyi + 2 * j] = round(Ltemp_R);
            out_mat[dimyi + 2 * j + 1] = round(Ltemp_I);
        }
    }
}
#endif

#ifndef CEVA_INTRINSICS

/* Complex Identity Matrix */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xmat DOXYGEN_TO_DO
 *  @param   dim DOXYGEN_TO_DO
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
 *  |*xmat         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |dim           |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void ceye_fx(S16 *xmat, short dim)
{
    short i, j, dimx;

    for (i = 0; i < dim; i++)
    {
        dimx = dim * 2 * i;
        for (j = 0; j < dim; j++)
        {
            if (i == j)
            {
                xmat[dimx + 2 * j] = 32767;
                xmat[dimx + 2 * j + 1] = 0;
            }
            else
            {
                xmat[dimx + 2 * j] = 0;
                xmat[dimx + 2 * j + 1] = 0;
            }
        }
    }
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   real_part DOXYGEN_TO_DO
 *  @param   imag_part DOXYGEN_TO_DO
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
 *  |real_part     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |imag_part     |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 get_angle_fx(S32 real_part, S32 imag_part)
{
    S16 temp1, temp2, temp3;
    S32 abs_img, abs_real;
    S16 expo1, expo2;
    S16 atan_res;
    S16 ret_value = 0;

    if (real_part == 0)
    {
        if (imag_part >= 0)
        {
            return (PI_FX_BY2); // fraction of PI
        }
        else
        {
            return (-PI_FX_BY2);
        }
    }
    else
    {
        abs_img = L_abs(imag_part);
        abs_real = L_abs(real_part);

        if (L_sub(abs_img, abs_real) < 0) // |x| <= 1
        {
            expo1 = norm_l(abs_img);
            expo2 = norm_l(abs_real);

            temp1 = round(L_shl(abs_img, (S16) (expo1-1)));
            temp2 = round(L_shl(abs_real, (S16) (expo2)));

            temp3 = divide_s(temp1, temp2);
            //-expo1+1 from normalization of num
            //+expo1 from normalization of den
            temp3 = shl(temp3, (S16) (-expo1 + 1 + expo2));

            atan_res = shr(atan_fx(temp3), 2); // to bring it to Q13
        }
        else // atan(x) = PI/2 -atan(1/x)
        {
            expo1 = norm_l(abs_real);
            expo2 = norm_l(abs_img);

            temp1 = round(L_shl(abs_real, (S16) (expo1-1)));
            temp2 = round(L_shl(abs_img, (S16) (expo2)));
            temp3 = divide_s(temp1, temp2);

            // -expo1+1 from normalization of num
            // +expo1 from normalization of den
            temp3 = shl(temp3, (S16) (-expo1 + 1 + expo2));

            atan_res = shr(atan_fx(temp3), 2); // to bring it to Q13
            atan_res = sub(PI_FX_BY2, atan_res);
        }

        if (real_part >= 0 && imag_part >= 0)
            return (atan_res);
        if (real_part < 0 && imag_part >= 0)
            return (sub(PI_FX, atan_res));
        if (real_part < 0 && imag_part < 0)
            return (sub(atan_res, PI_FX));
        if (real_part >= 0 && imag_part < 0)
            return (negate(atan_res));

        // if it reaches here return error
        return ret_value;
    }
}

#if 0

/*
 antan(x) = a1*x + a3*x^3 + a5*x^5 + a7*x^7 + a9*x^9
 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   val DOXYGEN_TO_DO
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
 *  |val           |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 atan_fx(S16 val)
{
#define a1 32763   // Q15
#define a3 -10823
#define a5 5903
#define a7 -2790
#define a9 683

    S16 n_val;
    S16 expo;
    S16 temp2, temp3, temp5, temp7, temp9;
    S32 L_temp;

    expo = norm_s(val);
    n_val = shl(val, expo);

    L_temp = L_mult(n_val,a1);
    temp2 = mult(n_val,n_val); //^2
    temp3 = mult(temp2,n_val); //^3
    temp5 = mult(temp2,temp3); //^5
    temp7 = mult(temp5,temp2); //^7
    temp9 = mult(temp7,temp2); //^9

    L_temp = L_mac(L_temp,temp3, a3);
    L_temp = L_mac(L_temp,temp5, a5);
    L_temp = L_mac(L_temp,temp7, a7);
    L_temp = L_mac(L_temp,temp9, a9);

    return(shr(round_s(L_temp), expo));
}
#else // More precision
/*
 antan(x) = a1*x + a3*x^3 + a5*x^5 + a7*x^7 + a9*x^9
 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   val DOXYGEN_TO_DO
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
 *  |val           |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 atan_fx(S16 val)
{
#define a1 32763   // Q15
#define a3 -10823
#define a5 5903
#define a7 -2790
#define a9 683

    S32 i;
    S16 n_val;
    S16 expo, expo2, expo3, expo5, expo7, min_expo, expo_temp;
    S16 expo_arr[5] =
    { 0 };
    S16 temp2, temp3, temp5, temp7;
    S16 ntemp2, ntemp3, ntemp5, ntemp7, ntemp9;
    S32 L_temp, L_temp3, L_temp5, L_temp7, L_temp9;

    expo = norm_s(val);
    n_val = shl(val, expo);
    L_temp = L_mult(n_val, a1);
    expo_arr[0] = expo;

    temp2 = mult(n_val, n_val); //^2
    expo2 = norm_s(temp2);
    ntemp2 = shl(temp2, expo2);
    expo_temp = 2 * expo_arr[0] + expo2;

    temp3 = mult(ntemp2, n_val); //^3
    expo3 = norm_s(temp3);
    ntemp3 = shl(temp3, expo3);
    L_temp3 = L_mult(ntemp3, a3);
    expo_arr[1] = expo_arr[0] + expo_temp + expo3;

    temp5 = mult(ntemp2, ntemp3); //^5
    expo5 = norm_s(temp5);
    ntemp5 = shl(temp5, expo5);
    L_temp5 = L_mult(ntemp5, a5);
    expo_arr[2] = expo_arr[1] + expo_temp + expo5;

    temp7 = mult(ntemp5, ntemp2); //^7
    expo7 = norm_s(temp7);
    ntemp7 = shl(temp7, expo7);
    L_temp7 = L_mult(ntemp7, a7);
    expo_arr[3] = expo_arr[2] + expo_temp + expo7;

    ntemp9 = mult(ntemp7, ntemp2); //^9
    L_temp9 = L_mult(ntemp9, a9);
    expo_arr[4] = expo_arr[3] + expo_temp;

    //Bring all exponents to a common one by finding mimimum instead of maximum
    //since we used normalization here, smallest exponent is actually "largest negative exponent"
    //We shift left with negative shift, rather than right
    min_expo = expo_arr[0];
    for (i = 1; i < 5; i++)
        if (expo_arr[i] < min_expo)
            min_expo = expo_arr[i];

    L_temp = L_shl(L_temp, (S16) (min_expo - expo_arr[0]));
    L_temp3 = L_shl(L_temp3, (S16) (min_expo - expo_arr[1]));
    L_temp5 = L_shl(L_temp5, (S16) (min_expo - expo_arr[2]));
    L_temp7 = L_shl(L_temp7, (S16) (min_expo - expo_arr[3]));
    L_temp9 = L_shl(L_temp9, (S16) (min_expo - expo_arr[4]));

    //Add all components now
    L_temp = L_add(L_temp, L_temp3);
    L_temp = L_add(L_temp, L_temp5);
    L_temp = L_add(L_temp, L_temp7);
    L_temp = L_add(L_temp, L_temp9);

    return (shr(round(L_temp), min_expo));
}
#endif

/*****************************************************************************
 *                                                                           *
 *   Function Name : L_divide                                                *
 *                                                                           *
 *   Purpose :                                                               *
 *             Fractionnal integer division of two 32 bit numbers.           *
 *             L_num / L_denom.                                              *
 *             L_num and L_denom must be positive and L_num < L_denom.       *
 *                                                                           *
 *   Inputs :                                                                *
 *                                                                           *
 *    L_num                                                                  *
 *             32 bit long signed integer whose value falls in the           *
 *             range : 0x0000 0000 < L_num < L_denom                         *
 *                                                                           *
 *    L_denom                                                                *
 *             32 bit positive normalized integer whose value falls in the   *
 *             range : 0x40000000 < L_denom < 0x7fffffff                     *
 *                                                                           *
 *   Return Value :                                                          *
 *                                                                           *
 *    L_div                                                                  *
 *             32 bit long signed integer whose value falls in the           *
 *             range : 0x0000 0000 <= L_div <= 0x7fff ffff.                  *
 *                                                                           *
 *  Algorithm:                                                               *
 *                                                                           *
 *  - find = 1/L_denom.                                                      *
 *      First approximation: approx = 1 / extract_h(L_denom)                 *
 *      1/L_denom = approx * (2.0 - L_denom * approx )                       *
 *                                                                           *
 *  - result = L_num * (1/L_denom)                                           *
 *****************************************************************************
 */
#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_num DOXYGEN_TO_DO
 *  @param   L_denom DOXYGEN_TO_DO
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
 *  |L_num         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |L_denom       |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S32 L_divide(S32 L_num, S32 L_denom)
{
    S16 approx;
    S32 L_div;

    if (L_num < 0 || L_denom < 0 || L_num > L_denom)
    {
        //printf("ERROR: Invalid input into L_divide! %ld %ld\n", L_num, L_denom);
        return (0);
    }

    //printf("ERROR: Invalid input into L_divide! %ld %ld\n", L_num, L_denom);
    /* First approximation: 1 / L_denom = 1/extract_h(L_denom) */

    approx = divide_s((S16) 0x3fff, extract_h(L_denom));
    //printf("approx %hd\n", approx);

    /* 1/L_denom = approx * (2.0 - L_denom * approx) */

    L_div = L_mpy_ls(L_denom, approx);

    L_div = L_sub((S32) 0x7fffffffL, L_div);

    L_div = L_mpy_ls(L_div, approx);
    //printf("L_div1 %ld\n", L_div);

    /* L_num * (1/L_denom) */

    L_div = L_mpy_ll(L_num, L_div);
    //printf("L_div2 %ld\n", L_div);

    L_div = L_shl(L_div, 2);

    return (L_div);
}
#endif //Don't need for CEVA

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   X DOXYGEN_TO_DO
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
 *  |X             |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 divide_tab_lookup(S16 X)
{
    S16 temp, X2, Y1, Y2, Y, A, B;
    S32 index_l, index_h;
    S32 L_temp1;

    temp = sub(X, 16384);
    index_l = shr(temp, 6); //Steps in X are 16384/256 = 64
    index_h = index_l + 1;

    // y= A*y1+ B*y2
    // A = (x2-x)/(x2-x1)
    // B = 1-A = (x-x1)/(x2-x1)

    X2 = add((S16) (16384), (S16) (index_h * 64));
    A = shr(sub(X2, X), (S16) (6 - 15)); //Q15
    B = sub(32767, A);
    Y1 = Inv_Tab[index_l];
    Y2 = Inv_Tab[index_h];

    L_temp1 = L_mult(A, Y1);
    Y = mac_r(L_temp1, B, Y2);
    //    L_temp2 = L_mult(B,Y2);
    //    Y = round_s(L_add(L_temp1,L_temp2));

    return (Y);
}

//-------------------------------------------------------------------------------------------
/** @brief Computes Upper Bound of IQ samples given a System Bandwidth
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   nFft Size of the FFT being used which is a function of the System Bandwidth
 *
 *  @return  Number of IQ samples per antenna for a given System Bandwitdh derived via the FFT size
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction |Length |Type           |Data Format  |Valid Range     |Units        |
 *  |              |standard |          |       |               |             |                |             |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nFft          |         |Input     |1      |unsigned short |(1:15:0)     |[128,..,2048]   |             |
 *  ----------------------------------------------------------------------------------------------------------
 *  |return value  |         |Output    |1      |signed int     |(1:31:0)     |[1920,..,30720] |             |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------
 *  |Block Name          |ComputeUpperBound          |
 *  --------------------------------------------------
 *  |Input Streams       |                           |
 *  --------------------------------------------------
 *  |Output Streams      |                           |
 *  --------------------------------------------------
 *  |Config Parameters   |nFft                       |
 *  --------------------------------------------------
 *  |Status Parameters   |Num IQ samples per Antenna |
 *  --------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function determines the Upper Bound on the number of IQ samples for one
 *  antenna given a system Bandwidth which is derived from the size of FFT in use
 *
 *
 *  <5> Cycle Count Formula:
 *  TBA
 *
 *
 *  <6> References:
 *  3GPP 36.211 and 36.104
 *
 *
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
#ifndef CEVA_INTRINSICS
S32 ComputeUpperBound(U16 nFft)
{
    S32 index = 0;
    U32 LTESAMPLES_PER_SF[6] =
    { 1920, 3840, 7680, 15360, 23040, 30720 };

    switch (nFft)
    {
        case 128:
            index = 0;
            break;
        case 256:
            index = 1;
            break;
        case 512:
            index = 2;
            break;
        case 1024:
            index = 3;
            break;
        case 1536:
            index = 4;
            break;
        case 2048:
            index = 5;
            break;
    }
    return (LTESAMPLES_PER_SF[index]);
}
#endif


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
// End added computation of upper bound of IQ samples
/** @brief Normalize 16bit signed array by (expo_reEst-expo_norm_reEst).
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param RXIN       - Pointer to Resource Grid for current symbol
 *  @param CHIN       - Pointer to Channel Estimage for current symbol
 *  @param length_b   - Number of IQ samples to skip to next Rx Antenna
 *  @param expo_reEst - Pointer to exponent for each element in RXIN
 *  @param expo_ch    - Pointer to exponent for each element in CHIN
 *  @param Nul_symb   - Number of Uplink symbols per slot
 *  @param expo_norm  - Normalization factor
 *  @param NOUT       - Length of RXIN and CHIN
 *  @param MO         - Number of Rx Antennas
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction    |Length    |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |             |          |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |RXIN       |         |pInput/pOutput|Mpusch_sc|pComplex Array|(1:0:15)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------
 *  |CHIN       |         |pInput/pOutput|Mpusch_sc|pComplex Array|(1:0:15)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------
 *  |length_b   |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------
 *  |expo_reEst |         |pInput        |1        |Real          |(1:15:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------
 *  |expo_ch    |         |pInput        |1        |Real          |(1:15:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------
 *  |Nul_symb   |         |Input         |1        |Real          |(1:15:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------
 *  |expo_norm  |         |pInput        |1        |Real          |(1:15:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------
 *  |NOUT       |         |Input         |1        |Real          |(1:15:0)    |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------
 *  |MO         |         |Input         |1        |Real          |(1:15:0)    |[1,2,4]     |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |rx_exponent_equalization_16bit       |
 *  ------------------------------------------------------------
 *  |Input Streams       |RXIN,CHIN                            |
 *  ------------------------------------------------------------
 *  |Output Streams      |RXIN,CHIN                            |
 *  ------------------------------------------------------------
 *  |Config Parameters   |length_b,expo_reEst,expo_ch,Nul_symb,|
 *  |                    |expo_norm,NOUT,MO                    |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Normalize 16bit signed array by (expo_reEst-expo_norm_reEst).
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
#ifndef CEVA_INTRINSICS
void rx_exponent_equalization_16bit(S16 *RXIN, S16 *CHIN, S32 length_b, S16 *expo_reEst,
                                    S16 *expo_ch, S16 L, S16 expo_norm, S16 NOUT, S16 MO)
{
    S32 mo;
    S32 k;
    S16 *ptr;

    for (mo = 0; mo < MO; mo++)
    {
        S16* reEst1 = &RXIN[mo * length_b];
        S16 shift1 = expo_reEst[mo] - expo_norm;

        if (shift1 != 0)
        {
            ptr = reEst1;
            for (k = 0; k < 2 * NOUT; k += 2)
            {
                ptr[k] = shl(ptr[k], shift1);
                ptr[k + 1] = shl(ptr[k + 1], shift1);
            }
        }
    }

    for (mo = 0; mo < MO; mo++)
    {
        S16* chEst = &CHIN[mo * length_b];
        S16 shift2 = expo_ch[mo * L] - expo_norm;

        if (shift2 != 0)
        {
            ptr = chEst;
            for (k = 0; k < 2 * NOUT; k += 2)
            {
                ptr[k] = shl(ptr[k], shift2);
                ptr[k + 1] = shl(ptr[k + 1], shift2);
            }
        }
    }
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   chEst[] DOXYGEN_TO_DO
 *  @param   Err[] DOXYGEN_TO_DO
 *  @param   Mpusch_rb DOXYGEN_TO_DO
 *  @param   kre[] DOXYGEN_TO_DO
 *  @param   length_kre DOXYGEN_TO_DO
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
 *  |chEst[]       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Err[]         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Mpusch_rb     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |kre[]         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length_kre    |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void Compute_phase_error(S16 chEst[], S32 Err[], S16 Mpusch_rb, S16 Mpusch_sc,
                         S16 *pScaleComputeErr_Tab)
{
    S32 i, I, J;
    S32 L_temp1;

    S16 index, temp1, temp2;

    index = Mpusch_rb - 1;
    temp1 = inverse_nrb[index];
    temp2 = inverse_nrb_expo[index];

    Err[0] = 0; // Real
    Err[1] = 0; // Imag


    for (i = 0; i < Mpusch_sc - 1; i++)
    {
        //Err = Err + //sum(conj(chEst(mi+CE_MI1).symbol(pilotCarrierSymbol+CE_MI1).element(k0 +
        //CE_MI1)) .* chEst(mi+CE_MI1).symbol(pilotCarrierSymbol+CE_MI1).element(k1 + +CE_MI1));
        I = i << 1;
        J = (i + 1) << 1;

        L_temp1 = L_mult(mult(chEst[I], temp1), chEst[J]);
        Err[0] = L_add(Err[0], L_shr(L_temp1, temp2));

        L_temp1 = L_mult(mult(chEst[I + 1], temp1), chEst[J + 1]);
        Err[0] = L_add(Err[0], L_shr(L_temp1, temp2));

        L_temp1 = L_mult(mult(chEst[I], temp1), chEst[J + 1]);
        Err[1] = L_add(Err[1], L_shr(L_temp1, temp2));

        L_temp1 = L_mult(mult(chEst[I + 1], temp1), chEst[J]);
        Err[1] = L_sub(Err[1], L_shr(L_temp1, temp2));
    }

    //Weighting
    Err[0] = L_mpy_ls(Err[0], pScaleComputeErr_Tab[index]);
    Err[1] = L_mpy_ls(Err[1], pScaleComputeErr_Tab[index]);

    // Err has scale of (2*sqr(scale)) or Q of (2*(14-expo_fac)+1) plus
    // sqr(max_scale) coming from normalization of dac samples
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Fs DOXYGEN_TO_DO
 *  @param   phaseOffset DOXYGEN_TO_DO
 *  @param   df DOXYGEN_TO_DO
 *  @param   Ndf DOXYGEN_TO_DO
 *  @param   Nfft DOXYGEN_TO_DO
 *  @param   Mpusch_sc DOXYGEN_TO_DO
 *  @param   NusedE DOXYGEN_TO_DO
 *  @param   derotation[] DOXYGEN_TO_DO
 *  @param   start_address DOXYGEN_TO_DO
 *  @param   islot DOXYGEN_TO_DO
 *  @param   *Noffset DOXYGEN_TO_DO
 *  @param   *expoNoffset DOXYGEN_TO_DO
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
 *  |Fs            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phaseOffset   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |df            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Ndf           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Nfft          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Mpusch_sc     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |NusedE        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |derotation[]  |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |start_address |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |islot         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Noffset      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*expoNoffset  |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void Compute_Derotation(S32 Fs, S16 phaseOffset, S16 df,  S16 Nfft, S16 Mpusch_sc,
                        S16 NusedE, S16 derotation[], S16 start_address, S16 islot, S16 *Noffset,
                        S16 *expoNoffset)
{
    S32 i;
    S32 L_temp1, L_temp2, L_temp3;

    S16 *fk;
    S16 expo1, expo2;
    S16 temp1, temp2, const1;
    S16 NegExpoNoffset, noffset;

    //= time offset in sec, where phaseOffset = 2.pi.df.Ndf.Toffset
    //= number of samples offset, where Toffset = Noffset*Tsam = Noffset/Fs
    //Noffset = (Fs * phaseOffset)/(2.0*PI*df*Ndf);
    L_temp1 = L_mpy_ls(Fs, phaseOffset);
    L_temp2 = ((short) df ) << 1;
    L_temp3 = L_mpy_ls(L_temp2, PI_FX);

    L_temp2 = L_abs(L_temp1);
    expo1 = norm_l(L_temp2);
    expo2 = norm_l(L_temp3);
    temp1 = round(L_shl(L_temp2, (S16)(expo1-1)));
    temp2 = round(L_shl(L_temp3, expo2));

#ifdef CEVA_INTRINSICS
    *Noffset = div_s(temp1,temp2);
    fk = (S16 *)(DataBuff_Bank3);
#else
    *Noffset = divide_s(temp1, temp2);
    fk = MemAlloc(1200 * sizeof(S16), CRAM_MEM, 0);
#endif

    if (L_temp1 < 0L)
        *Noffset = negate(*Noffset);

    *expoNoffset = 15 + expo1 - 1 - expo2; // 15 from divide_s

#ifndef CEVA_INTRINSICS
//    printf("TIMING_OFFSET %hd %hd\n", shr(*Noffset, *expoNoffset),
//           shift_r(*Noffset, negate(*expoNoffset)));
#endif
    // *Noffset = 0;
    // *expoNoffset = 0;

    if (islot == -1) // Regular PUSCH
    {
        const1 = (-NusedE) >> 1;
        for (i = 0; i < Mpusch_sc; i++)
            fk[i] = (S16) (const1 + i);
    }
    else if (islot == -3) // Regular PDSCH
    {
        const1 = (-NusedE) >> 1;
        for (i = 0; i < NusedE >> 1; i++)
            fk[i] = (S16) (const1 + i);

        const1 = 1 - (NusedE >> 1);
        for (i = (NusedE >> 1); i < NusedE; i++)
            fk[i] = (S16) (const1 + i);
    }
    else if (islot == -2) // SRS
    {
        fk[0] = (((-NusedE) >> 1) + start_address);
        for (i = 2; i < Mpusch_sc << 1; i += 2) // Mpusch_sc is just a generic variable in here
            fk[i >> 1] = fk[(i >> 1) - 1] + 2;
    }
    else if (islot == 0) // Lower Format2
    {
        const1 = (-NusedE / 2 + start_address);
        for (i = 0; i < Mpusch_sc; i++)
            fk[i] = (S16) (const1 + i);
    }
    else // Upper Format2
    {
        const1 = (NusedE / 2 - start_address);
        for (i = 0; i < Mpusch_sc; i++)
            fk[i] = (S16) (const1 + i);
    }

    //non-consecutive Resource Blocks.
    //Need to keep RB base and offset
    //array along with data

    //calculate array of rotation values for each subcarrier
    // cos(-2*PI*(fk[i]*df)*Noffset*Tsam) =
    // cos(-2*PI*(fk[i]*df)*Noffset/Fs) ==
    // cos(-2*PI*(fk[i])*Noffset/Nfft)
    temp2 = 4096 / Nfft;
    noffset = *Noffset;
    NegExpoNoffset = negate(*expoNoffset);

    //There is an error in Compute_Derotation_PartOne_A, some phases don't work.(09/24/10, SW)
#ifdef CEVA_INTRINSICS
    Compute_Derotation_PartOne_A(Mpusch_sc,noffset, NegExpoNoffset, temp2, fk, derotation);
#else
    Compute_Derotation_PartOne(Mpusch_sc, noffset, NegExpoNoffset, temp2, fk, derotation);
#endif

#ifndef CEVA_INTRINSICS
    MemFree(fk, CRAM_MEM);
#endif

}

void Compute_DerotationSRS(S32 Fs, S16 phaseOffset, S16 df,  S16 Nfft, S16 Mpusch_sc,
                        S16 NusedE, S16 start_address, S16 islot, S16 *Noffset,
                        S16 *expoNoffset)
{

    S32 L_temp1, L_temp2, L_temp3;


    S16 expo1, expo2;
    S16 temp1, temp2;

    //= time offset in sec, where phaseOffset = 2.pi.df.Ndf.Toffset
    //= number of samples offset, where Toffset = Noffset*Tsam = Noffset/Fs
    //Noffset = (Fs * phaseOffset)/(2.0*PI*df*Ndf);
    L_temp1 = L_mpy_ls(Fs, phaseOffset);
    L_temp2 = ((short) df ) << 1;
    L_temp3 = L_mpy_ls(L_temp2, PI_FX);

    L_temp2 = L_abs(L_temp1);
    expo1 = norm_l(L_temp2);
    expo2 = norm_l(L_temp3);
    temp1 = round(L_shl(L_temp2, (S16)(expo1-1)));
    temp2 = round(L_shl(L_temp3, expo2));


#ifdef CEVA_INTRINSICS
    *Noffset = div_s(temp1,temp2);
#else
    *Noffset = divide_s(temp1, temp2);
#endif


    if (L_temp1 < 0L)
        *Noffset = negate(*Noffset);

    *expoNoffset = 15 + expo1 - 1 - expo2; // 15 from divide_s
}
//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Mpusch_sc DOXYGEN_TO_DO
 *  @param   noffset DOXYGEN_TO_DO
 *  @param   NegExpoNoffset DOXYGEN_TO_DO
 *  @param   temp2 DOXYGEN_TO_DO
 *  @param   fk[] DOXYGEN_TO_DO
 *  @param   derotation[] DOXYGEN_TO_DO
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
 *  |Mpusch_sc     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |noffset       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |NegExpoNoffset|              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |temp2         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |fk[]          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |derotation[]  |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void Compute_Derotation_PartOne(S16 Mpusch_sc, S16 noffset, S16 NegExpoNoffset, S16 temp2,
                                S16 fk[], S16 derotation[])
{
    int i;
    int I, K;
    S32 L_temp1, L_temp2;
    S16 kb;
    for (i = 0; i < Mpusch_sc; i++)
    {
        I = i << 1;
        L_temp1 = (short) noffset * (short) fk[i];

        L_temp2 = L_shift_r(L_temp1, (S16) (NegExpoNoffset));

        //printf("der %hd %hd %hd %hd %hd %hd \n", fk[i],scale_fk, fk[i]/scale_fk,  temp1, shl(temp1, -expo1+1+expo2-shift_fk), temp2);
        //getchar();
        //Map index to the proper entry depending on Nfft

        kb = L_abs(temp2*L_temp2) & 0x0fff;
        //kb = L_abs(temp2 * (short) L_temp2) & 0x0fff;

        if (kb < 2048)
        {
            K = kb << 1;
            derotation[I] = Table_FFT16b_4096[K];
            derotation[I + 1] = Table_FFT16b_4096[K + 1];
            if (L_temp2 < 0)
            {
                derotation[I + 1] = negate(derotation[I + 1]);
            }

            //    printf("DEROT_L1 %hd %hd %f %hd %hd %f %f \n", fk[i]/scale_fk, temp2, abs_s(temp2)/(Nfft*1.0), index, kb, derotation[2*i]/32768.0, derotation[2*i+1]/32768.0);
        }
        else
        {
            kb = kb - 2048;
            K = kb << 1;
            derotation[I] = negate(Table_FFT16b_4096[K]);
            derotation[I + 1] = Table_FFT16b_4096[K + 1];
            if (L_temp2 >= 0)
            {
                derotation[I + 1] = negate(derotation[I + 1]);
            }

            //    printf("DEROT_H %hd %hd %f %hd %hd %f %f \n", fk[i]/scale_fk, temp2, abs_s(temp2)/(Nfft*1.0), index, kb, derotation[2*i]/32768.0, derotation[2*i+1]/32768.0);
        }

    }
}

/***************************************************************************
 *
 *   FUNCTION NAME: round_s
 *
 *   PURPOSE:
 *
 *     Round the 32 bit S32 into a 16 bit shortword with saturation.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     Perform a two's complement round_s on the input S32 with
 *     saturation.
 *
 *     This is equivalent to adding 0x0000 8000 to the input.  The
 *     result may overflow due to the add.  If so, the result is
 *     saturated.  The 32 bit round_sed number is then shifted down
 *     16 bits and returned as a S16.
 *
 *
 *   KEYWORDS: round_s
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief Round the 32 bit S32 into a 16 bit shortword with saturation.
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 32bit signed number
 *
 *  @return (var1 + 0x00008000) >> 16
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:31)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |round_s                              |
 *  ------------------------------------------------------------
 *  |Input Streams       |var1                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |return value                         |
 *  ------------------------------------------------------------
 *  |Config Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Round the 32 bit S32 into a 16 bit shortword with saturation.
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
#ifndef CEVA_INTRINSICS

S16 round_s(S32 L_var1)
{
    S32 L_Prod;

    L_Prod = L_add(L_var1, 0x00008000L); /* round_s MSP */

    return (extract_h(L_Prod));
}

#endif

/***************************************************************************
 *
 *   FUNCTION NAME: divide_s
 *
 *   PURPOSE:
 *
 *     Divide var1 by var2.  Note that both must be positive, and
 *     var1 >=  var2.  The output is set to 0 if invalid input is
 *     provided.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     In the case where var1==var2 the function returns 0x7fff.  The output
 *     is undefined for invalid inputs.  This implementation returns zero
 *     and issues a warning via stdio if invalid input is presented.
 *
 *   KEYWORDS: divide
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   var1 DOXYGEN_TO_DO
 *  @param   var2 DOXYGEN_TO_DO
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
 *  |var1          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var2          |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 divide_s(S16 var1, S16 var2)
{
    S32 L_div;
    S16 swOut;

    if (var1 < 0 || var2 <= 0 || var1 > var2)
    {
        /* undefined output for invalid input into divide_s */
        return (0);
    }

    if (var1 == var2)
    {
        return (0x7fff);
    }

    L_div = ((0x00008000L * (S32) var1) / (S32) var2);
    swOut = saturate(L_div);

    return (swOut);
}

/***************************************************************************
 *
 *   FUNCTION NAME: saturate
 *
 *   PURPOSE:
 *
 *     Limit the 32 bit input to the range of a 16 bit word.
 *
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   KEYWORDS: saturation, limiting, limit, saturate, 16 bits
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var1 DOXYGEN_TO_DO
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
 *  |L_var1        |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 saturate(S32 L_var1)
{
    S16 swOut;

    if (L_var1 > SW_MAXV)
    {
        swOut = SW_MAXV;
    }
    else if (L_var1 < SW_MIN)
    {
        swOut = SW_MIN;
    }
    else
        swOut = (S16) L_var1; /* automatic type conversion */

    return (swOut);
}

/***************************************************************************
 *
 *   FUNCTION NAME: shift_r
 *
 *   PURPOSE:
 *
 *     Shift and round_s.  Perform a shift right. After shifting, use
 *     the last bit shifted out of the LSB to round_s the result up
 *     or down.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *
 *   IMPLEMENTATION:
 *
 *     Shift and round_s.  Perform a shift right. After shifting, use
 *     the last bit shifted out of the LSB to round_s the result up
 *     or down.
 *
 *     If var2 is positive perform a arithmetic left shift
 *     with saturation (see shl() above).
 *
 *     If var2 is zero simply return var1.
 *
 *     If var2 is negative perform a arithmetic right shift (shr)
 *     of var1 by (-var2)+1.  Add the LS bit of the result to var1
 *     shifted right (shr) by -var2.
 *
 *     Note that there is no constraint on var2, so if var2 is
 *     -0xffff 8000 then -var2 is 0x0000 8000, not 0x0000 7fff.
 *     This is the reason the shl function is used.
 *
 *
 *   KEYWORDS:
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   var1 DOXYGEN_TO_DO
 *  @param   var2 DOXYGEN_TO_DO
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
 *  |var1          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var2          |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
#ifndef CEVA_INTRINSICS
S16 shift_r(S16 var1, S16 var2)
{
    S16 swOut, swRnd;

    if (var2 >= 0)
    {
        swOut = shl(var1, var2); // OP_COUNT(-1);
    }
    else
    {
        /* right shift */
        if (var2 < -15)
        {
            swOut = 0;
        }
        else
        {
            swRnd = shl(var1, (S16) (var2 + 1)) & 0x1;
            swOut = add(shl(var1, var2), swRnd);
        }
    }

    return (swOut);
}
#endif //Don't need for CEVA
/***************************************************************************
 *
 *   FUNCTION NAME: L_shift_r
 *
 *   PURPOSE:
 *
 *     Shift and round_s.  Perform a shift right. After shifting, use
 *     the last bit shifted out of the LSB to round_s the result up
 *     or down.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *
 *   IMPLEMENTATION:
 *
 *     Shift and round_s.  Perform a shift right. After shifting, use
 *     the last bit shifted out of the LSB to round_s the result up
 *     or down.  This is just like shift_r above except that the
 *     input/output is 32 bits as opposed to 16.
 *
 *     if var2 is positve perform a arithmetic left shift
 *     with saturation (see L_shl() above).
 *
 *     If var2 is zero simply return L_var1.
 *
 *     If var2 is negative perform a arithmetic right shift (L_shr)
 *     of L_var1 by (-var2)+1.  Add the LS bit of the result to
 *     L_var1 shifted right (L_shr) by -var2.
 *
 *     Note that there is no constraint on var2, so if var2 is
 *     -0xffff 8000 then -var2 is 0x0000 8000, not 0x0000 7fff.
 *     This is the reason the L_shl function is used.
 *
 *
 *   KEYWORDS:
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var1 DOXYGEN_TO_DO
 *  @param   var2 DOXYGEN_TO_DO
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
 *  |L_var1        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var2          |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S32 L_shift_r(S32 L_var1, S16 var2)
{
    S32 L_Out, L_rnd;

    if (var2 < -31)
    {
        L_Out = 0;
    }
    else if (var2 < 0)
    {
        /* right shift */
        L_rnd = L_shl(L_var1, (S16) (var2 + 1)) & 0x1;
        L_Out = L_add(L_shl(L_var1, var2), L_rnd);

    }
    else
    {
        L_Out = L_shl(L_var1, var2);

    }

    return (L_Out);
}

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *data_in DOXYGEN_TO_DO
 *  @param   size DOXYGEN_TO_DO
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
 *  |*data_in      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |size          |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void block_shift(S16 *data_in, S16 size, S16 expo)
{
    S32 i;

    for (i = 0; i < (S32) (size); i++)
        data_in[i] = shl(data_in[i], expo);
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief block_norm:  Block normalization of a vector
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed vector
 *  @param var2 - OUPUT, 16bit signed vector
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |data_in      |         |Input         |size     |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |data_out     |         |Output        |size     |Real           |(1:0:15)   |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |size         |         |Input         |1        |Real           |(1:15:0)   |[0:32767]   |      |
 *  --------------------------------------------------------------------------------------------------
 *  |headroom     |         |Input         |1        |Real           |(1:15:0)   |[0:15]      |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |block_norm                           |
 *  ------------------------------------------------------------
 *  |Input Streams       |data_in                              |
 *  ------------------------------------------------------------
 *  |Output Streams      |data_out                             |
 *  ------------------------------------------------------------
 *  |Config Parameters   |size, headroom                       |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Block normalization of a vector
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 block_norm(S16 * data_in, S16 *data_out, S32 size, S16 headroom)
{
    S32 i;
    S16 max, scnt, adata;

    max = abs_s(data_in[0]);
    for (i = 1; i < size; i++)
    {
        adata = abs_s(data_in[i]);
        if (adata > max)
            max = adata;
    }

    if (max == 0)
    {
        scnt = 15;
    }
    else
        scnt = (norm_s(max) - headroom);

    for (i = 0; i < size; i++)
    {
#ifndef CEVA_INTRINSICS
        data_out[i] = shift_r(data_in[i], scnt);
#else
        data_out[i] = shr_r(data_in[i], -scnt);
#endif
    }

    return (scnt);
}

/*******************************************************************/

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   * data DOXYGEN_TO_DO
 *  @param   size DOXYGEN_TO_DO
 *  @param   scnt DOXYGEN_TO_DO
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
 *  |* data        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |size          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |scnt          |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void block_denorm(S16 * data, S16 size, S16 scnt)
{
    S32 i;

    for (i = 0; i < (S32) (size); i++)
        data[i] = shr(data[i], negate(scnt));

    return;
}
#endif

//#ifndef CEVA_INTRINSICS

/*******************************************************************/
/*
 void CopyData(S16 *input, S16 *output, S32 length)
 {
 S32 i;

 for(i=0; i < length; i++)
 output[i] = input[i];
 }
 */

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   * data DOXYGEN_TO_DO
 *  @param   size DOXYGEN_TO_DO
 *  @param   headroom DOXYGEN_TO_DO
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
 *  |* data        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |size          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |headroom      |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 block_lnorm(S32 * data, S32 size, S16 headroom)
{
    S32 i;
    S16 scnt;
    S32 max, adata;

    max = L_abs(data[0]);
    for (i = 1; i < size; i++)
    {
        adata = L_abs(data[i]);
        if (adata > max)
            max = adata;
    }

    scnt = norm_l(max) - headroom;

    return (scnt);
}
//#endif //Don't need for CEVA

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *data DOXYGEN_TO_DO
 *  @param   *out DOXYGEN_TO_DO
 *  @param   size DOXYGEN_TO_DO
 *  @param   headroom DOXYGEN_TO_DO
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
 *  |*data         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*out          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |size          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |headroom      |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 norm32to16(S32 *data, S16 *out, S32 size, S16 headroom)
{
    S32 i;
    S16 scnt;
    S32 max, adata;

    max = L_abs(data[0]);
    for (i = 1; i < size; i++)
    {
        adata = L_abs(data[i]);
        if (adata > max)
            max = adata;
    }

    if (max == 0)
    {
        scnt = 31;
        return (scnt);
    }
    else
    {
        scnt = norm_l(max) - headroom;
    }

    for (i = 0; i < size; i++)
    {
        out[i] = round(L_shl(data[i], scnt));
    }

    return (scnt);
}

#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Num DOXYGEN_TO_DO
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
 *  |Num           |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
short Sqrt_lbc(long Num)
{
    int i;
    short Rez = 0;
    short Exp = 0x4000;
    long Acc;

    for (i = 0; i < 14; i++)
    {
        Acc = L_mult(add(Rez, Exp), add(Rez, Exp));
        if (Num >= Acc)
            Rez = add(Rez, Exp);

        Exp = shr(Exp, 1);
    }
    return Rez;
}
#endif //Don't need for CEVA
//Convert dB to linear
//Assumptions: Input is Q8 [-128*256 to 6*256]
//              Anything beyond 6 dB will be clamped to 2.0 as output
//             Output is Q14
//


//-------------------------------------------------------------------------------------------
/** @brief Converts a dB value to a linear scalar
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   Power_dB
 *
 *  @return  Linear Scalar equivalent in Q14 format
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name    |Name in   |Direction     |Length |Type                 |Data Format |Valid Range   |Units  |
 *  |        |Standard  |              |       |                     |            |              |       |
 *  ------------------------------------------------------------------------------------------------------
 *  |Power_dB|          |Input         |1      |real                 |(1:15:0)    |[-128,6]      |       |
 *  ------------------------------------------------------------------------------------------------------
 *  |return  |          |Output        |1      |real                 |(1:1:14)    |[0,32767]     |       |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |db to Q14      |
 *  --------------------------------------
 *  |Input Streams       | Power_dB      |
 *  --------------------------------------
 *  |Output Streams      |return value   |
 *  --------------------------------------
 *  |Config Parameters   |               |
 *  --------------------------------------
 *  |Status Parameters   |               |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function converts an input value expressed in dB into a linear scalar in Q14 format
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S16 Convert_dB_Linear(S16 Power_dB)
{
    S16 temp, expo;
    S16 scale_factor;
    S32 L_temp;

    //Convert from dB to linear
    // 10 ^(x/20) = 2^(log2(10) * x/20)
    temp = Power_dB;
    if (temp == 0)
        scale_factor = 16384; // Q14
    else
    {
        if (temp > 0)
            temp = negate(temp);
        L_temp = L_mult(temp, 21771); // 21771 is log2(10)/(32*10) in Q21
        L_temp = fnExp2(L_temp); // fnExp2 assumes argument < 0
        temp = round(L_temp);
        if (Power_dB > 0)
        {

            if (temp != 0)
            {
                expo = norm_s(temp);
                temp = shl(temp, expo);
                scale_factor = divide_tab_lookup(temp); // Q14
                scale_factor = shl(scale_factor, expo);
            }
            else
                scale_factor = 32767; //(1/0) is infinite

        }
        else
            scale_factor = shr(temp, 1); // make it Q14

    }
    return (scale_factor);
}

/***************************************************************************
 *
 *   FUNCTION NAME: fnExp2
 *
 *   PURPOSE:
 *     The purpose of this function is to implement a base two exponential
 *     2**(32*x) by polynomial approximation
 *
 *
 *   INPUTS:
 *
 *     L_Input
 *                     unnormalized input exponent (input range constrained
 *                     to be < 0).
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     LwIn
 *                     exponential output
 *
 *   DESCRIPTION:
 *
 *     polynomial approximation is used for the generation of the exponential
 *
 *     2**(32*X) = 0.1713425*X*X + 0.6674432*X + 0.9979554
 *                     c2              c1            c0
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_Input DOXYGEN_TO_DO
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
 *  |L_Input       |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
S32 fnExp2(S32 L_Input)
{

    /*_________________________________________________________________________
     |                                                                         |
     |                           Local Static Variables                        |
     |_________________________________________________________________________|
     */
 #if 0
    static S16 pswPCoefE[3] = // 2nd order fit
    { /* c2,   c1,    c0 */
    0x15ef, 0x556f, 0x7fbd };
#else
     static S16 pswPCoefE[5] = //4th order fit
    { /* c4, c3, c2,   c1,    c0 */
        220,        1735,        7838,       22708,       32767};
#endif

    /*_________________________________________________________________________
     |                                                                         |
     |                            Automatic Variables                          |
     |_________________________________________________________________________|
     */

    S16 swTemp1, swTemp2, swTemp3, swTemp4, swTemp;
    S32 LwIn;

    /*_________________________________________________________________________
     |                                                                         |
     |                              Executable Code                            |
     |_________________________________________________________________________|
     */

    /* initialize */
    /* ---------- */

    swTemp3 = 0x0020;

    /* determine normlization shift count */
    /* ---------------------------------- */

    swTemp1 = extract_h(L_Input);
    LwIn = L_mult(swTemp1, swTemp3);
    swTemp2 = extract_h(LwIn);

    /* determine un-normalized shift count */
    /* ----------------------------------- */

    swTemp3 = -0x0001;
    swTemp = sub(swTemp3, swTemp2);

    /* normalize input */
    /* --------------- */

    LwIn = LwIn & 0xffff;
    LwIn = L_add(LwIn, L_deposit_h(swTemp3));

    LwIn = L_shr(LwIn, 1);
    swTemp1 = extract_l(LwIn);

 #if 0 // 2nd order

    /* calculate x*x*c2 */
    /* ---------------- */

    swTemp2 = mult_r(swTemp1, swTemp1);
    LwIn = L_mult(swTemp2, pswPCoefE[0]);

    /* calculate x*x*c2 + x*c1 */
    /* ----------------------- */

    LwIn = L_mac(LwIn, swTemp1, pswPCoefE[1]);

    /* calculate x*x*c2 + x*c1 + c0 */
    /* --------------------------- */

    LwIn = L_add(LwIn, L_deposit_h(pswPCoefE[2]));

#else //4th order

     /* calculate x*x*x*x*c4 */
    /* ---------------- */

    swTemp2 = mult_r(swTemp1, swTemp1);
    swTemp3 = mult_r(swTemp2, swTemp1);
    swTemp4 = mult_r(swTemp2, swTemp2);

    LwIn = L_mult(swTemp4, pswPCoefE[0]);

    /* calculate x*x*x*x*c4 + x*x*x*c3 */
    /* ----------------------- */

    LwIn = L_mac(LwIn, swTemp3, pswPCoefE[1]);

    /* calculate x*x*x*x*c4 + x*x*x*c3 + x*x*c2 */
    /* ----------------------- */

    LwIn = L_mac(LwIn, swTemp2, pswPCoefE[2]);

    /* calculate x*x*x*x*c4 + x*x*x*c3 + x*x*c2 +x*c1 */
    /* ----------------------- */

    LwIn = L_mac(LwIn, swTemp1, pswPCoefE[3]);

    /* calculate x*x*x*x*c4 + x*x*x*c3 + x*x*c2 + x*c1 + c0 */
    /* --------------------------- */

    LwIn = L_add(LwIn, L_deposit_h(pswPCoefE[4]));
#endif


    /* un-normalize exponent if its requires it */
    /* ---------------------------------------- */

    if (swTemp > 0)
    {
        LwIn = L_shr(LwIn, swTemp);
    }

    /* return result */
    /* ------------- */

    return (LwIn);
}

//-------------------------------------------------------------------------------------------
/** @brief mult_complex_vec:  Complex Multiply of 2 vectors
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param vec1 - INPUT, 16bit signed vector
 *  @param vec2 - INPUT, 16bit signed vector
 *  @param vec3 - OUTPUT, 16bit signed vector
 *  @param N -   INPUT, 16bit unsigned number
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Vec1         |         |Input         |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Vec2         |         |Input         |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |VecOut       |         |Output        |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |N            |         |Input         |1        |Integer       |(1:15:0)   |[0:32767]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |mult_complex_vec                     |
 *  ------------------------------------------------------------
 *  |Input Streams       |Vec1, Vec2                           |
 *  ------------------------------------------------------------
 *  |Output Streams      |VecOut                               |
 *  ------------------------------------------------------------
 *  |Config Parameters   |N                                    |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  mult_complex_vec:  Complex Multiply of 2 vectors
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------

#ifndef CEVA_INTRINSICS
void mult_complex_vec(S16 Vec1[], S16 Vec2[], S16 VecOut[], S32 N)
{
    S32 i;

    for (i = 0; i < N; i++)
    {

        VecOut[2 * i] = sub(mult(Vec1[2 * i], Vec2[2 * i]), mult(Vec1[2 * i + 1], Vec2[2 * i + 1]));
        VecOut[2 * i + 1] = add(mult(Vec1[2 * i], Vec2[2 * i + 1]),
                                mult(Vec1[2 * i + 1], Vec2[2 * i]));
    }
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief computeCorrelation:  compute Cross Correlation vector between 2 vectors
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param vec1 - INPUT, 16bit signed vector
 *  @param vec2 - INPUT, 16bit signed vector
 *  @param vec3 - OUTPUT, 16bit signed vector
 *  @param N -   INPUT, 16bit unsigned number
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Vec1         |         |Input         |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Vec2         |         |Input         |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |VecOut       |         |Output        |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |N            |         |Input         |1        |Integer       |(1:15:0)   |[0:32767]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |computeCorrelation                   |
 *  ------------------------------------------------------------
 *  |Input Streams       |Vec1, Vec2                           |
 *  ------------------------------------------------------------
 *  |Output Streams      |VecOut                               |
 *  ------------------------------------------------------------
 *  |Config Parameters   |N                                    |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Compute Cross correlation vector between 2 vectors
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------

#ifndef CEVA_INTRINSICS
void computeCorrelation(S16 Vec1[], S16 Vec2[], S16 VecOut[], U16 N)
{
    U16 i;

    for (i = 0; i < N; i++)
    {

        VecOut[2 * i] = add(mult(Vec1[2 * i], Vec2[2 * i]), mult(Vec1[2 * i + 1], Vec2[2 * i + 1]));
        VecOut[2 * i + 1] = sub(mult(Vec1[2 * i + 1], Vec2[2 * i]),
                                mult(Vec1[2 * i], Vec2[2 * i + 1]));
    }
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief ratioCompute:  Compute ratio of 2 numbers and return output in specified Q factor
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param num - INPUT, 16bit signed number
 *  @param den - INPUT, 16bit signed number
 *  @param Qfac -   INPUT, 16bit Q factor
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |num          |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |den          |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Qfac         |         |Input         |1        |Integer       |(1:15:0)    |[0:15]      |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |computeRatio                         |
 *  ------------------------------------------------------------
 *  |Input Streams       |num, den                             |
 *  ------------------------------------------------------------
 *  |Output Streams      |return value                         |
 *  ------------------------------------------------------------
 *  |Config Parameters   |Qfac                                 |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  computeRatio:  Compute ratio of 2 numbers and return output in specified Q factor
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
#ifndef CEVA_INTRINSICS
S16 ratioCompute(S16 num, S16 den, S16 Qfac)
{

    S16 expo1, expo2;
    S16 temp1, temp2, temp3;

    expo1 = norm_s(num);
    expo2 = norm_s(den);
    temp1 = shl(num, (S16) (expo1 - 1));
    temp2 = shl(den, (S16) (expo2));
    temp3 = divide_s(temp1, temp2);

    //-expo1+1 from normalization of num
    //+expo1 from normalization of den
    temp3 = shl(temp3, (S16) (-expo1 + 1 + expo2));
    return (shr(temp3, (S16) (15 - Qfac))); // Qfac


}
#endif

//-------------------------------------------------------------------------------------------
/** @brief computePwr:  Compute Power of a complex vector
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param vec1 - INPUT, 16bit signed vector
 *  @param vec2 - OUTPUT, 16bit signed vector
 *  @param N -   INPUT, 16bit unsigned number
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Vec          |         |Input         |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |VecOut       |         |Output        |N        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |N            |         |Input         |1        |Integer       |(1:15:0)   |[0:32767]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |computePwr                           |
 *  ------------------------------------------------------------
 *  |Input Streams       |Vec                                  |
 *  ------------------------------------------------------------
 *  |Output Streams      |VevOut                               |
 *  ------------------------------------------------------------
 *  |Config Parameters   |N                                    |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Compute Power of a complex vector
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
#ifndef CEVA_INTRINSICS
void computePwr(S16 Vec[], S16 VecOut[], U16 N)
{
    U16 i;

    for (i = 0; i < N; i++)
    {

        VecOut[i] = add(mult(Vec[2 * i], Vec[2 * i]), mult(Vec[2 * i + 1], Vec[2 * i + 1]));
        //        if(VecOut[i]  == 0x7fff)
        //            printf("SAT_PWR \n");
    }
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief mac_complex_vec:  Complex Multiply Accumulate of 2 vectors
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param vec1 - INPUT, 16bit signed vector
 *  @param vec2 - INPUT, 16bit signed vector
 *  @param vec3 - OUTPUT, 16bit signed vector
 *  @param N -   INPUT, 16bit unsigned number
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Vec1         |         |Input         |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Vec2         |         |Input         |N        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |VecOut       |         |Output        |2        |Complex       |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Nzc          |         |Input         |1        |Integer       |(1:15:0)   |[0:32767]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |mac_complex_vec                      |
 *  ------------------------------------------------------------
 *  |Input Streams       |Vec1, Vec2                           |
 *  ------------------------------------------------------------
 *  |Output Streams      |VecOut                               |
 *  ------------------------------------------------------------
 *  |Config Parameters   |Nzc                                  |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  mac_complex_vec:  Complex Multiply Accumulate of 2 vectors
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
#ifndef CEVA_INTRINSICS
void mac_complex_vec(S16 Vec1[], S16 Vec2[], S16 VecOut[], U16 Nzc)
{

    U16 j;
    S32 Ltemp_R, Ltemp_I;

    Ltemp_R = 0;
    Ltemp_I = 0;
    for (j = 0; j < Nzc; j++)
    {
        Ltemp_R = L_mac(Ltemp_R, Vec1[2 * j], Vec2[2 * j]);
        Ltemp_R = L_msu(Ltemp_R, Vec1[2 * j + 1], Vec2[2 * j + 1]);

        Ltemp_I = L_mac(Ltemp_I, Vec1[2 * j + 1], Vec2[2 * j]);
        Ltemp_I = L_mac(Ltemp_I, Vec1[2 * j], Vec2[2 * j + 1]);
    }
    VecOut[0] = round(Ltemp_R);
    VecOut[1] = round(Ltemp_I);
}

#endif

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief computeAvgPwr:  compute Avg Power of a vector
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param vec - INPUT, 16bit signed vector
 *  @param N -   INPUT, 16bit unsigned number
 *
 *  @return (power of vec)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name            |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |                |Standard |              |         |              |            |            |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |xDetPwrProfile  |         |Input         |N        |Real          |(1:0:15)    |Full Range  |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |N               |         |Input         |1        |Integer       |(1:15:0)    |[0:32767]    |     |
 *  -----------------------------------------------------------------------------------------------------
 *  |return          |         |Output        |1        |Real          |(1:0:15)    |[0:32767]   |      |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |computeAvgPwr                        |
 *  ------------------------------------------------------------
 *  |Input Streams       |xDetPwrProfile                       |
 *  ------------------------------------------------------------
 *  |Config Parameters   |N                                    |
 *  ------------------------------------------------------------
 *  |Output Streams      |return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Compute average power in input Vector
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 computeAvgPwr(S16 xDetPwrProfile[], U16 N)
{
    U16 i;
    S32 AvgxDetPwrProfile;

    AvgxDetPwrProfile = 0;
    for (i = 0; i < N; i++)
    {
        AvgxDetPwrProfile = L_add(AvgxDetPwrProfile, L_deposit_l(xDetPwrProfile[i]));
    }

    //    printf("AVG %ld ", AvgxDetPwrProfile);
    AvgxDetPwrProfile = L_shr(AvgxDetPwrProfile, 10); // Since N is assumed 1024

    //    printf("%ld %hd\n", AvgxDetPwrProfile, extract_l(AvgxDetPwrProfile) );


    return (extract_l(AvgxDetPwrProfile));

}
#endif

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   array DOXYGEN_TO_DO
 *  @param   *array_ind DOXYGEN_TO_DO
 *  @param   num DOXYGEN_TO_DO
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
 *  |array         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*array_ind    |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |num           |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void SortMin(S16* array, S16 *array_ind, S16 num)
{

    S16 i, temp;

    next_pass0:

    for (i = 0; i < num - 1; i++)
    {
        if (array[i + 1] < array[i])
        {
            temp = array_ind[i];
            array_ind[i] = array_ind[i + 1];
            array_ind[i + 1] = temp;
            temp = array[i];
            array[i] = array[i + 1];
            array[i + 1] = temp;
            goto next_pass0;
        }
    }
}

#endif



//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   reDet DOXYGEN_TO_DO
 *  @param   Msc DOXYGEN_TO_DO
 *  @param   coeff DOXYGEN_TO_DO
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
 *  |reDet         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Msc           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |coeff         |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void Rotate(S16* reDet, S16 Msc, S16 coeff)
{
    S32 k, L_temp1, L_temp2;

    for (k = 0; k < Msc; k++)
    {
        L_temp1 = L_mult(reDet[2 * k], coeff);
        L_temp2 = L_mult(reDet[2 * k + 1], coeff);
        reDet[2 * k] = extract_h(L_sub(L_temp1, L_temp2));
        reDet[2 * k + 1] = extract_h(L_add(L_temp1, L_temp2));
    }
}


#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Vec1[] DOXYGEN_TO_DO
 *  @param   Vec2[] DOXYGEN_TO_DO
 *  @param   VecOut[] DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
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
 *  |Vec1[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Vec2[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |VecOut[]      |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void ComputeComplexConjMult(S16 Vec1[], S16 Vec2[], S16 VecOut[], S16 length)
{
    S32 i;
    S32 L_temp1, L_temp2;
    S32 L_temp3, L_temp4;
    S16 temp1, temp2, temp3, temp4;
    S16 temp5, temp6, temp7, temp8;

    //Apply minus-half-carrier shift
    for (i = 0; i < (S32) (length >> 1); i++)
    {
        temp1 = Vec1[4 * i];
        temp2 = Vec2[4 * i];
        temp3 = Vec2[4 * i + 1];
        temp4 = Vec1[4 * i + 1];
        temp5 = Vec1[4 * i + 2];
        temp6 = Vec2[4 * i + 2];
        temp7 = Vec2[4 * i + 3];
        temp8 = Vec1[4 * i + 3];

        L_temp1 = L_mult(temp1, temp2);
        L_temp2 = L_mult(temp4, temp2);

        L_temp3 = L_mult(temp5, temp6);
        L_temp4 = L_mult(temp8, temp6);

        VecOut[4 * i] = mac_r(L_temp1, temp4, temp3);
        VecOut[4 * i + 1] = msu_r(L_temp2, temp3, temp1);

        VecOut[4 * i + 2] = mac_r(L_temp3, temp8, temp7);
        VecOut[4 * i + 3] = msu_r(L_temp4, temp5, temp7);
    }

    return;
}
#endif //CEVA_INTRINSICS

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   i DOXYGEN_TO_DO
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
 *  |i             |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void delay(S32 i)
{
#ifdef LOWERARM

    int a, b, c;
    c = 0;
    a = 3;
    b = 5;
    for (i = 0; i < 100000; i++)
    {
        c += a * b;
    }
#endif
    return;
}

#endif


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Err[] DOXYGEN_TO_DO
 *  @param   MErr[] DOXYGEN_TO_DO
 *  @param   Err_expo DOXYGEN_TO_DO
 *  @param   MErr_expo[] DOXYGEN_TO_DO
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
 *  |Err[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |MErr[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Err_expo        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |MErr_expo        |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void RunningAverage0(ULRXINPUTPARAMS *pRxInParams, S32 *Err, S16 *ErrExpoPusch, S16 *ErrExpo, S32 *ErrAvg,
                    S16 *mErrExpoTemp,  S32 *mErrTemp, S16 Ns, S32 ChanNum)
{


    S32 k;
    S16 Inv_num_pilots;//, L;
    S16 max_expo,  *Err_expo;
    S32 numRxAnt; //, Nul_symb, pilotCarrierSymbol;

    S32 Ltmp_0, Ltmp_1, Ltmp_2, Ltmp_3;
    S16 AvgScale, CAvgScale;
    S32 MErr[2];
    S16 MErr_expo;

    numRxAnt = pRxInParams->MO;
    //L = pRxInParams->L;
    //Nul_symb = pRxInParams->Nul_symb;
    if (pRxInParams->physChanType[ChanNum] == PUSCH) // Do it for PUSCH only, PUCCH ErrAvg is not computed anymore.
    {


        //saving running average before update to be used in bs_rx_ChEstimator_part1_fx
        mErrTemp[0] = pRxInParams->mErrAvg[2*ChanNum];
        mErrTemp[1] = pRxInParams->mErrAvg[2*ChanNum + 1];
        *mErrExpoTemp = pRxInParams->mErrExpo[ChanNum];

        //pilotCarrierSymbol = pRxInParams->pilotCarrierSymbol;
        Err_expo = ErrExpoPusch;
        /*}
        else if (pRxInParams->physChanType[ChanNum] == PUCCH)
        {
            pilotCarrierSymbol = 0;
            Err_expo = ErrExpoPucch;

        }*/

        max_expo = -32768;
        ErrAvg[0] = ErrAvg[1] = 0;

        // Find common expo (max)
        for (k = 0; k < numRxAnt * Ns; k++)
        {
            if (max_expo < Err_expo[k])
            {
                max_expo = Err_expo[k];
            }
        }

        *ErrExpo = max_expo;
        Inv_num_pilots = 16384; // 1 antenna
        if (numRxAnt == 2)
            Inv_num_pilots = 8192;
        else if (numRxAnt == 4)
            Inv_num_pilots = 4096;

        //Average accross all antennas and slots
        for (k = 0; k < numRxAnt * Ns; k++)
        {
            //Bring to same exponent, before averaging, 2 comes from squaring in Compute_phase_error
            Ltmp_1 = L_shr(Err[2*k], (S16) 2 * (max_expo - Err_expo[k]));
            Ltmp_2 = L_shr(Err[2*k + 1], (S16) 2 * (max_expo - Err_expo[k]));

            // average over num_pilots
            ErrAvg[0] = L_add(ErrAvg[0], L_mpy_ls(Ltmp_1, Inv_num_pilots));
            ErrAvg[1] = L_add(ErrAvg[1], L_mpy_ls(Ltmp_2, Inv_num_pilots));
        }

        //Update of running average

        max_expo = *ErrExpo;

        MErr[0] = pRxInParams->mErrAvg[2*ChanNum];
        MErr[1] = pRxInParams->mErrAvg[2*ChanNum + 1];
        MErr_expo = pRxInParams->mErrExpo[ChanNum];

        /* The coefficients should be determined based on SNR and
         * the size of RB allocation.
         * We will use the modulation format instead of SNR and RB size
         * since the modulation format typically is set according to
         * the SNR
         */
        /* For  QPSK  - value is set to 0.92 = 30147 (Q15) */
        /* For 16 QAM - value is set to 0.6  = 19661 (Q15) */
        /* For 64QAM  - value is set to 0.2  =  6554 (Q15 ) */
        if ( pRxInParams->Qm[ChanNum] == 2 )
            AvgScale = 30147;
        else if ( pRxInParams->Qm[ChanNum] == 4 )
            AvgScale = 19661;
        else
            AvgScale = 6554;

        /* Set the average scale based on the running average of
           number of RBs accumulated */
        if ( pRxInParams->NumRbAvg[ChanNum] <= THRESH_TO_NUM_RB_AVG )
            AvgScale = 16384;

        if(MErr_expo == -32768)
            AvgScale = 0; // Use instantaneous

        // subtract the average scale
        CAvgScale = sub(32767, AvgScale);

        //Update Running mean with the above average
        if (MErr_expo <= max_expo)
        { //2*expo comes from fact that Err is Chest*Chest
            Ltmp_0 = L_shr(MErr[0], (S16) 2 * (max_expo - MErr_expo));
            Ltmp_1 = L_shr(MErr[1], (S16) 2 * (max_expo - MErr_expo));

            Ltmp_0 = L_mpy_ls(Ltmp_0, AvgScale);
            Ltmp_1 = L_mpy_ls(Ltmp_1, AvgScale);
            Ltmp_2 = L_mpy_ls(ErrAvg[0], CAvgScale);
            Ltmp_3 = L_mpy_ls(ErrAvg[1], CAvgScale);

            pRxInParams->mErrAvg[2*ChanNum]   = L_add(Ltmp_0, Ltmp_2);
            pRxInParams->mErrAvg[2*ChanNum+1] = L_add(Ltmp_1, Ltmp_3);
            pRxInParams->mErrExpo[ChanNum] = max_expo;
        }
        else
        {
            Ltmp_0 = L_shr(ErrAvg[0], (S16) 2 * (MErr_expo - max_expo));
            Ltmp_1 = L_shr(ErrAvg[1], (S16) 2 * (MErr_expo - max_expo));

            Ltmp_0 = L_mpy_ls(Ltmp_0, CAvgScale);
            Ltmp_1 = L_mpy_ls(Ltmp_1, CAvgScale);
            Ltmp_2 = L_mpy_ls(MErr[0], AvgScale);
            Ltmp_3 = L_mpy_ls(MErr[1], AvgScale);
            pRxInParams->mErrAvg[2*ChanNum]   = L_add(Ltmp_0, Ltmp_2);
            pRxInParams->mErrAvg[2*ChanNum+1] = L_add(Ltmp_1, Ltmp_3);
        }

    }
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Err[] DOXYGEN_TO_DO
 *  @param   ErrAvg[] DOXYGEN_TO_DO
 *  @param   max_expo_vec[] DOXYGEN_TO_DO
 *  @param   max_expo DOXYGEN_TO_DO
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
 *  |Err[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ErrAvg[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |max_expo_vec        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |max_expo        |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void RunningAveragePUCCH(ULRXINPUTPARAMS *pRxInParams, S32 *Err, S32 *ErrAvg,
                         S32 ChanNum, S16 *max_expo_vec, S16 max_expo, S32 *ErrSub, S16 *ErrSubExpo)
{


       S32 k, j;
       S16 Inv_num;

       S32 numRxAnt;
       S32 Ltmp_1, Ltmp_2;
       S32 ErrSubAvg[2];

       S16 expo;


       S16 Flag_pass;
       S16  min_expo; //This one is coming from block_lnorm in bs_rx_ChEstimator_PUCCH_part0

        numRxAnt = pRxInParams->MO;

        ErrAvg[0] = ErrAvg[1] = 0;
        ErrSubAvg[0] = ErrSubAvg[1] = 0;

        min_expo = 32767;

       // Find common expo (min)
        for (k = 0; k < numRxAnt * NUM_SLOTS_SUBFRM; k++)
        {
            if (min_expo > ErrSubExpo[k])
            {
               min_expo = ErrSubExpo[k];
            }
         }

        //Inv_num is (1/(numRxAnt * NUM_SLOTS_SUBFRM))

        Inv_num = 16384; // 1 antenna
        if (numRxAnt == 2)
            Inv_num = 8192;
        else if (numRxAnt == 4)
            Inv_num = 4096;

        //Average accross all antennas and slots
        for (k = 0; k < numRxAnt; k++)
        {
            for (j = 0; j < NUM_SLOTS_SUBFRM; j++)
            {

              //Bring to same exponent, before averaging, 2 comes from squaring in Compute_phase_error
               Ltmp_1 = L_shr(Err[2*(k*NUM_SLOTS_SUBFRM + j)], (S16) 2 * (max_expo - max_expo_vec[k*NUM_SLOTS_SUBFRM + j]));
               Ltmp_2 = L_shr(Err[2*(k*NUM_SLOTS_SUBFRM + j) + 1], (S16) 2 * (max_expo - max_expo_vec[k*NUM_SLOTS_SUBFRM + j]));

              // average over slot/antenna
               ErrAvg[0] = L_add(ErrAvg[0], L_mpy_ls(Ltmp_1, Inv_num));
               ErrAvg[1] = L_add(ErrAvg[1], L_mpy_ls(Ltmp_2, Inv_num));

               //Same for ErrSub
               expo = 2 * (max_expo - max_expo_vec[k*NUM_SLOTS_SUBFRM + j] + ErrSubExpo[k*NUM_SLOTS_SUBFRM + j] - min_expo);

               //Bring to same exponent, before averaging, 2 comes from squaring in Compute_phase_error
               Ltmp_1 = L_shr(ErrSub[2*(k*NUM_SLOTS_SUBFRM + j)], expo);
               Ltmp_2 = L_shr(ErrSub[1 + 2*(k*NUM_SLOTS_SUBFRM + j)], expo);

               // average over slot/antenna
               ErrSubAvg[0] = L_add(ErrSubAvg[0], L_mpy_ls(Ltmp_1, Inv_num));
               ErrSubAvg[1] = L_add(ErrSubAvg[1], L_mpy_ls(Ltmp_2, Inv_num));

            }
        }


        ErrSubAvg[1] = L_mpy_ls(ErrSubAvg[1], 1638); // 0.05 in Q15
        Flag_pass = 0;
        if(L_sub(ErrSubAvg[0], ErrSubAvg[1]) < 0L)
            Flag_pass = 1;

        pRxInParams->PucchValidTimeOffset[ChanNum] = Flag_pass;

}
//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Err[] DOXYGEN_TO_DO
 *  @param   MErr[] DOXYGEN_TO_DO
 *  @param   Err_expo DOXYGEN_TO_DO
 *  @param   MErr_expo[] DOXYGEN_TO_DO
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
 *  |Err[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |MErr[]        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Err_expo        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |MErr_expo        |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void RunningAverage(ULRXINPUTPARAMS *pRxInParams, S32 *Err1, S16 *ErrExpoPusch, S16 *ErrExpoPucch,
                    S16 Ns, S32 ChanNum)
{
    S32 Ltmp_0, Ltmp_1, Ltmp_2, Ltmp_3;
    S16 AvgScale, CAvgScale;
    S32 MErr[2];
    //S16 L;
    S16 max_expo, MErr_expo;
    //S32 numRxAnt, Nul_symb;

    //numRxAnt = pRxInParams->MO;
    //L = pRxInParams->L;
    //Nul_symb = pRxInParams->Nul_symb;

    if (pRxInParams->physChanType[ChanNum] == PUSCH) // Do it for PUSCH only, PUCCH ErrAvg is not computed anymore.
    {
        max_expo = ErrExpoPusch[0];

        MErr[0] = pRxInParams->mErrAvg[2*ChanNum];
        MErr[1] = pRxInParams->mErrAvg[2*ChanNum + 1];
        MErr_expo = pRxInParams->mErrExpo[ChanNum];

        /* The coefficients should be determined based on SNR and
         * the size of RB allocation.
         * We will use the modulation format instead of SNR and RB size
         * since the modulation format typically is set according to
         * the SNR
         */
        /* For  QPSK  - value is set to 0.92 = 30147 (Q15) */
        /* For 16 QAM - value is set to 0.6  = 19661 (Q15) */
        /* For 64QAM  - value is set to 0.2  =  6554 (Q15 ) */
        if ( pRxInParams->Qm[ChanNum] == 2 )
            AvgScale = 30147;
        else if ( pRxInParams->Qm[ChanNum] == 4 )
            AvgScale = 19661;
        else
            AvgScale = 6554;

        /* Set the average scale based on the running average of
           number of RBs accumulated */
        if ( pRxInParams->NumRbAvg[ChanNum] <= THRESH_TO_NUM_RB_AVG )
            AvgScale = 16384;

        if(MErr_expo == -32768)
            AvgScale = 0; // Use instantaneous

        // subtract the average scale
        CAvgScale = sub(32767, AvgScale);

        //Update Running mean with the above average
        if (MErr_expo <= max_expo)
        { //2*expo comes from fact that Err is Chest*Chest
            Ltmp_0 = L_shr(MErr[0], (S16) 2 * (max_expo - MErr_expo));
            Ltmp_1 = L_shr(MErr[1], (S16) 2 * (max_expo - MErr_expo));

            Ltmp_0 = L_mpy_ls(Ltmp_0, AvgScale);
            Ltmp_1 = L_mpy_ls(Ltmp_1, AvgScale);
            Ltmp_2 = L_mpy_ls(Err1[0], CAvgScale);
            Ltmp_3 = L_mpy_ls(Err1[1], CAvgScale);

            pRxInParams->mErrAvg[2*ChanNum]   = L_add(Ltmp_0, Ltmp_2);
            pRxInParams->mErrAvg[2*ChanNum+1] = L_add(Ltmp_1, Ltmp_3);
            pRxInParams->mErrExpo[ChanNum] = max_expo;
        }
        else
        {
            Ltmp_0 = L_shr(Err1[0], (S16) 2 * (MErr_expo - max_expo));
            Ltmp_1 = L_shr(Err1[1], (S16) 2 * (MErr_expo - max_expo));

            Ltmp_0 = L_mpy_ls(Ltmp_0, CAvgScale);
            Ltmp_1 = L_mpy_ls(Ltmp_1, CAvgScale);
            Ltmp_2 = L_mpy_ls(MErr[0], AvgScale);
            Ltmp_3 = L_mpy_ls(MErr[1], AvgScale);
            pRxInParams->mErrAvg[2*ChanNum]   = L_add(Ltmp_0, Ltmp_2);
            pRxInParams->mErrAvg[2*ChanNum+1] = L_add(Ltmp_1, Ltmp_3);
        }
    }
}



S32 LtePhyRxSduAverageUlCqi(U8 cqi, U32 isPusch, PUSED_INDEX_VARS pUsedIndexVars, U32 CurrTti, U32 Multi_PUCCH_PerRB)
{
    S32 returnVal = DEFAULT_CQI_RET_VAL;
    S32 currCqi = (S32)cqi;
    U32 diff;

    if (isPusch)
    {
        if (pUsedIndexVars->puschSinrPrevTti != SINR_INIT_VALUE)
        {
            diff = GetDiff(pUsedIndexVars->puschSinrPrevTti, CurrTti);
            if ((diff < UL_CQI_AVERAGING_PERIOD) && (pUsedIndexVars->puschSinrCnt < UL_CQI_AVERAGING_COUNT))
            {
                pUsedIndexVars->avgPuschSinr += currCqi;
                pUsedIndexVars->puschSinrCnt++;
                returnVal = pUsedIndexVars->puschSinrPrevSinr;
            }
            else
            {
                returnVal = pUsedIndexVars->avgPuschSinr / pUsedIndexVars->puschSinrCnt;
                pUsedIndexVars->avgPuschSinr = currCqi;
                pUsedIndexVars->puschSinrCnt = 1;
                pUsedIndexVars->puschSinrPrevTti = CurrTti;
                pUsedIndexVars->puschSinrPrevSinr = returnVal;
            }
        }
        else
        {
            returnVal = currCqi;
            pUsedIndexVars->avgPuschSinr = currCqi;
            pUsedIndexVars->puschSinrCnt = 1;
            pUsedIndexVars->puschSinrPrevTti = CurrTti;
            pUsedIndexVars->puschSinrPrevSinr = returnVal;
        }
    }
    else
    {
        if (pUsedIndexVars->pucchSinrPrevTti != SINR_INIT_VALUE)
        {
            diff = GetDiff(pUsedIndexVars->pucchSinrPrevTti, CurrTti);
            if ((diff < UL_CQI_AVERAGING_PERIOD) && (pUsedIndexVars->pucchSinrCnt < UL_CQI_AVERAGING_COUNT))
            {
                if (Multi_PUCCH_PerRB)      // This means that there is only 1 PUCCH in this RB :)
                {
                    pUsedIndexVars->avgPucchSinr += currCqi;
                    pUsedIndexVars->pucchSinrCnt++;
                }
                returnVal = pUsedIndexVars->pucchSinrPrevSinr;
            }
            else
            {
                returnVal = pUsedIndexVars->avgPucchSinr / pUsedIndexVars->pucchSinrCnt;
                pUsedIndexVars->avgPucchSinr = currCqi;
                pUsedIndexVars->pucchSinrCnt = 1;
                pUsedIndexVars->pucchSinrPrevTti = CurrTti;
                pUsedIndexVars->pucchSinrPrevSinr = returnVal;
            }
        }
        else
        {
            returnVal = currCqi;
            pUsedIndexVars->avgPucchSinr = currCqi;
            pUsedIndexVars->pucchSinrCnt = 1;
            pUsedIndexVars->pucchSinrPrevTti = CurrTti;
            pUsedIndexVars->pucchSinrPrevSinr = returnVal;
        }
    }

    return returnVal;
}

S16 LtePhyRxSduAverageUlTa(S16 currTa, U8 currCqi, U32 numRbs, PUSED_INDEX_VARS pUsedIndexVars)
{
    S16 retTa = 31;

    if (pUsedIndexVars->puschTaCnt > UL_TA_AVERAGING_TIME)
    {
        retTa = (S16)(pUsedIndexVars->avgPuschTa / pUsedIndexVars->puschTaCnt);
        pUsedIndexVars->puschTaCnt = 1;
        pUsedIndexVars->avgPuschTa = 31;
    }

    if(currCqi > UL_TA_AVERAGING_MIN_CQI || numRbs > UL_TA_AVERAGING_MIN_RESOURCE_BLOCKS)
    {
        // Bias the current TA
        if (currTa >= 35)       // Get it down to 34
            currTa -= 3;
        else if (currTa < 32)  // Get it upto 32
            currTa -= 2;
        else
            currTa = 31;

        if (currTa < 0)
            currTa = 0;
    }
    else
    {
        currTa = 31;
    }

    pUsedIndexVars->avgPuschTa += (currTa & 0x3F);
    pUsedIndexVars->puschTaCnt++;

    return retTa;
}



//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Err[] DOXYGEN_TO_DO
 *  @param   MErr[] DOXYGEN_TO_DO
 *  @param   Err_expo DOXYGEN_TO_DO
 *  @param   MErr_expo[] DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name            |TCB Type      |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Mrb             |              |              |              |              |              |             |
 *  ----------------------------------------------------------------------------------------------------------
 *  |MO              |              |              |              |              |              |             |
 *  ----------------------------------------------------------------------------------------------------------
 *  |snrPerRb_Pilot  |              |              |              |              |              |             |
 *  ----------------------------------------------------------------------------------------------------------
 *  |snrPilot        |              |              |              |              |              |             |
 *  ----------------------------------------------------------------------------------------------------------
 *  |rxV_snrPerRB    |              |              |              |              |              |             |
 *  ----------------------------------------------------------------------------------------------------------
 *  |rxV_snrSubfr    |              |              |              |              |              |             |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void SnrSubframe(ULRXINPUTPARAMS *pRxInPar, S32 *snrPerRb, S32 *snrPilot, S32 ChanNum)
{
    S32 i;
    S16 mo, ShrMo2;
    U16 *pu16;
    S32 snrSubf[4];
    S32 *snrPerRb_Pilot[8];
    S32 L_acc, L_temp1, MO, Mrb, RbStart;
    S16 expo, temp1, temp2;
    S16 avgFlag;
    U32 chanId;
    S32 retVal;

    MO = pRxInPar->MO;
    RbStart = pRxInPar->RBStart[ChanNum];
    Mrb = pRxInPar->NumRBs[ChanNum];

   /* if (pRxInPar->physChanType[ChanNum] == PUSCH)
        avgFlag = 1;
    else
        avgFlag = 0;*/

    avgFlag = 1; //always have 2 slots even for PUCCH
    for (i = 0; i < MO; i++)
    {
        snrPerRb_Pilot[2 * i] = (S32 *) &(snrPerRb[100 * (2 * i)]);
        snrPerRb_Pilot[2 * i + 1] = (S32 *) &(snrPerRb[100 * (2 * i + 1)]);
    }

    ShrMo2 = 1;
    if (MO == 2)
        ShrMo2 = 2;
    if (MO == 4)
        ShrMo2 = 3;
    snrPerRb = (S32 *) (snrPerRb_Pilot[0]);
    // The SNR array snrPerRb_fl[] has been accumulated over MO*2 pilot symbols
    for (i = RbStart; i < (RbStart + Mrb); i++)
    {
        // snrPerRb_fl is snrPerRb_Pilot_fl[0]
        for (mo = 1; mo < 2 * MO; mo++)
        {
            snrPerRb[i] = L_add(snrPerRb[i], snrPerRb_Pilot[mo][i]);
        }
        snrPerRb[i] = L_shr(snrPerRb[i], ShrMo2);

        //Convert to dB
        expo = norm_l(snrPerRb[i]);
        L_temp1 = L_shl(snrPerRb[i], expo);
        temp1 = log_fx(L_temp1, expo+15-5); // since snrPerRb[i] is in Q10 to accomodate up to 64 dB range
        temp2 = shr(temp1, 7); // temp1 is in Q8, 1/2 dB step
        pRxInPar->SnrPerRB_dB[i] = (U8) (temp2 + 128); // index from 0 to 255

    }
    L_acc = 0;
    ShrMo2 = ShrMo2 - 1;
    for (mo = 0; mo < MO; mo++)
    {
        snrSubf[mo] = L_add(snrPilot[2 * mo], snrPilot[2 * mo + 1]) >> (avgFlag); //* 0.5;
//#ifndef CEVA_INTRINSICS
//        phy_printf("SINR %hd %ld %ld\n", mo, snrPilot[2 * mo], snrPilot[2 * mo + 1]);
//#endif
        L_acc = L_add(L_acc, snrSubf[mo]);
    }
    L_acc = L_shr(L_acc, ShrMo2);

    //Convert to dB
    expo = norm_l(L_acc);
    L_temp1 = L_shl(L_acc, expo);
    temp1 = log_fx(L_temp1, expo+15-5); // since snrPerRb[i] is in Q10 to accomodate up to 64 dB range
    temp2 = shr(temp1, 7); // temp1 is in Q8, 1/2 dB step
    pRxInPar->ul_CQI[ChanNum] = (U8) (temp2 + 128); // index from 0 to 255

    //
    // Each SNR is in linear scale and its value is a U16 number.
    // The value should be interpreted as MS 8-bit integer and LS 8-bit fractional.
    // E.g 0x02c0 means the SNR is 2.75 (2.75 is in linear scale, i.e. about 4.39 dB)
    ShrMo2 = 7;
    pu16 = pRxInPar->SnrPerRB;
    for (i = RbStart; i < (Mrb+RbStart); i++)
    {
        /*
         x32 = (U32) (snrPerRb[i]*256 + 0.5);
         if (x32 > (U32) 0x0000ffff) x32 = (U32) 0x0000ffff;
         if (x32 == 0) x32 = 1;
         pu16[i] = (U16) x32;
         */
        pu16[i] = (U16) L_shr(snrPerRb[i], ShrMo2);
    }
    pu16 = &pRxInPar->SnrSubfr[MO*ChanNum];
    for (i = 0; i < MO; i++)
    {
        /*
         x32 = (U32) (snrSubf[i]*256 + 0.5);
         if (x32 > (U32) 0x0000ffff) x32 = (U32) 0x0000ffff;
         if (x32 == 0) x32 = 1;
         pu16[i] = (U16) x32;
         */
        pu16[i] = (U16) L_shr(snrSubf[i], ShrMo2);
    }

    chanId = pRxInPar->ChanId[ChanNum];

    // Average TA
    if (pRxInPar->physChanType[chanId] == PUSCH)
    {
        pRxInPar->BsRxChMeas[chanId].TimingAdvance = LtePhyRxSduAverageUlTa(pRxInPar->BsRxChMeas[chanId].TimingAdvance,
                                                                            pRxInPar->ul_CQI[chanId],
                                                                            pRxInPar->NumRBs[chanId],
                                                                            pRxInPar->usedIndexVars[chanId]);

        //Save mErrAvg for this PUSCH-RNTI so it can be restored when it comes back
        pRxInPar->usedIndexVars[chanId]->mErrAvgCh[0] = pRxInPar->mErrAvg[2*chanId];
        pRxInPar->usedIndexVars[chanId]->mErrAvgCh[1] = pRxInPar->mErrAvg[2*chanId + 1];
        pRxInPar->usedIndexVars[chanId]->mErrExpo     = pRxInPar->mErrExpo[chanId];
    }

    // Average UL CQI
    if (!((pRxInPar->physChanType[chanId] == PUCCH) && (pRxInPar->pucchFormat[chanId] == FORMAT1)))
    {
        retVal = LtePhyRxSduAverageUlCqi(pRxInPar->ul_CQI[chanId], ((pRxInPar->physChanType[chanId] == PUSCH)?1:0),
                                         pRxInPar->usedIndexVars[chanId], pRxInPar->currTti,
                                         pRxInPar->Multi_PUCCH_PerRB[chanId]);
        retVal &= 0xFF;
        pRxInPar->ul_CQI[chanId] = (U8)retVal;
    }

}


void Compute_phase_errorProfile(S16 chEst[], S32 Err[], S16 Mpusch_sc)
{
    S32 i, I, J;
    S32 L_temp1,L_temp2 ;

    S16 temp1 = 16384; // divide by 2 to prevent overflow;

    for (i = 0; i < Mpusch_sc-1; i++)
    {
        //Err = Err + //sum(conj(chEst(mi+CE_MI1).symbol(pilotCarrierSymbol+CE_MI1).element(k0 +
        //CE_MI1)) .* chEst(mi+CE_MI1).symbol(pilotCarrierSymbol+CE_MI1).element(k1 + +CE_MI1));
        I =  i << 1;
        J = (i + 1) << 1;

        L_temp1 = L_mult(mult(chEst[I],temp1), chEst[J]);
        L_temp2 = L_mult(mult(chEst[I+1],temp1), chEst[J+1]);
        Err[I] = L_add(L_temp1, L_temp2);

        L_temp1 = L_mult(mult(chEst[I],temp1), chEst[J+1]);
        L_temp2= L_mult(mult(chEst[I+1],temp1), chEst[J]);
        Err[I+1] = L_sub(L_temp1, L_temp2);
     }

    // Err has scale of (2*sqr(scale)) or Q of (2*(14-expo_fac)+1) plus
    // sqr(max_scale) coming from normalization of dac samples
}

//#ifndef CEVA_INTRINSICS
S16 log_fx(S32 mant_32, S16 expo)
{
     S32 L_tmp, L_tmp2, log_E;
     S16 expo_work;
     S16 norm_log_nrg;

    if( mant_32 == 0L)
        return(-32768);
    else
    {
        expo_work= expo;

        log_E = fnLog10(mant_32);
        L_tmp = L_mpy_ls(log_E,20480); // 10*log10()
        expo_work = sub(31,expo_work);

        // 10*log10(2^(expo_work)) = expo_work*10*log10(2)
        L_tmp2 = L_shl(L_mult(24660,expo_work),10); //11 to 10, //expo_work*10*log10(2) in Q24, Changed by Adil from Q25 to Q24 to avoid overflow
        L_tmp = L_shl(L_tmp,2); //3 to 2
        norm_log_nrg = extract_h(L_add(L_tmp, L_tmp2));
        return(norm_log_nrg);
    }
}

S32 fnLog2(S32 L_Input)
{

    static S16 swC0 = -0x2b2a, swC1 = 0x7fc5, swC2 = -0x54d0;

    S16 siShiftCnt, swInSqrd, swIn;
    S32 LwIn;

/*_________________________________________________________________________
 |                                                                         |
 |                              Executable Code                            |
 |_________________________________________________________________________|
*/

    /* normalize input and store shifts required */
    /* ----------------------------------------- */

    siShiftCnt = norm_l(L_Input);
    LwIn = L_shl(L_Input, siShiftCnt);
    siShiftCnt = add(siShiftCnt, 1);
    siShiftCnt = negate(siShiftCnt);

    /* calculate x*x*c0 */
    /* ---------------- */

    swIn = extract_h(LwIn);
    swInSqrd = mult_r(swIn, swIn);
    LwIn = L_mult(swInSqrd, swC0);

    /* add x*c1 */
    /* --------- */

    LwIn = L_mac(LwIn, swIn, swC1);

    /* add c2 */
    /* ------ */

    LwIn = L_add(LwIn, L_deposit_h(swC2));

    /* apply *(4/32) */
    /* ------------- */
    LwIn = L_shr(LwIn, 3);
    LwIn = LwIn & 0x03ffffff;
    siShiftCnt = shl(siShiftCnt, 10);
    LwIn = L_add(LwIn, L_deposit_h(siShiftCnt));

    /* return log2 */
    /* ----------- */

    return (LwIn);
}

S32 fnLog10(S32 L_Input)
{

   static S16 Scale = 9864;         /* 0.30103 = log10(2) */
   S32 LwIn;

/*_________________________________________________________________________
 |                                                                         |
 |                              Executable Code                            |
 |_________________________________________________________________________|
*/

    /* 0.30103*log2(x) */
    /* ------------------- */

    LwIn = fnLog2(L_Input);
    LwIn = L_mpy_ls(LwIn, Scale);

    return (LwIn);
}

void SRSSnrReporting(S16 MO, S16 numSrs, S16 Mrb, S16 *StartRb,
                            S16 *EndRb, S32 *snrPerRb, U8 *rxV_snrPerRB)
{
    S32 i, n;
    S16 mo, ShrMo2;

    S32 L_acc, L_temp1;
    S16 expo, temp1, temp2;

    ShrMo2 = 32767;
    if (MO == 2)
        ShrMo2 = 16384;
    if (MO == 4)
        ShrMo2 = 8192;

    for(n = 0; n < numSrs; n++)
    {
        for (i = StartRb[n]; i < EndRb[n]; i++)
        {
            L_acc = 0;
            for (mo = 0; mo < MO; mo++)
            {
              L_acc = L_add(L_acc, snrPerRb[n*Mrb + mo * Mrb * numSrs + i]);
            }

            L_acc = L_mpy_ls(L_acc, ShrMo2);

            //Convert to dB
           expo = norm_l(L_acc);
           L_temp1 = L_shl(L_acc, expo);
           temp1 = log_fx(L_temp1, expo+15-5); // since snrPerRb[i] is in Q10 to accomodate up to 64 dB range
           temp2 = shr(temp1, 7); // temp1 is in Q8, 1/2 dB step
           rxV_snrPerRB[n*Mrb + i] = (U8) (temp2 + 128); // index from 0 to 255

           // compensate bias SNR estimation when the actual SNR is low, especially when actual SNR is less than 0 dB
           if(rxV_snrPerRB[n*Mrb + i] <= 128 - 13) // detected SNR is -6.5 dB, the actual SNR is -10 dB
               rxV_snrPerRB[n*Mrb + i] = rxV_snrPerRB[n*Mrb + i] - 7;
           else if (rxV_snrPerRB[n*Mrb + i] <= 128 - 11) // detected SNR is -5.5 dB, the actual SNR is -8 dB
               rxV_snrPerRB[n*Mrb + i] = rxV_snrPerRB[n*Mrb + i] - 5;
           else if (rxV_snrPerRB[n*Mrb + i] <= 128 - 7) // detected SNR is -3.5 dB, the actual SNR is -5 dB
               rxV_snrPerRB[n*Mrb + i] = rxV_snrPerRB[n*Mrb + i] - 3;
           else if (rxV_snrPerRB[n*Mrb + i] <= 128 - 2) // detected SNR is -1 dB, the actual SNR is -2 dB
               rxV_snrPerRB[n*Mrb + i] = rxV_snrPerRB[n*Mrb + i] - 2;
           else if (rxV_snrPerRB[n*Mrb + i] <= 128) // detected SNR is - 0.3 dB, the actual SNR is -1 dB
               rxV_snrPerRB[n*Mrb + i] = rxV_snrPerRB[n*Mrb + i] - 1;
           else
               rxV_snrPerRB[n*Mrb + i] = rxV_snrPerRB[n*Mrb + i];

            #if 0
            {
                FILE *fp;
                fp = (FILE *) fopen( "srs_data_snr_final.dat", "at+");
                     fprintf( fp, "%d\n",  rxV_snrPerRB[n*Mrb + i]);
                fclose(fp);
            }
            #endif
       }
    }
}

//#endif

//-------------------------------------------------------------------------------------------
/** @brief This function computes the correlation of received IQ samples and the corresponding cazac sequence. it returns
 *  the average across both subcarriers and symbols  for Format 1s
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *
 *  @param   reDet0            -Subcarrier IQ samples
 *  @param   reDet0M           - Modified Subcarrier IQ samples (scratch)
 *  @param   *r_alpha_uv       -Cazac Sequence
 *  @param   AvgAcrossSymbols  -Average IQ samples over subcarriers & symbols
 *  @param   Msc               -number of subcarriers
 *  @param   isym              -ith symbol
 *  @param   Nul_symb          -number of symbols in a slot
 *  @param   numSymbols        -number of non-pilot symbols in slot
 *  @param   SrsEnable         -flag indicating Srs is enabled
 *  @param   *Sdescramb        -Scrambling flag for PUCCH 1, 1A, 1B
 *  @param   pucchFormat       -Pucch Format
 *  @param   *nOc              -Orthogonal Sequence Index (DATA)
 *  @param   slot              -slot number
 *  @param   expo              -exponent
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------
 *  |Name               |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |reDet0             |               |12            |pComplex      |[-2,2(        |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |reDet0M            |               |12            |pComplex      |[-2,2(        |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |AvgAcrossSymbols   |               |1             |pComplex       |[-2,2(       |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |*r_alpha_uv        |               |12            |pComplex      |[-1,1(        |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |Msc                |               |1             |short         |12            |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |isym               |               |1             |short         |[0:6]         |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |Nul_symb           |               |1             |short         |7             |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |numSymbols         |               |1             |short         |              |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |SrsEnable          |               |1             |short         |[0,1]         |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |*Sdescramb         |               |2             |pshort        |[0,1]         |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |pucchFormat        |               |1             |short         |[0:5]         |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |slot               |               |1             |short         |[0,1]         |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |expo               |               |1             |short         |[-15:15]      |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |*nOc               |               |2             |pshort        |              |              |              |
 *  ---------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------------
 *  |Block Name          | CazacCorr                            |
 *  -------------------------------------------------------------
 *  |Input Stream        |reDet0, r_alpha_uv, AvgAcrossSymbols, |
 *  |                    |Sdescramb,nOc, reDet0M                |
 *  -------------------------------------------------------------
 *  |Output Stream       |reDet0, AvgAcrossSymbols              |
 *  -------------------------------------------------------------
 *  |Config Parameters   |Msc, isym, slot,                      |
 *  |                    |SrsEnable, Nul_symb, expo,            |
 *  |                    |numSymbols                            |
 *  -------------------------------------------------------------
 *  |Status Parameters   |None                                  |
 *  -------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function computes the correlation of received IQ samples and the corresponding cazac sequence. it returns
 *  the average across both subcarriers and symbols  for Format 1s
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Sect 5.4
 *
 *
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void CazacCorr_part1(S16 expo_fac[], S16 max_expo, S16 num_dsym, S16 InvNumSym,  S16 Msc,
               S32 *AvgAcrossSc, S16 *reDet0, S16 *r_alpha_uv, S32 *pRxInPwrAvg_PerRbDataSym, S16 *pdsym)
{
    S16 i, isym;
    S32 k, L_temp1, L_temp2, L_temp3, L_temp4, L_acc1, L_acc2, L_acc3;
    S16 temp1, temp2, tmpI1, tmpQ1, tmpI2, tmpQ2;

    S16 expo, expo1, expo2;
    S16 *r_alpha_ptr, *reDet_ptr;
    S32 isymMsc;

    for (i = 0; i < num_dsym; i++)
    {
        L_acc1 = 0;
        L_acc2 = 0;
        L_acc3 = 0;

        isym = pdsym[i];
        expo = max_expo - expo_fac[isym];

        isymMsc = isym * Msc;
        r_alpha_ptr = &r_alpha_uv[2 * isymMsc];
        reDet_ptr = &reDet0[2 * isymMsc];

        for (k = 0; k < Msc; k++)
        {
            tmpI2 = reDet_ptr[2 * k];
            tmpQ2 = reDet_ptr[2 * k + 1];

            //averaging over subcarriers (divide by 12)
            temp1 = mult(tmpI2, INV_12);
            temp2 = mult(tmpQ2, INV_12);

            //divide by 2 to prevent overflow
            tmpI1 = mult(tmpI2, 16384);
            tmpQ1 = mult(tmpQ2, 16384);

            L_acc3 = L_mac(L_acc3, temp1, tmpI1);
            L_acc3 = L_mac(L_acc3, temp2, tmpQ1);

            L_temp1 = L_mult(tmpI2, r_alpha_ptr[2 * k]);
            L_temp2 = L_mult(tmpQ2, r_alpha_ptr[2 * k + 1]);
            L_temp3 = L_mult(tmpQ2, r_alpha_ptr[2 * k]);
            L_temp4 = L_mult(tmpI2, r_alpha_ptr[2 * k + 1]);

            temp1 = round(L_add(L_shr(L_temp1,1),L_shr(L_temp2,1))); //shr to avoid saturation
            temp2 = round(L_sub(L_shr(L_temp3,1),L_shr(L_temp4,1)));

            //averaging across subcarriers
            L_acc1 = L_mac(L_acc1, temp1, INV_6); //since we have a shr above
            L_acc2 = L_mac(L_acc2, temp2, INV_6);
        }
        expo1 = norm_l(L_acc1);
        expo2 = norm_l(L_acc2);

        L_acc1 = L_shl(L_acc1, expo1);
        L_acc2 = L_shl(L_acc2, expo2);

        temp1 = extract_h(L_acc1);
        temp2 = extract_h(L_acc2);

        AvgAcrossSc[2 * i + 0]  = shr(temp1, (S16) (expo + expo1));
        AvgAcrossSc[2 * i + 1]  = shr(temp2, (S16) (expo + expo2));

        //bring all symbols to the same exponent
        L_acc3 = L_mpy_ls(L_acc3, InvNumSym);
        L_acc3 = L_shr(L_acc3,(S16)(2*expo));
        *pRxInPwrAvg_PerRbDataSym = L_add(*pRxInPwrAvg_PerRbDataSym, L_acc3);
    }
}

void CazacCorr_part2(S16 num_dsym, S32 *pAvgAcrossSc, S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI)
{
    S32 i;

    for (i = 0; i < num_dsym; i++)
    {
        pAvgReDet_pucchCQI[2 * i + 0]  = (S16)(pAvgAcrossSc[2 * i + 0]);
        pAvgReDet_pucchCQI[2 * i + 1]  = (S16)(pAvgAcrossSc[2 * i + 1]);
    }

    //there is no Ack in Format 2
    pAvgReDet_pucchACK[0] = 0;    //Re
    pAvgReDet_pucchACK[1] = 0;    //Im
}

void CazacCorr_part3(S16 num_dsym, S32 *pAvgAcrossSc, S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI)
{
    S32 i, idx_cqi;

    idx_cqi = 0;
    for (i = 0; i < num_dsym-1; i++)
    {
        if (i == num_dsym - 2)
        {
            //save IQ sample for the ACK
            pAvgReDet_pucchACK[0]  = (S16)pAvgAcrossSc[2 * idx_cqi + 0];
            pAvgReDet_pucchACK[1]  = (S16)pAvgAcrossSc[2 * idx_cqi + 1];

            //skip IQ sample for the ACK, for next CQI sample
            idx_cqi += 1;
        }

        //store CQI IQ samples in consecutive memory
        pAvgReDet_pucchCQI[2 * i + 0]  = (S16)pAvgAcrossSc[2 * idx_cqi + 0];
        pAvgReDet_pucchCQI[2 * i + 1]  = (S16)pAvgAcrossSc[2 * idx_cqi + 1];

        idx_cqi += 1;
    }
}

void CazacCorr_part4(S16 num_dsym, S16 InvNumSym, S32 *pAvgAcrossSc, S16 *pAvgReDet_pucchACK, S16 *w_ptr)
{
    S32 i;
    S32 AvgAcrossSymbols[2];
    S16 temp1, temp2;
    S32 L_temp1, L_temp2, L_temp3, L_temp4;

    AvgAcrossSymbols[0] = 0;
    AvgAcrossSymbols[1] = 0;

    for (i = 0; i < num_dsym; i++)
    {
         //Getting rid of exponent here instead of in Demapper
         //Need it here anyway in preperation for averaging across symbols
         temp1 = (S16)pAvgAcrossSc[2 * i];
         temp2 = (S16)pAvgAcrossSc[2 * i + 1];

         L_temp1 = L_mult(temp1, w_ptr[2 * i]);
         L_temp2 = L_mult(temp2, w_ptr[2 * i + 1]);
         L_temp3 = L_mult(temp2, w_ptr[2 * i]);
         L_temp4 = L_mult(temp1, w_ptr[2 * i + 1]);

         temp1 = round(L_add(L_temp1,L_temp2));
         temp2 = round(L_sub(L_temp3,L_temp4));

         //averaging across subcarriers
         AvgAcrossSymbols[0] = L_mac(AvgAcrossSymbols[0], temp1, InvNumSym); //since we have a shr above
         AvgAcrossSymbols[1] = L_mac(AvgAcrossSymbols[1], temp2, InvNumSym);
    }

    pAvgReDet_pucchACK[0] = round(AvgAcrossSymbols[0]);
    pAvgReDet_pucchACK[1] = round(AvgAcrossSymbols[1]);
}

void CazacCorr(S16 *reDet0, S16 *r_alpha_uv,  S16 Msc, S16 Nul_symb,  S16 *Sdescramb, S16 slot,
               S16 *nOc, S16 SrsEnable, S16 expo_fac[], S16 max_expo, S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI,
               S32 *pRxInPwrAvg_PerRbDataSym, S16 pucch_format)
{

#define NUM_F1_DSYM     4
#define NUM_F2_DSYM     5
#define NUM_F2AB_DSYM   6

    S16 temp1;
    S32 cond;
    S16 *w_ptr;
    S16 *pdsym;

    S16 InvNumSym = 0;
    S32 AvgAcrossSc[2*NUM_F2AB_DSYM];
    S16 num_dsym;

    //init variables
    if (pucch_format < FORMAT2)
    {
        // Format 1s
        num_dsym = NUM_F1_DSYM;
        if (SrsEnable == 1)
            num_dsym = num_dsym - 1;

        pdsym    = pucchF1_data_symbol_loc;
    }
    else if (pucch_format == FORMAT2)
    {
        // Format 2
        num_dsym = NUM_F2_DSYM;
        pdsym    = pucchF2_data_symbol_loc;
    }
    else
    {
        // Format 2a or 2b
        num_dsym = NUM_F2AB_DSYM;
        pdsym    = pucchF2ab_data_symbol_loc;
    }

    switch (num_dsym)
    {
        case 3:
            InvNumSym = 10923; // 1/3
            break;
        case 4:
            InvNumSym = 8192; // 1/4
            break;
        case 5:
            InvNumSym = 6554; // 1/5
            break;
        case 6:
            InvNumSym = 5461; // 1/6
            break;

    }


#ifdef CEVA_INTRINSICS
    CazacCorr_part1_A(expo_fac, max_expo, num_dsym, InvNumSym, Msc, AvgAcrossSc, reDet0, &r_alpha_uv[2 * slot * Nul_symb * Msc], pRxInPwrAvg_PerRbDataSym, pdsym);
#else
    CazacCorr_part1(expo_fac, max_expo, num_dsym, InvNumSym, Msc, AvgAcrossSc, reDet0, &r_alpha_uv[2 * slot * Nul_symb * Msc], pRxInPwrAvg_PerRbDataSym, pdsym);
#endif
    if (pucch_format == FORMAT2)
    {
        //for Format 2, this is it! Return 1 IQ sample per symbol of CQI
        CazacCorr_part2(num_dsym, AvgAcrossSc, pAvgReDet_pucchACK, pAvgReDet_pucchCQI);
        return;
    }

    if (pucch_format >= FORMAT2A)
    {   //for Format 2a and 2b, this is it! Return 1 IQ sample per CQI data symbol followed by 1 IQ sample for the ACK
        CazacCorr_part3(num_dsym, AvgAcrossSc, pAvgReDet_pucchACK, pAvgReDet_pucchCQI);
        return;
    }

    //else Format 1S

    //Despreader
    if (slot == 0)
    {
        cond = Sdescramb[0];
        w_ptr = Orthog_Seq_PUCCHData_fm1 + 4 * 2 * nOc[0];
    }
    else
    {
        cond = Sdescramb[1];
        w_ptr = Orthog_Seq_PUCCHData_fm1 + 4 * 2 * nOc[1];
        if (SrsEnable == 1) // Short Format 1s
            w_ptr = Orthog_Seq_PUCCH_fm1 + 3 * 2 * nOc[1];
    }

#ifdef CEVA_INTRINSICS
    CazacCorr_part4_A(num_dsym, InvNumSym, AvgAcrossSc, pAvgReDet_pucchACK, w_ptr);
#else
    CazacCorr_part4(num_dsym, InvNumSym, AvgAcrossSc, pAvgReDet_pucchACK, w_ptr);
#endif
    if (cond == 1) // multiply by exp(-j*PI/2)
    {
        temp1 = negate(pAvgReDet_pucchACK[0]);
        pAvgReDet_pucchACK[0] = pAvgReDet_pucchACK[1];
        pAvgReDet_pucchACK[1] = temp1;
    }
}

//-------------------------------------------------------------------------------------------
/** @brief This function computes the Resource assignment (starting Resource block) for PUCCH channel
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *
 *  @param   *pChanEstPar      -Pointer to structure of parameters pertaining to this channel
 *  @param   *nPRB             -Start of resource block assignment for this channel
 *  @param   ChanNum           -channel number
 *  @param   n1Pucch           -resource index for this channel
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------
 *  |Name               |TCB Type       |Size            |Data Format   |Valid Range   |Default       |Units       |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |pChanEstPar        |               |ULRXINPUTPARAMS |pshort        |              |              |            |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |*nPRB              |               |1               |pshort        |[0:99]        |              |            |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |ChanNum            |               |1               |long          |(1:15:0)      |[0:100]       |            |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |n1pucch            |               |1               |short         |[0:2047]      |              |            |
 *  ---------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------------
 *  |Block Name          |bs_rx_RBassignment_pucch              |
 *  -------------------------------------------------------------
 *  |Input Stream        |pChanEstPar                           |
 *  -------------------------------------------------------------
 *  |Output Stream       |nPRB                                  |
 *  -------------------------------------------------------------
 *  |Config Parameters   |ChanNum, n1Pucch                      |
 *  -------------------------------------------------------------
 *  |Status Parameters   |None                                  |
 *  -------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function computes the Resource assignment (starting Resource block) for PUCCH channel
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Sect 5.4
 *
 *
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_RBassignment_pucch(ULRXINPUTPARAMS *pChanEstPar, S32 ChanNum, S16 *nPRB, S16 n1Pucch)
{
    S16 m;

    S16 temp1;

    S16 c_var;
    S16 var1, var2;

    // Format 2, 2a, 2b

    m = divide_floor(pChanEstPar->n2[ChanNum],pChanEstPar->Nrb_sc);
    if (pChanEstPar->pucchFormat[ChanNum] < FORMAT2) // Format 1, 1a, 1b
    {
        c_var = 3;
        if (pChanEstPar->CPType == EXTENDED) // extended
        {
            c_var = 2;
        }
        var1 = c_var * pChanEstPar->N1_cs / pChanEstPar->delta_shift_PUCCH;
        if (n1Pucch  < var1)
            m = pChanEstPar->N2_rb;
        else
        {
            var2 = c_var * pChanEstPar->Nrb_sc / pChanEstPar->delta_shift_PUCCH;
            temp1 = n1Pucch  - var1;
            m = divide_floor(temp1, var2) + pChanEstPar->N2_rb
                + divide_ceil(pChanEstPar->N1_cs,8);
        }
    }

    //Note SlotNumber is [0:19]
    //Need to return nPRB for slot 0 only, nPRB for slot 1 can be derived from slot 0
    temp1 = m + (pChanEstPar->slotNum % 2);
    *nPRB = divide_floor(m,2); // even
    if ((temp1 & 0x1) == 1)
    {
        *nPRB = pChanEstPar->Nul_rb - 1 - *nPRB;
    }
}


S32 ComputePwrAvg_PerRbPilot(S16 *adet, S16 Mpucch_sc)
{
    S32 Pwr_PilotSym;
    S16 i;
    S16 tmpR0, tmpI0;
    S16 tmpR1, tmpI1;


    Pwr_PilotSym = 0;
    for(i = 0; i <  Mpucch_sc; i++)
    {
        //average over subcarriers (divide by 12)
        tmpR0 = mult(adet[2*i], INV_12);
        tmpI0 = mult(adet[2*i + 1], INV_12);

        //divide by 2 to prevent overflow
        tmpR1 = mult(adet[2*i], 16384);
        tmpI1 = mult(adet[2*i + 1], 16384);

        //fprintf(fmyAVG2, "CHAN EST %hd %hd %hd %hd %hd %hd %hd\n", p, adet[2 * Msc * pilotCarrierSymbol[p]+2*i],adet[2 * Msc * pilotCarrierSymbol[p]+2*i+1], aPilots [ 2*p * Mpucch_sc +2*i], aPilots [ 2*p * Mpucch_sc +2*i+1], chEst [ 2 * pilotCarrierSymbol[p] * Mpucch_sc+2*i], chEst [ 2 * pilotCarrierSymbol[p] * Mpucch_sc +2*i+1]);

        //accumulate all subcarrier signal-plus-noise powers for this slot (and antenna)
        Pwr_PilotSym = L_mac(Pwr_PilotSym, tmpR0, tmpR1);
        Pwr_PilotSym = L_mac(Pwr_PilotSym, tmpI0, tmpI1);
    }
    return(Pwr_PilotSym);
}


void ComputeChestAvg_PerRbPilot(S16 *chEst, S16 Mpucch_sc, S16 *temp_vec1)
{

      S16 i;
      S16 tmpR0, tmpI0;


        for (i=0; i < Mpucch_sc; i++)
        {
            tmpR0 = chEst[2 * i];
            tmpI0 = chEst[2 * i + 1];
            temp_vec1[0] = add(temp_vec1[0], mult(tmpR0, INV_12));
            temp_vec1[1] = add(temp_vec1[1], mult(tmpI0, INV_12));
        }
}

//#ifdef WIN32
#define BOLTZMAN -21581 //Q7 10*log10(1000*1000*K) K = 1.3806e-023
                        //First 1000 is for dBm and second 1000 is Khz
void ComputeThermalNoise(S16 numRB, S16 *Temperature, S16 numAntenna, S16 *ThermalNoisePwr)
{

    //Temperature is in Q0 (Kelvin)
    S16 i, expo;
    S32 L_temp1;
    S16 temp1, temp2;
    S16 Bw, ShrMo, avgTemp;


    ShrMo = 0; // 1 antenna
    if (numAntenna == 2)
        ShrMo = 1;
    if (numAntenna == 4)
        ShrMo = 2;

    avgTemp = shr(Temperature[0], ShrMo);
    for (i = 1; i < numAntenna; i++)
        avgTemp = add(avgTemp, shr(Temperature[i], ShrMo));


    Bw = 180*numRB;
    L_temp1 =  avgTemp * Bw; // Q0

    //Convert to dB
    expo = norm_l(L_temp1);
    L_temp1 = L_shl(L_temp1, expo);
    temp1 = log_fx(L_temp1, expo); // 10*log10(TB) Q8

    temp1 = shr(temp1,1); // Q7
    temp2  = add(temp1, BOLTZMAN);

    *ThermalNoisePwr = shr(temp2, 6); // temp2 is in Q7, 1/2 dB step

    return;
}
//#endif

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   chEst[] DOXYGEN_TO_DO
 *  @param   Err[] DOXYGEN_TO_DO
 *  @param   Mpusch_rb DOXYGEN_TO_DO
 *  @param   kre[] DOXYGEN_TO_DO
 *  @param   length_kre DOXYGEN_TO_DO
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
 *  |chEst[]       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Err[]         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Mpusch_rb     |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |kre[]         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length_kre    |              |              |              |              |              |              |
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
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void Compute_phase_errorPUCCH(S16 chEst[], S32 Err[], S16 Mpusch_rb, S16 Mpusch_sc,
                         S16 *pScaleComputeErr_Tab, S32 *ErrSub, S32 *ErrUnWt)
{
    S32 i, I, J;
    S32 L_temp1;


    S16 index, temp1, temp2;

    index = Mpusch_rb - 1;
    temp1 = inverse_nrb[index];
    temp2 = inverse_nrb_expo[index];

    Err[0] = 0; // Real
    Err[1] = 0; // Imag


    for (i = 0; i < Mpusch_sc - 1; i++)
    {
        //Err = Err + //sum(conj(chEst(mi+CE_MI1).symbol(pilotCarrierSymbol+CE_MI1).element(k0 +
        //CE_MI1)) .* chEst(mi+CE_MI1).symbol(pilotCarrierSymbol+CE_MI1).element(k1 + +CE_MI1));
        I = i << 1;
        J = (i + 1) << 1;

        L_temp1 = L_mult(mult(chEst[I], temp1), chEst[J]);
        L_temp1 = L_shr(L_temp1, temp2);
        ErrSub[I] = L_temp1;
        Err[0] = L_add(Err[0], L_temp1);

        L_temp1 = L_mult(mult(chEst[I + 1], temp1), chEst[J + 1]);
        L_temp1 = L_shr(L_temp1, temp2);
        ErrSub[I] = L_add(ErrSub[I], L_temp1);
        Err[0] = L_add(Err[0], L_temp1);


        L_temp1 = L_mult(mult(chEst[I], temp1), chEst[J + 1]);
        L_temp1 = L_shr(L_temp1, temp2);
        ErrSub[I  + 1] =  L_temp1;
        Err[1] = L_add(Err[1], L_temp1);

        L_temp1 = L_mult(mult(chEst[I + 1], temp1), chEst[J]);
        L_temp1 = L_shr(L_temp1, temp2);
        ErrSub[I + 1] = L_sub(ErrSub[I + 1], L_temp1);
        Err[1] = L_sub(Err[1], L_temp1);
    }

    ErrUnWt[0] = Err[0];
    ErrUnWt[1] = Err[1];

    //Weighting
    Err[0] = L_mpy_ls(Err[0], pScaleComputeErr_Tab[index]);
    Err[1] = L_mpy_ls(Err[1], pScaleComputeErr_Tab[index]);

    // Err has scale of (2*sqr(scale)) or Q of (2*(14-expo_fac)+1) plus
    // sqr(max_scale) coming from normalization of dac samples
}

S32 ComputePhaseDifferencePower(S32 *diffErr, S16 expo, S16 Msc)
{
    S32 i;
    S32 L_acc;
    S32 L_temp1, L_temp2;
    S16 temp1, temp2;

    L_acc = 0;
    for (i = 0; i < Msc; i++)
    {
        L_temp1 = L_shl(diffErr[2 * i], expo);
        L_temp2 = L_shl(diffErr[2 * i + 1], expo);
        temp1 = extract_h(L_temp1);
        temp2 = extract_h(L_temp2);
        L_acc = L_mac(L_acc, temp1, temp1);
        L_acc = L_mac(L_acc, temp2, temp2);
    }

    return(L_acc);
}

void rbPowerAveraging(S16 *adet, S16 Nul_sym, S16 *expo_mat, S16 max_expo, S16 Nul_rb,
                      S16 pilotCarrierSymbol, S32 step_aDet, S32 *rbPwrAvg, U8 *rbUsedFlag)
{

    S32 iRB, iSc, iSym;
    S32 rbPwrAvgTmp, L_temp1, L_temp2;
    S16 temp0, temp1, tempR0, tempI0, tempR1, tempI1, Inv_num_symb;


    if (Nul_sym == 6)
        Inv_num_symb = 5461;
    else
        Inv_num_symb = 4681;

    for (iRB = 0; iRB < Nul_rb; iRB++)
    {
        if (rbUsedFlag[iRB] == 1) // RB is used for PUSCH, shift its expo to max_expo in slot
        {
            rbPwrAvg[iRB] = L_shl(rbPwrAvg[iRB], (S16)(2*(expo_mat[pilotCarrierSymbol]-max_expo)));
            rbUsedFlag[iRB] = 0;
            continue;
        }
        else if (rbUsedFlag[iRB] == 2)  // RB is used for PUCCH, do nothing
        {
            rbUsedFlag[iRB] = 0;
            continue;
        }

        rbPwrAvg[iRB] = 0;
        for (iSym = 0; iSym < Nul_sym; iSym++)
        {
            rbPwrAvgTmp = 0;
            for (iSc = 0; iSc < 12; iSc++)
            {
                temp0 = adet[iRB * 24 + 2 * iSc + step_aDet * iSym];
                temp1 = adet[iRB * 24 + 2 * iSc + 1 + step_aDet * iSym];

                tempR0 = mult(temp0, INV_12);
                tempI0 = mult(temp1, INV_12);

                tempR1 = mult(temp0, 16384);
                tempI1 = mult(temp1, 16384);

                rbPwrAvgTmp = L_mac(rbPwrAvgTmp, tempR0, tempR1);
                rbPwrAvgTmp = L_mac(rbPwrAvgTmp, tempI0, tempI1);
            }
            L_temp1 = L_mpy_ls(rbPwrAvgTmp, Inv_num_symb);
            L_temp2 = L_shl(L_temp1, (S16)(2*(expo_mat[iSym]-max_expo)));
            rbPwrAvg[iRB] = L_add(rbPwrAvg[iRB], L_temp2);
        }
    }
}

void bs_rx_RbPwrAvg(ULRXINPUTPARAMS *pRxInPar, S16 *adet, S32 *rxInRbPwrAvg_AntSlot, U8 *rbUsedFlag, S32 step_aDet, S16 srsPilot, S16 *expo_fac)
{

    S16 expo_mat[7], Nul_symb, K, L, Nul_sym, Nul_rb, MO;
    S32 mo, slot, i, slotsym;
    S16 *adetPtr;
    S16 srsEnable, max_expo;
    S16 pilotCarrierSymbol;

    srsEnable =  (srsPilot!=-1);

    Nul_symb = pRxInPar->Nul_symb;
    L        = pRxInPar->L;
    MO       = pRxInPar->MO;
    Nul_rb   = pRxInPar->Nul_rb;
    K        = pRxInPar->K;
    pilotCarrierSymbol = pRxInPar->pilotCarrierSymbol;


    for (mo = 0; mo < MO; mo++)
    {
        adetPtr = &adet[mo * step_aDet * L];
        for (slot = 0; slot < 2; slot++)
        {
            slotsym = slot * Nul_symb;

            max_expo = -32768;
            for (i = 0; i < Nul_symb; i++)
            {
                expo_mat[i] = adetPtr[2 * K + (i + slotsym) * step_aDet];
                if (max_expo < expo_mat[i])
                    max_expo = expo_mat[i];
            }
            expo_fac[mo * 2 + slot] = max_expo;

            if ((srsEnable == 1) && (slot == 1))
            {
                Nul_sym = Nul_symb - 1;
            }
            else
            {
                Nul_sym = Nul_symb;
            }
#if 1
            // calculate unused RB power per ant and slot
            rbPowerAveraging(adetPtr+slot*Nul_symb*step_aDet,
                             Nul_sym, expo_mat, max_expo, Nul_rb, pilotCarrierSymbol, step_aDet,
                             &rxInRbPwrAvg_AntSlot[(mo*2+slot)*MAXRESOURCEBLOCKS],
                             &rbUsedFlag[(mo*2+slot)*MAXRESOURCEBLOCKS]);
#endif
        }
    }

}

#define PWR_DATA_MAX_LEFT_SHIFTS                22 // 12 bit ADC
#define UNITY_23_BITS                           ((U32)0x00400000)
#define CONVERT_TO_LOG10_DOMAIN                 3
#define RIP_Q_SHIFT_FOR_ENUM_CONVERSION         6
#define QSHIFTED_RIP_PWR_MIN_ENUM_VAL           (0X1200)
#define RIP_PWR_MAX_ENUM                        (720) // 72*2^6
#define RIP_ENUM_INCREMENT                      10    // 0.1 dB step
void RipSubframe(S32 *rxInRbPwrAvg_AntSlot, S16 *expo_fac, S32 *rxInRIP, U16 *rxInRIP_dB, S16 MO, S16 Nul_rb, S16 Nfft, S16 *initFlag)
{
    S16 mo, expo_ant = 0, max_expo, slot;
    S32 L_temp1;
    S32 ripCurSfn, i;
    S16 count, calcVal, enumCounter;
    U16 reportFormatData;
    U32 ripTime;
    S16 fftExpo;

    if (MO == 2)
        expo_ant = 1;
    else if (MO == 4)
        expo_ant = 2;

    max_expo = -32768;
    for (i = 0; i < MO * 2; i++)
    {
        if (max_expo < expo_fac[i])
        {
            max_expo = expo_fac[i];
        }
    }

    for (i = 0; i < Nul_rb; i++)
    {
        ripCurSfn = 0;
        for (mo = 0; mo < MO; mo++)
        {
            for (slot = 0; slot < NUM_SLOTS_SUBFRM; slot++)
            {
                L_temp1 = L_shl(rxInRbPwrAvg_AntSlot[i + (NUM_SLOTS_SUBFRM*mo+slot)*MAXRESOURCEBLOCKS],
                    (S16)((2*(expo_fac[NUM_SLOTS_SUBFRM*mo + slot] - max_expo)-expo_ant-1)));
                ripCurSfn = L_add(ripCurSfn, L_temp1);
            }
        }

        // ripCurSfn Q(2*(11-max_expo))
        if (*initFlag == 0)
        {
            rxInRIP[i] = L_shl(ripCurSfn, 2*max_expo);
        }
        else
        {
            // ripCurSfn * 1/100 + rxInRIP[i] * 99/100 => ripCurSfn + (rxInRIP[i] - ripCurSfn)*99/100
            ripCurSfn = L_shl(ripCurSfn, 2*max_expo);
            rxInRIP[i] = L_add(ripCurSfn, L_mpy_ls(L_sub(rxInRIP[i], ripCurSfn), 32440));
        }
    }

    if (Nfft == 512)
        fftExpo = 9;
    else if (Nfft == 1024)
        fftExpo = 10;
    else if (Nfft == 2048)
        fftExpo = 11;
    else
        fftExpo = 8;            // Dummy so it wont complain

    calcVal = 0;
    reportFormatData = 0;
    for (i = 0; i < Nul_rb; i++)
    {
        ripTime = (U32)rxInRIP[i];
        ripTime = ripTime * 12 >> fftExpo;
        //for (count = 0; (count <= (PWR_DATA_MAX_LEFT_SHIFTS)) && (((UNITY_23_BITS) >> count) > ripTime); count++);
        count = norm_l(ripTime) - 8;
        calcVal =  ((S16)( count ) << RIP_Q_SHIFT_FOR_ENUM_CONVERSION ) * -1 ;
        if (( PWR_DATA_MAX_LEFT_SHIFTS - RIP_Q_SHIFT_FOR_ENUM_CONVERSION ) < count )
        {
            calcVal += (S16)(( ripTime & ~( 0xffffffff << ( PWR_DATA_MAX_LEFT_SHIFTS - count ))) << ( count + RIP_Q_SHIFT_FOR_ENUM_CONVERSION - PWR_DATA_MAX_LEFT_SHIFTS ));
        }
        else
        {
            calcVal += (S16)(( ripTime & ~( 0xffffffff << ( PWR_DATA_MAX_LEFT_SHIFTS - count ))) >> ( PWR_DATA_MAX_LEFT_SHIFTS - count - RIP_Q_SHIFT_FOR_ENUM_CONVERSION ));
        }
        calcVal *= CONVERT_TO_LOG10_DOMAIN;
        if ( 0 <= calcVal + QSHIFTED_RIP_PWR_MIN_ENUM_VAL )
        {
            enumCounter = (S16)( calcVal + QSHIFTED_RIP_PWR_MIN_ENUM_VAL );
            if ( RIP_PWR_MAX_ENUM < ( reportFormatData = (( enumCounter * RIP_ENUM_INCREMENT ) >> RIP_Q_SHIFT_FOR_ENUM_CONVERSION ) + 1 ))
            {
                reportFormatData = RIP_PWR_MAX_ENUM;
            }
        }
        rxInRIP_dB[i] = reportFormatData;
    }

    if (*initFlag == 0)
    {
        *initFlag = 1;
    }
}

#define PWR_DATA_MAX_LEFT_SHIFTS                22 // 12 bit ADC
#define UNITY_23_BITS                           ((U32)0x00400000)
#define CONVERT_TO_LOG10_DOMAIN                 3
#define RSSI_Q_SHIFT_FOR_ENUM_CONVERSION        6
#define QSHIFTED_RSSI_PWR_MIN_ENUM_VAL           (0X2000) // 128*2^6
#define RSSI_PWR_MAX_ENUM                       (256)    // -127.5~0 by 0.5dB step
#define RSSI_ENUM_INCREMENT                     2        // 0.5 dB step
void RssiSubframe(ULRXINPUTPARAMS *pRxInPar, S32 *rssi_AntSlot, S16 *expo_mat, S16 *max_expo_AntSlot, S32 chan_num)
{
    S16 num_pusch;
    S32 mo, slot;
    S16 MO;
    S16 ant_expo;
    S32 rssi_lnr, rssi_temp, rssiInd;
    S16 max_expo;
    S16 Nfft;

    S16 count, calcVal, enumCounter;
    U16 reportFormatData;
    U32 rssiTime;
    S16 fftExpo;

    num_pusch = pRxInPar->numPUSCH;
    MO = pRxInPar->MO;
    Nfft = pRxInPar->Nfft;

    ant_expo = 0;
    if (MO == 2)
        ant_expo = 1;
    else if (MO == 4)
        ant_expo = 2;

    max_expo = -32768;
    for (mo = 0; mo < MO; mo++)
    {
        for (slot = 0; slot < 2; slot++)
        {
            if (max_expo < max_expo_AntSlot[2*mo+slot])
                max_expo = max_expo_AntSlot[2*mo+slot];
        }
    }

    rssi_lnr = 0;
    rssiInd = chan_num * MO * 2;

    if (chan_num < num_pusch)  // PUSCH
    {

        for (mo = 0; mo < MO; mo++)
        {
            for (slot = 0; slot < 2; slot++)
            {
                rssi_temp = L_shl(rssi_AntSlot[rssiInd + 2*mo+slot], (S16)2 * (expo_mat[2*mo+slot] - max_expo_AntSlot[2*mo+slot]) - 1 - ant_expo);
                rssi_lnr += L_shl(rssi_temp, (S16)2 * (max_expo_AntSlot[2*mo+slot] - max_expo)); //Q(10)
            }
        }
        rssi_lnr = L_shl(rssi_lnr, 2*max_expo);
        rssi_lnr = L_shl(rssi_lnr, 12); // Q(10)<<12 --> Q(22)
    }
    else  // PUCCH
    {
        for (mo = 0; mo < MO; mo++)
        {
            for (slot = 0; slot < 2; slot++)
            {
                rssi_temp = L_shl(rssi_AntSlot[rssiInd + 2*mo+slot], (S16)2 * (max_expo_AntSlot[2*mo+slot] - max_expo) - 1 - ant_expo);
                rssi_lnr += rssi_temp;
            }
        }
        rssi_lnr = L_shl(rssi_lnr, 2*max_expo);   // Q(22)
        rssi_lnr = rssi_lnr * 12;
    }

    if (Nfft == 512)
        fftExpo = 9;
    else if (Nfft == 1024)
        fftExpo = 10;
    else if (Nfft == 2048)
        fftExpo = 11;
    else
        fftExpo = 8;        // Dummy so it wont complain

    calcVal = 0;
    reportFormatData = 0;

    rssiTime = (U32)rssi_lnr;
    rssiTime = rssiTime >> fftExpo;
    //for (count = 0; (count <= (PWR_DATA_MAX_LEFT_SHIFTS)) && (((UNITY_23_BITS) >> count) > ripTime); count++);
    count = norm_l(rssiTime) - 8;
    calcVal =  ((S16)( count ) << RSSI_Q_SHIFT_FOR_ENUM_CONVERSION ) * -1;
    if (( PWR_DATA_MAX_LEFT_SHIFTS - RSSI_Q_SHIFT_FOR_ENUM_CONVERSION ) < count )
    {
        calcVal += (S16)(( rssiTime & ~( 0xffffffff << ( PWR_DATA_MAX_LEFT_SHIFTS - count ))) << ( count + RSSI_Q_SHIFT_FOR_ENUM_CONVERSION - PWR_DATA_MAX_LEFT_SHIFTS ));
    }
    else
    {
        calcVal += (S16)(( rssiTime & ~( 0xffffffff << ( PWR_DATA_MAX_LEFT_SHIFTS - count ))) >> ( PWR_DATA_MAX_LEFT_SHIFTS - count - RSSI_Q_SHIFT_FOR_ENUM_CONVERSION ));
    }
    calcVal *= CONVERT_TO_LOG10_DOMAIN;
    if ( 0 <= calcVal + QSHIFTED_RSSI_PWR_MIN_ENUM_VAL )
    {
        enumCounter = (S16)( calcVal + QSHIFTED_RSSI_PWR_MIN_ENUM_VAL );
        if ( RSSI_PWR_MAX_ENUM < ( reportFormatData = (( enumCounter * RSSI_ENUM_INCREMENT ) >> RSSI_Q_SHIFT_FOR_ENUM_CONVERSION ) + 1 ))
        {
            reportFormatData = RSSI_PWR_MAX_ENUM;
        }
    }
    pRxInPar->Rssi_dB[chan_num] = reportFormatData; // index from 0 to 255 -> -127.5 dBFS to 0 dBFs
}


