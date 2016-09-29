//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Retarget.c
 *
 * @brief This file contains re-implementations of functions whose
 *          C library implementations rely on semihosting
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
#include "config.h"
#include "lcorecfg.h"
#include "uartdrv.h"
#include "heaps.h"
#include "4gmx.h"
#include "cline.h"
//#include "rt_sys.h"

UINT8  RvdsSysHeap [RVDS_SYSTEM_HEAP];
UINT32 RvdsSysHeapOffs = 0;
UINT32 RvdsSysHeapNotUsed = 0;

void *malloc(unsigned int size)
{
    LPVOID p = NULL;

    if (RvdsSysHeapNotUsed)
        return MxHeapAlloc (AppHeap, size);

    // here is C++ workaround to support global objects

    size = ROUND(size, 4);

    if ((RvdsSysHeapOffs + size) > RVDS_SYSTEM_HEAP)
    {
        while (p == NULL);
    }

    p = &RvdsSysHeap [RvdsSysHeapOffs];

    RvdsSysHeapOffs += size;

    return p;
}

void free(void *memblock )
{
    MxHeapFree(AppHeap, memblock);
}

//#pragma import(__use_no_semihosting)

ASMPROC void * _memcpy (void *dest, const void *src, size_t count)
{
    MOVS	r0,r0
	BXEQ	lr

	STMDB   sp!,{r0, lr}

	CMP     r2,#3
  	BLS     burst_lastbytes
  	ANDS    r12,r0,#3
  	BEQ     burst_dst_aligned
  	LDRB    r3,[r1],#1
  	CMP     r12,#2
  	ADD     r2,r2,r12
  	LDRLSB  r12,[r1],#1
  	STRB    r3,[r0],#1
  	LDRCCB  r3,[r1],#1
  	STRLSB  r12,[r0],#1
  	STRCCB  r3,[r0],#1
  	SUBS    r2,r2,#4
  	BLT     burst_lastbytes

burst_dst_aligned
  	ANDS    r3,r1,#3
  	BEQ	burst_aligned_copy

	STMDB   sp!,{r4-r11}
	SUB	r2, r2, #4
	BIC	r1, r1, #3
	LDR     lr, [r1], #4
	CMP     r3, #2
	BGT     burst_src1_40bytes
	BEQ     burst_src2_40bytes

burst_src3_40bytes
	CMP     r2, #40-4
	BLT     burst_src3_32bytes
	SUB     r2, r2, #40-4
0   MOV     r3,  lr, LSR #8
	LDMIA   r1!, {r4-r12,lr}
    ORR     r3,  r3, r4, LSL #24
    MOV     r4,  r4, LSR #8
    ORR     r4,  r4, r5, LSL #24
    MOV     r5,  r5, LSR #8
    ORR     r5,  r5, r6, LSL #24
    MOV     r6,  r6, LSR #8
    ORR     r6,  r6, r7, LSL #24
    MOV     r7,  r7, LSR #8
    ORR     r7,  r7, r8, LSL #24
    MOV     r8,  r8, LSR #8
    ORR     r8,  r8, r9, LSL #24
    MOV     r9,  r9, LSR #8
    ORR     r9,  r9, r10, LSL #24
    MOV     r10, r10, LSR #8
    ORR     r10, r10, r11, LSL #24
    MOV     r11, r11, LSR #8
    ORR     r11, r11, r12, LSL #24
    MOV     r12, r12, LSR #8
    ORR     r12, r12, lr, LSL #24
    STMIA   r0!, {r3,r4-r12}
    SUBS    r2, r2, #40
    BGE     %B0
    ADDS    r2, r2, #40-4
    BLT     %F2

burst_src3_32bytes
        CMP     r2, #32-4
        BLT     burst_src3_20bytes
        SUB     r2, r2, #32-4
0       MOV     r3,  lr, LSR #8
        LDMIA   r1!, {r6-r12,lr}
        ORR     r3,  r3, r6, LSL #24
        MOV     r6,  r6, LSR #8
        ORR     r6,  r6, r7, LSL #24
        MOV     r7,  r7, LSR #8
        ORR     r7,  r7, r8, LSL #24
        MOV     r8,  r8, LSR #8
        ORR     r8,  r8, r9, LSL #24
        MOV     r9,  r9, LSR #8
        ORR     r9,  r9, r10, LSL #24
        MOV     r10, r10, LSR #8
        ORR     r10, r10, r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12, LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
        STMIA   r0!, {r3,r6-r12}
        SUBS    r2, r2, #32
        BGE     %B0
        ADDS    r2, r2, #32-4
        BLT     %F2

burst_src3_20bytes
        CMP     r2, #20-4
        BLT     burst_src3_16bytes
        SUB     r2, r2, #20-4
0       MOV     r3,  lr, LSR #8
        LDMIA   r1!, {r9-r12,lr}
        ORR     r3,  r3, r9, LSL #24
        MOV     r9,  r9, LSR #8
        ORR     r9,  r9, r10, LSL #24
        MOV     r10, r10, LSR #8
        ORR     r10, r10, r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12, LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
        STMIA   r0!, {r3,r9-r12}
        SUBS    r2, r2, #20
        BGE     %B0
        ADDS    r2, r2, #20-4
        BLT     %F2

burst_src3_16bytes
        CMP     r2, #16-4
        BLT     burst_src3_12bytes
        SUB     r2, r2, #16-4
0       MOV     r3,  lr, LSR #8
        LDMIA   r1!, {r10-r12,lr}
        ORR     r3,  r3,  r10, LSL #24
        MOV     r10, r10, LSR #8
        ORR     r10, r10, r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12,LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
        STMIA   r0!, {r3,r10-r12}
        SUBS    r2, r2, #16
        BGE     %B0
        ADDS    r2, r2, #16-4
        BLT     %F2

burst_src3_12bytes
        CMP     r2, #12-4
        BLT     %F1
        SUB     r2, r2, #12-4
0       MOV     r3,  lr, LSR #8
        LDMIA   r1!, {r11-r12,lr}
        ORR     r3,  r3,  r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12,LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
        STMIA   r0!, {r3,r11-r12}
        SUBS    r2, r2, #12
        BGE     %B0
        ADDS    r2, r2, #12-4
        BLT     %F2
1       TST	r2,r2
	BLT	%F2
	MOV     r12, lr,LSR #8
        LDR     lr, [r1], #4
        ORR     r12, r12, lr, LSL #24
        STR     r12, [r0], #4
        SUBS    r2, r2, #4
        BGE     %B1
2       SUB     r1, r1, #3
	LDMIA   sp!,{r4-r11}
	ADDS	r2, r2, #4
        B       burst_lastbytes

burst_src2_40bytes
        CMP     r2, #40-4
        BLT     burst_src2_32bytes
        SUB     r2, r2, #40-4
0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r4-r12,lr}
        ORR     r3,  r3,  r4, LSL #16
        MOV     r4,  r4,  LSR #16
        ORR     r4,  r4,  r5, LSL #16
        MOV     r5,  r5,  LSR #16
        ORR     r5,  r5,  r6, LSL #16
        MOV     r6,  r6,  LSR #16
        ORR     r6,  r6,  r7, LSL #16
        MOV     r7,  r7,  LSR #16
        ORR     r7,  r7,  r8, LSL #16
        MOV     r8,  r8,  LSR #16
        ORR     r8,  r8,  r9, LSL #16
        MOV     r9,  r9,  LSR #16
        ORR     r9,  r9,  r10, LSL #16
        MOV     r10, r10, LSR #16
        ORR     r10, r10, r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
        STMIA   r0!, {r3,r4-r12}
        SUBS    r2, r2, #40
        BGE     %B0
        ADDS    r2, r2, #40-4
        BLT     %F2

