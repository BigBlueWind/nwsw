/**
 * @file    fth_download_upload.c
 * @brief This file holds the code for getting data from FTP server using FTP|SCP|SFTP Protocol.
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * Steps to use this file
 * First call createFtpCurl to create the FTP url in the CURL format
 * then call getFileFromFtp to get the file from remote server
 *
 * @author  Lakshmi Narayana Madala / Ankur Agrawal
 * @date    15-Jul-2010
 */

#include "fth.h"

/**
 * @param index              Index of fileDownloadStatus table
 * @param commandReceived    Command received DOWNLOAD/DOWNLOADCNCL
 * @param fileName           FileName which is requested for download
 * @param downloadStatus     Status of download whether it is failed or successful
 * @param percentageComplete Download percent completed
 * @details This function updates FileDownloadStatus table.
 */
void fthUpdateFileDownloadStatusTable(
  IN FileDownloadCommandReceived commandReceived,
  IN UINT8                       *fileName,
  IN DownloadStatus              downloadStatus,
  IN UINT32                      percentageComplete,
	IN UINT8                       *scheduleDateAndTime
  )
{
  FileDownloadStatus  *rowOfFileDownloadStatus = (FileDownloadStatus*)getFirstRowOfTable(FILEDOWNLOADSTATUS);
  rowOfFileDownloadStatus->index = 1 ;
  rowOfFileDownloadStatus->fileDownloadCommandReceived = commandReceived ;
  snprintf( (UINT8*)&rowOfFileDownloadStatus->fileName, MAX_SIZE_FILEDOWNLOADSTATUS_FILENAME + 1, "%s", fileName);
  rowOfFileDownloadStatus->downloadStatus = downloadStatus;
  rowOfFileDownloadStatus->percentageComplete = percentageComplete;
	if (scheduleDateAndTime EQ NULL)
	{
		clearBytes(rowOfFileDownloadStatus->scheduledDownloadStartTime,sizeof(UINT8));
	}
	else
	{
		snprintf( (UINT8*)&rowOfFileDownloadStatus->scheduledDownloadStartTime, MAX_SIZE_FILEDOWNLOADSTATUS_SCHEDULEDDOWNLOADSTARTTIME + 1, "%s", scheduleDateAndTime);
	}
}

/**
 * @param index              Index of fileUploadStatus table
 * @param commandReceived    Command received UPLOAD/UPLOADCNCL
 * @param fileName           FileName which is requested for upload
 * @param uploadStatus       Status of upload whether it is failed or successful
 * @param percentageComplete Upload percent completed
 * @details This function updates fileUploadStatus table.
 */
static void fthUpdateFileUploadStatusTable(
  IN FileUploadCommandReceived   commandReceived,
  IN UINT8                       *fileName,
  IN UploadStatus                uploadStatus,
  IN UINT32                      percentageComplete
  )
{
  FileUploadStatus  *rowOfFileUploadStatus = (FileUploadStatus*)getFirstRowOfTable(FILEUPLOADSTATUS);
  rowOfFileUploadStatus->index = 1 ;
  rowOfFileUploadStatus->fileUploadCommandReceived = commandReceived ;
  snprintf( (UINT8*)&rowOfFileUploadStatus->fileName, MAX_SIZE_FILEUPLOADSTATUS_FILENAME + 1, "%s", fileName);
  rowOfFileUploadStatus->uploadStatus = uploadStatus;
  rowOfFileUploadStatus->percentageComplete = percentageComplete;
}

/**
 * @param speed  Speed of download
 * @details This function convert enum to exact value of MAX_DOWNLOAD_SPEED.
 */
static UINT32 fthDownloadSpeedFromEnum(
  IN MaxDownloadSpeed speed
  )
{
  switch(speed)
  {
    case MAXDOWNLOADSPEED_50KBPS:    return (        50 * 1024);
    case MAXDOWNLOADSPEED_100KBPS:   return (       100 * 1024);
    case MAXDOWNLOADSPEED_500KBPS:   return (       500 * 1024);
    case MAXDOWNLOADSPEED_1MBPS:     return (  1 * 1024 * 1024);
    case MAXDOWNLOADSPEED_10MBPS:    return ( 10 * 1024 * 1024);
    case MAXDOWNLOADSPEED_UNLIMITED: return (100 * 1024 * 1024);
    default:
      BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown value received for MaxDownloadSpeed ENUM", speed );
      return (100 * 1024 * 1024);
  }
}

/**
 * @param speed  Speed of upload
 * @details This function convert enum to exact value of MAX_UPLOAD_SPEED.
 */
static UINT32 fthUploadSpeedFromEnum(
  IN MaxUploadSpeed speed
  )
{
  switch(speed)
  {
    case MAXUPLOADSPEED_50KBPS:     return (        50 * 1024);
    case MAXUPLOADSPEED_100KBPS:    return (       100 * 1024);
    case MAXUPLOADSPEED_500KBPS:    return (       500 * 1024);
    case MAXUPLOADSPEED_1MBPS:      return (  1 * 1024 * 1024);
    case MAXUPLOADSPEED_10MBPS:     return ( 10 * 1024 * 1024);
    case MAXUPLOADSPEED_UNLIMITED:  return (100 * 1024 * 1024);
    default:
      BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown value received for MaxUploadSpeed ENUM", speed );
      return (100 * 1024 * 1024);
  }
}

/**
 * @param dirSize     size of directory
 * @param fthContext  FTH Context
 * @details This function returns download directory size.
 */
static TBOOL fthGetDownloadDirSize(
  IO UINT32       *dirSize,
  IO FthContext   *fthContext
  )
{
  String150 str;
  FILE *fp;
  DIR  *dir = NULL;

  /* Open download directory if not opened successfully then not having access or folder not present */
  dir = opendir(fthContext->fthCfg.downloadDir);
  if( NULL EQ dir )
  {
    BS_ERROR_FILE_ACCESS_PERMISSION_DENIED(fthContext->fthCfg.downloadDir );
    fthContext->bsError = BS_ERR_FILE_ACCESS;
    return FALSE;
  }
  clearBytes(str, sizeof(String150));
  /* Find size of directory and write in file pwav.txt */
  snprintf(str, sizeof(String150), "du -bs %s |cut -f1>>pwav.txt",fthContext->fthCfg.downloadDir);
  if(0 NOTEQ system(str))
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);

  /* Read pwav.txt and finds size of directory and save in variable dirSize*/
  fp = fopen("pwav.txt","r");
  fscanf( fp,"%s",str);
  fclose(fp);

  *dirSize = atoi(str);
  /* Removed intermediate created file pwav.txt */
  if(0 NOTEQ system("rm pwav.txt"))
    BS_ERROR_SYSTEM_COMMAND_FAILED("rm pwav.txt");

  return TRUE;
}

