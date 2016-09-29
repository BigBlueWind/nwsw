//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL spidrv.h
 *
 * @brief The SPI driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __SPIDRV_H__
#define __SPIDRV_H__

#include "config.h"
#include "appids.h"
#include "4gmx.h"


#define SPI_RC_OK                   MX_OK
#define SPI_RC_UNKNOWN_DEV          DEF_USRRC(APP_DRIVER, SPI_DRV, 1)
#define SPI_RC_NOT_INITED           DEF_USRRC(APP_DRIVER, SPI_DRV, 2)
#define SPI_RC_PARAM_ERROR          DEF_USRRC(APP_DRIVER, SPI_DRV, 3)
#define SPI_RC_TIMEOUT              DEF_USRRC(APP_DRIVER, SPI_DRV, 4)
#define SPI_RC_INIT_ERROR           DEF_USRRC(APP_DRIVER, SPI_DRV, 5)
#define SPI_RC_NOT_OWNER            DEF_USRRC(APP_DRIVER, SPI_DRV, 6)
#define SPI_RC_NOT_SUPPORTED        DEF_USRRC(APP_DRIVER, SPI_DRV, 7)
#define SPI_RC_NUM_DEV_ERROR        DEF_USRRC(APP_DRIVER, SPI_DRV, 8)
#define SPI_RC_CALL_SHUTDOWN        DEF_USRRC(APP_DRIVER, SPI_DRV, 9)
#define SPI_RC_WRITE_ERROR          DEF_USRRC(APP_DRIVER, SPI_DRV, 10)
#define SPI_RC_READ_ERROR           DEF_USRRC(APP_DRIVER, SPI_DRV, 11)

/****************************************************
*  The list of devices' CS connected to SPI busses	*
****************************************************/

#define SPI_TEST_EPROM 1

#define USE_GPIO_CTRL_CS

#define SPI_FRAME_SIZE_8BITS	0x07
#define SPI_FRAME_SIZE_16BITS	0x0F

#define SPI_MOTOROLA_FORMAT 	0x00

#define SPI_CLOCK_MODE0			0x00
#define SPI_CLOCK_MODE1			0x01
#define SPI_CLOCK_MODE2			0x02
#define SPI_CLOCK_MODE3			0x03

#define SPI_RX_TX_ENABLE		0x00

//#define SPI_DIVIDER_SPEED_1M	200
#define SPI_DIVIDER_SPEED_1M    150
#define SPI_DIVIDER_SPEED_KHZ(x)  ((UINT32)(150000UL/(UINT32)x))
#define SPI_DIVIDER_SPEED_MHZ(x)  ((UINT32)(150UL/(UINT32)x))


#define SPI_FIFO_SIZE			8		//Only 8 words of 16 bits width
#define SPI_COMMAND_MAX_SIZE   	8

enum {
    SPI_DEV_RAD0 = 0,
    SPI_DEV_RAD1,
    SPI_DEV_DEJITTER,
    SPI_DEV_EPROM,
    SPI_DEV_MAX = 32
};

#ifdef USE_GPIO_CTRL_CS
#define CS_HIGH(GPIO)		REG32(GPIO_OUTPUT_REG) |= (1<<GPIO)
#define CS_LOW(GPIO)		REG32(GPIO_OUTPUT_REG) &= ~(1<<GPIO)
#endif

#if SPI_TEST_EPROM
#define AT25DF161_PAGE_SIZE		256
#define AT25DF161_TOTAL_SIZE	2*1024*1024	//16M Bits
#define AT25DF161_SECTOR_SIZE	64*1024
#define AT25DF161_TOTAL_SECTOR	32

