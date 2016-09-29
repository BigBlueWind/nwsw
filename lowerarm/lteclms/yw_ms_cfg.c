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

/************************************************************************

     Name:     LTE MAC Convergenc Layer

     Type:     C source file

     Desc:     C source code for Entry point fucntions

     File:     ys_ms_cfg.c

     Sid:      ys_ms_cfg.c@@/main/1 - Sun Mar 28 21:19:19 2010

     Prg:      pk

**********************************************************************/

/** @file ys_ms_cfg.c
@brief This module acts as an interface handler for upper interface and
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "ctf.h"           /* CTF defines */
#include "lys.h"           /* layer management defines for LTE-CL */
#include "tfu.h"
#include "ys_ms.h"            /* defines and macros for CL */

/* Silicon Includes */
#include "LtePhyL2Api.h"
#include "apidefs.h"

#include "ys_ms_err.h"        /* YS error defines */

#ifdef TENB_AS_SECURITY
#include "spaccdrv.h" /*sumanth*/
#endif


/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "ctf.x"           /* CTF types */
#include "lys.x"           /* layer management typedefs for CL */
#include "tfu.x"
#include "ys_ms.x"            /* typedefs for CL */

/*ys004.102 :  Merged MSPD code with phy 1.7 */
typedef S16 (*YsMsSm) ARGS((YsCellCb *cellCb, Void *status));
PRIVATE YsMsSm ysMsSmStateMtx[YS_MS_MAX_STATES][YS_MS_MAX_EVNT];

/* Function Prototype */
PRIVATE S16 ysMsSmInvSE ARGS((YsCellCb *cellCb, Void *status));
PRIVATE S16 ysMsSmStIdleEvtCfg ARGS((YsCellCb *cellCb, Void *status));
PRIVATE S16 ysMsSmStIdleEvtCfgRsp ARGS((YsCellCb *cellCb, Void *status));
PRIVATE S16 ysMsSmStCfgEvtStartRsp ARGS((YsCellCb *cellCb, Void *status));
#ifdef YS_PHY_STOP_AUTO
PUBLIC S16 ysMsSmStRunEvtStop ARGS((YsCellCb *cellCb, Void *status));
#else
PRIVATE S16 ysMsSmStRunEvtStop ARGS((YsCellCb *cellCb, Void *status));
#endif
PRIVATE S16 ysMsSmStRunEvtStopRsp ARGS((YsCellCb *cellCb, Void *status));

/* Adding Prototype as this function defination is available in ad9361radio.c file in PHY */
EXTERN MXRC Ad9361IRadioScheduleInit ARGS ((UINT32 nProfID, UINT32 NumTxAntennas, UINT32 NumRxAntennas, 
			                    UINT32 loopback, UINT32 rxfreq, UINT32 txfreq));

#define ysMsSmStCfgEvtCfg ysMsSmStIdleEvtCfg
#define ysMsSmStCfgEvtCfgRsp ysMsSmStIdleEvtCfgRsp
#define ysMsSmStRunEvtStartRsp ysMsSmStCfgEvtStartRsp

/*
*
*       Fun:    ysMsCfgSm
*
*       Desc:   This function invoke state machine
*
*       Ret:    ROK: if the event can be processed.
*               RFAILED               - FAILURE
*
*       Notes:  None
*
*       File:   ys_ms_cfg.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsCfgSm
(
YsCellCb   *cellCb,
U8         event,
Void       *status
)
#else
PUBLIC S16 ysMsCfgSm(cellCb, event, status)
YsCellCb   *cellCb;
U8         event;
Void       *status;
#endif
{
   S16 ret;

   TRC3(ysMsCfgSm);

   YS_DBG_INFO((_ysp, "Inside ysMsCfgSm. Event = %d \n", event));
   MSPD_DBG( "Inside ysMsCfgSm. Event = %d \n", event);


   ret = ysMsSmStateMtx[cellCb->phyState][event](cellCb, status);

   RETVALUE(ret);
} /* ysMsCfgSm */


/* In this matrix the rows represent states of the PHY and columns
 * represent events on the phy.
 *
 * State Index numbers:
 * ====================
 *
 * LYS_PHY_STATE_IDLE          ==> 0
 * LYS_PHY_STATE_CFG           ==> 1
 * LYS_PHY_STATE_RUN           ==> 2
 *
 *
 * The columns represent the events that can happen on a Phy.
 * Column Indexes are:
 *
 * YS_MS_INV_EVENT            ==> 0
 * YS_MS_EVENT_CFG            ==> 1
 * YS_MS_EVENT_CFG_RSP        ==> 2
 * YS_MS_EVENT_START_RSP      ==> 3
 * YS_MS_EVENT_STOP           ==> 4
 * YS_MS_EVENT_STOP_RSP       ==> 5
 *
 * The function handlers are named as:
 * ysMsSm<State Number><Event Number>
 * eg: ysMsSmS2E1 => Represents handler for the event Configuration
 *                      in the state running
 *
 */

PRIVATE YsMsSm ysMsSmStateMtx[YS_MS_MAX_STATES][YS_MS_MAX_EVNT] =
{
   /* INV,        CFG,               CFG_RSP,               START_RSP,
                     STOP,                STOP_RSP */
   {ysMsSmInvSE, ysMsSmStIdleEvtCfg, ysMsSmStIdleEvtCfgRsp, ysMsSmInvSE,
                     ysMsSmInvSE,         ysMsSmInvSE},
   {ysMsSmInvSE, ysMsSmStCfgEvtCfg, ysMsSmStCfgEvtCfgRsp,   ysMsSmStCfgEvtStartRsp,
                     ysMsSmInvSE,         ysMsSmInvSE},
   {ysMsSmInvSE, ysMsSmInvSE,        ysMsSmInvSE,           ysMsSmStRunEvtStartRsp,
                     ysMsSmStRunEvtStop,  ysMsSmStRunEvtStopRsp}
};

