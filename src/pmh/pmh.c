/**
 * @file    pmh.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file is the starting point for PMH module
 *
 * @author  Ramprasad Reddy Chinta
 * @date    10-02-2011
 * @details This file contains module initialisation for Performance Management Handler
 */

#include "pmh.h"

/**
 * @param argc used for finding number of arguments passed
 * @param argv arguments passed while running PMH module
 * @details This function is the entry point for PMH mOdule.
 */
int main (int argc, char *argv[])
{
  TBOOL       status;
  UINT8      *exeName = argv[0];
  String256   rootDir;
  TimeSpec    tick = { TIMEUNIT_SEC, 1 };
  PmhContext   *pmhContext = getPmhContext();

  clearBytes(rootDir,sizeof(String256));

  fillGenericFunctionTable();
  emptyAllTables();

  cmdLineParamHandler( argc, argv, &pmhContext->configDir[0], &pmhContext->versionInfo );
  copyBytes( (char*)argv[2], sizeof(FilePath), &rootDir[0] );
  snprintf( &pmhContext->pmLogDir[0], sizeof(FilePath), "%s/pmlogs/", &rootDir[0] );

  /* Allocate memory of pm file log status table */
  initPmFileLogStatusTable(pmhContext);

  status = modInit( MODID_PMH, exeName, (char*)argv[2], tick );

  if (status EQ TRUE)
  {
    sendStartIndToPrm(MODID_PMH, MSG_PMH_PRM_START_IND);
  }
  else
  {
    BS_ERROR_MOD_REGISTER("PMH", status );
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
    exit(EXIT_FAILURE);
  }

  setPmhState( pmhContext, PMH_WAIT_INIT_CFG );
  pmhContext->isOpStartRcvd = FALSE;
  /* Initialise Lib print for TLV  */
  printTlvInit( (UINT8*) &(pmhContext->configDir[0]));

  initPmhStateMachine();
  pmhInitCounterCollectionStatusTable();
  pmhInitCounterContext(pmhContext);
  pmhReadTimersFromCfg(pmhContext);
  pmhGetCombinedPmLogSizeThreshold();
  pmhReadPmLogFileTableAndUpdateFileStatusTable();
  schedMsgHandler(MODID_PMH, pmhMain);
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  return 0;
}


