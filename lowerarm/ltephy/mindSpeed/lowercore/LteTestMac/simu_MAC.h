/** @file MAC.h
 *
 * @brief Check API processing logic
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _MAC_H
#define _MAC_H


#include "basetypes.h"


#define DTCH 0

typedef int (*MacRlcIoFunc) (U16 *LogicChanID, U8** pBuf, U16 BlockSize);

// Called by RLC when a corresponding RLC link is created. Return LCID
RESULTCODE LteMacCreateLogicChannel(U16 LogicChanType, MacRlcIoFunc LteMacGetDlDataBlock, MacRlcIoFunc LteMacPutUlDataBlock);

// Called by RLC when a corresponding RLC link is destroyed. Return destroy status
RESULTCODE LteMacDestroyLogicChannel(U16 LogicChanId);

// This function is to process the received uplink Transport Block from a UE
void LteMacUpLinkTBProcessing(U8* pMacPdu);

#endif
