/** @file auxiface.c
 *
 * @brief AUX interface (dummy for BARE)
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (USE_BARE) || defined (USE_RTXC)

#include "osal.h"
#include "auxiface.h"

//
// Pointers to actual send function
//
RESULTCODE (*AuxSendMsg)(U32 msgSize, PTR data) = NULL;

/** Initialize Host Interface
 *
 * @return Standard result code
 */
RESULTCODE AuxInit(void)
{
    return SUCCESS;
}

/** This task receives messages from AUX interface
 *
 * @param data Task default parameter
 */
void AuxRxTask(PTR data)
{
}

RESULTCODE AuxDone(void)
{
    return SUCCESS;
}

#endif /* USE_BARE || USE_RTXC */
