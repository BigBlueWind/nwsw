#ifndef LTE_BB_LED_UTIL_H
#define LTE_BB_LED_UTIL_H

#include "basic_types.h"

#define LED_ERR_LOG(format...)            syslog(LOG_ERR,  format)
#define LED_CRIT_LOG(format...)           syslog(LOG_CRIT, format)

#ifdef DEBUG_LED
#define LED_DEBUG_LOG(format...)          syslog(LOG_DEBUG, format)
#else
#define LED_DEBUG_LOG(format...)
#endif

#define LED_DEVICE_FILE  "/dev/led"

typedef struct led_data_tag
{
    short led_num;
    short led_state;
}led_data;

#endif /* LTE_BB_LED_UTIL_H*/
