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


/**********************************************************************

     Name:     LTE-RLC Layer

     Type:     C file

     Desc:     Source code for RLC Unacknowledged mode assembly and
               reassembly.This file contains following functions

                  --kwUmmQSdu
                  --kwUmmProcessSdus
                  --kwUmmProcessPdus
                  --kwUmmReAssembleSdus
                  --kwUmmReEstablish

     File:     gp_umm.c

     Sid:      gp_umm.c@@/main/2 - Fri Nov 13 14:14:07 2009

     Prg:      vp

**********************************************************************/
/** @file gp_umm.c
@brief RLC Unacknowledged Mode module
*/

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000372     LTE-MAC
 *      1000369     LTE-RRC
 */

/* header (.h) include files */
#include "envopt.h"             /* environment options */
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm5.h"                /* Timer Functions */
#include "cm_lte.h"             /* common umts header file */
#include "cm_hash.h"            /* common hash module  file */
#include "cm_llist.h"           /* common list header file */
#include "ckw.h"                /* RRC layer */
#include "lkw.h"                /* RRC layer */
#include "kwu.h"                /* RLC service user */
#include "lkw.h"                /* LM Interface */
#include "rgu.h"                /* MAC layer */
#include "kw_env.h"             /* RLC environment options */
#ifdef KW_PDCP
#include "lpj.h"                /* LPJ defines */
#include "cpj.h"                /* CPJ defines */
#include "pju.h"                /* PJU defines */
#endif

#include "kw.h"                 /* RLC layer */
#include "kw_err.h"


/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_lib.x"             /* common library */
#include "cm5.x"                /* Timer Functions */
#include "cm_hash.x"            /* common hash module */
#include "cm_lte.x"             /* common umts file */
#include "cm_llist.x"           /* common list header file */
#include "ckw.x"                /* RRC layer */
#include "kwu.x"                /* RLC service user */
#include "lkw.x"                /* LM Interface */
#include "rgu.x"                /* MAC later */
#ifdef KW_PDCP
#include "lpj.x"                /* LPJ defines */
#include "cpj.x"                /* CPJ defines */
#include "pju.x"                /* PJU defines */
#endif

#include "kw.x"                 /* RLC layer */
#ifdef MSPD
#if 1
#include "LtePhyL2Api.h"
#include "apidefs.h"
#include "supervisor.h"
#include "resultcodes.h"
#include "hal.h"
#include "basetypes.h"
#endif
#endif

#define KW_UM_MIN_HDR_SZ  4 /* KW_MAC_HDR_SZ2 + KW_PDU_FIXED_HDRSZ*/
PRIVATE S16 kwUmmEstHdrSz ARGS ((KwRbCb *rbCb,U8 type,U16 sduCount));
PRIVATE S16 kwUmmCreatePdu ARGS ((KwRbCb *rbCb,Buffer **pdu,U8 fi));
PRIVATE S16 kwUmmExtractHdr ARGS ((KwRbCb *rbCb,Buffer *pdu,KwUmHdr **umHdr));
PRIVATE S16 kwUmmReAssembleSdus ARGS ((KwRbCb *rbCb, KwUmRecBuf *umRecBuf, U16 *numSdus, Bool *isPduProcessd));
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
PRIVATE Void kwUmmCalDlIpThrPut ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo, MsgLen pduSz));
/* kw006.201 ccpu00120058 fixed non ansi compilation problem, also added new functions to reduce code complexity */
PRIVATE Void kwUmmMeasDlIpBrstCont ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo));
PRIVATE Void kwUmmMeasDlIpBrstStart ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo));
PRIVATE Void kwUmmMeasDlIpBrstReset ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo));
PRIVATE Void kwUmmMeasDlIpBrstComplt ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo));
PRIVATE Void kwUmmCalUlIpThrPut ARGS ((KwRbCb *rbCb, Buffer *pdu, KwL2MeasUlThrpTimInfo *ulTimeInfo, Bool insideLoop));
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */
/* kw006.201 ccpu00118629, Replacing large macros with functions */
PRIVATE S16 kwUmmCheckDupPdu ARGS ((KwRbCb *rbCb, KwSn curSn));
PRIVATE Void kwUmmFindNextVrUr ARGS ((KwUmUl *umUl, KwSn prevVrUr, KwSn *vrUr));
PRIVATE Void kwUmmUlRlsRecBuf ARGS ((KwUmRecBuf *recBuf));
PRIVATE Void kwUmmReasmbleOnReEst ARGS ((KwRbCb *rbCb));
PRIVATE Void kwUmmResetStateVars ARGS ((KwRbCb *rbCb));
//PRIVATE Void kwUmmStopRbTmrs ARGS ((KwRbCb *rbCb));
PRIVATE Void kwUmmDlAddSdu ARGS ((KwRbCb *rbCb, KwSdu *sdu));
PRIVATE Void kwUmmDlGetSdu ARGS ((KwRbCb *rbCb, CmLList **firstNode));
PRIVATE Void kwUmmDlRemSdu ARGS ((KwRbCb *rbCb, KwSdu *sdu));
PRIVATE Void kwUmmUpdateBo ARGS ((KwRbCb *rbCb, MsgLen sduSz));
PRIVATE Void kwUmmUlProcessDeletionRb ARGS ((KwRbCb *rbCb));
PRIVATE Void kwUmmRbFree ARGS ((KwRbCb *rbCb));
PRIVATE Void kwUmmAddRbToDeleteLst ARGS((KwRbCb *rbCb));

#ifdef MSPD
extern Txt  tmpKwDbg[];
extern U32  kwUmmReassStart;
extern U32  kwUmmReassEnd;
EXTERN U32  ysMsUmmReEstReqRcvd;
EXTERN U32  ysMsUmmErrReEstReqRcvd;
EXTERN U32  ysMsUmBo;
extern U32  tempUlRlsPduCnt;
#endif


/** @addtogroup ummode */
/*@{*/


#ifdef ANSI
PRIVATE Void kwUmmDlAddSdu
(
KwRbCb                  *rbCb,
KwSdu                   *sdu
)
#else
PRIVATE Void kwUmmDlAddSdu(rbCb, sdu)
KwRbCb                  *rbCb;
KwSdu                   *sdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.sduLock);
   {
      cmLListAdd2Tail(&UMDL.sduQ, &sdu->lstEnt);
      sdu->lstEnt.node = (PTR)sdu;
/* TODO: This need to be reviewed when H8+ code is merged */
#ifdef PWAV
      if (UMDL.bo < 0)
      {
        UMDL.bo = 0;
      }
#endif
      /* Update BO and estimate header size for the current BO */ 
      UMDL.bo += sdu->sduSz;
      kwUmmEstHdrSz(rbCb, KW_ADD_SDU, 1);
   }
   KW_RDWR_UNLOCK(&kwCb.sduLock);
}

#ifdef ANSI
PRIVATE Void kwUmmDlGetSdu
(
KwRbCb                   *rbCb, 
CmLList                  **firstNode
)
#else
PRIVATE Void kwUmmDlGetSdu(rbCb, firstNode)
KwRbCb                   *rbCb;
CmLList                  **firstNode;
#endif
{
   KW_RDWR_LOCK(&kwCb.sduLock);
   {
      (*firstNode) = cmLListFirst(&UMDL.sduQ);
   }
   KW_RDWR_UNLOCK(&kwCb.sduLock);
}

#ifdef ANSI
PRIVATE Void kwUmmDlRemSdu
(
KwRbCb                  *rbCb,
KwSdu                   *sdu
)
#else
PRIVATE Void kwUmmDlRemSdu(rbCb, sdu)
KwRbCb                  *rbCb;
KwSdu                   *sdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.sduLock);
   {
      KW_FREE_BUF(sdu->mBuf);
      cmLListDelFrm(&UMDL.sduQ, &sdu->lstEnt);
      KW_FREE(sdu, sizeof(KwSdu));
   }
   KW_RDWR_UNLOCK(&kwCb.sduLock);
}

#ifdef ANSI
PRIVATE Void kwUmmUpdateBo
(
KwRbCb                  *rbCb,
MsgLen                  sduSz
)
#else
PRIVATE Void kwUmmUpdateBo(rbCb, sduSz)
KwRbCb                  *rbCb;
MsgLen                  sduSz;
#endif
{
   KW_RDWR_LOCK(&kwCb.sduLock);
   {
      UMDL.bo -= sduSz;
   }
   KW_RDWR_UNLOCK(&kwCb.sduLock);
}
/**
 *
 * @brief
 *
 *        Handler to queue the SDU in the SDU queue and update BO and report it to the lower layer.
 *
 * @b Description:
 *
 *        This function is used to queue the received SDU in the
 *        SDU queue maintained in the radio bearer control block.
 *        After queuing the SDU, BO is updated and is reported
 *        to the lower layer.
 *
 *  @param[in] rbCb RB control block
 *  @param[in] sdu  SDU to be queued
 *  @return  S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 kwUmmQSdu
(
KwRbCb *rbCb,                   /* Rb Control Block */
U32    sduId,                   /* Data Request Information */
Buffer *mBuf                    /* SDU buffer */
)
#else
PUBLIC S16 kwUmmQSdu(rbCb,sduId,mBuf)
KwRbCb        *rbCb;            /* Rb Control Block */
U32           sduId;            /* Data Request Information */
Buffer        *mBuf;            /* SDU buffer */
#endif
{
   MsgLen len;                  /* SDU buffer length */
   KwSdu  *sdu;                 /* SDU */
   KwUmDl *umDl;                /* UM Downlink Cb */

   TRC2(kwUmmQSdu)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL), (_kwPBuf,
           "kwUmmQSdu(rbCb, sduId, mBuf)\n"));
#ifdef MSPD
   if (rbCb->m.um.umDl.sduQ.count >= KW_UM_DL_MAX_SDUS)
   {
      extern U32 dldrops_kwu_um;
      extern U32 dlpkt_um;
      extern U32 buffer_occ;
      KW_FREE_BUF(mBuf);
      KWLOGERROR(ERRCLS_ADD_RES, EKW214, (ErrVal) RFAILED,
            "Too many SDUs, dropping SDU");
      ++dldrops_kwu_um;
      kwUtlSndDStaRsp(rbCb,rbCb->m.um.umDl.bo,rbCb->m.um.umDl.estHdrSz,FALSE);
      dlpkt_um = rbCb->m.um.umDl.sduQ.count;
      buffer_occ = rbCb->m.um.umDl.bo;
      RETVALUE(ROK);
   }
#endif
   KW_ALLOC(sdu,(Size)sizeof(KwSdu));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( sdu == NULLP )
   {
       KWLOGERROR(ERRCLS_ADD_RES, EKW214, (ErrVal) RFAILED,
            "KwSdu: sdu allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   SFndLenMsg(mBuf,&len);
   umDl = &(rbCb->m.um.umDl);

   sdu->mBuf = mBuf;
   sdu->sduSz = len;
   sdu->mode.um.sduId = sduId;
   sdu->mode.um.isSegmented = FALSE;

#ifdef MSPD
   {
         extern U32 dlrate_kwu;
         dlrate_kwu += len;
   }
#endif

   /* Add sdu to the sdu list                                                */
   kwUmmDlAddSdu(rbCb, sdu);
#ifdef MSPD
   ++kwCb.numDlUmSdus;
#endif
   if (rbCb->reestInProgress == FALSE)
   {
      kwUtlSndDStaRsp(rbCb,umDl->bo,umDl->estHdrSz,FALSE);
   }
   else
   {
     MSPD_DBG("<CRFIX>: reestInProgress Set to True\n");	   
   }
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   /* Update numActUe if it is not active */
   if((rbCb->rbL2Cb.measOn & LKW_L2MEAS_ACT_UE) &&
      (rbCb->ueCb->numActRb[rbCb->qci]++ == 0))
   {
     kwCb.kwL2Cb.numActUe[rbCb->qci]++;
   }
#endif
   RETVALUE(ROK);

}

#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
/* kw006.201 CR:ccpu00120058, added new function */
/**
 *
 * @brief Private handler to measure DL IP throughput.
 *
 * @b Description
 *      This function is called from kwUmmCalDlIpThrPut
 *      If the RLC bo is more than allocation from MAC,
 *        T2 time stamp is taken and stored in KwRbCb.
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to TRUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *      If the allocation from MAC is emptying the RLC buffer,
 *        Burst is said to be COMPLETED.
 *        T1 time stamp is calculated in kwUtlProcHarqInd
 *
 * @param[in]  rbCb       RB control block
 * @param[in]  dataVol    Data volume
 * @param[in]  initialBo  BO of RLC entity before constructing the PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwUmmMeasDlIpBrstCont
(
KwRbCb      *rbCb,
MsgLen      dataVol,
S32         initialBo
)
#else
PRIVATE Void kwUmmMeasDlIpBrstCont(rbCb, dataVol, initialBo)
KwRbCb      *rbCb;
MsgLen      dataVol;
S32         initialBo;
#endif
{
   KwTbSnMap         *curTbSnMap      = NULLP; /*Holds the current TbSnMap*/
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;

   TRC2(kwUmmMeasDlIpBrstCont)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL), (_kwPBuf,
           "kwUmmMeasDlIpBrstCont(rbCb, dataVol, initialBo)\n"));

   /*Get the current TbSnMap*/
   curTbSnMap = kwCb.kwL2Cb.curTbSnMap;

   l2MeasIpThruput = &rbCb->l2MeasIpThruput;
   if(initialBo > dataVol)
   {
      /*It means data burst is splitted across TTI. So set the*
       *isBurstSplitted flag to TRUE.                         */
      curTbSnMap->snSduMap[curTbSnMap->numSn -1].isBurstSplitted = TRUE;

      /*This count will be decremented when HARQ Ack will be received*
        for this TbId. It is done in kwUtlProcHarqInd.               */
      l2MeasIpThruput->count++;

      l2MeasIpThruput->dataVol += dataVol;
   }
   else
   {
      /*Change state to BURST_COMPLETED since current allocation has*
       *emptied the data buffer.                                    */
      l2MeasIpThruput->dlIpThrputSt = KW_DL_IPTHRU_BURST_COMPLETED;

      /*If count is 0, then it means that RLC has already received  *
       *all the ACKs for burst transmitted. So, state can be changed*
       * to RESET.                                                  */
      if(0 == l2MeasIpThruput->count)
      {
         /*Update Volume and time difference in kwL2MeasRbCb. Use*
          *tempTimeT1 which was stamped when all the part of     *
          *the burst(till that time) were ACK-ed.                        */
         rbCb->rbL2Cb.l2Sts[KW_L2MEAS_DL_IP]->dlIpThruput.volSummation
         += l2MeasIpThruput->dataVol;
         rbCb->rbL2Cb.l2Sts[KW_L2MEAS_DL_IP]->dlIpThruput.timeSummation
         += l2MeasIpThruput->tempTimeT1 - l2MeasIpThruput->timeT2;

         /*Reset the IP throughput related parameters*/
         kwUtlResetKwRbL2Meas(rbCb);
      }
   }
} /* kwUmmMeasDlIpBrstCont */

