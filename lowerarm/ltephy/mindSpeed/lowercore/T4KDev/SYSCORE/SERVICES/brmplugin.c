//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL brmplugin.c
 *
 * @brief T4K SOC initialization module
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "4gmx.h"
#include "hal.h"
#include "sysddrctrl.h"
#include "sysclkctrl.h"

UINT32 BrmPlugin(PUINT32 pAckBuf)
{
	REG32(SYS_AXI_RESET_0) = 0; // 7:ESPAH 6:SPACC 5:FEC 4:MDMA
	REG32(SYS_AXI_RESET_1) = 0; // 4: DDR1 2:DDR0  1:AHB 0:IRAM

	REG32(XP_AXI_RESET_0) = 0;
	REG32(XP_AXI_RESET_1) = 0;
	REG32(XP_PHY_RESET) = 0;

	REG32(SPU_AXI_RESET_0) = 0; // take CRAM23 6: CRAM01 5: INTC 4: SPUDMA out of reset
	REG32(SPU_AXI_RESET_1) = 0; // take RAD AXI 6: SYS AXI 5: 0: CRAM45 out of reset

	// Enable AMBA 2 Protocol Timeout for System and Radio clusters APB access
	REG32(APB_ACCESS_WS_REG) |= 1<<13;

	// set MDMA extra APB cycles to 6
	REG32(SPU_CONFIG) |= 3<<4;

    SysClocksSetup();
    
	SysDDRSetup(0);
	SysDDRSetup(1);

    REG32(TIMER0_CNTR_REG) = (UINT32)0xFFFFFFFF;

	return 0;
}
