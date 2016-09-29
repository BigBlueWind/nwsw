/** @file phi.h
 *
 * @brief PCI/PCIe Host Interface definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef PHI_H_
#define PHI_H_

#include "resultcodes.h"

//
// Limits
//
#define PHI_FIFO_SIZE32         128
#define PHI_FIFO_SIZE8          512

#define PHI_RXQ_SIZE            64
#define PHI_TXQ_SIZE            64

//
// PHI_INT_CTRL
//
#define PHI_RXDREQWE            0x0004  // RX FIFO Write Request Enable
#define PHI_TXDREQRE            0x0008  // TX FIFO Read Request Enable
#define PHI_TXCMPLTIE           0x0020  // TX Complete Interrupt Enable
#define PHI_TXFIE               0x0040  // TX FIFO Full Interrupt Enable
#define PHI_TXTHIE              0x0080  // TX FIFO Threshold Interrupt Enable
#define PHI_RXEEI               0x0100  // RX FIFO Empty Interrupt Enable
#define PHI_RXTHIE              0x0200  // RX FIFO Threshold Interrupt Enable
#define PHI_TXM3IE              0x0400  // TX MAIL3 Interrupt Enable
#define PHI_RXM3IE              0x0800  // RX MAIL3 Interrupt Enable
#define PHI_TXFF_RES            0x1000  // Reset pulse to TX_FIFO read & write
#define PHI_RXFF_RES            0x2000  // Reset pulse to RX_FIFO read & write
#define PHI_REQIOEN_N           0x4000  // Enable RX and TX through GPIO 6&7
#define PHI_RXCP_INH            0x8000  // Inhibits hardware RX Completed after DMA buffer empty condition
#define PHI_TFOVIE              0x10000 // TX FIFO Overflow Interrupt Enable
#define PHI_RFURIE              0x20000 // RX FIFO Under-run Interrupt Enable
#define PHI_TFURIE              0x40000 // TX FIFO Under-run Interrupt Enable
#define PHI_RFOVIE              0x80000 // RX FIFO Overflow Interrupt Enable

//
// PHI_INT_ACK
//
#define PHI_TXFIAK              0x0001
#define PHI_TXTHIAK             0x0002
#define PHI_TXM3IAK             0x0004
#define PHI_RXEIAK              0x0010
#define PHI_RXTHIAK             0x0020
#define PHI_RXM3IAK             0x0040
#define PHI_FLUSH_RX            0x0080
#define PHI_SRESET              0x0100  // Soft reset the entire device
#define PHI_TXCMPLTIAK          0x0200
#define PHI_TFOVIAK             0x10000
#define PHI_RFURIAK             0x20000
#define PHI_TFURIAK             0x40000
#define PHI_RFOVIAK             0x80000

//
// PHI_INT_STATUS
//
#define PHI_TXF                 0x01    // TX FIFO is full
#define PHI_TXTH                0x02    // TX FIFO depth is > than the threshold
#define PHI_RXE                 0x04    // RX FIFO is empty
#define PHI_RXTH                0x08    // RX FIFO depth is < than the threshold
#define PHI_RXM3I               0x10    // The Host has ack'd an RXM3F
#define PHI_TXM3I               0x20    // The Host has written to the TXM3
#define PHI_TXCMPLT             0x40    // TX COMPLETED bit set by Host
#define PHI_TFOV                0x10000 // TX FIFO overflow interrupt
#define PHI_RXUR                0x20000 // RX FIFO under-run interrupt
#define PHI_TFUR                0x40000 // TX FIFO under-run interrupt
#define PHI_RXOV                0x80000 // RX FIFO overflow interrupt

#ifdef M823XX
//
// Support for MSPDGWY driver
//
typedef struct PhiMessage
{
    U32 size;
    U16 mail0;
    U16 mail1;
    U16 mail2;
    U16 mail3;
    union
    {
        U32 fifo32[PHI_FIFO_SIZE32];
        U8 fifo8[PHI_FIFO_SIZE8];
    } fifo;
} PHI_MESSAGE;
#elif defined (C4KEVM)
typedef struct PhiMessage
{
    U32 size;
    U16 mail0;
    U16 mail1;
    U16 mail2;
    U16 mail3;
    union
    {
        U32 fifo32[PHI_FIFO_SIZE32];
        U8 fifo8[PHI_FIFO_SIZE8];
    } fifo;
} PHI_MESSAGE;
#else /* SIMUPC */
typedef struct PhiMessage
{
    U32 size;
    U16 mail0;
    U16 mail1;
    U16 mail2;
    U16 mail3;
    union
    {
        U32 fifo32[PHI_FIFO_SIZE32];
        U8 fifo8[PHI_FIFO_SIZE8];
    } fifo;
} PHI_MESSAGE;
#endif /*M823XX*/

RESULTCODE PhiInit(U32 cpuid);
RESULTCODE PhiWrite(U32 size, PTR data);

void PhiRxTask(void *param);
void PhiIntHandler(void);

#endif  // PHI_H_
