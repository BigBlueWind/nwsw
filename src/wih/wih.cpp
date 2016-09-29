/**
 * @file    wih.cpp
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file is the starting point for WIH module
 *
 * Author     : Tara Shankar Maji
 * Date       : 13-09-2010
 * Description: This file contains module initialization for Web Interface Handler.
 *              This module interfaces with web server over Fast CGI interface & BSM module.
 * Change History: 13-09-10 Initial version.
 */

#include "wih.h"
#include "wih_wsInterface.h"

extern void wihMain( FR Msg *msgIn );

/****************** GLOBAL VARIABLES START *******************/
bool g_RspRcvd = false;			// For checking whether BSM response has come
bool g_RspProcessed = false;    // For checking whether BSM response is processed and sent to Web
pthread_mutex_t g_session_table_mutex;	// Mutext for session table update from threads
pthread_mutex_t	g_bsm_rsp_mutex;	// Mutext for waiting on the condition variable
pthread_cond_t	g_bsm_rsp_cv;		// Condition variable for thread wait
OamTableContext g_OamTableContext;	// Configuration Table data for BSM based TLV msgs
Msg      *g_msgIn=NULL;			// Global Message Pointer to be used between threads
UINT8   g_configFilePath[256];       // Global configuration directory path
UINT8   *g_rootDir = NULL;
/****************** GLOBAL VARIABLES END *******************/

/**
 * This thread based function is used for
 * handling all web server based FCGI 
 * protocol based requests
 */
void *startFcgiInterface(void *tv)
{
        if(tv EQ NULL)
        {
           // Nothing to do.
        }

  	// Start the web server interface for receiving FastCGI requests
  	CWebInterface::getInstance().acceptNewRequest(); 

	pthread_exit(NULL);
}

/**
 * @param argc used for finding number of arguments passed
 * @param argv arguments passed while running WIH module
 * @details This function is the entry point for WIH module.
 */

int main (int argc, char *argv[])
{
 	UINT8  	status;
  	char   	*exeName = argv[0];
  	TimeSpec tick = { TIMEUNIT_SEC, 5 };
        /* To check - shweta */
  	VersionInfo   versionInfo;
  	clearBytes( &versionInfo, sizeof(VersionInfo) );
  	clearBytes( &g_configFilePath, sizeof(g_configFilePath) );

  	// Read root and config PATH
  	g_rootDir = (UINT8*) argv[1];
        snprintf((char *)g_configFilePath, sizeof(g_configFilePath), "%s/config/", argv[1]);

	// Read the configuration file for all TABLES
	parseOamCfg(g_configFilePath, &g_OamTableContext);

  	// Initialize WIH module
  	status = modInit( MODID_WIH, (UINT8*)exeName, g_rootDir, tick);
  	if(TRUE NOTEQ status)
  	{
    		BS_ERROR_MOD_REGISTER("WIH", status );
    		printAndEmptyErrorChain(&(getModuleContext()->errChain));
    		exit(EXIT_FAILURE);
  	}

  	// Log WIH module start
	logInfo("Starting WIH module, with %d startup parameters...", argc);

  	// Get the WIH based configuration table from BSM

  	// Copy the web directory for the web server pages
	if (0 NOTEQ system("rm -fr /tmp/web"))
	{
		BS_ERROR_SYSTEM_COMMAND_FAILED("rm -fr /tmp/web");
	}

	char strCmd[300];
	clearBytes( strCmd, 300 );
    snprintf( strCmd, sizeof(strCmd), " cp -rf %s/web /tmp/", (char*)g_rootDir );
	//system("cp -rf web /tmp/");
    if(0 NOTEQ system( strCmd ))
    {
    	BS_ERROR_SYSTEM_COMMAND_FAILED(strCmd);
    }
	if (0 NOTEQ system("chmod -R 755 /tmp/web"))
	{
		BS_ERROR_SYSTEM_COMMAND_FAILED("chmod -R 755 /tmp/web");
	}
  	// Start embedded web server for HTTP connections 
	// We will recreate the web server logs
	if ( 0 NOTEQ system(" rm -fr /tmp/lighttpd"))
	{
	   BS_ERROR_SYSTEM_COMMAND_FAILED("rm -fr /tmp/lighttpd");
	}

	if (0 NOTEQ system(" mkdir /tmp/lighttpd"))
	{
		BS_ERROR_SYSTEM_COMMAND_FAILED("mkdir /tmp/lighttpd");
	}

	if (0 NOTEQ system(" mkdir /tmp/lighttpd/logs"))
	{
		BS_ERROR_SYSTEM_COMMAND_FAILED("mkdir /tmp/lighttpd/logs");

	}
	char strWbSvr[MAX_WBSVR_STARTUP_STR];
	clearBytes(strWbSvr,MAX_WBSVR_STARTUP_STR);
	//sprintf(strWbSvr, "./bin/lighttpd.exe -f %s/lighttpd.conf",(char *)g_configFilePath);
	snprintf(strWbSvr, sizeof(strWbSvr),"%s/bin/lighttpd.exe -f %s/lighttpd.conf", (char*)g_rootDir, (char *)g_configFilePath);
	logInfo("config Path for lighthttpd is:%s", strWbSvr );

	if (0 NOTEQ system(strWbSvr))
	{
		BS_ERROR_SYSTEM_COMMAND_FAILED(strWbSvr);
	}

	logInfo("%s", "Started Web Server successfully.");

  	// Send start indication to PRM module
	  sendStartIndToPrm(MODID_WIH, MSG_WIH_PRM_START_IND);

	// Lets start the FCGI msg handling thread
	// Note: Web Server will be started only upon OP_START from BSM
	pthread_t pThr;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_mutex_init(&g_bsm_rsp_mutex,NULL);
	pthread_mutex_init(&g_session_table_mutex,NULL);
	pthread_cond_init(&g_bsm_rsp_cv,NULL);

	int rc = pthread_create(&pThr,&attr,startFcgiInterface, NULL);
	if(rc){
		// Stop the web server

		if (0 NOTEQ system("pkill -9 lighttpd.exe"))
		{
			BS_ERROR_SYSTEM_COMMAND_FAILED("pkill -9 lighttpd.exe");
		}
		// Log the error
		logError("%s","FAILED to start the FCGI msg handling thread.. CLOSING DOWN \n");
		// Close WIH application
		exit(EXIT_FAILURE);
	}
	else{
		logInfo("%s", "Ready to service FCGI messages from WEB CRAFT Clients.");
	}

	// Start the inactivity timer
	TimerDesc usrInactTmr;
	clearBytes(&usrInactTmr, sizeof(usrInactTmr));

	usrInactTmr.moduleId = MODID_WIH; 
	usrInactTmr.userInfo = MODID_WIH;
	usrInactTmr.timeoutMsgId = MSG_TIMEKEEPER_WIH_TIMEOUT_IND;
	usrInactTmr.timeSpec.timeUnit = TIMEUNIT_SEC;
	usrInactTmr.timeSpec.duration = 5;
	usrInactTmr.timerType = TIMERNAME_WAITCMDRSP;

	createAndStartTimer(&usrInactTmr, NULL, NULL);

	/* Initialise Lib print for TLV  */
        printTlvInit( g_configFilePath );

	// Call the framework based schedMsgHandler
  	schedMsgHandler(MODID_WIH, wihMain);

	// Clean-up
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&g_bsm_rsp_mutex);
	pthread_mutex_destroy(&g_session_table_mutex);
	pthread_cond_destroy(&g_bsm_rsp_cv);

  	return 0;
}
