/**
* @file    bs_err_macro.h
* @section Copyright Powerwave Technologies, Inc., 2010
* @section This file contains all Macro related to bs_err
* @date:  7/7/2011
*/

#ifndef BS_ERR_MACRO_H_
#define BS_ERR_MACRO_H_

/* Exit error codes */
#define  EXIT_PICO_HOME_PATH_NOT_SET      2       /* Failing exit status.  */
#define  EXIT_CONFIG_PATH_NOT_SET         3
#define  EXIT_SW_PACKAGE_CFG_READ_FAIL    4       /* Failing exit status.  */
#define  EXIT_WRONG_VERSION_INFO          5       /* Failing exit status.  */
#define  EXIT_MOD_INIT_FAILED             6       /* Failing exit status.  */
#define  EXIT_CONFIG_READ_FAIL            7       /* Failing exit status.  */

/************************************************************************************/
/******************** General Errors    *********************************************/
/************************************************************************************/
/*TODO: Need to re-organise the file in this categories. Only enum values which are
 *      not used are cleaned up */
#define  BS_ERROR_SYSTEM_COMMAND_FAILED( cmd )\
  reportError("%s:%d :System command [%s] failed, errno=%d:%s ",  __FILE__, __LINE__,cmd,errno,strerror(errno));

#define BS_ERROR_MOD_REGISTER( modName, status )\
  reportError("%s:%d :Could not register %s Module, Status returned=%d", __FILE__, __LINE__,modName,status);

#define BS_ERROR_UNKNOWN_TABLE_ID( tableId )\
  reportError("%s:%d : unknown tableId %d", __FILE__, __LINE__,tableId);

#define BS_ERROR_UNKNOWN_TABLE_TYPE( tableType )\
  reportError("%s:%d : unknown table type %d", __FILE__, __LINE__,tableType);

#define BS_ERROR_INVALID_COLID_RCVD( tableId, rowId, colId )\
  reportError( "%s:%d :Invalid colId:%d received in Table = %d for row = %d ",__FILE__, __LINE__,colId, tableId, rowId );

#define BS_ERROR_UNDEFINED_MSGTRACE_MSG_ID( )\
  reportError( "%s:%d :Message ID Not defined ",__FILE__, __LINE__ );

#define BS_ERROR_RECEIVED_INVALID_MSG( int_msg, int_state )\
  reportError( "%s:%d : Received invalid message=%d, in state=%d", __FILE__, __LINE__ ,int_msg, int_state);

#define BS_ERROR_UNKNOWN_MOD_ID( modId )\
  reportError("%s:%d : unknown mod ID %d", __FILE__, __LINE__,modId);

#define BS_ERROR_UNKNOWN_MSG( msgId )\
  reportError("%s:%d : unknown message %d", __FILE__, __LINE__,msgId);

#define BS_ERROR_UNKNOWN_ACTION_RECD( bsAction )\
  reportError("%s:%d : Unknown Action received:%d", __FILE__, __LINE__,bsAction);

#define BS_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( msg, state )\
  reportError("%s:%d :Message=%d in state=%s is not expected", __FILE__, __LINE__,msg, state);

#define BS_ERR_UNKNOWN_EVENT(pst)\
  reportError("%s:%d :Source Entity=%d , Event=%d is not expected", __FILE__, __LINE__,pst->srcEnt,pst->event);

#define BS_ERROR_MSG_RCVD_IN_UNKNOWN_STATE( msg, stateName )\
  reportError("%s:%d :Message=%d in not handled in state=%s! Internal error! Check! ", __FILE__, __LINE__,msg, stateName);

#define BS_ERROR_FILE_NOT_FOUND( fileName )\
  reportError("%s:%d : %s file not found ", __FILE__, __LINE__,fileName);

#define BS_ERROR_INTERNAL_ERROR_OCCURED( errInfo )\
  reportError("%s:%d : Internal error occurred. Error Information: %s ", __FILE__, __LINE__, errInfo);

#define BS_ERROR_NO_PRINT_FUNCTION_DEFINED( msgName )\
  reportError("%s:%d : no print function defined for message %s", __FILE__, __LINE__,msgName);

#define BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED( tableName )\
  reportError("%s:%d : No configuration read function defined for table %s", __FILE__, __LINE__,tableName);

#define BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED( tableName )\
  reportError("%s:%d : No configuration write function defined for table %s", __FILE__, __LINE__,tableName);

#define BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( tableName )\
  reportError("%s:%d : No TLV 2 Structure function defined for table %s", __FILE__, __LINE__,tableName);

#define BS_ERROR_NO_STRUCT2TLV_FUNCTION_DEFINED( tableId )\
  reportError("%s:%d : No Structure 2 TLV function defined for table %d", __FILE__, __LINE__,tableId);

#define BS_ERROR_NO_EMPTY_FUNCTION_DEFINED( tableName )\
  reportError("%s:%d : No empty function defined for table %s", __FILE__, __LINE__,tableName);

#define BS_ERROR_CONFIG_WRITE_FAILED()\
  reportError("%s:%d : Failed to write config params to file", __FILE__, __LINE__);

#define BS_ERROR_CONFIG_READ_FAILED()\
  reportError("%s:%d : Failed to read config params from file ",__FILE__, __LINE__ );

#define BS_ERROR_ENC_FAILED_BUFFER_OVERFLOW( maxBytes ) \
  reportError("%s:%d : Encoding failed due to buffer overflow. Max Bytes allowed : %d", __FILE__, __LINE__, maxBytes );

#define BS_ERROR_FILE_ACCESS(fileName, error )\
  reportError("%s:%d : The local file path %s, cannot be accessed, error value is:%d ", __FILE__, __LINE__,fileName,error);

#define BS_ERROR_FILE_ACCESS_PERMISSION_DENIED(  filename )\
  reportError("%s:%d : File %s access permission denied ", __FILE__, __LINE__,filename );

#define BS_ERROR_FILE_ALREADY_OPEN(fileName )\
  reportError("%s:%d : file %s is already opened by other program", __FILE__, __LINE__,fileName);

