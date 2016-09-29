//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL UARTdrv.h
 *
 * @brief This file contains UART 16550 driver header
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
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

#ifndef _UART_DRV_H_
#define _UART_DRV_H_

#include "4gmx.h"
#include "appids.h"
#include "hal.h"
#include "clkrst.h"

/********************************************************************
**               The error codes of module                          *
*********************************************************************/

#define RC_UART_OK		        MX_OK
#define RC_UART_PARAM_ERROR	DEF_USRRC(APP_MODULE, UART_DRV, 1)
#define RC_UART_BUF_EMPTY	    DEF_USRRC(APP_MODULE, UART_DRV, 2)
#define RC_UART_TIMEOUT		DEF_USRRC(APP_MODULE, UART_DRV, 3)
#define RC_UART_ALLOC_ERR      DEF_USRRC(APP_MODULE, UART_DRV, 4)
#define RC_UART_ALLOC_TX_ERR   DEF_USRRC(APP_MODULE, UART_DRV, 5)
#define RC_UART_ALLOC_RX_ERR   DEF_USRRC(APP_MODULE, UART_DRV, 6)
#define RC_UART_CMD_ERR        DEF_USRRC(APP_MODULE, UART_DRV, 7)
#define RC_UART_UART_ID_ERR    DEF_USRRC(APP_MODULE, UART_DRV, 8)

#define UART_WAIT_CYCLE        200000

#define UART_BUSCLOCK          (ClkRstGetFreq (XP_AXI))

#define LCR_ONE_STOP        0x00    /* One stop bit! - default */
#define LCR_TWO_STOP        0x04    /* Two stop bit! */
#define LCR_PEN             0x08    /* Parity Enable */
#define LCR_PARITY_NONE     0x00
#define LCR_EPS             0x10    /* Even Parity Select */
#define LCR_PS              0x20    /* Enable Parity  Stuff */
#define LCR_SBRK            0x40    /* Start Break */
#define LCR_PSB             0x80    /* Parity Stuff Bit */
#define LCR_DLAB            0x80    /* UART 16550 Divisor Latch Assess */

/* UART 16550 FIFO Control Register */
#define FCR_FIFOEN          0x01
#define FCR_RCVRRES         0x02
#define FCR_XMITRES         0x04

/* Interrupt Enable Register */
/* UART 16550*/
#define IER_RXTH            0x01    /* Enable Received Data Available Interrupt*/
#define IER_TXTH            0x02    /* Enable Transmitter Empty Interrupt        */
/* Legacy UART*/
#define IER_ERDAI           0x01    /* Enable Rx Data Available Int */
#define IER_ETHREI          0x02    /* Enable THR Empty Int */
#define IER_RSI             0x04    /* Receiver Status Int */
#define IER_RSF             0x80    /* Receiver Status Flag */

/* Legacy UART Mode Control Register */
#define MCR_TX_ENABLE_MODE  0x80
#define MCR_RX_ENABLE_MODE  0x40
#define MCR_LOOPBACK        0x10

/* Legacy UART Serial Status Register */
#define SSR_BF              0x01    /* Buffer Full  */
#define SSR_OE              0x02    /* Overrun Error */
#define SSR_PE              0x04    /* Parity Error */
#define SSR_FR              0x08    /* Framing Error */
#define SSR_BI              0x10    /* Received Break Signal */
#define SSR_BE              0x20    /* Buffer Empty */
#define SSR_UR              0x40    /* Under-Run */
#define SSR_PAR             0x80    /* Parity Bit */

#define UART_DEFAULT_SPEED      115200
//#define UART_DEFAULT_SPEED      9600
#define UART_DEFAULT_BYTE_SIZE  UART_BYTE_LEN_8
#define UART_DEFAULT_PARITY     LCR_PARITY_NONE
#define UART_DEFAULT_STOP_BITS  LCR_ONE_STOP


#define UART_DEFAULT_RX_SIZE    100
#define UART_DEFAULT_TX_SIZE    100

typedef enum
{
    UART_BYTE_LEN_5	= 0x00,
    UART_BYTE_LEN_6	= 0x01,
    UART_BYTE_LEN_7	= 0x02,
    UART_BYTE_LEN_8	= 0x03

}UartByseSize;

