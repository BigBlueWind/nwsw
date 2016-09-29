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
  
        Name:    RLC - AM module file
    
        Type:    C source file
  
        Desc:    Source code for Acknowledged Mode Module functions such as, 

                 Transmission of data/control PDUs 
                 Retransmission (Feedback in terms of status)
                 Polling
                 Assemble SDUs
                 Reception - reordering
                 Duplicate detection for byte segments
                 Reassemble SDUs
                  
        File:    gp_amm.c

        Sid:      gp_amm.c@@/main/2 - Fri Nov 13 14:13:34 2009
 
        Prg:     nm
  
*********************************************************************21*/


/* header include files (.h) */
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
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */
#ifdef KW_PDCP
#include "lpj.h"           /* LPJ defines */
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#endif /* KW_PDCP */

#include "kw.h"            /* RLC defines */

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
#include "lpj.x"           /* LPJ defines */
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#endif /* KW_PDCP */

#include "kw.x"
#include "ss_diag.h"        /* Common log file */

#define KW_AMM_NO_PDCP_CFM      0
#define KW_AMM_SEND_PDCP_CFM    1

/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */
typedef struct kwCntrlInfo
{
   U16  val;
   U8   len;
   U16  idx;
   U8   emtBits;
   U16  e1Idx;
   U16  e2Idx;   
   U8   e1eb;
}KwCntrlInfo;

typedef struct kwHdrInfo
{
   U16  val;
   U8   len;
   U8   eb;
   U8   *hdr;
   U16  idx;
   U8   pEb;
   U8   pLen;
}KwHdrInfo;

typedef struct kwExtHdr
{
   U16 val;
   U16 len;
   U8  hdr;
   U8  pLen;
}KwExtHdr;

#define KW_AM_RMV_HDR(_rbCb, _retx) do { \
  if ((_retx)->yetToConst == FALSE) \
  {\
      Buffer    *_tSeg; \
     SSegMsg ((_retx)->seg, (_retx)->hdrSz, &_tSeg); \
      KW_FREE_BUF((_retx)->seg);\
     (_retx)->seg = _tSeg; \
  }\
  (_rbCb)->m.am.amDl.estHdrSz -= retx->hdrSz;\
} while(0)

#define KW_AM_SND_DSTA_RSP() do { \
   if(AMDL.isRbstalled)           \
   {                              \
      kwUtlSndDStaRsp(rbCb, 0, 0, 0); \
   }                              \
   else                           \
   {                              \
      U32        _bo;             \
      KW_UPD_AMBO(AMDL, _bo);     \
      kwUtlSndDStaRsp(rbCb, _bo, AMDL.estHdrSz, AMDL.cntrlBo ? TRUE : FALSE); \
   }                              \
} while(0)

#define KW_AM_MIN_HDR_SZ  (KW_MAC_HDR_SZ2 + KW_AM_PDU_FIXED_HDRSZ)

#define KW_ADD_BUF_TO_PDU(_pdu, _buf) do {\
   if (_pdu == NULLP) {_pdu = _buf; _buf = NULLP;}\
   else {SCatMsg(_pdu, _buf, M1M2); SPutMsg(_buf); _buf = NULLP;}\
} while(0)

/* private function declarations */

PRIVATE S16  kwAmmPrepStatusPdu ARGS ((KwRbCb *rbCb, KwDatReq *kwDatReq));
PRIVATE S16  kwResegRetxPdus ARGS ((KwRbCb *rbCb, KwDatReq *kwDatReq));
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
PRIVATE Void  kwAmmCalDlIpThrPut ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo, MsgLen pduSz));
PRIVATE Void  kwAmmMeasDlIpBrstCont ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo));
PRIVATE Void  kwAmmMeasDlIpBrstStart ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo));
PRIVATE Void  kwAmmMeasDlIpBrstReset ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo));
PRIVATE Void  kwAmmMeasDlIpBrstComplt ARGS ((KwRbCb *rbCb, MsgLen dataVol, S32 initialBo));
PRIVATE Void  kwAmmCalUlIpThrPut ARGS ((KwRbCb *rbCb, Buffer *pdu, KwL2MeasUlThrpTimInfo *ulTimeInfo, Bool insideLoop));
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */
PRIVATE S16  kwAmmDlAssembleSdus ARGS ((KwRbCb *rbCb, KwDatReq *kwDatReq));
PRIVATE Bool kwChkandSetPoll ARGS ((KwRbCb *rbCb, Bool newPdu, MsgLen bufSz));
PRIVATE Bool kwCreatePdu ARGS ((KwRbCb *rbCb, KwAmHdr *amHdr, Buffer *pdu));
PRIVATE S16  kwExtractHdr ARGS ((KwRbCb *rbCb, Buffer *pdu, KwAmHdr *amHdr, 
                                 U8 *fByte));
PRIVATE S16  kwAmmDlProcessStatusPdu ARGS ((KwRbCb *rbCb, Buffer *pdu, U8 *fByte));
PRIVATE Bool kwPlcPduInRecBuf ARGS ((Buffer *pdu, KwRbCb *rbCb, 
                                 KwAmHdr *amHdr));
PRIVATE Void kwTriggerStatus ARGS ((KwRbCb *rbCb, KwSn sn, Bool discFlg));
PRIVATE Void kwUpdCntrlBo ARGS ((KwAmUl *amUl));
PRIVATE S16 kwReassembleSdus ARGS ((KwRbCb *rbCb, KwAmRecBuf **recBuf, U16 *numSdu));
PRIVATE Void kwAmmProcPdu ARGS ((KwRbCb *rbCb, KwAmHdr *amHdr, Buffer *pdu, KwAmRecBuf **recBuf, U16 *numSdu));
PRIVATE Void kwAmmProcSeg ARGS ((KwRbCb *rbCb, KwAmHdr *amHdr, Buffer *pdu, KwSeg *seg, U16 *numSdu));
PRIVATE Void kwAmmReasmbleOnReEst ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmEstHdrSz ARGS ((KwAmDl *amDl, U8 type));
PRIVATE S16  kwAmmSndStaInd ARGS ((KwRbCb *rbCb, KwRetx *retx));
PRIVATE S16  kwAmmSndDatCfm ARGS ((KwRbCb *rbCb, U32 sduId));
PRIVATE S16  kwGetNxtRetx ARGS ((KwAmDl *amDl, KwRetx **retx));
PRIVATE Void kwAmmUpdExpByteSeg ARGS ((KwAmUl *amUl, KwSeg* newSeg));
PRIVATE Void kwFormCntrlBits ARGS ((KwCntrlInfo *cntrlInfo, U16 *staPdu, 
                                    KwDatReq *kwDatReq, U32 *pduSzInBits,
                                    U16 *staPduSzInBits));
PRIVATE Void kwConstructAmHdr ARGS ((KwAmHdr *amHdr, Bool isSeg, U8 *hdr, 
                                     U16 *idx));
PRIVATE Void kwFormBits ARGS ((KwHdrInfo *hdrInfo));
PRIVATE Void kwExtractElmnt ARGS ((Buffer *pdu, KwExtHdr *hdrInfo));
PRIVATE Void kwUpdRemPduAck ARGS ((KwRbCb *rbCb, KwSn mAckSn, CmLList *retx));
PRIVATE S16 kwUpdFrmTxtoRetx ARGS ((KwAmDl *amDl, KwRetx **retx, KwSn sn));
PRIVATE Void kwAmmChkSduMap ARGS((KwRbCb *rbCb, KwSn sn, KwSduMap *sduLst, 
                                 U8 numSdu, U8 pdcpCfm));
PRIVATE Void kwAmmDlAddRetxPduToRlsLst ARGS ((KwRbCb *rbCb, KwRetx *retx));
PRIVATE Void kwAmmDlAddRbToPduRlsLst ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmDlRemRbFrmPduRlsLst ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmDlPostSelfMsg ARGS ((Void));
PRIVATE Void kwAmmDlRlsTxPdu ARGS((KwRbCb *rbCb, KwTx *tx, U8 pdcpCfm));
PRIVATE Void kwAmmDlGetTxPduForRelease ARGS((KwRbCb *rbCb, KwTx **pdu));
PRIVATE Void kwAmmDlRemRetxPdu ARGS((KwRbCb *rbCb, KwRetx *retx));
PRIVATE Void kwAmmDlRlsReTxPdu ARGS((KwRbCb *rbCb, KwRetx *retx, U8 pdcpCfm));
PRIVATE Void kwAmmDlGetRetxPduForRelease ARGS((KwRbCb *rbCb, KwRetx **retx));
PRIVATE Void kwAmmDlGetRbForPduRls ARGS((KwRbCb **rbCb));
PRIVATE Void kwAmmAddRbToDeleteLst ARGS((KwRbCb *rbCb));
PRIVATE Void kwAmmDlAddRbToStaPduLst ARGS((KwRbCb *rbCb));
PRIVATE Void kwAmmDlRemRbFrmStaPduLst ARGS((KwRbCb *rbCb));
PRIVATE Void kwAmmRemRbFrmReestablishLst ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmDlMarkForReestablish ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmUlMarkForReestablish ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmUlProcessDeletionRb ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmRbFree ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmDlMarkForDeletion ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmUlMarkForDeletion ARGS ((KwRbCb *rbCb));
PRIVATE Void kwAmmDlAddSdu ARGS ((KwRbCb *rbCb, KwSdu *sdu));
PRIVATE Void kwAmmSetReestInProgress ARGS ((KwUeCb  *ueCb));
PRIVATE Void kwAmmUlProcessInSeqPdu ARGS ((KwRbCb *rbCb, U16 sn));
PRIVATE Void kwAmmDlProcessStaPdus ARGS ((Void));
PRIVATE Void kwAmmUlProcessReassemblyRb ARGS ((KwRbCb *rbCb, U16 *numSdus));
PRIVATE Void kwAmmUpdateBo ARGS ((KwRbCb *rbCb, MsgLen sduSz));

PRIVATE Void kwAmmAddRetxBo(KwAmDl   *amDl,MsgLen   size);
PRIVATE Void kwAmmAddEstHdrSize(KwAmDl   *amDl,MsgLen   size);
PRIVATE Void kwAmmDecEstHdrSize(KwAmDl   *amDl,MsgLen   size);

 
#ifdef MSPD
extern U32 ysMsAmmDlPollReTx;
EXTERN U32 ysMsAmmDlRetxs[10];
EXTERN U32 ysMsNumPartRlcNacks, ysMsNumMaxReTx;
EXTERN U32 ysMsNumCtrlPdus, ysMsNumRlcNacks, ysMsNumErrCtrlPdus;
EXTERN U32 ysMsAmmReEstReqRcvd, ysMsAmmErrReEstReqRcvd;
EXTERN U32 ysMsAmmNumToBeRlsTxLstCount, ysMsAmmNumToBeRlsReTxLstCount;
EXTERN U32 ysMsAmmNumStaPduRbLstCount, ysMsAmmNumPduRlsRbLstCount, ysMsAmmNumReestablishRbLstCount, ysMsAmmNumDeleteRbLstCount;
EXTERN U32 ysMsAmmStaPduCounter, ysMsAmmNumAllBo, ysMsAmmNewDataBo;
EXTERN U32 ysMsAmmUlNumPduDiscarded, ysMsAmmUlMaxLiCntRcvd, ysMsAmmNumSduQCount, ysMsAmmNumRetxLstCnt, ysMsAmmPjNumEntries, ysMsAmmPjDatQ;
#endif
/** @file gp_amm.c
@brief RLC Acknowledged Mode Module
*****************************************************************************

  AM Module contains the following funcitons:

  -  kwAmmQSdu 
  -  kwAmmProcessSdus
     -  kwAmmPrepStatusPdu
     -  kwResegRetxPdus
     -  kwAmmDlAssembleSdus
     -  kwChkandSetPoll
  -  kwAmmProcessPdus
     -  kwAmmDlProcessStatusPdu
     -  kwTriggerStatus
     -  kwReassembleSdus
   
*******************************************************************************/
/** @addtogroup ammode */
/*@{*/

#ifdef ANSI
PRIVATE Void kwAmmDlAddSdu
(
KwRbCb                  *rbCb,
KwSdu                   *sdu
)
#else
PRIVATE Void kwAmmDlAddSdu(rbCb, sdu)
KwRbCb                  *rbCb;
KwSdu                   *sdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.sduLock);
   {
      cmLListAdd2Tail(&AMDL.sduQ, &sdu->lstEnt);
      sdu->lstEnt.node = (PTR)sdu;

      /* Update BO and estimate header size for the current BO */ 
      AMDL.bo = AMDL.bo + sdu->sduSz;
   }
   KW_RDWR_UNLOCK(&kwCb.sduLock);
   kwAmmAddEstHdrSize(&AMDL, 2);
}



#ifdef ANSI
PRIVATE Bool kwAmmDlTxSduPrsnt
(
KwRbCb                  *rbCb
)
#else
PRIVATE Bool kwAmmDlTxSduPrsnt(rbCb)
KwRbCb                  *rbCb;
#endif
{
   RETVALUE((AMDL.sduQ.first != NULLP) || (AMDL.partialSdu != NULLP));
}

#ifdef ANSI
PRIVATE KwSdu* kwAmmDlGetTxSdu
(
KwRbCb                  *rbCb
)
#else
PRIVATE KwSdu* kwAmmDlGetTxSdu(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KwSdu                *sdu = NULLP;

   /* There is no partial SDU in the queue. Get if from queue */
   if (AMDL.partialSdu != NULLP)
   {
      sdu = AMDL.partialSdu;
      AMDL.partialSdu = NULLP;
   }
   else
   {
      KW_RDWR_LOCK(&kwCb.sduLock);
      {
         if (AMDL.sduQ.first != NULLP)
         {
            sdu = (KwSdu *)(AMDL.sduQ.first->node);
            cmLListDelFrm(&AMDL.sduQ, AMDL.sduQ.first);
         }
      }
      KW_RDWR_UNLOCK(&kwCb.sduLock);
   }
   RETVALUE(sdu);
}

#ifdef ANSI
PRIVATE Void kwAmmUpdateBo
(
KwRbCb                  *rbCb,
MsgLen                  sduSz
)
#else
PRIVATE Void kwAmmUpdateBo(rbCb, sduSz)
KwRbCb                  *rbCb;
MsgLen                  sduSz;
#endif
{
   KW_RDWR_LOCK(&kwCb.sduLock);
   {
      AMDL.bo -= sduSz;
   }
   KW_RDWR_UNLOCK(&kwCb.sduLock);
}

/**
 * @brief Handler to queue the SDUs received from PDCP
 *       
 *
 * @b Description
 *  This function is invoked by UIM to queue the SDU received from PDCP in the 
 *  SDU queue of the corresponding RbCb. It also updates the BO and report the 
 *  same to MAC.
 *  -  Allocate memory for and assign received buffer to the SDU
 *  -  Add SDU in the sduQ of KwAmDl 
 *  -  Calculate bo with the buffer received
 *  -  Accumulate bo with retransmission bo and control pdu's bo if available
 *  -  Estimate the header size for the bo; Fill in StaRspInfo and send it 
 *     to MAC
 *
 *  @param[in] rbCb RB control block 
 *  @param[in] mBuf Sdu to be queued 
 *
 *  @return Void
 *      -# RETVOID
*/
#ifdef ANSI
PUBLIC S16 kwAmmQSdu
(
KwRbCb                  *rbCb,
Buffer                  *mBuf,
U32                     sduId
)
#else
PUBLIC S16 kwAmmQSdu(rbCb, mBuf, sduId)
KwRbCb                  *rbCb;
Buffer                  *mBuf;
U32                     sduId;
#endif
{
   KwSdu                *sdu;

   TRC2(kwAmmQSdu)
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmQSdu(rbCb, mBuf, sduId) \n"));

   KW_ALLOC (sdu, sizeof(KwSdu)); 
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (sdu == NULLP)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW001, (ErrVal)0,
            "kwAmmQSdu: Allocation failed for sdu \n");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   /* Assign values to sdu */
   SFndLenMsg(mBuf, &sdu->sduSz);
   sdu->mBuf = mBuf; 
   sdu->mode.am.sduId = sduId;
   sdu->actSz = sdu->sduSz;
#ifdef MSPD
   {
      extern U32 dlrate_kwu;
      dlrate_kwu += sdu->sduSz;
   }
#endif
   /* Add sdu to the sdu list */
   kwAmmDlAddSdu(rbCb, sdu);
   //MSPD_DBG("[%d][%d] Received SDU, count %d\n", KW_UEID, KW_RBID, AMDL.sduQ.count);

#ifdef LTE_L2_MEAS
   /* Update numActUe if it is not active */
   if ((rbCb->rbL2Cb.measOn & LKW_L2MEAS_ACT_UE) &&
       (rbCb->ueCb->numActRb[rbCb->qci] == 0))
   {
      rbCb->ueCb->numActRb[rbCb->qci]++;
      kwCb.kwL2Cb.numActUe[rbCb->qci]++;
   }
#endif

   /* Changed for RLC_AM_PERF,this thread was consuming memory cycles */
#ifdef PWAV
MSPD_DBG("Queueing the SDU:%d, UE:%d, RB Id :%d\n", sduId, rbCb->rlcId.ueId, rbCb->rlcId.rbId);
#endif
   if (rbCb->reestInProgress == FALSE)
   {
      KW_AM_SND_DSTA_RSP();
   }
#ifndef PRODUCTION
   else
   {
      MSPD_DBG_RLC("[%d][%d]BO dropped due to reestablishment\n",
                   KW_UEID, KW_RBID);
   }
#endif

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to form the PDUs with the size indicated by MAC
 *
 * @b Description
 *        This function is invoked by UTL with the PDU size indicated by 
 *        MAC (after eliminating MAC header size). It assembles control 
 *        Info / data (New SDUs / Retx PDUs), check if polling needs to be 
 *        set for the data PDU and returns PDU(s) and updated BO with 
 *        estimated header size to be sent to MAC.
 *
 *        - Check if the control BO is available and call kwAmmPrepStatusPdu 
 *          to assemble control Information
 *        - Check if the pdu size is available to form PDUs from retransmission 
 *          buffer and call kwResegRetxPdus
 *        - Check if the pdu size is available and assemble SDUs from sduQ 
 *          if exist, using kwAmmDlAssembleSdus
 *        - PDU Info and bo are filled in and then sent to MAC from the 
 *          utility function
 *
 * @param[in]  rbCb     RB control block 
 * @param[in]  kwdatReq DatReq to be sent to MAC 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 kwAmmProcessSdus
(
KwRbCb                   *rbCb,
KwDatReq                 *kwDatReq
)
#else
PUBLIC S16 kwAmmProcessSdus(rbCb, kwDatReq)
KwRbCb                   *rbCb;
KwDatReq                 *kwDatReq;
#endif    
{
   S16                   ret;
   extern U32            ysPktsDropped;

#ifdef LTE_L2_MEAS
   Bool dataWasPrsnt = FALSE;
#endif

   TRC2(kwAmmProcessSdus)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmProcessSdus(rbCb, kwDatReq) \n"));

#ifdef LTE_L2_MEAS
   dataWasPrsnt = (AMDL.cntrlBo || AMDL.retxBo  || AMDL.bo)?TRUE:FALSE;
#endif /*  LTE_L2_MEAS */
   /* Assemble control information */ 
   if (AMDL.cntrlBo != 0)
   {
      kwDatReq->boRep.staPduPrsnt = TRUE;
      kwAmmPrepStatusPdu (rbCb, kwDatReq);
      AMDL.cntrlBo = AMUL.cntrlBo= 0;
   }   

   /* For estHdrSz */
   AMDL.numSeg    = 0;
   AMDL.numSduPdu = 0;

   /* Retransmit PDUs /portions of PDUs available in retxLst */
   if ( (kwDatReq->pduSz > 0) && (AMDL.nxtRetx != NULLP) )
   {
      KW_RDWR_LOCK(&kwCb.dlLock);
      ret = kwResegRetxPdus (rbCb, kwDatReq);
      KW_RDWR_UNLOCK(&kwCb.dlLock);
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         KWLOGERROR(ERRCLS_DEBUG, EKW002, (ErrVal) ret,
                  "kwAmmProcessSdus: Resegmentation failed.\n");
#endif /* ERRCLASS & ERRCLS_DEBUG */
         MSPD_ERR("[%d][%d][%d] Reseg failed\n", KW_UEID, KW_RBTYPE, KW_RBID);
      }
   }

   /* Assemble SDUs to form new PDUs */ 
   if ((kwDatReq->pduSz > 0) && (kwAmmDlTxSduPrsnt(rbCb)))
   {
      ret = kwAmmDlAssembleSdus(rbCb, kwDatReq); 
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         KWLOGERROR(ERRCLS_DEBUG, EKW003, (ErrVal) ret,
                  "kwAmmProcessSdus: AssembleSdus failed.\n");
#endif /* ERRCLASS & ERRCLS_DEBUG */
         MSPD_ERR("[%d][%d][%d] AssembleSdus failed\n", KW_UEID, KW_RBTYPE, KW_RBID);
      }
   }
  
   /* Accumulate bo */
   KW_UPD_AMBO(AMDL, kwDatReq->boRep.bo);
#ifdef MSPD
   if(rbCb->rlcId.rbType == 1)
   {
      ysMsAmmNumAllBo = kwDatReq->boRep.bo;
   }
#endif
#ifdef LTE_L2_MEAS
   if((rbCb->rbL2Cb.measOn & LKW_L2MEAS_ACT_UE) &&
      (dataWasPrsnt) &&
      !((AMDL.cntrlBo) ||
        (AMDL.retxBo)  ||
        (AMDL.bo)))
   {
      if(--(rbCb->ueCb->numActRb[rbCb->qci]) == 0)
      {
         kwCb.kwL2Cb.numActUe[rbCb->qci]--;
      }
   }
#endif /* LTE_L2_MEAS */
   /* Estimate hdr size for boReport */
   kwAmmEstHdrSz (&AMDL, KW_DEL_SDU);
   kwDatReq->boRep.estHdrSz = AMDL.estHdrSz;

   RETVALUE(ROK);
} /* kwAmmProcessSdus */

/**
 *
 * @brief Private handler to construct control PDU
 *
 * @b Description
 *        Its a private function called by kwProcessSdus, to create the 
 *        control PDU if it needs to be sent. With the PDU size indicated 
 *        by MAC, it constructs the Status PDU.
 *
 *        - Eliminate fixed header size and ACK_SN with E1 from the pduSz
 *        - Update ACK_SN with the sn of vrMr, to indicate the next not 
 *          received PDU  
 *        - Scan through PDUs in the reception buffer, starting from vrR 
 *          till vrMs and include NACK_SN for missing PDUs and NACK_SN 
 *          along with soStart and soEnd for missing byte segments.
 *
 * @param[in]  rbCb     RB control block 
 * @param[in]  kwdatReq DatReq to be sent to MAC 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmPrepStatusPdu 
(
KwRbCb                  *rbCb,
KwDatReq                *kwDatReq
)
#else
PRIVATE S16 kwAmmPrepStatusPdu(rbCb, kwDatReq)
KwRbCb                  *rbCb;
KwDatReq                *kwDatReq;
#endif
{
   KwCntrlInfo          cntrlInfo;
   KwSn                 sn;                /* sequence number */
   KwSn                 mSn;               /* Mod val of sequence number */
   KwSn                 vrMs;              /* Mod val of vrMs */
   KwSn                 ack_sn;            /* Ack sequence number */
   Buffer               *mBuf = NULLP;     /* control pdu buffer */
   MsgLen               cntrlPduSz = 0;    /* control pdu size */
   KwSeg                *seg    = NULLP;   /* pdu segment */ 
   Bool                 partSeg  = FALSE;  /* Flag to indicate partial segment
                                              construction in cntrl PDU */
   U16                  staPdu[(KW_MAX_CNTRL_FIELDS + 1)/2];
   U8                   cntrlPdu[KW_MAX_CNTRL_FIELDS];
   U16                  idx     = 0;             /* Index for staPdu */
   U8                   emtBits = 0;             /* empty bits */
   U16                  e1      = 0;             /* E1 */
   U16                  e1Idx   = 0;             /* E1 Index */
   U16                  e2Idx   = 0;             /* E2 Index */
   U8                   e1eb    = 0;             /* empty bits after E1 */
   U16                  soStart = 0;             /* segment offset start */
   U16                  soEnd   = 0;             /* segment offset end */
   U16                  seqSo   = 0;             /* segmment offset */
   U16                  i       = 0;             /* Index for staPdu */
   U16                  j       = 0;             /* Index for controlPdu */
   KwSn                 tSn     = 0;
   U32                  pduSzInBits = 0;
   U16                  staPduSzInBits = 8 * KW_MAX_CNTRL_FIELDS;
   Bool                 notEnoughSpace = FALSE;

   TRC2(kwAmmPrepStatusPdu)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmPrepStatusPdu(rbCb, kwDatReq) \n"));

   KW_MEM_ZERO(&cntrlInfo, sizeof(KwCntrlInfo));

   pduSzInBits = 8 * kwDatReq->pduSz;

   /* Eliminate fixed hdr size (14bits including ACK_SN) */
   kwDatReq->pduSz -= KW_CNTRL_PDU_FIXED_HDRSZ;
   pduSzInBits -= 14;
   staPduSzInBits -= 14;
   
   /* Now eliminating MAC header size */ 
   kwDatReq->pduSz -= KW_MAC_HDR_SZ2;
   pduSzInBits -= 8 * KW_MAC_HDR_SZ2;
   staPduSzInBits -= 8 * KW_MAC_HDR_SZ2;

   /* Add ACK_SN */
   ack_sn = 0;
   staPdu[idx] = ack_sn;

   /* Empty bits left after ACK_SN -  2 bits */
   emtBits = 2;

   /* Add E1 to ACK_SN */
   KW_FILL_CNTRL_INFO(cntrlInfo, e1, KW_E1_LEN, idx, emtBits);
   kwFormCntrlBits(&cntrlInfo, staPdu, kwDatReq, &pduSzInBits, &staPduSzInBits);
   idx = cntrlInfo.idx;
   emtBits = cntrlInfo.emtBits;

   e1Idx = 0;
   e1eb = 0;

   sn = AMUL.vrR;
   MODAMR(sn, mSn, AMUL.vrR);
   MODAMR(AMUL.vrMs, vrMs, AMUL.vrR);

   /*ccpu00121123:Fix for stack corruption while generating large status pdu*/
   /*If pduSzInBits is less than 12, then it means there is no allocation left*
    *to accomodate anymore NACK SN. If staPduSzInBits is less than 12, then it*
    *means there is no space left in array to accomodate anymore NACK SN.     */
   while ((pduSzInBits >= KW_NACK_E1E2_LEN) && (staPduSzInBits >= KW_NACK_E1E2_LEN) && (mSn < vrMs ))
   {
      /*Initially partSeg and notEnoughSpace are set to FALSE*/
      partSeg = FALSE;
      notEnoughSpace = FALSE;

      /* For missing PDUs */
      if (AMUL.recBuf[sn] == NULLP)
      {
         /* Whole Nacks */
         tSn = sn; 

         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
               (_kwPBuf, "kwAmmPrepStatusPdu: Missing PDU's SN = %d\n", sn));

         /* set prev E1 */
         staPdu[e1Idx] = staPdu[e1Idx] | KW_POWER(2, (e1eb+1));
         
         /* Add NACK_SN, E1 and E2 to staPdu */

         /* For NACK_E1E2, lsh by 2 to append e1, e2 value */
         tSn = sn << 2;
         KW_FILL_CNTRL_INFO(cntrlInfo, tSn, KW_NACK_E1E2_LEN, idx, emtBits);  
         kwFormCntrlBits(&cntrlInfo, staPdu, kwDatReq, &pduSzInBits, &staPduSzInBits);
         KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e2Idx, e1eb, idx, emtBits);
         //MSPD_DBG_RLC("[%d][%d] Whole Nack sn %d\n", KW_UEID, KW_RBID, sn);
      }   
      else
      {
         if ((AMUL.recBuf[sn]->pdu == NULLP) && 
             (AMUL.recBuf[sn]->segLst.count > 0))
         {
            /* Segmented Nacks */
            /* Scan through the byte segments of PDU and add this sn 
               with soStart and soEnd info to staPdu */

            seqSo  = 0;
            KW_LLIST_FIRST_SEG(AMUL.recBuf[sn]->segLst, seg); 
            /*If pduSzInBits is less than 42, then it means there is no   *
             *allocation left to accomodate anymore segmented NACK SN. If *
             *staPduSzInBits is less than 42, then it means there is no   *
             *space left in array to accomodate anymore segmented NACK SN.*/
            while (seg != NULLP)
            {
               /*ccpu00121123:Fix for stack corruption while generating large*
                *status pdu.                                                 */
               /*If enough space is not there to put the NackSN, E1E2, SoStart*
                *and SoEnd, then break from the inner while loop.             */
               if( (pduSzInBits < KW_SEGNACK_E1E2_SO_LEN) || 
                   (staPduSzInBits < KW_SEGNACK_E1E2_SO_LEN) )
               {
                  notEnoughSpace = TRUE;
                  break;
               }
               /* For missing byte segments */
               if (seg->amHdr.so != seqSo)
               {
                  tSn = sn;
                  /* set prev E1 */
                  staPdu[e1Idx] = staPdu[e1Idx] | KW_POWER(2, (e1eb+1));

                  /* set E2 */
                  tSn = sn << 2;
                  tSn |= 1;  

                  /* Add NACK_SN, E1 and E2 to staPdu */
                  KW_FILL_CNTRL_INFO(cntrlInfo, tSn, KW_NACK_E1E2_LEN, idx, 
                                     emtBits);  
                  kwFormCntrlBits(&cntrlInfo, staPdu, kwDatReq, &pduSzInBits, &staPduSzInBits);
                  KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e2Idx, e1eb, idx, 
                                   emtBits); 

                  /* Add soStart and soEnd */
                  soStart = seqSo;
                  soEnd = seg->amHdr.so - 1; 
                  KW_FILL_CNTRL_INFO(cntrlInfo, soStart, KW_SO_LEN, idx, 
                                     emtBits);
                  kwFormCntrlBits(&cntrlInfo, staPdu, kwDatReq, &pduSzInBits, &staPduSzInBits);
                  KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e2Idx, e1eb, idx, 
                                   emtBits); 

                  KW_FILL_CNTRL_INFO(cntrlInfo, soEnd, KW_SO_LEN, idx, 
                                     emtBits);
                  kwFormCntrlBits(&cntrlInfo, staPdu, kwDatReq, &pduSzInBits, &staPduSzInBits);
                  KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e2Idx, e1eb, idx, 
                                   emtBits);
                  //MSPD_DBG("[%d][%d] Seg Nack %d soStart %d soEnd %d\n",
                  //         KW_UEID, KW_RBID, sn, soStart, soEnd);

                  /* Setting partSeg to TRUE since at least one segment for */
                  /* this SN is put in status pdu.                          */
                  partSeg = TRUE;
                  KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
                        (_kwPBuf, "kwAmmPrepStatusPdu: Missing byte segment's"
                         " SN, soStart and soEnd = %d, %d, %d \n", sn, soStart,
                         soEnd));
                  //MSPD_DBG_RLC("[%d][%d] Seg Nack SN, soStart, soEnd =" 
                  //              "%d, %d, %d \n", KW_UEID, KW_RBID, sn, soStart, soEnd);
               }
               seqSo = seg->soEnd + 1;
               KW_LLIST_NEXT_SEG(AMUL.recBuf[sn]->segLst, seg);
            } /* while ends */

            /*If notEnoughSpace is TRUE, then it means that not all missing *
             *segments could be added to status pdu. So, it should break the*
             *outer while loop even if there is space to accomodate the next*
             *whole NACK(if present).                                       */
            if (notEnoughSpace)
            {
               /*If partSeg is TRUE, it means that at least one missing seg*
                *is put inside status pdu, so sn is incremented because the*
                *logic to calculate ACKSN after this loop expects the next *
                *SN.                                                       */
               if(partSeg)
               { 
                  sn = (sn + 1) % 1024;
                  MODAMR(sn, mSn, AMUL.vrR);
               }
               break;
            }

            /* Check if the last segment is missing */
            KW_LLIST_LAST_SEG(AMUL.recBuf[sn]->segLst, seg);
            if ((seg != NULLP) && (seg->amHdr.lsf != 1))
            {
               /*If enough space is not there to put the NackSN, E1E2, SoStart*
                *and SoEnd, then break from the outer while loop.             */
               if( (pduSzInBits < KW_SEGNACK_E1E2_SO_LEN) && 
                   (staPduSzInBits < KW_SEGNACK_E1E2_SO_LEN) )
               {
                  if(partSeg)
                  { 
                     sn = (sn + 1) % 1024;
                     MODAMR(sn, mSn, AMUL.vrR);
                  }
                  break;
               }
               tSn = sn;
               /* set prev E1 */
               staPdu[e1Idx] = staPdu[e1Idx] | KW_POWER(2, (e1eb+1));

               /* set E2 */
               tSn = sn << 2;
               tSn |= 1;  

               /* Add NACK_SN, E1 and E2 to staPdu */
               KW_FILL_CNTRL_INFO(cntrlInfo, tSn, KW_NACK_E1E2_LEN, idx, 
                                  emtBits);  
               kwFormCntrlBits(&cntrlInfo, staPdu, kwDatReq, &pduSzInBits, &staPduSzInBits);
               KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e2Idx, e1eb, idx, emtBits);

               soStart = seqSo;
               /*soEnd = AMUL.recBuf[sn]->pduSz;*/
               soEnd = KW_ALL_BYTES_MISSING;
               KW_FILL_CNTRL_INFO(cntrlInfo, soStart, KW_SO_LEN, idx, emtBits);
               kwFormCntrlBits(&cntrlInfo, staPdu, kwDatReq, &pduSzInBits, &staPduSzInBits);
               KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e2Idx, e1eb, idx, emtBits);

               KW_FILL_CNTRL_INFO(cntrlInfo, soEnd, KW_SO_LEN, idx, emtBits);
               kwFormCntrlBits(&cntrlInfo, staPdu, kwDatReq, &pduSzInBits, &staPduSzInBits);
               KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e2Idx, e1eb, idx, emtBits);

               //MSPD_DBG_RLC("[%d][%d] Seg Nack SN, soStart, soEnd =" 
               //             "%d, %d, %d \n", KW_UEID, KW_RBID, sn, 
               //              soStart, soEnd);
               /*Assigning NULLP to seg since this last segment has already*
                *been utilized in constructing status pdu                  */
               seg = NULLP;
            }
         }
