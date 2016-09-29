#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

#include "lte_bb_platform.h"
#include "lte_bb_platform_spi.h"


/****************************************************************************
*  Function:    isSystemClockStable() 
*
*  Description:
*
*  Prototype:   Ad9548ReturnCode isSystemClockStable()
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
Ad9548ReturnCode isSystemClockStable(void)
{
   return AD9548RETURNCODE_STATUS_STABLE;
}
	
/****************************************************************************
*  Function:    isFrequencyLocked() 
*
*  Description:
*
*  Prototype:   Ad9548ReturnCode isFrequencyLocked()
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
Ad9548ReturnCode isFrequencyLocked(void)
{
   return AD9548RETURNCODE_STATUS_LOCKED;
}
	
/****************************************************************************
*  Function:    activeReferenceToAd9548() 
*
*  Description:
*
*  Prototype:   Ad9548ReturnCode activeReferenceToAd9548 ()
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
Ad9548ReturnCode activeReferenceToAd9548(void)
{
   return AD9548RETURNCODE_CLK_SOURCE_2;
}
	
/****************************************************************************
*  Function:    isPllLocked() 
*
*  Description:
*
*  Prototype:   CdceReturnCode isPllLocked()
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
CdceReturnCode isPllLocked(void)
{
   return CDCERETURNCODE_STATUS_LOCKED;
}
	
/****************************************************************************
*  Function:    isLinkActive(EthernetLink linkNum)) 
*
*  Description:
*
*  Prototype:   EthReturnCode isLinkActive (EthernetLink linkNum)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
EthReturnCode isLinkActive(EthernetLink linkNum)
{
   return ETHRETURNCODE_STATUS_LINK_ACTIVE;
}
	
/****************************************************************************
*  Function:    isLinkActive(EthernetLink linkNum)) 
*
*  Description:
*
*  Prototype:   EthReturnCode isLinkActive (EthernetLink linkNum)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
EthReturnCode Switch_isLinkActive(EthernetLink linkNum)
{
	return ETHRETURNCODE_STATUS_LINK_ACTIVE;
}
/****************************************************************************
*  Function:  get_receive_error_counter_val(EthernetLink linkNum, int *error_count) 
*
*  Description:
*
*  Prototype:  EthReturnCode getReceiveErrorCounterVal(EthernetLink linkNum, int *error_count)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
EthReturnCode getReceiveErrorCounterVal (EthernetLink linkNum, UINT32 *error_count)
{
   return ETHRETURNCODE_STATUS_SUCCESS;
}

/********************************** GPS USER API's**************************************/	
/****************************************************************************
*  Function:  isConnetedToSatelite() 
*
*  Description:
*
*  Prototype: gpsReturnCode isConnetedToSatelite()
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/

GpsReturnCode isConnectedToSatellite(void)
{
   return GPSRETURNCODE_STATUS_TRUE;
}
	
/****************************************************************************
*  Function:  getDate (DateInfo *pDateInfo) 
*
*  Description:
*
*  Prototype: gpsReturnCode getDate (DateInfo *pDateInfo)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
GpsReturnCode getDate (DateInfo *pDateInfo)
{

  /* Do Sanity Check*/
  if(pDateInfo == NULL)
  {
     return GPSRETURNCODE_STATUS_FAILURE;
  }

  time_t currentTime = time(NULL);

  struct tm* tm = localtime(&currentTime);

  /* memset the given structure */
   memset(pDateInfo,'\0',sizeof(DateInfo));

   pDateInfo->year =  tm->tm_year + 1900; /* The number of years since 1900. +1900 */
   pDateInfo->month = tm->tm_mon + 1;/* The number of months since January, in the range 0 to 11. +1 */
   pDateInfo->date =  tm->tm_mday;

   return GPSRETURNCODE_STATUS_SUCCESS;
}

/****************************************************************************
*  Function   : getTime (TimeInfo *pTimeInfo) 
*
*  Description:
*
*  Prototype:   gpsReturnCode getTime (TimeInfo *pTimeInfo)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
GpsReturnCode getTime (TimeInfo *pTimeInfo)
{

  /* Do Sanity Check*/
  if(pTimeInfo == NULL)
  {
     return GPSRETURNCODE_STATUS_FAILURE;
  }

  time_t currentTime = time(NULL);

  struct tm* tm = localtime(&currentTime);
  /* memset the given structure */
   memset(pTimeInfo,'\0',sizeof(TimeInfo));

   pTimeInfo->hours   =  tm->tm_hour;
   pTimeInfo->minutes =  tm->tm_min;
   pTimeInfo->seconds =  tm->tm_sec;

   return GPSRETURNCODE_STATUS_SUCCESS;
}