/*
*
*       Fun:    ysMsSmInvSE
*
*       Desc:   This function handles the invalid event or state.
*
*       Ret:    ROK: if the event can be processed.
*               RFAILED               - FAILURE
*
*       Notes:  None
*
*       File:   ys_ms_cfg.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsSmInvSE
(
YsCellCb   *cellCb,
Void       *status
)
#else
PRIVATE S16 ysMsSmInvSE(cellCb, status)
YsCellCb   *cellCb;
Void       *status;
#endif
{

   TRC3(ysMsSmInvSE);

   YS_DBG_ERR((_ysp, "Inside ysMsSmInvSE \n"));

   RETVALUE(RFAILED);
} /* ysMsSmInvSE */

/*
*
*       Fun:    ysMsSmStIdleEvtCfg
*
*       Desc:   This function handles the YS_MS_EVENT_CFG event in
*               state LYS_PHY_STATE_IDLE.
*
*       Ret:    ROK: if the event can be processed.
*               RFAILED               - FAILURE
*
*       Notes:  None
*
*       File:   ys_ms_cfg.c
*
*/

#define MY_1ENABLE_SVSR_API_LOGGER_DEF_MASK         ((1<<12)|(1<<17))

#ifdef ANSI
PRIVATE S16 ysMsSmStIdleEvtCfg
(
YsCellCb   *cellCb,
Void       *status
)
#else
PRIVATE S16 ysMsSmStIdleEvtCfg(cellCb, status)
YsCellCb   *cellCb;
Void       *status;
#endif
{
#ifndef YS_JMP_PHY_RUN_STE
   PGENMSGDESC  pMsgDesc;
   PINITPARM    msCfgReq;
   U32          l1Size;
#endif
   /* HARQ: PHY_API_CHANGE */
   PMAC2PHY_QUEUE_EL pElem;
   S16          ret;

   TRC3(ysMsSmStIdleEvtCfg);

   YS_DBG_INFO((_ysp, "Inside ysMsSmStIdleEvtCfg \n"));

   SvsrLoggerSetMask(MY_1ENABLE_SVSR_API_LOGGER_DEF_MASK);

#ifndef YS_JMP_PHY_RUN_STE
   l1Size = sizeof(GENMSGDESC) + sizeof(INITPARM);

   pMsgDesc     = (PGENMSGDESC) ysMsUtlAllocSvsrMsg(FALSE);

   msCfgReq = (PINITPARM)(pMsgDesc + 1);

   pMsgDesc->msgType     = PHY_INIT_REQ;
   pMsgDesc->phyEntityId = YS_PHY_INST_ID;
   pMsgDesc->msgSpecific = sizeof(INITPARM);

   ret = ysMsUtlFillCfgReq(msCfgReq, cellCb);

   /* PHY_API_CHANGE : Fill the linked list element */
   pElem = ysMsUtlGetPhyListElem();
   if (!pElem)
   {
      /* HARQ_DBG */
      uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element for\
      PHY_INIT_REQ at cellTime(%d,%d)\n",
      cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
      RETVALUE(RFAILED);
   }
   YS_MS_FILL_PHY_LIST_ELEM(pElem, cellCb->timingInfo.sfn, \
         cellCb->timingInfo.subframe,
   	  pMsgDesc, l1Size, PHY_INIT_REQ);

/*   DiagDumpPhyApi((PTR)pMsgDesc);*/
   ysMsLimSendToPhy(l1Size, (PGENMSGDESC)pElem);

#else
   /*Set the PHY State to CFG */
   cellCb->phyState = LYS_PHY_STATE_CFG;

   /*Simulate the receipt of START_CFM primitive from PHY,
       here status parameter is unused in the invoked function*/
   ret = ysMsCfgSm(cellCb, YS_MS_EVENT_START_RSP, status);
   uart_printf("PHY state is RUN, sent the Cell CfgCfm @CTF\n");
#endif

   RETVALUE(ret);
} /* ysMsSmStIdleEvtCfg */

/*
*
*       Fun:    ysMsSmStIdleEvtCfgRsp
*
*       Desc:   This function handles the YS_MS_EVENT_CFG_RSP event in
*               state LYS_PHY_STATE_IDLE.
*
*       Ret:    ROK: if the event can be processed.
*               RFAILED               - FAILURE
*
*       Notes:  None
*
*       File:   ys_ms_cfg.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsSmStIdleEvtCfgRsp
(
YsCellCb   *cellCb,
Void       *status
)
#else
PRIVATE S16 ysMsSmStIdleEvtCfgRsp(cellCb, status)
YsCellCb   *cellCb;
Void       *status;
#endif
{
   S16         ret;
   PINITIND    initInd;
   PSTARTREQ   startReq;
   U32         l1Size;
   PGENMSGDESC pMsgDesc;
   /* PHY_API_CHANGE */
   PMAC2PHY_QUEUE_EL pElem;

   TRC3(ysMsSmStIdleEvtCfgRsp);

   YS_DBG_INFO((_ysp, "Inside ysMsSmStIdleEvtCfgRsp \n"));

   initInd = (PINITIND)status;

   if(initInd->status != SUCCESS)
   {
      /* TODO free the INITIND */
      YS_DBG_ERR((_ysp, "Phy Init Failed. Error = %d\n", initInd->status));
      MSPD_ERR("Phy Init Failed. Error = %d\n", initInd->status);
      YsUiCtfCfgCfm(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId,
                     ysCb.ctfSap.transId, CTF_CFG_CFM_NOK);
      RETVALUE(RFAILED);
   }
   else
   {
      /* Who is going to free INITIND ??*/
      /* TODO free the INITIND */
   }

   l1Size = sizeof(GENMSGDESC) + sizeof(STARTREQ);

      /* Allocate a message towards Phy using sysCore */
      pMsgDesc = (PGENMSGDESC) ysMsUtlAllocSvsrMsg (FALSE);
      if (pMsgDesc == NULLP)
      {
         YS_DBG_ERR((_ysp, "Unable to allocate memory for TxVector"));
         MSPD_ERR("Unable to allocate memory for TxVector");
         RETVALUE (RFAILED);
      }

   startReq = (PSTARTREQ)(pMsgDesc );

