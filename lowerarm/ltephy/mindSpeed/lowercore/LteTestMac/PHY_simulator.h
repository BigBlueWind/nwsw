/** @file phy_simulator.h
 *
 * @simulator for PHY layer
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef PHY_SIM_H
#define PHY_SIM_H

#include "osal.h"
#include "basetypes.h"
#include "ltemaccore.h"

#ifdef WIN32
typedef HANDLE key_t;
#endif

//for now, only a few big messages
#define MSG_MAXSIZE                 16384
#define MSG_MAXCOUNT                6

#define MAX_PHY_DATA_SIZE 10*1024
#define SHM_BUFFER_SIZE 	32*1024

enum
{
	SHM_Ready = 0,
	SHM_TXed,
	SHM_RXed,
	SHM_Init
};

enum
{
	SHM_SLAVE = 0,
	SHM_MASTER
};

typedef struct _PHY_PDU_HEADER
{
  int transactionFlag;
  int	length;
  int	sequenceNumber;	//debug purpose
  U32 stuffing[5];
}PHY_PDU_HEADER, *PPHY_PDU_HEADER;


typedef struct _PHY_CONTEXT
{
    U32 property;
    key_t TxSHM_key;
    key_t RxSHM_key;
    int TX_SHM_ID;
    int RX_SHM_ID;
    HANDLE TxBuffer;
    HANDLE RxBuffer;
    PPHY_PDU_HEADER TxHeader;
    PPHY_PDU_HEADER RxHeader;

    SUBFRDESC downlinkSubFrame;
    SUBFRDESC uplinkSubFrame;

    U8* toMAC_data;
    U8* fromMAC_data;
    U32 toMAC_dataLength;
    U32 fromMAC_dataLength;

    //for now, just to compile
    PSUBFRDESC pDlSubFrame;
    PSUBFRDESC pUlSubFrame;

    U32 TxSequenceNumber;
    U32 RxSequenceNumber;
    U32 TxBytesCount;
    U32 RxBytesCount;
} PHY_CONTEXT, *PPHY_CONTEXT;

RESULTCODE PhyInit(void);
void PhyDone(void);
RESULTCODE PhyTransmit(void);
RESULTCODE PhyReceive(void);
char PhyReadyToTransmit(void);
void PhyMarkReceiveDone(void);

#endif //PHY_SIM_H