/**
 * @param fthContext  FTH context
 * @details This function checks /picobs/download size and checks whether new file can accommodate or not.
 */
static TBOOL fthDoesSpaceAvailableInDownloadDir(
  IO FthContext    *fthContext
  )
{
  UINT32 downloadDirSize;

  if( FALSE EQ fthGetDownloadDirSize( &downloadDirSize, fthContext ) )
  {
    return FALSE;
  }
  /* Compare download directory size with max allowed size */
  if(downloadDirSize > fthContext->fthCfg.maxDownloadDirSize )
  {
    BS_ERROR_DIRECTORY_SIZE_EXCEEDED(fthContext->fthCfg.maxDownloadDirSize, downloadDirSize  );
    fthContext->bsError = BS_ERR_NO_DISK_SPACE;
		fthUpdateFileDownloadStatusTable(fthContext->fileDownload.fileDownloadCommand,
				&(fthContext->fileDownload.fileName[0]),
				DOWNLOADSTATUS_FAILED,
				0,
				NULL);
    return FALSE;
  }
  logInfo("downloadDirSize =%d KB < maxDownloadDirSize in KBytes=%d KB", (downloadDirSize/1024), (fthContext->fthCfg.maxDownloadDirSize/1024) );
  return TRUE;
}

/**
 * @param fileSize    size of file which is requested to download
 * @param fthContext  FTH Context
 * @details This function checkes /picobs/download size and checks whether given file can accommodate or not.
 */
static TBOOL fthIsFileAccommodationPossible(
  IN double        fileSize,
  IO FthContext    *fthContext
  )
{
  UINT32     downloadDirSize;
  UINT32     dirSizeAfterDownload;
  TBOOL      isAccomodationPossible = TRUE;
  UINT32     maxAllowedSizeOfDownloadDirectory = fthContext->fthCfg.maxDownloadDirSize;

  /* This variable is set only in this function if the file size limit exceeded
   * Then this file cannot be accommodated */
  if( TRUE EQ fthContext->isFileSizeExceed )
  {
    return FALSE;
  }
  if( FALSE EQ fthGetDownloadDirSize( &downloadDirSize, fthContext ) )
  {
    isAccomodationPossible = FALSE;
  }

  /* check what will be size if download after completion of download
   * if dirSizeAfterDownload exceeds download directory size limit then log the error */
  dirSizeAfterDownload = downloadDirSize + fileSize;
  if( dirSizeAfterDownload > maxAllowedSizeOfDownloadDirectory )
  {
    BS_ERROR_DIRECTORY_SIZE_EXCEEDED( maxAllowedSizeOfDownloadDirectory, dirSizeAfterDownload  );
    fthContext->bsError = BS_ERR_NO_DISK_SPACE;
    isAccomodationPossible = FALSE;
  }

  /* If any one of the check fails then send reject and update downloadStatusTable */
  if(FALSE EQ isAccomodationPossible )
  {
		fthFillAndSendSetObjFailRsp( NULL, BS_ERR_FILESIZE_EXCEEDED, fthContext->isScheduled );
		fthContext->isScheduled = FALSE;
		fthContext->isFileSizeExceed = TRUE;

    /* Update the status table */
		fthUpdateFileDownloadStatusTable(fthContext->fileDownload.fileDownloadCommand,
				&(fthContext->fileDownload.fileName[0]),
				DOWNLOADSTATUS_FAILED,
				0,
				NULL);
	}

  return isAccomodationPossible;
}

/**
 * @param filename  file name.
 * @details This function checks whether requested file is exists or not.
 */
static TBOOL fthDoesFileExist(
  IN UINT8 *filename
  )
{
  FILE *fp=NULL;

  /* open file and if we can open it successfully then it means that file is present */
  fp= fopen( filename, "r");
  if(NULL EQ fp)
  {
    return FALSE;
  }
  else
  {
    fclose(fp);
  }
  return TRUE;
}
/*  ################## INTERNAL FUNCTIONS ######################## */
/**
 * @param fthContext  FTH context
 * @details This function initialize FTH context which are necessary to transfer a file.
 */
static void fthInitContext(
  IO FthContext   *fthContext
  )
{
  /* Before starting download/upload Initialise FthContext */
  fthContext->isFileSizeChecked  = FALSE;
  fthContext->isSuccSent         = FALSE;
  fthContext->fthState           = FTH_STATE_IDLE;
  fthContext->isFileSizeExceed   = FALSE;
  fthContext->isStopFileTransfer = FALSE;
  fthContext->bsError            = BS_NO_ERROR;
  fthContext->ulFileSize         = 0;
  fthContext->dlFileSize         = 0;
}

/**
 * @param loginId     The name of user account on FTP server
 * @param passwd      The password of that user account on FTP server
 * @param serverName  server-name
 * @param rmtFilePath Fully Qualified(Absolute) File name with path "directory/file"
 * @param protoToUse  protocol which is used to transfer the file
 * @param ftpUrl      he CURL is copied into this buffer, format of CURL is as follows protocol://username:passwd@\ipAddress/filepath
 * @details This function creates a CURL by concatenating all the supplied user fields.
 */
