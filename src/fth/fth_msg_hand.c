/**
 * @file    fth_msg_hand.c
 * @brief This file holds the code for handling Messages received by FTH
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author  Lakshmi Narayana Madala/ Ankur Agrawal
 * @date    04-Oct-2010
 */

#include "fth.h"

/* Global state machine */
FthStateMachine     fthStateMachine[FTH_MAX_STATE][MSG_ID_MAX];


/**
 * @param   fileName      file name of the transferred file.
 * @param   fileStatus    status file download/upload
 * @param   wasSuccessful whether transfer is successful or not.
 * @details This function will update the file status table when file transferred is completed or failed.
 * */
void fthUpdateFileEventHistory(
  IN UINT8 	    *fileName,
  IN FileStatus      fileStatus,
  IN TBOOL           wasSuccessful
  )
{
  UINT8 i;
  FthContext *fthContext = getFthContext();
  FileEventHistory  *rowOfFileEventHistory = (FileEventHistory*)getFirstRowOfTable(FILEEVENTHISTORY);

  /* Skip rows which are already filled */
  for ( i=0; i < fthContext->indexFileStatusTable; i++ )
  {
	getNextRowOfTable(rowOfFileEventHistory);
  }

  /* Fill current row of status table */
  snprintf( (UINT8*)&rowOfFileEventHistory->fileName, MAX_SIZE_FILEEVENTHISTORY_FILENAME + 1, "%s", fileName);
  rowOfFileEventHistory->wasSuccessful = wasSuccessful;
  rowOfFileEventHistory->fileStatus = fileStatus;
  rowOfFileEventHistory->index = (fthContext->indexFileStatusTable)+1;

  /* Update index in FTH context which are used diring GET_OBJ_REQ */
  fthContext->indexFileStatusTable++;
  fthContext->noOfEntriesInFileEventHistory++;
  fthContext->indexFileStatusTable = (fthContext->indexFileStatusTable) % MAX_ROWS_IN_FILEEVENTHISTORY_TABLE;
  if(fthContext->noOfEntriesInFileEventHistory > MAX_ROWS_IN_FILEEVENTHISTORY_TABLE)
  {
    fthContext->noOfEntriesInFileEventHistory = MAX_ROWS_IN_FILEEVENTHISTORY_TABLE;
  }
}
/**
 * @param setReq  set object request
 * @param fthContext context of FTH
 * @details This function will delete fileEventHistory table
 */
void fthPerformSetOnEnbAdmin(
  IO SetObjReq  *setReq, 
  IO FthContext *fthContext
)
{
  EnbAdmin  enbAdmin;

  FileEventHistory  *rowOfFileEventHistory = (FileEventHistory*)getFirstRowOfTable(FILEEVENTHISTORY);
  GenericTable      *tableFunc             = getTableFunctions(ENBADMIN);
  
  clearBytes(&enbAdmin,sizeof(EnbAdmin));

  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setReq->tlvArray, &enbAdmin );
  }
 
  if ( ENBCOMMAND_DELETEFILEEVENTHISTORY NOTEQ enbAdmin.eNbCommand )
  {
    BS_ERROR_UNKNOWN_ACTION_RECD( enbAdmin.eNbCommand );
    fthFillAndSendSetObjFail( setReq , BS_ERR_UNKNOWN_ACTION_RECD, fthContext );
    return;
  }

  /* Initialise the File Event History Table */
  logInfo("deleting %d  rows in fileEventHistory ",fthContext->noOfEntriesInFileEventHistory);
  fthContext->indexFileStatusTable =0;
  fthContext->noOfEntriesInFileEventHistory =0;

  clearBytes( rowOfFileEventHistory, sizeof(FileEventHistory) );

  snprintf( (UINT8*)&rowOfFileEventHistory->fileName, MAX_SIZE_FILEEVENTHISTORY_FILENAME + 1, "%s", "None");
  rowOfFileEventHistory->fileStatus = FILESTATUS_NOTAPPLICABLE;
   
  fthFillAndSendSetObjSucc( setReq, fthContext);
}
/*  ################## FUNCTIONS SENDING RESPONSE  MESSAGES ######################## */

/*  %%%%%%%%%%%%%%%%%%%%%%%%% UPLOAD  MESSAGES %%%%%%%%%%%%%%%%%%%%%%%%%%%   */
/**
 * @param   fthContext   Context of FTH
 * @details This function creates a message MSG_FTH_BSM_UPLOAD_CMPL, fills the
 * parameters and sends the message to BSM
 */
