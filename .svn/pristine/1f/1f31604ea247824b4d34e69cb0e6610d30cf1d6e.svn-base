/**
 * @file    deh_board.c
 * @brief This file is contains code to get details about Baseband Board
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author kgummad
 * @date   29-07-2011
 * @details 1)Ethernet supported added in DEH.
 *
 */

#include "deh.h"
#include "lte_bb_platform.h"

/**
 * @details This function is initialises DEH global context POST data. *
 */
void initDehPostCtx()
{
  UINT8 postresult[5];
  UINT32 index = 0;
  TBOOL allTestPass = TRUE;
  DehPostContext  *dehPostContext = getGlobalDehPostContext();
  clearBytes( dehPostContext, sizeof(DehPostContext));

  /* Initialize postresult array to all 1's */
  postresult[0] = 0xff;
  postresult[1] = 0xff;
  postresult[2] = 0xff;
  postresult[3] = 0xff;
  postresult[4] = 0xff;

  dehPostContext->isPostSuccess = FALSE;

  if( RWNORFLASHRETURNCODE_FAILURE EQ ReadPostResults( postresult ))
  {
	logError("%s", "Reading POST Results Failed.");
	for ( index = POST_SHORT_DDR; index <= POST_LONG_NANDFLASH; index++ )
	{
	  dehPostContext->postDeviceStatus[index + 1].isPerformed = FALSE;
	  dehPostContext->postDeviceStatus[index + 1].status = FALSE;
	}
	dehPostContext->isPostSuccess = FALSE;
	snprintf(dehPostContext->errorString, ERRORSTRING_LENGTH, "%s", "POST: Read Result Fail." );
	return;
  }

  if( postresult[0] EQ 0 )
  {
	logInfo("%s", "SHORT POST performed on Boot Up.");
	/* POST SHORT DDR */
	if ( postresult[1] EQ 1)
	{
	  dehPostContext->postDeviceStatus[POST_SHORT_DDR].isPerformed = TRUE;
	  dehPostContext->postDeviceStatus[POST_SHORT_DDR].status = TRUE;
	}
	else if ( postresult[1] EQ 0)
	{
	  dehPostContext->postDeviceStatus[POST_SHORT_DDR].isPerformed = TRUE;
	  dehPostContext->postDeviceStatus[POST_SHORT_DDR].status = FALSE;
	  allTestPass = FALSE;
	}
	else
	{
	  dehPostContext->postDeviceStatus[POST_SHORT_DDR].isPerformed = FALSE;
	  dehPostContext->postDeviceStatus[POST_SHORT_DDR].status = FALSE;
	}
	/* POST SHORT NAND FLASH */
	if ( postresult[3] EQ 1 )
	{
	  dehPostContext->postDeviceStatus[POST_SHORT_NANDFLASH].isPerformed = TRUE;
	  dehPostContext->postDeviceStatus[POST_SHORT_NANDFLASH].status = TRUE;
	}
	else if ( postresult[3] EQ 0 )
	{
	  dehPostContext->postDeviceStatus[POST_SHORT_NANDFLASH].isPerformed = TRUE;
	  dehPostContext->postDeviceStatus[POST_SHORT_NANDFLASH].status = FALSE;
	  allTestPass = FALSE;
	}
	else
	{
	  dehPostContext->postDeviceStatus[POST_SHORT_NANDFLASH].isPerformed = FALSE;
	  dehPostContext->postDeviceStatus[POST_SHORT_NANDFLASH].status = FALSE;
	}

  }
  else if( postresult[0] EQ 1 )
  {
	logInfo("%s", "LONG POST performed on Boot Up.");
	/* POST LONG DDR */
	if ( postresult[2] EQ 1 )
	{
	  dehPostContext->postDeviceStatus[POST_LONG_DDR].isPerformed = TRUE;
	  dehPostContext->postDeviceStatus[POST_LONG_DDR].status = TRUE;
	}
	else if ( postresult[2] EQ 0 )
	{
	  dehPostContext->postDeviceStatus[POST_LONG_DDR].isPerformed = TRUE;
	  dehPostContext->postDeviceStatus[POST_LONG_DDR].status = FALSE;
	  allTestPass = FALSE;
	}
	else
	{
	  dehPostContext->postDeviceStatus[POST_LONG_DDR].isPerformed = FALSE;
	  dehPostContext->postDeviceStatus[POST_LONG_DDR].status = FALSE;
	}
	/* POST SHORT FLASH */
	if ( postresult[4] EQ 1)
	{
	  dehPostContext->postDeviceStatus[POST_LONG_NANDFLASH].isPerformed = TRUE;
	  dehPostContext->postDeviceStatus[POST_LONG_NANDFLASH].status = TRUE;
	}
	else if ( postresult[4] EQ 0 )
	{
	  dehPostContext->postDeviceStatus[POST_LONG_NANDFLASH].isPerformed = TRUE;
	  dehPostContext->postDeviceStatus[POST_LONG_NANDFLASH].status = FALSE;
	  allTestPass = FALSE;
	}
	else
	{
	  dehPostContext->postDeviceStatus[POST_LONG_NANDFLASH].isPerformed = FALSE;
	  dehPostContext->postDeviceStatus[POST_LONG_NANDFLASH].status = FALSE;
	}
  }
  if ( ( allTestPass EQ FALSE ) )
  {
	dehPostContext->isPostSuccess = FALSE;
	if( postresult[0] EQ 0 )
	{
	  snprintf(dehPostContext->errorString, ERRORSTRING_LENGTH, "%s", "SHORT POST" );
	}
	else
	{
	  snprintf(dehPostContext->errorString, ERRORSTRING_LENGTH, "%s", "LONG POST" );
	}
	for ( index = POST_SHORT_DDR; index <= POST_LONG_NANDFLASH; index++ )
	{
	  if( dehPostContext->postDeviceStatus[index + 1].isPerformed EQ TRUE &&
	      dehPostContext->postDeviceStatus[index + 1].status EQ FALSE  )
	  {
		strncat(dehPostContext->errorString, "|", ERRORSTRING_LENGTH  );
		switch( index )
		{
		  case POST_SHORT_DDR: strncat(dehPostContext->errorString, " DDR-Fail ", ERRORSTRING_LENGTH  ); break;
		  case POST_LONG_DDR:  strncat(dehPostContext->errorString, " DDR-Fail ", ERRORSTRING_LENGTH  ); break;
		  case POST_SHORT_NANDFLASH: strncat(dehPostContext->errorString, " NAND-Fail ", ERRORSTRING_LENGTH  ); break;
		  case POST_LONG_NANDFLASH:  strncat(dehPostContext->errorString, " NAND-Fail ", ERRORSTRING_LENGTH  );break;
		}
	  }
	}
  }
  else
  {
	if( postresult[0] EQ 0 )
	{
	  logInfo("%s", "SHORT POST: PASS");
	}
	else
	{
	  logInfo("%s", "LONG POST: PASS");
	}
	dehPostContext->isPostSuccess = TRUE;
  }
}

