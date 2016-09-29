/**
 * @file     gtp_main.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains Entry point(Main function) to GTP(Data Plane .. UDH, GTP. TUCL)
 *
 * @author   Madhusudana Rao Kata
 * @date     01-Jun-2011
 * @details  1. Basic version @n
 */

#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "lueh.h"
#include "lueh.x"
#include "udh.h"
#include "sfi.h"
#include "bs_tables.h"
#include "log.h"
#include "rb_common.h"
#include "sendToOam.h"
#include "ipc.h"


#ifdef PWAV_L2SIM 
#undef SS_4GMX_UCORE 
#endif

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

/**
 * @param     selfProcId  GTP Process Identifier which is used to communicate between Other process in eNodeB
 * @details   Main function for Data Plane Executable..
 */
PUBLIC S16 gtpMain(S16 selfProcId)
{
  /* System tasks that are needed as per the architecture */
  SSTskId egtp_udh_taskId;
  SSTskId tucl_taskId;

  UINT8   logFileName[SIZE_UEH_LOG_FILENAME];
  FILE    *filePtr;

  logInit((UINT8 *)g_exeName, (UINT8 *)g_rootDir);
  if( NULL EQ g_rootDir )
  {
     fprintf(stderr,"Config Directory is not specified, hence exiting\nEnter [ --rootDir <Path-Of-Config-Directory> ] \n");
     exit(FALSE);
  }

  snprintf((char *)logFileName, SIZE_UEH_LOG_FILENAME, "%s/log/%s_ccpu.log", g_rootDir, g_exeName );

  filePtr = freopen((char *)logFileName, "w", stdout);
  if( NULL EQ filePtr )
  {
    fprintf(stderr,"Could not open log/%s_ccpu.log file for logging.\n", g_exeName );
  }
  else
  {
    setvbuf( filePtr, NULL, _IONBF /*_IOLBF*/, 30);
  }

 // g_blockBufTable.isEnabled   = 0;
 // g_msgBufTable.isEnabled     = 0;
 // g_staticBufTable.isEnabled  = 0;
 
 logConfig("Mem leak detection: block %d | msg %d | static %d ",
    g_blockBufTable.isEnabled,
    g_msgBufTable.isEnabled,
    g_staticBufTable.isEnabled
  );

  SSetProcId(selfProcId);

  /* All the System tasks(threads)  are created with same priority*/
  SCreateSTsk(PRIOR0, &egtp_udh_taskId);
  SCreateSTsk(PRIOR0, &tucl_taskId);

  /* Register GTP TAPA Task */
  if (ROK != regAndAttachTsk("GTP", ENTEG, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, egActvInit, (ActvTsk)egActvTsk, egtp_udh_taskId))
  {
   RETVALUE(RFAILED);
  }

  /* Register UDH TAPA Task */
  if (ROK != regAndAttachTsk("UDH", ENTUDH, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, udhActvInit, (ActvTsk)udhActvTsk, egtp_udh_taskId))
  {
   RETVALUE(RFAILED);
  }

  /* Register TUCL TAPA Task */
  if (ROK != regAndAttachTsk("TUCL for GTP & SCTP", ENTHI, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, hiActvInit, (ActvTsk)hiActvTsk, tucl_taskId))
  {
   RETVALUE(RFAILED);
  }

   ipcInit(selfProcId,OAM_PROC_ID,MAX_PROC_ID);
   
   regDriverTask(1,OAM_PROC_ID);
   regDriverTask(2,CP_PROC_ID);
#ifdef SS_4GMX_UCORE
   regDriverLwrArmTask(SVSR_CHANNEL, RADIOUP_PROC_ID);
#else
   regDriverTask(3,RADIOUP_PROC_ID);
#endif

  RETVALUE(ROK);
} /* end of gtpMain function */

/**
 * @details   Application entry point in Data Plane Executable(gtp.exe)..
 */
PUBLIC S16 psMain(Void)
{
//   allocSendBuf();
   if (0 EQ strncmp(g_exeName, "gtp.exe", strlen("gtp.exe")))
   { /* GTP */
     gtpMain(GTP_PROC_ID);
   }
   else
   {
     fprintf(stderr,"ERROR: Unknown executable name = %s. Valid ones are {cp.exe, gtp.exe, radioup.exe}. Exiting\n", g_exeName);
     exit(EXIT_FAILURE);
   }
  RETVALUE(ROK);
}
