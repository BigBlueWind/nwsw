/**
 * @file    rh_dec.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains decoding functions for messages received from LK at BS. ( LK->BS )
 *
 * @author   mkata, spolepally, aagrawal, kgummad
 * @date     25-05-2011
 * @details  Basic version
 *
 */
#include "rh_dec.h"
#include "bsm_deh_if.h"
#include "basic_dec.h"
#include "deh.h"

/**
 * @details  This function checks what type was result was received in the
 *           RSP msg and logs the Result type.
 * @param    result Result received in the RSP
 */
void logResultTypeReset(
  IN SINT16   result
  )
{
  if ( result EQ 0 )
  {
    logInfo ( "%s", " Result : NORMAL RESPONSE ");
  }
  else if ( result EQ -2 )
  {
    logInfo ( "%s", " Result : 'MSG ID' field error in REQ ");
  }
  else if ( result EQ -201 )
  {
    logInfo ( "%s", " Result : Abnormal reason 1 ");
  }
  else if ( result EQ -202 )
  {
    logInfo ( "%s", " Result : Abnormal reason 2 ");
  }
  else
  {
    logError ("Result type = %d was not expected ", result);
  }
}

/**
 * @details  This function checks what type was result was received in the
 *           RSP msg and logs the Result type.
 * @param    result Result received in the RSP
 */
void logResultType1(
  IN SINT16   result
  )
{
  if ( result EQ 0 )
  {
    logInfo ( "%s", " Result : NORMAL RESPONSE ");
  }
  else if ( result EQ -2 )
  {
    logInfo ( "%s", " Result : 'MSG ID' field error in REQ ");
  }
  else if ( result EQ -3 )
  {
    logInfo ( "%s", " Result : 'VAR' field error in REQ ");
  }
  else if ( result EQ -101 )
  {
    logInfo ( "%s", " Result : Function used is restricted for a while ");
  }
  else if ( result EQ -201 )
  {
    logInfo ( "%s", " Result : Abnormal reason 1 ");
  }
  else if ( result EQ -202 )
  {
    logInfo ( "%s", " Result : Abnormal reason 2 ");
  }
  else 
  {
    logError ("Result type = %d was not expected ", result);
  }
}

/**
 * @details  This function checks what type was result was received in the
 *           RSP msg and logs the Result type.
 * @param    result : Result received in the RSP
 */
void logResultTypeAlarmChangeNoti(
  IN SINT16   result
  )
{
  if ( result EQ 0 )
  {
    logInfo ( "%s", " Result : NORMAL RESPONSE ");
  }
  else if ( result EQ -101 )
  {
    logInfo ( "%s", " Result : Function used is restricted for a while ");
  }
  else if ( result EQ -201 )
  {
    logInfo ( "%s", " Result : Abnormal reason 1 ");
  }
  else if ( result EQ -202 )
  {
    logInfo ( "%s", " Result : Abnormal reason 2 ");
  }
  else
  {
    logError ("Result type = %d was not expected ", result);
  }
}

/**
 * @details  This function fills RhDehHwInfoRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsHwInfoRsp : RSP structure.
 */
TBOOL decRhDehHwInfoRsp(
  IO DecBuf          *decBuf,
  IO LkHwInfoTable   *rsp
  )
{
  SINT16 varResult = 0;
  SINT16 index = 0;
  SINT16 count = 0;
  UINT16 dummyUINT16;
  UINT32 versionInfo = 0;
  UINT8  *versionInfoPtr = NULL;
  UINT8  description[MAX_HWINFO_DESC+1];

  varResult         = getUint16( decBuf );   /* 4-5 */
  /* Check the result */
  if( varResult NOTEQ 0 )
  {
    logError( "Expected=0, received=%d", varResult );
    reportError(" %s, %d : RhDehHwInfoReqFailed,Expected=0, received=%d ",__FILE__, __LINE__,varResult);
    return FALSE;
  }

  dummyUINT16  = getUint16( decBuf );

  /*Hardware version V1.2.3.40 indicates 0x01020328.*/
  versionInfo     = getUint32( decBuf );
  versionInfoPtr  = (UINT8 *) &versionInfo;

  /*Updating Hardware version in DEH Global Context*/
  snprintf(rsp->version, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1 , "%d.%d.%d.%d", *(versionInfoPtr+3), *(versionInfoPtr+2), *(versionInfoPtr+1), *versionInfoPtr );

  /*Product Description*/
  copyBytes( (decBuf)->value + (decBuf)->index,  MAX_HWINFO_DESC, description);


  /*Contents of description. Identifier = SEMICOLON, Format:= {CoName};{CoIni};{PrDesc};{PrIni};{SN};{CLEI};{MN}*/
  /*Count variable to keep track of SEMICOLON*/
  for (index = 0, count = 0; index < MAX_HWINFO_DESC ; index++  )
  {
	if( description[index] EQ ';')
	{
	  count++;
	}
	if( count EQ 2 )
	{
	  index++;
	  break;
	}
  }
  /*Updating Hardware description in DEH Global Context*/
  sscanf(description + index ,"%[^;]", rsp->description );

  /* Log the RSP message received for testing purpose */
  logInfo( "%s", " Msg RhDehHwInfoRsp Received with following parameters :" );
  logResultType1( varResult );
  logInfo( " HW Version :v%s, Description : %s ", rsp->version, rsp->description );
 return TRUE;
}

/**
 * @details  This function fills RhDehFwInfoRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsFwInfoRsp : RSP structure.
 */
