/* header include files (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_lte.h"        /* Common LTE Defines              */
#include "ctf.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_lte.x"        /* Common LTE library              */
#ifdef PWAV_TS
#include "ts.h"            /* Upperarm and Lowerarm timestamp set */

U32             gSf_b1;
U32             gSf_b2;

U32             gRtt_a;
U32             gRtt_b;

S16 cmPkAndSndSetTsReq (
    Pst       *pst,
    SetTsReq  *setTsReq
  )
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
    RETVALUE(RFAILED);
  }

  CMCHKPK(SPkU32, setTsReq->a1, mBuf);
  
  pst->event = (Event) EVTCTFUEHSETTSREQ;
  RETVALUE(SPstTsk(pst,mBuf));
}

S16 cmUnpkSetTsReq (
    CtfSetTsReq func,
    Pst         *pst,
    Buffer      *mBuf
  )
{
  SetTsReq      setTsReq;

  CMCHKUNPK(SUnpkU32, &setTsReq.a1, mBuf);
  
  SPutMsg(mBuf);
  RETVALUE((*func)(pst, &setTsReq));
}

S16 cmPkAndSndSetTsRsp (
    Pst       *pst,
    SetTsRsp  *setTsRsp
  )
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
    RETVALUE(RFAILED);
  }

  CMCHKPK(SPkU32, setTsRsp->a1, mBuf);
  CMCHKPK(SPkU32, setTsRsp->b1, mBuf);
  
  pst->event = (Event) EVTCTFUEHSETTSRSP;
  RETVALUE(SPstTsk(pst,mBuf));
}

S16 cmUnpkSetTsRsp (
    CtfSetTsRsp func,
    Pst         *pst,
    Buffer      *mBuf
  )
{
  SetTsRsp      setTsRsp;

  CMCHKUNPK(SUnpkU32, &setTsRsp.b1, mBuf);
  CMCHKUNPK(SUnpkU32, &setTsRsp.a1, mBuf);
   
  SPutMsg(mBuf);
  RETVALUE((*func)(pst, &setTsRsp));
}

S16 cmPkAndSendSetTsCfm (
    Pst       *pst,
    SetTsCfm  *setTsCfm
  )
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
    RETVALUE(RFAILED);
  }

  CMCHKPK(SPkU32, setTsCfm->b1, mBuf);
  CMCHKPK(SPkU32, setTsCfm->a2, mBuf);
  CMCHKPK(SPkU32, setTsCfm->rtt_a, mBuf);
  
  pst->event = (Event) EVTCTFUEHSETTSCFM;
  RETVALUE(SPstTsk(pst,mBuf));
}

S16 cmUnpkSetTsCfm (
    CtfSetTsCfm func,
    Pst         *pst,
    Buffer      *mBuf
  )
{
  SetTsCfm  setTsCfm;

  CMCHKUNPK(SUnpkU32, &setTsCfm.rtt_a, mBuf);
  CMCHKUNPK(SUnpkU32, &setTsCfm.a2, mBuf);
  CMCHKUNPK(SUnpkU32, &setTsCfm.b1, mBuf);

  SPutMsg(mBuf);
  RETVALUE((*func)(pst, &setTsCfm));
}
#endif

