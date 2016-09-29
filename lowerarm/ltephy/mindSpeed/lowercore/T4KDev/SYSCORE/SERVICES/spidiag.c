//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL spidiag.c
 *
 * @brief The SPI diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "appprintf.h"
//#include "4gmx_heap.h"
#include "spidrv.h"
#include "heaps.h"
#include "systimers.h"
#include "spidiag.h"



MXRC SPIDiagInit(UINT32* pReportData, UINT32* pReportSize)
{
	MXRC rc = MX_OK;
	SPICONFIG * pSpiConfig;
	UINT32 SPIreg;
	SPI_CTRL_REGS* pSpiCtrlRegs;
	P_SPI_SR_REG pStatReg = (P_SPI_SR_REG)SPI_SR_REG;
	UINT32 ts;
	VUINT32 dummy_data_;

	// test SPI controller using SPI driver's functions

	// initialization of SPI
	if (!SPIIsInit())
	{
	    if (FAILED(rc = SPIInit(10)))
	    {
	        uart_printf ("SPI drv init error, rc=%x\r\n", rc);
	        return rc;
	    }
	}

	pSpiConfig = SPICreateDevCtx ();

	if (pSpiConfig == NULL)
	{
	    uart_printf ("SPI diagnostics: Cannot allocate the device context\r\n");
	    return SPI_RC_INIT_ERROR;
	}

	pSpiConfig->frameSize = SPI_FRAME_SIZE_8BITS;
	pSpiConfig->frameFormat = SPI_MOTOROLA_FORMAT;
	pSpiConfig->clkMode = SPI_CLOCK_MODE3;
	pSpiConfig->transferMode = SPI_RX_TX_ENABLE;
	pSpiConfig->frqDivider = SPI_DIVIDER_SPEED_1M;
	pSpiConfig->DevID = SPI_DEV_EPROM;
	pSpiConfig->CS = SPI_EPROM_CS;

	SPISelectDevice(pSpiConfig);

	// check CTRLR0 register
	SPIreg = REG32(SPI_CTRLR0_REG);
	if ((SPIreg & 0x0000000F) != SPI_FRAME_SIZE_8BITS)
		return SPI_RC_INIT_ERROR;
	if (((SPIreg & 0x00000030) >> 4) != SPI_MOTOROLA_FORMAT)
		return SPI_RC_INIT_ERROR;
	if (((SPIreg & 0x000000C0) >> 6) != SPI_CLOCK_MODE3)
		return SPI_RC_INIT_ERROR;
	if (((SPIreg & 0x00000300) >> 8) != SPI_RX_TX_ENABLE)
		return SPI_RC_INIT_ERROR;

	// check SER register
	SPIreg = REG32(SPI_SER_REG);
	if (SPIreg != SPI_EPROM_CS)
		return SPI_RC_INIT_ERROR;

	// check BAUDR register
	SPIreg = REG32(SPI_BAUDR_REG);
	if (SPIreg != SPI_DIVIDER_SPEED_1M)
		return SPI_RC_INIT_ERROR;

	// check SR register
	SPIreg = REG32(SPI_SR_REG);
	if (SPIreg != 0x6)
		return SPI_RC_INIT_ERROR;

	// check an attempt to modify a register when the device is enabled, using BAUDR register
	pSpiCtrlRegs = SPIGetCtrlRegs();
	SPIEnable (pSpiCtrlRegs);

	REG32(SPI_BAUDR_REG) = 6;
	SPIreg = REG32(SPI_BAUDR_REG);
	if (SPIreg == 6)
	{
		SPISelectDevice(NULL);
		SPIRemoveDevCtx(pSpiConfig);
		rc = SPIShutdown ();
		uart_printf("1. SPI diagnostics: BAUDR reg error: %08X\n", SPIreg);
		return SPI_RC_INIT_ERROR;
	}
	SPIDisable (pSpiCtrlRegs);
	REG32(SPI_BAUDR_REG) = 6;
	SPIreg = REG32(SPI_BAUDR_REG);
	if (SPIreg != 6)
	{
		SPISelectDevice(NULL);
		SPIRemoveDevCtx(pSpiConfig);
		rc = SPIShutdown ();
		uart_printf("2. SPI diagnostics: BAUDR reg error: %08X\n", SPIreg);
		return SPI_RC_INIT_ERROR;
	}
	// check LSB of BAUDR register to be always 0
	REG32(SPI_BAUDR_REG) = 7;
	SPIreg = REG32(SPI_BAUDR_REG);
	if (SPIreg != 6)
	{
		SPISelectDevice(NULL);
		SPIRemoveDevCtx(pSpiConfig);
		rc = SPIShutdown ();
		uart_printf("3. SPI diagnostics: BAUDR reg error: %08X\n", SPIreg);
		return SPI_RC_INIT_ERROR;
	}
	// restore BAUDR register
	REG32(SPI_BAUDR_REG) = SPI_DIVIDER_SPEED_1M;
	SPIEnable (pSpiCtrlRegs);

	// check data transmit/receive
	REG32(SPI_DR_REG) = DUMMY_DATA;
	ts = SysTimerGetTick();
	while ((SysTimerGetTick() - ts) < 150000)
	{
		if (pStatReg->BSY == 0)
			break;
	}
	if (pStatReg->BSY != 0)
	{
		SPISelectDevice(NULL);
		SPIRemoveDevCtx(pSpiConfig);
		rc = SPIShutdown ();
		uart_printf("SPI diagnostics: the device is still busy.\n");
		return SPI_RC_TIMEOUT;
	}
	ts = SysTimerGetTick();
	while ((SysTimerGetTick() - ts) < 150000)
	{
		if (REG32(SPI_RXFLR_REG) != 0)
			break;
	}
	if (REG32(SPI_RXFLR_REG) == 0)
	{
		SPISelectDevice(NULL);
		SPIRemoveDevCtx(pSpiConfig);
		rc = SPIShutdown ();
		uart_printf("SPI diagnostics: the device cannot receive any data.\n");
		return SPI_RC_TIMEOUT;
	}
	ts = SysTimerGetTick();
	while (REG32(SPI_RXFLR_REG) != 0)
	{
		dummy_data_ = REG32(SPI_DR_REG);
		if ((SysTimerGetTick() - ts) > 1500000)
		{
			SPISelectDevice(NULL);
			SPIRemoveDevCtx(pSpiConfig);
			rc = SPIShutdown ();
			uart_printf("SPI diagnostics: the device cannot read any data.\n");
			return SPI_RC_TIMEOUT;
	        }
	}

	// shutdown SPI controller
	SPISelectDevice(NULL);
	SPIRemoveDevCtx(pSpiConfig);
	rc = SPIShutdown ();

	// test SPI EEPROM using direct access to SPI controller registers

#ifdef _SPIDIAGTESTSPI_
	rc = SPIDiagTestSPI();
	if (rc != MX_OK)
		return rc;
#endif

	uart_printf ("SPI diagnostics PASSED.\n");

	return rc;
}