TBOOL decRhDehFwInfoRsp(
  IO DecBuf          *decBuf,
  IO LkBsFwInfoRsp   *rspMsg
  )
{
  UINT8  *runningVersionInfoPtr = NULL;
  UINT8  *nonRunningVersionInfoPtr = NULL;

  DehContext *dehContext = getDehContext();

  rspMsg->varResult                = getUint16( decBuf );
  rspMsg->activeSide               = getUint16( decBuf ); /*Active side of RH*/
  rspMsg->runningVersion           = getUint32( decBuf ); /*Running version of RH */
  rspMsg->nonRunningVersion        = getUint32( decBuf ); /*Non running version of RH*/
  rspMsg->runningVersionBuildDate  = getUint32( decBuf ); /*Build date of running version in UTC*/
  getBytes(decBuf,16,rspMsg->vendorName);        /*Vendor name*/
  rspMsg->nonRunningVersionBuildDate = getUint32( decBuf );/*Build date of non-running version in UTC*/


  /* Check the result */
  if( rspMsg->varResult NOTEQ 0 )
  {
    logError( "Expected=0, received=%d", rspMsg->varResult );
    reportError(" %s, %d : RhDehFwInfoReqFailed,Expected=0, received=%d ",__FILE__, __LINE__,rspMsg->varResult);
    return FALSE;
  }

  /*Updating Active side in DEH Global Context*/
  dehContext->rhPackageStatus.runningSide  =   rspMsg->activeSide;
  if(RH_SIDE_A EQ rspMsg->activeSide)
  {
    dehContext->rhPackageStatus.nonRunningSide = RH_SIDE_B;
  }
  else
  {
    dehContext->rhPackageStatus.nonRunningSide = RH_SIDE_A;
  }

  /*Firmware version V1.2.3.40 indicates 0x01020328.*/
  runningVersionInfoPtr  = (UINT8 *) &(rspMsg->runningVersion);//runnningVersionInfo;

  /*Updating Firmware Running version in DEH Global Context*/
  snprintf(dehContext->rhPackageStatus.runningVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION+1, "0.%d.%d.%d.%d",
           *(runningVersionInfoPtr+3), *(runningVersionInfoPtr+2), *(runningVersionInfoPtr+1), *runningVersionInfoPtr );

  /*Firmware version V1.2.3.40 indicates 0x01020328.*/
  nonRunningVersionInfoPtr  = (UINT8 *) &(rspMsg->nonRunningVersion);//nonRunningVersionInfo;

  /*Updating Firmware NonRunning version version in DEH Global Context*/
  snprintf(dehContext->rhPackageStatus.nonRunningVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION+1, "0.%d.%d.%d.%d",
           *(nonRunningVersionInfoPtr+3), *(nonRunningVersionInfoPtr+2), *(nonRunningVersionInfoPtr+1), *nonRunningVersionInfoPtr );

  /*Updating build date in DEH Global Context*/
  //strftime(rsp->date, MAX_FWBUILDINFO_DESC, "%d/%m/%Y", (struct tm *)gmtime((time_t *) &buildDate));
  strftime(dehContext->rhPackageStatus.runningVersionBuildDate, MAX_FWBUILDINFO_DESC, "%Y%m%d%H%M%S000", (struct tm *)gmtime((time_t *) &(rspMsg->runningVersionBuildDate)));
  strftime(dehContext->rhPackageStatus.nonRunningVersionBuildDate, MAX_FWBUILDINFO_DESC, "%Y%m%d%H%M%S000", (struct tm *)gmtime((time_t *) &(rspMsg->nonRunningVersionBuildDate)));

  /* Log the RSP message received for testing purpose */
  logInfo( "%s", " Msg RhDehFwInfoRsp Received with following parameters :\n" );
  logResultType1( rspMsg->varResult );
  logInfo( "\n  RH active Side = %u\n  Running Version :v%s\n  Non running version: v%s\n  runningVersionBuildDate : %s\n nonRunningVersionBuildDate : %s\n",
      dehContext->rhPackageStatus.runningSide,dehContext->rhPackageStatus.runningVersion,dehContext->rhPackageStatus.nonRunningVersion, dehContext->rhPackageStatus.runningVersionBuildDate,dehContext->rhPackageStatus.nonRunningVersionBuildDate );
 return TRUE;

}

/**
 * @details  This function fills RhDehHistoryInfoReadRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsHistoryInfoReadRsp : RSP structure.
 */

TBOOL decRhDehHistoryInfoReadRsp(
  IO DecBuf          *decBuf,
  IO LkHistoryTable  *rsp
 )
{
  SINT16 varResult = 0;
  UINT16 dummyUINT16;

  varResult         = getUint16( decBuf );   /* 4-5 */
  /* Check the result */
  if( varResult NOTEQ 0 )
  {
    logError( "Expected=0, received=%d", varResult );
    reportError(" %s, %d : RhDehFwInfoReqFailed,Expected=0, received=%d ",__FILE__, __LINE__,varResult);
    return FALSE;
  }
  dummyUINT16  = getUint16( decBuf );

  rsp->ManufactureDate = getUint32( decBuf );
  rsp->InstallDate     = getUint32( decBuf );
  rsp->RepairCount     = getUint32( decBuf );

  rsp->RepairDate      = getUint32( decBuf );
  rsp->RepairCode      = getUint32( decBuf );


  /**
   * @todo Needs  to Handle All 7 Repair History Information -- Needs to update OAM Table
   *
   */


  logInfo( "%s", " Msg LkBsHistoryInfoReadRsp Received with following parameters :" );
  logResultType1( varResult );
  logInfo( "Manufacture Date : %u \n, Install Date : %u \n, Repair count : %u \n",
      rsp->ManufactureDate,
      rsp->InstallDate, rsp->RepairCount );
  /* Recent History Info */

  logInfo( "Repair Date : %u\n, Repair Code : %u\n ", rsp->RepairDate, rsp->RepairCode);
  return TRUE;
}

/**
 * @details  This function fills RhDehHistoryInfoWriteRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsHistoryInfoWriteRsp : RSP structure.
 */
TBOOL decRhDehHistoryInfoWriteRsp(
  IO DecBuf     *decBuf
  )
{
  SINT16 varResult = 0;

  varResult         = getUint16( decBuf );   /* 4-5 */
  /* Check the result */
  if( varResult NOTEQ 0 )
  {
    logError( "Expected=0, received=%d", varResult );
    reportError(" %s, %d : RhDehFwInfoReqFailed,Expected=0, received=%d ",__FILE__, __LINE__,varResult);
    return FALSE;
  }

  logInfo( "%s", " Msg LkBsHistoryInfoWriteRsp Received with following parameters :" );
  logResultType1( varResult );
  return TRUE;
}
TBOOL isCpriUp(
  UINT16 info,
  UINT16 bitMacro
  )
{
  if(info & bitMacro)
  {
  return TRUE;
  }
  return FALSE;
}
/**
 * @details  This function fills RhDehOperationalModeInfoRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsOperationalModeInfoRsp : RSP structure.
 */
