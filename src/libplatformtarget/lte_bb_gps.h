#ifndef __GPSHANDLER_H__
#define __GPSHANDLER_H__

#define BAUDRATE    B38400
#define GPS_RETRY_COUNT 20

typedef enum e_GpsDirection
{
  GPSDIRECTION_NORTH =0,
  GPSDIRECTION_SOUTH,
  GPSDIRECTION_EAST,
  GPSDIRECTION_WEST
}GpsDirection;

typedef enum e_GpsReturnCode
{
  GPSRETURNCODE_STATUS_FAILURE = 10,
  GPSRETURNCODE_STATUS_SUCCESS,
  GPSRETURNCODE_STATUS_FALSE,
  GPSRETURNCODE_STATUS_TRUE,
  GPSRETURNCODE_INVALID_INPUT
}GpsReturnCode;

typedef struct s_DateInfo
{
   UINT16                     year;
   UINT8                      month;
   UINT8                      date;
}DateInfo;

typedef struct s_TimeInfo
{
   UINT8                      hours;
   UINT8                      minutes;
   UINT8                      seconds;
}TimeInfo;

typedef struct s_LatitudeInfo
{
    double                    latitude;
    GpsDirection              gpsDirection; // direction N or S 
}LatitudeInfo;

/* latitude  format will be whatever is received from GPS ,
EX . latitude is 1726.551546 and directions is  .N. or .S. */

typedef struct s_LongitudeInfo
{
   double                    longitude; 
   GpsDirection              gpsDirection;// direction E or W
}LongitudeInfo;

extern GpsReturnCode  isConnectedToSatellite (void);
extern GpsReturnCode  getDate              (DateInfo *pDateInfo);
extern GpsReturnCode  getTime              (TimeInfo *pTimeInfo);
extern GpsReturnCode  getLatitude          (LatitudeInfo *pLatitudeInfo);
extern GpsReturnCode  getLongitude         (LongitudeInfo *pLongitudeInfo);
extern GpsReturnCode  gpsReset 		   (void);

#endif  /* __GPSHANDLER_H__ */