static void fthCreateUrl(
  IN UINT8         *loginId,
  IN UINT8         *passwd,
  IN UINT8         *serverName,
  IN UINT8         *rmtFilePath,
  IN ProtocolUsed   protoToUse,
  IO UINT8         *ftpUrl
  )
{
  UINT16   ftpurlIndex = 0;
  UINT16   lenOfStr;

  memset( ftpUrl, 0, LEN_OF_FTP_URL );

  /* Copy ftp:// */
  switch( protoToUse )
  {
    case PROTOCOLUSED_SFTP:
      lenOfStr = strlen("sftp://");
      copyBytes( "sftp://", lenOfStr, (ftpUrl + ftpurlIndex) ); ftpurlIndex += lenOfStr;
      break;
    case PROTOCOLUSED_SCP:
      lenOfStr = strlen("scp://");
      copyBytes( "scp://", lenOfStr, (ftpUrl + ftpurlIndex) ); ftpurlIndex += lenOfStr;
      break;
    case PROTOCOLUSED_FTP:
    default:
      lenOfStr = strlen("ftp://");
      copyBytes( "ftp://", lenOfStr, (ftpUrl + ftpurlIndex) ); ftpurlIndex += lenOfStr;
      break;
  }

  /* If User name is present then,
   * Concatenate user name and password as username:password@ */
  lenOfStr = strlen(loginId);
  if( lenOfStr NOTEQ 0  )
  {
    copyBytes( loginId, lenOfStr, (ftpUrl + ftpurlIndex) ); ftpurlIndex += lenOfStr;
    ftpUrl[ftpurlIndex] = ':'; ftpurlIndex++;

    lenOfStr = strlen(passwd);
    copyBytes( passwd, lenOfStr, (ftpUrl + ftpurlIndex) );  ftpurlIndex += lenOfStr;
    ftpUrl[ftpurlIndex] = '@'; ftpurlIndex++;
  }

  /* We are done till ftp://user:passwd@, now get the fileServer */
  lenOfStr = strlen(serverName);
  copyBytes( serverName, lenOfStr, (ftpUrl + ftpurlIndex) ); ftpurlIndex += lenOfStr;
  ftpUrl[ftpurlIndex] = '/'; ftpurlIndex++;

  /* We are done till ftp://user:passwd@server/, now get the filenamepath */
  lenOfStr = strlen(rmtFilePath);
  copyBytes( rmtFilePath, lenOfStr, (ftpUrl + ftpurlIndex) ); ftpurlIndex += lenOfStr;

  logInfo( "CURL=%s", ftpUrl);
}

/**
 * @param   fthContext  FTH context
 * @details This function invokes the function fthReadAndProcessRecvMsg, And returns
 *          FLAG isStopFileTransfer.
 */
static TBOOL fthCheckAndProcessMessageInQueue(
  IO FthContext  *fthContext
  )
{
  Msg *msgIn = NULL;
  UINT16 errCode;

  msgIn = recvMsg( MODID_FTH, &errCode );
  if( NULL NOTEQ msgIn )
  {
    fthReadAndProcessRecvMsg(msgIn );
  }
  return(fthContext->isStopFileTransfer);
}

/**
 * @param buffer  buffer returned by CURL where the data is, it is to be written to local file.
 * @param size    size of one element in the buffer
 * @param num     number of elements read from FTP and in the buffer
 * @param usrData The output file stream, where to write
 * @details
 * This function is the call back passed to CURL, It is invoked by CURL when there is any data
 * from FTP server, We check for reception of CANCEL message from BSM while downloading
 * If cancel message is received we abort the FTP, Aborting of FTP is possible only if the return value
 * of this function didn't match the number of bytes received from CURL for saving,
 * To accomplish this we return 0 from this function to libCURL.
 */
static SINT32 fthCurlWriteCallbackFunc(
  void   *buffer,
  SINT32  size,
  SINT32  num,
  void   *usrData
  )
{
  FthContext  *fthContext;
  fthContext = (FthContext*)usrData;
  if( NULL EQ fthContext  )
  {
    BS_ERROR_CURL_RETURNED_NULL_IN_CALLBACK();
    return 0;
  }
   /* open file for writing,
   * this is executed only once when the write happens first time
   * Open the file in write mode and binary */
  if( NULL EQ fthContext->lclFilePtr )
  {
    fthContext->lclFilePtr = fopen( fthContext->lclFilePath, "wb");
    if( NULL EQ fthContext->lclFilePtr )
    {
      /* failure, can't open file to write */
      BS_ERROR_FILE_ACCESS(fthContext->lclFilePath, errno  );
      BS_ERROR_FILE_OPENING_FAILED( "wb" );
      return -1;
    }
    logInfo("write call back is called ---- file size= %d", fthContext->dlFileSize);

    /* If file size is nonzero then send SUCC to BSM */
    if( fthContext->dlFileSize > 0 )
    {
      fthFillAndSendSetObjSuccRsp( NULL, fthContext->isScheduled );
			fthContext->isScheduled = FALSE;
			fthContext->isSuccSent = TRUE;
    }
  }

  /* Check whether the file can fit in Directory */
  if( TRUE EQ fthContext->isFileSizeExceed )
  {
    return 0;
  }

  /* Check to see if any cancel request pending,
   * If cancel is received, Abort the FTP */
  fthCheckAndProcessMessageInQueue( fthContext );
  if( TRUE EQ fthContext->isStopFileTransfer )
  {
    return 0;
  }

  return fwrite(buffer, size, num, fthContext->lclFilePtr);
}

/**
 * @param buffer  buffer where the data to be uploaded need to be placed
 * @param size    size of one element to be uploaded
 * @param num     number of elements to be uploaded
 * @param usrData The input file stream to read from
 * @details
 * This function is the call back passed to CURL, It is invoked by CURL when data is to be written to FTP server,
 * We check for reception of CANCEL message from BSM while uploading
 * If cancel message is received we abort the FTP, Aborting of FTP is possible only if the return value
 * of this function didn't match the number of bytes received from CURL for saving,
 * To accomplish that we return 0 from this function to libCURL.
 */
static SINT32 fthCurlReadCallbackFunc(
  void   *buffer,
  SINT32  size,
  SINT32  num,
  void   *usrData
  )
{
  FthContext  *fthContext;
  fthContext = (FthContext*)usrData;
  if( NULL EQ fthContext  )
  {
    BS_ERROR_CURL_RETURNED_NULL_IN_CALLBACK();
    return 0;
  }
  /* open file for Reading,
   * this is executed only once when the read happens first time
   * Open the file in read mode and binary */
  if( NULL EQ fthContext->lclFilePtr )
  {
    fthContext->lclFilePtr = fopen( fthContext->lclFilePath, "r");
    if( NULL EQ fthContext->lclFilePtr )
    {
      /* failure, can't open file to Read, local file didn't exist */
      BS_ERROR_FILE_ACCESS(fthContext->lclFilePath, errno  );
      BS_ERROR_FILE_OPENING_FAILED( "rb" );
      return -1;
    }

    if( fthContext->ulFileSize > 0 )
    {
      fthFillAndSendSetObjSucc( NULL, fthContext );
      fthContext->isSuccSent = TRUE;
    }
  }

  /* Check to see if any cancel request pending,
   * If cancel is received, Abort the FTP */
  fthCheckAndProcessMessageInQueue( fthContext );
  if( TRUE EQ fthContext->isStopFileTransfer )
  {
    return 0;
  }

  return fread(buffer, size, num, fthContext->lclFilePtr);
}

