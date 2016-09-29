/** @file phyiface.h
 *
 * @brief PHY layer interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef PHYIFACE_H_
#define PHYIFACE_H_

#include "osal.h"
#include "resultcodes.h"

extern PTR pIMSG_FROM_PHY;
extern PTR pIMSG_TO_PHY;

RESULTCODE PhyInit(void);
RESULTCODE PhyEnable(U16 *instID);
RESULTCODE PhyDone(void);

// PhySendMsg must copy data to the lower layer
RESULTCODE PhySendMsg(HANDLE h, PTR pMsg, OSAL_SIZE msgSize);
// PhyGetMsg must be the blocking call
RESULTCODE PhyGetMsg(HANDLE h, PTR pMsg, OSAL_SIZE *pMsgSize);

RESULTCODE PhyDispatch(U32 size, PTR msg);
// For MAC-MAC and other simulation modes
RESULTCODE PhyTransfer(U32 size, PTR msg);
// Send PHY API type of message to PHY
RESULTCODE PhySendToPhyMsg(U8 msgType, PTR data);

#endif /* PHYIFACE_H_ */
