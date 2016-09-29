/**
 * @file    deh_gps.c
 * @brief This file is contains code get detail from GPS module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author    aagrawal, spolepally
 * @date      25-05-2011
 * @details 1) LED color is supported in DEH.
 *			2) Added support for GET_OBJ_REQ on ledStatus table.
 *			3) Implemented handling of component status table at BSM and DEH.
 */


#include "deh.h"
#include "lte_bb_platform.h"


#define MAX_LEDID    4

/**
 * @param   ledid  contains ledid
 * @details  This function returns the LED Number of respective LED Id.
 */

static LedNum getLedNumberFromLedId(
  IN LedId    ledId
  )
{
  switch(ledId)
  {
    case LEDID_GPS:              return LEDNUM_THIRD_LED;
    case LEDID_EMSREGISTRATION:  return LEDNUM_SECOND_LED;
    case LEDID_ENBOPERATIONAL:  return LEDNUM_FIRST_LED;
    default:
      BS_ERROR_INTERNAL_ERROR_OCCURED("LEDID is received other than LEDID_GPS,LEDID_EMS_REGISTRATION,LEDID_ENB_OPERATIONAL");
      return LEDNUM_MAX_LED;
  }
}

/**
 * @param   ledcolor  contains ledcolor
 * @details  This function returns the LED State of respective LED Color.
 */
static LedState getLedStateFromLedColor(
  IN LedColor    ledColor
  )
{
  switch(ledColor)
  {
    case LEDCOLOR_AMBER: return LEDSTATE_AMBER;
    case LEDCOLOR_RED:   return LEDSTATE_RED;
    case LEDCOLOR_GREEN: return LEDSTATE_GREEN;
    default:
      BS_ERROR_INTERNAL_ERROR_OCCURED("LEDCOLOR is received other than AMBER,RED,GREEN");
      return LEDSTATE_MAX_STATE;
  }
}

/**
 * @details  This function initialises all the LEDs at start up.
 */
void initLedColorAndUpdateStatusTable(void )
{
  UINT8      i = 0;
  LedStatus  *rowOfLedStatus = (LedStatus*)getFirstRowOfTable(LEDSTATUS);

  LedId     numOfLedIdToInitialize[MAX_LEDID] = { LEDID_BBPOWER, LEDID_GPS, LEDID_EMSREGISTRATION, LEDID_ENBOPERATIONAL };
  LedNum    ledNumOfApi;
  LedState  ledState;

  /* LED1 is set to GREEN and rest all are set to AMBER during initialisation */
  rowOfLedStatus->index = i+1;
  rowOfLedStatus->ledId    = numOfLedIdToInitialize[i];
  rowOfLedStatus->ledColor = LEDCOLOR_GREEN;
  getNextRowOfTable(rowOfLedStatus);

  for ( i=1; i < MAX_LEDID; i++ )
  {
    /* Below 2 steps are required because we have to map the LED color and LEd id with the ones mentioned in the
     * platform API */
    ledNumOfApi = getLedNumberFromLedId(numOfLedIdToInitialize[i]);
    ledState    = getLedStateFromLedColor(LEDCOLOR_AMBER);

    if( LEDRETURNCODE_LED_STATUS_SUCCESS EQ bbSetLedState( ledNumOfApi, ledState ) )
    {
      logInfo("Color of %s is set to %s", ledIdStr(numOfLedIdToInitialize[i]), ledColorStr(LEDCOLOR_AMBER) );
    }
    else
    {
      BS_ERROR_SET_LED_COLOR_FAILED( ledIdStr(numOfLedIdToInitialize[i]), ledColorStr(LEDCOLOR_AMBER) );
    }
    rowOfLedStatus->index = i+1;
    rowOfLedStatus->ledColor = LEDCOLOR_AMBER;
    rowOfLedStatus->ledId    = numOfLedIdToInitialize[i];

    getNextRowOfTable(rowOfLedStatus);
  }
}

/**
 * @param   ledId     contains ledId
 * @param   ledcolor  contains ledcolor
 * @details  This function updates the LED Status table.
 */

static void updateLedStatusTable(
  LedId   ledId,
  LedColor ledColor
  )
{
  UINT8  i;
  LedStatus  *rowOfLedStatus = (LedStatus*)getFirstRowOfTable(LEDSTATUS);

  /* Skip rows which are already filled */
  for ( i=0; i < MAX_ROWS_IN_LEDSTATUS_TABLE; i++ )
  {
    if( rowOfLedStatus->ledId EQ ledId )
    {
      rowOfLedStatus->ledColor = ledColor;
      break;
    }
    getNextRowOfTable(rowOfLedStatus);
  }
}

/**
 * @param   msg         contains msg
 * @param   dehContext  contains dehcontext
 * @details  This function is used to set LED color.
 */
DehState dehProcSetLedColor(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgBsmDehSetLedColor   *setReq    = (MsgBsmDehSetLedColor*)msg->data;
  /* Below 2 steps are required because we have to map the LED color and LEd id with the ones mentioned in the
   * platform API */
  LedNum    ledNumOfApi = getLedNumberFromLedId(setReq->ledId);
  LedState  ledState    = getLedStateFromLedColor(setReq->ledColor);

  if( LEDRETURNCODE_LED_STATUS_SUCCESS EQ bbSetLedState(ledNumOfApi,ledState ) )
  {
    logInfo("Color of %s is set to %s", ledIdStr(setReq->ledId), ledColorStr(setReq->ledColor) );
    updateLedStatusTable( setReq->ledId, setReq->ledColor );
    return dehContext->dehState;
  }
  else
  {
    BS_ERROR_SET_LED_COLOR_FAILED( ledIdStr(setReq->ledId), ledColorStr(setReq->ledColor) );
    return dehContext->dehState;
  }
}