burst_src2_32bytes
        CMP     r2, #32-4
        BLT     burst_src2_20bytes
        SUB     r2, r2, #32-4
0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r6-r12,lr}
        ORR     r3,  r3,  r6, LSL #16
        MOV     r6,  r6,  LSR #16
        ORR     r6,  r6,  r7, LSL #16
        MOV     r7,  r7,  LSR #16
        ORR     r7,  r7,  r8, LSL #16
        MOV     r8,  r8,  LSR #16
        ORR     r8,  r8,  r9, LSL #16
        MOV     r9,  r9,  LSR #16
        ORR     r9,  r9,  r10, LSL #16
        MOV     r10, r10, LSR #16
        ORR     r10, r10, r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
        STMIA   r0!, {r3,r6-r12}
        SUBS    r2, r2, #32
        BGE     %B0
        ADDS    r2, r2, #32-4
        BLT     %F2

burst_src2_20bytes
        CMP     r2, #20-4
        BLT     burst_src2_16bytes
        SUB     r2, r2, #20-4
0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r9-r12,lr}
        ORR     r3,  r3,  r9, LSL #16
        MOV     r9,  r9,  LSR #16
        ORR     r9,  r9,  r10, LSL #16
        MOV     r10, r10, LSR #16
        ORR     r10, r10, r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
        STMIA   r0!, {r3,r9-r12}
        SUBS    r2, r2, #20
        BGE     %B0
        ADDS    r2, r2, #20-4
        BLT     %F2

burst_src2_16bytes
        CMP     r2, #16-4
        BLT     burst_src2_12bytes
        SUB     r2, r2, #16-4
