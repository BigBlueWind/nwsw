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
  
        Name:    LTE-RLC Layer - System Services Interface Functions
    
        Type:    C file
  
        Desc:    C source code for the interface to System Services
                  of LTE-RLC
 
        File:    kw_ex_ms.c

        Sid:      gp_ex_ms.c@@/main/2 - Fri Nov 13 14:13:38 2009
  
        Prg:     gk
  
*********************************************************************21*/

/** @file gp_ex_ms.c
@brief RLC System Services Interface
*/


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#ifdef KW_PDCP
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#endif /* KW_PDCP */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#ifdef KW_PDCP
#include "kw_pj_lib.h"
#endif /* KW_PDCP */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#ifdef KW_PDCP
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#endif
#include "kw.x"
#ifdef KW_PDCP
#include "kw_pj_lib.x"      /* Acceptance defines */
#endif
#include "pst_events.h"
#include "log.h"

PUBLIC S16 kwInitExt ARGS (( Void ));

PRIVATE S16 rlcCfgReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
CkwCfgInfo *cfgInfo
);


PRIVATE S16 pdcpCfgReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
CpjCfgReqInfo *pdCfgReq
);

/**
 *
 * @brief
 *
 *  <b> Initialize External </b>
 *
 *  @b Description:
 *  Initializes variables used to interface with Upper/Lower Layer  
 *
 *  @return  S16
 *      -# ROK 
 *
*/
  
#ifdef ANSI
PUBLIC S16 kwInitExt 
(
)
#else
PUBLIC S16 kwInitExt()
#endif
{
   TRC2(kwInitExt);

   RETVALUE(ROK);
} /* kwInitExt */



/***********************************************************************
                      System Service Interface Functions
 ***********************************************************************/
/**
 *
 * @brief
 *
 *    <b> Activates Initialization </b>
 *
 *    @b Description:
 *    This function is invoked by system services to initialize the LTE-RLC
 *    layer. This is an entry point used by LTE_RLC layer to initialize its
 *    global variables, before becoming operational.
 *
 *    Allowable values for parameters are specified in ssi.h.
 *
 *    @param[in] ent    - Specify the entity id of the LTE-RLC task.
 *    @param[in] inst   - Specify the entity id of the LTE-RLC task.
 *    @param[in] region - Specifies the memory region from which
 *                         LTE-RLC should allocate structures and buffers.
 *    @param[in] reason - Specifies the reason for calling this
 *                         initialization function.
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 kwActvInit
(
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
)
#else
PUBLIC S16 kwActvInit(ent, inst, region, reason)
Ent    ent;                 /* entity */
Inst   inst;                /* instance */
Region region;              /* region */
Reason reason;              /* reason */
#endif
{
   TRC3(kwActvInit)

   /* Initialize kwCb */
   KW_MEM_SET(&kwCb, 0, sizeof(KwCb));

   /* Initialize task configuration parameters */
   kwCb.init.ent     = ent;           /* entity */
   kwCb.init.inst    = inst;          /* instance */
   kwCb.init.region  = region;        /* static region */
   kwCb.init.pool    = 0;             /* static pool */
   kwCb.init.reason  = reason;        /* reason */
   kwCb.init.cfgDone = FALSE;         /* configuration done */
   kwCb.init.acnt    = TRUE;          /* enable accounting */
   kwCb.init.usta    = TRUE;          /* enable unsolicited status */
   kwCb.init.trc     = FALSE;         /* enable trace */
   kwCb.init.procId  = SFndProcId();

   /* call external function for intialization */
   /*
   kwInitExt();
   */
   //MSPD_DBG("<Rahul>ENT %d INST %d\n", kwCb.init.ent, kwCb.init.inst);
   RETVALUE(ROK);
} /* kwActvInit */