#define BS_ERROR_OUT_OF_MEMORY()\
  reportError("%s:%d : Out of memory ",__FILE__, __LINE__ );

#define BS_ERROR_OPERATION_TIMEOUTED(timeOut )\
  reportError("%s:%d : Write operation timed out after %d seconds ", __FILE__, __LINE__,timeOut);

#define BS_ERROR_FILE_UNKNOWN_ERROR(errNo )\
  reportError("%s:%d : unknown file error %d from OS", __FILE__, __LINE__,errNo);

#define BS_ERROR_NO_DISK_SPACE( buf, index )\
  reportError("%s:%d : No disk space available", __FILE__, __LINE__);

#define BS_ERROR_SYSTEM_OVERLOAD()\
  reportError("%s:%d : System over loaded", __FILE__, __LINE__);

#define BS_ERROR_REMOTE_FILE_NOT_FOUND(fileName )\
  reportError("%s:%d : Error remote file %s  not found", __FILE__, __LINE__,fileName);

#define BS_ERROR_LENGTH_MISMATCH_FOR_RCVD_MSG(rcvdLen, expLen )\
  reportError("%s:%d : Length mismatch for rcvd msg Recvd %d Expected %d", __FILE__, __LINE__,rcvdLen,expLen);

#define BS_ERROR_MSG_CREATE_FAILED(msgId )\
  reportError("%s:%d : Failed to create msgId=%d", __FILE__, __LINE__,msgId);

#define BS_ERROR_SEND_MSG_INST_FAILED(modId, msgId )\
  reportError("%s:%d : sendMsgInst() failed for msgId=%d. Addr of dest modId=%d is not known", __FILE__, __LINE__,modId,msgId);

#define BS_ERROR_DATA_RECD_NOT_SAME(noBytesRecd, lenOfMsg )\
  reportError("%s:%d : Data received is not same, BytesReceived=%d, LengthOfMsg=%d ", __FILE__, __LINE__, noBytesRecd,lenOfMsg);

#define BS_ERROR_NO_MSG_ALLOCATED(inst, msgId )\
  reportError("%s:%d : Incorrect msg-free detected; decMsgInst() has been called when number of allocated instances=%d for msgId=%d", __FILE__, __LINE__,inst,msgId);

#define BS_ERROR_MSG_INST_THRESH_EXCEEDED(maxInst, inst, msgId )\
  reportError("%s:%d : Msg instance threshold exceeded; maxInstances=%d > allocated=%d for msgId=%d", __FILE__, __LINE__,maxInst,inst,msgId);

#define BS_ERROR_MAX_MSG_INST_ZERO()\
  reportError("%s:%d : Please check Msg instance in Message Description Table; It shouldn't be ZERO", __FILE__, __LINE__);

#define BS_ERROR_IN_OPENING_LOG_FILE(fileName )\
  reportError("%s:%d : error in opening log file %s, errno=%d:%s ", __FILE__, __LINE__, fileName, errno, strerror(errno));

#define BS_ERROR_IN_CLOSING_LOG_FILE(fileName )\
  reportError("%s:%d : error in closing log file %s", __FILE__, __LINE__,fileName);

#define BS_ERROR_UNSUPPORTED_PROTOCOL(protoNo )\
  reportError("%s:%d : unsupported protocol %d ",__FILE__, __LINE__, protoNo);

#define BS_ERROR_FAILED_CONNECTION_INIT(  serName )\
  reportError("%s:%d : failed connection init with server %s ",__FILE__, __LINE__,serName);

#define BS_ERROR_FTP_SENDING_ERROR()\
  reportError("%s:%d : Error in sending file through FTP ",__FILE__, __LINE__ );

#define BS_ERROR_FTP_RECEIVING_ERROR()\
  reportError("%s:%d : Error in receiving file through FTP ",__FILE__, __LINE__  );

#define BS_ERROR_QUEUE_FULL()\
  reportError("%s:%d : Queue full", __FILE__, __LINE__);

#define BS_ERROR_MSG_PROC_DURAT_EXCEEDED(msgId, msgProcDurationUsec )\
  reportError("%s:%d : Msg=%u took %lu usec", __FILE__, __LINE__, msgId,msgProcDurationUsec);

#define BS_ERROR_MAX_SCHED_DELAY_EXCEEDED( elapsed )\
  logError(" Sched delay=%lu", elapsed);

#define BS_ERROR_SOCK_CREATE_FAILED()\
  reportError("%s:%d : Error in socket creation", __FILE__,  __LINE__);

#define BS_ERROR_SET_SOCK_OPT_FAILED(sockOpt, errno)\
  reportError("%s:%d : Can't set socket option %d;  errno=%d", __FILE__, __LINE__, sockOpt,errno);

#define BS_ERROR_SOCK_BIND(errno)\
  reportError("%s:%d : Error in socket bind, errno=%d", __FILE__, __LINE__, errno);

#define BS_ERROR_SOCK_CONNECT()\
  reportError("%s:%d : Error while connecting socket", __FILE__, __LINE__);

#define BS_ERROR_DATA_SENT_NOT_SAME(len, numBytes, errno )\
  reportError("%s:%d: Couldn't send all bytes|wantedToSend=%d |actuallySent=%d |errno=%d", __FILE__, __LINE__,len,numBytes, errno);

#define BS_ERROR_FUN_NOT_FOUND(tcName )\
  reportError("%s:%d : Function for Test case=%s not found; test not run", __FILE__, __LINE__,tcName);

#define BS_ERROR_RECD_EXPECT_NOT_SAME()\
  reportError("%s:%d : received and expected values are not proper", __FILE__, __LINE__);

#define BS_ERROR_STR_CONVERT_INT(str)\
  reportError("%s:%d :Could not covert %s to UINT32", __FILE__, __LINE__,str);

#define BS_ERROR_SPAWN(task )\
  reportError("%s:%d :Could not spawn(%s)", __FILE__, __LINE__,task);

