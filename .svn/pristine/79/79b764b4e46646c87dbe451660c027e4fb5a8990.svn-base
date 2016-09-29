/**
 * @file     radioup_main.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains Entry point(Main function) to RADIOUP(Radio User and Control Plane ..,PHY, CL, MAC, RLC, PDCP)
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

/**
 * @param     selfProcId  RADIOUP Process Identifier which is used to communicate between Other process in eNodeB
 * @details   Main function for Radio user and control Plane Executable..
 */
PUBLIC S16 radioupMain(S16 selfProcId)
{
   /* System tasks that are needed as per the architecture */

   SSTskId rlc_pdcp_mac_cl_taskId;
   UINT8   logFileName[SIZE_UEH_LOG_FILENAME];
   FILE    *filePtr;

   snprintf((char *)logFileName, SIZE_UEH_LOG_FILENAME , "%s/log/%s_ccpu.log", g_rootDir, g_exeName );
   filePtr = freopen((char *)logFileName, "w", stdout);
   setvbuf( filePtr, NULL, _IONBF /*_IOLBF*/, 30);

   logInit((UINT8 *)g_exeName,(UINT8 *)g_rootDir);
   SSetProcId(selfProcId);

   /* All the System tasks(threads)  are created with same priority*/
   SCreateSTsk(PRIOR0, &rlc_pdcp_mac_cl_taskId);

   /* Register RLC TAPA Task */
   if (ROK != regAndAttachTsk("RLC", ENTKW, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, kwActvInit, (ActvTsk)kwActvTsk, rlc_pdcp_mac_cl_taskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register PDCP TAPA Task */
   if (ROK != regAndAttachTsk("PDCP", ENTPJ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, pjActvInit, (ActvTsk)pjActvTsk, rlc_pdcp_mac_cl_taskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register MAC TAPA Task */
   if (ROK != regAndAttachTsk("MAC", ENTRG, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, rgActvInit, (ActvTsk)rgActvTsk, rlc_pdcp_mac_cl_taskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register MAC SCHEDULER Task */
   if (ROK != regAndAttachTsk("SCH", ENTRG, (Inst)SCH_INST_ID, (Ttype)TTNORM, (Prior)PRIOR0, schActvInit, (ActvTsk)schActvTsk, rlc_pdcp_mac_cl_taskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register CL TAPA Task */
   if (ROK != regAndAttachTsk("CL", ENTYS, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, ysActvInit, (ActvTsk)ysActvTsk, rlc_pdcp_mac_cl_taskId))
   {
     RETVALUE(RFAILED);
   }

   PW_PALInitLibrary(); /* TODO: what is this? */

   ipcInit(selfProcId,OAM_PROC_ID,MAX_PROC_ID);
   
   regDriverTask(1,OAM_PROC_ID);
   regDriverTask(2,CP_PROC_ID);
   regDriverTask(3,GTP_PROC_ID);

   RETVALUE(ROK);
} /* end of psMain function */

/**
 * @details   Application entry point in Radio Data and Control Plane Executable(radioup.exe)..
 */
PUBLIC S16 psMain(Void)
{
   if (0 EQ strncmp(g_exeName, "radioup.exe", strlen("radioup.exe")))
   {
     radioupMain(RADIOUP_PROC_ID); /* Radio User Plane */
   }
   else
   {
     fprintf(stderr,"ERROR: Unknown executable name = %s. Valid ones are {cp.exe, gtp.exe, radioup.exe}. Exiting", g_exeName);
     exit(EXIT_FAILURE);
   }
  RETVALUE(ROK);
}
