/** @file bsp.c
 *
 * @brief Board Support Package for Windows
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_WINDOWS

#include "bsp.h"
#include "osal.h"
#include "c4ktrace.h"

/** Not needed in user mode app
 *
 * @param cpuid CPU ID
 * @return
 */
RESULTCODE BspInit(U32 cpuid)
{
    C4KTraceInit(APP_MAC, 0);
    return SUCCESS;
}

/** Initialize PCI Host Interface
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE PhiInit(U32 cpuid)
{
    return SUCCESS;
}

/** Write data to the Host interface
 *
 * @param size
 * @param data
 * @return
 */
RESULTCODE PhiWrite(U32 size, PTR data)
{
    return SUCCESS;
}

/*! \brief Initialize UART
 *
 *  \param cpuid CPU ID on which this function called
 *  \return Standard result code
*/
RESULTCODE UartInit(U32 cpuid)
{
    return SUCCESS;
}

#endif /* USE_WINDOWS */
