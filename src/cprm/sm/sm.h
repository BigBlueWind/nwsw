/**
 * @file    sm.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains all function definitions and Macros required by SM module.
 *
 * @author  mkata
 * @date    07-Jan-2011
 */
#ifndef SM_H_
#define SM_H_

#include "sm_includes.h"

/* Macros */
#define         MAX_SIZE_OF_TRC_MSG             255

/* Function prototypes */
/* sm_bsmsm_exms.c */
PUBLIC S16 sendSmMsgToBsm( Event   event, MsgLen     len, U8     *data );
void saveUehCfgInSmCb(MsgBsmUehEnbConfigReq   *enbConfigReq, SmCfgCb *smCfgCallBlock );
void cemProcEnbCfgReq(Buffer *mBuf, Pst *pst );
U32 smGetCcpuTrcFlagFromPwavFlag( TraceCommand     trcStatus );
void smFillMacTrcCntrlReq( RgMngmt  *rgCntrlReq, S16  element, S16  action, S16  subAction );
void smProcMacTrcReq( Buffer *mBuf );
void smFillRlcTrcCntrlReq( KwMngmt *kwCntrlReq, S16     element, S16     action, S16     subAction );
void sfiProcRlcTrcReq( Buffer *mBuf,U8 trcFlag );
void smFillEgtpTrcCntrlReq( EgMngmt *egCntrlReq, S16     element, S16     action, S16     subAction, SpId    tSapId );
void sfiProcEgtpTrcReq( Buffer *mBuf );
void smFillPdcpTrcCntrlReq( PjMngmt *pjCntrlReq, S16     element, S16     action, S16     subAction );
void smProcPdcpTrcReq( Buffer *mBuf,U8 trcFlag );
void smFillS1apTrcCntrlReq( SzMngmt *s1apCntrlReq, S16     element, S16     action, S16     subAction );
void smProcS1apTrcReq( Buffer *mBuf,U8 trcFlag );
void smProcRlcTrcReq( Buffer *mBuf,U8 trcFlag );
void smFillRrcTrcCntrlReq( NhMngmt *rrcCntrlReq, S16     element, S16     action, S16     subAction );
void smFillNhRlcDataSapTrcCntrlReq( NhMngmt *rrcCntrlReq, S16      action );
void smFillNhRlcControlSapTrcCntrlReq( NhMngmt *rrcCntrlReq, S16   action );
void smFillNhMacSapTrcCntrlReq( NhMngmt *rrcCntrlReq, S16   action );
void smFillNhPdcpDataSapTrcCntrlReq( NhMngmt *rrcCntrlReq, S16     action );
void smFillNhPdcpControlSapTrcCntrlReq( NhMngmt *rrcCntrlReq, S16      action );
void smProcRrcTrcReq( Buffer *mBuf,U8 trcFlag );
void smProcSetLogCfg( Buffer *mBuf );
void smProcTrcStartOrStopReq(Buffer *mBuf);
PUBLIC S16 bsmSmActvTsk( Pst         *pst, Buffer      *mBuf );
PUBLIC S16 lthSmActvTsk( Pst         *pst, Buffer      *mBuf );

PUBLIC S16 sendEventToOam (  Ent srcEnt, Ent dstEnt, Event event, MsgLen len, U8  *data );
PUBLIC S16 cmPkLrgCntrlReq ( Pst * pst, RgMngmt * cntrl);

PUBLIC S16 cmPkLkwCntrlReq ( Pst *pst,KwMngmt *cntrl );
PUBLIC S16 cmPkLegCntrlReq ( Pst* pst, EgMngmt * cntrlReq);
PUBLIC S16 cmPkLpjCntrlReq ( Pst * pst, PjMngmt * cntl);
PUBLIC S16 cmPkLszCntrlReq ( Pst *pst, SzMngmt *cntrl);
/*BS-1799 Cell Barring*/
void cemProcCellBarReq(Buffer *mBuf);
void cemProcCellUnbarReq(Buffer *mBuf);

/***********************************************************************/
/* sm_pmhsm_exms.c */

PUBLIC S16 sendSmMsgToLth( Event   event,MsgLen    len, U8     *data );
//#define sendSmMsgToPmh (_event,_len,_data) sendEventToOam(ENTSM, ENT_PMH, _event, _len, _data)

PUBLIC S16 sendSmMsgToPmh( Event   event,MsgLen    len, U8     *data );

void smFillPdcpStsReq( PjMngmt *pjCntrlReq, S16      element );
void smProcPdcpStsReq( Pst     *rcvdPst,    Buffer   *mBuf );
void smFillSctpStsReq( SbMgmt  *sbCntrlReq, S16      element );
void smProcSctpStsReq( Pst     *rcvdPst,    Buffer   *mBuf );
void smFillEgtpStsReq( EgMngmt *egCntrlReq, S16      element );
void smProcEgtpStsReq( Pst     *rcvdPst,    Buffer   *mBuf );
void smFillRlcStsReq( KwMngmt  *kwCntrlReq, S16      element );
void smProcRlcStsReq( Pst      *rcvdPst,    Buffer   *mBuf );
void smFillS1apStsReq( SzMngmt *szCntrlReq, S16      element );
void smProcS1apStsReq( Pst     *rcvdPst,    Buffer   *mBuf );
void smFillRrcStsReq( NhMngmt  *nhCntrlReq, S16      element );
void smProcRrcStsReq( Pst      *rcvdPst,    Buffer   *mBuf );
void smFillMacStsReq( RgMngmt  *rgCntrlReq, S16      element );
void smProcMacStsReq( Pst      *rcvdPst,    Buffer   *mBuf );
PUBLIC S16 pmhSmActvTsk( Pst   *pst,        Buffer   *mBuf );

S16 cmPkLpjStsReq ( Pst * pst, Action actn, PjMngmt * sts);
S16 cmPkLsbStsReq (Pst *pst, Action action, SbMgmt *sts);
S16 cmPkLegStsReq (Pst* pst, Action action, EgMngmt * stsReq);
S16 cmPkLkwStsReq ( Pst *pst, Action action, KwMngmt *sts);
S16 cmPkLszStsReq ( Pst *pst, Action action, SzMngmt *sts);
S16 cmPkLrgStsReq ( Pst *pst, RgMngmt *sts);

/***********************************************************************/
/* sm_nh_ptmi.c */
void uehNhProcTrcCtrlRsp( NhMngmt *cntrl, U8 *trcMsgId);

/***********************************************************************/
PUBLIC S16 smBindUehToUdhSap ( Void);
//PUBLIC S16 smUdhActvInit ( Ent ent, Inst inst, Region region, Reason reason);
PUBLIC S16 smUdhActvTsk ( Pst *pst, Buffer *mBuf);
#endif /* SM_H_ */