#ifndef PRODUCTION
#endif
      }
      sn = (sn + 1) % 1024;
      MODAMR(sn, mSn, AMUL.vrR);
   }

   /*Calculate ACK SN*/
   if(mSn == vrMs)
   {
      ack_sn = AMUL.vrMs;
   }
   else
   {
      while(mSn < vrMs )
      {
         if (AMUL.recBuf[sn] == NULLP)
         {
            /*The whole PDU is missing*/
            ack_sn = sn;
            break;
         }
         else if( (AMUL.recBuf[sn]->pdu == NULLP) &&
               (AMUL.recBuf[sn]->segLst.count > 0) )
         {
            /*One or more segments of the PDU are missing*/
            ack_sn = sn;
            break;
         }
         else
         {
            /*NOP: The PDU is completely received*/
         }
         sn = (sn + 1) % 1024;
         MODAMR(sn, mSn, AMUL.vrR);
      }
      if(mSn == vrMs)
      {
         ack_sn = AMUL.vrMs;
      }
   }
   idx = cntrlInfo.idx;
   emtBits = cntrlInfo.emtBits;

   //MSPD_DBG("[%d][%d] ACK SN %d\n", KW_UEID, KW_RBID, ack_sn);
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
         (_kwPBuf, "kwAmmPrepStatusPdu: ACK PDU's SN = %d\n", ack_sn));
   MSPD_DBG_RLC("[%d][%d] Send RLC Ack %d\n",  KW_UEID, KW_RBID, ack_sn);
   staPdu[0] = staPdu[0] | (ack_sn << 2);
   if (sn == AMUL.vrR)
   {
      idx = 0;
   }
 
   /* Add staPdu to mBuf */
   for (i = 0, j = 0; ((i <= idx) && (j < KW_MAX_CNTRL_FIELDS)); i++)
   {
      KW_COPY_STAPDU(i, j);
   }

   SGetMsg (kwCb.rguSap.pst.region, 
            kwCb.rguSap.pst.pool, &mBuf);

   cntrlPduSz = (emtBits >= 8) ? j-1 : j;
   kwDatReq->pduSz -= (cntrlPduSz - KW_CNTRL_PDU_FIXED_HDRSZ);

   SAddPstMsgMult (cntrlPdu, cntrlPduSz, mBuf); 
   /* Start staProhTmr */
   kwStartTmr((PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);
   kwChkTmr((PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);
   AMUL.staTrg = FALSE;

   /* Update number of pdus in pduInfo */
   kwDatReq->pduInfo.mBuf[kwDatReq->pduInfo.numPdu] = mBuf; 
   kwDatReq->pduInfo.numPdu++;

   kwCb.ckwSap.sts.statMsgs += kwDatReq->pduInfo.numPdu;
   RETVALUE(ROK);
}

/**
 *
 * @brief Private handler split a PDU/segment into two
 *
 * @b Description
 *        Its a private function called by kwResegRetxPdu to split a segment
 *        or a retransmit PDU into two segments splitting at the passed size.
 *        This function is called only for those PDUs that dont have any LIs.
 *
 * @param[in]  rbCb     RB control block 
 * @param[in]  kwdatReq DatReq to be sent to MAC 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PRIVATE S16 kwSplitPdu
(
KwRbCb           *rbCb,
KwRetx           *crnt,
KwRetx           *next,
U16              size
)
#else
PRIVATE S16 kwSplitPdu(rbCb, crnt, next, size)
KwRbCb           *rbCb;
KwRetx           *crnt;
KwRetx           *next;
U16              size;
#endif
{
   U8            fi;

   TRC2(kwSplitPdu)
   /* Set the SN for the new segment */
   next->amHdr.sn = crnt->amHdr.sn;

   /* Set the protocol specific fields appropriately */
   fi = crnt->amHdr.fi;
   crnt->amHdr.fi  = crnt->amHdr.fi | KW_FI_LAST_SEG;
   next->amHdr.fi  = fi | KW_FI_FIRST_SEG;
   crnt->amHdr.rf  = next->amHdr.rf = 1;
   next->amHdr.lsf = crnt->amHdr.lsf;
   crnt->amHdr.lsf = 0;
   crnt->amHdr.p   = 0;

   /* Update seg size */
   next->segSz = crnt->segSz - size;
   crnt->segSz = size;
   crnt->hdrSz = KW_AM_SEG_FIXED_HDRSZ;

   /* Set the SO fields appropriately */
   /* MS_FIX for DL stall */
   next->soEnd = crnt->soEnd;

   crnt->soEnd = crnt->amHdr.so + crnt->segSz - 1;
   next->amHdr.so = crnt->amHdr.so + crnt->segSz;

   /* This is a special case of single LI and hence set that to 1 */
   crnt->amHdr.numLi = next->amHdr.numLi = 1;

   /* This macro is called for No LI case - one SDU */
   /* Update the size of SDU in each node's sduMap  */
   crnt->sduMap[0].sdu->mode.am.numPdu++;
   next->sduMap[0].sdu = crnt->sduMap[0].sdu;
   crnt->sduMap[0].sduSz = crnt->segSz;
   next->sduMap[0].sduSz = next->segSz;
   next->sduMap[0].actSz = crnt->sduMap[0].actSz;
   next->sduMap[0].isSegmented = crnt->sduMap[0].isSegmented = TRUE;
   next->numSdu = crnt->numSdu;

   /* Segment the payload into two parts based on the size passed */
   SSegMsg(crnt->seg, size, &next->seg);
   next->retxCnt     = crnt->retxCnt;
   next->yetToConst  = TRUE;
   next->wtForAck    = FALSE;

   /* Compute the header size and update the BO appropriately */
   KW_AM_EXTN_HDRSZ(next->amHdr.numLi, next->hdrSz);
   next->hdrSz += KW_AM_SEG_FIXED_HDRSZ;
   AMDL.estHdrSz += next->hdrSz;

   /* Add the next to the retx list */
   AMDL.retxLst.crnt = &crnt->lstEnt;
   CM_LLIST_INS_AFT_CRNT(AMDL.retxLst, next); 
   AMDL.nxtRetx = next;
   RETVALUE(ROK);
}

/**
 *
 * @brief Private handler split a PDU/segment into two
 *
 * @b Description
 *        Its a private function called by kwResegRetxPdu to split a segment
 *        or a retransmit PDU into two segments splitting at the passed size.
 *        This function is called only for those PDUs that have valid LIs.
 *
 * @param[in]  rbCb     RB control block 
 * @param[in]  kwdatReq DatReq to be sent to MAC 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PRIVATE S16 kwSplitPduWithLi
(
KwRbCb           *rbCb,
KwRetx           *crnt,
KwRetx           *next,
U16              size
)
#else
PRIVATE S16 kwSplitPduWithLi(rbCb, crnt, next, size)
KwRbCb           *rbCb;
KwRetx           *crnt;
KwRetx           *next;
U16              size;
#endif
{
   U16           segSz = 0;
   U16           tSz   = size;
   U32           liCnt = 0;
   U32           liIdx; 
   KwAmHdr       *oldHdr;
   KwAmHdr       *newHdr;

   TRC2(kwSplitPduWithLi)
   /* Set the SN for the new segment */
   next->amHdr.sn = crnt->amHdr.sn;

   /* Figure out how many SDUs from the original may be included in */
   /* the current one based on the size passed                      */
   oldHdr = &crnt->amHdr;
   newHdr = &next->amHdr;
   while ( (liCnt < oldHdr->numLi) && (tSz >= oldHdr->li[liCnt]) )
   {
      KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
             (_kwPBuf, "kwSplitPduWithLi: numLi of in PDU = %d \n",
             crnt->amHdr.numLi));

      segSz      += oldHdr->li[liCnt];
      tSz        -= oldHdr->li[liCnt];

      /* Eliminate extension header size */
      if (tSz > 2)
      {
         tSz -= ((liCnt + 1) % 2) ? 2 : 1; 
      }
      else
      {
         /* Even though a few bits (1 or 2) may be left, they */
         /* are useless as we cant fit any into it            */
         tSz = 0;
      }
      liCnt++;
   }
   next->segSz = crnt->segSz - (segSz + tSz);
   crnt->segSz = (segSz + tSz);

   /* Transfer the remaining LIs to the subsequent segment */
   if (tSz && (liCnt < oldHdr->numLi))
   {
      newHdr->li[0] = oldHdr->li[liCnt] - tSz;
      oldHdr->li[liCnt] = tSz;
      crnt->sduMap[liCnt].isSegmented = TRUE;
      crnt->sduMap[liCnt].sdu->mode.am.numPdu++;
      next->sduMap[0] = crnt->sduMap[liCnt];
      next->sduMap[0].sduSz -= tSz;
      crnt->sduMap[liCnt].sduSz = tSz;
      newHdr->numLi++;
      next->numSdu++;
      crnt->numSdu = liCnt + 1;
      liCnt++;
   }

   liIdx = 0;
   while ((liCnt + liIdx) < oldHdr->numLi)
   {
      newHdr->li[newHdr->numLi] = oldHdr->li[liCnt + liIdx];
      next->sduMap[next->numSdu++] = crnt->sduMap[liCnt + liIdx];
      liIdx++; 
      newHdr->numLi++;
   }
   oldHdr->numLi  = liCnt;
   crnt->numSdu = liCnt;

   /* Manage header sizes and also updating the BO on RB CB */
   KW_AM_EXTN_HDRSZ(next->amHdr.numLi, next->hdrSz);
   next->hdrSz += KW_AM_SEG_FIXED_HDRSZ;
   AMDL.estHdrSz += next->hdrSz;

   /* Set the frame indicator field for both the segments */
   next->amHdr.fi  = crnt->amHdr.fi;
   if (tSz == 0)
   {
      /* We exactly finished an SDU with this segment. We want to set FI */
      /* appropriately. The current one should have last (un)set and the */
      /* the next one should have first (un)set                          */
      crnt->amHdr.fi &= 0x02;
      next->amHdr.fi &= 0x01;
   }
   else
   {
      /* We exactly finished an SDU with this segment */
      crnt->amHdr.fi |= 0x01;
      next->amHdr.fi |= 0x02;
      
   }
   crnt->amHdr.p   = 0;
   crnt->amHdr.rf  = next->amHdr.rf = 1;
   next->amHdr.lsf = crnt->amHdr.lsf;
   crnt->amHdr.lsf = 0;

   /* MS_FIX for DL stall */
   /* Set the SO fields appropriately */
   next->soEnd = crnt->soEnd;

   crnt->soEnd = crnt->amHdr.so + crnt->segSz - 1;
   next->amHdr.so = crnt->amHdr.so + crnt->segSz;

   /* Segment the payload into two parts based on the size passed */
   SSegMsg(crnt->seg, crnt->segSz, &next->seg);
   next->retxCnt     = crnt->retxCnt;
   next->yetToConst  = TRUE;
   next->wtForAck    = FALSE;

   /* Add the next to the retx list */
   AMDL.retxLst.crnt = &crnt->lstEnt;
   CM_LLIST_INS_AFT_CRNT(AMDL.retxLst, next); 
   AMDL.nxtRetx = next;
   RETVALUE(ROK);
}

/**
 *
 * @brief Private handler to retransmit PDUs or PDU segments
 *
 * @b Description
 *        Its a private function called by kwProcessSdus, to create the 
 *        PDUs / its segments from the retransmission buffer available in RbCb.
 *       
 *        - Eliminate the fixed header size and MAC header size while 
 *          forming PDUs/segments
 *        - While pdusize is available and retxBuf has data (pdu or portion 
 *          of pdu) to be sent, form the pdu as it is if it matches with the 
 *          pdusize else segment the PDUs/portion of PDUs
 *        - Call kwChkandSetPoll function to check and set the poll bit as 
 *          required 
 *        - Concatenate data and header info and fill pduInfo 
 *        - Update retxCnt and send indication to PDCP if it reaches maxRetx 
 *          threshold
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  kwdatReq DatReq to be sent to MAC 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PRIVATE S16 kwResegRetxPdus 
(
KwRbCb                 *rbCb,
KwDatReq               *kwDatReq
)
#else
PRIVATE S16 kwResegRetxPdus(rbCb, kwDatReq)
KwRbCb                 *rbCb;
KwDatReq               *kwDatReq;
#endif
{
   KwAmDl              *amDl   = NULLP;
   MsgLen              *pduSz  = 0;
   KwRetx              *retx   = NULLP;
   KwRetx              *tNode  = NULLP;
   U8                  hdr[KW_MAX_HDRSZ];
   U16                 idx     = 0; 
   U8                  pollBit = 0;
   Bool                segFlg  = TRUE;
   Buffer              *pdu = NULLP;

   TRC2(kwResegRetxPdus)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwResegRetxPdus(rbCb, kwDatReq) \n"));

   pduSz = &kwDatReq->pduSz;
   amDl  = &AMDL;
   retx  = amDl->nxtRetx;

   /* Scan through the retxLst for retransmission */
   /* MS_FIX for L-ARM freeze */
   while ((*pduSz > 0) && (amDl->nxtRetx != NULLP) && \
          (kwDatReq->pduInfo.numPdu < KW_MAX_PDU))
   {
      U16     tmpSz;
      retx = amDl->nxtRetx;
      /* Add header size to seg size to determine if the      */
      /* the segment can be completed within the allocation   */
      /* Eliminate MAC Header Size based on bites needed      */
      tmpSz = KW_MIN((retx->segSz + retx->hdrSz), *pduSz);
      (*pduSz) -= (tmpSz <= 128) ?  KW_MAC_HDR_SZ2 : KW_MAC_HDR_SZ3;

      /* We should have at least one more than basic header */
      if (*pduSz <= retx->hdrSz)
      {
         RETVALUE(ROK);
      }
      kwGetNxtRetx(amDl, &(amDl->nxtRetx));

      /* Send retx buf without segmentation */
      if (*pduSz >= (retx->segSz + retx->hdrSz))
      {
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                (_kwPBuf, "kwResegRetxPdus: Send retx buf without \
                 segmentation \n")); 

         if (retx->yetToConst)
         {
            /* Construct hdr with the available hdr values */ 
            kwConstructAmHdr(&retx->amHdr, segFlg, hdr, &idx);
            KW_ADD_PRE_MSG_AM(retx->seg, hdr, idx + 1);
            retx->yetToConst = FALSE;
         } 

         /* Check if poll bit needs to be set. Retx size does     */
         /* not affect the poll bit so it is being passed as zero */ 
         pollBit = kwChkandSetPoll(rbCb, FALSE, 0);
         KW_UPD_POLL_BIT(retx, pollBit);

         *pduSz        -= retx->hdrSz;
         *pduSz        -= retx->segSz;
         AMDL.estHdrSz -= retx->hdrSz;
      }
      else
      {
         /* Segment this pdu / portion of pdu. Insert this segment into */
         /* retxLst and update offset                                   */
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                (_kwPBuf, "kwResegRetxPdus: Segment retx buf \n"));

         /* Eliminate fixed header size if the pdu is segmented for the */
         /* first time                                                  */
         *pduSz -= KW_AM_SEG_FIXED_HDRSZ;
         if (*pduSz <= 0)
         {
            RETVALUE(ROK);
         }

         /* Allocate memory for tracking a new segment */
         KW_ALLOC(tNode, sizeof(KwRetx)); 
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (tNode == NULLP)
         {
            KWLOGERROR(ERRCLS_ADD_RES, EKW004, (ErrVal)0,
                  "kwResegRetxPdus: Allocation failed for tNode \n");
            RETVALUE(RFAILED);
         }
#endif /* ERRCLASS & ERRCLS_RES */

         /* Segment header and data */
         KW_AM_RMV_HDR(rbCb, retx);

         /* No LI, segment the data filed, update hdr Info */ 
         if ((retx->amHdr.numLi - 1) == 0)
         {
            KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                   (_kwPBuf, "kwResegRetxPdus: No LI for segmentation \n"));
            /* Split the payload and update other fields */
            MSPD_DBG("[%d][%d] Entering kwSplitPdu\n", KW_UEID, KW_RBID);
            kwSplitPdu(rbCb, retx, tNode, *pduSz); 
         }
         else
         {
            KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                   (_kwPBuf, "kwResegRetxPdus: LIs for segmentation \n"));
            /* With LI, segment the data field, update hdr Info */    
            /* Remove the header if it exists as it would be reconstructed */
            MSPD_DBG("[%d][%d] Entering kwSplitPduWithLi\n", KW_UEID, KW_RBID);
            kwSplitPduWithLi(rbCb, retx, tNode, *pduSz);
         }
         /* Construct hdr with the available hdr values */
         kwConstructAmHdr(&retx->amHdr, segFlg, hdr, &idx);
         KW_ADD_PRE_MSG_AM(retx->seg, hdr, idx + 1);
         retx->hdrSz = idx + 1;

         /* Poll bit need not be set for this seg, since its second  */
         /* half remains in retxLst                                  */
         KW_UPD_POLL_BIT(retx, FALSE);
         retx->yetToConst = FALSE;
         *pduSz = 0; 
      }

      kwCpyMsg(retx->seg, &pdu);

      /* Update pduInfo */
      kwDatReq->pduInfo.mBuf[kwDatReq->pduInfo.numPdu] = pdu;
      kwDatReq->pduInfo.numPdu++;
      kwCb.genSts.pdusRetx += 1;
      retx->soEnd = retx->amHdr.so + retx->segSz - 1;
      retx->wtForAck = TRUE;
      amDl->retxBo -= retx->segSz;
#ifdef MSPD
      //MSPD_DBG("[%d][%d] RETXing SN[%d], soEnd[%d], so[%d], rf[%d], lsf[%d]"
      //         " p[%d]\n",
      //         rbCb->rlcId.ueId, rbCb->rlcId.rbId, retx->amHdr.sn, retx->soEnd,
      //         retx->amHdr.so, retx->amHdr.rf, retx->amHdr.lsf, retx->amHdr.p);
#endif
   }

#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
         (_kwPBuf, "kwResegRetxPdus: retxBo after resegmentation = %ld \n", 
          amDl->retxBo));
#else
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
         (_kwPBuf, "kwResegRetxPdus: retxBo after resegmentation = %d \n",
          amDl->retxBo));
#endif

   amDl->numSduPdu = 0;
   amDl->numSeg    = 0;

   RETVALUE(ROK);
}

#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
/**
 *
 * @brief Private handler to measure DL IP throughput.
 *
 * @b Description
 *      It is called from kwAmmCalDlIpThrPut
 *      If the RLC bo is more than allocation from MAC, 
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to TRUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *      If the allocation from MAC is emptying the RLC buffer,
 *        Burst is said to be COMPLETED.
 *        T1 time stamp is calculated in kwUtlProcHarqInd
 *
 * @param[in]  rbCb      RB control block
 * @param[in]  dataVol   Data volume
 * @param[in]  initialBo BO of RLC entity before starting construction of PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmMeasDlIpBrstCont
(
KwRbCb       *rbCb,
MsgLen       dataVol,
S32          initialBo
)
#else
PRIVATE Void kwAmmMeasDlIpBrstCont(rbCb, dataVol, initialBo)
KwRbCb       *rbCb;
MsgLen       dataVol;
S32          initialBo;
#endif
{
   KwTbSnMap         *curTbSnMap = NULLP;   /*Holds the current TbSnMap*/
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;

   TRC2(kwAmmMeasDlIpBrstCont)
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmMeasDlIpBrstCont(rbCb, dataVol, initialBo)\n"));

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
} /* kwAmmMeasDlIpBrstCont */

/**
 *
 * @brief Private handler to measure DL IP throughput.
 *
 * @b Description
 *      It is called from kwAmmCalDlIpThrPut
 *      If the RLC bo is more than allocation from MAC, 
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to TRUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *      If the allocation from MAC is emptying the RLC buffer,
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to FALSE.
 *
 * @param[in]  rbCb      RB control block
 * @param[in]  dataVol   Data volume
 * @param[in]  initialBo BO of RLC entity before starting construction of PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmMeasDlIpBrstStart
(
KwRbCb       *rbCb,
MsgLen       dataVol,
S32          initialBo
)
#else
PRIVATE Void kwAmmMeasDlIpBrstStart(rbCb, dataVol, initialBo)
KwRbCb       *rbCb;
MsgLen       dataVol;
S32          initialBo;
#endif
{
   KwTbSnMap         *curTbSnMap = NULLP;   /*Holds the current TbSnMap*/
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;

   TRC2(kwAmmMeasDlIpBrstStart)
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmMeasDlIpBrstStart(rbCb, dataVol, initialBo)\n"));

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
         l2MeasIpThruput->prevTbSnMap->snSduMap[l2MeasIpThruput->
                          prevTbSnMap->numSn - 1].isBurstSplitted = FALSE;
      }
      else if(l2MeasIpThruput->count == 0)
      {
         l2MeasIpThruput->prevTbSnMap = NULLP;
      }
      else
      {
#ifndef ALIGN_64BIT
               KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_ERROR | KW_DBGMASK_DL),
               (_kwPBuf, "kwAmmMeasDlIpBrstStart: count can be either 1 or 0.\
                L2Mcount = %d \n", l2MeasIpThruput->count));
#else
               KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_ERROR | KW_DBGMASK_DL),
               (_kwPBuf, "kwAmmMeasDlIpBrstStart: count can be either 1 or 0.\
                L2Mcount = %d \n", l2MeasIpThruput->count));
#endif
      }

      /*Change state back to RESET*/
      kwUtlResetKwRbL2Meas(rbCb);
   }
} /* kwAmmMeasDlIpBrstStart */

/**
 *
 * @brief Private handler to measure DL IP throughput.
 *
 * @b Description
 *      It is called from kwAmmCalDlIpThrPut
 *      If the RLC bo is more than allocation from MAC, 
 *        Variable "isBurstSplitted" flag in KwTbSnMap set to TRUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *        T2 time stamp is taken.
 *
 * @param[in]  rbCb      RB control block
 * @param[in]  dataVol   Data volume
 * @param[in]  initialBo BO of RLC entity before starting construction of PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmMeasDlIpBrstReset
(
KwRbCb       *rbCb,
MsgLen       dataVol,
S32          initialBo
)
#else
PRIVATE Void kwAmmMeasDlIpBrstReset(rbCb, dataVol, initialBo)
KwRbCb       *rbCb;
MsgLen       dataVol;
S32          initialBo;
#endif
{
   KwTbSnMap         *curTbSnMap = NULLP;   /*Holds the current TbSnMap*/
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;
   EpcTime           timeSt;

   TRC2(kwAmmMeasDlIpBrstReset)
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmMeasDlIpBrstReset(rbCb, dataVol, initialBo)\n"));

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
} /* kwAmmMeasDlIpBrstReset */

/**
 *
 * @brief Private handler to measure DL IP throughput.
 *
 * @b Description
 *      It is called from kwAmmCalDlIpThrPut
 *      If the RLC bo is more than allocation from MAC and 
 *      count is more than 0,
 *        State is changed to KW_DL_IPTHRU_BURST_CONTINUE.
 *        Variable "count" is incremented.
 *        Volume of data for this PDU is stored.
 *
 * @param[in]  rbCb      RB control block
 * @param[in]  dataVol   Data volume
 * @param[in]  initialBo BO of RLC entity before starting construction of PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmMeasDlIpBrstComplt
(
KwRbCb       *rbCb,
MsgLen       dataVol,
S32          initialBo
)
#else
PRIVATE Void kwAmmMeasDlIpBrstComplt(rbCb, dataVol, initialBo)
KwRbCb       *rbCb;
MsgLen       dataVol;
S32          initialBo;
#endif
{
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;

   TRC2(kwAmmMeasDlIpThrPut)
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmMeasDlIpThrPut(rbCb, dataVol, initialBo)\n"));

   l2MeasIpThruput = &rbCb->l2MeasIpThruput;
   if((initialBo > dataVol) && (l2MeasIpThruput->count > 0))
   {
      /*Change state from RESET to BURST_CONTINUE*/
      l2MeasIpThruput->dlIpThrputSt = KW_DL_IPTHRU_BURST_CONTINUE;

      l2MeasIpThruput->count++;

      l2MeasIpThruput->dataVol += dataVol;
   }
} /* kwAmmMeasDlIpThrPut */

/**
 *
 * @brief Private handler to calculate DL IP throughput.
 *
 * @b Description
 *      It is called from kwAmmDlAssembleSdus.
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
 * @param[in]  initialBo BO of RLC entity before starting construction of PDU
 * @param[in]  pduSz    pdu size
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmCalDlIpThrPut
(
KwRbCb          *rbCb,
MsgLen          dataVol,
S32             initialBo,
MsgLen          pduSz
)
#else
PRIVATE Void  kwAmmCalDlIpThrPut (rbCb, dataVol, initialBo, pduSz)
KwRbCb          *rbCb;
MsgLen          dataVol;
S32             initialBo;
MsgLen          pduSz;
#endif
{
   KwTbSnMap         *curTbSnMap = NULLP;   /*Holds the current TbSnMap*/
   KwL2MeasIpThruput *l2MeasIpThruput = NULLP;

   TRC2(kwAmmCalDlIpThrPut)
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmCalDlIpThrPut(rbCb, dataVol, initialBo, pduSz)\n"));

   /*Get the current TbSnMap*/
   curTbSnMap = kwCb.kwL2Cb.curTbSnMap;

   l2MeasIpThruput = &rbCb->l2MeasIpThruput;
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
   (_kwPBuf, "kwAmmCalDlIpThrPut: Log for dl ip throughput:\n \
   RB_MeasOn: %d\n curTbSnMap->numSn: %d\n State: %d\n      \
   Count: %d\n dataVol: %lu\n prevTbSnMap: %p\n initialBo: %ld\n\
   Alloc from MAC: %d\n", rbCb->rbL2Cb.measOn, curTbSnMap->numSn,
   l2MeasIpThruput->dlIpThrputSt, l2MeasIpThruput->count,
   l2MeasIpThruput->dataVol, (Void*)l2MeasIpThruput->prevTbSnMap,
   initialBo, pduSz));