/* kw006.201 CR:ccpu00120058, added new function */
/**
 *
 * @brief Private handler to measure DL IP throughput.
 *
 * @b Description
 *      This function is called from kwUmmCalDlIpThrPut
 *      If the RLC bo is more than allocation from MAC,
 *        T2 time stamp is taken and stored in KwRbCb.
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to TRUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *      If the allocation from MAC is emptying the RLC buffer,
 *        Burst is said to be COMPLETED.
 *        T1 time stamp is calculated in kwUtlProcHarqInd
 *
 * @param[in]  rbCb       RB control block
 * @param[in]  dataVol    Data volume
 * @param[in]  initialBo  BO of RLC entity before constructing the PDU
 *
 *  @return  Void
 *
 */
/* kw006.201 ccpu00120058 fixed non ansi compilation problem */
#ifdef ANSI
PRIVATE Void kwUmmMeasDlIpBrstStart
(
KwRbCb      *rbCb,
MsgLen      dataVol,
S32         initialBo
)
#else
PRIVATE Void kwUmmMeasDlIpBrstStart(rbCb, dataVol, initialBo)
KwRbCb      *rbCb;
MsgLen      dataVol;
S32         initialBo;
#endif
{
   KwTbSnMap         *curTbSnMap      = NULLP; /*Holds the current TbSnMap*/
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;

   TRC2(kwUmmMeasDlIpBrstStart)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL), (_kwPBuf,
           "kwUmmMeasDlIpBrstStart(rbCb, dataVol, initialBo)\n"));

   /*Get the current TbSnMap*/
   curTbSnMap = kwCb.kwL2Cb.curTbSnMap;

   l2MeasIpThruput = &rbCb->l2MeasIpThruput;
   if(initialBo > dataVol)
   {
      /*Change the state to CONTINUE since T2 is confirmed now*/
      l2MeasIpThruput->dlIpThrputSt = KW_DL_IPTHRU_BURST_CONTINUE;

      /*It means data burst is splitted across TTI. So set the*
       *isBurstSplitted flag to TRUE.                         */
      curTbSnMap->snSduMap[curTbSnMap->numSn -1].isBurstSplitted = TRUE;

      /*This count will be decremented when HARQ Ack will be received*
       *for this TbId. It is done in kwUtlProcHarqInd.               */
      l2MeasIpThruput->count++;

      l2MeasIpThruput->dataVol += dataVol;
   }
   else
   {
      /*Since 2nd allocation is emptying the buffer, isBurstSplitted *
       *is set to FALSE. If count is 0, then it means that the ACK or*
       *NACK has been received and the corresponding KwTbSnMap is    *
       *freed(in KwUtlProcHarqInd) and so prevTbSnMap has become a   *
       *dangling pointer and so it is set to NULLP.                  */
      if(l2MeasIpThruput->count == 1)
      {
         l2MeasIpThruput->prevTbSnMap->snSduMap[l2MeasIpThruput->prevTbSnMap->numSn - 1].isBurstSplitted = FALSE;
      }
      else if(l2MeasIpThruput->count == 0)
      {
         l2MeasIpThruput->prevTbSnMap = NULLP;
      }
      else
      {
   /* kw006.201 ccpu00120058 removed 64 bit compilation warning */
#ifndef ALIGN_64BIT
         KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_DL),
         (_kwPBuf, "kwUmmMeasDlIpBrstStart: count can be either 1 or 0.\
          L2Mcount = %d \n", l2MeasIpThruput->count));
#else
         KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_DL),
         (_kwPBuf, "kwUmmMeasDlIpBrstStart: count can be either 1 or 0.\
         L2Mcount = %d \n", l2MeasIpThruput->count));
#endif
      }

      /*Change state back to RESET*/
      kwUtlResetKwRbL2Meas(rbCb);
   }
} /* kwUmmMeasDlIpBrstStart */

/* kw006.201 CR:ccpu00120058, added new function */
/**
 *
 * @brief Private handler to measure DL IP throughput.
 *
 * @b Description
 *      This function is called from kwUmmCalDlIpThrPut
 *      If the RLC bo is more than allocation from MAC,
 *        T2 time stamp is taken and stored in KwRbCb.
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to TRUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *      If the allocation from MAC is emptying the RLC buffer,
 *        Burst is said to be COMPLETED.
 *        T1 time stamp is calculated in kwUtlProcHarqInd
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  dataVol  Data volume
 * @param[in]  initialBo  BO of RLC entity before constructing the PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwUmmMeasDlIpBrstReset
(
KwRbCb      *rbCb,
MsgLen      dataVol,
S32         initialBo
)
#else
PRIVATE Void kwUmmMeasDlIpBrstReset(rbCb, dataVol, initialBo)
KwRbCb      *rbCb;
MsgLen      dataVol;
S32         initialBo;
#endif
{
   KwTbSnMap      *curTbSnMap = NULLP; /*Holds the current TbSnMap*/
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;
   EpcTime timeSt;

   TRC2(kwUmmMeasDlIpBrstReset)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL), (_kwPBuf,
           "kwUmmMeasDlIpBrstReset(rbCb, dataVol, initialBo)\n"));

   /*Get the current TbSnMap*/
   curTbSnMap = kwCb.kwL2Cb.curTbSnMap;

   l2MeasIpThruput = &rbCb->l2MeasIpThruput;
   if(initialBo > dataVol)
   {
      /*Change state from RESET to BURST_STARTED*/
      l2MeasIpThruput->dlIpThrputSt = KW_DL_IPTHRU_BURST_STARTED;

      /*Take timestamp for T2.*/
      SGetEpcTime(&timeSt);

      /*Store in KwRbCb.*/
      l2MeasIpThruput->timeT2 = timeSt;

      /*Preserve the curent TbSnMap in KwRbCb upto next allocation.*/
      l2MeasIpThruput->prevTbSnMap = curTbSnMap;

      /*It means data burst is splitted across TTI. So set the*
       *isBurstSplitted flag to TRUE.                         */
      curTbSnMap->snSduMap[curTbSnMap->numSn -1].isBurstSplitted = TRUE;

      /*This count will be decremented when HARQ Ack will be received*
       *for this TbId. It is done in kwUtlProcHarqInd.               */
      l2MeasIpThruput->count++;

      l2MeasIpThruput->dataVol += dataVol;
   }
   else
   {
      /* NOP */
   }
} /* kwUmmMeasDlIpBrstReset */

/* kw006.201 CR:ccpu00120058, added new function */
/**
 *
 * @brief Private handler to measure DL IP throughput.
 *
 * @b Description
 *      This function is called from kwUmmCalDlIpThrPut
 *      If the RLC bo is more than allocation from MAC,
 *        T2 time stamp is taken and stored in KwRbCb.
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to TRUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *      If the allocation from MAC is emptying the RLC buffer,
 *        Burst is said to be COMPLETED.
 *        T1 time stamp is calculated in kwUtlProcHarqInd
 *
 * @param[in]  rbCb       RB control block
 * @param[in]  dataVol    Data volume
 * @param[in]  initialBo  BO of RLC entity before constructing the PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwUmmMeasDlIpBrstComplt
(
KwRbCb      *rbCb,
MsgLen      dataVol,
S32         initialBo
)
#else
PRIVATE Void kwUmmMeasDlIpBrstComplt(rbCb, dataVol, initialBo)
KwRbCb      *rbCb;
MsgLen      dataVol;
S32         initialBo;
#endif
{
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;

   TRC2(kwUmmMeasDlIpBrstComplt)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL), (_kwPBuf,
           "kwUmmMeasDlIpBrstComplt(rbCb, dataVol, initialBo)\n"));

   l2MeasIpThruput = &rbCb->l2MeasIpThruput;
   if((initialBo > dataVol) && (l2MeasIpThruput->count > 0))
   {
      /*Change state from RESET to BURST_CONTINUE*/
      l2MeasIpThruput->dlIpThrputSt = KW_DL_IPTHRU_BURST_CONTINUE;

      l2MeasIpThruput->count++;

      l2MeasIpThruput->dataVol += dataVol;
   }
} /* kwUmmMeasDlIpBrstComplt */

/**
 *
 * @brief Private handler to calculate DL IP throughput.
 *
 * @b Description
 *      This function is called from kwUmmProcessSdus.
 *      If the RLC bo is more than allocation from MAC,
 *        T2 time stamp is taken and stored in KwRbCb.
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to TRUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *      If the allocation from MAC is emptying the RLC buffer,
 *        Burst is said to be COMPLETED.
 *        T1 time stamp is calculated in kwUtlProcHarqInd
 *
 * @param[in]  rbCb       RB control block
 * @param[in]  dataVol    Data volume
 * @param[in]  initialBo  BO of RLC entity before constructing the PDU
 * @param[in]  pduSz      pdu size
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwUmmCalDlIpThrPut
(
KwRbCb          *rbCb,
MsgLen          dataVol,
S32             initialBo,
MsgLen          pduSz
)
#else
PRIVATE Void  kwUmmCalDlIpThrPut (rbCb, dataVol, initialBo, pduSz)
KwRbCb          *rbCb;
MsgLen          dataVol;
S32             initialBo;
MsgLen          pduSz;
#endif
{
   KwTbSnMap      *curTbSnMap = NULLP; /*Holds the current TbSnMap*/
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;

   TRC2(kwUmmCalDlIpThrPut)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL), (_kwPBuf,
           "kwUmmCalDlIpThrPut(rbCb, dataVol, initialBo, pduSz)\n"));

   /*Get the current TbSnMap*/
   curTbSnMap = kwCb.kwL2Cb.curTbSnMap;

   l2MeasIpThruput = &rbCb->l2MeasIpThruput;
   /* kw006.201 ccpu00120058 removed 64 bit compilation warning */
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
   (_kwPBuf, "kwUmmCalDlIpThrPut: Log for dl ip throughput:\n \
    RB_MeasOn: %d\n curTbSnMap->numSn: %d\n State: %d\n      \
    Count: %d\n  dataVol: %lu\n prevTbSnMap: %p\n initialBo: %ld\n \
    Alloc from MAC: %d\n", rbCb->rbL2Cb.measOn, curTbSnMap->numSn,
    l2MeasIpThruput->dlIpThrputSt, l2MeasIpThruput->count,
    l2MeasIpThruput->dataVol, (Void*)l2MeasIpThruput->prevTbSnMap,
    initialBo, pduSz));
#else
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
   (_kwPBuf, "kwUmmCalDlIpThrPut: Log for dl ip throughput:\n \
    RB_MeasOn: %d\n curTbSnMap->numSn: %d\n State: %d\n      \
    Count: %d\n  dataVol: %u\n prevTbSnMap: %p\n initialBo: %d\n \
    Alloc from MAC: %d\n", rbCb->rbL2Cb.measOn, curTbSnMap->numSn,
    l2MeasIpThruput->dlIpThrputSt, l2MeasIpThruput->count,
    l2MeasIpThruput->dataVol, (Void*)l2MeasIpThruput->prevTbSnMap,
    initialBo, pduSz));
#endif
   /*Check if DL ip throughput measurement is going on or not AND at least*
    *one new data PDU is formed.                                          */
   /* kw006.201 ccpu00120058, reduced code complexity by adding new function */
   if( (0 != (rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_IP)) && (curTbSnMap->numSn > 0) )
   {
      if(KW_DL_IPTHRU_BURST_CONTINUE == l2MeasIpThruput->dlIpThrputSt)
      {
         kwUmmMeasDlIpBrstCont(rbCb, dataVol, initialBo);
      }
      /*2nd allocation, T2 already stamped*/
      else if (KW_DL_IPTHRU_BURST_STARTED == l2MeasIpThruput->dlIpThrputSt)
      {
         kwUmmMeasDlIpBrstStart(rbCb, dataVol, initialBo);
      }
      /*1st allocation for the current burst*/
      else if(KW_DL_IPTHRU_RESET == l2MeasIpThruput->dlIpThrputSt)
      {
         kwUmmMeasDlIpBrstReset(rbCb, dataVol, initialBo);
      }
      /*If a subsequent burst is split across TTI but the previous burst is*
       *not fully Ack-ed by MAC, then the subsequent bursts will be        *
       *considered as part of previous burst and so state is changed to    *
       *CONTINUE.                                                          */
      else /*dlIpThrputSt is KW_DL_IPTHRU_BURST_COMPLETED*/
      {
         kwUmmMeasDlIpBrstComplt(rbCb, dataVol, initialBo);
      }
   }
   /* kw006.201 ccpu00120058 removed 64 bit compilation warning */
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
   (_kwPBuf, "kwUmmCalDlIpThrPut: Log for dl ip throughput:\n \
    RB_MeasOn: %d\n curTbSnMap->numSn: %d\n State: %d\n      \
    Count: %d\n  dataVol: %lu\n prevTbSnMap: %p\n initialBo: %ld\n \
    Alloc from MAC: %d\n", rbCb->rbL2Cb.measOn, curTbSnMap->numSn,
    l2MeasIpThruput->dlIpThrputSt, l2MeasIpThruput->count,
    l2MeasIpThruput->dataVol, (Void*)l2MeasIpThruput->prevTbSnMap,
    initialBo, pduSz));
