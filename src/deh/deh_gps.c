/**
 * @file    deh_gps.c
 * @brief This file is contains code get detail from GPS module
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author aagrawal, spolepally, kgummad, mkata, tmaji
 * @date   02-06-2011
 * @details 1)Message MSG_BSM_DEH_GPS_DET_REQ is handled at DEH.
 * 			2)Implemented EMS registration code.
 *			3)BB APIs Integrated.
 *			4)LED color is supported in DEH.
 *			4)GPS Status added in DEH.
 */

#include "deh.h"
#include "lte_bb_platform.h"

/**
 * @details This function is initialises DEH global context GPS data. *
 */
void initDehGpsCtx()
{
  DehGpsContext  *dehGpsContext = getGlobalDehGpsContext();
  clearBytes( dehGpsContext, sizeof(DehGpsContext));
  dehGpsContext->isGpsDataValid = FALSE;
  /* Updating GPS Mutex Variable */
  pthread_mutex_init(&g_gpsMutex, NULL);
}

/**
 * @details This function is the main code of execution of GPS thread.
 *          It updates the DEH global context GPS data after getting the data from GPS device.
 */

void *gpsThreadMain()
{
  DehGpsContext  *dehGpsContext = getGlobalDehGpsContext();
  LatitudeInfo     gpsLatitudeInfo;
  LongitudeInfo    gpsLongitudeInfo;
  DateInfo         gpsDateInfo;
  TimeInfo         gpsTimeInfo;

  /* Reset GPS */
  if( TRUE EQ dehGpsReset())
  {
    logInfo("%s", "GPS RESET Success", NULL);
  }
  else
  {
    logError("%s", "Error in GPS RESET", NULL);
  }
  if(GPSRETURNCODE_STATUS_SUCCESS EQ openGPSdevice())
  {
    logInfo("%s","GPS device file opened", NULL);
  }
  while(1)
  {
    /* Critical section where GPS data is updated */

    if( GPSRETURNCODE_STATUS_SUCCESS NOTEQ getDate( &gpsDateInfo ))
    {
      pthread_mutex_lock( &g_gpsMutex );
      clearBytes(dehGpsContext, sizeof(DehGpsContext));
      pthread_mutex_unlock( &g_gpsMutex );
      continue;
    }
    if( GPSRETURNCODE_STATUS_SUCCESS NOTEQ getTime( &gpsTimeInfo ))
    {
      pthread_mutex_lock( &g_gpsMutex );
      clearBytes(dehGpsContext, sizeof(DehGpsContext));
      pthread_mutex_unlock( &g_gpsMutex );
      continue;
    }
    if( GPSRETURNCODE_STATUS_SUCCESS NOTEQ getLatitude( &gpsLatitudeInfo ))
    {
      pthread_mutex_lock( &g_gpsMutex );
      clearBytes(dehGpsContext, sizeof(DehGpsContext));
      pthread_mutex_unlock( &g_gpsMutex );
      continue;
    }
    if( GPSRETURNCODE_STATUS_SUCCESS NOTEQ getLongitude( &gpsLongitudeInfo ))
    {
      pthread_mutex_lock( &g_gpsMutex );
      clearBytes(dehGpsContext, sizeof(DehGpsContext));
      pthread_mutex_unlock( &g_gpsMutex );
      continue;
    }
    pthread_mutex_lock( &g_gpsMutex );
    dehGpsContext->dehGpsDateInfo= gpsDateInfo;
    dehGpsContext->dehGpsTimeInfo= gpsTimeInfo;
    dehGpsContext->dehGpsLatitudeInfo= gpsLatitudeInfo;
    dehGpsContext->dehGpsLongitudeInfo= gpsLongitudeInfo;
    dehGpsContext->isGpsDataValid = TRUE;
    pthread_mutex_unlock( &g_gpsMutex );
  }
}