#define BS_ERROR_FILE_OPEN_FAILED( filename )\
  reportError("%s:%d :Could not open file ( Filename: %s) ", __FILE__, __LINE__, filename );

#define BS_ERROR_NO_PROCESSING_FUNCTION_DEFINED(msgId,state)\
  reportError("%s:%d : No Processing function defined for this Message=%s, in this state=%s",  __FILE__, __LINE__, msgId,state);

#define BS_ERROR_UNKNOWN_ENUM_VALUE(str, value)\
  reportError("%s:%d : %s, value is: %d", __FILE__, __LINE__, str, value );

/************************************************************************************/
/******************** Timer Related Errors ******************************************/
/************************************************************************************/
#define BS_ERROR_UNKNOWN_TIMEER_UNIT(timeUnit )\
  reportError(" %s:%d : Unknown TimeUnit=%d", __FILE__, __LINE__,timeUnit);

#define BS_ERROR_ALL_TIMERS_IN_USE(__FUNCTION__, timersInUse)\
  reportError("%s:%d : %s failed. All %d timers are in use", __FILE__, __LINE__,__FUNCTION__,timersInUse);

#define BS_ERROR_TIMER_NOT_ALLOCATED( __FUNCTION__, id)\
  reportError("%s:%d : %s called but TimerId=%d is not allocated", __FILE__, __LINE__,__FUNCTION__, id);

#define BS_ERROR_INVALID_TIMERID_EXPIRED(int_tmrType, int_state_name )\
  reportError("%s:%d : Received invalid TimerType=%d, in state=%s ", __FILE__, __LINE__, int_tmrType, int_state_name );

#define BS_ERROR_TIMER_NOT_FOUND( tmrName )\
  reportError("%s:%d : Timer not found!! TimerName :%s", __FILE__, __LINE__, tmrName );

#define BS_ERROR_DATE_AND_TIME_COULD_NOT_BE_SET( dateAndTimeString, errString )\
  reportError("%s:%d : Date And Time - %s could not be set!! Set was performed by %s errno=%d:%s  ",__FILE__, __LINE__, \
                                dateAndTimeString, errString,errno,strerror(errno) );

/************************************************************************************/
/******************** FTP/Upload/Download/Curl Related Errors************************/
/************************************************************************************/
#define BS_ERROR_NO_ACTIVE_DOWNLOAD()\
  reportError("%s:%d : No active download in progress", __FILE__, __LINE__);

#define BS_ERROR_NO_ACTIVE_UPLOAD()\
  reportError("%s:%d : No active upload in progress", __FILE__, __LINE__);

#define BS_ERROR_USER_INITIATED_CANCEL()\
  reportError("%s:%d : User has initiated cancel !! ", __FILE__, __LINE__);

#define BS_ERROR_UPLOAD_IN_PROGRESS(fileName )\
  reportError("%s:%d : File %s Upload in Progress", __FILE__, __LINE__,fileName);

#define BS_ERROR_DOWNLOAD_IN_PROGRESS(fileName )\
  reportError("%s:%d : File %s Download in Progress", __FILE__, __LINE__, fileName );

#define BS_ERROR_COULDNT_RESOLVE_HOST( serName )\
  reportError("%s:%d : server name %s cannot be resolved by DNS ", __FILE__, __LINE__, serName );

#define BS_ERROR_CHECK_CURL(curlError )\
  reportError("%s:%d : Check CURL Error Code, CURL returned:%d ", __FILE__, __LINE__,curlError );

#define BS_ERROR_DIRECTORY_SIZE_EXCEEDED( allowed, present )\
  reportError("%s:%d : Directory Size exceeded, Allowed:%d After:%d ", __FILE__, __LINE__, allowed, present );

#define BS_ERROR_COULDNT_CONNECT(serName )\
  reportError("%s:%d : cannot connect to server %s ", __FILE__, __LINE__,serName );

#define BS_ERROR_FTP_WEIRD_SERVER_REPLY(serName )\
  reportError("%s:%d : Reply from server %s can't be understood, refer fthlog ",__FILE__, __LINE__, serName);

#define BS_ERROR_FTP_ACCESS_DENIED( loginId )\
  reportError("%s:%d : Access denied, check username or password, Entered userId %s ",__FILE__, __LINE__, loginId )

#define BS_ERROR_FTP_WEIRD_PASS_REPLY()\
  reportError("%s:%d : Server reply for password can't be understood ",__FILE__, __LINE__);

#define BS_ERROR_FTP_WEIRD_USER_REPLY()\
  reportError("%s:%d : Server reply for username can't be understood ",__FILE__, __LINE__);

#define BS_ERROR_FTP_WEIRD_PASV_REPLY()\
  reportError("%s:%d : Server don't support FTP pasv mode ",__FILE__, __LINE__);

#define BS_ERROR_FTP_CANT_GET_HOST(serName )\
  reportError("%s:%d : cannot get %s server name ",__FILE__, __LINE__, serName);

#define BS_ERROR_FTP_COULDNT_SET_BINARY()\
  reportError("%s:%d : cannot set the FTP transfer mode to binary ",__FILE__, __LINE__ );

#define BS_ERROR_FTP_COULDNT_RETR_FILE(fileName )\
  reportError("%s:%d : cannot retrieve %s file on server ",__FILE__, __LINE__, fileName);

#define BS_ERROR_FTP_WRITE_ERROR(fileName )\
  reportError("%s:%d : Cannot create %s file locally ",__FILE__, __LINE__,fileName);

#define BS_ERROR_FTP_COULDNT_GET_SIZE()\
  reportError("%s:%d : FTP size can't be retrived from server ",__FILE__, __LINE__);

#define BS_ERROR_FILESIZE_EXCEEDED(maxSize )\
  reportError("%s:%d : Filesize of exceeded than %d max allowed ", __FILE__, __LINE__, maxSize);

#define BS_ERROR_CURL_ERROR_UNKNOWN()\
  reportError("%s:%d : CURL error cannot be interpreted", __FILE__, __LINE__);

