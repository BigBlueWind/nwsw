//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL pciedrv.c
 *
 * @brief PCIe (PCI express) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.18 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "pciedrv.h"
#include "syscore.h"
#include "hal.h"
#include <string.h>
#include "devinfo.h"
#include "i2cdrv.h"
#include "icdrv.h"
#include "oxpcie952.h"
#include "mmumgr.h"

// This is a special setup where 2 T4K (1 as RC and 1 as EP) are connected back to back over SerDes 0 (SATA cables)
// Because of the limitation of the Denali IP, when in RC, we can only access the memory range 0x0000_0000 to 0xFA80_0000
// which is the beginning of the PCIe window, so we *cannot* access CRAM or IRAM (note that this does not apply when using a 3rd party
// controller as RC).
// Since a lot of memory is Read/Write protected (like JRAM used for Linux HI-Vectors or 0x0000_0000 -> 0x07FF_FFFF used by Linux etc....)
// When in this mode, we want to use the DDR1 NcNb heap as the memory region to read/write.
// To access this memory, the "Address Base for master mem address translation" (pLocMgtRegs->MemBase) needs to be set at:
// 0x0FA0_0000 - DDR1NcNbBuffer (i.e. PCIE_RC_MEM - DDR1NcNbBuffer) so that address that go out on the PCIe bus
// start @ DDR1NcNbBuffer - Of course this assumes that the same buffer is allocated on the EP and RC (which it should if nothing else is started)
// Also, the EP BAR[0] and RC BAR[0] need to be configured with DDR1NcNbBuffer to enable access to that region by the
// other side.
//#define T4K_BACK2BACK

PCIEDRVCTX  PCIeDrvCtx;

/** @brief This function returns pointer to the driver context
    @return [PCIEDRVCTX*] */

INLINE PCIEDRVCTX * PCIeDrvGetCtx (void)
{
    return &PCIeDrvCtx;
}

UINT32 PCIeReadReg(VUINT32 * pReg)
{
    VUINT32 tmp;

    // We need to read registers twice

    tmp = *pReg;
    tmp = *pReg;

    return tmp;
}

INLINE void PCIeWriteReg(VUINT32 * pReg, UINT32 nVal)
{
    *pReg = nVal;
}

#if 0
/** @brief this function initializes SerDes-0 which is used
           by PCIe HW component, this block is configured to operate
           on 2.5 Gbps

    @param SrdsRate [in] - should be specified to operate at 2.5 GBPS
    @param SrdsMode [in] - for PCIe should be configured for PCIe mode

    @return [MXRC] an error code */

MXRC Serdes0Init_(UINT32 SrdsRate, UINT32 SrdsMode)
{
    // reset Serdes 0
    REG32(RAD_SRDS0_CNTRL) = 0xF1;

    REG32(RAD_SRDS0_INIT_CFG_R0) = 0x000204DF;
    REG32(RAD_SRDS0_INIT_CFG_R1) = 0xAA800000;
    REG32(RAD_SRDS0_INIT_CFG_R2) = (SrdsMode<<18) | 0x0000002A;

    // to wait 10ms
    MxDelayTicks(150000 * 10);

    //====================================
    // SERDES-0
    //====================================

    // Program SRDS0/SRIO0 clocks
    REG32(RAD_SRDS0_CONT_CLK_DIV_CNTRL) = 0x82;
    MxDelayTicks(150000 * 3);
    //!!! TxIf Clock derived from SERDES
    REG32(RAD_SRDS0_TXIF_CLK_DIV_CNTRL) = 0x82;
    MxDelayTicks(150000 * 3);
    REG32(RAD_SRDS0_CNTRL) = 0; // Release serdes0 reset
    MxDelayTicks(150000 * 3);

    // Serdes TX Config settings
    REG32(RAD_CFG_SRDS_MMD) = 0x1;
    REG32(RAD_CFG_SRDS_LANE) = 0xF;

    // PLL clock ready assertion - section 3.2 of bringup doc
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC000) = 0x78DF; // 0x70DF
    MxDelayTicks(150000 * 3);
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC004) = 0xC440;
    MxDelayTicks(150000 * 3);
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC008) = 0x0362;
    MxDelayTicks(150000 * 3);
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC004) = 0xC640;
    MxDelayTicks(150000 * 3);
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8034) = 0x007E; // RX CTLE Control
    MxDelayTicks(150000 * 3);
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8038) = 0x0C83; // TX Config 1
    MxDelayTicks(150000 * 3);
    // Preemp 0xc1 default, 0x9b works better, also 0xac
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8040) = 0x00ac; // TX Pre-emphasis
    MxDelayTicks(150000 * 3);
    // CTLE peak-pole
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8030) = 0x00ff;
    MxDelayTicks(150000 * 3);
    // Serdes DLL settings - Need this for 3.125GHz
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC008) = 0x262;
    MxDelayTicks(150000 * 3);
    // Phase Interpolator
    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC078) = 0xC2F0;
    MxDelayTicks(150000 * 3);

#if 0
    REG32(RAD_SRDS0_PCS_MISC_CFG) = 0xB117; // Invert PCS RX CLK
    MxDelayTicks(150000 * 3);
#endif

    return MX_OK;
}

/** @brief this function initializes SerDes-0 which is used
           by PCIe HW component, this block is configured to operate
           on 2.5 Gbps

    @param SrdsRate [in] - should be specified to operate at 2.5 GBPS
    @param SrdsMode [in] - for PCIe should be configured for PCIe mode
    @param nInvPcsRxClk [in] - if TRUE, the SERDES PCS RX CLK will be inverted (it's needed for SRIO internal loopback)

    @return [MXRC] an error code */

MXRC Serdes0InitX1(UINT32 SrdsRate, UINT32 SrdsMode, UINT32 nInvPcsRxClk)
{
    // reset Serdes 0
    REG32(RAD_SRDS0_CNTRL) = 0xF1;
    MxDelayTicks(150000 * 30);

    REG32(RAD_SRDS0_INIT_CFG_R0) = 0x000204DF;
    REG32(RAD_SRDS0_INIT_CFG_R1) = 0xAA800000;
    REG32(RAD_SRDS0_INIT_CFG_R2) = (SrdsMode<<18) | 0x0000002A;
    /*
        REG32(RAD_SRDS0_INIT_CFG_R0) = 0x00020253;
    //    REG32(RAD_SRDS0_INIT_CFG_R1) = 0xAA94A528;
    //    REG32(RAD_SRDS0_INIT_CFG_R2) = 0x0010002A;
        // Half rate
        REG32(RAD_SRDS0_INIT_CFG_R1) = 0x55400000;
        REG32(RAD_SRDS0_INIT_CFG_R2) = 0x00300015;//0x00100015;
        // Quarter rate
        //REG32(RAD_SRDS0_INIT_CFG_R1) = 0xAA800000;
    */
    //    REG32(RAD_SRDS0_CNTRL) = 0;
        MxDelayTicks(150000 * 10);

        //====================================
        // SERDES-0
        //====================================

        // Program SRDS0/SRIO0 clocks
        REG32(RAD_SRDS0_CONT_CLK_DIV_CNTRL) = 0x82;
        MxDelayTicks(150000 * 3);
        //!!! TxIf Clock derived from SERDES
        REG32(RAD_SRDS0_TXIF_CLK_DIV_CNTRL) = 0x82;
        MxDelayTicks(150000 * 3);
        REG32(RAD_SRDS0_CNTRL) = 0; // Release serdes0 reset
        MxDelayTicks(150000 * 3);

        // Serdes TX Config settings
        REG32(RAD_CFG_SRDS_MMD) = 0x1;
        REG32(RAD_CFG_SRDS_LANE) = 0xF;

        // PLL clock ready assertion - section 3.2 of bringup doc
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC000) = 0x78DF; // 0x78DF
        MxDelayTicks(150000 * 3);
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC004) = 0xC440;
        MxDelayTicks(150000 * 3);
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC008) = 0x0362;
        MxDelayTicks(150000 * 3);
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC004) = 0xC640;
        MxDelayTicks(150000 * 3);
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8034) = 0x007E; // RX CTLE Control
        MxDelayTicks(150000 * 3);
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8038) = 0x0C83; // TX Config 1
        MxDelayTicks(150000 * 3);
    //    REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x803C) = 0x0021; // TX Config 2
    //    SysDelay(3);

        // Preemp 0xc1 default, 0x9b works better, also 0xac
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8040) = 0x00ac; // TX Pre-emphasis
        MxDelayTicks(150000 * 3);

        SerdesGemWriteReg(RAD_APB_SERDES0_4CH_BASEADDR+0x8044, 0x2828); // Duty Cycle control - TX clock select0
        MxDelayTicks(150000 * 3);
        SerdesGemWriteReg(RAD_APB_SERDES0_4CH_BASEADDR+0x8048, 0x2828); // TX Clock Select 1 0x2f00
        MxDelayTicks(150000 * 3);

        // CTLE peak-pole
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8030) = 0x00ff;
        MxDelayTicks(150000 * 3);

        // Serdes DLL settings - Need this for 3.125GHz
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC008) = 0x262;
        MxDelayTicks(150000 * 3);

        // Phase Interpolator
        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC078) = 0xC2F0;
        MxDelayTicks(150000 * 3);

    if (nInvPcsRxClk)
    {
        REG32(RAD_CFG_SRDS_MMD) = 0x3;
        REG32(RAD_CFG_SRDS_LANE) = 0xF;

        REG32(RAD_SRDS0_PCS_MISC_CFG) = 0xB117; // Invert PCS RX CLK
        MxDelayTicks(3 * 150000);
    }

    return MX_OK;
}
#endif

#define PCA9555_OUTPUT_PORT_0			2 
#define PCA9555_POLARITY_PORT_0		4
#define PCA9555_CONFIG_PORT_0			6
#define PCA9555_I2C_ADDRESS(x)			(0x40 | (x<<1))
#define PCA9555_OUTPUT_PORT_CMD(x)	(PCA9555_OUTPUT_PORT_0   | x)
#define PCA9555_POLARITY_PORT_CMD(x)	(PCA9555_POLARITY_PORT_0 | x)
#define PCA9555_CONFIG_PORT_CMD(x)	(PCA9555_CONFIG_PORT_0   | x)

#define IO_BIT_PCI_WAKE_N(x)			(x<<7)   /** @brief I/O 0.7 on T4K EVM MST_U2 low cost EVM MST_U1 (same address) */
#define IO_BIT_PCI_PRSNT_N(x)			(x<<7)   /** @brief I/O 0.7 & 1.7 on T4K EVM MST_U2 low cost EVM MST_U1 (same address) */
#define EXT_GPIO_OUTPUT				0
#define EXT_GPIO_INPUT					1

/* Common SFP extended I/O defines */
#define IO_BIT_TFAULT(x)			(x<<0)	/** @brief I/O x.0 */
#define IO_BIT_TDIS(x)				(x<<1)   /** @brief I/O x.1 */
#define IO_BIT_MOD_DEF2(x)			(x<<2)   /** @brief I/O x.2 */
#define IO_BIT_MOD_DEF1(x)			(x<<3)   /** @brief I/O x.3 */
#define IO_BIT_MOD_DEF0(x)			(x<<4)   /** @brief I/O x.4 */
#define IO_BIT_RATE_SEL(x)			(x<<5)   /** @brief I/O x.5 */
#define IO_BIT_LOS(x)				(x<<6)   /** @brief I/O x.6 */
#define IO_BIT_RSVD_7(x)			(x<<7)   /** @brief I/O x.7 */

