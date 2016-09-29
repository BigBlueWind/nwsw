/**
 * @file     cp_main.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains Entry point(Main function) to CP(Control Plane)
 *
 * @author   Madhusudana Rao Kata
 * @date     01-Jun-2011
 * @details  1. Basic version @n
 */

#include "ccpu_cmn.h"

/* RRM Related Includes */
#include "ueh.h"
#include "lueh.h"
#include "lueh.x"
#include "udh.h"
#include "sfi.h"
#include "bs_tables.h"
#include "log.h"
#include "rb_common.h"

#include <errno.h>
#include "sendToOam.h"
#include "ipc.h"

void configMemLeakDetection(UINT8 memoryDiagCfg )
{

  g_blockBufTable.isEnabled   =0;
  g_msgBufTable.isEnabled     = 0;
  g_staticBufTable.isEnabled  = 0;
  
  if(memoryDiagCfg & STATICBUFFBIT)
   g_staticBufTable.isEnabled   =TRUE;
  if(memoryDiagCfg & BLOCKBUFFBIT)
   g_blockBufTable.isEnabled    = TRUE;
  if(memoryDiagCfg & MSGBUFFBIT)
    g_msgBufTable.isEnabled     = TRUE;
   
}

TBOOL readEnbPreConfig(void)
{
  char              *rootDir = NULL;
  FilePath           filename;
  UINT8              isError = FALSE;
  UehConfig          *uehConfig  = &(smCb.cfgCb.uehConfig);
  EgtpConfig         *egtpConfig = &(smCb.cfgCb.egtpConfig);
  S1apConfig         *s1apConfig = &(smCb.cfgCb.s1apConfig);
  SctpConfig         *sctpConfig = &(smCb.cfgCb.sctpConfig);

  rootDir = msArgv[2];
  if ( rootDir EQ NULL )
  {
    logError( "%s", "Environment variable --rootDir is not set !");
    exit(EXIT_FAILURE);
  }
  snprintf((char *)filename, sizeof(FilePath) , "%s/config/enb_pre_config.cfg", rootDir );

  /* TODO: Set these using a run time message/config flag */
   
   
  readFromFileUehConfigTable( filename, uehConfig, &isError );
  configMemLeakDetection(uehConfig->memoryDiagCfg);
  
    logConfig("Mem leak detection: block %d | msg %d | static %d ",
    g_blockBufTable.isEnabled,
    g_msgBufTable.isEnabled,
    g_staticBufTable.isEnabled
  );

  if ( isError EQ TRUE )
  {
    return FALSE;
  }

  readFromFileEgtpConfigTable( filename, egtpConfig, &isError );
  if ( isError EQ TRUE )
  {
    return FALSE;
  }

  readFromFileSctpConfigTable( filename, sctpConfig, &isError );
  if ( isError EQ TRUE )
  {
    return FALSE;
  }

  readFromFileS1apConfigTable( filename, s1apConfig, &isError );
  if ( isError EQ TRUE )
  {
    return FALSE;
  }

  return TRUE;
}

void openCpLog(void)
{
     UINT8   logFileName[SIZE_UEH_LOG_FILENAME];
     FILE    *filePtr;
     int numBytesToBeWritten = 0;

     clearBytes( &logFileName[0], SIZE_UEH_LOG_FILENAME );

     logInit((UINT8 *)g_exeName,(UINT8 *)g_rootDir);

     numBytesToBeWritten = snprintf((char *)&logFileName[0], SIZE_UEH_LOG_FILENAME, "%s/log/%s_ccpu.log", g_rootDir, g_exeName );
     if (numBytesToBeWritten >= SIZE_UEH_LOG_FILENAME)
     {
       UEH_PROCEDURE_FAILED("LOG file creation", NOT_APPLICABLE, NOT_APPLICABLE, "File name truncated");   
       exit(EXIT_FAILURE);
     }

     filePtr = freopen((char *)logFileName, "w", stdout);
     if (!filePtr)
     {
	logFatalErrorAndExit(errno, "freopen(%s,...) failed, %s", logFileName, strerror(errno));
     }
     setvbuf( filePtr, NULL, _IONBF /*_IOLBF*/, 30);
}


/**
 * @param     selfProcId  CP Process Identifier which is used to communicate between Other process in eNodeB
 * @details   Main function for Control Plane Executable..
 */
