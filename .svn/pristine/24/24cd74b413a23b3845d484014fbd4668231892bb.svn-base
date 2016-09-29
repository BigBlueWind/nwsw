/**
 * @file      cih_timers.c
 * @brief     This file contains Timer handling functions of CIH modules
 * @details   Copyright Powerwave Technologies, Inc., 2010
 *
 * @author    Madhu
 * @date      Oct 2010
 *
 *
 */

#include "cih.h"
#include "cih_tecla.h"

#define DUMMY_CIH_TIMER_USER_INFO    1

/**
 * @details          This function used to Initialise Commands Status Information Context
 */
void initCmdStatus()
{
  CihContext *ctx = getGlobalCihContext();
  ctx->isBatchFile     = FALSE;
  ctx->isRunScriptFile = FALSE;

  ctx->presentState = NULL;
  ctx->prevState    = NULL;

  clearBytes(&(ctx->cmdsAtTimerStart), sizeof(CmdStatus)*MAX_CMDS_IN_TIMER_UNIT);
  clearBytes(&(ctx->cmdsWaitingForRsp), sizeof(CmdStatus)*MAX_CMDS_IN_TIMER_UNIT);

  ctx->presentState =  getCihCmdsWaitingForRsp();
  ctx->prevState    =  getCihCmdsAtTimerStart();
}

/**
 * @details          This function is used to change Context of Global Commands Status
 *                   for Timers i.e used two pointers Pointing to 2 Structures that contains
 *                   Present and Previous context of commands with respect to timer start time.
 */
void changeTimersRspTable()
{
  CmdStatus   *temp = NULL;
  CihContext *ctx = getGlobalCihContext();

  temp = ctx->presentState;
  ctx->prevState->noOfCommands = 0;
  clearBytes(ctx->prevState, sizeof(CmdStatus));
  ctx->presentState = ctx->prevState;
  ctx->prevState    = temp;
}

/**
 * @param   cmdRef   contains the command Number and Instance Id
 * @details          This function is used to add entry to Status table after sending Request to BSM
 */
void addEntryToCmdStatus(
    IO CmdRef *cmdRef
    )
{
  CmdStatus  *cmdsWaitingForRsp = NULL;
  Index     i =0 ;
  ObjStr     *userCmdStr = NULL;

  cmdsWaitingForRsp = getPresentCmdsStatus();

  for(i=0 ; i < MAX_CMDS_IN_TIMER_UNIT ; i++)
  {
    if (cmdsWaitingForRsp->cmdsInfo[i].isRspExpected EQ FALSE) /* if Entry is Empty add new command information to CmdStatus */
    {
      cmdsWaitingForRsp->cmdsInfo[i].cmdRef.cmdRefNum  = cmdRef->cmdRefNum;
      cmdsWaitingForRsp->cmdsInfo[i].cmdRef.instanceId  = cmdRef->instanceId;
      cmdsWaitingForRsp->cmdsInfo[i].cmdRef.origin  = cmdRef->origin;

      userCmdStr = getUserCommand();
      strncpy((char*)&cmdsWaitingForRsp->cmdsInfo[i].userCmdStr, (char*)userCmdStr, strlen((char*)userCmdStr));
      cmdsWaitingForRsp->cmdsInfo[i].isRspExpected = TRUE;
      cmdsWaitingForRsp->noOfCommands ++;
      return;
    }
  }
}

/**
 * @param   cmdRef  Contains the command Number and Instance Id
 * @details         This function is used to update entry to Status table after receiving Response from BSM
 */