void decRhDehOperationalModeInfoRsp(
  IO DecBuf                      *decBuf,
  IO LkBsOperationalModeInfoRsp  *rsp
  )
{

  rsp->varResult = getUint16( decBuf );
  rsp->varMode   = getUint16( decBuf );

#ifdef PWAV_L2SIM
  rsp->varMode = rsp->varMode | BIT_CPRI;
#endif

  if(isCpriUp(rsp->varMode,BIT_CPRI))
  {
    setConnected(DEV_CPRI);
  }
  else
  {
    setNotConnected(DEV_CPRI);
  }

  logInfo( "%s", " Msg Operational Mode Information RSP Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo( " Mode :%d, (0:Normal Mode , 1:Remote Fusing Mode) ",rsp->varMode );
}

/**
 * @details  This function fills RhDehRemoteFusingRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsRemoteFusingRsp : RSP structure.
 */
void decRhDehRemoteFusingRsp(
  IO DecBuf               *decBuf,
  IO LkBsRemoteFusingRsp  *rsp
  )
{
  logInfo( "%s", " Msg LkBsRemoteFusingRsp Received with following parameters :" );
  rsp->varResult = getUint16( decBuf );
  rsp->var2      = getUint16( decBuf );
}

/**
 * @details  This function fills RhDehRemoteFusingCompleteNoti message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsRemoteFusingCompleteNoti : RSP structure.
 */
void decRhDehRemoteFusingCompleteNoti(
  IO DecBuf                        *decBuf,
  IO LkBsRemoteFusingCompleteNoti  *noti
  )
{
  UINT8  *nonRunningVersionInfoPtr = NULL;
  DehContext *dehContext = getDehContext();

  noti->varResult  = getUint16( decBuf );
  noti->varAction  = getUint16( decBuf );
  noti->fwVersion  = getUint32( decBuf );

  nonRunningVersionInfoPtr  = (UINT8 *) &(noti->fwVersion);
  /*Updating Firmware Running version in DEH Global Context*/
   snprintf(dehContext->rhPackageStatus.nonRunningVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION+1, "0.%d.%d.%d.%d",
            *(nonRunningVersionInfoPtr+3), *(nonRunningVersionInfoPtr+2), *(nonRunningVersionInfoPtr+1), *nonRunningVersionInfoPtr );

  logInfo(" Msg LkBsRemoteFusingCompleteNoti Received with following parameters {varResult=%d|varAction=%u|fwVer=%u}",
      noti->varResult,noti->varAction,noti->fwVersion);
}

/**
 * @details  This function fills RhDehResetRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsResetRsp : RSP structure.
 */
void decRhDehResetRsp(
  IO DecBuf        *decBuf,
  IO LkBsResetRsp  *rsp
  )
{
  rsp->varResult  = getUint16( decBuf );
  rsp->varDummy   = getUint16( decBuf );

  logInfo( "%s", " Msg LkBsResetRsp Received with following parameters :" );
  logResultTypeReset( rsp->varResult );
}

/**
 * @details  This function fills LkBsInitializationReqNoti message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsInitializationReqNoti : RSP structure.
 */
void decRhDehInitializationReqNoti(
  IO DecBuf                     *decBuf,
  IO LkBsInitializationReqNoti  *noti
  )
{
  UINT8  *runningVersionInfoPtr = NULL;
  DehContext *dehContext = getDehContext();

  noti->varFwMode  = getUint8( decBuf );
  noti->varLogAvailable = getUint8( decBuf );
  noti->varDummy = getUint16( decBuf );
  noti->dataDummy1 = getUint16( decBuf );
  noti->dataDummy2 = getUint16( decBuf );
  noti->fwVersion = getUint32( decBuf );
  logInfo("%s", " Initialization Noti received  " );

  runningVersionInfoPtr  = (UINT8 *) &(noti->fwVersion);
  /*Updating Firmware Running version in DEH Global Context*/
   snprintf(dehContext->rhPackageStatus.runningVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION+1, "0.%d.%d.%d.%d",
            *(runningVersionInfoPtr+3), *(runningVersionInfoPtr+2), *(runningVersionInfoPtr+1), *runningVersionInfoPtr );

}

/**
 * @details  This function fills RhDehAlarmInfoRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsAlarmInfoRsp : RSP structure.
 */

TBOOL decRhDehAlarmInfoRsp(
  IO DecBuf            *decBuf,
  IO LkBsAlarmInfoRsp  *rsp
 )
{
  SINT16 varResult = 0;
  UINT32 varSeqNum = 0;


  varResult         = getUint16( decBuf );   /* 4-5 */
  /* Check the result */
  if( varResult NOTEQ 0 )
  {
    logError( "Expected=0, received=%d", varResult );
    reportError(" %s, %d : decRhDehAlarmInfoRsp,Expected=0, received=%d ",__FILE__, __LINE__,varResult);
    return FALSE;
  }
  varSeqNum    = getUint16( decBuf );
  rsp->dataValidInd = getUint32( decBuf );
  /**
   *  @todo Alarms for Both paths will be added after finalising Alarms in LTE solution
   */
  rsp->alarmDataPathA = getUint32( decBuf );
  rsp->alarmDataPathB = getUint32( decBuf );


  logInfo( "%s", " Msg LkBsAlarmInfoRsp Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo( " SequenceNo :%d\n, Valid Data Indicator:%u\n, Alarm Data Path A : %x\n, Alarm Data Path A : %x \n",
		        rsp->varSeqNum, rsp->dataValidInd, rsp->alarmDataPathA, rsp->alarmDataPathB );

  return TRUE;

 }

/**
 * @details : This function fills RhDehAlarmChangeNoti message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsAlarmChangeNoti : RSP structure.
 */
TBOOL decRhDehAlarmChangeNoti(
  IO DecBuf               *decBuf,
  IO LkBsAlarmChangeNoti  *noti
  )
{
  noti->varResult    = getUint16( decBuf );
  /* Check the result */
  if( noti->varResult  NOTEQ 0 )
  {
    logError( "Expected=0, received=%d", noti->varResult );
    reportError(" %s, %d : decRhDehAlarmChangeNoti,Expected=0, received=%d ",__FILE__, __LINE__,noti->varResult );
    return FALSE;
  }
  noti->varSeqNum       = getUint16( decBuf );
  noti->dataValidInd    = getUint32( decBuf );
  noti->dataDummy1      = getUint32( decBuf );
  noti->dataDummy2      = getUint32( decBuf );
  noti->alarmDataCommon = getUint32( decBuf );
  noti->alarmDataPathA  = getUint32( decBuf );
  noti->alarmDataPathB  = getUint32( decBuf );
  logInfo( "%s", " Msg LkBsAlarmChangeNoti Received with following parameters :" );
  logResultTypeAlarmChangeNoti( noti->varResult );

  logInfo( " \n { \n SequenceNo :%d\n, Alarm Data common  : %x Alarm Data Path A : %x\n, Alarm Data Path B : %x \n}",
		  noti->varSeqNum,  noti->alarmDataCommon, noti->alarmDataPathA, noti->alarmDataPathB );
  return TRUE;
}

/**
 * @details  This function fills LkStatusTable message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsStatusInfoRsp : RSP structure.
 */
TBOOL decRhDehStatusInfoRsp(
  IO DecBuf             *decBuf,
  IO RadioHeadStatus    *rspMsg,
  IO UINT16             *varSeqNum
  )
{

  SINT16 varResult = 0;
  UINT8  dummyUINT8;
  UINT16 dummyUINT16;
  UINT32 dummyUINT32;
  UINT8  iqMap = 0;
  UINT8  faMapping = 0;
  UINT8  chBandwidth = 0;
  UINT16 multiDummy = 0;
  DehContext *dehContext = NULL;
  dehContext = getDehContext();
  varResult         = getUint16( decBuf );   /* 4-5 */
  /* Check the result */
  if( varResult NOTEQ 0 )
  {
    logError( "Expected=0, received=%d", varResult );
    reportError(" %s, %d : RhStatusReqFailed,Expected=0, received=%d ",__FILE__, __LINE__,varResult);
    return FALSE;
  }
  /**
   * @todo need to work on sequence number
   */
  *varSeqNum              = getUint16( decBuf );     /* 6-7 */
  //logError( "varSeqNum Expected=1, varSeqNum received=%d", *varSeqNum );

  rspMsg->radioHeadProfileId = 1;

  iqMap                  = getUint8( decBuf );      /* 8  */
  if(iqMap EQ 0)
  {
    rspMsg->c1IQMap =  C1IQMAP_0;
  }
  else if(iqMap EQ 1)
  {
    rspMsg->c1IQMap =  C1IQMAP_1;
  }
  else if(iqMap EQ 3)
  {
    rspMsg->c1IQMap =  C1IQMAP_3;
  }
  else if(iqMap EQ 5)
  {
    rspMsg->c1IQMap =  C1IQMAP_5;
  }
  else if(iqMap EQ 10)
  {
    rspMsg->c1IQMap =  C1IQMAP_10;
  }
  else if(iqMap EQ 15)
  {
    rspMsg->c1IQMap =  C1IQMAP_15;
  }
  else
  {
    rspMsg->c1IQMap =  C1IQMAP_20;
  }

  iqMap                  = getUint8( decBuf );      /* 8  */
  if(iqMap EQ 0)
  {
	rspMsg->c2IQMap =  C2IQMAP_0;
  }
  else if(iqMap EQ 1)
  {
    rspMsg->c2IQMap =  C2IQMAP_1;
  }
  else if(iqMap EQ 3)
  {
	rspMsg->c2IQMap =  C2IQMAP_3;
  }
  else if(iqMap EQ 5)
  {
	rspMsg->c2IQMap =  C2IQMAP_5;
  }
  else if(iqMap EQ 10)
  {
	rspMsg->c2IQMap =  C2IQMAP_10;
  }
  else if(iqMap EQ 15)
  {
	rspMsg->c2IQMap =  C2IQMAP_15;
  }
  else
  {
	rspMsg->c2IQMap =  C2IQMAP_20;
  }

  iqMap                  = getUint8( decBuf );
  if(iqMap EQ 0)
  {
    rspMsg->c3IQMap =  C3IQMAP_0;
  }
  else if(iqMap EQ 1)
  {
    rspMsg->c3IQMap =  C3IQMAP_1;
  }
  else if(iqMap EQ 3)
  {
    rspMsg->c3IQMap =  C3IQMAP_3;
  }
  else if(iqMap EQ 5)
  {
    rspMsg->c3IQMap =  C3IQMAP_5;
  }
  else if(iqMap EQ 10)
  {
    rspMsg->c3IQMap =  C3IQMAP_10;
  }
  else if(iqMap EQ 15)
  {
    rspMsg->c3IQMap =  C3IQMAP_15;
  }
  else
  {
    rspMsg->c3IQMap =  C3IQMAP_20;
  }

  iqMap                  = getUint8( decBuf );      /* 8  */
  if(iqMap EQ 0)
  {
    rspMsg->c4IQMap =  C4IQMAP_0;
  }
  else if(iqMap EQ 1)
  {
    rspMsg->c4IQMap =  C4IQMAP_1;
  }
  else if(iqMap EQ 3)
  {
    rspMsg->c4IQMap =  C4IQMAP_3;
  }
  else if(iqMap EQ 5)
  {
    rspMsg->c4IQMap =  C4IQMAP_5;
  }
  else if(iqMap EQ 10)
  {
    rspMsg->c4IQMap =  C4IQMAP_10;
  }
  else if(iqMap EQ 15)
  {
    rspMsg->c4IQMap =  C4IQMAP_15;
  }
  else
  {
    rspMsg->c4IQMap =  C4IQMAP_20;
  }

  dummyUINT8             = getUint8( decBuf );      /* 12 */
  dummyUINT8             = getUint8( decBuf );      /* 13 */
  dummyUINT8             = getUint8( decBuf );      /* 14 */
  dummyUINT8             = getUint8( decBuf );      /* 15 */
  rspMsg->fwMode         = getUint8( decBuf );
  rspMsg->numberOfFA     = getUint8( decBuf );      /* 13 */
  dummyUINT16            = getUint16( decBuf );     /* 14-15 */


  faMapping  = getUint8( decBuf );      /* 16 */
  if(faMapping EQ 0)
  {
    rspMsg->mappingOf1FAStatus = MAPPINGOF1FASTATUS_0;
  }
  else if(faMapping EQ 1)
  {
    rspMsg->mappingOf1FAStatus = MAPPINGOF1FASTATUS_C1;
  }
  else if(faMapping EQ 2)
  {
    rspMsg->mappingOf1FAStatus = MAPPINGOF1FASTATUS_C2;
  }
  else if(faMapping EQ 3)
  {
    rspMsg->mappingOf1FAStatus = MAPPINGOF1FASTATUS_C3;
  }
  else if(faMapping EQ 4)
  {
    rspMsg->mappingOf1FAStatus = MAPPINGOF1FASTATUS_C4;
  }


  faMapping  = getUint8( decBuf );      /* 16 */
  if(faMapping EQ 0)
  {
    rspMsg->mappingOf2FAStatus = MAPPINGOF2FASTATUS_0;
  }
  else if(faMapping EQ 1)
  {
    rspMsg->mappingOf2FAStatus = MAPPINGOF2FASTATUS_C1;
  }
  else if(faMapping EQ 2)
  {
    rspMsg->mappingOf2FAStatus = MAPPINGOF2FASTATUS_C2;
  }
  else if(faMapping EQ 3)
  {
    rspMsg->mappingOf2FAStatus = MAPPINGOF2FASTATUS_C3;
  }
  else if(faMapping EQ 4)
  {
    rspMsg->mappingOf2FAStatus = MAPPINGOF2FASTATUS_C4;
  }

  faMapping  = getUint8( decBuf );      /* 16 */
  if(faMapping EQ 0)
  {
    rspMsg->mappingOf3FAStatus = MAPPINGOF3FASTATUS_0;
  }
  else if(faMapping EQ 1)
  {
    rspMsg->mappingOf3FAStatus = MAPPINGOF3FASTATUS_C1;
  }
  else if(faMapping EQ 2)
  {
    rspMsg->mappingOf3FAStatus = MAPPINGOF3FASTATUS_C2;
  }
  else if(faMapping EQ 3)
  {
    rspMsg->mappingOf3FAStatus = MAPPINGOF3FASTATUS_C3;
  }
  else if(faMapping EQ 4)
  {
    rspMsg->mappingOf3FAStatus = MAPPINGOF3FASTATUS_C4;
  }


  faMapping  = getUint8( decBuf );      /* 16 */
  if(faMapping EQ 0)
  {
    rspMsg->mappingOf4FAStatus = MAPPINGOF4FASTATUS_0;
  }
  else if(faMapping EQ 1)
  {
    rspMsg->mappingOf4FAStatus = MAPPINGOF4FASTATUS_C1;
  }
  else if(faMapping EQ 2)
  {
    rspMsg->mappingOf4FAStatus = MAPPINGOF4FASTATUS_C2;
  }
  else if(faMapping EQ 3)
  {
    rspMsg->mappingOf4FAStatus = MAPPINGOF4FASTATUS_C3;
  }
  else if(faMapping EQ 4)
  {
    rspMsg->mappingOf4FAStatus = MAPPINGOF4FASTATUS_C4;
  }

  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );


  rspMsg->cvc1           = getUint8( decBuf );
  rspMsg->cvc2           = getUint8( decBuf );
  rspMsg->cvc3           = getUint8( decBuf );
  rspMsg->cvc4           = getUint8( decBuf );

  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );

  rspMsg->txFilterBandwidth   = getUint8( decBuf );
  rspMsg->rxFilterBandwidth   = getUint8( decBuf );
  rspMsg->dlRatio             = getUint8( decBuf );
  rspMsg->ulRatio             = getUint8( decBuf );


  rspMsg->vswrShutdownAlarmThreshold  = getUint16( decBuf );
  dummyUINT16  = getUint16( decBuf );
  rspMsg->overpowerShutdownAlarmThreshold  = getUint32( decBuf );
  rspMsg->lowpowerAlarmThreshold  = getUint32( decBuf );
  rspMsg->centerFrequency         = getUint32( decBuf );

  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT16            = getUint16( decBuf );
  dummyUINT16            = getUint16( decBuf );
  dummyUINT16            = getUint16( decBuf );


  chBandwidth  = getUint8( decBuf );      /* 52 */
  if(chBandwidth EQ 0)
  {
    rspMsg->fa1ChannelBandwidthStatus = FA1CHANNELBANDWIDTHSTATUS_0;
  }
  else if(chBandwidth EQ 1)
  {
    rspMsg->fa1ChannelBandwidthStatus = FA1CHANNELBANDWIDTHSTATUS_1;
  }
  else if(chBandwidth EQ 3)
  {
    rspMsg->fa1ChannelBandwidthStatus = FA1CHANNELBANDWIDTHSTATUS_3;
  }
  else if(chBandwidth EQ 5)
  {
    rspMsg->fa1ChannelBandwidthStatus = FA1CHANNELBANDWIDTHSTATUS_5;
  }
  else if(chBandwidth EQ 10)
  {
    rspMsg->fa1ChannelBandwidthStatus = FA1CHANNELBANDWIDTHSTATUS_10;
  }
  else if(chBandwidth EQ 15)
  {
    rspMsg->fa1ChannelBandwidthStatus = FA1CHANNELBANDWIDTHSTATUS_15;
  }
  else if(chBandwidth EQ 20)
  {
    rspMsg->fa1ChannelBandwidthStatus = FA1CHANNELBANDWIDTHSTATUS_20;
  }

  chBandwidth  = getUint8( decBuf );      /* 52 */
  if(chBandwidth EQ 0)
  {
    rspMsg->fa2ChannelBandwidthStatus = FA2CHANNELBANDWIDTHSTATUS_0;
  }
  else if(chBandwidth EQ 1)
  {
    rspMsg->fa2ChannelBandwidthStatus = FA2CHANNELBANDWIDTHSTATUS_1;
  }
  else if(chBandwidth EQ 3)
  {
    rspMsg->fa2ChannelBandwidthStatus = FA2CHANNELBANDWIDTHSTATUS_3;
  }
  else if(chBandwidth EQ 5)
  {
    rspMsg->fa2ChannelBandwidthStatus = FA2CHANNELBANDWIDTHSTATUS_5;
  }
  else if(chBandwidth EQ 10)
  {
    rspMsg->fa2ChannelBandwidthStatus = FA2CHANNELBANDWIDTHSTATUS_10;
  }
  else if(chBandwidth EQ 15)
  {
    rspMsg->fa2ChannelBandwidthStatus = FA2CHANNELBANDWIDTHSTATUS_15;
  }
  else if(chBandwidth EQ 20)
  {
    rspMsg->fa2ChannelBandwidthStatus = FA2CHANNELBANDWIDTHSTATUS_20;
  }

  chBandwidth  = getUint8( decBuf );      /* 52 */
  if(chBandwidth EQ 0)
  {
    rspMsg->fa3ChannelBandwidthStatus = FA3CHANNELBANDWIDTHSTATUS_0;
  }
  else if(chBandwidth EQ 1)
  {
    rspMsg->fa3ChannelBandwidthStatus = FA3CHANNELBANDWIDTHSTATUS_1;
  }
  else if(chBandwidth EQ 3)
  {
    rspMsg->fa3ChannelBandwidthStatus = FA3CHANNELBANDWIDTHSTATUS_3;
  }
  else if(chBandwidth EQ 5)
  {
    rspMsg->fa3ChannelBandwidthStatus = FA3CHANNELBANDWIDTHSTATUS_5;
  }
  else if(chBandwidth EQ 10)
  {
    rspMsg->fa3ChannelBandwidthStatus = FA3CHANNELBANDWIDTHSTATUS_10;
  }
  else if(chBandwidth EQ 15)
  {
    rspMsg->fa3ChannelBandwidthStatus = FA3CHANNELBANDWIDTHSTATUS_15;
  }
  else if(chBandwidth EQ 20)
  {
    rspMsg->fa3ChannelBandwidthStatus = FA3CHANNELBANDWIDTHSTATUS_20;
  }

  chBandwidth  = getUint8( decBuf );      /* 52 */
  if(chBandwidth EQ 0)
  {
    rspMsg->fa4ChannelBandwidthStatus = FA4CHANNELBANDWIDTHSTATUS_0;
  }
  else if(chBandwidth EQ 1)
  {
    rspMsg->fa4ChannelBandwidthStatus = FA4CHANNELBANDWIDTHSTATUS_1;
  }
  else if(chBandwidth EQ 3)
  {
    rspMsg->fa4ChannelBandwidthStatus = FA4CHANNELBANDWIDTHSTATUS_3;
  }
  else if(chBandwidth EQ 5)
  {
    rspMsg->fa4ChannelBandwidthStatus = FA4CHANNELBANDWIDTHSTATUS_5;
  }
  else if(chBandwidth EQ 10)
  {
    rspMsg->fa4ChannelBandwidthStatus = FA4CHANNELBANDWIDTHSTATUS_10;
  }
  else if(chBandwidth EQ 15)
  {
    rspMsg->fa4ChannelBandwidthStatus = FA4CHANNELBANDWIDTHSTATUS_15;
  }
  else if(chBandwidth EQ 20)
  {
    rspMsg->fa4ChannelBandwidthStatus = FA4CHANNELBANDWIDTHSTATUS_20;
  }


  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );

  rspMsg->tx1stFaFrequency           = getUint32( decBuf );
  rspMsg->tx2ndFaFrequency           = getUint32( decBuf );
  rspMsg->tx3rdFaFrequency           = getUint32( decBuf );
  rspMsg->tx4thFaFrequency           = getUint32( decBuf );

  dummyUINT32             = getUint32( decBuf );
  dummyUINT32             = getUint32( decBuf );
  dummyUINT32             = getUint32( decBuf );
  dummyUINT32             = getUint32( decBuf );

  rspMsg->rx1stFaFrequency           = getUint32( decBuf );
  rspMsg->rx2ndFaFrequency           = getUint32( decBuf );
  rspMsg->rx3rdFaFrequency           = getUint32( decBuf );
  rspMsg->rx4thFaFrequency           = getUint32( decBuf );

  dummyUINT32             = getUint32( decBuf );
  dummyUINT32             = getUint32( decBuf );
  dummyUINT32             = getUint32( decBuf );
  dummyUINT32             = getUint32( decBuf );
  dummyUINT32             = getUint32( decBuf );

  rspMsg->txDelay           = getUint32( decBuf );
  rspMsg->rxDelay           = getUint32( decBuf );

  dummyUINT16             = getUint16( decBuf );
  dummyUINT16             = getUint16( decBuf );
  dummyUINT16             = getUint16( decBuf );
  dummyUINT16             = getUint16( decBuf );
  dummyUINT16             = getUint16( decBuf );
  dummyUINT16             = getUint16( decBuf );
  dummyUINT16             = getUint16( decBuf );
  dummyUINT16             = getUint16( decBuf );

  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );

  dummyUINT8             = getUint8( decBuf );
  dummyUINT16            = getUint16( decBuf );

  dummyUINT16            = getUint16( decBuf );
  dummyUINT16            = getUint16( decBuf );

  dummyUINT8             = getUint8( decBuf );
  dummyUINT8             = getUint8( decBuf );

  rspMsg->txPathAFaControl = getUint8( decBuf );
  rspMsg->txPathBFaControl = getUint8( decBuf );
  dummyUINT8 = getUint8( decBuf );
  dummyUINT8 = getUint8( decBuf );

  rspMsg->rxPathAFaControl = getUint8( decBuf );
  rspMsg->rxPathBFaControl = getUint8( decBuf );
  dummyUINT8 = getUint8( decBuf );
  rspMsg->rxPathBFaControl = getUint8( decBuf );

  for( multiDummy = 0; multiDummy<42 ;multiDummy++)
  {
	dummyUINT8             = getUint8( decBuf );
  }

  rspMsg->multiMode    = getUint8( decBuf );
  rspMsg->powerBoost   = getUint8( decBuf );
  rspMsg->filterSwithingMode   = getUint8( decBuf );

  dummyUINT8             = getUint8( decBuf );

  /* Path A */

  rspMsg->pathATxOnOrOff      = getUint8(decBuf);         /* 100 */
  rspMsg->pathATxEnableOrDisable  = getUint8( decBuf );       /* 101-- Tx Enable/Disable */
  dummyUINT8                  = getUint8( decBuf );       /* 102 */
  dummyUINT8                  = getUint8( decBuf );       /* 103 */
  dummyUINT16                 = getUint16( decBuf );      /* 104-105 */
  rspMsg->pathATemperature    = getUint16( decBuf );      /* 106-107 */
  rspMsg->pathATxRFPower      = getUint16( decBuf );      /* 108-109 */
  dummyUINT16                 = getUint16( decBuf );      /* 110-111 */
  rspMsg->pathAReturnLoss     = getUint16( decBuf );      /* 112-113 */
  dummyUINT16                 = getUint16( decBuf );      /* 114-115 */
  rspMsg->pathAFa1RssiDigitalIQLevel     = getUint32( decBuf );      /* 120-123 */
  rspMsg->pathAFa2RssiDigitalIQLevel     = getUint32( decBuf );      /* 124-127 */
  rspMsg->pathAFa3RssiDigitalIQLevel     = getUint32( decBuf );      /* 120-123 */
  rspMsg->pathAFa4RssiDigitalIQLevel     = getUint32( decBuf );      /* 124-127 */
  dummyUINT32     = getUint32( decBuf );   /* 136-139 */
  dummyUINT32     = getUint32( decBuf );   /* 136-139 */
  dummyUINT32     = getUint32( decBuf );   /* 136-139 */
  dummyUINT32     = getUint32( decBuf );   /* 136-139 */
  rspMsg->pathAFa1TssiDigitalIQLevel     = getUint32( decBuf );      /* 120-123 */
  rspMsg->pathAFa2TssiDigitalIQLevel     = getUint32( decBuf );      /* 124-127 */
  rspMsg->pathAFa3TssiDigitalIQLevel     = getUint32( decBuf );      /* 120-123 */
  rspMsg->pathAFa4TssiDigitalIQLevel     = getUint32( decBuf );      /* 124-127 */

  dummyUINT32     = getUint32( decBuf );
  dummyUINT32     = getUint32( decBuf );
  dummyUINT32     = getUint32( decBuf );
  dummyUINT32     = getUint32( decBuf );
  dummyUINT8      = getUint8( decBuf );
  dummyUINT8      = getUint8( decBuf );
  dummyUINT16     = getUint16( decBuf );

  rspMsg->pathAFa1TxAtten =getUint16( decBuf );
  rspMsg->pathAFa2TxAtten =getUint16( decBuf );
  rspMsg->pathAFa3TxAtten =getUint16( decBuf );
  rspMsg->pathAFa4TxAtten =getUint16( decBuf );

  dummyUINT16     = getUint16( decBuf );
  dummyUINT16     = getUint16( decBuf );
  dummyUINT16     = getUint16( decBuf );
  dummyUINT16     = getUint16( decBuf );

  /* Path B */

  rspMsg->pathBTxOnOrOff      = getUint8(decBuf);         /* 100 */
  rspMsg->pathBTxEnableOrDisable  = getUint8( decBuf );       /* 101-- Tx Enable/Disable */
  dummyUINT8                  = getUint8( decBuf );       /* 102 */
  dummyUINT8                  = getUint8( decBuf );       /* 103 */
  dummyUINT16                 = getUint16( decBuf );      /* 104-105 */
  rspMsg->pathBTemperature    = getUint16( decBuf );      /* 106-107 */
  rspMsg->pathBTxRFPower      = getUint16( decBuf );      /* 108-109 */
  dummyUINT16                 = getUint16( decBuf );      /* 110-111 */
  rspMsg->pathBReturnLoss     = getUint16( decBuf );      /* 112-113 */
  dummyUINT16                 = getUint16( decBuf );      /* 114-115 */
  rspMsg->pathBFa1RssiDigitalIQLevel     = getUint32( decBuf );      /* 120-123 */
  rspMsg->pathBFa2RssiDigitalIQLevel     = getUint32( decBuf );      /* 124-127 */
  rspMsg->pathBFa3RssiDigitalIQLevel     = getUint32( decBuf );      /* 120-123 */
  rspMsg->pathBFa4RssiDigitalIQLevel     = getUint32( decBuf );      /* 124-127 */
  dummyUINT32     = getUint32( decBuf );   /* 136-139 */
  dummyUINT32     = getUint32( decBuf );   /* 136-139 */
  dummyUINT32     = getUint32( decBuf );   /* 136-139 */
  dummyUINT32     = getUint32( decBuf );   /* 136-139 */
  rspMsg->pathBFa1TssiDigitalIQLevel     = getUint32( decBuf );      /* 120-123 */
  rspMsg->pathBFa2TssiDigitalIQLevel     = getUint32( decBuf );      /* 124-127 */
  rspMsg->pathBFa3TssiDigitalIQLevel     = getUint32( decBuf );      /* 120-123 */
  rspMsg->pathBFa4TssiDigitalIQLevel     = getUint32( decBuf );      /* 124-127 */

  dummyUINT32     = getUint32( decBuf );
  dummyUINT32     = getUint32( decBuf );
  dummyUINT32     = getUint32( decBuf );
  dummyUINT32     = getUint32( decBuf );
  dummyUINT8      = getUint8( decBuf );
  dummyUINT8      = getUint8( decBuf );
  dummyUINT16     = getUint16( decBuf );

  rspMsg->pathBFa1TxAtten =getUint16( decBuf );
  rspMsg->pathBFa2TxAtten =getUint16( decBuf );
  rspMsg->pathBFa3TxAtten =getUint16( decBuf );
  rspMsg->pathBFa4TxAtten =getUint16( decBuf );

  dummyUINT16     = getUint16( decBuf );
  dummyUINT16     = getUint16( decBuf );
  dummyUINT16     = getUint16( decBuf );
  dummyUINT16     = getUint16( decBuf );

  return TRUE;
}