#else
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
   (_kwPBuf, "kwUmmCalDlIpThrPut: Log for dl ip throughput:\n \
    RB_MeasOn: %d\n curTbSnMap->numSn: %d\n State: %d\n      \
    Count: %d\n  dataVol: %u\n prevTbSnMap: %p\n initialBo: %d\n \
    Alloc from MAC: %d\n", rbCb->rbL2Cb.measOn, curTbSnMap->numSn,
    l2MeasIpThruput->dlIpThrputSt, l2MeasIpThruput->count,
    l2MeasIpThruput->dataVol, (Void*)l2MeasIpThruput->prevTbSnMap,
    initialBo, pduSz));
#endif
} /* kwUmmCalDlIpThrPut */
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */


/**
 *
 * @brief
 *
 *        Handler to form PDU(s) and update the BO.
 *
 * @b Description:
 *
 *        -# This function forms pdu(s) from the SDU(s) in the
 *        SDU queue and returns them.
 *        -# This function also updates the BO along with the
 *        along with the estimated Header size.
 *
 * @param[in]  rbCb     RB control block
 * @param[out] pduInfo  Pdu Info to be filled and the PDU size to be formed and the updated BO
 * @param[in]  pduSz    The size for which PDUs have to constructed
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 kwUmmProcessSdus
(
KwRbCb    *rbCb,                /* Rb Control Block */
KwDatReq  *datReq               /* PDU Info to be filled */
)
#else
PUBLIC S16 kwUmmProcessSdus(rbCb, datReq)
KwRbCb    *rbCb;                /* Rb Control Block */
KwDatReq  *datReq;              /* PDU Info to be filled */
#endif
{
   CmLList *firstNode;          /* First Node in SDU queue */
   CmLListCp *sduQ;             /* SDU queue */
   U8      fi;                  /* Framing Info */
   Buffer  *pdu;                /* Buffer for holding the formed PDU */
   S32     *bo;                 /* BO   */
   U16     sduCount;            /* SDU count */
   /* kw006.201, ccpu00120574, fixed the warning */
   KwPduInfo *pduInfo;          /* PDU Info pointer */
   S16     pduSz;               /* PDU Size to be constructed */
   S16     ret;                 /* Return value */
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   Bool    dataWasPrsnt;        /* Initially was data existing? */
#ifdef LTE_RLC_R9
   S32            initialBo   = 0;     /*Used to compare with DL allocation*/
   U16            dataVol     = 0;     /* Holds the sum of added RLC SDUs */
   MsgLen         sduSz;
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

   /* kw005.201, updated for statistics */
   KwKwuSapCb   *kwuSapCb;      /* KWU SapCb */

   TRC2(kwUmmProcessSdus)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL), (_kwPBuf,
           "kwUmmProcessSdus(rbCb, datReq)\n"));
   if (rbCb->reestInProgress == TRUE)
   {
      MSPD_DBG("Process Sdu getting called even when re-establishment is in progress\n");	   
      RETVALUE(ROK);
   }
   fi = 0;
   sduCount = 0;
   pdu = NULLP;
   bo = &(rbCb->m.um.umDl.bo);
   sduQ = &(rbCb->m.um.umDl.sduQ);
   /* kw006.201, ccpu00120574, fixed the warning */
   pduInfo = &(datReq->pduInfo);
   pduSz = datReq->pduSz;
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   dataWasPrsnt   =  sduQ->count > 0? TRUE: FALSE;
#ifdef LTE_RLC_R9
   initialBo = *bo;
#endif /* LTE_RLC_R9 */
#endif /*  LTE_L2_MEAS */

   while( pduSz > KW_UM_MIN_HDR_SZ && sduQ->count > 0 )
   {
      Buffer *tmpPdu = NULLP;
      KwSdu  *sdu;
      MsgLen *msgLen;

      kwUmmDlGetSdu(rbCb, &firstNode);
      //CM_LLIST_FIRST_NODE(sduQ,firstNode);
      sdu = (KwSdu *)(firstNode->node);
      msgLen = &(sdu->sduSz);
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
      sduSz = sdu->sduSz;
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */
      //MSPD_DBG("UM: ProcessSdu SduId %d\n", sdu->mode.um.sduId);

      /* When forming a new PDU
           -# Eliminate MAC header size for each pdu
           -# Initialize the li array to 0
           -# Substract the fixed header length based on SN length
      */
      if ( pdu == NULLP )
      {
         KW_RMV_MAC_HDR_SZ(pduSz);
         UMDL.numLi = 0;
         if ( sdu->mode.um.isSegmented == TRUE )
            fi = 2;
         else
            fi = 0;
         KW_RMV_RLC_HDR_SZ(pduSz, rbCb->m.um.umDl.snLen);
         /* kw005.201 fixing pduSz <= 0 problem, ccpu00119417 */
         if(pduSz <= 0)
         {
            break;
         }
      }
      kwUtlCalcLiForSdu(rbCb->m.um.umDl.numLi,*msgLen,&pduSz);

      /* Exact fit scenario :
         If the SDU size matches with the PDU size
           -# Allocate memory equal to PDU size;
           -# update BO
           -# Remove SDu from queue
           -# Append to already existing PDU portion if present .
           -# Add Header and create complete PDU and place it in pduInfo and return
      */
      if ( *msgLen == pduSz )
      {
         SGetMsg(KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb),&tmpPdu);

         ret = SCatMsg(tmpPdu,sdu->mBuf,M1M2);
#if (ERRCLASS & ERRCLS_DEBUG)
            if ( ROK != ret )
            {
               /* SCatMsg failed */
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"SCatMsg Failed\n"));
               if ( tmpPdu )
               {
                  SPutMsg(tmpPdu);
               }
               (UMDL.numLi)--;
               break;
            }
#endif
         if ( pdu == NULLP )
         {
            pdu = tmpPdu;
         }
         else
         {
            ret = SCatMsg(pdu,tmpPdu,M1M2);
#if (ERRCLASS & ERRCLS_DEBUG)
            if ( ROK != ret )
            {
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"SCatMsg Failed\n"));
               sdu->mBuf = tmpPdu;
               (UMDL.numLi)--;
               break;
            }
#endif
            SPutMsg(tmpPdu);
         }
         kwUmmUpdateBo(rbCb, pduSz);
         pduSz = 0;

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         kwUtlUpdSduSnMap(rbCb, sdu, datReq, TRUE);
#endif /*  LTE_L2_MEAS */
         //KW_RMV_SDU_UM(&(rbCb->m.um.umDl.sduQ),sdu); /* kw003.201 */
         kwUmmDlRemSdu(rbCb, sdu);
         KW_CREATE_UM_PDU(rbCb,pdu,fi,pduInfo,ret);
         kwCb.genSts.pdusSent++;
#if (ERRCLASS & ERRCLS_DEBUG)
         if ( ret != ROK )
         {
            KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"KW_CREATE_UM_PDU Failed\n"));
            SPutMsg(pdu);
            break;
         }
#endif
         /* kw005.201 ccpu00117318, updating the statistics */
         kwuSapCb = (KwKwuSapCb *)(kwCb.kwuSap + rbCb->kwuSapId);
         kwuSapCb->sts.sduTx++;
         sduCount++;
      }
      /* Concatenation scenario :
         If SDU size is less than the requested PDU size
           -# Allocate memory and copy SDU into it.
           -# Update BO
           -# Remove SDU from the Queue.
           -# Append to already existing PDU portion if present .
           -# If the SDU size is greater than 2047 or the number of LIs reaches max,
                 place it as a separate PDU in pduInfo and set pdu to NULL
              else
                 place the msglen in li array and continue with the next SDU.
           -# If the number of PDUs is more than KW_MAX_PDU, return from the function
              even if pduSize > 0.
      */
      else if (*msgLen < pduSz )
      {
         SGetMsg(KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb),&tmpPdu);
         ret = SCatMsg(tmpPdu,sdu->mBuf,M1M2);
#if (ERRCLASS & ERRCLS_DEBUG)
            if ( ROK != ret )
            {
               /* SCatMsg Failed */
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"SCatMsg Failed\n"));
               if ( tmpPdu )
               {
                  SPutMsg(tmpPdu);
               }
               (UMDL.numLi)--;
               break;
            }
#endif
         if (pdu == NULLP)
         {
            pdu = tmpPdu;
         }
         else
         {
            ret = SCatMsg(pdu,tmpPdu,M1M2);
#if (ERRCLASS & ERRCLS_DEBUG)
            if ( ROK != ret )
            {
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"SCatMsg Failed\n"));
               sdu->mBuf = tmpPdu;
               (UMDL.numLi)--;
               break;
            }
#endif
            SPutMsg(tmpPdu);
         }
         kwUmmUpdateBo(rbCb, *msgLen);

         pduSz -= *msgLen;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
          kwUtlUpdSduSnMap(rbCb, sdu, datReq, TRUE);
#endif /*  LTE_L2_MEAS */
         if ( *msgLen < 2048 && UMDL.numLi < KW_MAX_LI )
         {
            rbCb->m.um.umDl.li[(UMDL.numLi)++] = *msgLen;
         }
         else
         {
            KW_CREATE_UM_PDU(rbCb,pdu,fi,pduInfo,ret);
            kwCb.genSts.pdusSent++;
#if (ERRCLASS & ERRCLS_DEBUG)
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"KW_CREATE_UM_PDU Failed\n"));
               SPutMsg(pdu);
               break;
            }
#endif
            if ( pduInfo->numPdu == KW_MAX_PDU)
            {
               /* Could not transmit what MAC asked for because the number of PDUs to be transmitted has reached maximum. */
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF| KW_DBGMASK_DL),
                  (_kwPBuf,"Maximum Pdu limit has been reached\n"));
               break;
            }
         }
         //KW_RMV_SDU_UM(&(rbCb->m.um.umDl.sduQ),sdu);
         kwUmmDlRemSdu(rbCb, sdu);
         /* kw005.201 ccpu00117318, updating the statistics */
         kwuSapCb = (KwKwuSapCb *)(kwCb.kwuSap + rbCb->kwuSapId);
         kwuSapCb->sts.sduTx++;
         sduCount++;
      }
      /* Segmentation scenario :
         If size of SDU is greater than PDU size
           -# Allocate memory and Segment the Sdu.
           -# Update BO
           -# Append to already existing PDU if any.
           -# Set the second bit of the framing info.
           -# Create the complete PDU and place in pduInfo.
      */
      else {
         Buffer *remSdu;

         SGetMsg(KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb),&tmpPdu);
         ret = SSegMsg(sdu->mBuf,pduSz,&remSdu);
#if (ERRCLASS & ERRCLS_DEBUG)
            if ( ROK != ret )
            {
               /* SSegMsg failed */
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"SSegMsg Failed\n"));
               if ( tmpPdu )
               {
                  SPutMsg(tmpPdu);
               }
               (UMDL.numLi)--;
               break;
            }
#endif
         ret = SCatMsg(tmpPdu,sdu->mBuf,M1M2);
#if (ERRCLASS & ERRCLS_DEBUG)
            if ( ret != ROK )
            {
                KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"SCatMsg Failed\n"));
               if ( tmpPdu )
               {
                  SPutMsg(tmpPdu);
               }
               SPutMsg(sdu->mBuf);
               //KW_RMV_SDU_UM(sduQ,sdu);
               kwUmmDlRemSdu(rbCb, sdu);
               (UMDL.numLi)--;
               break;
            }
#endif
         SPutMsg(sdu->mBuf);
         sdu->mBuf = remSdu;
         *msgLen -= pduSz;
         kwUmmUpdateBo(rbCb, pduSz);
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
         sduSz = pduSz;
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */
         sdu->mode.um.isSegmented = TRUE;

         pduSz = 0;

         if (pdu == NULLP)
            pdu = tmpPdu;
         else
         {
            ret = SCatMsg(pdu,tmpPdu,M1M2);
#if (ERRCLASS & ERRCLS_DEBUG)
            if ( ROK != ret )
            {
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"SCatMsg Failed\n"));
               SPutMsg(tmpPdu);
               (UMDL.numLi)--;
               break;
            }
#endif
            SPutMsg(tmpPdu);
         }
         fi |= 1;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         kwUtlUpdSduSnMap(rbCb, sdu, datReq, FALSE);
#endif /*  LTE_L2_MEAS */
         KW_CREATE_UM_PDU(rbCb,pdu,fi,pduInfo,ret);
         kwCb.genSts.pdusSent++;
#if (ERRCLASS & ERRCLS_DEBUG)
         if ( ret != ROK )
         {
            KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"KW_CREATE_UM_PDU Failed\n"));
            SPutMsg(pdu);
            break;
         }
#endif
      }
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
      dataVol += sduSz;
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */
   }
#ifdef LTE_L2_MEAS
   if((rbCb->rbL2Cb.measOn) &&
      (sduQ->count == 0)    &&
      (dataWasPrsnt))
   {
      if(--(rbCb->ueCb->numActRb[rbCb->qci]) == 0)
      {
         kwCb.kwL2Cb.numActUe[rbCb->qci]--;
      }
   }
