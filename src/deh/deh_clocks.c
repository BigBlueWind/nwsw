/**
 * @file    deh_clocks.c
 * @brief This file contains code to get details of AD9548 and CDCE62005 clock devices.
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author kgummad
 * @date   01-12-2011
 * @details 1) AD9548 and CDCE62005 clock devices support added in DEH.
 *
 */

#include "deh.h"
#include "lte_bb_misc_api.h"

/**
 * @details This function will return the error string if AD9548 clock is not working.
 */
UINT8* getdehAD9548State()
{
  Ad9548ReturnCode stability, state, source;
  stability = isSystemClockStable();
  state  = isFrequencyLocked();
  source = activeReferenceToAd9548();

  if( AD9548RETURNCODE_STATUS_STABLE EQ stability && AD9548RETURNCODE_STATUS_LOCKED EQ state
        && AD9548RETURNCODE_GPS_SIGNAL_SOURCE EQ source)
      return "GPS-Stable-Locked";
  if( AD9548RETURNCODE_STATUS_STABLE EQ stability && AD9548RETURNCODE_STATUS_LOCKED EQ state
        && AD9548RETURNCODE_CLK_SOURCE_2 EQ source)
      return "CLK2-Stable-Locked";
  if( AD9548RETURNCODE_STATUS_UNSTABLE EQ stability && AD9548RETURNCODE_STATUS_NOT_LOCKED EQ state
        && AD9548RETURNCODE_GPS_SIGNAL_SOURCE EQ source)
    return "GPS-Unstable-Unlocked";
  if( AD9548RETURNCODE_STATUS_UNSTABLE EQ stability && AD9548RETURNCODE_STATUS_LOCKED EQ state
        && AD9548RETURNCODE_GPS_SIGNAL_SOURCE EQ source)
    return "GPS-Unstable-Locked";
  if( AD9548RETURNCODE_STATUS_STABLE EQ stability && AD9548RETURNCODE_STATUS_NOT_LOCKED EQ state
        && AD9548RETURNCODE_GPS_SIGNAL_SOURCE EQ source)
    return "GPS-Stable-Unlocked";
  if( AD9548RETURNCODE_STATUS_UNSTABLE EQ stability && AD9548RETURNCODE_STATUS_NOT_LOCKED EQ state
        && AD9548RETURNCODE_CLK_SOURCE_2 EQ source)
    return "CLK2-Unstable-Unlocked";
  if( AD9548RETURNCODE_STATUS_UNSTABLE EQ stability && AD9548RETURNCODE_STATUS_LOCKED EQ state
        && AD9548RETURNCODE_CLK_SOURCE_2 EQ source)
    return "CLK2-Unstable-Locked";
  if( AD9548RETURNCODE_STATUS_STABLE EQ stability && AD9548RETURNCODE_STATUS_NOT_LOCKED EQ state
        && AD9548RETURNCODE_CLK_SOURCE_2 EQ source)
    return "CLK2-Stable-Unlocked";
  return "Device Malfunction";
}

/**
 * @details This function will check whether CDCE62005 Clock is working or not.
 */
TBOOL dehIsCdce62005ClockActive()
{
  CdceReturnCode status;
  status = isPllLocked();

  if( CDCERETURNCODE_STATUS_LOCKED EQ status )
  {
    logInfo("CDCE62005 Clock is Active. AD9548= %s", getdehAD9548State());
    return TRUE;
  }
  else if( CDCERETURNCODE_STATUS_NOT_LOCKED EQ status )
  {
    logError("CDCE62005 Clock is Inactive. AD9548= %s", getdehAD9548State());
    return FALSE;
  }
  logError("Unable read CDCE62005 Clock status. AD9548= %s", getdehAD9548State());
  return FALSE;
}

/**
 * @details This function will return the error string if CDCE62005 clock is not working.
 */
UINT8* dehCdce62005ErrorString()
{
  CdceReturnCode status;
  status = isPllLocked();
  if( status EQ CDCERETURNCODE_STATUS_NOT_LOCKED)
  {
    return getdehAD9548State();
  }
  return "Device Malfunction";
}
