#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <time.h>
#include <sys/ioctl.h>


#include "lte_bb_platform.h"
//#include "lte_bb_platform_spi.h"
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
   UINT8 RegData;
   SINT8 ret;
   ret=ReadAD9548(SYS_CLK_AD9548_REGISTER,&RegData);
   if(ret<0)
   {
	return ret;
   }
   if((RegData & 0x10) == 0x10)
   {
      return AD9548RETURNCODE_STATUS_STABLE;
   }
   else
   {
      return AD9548RETURNCODE_STATUS_UNSTABLE;
   }
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
   UINT8 RegData;
   SINT8 ret;
   ret=ReadAD9548(SYS_FREQ_AD9548_REGISTER,&RegData);
   if(ret<0)
   {
	return ret;
   }
   if((RegData & 0x20) == 0x20)
   {
      return AD9548RETURNCODE_STATUS_LOCKED;
   }
   else
   {
       return AD9548RETURNCODE_STATUS_NOT_LOCKED;
   }
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
   UINT8 RegData;
   SINT8 ret;
   ret=ReadAD9548(DLL_AD9548_REGISTER,&RegData);
   if(ret<0)
   {
	return ret;
   }
   if((RegData & 0x07) == 0x0)
   {
      return AD9548RETURNCODE_GPS_SIGNAL_SOURCE;
   }
   if((RegData & 0x07) == 0x2)
   {
      return AD9548RETURNCODE_CLK_SOURCE_2;
   }
   return ERROR;
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
   UINT32 regValue;
   SINT8 ret;
   ret=ReadCDCE620005(CDCE_REG008_REGISTER,&regValue);
   if(ret<0)
   {
	return ret;
   }
   if((regValue & 0x40) == 0x40)
   {
	return CDCERETURNCODE_STATUS_LOCKED;
   }
   else
   {
        return CDCERETURNCODE_STATUS_NOT_LOCKED;
   }
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
   UINT16 value;
   UINT32 reg=1;
   SINT16 fileDescriptor;
   SINT8 ret;
   unsigned char temp[100];
   int ret1;
   char ifname[5];
   snprintf(ifname,5,"%s%d","eth",linkNum);
   if(linkNum == ETHERNETLINK_BACK_HAUL)
   {
        if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket");
            close(fileDescriptor);
            return ETHRETURNCODE_ERR_MDIO_DRV_OPEN;    
        }
        if (do_getphy(fileDescriptor, ifname))
        {
           close(fileDescriptor);
           return ETHRETURNCODE_STATUS_LINK_INACTIVE;
        }
        ret=mdio_read(fileDescriptor, reg, &value);
        if(ret<0)
        {
	    close(fileDescriptor);
	    return ETHRETURNCODE_ERR_MDIO_DRV_OPEN;
        }
	snprintf(temp,90,"%s","route -n| grep eth2 >> /dev/null");
        ret1=system(temp);
        if(((value & (1<<2)))!=0 && (ret1==0))
        {
            close(fileDescriptor);
            return ETHRETURNCODE_STATUS_LINK_ACTIVE; 
        }
	else
	{
        	close(fileDescriptor);
        	return ETHRETURNCODE_STATUS_LINK_INACTIVE;
	}
   }
   return ETHRETURNCODE_ERR_INVALID_PARAMETER;
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
   UINT16 value;
   UINT32 reg=1;
   UINT32 phy_id=0;
   SINT16 fileDescriptor;
   SINT8 ret;
   int ret1;
   char ifname[5];
   char temp[100];
   int switch_port=1;
   snprintf(ifname,5,"%s%d","eth",switch_port);
   if((linkNum <ETHERNETLINK_DEBUG_PORT) && (linkNum > ETHERNETLINK_LK_PORT))
   {
	return ETHRETURNCODE_ERR_INVALID_PARAMETER;
   }
   if(linkNum == ETHERNETLINK_DEBUG_PORT)
   {
       phy_id=0;
   }
  else if(linkNum == ETHERNETLINK_WIFI_PORT)
  {
	phy_id=1;
  }
  else if(linkNum == ETHERNETLINK_LK_PORT)
  {
	phy_id=2;
  }
        if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket");
            close(fileDescriptor);
            return ETHRETURNCODE_ERR_MDIO_DRV_OPEN;    
        }
        if (switch_getphy(ifname))
        {
           close(fileDescriptor);
           return ETHRETURNCODE_ERR_INVALID_PARAMETER;
        }
        ret=switch_read(fileDescriptor, reg, phy_id, &value);
        if(ret<0)
        {
	    close(fileDescriptor);
	    return ETHRETURNCODE_ERR_MDIO_DRV_OPEN;
        }
	snprintf(temp,90,"%s","route -n| grep eth1 >> /dev/null");
        ret1=system(temp);
        if(ret1 ==0)
        {
            close(fileDescriptor);
            return ETHRETURNCODE_STATUS_LINK_ACTIVE; 
        }
	else
 	{
		close(fileDescriptor);
		return ETHRETURNCODE_STATUS_LINK_INACTIVE;
	}
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
   UINT32 reg=21;
   SINT16 fileDescriptor;
   SINT8 ret;
   UINT16 count;
   char ifname[5];
   snprintf(ifname,5,"%s%d","eth",linkNum);
   if(linkNum == ETHERNETLINK_BACK_HAUL)
   {
        if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket");
            close(fileDescriptor);
            return ETHRETURNCODE_ERR_MDIO_DRV_OPEN;
        }
        if (do_getphy(fileDescriptor, ifname))
        {
           close(fileDescriptor);
           return ETHRETURNCODE_ERR_INVALID_PARAMETER;
        }
        ret=mdio_read(fileDescriptor, reg, &count);
        if(ret<0)
        {
            close(fileDescriptor);
            return ETHRETURNCODE_ERR_MDIO_DRV_OPEN;
        }

	*error_count = (UINT32)count;
   }
   return ETHRETURNCODE_STATUS_SUCCESS;

}