0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r10-r12,lr}
        ORR     r3,  r3,  r10, LSL #16
        MOV     r10, r10, LSR #16
        ORR     r10, r10, r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
        STMIA   r0!, {r3,r10-r12}
        SUBS    r2, r2, #16
        BGE     %B0
        ADDS    r2, r2, #16-4
        BLT     %F2

burst_src2_12bytes
        CMP     r2, #12-4
        BLT     %F1
        SUB     r2, r2, #12-4
0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r11-r12,lr}
        ORR     r3,  r3,  r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
        STMIA   r0!, {r3,r11-r12}
        SUBS    r2, r2, #12
        BGE     %B0
        ADDS    r2, r2, #12-4
        BLT     %F2
1       TST	r2,r2
	BLT	%F2
        MOV     r12, lr, LSR #16
        LDR     lr, [r1], #4
        ORR     r12, r12, lr, LSL #16
        STR     r12, [r0], #4
        SUBS    r2, r2, #4
        BGE     %B1
2       SUB     r1, r1, #2
	LDMIA   sp!,{r4-r11}
	ADDS	r2, r2, #4
        B       burst_lastbytes

burst_src1_40bytes
        CMP     r2, #40-4
        BLT     burst_src1_32bytes
        SUB     r2, r2, #40-4
0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r4-r12,lr}
        ORR     r3,  r3,  r4, LSL #8
        MOV     r4,  r4,  LSR #24
        ORR     r4,  r4,  r5, LSL #8
        MOV     r5,  r5,  LSR #24
        ORR     r5,  r5,  r6, LSL #8
        MOV     r6,  r6,  LSR #24
        ORR     r6,  r6,  r7, LSL #8
        MOV     r7,  r7,  LSR #24
        ORR     r7,  r7,  r8, LSL #8
        MOV     r8,  r8,  LSR #24
        ORR     r8,  r8,  r9, LSL #8
        MOV     r9,  r9,  LSR #24
        ORR     r9,  r9,  r10, LSL #8
        MOV     r10, r10, LSR #24
        ORR     r10, r10, r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
        STMIA   r0!, {r3,r4-r12}
        SUBS    r2, r2, #40
        BGE     %B0
        ADDS    r2, r2, #40-4
        BLT     %F2

burst_src1_32bytes
        CMP     r2, #32-4
        BLT     burst_src1_20bytes
        SUB     r2, r2, #32-4
0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r6-r12,lr}
        ORR     r3,  r3,  r6, LSL #8
        MOV     r6,  r6,  LSR #24
        ORR     r6,  r6,  r7, LSL #8
        MOV     r7,  r7,  LSR #24
        ORR     r7,  r7,  r8, LSL #8
        MOV     r8,  r8,  LSR #24
        ORR     r8,  r8,  r9, LSL #8
        MOV     r9,  r9,  LSR #24
        ORR     r9,  r9,  r10, LSL #8
        MOV     r10, r10, LSR #24
        ORR     r10, r10, r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
        STMIA   r0!, {r3,r6-r12}
        SUBS    r2, r2, #32
        BGE     %B0
        ADDS    r2, r2, #32-4
        BLT     %F2

burst_src1_20bytes
        CMP     r2, #20-4
        BLT     burst_src1_16bytes
        SUB     r2, r2, #20-4

0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r9-r12,lr}
        ORR     r3,  r3,  r9, LSL #8
        MOV     r9,  r9,  LSR #24
        ORR     r9,  r9,  r10, LSL #8
        MOV     r10, r10, LSR #24
        ORR     r10, r10, r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
        STMIA   r0!, {r3,r9-r12}
        SUBS    r2, r2, #20
        BGE     %B0
        ADDS    r2, r2, #20-4
        BLT     %F2

burst_src1_16bytes
        CMP     r2, #16-4
        BLT     burst_src1_12bytes
        SUB     r2, r2, #16-4
0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r10-r12,lr}
        ORR     r3,  r3,  r10, LSL #8
        MOV     r10, r10, LSR #24
        ORR     r10, r10, r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
        STMIA   r0!, {r3,r10-r12}
        SUBS    r2, r2, #16
        BGE     %B0
        ADDS    r2, r2, #16-4
        BLT     %F2

burst_src1_12bytes
        CMP     r2, #12-4
        BLT     %F1
        SUB     r2, r2, #12-4
0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r11-r12,lr}
        ORR     r3,  r3,  r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
        STMIA   r0!, {r3,r11-r12}
        SUBS    r2, r2, #12
        BGE     %B0
        ADDS    r2, r2, #12-4
        BLT     %F2
