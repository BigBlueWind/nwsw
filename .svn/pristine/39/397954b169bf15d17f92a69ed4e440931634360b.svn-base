/**
 * @file    bsm.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file is the starting point for Base Station Manager (BSM) module.
 *
 * @author:  Manu Sharma / Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version.
 *           Manu Sharma, 18May2010 :
 *          a) Added command line arg --version.
 *             When an executable is started with this option, the version will be displayed and
 *             executable will exit.
 *          b) command line arg is handled before modInit.
 **/

#include "bsm.h"

/**
 * @param argc  Argument Count
 * @param argv  Argument List
 * @details     Main funtion - Entry point for BSM Module.
 */
int main(
  int argc,
  char *argv[]
  )
{
  UINT8        status;
  UINT8       *exeName = argv[0];
  TimeSpec     tick = { TIMEUNIT_SEC, 1 };
  BsmContext   *ctx = getBsmContext();
  String512     cmdStr;
  initBsmContext( ctx );
  /* Clear Error chain buffer */
  clearBytes( getModuleContext(), sizeof(ModContext) );
  clearBytes( cmdStr, sizeof(String512) );

  copyBytes( (char*)argv[2], sizeof(FilePath), &ctx->rootDir[0] );
  snprintf( &ctx->traceDir[0], sizeof(FilePath), "%s/trace/", &ctx->rootDir[0] );
  snprintf( ctx->afhCtx.alarmLogFilePathAndName, sizeof(FilePath), "%s/log/alarmLog.log", &ctx->rootDir[0] );

  alarmInitCtx( &ctx->afhCtx );
  fillGenericFunctionTable();
  emptyAllTables();

  /* Handle command line arguments before modInit.
   * This allows a user to know the version of the executable even if modInit fails.
   */
  cmdLineParamHandler( argc, argv, &ctx->configDir[0], &ctx->versionInfo );

  if(ctx->configDir[0] NOTEQ '\0')
 {
    snprintf( cmdStr, sizeof(String512), "cp -f %s/enb_user_config.cfg %s/enb_user_running.cfg", &ctx->configDir[0], &ctx->configDir[0] );
    if( 0 NOTEQ system(cmdStr) )
    {
       BS_ERROR_SYSTEM_COMMAND_FAILED(cmdStr);
       exit(FALSE);
    }

    /* Call the function to load configuration from files to global variables */
    if(FALSE EQ bsmLoadCfgFromFiles( &ctx->configDir[0] ))
    {
      BS_ERROR_CONFIG_READ_FAILED();
      printAndEmptyErrorChain(&(getModuleContext()->errChain));
      exit(EXIT_CONFIG_READ_FAIL);
    }
  }
  else
  {
    printf( "Enter Configuration file path !! --configPath <path>\n" );
    exit(EXIT_CONFIG_PATH_NOT_SET);
  }

  BsmOamTableContext *oamCtx = getBsmOamTableContext();
  if( FALSE EQ bsmParseOamCfg( &ctx->configDir[0], oamCtx ) )
  {
    BS_ERROR_CONFIG_READ_FAILED();
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
    exit(EXIT_CONFIG_READ_FAIL);
  }

  status = modInit( MODID_BSM, exeName, (char*)argv[2], tick);
  if (status EQ TRUE)
  {
    /* Store the first time info */
    ctx->moduleStartTime = getCurrentTime();
    sendStartIndToPrm( MODID_BSM, MSG_BSM_PRM_START_IND );
  }
  else
  {
    BS_ERROR_MOD_REGISTER("BSM", status );
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
    exit(EXIT_MOD_INIT_FAILED);
  }

  printAndEmptyErrorChain(&(getModuleContext()->errChain));

  /* Initialise Lib print for TLV  */
  printTlvInit( (UINT8*)ctx->configDir);

  /* Intialise Lib Print for TLV */
  printNotiMsgTlvInit( (UINT8*)ctx->configDir );

  /* Initialise the State machine */
  initBsmStateMachine();

  /* Initialise status tables */
  initEnodeBStatusTable();
  initRabHistoryStatusTable();
  initUeEventHistoryTable();
  initActiveRabStatusTable();
  initActiveUeStatusTable();
  initIndexForCmdTables();
  initLedColorInfo(ctx);
  initModuleLogStatusTable(ctx);
  initLoginStatusTable(ctx);
  initCellStatusTable();
  getNumOfMaxMmesToConnectWith( ctx );

  schedMsgHandler(MODID_BSM, bsmMain);
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  return 0;
}