void fthCreateAndSendUploadCmpl(
  IN FthContext *fthContext
  )
{
  MsgFthBsmUploadCmpInd uploadCmpl;
  clearBytes( &uploadCmpl, sizeof(MsgFthBsmUploadCmpInd));

  uploadCmpl.deleteAfterUpload = fthContext->fileUpload.deleteAfterUpload;
  snprintf( uploadCmpl.fileName, (strlen(fthContext->fileUpload.fileName)+1),"%s",
                                         fthContext->fileUpload.fileName );

  createAndSendMsg( MSG_FTH_BSM_UPLOAD_CMPL_IND, (UINT8*)&uploadCmpl, sizeof(MsgFthBsmUploadCmpInd) );

  /* Update fileEventHistory table */
  if( TRUE EQ fthContext->fileUpload.deleteAfterUpload )
  {
    fthUpdateFileEventHistory( fthContext->fileUpload.fileName,FILESTATUS_FILEUPLOADANDDELETE,TRUE );
  }
  else
  {
    fthUpdateFileEventHistory( fthContext->fileUpload.fileName,FILESTATUS_FILEUPLOAD,TRUE );
  }
}


/**
 * @param error   Error code
 * @details This function creates a message MSG_FTH_BSM_UPLOAD_FAIL, fills the
 * parameters and sends the message to BSM
 */
void fthCreateAndSendUploadFail(
  IN BsErrorValue   error
  )
{
  MsgFthBsmUploadFail     uploadFail;
  FthContext *fthContext = getFthContext();
  clearBytes( &uploadFail, sizeof(MsgFthBsmUploadFail));

  snprintf( uploadFail.fileName, (strlen(fthContext->fileUpload.fileName)+1),"%s",
                                        fthContext->fileUpload.fileName );
  uploadFail.bsErrorValue = error;

  createAndSendMsg( MSG_FTH_BSM_UPLOAD_FAIL, (UINT8*)&uploadFail, sizeof(MsgFthBsmUploadFail) );

  /* If file transfer is forcefully stopped then we don't update FileEventHistory table */
  if(FALSE EQ fthContext->isStopFileTransfer )
  {
    if(TRUE EQ fthContext->fileUpload.deleteAfterUpload)
    {
      fthUpdateFileEventHistory(fthContext->fileUpload.fileName,FILESTATUS_FILEUPLOADANDDELETE,FALSE);
    }
    else
    {
      fthUpdateFileEventHistory(fthContext->fileUpload.fileName,FILESTATUS_FILEUPLOAD,FALSE);
    }
  }
}

/*  %%%%%%%%%%%%%%%%%%%%%%%%% DOWNLOAD  MESSAGES %%%%%%%%%%%%%%%%%%%%%%%%%%%   */

/**
 * @param   fthContext   Context of FTH
 * @details This function creates a message MSG_FTH_BSM_DOWNLOAD_CMPL, fills the
 *          parameters and sends the message to BSM
 */
void fthCreateAndSendDwCmpl(
  IO FthContext    *fthContext
  )
{
  MsgFthBsmDownloadCmpInd dwCmpl;

  clearBytes( &dwCmpl, sizeof(MsgFthBsmDownloadCmpInd));

  snprintf( dwCmpl.fileName, strlen(fthContext->fileDownload.fileName)+1, "%s",
            fthContext->fileDownload.fileName );

  createAndSendMsg( MSG_FTH_BSM_DOWNLOAD_CMPL_IND, (UINT8*)&dwCmpl, sizeof(MsgFthBsmDownloadCmpInd) );
  fthUpdateFileEventHistory(fthContext->fileDownload.fileName,FILESTATUS_FILEDOWNLOAD,TRUE);
}

/**
 * @param error        Error code
 * @details This function creates a message MSG_FTH_BSM_DOWNLOAD_FAIL, fills the
 * parameters and sends the message to BSM.
 */
void fthCreateAndSendDwFail(
  IN BsErrorValue  error
  )
{
  MsgFthBsmDownloadFail dwFail;
  FthContext *fthContext = getFthContext();
  clearBytes( &dwFail, sizeof(MsgFthBsmDownloadFail));

  snprintf( dwFail.fileName, strlen(fthContext->fileDownload.fileName)+1, "%s",
            fthContext->fileDownload.fileName);

  dwFail.bsErrorValue = error;
  createAndSendMsg( MSG_FTH_BSM_DOWNLOAD_FAIL, (UINT8*)&dwFail, sizeof(MsgFthBsmDownloadFail) );

  if(FALSE EQ fthContext->isStopFileTransfer)
  {
    fthUpdateFileEventHistory(fthContext->fileDownload.fileName,FILESTATUS_FILEDOWNLOAD,FALSE);
  }
}

/**
 * @param setReq          SetObjReq
 * @param bsErrorValue    Error code
 * @param fthContext      FTH Context
 * @details This function creates a message MSG_FTH_BSM_SET_OBJ_FAIL, fills the
 * parameters and sends the message to BSM.
 */