#ifdef _SPIDIAGTESTSPI_

MXRC SPIDiagTestSPI(void)
{
	MXRC rc = MX_OK;
	UINT8* pSrcData = NULL;
	UINT8* pDstData = NULL;
	UINT32 addr = 0x0;
	UINT32 datasize = 256;
	UINT32 i;
	SPI_INIT_PARAMS initparams;
//	UINT32 DevID = 0x1F460200;
	UINT32 DevID = 0x20201500;
	UINT32 DevCS;

	// allocating input and output data buffers
	pSrcData = MxHeapAlloc(NcnbHeap, datasize);
	pDstData = MxHeapAlloc(NcnbHeap, datasize);
	if ((pSrcData == NULL) || (pDstData == NULL))
	{
		if (pSrcData)
			MxHeapFree(NcnbHeap, pSrcData);
		if (pDstData)
			MxHeapFree(NcnbHeap, pDstData);
		uart_printf ("SPI diagnostics: Cannot allocate the memory.\n");
		return MX_HEAP_ALLOC_ERROR;
	}

	memset (pSrcData, 0x0, datasize);
	memset (pDstData, 0x0, datasize);
	for (i = 0; i < datasize; i++)
	{
		if (i < 256) // if i < than one byte can contain
			*(pSrcData + i) = i;
		else         // if i > than one byte can contain
			*(pSrcData + i) = i%256;
	}

	// initialization of SPI controller
//	initparams.BAUDR_BaudRate = 6;
	initparams.BAUDR_BaudRate = SPI_DIVIDER_SPEED_1M;
	initparams.CTRL0_SCPH_MOPhase = 1;
	initparams.CTRL0_SCPOL_MOPolarity = 1;
	rc = SPIDiagInitDeviceEx(initparams);
	if (rc != MX_OK)
	{
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		uart_printf("SPI diagnostics: the device cannot be initialized.\n");
		return rc;
	}

	// for debugging
//	uart_printf("CTRL0 : %08X\n", REG32(SPI_CTRLR0_REG));
//	uart_printf("CTRL1 : %08X\n", REG32(SPI_CTRLR1_REG));
//	uart_printf("SSIENR : %08X\n", REG32(SPI_SSIENR_REG));
//	uart_printf("SER : %08X\n", REG32(SPI_SER_REG));
//	uart_printf("BAUDR : %08X\n", REG32(SPI_BAUDR_REG));
//	uart_printf("SR : %08X\n", REG32(SPI_SR_REG));
	////////////////

	// looking for device on SPI bus
	rc = SPIDiagLookForDevice(DevID, &DevCS);
	if (rc != MX_OK)
	{
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		uart_printf("SPI diagnostics: cannot find the device.\n");
		return rc;
	}
	DevCS = 0x1<<3; // only this device is placed on SPI bus

	// diagnostics SPI EEPROM using direct access to SPI controller registers

	// writing of device
	rc = SPIDiagWriteEEPROM(DevCS, addr, pSrcData, datasize);
	if (rc != MX_OK)
	{
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		uart_printf("SPI diagnostics: cannot write the data.\n");
		return rc;
	}

	// reading of EEPROM device
	rc = SPIDiagReadEEPROM(DevCS, addr, pDstData, datasize);
	if (rc != MX_OK)
	{
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		uart_printf("SPI diagnostics: cannot read the data.\n");
		return rc;
	}

	// comparing of input and output data
	if (memcmp(pSrcData, pDstData, datasize) != 0)
	{
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		uart_printf("SPI diagnostics: input and output buffers are different.\n");
		return SPI_RC_NUM_DEV_ERROR;
	}


	MxHeapFree(NcnbHeap, pSrcData);
	MxHeapFree(NcnbHeap, pDstData);
	uart_printf("SPI diagnostics: read-write OK.\n");

	return rc;
}