#define BS_ERROR_CURL_SSH_ERROR_UNKNOWN()\
  reportError("%s:%d : CURL SSH error cannot be interpreted. ", __FILE__, __LINE__);

#define BS_ERROR_UPLOAD_CANCELED()\
  reportError("%s:%d : Upload cancelled", __FILE__, __LINE__);

#define BS_ERROR_DOWNLOAD_CANCELED()\
  reportError("%s:%d : Download cancelled", __FILE__), __LINE__);

#define BS_ERROR_CURL_CALLBACK_RETURNED_NULL()\
  reportError("%s:%d : CURL error cannot be interpreted", __FILE__, __LINE__);

#define BS_ERROR_CURL_HANDLE_NOT_CREATED() \
  reportError("%s:%d : CURL Handle not created, curl_easy_init() returned NULL ",__FILE__, __LINE__);

#define BS_ERROR_MODULE_STOPPED() \
  reportError("%s:%d : File Transfer stopped due to EXIT_CMD received ",__FILE__, __LINE__);

#define BS_ERROR_FILE_OPENING_FAILED( char_mode ) \
  reportError( " %s:%d : fopen() returned NULL, file can't be Opened in mode=%s ", __FILE__, __LINE__, char_mode )

#define BS_ERROR_CURL_RETURNED_NULL_IN_CALLBACK( ) \
  reportError( " %s:%d : CURL returned NULL, in the call back for user data", __FILE__, __LINE__ )

#define BS_ERROR_WRITE_CALLBACK_NEVER_CALLED() \
  reportError(" %s:%d: Error in getting size of file, Write Call back never called ",__FILE__, __LINE__)

#define BS_ERROR_READ_CALLBACK_NEVER_CALLED() \
  reportError(" %s:%d: Error in getting size of file, Read Call back never called ",__FILE__, __LINE__)

/************************************************************************************/
/******************** Errors used at BSM ********************************************/
/************************************************************************************/

#define BS_ERROR_RCVD_CONFIG_FAIL(moduleName, transId, errorNum, failReason ) \
  reportError("%s:%d : Received Configuration Failed from :%s Module: TransId: %d, errorNum :%d, failReason :%s",\
                               __FILE__, __LINE__, moduleName, transId, errorNum, failReason );

#define BS_ERROR_INVALID_TRANSACTION_ID(recvtransId, moduleName, exptransId ) \
  reportError("%s:%d : Received response with transaction ID =%d from :%s Module: Expected response TransId: %d",\
                               __FILE__, __LINE__, recvtransId, moduleName, exptransId );

#define BS_ERROR_UNSUPPORTED_PARAMETERS( colName, colValue1, colValue2 ) \
  reportError("%s:%d : Unsupported parameter %s with value=%d and value =%d",\
                             __FILE__, __LINE__, colName, colValue1, colValue2 );

#define BS_ERROR_UNSUPPORTED_PARAMETER( colName, colValue ) \
  reportError("%s:%d : Unsupported parameter %s with value=%d",\
                             __FILE__, __LINE__, colName, colValue );

#define BS_ERROR_CONFIG_PROCEDURE_FAILED( ) \
  reportError("%s:%d : Configuration Procedure failed.", __FILE__, __LINE__ );

#define BS_ERROR_ENABLE_PROCEDURE_NOT_STARTED( ) \
  reportError("%s:%d : Enable procedure not started due to some errors occurred previously", __FILE__, __LINE__ );

#define BS_ERROR_NO_RSP_FOR_CONFIG_REQ( moduleName ) \
  reportError("%s:%d : Module:%s did not respond for configuration Request", __FILE__, __LINE__, moduleName );

#define BS_ERROR_NO_RSP_FOR_INIT_CONFIG_REQ( moduleName ) \
  reportError("%s:%d : Module:%s did not respond for Init configuration Request", __FILE__, __LINE__, moduleName );

#define BS_ERROR_ENODEB_IP_ADDR_CHANGE_NOT_ALLOWED() \
  reportError("%s:%d : EnodeB IP address change is not allowed", __FILE__, __LINE__);

#define BS_ERROR_SET_NOT_ALLOWED_BY_NON_EMS_INTERFACE( ) \
  reportError("%s:%d : Parameter change not allowed by non-ems interface", __FILE__, __LINE__ );

#define BS_ERROR_ENODEB_LOCKED()\
  reportError("%s:%d : BS is locked", __FILE__, __LINE__);

#define BS_ERROR_ENODEB_NOT_LOCKED()\
  reportError("%s:%d : BS is not locked", __FILE__, __LINE__);

#define BS_ERROR_INVALID_DEVICE_TYPE(deviceType)\
  reportError("%s:%d : Device Type: %d is not valid", __FILE__, __LINE__, deviceType);

#define BS_ERROR_DEVICE_NOT_OPERATIONAL(deviceName)\
  reportError("%s:%d : Device %s is not operational.", __FILE__, __LINE__, deviceName);

#define BS_ERROR_CONSISTENCY_CHECK_FAILED( tableName, tlvInfo ){\
  ColName        colName;\
  UINT8      data[MAX_TLV_VALUE_IN_BYTES];\
  getColNameAndData(&tlvInfo, colName, data);\
  reportError("%s:%d : Consistency check failed for %s table, Tag Info:: TableId:%d RowId:%d, ColId:%d, ColName:%s, value entered:%s", \
                     __FILE__, __LINE__, tableName, tlvInfo.tag.tableId, tlvInfo.tag.rowId, tlvInfo.tag.colId, colName, data );\
  }

#define BS_ERROR_REQ_REJ_CRTICIAL_ALARMS_PRESENT( numCriticalAlarms )\
  reportError("%s:%d : Critical alarms are present. Number of active alarms:%d!! ", __FILE__, __LINE__ , numCriticalAlarms );