#ifdef LTE_RLC_R9
   /* Calculate DL IP throughput */
   kwUmmCalDlIpThrPut(rbCb, dataVol, initialBo, datReq->pduSz);
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

   /* If we have a situation wherein the size requested is greater than the total size of SDUs
      and a pdu buffer which is not null, this if loop helps to send
      a non null PDU to the lower layer.
   */
   if ( pduSz > 0 && pdu != NULLP )
   {
      if ( pduInfo->numPdu != KW_MAX_PDU )
      {
         rbCb->m.um.umDl.numLi--;
         KW_CREATE_UM_PDU(rbCb,pdu,fi,pduInfo,ret);
         kwCb.genSts.pdusSent++;
#if (ERRCLASS & ERRCLS_DEBUG)
         if ( ret != ROK )
         {
            KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR| KW_DBGMASK_DL),
                  (_kwPBuf,"KW_CREATE_UM_PDU Failed\n"));
            SPutMsg(pdu);
         }
#endif
      }
      else
          SPutMsg(pdu);
   }
   kwUmmEstHdrSz(rbCb,KW_DEL_SDU,sduCount);
   datReq->boRep.bo = UMDL.bo;
   datReq->boRep.estHdrSz = UMDL.estHdrSz;
   datReq->boRep.staPduPrsnt = FALSE;
#ifdef MSPD
   ysMsUmBo = UMDL.bo;
#endif
   RETVALUE(ROK);
}

#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
/* kw006.201 ccpu00120058, reduced code complexity by adding new function */
/**
 *
 * @brief
 *
 *        Handler to calculate the Ul Ip throughput for a LCH
 *
 * @b Description:
 *
 *        If SegmentIndication is FIRST or INTERIM
 *           Calculate the RLC SDU size.
 *        If SegmentIndication is LAST
 *           Do not take the corresponding RLC SDU size.
 *           Take the timing info.
 *
 * @param[in] rbCb         RB control block
 * @param[in] pdu          Pdu of LCH
 * @param[in] ulTimeInfo   Timing info(T1-T2) for this LCH
 * @param[in] insideLoop   It is called from within while loop or not.
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwUmmCalUlIpThrPut
(
KwRbCb                *rbCb,
Buffer                *pdu,
KwL2MeasUlThrpTimInfo *ulTimeInfo,
Bool                  insideLoop
)
#else
PRIVATE Void kwUmmCalUlIpThrPut(rbCb, pdu, ulTimeInfo, insideLoop)
KwRbCb                *rbCb;
Buffer                *pdu;
KwL2MeasUlThrpTimInfo *ulTimeInfo;
Bool                  insideLoop;
#endif
{
   MsgLen              rlcSduSz = 0;  /*Holds length of Rlc Sdu*/
   /* kw006.201 ccpu00120058 removed 64 bit compilation warning */
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
   (_kwPBuf, "kwUmmCalUlIpThrPut: Log for ul ip throughput:\n \
   RB_MeasOn:%d SegInd:%d thrpTimeUl:%lu\n", rbCb->rbL2Cb.measOn, \
   ulTimeInfo->firstLastSegInd, ulTimeInfo->thrpTimeUl));
#else
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
   (_kwPBuf, "kwUmmCalUlIpThrPut: Log for ul ip throughput:\n \
   RB_MeasOn:%d SegInd:%d thrpTimeUl:%u\n", rbCb->rbL2Cb.measOn, \
   ulTimeInfo->firstLastSegInd, ulTimeInfo->thrpTimeUl));
#endif
   /*Check if UL IP throughput measurement is ON for this RB or not*/
   if(0 != (rbCb->rbL2Cb.measOn & LKW_L2MEAS_UL_IP) )
   {
      if(TRUE == insideLoop)
      {
         /*If the indication is first and interim, then calculate the RLC SDU *
          *size and add it to the ulDataVol. If it is last, then its data size*
          *should not be added.                                               */
         if( (RGU_L2M_ULTHRP_FIRST == ulTimeInfo->firstLastSegInd) ||
             (RGU_L2M_ULTHRP_INTERIM == ulTimeInfo->firstLastSegInd))
         {
            SFndLenMsg(pdu, &rlcSduSz);
            rbCb->l2MeasIpThruput.ulDataVol += rlcSduSz;
         }
      }
      else
      {
         /*If the indication is last*/
         if(RGU_L2M_ULTHRP_LAST == ulTimeInfo->firstLastSegInd)
         {
            /*If the indication is last and time difference is 0, then it means*
             *that ul burst for this LCH is split across 2 TTIs and so UL IP   *
             *measurement can not be taken. So reset the data volume.          */
            if(0 == ulTimeInfo->thrpTimeUl)
            {
               rbCb->l2MeasIpThruput.ulDataVol = 0;
            }
            /*Else save the total data volume and time difference in KwL2Cntr*/
            else
            {
               rbCb->rbL2Cb.l2Sts[KW_L2MEAS_UL_IP]->ulIpThruput.volSummation +=
                  rbCb->l2MeasIpThruput.ulDataVol;
               rbCb->rbL2Cb.l2Sts[KW_L2MEAS_UL_IP]->ulIpThruput.timeSummation +=
                  ulTimeInfo->thrpTimeUl;
               /*Reset ulDataVol in rbCb since it has been saved in l2Sts*/
               rbCb->l2MeasIpThruput.ulDataVol = 0;
            } /* end of if(0 == ulTimeInfo->thrpTimeUl) */
         } /* end of if(RGU_L2M_ULTHRP_LAST == ulTimeInfo->firstLastSegInd) */
      } /* end of if(TRUE == insideLoop) */
   } /* end of if(0 != (rbCb->rbL2Cb.measOn & LKW_L2MEAS_UL_IP) ) */
} /* kwUmmCalcUlIpThrPut */
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

/* kw006.201 ccpu00118629, Replacing large macros with functions */
/**
 *
 * @brief
 *        Handler to check whether a received PDU is duplicate or not
 *
 * @b Description:
 *        This function is called from kwUmmProcessPdus.
 *        It is called for each SN received from MAC to check whether it
 *        duplicate or not.
 *
 * @param[in] rbCb   RB control block
 * @param[in] curSn  SN of the received RLC PDU
 *
 *  @return  S16
 *      -# TRUE
 *      -# FALSE
 *
 */
#ifdef ANSI
PRIVATE S16 kwUmmCheckDupPdu
(
KwRbCb *rbCb,
U16    curSn
)
#else
PRIVATE S16 kwUmmCheckDupPdu(rbCb, curSn)
KwRbCb *rbCb;
U16    curSn;
#endif
{
   KwSn   ur, uh, sn;
   KwSn   winSz, umReOrdWinSz, vrUh, vrUr;

   winSz = UMUL.umWinSz << 1;
   umReOrdWinSz = UMUL.umWinSz;
   vrUh = UMUL.vrUh;
   vrUr = UMUL.vrUr;

   ur = KW_UM_GET_VALUE(vrUr, vrUh, umReOrdWinSz, winSz);
   uh = KW_UM_GET_VALUE(vrUh, vrUh, umReOrdWinSz, winSz);
   sn = KW_UM_GET_VALUE(curSn, vrUh, umReOrdWinSz, winSz);

   if ( ((ur < sn) && (sn < uh) &&
         (rbCb->m.um.umUl.recBuf[curSn] != NULLP)) ||
         (sn < ur))
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
} /* kwUmmCheckDupPdu */

/* kw006.201 ccpu00118629, Replacing large macros with functions */
/**
 *
 * @brief
 *        Handler to find the next VrUr
 *
 * @b Description:
 *        This function is called from kwUmmProcessPdus.
 *
 * @param[in] rbCb   RB control block
 * @param[in] curSn  SN of the received RLC PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwUmmFindNextVrUr
(
KwUmUl *umUl,
KwSn   prevVrUr,
KwSn   *vrUr
)
#else
PRIVATE Void kwUmmFindNextVrUr(umUl, prevVrUr, vrUr)
KwUmUl *umUl;
KwSn   prevVrUr;
KwSn   *vrUr;
#endif
{
   KwSn umWinSz, vrUh, ur, prvUr, umReOrdWinSz;
   KwUmRecBuf **umRecBuf;

   umRecBuf = umUl->recBuf;
   umWinSz      = umUl->umWinSz << 1;
   vrUh         = umUl->vrUh;
   umReOrdWinSz = umUl->umWinSz;
   ur = KW_UM_GET_VALUE(*vrUr, vrUh, umReOrdWinSz, umWinSz);
   prvUr = KW_UM_GET_VALUE(prevVrUr, vrUh, umReOrdWinSz, umWinSz);

   while (ur < prvUr )
   {
      if ( umRecBuf[prevVrUr] == NULLP )
      {
         *vrUr = prevVrUr;
         break;
      }
      prevVrUr = (prevVrUr + 1) % umWinSz;
      prvUr = KW_UM_GET_VALUE(prevVrUr, vrUh, umReOrdWinSz, umWinSz);
   }
}
#ifdef ANSI
PRIVATE Void kwUmmUlAddPduForReassembly
(
KwRbCb                  *rbCb,
KwUmRecBuf              *pdu
)
#else
PRIVATE Void kwUmmUlAddPduForReassembly(rbCb, pdu)
KwRbCb                  *rbCb;
KwUmRecBuf              *pdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.ulLock);
   {
      cmLListAdd2Tail(&UMUL.reassemblyLst, &(pdu->reassemblyLnk));
      pdu->reassemblyLnk.node = (PTR)pdu;
   }
   KW_RDWR_UNLOCK(&kwCb.ulLock);
   RETVOID;
}
/**
 *
 * @brief Function that is invoked when an in seuquence UL PDU rcvd.
 *
 * @b Description
 *        This function is responsible for processing receivind in seuqunce 
 *        UL PDU. Based on macro, the PDU may be queued up for delayed
 *        reassembly or immediate reassebmly.
 *
 * @param[in]  rbCb     RB control block 
 * @param[in]  sn       Sequence number of PDU to be reassembled
 *
 *  @return    Void
 *
 */