void SPIDiagEnableDev(void)
{
//	REG32(SPI_SSIENR_REG) = 0x1;
	REG32(SPI_SSIENR_REG) = 0x3;
}

void SPIDiagDisableDev(void)
{
	REG32(SPI_SSIENR_REG) = 0x0;
}

MXRC SPIDiagWaitBusyFlag(void)
{
	P_SPI_SR_REG pStatReg = (P_SPI_SR_REG)SPI_SR_REG;
	UINT32 ts = SysTimerGetTick();

	while ((SysTimerGetTick() - ts) < 150000)
	{
		if (pStatReg->BSY == 0)
			return MX_OK;
	}

	return SPI_RC_TIMEOUT;
}

void SPIDiagSetCS(UINT32 DevCS)
{
	SPIDiagDisableDev();
	REG32(SPI_SER_REG) = DevCS;
	SPIDiagEnableDev();
}

MXRC SPIDiagGetStatus(UINT32* statusReg)
{
	MXRC rc = MX_OK;
	UINT32 i;

	EPROM_CS_LOW;

	REG32(SPI_DR_REG) = cmd_RD_STATUS;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;

	// send 4 dummy bytes to receive correct status
//	for (i = 0; i < 4; i++)
//	{
//		REG32(SPI_DR_REG) = SPI_DUMMY_BYTE;
//		rc = SPIDiagWaitBusyFlag();
//		if (rc != MX_OK)
//			return rc;
//	}

	// clears RxFIFO of unknown data
	SPIDiagDisableDev(); // disables SPI controller
	SPIDiagEnableDev();  // enables SPI controller

	// read status register
	*statusReg = 0;
	for (i = 0; i < 2; i++)
	{
		REG32(SPI_DR_REG) = SPI_DUMMY_BYTE;
		rc = SPIDiagWaitBusyFlag();
		if (rc != MX_OK)
			return rc;
		*statusReg |= (REG32(SPI_DR_REG) & 0x000000FF);
//		*statusReg <<= sizeof(char);
		if (i == 1)
			break;
		*statusReg <<= 8;
	}

	EPROM_CS_HIGH;

	return rc;
}