/* Filling the band ID for automating the ADI ID */
   MSPD_DBG(" cellCb->cellCfg.antennaCfg.antPortsCnt %d\n", \
         cellCb->cellCfg.antennaCfg.antPortsCnt);

   if (cellCb->cellCfg.opMode == 4)
   {
      U8 retVal = 1;

      if (cellCb->cellCfg.antennaCfg.antPortsCnt == CTF_AP_CNT_2)
         retVal = Ad9361IRadioScheduleInit(cellCb->cellInfo.bandId,2,2,0,0,0) & 0xFF;
      else if (cellCb->cellCfg.antennaCfg.antPortsCnt == CTF_AP_CNT_1)
         retVal = Ad9361IRadioScheduleInit(cellCb->cellInfo.bandId,1,1,0,0,0) & 0xFF;

      if (retVal)
      {
         YS_DBG_ERR((_ysp, "ysMsSmStIdleEvtCfgRsp(): ADI comand Failed \n"));
         RETVALUE(RFAILED) ;
      }

   }

   ret = ysMsUtlFillStartReq(startReq, cellCb);
   if (ret != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsSmStIdleEvtCfgRsp(): StartReq Failed \n"));
      RETVALUE(RFAILED);
   }

   /* PHY_API_CHANGE : Fill the linked list element */
   pElem = ysMsUtlGetPhyListElem();
   if (!pElem)
   {
      /* HARQ_DBG */
      uart_printf("SvsrAllocMsg failed for PHY list element for \
            PHY_START_REQ at time(%d,%d)\n",
      cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
      RETVALUE(RFAILED);
   }
   YS_MS_FILL_PHY_LIST_ELEM(pElem, cellCb->timingInfo.sfn,
         cellCb->timingInfo.subframe,
   	  pMsgDesc, l1Size, PHY_START_REQ);

   cellCb->phyState = LYS_PHY_STATE_CFG;

   ysMsLimSendToPhy(l1Size, (PGENMSGDESC)pElem);


   RETVALUE(ret);
} /* ysMsSmStIdleEvtCfgRsp */

/*
*
*       Fun:    ysMsSmStCfgEvtStartRsp
*
*       Desc:   This function handles the YS_MS_EVENT_START_RSP event in
*               state LYS_PHY_STATE_CFG.
*
*       Ret:    ROK: if the event can be processed.
*               RFAILED               - FAILURE
*
*       Notes:  None
*
*       File:   ys_ms_cfg.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsSmStCfgEvtStartRsp
(
YsCellCb   *cellCb,
Void       *status
)
#else
PRIVATE S16 ysMsSmStCfgEvtStartRsp(cellCb, status)
YsCellCb   *cellCb;
Void       *status;
#endif
{
   PINITIND   initInd;

   TRC3(ysMsSmStCfgEvtStartRsp);

   YS_DBG_INFO((_ysp, "Inside ysMsSmStCfgEvtStartRsp \n"));

   /*
      crude workaround: un-comment this code once fix is there in the PHY 
      to send proper
           MSGT_RESPONSE with PHY_START_CONF for the PHY_START_REQ msg
	   */

   initInd = (PINITIND)status;

   if(initInd->status != SUCCESS)
   {
      YS_DBG_ERR((_ysp, "Starting Phy Failed. Error = %d\n", initInd->status));
      MSPD_ERR("Starting Phy Failed. Error = %d\n", initInd->status);
      YsUiCtfCfgCfm(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId,
                     ysCb.ctfSap.transId, CTF_CFG_CFM_NOK);
      RETVALUE(RFAILED);
   }

   YsUiCtfCfgCfm(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId,
                     ysCb.ctfSap.transId, CTF_CFG_CFM_OK);

   cellCb->phyState = LYS_PHY_STATE_RUN;

   RETVALUE(ROK);
} /* ysMsSmStCfgEvtStartRsp */

/*
*
*       Fun:    ysMsSmStRunEvtStop
*
*       Desc:   This function handles the YS_MS_EVENT_STOP event in
*               state LYS_PHY_STATE_RUN.
*
*       Ret:    ROK: if the event can be processed.
*               RFAILED               - FAILURE
*
*       Notes:  None
*
*       File:   ys_ms_cfg.c
*
*/