/* Extended I/O chip and port defines */
#define MST_U2				1
#define MST_U3				2
#define MST_U4				3
#define MST_UX_PORT0		0
#define MST_UX_PORT1		1

MXRC SetExtGpioRegister(UINT32 ChipID, UINT8 Register, UINT8 Data)
{	
	UINT8  WriteData = Data;
	UINT32 BytesWritten;

	BytesWritten = I2CDrvWriteBytes(PCA9555_I2C_ADDRESS(ChipID),
			                        Register,
			                        &WriteData,
			                        1 
			                       );
	if (BytesWritten == 1)
	  	return MX_OK;
	else
	  	return PCIE_RC_EXTGPIO_WR_ERROR;
}

MXRC InitExtGpioDevice(UINT32 ChipID,
		                 UINT8  Port,
		                 UINT8  InitialOutputValue, 
		                 UINT8  PolaritySelect,
		                 UINT8  InputOutputSelect 
		                )
{
	MXRC rc;

	rc = SetExtGpioRegister(ChipID, 
							PCA9555_OUTPUT_PORT_CMD(Port),
							InitialOutputValue);
	if (rc != MX_OK)
	{
		uart_printf ("Output port error (0x%x)\n", rc);
		return rc;
	}

	rc = SetExtGpioRegister(ChipID, 
							PCA9555_POLARITY_PORT_CMD(Port),
							PolaritySelect);
	if (rc != MX_OK)
	{
		uart_printf ("Polarity port error (0x%x)\n", rc);
		return rc;
	}

	rc = SetExtGpioRegister(ChipID, 
							PCA9555_CONFIG_PORT_CMD(Port),
							InputOutputSelect);
	if (rc != MX_OK)
	{
		uart_printf ("Config port error (0x%x)\n", rc);
		return rc;
	}

	return rc;
}

/** @brief This function release the PCIe reset line (GPIO_8) and Wake line (ExtGpio) on SerDes 2
    @return [MXRC] an error code */

MXRC PCIeDrvReleasePCIeSlot(UINT32 Mode)
{
	MXRC rc = MX_OK;
	UINT8 DefaultInitialOutput = 0
					| IO_BIT_TDIS(1)     // 1: SFP Disabled (set to 0 to enable)
					| IO_BIT_MOD_DEF2(1) // 1: SFP I2C EEPROM SCL high
					| IO_BIT_MOD_DEF1(1) // 1: SFP I2C EEPROM SDA high
					| IO_BIT_RATE_SEL(0) // 1: SFP Low speed rate select (ignored if single rate)
					;
	UINT8 DefaultPolaritySelect = 0;  // All bits normal polarity (not inverted)
	UINT8 DefaultInputOutput = 0
					| IO_BIT_TFAULT(EXT_GPIO_INPUT)	  /* I/O x.0 */
					| IO_BIT_TDIS(EXT_GPIO_OUTPUT)     /* I/O x.1 */
					| IO_BIT_MOD_DEF2(EXT_GPIO_OUTPUT) /* I/O x.2 */
					| IO_BIT_MOD_DEF1(EXT_GPIO_OUTPUT) /* I/O x.3 */
					| IO_BIT_MOD_DEF0(EXT_GPIO_INPUT)  /* I/O x.4 */
					| IO_BIT_RATE_SEL(EXT_GPIO_OUTPUT) /* I/O x.5 */
					| IO_BIT_LOS(EXT_GPIO_INPUT)       /* I/O x.6 */
					//| IO_BIT_RSVD_7(EXT_GPIO_OUTPUT)   /* I/O x.7 */
					;

	rc = I2CInitHw();
	if (FAILED(rc))
	{
		uart_printf ("I2C initialization error, rc=0x%x\r\n", rc);
		return rc;
	}

	// Reserved/Unused
	rc |= InitExtGpioDevice
		(MST_U4,
		MST_UX_PORT0,
		DefaultInitialOutput,
		DefaultPolaritySelect,
		DefaultInputOutput | IO_BIT_RSVD_7(EXT_GPIO_INPUT)
		);
	rc |= InitExtGpioDevice
		(MST_U4,
		MST_UX_PORT1,
		DefaultInitialOutput,
		DefaultPolaritySelect,
		DefaultInputOutput | IO_BIT_RSVD_7(EXT_GPIO_INPUT)
		);
	rc |= InitExtGpioDevice
		(MST_U2,
		MST_UX_PORT1,
		DefaultInitialOutput,
		DefaultPolaritySelect,
		DefaultInputOutput | IO_BIT_RSVD_7(EXT_GPIO_INPUT)
		);

	if(Mode == PCIE_DRV_OPT_RC)
	{// RC

		// WAKE#
		rc |= InitExtGpioDevice(
			MST_U2,
			MST_UX_PORT0,
			DefaultInitialOutput,
			DefaultPolaritySelect,
			DefaultInputOutput | IO_BIT_PCI_WAKE_N(EXT_GPIO_INPUT)
			);
#if 0
		// PRSNT1# - Hot plug detect (A1)
		rc |= InitExtGpioDevice(
			MST_U3,
			MST_UX_PORT0,
			DefaultInitialOutput,
			DefaultPolaritySelect,
			DefaultInputOutput | IO_BIT_PCI_PRSNT_N(EXT_GPIO_INPUT)
			);

		// PRSNT2# - Hot plug detect (B31)
		rc |= InitExtGpioDevice(
			MST_U3,
			MST_UX_PORT1,
			DefaultInitialOutput,
			DefaultPolaritySelect,
			DefaultInputOutput | IO_BIT_PCI_PRSNT_N(EXT_GPIO_INPUT)
			);
#endif
	}
	else
	{// EP
		// WAKE# - Drive low
		rc |= InitExtGpioDevice(
			MST_U2,
			MST_UX_PORT0,
			DefaultInitialOutput | IO_BIT_PCI_WAKE_N(0),
			DefaultPolaritySelect,
			DefaultInputOutput | IO_BIT_PCI_WAKE_N(EXT_GPIO_OUTPUT)
			);
#if 0
		// PRSNT1# - Hot plug detect (A1) - Drive low
		rc |= InitExtGpioDevice(
			MST_U3,
			MST_UX_PORT0,
			DefaultInitialOutput | IO_BIT_PCI_PRSNT_N(0),
			DefaultPolaritySelect,
			DefaultInputOutput | IO_BIT_PCI_PRSNT_N(EXT_GPIO_OUTPUT)
			);

		// PRSNT2# - Hot plug detect (B31) - Drive low
		rc |= InitExtGpioDevice(
			MST_U3,
			MST_UX_PORT1,
			DefaultInitialOutput | IO_BIT_PCI_PRSNT_N(0),
			DefaultPolaritySelect,
			DefaultInputOutput | IO_BIT_PCI_PRSNT_N(EXT_GPIO_OUTPUT)
			);
#endif
	}

	if(rc != MX_OK)
		return rc;

	// GPIO8 controls PCIe module reset - set it high to pull it out of reset
	REG32(GPIO_OE_REG) |= (1<<8);
	REG32(GPIO_OUTPUT_REG) |= (1<<8);
	SysDelay(10);

	return MX_OK;
}

/** @brief This function just reads all configuration register (FNum=0)
    @param pRegs [out] - the pointer to the local storage
    @return [MXRC] an error code */

MXRC PCIeDrvReadRCConfRegs(PPCIERCREGS pRegs)
{
    UINT32 Val = REG32(RAD_CFG_PCIE_FNUM);

    // Select RC registers
    REG32(RAD_CFG_PCIE_FNUM) = 0;

    PCIeDrvReadCtrlReg (0, (PUINT32)pRegs, sizeof (*pRegs)/sizeof (UINT32));

    // Select Root Complex registers
    REG32(RAD_CFG_PCIE_FNUM) = Val;

    return MX_OK;
}

/** @brief This function just reads all local management register (FNum=0x100)
    @param pRegs [out] - the pointer to the local storage
    @return [MXRC] an error code */

MXRC PCIeDrvReadLocMgtRegs(PPCIELOCREGS pRegs)
{
    UINT32 Val = REG32(RAD_CFG_PCIE_FNUM);

    // Select Local Management registers
    REG32(RAD_CFG_PCIE_FNUM) = 0x100;

    PCIeDrvReadCtrlReg (0, (UINT32*)pRegs, sizeof (*pRegs)/sizeof (UINT32));

    // Select Root Complex registers
    REG32(RAD_CFG_PCIE_FNUM) = Val;

    return MX_OK;
}

/** @brief This function reads Endpoint (external device) config registers to PCIEREG struct when in RC mode.
    @param pRegs [out] - the pointer to the local storage struct
    @return [MXRC] an error code */

MXRC PCIeDrvReadEPConfRegs(PPCIEREGS pRegs, VUINT32 * pSrc)
{
	UINT32 i;
	UINT32 *pDst = (UINT32 *)pRegs;

	for (i = 0; i < sizeof(PCIEREGS)/sizeof(UINT32); i++)
	{
		*pDst++ = PCIeReadReg (pSrc ++);
	}

	return MX_OK;
}

/** @brief This function prints the Endpoint (external device) config register
    @param [] - none
    @return [] none */

