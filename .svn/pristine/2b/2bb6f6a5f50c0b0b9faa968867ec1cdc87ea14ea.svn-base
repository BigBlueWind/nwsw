//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL spidrv.c
 *
 * @brief This file contains SPI driver "C" code
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "spidrv.h"
#include "heaps.h"
#include <string.h>
#include "appprintf.h"
#include "mmumgr.h"

SPI_CTX gSpiCtx = {0};

INLINE PSPI_CTX SpiGetCtx(void)
{
    return &gSpiCtx;
}

static void SpiIrqHandler(volatile UINT32 * pClear, UINT32 nIrq, LPVOID pParam)
{
    PSPI_CTX pCtx = (PSPI_CTX)pParam;

    if (pCtx)
    {
        if (pCtx->IrqProc != NULL)
            pCtx->IrqProc(pCtx->IrqProcData);
    }

    // to ack interrupt on SPI bus

    *pClear = nIrq;
}

/** @brief This function should be used to initialize/reinitialize the SPI driver
    @param NumDevs [in] - the number of devices, that can be llocated by the aplication
    @return [MXRC] an error code */

MXRC SPIInit(UINT32 NumDevs)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;
    MXRC rc = SPI_RC_OK;
    VUINT16 dummy;
    UINT32 BlockSize;

    _ASSERT_PTR(pSpiCtx);

    if (NumDevs == 0)
        return SPI_RC_NUM_DEV_ERROR;

    if (pSpiCtx->bInited)
        return SPI_RC_CALL_SHUTDOWN;

    memset(pSpiCtx, 0, sizeof(*pSpiCtx));

    pSpiCtx->SpiCtrlRegs = (SPI_CTRL_REGS *)SPI_CTRL_REGS_BASE;
    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    pSpiCtx->hHeap = SPI_HEAP;

    BlockSize = ROUND(sizeof(SPICONFIG), 16);

    if (NULL == pSpiCtx->SpiBuf)
    {
        pSpiCtx->SpiBuf = MxHeapAlloc(SPI_HEAP, NumDevs * BlockSize);

        _ASSERT_PTR(pSpiCtx->SpiBuf);

        if (HANDLE_NULL(pSpiCtx->SpiPart))
        {
            if (FAILED(rc = MxCreatePart(pSpiCtx->SpiBuf, NumDevs, BlockSize, NULL, HNULL, HNULL, &pSpiCtx->SpiPart)))
            {
                _ASSERT_RC(rc);
                return rc;
            }

            // the owner should share this object
            // for the other ARMs

            if (FAILED(rc = MxShareObject(pSpiCtx->SpiPart, OBJ_SHARE_ON)))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }
    }

    pSpiCtx->NumDevs = NumDevs;
    pSpiCtx->CpuID = MxGetCpuID ();

    //Set upt spi pin configuration
    pSpiCtx->GpioPins = REG32(GPIO_PIN_SELECT_REG);
    REG32(GPIO_PIN_SELECT_REG) &= ~(0x3F<<21);

    SPIDisable(pSpiCtrlRegs);

    if (FAILED(rc = MxSetIntHandler(HAL_IRQ_SPI, INT_TYPE_IRQ, PRI_SPI, SpiIrqHandler, pSpiCtx)))
        return rc;
#if 0
    if (FAILED(rc = MxEnableInt(HAL_IRQ_SPI)))
        return rc;
#endif

    SPIEnable(pSpiCtrlRegs);

    //Clear FIFO
    while ((VUINT16)pSpiCtrlRegs->RxFlR != 0)
        dummy = (VUINT16)pSpiCtrlRegs->DR;

    pSpiCtx->bInited = TRUE;

    printf ("SPI inited on ARM-%d (NumDevs=%d)\r\n", MxGetCpuID (), NumDevs);

    return SPI_RC_OK;
}

/** @brief This function returns status of driver initialization
    @return [BOOL] status */

BOOL SPIIsInit (void)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    _ASSERT_PTR(pSpiCtx);

    return pSpiCtx->bInited;
}

/** @brief This function shutdowns the driver
    @return [MXRC] an error code */

