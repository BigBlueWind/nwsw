//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL sysddrctrl.c
 *
 * @brief DDR control module
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx.h"
#include "hal.h"
#include "sysddrctrl.h"

#define INT_PLL_CLOCK

void SysDDRTraining(PDENALIDDRC pDDR);

const UINT32 DdrControllerParams[] = {

/*0x000*/ 0x00000101,0x01010100,0x00000000,0x01000101,0x00000001,0x01000000,0x00010001,0x00000000,
/*0x020*/ 0x00000000,0x00000000,0x00000000,0x01010000,0x01010000,0x00000000,0x00000101,0x00000001,
/*0x040*/ 0x00000001,0x01000101,0x00030001,0x01020000,0x00010102,0x02000100,0x04000303,0x07000000,
/*0x060*/ 0x09070700,0x00050404,0x02000100,0x090c0a02,0x00000006,0x00000204,0x050A0902,0x12020707,
/*0x080*/ 0x00000012,0x00081203,0x3f090902,0x1e12003f,0x00642819,0x00000064,0x00000000,0x00000040,
/*0x0A0*/ 0x03030000,0x0403030c,0x4209160c,/*0x05970000,*/0x05870000,0x21000000,0x00000000,0x000a0000,0x01000000,
/*0x0C0*/ 0x02000040,0x091b091b,0x091b091b,0x091b091b,0x5555091b,0x8000ff00,0x0a500000,0x00020a50,
/*0x0E0*/ 0x00100002,0x00000010,0x00000000,0x00000000,0x00000000,0x23232323,0x23232323,0x00002323,
/*0x100*/ 0x00000000,0x00000000,0x00000000,0x00000000,0x00100000,0x0000ffff,0x00000000,0x00000000,
/*0x120*/ 0x00000000,0x00000000,0x00000000,0x00000000,0x00040200,0x000fa42a,0x02000048,0x00000000,
/*0x140*/ 0x00000000,0x00000000,0x00000004,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
/*0x160*/ 0x00000000,0x06000000,0x012c0000,0x51890000,0x51890001,0x51890001,0x51890001,0x51890001,
/*0x180*/ 0x00000001,0x00000000,0x00000000,0x00000000,0x00780000,0x00000000,0x00000000,0x00000000,
/*0x1A0*/ 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
/*0x1C0*/ 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00004000
};

const UINT16 WritePhaseDelays[2][5] = {
    {7, 6, 1, 1, 2},
    {7, 6, 1, 1, 2},
};

/**
 * @brief function setup 16 bit or 32 bit
 * aligned address access to the DDR3 PHY controller
 * in Transcede devices
 *
 * @param DdrPhyAddress[in] Unsigned 32 bit address of DDR PHY register to access
 *
 * @returns Unsigned 32 bit address of data to read or write adjusted as necessary for proper access
 */
UINT32 SetupDdrPhyAccess(UINT32 DdrPhyAddress)
{
	UINT32 ReadData;
    UINT32 AdjustedAddress = DdrPhyAddress;

    /* 
     * Get current value of control register
     * to select 16 bit or 32 bit aligned data
     */
	ReadData = REG32(0xFE430000);

    /*
     * Test if Address is 32 bit or 16 bit aligned
     */
	if((DdrPhyAddress & 0x00000003) != 0)
    {
        /* 
         * Address is not 32 bit aligned
         * Set upper bit to select 16 bit aligned
         * DDR phy access
         */
	 	REG32(0xFE430000) = ReadData | 0x80000000;
        /*
         * Force address to write to PHY to address - 2
         * to write to a 32 bit aligned lower address
         * which is then forced to the original
         * address in the PHY itself
         */
	 	AdjustedAddress -= 2;
	}
    else
    {
        /* 
         * Address is 32 bit aligned
         * Clear upper bit to select 16 bit aligned
         * DDR phy access
         */
		REG32(0xFE430000) = ReadData & 0x7FFFFFFF;
	}
    /*
     * Address selected OK, return address to then
     * later access the PHY (decremented by 2 if
     * 16 bit aligned).
     */
	return(AdjustedAddress);
}

/**
 * @brief function to read from the DDR3 PHY controller
 * in Transcede devices
 *
 * @param [in] DdrPhyAddress Unsigned 32 bit address of DDR PHY register to access (must be 16 bit aligned)
 *
 * @returns 16 bit value read from the DDR PHY controller as an unsigned 32 bit value
 */