void PCIeDrvPrintRegs(PPCI_DEV pDev)
{
	UINT32 i, Val32, Old32, NextCap;
#ifdef SHOW_CAPABILITIES
	VUINT32 *CapPtr;
#endif

	Val32 = PCIeReadReg(PCIEREG(PCI_FUNC_VIDDID));
	uart_printf("Vendor ID: 0x%X and Device ID: 0x%X\n", (U16)(Val32), (U16)(Val32>>16));
	Val32 = PCIeReadReg(PCIEREG(PCI_FUNC_CMDSTAT));
	uart_printf("Status: 0x%X and Command: 0x%X\n", (U16)(Val32>>16), (U16)(Val32));
	Val32 = PCIeReadReg(PCIEREG(PCI_FUNC_REVIDCLASS));
	uart_printf("Class: 0x%X, Sub-class: 0x%X, Prog IF: 0x%x, RevID: 0x%X\n", Val32 >> 24, (Val32 >> 16) & 0xFF, (Val32 >> 8) & 0xFF, (Val32 & 0xFF));
	Val32 = PCIeReadReg(PCIEREG(PCI_FUNC_BIST));
	uart_printf("HdrType: 0x%X, DevType: 0x%X, CacheLineSize: 0x%X\n", (Val32 >> 16) & 0x7F, (Val32 >> 23) & 0x1, Val32 & 0xFF);

	SysDelay(10);

	for (i=0; i<6; i++)
	{
		Old32 = PCIeReadReg(PCIEREG(PCI_FUNC_BAR0 + i));

		// PCI 2.2 spec: the PCI I/O space size for a target device is determined by sending FFFFFFFFh to the base address register 
		// then reading back to see which bits are hardwired to zero. These bits determine the size of the address area you need to 
		// reserve in the memory map. In other words, the binary weighted value of the least significant bit set to one in the BAR field
		// indicates the required amount of space (e.g. bit 20 is the first set to '1' -> 1Mb is required)
		PCIeWriteReg(PCIEREG(PCI_FUNC_BAR0 + i) , 0xFFFFFFFF);
		Val32 = PCIeReadReg(PCIEREG(PCI_FUNC_BAR0 + i));

		if ((Val32 != 0xFFFFFFFF) && (Val32 != 0))
		{// The fact that any bits could be changed to one indicates that the BAR is implemented
			uart_printf("BAR[%d]: read: %x\n", i, Val32);
			Val32 &= ~0xF;			// Clear lower 4 bits
			Val32 ^= 0xFFFFFFFF;		// Check which bits are hardwired to 0 -> that's the size
			uart_printf(" - size: 0x%X\n", Val32+1);
			pDev->BarSize[i] = Val32+1;

			if (Old32 & 0x01)
				uart_printf(" - IO Space\n");
			else
			{
				uart_printf(" - MEM Space\n");
				pDev->BarIsMem[i] = TRUE;

				if (Old32 & 0x08)
					uart_printf(" - Prefetchable\n");
				uart_printf(" - %s addr space\n", (Old32 & 0x06)?("64-bit"):("32-bit"));
			}
		}

		//Restore BAR
//		PCIeWriteReg(PCIEREG(PCI_FUNC_BAR0 + i), Old32);
		SysDelay(10);
	}

	Val32 = PCIeReadReg(PCIEREG(PCI_FUNC_SUBVIDDID));
	uart_printf("SubVendorID:0x%X, SubSysID:0x%X\n", Val32 & 0xFFFF, (Val32 >> 16) & 0xFFFF);
	Val32 = PCIeReadReg(PCIEREG(PCI_FUNC_CAPPTR));
	NextCap = Val32 & 0xFF;
	uart_printf("CapPointer:0x%X\n", NextCap);
	SysDelay(10);

#ifdef SHOW_CAPABILITIES
	while(NextCap)
	{
		CapPtr = (VUINT32 *)(PCIE_RC_CFG + NextCap);
	 	uart_printf("Checking Capability Registers at 0x%08p\n", CapPtr);

		Old32 = Val32 = PCIeReadReg(CapPtr);

		// Check capability ID field
		if(CAPABILITY_ID(Val32) == CAPABILITY_ID_POWER_MGT)
		{
			uart_printf("  Power Management\n");
			pDev->PmCap = NextCap;

			uart_printf("  Control: 0x%X, CapPointer: 0x%X, CapID: 0x%X\n", CAPABILITY_CONTROL(Val32), CAPABILITY_NEXT(Val32), CAPABILITY_ID(Val32));
			Val32 = CAPABILITY_CONTROL(Val32);
			uart_printf("     VersionID: 0x%X\n", Val32 & 0x7);
			Val32 = PCIeReadReg(++CapPtr);
			uart_printf("  NoSoftReset: %d, Current Power State: D%d\n", (Val32 >> 3) & 0x1, Val32 & 0x3);
		}
		else	 if(CAPABILITY_ID(Val32) == CAPABILITY_ID_MSI)
		{
			uart_printf("  MSI (Message Signaled Interrupts)\n");
			pDev->MSICap = NextCap;

			uart_printf("  Control: 0x%X, CapPointer: 0x%X, CapID: 0x%X\n", CAPABILITY_CONTROL(Val32), CAPABILITY_NEXT(Val32), CAPABILITY_ID(Val32));
			Val32 = CAPABILITY_CONTROL(Val32);
			uart_printf("     PerVectorMaskCapable: %d\n     CapableOf64Bits: %d\n     MultipleMessageEnable: %d\n     MultipleMessageCapable: %d\n     MSIEnable: %d\n", (Val32 >> 8) & 1,
																																	(Val32 >> 7) & 1,
																																	1 << ((Val32 >> 4) & 0x7),
																																	1 << ((Val32 >> 1) & 0x7),
																																	Val32 & 1);
			if(Val32 & (1<<7))
			{// 64 bits addressing capable
				uart_printf("  Lower 32 bits address: 0x%X\n", PCIeReadReg(++CapPtr));
				uart_printf("  Upper 32 bits address: 0x%X\n", PCIeReadReg(++CapPtr));
				Val32 = PCIeReadReg(++CapPtr);
				uart_printf("  Message Data: 0x%X\n", Val32 & 0xFFFF);

				uart_printf("  MaskBits: 0x%X\n", PCIeReadReg(++CapPtr));
				uart_printf("  PendingBits: 0x%X\n", PCIeReadReg(++CapPtr));
			}
			else
			{// 32 bits addressing capable
				uart_printf("  32 bits address: 0x%X\n", PCIeReadReg(++CapPtr));
				Val32 = PCIeReadReg(++CapPtr);
				uart_printf("  Message Data: 0x%X\n", Val32 & 0xFFFF);
			}
		}
		else	 if(CAPABILITY_ID(Val32) == CAPABILITY_ID_MSI_X)
		{
			uart_printf("  MSI-X\n");
			pDev->MSIXCap = NextCap;

			uart_printf("  Control: 0x%X, CapPointer: 0x%X, CapID: 0x%X\n", CAPABILITY_CONTROL(Val32), CAPABILITY_NEXT(Val32), CAPABILITY_ID(Val32));
			Val32 = CAPABILITY_CONTROL(Val32);
			uart_printf("     MSIXEnable: %d, Function mask: %d, Table size: %d\n", (Val32 >> 15) & 1, (Val32 >> 14) & 1, (Val32 & 0xFFF)+1);
			Val32 = PCIeReadReg(++CapPtr);
			uart_printf("  tab offset: %d, tab BIR: %d\n", (Val32 >> 3), Val32 & 0x7);
			Val32 = PCIeReadReg(++CapPtr);
			uart_printf("  PBA offset: %d, PBA BIR: %d\n", (Val32 >> 3), Val32 & 0x7);
		}
		else
		{
			if(CAPABILITY_ID(Val32) == CAPABILITY_ID_PCI_EXPRESS)
			{
				uart_printf("  PCI Express register set\n");
				pDev->PCIeCap = NextCap;
			}
			else
				uart_printf("  Unknown\n");

			uart_printf("  CapPointer: 0x%X, CapID: 0x%X\n", CAPABILITY_NEXT(Val32), CAPABILITY_ID(Val32));
		}

		// Get Next Capability pointer
		NextCap = CAPABILITY_NEXT(Old32);
	}
#endif

	Val32 = PCIeReadReg(PCIEREG(PCI_FUNC_INTLINEPIN));
	Old32 = ((Val32 >> 8) & 0xFF);
	if(Old32 == 0)
		uart_printf("IRQ Pin: No INT pin used, IRQ Line: IRQ%d\n", (UINT8)(Val32 & 0xFF));
	else
		uart_printf("IRQ Pin: INT%c, IRQ Line: IRQ%d\n", 'A' + (Old32-1), (UINT8)(Val32 & 0xFF));
	SysDelay(50);
}

void PCIeDrvPrintLocalCfgSpace(void)
{
	PCIEDRVCTX * pCtx = PCIeDrvGetCtx ();
	PCIEREGS CfgSpace;
	UINT32 i;

	if (pCtx ->Initialized == 0)
		return;

	REG32 (RAD_CFG_PCIE_FNUM) = 0x0;

	// Read own config space (we should be in EP mode here)
	PCIeDrvReadEPConfRegs(&CfgSpace, (VUINT32 *)(pCtx->CtrlReg));

	// Print out some info
	uart_printf("Vendor ID: 0x%X and Device ID: 0x%X\n", (U16)(CfgSpace.VDid), (U16)(CfgSpace.VDid>>16));
	uart_printf("Status: 0x%X and Command: 0x%X\n", (U16)(CfgSpace.CmdStat>>16), (U16)(CfgSpace.CmdStat));
	for (i=0; i<6; i++)
		uart_printf("BAR[%d]: 0x%X\n", i, CfgSpace.Bar[i]);
	uart_printf("SubVendorID:0x%X, SubSysID:0x%X\n", CfgSpace.SubVid & 0xFFFF, (CfgSpace.SubVid >> 16) & 0xFFFF);
	i = ((CfgSpace.IntCtrl >> 8) & 0xFF);
	if(i == 0)
		uart_printf("IRQ Pin: No INT pin used, IRQ Line: IRQ%d\n", (UINT8)(CfgSpace.IntCtrl & 0xFF));
	else
		uart_printf("IRQ Pin: INT%c, IRQ Line: IRQ%d\n", 'A' + (i-1), (UINT8)(CfgSpace.IntCtrl & 0xFF));
}

void PCIeDrvSetIrqCb(BOOL IsMIRQ, PCIEIRQCB irq_cb, void *pData)
{
	PCIEDRVCTX * pCtx = PCIeDrvGetCtx();
	IRQTYPE irq;

	irq = ARM_INT_disable();

	if(!IsMIRQ)
	{
		pCtx->IrqCB = irq_cb;
		pCtx->pIrqData= pData;
	}
	else
	{
		pCtx->MIrqCB = irq_cb;
		pCtx->pMIrqData= pData;
	}

	ARM_INT_restore(irq);
}

void PCIeDrvIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
	PCIEDRVCTX * pCtx = (PCIEDRVCTX *)pParam;

	*pIntClearReg = nIrqID;

	if (pCtx->IrqCB != NULL)
		pCtx->IrqCB (nIrqID, pCtx->pIrqData);
}

void PCIeDrvMIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
	PCIEDRVCTX * pCtx = (PCIEDRVCTX *)pParam;

	if (pCtx->MIrqCB != NULL)
		pCtx->MIrqCB (nIrqID, pCtx->pMIrqData);

	// Clear MIRQ
	REG32(PCIE_MIRQ) = 1;

	*pIntClearReg = nIrqID;
}

MXRC PCIeDrvRootComplexInit(PCIEDRVCTX * pCtx)
{
	PCIELOCREGS *pLocMgtRegs = (PCIELOCREGS *)pCtx->CtrlReg;
	PCIERCREGS *pRcRegs = (PCIERCREGS *)pCtx->CtrlReg;

#ifdef T4K_BACK2BACK
	UINT32* pData = (UINT32*)pCtx->SysBaseMem;
	UINT32 nVal;

	// Initialise some dummy values to be recognized on the other side
	for(nVal=0; nVal < 10; nVal++)
		pData[nVal] = (nVal << 16) | 0xDCBA;
#endif

	// Select Local Management registers
	REG32(RAD_CFG_PCIE_FNUM) = 0x100;

	// to configure BAR_0-1 for 4KB memory range
	PCIeWriteReg(&pLocMgtRegs->RootBarCfg, (1 << 4) | 0);

	// Set the Address translation
	PCIeWriteReg(&pLocMgtRegs->MemBase, pCtx->BaseMem >> 20);
	PCIeWriteReg(&pLocMgtRegs->IOBase, pCtx->BaseIO >> 20);
	PCIeWriteReg(&pLocMgtRegs->CfgBase, pCtx->BaseCfg >> 20);

//	PCIeWriteReg(&pLocMgtRegs->LOSTimeoutLim, 0);
//	PCIeWriteReg(&pLocMgtRegs->CompTimeoutLim, (UINT32)-1); // nanosec

	// Modify the RX credits
//	PCIeWriteReg(&pLocMgtRegs->RxCreditLim0, (498 | 4<<12 | 4 <<20));
//	PCIeWriteReg(&pLocMgtRegs->RxCreditLim1, 4);

	// Select Root Complex registers
	REG32(RAD_CFG_PCIE_FNUM) = 0;

	// For incoming requests (in conjunction with the aperture set with RootBarCfg)
	PCIeWriteReg(&pRcRegs->BAR[0], pCtx->SysBaseMem & ~0xFFF);
	PCIeWriteReg(&pRcRegs->BAR[1], 0);

	return MX_OK;
}

