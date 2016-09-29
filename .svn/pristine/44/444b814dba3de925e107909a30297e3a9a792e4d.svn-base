/**
 * @file    fth.c
 * @brief This file is the starting point for FileTransferHandler module
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * Change History : 1. Basic Version
 *                  2. Shweta (19-May-2010) Added command line arg --version.
 *                     When executable is started with this option, the version
 *                     will be displayed and executable will exit.
 *
 * @author  Lakshmi Narayana Madala
 * @date    15-Jul-2010
 */

#include "fth.h"

int main(int argc, char *argv[])
{
  UINT8          status;
  UINT8         *exeName = argv[0];
  TimeSpec       tick = { TIMEUNIT_SEC, 1 };

  FthContext    *fthContext = getFthContext();

  /* Read version information from version.cfg file and store it in the context */
  fillGenericFunctionTable();
  emptyAllTables();

  /* Handle command line arguments before modInit.
   * This allows a user to know the version of the executable even if modInit fails.
   */
  cmdLineParamHandler( argc, argv, &(fthContext->configDir[0]), &(fthContext->versionInfo));

  status = modInit( MODID_FTH, exeName, (char*)argv[2], tick);
  if (TRUE NOTEQ status )
  {
    BS_ERROR_MOD_REGISTER("FTH", status );
    emptyErrorChain(&(getModuleContext()->errChain));
    exit(EXIT_MOD_INIT_FAILED);
  }

	readFthTimersFromCfg(&fthContext->configDir[0]);

  /* Initialise Lib print for TLV  */
  printTlvInit( (UINT8*) &(fthContext->configDir[0]));

  /* Initialize FTH state Machine,
   * Set the initial state of FTH to IDLE
   * Read the default FTH configuration parameters */
  initFthStateMachine();
  fthContext->fthState = FTH_STATE_IDLE;
	fthContext->isScheduled = FALSE;

  if(FALSE EQ readFthCfgTable( &(fthContext->configDir[0])))
  {
    /* Error is logged inside the function */
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
	exit(EXIT_CONFIG_READ_FAIL);
  }

  sendStartIndToPrm(MODID_FTH, MSG_FTH_PRM_START_IND);

  clearBytes( &(fthContext->fthCfg.downloadDir), sizeof(String256));
  fthUpdateDownloadDir( (char*)argv[2], (UINT8*)&(fthContext->fthCfg.downloadDir) );

  fthInitStatusTables(fthContext);

  emptyErrorChain(&(getModuleContext()->errChain));
  schedMsgHandler(MODID_FTH, fthMain);
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  return 0;
}
