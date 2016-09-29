/** @file msgroute.h
 *
 * @brief T4000 U-ARM message routing
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef MSGROUTE_H_
#define MSGROUTE_H_

#include "osal.h"
#include "basetypes.h"
#include "resultcodes.h"
#include "apidefs.h"
#include "ctrlmsg.h"

//
// Flags for message allocation
//
#define MSG_COMMON                  0x00000000
#define MSG_FAST                    0x00000001

extern OSAL_MEMARRAY MsgArray;

// Callback type for message dispatcher
// 1st param - message size
// 2nd param - pointer to the message
typedef RESULTCODE (*MSGDISPATCHER)(U32,PTR);

// Init message dispatcher
EXPORT RESULTCODE MsgDispatcherInit(void);
// Free resources allocated by message dispatcher
void MsgDispatcherDone(void);

// Message dispatcher
EXPORT RESULTCODE MsgDispatch(U32 srcID, U32 size, PTR msg);

// Registration/deregistration
U16 MsgGetAvailableInstance(void);
RESULTCODE MsgRegDispatcher(U16 msgDest, MSGDISPATCHER msgCB);
RESULTCODE MsgDeregDispatcher(U16 msgDest);



#endif /* MSGROUTE_H_ */
