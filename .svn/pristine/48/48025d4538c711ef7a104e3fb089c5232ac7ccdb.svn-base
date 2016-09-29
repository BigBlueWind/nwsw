/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/
/********************************************************************20**

     Name:

     Type:     C File.

     Desc:    Supervisor is used to communicate with upper arm
              Using shared memory

     File:     rb_icpu.c

     Sid:      ju_icpu.c@@/main/1 - Mon Aug 29 22:31:33 2011

     Prg:

************************************************************************/

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "rb_include.h"
#include "mlog.h"
#include "appinit.h"


#ifdef SS_4GMX_LCORE

PUBLIC APIHEC rbICpuSvsrHndlr (LPVOID pClientCtx,ApiHeader *pApi,ApiHeader *pResp);
#ifdef SS_DRVR_SUPPORT

/*
 *
 *     Fun  : rbDrvrInit
 *
 *     Desc : Initializes lowercore driver task to facilitate communication with
 *     upper arm.
 *
 *     Ret:   ROK      - ok
 *            RFAILED  - failed, general (optional)
 *
 *     Notes: None
 *
 *     File : rb_icpu.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbDrvrInit
(
void
)
#else
PUBLIC S16 rbDrvrInit(void)
#endif
{
   S16 ret;

   TRC0(rbDrvrInit);

   /* Channel initialized to handle messages targetted towards
    * SVSR channels, And Vice versa.
    */
   ret = SRegDrvrTsk(SVSR_CHANNEL, MT_PROCESSOR_ID_LOW, MT_PROCESSOR_ID_HIGH, 
                        rbICpuPstTsk, NULL);
   if (ret != ROK)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB030, ERRZERO,
            "SRegDrvrTsk failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /*end of rbDrvrInit*/
#endif


/*
 *
 *     Fun  : rbICpuInit
 *
 *     Desc : Registers client with inter CPU supervisor channel
 *
 *     Ret:   ROK      - ok
 *            RFAILED  - failed, general (optional)
 *
 *     Notes: None
 *
 *     File : rb_icpu.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbICpuInit
(
void
)
#else
PUBLIC S16 rbICpuInit()
#endif
{
   U32 CoreID = MxGetCpuID();
   U32 InstId = IID_CORE + CoreID;
   SsAppCtx *pCtx = NULL;

   TRC0(rbICpuInit);

   pCtx = (SsAppCtx *) &osCp.dep.pICpuCtx[CoreID];

   SMemSet(pCtx,0,sizeof(SsAppCtx));

   if(MX_OK != SvsrRegClient(InstId, rbICpuSvsrHndlr,  pCtx))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB031, ERRZERO,"ERROR: Unable to register SSI application \r\n");
      RETVALUE(RFAILED);
   }

   pCtx->nInstanceID = InstId;
   RB_PRINTF("\n[%s:%d] coreId=%d InstId=%d nInstanceID=%d pCtx=%p\n",
         __func__, __LINE__, CoreID, InstId, pCtx->nInstanceID, pCtx);

   RETVALUE(ROK);
} /*end of rbCfgInit */


/*
 *
 *     Fun  : rbCfgInit
 *
 *     Desc : Initializes larm ICPU configuration.
 *             This function should be called after
 *             all the TAPA tasks get registered.
 *
 *     Ret:   ROK      - ok
 *            RFAILED  - failed, general (optional)
 *
 *     Notes: None
 *
 *     File : rb_icpu.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbCfgInit
(
void
)
#else
PUBLIC S16 rbCfgInit()
#endif
{
   U16 idx = 0;
   U16 i   = 0;
   SsRegIcpuCfg icpuCfg[SS_MAX_TTSKS] = ICPUINIT;

   TRC0(rbCfgInit);

   /*Initialize inter cpu configuration*/
   for(i=0;i<osCp.numTTsks;i++)
   {
      idx = osCp.tTskIds[icpuCfg[i].Ent][icpuCfg[i].Inst];
      /* verify that it hasn't been deregistered */
      if (idx == SS_TSKNC)
      {
         RBLOGERROR(ERRCLS_DEBUG, ERB032, ERRZERO,"Task not registered \r\n");
         RETVALUE(RFAILED);
      }

      osCp.tTskTbl[idx].dep = icpuCfg[i].CoreId;
   }

   RETVALUE(ROK);
} /*end of rbCfgInit*/


/*
 *
 *     Fun  : rbGetCoreId
 *
 *     Desc : Provides the core id corresponding to ent and inst ID
 *
 *     Ret:  core id - SUCCESS
 *           -1 - FAILURE
 *
 *     Notes: None
 *
 *     File : rb_icpu.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetCoreId
(
U32 ent,
U32 inst
)
#else
PUBLIC S16 rbGetCoreId(ent,inst)
U32 ent;
U32 inst;
#endif
{
   U16 idx = 0;

   TRC0(rbGetCoreId);

   idx = osCp.tTskIds[ent][inst];
   /* verify that it hasn't been deregistered */
   if (idx == SS_TSKNC)
   {
      return -1;
   }

   return osCp.tTskTbl[idx].dep;
} /*end of rbGetCoreId*/