MXRC SPIDiagWaitEEPROMbusy(void)
{
	MXRC rc = MX_OK;
	UINT32 statusReg;
	UINT32 ts = SysTimerGetTick();

	while ((SysTimerGetTick() - ts) < 15000000)
	{
		// read Status Register
		rc = SPIDiagGetStatus(&statusReg);
		if (rc != MX_OK)
			return rc;
		if (((P_SPI_EEPROM_STAT_REG)(&statusReg))->RDYBSY1 == 0)
			return MX_OK;
	}
	return SPI_RC_TIMEOUT;
}

MXRC SPIDiagWriteEnable(UINT32 DevCS)
{
	MXRC rc = MX_OK;
	UINT32 statusReg = 0;

	// set Write Enable bit
	EPROM_CS_LOW;
	REG32(SPI_DR_REG) = cmd_WR_ENABLE;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	uart_printf("in Write enable func1\n");
	// launch Write Enable algorithm
//	SPIDiagDisableDev(); // disables SPI controller
//	REG32(SPI_SER_REG) = 0x0;   // deselect device on SPI bus
//	REG32(SPI_SER_REG) = DevCS; // select device on SPI bus
//	SPIDiagEnableDev(); // enables SPI controller
	EPROM_CS_HIGH;
//	EPROM_CS_LOW;
	// wait while EEPROM is being busy
	rc = SPIDiagWaitEEPROMbusy();
	if (rc != MX_OK)
		return rc;
	// read Status register of EEPROM
	rc = SPIDiagGetStatus(&statusReg);
	if (rc != MX_OK)
		return rc;
	uart_printf ("Status reg after Write Enable: %08X\n", statusReg);
	if (((P_SPI_EEPROM_STAT_REG)(&statusReg))->WELbit == 0)
		return SPI_RC_WRITE_ERROR;

	return rc;
}

MXRC SPIDiagWriteOpcode(UINT32 opcode)
{
	MXRC rc = MX_OK;

	REG32(SPI_DR_REG) = opcode;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;

	return rc;
}

MXRC SPIDiagWriteByte(UINT32 data)
{
	MXRC rc = MX_OK;

	REG32(SPI_DR_REG) = data;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;

	return rc;
}

MXRC SPIDiagReadByte03h(UINT8* pData)
{
	MXRC rc = MX_OK;

	// clears RxFIFO of unknown data
	SPIDiagDisableDev(); // disables SPI controller
	SPIDiagEnableDev();  // enables SPI controller

	// read one byte
	REG32(SPI_DR_REG) = SPI_DUMMY_BYTE;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	*pData = (UINT8)REG32(SPI_DR_REG);

	return rc;
}

MXRC SPIDiagWriteAddrEEPROM(UINT32 addr)
{
	MXRC rc = MX_OK;

	// write three address bytes into the EEPROM memory
	REG32(SPI_DR_REG) = addr & 0x001F0000;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	REG32(SPI_DR_REG) = addr & 0x0000FF00;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	REG32(SPI_DR_REG) = addr & 0x000000FF;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;

	return rc;
}

MXRC SPIDiagGetSectorProtectReg(UINT32 addr, UINT8* protectReg)
{
	MXRC rc = MX_OK;
	UINT32 i = 0;

	REG32(SPI_DR_REG) = 0x3C;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	rc = SPIDiagWriteAddrEEPROM(addr);
	if (rc != MX_OK)
		return rc;
	// send 2 dummy bytes to receive correct sector protect register
	for (i = 0; i < 2; i++)
	{
		REG32(SPI_DR_REG) = SPI_DUMMY_BYTE;
		rc = SPIDiagWaitBusyFlag();
		if (rc != MX_OK)
			return rc;
	}
	// clears RxFIFO of unknown data
	SPIDiagDisableDev(); // disables SPI controller
	SPIDiagEnableDev();  // enables SPI controller

	*protectReg = 0;
	REG32(SPI_DR_REG) = SPI_DUMMY_BYTE;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	*protectReg = REG32(SPI_DR_REG);

	return rc;
}

