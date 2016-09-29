//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL devinit.c
 *
 * @brief T4K + EVM additional devices initialization
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "devinit.h"
#include "devinfo.h"
#include "hal.h"
#include "FECdrv.h"

void DevBaseInit(void)
{
    if (MxGetCpuID () != 0)
        return;

    // to prevent writing to this register 
    // we need to read value and be sure 
    // that CRAM reset bits are really set 
    
    if (REG32(SPU_AXI_RESET_0)&((1<<7)|(1<<6)))
    {
        REG32(SPU_AXI_RESET_0) &= ~((1<<7)|(1<<6));
    }

    if (REG32(SPU_AXI_RESET_1)&((1<<0)))
    {
        REG32(SPU_AXI_RESET_0) &= ~((1<<0));
    }
}

MXRC DevInit (void)
{
   // this code is mainly should be called from Core-0
    if (MxGetCpuID () != 0)
        return MX_OK;

    // to set CEVA under reset 
    REG32(CEVA_RESET_1) = 0x3F;
    REG32(CEVA_RESET_0) = 0xF0;

    DevInfoInit ();

    if (DevInfoReadID() == HWID_T4K)
    {
        // to reset the CEVA clock to 300 Mhz (PLL-1)
        REG32(CEVA_CLK_CNTRL) = PLL_SOURCE(1);
    }

    // to set FP under reset 
    REG32(FFT_RESET_0) = 0xF0;
    REG32(FFT_RESET_1) = 0x3F;

    REG32(SYS_AXI_RESET_0) = (1 << 6) | (1 << 5) | (1 << 4); // 7:ESPAH 6:SPACC 5:FEC 4:MDMA
    REG32(SYS_AXI_RESET_0) = 0; // 7:ESPAH 6:SPACC 5:FEC 4:MDMA
//  REG32(SYS_AXI_RESET_1) = 0; // 4: DDR1 2:DDR0  1:AHB 0:IRAM

    REG32(XP_AXI_RESET_0) = 0;
    REG32(XP_AXI_RESET_1) = 0;
    REG32(XP_PHY_RESET) = 0;

    REG32(SPU_AXI_RESET_0) = 0; // take CRAM23 6: CRAM01 5: INTC 4: SPUDMA out of reset
    REG32(SPU_AXI_RESET_1) = 0; // take RAD AXI 6: SYS AXI 5: 0: CRAM45 out of reset

    // Enable AMBA 2 Protocol Timeout for System and Radio clusters APB access
    // REG32(APB_ACCESS_WS_REG) |= 1<<13;

    // set MDMA extra APB cycles to 6
    // REG32(SPU_CONFIG) |= 3<<4;
    
    /* 26 AXI Bus Dead Lock Avoidance. (added in All-Layer Change)
    '0' = Not enabled. Deadlock can happen when writes to slave in other cluster and in its own cluster are executed from both SPU and SYS AXI masters.
    '1' = Enable and the circuit active monitor Ceva write to SYS Cluster. If detected, it delays the next Ceva write to SPU slave.
    */
    REG32(SPU_CONFIG) |= 1<<26;

#if 0
    REG32(XPBUS_CTRL_REG) = 0;
    REG32(GPIO_OE_REG)  = 0x60000;
    REG32(XP_CS4_TMG1_REG) = 0xffffffff;
    REG32(XP_CS4_CFG_REG) = 1 << 9;
    // Enable Expansion Clock Output, CS0, CS1, CS2, CS3, CS4
    REG32(XP_CSEN_REG) |= 0x3F;
    REG32(GPIO_PIN_SELECT_REG) &= ~((1 << 18) | (1 << 17) | (1 << 16));
#endif

#if 0
    // 0100_0100_0100_0100_0100_1010_1000_0110
    // X_SYS2SPU
    // X_RAD2SPU
    // X_CPRI2SPU
    // X_CEVA036
    // X_CEVA147
    // X_CEVA258
    // X_CEVAMDMA
    // X_FPGROUP0
    
    REG32(SPU_AXI_MASTER_PRI_A) = 0;
    // X_FPGROUP1 
    // RES 
    // CEVA9 in X_CEVAMDMA 
    // MDMA in X_CEVAMDMA  
    REG32(SPU_AXI_MASTER_PRI_B) = 0;
    
    REG32(CFG_SYS_BUS_PRIO) = 0;//(1<<18) | (0<<15) | (0<<12) | (1<<9) | (1<<1) | (1<<1) | (2);

    REG32(XP_SRDS_CNTRL) = 0;
    REG32(GPIO_SRDS_PLL_CONF_REG) = 0x027; // changing input clock to differencial clock
#endif    

	if (IS_HBI_MODE())
	{
            // Need to initialize chip select registers
            // 16 bits data interface for CS0
            REG32(XP_CS0_CFG_REG) = (1 << 1);
            // Define memory segment for CS0
            REG32(XP_CS0_BASE_REG) = NOR16_BASEADDR >> 12;
            REG32(XP_CS0_SEG_REG) = (NOR16_BASEADDR+0x4000000) >> 12; // 64M
            // 8 bits data interface for CS1
            REG32(XP_CS1_CFG_REG) |= 0;
            // Define memory segment for CS1
            REG32(XP_CS1_BASE_REG) = NOR8_BASEADDR >> 12;
            REG32(XP_CS1_SEG_REG) = (NOR8_BASEADDR+0x80000) >> 12; // 512K

            // 8 bits data interface for CS2
            REG32(XP_CS2_CFG_REG) |= 0;
            // Define memory segment for CS2
            REG32(XP_CS2_BASE_REG) = FPGA_BASEADDR >> 12;
            REG32(XP_CS2_SEG_REG) = (FPGA_BASEADDR+0x4000000) >> 12; // 64M

            REG32(XP_CS2_TMG1_REG) = (0x25 << 24) | (REG32(XP_CS2_TMG1_REG) & 0xFFFFFF);
            REG32(XP_CS2_TMG2_REG) = (0x00 << 24) | (REG32(XP_CS2_TMG2_REG) & 0xFFFFFF);

            // 16 bits data interface for CS3
            REG32(XP_CS3_CFG_REG) = (1 << 1);
            // Define memory segment for CS3
            REG32(XP_CS3_BASE_REG) = RADIO_BASEADDR >> 12;
            REG32(XP_CS3_SEG_REG) = (RADIO_BASEADDR+0x4000000) >> 12; // 64M

            REG32(XP_CS4_BASE_REG) = NAND_BASEADDR >> 12;
            REG32(XP_CS4_SEG_REG) =  (NAND_BASEADDR >> 12) | 0xFFF;

            REG32(XPBUS_CTRL_REG) = 0;
    //      REG32(GPIO_OE_REG)  = 0x60000;
            REG32(XP_CS4_TMG1_REG) = 0xffffffff;
            REG32(XP_CS4_CFG_REG) = 1 << 9;
            // Enable Expansion Clock Output, CS0, CS1, CS2, CS3, CS4
            REG32(XP_CSEN_REG) |= 0x3F;

            *(PUINT32)GPIO_OUTPUT_REG |= (1 << 18);
            REG32(GPIO_PIN_SELECT_REG) &= ~((1 << 18) | (1 << 17) | (1 << 16));
	}
	else
	{
            // 8 bits data interface for CS2
            REG32(XP_CS2_CFG_REG) |= 0;
            // Define memory segment for CS2
            REG32(XP_CS2_BASE_REG) = FPGA_BASEADDR >> 12;
            REG32(XP_CS2_SEG_REG) = (FPGA_BASEADDR+0x4000000) >> 12; // 64M

            REG32(XP_CS2_TMG1_REG) = (0x25 << 24) | (REG32(XP_CS2_TMG1_REG) & 0xFFFFFF);
            REG32(XP_CS2_TMG2_REG) = (0x00 << 24) | (REG32(XP_CS2_TMG2_REG) & 0xFFFFFF);

            // 16 bits data interface for CS3
            REG32(XP_CS3_CFG_REG) = (1 << 1);
            // Define memory segment for CS3
            REG32(XP_CS3_BASE_REG) = RADIO_BASEADDR >> 12;
            REG32(XP_CS3_SEG_REG) = (RADIO_BASEADDR+0x4000000) >> 12; // 64M

            // Enable Expansion Clock Output, CS0, CS1, CS2, CS3, CS4
            REG32(XP_CSEN_REG) |= 0x3F;

            *(PUINT32)GPIO_OUTPUT_REG |= (1 << 18);
            REG32(GPIO_PIN_SELECT_REG) &= ~((1 << 18) | (1 << 17) | (1 << 16));
	}

	return MX_OK;
}
