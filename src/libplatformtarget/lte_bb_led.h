#ifndef LTE_PLTM_LED_H
#define LTE_PLTM_LED_H

#include "lte_bb_led_util.h"

typedef enum e_LedReturnCode
{
   LEDRETURNCODE_ERR_LED_DRV_OPEN = 15,
   LEDRETURNCODE_ERR_LED_INVALID_PARAM,
   LEDRETURNCODE_LED_STATUS_SUCCESS,
   LEDRETURNCODE_LED_STATE_OFF,
   LEDRETURNCODE_LED_STATE_RED,
   LEDRETURNCODE_LED_STATE_GREEN,
   LEDRETURNCODE_LED_STATE_AMBER,
   LEDRETURNCODE_LED_STATE_UNKNOWN,
}LedReturnCode;

typedef enum e_LedState
{
    LEDSTATE_OFF,
    LEDSTATE_RED,
    LEDSTATE_GREEN,
    LEDSTATE_AMBER,
    LEDSTATE_MAX_STATE
}LedState;

typedef enum e_LedNum 
{
    LEDNUM_FIRST_LED,  /* DS33 */
    LEDNUM_SECOND_LED, /* DS32 */
    LEDNUM_THIRD_LED,  /* DS34 */
    LEDNUM_MAX_LED
}LedNum;

/* extern function decalratios */
extern LedNum bbGetNumAvailableLed (void);
extern LedReturnCode bbGetLedState (LedNum ledNum);
extern LedReturnCode bbSetLedState (LedNum ledNum,LedState ledState);

#endif /* LTE_PLTM_LED_H */
