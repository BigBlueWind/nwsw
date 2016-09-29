/** @file arbiter.c
 *
 * @brief AHB arbiter setup
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (USE_BARE) || defined (USE_RTXC)

#include "arbiter.h"

/** Initialize AHB
 *
 * @return Standard result code
 */
RESULTCODE AhbInit(void)
{
    // Priority levels on common bus (tdm, arm0, arm1, idma, smc)
    AhbPriority(1, 3, 4, 2, 5);

    // Enable requests for arbitration
    // On common bus: all
    IOWrite32A(ASA_TC_CR, ASA_TC_REQTDMAEN | ASA_TC_REQARM0EN |
               ASA_TC_REQARM1EN | ASA_TC_REQSMCEN | ASA_TC_REQIDMAEN);

    // On ARAM bus: all
    IOWrite32A(ASA_ARAM_TC_CR, ASA_TC_REQARM0EN | ASA_TC_REQARM1EN |
               ASA_TC_REQSMCEN | ASA_TC_REQTDMAEN | ASA_TC_REQIDMAEN);

    // On ERAM bus: all
    IOWrite32A(ASA_ERAM_TC_CR, ASA_TC_REQARM0EN | ASA_TC_REQARM1EN |
               ASA_TC_REQSMCEN | ASA_TC_REQTDMAEN);

    // On UEXP bus: ARM0, ARM1 only
    IOWrite32A(ASA_UEXP_TC_CR, ASA_TC_REQARM0EN | ASA_TC_REQARM1EN);

    return SUCCESS;
}

#endif /* USE_BARE || USE_RTXC */
