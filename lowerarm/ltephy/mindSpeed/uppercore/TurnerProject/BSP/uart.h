/** @file uart.h
 *
 * @brief UART interfaces and definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef UART_H_
#define UART_H_

#include "resultcodes.h"
#include "osal.h"

//
// Application limits
//
#define UART_RXQ_SIZE           256
#define UART_TXQ_SIZE           256

//
// Default baud rate for UART
//
#define UART_BAUD_RATE          115200

//
// Default FIFO depth (configurable?)
//
#define UART_FIFO_SIZE          16

//
// Line Control Register (LCR)
//
#define LCR_CHAR_LEN5           0x00     // 5 bits - default
#define LCR_CHAR_LEN6           0x01
#define LCR_CHAR_LEN7           0x02
#define LCR_CHAR_LEN8           0x03
#define LCR_ONE_STOP            0x00    // One stop bit - default
#define LCR_TWO_STOP            0x04    // 1.5 or 2 stop bits
#define LCR_PARITY_ENA          0x08    // Parity Enable
#define LCR_PARITY_EVEN         0x10    // Even Parity
#define LCR_BREAK_CTRL          0x40    // Transmit Break condition
#define LCR_DLAB                0x80    // Divisor Latch Access

//
// Line Status Register (LSR)
//
#define LSR_DR                  0x01    // Data Ready
#define LSR_OE                  0x02    // Overrun Error
#define LSR_PE                  0x04    // Parity Error
#define LSR_FE                  0x08    // Framing Error
#define LSR_BI                  0x10    // Break Interrupt
#define LSR_THRE                0x20    // Transmit Holding Register Empty
#define LSR_TEMT                0x40    // Transmitter Empty
#define LSR_RFE                 0x80    // Receiver FIFO Error

//
// FIFO Control Register
//
#define FCR_FIFOE               0x01    // FIFO Enable
#define FCR_RFIFOR              0x02    // Receiver FIFO Reset
#define FCR_XFIFOR              0x04    // Transmitter FIFO Reset
#define FCR_RXTRG14             0xC0    // RCVR trigger on FIFO 2 less than full

//
// Interrupt Enable Register
//
#define IER_ERBFI               0x01    // Enable Received Data Available Intr
#define IER_ETBEI               0x02    // Enable Transmit Holding Register Empty Intr

//
// Interrupt Identity Register
//
#define IIR_NOINT               0x01    // No interrupt pending
#define IIR_TX_EMPTY            0x02    // Transmitter Buffer Empty
#define IIR_RX_DATA             0x04    // Receiver Data Available

extern OSAL_SEMAPHORE Uart0RxSema;
extern OSAL_RINGBUF Uart0RxQueue;
extern OSAL_RINGBUF Uart0TxQueue;

#if 0
typedef struct UartRegisters
{
    union
    {
        V32 rbr;                // Receive Buffer Register
        V32 thr;                // Transmit Holding Register
        V32 dll;                // Divisor Latch (Low)
    };
    union
    {
        V32 dlh;                // Divisor Latch (High)
        V32 ier;                // Interrupt Enable Register
    };
    union
    {
        V32 iir;                // Interrupt Identification Register
        V32 fcr;                // FIFO Control Register
    };
    V32 lcr;                    // Line Control Register
    V32 mcr;                    // Modem Control Register
    V32 lsr;                    // Line Status Register
    V32 msr;                    // Modem Status Register
    V32 scr;                    // Scratchpad Register
    V32 lpdll;                  // Low Power Divisor Latch (Low) Register
    V32 lpdlh;                  // Low Power Divisor Latch (High) Register
    U32 unused0[2];             // Skip to offset 0x30
    union
    {
        V32 srbr;               // Shadow Receive Buffer Register
        V32 sthr;               // Shadow Transmit Holding Register
    };
    U32 unused1[15];            // Skip to offset 0x70
    V32 far;                    // FIFO Access Register
    V32 tfr;                    // Transmit FIFO Read
    V32 rfw;                    // Receive FIFO Write
    V32 usr;                    // UART Status Register
    V32 tfl;                    // Transmit FIFO Level
    V32 rfl;                    // Receive FIFO Level
    V32 srr;                    // Software Reset Register
    V32 srts;                   // Shadow Request to Send
    V32 sbcr;                   // Shadow Break Control Register
    V32 sdmam;                  // Shadow DMA Mode
    V32 sfe;                    // Shadow FIFO Enable
    V32 srt;                    // Shadow RCVR Trigger
    V32 stet;                   // Shadow TX Empty Trigger
    V32 htx;                    // Halt TX
    V32 dmasa;                  // DMA Software Acknowledge
    U32 unused2[18];            // Skip to offset 0xF4
    V32 cpr;                    // Component Parameter Register
    V32 ucv;                    // UART Component Version
    V32 ctr;                    // Component Type Register
} UART_REG;
#endif /* 0 */

RESULTCODE UartInit(U32 cpuid);
void UartIntHandler(void);
void Uart0RxTask(void *param);
void Uart1RxTask(void *param);
#ifdef USE_RTXC
void Uart0TxTask(void *param);
#endif /*USE_RTXC*/
RESULTCODE Uart0Write(U32 size, PTR data);
RESULTCODE Uart1Write(U32 size, PTR data);

#endif /*UART_H_*/
