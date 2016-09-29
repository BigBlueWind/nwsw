/**
 *
 * @file    imc_sched.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * 1. A function called schegMsgHandler() is defined, This function shall invoke
 *   message handling of modules (such as asnMain, bsmMain).
 *   Example:
 *     int main(int argc, char *argv[])    {
 *               .....
 *               status = modInit( MODID_BSM, exeName );
 *               other stuff here
 *               schedMsgHandler(MODID_BSM, bsmMain); }
 * 2. recvMsg() is called inside schedMsgHandler().
 *    If a message is received for the module, schedMsgHandler()invokes the
 *    module�s message handling function (bsmMain() in the above example).
 *    otherwise, it calls usleep().
 * 3. Since recvMsg() is called in schedMsgHandler(), it MUST NOT be called
 *    inside bsmMain(). Thus bsmMain() is declared as void bsmMain(Msg *msg)
 * The advantages of the above approach are:
 * 1. usleep() is not cluttering main() of modules.
 * 2. Scheduling of process and giving control back to OS is now at a single place (schedMsgHandler()).
 * 3. Msg processing duration can be obtained and benchmarked.
 *
 * @author : Manu Sharma
 * Date   :
 * Description :
 */

#ifndef IMC_SCHED_H
#define IMC_SCHED_H

#include "all_cmn.h"
#include <sched.h>

/* ************************************************************
 * TYPES
 */
typedef void (*MsgHandler)(FR Msg* msg);

/* ************************************************************
 * API
 */

void schedMsgHandler(
  IN ModuleId   modId,
  IN MsgHandler msgHandler
);

/* macros to manipulate and checking a socket file descriptor set */
#define IMC_INET_FD_SET(_sockFd, _fdSet)    FD_SET((_sockFd), _fdSet)
#define IMC_INET_FD_CLR(_sockFd, _fdSet)    FD_CLR((_sockFd)->fd, _fdSet)
#define IMC_INET_FD_ISSET(_sockFd, _fdSet)  FD_ISSET((_sockFd), _fdSet)
#define IMC_INET_FD_ZERO(_fdSet)            FD_ZERO(_fdSet)


#endif /* IMC_SCHED_H */