MXRC PCIeDrvEndPointInit(PCIEDRVCTX * pCtx)
{
	PCIELOCREGS *pLocMgtRegs = (PCIELOCREGS *)pCtx->CtrlReg;
	PCIEREGS * pFunRegs = (PCIEREGS *)pCtx->CtrlReg;
	UINT32 nVal;

#ifdef T4K_BACK2BACK
	UINT32* pData = (UINT32*)pCtx->SysBaseMem;
	UINT32* pData2 = (UINT32*)(pCtx->SysBaseMem + (0x8000/sizeof(UINT32)));

	// Initialise some dummy values to be recognized on the other side
	for(nVal=0; nVal < 10; nVal++)
	{
		pData[nVal] = (nVal << 16) | 0xABCD;		// 1st 32K Window
		pData2[nVal] = (nVal << 16) | 0xAABB;	// 2nd 32K Window
	}
#endif

	// select Local Management registers
	REG32(RAD_CFG_PCIE_FNUM) = 0x100;

	// configure BAR_0-1 memory range
	// 0x0: 0x8000 (32KB)
	// 0x1: 0x10000 (64KB)
	// 0x2: 0x20000 (128KB)
	// 0x3: 0x40000 (256KB)
	// 0x4: 0x80000 (512KB)
	// 0x5: 0x100000 (1MB)
	// 0x6: 0x200000 (2MB)
	// 0x7: 0x400000 (4MB)
	// 0x8: 0x800000 (8MB)
	// 0x9: 0x1000000 (16MB)
	// 0xA: 0x2000000 (32MB)
	// 0xB: 0x4000000 (64MB)
	// 0xC: 0x8000000 (128MB)
	// 0xD: 0x10000000 (256MB)
	// 0xE: 0x20000000 (512MB)
	// 0xF: 0x40000000 (1GB) <- Default value (All BARs enabled)
	PCIeWriteReg(&pLocMgtRegs->PhyBarCfg[0], (1 << 4) | 0);

	// For bus master mode
	PCIeWriteReg(&pLocMgtRegs->MemBase, pCtx->BaseMem >> 20);
	PCIeWriteReg(&pLocMgtRegs->IOBase, pCtx->BaseIO >> 20);
	PCIeWriteReg(&pLocMgtRegs->CfgBase, pCtx->BaseCfg >> 20);

	// Modify the RX credits
//	PCIeWriteReg(&pLocMgtRegs->RxCreditLim0, (498 | 4<<12 | 4 <<20));
//	PCIeWriteReg(&pLocMgtRegs->RxCreditLim1, 4);

	// Set Vendor & Subvendor ID
	PCIeWriteReg(&pLocMgtRegs->VendorID, (PCI_SUBVENDOR_ID_MINDSPEED << 16) | PCI_VENDOR_ID_MINDSPEED);

	REG32 (RAD_CFG_PCIE_FNUM) = 0x000;

	// Set Device ID
	nVal = PCIeReadReg(&pFunRegs->VDid);
	nVal &= ~0xFFFF0000;
	nVal |= ((UINT32)PCI_DEVICE_ID_MINDSPEED << 16);
	PCIeWriteReg(&pFunRegs->VDid, nVal);

	// Set Class/Sub Class - leave RevID
	nVal = PCIeReadReg(&pFunRegs->RevIDClassID);
	nVal &= ~0xFFFFFF00;
	nVal |= ((UINT32)PCI_CLASS_MULTIMEDIA << 24) | ((UINT32)PCI_SUBCLASS_OTHER << 16);
	PCIeWriteReg(&pFunRegs->RevIDClassID, nVal);

	// INT Pin
	pCtx->INTPin = PCIE_INTPIN_INTA;
	nVal = PCIeReadReg(&pFunRegs->IntCtrl);
	nVal &= ~0x0000FF00;
	nVal |= (pCtx->INTPin << 8);
	PCIeWriteReg(&pFunRegs->IntCtrl, nVal);

#if 0
	// This needs to be done by the RC after enumeration

	PCIeWriteReg(&pFunRegs->Bar[0], CRAM_BASEADDR);
	PCIeWriteReg(&pFunRegs->Bar[1], 0);

	nVal = PCIeReadReg(&pFunRegs->CmdStat);
	nVal |= //PCI_STATUS_CAPABILITIES_LIST | 
			//PCI_COMMAND_SERR |
			PCI_COMMAND_MASTER |
			PCI_COMMAND_MEMORY |
			PCI_COMMAND_IO	;
	PCIeWriteReg(&pFunRegs->CmdStat, nVal);
#endif

	return MX_OK;
}


/******************************************************************************
    @brief  Find base address of specified capabilitiy resisters. 

    @param  CfgBase     PCI(e) configuration base address
    @param  CapID       Capability ID (8 bits)

   @return Base address of capability registers or 0 if not found.
******************************************************************************/    
UINT32 PCIeDrvDetectCap(UINT32 CfgBase, UINT8 CapID)
{
    UINT32  val, 
            addr;

    addr = REG32(CfgBase + (13 << 2)) & 0xFC;
    addr += CfgBase;
    while(addr != CfgBase)
    {
        val = REG32(addr);
        if( (val & 0xff) == CapID)            
            return addr;    // capability foung    
        addr = CfgBase + ((val >> 8) & 0xFC);  // 8 bits      
    };
    
    return 0;           // no capability found
}

/**
    Configure MSI for EP
*/    
UINT32 PCIeDrvCfgMSI(UINT32 cfg_base)
{
    PMSI_CAP_REGISTERS pMSICap = (PMSI_CAP_REGISTERS)PCIeDrvDetectCap(cfg_base, CAPABILITY_ID_MSI);
    UINT32 val;
    
    if(pMSICap == 0)
    {
        uart_printf("PCIDRV :No MSI capability detected in config.space 0x%08X\n", cfg_base);
        return PCIE_MSI_CAP_ERROR;
    }
    // disable INTx    
    REG32(cfg_base + 0x4) |= (1 << 10);    // disable INTx    
        
    // configure EP MSI           
    pMSICap->HighAddr = 0;
    pMSICap->LowAddr = RAD_CFG_RC_MSI_CATCHER;
    pMSICap->Data = 0xf000;  
    pMSICap->MaskBits = 0;
    val = (pMSICap->Header >> 17) & 0x7; // requested number of MSI
    if(val > 5) val = 5;                // max 32 interrupt
    pMSICap->Header = (val << 20) | (1 << 16);   // enable multiple inteppupts and global

    uart_printf ("[MSI]");
    
    return MX_OK;
}


/** @brief This function initializes and configures driver,
    it should be called first before usage of any driver API

    @param pCfg[in] - configuration parameters
    @return [MXRC] an error code */
MXRC PCIeDrvInit(PCIECFG * pCfg)
{
	PCIEDRVCTX * pCtx;
	MXRC rc = MX_OK;
#ifdef T4K_BACK2BACK
	UINT32 BufferBase;
#define M			(1024*1024)
#define MASK_1M		(M-1)
#endif

	_ASSERT_PTR (pCfg);

	pCtx = PCIeDrvGetCtx ();

	_ASSERT_PTR (pCtx);

	if ((pCfg->BaseMem & 0xFFF) != 0)
		return PCIE_RC_BASEMEM_NOT_ALIGNED;

	memset (pCtx, 0, sizeof (*pCtx));

	pCtx->CtrlReg		= RAD_APB_PCIE_SYS_BASEADDR;
	pCtx->Mode			= pCfg->Mode;
	pCtx->Lanes			= pCfg->Lanes;
	pCtx->BaseMem		= PCIE_RC_MEM;
	pCtx->BaseIO		= PCIE_RC_IO;
	pCtx->BaseCfg		= PCIE_RC_CFG;
	pCtx->SysBaseMem	= pCfg->BaseMem;
	pCtx->IrqCB			= pCfg->IrqCB;
	pCtx->pIrqData		= pCfg->pIrqData;
	pCtx->MIrqCB		= pCfg->MIrqCB;
	pCtx->pMIrqData		= pCfg->pMIrqData;
    pCtx->MSIEnabled    = pCfg->MSICfg;

#ifdef T4K_BACK2BACK
	BufferBase= (UINT32) MxHeapAlloc (DDR1NcnbHeap, 2*M);	// Alloc 2Meg - because the Base Mem pointer *must* be 1Mb aligned
	if(BufferBase == 0)
		uart_printf("Alloc fail\n");
	else if(BufferBase & ~MASK_1M)
	{// Buffer is not aligned (which is expected)
		BufferBase += 1*M;
		BufferBase &= ~MASK_1M;
	}

	pCtx->SysBaseMem	= BufferBase;
	pCtx->BaseMem		-= BufferBase;
#endif

	// Release PCIe clk reset
	REG32(RAD_PCIE_RESET) = 0x0;	// 0xFE800090

	// Release global reset AXIs PCIe clk
	REG32(RAD_AXI_RESET_0) = 0x0;	// 0xFE800010
	REG32(RAD_AXI_RESET_1) = 0x0;	// 0xFE800014

	// For SerDes 2 (i.e. PCIe slot is used) release the Reset/Wake signals
	if(pCfg->SerDesID == PCIE_DRV_OPT_SRDS_2)
	{
		if (FAILED(rc = PCIeDrvReleasePCIeSlot(pCfg->Mode)))
			return rc;
	}

	// Config the SerDes
	if (pCfg->SerDesID == PCIE_DRV_OPT_SRDS_0)
	{
		uart_printf("PCI-E: SerDes(%d)\n", pCfg->SerDesID);
		REG32(RAD_CFG_SRDS_MUX) = 0x3;

		if (DevInfoIsT4Kx6 () || DevInfoIsT4Kx7 ())
		{
			Serdes0Init(SRDSRATE_2500, SRDSMODE_PCIE);
		}
		else
		{
			Serdes0Init(SRDSRATE_2500, SRDSMODE_PCIE);
			//Serdes0InitX1(SRDSRATE_2500, SRDSMODE_PCIE, 0);
		}
	}
	else if (pCfg->SerDesID == PCIE_DRV_OPT_SRDS_2)
	{
		uart_printf("PCI-E: SerDes(%d)\n", pCfg->SerDesID);
		REG32(RAD_CFG_SRDS_MUX) = 0x4;
		SysDelay(10);
		Serdes2Init(SRDSRATE_2500, SRDSMODE_PCIE);
		SysDelay(10);
	}
	else
	{
		uart_printf("PCI-E: Invalid Serdes ID (%d)\n", pCfg->SerDesID);
		return PCIE_RC_SERDES_ID_ERROR;
	}

	// Wait 10ms
	MxDelayTicks(150000 * 100);

	// Set L0S Timeout register value to 0 - to avoid a bug
	REG32(PCIE_LOS_TIMEOUT_REG) = 0x0;

	REG32(RAD_CFG_PCIE_CFG) = 	(0 << 7) | // Request PM transition L23 ready state, end station only 
								(1 << 6) | // PCIe target max burst (1: 16 beats (128 bytes)) 
								(0 << 5) | // Hot Reset In bit (0: no reset)
								(1 << 4) | // HAL Master Enable Client Tag: 1 (iDsable Internal Tag management) 
								((pCfg->Lanes >> 1) << 2) | // Lanes 0b10: x4, 0b01: x2, 0b00: x1
								(pCfg->Mode << 1) | // Root complex (1) / EP (0)
								(0 << 0); // GEN1

	if (pCfg->Mode == PCIE_DRV_OPT_EP)
		rc = PCIeDrvEndPointInit(pCtx);
	else
		rc = PCIeDrvRootComplexInit(pCtx);

	if (SUCCESSFUL(rc))
	{
		if (FAILED(rc = MxSetIntHandler (HAL_IRQ_PCIE_0, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_PCIE, PCIeDrvIrqHandler, pCtx)))
			return rc;
		if (FAILED(rc = MxSetIntHandler (HAL_IRQ_PCIE_1, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_PCIE, PCIeDrvIrqHandler, pCtx)))
			return rc;
		if (FAILED(rc = MxSetIntHandler (HAL_IRQ_PCIE_2, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_PCIE, PCIeDrvIrqHandler, pCtx)))
			return rc;
		if (FAILED(rc = MxSetIntHandler (HAL_IRQ_PCIE_3, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_PCIE, PCIeDrvIrqHandler, pCtx)))
			return rc;

		if (FAILED(rc = MxEnableInt (HAL_IRQ_PCIE_0)))
			return rc;
		if (FAILED(rc = MxEnableInt (HAL_IRQ_PCIE_1)))
			return rc;
		if (FAILED(rc = MxEnableInt (HAL_IRQ_PCIE_2)))
			return rc;
		if (FAILED(rc = MxEnableInt (HAL_IRQ_PCIE_3)))
			return rc;

        if (pCfg->MIrqCB != NULL)
        {
    		// For MIRQ (message IRQ) - MSI or Vendor Defined Messages
    		if (FAILED(rc = MxSetIntHandler (HAL_IRQ_PCIE_4, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_PCIE, PCIeDrvMIrqHandler, pCtx)))
    			return rc;
    		if (FAILED(rc = MxEnableInt (HAL_IRQ_PCIE_4)))
    			return rc;
        }

		// Config was successful
		pCtx->Initialized = 1;
		pCtx->MasterCpu = MxGetCpuID ();
	}

	return rc;
}