TBOOL removeEntryFromCmdStatus(
    IN CmdRef *cmdRef
    )
{
  CmdStatus   *cmdsWaitingForRsp = NULL;
  CmdStatus   *cmdsAtTimerStart = NULL;
  Index cmdsIndex = 0;

  cmdsWaitingForRsp = getPresentCmdsStatus();

  for(cmdsIndex = 0 ; cmdsIndex < MAX_CMDS_IN_TIMER_UNIT ; cmdsIndex++)
  {
    if (cmdsWaitingForRsp->cmdsInfo[cmdsIndex].cmdRef.cmdRefNum EQ cmdRef->cmdRefNum) /* empty */
    {
      if(cmdsWaitingForRsp->cmdsInfo[cmdsIndex].isRspExpected EQ TRUE)
      {
        cmdsWaitingForRsp->cmdsInfo[cmdsIndex].isRspExpected = FALSE;
        if(cmdsWaitingForRsp->noOfCommands > 0)
        {
          cmdsWaitingForRsp->noOfCommands --;
        }
        return TRUE; /* Returns TRUE if it founds command reference number otherwise late response */
      }
    }
  }

  cmdsAtTimerStart = getPreviousCmdsStatus();

  for(cmdsIndex = 0 ; cmdsIndex < MAX_CMDS_IN_TIMER_UNIT ; cmdsIndex++)
  {
    if (cmdsAtTimerStart->cmdsInfo[cmdsIndex].cmdRef.cmdRefNum EQ cmdRef->cmdRefNum) /* empty */
    {
      if(cmdsAtTimerStart->cmdsInfo[cmdsIndex].isRspExpected EQ TRUE)
      {
        cmdsAtTimerStart->cmdsInfo[cmdsIndex].isRspExpected = FALSE;
        if(cmdsAtTimerStart->noOfCommands > 0)
        {
          cmdsAtTimerStart->noOfCommands --;
        }
        return TRUE; /* Returns TRUE if it founds command reference number otherwise late response */
      }
    }
  }
  return FALSE; /* Late response from BSM */
}

/**
 * @param   timerType  Type of timer
 * @details            This function creates and starts RSP Timeout Timer.
 **/
void startCihRspTimer(
    IN TimerName         timerType
    )
{
  Index       timerIndex=0;
  CihTimersInfo      *timerInfo = getCihTimersInfo();

  for ( timerIndex = 0; timerIndex< timerInfo->totalTimersInMod; timerIndex++ )
  {
    if( timerType EQ timerInfo->modTimerInfo[timerIndex].timerDesc.timerType )
      break;
  }
  timerInfo->modTimerInfo[timerIndex].timerDesc.userInfo = DUMMY_CIH_TIMER_USER_INFO;

  /* Create and start Timer */
  timerInfo->modTimerInfo[timerIndex].timerId = createAndStartTimer( &timerInfo->modTimerInfo[timerIndex].timerDesc , NULL, NULL);
}

/**
 * @param   type  Type of timer
 * @details       This function is used to stop RSP Timeout Timer.
 **/
void  stopCihRspTimer(
    IO TimerName   type
    )
{
  CihTimersInfo    *timerInfo = getCihTimersInfo();
  Index           timerIndex;

  for ( timerIndex=0; timerIndex< timerInfo->totalTimersInMod; timerIndex++ )
  {
    if (timerInfo->modTimerInfo[timerIndex].timerDesc.timerType EQ type )
    {
      stopTimer( timerInfo->modTimerInfo[timerIndex].timerId );
      break;
    }
  }
}
/**
 * @param   cmdNumber        Command Number of Command For which Response is not received with in time
 * @param   cmdsAtTimerStart Commands Information that are waiting for RSP from BSM at timer start
 * @details                  This function used to Display TIMEOUT Indication to user
 */
void cihDisplayTimeoutMsgs(
    IN UINT32      cmdNumber,
    IO CmdStatus   *cmdsAtTimerStart
    )
{
  ObjStr   displayBuffer;
  clearBytes(&displayBuffer,sizeof(ObjStr));
  snprintf((UINT8*)&displayBuffer, sizeof(ObjStr),"\nTIMEOUT!! System did not respond to %s       [Command Reference No.:%d]\n", \
      (char*)&cmdsAtTimerStart->cmdsInfo[cmdNumber].userCmdStr, \
      cmdsAtTimerStart->cmdsInfo[cmdNumber].cmdRef.cmdRefNum
      );
  logInfo("%s",displayBuffer);
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}


/**
 * @param   msg   Message received
 * @details       This function will handle Timer expire message
 * */
