/**
 * @file    imc_udp.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief User Datagram Protocol functions
 *
 * @author : Manu Sharma
 * Date   :
 * Description
 *
 */

#ifndef IMC_UDP_H
#define IMC_UDP_H

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

TBOOL openUdpSocket(
  IN UINT16   udpPort,
  IN TBOOL    needToBind,
  IO SINT32   *sockFd
  );

SINT32 sendUdpMsg(
  IN SINT32 sendFd,
  IN UINT8  *ip,
  IN UINT16 udpPort,
  IN UINT8  *data,
  IN UINT16 len
  );

#endif /* IMC_UDP_H*/