/*
 *
 *       Fun:   rbICpuPstTsk
 *
 *       Desc:  This function serves as a Tx for reaching the entities and instances on linux.
 *              Basically its a message post from lower ARM to
 *              upper ARM using shared memory.
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - failed, general (optional)
 *
 *       Notes:
 *
 *       File:  rb_icpu.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbICpuPstTsk
(
Pst *pst,                       /* post information */
Buffer *mBuf                    /* message to post */
)
#else
PUBLIC S16 rbICpuPstTsk(pst, mBuf)
Pst *pst;                       /* post information */
Buffer *mBuf;                   /* message to post */
#endif
{
   S16       ret;
   MsgLen    bufLen;
   MsgLen    msgLen;
   Data      *pMsg;
   ApiHeader *pApi  = NULLP;
   U32       CoreID = MxGetCpuID();
   SsAppCtx  *pCtx  = (SsAppCtx *) &osCp.dep.pICpuCtx[CoreID];
   volatile int t = *(volatile U32 *) 0xFE050004;
   U32        var1;
   TRC0(rbICpuPstTsk);

   if (!pst || !mBuf)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB033, ERRZERO,"Null Buffer/Pst pointers \n");
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   CMCHKPK(cmPkPst, pst, mBuf);

   ret = SFndLenMsg(mBuf, &bufLen);

   if (bufLen > (MAX_ICPU_BUF_SIZE - sizeof (ApiHeader)))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB034, ERRZERO,"Buffer length is greater than MAX_BUF_SIZE\n");
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   t = *(volatile U32 *) 0xFE050004;
   pApi = SvsrAllocICApi();
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_SVSRALLOC_ICMSG, RESOURCE_LARM, t, *(volatile U32 *) 0xFE050004);
#endif
   if(pApi == NULLP)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB035, ERRZERO,"Cant allocate ICPU\n");
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   pApi->Control   = API_ENCAP_FM;
   //pApi->DstInstID = pCtx->nMacInstanceId; //L2Split
   pApi->DstInstID = 3; //pCtx->nMacInstanceId;
   pApi->SrcInstID = pCtx->nInstanceID; /* IID_EXT_APP for interoperability with old implementation of MSPD eNodeB */
   pApi->Type      = API_TYPE_DATA;
   pApi->MessageID = PHY_LTE_MSG;
   pApi->Length    = bufLen;

   pMsg = (Data*)((PUINT8)pApi + sizeof (ApiHeader));

   ret = SCpyMsgFix(mBuf, 0, bufLen, pMsg, &msgLen);
   if((ret != ROK) || (bufLen != msgLen))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB036, ERRZERO,"\n SAddPstMsgMult failed \n");
      SvsrFreeICApi(pApi);
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   /*Done with the mBuf, put it away*/
   SPutMsg(mBuf);

   t = *(volatile U32 *) 0xFE050004;
   ret = SvsrSendICApi(pApi);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_SVSRSEND_ICMSG, RESOURCE_LARM, t, *(volatile U32 *) 0xFE050004);
#endif
   var1 = bufLen;
   MLogAddVariables(1, &var1, t);
   if(ret != 0)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB037, ERRZERO,"Call to SvsrSendApi failed\n");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /*end of rbICpuPstTsk*/


/*
 *
 *       Fun:   rbICpuSvsrHndlr
 *
 *       Desc:  This primitive is a callback from Syscore for
 *              handling messages from upper arm
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - failed, general (optional)
 *
 *
 *       Notes: None
 *
 *       File:  rb_icpu.c
 *
 */