/** @brief This function just disables PCIe driver and set HW block under reset
    @return [MX_OK] */

MXRC PCIeDrvShutdown (void)
{
    PCIEDRVCTX * pCtx = PCIeDrvGetCtx ();

    if (pCtx->Initialized == 0 || pCtx->MasterCpu != MxGetCpuID ())
        return MX_OK;

    pCtx->Initialized = 0;

    MxDisableInt (HAL_IRQ_PCIE_0);
    MxDisableInt (HAL_IRQ_PCIE_1);
    MxDisableInt (HAL_IRQ_PCIE_2);
    MxDisableInt (HAL_IRQ_PCIE_3);
    MxDisableInt (HAL_IRQ_PCIE_4);

    REG32(RAD_PCIE_RESET) = 0x1;

    return MX_OK;
}

/** @brief  This function just stops device and does not free any resources
 *          like allocated memory. It's needed to save allocated resources for
 *          the futher debugging and do not touch potentially corrupted heaps

 @return [MX_OK] */

MXRC PCIeDrvGenShedding (void)
{
    PCIEDRVCTX * pCtx = PCIeDrvGetCtx ();

    if (pCtx->Initialized == 0 || pCtx->MasterCpu != MxGetCpuID ())
        return MX_OK;

    pCtx->Initialized = 0;

    MxDisableInt (HAL_IRQ_PCIE_4);
    MxDisableInt (HAL_IRQ_PCIE_3);
    MxDisableInt (HAL_IRQ_PCIE_2);
    MxDisableInt (HAL_IRQ_PCIE_1);
    MxDisableInt (HAL_IRQ_PCIE_0);
    SysDelay(10);

    // Set PCIe clk reset
    //REG32(RAD_PCIE_RESET) = 0x1;	// 0xFE800090
    SysDelay(10);

    return MX_OK;
}

/** @brief This function is designed to be used with EndPoint PCIe configuration
           to generate IRQ-A, B, C, D

    @param pCtx     [in] - the driver context
    @param IrqNum   [in] - IRQ number
    @param bWaitAck [in] - the wait the ack

    @return [UINT32] error code */

UINT32 PCIeDrvGenIrq(LPVOID pCtx, UINT32 irqNum, BOOL bWaitAck)
{
	UINT32 nTime = GetTIMETICK ();
	UINT32 INTx = 1 << irqNum;

	if (PCieDrvGetLinkStatus() != PCIE_LINK_DL_COMPLETE)
		return PCIE_RC_LINK_IS_NOT_SET;

	// BIT#6 - IRQ function is disabled
	if ((REG32(RAD_CFG_PCIE_STATUS) & (1 << 6)))
		return PCIE_RC_IRQ_IS_DISABLED;

	// Set INT pending
	REG32 (RAD_CFG_PCIE_PEND_IRQ) = 1;

	// Set PCIe Interrupt
	REG32 (RAD_CFG_PCIE_SET_IRQ) |= INTx;

	while (bWaitAck)
	{// Wait for INT ACK - this is set by the controller when the Assert INTx message is sent (not when the interrupt is acknowledged by the RC)
		if (REG32 (RAD_CFG_PCIE_IRQ_ACK) != 0)
		{
			// Clear INT ACK
			REG32(PCIE_INT_ACK) = 1;
			break;
		}

		if ((GetTIMETICK () - nTime) >= PCIE_DRV_IRQ_ACK_TIMEOUT)
		{// IRQ wasn't sent by controller (??)
			// Clear Interrupt
			REG32 (RAD_CFG_PCIE_SET_IRQ) &= ~INTx;

			// Clear INT pending
			REG32 (RAD_CFG_PCIE_PEND_IRQ) = 0;

			uart_printf("ERROR: No INT_ACK on interrupt set\n");

			return PCIE_EP_IRQ_GEN_ERROR;
		}
	}

	return MX_OK;
}


/** @brief This function is designed to be used with EndPoint PCIe configuration
           to clear IRQ-A, B, C, D

    @param pCtx     [in] - the driver context
    @param IrqNum   [in] - IRQ number
    @param bWaitAck [in] - the wait the ack

    @return [UINT32] error code */
    
UINT32 PCIeDrvClearIrq(LPVOID pCtx, UINT32 irqNum, BOOL bWaitAck)
{
	UINT32 nTime = GetTIMETICK ();
	UINT32 INTx = 1 << irqNum;

	if (PCieDrvGetLinkStatus() != PCIE_LINK_DL_COMPLETE)
		return PCIE_RC_LINK_IS_NOT_SET;

	// BIT#6 - IRQ function is disabled
	if ((REG32(RAD_CFG_PCIE_STATUS) & (1 << 6)))
		return PCIE_RC_IRQ_IS_DISABLED;

	// Clear PCIe Interrupt
	REG32 (RAD_CFG_PCIE_SET_IRQ) &= ~INTx;

	while (bWaitAck)
	{// Wait for INT ACK - this is set by the controller when the Deassert INTx message is sent
		if (REG32 (RAD_CFG_PCIE_IRQ_ACK) != 0)
		{
			// Clear INT ACK
			REG32(PCIE_INT_ACK) = 1;
			break;
		}

		if ((GetTIMETICK () - nTime) >= PCIE_DRV_IRQ_ACK_TIMEOUT)
		{// IRQ wasn't sent by controller (??)
			uart_printf("ERROR: No INT_ACK on interrupt clear\n");

			// Clear INT pending
			REG32 (RAD_CFG_PCIE_PEND_IRQ) = 0;

			return PCIE_EP_IRQ_GEN_ERROR;
		}
	}

	// Clear INT pending
	REG32 (RAD_CFG_PCIE_PEND_IRQ) = 0;

	return MX_OK;
}

UINT32 PCieDrvGetLinkStatus(void)
{
    return REG32(RAD_CFG_PCIE_STATUS) & 0x3;
}

/** @brief This function sets connectionm between RC and EP
    @param timeout [in] - timeout in ms of waiting
    @return [MXRC] an error code of status */

MXRC PCIeDrvConnect(UINT32 timeout)
{
    PPCIERCREGS pRcRegs = (PPCIERCREGS)RAD_APB_PCIE_SYS_BASEADDR;
    UINT32 nTime = GetTIMETICK ();

    if (PCIeDrvIsRC())
    {
        pRcRegs->LinkCtrlStatReg |= (1 << 5); // Retrain link, doc, page 122
    }

	// to wait  1ms
    MxDelayTicks(150000);

    while (timeout)
    {
        if (PCieDrvGetLinkStatus() == PCIE_LINK_DL_COMPLETE)
            break;

        if ((GetTIMETICK () - nTime) >= timeout)
            break;
    }

    return (PCieDrvGetLinkStatus() == PCIE_LINK_DL_COMPLETE) ? MX_OK : PCIE_RC_LINK_IS_NOT_SET;
}

#ifdef Korean_BMT_RF_PCIE
MXRC PCIeSrdsDrvConnect(UINT32 timeout, UINT32 SerDesID)
{
    PPCIERCREGS pRcRegs = (PPCIERCREGS)RAD_APB_PCIE_SYS_BASEADDR;
    UINT32 nTime = GetTIMETICK ();

    if (PCIeDrvIsRC())
    {
        pRcRegs->LinkCtrlStatReg |= (1 << 5); // Retrain link, doc, page 122
    }

	// to wait  1ms
    MxDelayTicks(300000);

    while (timeout)
    {
        if (PCieDrvGetLinkStatus() == PCIE_LINK_DL_COMPLETE)
            break;

        if ((GetTIMETICK () - nTime) >= timeout)
            break;
		if (SerDesID == PCIE_DRV_OPT_SRDS_0)
			Serdes0Init(SRDSRATE_2500, SRDSMODE_PCIE);
		else if (SerDesID == PCIE_DRV_OPT_SRDS_2)
			Serdes2Init(SRDSRATE_2500, SRDSMODE_PCIE);

    	MxDelayTicks(30000);
    }

    return (PCieDrvGetLinkStatus() == PCIE_LINK_DL_COMPLETE) ? MX_OK : PCIE_RC_LINK_IS_NOT_SET;
}
#endif

/** @brief This function select special bit in EVM FPGA
           device to activate master access

    @return [MXRC] an error code */

MXRC PCIeDrvSelectMaster (void)
{
    PCIEDRVCTX * pCtx = PCIeDrvGetCtx ();
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);

    if (pCtx->Initialized == 0)
        return PCIE_RC_DEV_NOT_INITIALIZED;

    // enable master access
    REG32(PCIE_RC_CFG + 4) = 6;

    if (pCtx->MSIEnabled)
        rc = PCIeDrvCfgMSI(PCIE_RC_CFG);

    return rc;
}

/** @brief This function selects PCI FunctionNumber,
    @Note: Please use 0x100 to select local management

    @param nFNum [in] - the Function number

    @return [MXRC] an error code */

MXRC PCIeDrvSelectFNum (UINT32 nFNum)
{
    REG32(RAD_CFG_PCIE_FNUM) = nFNum;
    return MX_OK;
}

/** @brief This function reads PCIe registers selected by PCIeDrvSelectFNum,

    @param nIndex  [in]  - register index in the HW registers list
    @param pRegOut [out] - the pointer to registers structure of:
                                PCIELOCREGS, PCIERCREGS or PCIEREGS

    @param nRegNum [in] - the number of required registers

    @return [MXRC] an error code */

