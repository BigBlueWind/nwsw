/**
 * @file    bsm_device_init.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief   This file contains device initialisation functions.
 *
 * @author:  Shweta Polepally
 * @date:    20 May 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param msg        Message from DEH module
 * @param bsmContext BSM Context
 * @details This function will process MSG_DEH_BSM_DEVICE_INIT_SUCC message.
 */
BsmProceduralState bsmProcDeviceInitSucc(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgDehBsmDeviceInitSucc  *initSucc = (MsgDehBsmDeviceInitSucc*)msg->data;
  UINT8                     i=0;

  stopBsmProcedureTimer( TIMERNAME_WAITDEVICEINITCOMPLETE );

  /* Reception of DeviceInitSucc states that all devices are up, hence Rh is also UP
   * To Cross check, check if RH device status is received and set the flag. Same for GPS device */
  for ( i=0; i < initSucc->numOfDevices; i++ )
  {
    switch( initSucc->componentOpState[i].componentType )
    {
      case COMPONENTTYPE_RADIOHEAD:
        bsmContext->isRhEnabled = TRUE;
        break;
      case COMPONENTTYPE_GPS:
        if (COMPONENTOPERATIONALSTATE_NOTMONITORED EQ initSucc->componentOpState[i].componentOperationalState)
        {
          bsmCreateAndSendSetLedColor( LEDID_GPS, LEDCOLOR_AMBER, bsmContext );
        }
        else
        {
          bsmCreateAndSendSetLedColor( LEDID_GPS, LEDCOLOR_GREEN, bsmContext );
        }
        break;
        /* Add for other devices if required */
      case COMPONENTTYPE_BASEBAND:
      case COMPONENTTYPE_ETHERNET:
      case COMPONENTTYPE_TEMPSENSOR:
      case COMPONENTTYPE_CLOCK:
      case COMPONENTTYPE_CPRI:
        break;
      default:
        BS_ERROR_INVALID_DEVICE_TYPE( initSucc->componentOpState[i].componentType );
        break;
    }
  }
  doCollectInfoProcedure(bsmContext);
  return bsmContext->bsmProceduralState;
}

/**
 * @param alarmCause Alarm Cause.
 * @details This function will return TRUE/FALSE based on InstanceId needs to be appended for
 *          the alarm cause or not.
 */
TBOOL appendInstanceId(
  IN AlarmCause alarmCause
  )
{
  switch(alarmCause)
  {
    case RHCLOCKFAIL:
    case RHHIGHTEMPERATURE:
    case RHLOWTEMPERATURE:
    case RHHIGHVOLTAGE:
    case RHLOWVOLTAGE:
    case RHCOMMONINTERNALERROR:
    case CPRIINTERFACEDOWN:
    case GPSRECVNOTRESPONDING:
    case BACKHAULETHERNETLINKDOWN:
    case DEBUGETHERNETLINKDOWN:
      return TRUE;
    default: return FALSE;
  }
}

/**
 * @param alarmCause Alarm Cause.
 * @details This function will return TRUE/FALSE based on InstanceId and Path needs to be appended for
 *          the alarm cause or not.
 */
TBOOL appendInstanceIdAndPath(
  IN AlarmCause alarmCause
  )
{
  switch(alarmCause)
  {
    case RHOVERPOWER:
    case VSWRFAIL:
    case LOWPOWER:
    case LOWGAIN:
    case RHDISABLED:
    case RHSHUTDOWN:
    case LNAFAIL:
    case RHPATHINTERNALERROR:
      return TRUE;
    default: return FALSE;
  }
}

/**
 * @param   componentEntry       Component table entry.
 * @param   alarmDetails         Alarm Detail Structure
 * @details This function will form Alarm String location for the Equipment type of alarm.
 */
static void formAlarmLocationStringForEquipAlarms(
  IN ComponentOpState     *componentEntry,
  IO CreateAlarmDetails   *alarmDetails
  )
{
  UINT8  strAppendLocation[MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION];
  clearBytes( &strAppendLocation, MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION );

  if ( TRUE EQ appendInstanceId( alarmDetails->alarmCause ) )
  {
    snprintf( strAppendLocation, MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION, "%d", componentEntry->componentInstance );
    strcat( (char*)alarmDetails->alarmLocation, strAppendLocation );
  }
  else if ( TRUE EQ appendInstanceIdAndPath( alarmDetails->alarmCause ) )
  {
    snprintf( strAppendLocation, MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION, "%d->%s", componentEntry->componentInstance, componentEntry->infoString );
    strcat( (char*)alarmDetails->alarmLocation, strAppendLocation );
  }
}

/**
 * @param   componentEntry       Component table entry.
 * @details This function will raise component down alarm.
 */