#else
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
   (_kwPBuf, "kwAmmCalDlIpThrPut: Log for dl ip throughput:\n \
   RB_MeasOn: %d\n curTbSnMap->numSn: %d\n State: %d\n      \
   Count: %d\n dataVol: %u\n prevTbSnMap: %p\n initialBo: %d\n\
   Alloc from MAC: %d\n", rbCb->rbL2Cb.measOn, curTbSnMap->numSn,
   l2MeasIpThruput->dlIpThrputSt, l2MeasIpThruput->count,
   l2MeasIpThruput->dataVol, (Void*)l2MeasIpThruput->prevTbSnMap,
   initialBo, pduSz));
#endif
   /* Check if DL ip throughput measurement is going on or not AND */
   /* at least one new data PDU is formed.                         */
   if( (0 != (rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_IP)) && 
             (curTbSnMap->numSn > 0) )
   {
      if(KW_DL_IPTHRU_BURST_CONTINUE == l2MeasIpThruput->dlIpThrputSt)
      {
         kwAmmMeasDlIpBrstCont(rbCb, dataVol, initialBo);
      }
      /*2nd allocation, T2 already stamped*/
      else if (KW_DL_IPTHRU_BURST_STARTED == l2MeasIpThruput->dlIpThrputSt)
      {
         kwAmmMeasDlIpBrstStart(rbCb, dataVol, initialBo);
      }
      /*1st allocation for the current burst*/
      else if(KW_DL_IPTHRU_RESET == l2MeasIpThruput->dlIpThrputSt)
      {
         kwAmmMeasDlIpBrstReset(rbCb, dataVol, initialBo);
      }
      /*If a subsequent burst is split across TTI but the previous burst is*
       *not fully Ack-ed by MAC, then the subsequent bursts will be        *
       *considered as part of previous burst and so state is changed to    *
       *CONTINUE.                                                          */
      else /*dlIpThrputSt is KW_DL_IPTHRU_BURST_COMPLETED*/
      {
         kwAmmMeasDlIpBrstComplt(rbCb, dataVol, initialBo);
      }
   }
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
   (_kwPBuf, "kwAmmCalDlIpThrPut: Log for dl ip throughput:\n \
   RB_MeasOn: %d\n curTbSnMap->numSn: %d\n State: %d\n      \
   Count: %d\n dataVol: %lu\n prevTbSnMap: %p\n initialBo: %ld\n\
   Alloc from MAC: %d\n", rbCb->rbL2Cb.measOn, curTbSnMap->numSn,
   l2MeasIpThruput->dlIpThrputSt, l2MeasIpThruput->count,
   l2MeasIpThruput->dataVol, (Void*)l2MeasIpThruput->prevTbSnMap,
   initialBo, pduSz));
#else
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
   (_kwPBuf, "kwAmmCalDlIpThrPut: Log for dl ip throughput:\n \
   RB_MeasOn: %d\n curTbSnMap->numSn: %d\n State: %d\n      \
   Count: %d\n dataVol: %u\n prevTbSnMap: %p\n initialBo: %d\n\
   Alloc from MAC: %d\n", rbCb->rbL2Cb.measOn, curTbSnMap->numSn,
   l2MeasIpThruput->dlIpThrputSt, l2MeasIpThruput->count,
   l2MeasIpThruput->dataVol, (Void*)l2MeasIpThruput->prevTbSnMap,
   initialBo, pduSz));
#endif
}
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

/**
 *
 * @brief Private handler to assemble SDUs to form new data PDU(s)
 *
 * @b Description
 *        Its a private function called by kwProcessSdus, to create the new data PDUs 
 *        from the SDU queue of RbCb.
 *
 *        - While pdusize is available, segment/concatenate SDUs or else if it
 *          matches the pdu size form PDUs accordingly.
 *        - RLC header and MAC header size are eliminated while forming the PDUs
 *        - Call kwChkandSetPoll function to check and set the poll bit as required 
 *        - Concatenate data and header info and fill pduInfo  
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  kwdatReq DatReq to be sent to MAC 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmDlAssembleSdus 
(
KwRbCb                  *rbCb,
KwDatReq                *kwDatReq
)
#else
PRIVATE S16 kwAmmDlAssembleSdus (rbCb, kwDatReq)
KwRbCb                  *rbCb;
KwDatReq                *kwDatReq;
#endif
{
   Buffer               *pdu    = NULLP;
   MsgLen               pduSz   = kwDatReq->pduSz;
   KwAmDl               *amDl   = &AMDL;
   KwAmHdr              amHdr;
   KwSdu                *sdu;
   KwSdu                *lastSdu;
   Bool                 pduCompleted = FALSE;
   U8                   numSdu  = 0;
   KwSduMap             sduMap[KW_MAX_LI + 1];
   KwKwuSapCb           *kwuSapCb;
   MsgLen               sduSz;
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
   MsgLen               dataVol   = 0;
   S32                  initialBo = amDl->bo; /* To compare with DL alloc */
#endif
#endif

   TRC2(kwAmmDlAssembleSdus)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmDlAssembleSdus(rbCb, kwDatReq) \n"));

   kwuSapCb = (KwKwuSapCb *)(kwCb.kwuSap + rbCb->kwuSapId);
   while ((pduSz > KW_EXTN_HDRSZ) &&
          (kwDatReq->pduInfo.numPdu < KW_MAX_PDU) &&
          ((sdu = kwAmmDlGetTxSdu(rbCb)) != NULLP))
   {
      sduSz    = sdu->sduSz;

      if (pdu == NULLP)
      {
         /* Check for window stall when you are creating a new PDU */
         if (amDl->vtS == amDl->vtMs)
         {
            AMDL.partialSdu = sdu;
            break;
         }

         /* Eliminate MAC header */
         /* Bug fix: ccpu00119438, 128 is replaced with 127 */
         pduSz -= (pduSz <= 127) ? KW_MAC_HDR_SZ2 : KW_MAC_HDR_SZ3;

         /* Eliminate fixed header size */
         pduSz -= KW_AM_PDU_FIXED_HDRSZ;
        
         /* Check for PDU Size is large enough */
         if (pduSz <= KW_EXTN_HDRSZ)
         {
            AMDL.partialSdu = sdu;
            break;
         }

         /* Initialize the datReq structure */
         cmMemset( (U8 *)&amHdr, 0, sizeof(KwAmHdr) );
         amHdr.sn = amDl->vtS;

         /* Update Framing Info */
         if (sdu->mode.am.isSegmented) 
         {
            amHdr.fi = KW_FI_FIRST_SEG;
         }
         pduCompleted = FALSE;
      }

      /* Eliminate extension hdr size */
      KW_AM_ELIMINATE_EXTN_HDR(pduSz, sduSz, amHdr.numLi);
      if (pduSz <= 0)
      {
         MSPD_ERR("[%d][%d][%d] Erroneous pduSz %d\n", KW_UEID, KW_RBTYPE, KW_RBID, pduSz);
#ifdef MSPD
         stop_printf("Erroneous pduSz = %d\n", pduSz);
#endif
         break;
      }
 
      amHdr.li[amHdr.numLi++] = sduSz;
      /* WORKAROUND
       * Increment sdu->mode.am.numPdu here Dont move this Line of code This is done specifially
       * Here to avoid race condition for accessing numPdu during preparation of DL PDU and processing of
       * Status PDU in case same sdu is present in 2 PDU
       */
      sdu->mode.am.numPdu++;
      if (pduSz == sduSz)
      {
         /* Exact fit : Either one sdu equals pdu or the last sdu of a pdu */
         sdu->sduSz = 0;
#ifndef ALIGN_64BIT
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
                (_kwPBuf, "kwAmmDlAssembleSdus: PduSz matches SduSz for \
                 sduId = %ld \n", sdu->mode.am.sduId));
#else
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
                (_kwPBuf, "kwAmmDlAssembleSdus: PduSz matches SduSz for \
                 sduId = %d \n", sdu->mode.am.sduId));
#endif
         /* Concatenate sdu to pdu */
         if (pdu == NULLP)
         {
            pdu = sdu->mBuf;     
            sdu->mBuf = NULLP; 
         }
         else
         {
            SCatMsg (pdu, sdu->mBuf, M1M2);  
         }

#ifdef LTE_L2_MEAS
         kwUtlUpdSduSnMap(rbCb, sdu, kwDatReq, TRUE);
#endif
         /* Create PDU with hdr and data */
         pduCompleted = kwCreatePdu(rbCb, &amHdr, pdu);

         /* For estHdrSz */
         amDl->numSduPdu++; 
         kwuSapCb->sts.sduTx++;
      }
      else if (pduSz > sduSz)
      {
         /* Concatenation: Join SDU to PDU */
         sdu->sduSz = 0;
#ifndef ALIGN_64BIT
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
                (_kwPBuf, "kwAmmDlAssembleSdus: Concatenation for \
                 sduId = %ld \n", sdu->mode.am.sduId));
#else
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
                (_kwPBuf, "kwAmmDlAssembleSdus: Concatenation for \
                 sduId = %d \n", sdu->mode.am.sduId));
#endif
         /* Concatenate sdu to pdu */
         if (pdu == NULLP)
         {
            pdu = sdu->mBuf;      
            sdu->mBuf = NULLP; 
         }
         else
         {
            SCatMsg(pdu, sdu->mBuf, M1M2);
         }
#ifdef LTE_L2_MEAS
         kwUtlUpdSduSnMap(rbCb, sdu, kwDatReq, TRUE);
#endif /*  LTE_L2_MEAS */

         /* This sdu completes the PDU with the following condition */
         /* Temp fix in MSPD, use '5' instead of KW_MAX_LI for numLi check */
         if ((sduSz > 2048) || (amHdr.numLi >= 5))
         {
            /* Create PDU with hdr and data */
            pduCompleted = kwCreatePdu(rbCb, &amHdr, pdu);
         }
         
         /* For estHdrSz */
         amDl->numSduPdu++; 
         kwuSapCb->sts.sduTx++;
      }
      else 
      {
         /* Segmentation
          * Allocate buffer for next PDU
          * Remove the segmented portion from SDUQ 
          * Calculate the hdr with LI for SDU */
         Buffer         *remSeg = NULLP;

#ifndef ALIGN_64BIT
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
                (_kwPBuf, "kwAmmDlAssembleSdus: Segmentation for \
                 sduId = %ld \n", sdu->mode.am.sduId));
#else
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
                (_kwPBuf, "kwAmmDlAssembleSdus: Segmentation for \
                 sduId = %d \n", sdu->mode.am.sduId));
#endif

         /* Segment the SDU to the size of the PDU and update header Info */
         SSegMsg(sdu->mBuf, pduSz, &remSeg);
         if (pdu == NULLP)
         {
            pdu = sdu->mBuf;      
         }
         else
         {
            SCatMsg (pdu, sdu->mBuf, M1M2);  
            KW_FREE_BUF(sdu->mBuf);
         }
         sdu->mBuf                = remSeg;
         sdu->sduSz              -= pduSz;
         sduSz                    = pduSz;
         sdu->mode.am.isSegmented = TRUE;
         AMDL.partialSdu          = sdu;

         /* Update FI and Li */
         amHdr.fi |= KW_FI_LAST_SEG; /* 01 */

#ifdef LTE_L2_MEAS
         kwUtlUpdSduSnMap(rbCb, sdu, kwDatReq, FALSE);
#endif
         /* Create PDU with hdr and data */
         pduCompleted = kwCreatePdu(rbCb, &amHdr, pdu);
      }

      /* Update bo for boReport */
      kwAmmUpdateBo(rbCb, sduSz);
#ifdef MSPD
      if(rbCb->rlcId.rbType == CM_LTE_DRB)
      {
         if(amDl->bo < 0)
         {
            MSPD_ERR("[%d][%d]stop_printf BO %d %d\n", KW_UEID, KW_RBID, 
                      amDl->bo, sduSz);
            stop_printf("amDl->bo negetive bo %d sduSz %d\n", amDl->bo, sduSz);
         }
         ysMsAmmNewDataBo = amDl->bo;
      }
#endif
      /* Incrementing sdu->mode.am.numPdu on top if block above */
      sdu->mode.am.pduMap[sdu->mode.am.numPdu].sn = amHdr.sn; 
      sduMap[numSdu].sdu   = sdu;
      sduMap[numSdu].sduSz = sduSz;
      sduMap[numSdu].actSz = sduSz;
      sduMap[numSdu++].isSegmented = sdu->mode.am.isSegmented;

      if (pduCompleted)
      {
         U8             i;
         KwTx           *txBuf;

         /* Update pduInfo */
         kwDatReq->pduInfo.mBuf[kwDatReq->pduInfo.numPdu] = pdu; 
         kwDatReq->pduInfo.numPdu++;
         kwCb.genSts.pdusSent++;

         /* Update the RLC Tx buffer with the new PDU info */
         txBuf = amDl->txBuf[amHdr.sn];
         txBuf->numSdu = numSdu;
         for (i = 0; i < numSdu; i++)
         {
            txBuf->sduMap[i] = sduMap[i];
         }
         /* initialize the loop variables that are per PDU */
         pdu = NULLP;
         numSdu = 0;
      }

      pduSz -= sduSz;

#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
      dataVol += sduSz;
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

      /* If the SDU is not completely processed, it is passed to code */
      /* below the while loop through this value                      */
      lastSdu = sdu;
   }

#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
   /* Calculate DL IP throughput */
   kwAmmCalDlIpThrPut(rbCb, dataVol, initialBo, kwDatReq->pduSz);
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

   if ((pdu != NULLP) && (pduSz > 0))
   {
      U8                i;

      if (!pduCompleted)
      {
         KwTx           *txBuf;

         /* Create PDU with hdr and data */
         kwCreatePdu(rbCb, &amHdr, pdu);
         kwDatReq->pduInfo.mBuf[kwDatReq->pduInfo.numPdu] = pdu; 
         kwDatReq->pduInfo.numPdu++;
         kwCb.genSts.pdusSent++;
         pdu = NULLP;

         lastSdu->mode.am.pduMap[lastSdu->mode.am.numPdu].sn = amHdr.sn;
         txBuf = amDl->txBuf[amHdr.sn];
         txBuf->numSdu = numSdu;
         for (i = 0; i < numSdu; i++)
         {
            txBuf->sduMap[i] = sduMap[i];
         }
      }
   }
   
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
         (_kwPBuf, "kwAmmDlAssembleSdus: BO after assembly = %ld \n",\
          amDl->bo));
#else
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
         (_kwPBuf, "kwAmmDlAssembleSdus: BO after assembly = %d \n",\
          amDl->bo));
#endif
   RETVALUE(ROK);
}

/**
 *
 * @brief Private handler to check if the poll bit needs to be set for data PDU
 *
 * @b Description
 *        Its a private function called by kwProcessSdus, to checks if the 
 *        polling bit needs to be set for any RLC data PDU and updates the 
 *        same.
 *        - For the new PDUs, if the counters exceed the configured 
 *          pduWoPoll/byteWoPoll values, return poll bit.
 *        - For the PDUs/portion of PDUs, if the SDU list / retxBuf is 
 *          empty, return poll bit.
 *        - Update the pollPdu, pollByte counters and Poll_SN; start staProhTmr 
 *
 * @param[in]  rbCb     RB control block 
 * @param[in]  newPdu   Flag to indicate if its a new AMD PDU. 
 *
 *  @return  Bool 
 *      -# 1 - To set the poll bit
 *      -# 0 - Poll bit is not set
 *
 */
#ifdef ANSI
PRIVATE Bool kwChkandSetPoll
(
KwRbCb                 *rbCb,
Bool                   newPdu,
MsgLen                 bufSz
)
#else
PRIVATE Bool kwChkandSetPoll(rbCb, newPdu, bufSz)
KwRbCb                 *rbCb;
Bool                   newPdu;
MsgLen                 bufSz;
#endif
{ 
   KwSn                mSn;
   KwSn                vtA;
   KwSn                vtMs;
   Bool                pollBit = FALSE;
   KwAmDl              *amDl = &(rbCb->m.am.amDl);

   TRC2(kwChkandSetPoll)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwChkandSetPoll(rbCb, newPdu, bufSz) \n"));
 
   /* Check if its a new PDU */ 
   if (newPdu)
   {
      amDl->pduWoPoll++;
      amDl->byteWoPoll += bufSz;

     if (((amDl->pollPdu != -1) && (amDl->pduWoPoll >= amDl->pollPdu)) || 
         ((amDl->pollByte != -1) && (amDl->byteWoPoll >= amDl->pollByte))) 
     {
        pollBit = TRUE;
     }
   }
   MODAMT(amDl->vtA, vtA, amDl->vtA); 
   MODAMT(amDl->vtS, mSn, amDl->vtA);
   MODAMT(amDl->vtMs, vtMs, amDl->vtA);

   /* Check if both tx/retx buffer are empty or if tx window is stalled */
   if (((kwAmmDlTxSduPrsnt(rbCb)) && (amDl->nxtRetx == NULLP)) ||
       (!((vtA <= mSn) && (mSn < vtMs))))
   {
      pollBit = TRUE;
   }
   

   if (pollBit)
   {
      amDl->pduWoPoll  = 0;
      amDl->byteWoPoll = 0;

      amDl->pollSn = (amDl->vtS) ? amDl->vtS - 1 : 1023;

      KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
            (_kwPBuf, "kwChkandSetPoll: Poll SN = %d \n", amDl->pollSn));

      /* Fix for poll retransmission timer. Timer is stopped if it is */
      /* already running and then starting the timer.                 */
      if (TRUE == kwChkTmr((PTR)rbCb,KW_EVT_AMDL_POLL_RETX_TMR))
      {
         kwStopTmr((PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR);
      }

      kwStartTmr((PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR);
   }

   RETVALUE(pollBit);
}

/**
 *
 * @brief Private handler to create AMD PDU 
 *
 * @b Description 
 *        This function constructs header and concatenate it with the data for
 *        the PDU. It also updates the txBuf with the created PDU.
 *
 * @param[in]  amDl     AM Downlink Information 
 * @param[in]  amHdr    AM header
 * @param[in]  pdu      PDU buffer 
 *
 *  @return  Bool 
 *      -# 1 - to set the poll bit
 *      -# 0 - poll bit will not be set
 *
 */
#ifdef ANSI
PRIVATE Bool kwCreatePdu
(
KwRbCb               *rbCb,
KwAmHdr              *amHdr,
Buffer               *pdu
)
#else
PRIVATE Bool kwCreatePdu(rbCb, amHdr, pdu)
KwRbCb               *rbCb;
KwAmHdr              *amHdr;
Buffer               *pdu;
#endif
{
   U8                hdr[KW_MAX_HDRSZ];
   U16               idx     = 0;
   Bool              segFlg = FALSE;
   KwTx              *txBuf = NULLP;
   MsgLen            pduSz  = 0;
   KwAmDl            *amDl = &(rbCb->m.am.amDl);

   TRC2(kwCreatePdu)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwCreatePdu(rbCb, amHdr, pdu) \n"));

   /* Update sn */
   amHdr->sn = amDl->vtS;

   /* Increment vtS */
   amDl->vtS = (amDl->vtS + 1) % 1024;

   /* Update hdr Info */
   amHdr->rf = 0;
   SFndLenMsg(pdu, &pduSz);

   amHdr->p = kwChkandSetPoll(rbCb, TRUE, pduSz);

   /* Construct header with the available hdr Info */
   kwConstructAmHdr(amHdr, segFlg, hdr, &idx);

   /* Concatenate hdr and data */
   KW_ADD_PRE_MSG_AM(pdu, hdr, idx+1);

   /* Update txBuf */
   KW_ALLOC(txBuf, sizeof(KwTx));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (txBuf == NULLP)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW007, (ErrVal)0,
            "kwCreatePdu: Allocation failed for txBuf \n");
      RETVALUE(FALSE);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   amDl->txBuf[amHdr->sn] = txBuf;

   kwCpyMsg(pdu,&(txBuf->pdu));
   txBuf->pduSz = pduSz;
   txBuf->hdrSz = idx+1;
   kwCb.genSts.bytesSent += pduSz;
   
   KW_MEM_CPY(&txBuf->amHdr, amHdr, sizeof(KwAmHdr));    

   //MSPD_DBG("1 **** SN %d KwTx %x pdu %x pduSz %d ****\n", 
   //          txBuf->amHdr.sn, txBuf, txBuf->pdu, txBuf->pduSz);
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
          (_kwPBuf, "kwCreatePdu: Created PDU with SN = %d \n", amHdr->sn));

#ifdef MSPD
   {
       extern U32 ysMsDlAmTxPdus;
       ysMsDlAmTxPdus++;
   }
#endif
   RETVALUE(TRUE);
}

#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
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
PRIVATE Void kwAmmCalUlIpThrPut
(
KwRbCb                *rbCb,
Buffer                *pdu,
KwL2MeasUlThrpTimInfo *ulTimeInfo,
Bool                  insideLoop
)
#else
PRIVATE Void kwAmmCalUlIpThrPut(rbCb, pdu, ulTimeInfo, insideLoop)
KwRbCb                *rbCb;
Buffer                *pdu;
KwL2MeasUlThrpTimInfo *ulTimeInfo;
Bool                  insideLoop;
#endif
{
   MsgLen              rlcSduSz = 0;  /*Holds length of Rlc Sdu*/

   TRC2(kwAmmCalUlIpThrPut)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwAmmCalUlIpThrPut(rbCb, pduInfo) \n"));
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
   (_kwPBuf, "kwAmmCalUlIpThrPut: Log for ul ip throughput:\n \
   RB_MeasOn:%d SegInd:%d thrpTimeUl:%lu\n", rbCb->rbL2Cb.measOn, \
   ulTimeInfo->firstLastSegInd, ulTimeInfo->thrpTimeUl));
#else
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
   (_kwPBuf, "kwAmmCalUlIpThrPut: Log for ul ip throughput:\n \
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
               rbCb->rbL2Cb.l2Sts[KW_L2MEAS_UL_IP]->ulIpThruput.volSummation 
                  += rbCb->l2MeasIpThruput.ulDataVol;
               rbCb->rbL2Cb.l2Sts[KW_L2MEAS_UL_IP]->ulIpThruput.timeSummation 
                  += ulTimeInfo->thrpTimeUl;
               /*Reset ulDataVol in rbCb since it has been saved in l2Sts*/
               rbCb->l2MeasIpThruput.ulDataVol = 0;
            } /* end of if(0 == ulTimeInfo->thrpTimeUl) */
         } /* end of if(RGU_L2M_ULTHRP_LAST == ulTimeInfo->firstLastSegInd) */
      } /* end of if(TRUE == insideLoop) */
   } /* end of if(0 != (rbCb->rbL2Cb.measOn & LKW_L2MEAS_UL_IP) ) */
} /* kwAmmCalUlIpThrPut */
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

#ifdef ANSI
PRIVATE Void kwAmmUlRlsPdu
(
KwRbCb                  *rbCb,
KwAmRecBuf              *pdu
)
#else
PRIVATE Void kwAmmUlRlsPdu(rbCb, pdu)
KwRbCb                  *rbCb;
KwAmRecBuf              *pdu;
#endif
{
   KwSeg                *seg;

   if (pdu->pdu != NULLP)
   {
      KW_FREE_BUF(pdu->pdu);
   }
   /*If there are any segments for the PDU, free them    */
   KW_LLIST_FIRST_SEG(pdu->segLst, seg);                     
   while (seg)
   {
      KW_FREE_BUF(seg->seg); 
      cmLListDelFrm(&(pdu->segLst),&(seg->lstEnt));
      KW_FREE(seg, sizeof(KwSeg));
      KW_LLIST_FIRST_SEG(pdu->segLst, seg);
   }
   KW_FREE(pdu, sizeof(KwAmRecBuf));
}