/**
 *
 * @brief
 *
 *  <b> Activation Task </b>
 *
 *  @b Description:
 *  Processes events received for MLTE-RLC layer via System Services from
 *  other layers.
 *
 *  @param[in] pst   - Pst Structure
 *  @param[in] mBuf  - Message Buffer
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 kwActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 kwActvTsk(pst, mBuf)
Pst *pst;              /* pst structure */
Buffer *mBuf;           /* message buffer */
#endif
{
   S16 ret = ROK;

   logInfo("kwActvTsk received message from Ent=%d, dst=%d", pst->srcEnt, pst->dstEnt);

   switch(pst->srcEnt)
   {
     case ENT_PRM:
       {
         switch (pst->event)
         {
           case EVT_HELLO:
             {
               sendKwHelloAckToPrm(mBuf);
               SPutMsg(mBuf);
               break;
             }
           case EVT_EXIT:
             {
               logInfo ("%s", "EXIT received in RLC");
               procRcvdKwExitMsg();
               SPutMsg(mBuf);
               break;
             }
           case EVT_START_QUERY:
             {
               logInfo ("%s", "START QUERY received in RLC");
               sendKwStartIndToPrm();
               SPutMsg(mBuf);
               break;
             }
           default:
             {
               logInfo ("%s", "UNKNOWN message received from PRM");
               SPutMsg(mBuf);
               break;
             }
         }
         break;
       }
      case ENTSM:
         {
            switch(pst->event)
            {
#ifdef LCLKW
               case LKW_EVT_CFG_REQ:
                  {
                     ret = cmUnpkLkwCfgReq(KwMiLkwCfgReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_CNTRL_REQ:
                  {
                     ret = cmUnpkLkwCntrlReq(KwMiLkwCntrlReq, pst, mBuf);
                     break;
                  }
#if 0
               case LKW_EVT_STS_REQ:
                  {
                     ret = cmUnpkLkwStsReq(KwMiLkwStsReq, pst, mBuf);
                     break;
                  }
#endif
               case LKW_EVT_STA_REQ:
                  {
                     ret = cmUnpkLkwStaReq(KwMiLkwStaReq, pst, mBuf);
                     break;
                  }
#endif  /* LCLKW */
               default:
                  KW_FREE_BUF(mBuf);
#if  (ERRCLASS & ERRCLS_DEBUG)
                  KWLOGERROR(ERRCLS_DEBUG, EKW029, (ErrVal)pst->event,
                        "Invalid Event");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTNH:
         {
            switch(pst->event)
            {
#ifdef LCCKW
               case CKW_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkCkwBndReq(KwUiCkwBndReq, pst, mBuf );
                     break;
                  }

               case CKW_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkCkwUbndReq(KwUiCkwUbndReq, pst, mBuf );
                     break;
                  }

               case CKW_EVT_CFG_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkCkwCfgReq(rlcCfgReq, pst, mBuf);
                     break;
                  }
#if 0
               case CKW_EVT_UEIDCHG_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkCkwUeIdChgReq(KwUiCkwUeIdChgReq, pst, mBuf);
                     break;
                  }
#endif
#endif  /* LCCKW */

#ifdef LCKWU
               case KWU_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkKwuBndReq(KwUiKwuBndReq, pst, mBuf );
                     break;
                  }

               case KWU_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkKwuUbndReq(KwUiKwuUbndReq, pst, mBuf );
                     break;
                  }
#if 0
               case KWU_EVT_DAT_REQ:              /* Data request */
                  {
                     ret = cmUnpkKwuDatReq(KwUiKwuDatReq, pst, mBuf);
                     break;
                  }

#ifndef KW_PDCP
               case KWU_EVT_DISC_SDU_REQ:         /* Discard SDU request */
                  {
                     ret = cmUnpkKwuDiscSduReq(KwUiKwuDiscSduReq, pst, mBuf);
                     break;
                  }
#endif /* KW_PDCP */
#endif
#endif  /* LCKWU */
               default:
                  KW_FREE_BUF(mBuf);
#if  (ERRCLASS & ERRCLS_DEBUG)
                  KWLOGERROR(ERRCLS_DEBUG, EKW030, (ErrVal)pst->event,
                        "Invalid Event");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTPJ:
         {
            switch(pst->event)
            {
#ifdef LCKWU
               case KWU_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkKwuBndReq(KwUiKwuBndReq, pst, mBuf );
                     break;
                  }

               case KWU_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkKwuUbndReq(KwUiKwuUbndReq, pst, mBuf );
                     break;
                  }
#if 0
               case KWU_EVT_DAT_REQ:              /* Data request */
                  {
                     ret = cmUnpkKwuDatReq(KwUiKwuDatReq, pst, mBuf);
                     break;
                  }

               case KWU_EVT_DISC_SDU_REQ:         /* Discard SDU request */
                  {
                     ret = cmUnpkKwuDiscSduReq(KwUiKwuDiscSduReq, pst, mBuf);
                     break;
                  }
#endif
               default:
                  KW_FREE_BUF(mBuf);
#if  (ERRCLASS & ERRCLS_DEBUG)
                  KWLOGERROR(ERRCLS_DEBUG, EKW031, (ErrVal)pst->event,
                        "Invalid Event");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                  ret = RFAILED;
                  break;
#endif  /* LCKWU */
            }
            break;
         }

      case ENTRG:
         {
            switch(pst->event)
            {
#ifdef LCRGU
               case EVTRGUBNDCFM:     /* Bind request */
                  {
                     ret = cmUnpkRguBndCfm(KwLiRguBndCfm, pst, mBuf );
                     break;
                  }
#if 0
               case EVTRGUCDATIND:    /* Coomon Channel Data request */
                  {
                     ret = cmUnpkRguCDatInd(KwLiRguCDatInd, pst, mBuf);
                     break;
                  }

               case EVTRGUDDATIND:    /* Dedicated Channel Data request */
                  {
                     ret = cmUnpkRguDDatInd(KwLiRguDDatInd, pst, mBuf);
                     break;
                  }

               case EVTRGUCSTAIND:    /* Coomon Channel Status Response */
                  {
                     ret = cmUnpkRguCStaInd(KwLiRguCStaInd, pst, mBuf);
                     break;
                  }

               case EVTRGUDSTAIND:    /* Dedicated Channel Status Response */
                  {
                     ret = cmUnpkRguDStaInd(KwLiRguDStaInd, pst, mBuf);
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
               case EVTRGUHQSTAIND:    /* Harq status indication */
                  {
                     ret = cmUnpkRguHqStaInd(KwLiRguHqStaInd, pst, mBuf);
                     break;
                  }
#endif
#endif
#endif  /* LCRGU */

               default:
                  KW_FREE_BUF(mBuf);
#if  (ERRCLASS & ERRCLS_DEBUG)
                  KWLOGERROR(ERRCLS_DEBUG, EKW032, (ErrVal)pst->event,
                        "Invalid Event");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                  ret = RFAILED;
                  break;
            }
            break;
         }
#if 0
         /* kw006.201 optimization, fix for ccpu00111716 and ccpu00117290 */
      case ENTKW:
         {
            switch(pst->event)
            {
#ifdef KW_BG_DL_PROC
               case EVTKWSELFMSGDL:
                  {
                     kwAmmProcessSelfMsgDl();
                     break;
                  }
#endif /* KW_BG_DL_PROC */
#ifdef KW_BG_UL_PROC
               case EVTKWSELFMSGUL:
                  {
                     kwUtlProcessSelfMsgUl();
                     break;
                  }
#endif /* KW_BG_UL_PROC */
            }
            break;
         }        
#endif
      default:
         {
#if  (ERRCLASS & ERRCLS_DEBUG)
            KWLOGERROR(ERRCLS_DEBUG, EKW033, (ErrVal)pst->srcEnt,
                  "Invalid Source Entity ");
#endif /* ERRCLASS & ERRCLS_DEBUG */
            KW_FREE_BUF(mBuf);
            ret = RFAILED;
            break;
         }
    }
   SExitTsk();
   logInfo("kwActvTsk src=%d, dst=%d event=%d result=%d", pst->srcEnt, pst->dstEnt, pst->event, ret);

   RETVALUE(ret);
} /* kwActvTsk */

