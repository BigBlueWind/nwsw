/**
 * @file    deh.c
 * @brief This module is Device Handler that shall poll the various devices and report back the
 *	    response to the Base Station Manager
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author Amar Ramteke, splolepally, mkata
 * @date   25-05-2011
 *  @details 1) Created initial version
 *           2) Added errChain parameter to all functions.
 */

#include "deh.h"


extern DehContext    g_DehCtx;

int main(int argc, char *argv[])
{
  UINT8 status;
  UINT8 *exeName = argv[0];
  TimeSpec  tick = { TIMEUNIT_SEC, 1 };
  pthread_t  gpsThread;
  UINT32     userInfo = 0;

  DehContext *dehContext = getDehContext();
  clearBytes(dehContext,sizeof(DehContext));

  cmdLineParamHandler( argc, argv, &dehContext->configDir[0], &dehContext->versionInfo );
  /* DEH Module Initialisation */
  status = modInit( MODID_DEH, exeName, (char*)argv[2], tick);
  if(status NOTEQ TRUE)
  {
    BS_ERROR_MOD_REGISTER( "DEH", status );
	printAndEmptyErrorChain(&(getModuleContext()->errChain));
	exit(EXIT_FAILURE);
  }

  /* Get and Update IP address of enodeb*/
  getAndUpdateEnodebIpAddr(dehContext);

  /* Send Start Indication to PRM */
  sendStartIndToPrm(MODID_DEH, MSG_DEH_PRM_START_IND);

  /* Initialise Timers, Global variables, fill default Values  */
  dehInit(&dehContext->configDir[0]);


  /* OPerating Request to Radio Head */
  if( TRUE EQ dehRhOperatingModeInfo())
  {
    setProcedureOngoing(DEV_RADIOHEAD,PROC_DEV_INIT);/*PROC_DEV_INIT is taken as we are initialising the RH At Initial bootup*/
    logInfo("Initial Operating Mode Info request sent to Radio Head", NULL);
  }
  else
  {
    logError("Error in sending Operating Mode Info request to Radio Head", NULL);
  }
  
  if(isDeviceMonitored(DEV_GPS))
  {
    /* Spawn GPS Thread*/
    if ( 0 EQ pthread_create(&gpsThread, NULL, gpsThreadMain, NULL))
    {
      logInfo("GPS Thread spawning successful", NULL);
    }
    else
    {
      logError("Error in spawning GPS thread", NULL);
    }
  }

  /* Initialise Lib print for TLV  */
  printTlvInit( &dehContext->configDir[0]);

  /* Start the state machine and change DEH state to DEH_STATE_IDLE */
  initDehStateMachine();

  dehSetState(dehContext, DEH_STATE_WAIT_CFG);

  /* Updating the Status of all Devices */
  dehUpdateDeviceStatus();
  dehUpdateDeviceAlarmStatus();

  /* Start the timer - timerTypeWaitDehStatusReportPeriod
   * This timer will be running for ever and try to fetch status of all devices attached
   * If any change is detected in Device status then it will report it to BSM
   * */
  startDehTimer( TIMERNAME_WAITDEHSTATUSREPORTPERIOD, dehContext, &userInfo );
  /* Start a Timer to receive INIT NOTI from Radio Head */
  startDehTimer( TIMERNAME_WAITRHINIT, dehContext, &userInfo );
  DehRhRspTimersContext  *timersContext = getRhRspTimersContext();
  timersContext->rhInitTimerInfo.dehCmdType = RSP_USER_NOTI;
  timersContext->rhInitTimerInfo.isRhNotiReceived = FALSE;

  /* Start Message Handler */
  schedDehMsgHandler(MODID_DEH, dehMain);
  /* Empty Error Chain Before leaving DEH */
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  return 0;
}