MXRC PCIeDrvReadCtrlReg (UINT32 nIndex, PUINT32 pRegOut, UINT32 nRegNum)
{
    VUINT32 * pIn;

    pIn = (VUINT32 *)PCIeDrvGetCtx()->CtrlReg;

    if (pIn == NULL)
        return PCIE_RC_DEV_NOT_INITIALIZED;

    pIn += nIndex;

    while (nRegNum --)
    {
        (*pRegOut++) = PCIeReadReg (pIn ++);
    }

    return MX_OK;
}

/** @brief This function writes PCIe registers selected by PCIeDrvSelectFNum,

    @param nIndex  [in]  - register index in the HW registers list
    @param pReg [in] - the pointer to registers structure of:
                                PCIELOCREGS, PCIERCREGS or PCIEREGS

    @param nRegNum [in] - the number of required registers

    @return [MXRC] an error code */

MXRC PCIeDrvWriteCtrlReg (UINT32 nIndex, PUINT32 pReg, UINT32 nRegNum)
{
    VUINT32 * pOut;

    pOut = (VUINT32 *)PCIeDrvGetCtx()->CtrlReg;

    if (pOut == NULL)
        return PCIE_RC_DEV_NOT_INITIALIZED;

    pOut += nIndex;

    while (nRegNum --)
    {
        PCIeWriteReg (pOut++, *pReg++);
    }

    return MX_OK;
}

UINT32 PCIeDrvReadMemory (UINT32 addr, PUINT32 pOut, UINT32 nWords)
{
	VUINT8 * pPCIeMem;
	UINT32 StartAddr;

	pPCIeMem = (VUINT8 *)PCIE_RC_MEM;

	pPCIeMem += addr;
	StartAddr = (UINT32)pPCIeMem;

	while (nWords --)
	{
		(*pOut++) = PCIeReadReg ((VUINT32*)pPCIeMem);
		pPCIeMem += sizeof(UINT32);
	}

	return StartAddr;
}

UINT32 PCIeDrvWriteMemory (UINT32 addr, PUINT32 pIn, UINT32 nWords)
{
	VUINT8 * pPCIeMem;
	UINT32 StartAddr;

	pPCIeMem = (VUINT8 *)PCIE_RC_MEM;

	pPCIeMem += addr;
	StartAddr = (UINT32)pPCIeMem;

	while (nWords --)
	{
		PCIeWriteReg ((VUINT32*)pPCIeMem, *pIn++);
		pPCIeMem += sizeof(UINT32);
	}

	return StartAddr;
}

void PCIeDrvPrintUARTReg(void)
{
	VUINT32 * pData;
	UINT32 i, BaseMem = PCIeDrvGetCtx()->BaseMem;
	UINT32 UARTRegOffset[12] = {   0x00,	// Baseline 550 registers 0x00 -> 0x03
								0x01,	// Baseline 550 registers 0x04 -> 0x07
								0x20,	// Extended 550 registers 0x80 -> 0x83
								0x21,	// Extended 550 registers 0x84 -> 0x87
								0x24,	// Baseline 650 registers 0x90 -> 0x93
								0x25,	// Baseline 650 registers 0x94 -> 0x97
								0x28,	// Baseline 950 registers 0xA0 -> 0xA3
								0x29,	// Baseline 950 registers 0xA4 -> 0xA7
								0x30,	// Baseline Index Control registers 0xC0 -> 0xC3
								0x31,	// Baseline Index Control registers 0xC4 -> 0xC7
								0x32,	// Baseline Index Control registers 0xC8 -> 0xCB
								0x33,	// Baseline Index Control registers 0xCC -> 0xCF
							};

	uart_printf("\nUART Reg (%x):\n", PCIE_RC_MEM+0x1000);
	for(i=0; i < 12; i++)
	{
		pData = (VUINT32 *)(PCIE_RC_MEM+0x1000);
		pData+=UARTRegOffset[i];
		uart_printf(" [0x%x / 0x%x]: 0x%x\n", pData, (UINT32)pData-BaseMem, PCIeReadReg (pData));
	}
	SysDelay(20);
}

UINT32 GetPCIeIntStatus(UINT32 Limit)
{
	volatile GicDistRegs * pDistr = (GicDistRegs *)A9IC_DISTR_BASEADDR;
	UINT32 Val = pDistr->SPIStatus[2];
	UINT32 Mask = 1 << (70 - 64);	// 64 = Base SPI of the register [2] / 70 = IRQ_PCIE0
	UINT32 i, rc = 0;

	for(i=0; i<Limit; i++)
	{
		uart_printf("   INT%c = %s\n", 'A'+i, (Val & Mask) ? "High":"Low");
		if(Val & Mask)
			rc++;
		Mask <<= 1;
	}

	return rc;
}

#ifdef T4K_BACK2BACK

#define T4KBACKTOBACK_ISR_REG				0xFF		// Random offset in the EP memory
#define T4KBACKTOBACK_ISR_PENDING			0xAC009900	// Random value - indicates that an Int is pending

#define T4KBACKTOBACK_RC_INT_CLEAR_REG	0xF0		// Random offset in the RC&EP memory
#define T4KBACKTOBACK_RC_MAGICNUMBER	0xCB64BEEF	// Random value - indicates that RC is ready

#define VERIFY_INT_LINE_GETS_CLEARED

static void T4KBackToBackISR(UINT32 irq, void *pData)
{
	PPCIEDRVCTX pCtx = (PPCIEDRVCTX)pData;
	VUINT32* pLocalMem = (VUINT32*)pCtx->SysBaseMem;
	VUINT32* pRemoteMem = (VUINT32*) PCIE_RC_MEM;

	uart_printf("%dx", ++pCtx->TestData);

	GetPCIeIntStatus(1);

#ifdef VERIFY_INT_LINE_GETS_CLEARED
	pLocalMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] = 0;	// INT is high
	pRemoteMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] = 0;
#endif

	// Check that the remote side has Int pending
	if(pRemoteMem[T4KBACKTOBACK_ISR_REG] != T4KBACKTOBACK_ISR_PENDING)
	{// Error
		uart_printf("  RC - Remote ISR invalid 0x%x", pRemoteMem[T4KBACKTOBACK_ISR_REG]);
	}

	// Clear ISR reg
	pRemoteMem[T4KBACKTOBACK_ISR_REG] = 0;
}

static void T4KBackToBackIntTest(PPCIEDRVCTX pCtx)
{
	VUINT32* pLocalMem = (VUINT32*)pCtx->SysBaseMem;
	VUINT32* pRemoteMem = (VUINT32*) PCIE_RC_MEM;
	UINT32 loopctr = 0;
	UINT32 nTime;
	UINT32 IntPin = pCtx->INTPin-1;	// Value is 0 based

	// Init
	pCtx->TestData = 0;

	if(PCIeDrvIsRC())
	{// RC
		// Set up callback
		PCIeDrvSetIrqCb(FALSE, T4KBackToBackISR, pCtx);

		pLocalMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] = T4KBACKTOBACK_RC_MAGICNUMBER;

		uart_printf("T4K BackToBack Interrupt test - RC Configured\n");

#ifdef VERIFY_INT_LINE_GETS_CLEARED
		pRemoteMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] = T4KBACKTOBACK_RC_MAGICNUMBER;

		nTime = 0;
		loopctr = 0;
		while(1)
		{
			if(pLocalMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] == 0)
			{// An int has been processed
				if(GetPCIeIntStatus(1))
				{// INT line still up
					if(loopctr++ == 5)
					{// Stuck high for 5*100 = 500ms
						uart_printf("   RC - INT line stuck high -> ISR = 0x%x\n", pRemoteMem[T4KBACKTOBACK_ISR_REG]);
						return;
					}
				}
				else
				{// INT line has been cleared
					pLocalMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] = T4KBACKTOBACK_RC_MAGICNUMBER;
					pRemoteMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] = T4KBACKTOBACK_RC_MAGICNUMBER;
					nTime = 0;
					loopctr = 0;
				}
			}
			else
			{
				if(nTime++ == 100)
				{// No activity for 100*100 = 10s
					return;
				}
			}

			SysDelay(100);
		}
#endif

		return;
	}
	else
		uart_printf("T4K BackToBack Interrupt test - EP Configured\n");

	while(1)
	{
		uart_printf("%d.", ++pCtx->TestData);

		// Test
		pLocalMem[0] = pCtx->TestData;

		// Show pending INT
		pLocalMem[T4KBACKTOBACK_ISR_REG] = T4KBACKTOBACK_ISR_PENDING;

		// Gen int
		if(PCIeDrvGenIrq(pCtx, IntPin, TRUE) != MX_OK)
			return;

		// Wait until the RC has acknowledged the int before moving on
		nTime = GetTIMETICK ();
		while(pLocalMem[T4KBACKTOBACK_ISR_REG] == T4KBACKTOBACK_ISR_PENDING)
		{
			if ((GetTIMETICK () - nTime) >= PCIE_DRV_IRQ_ACK_TIMEOUT)
			{
				uart_printf("   EP - RC hasn't acknowledged/processed the INT\n");
	#if 1
//				PCIeDrvClearIrq(pCtx, IntPin, TRUE);
				return;
	#else
				uart_printf("      waiting...\n");
				SysDelay(5000);
	#endif
			}
		}

		// Clear IRQ
		if(PCIeDrvClearIrq(pCtx, IntPin, TRUE) != MX_OK)
			return;

		// Wait before running next loop
		SysDelay(1000);

#ifdef VERIFY_INT_LINE_GETS_CLEARED
		if(pLocalMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] != T4KBACKTOBACK_RC_MAGICNUMBER)
		{// INT line hasn't been cleared on the RC side - abort
			uart_printf("   EP - RC INT line still up\n");
			return;
		}
#endif
	}
}

static void T4KBackToBackPollTest(PPCIEDRVCTX pCtx)
{
	VUINT32* pLocalMem = (VUINT32*)pCtx->SysBaseMem;
	VUINT32* pRemoteMem = (VUINT32*) PCIE_RC_MEM;
	UINT32 nTime;

	// Init
	pCtx->TestData = 0;

	if(PCIeDrvIsRC())
	{// RC
		uart_printf("T4K BackToBack Polling test - RC Configured\n");

		pLocalMem[T4KBACKTOBACK_RC_INT_CLEAR_REG] = T4KBACKTOBACK_RC_MAGICNUMBER;
		pRemoteMem[T4KBACKTOBACK_ISR_REG] = 0;

		while(pRemoteMem[T4KBACKTOBACK_ISR_REG] != T4KBACKTOBACK_ISR_PENDING)
		{
			uart_printf("EP not up and ready - waiting....\n");

			// Wait 3 sec and retry
			SysDelay(3000);
		}

		while(1)
		{
			if((++pCtx->TestData % 500) == 0)
				uart_printf("%dx", pCtx->TestData);

			// Test
			pLocalMem[0] = pCtx->TestData;

			// Wait until the EP has raised the int before moving on
			nTime = GetTIMETICK ();
			while(pRemoteMem[T4KBACKTOBACK_ISR_REG] != T4KBACKTOBACK_ISR_PENDING)
			{
				if ((GetTIMETICK () - nTime) >= PCIE_DRV_IRQ_ACK_TIMEOUT)
				{
					uart_printf("   RC - EP hasn't set the INT\n");
					return;
				}
			}

			// Fake INT servicing by moving a bunch of data around
			for(nTime=0; nTime < 1000; nTime++)
			{
				pLocalMem[0x100+nTime] = pRemoteMem[nTime];
				pRemoteMem[0x100+nTime] = 0xdead0000 + nTime + pCtx->TestData;
			}

			// Clear ISR
			pRemoteMem[T4KBACKTOBACK_ISR_REG] = 0;
		}
	}
	else
	{// EP
		uart_printf("T4K BackToBack Polling test - EP Configured\n");

		while(1)
		{
			if((++pCtx->TestData % 500) == 0)
				uart_printf("%d.", pCtx->TestData);

			// Test
			pLocalMem[0] = pCtx->TestData;

			// Show pending INT
			pLocalMem[T4KBACKTOBACK_ISR_REG] = T4KBACKTOBACK_ISR_PENDING;

			// Wait until the RC has acknowledged the int before moving on
			nTime = GetTIMETICK ();
			while(pLocalMem[T4KBACKTOBACK_ISR_REG] == T4KBACKTOBACK_ISR_PENDING)
			{
				if ((GetTIMETICK () - nTime) >= PCIE_DRV_IRQ_ACK_TIMEOUT)
				{
					uart_printf("   EP - RC hasn't acknowledged/processed the INT\n");
					return;
				}
			}

			if(pCtx->TestData == 100000)
				break;
		}
	}
}
#endif