void cihProcTimerExpiry(
    IN Msg           *msg
    )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;
  CmdStatus   *cmdsAtTimerStart = NULL;
  CihContext  *ctx = getGlobalCihContext();
  UINT32 cmdIndex = 0;

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITCMDRSP:
      {
        cmdsAtTimerStart = getPreviousCmdsStatus();

        /* If We are waiting for Response for any command  */
        for(cmdIndex = 0 ; cmdIndex < cmdsAtTimerStart->noOfCommands ; cmdIndex++)
        {
          /* Response Not received from BSM */
          if(TRUE EQ cmdsAtTimerStart->cmdsInfo[cmdIndex].isRspExpected)
          {
            cihDisplayTimeoutMsgs(cmdIndex, cmdsAtTimerStart);
            if(( ctx->isBatchFile EQ TRUE) || (ctx->isCmdLineMode EQ TRUE))
            {
              printAndEmptyErrorChain(&(getModuleContext()->errChain));
              exit(EXIT_SUCCESS);
            }
          }
        }
        copyBytes( &(ctx->cmdsWaitingForRsp), sizeof(CmdStatus), &(ctx->cmdsAtTimerStart));
        clearBytes(&(ctx->cmdsWaitingForRsp), sizeof(CmdStatus));
        /* Start Timer again */
        startCihRspTimer( TIMERNAME_WAITCMDRSP);
      }
      break;
    default:
      {
        BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, "NO-STATE-PRESENT" );
        break;
      }
  }
}

/**
 * @param   totalTimers total timers used in CIH module
 * @param   config      pointer to timer configuration structure with timer information
 * @details             This function is used to update entry to Status table after receiving Response from BSM
 */
void cihStoreTimersInCtx(
    IN UINT8         totalTimers,
    IN TimersConfig  *config
    )
{
  Index          i =0, timerArrayIndex = 0;
  InstanceId instanceId = getCihInstanceId();
  CihTimersInfo  *ctxTimerInfo = getCihTimersInfo();

  ModuleId    moduleId = MODID_NONE;
  MsgId       timeOutMsgId = MSG_UNDEFINED;

  if(instanceId EQ 1)
  {
    moduleId = MODID_CIH1;
    timeOutMsgId = MSG_TIMEKEEPER_CIH1_TIMEOUT_IND;
  }
  else if(instanceId EQ 2)
  {
    moduleId = MODID_CIH2;
    timeOutMsgId = MSG_TIMEKEEPER_CIH2_TIMEOUT_IND;
  }
  else if(instanceId EQ 3)
  {
    moduleId = MODID_CIH3;
    timeOutMsgId = MSG_TIMEKEEPER_CIH3_TIMEOUT_IND;
  }

  for ( i=0; i < totalTimers; i++ )
  {
    if ( config->moduleId EQ MODID_CIH1 )
    {
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.moduleId         = config->moduleId;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.duration = config->duration;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.timeUnit = config->timeUnit;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timerType         = config->timerName;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeoutMsgId      = timeOutMsgId;
      ctxTimerInfo->totalTimersInMod++;
      timerArrayIndex++;
    }
    config++;
  }
}

/**
 * @param   configFilePath This is the configuration path, either will be NULL / or some valid path
 * @details                This function will read the bs_timers Configuration file
 **/
void readCihTimersFromCfg(
    IN UINT8   *configFilePath
    )
{

  UINT8       isError = FALSE;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];

  GenericTable  *tableFunc = getTableFunctions(TIMERSCONFIG);
  TimersConfig  *config = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);
  clearBytes( &fullFilePath, sizeof(fullFilePath));
  constructFullPath ( configFilePath, "enb_pre_config.cfg", fullFilePath );

  if ( NULL EQ tableFunc->getReadFromFileFunc )
  {
    logError("%s","No getReadFromFileFunc for BSTIMERSCONFIG");
    exit(EXIT_FAILURE);
  }
  else
  {
    CihContext  *ctx = getGlobalCihContext();
    ctx->totalTimers = tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
    if ( isError EQ TRUE )
    {
      BS_ERROR_CONFIG_READ_FAILED()
        printAndEmptyErrorChain(&(getModuleContext()->errChain));
      exit(EXIT_FAILURE);
    }
    /* Copy CIH timers in CIH context */
    cihStoreTimersInCtx( ctx->totalTimers, config );
  }
}