#ifdef YS_PHY_STOP_AUTO
#ifdef ANSI
PUBLIC S16 ysMsSmStRunEvtStop
(
YsCellCb   *cellCb,
Void       *status
)
#else
PUBLIC S16 ysMsSmStRunEvtStop(cellCb, status)
YsCellCb   *cellCb;
Void       *status;
#endif
#else
#ifdef ANSI
PRIVATE S16 ysMsSmStRunEvtStop
(
YsCellCb   *cellCb,
Void       *status
)
#else
PRIVATE S16 ysMsSmStRunEvtStop(cellCb, status)
YsCellCb   *cellCb;
Void       *status;
#endif
#endif
{
   PGENMSGDESC stopReq;
   S16         ret;
   /* PHY_API_CHANGE */
   PMAC2PHY_QUEUE_EL pElem;

   TRC3(ysMsSmStRunEvtStop);

   YS_DBG_INFO((_ysp, "Inside ysMsSmStRunEvtStop \n"));

   /* Allocate a message towards Phy using sysCore */
   stopReq = (PGENMSGDESC) ysMsUtlAllocSvsrMsg (FALSE);
   if (stopReq == NULLP)
   {
      YS_DBG_ERR((_ysp, "Unable to allocate memory for TxVector"));
      MSPD_ERR("Unable to allocate memory for TxVector");
      RETVALUE (RFAILED);
   }

   ret = ysMsUtlFillStopReq(stopReq, cellCb);
   if (ret != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsSmStRunEvtStop(): cellCfg Failed \n"));
      RETVALUE(RFAILED);
   }

   /* PHY_API_CHANGE : Fill the linked list element */
   pElem = ysMsUtlGetPhyListElem();
   if (!pElem)
   {
      /* HARQ_DBG */
      uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element for \
            PHY_STOP_REQ at time (%d,%d)\n",
      cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
      RETVALUE(RFAILED);
   }
   YS_MS_FILL_PHY_LIST_ELEM(pElem, cellCb->timingInfo.sfn,
         cellCb->timingInfo.subframe,
        stopReq, sizeof(GENMSGDESC), PHY_STOP_REQ);
   MSPD_DBG("PST: sending PHY_STOP_REQ to PHY at time(%d,%d) \n",cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);

   ysMsLimSendToPhy(sizeof(GENMSGDESC), (PGENMSGDESC)pElem);

   RETVALUE(ret);
} /* ysMsSmStRunEvtStop */

/*
*
*       Fun:    ysMsSmStRunEvtStopRsp
*
*       Desc:   This function handles the YS_MS_EVENT_STOP_RSP event in
*               state LYS_PHY_STATE_RUN.
*
*       Ret:    ROK: if the event can be processed.
*               RFAILED               - FAILURE
*
*       Notes:  None
*
*       File:   ys_ms_cfg.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsSmStRunEvtStopRsp
(
YsCellCb   *cellCb,
Void       *status
)
#else
PRIVATE S16 ysMsSmStRunEvtStopRsp(cellCb, status)
YsCellCb   *cellCb;
Void       *status;
#endif
{
   PINITIND   initInd;

   TRC3(ysMsSmStRunEvtStopRsp);

   YS_DBG_INFO((_ysp, "Inside ysMsSmStRunEvtStopRsp \n"));

   initInd = (PINITIND)status;

   if(initInd->status != SUCCESS)
   {
      YS_DBG_ERR((_ysp, "Stoping Phy Failed. Error = %d\n", initInd->status));
      MSPD_ERR("PST: Stoping Phy Failed. Error = %d\n", initInd->status);
      RETVALUE(RFAILED);
   }
   /* Sending configuration confirm to SM ***PST-CellReset*** */
   YsUiCtfCfgCfm(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId, ysCb.ctfSap.transId, CTF_CFG_CFM_OK);
   
   cellCb->phyState = LYS_PHY_STATE_CFG;
   MSPD_DBG("ysMsSmStRunEvtStopRsp PHY STATE is CONFIG = STOPPED\n");

   RETVALUE(ROK);
} /* ysMsSmStRunEvtStopRsp */

/**
 * @brief API for handle cell configuration request from RRM
 *
 * @details
 *
 *     Function: ysMsCfgAddCellCfg
 *
 *     This API for handle cell configuration request from RRM
 *
 *  @param[in]
 *  @param[in]
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 ysMsCfgAddCellCfg
(
CtfCellCfgInfo *cellCfg
)
#else
PUBLIC S16 ysMsCfgAddCellCfg(cellCfg)
CtfCellCfgInfo *cellCfg;
#endif
{
   S16       ret;
   YsCellCb  *cellCb;
   U16       idx;
   YsTfuSapCb *sap;
   YsTfuSapCb *schTfuSap;
   U16        offset;
   YsUeCb     ueCb;

   TRC2(ysMsCfgAddCellCfg)

   YS_DBG_PARAM((_ysp, "ysMsCfgAddCellCfg(): cellId = %d physCellId = %d\n",
                        cellCfg->cellId, cellCfg->physCellId));


   ret = ROK;
   cellCb    = NULLP;

   if(cellCfg->physCellId >= YS_NUM_PHY_CELLS)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cellCfg->physCellId,
            "Invalid phyCellId: ysMsCfgAddCellCfg failed\n");
      RETVALUE(RFAILED);
   }

   if(cellCfg->cellId >= YS_NUM_LOG_CELLS)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cellCfg->cellId,
            "Invalid cellId: ysMsCfgAddCellCfg failed\n");
      RETVALUE(RFAILED);
   }

   schTfuSap = NULLP;

   if((cellCb = ysMsCfgGetCellCfg(cellCfg->cellId)) != NULLP)
   {
      YS_DBG_ERR((_ysp, "Cell Configuration exist for cell (%d). \
                     ysMsCfgAddCellCfg Failed \n",
                     cellCfg->cellId));
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < ysCb.genCfg.maxTfuSaps; idx++)
   {
      sap = *(ysCb.tfuSapLst + idx);

      if (sap != NULLP)
      {
         if(sap->tfuSapCfg.cellId == cellCfg->cellId)
         {
            if((schTfuSap == NULLP) && (sap->tfuSapCfg.type == LYS_TFU_SCH_SAP))
            {
               schTfuSap = sap;
            }
         }
      }

      if (schTfuSap != NULLP)
         break;
   }

   if (schTfuSap == NULLP)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cellCfg->cellId,
            "Cell is not associated with schTfuSap: ysMsCfgAddCellCfg failed\n");
      RETVALUE(RFAILED);
   }

   if ((cellCb = (YsCellCb *)ysUtlMalloc(sizeof(YsCellCb))) == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysAlloc failed in ysMsCfgAddCellCfg\n"));
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&cellCb->cellCfg, (U8 *)cellCfg, sizeof(CtfCellCfgInfo));
   cellCb->cellId = cellCfg->cellId;

   /* 118146: Storing the cell Id in the globle cb */
   ysCb.cellId = cellCfg->cellId;
   cellCb->phyState = LYS_PHY_STATE_IDLE;
   cellCb->gotRACH = 0;

   /* HARQ: Minor optimization to clean up the code */
   cellCb->cmnChScrInit = 3*cellCb->cellCfg.cellIdGrpId + 
      cellCb->cellCfg.physCellId;

   offset = (U16)((PTR)&ueCb.ueHlEnt - (PTR)&ueCb);
   ret = cmHashListInit(&(cellCb->ueHLst),      /* hash list Cp */
            (U16)ysCb.genCfg.nmbUe,             /* HL bins */
            offset,                             /* Offset of HL Entry */
            FALSE,                              /* Allow dup. keys ? */
            CM_HASH_KEYTYPE_DEF,                /* HL key type */
            ysCb.ysInit.region,                 /* Mem region for HL */
            ysCb.ysInit.pool);                  /* Mem pool for HL */
   if(ret != ROK)
   {
      YS_DBG_ERR((_ysp, "cmHashListInit: ueHLst failed \n"));
      ysUtlDeAlloc((Data *)cellCb, sizeof(YsCellCb));
      RETVALUE(RFAILED);
   }

   if (ysMsUlmPrcCellCfg(cellCb) != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsUlmPrcCellCfg failed in ysMsCfgAddCellCfg\n"));
      cmHashListDeinit(&cellCb->ueHLst);
      ysUtlDeAlloc((Data *)cellCb, sizeof(YsCellCb));
      RETVALUE(RFAILED);
   }

   cellCb->schTfuSap = schTfuSap;

   /* HARQ: Initializing the time here */
   cellCb->timingInfo.sfn = 1023;
   cellCb->timingInfo.subframe = 9;