#ifdef ANSI
PRIVATE Void kwUmmUlProcessInSeqPdu
(
KwRbCb                       *rbCb,
U16                          sn
)
#else
PRIVATE Void kwUmmUlProcessInSeqPdu(rbCb, sn)
KwRbCb                       *rbCb;
U16                          sn;
#endif
{
   kwUmmUlAddPduForReassembly(rbCb, UMUL.recBuf[sn]);
   UMUL.recBuf[sn] = NULLP;
}
/**
 *
 * @brief
 *
 *        Handler to process the Data Indication from the lower layer and send the PDUs to
 *        re-assembly unit.
 *
 * @b Description:
 *
 *        This function processes the PDUs received from the lower layer
 *        re-orders them and sends them one after the other in sequence
 *        to the re-assembly unit.
 *
 * @param[in] rbCb     RB control block
 * @param[in] pduInfo  Pdu information
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 *
 */

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
#ifdef ANSI
PUBLIC S16 kwUmmProcessPdus
(
KwRbCb    *rbCb,                   /* Rb Control Block */
KwPduInfo *pduInfo,                 /* Pdu  data and related information */
KwL2MeasUlThrpTimInfo *ulTimeInfo  /* Ul timing info */
)
#else
PUBLIC S16 kwUmmProcessPdus(rbCb,pduInfo,ulTimeInfo)
KwRbCb    *rbCb;                   /* Rb Control Block */
KwPduInfo *pduInfo;                /* Pdu  data and related information */
KwL2MeasUlThrpTimInfo *ulTimeInfo; /* Ul timing info */
#endif
#else
#ifdef ANSI
PUBLIC S16 kwUmmProcessPdus
(
KwRbCb    *rbCb,                /* Rb Control Block */
KwPduInfo *pduInfo              /* Pdu  data and related information */
)
#else
PUBLIC S16 kwUmmProcessPdus(rbCb,pduInfo)
KwRbCb    *rbCb;                /* Rb Control Block */
KwPduInfo *pduInfo;             /* Pdu  data and related information */
#endif
#endif
#else
#ifdef ANSI
PUBLIC S16 kwUmmProcessPdus
(
KwRbCb    *rbCb,                /* Rb Control Block */
KwPduInfo *pduInfo              /* Pdu  data and related information */
)
#else
PUBLIC S16 kwUmmProcessPdus(rbCb,pduInfo)
KwRbCb    *rbCb;                /* Rb Control Block */
KwPduInfo *pduInfo;             /* Pdu  data and related information */
#endif
#endif
{
   U16 *vrUh;                   /* vr(uh) */
   U16 *vrUr;                   /* vr(ur) */
   KwSn *vrUx;                  /* vr(ux) */
   U16 curSn;                   /* Current Sequence Number */
   U8  pduCount;                /* PDU count */
   U8  count;                   /* Loop counter */
   U16 umReOrdWinSz;            /* UM ReOrder Window Size */
   KwUmHdr *umHdr;              /* UM Header Structure */
   KwUmRecBuf **recBuf;         /* UM Reception Buffer */
   S16 ret;                     /* Return Value */
   U16 umRcpWinSz;              /* Reception Window Size */
   KwUmUl *umUl;                /* UM UL Pointer */
   Bool tmrRunning;             /* Boolean for checking Tmr */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
/* kw006.201 ccpu00120058 */
   Bool insideLoop = TRUE;      /* TRUE: Ul Ip function is called from within*
                                 *while loop.                                */
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

   TRC2(kwUmmProcessPdus)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF| KW_DBGMASK_UL),
                  (_kwPBuf," kwUmmProcessPdus(rbCb,pduInfo) \n"));
   count = 0;

   pduCount = pduInfo->numPdu;
   vrUh   = &(rbCb->m.um.umUl.vrUh);
   vrUr   = &(rbCb->m.um.umUl.vrUr);
   vrUx   = &(rbCb->m.um.umUl.vrUx);
   recBuf = (rbCb->m.um.umUl.recBuf);
   umReOrdWinSz= rbCb->m.um.umUl.umWinSz;
   umRcpWinSz = umReOrdWinSz << 1;
   umHdr = NULLP;
   umUl = &(rbCb->m.um.umUl);

   while ( (count < pduCount) &&
           (count < RGU_MAX_PDU))
   {
      Buffer *pdu = pduInfo->mBuf[count];
      kwCb.genSts.pdusRecv++;
#ifdef MSPD
      {
          extern U32 ulrate_rgu;
          MsgLen len;
          SFndLenMsg(pdu, &len);
          ulrate_rgu += len;
      }
#endif

      if ( kwUmmExtractHdr(rbCb,pdu,&umHdr) != ROK )
      {
         KW_FREE(umHdr,sizeof(KwUmHdr));
         SPutMsg(pdu);
         count++;
         /* kw005.201 ccpu00117318, updating the statistics */
         kwCb.genSts.errorPdusRecv++;
         continue;
      }
      curSn = umHdr->sn;
      //MSPD_DBG("Received PDU (=%u) vrUr(=%u) vrUh(=%u) recBuf[*vrUh] %x\n",
      //curSn, *vrUr, *vrUh, recBuf[*vrUh]);

      ret = kwUmmCheckDupPdu(rbCb, curSn);
      /*KW_UM_CHK_DUP_PDU(rbCb,curSn,ret);*/
      if ( ret )
      {
         MSPD_DBG("Duplicate PDU (sn=%u) dropped\n", curSn);
         SPutMsg(pdu);
         KW_FREE(umHdr,sizeof(KwUmHdr));
         count++;
         /* kw005.201 ccpu00117318, updating the statistics */
         kwCb.genSts.unexpPdusRecv++;
         continue;
      }

      KW_ALLOC(recBuf[curSn],sizeof(KwUmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( recBuf[curSn] == NULLP )
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW215, (ErrVal) RFAILED,
            "KwRecBuf: recBuf allocation Failed.");
      KW_FREE(umHdr,sizeof(KwUmHdr));
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
   /* kw006.201 ccpu00120058, reduced code complexity by adding new function */
   kwUmmCalUlIpThrPut(rbCb, pdu, ulTimeInfo, insideLoop);
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

#ifdef MSPD
#if 0
      {
         static U32  lastSn = -1;
         if ((lastSn + 1 != curSn) &&
		 	!((lastSn == 31 && curSn==0) || (lastSn == 1023 && curSn==0))
		 	)
            MSPD_DBG("Missed/dup packet, lastSn=%u, curSn=%u VrUr(%u) VrUh(%u) VrUx(%u)\n",
	          lastSn, curSn,
	          rbCb->m.um.umUl.vrUr,rbCb->m.um.umUl.vrUh, rbCb->m.um.umUl.vrUx);
         lastSn = (U32)curSn;
      }
#endif
#endif
      recBuf[curSn]->pdu = pdu;
      recBuf[curSn]->umHdr = umHdr;
      SFndLenMsg(pdu,&(recBuf[curSn]->pduSz));
      /* kw005.201 ccpu00117318, updating the statistics */
      kwCb.genSts.bytesRecv += recBuf[curSn]->pduSz;
      KW_CHK_SN_IN_REORD_WIN(curSn,rbCb,ret);

      if ( !ret )
      {
         U16 prevVrUr = *vrUr;
         KwSn sn;
         KwSn ur;
         KwSn tSn;
         KwSn tVrUh = rbCb->m.um.umUl.vrUh;
         *vrUh  = (curSn + 1) % umRcpWinSz;
         KW_CHK_SN_IN_REORD_WIN(prevVrUr,rbCb,ret);
         if (!ret )
         {
            /* kw005.201 Added umRcpWinSz in calculating *vrUr */
            *vrUr = (*vrUh + umRcpWinSz - umReOrdWinSz) %  umRcpWinSz;
            sn = (prevVrUr + 1) % umRcpWinSz;
            tSn = KW_UM_GET_VALUE(sn,tVrUh,umReOrdWinSz,umRcpWinSz);
            ur = KW_UM_GET_VALUE(*vrUr,tVrUh,umReOrdWinSz,umRcpWinSz);
            while ( tSn < ur )
            {
               if ( recBuf[sn] != NULLP)
               {
                  MSPD_DBG("UM: Received1 SN %d\n", sn);
                  kwUmmUlProcessInSeqPdu(rbCb, sn);
               }
               sn = (sn + 1) % umRcpWinSz;
               tSn = KW_UM_GET_VALUE(sn,tVrUh,umReOrdWinSz,umRcpWinSz);
            }
         }
         /* kw006.201 fix for ccpu00120341, discarding the partial sdu
          * if sn is out of  reordering window */
         if (rbCb->m.um.umUl.partialSdu)
         {
            KW_CHK_SN_IN_REORD_WIN(rbCb->m.um.umUl.sn,rbCb,ret);
            if (!ret)
            {
               MsgLen len;
               SFndLenMsg(rbCb->m.um.umUl.partialSdu, &len);
               MSPD_DBG("Dropping Partial SDU len %d sn %d Curr Sn %d vrUr(%d) vrUh(%d) vrUx(%d)\n",
                         len, rbCb->m.um.umUl.sn, curSn,
                         rbCb->m.um.umUl.vrUr,rbCb->m.um.umUl.vrUh, rbCb->m.um.umUl.vrUx);
               SPutMsg(rbCb->m.um.umUl.partialSdu);
               rbCb->m.um.umUl.partialSdu = NULLP;
            }
         }
      }
      if ( recBuf[*vrUr] != NULLP )
      {
         KwSn prevVrUr = *vrUr;
         KwSn sn       = prevVrUr;
         KwSn tSn      = KW_UM_GET_VALUE(sn,*vrUh,umReOrdWinSz,umRcpWinSz);
         KwSn tVrUr;
        /* kw005.201 */
         KwSn nextVrUr = (prevVrUr + 1)%umRcpWinSz;

         /* kw005.201 */
         /* kw006.201 ccpu00118629, Replacing large macros with functions */
         kwUmmFindNextVrUr(umUl, nextVrUr, vrUr);
         /*KW_UM_FIND_NEXT_VRUR(umUl,nextVrUr,*vrUr);*/
         tVrUr = KW_UM_GET_VALUE(*vrUr,*vrUh,umReOrdWinSz,umRcpWinSz);
         while ( recBuf[sn] != NULLP && tSn < tVrUr )
         {
            /*Adding the received PDU to UmPduList. It will get reassembled when the*
             *self post message will be processed.                                  */
            MSPD_DBG("UM: Received2 SN %d\n", sn);
            kwUmmUlProcessInSeqPdu(rbCb, sn);
            sn = (sn + 1) % umRcpWinSz;
            tSn = KW_UM_GET_VALUE(sn,*vrUh,umReOrdWinSz,umRcpWinSz);
         }
      }

      tmrRunning = kwChkTmr((PTR)rbCb, KW_EVT_UMUL_REORD_TMR);

      if ( tmrRunning )
      {
         KwSn tVrUx = KW_UM_GET_VALUE(*vrUx,*vrUh,umReOrdWinSz,umRcpWinSz);
         KwSn tVrUr = KW_UM_GET_VALUE(*vrUr,*vrUh,umReOrdWinSz,umRcpWinSz);
         /* kw005.201 changes for ccpu00117408 */
         KwSn tVrUh = KW_UM_GET_VALUE(*vrUh,*vrUh,umReOrdWinSz,umRcpWinSz);

         KW_CHK_SN_IN_REORD_WIN(*vrUx,rbCb,ret);
         /* kw005.201 changes for ccpu00117408 */
         if ( (tVrUx <= tVrUr) || ((!ret) && (tVrUx != tVrUh)))
         {
            MSPD_DBG("Stopping reordering timer vrUx=%u vrUr=%u vrUh=%u\n",
                      *vrUx, *vrUr, *vrUh);
            kwStopTmr((PTR)rbCb,KW_EVT_UMUL_REORD_TMR);
         }
      }
      tmrRunning = kwChkTmr((PTR)rbCb, KW_EVT_UMUL_REORD_TMR);
      if ( !tmrRunning )
      {
         KwSn tVrUr = KW_UM_GET_VALUE(*vrUr,*vrUh,umReOrdWinSz,umRcpWinSz);
         KwSn tVrUh = KW_UM_GET_VALUE(*vrUh,*vrUh,umReOrdWinSz,umRcpWinSz);
         if ( tVrUh > tVrUr )
         {
	    MSPD_DBG("Starting reordering timer vrUr=%u vrUh=%u(=vrUx)\n",
                      *vrUr, *vrUh);
            kwStartTmr((PTR)rbCb,KW_EVT_UMUL_REORD_TMR);
            *vrUx = * vrUh;
         }
      }
      count++;
   }
/* kw006.201 ccpu00120058, reduced code complexity by adding new function */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
   insideLoop = FALSE;
   kwUmmCalUlIpThrPut(rbCb, NULLP, ulTimeInfo, insideLoop);
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

   /* Add the RB to the list of RBs which have PDUs to be reassembled */
   /* Post a message for reassembly which happens in background       */
   kwUtlUlAddRbToReassemblyLst(rbCb);
   //MSPD_DBG("Pdu received, about to post self message\n");
   kwUtlPostSelfMsgUl(2);

   RETVALUE(ROK);
} /* end of kwUmmProcessPdus */

/**
 *
 * @brief
 *
 *        Handler to reassemble the SDUs and send them to the upper layer.
 *
 * @b Description:
 *
 *        This function processes the received in-sequence PDU and
 *        re-assembles the SDUs and sends them to the upper layer.
 *
 * @param[in] rbCb       RB control block
 * @param[in] umRecBuf   Reception Buffer to be Re-Assembled
 * @param[in] numPdus         Number of PDCP PDUs to be delivered
 * @param[in] isPduProcessed  Pdu is fully processed or not
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 kwUmmReAssembleSdus
(
KwRbCb *rbCb,                   /* RB Control Block */
KwUmRecBuf *umRecBuf,           /* Pdu to be re-assembled */
U16 *numSdus,                   /* Max number of PDCP PDUs to be delivered */
Bool *isPduProcessd             /* Pdu is fully processed or not */
)
#else
PRIVATE S16 kwUmmReAssembleSdus(rbCb,umRecBuf,numSdus,isPduProcessd)
KwRbCb *rbCb;                   /* RB Control Block */
KwUmRecBuf *umRecBuf;           /* Pdu to be re-assembled */
U16 *numSdus;                   /* Max number of PDCP PDUs to be delivered */
Bool *isPduProcessd;            /* Pdu is fully processed or not */
#endif
{
   U16 liCount;                 /* LI count */
   U16 count;                   /* Loop counter */
   U8  fi;                      /* Framing Info */
   U16 sn;                      /* Sequence Number of current PDU */
   MsgLen len;                  /* PDU Length */
   Buffer *sdu;                 /* SDU to be sent to upper layer */
   Buffer *remPdu;              /* Remaining PDU */
   Buffer *pdu;                 /* PDU buffer */
   Buffer **partialSdu;         /* Partial SDU */

   TRC2(kwUmmReAssembleSdus)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF| KW_DBGMASK_UL),
                  (_kwPBuf," kwUmmReAssembleSdus(rbCb,umRecBuf) \n"));
   *isPduProcessd = TRUE;
#ifdef MSPD_MLOG_NEW
   kwUmmReassStart = GetTIMETICK();
#endif

   liCount = umRecBuf->umHdr->numLi;
   fi = umRecBuf->umHdr->fi;
   pdu = umRecBuf->pdu;
   sn  =  umRecBuf->umHdr->sn;
   MSPD_DBG("UM: Reassembling SN %d\n", sn);

   umRecBuf->pdu = NULLP;
   for ( count = umRecBuf->umHdr->numSduProcessed; (count <= liCount);count++ )
   {
      /*If numPdus equals max number of PDUs to be processed then break*
       *from the for loop and store the PDU context.                   */
      if(KW_AM_UM_MAX_UL_SDUS == *numSdus)
      {
         /*The for loop will iterate for at least one time. So, if the MSB of*
          *FI is 1, then 1st SDU(which is segmented) is used up during 1st   *
          *iteration of for loop. So, for the remaining portion of PDU, MSB  *
          *of FI can be set to 0. LSB of FI does not change.                 */
         umRecBuf->umHdr->fi = fi & 0x01;
         /*The remaining portion of PDU is assigned to pdu*/
         umRecBuf->pdu = pdu;
         /*Update the numSduProcessed with number of SDUs processed*/
         umRecBuf->umHdr->numSduProcessed = count;
         *isPduProcessd = FALSE;
         break;
      }
      if ( count < liCount )
         len = umRecBuf->umHdr->li[count];
      else
      {
         if (pdu == NULLP)
         {
           RETVALUE(ROK);
         }
         SFndLenMsg(pdu,&len);
      }

      SGetMsg(KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb), &sdu);
      SSegMsg(pdu,len,&remPdu);
      SCatMsg(sdu,pdu,M1M2);
      SPutMsg(pdu);
      pdu = remPdu;

      partialSdu = &(rbCb->m.um.umUl.partialSdu);
      /* While re-assembling the SDUs, consider the first LI and perform the following steps.
          -# If the first bit of FI(Framing Info of 2  bits) is set =>
                -# The current Data field in the PDU is a segment. So form a SDU only if the
                   rbCb->m.um.umUl.partialSdu exists and the SNs are in-sequence.
                -# If there are no LIs and the second bit of LI is 1 then a partial SDU is formed
                   which would not be sent to the upper layer.
          -# else
                -# If rbCb->partialSdu is not NULL then flush it off.
                -# If LI count > 0 or LI count is 0 and second bit of FI is not 1
                         The SDU is complete.So send it to upper layer.
                -# else
                         The SDU is partial and is placed in rbCb->m.um.umUl.partialSdu;
      */

      if (count == 0)
      {
         if ( fi & 2 )
         {
            /* kw005.201 */
            if ((*partialSdu != NULLP) &&
                               (sn == ((rbCb->m.um.umUl.sn + 1) % (rbCb->m.um.umUl.umWinSz << 1))))
            {
               SCatMsg(*partialSdu,sdu,M1M2);
               SPutMsg(sdu);
               if ( liCount > 0 || !(fi & 1))
               {
                  kwUtlSndDatInd(rbCb,*partialSdu);
                  *partialSdu = NULLP;
               }
            }
            else
            {
                /* Partial Sdu stored is not valid now.So free it */
               if (*partialSdu != NULLP)
               {
#ifdef MSPD
                  MsgLen len;
                  SFndLenMsg(*partialSdu, &len);
                  MSPD_DBG("Dropping Partial SDU %d Prev Sn %d Curr Sn %d vrUr(%d) vrUh(%d) vrUx(%d)\n",
                            len, rbCb->m.um.umUl.sn, sn,
                            rbCb->m.um.umUl.vrUr,rbCb->m.um.umUl.vrUh, rbCb->m.um.umUl.vrUx);
#endif
                  SPutMsg(*partialSdu);
                  *partialSdu = NULLP;
               }

               SPutMsg(sdu);
               sdu = NULLP;
            }
         }
         else
         {
            if ( *partialSdu )
               *partialSdu = NULLP;
            if (liCount > 0 || !( fi & 1))
            {
               kwUtlSndDatInd(rbCb,sdu);
            }
            else
            {
                  *partialSdu = sdu;
            }
         }
      }
      /*
          If the SDU pointer has the last Data field of the PDU
            -# If FI is 1,place the SDU in rbCb->m.um.umDl.partialSdu
            -# else send the SDU to upper layer.
      */
       else if ( count == liCount )
      {
         if ( fi & 1)
            *partialSdu = sdu;
         else
            kwUtlSndDatInd(rbCb,sdu);
      }
      /*
         If the LI is something other than the first one, just send the SDU to the upper layer */
      else
      {
         kwUtlSndDatInd(rbCb,sdu);
      }
      (*numSdus)++;
      umRecBuf->umHdr->numSduProcessed = count + 1;
   }
   rbCb->m.um.umUl.sn = sn;