/********************************** GPS USER API's**************************************/	
/****************************************************************************
*  Function:  isConnectedToSatelite() 
*
*  Description:
*
*  Prototype: gpsReturnCode isConnectedToSatelite()
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

   GpsMessage tmpGpsmessage;
   memset(&tmpGpsmessage,'\0',sizeof(tmpGpsmessage));
   int i;
   struct timespec time_delay;
   struct timespec rem_delay;
   /* Try till command is found */
   time_delay.tv_sec = 0;
   time_delay.tv_nsec = 100000000;   

   for(i = 0 ; i < GPS_RETRY_COUNT ; i++)
   {
       if( readGPSData(&tmpGpsmessage) >=1 )
       {
          if(tmpGpsmessage.fix == 1)
          {
             GPS_DEBUG_LOG("isConnetedToSatelite SUCESS ");
             return GPSRETURNCODE_STATUS_TRUE;
          }
          else
          {
             GPS_DEBUG_LOG("isConnetedToSatelite FAILURE \n");
             return  GPSRETURNCODE_STATUS_FALSE;
          }
       }
       nanosleep(&time_delay, &rem_delay);
    }
    return  GPSRETURNCODE_STATUS_FAILURE ;
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
   GpsMessage tmpGpsmessage;
   int i;
   struct timespec time_delay;
   struct timespec rem_delay;
 
   /* Do Sanity Check*/	 
   if(pDateInfo == NULL)
   {
     return GPSRETURNCODE_INVALID_INPUT;
   }

   memset(&tmpGpsmessage,'\0',sizeof(tmpGpsmessage));
   memset(pDateInfo,'\0',sizeof(DateInfo));

   time_delay.tv_sec = 0;
   time_delay.tv_nsec = 100000000;   

   /* Try till command is found */
   for(i = 0 ; i < GPS_RETRY_COUNT ; i++)
   {
       if( readGPSData(&tmpGpsmessage) >=1 )
       {
          if(tmpGpsmessage.fix == 1)
          {
            /* fill the structure */
             pDateInfo->year  = tmpGpsmessage.year;
             pDateInfo->month = tmpGpsmessage.month;
             pDateInfo->date  = tmpGpsmessage.date;

             GPS_DEBUG_LOG(" %s SUCESS \n",__func__);
             return GPSRETURNCODE_STATUS_SUCCESS;
          }
          else
          {
             GPS_DEBUG_LOG(" %s FAILURE \n",__func__);
             return  GPSRETURNCODE_STATUS_FAILURE ;
          }
       }
       nanosleep(&time_delay, &rem_delay);
    }
    return  GPSRETURNCODE_STATUS_FAILURE ;
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
   GpsMessage tmpGpsmessage;
   int i;
   struct timespec time_delay;
   struct timespec rem_delay;

   /* Do Sanity Check*/	 
   if(pTimeInfo == NULL)
   {
     return GPSRETURNCODE_INVALID_INPUT;
   }

   memset(&tmpGpsmessage,'\0',sizeof(tmpGpsmessage));
   memset(pTimeInfo,'\0',sizeof(TimeInfo));

   time_delay.tv_sec = 0;
   time_delay.tv_nsec = 100000000;   

   /* Try till command is found */
   for(i = 0 ; i < GPS_RETRY_COUNT ; i++)
   {
       if( readGPSData(&tmpGpsmessage) >=1 )
       {
          if(tmpGpsmessage.fix == 1)
          {
            /* fill the structure */
             pTimeInfo->hours   = tmpGpsmessage.hours;
             pTimeInfo->minutes = tmpGpsmessage.minutes;
             pTimeInfo->seconds = tmpGpsmessage.seconds;

             GPS_DEBUG_LOG(" %s SUCESS \n",__func__);
             return GPSRETURNCODE_STATUS_SUCCESS;
          }
          else
          {
             GPS_DEBUG_LOG(" %s FAILURE \n",__func__);
             return  GPSRETURNCODE_STATUS_FAILURE ;
          }
       }
       nanosleep(&time_delay, &rem_delay);
    }
    return  GPSRETURNCODE_STATUS_FAILURE ;
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
   GpsMessage tmpGpsmessage;
   int i;
   struct timespec time_delay;
   struct timespec rem_delay;

   /* Do Sanity Check*/	 
   if(pLatitudeInfo == NULL)
   {
      return GPSRETURNCODE_INVALID_INPUT;
   }

   memset(&tmpGpsmessage,'\0',sizeof(tmpGpsmessage));
   memset(pLatitudeInfo,'\0',sizeof(LatitudeInfo));

   time_delay.tv_sec = 0;
   time_delay.tv_nsec = 100000000;   

   /* Try till command is found */
   for(i = 0 ; i < GPS_RETRY_COUNT ; i++)
   {
       if( readGPSData(&tmpGpsmessage) >=1 )
       {
          if(tmpGpsmessage.fix == 1)
          {
            /* fill the structure */
             pLatitudeInfo->latitude = tmpGpsmessage.latitude;
             pLatitudeInfo->gpsDirection    = tmpGpsmessage.nind;

             GPS_DEBUG_LOG(" %s SUCESS \n",__func__);
             return GPSRETURNCODE_STATUS_SUCCESS;
          }
          else
          {
             GPS_DEBUG_LOG(" %s FAILURE \n",__func__);
             return  GPSRETURNCODE_STATUS_FAILURE ;
          }
       }
       nanosleep(&time_delay, &rem_delay);
    }
    return  GPSRETURNCODE_STATUS_FAILURE ;
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
   GpsMessage tmpGpsmessage;
   int i;
   struct timespec time_delay;
   struct timespec rem_delay;


   /* Do Sanity Check*/	 
   if(pLongitudeInfo == NULL)
   {
     return GPSRETURNCODE_INVALID_INPUT;
   }

   memset(&tmpGpsmessage,'\0',sizeof(tmpGpsmessage));
   memset(pLongitudeInfo,'\0',sizeof(LongitudeInfo));

   time_delay.tv_sec = 0;
   time_delay.tv_nsec = 100000000;   

   /* Try till command is found */
   for(i = 0 ; i < GPS_RETRY_COUNT ; i++)
   {
       if( readGPSData(&tmpGpsmessage) >=1 )
       {
          if(tmpGpsmessage.fix == 1)
          {
            /* fill the structure */
             pLongitudeInfo->longitude = tmpGpsmessage.longitude;
             pLongitudeInfo->gpsDirection     = tmpGpsmessage.eind;

             GPS_DEBUG_LOG(" %s SUCESS \n",__func__);
             return GPSRETURNCODE_STATUS_SUCCESS;
          }
          else
          {
             GPS_DEBUG_LOG(" %s FAILURE \n",__func__);
             return  GPSRETURNCODE_STATUS_FAILURE ;
          }
       }
       nanosleep(&time_delay, &rem_delay);
    }
    return  GPSRETURNCODE_STATUS_FAILURE ;
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
    openGPSdevice();

    unsigned char reset_cmd[50];
    reset_cmd[0] = 0x24;
    reset_cmd[1] = 'P';
    reset_cmd[2] = 'R';
    reset_cmd[3] = 'T';
    reset_cmd[4] = 'H';
    reset_cmd[5] = 'R';
    reset_cmd[6] = ',';
    reset_cmd[7] = 'R';
    reset_cmd[8] = 'S';
    reset_cmd[9] = 'E';
    reset_cmd[10] = 'T';
    reset_cmd[11] = ',';
    reset_cmd[12] = 0x0D;
    reset_cmd[13] = 0x0A;


   if(writeToGPSDevice(reset_cmd, 15) < 0 )
   {
      GPS_DEBUG_LOG(" %s FAILURE \n",__func__);
      closeGPSDevice();
      return  GPSRETURNCODE_STATUS_FAILURE ;
   }
   closeGPSDevice();
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
   int led_fd;
   led_data get_led_state;
   int ret_val;

   if(ledNum >= LEDNUM_MAX_LED)
   {
       return LEDRETURNCODE_ERR_LED_INVALID_PARAM;
   }

   led_fd = open(LED_DEVICE_FILE,O_RDWR);
   if(led_fd < 0)
   {
       return LEDRETURNCODE_ERR_LED_DRV_OPEN;
   }
   get_led_state.led_num = ledNum;
   
   ret_val = read(led_fd,(void*)&get_led_state, sizeof(led_data));
   if(ret_val < 0)
   {
       close(led_fd);
       return LEDRETURNCODE_LED_STATE_UNKNOWN;
   }
   close(led_fd);   
   if(get_led_state.led_state == LEDSTATE_OFF)
       return LEDRETURNCODE_LED_STATE_OFF;
   else if(get_led_state.led_state == LEDSTATE_RED)
       return LEDRETURNCODE_LED_STATE_RED;
   else if(get_led_state.led_state == LEDSTATE_GREEN)
       return LEDRETURNCODE_LED_STATE_GREEN;
   else
       return LEDRETURNCODE_LED_STATE_AMBER;
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
   led_fd = open(LED_DEVICE_FILE,O_RDWR);
   if(led_fd < 0)
   {
       return LEDRETURNCODE_ERR_LED_DRV_OPEN;
   }
   set_led_state.led_num = ledNum;
   set_led_state.led_state = ledState;
   ret_val = write(led_fd, (void *)&set_led_state, sizeof(led_data));
   if(ret_val < 0)
   {
        close(led_fd);
        return LEDRETURNCODE_LED_STATE_UNKNOWN;
   }
   close(led_fd);
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

   SINT16  fileDescriptor;
   UINT8   buff[2]; 

   if ((fileDescriptor = open(I2C_BUS_DEV,O_RDWR)) < 0 )
   {
      return TEMPSENSORRETURNCODE_STATUS_FAILURE; 
   }

   if(ioctl( fileDescriptor, I2C_SLAVE_FORCE , I2C_ADDR_LTE_TEMP_SENSOR) < 0)
   {
      close(fileDescriptor);
      return TEMPSENSORRETURNCODE_STATUS_FAILURE; 
   }
 
   /* write R0 & R1 for better precision */
   buff[0] =LTE_TEMP_CONF_REG_ADDR;
   buff[1] =LTE_TEMP_CONF_DATA;

   if(write(fileDescriptor,buff,2) < 0)
   {
      close(fileDescriptor);
      return TEMPSENSORRETURNCODE_STATUS_FAILURE; 
   }
   buff[0] =LTE_TEMP_REG_ADDR;
   buff[1] =LTE_TEMP_CONF_DATA;


   /* Dummy write ,Bring back to fd to starting position*/
   write(fileDescriptor,buff,2);
   
   close(fileDescriptor);
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
   SINT16  fileDescriptor;
   UINT8   buff[2]; 
   SINT16  retry=3;
   UINT16  temp_hex;

   if ((fileDescriptor = open(I2C_BUS_DEV,O_RDWR)) < 0 )
   {
      return TEMPSENSORRETURNCODE_STATUS_FAILURE; 
   }

   if(ioctl( fileDescriptor, I2C_SLAVE_FORCE , I2C_ADDR_LTE_TEMP_SENSOR) < 0)
   {
      close(fileDescriptor);
      return TEMPSENSORRETURNCODE_STATUS_FAILURE; 
   }
   sleep(1);
 
   while(read(fileDescriptor, buff, 2) < 0)
   {
      GPS_DEBUG_LOG("read failed \n");
      if(retry >= 0 )
      {
          retry--;
      }
      else
      {
          close(fileDescriptor);
          return TEMPSENSORRETURNCODE_STATUS_FAILURE;
      }
    }
   temp_hex = (buff[0] << 8) | buff[1]; 

   /*! only high 12 bits of the returned 16 bits are used  */
   temp_hex = temp_hex >> 4 ; 
   GPS_DEBUG_LOG("Temp hex is %x \n",temp_hex);

    /*!  total range of 0-128(0x00-0x80) corresponds to value 0x000-0x7ff read from sensor
    *  ie, temp_hex * (0x80/0x7ff) = actual temperature
    *  ie, temp_hex * 0.0625 = actual temperature
    */

    /*!  total range of 0-(-128) corresponds to value 0xfff-0x800 read from sensor
    *  ie, (0x1000 - temp_hex) * 0.0625 = actual temperature
    */

   if(temp_hex <= 0x7ff)
   {
      *temperature = temp_hex * LTE_TEMP_PRECISION_FACTOR;
   }
   else
   {
      *temperature = (0x1000 - temp_hex ) * LTE_TEMP_PRECISION_FACTOR * (-1);
   }

   close(fileDescriptor);
   return TEMPSENSORRETURNCODE_STATUS_SUCCESS;
}
