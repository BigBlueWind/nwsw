/** @file packetif.h
 *
 * @brief Direct packet interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _PACKETIF_H_
#define _PACKETIF_H_

#include "resultcodes.h"
#include "osal.h"

typedef RESULTCODE(*MacCB)(PTR, U32);

RESULTCODE NetEthInit(BOOLEAN bOpen, OSAL_HEAP *pHeap, MacCB cb);

#endif /* _PACKETIF_H_ */
