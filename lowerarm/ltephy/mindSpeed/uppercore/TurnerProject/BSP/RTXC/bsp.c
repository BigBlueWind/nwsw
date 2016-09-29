/** @file bsp.c
 *
 * @brief Board Support Package for RTXC
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_RTXC
#ifdef M823XX

#include "basetypes.h"
#include "portio.h"
#include "sysctrl.h"
#include "apb.h"
#include "arbiter.h"
#include "uart.h"
#include "clkcore.h"
#include "mmu.h"
#include "intc.h"
#include "phi.h"
#include "timers.h"
#include "isr.h"
#include <config.h>
#include <koptions.h>
#include <rccodes.h>
#include <typedef.h>
#include <rtxcapi.h>

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

extern __irq void IrqHandler(void);
extern __irq void IrqHandlerL(void);

#ifdef __cplusplus
}      // extern "C"
#endif /*__cplusplus*/

/** Initialize EVM peripherals
 *
 *  @param cpuid CPU ID
 *  @return Standard result code
 */
RESULTCODE BspInit(U32 cpuid)
{
    __disable_irq();
    __disable_fiq();

    // Looks like this must be done before APB access
    MmuInit(cpuid);

    if (cpuid == UARM)
    {
        // Early disable of PCI interrupt
        IOWriteAnd32A(PHI_INT_CTRL, ~(PHI_TXM3IE | PHI_RXM3IE));

        // TODO: put into external boot loader
        IOWrite32A(SYSMEM_CONFIG, 0xF);
        IOWrite32A(UEXP_CONFIG, 4);
        IOWrite32A(GPIO_APB_ACCESS_WS, 0x0);

        // ARM0 burst enable (?)
        IOWrite32A(ASA_STATUS, 3);

        // TODO: Call AhbInit()
        IOWriteOr32A(ASA_TC_CR, ASA_TC_REQARM0EN | ASA_TC_REQARM1EN | ASA_TC_REQIDMAEN);
        IOWriteOr32A(ASA_ARAM_TC_CR, ASA_TC_REQARM0EN | ASA_TC_REQARM1EN | ASA_TC_REQIDMAEN);
        IOWriteOr32A(ASA_ERAM_TC_CR, ASA_TC_REQARM0EN | ASA_TC_REQARM1EN | ASA_TC_REQIDMAEN);
        IOWriteOr32A(ASA_UEXP_TC_CR, ASA_TC_REQARM0EN | ASA_TC_REQARM1EN);
        // SMC=5, TDM=4, ARM0=1, ARM1=2, PCI=3
        IOWrite32A(ASA_PRI, (5 << 16) | (4) | (1 << 4) | (2 << 8) | (3 << 12));

        // Pulse ???
        IOWriteAnd32A(GPIO_OUTPUT, ~(1 << 2));
        IOWriteOr32A(GPIO_OUTPUT, 1 << 2);

        // UART1 board specific init
        // (moved to uart.c
        // TODO: remove here
//        IOWrite32A(UEXP_CS_CFGA4, 0x1106);
//        IOWriteOr32A(ASD_CS_ENABLE, 1 << UEXP_CS4);
//        IOWrite32A(M823XX_PLD_UART_MUX, 1);

        ClkSetArmClock(CLK_ARMCLK);
        // ERAM test should (must?) be run before SPU CLK
        ClkSetSpuClock(CLK_SPUCLK);

        // Enable high vectors area - this is Upper ARM
        SysWriteControl(SysReadControl() | CR_V);
    }

    IsrInit(cpuid);

    return SUCCESS;
}

#endif /*USE_RTXC*/
#endif /*M823XX*/
