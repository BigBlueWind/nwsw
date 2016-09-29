/**
 * @file    bsm_download_upload.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions to process messages related to file download feature.
 *
 * @author:  Ankur Agrawal
 * @date:    23-Sep-2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param filename      uploaded fileName
 * @brief This function checks whether given file is a PM logs file or not.
 */
static TBOOL isFileUploadedIsPmLogFile(
  IO UINT8  *filename
  )
{
  UINT8 *word = NULL;
  word = strtok( filename, "/" );
  while ( word != NULL )
  {
    if ( 0  EQ strcmp(word, "pmlogs"))
    {
      return TRUE;
    }
    word = strtok( NULL, "/" );
  }
  return FALSE;
}

/**
 * @param   msg        Message from FTH that download is complete
 * @param   bsmContext BSM Context
 * @details This function processes file download complete message from FTH in all states,
 *          and send the notification to SIH.
 */
inline BsmProceduralState bsmProcSwUploadCmplInAllstates(
  IN Msg        *msg,
  IO BsmContext *bsmContext
  )
{
  MsgFthBsmUploadCmpInd  *ind = (MsgFthBsmUploadCmpInd*)msg->data;
  UINT8   rcvdFileName[MAX_SIZE_FILEUPLOAD_FILENAME + 1 ];
  /* Send File upload completed Event to SIH */
  sendEventFileUploadCompleted( ind->fileName );

  snprintf( rcvdFileName, strlen(ind->fileName)+1, "%s", ind->fileName );
  /* Check if the uploaded file was PM Log file, if yes, send an indication to PMH Module */
  if ( TRUE EQ isFileUploadedIsPmLogFile( rcvdFileName ) && (TRUE EQ ind->deleteAfterUpload))
  {
    /* send indication to PMH Module */
    MsgBsmPmhPmFileUploaded  fileUploadInd;
    clearBytes( &fileUploadInd, sizeof(MsgBsmPmhPmFileUploaded) );
    snprintf( fileUploadInd.pmLogFileName, strlen(ind->fileName)+1 , "%s", ind->fileName );
    createAndSendMsg( MSG_BSM_PMH_PM_FILE_UPLOADED, (UINT8*)&fileUploadInd, sizeof(MsgBsmPmhPmFileUploaded) );
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg   Message from FTH that file download failed
 * @param   bsmContext BSM Context
 * @details This function processes file download fail from FTH in All state,
 *          and sends notification to SIH.
 */
inline BsmProceduralState bsmProcSwUploadFailInAllStates(
    IN Msg        *msg,
    IO BsmContext *bsmContext
    )
{
  MsgFthBsmUploadFail  *ind = (MsgFthBsmUploadFail*)msg->data;
  /* Send File upload completed Event to SIH */

  sendEventFileUploadFailed( ind->fileName, errorCodeStr(ind->bsErrorValue) );

  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message from FTH that download is complete
 * @param   bsmContext BSM Context
 * @details This function processes file download complete message from FTH in all states,
 *          and send the indication to SIH.
 */
inline BsmProceduralState bsmProcSwDownloadCmplInAllstates(
    IN Msg        *msg,
    IO BsmContext *bsmContext
    )
{
  MsgFthBsmDownloadCmpInd  *ind = (MsgFthBsmDownloadCmpInd*)msg->data;
  /* Send File download completed Event to SIH */
  sendEventFileDownloadCompleted( ind->fileName );
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg   Message from FTH that file download failed
 * @param   bsmContext BSM Context
 * @details This function processes file download fail from FTH in All state,
 *          and send the indication to SIH.
 */
inline BsmProceduralState bsmProcSwDownloadFailInAllStates(
    IN Msg        *msg,
    IO BsmContext *bsmContext
    )
{
  MsgFthBsmDownloadFail  *ind = (MsgFthBsmDownloadFail*)msg->data;
  /* Send File upload completed Event to SIH */

  sendEventFileDownloadFailed( ind->fileName, errorCodeStr(ind->bsErrorValue) );
  return bsmContext->bsmProceduralState;
}