#ifdef RG_ULSCHED_AT_CRC
   cellCb->crcSent[9] = 1;
#endif

   schTfuSap->cellCb = cellCb;

	cellCb->cellId = cellCfg->cellId;
   if(cmHashListInsert(&(ysCb.cellCfgLst), (PTR)cellCb,
         (U8 *)&cellCb->cellId, sizeof(CmLteCellId)) != ROK)
   {
      uart_printf("\n ysMsCfgAddCellCfg cmHashListInsert returned failed \n");
      YS_DBG_ERR((_ysp, "cmHashListInsert returned failed \n"));
      cmHashListDeinit(&cellCb->ueHLst);
      ysUtlDeAlloc((Data *)cellCb, sizeof(YsCellCb));
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(cellCfg->cellId)) != NULLP)
   {
			  uart_printf ("\n in ysMsCfgAddCellCfg still able to get cellId #1 \n");
	}

   ret = ysMsCfgSm(cellCb, YS_MS_EVENT_CFG, NULLP);
   if(ret != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsCfgAddCellCfg: Phy Cfg failed \n"));
      cmHashListDeinit(&cellCb->ueHLst);
      ysUtlDeAlloc((Data *)cellCb, sizeof(YsCellCb));
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(cellCfg->cellId)) != NULLP)
   {
			  uart_printf ("\n in ysMsCfgAddCellCfg still able to get cellId #2 \n");
	}

   /*Initialize the information maintainted per SF for RRC
    * messages simulation */
#ifdef YS_UL_RRC_CONN_SIMUL
   cmMemset(cellCb->rrcMsgSimlCb.msgInfoSf, 0,
		   sizeof(cellCb->rrcMsgSimlCb.msgInfoSf));
#endif

#ifdef YS_ALL_PERF
	cellCb->ttiIndex 	  = 0;
	cellCb->capPerfData = TRUE;
#endif

   RETVALUE(ret);
}  /* ysMsCfgAddCellCfg*/

/*Start Fix for ccpu00123185 */
/**
 * @brief API for handle Tx Power Control request from eNB App
 *
 * @details
 *
 *     Function: ysMsCfgModTxPwrCfg
 *
 *     This API for handle Tx Power configuration request from eNB App
 *
 *  @param[in]
 *  @param[in]
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
PUBLIC S16 ysMsCfgModTxPwrCfg
(
CtfCellRecfgInfo  *cellRecfg
)
{
   S16       ret;
   YsCellCb  *cellCb;

   TRC2(ysMsCfgModTxPwrCfg)

   YS_DBG_PARAM((_ysp, "ysMsCfgModTxPwrCfg(): cellId = %d \n", 
            cellRecfg->cellId));

   ret = ROK;
   cellCb = NULLP;

   if(cellRecfg->cellId >= YS_NUM_LOG_CELLS)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cellRecfg->cellId,
            "Invalid cellId: ysMsCfgModCellCfg failed\n");
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(cellRecfg->cellId)) == NULLP)
   {
      YS_DBG_ERR((_ysp, "Cell Configuration doesn't exist for cell (%d). \
                      ysMsCfgModTxPwrCfg Failed \n", cellRecfg->cellId));
      RETVALUE(RFAILED);
   }

   cellCb->cellCfg.pdschCfg.refSigPwr  = cellRecfg->pdschCfg.refSigPwr;
   cellCb->cellCfg.pilotSigPwr         = cellRecfg->pilotSigPwr * YS_MS_DB_UNIT;
   cellCb->cellCfg.priSigPwr           = cellRecfg->priSigPwr   * YS_MS_DB_UNIT;
   cellCb->cellCfg.secSigPwr           = cellRecfg->secSigPwr   * YS_MS_DB_UNIT;

   RETVALUE(ret);
}  /* ysMsCfgModTxPwrCfg*/
   /*End of Fix for ccpu00123185 */

