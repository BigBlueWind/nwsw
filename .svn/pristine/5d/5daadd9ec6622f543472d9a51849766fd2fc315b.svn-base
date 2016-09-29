/*
 * ueh_cm_func.c
 *
 *  Created on: 01-Jun-2011
 *      Author: mkata
 */

#include "cm_func.h"




/*
*
*       Fun:  rdConQ
*
*       Desc:
*
*       Ret:   ROK, RFAILED
*
*       File:  ueh_cm_func.c TODO : This function should be in Common folder
*
*/

PUBLIC S16 rdConQ
(
Data data
)
{
   U32  memAvail;
   if ((data == 0x03) ||
       (data == 27))
   {
      printf("wrong key hit \n");
   }
   switch (data)
   {
      case 'm' :
      case 'M' :
         SRegInfoShow(DFLT_REGION, &memAvail);
         break;
      case 'q' :
      case 'Q' :
         exit(0);
         break;

      default :
         break;
   }

   RETVALUE(ROK);
}

void fillDataAndTimeStampField(
  UINT8   *dataAndTimeStampField
  )
{
  struct timeval  tv;
  struct timezone tz;
  struct tm* tm;
  long   milliSeconds = 0;

  UINT8 month[3], date[3], hour[3], min[3], sec[3];
  clearBytes( &month, 3 );
  clearBytes( &date, 3 );
  clearBytes( &hour, 3 );
  clearBytes( &min, 3 );
  clearBytes( &sec, 3 );

  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);
  /* Compute milliseconds from microseconds. */
  milliSeconds = tv.tv_usec / 1000;

  if ( (tm->tm_mon+1) < 10 )  snprintf((char *)month, 3, "0%d", (tm->tm_mon+1));
  else                        snprintf((char *)month, 3, "%d",  (tm->tm_mon+1));

  if ( tm->tm_mday < 10 )     snprintf((char *)date, 3, "0%d", tm->tm_mday);
  else                        snprintf((char *)date, 3, "%d",  tm->tm_mday);

  if ( tm->tm_hour < 10 )     snprintf((char *)hour, 3, "0%d", tm->tm_hour);
  else                        snprintf((char *)hour, 3, "%d",  tm->tm_hour);

  if ( tm->tm_min < 10 )      snprintf((char *)min, 3, "0%d", tm->tm_min);
  else                        snprintf((char *)min, 3, "%d",  tm->tm_min);

  if ( tm->tm_sec < 10 )      snprintf((char *)sec, 3, "0%d", tm->tm_sec);
  else                        snprintf((char *)sec, 3, "%d",  tm->tm_sec);

  snprintf((char *)dataAndTimeStampField, MAX_SIZE_ACTIVEUESTATUS_ENTRYTIME+1, "%d%s%s%s%s%s%3ld",
          (tm->tm_year+1900), (SINT8 *)month, date, hour, min, sec, milliSeconds );
}



