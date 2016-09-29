/** @file rrc_types.c
 *
 * @brief RRC definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _RRC_TYPES_H_
#define _RRC_TYPES_H_

//#include "rrc_extern_types.h"
#include "osal.h"
#include "apidefs.h"
#include "resultcodes.h"

#define RRC_CHANNEL_CONFIG_ERROR    0x1
#define RRC_UNKNOWN_API     0x2
#define RRC_UNKNOWN_LOGIC_CHANNEL   0x3
#define RRC_UNKNOWN_UE_INDEX    0x4

#define RRC_MSG_MAXSIZE		0x1000

#define DOWNLINK    1
#define UPLINK  2

extern OSAL_QUEUE qRrcToMac;
extern OSAL_QUEUE qRrcFromMac;
extern OSAL_SEMAPHORE LteMacUlSema;
extern OSAL_MAILBOX PhyRxMailBox;
extern OSAL_QUEUE bRrcToMac;
extern OSAL_MAILBOX bRrcFromMac;
extern OSAL_SEMAPHORE ToRRCSema;
extern OSAL_QUEUE ToRRCQueue;
extern OSAL_TASK RrcFromMacTaskId;
extern OSAL_SEMAPHORE FromMACSema;

typedef  enum _eRRC_Status
{
	Connected,
	Idle,

} eRRC_Status;

typedef struct _tRRCCtx_
{
	eRRC_Status	Status;
	U8 ueIdentity[5];
    U32 UeId;
	U32	establishmentCause;
	OSAL_HEAP hHeap;
	// to upper layer
	OSAL_QUEUE qToUpper;
} tRRCCtx, *PtRRCCtx;

#ifndef EHDMAC
/*structure for sending information to RRC*/
typedef struct rrcInfoDataT {
    U32 dataLen;
    U16 msgType; //CCCH_PDU or CRNTI passed to RRC layer from UL de-mux*/
    U16 ueIdx;
	U32 msgID;
    void *data_p;    /*pointer to the data to be*/
}rrcInfoData;
#endif /* !EHDMAC */

U32 RRC_Ctx_Init(HANDLE pHeap);

void RRC_Destroy(void);
RESULTCODE RRC_ConnectSetup(void* pParam, U16 ParamLen);
RESULTCODE RRC_DebugData(void* pParam, U16 ParamLen);
RESULTCODE RRC_ConnectRequest(void* msg, void* pParam, U16 ParamLen);

int QueueRrcToMac(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize);
int QueueMacToRrc(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize);
U32 RrcGetBufferLength(PTR buf);
U32 RrcGetBufferlcID(PTR buf);

#endif /*_RRC_TYPES_H_*/