1       TST	r2,r2
	BLT	%F2
        MOV     r12, lr, LSR #24
        LDR     lr, [r1], #4
        ORR     r12, r12, lr, LSL #8
        STR     r12, [r0], #4
        SUBS    r2, r2, #4
        BGE     %B1
2       SUB     r1, r1, #1
	LDMIA   sp!,{r4-r11}
	ADDS	r2, r2, #4
        B       burst_lastbytes

burst_aligned_copy
	STMDB	 sp!,{r4}

burst_aligned_128bytes
	CMP	r2,#0x80
	BLT	burst_aligned_64bytes
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	SUBS    r2,r2,#0x80
	BGE     burst_aligned_128bytes

burst_aligned_64bytes
	CMP	r2,#0x40
	BLT	burst_aligned_32bytes
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	LDMIA   r1!, {r3,r4,r12,lr}
	STMIA   r0!, {r3,r4,r12,lr}
	SUBS    r2,r2,#0x40
	BGE     burst_aligned_64bytes

burst_aligned_32bytes
	SUBS    r2,r2,#0x20
	BCC     burst_aligned_16bytes
0	LDMCSIA r1!, {r3,r4,r12,lr}
	STMCSIA r0!, {r3,r4,r12,lr}
	LDMCSIA r1!, {r3,r4,r12,lr}
	STMCSIA r0!, {r3,r4,r12,lr}
	SUBCSS  r2,r2,#0x20
	BCS     %B0

burst_aligned_16bytes
	MOVS    r12, r2, LSL #28
	LDMCSIA r1!, {r3,r4,r12,lr}
	STMCSIA	r0!, {r3,r4,r12,lr}
	LDMMIIA r1!, {r3,r4}
	STMMIIA r0!, {r3,r4}
	LDMIA	sp!, {r4}
	MOVS    r12, r2, LSL #30
	LDRCS   r3, [r1], #4
	STRCS   r3, [r0], #4

burst_lastbytes
	MOVS    r2, r2, LSL #31
	LDRMIB  r2, [r1], #1
	LDRCSB  r3, [r1], #1
	LDRCSB  r12,[r1], #1
	STRMIB  r2, [r0], #1
	STRCSB  r3, [r0], #1
	STRCSB  r12,[r0], #1
	LDMIA   sp!, {r0, pc}
}