/**
 * @brief API for handle cell configuration request from RRM
 *
 * @details
 *
 *     Function: ysMsCfgModCellCfg
 *
 *     This API for handle modify cell configuration request from RRM
 *
 *  @param[in]
 *  @param[in]
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 ysMsCfgModCellCfg
(
CtfCellRecfgInfo  *cellRecfg
)
#else
PUBLIC S16 ysMsCfgModCellCfg(cellRecfg)
CtfCellRecfgInfo  *cellRecfg;
#endif
{
   S16       ret;
   YsCellCb  *cellCb;

   TRC2(ysMsCfgModCellCfg)

   YS_DBG_PARAM((_ysp, "ysMsCfgModCellCfg(): cellId = %d \n", 
            cellRecfg->cellId));

   ret = ROK;
   cellCb = NULLP;

   if(cellRecfg->cellId >= YS_NUM_LOG_CELLS)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cellRecfg->cellId,
            "Invalid cellId: ysMsCfgModCellCfg failed\n");
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(cellRecfg->cellId)) == NULLP)
   {
      YS_DBG_ERR((_ysp, "Cell Configuration doesn't exist for cell (%d). \
                      ysMsCfgModCellCfg Failed \n", cellRecfg->cellId));
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&cellCb->cellCfg.prachCfg, (U8 *)&cellRecfg->prachCfg,
                                          sizeof(CtfPrachCfgInfo));
   cmMemcpy((U8 *)&cellCb->cellCfg.pdschCfg, (U8 *)&cellRecfg->pdschCfg,
                                          sizeof(CtfPdschCfgInfo));
   cmMemcpy((U8 *)&cellCb->cellCfg.puschCfg, (U8 *)&cellRecfg->puschCfg,
                                          sizeof(CtfPuschCfgInfo));
   cmMemcpy((U8 *)&cellCb->cellCfg.phichCfg, (U8 *)&cellRecfg->phichCfg,
                                          sizeof(CtfPhichCfgInfo));
   cmMemcpy((U8 *)&cellCb->cellCfg.pucchCfg, (U8 *)&cellRecfg->pucchCfg,
                                          sizeof(CtfPucchCfgInfo));
   cmMemcpy((U8 *)&cellCb->cellCfg.srsUlCfg, (U8 *)&cellRecfg->srsUlCfg,
                                          sizeof(CtfSrsUlCfgInfo));

   if (ysMsUlmPrcCellRecfg(cellCb) != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsUlmPrcCellRecfg failed in ysMsCfgModCellCfg\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}  /* ysMsCfgModCellCfg*/

/**
 * @brief API for delete cell configuration
 *
 * @details
 *
 *     Function: ysMsCfgDelCellCfg
 *
 *     This API for handle delete cell configuration request from RRM
 *
 *  @param[in]
 *  @param[in]
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 ysMsCfgDelCellCfg
(
CmLteCellId    cellId
)
#else
PUBLIC S16 ysMsCfgDelCellCfg(cellId)
CmLteCellId    cellId;
#endif
{
   S16       ret;
   YsCellCb  *cellCb;
   YsUeCb    *pUeCb;
   YsUeCb    *nUeCb;

   TRC2(ysMsCfgDelCellCfg)

   YS_DBG_PARAM((_ysp, "ysMsCfgDelCellCfg(): cellId = %d \n", cellId));

   ret    = ROK;
   cellCb = NULLP;
   pUeCb   = NULLP;
   nUeCb   = NULLP;

   if((cellCb = ysMsCfgGetCellCfg(cellId)) == NULLP)
   {
      YS_DBG_ERR((_ysp, "Cell Configuration doesnt exist for cell (%d). \
                     ysMsCfgDelCellCfg Failed \n", cellId));
      RETVALUE(RFAILED);
   }

   ret = cmHashListGetNext(&cellCb->ueHLst, (PTR)pUeCb, (PTR *)&nUeCb);
   for (;;)
   {
      if (ret != ROK)
      {
         break;
      }
      else
      {
         pUeCb = nUeCb;
         ret = cmHashListGetNext(&cellCb->ueHLst, (PTR)pUeCb, (PTR *)&nUeCb);
         ysMsCfgDeAllocUeCb(cellCb, pUeCb);
      }
   }

   cmHashListDeinit(&cellCb->ueHLst);

   if (cellCb->schTfuSap != NULLP)
   {
      cellCb->schTfuSap->cellCb = NULLP;
   }

   if (ysMsUlmPrcDelCellCfg(cellCb) != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsUlmPrcDelCellcfg failed in ysMsCfgModCellCfg\n"));
      RETVALUE(RFAILED);
   }

   ret = ysMsCfgSm(cellCb, YS_MS_EVENT_STOP, NULLP);

   cmHashListDelete (&(ysCb.cellCfgLst), (PTR)cellCb);
   ysUtlDeAlloc((Data *) (cellCb), sizeof(YsCellCb));

   RETVALUE(ret);
}  /* ysMsCfgDelCellCfg*/

