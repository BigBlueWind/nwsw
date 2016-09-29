/** @file bsp.c
 *
 * @brief Board Support Package for OS-free environment
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE
#ifdef M823XX

#include "bsp.h"
#include "apb.h"
#include "sysctrl.h"
#include "clkcore.h"
#include "arbiter.h"
#include "isr.h"
#include "mmu.h"
#include "timers.h"
#include "phi.h"
#include "memlimits.h"
#include "gpio.h"
#include "uexp.h"
#include "intc.h"
#include "portio.h"
#include "osal.h"

/** Initialize EVM peripherals
 *
 * @param cpuid CPU ID
 * @return
 */
RESULTCODE BspInit(U32 cpuid)
{
    __disable_irq();
    __disable_fiq();
    MmuInit(cpuid);

    if (cpuid == UARM)
    {
        // Enable High Vector area - this is Upper ARM
        SysWriteControl(SysReadControl() | CR_V | CR_I | CR_Z);

        ClkSetArmClock(CLK_ARMCLK);
        // ERAM test should (must?) be run before SPU CLK
        ClkSetSpuClock(CLK_SPUCLK);

        MemDetectSdramSize();
        IsrInit(UARM);
        GpioInit();
        UexpInit();
        AhbInit();

        // ARM0 burst mode (?)
        IOWriteOr32A(ASA_STATUS, 1);

    }
    else
    {
        SysWriteControl(SysReadControl() | CR_I | CR_Z);
        IsrInit(LARM);
    }

    // Enable data cache
    SysWriteControl(SysReadControl() | CR_C);

    return SUCCESS;
}

#endif /* M823XX */
#endif /* USE_BARE */