TBOOL decRhDehConfigStatusInfoRsp(
  IO DecBuf             *decBuf,
  IO RadioHeadConfig    *rspMsg,
  IO UINT16             *varSeqNum
  )
{

  SINT16 varResult = 0;
  DehContext *dehContext = NULL;

  dehContext = getDehContext();
  varResult  = getUint16( decBuf );   /* 4-5 */
  /* Check the result */
  if( varResult NOTEQ 0 )
  {
    logError( "Expected=0, received=%d", varResult );
    reportError(" %s, %d : RhStatusReqFailed,Expected=0, received=%d ",__FILE__, __LINE__,varResult);
    return FALSE;
  }
  /**
   * @todo need to work on sequence number
   */
  *varSeqNum              = getUint16( decBuf );     /* 6-7 */
  rspMsg->radioHeadProfileId = 1;
  rspMsg->txAttenPathA = getUint16FromDecBuf ( decBuf, 332 );
  rspMsg->txAttenPathB = getUint16FromDecBuf ( decBuf, 334 );
  rspMsg->txDelay = getUint32FromDecBuf(decBuf, 152);
  rspMsg->rxDelay = getUint32FromDecBuf(decBuf, 156);
  rspMsg->vswrShutdownAlarmThreshold = getUint16FromDecBuf(decBuf, 44);
  rspMsg->overpowerAlarmThreshold = getUint32FromDecBuf(decBuf, 48);
  rspMsg->lowpowerAlarmThreshold = getUint32FromDecBuf(decBuf, 52);
  return TRUE;
}