/****************************************************************************
*  Function:    getLatitude (LatitudeInfo *pLatitudeInfo) 
*
*  Description:
*
*  Prototype:   gpsReturnCode  getLatitude (LatitudeInfo *pLatitudeInfo)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
GpsReturnCode  getLatitude (LatitudeInfo *pLatitudeInfo)
{

  /* Do Sanity Check*/
  if(pLatitudeInfo == NULL)
  {
     return GPSRETURNCODE_STATUS_FAILURE;
  }

  /* memset the given structure */
   memset(pLatitudeInfo,'\0',sizeof(LatitudeInfo));

   pLatitudeInfo->latitude       = 4123.100121;
   pLatitudeInfo->gpsDirection   = GPSDIRECTION_NORTH;

   return GPSRETURNCODE_STATUS_SUCCESS;
}

/****************************************************************************
*  Function:   getLongitude (struct LongitudeInfo *pLongitudeInfo)
*
*  Description:
*
*  Prototype:   gpsReturnCode   getLongitude (struct LongitudeInfo *pLongitudeInfo)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
GpsReturnCode   getLongitude (LongitudeInfo *pLongitudeInfo)
{

  /* Do Sanity Check*/
  if(pLongitudeInfo == NULL)
  {
     return GPSRETURNCODE_STATUS_FAILURE;
  }

  /* memset the given structure */
   memset(pLongitudeInfo,'\0',sizeof(LongitudeInfo));


   pLongitudeInfo->longitude        = 209.011612;
   pLongitudeInfo->gpsDirection     = GPSDIRECTION_EAST;

   return GPSRETURNCODE_STATUS_SUCCESS;
}

/****************************************************************************
*  Function:   gpsReset ()
*
*  Description:
*
*  Prototype:   gpsReturnCode   gpsReset ()
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     31/03/11
*
*  Originator:  PIPAD
****************************************************************************/
GpsReturnCode   gpsReset (void)
{
   return GPSRETURNCODE_STATUS_SUCCESS;
}

/********************************** LED USER API's**************************************/
	
/****************************************************************************
*  Function:     bbGetNumAvailableLed (void)
*
*  Description:
*
*  Prototype:   LedNum bbGetNumAvailableLed (void)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
LedNum bbGetNumAvailableLed (void)
{
   return LEDNUM_MAX_LED;
}
/****************************************************************************
*  Function:    bbGetLedState (LedNum ledNum)
*
*  Description:
*
*  Prototype:   LedReturnCode bbGetLedState (LedNum ledNum)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
LedReturnCode bbGetLedState (LedNum ledNum)
{
   if(ledNum >= LEDNUM_MAX_LED)
   {
       return LEDRETURNCODE_ERR_LED_INVALID_PARAM;
   }
   return LEDRETURNCODE_LED_STATUS_SUCCESS;

}
/****************************************************************************
*  Function:   bbSetLedState (LedNum ledNum, LedState ledState)
*
*  Description:
*
*  Prototype:   LedReturnCode bbSetLedState (LedNum ledNum,LedState ledState)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
LedReturnCode bbSetLedState (LedNum ledNum,LedState ledState)
{
   int led_fd;
   led_data set_led_state;
   int ret_val;

   if(ledNum >= LEDNUM_MAX_LED)
   {
       return LEDRETURNCODE_ERR_LED_INVALID_PARAM;
   }
   if(ledState >= LEDSTATE_MAX_STATE)
   {
       return LEDRETURNCODE_ERR_LED_INVALID_PARAM;
   }
   return LEDRETURNCODE_LED_STATUS_SUCCESS;
}

/********************************** TEMP USER API's**************************************/
/****************************************************************************
*  Function:   initTempSensor(void)
*
*  Description:
*
*  Prototype:   TempSensorReturnCode initTempSensor (void)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
TempSensorReturnCode initTempSensor (void)
{
   return TEMPSENSORRETURNCODE_STATUS_SUCCESS;
}
/****************************************************************************
*  Function:   readTempSensor(float *temperature)
*
*  Description:
*
*  Prototype:   TempSensorReturnCode readTempSensor(float *temperature)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
TempSensorReturnCode readTempSensor(float *temperature)
{
  *temperature = 40.5;
   return TEMPSENSORRETURNCODE_STATUS_SUCCESS;
}