/**
 * @param maxDownloadSpeed  maximum download speed
 * @param maxFileSize       maximum allowed file size
 * @param curlHandle        CURL handle
 * @param writeCallbackData Structure holding FTH context
 * @details This function sets the Write parameters of CURL handle.
 */
static void fthSetCurlWriteParams(
  IN BytesPerSec     maxDownloadSpeed,
  IN UINT32          maxFileSize,
  IO CURL           *curlHandle,
  IO FthContext     *writeCallbackData /* call back will return this */
  )
{
  curl_easy_setopt( curlHandle, CURLOPT_MAXFILESIZE_LARGE, (curl_off_t)maxFileSize);
  curl_easy_setopt( curlHandle, CURLOPT_MAX_RECV_SPEED_LARGE, (curl_off_t)(maxDownloadSpeed) );
  curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, fthCurlWriteCallbackFunc );
  curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, writeCallbackData );
}

/**
 * @param maxUploadSpeed  maximum upload speed
 * @param fthContext      FTH context
 * @details This function sets the Read parameters of CURL handle,
 * we set the FthContext as the call back data to be returned.
 */
static void fthSetCurlReadParams(
  IN MaxUploadSpeed     maxUploadSpeed,
  IO FthContext        *fthContext
  )
{
  /* CURL needs the size of file before starting the upload
   * We get size of file here using stat() */
  struct stat st;

  if( 0 EQ stat( fthContext->lclFilePath, &st ) )
    fthContext->ulFileSize = st.st_size;

  curl_easy_setopt( fthContext->curlHandle, CURLOPT_READFUNCTION, fthCurlReadCallbackFunc );
  curl_easy_setopt( fthContext->curlHandle, CURLOPT_UPLOAD, 1L );
  curl_easy_setopt( fthContext->curlHandle, CURLOPT_READDATA, fthContext );
  curl_easy_setopt( fthContext->curlHandle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)st.st_size );
  curl_easy_setopt( fthContext->curlHandle, CURLOPT_MAX_SEND_SPEED_LARGE, (curl_off_t)(maxUploadSpeed) );
}

/**
 * @param callBackFthContext  FthContext This is returned by CURL
 * @param dltotal             Total number of bytes for download
 * @param dlnow               Total number of bytes downloaded till now
 * @param ultotal             Total number of bytes for upload
 * @param ulnow               Total number of bytes uploaded till now
 * @details
 * This function is the CURL callback which is returned when ever data is received
 * or sent by CURL library, We update the status table every time we hit this function.
 */
static SINT32 fthProgCallbackFunc(
  void  *callBackFthContext,
  double dltotal,
  double dlnow,
  double ultotal,
  double ulnow
  )
{
  FthContext   *fthContext = getFthContext();

  if( NULL EQ callBackFthContext )
  {
    BS_ERROR_CURL_RETURNED_NULL_IN_CALLBACK();
    return 0;
  }

  /* Check to see if any cancel request pending,
   * If cancel is received, Abort the FTP */
  fthCheckAndProcessMessageInQueue( fthContext );
  if( TRUE EQ fthContext->isStopFileTransfer )
  {
    return 1;
  }

  if( FTH_STATE_DOWNLOAD_IN_PROG EQ ((FthContext*)callBackFthContext)->fthState )
  {
    /* During download, upload variables are not used */
    ultotal = ulnow = 0;
     /* If the file cann't be accomadated then send download Reject */
    if( FALSE EQ fthContext->isFileSizeChecked )
    {
      if( TRUE EQ fthIsFileAccommodationPossible(dltotal, fthContext) )
      {
        fthContext->isFileSizeChecked = TRUE;
      }
      else
      {
        return 1;
      }
    }
    fthContext->dlFileSize = dltotal;
		fthUpdateFileDownloadStatusTable(fthContext->fileDownload.fileDownloadCommand,
				((FthContext*)callBackFthContext)->fileDownload.fileName,
				DOWNLOADSTATUS_ONGOING,
				(dlnow*100)/dltotal,
				NULL);

  }
  else if( FTH_STATE_UPLOAD_IN_PROG EQ ((FthContext*)callBackFthContext)->fthState )
  {
     /* During upload, download variables are not used */
     dltotal = dlnow = 0;
     /* Update the status */
     fthUpdateFileUploadStatusTable(FILEUPLOADCOMMANDRECEIVED_UPLOAD,
                                 ((FthContext*)callBackFthContext)->fileUpload.fileName,
                                 UPLOADSTATUS_ONGOING,
                                 ( (UINT8)((ulnow*100)/((FthContext*)callBackFthContext)->ulFileSize) )
                                 );
  }
  return 0;
}

/**
 * @param curlHandle         CURL handle
 * @param progCallBackData   FTH context, this is returned by CURL while invoking callback
 * @details This function sets the Progress parameters of CURL handle
 */
static void fthSetCurlProgParams(
  IO CURL          *curlHandle,
  IO FthContext    *progCallBackData
  )
{
  curl_easy_setopt( curlHandle, CURLOPT_NOPROGRESS, 0L );
  curl_easy_setopt( curlHandle, CURLOPT_PROGRESSFUNCTION, fthProgCallbackFunc );
  curl_easy_setopt( curlHandle, CURLOPT_PROGRESSDATA, progCallBackData );
}

/**
 * @param rmtPort      Remote port of server to connect
 * @param totTimeOut   The maximum time limit before which download must complete
 * @param connTimeOut  The maximum time limit before connection stabilised
 * @param curlHandle   CURL handle
 * @details This function sets the optional parameters of CURL handle.
 */
static void fthSetCurlOptParams(
  IN PortNum         rmtPort,
  IN IntervalSec  totTimeOut,
  IN IntervalSec  connTimeOut,
  IO CURL        *curlHandle
  )
{
  curl_easy_setopt(curlHandle, CURLOPT_PORT, (long)rmtPort);
  curl_easy_setopt(curlHandle, CURLOPT_FORBID_REUSE, 1L);
  curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, (long)totTimeOut);
  curl_easy_setopt(curlHandle, CURLOPT_FTP_RESPONSE_TIMEOUT, (long)connTimeOut);
  curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, (long)connTimeOut );
  curl_easy_setopt(curlHandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L );
  curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1L);


  /* TODO Disable verbose for deployment*/
  curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L );

  /* SET the time OUT delay for LOW SPEED,
   * If the data is 50 bytes for 3 seconds, then close the connection */
  curl_easy_setopt(curlHandle, CURLOPT_LOW_SPEED_LIMIT, (long)50);
  curl_easy_setopt(curlHandle, CURLOPT_LOW_SPEED_TIME, (long)(connTimeOut*1000)); /* the value is in milli seconds */
}