/**
 * @details This function is the main code of execution of BIST thread.
 *          It updates the DEH global context BIST data after getting the data.
 */

void *bistThreadMain()
{
  DehBistContext  *dehBistContext = getGlobalDehBistContext();
  TBOOL status = FALSE;
  clearBytes(dehBistContext, sizeof(DehBistContext));


  pthread_detach(pthread_self());

  pthread_mutex_lock( &g_bistMutex );
  dehBistContext->isBistRunning = TRUE;
  pthread_mutex_unlock( &g_bistMutex );

  /* Get Backhaul Port Status */
  if( BISTRETURNCODE_SUCCESS EQ bistTestStart(  BISTTESTTYPE_BACKHAUL_ETHERNET_PORT, ETHERTESTYPE_ALL))
  {
	pthread_mutex_lock( &g_bistMutex );
	status = TRUE;
	dehBistContext->bistDeviceStatus[BIST_BACKHAUL_PORT].status = TRUE;
	pthread_mutex_unlock( &g_bistMutex );
  }
  else
  {
   	pthread_mutex_lock( &g_bistMutex );
   	status = FALSE;
	dehBistContext->bistDeviceStatus[BIST_BACKHAUL_PORT].status = FALSE;
	strncpy(dehBistContext->errorString, "BIST: BACKHAULPORT-Fail ", ERRORSTRING_LENGTH  );
	pthread_mutex_unlock( &g_bistMutex );
  }

  /* Get Debug Port Status */
  if( BISTRETURNCODE_SUCCESS EQ bistTestStart(  BISTTESTTYPE_DEBUG_ETHERNET_PORT, ETHERTESTYPE_ALL))
  {
	pthread_mutex_lock( &g_bistMutex );
	status = TRUE;
	dehBistContext->bistDeviceStatus[BIST_DEBUG_PORT].status = TRUE;
	pthread_mutex_unlock( &g_bistMutex );
  }
  else
  {
   	pthread_mutex_lock( &g_bistMutex );
   	status = FALSE;
	dehBistContext->bistDeviceStatus[BIST_DEBUG_PORT].status = FALSE;
	strncat(dehBistContext->errorString, "BIST: DEBUGPORT-Fail", ERRORSTRING_LENGTH  );
	pthread_mutex_unlock( &g_bistMutex );
  }
  if ( status EQ TRUE )
  {
    pthread_mutex_lock( &g_bistMutex );
    dehBistContext->isBistSuccess = TRUE;
    pthread_mutex_unlock( &g_bistMutex );
  }
  else
  {
    pthread_mutex_lock( &g_bistMutex );
    dehBistContext->isBistSuccess = FALSE;
    pthread_mutex_unlock( &g_bistMutex );
  }

  return NULL;
}

/**
 *  @details This function will Set Long POST on Baseband.
 */
TBOOL dehApplyLongPost()
{
  if( RWNORFLASHRETURNCODE_SUCCESS EQ ApplyLongPost( POST_DDR_FLASH ))
  {
	return TRUE;
  }
  return FALSE;
}

/**
 *  @details This function will check whether Baseband POST is success.
 */