void fthFillAndSendSetObjFail(
  IN SetObjReq          *setReq,
  IN BsErrorValue        bsErrorValue,
  IO FthContext         *fthContext
  )
{
  SetObjRspFail   rspFail;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes(&rspFail, sizeof(SetObjRspFail));

  /* If setObjReq is not passed then take the details from FthContext */
  if(NULL EQ setReq)
  {
    rspFail.tlvArray.numTlvs = fthContext->setObjReq.numTlvs;
    copyBytes(&(fthContext->setObjReq.cmdRef), sizeof(CmdRef), &(rspFail.cmdRef));
    copyBytes(&(fthContext->setObjReq.tlvInfo),sizeof(TlvInfo)*(fthContext->setObjReq.numTlvs),&(rspFail.tlvArray.tlvInfo));
    copyBytes(&(fthContext->setObjReq.tlvInfo[0].tag), sizeof(TagInfo), &(rspFail.setErrRsp.failedTagInfo));
  }
  else
  {
    copyBytes( &(setReq->cmdRef), sizeof(CmdRef), &(rspFail.cmdRef));
    copyBytes( &(setReq->tlvArray), sizeof(TlvArray), &(rspFail.tlvArray));
    copyBytes( &(setReq->tlvArray.tlvInfo[0].tag), sizeof(TagInfo), &(rspFail.setErrRsp.failedTagInfo));
  }

  rspFail.setErrRsp.errRspCode = bsErrorValue;

  /* encode the message */
  encSetObjRspFailMsg( &rspFail, &encBuf );
  createAndSendMsg( MSG_FTH_BSM_SET_OBJ_FAIL, (UINT8*)&encBuf.value, encBuf.len );
}

/**
 * @param setReq          SetObjReq
 * @param fthContext      FTH Context
 * @details This function creates a message MSG_FTH_BSM_SET_OBJ_SUCC, fills the
 * parameters and sends the message to BSM.
 */
void fthFillAndSendSetObjSucc(
  IN SetObjReq          *setReq,
  IO FthContext         *fthContext
  )
{
  SetObjRspSucc   rspSucc;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes(&rspSucc, sizeof(SetObjRspSucc));

  /* If setObjReq is not passed then take the details from FthContext */
  if(NULL EQ setReq)
  {
    rspSucc.tlvArray.numTlvs = fthContext->setObjReq.numTlvs;
    copyBytes(&(fthContext->setObjReq.cmdRef), sizeof(CmdRef), &(rspSucc.cmdRef));
    copyBytes(&(fthContext->setObjReq.tlvInfo),sizeof(TlvInfo)*(fthContext->setObjReq.numTlvs),&(rspSucc.tlvArray.tlvInfo));
  }
  else
  {
    copyBytes( &(setReq->cmdRef), sizeof(CmdRef), &(rspSucc.cmdRef));
    copyBytes( &(setReq->tlvArray), sizeof(TlvArray), &(rspSucc.tlvArray));
  }

  /* encode the message */
  encSetObjRspSuccMsg( &rspSucc, &encBuf );
  createAndSendMsg( MSG_FTH_BSM_SET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len );
}

void fthFillAndSendSetObjSuccRsp(
	IN SetObjReq          *setReq,
	IN TBOOL							isDelayed         
	)
{
	FthContext *fthContext = getFthContext();

	if (isDelayed EQ FALSE)
	{
		fthFillAndSendSetObjSucc(setReq,fthContext);
	}
	else
	{
		fthCreateAndSendDwCmpl(fthContext);
	}
}
	

void fthFillAndSendSetObjFailRsp(
		IN SetObjReq          *setReq,
		IN BsErrorValue       bsErrorValue,
		IN TBOOL              isDelayed
		)
{
	FthContext *fthContext= getFthContext();

	if (isDelayed EQ FALSE)
	{
		fthFillAndSendSetObjFail(setReq,bsErrorValue,fthContext);
	}
	else
	{
		fthCreateAndSendDwFail(bsErrorValue);
	}
}


void fthCopyTlvArrayInCtx(
	IN SetObjReq   *setReq,
	IO FthContext  *fthContext
	)
{
fthContext->setObjReq.numTlvs = setReq->tlvArray.numTlvs;
copyBytes(&(setReq->cmdRef),sizeof(CmdRef),&(fthContext->setObjReq.cmdRef));
copyBytes(&(setReq->tlvArray.tlvInfo),sizeof(TlvInfo)*(fthContext->setObjReq.numTlvs),&(fthContext->setObjReq.tlvInfo));
}