MXRC SPIDiagProtectSector(UINT32 DevCS, UINT32 addr)
{
	MXRC rc = MX_OK;
	UINT8 protectReg;

	// set Write Enable bit
	rc = SPIDiagWriteEnable(DevCS);
	if (rc != MX_OK)
		return rc;
	// write opcode
	REG32(SPI_DR_REG) = 0x36;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	// write 3 address bytes
	rc = SPIDiagWriteAddrEEPROM(addr);
	if (rc != MX_OK)
		return rc;
	// launch protect algorithm
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = 0x0;   // deselect device on SPI bus
	REG32(SPI_SER_REG) = DevCS; // select device on SPI bus
	SPIDiagEnableDev(); // enables SPI controller
	// wait while EEPROM is being busy
	rc = SPIDiagWaitEEPROMbusy();
	if (rc != MX_OK)
		return rc;
	// check  Sector Protection Register
	rc = SPIDiagGetSectorProtectReg(addr, &protectReg);
	if (rc != MX_OK)
		return rc;
	if (protectReg == 0x0)
		return SPI_RC_WRITE_ERROR;

	return rc;
}
MXRC SPIDiagUnprotectSector(UINT32 DevCS, UINT32 addr)
{
	MXRC rc = MX_OK;
	UINT8 protectReg;

	// set Write Enable bit
	rc = SPIDiagWriteEnable(DevCS);
	if (rc != MX_OK)
		return rc;
	// write opcode
	REG32(SPI_DR_REG) = 0x39;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	// write 3 address bytes
	rc = SPIDiagWriteAddrEEPROM(addr);
	if (rc != MX_OK)
		return rc;
	// launch unprotect algorithm
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = 0x0;   // deselect device on SPI bus
	REG32(SPI_SER_REG) = DevCS; // select device on SPI bus
	SPIDiagEnableDev(); // enables SPI controller
	// wait while EEPROM is being busy
	rc = SPIDiagWaitEEPROMbusy();
	if (rc != MX_OK)
		return rc;
	// check  Sector Protection Register
	rc = SPIDiagGetSectorProtectReg(addr, &protectReg);
	if (rc != MX_OK)
		return rc;
	if (protectReg == 0xFF)
		return SPI_RC_WRITE_ERROR;

	return rc;
}
MXRC SPIDiagSectorErase4K(UINT32 DevCS, UINT32 addr)
{
	MXRC rc = MX_OK;
	UINT32 statusReg;

	// set Write Enable bit
	rc = SPIDiagWriteEnable(DevCS);
	if (rc != MX_OK)
		return rc;
	// write opcode
	REG32(SPI_DR_REG) = 0x20;
	rc = SPIDiagWaitBusyFlag();
	if (rc != MX_OK)
		return rc;
	// write 3 address bytes
	rc = SPIDiagWriteAddrEEPROM(addr);
	if (rc != MX_OK)
		return rc;
	// launch erasing algorithm
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = 0x0;   // deselect device on SPI bus
	REG32(SPI_SER_REG) = DevCS; // select device on SPI bus
	SPIDiagEnableDev(); // enables SPI controller
	// wait while EEPROM is being busy
	rc = SPIDiagWaitEEPROMbusy();
	if (rc != MX_OK)
		return rc;
	// read Status Register to check EPE bit for errors
	rc = SPIDiagGetStatus(&statusReg);
	if (rc != MX_OK)
		return rc;
	if (((P_SPI_EEPROM_STAT_REG)(&statusReg))->EPE == 1)
		return SPI_RC_WRITE_ERROR;

	return rc;
}

MXRC SPIDiagRunProgramAlgorithm(UINT32 DevCS)
{
	MXRC rc = MX_OK;
	UINT32 statusReg = 0;

	// launch program algorithm
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = 0x0;   // deselect device on SPI bus
	REG32(SPI_SER_REG) = DevCS; // select device on SPI bus
	SPIDiagEnableDev(); // enables SPI controller
	// wait while EEPROM is being busy
	rc = SPIDiagWaitEEPROMbusy();
	if (rc != MX_OK)
		return rc;
	// check EPE status flag
	rc = SPIDiagGetStatus(&statusReg);
	if (rc != MX_OK)
		return rc;
	if (((P_SPI_EEPROM_STAT_REG)(&statusReg))->EPE == 1)
		return SPI_RC_WRITE_ERROR;

	return rc;
}

MXRC SPIDiagInitDevice(void)
{
	return MX_OK;
}

