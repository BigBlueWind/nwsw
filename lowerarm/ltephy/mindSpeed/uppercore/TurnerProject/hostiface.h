/** @file hostiface.h
 *
 * @brief Host interface messages exchange
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef HOSTIFACE_H_
#define HOSTIFACE_H_

#include "resultcodes.h"

//
// Limits
//
#define HIF_MAXMSGS                 6

//
// Modes
//
#define HMODE_RELEASE               0
#define HMODE_DEMO                  1
#define HMODE_FORCE_NET             3

RESULTCODE HostInit(void);
RESULTCODE HostDone(void);

extern void (*HostRxTask)(PTR data);
void HostTxTask(PTR data);

// Can use different mailboxes depending on mode
// Can use direct access
extern RESULTCODE (*HostSendMsg)(U32 msgSize, PTR data);

#endif /* HOSTIFACE_H_ */
