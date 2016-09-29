/** @file auxiface.h
 *
 * @brief AUX interface messages exchange
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _AUXIFACE_H_
#define _AUXIFACE_H_

#include "resultcodes.h"

//
// Limits
//
#define AUX_MAXMSGS             6

//
// Usage
//
#define AUX_NOTUSED             0
#define AUX_ORIGIN              1
#define AUX_CHAINED             2
#define AUX_REMOTE              3

//
// Pointer to actual send function
//
extern RESULTCODE (*AuxSendMsg)(U32 msgSize, PTR data);

RESULTCODE AuxInit(void);
RESULTCODE AuxDone(void);
RESULTCODE AuxSendMsgQueue(U32 msgSize, PTR data);

#endif /* _AUXIFACE_H_ */
