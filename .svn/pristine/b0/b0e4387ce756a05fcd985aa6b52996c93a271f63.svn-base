/** @file gemac.h
 *
 * @brief GEMAC driver interfaces and definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef GEMAC_H_
#define GEMAC_H_

#include "resultcodes.h"
#include "idma.h"
// FIXME: RTXC
#if 0
#include <trsocket.h>
#endif

//
// Limits
//
#define GEM_MAX_RXQ                 1024
#define GEM_MAX_TXQ                 1024
#define GEM_MAX_FRAME               1536

typedef struct GemacFrame
{
    IDMA_DESC control;
    union
    {
        U32 data32[GEM_MAX_FRAME/sizeof(U32)];
        U8 data8[GEM_MAX_FRAME];
    } data;
} GEMAC_FRAME;

RESULTCODE GemacInit(U32 cpuid);

void Gemac0RxTask(PTR taskParm);
void Gemac0TxTask(PTR taskParm);
void Gemac1RxTask(PTR taskParm);
void Gemac1TxTask(PTR taskParm);

// GEMAC0 for now
RESULTCODE GemacWrite(U32 size, PTR data);
RESULTCODE GemacWriteBlocks(U32 blkNum, U32 size, PTR data, ...);

// FIXME: RTXC
#if 0
int GemacOpenFunc(ttUserInterface interfaceHandle);
int GemacCloseFunc(ttUserInterface interfaceHandle);
int GemacSendFunc(ttUserInterface interfaceHandle, char *dataPtr, int dataLength, int flag);
int GemacRecvFunc(ttUserInterface interfaceHandle, char TM_FAR **dataPtr, int TM_FAR *dataLength);
#endif /* 0 */

#endif  // GEMAC_H_
