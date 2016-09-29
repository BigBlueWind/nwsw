/**
 * @file    bsm_event_func.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief   This file contains event functions which will be sent to SIH.
 *
 * @author:  Shweta Polepally
 * @date:    Jan 2011
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   mmeAddr  MME Address received from SIAP
 * @details This function fills and sends MsgBsmSihMmeReset event to SIH.
 **/
void sendEventMmeReset(
  IN UINT8     *mmeAddr
  )
{
  MsgBsmSihMmeReset     event;
  NotiObj               notiObj;
  EncBuf                encBuf;
  clearBytes( &event, sizeof(MsgBsmSihMmeReset) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_MMERESET_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.mmeIpAddr, strlen(mmeAddr)+1, "%s", mmeAddr );

  /* Convert Struct to TLV */
  convertStruct2TlvMmeReset( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventFileUploadCompleted(
  IN UINT8         *fileName
  )
{
  MsgBsmSihFileUploadCompleted     event;
  NotiObj                          notiObj;
  EncBuf                           encBuf;
  clearBytes( &event, sizeof(MsgBsmSihFileUploadCompleted) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_FILEUPLOADCOMPLETED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.fileName, strlen(fileName)+1, "%s", fileName );

  /* Convert Struct to TLV */
  convertStruct2TlvFileUploadCompleted( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventFileUploadFailed(
  IN UINT8         *fileName,
  IN UINT8         *errorInfo
  )
{
  MsgBsmSihFileUploadFailed     event;
  NotiObj                       notiObj;
  EncBuf                        encBuf;
  clearBytes( &event, sizeof(MsgBsmSihFileUploadFailed) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_FILEUPLOADFAILED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.fileName, (strlen(fileName)+1), "%s", fileName );
  snprintf( (UINT8*)&event.errorInfo, (strlen(errorInfo)+1), "%s", errorInfo );

  /* Convert Struct to TLV */
  convertStruct2TlvFileUploadFailed( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventFileDownloadCompleted(
  IN UINT8         *fileName
  )
{
  MsgBsmSihFileDownloadCompleted     event;
  NotiObj                            notiObj;
  EncBuf                             encBuf;
  clearBytes( &event, sizeof(MsgBsmSihFileDownloadCompleted) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_FILEDOWNLOADCOMPLETED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.fileName, (strlen(fileName)+1), "%s", fileName );

  /* Convert Struct to TLV */
  convertStruct2TlvFileDownloadCompleted( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventFileDownloadFailed(
  IN UINT8         *fileName,
  IN UINT8         *errorInfo
  )
{
  MsgBsmSihFileDownloadFailed     event;
  NotiObj                         notiObj;
  EncBuf                          encBuf;
  clearBytes( &event, sizeof(MsgBsmSihFileDownloadFailed) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_FILEDOWNLOADFAILED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.fileName, (strlen(fileName)+1), "%s", fileName );
  snprintf( (UINT8*)&event.errorInfo, (strlen(errorInfo)+1), "%s", errorInfo );

  /* Convert Struct to TLV */
  convertStruct2TlvFileDownloadFailed( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventMessageTraceCompleted(
  IN UINT8         *fileName
  )
{
  MsgBsmSihMessageTraceCompleted     event;
  NotiObj                            notiObj;
  EncBuf                             encBuf;
  clearBytes( &event, sizeof(MsgBsmSihMessageTraceCompleted) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_MESSAGETRACECOMPLETED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.fileName, (strlen(fileName)+1), "%s", fileName );

  /* Convert Struct to TLV */
  convertStruct2TlvMessageTraceCompleted( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventMessageTraceFailed(
  IN UINT8   *errorInfo
  )
{
  MsgBsmSihMessageTraceFailed        event;
  NotiObj                            notiObj;
  EncBuf                             encBuf;
  clearBytes( &event, sizeof(MsgBsmSihMessageTraceFailed) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_MESSAGETRACEFAILED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.errorInfo, (strlen(errorInfo)+1), "%s", errorInfo );

  /* Convert Struct to TLV */
  convertStruct2TlvMessageTraceFailed( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventCounterFileAvailable(
  IN UINT8         *fileName
  )
{
  MsgBsmSihCounterFileAvailable        event;
  NotiObj                              notiObj;
  EncBuf                               encBuf;
  clearBytes( &event, sizeof(MsgBsmSihCounterFileAvailable) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_COUNTERFILEAVAILABLE_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.fileName, (strlen(fileName)+1), "%s", fileName );

  /* Convert Struct to TLV */
  convertStruct2TlvCounterFileAvailable( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventCounterCollectionFailed(
  IN UINT8   *errorInfo
  )
{
  MsgBsmSihCounterCollectionFailed        event;
  NotiObj                              notiObj;
  EncBuf                               encBuf;
  clearBytes( &event, sizeof(MsgBsmSihCounterCollectionFailed) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_COUNTERCOLLECTIONFAILED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.errorInfo, (strlen(errorInfo)+1), "%s", errorInfo );

  /* Convert Struct to TLV */
  convertStruct2TlvCounterCollectionFailed( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventMeasurementCompleteInd(
  IO MsgPmhBsmMeasurementCompleteInd   *ind
  )
{
  MsgBsmSihMeasurementCompleted        event;
  NotiObj                              notiObj;
  EncBuf                               encBuf;
  clearBytes( &event, sizeof(MsgBsmSihMeasurementCompleted) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_MEASUREMENTCOMPLETED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  event.profileId                  = ind->profileId;
  snprintf( (UINT8*)&event.stoppedReason, MAX_SIZE_MEASUREMENTCOMPLETED_STOPPEDREASON + 1 , "%s", ind->cause );

  /* Convert Struct to TLV */
  convertStruct2TlvMeasurementCompleted(&event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

static void resetConfigChangeAckByEmsField()
{
  EnodebConfig   *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  enodebConfig->configChangeAckByEms = FALSE;
}

void sendEventConfigurationChanged(
  IN OamIfType      oamIf
  )
{
  MsgBsmSihConfigurationChanged        event;
  NotiObj                              notiObj;
  EncBuf                               encBuf;
  clearBytes( &event, sizeof(MsgBsmSihConfigurationChanged) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  /* Set configChangeAckByEms to FALSE in enbConfig Table.
   * Set this to TRUE ones received from EMS in SET_OBJ_REQ message */
  resetConfigChangeAckByEmsField();

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_CONFIGURATIONCHANGED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  event.oamIfType = oamIf;

  /* Convert Struct to TLV */
  convertStruct2TlvConfigurationChanged( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventConfigurationSaved(
  IN OamIfType      oamIf
  )
{
  MsgBsmSihConfigurationSaved           event;
  NotiObj                               notiObj;
  EncBuf                                encBuf;
  clearBytes( &event, sizeof(MsgBsmSihConfigurationSaved) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  /* Set configChangeAckByEms to FALSE in enbConfig Table.
   * Set this to TRUE ones received from EMS in SET_OBJ_REQ message */
  resetConfigChangeAckByEmsField();

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_CONFIGURATIONSAVED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  event.oamIfType = oamIf;

  /* Convert Struct to TLV */
  convertStruct2TlvConfigurationSaved( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventConfigurationRollBack(
  IN OamIfType      oamIf
  )
{
  MsgBsmSihConfigurationRollBack        event;
  NotiObj                               notiObj;
  EncBuf                                encBuf;
  clearBytes( &event, sizeof(MsgBsmSihConfigurationRollBack) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  /* Set configChangeAckByEms to FALSE in enbConfig Table.
   * Set this to TRUE ones received from EMS in SET_OBJ_REQ message */
  resetConfigChangeAckByEmsField();

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_CONFIGURATIONROLLBACK_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  event.oamIfType = oamIf;
  /* Convert Struct to TLV */
  convertStruct2TlvConfigurationRollBack( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventEnbReboot(
  IN UINT8*     reason
  )
{
  MsgBsmSihEnbRebootInd   event;
  NotiObj                 notiObj;
  EncBuf                  encBuf;
  clearBytes( &event, sizeof(MsgBsmSihEnbRebootInd) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_ENBREBOOTIND_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.rebootReason, (strlen(reason)+1), "%s", reason );

  /* Convert Struct to TLV */
  convertStruct2TlvEnbRebootInd( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}


void sendEventSwActivationFailed(
  IN SwType     swType,
  IN UINT8*     version,
  IN UINT8*     errorInfo
  )
{
  MsgBsmSihSwActivationFailed   event;
  NotiObj                       notiObj;
  EncBuf                        encBuf;
  clearBytes( &event, sizeof(MsgBsmSihSwActivationFailed) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_SWACTIVATIONFAILED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  event.swType = swType;
  snprintf( (UINT8*)&event.version, (strlen(version)+1), "%s", version );
  snprintf( (UINT8*)&event.errorInfo, (strlen(errorInfo)+1), "%s", errorInfo );

  /* Convert Struct to TLV */
  convertStruct2TlvSwActivationFailed( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventSwActivationSuccess(
  IN SwType     swType,
  IN UINT8*     version
  )
{
  MsgBsmSihSwActivationSuccess       event;
  NotiObj                            notiObj;
  EncBuf                             encBuf;
  clearBytes( &event, sizeof(MsgBsmSihSwActivationSuccess) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_SWACTIVATIONSUCCESS_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  event.swType = swType;
  snprintf( (UINT8*)&event.version, (strlen(version)+1), "%s", version );

  /* Convert Struct to TLV */
  convertStruct2TlvSwActivationSuccess( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventSwInstallFailed(
  IN UINT8*     fileName,
  IN UINT8*     errorInfo
  )
{
  MsgBsmSihSwInstallFailed           event;
  NotiObj                            notiObj;
  EncBuf                             encBuf;
  clearBytes( &event, sizeof(MsgBsmSihSwInstallFailed) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_SWINSTALLFAILED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.fileName, (strlen(fileName)+1), "%s", fileName );
  snprintf( (UINT8*)&event.errorInfo, (strlen(errorInfo)+1), "%s", errorInfo );

  /* Convert Struct to TLV */
  convertStruct2TlvSwInstallFailed( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventSwInstallSuccess(
  IN UINT8*     fileName,
  IN UINT8*     version
  )
{
  MsgBsmSihSwInstallSuccess      event;
  NotiObj                        notiObj;
  EncBuf                         encBuf;
  clearBytes( &event, sizeof(MsgBsmSihSwInstallSuccess) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_SWINSTALLSUCCESS_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( (UINT8*)&event.fileName, (strlen(fileName)+1), "%s", fileName );
  snprintf( (UINT8*)&event.version, (strlen(version)+1), "%s", version );

  /* Convert Struct to TLV */
  convertStruct2TlvSwInstallSuccess( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventEnbRegistrationReq( void )
{
  MsgBsmSihEnbRegistrationReq      event;
  NotiObj                          notiObj;
  EncBuf                           encBuf;
  clearBytes( &event, sizeof(MsgBsmSihEnbRegistrationReq) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  EnbAddresses    *enbAddrTable        = (EnbAddresses*)getFirstRowOfTable(ENBADDRESSES);
  LocationStatus  *locationStatusTable = (LocationStatus*)getFirstRowOfTable(LOCATIONSTATUS);

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_ENBREGISTRATIONREQ_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  snprintf( event.latitude, strlen(locationStatusTable->latitude)+1, "%s", locationStatusTable->latitude);
  snprintf( event.longitude, strlen(locationStatusTable->longitude)+1, "%s", locationStatusTable->longitude);
  snprintf( event.productId, strlen(enbAddrTable->uniqueProductId)+1, "%s", enbAddrTable->uniqueProductId);

  /* Convert Struct to TLV */
  convertStruct2TlvEnbRegistrationReq( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventEnbOperationalStateChanged(
  IN OperationalState    operationalState
  )
{
  MsgBsmSihEnbOperationalStateChanged   event;
  NotiObj                               notiObj;
  EncBuf                                encBuf;
  clearBytes( &event, sizeof(MsgBsmSihEnbOperationalStateChanged) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_ENBOPERATIONALSTATECHANGED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  event.operationalState           = operationalState;

  /* Convert Struct to TLV */
  convertStruct2TlvEnbOperationalStateChanged( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventCellBarredInd()
{
  MsgBsmSihCellBarredInd  event;
  NotiObj                 notiObj;
  EncBuf                  encBuf;
  CellConfig             *cellConfig  =  (CellConfig*)getFirstRowOfTable(CELLCONFIG);

  clearBytes( &event, sizeof(MsgBsmSihCellBarredInd) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );


  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  event.cellId                     = cellConfig->cellId;
  getAndFillDateAndTimeStamp( MAX_SIZE_CELLBARREDIND_DATEANDTIMESTAMP, event.dateAndTimeStamp );

  /* Convert Struct to TLV */
  convertStruct2TlvCellBarredInd(&event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventEnbAdminStateChanged(
  IN AdminState          adminState
  )
{
  MsgBsmSihEnbAdminStateChanged         event;
  NotiObj                               notiObj;
  EncBuf                                encBuf;
  clearBytes( &event, sizeof(MsgBsmSihEnbAdminStateChanged) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_ENBADMINSTATECHANGED_DATEANDTIMESTAMP, event.dateAndTimeStamp );
  event.adminState                 = adminState;

  /* Convert Struct to TLV */
  convertStruct2TlvEnbAdminStateChanged( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventEnbLocationChanged()
{
  MsgBsmSihEnbLocationChanged           event;
  NotiObj                               notiObj;
  EncBuf                                encBuf;

  LocationStatus       *locationStatus     = (LocationStatus*)getFirstRowOfTable(LOCATIONSTATUS);

  clearBytes( &event, sizeof(MsgBsmSihEnbLocationChanged) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  snprintf(event.longitude, MAX_SIZE_LOCATIONSTATUS_LONGITUDE+1, "%s",locationStatus->longitude);
  snprintf(event.latitude,  MAX_SIZE_LOCATIONSTATUS_LATITUDE+1, "%s",locationStatus->latitude);

  getAndFillDateAndTimeStamp( MAX_SIZE_ENBLOCATIONCHANGED_DATEANDTIMESTAMP, event.dateAndTimeStamp );

  /* Convert Struct to TLV */
  convertStruct2TlvEnbLocationChanged( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

void sendEventEnbNoNeighborConfigured()
{
  MsgBsmSihNoNeighborConfigured         event;
  NotiObj                               notiObj;
  EncBuf                                encBuf;
  clearBytes( &event, sizeof(MsgBsmSihNoNeighborConfigured) );
  clearBytes( &notiObj, sizeof(NotiObj) );
  initEncBuf( &encBuf );

  event.enodebId                   = getEnodeBId();
  event.notificationSequenceNumber = getNextNotificationSequenceNumber();
  getAndFillDateAndTimeStamp( MAX_SIZE_NONEIGHBORCONFIGURED_DATEANDTIMESTAMP, event.dateAndTimeStamp );

  /* Convert Struct to TLV */
  convertStruct2TlvNoNeighborConfigured( &event, &notiObj );

  /* Encode TLV in encodeBuf */
   encNotiObjMsg( &notiObj, &encBuf );
  /* Send Message */
   createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}
