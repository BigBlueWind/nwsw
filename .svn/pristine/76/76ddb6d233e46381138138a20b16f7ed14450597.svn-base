/** @file startup.c
 *
 * @brief C startup code for ARM RVDS
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef __CC_ARM

#ifdef USE_BARE
#pragma import(__use_two_region_memory)
#pragma import(__use_realtime_heap)
#endif /*USE_BARE*/

extern void $Super$$main(void);

/** Called from ARM RVDS C library. Calls main() ($Super$$main)
 *
 */
void $Sub$$main(void)
{
    // Calls the original function main()
    $Super$$main();

    // Dead here in case we reached this point
    while (1)
    {
        __nop();
    }
}

#ifdef USE_RTXC
/** Set up stack/heap according to ARM EABI
 */
__asm void __user_initial_stackheap(void)
{
    IMPORT RtlHeap
    IMPORT RtlStack
    LDR r0,=RtlHeap
    LDR r1,=RtlStack+4092
    LDR r2,=RtlHeap+4092
    LDR r3,=RtlStack
    BX  lr
}
#endif /* USE_RTXC */

#endif /* __CC_ARM */
