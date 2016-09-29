//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL gputil_fx.c
 *
 * @brief This file contains dynamic memory allocation utility functions.
 * @author Mindspeed Technologies
 * @version $Revision: 1.29 $
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
#else
#include "basic_op_cc.h"
#endif

#include "gputil_fx.h"

//-------------------------------------------------------------------------------------------
/** @brief svector: Dynamic Memory allocation for short signed array
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |svector                              |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nl, nh                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for short signed array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 *svector(long nl, long nh)
{
    short *v;
#ifdef MSEVM
    v = MemAlloc((nh-nl+1)*sizeof(short), DDR_MEM, 1);

#else
    v = MemAlloc((nh - nl + 1) * sizeof(short), CRAM_MEM, 1);
#endif
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief svectoru: Dynamic Memory allocation for short signed array without
 *  zero initialization
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |svectoruu                            |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nl, nh                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for short signed array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 *svectoru(long nl, long nh)
{
    short *v;
#ifdef MSEVM
    v = MemAlloc((nh-nl+1)*sizeof(short), DDR_MEM, 0);

#else
    v = MemAlloc((nh - nl + 1) * sizeof(short), CRAM_MEM, 0);
#endif
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief free_svector: Free Dynamic Memory allocation for short signed array
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param v  - INPUT, Pointer to memory to be freed
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |v        |         |pInput        |1        |Real          |(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |free_svector                         |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |v, nl, nh                            |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Free Dynamic Memory allocation for short signed array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void free_svector(short *v, long nl, long nh)
{
#ifdef MSEVM
    MemFree((void*)(v+nl), DDR_MEM);

#else
    MemFree((void*) (v + nl), CRAM_MEM);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief lvector: Dynamic Memory allocation for long signed array
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |lvector                              |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nl, nh                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for long signed array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 *lvector(S32 nl, S32 nh)
{
    S32 *v;
#ifdef MSEVM
    v = MemAlloc((nh-nl+1)*sizeof(S32), DDR_MEM, 1);

#else
    v = MemAlloc((nh - nl + 1) * sizeof(S32), CRAM_MEM, 1);
#endif
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief lvectoru: Dynamic Memory allocation for long signed array without
 *  zero initialization
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |lvectoru                             |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nl, nh                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for long signed array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 *lvectoru(S32 nl, S32 nh)
{
    S32 *v;
#ifdef MSEVM
    v = MemAlloc((nh-nl+1)*sizeof(S32), DDR_MEM, 0);

#else
    v = MemAlloc((nh - nl + 1) * sizeof(S32), CRAM_MEM, 0);
#endif
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief free_lvector: Free Dynamic Memory allocation for long signed array
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param v  - INPUT, Pointer to memory to be freed
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |v        |         |pInput        |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |free_lvector                         |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |v, nl, nh                            |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Free Dynamic Memory allocation for long signed array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void free_lvector(S32 *v, S32 nl, S32 nh)
{
#ifdef MSEVM
    MemFree((void*)(v+nl), DDR_MEM);

#else
    MemFree((void*) (v + nl), CRAM_MEM);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL usvector: Dynamic Memory allocation for short unsigned array
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |usvector                              |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nl, nh                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for short unsigned array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
U16 *usvector(long nl, long nh)
{
    U16 *v;
#ifdef MS_EVM
    v = MemAlloc((nh-nl+1)*sizeof(U16), DDR_MEM, 1);
#else
    v = MemAlloc((nh - nl + 1) * sizeof(U16), CRAM_MEM, 1);
#endif
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL usvectoru: Dynamic Memory allocation for short unsigned array without
 *  zero initialization
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |usvectoru                            |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nl, nh                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for short unsigned array	w/o zero initialization
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
U16 *usvectoru(long nl, long nh)
{
    U16 *v;
#ifdef MS_EVM
    v = MemAlloc((nh-nl+1)*sizeof(U16), DDR_MEM, 0);
#else
    v = MemAlloc((nh - nl + 1) * sizeof(U16), CRAM_MEM, 0);
#endif
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL free_usvector: Free Dynamic Memory allocation for short unsigned array
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param v  - INPUT, Pointer to memory to be freed
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |v        |         |pInput        |1        |Real          |(0:16:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |free_usvector                        |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |v, nl, nh                            |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Free Dynamic Memory allocation for short unsigned array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void free_usvector(U16 *v, long nl, long nh)
{
#ifdef MSEVM
    MemFree((void*)(v+nl), DDR_MEM);
#else
    MemFree((void*) (v + nl), CRAM_MEM);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief smatrix: Dynamic Memory allocation for short signed matrix
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nrl - INPUT, initial number of rows in matrix
 *  @param nrh - INPUT, number of rows in matrix to be allocated
 *  @param ncl - INPUT, initial number of columns in matrix
 *  @param nch - INPUT, number of columns in matrix to be allocated
 *
 *  @return pointer to memory allocated of size (nrh-nrl) by (nch-ncl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrh      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ncl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nch      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |smatrix                              |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nrl,nrh,ncl,nch                      |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for short signed matrix
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 **smatrix(S32 nrl, S32 nrh, S32 ncl, S32 nch)
{
    S32 i;
    S16 **m;
    S16 *tmp;
#ifdef MSEVM
    m = MemAlloc((nrh-nrl+1)*sizeof(S16*), DDR_MEM, 0);
    m -= nrl;

    for(i=nrl;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch-ncl+1)*sizeof(S16), DDR_MEM, 1);
        m[i] -= ncl;
    }

#else

    m = MemAlloc((nrh - nrl + 1) * sizeof(S16*), CRAM_MEM, 0);
    m -= nrl;
#if 0
    for(i=nrl;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch-ncl+1)*sizeof(S16), CRAM_MEM, 1);
        m[i] -= ncl;
    }
#else
    tmp = MemAlloc((nrh - nrl + 1) * (nch - ncl + 1) * sizeof(S16), CRAM_MEM, 1);
    for (i = nrl; i <= nrh; i++)
    {
        m[i] = tmp + i * (nch - ncl + 1);
        m[i] -= ncl;
    }
#endif
#endif
    return m;
}

#if 1

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   nrh DOXYGEN_TO_DO
 *  @param   nch  DOXYGEN_TO_DO
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
 *  |nrh           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nch           |              |              |              |              |              |              |
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
S16 **smatrix0(S32 nrh, S32 nch)
{
    S32 i;
    S16 **m;
    S16 *tmp;
    U32 SizeP;
    U32 SizeD;
#ifdef MSEVM
    SizeP = RUP32B((nrh+1)*sizeof(S16*));
    SizeD = (nrh+1)*(nch+1)*sizeof(S16);
    m = MemAlloc(SizeP + SizeD, DDR_MEM, 0);
    tmp = (S16*)((S8*)m + SizeP);
    memset(tmp, 0, SizeD);
    for(i=0;i<=nrh;i++)
    {
        m[i] = tmp;
        tmp += (nch+1);
    }
#else
    SizeP = RUP32B((nrh+1)*sizeof(S16*));
    SizeD = (nrh + 1) * (nch + 1) * sizeof(S16);
    m = MemAlloc(SizeP + SizeD, CRAM_MEM, 0);
    tmp = (S16*) ((S8*) m + SizeP);
    memset(tmp, 0, SizeD);
    for (i = 0; i <= nrh; i++)
    {
        m[i] = tmp;
        tmp += (nch + 1);
    }
#endif
    return m;
}
#else

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   nrh DOXYGEN_TO_DO
 *  @param   nch  DOXYGEN_TO_DO
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
 *  |nrh           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nch           |              |              |              |              |              |              |
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
S16 **smatrix0( S32 nrh, S32 nch )
{
    S32 i;
    S16 **m;
    S16 *tmp;

#ifdef MSEVM
    m = MemAlloc((nrh+1)*sizeof(S16*), DDR_MEM, 0);

    for(i=0;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch+1)*sizeof(S16), DDR_MEM, 0);
    }

#else

    m = MemAlloc(RUP32B((nrh+1)*sizeof(S16*)) + (nrh+1)*(nch+1)*sizeof(S16), CRAM_MEM, 0);
#if 0

    for(i=0;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch+1)*sizeof(S16), CRAM_MEM, 0);
    }
#else
    tmp = (S16*)((S8*)m + RUP32B((nrh+1)*sizeof(S16*)));
    memset(tmp, 0, (nrh+1)*(nch+1)*sizeof(S16));
    for(i=0;i<=nrh;i++)
    {
        m[i] = tmp + i*(nch+1);
    }
#endif

#endif
    return m;
}

#endif

//-------------------------------------------------------------------------------------------
/** @brief smatrixu: Dynamic Memory allocation for short signed matrix without zero
 *  initialization
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nrl - INPUT, initial number of rows in matrix
 *  @param nrh - INPUT, number of rows in matrix to be allocated
 *  @param ncl - INPUT, initial number of columns in matrix
 *  @param nch - INPUT, number of columns in matrix to be allocated
 *
 *  @return pointer to memory allocated of size (nrh-nrl) by (nch-ncl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrh      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ncl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nch      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |smatrixu                             |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nrl,nrh,ncl,nch                      |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for short signed matrix w/o zero initialization
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 **smatrixu(S32 nrl, S32 nrh, S32 ncl, S32 nch)
{
    S32 i;
    S16 **m;
    S16 *tmp;

#ifdef MSEVM
    m = MemAlloc((nrh-nrl+1)*sizeof(S16*), DDR_MEM, 0);
    m -= nrl;

    for(i=nrl;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch-ncl+1)*sizeof(S16), DDR_MEM, 0);
        m[i] -= ncl;
    }

#else

    m = MemAlloc((nrh - nrl + 1) * sizeof(S16*), CRAM_MEM, 0);
    m -= nrl;
#if 0

    for(i=nrl;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch-ncl+1)*sizeof(S16), CRAM_MEM, 0);
        m[i] -= ncl;
    }
#else
    tmp = MemAlloc((nrh - nrl + 1) * (nch - ncl + 1) * sizeof(S16), CRAM_MEM, 0);
    for (i = nrl; i <= nrh; i++)
    {
        m[i] = tmp + i * (nch - ncl + 1);
        m[i] -= ncl;
    }
#endif

#endif
    return m;
}

#if 1

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   nrh DOXYGEN_TO_DO
 *  @param   nch  DOXYGEN_TO_DO
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
 *  |nrh           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nch           |              |              |              |              |              |              |
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
S16 **smatrixu0(S32 nrh, S32 nch)
{
    S32 i;
    S16 **m;
    S16 *tmp;
    U32 SizeP;
    U32 SizeD;
#ifdef MSEVM
    SizeP = RUP32B((nrh+1)*sizeof(S16*));
    SizeD = (nrh+1)*(nch+1)*sizeof(S16);
    m = MemAlloc(SizeP + SizeD, DDR_MEM, 0);
    tmp = (S16*)((S8*)m + SizeP);
    for(i=0;i<=nrh;i++)
    {
        m[i] = tmp;
        tmp += (nch+1);
    }
#else
    SizeP = RUP32B((nrh+1)*sizeof(S16*));
    SizeD = (nrh + 1) * (nch + 1) * sizeof(S16);
    m = MemAlloc(SizeP + SizeD, CRAM_MEM, 0);
    tmp = (S16*) ((S8*) m + SizeP);
    for (i = 0; i <= nrh; i++)
    {
        m[i] = tmp;
        tmp += (nch + 1);
    }
#endif
    return m;
}
#else

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   nrh DOXYGEN_TO_DO
 *  @param   nch  DOXYGEN_TO_DO
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
 *  |nrh           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nch           |              |              |              |              |              |              |
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
S16 **smatrixu0( S32 nrh, S32 nch )
{
    S32 i;
    S16 **m;
    S16 *tmp;

#ifdef MSEVM
    m = MemAlloc((nrh+1)*sizeof(S16*), DDR_MEM, 0);

    for(i=0;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch+1)*sizeof(S16), DDR_MEM, 0);
    }

#else

    m = MemAlloc(RUP32B((nrh+1)*sizeof(S16*)) + (nrh+1)*(nch+1)*sizeof(S16), CRAM_MEM, 0);
#if 0

    for(i=0;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch+1)*sizeof(S16), CRAM_MEM, 0);
    }
#else
    tmp = (S16*)((S8*)m + RUP32B((nrh+1)*sizeof(S16*)));
    for(i=0;i<=nrh;i++)
    {
        m[i] = tmp + i*(nch+1);
    }
#endif

#endif
    return m;
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief free_smatrix: Free Dynamic Memory allocation for short signed matrix
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param m   - INPUT, Pointer to memory to be freed
 *  @param nrl - INPUT, initial number of rows in matrix
 *  @param nrh - INPUT, number of rows in matrix to be allocated
 *  @param ncl - INPUT, initial number of columns in matrix
 *  @param nch - INPUT, number of columns in matrix to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |m        |         |pInput        |1        |Real          |(0:16:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrh      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ncl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nch      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |free_smatrix                         |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |m,nrl,nrh,ncl,nch                    |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Free Dynamic Memory allocation for short signed matrix
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void free_smatrix(S16 **m, S32 nrl, S32 nrh, S32 ncl, S32 nch)
{
#ifdef MSEVM
    S32 i;
    for(i=nrh;i>=nrl;i--)
    {
        MemFree((void*)(m[i]+ncl), DDR_MEM);
    }
    MemFree((void*)(m+nrl), DDR_MEM);

#else
#if 0
    for(i=nrh;i>=nrl;i--)
    {
        MemFree((void*)(m[i]+ncl), CRAM_MEM);
    }
#else
    MemFree(m[0], CRAM_MEM);
#endif
    MemFree((void*) (m + nrl), CRAM_MEM);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   **m DOXYGEN_TO_DO
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
 *  |**m           |              |              |              |              |              |              |
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
void free_smatrix0(S16 **m)
{
#ifdef MSEVM
    MemFree((void*)m, DDR_MEM);
#else
    MemFree((void*) m, CRAM_MEM);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL u8vector: Dynamic Memory allocation for byte unsigned array
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |u8vector                              |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nl, nh                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for byte unsigned array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
U8 *u8vector(long nl, long nh)
{
    U8 *v;
#ifdef MSEVM
    v = MemAlloc((nh-nl+1)*sizeof(U8), DDR_MEM, 1);

#else
    v = MemAlloc((nh - nl + 1) * sizeof(U8), CRAM_MEM, 1);
#endif
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL u8vectoru: Dynamic Memory allocation for byte unsigned array without
 *  zero initialization
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |u8vectoru                            |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nl, nh                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for byte unsigned array w/o zero initialization
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
U8 *u8vectoru(long nl, long nh)
{
    U8 *v;
#ifdef MSEVM
    v = MemAlloc((nh-nl+1)*sizeof(U8), DDR_MEM, 0);

#else
    v = MemAlloc((nh - nl + 1) * sizeof(U8), CRAM_MEM, 0);
#endif
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL free_u8vector: Free Dynamic Memory allocation for byte unsigned array
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param v  - INPUT, Pointer to memory to be freed
 *  @param nl - INPUT, initial size of array to be allocated
 *  @param nh - INPUT, size of array to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |v        |         |pInput        |1        |Real          |(0:8:0)     |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nl       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nh       |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |free_u8vector                        |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |v, nl, nh                            |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Free Dynamic Memory allocation for byte unsigned array
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void free_u8vector(U8 *v, long nl, long nh)
{
#ifdef MSEVM
    MemFree((void*)(v+nl), DDR_MEM);

#else
    MemFree((void*) (v + nl), CRAM_MEM);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL u8matrix: Dynamic Memory allocation for byte unsigned matrix
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nrl - INPUT, initial number of rows in matrix
 *  @param nrh - INPUT, number of rows in matrix to be allocated
 *  @param ncl - INPUT, initial number of columns in matrix
 *  @param nch - INPUT, number of columns in matrix to be allocated
 *
 *  @return pointer to memory allocated of size (nrh-nrl) by (nch-ncl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrh      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ncl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nch      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |u8matrix                              |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nrl,nrh,ncl,nch                      |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for byte unsigned matrix
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
U8 **u8matrix(S32 nrl, S32 nrh, S32 ncl, S32 nch)
{
    S32 i;
    U8 **m, *tmp;
#ifdef MSEVM
    m = MemAlloc((nrh-nrl+1)*sizeof(U8*), DDR_MEM, 1);
    m -= nrl;

    for(i=nrl;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch-ncl+1)*sizeof(U8), DDR_MEM, 1);
        m[i] -= ncl;
    }

#else
    m = MemAlloc((nrh - nrl + 1) * sizeof(U8*), CRAM_MEM, 0);
    m -= nrl;
#if 0
    for(i=nrl;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch-ncl+1)*sizeof(U8), CRAM_MEM, 1);
        m[i] -= ncl;
    }
#else
    tmp = MemAlloc((nrh - nrl + 1) * (nch - ncl + 1) * sizeof(U8), CRAM_MEM, 1);
    for (i = nrl; i <= nrh; i++)
    {
        m[i] = tmp + i * (nch - ncl + 1);
        m[i] -= ncl;
    }
#endif
#endif
    return m;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL u8matrixu: Dynamic Memory allocation for byte unsigned matrix without
 * 	zero initialization
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param nrl - INPUT, initial number of rows in matrix
 *  @param nrh - INPUT, number of rows in matrix to be allocated
 *  @param ncl - INPUT, initial number of columns in matrix
 *  @param nch - INPUT, number of columns in matrix to be allocated
 *
 *  @return pointer to memory allocated of size (nrh-nrl) by (nch-ncl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrh      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ncl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nch      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |u8matrixu                            |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |nrl,nrh,ncl,nch                      |
 *  ------------------------------------------------------------
 *  |Status Parameters   |Return value                         |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Dynamic Memory allocation for byte unsigned matrix w/o zero initialization
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
U8 **u8matrixu(S32 nrl, S32 nrh, S32 ncl, S32 nch)
{
    S32 i;
    U8 **m, *tmp;
#ifdef MSEVM
    m = MemAlloc((nrh-nrl+1)*sizeof(U8*), DDR_MEM, 0);
    m -= nrl;

    for(i=nrl;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch-ncl+1)*sizeof(U8), DDR_MEM, 0);
        m[i] -= ncl;
    }

#else
    m = MemAlloc((nrh - nrl + 1) * sizeof(U8*), CRAM_MEM, 0);
    m -= nrl;

#if 0
    for(i=nrl;i<=nrh;i++)
    {
        m[i] = MemAlloc((nch-ncl+1)*sizeof(U8), CRAM_MEM, 0);
        m[i] -= ncl;
    }
#else

    tmp = MemAlloc((nrh - nrl + 1) * (nch - ncl + 1) * sizeof(U8), CRAM_MEM, 0);
    for (i = nrl; i <= nrh; i++)
    {
        m[i] = tmp + i * (nch - ncl + 1);
        m[i] -= ncl;
    }
#endif
#endif
    return m;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL free_u8matrix: Free Dynamic Memory allocation for byte unsigned matrix
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param m   - INPUT, Pointer to memory to be freed
 *  @param nrl - INPUT, initial number of rows in matrix
 *  @param nrh - INPUT, number of rows in matrix to be allocated
 *  @param ncl - INPUT, initial number of columns in matrix
 *  @param nch - INPUT, number of columns in matrix to be allocated
 *
 *  @return pointer to memory allocated of size (nh-nl).
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |m        |         |pInput        |1        |Real          |(0:8:0)     |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nrh      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ncl      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nch      |         |Input         |1        |Real          |(1:31:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |free_u8matrix                         |
 *  ------------------------------------------------------------
 *  |Input Streams       |none                                 |
 *  ------------------------------------------------------------
 *  |Output Streams      |none                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |m,nrl,nrh,ncl,nch                    |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Free Dynamic Memory allocation for byte unsigned matrix
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void free_u8matrix(U8 **m, S32 nrl, S32 nrh, S32 ncl, S32 nch)
{
#ifdef MSEVM
    S32 i;
    for(i=nrh;i>=nrl;i--)
    {
        MemFree((void*)(m[i]+ncl), DDR_MEM);
    }
    MemFree((void*)(m+nrl), DDR_MEM);

#else
#if 0
    for(i=nrh;i>=nrl;i--)
    {
        MemFree((void*)(m[i]+ncl), CRAM_MEM);
    }
#else
    MemFree(m[0], CRAM_MEM);
#endif
    MemFree((void*) (m + nrl), CRAM_MEM);
#endif
}