UINT32 ReadDdrPhyRegister(UINT32 DdrPhyAddress)
{
    UINT32 ReadData;
    UINT32 ReadAddress;

    /* Setup PHY access and get adjusted address
     * to actually write to based on value of Address
     */
	ReadAddress = SetupDdrPhyAccess(DdrPhyAddress);
    /*
     * Address selected OK, read intended data to PHY 
     * 16 bit register, clear upper bits to be safe.
     */
	ReadData = REG32(ReadAddress);
    return(ReadData & 0xFFFF);
}

/**
 * @brief function to read from the DDR3 PHY controller
 * in Transcede devices.
 *
 * @details
 *
 * @par NOTE
 * Note: This function must be called after DDR initialization is complete, not before
 *
 * @param [in] DdrPhyAddress Unsigned 32 bit address of DDR PHY register to access (must be 16 bit aligned)
 *
 * @returns 7 if non 32 bit aligned, but 16 bit aligned register in PHY writes and reads OK, otherwise returns 6
 */
unsigned int GetChipRevisionByTestingDdrPhy(void) 
{
	UINT32 ReadData1;
    UINT32 ReadData2;
	UINT32 RevisionID = 7; /* Assume newer version of transcede device */
    /*
     * Read value from a specific non-32-bit aligned DDR PHY register
     * and also a another register should will be different
     * if DDR access fix is in this chip revision
     */
	ReadData1 = ReadDdrPhyRegister(0xFE450020);
    ReadData2 = ReadDdrPhyRegister(0xFE450022);
    /*
     * Test the 2 values, if they are the same, then this does
     * not have the bug fix and values read are the same.
     *
     * If values are read are different, then this has the bug
     * fix and is the newer revision.
     */
    if (ReadData1 == ReadData2)
    {
        RevisionID = 6;
    }
    /*
     * Return detected revision ID
     */

    //printf("GetChipRevisionByTestingDdrPhy: Data1=%d, Data2=%d\n",ReadData1, ReadData2);
    //printf("GetChipRevisionByTestingDdrPhy: RevisionID=%d\n", RevisionID);

	return RevisionID;
}

#ifndef DDR0_CONTROLLER_BASE
#define DDR0_CONTROLLER_BASE            0xFBE00000 /**< @brief Internal DDR3 DRAM controller 0 base address */
#endif

#ifndef DDR1_CONTROLLER_BASE
#define DDR1_CONTROLLER_BASE            0xFBE80000 /**< @brief Internal DDR3 DRAM controller 0 base address */
#endif

/**
 * @brief Function to write protect a region of memory for a Denali
 * memory controller
 * 
 * @details
 * 
 * @par Overview
 * This function will cause the denali controller to write protect 
 * a region of memory from all writes regardless of source.
 * 
 * @param ddr_controller_id[in]            DDR controller ID, 0 or 1 (NOTE: C5K does not support DDR 1)
 * @param write_protect_region_number[in]  Write protect region, Denali supports two regions, 0 and 1 
 * @param lower_address[in]                Lower (starting) address to protect
 * @param upper_address[in]                Upper (ending)   address to protect
 *  
 * @returns Nothing (void function)
 */
