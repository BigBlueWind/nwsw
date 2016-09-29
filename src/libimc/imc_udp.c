/**
 * @file    imc_udp.c
 * @brief   UDP service @n
 *          This file holds the functions to create a UDP socket,
 *          Send the data on socket.
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Manu Sharma
 * @author  lmadala
 *
 * @details Initial Coding
 * @details Shweta (08th June 2010) : Increased the size of the UDP buffer.
 *
 */

#include "imc_udp.h"
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
TBOOL openUdpSocket(
  IN UINT16   udpPort,
  IN TBOOL    needToBind,
  IO SINT32   *sockFd
  )
{
  struct sockaddr_in serv_addr;
  int  val =  512 * 1024 ;  /* 512 kBytes  */
  int getval=0;

  UINT32 len = sizeof(val);

  if ((*sockFd = socket(AF_INET, SOCK_DGRAM, 0)) EQ -1)
  {
    BS_ERROR_SOCK_CREATE_FAILED();
    return FALSE;
  }


  if( 0 NOTEQ fcntl( *sockFd, F_SETFL, O_NONBLOCK) )
  {
    BS_ERROR_SET_SOCK_OPT_FAILED(O_NONBLOCK, errno );
    return FALSE;
  }
  if( TRUE EQ needToBind )
  {
    //logInfo("Opened socket fd=%d, Port=%d", *sockFd, udpPort);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(udpPort);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    memset( &(serv_addr.sin_zero), 0, 8);
    // Now bind the socket
    logInfo("Binding To The UDP Port = %d", udpPort);
    if (bind( *sockFd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) EQ -1)
    {
      BS_ERROR_SOCK_BIND(errno);
      return FALSE;
    }
  }

  /* This is done to increase the size of the buffer */
  if ( setsockopt( *sockFd, SOL_SOCKET, SO_SNDBUF, &val, len) NOTEQ 0 )
  {
    BS_ERROR_SET_SOCK_OPT_FAILED(SO_SNDBUF, val);
    perror("setsockopt");
    return FALSE;
  }
//  else logInfo("Set SO_SNDBUF for fd=%d, port=%d, to %d bytes: OK", *sockFd, udpPort, val);

  if ( setsockopt( *sockFd, SOL_SOCKET, SO_RCVBUF, &val, len) NOTEQ 0 )
  {
    BS_ERROR_SET_SOCK_OPT_FAILED(SO_RCVBUF, val);
    perror("setsockopt");
    return FALSE;
  }
//  else logInfo("Set SO_RCVBUF for fd=%d, port=%d, to %d bytes: OK", *sockFd, udpPort, val);

  /* Code to check whether the recvBuffer size is set to required size
   * value returned by getsockopt is double of the value used in setsockopt
   * kernel keeps this extra space for book keeping
   * Ex: is 1000 bytes is set through setsockopt, getsockopt returns 2000 bytes
   */
  int retval;
  retval = getsockopt( *sockFd, SOL_SOCKET, SO_RCVBUF, &getval, &len);
  if( val NOTEQ (getval/2) )
  {
    fprintf(stderr,"Cannot set the receive buffer to value:%d\n", val );
    fprintf(stderr,"Increase the buffer limit and run again; as a root user, run command to set buffer: sysctl net.core.rmem_max=%d\n", val);
    exit(1);
  }
  return TRUE;
}

/**
 * @param sockFd Socket descriptor
 * @param udpPort Destination Port address
 * @param ipAddr Destination IP Address
 * @details This function connects the socket to the destination IP Address
 * By connecting a UDP socket we can directly use send() and recv() routines
 * rather than sendto() and recvfrom(), which avoids the overhead of passing destination arguments
 * every time
 */
void connectUdpSocket(
  IN SINT32 sockFd,
  IN UINT16 udpPort,
  IN UINT8  *ipAddr
)
{
  struct sockaddr_in serv_addr;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons (udpPort);
  if( NULL NOTEQ ipAddr ) inet_aton ( ipAddr, &serv_addr.sin_addr);
  memset( &(serv_addr.sin_zero), 0, 8);

  if (connect( sockFd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) NOTEQ -1)
  {
    BS_ERROR_SOCK_CONNECT();
    exit(EXIT_FAILURE);
  }
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
SINT32 sendUdpMsg(
  IN SINT32 udpSendFd,
  IN UINT8  *ip,
  IN UINT16 udpPort,
  IN UINT8  *data,
  IN UINT16 len
  )
{
  SINT32 numBytes;
  struct sockaddr_in destIpAddr;

  /* Set the destination address */
  destIpAddr.sin_family = AF_INET;
  destIpAddr.sin_port = htons (udpPort);
  inet_aton (ip, &destIpAddr.sin_addr);
  memset( &(destIpAddr.sin_zero), 0, 8);

  numBytes = sendto(udpSendFd, data, len, SOCK_FLAGS, (struct sockaddr*)&destIpAddr, sizeof(struct sockaddr) );
  if( numBytes < len  || numBytes EQ -1)
  {
    BS_ERROR_DATA_SENT_NOT_SAME(len, numBytes, errno );
  }
  return numBytes;
}