#ifdef ANSI
PRIVATE Void kwAmmUlAddPduForReassembly
(
KwRbCb                  *rbCb,
KwAmRecBuf              *pdu
)
#else
PRIVATE Void kwAmmUlAddPduForReassembly(rbCb, pdu)
KwRbCb                  *rbCb;
KwAmRecBuf              *pdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.ulLock);
   {
      cmLListAdd2Tail(&AMUL.reassemblyLst, &(pdu->reassemblyLnk));
      pdu->reassemblyLnk.node = (PTR)pdu;
   }
   KW_RDWR_UNLOCK(&kwCb.ulLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmUlRemPduFrmRlsLst
(
KwRbCb                  *rbCb,
KwAmRecBuf              *pdu
)
#else
PRIVATE Void kwAmmUlRemPduFrmRlsLst(rbCb, pdu)
KwRbCb                  *rbCb;
KwAmRecBuf              *pdu;
#endif
{
   //KW_RDWR_LOCK(&kwCb.ulLock);
   {
      cmLListDelFrm(&AMUL.reassemblyLst, &(pdu->reassemblyLnk));
   }
   //KW_RDWR_UNLOCK(&kwCb.ulLock);
   pdu->reassemblyLnk.node = NULLP;
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmUlGetPduFrmReassemblyLst
(
KwRbCb                  *rbCb,
KwAmRecBuf              **pdu
)
#else
PRIVATE Void kwAmmUlGetPduFrmReassemblyLst(rbCb, pdu)
KwRbCb                  *rbCb;
KwAmRecBuf              **pdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.ulLock);
   {
      if (AMUL.reassemblyLst.first == NULLP)
      {
         *pdu = NULLP;
      }
      else
      {
         *pdu = (KwAmRecBuf *)(AMUL.reassemblyLst.first->node);
         cmLListDelFrm(&AMUL.reassemblyLst, &((*pdu)->reassemblyLnk));
      }
   }
   KW_RDWR_UNLOCK(&kwCb.ulLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmUlGetPduForReassembly
(
KwRbCb                  *rbCb,
KwAmRecBuf              **pdu
)
#else
PRIVATE Void kwAmmUlGetPduForReassembly(rbCb, pdu)
KwRbCb                  *rbCb;
KwAmRecBuf              **pdu;
#endif
{
   /* Get the pdu only if delete/reestablishment flag is not set             */
   if(rbCb->rbOp == 0)
   {
      if (AMUL.partPdu == NULLP)
      {
         kwAmmUlGetPduFrmReassemblyLst(rbCb, pdu);
      }
      else
      {
         /* A partial PDU is waiting to be reassmbled */
         *pdu = AMUL.partPdu;
         AMUL.partPdu = NULLP;
      }
   }
   else
   {
      *pdu = NULLP;
   }
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
PRIVATE Void kwAmmUlProcessInSeqPdu
(
KwRbCb                       *rbCb,
U16                          sn
)
#else
PRIVATE Void kwAmmUlProcessInSeqPdu(rbCb, sn)
KwRbCb                       *rbCb;
U16                          sn;
#endif
{
   kwAmmUlAddPduForReassembly(rbCb, AMUL.recBuf[sn]);
   AMUL.recBuf[sn] = NULLP;
}
#ifdef ANSI
PRIVATE Void kwAmmDlAddStaPduToRb
(
KwRbCb                  *rbCb,
Buffer                  *pdu,
U8                      firstByte
)
#else
PRIVATE Void kwAmmDlAddStaPduToRb(rbCb, pdu, firstByte)
KwRbCb                  *rbCb;
Buffer                  *pdu;
U8                      firstByte;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      if (AMDL.staPdu != NULLP)
      {
#ifdef MSPD
         ysMsAmmStaPduCounter--;
#endif
         SPutMsg(AMDL.staPdu);
      }
      AMDL.staPdu = pdu;
      AMDL.staPduFirstByte = firstByte;
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
}

#ifdef ANSI
PRIVATE Void kwAmmDlAddRbToStaPduLst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmDlAddRbToStaPduLst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      if (AMDL.staPduLnk.node == NULLP)
      {
         cmLListAdd2Tail(&kwCb.staPduRbLst, &AMDL.staPduLnk);
         AMDL.staPduLnk.node = (PTR)rbCb;
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
}

#ifdef ANSI
PRIVATE Void kwAmmDlRemRbFrmStaPduLst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmDlRemRbFrmStaPduLst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      if (AMDL.staPduLnk.node != NULLP)
      {
         cmLListDelFrm(&kwCb.staPduRbLst, &AMDL.staPduLnk);
         AMDL.staPduLnk.node = NULLP;
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
}

#ifdef ANSI
PRIVATE Void kwAmmDlGetRbFrmStatusPduLst
(
KwRbCb                  **rbCb,
Buffer                  **pdu,
U8                      *fByte
)
#else
PRIVATE Void kwAmmDlGetRbFrmStatusPduLst(rbCb, pdu, fByte)
KwRbCb                  **rbCb;
Buffer                  **pdu;
U8                      *fByte;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      if (kwCb.staPduRbLst.first == NULLP)
      {
         *rbCb = NULLP;
         *pdu  = NULLP;
      }
      else
      {
         *rbCb = (KwRbCb *)kwCb.staPduRbLst.first->node;
         (*pdu) = (*rbCb)->m.am.amDl.staPdu;
         (*fByte) = (*rbCb)->m.am.amDl.staPduFirstByte;
         (*rbCb)->m.am.amDl.staPdu = NULLP;
         (*rbCb)->m.am.amDl.staPduLnk.node = NULLP;
         cmLListDelFrm(&kwCb.staPduRbLst, kwCb.staPduRbLst.first);
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

#ifdef SIM_REEST
//U8   wrapCnt;
#endif
#ifdef SIM_MAXRETX
//U16   tempStaPduCnt;
#endif
/**
 *
 * @brief Handler to process the PDUs received from MAC and send it to PDCP 
 *
 * @b Description
 *        This function is invoked by UTL with the PDU(s) received from MAC. 
 *        It reorders the received data PDUs and trigger status report as 
 *        needed. Reassembles the SDUs in sequence and send it to PDCP. 
 *        It also processes the control PDU and updates PDCP with the 
 *        successful delivery of an SDU if it receives ACK for all the PDUs 
 *        mapped with that SDU.
 *
 * @param[in]  rbCb     RB control block 
 * @param[out] pduInfo  PDU Info received from MAC
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
#ifdef ANSI
PUBLIC S16 kwAmmProcessPdus
(
KwRbCb                *rbCb,
KwPduInfo             *pduInfo,
KwL2MeasUlThrpTimInfo *ulTimeInfo
)
#else
PUBLIC S16 kwAmmProcessPdus(rbCb, pduInfo, ulTimeInfo)
KwRbCb                *rbCb;
KwPduInfo             *pduInfo;
KwL2MeasUlThrpTimInfo *ulTimeInfo;
#endif
#else
#ifdef ANSI
PUBLIC S16 kwAmmProcessPdus
(
KwRbCb                *rbCb,
KwPduInfo             *pduInfo
)
#else
PUBLIC S16 kwAmmProcessPdus(rbCb, pduInfo)
KwRbCb                *rbCb;
KwPduInfo             *pduInfo;
#endif
#endif
#else
#ifdef ANSI
PUBLIC S16 kwAmmProcessPdus
(
KwRbCb                *rbCb,
KwPduInfo               *pduInfo
)
#else
PUBLIC S16 kwAmmProcessPdus(rbCb, pduInfo)
KwRbCb                  *rbCb;
KwPduInfo               *pduInfo;
#endif
#endif
{
   
   Buffer               *pdu   = NULLP;
   KwAmUl               *amUl  = &AMUL;
   KwAmHdr              amHdr;
   U8                   numPdu = 0;    
   KwSn                 sn     = 0;
   KwSn                 tSn    = 0;
   KwSn                 mSn    = 0;
   U8                   fByte  = 0;
   Bool                 discFlg;
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
   Bool                 insideLoop = TRUE;
#endif
#endif
#ifdef MSPD_MLOG_NEW
   U32                  t1;
#endif
   TRC2(kwAmmProcessPdus)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwAmmProcessPdus(rbCb, pduInfo) \n"));
#ifdef MSPD_MLOG_NEW
   t1 = GetTIMETICK();
#endif
   amUl = &AMUL;

   while ((numPdu < pduInfo->numPdu) &&
          (numPdu < RGU_MAX_PDU))
   {
      S16     ret   = ROK;

      discFlg = FALSE;
      pdu     = pduInfo->mBuf[numPdu++];
      
      /* Avoided the allocation of amHdr and sending a single pointer */
      /* Extract AM PDU/SEG header Info                               */
      KW_MEM_ZERO(&amHdr, sizeof(KwAmHdr));
      ret = kwExtractHdr(rbCb, pdu, &amHdr, &fByte);
      if (ret != ROK)
      {
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                (_kwPBuf, "kwAmmProcessPdus: Failed to extract hdr \n"));
         MSPD_ERR("[%d][%d][%d] Failed to extract header\n", KW_UEID, KW_RBTYPE, KW_RBID);
         kwCb.genSts.errorPdusRecv++;
#ifdef MSPD
         ysMsAmmUlNumPduDiscarded++;
#endif
         KW_FREE_BUF(pdu);
         continue;
      }
      /* Check if its a control PDU */
      if (amHdr.dc == 0)
      {
#ifdef SIM_MAXRETX
         if(rbCb->rlcId.rbType == CM_LTE_DRB)
         {
            rbCb->tempStaPduCnt++;
            if(rbCb->tempStaPduCnt >= 1000)
            {
               /* Start dropping status pdu until DL max retransmission expires */
               MSPD_DBG("[%d][%d]Dropping status pdu\n", KW_UEID, KW_RBID);
               KW_FREE_BUF(pdu);
               continue;
            }
         }
#endif
#ifdef MSPD
         ysMsAmmStaPduCounter++;
#endif
         /* WORKAROUND for Stoping PollRetxTmr in Core 2*/
         {
            U8 tempDat;
            KwSn ackSn, mAckSn, mVts;
            SExamMsg(&tempDat, pdu, 0);
            ackSn = (fByte & 0x0F )<< 6;
            ackSn = ackSn | ((tempDat & 0xFC)>>2);
            MODAMT(AMDL.vtS, mVts, AMDL.vtA);
            MODAMT(ackSn, mAckSn, AMDL.vtA);
            if (mAckSn > mVts)
            {
                MSPD_ERR("[%d][%d][%d] Received erroneous status pdu\n", 
                KW_UEID, KW_RBTYPE, KW_RBID);
            }
            else
            {
               KW_CHK_FOR_POLLSN(rbCb, mAckSn); 
            }
         }
         kwAmmDlAddStaPduToRb(rbCb, pdu, fByte);
         kwAmmDlAddRbToStaPduLst(rbCb);
         kwAmmDlPostSelfMsg();
         continue;
      }   
       
      /* Reordering data PDU */ 
      sn = amHdr.sn;
#ifdef SIM_REEST
      if((KW_RBTYPE == CM_LTE_DRB) && (sn == 1023))
      {
         rbCb->wrapCnt++;
      }
      if((rbCb->wrapCnt >= 5) && (KW_RBTYPE == CM_LTE_DRB) && (sn == 1000))
      {
         /* Dropping PDU SN 1000*/
         KW_FREE_BUF(pdu);
         ret = FALSE;
      }
      else
      {
         ret = kwPlcPduInRecBuf(pdu, rbCb, &amHdr);
      }
#else
      ret = kwPlcPduInRecBuf(pdu, rbCb, &amHdr);
#endif
      if ( ret == TRUE) 
      {
         Bool    tmrRunning = FALSE;
         KwSn    tVrMr      = 0;
         KwSn    mVrH       = 0;
         //MSPD_DBG("[%d][%d]Received SN %d, NumLi %d FI %d RF %d\n", KW_UEID, KW_RBID,
         //          amHdr.sn, amHdr.numLi, amHdr.fi, amHdr.rf);
   
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
         kwAmmCalUlIpThrPut(rbCb, pdu, ulTimeInfo, insideLoop);
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

         /* Update vrH */
         MODAMR(sn, mSn, amUl->vrR);
         MODAMR(amUl->vrH, mVrH, amUl->vrR);
         if (mSn >= mVrH)
         {
            amUl->vrH = (sn + 1) % 1024;

            KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
                  (_kwPBuf, "kwAmmProcessPdus: Updated vrH = %d \n", \
                   amUl->vrH));
         }

         MODAMR(amUl->vrMr, tVrMr, amUl->vrR);

         /* Update vrMs */
         if (sn == amUl->vrMs)
         {
            /* Check if all byte segments are received */
            if (amUl->recBuf[sn]->allRcvd)
            {
               tSn = (sn + 1) % 1024;

               /* Scan through till the upper edge of the window */
               MODAMR(tSn, mSn, amUl->vrR);
               while (mSn <= tVrMr) 
               {
                 if ((amUl->recBuf[tSn] == NULLP) || 
                     (!amUl->recBuf[tSn]->allRcvd))
                 {
                    KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
                           (_kwPBuf, "kwAmmProcessPdus: Updated vrMs = %d \n", \
                            tSn));

                    amUl->vrMs = tSn;
                    break;
                 }
                 tSn = (tSn + 1) % 1024; mSn++;
               }
            }
         }

         /* Update vrR */
         if (sn == amUl->vrR)
         {
            /* Check if all byte segments are received */
            if (amUl->recBuf[sn]->allRcvd)
            {
               tSn = sn;
               MODAMR(tSn, mSn, amUl->vrR);
               /* Scan through till the upper edge of the window */
               while (mSn <= tVrMr)
               {
                  if ((amUl->recBuf[tSn] != NULLP) && 
                      (amUl->recBuf[tSn]->allRcvd))
                  {
                     kwAmmUlProcessInSeqPdu(rbCb, tSn);
                  }
                  else
                  {
                     amUl->vrR = tSn;
                     amUl->vrMr = (amUl->vrR + KW_AM_WIN_SZ) % 1024; 
                     //MSPD_DBG_RLC("[%d][%d] vrR updated to %d\n",
                     //             KW_UEID, KW_RBID, tSn);
                     break;
                  }
                  tSn = (tSn + 1) % 1024; mSn++;
               }
            }
         }

         /* R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0090           *
          * Removed VR(H) update from here and moved it at the    *
          * beginning.                                            */

         /* Check if reOrdTmr is running and update vrX accordingly */
         tmrRunning = kwChkTmr((PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
         if (tmrRunning)
         {
            Bool    snInWin = FALSE;
            KW_AM_CHK_SN_IN_WINDOW(amUl->vrX, amUl, snInWin);
       
            /* R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0083          *
             * The 3rd condition in "if" statement is added.        */
            if ( (amUl->vrX == amUl->vrR) || ( (!snInWin) && 
                                             (amUl->vrX != amUl->vrMr) ) )
            {
               //MSPD_DBG_RLC("[%d][%d][%d] Stop Reordering Tmr vr(R:%d H:%d X:%d Mr:%d Ms:%d)Hdr(%d:%d:%d:%d)\n", KW_UEID, KW_RBTYPE, KW_RBID,
               //amUl->vrR, amUl->vrH, amUl->vrX, amUl->vrMr, amUl->vrMs, amHdr.p, amHdr.rf, amHdr.lsf, amHdr.so);
            
               kwStopTmr((PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
               tmrRunning = FALSE;
            }
         }
         if (!tmrRunning)
         {
            if (amUl->vrR != amUl->vrH)
            {
               //MSPD_DBG_RLC("[%d][%d][%d] Start Reordering Tmr\n",
               //             KW_UEID, KW_RBTYPE, KW_RBID);
               //MSPD_DBG_RLC("[%d][%d]Received SN %d Exp SN %d,vr(Mr %d, Ms "
               //             "%d, H %d, X %d) NumLi %d FI %d RF %d p %d so %d\n",
               //             KW_UEID, KW_RBID, sn, amUl->vrR,amUl->vrMr,
               //             amUl->vrMs, amUl->vrH, amUl->vrX, amHdr.numLi,
               //             amHdr.fi, amHdr.rf, amHdr.p, amHdr.so);
               kwStartTmr((PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
               amUl->vrX = amUl->vrH;

               KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
                     (_kwPBuf, "kwAmmProcessPdus: Updated vrX = %d \n", \
                      amUl->vrX));
            }
         }
      }
      else
      {
         discFlg = TRUE;
         MSPD_ERR("[%d][%d][%d] Discarded SN %d FI %d RF %d\n", KW_UEID, KW_RBTYPE, KW_RBID, sn, amHdr.fi, amHdr.rf);
      }

      if (amHdr.p)
      {
         kwTriggerStatus (rbCb, sn, discFlg);
      } 
   }
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
   insideLoop = FALSE;
   kwAmmCalUlIpThrPut(rbCb, NULLP, ulTimeInfo, insideLoop);
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */
   kwCb.genSts.pdusRecv += pduInfo->numPdu;

   /* Add the RB to the list of RBs which have PDUs to be reassembled */
   /* Post a message for reassembly which happens in background       */
   kwUtlUlAddRbToReassemblyLst(rbCb);
   kwUtlPostSelfMsgUl(2);
   if (amUl->cntrlBo)
   {
      AMDL.cntrlBo = amUl->cntrlBo;
      KW_AM_SND_DSTA_RSP();
   }
#ifdef MSPD_MLOG_NEW
   /* Logging time taken by this task */
   MLogTask(PID_RLC_AM_RGU_DATIND, RESOURCE_LARM, t1, GetTIMETICK());
#endif

   RETVALUE(ROK);
} /* kwAmmProcessPdus */

/**
 *
 * @brief Private handler to extract header Information of the PDU
 *
 * @b Description 
 *        This function extracts the header elements of the PDU and store them 
 *        in db for future reference.
 *
 *        fByte - is the first byte removed from the PDU as part of calling
 *                functions
 *
 * @param[in]  pdu      Received PDU
 * @param[out] amUl     AM UL Info 
 *
 *  @return S16 
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PRIVATE S16 kwExtractHdr 
(
KwRbCb    *rbCb,
Buffer    *pdu,
KwAmHdr   *amHdr,
U8        *fByte
)
#else
PRIVATE S16 kwExtractHdr(rbCb, pdu, amHdr, fByte)
KwRbCb    *rbCb;
Buffer    *pdu;
KwAmHdr   *amHdr;
U8        *fByte;
#endif
{
   U8     e = 0;
   U8     snByte = 0;
   U16    li = 0;
   U16    liCnt = 0;
   U16    sn = 0;
   MsgLen pduSz = 0;
   MsgLen totalSz = 0;
   KwExtHdr hdrInfo;

   TRC2(kwExtractHdr)
   UNUSED(rbCb);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwExtractHdr(rbCb, pdu, amHdr, fByte) \n"));

   KW_MEM_ZERO(&hdrInfo, sizeof(KwExtHdr)); 


   /* Extract fixed part of the header */ 
   SFndLenMsg(pdu,&pduSz);
   SRemPreMsg(fByte, pdu);
   amHdr->dc = (*fByte & KW_DC_POS)? KW_DATA_PDU : KW_CNTRL_PDU;
   if (amHdr->dc == KW_CNTRL_PDU)
   {
      RETVALUE(ROK);
   }
   amHdr->rf = (*fByte & KW_RF_POS) ? TRUE: FALSE;
   amHdr->p  = (*fByte & KW_POLL_POS) ? TRUE:FALSE;
   amHdr->fi = (*fByte & KW_FI_POS) >> 3;
   e = amHdr->e  = (*fByte & KW_E_POS)? TRUE:FALSE;

   SRemPreMsg(&snByte, pdu);
   sn = (U16)(((*fByte & KW_SN_POS) << KW_BYTE_LEN ) | snByte);
   amHdr->sn = sn;
   if (amHdr->rf == 1)
   {
      /* Extract extn part of the header */
      hdrInfo.len = KW_LSF_LEN;
      kwExtractElmnt(pdu, &hdrInfo);
      amHdr->lsf = (U8)hdrInfo.val;

      hdrInfo.len = KW_SO_LEN;
      kwExtractElmnt(pdu, &hdrInfo); 
      amHdr->so = hdrInfo.val;
      pduSz -= 2;
   }

   /* Extract LIs */ 
   /*ccpu00122597:Boundary check for li is added*/
   while (e && pdu && (liCnt < KW_MAX_LI))
   {
      hdrInfo.len = KW_E_LEN;
      kwExtractElmnt(pdu, &hdrInfo); 
      e = amHdr->e = (U8)hdrInfo.val;

      if ( pdu )
      {
         hdrInfo.len = KW_LI_LEN;
         kwExtractElmnt(pdu, &hdrInfo);
         li = hdrInfo.val;

         if (li == 0)
         {
            KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf, "kwExtractHdr: Received LI with 0 \n"));
            MSPD_ERR("[%d][%d][%d] Received LI with value as 0\n", KW_UEID, 
                      KW_RBTYPE, KW_RBID);
            RETVALUE(RFAILED);
         }

         amHdr->li[liCnt++] = li;
         totalSz += li;
      }
      else
      {
         MSPD_ERR("[%d][%d][%d] PDU is NULL\n", KW_UEID, KW_RBTYPE, KW_RBID);
         RETVALUE(RFAILED);
      }
   }
#ifdef MSPD
   if(liCnt > ysMsAmmUlMaxLiCntRcvd)
   {
      ysMsAmmUlMaxLiCntRcvd = liCnt;
   }
#endif
   /*ccpu00122597:PDU is dropped if liCnt exceeds KW_MAX_LI*/
   if(e && pdu && (liCnt >= KW_MAX_LI))
   {
      KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
                  (_kwPBuf," liCount exceeded the maximum count \n"));
      MSPD_ERR("[%d][%d][%d] PDU dropped as LI count exceeded max supported\n", 
                KW_UEID, KW_RBTYPE, KW_RBID);
      RETVALUE(RFAILED);
   }
   
   if (liCnt)
      amHdr->numLi = liCnt;

   if (liCnt % 2)
   {
      pduSz -= ( liCnt + ((liCnt -1)/2) + 1 +2);
   }
   else
   {
      pduSz -= ( liCnt + (liCnt/2) + 2);
   }

   if ( totalSz >= pduSz )
   {
      KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_ERROR | KW_DBGMASK_UL),
             (_kwPBuf, "kwExtractHdr: Corrupted PDU received from the lower\
              layer \n"));
      MSPD_ERR("[%d][%d][%d] Corrupted PDU received from lower layer\n", 
                KW_UEID, KW_RBTYPE, KW_RBID);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief Private handler to mark a retx PDU for further retransmission
 *
 * @b Description 
 *        This function sets a retx PDU that has not been ACKed in the   
 *        received Status PDU for futher retransmission. If the retransmission
 *        limit is reached, it releases the retx PDU and informs the higher
 *        layers about the same.
 *
 * @param[in]  retx     retransmit PDU to be removed
 * @param[in]  rbCb     Radio Bearer Control Block
 *
 *  @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwMrkRetxPdu
(
KwRbCb        *rbCb,
KwRetx        *retx
)
#else
PRIVATE Void kwMrkRetxPdu(rbCb, retx)
KwRbCb        *rbCb;
KwRetx        *retx;
#endif
{
   TRC2(kwMrkRetxPdu)
   if (AMDL.isRbstalled == TRUE)
   {
      RETVOID;
   }
   if (retx->wtForAck == TRUE)
   {
      if (++retx->retxCnt > AMDL.maxRetx)
      {
         KW_DIAG_LVL0(0x60100001, KW_DIAG_UEID, rbCb->rlcId.ueId,
                 "Reached Maximum Retry for a SDU, RAB-Id = %d, RAB-Type = %d",
                  rbCb->rlcId.rbId, rbCb->rlcId.rbType, 0, 0);
#ifdef MSPD
         ysMsAmmDlRetxs[retx->retxCnt - 1]++;
         ysMsNumMaxReTx++;
#endif
         /* RLC_DL_MAX_RETX fix */
         if (TRUE != rbCb->m.am.amDl.isRbstalled)
         {
            rbCb->m.am.amDl.isRbstalled = TRUE;
            /* MS FIX: Fix for negative BO */
            //rbCb->m.am.amDl.bo = 0;
            //rbCb->m.am.amDl.cntrlBo = 0;
            //kwAmmAddRetxBo(&AMDL, 0);
            KW_AM_SND_DSTA_RSP(); 
            kwAmmSndStaInd(rbCb, retx);
#ifdef SIM_MAXRETX
            rbCb->tempStaPduCnt = 0;
#endif
         }
         RETVOID;
      }
      retx->wtForAck = FALSE;
      kwAmmAddRetxBo(&AMDL, retx->segSz);
      kwAmmAddEstHdrSize(&AMDL,  retx->hdrSz);
      if (AMDL.nxtRetx == NULLP)
      {
         AMDL.nxtRetx = retx;
      }
#ifdef MSPD
      ysMsAmmDlRetxs[retx->retxCnt - 1]++;
#endif
   }
   RETVOID;
}

/**
 *
 * @brief Private handler to send higher layers data confirmation
 *
 * @b Description 
 *        This function sends higher layers data confirmation for SDUs which
 *        have been successfully delivered to the peer RLC entity.
 *
 * @param[in]  rbCb     Radio Bearer Control Block
 * @param[in]  sn       SN that is successfully delivered to the peer
 * @param[in]  sduLst   List of SDUs that were part of the PDU
 * @param[in]  numSdu   Number of SDUs in the list
 *
 *  @return Void 
 *
 */
#ifdef ANSI
Void kwAmmChkSduMap
(
KwRbCb                  *rbCb,
KwSn                    sn,
KwSduMap                *sduLst,
U8                      numSdu,
U8                      pdcpCfm
)
#else
Void kwAmmChkSduMap(rbCb, sn, sduLst, numSdu, pdcpCfm)
KwRbCb                  *rbCb;
KwSn                    sn;
KwSduMap                *sduLst;
U8                      numSdu;
U8                      pdcpCfm;
#endif
{
   U8                   i;
   KwSdu                *sdu;
#ifdef MSPD_MLOG_NEW   
   U32                  t = GetTIMETICK();
#endif
   
   TRC2(kwAmmChkSduMap)

   for (i = 0; i < numSdu; i++)
   {
      sdu = sduLst[i].sdu;
      sdu->mode.am.ackedPdus++;
      /* [ccpu00126886]: Fix for double de-allocation, The order of the */
      /* condition is reversed to avoid a race condition with DDATREQ   */
      /* preparation in core 2.                                         */
      if ((sdu->sduSz == 0) && (sdu->mode.am.numPdu == sdu->mode.am.ackedPdus))
      {
         /* Send DatCfm for this sdu */
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DL),
                (_kwPBuf, "kwAmmChkSduMap:DatCfm to PDCP for rb %d type %d\n",
                rbCb->rlcId.rbId, rbCb->rlcId.rbType));
         if (pdcpCfm == KW_AMM_SEND_PDCP_CFM)
         {
            kwAmmSndDatCfm(rbCb, sdu->mode.am.sduId);
         }
         //MSPD_DBG("[%d][%d][%d] Freeing sduId %d\n", KW_UEID, KW_RBTYPE, KW_RBID, sdu->mode.am.sduId);
         KW_FREE_BUF(sdu->mBuf);
         KW_FREE(sdu, sizeof(KwSdu));
      }
   }

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_RLC_CHK_SDU_MAP , RESOURCE_LARM, t, GetTIMETICK());
#endif
   RETVOID;
}

/**
 *
 * @brief
 *
 * @b Description 
 *        This function should be called up on a trigger at which it is safe
 *        to release a set of PDUs without affecting the TTI response.
 *
 *  @return Void 
 *
 */