/**
 * @param localFileName Name of local file to be created
 * @param fthContext    FTH context holding local file name and FILE pointer to the file
 * @details This function initialises local file pointer in FTH context.
 */
static void fthInitLocalFileHandle(
  IN UINT8         *localFileName,
  IO FthContext    *fthContext
  )
{
  /* Initialize local file name, to the name passed from BSM */
  copyBytes( localFileName, sizeof(FileName), &fthContext->lclFilePath );
  fthContext->lclFilePtr = NULL;
}

/**
 * @param fthContext  FTH context holding local file name and FILE pointer to the file
 * @details This function initializes the log file pointer, This is the file where CURL writes the log messages.
 */
static void fthInitLogFileHandle(
  IO FthContext   *fthContext
  )
{
  FileName  logFileName;

  snprintf( logFileName, sizeof(FileName), "%s/%s", fthContext->fthCfg.downloadDir, FTH_LOG_FILENAME );
  /* Open log file for writing,
   * Log file pointer is passed to CURL, all the log data is placed in this file.
   * If log file cannot be created then redirect the log to stdout
   */
  logInfo("Name of FTH LogFile is:%s", logFileName);

  if( NULL EQ fthContext->logFilePtr )
  {
    fthContext->logFilePtr = fopen( logFileName, "w");
    if( NULL EQ fthContext->logFilePtr )
    {
      /* failure, can't open file to write */
      BS_ERROR_FILE_OPENING_FAILED( "w" );
      curl_easy_setopt( fthContext->curlHandle, CURLOPT_STDERR, stdout );
    }
    else
    {
      curl_easy_setopt( fthContext->curlHandle, CURLOPT_STDERR, fthContext->logFilePtr );
    }
  }
}

/**
 * @param fthContext  FTH context holding local file name and FILE pointer to the file
 * @details This function does clearing operations of CURL library and CURL handle
 */
static void fthCleanUp(
  IO FthContext  *fthContext
  )
{
  if( NULL NOTEQ fthContext->curlHandle )
  {
    /* Clean up, the session, This will close the connection
     * Call the global library cleanup routine*/
    curl_easy_cleanup(fthContext->curlHandle);
    curl_global_cleanup();

    fthContext->curlHandle = NULL;
  }
  /* If Local file is created, close the local file
   * set File pointer to NULL */
  if(fthContext->lclFilePtr)
  {
    fclose(fthContext->lclFilePtr);
    fthContext->lclFilePtr = NULL;
  }
  /* Close the log File */
  if(fthContext->logFilePtr)
  {
    fclose(fthContext->logFilePtr);
    fthContext->logFilePtr = NULL;
  }
}

/**
 * @param curlErrorCode  Response code returned by CURL
 * @details
 * This function converts response code returned by CURL into BS Error Values
 * For detailed logging we need the data from Download/Upload requests.
 */
static BsErrorValue fthConvertCurlErrToBsErr(
  IN CURLcode   curlErrorCode
  )
{
  BsErrorValue errVal;
  FthContext *fthContext = getFthContext();
  FileDownload *req = NULL;

  if( fthContext->ulFileSize > 0 )
  {
    /* This variable is only filled for upload, so we set the upload structure for download pointer
     * req contains  starting 9 parameter of Upload(common in both upload and download)*/
    req = (FileDownload*)&(fthContext->fileUpload);
  }
  else
  {
    req = &(fthContext->fileDownload);
  }

  switch(curlErrorCode)
  {
    case CURLE_COULDNT_RESOLVE_HOST:
      errVal = BS_ERR_COULDNT_RESOLVE_HOST;
      BS_ERROR_COULDNT_RESOLVE_HOST(req->sourceIp );
      break;
    case CURLE_COULDNT_CONNECT:
      errVal = BS_ERR_COULDNT_CONNECT;
      BS_ERROR_COULDNT_CONNECT(req->sourceIp );
      break;
    case CURLE_FTP_WEIRD_SERVER_REPLY:
      errVal = BS_ERR_FTP_WEIRD_SERVER_REPLY;
      BS_ERROR_FTP_WEIRD_SERVER_REPLY(req->sourceIp );
      break;
    case CURLE_LOGIN_DENIED:
      errVal = BS_ERR_FTP_ACCESS_DENIED;
      BS_ERROR_FTP_ACCESS_DENIED(req->loginId );
      break;
    case CURLE_FTP_WEIRD_PASS_REPLY:
      errVal = BS_ERR_FTP_WEIRD_PASS_REPLY;
      BS_ERROR_FTP_WEIRD_PASS_REPLY();
      break;
    case CURLE_FTP_WEIRD_USER_REPLY:
      errVal = BS_ERR_FTP_WEIRD_USER_REPLY;
      BS_ERROR_FTP_WEIRD_USER_REPLY();
      break;
    case CURLE_FTP_WEIRD_PASV_REPLY:
      errVal = BS_ERR_FTP_WEIRD_PASV_REPLY;
      BS_ERROR_FTP_WEIRD_PASV_REPLY();
      break;
    case CURLE_FTP_CANT_GET_HOST:
      errVal = BS_ERR_FTP_CANT_GET_HOST;
      BS_ERROR_FTP_CANT_GET_HOST( req->sourceIp );
      break;
    case CURLE_FTP_COULDNT_SET_BINARY:
      errVal = BS_ERR_FTP_COULDNT_SET_BINARY;
      BS_ERROR_FTP_COULDNT_SET_BINARY();
      break;
    case CURLE_FTP_COULDNT_RETR_FILE:
      errVal = BS_ERR_FTP_COULDNT_RETR_FILE;
      BS_ERROR_FTP_COULDNT_RETR_FILE(req->fileName );
      break;
    case CURLE_FTP_WRITE_ERROR:
      errVal = BS_ERR_FTP_WRITE_ERROR;
      BS_ERROR_FTP_WRITE_ERROR(fthContext->fthCfg.downloadDir );
      break;
    case CURLE_OUT_OF_MEMORY:
      errVal = BS_ERR_OUT_OF_MEMORY;
      BS_ERROR_OUT_OF_MEMORY();
      break;
    case CURLE_OPERATION_TIMEDOUT:
      errVal = BS_ERR_OPERATION_TIMED_OUT;
      BS_ERROR_OPERATION_TIMEOUTED(fthContext->fthCfg.downloadTimeOutInterval );
      break;
    case CURLE_FTP_COULDNT_GET_SIZE:
      errVal = BS_ERR_FTP_COULDNT_GET_SIZE;
      BS_ERROR_FTP_COULDNT_GET_SIZE();
      break;
    case CURLE_FILESIZE_EXCEEDED:
      errVal = BS_ERR_FILESIZE_EXCEEDED;
      BS_ERROR_FILESIZE_EXCEEDED(fthContext->fthCfg.maxFileSize);
      break;
    case CURLE_SSH:
      errVal = BS_ERR_CURL_SSH_ERROR_UNKNOWN;
      BS_ERROR_CURL_SSH_ERROR_UNKNOWN();
      break;
    case CURLE_REMOTE_FILE_NOT_FOUND:
      errVal = BS_ERR_REMOTE_FILE_NOT_FOUND;
      BS_ERROR_FILE_NOT_FOUND(req->fileName );
      break;
    case CURLE_UNSUPPORTED_PROTOCOL:
      errVal = BS_ERR_UNSUPPORTED_PROTOCOL;
      BS_ERROR_UNSUPPORTED_PROTOCOL( req->protocolUsed );
      break;
    case CURLE_FAILED_INIT:
      errVal = BS_ERR_FAILED_CONNECTION_INIT;
      BS_ERROR_FAILED_CONNECTION_INIT(req->sourceIp );
      break;
    case CURLE_READ_ERROR:
      errVal = BS_ERR_FILE_ACCESS_PERMISSION_DENIED;
      BS_ERROR_FILE_ACCESS_PERMISSION_DENIED(fthContext->fthCfg.downloadDir);
      break;
    case CURLE_WRITE_ERROR:
      errVal = BS_ERR_FILE_ACCESS_PERMISSION_DENIED;
      BS_ERROR_FILE_ACCESS_PERMISSION_DENIED(fthContext->fthCfg.downloadDir );
      break;
    case CURLE_SEND_ERROR:
      errVal = BS_ERR_FTP_SENDING_ERROR;
      BS_ERROR_FTP_SENDING_ERROR();
      break;
    case CURLE_RECV_ERROR:
      errVal = BS_ERR_FTP_RECEIVING_ERROR;
      BS_ERROR_FTP_RECEIVING_ERROR();
      break;
    default:
      BS_ERROR_CHECK_CURL( curlErrorCode );
      BS_ERROR_CURL_ERROR_UNKNOWN();
      errVal = BS_ERR_CURL_ERROR_UNKNOWN;
      break;
 }
 return errVal;
}

