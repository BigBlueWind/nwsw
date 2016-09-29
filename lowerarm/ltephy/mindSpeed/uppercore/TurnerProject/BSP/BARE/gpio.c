/** @file gpio.c
 *
 * @brief GPIO module
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "gpio.h"
#include "apb.h"
#include "portio.h"

/** Initialize GPIO
 *
 * @return Standard result code
 */
RESULTCODE GpioInit(void)
{
#ifdef M823XX
    // Reset all peripheral devices
    IOWriteOr32A(GPIO_OUTPUT_ENABLE, GPIO2);    // enable output for GPIO_2
    IOWriteAnd32A(GPIO_OUTPUT, ~GPIO2);         // Set 0
    IOWriteOr32A(GPIO_OUTPUT, GPIO2);           // Set 1

    // Set min value for APB wait state
    IOWrite32A(GPIO_APB_ACCESS_WS, 0);
#endif /*M823XX*/

    return SUCCESS;
}
