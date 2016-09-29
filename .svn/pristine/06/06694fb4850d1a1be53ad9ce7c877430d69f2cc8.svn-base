/** @file maciface.h
 *
 * @brief Interface to WiMAX MAC or LTE L2/L3
 * @author Mindspeed Technologies
 * @version $Revision: 1.18 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef MACIFACE_H_
#define MACIFACE_H_

#include "resultcodes.h"
#include "osal.h"

#if !defined(MAC_SUCCESS)
#define MAC_SUCCESS                     SUCCESS
#endif

typedef RESULTCODE(*PhyTxCb)(HANDLE h,HANDLE pMsg,OSAL_SIZE msgSize);
typedef RESULTCODE(*PhyRxCb)(HANDLE h,PTR pMsg,OSAL_SIZE *pMsgSize);


//
// MAC-PHY Callbacks
//
RESULTCODE MacSetInitConfig(HANDLE h);
RESULTCODE MacInit(PhyTxCb txCb, PhyRxCb rxCb, HANDLE toPhy, HANDLE fromPhy);
RESULTCODE MacDispatch(U32 size, PTR msg);
RESULTCODE MacGetMsg(PTR pMsg);
RESULTCODE MacDone(void);

//
// Debugging purposes
//
// FIXME: Make single interface to all variants
RESULTCODE AppInit(OSAL_HEAP *heap);
RESULTCODE MacRxEthPacket(PTR pPacket, U32 Len);
#endif /* MACIFACE_H_ */
