#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>

#include "lte_bb_gps_util.h"

#define GPS_FAILURE  -1
#define GPS_SUCCESS   0

/* Globals */
SINT16       m_GPSFileDescriptor;

/*
********************************************************************************
* NAME         : initGPS
*
* DESCRIPTION  : Opens the serial port (i.e ttyS0)
* INPUTS       : None
* OUTPUTS      :
* RETURN       : 1 in case of sucess -1 in case of ERROR
*
*
********************************************************************************/
SINT8 initGPS()
{
    return openGPSdevice();
}

/****************************************************************************
*  Function    : openGPSdevice()
*
*  Description : Process the RCM Driver Commands
*
*  Prototype   : unsigned char openGPSdevice()
*
*  Inputs      : None
*
*  Outputs     :     
*
*           Type   :  unsigned char    Error code:
*           Values :    1               Success
*                       0               Error
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
GpsReturnCode openGPSdevice()
{
    struct termios newt;

    m_GPSFileDescriptor = open (GPS_DEVICE_FILE,O_RDWR | O_NOCTTY );

    if (m_GPSFileDescriptor < 0)
    {
        GPS_CRIT_LOG("Failed To Open GPS Device\n");
        return GPSRETURNCODE_STATUS_FAILURE;
    }

    bzero ( &newt,sizeof ( newt ) );
    newt.c_lflag &= ~(ECHO | ICANON);
    newt.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newt.c_iflag = IGNPAR;

    newt.c_oflag = 0;
    newt.c_oflag &= ~(OPOST);

    newt.c_cc[VTIME]    = 5;   /* inter-character timer unused */
    newt.c_cc[VMIN]     = 0;   /* blocking read until 9 chars received */

    tcflush ( m_GPSFileDescriptor, TCIFLUSH );
    tcsetattr ( m_GPSFileDescriptor,TCSANOW,&newt );
    
    return GPSRETURNCODE_STATUS_SUCCESS;
}