void SysDDREnableWriteProtect(unsigned int ddr_controller_id,
                              unsigned int write_protect_region_number,
                              unsigned int lower_address,
                              unsigned int upper_address
                             )
{
    volatile unsigned char * pDDRController;

    if (ddr_controller_id==0)
    {
        // DDR1 bank
        pDDRController = (volatile unsigned char *) DDR0_CONTROLLER_BASE;		
    }
    else
    {
        // DDR1 controller
        pDDRController = (volatile unsigned char *) DDR1_CONTROLLER_BASE;		
    }

        pDDRController[0x54] = 0; // Disable write protect

    if (write_protect_region_number == 0)
    {
        // Set lower address
        pDDRController[0x1B9] = (unsigned char) (lower_address       & 0xFF); // Address bits  7:0
        pDDRController[0x1BA] = (unsigned char) ((lower_address>>8)  & 0xFF); // Address bits 15:8
        pDDRController[0x1BB] = (unsigned char) ((lower_address>>16) & 0xFF); // Address bits 23:16
        pDDRController[0x1BC] = (unsigned char) ((lower_address>>24) & 0xFF); // Address bits 31:24
        pDDRController[0x1BD] = 0;                                            // Address bits 35:32
        // Set upper address
        pDDRController[0x1C3] = (unsigned char) (upper_address       & 0xFF); // Address bits  7:0
        pDDRController[0x1C4] = (unsigned char) ((upper_address>>8)  & 0xFF); // Address bits 15:8
        pDDRController[0x1C5] = (unsigned char) ((upper_address>>16) & 0xFF); // Address bits 23:16
        pDDRController[0x1C6] = (unsigned char) ((upper_address>>24) & 0xFF); // Address bits 31:24		
        pDDRController[0x1C7] = 0;                                            // Address bits 35:32
    }
    else
    {
        // Set lower address
        pDDRController[0x1BE] = (unsigned char) (lower_address       & 0xFF); // Address bits  7:0
        pDDRController[0x1BF] = (unsigned char) ((lower_address>>8)  & 0xFF); // Address bits 15:8
        pDDRController[0x1C0] = (unsigned char) ((lower_address>>16) & 0xFF); // Address bits 23:16
        pDDRController[0x1C1] = (unsigned char) ((lower_address>>24) & 0xFF); // Address bits 31:24
        pDDRController[0x1C2] = 0;                                            // Address bits 35:32
        // Set upper address
        pDDRController[0x1C8] = (unsigned char) (upper_address       & 0xFF); // Address bits  7:0
        pDDRController[0x1C9] = (unsigned char) ((upper_address>>8)  & 0xFF); // Address bits 15:8
        pDDRController[0x1CA] = (unsigned char) ((upper_address>>16) & 0xFF); // Address bits 23:16
        pDDRController[0x1CB] = (unsigned char) ((upper_address>>24) & 0xFF); // Address bits 31:24		
        pDDRController[0x1CC] = 0; 
    }
    // Address setup, enable write protection
    pDDRController[0x54] = 1; // Enable write protect
}

/**
 * @brief Function to write protect a region of memory for a Denali
 * memory controller
 * 
 * @details
 * 
 * @par Overview
 * This function will cause the denali controller to disable write protect 
 * a region of memory from all writes regardless of source.
 * 
 * @details
 * 
 * @par Overiview
 * This function is used to disable a write protect region in the DDR controller.
 * If the region was previously disabled, this will restore the capability
 * for writes into the region.
 *  
 * @returns Nothing (void function)
 */
void SysDDRDisableWriteProtect(unsigned int ddr_controller_id)
{
    volatile unsigned char * pDDRController;

    if (ddr_controller_id==0)
    {
        // DDR1 bank
        pDDRController = (volatile unsigned char *) DDR0_CONTROLLER_BASE;		
    }
    else
    {
        // DDR1 controller
        pDDRController = (volatile unsigned char *) DDR1_CONTROLLER_BASE;		
    }

    pDDRController[0x54] = 0; // Disable write protect
}

void SysDDRTraining(volatile PDENALIDDRC pDDR)
{
	volatile UINT32 delay_count;

    // gate leveling
    pDDR->use_mc_eval_rdgate = 1;

    delay_count = 500;
    while (delay_count--); // 100 cycles delay

    // select CS0
    pDDR->RDLVL_CS = 0;
    pDDR->SW_LEVELING_MODE = 3;
    pDDR->SWLVL_START = 1;

    delay_count = 500;
    while (delay_count--); // 100 cycles delay

    while (pDDR->SWLVL_OP_DONE == 0);

    pDDR->INT_ACK_8_15 = 0x10;
    pDDR->SWLVL_LOAD = 1;

    delay_count = 500;
    while (delay_count--); // 100 cycles delay

    while (pDDR->SWLVL_OP_DONE == 0);

    pDDR->SWLVL_EXIT = 1;

    delay_count = 500;
    while (delay_count--);

    pDDR->use_mc_eval_rdgate = 0;
}


UINT32 SysDDRGetFreq (UINT32 nID, UINT32 SrcClock)
{
    volatile PDENALIDDRC pDDR;
    UINT32 nVal;
    UINT32 nDiv [4] = {1,4,8,16};

    pDDR = (PDENALIDDRC) (nID ? (DDR1_CONTROLLER_BASE):(DDR0_CONTROLLER_BASE));

    // if DDR-1 is in reset
    //--------------------------------------------
    if (nID == 1 && (REG32(SYS_DDR_RESET) & 0x10))
        return 0;

    nVal = SrcClock * (pDDR->pll_fbdiv+1);
    nVal /= nDiv[pDDR->pll_odiv2];

    return nVal;
}