ASMPROC void * memcpy (void *dest, const void *src, size_t count)
{
    MOVS	r0,r0
	BXEQ	lr
	STMDB   sp!,{r0, lr}
	CMP     r2,#3
  	BLS     nonburst_lastbytes
  	ANDS    r12,r0,#3
  	BEQ     nonburst_dst_aligned
  	LDRB    r3,[r1],#1
  	CMP     r12,#2
  	ADD     r2,r2,r12
  	LDRLSB  r12,[r1],#1
  	STRB    r3,[r0],#1
  	LDRCCB  r3,[r1],#1
  	STRLSB  r12,[r0],#1
  	STRCCB  r3,[r0],#1
  	SUBS    r2,r2,#4
	BLT	nonburst_lastbytes

nonburst_dst_aligned
  	ANDS    r3,r1,#3
  	BEQ	nonburst_aligned_copy
	STMDB   sp!,{r4-r11}
	SUB	r2, r2, #4
	BIC	r1, r1, #3
	LDR     lr, [r1], #4
	CMP     r3, #2
	BGT     nonburst_src1_40bytes
	BEQ     nonburst_src2_40bytes

nonburst_src3_40bytes
	CMP     r2, #40-4
	BLT     nonburst_src3_32bytes
	SUB     r2, r2, #40-4
0   	MOV     r3,  lr, LSR #8
	LDMIA   r1!, {r4-r12,lr}
        ORR     r3,  r3, r4, LSL #24
        MOV     r4,  r4, LSR #8
        ORR     r4,  r4, r5, LSL #24
        MOV     r5,  r5, LSR #8
        ORR     r5,  r5, r6, LSL #24
        MOV     r6,  r6, LSR #8
        ORR     r6,  r6, r7, LSL #24
        MOV     r7,  r7, LSR #8
        ORR     r7,  r7, r8, LSL #24
        MOV     r8,  r8, LSR #8
        ORR     r8,  r8, r9, LSL #24
        MOV     r9,  r9, LSR #8
        ORR     r9,  r9, r10, LSL #24
        MOV     r10, r10, LSR #8
        ORR     r10, r10, r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12, LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
	STR	r3, [r0], #4
	STR	r4, [r0], #4
	STR	r5, [r0], #4
	STR	r6, [r0], #4
	STR	r7, [r0], #4
	STR	r8, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4
        SUBS    r2, r2, #40
        BGE     %B0
        ADDS    r2, r2, #40-4
        BLT     %F2

nonburst_src3_32bytes
        CMP     r2, #32-4
        BLT     nonburst_src3_20bytes
        SUB     r2, r2, #32-4
0       MOV     r3,  lr, LSR #8
        LDMIA   r1!, {r6-r12,lr}
        ORR     r3,  r3, r6, LSL #24
        MOV     r6,  r6, LSR #8
        ORR     r6,  r6, r7, LSL #24
        MOV     r7,  r7, LSR #8
        ORR     r7,  r7, r8, LSL #24
        MOV     r8,  r8, LSR #8
        ORR     r8,  r8, r9, LSL #24
        MOV     r9,  r9, LSR #8
        ORR     r9,  r9, r10, LSL #24
        MOV     r10, r10, LSR #8
        ORR     r10, r10, r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12, LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
	STR	r3, [r0], #4
	STR	r6, [r0], #4
	STR	r7, [r0], #4
	STR	r8, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4
        SUBS    r2, r2, #32
        BGE     %B0
        ADDS    r2, r2, #32-4
        BLT     %F2

nonburst_src3_20bytes
        CMP     r2, #20-4
        BLT     nonburst_src3_16bytes
        SUB     r2, r2, #20-4

0       MOV     r3,  lr, LSR #8
        LDMIA   r1!, {r9-r12,lr}
        ORR     r3,  r3, r9, LSL #24
        MOV     r9,  r9, LSR #8
        ORR     r9,  r9, r10, LSL #24
        MOV     r10, r10, LSR #8
        ORR     r10, r10, r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12, LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
	STR	r3, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4
        SUBS    r2, r2, #20
        BGE     %B0
        ADDS    r2, r2, #20-4
        BLT     %F2

nonburst_src3_16bytes
        CMP     r2, #16-4
        BLT     nonburst_src3_12bytes
        SUB     r2, r2, #16-4
0       MOV     r3,  lr, LSR #8
        LDMIA   r1!, {r10-r12,lr}
        ORR     r3,  r3,  r10, LSL #24
        MOV     r10, r10, LSR #8
        ORR     r10, r10, r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12,LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
	STR	r3, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4
        SUBS    r2, r2, #16
        BGE     %B0
        ADDS    r2, r2, #16-4
        BLT     %F2

nonburst_src3_12bytes
        CMP     r2, #12-4
        BLT     %F1
        SUB     r2, r2, #12-4

0       MOV     r3,  lr, LSR #8
        LDMIA   r1!, {r11-r12,lr}
        ORR     r3,  r3,  r11, LSL #24
        MOV     r11, r11, LSR #8
        ORR     r11, r11, r12,LSL #24
        MOV     r12, r12, LSR #8
        ORR     r12, r12, lr, LSL #24
	STR	r3, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4
        SUBS    r2, r2, #12
        BGE     %B0
        ADDS    r2, r2, #12-4
        BLT     %F2

1       TST	r2,r2
	BLT	%F2
        MOV     r12, lr,LSR #8
        LDR     lr, [r1], #4
        ORR     r12, r12, lr, LSL #24
        STR     r12, [r0], #4
        SUBS    r2, r2, #4
        BGE     %B1
2       SUB     r1, r1, #3
	LDMIA   sp!,{r4-r11}
	ADDS	r2, r2, #4
        B       nonburst_lastbytes

nonburst_src2_40bytes
        CMP     r2, #40-4
        BLT     nonburst_src2_32bytes
        SUB     r2, r2, #40-4

0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r4-r12,lr}
        ORR     r3,  r3,  r4, LSL #16
        MOV     r4,  r4,  LSR #16
        ORR     r4,  r4,  r5, LSL #16
        MOV     r5,  r5,  LSR #16
        ORR     r5,  r5,  r6, LSL #16
        MOV     r6,  r6,  LSR #16
        ORR     r6,  r6,  r7, LSL #16
        MOV     r7,  r7,  LSR #16
        ORR     r7,  r7,  r8, LSL #16
        MOV     r8,  r8,  LSR #16
        ORR     r8,  r8,  r9, LSL #16
        MOV     r9,  r9,  LSR #16
        ORR     r9,  r9,  r10, LSL #16
        MOV     r10, r10, LSR #16
        ORR     r10, r10, r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
	STR	r3, [r0], #4
	STR	r4, [r0], #4
	STR	r5, [r0], #4
	STR	r6, [r0], #4
	STR	r7, [r0], #4
	STR	r8, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #40
        BGE     %B0
        ADDS    r2, r2, #40-4
        BLT     %F2

nonburst_src2_32bytes
        CMP     r2, #32-4
        BLT     nonburst_src2_20bytes
        SUB     r2, r2, #32-4