#ifdef KW_PDCP

/***********************************************************************
                      System Service Interface Functions
 ***********************************************************************/
/**
 *
 * @brief
 *
 *    <b> Activates Initialization </b>
 *
 *    @b Description:
 *    This function is invoked by system services to initialize the LTE-PDCP
 *    layer. This is an entry point used by LTE_PDCP layer to initialize its
 *    global variables, before becoming operational.
 *
 *    Allowable values for parameters are specified in ssi.h.
 *
 *    @param[in] ent    - Specify the entity id of the LTE-RLC task.
 *    @param[in] inst   - Specify the entity id of the LTE-RLC task.
 *    @param[in] region - Specifies the memory region from which
 *                         LTE-PDCP should allocate structures and buffers.
 *    @param[in] reason - Specifies the reason for calling this
 *                         initialization function.
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjActvInit
(
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
)
#else
PUBLIC S16 pjActvInit(ent, inst, region, reason)
Ent    ent;                 /* entity */
Inst   inst;                /* instance */
Region region;              /* region */
Reason reason;              /* reason */
#endif
{
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U16 i;
#endif /* LTE_L2_MEAS */
   TRC3(pjActvInit)


   /* Initialize task configuration parameters */
   /* We will take only entity name */
   kwCb.pjInit.ent     = ent;           /* entity */
   kwCb.pjInit.inst    = inst;          /* instance */
   kwCb.pjInit.region  = region;        /* static region */
   kwCb.pjInit.pool    = 0;             /* static pool */
   kwCb.pjInit.reason  = reason;        /* reason */
   kwCb.pjInit.cfgDone = FALSE;         /* configuration done */
   kwCb.pjInit.acnt    = TRUE;          /* enable accounting */
   kwCb.pjInit.usta    = TRUE;          /* enable unsolicited status */
   kwCb.pjInit.trc     = FALSE;         /* enable trace */
   kwCb.pjInit.procId  = SFndProcId();

#ifdef LTE_L2_MEAS
   kwCb.pjL2Cb.pjNumMeas   =  0;
   for(i = 0; i < PJ_MAX_L2MEAS_EVT; i++)
   {
      kwCb.pjL2Cb.pjMeasEvtCb[i] = NULLP;
   }
#endif /* LTE_L2_MEAS */

   //MSPD_DBG("<Rahul>ENT %d INST %d\n", kwCb.pjInit.ent, kwCb.pjInit.inst);
   RETVALUE(ROK);
} /* pjActvInit */