/*************** command define******************/
//Read Commmans
#define cmd_RD_DATA				0X03
#define cmd_FAST_RD				0X0B
//Program and Erase Commands
#define cmd_SECTOR_ERASE		0XD8	//BLOCK 64K
#define cmd_CHIP_ERASE			0XC7
#define cmd_PAGE_PROGRAM		0X02
//Protection Commands
#define cmd_WR_ENABLE			0X06	// write enable command
#define cmd_WR_DISABLE			0X04	// write disable command
//Status Commands
#define cmd_RD_STATUS			0X05	// read status register command
#define cmd_WD_STATUS			0X01	// write status register command
//Miscellaneous Commands
#define cmd_ID					0X9F
#define cmd_POWER_DOWN			0XB9
#define cmd_RELEASE_POWER_DOWN	0XAB

/**************** DEVICE ID ***********************/
#define ST25P_MANUFACTURER_ID   0X20
#define ST25P16_TYPE_ID			0X20
#define ST25P16_CAPACITY_ID		0X15
#define ST25P16_ID				((ST25P16_TYPE_ID<<8) | ST25P16_CAPACITY_ID)

#define DUMMY_DATA				0XFF
/************************** PIN define **************/
#define EEPROM_CS_GPIO 		24
#define EPROM_CS_HIGH		CS_HIGH(EEPROM_CS_GPIO)
#define EPROM_CS_LOW		CS_LOW(EEPROM_CS_GPIO)
/**** status register bit define ******/
#define SRWD 				0X80
#define BP2					0X10
#define BP1					0X08
#define BP0					0X04
#define WEL					0X02
#define BUSY				0X01

#define PROTECT_BITS    	(BP2|BP1|BP0)

typedef struct tagAT25ID{
    UINT8 	ManufacturerID;
    UINT8	DeviceID1;
    UINT8	DeviceID2;
}AT25ID;
#endif


#define SPI_CS_ALL (SPI_RAD0 | SPI_RAD1 | SPI_DEJITTER_CS | SPI_EPROM_CS)

#define SPI_CTRL_REGS_BASE  SPI_BASEADDR

#define SPI_CTRLR0_REG      (SPI_BASEADDR+0x00)
#define SPI_CTRLR1_REG      (SPI_BASEADDR+0x04)
#define SPI_SSIENR_REG      (SPI_BASEADDR+0x08)
#define SPI_MWCR_REG        (SPI_BASEADDR+0x0c)
#define SPI_SER_REG         (SPI_BASEADDR+0x10)
#define SPI_BAUDR_REG       (SPI_BASEADDR+0x14)
#define SPI_TXFTLR_REG      (SPI_BASEADDR+0x18)
#define SPI_RXFTLR_REG      (SPI_BASEADDR+0x1c)
#define SPI_TXFLR_REG       (SPI_BASEADDR+0x20)
#define SPI_RXFLR_REG       (SPI_BASEADDR+0x24)
#define SPI_SR_REG          (SPI_BASEADDR+0x28)
#define SPI_IMR_REG         (SPI_BASEADDR+0x2c)
#define SPI_ISR_REG         (SPI_BASEADDR+0x30)
#define SPI_RISR_REG        (SPI_BASEADDR+0x34)
#define SPI_TXOICR_REG      (SPI_BASEADDR+0x38)
#define SPI_RXOICR_REG      (SPI_BASEADDR+0x3c)
#define SPI_RXUICR_REG      (SPI_BASEADDR+0x40)
#define SPI_MSTICR_REG      (SPI_BASEADDR+0x44)
#define SPI_ICR_REG         (SPI_BASEADDR+0x48)
#define SPI_IDR_REG         (SPI_BASEADDR+0x58)
#define SPI_DR_REG          (SPI_BASEADDR+0x60)

