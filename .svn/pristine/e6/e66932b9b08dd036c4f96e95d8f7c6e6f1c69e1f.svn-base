//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL i2cdrv.h
 *
 * @brief The I2C driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
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

#ifndef __I2CDRV_H__
#define __I2CDRV_H__

#include "syscore.h"
#include "appids.h"

/********************************************************************
 *   The module constants                                           *
 ********************************************************************/
#define I2C_CLKDIV  0x4d
#define I2C_BUSENA  0x3     // According to the specification: IO Control Register[1:0] - enable/disable I2C

/********************************************************************
*   The list of module error codes                                  *
********************************************************************/
#define I2C_RC_OK               MX_OK
#define I2C_RC_TIMEOUT          DEF_USRRC(APP_DRIVER, I2C_DRV, 1)
#define I2C_RC_ERROR            DEF_USRRC(APP_DRIVER, I2C_DRV, 2)
#define I2C_RC_EMPTY            DEF_USRRC(APP_DRIVER, I2C_DRV, 3)
#define I2C_RC_SET_MASTER_ERROR DEF_USRRC(APP_DRIVER, I2C_DRV, 4)
#define I2C_RC_STATUS_ERROR     DEF_USRRC(APP_DRIVER, I2C_DRV, 5)
/********************************************************************
*   CNTR - Control register bits                                    *
********************************************************************/

#define I2C_IEN                             (1<<7)
#define I2C_ENAB                            (1<<6)
#define I2C_STA                             (1<<5)
#define I2C_STP                             (1<<4)
#define I2C_IFLG                            (1<<3)
#define I2C_AAK                             (1<<2)

#define I2C_EEPROM_OPCODE                   0xA0    //[1010=atmel dev] [000] [r/!w], the chip select are at 000
#define I2C_PCA_OPCODE                      0x40    //fixed device address

#define I2C_MAX_TIMEOUT                     1000    //an emprical value, large to be conservative
#define I2C_WRITE                           0
#define I2C_READ                            1
#define START_MASTER                        (1<<5)

/* STAT - Status codes */
#define I2C_BUS_ERROR                       0x00    /* Bus error in master mode only */
#define I2C_START_TRANSMIT                  0x08    /* Start condition transmitted */
#define I2C_REPEAT_START_TRANSMIT           0x10    /* Repeated Start condition transmited */
#define I2C_ADDRESS_W_ACK                   0x18    /* Address + Write bit transmitted, ACK received */
#define I2C_ADDRESS_W_NACK                  0x20    /* Address + Write bit transmitted, NACK received */
#define I2C_DATA_TRANSMIT_ACK               0x28    /* Data byte transmitted in master mode , ACK received */
#define I2C_DATA_TRANSMIT_NACK              0x30    /* Data byte transmitted in master mode , NACK received */
#define I2C_ARBIT_LOST                      0x38    /* Arbitration lost in address or data byte */
#define I2C_ADDRESS_R_ACK                   0x40    /* Address + Read bit transmitted, ACK received  */
#define I2C_ADDRESS_R_NACK                  0x48    /* Address + Read bit transmitted, NACK received  */
#define I2C_DATA_RECEIVE_ACK                0x50    /* Data byte received in master mode, ACK transmitted  */
#define I2C_DATA_RECEIVE_NACK               0x58    /* Data byte received in master mode, NACK transmitted*/
#define I2C_ARBIT_LOST_ADDRESS              0x68    /* Arbitration lost in address  */
#define I2C_GENERAL_CALL                    0x70    /* General Call, ACK transmitted */
#define I2C_NO_RELEVANT_INFO                0xF8    /* No relevant status information, IFLF=0 */

#define I2C_PAGE_SIZE                       32  // 32 byte in one page
#define I2C_MAX_SET_MASTER                  200


