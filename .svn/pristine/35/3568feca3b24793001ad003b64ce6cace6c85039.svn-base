/**
 * @file    logutil.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief   This file contains data structures and macros for logging to shared memory usage
 *
 * @author : RampRasad Reddy
 * Date   :  28-jul-2011
 * Description : Initial version
 *
 */

#ifndef LOGUTIL_H_
#define LOGUTIL_H_

#include "basic_types.h"
#include "bs_config.h"

#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>

void printHelp(void);

FILE* openFile(
    IN FileName logFiileName
    );

void closeFile(
    FILE *fp
    );

void writeAllLogs (
    int argc, char* argv[]
    );

#endif /* LOGUTIL_H_ */
