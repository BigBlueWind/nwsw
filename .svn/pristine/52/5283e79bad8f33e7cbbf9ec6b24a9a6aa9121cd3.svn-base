/**
 * @file    logutil.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains functions to dump logs from shared memory
 *
 * @author     : Ramprasad Reddy
 * Date       :  28-Jul-2011
 * Change History: Basic version
 *
 */

#include "libimc/log.h"
#include "logutil.h"
#include "shm.h"

/**
 * @details Self-explanatory
 */
UINT8* errString = "*************************** START OF ERROR SECTION ****************************\n";
UINT8* ctrString = "************************** START OF COUNTER SECTION ***************************\n";
UINT8* cfgString = "*********************** START OF CONFIGURATION SECTION ************************\n";
UINT8* infoString ="*************************** START OF DEBUG SECTION ****************************\n";
UINT8* cmnString = "\n******************************* END OF SECTION *******************************\n";

void printHelp(void)
{
	fprintf(stderr, "%s %s", "Please use the following format for different commands\n", 
			"The logs will be put in inst_A/log or inst_B/log directory whichever is active\n");
	fprintf(stderr, "%s", "logutil\n");
	fprintf(stderr, "%s", "logutil help\n");
}

/**
 * @param logFilename FileName
 * @details This function returns pointer to file
 */
FILE* openFile(
		IN FileName logFileName
		)
{
	FILE *fp = NULL;

	if(NULL NOTEQ (fp = fopen(logFileName, "a+")))
	{
		setvbuf( fp, NULL, _IONBF /*_IOLBF*/, 80);
	}

	return fp;
}

/**
 * @param fp Pointer to FILE
 * @details This function closes file.
 */
void closeFile(
		FILE *fp
		)
{
	if(NULL NOTEQ fp)
	{
		fclose(fp);
	}
}

/**
 * @param argc Number of arguments
 * @param argv[] pointers to arguments
 * @details This function writes to files the data contained in shared memory.
 */
void writeAllLogs (int argc, char *argv[])
{
	FILE            *logFilePtr = NULL;
	FileName         logFileName;
	ExeType          exeType;
	LogType          logType;
	CircularBuffer* cBuf = NULL;
	CircularBuffers cBufs;
	SINT32 cBufSize = 0;

	UNUSED(argc);
	UNUSED(argv);

	logType = getLogType();

	if(LOG_TYPE_SHARED_MEMORY EQ logType)
	{
		for(exeType=BSM_EXE;exeType<MAX_EXE;exeType++)
		{
			clearBytes(&cBufs, sizeof(cBufs));
			/*
			 * Attach to shared memory of that exe and write log to file
			 */
			if(attachToSharedMem(exeType, &cBufs) NOTEQ FALSE)
			{
				clearBytes(logFileName, sizeof(logFileName));
				sprintf(logFileName, "%s/log/%s.log", cBufs.partition, getExeNameByExeType(exeType)); 
				if(NULL NOTEQ (logFilePtr = openFile(logFileName)))
				{

					SectionType sectionType = ERR_SECTION;
					while (sectionType < MAX_SECTIONS)
					{
						switch(sectionType)
						{
							case ERR_SECTION:
								cBuf = cBufs.errBuffer;
								fwrite(errString, 1, 80, logFilePtr);
								break;
							case CTR_SECTION:
								cBuf = cBufs.ctrBuffer;
								fwrite(ctrString, 1, 80, logFilePtr);
								break;
							case CFG_SECTION:
								cBuf = cBufs.cfgBuffer;
								fwrite(cfgString, 1, 80, logFilePtr);
								break;
							case INFO_SECTION:
								cBuf = cBufs.infoBuffer;
								fwrite(infoString, 1, 80, logFilePtr);
								break;
							default:
								break;
						}
						
						cBufSize = getSizeByExeAndSectionTypes(exeType, sectionType);
						readFromCircularBuffer(cBuf, logFilePtr, cBufSize);
						fwrite(cmnString, 1, 80, logFilePtr);
						sectionType++;
					}
					closeFile(logFilePtr);
					fprintf(stderr, "Created log %s for exe=%s\n", logFileName, getExeNameByExeType(exeType));
				}
				else
				{
					fprintf(stderr, "ERROR: Log file could not be opened for partition =%s, exe=%s, logfilename=%s\n", cBufs.partition, getExeNameByExeType(exeType), logFileName);
					return;
				}
			}
			else
			{
				fprintf(stderr, "Attaching to shared memory failed for exe=%s\n",getExeNameByExeType(exeType));
				return;
			}
		}
	}
}

/**
 * @param argc Number of arguments
 * @param argv[] pointers to arguments
 * @details This function is the entry point.
 */
int main (int argc, char *argv[])
{
	if(argc EQ 1)
  {
    writeAllLogs(argc, argv);
  }
  else
  {
    printHelp();
  }
	exit(EXIT_SUCCESS);
}
