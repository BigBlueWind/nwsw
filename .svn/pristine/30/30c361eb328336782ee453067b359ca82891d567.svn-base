/**
 * @file    start_indication.c
 * @brief Description
 * This file contains function to send START_IND message to PRM
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author : Ramprasad Reddy
 * Date   : 26-May-2011
 * Description : 1. Basic Version
 */

#include "start_indication.h"
#include "imc.h"

void sendStartIndToPrm(
    IN ModuleId moduleId,
    IN MsgId    msgId
    )
{
  MsgStartInd   msgStartInd;

  clearBytes(&msgStartInd, sizeof(MsgStartInd));
  msgStartInd.svnVersion = getSvnVersion(moduleId);
  createAndSendMsg( msgId, (UINT8 *)&msgStartInd, sizeof(MsgStartInd));
}