TBOOL dehIsBasebandActive()
{
  TBOOL status = FALSE;
  DehPostContext  *dehPostContext = getGlobalDehPostContext();
  DehBistContext  *dehBistContext = getGlobalDehBistContext();
  pthread_mutex_lock( &g_bistMutex );
  if( dehPostContext->isPostSuccess EQ TRUE )
  {
    if( dehBistContext->isBistPerformed EQ TRUE )
    {
      if( dehBistContext->isBistSuccess EQ TRUE  )
      {
	    status = TRUE;
      }
      else
      {
    	status = FALSE;
      }
    }
    else
    {
      status = TRUE;
    }
    if( dehBistContext->isBistRunning EQ TRUE )
    {
      status = TRUE;
    }
  }
  else
  {
	status = FALSE;
  }
  pthread_mutex_unlock( &g_bistMutex );
  return status;
}

/**
 *  @details This function will return the result of baseband POST failure.
 */
UINT8* dehBasebandPostErrorString()
{
  DehPostContext  *dehPostContext = getGlobalDehPostContext();

  if( dehPostContext->isPostSuccess EQ FALSE )
  {
	return dehPostContext->errorString;
  }
  return "";
}

/**
 *  @details This function will return the result of baseband BISTT failure.
 */
UINT8* dehBasebandBistErrorString()
{
  DehBistContext  *dehBistContext = getGlobalDehBistContext();
  TBOOL status = TRUE;
  TBOOL isBistRunning = FALSE;

  pthread_mutex_lock( &g_bistMutex );
  if( dehBistContext->isBistRunning EQ TRUE)
  {
	  isBistRunning = TRUE;
  }
  else
  if( dehBistContext->isBistPerformed EQ TRUE )
  {
    if( dehBistContext->isBistSuccess EQ TRUE  )
    {
      status = TRUE;
    }
    else
    {
      status = FALSE;
    }
  }
  pthread_mutex_unlock( &g_bistMutex );
  if( isBistRunning EQ TRUE)
  {
	return "BIST In Progress";
  }
  if( ( dehBistContext->isBistPerformed EQ TRUE ) && ( dehBistContext->isBistSuccess EQ TRUE ) )
  {
    return "BIST: Success";
  }
  if( status EQ FALSE)
  {
    return dehBistContext->errorString;
  }
  return "";
}

/**
 *  @details This function will get the Hardware version of Baseband at init.
 */
void dehUpdateBasebandHardwareVersion(
  BasebandHwInfo *dehVersionInfo
  )
{
  InventoryInfo platformVersionInfo;
  clearBytes( &platformVersionInfo, sizeof(InventoryInfo));

  /* Here buf is used to write hardware information on the board. */
  /* IMP: This should be removed after AT team provides hardware information by default */
  InventoryInfo buf;
  clearBytes( &buf, sizeof(InventoryInfo));
  char modtype[9] = "LTEENODE";
  char factorycode[3] = "1_";
  char partnum[14] = "123SW@u834l5l";
  char hwrev[4] = "1.0";
  char modserNum[6] = "S12h2";
  char manufdate[5] = "2013";
  char lotcode[11] = "POWERWAVE@";
  buf.blockId = 0x10;
  buf.blockSize = 0x1234;
  buf.tableFormat = 0x12;
  memcpy(buf.modType, &modtype, 9);
  memcpy(buf.factoryCode, &factorycode, 3);
  memcpy(buf.partNum, &partnum, 14);
  memcpy(buf.hwRev, &hwrev, 4);
  memcpy(buf.modSerNum, &modserNum, 6);
  memcpy(buf.manufDate, &manufdate, 5);
  memcpy(buf.lotCode, &lotcode, 11);

  if ( RWNORFLASHRETURNCODE_SUCCESS EQ writeInventoryInfo( &buf ) )
  {
    logInfo("%s", "Write to flash Hardware details success");
  }
  else
  {
    logInfo("%s", "Write to flash Hardware details fail");
  }

  if ( RWNORFLASHRETURNCODE_SUCCESS EQ readInventoryInfo( &platformVersionInfo ) )
  {
    strncpy( dehVersionInfo->hwRev, platformVersionInfo.hwRev, EE_HW_REV_SIZE  ) ;
  }
  else
  {
    strncpy( dehVersionInfo->hwRev, "-", sizeof("-"));
  }
  return;
}

/**
 *  @details This function will get the Product ID of Baseband.
 */
TBOOL dehGetBasebandProductId(
  UINT8 *productId
  )
{
  InventoryInfo platformInfo;
  clearBytes( &platformInfo, sizeof(InventoryInfo));

  if ( RWNORFLASHRETURNCODE_SUCCESS EQ readInventoryInfo( &platformInfo ) )
  {
    snprintf( productId, MAX_SIZE_ENBADDRESSES_UNIQUEPRODUCTID, "%s%s%s%s%s%s%s", platformInfo.modType, platformInfo.factoryCode, platformInfo.partNum,  platformInfo.hwRev,  platformInfo.modSerNum,  platformInfo.manufDate, platformInfo.lotCode   ) ;
    return TRUE;
  }
  else
  {
    strncpy( productId, "-", sizeof("-"));
    return FALSE;
  }
}



