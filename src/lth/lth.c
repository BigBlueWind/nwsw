/**
 * @file    lth.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file is the starting point for LTH module
 *
 * @author  Muktheshwar Madishetty
 * @date    12-10-2012
 * @details This file contains module initialisation for Logging and Tracing Handler
 */

#include "lth.h"

/**
 * @param argc used for finding number of arguments passed
 * @param argv arguments passed while running LTH module
 * @details This function is the entry point for LTH mOdule.
 */
int main (int argc, char *argv[])
{
  TBOOL       status;
  UINT8      *exeName = argv[0];
  TimeSpec    tick = { TIMEUNIT_SEC, 1 };

  LthTraceContext   *lthTrcCtx = getLthTraceContext();

  cmdLineParamHandler( argc, argv, &lthTrcCtx->swConfigDir[0], &lthTrcCtx->versionInfo );
  sprintf( lthTrcCtx->swRootDir, "%s/trace/", argv[2] );
  status = modInit( MODID_LTH, exeName, (char*)argv[2], tick );

  if (status EQ TRUE)
  {
    sendStartIndToPrm(MODID_LTH, MSG_LTH_PRM_START_IND);
  }
  else
  {
    logError("Could not register LTH Module, Status returned=%d", status);
    emptyErrorChain(&(getModuleContext()->errChain));
    exit(EXIT_FAILURE);
  }
  /*
   * Load enbLimits config file into LTH context
   */
  readEnbLimitsFromCfg( &lthTrcCtx->swConfigDir[0] );
  initTimersInfoInCtx();
  fillGenericFunctionTable();
  emptyAllTables();
  initLthTraceContext();
  initTraceStatusTable();
  initTraceHistoryTable();
  emptyErrorChain(&(getModuleContext()->errChain));
  schedMsgHandler(MODID_LTH, lthMain);
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  return 0;
}