MXRC PCIeDrvDebug (UINT32 Type)
{
	VUINT32 * pData;
	UINT32 i, rc;
	PPCIEDRVCTX pCtx = PCIeDrvGetCtx();
	UINT32 BaseMem = pCtx->BaseMem;
	PCIELOCREGS lm_regs;
	PCIERCREGS rc_regs;

#define TYPE_RC						0
#define TYPE_EP						1
#define TYPE_SHOW_EPCFGSPACE		2
#define TYPE_SHOW_ERR_STATUS		3
#define TYPE_GEN_IRQ_A				4
#define TYPE_GEN_IRQ_B				5
#define TYPE_GEN_IRQ_C				6
#define TYPE_GEN_IRQ_D				7
#define TYPE_OXPCIE_INT_TEST		8
#define TYPE_T4K_INT_TEST			9
#define TYPE_T4K_POLL_TEST			10
#define TYPE_FORCE_RETRAIN			11
#define TYPE_GET_INT_STATUS		12

	uart_printf("\n");

	if(pCtx->Initialized == 0)
	{
		uart_printf("PCIe not initialized\n");
		return PCIE_RC_DEV_NOT_INITIALIZED;
	}

	switch(Type)
	{
		case TYPE_RC:
		case TYPE_EP:
			if(PCIeDrvIsRC())
			{
				uart_printf("Cfg (%x):\n", PCIE_RC_CFG);
				pData = (VUINT32 *)PCIE_RC_CFG;
				i=5;
				while (i--)
				{
					uart_printf(" [0x%x]: 0x%x\n", pData, PCIeReadReg (pData));
					pData++;
				}
				SysDelay(10);
			}

			uart_printf("Mem (%x):\n", PCIE_RC_MEM);
			pData = (VUINT32 *)PCIE_RC_MEM;
			i=5;
			while (i--)
			{
				uart_printf(" [0x%x / 0x%x]: 0x%x\n", pData, (UINT32)pData-BaseMem, PCIeReadReg (pData));
				pData++;
			}
			SysDelay(10);

#if 0
			uart_printf("IO (%x):\n", PCIE_RC_IO);
			pData = (VUINT32 *)PCIE_RC_IO;
			i=5;
			while (i--)
			{
				uart_printf(" [0x%x]: 0x%x\n", pData, PCIeReadReg (pData));
				pData++;
			}
			SysDelay(10);
#endif

			uart_printf("Sys (i.e. local) Mem (%x):\n", pCtx->SysBaseMem);
			pData = (VUINT32 *)pCtx->SysBaseMem;
			i=5;
			while (i--)
			{
				uart_printf(" [0x%x]: 0x%x\n", pData, PCIeReadReg (pData));
				pData++;
			}
			SysDelay(10);

			if((pCtx->PciDev.VendorID == PCI_VENDOR_ID_OXPCIE952) && (pCtx->PciDev.DeviceID == PCI_DEVICE_ID_OXPCIE952))
				PCIeDrvPrintUARTReg();
			return MX_OK;

		case TYPE_SHOW_EPCFGSPACE:		// Print out the Cfg Space when in EP mode
			if(PCIeDrvIsRC())
				uart_printf("Can't show EP Cfg Space when in RC mode\n");
			else
				PCIeDrvPrintLocalCfgSpace();
			return MX_OK;

		case TYPE_SHOW_ERR_STATUS:		// Error/Status Register
			uart_printf("PCIe Status       [0x%08x] = 0x%08x\n", RAD_CFG_PCIE_STATUS, REG32(RAD_CFG_PCIE_STATUS));

			uart_printf("PCIe Write Err    [0x%08x] = 0x%08x\n", RAD_CFG_PCIE_WRITE_ERR, REG32(RAD_CFG_PCIE_WRITE_ERR));
			uart_printf("PCIe Read Err     [0x%08x] = 0x%08x\n", RAD_CFG_PCIE_READ_ERR, REG32(RAD_CFG_PCIE_READ_ERR));

			uart_printf("PCIe Set IRQ      [0x%08x] = 0x%08x\n", RAD_CFG_PCIE_SET_IRQ, REG32(RAD_CFG_PCIE_SET_IRQ));
			uart_printf("PCIe IRQ Pending  [0x%08x] = 0x%08x\n", RAD_CFG_PCIE_PEND_IRQ, REG32(RAD_CFG_PCIE_PEND_IRQ));
			uart_printf("PCIe IRQ ACK      [0x%08x] = 0x%08x\n", RAD_CFG_PCIE_IRQ_ACK, REG32(RAD_CFG_PCIE_IRQ_ACK));

			// Read some more controller stats
			PCIeDrvReadLocMgtRegs (&lm_regs);
			uart_printf("\nPhyLayer Conf Reg 0  [LM=0x00] = 0x%08x\n", lm_regs.PhyLayer0);

			uart_printf("Local Error Status   [LM=0x83] = 0x%08x\n", lm_regs.ErrStatus);
			uart_printf("LCRC Error Count     [LM=0x85] = 0x%08x\n", lm_regs.LCRCErrCount);

			uart_printf("Rx Credit Lim 0      [LM=0x03] = 0x%08x\n", lm_regs.RxCreditLim0);
			uart_printf("Rx Credit Lim 1      [LM=0x04] = 0x%08x\n", lm_regs.RxCreditLim1);
			uart_printf("Tx Credit Lim 0      [LM=0x05] = 0x%08x\n", lm_regs.TxCreditLim0);
			uart_printf("Tx Credit Lim 1      [LM=0x06] = 0x%08x\n", lm_regs.TxCreditLim1);
			uart_printf("Tx Credit Update 0   [LM=0x07] = 0x%08x\n", lm_regs.UpdateInterval0);
			uart_printf("Tx Credit Update 1   [LM=0x08] = 0x%08x\n", lm_regs.UpdateInterval1);
			uart_printf("Comp Timeout         [LM=0x0E] = 0x%08x\n", lm_regs.CompTimeoutLim);

			uart_printf("Tx TLP Count         [LM=0x0A] = 0x%08x\n", lm_regs.TxTLPCount);
			uart_printf("Tx TLP Payload Dword [LM=0x0B] = 0x%08x\n", lm_regs.TxTLPPayload);
			uart_printf("Rx TLP Count         [LM=0x0C] = 0x%08x\n", lm_regs.RxTLPCount);
			uart_printf("Rx TLP Payload Dword [LM=0x0D] = 0x%08x\n", lm_regs.RxTLPPayload);

			if(PCIeDrvIsRC())
			{
				// Read RC registers
			        PCIeDrvReadRCConfRegs (&rc_regs);

				uart_printf("\n");

				uart_printf("Link Ctrl & Status   [RC=0x34] = 0x%08x\n", rc_regs.LinkCtrlStatReg);

				uart_printf("Uncor Err Status     [RC=0x41] = 0x%08x\n", rc_regs.UncorrErrStatReg);
				uart_printf("Uncor Err Mask       [RC=0x42] = 0x%08x\n", rc_regs.UncorrErrMaskReg);
				uart_printf("Uncor Err Sev        [RC=0x43] = 0x%08x\n", rc_regs.UncorrSeverityReg);
				uart_printf("Cor Err Status       [RC=0x44] = 0x%08x\n", rc_regs.CorrErrStatReg);
				uart_printf("Cor Err Mask         [RC=0x45] = 0x%08x\n", rc_regs.CorrErrMaskReg);
				uart_printf("Root Err Status      [RC=0x4C] = 0x%08x\n", rc_regs.RootErrStatReg);
			}
			return MX_OK;

		case TYPE_GEN_IRQ_A:
		case TYPE_GEN_IRQ_B:
		case TYPE_GEN_IRQ_C:
		case TYPE_GEN_IRQ_D:
			uart_printf("Generating IRQ on INT%c -> ", 'A' + (Type-TYPE_GEN_IRQ_A));
			rc = PCIeDrvGenIrq(pCtx, (Type-TYPE_GEN_IRQ_A), TRUE);
			uart_printf("Done (%d)\n", rc);
			SysDelay(100);
			uart_printf("Clearing IRQ -> ");
			rc = PCIeDrvClearIrq(pCtx, (Type-TYPE_GEN_IRQ_A), TRUE);
			uart_printf("Done (%d)\n", rc);
			return MX_OK;

		case TYPE_OXPCIE_INT_TEST:
#ifdef T4K_BACK2BACK
			uart_printf("Code is in T4K Back to Back (RC to EP) mode - cannot test OXPCIe952\n");
#else
			OXPCIe952RxIntTest();
#endif
			return MX_OK;

#ifdef T4K_BACK2BACK
		case TYPE_T4K_INT_TEST:
			T4KBackToBackIntTest(pCtx);
			return MX_OK;

		case TYPE_T4K_POLL_TEST:
			T4KBackToBackPollTest(pCtx);
			return MX_OK;
#else
		case TYPE_T4K_INT_TEST:
		case TYPE_T4K_POLL_TEST:
			uart_printf("Code is NOT in T4K Back to Back (RC to EP) mode - cannot run test\n");
			return MX_OK;
#endif

		case TYPE_FORCE_RETRAIN:
			uart_printf("Forcing retrain\n");
			rc = PCIeDrvConnect (PCIE_DRV_CONNECT_TIMEOUT);
			if (FAILED(rc))
				uart_printf ("   Failed, rc=%x\n", rc);
			else
				uart_printf ("   OK\n");
			return MX_OK;

		case TYPE_GET_INT_STATUS:
			GetPCIeIntStatus(4);
			break;

		default:
			uart_printf("   Unknown argument %d\n", Type);
			break;
	}

	return MX_OK;
}


/** @brief This function scans the PCIe bus for Endpoint
    @return [UINT32] number of devices found on the bus */

