/** @file idma.h
 *
 * @brief IDMA/MDMA/EDMA module definitions and interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef IDMA_H_
#define IDMA_H_

#include "resultcodes.h"

//
// Limits
//
#define IDMA_MAX_BDESC                  6

//
// FControl masks (IdmaDesc.control)
//
#define IDMA_FCONTROL_FREADY            0x00000001
#define IDMA_FCONTROL_FLAST             0x00000002
#define IDMA_FCONTROL_IRQEN             0x00000004
#define IDMA_FCONTROL_SCATTER           0x00000008
#define IDMA_FCONTROL_CRCFLIP           0x00000010      // Bit7 is LSB, Bit0 is MSB (outbound)
#define IDMA_FCONTROL_CRCEND            0x00000020      // BigEndian (inbound)
#define IDMA_FCONTROL_CRCINIT0          0x00000000      // Bit5 && Bit6 == 0 (outbound)
#define IDMA_FCONTROL_CRCINIT1          0x00000020      // All 1's (outbound)
#define IDMA_FCONTROL_CRCINITSYS        0x00000040
#define IDMA_FCONTROL_CRC32             0x00000080
#define IDMA_FCONTROL_DMACTRL           0x00000100      // Mem to FIFO only

// ???
#define IDMA_FCONTROL_PHY_NB_MASK       0x00000070
#define IDMA_FCONTROL_PHY_OFFSET        0x00000004

//
// BControl masks (BufferDesc.control)
//
#define IDMA_BCONTROL_BLAST             0x00010000
#define IDMA_BCONTROL_SCATTER_BLAST     0x80000000
#define IDMA_BCONTROL_BLEN_MASK         0x0000FFFF

//
// FStatus masks (IdmaDesc.status)
//
#define IDMA_FSTATUS_FDONE              0x80000000
#define IDMA_FSTATUS_BIT27              0x08000000
#define IDMA_FSTATUS_FKEEP              0x04000000

#define IDMA_START                      0x00000001

//
// Flags used in IdmaInit, IdmaStart
//
#define IDMA_EDMA0                      0x01    // GEMAC0
#define IDMA_EDMA1                      0x02    // GEMAC1
#define IDMA_MDMAIN                     0x04    // Inbound  (FIFO to Mem)
#define IDMA_MDMAOUT                    0x08    // Outbound (Mem to FIFO)

#if 0
typedef struct MdmaRegisters
{
    V32 fifoThreshold;
    U32 unused0[63];                    // Skip to offset 0x100
    V32 outStart;                       // Mem to FIFO start
    V32 *outHead;                       // Mem to FIFO head pointer
    V32 outLockedSize;                  // Mem to FIFO locked transfer size (max 255)
    U32 unused1[5];                     // Skip to offset 0x120
    V32 outSoftReset;                   // Mem to FIFO soft reset
    U32 unused2[23];                    // Skip to offset 0x180
    V32 inStart;                        // FIFO to Mem start
    V32 *inHead;                        // FIFO to Mem head pointer
    V32 inLockedSize;                   // FIFO to Mem locked transfer size (max 255)
    U32 unused3[5];                     // Skip to offset 0x1A0
    V32 inSoftReset;                    // FIFO to Mem soft reset
} MDMA_REG;

typedef struct HdmaRegisters
{
    V32 softReset;
    U32 unused0[2];                     // Skip to offset 0x00C
    V32 rxDmaEnable;                    // MMHI
    V32 txDmaEnable;                    // HIMM
    U32 unused1[11];                    // Skip to offset 0x040
    V32 rxBaseAddr;                     //
    V32 txBaseAddr;                     //
    V32 rxBlockSize;                    //
    V32 txBlockSize;                    //
    V32 lockedSize;                     // Both TX & RX
} HDMA_REG;

typedef struct EdmaRegisters
{
    V32 txStart;                        // Base offset is 0x100
    V32 *txHead;                        // Head pointer
    V32 txThreshold;                    // Lock and threshold
    V32 txFrameLength;                  //
    V32 txControl;                      //
    V32 txStatus;                       //
    U32 unused0[2];                     // Skip to offset 0x120
    V32 txSoftReset;                    //
    U32 unused1[23];                    // Skip to offset 0x180
    V32 rxStart;                        // TODO: Maybe split into separate entities?
    V32 *rxHead;                        // Head pointer
    V32 rxThreshold;                    // Lock and threshold
    V32 rxFrameLength;                  //
    V32 rxControl;                      //
    V32 rxStatus;                       //
    U32 unused2[2];                     // Skip to offset 0x190
    V32 rxSoftReset;                    //
} EDMA_REG;
#endif /* 0 */

typedef struct BufferDesc
{
    U32 *pointer;
    U32 control;
} BUFFER_DESC;

typedef struct IdmaDesc
{
    struct IdmaDesc *next;
    U32 system;
    U32 status;
    U32 control;
    BUFFER_DESC descr[IDMA_MAX_BDESC];
} IDMA_DESC;

RESULTCODE IdmaInit(U32 cpuid, U32 idmaMask);
RESULTCODE IdmaStart(U32 idmaMask);
RESULTCODE IdmaStop(U32 idmaMask);

void Edma0RxHandler(void);
void Edma0TxHandler(void);
void Edma1RxHandler(void);
void Edma1TxHandler(void);

#endif  // IDMA_H_
