/** @file uexp.c
 *
 * @brief Universal Expansion Bus
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (USE_BARE) || defined (USE_RTXC)

#include "uexp.h"
#include "apb.h"

/** Initialize Universal Expansion Bus
 *
 * @return Standard result code
 */
RESULTCODE UexpInit(void)
{
    // Universal Expansion Bus setup
    IOWrite16A(UEXP_CONFIG, 4);

    // Configure EXPCSP4 (PLD)
    // csm = 00, xre#/xwe#, 32bit, cmd = 4, addr = 2
#ifdef M823XX
    IOWrite16A(UEXP_CS_CFGA4, 0x1106);
#else
    IOWrite16A(UEXP_CS_CFGA(4), 0x1106);
#endif

    // Enable CS
    IOWriteOr32A(ASD_CS_ENABLE, 0x00000001 << UEXP_CS4);

    return SUCCESS;
}

#endif /* USE_BARE || USE_RTXC */