/**
 * @brief API for handle UE configuration request from RRM
 *
 * @details
 *
 *     Function: ysMsCfgAddUeCfg
 *
 *     This API for handle UE configuration request from RRM
 *
 *  @param[in]
 *  @param[in]
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 ysMsCfgAddUeCfg
(
CtfDedCfgInfo  *dedCfg
)
#else
PUBLIC S16 ysMsCfgAddUeCfg(dedCfg)
CtfDedCfgInfo  *dedCfg;
#endif
{
   S16       ret;
   YsCellCb  *cellCb;
   YsUeCb    *ueCb;

   TRC2(ysMsCfgAddUeCfg)

   YS_DBG_PARAM((_ysp, "ysMsCfgAddUeCfg(): ueId = %d \n", dedCfg->ueId));

   ret = ROK;
   cellCb = NULLP;

   if(dedCfg->cellId >= YS_NUM_LOG_CELLS)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)dedCfg->cellId,
            "Invalid cellId: ysMsCfgAddUeCfg failed\n");
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(dedCfg->cellId)) == NULLP)
   {
      YS_DBG_ERR((_ysp, "Cell Configuration doesn't exist for cell (%d). \
                     ysMsCfgAddUeCfg Failed \n", dedCfg->cellId));
      RETVALUE(RFAILED);
   }

   if ((ueCb = (YsUeCb *)ysUtlMalloc(sizeof(YsUeCb))) == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysUtlMalloc failed in ysMsCfgAddUeCfg\n"));
      MSPD_ERR("ysUtlMalloc failed in ysMsCfgAddUeCfg\n");
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&ueCb->ueCfg, (U8 *)dedCfg, sizeof(CtfDedCfgInfo));

   ueCb->ueId = dedCfg->ueId;
   ueCb->cellCb = cellCb;
#ifndef YS_MS_NO_TA
   ueCb->tarptInfo.lnk.node = NULL;
   ueCb->tarptInfo.ta = 100; /* initialised with invalid value */
#endif

   if(cmHashListInsert (&(cellCb->ueHLst), (PTR)ueCb,
         (U8 *)&ueCb->ueId, sizeof(CmLteRnti)) != ROK)
   {
      YS_DBG_ERR((_ysp, "cmHashListInsert returned failed \n"));
      MSPD_ERR("cmHashListInsert returned failed \n");
      ysUtlDeAlloc((Data *)ueCb, sizeof(YsUeCb));
      RETVALUE(RFAILED);
   }

#ifndef TFU_UPGRADE
   if(ysMsUlmPrcUeCfg(cellCb, ueCb) != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsUlmPrcUeCfg returned failed \n"));
      MSPD_ERR("ysMsUlmPrcUeCfg returned failed \n");
      /* Delete from UE list */
      cmHashListDelete (&(cellCb->ueHLst), (PTR)ueCb);
      ysUtlDeAlloc((Data *)ueCb, sizeof(YsUeCb));
      RETVALUE(RFAILED);
   }
#endif /* TFU_UPGRADE */

   RETVALUE(ret);
}  /* ysMsCfgAddUeCfg*/

/**
 * @brief API for handle UE configuration request from RRM
 *
 * @details
 *
 *     Function: ysMsCfgModUeCfg
 *
 *     This API for handle UE configuration request from RRM
 *
 *  @param[in]
 *  @param[in]
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 ysMsCfgModUeCfg
(
CtfDedRecfgInfo   *dedRecfg
)
#else
PUBLIC S16 ysMsCfgModUeCfg(dedRecfg)
CtfDedRecfgInfo   *dedRecfg;
#endif
{
   S16       ret;
   YsCellCb  *cellCb;
   YsUeCb    *ueCb;

   TRC2(ysMsCfgModUeCfg)

   YS_DBG_PARAM((_ysp, "ysMsCfgModUeCfg(): ueId = %d \n", dedRecfg->ueId));

   ret = ROK;
   cellCb = NULLP;
   ueCb = NULLP;

   if(dedRecfg->cellId >= YS_NUM_LOG_CELLS)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)dedRecfg->cellId,
            "Invalid cellId: ysMsCfgModUeCfg failed\n");
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(dedRecfg->cellId)) == NULLP)
   {
      YS_DBG_ERR((_ysp, "Cell Configuration doesn't exist for cell (%d). \
                     ysMsCfgModUeCfg Failed \n", dedRecfg->cellId));
      RETVALUE(RFAILED);
   }

   ueCb = ysMsCfgGetUe(cellCb, dedRecfg->ueId);
  /* MSPD ReEst Fix Start*/
   if(ueCb == NULLP)
   {
      MSPD_DBG("[%d]UeCB not Found\n",dedRecfg->ueId);
	  RETVALUE(RFAILED);
   }
   if (dedRecfg->ueId != dedRecfg->newUeId)
   {

        if(cmHashListDelete (&(cellCb->ueHLst), (PTR)ueCb) != ROK)
        {
           YS_DBG_ERR((_ysp, "UE Not Found in List\n", dedRecfg->cellId));
           MSPD_ERR("UE Not Found in List\n");
           RETVALUE(RFAILED) ;
        }
        ueCb->ueId= dedRecfg->newUeId;
        if(cmHashListInsert(&(cellCb->ueHLst), (PTR)ueCb,
             (U8 *)&ueCb->ueId, sizeof(CmLteRnti)) != ROK)
        {
           YS_DBG_ERR((_ysp, "UE Not Found in List\n", dedRecfg->cellId));
           MSPD_ERR("UE Not Found in List\n");
           RETVALUE(RFAILED) ;
        }
      MSPD_DBG("CL Reconfigured for newUeId %d old %d\n", dedRecfg->newUeId,
            dedRecfg->ueId );
        RETVALUE(ROK);
   }
   /* MSPD ReEst Fix End*/


   if (ueCb != NULLP)
   {
#ifndef TFU_UPGRADE
      ysMsUlmPrcDelUeCfg(cellCb, ueCb);

      cmMemcpy((U8 *)&ueCb->ueCfg.pdschCfg, (U8 *)&dedRecfg->pdschCfg,
                                             sizeof(CtfDedPdschCfgInfo));
      cmMemcpy((U8 *)&ueCb->ueCfg.pucchCfg, (U8 *)&dedRecfg->pucchCfg,
                                             sizeof(CtfDedPucchCfgInfo));
      cmMemcpy((U8 *)&ueCb->ueCfg.puschCfg, (U8 *)&dedRecfg->puschCfg,
                                             sizeof(CtfDedPuschCfgInfo));
      cmMemcpy((U8 *)&ueCb->ueCfg.srsUlCfg, (U8 *)&dedRecfg->srsUlCfg,
                                             sizeof(CtfDedSrsUlCfgInfo));
      cmMemcpy((U8 *)&ueCb->ueCfg.dedSRCfg, (U8 *)&dedRecfg->dedSRCfg,
                                             sizeof(CtfDedSRCfgInfo));

      if(ysMsUlmPrcUeCfg(cellCb, ueCb) != ROK)
      {
         YS_DBG_ERR((_ysp, "ysMsUlmPrcUeCfg returned failed \n"));
      }
#endif /* TFU_UPGRADE */
   }
   else
   {
      YS_DBG_ERR((_ysp, "UE (%d) doesnt exist in cell (%d). ysMsCfgModUeCfg \
               Failed \n",
                     dedRecfg->ueId, dedRecfg->cellId));
      MSPD_ERR("UE (%d) doesnt exist in cell (%d). "
               "Failed \n", dedRecfg->ueId, dedRecfg->cellId);
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}  /* ysMsCfgModUeCfg*/

