/**
 * @file    bsm_ems_reg.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to EMS registration.
 *
 * @author:  Shweta Polepally
 * @date:    Dec 2010
 * @details: Initial Version.
 **/

#include "bsm.h"

/**
 * @param locationSource  Location Source - GPS/Configured
 * @param latitude        Latitude details of enodeB
 * @param longitude       Longitude details of enodeB
 * @details This function updates Location status table with the details provided in the input
 *          parameters.
 */
void updateLocationStatusTable(
  IN LocationSource  locationSource,
  IN UINT8           *latitude,
  IN UINT8           *longitude
  )
{
  LocationStatus        *rowOfStatusTable = (LocationStatus*)getFirstRowOfTable(LOCATIONSTATUS);

  rowOfStatusTable->locationSource = locationSource;
  snprintf( rowOfStatusTable->latitude, strlen(latitude)+1, "%s", latitude );
  snprintf( rowOfStatusTable->longitude, strlen(longitude)+1, "%s", longitude );
}

/**
 * @param bsmContext   BSM context information
 * @details This function sends MSG_BSM_DEH_GPS_DET_REQ to DEH Module for getting GPS details.
 */
void sendGpsDetailReqMsg()
{
  createAndSendMsg( MSG_BSM_DEH_GPS_DETAIL_REQ, NULL, 0 );
  /* Start timer and wait for the response from DEH module */
  startBsmProcTimer( TIMERNAME_WAITGPSPARAMETERSRSP, getBsmTimersInfo() );
}

/**
 * @param bsmContext   BSM context information
 * @details This function carries out EMS registration procedure.
 */
void doEmsRegistration(
  IO BsmContext   *bsmContext
  )
{
  bsmContext->emsRegSucc = FALSE;

  setBsmProceduralState( bsmContext, BS_WAIT_REGISTRATION_CMPL );

  /* Send Registration Request message to SIH */
  sendEventEnbRegistrationReq();

  /* Start timer and wait for the response from SIH module */
  startBsmProcTimer( TIMERNAME_WAITEMSREGRSP, getBsmTimersInfo() );
}

/**
 * @param monthInNum   Month in integer format
 * @details This function returns month is string format.(XXX format)
 */
char *getMonthInString(
  UINT8   monthInNum
  )
{
  switch(monthInNum)
  {
    case 1:  return "JAN";
    case 2:  return "FEB";
    case 3:  return "MAR";
    case 4:  return "APR";
    case 5:  return "MAY";
    case 6:  return "JUN";
    case 7:  return "JUL";
    case 8:  return "AUG";
    case 9:  return "SEP";
    case 10: return "OCT";
    case 11: return "NOV";
    case 12: return "DEC";
  }
  return "NON";  /* This would never happen, added to remove warning */
}

/**
 * @param msg        Message received from DEH
 * @param bsmContext BSM module information.
 * @details This function handles
 */
