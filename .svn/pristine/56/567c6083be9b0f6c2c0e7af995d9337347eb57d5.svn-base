/** @file LteMacRlcInterface.h
 *
 * @brief Header file to
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _LTE_UE_MAC_RLC_INTERFACE_H
#define _LTE_UE_MAC_RLC_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "resultcodes.h"
#include "basetypes.h"
#include "LtePhyL2Api.h"

#ifdef EHDMAC
#include "lteLayer2MacRlcInterface.h"
#else


/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define LCID_MAX_LCHAN              0x0A

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*typedefs for data transfer between MAC and RLC */
typedef struct RLCUplinkDataInfoT
{
    U8 *rlcdataBuffer; /*pointer to RLC data*/
    U32 dataLength; /*length of RLC data*/
    U8 lcId; /*LC id for which data is present*/
} RLCUplinkDataInfo;

typedef struct RLCUplinkDataT
{
    RLCUplinkDataInfo rlcDataArr[LCID_MAX_LCHAN - 1]; /*each index indicate a lc id*/
    U8 numLCInfo; /*number of logical channels info present in rlcDataArr */
} RLCUplinkData;
typedef struct RLCDataBufferT {
	U8   lcID;            /* Logical Channel ID */
	U16  seqNumber;
	U8   * rlcBuffer_p;   /* RLC PDU pointer */
	U32  bufferLength;    /* RLC PDU buffer length returned*/
	U32  requestedDataSize;    /* RLC PDU buffer length requested*/
    U32 remainingDataSize; /* Remaining Queue load for logical channel */
} RLCDataBuffer;

#endif

typedef struct stRLC_CONTEXT
{
    RLCUplinkData RlcDataTransport[2];
    RLCUplinkData *Current;
    RLCUplinkData *Next;
    OSAL_MUTEX CurrentLock;
    U8 ToRlcDataBusy;
    OSAL_SEMAPHORE LteRlcTxSema;
    U16 UeIdx;
    U8 RlcActive;
} RlcContext, *PRlcContext;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
extern OSAL_QUEUE RlcAmQueue;

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
RESULTCODE RlcDataAdd(U16 lcid, U8* pMacPdu, U32 BlockLen);
RESULTCODE RlcDataRcv(U16 lcid, PTR pBuf, OSAL_SIZE blockSize);

void RlcTxStart(void);
int QueueRlcToMac(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize);
U32 RlcGetBufferLength(PTR buf);
U32 RlcGetBufferlcID(PTR buf);
RESULTCODE LteMacRxEthPacket(PTR PacketPtr, U32 Len);

#endif /* _LTE_UE_MAC_RLC_INTERFACE_H */
