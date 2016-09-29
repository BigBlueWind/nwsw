/**
 * @file    cih.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file is the starting point for CIH module.
 *
 * @author  Madhusudana Rao Kata
 * @date    10-05-2010
 * @details     1. Basic version @n
 *              2. Added polling feature so that user no need to wait for response on 18-05-2010. @n
 *              3. Shweta (19-May-2010) Added command line arg --version.
 *                 When executable is started with this option, the version
 *                 will be displayed and executable will exit. @n
 *              4. Added batch file processing feature --madhu. @n
 *              5. Log file will be generated with all commands entered by the user. @n
 *              6. libtecla is added to display previous command on pressing uparrow and History functionality. @n
 *              7. TLV Changes added. @n
 *              8. Timer Added in CIH. @n
 *
 */

#include "cih.h"
#include "cih_tecla.h"

/* Global Context */
CihContext       g_cihCtx;
CihTeclaContext  g_CihTeclaContext;
SINT32           g_cmdsSent = 0;
SINT32           g_respRecd = 0;

/**
 * @param   argc  used for finding number of arguments passed
 * @param   argv  arguments passed while running CIH module
 * @details       This function is the Main Function for CIH Module.
 */
int main(int argc, char *argv[])
{
  GetLine        *gl=NULL;           /* The resource object of gl_get_line() */
  SignalActions  *si=NULL;           /* Temporary storage of displaced signal handlers. */
  sigset_t       all_signal_mask;    /* The set of signals known by gl_get_line() */

  UINT8          status ;
  fd_set         rfds;                /* The set of file descriptors to watch for readability */
  fd_set         wfds;                /* The set of file descriptors to watch for writability */

  String100 exeName;
  clearBytes(exeName, sizeof(exeName));
  snprintf(exeName, sizeof(exeName), "%s", argv[0]);
  TimeSpec  tick = { TIMEUNIT_SEC, 1};
  CihContext   *ctx = getGlobalCihContext();

  if ( argc < 3 )
  {
    printf("\n Enter --rootDir <PATH of root> as command line argument!! \n");
    exit(EXIT_FAILURE);
  }
  /* Handle command line arguments before modInit. */
  cmdLineParamHandler(argc, argv, &ctx->configDir[0], &ctx->versionInfo);

  getCihRootDir(argc, argv, (char*)&ctx->rootDir[0]);

  /* Print CIH Banner */
  cihPrintBanner(argc,argv, &ctx->versionInfo);

  /* Initialisation of Tecla */
  cihInitTecla(&gl,&si,&all_signal_mask);

  /* CIH Module Initialisation */
  snprintf(exeName, sizeof(exeName), "%s", "cih1.exe");
  status = modInit( MODID_CIH1, exeName, (char*)argv[2], tick);
  if(status EQ TRUE)
  {
    ctx->instanceId = 1;
    printf("Instance 1 is  running !\n");
  }
  else
  {
    snprintf(exeName, sizeof(exeName), "%s", "cih2.exe");
    status = modInit( MODID_CIH2, exeName, (char*)argv[2] , tick);
    if(status EQ TRUE)
    {
      ctx->instanceId = 2;
      printf("Instance 2 is  running !\n");
    }
    else
    {
      snprintf(exeName, sizeof(exeName), "%s", "cih3.exe");
      status = modInit( MODID_CIH3, exeName, (char*)argv[2], tick);
      if(status EQ TRUE)
      {
        ctx->instanceId = 3;
        printf("Instance 3 is  running !\n");
      }
      else
      {

        BS_ERROR_MOD_REGISTER( "CIH", status );
        printf("Could not register CIH Module, All 3 instances of CIH are running. Status returned=%d.\n", status);
        printAndEmptyErrorChain(&(getModuleContext()->errChain));
        exit(EXIT_FAILURE);
      }
    }

  }

  fillGenericFunctionTable();
  emptyAllTables();

  /* This function initialise Global Data structures used to store commands related information for Timers */
  initCmdStatus();
  /* This will read all the timers related to CIH module and store them in the CIH context */
  readCihTimersFromCfg( &ctx->configDir[0]);

  /* Initialise Lib print for TLV  */
  printTlvInit( &ctx->configDir[0] );

  /* Start CIH Timer */
  startCihRspTimer( TIMERNAME_WAITCMDRSP);

  /* Parse OAM Configuration Template */
  OamTableContext *oamCtx = getOamTableContext();
  parseOamCfg( &ctx->configDir[0], oamCtx );

  /* Initialisation of LogInit */
  cihLogInit( (char*)&ctx->rootDir[0] );

  /* This function executes commands passed using -c and -f option and Exits*/
  cihExecuteCmdsInCmdLine( argc, argv );

  /* Switch gl_get_line() to non-blocking server mode. */
  if(gl_io_mode(*(&gl), GL_SERVER_MODE))
    cleanup_and_exit(*(&gl), *(&si), 1);

  while(1)
  {
    cihTelcaProcSignals(&gl,&si,&all_signal_mask,&rfds,&wfds);

    /* If the terminal file descriptor is now ready for I/O, call
       gl_get_line() to continue editing the current input line.*/
    if(FD_ISSET(STDIN_FILENO, &rfds) || FD_ISSET(STDIN_FILENO, &wfds))
    {
      if (gl_customize_completion(gl, NULL, cmd_cpl_fn) NOTEQ 0)
      {
        exit(0);
      }
      /* Process user commands */
      procCihCommands(gl);
      /* To resume command-line editing, return the terminal to raw,non-blocking I/O mode.*/
      gl_raw_io(gl);
    }
    else if(gl_return_status(gl) EQ GLR_ERROR || gl_return_status(gl) EQ GLR_EOF)
    {
      cleanup_and_exit(gl, si, 1);
      cihCloseLog();
    };
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
  };
}
