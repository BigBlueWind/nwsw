/**
 * @file    bsm_set_opr_cfg_table.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief   This file will handle all the set messages on Operator Configuration table.
 *
 * @author:  Shweta Polepally
 * @date:    Feb 2011
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   setObj  Set Object Request sent by CIH/WIH/SIH
 * @details This function returns the value recd for EnbRegAccepted field.
 */
TBOOL getEnbRegAccepted(
  IN SetObjReq      *setObj
  )
{
  UINT8  i=0;
  for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
  {
    if( COL_ENODEBCONFIG_ENODEBREGISTRATIONACCEPTED EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      return getUint8FromArray(setObj->tlvArray.tlvInfo[i].data);
    }
  }
  return FALSE;
}

/**
 * @param   setObj  Set Object Request sent by CIH/WIH/SIH
 * @details This function will check if EnodeB Registration field is set to TRUE in the received SET OBJ command.
 */
TBOOL isSetOnEnbRegAcceptedRcvd(
  IN SetObjReq      *setObj
  )
{
  UINT8  i=0;
  for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
  {
    if( COL_ENODEBCONFIG_ENODEBREGISTRATIONACCEPTED EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * @param currConfig     current EnbRegistrationConfig
 * @param oldConfig      old EnbRegistrationConfig
 * @details This function checks is any any of three parameter(location) is updated or not.
 */
static TBOOL isLocationUpdated(
  IN EnbRegistrationConfig       *currConfig,
  IN EnbRegistrationConfig       *oldConfig
  )
{
  if ((currConfig->timeAndLocationSourceToUse NOTEQ oldConfig->timeAndLocationSourceToUse) ||
      (0 NOTEQ strcmp(currConfig->configuredLatitude, oldConfig->configuredLatitude)) ||
      (0 NOTEQ strcmp(currConfig->configuredLongitude, oldConfig->configuredLongitude)))
  {
    return TRUE;
  }
  return FALSE;
}
/**
 * @param   prevIpAddr  Previous IP Address.
 * @param   setObj      Set Object Request sent by CIH/WIH/SIH
 * @details This function will check if Enodeb IP address in the received SET Object command and already
 *          present is same or not.
 */
static TBOOL isEnodeIpAddrChanged(
  IN char       *prevIpAddr,
  IO SetObjReq  *setObj
  )
{
  UINT8  i=0;
  UINT8  newIpAddr[MAX_SIZE_ENODEBCONFIG_ENODEBSTATICIPADDR + 1 ];
  clearBytes( newIpAddr, MAX_SIZE_ENODEBCONFIG_ENODEBSTATICIPADDR+1 );

  for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
  {
    if( COL_ENODEBCONFIG_ENODEBSTATICIPADDR EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      getBytesFromArray( setObj->tlvArray.tlvInfo[i].data , setObj->tlvArray.tlvInfo[i].len, newIpAddr );
      if( 0 EQ strncmp(newIpAddr, prevIpAddr, strlen(prevIpAddr)) )
      {
        return FALSE;
      }
      else
      {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * @param   prevEmsIpAddr   Previous EMS IP Address.
 * @param   prevSnmpString  Previous Snmp Community String
 * @param   setObj          Set Object Request sent by CIH/WIH/SIH
 * @details This function will check if Enodeb IP address in the received SET Object command and already
 *          present is same or not.
 */
static TBOOL isEmsIpAddrOrSnmpStringChanged(
  IN char       *prevEmsIpAddr,
  IN char       *prevSnmpString,
  IO SetObjReq  *setObj
  )
{
  TBOOL emsIpAddrChanged = FALSE;
  TBOOL snmpStringChanged = FALSE;
  UINT8  i=0;
  UINT8  newEmsIpAddr[MAX_SIZE_ENBREGISTRATIONCONFIG_EMSADDR + 1 ];
  UINT8  newSnmpString[MAX_SIZE_ENBREGISTRATIONCONFIG_SNMPCOMMUNITYSTRING + 1 ];
  clearBytes( newEmsIpAddr, MAX_SIZE_ENBREGISTRATIONCONFIG_EMSADDR + 1 );
  clearBytes( newSnmpString, MAX_SIZE_ENBREGISTRATIONCONFIG_SNMPCOMMUNITYSTRING + 1 );

  for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
  {
    if( COL_ENBREGISTRATIONCONFIG_EMSADDR EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      getBytesFromArray( setObj->tlvArray.tlvInfo[i].data , setObj->tlvArray.tlvInfo[i].len, newEmsIpAddr );
      if( 0 NOTEQ strncmp(newEmsIpAddr, prevEmsIpAddr, strlen(prevEmsIpAddr)) )
      {
        emsIpAddrChanged = TRUE;
      }
    }
    if( COL_ENBREGISTRATIONCONFIG_SNMPCOMMUNITYSTRING EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      getBytesFromArray( setObj->tlvArray.tlvInfo[i].data , setObj->tlvArray.tlvInfo[i].len, newSnmpString );
      if( 0 NOTEQ strncmp(newSnmpString, prevSnmpString, strlen(prevSnmpString)) )
      {
        snmpStringChanged = TRUE;
      }
    }
  }
  if ( (TRUE EQ emsIpAddrChanged) || ( TRUE EQ snmpStringChanged ) )
    return TRUE;
  else
    return FALSE;
}

/**
 * @param   prevDateAndTimeSource   Previous DateAndTimeSource.
 * @param   setObj          Set Object Request sent by CIH/WIH/SIH
 * @details This function will check if DateAndTimeSource in the received SET Object command and already
 *          present is same or not.
 */
static TBOOL checkGpsDeviceAlarmForSetOnTimeDateSourceToGps(
  IN UINT32     *prevDateAndTimeSource,
  IO SetObjReq  *setObj
  )
{
  UINT8  i=0;
  TBOOL retVal = FALSE;
  UINT32 newDateAndTimeSource = 0;
  CreateAlarmDetails alarmDetails;
  clearBytes(&alarmDetails, sizeof(CreateAlarmDetails));

  /* Check for GPS related Alarm */
  if( TRUE NOTEQ getAlarmDetailsByBsErrorCode( BS_ERR_GPS_RECV_NOT_RESPONDING, &alarmDetails ) )
  {
    return FALSE;
  }
  strncat( (char*)alarmDetails.alarmLocation, "1",MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION );

  for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
  {
    if( COL_ENBREGISTRATIONCONFIG_TIMEANDLOCATIONSOURCETOUSE EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      newDateAndTimeSource= getUint32FromArray(setObj->tlvArray.tlvInfo[i].data);
      if( newDateAndTimeSource NOTEQ  *prevDateAndTimeSource )
      {
        retVal = checkIfActiveAlarmExists( &alarmDetails );
      }
      break;
    }
  }
  return retVal;
}


/**
 * @param   setObj          Set Object Request sent by CIH/WIH/SIH
 * @param   setSourceToUse  Source To Use OAM / GPS configured.
 * @param   setLatitude     Latitude which source to use - OAM / GPS configured.
 * @param   setLongitude    Longitude field to returned as TRUE on set on this field.
 * @param   setDateAndTime  Date And Time field to returned as TRUE on set on this field.
 * @param   setUtc          UTC field to returned as TRUE on set on this field.
 * @details This function will check set is received on which field - Source/Latitude/Longitude/DateAndTime/UTC.
 */
static void setOnWhichDateAndLocationFieldsAreRcvd(
  IN SetObjReq    *setObj,
  IO TBOOL        *setSourceToUse,
  IO TBOOL        *setLatitude,
  IO TBOOL        *setLongitude,
  IO TBOOL        *setDateAndTime,
  IO TBOOL        *setUtc
  )
{
  UINT8  i=0;
  for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
  {
    if( COL_ENBREGISTRATIONCONFIG_TIMEANDLOCATIONSOURCETOUSE EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      *setSourceToUse = TRUE;
    }
    if( COL_ENBREGISTRATIONCONFIG_CONFIGUREDLATITUDE EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      *setLatitude = TRUE;
    }
    if( COL_ENBREGISTRATIONCONFIG_CONFIGUREDLONGITUDE EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      *setLongitude = TRUE;
    }
    if( COL_ENBREGISTRATIONCONFIG_INITIALLOCALTIME EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      *setDateAndTime = TRUE;
    }
    if( COL_ENBREGISTRATIONCONFIG_INITIALLOCALTIMEOFFSETFROMUTC EQ setObj->tlvArray.tlvInfo[i].tag.colId )
    {
      *setUtc = TRUE;
    }
  }
}

/**
 * @param   rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param   setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - ENBREGISTRATIONCONFIG
 */
TBOOL bsmPerformOperationOnEnbRegistrationConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  TBOOL                   sendConfigChangeIndToSih = FALSE;
  TBOOL                   valueToReturn = FALSE;
  TBOOL                   dateTimeUpdated = FALSE;
  GenericTable           *tableFunc = getTableFunctions(ENBREGISTRATIONCONFIG);
  EnbRegistrationConfig  *config = (EnbRegistrationConfig*)getRowOfTable(ENBREGISTRATIONCONFIG, rowIdToUpdate);
  BsmContext             *ctx = getBsmContext();
  EnbRegistrationConfig   oldEmsRegConfig;

  clearBytes(&oldEmsRegConfig, sizeof(EnbRegistrationConfig));
  copyBytes(config, sizeof(EnbRegistrationConfig), &oldEmsRegConfig);

  /* Check if date and source is SET to GPS and GPS is working */
  if( TRUE EQ checkGpsDeviceAlarmForSetOnTimeDateSourceToGps( &config->timeAndLocationSourceToUse, setObj ) )
  {
    BS_ERROR_GPS_NOT_CONNECTED("BSM");
    bsmFillAndSendSetObjFail( setObj, BS_ERR_GPS_RECV_NOT_RESPONDING, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }

  /* Check if emsIpAddr or snmpCommunityString was updated. If any one is updated, send SIH_INIT_CONFIG_REQ */
  if ( TRUE EQ isEmsIpAddrOrSnmpStringChanged( config->emsAddr, config->snmpCommunityString, setObj ) )
  {
    /* Send Init Config Request To SIH Module*/
    sendConfigChangeIndToSih = TRUE;
  }

  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    valueToReturn = TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "ENBREGISTRATIONCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    valueToReturn = FALSE;
  }

  /* If Enodeb is locked then only below changes - Date and time and location can be updated
   * else ignore and return */
  if (FALSE EQ isEnodeBLocked())
  {
    return valueToReturn;
  }

  if ( TRUE EQ sendConfigChangeIndToSih )
  {
    ctx->configTransId++;
    fillAndSendSihInitCfgReqMsg();
  }
  /* Check if date, time and location fields were updated */
  TBOOL setSourceToUse = FALSE;
  TBOOL setLatitude    = FALSE;
  TBOOL setLongitude   = FALSE;
  TBOOL setDateAndTime = FALSE;
  TBOOL setUtc         = FALSE;

  setOnWhichDateAndLocationFieldsAreRcvd( setObj, &setSourceToUse, &setLatitude, &setLongitude, &setDateAndTime, &setUtc );

  if( TRUE EQ isLocationUpdated(config, &oldEmsRegConfig))
  {
    sendEventEnbLocationChanged();
  }

  /* Check 1 */
  if ( TRUE EQ setSourceToUse )
  {
    if ( oldEmsRegConfig.timeAndLocationSourceToUse NOTEQ config->timeAndLocationSourceToUse )
    {
      /* Case1 : Both GPS  : Send GET_GPS to DEH. Upon response, set update the values */
      if ( TIMEANDLOCATIONSOURCETOUSE_GPS EQ config->timeAndLocationSourceToUse )
      {
        /* send Request to DEH to send current GPS details */
        sendGpsDetailReqMsg();
        /* Update the values on receiving the response from DEH */
      }
      /* Case2 : Both OPERATOR */
      else if ( TIMEANDLOCATIONSOURCETOUSE_OPERCONFIGURED EQ config->timeAndLocationSourceToUse )
      {
        dateTimeUpdated = TRUE;
        ctx->dateAndTimeSource = DATEANDTIMESOURCE_CONFIGURED;
        /* Update date and time field present in the enodeB config file */
        bsmSetDateAndTimeOfSystem( config->initialLocalTime );
        updateLocationStatusTable( LOCATIONSOURCE_CONFIGUREDBYOAM, config->configuredLatitude, config->configuredLongitude );
        /* Store time zone from config file - USed to send in status table */
        snprintf( &ctx->currTimeOffsetFromUTC[0], MAX_SIZE_ENBSTATUS_LOCALTIMEOFFSETFROMUTC+1, "%s", config->initialLocalTimeOffsetFromUTC );
      }
    }
    else
    {
       /* Ignore - Dont do anything */
    }
  }
  /* Check 2 */
  if ( (FALSE EQ dateTimeUpdated) && (TIMEANDLOCATIONSOURCETOUSE_OPERCONFIGURED EQ config->timeAndLocationSourceToUse) )
  {
    if ( (TRUE EQ setDateAndTime)  || (TRUE EQ setUtc) )
    {
      if ( TIMEANDLOCATIONSOURCETOUSE_OPERCONFIGURED EQ config->timeAndLocationSourceToUse )
      {
        /* Update date and time field present in the enodeB config file */
        bsmSetDateAndTimeOfSystem( config->initialLocalTime );
      }
    }
    else if ( (TRUE EQ setLatitude) || (TRUE EQ setLongitude) )
    {
      if ( TIMEANDLOCATIONSOURCETOUSE_OPERCONFIGURED EQ config->timeAndLocationSourceToUse )
      {
        updateLocationStatusTable( LOCATIONSOURCE_CONFIGUREDBYOAM, config->configuredLatitude, config->configuredLongitude );
      }
    }
  }
  return valueToReturn;
}

/**
 * @param   setObj  Set Object Request sent by CIH/WIH/SIH
 * @details This function will check if parameter change is allowed or not.
 */
static TBOOL isParamChangeAllowedOnlyBySih(
  IO SetObjReq   *setObj
  )
{
  UINT8  i=0;
  for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
  {
    if( ( COL_ENODEBCONFIG_ENODEBREGISTRATIONACCEPTED EQ setObj->tlvArray.tlvInfo[i].tag.colId ) ||
         ( COL_ENODEBCONFIG_CONFIGCHANGEACKBYEMS EQ setObj->tlvArray.tlvInfo[i].tag.colId ) ||
	 ( COL_ENODEBCONFIG_ENODEBID EQ setObj->tlvArray.tlvInfo[i].tag.colId ) ||
         ( COL_ENODEBCONFIG_ENODEBNAME EQ setObj->tlvArray.tlvInfo[i].tag.colId )
      )
    {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * @param    rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param    setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - ENODEBCONFIG
 */
TBOOL bsmPerformOperationOnEnbConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(ENODEBCONFIG);
  EnodebConfig  *config    = (EnodebConfig*)getRowOfTable(ENODEBCONFIG, rowIdToUpdate);
  BsmContext    *ctx       = getBsmContext();

  /* Check if set is received on parameters allowed to be changed only by EMS(SIH), if yes, reject */
  if ( OAMIF_SIH NOTEQ setObj->cmdRef.origin )
  {
    if ( TRUE EQ isParamChangeAllowedOnlyBySih( setObj ) )
    {
      BS_ERROR_SET_NOT_ALLOWED_BY_NON_EMS_INTERFACE();
      logInfo("%s", "Set on EnodeConfig tables, enodebRegistrationAccepted & configChangeAckByEms not allowed from non-ems interfaces");
      bsmFillAndSendSetObjFail( setObj, BS_ERR_SET_NOT_ALLOWED_BY_NON_EMS_INTERFACE, &setObj->tlvArray.tlvInfo[0].tag );
      return FALSE;
    }
  }
  /* Check if request is received to change the enodeb IP address, if yes, reject it.
   * Enodeb IP address change is not permitted as of now.
   */
  if ( TRUE EQ isEnodeIpAddrChanged( config->enodebStaticIpAddr, setObj ) )
  {
    BS_ERROR_ENODEB_IP_ADDR_CHANGE_NOT_ALLOWED();
    bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_IP_ADDR_CHANGE_NOT_ALLOWED, NULL );
    return FALSE;
  }

  /* Check if this table was set when BSM is in BS_WAIT_REGISTRATION_STATE,
   * In that case, we need to check if enbRegistrationAccepted is TRUE or not */
  if ( getBsmProceduralState(ctx) EQ BS_WAIT_REGISTRATION_CMPL )
  {
      if(isSetOnEnbRegAcceptedRcvd( setObj ))
      {
        stopBsmProcedureTimer( TIMERNAME_WAITEMSREGRSP );
        ctx->emsRegSucc = getEnbRegAccepted ( setObj );
        if ( TRUE EQ ctx->emsRegSucc )
        {
          bsmCreateAndSendSetLedColor( LEDID_EMSREGISTRATION, LEDCOLOR_GREEN, ctx );

          /* Clear Alarm */
          clearEmsRegFailedAlarm();
          /* Registration procedure is completed
          * Check if any active alarm is present, if yes, post them to SIH
          * This is required, because when eNodeB was trying to connect to EMS and meanwhile
          * any alarm is generated, they are buffered but not posted to SIH.
          * After getting connected to EMS successfully post all of them to SIH */
          postActiveAlarmsToSihIfAny();
       }
      else
      {
        bsmCreateAndSendSetLedColor( LEDID_EMSREGISTRATION, LEDCOLOR_RED, ctx );
        logError("%s", "eNodeB registration rejected by EMS.");
        /*TODO: Add cause, why BS is DISABLED and unlock is NOT allowed */
      }
      setBsmProceduralState( ctx, BS_IDLE );
    }
  }

  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    /* BS-2324 ENB auto UNLOCK at boot up */
    if((TRUE  EQ isEnodeBLocked()) &&
       (TRUE  EQ ctx->emsRegSucc)  &&
       (FALSE EQ ctx->isAutoUnlockTried))
    {
      /* isAutoUnlockTried is set to FALSE on init */
      ctx->isAutoUnlockTried = TRUE;
      if((TRUE EQ isMmeIpAddressConfigured()) && (TRUE EQ isPlmnConfigured()))
      {
        bsmDoUnLock(NULL, ctx);
        logInfo("%s","BSM is performing the UNLOCK procedure as MME IP and PLMN are configured");

      }
    }
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "ENODEBCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, NULL );
    return FALSE;
  }
}

/**
 * @param   rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param   setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - CELLCONFIG
 */
TBOOL bsmPerformOperationOnCellConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(CELLCONFIG);
  CellConfig  *config = (CellConfig*)getRowOfTable(CELLCONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    TlvInfo  failedTlvInfo;
    clearBytes( &failedTlvInfo, sizeof(TlvInfo) );
    /* Remove this check after all MIMO features are supported */
    if( FALSE EQ bsmMimoFeatureCheckOnCellConfig( &setObj->tlvArray, &failedTlvInfo ) )
    {
      bsmFillAndSendSetObjFail( setObj, BS_ERR_CONFIGURATION_OR_CUSTOMIZATION_ERROR, &(failedTlvInfo.tag) );
      logError( "%s", "This MIMO Configuration is not supported.");
      return FALSE;
    }
    if( TRUE EQ bsmConsistencyCheckOnCellConfig( &setObj->tlvArray, &failedTlvInfo ) )
    {
      tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
      bsmFillAndSendSetObjSucc( setObj );
      return TRUE;
    }
    else
    {
      bsmFillAndSendSetObjFail( setObj, BS_ERR_CONSISTENCY_CHECK_FAILED, &(failedTlvInfo.tag) );
      BS_ERROR_CONSISTENCY_CHECK_FAILED( "CELLCONFIG", failedTlvInfo );
      return FALSE;
    }
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "CELLCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, NULL );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - SYSINFO1CONFIG
 */
TBOOL bsmPerformOperationOnSysInfo1Config(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(SYSINFO1CONFIG);
  SysInfo1Config  *config = (SysInfo1Config*)getRowOfTable(SYSINFO1CONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "SYSINFO1CONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - SYSINFO2CONFIG
 */
TBOOL bsmPerformOperationOnSysInfo2Config(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(SYSINFO2CONFIG);
  SysInfo2Config  *config = (SysInfo2Config*)getRowOfTable(SYSINFO2CONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    TlvInfo  failedTlvInfo;
    clearBytes( &failedTlvInfo, sizeof(TlvInfo) );
    if( TRUE EQ bsmConsistencyCheckOnSysInfo2Config( &setObj->tlvArray, &failedTlvInfo ) )
    {
      tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
      bsmFillAndSendSetObjSucc( setObj );
      return TRUE;
    }
    else
    {
      bsmFillAndSendSetObjFail( setObj, BS_ERR_CONSISTENCY_CHECK_FAILED, &(failedTlvInfo.tag) );
      BS_ERROR_CONSISTENCY_CHECK_FAILED( "SYSINFO2CONFIG", failedTlvInfo );
      return FALSE;
    }
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "SYSINFO2CONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, NULL );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - SYSINFO3CONFIG
 */
TBOOL bsmPerformOperationOnSysInfo3Config(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(SYSINFO3CONFIG);
  SysInfo3Config  *config = (SysInfo3Config*)getRowOfTable(SYSINFO3CONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "SYSINFO3CONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - SCHEDULERCONFIG
 */
TBOOL bsmPerformOperationOnSchedulerConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(SCHEDULERCONFIG);
  SchedulerConfig  *config = (SchedulerConfig*)getRowOfTable(SCHEDULERCONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "SCHEDULERCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - PHYCONFIG
 */
TBOOL bsmPerformOperationOnPhyConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(PHYCONFIG);
  PhyConfig  *config = (PhyConfig*)getRowOfTable(PHYCONFIG, rowIdToUpdate);
  BsErrorValue  bsError = BS_NO_ERROR;

  /* Calculate reference signal power */
  bsmCalculateRefSigPower(config);

   if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
   {
     TlvInfo  failedTlvInfo;
     clearBytes( &failedTlvInfo, sizeof(TlvInfo) );
     if( TRUE EQ bsmConsistencyCheckOnPhyConfig( &setObj->tlvArray, &failedTlvInfo, &bsError ) )
     {
       tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
       bsmFillAndSendSetObjSucc( setObj );
       return TRUE;
     }
     else
     {
       bsmFillAndSendSetObjFail( setObj, bsError, &(failedTlvInfo.tag) );
       BS_ERROR_CONSISTENCY_CHECK_FAILED( "PHYCONFIG", failedTlvInfo );
       return FALSE;
     }
   }
   else
   {
     BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "PHYCONFIG" );
     bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, NULL );
     return FALSE;
   }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - QOSCONFIG
 */
TBOOL bsmPerformOperationOnQosConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(QOSCONFIG);
  QosConfig  *config = (QosConfig*)getRowOfTable(QOSCONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "QOSCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - EUTRAHOPROFILECONFIG
 */
TBOOL bsmPerformOperationOnEutraHoProfileConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(EUTRAHOPROFILECONFIG);
  EutraHoProfileConfig  *config = (EutraHoProfileConfig*)getRowOfTable(EUTRAHOPROFILECONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "EUTRAHOPROFILECONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - EUTRANEIGHCELLCONFIG
 */
TBOOL bsmPerformOperationOnEutraNeighCellConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(EUTRANEIGHCELLCONFIG);
  EutraNeighCellConfig  *config = (EutraNeighCellConfig*)getRowOfTable(EUTRANEIGHCELLCONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
	TlvInfo      failedTlvInfo;
	UINT8        errInfoStr[MAX_TLV_VALUE_IN_BYTES];
	clearBytes( &failedTlvInfo, sizeof(TlvInfo) );
	clearBytes( &errInfoStr, sizeof(MAX_TLV_VALUE_IN_BYTES));

    if( TRUE EQ bsmConsistencyCheckOnEutraNeighCellConfig( &setObj->tlvArray, &failedTlvInfo, &errInfoStr[0] ) )
	{
      tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
      bsmFillAndSendSetObjSucc( setObj );
      return TRUE;
	}
	else
	{
	  bsmFillAndSendSetObjFail( setObj, BS_ERR_CONSISTENCY_CHECK_FAILED, &(failedTlvInfo.tag));
	  BS_ERROR_CONSISTENCY_CHECK_FAILED( "EUTRANEIGHCELLCONFIG", failedTlvInfo );
	  return FALSE;
	}
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "EUTRANEIGHCELLCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}


/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - PMCONFIG
 */
TBOOL bsmPerformOperationOnPmConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(PMCOUNTERLOGCONFIG);
  PmCounterLogConfig   *config = (PmCounterLogConfig*)getRowOfTable(PMCOUNTERLOGCONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    bsmFillAndSendPmhCfgReqMsg();
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "PMCOUNTERLOGCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - LOGCONFIG
 */
TBOOL bsmPerformOperationOnLogConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(LOGCONFIG);
  LogConfig     *config = (LogConfig*)getRowOfTable(LOGCONFIG, rowIdToUpdate);
  SetLogConfig   setLogCfg;

  clearBytes(&setLogCfg, sizeof(SetLogConfig));

  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    /* Sending the configurations received to the respective module */

    if (MODID_BSM EQ config->moduleId)
    {
      copyBytes(config, sizeof(LogConfig), &(getLogContext()->logConfig));
      logInfo("Loglevel is set to new value and LogMask = %d ", getLogContext()->logConfig.logMask1);
    }
    else if ((MODID_CIH1 EQ config->moduleId)||(MODID_CIH2 EQ config->moduleId)||(MODID_CIH3 EQ config->moduleId))
    {
      logInfo("%s", "New LogLevel will be reflected for CIH after CIH restart.");
    }
    else
    {
      copyBytes(config, sizeof(LogConfig), &setLogCfg);
      createAndSendMsg( getMsgId(config->moduleId), (UINT8*)&setLogCfg, sizeof(SetLogConfig));
    }
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "LOGCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - MMECONFIG
 */
TBOOL bsmPerformOperationOnMmeConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(MMECONFIG);
  MmeConfig     *config = (MmeConfig*)getRowOfTable(MMECONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    /* Update the index value of mme config table in the BSM context */
    BsmContext  *ctx = getBsmContext();
    ctx->maxMmesToConnectWith = config->index;
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "MMECONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - SONCONFIG
 */
TBOOL bsmPerformOperationOnSonConfig(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(SONCONFIG);
  SonConfig     *config = (SonConfig*)getRowOfTable(SONCONFIG, rowIdToUpdate);


  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "SONCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - SRSCONFIGDEDICATED
 */
TBOOL bsmPerformOperationOnSrsConfigDedicated(
  IN RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  GenericTable  *tableFunc = getTableFunctions(SRSCONFIGDEDICATED);
  SrsConfigDedicated     *config = (SrsConfigDedicated*)getRowOfTable(SRSCONFIGDEDICATED, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "SRSCONFIGDEDICATED" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @details This function will send UEEVENTHISTORYLOGCONFIG to UEH
 */
void bsmSendUeEventHistoryLogConfigToUeh(void)
{
  UeEventHistoryLogConfig  *config    = (UeEventHistoryLogConfig*)getFirstRowOfTable(UEEVENTHISTORYLOGCONFIG);
  MsgBsmUehUeEventHistoryLogConfigReq msgBsmUehUeEventHistoryLogConfigReq;

  clearBytes(&msgBsmUehUeEventHistoryLogConfigReq, sizeof(MsgBsmUehUeEventHistoryLogConfigReq));
  msgBsmUehUeEventHistoryLogConfigReq.ueEventHistoryLogConfig = *config;
  createAndSendMsg(MSG_BSM_UEH_UEEVENTHISTORYLOG_CONFIG_REQ, (UINT8 *)&msgBsmUehUeEventHistoryLogConfigReq, sizeof(MsgBsmUehUeEventHistoryLogConfigReq));
}

/**
 * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
 * @param setObj         Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on OAM Table - UEEVENTHISTORYLOGCONFIG
 */
TBOOL bsmPerformOperationOnUeEventHistoryLogConfig(
  IO RowId        rowIdToUpdate,
  IO SetObjReq   *setObj
  )
{
  UNUSED(rowIdToUpdate);  
  GenericTable                       *tableFunc = getTableFunctions(UEEVENTHISTORYLOGCONFIG);
  UeEventHistoryLogConfig *config    = (UeEventHistoryLogConfig*)getRowOfTable(UEEVENTHISTORYLOGCONFIG, rowIdToUpdate );
  MsgBsmUehUeEventHistoryLogConfigReq msgBsmUehUeEventHistoryLogConfigReq;

  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    clearBytes(&msgBsmUehUeEventHistoryLogConfigReq, sizeof(MsgBsmUehUeEventHistoryLogConfigReq));
    msgBsmUehUeEventHistoryLogConfigReq.ueEventHistoryLogConfig = *config;
    msgBsmUehUeEventHistoryLogConfigReq.cmdRef = setObj->cmdRef;
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, &msgBsmUehUeEventHistoryLogConfigReq.ueEventHistoryLogConfig );
    createAndSendMsg(MSG_BSM_UEH_UEEVENTHISTORYLOG_CONFIG_REQ, (UINT8 *)&msgBsmUehUeEventHistoryLogConfigReq, sizeof(MsgBsmUehUeEventHistoryLogConfigReq));
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "UEEVENTHISTORYLOGCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
   * @param rowIdToUpdate  Row to update in the OAM table. For Single Row, row Id = 1, For Multi Row, Row Id = n
   * @param setObj         Set Object Request sent by CIH/WIH/SIH
   * @details This function will perform operation on OAM Table - SECURITYCONFIG
   */
TBOOL bsmPerformOperationOnSecurityConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    )
{
  GenericTable  *tableFunc = getTableFunctions(SECURITYCONFIG);
  SecurityConfig     *config = (SecurityConfig*)getRowOfTable(SECURITYCONFIG, rowIdToUpdate);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, config );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "SECURITYCONFIG" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}
 
/**
 * @param msg        Message from UEH module
 * @param bsmContext BSM Context
 * @details This function will process MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC message in IDLE state.
 */
BsmProceduralState bsmProcUeEventHistoryLogConfigSuccFromUeh(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  GenericTable            *tableFunc = getTableFunctions(UEEVENTHISTORYLOGCONFIG);
  UeEventHistoryLogConfig *config    = (UeEventHistoryLogConfig*)getFirstRowOfTable(UEEVENTHISTORYLOGCONFIG);
  MsgUehBsmUeEventHistoryLogConfigSucc *rspMsg = (MsgUehBsmUeEventHistoryLogConfigSucc *)msg->data;
  SetObjReq setObj;

  if(rspMsg->cmdRef.origin)
  {
    /* Save the config */
    *config = rspMsg->ueEventHistoryLogConfig;

    clearBytes(&setObj, sizeof(SetObjReq));
    setObj.cmdRef = rspMsg->cmdRef;
    if ( NULL NOTEQ tableFunc->getStruct2TlvFunc )
    {
      setObj.tlvArray.tlvInfo[0].tag.tableId = UEEVENTHISTORYLOGCONFIG;
      tableFunc->getStruct2TlvFunc( &setObj.tlvArray.tlvInfo[0].tag, &setObj.tlvArray );
      bsmFillAndSendSetObjSucc( &setObj );
    }
    else
    {
      BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "UEEVENTHISTORYLOGCONFIG" );
      bsmFillAndSendSetObjFail( &setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj.tlvArray.tlvInfo[0].tag );
      return FALSE;
    }
  }

  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from UEH module
 * @param bsmContext BSM Context
 * @details This function will process MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL message in IDLE state.
 */
BsmProceduralState bsmProcUeEventHistoryLogConfigFailFromUeh(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  GenericTable            *tableFunc = getTableFunctions(UEEVENTHISTORYLOGCONFIG);
  UeEventHistoryLogConfig *config    = (UeEventHistoryLogConfig*)getFirstRowOfTable(UEEVENTHISTORYLOGCONFIG);
  UeEventHistoryLogConfig retainConfig;
  MsgUehBsmUeEventHistoryLogConfigFail *rspMsg = (MsgUehBsmUeEventHistoryLogConfigFail *)msg->data;
  SetObjReq setObj;

  clearBytes(&retainConfig, sizeof(UeEventHistoryLogConfig));
  retainConfig = *config;
  *config = rspMsg->ueEventHistoryLogConfig;

  if(rspMsg->cmdRef.origin)
  {
    clearBytes(&setObj, sizeof(SetObjReq));
    setObj.cmdRef = rspMsg->cmdRef;
    if ( NULL NOTEQ tableFunc->getStruct2TlvFunc )
    {
      setObj.tlvArray.tlvInfo[0].tag.tableId = UEEVENTHISTORYLOGCONFIG;
      tableFunc->getStruct2TlvFunc( &setObj.tlvArray.tlvInfo[0].tag, &setObj.tlvArray );
      bsmFillAndSendSetObjFail( &setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj.tlvArray.tlvInfo[0].tag  );
    }
    else
    {
      BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "UEEVENTHISTORYLOGCONFIG" );
      bsmFillAndSendSetObjFail( &setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj.tlvArray.tlvInfo[0].tag );
    }
  }

  *config = retainConfig;
  return bsmContext->bsmProceduralState;
}