0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r6-r12,lr}
        ORR     r3,  r3,  r6, LSL #16
        MOV     r6,  r6,  LSR #16
        ORR     r6,  r6,  r7, LSL #16
        MOV     r7,  r7,  LSR #16
        ORR     r7,  r7,  r8, LSL #16
        MOV     r8,  r8,  LSR #16
        ORR     r8,  r8,  r9, LSL #16
        MOV     r9,  r9,  LSR #16
        ORR     r9,  r9,  r10, LSL #16
        MOV     r10, r10, LSR #16
        ORR     r10, r10, r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
	STR	r3, [r0], #4
	STR	r6, [r0], #4
	STR	r7, [r0], #4
	STR	r8, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #32
        BGE     %B0
        ADDS    r2, r2, #32-4
        BLT     %F2

nonburst_src2_20bytes
        CMP     r2, #20-4
        BLT     nonburst_src2_16bytes
        SUB     r2, r2, #20-4

0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r9-r12,lr}
        ORR     r3,  r3,  r9, LSL #16
        MOV     r9,  r9,  LSR #16
        ORR     r9,  r9,  r10, LSL #16
        MOV     r10, r10, LSR #16
        ORR     r10, r10, r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
	STR	r3, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #20
        BGE     %B0
        ADDS    r2, r2, #20-4
        BLT     %F2

nonburst_src2_16bytes
        CMP     r2, #16-4
        BLT     nonburst_src2_12bytes
        SUB     r2, r2, #16-4

0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r10-r12,lr}
        ORR     r3,  r3,  r10, LSL #16
        MOV     r10, r10, LSR #16
        ORR     r10, r10, r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
	STR	r3, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #16
        BGE     %B0
        ADDS    r2, r2, #16-4
        BLT     %F2

nonburst_src2_12bytes
        CMP     r2, #12-4
        BLT     %F1
        SUB     r2, r2, #12-4

0       MOV     r3,  lr, LSR #16
        LDMIA   r1!, {r11-r12,lr}
        ORR     r3,  r3,  r11, LSL #16
        MOV     r11, r11, LSR #16
        ORR     r11, r11, r12,LSL #16
        MOV     r12, r12, LSR #16
        ORR     r12, r12, lr, LSL #16
	STR	r3, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #12
        BGE     %B0
        ADDS    r2, r2, #12-4
        BLT     %F2

1       TST	r2,r2
	BLT	%F2
        MOV     r12, lr, LSR #16
        LDR     lr, [r1], #4
        ORR     r12, r12, lr, LSL #16
        STR     r12, [r0], #4
        SUBS    r2, r2, #4
        BGE     %B1
2       SUB     r1, r1, #2
	LDMIA   sp!,{r4-r11}
	ADDS	r2, r2, #4
        B       nonburst_lastbytes

nonburst_src1_40bytes
        CMP     r2, #40-4
        BLT     nonburst_src1_32bytes
        SUB     r2, r2, #40-4

0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r4-r12,lr}
        ORR     r3,  r3,  r4, LSL #8
        MOV     r4,  r4,  LSR #24
        ORR     r4,  r4,  r5, LSL #8
        MOV     r5,  r5,  LSR #24
        ORR     r5,  r5,  r6, LSL #8
        MOV     r6,  r6,  LSR #24
        ORR     r6,  r6,  r7, LSL #8
        MOV     r7,  r7,  LSR #24
        ORR     r7,  r7,  r8, LSL #8
        MOV     r8,  r8,  LSR #24
        ORR     r8,  r8,  r9, LSL #8
        MOV     r9,  r9,  LSR #24
        ORR     r9,  r9,  r10, LSL #8
        MOV     r10, r10, LSR #24
        ORR     r10, r10, r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
	STR	r3, [r0], #4
	STR	r4, [r0], #4
	STR	r5, [r0], #4
	STR	r6, [r0], #4
	STR	r7, [r0], #4
	STR	r8, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #40
        BGE     %B0
        ADDS    r2, r2, #40-4
        BLT     %F2

nonburst_src1_32bytes
        CMP     r2, #32-4
        BLT     nonburst_src1_20bytes
        SUB     r2, r2, #32-4

0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r6-r12,lr}
        ORR     r3,  r3,  r6, LSL #8
        MOV     r6,  r6,  LSR #24
        ORR     r6,  r6,  r7, LSL #8
        MOV     r7,  r7,  LSR #24
        ORR     r7,  r7,  r8, LSL #8
        MOV     r8,  r8,  LSR #24
        ORR     r8,  r8,  r9, LSL #8
        MOV     r9,  r9,  LSR #24
        ORR     r9,  r9,  r10, LSL #8
        MOV     r10, r10, LSR #24
        ORR     r10, r10, r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
	STR	r3, [r0], #4
	STR	r6, [r0], #4
	STR	r7, [r0], #4
	STR	r8, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #32
        BGE     %B0
        ADDS    r2, r2, #32-4
        BLT     %F2

