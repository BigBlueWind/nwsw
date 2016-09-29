/**
 * @file     imc_cmn_format.c
 * @brief  All common formats to be used functions will be added here.
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author  Shweta Polepally
 * @date    11 April 2011     Initial version.
 */

#include "imc_cmn_format.h"

/**
 *   @param  Date and time stamp to be filled.
 *   @details  This function will get date and time and put in the output string in the required format.
 * */
void getAndFillDateAndTimeStamp(
  IN UINT32   maxLenOfField,
  IO UINT8   *dataAndTimeStampField
  )
{
  struct timeval  tv;
  struct timezone tz;
  struct tm* tm;
  UINT32   milliSeconds = 0;

  UINT8 month[3], date[3], hour[3], min[3], sec[3], msec[4];
  clearBytes( &month, 3 );
  clearBytes( &date, 3 );
  clearBytes( &hour, 3 );
  clearBytes( &min, 3 );
  clearBytes( &sec, 3 );
  clearBytes( &msec, 4 );

  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);

  if ( (tm->tm_mon+1) < 10 )  snprintf( month, 3, "0%d", (tm->tm_mon+1) );
  else                        snprintf( month, 3, "%d",  (tm->tm_mon+1) );

  if ( tm->tm_mday < 10 )     snprintf( date, 3, "0%d", tm->tm_mday );
  else                        snprintf( date, 3, "%d",  tm->tm_mday );

  if ( tm->tm_hour < 10 )     snprintf( hour, 3, "0%d", tm->tm_hour );
  else                        snprintf( hour, 3, "%d",  tm->tm_hour );

  if ( tm->tm_min < 10 )      snprintf( min, 3, "0%d", tm->tm_min );
  else                        snprintf( min, 3, "%d",  tm->tm_min );

  if ( tm->tm_sec < 10 )      snprintf( sec, 3, "0%d", tm->tm_sec );
  else                        snprintf( sec, 3, "%d",  tm->tm_sec );

  /* Compute milliseconds from microseconds. */
  milliSeconds = tv.tv_usec / 1000;
  if ( milliSeconds < 10 )         snprintf( msec, 4, "00%d", milliSeconds );
  else if ( milliSeconds < 100 )   snprintf( msec, 4, "0%d",  milliSeconds );
  else                             snprintf( msec, 4, "%d",   milliSeconds );

  snprintf( dataAndTimeStampField, maxLenOfField+1 , "%d%s%s%s%s%s%s",
          (tm->tm_year+1900), month, date, hour, min, sec, msec );
}

void convertVersionInfoToVersionCfgContents(
  IN VersionInfo         *versionInfo,
  IO VersionCfgContents  *versionCfgContents
  )
{
  UINT8  version[MAX_SIZE_SWPACKAGESTATUS_VERSION + 1 ];
  char *word = NULL;
  char *delim = ".";
  UINT8 index = 0;
  UINT32 versionInfoArray[5];

  clearBytes( &versionInfoArray, 5 );
  clearBytes( &version, MAX_SIZE_SWPACKAGESTATUS_VERSION+1 );

  snprintf( version, strlen(versionInfo->version)+1, "%s", versionInfo->version );

  word = strtok( (char*)version, delim );
  versionInfoArray[index] = atoi(word);
  index++;
  while ( word NOTEQ NULL )
  {
    word = strtok( NULL, delim );
    if( word NOTEQ NULL )
    {
      versionInfoArray[index] = atoi(word);
      index++;
    }
  }
  versionCfgContents->releaseVersion = versionInfoArray[0];
  versionCfgContents->featureVersion = versionInfoArray[1];
  versionCfgContents->bugFixVersion  = versionInfoArray[2];
  versionCfgContents->customerId     = versionInfoArray[3];
  versionCfgContents->svnVersion     = versionInfoArray[4];

  /*Copy Others Fields Too */
  versionCfgContents->swType = versionInfo->swType;
  sprintf( versionCfgContents->builtOn, "%s", versionInfo->builtOn );
}

void convertVersionCfgContentsToVersionInfo(
  IN VersionCfgContents  *versionCfgContents,
  IO VersionInfo         *versionInfo
  )
{
  sprintf( versionInfo->version, "%d.%d.%d.%d.%d", versionCfgContents->releaseVersion, versionCfgContents->featureVersion,
      versionCfgContents->bugFixVersion, versionCfgContents->customerId, versionCfgContents->svnVersion );

  versionInfo->swType = versionCfgContents->swType;
  sprintf( versionInfo->builtOn, "%s", versionCfgContents->builtOn );
}