MXRC SPIShutdown(void)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    _ASSERT_PTR(pSpiCtx);

    if(pSpiCtx->bInited == FALSE)
        return SPI_RC_OK;

    if(pSpiCtx->CpuID != MxGetCpuID ())
        return SPI_RC_NOT_OWNER;

    SPIDisableCS(pSpiCtrlRegs, 0xFFFFFFFF);
    SPIDisable(pSpiCtrlRegs);

    if(HANDLE_NOT_NULL(pSpiCtx->SpiPart))
    {
        MxShareObject(pSpiCtx->SpiPart, OBJ_SHARE_OFF);
        MxRemovePart(pSpiCtx->SpiPart);
        pSpiCtx->SpiPart = HNULL;
    }

    if(pSpiCtx->SpiBuf != NULL)
    {
        MxHeapFree(SPI_HEAP, pSpiCtx->SpiBuf);
        pSpiCtx->SpiBuf = NULL;
    }

    MxDisableInt (HAL_IRQ_SPI);

    // to restore the GPIO pins to provide the possibility to use UART-3
    REG32(GPIO_PIN_SELECT_REG) = pSpiCtx->GpioPins;

    memset (pSpiCtx, 0, sizeof (*pSpiCtx));

    printf("SPI shutdown completed\r\n");

    return SPI_RC_OK;
}

/** @brief This function sets irq proc pointer to be called when
           SPI IRQ is processed

    @param Irq  [in] - the pointer the IRQ proc, it can be NULL
    @param pData[in] - the data

    @return [MXRC] an error code */

MXRC SPIDrvSetIrqProc (void(*Irq)(void*), void*pData)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    _ASSERT_PTR (pSpiCtx);

    pSpiCtx->IrqProc        = Irq;
    pSpiCtx->IrqProcData    = pData;

    return SPI_RC_OK;
}

/** @brief The function allocates device context (set of device parameters)
            that has to be used in SPI read/write function

    @note: The algorithm is:
        - to create the device context
        - to setup parameters
        - to select device
        - to work with device (by using read/write command)
        - to close device
        - to remove device context

    @return [PSPICONFIG] the pointer to the device context */

PSPICONFIG SPICreateDevCtx(void)
{
    PSPICONFIG pConfig = NULL;
    PSPI_CTX pSpiCtx = SpiGetCtx();

    _ASSERT_PTR(pSpiCtx);

    pConfig = MxAllocBlock(pSpiCtx->SpiPart);

    _ASSERT_PTR(pConfig);

    if (pConfig != NULL)
        memset (pConfig, 0, sizeof (pConfig));

    return pConfig;
}

/** @brief The function frees the device context (set of device parameters)
            that was allocated by SPICreateDevCtx function

    @param pConfig [in] - the pointer to the allocated context

    @return [MXRC] the error code */

MXRC SPIRemoveDevCtx(PSPICONFIG pConfig)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();

    _ASSERT_PTR (pSpiCtx);
    _ASSERT_PTR (pConfig);

    return MxFreeBlock(pSpiCtx->SpiPart, pConfig);
}

/** @brief The function setups device parameters, selects CS and prepares
            SPI HW device to work with HW device connected to SPI

    @param pDevConfig [in] - the pointer to the device context

    @return [MXRC] the error code */

MXRC SPISelectDevice(PSPICONFIG pDevConfig)
{
    SPI_CTX * pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;
    VUINT16 tmp = 0;

    _ASSERT_PTR(pSpiCtx);

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    if (pDevConfig == NULL)
    {
        SPIDisableCS(pSpiCtrlRegs, pSpiCtx->SelectedCS);
        pSpiCtx->SelectedCS = 0;

        return SPI_RC_OK;
    }

    SPIDisable(pSpiCtrlRegs);

    // b0 - b3 		: 16 bit transfer
    // b4 - b5 		: Motorola SPI
    // b6      		: Clock toggles in the middle of data bit
    // b7	   		: Inactive state of serial
    // b8 - b9 		: Transmit & receive,
    // b10	   		: Slave txd is enabled
    //*(UINT32 *)SPI_CTRLR0_REG = 0x00c7; // 8-bit interface
    pSpiCtrlRegs->CtrlR0 = (pDevConfig->frameSize) | \
                                        (pDevConfig->frameFormat << 4) | \
                                        (pDevConfig->clkMode << 6) | \
                                        (pDevConfig->transferMode << 8);

    pSpiCtrlRegs->BaudR = pDevConfig->frqDivider;	// run it approx @ 2MHz = 133/ 66
    pSpiCtrlRegs->IMR = 0;  				// disable all interrupts
    pSpiCtrlRegs->RxFtlR = 0;					// interrupt is asserted when 1 or more valid data entry is present
    pSpiCtrlRegs->SER = 0;					// disable individual slave chip select pin

    SPIEnable(pSpiCtrlRegs);

    //Clear FIFO
    while ((VUINT16)pSpiCtrlRegs->RxFlR != 0)
        tmp = (VUINT16)pSpiCtrlRegs->DR;

    // To select the device
    if(pDevConfig->CS != 0)
    {
        SPIEnableCS(pSpiCtrlRegs, pDevConfig->CS);
        pSpiCtx->SelectedCS = pDevConfig->CS;
    }

    return SPI_RC_OK;
}