/**
 * @param curlHandle  CURL handle
 * @param bsErr       BS Error code
 * @details
 * This function does the file transfer using curl library and returns the response
 * Actual destination file creation is delayed until the first write
 * call back so that we won't create an empty file in case the remote file
 * doesn't exist or some error with server.
 */
static TBOOL fthDoFileTransfer(
  IO CURL                   *curlHandle,
  IO BsErrorValue           *bsErr
  )
{
  CURLcode res;

  res = curl_easy_perform(curlHandle);

  if(CURLE_OK NOTEQ res)
  {
    *bsErr = fthConvertCurlErrToBsErr( res );
    return FALSE;
  }
  return TRUE;
}

/**
 * @param fthContext     Context Information of FTH
 * @param bsErr          BS Error code
 * @details This function init CURL parameter, create URL and perform file Transfer.
 */
static void fthStartDownload(
  IO FthContext             *fthContext,
  IO BsErrorValue           *bsErr
  )
{

  UINT8   ftpurl[LEN_OF_FTP_URL]; /* CURL is stored in this buffer */
  UINT8   *baseName = NULL;

  /* Create URL */
  fthCreateUrl( fthContext->fileDownload.loginId,     fthContext->fileDownload.password,
                fthContext->fileDownload.sourceIp,    fthContext->fileDownload.fileName,
                fthContext->fileDownload.protocolUsed,ftpurl );

  /*
   * 1. Initialise CURL Library
   * 2. Get CURL handle
   */
  fthContext->curlHandle = NULL;
  curl_global_init( CURL_GLOBAL_DEFAULT );
  fthContext->curlHandle = curl_easy_init();
  if( NULL EQ fthContext->curlHandle )
  {
    BS_ERROR_CURL_HANDLE_NOT_CREATED();
		*bsErr = BS_ERR_OUT_OF_MEMORY;
		fthFillAndSendSetObjFailRsp( NULL, BS_ERR_OUT_OF_MEMORY, fthContext->isScheduled );
		fthContext->isScheduled =  FALSE;
		return;
  }

  /*set the FTP URL in CURL Handle */
  curl_easy_setopt( fthContext->curlHandle, CURLOPT_URL, ftpurl );

  /* Set port, timeout interval and verbose parameters of Handle*/
  fthSetCurlOptParams( fthContext->fileDownload.sourcePort,    fthContext->fthCfg.downloadTimeOutInterval,
                       fthContext->fthCfg.connTimeOutInterval, fthContext->curlHandle);

  /* Set the log File Handle */
  fthInitLogFileHandle( fthContext );
  /* Set Write Parameters of Handle*/
  baseName = basename(&(fthContext->fileDownload.fileName[0]) );
  snprintf( fthContext->lclFilePath, sizeof(FileName), "%s/%s", fthContext->fthCfg.downloadDir, baseName );
  logInfo("Local file name is :%s", &fthContext->lclFilePath );
  fthContext->lclFilePtr = NULL;
  fthSetCurlWriteParams( fthDownloadSpeedFromEnum(fthContext->fileDownload.maxDownloadSpeed),
                         fthContext->fthCfg.maxFileSize,fthContext->curlHandle, fthContext);

  /* Set progress parameters of handle */
  fthSetCurlProgParams( fthContext->curlHandle, fthContext );
	fthUpdateFileDownloadStatusTable(fthContext->fileDownload.fileDownloadCommand,
			&(fthContext->fileDownload.fileName[0]),
			DOWNLOADSTATUS_ONGOING,
			0,
			NULL);
	fthDoFileTransfer( fthContext->curlHandle, bsErr);

  /* Download is complete by now
   * Do CURL cleanup
   * Now trace file is not going to BSM so no check on return value */
  fthCleanUp( fthContext );
}




