/** @file gemac.c
 *
 * @brief Substitute for GEMAC driver interface for Linux
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "osal.h"

/** Initialize GEMAC module
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE GemacInit(U32 cpuid)
{
    return SUCCESS;
}

/** Send data block to GEMAC0
 *
 * @param size Size of block in bytes
 * @param data Pointer to data
 * @return Standart result code
 */
RESULTCODE Gemac0Write(U32 size, PTR data)
{
    return SUCCESS;
}

/** Send data block to GEMAC1
 *
 * @param size Size of block in bytes
 * @param data Pointer to data
 * @return Standart result code
 */
RESULTCODE Gemac1Write(U32 size, PTR data)
{
    return SUCCESS;
}

/** This task handles Rx requests on GEMAC0
 *
 * @param taskParm Not used
 */
void Gemac0RxTask(PTR taskParm)
{
}

/** This task handles Rx requests on GEMAC1
 *
 * @param taskParm Not used
 */
void Gemac1RxTask(PTR taskParm)
{
}

#endif /* USE_LINUX */