UINT32 PCIeDrvScan(void)
{
	UINT32 i;
	UINT32 BusNumber = 0, DeviceNumber = 0, FunctionNumber = 0;
	UINT32 Vid,Did, HeaderType;
	UINT32 Temp = 0, DevicesFound = 0;
	PPCIEDRVCTX pCtx = PCIeDrvGetCtx();
	PPCI_DEV pPciDev = &(pCtx->PciDev);
	PPCIEREGS pEpRegs = &(pCtx->EpRegs);

	if(pCtx->Initialized == 0)
		return DevicesFound;

	uart_printf("\nStarting PCIe enumeration\n");

	for (i = 0; i < MAX_PCIE_DEVICES; i++)
	{
		Temp = REG32(RAD_CFG_PCIE_MRC_WC_ID);
		BusNumber		= (Temp >> 8) & 0xFF;
		DeviceNumber	= (Temp >> 3) & 0x1F;
		FunctionNumber	= (Temp >> 0) & 0x7;

		uart_printf("\nChecking EP on BDF %d/%d/%d\n", BusNumber, DeviceNumber, FunctionNumber);

		// Read the EP Cfg Space
		PCIeDrvReadEPConfRegs(pEpRegs, (VUINT32 *)(PCIeDrvGetCtx()->BaseCfg));

		Vid = pEpRegs->VDid & 0xFFFF;
		Did = pEpRegs->VDid >> 16;
		HeaderType = (pEpRegs->BistHdrLatCache >> 16) & 0xFF;

		uart_printf("   VendorID:0x%04X, DeviceID:0x%04X, HeaderType:0x%02X\n", Vid, Did, HeaderType);

		if (!( Vid || Did )) // No more devices implemented, exit
		{
			uart_printf("No more devices found\n");

			// Set bus and function back to 0 for future reads (i.e. back to root bus 0, device 0)
			REG32(RAD_CFG_PCIE_MRC_WC_ID) = 0;
			REG32(RAD_CFG_PCIE_MRC_RC_ID) = 0;
			return DevicesFound;
		}

		// Get common info based on comman data for all headers
		DevicesFound++;
		pPciDev->FuncNum	= FunctionNumber;
		pPciDev->VendorID	= Vid;
		pPciDev->DeviceID	= Did;
		pPciDev->Class		= (pEpRegs->RevIDClassID >> 24) & 0xFF;
		pPciDev->SubClass	= (pEpRegs->RevIDClassID >> 16) & 0xFF;
		pPciDev->RevID		= pEpRegs->RevIDClassID & 0xFF;
		pPciDev->IntPin		= (pEpRegs->IntCtrl >> 8) & 0xFF;

		if ((HeaderType & PCI_HDR_TYPE_MASK) == PCI_HDR_PCI_TO_PCI_BRIDGE)
		{
			PPCIEHDR1REGS pHdr1Regs;

			if ((HeaderType & PCI_HDR_FUNCTION_MASK) == PCI_HDR_SINGLE_FUNCTION_DEVICE)
				uart_printf("   Bridge / Single function device\n");
			else
				uart_printf("   Bridge / Multi function device\n");

			// Setup configuration writes for Root complex to Bridge (i.e. first hop)

			// Cast Pointer for header type 1 (PCI/PCIe Bridge) registers
			pHdr1Regs = (PPCIEHDR1REGS) pEpRegs;
			// Setup bus numbers (0 (this bus) and 1(downstream bus) )

			PCIeWriteReg(PCIEREG(PCI_FUNC_BUS_CONFIG), 
							(pHdr1Regs->BusConfig & 0xFF000000) |		// Keep Secondary Latency Timer
									((BusNumber+1) << 16) |			// Set Subordinate bus to current bus + 1
									((BusNumber+1) << 8) |			// Set Secondary (Downstream) bus to current bus + 1
									((BusNumber) << 0));				// Set Primary (Upstream) bus to current bus

			// Setup for Bus master - bit 2 of command register (Least significant word of Command/Status reg)
			PCIeDrvSetMaster(TRUE);

			// Setup reads for next bus
			BusNumber++;
			DeviceNumber   = 0;
			FunctionNumber = 0;
		}
		else
		{
			if ((HeaderType & PCI_HDR_FUNCTION_MASK) == PCI_HDR_SINGLE_FUNCTION_DEVICE)
				uart_printf("   Non-bridge / Single function device\n");
			else
				uart_printf("   Non-bridge / Multi function device\n");

			// Debug
			PCIeDrvPrintRegs(pPciDev);

			// Enable
			PCIeDrvEnableDevice(pPciDev);

			DeviceNumber++;
			FunctionNumber = 0;

			// We found 1 EP (not a bridge) - no need to scan anymore
			break;
		}

		// Set Completer ID associated with a write and read requests as new bus/device/function
		Temp =	((UINT32)BusNumber << 8) | 		// bits 15:8
				((UINT32)DeviceNumber << 3) |	 	// bits 7:3
				((UINT32)FunctionNumber << 0);		// bits 2:0

		REG32(RAD_CFG_PCIE_MRC_WC_ID) = Temp;
		REG32(RAD_CFG_PCIE_MRC_RC_ID) = Temp;
	}

	// Set bus and function back to 0 for future reads (i.e. back to root bus 0, device 0)
	REG32(RAD_CFG_PCIE_MRC_WC_ID) = 0;
	REG32(RAD_CFG_PCIE_MRC_RC_ID) = 0;

	uart_printf ("\nFound %d EPs", DevicesFound);

	return DevicesFound;
}


/** @brief This function enables/disables PCI bus mastering on an Endpoint

    @param Enable [in]  - PCI bus mastering on/off

    @return [] none */

void PCIeDrvSetMaster(BOOL Enable)
{
	UINT32 Cmd;

	Cmd = PCIeReadReg(PCIEREG(PCI_FUNC_CMDSTAT));
	if (Enable)
		Cmd |=  PCI_COMMAND_MASTER;
	else
		Cmd &= ~PCI_COMMAND_MASTER;

	PCIeWriteReg(PCIEREG(PCI_FUNC_CMDSTAT), Cmd);
}

/** @brief This function initialize device before it's used by a driver

    @param pDev [in]  - Pointer to the PCI_DEV structure

    @return [MXRC] error code */

MXRC PCIeDrvEnableDevice(PPCI_DEV pDev)
{
	UINT32 Cmd, OldCmd;

	uart_printf("\nEnabling PCIe endpoint\n");

#if 0
	U32 OldVal,RegVal;

	RegVal = PCIeReadReg((V32 *)(PCIE_RC_CFG + dev->PmCap + PCI_PM_CTRL)); // Get the current state of the EP
	OldVal = RegVal;
	RegVal &= ~3; // D0 state (active)
	PCIeWriteReg((V32 *)(PCIE_RC_CFG + dev->PmCap + PCI_PM_CTRL), RegVal);

	dev->CurPwrState = RegVal & PCI_PM_CTRL_STATE_MASK; // Current EP power state.
	uart_printf("Power state: D%d -> D%d\n", OldVal, RegVal);
#endif

	Cmd = PCIeReadReg(PCIEREG(PCI_FUNC_CMDSTAT));
	OldCmd = Cmd;
	Cmd &= ~(PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER); // Disable IO and Mem accesses

	PCIeWriteReg(PCIEREG(PCI_FUNC_CMDSTAT), Cmd);

	// Make sure we're configuring the correct EP
	if((pDev->VendorID == PCI_VENDOR_ID_MINDSPEED) && (pDev->DeviceID == PCI_DEVICE_ID_MINDSPEED))
	{// BAR0 only
		uart_printf("  Mindspeed T4K\n");
#ifdef T4K_BACK2BACK
		PCIeWriteReg(PCIEREG(PCI_FUNC_BAR0), PCIeDrvGetCtx()->SysBaseMem);
#else
		PCIeWriteReg(PCIEREG(PCI_FUNC_BAR0), 0);
#endif
		PCIeWriteReg(PCIEREG(PCI_FUNC_BAR1), 0);	// 64 bits BAR - need to clear BAR[1]
	}
	else if((pDev->VendorID == PCI_VENDOR_ID_OXPCIE952) && (pDev->DeviceID == PCI_DEVICE_ID_OXPCIE952))
	{
		uart_printf("  OXPCIE952 external UART\n");
#ifdef T4K_BACK2BACK
		uart_printf("     WARNING!!!! Code is in T4K Back to Back (RC to EP) mode\n");
#endif
		PCIeWriteReg(PCIEREG(PCI_FUNC_BAR0), 0);		// UART and UDMA
		PCIeWriteReg(PCIEREG(PCI_FUNC_BAR1), 0);		// All visible modules & MSI-X (Used for MSI-X)
		PCIeWriteReg(PCIEREG(PCI_FUNC_BAR2), 0x200000);	// All visible modules & MSI-X (Used for EEPROM) - Don't Care

		// UART Registers will be @  (PCIE_RC_MEM + 0x1000*(UARTid+1));
		// since UART registers are at offset 0x1000 of the Mem Base (BAR0 + RC_MEM)
	}
	else
	{
		uart_printf("  Unknown device");
		return PCIE_RC_UNKNOWN_EP_ENABLE;
	}

	uart_printf("      Setting Bar[0] to 0x%x\n", PCIeReadReg(PCIEREG(PCI_FUNC_BAR0)));

	OldCmd = Cmd = PCIeReadReg(PCIEREG(PCI_FUNC_CMDSTAT));
	Cmd |= PCI_COMMAND_IO | 
			PCI_COMMAND_MEMORY |
			PCI_COMMAND_MASTER |
			PCI_COMMAND_PARITY_ERROR_RESP |
			PCI_COMMAND_SERR; // Enable IO, Mem, Bus Master, SERR and Parity Error

	uart_printf("Enable resources (IO and MEM): 0x%X -> 0x%X\n", OldCmd, Cmd);

	PCIeWriteReg(PCIEREG(PCI_FUNC_CMDSTAT), Cmd);

	pDev->IntMode = PCIE_INT_LEGACY;
	uart_printf("Interrupt mode %d\n", pDev->IntMode);

#if 0
	// Interrupt
	if (pDev->IntMode == PCIE_INT_MSI)	// Message Signaled Interrupt Mode
	{
		// Enable MSI Interrupts in MSI Control Register
		OldVal = PCIeReadReg(PCIEREG(PCI_FUNC_MSI_CTRL));
		RegVal = OldVal | (1 << 16);
		PCIeWriteReg(PCIEREG(PCI_FUNC_MSI_CTRL),  RegVal);
		RegVal = PCIeReadReg(PCIEREG(PCI_FUNC_MSI_CTRL));

		uart_printf("MSI Control: 0x%08X -> 0x%08X\n", OldVal, RegVal);
		ICEnableNonSecureInt(HAL_IRQ_PCIE_4, KSE_get_hw_cpuid() );
	}
	else if (pDev->IntMode == PCIE_INT_LEGACY) 	// Legacy Interrupt Mode
	{
		//REG32(RAD_CFG_PCIE_SET_IRQ) = 0x1; // PCIe INTA
		ICEnableNonSecureInt(HAL_IRQ_PCIE_0, KSE_get_hw_cpuid());
	}
#endif

	uart_printf("Configuration done\n");

	if((pDev->VendorID == PCI_VENDOR_ID_OXPCIE952) && (pDev->DeviceID == PCI_DEVICE_ID_OXPCIE952))
#ifdef T4K_BACK2BACK
		uart_printf("\nCode is in T4K Back to Back (RC to EP) mode - cannot configure OXPCIe952 \n");
#else
		OXPCIe952Init(0, PCIE_INT_LEGACY);
#endif

	return MX_OK;
}

void PcieMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (nCpuID == 0)
    {
        MmuMgrCfgMemAP(PCIE_RC_MEM,                 1024*1024,  AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(PCIE_RC_IO,                  1024*1024,  AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(PCIE_RC_CFG,                 1024*1024,  AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_PCIE_SYS_BASEADDR,   10*1024,    AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_CLKRST_BASEADDR,     4*1024,     AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}