void fthProcFileDownCmdInIdleState(
	IO SetObjReq   *setReq,
	IN FileDownload fileDownload
		)
{
	FthContext *fthContext = getFthContext();
	SINT32 duration;

	switch(fileDownload.fileDownloadCommand)
	{
		case FILEDOWNLOADCOMMAND_INSTANTDOWNLOAD:
			{
				if(fthContext->isScheduled EQ FALSE)
				{
					fthCopyTlvArrayInCtx(setReq,fthContext);
					fthStartFileDownload(&fileDownload, fthContext);
				}
				else
				{
					fthFillAndSendSetObjFailRsp( setReq, BS_ERR_DOWNLOAD_ALREADY_SCHEDULED, FALSE );
				}
			}
			break;
		case FILEDOWNLOADCOMMAND_SCHEDULEADOWNLOAD:
			{
				if(fthContext->isScheduled EQ FALSE)
				{
					if (checkAndFillTimeDuration ((char *)fileDownload.scheduleDateAndTime,&duration))
					{
						fthCopyTlvArrayInCtx(setReq,fthContext);
						fthContext->fileDownload = fileDownload;
						fthStartTimer(TIMERNAME_WAITSCHEDULEDFILETRANSFERTOSTART,duration);
						fthContext->isScheduled = TRUE;
						fthUpdateFileDownloadStatusTable(FILEDOWNLOADCOMMANDRECEIVED_SCHEDULEADOWNLOAD,
								&(fileDownload.fileName[0]),
								DOWNLOADSTATUS_NOTINITIATED,
								0,
								&(fileDownload.scheduleDateAndTime[0]));
						fthFillAndSendSetObjSuccRsp(setReq, FALSE );
					}
					else
					{
						fthFillAndSendSetObjFailRsp( setReq, BS_ERR_INVALID_SCHEDULE_TIME, FALSE );
					}

				}
				else
					fthFillAndSendSetObjFailRsp( setReq, BS_ERR_DOWNLOAD_ALREADY_SCHEDULED, FALSE );
			}
			break;

		case FILEDOWNLOADCOMMAND_MODIFYSCHEDULEDDOWNLOAD:
			{
				if (fthContext->isScheduled EQ TRUE)
				{
					if (checkAndFillTimeDuration ((char *)fileDownload.scheduleDateAndTime,&duration) EQ TRUE)
					{
						fthStopTimer(TIMERNAME_WAITSCHEDULEDFILETRANSFERTOSTART);
						fthCopyTlvArrayInCtx(setReq,fthContext);
						fthContext->fileDownload = fileDownload;
						fthStartTimer(TIMERNAME_WAITSCHEDULEDFILETRANSFERTOSTART,duration);
						fthUpdateFileDownloadStatusTable(FILEDOWNLOADCOMMANDRECEIVED_MODIFYSCHEDULEDDOWNLOAD,
								&(fileDownload.fileName[0]),
								DOWNLOADSTATUS_NOTINITIATED,
								0,
								&(fileDownload.scheduleDateAndTime[0]));

						fthFillAndSendSetObjSuccRsp(setReq, FALSE );
					}
					else
					{
						fthFillAndSendSetObjFailRsp( setReq, BS_ERR_INVALID_SCHEDULE_TIME, FALSE );
					}
				}
				else
				{
					fthFillAndSendSetObjFailRsp( setReq, BS_ERR_NO_PRIOR_DOWNLOAD_SCHEDULED, FALSE );
				}
			}
			break;
		case FILEDOWNLOADCOMMAND_CANCELDOWNLOAD:
			{
				if(fthContext->isScheduled EQ TRUE)
				{
					fthStopTimer(TIMERNAME_WAITSCHEDULEDFILETRANSFERTOSTART );
					fthContext->isScheduled = FALSE;
					fthUpdateFileDownloadStatusTable(FILEDOWNLOADCOMMANDRECEIVED_CANCELDOWNLOAD,
							&(fthContext->fileDownload.fileName[0]),
							DOWNLOADSTATUS_CANCELLED,
							0,
							NULL);
					fthFillAndSendSetObjSuccRsp( setReq, FALSE);
				}
				else
				{
					fthFillAndSendSetObjFailRsp( setReq, BS_ERR_NO_ACTIVE_DOWNLOAD, FALSE );
				}
			}
			break;
		default:
			BS_ERROR_UNKNOWN_ACTION_RECD( fileDownload.fileDownloadCommand );
			fthFillAndSendSetObjFailRsp( setReq, BS_ERR_UNKNOWN_ACTION_RECD, FALSE );
			break;
	}
}