/** @brief The function sets SPI clock mode

    @param mode [in] - the clock mode, please see SPI doc.

    @return [MXRC] the error code */

MXRC SPISetClockMode(UINT32 mode)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;

    _ASSERT_PTR(pSpiCtx);

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    if(pSpiCtx->bInited == FALSE)
        return SPI_RC_NOT_INITED;

    SPIDisable(pSpiCtrlRegs);
    pSpiCtrlRegs->CtrlR0 = (pSpiCtrlRegs->CtrlR0 & ~(3 << 6)) | ((mode & 3) << 6);
    SPIEnable(pSpiCtrlRegs);

    return SPI_RC_OK;
}

MXRC SPISetSpecMode(UINT32 mode)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;

    _ASSERT_PTR(pSpiCtx);

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    if(pSpiCtx->bInited == FALSE)
        return SPI_RC_NOT_INITED;

    SPIDisable(pSpiCtrlRegs);
    pSpiCtrlRegs->CtrlR0 = (pSpiCtrlRegs->CtrlR0 & ~(3 << 6)) | mode;
    SPIEnable(pSpiCtrlRegs);

    return SPI_RC_OK;
}

/** @brief The function writes data to the device
           selected by SPISelectDevice function and
           reads (and return) than read data

    @param data [in] - the data to be sent to HW dev connected to SPI

    @return [UINT16] read data*/

UINT16 SPIWrite(UINT16 data)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;

    _ASSERT_PTR(pSpiCtx);

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    if(pSpiCtx->bInited == FALSE)
    {
        _ASSERT(pSpiCtx->bInited);
        return 0;
    }

    pSpiCtrlRegs->DR = data;
    while ((UINT16)pSpiCtrlRegs->RxFlR == 0);
    data = pSpiCtrlRegs->DR;

    return data;
}

/** @brief The function reads data from the device
           selected by SPISelectDevice function

    @return [UINT16] read data */

UINT16 SPIRead(void)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;
    UINT16 out;

    _ASSERT_PTR(pSpiCtx);

    if(pSpiCtx->bInited == FALSE)
    {
        _ASSERT(pSpiCtx->bInited);
        return 0;
    }

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    pSpiCtrlRegs->DR= 0xFF;
    while ((UINT16)pSpiCtrlRegs->RxFlR == 0);
    out = pSpiCtrlRegs->DR;

    return out;
}

#ifdef USE_GPIO_CTRL_CS

/** @brief The function writes one data byte to the
           SPI device selected by SPISelectDevice function

    @param Data [in] - the data to be written

    @return [MXRC] the error code */

MXRC SPISendByte(UINT8 Data)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;

    _ASSERT_PTR(pSpiCtx);

    if(pSpiCtx->bInited == FALSE)
        return SPI_RC_NOT_INITED;

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    _ASSERT_PTR(pSpiCtrlRegs);

    pSpiCtrlRegs->DR = Data;
    while ((UINT16)pSpiCtrlRegs->RxFlR == 0);
    Data = (UINT8)pSpiCtrlRegs->DR;

    return SPI_RC_OK;
}

/** @brief The function sends and reads one data byte to/from the
           SPI device selected by SPISelectDevice function

    @param In   [in] - the data to be written
    @param pOut [out]- the data to be read

    @return [MXRC] the error code */

