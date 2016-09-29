/**
 * @file    imc_sig.h
 * @brief Description of all messages in the system
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author : Lakshmi Narayan Madala
 * Date   :
 * Description :
 */

#ifndef IMC_SIG_H
#define IMC_SIG_H

#include <sys/types.h>
#include <signal.h>
#include <string.h> /*strsignal*/
#include "log.h"

#define NO_PROCESS_EXISTS       -2

void setSigActions(void);
pid_t waitForChld( pid_t pid);
SINT32 killChld( pid_t pid, SINT32 sig);

#endif /* IMC_SIG_H */