nonburst_src1_20bytes
        CMP     r2, #20-4
        BLT     nonburst_src1_16bytes
        SUB     r2, r2, #20-4

0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r9-r12,lr}
        ORR     r3,  r3,  r9, LSL #8
        MOV     r9,  r9,  LSR #24
        ORR     r9,  r9,  r10, LSL #8
        MOV     r10, r10, LSR #24
        ORR     r10, r10, r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
	STR	r3, [r0], #4
	STR	r9, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #20
        BGE     %B0
        ADDS    r2, r2, #20-4
        BLT     %F2

nonburst_src1_16bytes
        CMP     r2, #16-4
        BLT     nonburst_src1_12bytes
        SUB     r2, r2, #16-4

0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r10-r12,lr}
        ORR     r3,  r3,  r10, LSL #8
        MOV     r10, r10, LSR #24
        ORR     r10, r10, r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
	STR	r3, [r0], #4
	STR	r10, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #16
        BGE     %B0
        ADDS    r2, r2, #16-4
        BLT     %F2

nonburst_src1_12bytes
        CMP     r2, #12-4
        BLT     %F1
        SUB     r2, r2, #12-4

0       MOV     r3,  lr, LSR #24
        LDMIA   r1!, {r11-r12,lr}
        ORR     r3,  r3,  r11, LSL #8
        MOV     r11, r11, LSR #24
        ORR     r11, r11, r12,LSL #8
        MOV     r12, r12, LSR #24
        ORR     r12, r12, lr, LSL #8
	STR	r3, [r0], #4
	STR	r11, [r0], #4
	STR	r12, [r0], #4

        SUBS    r2, r2, #12
        BGE     %B0
        ADDS    r2, r2, #12-4
        BLT     %F2

1       TST	r2,r2
	BLT	%F2
        MOV     r12, lr, LSR #24
        LDR     lr, [r1], #4
        ORR     r12, r12, lr, LSL #8
        STR     r12, [r0], #4
        SUBS    r2, r2, #4
        BGE     %B1
2       SUB     r1, r1, #1
	LDMIA   sp!,{r4-r11}
	ADDS	r2, r2, #4
        B       nonburst_lastbytes

nonburst_aligned_copy
	STMDB	 sp!,{r4}

nonburst_aligned_32bytes
	SUBS    r2,r2,#0x20
	BCC     nonburst_aligned_16bytes
0	LDMCSIA r1!, {r3,r4,r12,lr}
	STRCS	r3, [r0], #4
	STRCS	r4, [r0], #4
	STRCS	r12, [r0], #4
	STRCS	r14, [r0], #4
	LDMCSIA r1!, {r3,r4,r12,lr}
	STRCS	r3, [r0], #4
	STRCS	r4, [r0], #4
	STRCS	r12, [r0], #4
	STRCS	r14, [r0], #4
	SUBCSS  r2,r2,#0x20
	BCS     %B0

nonburst_aligned_16bytes
	MOVS    r12, r2, LSL #28
	LDMCSIA r1!, {r3,r4,r12,lr}
	STRCS	r3, [r0], #4
	STRCS	r4, [r0], #4
	STRCS	r12, [r0], #4
	STRCS	r14, [r0], #4
	LDMMIIA r1!, {r3,r4}
	STRMI	r3, [r0], #4
	STRMI	r4, [r0], #4
	LDMIA	sp!, {r4}
	MOVS    r12, r2, LSL #30
	LDRCS   r3, [r1], #4
	STRCS   r3, [r0], #4

nonburst_lastbytes
	MOVS    r2, r2, LSL #31
	LDRMIB  r2, [r1], #1
	LDRCSB  r3, [r1], #1
	LDRCSB  r12,[r1], #1
	STRMIB  r2, [r0], #1
	STRCSB  r3, [r0], #1
	STRCSB  r12,[r0], #1
	LDMIA   sp!, {r0, pc}
}

/*
** Retargeted I/O
** ==============
** The following C library functions make use of semihosting
** to read or write characters to the debugger console: fputc(),
** fgetc(), and _ttywrch().  They must be retargeted to write to
** the model's UART.  __backspace() must also be retargeted
** with this layer to enable scanf().  See the Compiler and
** Libraries Guide.
*/


/*
** These must be defined to avoid linking in stdio.o from the
** C Library
*/

#include <string.h>
#include <rt_misc.h>
#include <rt_sys.h>

#pragma import(__use_no_semihosting_swi)

