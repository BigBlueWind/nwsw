/** @file gpio.h
 *
 * @brief GPIO module definitions and interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "resultcodes.h"

//
// GPIO Enable Register masks
//
#define GPIO0                   0x01
#define GPIO1                   0x02
#define GPIO2                   0x04
#define GPIO3                   0x08
#define GPIO4                   0x10
#define GPIO5                   0x20
#define GPIO6                   0x40
#define GPIO7                   0x80

RESULTCODE GpioInit(void);

#endif /*GPIO_H_*/