#ifdef ANSI
PUBLIC Void kwAmmDlRlsPdusSet
(
KwRbCb                  *rbCb,
U8                      *numOfTxReTxCnt
)
#else
PUBLIC Void kwAmmDlRlsPdusSet(rbCb, numOfTxReTxCnt)
KwRbCb                  *rbCb;
U8                      *numOfTxReTxCnt;
#endif
{
   KwTx                 *pdu;
   KwRetx               *retx;

   TRC2(kwAmmDlRlsPdusSet)
   
   kwAmmDlGetTxPduForRelease(rbCb, &pdu);
   while (pdu)
   {
      //MSPD_DBG_RLC("[%d][%d]Releasing PDU with SN = %d\n", rbCb->rlcId.ueId,
      //                                   rbCb->rlcId.ueId, pdu->amHdr.sn);
      kwAmmDlRlsTxPdu(rbCb, pdu, KW_AMM_SEND_PDCP_CFM);
      (*numOfTxReTxCnt)++;
      if (*numOfTxReTxCnt > KW_AM_MAX_PDUS_RLS)
      {
         RETVOID;
      }
      kwAmmDlGetTxPduForRelease(rbCb, &pdu);
   }

   /* Now free Retransmission list and send data confirm */
   kwAmmDlGetRetxPduForRelease(rbCb, &retx);
   while (retx)
   {
      // MSPD_DBG_RLC("[%d][%d]Release RETX PDU with SN = %d\n",
      //              rbCb->rlcId.ueId, rbCb->rlcId.ueId, retx->amHdr.sn);
      kwAmmDlRlsReTxPdu(rbCb, retx, KW_AMM_SEND_PDCP_CFM);
      (*numOfTxReTxCnt)++;
      if (*numOfTxReTxCnt > KW_AM_MAX_PDUS_RLS)
      {
         RETVOID;
      }
      kwAmmDlGetRetxPduForRelease(rbCb, &retx);
   }
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlProcessStaPdus
(
Void
)
#else
PRIVATE Void kwAmmDlProcessStaPdus()
#endif
{
   KwRbCb               *rbCb;
   Buffer               *pdu;
   U8                   fByte;

   kwAmmDlGetRbFrmStatusPduLst(&rbCb, &pdu, &fByte);
   if ((rbCb != NULLP) && (pdu != NULLP))
   {
      //if (!(rbCb->rbOp & 0x0f))
      if (0 == rbCb->rbOp)
      {
         kwAmmDlProcessStatusPdu(rbCb, pdu, &fByte);
         KW_FREE_BUF(pdu);
#ifdef MSPD
         ysMsAmmStaPduCounter--;
#endif
      }
      else
      {
         /* The RB is already added to PDU release list and will be moved */
         /* to delete/reestablish list                                    */
         //MSPD_DBG("[%d][%d] Not processing as marked for delete/reest\n",
         //                             rbCb->rlcId.ueId, rbCb->rlcId.rbId);
         KW_FREE_BUF(pdu);
#ifdef MSPD
         ysMsAmmStaPduCounter--;
#endif
      }
   }
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlRlsPdus
(
Void
)
#else
PRIVATE Void kwAmmDlRlsPdus()
#endif
{
   KwRbCb               *rbCb;
   U8                   numOfTxRetxCnt = 0;
   CmLList              *tempLst;

   kwAmmDlGetRbForPduRls(&rbCb);
   if (rbCb != NULLP)
   {
      //MSPD_DBG("RbCb retrieved for Pdu release rbOp %d\n", rbCb->rbOp);
      if (0 == rbCb->rbOp)
      {
         kwAmmDlRlsPdusSet(rbCb, &numOfTxRetxCnt);
#ifdef MSPD
         if(rbCb->rlcId.rbType == 1)
         {
            ysMsAmmNumSduQCount = AMDL.sduQ.count;
            ysMsAmmNumRetxLstCnt = AMDL.retxLst.count;
            ysMsAmmNumStaPduRbLstCount = kwCb.staPduRbLst.count;
            ysMsAmmNumPduRlsRbLstCount = kwCb.pduRlsRbLst.count;
            ysMsAmmNumReestablishRbLstCount = kwCb.reestablishRbLst.count;
            ysMsAmmNumDeleteRbLstCount = kwCb.deleteRbLst.count;
            ysMsAmmNumToBeRlsTxLstCount = AMDL.toBeRlsTxLst.count;
            ysMsAmmNumToBeRlsReTxLstCount = AMDL.toBeRlsReTxLst.count;
            ysMsAmmPjNumEntries = rbCb->pjRbCb->dlCb.txBuf.numEntries;
            ysMsAmmPjDatQ = rbCb->pjRbCb->dlCb.txBuf.datQ->count;
         }
#endif
         if ( (numOfTxRetxCnt >= KW_AM_MAX_PDUS_RLS) && (0 == rbCb->rbOp) )
         {
            kwAmmDlAddRbToPduRlsLst(rbCb);
         }
      }
      else
      {
      }
   }
   RETVOID;
}

/**
 * @brief Handler to process the message posted by RLC itself.
 *
 * @b Description
 *       This is the low priority message to be processed to 
 *       clear Transmission and retransmission buffer in RLC.
 *
 *  @return Void
 */
#ifdef ANSI
PUBLIC Void kwAmmDlProcessSelfMsg
(
)
#else
PUBLIC Void kwAmmDlProcessSelfMsg()
#endif
{
#ifdef MSPD_MLOG_NEW
   U32        startTick, endTick;

   startTick = GetTIMETICK();
#endif

   TRC2(kwAmmDlProcessSelfMsg);

   /* Set the flag to indicate that there is no message currently posted  */
   kwCb.dlSelfMsgPosted = 0;

   /* Process re-establishment and delete list as the higher priority     */
   /* tasks                                                               */

   /* Process status PDUs and also release of PDUs that are acked         */
   kwAmmDlProcessStaPdus();
   kwAmmDlRlsPdus();

   /* Post self message to RLC if Rb list is not empty                    */
   if ((NULLP != kwCb.pduRlsRbLst.first) || (NULLP != kwCb.staPduRbLst.first))
   {
      kwAmmDlPostSelfMsg();
   }
#ifdef MSPD_MLOG_NEW
   endTick = GetTIMETICK();
   MLogTask(PID_RLC_AM_PDU_RLS , RESOURCE_LARM, startTick, endTick);
#endif

}

#ifdef ANSI
PRIVATE Void kwAmmDlRlsTxPdu
(
KwRbCb                  *rbCb,
KwTx                    *tx,
U8                      pdcpCfm
)
#else
PRIVATE Void kwAmmDlRlsTxPdu(rbCb, tx, pdcpCfm)
KwRbCb                  *rbCb;
KwTx                    *tx;
U8                      pdcpCfm;
#endif
{
   //MSPD_DBG("2 **** SN %d KwTx %x pdu %x pduSz %d numSdu %d ****\n", 
   //          tx->amHdr.sn, tx, tx->pdu, tx->pduSz, tx->numSdu);
   kwAmmChkSduMap(rbCb, tx->amHdr.sn, tx->sduMap, tx->numSdu, pdcpCfm);
   KW_FREE_BUF(tx->pdu);
   KW_FREE(tx, sizeof(KwTx));
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlAddTxPduToRlsLst
(
KwRbCb                  *rbCb,
KwTx                    *pdu
)
#else
PRIVATE Void kwAmmDlAddTxPduToRlsLst(rbCb, pdu)
KwRbCb                  *rbCb;
KwTx                    *pdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      cmLListAdd2Tail(&AMDL.toBeRlsTxLst, &(pdu->rlsTxLnk));
      pdu->rlsTxLnk.node = (PTR)(pdu);
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlGetTxPduForRelease
(
KwRbCb                  *rbCb,
KwTx                    **pdu
)
#else
PRIVATE Void kwAmmDlGetTxPduForRelease(rbCb, pdu)
KwRbCb                  *rbCb;
KwTx                    **pdu;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      if (AMDL.toBeRlsTxLst.first == NULLP)
      {
         (*pdu) = NULLP;
      }
      else
      {
         (*pdu) = (KwTx *)(AMDL.toBeRlsTxLst.first->node);
         (*pdu)->rlsTxLnk.node = NULLP;
         cmLListDelFrm(&AMDL.toBeRlsTxLst, AMDL.toBeRlsTxLst.first);
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

/**
 *
 * @brief Private handler to mark a PDU successful.
 *
 * @b Description 
 *        This function is called when we receive a STATUS pdu that marks
 *        a PDU as successful. It releases the PDU from RLC entity and 
 *        informs PDCP of successful SDUs delivered as a result of this PDU.
 *
 * @param[in]  rbCb     Radio Bearer Control Block
 * @param[in]  sn       SN that is successfully delivered to the peer
 *
 *  @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmDlRemTxPdu
(
KwRbCb            *rbCb,
KwSn              sn
)
#else
PRIVATE Void kwAmmDlRemTxPdu(rbCb, sn)
KwRbCb            *rbCb;
KwSn              sn;
#endif
{
   TRC2(kwAmmDlRemTxPdu)

   //MSPD_DBG("[%d][%d]Removing TX PDU for SN = %d\n", KW_UEID,
   //          KW_RBID, sn);
   kwAmmDlAddTxPduToRlsLst(rbCb, AMDL.txBuf[sn]);
   AMDL.txBuf[sn] = NULLP;

   RETVOID;
}

/**
 *
 * @brief Private handler to process the status PDU
 *
 * @b Description
 *        Private handler invokded by kwAmmProcessPdus to process the 
 *        control PDU (status report) received from its peer RLC entity.
 *
 *        - Decode the values from the received control pdu and flush pdu 
 *          from txBuf if it receives ack for the pdu, else move it to the 
 *          retxBuf.
 *        - Flush the pdu from retxBuf if ack is received for all byte 
 *          segments of the pdu else it remains in retx buffer until it 
 *          receives ack / it reaches maxRetx threashold value.
 *        - Before cleaning up the PDUs from txBuf/retxBuf, DatCfm will be 
 *          sent to PDCP for each SDU using the SDU to pdu (SN) mapping 
 *          available in the sduQ 
 *
 * @param[in]  rbCb     RB control block 
 * @param[out] cntrlPdu Control PDU received from MAC
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmDlProcessStatusPdu
(
KwRbCb             *rbCb,
Buffer             *cntrlPdu,
U8                 *fByte
)
#else
PRIVATE S16 kwAmmDlProcessStatusPdu(rbCb, cntrlPdu, fByte)
KwRbCb             *rbCb;
Buffer             *cntrlPdu;
U8                 *fByte;
#endif
{
   KwRetx          *retx   = NULLP;
   U8              e1      = 0;
   KwSn            ackSn   = 0;
   KwSn            mSn     = 0;
   KwSn            mAckSn  = 0;
   KwSn            mTxSn   = 0;
   S32             retxBo  = 0;
   S16             ret     = ROK;
   CmLList         *retxNode;
   KwExtHdr        hdrInfo;
   KwSn            mVts;

#ifdef MSPD_MLOG_NEW
   U32             startTick, endTick;
#endif

   TRC2(kwAmmDlProcessStatusPdu)

#ifdef MSPD_MLOG_NEW
   startTick = GetTIMETICK();
#endif
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwAmmDlProcessStatusPdu(rbCb, cntrlPdu, fByte) \n"));
#ifdef MSPD
   ysMsNumCtrlPdus++;
#endif

   KW_MEM_ZERO(&hdrInfo, sizeof(KwExtHdr));

   AMDL.numSduPdu = 0;
   AMDL.numSeg    = 0;
   retxBo = AMDL.retxBo;

   /* Extract the Control PDU */ 
   hdrInfo.hdr  = (*fByte << 1);
   hdrInfo.pLen = 4;

   /* D/C has been shifted in the calling function */
   if (hdrInfo.hdr & 0xE0)
   {
      KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwAmmDlProcessStatusPdu: Rcd CPT value, discard cntrlPDU\n"));
#ifdef MSPD
      ysMsNumErrCtrlPdus++;
      //stop_printf("Erroroneous control pdu\n");
#endif
      RETVALUE(ROK);
   }

   /* For CPT */
   hdrInfo.hdr = hdrInfo.hdr << KW_CPT_LEN; 

   hdrInfo.len = KW_SN_LEN;
   kwExtractElmnt(cntrlPdu, &hdrInfo);
   ackSn = hdrInfo.val;

   hdrInfo.len = KW_E1_LEN;
   kwExtractElmnt(cntrlPdu, &hdrInfo); 
   e1 = (U8)hdrInfo.val; 
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
                    (_kwPBuf, "kwAmmDlProcessStatusPdu: ACK SN = %d \n", ackSn));
   MSPD_DBG("[%d][%d][%d] ACK SN %d vtA %d vtS %d\n", KW_UEID, KW_RBTYPE, 
                 KW_RBID, ackSn, AMDL.vtA, AMDL.vtS);
   MODAMT(ackSn, mAckSn, AMDL.vtA);
   MODAMT(AMDL.txSn, mTxSn, AMDL.vtA);
   MODAMT(AMDL.vtS, mVts, AMDL.vtA);

   if (mAckSn > mVts)
   {
#ifdef MSPD
      ysMsNumErrCtrlPdus++;
#endif
      MSPD_ERR("[%d][%d][%d] Received erroneous status pdu\n", 
                KW_UEID, KW_RBTYPE, KW_RBID);
      RETVALUE(ROK);
   }

   /* Stop PollRetx Tmr */
   /* WORKAROUND for Stoping PollRetxTmr in Core 2*/
   /* KW_CHK_FOR_POLLSN(rbCb, mAckSn);  */

   /* Set the first node in retx list to retxNode */
   retxNode = AMDL.retxLst.first;

   /* If NACK exists in control PDU */
   if (e1)
   {
      U8     e2      = 0;
      Bool   first   = TRUE; 
      KwSn   sn      = 0;
      KwSn   soStart = 0;
      KwSn   soEnd   = 0;
      KwSn   nackSn  = 0;
      KwSn   mNackSn = 0;
      KwSn   vtA     = AMDL.vtA;

      /* For ACKs and NACKs */
      while (e1)
      {
         hdrInfo.len = KW_SN_LEN;
         kwExtractElmnt(cntrlPdu, &hdrInfo);
         nackSn = hdrInfo.val;

         hdrInfo.len = KW_E1_LEN;
         kwExtractElmnt(cntrlPdu, &hdrInfo); 
         e1 = (U8)hdrInfo.val;

         hdrInfo.len = KW_E1_LEN;
         kwExtractElmnt(cntrlPdu, &hdrInfo);
         e2 = (U8) hdrInfo.val;
         if (first)
         {
            vtA   = nackSn;
            first = FALSE;
         }
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
                (_kwPBuf, "kwAmmDlProcessStatusPdu: NACK SN = %d \n", nackSn));

         if (e2)
         {
            hdrInfo.len = KW_SO_LEN;
            kwExtractElmnt(cntrlPdu, &hdrInfo);
            soStart = hdrInfo.val;

            kwExtractElmnt(cntrlPdu, &hdrInfo);
            soEnd = hdrInfo.val;
            KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
               (_kwPBuf, "kwAmmDlProcessStatusPdu: soStart and soEnd = %d %d \n",
                    soStart, soEnd));
         }
         /* txSn for whatever reason points to the last processed */
         /* PDU and thereby represents as the first PDU to be     */
         /* taken care of from the transmitted PDU array.         */
         sn = AMDL.txSn;
         AMDL.txSn = (nackSn + 1) % 1024;
         MODAMT(nackSn, mNackSn, AMDL.vtA);
         if ((mNackSn > mAckSn) || (mNackSn >= mVts))
         {
            /* Erroneous NACK_SN, dont process the current Nack SN */
            MSPD_ERR("[%d][%d][%d] Received erroneous NACK SN (%d), ACK SN %d\
                      VTA %d VTS %d\n", KW_UEID, KW_RBTYPE, KW_RBID, nackSn,
                      ackSn, AMDL.vtA, AMDL.vtS);
            continue;
         }
#ifdef MSPD
         if(e2)
            ysMsNumPartRlcNacks++;
         else
            ysMsNumRlcNacks++;
#endif
         //MSPD_DBG_RLC("[%d][%d]NACK SN = %d  e2 %d soStart, soEnd = %d %d \n",
         //              KW_UEID, KW_RBID, nackSn, e2,soStart, soEnd);
          
         /* Clear the acked SNs from the retx list */
         while (retxNode)
         {
            retx = (KwRetx *)(retxNode->node);
            MODAMT(retx->amHdr.sn, mSn, AMDL.vtA);
            if (mSn < mNackSn)
            {
               retxNode = retxNode->next;
               kwAmmDlRemRetxPdu(rbCb, retx);
            }
            else
            {
               break;
            }
         }

         /* For the ACKs */ 
         MODAMT(sn, mSn, AMDL.vtA);
         while (mSn < mNackSn)
         {
            if ((mSn >= mTxSn) && (AMDL.txBuf[sn] != NULLP)) 
            {
               KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
                      (_kwPBuf, 
                      "kwAmmDlProcessStatusPdu: Handle ACK for PDU with sn = %d\n",
                       sn));
               kwAmmDlRemTxPdu(rbCb, sn);
            }
            mSn++; sn = (sn + 1) % 1024;
         }

         /* Now process the NACK_SN received. Now the NACK_SN is    */
         /* either the first element of RETX or is in TX array      */
         /* To remove the remaining acks from the pdu byte segments */
         if (AMDL.txBuf[nackSn] != NULLP)
         {
            KwAmDl *amDl = &(rbCb->m.am.amDl); 
            /* This is the case of a PDU to be added to retx list       */
            //MSPD_DBG("[%d][%d] Adding SN %d to retx list\n",
            //                   rbCb->rlcId.ueId, rbCb->rlcId.rbId, sn);
            ret = kwUpdFrmTxtoRetx(amDl, &retx, nackSn);
            if (ret != ROK)
            {
               MSPD_ERR("[%d][%d][%d] Failed in updating from Tx to Retx\n", 
                         KW_UEID, KW_RBTYPE, KW_RBID);
               RETVALUE(RFAILED);
            }
            retxNode = retx->lstEnt.next;
            continue;
         }
         while (retxNode)
         {
            retx = (KwRetx *)(retxNode->node);
            if (retx->amHdr.sn != nackSn)
            {
               break;
            }
            if ((e2) && (retx->soEnd < soStart))
            {
               KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
                      (_kwPBuf,
                      "kwAmmDlProcessStatusPdu: Handle ACK for byte seg, Its sn,"
                      " soStart  and soEnd = %d, %d, %d\n",
                      sn, retx->amHdr.so, retx->soEnd)); 
               retxNode = retxNode->next;
               kwAmmDlRemRetxPdu(rbCb, retx);
            }
            else if (((e2) && (retx->soEnd <= soEnd)) || (!e2))
            {
               /* This case covers the NACKED segments and also the case */
               /* when there are segments and the entire SN is nacked.   */
               /* This case also covers the case of nonsegmented retx PDU*/
               /* Mark the retx PDU we found for further retransmission  */
               retxNode = retxNode->next;
               kwMrkRetxPdu(rbCb, retx);
            }
            else
            {
               break;
            }
         }
      }

      /* All the PDUs after the last nackSn up to the ackSn in the report */
      /* are acked PDUs. Release all those PDUs                           */
      kwUpdRemPduAck(rbCb, mAckSn, retxNode);

      /* Update vtA */ 
      AMDL.vtA = vtA;
      AMDL.vtMs = (AMDL.vtA + 512) % 1024;
   }
   else
   {
      /* For All ACKs */
      KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
            (_kwPBuf, "kwAmmDlProcessStatusPdu: Received All ACKS \n"));

      /* For the remaining ACKs  after last nackSn */
      kwUpdRemPduAck(rbCb, mAckSn, retxNode);
      AMDL.vtA = ackSn;
      AMDL.vtMs = (AMDL.vtA + 512) % 1024;
   }
   /* Set the first node in the retx list as the retransmission PDU */
   KW_LLIST_FIRST_RETX(AMDL.retxLst, AMDL.nxtRetx);

   AMDL.txSn = ackSn;

   /* BO update */
   if (retxBo != AMDL.retxBo)
   {
      KW_AM_SND_DSTA_RSP();
   }

   /* Add RbCb entry in to pduRlsRbLst in kwCb to process these Rb later */
   /* point of time, i.e when ever CPU gets time rlc process this rbCb   */
   kwAmmDlAddRbToPduRlsLst(rbCb);
   kwAmmDlPostSelfMsg();

#ifdef MSPD_MLOG_NEW
   endTick = GetTIMETICK();
   MLogTask(PID_RLC_AM_STA_PDU , RESOURCE_LARM, startTick, endTick);
#endif

   RETVALUE(ROK);
}

/**
 *
 * @brief Private handler to release all stored segments
 *
 * @b Description
 *        Private handler invokded by kwPlcPduInRecBuf to release the 
 *        stored segements in case a complete PDU is received later.
 *
 * @param[in]  recBuf   Buffer that stores a received PDU or segments
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmUlRlsAllSegs
(
KwAmRecBuf        *recBuf
)
#else
PRIVATE Void kwAmmUlRlsAllSegs(recBuf)
KwAmRecBuf        *recBuf;
#endif
{
   KwSeg          *seg;

   TRC2(kwAmmUlAllSegs)

   KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while (seg != NULLP)
   {
      KW_FREE_BUF(seg->seg);
      cmLListDelFrm(&(recBuf->segLst),&(seg->lstEnt));
      KW_FREE(seg, sizeof(KwSeg));
      KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   }
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmUlRlsRecBuf
(
KwAmRecBuf        *recBuf
)
#else
PRIVATE Void kwAmmUlRlsRecBuf(recBuf)
KwAmRecBuf        *recBuf;
#endif
{
   TRC2(kwAmmUlRlsRecBuf)

   kwAmmUlRlsAllSegs(recBuf);
   KW_FREE_BUF(recBuf->pdu);
   KW_FREE(recBuf, sizeof(KwAmRecBuf));
   RETVOID;
}

/**
 *
 * @brief Private handler to store the received segment
 *
 * @b Description
 *        Private handler invokded by kwPlcPduInRecBuf to add a received
 *        segment in receiver buffer for a RBCB.
 *        - It is responsible for detecting duplicate segments
 *        - Adding it at appropriate position in the received buffer
 *        - Calling ExpByteSeg to set expSo field in the receiver buffer
 *
 * @param[in]  rbCb     Radio Bearer Contro Block
 * @param[in]  amHdr    AM Header received
 * @param[in]  pdu      Buffer received other than the headers
 * @param[in]  pduSz    size of the PDU buffer received
 *
 *  @return  Bool
 *   -#TRUE  Successful insertion into the receiver buffer
 *   -#FALSE Possibly a duplicate segment
 */
#ifdef ANSI
PRIVATE Bool kwAmmAddRcvdSeg
(
KwRbCb            *rbCb,
KwAmHdr           *amHdr,
Buffer            *pdu,
U16               pduSz
)
#else
PRIVATE Bool kwAmmAddRcvdSeg(rbCb, amHdr, pdu, pduSz)
KwRbCb            *rbCb;
KwAmHdr           *amHdr;
Buffer            *pdu;
U16               pduSz;
#endif
{
   KwAmRecBuf     *recBuf;
   KwSeg          *seg;
   KwSeg          *tseg;
   U16            soEnd;          /* Holds the SoEnd of received segment */
   U16            expSo = 0;      /* Expected SO */

   TRC2(kwAmmAddRcvdSeg)

   soEnd = amHdr->so + pduSz - 1;

   if (AMUL.recBuf[amHdr->sn] == NULLP)
   {
      KW_ALLOC(recBuf, sizeof(KwAmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (recBuf == NULLP)
      {
         KWLOGERROR(ERRCLS_ADD_RES, EKW009, (ErrVal)0,
                  "kwAmmAddRcvdSeg: Allocation failed for recBuf \n");
         KW_FREE_BUF(pdu);
         RETVALUE(FALSE);
      }
#endif /* ERRCLASS & ERRCLS_RES */
      AMUL.recBuf[amHdr->sn] = recBuf;
      recBuf->amHdr.rf = 1;
      recBuf->amHdr.sn = amHdr->sn;
   }
   else
   {
      recBuf = AMUL.recBuf[amHdr->sn];
   }
   if (recBuf->allRcvd == TRUE)
   {
      MSPD_ERR("[%d][%d][%d] All segments have already been received, printing\
                header sn %d, rf %d lsf %d numLi %d\n", KW_UEID, KW_RBTYPE,
                KW_RBID, recBuf->amHdr.sn, recBuf->amHdr.rf, recBuf->amHdr.lsf,
                recBuf->amHdr.numLi);
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }
   
   /* Move past the segments that are different than the */
   /* one received.                                      */
   KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while ((seg != NULLP) && (seg->amHdr.so < amHdr->so))
   {
      expSo = seg->amHdr.so + seg->segSz;
      KW_LLIST_NEXT_SEG(recBuf->segLst, seg);
   }
   //MSPD_DBG_RLC("[%d][%d] Sn[%d] ExSo %d ExSo1 %d RecSo %d soEnd %d lsf"
   //             " %d rf %d allRcvd %d\n", KW_UEID, KW_RBID,
   //             amHdr->sn, expSo, recBuf->expSo, amHdr->so, soEnd, 
   //             amHdr->lsf, amHdr->rf, recBuf->allRcvd);

   /* The received segment should start after the end of previous seg */
   if (expSo > amHdr->so)
   {
      /* This is a duplicate segment */
      MSPD_ERR("[%d][%d][%d] Received duplicate seg, sn %d\n", KW_UEID,
                KW_RBTYPE, KW_RBID, amHdr->sn);
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }

   if ((seg) && (seg->amHdr.so <= soEnd))
   {
      /* This is a duplicate segment */
      MSPD_ERR("[%d][%d][%d] Received duplicate seg, sn %d\n", KW_UEID,
                KW_RBTYPE, KW_RBID, amHdr->sn);
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }

   /* If we have come this far, we have to add this segment to the   */
   /* reception buffer as we either have eliminated duplicates or    */
   /* have found none.                                               */
   KW_ALLOC(tseg, sizeof(KwSeg));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (tseg == NULLP)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW010, (ErrVal)0,
                  "kwAmmAddRcvdSeg: Allocation failed for seg \n");
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   tseg->seg = pdu;
   tseg->segSz = pduSz; 
   tseg->soEnd = soEnd;
   KW_MEM_CPY(&tseg->amHdr, amHdr, sizeof(KwAmHdr));
   if (seg == NULLP)
   {
      cmLListAdd2Tail(&recBuf->segLst, &tseg->lstEnt);
   }
   else
   {
      recBuf->segLst.crnt = &seg->lstEnt;
      cmLListInsCrnt(&recBuf->segLst, &tseg->lstEnt);
   }
   tseg->lstEnt.node = (PTR)tseg;
   kwAmmUpdExpByteSeg(&AMUL, tseg);

   //MSPD_DBG_RLC("[%d][%d]END - Sn[%d] ExSo %d ExSo1 %d RecSo %d soEnd %d lsf"
   //             " %d rf %d allRcvd %d\n", KW_UEID, KW_RBID,
   //             amHdr->sn, expSo, recBuf->expSo, amHdr->so, soEnd, 
   //             amHdr->lsf, amHdr->rf, recBuf->allRcvd);
   RETVALUE(TRUE);
}

/**
 *
 * @brief Private handler to place the PDU in the reception buffer 
 *
 * @b Description 
 *        This function checks if the received PDU's SN falls within the
 *        receiving window, after which it places the same in the reception 
 *        buffer if its not a duplicate.
 *
 * @param[in]  pdu      Received PDU
 * @param[out] amUl     AM UL Info 
 *
 *  @return Bool 
 *      -# TRUE 
 *      -# FALSE 
 *
 */
#ifdef ANSI
PRIVATE Bool kwPlcPduInRecBuf
(
Buffer             *pdu,
KwRbCb             *rbCb,
KwAmHdr            *amHdr
)
#else
PRIVATE Bool kwPlcPduInRecBuf(pdu, rbCb, amHdr)
Buffer             *pdu;
KwRbCb             *rbCb;
KwAmHdr            *amHdr;
#endif
{
   KwSn            sn      = 0;
   Bool            snInWin = 0;
   MsgLen          pduSz   = 0;
   KwAmUl          *amUl   = &(rbCb->m.am.amUl);

   TRC2(kwPlcPduInRecBuf)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwPlcPduInRecBuf(pdu, rbCb, amHdr)\n"));

   sn = amHdr->sn;
   SFndLenMsg(pdu, &pduSz);
   kwCb.genSts.bytesRecv += pduSz;

   /* Check if SN falls within reordering window */
   KW_AM_CHK_SN_IN_WINDOW(sn, amUl, snInWin);
   if (!snInWin)
   {
      KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
            (_kwPBuf, "kwPlcPduInRecBuf: SN  %d falls outside the window \n",
              sn));
      kwCb.genSts.unexpPdusRecv++;
      MSPD_ERR("[%d][%d][%d]Out Side window SN %d Exp SN %d," "vr(Mr %d, "
                   "Ms %d, H %d, X %d) NumLi %d FI %d RF %d p %d so %d\n", 
                   KW_UEID,KW_RBTYPE, KW_RBID, sn, amUl->vrR,amUl->vrMr,
                   amUl->vrMs, amUl->vrH, amUl->vrX, amHdr->numLi, amHdr->fi,
                   amHdr->rf, amHdr->p, amHdr->so);
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }
#ifdef MSPD  /* WORKAROUND */
   /* This is a work around for the invalid PDU that affects RLC and PDCP */
   /* state variables and causes UL stall. This happens because we are    */
   /* likely receiving an incorrectly decoded PDU which escapes CRC check.*/
   /* We drop such a PDU by checking if we received a large jump in SN    */
   /* when the last received PDU is not that far away in time. If the SN  */
   /* of the received PDU is much larger than vrH and is not separated in */
   /* time by much, we will ignore such a PDU                             */
   {
      extern U16        macGT;
      U16               sfDiff;
      U16               mVrH;
      U16               mSn;

      sfDiff = AMUL.lastGt > macGT ? ((10249 - AMUL.lastGt) + macGT) :
                                      (macGT - AMUL.lastGt);
      MODAMR(amHdr->sn, mSn,  AMUL.vrR);
      MODAMR(AMUL.vrH,  mVrH, AMUL.vrR);
      if ((mSn > (mVrH + 40)) && (sfDiff < 20))
      {
         MSPD_DBG("[%d][%d][%d]<workaround>Out Side window SN %d Exp SN %d, vr(Mr %d,"
                      "Ms %d, H %d, X %d) NumLi %d FI %d RF %d p %d so %d\n", 
                      KW_UEID,KW_RBTYPE, KW_RBID, sn, amUl->vrR,amUl->vrMr,
                      amUl->vrMs, amUl->vrH, amUl->vrX, amHdr->numLi, amHdr->fi,
                      amHdr->rf, amHdr->p, amHdr->so);
         KW_FREE_BUF(pdu);
        /* stop_printf("[%d][%d][%d]Out Side window SN %d Exp SN %d, vr(Mr %d,"
                      "Ms %d, H %d, X %d) NumLi %d FI %d RF %d p %d so %d\n", 
                      KW_UEID,KW_RBTYPE, KW_RBID, sn, amUl->vrR,amUl->vrMr,
                      amUl->vrMs, amUl->vrH, amUl->vrX, amHdr->numLi, amHdr->fi,
                      amHdr->rf, amHdr->p, amHdr->so);	 */
         RETVALUE(FALSE);
       }
       AMUL.lastGt = macGT;
   }
#endif

   if (amHdr->rf == 0)
   {
      KwAmRecBuf      *recBuf;
      //MSPD_DBG_RLC("[%d][%d][%d]Recved SN %d Exp SN %d,vr(Mr %d, Ms %d, H %d, X %d) NumLi %d FI %d RF %d p %d" 
      //                " so %d\n", 
      //                KW_UEID,KW_RBTYPE, KW_RBID, sn, amUl->vrR,amUl->vrMr, amUl->vrMs, amUl->vrH, amUl->vrX,
      //                amHdr->numLi, amHdr->fi, amHdr->rf, amHdr->p, 
      //                amHdr->so);

      /* We received a complete PDU. Either we already have it, in which */
      /* case we just ignore the new PDU and discard it. Otherwise,      */
      /* store the received PDU in the reception buffer                  */
      /* This log is added to catch a crash                              */
      if(CM_LTE_SRB == rbCb->rlcId.rbType)
      {
         //MSPD_DBG_RLC("<RD>[%d][%d][%d] Ul PDU struct: SN %d recBuf[%d] 0x%x\n",
         //              KW_UEID, KW_RBTYPE, KW_RBID, sn, amHdr->sn, 
         //              amUl->recBuf[amHdr->sn]);
      }
      if (amUl->recBuf[amHdr->sn] == NULLP)
      {
         KW_ALLOC(recBuf, sizeof(KwAmRecBuf));
         if(CM_LTE_SRB == rbCb->rlcId.rbType)
         {
            //MSPD_DBG_RLC("<RD>Allocation done\n");
         }
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (recBuf == NULLP)
         {
            KWLOGERROR(ERRCLS_ADD_RES, EKW009, (ErrVal)0,
                  "kwPlcPduInRecBuf: Allocation failed for recBuf \n");
            KW_FREE_BUF(pdu);
            RETVALUE(FALSE);
         }
#endif /* ERRCLASS & ERRCLS_RES */
         amUl->recBuf[amHdr->sn] = recBuf;
         if(CM_LTE_SRB == rbCb->rlcId.rbType)
         {
            //MSPD_DBG_RLC("<RD>recBuf put in array\n");
         }
      }
      else if (amUl->recBuf[amHdr->sn]->allRcvd != TRUE)
      {
         kwAmmUlRlsAllSegs(amUl->recBuf[amHdr->sn]);
         recBuf = amUl->recBuf[sn];
      }
      else
      {
         kwCb.genSts.unexpPdusRecv++;
         MSPD_ERR("[%d][%d][%d] Duplicate PDU received sn %d\n", KW_UEID, 
                   KW_RBTYPE, KW_RBID, amHdr->sn);
         KW_FREE_BUF(pdu);
         RETVALUE(FALSE);
      }
      recBuf->pdu = pdu;
      recBuf->pduSz = pduSz;
      recBuf->allRcvd = TRUE;
      KW_MEM_CPY(&recBuf->amHdr, amHdr, sizeof(KwAmHdr));
      RETVALUE(TRUE);
   }
   else
   {
      /* We received a segment. We need to add that to the existing */
      /* segments, if any.                                          */
      RETVALUE(kwAmmAddRcvdSeg(rbCb, amHdr, pdu, pduSz));
   }
} /* kwPlcPduInRecBuf */


/**
 *
 * @brief Private handler to trigger status report
 *
 * @b Description
 *        Private handler invokded by kwAmmProcessPdus to check if the 
 *        status report need to be sent, and update the status trigger 
 *        flag accordingly based on status prohibit timer.
 *   
 *        - Check if the received pdu's sn is less than vrMs, set the 
 *          staTrg flag.
 *        - If staProhTmr is not running, calculate cntrlBo, else it'll be
 *          updated at the expiry of staProhTmr.
 *        - Expiry of reOrdTmr also will set staTrg flag.
 *   
 * @param[in]  rbCb     RB control block 
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwTriggerStatus
(
KwRbCb    *rbCb,                       /* AM UL Info */
KwSn      sn,                          /* Received PDU's SN */
Bool      discFlg                      /* Discard flag */
)
#else
PRIVATE Void kwTriggerStatus(rbCb, sn, discFlg)
KwRbCb    *rbCb;                       /* AM UL Info */
KwSn      sn;                          /* Received PDU's SN */
Bool      discFlg;                     /* Discard Flag */
#endif
{
   Bool tmrRunning = FALSE;
   KwSn  tSn,tVrMr, tVrMs;
   KwAmUl *amUl = &(rbCb->m.am.amUl);

   TRC2(kwTriggerStatus)

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwTriggerStatus(rbCb, sn, discFlg)\n"));

   MODAMR(amUl->vrMr, tVrMr, amUl->vrR);
   MODAMR(amUl->vrMs, tVrMs, amUl->vrR);
   MODAMR(sn , tSn, amUl->vrR);

   if ((discFlg) || (tSn < tVrMs) || (tSn >= tVrMr))
   {
      KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
            (_kwPBuf, "kwTriggerStatus: Set Status Trigger \n"));

      amUl->staTrg = TRUE;

      /* Check if staProhTmr is running */
      tmrRunning = kwChkTmr((PTR) rbCb, KW_EVT_AMUL_STA_PROH_TMR);
      //MSPD_DBG_RLC("[%d][%d][%d]Status Triggred SPT Running Status %d, sn %d\n",
      //                KW_UEID, KW_RBTYPE, KW_RBID, tmrRunning, sn);
      if (!tmrRunning)
      {
         kwUpdCntrlBo(amUl);
      }
   }
   RETVOID;
}

/**
 *
 * @brief Private handler to update control BO
 *
 * @b Description
 *  
 *         This function updates the control BO in the AM Uplink structure. 
 *   
 * @param[in]  amUl     AM UL Info 
 *
 * @return  Void
 * 
 */
#ifdef ANSI
PRIVATE Void kwUpdCntrlBo 
(
KwAmUl    *amUl
)
#else
PRIVATE Void kwUpdCntrlBo(amUl)
KwAmUl    *amUl;
#endif
{
   KwSn             sn  = 0;
   KwSn             mSn = 0;
   KwSn             mvrMs = 0;
   U16              seqSo = 0;
   S32              cntrlBo = 0;
   KwSeg            *seg = NULLP;

   TRC2(kwUpdCntrlBo)
   
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwUpdCntrlBo(amUl)\n"));

   cntrlBo += KW_CNTRL_PDU_FIXED_HDRSZ;

   sn = amUl->vrR;

   MODAMR(sn, mSn, amUl->vrR);
   MODAMR(amUl->vrMs, mvrMs, amUl->vrR); 
   while (mSn < mvrMs)
   {
      if (amUl->recBuf[sn] == NULLP)
      {
         /* For NACK_SN, E1 and E2 */
         cntrlBo += 2;
      }
      else
      {
         if ( amUl->recBuf[sn]->amHdr.rf == 0 )
         {
            sn = (sn + 1) % 1024;
            MODAMR(sn, mSn, amUl->vrR);
            continue;
         }
 
         if ( (amUl->recBuf[sn]->amHdr.rf == 1) &&
              (amUl->recBuf[sn]->segLst.count > 0))
         {
            /* For NACK_SN, E1 and E2 */
            cntrlBo += 2;
         } 

         seqSo  = 0;
         
         KW_LLIST_FIRST_SEG(amUl->recBuf[sn]->segLst, seg);
         while (seg != NULLP)
         {
            /* For missing byte segments */
            if (seg->amHdr.so != seqSo)
            {
               /* For soStart and soEnd */
               cntrlBo+= 4;
            }
            seqSo = seg->soEnd + 1;
            KW_LLIST_NEXT_SEG(amUl->recBuf[sn]->segLst, seg);
         }

         /* Check if the last segment is missing */
         KW_LLIST_LAST_SEG(amUl->recBuf[sn]->segLst, seg);
         if ( (seg != NULLP) && (seg->amHdr.lsf != 1) )
         {
            cntrlBo += 2;
         }
      }
      sn = (sn + 1) % 1024;
      MODAMR(sn, mSn, amUl->vrR);
   }

   amUl->cntrlBo = cntrlBo;
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
          (_kwPBuf, "kwUpdCntrlBo: Updated cntrlBo = %ld\n", cntrlBo));
