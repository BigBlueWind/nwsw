//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL i2cdiag.c
 *
 * @brief The I2C diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

//#include "i2cdiag.h"
#include <string.h>
#include "appprintf.h"
#include "i2cdrv.h"

#include "i2cdiag.h"

static void I2CDiagPrintError(MXRC rc)
{
	switch (rc)
	{
	case I2C_RC_TIMEOUT:
		uart_printf ("I2C diagnostic: Timeout reached.\n");
		break;
	case I2C_RC_STATUS_ERROR:
		uart_printf ("I2C diagnostic: Status is wrong.\n");
		break;
	case I2C_RC_EMPTY:
		uart_printf ("I2C diagnostic: I2C bus is empty.\n");
		break;
	case I2C_RC_ERROR:
		uart_printf ("I2C diagnostic: ERROR.\n");
		break;
	default:
		uart_printf ("I2C diagnostic: ERROR.\n");
	}
}

static MXRC I2CDiagWriteEEPROM(UINT8 DevSelCode, PUINT8 pSrcData, UINT32 nDstAddr, UINT32 nLen)
{
	MXRC rc;
	UINT32 i = 0;
	UINT32 nTry = 0;

	while (i < nLen)
	{
		// Transmits START condition
		rc = I2CStartXmit();
		if (rc != I2C_RC_OK)
			return rc;

		// Checks STATUS register
		if ((I2CGetStatus() != 0x08) && (I2CGetStatus() != 0x10))
			return I2C_RC_STATUS_ERROR;

		// Transmits Device Select Code
		DevSelCode &= 0xFE;
		DevSelCode |= I2C_WRITE;
		rc = I2CXmitByte(DevSelCode);
		if (rc != I2C_RC_OK)
			return rc;

		// Check STATUS register
		if (I2CGetStatus() != 0x18)
		{
			nTry += 1;
			if (nTry == I2C_MAX_SET_MASTER)
				return I2C_RC_SET_MASTER_ERROR;
			continue;
		}
		nTry = 0;

		// Transmits MSB of the address in the memory
		rc = I2CXmitByte((UINT8)(nDstAddr >> 8));
		if (rc != I2C_RC_OK)
			return rc;
		// Check STATUS register
		if (I2CGetStatus() != 0x28)
			return I2C_RC_STATUS_ERROR;
		// Transmits LSB of the address in the memory
		rc = I2CXmitByte((UINT8)nDstAddr);
		if (rc != I2C_RC_OK)
			return rc;
		// Check STATUS register
		if (I2CGetStatus() != 0x28)
			return I2C_RC_STATUS_ERROR;

		do
		{
			// Transmits data byte
			rc = I2CXmitByte(pSrcData[i]);
			if (rc != I2C_RC_OK)
				return rc;
			// Check STATUS register
			if (I2CGetStatus() != 0x28)
				return I2C_RC_STATUS_ERROR;
			i += 1;
		} while((i < nLen) && ((i % 128) != 0));

		// Transmits STOP condition
		rc = I2CStopXmit();
		if (rc != I2C_RC_OK)
			return rc;
		nDstAddr += i;
	}

	return I2C_RC_OK;
}