BsmProceduralState bsmProcGpsDetailSucc(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgDehBsmGpsDetailSucc   *succMsg = (MsgDehBsmGpsDetailSucc*)msg->data;
  EnbRegistrationConfig    *rowOfTable = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);

  stopBsmProcedureTimer( TIMERNAME_WAITGPSPARAMETERSRSP );

  /* Store time zone from configuration file */
  clearBytes( &bsmContext->currTimeOffsetFromUTC, MAX_SIZE_ENBSTATUS_LOCALTIMEOFFSETFROMUTC+1 );
  snprintf( &bsmContext->currTimeOffsetFromUTC[0], MAX_SIZE_ENBSTATUS_LOCALTIMEOFFSETFROMUTC+1, "%s", rowOfTable->initialLocalTimeOffsetFromUTC );
  logInfo("%s", "Date, time and location read from EnbRegistrationConfig file will be used ");

  /*
   * Normal addition not exceeding the max hrs
   * Addition which exceeds the max hrs 23.00 + 5hrs30Hrs  = 4 . 30 */
  if ( TIMEANDLOCATIONSOURCETOUSE_GPS EQ rowOfTable->timeAndLocationSourceToUse )
  {
    if (TRUE EQ isEnodeBLocked())
    {
      /* Remove. I suppose this is not required */
      bsmContext->dateAndTimeSource = DATEANDTIMESOURCE_GPS;
      /* Update location Update table */
      updateLocationStatusTable( LOCATIONSOURCE_GPS, succMsg->latitude, succMsg->longitude );
      /* Change the system time with the time received in MsgDehBsmGpsDetailSucc Message */
      String50   dateAndTimeToSet;
      clearBytes( dateAndTimeToSet, sizeof(String50) );      
      snprintf( dateAndTimeToSet, sizeof(String50) ,"date -s \"%d %s %d %d:%d:%d\" > /dev/null 2>&1",
               succMsg->date.date, getMonthInString( succMsg->date.month ), succMsg->date.year,
               succMsg->time.hours, succMsg->time.minutes, succMsg->time.seconds );

      /* Calculating Uptime Difference when updating timeanddatesource */
      bsmContext->moduleUpTimeDifference = bsmContext->moduleUpTimeDifference + (UINT32)getUpTime( bsmContext->moduleStartTime );

      if ( 0 NOTEQ system( dateAndTimeToSet ) )
      {
        BS_ERROR_DATE_AND_TIME_COULD_NOT_BE_SET( dateAndTimeToSet, "GPS device" );
      }
      bsmContext->moduleStartTime = getCurrentTime();
    }
  }
  else if ( TIMEANDLOCATIONSOURCETOUSE_OPERCONFIGURED EQ rowOfTable->timeAndLocationSourceToUse )
  {
    /* Scenario: GPS details are received but the configuration file value says the TimeAndLocation to use
     * is from config file.Hence read from configuration file and then continue.
     * Although this condition should never occur because configuration file default value
     * will always be to read from GPS during initialisation */
    if (TRUE EQ isEnodeBLocked())
    {
      updateLocationStatusTable( LOCATIONSOURCE_CONFIGUREDBYOAM, rowOfTable->configuredLatitude, rowOfTable->configuredLongitude );
      /* Update date and time field present in the enodeB config file */
      if ( TRUE EQ bsmSetDateAndTimeOfSystem( rowOfTable->initialLocalTime ) )
        bsmContext->dateAndTimeSource = DATEANDTIMESOURCE_CONFIGURED;
    }
  }

  if ( BS_WAIT_COLLECT_INFO_CMPL EQ bsmContext->bsmProceduralState )
  {
    /* Do EMS Registration Procedure */
    doEmsRegistration( bsmContext );
  }

  return ( bsmContext->bsmProceduralState );
}

void doCollectionInfoProcedureFailureSteps(
  IN UINT8            *errString,
  IO BsmContext       *bsmContext
  )
{
  /* log the error reason */
  BS_ERROR_GPS_PARA_NOT_CONFIGURED( errString );

  /* Update location status table with the default values present in the config file */
  EnbRegistrationConfig *rowOfTable = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);
  updateLocationStatusTable( LOCATIONSOURCE_CONFIGUREDBYOAM, rowOfTable->configuredLatitude, rowOfTable->configuredLongitude );
  /* Update date and time field present in the enodeB config file */
  if ( TRUE EQ bsmSetDateAndTimeOfSystem( rowOfTable->initialLocalTime ) )
    bsmContext->dateAndTimeSource = DATEANDTIMESOURCE_CONFIGURED;
  /* Store time zone from configuration file */
  clearBytes( &bsmContext->currTimeOffsetFromUTC, MAX_SIZE_ENBSTATUS_LOCALTIMEOFFSETFROMUTC+1 );
  snprintf( &bsmContext->currTimeOffsetFromUTC[0], strlen(rowOfTable->initialLocalTimeOffsetFromUTC)+1, "%s", rowOfTable->initialLocalTimeOffsetFromUTC );
  logInfo("%s", "Date, time and location read from EnbRegistrationConfig file will be used ");
}

/**
 * @param msg        Message received from DEH
 * @param bsmContext BSM module information.
 * @details This function handles
 */
BsmProceduralState bsmProcGpsDetailFail(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgDehBsmGpsDetailFail   *failMsg = (MsgDehBsmGpsDetailFail*)msg->data;


  DeviceMonitorConfig   *deviceMonitorConfig = (DeviceMonitorConfig*)getFirstRowOfTable(DEVICEMONITORCONFIG);
  stopBsmProcedureTimer( TIMERNAME_WAITGPSPARAMETERSRSP );
  
  if(deviceMonitorConfig->gps EQ FALSE)
  {
    bsmCreateAndSendSetLedColor( LEDID_GPS, LEDCOLOR_AMBER, bsmContext );
  }
  else
  {
    bsmCreateAndSendSetLedColor( LEDID_GPS, LEDCOLOR_RED, bsmContext );
  }

  doCollectionInfoProcedureFailureSteps( failMsg->errDetail, bsmContext );

  /* Even if parameters are not received from DEH module,
   * go ahead and do registration procedure */
  if ( BS_WAIT_COLLECT_INFO_CMPL EQ bsmContext->bsmProceduralState )
  {
    /* Do EMS Registration Procedure */
    doEmsRegistration( bsmContext );
  }

  return ( bsmContext->bsmProceduralState );
}