typedef struct tagSPI_CTRL_REGS
{
    VUINT32 CtrlR0;         // offset 0x00
    VUINT32 CtrlR1;         // offset 0x04
    VUINT32 SsiEnR;         // offset 0x08
    VUINT32 MwCR;           // offset 0x0C
    VUINT32 SER;            // offset 0x10
    VUINT32 BaudR;          // offset 0x14
    VUINT32 TxFtlR;         // offset 0x18
    VUINT32 RxFtlR;         // offset 0x1C
    VUINT32 TxFlR;          // offset 0x20
    VUINT32 RxFlR;          // offset 0x24
    VUINT32 SR;             // offset 0x28
    VUINT32 IMR;            // offset 0x2C
    VUINT32 ISR;            // offset 0x30
    VUINT32 RISR;           // offset 0x34
    VUINT32 TxIOCR;         // offset 0x38
    VUINT32 RxIOoCR;        // offset 0x3C
    VUINT32 RxUICR;         // offset 0x40
    VUINT32 MstICR;         // offset 0x44
    VUINT32 ICR;            // offset 0x48
    VUINT32 rsrv1;          // offset 0x4C
    VUINT32 rsrv2;          // offset 0x50
    VUINT32 rsrv3;          // offset 0x54
    VUINT32 IDR;            // offset 0x58
    VUINT32 rsrv4;          // offset 0x5C
    VUINT16 DR;             // offset 0x60-0x9C
    VUINT16 DRspace[31];
}SPI_CTRL_REGS, *PSPI_CTRL_REGS;

/** @brief It should contain all the specific configuration
           for using of particular device on SPI bus */

typedef struct tagSPICONFIG
{
    UINT32  DevID;              // some unique DevID (like index 0,1,2...)
    UINT32  CS;                 // CS value for this DevID

    UINT32    frameSize;        /*3:0 - 0x07: 8bits             */
    UINT32    frameFormat;      /*5:4 - 0x0: Motorola SPI       */
    UINT32    clkMode;          /*7:6 - 0x03 Start at first edge, high when inactive*/
    UINT32    transferMode;     /*9:8 - 0x0: rx&tx              */
    UINT32    frqDivider;

    UINT32    bOpened;          // internally used

} SPICONFIG, *PSPICONFIG;

typedef struct tagSPI_CTX
{
    SPI_CTRL_REGS *SpiCtrlRegs;

    BOOL            bInited;
    VUINT32         bHwBusy;
    VUINT32         CpuID;          // this value is used to detect CPU ID where driver was started

    UINT32          NumDevs;

    HANDLE          SpiPart;
    PUINT32         SpiBuf;
    UINT32          SelectedCS;
    HANDLE          hHeap;
    UINT32          GpioPins;

    void (*IrqProc) (void*);
    void * IrqProcData;

}SPI_CTX, *PSPI_CTX;

/******************************************************************
*       Just the 4GMX SDI to be compatible with drivers table     *
*******************************************************************/

MXRC SPIDrv_init(UINT32 nInit, LPVOID * pDrvCtx);

INLINE MXRC SPIDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                    UINT32 nCfgDataSize) { return SPI_RC_NOT_SUPPORTED; }

INLINE MXRC SPIDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 nCmdDataSize) { return SPI_RC_NOT_SUPPORTED; }

INLINE MXRC SPIDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                  UINT32 * pnCmdDataSize) { return SPI_RC_NOT_SUPPORTED; }

/******************************************************************
*       The SPI Driver interface                                  *
*******************************************************************/

INLINE void SPIDisable(SPI_CTRL_REGS * pSpiCtrlRegs)
{
    pSpiCtrlRegs->SsiEnR = 0x0;
}

INLINE void SPIEnable(SPI_CTRL_REGS * pSpiCtrlRegs)
{
    pSpiCtrlRegs->SsiEnR = 0x1;
}

INLINE void SPIEnableCS(SPI_CTRL_REGS * pSpiCtrlRegs, UINT32 cs)
{
    SPIDisable(pSpiCtrlRegs);
    pSpiCtrlRegs->SER |= cs;
    SPIEnable(pSpiCtrlRegs);
}

INLINE SPI_CTRL_REGS * SPIGetCtrlRegs (void)
{
    extern SPI_CTX gSpiCtx;

    _ASSERT(gSpiCtx.bInited != 0);
    _ASSERT_PTR(gSpiCtx.SpiCtrlRegs);

    return gSpiCtx.SpiCtrlRegs;
}

