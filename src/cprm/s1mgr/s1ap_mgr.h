/**
 * @file    s1ap_mgr.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @details
 * This file contains S1AP Manager function prototypes
 *
 * @author Lakshmi Narayana Madala
 *
 */

#include "s1ap_cm.h"
#include "bsm_s1ap_if.h"
#include "s1ap.h"

#ifndef S1AP_MGR_H
#define S1AP_MGR_H


#define S1_LINK_UP                              1
#define S1_LINK_DOWN                            0

#define TMR_UNITS_PER_SEC                      1000 /* Timer unit is in milli seconds */

/* #define ERR_RCVD_SETUPFAIL_FROM_MME                 "Received S1 Setup Fail from MME, without timeout"*/
#define ERR_RCVD_MAX_RETRY_EXHAUSTED                "S1 Max retries exhausted"
#define ERR_RCVD_SCTP_ASSOC_DOWN_IND                "Received SCTP Association Down Indication"
#define ERR_GUARD_TIMER_EXPIRED                     "S1AP Procedure Guard Timer Expired"

#define NO_BITS_IN_UINT32                       32

#define fillTransId( uDatEvnt, tId)             {  \
                                                      uDatEvnt.transId.pres = PRSNT_NODEF; \
                                                      uDatEvnt.transId.val = tId; \
                                                    }
#define fillPeerId( uDatEvnt, pId)             {  \
                                                      uDatEvnt.peerId.pres = PRSNT_NODEF; \
                                                      uDatEvnt.peerId.val = pId; \
                                                    }


S1apMgrCb *getS1MgrCb(
    UINT32 peerId
    );
void s1apGetMmeIpAddr(
    IO MsgS1apBsmS1LinkStatus      *s1LinkStatus,
    IN S1apMgrCb                   *s1Cb
    );
void s1apLinkStatusToBsm(
     IN UINT16        linkStatus,
     IO S1apMgrCb     *s1Cb
  );
void s1apSendLinkStatusToUeh(
    IN TBOOL status
    );
void s1apSendOpstartFailClearSctp(
    IN BsErrorValue errorNum,
    IN SINT8        *failReason,
    IO S1apMgrCb    *s1Cb
    );
void s1apSendToBsm(
    MsgId   msgId,
    U16     len,
    U8     *data
    );
S16 s1apBuildSzPeerDel(
    IN U32           peerId,
    IO S1apMgrCb    *s1Cb
  );

void s1apInitS1MgrCb(
    IO S1apMgrCb       *s1apMgrCb,
    IN MsgBsmS1apConfigReq     *s1apCfgInfo
    );

void s1apSndResetToPeerOamInt( void );
void s1apProcS1setupTmrExpiry(
    IO S1apMgrCb     *s1Cb
    );
void s1apProcS1GuardTmrExpiry(
    IO S1apMgrCb     *s1Cb
    );

void s1apProcOpStartReq(
    IN Buffer *mbuf
    );
void s1apProcOpStopReq(
    IN Buffer *mbuf
    );
void s1apProcS1SetupRsp(
    IN SztUDatEvnt *uDatEvnt
    );
void  s1apProcS1SetupFail(
    IN SztUDatEvnt *uDatEvnt
    );
void  s1apProcS1ResetAck(
    IN SztUDatEvnt *uDatEvnt
    );
void  s1apProcS1Reset(
    IN SztUDatEvnt *uDatEvnt
    );
void  s1apProcMmeCfgUpdate(
    IN SztUDatEvnt *uDatEvnt
    );
void  s1apProcOverloadStart(
    IN SztUDatEvnt *uDatEvnt
    );
void  s1apProcOverloadStop(
    IN SztUDatEvnt *uDatEvnt
    );
void  s1apProcPagingMsg(
    IN SztUDatEvnt *uDatEvnt
    );
PUBLIC S16 s1apProcStatInd(
    IN Pst            *post,
    IN SuId           suId,
    IN SztStaInd      *sztSta
  );
void  s1apProcWarningReq(
    IN SztUDatEvnt *uDatEvnt
    );
#endif /* S1AP_MGR */
