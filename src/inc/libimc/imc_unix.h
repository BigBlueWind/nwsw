/**
 * @file    imc_unix.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief   Unix Domain Socket Functions
 *
 * @author  Girish Vakati
 *
 * @date    July 04 2010 - Initial Coding
 *
 *
 */

#ifndef IMC_UNIX_H
#define IMC_UNIX_H

/* Local Includes */
#include "imc_timer.h"
#include "log.h"
#include "imc_module.h"
#include "imc_err.h"

#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

TBOOL openUnixSocket(
  IN UINT8    *unixSockFilePath,
  IN TBOOL    needToBind,
  IO SINT32   *sockFd
  );

SINT32 sendUnixSockMsg(
  IN SINT32 sendFd,
  IN UINT8  *unixSockFilePath,
  IN UINT8  *data,
  IN UINT16 len
  );

#endif /* IMC_UNIX_H*/