typedef struct _I2CInitType_
{
    UINT32 clkdiv;      // the clock divider (see specifications: CCRFS AND CCRH REGISTERS)
    UINT32 resetseq;    // the flag to generate reset seq for EEPROMs or not
    UINT32 ctrlreg;     // CNTR REGISTER
                        //  D7 IEN Interrupt enable
                        //  D6 ENAB Bus enable
                        //  D5 STA Master mode start
                        //  D4 STP Master mode stop
                        //  D3 IFLG Interrupt flag
                        //  D2 AAK Assert acknowledge
                        //  D1 0 Unused
                        //  D0 0 Unused

    UINT32 addrreg;     // Slave address, Extended Slave Address, General call address enable
                        //  D7 SLAA6 Slave Address 1
                        //  D6 SLAA5 Slave Address 1
                        //  D5 SLAA4 Slave Address 1
                        //  D4 SLAA3 Slave Address 1
                        //  D3 SLAA2 Slave Address 0
                        //  D2 SLAA1 Slave Address SLAX9 Extended Slave Address
                        //  D1 SLAA0 Slave Address SLAX8 Extended Slave Address
                        //  D0 GCE General call address enable GCE General call address enable

    UINT32 xaddrreg;    // Extended Slave Address
                        //  D7 SLAX7 Extended Slave Address
                        //  D6 SLAX6 Extended Slave Address
                        //  D5 SLAX5 Extended Slave Address
                        //  D4 SLAX4 Extended Slave Address
                        //  D3 SLAX3 Extended Slave Address
                        //  D2 SLAX2 Extended Slave Address
                        //  D1 SLAX1 Extended Slave Address
                        //  D0 SLAX0 Extended Slave Address

}I2CINIT, *PI2CINIT;

/** @brief This function enables I2C device and setup
           dividers to the default value: 0x4d (M=9, N=5)

    @return [MXRC] an error code */

MXRC I2CInitHw(void);

/** @brief This function enables I2C device and setup dividers
    @param pInit  [in] - init data
    @return [MXRC] an error code */

MXRC I2CInitHwEx(PI2CINIT pInit);

/** @brief This function is designed to generate reset seq.
           for the connected EEPROM devices

    @return [MXRC] an error code */

MXRC I2CDrvResetDevice(void);

/** @brief This function waits for the specified FLAG, the time of
           waiting should be specified in ticks,
           150 ticks = 1 microseconds

    @param nFlagID [in] - flag id, see: I2C_xxx
    @param timeout [in] - the timeout

    @return [UINT32] status of flag */

UINT32 I2CWaitForFlag(UINT32 nFlagID, UINT32 timeout);

/** @brief This function waits for I-FLAG, the time of
           waiting is equal to 1ms

    @return [UINT32] status of I-FLAG*/

UINT32 I2CWaitForIFlag(void);

/** @brief This function waits for I-FLAG, the time of
           waiting should be specified in ticks,
           150 ticks = 1 microseconds

    @return [UINT32] status of I-FLAG*/

UINT32 I2CWaitForIFlagEx(UINT32 timeout);

/** @brief This function writes byte to be sent with using I2CStartXmit function
    @param Data [in] - byte of data
    @return [MXRC] an error code*/

MXRC I2CXmitByte(UINT8 Data);

/** @brief This function sends byte of data specified with
            I2CXmitByte function

    @return [MXRC] an error code */

MXRC I2CStartXmit(void);

/** @brief This function sends byte of data
    @return [MXRC] an error code */

MXRC I2CSendData(UINT8 Data);

/** @brief This function reads byte of data

    @param pData[out] - the pointer to the storage
    @return [MXRC] an error code */

MXRC I2CReadData (UINT8 * pData);

UINT32 I2CDrvWrite(UINT32 MfgID, PUINT8 pSrcData, UINT32 nDstAddr, UINT32 nLen);
UINT32 I2CDrvRead(UINT32 MfgID, PUINT8 target_address, UINT32 eeprom_address, UINT32 len);
UINT32 I2CDrvWriteBytes(UINT8 DeviceAddress, UINT8 RegisterAddress, PUINT8 pData, UINT16 RequestedLength);

/** @brief This function returns value of 8-bit STATUS register */
UINT8 I2CGetStatus(void);

/** @brief This function sends STOP condition
    @return [MXRC] an error code  */
MXRC I2CStopXmit(void);

/** @brief This function sets specified bit in Control register
    @param nBit - cpecified bit from number of bits defined in this file above in partition "CNTR - Control register bits"
    @return This function has no return value */
void I2CSetBit(UINT32 nBit);

/** @brief This function clears specified bit in Control register
    @param nBit - cpecified bit from number of bits defined in this file above in partition "CNTR - Control register bits"
    @return This function has no return value */
void I2CClearBit(UINT32 nBit);

#endif /*__I2CDRV_H__*/


#ifdef __cplusplus
}
#endif

