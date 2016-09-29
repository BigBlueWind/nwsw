/** @file clkcore.c
 *
 * @brief CLK module
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (USE_BARE) || defined (USE_RTXC)

#include "clkcore.h"
#include "apb.h"
#include "portio.h"
#include "sysctrl.h"

/** Set AMBA clock
 *
 * @param hclk CLK frequency in Hz
 */
void ClkSetAmbaClock(U32 hclk)
{
    V32 i;
    U32 clkf, clkr;
    U32 refclk = CLK_REFCLK / 1000000;
    clkf = (2 * hclk) / 1000000 - 1;
    clkr = (refclk / 2) - 1;

    IOWriteOr32A(CLKCORE_AMBA_PLL, PLL_BYPASS);
    IOWriteAnd32A(CLKCORE_AMBA_PLL, PLL_CTLMSK);
    IOWriteOr32A(CLKCORE_AMBA_PLL, PLL_RESET | clkf | (clkr << 16));

    // Wait 5us for PLL to lock and take PLL out of reset
    i = 100;
    while (i--)
    {
        __nop();
    }
    IOWriteAnd32A(CLKCORE_AMBA_PLL, ~PLL_RESET);

    // Wait 500 cycles and enable PLL
    i = 500;
    while (i--)
    {
        __nop();
    }
    IOWriteAnd32A(CLKCORE_AMBA_PLL, ~PLL_BYPASS);
    IOWriteOr32A(CLKCORE_AMBA_PLL, PLL_MUXSEL);
}

/** Get AMBA clock
 *
 * @return AMBA clock frequancy in Hz
 */
U32 ClkGetAmbaClock(void)
{
    U32 clkf, clkr;
    U32 refclk = CLK_REFCLK / 1000000;

    clkf = IORead32A(CLKCORE_AMBA_PLL) & 0x0FFF;
    clkr = (IORead32A(CLKCORE_AMBA_PLL) >> 16) & 0x3F;
    return (refclk * (clkf + 1) / (clkr + 1) / 4) * 1000000;
}

/** Set ARM CPU core clock
 *
 * @param fclk Core clock in Hz
 */
void ClkSetArmClock(U32 fclk)
{
    V32 i;
    U32 clkf, clkr;
    U32 refclk = CLK_REFCLK / 1000000;

#ifdef M823XX
    clkr = 4;
    clkf = (((fclk * 2) / 1000000) * clkr) / refclk;
    clkf -= 1;
    clkr -= 1;
#else
    // TODO: Update for Turner
    clkf = (4 * fclk) / 1000000 - 1;
    clkr = (2 * refclk) - 1;
#endif

    IOWriteOr32A(CLKCORE_ARM_PLL, PLL_BYPASS);
    IOWriteAnd32A(CLKCORE_ARM_PLL, PLL_CTLMSK);
    IOWriteOr32A(CLKCORE_ARM_PLL, PLL_RESET | clkf | (clkr << 16));

    // Wait 5us for PLL to lock and take PLL out of reset
    i = 100;
    while (i--)
    {
        __nop();
    }
    IOWriteAnd32A(CLKCORE_ARM_PLL, ~PLL_RESET);

    // Wait 500 cycles and enable PLL
    i = 500;
    while (i--)
    {
        __nop();
    }
    IOWriteAnd32A(CLKCORE_ARM_PLL, ~PLL_BYPASS);
}

/** Get ARM core clock
 *
 * @return ARM core clock in Hz
 */
U32 ClkGetArmClock(void)
{
    U32 clkf, clkr;
    U32 refclk = CLK_REFCLK / 1000000;

    clkf = IORead32A(CLKCORE_ARM_PLL) & 0x0FFF;
    clkr = (IORead32A(CLKCORE_ARM_PLL) >> 16) & 0x3F;
    return (refclk * (clkf + 1) / (clkr + 1) / 2) * 1000000;
}

/** Set SPU clock
 *
 * @param fclk SPU core clock in Hz
 */
void ClkSetSpuClock(U32 fclk)
{
#ifdef M823XX
    V32 i;
    U32 refclk = CLK_REFCLK / 1000000;
    U32 clkf;
    U32 clkr;

    clkr = refclk - 1;
    clkf = (fclk * 2) / 1000000 - 1;
    IOWriteOr32A(CLKCORE_SPU_PLL, PLL_BYPASS);
    IOWriteAnd32A(CLKCORE_SPU_PLL, PLL_CTLMSK);
    IOWriteOr32A(CLKCORE_SPU_PLL, PLL_RESET | clkf | (clkr << 16));

    // Wait 5us for PLL to lock and take PLL out of reset
    i = 100;
    while (i--)
    {
        __nop();
    }
    IOWriteAnd32A(CLKCORE_SPU_PLL, ~PLL_RESET);

    // Wait 500 cycles and enable PLL
    i = 500;
    while (i--)
    {
        __nop();
    }
    IOWriteAnd32A(CLKCORE_SPU_PLL, ~PLL_BYPASS);
    IOWriteOr32A(CLKCORE_SPU_PLL, PLL_MUXSEL);
#endif
}

/** Get SPU clock
 *
 * @return SPU clock in Hz
 */
U32 ClkGetSpuClock(void)
{
    U32 clkf, clkr, refclk = CLK_REFCLK / 1000000;

    clkf = IORead32A(CLKCORE_SPU_PLL) & 0x0FFF;
    clkr = (IORead32A(CLKCORE_SPU_PLL) >> 16) & 0x3F;

    return refclk * (clkf + 1) / (clkr + 1) / 2;
}

#endif /* USE_BARE || USE_RTXC */
