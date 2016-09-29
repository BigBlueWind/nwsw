//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL sysclkctrl.c
 *
 * @brief Clocks and Resets Control Module
 * @author Mindspeed Technologies
 * @version $Revision: 1.34 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx.h"
#include "hal.h"

#include "lcorecfg.h"
#include "config.h"

//-------------------------------------------------------------------------------------------

// Define function to easily configure PLL clock selection regsisters.
#ifndef PLL_SOURCE
#define PLL_SOURCE(pll)     (((pll+1)<<1)|1)
#endif

// The line below is used to select the desired device T4000 or T3000.
// For T3000 CLKCFG_T3000 must be defined.  For T4000 CLKCFG_T3000 must not be
// defined.

//#define CLKCFG_T3000

/******************************************************************************
**********           DEVICE PLL ASSIGNMENTS AND FREQUENCIES          **********
*******************************************************************************

    ------------------------------------------------------------------------
                     |--------- T4000 ---------| |--------- T3000 ---------|
    DEVICE    UBOOT   PLL   PLL    PLL   DEVICE   PLL   PLL    PLL   DEVICE
                            FREQ   DIV   FREQ           FREQ   DIV   FREQ
    ------    -----   ---   ----   ---   ------   ---   ----   ---   ------
    DDR       YES     0     120    1     600      0     100    1     500
    RAD AXI   YES     1     600    2     300      1     600    2     300
    XP AXI    YES     1     600    4     150      1     600    4     150
    FEC DL    YES     2     525    1     525      2     525    1     525
    SYS AXI   YES     2     525    2     262.5    2     525    2     262.5
    ARMDP     YES     2     525    1     525      2     525    1     525
    ARMQP     YES     3     700    1     600      5     600    1     600
    CEVA      YES     3     700    1     700      3     400    1     400
    FP        YES     3     700    1     700      3     400    1     400
    SPU AXI   YES     3     700    2     350      3     400    2     200
    FEC UL    YES     4     450    1     450      4     450    1     450
    ------------------------------------------------------------------------

    Notes:

    1) New U-BOOT now configures all required PLLs and device clocks and
       enables all of the PLLs (PLL enable bit in register TOP_GEN_CNTRL will
       be 0).  If the new U-BOOT has not run, then PLLs (0 - 5) will be
       disabled (register TOP_GEN_CNTRL = 0x3f).
    2) All frequencies are in MHz.
    3) Actual DDR operating frequency is 5 times PLL frequency.
    4) ARMDP is ARM dual core processor.  ARMQP is ARM quad core processor.
    5) Because of X7 hardware issue, DL FEC and SYS AXI must use the same PLL 
       clock.
    6) Because ARMDP is configured for synchronous mode, the ARMDP must use the
       same PLL as the SYS AXI.
    7) Because the CEVA is configured for synchronous mode, the CEVA must use
       the same PLL as the SPU AXI.

*******************************************************************************
**********                     PLL PROGRAMMING                       **********
*******************************************************************************

The operation of the PLLs as well as the system clocks is defined in document:
    clkrst_hdd.doc.

The output clock corresponding to each PLL is defined below.

    PLL clkout = (refclk*(NF+1)/(NR+1))/(OD+1)

    Where:  refclk =    reference clock = 25MHz.
            NF =        clock multiplier.  0 <= NF <= 4095.
            NR =        1st clock divisor.  0 <= NR <= 63.
            OD =        2nd clock divisor.  0 <= OD <= 15.

    Note:   NF, NR and OD must be 0 or an odd number.

Each PLL (n = 0 to 5) is programmed as defined below.

    REG32(PLLn_NR) = NR;
    REG32(PLLn_NF_0) = NF;
    REG32(PLLn_BWADJ_0) = NF;
    REG32(PLLn_OD) = OD;
    REG32(PLLn_CNTRL) = 0;

*******************************************************************************
**********                USEFUL PLL PROGRAMMING VALUES              **********
*******************************************************************************

    -----------------------------------
    NR      NF       OD      PLL clkout
    -----------------------------------
    0x0     0x27     0x9     100 MHz
    0x0     0x2F     0x9     120 MHz
    0x0     0x3F     0x3     400 MHz
    0x0     0x6B     0x5     450 MHz
    0x0     0x4f     0x3     500 MHz
    0x0     0x53     0x3     525 MHz
    0x0     0x5F     0x3     600 MHz
    0x0     0x6F     0x3     700 MHz
    -----------------------------------

******************************************************************************/