typedef struct
{
    UINT32 Speed;	    /**< The speed of RX/TX	*/

    UINT8  ByteSize;	/**< see: UartByteSize	*/
    UINT8  StopBits;	/**< number of stop bits*/
    UINT8  Parity;	    /**< Parity 		*/

}UARTCfg, *PUARTCfg;

typedef struct
{
    UINT8 * Buf;

    UINT16  Put;
    UINT16  Get;
    UINT16  Size;

}CYCLBUF;

typedef struct
{
    UINT32   BaseAddr;  /**< The base address of UART device */
    UARTCfg  Config;	/**< UART device config parameters   */
    UINT32   bInited;

    VUINT32  Sync;

    CYCLBUF  Rx;         // The RX data is stored in this cyclic buffer
    CYCLBUF  Tx;         // The TX data is stored in this cyclic buffer

}UARTCtx, *PUARTCtx;

typedef enum _UARTCMD_
{
    UART_CMD_SET_SPEED,
    UART_CMD_SET_PARITY,
    UART_CMD_SET_STOPBITS,
    UART_CMD_SET_BYTESIZE,

    UART_CMD_PUT_STR,
    UART_CMD_PUT_CHAR,
    UART_CMD_GET_CHAR,

    UART_CMD_GET_IRQ_ID,
    UART_CMD_GET_IIR,

    UART_CMD_ENABLE_TX_EMPTY,
    UART_CMD_DISABLE_TX_EMPTY,
    UART_CMD_ENABLE_RX_READY,
    UART_CMD_DISABLE_RX_READY,

    UART_CMD_SET_LOOPBACK,
    UART_CMD_RESET_LOOPBACK,

    UART_CMD_GET_IER,
    UART_CMD_SET_IER,
    UART_CMD_IS_TX_IRQ_ENABLED,

    UART_CMD_NUM

} UARTCMD;

typedef struct
{
	unsigned int size    :2; // bits 0-1 - size
	unsigned int stop    :1; // bit 2 - stop
	unsigned int parity  :1; // bit 3 - parity
	unsigned int parsel  :1; // bit 4 - parity select: 1 - even, 0 - odd
	unsigned int speed   :17;// bits 5-21 - speed
	unsigned int EN      :1; // bit 22 - UART enabled/disabled: 1 - enabled, 0 - disabled
	unsigned int inited  :1; // bit 23 - whether UART has been inited by Linux. If 1 - Linux has inited UART
	unsigned int reserv  :8; // bits 24-31 - reserved
} UARTPARAMS, *PUARTPARAMS ;

LPVOID UARTGetCtx (UINT32 nDev);

MXRC UartDrvWriteStr (LPVOID pDrvCtx,  char * pStr);
BOOL UartDrvReadChar(LPVOID pDrvCtx , char *pCh);
BOOL UartDrvWriteChar(UARTCtx * pCtx, char ch, BOOL bWait);
void UartDrvAckIrq(UINT32 irq);

UINT32 UartDrvShutdown(UINT32 nDev);

void UartDrvShutdownAll(void);

MXRC UartDrvResetDevInt (UINT32 nDevID);

MXRC UartDrv_init(UINT32 nInit, LPVOID * pDrvCtx);

MXRC UartDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                    UINT32 nCfgDataSize);

MXRC UartDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 nCmdDataSize);

MXRC UartDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                  UINT32 * pnCmdDataSize);

void UartDrvLoadInitParams(LPVOID pUartInitParams);

// Returns number of UART using address of UART.
int UartDrvGetNum(UINT32 baseaddr);
UINT32 UartDrvGetBaseAddr(UINT32 index);
MXRC UartDrvFix(LPVOID pDrvCtx);

void UartDrvStartHoldPrinting(LPVOID pBuf, UINT32 bufsize);

MXRC UartDrvPrintBuf(void);

void UartDrvStopHoldPrinting(void);

void UartDrvReinitFreq(void);
void UartMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif /*_UART_DRV_H_*/

#ifdef __cplusplus
}
#endif