#else
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
          (_kwPBuf, "kwUpdCntrlBo: Updated cntrlBo = %d\n", cntrlBo));
#endif
   RETVOID;
}

/**
 *
 * @brief Private handler to get a portion of SDU
 *
 * @b Description
 *        Private handler invokded by kwAmmProcPduOrSeg to fetch a portion
 *        of SDU that is addressed by the LI at position liCnt. It also
 *        increments liCnt.
 *
 * @param[in]  liCnt    refers to the SDU (portion) to be retrieved
 * @param[in]  amHdr    AM header carries the SDU size through LIs
 * @param[in]  pdu      PDU from which SDU is formed
 * @param[in]  sdub     Updated and returned to the caller
 *
 *  @return  S16
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmGetSdu
(
U16          *liCnt,
KwAmHdr      *amHdr,
Buffer       **pdu,
Buffer       **sdu
)
#else
PRIVATE S16 kwAmmGetSdu
(
U16          *liCnt,
KwAmHdr      *amHdr,
Buffer       **pdu,
Buffer       **sdu
)
#endif
{
   Buffer    *remPdu;

   TRC2(kwAmmGetSdu)

   if ((*liCnt) < amHdr->numLi)
   {
      /* Segment the PDU based on length in LI table */
      SSegMsg(*pdu, amHdr->li[*liCnt], &remPdu);
      *sdu = *pdu;
      *pdu = remPdu;
   }
   else
   {
      /* The entire remaining PDU forms the SDU */
      *sdu = *pdu;
      *pdu = NULLP;
   }
   (*liCnt)++;
   RETVALUE(ROK);
}

/**
 *
 * @brief Private handler to reassemble from a PDU
 *
 * @b Description
 *        Private handler invokded by kwAmmReassembleSdus with either a
 *        PDU or a segment of a PDU. This is also called in the case of
 *        reestablishment and hence out of sequence joining is also to
 *        be supported
 *
 *
 * @param[in]      rbCb     RB control block 
 * @param[in]      amHdr    AM header received for this PDU
 * @param[in]      pdu      PDU to be reassembled
 * @param[in]      recBuf   AM reception buffer
 * @param[in/out]  numSdu   
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmProcPdu
(
KwRbCb                *rbCb,
KwAmHdr               *amHdr,
Buffer                *pdu,
KwAmRecBuf            **recBuf,
U16                   *numSdu
)
#else
PRIVATE Void kwAmmProcPdu(rbCb, amHdr, pdu, recBuf, numSdu)
KwRbCb                *rbCb;
KwAmHdr               *amHdr;
Buffer                *pdu;
KwAmRecBuf            **recBuf;
U16                   *numSdu;
#endif
{
   Buffer             *sdu;
   U16                liCnt;
   Bool               isPduProcessd;  /* Flag whether the PDU is fully * 
                                        *processed or not.             */ 
   TRC2(kwAmmProcPdu)

   liCnt = (*recBuf)->amHdr.numSduProcessed;
   isPduProcessd = TRUE;

   //MSPD_DBG("[%d][%d] Exp SN = %d, SN = %d\n", rbCb->rlcId.ueId,
   //                                            rbCb->rlcId.rbId,
   //                                            AMUL.expSn, amHdr->sn);
   /* It gets transferred if not fully processed */
   (*recBuf)->pdu = NULLP;
   if ((AMUL.expSn != amHdr->sn) || (AMUL.expSo != amHdr->so))
   {
      /* Release the existing partial SDU as we have PDUs or */
      /* segments that are out of sequence                   */
      MSPD_ERR("[%d][%d][%d] Freeing partial sdu, Recvd pdu sn %d, so %d\
                vrR %d, expSn %d, expSo %d\n", KW_UEID, KW_RBTYPE, KW_RBID,
                amHdr->sn, amHdr->so, AMUL.vrR, AMUL.expSn, AMUL.expSo);
      KW_FREE_BUF(AMUL.partialSdu);
   }

   if (amHdr->fi & KW_FI_FIRST_SEG)
   {
      /* The first byte is not first byte of any SDU. That would mean */
      /* that there should be a partial SDU. If it is not there we    */
      /* ignore this portion of the PDU                               */
      if (AMUL.partialSdu == NULLP)
      {
         kwAmmGetSdu(&liCnt, amHdr, &pdu, &sdu);
         KW_FREE_BUF(sdu);
         (*recBuf)->amHdr.numSduProcessed++;
      }
      else if (amHdr->numLi == 0)
      {
         SCatMsg(pdu, AMUL.partialSdu, M2M1);
         KW_FREE_BUF(AMUL.partialSdu);
      }
      else
      {
         kwAmmGetSdu(&liCnt, amHdr, &pdu, &sdu);
         SCatMsg(sdu, AMUL.partialSdu, M2M1);
         KW_FREE_BUF(AMUL.partialSdu);
         kwUtlSndDatInd(rbCb, sdu);
         (*numSdu)++;
         (*recBuf)->amHdr.numSduProcessed++;
      }
   }
   else
   {
      /* This is the case where the first byte starts an SDU */
      /* Any existing partial SDU is there by error         */
      if (AMUL.partialSdu != NULLP)
      {
         MSPD_ERR("[%d][%d][%d] Freeing partial sdu, Recvd pdu sn %d vrR %d\n", 
                   KW_UEID, KW_RBTYPE, KW_RBID, amHdr->sn, AMUL.vrR);
         KW_FREE_BUF(AMUL.partialSdu);
      }
   }

   /* The below while loop covers starting with second LI */
   while(liCnt < amHdr->numLi)
   {
      kwAmmGetSdu(&liCnt, amHdr, &pdu, &sdu);
      kwUtlSndDatInd(rbCb, sdu);
      (*numSdu)++;
      (*recBuf)->amHdr.numSduProcessed++;
      /*If numPdu matches with MAX_UL_PDUS then the reassembly context needs*
       *to be stored so that it can be resumed in the next trigger.         */
      if(KW_AM_UM_MAX_UL_SDUS <= *numSdu)
      {
         (*recBuf)->amHdr.fi = amHdr->fi & 0x01;
         (*recBuf)->pdu = pdu;
         isPduProcessd = FALSE;
         break;
      }
   }
   if(TRUE == isPduProcessd)
   {
      if (amHdr->fi & KW_FI_LAST_SEG)
      {
         /* Any left over PDU will be joined with the next PDU */
         AMUL.partialSdu = pdu;
         (*recBuf)->amHdr.numSduProcessed++;
      }
      else if (pdu != NULLP)
      {
         AMUL.partialSdu = NULLP;
         kwUtlSndDatInd(rbCb, pdu);
         (*numSdu)++;
         (*recBuf)->amHdr.numSduProcessed++;
      }
   }
   RETVOID;
}


/**
 *
 * @brief Private handler to reassemble from a segment of a PDU
 *
 * @b Description
 *        Private handler invokded by kwAmmReassembleSdus with either a
 *        PDU or a segment of a PDU. This is also called in the case of
 *        reestablishment and hence out of sequence joining is also to
 *        be supported
 *
 *
 * @param[in]      rbCb     RB control block 
 * @param[in]      amHdr    AM header received for this segment
 * @param[in]      pdu      PDU to be reassembled
 * @param[in]      seg      Segment to be reassembled
 * @param[in/out]  numSdu   
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmProcSeg
(
KwRbCb                *rbCb,
KwAmHdr               *amHdr,
Buffer                *pdu,
KwSeg                 *seg,
U16                   *numSdu
)
#else
PRIVATE Void kwAmmProcSeg(rbCb, amHdr, pdu, seg, numSdu)
KwRbCb                *rbCb;
KwAmHdr               *amHdr;
Buffer                *pdu;
KwSeg                 *seg;
U16                   *numSdu;
#endif
{
   Buffer             *sdu;
   U16                liCnt;
   Bool               isSegProcessd;

   TRC2(kwAmmProcSeg)

   liCnt = seg->amHdr.numSduProcessed;
   isSegProcessd = TRUE;

   /* Set the seg->seg to NULLP. If not fully processed, it is set to */
   /* the remaining portion of the segement by this function          */
   seg->seg = NULLP;
   if ((AMUL.expSn != amHdr->sn) || (AMUL.expSo != amHdr->so))
   {
      /* Release the existing partial SDU as we have PDUs or */
      /* segments that are out of sequence                   */
      MSPD_ERR("[%d][%d][%d] Freeing partial sdu, Recvd pdu sn %d, so %d\
                vrR %d, expSn %d, expSo %d\n", KW_UEID, KW_RBTYPE, KW_RBID,
                amHdr->sn, amHdr->so, AMUL.vrR, AMUL.expSn, AMUL.expSo);
      KW_FREE_BUF(AMUL.partialSdu);
   }

   if (amHdr->fi & KW_FI_FIRST_SEG)
   {
      /* The first byte is not first byte of any SDU. That would mean */
      /* that there should be a partial SDU. If it is not there we    */
      /* ignore this portion of the PDU                               */
      if (AMUL.partialSdu == NULLP)
      {
         kwAmmGetSdu(&liCnt, amHdr, &pdu, &sdu);
         KW_FREE_BUF(sdu);
         seg->amHdr.numSduProcessed++;
      }
      else if (amHdr->numLi == 0)
      {
         SCatMsg(pdu, AMUL.partialSdu, M2M1);
         KW_FREE_BUF(AMUL.partialSdu);
      }
      else
      {
         kwAmmGetSdu(&liCnt, amHdr, &pdu, &sdu);
         SCatMsg(sdu, AMUL.partialSdu, M2M1);
         KW_FREE_BUF(AMUL.partialSdu);
         kwUtlSndDatInd(rbCb, sdu);
         (*numSdu)++;
         seg->amHdr.numSduProcessed++;
      }
   }
   else
   {
      /* This is the case where the first byte starts an SDU */
      /* Any existingg partial SDU is there by error         */
      if (AMUL.partialSdu != NULLP)
      {
         MSPD_ERR("[%d][%d][%d] Freeing partial sdu, Recvd pdu sn %d vrR %d\n", 
                   KW_UEID, KW_RBTYPE, KW_RBID, amHdr->sn, AMUL.vrR);
         KW_FREE_BUF(AMUL.partialSdu);
      }
   }

   /* The below while loop covers starting with second LI */
   while(liCnt < amHdr->numLi)
   {
      kwAmmGetSdu(&liCnt, amHdr, &pdu, &sdu);
      kwUtlSndDatInd(rbCb, sdu);
      (*numSdu)++;
      seg->amHdr.numSduProcessed++;
      /*If numPdu matches with MAX_UL_PDUS then the reassembly context needs*
       *to be stored so that it can be resumed in the next trigger.         */
      if(KW_AM_UM_MAX_UL_SDUS <= *numSdu)
      {
         seg->amHdr.fi = amHdr->fi & 0x01;
         seg->seg = pdu;
         isSegProcessd = FALSE;
         break;
      }
   }
   if(TRUE == isSegProcessd)
   {
      if (amHdr->fi & KW_FI_LAST_SEG)
      {
         /* Any left over PDU will be joined with the next PDU */
         AMUL.partialSdu = pdu;
         seg->amHdr.numSduProcessed++;
      }
      else if (pdu != NULLP)
      {
         AMUL.partialSdu = NULLP;
         kwUtlSndDatInd(rbCb, pdu);
         (*numSdu)++;
         seg->amHdr.numSduProcessed++;
      }
   }
   RETVOID;
}