/**
 * @param   errDetail   error Detail String
 * @details This function creates a message MSG_DEH_BSM_GPS_DET_FAIL to BSM.
 */
static void createAndSendGpsDetailsFail(
  IN UINT8  *errDetail
  )
{
  MsgDehBsmGpsDetailFail  gpsDetFail;
  clearBytes(&gpsDetFail, sizeof(MsgDehBsmGpsDetailFail));
  copyBytes(errDetail, sizeof(String256), gpsDetFail.errDetail);
  createAndSendMsg( MSG_DEH_BSM_GPS_DETAIL_FAIL, (UINT8*)&gpsDetFail, sizeof(MsgDehBsmGpsDetailFail) );
}

/**
 *  @details This function will check whether GPS is active or not.
 */
TBOOL dehIsGpsActive()
{
  TBOOL result = FALSE;
  DehGpsContext  *dehGpsCtx = getGlobalDehGpsContext();

  pthread_mutex_lock(&g_gpsMutex);
  result = dehGpsCtx->isGpsDataValid;
  pthread_mutex_unlock(&g_gpsMutex);
  if ( TRUE EQ result )
  {
    logInfo("GPS is Active", NULL);
  }
  else
  {
    logError("GPS is Not Active", NULL);
  }
  return result;
}

/**
 *  @details This function will return the GPS inactive reason.
 */
UINT8* dehGpsErrorString()
{
  logError("Unable to connect to satellite", NULL);
  return "Satellite Connection Fail";
}

/**
 * @param   msg    GPS detail request message coming from BSM
 * @param   dehContext   Deh Context
 * @details This function will process GPS Detail Request message received from BSM.
 */
DehState dehProcGpsDetailsReq(
 IN Msg              *msg,
 IO DehContext       *dehContext
 )
{
  MsgDehBsmGpsDetailSucc  gpsDetSucc;

  UNUSED(msg);
  /* Local context of Latitude and Longitude */
  LatitudeInfo  latitudeInfo;
  LongitudeInfo longitudeInfo;
  DevStatus     *presentStatus  = getDevicePresentStatusPtr();
  DehGpsContext *dehGpsContext = getGlobalDehGpsContext();
  clearBytes( &longitudeInfo, sizeof(LongitudeInfo));
  clearBytes( &latitudeInfo, sizeof(LatitudeInfo));
  clearBytes( &gpsDetSucc, sizeof(MsgDehBsmGpsDetailSucc));

  if(!isDeviceMonitored(DEV_GPS))
  {
    createAndSendGpsDetailsFail("GPS is Not Monitored");
    return dehContext->dehState;
  }
  if(presentStatus->devConnectionStatus[DEV_GPS] NOTEQ DEV_CONNECTED)
  {
  	createAndSendGpsDetailsFail("GPS not connected");
    return dehContext->dehState;
  }

  /*Copying latitude and longitude info into local context from DEH Context*/
  pthread_mutex_lock(&g_gpsMutex);
  //logInfo("%d/%d/%d", dehGpsContext->dehGpsDateInfo.year, dehGpsContext->dehGpsDateInfo.month, dehGpsContext->dehGpsDateInfo.date);
  latitudeInfo  = dehGpsContext->dehGpsLatitudeInfo;
  longitudeInfo = dehGpsContext->dehGpsLongitudeInfo;

  /*Copying time and date info into local context from DEH Context*/
  gpsDetSucc.time = dehGpsContext->dehGpsTimeInfo;
  gpsDetSucc.date = dehGpsContext->dehGpsDateInfo;
  pthread_mutex_unlock(&g_gpsMutex);


  /* Checking and formatting Longitude Information */
  if( longitudeInfo.gpsDirection EQ GPSDIRECTION_WEST )
  {
    snprintf((UINT8*)&gpsDetSucc.longitude[0], MAX_SIZE_LOCATIONSTATUS_LONGITUDE + 1, "%f%s ", longitudeInfo.longitude,"W");
  }
  else if( longitudeInfo.gpsDirection EQ GPSDIRECTION_EAST )
  {
    snprintf((UINT8*)&gpsDetSucc.longitude[0], MAX_SIZE_LOCATIONSTATUS_LONGITUDE + 1, "%f%s ", longitudeInfo.longitude,"E");
  }
  else
  {
	logError("%s", "Direction received in Longitude is wrong");
    createAndSendGpsDetailsFail( "Direction received in Longitude is wrong" );
    return dehContext->dehState;
  }

  /* Checking and formatting Latitude Information */
  if( latitudeInfo.gpsDirection EQ GPSDIRECTION_NORTH )
  {
	snprintf((UINT8*)&gpsDetSucc.latitude[0], MAX_SIZE_LOCATIONSTATUS_LATITUDE + 1, "%f%s ", latitudeInfo.latitude,"N");
  }
  else if( latitudeInfo.gpsDirection EQ GPSDIRECTION_SOUTH )
  {
	snprintf((UINT8*)&gpsDetSucc.latitude[0], MAX_SIZE_LOCATIONSTATUS_LATITUDE + 1, "%f%s ", latitudeInfo.latitude,"S");
  }
  else
  {
	logError("%s", "Direction received in Latitude is wrong");
    createAndSendGpsDetailsFail("Direction received in Latitude is wrong");
    return dehContext->dehState;
  }
  createAndSendMsg( MSG_DEH_BSM_GPS_DETAIL_SUCC, (UINT8*)&gpsDetSucc, sizeof(MsgDehBsmGpsDetailSucc) );
  return dehContext->dehState;
}