void fthProcFileDownCmdInDownloadInProgState(
	IO SetObjReq   *setReq,
	IN FileDownload fileDownload
		)
{
	FthContext *fthContext = getFthContext();
	switch(fileDownload.fileDownloadCommand)
	{
		case FILEDOWNLOADCOMMAND_INSTANTDOWNLOAD:
		case FILEDOWNLOADCOMMAND_SCHEDULEADOWNLOAD:
		case FILEDOWNLOADCOMMAND_MODIFYSCHEDULEDDOWNLOAD:
			{
				fthFillAndSendSetObjFailRsp( setReq, BS_ERR_DOWNLOAD_IN_PROGRESS, FALSE);
			}
			break;
		case FILEDOWNLOADCOMMAND_CANCELDOWNLOAD:
			{
				fthStopDownloadAndClearLocalFile( fthContext );
				fthContext->isScheduled = FALSE;
				fthFillAndSendSetObjSuccRsp( setReq, FALSE );
			}
			break;
		default:
			BS_ERROR_UNKNOWN_ACTION_RECD( fileDownload.fileDownloadCommand );
			fthFillAndSendSetObjFailRsp( setReq, BS_ERR_UNKNOWN_ACTION_RECD, FALSE );
			break;
	}
}

/**
 * @param setReq          SetObjReq
 * @param fthContext      FTH Context
 * @details This function triggers the FileDownload /FileDownloadCancel as requested in SET_OBJ_REQ.
 */
void fthPerformSetOnFileDownload(
		IO SetObjReq   *setReq,
		IO FthContext  *fthContext
		)
{
	FileDownload   fileDownload;
	//UINT8 allowedNoOfTLV;
	clearBytes( &fileDownload, sizeof(FileDownload) );

	GenericTable     *tableFunc = getTableFunctions(FILEDOWNLOAD);
	if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
	{
		tableFunc->getTlv2StructFunc( &setReq->tlvArray, &fileDownload );
	}

/*
	switch( fileDownload.fileDownloadCommand )
	{
		case  FILEDOWNLOADCOMMAND_INSTANTDOWNLOAD:
			{
				allowedNoOfTLV = MAX_COLS_IN_FILEDOWNLOAD_TABLE -2;
			}
			break;
		case  FILEDOWNLOADCOMMAND_SCHEDULEADOWNLOAD:
			{
				allowedNoOfTLV = MAX_COLS_IN_FILEDOWNLOAD_TABLE -1;
			}
			break;
		case  FILEDOWNLOADCOMMAND_MODIFYSCHEDULEDDOWNLOAD:
			{
				allowedNoOfTLV = MAX_COLS_IN_FILEDOWNLOAD_TABLE -1;
			}
			break;
		case FILEDOWNLOADCOMMAND_CANCELDOWNLOAD:
			{
				allowedNoOfTLV = 1;
			}
			break;
		default:
			BS_ERROR_UNKNOWN_ACTION_RECD( fileDownload.fileDownloadCommand );
			fthFillAndSendSetObjFailRsp( setReq, BS_ERR_UNKNOWN_ACTION_RECD, FALSE );
			break;
	}

	if( setReq->tlvArray.numTlvs < allowedNoOfTLV)
	{
	///Check all the TLV are present or not. Index is not necessary 
		fthFillAndSendSetObjFail( setReq, BS_ERR_ALL_PARAMETERS_NOT_SET_ON_REQ, fthContext );
		return;
	}
*/
	switch( fthContext->fthState )
	{
		case FTH_STATE_IDLE:
			{
				fthProcFileDownCmdInIdleState(setReq,fileDownload);
			}
			break;
		case FTH_STATE_DOWNLOAD_IN_PROG:
			{
				fthProcFileDownCmdInDownloadInProgState(setReq,fileDownload);
			}
			break;
		case FTH_STATE_UPLOAD_IN_PROG:
			{
				fthFillAndSendSetObjFail( setReq, BS_ERR_UPLOAD_IN_PROGRESS, FALSE);	
			}
			break;
		default:
			logError("Internal Error: No Such State:%d",fthContext->fthState);
			fthFillAndSendSetObjFail( setReq, BS_ERR_MSG_RCVD_IN_UNKNOWN_STATE, fthContext );
			break;
	}
}

/**
 * @param setReq          SetObjReq
 * @param fthContext      FTH Context
 * @details This function triggers the FileUpload /FileUploadCancel as requested in SET_OBJ_REQ.
 */