static void raiseComponentDownAlarm(
  IN ComponentOpState *componentEntry
  )
{
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( componentEntry->bsErrorForDeviceDown.errorNum, &alarmDetails ) )
  {
    formAlarmLocationStringForEquipAlarms( componentEntry, &alarmDetails );

    /* Check if any additional error infor is provided by DEH Module. */
    if ( NULL NOTEQ componentEntry->errorString )
    {
      snprintf( alarmDetails.alarmAdditionalText, strlen(componentEntry->errorString)+1, "%s", componentEntry->errorString );
    }
    doSaveAlarm( &alarmDetails );
  }
}

/**
 * @param   componentEntry       Component table entry.
 * @details This function will clear component down alarm.
 */
static void clearComponentDownAlarm(
  IN ComponentOpState *componentEntry
  )
{
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( componentEntry->bsErrorForDeviceDown.errorNum, &alarmDetails ) )
  {
    formAlarmLocationStringForEquipAlarms( componentEntry, &alarmDetails );

    /* Check if any additional error infor is provided by DEH Module. */
    if ( NULL NOTEQ componentEntry->errorString )
    {
      snprintf( alarmDetails.alarmAdditionalText, strlen(componentEntry->errorString)+1, "%s", componentEntry->errorString );
    }
    doClearAlarm( &alarmDetails );
  }
}

/**
 * @param   msg        Message from DEH module
 * @param   bsmContext BSM Context
 * @details This function will process MSG_DEH_BSM_DEVICE_INIT_FAIL message.
 * Note : Fail message will contain status of all devices, status few can be up
 *        and few can be down.
 */