#ifdef MSPD_MLOG_NEW
{
   kwUmmReassEnd = GetTIMETICK();
   MLogTask(PID_RLC_REASSEMBLY , RESOURCE_LARM, kwUmmReassStart, kwUmmReassEnd);
   MLogAddVariables(1,(unsigned int *)&liCount,kwUmmReassEnd);
}
#endif

   RETVALUE(ROK);
}
/**
 *
 * @brief
 *
 *        Handler to create the header and complete a PDU.
 *
 * @b Description:
 *
 *        This function is used to create the header of a PDU and concatenate  it
 *        with the data part of the PDU.
 *
 *  @param[in] rbCb RB control block
 *  @param[in] pdu  PDU
 *  @param[in] fi   Framing Info field
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 kwUmmCreatePdu
(
KwRbCb *rbCb,           /* Rb Control Block */
Buffer **pdu,           /* PDU */
U8     fi               /* Framing Info */
)
#else
PRIVATE S16 kwUmmCreatePdu(rbCb,pdu,fi)
KwRbCb *rbCb;           /* Rb Control Block */
Buffer **pdu;           /* PDU */
U8     fi;              /* Framing Info */
#endif
{
   U16  sn;             /*  Sequence Number */
   S16 liCount;         /*  LI count */
   S16 oldliCount;         /*  LI count */
   U16 *li;             /*  LI Array */
   U8   e;              /* Extension Bit */
   S16 count;           /* Loop Counter */
   U16  modVal;          /* Modulation element */

   TRC2(kwUmmCreatePdu)
#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF| KW_DBGMASK_DL),
                  (_kwPBuf," kwUmmCreatePdu(rbCb,pdu,fi) \n"));
#endif
   sn = rbCb->m.um.umDl.vtUs;
   liCount = rbCb->m.um.umDl.numLi;
   oldliCount = rbCb->m.um.umDl.numLi;
   li = (rbCb->m.um.umDl.li);
   e = 0;


   if ( liCount % 2 == 1 )
   {
      U16 liData;
      liData = li[liCount -1] << 4;
      KW_ADD_PRE_MSG(*pdu,liData,2);
      liCount--;
      e = 1;
   }
   for ( count = liCount - 1;count >= 0;count = count -2 )
   {
      U32 liData = li[count] | ( e << 11);

      li[count-1] |= (1<<11);
      liData |= (li[count-1] << 12);

      KW_ADD_PRE_MSG(*pdu,liData,3);
      e = 1;
   }
   if ( rbCb->m.um.umDl.snLen == 1)
   {
      U8 hdrData = 0;
      hdrData |= sn;

      if ( liCount > 0)
         hdrData |= 0x20;
      hdrData |= (fi<<6);

      KW_ADD_PRE_MSG(*pdu,hdrData,1);
   }
   else if ( rbCb->m.um.umDl.snLen == 2)
   {
      U16 hdrData = 0;
      hdrData |= sn;
      /* kw002.201 Checking for proper liCnt to set first e-bit */
      if ( oldliCount  > 0 )
         hdrData |= 0x0400;
      hdrData |= (fi<<11);

      KW_ADD_PRE_MSG(*pdu,hdrData,2);
   }
   modVal = KW_POWER(2, (5 * rbCb->m.um.umDl.snLen));
   sn = (sn + 1) % modVal;
   rbCb->m.um.umDl.vtUs = sn;
   RETVALUE(ROK);
}

/**
 *
 * @brief
 *
 *        Handler to estimate the header size of the RLC SDUs present in the SDU queue.
 *
 * @b Description:
 *
 *        This function is used to update the estimated header size variable in RB.
 *        This function is called when a SDU is queued and when a PDU is formed and
 *        sent to the lower layer.
 *
 *  @param[in] rbCb     RB control block
 *  @param[in] type     Type of Operation( Addition or Deletion of SDU)
 *  @param[in] sduCount Number of SDUs which are Added/Deleted
 *  @return  S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 kwUmmEstHdrSz
(
KwRbCb *rbCb,           /* RB Control Block */
U8     type,            /* Type of Operation */
U16 sduCount            /* SDU count */
)
#else
PUBLIC S16 kwUmmEstHdrSz(rbCb,type,sduCount)
KwRbCb *rbCb;           /* RB Control Block */
U8     type;            /* Type of Operation */
U16 sduCount;           /* SDU count */
#endif
{
   /* kw005.201 removed warning */
   S32 *estHdrSz;
   S32  totalSduCount;

   TRC3(kwUmmEstHdrSz)
#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF| KW_DBGMASK_DL),
                  (_kwPBuf," kwUmmEstHdrSz(rbCb) \n"));
#endif
   estHdrSz = &(rbCb->m.um.umDl.estHdrSz);
   totalSduCount = rbCb->m.um.umDl.sduQ.count;

   if ( type == KW_DEL_SDU )
   {
      if (totalSduCount == 0)
      {
         *estHdrSz = 0;
      }
      else
      {
         *estHdrSz -= sduCount * 2;
      }
   }
   else if ( type == KW_ADD_SDU)
   {
      if (totalSduCount == 0)
         *estHdrSz = 3;
      else
         *estHdrSz += 2;
   }

   if ( *estHdrSz < 0)
      *estHdrSz = 0;

   RETVALUE(ROK);
}

/**
 *
 * @brief
 *
 *        Handler to extract the header from a PDU.
 *
 * @b Description:
 *
 *        This function is used to extract the header of a PDU and store it along with the
 *        PDU buffer.The sequence number,framing info and LIs are extracted by this
 *        function.
 *
 *  @param[in]  rbCb    RB control block
 *  @param[in]  pdu     PDU
 *  @param[out] umHdr   UM Header to be extracted
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 kwUmmExtractHdr
(
KwRbCb *rbCb,                   /* RB Control Block */
Buffer *pdu,                    /* PDU */
KwUmHdr **umHdr                 /* Extracted UM Header */
)
#else
PUBLIC S16 kwUmmExtractHdr(rbCb,pdu,umHdr)
KwRbCb *rbCb;                   /* RB Control Block */
Buffer *pdu;                    /* PDU */
KwUmHdr **umHdr;                /* Extracted UM Header */
#endif
{
   U8 e;                        /* Extension Bit */
   Data *dst = NULLP;           /* Destination Buffer */
   Data li[2];                  /* Data */
   U16 numLi;                   /* Number of LIs */
   KwUmHdr *hdr;                /* UM Header Pointer */
   S32     totalSz;             /* Sum of LIs */
   MsgLen  pduSz;               /* PDU size */
   MsgLen  tPduSz;              /* PDU size backup */
   S16      ret;                /* Return Value */

   TRC3(kwUmmExtractHdr)
#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF| KW_DBGMASK_UL),
                  (_kwPBuf," kwUmmExtractHdr(rbCb,pdu,umHdr) \n"));