void fthPerformSetOnFileUpload(
  IO SetObjReq   *setReq,
  IO FthContext  *fthContext
  )
{
  FileUpload   fileUpload;

  clearBytes( &fileUpload, sizeof(FileUpload) );

  GenericTable     *tableFunc = getTableFunctions(FILEUPLOAD);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setReq->tlvArray, &fileUpload );
  }

  /* Check all the TLV are present or not. Index is not necessary */
  if(MAX_COLS_IN_FILEUPLOAD_TABLE -1 > setReq->tlvArray.numTlvs)
  {
    if(FILEUPLOADCOMMAND_UPLOADCANCEL NOTEQ fileUpload.fileUploadCommand)
    {
      fthFillAndSendSetObjFail( setReq, BS_ERR_SET_ON_SINGLE_PARAMETER_UNSUPPORTED, fthContext );
      return;
    }
  }

  switch( fileUpload.fileUploadCommand )
  {
    case FILEUPLOADCOMMAND_UPLOAD:
    {
      if(FTH_STATE_IDLE EQ fthContext->fthState)
      {
        /* Copy TlvArray in FthContext */
        fthContext->setObjReq.numTlvs = setReq->tlvArray.numTlvs;
        copyBytes(&(setReq->cmdRef),sizeof(CmdRef),&(fthContext->setObjReq.cmdRef));
        copyBytes(&(setReq->tlvArray.tlvInfo),sizeof(TlvInfo)*(fthContext->setObjReq.numTlvs),&(fthContext->setObjReq.tlvInfo));
        fthStartFileUpload(&fileUpload, fthContext);
      }
      else if(FTH_STATE_DOWNLOAD_IN_PROG EQ fthContext->fthState)
      {
        fthFillAndSendSetObjFail( setReq, BS_ERR_DOWNLOAD_IN_PROGRESS, fthContext );
      }
      else
      {
        fthFillAndSendSetObjFail( setReq, BS_ERR_UPLOAD_IN_PROGRESS, fthContext );
      }
    }
    break;
    case FILEUPLOADCOMMAND_UPLOADCANCEL:
    {
      if(FTH_STATE_UPLOAD_IN_PROG EQ fthContext->fthState)
      {
        fthStopUploadAndClearRemoteFile( fthContext );
        fthFillAndSendSetObjSucc( setReq, fthContext );
      }
      else
      {
        fthFillAndSendSetObjFail( setReq, BS_ERR_NO_ACTIVE_UPLOAD, fthContext );
      }
    }
    break;
    default:
      BS_ERROR_UNKNOWN_ACTION_RECD( fileUpload.fileUploadCommand );
      fthFillAndSendSetObjFail( setReq, BS_ERR_UNKNOWN_ACTION_RECD, fthContext );
      break;
    }

}

/************************HANDLING OF SET REQ IN ALL STATES ************************************/
/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process SET_OBJ_REQ message in All states.
 */
static inline void fthProcSetObjReqInAllState(
  IN Msg           *msg,
  IO FthContext    *fthContext
  )
{
  SetObjReq    setReq;
  TableId      tableId;
  clearBytes( &setReq, sizeof(SetObjReq));

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setReq );
  tableId = setReq.tlvArray.tlvInfo[0].tag.tableId;
  switch( tableId )
  {
    case FILEDOWNLOAD:
      fthPerformSetOnFileDownload(&setReq, fthContext);
      break;
    case FILEUPLOAD:
      fthPerformSetOnFileUpload(&setReq, fthContext);
      break;
    case ENBADMIN:
      fthPerformSetOnEnbAdmin(&setReq, fthContext);
      break;
  }
}

/************************HANDLING OF GET STATUS REQ IN ALL STATES ************************************/
/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process GET_OBJ_REQ message in All states.
 */
static inline void fthProcGetObjReqInAllState(
  IN Msg           *msg,
  IO FthContext    *fthContext
  )
{
  fthGetObjReq( msg, fthContext);
}

/**
 * @details This function creates a message MSG_FTH_PRM_HELLO_ACK, and sends to PRM
 */
void fthCreateAndSendHelloAck(
  IN Msg    *msg
  )
{
  MsgPrmModHello     *hello = (MsgPrmModHello*)msg->data;
  MsgModPrmHelloAck   helloAck;
  clearBytes( &helloAck, sizeof(MsgModPrmHelloAck) );
  helloAck.helloAckSeqNum = hello->helloSeqNum;

  createAndSendMsg( MSG_FTH_PRM_HELLO_ACK, (UINT8*)&helloAck, sizeof(MsgModPrmHelloAck) );
}

/**
 * @details DESCRIPTION
 * This function processes the received Shutdown message
 * This function sends Shutdown Complete, and Exits.
 */
static void fthExit()
{
  modExit(MODID_FTH, &(getModuleContext()->exeName[0]));
  exit(EXIT_SUCCESS);
}

/*  ################## MESSAGE HANDLER FUNCTIONS ######################## */
/************************HANDLING OF HELLO REQ IN ALL STATES ************************************/
/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process MSG_PRM_FTH_HELLO message in all states.
 */
