/**
 * @file    prm.c
 * @brief This is the entry point of PRM module.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        :  Shweta Polepally
 * Date           :  July 2010
 * Change History :  Basic Version
 *
 */

#include "prm.h"

int main(int argc, char *argv[])
{
  UINT8        status;
  UINT8        *exeName = argv[0];
  TimeSpec     tick = { TIMEUNIT_SEC, 1 };
  PrmContext   *ctx = getPrmContext();
  

  clearBytes( ctx, sizeof(PrmContext));

  /* Handle command line arguments before modInit.
   * This allows a user to know the version of the executable even if modInit fails.
   */
  cmdLineParamHandler( argc, argv, NULL, &ctx->prmInfo.versionInfo );

  fillGenericFunctionTable();    /* Initialise fill functions of generic table */
  emptyAllTables();


  if ( FALSE EQ prmGetPicoBsHomePath(&ctx->picoBsHomePath[0]) )
  {	
    exit(EXIT_PICO_HOME_PATH_NOT_SET);
  }

  /* log Init will not be done in modInit function for PRM only. Don't remove below line
   * Reason: Logs for PRM needs to be created in <home>/logs/
   * But LogInit will default create logs in rootDir/inst_A<B>/logs */
  logInit( exeName, &ctx->picoBsHomePath[0] );

  /* set defaults - paths, values */
  initPrmContext(ctx);

  /* ModuleVersion = Part1 (received in swInfo) + Part2(received in startInd)
   * Note: Part2 to be appended with the version info received in START_IND from all modules */
  prmFormModuleVersionStringPart1( &ctx->prmInfo, &ctx->moduleVersionStr[0] );

  initPrmStateMachine();

  status = modInit( MODID_PRM, exeName, &ctx->isRunning->rootDir[0], tick );
  if (status EQ FALSE)
  {
    BS_ERROR_MOD_REGISTER("PRM", status);
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
    exit(EXIT_MOD_INIT_FAILED);
  }
  /* Parse OAM Configuration Template */
  if ( FALSE EQ prmParseOamOprCfgTables( &(ctx->cfgFilePath[0]), &ctx->oprCfgTableInfo[0] ) )
  {
    exit(EXIT_CONFIG_READ_FAIL);
  }

  if( TRUE EQ isPrmStartedDueToSwActivation(ctx) )
  {
	ctx->isSwActivationPerformed = TRUE;
    /* PRM started due to SW Activation*/
    if ( FALSE EQ prmInitTablesAndStartModules( REASON_SW_ACTIVATION ) )
	{
	  /* Error is logged inside the function */
	  exit(EXIT_CONFIG_READ_FAIL);
	}
  }
  else
  {
    if ( FALSE EQ prmInitTablesAndStartModules( REASON_INIT ) )
    {
      /* Error is logged inside the function */
      exit(EXIT_CONFIG_READ_FAIL);
    }
  }

  /* Initialise Lib print for TLV  */
  printTlvInit( (UINT8 *)&(ctx->cfgFilePath[0]) );

  schedMsgHandler(MODID_PRM, prmMain);
  printAndEmptyErrorChain(&(getModuleContext()->errChain));

  return FALSE;
}

