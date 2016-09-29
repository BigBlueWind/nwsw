/** @file spusched.h
 *
 * @brief SPU scheduler interfaces and definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef SPUSCHED_H_
#define SPUSCHED_H_

#include "resultcodes.h"

//
// Limits
//
#define SPU_RXQSIZE             1024
#define SPU_TXQSIZE             1024

//
// Initialization
//
RESULTCODE SpuInit(void);

//
// Tasks
//
void SpuRxTask(PTR data);
void SpuTxTask(PTR data);

#endif /* SPUSCHED_H_ */