/**
 * @param   gpsDetSucc   GPS detail structure
 * @details This function will process the GPS Status Request message received from BSM.
 */
TBOOL dehProcGpsStatusReq(
 MsgDehBsmGpsStatusSucc  *gpsDetSucc
 )
{
  LatitudeInfo  latitudeInfo;
  LongitudeInfo longitudeInfo;
  DevStatus *presentStatus  = getDevicePresentStatusPtr();
  DehGpsContext  *dehGpsContext = getGlobalDehGpsContext();

  if(presentStatus->devConnectionStatus[DEV_GPS] NOTEQ DEV_CONNECTED)
  {
	logError("GPS not connected", NULL);
    return FALSE;
  }

  clearBytes( &longitudeInfo, sizeof(LongitudeInfo));
  clearBytes( &latitudeInfo, sizeof(LatitudeInfo));

  /*Copying latitude and longitude info into local context from DEH Context*/
  pthread_mutex_lock(&g_gpsMutex);
  latitudeInfo =  dehGpsContext->dehGpsLatitudeInfo;
  longitudeInfo = dehGpsContext->dehGpsLongitudeInfo;  

  /*Copying time and date info into GPS structure from DEH Context*/
  gpsDetSucc->time = dehGpsContext->dehGpsTimeInfo;
  gpsDetSucc->date = dehGpsContext->dehGpsDateInfo;
  pthread_mutex_unlock(&g_gpsMutex);

  /* Checking and formatting Longitude Information */
  if( longitudeInfo.gpsDirection EQ GPSDIRECTION_WEST )
  {
    snprintf((UINT8*)gpsDetSucc->longitude, MAX_SIZE_LOCATIONSTATUS_LONGITUDE + 1, "%f%s ", longitudeInfo.longitude,"W");
  }
  else if( longitudeInfo.gpsDirection EQ GPSDIRECTION_EAST )
  {
    snprintf((UINT8*)gpsDetSucc->longitude, MAX_SIZE_LOCATIONSTATUS_LONGITUDE + 1, "%f%s ", longitudeInfo.longitude,"E");
  }
  else
  {
    logError( "Direction received in Longitude is wrong", NULL);
    return FALSE;
  }

 /* Checking and formatting Latitude Information */
  if( latitudeInfo.gpsDirection EQ GPSDIRECTION_NORTH )
  {
	snprintf((UINT8*)gpsDetSucc->latitude, MAX_SIZE_LOCATIONSTATUS_LATITUDE + 1, "%f%s ", latitudeInfo.latitude,"N");
  }
  else if( latitudeInfo.gpsDirection EQ GPSDIRECTION_SOUTH )
  {
	snprintf((UINT8*)gpsDetSucc->latitude, MAX_SIZE_LOCATIONSTATUS_LATITUDE + 1, "%f%s ", latitudeInfo.latitude,"S");
  }
  else
  {
    logError("Direction received in Latitude is wrong", NULL);
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   gpsStatusSucc   GPS detail structure
 * @details This function will process the GPS Status response message received from GPS.
 */
void decGpsDehStatusInfoRsp(
 IO  MsgDehBsmGpsStatusSucc    *gpsStatusSucc,
 IO  GpsStatus *statusTable
 )
{
  strncpy( statusTable->gpsLatitude,  gpsStatusSucc->latitude,  MAX_SIZE_LOCATIONSTATUS_LATITUDE + 1);
  strncpy( statusTable->gpsLongitude, gpsStatusSucc->longitude, MAX_SIZE_LOCATIONSTATUS_LONGITUDE + 1);

  if (gpsStatusSucc->date.year < 100)
  {
	snprintf(&(statusTable->gpsDateAndTime[0]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1,"00%u",gpsStatusSucc->date.year);
  }
  else if (gpsStatusSucc->date.year < 1000)
  {
	snprintf(&(statusTable->gpsDateAndTime[0]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1, "0%u",gpsStatusSucc->date.year);
  }
  else
  {
	snprintf(&(statusTable->gpsDateAndTime[0]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1, "%u",gpsStatusSucc->date.year);
  }

  if (gpsStatusSucc->date.month < 10)
  {
	snprintf(&(statusTable->gpsDateAndTime[4]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 4, "0%u",gpsStatusSucc->date.month);
  }
  else
  {
	snprintf(&(statusTable->gpsDateAndTime[4]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 4, "%u",gpsStatusSucc->date.month);
  }

  if (gpsStatusSucc->date.date < 10)
  {
	snprintf(&(statusTable->gpsDateAndTime[6]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 6, "0%u",gpsStatusSucc->date.date);
  }
  else
  {
	snprintf(&(statusTable->gpsDateAndTime[6]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 6, "%u",gpsStatusSucc->date.date);
  }

  if (gpsStatusSucc->time.hours < 10)
  {
	snprintf(&(statusTable->gpsDateAndTime[8]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 8, "0%u",gpsStatusSucc->time.hours);
  }
  else
  {
	snprintf(&(statusTable->gpsDateAndTime[8]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 8, "%u",gpsStatusSucc->time.hours);
  }

  if (gpsStatusSucc->time.minutes < 10)
  {
	snprintf(&(statusTable->gpsDateAndTime[10]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 10, "0%u",gpsStatusSucc->time.minutes);
  }
  else
  {
	snprintf(&(statusTable->gpsDateAndTime[10]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 10,"%u",gpsStatusSucc->time.minutes);
  }

  if (gpsStatusSucc->time.seconds < 10)
  {
	snprintf(&(statusTable->gpsDateAndTime[12]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 12,"0%u",gpsStatusSucc->time.seconds);
  }
  else
  {
	snprintf(&(statusTable->gpsDateAndTime[12]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 12,"%u",gpsStatusSucc->time.seconds);
  }
  snprintf(&(statusTable->gpsDateAndTime[14]), MAX_SIZE_GPSSTATUS_GPSDATEANDTIME + 1 - 14, "%s","000");
}

/**
 * @details This function will Reset the GPS Device.
 */
TBOOL dehGpsReset()
{
  if ( GPSRETURNCODE_STATUS_SUCCESS EQ gpsReset() )
  {
	return TRUE;
  }
  return FALSE;
}