#define BS_ERROR_REQ_REJ_ALL_MODULES_ARE_NOT_UP()\
  reportError("%s:%d : All modules are not up!! ", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_RH_IS_NOT_ENABLED()\
  reportError("%s:%d : RH is not enabled!! ", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_CONFIGURATION_FAILED()\
  reportError("%s:%d : Configuration failed!! ", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_INITIAL_CONFIGURATION_FAILED()\
  reportError("%s:%d : Initial configuration failed!! ", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_DEVICE_INIT_FAILED()\
  reportError("%s:%d : Device Initialisation failed!! ", __FILE__, __LINE__);

#define BS_ERROR_GPS_PARA_NOT_CONFIGURED( errString )\
  reportError("%s:%d : GPS parameters are not configured by GPS device!! Reason : %s", __FILE__, __LINE__, errString );

#define BS_ERROR_REQ_REJ_UNABLE_TO_USE_CONFIGURED_TIME_AND_LOCATION_SOURCE( )\
  reportError("%s:%d : Unable to use configured time and location source, GPS not responding.\n Change the source to operator configured for UNLOCKING.", \
              __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_MME_IP_ADDRESS_NOT_CONFIGURED( mmeIp)\
  reportError("%s:%d : MME IP Address not Configured. Current IP is=%s ", __FILE__, __LINE__,mmeIp);

#define BS_ERROR_REQ_REJ_MCC_VALUE_NOT_CONFIGURED( mcc)\
  reportError("%s:%d : PLMN MCC value not Configured. Current MCC is=%s ", __FILE__, __LINE__,mcc);

#define BS_ERROR_REQ_REJ_MNC_VALUE_NOT_CONFIGURED( mnc)\
  reportError("%s:%d : PLMN MNC value not Configured. Current MNC is=%s ", __FILE__, __LINE__,mnc);

#define BS_ERROR_EMS_REGISTRATION_CONFIRM_NOT_RECEIVED( )\
  reportError("%s:%d : EMS Registration Success not received from SIH module", __FILE__, __LINE__);

#define BS_ERROR_TOTALTXPOWER_NOT_IN_RANGE(totalPower, minPower, maxPower)\
  reportError("%s:%d : Total Tx Power %d is not within range, allowed MIN power %d MAX power %d", __FILE__, __LINE__, totalPower, minPower, maxPower );

#define BS_ERROR_EMS_REGISTRATION_FAILED( )\
  reportError("%s:%d : EMS Registration Failed", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_REBOOT_IN_PROGRESS()\
  reportError("%s:%d : BS Reboot in progress", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_RESTART_IN_PROGRESS()\
  reportError("%s:%d : BS restart in progress", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_SW_ACT_IN_PROGRESS()\
  reportError("%s:%d : Software activation in progress", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_LOCK_IN_PROGRESS()\
  reportError("%s:%d : BS Lock in progress", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_MOD_DOWN_DISABLE_IN_PROGRESS()\
  reportError("%s:%d : BS disable in progress due to module down", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_LINK_DOWN_DISABLE_IN_PROGRESS()\
  reportError("%s:%d : BS disable in progress due to S1AP Link down", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_UNLOCK_IN_PROGRESS()\
  reportError("%s:%d : BS Unlock in progress", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_INIT_IN_PROGRESS()\
  reportError("%s:%d : BS Init in progress", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_INITIAL_CONFIG_IN_PROGRESS()\
  reportError("%s:%d : BS Initial Configuration in progress", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_COLLECT_INFO_IN_PROGRESS()\
  reportError("%s:%d : BS Collect information from modules is in progress", __FILE__, __LINE__);

#define BS_ERROR_REQ_REJ_SW_ACTIVATION_IN_PROGRESS()\
  reportError("%s:%d : Software Activation in progress", __FILE__, __LINE__);

#define BS_ERROR_OPSTOP_SUCC_NOT_RCVD( mod_name )\
  reportError("%s:%d :OPSTOP_SUCC message not received from module=%s", __FILE__, __LINE__,mod_name);

#define BS_ERROR_INVALID_ENODEB_ID( )\
  reportError("%s:%d :EnodeB ID is invalid. ID cannot be 0", __FILE__, __LINE__);

#define BS_ERROR_NEIGH_ENBID_SAME_AS_ENBID(enbId)\
  reportError("%s:%d :Neighbour ENBID is same as self ENBID = %d", __FILE__, __LINE__, enbId );

#define BS_ERROR_NEIGH_CELLID_SAME_AS_CELLID(cellId)\
  reportError("%s:%d :Neighbour CELLID is same as self CELLID = %d", __FILE__, __LINE__, cellId );

#define BS_ERROR_SET_IP_ADDR_FAILED(interface)\
  reportError("%s:%d :Failed to SET %s IP Address. This is a fatal error hence eNB will not continue with INIT_CONFIG procedure",\
               __FILE__, __LINE__, interface );

#define BS_ERROR_SET_IP_ADDR_REQ_TIMEOUTED()\
  reportError("%s:%d :Failed to SET IP Address. This is a fatal error so not performing INIT_CONFIG procedure", __FILE__, __LINE__ );

/************************************************************************************/
/******************** Alarm related errors*******************************************/
/************************************************************************************/
#define BS_ERROR_ALARM_CONDITION_UNKNOWN(condition )\
  reportError("%s:%d :Unknown Alarm condition received =%d ", __FILE__, __LINE__,condition);

#define BS_ERROR_ALARM_SOURCE_UNKNOWN(source )\
  reportError("%s:%d :Alarm source is unknown, received =%d ", __FILE__, __LINE__,source);

#define BS_ERROR_ALARM_ACTIVE_TABLE_FULL(alarm_id )\
  reportError("%s:%d :Alarm Active Table Full, Alarm Cause =%d not added to the table", __FILE__, __LINE__,alarm_id);

#define BS_ERROR_ALARM_ENTRY_NOT_DELETED(alarm_id, alarmTableType )\
  reportError("%s:%d :Alarm Id Entry = %d not deleted from %s table", __FILE__, __LINE__,alarm_id, alarmTableType);

#define BS_ERROR_ALARM_TABLE_IS_EMPTY(alarmTableType )\
  reportError("%s:%d : %s table is empty, hence operation not performed ", __FILE__, __LINE__, alarmTableType);

#define BS_ERROR_ALARM_ENTRY_NOT_FOUND( bsErrorString, bsErrorValue )\
  reportError("%s:%d : Alarm with bsError:%s:%d not found in alarm DB ", __FILE__, __LINE__, bsErrorString, bsErrorValue );

/************************************************************************************/
/******************** Errors used at PRM  *******************************************/
/************************************************************************************/

#define BS_ERROR_PROCESS_DID_NOT_START(exeName)\
  reportError("%s:%d : Process %s did not start.", __FILE__, __LINE__, exeName );

#define BS_ERROR_MODULE_DID_NOT_RESPOND(moduleName ) \
  reportError("%s:%d : Module:%s did not respond", __FILE__, __LINE__, moduleName );

#define BS_ERROR_MODULE_DID_NOT_RESPOND_AT_INIT(moduleName, retVal ) \
  reportError("%s:%d : Module:%s did not respond to kill(pid,0) command return value is %d", __FILE__, __LINE__, moduleName, retVal );

#define BS_ERROR_PROCESS_CRASHED(moduleName,pid ) \
  reportError("%s:%d : Process:%s with pid %d crashed", __FILE__, __LINE__, moduleName, pid );

#define BS_ERROR_PROCESS_DID_NOT_RESPOND(exeName, pid )\
  reportError("%s:%d : Process :%s with pid :%d did not respond", __FILE__, __LINE__, exeName, pid );

#define BS_ERROR_PROCESS_STILL_RUNNING(exeName, pid )\
  reportError("%s:%d : Process :%s with pid :%d is still running even when asked to exit", __FILE__, __LINE__, exeName, pid );

#define BS_ERROR_LOWER_ARM_BOOTING_FAILED(retVal)\
  reportError("%s:%d : Booting Lower ARM Image Fail. Value Returned is :%d  errno=%d", __FILE__, __LINE__, retVal,errno );

/************************************************************************************/
/******************** DEH related errors  *******************************************/
/************************************************************************************/
#define BS_ERROR_RH_CONFIG_TIMER_EXPIRED(timerName, noOfMsg)\
  reportError("%s:%d : Timer Name : %s ,and No Of Messages Waiting for Response : %d ", __FILE__, __LINE__, timerName ,noOfMsg);

#define BS_ERROR_RH_STATUS_TIMER_EXPIRED(timerName, noOfMsg)\
  reportError("%s:%d : Timer Name : %s ,and No Of Messages Waiting for Response : %d ", __FILE__, __LINE__, timerName ,noOfMsg);

#define BS_ERROR_RH_RESET_TIMER_EXPIRED(timerName, noOfMsg)\
  reportError("%s:%d : Timer Name : %s ,and No Of Messages Waiting for Response : %d ", __FILE__, __LINE__, timerName ,noOfMsg);

#define BS_ERROR_DATA_NOT_SENT_ON_RH_SOCKET(moduleName)\
  reportError("%s:%d : Module:%s Not Able to send Data on UDP Socket", __FILE__, __LINE__, moduleName );

#define BS_ERROR_DEH_ALREADY_PROCESSING_SAME_REQ(moduleName)\
  reportError("%s:%d : Module:%s Same Request is already processing", __FILE__, __LINE__, moduleName );

#define BS_ERR_BIST_ALREADY_RUNNING(moduleName)\
  reportError("%s:%d : Module:%s BIST is already processing", __FILE__, __LINE__, moduleName );

#define BS_ERR_BIST_TIMER_EXPIRED(moduleName)\
  reportError("%s:%d : Module:%s BIST Complete Timer Expired", __FILE__, __LINE__, moduleName );

#define BS_ERR_SET_LONG_POST_FAILED(moduleName)\
  reportError("%s:%d : Module:%s : Set on Long POST Failed", __FILE__, __LINE__, moduleName );

#define BS_ERROR_RH_CONFIG_FAILED(moduleName)\
  reportError("%s:%d : Module:%s : RH Configuration Failed", __FILE__, __LINE__, moduleName );

#define BS_ERROR_DEH_BUSY(moduleName)\
  reportError("%s:%d : Module:%s is in Busy State", __FILE__, __LINE__, moduleName );

#define BS_ERROR_RH_NOT_RUNNING(moduleName)\
  reportError("%s:%d : Module:%s Radio Head is Not Running", __FILE__, __LINE__, moduleName );

#define BS_ERROR_GET_ON_SINGLE_PARAMETER_UNSUPPORTED(tableId){\
  OamTableInfoForTlvPrint *tableInfo = NULL ;\
  tableInfo = printTlvGetTablePtrByTableId(tableId);\
  reportError("%s:%d : Table:%s GET on Single Parameter Not Supported", __FILE__, __LINE__, tableInfo->tableName );\
}

#define BS_ERROR_GET_ON_SINGLE_ROW_UNSUPPORTED(tableId){\
  OamTableInfoForTlvPrint *tableInfo = NULL ;\
  tableInfo = printTlvGetTablePtrByTableId(tableId);\
  reportError("%s:%d : Table:%s GET on Single Row Not Supported", __FILE__, __LINE__, tableInfo->tableName );\
}

#define BS_ERROR_DEVICE_NOT_CONNECTED(moduleName,devName)\
  reportError("%s:%d : Module:%s Device: %s is Not Connected", __FILE__, __LINE__, moduleName , devName );

#define BS_ERR_DEVICE_INIT_IN_PROGRESS()\
  reportError("%s:%d : Device Initiailisation in progress", __FILE__, __LINE__ );

#define BS_ERROR_RH_PATH_IN_DISABLE_STATE(moduleName)\
  reportError("%s:%d : Module:%s Radio Head is in Disable State", __FILE__, __LINE__, moduleName );

#define BS_ERROR_RH_DELAYED_RSP(moduleName,msgName, dehState )\
  reportError("%s:%d : Module:%s Delayed RSP Received from Radio head for Msg: %s in DEH State: %d", __FILE__, __LINE__, moduleName ,msgName , dehState );

#define BS_ERROR_TIMER_EXPIRED_IN_WRONG_STATE(moduleName,timerName, dehState )\
  reportError("%s:%d : Module:%s Timer: %s Expired in Wrong State:%d ", __FILE__, __LINE__, moduleName ,timerName , dehState );

#define BS_ERROR_RH_RSP_NOT_RECEIVED(moduleName,cmdName, dehState )\
  reportError("%s:%d : Module:%s Command: %s  State:%s ", __FILE__, __LINE__, moduleName ,cmdName , dehState );

#define BS_ERROR_RH_RSP_ERROR(moduleName,cmdName, dehState )\
  reportError("%s:%d : Module:%s Command: %s  State:%d ", __FILE__, __LINE__, moduleName ,cmdName , dehState );

#define BS_ERROR_SET_LED_COLOR_FAILED(ledId, ledColor )\
  reportError("%s:%d : Failed to set color of %s to %s ", __FILE__, __LINE__, ledId ,ledColor);

#define BS_ERROR_GPS_NOT_CONNECTED(moduleName)\
  reportError("%s:%d : Module:%s GPS Response not responding", __FILE__, __LINE__, moduleName);

#define BS_ERROR_MULTIPLE_ADDR_CHANGE_NOT_ALLOWED()\
  reportError("%s:%d : Multiple Ethernet address are tried to SET a time.", __FILE__, __LINE__);

#define BS_ERROR_ETHERNET_ADDR_ALREADY_IN_USE(addr)\
  reportError("%s:%d : Ethernet Address:%s is already in used.", __FILE__, __LINE__, addr);

/************************************************************************************/
/************************ UEH module related errors  ********************************/
/************************************************************************************/


#define UEH_ERROR_NO_CB_LOCATED( cbType, objId, eventRcvd ,srcEnt)\
  logError("Locating CB for %d(Type):%d(ID) failed on receiving Event:%d from Source entity: %s", cbType, objId, eventRcvd, getModuleName(sfiGetModIdFromEntityId(srcEnt)))

#define UEH_ERROR_MEM_ALLOC_FAILED( size, typeName)\
  logError(" Memory Alloc of %s, size %d failed", typeName, size)

#define UEH_ERROR_SSI_EVENT_SEND_FAILED( pst, eventType, objType, objId)\
  logError("Sending Message failed of the event type= %s, event=%d to Module=%s for %d:%d", uehGetEventName(eventType), pst->event, getModuleName(sfiGetModIdFromEntityId(pst->dstEnt)), objType, objId )

#define UEH_ERROR_RECEIVED_FAILURE_EVENT(event, srcEnt, valueRcvd, objType, objId)\
  logError("Received failure Msg:%d from %s with Value(status):%d for %d:%d", event, getModuleName(sfiGetModIdFromEntityId(srcEnt)), valueRcvd, objType, objId)

#define UEH_ERROR_ELMNT_OUT_OF_RANGE(elemName, elemValue, minValue, maxValue)\
  logError("Element %s received with value %d : Valid range from %d to %d ",elemName, elemValue, minValue, maxValue)

#define UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE(elemName, elemValue, validString)\
  logError("Invalid %s:%d received : Valid values are %s ",elemName, elemValue, validString)

#define UEH_UE_TIMER_EXPIRED(crnti, timerId, duration, Tmr)\
  logTimeout("crnti=%d|TimerId=%d|Duration=%d|Sent=%d|Expected=%d",  crnti, timerId, duration, Tmr.sentEvt, Tmr.expEvt)

#define UEH_ERROR_MSG_ENCODING_FAILED(msg)\
  logError("Failed to Encode %d PDU", msg)

#define UEH_ERROR_UNEXPECTED_EVENT(_ueCb,_ueEvt)\
  logError("Unexpected Event [%d:%s] received in state [%s] for CRNTI [%d]", ueEvt, uehGetEventName(ueEvt), uehGetUeStateName(ueCb->ueState), ueCb->crnti);

#define UEH_ERROR_MSG_FILLING_FAILED(msgName, objType, objId)\
  logError("Filling %s message failed for %d:%d", msgName, objType, objId)

#define UEH_ERROR_NULL_POINTER_IDENTIFIED()\
  logError("%s","NULL/EMPTY pointer identified")

#define UEH_PROCEDURE_FAILED(procName, objType, objId, reason)\
  logError("%s failed for objType(%d):%d due to the reason %s", procName, objType, objId, reason)

#define UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( msg, state )\
  logError("Message=%d in state=%d is not expected", msg, state)

#define UEH_ERROR_RESOURCE_NOT_AVAILABLE(elemName)\
  logError("%s is not available",elemName)

#define UEH_ERROR_LIMIT_REACHED(elemName,minVal,maxVal,curVal)\
  logError("%s limit reached minVal=%d,maxVal=%d,curVal=%d",elemName,minVal,maxVal,curVal)

#define UEH_ERROR_VALUE_NOT_SUPPORTED(elemName,elemValue)\
  logError("Value received for %s : %d is not supported",elemName,elemValue)

/************************************************************************************/
/******************** Sw Installation related errors  *******************************/
/************************************************************************************/

#define BS_ERROR_INSTALL_IN_PROGRESS(error )\
  reportError("%s:%d : %s", __FILE__, __LINE__, error);

#define BS_ERROR_INSTALL_INSTALLER_NOT_FOUND(error )\
  reportError("%s:%d : %s", __FILE__, __LINE__, error);

#define CEM_ERROR_UNEXPECTED_EVENT(_cellCb, _event)\
  logError("CELL|Rx Unexpected Event %d state=[isUnlocked=%d|isUnbarred=%d|isS1LinkUp=%d|proc=%d]", \
    _event, (_cellCb)->isUnlocked, (_cellCb)->isUnbarred, (_cellCb)->isS1LinkUp, (_cellCb)->proc);

/* BS_ERROR_INSTALL_INSTALLER_FAILED error contains 7 error value
 * BS_ERR_INSTALL_FILE_NOT_FOUND,
 * BS_ERR_INSTALL_PKG_UNPACKING_FAILED,
 * BS_ERR_INSTALL_PKG_DUPLICATE,
 * BS_ERR_INSTALL_VERIFICATION_FAILED,
 * BS_ERR_INSTALL_UNSUPPORTED_FILE_FORMAT,
 * BS_ERR_INSTALL_FLASH_ACCESS_FAILED,
 * BS_ERR_INSTALL_INVALID_INSTALLER_SCRIPT
 * */

#define BS_ERROR_INSTALL_INSTALLER_FAILED(error)\
  reportError("%s:%d : %s", __FILE__, __LINE__, error);

#define BS_ERROR_CONFIG_FILE_INVALID_SYNTAX(error )\
  reportError("%s:%d : %s", __FILE__, __LINE__, error);

#define BS_ERROR_INSTALL_PATCH_STARTUP_FAILED(error )\
  reportError("%s:%d : %s", __FILE__, __LINE__, error);

/************************************************************************************/
/******************** Sw Activation related errors  *********************************/
/************************************************************************************/

#define BS_ERROR_VER_ALREADY_ACTIVE(version )\
  reportError("%s:%d : Version %s is already active", __FILE__, __LINE__,version);

#define BS_ERROR_SW_VER_NOT_AVAILABLE(version )\
  reportError("%s:%d : Version %s is not available", __FILE__, __LINE__,version);

#define BS_ERROR_FLASH_ACCESS_FAILED()\
  reportError("%s:%d : Writing to persistent memory failed", __FILE__, __LINE__);

/************************************************************************************/
/******************** Trace related errors ******** *********************************/
/************************************************************************************/
#define BS_ERROR_TRACE_CTRL_CONFIRM_FAILED( moduleName, received )\
  reportError( "%s:%d : Trace Control COnfirm failed from Module :%s received value:%d", \
                    __FILE__, __LINE__, moduleName, received);

#define BS_ERROR_TRACE_ALREADY_RUNNING()\
  reportError("%s:%d : Trace Already running ",__FILE__, __LINE__);

#define BS_ERROR_TRACE_CONFIG_ALL_RESET()\
  reportError( "%s:%d : None of the trace protocols set to TRUE!! Set and then try again",__FILE__, __LINE__);

#define BS_ERROR_TRC_FILE_NOT_OPENED()\
  reportError("%s:%d : Trace file not opened", __FILE__, __LINE__);

#define BS_ERROR_TRACE_NOT_RUNNING()\
  reportError( "%s:%d : Trace Not running ",__FILE__, __LINE__);

/************************************************************************************/
/******************** Counters related errors ******** *********************************/
/************************************************************************************/
#define BS_ERROR_COUNTER_VALUES_NOT_RECEIVED(counterName)\
  reportError( "%s:%d : %s counter values are not received ",__FILE__, __LINE__, counterName);

#define BS_ERROR_PMH_TIMER_NOT_STARTED(timerName)\
  reportError( "%s:%d : %s timer cannot be started ",__FILE__, __LINE__, timerName);

#define BS_ERROR_INVALID_PMH_TIMERID_EXPIRED(int_tmrType )\
  reportError("%s:%d : Received invalid TimerType=%d ",__FILE__, __LINE__, int_tmrType );

#define BS_ERROR_ALL_PMLOGFILESTATUS_ROWS_FILLED(tableName)\
  reportError("%s:%d : All rows are filled up in %s Table ",__FILE__, __LINE__, tableName );

#define BS_ERROR_ALL_PM_GROUPS_DISABLED()\
  reportError( "%s:%d : No counter groups enabled ",__FILE__, __LINE__);

#define BS_ERROR_COMBINED_STAT_FILE_SIZE_EXCEEDED(maxCombinedPmFileSize)\
  reportError( "%s:%d : PmLog directory size exceeded threshold %d bytes",__FILE__, __LINE__, maxCombinedPmFileSize);

/************************************************************************************/
/******************** S1AP related errors   *****************************************/
/************************************************************************************/
#define BS_ERROR_S1AP_HANDOVER_TYPE_NOT_SUPPORTED(handoverTypeName)\
  reportError( "%s:%d : Handover Type :%s not supported yet",__FILE__, __LINE__, handoverTypeName);

#define BS_ERROR_S1AP_TARGET_TYPE_NOT_SUPPORTED(targetTypeName)\
  reportError( "%s:%d : Target Type :%s not supported yet",__FILE__, __LINE__, targetTypeName);

#define BS_ERROR_UNKNOWN_MSG_RCVD( modId, msgId ) \
  reportError( "%s:%d UNKNOWN MESSAGE RECVD at module=%s, msgId=%d \n ", __FILE__, __LINE__, getModuleName(modId), (msgId) )

/************************************************************************************/
/******************** PMH related errors   *****************************************/
/************************************************************************************/

#define BS_ERROR_TIMER_EXPIRED( timerId) \
  reportError( "%s:%d : Timer %s Expired",__FILE__, __LINE__, timerNameStr(timerId));

#define BS_ERROR_CONFIG_CHANGE_RCVD_WHILE_REQUESTED_PROFILE_IS_RUNNING(profileId, counterId)\
  reportError( "%s:%d : Config change is received on CounterId = %s while Profile%d is running",__FILE__, __LINE__, getCounterIdStr(counterId), profileId);

#define BS_ERROR_TRANSID_ALLOC_FAILED()\
  reportError( "%s:%d : Allocation of transId failed",__FILE__, __LINE__);

#define BS_ERROR_NO_COUNTER_IS_CONFIGURED_FOR_REQUESTED_PROFILE(profileId)\
  reportError( "%s:%d : No counter is configured for the profile %d",__FILE__, __LINE__, profileId);

#define BS_ERROR_TRANSID_DEALLOC_FAILED(transId)\
  reportError( "%s:%d : Deallocation of transId = %d failed",__FILE__, __LINE__, transId);

#endif /* BS_ERR_MACRO_H_ */