BsmProceduralState bsmProcDeviceInitFail(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
/* Need to update according to message structure change */
  MsgDehBsmDeviceInitFail  *initFail = (MsgDehBsmDeviceInitFail*)msg->data;
  UINT8                     index;
  TBOOL                     isEth0Up = FALSE;
  TBOOL                     isGpsUp  = FALSE;

  stopBsmProcedureTimer( TIMERNAME_WAITDEVICEINITCOMPLETE );

  for( index=0; index < initFail->numOfDevices; index++ )
  {
    ComponentOpState *componentEntry = &initFail->componentOpState[index];
    switch( componentEntry->componentType )
    {
      case COMPONENTTYPE_BASEBAND:
      case COMPONENTTYPE_CPRI:
      case COMPONENTTYPE_TEMPSENSOR:
      case COMPONENTTYPE_CLOCK:
          {
            if( COMPONENTOPERATIONALSTATE_DOWN EQ componentEntry->componentOperationalState )
            {
              BS_ERROR_DEVICE_NOT_OPERATIONAL(componentTypeStr(componentEntry->componentType));
              raiseComponentDownAlarm( componentEntry );
            }
          }
          break;

      case COMPONENTTYPE_RADIOHEAD:
          {
            if( COMPONENTOPERATIONALSTATE_DOWN EQ componentEntry->componentOperationalState )
            {
              bsmContext->isRhEnabled = FALSE;
              BS_ERROR_DEVICE_NOT_OPERATIONAL(componentTypeStr(componentEntry->componentType));
              raiseComponentDownAlarm( componentEntry );
            }
            else
            {
              bsmContext->isRhEnabled = TRUE;
            }
          }
          break;

      case COMPONENTTYPE_ETHERNET:
          {
            if( COMPONENTOPERATIONALSTATE_DOWN EQ componentEntry->componentOperationalState )
            {
              isEth0Up = FALSE;
              BS_ERROR_DEVICE_NOT_OPERATIONAL(componentTypeStr(componentEntry->componentType));
              bsmCreateAndSendSetLedColor( LEDID_EMSREGISTRATION, LEDCOLOR_RED, bsmContext );
              raiseComponentDownAlarm( componentEntry );
            }
            else
            {
              isEth0Up = TRUE;
            }
          }
          break;

      case COMPONENTTYPE_GPS:
          {
            if( COMPONENTOPERATIONALSTATE_DOWN EQ componentEntry->componentOperationalState )
            {
              isGpsUp = FALSE;
              BS_ERROR_DEVICE_NOT_OPERATIONAL(componentTypeStr(componentEntry->componentType));
              bsmCreateAndSendSetLedColor( LEDID_GPS, LEDCOLOR_RED, bsmContext );
              raiseComponentDownAlarm( componentEntry );
            }
            else if (COMPONENTOPERATIONALSTATE_NOTMONITORED EQ componentEntry->componentOperationalState)
            {
              bsmCreateAndSendSetLedColor( LEDID_GPS, LEDCOLOR_AMBER, bsmContext );
            }
            else
            { 
              isGpsUp = TRUE;
              bsmCreateAndSendSetLedColor( LEDID_BBPOWER, LEDCOLOR_GREEN, bsmContext );
            }
          }
          break;

      default:
        BS_ERROR_INVALID_DEVICE_TYPE( componentEntry->componentType );
        break;
    }
  }
  if ( (TRUE EQ isEth0Up) && (TRUE EQ isGpsUp) )
  {
    doCollectInfoProcedure(bsmContext);
  }
  else if ( (TRUE EQ isEth0Up) && (FALSE EQ isGpsUp) )
  {
    /* Skip Collect info procedure as GPS device is down */
    doCollectionInfoProcedureFailureSteps( "GPS Device DOWN indication received", bsmContext );
    /* Start Registration procedure */
    doEmsRegistration( bsmContext );
  }
  else if ( FALSE EQ isEth0Up )
  {
    /* Still go ahead and start Collection info procedure */
    doCollectInfoProcedure(bsmContext);
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message from DEH module
 * @param   bsmContext BSM Context
 * @details This function will process component DOWN indication received from DEH.
 */
inline BsmProceduralState  bsmProcComponentDownInd(
  IN Msg         *msg,
  IO BsmContext  *bsmContext
  )
{
  TBOOL     disableEnb       = FALSE;
  MsgDehBsmComponentDownInd *ind       = (MsgDehBsmComponentDownInd*)msg->data;
  EnbRegistrationConfig     *enbRegConfig = (EnbRegistrationConfig*)getFirstRowOfTable(ENBSTATUS);

  /* Raise the alarm */
  raiseComponentDownAlarm( ind );
  switch( ind->componentType )
  {
    case COMPONENTTYPE_GPS:
         bsmCreateAndSendSetLedColor( LEDID_GPS, LEDCOLOR_RED, bsmContext );
         if(TIMEANDLOCATIONSOURCETOUSE_GPS EQ enbRegConfig->timeAndLocationSourceToUse)
           disableEnb = TRUE;
         break;
    case COMPONENTTYPE_RADIOHEAD:
         bsmContext->isRhEnabled = FALSE;
         disableEnb    = TRUE;
         break;
    case COMPONENTTYPE_ETHERNET: /* Backhaul and Debug */
         if ( ETHERNET_BACKHAUL EQ ind->componentInstance )
           bsmCreateAndSendSetLedColor( LEDID_EMSREGISTRATION, LEDCOLOR_RED, bsmContext );
         disableEnb = TRUE;
         break;
    case COMPONENTTYPE_BASEBAND:
    case COMPONENTTYPE_TEMPSENSOR:
    case COMPONENTTYPE_CPRI:
    case COMPONENTTYPE_CLOCK:
         disableEnb = TRUE;
         break;
    default:
        BS_ERROR_INVALID_DEVICE_TYPE( ind->componentType );
        break;
  }
  /* Check whether ENB disabling is required and ENB is Enabled or not */
  if ( (TRUE EQ disableEnb) && (TRUE EQ isEnodeBEnabled()))
  {
    bsmContext->enbDisableReason = CRITICAL_ALARM_GENERATED;
    enbDisable( TRUE, bsmContext );
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param   msg        Message from DEH module
 * @param   bsmContext BSM Context
 * @details This function will process component UP indication received from DEH.
 */
inline BsmProceduralState  bsmProcComponentUpInd(
  IN Msg         *msg,
  IO BsmContext  *bsmContext
  )
{
  MsgDehBsmComponentUpInd *ind       = (MsgDehBsmComponentUpInd*)msg->data;
  BsmContext    *ctx                 = getBsmContext();
  EnbRegistrationConfig  *config     = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);

  /* Clear the alarm */
  clearComponentDownAlarm(ind);

  /* TODO: Check if enodeB is Unlocked and disable.
   * If disabled due to raised alarm ( mapped to the received clearing alarm )
   * then as part of recovery, enable procedure should be called. */
  switch( ind->componentType )
  {
    case COMPONENTTYPE_GPS:
         bsmCreateAndSendSetLedColor( LEDID_GPS, LEDCOLOR_GREEN, bsmContext );
         if( config->timeAndLocationSourceToUse EQ TIMEANDLOCATIONSOURCETOUSE_GPS )
         {
           sendGpsDetailReqMsg();
         }
         break;
    case COMPONENTTYPE_RADIOHEAD:
         bsmContext->isRhEnabled = TRUE;
         break;
    case COMPONENTTYPE_BASEBAND:
    	 break;
    case COMPONENTTYPE_ETHERNET: /* Backhaul and Debug */
         if ( (ETHERNET_BACKHAUL EQ ind->componentInstance) && (TRUE EQ ctx->emsRegSucc))
         {
           bsmCreateAndSendSetLedColor( LEDID_EMSREGISTRATION, LEDCOLOR_GREEN, bsmContext );
         }
         break;
    case COMPONENTTYPE_TEMPSENSOR:
    case COMPONENTTYPE_CPRI:
    case COMPONENTTYPE_CLOCK:
         break; /* No need to perform any operation */
    default:
         BS_ERROR_INVALID_DEVICE_TYPE( ind->componentType );
         break;
  }
  if (0 EQ getNumActiveCriticalAlarms() && ( FALSE EQ isEnodeBLocked()))
  {
    bsmContext->enbEnableReason = ENABLE_REASON_COMPONENT_ALARM_CLEARED;
    enbEnable( bsmContext );
  }

  return ( bsmContext->bsmProceduralState );
}