/**
 * @param bsErr          BS Error code
 * @param fthContext     Context Information of FTH
 * @details This function init CURL parameter, create URL and perform file Transfer.
 */
static void fthStartUpload(
  IO FthContext             *fthContext,
  IO BsErrorValue           *bsErr
  )
{
  UINT8   ftpurl[LEN_OF_FTP_URL]; /* CURL is stored in this buffer */

 /* CURL Expects the remote filename as a complete path for upload in case of SCP, SFTP */

  /* Create URL */
  fthCreateUrl( fthContext->fileUpload.loginId,       fthContext->fileUpload.password,
		fthContext->fileUpload.destinationIp, fthContext->fileUpload.remoteFileName,
		fthContext->fileUpload.protocolUsed,  ftpurl );

  /*
   * Initializes CURL library and creates CURL handle
   * 1. Initialize CURL Library
   * 2. Get CURL handle
   */
  fthContext->curlHandle = NULL;
  curl_global_init( CURL_GLOBAL_ALL );
  fthContext->curlHandle = curl_easy_init();
  if( NULL EQ fthContext->curlHandle )
  {
    BS_ERROR_CURL_HANDLE_NOT_CREATED();
    *bsErr = BS_ERR_OUT_OF_MEMORY;
    return;
  }

  /*set the FTP URL */
  curl_easy_setopt( fthContext->curlHandle, CURLOPT_URL, ftpurl );

  /* Set Optional Parameters port, timeout interval, verbose parameters of Handle*/
  fthSetCurlOptParams( fthContext->fileUpload.destinationPort, fthContext->fthCfg.uploadTimeOutInterval,
                       fthContext->fthCfg.connTimeOutInterval, fthContext->curlHandle);

  /* Set the log File Handle */
  fthInitLogFileHandle( fthContext );

  /* Set Read Parameters of Handle,
   * We expect the local filename to be complete file path*/
  fthInitLocalFileHandle( fthContext->fileUpload.fileName, fthContext );
  fthSetCurlReadParams( fthUploadSpeedFromEnum(fthContext->fileUpload.maxUploadSpeed),fthContext);

  /* Set progress parameters of handle */
  fthSetCurlProgParams( fthContext->curlHandle, fthContext );

  fthUpdateFileUploadStatusTable(FILEUPLOADCOMMANDRECEIVED_UPLOAD,
                              &(fthContext->fileUpload.fileName[0]),
                              UPLOADSTATUS_ONGOING,0);

  fthDoFileTransfer( fthContext->curlHandle, bsErr);

  /* Do the clean up */
  fthCleanUp( fthContext );

}

/**
 * @param fthContext   Context of FTH
 * @details
 * This function removes the temporary files that are created, if transfer stops in the middle.
 */
void fthStopDownloadAndClearLocalFile(
  IO FthContext   *fthContext
  )
{
  FILE *fp;

  /* We need to stop the download after this so set the flag to TRUE */
  fthContext->isStopFileTransfer = TRUE;

  /* Close the open file first before processing
   * If Local file is created, close the local file
   * set File pointer to NULL
   */
  if(fthContext->lclFilePtr)
  {
    fclose(fthContext->lclFilePtr);
    fthContext->lclFilePtr = NULL;
  }
  fp = fopen(fthContext->lclFilePath, "a");
  if( NULL NOTEQ fp )
  {
    /* Tmp file Exists, remove the file */
    remove(fthContext->lclFilePath);
    fclose(fp);
  }
}

/**
 * @param fthContext   Context of FTH
 * @details
 * This function removes the temporary files that are created, if FTP stops in the middle
 * check for existence of file using fopen, if file exists then remove the file.
 */
void fthStopUploadAndClearRemoteFile(
  IO FthContext   *fthContext
  )
{
  /* We need to stop the download after this so set the flag to TRUE */
  fthContext->isStopFileTransfer = TRUE;

  /* Close the open file first before processing
   * set File pointer to NULL
   */
  if(fthContext->lclFilePtr)
  {
    fclose(fthContext->lclFilePtr);
    fthContext->lclFilePtr = NULL;
  }
  /* TODO How to Remove remote file */
}

/**
 * @param dwReq        download request message
 * @param fthContext   Context of FTH
 * @details
 * This function processes the received download message,
 * In this function FTH does sanity checks and sanity checks pass then start the download,
 * After returning from libCurl handler it checks return value of error and decides
 * whether transfer is successful or not.
 */
