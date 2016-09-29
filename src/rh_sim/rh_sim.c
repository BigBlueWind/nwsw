/**
 * @file    rh_sim.c
 * @brief This module is Device Handler that pass on the response to the DEH module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change History
 * ------------------------
 * 1. 
 * 2. 
 */

#include "rh_sim.h"
//#include "rh_deh_if.h"

SINT32   g_sockFd;
#define  LK_MAILBOX_ID                               9001      /* 0x2000*/

/* Global Context */
DehMtContext     g_dehMtCtx;

FILE *rhSimulatorLog = NULL;

/**
 * @details DESCRIPTION
 * This functions returns the DEH Context
 */
DehMtContext  *getDehMtContext()
{
  return (&g_dehMtCtx);
}

int main( )
{
  TBOOL  retVal = FALSE;
  TimeSpec  tick = { TIMEUNIT_SEC, 1};
  /* Init DEH-MT context  */
  initDehMtCtx();

  if( (rhSimulatorLog = fopen( "../simulators/rh_sim.exe.log", "w" )) EQ NULL)
  {
    printf("Cannot Open RH Simulator Log File !");
    exit(0);
  }
  else
  {
    fprintf(rhSimulatorLog, "====================== RH Simulator Log ===================== \n");
  }

  retVal = readSimulatorConfigFile();
  if ( retVal EQ FALSE )
  {
    exit(EXIT_FAILURE);
  }
  fprintf(rhSimulatorLog, "Radio Head IP Address : %s\n", g_dehMtCtx.rhIpAddr );
  fprintf(rhSimulatorLog, "EnodeB IP Address     : %s\n", g_dehMtCtx.eNodeBIpAddr );
  fprintf(rhSimulatorLog, "Radio Head Simulator Version : %s\n", g_dehMtCtx.version );
  retVal = readRhSimConfigFile();
  if ( retVal EQ FALSE )
  {
    exit(EXIT_FAILURE);
  }

  openUdpSocket( LK_MAILBOX_ID, TRUE, &g_sockFd);

#if 1
  TimersConfig  *dehMtTimerInfo = getTimerInfoRsp();
  clearBytes( &tick, sizeof(TimeSpec));
  tick.duration = dehMtTimerInfo->duration;
  tick.timeUnit = dehMtTimerInfo->timeUnit;
  startTimerForSim( tick );
#endif

  schedDehMtMsgHandler();

  return 0;
}
