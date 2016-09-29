/**
 * @file    log.c
 * @brief   This file contains functions for logging data from the modules
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @author  Manu Sharma
 * @author  Lakshmi Narayana Madala
 * @author  Shweta Polepally
 *
 * @details Initial coding done - Lakshmi
 * @details Removed as call to timer is leading to SIGSEGV - Manu/Shweta
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include "log.h"
#include "imc_util.h"


/* Global variable, The log context of a module, this contains the log file name
 **/
LogContext g_logContext;

/**
 * @param   logContext The log Context
 * @details
 * This functions opens the log file
 * If log file cannot be opened, logs are written to stderr.
 */
BsErrorValue openLogFile(
		IO LogContext   *logContext
		)
{
	BsErrorValue error = BS_NO_ERROR;

	if( strncmp( LOG_ON_STDOUT, logContext->logFilename, sizeof(LOG_ON_STDOUT) ) EQ 0 )
	{
		logContext->logFp = stdout;
	}
	else
	{
		logContext->logFp = fopen( logContext->logFilename, LOG_FILE_MODE );
	}

	if( NULL EQ logContext->logFp )
	{
		logContext->logFp = stderr;
		error = BS_ERR_IN_OPENING_LOG_FILE;
		BS_ERROR_IN_OPENING_LOG_FILE( logContext->logFilename );
	}
	else
		setvbuf( logContext->logFp, NULL, _IONBF /*_IOLBF*/, 80);

	return error;
}

/**
 * @param   logContext The log Context
 * @details
 * This functions closes the log file
 */
void closeLogFile(LogContext *logContext)
{
	if(logContext->logType EQ LOG_TYPE_WRITE_TO_FILE)
	{
		if( logContext->logFp )
		{
			fclose( logContext->logFp );
		}
		logContext->logFp = NULL;
	}
}

/**
 * @param buf buffer to write log
 * @param len length of log
 * @param msg log message
 * @details This function will write log to buffer
 */
void writeToBuf(
		IO UINT8  *buf,
		IN UINT16  len,
		IN UINT8  *msg
		)
{
	snprintf(buf, len, msg);
}

/**
 * @param modId        Module ID
 * @param rootDir      root Directory
 * @param ptrLogConfig logConfig context
 * @details This function will read logConfig table from file and updates the context.
 */