static MXRC I2CDiagReadEEPROM(UINT8 DevSelCode, PUINT8 pSrcData, UINT32 nDstAddr, UINT32 nLen)
{
	MXRC rc;
	UINT32 i = 0;

	// Transmits START condition
	rc = I2CStartXmit();
	if (rc != I2C_RC_OK)
		return rc;

	// Checks STATUS register
	if ((I2CGetStatus() != 0x08) && (I2CGetStatus() != 0x10))
		return I2C_RC_STATUS_ERROR;

	// Transmits Device Select Code and write bit
	DevSelCode &= 0xFE;
	DevSelCode |= I2C_WRITE;
	rc = I2CXmitByte(DevSelCode);
	if (rc != I2C_RC_OK)
		return rc;

	// Check STATUS register
	if (I2CGetStatus() != 0x18)
		return I2C_RC_STATUS_ERROR;

	// Transmits MSB of the address in the memory
	rc = I2CXmitByte((UINT8)(nDstAddr >> 8));
	if (rc != I2C_RC_OK)
		return rc;
	// Check STATUS register
	if (I2CGetStatus() != 0x28)
		return I2C_RC_STATUS_ERROR;
	// Transmits LSB of the address in the memory
	rc = I2CXmitByte((UINT8)nDstAddr);
	if (rc != I2C_RC_OK)
		return rc;
	// Check STATUS register
	if (I2CGetStatus() != 0x28)
		return I2C_RC_STATUS_ERROR;

	while (i < nLen)
	{
		// Transmits START condition repeatedly
		rc = I2CStartXmit();
		if (rc != I2C_RC_OK)
			return rc;

		// Checks STATUS register
		if ((I2CGetStatus() != 0x08) && (I2CGetStatus() != 0x10))
			return I2C_RC_STATUS_ERROR;

		// Transmits Device Select Code and read bit
		DevSelCode &= 0xFE;
		DevSelCode |= I2C_READ;
		rc = I2CXmitByte(DevSelCode);
		if (rc != I2C_RC_OK)
			return rc;

		// Check STATUS register
		if (I2CGetStatus() != 0x40)
			return I2C_RC_STATUS_ERROR;

		// Clears IFLG
		I2CClearBit(I2C_IFLG);

		// Reads the byte
		rc = I2CReadData(&pSrcData[i]);
		if (rc != I2C_RC_OK)
			return rc;
		i += 1;

		// Transmits STOP condition
		rc = I2CStopXmit();
		if (rc != I2C_RC_OK)
			return rc;
	}

	return I2C_RC_OK;
}