#ifdef ANSI
PUBLIC APIHEC rbICpuSvsrHndlr
(
LPVOID     pClientCtx,
ApiHeader  *pApi,
ApiHeader  *pResp
)
#else
PUBLIC APIHEC rbICpuSvsrHndlr(pClientCtx, pApi, pResp)
LPVOID     pClientCtx;
ApiHeader  *pApi;
ApiHeader  *pResp;
#endif
{
   S16          ret;
   SsIdx       idx;
   SsTTskEntry *tTsk;
   SsMsgInfo   *mInfo;
   Pst          nPst;
   Data         *pMsg;
   Buffer       *mBuf;
   PRIVATE  S8  firstMsg = 1;
   U32 tick = *(volatile U32 *) 0xFE050004;
   U32 tick2;
   U32          CoreID   = MxGetCpuID();
   SsAppCtx     *pCtx    = (SsAppCtx *) &osCp.dep.pICpuCtx[CoreID];
   extern SysStayInfo syscoreinfo [4];

   TRC0(rbICpuSvsrHndlr);

   if (pClientCtx == NULL)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB038, ERRZERO,"pClientCtx is NULL\n");
      RETVALUE(APIHEC_DROP_RESP);
   }

   /* Retrieve the data beyond ApiHeader*/
   pMsg = (UINT8*)((PUINT8)pApi + sizeof (ApiHeader));
   if(pMsg == NULLP)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB039, ERRZERO,"pMsg is NULL\n");
      RETVALUE(APIHEC_DROP_RESP);
   }

   if (firstMsg > 0)
   {
      /* This is generally MSGS_MAC(=3) */
      pCtx->nMacInstanceId = pApi->SrcInstID;
      MxSetThreadPriority(MxGetCurrentThread(),RB_ICPU_PRIORITY);
      RB_PRINTF("\n[%s:%d] coreId=%d nMacInstanceId=%d pApi->SrcInstID=%d nInstanceID=%d pCtx=%p\n", __func__, __LINE__, CoreID, pCtx->nMacInstanceId, pApi->SrcInstID, pCtx->nInstanceID, pCtx);
      firstMsg--;
   }

   ret = SGetMsg(CL_REGION, CL_POOL+1, &mBuf);
   if (ret != ROK)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB040, ERRZERO,"SGetMsg failed\n");
      RETVALUE(APIHEC_DROP_RESP);
   }

   ret = SAddPstMsgMult((Data*)pMsg, pApi->Length, mBuf);
   if(ret != ROK)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB041, ERRZERO,"SAddPstMsgMult failed\n");
      SPutMsg(mBuf);
      RETVALUE(APIHEC_DROP_RESP);
   }
   mInfo = (SsMsgInfo *)mBuf->b_rptr;

   syscoreinfo[MxGetCpuID()].timestamp = *(volatile U32 *) 0xFE050004;
   syscoreinfo[MxGetCpuID()].res[0] = 2; /* 2 is for ICPU msg */
   syscoreinfo[MxGetCpuID()].res[1] = 1; /* 1 is for start of the task */  

   CMCHKUNPK(cmUnpkPst, &nPst, mBuf);
   {
      U32   mlogDebug[5];
	  syscoreinfo[MxGetCpuID()].mBuf	= (unsigned int)mBuf;
	  syscoreinfo[MxGetCpuID()].event	= (unsigned int)nPst.event;
	  syscoreinfo[MxGetCpuID()].dstEnt	= (unsigned int)nPst.dstEnt;
	  syscoreinfo[MxGetCpuID()].dstInst = (unsigned int)nPst.dstInst;
	  syscoreinfo[MxGetCpuID()].srcEnt	= (unsigned int)nPst.srcEnt;
	  syscoreinfo[MxGetCpuID()].timestamp = *(volatile U32 *) 0xFE050004;
	  syscoreinfo[MxGetCpuID()].res[0] = 2; /* 2 is for ICPU msg */
	  syscoreinfo[MxGetCpuID()].res[1] = 2; /* 2 is unpack of pst done */  
      mlogDebug[0] = (unsigned int)mBuf;
      mlogDebug[1] = (unsigned int)nPst.event;
      mlogDebug[2] = (unsigned int)nPst.dstEnt;
      mlogDebug[3] = (unsigned int)nPst.dstInst;         
      mlogDebug[4] = (unsigned int)nPst.srcEnt;         
      //MLogAddVariables(5, mlogDebug, PID_SSI_RECICPU_MSG);
      MLogAddVariables(5, mlogDebug, tick);
   }
   
	if (mInfo->eventInfo.event == SS_EVNT_DATA)
   {
      idx = osCp.tTskIds[nPst.dstEnt][nPst.dstInst];
      /* verify that it hasn't been deregistered */
      if (idx == SS_TSKNC)
      {
         SPutMsg(mBuf);
         RB_PRINTF("\n mtTskHandler tsk:ret from idx == SS_TSKNC \n");
         RETVALUE(APIHEC_DROP_RESP);
      }
      tTsk = &osCp.tTskTbl[idx]; 
      ret = tTsk->actvTsk(&nPst, mBuf);
	  {
		extern SysStayInfo syscoreinfo [4];
		syscoreinfo[MxGetCpuID()].res[1] = 255; /* 255 denotes the end of this task */
      }
   }
   if(ROK != ret)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB042, ERRZERO,"SPstTsk failed\n");
   }

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_SSI_ICPU_MSG, RESOURCE_LARM, tick, tick2 = *(volatile U32 *) 0xFE050004);
   //MLogTask(PID_L2_PROCESSING, RESOURCE_LARM, tick, tick2);
#endif
   RETVALUE(APIHEC_DROP_RESP);
} /* end of rbICpuSvrHndlr */

#endif


/********************************************************************30**

         End of file:     ju_icpu.c@@/main/1 - Mon Aug 29 22:31:33 2011

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

   ver        pat        init                  description
---------  ---------  ----------  -------------------------------------
/main/1      --------     ------    1. Initial release
*********************************************************************91*/
