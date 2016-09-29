/**
 * @file    imc_sched.c
 * @brief   Each module has a message/event handler.
 * @brief   This file contains a scheduler that invokes the module's
 *          message handler when a message is received at the recv buffer.
 *
 * @details Copyright PowerWave Technologies, Inc., 2009
 * @author  Manu Sharma
 */

#include <time.h>
#include "imc_sched.h"

/**
 * @param modId The module where the message is received, this is used to identify the dest queue.
 * @param msgHandler The function to invoke after a message is received at the queue.
 * @details This function is a runs forever, and is the last function called from main.
 * This function looks at recv buffer of a process if a message is found then msgHandler is invoked
 * else goes to sleep for a duration of MAX_MSG_PROCESSING_DURATION,
 * this approach stops a process from continuously hogging on to CPU.
 */
void schedMsgHandler(
  IN ModuleId   modId,
  IN MsgHandler msgHandler
)
{
  Msg     *msgIn;
  MsgId    msgId;
  UINT16   ec;
  fd_set   masterReadFdSet;
  fd_set   readFdSet;
  int      retVal = 0;
  struct timeval timeAtStart,timeAtMsgIn,timeAtMsgOut,msgProcDuration;

  ModAddr *modAddr = getModAddr(modId);
  /* Creating readFdSet */
  IMC_INET_FD_ZERO( &masterReadFdSet );
  IMC_INET_FD_SET( modAddr->recvFd,    &masterReadFdSet);

  while(1)
  {
    gettimeofday (&timeAtStart, NULL);
    /* Initialising readFdSet. Needed Init after every select() call */
    readFdSet = masterReadFdSet;
    /* Waiting for any message received on its port */
    retVal = select(65000 ,&readFdSet,NULL,NULL,NULL);
    if (-1 EQ retVal)
    {
      if (errno EQ EINTR )
      {
        continue;
      }
      logError("Error in selecting the FDs: selectRetVal=%d, errno =%d", retVal, errno);
    }
    else
    {
      /* Handle if any Messages received on its port */
      if (IMC_INET_FD_ISSET(modAddr->recvFd, &readFdSet))
      {
        while((msgIn = recvMsg(modId, &ec)))
        {
          gettimeofday (&timeAtMsgIn, NULL);
          msgId = msgIn->msgId;
          msgHandler(msgIn);
          gettimeofday (&timeAtMsgOut, NULL);
          getLapsedTime(&timeAtMsgIn, &timeAtMsgOut, &msgProcDuration);
          if(msgProcDuration.tv_usec > MAX_MSG_PROCESSING_DURATION)
          {
            BS_ERROR_MSG_PROC_DURAT_EXCEEDED( msgId, msgProcDuration.tv_usec );
          }
        }
      }
    }
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
  }
}