TBOOL readLogConfigTableAndUpdateLogCtx(
		IN ModuleId    modId,
		IN UINT8      *rootDir,
		IO LogConfig  *ptrLogConfig
		)
{
	UINT8 fullFilePath[MAX_BUFFER_SIZE] ;
	config_t         cfg;
	config_setting_t *setting;

	/* create a full path of the file */
	clearBytes( &fullFilePath, sizeof(fullFilePath));
	constructFullPath ( rootDir, "/config/enb_user_config.cfg", fullFilePath );

	config_init(&cfg); /* init LibConfig */
	/* Read the file. If there is an error, report it and exit. */
	if(! config_read_file(&cfg, fullFilePath))
	{
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
				config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return FALSE;
	}
	setting = config_lookup(&cfg, "logConfig");
	if(setting NOTEQ NULL)
	{
		int count = config_setting_length(setting);
		int       numElem;
		for(numElem = 0;numElem < count; ++numElem)
		{
			int         moduleId;

			config_setting_t *table = config_setting_get_elem(setting, numElem);
			/* only output the record if all of the expected fields are present */
			clearBytes(ptrLogConfig, sizeof(LogConfig));
			config_setting_lookup_int(table, "moduleId", &moduleId);

			/* Check if moduleId is same as modId of the row */
			if((SINT32)modId EQ moduleId)
			{
				ptrLogConfig->moduleId = moduleId;
				config_setting_lookup_int(table, "logMask1", &ptrLogConfig->logMask1);
				break;
			}
		}
		config_destroy(&cfg);
		return TRUE;
	}
	else
	{
		fprintf(stderr, "Setting Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
	}
	config_destroy(&cfg);
	return FALSE;
}

/**
 * @param args arguments to write to buffer
 * @param format format of arguments received
 * @details This function writes log to shared memory
 */
void dispatchLogToSharedMem(
		IO SectionType sectionType,
		IN va_list args,
		const char *format, ...
		)
{
	char            *logMsg = NULL;
	SINT32          logLen = 0;
	LogContext      *logContext = getLogContext();

	/*
	 * Find the length of received log
	 */
	logLen = vasprintf(&logMsg, format, args);

	/*
	 * return if log length is 0 bytes
	 */
	if(!logLen)
	{
		fprintf(stderr, "%s", "Trying to write 0 bytes of log message\n");
		return;
	}
	
	writeToSharedMem(logMsg, logLen, &(logContext->circularBuffers), logContext->exeType, sectionType);

	if(logMsg NOTEQ NULL)
	{
		free(logMsg);
	}
}

/**
 * @param exeName The program name to be printed in log file
 * @details
 * This function initializes logging for a module.
 * It should be called at module initialization so that all logs are captured.
 * It opens log file for the calling module, sets log priority
 * Default priority is to print all logs when log4c is not used
 */
void logInit(
		IN UINT8    *exeName,
		IN UINT8    *rootDir
		)
{
	UINT8      *exeBaseName = getBasename(exeName);

	LogContext *logContext  = getLogContext();
	
	clearBytes( logContext, sizeof(LogContext));
	clearBytes( logContext->logFilename, MAX_LOG_FILENAME ) ;
	clearBytes( logContext->exeName, MAX_LOG_FILENAME ) ;

	if( NULL NOTEQ rootDir )
	{
		setString100( &(logContext->logFilename), rootDir );
		appendString100( &(logContext->logFilename), "/log/" );
	}
	appendString100( &(logContext->logFilename), exeBaseName);
	appendString100( &(logContext->logFilename), ".log" );

	strncpy( logContext->exeName, exeBaseName, strlen(exeBaseName) );

	/*
	 * store the exe type in log context
	 */
	logContext->exeType = getExeTypeByExeName(logContext->exeName);
	logContext->logType = getLogType();

	/*
	 * If exe wish to write log to shared memory, attach to shared memory else allocate local buffer
	 */
	if(logContext->logType EQ LOG_TYPE_WRITE_TO_FILE)
	{
		openLogFile( logContext );
	}
	else if(logContext->logType EQ LOG_TYPE_SHARED_MEMORY)
	{
		/*
		 * Get the shared memory key and attach to shared memory using this key
		 */
		if (attachToSharedMem(logContext->exeType, &(logContext->circularBuffers)) EQ FALSE) 
		{
			fprintf(stderr, "attachTosharedMemory failed ... Exiting\n");
			exit(EXIT_FAILURE);
		} 
		else
		{
			initCircularBuffers(logContext->exeType, &(logContext->circularBuffers));
			sprintf(logContext->circularBuffers.partition, rootDir);
		}
	}
	else
	{
		exit(EXIT_FAILURE);
	}
	// Set all bits to 1 (TRUE)
	if ( (MODID_CIH1 NOTEQ logContext->logConfig.moduleId) && (MODID_CIH2 NOTEQ logContext->logConfig.moduleId) && (MODID_CIH3 NOTEQ logContext->logConfig.moduleId))
	{
		logContext->logConfig.logMask1 = 1048575; /* All logs are enabled at start-up */
	}
	logInfo("=== LOG OPENED FOR PROGRAM=%s === ", logContext->exeName);
}

/**
 * @param   exeName The program name to be printed in log file
 * @details This functions closes the log file
 */
void logClose(
		IN UINT8 *exeName
		)
{
	logInfo("=== LOG CLOSED FOR PROGRAM=%s ===", exeName);
	LogContext *logContext = getLogContext();

	if(logContext->logType EQ LOG_TYPE_SHARED_MEMORY)
	{
		if(detachFromShm(logContext->exeType, &(logContext->circularBuffers)) EQ -1)
		{
			logInfo("Shared memory detach failed for [%s]", logContext->exeName);
		}
	}
	else
	{
		closeLogFile( logContext );
	}
}

/**
 * @details 
 * This function writes the log with givenPriority into logFile, as per format
 */
void logWriteWithPriority (
		IN UINT32  isWriteToLogEnabled,
		IN SectionType sectionType,
		const char *format, ...
		)
{
	if(FALSE EQ isWriteToLogEnabled)
	{
		return;
	}
	va_list  args;
	struct timeval tv;  
        char        *formatBuffer = NULL;
	LogContext  *logContext   = getLogContext();

	va_start(args, format);

	if((logContext->logType EQ LOG_TYPE_WRITE_TO_FILE)  && (!logContext->logFp))
	{
		fprintf(stderr, "log file is not opened for exe %s\n", logContext->exeName);
		return;
	}

	gettimeofday(&tv, NULL);
	/* Print time in [HH:MM:SS:mmm] format where m = milliseconds */
	/* Note: The below changes are made due to crash which occurs at WIH due to localtime
	 * Temporary fix. Need to check this later - 19July2011 */
	/* strftime(buffer,sizeof(String10),"%T",localtime(&curtime)); */

	if(logContext->logType EQ LOG_TYPE_WRITE_TO_FILE)
	{
		fprintf (logContext->logFp, "[%ld:%ld]",tv.tv_sec,tv.tv_usec);
		vfprintf (logContext->logFp, format, args); /*Changed this to test LK*/
	}
	else if(logContext->logType EQ LOG_TYPE_SHARED_MEMORY)
	{
		UINT32 ctr = 0;
		switch(sectionType)
		{
			case ERR_SECTION:
				ctr = ++logContext->errCounter; 
				break;

			case CFG_SECTION:
				ctr = ++logContext->cfgCounter; 
				break;

			case CTR_SECTION:
				ctr = ++logContext->ctrCounter; 
				break;

			case INFO_SECTION:
				ctr = ++logContext->infoCounter; 
				break;

			default:
				break;

		}

		asprintf(&formatBuffer, "[%d][%ld:%ld]%s", ctr, tv.tv_sec,tv.tv_usec,format);
		dispatchLogToSharedMem(sectionType, args, formatBuffer);
	        if(formatBuffer NOTEQ NULL)
	        {
	                free(formatBuffer);
	        }
		/* An error should also be logged to info section */
		if (sectionType == ERR_SECTION)
		{
		        formatBuffer =  NULL;
			ctr = ++logContext->infoCounter; 
	                asprintf(&formatBuffer, "[%d][%ld:%ld]%s", ctr, tv.tv_sec,tv.tv_usec,format);
	                dispatchLogToSharedMem(INFO_SECTION, args, formatBuffer);
	                if(formatBuffer NOTEQ NULL)
	                {
	                        free(formatBuffer);
	                }
		}
	}
	va_end(args);
}

/**
 * @details 
 * This function writes the log with givenPriority into logFile, as per format
 */
void logMsgParams (
		const char *format, ...
		)
{
	LogContext  *logContext = getLogContext();
	va_list args;
	va_start(args, format);
	if(logContext->logType EQ LOG_TYPE_WRITE_TO_FILE)
	{
		vfprintf (logContext->logFp, format, args); /*Changed this to test LK*/
	}
	else if(logContext->logType EQ LOG_TYPE_SHARED_MEMORY)
	{
		dispatchLogToSharedMem(INFO_SECTION, args, format);
	}
	va_end(args);
}

/**
 * @details
 * This function writes the message In parameter into logFile, as per format
 */
void logMsgInParams (
		const char *format, ...
		)
{
	LogContext  *logContext = getLogContext();
	if(FALSE EQ (logContext->logConfig.logMask1 & LOG_MASK_MSGIN_02))
	{
		return;
	}
	va_list args;
	va_start(args, format);
	if(logContext->logType EQ LOG_TYPE_WRITE_TO_FILE)
	{
		vfprintf (logContext->logFp, format, args); /*Changed this to test LK*/
	}
	else if(logContext->logType EQ LOG_TYPE_SHARED_MEMORY)
	{
		dispatchLogToSharedMem(INFO_SECTION, args, format);
	}
	va_end(args);
}

/**
 * @details
 * This function writes the message In parameter into logFile, as per format
 */
void logMsgOutParams (
		const char *format, ...
		)
{
	LogContext  *logContext = getLogContext();
	if(FALSE EQ (logContext->logConfig.logMask1 & LOG_MASK_MSGOUT_02))
	{
		return;
	}
	va_list args;
	va_start(args, format);
	if(logContext->logType EQ LOG_TYPE_WRITE_TO_FILE)
	{
		vfprintf (logContext->logFp, format, args); /*Changed this to test LK*/
	}
	else if(logContext->logType EQ LOG_TYPE_SHARED_MEMORY)
	{
		dispatchLogToSharedMem(INFO_SECTION, args, format);
	}
	va_end(args);
}

/**
 * @details
 * This function checks whether LogApi is ENABLED or not.
 */

TBOOL islogApiEnabled(void) 
{
  UINT32  isWriteToLogEnabled=(getLogMask1() & LOG_MASK_API);
  if(FALSE EQ isWriteToLogEnabled)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}