UINT32 SysDDRSetup(UINT32 ddr_controller_id)
{
    UINT32 try_count=0;
    UINT32 i;
    volatile PDDRPHYSLICEREGS p_ddrphy_slice_ctrl;
    volatile UINT32 value;
    volatile UINT32 dummy;
    volatile UINT32 *p_mem;
    volatile UINT8 *p_controller_regs;
    volatile PDENALIDDRC pDDR;
    volatile UINT8 *p_denali_params = (UINT8 *)DdrControllerParams;

#ifdef INT_PLL_CLOCK
    REG32(GP_CONFIG) |= 1; // set ddr_phy_refclk_sel to system PLL
#endif

    if (ddr_controller_id)
    {
        p_controller_regs = (UINT8 *)DDR1_CONTROLLER_BASE;
        p_ddrphy_slice_ctrl = (PDDRPHYSLICEREGS)(DDRPHY1_BASE+0x40);

        REG32(CFG_SYS_DDR_LL1) = DDR1_BASEADDR >> 20;
        REG32(CFG_SYS_DDR_HL1) = (DDR1_BASEADDR+DDR1_SIZE) >> 20;

        REG32(SYS_DDR_RESET) &= ~0x10;

        p_mem = (UINT32 *)DDR1_BASEADDR;
    }
    else
    {
        p_controller_regs = (UINT8 *)DDR0_CONTROLLER_BASE;
        p_ddrphy_slice_ctrl = (PDDRPHYSLICEREGS)(DDRPHY0_BASE+0x40);

        REG32(CFG_SYS_DDR_LL0) = DDR0_BASEADDR >> 20;
        REG32(CFG_SYS_DDR_HL0) = (DDR0_BASEADDR+DDR0_SIZE) >> 20;

        REG32(SYS_DDR_RESET) &= ~0x01;

        REG32(CFG_SYS_BUS_CFG) = 1;
        REG32(CFG_SYS_BUS_CFG) = 0;
        dummy = *(volatile UINT32 *)CFG_SYS_DDR_CFG;

        p_mem = (UINT32 *)DDR0_BASEADDR;
    }

    pDDR = (PDENALIDDRC)p_controller_regs;

    //  PHY PLL setup
    pDDR->pll_fbdiv = 19; //x4
    pDDR->pll_odiv1 = 2;
    pDDR->pll_odiv2 = 1;
    pDDR->pll_pcsdiv = 1; // 0x05
    //	    pDDR->pri_dq_slew_rate = 1; // 0x21
    //	    pDDR->pri_ca_slew_rate = 1;
#ifdef INT_PLL_CLOCK
    pDDR->pll_refclk_sel = 2;
#endif
    dummy = 100;
    while (dummy--)
        ; // 100 cycles delay

    // Take PLL out of reset
    pDDR->pll_reset_n = 1;

    dummy = 2000;
    while (dummy--)
        ; // 1000 cycles delay

    // switch clock source to DFI
    REG32(SYS_DDR_CLK_CNTRL) &= (ddr_controller_id) ? ~(2 << 4) : ~2;

    for (i = 0; i < sizeof(DdrControllerParams); i++)
    {
        if (i >= 0xac && i <= 0xb7)
            continue;
        if (i >= 0x1cd && i <= 0x1dc)
            continue;

        p_controller_regs[i] = p_denali_params[i];
    }

    pDDR->BSTLEN = 3; // set burst size to 8
    pDDR->START = 1; // initilize DDR

    while ((pDDR->INT_STATUS_8_15 & 1) == 0)
        ; // wait till DRAM initialization is complete

    // For Write leveling override (this is per slice)
    // WMDLL<#> (0x00-0; 0x0040 for WMDLL0, 0x0050 for WMDLL1, etc.)
    // MDLL_PH_SEL - MDLL phase select; sweep through code to set phase
    // MDLL_PH_OVERRIDE set 1 to override
    pDDR->pri_use_reg = 1;
    for (i = 0; i < 5; i++)
        p_ddrphy_slice_ctrl[i].WMDLL0 = 8 | WritePhaseDelays[ddr_controller_id][i];
    pDDR->pri_use_reg = 0;

    do
    {
        SysDDRTraining(pDDR);
        *p_mem = 0x12345678;
        *(p_mem+1) = 0x87654321;
        *(p_mem+2) = 0x11223344;
        try_count++;
    } while (*p_mem != 0x12345678 || *(p_mem+1) != 0x87654321 || *(p_mem+2) != 0x11223344);

    return try_count;
}