#endif
   numLi = 0;
   e = 0;
   dst = li;

   KW_ALLOC (*umHdr, sizeof(KwUmHdr));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( *umHdr == NULLP )
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW216, (ErrVal) RFAILED,
            "KwUmHdr: umHdr allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   hdr = *umHdr;
   SFndLenMsg(pdu,&pduSz);
   tPduSz = pduSz;
   totalSz = 0;

   if ( rbCb->m.um.umUl.snLen == 1)
   {
      ret = SRemPreMsg(dst,pdu);
      /* kw006.201, ccpu00120574, fixed the warning */
      if (ret != ROK)
      {
         KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," SRemPreMsg Failed \n"));
         RETVALUE(RFAILED);
      }
      pduSz--;
      hdr->sn = (dst[0]) & 0x1F;
      hdr->fi = (dst[0]) >> 6;
      e       = (dst[0]>>5) & 0x01;
   }
   else if ( rbCb->m.um.umUl.snLen == 2)
   {
      ret = SRemPreMsgMult(dst,2,pdu);
      /* kw006.201, ccpu00120574, fixed the warning */
      if (ret != ROK)
      {
         KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," SRemPreMsgMult Failed \n"));
         RETVALUE(RFAILED);
      }
      pduSz -= 2;

      /* kw005.201 R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0082      *
       * Removed the "if" condition for checking the reserved field *
       * Added mask 0x03 for extracting the FI field.		    */

      hdr->fi = ( (dst[0] ) >> 3) & 0x03;
      e       = ( (dst[0] ) >> 2) & 0x01;
      hdr->sn = (  dst[0] & 0x03) << 8;
      hdr->sn  |= dst[1];
   }
   while (e && numLi < KW_MAX_LI && pdu)
   {
      ret = SRemPreMsgMult(dst,2,pdu);
      /* kw006.201, ccpu00120574, fixed the warning */
      if (ret != ROK)
      {
         KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," SRemPreMsgMult Failed \n"));
         RETVALUE(RFAILED);
      }
      hdr->li[numLi] = ((dst[0]) & 0x7F) << 4;
      hdr->li[numLi] |= dst[1] >> 4;
      if (hdr->li[numLi] == 0)
      {
         KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," Received LI with 0 \n"));
         RETVALUE(RFAILED);
      }
      totalSz += hdr->li[numLi];
      if ( pduSz <=  totalSz )
      {
         KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," Corrupted PDU  \n"));
         RETVALUE(RFAILED); /* the situation where in the PDU size is something that does not match with the size in LIs*/
      }
      numLi++;
      pduSz -= 2;

      e = ((dst[0]) & 0x80) >> 7;

      if ( e && numLi < KW_MAX_LI)
      {
         U8 tmp = ((dst[1]) & 0x08) >> 3;
         hdr->li[numLi] = ( dst[1] & 0x07) << 8;
         if (e)
         {
            ret = SRemPreMsg(dst,pdu);
            /* kw006.201, ccpu00120574, fixed the warning */
            if (ret != ROK)
            {
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," SRemPreMsg Failed \n"));
               RETVALUE(RFAILED);
            }
            hdr->li[numLi] |= ( dst[0] );    /* The first byte lies in the first 8 bits.We want them in the last 8 bits */
            if (hdr->li[numLi] == 0)
            {
               KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                     (_kwPBuf," Received 0 for LI \n"));
               RETVALUE(RFAILED);
            }
            totalSz += hdr->li[numLi];
            pduSz--;
            numLi++;

            if ( pduSz <  totalSz )
            {
               RETVALUE(RFAILED); /* the situation where in the PDU size is something that does not match with the size in LIs*/
            }
         }
         e = tmp;
      }
      else
      {
         /* PDU was constructed with LIs that exceeded KW_MAX_LI */
         if (e)
            RETVALUE(RFAILED);
      }
   }
   /*ccpu00121490: Fix for dropping the entire RLC PDU if numLi exceeds*
    *KW_MAX_LI.                                                        */
   if((numLi >= KW_MAX_LI) && pdu && e)
   {
#ifdef MSPD
      MSPD_DBG("Unhandled numLi %u\n", numLi);
#endif
   	#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," liCount exceeded the maximum count \n"));
    #endif
      RETVALUE(RFAILED);
   }
   else if (numLi <= KW_MAX_LI)
   {
      hdr->numLi = numLi;
   }

   if ((totalSz +  rbCb->m.um.umUl.snLen) > tPduSz)
   {
      /* kw006.201, ccpu00120574, fixed the warning */
      KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," Corrupted PDU \n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/**
 *
 * @brief
 *
 *        Handler to discard a SDU.
 *
 * @b Description:
 *
 *        This function is used to discard a SDU after receiving
 *        the Discard Request from the upper layer.The SDU is
 *        discarded if it is present and is not mapped to any PDU yet.
 *
 *  @param[in] rbCb   RB control block
 *  @param[in] sduId  SDU ID of the SDU to be discarded
 *  @return  S16
 *      -# ROK
 *
 */

#ifdef ANSI
PUBLIC S16 kwUmmDiscSdu
(
KwRbCb    *rbCb,                /* RB Control Block */
U32       sduId                 /* SDU ID */
)
#else
PUBLIC S16 kwUmmDiscSdu(rbCb,sduId)
KwRbCb    *rbCb;                /* RB Control Block */
U32       sduId;                /* SDU ID */
#endif
{
   CmLList *tmpNode;            /* Temporary Node in SDU queue */
   KwSdu   *sdu;                /* SDU */
   CmLListCp *sduQ;             /* SDU Queue Pointer */

   TRC2(kwUmmDiscSdu)
#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                  (_kwPBuf,"  kwUmmDiscSdu(rbCb,sduId) \n"));
#endif
   sduQ = &(rbCb->m.um.umDl.sduQ);

   CM_LLIST_FIRST_NODE(sduQ,tmpNode);

   while (tmpNode)
   {
      sdu = (KwSdu *)tmpNode->node;

      if (sdu->mode.um.sduId == sduId && sdu->mode.um.isSegmented == FALSE)
      {
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         if(rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_DISC)
         {
            kwUtlSndDiscSduCfm(rbCb, sdu->mode.um.sduId);
         }
#endif /*  LTE_L2_MEAS */
         //KW_RMV_SDU_UM(sduQ,sdu);
         kwUmmDlRemSdu(rbCb, sdu);
         kwCb.genSts.numSduDisc++;         
         tmpNode = NULLP;
         break;
      }
      CM_LLIST_NEXT_NODE(sduQ,tmpNode);
   }
   if (tmpNode == NULLP)
   {
      /* SDU which has to be deleted if not found in the queue or is segmented and can't be removed */
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                  (_kwPBuf,"SDU not found for discard\n"));
#endif
   }
   RETVALUE(ROK);
}
#ifdef ANSI
PUBLIC S16 kwUmmReOrdTmrExp
(
KwRbCb  *rbCb                         /* RB Control Block */
)
#else
PUBLIC S16 kwUmmReOrdTmrExp(rbCb)
KwRbCb  *rbCb;                        /* RB Control Block */
#endif
{
   KwSn vrUh;                         /* vrUh */
   KwSn *vrUr;                        /* vrUr */
   KwSn prevVrUr;                     /* prevVrUr */
   KwUmRecBuf **recBuf;               /* Um Reception Buffer */
   KwUmUl *umUl;                      /* UM Uplink structure */
   U16 umReOrdWinSz;            /* UM ReOrder Window Size */
   U16 umRcpWinSz;              /* Reception Window Size */

   TRC3(kwUmmReOrdTmrExp)

   umUl = &(rbCb->m.um.umUl);
   vrUh = umUl->vrUh;
   vrUr = &umUl->vrUr;
   prevVrUr = *vrUr;
   recBuf = umUl->recBuf;
   umReOrdWinSz= rbCb->m.um.umUl.umWinSz;
   umRcpWinSz = umReOrdWinSz << 1;
   MSPD_DBG("Reordering timer expired vrUx=%u vrUr=%u vrUh=%u\n",
	          umUl->vrUx, *vrUr, vrUh);
   /* kw006.201 ccpu00118629, Replacing large macros with functions */
   kwUmmFindNextVrUr(umUl, umUl->vrUx, vrUr);
   /*KW_UM_FIND_NEXT_VRUR(umUl,umUl->vrUx, *vrUr);*/
   while ( KW_UM_GET_VALUE(prevVrUr,vrUh,umReOrdWinSz,umRcpWinSz) < KW_UM_GET_VALUE(*vrUr,vrUh,umReOrdWinSz,umRcpWinSz) )
   {
      if ( recBuf[prevVrUr] != NULLP)
      {
#ifndef KW_BG_UL_PROC
         kwUmmReAssembleSdus(rbCb, recBuf[prevVrUr]);
         KW_FREE(recBuf[prevVrUr]->umHdr, sizeof(KwUmHdr));
         KW_FREE(recBuf[prevVrUr], sizeof(KwUmRecBuf));
#else /* KW_BG_UL_PROC */
         kwUmmUlProcessInSeqPdu(rbCb, prevVrUr);
#endif /* KW_BG_UL_PROC */
      }
      prevVrUr = (prevVrUr + 1)%umRcpWinSz;
   }
   /* Add the RB to the list of RBs which have PDUs to be reassembled */
   /* Post a message for reassembly which happens in background       */
   kwUtlUlAddRbToReassemblyLst(rbCb);
   //MSPD_DBG("Pdu received, about to post self message\n");
   kwUtlPostSelfMsgUl(2);
   if ( KW_UM_GET_VALUE(vrUh,vrUh,umReOrdWinSz,umRcpWinSz) > KW_UM_GET_VALUE(*vrUr,vrUh,umReOrdWinSz,umRcpWinSz) )
   {
      kwStartTmr((PTR)rbCb,KW_EVT_UMUL_REORD_TMR);
      umUl->vrUx = vrUh;
   }

   RETVALUE(ROK);
}
#ifdef KW_BG_UL_PROC
#ifdef ANSI
PRIVATE Void kwUmmUlGetPduFrmReassemblyLst
(
KwRbCb                  *rbCb,
KwUmRecBuf              **pdu
)
#else
PRIVATE Void kwUmmUlGetPduFrmReassemblyLst(rbCb, pdu)
KwRbCb                  *rbCb;
KwUmRecBuf              **pdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.ulLock);
   {
      if (UMUL.reassemblyLst.first == NULLP)
      {
         *pdu = NULLP;
      }
      else
      {
         *pdu = (KwUmRecBuf *)(UMUL.reassemblyLst.first->node);
         cmLListDelFrm(&UMUL.reassemblyLst, &((*pdu)->reassemblyLnk));
      }
   }
   KW_RDWR_UNLOCK(&kwCb.ulLock);
   RETVOID;
}
#ifdef ANSI
PRIVATE Void kwUmmUlGetPduForReassembly
(
KwRbCb                  *rbCb,
KwUmRecBuf              **pdu
)
#else
PRIVATE Void kwUmmUlGetPduForReassembly(rbCb, pdu)
KwRbCb                  *rbCb;
KwUmRecBuf              **pdu;
#endif
{
   if(rbCb->rbOp == 0)
   {	   
      if (UMUL.partPdu == NULLP)
      {
         kwUmmUlGetPduFrmReassemblyLst(rbCb, pdu);
      }
      else
      {
         /* A partial PDU is waiting to be reassmbled */
         *pdu = UMUL.partPdu;
         UMUL.partPdu = NULLP;
      }
   }
   else
   {
      *pdu = NULLP;	   
   }
   RETVOID;
}

#ifdef ANSI
PUBLIC Void kwUmmProcessSelfMsgDelete
(
KwRbCb                  *rbCb
)
#else
PUBLIC Void kwUmmProcessSelfMsgDelete(rbCb)
KwRbCb                  *rbCb;
#endif
{
#ifdef MSPD_MLOG_NEW
   U32            startTick, endTick;
#endif
   MSPD_DBG("[%d][%d][%d]Process Delete request\n", KW_UEID, KW_RBTYPE,
             KW_RBID);

#ifdef MSPD_MLOG_NEW
   startTick = GetTIMETICK();
#endif
   MSPD_DBG("COUNT SduQ %d \n", UMDL.sduQ.count);
  
   if (rbCb->dir & KW_DIR_DL)
   { 
      /* Empty SDUQ                                                            */
      kwUtlEmptySduQ(rbCb, &UMDL.sduQ);
   }

   /* Delete/PDUs in reassemblyLst and reception buffer                     */
   if (rbCb->dir & KW_DIR_UL)
   { 
      kwUmmUlProcessDeletionRb(rbCb);
   }

   MSPD_DBG("COUNT SduQ %d ULRls %d\n",UMDL.sduQ.count,tempUlRlsPduCnt);
   
   /* Reset back the rbOp bits for delete                                   */
   rbCb->rbOp = 0;


   kwUmmRbFree(rbCb);
#ifdef MSPD_MLOG_NEW
   endTick = GetTIMETICK();
   //MLogTask(PID_RLC_UM_RB_DELETE, RESOURCE_LARM, startTick, endTick);
#endif
}

#ifdef ANSI
PRIVATE Void kwUmmUlRlsPdu
(
KwRbCb                  *rbCb,
KwUmRecBuf              *pdu
)
#else
PRIVATE Void kwUmmUlRlsPdu(rbCb, pdu)
KwRbCb                  *rbCb;
KwUmRecBuf              *pdu;
#endif
{
   if (pdu->pdu != NULLP)
   {
      KW_FREE_BUF(pdu->pdu);
   }
   KW_FREE(pdu, sizeof(KwUmRecBuf));
}

#ifdef ANSI
PRIVATE Void kwUmmUlProcessDeletionRb
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwUmmUlProcessDeletionRb(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KwUmRecBuf           *pduRecBuf;
   CmLListCp            *pduList;

   TRC2(kwUmmUlProcessDeletionRb);
#if 0
   pduList = &(AMUL.reassemblyLst);
   while (pduList->first)
   {
      pduRecBuf = (KwAmRecBuf*)(pduList->first->node);
      kwAmmUlRemPduFrmRlsLst(rbCb, pduRecBuf);
      kwAmmUlRlsPdu(rbCb, pduRecBuf);
      tempUlRlsPduCnt++;
   }
#endif
   kwUmmUlGetPduFrmReassemblyLst(rbCb, &pduRecBuf);
   while(pduRecBuf)
   {
      kwUmmUlRlsPdu(rbCb, pduRecBuf);
      tempUlRlsPduCnt++;
      kwUmmUlGetPduFrmReassemblyLst(rbCb, &pduRecBuf);
   }
   RETVOID;
}

/**
 * @brief Handler to release RB when a delete is received
 *
 * @b Description:
 *
 * @param [in]    rlcId    -  RLC Identity (rbId, lcId, ueId)
 * @param [in]    rbCb     -  Radio Bearer Control Block
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwUmmDeleteRb
(
KwRbCb            *rbCb
)
#else
PUBLIC S16 kwUmmDeleteRb(rbCb)
KwRbCb            *rbCb;
#endif
{
   TRC2(kwUmmDeleteRb)

   rbCb->rbOp |= 0x10;

   /* Here we mark the UE for deletion. Given that UL and DL may be on    */
   /* two different cores, RB is handed over from one direction to the    */
   /* other in a sequential manner to avoid any contention                */

   pjDlmRemRbFrmReestLst(&rbCb->rb->pjRbCb);
   if(rbCb->dir & KW_DIR_UL)
   {
      kwUtlUlRemRbFrmReassemblyLst(rbCb);
      kwUtlPostSelfMsgUl(2);
   }

   /* Added to reset all BO for this RB since it is to be deleted         */
   kwUmmResetAllBo(rbCb);
   rbCb->m.um.umDl.estHdrSz = 0;
   kwUtlSndDStaRsp(rbCb, 0, rbCb->m.um.umDl.estHdrSz,FALSE);

   kwUmmAddRbToDeleteLst(rbCb);
   kwUtlPostSelfMsgDelete();

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC Void kwUmmResetAllBo
(
KwRbCb                 *rbCb
)
#else
PUBLIC Void kwUmmResetAllBo(rbCb)
KwRbCb                 *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.sduLock);
   {
      UMDL.bo = 0;
   }
   KW_RDWR_UNLOCK(&kwCb.sduLock);
}

#ifdef ANSI
PRIVATE Void kwUmmAddRbToDeleteLst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwUmmAddRbToDeleteLst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.deleteRbLstLock);
   if (rbCb->deleteRbLnk.node == NULLP)
   {
      cmLListAdd2Tail(&(kwCb.deleteRbLst), &(rbCb->deleteRbLnk));
      rbCb->deleteRbLnk.node = (PTR)rbCb;
      //MSPD_DBG("[%d][%d][%d] RbCb added to DeleteLst\n", KW_UEID, KW_RBTYPE,
      //          KW_RBID);
   }
   KW_RDWR_UNLOCK(&kwCb.deleteRbLstLock);
   RETVOID;
}




