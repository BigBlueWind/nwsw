/**
 * @file    log.h
 * @brief Logging and tracing functions
 * @details Copyright Powerwave Technologies, Inc., 2009
 * There shall be only two log levels, Error and Info which can be used.
 * @author : Lakshmi Narayan Madala
 * Date   :
 * Description :
 */

#ifndef LOG_H
#define LOG_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "cmn_types.h"
#include "bs_config.h"
#include <stdarg.h>
#include "shm.h"

#define LOG_MASK_ERROR            0x00000001
#define LOG_MASK_DEBUG            0x00000002
#define LOG_MASK_PROCEDURE        0x00000004
#define LOG_MASK_TRANSACTION      0x00000008
#define LOG_MASK_COUNTER          0x00000010
#define LOG_MASK_STATE            0x00000020
#define LOG_MASK_CONFIGURATION    0x00000040
#define LOG_MASK_OS               0x00000080
#define LOG_MASK_TIMER            0x00000100
#define LOG_MASK_TIMEOUT          0x00000200
#define LOG_MASK_MSGIN_01         0x00000400
#define LOG_MASK_MSGIN_02         0x00000800
#define LOG_MASK_MSGIN_03         0x00001000
#define LOG_MASK_MSGOUT_01        0x00002000
#define LOG_MASK_MSGOUT_02        0x00004000
#define LOG_MASK_MSGOUT_03        0x00008000
#define LOG_MASK_FUNCTION         0x00010000
#define LOG_MASK_PROFILE          0x00020000
#define LOG_MASK_EVENTIN					0x00040000
#define LOG_MASK_EVENTOUT					0x00080000
#define LOG_MASK_API              0x00100000

#define LOG_ON_STDOUT             "stdout"

typedef struct s_logContext
{
  FILE*              logFp;
  UINT8              logFilename[MAX_LOG_FILENAME];
  UINT8              exeName[MAX_LOG_FILENAME];
  ExeType            exeType;
  LogType            logType;
  LogConfig          logConfig;
  CircularBuffers    circularBuffers;
	UINT32             errCounter;
	UINT32             cfgCounter;
	UINT32             ctrCounter;
	UINT32             infoCounter;
} LogContext;

/* Global variable, The log context of a module, this contains the log file name
 **/
extern LogContext g_logContext;

/*This macro returns the logging context of a process, a reference of this maintained in module context */
#define getLogContext() (&g_logContext)

#define getLogMask1() (g_logContext.logConfig.logMask1)

typedef LogConfig  SetLogConfig;


#ifdef SHM_SUPPORTED 
 #define getLogType()  LOG_TYPE_SHARED_MEMORY
#else
 #define getLogType()  LOG_TYPE_WRITE_TO_FILE
#endif

TBOOL readLogConfigTableAndUpdateLogCtx(
  IN ModuleId    modId,
  IN UINT8      *rootDir,
  IO LogConfig  *ptrLogConfig
  );

void logInit(
 IN UINT8    *exeName,
 IN UINT8    *logDir
 );

void logClose(
  IN UINT8 *exeName
);
void logMsgParams (
    const char *format, ...
    );

void logMsgInParams (
    const char *format, ...
    );

void logMsgOutParams (
    const char *format, ...
    );

void logWriteWithPriority (
		IN UINT32  isWriteToLogEnabled,
		IN SectionType sectionType,
		const char *format, ...
		);

void dispatchLogToSharedMem (
		IO SectionType sectionType,
		IN va_list args,
		const char *format, ...
		);

BsErrorValue setLogConfig(
    IN SetLogConfig *setLogConfig
    );

#define logToFile(fp, logKind, file, line, format, ...) \
do { struct timeval tv; gettimeofday(&tv, NULL); \
    fprintf(fp, "[%ld:%ld][%s][%s][%u]["format"]\n", tv.tv_sec, tv.tv_usec, logKind, file, line, __VA_ARGS__);\
   } while (0);

TBOOL islogApiEnabled(void) ;

#define logError(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_ERROR), ERR_SECTION,        "[ERR:%s:%d:"format"]\n",    __FILE__, __LINE__, __VA_ARGS__ )

#define logInfo(format, ...)  \
  logWriteWithPriority((getLogMask1() & LOG_MASK_DEBUG), INFO_SECTION,         "[INFO:%s:%d:"format"]\n",   __FILE__, __LINE__, __VA_ARGS__ )