MXRC I2CDiagInit (UINT32* pReportData, UINT32* pReportSize)
{
	MXRC rc = I2C_RC_OK;
	UINT32 CpuID = MxGetCpuID();
	UINT32 i;
//	UINT32 datasize = 128; // size of page in EEPROM
	UINT32 datasize = 148; // size is more than page in EEPROM
	UINT32 memaddr = 0x0; // address in the memory
//	UINT32 memaddr = 0x3; // address in the memory
	UINT8* pSrcData = NULL;
	UINT8* pDstData = NULL;
	I2CINIT InitData = { I2C_CLKDIV, 0, 0, 0, 0 }; // Initial data for I2C controller
	UINT32 DevSelCode = 0xA0;  // Device Select Code for 512 Kbit serial I2C EEPROM

    DevSelCode = 0xA0; // Device Select Code for 512 Kbit serial I2C EEPROM

	// Allocates the memory for input and output test buffers
	pSrcData = MxHeapAlloc(NcnbHeap, datasize);
	pDstData = MxHeapAlloc(NcnbHeap, datasize);
	if ((pSrcData == NULL) || (pDstData == NULL))
	{
		if (pSrcData)
			MxHeapFree(NcnbHeap, pSrcData);
		if (pDstData)
			MxHeapFree(NcnbHeap, pDstData);
		uart_printf ("I2C diagnostic: Cannot allocate the memory.\n");
		return MX_HEAP_ALLOC_ERROR;
	}
	memset(pSrcData, 0x0, datasize);
	memset(pDstData, 0x0, datasize);
	// Fills input test buffer by some data
	for (i = 0; i < datasize; i++)
	{
		pSrcData[i] = (UINT8)i + 100;
	}

	uart_printf ("I2C diagnostic started on core %d.\n", CpuID);

	// Searches any EEPROM device on I2C bus
	I2CInitHwEx(&InitData);
	rc = I2CDiagSearchDev((PUINT8)&DevSelCode);
	if (rc != I2C_RC_OK)
	{
		I2CDiagPrintError(rc);
		I2CInitHw(); // returns the device to idle state
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		return rc;
	}

	//Start with simple Init and test EEPROM by using special EEPROM drivers
	I2CInitHw();
	// Writes the data in EEPROM
	i = I2CDrvWrite((UINT32)DevSelCode, pSrcData, memaddr, datasize);
	if (i != datasize)
	{
		uart_printf ("I2C diagnostic: Data writting ERROR, using EEPROM driver. There are written %d bytes from %d.\n", i, datasize);
		I2CInitHw(); // returns the device to idle state
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		return I2C_RC_ERROR;
	}
	MxDelay(5);
	// Reads the data from EEPROM
	i = I2CDrvRead((UINT32)DevSelCode, pDstData, memaddr, datasize);
	if (i != datasize)
	{
		uart_printf ("I2C diagnostic: Data reading ERROR, using EEPROM driver. There are read %d bytes from %d.\n", i, datasize);
		I2CInitHw(); // returns the device to idle state
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		return I2C_RC_ERROR;
	}

	if (memcmp(pSrcData, pDstData, datasize) != 0)
	{
		uart_printf ("I2C diagnostic EEPROM driver: Data in input and output buffers are different.\n");
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		return I2C_RC_ERROR;
	}

	//Start with simple Init and test EEPROM by using common drivers
	I2CInitHw();
	memset(pSrcData, 0x0, datasize);
	memset(pDstData, 0x0, datasize);
	// Fills input test buffer by some data
	for (i = 0; i < datasize; i++)
	{
		pSrcData[i] = (UINT8)i + 5;
	}
	// Writes the data in EEPROM
	rc = I2CDiagWriteEEPROM((UINT8)DevSelCode, pSrcData, memaddr, datasize);
	if (rc != I2C_RC_OK)
	{
		uart_printf ("I2C diagnostic: Data writting ERROR, using common driver. There are written %d bytes from %d.\n", i, datasize);
		I2CInitHw(); // returns the device to idle state
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		return rc;
	}
	MxDelay(5);
	// Reads the data from EEPROM
	rc = I2CDiagReadEEPROM((UINT8)DevSelCode, pDstData, memaddr, datasize);
	if (rc != I2C_RC_OK)
	{
		uart_printf ("I2C diagnostic: Data reading ERROR, using common driver. There are read %d bytes from %d.\n", i, datasize);
		I2CInitHw(); // returns the device to idle state
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		return rc;
	}

	if (memcmp(pSrcData, pDstData, datasize) != 0)
	{
		uart_printf ("I2C diagnostic common driver: Data in input and output buffers are different.\n");
		MxHeapFree(NcnbHeap, pSrcData);
		MxHeapFree(NcnbHeap, pDstData);
		return I2C_RC_ERROR;
	}

	uart_printf("I2C diagnostic: the test PASSED.\n");
	MxHeapFree(NcnbHeap, pSrcData);
	MxHeapFree(NcnbHeap, pDstData);

	return rc;
}

MXRC I2CDiagSearchDev(UINT8* DevSelCode)
{
	MXRC rc;
	UINT8 dev_addr = 0x00;
	UINT32 found = 0;

	do
	{
		// Transmits START condition
		rc = I2CStartXmit();
		if (rc != I2C_RC_OK)
			return rc;

		// Checks STATUS register
		if ((I2CGetStatus() != 0x08) && (I2CGetStatus() != 0x10))
			return I2C_RC_STATUS_ERROR;


		// Transmits Device Select Code
		*DevSelCode &= 0xF0;
		*DevSelCode |= (dev_addr << 1);
		rc = I2CXmitByte(*DevSelCode);
		if (rc != I2C_RC_OK)
			return rc;

		// Check STATUS register
		if (I2CGetStatus() == 0x18)
			found = 1;

		// Transmits STOP condition
		rc = I2CStopXmit();
		if (rc != I2C_RC_OK)
			return rc;

		if (found == 1)
			return I2C_RC_OK;

		dev_addr += 1;

	} while (dev_addr < 8);

	return I2C_RC_EMPTY;
}