/**
 *
 * @brief
 *
 *  <b> Activation Task </b>
 *
 *  @b Description:
 *  Processes events received for LTE-PDCP layer via System Services from
 *  other layers.
 *
 *  @param[in] pst   - Pst Structure
 *  @param[in] mBuf  - Message Buffer
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 pjActvTsk(pst, mBuf)
Pst *pst;              /* pst structure */
Buffer *mBuf;           /* message buffer */
#endif
{
   S16 ret = ROK;

   logInfo("pjActvTsk received message from Ent=%d", pst->srcEnt);

   switch(pst->srcEnt)
   {
     case ENT_PRM:
       {
         switch (pst->event)
         {
           case EVT_HELLO:
             {
               sendPjHelloAckToPrm(mBuf);
               SPutMsg(mBuf);
               break;
             }
           case EVT_EXIT:
             {
               procRcvdPjExitMsg();
               SPutMsg(mBuf);
               break;
             }
           case EVT_START_QUERY:
             {
               logInfo ("%s", "START QUERY received in PDCP");
               sendPjStartIndToPrm();
               SPutMsg(mBuf);
               break;
             }
           default:
             {
               SPutMsg(mBuf);
               break;
             }
         }
         break;
       }
      case ENTSM:
         {
            switch(pst->event)
            {
#ifdef LCLPJ
               case LPJ_EVT_CFG_REQ:
                  {
                     ret = cmUnpkLpjCfgReq(PjMiLpjCfgReq, pst, mBuf);
                     break;
                  }

               case LPJ_EVT_CNTRL_REQ:
                  {
                     ret = cmUnpkLpjCntrlReq(PjMiLpjCntrlReq, pst, mBuf);
                     break;
                  }
#if 0
               case LPJ_EVT_STS_REQ:
                  {
                     ret = cmUnpkLpjStsReq(PjMiLpjStsReq, pst, mBuf);
                     break;
                  }
#endif
               case LPJ_EVT_STA_REQ:
                  {
                     ret = cmUnpkLpjStaReq(PjMiLpjStaReq, pst, mBuf);
                     break;
                  }
#if 0
                  /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
               case LPJ_EVT_L2MEAS_REQ:
                  {
                     ret = cmUnpkLpjL2MeasReq(PjMiLpjL2MeasReq, pst, mBuf);
                     break;
                  }
#endif
#endif
               default:
                  {
#if  (ERRCLASS & ERRCLS_DEBUG)
                     KWLOGERROR(ERRCLS_INT_PAR, EKW034, (ErrVal)pst->event,
                           "Invalid Event from SM");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                     ret = RFAILED;
                     break;
                  }
#endif  /* LCLPJ */
            }
            break;
         }

      case ENTNH:
         {
            switch(pst->event)
            {
#ifdef LCCPJ
            case EVTCPJBNDREQ:
                  {
                     cmUnpkCpjBndReq(PjUiCpjBndReq, pst, mBuf);
                     break;
                  }

            case EVTCPJUBNDREQ:
                  {
                     cmUnpkCpjUbndReq(PjUiCpjUbndReq, pst, mBuf);
                     break;
                  }

            case EVTCPJCFGREQ:
                  {
                     cmUnpkCpjCfgReq(pdcpCfgReq, pst, mBuf);
                     break;
                  }
#if 0
            case EVTCPJREESTREQ:
                  {
                     cmUnpkCpjReEstReq(PjUiCpjReEstReq, pst, mBuf);
                     break;
                  }

            case EVTCPJSECCFGREQ:
                  {
                     cmUnpkCpjSecCfgReq(PjUiCpjSecCfgReq, pst, mBuf);
                     break;
                  }

            case EVTCPJUEIDCHGREQ:
                  {
                     cmUnpkCpjUeIdChgReq(PjUiCpjUeIdChgReq, pst, mBuf);
                     break;
                  }

            case EVTCPJCOUNTREQ:
                  {
                     cmUnpkCpjCountReq(PjUiCpjCountReq, pst, mBuf);
                     break;
                  }

            case EVTCPJSDUSTAREQ:
                  {
                     cmUnpkCpjSduStaReq(PjUiCpjSduStaReq, pst, mBuf);
                     break;
                  }
#endif
#endif /* LCCPJ */
#ifdef LCPJU
               case EVTPJUBNDREQ:
                  {
                     cmUnpkPjuBndReq(PjUiPjuBndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUUBNDREQ:
                  {
                     cmUnpkPjuUbndReq(PjUiPjuUbndReq, pst, mBuf);
                     break;
                  }
#if 0
               case EVTPJUDATREQ:
                  {
                     cmUnpkPjuDatReq(PjUiPjuDatReq, pst, mBuf);
                     break;
                  }

               case EVTPJUDATFWDREQ:
                  {
                     cmUnpkPjuDatFwdReq(PjUiPjuDatFwdReq, pst, mBuf);
                     break;
                  }
#ifdef CTF_AS_SEC_ENB
	       case EVTCTFDATREQ:
		  {
               	     cmUnpkCtfDatReq (YsUiCtfDatReq, pst, mBuf);
                     break;
		  }
#endif
#endif /* removed for l2sim */     
#endif /* LCPJU */
               default:
                  KW_FREE_BUF(mBuf);
#if  (ERRCLASS & ERRCLS_DEBUG)
                  KWLOGERROR(ERRCLS_DEBUG, EKW035, (ErrVal)pst->event,
                        "Invalid Event");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTPX:
         {
            switch(pst->event)
            {
#ifdef LCPJU
               case EVTPJUBNDREQ:
                  {
                     cmUnpkPjuBndReq(PjUiPjuBndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUUBNDREQ:
                  {
                     cmUnpkPjuUbndReq(PjUiPjuUbndReq, pst, mBuf);
                     break;
                  }
#if 0
               case EVTPJUDATREQ:
                  {
                     cmUnpkPjuDatReq(PjUiPjuDatReq, pst, mBuf);
                     break;
                  }

               case EVTPJUDATFWDREQ:
                  {
                     cmUnpkPjuDatFwdReq(PjUiPjuDatFwdReq, pst, mBuf);
                     break;
                  }
#endif
#endif
               default:
                  KW_FREE_BUF(mBuf);
#if  (ERRCLASS & ERRCLS_DEBUG)
                  KWLOGERROR(ERRCLS_DEBUG, EKW036, (ErrVal)pst->event,
                        "Invalid Event");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                  ret = RFAILED;
                  break;
            }
            break;
         }
#if 0
      case ENTOC:
         {
            switch(pst->event)
            {
#ifdef PJ_CMP_ASYNC
               /* Compression Confirmations */
               case OBD_EVT_CMP_INIT_CFM:
                  ret = cmUnpkLibObdCmpInitCfm(PjLibObdCmpInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_CMP_CFM:
                  ret = cmUnpkLibObdCmpCfm(PjLibObdCmpCfm, pst, mBuf);
                  break;

               case OBD_EVT_DECMP_CFM:
                  ret = cmUnpkLibObdDecmpCfm(PjLibObdDecmpCfm, pst, mBuf);
                  break;
#endif /* PJ_CMP_ASYNC */
               default:
                  KW_FREE_BUF(mBuf);
#if  (ERRCLASS & ERRCLS_DEBUG)
                  KWLOGERROR(ERRCLS_DEBUG, EKW037, (ErrVal)pst->event,
                        "Invalid Event");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                  ret = RFAILED;
                  break;
            }
            break;
         }
#endif
      case ENTOS:
         {
            switch(pst->event)
            {
#ifdef PJ_SEC_ASYNC
                  /* Integrity Confirmations */
               case OBD_EVT_INT_INIT_CFM:
                  ret = cmUnpkLibObdIntInitCfm(PjLibObdIntInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_INT_PROT_CFM:
                  ret = cmUnpkLibObdIntProtCfm(PjLibObdIntProtCfm, pst, mBuf);
                  break;

               case OBD_EVT_INT_VER_CFM:
                  ret = cmUnpkLibObdIntVerCfm(PjLibObdIntVerCfm, pst, mBuf);
                  break;

                  /* Ciphering Confirmations */
               case OBD_EVT_CP_INIT_CFM:
                  ret = cmUnpkLibObdCpInitCfm(PjLibObdCpInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_UP_INIT_CFM:
                  ret = cmUnpkLibObdUpInitCfm(PjLibObdUpInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_CIPHER_CFM:
                  ret = cmUnpkLibObdCipherCfm(PjLibObdCipherCfm, pst, mBuf);
                  break;

               case OBD_EVT_DECIPHER_CFM:
                  ret = cmUnpkLibObdDecipherCfm(PjLibObdDecipherCfm, pst, mBuf);
                  break;
#endif /* PJ_SEC_ASYNC */
               default:
                  KW_FREE_BUF(mBuf);
#if  (ERRCLASS & ERRCLS_DEBUG)
                  KWLOGERROR(ERRCLS_DEBUG, EKW038, (ErrVal)pst->event,
                        "Invalid Event");
#endif /* ERRCLASS & ERRCLS_DEBUG */
                  ret = RFAILED;
                  break;
            }
            break;
         }

      default:
         {
#if  (ERRCLASS & ERRCLS_DEBUG)
            KWLOGERROR(ERRCLS_DEBUG, EKW039, (ErrVal)pst->srcEnt,
                  "Invalid Source Entity ");
#endif /* ERRCLASS & ERRCLS_DEBUG */
            KW_FREE_BUF(mBuf);
            ret = RFAILED;
            break;
         }
   }

   SExitTsk();
   logInfo("pjActvTsk src=%d, dst=%d event=%d result=%d", pst->srcEnt, pst->dstEnt, pst->event, ret);
   RETVALUE(ret);
} /* pjActvTsk */

#endif /* KW_PDCP */

#ifdef ANSI
PRIVATE S16 rlcCfgReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
CkwCfgInfo *cfgInfo
)
#else /* ANSI */
PRIVATE S16 rlcCfgReq(post, spId, cfgInfo)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
CkwCfgInfo *cfgInfo;
#endif /* ANSI */
{
  
  CkwCfgCfmInfo        *cfgCfm;       /* Configuraiton Confirm */
  KwCkwSapCb     *ckwSap;
  int i=0;
  KW_ALLOC(cfgCfm, sizeof(CkwCfgCfmInfo));

  /* Get the ckwSap */
  ckwSap = &(kwCb.ckwSap);
  
   cfgCfm->transId = cfgInfo->transId;
   cfgCfm->ueId = cfgInfo->ueId;
   cfgCfm->cellId = cfgInfo->cellId;
   cfgCfm->numEnt = cfgInfo->numEnt;
  for( i = 0; i < cfgInfo->numEnt; i++)
  {
    cfgCfm->entCfgCfm[i].rbId = cfgInfo->entCfg[i].rbId;
    cfgCfm->entCfgCfm[i].rbType = cfgInfo->entCfg[i].rbType;
    cfgCfm->entCfgCfm[i].status.status = CKW_CFG_CFM_OK;
  }
  
  KwUiCkwCfgCfm(&(ckwSap->pst), ckwSap->suId, cfgCfm);

}


#ifdef ANSI
PRIVATE S16 pdcpCfgReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
CpjCfgReqInfo *pdCfgReq
)
#else /* ANSI */
EXTERN S16 pdcpCfgReq (post, spId, pdCfgReq)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
CpjCfgReqInfo *pdCfgReq;
#endif /* ANSI */
{
   KwCpjSapCb           *cpjSap;       /* CPJ SAP Control Block */
   CpjCfgCfmInfo        *cfgCfm;       /* Configuraiton Confirm */
   int i=0;
  KW_ALLOC(cfgCfm, sizeof(CkwCfgCfmInfo));

  /* Get the ckwSap */
  cpjSap = &(kwCb.cpjSap);
  
   cfgCfm->transId = pdCfgReq->transId;
   cfgCfm->ueId = pdCfgReq->ueId;
   cfgCfm->cellId = pdCfgReq->cellId;
   cfgCfm->numEnt = pdCfgReq->numEnt;
  for( i = 0; i < pdCfgReq->numEnt; i++)
  {
    cfgCfm->cfmEnt[i].rbId = pdCfgReq->cfgEnt[i].rbId;
    cfgCfm->cfmEnt[i].rbType = pdCfgReq->cfgEnt[i].rbType;
    cfgCfm->cfmEnt[i].status = CKW_CFG_CFM_OK;
  }

  PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
}
  
/********************************************************************30**
         End of file:     gp_ex_ms.c@@/main/2 - Fri Nov 13 14:13:38 2009
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
------------ -------- ---- ----------------------------------------------
/main/1      ---      gk   1. Initial release.
/main/2      ---      gk   1. LTERLC Release 2.1
/main/3     kw005.201 ap   1. Added support for L2 Measurement.
/main/4     kw006.201 ap   1. Added event KWEVTSELFMSG for ccpu00111716 in
                              function kwActvTsk.
                      rd   2. Added event KWEVTSELFMSGUL for ccpu00117290 in
                              function kwActvTsk.
                           3. Removed the freeing of mBuf after the posting
                              of self message to RLC in kwActvTsk in case
                              ENTKW.
*********************************************************************91*/