/**
 * @details  This function fills RhDehDelaySettingRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsDelaySettingRsp : RSP structure.
 */
void decRhDehDelaySettingRsp(
  IO DecBuf               *decBuf,
  IO LkBsDelaySettingRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->sequenceNo    = getUint16( decBuf );

  logInfo( "%s", " Msg LkBsDelaySettingRsp Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " Delay Setting Rsp received and decoded ");
}

/**
 * @details  This function fills RhDehTxOnOffRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsTxOnOffRsp : RSP structure.
 */
void decRhDehTxOnOffRsp(
  IO DecBuf          *decBuf,
  IO LkBsTxOnOffRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->sequenceNo  = getUint16( decBuf );
  rsp->faId        = getUint16( decBuf );
  rsp->dataDummy   = getUint16( decBuf );

  logInfo( "%s", " Msg LkBsTxOnOffRsp Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " Tx On Off Rsp received and decoded ");
}

/**
 * @details  This function fills RhDehEnableOrDisableRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsEnableOrDisableRsp : RSP structure.
 */
void decRhDehEnableOrDisableRsp(
  IO DecBuf                  *decBuf,
  IO LkBsEnableOrDisableRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->sequenceNo  = getUint16( decBuf );

  logInfo( "%s", " Msg LkBsEnableOrDisableRsp Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " Enable Disable Rsp received and decoded ");
}

#if 0 /* Not Supported in release v1.0 */
/*
 * @details  This function fills RhDehDlUlRatioRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsDlUlRatioRsp : RSP structure.
 */
void decRhDehDlUlRatioRsp(
  IO DecBuf            *decBuf,
  IO LkBsDlUlRatioRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->varDummy    = getUint16( decBuf );

  logInfo( "%s", " Msg LkBsDlUlRatioRsp Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " Dl UL Ratio Rsp received and decoded ");
}
#endif

/**
 * @details  This function fills RhDehDataPathRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsDataPathRsp : RSP structure.
 */
void decRhDehDataPathRsp(
  IO DecBuf           *decBuf,
  IO LkBsDataPathRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->sequenceNo  = getUint16( decBuf );
  rsp->dataDummy1  = 0;
  rsp->dataDummy2  = 0;

  logInfo( "%s", " Msg LkBsDataPathRsp Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " DataPathRsp received and decoded ");
}

/**
 * @details  This function fills RhDehFaControlRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsFaControlRsp : RSP structure.
 */
void decRhDehFaControlRsp(
  IO DecBuf            *decBuf,
  IO LkBsFaControlRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->sequenceNo  = getUint16( decBuf );
  rsp->dataDummy1  = getUint8( decBuf );
  rsp->dataDummy2  = getUint8( decBuf );
  rsp->dataDummy3  = getUint16( decBuf );

  logInfo( "%s", " Msg LkBsFaControlRsp Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " FaControlRsp received and decoded ");
}

/**
 * @details  This function fills RhDehAlarmThresholdSettingRsp message
 * @param    DecBuf : Buffer from which data will needs to be decoded.
 * @param    LkBsAlarmThresholdSettingRsp : RSP structure.
 */
void decRhDehAlarmThresholdSettingRsp(
  IO DecBuf                        *decBuf,
  IO LkBsAlarmThresholdSettingRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->sequenceNo  = getUint16( decBuf );

  logInfo( "%s", " Msg LkBsAlarmThresholdSettingRsp Received with following parameters :" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " AlarmThresholdSettingRsp received and decoded ");

}

char *decRhDehRspError(
  IN SINT16 rhRspError,
  IN UINT32 msgId
  )
{
  switch(rhRspError)
  {
    case RH_MSGID_ERROR:
      return "MSG ID Field Error in REQ";
    case RH_VAR_ERROR:
      return "VAR Field Error in REQ";
    case RH_SET_RESTRICT:
      return "RH Set Restricted for a while ";
  }
  switch (msgId)
  {
    case MSG_LK_BS_FA_CONTROL_RSP:
      if( rhRspError EQ -4)
        return "Cell Setup Requesting Already Allocated Frequency";
      else if(rhRspError EQ -5 )
        return "FA Control Request Received during RU Enable";
    break;
    case MSG_LK_BS_ENABLE_OR_DISABLE_RSP:
    	if( rhRspError EQ -4)
		  return "Not Configured";
	    else if(rhRspError EQ -5 )
		  return "Already Disabled";
	    else if(rhRspError EQ -6 )
		  return "Already Enabled";
	    else if(rhRspError EQ -7 )
		  return "Disabled by User";
	    else if(rhRspError EQ -8 )
		  return "Already Shutdown";
	    else if(rhRspError EQ -9 )
		  return "Some path is NOK";
    	break;
    case MSG_LK_BS_DATA_PATH_RSP:
    	if( rhRspError EQ -102)
    	  return "Failed to receive INIT ACK" ;
    break;
    default:
      return "Abnormal Reason";
	}
  return "Abnormal Reason";
}


void decRhDehSwInstallRsp(
  IO DecBuf            *decBuf,
  IO LKBsSwInstallRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->sequenceNo  = getUint16( decBuf );
  rsp->dummy       = getUint16( decBuf );

  logInfo( "%s", " Msg LKBsSwInstallRsp Received with following parameters : \n" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " LKBsSwInstallRsp received and decoded ");
}

void decRhDehSwActivateRsp(
  IO DecBuf            *decBuf,
  IO LkBsSwActivateRsp  *rsp
  )
{
  rsp->varResult   = getUint16( decBuf );
  rsp->sequenceNo  = getUint16( decBuf );
  rsp->dummy       = getUint16( decBuf );

  logInfo( "%s", " Msg LkBsSwActivateRsp Received with following parameters : \n" );
  logResultType1( rsp->varResult );
  logInfo ( "%s", " LkBsSwActivateRsp received and decoded ");
}