/**
 *
 * @brief Private handler to reassemble SDUs
 *
 * @b Description
 *        Private handler invokded by kwAmmProcessPdus with the PDU
 *        from the reception buffer in sequence to reassemble SDUs and 
 *        send it to PDCP. 
 *
 *        - With the stored header info, FI and LSF segment / concatenate 
 *          PDUs or byte segments of PDUs to get the associated SDU.   
 *
 * @param[in]     rbCb     RB control block 
 * @param[in]     recBuf   AM reception buffer
 * @param[in/out] numSdu   
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PRIVATE S16 kwReassembleSdus
(
KwRbCb              *rbCb,
KwAmRecBuf          **recBuf,
U16                 *numSdu
)
#else
PRIVATE S16 kwReassembleSdus(rbCb, recBuf, numSdu)
KwRbCb              *rbCb;
KwAmRecBuf          **recBuf;
U16                 *numSdu;
#endif
{
   KwSeg        *seg;

   TRC2(kwReassembleSdus)
   //MSPD_DBG_RLC("[%d][%d][%d] Reassembling SN %d %d %d %d\n",KW_UEID, KW_RBTYPE,
   //             KW_RBID, (*recBuf)->amHdr.sn, (*recBuf)->amHdr.numLi,
   //             (*recBuf)->amHdr.fi, (*recBuf)->amHdr.rf);
   if ((*recBuf)->amHdr.rf == 0)
   {
      /* This is a PDU */
      kwAmmProcPdu(rbCb, &(*recBuf)->amHdr, (*recBuf)->pdu, recBuf, numSdu);
      if ((*recBuf)->pdu == NULLP)
      {
         AMUL.expSn = ((*recBuf)->amHdr.sn + 1) % 1024;
         AMUL.expSo = 0;
      }
   }
   else
   {
      /* This is a set of segments */
      KW_LLIST_FIRST_SEG((*recBuf)->segLst, seg);
      AMUL.expSn = (*recBuf)->amHdr.sn;
      while (seg && (*numSdu < KW_AM_UM_MAX_UL_SDUS))
      {
         kwAmmProcSeg(rbCb, &seg->amHdr, seg->seg, seg, numSdu);
         if (seg->seg == NULLP)
         {
            /*Delete the current segment from list and free this segment*/
            AMUL.expSo = seg->soEnd + 1;
            cmLListDelFrm(&((*recBuf)->segLst),&(seg->lstEnt));
            KW_FREE(seg, sizeof(KwSeg));
         }
         KW_LLIST_FIRST_SEG((*recBuf)->segLst, seg);
      }
      /* If all the segments are processed, move to the next PDU */
      if (0 == (*recBuf)->segLst.count)
      {
         AMUL.expSn = ((*recBuf)->amHdr.sn + 1) % 1024;
         AMUL.expSo = 0;
      }
   }
	RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler to estimate the header size of the RLC SDUs
 *
 * @b Description: 
 *
 *        This function is used to update the estimated Header size for the BO
 *        report. This function is called when a SDU is queued and also when 
 *        a PDU is formed and sent to the lower layer.
 *            
 *  @param[in] amDl     AM DL Information 
 *  @param[in] type     Type of Operation (Addition or Deletion of SDU)
 *  @param[in] sduCount Number of SDUs which are Added/Deleted.
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmEstHdrSz
(
KwAmDl           *amDl,
U8               type
)
#else
PRIVATE Void kwAmmEstHdrSz(amDl, type)
KwAmDl           *amDl;
U8               type;
#endif
{
   TRC2(kwAmmEstHdrSz);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmEstHdrSz(amDl, type)\n"));

   if (type == KW_ADD_SDU)
   {
      kwAmmAddEstHdrSize(amDl, (amDl->numSduPdu * 2) + (amDl->numSeg * 4));

   }
   else if (type == KW_DEL_SDU)
   {
      kwAmmDecEstHdrSize(amDl, (amDl->numSduPdu * 2) + (amDl->numSeg * 4));
   }   
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmEstHdrSz: estHdrSz = %ld \n", amDl->estHdrSz));
#else
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmEstHdrSz: estHdrSz = %d \n", amDl->estHdrSz));
#endif
   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to send Status Indication to PDCP
 *
 * @b Description: 
 *
 *        This function is used to send status indication to PDCP when the 
 *        maximum retransmission threshold value is reached for a PDU.
 *            
 *  @param[in] rbCb     RB control block 
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmSndStaInd
(
KwRbCb            *rbCb,
KwRetx            *retx
)
#else
PRIVATE S16 kwAmmSndStaInd(rbCb, retx)
KwRbCb            *rbCb;
KwRetx            *retx;
#endif
{
   KwuStaIndInfo *staInd;
   U8 i;
#ifdef KW_PDCP
   PjDatCfm      pjStaInd;
   PjRbCb        *pjRbCb;
#else
   KwKwuSapCb    *kwuSap;
#endif

   TRC2(kwAmmSndStaInd);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmSndStaInd(rbCb, retx)\n"));
#ifndef KW_PDCP
   /* Sap control block */
   kwuSap = kwCb.kwuSap + KW_UI_PDCP;
#endif
   /* Allocate memory for staInd Info */
   KW_ALLOC(staInd, sizeof(KwuStaIndInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (staInd == NULLP)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW012, (ErrVal)0,
            "kwAmmSndStaInd: Allocation failed for staInd \n");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   /* Fill staInd Info */ 
   KW_MEM_CPY(&staInd->rlcId, &rbCb->rlcId, sizeof(CmLteRlcId));    

   staInd->numSdu = retx->numSdu;
   for (i = 0; i < retx->numSdu; i++) 
   {
      staInd->sduId[i] = retx->sduMap[i].sdu->mode.am.sduId;
   }

#ifdef KW_PDCP
   if (rbCb->pjRbCb != NULLP)
   {
       pjRbCb = rbCb->pjRbCb;
   }
   else
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW013, (ErrVal)0,
            "kwAmmSndStaInd: PDCP CB is Null. Cannot send a cfm \n");
      RETVALUE(RFAILED);
   }
   KW_MEM_ZERO(&pjStaInd, sizeof(PjDatCfm)); 
   /* Fill datCfm Info */
   PJ_MEM_CPY(&pjStaInd, staInd, sizeof(KwuStaIndInfo));
   PJ_FREE(staInd, sizeof(KwuStaIndInfo));
   MSPD_DBG("[%d][%d] Sending STATUS IND for DL RLC MAX RETX\n", KW_UEID, 
             KW_RBID);
   pjLimStaInd(pjRbCb,&pjStaInd);
#else
   KwUiKwuStaInd(&kwuSap->pst, kwuSap->suId, staInd);
#endif /* KW_PDCP */

   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler to send Data Confirmation to PDCP
 *
 * @b Description: 
 *
 *        This function is used to send data confirmation to PDCP for each SDU
 *        after an acknowledgment is received for the same. 
 *            
 *  @param[in] rbCb     RB control block 
 *  @param[in] type     Type of Operation( Addition or Deletion of SDU)
 *  @param[in] sduCount Number of SDUs which are Added/Deleted.
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmSndDatCfm
(
KwRbCb *rbCb,                          /* RB context */
U32    sduId                           /* SDU for which datCfm is to be sent */
)
#else
PRIVATE S16 kwAmmSndDatCfm(rbCb, sduId)
KwRbCb *rbCb;                          /* RB context */
U32    sduId;                          /* SDU for which datCfm is to be sent*/
#endif
{
#ifndef KW_PDCP
   KwuDatCfmInfo *datCfm;
   KwKwuSapCb    *kwuSap;
#endif
#ifdef KW_PDCP
   PjDatCfm      pjDatCfm;
   PjRbCb        *pjRbCb;
#endif

   TRC2(kwAmmSndDatCfm);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmSndDatCfm(rbCb, sduId)\n"));
#ifndef KW_PDCP
   /* Sap control block */
   kwuSap = kwCb.kwuSap + KW_UI_PDCP;
#endif
#ifndef KW_PDCP
   /* Allocate memory for datCfm Info */
   KW_ALLOC(datCfm, sizeof(KwuDatCfmInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (datCfm == NULLP)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW015, (ErrVal)0,
            "kwAmmSndDatCfm: Allocation failed for datCfm\n");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   /* Fill datCfm Info */ 
   KW_MEM_CPY(&datCfm->rlcId, &rbCb->rlcId, sizeof(CmLteRlcId));    

   datCfm->sduId = sduId;
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmSndDatCfm: sduId = %ld \n", sduId));
#else
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmSndDatCfm: sduId = %d \n", sduId));
#endif
#endif
   /* If trace flag is enabled send the trace indication */
   if(kwCb.init.trc == TRUE)
   {
      /* Populate the trace params */
      kwLmmSendTrc(KWU_EVT_DAT_CFM, NULLP);
   }

#ifdef KW_PDCP
   if ( rbCb->pjRbCb != NULLP )
   {
       pjRbCb = rbCb->pjRbCb;
   }
   else
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW016, (ErrVal)0,
         "kwAmmSndDatCfm: PDCP CB is Null. Cannot send a cfm \n");
      RETVALUE(RFAILED);
   }

   /* Fill datCfm Info */
   PJ_MEM_CPY(&(pjDatCfm.rlcId), &rbCb->rlcId, sizeof(CmLteRlcId));
   pjDatCfm.numSdu = 1;
   pjDatCfm.sduId[0] = sduId;
   //MSPD_DBG("[%d][%d][%d] Sending DatCfm to PDCP\n", KW_UEID, KW_RBTYPE, KW_RBID);
   pjLimDatCfm(pjRbCb,&pjDatCfm);
#else
   KwUiKwuDatCfm(&kwuSap->pst, kwuSap->suId, datCfm);
#endif /* KW_PDCP */

   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler to get the next node to be retransmitted from retxLst
 *
 * @b Description: 
 *
 *        This function is used to get the next node to be retransmitted 
 *        from the retxLst
 *            
 *  @param[in] rbCb     RB control block. 
 *  @param[in] type     Type of Operation( Addition or Deletion of SDU)
 *  @param[in] sduCount Number of SDUs which are Added/Deleted.
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PRIVATE S16 kwGetNxtRetx 
(
KwAmDl *amDl,                          /* AM DL Info */
KwRetx **retx                           /* Next node to be retransmitted */
)
#else
PRIVATE S16 kwGetNxtRetx(amDl, retx)
KwAmDl *amDl;                          /* AM DL Info */
KwRetx **retx;                          /* Next node to be retransmitted */
#endif
{
   CmLList *tNode = NULLP;

   TRC2(kwGetNxtRetx);
   UNUSED(amDl);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwGetNxtRetx(amDl, retx)\n"));
   do
   {
      tNode = &((*retx)->lstEnt);
      tNode = tNode->next;
      
      if (tNode)
      {
         *retx = (KwRetx *)tNode->node;
      }
      else
      {
         *retx = NULLP;
      }

      if (*retx == NULLP) 
      {
         RETVALUE(ROK);
      }

   } while((*retx)->wtForAck);

   RETVALUE(ROK);
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
PRIVATE Void kwAmmResetStateVars
(
KwRbCb                       *rbCb
)
#else
PRIVATE Void kwAmmResetStateVars(rbCb)
KwRbCb                       *rbCb;
#endif
{
   /* Reset all receiver state variables */
   AMUL.sn         = 0;
   AMUL.vrR        = 0; 
   AMUL.vrH        = 0;
   AMUL.vrX        = 0;
   AMUL.vrMr       = (rbCb->m.am.amUl.vrR + KW_AM_WIN_SZ) % 1024;
   AMUL.vrMs       = 0;
   AMUL.staTrg     = FALSE;
   AMUL.cntrlBo    = 0;
   AMUL.expSn      = 0;
   AMUL.expSo      = 0;
   /* Reset all trasmitter state variables */
   AMDL.txSn       = 0;
   AMDL.vtA        = 0;
   AMDL.vtS        = 0;
   AMDL.vtMs       = (AMDL.vtA + 512) % 1024;
   AMDL.pollSn     = 0;
   AMDL.bo         = 0;
   AMDL.retxBo     = 0;
   AMDL.cntrlBo    = 0;
   AMDL.estHdrSz   = 0;
   AMDL.numSduPdu  = 0;
   AMDL.numSeg     = 0;
   AMDL.pduWoPoll  = 0;
   AMDL.byteWoPoll = 0;
   RETVOID;
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
PUBLIC Void kwAmmStopRbTmrs
(
KwRbCb                       *rbCb
)
#else
PUBLIC Void kwAmmStopRbTmrs(rbCb)
KwRbCb                       *rbCb;
#endif
{
   U32                       tmrRunning;

   tmrRunning = kwChkTmr((PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
   if (tmrRunning)
   {
      kwStopTmr((PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
   }
   tmrRunning = kwChkTmr((PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR); 
   if (tmrRunning)
   {
      //MSPD_DBG_RLC("[%d][%d][%d]KW_EVT_AMUL_STA_PROH_TMR Timer running\n",
      //             KW_UEID,KW_RBTYPE, KW_RBID);    
      kwStopTmr((PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);
   }
   tmrRunning = kwChkTmr((PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR); 
   if (tmrRunning)
   {
      kwStopTmr((PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR);
   }
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlRlsReTxPdu
(
KwRbCb                       *rbCb,
KwRetx                       *retx,
U8                           pdcpCfm
)
#else
PRIVATE Void kwAmmDlRlsReTxPdu(rbCb, retx, pdcpCfm)
KwRbCb                       *rbCb;
KwRetx                       *retx;
U8                           pdcpCfm;
#endif
{
   kwAmmChkSduMap(rbCb, retx->amHdr.sn, retx->sduMap, retx->numSdu, pdcpCfm);
   KW_FREE_BUF(retx->seg);
   KW_FREE(retx, sizeof(KwRetx));
   RETVOID;
}

/**
 * @brief Handler to process acked retx PDU or Segment
 *
 * @b Description:
 *        This function either releases the acked PDU/segment or queues
 *        the PDU for background release based on the compile choice.
 *
 * @param [in]    rbCb     -  UE Identifier
 * @param [in]    retx     -  Retransmission PDU/segment
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE Void kwAmmDlRemRetxPdu
(
KwRbCb                       *rbCb,
KwRetx                       *retx
)
#else
PRIVATE Void kwAmmDlRemRetxPdu(rbCb, retx)
KwRbCb                       *rbCb;
KwRetx                       *retx;
#endif
{
   kwAmmDlAddRetxPduToRlsLst(rbCb, retx);
   RETVOID;
}


/**
 *
 * @brief
 *
 *        Handler to free the PDUs present in transmission buffer txBuf
 *
 * @b Description:
 *
 * @param[in]     rbCb               -  RB control block
 *
 * @return  Void
 *
 */
U32        tempTxBufCnt = 0;
#ifdef ANSI
PRIVATE Void kwAmmEmptyTxBuf
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmEmptyTxBuf(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KwSn                 sn, mSn, mvtS;

   //MSPD_DBG("[%d][%d][%d] TxSn %d VtA %d VtS %d\n", KW_UEID, KW_RBTYPE, KW_RBID,
   //          AMDL.txSn, AMDL.vtA, AMDL.vtS);
   sn = AMDL.txSn;
   MODAMR(sn, mSn, AMDL.vtA);
   MODAMR(AMDL.vtS, mvtS, AMDL.vtA);
   while(mSn < mvtS)
   {
      if (AMDL.txBuf[sn])
      {
         kwAmmDlRlsTxPdu(rbCb, AMDL.txBuf[sn], KW_AMM_NO_PDCP_CFM);
         AMDL.txBuf[sn] = NULLP;
         tempTxBufCnt++;
      }
      sn = (sn + 1) % 1024;
      MODAMR(sn, mSn, AMDL.vtA);
   }

   /* Update txSn with the SN which will be freed next time when it is called */
   AMDL.txSn = sn;
}

/**
 *
 * @brief
 *
 *        Handler to delete every node in the retxLst LL.
 *
 * @b Description:
 *
 * @param[in]     rbCb               -  RB control block
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmEmptyRetxLst
(
KwRbCb     *rbCb
)
#else
PRIVATE Void kwAmmEmptyRetxLst(rbCb)
KwRbCb     *rbCb;
#endif
{
   KwRetx         *retx;

   TRC2(kwAmmEmptyRetxLst)

   KW_LLIST_FIRST_RETX(AMDL.retxLst, retx);
   while (retx)
   {
      cmLListDelFrm(&AMDL.retxLst, &retx->lstEnt);
      kwAmmDlRlsReTxPdu(rbCb, retx, KW_AMM_NO_PDCP_CFM);
      KW_LLIST_FIRST_RETX(AMDL.retxLst, retx);
   }
   /* Fix for Crash During Reestablishment */   
   AMDL.nxtRetx = NULLP;
}

/**
 *
 * @brief
 *
 *        Handler to process the deletion of PDUs in the ToBeRlsTxLst.
 *
 * @b Description:
 *
 * @param[in]     rbCb               -  RB control block
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmEmptyToBeRlsTxLst
(
KwRbCb     *rbCb
)
#else
PRIVATE Void kwAmmEmptyToBeRlsTxLst(rbCb)
KwRbCb     *rbCb;
#endif
{
   KwTx           *pdu;
   CmLListCp      *lst;
   CmLList        *toBeRlsNode;

   TRC2(kwAmmEmptyToBeRlsTxLst)

   pdu = NULLP;
   lst = NULLP;
   toBeRlsNode = NULLP;

   lst  = &AMDL.toBeRlsTxLst;

   /* First clear the node in the tranmission list and send the data confirm */
   /* to upper layer if this is configured.                                  */
   toBeRlsNode = cmLListFirst(lst);
   while(toBeRlsNode)
   {
      pdu = (KwTx *)(toBeRlsNode->node);
      cmLListDelFrm(lst, lst->first);
      KW_FREE_BUF(pdu->pdu);
      KW_FREE(pdu, sizeof(KwTx));
      toBeRlsNode = cmLListFirst(lst);
   }
}

/**
 *
 * @brief
 *
 *        Handler to process the deletion of PDUs in the ToBeRlsReTxLst.
 *
 * @b Description:
 *
 * @param[in]     rbCb               -  RB control block
 *
 *  @return Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmEmptyToBeRlsReTxLst
(
KwRbCb     *rbCb
)
#else
PRIVATE Void kwAmmEmptyToBeRlsReTxLst(rbCb)
KwRbCb     *rbCb;
#endif
{
   KwRetx         *retx;
   CmLListCp      *lst;
   CmLList        *toBeRlsNode;

   TRC2(kwAmmProcPduFrmToBeRlsReTxLst)

   lst = NULLP;
   retx = NULLP;
   toBeRlsNode = NULLP;

   /* Now free Retransmission list and send data confirm to upper layer if */
   /* it is configured.                                                    */
   lst = &AMDL.toBeRlsReTxLst;
   toBeRlsNode = cmLListFirst(lst);
   while(toBeRlsNode)
   {
      retx = (KwRetx *)(toBeRlsNode->node);
      cmLListDelFrm(lst, lst->first);
      KW_FREE_BUF(retx->seg);
      KW_FREE(retx, sizeof(KwRetx));
      toBeRlsNode = cmLListFirst(lst);
   }
}

/**
 * @brief 
 *        Handler to delete or reassemble the UL PDUs during re-establishment
 *
 * @b Description: 
 *        This function gets the AM pdu list and UL reception buffer and 
 *        traverses through this list/buffer to delete or reassemble the PDUs.
 *            
 *  @return  Void
 */
#ifdef ANSI
PRIVATE Void kwAmmReasmbleOnReEst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmReasmbleOnReEst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KwAmUl               *amUl;
   CmLListCp            *pduList;
   KwAmRecBuf           *pduRecBuf;
   KwSeg                *seg;
   KwSn                 mVrMr;
   KwSn                 mSn;
   KwSn                 sn;

   TRC2(kwAmmReasmbleOnReEst);
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwAmmReasmbleOnReEst(rbCb, numSdus)\n"));

   seg     = NULLP;
   amUl    = &(rbCb->m.am.amUl);
   pduList = &amUl->reassemblyLst;
   while(pduList->first)
   {
      pduRecBuf = (KwAmRecBuf*) pduList->first->node;
      KW_FREE_BUF(pduRecBuf->pdu);

      KW_LLIST_FIRST_SEG(pduRecBuf->segLst, seg)
      while (seg != NULLP)
      {
         KW_FREE_BUF(seg->seg);
         cmLListDelFrm(&(pduRecBuf->segLst),&(seg->lstEnt));
         KW_FREE(seg, sizeof(KwSeg));
         KW_LLIST_FIRST_SEG(pduRecBuf->segLst, seg);
      }
      cmLListDelFrm(pduList, pduList->first);
      KW_FREE(pduRecBuf, sizeof(KwAmRecBuf));
   }
  /* ccpu00127333:Freeing of UL partial sdu                                */
   KW_FREE_BUF(amUl->partialSdu);

   sn = AMUL.vrR;
   MODAMR(AMUL.vrMr, mVrMr, AMUL.vrR);
   MODAMR(sn, mSn, AMUL.vrR);   
   while(mSn < mVrMr)
   {
      if(amUl->recBuf[sn])
      {
         pduRecBuf = amUl->recBuf[sn];
         kwAmmUlRlsRecBuf(pduRecBuf);
         amUl->recBuf[sn] = NULLP;
      }
      sn = (sn + 1) % 1024;
      MODAMR(sn, mSn, amUl->vrR);
   }

   kwAmmResetStateVars(rbCb);
}

PRIVATE Void kwAmmUlAddRbForDeletion
(
KwRbCb                  *rbCb
)
{
   KW_RDWR_LOCK(&kwCb.deleteRbLstLock);
   {
      if (rbCb->deleteRbLnk.node == NULLP)
      {
         cmLListAdd2Tail(&kwCb.deleteRbLst, &rbCb->deleteRbLnk);
         rbCb->deleteRbLnk.node = (PTR)(rbCb);
      }
   }
   KW_RDWR_UNLOCK(&kwCb.deleteRbLstLock);
}


/**
 * @brief 
 *
 *        Handler to discard a SDU.
 *
 * @b Description: 
 *
 *        This function is used to discard a SDU after receiving
 *        the Discard Request from UIM. The SDU is discarded if its 
 *        available and is not mapped to any PDU yet.
 *            
 *  @param[in] rbCb   RB control block 
 *  @param[in] sduId  Sdu ID of the SDU to be discarded
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 kwAmmDiscSdu
(
KwRbCb                  *rbCb,
U32                     sduId
)
#else
PUBLIC S16 kwAmmDiscSdu(rbCb, sduId)
KwRbCb                  *rbCb;
U32                     sduId;
#endif
{
   KwSdu                *sdu = NULLP;

   TRC2(kwAmmDiscSdu);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmDiscSdu(rbCb, sduId)\n"));

   MSPD_DBG_RLC("[%d][%d][%d]DISCARD SDU request received sduId %d\n", 
                 KW_UEID, KW_RBTYPE, KW_RBID, sduId);

   /* Identify the SDU based on sduId and perform SDU discard */
   KW_LLIST_FIRST_SDU(AMDL.sduQ, sdu);
   while (sdu != NULLP) 
   {
      if (sdu->mode.am.sduId == sduId)
      {
         KwSdu          *tmpSdu = NULLP;

#ifdef LTE_L2_MEAS
         if(rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_DISC)
         {
            kwUtlSndDiscSduCfm(rbCb, sdu->mode.am.sduId);
         }
#endif
         rbCb->m.am.amDl.bo -= sdu->sduSz;
         KW_RMV_SDU(&AMDL.sduQ, sdu);
         kwCb.genSts.numSduDisc++;         
         AMDL.numSduPdu = 1;
         AMDL.numSeg    = 0;
         kwAmmEstHdrSz (&AMDL, KW_DEL_SDU); 
         break;
      }
      KW_LLIST_NEXT_SDU(AMDL.sduQ, sdu);
   }
   
   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler for reorder timer expiry
 *
 * @b Description: 
 *
 *        This function is used to handle events upon expiry of reorder timer
 *            
 *  @param[in] rbCb   RB control block 
 *  @param[in] sduId  Sdu ID of the SDU to be discarded
 *  @return  Void 
 *
 */

#ifdef ANSI
PUBLIC Void kwAmmReOrdTmrExp 
(
KwRbCb                  *rbCb
)
#else
PUBLIC Void kwAmmReOrdTmrExp(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KwAmUl *amUl = &(rbCb->m.am.amUl);
   KwSn sn    = 0;
   KwSn mSn   = 0;
   KwSn mVrMr = 0; 
   KwSn mVrMs = 0; 
   KwSn mVrH = 0; 
   Bool tmrRunning = FALSE;

   TRC2(kwAmmReOrdTmrExp);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmReOrdTmrExp(rbCb)\n"));

   /* Update vrMs */
   sn = amUl->vrX;

   MODAMR(sn, mSn, amUl->vrR);
   MODAMR(amUl->vrMr, mVrMr, amUl->vrR);

   while (mSn < mVrMr)
   {
      if ( (amUl->recBuf[sn] == NULLP) ||
            ((amUl->recBuf[sn] != NULLP) &&
            (!amUl->recBuf[sn]->allRcvd)) )
      {
         amUl->vrMs = sn;
         amUl->staTrg = TRUE; 
         /* Check if staProhTmr is running */
         tmrRunning = kwChkTmr((PTR) rbCb, KW_EVT_AMUL_STA_PROH_TMR);
         if (!tmrRunning)
         {
            kwUpdCntrlBo(amUl);
         }
         if (amUl->cntrlBo)
         {
            Bool staPduPrsnt;
            S32  bo = 0;

            AMDL.cntrlBo = AMUL.cntrlBo;
            /* Accumulate bo */
            //KW_UPD_AMBO(AMDL, bo); 

            /* Fill StaRsp and send it to MAC */
            //staPduPrsnt = (AMDL.cntrlBo) ? TRUE : FALSE;
            //kwUtlSndDStaRsp(rbCb, bo, AMDL.estHdrSz, staPduPrsnt);
            KW_AM_SND_DSTA_RSP();
         }

         break;
      }
      sn = (sn + 1) % 1024;
      MODAMR(sn, mSn, amUl->vrR);
   }

   /* Update vrX */
   MODAMR(amUl->vrH, mVrH, amUl->vrR);
   MODAMR(amUl->vrMs, mVrMs, amUl->vrR);
   if (mVrH > mVrMs)
   {
      //MSPD_DBG_RLC("[%d][%d][%d] Start Reordering Tmr %d %d\n",
      //             KW_UEID, KW_RBTYPE, KW_RBID, mVrH, mVrMs);
      kwStartTmr((PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
      amUl->vrX = amUl->vrH;
   }

   RETVOID;
} /* kwAmmReOrdTmrExp */

/**
 *
 * @brief 
 *
 *        Handler for status prohibit timer expiry
 *
 * @b Description: 
 *
 *        This function is used to handle events upon expiry of status prohibit
 *        timer 
 *            
 *  @param[in] rbCb   RB control block
 *  @param[in] sduId  Sdu ID of the SDU to be discarded
 *  @return  Void 
 *
 */

#ifdef ANSI
PUBLIC Void kwAmmStaProTmrExp 
(
KwRbCb    *rbCb                        /* AM UL Info */
)
#else
PUBLIC Void kwAmmStaProTmrExp(rbCb)
KwRbCb    *rbCb;                       /* AM UL Info */
#endif
{
   KwAmUl *amUl = &(rbCb->m.am.amUl);

   TRC2(kwAmmStaProTmrExp);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmStaProTmrExp(rbCb)\n"));

   if (amUl->staTrg == TRUE)
   {
      //MSPD_DBG_RLC("[%d][%d]SPT expired, status already triggered\n",
      //                KW_UEID, KW_RBID);   
      kwUpdCntrlBo(amUl);
   }
   if (amUl->cntrlBo)
   {
      Bool staPduPrsnt;
      S32  bo = 0;

      AMDL.cntrlBo = AMUL.cntrlBo;
      /* Accumulate bo */
      //KW_UPD_AMBO(AMDL, bo); 
      /* Fill StaRsp and send it to MAC */
      //staPduPrsnt = (AMDL.cntrlBo) ? TRUE : FALSE;
      //kwUtlSndDStaRsp(rbCb, bo, AMDL.estHdrSz, staPduPrsnt);
      KW_AM_SND_DSTA_RSP();
   }

   RETVOID; 
} /* kwAmmStaProTmrExp */

/**
 *
 * @brief 
 *        Handler for Poll retransmit timer expiry
 *
 * @b Description: 
 *
 *        This function is used to handle events upon expiry of Poll
 *        retransmit timer.
 *            
 *  @param[in] rbCb   RB control block 
 *  @return  Void 
 */
#ifdef ANSI
PUBLIC Void kwAmmPollRetxTmrExp
(
KwRbCb            *rbCb
)
#else
PUBLIC Void kwAmmPollRetxTmrExp(rbCb)
KwRbCb            *rbCb;
#endif
{
   S16            ret = ROK;
   KwRetx         *retx = NULLP; 
   KwAmDl         *amDl = &(rbCb->m.am.amDl);
   KwSn           sn    = 0;

   TRC2(kwAmmPollRetxTmrExp);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmPollRetxTmrExp(rbCb)\n"));
   
   /* Correcting the logic for determmining whether to do   */
   /* any transmission of PDU. As per the spec section      */
   /* 5.2.2.3, if there is any to transmit or retransmit,   */
   /* do nothing. Else, pick up the VT(S) -1 for retx       */
   /* We have nothing to transmit if window is stalled or   */
   /* there are no SDUs to be transmitted or if there are   */
   /* PDUs to be retransmitted.                             */
   if (((kwAmmDlTxSduPrsnt(rbCb)) && (amDl->nxtRetx == NULLP)) ||
       (amDl->vtS == amDl->vtMs))
   {
      sn = (amDl->vtS - 1) & 0x3ff;
      if (amDl->txBuf[sn] != NULLP)
      {
         //MSPD_DBG("[%d][%d] Adding SN %d to retx list\n",
         //                   rbCb->rlcId.ueId, rbCb->rlcId.rbId, sn);
         ret = kwUpdFrmTxtoRetx(amDl, &retx, sn); 
         if(ret != ROK)
         {
            RETVOID;
         }
         else if (AMDL.nxtRetx == NULLP)
         {
            AMDL.nxtRetx = retx;
         }
         KW_AM_SND_DSTA_RSP();
#ifdef MSPD
         ysMsAmmDlPollReTx++;
#endif
         RETVOID;
      }
      /* Get the last node in retxLst */
      KW_LLIST_LAST_RETX(amDl->retxLst, retx);

      /* Unset wtForAck flag for the NACK PDUs */ 
      if (retx != NULLP)
      {
         kwMrkRetxPdu(rbCb, retx);
         KW_AM_SND_DSTA_RSP();
      }
#ifdef MSPD
      ysMsAmmDlPollReTx++;
#endif
   }

   RETVOID;
} /* kwAmmPollRetxTmrExp*/

/**
 * @brief 
 *
 *        Handler to update Acks for the remaining PDUs after the last accessed
 *        NACK PDU.
 *
 * @b Description: 
 *
 *        This function is used to handle ACKs for the PDUs remaining after the 
 *        last accessed NACK PDU, It updates the txBuf/retxBuf for the ACKs and 
 *        sends DatCfm to PDCP for the same.
 */
#ifdef ANSI
PRIVATE Void kwUpdRemPduAck
(
KwRbCb                  *rbCb,
KwSn                    mAckSn,
CmLList                 *retxNode
)
#else
PRIVATE Void kwUpdRemPduAck(rbCb, mAckSn, retxNode)
KwRbCb                  *rbCb;
KwSn                    mAckSn;
CmLList                 *retxNode;
#endif
{
   KwSn                 mSn = 0;
   KwSn                 sn  = 0;
   KwRetx               *retx;

   TRC2(kwUpdRemPduAck);

   /* Remove pdus/segs from retxLst */ 
   while (retxNode)
   {
      retx = (KwRetx *)(retxNode->node);
      retxNode = retxNode->next;
      MODAMT(retx->amHdr.sn, mSn, AMDL.vtA);
      if (mSn < mAckSn) 
      {
         kwAmmDlRemRetxPdu(rbCb, retx);
      }
   }

   /* For the remaining ACKs  after last nackSn */
   MODAMT(AMDL.txSn, mSn, AMDL.vtA);
   sn = AMDL.txSn;
   while(mSn < mAckSn)
   {
      if ((AMDL.txBuf[sn] != NULLP) )
      {
         KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_DETAIL | KW_DBGMASK_UL),
                  (_kwPBuf, "kwUpdRemPduAck: Handle ACK for PDU with sn = \
                   %d\n", sn));

         /* Remove pdus from txBuf */
         kwAmmDlRemTxPdu(rbCb, sn);
      }
      mSn++; sn = (sn + 1)%1024;
   }

   RETVOID;
}

/**
 * @brief 
 *
 *        Handler to extract an element of AM Header
 *
 * @b Description: 
 *
 *        This function is used to extract an element of AM header. 
 */

#ifdef ANSI
PRIVATE Void kwExtractElmnt
(
Buffer      *pdu,
KwExtHdr    *hdrInfo
)
#else
PRIVATE Void kwExtractElmnt(pdu, hdrInfo)
Buffer      *pdu;
KwExtHdr    *hdrInfo;
#endif
{
   U8   hdr  = 0x00;
   U8   pLen = hdrInfo->pLen;
   U8   len  = (U8)hdrInfo->len;
   U16  val  = 0;
   U8   tHdr = 0x00;
   U8   fLen = 0;
   U8   rLen = 0;
   U8   rLen1 = 0;
   U16  tVal;
       
   TRC2(kwExtractElmnt);

   hdr = hdrInfo->hdr;
   
   if (pLen == 0)
   {
      SRemPreMsg(&hdr, pdu);
      pLen = 8;
   }
   tHdr = hdr;
   if (len <= 8)
   {
      val = tHdr >> (KW_BYTE_LEN - (len));
      hdr =  hdr << len;
      pLen -= len;
   }
   else if (len > 8)                                       
   {
      fLen = pLen;
      val = tHdr;
      val = val >> (KW_BYTE_LEN - fLen);
      val = val << (len - fLen);
      rLen = len - fLen;
      SRemPreMsg(&hdr, pdu);
      tHdr = hdr;
      if (rLen <= 8)
      {
         hdr = hdr >> (KW_BYTE_LEN - rLen);               
         val = val | hdr;
         hdr = tHdr << rLen;
         pLen = (KW_BYTE_LEN - rLen);
      }                                                     
      else
      {
        rLen1 = rLen - KW_BYTE_LEN;
        tVal = hdr;
        tVal = tVal << rLen1;
        val = val | tVal;

        SRemPreMsg(&hdr, pdu);
        tHdr = hdr;
        hdr = hdr >> (KW_BYTE_LEN - rLen1);
        val = val | hdr;
        hdr = tHdr << rLen1;
        pLen = (KW_BYTE_LEN - rLen1);
      }
   }

   hdrInfo->pLen = pLen;
   hdrInfo->hdr = hdr;
   hdrInfo->val = val;
   RETVOID;
}

/**
 * @brief 
 *
 *        Handler to form construct AM header
 *
 * @b Description: 
 *
 *        This function is used to construct am header with the available header
 *        elements.
 *
 *  @param[in]  amHdr  AM Header
 *  @param[in]  isSeg  Check for Segmentation of PDU
 *  @param[in]  hdr    Header field
 *  @param[in]  idx    Index
 *  
 *  @return Void            
 */
#ifdef ANSI
PRIVATE Void kwConstructAmHdr
(
KwAmHdr     *amHdr,
Bool        isSeg,
U8          *hdr,
U16         *idx
)
#else
PRIVATE Void kwConstructAmHdr(amHdr, isSeg, hdr, idx)
KwAmHdr     *amHdr;
Bool        isSeg;
U8          *hdr;
U16         *idx;
#endif
{
   KwHdrInfo hdrInfo;
   U8  e;                                                   
   S16 numLi;                                               
   U8  eb   = 0;                                              

   TRC2(kwConstructAmHdr);
                                                            
   KW_MEM_ZERO(&hdrInfo, sizeof(KwHdrInfo)); 

   hdrInfo.pEb  = 1;
   hdrInfo.pLen = 0;
   hdrInfo.idx  = *idx = 0;
   numLi        = amHdr->numLi - 1;                                

   /* For fixed part of the header */                       
   hdr[0] = KW_DATA_PDU;                                    
   hdr[0] = hdr[0] << (KW_BYTE_LEN - KW_DC_LEN);            
   eb = KW_BYTE_LEN - KW_DC_LEN;                            
 
   hdrInfo.hdr = hdr;
   hdrInfo.eb  = eb;

   KW_FILL_HDR_ARGS(hdrInfo, amHdr->rf, KW_RF_LEN); 
   kwFormBits(&hdrInfo);           

   KW_FILL_HDR_ARGS(hdrInfo, amHdr->p, KW_P_LEN);            
   kwFormBits(&hdrInfo);

   KW_FILL_HDR_ARGS(hdrInfo, amHdr->fi, KW_FI_LEN);
   kwFormBits(&hdrInfo);

   e = (numLi > 0) ? 1 : 0;
   KW_FILL_HDR_ARGS(hdrInfo, e, KW_E_LEN);
   kwFormBits(&hdrInfo);

   KW_FILL_HDR_ARGS(hdrInfo, amHdr->sn, KW_SN_LEN);
   kwFormBits(&hdrInfo);

   if (isSeg == TRUE)                                      
   {                                                        
      KW_FILL_HDR_ARGS(hdrInfo, amHdr->lsf, KW_LSF_LEN);
      kwFormBits(&hdrInfo);

      KW_FILL_HDR_ARGS(hdrInfo, amHdr->so, KW_SO_LEN);
      kwFormBits(&hdrInfo);
   }                                                        

   /* For extension part of the header */                   
   if (numLi > 0)                                           
   {                                                        
      U8 count = 0;                                         
      while (count < (amHdr->numLi-1))                                
      {                                                     
         e = (numLi > 1) ? 1 : 0;                           
         KW_FILL_HDR_ARGS(hdrInfo, e, KW_E_LEN);
         kwFormBits(&hdrInfo);

         KW_FILL_HDR_ARGS(hdrInfo, amHdr->li[count], KW_LI_LEN);
         kwFormBits(&hdrInfo);

         count++;
         numLi--;
      }                                                    
   }                                                       

   hdr  = hdrInfo.hdr;
   *idx = hdrInfo.idx;
   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to form am header Info
 *
 * @b Description: 
 *
 *        This function is used to assemble AM header Information
 *            
 * @param[in] hdrInfo  Header Information
 * @return Void
 */

#ifdef ANSI
PRIVATE Void kwFormBits
(
KwHdrInfo *hdrInfo
)
#else
PRIVATE Void kwFormBits(hdrInfo)
KwHdrInfo *hdrInfo;
#endif
{
   U16  val  = hdrInfo->val;
   U8   len  = hdrInfo->len;
   U8   eb   = hdrInfo->eb;
   U8   *hdr = NULLP;
   U16  idx  = hdrInfo->idx;
   U8   pEb  = hdrInfo->pEb;
   U8   pLen = hdrInfo->pLen;

   U16 pIdx = idx;                                         
   U16 cIdx = idx + 1;                                     
   U8  tPdu;                                                
   U16 tVal;                                                

   TRC2(kwFormBits);

   hdr = hdrInfo->hdr;

   if (eb >= len)                                         
   {                                                        
      if (pEb - pLen == 0)
      {                                                    
         idx++;
         pIdx = idx;
      }                                                    
      tPdu = (val << (eb - len)) | hdr[pIdx]; 
      hdr[pIdx] = tPdu;                                   
      pEb = eb;                                           
      pLen = len;                                         
      eb = eb - len;       
   }                                                        
   else if (eb < len)                                     
   {                                                        
      tVal = val;                                          
      if (eb != 0)                                         
      {                                                     
         tPdu = (tVal >> (len - eb)) | hdr[pIdx];       
         hdr[pIdx] = tPdu;                                
      }                                                     
      if (len - eb == KW_BYTE_LEN)                        
      {                                                     
         hdr[cIdx] = (U8)tVal;                                
         idx = cIdx;                                       
         pEb = eb;
         pLen = len;                                      
         eb = KW_BYTE_LEN - (len - eb);                  
      }                                                     
      else if (len - eb < KW_BYTE_LEN)                    
      {                                                     
         if (eb == 0)                                     
            hdr[cIdx] = tVal << (KW_BYTE_LEN - len);     
         else                                              
           hdr[cIdx] = tVal << (len - eb);    
         idx = cIdx;                                       
         pEb = eb;                                        
         pLen = len;
         eb = KW_BYTE_LEN - (len - eb);                  
      }                                                     
      else                                                  
      {                                                     
         hdr[cIdx++] = tVal >> ((len - eb) - KW_BYTE_LEN); 
         hdr[cIdx] = tVal << KW_BYTE_LEN; 
         idx = cIdx;                                      
         pEb = eb;
         pLen = len;
         eb = KW_BYTE_LEN - ((len - eb) - KW_BYTE_LEN); 
      }                                                    
   }                                                       

   hdrInfo->val  = val;
   hdrInfo->len  = len;
   hdrInfo->eb   = eb;
   hdrInfo->hdr  = hdr;
   hdrInfo->idx  = idx;
   hdrInfo->pEb  = pEb;
   hdrInfo->pLen = pLen;
   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to form control bits
 *
 * @b Description: 
 *
 *        This function is used to assemble control bits for the control PDU
 *            
 * @param[in]     cntrlInfo      Control Info
 * @param[in]     staPdu         Status PDU
 * @param[in]     kwDatReq       Data Request Information
 * @param[in/out] pduSzInBits    Remaining allocation in bits
 * @param[in/out] staPduSzInBits Remaining stapdu[] array size in bits
 *
 * @return Void   
 */

#ifdef ANSI
PRIVATE Void kwFormCntrlBits 
(
KwCntrlInfo *cntrlInfo,
U16         *staPdu,
KwDatReq    *kwDatReq,
U32         *pduSzInBits,
U16         *staPduSzInBits
)
#else
PRIVATE Void kwFormCntrlBits(cntrlInfo, staPdu, kwDatReq, pduSzInBits, staPduSzInBits)
KwCntrlInfo *cntrlInfo;
U16         *staPdu;
KwDatReq    *kwDatReq;
U32         *pduSzInBits;
U16         *staPduSzInBits;
#endif
{
   U16  val     = cntrlInfo->val;
   U16  len     = cntrlInfo->len;
   U16  idx     = cntrlInfo->idx;
   U8   eb      = cntrlInfo->emtBits;
   U16  e1Idx   = cntrlInfo->e1Idx;
   U16  e2Idx   = cntrlInfo->e2Idx;   
   U8   e1eb    = cntrlInfo->e1eb;

   U16 pIdx      = idx;                                       
   U16 cIdx      = idx + 1;                                   
   U16 tPdu;                                               
   U16 tVal      = val;                                        

   TRC2(kwFormCntrlBits);

   *pduSzInBits -= len;
   *staPduSzInBits -= len;

   if (eb >= len)                                        
   {                                                       
      tPdu = (tVal << (eb-len) ) | staPdu[pIdx];           
      staPdu[pIdx] = tPdu;                                
      eb = eb - len;                                    
      if (len != KW_SO_LEN)
      {
         /* Store e1Idx/e2Idx and e1eb */                  
         e1Idx = e2Idx = pIdx;                            
         e1eb = eb;                                       
      }
   }                                                       
   else                                                    
   {                                                       
      if (eb == 0)                                        
      {                                                    
         staPdu[cIdx] = 0;
         tVal = tVal << (KW_2BYTE_LEN - len);
         staPdu[cIdx] = tVal;                             
         eb = KW_2BYTE_LEN - len;                        
         /* Store e1Idx/e2Idx and e1eb */                  
         if (len != KW_SO_LEN)
         {
            e1Idx = e2Idx = cIdx;                            
            e1eb = eb;                                       
         }
      }                                                    
      else /* eb < len */                                
      {                                                    
         tPdu = (tVal >> (len - eb)) | staPdu[pIdx];    
         staPdu[pIdx] = tPdu;                             
         staPdu[cIdx] = 0;
         staPdu[cIdx] = tVal << (KW_2BYTE_LEN - (len - eb));             
         eb = KW_2BYTE_LEN - (len - eb);                

         if (len != KW_SO_LEN)
         {
         /* Store e1Idx/e2Idx and e1eb */                  
         if (eb == 15)                                    
         {                                                  
            e1Idx = pIdx;                                  
            e2Idx = cIdx;                                  
            e1eb = 0;                                       
         }                                                  
         else                                               
         {                                                  
            e1Idx = e2Idx = cIdx;                          
            e1eb = eb;                                     
         }                                                  
         }
      }                                                    
      idx = cIdx;                                        
   }                                                       

   cntrlInfo->val     = val;
   cntrlInfo->len     = (U8)len;
   cntrlInfo->idx     = idx;
   cntrlInfo->emtBits = eb;
   cntrlInfo->e1Idx   = e1Idx;
   cntrlInfo->e2Idx   = e2Idx;   
   cntrlInfo->e1eb    = e1eb;
   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to updated expected byte seg 
 *
 * @b Description: 
 *
 *        This function is used to update expected byte segment and allRcvd flag
 *        for recBuf.
 *
 * @param[in]  amUl  AM Uplink Control Block
 * @param[in]  sn    Sequence Number
 * @param[in]  so    Segment Offset
 * 
 * @return Void
 *            
 *
 */

#ifdef ANSI
PRIVATE Void kwAmmUpdExpByteSeg
(
KwAmUl             *amUl,
KwSeg              *seg
)
#else
PRIVATE Void kwAmmUpdExpByteSeg(amUl, seg)
KwAmUl             *amUl;
KwSeg              *seg;
#endif
{
   U16             seqSo   = 0;                                          
   KwSn            sn      = seg->amHdr.sn;
   Bool            lstRcvd = FALSE;
   KwSeg           *tSeg   = seg;

   TRC2(kwAmmUpdExpByteSeg);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmUpdExpByteSeg(amUl, sn, so)\n"));

#if 0
#else
   KW_LLIST_FIRST_SEG(amUl->recBuf[sn]->segLst, tSeg);
#endif
   while(tSeg)
   {
      if (tSeg->amHdr.so == seqSo)
      {
         seqSo = tSeg->soEnd + 1;
         lstRcvd = tSeg->amHdr.lsf;
         KW_LLIST_NEXT_SEG(amUl->recBuf[sn]->segLst, tSeg);
      }
      else
      {
         amUl->recBuf[sn]->expSo = seqSo;
         MSPD_DBG_RLC("Updating expSo[%d] next seg so[%d]\n",
                      seqSo, tSeg->amHdr.so);
         RETVOID;
      }
   }
   if (lstRcvd == TRUE)
   {
      amUl->recBuf[sn]->allRcvd = TRUE;
   }

   RETVOID;
} /* kwAmmUpdExpByteSeg */

/**
 *
 * @brief 
 *
 *        This function adds a retx PDU to list of retx PDUs
 *
 * @b Description: 
 *       kw003.201 - Poll expiry may cause an SN to be added to retx 
 *                   out of sequence and hence all additions to retx 
 *                   must validate that they are added in sequence   
 *
 * @param[in]  amDl  AM Downlink Control Block
 * @param[in]  retx  Retransmit PDU
 * 
 * @return Void
 *            
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmAddPduToRetxLst
(
KwAmDl               *amDl,
KwRetx               *retx
)
#else
PRIVATE Void kwAmmAddPduToRetxLst(amDl, retx)
KwAmDl               *amDl;
KwRetx               *retx;
#endif
{
   CmLList           *node;
   KwRetx            *tRetx;
   KwSn              tMSn;
   KwSn              retxMSn;
   TRC2(kwAmmAddPduToRetxLst);
 
   KW_RDWR_LOCK(&kwCb.dlLock);
   node = amDl->retxLst.last;
   MODAMT(retx->amHdr.sn, retxMSn, amDl->vtA);
   while(node != NULLP)
   {
      tRetx = (KwRetx *)(node->node);
      MODAMT(tRetx->amHdr.sn, tMSn, amDl->vtA);
      if (tMSn > retxMSn)
      {
         node = node->prev;
      }
      else
      {
         break;
      }
   }
   if (node)
   {
      amDl->retxLst.crnt = node;
      cmLListInsAfterCrnt(&amDl->retxLst, &retx->lstEnt);
      retx->lstEnt.node = (PTR)retx;
   }
   else
   {
      amDl->retxLst.crnt = amDl->retxLst.first;
      cmLListInsCrnt(&amDl->retxLst, &retx->lstEnt);
      retx->lstEnt.node = (PTR)retx;
   }

   if (amDl->nxtRetx == NULLP)
   {
      amDl->nxtRetx = retx;
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to Move the PDU from txBuf to retxLst 
 *
 * @b Description: 
 *
 *        This function is used to move the PDU from the txBuf to retxLst
 *
 * @param[in]  amDl  AM Downlink Control Block
 * 
 * @return Void
 *            
 *
 */

#ifdef ANSI
PRIVATE S16 kwUpdFrmTxtoRetx
(
KwAmDl                  *amDl,
KwRetx                  **retx,
KwSn                    sn
)
#else
PRIVATE S16 kwUpdFrmTxtoRetx(amDl, retx, sn)
KwAmDl                  *amDl;
KwRetx                  **retx;
KwSn                    sn;
#endif
{
   U8                   i;

   TRC2(kwUpdFrmTxtoRetx);

   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwUpdFrmTxtoRetx(amUl, sn, so)\n"));

   KW_ALLOC(*retx, sizeof(KwRetx));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*retx == NULLP)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW018, (ErrVal)0,
            "kwUpdFrmTxtoRetx: Allocation failed for retxBuf \n");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   (*retx)->seg = amDl->txBuf[sn]->pdu;
   (*retx)->segSz = amDl->txBuf[sn]->pduSz;
   /* MS_FIX for DL stall */
   (*retx)->soEnd = amDl->txBuf[sn]->pduSz - 1;

   (*retx)->hdrSz = amDl->txBuf[sn]->hdrSz;
   (*retx)->numSdu = amDl->txBuf[sn]->numSdu;
   (*retx)->retxCnt = 1;

   for (i = 0; i < (*retx)->numSdu; i++)
   {
      (*retx)->sduMap[i].sdu = amDl->txBuf[sn]->sduMap[i].sdu;
      //(*retx)->sduMap[i].sduId = amDl->txBuf[sn]->sduMap[i].sduId;
      (*retx)->sduMap[i].sduSz = amDl->txBuf[sn]->sduMap[i].sduSz;
      (*retx)->sduMap[i].actSz = amDl->txBuf[sn]->sduMap[i].actSz;
      (*retx)->sduMap[i].isSegmented = amDl->txBuf[sn]->sduMap[i].isSegmented;
   }

   KW_MEM_CPY(&((*retx)->amHdr), &amDl->txBuf[sn]->amHdr, sizeof(KwAmHdr));
   (*retx)->amHdr.lsf = 1;
   kwAmmAddPduToRetxLst(amDl, (*retx));

   /* Remove PDU from txBuf */
   KW_FREE(amDl->txBuf[sn], sizeof(KwTx));

   /* Update the BO appropriately */
   kwAmmAddRetxBo(amDl, (*retx)->segSz);
   kwAmmAddEstHdrSize(amDl, (*retx)->hdrSz);
#ifdef MSPD
   ysMsAmmDlRetxs[0]++;
#endif
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE Void kwAmmDlAddRetxPduToRlsLst
(
KwRbCb         *rbCb,
KwRetx         *retx
)
#else
PRIVATE Void kwAmmDlAddRetxPduToRlsLst(rbCb, retx)
KwRbCb         *rbCb;
KwRetx         *retx;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      cmLListDelFrm(&AMDL.retxLst, &retx->lstEnt);
      cmLListAdd2Tail(&AMDL.toBeRlsReTxLst, &(retx->rlsReTxLnk));
      retx->rlsReTxLnk.node =(PTR) retx;
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlGetRetxPduForRelease
(
KwRbCb         *rbCb,
KwRetx         **retx
)
#else
PRIVATE Void kwAmmDlGetRetxPduForRelease(rbCb, retx)
KwRbCb         *rbCb;
KwRetx         **retx;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      if (AMDL.toBeRlsReTxLst.first == NULLP)
      {
         *retx = NULLP;
      }
      else
      {
         (*retx) = (KwRetx *)(AMDL.toBeRlsReTxLst.first->node);
         (*retx)->rlsReTxLnk.node = NULLP;
         cmLListDelFrm(&AMDL.toBeRlsReTxLst, AMDL.toBeRlsReTxLst.first); 
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlAddRbToPduRlsLst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmDlAddRbToPduRlsLst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   if (rbCb->pduRlsRbLnk.node == NULLP)
   {
      cmLListAdd2Tail(&(kwCb.pduRlsRbLst), &(rbCb->pduRlsRbLnk));
      rbCb->pduRlsRbLnk.node = (PTR)rbCb;
      //MSPD_DBG("[%d][%d][%d] RbCb added to PduRlsLst\n", KW_UEID, KW_RBTYPE,
      //          KW_RBID);
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlRemRbFrmPduRlsLst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmDlRemRbFrmPduRlsLst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   if (rbCb->pduRlsRbLnk.node != NULLP)
   {
      cmLListDelFrm(&kwCb.pduRlsRbLst, &rbCb->pduRlsRbLnk);
      rbCb->pduRlsRbLnk.node = NULLP;
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlGetRbForPduRls
(
KwRbCb                  **rbCb
)
#else
PRIVATE Void kwAmmDlGetRbForPduRls(rbCb)
KwRbCb                  **rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   {
      if (kwCb.pduRlsRbLst.first == NULLP)
      {
         *rbCb = NULLP;
      }
      else
      {
         (*rbCb) = (KwRbCb *)(kwCb.pduRlsRbLst.first->node);
         kwCb.pduRlsRbLst.first->node = NULLP;
         cmLListDelFrm(&kwCb.pduRlsRbLst, kwCb.pduRlsRbLst.first);
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dlLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlPostSelfMsg
(
Void
)
#else
PRIVATE Void kwAmmDlPostSelfMsg(Void)
#endif
{
   KW_RDWR_LOCK(&kwCb.selfPstDlLock);
   if (1 != kwCb.dlSelfMsgPosted)
   {
      kwCb.dlSelfMsgPosted = 1;
      if(SPstTsk(&selfPstDl, kwCb.mBufDl) != ROK)
      {
         kwCb.dlSelfMsgPosted = 0;
      }
      //MSPD_DBG("DL self post message posted\n");
   }
   KW_RDWR_UNLOCK(&kwCb.selfPstDlLock);
   RETVOID;
}


/**
 *
 * @brief 
 *
 *        Handler to process the self posted message by RLC for 
 *        reassembling of SDUs in AM
 *
 * @b Description: 
 *
 *        This function gets the AM pdu list and traverses through this list
 *        and calls a function to reassemble the SDUs
 *            
 *  @return  Void
 *
 */
#ifdef MSPD
#include "Appinit.h"
extern SysStayInfo syscoreinfo [4];
#endif
#ifdef ANSI
PRIVATE Void kwAmmUlProcessReassemblyRb
(
KwRbCb                  *rbCb,
U16                     *numSdus
)
#else
PRIVATE Void kwAmmUlProcessReassemblyRb(rbCb, numSdus)
KwRbCb                  *rbCb;
U16                     *numSdus;
#endif
{
   KwAmRecBuf           *pduRecBuf; 
   U32 cnt = 0;
   TRC2(kwAmmUlProcessReassemblyRb);
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_UL),
          (_kwPBuf, "kwAmmUlProcessReassemblyRb(rbCb, numSdus)\n"));

   pduRecBuf = NULLP;
   /* This case is neither deletion nor reestablishment. RB is */
   /* queued for PDUs that are to be reassembled.              */
#ifdef MSPD
   syscoreinfo[2].res[2]=0;
   syscoreinfo[2].res[3]=0;   
#endif
   kwAmmUlGetPduForReassembly(rbCb, &pduRecBuf);
#ifdef MSPD
      syscoreinfo[2].res[2]=1;
#endif
   while (pduRecBuf != NULLP)
   {

      kwReassembleSdus(rbCb, &pduRecBuf, numSdus);
      if (((pduRecBuf->amHdr.rf == 0) && (pduRecBuf->pdu == NULLP)) ||
          ((pduRecBuf->amHdr.rf == 1) && (pduRecBuf->segLst.first == NULLP)))
      {
         /* PDU is fully processed, so reset the part PDU and release */
         /* the PDU                                                   */
         cnt++;
#ifdef MSPD
         syscoreinfo[2].res[3]=cnt;		 
#endif 
         AMUL.partPdu = NULLP;
         KW_FREE(pduRecBuf, sizeof(KwAmRecBuf));
      }
      else
      {
         cnt++;
#ifdef MSPD
         syscoreinfo[2].res[3]=cnt;		 
#endif
         AMUL.partPdu = pduRecBuf;
      }
      if ((*numSdus) >= KW_AM_UM_MAX_UL_SDUS)
      {
         RETVOID;
      }
      kwAmmUlGetPduForReassembly(rbCb, &pduRecBuf);
   }
#ifdef MSPD
   syscoreinfo[2].res[2]=2;
#endif

   RETVOID;
}

#ifdef ANSI
PUBLIC Void kwAmmUlProcessSelfMsg
(
KwRbCb                  *rbCb,
U16                     *numSdus
)
#else
PUBLIC Void kwAmmUlProcessSelfMsg(rbCb, numSdus)
KwRbCb                  *rbCb;
U16                     *numSdus;
#endif
{
#ifdef MSPD_MLOG_NEW
   U32                  startTick, endTick;
#endif
   /* If the RB CB is marked for deletion, do not further process */
   /* but pass it to DL for similar processing                    */
   if (rbCb->rbOp & 0x10)
   {
      //kwAmmDlMarkForDeletion(rbCb);
   }
   else if(rbCb->rbOp & 0x01)
   {
      /* Since RbCb is marked for re establishment, dont process  */
      /* further for reassembly. Instead pass it to DL.           */
      //kwAmmDlMarkForReestablish(rbCb);
   }
   else
   {
#ifdef MSPD_MLOG_NEW
      startTick = GetTIMETICK();
#endif
      kwAmmUlProcessReassemblyRb(rbCb, numSdus);
      if ((*numSdus >= KW_AM_UM_MAX_UL_SDUS) && (0 == rbCb->rbOp))
      {
         kwUtlUlAddRbToReassemblyLst(rbCb);
      }
#ifdef MSPD
   syscoreinfo[2].res[2]=3;	  
#endif
#ifdef MSPD_MLOG_NEW
      endTick = GetTIMETICK();
      MLogTask(PID_RLC_AM_UTL_RSMBL, RESOURCE_LARM, startTick, endTick);
#endif
   }
   RETVOID;
}
/***************************************************************************/
/***********Functions related to reestablishment starts*********************/
/***************************************************************************/
#ifdef ANSI
PUBLIC Void kwAmmProcessSelfMsgReest
(
KwRbCb                   *rbCb
)
#else
PUBLIC Void kwAmmProcessSelfMsgReest(rbCb)
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
      MSPD_DBG_RLC("[%d][%d]Dont process reestablishment request since delete\
                    flag is set. Post self message for UE delete\n", KW_UEID, 
                    KW_RBID);
      kwAmmAddRbToDeleteLst(rbCb);
      kwUtlPostSelfMsgDelete();
      RETVOID;
   }
   MSPD_DBG("[%d][%d]Process Reestablishment request\n", KW_UEID, KW_RBID);
#ifdef MSPD_MLOG_NEW
   startTick = GetTIMETICK();
#endif
   /* Empty SDUQ                                                            */
   kwUtlEmptySduQ(rbCb, &AMDL.sduQ);
   AMDL.partialSdu = NULLP;

   /* Discard PDUs / byte segments from the txBuf                           */
   kwAmmEmptyTxBuf(rbCb);

   /* Delete PDUs from retxLst                                              */
   kwAmmEmptyRetxLst(rbCb);

   /* Delete PDUs from ToBeRlsTxLst                                         */
   kwAmmEmptyToBeRlsTxLst(rbCb);

   /* Delete PDUs from ToBeRlsReTxLst                                       */
   kwAmmEmptyToBeRlsReTxLst(rbCb);

   /* Delete/Re-assemble PDUs in reassemblyLst and reception buffer         */
   kwAmmReasmbleOnReEst(rbCb);

   /* Reset back the rbOp bits for re-establishment                         */
   rbCb->rbOp &= 0xfe;

   kwUtlSendRbReEstCfm(rbCb);
#ifdef MSPD_MLOG_NEW
   endTick = GetTIMETICK();
   MLogTask(PID_RLC_AM_REEST, RESOURCE_LARM, startTick, endTick);
#endif
}

#ifdef ANSI
PRIVATE Void kwAmmDlMarkForReestablish
(
KwRbCb                 *rbCb
)
#else
PRIVATE Void kwAmmDlMarkForReestablish(rbCb)
KwRbCb                 *rbCb;
#endif
{
   /* RB is first handed over to PDU release list and then moved to the */
   /* reestablish list subsequently                                          */
   //MSPD_DBG("[%d][%d] Marking for DL reestablish\n", 
   //rbCb->rlcId.ueId, rbCb->rlcId.rbId);
   //rbCb->rbOp |= 0x02;
   kwAmmDlAddRbToPduRlsLst(rbCb);
   kwAmmDlPostSelfMsg();
}

#ifdef ANSI
PRIVATE Void kwAmmUlMarkForReestablish
(
KwRbCb                 *rbCb
)
#else
PRIVATE Void kwAmmUlMarkForReestablish(rbCb)
KwRbCb                 *rbCb;
#endif
{
   /* This is to ensure that RB is removed from reassembly if it already  */
   /* is there. It is the responsibility of UL processing to remove the   */
   /* RB from that list and add to DL processing which in turn moves it   */
   /* to DL PDU release list                                              */
   //MSPD_DBG("[%d][%d] Marking for UL reestablish\n", 
   //rbCb->rlcId.ueId, rbCb->rlcId.rbId);
   //rbCb->rbOp |= 0x01;
   kwUtlUlAddRbToReassemblyLst(rbCb);
   kwUtlPostSelfMsgUl(2);
}
/**
 * @brief 
 *
 *        Handler to process the re-establishment request received from UIM
 *
 * @b Description:
 *
 * @param[in]     rbCb     -  RB control block 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PUBLIC S16 kwAmmReEstablish
(
KwRbCb                 *rbCb
)
#else
PUBLIC S16 kwAmmReEstablish(rbCb)
KwRbCb                 *rbCb;
#endif
{
   KwUeCb             *ueCb;
   TRC2(kwAmmReEstablish);
   KWDBGP((KW_DBGMASK_AM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
          (_kwPBuf, "kwAmmReEstablish(rbCb)\n"));

#ifdef SIM_REEST
   if(KW_RBTYPE == CM_LTE_DRB)
   {
      rbCb->wrapCnt = 0;
   }
#endif
   if((rbCb->rlcId.rbType == CM_LTE_SRB) && (rbCb->rlcId.rbId == 1))
   {
#ifdef MSPD
      ysMsAmmReEstReqRcvd++;
#endif
      if(ROK  != kwDbmFetchUeCb(rbCb->rlcId.ueId,rbCb->rlcId.cellId, &ueCb))
      {
          RETVALUE(RFAILED)		  ;
      }
      kwUtlSetReestInProgress(ueCb);
   }

   if(rbCb->rbOp != 0)
   {
#ifdef MSPD
      ysMsAmmErrReEstReqRcvd++;
#endif
      RETVALUE(ROK);
   }
   /* Add this RB to the list of RBs that are processed with lower priority */
   /* Post the self message to RLC only when the 1st Rb is added to the     */
   /* Rb list. Pass the core Id as 2                                        */
   rbCb->rbOp |= 0x01;
   MSPD_DBG_RLC("[%d][%d][%d]Reestablishment request received\n", KW_UEID, 
             KW_RBTYPE, KW_RBID);

   /* Add rbCb to reassemblyRbLst and post self message for UL              */
   //kwAmmUlMarkForReestablish(rbCb);

   kwUtlUlRemRbFrmReassemblyLst(rbCb);
   kwAmmDlRemRbFrmPduRlsLst(rbCb);
   kwAmmDlRemRbFrmStaPduLst(rbCb);
   kwUtlPostSelfMsgUl(2);
   kwAmmDlPostSelfMsg();

   /* Intention is to process all timer related stuffs in core 2          */
   kwAmmStopRbTmrs(rbCb);

   kwUtlAddRbToReestablishLst(rbCb);
   kwUtlPostSelfMsgReest();

   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE Void kwAmmRemRbFrmReestablishLst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmRemRbFrmReestablishLst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.reestRbLstLock);
   if (rbCb->reestablishRbLnk.node != NULLP)
   {
      cmLListDelFrm(&(kwCb.reestablishRbLst), &(rbCb->reestablishRbLnk));
      rbCb->reestablishRbLnk.node = NULLP;
   }
   KW_RDWR_UNLOCK(&kwCb.reestRbLstLock);
   RETVOID;
}
/***************************************************************************/
/******************Functions related to UE delete starts********************/
/***************************************************************************/
/**
 * @brief Handler to check if RLC is completely released.
 *
 * @b Description:
 *        This function checks if RLC deletion is complete and invokes
 *        a utility that checks for completion of deletion of PDCP too
 *        prior to releasing the memory
 *
 * @param [in]    rbCb     -  Radio Bearer Control Block
 *
 * @return  Void
 */
#ifdef ANSI
PRIVATE Void kwAmmRbFree
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmRbFree(rbCb)
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
 *        reassembling of SDUs in AM
 *
 * @b Description: 
 *
 *        This function gets the AM pdu list and traverses through this list
 *        and calls a function to reassemble the SDUs
 *            
 *  @return  Void
 *
 */
U32        tempUlRlsPduCnt = 0;
#ifdef ANSI
PRIVATE Void kwAmmUlProcessDeletionRb
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmUlProcessDeletionRb(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KwAmRecBuf           *pduRecBuf;
   CmLListCp            *pduList;

   TRC2(kwAmmUlProcessDeletionRb);
   kwAmmUlGetPduFrmReassemblyLst(rbCb, &pduRecBuf);
   while(pduRecBuf)
   {
      kwAmmUlRlsPdu(rbCb, pduRecBuf);
      tempUlRlsPduCnt++;
      kwAmmUlGetPduFrmReassemblyLst(rbCb, &pduRecBuf);
   }
   RETVOID;
}

#ifdef ANSI
PUBLIC Void kwAmmProcessSelfMsgDelete
(
KwRbCb                  *rbCb
)
#else
PUBLIC Void kwAmmProcessSelfMsgDelete(rbCb)
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
   MSPD_DBG("COUNT SduQ %d TxLst %d ReTxLst %d\n", AMDL.sduQ.count, 
             AMDL.toBeRlsTxLst.count, AMDL.toBeRlsReTxLst.count);
   
   /* Empty SDUQ                                                            */
   kwUtlEmptySduQ(rbCb, &AMDL.sduQ);

   /* Discard PDUs / byte segments from the txBuf                           */
   kwAmmEmptyTxBuf(rbCb);

   /* Delete PDUs from retxLst                                              */
   kwAmmEmptyRetxLst(rbCb);

   /* Delete PDUs from ToBeRlsTxLst                                         */
   kwAmmEmptyToBeRlsTxLst(rbCb);

   /* Delete PDUs from ToBeRlsReTxLst                                       */
   kwAmmEmptyToBeRlsReTxLst(rbCb);

   /* Delete/PDUs in reassemblyLst and reception buffer                     */
   kwAmmUlProcessDeletionRb(rbCb);

   MSPD_DBG("COUNT SduQ %d TxBuf %d TxLst %d ReTxLst %d UlRls %d\n", 
             AMDL.sduQ.count, tempTxBufCnt, AMDL.toBeRlsTxLst.count, 
             AMDL.toBeRlsReTxLst.count, tempUlRlsPduCnt);
   tempTxBufCnt = 0;
   tempUlRlsPduCnt = 0;
   /* Reset back the rbOp bits for delete                                   */
   rbCb->rbOp = 0;


   kwAmmRbFree(rbCb);
#ifdef MSPD_MLOG_NEW
   endTick = GetTIMETICK();
   MLogTask(PID_RLC_AM_RB_DELETE, RESOURCE_LARM, startTick, endTick);
#endif
}

#ifdef ANSI
PUBLIC Void kwAmmGetNumRbInDeleteLst
(
U16                     *numRb
)
#else
PUBLIC Void kwAmmGetNumRbInDeleteLst(numRb)
U16                     *numRb
#endif
{
   KW_RDWR_LOCK(&kwCb.deleteRbLstLock);
   {
      *numRb = cmLListLen(&(kwCb.deleteRbLst));
   }
   KW_RDWR_UNLOCK(&kwCb.deleteRbLstLock);
}

#ifdef ANSI
PRIVATE Void kwAmmAddRbToDeleteLst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmAddRbToDeleteLst(rbCb)
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
PRIVATE Void kwAmmRemRbFrmDeleteLst
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmRemRbFrmDeleteLst(rbCb)
KwRbCb                  *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.deleteRbLstLock);
   cmLListDelFrm(&kwCb.deleteRbLst, &rbCb->deleteRbLnk);
   rbCb->deleteRbLnk.node = NULLP;
   KW_RDWR_UNLOCK(&kwCb.deleteRbLstLock);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void kwAmmGetRbFrmDeleteLst
(
KwRbCb                  **rbCb
)
#else
PUBLIC Void kwAmmGetRbFrmDeleteLst(rbCb)
KwRbCb                  **rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.deleteRbLstLock);
   {
      if (kwCb.deleteRbLst.first == NULLP)
      {
         *rbCb = NULLP;
      }
      else
      {
         (*rbCb) = (KwRbCb *)(kwCb.deleteRbLst.first->node);
         kwCb.deleteRbLst.first->node = NULLP;
         cmLListDelFrm(&kwCb.deleteRbLst, kwCb.deleteRbLst.first);
      }
   }
   KW_RDWR_UNLOCK(&kwCb.deleteRbLstLock);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void kwAmmDlMarkForDeletion
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmDlMarkForDeletion(rbCb)
KwRbCb                  *rbCb;
#endif
{
   /* RB is first handed over to PDU release list and then moved to the */
   /* delete list subsequently                                          */
   //MSPD_DBG("[%d][%d][%d] Marking for DL deletion\n", KW_UEID, KW_RBTYPE,
   //          KW_RBID);
   //rbCb->rbOp |= 0x08;
   kwAmmDlAddRbToPduRlsLst(rbCb);
   kwAmmDlPostSelfMsg();
}

#ifdef ANSI
PRIVATE Void kwAmmUlMarkForDeletion
(
KwRbCb                  *rbCb
)
#else
PRIVATE Void kwAmmUlMarkForDeletion(rbCb)
KwRbCb                  *rbCb;
#endif
{
   /* This is to ensure that RB is removed from reassembly if it already  */
   /* is there. It is the responsibility of UL processing to remove the   */
   /* RB from that list and add to DL processing which in turn moves it   */
   /* to DL PDU release list                                              */
   //MSPD_DBG("[%d][%d][%d] Marking for UL deletion\n", KW_UEID, KW_RBTYPE,
   //          KW_RBID);
   //rbCb->rbOp |= 0x04;
   kwUtlUlAddRbToReassemblyLst(rbCb);
   kwUtlPostSelfMsgUl(2);
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
PUBLIC S16 kwAmmDeleteRb
(
KwRbCb            *rbCb
)
#else
PUBLIC S16 kwAmmDeleteRb(rbCb)
KwRbCb            *rbCb;
#endif
{
   TRC2(kwAmmDeleteRb)

   rbCb->rbOp |= 0x10;

   /* Here we mark the UE for deletion. Given that UL and DL may be on    */
   /* two different cores, RB is handed over from one direction to the    */
   /* other in a sequential manner to avoid any contention                */
   //kwAmmUlMarkForDeletion(rbCb);

   kwUtlUlRemRbFrmReassemblyLst(rbCb);
   kwAmmDlRemRbFrmPduRlsLst(rbCb);
   kwAmmDlRemRbFrmStaPduLst(rbCb);
   pjDlmRemRbFrmReestLst(&rbCb->rb->pjRbCb);
   kwUtlPostSelfMsgUl(2);
   kwAmmDlPostSelfMsg();
   /* Added to reset all BO for this RB since it is to be deleted         */
   kwAmmResetAllBo(rbCb);
   KW_AM_SND_DSTA_RSP();

   kwAmmAddRbToDeleteLst(rbCb);
   kwUtlPostSelfMsgDelete();

   RETVALUE(ROK);
}
/**
 *
 * @brief Private handler to mark a retx PDU for further retransmission
 *
 * @b Description 
 *        This function is added to protect the simultaneous wr access
 *        for retxBo variable.
 *
 * @param[in]  amdl
 * @param[in]  size
 *
 *  @return Void 
 *
 */
PRIVATE Void kwAmmAddRetxBo
(
KwAmDl                 *amDl,
MsgLen                 size
)
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   amDl->retxBo   += size;
   KW_RDWR_UNLOCK(&kwCb.dlLock);
}


/**
 *
 * @brief Private handler to mark a retx PDU for further retransmission
 *
 * @b Description 
 *        This function is added to protect the simultaneous wr access
 *        for estHdrSz variable.
 *
 * @param[in]  amdl
 * @param[in]  size
 *
 *  @return Void 
 *
 */
PRIVATE Void kwAmmAddEstHdrSize
(
KwAmDl                 *amDl,
MsgLen                 size
)
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   amDl->estHdrSz += size;
   KW_RDWR_UNLOCK(&kwCb.dlLock);
}

PRIVATE Void kwAmmDecEstHdrSize
(
KwAmDl                 *amDl,
MsgLen                 size
)
{
   KW_RDWR_LOCK(&kwCb.dlLock);
   amDl->estHdrSz -= size;
   KW_RDWR_UNLOCK(&kwCb.dlLock);
} 

#ifdef ANSI
PUBLIC Void kwAmmResetAllBo
(
KwRbCb                 *rbCb
)
#else
PUBLIC Void kwAmmResetAllBo(rbCb)
KwRbCb                 *rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.sduLock);
   {
      AMDL.bo = 0;
      AMDL.cntrlBo = 0;
      AMDL.retxBo = 0;
      AMUL.cntrlBo = 0;
   }
   KW_RDWR_UNLOCK(&kwCb.sduLock);
}


/********************************************************************30**
  
         End of file:     gp_amm.c@@/main/2 - Fri Nov 13 14:13:34 2009
  
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
/main/1      ---       nm         1. Initial release.
/main/2      ---       nm         1. LTERLC Release 2.1.
/main/3      kw001.201 vp         1. Patch  for HENB, changes for max RBs
                                  in cell and avoidance of float variables.
/main/4      kw002.201 vp         1. Patch for optimization,fixes for multi-region
                                     and retransmission.
/main/5      kw003.201 vp         1. Fixes for re-segmentation, segement offset
                                     value and optimization
/main/6      kw004.201 av         1. Adding of missing trace in RLC PDCP
                                  2. Modified the definition of kwAmmDlRlsPdusSet
                                  3. byteWoPoll assignement have been corrected in 
                                     the function kwChkandSetPoll.
/main/7      kw005.201 ap         1. Removed redundant code from function kwCreatePdu and
                                     kwAmmPollRetxTmrExp.
                                  2. Removed warning from the code.
                                  3. Updated statistics in the code.
                                  4. Added L2 Measurement feature.
                                  5. Fixed the problem in AssembleSdu function when pduSz is
                                     less than minimum rlc header size. Fix for ccpu00118973
                       rd         6. Added changes in function kwAmmProcessPdus for RLC
                                     R9 upgrade corresponding to CR0083 and CR0090.
                                     Added a condition for stopping the t-reordering 
                                     timer. Updation of VR(H) is moved before the 
                                     updation of VR(MS).
                                  7. The "=" in the condition for triggering the status
                                     report is removed according to product CR 
                                     ccpu00117114 in function kwTriggerStatus.
                                  8. Added code for L2 Meas DL/UL ip throughput.
                                  9. Incorporated the important Avtecs comments.
                                 10. Bug fix: ccpu00119438.
/main/8      kw006.201 rd         1. Splitted the function kwAmmCalDlIpThrPut into 4 
                                     functions:kwAmmMeasDlIpBrstCont, kwAmmMeasDlIpBrstStart,
                                     kwAmmMeasDlIpBrstReset, kwAmmMeasDlIpBrstComplt.
                                  2. Extracted the L2M related code from function 
                                     kwAmmProcessPdus to kwAmmCalUlIpThrPut.
                       ap         3. Removed 64 bit compilation warning.
                                  4. Fixed timer stopping problem in the product.
                       rd         5. Interchanged the order of condition in "if" cond
                                     in kwSplitPduWithLi and added a check or liCnt
                                     in the next "if" statement.
                                  6. Added boundary condition check for "j" before
                                     calling KW_COPY_STATUS() in kwAmmPrepStatusPdu.
                                  7. Updated the code with BG processing for AM
                                     reassembly under macro KW_BG_UL_PROC for CR ccpu00117290
                                     and added a function kwAmmProcessSelfMsgUl.
                       ap         8. Changes for ccpu00120505, changed parameter passed to the
                                     function kwUpdFrmTxtoRetx from sn to nackSn.
                       rd         9. Put the code for decrementing estHdrSize and retxBo
                                     under #ifndef KW_BG_DL_PROC in function kwRemRetxPdu.
                                 10. ccpu00117290:Added alarm code kwAmmChkReasmblyLstSz().
                       ap        11. ccpu00120574 warning fix.
                       rd        12. Self post of message to RLC is done when new
                                     RB is added to RbList and when PduList is not
                                     empty during self message processing.
*********************************************************************91*/