PUBLIC S16 cpMain(S16 selfProcId)
{
   /* System tasks that are needed as per the architecture */
   SSTskId rrc_s1ap_ueh_nx_taskId, sctp_taskId, sm_taskId, systaskId;

   openCpLog();
   SSetProcId(selfProcId);
   /* All the System tasks(threads)  are created with same priority*/
   SCreateSTsk(PRIOR0, &systaskId);
   rrc_s1ap_ueh_nx_taskId = sctp_taskId = sm_taskId = systaskId;

   /* Register SM TAPA Task */
   if (ROK != regAndAttachTsk("SM", ENTSM, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, smActvInit, (ActvTsk)smActvTsk, sm_taskId))
   {
     RETVALUE(RFAILED);
   }
   smCb.init.dbgMask = 0xffffffff;
   smCb.init.ent  = ENTSM;
   smCb.init.inst = 0;
   readEnbPreConfig();

   /* Register RRC TAPA Task */
   if (ROK != regAndAttachTsk("RRC", ENTNH, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, nhActvInit, (ActvTsk)nhActvTsk, rrc_s1ap_ueh_nx_taskId))
   {
     RETVALUE(RFAILED);
   }
   if (ROK != regAndAttachTsk("NX", ENTNX, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, uehActvInit, (ActvTsk)uehActvTsk, rrc_s1ap_ueh_nx_taskId))
   {
     RETVALUE(RFAILED);
   }

   /* Register UEH TAPA Task */
   if (ROK != regAndAttachTsk("UEH", ENTUEH, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, uehActvInit, (ActvTsk)uehActvTsk, rrc_s1ap_ueh_nx_taskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register S1AP TAPA Task */
   if (ROK != regAndAttachTsk("S1AP", ENTSZ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, szActvInit, (ActvTsk)szActvTsk, rrc_s1ap_ueh_nx_taskId))
   {
     RETVALUE(RFAILED);
   }

   /* Register SCTP TAPA Task */
   if (ROK != regAndAttachTsk("SCTP", ENTSB, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, sbActvInit, (ActvTsk)sbActvTsk, sctp_taskId))
   {
     RETVALUE(RFAILED);
   }

   ipcInit(selfProcId,OAM_PROC_ID,MAX_PROC_ID);
   
   regDriverTask(1, OAM_PROC_ID);
   regDriverTask(2, GTP_PROC_ID);
   regDriverTask(3, RADIOUP_PROC_ID);

   /*Initialize the state machine of SM,
    * We wait until we receive config requests from BSM,
    * Once EnodeBConfig is received then we can start configuring the modules.
    */
   sfiSendStartQueryToAllModules();
   
   smCb.smState = UEH_SM_BSM_ENB_CONFIG_REQ;
   RETVALUE(ROK);
} /* end of cpMain function */

/**
 * @details   Application entry point in Control Plane Executable(cp.exe)..
 */
PUBLIC S16 psMain(Void)
{

//   allocSendBuf();
   
   if(0 EQ strncmp(g_exeName, "cp.exe", strlen("cp.exe")))
   {
     /* Control Plane Tasks */
     cpMain(CP_PROC_ID);
   }

   else
   {
     fprintf(stderr,"ERROR: Unknown executable name = %s. Valid ones are {cp.exe, gtp.exe, radioup.exe}. Exiting\n", g_exeName);
     exit(EXIT_FAILURE);
   }

  RETVALUE(ROK);
}
/**
 * @param       cfg         config Structure
 * @details
 * This function opens enb_user_config.cfg and fills the context in cfg *
 */
void openlogConfig(
     config_t *cfg
    )
{
  char *rootDir = NULL;
  char filename[CONFIG_FILE_LENGTH];

  rootDir = msArgv[2];
  if ( rootDir EQ NULL )
  {
    fprintf( stderr, "Environment variable --rootDir is not set !\n");
    exit(EXIT_FAILURE);
  }

  sprintf( filename,"%s/config/enb_user_config.cfg", rootDir );

  config_init(cfg);
  logInfo("UdpInitCtx(): Reading enb_user_config.cfg file= %s",filename);
  if(! config_read_file(cfg, filename))
  {
    fprintf(stderr, "%s:%d - %s\n",
        config_error_file(cfg),
        config_error_line(cfg),
        config_error_text(cfg));

    UEH_PROCEDURE_FAILED("config_read_file", NOT_APPLICABLE, NOT_APPLICABLE, "Incorrect config file path");
    config_destroy(cfg);
    exit(EXIT_FAILURE);
  }
}
