/**
 * @file    imc_unix.c
 * @brief   UNIX Domain Socket service @n
 *          This file holds the functions to create a Unix domain socket,
 *          Send the data on socket.
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Girish Vakati
 *
 * @date    July 04 2010 - Initial Coding
 *
 */

#include "imc_unix.h"
#include <errno.h>

/**
 * @param sockFd  Socket descriptor
 * @param udpPort The udp port to be used for socket creation
 * @param needToBind This flag tells whether to bind the socket or not,
 * If bind is true then we use specific port othe
 * @details This function creates the UDP socket. IF  needToBind is true then socket is
 * created on the port udpPort, else OS decides the port for socket.
 * We set needToBind to TRUE for recv type sockets
 */
TBOOL openUnixSocket(
    IN UINT8    *unixSockFilePath,
    IN TBOOL    needToBind,
    IO SINT32   *sockFd
  )
{
  struct sockaddr_un serv_addr;
  //int  val =  512 * 1024 ;  /* 512 kBytes  */
  int len;

  if ((*sockFd = socket(AF_UNIX, SOCK_DGRAM, 0)) EQ -1)
  {
    BS_ERROR_SOCK_CREATE_FAILED();
    return FALSE;
  }

  serv_addr.sun_family = AF_UNIX;

  if( 0 NOTEQ fcntl( *sockFd, F_SETFL, O_NONBLOCK) )
  {
    BS_ERROR_SET_SOCK_OPT_FAILED(O_NONBLOCK, errno );
    return FALSE;
  }
  if( TRUE EQ needToBind )
  {
    strncpy(serv_addr.sun_path, unixSockFilePath, 108);
    // Now bind the socket
    unlink(unixSockFilePath);
    logInfo("Binding to the UNIX DOMAIN SOCKET FILE = %s", unixSockFilePath);
    len = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    if (bind( *sockFd,(struct sockaddr *)&serv_addr, len) EQ -1)
    {
      BS_ERROR_SOCK_BIND(errno);
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * @param sendFd The socket descriptor to be used for sending data
 * @param ip The destination IP address
 * @param udpPort The destination UDP port
 * @param data The message that is to be sent
 * @param len The length of message
 * @details This function send the message to destination process, identified by ip
 *          and udpPort.
 *          This function Will free the message pointer
 */
SINT32 sendUnixSockMsg(
  IN SINT32 unixSendFd,
  IN UINT8  *unixSockFilePath,
  IN UINT8  *data,
  IN UINT16 len
  )
{
  SINT32 numBytes;
  struct sockaddr_un destUnixSockFile;

  /* Set the destination File Name */
  destUnixSockFile.sun_family = AF_UNIX;
  strncpy(destUnixSockFile.sun_path, unixSockFilePath, 108);

  numBytes = sendto(unixSendFd, data, len, SOCK_FLAGS, (struct sockaddr*)&destUnixSockFile, sizeof(struct sockaddr_un) );
  if( numBytes < len  || numBytes EQ -1)
  {
    logInfo("Fd = %d", unixSendFd);
    BS_ERROR_DATA_SENT_NOT_SAME(len, numBytes, errno );
  }
  return numBytes;
}