INLINE void SPIDisableCS(SPI_CTRL_REGS * pSpiCtrlRegs, UINT32 cs)
{
    SPIDisable(pSpiCtrlRegs);
    pSpiCtrlRegs->SER &= ~cs;
    SPIEnable(pSpiCtrlRegs);
}

/** @brief This function should be used to initialize/reinitialize the SPI driver
    @param NumDevs [in] - the number of devices, that can be llocated by the aplication
    @return [MXRC] an error code */

MXRC SPIInit(UINT32 NumDevs);

/** @brief This function returns status of driver initialization
    @return [BOOL] status */
BOOL SPIIsInit (void);

/** @brief This fnction shutdowns the driver
    @return [MXRC] an error code */

MXRC SPIShutdown(void);

/** @brief This function sets irq proc pointer to be called when
           SPI IRQ is processed

    @param Irq  [in] - the pointer the IRQ proc, it can be NULL
    @param pData[in] - the data

    @return [MXRC] an error code */

MXRC SPIDrvSetIrqProc (void(*Irq)(void*), void*pData);

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

PSPICONFIG SPICreateDevCtx(void);

/** @brief The function frees the device context (set of device parameters)
            that was allocated by SPICreateDevCtx function

    @return [MXRC] the error code */

MXRC SPIRemoveDevCtx(PSPICONFIG pConfig);

/** @brief The function setups device parameters, selects CS and prepares
            SPI HW device to work with HW device connected to SPI

    @param pDevConfig [in] - the pointer to the device context

    @return [MXRC] the error code */

MXRC SPISelectDevice(PSPICONFIG pDevConfig);

/** @brief The function sets SPI clock mode

    @param mode [in] - the clock mode, please see SPI doc.

    @return [MXRC] the error code */

MXRC SPISetClockMode(UINT32 mode);
MXRC SPISetSpecMode(UINT32 mode);

/** @brief The function writes data to the device
           selected by SPISelectDevice function and
           reads (and return) than read data

    @param data [in] - the data to be sent to HW dev connected to SPI

    @return [UINT16] read data*/

UINT16 SPIWrite(UINT16 data);

/** @brief The function reads data from the device
           selected by SPISelectDevice function

    @return [UINT16] read data */

UINT16 SPIRead(void);

#ifdef USE_GPIO_CTRL_CS

    /** @brief The function writes one data byte to the
           SPI device selected by SPISelectDevice function

    @param Data [in] - the data to be written

    @return [MXRC] the error code */

    MXRC SPISendByte(UINT8 Data);

    /** @brief The function sends and reads one data byte to/from the
           SPI device selected by SPISelectDevice function

    @param In   [in] - the data to be written
    @param pOut [out]- the data to be read

    @return [MXRC] the error code */

    MXRC SPISendReadByte(UINT8 In, UINT8* pOut);


    /** @brief The function reads data from the device
               selected by SPISelectDevice function

        @param pOut [out]- the data to be read

        @return [MXRC] the error code */

    MXRC SPIRecvWord(UINT16* pOut);

#endif

/** @brief The function writes data to the
           SPI device selected by SPISelectDevice function

    @param CmdID    [in] - the command ID
    @param pParams  [in] - the pointer to the buffer
    @param nParCount[in] - the number of parameters

    @return [MXRC] the error code */

MXRC SPIWriteCmd (UINT16 CmdID, UINT16 * pParams, UINT32 nParCount);

/** @brief The function reads data from the
           SPI device selected by SPISelectDevice function

    @param CmdID    [in] - the command ID
    @param pParams  [out]- the pointer to the buffer
    @param nParCount[in] - the number of parameters

    @return [MXRC] the error code */

MXRC SPIReadCmd (UINT16 CmdID, UINT16 * pParams, UINT32 nParCount);

void SpiDrvMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif

#ifdef __cplusplus
}
#endif