#define logProc(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_PROCEDURE), INFO_SECTION,   "[PROC:"format"]\n", __VA_ARGS__ )

#define logTrans(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_TRANSACTION), INFO_SECTION,   "[TRANS:%s:%d:"format"]\n",  __FILE__, __LINE__, __VA_ARGS__ )

#define logCtr(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_COUNTER),CTR_SECTION,       "[CTR:%s:%d:"format"]\n",    __FILE__, __LINE__, __VA_ARGS__ )

#define logStateChange(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_CONFIGURATION), CFG_SECTION,         "["format"]\n", __VA_ARGS__ )

#define logState(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_STATE),INFO_SECTION,         "[STATE:%s:%d:"format"]\n",  __FILE__, __LINE__, __VA_ARGS__ )

#define logConfig(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_CONFIGURATION), CFG_SECTION, "[CONFIG:%s:%d:"format"]\n", __FILE__, __LINE__, __VA_ARGS__ )

#define logOs(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_OS),INFO_SECTION,            "[OS:%s:%d:"format"]\n",     __FILE__, __LINE__, __VA_ARGS__ )

#define logTmr(format, ...)  \
  logWriteWithPriority((getLogMask1() & LOG_MASK_TIMER),INFO_SECTION,         "[TMR:%s:%d:"format"]\n",    __FILE__, __LINE__, __VA_ARGS__ )

#define logTimeout(format, ...)  \
  logWriteWithPriority((getLogMask1() & LOG_MASK_TIMEOUT),INFO_SECTION,       "[TIMEOUT:%s:%d:"format"]\n",__FILE__, __LINE__, __VA_ARGS__ )

#define logFunc(format, ...)  \
  logWriteWithPriority((getLogMask1() & LOG_MASK_FUNCTION),INFO_SECTION,      "[FUNC:%s:%d:"format"]\n",   __FILE__, __LINE__, __VA_ARGS__ )

#define logProfile(format, ...)  \
  logWriteWithPriority((getLogMask1() & LOG_MASK_PROFILE),INFO_SECTION,       "[PROFILE:%s:%d:"format"]\n",__FILE__, __LINE__, __VA_ARGS__ )

#define logMsgOut(format, ...)  \
  logWriteWithPriority(((getLogMask1() & LOG_MASK_MSGOUT_02) || (getLogMask1() & LOG_MASK_MSGOUT_01)),INFO_SECTION, "[MSG:OUT:%s:%d:"format"]\n", __FILE__, __LINE__, __VA_ARGS__ )

#define logMsgIn(format, ...)  \
  logWriteWithPriority(((getLogMask1() & LOG_MASK_MSGIN_02) || (getLogMask1() & LOG_MASK_MSGIN_01)),INFO_SECTION, "[MSG:IN:%s:%d:"format"]\n", __FILE__, __LINE__, __VA_ARGS__ )

#define logEventIn(pst, state, format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_EVENTIN), INFO_SECTION, "[EVT-IN:%s:%d |event: %d |to: %d |in-state: %d |from: %d |"format"]\n", __FILE__, __LINE__, pst->event, pst->dstEnt, state, pst->srcEnt, __VA_ARGS__ );

#define logEventOut(pst, state, format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_EVENTOUT), INFO_SECTION, "[EVT-OUT:%s:%d |event: %d |from: %d |in-state: %d |to: %d |"format"]\n", __FILE__, __LINE__, pst->event, pst->srcEnt, state, pst->dstEnt, __VA_ARGS__ );

#define logFatalErrorAndExit(errCode, format, ...) \
  do {\
    logWriteWithPriority( TRUE /* Fatal Error should always be logged */,ERR_SECTION, "[ERR:FATAL %d:%s:%d:"format"]\n", errCode, __FILE__, __LINE__, __VA_ARGS__ );\
    fprintf(stderr, "[ERR:FATAL %d:%s:%d:"format"]\n", errCode, __FILE__, __LINE__, __VA_ARGS__ );\
    exit(errCode);\
  } while(0);

#define logAPI(format, ...) \
  logWriteWithPriority((getLogMask1() & LOG_MASK_API),INFO_SECTION,         "[API:%s:%d:"format"]\n",    __FILE__, __LINE__, __VA_ARGS__ )

  #endif
