/**
 * @file    fth_main.c
 * @brief This file is the starting point for FileTransferAgent module
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Lakshmi Narayana Madala
 * @date    15-Jul-2010
 */

#include "fth.h"

/* ################### FTH Context ############### */
FthContext    g_FthContext;


/**
* @details
* This functions returns the FTH Context, It is stored as a global variable
*/
FthContext    *getFthContext()
{
  return (&g_FthContext);
}

/**
* @param msgIn  message received
* @details
* This function is the starting point of FTH processing,
* AS FTP is a blocking call, we can't process any messages received while file transfer is in progress
* libCURL provides a callback, which is called when there is data from FTP, In the callback we
* check whether we received any message, As same code is being used at fthMain() and in callback, we moved the code
* to a separate function fthReadAndProcessRecvMsg.
*/
void fthMain(
  FR Msg *msgIn
  )
{
  fthReadAndProcessRecvMsg(msgIn);
}