/****************************************************************************
*  Function    :  closeGPSDevice()
*
*  Description :  closes the GPS device
*
*  Prototype   :  unsigned char closeGPSDevice()
*
*  Inputs      :  None
*
*  Outputs:     
*
*           Type:   unsigned char    Error code:
*           Values:     1               Success
*                       0               Error
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
GpsReturnCode closeGPSDevice()
{
    if(isGPSDeviceOpen())
    {
       GPS_DEBUG_LOG("closing GPS device \n");
       close(m_GPSFileDescriptor);
    }
    return GPSRETURNCODE_STATUS_SUCCESS;
}


/****************************************************************************
*  Function   : reOpenGPSDevice()
*
*  Description: Re-open the GPS device
*
*  Prototype  : unsigned char reOpenGPSDevice()
*
*  Inputs     : None
*
*  Outputs:     
*
*           Type:   unsigned char    Error code:
*           Values:     1               Success
*                       0               Error
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
SINT8 reOpenGPSDevice()
{
    if(isGPSDeviceOpen())
    {
        closeGPSDevice();
    }

    return openGPSdevice();
}
SINT8 isGPSDeviceOpen()
{
    if ( m_GPSFileDescriptor > 0 )
    {
        return 1;
    }
    return GPS_SUCCESS;
}

/****************************************************************************
*  Function    : readGPSData()
*
*  Description : reads the data from GPS
*
*  Prototype   : It reads gps Data from gps,parse & store 
*
*  Inputs:      
*       char * gpsBuf
*
*  Outputs:     
*
*       PARAMETERS:
*            char * gpsBuf contains gps data in (Latitude,N#Longitude,E#speed#
*                         DATE,TIME#)
*       RETURN :
*           Type:       int             Error code:
*           Values:     1               Success
*                       0               Error
*
*       [1] Read 1280 Bytes Character data from GPS
*       [2] Parse data by new line(\n)
*       [3] And in Each new line in starting  Look for "$GPRMC",
*           if found, Parse all "," separated words,then look for 2nd Element
*           If its "A" ,then this is not Valid GPRMC string.
*       [4] So repeat it ,till it gets valid data or end of the datas.
*       [5] If it gets valid data,then datas are converted
*           to appropriate form by calling subsequent functions.
*       [6] Date & Time are in six character format,speed in kmph
*       [7] Orange LED is ON ,If GPS data is valid else OFF.
*
*  $GPRMC,064157.00,A,1259.47296,N,07742.33268,E,0.578,93.20,010909,,,D*52
*  2004,2920010007,2580#12.991117,N#77.705582,E#000#010909,063938#
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
SINT8  readGPSData(GpsMessage *pGpsmessage)
{
    int           len=0;
    char         *token;
    char         *dataList[13];
    char         *ch;
    char         *ptmp;
    char         tmp[2];
    unsigned long temp_time=0;
    unsigned long temp_date=0;
    int i=0;

    memset(pGpsmessage,'\0',sizeof(GpsMessage));
    ch=(char *)malloc(1280*sizeof(char));//change for new kernel-21.7
    if(ch ==  NULL)
    {
        GPS_CRIT_LOG("Couldn't allocate the memory to GPS Buff\n");
        return 0;
    }
    memset(ch,'\0',1280);
    len = 0;
    len=read(m_GPSFileDescriptor,ch,1280);

    GPS_DEBUG_LOG("\n In getGPSData\n");
    GPS_DEBUG_LOG("GPS Data is %s \n",ch);

    while(strlen(ch) > 68)
    {
        token=strtok(ch,"\n");
        while(token!=NULL)
        {
            if(!strncmp("$GPRMC",token,6))
            {
                if(strlen(token) < 68)
                {
                    GPS_CRIT_LOG("Incomplete GPRMC string\n");
                    return 0;
                }
                
                dataList[0]=strtok(token,",");
                for(i = 1; i < 13; i++)
                {
                    dataList[i] = strtok(NULL,",");
                }
                //if(strcmp(dataList[2],"A"))
                if(strcmp(dataList[2],"V"))
                {
                        GPS_CRIT_LOG("GPRMC Data is not Valid\n");
                        pGpsmessage->fix = 0; 
                        /* GPRMC command found but no fix */
                        //return 0;
                }
                else
                {
                    pGpsmessage->fix = 1; 
                }

                pGpsmessage->latitude = atof(dataList[3]);
                pGpsmessage->longitude = atof(dataList[5]);
                
                temp_date=atoi(dataList[9]);
                if(validate_date(temp_date) < 0 )
                {
                    GPS_DEBUG_LOG("FALSE--date\n");
                    return 0;
                }
                /* caluculate date */
                ptmp = dataList[9];

                tmp[0] = ptmp[0];
                tmp[1] = ptmp[1];

                pGpsmessage->date =atoi(tmp); 

                /* caluculate month*/
                tmp[0] = ptmp[2];
                tmp[1] = ptmp[3];

                pGpsmessage->month =atoi(tmp); 
             
                tmp[0] = ptmp[4];
                tmp[1] = ptmp[5];

                /* caluculate year */
                pGpsmessage->year =atoi(tmp); 
                
                temp_time=atoi(dataList[1]);
                if(validate_time(temp_time) < 0)
                {
		    GPS_DEBUG_LOG("FALSE--time\n");
	            return 0;
                }
                /* caluculate hours */
                ptmp =dataList[1];

                tmp[0] = ptmp[0];
                tmp[1] = ptmp[1];

                pGpsmessage->hours =atoi(tmp); 

                /* caluculate min*/
                tmp[0] = ptmp[2];
                tmp[1] = ptmp[3];

                pGpsmessage->minutes =atoi(tmp); 
             
                tmp[0] = ptmp[4];
                tmp[1] = ptmp[5];

                /* caluculate seconds  */
                pGpsmessage->seconds =atoi(tmp);

                if(*dataList[4]!='N' && *dataList[4]!='S')
                {
                    GPS_DEBUG_LOG("FALSE--nind\n");
            	    return 0;
                }
                if(*dataList[4] == 'N')
                {
                    pGpsmessage->nind =GPSDIRECTION_NORTH;
                }   
                if(pGpsmessage->nind == 'S')
                {
                    pGpsmessage->nind =GPSDIRECTION_SOUTH;
                }

                if(*dataList[6]!='E' && *dataList[6]!='W')
                {
                    GPS_DEBUG_LOG("FALSE--eind\n");
                    return 0;
                }
                if(*dataList[6] == 'E')
                {
                    pGpsmessage->eind =GPSDIRECTION_EAST;
                } 
                if(*dataList[6] == 'W')
                {
                    pGpsmessage->eind =GPSDIRECTION_WEST;
                }  
                GPS_DEBUG_LOG("data recvd latitude & longitue ,date ,time [%s:%s:%s:%s] is\n ",dataList[3],dataList[5],dataList[8],dataList[1]);
                free(ch);
                return 1;
            }
            token=strtok(NULL,"\n");
        }
    	free(ch);//To check ..
    	GPS_DEBUG_LOG("GPRMC string not found...\n");
    	return 0;
    }
    free(ch);//To check ..
    GPS_DEBUG_LOG("GPRMC string not found...\n");
    return 0;
}
/****************************************************************************
*  Function   : validate_date()
*
*  Description: Validates the date extraced from GPS
*
*  Prototype  : SINT8 validate_date(unsigned long date)
*
*  Inputs     : None
*
*  Outputs:     
*
*           Type:   unsigned char    Error code:
*           Values:     1               Success
*                       0               Error
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
SINT8 validate_date(unsigned long date)
{
   SINT32 year,month;

   year = date%100;
   if(year < 9 || year > 99)
   {
      return GPS_FAILURE;
   }
   date = date/100;
   month = date%100;
   if(month < 1 || month > 12)
   {
      return GPS_FAILURE;
   }
   date = date/100;
   if(date < 1 || date > 31)
   {
      return GPS_FAILURE;
   }
   return GPS_SUCCESS;
}
/****************************************************************************
*  Function   : validate_time()
*
*  Description: Validates the Time
*
*  Prototype  : SINT8 validate_time(unsigned long time)
*
*  Inputs     : None
*
*  Outputs:     
*
*           Type:   unsigned char    Error code:
*           Values:     1               Success
*                       0               Error
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
SINT8 validate_time(unsigned long time)
{
   SINT32 hour,min,sec;
   sec=time%100;

   if(sec < 0 || sec > 59)
   {
      return GPS_FAILURE;
   }
   time=time/100;
   min=time%100;
   if(min < 0 || min > 59)
   { 
      return GPS_FAILURE;
   }
   hour=time/100;
   if(hour < 0 || hour > 23)
   {
      return GPS_FAILURE;
   }
   return GPS_SUCCESS;
}

/****************************************************************************
*  Function   : writeToGPSDevice(char *buff, int length)
*
*  Description: Writes to the GPS device
*
*  Prototype  : int writeToGPSDevice(char * buff, int length)
*
*  Inputs     : 
*          
*           Type:   char *                   length
*           Value:  Address of the buffer    length of the data
*
*  Outputs:     
*
*           Type:       int            Error code:
*           Values:     0               Success
*                       -ve               Error
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
int writeToGPSDevice(unsigned char *buff, int length)
{
    int ret_val;
    ret_val = write(m_GPSFileDescriptor,buff,length);
    if(ret_val < 0) {
        return GPS_FAILURE;
    }
    return GPS_SUCCESS;
}
