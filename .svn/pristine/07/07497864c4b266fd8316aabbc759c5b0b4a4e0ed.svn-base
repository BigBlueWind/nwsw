/**
 * @file    config.h
 * @brief All configurable/tunable parameters are set here.
 * @details Copyright Powerwave Technologies, Inc., 2009
 * Author, Date, Description:
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "bs_modules.h"
#include "module_names.h"
#include "msg_names.h"

/** Buffer size for messages recd from UDP */
#define SIZE_RECV_DATA            12000

/** Inter module communication (IMC) *****************************************/

/* Interval between successive calls to recvMsg() */
#define RECV_MSG_POLL_INTERVAL_MSEC 5

/* Max msgs that can be in a msg queue; also called length of msg que */
#define MAX_MSG_Q_LEN      30

/** The max number of instances of a msgId.
 *  The default value is set here.
 *  The instances can be set for each msgId individually.
 *  If you want to set the instances of a specific msgId to other than default,
 *   without affecting others, you can do than in msg.h
 */

#define DEFAULT_MAX_MSG_INST   20

/** UDP Socket Flags */
#define SOCK_FLAGS         O_NONBLOCK

#define BYTES_PER_ONE_MEGABYTE    ( 1024 * 1024 ) // 1 MB

/** TIMERS ********************************************************************/

/* max no of timers that can be started */
#define MAX_TIMER_INST            30

/* Parameters that affect logging */

/* ****** FRAMEWORK PARAMETERS ************* */
/* imc */

#define MAX_TOKENS_IN_STR    10
#define DEBUG                1
#define NULL_STR             "NULL_STR"
#define MAX_BUFFER_SIZE      1024


typedef String100   DataFromFileStr;

/*** ENB Limits config structure ********/
typedef struct s_EnbLimitsConfig
{
  UINT32 maxTraceFileSize;
  UINT32 maxCombinedPmLogFileSize;
}EnbLimitsConfig;

/* log */
#define MAX_LOG_FILENAME           100
/* Log files are created in write mode, if the log file exists its contents flushed */
/* TODO LOG_FILE_MODE has to be revisited for not truncating logs on restart*/
#define LOG_FILE_MODE          "a"

/* imc_sched */
#define MAX_MSG_PROCESSING_DURATION  5000  /* in Micro seconds */
#define Q_POLL_INTERVAL              2000  /* Rate of polling the receive Biffer, in Micro Seconds*/
#define INACCURACY_OF_SLEEPFUNC       100
#define MAX_SCHED_DELAY              ( MAX_MSG_PROCESSING_DURATION + Q_POLL_INTERVAL ) /* in clock ticks */

/*  Unix Domain Socket File Path Max Length */
#define UNIX_SOCK_PATH_MAX   108

#endif /*CONFIG_H*/