void SysClocksSetup(void)
{
    // If new u-boot has not configured system clocks, then.
    if ((REG32(TOP_GEN_CNTRL) & 0x3f) == 0x3f)
    {
        // 120 MHZ from PLL0 (DDR PHY x5)
        REG32(PLL0_NR)      = 0x0;
        REG32(PLL0_NF_0)    = 0x2F;
        REG32(PLL0_BWADJ_0) = 0x2F;
        REG32(PLL0_OD)      = 0x9;
        REG32(PLL0_CNTRL)   = 0;

        // 600 MHz from PLL1 (for RAD, XP AXIs)
        REG32(PLL1_NR)      = 0x0;
        REG32(PLL1_NF_0)    = 0x5f;
        REG32(PLL1_BWADJ_0) = 0x5f;
        REG32(PLL1_OD)      = 0x3;
        REG32(PLL1_CNTRL)   = 0;

        // 600 MHz from PLL2 (for SYS AXIs)
        REG32(PLL2_NR)      = 0x0;
        REG32(PLL2_NF_0)    = 0x5f;
        REG32(PLL2_BWADJ_0) = 0x5f;
        REG32(PLL2_OD)      = 0x3;
        REG32(PLL2_CNTRL)   = 0;

#ifdef CONFIG_TRANSCEDE_3000
        // 500 MHz from PLL3 (MAP, CEVA)
        REG32(PLL3_NR)      = 0;
        REG32(PLL3_NF_0)    = 0x4f;
        REG32(PLL3_BWADJ_0) = 0x4f;
        REG32(PLL3_OD)      = 3;
        REG32(PLL3_CNTRL)   = 0;

        REG32(CV_AXI_CLK_DIV_CNTRL) = 0x82;

        // Configure CEVA to use synchronous mode
        REG32(SPU_AXI_CLK_CNTRL_0) = 0xF3;
        REG32(CFG_SYS_BUS_CFG) |= (1<<3);

        REG32(ARMQP_CLK_CNTRL) = PLL_SOURCE(1);
#else
        // 600 MHz from PLL3 (DA9, QA9, MAP, CEVA, SPU AXI)
        REG32(PLL3_NR)      = 0;
        REG32(PLL3_NF_0)    = 0x5f;
        REG32(PLL3_BWADJ_0) = 0x5f;
        REG32(PLL3_OD)      = 3;
        REG32(PLL3_CNTRL)   = 0;
        // QA9 clock from PLL3
        REG32(ARMQP_CLK_CNTRL) = PLL_SOURCE(3);
#endif

        // 450 MHz from PLL4 (FEC UL)
        REG32(PLL4_NR)      = 0;
        REG32(PLL4_NF_0)    = 0x6b;
        REG32(PLL4_BWADJ_0) = 0x6b;
        REG32(PLL4_OD)      = 5;
        REG32(PLL4_CNTRL)   = 0;

        // XP_AXI_CLK from PLL2 and SYS_AXI_CLK from PLL1
        REG32(AXI_CLK_CNTRL_0) = (PLL_SOURCE(1)<<4) | PLL_SOURCE(2);
        // SPU_AXI_CLK from PLL3 and RAD_AXI_CLK from PLL2
        REG32(AXI_CLK_CNTRL_1) = (PLL_SOURCE(3)<<4) | PLL_SOURCE(1);

        // XP_AXI_Clk_Div_Ratio is 4, SYS_AXI_Clk_Div_Ratio is 2
        REG32(AXI_CLK_DIV_CNTRL_0) = 0x42;
        // SPU_AXI_Clk_Div_Ratio is 2, RAD_AXI_Clk_Div_Ratio is 2
        REG32(AXI_CLK_DIV_CNTRL_1) = 0x22;

        // Enable Turner top AXI clock divider
        REG32(AXI_CLK_DIV_CNTRL_2) = 0;
        
        // set DA9 to sync mode
        //REG32(SYS_AXI_CLK_CNTRL_0) = 0xF3;
        REG32(CFG_SYS_BUS_CFG) |= (1 << 4);
        
        // DDR clock from PLL0
        REG32(DDR_CLK_DIV_CNTRL) = 0x82;
        REG32(DDR_CLK_CNTRL) = PLL_SOURCE(0);
        
        // Program FEC DL clock control to bypass divider
        REG32(FEC_DL_CLK_DIV_CNTRL) = 0x82;
        // Program FEC DL clock control to use PLL2
        REG32(FEC_DL_CLK_CNTRL) = PLL_SOURCE(2);

        // DA9 clock from PLL2
        REG32(ARMDP_CLK_CNTRL) = PLL_SOURCE(2);

        // Program CEVA clock control to bypass divider
        REG32(CEVA_CLK_DIV_CNTRL) = 0x82;
        // Program CEVA clock control to use PLL5
        REG32(CEVA_CLK_CNTRL) = PLL_SOURCE(3);

        // Program FFT clock control to bypass divider.
        REG32(FFT_CLK_DIV_CNTRL) = 0x82;
        // Program FFT clock control to use PLL3
        REG32(FFT_CLK_CNTRL) = PLL_SOURCE(3);

        // Program FEC UL clock control to bypass divider
        REG32(FEC_UL_CLK_DIV_CNTRL) = 0x82;
        // Program FEC UL clock control to use PLL4
        REG32(FEC_UL_CLK_CNTRL) = PLL_SOURCE(4);
        
        //// by a.g.
        //// according to changes is SYS_CLOCK (Clocking and Reset document: Version 3.0, 05/25/2009)
        //// we should do it to make CoreSite working on AXI bus freq.
        //// problems detected if default value (0x82) is used
        REG32(SYS_TPI_CLK_DIV_CNTRL) = 0xC2;
        REG32(TPI_CLK_DIV_CNTRL) = 6;
        REG32(TPI_CLK_CNTRL) = PLL_SOURCE(1);

        // Take 0-4 PLLs out of reset
        REG32(TOP_GEN_CNTRL) &= ~0x1f;
    }
    // Else, if existing u-boot (not new u-boot) has already run, then.
    else if ((REG32(TOP_GEN_CNTRL) & 0x3f) == 0x3c)
    {
        // Configure PLL3 clock = 700MHz.
        REG32(PLL3_NR)      = 0;
        REG32(PLL3_NF_0)    = 0x6f;
        REG32(PLL3_BWADJ_0) = 0x6f;
        REG32(PLL3_OD)      = 3;
        REG32(PLL3_CNTRL)   = 0;
	
        // Configure PLL4 clock = 450MHz.
        REG32(PLL4_NR)      = 0;
        REG32(PLL4_NF_0)    = 0x6b;
        REG32(PLL4_BWADJ_0) = 0x6b;
        REG32(PLL4_OD)      = 5;
        REG32(PLL4_CNTRL)   = 0;

        // Configure SPU_AXI and RAD_AXI PLLs.
        // SPU_AXI_CLK_Source = Bits(7:4) = PLL3.
        // RAD_AXI_CLK_Source = Bits(3:0) = PLL1.
        REG32(AXI_CLK_CNTRL_1) = (PLL_SOURCE(3)<<4) | PLL_SOURCE(1);
    
        // Configure FEC DL clock control to bypass divider.
        REG32(FEC_DL_CLK_DIV_CNTRL) = 0x82;

        // Configure FEC DL clock control to use PLL1.
        REG32(FEC_DL_CLK_CNTRL) = PLL_SOURCE(1);

        // Configure CEVA clock control to bypass divider.
        REG32(CEVA_CLK_DIV_CNTRL) = 0x82;

        // Configure CEVA clock control to use PLL3.
        REG32(CEVA_CLK_CNTRL) = PLL_SOURCE(3);

        // Configure FFT clock control to bypass divider.
        REG32(FFT_CLK_DIV_CNTRL) = 0x82;

        // Configure FFT clock control to use PLL3.
        REG32(FFT_CLK_CNTRL) = PLL_SOURCE(3);

        // Configure FEC UL clock control to bypass divider.
        REG32(FEC_UL_CLK_DIV_CNTRL) = 0x82;

        // Configure FEC UL clock control to use PLL4.
        REG32(FEC_UL_CLK_CNTRL) = PLL_SOURCE(4);

        // Enable PLL 3 and 4 clocks.
        REG32(TOP_GEN_CNTRL) &= ~0x18;

	}
}