static inline void fthProcHelloReqInAllStates(
  IN Msg           *msg,
  IO FthContext    *fthContext
  )
{
  fthCreateAndSendHelloAck( msg );
  UNUSED(fthContext);
}
/************************HANDLING OF EXIT REQ IN ALL STATES ************************************/
/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process MSG_PRM_FTH_EXIT_CMD message in IDLE state.
 */
static inline void fthProcExitReqInIdleState(
  IN Msg           *msg,
  IO FthContext    *fthContext
  )
{
  UNUSED(msg);
  UNUSED(fthContext);
  fthExit();
}

/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process MSG_PRM_FTH_EXIT_CMD message in DOWNLOAD_IN_PROGRESS state.
 */
static inline void fthProcExitReqInDwProgState(
  IN Msg           *msg,
  IO FthContext    *fthContext
 )
{
  UNUSED(msg);
  fthStopDownloadAndClearLocalFile( fthContext );
  BS_ERROR_MODULE_STOPPED();
  fthCreateAndSendDwFail( BS_ERR_MODULE_STOPPED);
  fthExit();
}

/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process MSG_PRM_FTH_EXIT_CMD message in UPLOAD_IN_PROGRESS state.
 */
static inline void fthProcExitReqInUploadProgState(
  IN Msg           *msg,
  IO FthContext    *fthContext
  )
{
  UNUSED(msg);
  fthStopUploadAndClearRemoteFile( fthContext );
  BS_ERROR_MODULE_STOPPED();
  fthCreateAndSendUploadFail( BS_ERR_MODULE_STOPPED);
  fthExit();
}

/************************HANDLING OF TRACE CFG IN ALL STATES ************************************/

/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process SET_MSG_TRACE message in All states.
 */
static inline void fthProcSetTraceCfgInAllStates(
  IN Msg           *msg,
  IO FthContext    *fthContext
  )
{
  MsgBsmFthSetMsgTrace   *msgTrace = (MsgBsmFthSetMsgTrace*)(msg->data);
  MsgDesc                *msgDesc  = getMsgDesc(msgTrace->msgId);

  UNUSED(fthContext);
  /* Set the values in the table */
  msgDesc->msgTraceLevel     = msgTrace->msgTraceLevel;
  msgDesc->msgTraceDirection = msgTrace->msgTraceDirection;
}

/************************HANDLING OF LOG CFG IN ALL STATES ************************************/
/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process SET_LOG_CFG message in All states.
 */
static inline void fthProcSetLogCfgInAllStates(
  IN Msg           *msg,
  IO FthContext    *fthContext
  )
{
  MsgBsmFthSetLogCfg       *msgSetLog = (MsgBsmFthSetLogCfg*)(msg->data);
  LogContext  *logContext  = getLogContext();

  UNUSED(fthContext);
  copyBytes(msgSetLog, sizeof(LogConfig), &(logContext->logConfig));
  logInfo("%s", "Loglevel is set to new values ");
}

/**
 * @details This function init FTH state machine
 */