MXRC SPIDiagInitDeviceEx(SPI_INIT_PARAMS initparams)
{
	MXRC rc = MX_OK;
	P_SPI_CTRL0_REG pCtrlReg = (P_SPI_CTRL0_REG)SPI_CTRLR0_REG;

	//Set upt spi pin configuration
	REG32(GPIO_PIN_SELECT_REG) &= ~(0x3F<<21);

	//allowing signals transmit through buffer
	REG32(GPIO_PIN_SELECT_REG) |= (1<<9);
	REG32(GPIO_OE_REG) |= (1<<9);
	REG32(GPIO_OUTPUT_REG) &= ~(1<<9);

	SPIDiagDisableDev(); // disables SPI controller

	// initialization of the registers
	REG32(SPI_BAUDR_REG) = initparams.BAUDR_BaudRate;
	pCtrlReg->CTRL0_SCPH_MOPhase = initparams.CTRL0_SCPH_MOPhase;
	pCtrlReg->CTRL0_SCPOL_MOPolarity = initparams.CTRL0_SCPOL_MOPolarity;

	SPIDiagEnableDev(); // enable SPI controller

	//use GPIO to control device due to the cs timing requirement
	REG32(GPIO_PIN_SELECT_REG) |= (1<<EEPROM_CS_GPIO);
	REG32(GPIO_OE_REG) |= (1<<EEPROM_CS_GPIO);
	REG32(GPIO_OUTPUT_REG) |= (1<<EEPROM_CS_GPIO);

	return rc;
}

MXRC SPIDiagLookForDevice(UINT32 DevID, UINT32* DevCS)
{
	MXRC rc;
	UINT32 i, j;
	UINT32 ReadDevID;
//	P_SPI_SR_REG pStatReg = (P_SPI_SR_REG)SPI_SR_REG;
//	UINT32 dummy;
//	UINT32 k = 0;

//	EPROM_CS_LOW;

	for (i = 0; i < SPI_DEV_NUM; i++)
	{
		EPROM_CS_LOW;

		SPIDiagDisableDev(); // disables SPI controller
		REG32(SPI_SER_REG) = 0x1<<i; // select device on SPI bus
//		REG32(SPI_SER_REG) = 0x1<<3; // select device on SPI bus
		SPIDiagEnableDev(); // enables SPI controller
//		MxDelay(10);
//		EPROM_CS_LOW;
		REG32(SPI_DR_REG) = cmd_ID; // send opcode
		rc = SPIDiagWaitBusyFlag();
		if (rc != MX_OK)
			return rc;

		// clears RxFIFO of unknown data
		SPIDiagDisableDev(); // disables SPI controller
		SPIDiagEnableDev();  // enables SPI controller
//		k = 0;
//		while ( REG32(SPI_RXFLR_REG) != 0)
//		{
//			dummy = REG32(SPI_DR_REG);
//			k += 1;
//		}
//		uart_printf("k = %d\n", k);

		ReadDevID = 0;
		for (j = 0; j < 4; j++)
		{
			REG32(SPI_DR_REG) = SPI_DUMMY_BYTE; // send dummy byte for reading the byte of data
			rc = SPIDiagWaitBusyFlag();
			if (rc != MX_OK)
				return rc;
//			while ( REG32(SPI_RXFLR_REG) == 0);
			ReadDevID |= (REG32(SPI_DR_REG) & 0x000000FF); // read the byte of data
			uart_printf("ID: %08X\n", ReadDevID);
//			ReadDevID <<= sizeof(char);
			if (j == 3)
				break;
			ReadDevID <<= 8;
		}
		uart_printf("ID of SPI device %d: %08X\n", i, ReadDevID); // for debugging
		if (ReadDevID == DevID)
		{
			*DevCS = 0x1<<i;
			SPIDiagDisableDev(); // disables SPI controller
			REG32(SPI_SER_REG) = 0x0; // deselect all devices on SPI bus
			SPIDiagEnableDev(); // enables SPI controller
			EPROM_CS_HIGH;
			return MX_OK;
		}

		EPROM_CS_HIGH;
	}
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = 0x0; // deselect all devices on SPI bus
	SPIDiagEnableDev(); // enables SPI controller

//	EPROM_CS_HIGH;

	return SPI_RC_NUM_DEV_ERROR;
}