//FILE __stdin, __stdout, __stderr;
//FILE *__aeabi_stdin, *__aeabi_stdout, *__aeabi_stderr;
/*
** __backspace must return the last char read to the stream
** fgetc() needs to keep a record of whether __backspace was
** called directly before it
*/
int last_char_read;
int backspace_called;

extern const char* UARTName[];

int fgetc(FILE *f)
{
    HANDLE hDrv;
    unsigned char tempch;
    unsigned nSize = sizeof(char);
    UINT32 cmgrUART = ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART;
    if (cmgrUART == CMGR_TURNED_OFF)
    	return 0;

    /* if we just backspaced, then return the backspaced character */
    /* otherwise output the next character in the stream */
    if (backspace_called)
    {
      backspace_called = 0;
      return last_char_read;
    }

    if (FAILED(MxGetObjByName(CUR_CPU, UARTName[cmgrUART], &hDrv)))
        return 0;

    MxReadDrvData(hDrv, UART_CMD_GET_CHAR, &tempch, &nSize);

    last_char_read = (int)tempch;       /* backspace must return this value */
    return tempch;
}



int fputc(int ch, FILE *f)
{
    UINT32 cmgrUART = ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART;

#ifndef CMD_LINE_INIT // in non-interrupt mode
    HANDLE hDrv;
    unsigned char tempch = ch;

    if (cmgrUART == CMGR_TURNED_OFF)
		return 0;

    if (FAILED(MxGetObjByName(CUR_CPU, UARTName[cmgrUART], &hDrv)))
        return 0;

    MxWriteDrvData(hDrv, UART_CMD_PUT_CHAR, (LPVOID)(UINT32) tempch, 1);
#else // in interrupt mode
    if (cmgrUART == CMGR_TURNED_OFF)
		return 0;
    CLWriteChar (ch);
#endif

    return ch;
}



void _ttywrch(int ch)
{
    UINT32 cmgrUART = ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART;

#ifndef CMD_LINE_INIT // in non-interrupt mode

    HANDLE hDrv;
    unsigned char tempch = ch;

    if (cmgrUART == CMGR_TURNED_OFF)
		return;

    if (FAILED(MxGetObjByName(CUR_CPU, UARTName[cmgrUART], &hDrv)))
        return;

    MxWriteDrvData(hDrv, UART_CMD_PUT_CHAR, (LPVOID)(UINT32) tempch, 1);
#else // in interrupt mode
    if (cmgrUART == CMGR_TURNED_OFF)
		return;
    CLWriteChar (ch);
#endif
}

/*
** The effect of __backspace() should be to return the last character
** read from the stream, such that a subsequent fgetc() will
** return the same character again.
*/

int __backspace(FILE *f)
{
    backspace_called = 1;
    return 0;
}

/* END of Retargeted I/O */

/*
** Exception Signaling and Handling
** ================================
** The C library implementations of ferror() uses semihosting directly
** and must therefore be retargeted.  This is a minimal reimplementation.
** _sys_exit() is called after the user's main() function has exited.  The C library
** implementation uses semihosting to report to the debugger that the application has
** finished executing.
*/



int ferror(FILE *f)
{
    return EOF;
}

void _sys_exit(int return_code)
{
    while(1);
}

typedef int FILEHANDLE;

#define STDIO

/* Standard IO device handles. */
#define STDIN   0x8001
#define STDOUT  0x8002
#define STDERR  0x8003

/* Standard IO device name defines. */
const char __stdin_name[]  = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";

int uart_printf(const char *fmt, ...);

FILEHANDLE _sys_open (const char *name, int openmode) 
{
    uart_printf ("fopen - cannot be used at this time\r\n");
    return 0;
}

int _sys_close (FILEHANDLE fh) 
{
    uart_printf ("fclose - cannot be used at this time\r\n");
    return 0;
}

int _sys_write (FILEHANDLE fh, const UINT8 *buf, UINT32 len, int mode) 
{
    uart_printf ("fwrite - cannot be used at this time\r\n");
    return 0;
}

int _sys_read (FILEHANDLE fh, UINT8 *buf, UINT32 len, int mode) 
{
    uart_printf ("fread - cannot be used at this time\r\n");   
    return 0;
}

int _sys_istty (FILEHANDLE fh) 
{
    return (0);
}

int _sys_seek (FILEHANDLE fh, long pos) 
{
    uart_printf ("fseek - cannot be used at this time\r\n");
    return 0;
}

int _sys_ensure (FILEHANDLE fh) 
{
    return 0;
}

long _sys_flen (FILEHANDLE fh) 
{
    return 0;
}

int _sys_tmpnam (char *name, int sig, unsigned maxlen) 
{
    return (1);
}

char *_sys_command_string (char *cmd, int len) 
{
    return 0;
}

