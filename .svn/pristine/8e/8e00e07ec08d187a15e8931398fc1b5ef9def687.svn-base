#ifndef LTE_BB_GPS_UTIL_H
#define LTE_BB_GPS_UTIL_H

#include "basic_types.h"
#include "lte_bb_gps.h"
//#define DEBUG_GPS

#ifdef DEBUG_GPS
#define GPS_DEBUG_LOG(format...)          syslog(LOG_DEBUG, format)
#define GPS_ERR_LOG(format...)            syslog(LOG_ERR,  format)
#define GPS_CRIT_LOG(format...)           syslog(LOG_CRIT, format)
#else
#define GPS_DEBUG_LOG(format...)
#define GPS_ERR_LOG(format...)            
#define GPS_CRIT_LOG(format...)           
#endif

#define GPS_DEVICE_FILE  "/dev/ttyS1"

typedef struct e_GpsMessage
{
   double        latitude;
   double        longitude;
   UINT16        year;
   UINT8         month;
   UINT8         date;
   UINT8         hours;
   UINT8         minutes;
   UINT8         seconds;
   GpsDirection  nind;
   GpsDirection  eind;
   UINT16        fix;
}GpsMessage;

extern SINT8 initGPS();
extern SINT8 validate_date(unsigned long date);
extern SINT8 validate_time(unsigned long time);
extern SINT8 readGPSData(GpsMessage *pGpsmessage);
extern int writeToGPSDevice(unsigned char *buff, int length);
extern SINT8 isGPSDeviceOpen();
extern SINT8 reOpenGPSDevice();
extern GpsReturnCode closeGPSDevice();
extern GpsReturnCode openGPSdevice();
#endif /* LTE_BB_GPS_UTIL_H*/

