/**
 * @file    deh_temp.c
 * @brief This file is contains code get detail from Temperature Sensor Module
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author kgummad
 * @date   29-07-2011
 * @details 1)Temperature Sensor supported added in DEH.
 *
 */

#include "deh.h"
#include "lte_bb_platform.h"

/**
 * @details This function will Initialise the Temperature Sensor.
 */
TBOOL dehInitTempSensor(void)
{
  if( TEMPSENSORRETURNCODE_STATUS_SUCCESS EQ initTempSensor())
  {
	logInfo("Temperature Sensor Initialisation Success !!", NULL);
	return TRUE;
  }
  logError("Temperature Sensor Initialisation Fail !!", NULL);
  return FALSE;
}


/**
 * @param   temperature   contains temperature
 * @details This function will check whether Temperature Sensor is Working and read the temperature .
 */
TBOOL dehIsTempSensorActive()
{
  DehContext *dehContext = getDehContext();

  if( TEMPSENSORRETURNCODE_STATUS_SUCCESS EQ readTempSensor(&(dehContext->temperature)))
  {
	logInfo("Temperature Sensor is Up", NULL);
	return TRUE;
  }
  return FALSE;
}

/**
 * @param   temperature   contains temperature
 * @details This function will return the error string if Temperature Sensor is not Working.
 */
UINT8* dehTempSensorErrorString()
{
  REAL temperature = 0;
  if( TEMPSENSORRETURNCODE_STATUS_FAILURE EQ readTempSensor(&temperature))
  {
	logError("Temperature Sensor is Down", NULL);
	return "Inactive";
  }
  logError("Unable to read Temperature Sensor Status", NULL);
  return "Device Malfunction";
}