/**
 * @brief API for handle UE configuration request from RRM
 *
 * @details
 *
 *     Function: ysMsCfgDelUeCfg
 *
 *     This API for handle UE configuration request from RRM
 *
 *  @param[in]
 *  @param[in]
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 ysMsCfgDelUeCfg
(
CtfDedReleaseInfo   *dedRel
)
#else
PUBLIC S16 ysMsCfgDelUeCfg(dedRel)
CtfDedReleaseInfo   *dedRel;
#endif
{
   S16       ret;
   YsCellCb  *cellCb;
   YsUeCb    *ueCb;

   TRC2(ysMsCfgDelUeCfg)

   YS_DBG_PARAM((_ysp, "ysMsCfgDelUeCfg(): ueId = %d \n", dedRel->ueId));

   ret = ROK;
   cellCb = NULLP;

   if(dedRel->cellId >= YS_NUM_LOG_CELLS)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)dedRel->cellId,
            "Invalid cellId: ysMsCfgDelUeCfg failed\n");
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(dedRel->cellId)) == NULLP)
   {
      YS_DBG_ERR((_ysp, "Cell Configuration doesn't exist for cell (%d). \
                     ysMsCfgDelUeCfg Failed \n", dedRel->cellId));
      RETVALUE(RFAILED);
   }

   ueCb = ysMsCfgGetUe(cellCb, dedRel->ueId);

   if (ueCb != NULLP)
   {
#ifndef YS_MS_NO_TA
      if (ueCb->tarptInfo.lnk.node)
      {
         cmLListDelFrm(&cellCb->taUeLst, &ueCb->tarptInfo.lnk);
      }
#endif
      ysMsCfgDeAllocUeCb(cellCb, ueCb);
   }
   else
   {
      YS_DBG_ERR((_ysp, "UE (%d) doesnt exist in cell (%d). ysMsCfgModUeCfg \
               Failed \n", dedRel->ueId, dedRel->cellId));
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}  /* ysMsCfgDelUeCfg*/


/*
*
*       Fun:   ysMsCfgGetCellCfg
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_utl.c
*
*/

#ifdef ANSI
PUBLIC YsCellCb *ysMsCfgGetCellCfg
(
CmLteCellId    cellId
)
#else
PUBLIC YsCellCb *ysMsCfgGetCellCfg(cellId)
CmLteCellId    cellId;
#endif
{
   YsCellCb *cellCb;

   TRC2(ysMsCfgGetCellCfg)

   cellCb = NULLP;

   if(cellId >= YS_NUM_LOG_CELLS)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cellId,
            "Invalid cellId:ysMsCfgGetCellCfg failed\n");
      RETVALUE(NULLP);
   }

   cmHashListFind(&ysCb.cellCfgLst, (U8 *)&cellId,
               sizeof(CmLteCellId), 0, (PTR *)&cellCb);

   RETVALUE(cellCb);
} /* end of ysMsCfgGetCellCfg */

/*
*
*       Fun:    ysMsCfgGetUe
*
*       Desc:   This function return UE control block
*
*       Ret:   Pointer to ue control block
*
*       Notes: None
*
*       File:   ys_utl.c
*/
#ifdef ANSI
PUBLIC YsUeCb *ysMsCfgGetUe
(
YsCellCb        *cellCb,
CmLteRnti       ueId
)
#else
PUBLIC YsUeCb *ysMsCfgGetUe(cellCb, ueId)
YsCellCb        *cellCb;
CmLteRnti       ueId;
#endif
{
   YsUeCb    *ueCb;

   TRC2(ysMsCfgGetUe)

   ueCb = NULLP;

   cmHashListFind(&cellCb->ueHLst, (U8 *)&ueId,
               sizeof(CmLteRnti), 0, (PTR *)&ueCb);

   RETVALUE (ueCb);
} /* end of ysMsCfgGetUe () */

/*
*
*       Fun:   ysMsCfgDeAllocUeCb
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsCfgDeAllocUeCb
(
YsCellCb        *cellCb,
YsUeCb          *ueCb
)
#else
PUBLIC S16 ysMsCfgDeAllocUeCb(cellCb, ueCb)
YsCellCb        *cellCb;
YsUeCb          *ueCb;
#endif
{

   TRC2(ysMsCfgDeAllocUeCb)

#ifndef TFU_UPGRADE
   ysMsUlmPrcDelUeCfg(cellCb, ueCb);
#endif /* TFU_UPGRADE */

   /* Delete from UE list */
   cmHashListDelete (&(cellCb->ueHLst), (PTR)ueCb);

   /* Free the transaction control block memory */
   ysUtlDeAlloc((Data *)ueCb, sizeof(YsUeCb));

   RETVALUE(ROK);
} /* end of ysMsCfgDeAllocUeCb */

/********************************************************************30**

         End of file:     ys_ms_cfg.c@@/main/1 - Sun Mar 28 21:19:19 2010

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

     ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      pk   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/