void initFthStateMachine(void)
{
  int i,j;

  /* Initialize the Function pointer to process Invalid Message
   * Then fill the valid processing functions
   */
  for(i=0; i< FTH_MAX_STATE; i++)
  {
    for(j=0; j<MSG_ID_MAX; j++ )
    {
      fthStateMachine[i][j].fp = NULL;
    }
  }
  InitFthStateMachine( FTH_STATE_IDLE,             MSG_PRM_FTH_HELLO, fthProcHelloReqInAllStates, FTH_STATE_IDLE );
  InitFthStateMachine( FTH_STATE_DOWNLOAD_IN_PROG, MSG_PRM_FTH_HELLO, fthProcHelloReqInAllStates, FTH_STATE_DOWNLOAD_IN_PROG );
  InitFthStateMachine( FTH_STATE_UPLOAD_IN_PROG,   MSG_PRM_FTH_HELLO, fthProcHelloReqInAllStates, FTH_STATE_UPLOAD_IN_PROG );

  InitFthStateMachine( FTH_STATE_IDLE,             MSG_PRM_FTH_EXIT_CMD, fthProcExitReqInIdleState,       FTH_STATE_IDLE );
  InitFthStateMachine( FTH_STATE_DOWNLOAD_IN_PROG, MSG_PRM_FTH_EXIT_CMD, fthProcExitReqInDwProgState,     FTH_STATE_IDLE );
  InitFthStateMachine( FTH_STATE_UPLOAD_IN_PROG,   MSG_PRM_FTH_EXIT_CMD, fthProcExitReqInUploadProgState, FTH_STATE_IDLE );

  InitFthStateMachine( FTH_STATE_IDLE,             MSG_BSM_FTH_SET_MSG_TRACE, fthProcSetTraceCfgInAllStates, FTH_STATE_IDLE );
  InitFthStateMachine( FTH_STATE_DOWNLOAD_IN_PROG, MSG_BSM_FTH_SET_MSG_TRACE, fthProcSetTraceCfgInAllStates, FTH_STATE_DOWNLOAD_IN_PROG );
  InitFthStateMachine( FTH_STATE_UPLOAD_IN_PROG,   MSG_BSM_FTH_SET_MSG_TRACE, fthProcSetTraceCfgInAllStates, FTH_STATE_UPLOAD_IN_PROG );

  InitFthStateMachine( FTH_STATE_IDLE,             MSG_BSM_FTH_SET_LOG_CFG, fthProcSetLogCfgInAllStates, FTH_STATE_IDLE );
  InitFthStateMachine( FTH_STATE_DOWNLOAD_IN_PROG, MSG_BSM_FTH_SET_LOG_CFG, fthProcSetLogCfgInAllStates, FTH_STATE_DOWNLOAD_IN_PROG );
  InitFthStateMachine( FTH_STATE_UPLOAD_IN_PROG,   MSG_BSM_FTH_SET_LOG_CFG, fthProcSetLogCfgInAllStates, FTH_STATE_UPLOAD_IN_PROG );

  /* LibCURL is run to completion after we exit this function download must be complete, so we should be in IDLE */
  InitFthStateMachine( FTH_STATE_IDLE,             MSG_BSM_FTH_GET_OBJ_REQ, fthProcGetObjReqInAllState,     FTH_STATE_IDLE );
  InitFthStateMachine( FTH_STATE_DOWNLOAD_IN_PROG, MSG_BSM_FTH_GET_OBJ_REQ, fthProcGetObjReqInAllState,     FTH_STATE_DOWNLOAD_IN_PROG );
  InitFthStateMachine( FTH_STATE_UPLOAD_IN_PROG,   MSG_BSM_FTH_GET_OBJ_REQ, fthProcGetObjReqInAllState,     FTH_STATE_UPLOAD_IN_PROG );

  InitFthStateMachine( FTH_STATE_IDLE,             MSG_BSM_FTH_SET_OBJ_REQ, fthProcSetObjReqInAllState,     FTH_STATE_IDLE );
  InitFthStateMachine( FTH_STATE_DOWNLOAD_IN_PROG, MSG_BSM_FTH_SET_OBJ_REQ, fthProcSetObjReqInAllState,     FTH_STATE_DOWNLOAD_IN_PROG );
  InitFthStateMachine( FTH_STATE_UPLOAD_IN_PROG,   MSG_BSM_FTH_SET_OBJ_REQ, fthProcSetObjReqInAllState,     FTH_STATE_UPLOAD_IN_PROG );

	InitFthStateMachine( FTH_STATE_IDLE,             MSG_TIMEKEEPER_FTH_TIMEOUT_IND, fthProcTimerExpiryInAllStates,     FTH_STATE_IDLE );
	InitFthStateMachine( FTH_STATE_DOWNLOAD_IN_PROG, MSG_TIMEKEEPER_FTH_TIMEOUT_IND, fthProcTimerExpiryInAllStates,     FTH_STATE_DOWNLOAD_IN_PROG );
	InitFthStateMachine( FTH_STATE_UPLOAD_IN_PROG,   MSG_TIMEKEEPER_FTH_TIMEOUT_IND, fthProcTimerExpiryInAllStates,     FTH_STATE_UPLOAD_IN_PROG );
}

/**
 * @details This function reads messages in queue and process them.
 */
void fthReadAndProcessRecvMsg(
  Msg         *msgIn
  )
{
  FthContext *fthContext = getFthContext();
  FthState    fthPrevState;

  logInfo("Received Message=%s, in state=%s", getMsgName(msgIn->msgId), getFthProceduralStateName(fthContext->fthState) );
  if( NULL NOTEQ fthStateMachine[fthContext->fthState][msgIn->msgId].fp )
  {
    /* As FTH changes the state while processing a request beacuse of LibCURL dependency
     * We save the previous state and move to next state as per the previous state in the init table
     */
    fthPrevState = fthContext->fthState;
    fthStateMachine[fthContext->fthState][msgIn->msgId].fp( msgIn, fthContext);
    fthContext->fthState = fthStateMachine[fthPrevState][msgIn->msgId].nextState;
  }
  else
  {
    BS_ERROR_NO_PROCESSING_FUNCTION_DEFINED( getMsgName(msgIn->msgId), getFthProceduralStateName(fthContext->fthState));
  }

  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  /* Free the received Message */
  freeMsg(msgIn);
}

