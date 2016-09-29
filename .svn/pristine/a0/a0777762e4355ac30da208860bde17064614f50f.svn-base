#include "ccpu_cmn.h"

#include "stdio.h"
#include "string.h"
#include "ueh.h"
#include "log.h"

/* Activation functions */
EXTERN S16 kwActvInit ARGS ((
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
));

EXTERN S16 kwActvTsk ARGS ((
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
));

/* Activation functions */
EXTERN S16 pjActvInit ARGS (( Ent ent, Inst inst, Region region, Reason reason )); 
EXTERN S16 pjActvTsk ARGS (( Pst *pst, Buffer *mBuf));

EXTERN S16 rgActvInit ARGS((Ent entity, Inst inst, Region region, Reason reason));
EXTERN S16 rgActvTsk ARGS((Pst *pst, Buffer *mBuf));

EXTERN S16 schActvInit ARGS((Ent entity, Inst inst, Region region, Reason reason));
EXTERN S16 schActvTsk ARGS((Pst *pst, Buffer *mBuf));

EXTERN S16 ysActvInit ARGS((Ent entity, Inst inst, Region region, Reason reason));
EXTERN S16 ysActvTsk ARGS((Pst *pst, Buffer *mBuf));

PUBLIC S16 l2simMain(S16 selfProcId)
{
  SSTskId systskId, rlctskId, pdcptskId, mactskId, schedtskId, cltskId;

  UINT8   logFileName[SIZE_UEH_LOG_FILENAME];
  FILE    *filePtr;
  logInit((UINT8 *)g_exeName, (UINT8 *)g_rootDir);
  if( NULL EQ g_rootDir )
  {
     fprintf(stderr,"Config Directory is not specified, hence exiting\nEnter [ --rootDir <Path-Of-Config-Directory> ] \n");
     exit(FALSE);
  }
#if 0
  snprintf((char *)logFileName, SIZE_UEH_LOG_FILENAME, "%s/log/%s.log", g_rootDir, g_exeName );

  filePtr = freopen((char *)logFileName, "w", stdout);
  if( NULL EQ filePtr )
  {
    fprintf(stderr,"Could not open log/%s.log file for logging.\n", g_exeName );
  }
  else
  {
    setvbuf( filePtr, NULL, _IONBF /*_IOLBF*/, 30);
  } 
#endif  
   SSetProcId(selfProcId);
   SCreateSTsk(PRIOR0, &systskId);

   systskId = rlctskId = pdcptskId = mactskId = schedtskId = cltskId;
   logInfo("rlc tapa task addrees =%u", kwActvTsk);
   logInfo("rlc tapa task addrees =%u", rgActvTsk);
   logInfo("rlc tapa task addrees =%u", pjActvTsk);
   /* Register RLC TAPA Task */
   if (ROK != regAndAttachTsk("RLC", ENTKW, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, kwActvInit, (ActvTsk)kwActvTsk, rlctskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register MAC TAPA Task */
   if (ROK != regAndAttachTsk("MAC", ENTRG, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, rgActvInit, (ActvTsk)rgActvTsk, mactskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register PDCP TAPA Task */
   if (ROK != regAndAttachTsk("PDCP", ENTPJ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, pjActvInit, (ActvTsk)pjActvTsk, pdcptskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register SCHED TAPA Task */
   if (ROK != regAndAttachTsk("SCHED", ENTRG, (Inst)SCH_INST_ID, (Ttype)TTNORM, (Prior)PRIOR0, schActvInit, (ActvTsk)schActvTsk, rlctskId))
   {
     RETVALUE(RFAILED);
   }
   /* Register CL TAPA Task */
   if (ROK != regAndAttachTsk("CL", ENTYS, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0, ysActvInit, (ActvTsk)ysActvTsk, cltskId))
   {
     RETVALUE(RFAILED);
   }

   ipcInit(selfProcId,OAM_PROC_ID,MAX_PROC_ID);
   regDriverTask(1,OAM_PROC_ID);
   regDriverTask(2,CP_PROC_ID);
   regDriverTask(3,GTP_PROC_ID);
   RETVALUE(ROK);
}

/**
 * @details   Application entry point in e Executable(l2sim.exe)..
 */
PUBLIC S16 psMain(Void)
{
//   allocSendBuf();
   if (0 EQ strncmp(g_exeName, "l2sim.exe", strlen("l2sim.exe")))
   { /* GTP */
     l2simMain(RADIOUP_PROC_ID);
   }
   else
   {
     fprintf(stderr,"ERROR: Unknown executable name = %s. Valid ones are {cp.exe, gtp.exe, radioup.exe, l2sim.exe}. Exiting\n", g_exeName);
     exit(EXIT_FAILURE);
   }
  RETVALUE(ROK);
}

#if 0
#ifdef ANSI
PUBLIC S16 macCfgReq
(
Pst           *pst, 
SpId          spId,
CrgCfgTransId transId,
CrgCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 macCfgReq(pst, spId, transId, cfgReqInfo)
Pst           *pst; 
SpId          spId;
CrgCfgTransId transId;
CrgCfgReqInfo *cfgReqInfo;
#endif
{
  rgUIMCrgCfgCfm(transId, CRG_CFG_CFM_OK); 
}


#ifdef ANSI
PUBLIC S16 schedCfgReq
(
Pst           *pst, 
SpId          spId,
RgrCfgTransId transId,
RgrCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 schedCfgReq(pst, spId, transId, cfgReqInfo)
Pst           *pst; 
SpId          spId;
RgrCfgTransId transId;
RgrCfgReqInfo *cfgReqInfo;
#endif
{
   Inst instId = pst->dstInst-RGSCH_INST_START;
   rgSCHUtlRgrCfgCfm(instId, spId, transId, RGR_CFG_CFM_OK);
}
/*
#ifdef ANSI
PUBLIC S16 siCfgReq
(
Pst           *pst, 
SpId          spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 siCfgReq(pst, spId, transId, cfgReqInfo)
Pst           *pst; 
SpId          spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo *cfgReqInfo;
#endif
{
   Inst      instId = pst->dstInst-RGSCH_INST_START;
   rgSCHUtlRgrSiCfgCfm(instId, spId, transId,RGR_CFG_CFM_OK ); 
}
*/

#ifdef ANSI
PUBLIC S16 clCfgReq
(
Pst*                 pst,
SpId                 spId,
CtfCfgTransId        transId,
CtfCfgReqInfo*       cfgReqInfo
)
#else
PUBLIC S16 clCfgReq(pst, spId, transId, cfgReqInfo)
Pst*                 pst;
SpId                 spId;
CtfCfgTransId        transId;
CtfCfgReqInfo*       cfgReqInfo;
#endif
{
      YsUiCtfCfgCfm(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId, transId, CTF_CFG_CFM_OK);
      RETVALUE(ROK);
}
#endif