#ifdef ANSI
PRIVATE Void kwUmmRbFree
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwUmmRbFree(rbCb)
KwRbCb                  *rbCb;
#endif
{
   //MSPD_DBG("Value of rbOp %d\n", rbCb->rbOp);
   if (rbCb->rbOp == 0)
   {
      /* If both RLC and PDCP are done, RB is released */
      kwDbmDelRbCb(rbCb->rlcId.rbId, rbCb->rlcId.rbType, &rbCb->rb, PJ_CFG_RLC);
   }
   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to process the self posted message by RLC for 
 *        reassembling of SDUs in UM
 *
 * @b Description: 
 *
 *        This function gets the UM pdu list and traverses through this list
 *        and calls a function to reassemble the SDUs
 *            
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwUmmUlProcessReassemblyRb
(
KwRbCb                  *rbCb,
U16                     *numSdus
)
#else
PRIVATE Void kwUmmUlProcessReassemblyRb(rbCb, numSdus)
KwRbCb                  *rbCb;
U16                     *numSdus;
#endif
{
   KwUmRecBuf           *pduRecBuf;
   Bool                 isPduProcessd;

   TRC2(kwUmmUlProcessReassemblyRb);
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwUmmUlProcessReassemblyRb(rbCb, numSdus)\n"));

   kwUmmUlGetPduForReassembly(rbCb, &pduRecBuf);
   while (pduRecBuf != NULLP)
   {
      kwUmmReAssembleSdus(rbCb, pduRecBuf, numSdus, &isPduProcessd);
      if (pduRecBuf->pdu == NULLP)
      {
         /* PDU is fully processed, so reset the part PDU and release */
         /* the PDU                                                   */
         UMUL.partPdu = NULLP;
         KW_FREE(pduRecBuf->umHdr, sizeof(KwUmHdr));
         KW_FREE(pduRecBuf, sizeof(KwUmRecBuf));
      }
      else
      {
         UMUL.partPdu = pduRecBuf;
      }
      if ((*numSdus) >= KW_AM_UM_MAX_UL_SDUS)
      {
         RETVOID;
      }
      kwUmmUlGetPduForReassembly(rbCb, &pduRecBuf);
   }
   RETVOID;
}
/**
 *
 * @brief
 *
 *        Handler to process the self posted message by RLC for
 *        reassembling of SDUs in UM
 *
 * @b Description:
 *
 *        This function gets the UM pdu list and traverses through this list
 *        and calls a function to reassemble the SDUs
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PUBLIC Void kwUmmProcessSelfMsgUl
(
KwRbCb *rbCb,
U16    *numSdus
)
#else
PUBLIC Void kwUmmProcessSelfMsgUl(rbCb, numSdus)
KwRbCb *rbCb;
U16    *numSdus;
#endif
{
   TRC2(kwUmmProcessSelfMsgUl);
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwUmmProcessSelfMsgUl(rbCb, numPdus)\n"));

   if(rbCb->rbOp & 0x10)
   {
      
   }
   else if(rbCb->rbOp & 0x01)
   {
     
   }
   else
   {
      kwUmmUlProcessReassemblyRb(rbCb, numSdus);
      if ((*numSdus >= KW_AM_UM_MAX_UL_SDUS) && (0 == rbCb->rbOp))
      {
         kwUtlUlAddRbToReassemblyLst(rbCb);
      }
   }
}
#ifdef ANSI
PRIVATE Void kwUmmUlRlsRecBuf
(
KwUmRecBuf        *recBuf
)
#else
PRIVATE Void kwUmmUlRlsRecBuf(recBuf)
KwUmRecBuf        *recBuf;
#endif
{
   TRC2(kwUmmUlRlsRecBuf)

   KW_FREE_BUF(recBuf->pdu);
   KW_FREE(recBuf, sizeof(KwUmRecBuf));
   RETVOID;
}
/**
 * @brief Handler to reset all the state variables and others
 *
 * @b Description:
 *        This function is called when reestablishment is triggered
 *
 * @param [in]    rbCb     -  Radio Bearer Control Block
 *
 * @return  Void
 */
#ifdef ANSI
PRIVATE Void kwUmmResetStateVars
(
KwRbCb                       *rbCb
)
#else
PRIVATE Void kwUmmResetStateVars(rbCb)
KwRbCb                       *rbCb;
#endif
{
   UMUL.sn = 0;
   UMUL.vrUr = 0;
   UMUL.vrUh = 0;
   UMUL.vrUx = 0;

   UMDL.bo = 0;
   UMDL.estHdrSz = 0;
   UMDL.vtUs = 0;
   UMDL.numLi = 0;
}

/**
 * @brief Handler to Stop all running timers on the RB
 *
 * @b Description
 *        This function is called when re-establishment is triggered
 *
 * @param   [in]    rbCb     -  Radio Bearer Control Block
 * @return  Void
 */
#ifdef ANSI
PUBLIC Void kwUmmStopRbTmrs
(
KwRbCb                       *rbCb
)
#else
PUBLIC Void kwUmmStopRbTmrs(rbCb)
KwRbCb                       *rbCb;
#endif
{
   U32                       tmrRunning;

   tmrRunning = kwChkTmr((PTR)rbCb, KW_EVT_UMUL_REORD_TMR);
   if (tmrRunning)
   {
      kwStopTmr((PTR)rbCb, KW_EVT_UMUL_REORD_TMR);
   }
}
/***************************************************************************/
/***********Functions related to reestablishment starts*********************/
/***************************************************************************/
#ifdef ANSI
PRIVATE Void kwUmmReasmbleOnReEst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwUmmReasmbleOnReEst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KwUmUl               *umUl;
   CmLListCp            *pduList;
   KwUmRecBuf           *pduRecBuf;
   KwSn                 mVrUh;
   KwSn                 mSn;
   KwSn                 sn;
   U16                  modVal = KW_POWER(2, (5 * UMUL.snLen));  
   TRC2(kwUmmReasmbleOnReEst);
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwUmmReasmbleOnReEst(rbCb, numSdus)\n"));

   /* Delete from reassemblyLst                                              */
   umUl    = &(rbCb->m.um.umUl);
   pduList = &umUl->reassemblyLst;
   while(pduList->first)
   {
      pduRecBuf = (KwUmRecBuf*) pduList->first->node;
      KW_FREE_BUF(pduRecBuf->pdu);
      cmLListDelFrm(pduList, pduList->first);
      KW_FREE(pduRecBuf, sizeof(KwUmRecBuf));
   }
   /* ccpu00127333:Freeing of UL partial sdu                                 */
   KW_FREE_BUF(umUl->partialSdu);

   /* Delete from reception buffer                                           */
   sn = UMUL.vrUr;
   MODUMR(UMUL.vrUh, mVrUh, UMUL.vrUr,UMUL.snLen);
   MODUMR(sn, mSn, UMUL.vrUr,UMUL.snLen);  
   MSPD_DBG("<CRFIX>:mSn %d,mVrUh %d UMUL.vrUh %d sn %d \n",mSn,mVrUh,UMUL.vrUh,sn);
   while(mSn < mVrUh)
   {
      if(umUl->recBuf[sn])
      {
         pduRecBuf = umUl->recBuf[sn];
         kwUmmUlRlsRecBuf(pduRecBuf);
         umUl->recBuf[sn] = NULLP;
      }
      sn = (sn + 1) % modVal;
      MODUMR(sn, mSn, umUl->vrUr,UMUL.snLen);
      MSPD_DBG("<CRFIX>:mSn %d, umUl->vrUr %d sn %d \n",mSn,UMUL.vrUr,sn);
   }

   rbCb->rbOp &= 0xfd;
   kwUmmResetStateVars(rbCb);
   //kwUmmStopRbTmrs(rbCb);
}

#ifdef ANSI
PUBLIC Void kwUmmProcessSelfMsgReest
(
KwRbCb                   *rbCb
)
#else
PUBLIC Void kwUmmProcessSelfMsgReest(rbCb)
KwRbCb                   *rbCb;
#endif
{
#ifdef MSPD_MLOG_NEW
   U32            startTick, endTick;
#endif
   /* If RbCb is marked for deletion, do not process for re establishment.  */
   /* Instead post a self message for deletion and return.                  */
   if(rbCb->rbOp & 0x10)
   {
      MSPD_DBG("[%d][%d]Dont process reestablishment request since delete\
      flag is set. Post self message for UE delete\n", KW_UEID, KW_RBID);
      kwUmmAddRbToDeleteLst(rbCb);
      kwUtlPostSelfMsgDelete();
      RETVOID;
   }
   MSPD_DBG("[%d][%d]Process Reestablishment request Core Id %d\n", KW_UEID, KW_RBID,MxGetCpuID());
#ifdef MSPD_MLOG_NEW
   startTick = GetTIMETICK();
#endif
   /* Empty SDUQ                                                            */
   kwUtlEmptySduQ(rbCb, &UMDL.sduQ);

   /* Delete/Re-assemble PDUs in reassemblyLst and reception buffer         */
   kwUmmReasmbleOnReEst(rbCb);

   /* Reset back the rbOp bits for re-establishment                         */
   rbCb->rbOp &= 0xfe;

   kwUtlSendRbReEstCfm(rbCb);
#ifdef MSPD_MLOG_NEW
   endTick = GetTIMETICK();
   MLogTask(PID_RLC_UM_REEST, RESOURCE_LARM, startTick, endTick);
#endif
}

/**
 *
 * @brief
 *
 *        Handler to process the re-establishment request received from the upper layer.
 *
 * @b Description:
 *
 *        This function does the following functions :
 *         - If direction of the RB is downlink : Remove all the SDUs in the SDU queue.
 *         - If direction of the RB is uplink   : Call kwUmmReAssembleSdus() for each PDU with SN < VR(UH)
 *
 *  @param[in] rbCb  RB control block
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 kwUmmReEstablish
(
CmLteRlcId  rlcId,
KwRbCb      *rbCb
)
#else
PUBLIC S16 kwUmmReEstablish(rlcId, rbCb)
CmLteRlcId  rlcId;
KwRbCb      *rbCb;
#endif
{
   TRC2(kwUmmReEstablish)
   KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF| KW_DBGMASK_DL |  KW_DBGMASK_UL),
                  (_kwPBuf," kwUmmReEstablish(rbCb) \n"));

   if((rbCb->rlcId.rbType == CM_LTE_SRB) && (rbCb->rlcId.rbId == 1))
   {
#ifdef MSPD
      ysMsUmmReEstReqRcvd++;
#endif
   }
   if(rbCb->rbOp != 0)
   {
#ifdef MSPD
      ysMsUmmErrReEstReqRcvd++;
#endif
      RETVALUE(ROK);
   }
   /* Add this RB to the list of RBs that are processed with lower priority */
   /* Post the self message to RLC only when the 1st Rb is added to the     */
   /* Rb list. Pass the core Id as 2                                        */
   rbCb->rbOp |= 0x01;
   MSPD_DBG("[%d][%d][%d]Reestablishment request received\n", KW_UEID, 
             KW_RBTYPE, KW_RBID);

   kwUtlUlRemRbFrmReassemblyLst(rbCb);
   kwUtlPostSelfMsgUl(2);

   kwUmmStopRbTmrs(rbCb);

   kwUtlAddRbToReestablishLst(rbCb);
   kwUtlPostSelfMsgReest();

   RETVALUE(ROK);
}
/**
 *
 * @brief
 *
 *        Handler to raise a alarm to Layer Manager.
 *
 * @b Description:
 *
 *        This function is called from kwUmmProcessPdus, kwUmmReEstablish,
 *        kwUmmReOrdTmrExp and kwUtlFreeRbCb.
 *        It raises the alarm in case the number of UM UL PDUs in the PDU list
 *        goes beyond 500.
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PUBLIC Void kwUmmChkReasmblyLstSz
(
KwRbCb *rbCb
)
#else
PUBLIC Void kwUmmChkReasmblyLstSz(rbCb)
KwRbCb *rbCb;
#endif
{
   TRC2(kwUmmChkReasmblyLstSz);

   /*If the size of toBeReasmbldUmPduLst is more than 500*
    *then send an alarm to layer manager.                */
   if(UMUL.reassemblyLst.count > 500)
   {
      KWDBGP((KW_DBGMASK_UM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
             (_kwPBuf, "kwUmmChkReasmblyLstSz:Alarm is raised\n"));
#ifdef LTE_L2_MEAS
      kwLmmSendAlarm(LCM_CATEGORY_INTERFACE, LKW_EVENT_UM_UL_PDU,
            LKW_CAUSE_REASMBLDUMLST_FULL, 0,
            rbCb->rlcId.ueId, rbCb->qci);
#else /* LTE_L2_MEAS */
      kwLmmSendAlarm(LCM_CATEGORY_INTERFACE, LKW_EVENT_UM_UL_PDU,
            LKW_CAUSE_REASMBLDUMLST_FULL, 0,
            rbCb->rlcId.ueId);
#endif/* LTE_L2_MEAS */
   }
} /* end of kwUmmChkReasmblyLstSz */
#endif /* KW_BG_UL_PROC */

/********************************************************************30**
         End of file:     gp_umm.c@@/main/2 - Fri Nov 13 14:14:07 2009
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
/main/1      ---       vp   1. initial release.
/main/2      ---       nm   1. LTERLC Release 2.1
/main/3      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/4      kw003.201 vp   1. Fixes for re-segmentation, segement offset
                               value and optimization.
/main/5      kw005.201 rd   1. R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0082
               		       Removed the reserved field validation for UM
                               10 bit SN header in function kwUmmExtractHdr
                            2. L2 Meas Code added for DL/UL ip throughput.
                       ap   3. Added missing condition in kwUmmProcessPdus
                            4. Updated for missing statistics calculation.
                            5. Added L2 Measurement feature.
                       rd   6. Bug related to dl ip throughput during
                               segmentation is fixed.
/main/6     kw006.201  rd   1. Splitted the function kwUmmCalDlIpThrPut into 4
                               functions:kwUmmMeasDlIpBrstCont, kwUmmMeasDlIpBrstStart,
                               kwUmmMeasDlIpBrstReset, kwUmmMeasDlIpBrstComplt.
                            2. Extracted the L2M related code from function
                               kwUmmProcessPdus to kwUmmCalUlIpThrPut.
                       ap   3. Removed 64bit compilation warning.
                       rd   4. CR: ccpu00118629, replaced the macros
                               KW_UM_CHK_DUP_PDU and KW_UM_FIND_NEXT_VRUR with
                               kwUmmCheckDupPdu and kwUmmFindNextVrUr functions.
                            5. Added code for ccpu00117290 in various function.
                               Added a new function kwUmmProcessSelfMsgUl.
                            6. ccpu00117290:Added alarm code kwUmmChkReasmblyLstSz.
                            7. ccpu00120341 fix for discarding the partial sdu
                               if sn is out of reordering window.
                            8. ccpu00120574, fixed the warning.
                            9. Self post of message to RLC is done when new
                               RB is added to RbList and when PduList is not
                               empty during self message processing.
                           10. Changes made in kwUmmProcessSelfMsgUl and 
                               kwUmmReAssembleSdus for controlling the number of
                               SDUs to be reassembled per self message.
                           11. ccpu00121490: Fix for dropping the entire PDU
                               when number of LIs exceeds the KW_MAX_LI.
                           12. Fix done for controlling number of SDUs to be
                               sent to PDCP in UL data path.
*********************************************************************91*/
