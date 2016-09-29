/**
 * @file    fth_cmn.c
 * @brief This file is contains the code common across all features of FTH
 *          It also functions will calls config reading function.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         Ankur Agrawal
 * @date           01-Oct-2010
 */
#include "fth.h"

/**
 * @param   configFilePath   Path for config dir.
 * @details This function will read the fth.cfg file and store the details in fthContext->fthCfg.
 * */
TBOOL readFthCfgTable(
  IN UINT8    *configFilePath
  )
{
  FthContext *fthContext = getFthContext();
  UINT8       isError;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];

  /* Read the default configurations from the fth.cfg file and store it in
   * the FTH context FthCfgTable */
  memset( &fullFilePath, 0, sizeof(fullFilePath));
  constructFullPath ( configFilePath, "fth.cfg", fullFilePath );

  readFthConfigTable( fullFilePath, &fthContext->fthCfg, &isError );
  if ( TRUE EQ isError)
  {
    BS_ERROR_CONFIG_READ_FAILED()
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   rootDir          Path for FTH to run PICOBS_HOME/rel/(inst_A or inst_B)
 * @param   downloadDirPath  Path where downloads are kept PICOBS_HOME/download
 * @details This function will append the download dirPath to picoBsPath.
 * */
void fthUpdateDownloadDir(
  IN UINT8   *rootDir,
  IO UINT8   *downloadDirPath
  )
{
  UINT8 *word = NULL;
  UINT8 index = 0;
  UINT8 *delim   = "/";

  String50 temp[20];
  clearBytes(temp,sizeof(String50)* 20);
  logInfo("rootDir = %s",downloadDirPath);
  word = strtok((char *)rootDir, delim);
  logInfo("rootDir = %s",word);
  if(word NOTEQ NULL)
  {
    strcpy(temp[index++],word);
  }
  word = strtok(NULL, delim);
  while( NULL NOTEQ word)
  {
    strcpy(temp[index++],word);
    word = strtok(NULL,delim);
  }

  *temp[index]='\0';
  int i = 0;
  strcat( downloadDirPath, "/" );
  while( i <  index)
  {
    if ( 0 EQ strcmp(temp[i], "rel"))
    {
      strcat( downloadDirPath, "/download" );
      return;
    }
    else
    {
      strcat( downloadDirPath, temp[i] );
      strcat( downloadDirPath, "/" );
    }
    i++;
  }
}

/**
 * @param   fthContext  FTH Context
 * @details This function will initialise the FileDownlaodStatus, FileUploadStatus, FileEventHistory tables.
 * */
void fthInitStatusTables(
  IO FthContext   *fthContext
  )
{
  UINT8 i;
  FileDownloadStatus  *rowOfFileDownloadStatus  = (FileDownloadStatus*)getFirstRowOfTable(FILEDOWNLOADSTATUS);
  FileUploadStatus    *rowOfFileUplaodStatus    = (FileUploadStatus*)getFirstRowOfTable(FILEUPLOADSTATUS);
  FileEventHistory    *rowOfFileEventHistory    = (FileEventHistory*)getFirstRowOfTable(FILEEVENTHISTORY);

  /* Initialize the FileDownloadStatus table */
  snprintf( (UINT8*)&rowOfFileDownloadStatus->fileName, MAX_SIZE_FILEDOWNLOADSTATUS_FILENAME + 1, "%s", "None");
  rowOfFileDownloadStatus->fileDownloadCommandReceived = FILEDOWNLOADCOMMANDRECEIVED_NONE;
  rowOfFileDownloadStatus->downloadStatus = DOWNLOADSTATUS_NOTINITIATED;
  rowOfFileDownloadStatus->percentageComplete = 0;

  /* Initialize the FileUploadStatus table */
  snprintf( (UINT8*)&rowOfFileUplaodStatus->fileName, MAX_SIZE_FILEUPLOADSTATUS_FILENAME + 1, "%s", "None");
  rowOfFileUplaodStatus->fileUploadCommandReceived = FILEUPLOADCOMMANDRECEIVED_NONE;
  rowOfFileUplaodStatus->uploadStatus = UPLOADSTATUS_NOTINITIATED;
  rowOfFileUplaodStatus->percentageComplete = 0;

  /* Initialise the FileStatus Table */
  fthContext->indexFileStatusTable =0;
  fthContext->noOfEntriesInFileEventHistory =0;

  for ( i=0; i < MAX_ROWS_IN_FILEEVENTHISTORY_TABLE; i++ )
  {
    snprintf( (UINT8*)&rowOfFileEventHistory->fileName, MAX_SIZE_FILEEVENTHISTORY_FILENAME + 1, "%s", "None");
    rowOfFileEventHistory->fileStatus = FILESTATUS_NOTAPPLICABLE;
    getNextRowOfTable(rowOfFileEventHistory);
  }

}
/**
 * @param   state   FTH state enum value
 * @details This function returns the state name in string format
 **/
char *getFthProceduralStateName(
  IN FthState state
  )
{
  switch( state )
  {
    case FTH_STATE_IDLE:             return "FTH_STATE_IDLE";
    case FTH_STATE_DOWNLOAD_IN_PROG: return "FTH_STATE_DOWNLOAD_IN_PROG";
    case FTH_STATE_UPLOAD_IN_PROG:   return "FTH_STATE_UPLOAD_IN_PROG";
    default:
      BS_ERROR_UNKNOWN_ENUM_VALUE("No name defined for this FTH state ", state );
      break;
  }
  return "No name defined for this FTH state";
}