MXRC SPISendReadByte(UINT8 In, UINT8* pOut)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;

    _ASSERT_PTR(pSpiCtx);

    if(pSpiCtx->bInited == FALSE)
        return SPI_RC_NOT_INITED;

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    _ASSERT_PTR(pSpiCtrlRegs);

    pSpiCtrlRegs->DR = In;
    while ((UINT16)pSpiCtrlRegs->RxFlR == 0);
    *pOut = (UINT8)pSpiCtrlRegs->DR;

    return SPI_RC_OK;
}

/** @brief The function reads data from the device
           selected by SPISelectDevice function

    @param pOut [out]- the data to be read

    @return [MXRC] the error code */

MXRC SPIRecvWord(UINT16* pOut)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;

    _ASSERT_PTR(pSpiCtx);
    _ASSERT_PTR(pOut);

    if(pSpiCtx->bInited == FALSE)
        return SPI_RC_NOT_INITED;

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    pSpiCtrlRegs->DR= 0xFF;
    while ((UINT16)pSpiCtrlRegs->RxFlR == 0);
    *pOut = pSpiCtrlRegs->DR;

    return SPI_RC_OK;
}


#endif

/** @brief The function writes data to the
           SPI device selected by SPISelectDevice function

    @param CmdID    [in] - the command ID
    @param pParams  [in] - the pointer to the buffer
    @param nParCount[in] - the number of parameters

    @return [MXRC] the error code */

MXRC SPIWriteCmd (UINT16 CmdID, UINT16 * pParams, UINT32 nParCount)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;
    VUINT16 value, i;

    _ASSERT_PTR(pSpiCtx);

    if(pSpiCtx->bInited == FALSE)
        return SPI_RC_NOT_INITED;

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    _ASSERT_PTR(pSpiCtrlRegs);

    pSpiCtrlRegs->DR = CmdID;
    while ((VUINT16)pSpiCtrlRegs->RxFlR == 0);
    value = pSpiCtrlRegs->DR;

    for (i = 0; i < nParCount; i++)
    {
        pSpiCtrlRegs->DR = pParams[i];
        while ((VUINT16)pSpiCtrlRegs->RxFlR == 0);
        value = pSpiCtrlRegs->DR;
    }

    return SPI_RC_OK;
}

/** @brief The function reads data from the
           SPI device selected by SPISelectDevice function

    @param CmdID    [in] - the command ID
    @param pParams  [out]- the pointer to the buffer
    @param nParCount[in] - the number of parameters

    @return [MXRC] the error code */

MXRC SPIReadCmd (UINT16 CmdID, UINT16 * pParams, UINT32 nParCount)
{
    PSPI_CTX pSpiCtx = SpiGetCtx();
    SPI_CTRL_REGS *pSpiCtrlRegs;
    VUINT16 value, i;

    _ASSERT_PTR(pSpiCtx);

    if(pSpiCtx->bInited == FALSE)
        return SPI_RC_NOT_INITED;

    pSpiCtrlRegs = pSpiCtx->SpiCtrlRegs;

    _ASSERT_PTR(pSpiCtrlRegs);

    pSpiCtrlRegs->DR = CmdID;
    while ((VUINT16)pSpiCtrlRegs->RxFlR == 0);
    value = pSpiCtrlRegs->DR;

    for (i = 0; i < nParCount; i++)
    {
        pSpiCtrlRegs->DR = i;
        while ((VUINT16)pSpiCtrlRegs->RxFlR == 0);
        pParams[i] = pSpiCtrlRegs->DR;
    }

    return SPI_RC_OK;
}

/******************************************************************
*       Just the 4GMX SDI to be compatible with drivers table     *
*******************************************************************/

MXRC SPIDrv_init(UINT32 nInit, LPVOID * pDrvCtx)
{
    //return SPIInit(nInit);
    return SPI_RC_OK;
}

void SpiDrvMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    // SPI registers + GPIO registers accessed for CORE-0/1
    //-----------------------------------------------------

    if (nCpuID < 1)
    {
        MmuMgrCfgMemAP(SPI_CTRL_REGS_BASE, 4*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
        MmuMgrCfgMemAP(GPIO_APB_BASEADDR,  4*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
    }
}
