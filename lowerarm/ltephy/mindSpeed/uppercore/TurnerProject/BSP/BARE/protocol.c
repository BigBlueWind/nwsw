/** @file protocol.c
 *
 * @brief SCTP/TCP/UDP interface for Linux
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "osal.h"
#include "transport.h"

/** Initialize transport module
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE ProtoInit(U32 protoid)
{
    return SUCCESS;
}

/** Stop transport module
 *
 * @return Standard result code
 */
RESULTCODE ProtoDone(void)
{
    return SUCCESS;
}

/** Send data block to transport layer
 *
 * @param size Size of block in bytes
 * @param data Pointer to data
 * @return Standart result code
 */
RESULTCODE SctpSend(U32 size, PTR data)
{
    return SUCCESS;
}

/** This task receives packets from transport layer
 *
 * @param taskParm Not used
 */
void ProtoRxTask(PTR taskParm)
{
}

#endif /* USE_BARE */
