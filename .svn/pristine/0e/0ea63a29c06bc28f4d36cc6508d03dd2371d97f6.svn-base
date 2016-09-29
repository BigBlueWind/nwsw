#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

#include "lte_bb_gps_util.h"



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
    return GPS_SUCCESS;
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
SINT8 openGPSdevice()
{
    struct termios newt;

    m_GPSFileDescriptor = open (GPS_DEVICE_FILE,O_RDWR | O_NOCTTY );

    if (m_GPSFileDescriptor < 0)
    {
        GPS_CRIT_LOG("Failed To Open GPS Device\n");
        return GPS_FAILURE;
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
    
    return GPS_SUCCESS;
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
SINT8 closeGPSDevice()
{
    if(isGPSDeviceOpen())
    {
       GPS_DEBUG_LOG("closing GPS device \n");
       close(m_GPSFileDescriptor);
    }
    return 1;
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
        //printf("GPS Device is already opened\n");
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
    unsigned int  len=0;
    UINT8         *token;
    UINT8         *temp;
    UINT8         *temp1;
    UINT8         *dataList[10];
    UINT8         *ch;
    int           length;
    double        test1;
    UINT8         *ptmp;
    UINT8         tmp[2];
    unsigned long temp_time=0;
    unsigned long temp_date=0;
    int i=0;

    ch=(char *)malloc(1280*sizeof(char));//change for new kernel-21.7
    if(ch ==  NULL)
    {
        GPS_CRIT_LOG("Couldn't allocate the memory to GPS Buff\n");
        return 0;
    }
    len=read(m_GPSFileDescriptor,ch,1280);

    GPS_DEBUG_LOG("\n In getGPSData\n");
    GPS_DEBUG_LOG("GPS Data is %s \n",ch);

    if(len>50)
    {
    printf("reading characters \n ");
    token=strtok(ch,"\n");
    while(token!=NULL)
    {
        if(!strncmp("$GPRMC",token,6))
        {
            temp=strtok(token,",");
            dataList[0]=temp;
            for(i=1;i<8;i++)
            {
                temp=strtok(NULL,",");
                dataList[i]=temp;
                if(i==2)
                {
                    if(strcmp(dataList[i],"V"))
                    {
                        GPS_CRIT_LOG("GPRMC Data is not Valid\n");
                        pGpsmessage->fix = 0; 
                        /* GPRMC command found but no fix */
                        return 1;
                    }
                    else
                    {
                     
                        pGpsmessage->fix = 1; 
                    } 
                }
                if(i==7)
                {
                    temp=strtok(NULL,"\n");
                    if(!strncmp(",",temp,1))
                    {
                        temp1=strtok(temp,",");
                        dataList[8]=temp1;
                    }
                    else
                    {
                        temp1=strtok(temp,",");
                        temp1=strtok(NULL,",");
                        dataList[8]=temp1;
                    }

                }
            }

            pGpsmessage->latitude = atof(dataList[3]);
            pGpsmessage->longitude = atof(dataList[5]);
            GPS_DEBUG_LOG("data recvd latitude & longitue ,date ,time [%s:%s:%s:%s] is\n ",dataList[3],dataList[5],dataList[8],dataList[1]);

            temp_date=atoi(dataList[8]);
            
            if(validate_date(temp_date) < 0 )
            {
               GPS_DEBUG_LOG("FALSE--date\n");
               return 0;
            }

            /* caluculate date */
            ptmp =dataList[8];

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
            free(ch);//To check ..
            return 1;
        }
        token=strtok(NULL,"\n");
    }
    free(ch);//To check ..
  }
else
{
   GPS_DEBUG_LOG(" GPS recvd lenght is less than 0 \n");
   return -2;
}
    return GPS_SUCCESS;
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
int writeToGPSDevice(char *buff, int length)
{
    int ret_val;
    ret_val = write(m_GPSFileDescriptor,buff,length);
    if(ret_val < 0)
        return -1;

    return 0;
}