void fthStartFileDownload(
  IN FileDownload        *dwReq,
  IO FthContext          *fthContext
  )
{
  BsErrorValue  retval = BS_NO_ERROR;

  fthContext->fileDownload = *dwReq;
  /* Checking the space is available for download */
  if(FALSE EQ fthDoesSpaceAvailableInDownloadDir(fthContext))
  {
		fthFillAndSendSetObjFailRsp( NULL, fthContext->bsError, fthContext->isScheduled );
		fthContext->isScheduled =  FALSE;
		return;
  }

  /* space is available. So initiate download
   * Copy the download request from BSM, so that we can retain filename and other parameters
   * Change state to FTH_STATE_DOWNLOAD_IN_PROG,
   * This function will return after File is downloaded,
   * This is the only exception where state change is allowed because libCURL is a run to completion library
   */
  fthInitContext( fthContext );
  fthSetState( fthContext, FTH_STATE_DOWNLOAD_IN_PROG);
  fthStartDownload( fthContext, &retval);

  /* Check whether Downlaod is rejected because of file size
   * Check that download is not stopped in the middle, if it is stopped
   * Nothing to be done, If Cancel/OpStop is received in the middle of download.
   * Processing of received request already done, replies already sent to BSM
   */
  /* TODO
   *  1. Decide whether we need to send Download-fail when download is cancelled by user
   *  2. check the validity of the image downloaded
   */
  if( TRUE EQ fthContext->isFileSizeExceed )
  {
    return; /* Reject message is already Sent */
  }

  if( TRUE EQ fthContext->isStopFileTransfer )
  {
    /* On failure remove temporary file,
     * Send download failure message */
    fthStopDownloadAndClearLocalFile( fthContext );
		fthUpdateFileDownloadStatusTable(FILEDOWNLOADCOMMANDRECEIVED_CANCELDOWNLOAD,
				&(fthContext->fileDownload.fileName[0]),
				DOWNLOADSTATUS_CANCELLED,
				0,
				NULL);
		BS_ERROR_USER_INITIATED_CANCEL();
		fthFillAndSendSetObjFailRsp( NULL, BS_ERR_USER_INITIATED_CANCEL, TRUE );
		return;
  }

  if( (BS_NO_ERROR EQ retval ) && (TRUE EQ fthContext->isSuccSent) )
  {
    /* File download successful.
     * Send download complete message
     * Updated downlaod Status table
     */
		fthUpdateFileDownloadStatusTable(fthContext->fileDownload.fileDownloadCommand,
				&(fthContext->fileDownload.fileName[0]),
				DOWNLOADSTATUS_SUCCESSFUL,
				100,
				NULL);
		fthFillAndSendSetObjSuccRsp(NULL,TRUE);
	}
  else
  {
    /* Success not sent indicates that the request is rejected */
    if( FALSE EQ fthContext->isSuccSent )
    {
			fthUpdateFileDownloadStatusTable(fthContext->fileDownload.fileDownloadCommand,
					&(fthContext->fileDownload.fileName[0]),
					DOWNLOADSTATUS_FAILED,
					0,
					NULL);
			if( BS_NO_ERROR EQ retval )
      {
        BS_ERROR_WRITE_CALLBACK_NEVER_CALLED();
				fthFillAndSendSetObjFailRsp( NULL, BS_ERR_FTP_WEIRD_SERVER_REPLY, fthContext->isScheduled );
				fthContext->isScheduled =  FALSE;
				return;
			}
			fthFillAndSendSetObjFailRsp( NULL, retval, fthContext->isScheduled );
			fthContext->isScheduled =  FALSE;
		}
    else
    {
	   /* There is error in accessing local file */
       fthStopDownloadAndClearLocalFile( fthContext );
			 fthUpdateFileDownloadStatusTable(fthContext->fileDownload.fileDownloadCommand,
					 &(fthContext->fileDownload.fileName[0]),
					 DOWNLOADSTATUS_FAILED,
					 0,
					 NULL);

			 fthFillAndSendSetObjFailRsp( NULL, retval, TRUE );
		}
  }

}

/**
 * @param uploadReq    upload request message
 * @param fthContext   Context of FTH
 * @details
 * This function processes the received upload message
 * In this function FTH does sanity checks and sanity checks pass then start the upload
 * After returning from libCurl handler it checks return value of error and decides
 * whether transfer is successful or not.
 */
void fthStartFileUpload(
  IN FileUpload    *uploadReq,
  IO FthContext    *fthContext
  )
{
  BsErrorValue  retval=BS_NO_ERROR;

  fthContext->fileUpload = *uploadReq;
  /* If the file don't exist then send Reject to User */
  if( FALSE EQ fthDoesFileExist( uploadReq->fileName) )
  {
    BS_ERROR_FILE_NOT_FOUND(&fthContext->fileUpload.fileName);
    fthFillAndSendSetObjFail( NULL, BS_ERR_FILE_NOT_FOUND, fthContext );
    fthUpdateFileUploadStatusTable(FILEUPLOADCOMMANDRECEIVED_UPLOAD,
                                &(fthContext->fileUpload.fileName[0]),
                                UPLOADSTATUS_FAILED,0);
    return;
  }

  /* Initialize FTH Context
   * Save the upload request */
  fthInitContext( fthContext );

  /* Change state to Upload in progress,
   * This function will return after data is downloaded,
   * This is the only exception where state change is allowed because libCURL is run to completion library
   */
  fthSetState( fthContext, FTH_STATE_UPLOAD_IN_PROG);

  /* Start file upload */
  fthStartUpload( fthContext, &retval );


  /* Check that upload is not stopped in the middle, if it is stopped
   * Nothing to be done, If Cancel/OpStop is received in the middle of upload.
   * Processing of received request already done, replies already sent to BSM
   */
  if( TRUE EQ fthContext->isStopFileTransfer )
  {
    /* File transfer was stopped in the middle,
     * Send upload failure message */
    fthStopUploadAndClearRemoteFile( fthContext );
    fthUpdateFileUploadStatusTable(FILEUPLOADCOMMANDRECEIVED_UPLOADCANCEL,
                                &(fthContext->fileUpload.fileName[0]),
                                UPLOADSTATUS_CANCELLED,0);
    BS_ERROR_USER_INITIATED_CANCEL();
    fthCreateAndSendUploadFail( BS_ERR_USER_INITIATED_CANCEL);
    return;
  }

  if( ( BS_NO_ERROR EQ retval ) && ( TRUE EQ fthContext->isSuccSent) )
  {
    /* Update status table
     * Send Upload complete
     */
    fthUpdateFileUploadStatusTable(FILEUPLOADCOMMANDRECEIVED_UPLOAD,
                                &(fthContext->fileUpload.fileName[0]),
                                UPLOADSTATUS_SUCCESSFUL,100);
    /* Check whether uploaded file is to be delete or not*/
    if( TRUE EQ fthContext->fileUpload.deleteAfterUpload)
    {
      remove(fthContext->fileUpload.fileName);
    }
    fthCreateAndSendUploadCmpl( fthContext);
  }
  else
  {
    /* Success not sent indicates that the request is rejected */
    if( FALSE EQ fthContext->isSuccSent )
    {
      fthUpdateFileUploadStatusTable(FILEUPLOADCOMMANDRECEIVED_UPLOAD,
                                  &(fthContext->fileUpload.fileName[0]),
                                  UPLOADSTATUS_FAILED,0);
      if( BS_NO_ERROR EQ retval )
      {
        BS_ERROR_READ_CALLBACK_NEVER_CALLED();
        fthFillAndSendSetObjFail( NULL, BS_ERR_FTP_WEIRD_SERVER_REPLY, fthContext );
        return;
      }
      fthFillAndSendSetObjFail( NULL, retval, fthContext );
    }
    else
    {
      /* There is error in accessing local file */
      fthStopDownloadAndClearLocalFile( fthContext );
      fthUpdateFileUploadStatusTable(FILEUPLOADCOMMANDRECEIVED_UPLOAD,
                                  &(fthContext->fileUpload.fileName[0]),
                                  UPLOADSTATUS_FAILED,0);
      fthCreateAndSendUploadFail( retval);
    }
  }
}
