/**
 * @file    imc_module.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief   This file contains the functions definitions for registering a module with framework
 *          and de-registering the module from framework
 *
 * @author  Lakshmi Narayan Madala
 *
 * @details Initial coding done - Lakshmi @n
 *          imc.cfg is added   07-06-2010 - Madhu
 */

#include "msg_desc.h"

ModContext g_modContext; /* Global variable, The context of a module, this contains the the send
                           sendFileDescriptor, recvFileDescriptor and context info of a process */
/**
 * @details This function returns the global context of a process
 */
ModContext *getModuleContext()
{
  return (&g_modContext);
}

/** @defgroup imc_api_fun Framework API Functions
 * @param   modId   The module is identified by this identifier in framework
 * @param   exeName The program to exit
 * @details This is the last function to be executed. This removes the module from framework
 * @{
 */
void  modExit(
  IN ModuleId  modId,
  IN UINT8     *exeName
)
{
  logInfo("EXIT exe=%s, ModuleId=%d, PID=%u", exeName, modId, getpid());
  deregisterForImc( modId );
  /* Reset the counters and disable the Counters Flag*/
  if(TRUE EQ msgCntrsRunning)
  {
    msgCntrsRunning = FALSE;
    resetSendMsgCntrs();
    resetSendMsgErrCntrs();
    resetRcvdMsgCntrs();
    resetRcvdMsgErrCntrs();
  }
  logClose( getBasename(exeName) );
}

/******************************************************/
/**
 * @param   modId            The module is identified by this identifier in framework
 * @param   exeName          The program to be executed
 * @param   configFilePath   Configuration file path given by user
 * @param   tick             Timers will be a multiple of this duration.
 * @details This function registers a module with framework, and starts the timer service
 * as per user request. This functions registers for logging service
 */
TBOOL modInit(
  IN ModuleId  modId,
  IN UINT8    *exeName,
  IN UINT8    *rootDir,
  IN TimeSpec  tick
  )
{
  TBOOL        retval;
  ModContext  *context = getModuleContext();
  UINT8       *exeBaseName = getBasename(exeName);
  LogContext  *logContext  = getLogContext();

  UINT8       fullFilePath[MAX_BUFFER_SIZE] ;

  memset( &context->errChain, 0, sizeof(ErrChain));
  memset( &fullFilePath, 0, sizeof(fullFilePath));

  /* rootDir Path is : PICOBS_HOME/rel/inst_A
   **/
  /* Read from the config file only for BSM & CIH modules */
  
  if( (modId EQ MODID_CIH1) || (modId EQ MODID_CIH2) || (modId EQ MODID_CIH3) )
  {
    if(FALSE EQ readLogConfigTableAndUpdateLogCtx(modId, rootDir, &(logContext->logConfig)))
    {
       exit(EXIT_FAILURE);
    }
  }

  /* In case of PRM, logInit is done outside this funtion.
   * Reason: PRM logs are expected to be created under PICOBS_HOME/log
   *         For all other Modules logs are created under PICOBS_HOME/rel/inst_A/log
   **/
 
  if ( (modId NOTEQ MODID_PRM ) && (modId NOTEQ MODID_CIH1) && (modId NOTEQ MODID_CIH2) && (modId NOTEQ MODID_CIH3) )
  {
    logInit( exeBaseName, rootDir );
  }
  initMsgDescTable( modId);
  constructFullPath ( rootDir, "/config/imc.cfg", fullFilePath );
  initImcContext(fullFilePath);
  retval = registerForImc( modId );
  if( (retval EQ TRUE) && ( (modId EQ MODID_CIH1) || (modId EQ MODID_CIH2) || (modId EQ MODID_CIH3) ) )
  {
    logInit( exeBaseName, rootDir );
  }
  sprintf(&context->exeName[0], exeBaseName);
  logInfo("INIT exe=%s, ModuleId=%d, PID=%u", exeName /*let it be exeName */, modId, getpid());
  setSigActions();
  
  if( tick.duration NOTEQ 0 )
  {
    timerServiceInit(tick); /* Register for Timer services */
  }
  return retval;
}
/** @} */