MXRC SPIDiagWriteEEPROM(UINT32 DevCS, UINT32 addr, UINT8* pData, UINT32 datasize)
{
	MXRC rc = MX_OK;
	UINT32 sectorsize = 4 * 1024; // sector size is 4 KBytes
	UINT32 bufsize = 256; // size of internal buffer of EEPROM
	UINT32 statusReg; // status register of EEPROM
	UINT8  sectorProtectReg; // sector protection register of EEPROM
	UINT32 i = 0; // counter of data to be written

	if (datasize == 0)
		return SPI_RC_WRITE_ERROR;
	if (pData == NULL)
		return SPI_RC_WRITE_ERROR;

	// select device on SPI bus
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = DevCS; // select device on SPI bus
	SPIDiagEnableDev(); // enables SPI controller

//	EPROM_CS_LOW;

	// read status of write protection pin
//	rc = SPIDiagGetStatus(&statusReg);
//	if (rc != MX_OK)
//		return rc;
//	uart_printf("EEPROM Status reg: %08X\n", statusReg); // for debugging
//	if (((P_SPI_EEPROM_STAT_REG)(&statusReg))->WPP == 0)
//		return SPI_RC_WRITE_ERROR;
//	uart_printf("EEPROM Status reg: %08X\n", statusReg); // for debugging


	// write data
	while (i < datasize)
	{
		// read sector protect register
		rc = SPIDiagGetSectorProtectReg(addr, &sectorProtectReg);
		if (rc != MX_OK)
			return rc;
		uart_printf ("Protect reg %08X\n", sectorProtectReg); // for debugging
		if (sectorProtectReg == 0xFF)
			// if sector protected - unprotect one
			rc = SPIDiagUnprotectSector(DevCS, addr);
		if (rc != MX_OK)
			return rc;
		uart_printf ("Protect reg %08X\n", sectorProtectReg); // for debugging
		// erase sector
		rc = SPIDiagSectorErase4K(DevCS, addr);
		if (rc != MX_OK)
			return rc;
		uart_printf ("Sector erased\n"); // for debugging
		// write not more than 4 KBytes of the data
		do
		{
			// set Write Enable bit
			rc = SPIDiagWriteEnable(DevCS);
			if (rc != MX_OK)
				return rc;
			// write Byte/Page program opcode into EEPROM
			rc = SPIDiagWriteOpcode(cmd_PAGE_PROGRAM);
			if (rc != MX_OK)
				return rc;
			// write three address bytes into the EEPROM memory
			rc = SPIDiagWriteAddrEEPROM(addr);
			if (rc != MX_OK)
				return rc;
			// write not more than 256 bytes of the data into internal buffer
			do
			{
				rc = SPIDiagWriteByte((UINT32)*pData);
				if (rc != MX_OK)
					return rc;
				pData += 1;
				addr  += 1;
				i     += 1;
			} while ((addr % bufsize) && (i < datasize));
			// launch program algorithm
			rc = SPIDiagRunProgramAlgorithm(DevCS);
			if (rc != MX_OK)
				return rc;
		} while ((addr % sectorsize) && (i < datasize));
		// protect sector
		rc = SPIDiagProtectSector(DevCS, addr - 1);
		if (rc != MX_OK)
			return rc;
	}

//	EPROM_CS_HIGH;

	// deselect device on SPI bus
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = 0x0; // deselect device on SPI bus
	SPIDiagEnableDev(); // enables SPI controller

	return rc;
}

MXRC SPIDiagReadEEPROM(UINT32 DevCS, UINT32 addr, UINT8* pData, UINT32 datasize)
{
	MXRC rc = MX_OK;
	UINT32 i = 0;

	if (datasize == 0)
		return SPI_RC_READ_ERROR;
	if (pData == NULL)
		return SPI_RC_READ_ERROR;

	// select device on SPI bus
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = DevCS; // select device on SPI bus
	SPIDiagEnableDev(); // enables SPI controller

	// send opcode
	rc = SPIDiagWriteOpcode(cmd_RD_DATA);
	if (rc != MX_OK)
		return rc;
	// send address
	rc = SPIDiagWriteAddrEEPROM(addr);
	if (rc != MX_OK)
		return rc;
	// read data
	while (i < datasize)
	{
		rc = SPIDiagReadByte03h(pData);
		if (rc != MX_OK)
			return SPI_RC_READ_ERROR;
		pData += 1;
		i += 1;
	}
	// deselect device on SPI bus
	SPIDiagDisableDev(); // disables SPI controller
	REG32(SPI_SER_REG) = 0x0; // deselect device on SPI bus
	SPIDiagEnableDev(); // enables SPI controller

	return rc;
}

#endif

