
#ifndef UEH_FUNC_H
#define UEH_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif
#include "envdep.h"
#include "ccpu_cmn.h"
#include "libconfig.h"
#include "ludh.h"
#include "lueh.x"
#include "ueh_types.h"
#include "log.h"


#define SWAP32(x) ((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) |(((x) & 0xff0000) >> 8) | (((x) >> 24) & 0xff))
#define SWAP16(x)   (((x & 0x00FF) << 8) |((x & 0xFF00) >> 8))
#define CONVERT_ASCII_TO_INT(ascii) ascii - '0'

#define UEH_FILL_PST(_pst,_srcProc,_dstEnt,_srcEnt,_event,_pool,_selector) \
{                                                                \
   cmMemset((U8 *)&(_pst), 0, sizeof(Pst));                       \
   _pst.dstProcId = SM_SM_PROC;                                    \
   _pst.srcProcId = _srcProc;                                    \
   _pst.dstEnt = _dstEnt;                                        \
   _pst.srcEnt = _srcEnt;                                        \
   _pst.dstInst = 0;                                             \
   _pst.srcInst = 0;                                             \
   _pst.event  = _event;                                         \
   _pst.pool   = _pool;                                         \
   _pst.selector= _selector;                                     \
}

#define SM_SET_ZERO(_buf, _size)   \
    cmMemset((U8 *)(_buf), 0, _size);

#define SM_FREE(_region, _pool, _buf, _size)          \
do                                                    \
{                                                     \
   if (_buf != NULLP)                                 \
   {                                                  \
   (Void) SPutSBuf(_region, _pool, (Data *) _buf,     \
   (Size) _size);                                     \
   (_buf) = NULLP;                                    \
   }                                                  \
}                                                     \
while(0)

#define UEHSM_ALLOC(_buf, _size)         \
do                                                    \
{                                                     \
   if (SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)(_buf),      \
                _size) == ROK)                        \
   {                                                  \
      SM_SET_ZERO((*_buf), _size);                    \
   }                                                  \
   else                                               \
   {                                                  \
      (*_buf) = NULLP;                                \
   }                                                  \
}                                                     \
while(0)

#define uehUeProcStart(ueCb,procId)\
 logProc("crnti=%d|proc|prev=%d|new=%d (%s)|", ueCb->crnti, ueCb->ueProcedure, procId, #procId), \
 ueCb->ueProcedure=procId

#define uehUeProcSuccess(ueCb,procId)\
do {\
  if (ueCb->ueProcedure EQ procId) \
    { logProc("crnti=%d|procId=%d|SUCC", ueCb->crnti, procId); }\
  else \
    { logError("crnti=%d|currentProc=%d|Does not match|procId=%d", ueCb->crnti, ueCb->ueProcedure, procId ); }\
  } while(0)

#define uehUeProcFail(ueCb,procId,failureCause)\
{\
  if(ueCb->ueProcedure NOTEQ procId)\
   {\
       logError("crnti=%d|currentProc=%d|Does not match|procId=%d", ueCb->crnti, ueCb->ueProcedure, procId );\
   }\
  else \
    logProc("crnti=%d|procId=%d|FAIL|cause=%d", ueCb->crnti, procId, failureCause); \
}

char *uehGetUeProcedureName(      
  UehUeProcedure proc             
);
/* 
 * Message building related macros 
 * */

/* Extract the bits from SFN */
#define UEH_GET_SFN_MIB(_var, _sfnInt)  \
{\
   U8 _byte = 0;    \
   _byte = (U8)((_sfnInt & 0x0003FC) >> 2);    \
   _var = _byte; \
}

/**
 * @details Macro for SGetMsg.
 */
#define UEH_ALLOCMBUF(_region, _pool, _buf)            \
do                                                    \
{                                                     \
   if (SGetMsg((_region), (_pool), (Buffer **)(_buf)) != ROK)    \
   {                                                  \
      UEH_ERROR_MEM_ALLOC_FAILED( 0, "mBuf");           \
      (*(_buf)) = NULLP;                              \
   }\
}                                                     \
while(0)

/**
 * @details Macro for SPutMsg.
 */
#define UEH_FREEMBUF(_mBuf)                            \
do{                                                     \
   if (_mBuf != NULLP)                                               \
   {                                                  \
      (Void)SPutMsg((_mBuf));                            \
      (_mBuf) = NULLP;                                   \
   }                                                  \
   else logError("%s", "Trying to free NULL mBuf");\
}while(0)

/* initialize the memCp & allocate memory for the eUEHnt structure */
#define UEH_ALLOCEVNT_OR_RET(_evntPtr, _size)\
{\
   (*_evntPtr) = NULLP; \
   if(cmAllocEvnt((Size) _size, UEH_MEM_PDU_SIZE, &uehCb.mem,  (Ptr *)(_evntPtr)) != ROK)  \
   {                                            \
      UEH_ERROR_MEM_ALLOC_FAILED( _size, "evnt");           \
      RETVALUE(RFAILED);  \
   }                        \
}

/* allocate memory for members in the event structure */
#define UEH_GET_MEM(_memPtr, _size, _allocPtr)\
{\
   (*_allocPtr) = NULLP;    \
   if(cmGetMem((U8 *)_memPtr, _size, (Ptr *)_allocPtr) != ROK)    \
   {                                                              \
       UEH_ERROR_MEM_ALLOC_FAILED( _size, "cmGetMem");           \
   }                                                              \
}
/* macro to fill TknUInt */ 
#define UEH_FILL_TKN_UINT(_ptr, _val) {\
   _ptr.pres = PRSNT_NODEF;\
   _ptr.val = _val;\
}
/* macro to fill TknUInt NOTPRSNT*/ 
#define UEH_FILL_TKN_UINT_NOTPRSNT(_ptr, _val) {\
   _ptr.pres = NOTPRSNT;\
   _ptr.val = _val;\
}

#define UEH_MEM_COPY(_dst, _src, _len) \
   cmMemcpy((U8*) (_dst), (U8*) (_src), _len)


#define UEH_FREEEVNT(_evntPtr)       \
{                                      \
   cmFreeMem((_evntPtr));              \
   (_evntPtr) = NULLP;                 \
}

#define UEH_DBG_ERROR(_args) \
{ \
   DBGP(&(uehCb.init), UEHLAYERNAME, UEH_ERROR, _args); \
}



#define allocInternalUeId() ((uehCb.ueCounter=(uehCb.ueCounter % MAX_INTERNAL_UEID+1)))
#define getInternalUeId()   (uehCb.ueCounter)

#define UEH_ALLOC(_buf, _size)         \
{\
   if (SGetSBuf(UEH_MEM_REGION, UEH_MEM_POOL, (Data **)(_buf), _size) == ROK)\
   {                                                  \
      UEH_SET_ZERO((*_buf), _size);                    \
   }                                                  \
   else                                               \
   {                                                  \
      UEH_ERROR_MEM_ALLOC_FAILED( _size, "SBuf");           \
      (*_buf) = NULLP;                                \
   }                                                  \
}

#define UEH_SET_ZERO(_buf, _size) \
{\
  cmMemset((U8 *)(_buf), 0, _size);\
}

#define UEH_SET_ONE(_buf, _size) \
{\
  cmMemset((U8 *)(_buf), 1, _size);\
}

#define UEH_GET_MIN(_val1, _val2) ((_val1 < _val2) ? (_val1) : (_val2));

#define UEH_FREE(_buf, _size)          \
{\
   (Void) SPutSBuf(UEH_MEM_REGION, UEH_MEM_POOL, (Data *) _buf, (Size) _size); \
   (_buf) = NULLP;\
}


#define UEH_SET_RGR_TRANS(_arr, _cfgTyp, _lcId, _crnti)          \
{\
   U16 _tempByte = 0;\
   _arr[0] = (U8) _cfgTyp;\
   _arr[1] = (U8) _lcId;\
   _tempByte = (_crnti & 0xff);\
   _arr[2] = (U8) _tempByte;\
   _tempByte = (_crnti >> 8);\
   _arr[3] = (U8) _tempByte;\
}

#define UEH_GET_VALS_RGR_TRANS(_arr, _cfgTyp, _lcId, _crnti)          \
{\
   U16 _tempByte = 0;\
   _cfgTyp = _arr[0];\
   _lcId = _arr[1];\
   _tempByte = _arr[3];\
   _tempByte = ((_tempByte << 8) | _arr[2]);\
   _crnti = _tempByte;\
}
#define UEH_SET_CTF_TRANS(_arr, _cfgTyp, _id)          \
{\
   _arr[0] = (U8) _cfgTyp;\
   _arr[1] = (U8) _id;\
}

#define UEH_GET_VALS_CTF_TRANS(_arr, _cfgTyp, _id)          \
{\
   _cfgTyp = _arr[0];\
   _id = _arr[1];\
}

#define UEH_EVT_HIS_LOG_PROC(ueIdx,ueEventHisRsp)\
{\
  if (1 == (uehCb.ueEventHistoryLogConfig.ueProcedureBitMap & 1))\
  {\
    if (1 == (uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus & 1))\
    {\
      event = EVENTTYPE_RRCCONNESTASUCC;\
      fillProcDetail(event,EVENTTYPE_RRCCONNSETUPCOMPL,ueEventHisRsp,ueIdx);\
    }\
  }\
  if (2 == (uehCb.ueEventHistoryLogConfig.ueProcedureBitMap & 2))\
  { \
    if (2 == (uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus & 2))\
    {\
      event = EVENTTYPE_SECURITYSUCC;\
      fillProcDetail(event,EVENTTYPE_SECMODECOMPL,ueEventHisRsp,ueIdx);\
    }\
  }\
  if (8 == (uehCb.ueEventHistoryLogConfig.ueProcedureBitMap & 8))\
  { \
    if (8 == (uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus & 8))\
    {\
      event = EVENTTYPE_UECAPABILITYSUCC;\
      fillProcDetail(event,EVENTTYPE_UECAPABILITYINFO,ueEventHisRsp,ueIdx);\
    }\
  }\
  if (16 == (uehCb.ueEventHistoryLogConfig.ueProcedureBitMap & 16))\
  { \
    if (16 == (uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus & 16))\
    {\
      event = EVENTTYPE_RRCRECONFIGSUCC;\
      fillProcDetail(event,EVENTTYPE_RRCCONNRECONFCOMPL,ueEventHisRsp,ueIdx);\
    }\
  }\
  if (32 == (uehCb.ueEventHistoryLogConfig.ueProcedureBitMap & 32))\
  {\
    if (32 == (uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus & 32))\
    { \
      event = EVENTTYPE_INITIALCTXSETUPSUCC;\
      fillProcDetail(event,EVENTTYPE_INITCONTEXTSETUPRESP,ueEventHisRsp,ueIdx);\
    }\
  }\
  if (64 == (uehCb.ueEventHistoryLogConfig.ueProcedureBitMap & 64))\
  { \
    if (64 == (uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus & 64))\
    {\
      event = EVENTTYPE_UECTXRELEASESUCC;\
      fillProcDetail(event,EVENTTYPE_RRCCONNRELEASE,ueEventHisRsp,ueIdx);\
    }\
  }\
}

/* Patch ve002.101 */
#define UEH_GET_U32_FRM_OSXL(_var, _tkn) \
{ \
   U16 _len = 0;\
   U16 _idx = 0;\
   U32 _finalVal = 0;\
   _len = _tkn.len;\
   if(_len > 4)\
   _len = 4;\
   for(_idx = 0; _idx < _len; _idx++) \
   {\
      _finalVal = ((_finalVal << 8) | (_tkn.val[_idx])); \
   }\
   _var = _finalVal; \
}

#define getUint32FromArray( data ) ( ( (data[0]) << 24) | ( (data[1]) << 16) | ( (data[2]) << 8) | ( data[3] ) );

#define UEH_GET_LOC_TUNNEL_ID(_idx)\
do{\
   _idx = (++(_idx)) % UEH_MAX_TUNNEL_ID; \
   if ((_idx) == 0) ++(_idx); \
}while(0);

#define UEH_GET_UE_IDX(_crnti) (_crnti - uehCb.uehCfgCb.cellCfg.startRnti)
#define isUeIndexValid(_ueIdx, maxVal) ( ( _ueIdx >= maxVal || _ueIdx < 0) ? FALSE : TRUE )
/*    UEH_ERROR_ELMNT_OUT_OF_RANGE("ueIdx", _ueIdx, 0, maxVal); */

#define UEH_LOGERROR(errCls, errCode, errVal, errDesc) \
do \
{ \
   { \
    SLogError(uehCb.init.ent, uehCb.init.inst, uehCb.init.procId,     \
          (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
          (Txt *)errDesc); \
   } \
}while(0)


#define UEH_GET_UECB_FROM_CRNTI(_ueCb ,_crnti)\
{\
   U8          _ueIdx = 0; \
   UehCellCb *_cellCb = uehGetCellCb(); \
   _ueIdx = _crnti -  uehCb.uehCfgCb.cellCfg.startRnti; \
   if (  (_ueIdx >= 0) && \
         (_ueIdx <  MAX_UE_SUPP) && \
         (TRUE EQ _cellCb->ueCbLst[_ueIdx].isUeCbAllocated) && \
         (_cellCb->ueCbLst[_ueIdx].crnti EQ _crnti)\
     )\
     {\
        _ueCb = &(_cellCb->ueCbLst[_ueIdx]);\
     }\
   else _ueCb = NULLP;\
}

#define UEH_GET_UECB_FROM_UEIDX(_ueCb ,_ueIdx)   _ueCb = &((uehGetCellCb())->ueCbLst[_ueIdx])

#define UEH_UECB_RB_SET_TNL_STATUS(_ueCb, _teId) \
{\
  U8 _idx;\
  U8 _drbIdx;\
  for(_idx = 0; _idx < _ueCb->rbInfo.numOfDrbAdd; _idx++)\
  {\
    _drbIdx = _ueCb->rbInfo.drbAddLstIndx[_idx];\
    if(_teId == _ueCb->rbInfo.drb[_drbIdx].locEgtpTunId)\
    {\
      _ueCb->rbInfo.drb[_drbIdx].isEgtpTnlEnbl = TRUE;\
      break;\
    }\
  }\
}

#define UEH_UECB_RB_SET_RAB_STATUS(_ueCb, _rbId) \
{\
  U8 _idx;\
  U8 _drbIdx;\
  for(_idx = 0; _idx < _ueCb->rbInfo.numOfDrbAdd; _idx++)\
  {\
    _drbIdx = _ueCb->rbInfo.drbAddLstIndx[_idx];\
    if(_rbId == _ueCb->rbInfo.drb[_drbIdx].rbId)\
    {\
      _ueCb->rbInfo.drb[_drbIdx].isUdhRabEnbl = TRUE;\
      break;\
    }\
  }\
}

#define UEH_UECB_RB_DEALLOCATE_TNL(_ueCb, _teId)\
{\
  U8 _idx;\
  U8 _drbIdx;\
  for(_idx = 0; _idx < _ueCb->rbInfo.numOfDrbAdd; _idx++)\
  {\
    _drbIdx = _ueCb->rbInfo.drbAddLstIndx[_idx];\
    if(_teId == _ueCb->rbInfo.drb[_drbIdx].locEgtpTunId)\
    {\
      freeTeid(_ueCb->rbInfo.drb[_drbIdx].locEgtpTunId);\
      _ueCb->rbInfo.drb[_drbIdx].isEgtpTnlEnbl = FALSE;\
      break;\
    }\
  }\
}
#define UEH_SRB_RGRMSK(_ueCb, _lchId)   (_ueCb)->rbInfo.srb[(_lchId) - 1].rgrCfgMask

#define UEH_DRB_RGRMSK(_ueCb, _lchId)   (_ueCb)->rbInfo.drb[(_lchId) - 3].rgrCfgMask

#define UEH_GET_RGR_SUBSTATE(_ueCb, _lchId, _rgrSubState)\
{\
  if (UEH_VAL_ZERO != _lchId)\
  {\
    if (UEH_PDCP_ID_SRB1 == (_lchId) || UEH_PDCP_ID_SRB2 == (_lchId))\
    {\
      (_rgrSubState) = (_ueCb)->rbInfo.srb[(_lchId) - 1].rgrSubState;\
    }\
    else\
    {\
      (_rgrSubState) = (_ueCb)->rbInfo.drb[(_lchId) - 3].rgrSubState;\
    }\
  }\
}

#define UEH_SET_RGR_SUBSTATE(_ueCb, _lchId, _rgrSubState)\
{\
  if (UEH_PDCP_ID_SRB1 == (_lchId) || UEH_PDCP_ID_SRB2 == (_lchId))\
  {\
    (_ueCb)->rbInfo.srb[(_lchId) - 1].rgrSubState = _rgrSubState;\
  }\
  else\
  {\
    (_ueCb)->rbInfo.drb[(_lchId) - 3].rgrSubState = _rgrSubState;\
  }\
}

#define UEH_FILL_BIT_STR(_ptr, _len, _val, _pdu) {\
   U8      i;                                    \
   U16     _tmpLen;                             \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   \
   _tmpLen = _len;                              \
   if((_len%8) > 0)                              \
   {                                             \
      _tmpLen = (_len/8)+1;                         \
   }                                             \
   else                                          \
   {                                             \
      _tmpLen = _len/8;                             \
   }                                             \
   UEH_GET_MEM(_pdu, _tmpLen * sizeof(U8), (Ptr *)&(_ptr.val));  \
                      \
   if(_ptr.val != NULLP)                         \
   {                                             \
      for(i = 0; i < _tmpLen; i++)                  \
      {                                          \
         _ptr.val[i] = _val;                  \
      }                                          \
   }                                             \
}

#define UEH_FILL_BIT_STR_VAL(_ptr, _len, _val, _pdu) {\
   U8      i;                                    \
   U16     _tmpLen;                             \
   U32    _val2= _val; \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   _tmpLen = _len;                              \
   if((_len%8) > 0)                              \
   {                                             \
      _tmpLen = (_len/8)+1;                         \
   }                                             \
   else                                          \
   {                                             \
      _tmpLen = _len/8;                             \
   }                                             \
   UEH_GET_MEM(_pdu, _tmpLen * sizeof(U8), (Ptr *)&(_ptr.val));  \
                      \
   if(_ptr.val != NULLP)                         \
   {                                             \
      for(i = 0; i < _tmpLen; i++)                  \
      {                                          \
         _ptr.val[i] = (U8)(0x000000ff & _val2);                  \
         _val2 = _val2>>8;\
      }                                          \
   }                                             \
}

#define UEH_BIT_TO_BYTE_CONVSN(_byteCnt, _bitCnt) \
  _byteCnt = ((0 == (_bitCnt % 8)) ? (_bitCnt / 8) : ((_bitCnt / 8) + 1));\

#define GET_PAGING_CYCLE(enmValue, T) {\
   if (enmValue == 0) T = 32;\
   else if (enmValue == 1) T = 64;\
   else if (enmValue == 2) T = 128;\
   else T = 256;\
}

#define GET_NB_VALUE(enmValue, T, value) {\
   if (enmValue == 0) value = T * 4;\
   else if (enmValue == 1) value = T * 2;\
   else if (enmValue == 2) value = T * 1;\
   else if (enmValue == 3) value = (T / 2);\
   else if (enmValue == 4) value = (T / 4);\
   else if (enmValue == 5) value = (T / 8);\
   else if (enmValue == 6) value = (T / 16);\
}

#define GET_RGRCFGCFM_MASK(_ueCb, _counter) {\
  U8 _idx = 0;\
  U8 _drbIdx = 0;\
  if(UEH_RB_RGRMSK_DONE != _ueCb->rbInfo.srb[1].rgrCfgMask)\
  {\
    RETVALUE(ROK);\
  }\
  for(_idx = 0; _idx < _ueCb->rbInfo.numOfDrbAdd; _idx++)\
  {\
    _drbIdx = _ueCb->rbInfo.drbAddLstIndx[_idx];\
    if(UEH_RB_RGRMSK_DONE == _ueCb->rbInfo.drb[_drbIdx].rgrCfgMask)\
    {\
      _counter++;\
    }\
    else\
    {\
      RETVALUE(ROK);\
    }\
  }\
}
#define GET_CRNTI_FROM_UEID(_ueId) (_ueId->c_RNTI.val[1] | (_ueId->c_RNTI.val[0] << 8))

#define SET_REL_CAUSE_IN_UECB(_ueCb, _causeType, _causeVal) { \
   _ueCb->s1ConCb.s1CauseType = _causeType;              \
   _ueCb->s1ConCb.s1Cause     = _causeVal;               \
}

#define STOP_UE_TMR_IF_RUNNING(_ueCb,_tmr,_event,_ueId) \
   if ((*_tmr).isTmrRunning) uehStopTmr(_event,_tmr,_ueCb,_ueId)

#define UEH_GET_S1CONCB(_s1ConCb,_s1ConId) { \
 if (uehCb.s1UeConLst[_s1ConId]) \
 {  \
    _s1ConCb = &uehCb.s1UeConLst[_s1ConId]->s1ConCb; \
 } \
 else \
 {  \
  _s1ConCb = NULLP; \
 } \
}


#define uehGetState(ueCb)  (ueCb)->ueState

#define uehGetCellCb() (&uehCb.cellCb[0])

#define UECM_OUTPUT(_id,_name,_to,_details) \
  logMsgOut("%d|%s|%s|%s",_id, #_name, #_to, _details)

#define UECM_INPUT(_id,_name,_from,_details) \
  logMsgIn("%d|%s|%s|%s", _id, #_name, #_from, _details)

#define UECM_EVENTHDLR(_id,_name,_details) \
  logInfo("EVHDLR|%d|%s|%s", _id ,#_name, _details)

#define UECM_FUNC(_id,_name,_from,_to,_details) \
  logFunc("%d|%s|%s|%s|%s", _id, #_name,#_from,#_to, _details)

#define UECM_TIMERSTART(_ueId, _name, _tmrEvnt,_delay) \
  logTmr("ueId=%ld|%s|STARTED|TimerId = %d|duration = %lu", _ueId, _name,_tmrEvnt,_delay)

#define UECM_TIMERSTOP(_ueId,_name,_tmrEvnt,_delay) \
  logTmr("ueId=%ld|%s|STOPPED|TimerId = %d|duration = %lu", _ueId, _name,_tmrEvnt,_delay)

#define UECM_TIMEREXP(_not_used,_name,_event) \
  logError("%s|EXPIRED|event=%d", #_name, _event)


#define UEH_CONVERT_UL_MHZ_TO_RB(bw)  bw2RbMapping[bw].nhurgrRbs
#define UEH_CONVERT_DL_MHZ_TO_RB(bw)  bw2RbMapping[bw].nhurgrRbs
#define GET_NUM_RB_FRM_CH_BW(bw) bw2RbMapping[bw].ctfRbs
#define UEH_CONVERT_UL_MHZ_TO_RB_ENUM(bw)  bw2RbMapping[bw].rrcSpecRbs

#ifdef RRM_PRL_FLAG
#define GET_UEHCB_PRNT_LIB_BUFF() &(uehCb.buff)
#endif

#define getCellAvState(cb)       ( ( ((cb)->isS1LinkUp) && ((cb)->isUnlocked) ) ? UEH_CELL_STATE_AVAILABLE : UEH_CELL_STATE_UNAVAILABLE)


PUBLIC S16 uehStoreRcvdCellCfg(Buffer   *mBuf);

EXTERN S16 smUehReadConfigParams ARGS((Void));
PUBLIC S16 sendEventToOam (
  Ent      srcEnt,
  Ent      dstEnt,
  Event    event,
  MsgLen      len,
  U8      *data
  );

U8 oam2nhuRlcMode(RlcMode oam);

PUBLIC S16 sendSmMsgToBsm(
    Event   event,
    MsgLen     len,
    U8     *data
    );
PUBLIC S16 sendSmMsgToLth(
    Event   event,
    MsgLen     len,
    U8     *data
    );
PUBLIC S16 sendSmMsgToPmh(
    Event   event,
    MsgLen     len,
    U8     *data
    );
/* Prototypes of Init functions */
EXTERN S16 smUehInit ARGS((SSTskId sysTskId));
EXTERN S16 smSzInit ARGS((SSTskId sysTskId));
EXTERN S16 smEgInit ARGS((SSTskId sysTskId));
EXTERN S16 smEuInit ARGS((SSTskId sysTskId));
EXTERN S16 smSbInit ARGS((SSTskId sysTskId));
EXTERN S16 smHiInit ARGS((SSTskId sysTskId));
EXTERN S16 smNhInit ARGS ((SSTskId sysTskId));
EXTERN S16 smPjInit ARGS ((SSTskId sysTskId));
EXTERN S16 smRgInit ARGS ((SSTskId sysTskId));
EXTERN S16 smKwInit ARGS ((SSTskId sysTskId));
EXTERN S16 smYsInit ARGS ((SSTskId sysTskId));




EXTERN S16 regAndAttachTsk(
  const char* entName,                    /* entity name */
  Ent ent,                        /* entity */
  Inst inst,                      /* instance */
  Ttype type,                     /* task type */
  Prior prior,                    /* task priority */
  PAIFS16 initTsk,                /* initialization function */
  ActvTsk actvTsk,                /* activation function */
  SSTskId sTskId                 /* System Task Id to which the TAPA Task it to be attached */
  );
TBOOL isFailureEvent(EventType evt);                                              
EXTERN S16 cpMain(S16 selfProcId);
EXTERN S16 gtpMain(S16 selfProcId);
EXTERN S16 radioupMain(S16 selfProcId);
EXTERN S16 psMain(Void);
void openCpLog(void);
TBOOL readEnbPreConfig(void);
void openlogConfig(config_t *cfg);




/* Prototypes of state machine functions */
EXTERN Void smUehProcSm          ARGS((U8 event));
EXTERN S16  uehSmSelfPstMsg      ARGS((Pst *pst));
EXTERN Void uehSmHdlTuclCfgEvent ARGS((U8 event));
EXTERN Void uehSmHdlSctpCfgEvent ARGS((U8 event));
EXTERN Void uehSmHdlS1apCfgEvent ARGS((U8 event));
EXTERN Void uehSmHdlEgtpCfgEvent ARGS((U8 event));
EXTERN Void uehSmHdlAppCfgEvent  ARGS((U8 event));
EXTERN Void uehSmHdlRrcCfgEvent  ARGS((U8 event));
EXTERN Void uehSmHdlUdhCfgEvent  ARGS((U8 event));
EXTERN Void uehSmHdlPdcpCfgEvent ARGS((U8 event));
EXTERN Void uehSmHdlRlcCfgEvent  ARGS((U8 event));
EXTERN Void uehSmHdlMacCfgEvent  ARGS((U8 event));
EXTERN Void uehSmHdlCLCfgEvent   ARGS((U8 event));
EXTERN Void uehSmHdlBndS1apStack ARGS((U8 event));
EXTERN Void uehSmHdlBndEgtpStack ARGS((U8 event));
EXTERN Void uehSmHdlBndRrcStack  ARGS((U8 event));
EXTERN Void uehSmHdlEnbAlarms    ARGS((U8 event));
EXTERN Void uehSmHdlEnbDebugs    ARGS((void));
EXTERN Void uehSmHdlInitS1Setup  ARGS((U8 event));
EXTERN Void uehSmHdlInitCellCfg  ARGS((U8 event));
EXTERN Void uehSmHdlUdhCfgEvent ARGS((U8 event));
EXTERN Void smHdlBsmCellConfig  ARGS((U8 event, Buffer *mBuf));

/* Actv Task  and Actv Init*/
EXTERN S16 nhActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 nhActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 kwActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 kwActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 rgActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 rgActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 schActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 schActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 egActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 egActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 pjActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 pjActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 ysActvTsk ARGS((Pst *pst, Buffer *mBuf));
EXTERN S16 ysActvInit ARGS((Ent entity, Inst inst, Region
       region, Reason reason));

EXTERN S16 smEgActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smRgActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smNhActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smKwActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smPjActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smHiActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smSbActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smSzActvTsk ARGS((Pst *, Buffer *));

/* Config functions */
EXTERN Void uehSmHiCfg ARGS((Void));
EXTERN Void uehSmSbCfg ARGS((Void));
EXTERN Void uehSmSzCfg ARGS((Void));
EXTERN Void uehSmEgCfg ARGS((Void));
EXTERN Void uehSmUehCfg ARGS((Void));
EXTERN Void uehSmNhCfg ARGS((Void));
EXTERN Void uehSmPjCfg ARGS((Void));
EXTERN Void uehSmKwCfg ARGS((Void));
EXTERN Void uehSmRgCfg ARGS((Void));
EXTERN Void uehSmYsCfg ARGS((Void));


/* HI(TUCL) Build Functions */
EXTERN Void smBuildHiGenCfg     ARGS((Void));
EXTERN Void smBuildHiTSapCfg    ARGS((SpId spId));
EXTERN Void smBuildHiDbgCntrl   ARGS((LogConfig *));

/* SB(SCTP) Build Functions */
EXTERN Void smBuildSbGenCfg     ARGS((Void));
EXTERN Void smBuildSbTSapCfg    ARGS((SpId spId, SuId suId));
EXTERN Void smBuildSbSctSapCfg  ARGS((SpId spId));
EXTERN Void smBindSbToLSap      ARGS((SpId spId));
EXTERN Void smBuildSbDbgCntrl   ARGS((LogConfig *));

/* SZ(S1AP) Build Functions */
EXTERN S16 smBuildSzGenCfg     ARGS((Void));
EXTERN S16 smBuildSzLSapCfg    ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildSzUSapCfg    ARGS((SpId spId));
EXTERN S16 smBuildSzProtCfg    ARGS((Void));
EXTERN S16 smBuildSzPeerCfg    ARGS((Void));
EXTERN S16 smBindSzToLSap      ARGS((Void));
EXTERN S16 smBuildSzAlarmCntrl   ARGS((Void));
EXTERN S16 smBuildSzDbgCntrl   ARGS((LogConfig *));

/* EG(EGTP) Build Functions */
EXTERN S16  smBuildEgGenCfg     ARGS((Void));
EXTERN S16  smBuildEgUSapCfg    ARGS((SpId spId));
EXTERN S16  smBuildEgLSapCfg    ARGS((SpId spId, SuId suId));
/* Shwetha - adding IP Address configuration for EGTP-U */
EXTERN S16  smBuildEgIpAddrCfg  ARGS((SpId  spId));
EXTERN S16  smBuildTServerCfg   ARGS((Void));
EXTERN Void smBindEgToHitSap    ARGS((SpId spId));
EXTERN Void  smBuildEgDbgCntrl   ARGS((LogConfig *));

/* YS(CL) Build Functions */
EXTERN S16  smBuildYsGenCfg     ARGS((Void));
EXTERN S16  smBuildYsTfuSapCfg  ARGS((SpId spId, SuId suId));
EXTERN S16  smBuildYsSchTfuSapCfg  ARGS((SpId spId, SuId suId));
EXTERN S16  smBuildYsCtfSapCfg  ARGS((SpId spId, SuId suId));
EXTERN S16  smBuildYsGenCntrl   ARGS((Void));

/* UEH(APP) Build Functions */
EXTERN S16 smBuildUehGenCfg     ARGS((Void));
EXTERN S16 smBuildUehCtfLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildUehNhuLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildUehRgrLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildUehSztLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildUehUdhLSapCfg ARGS((SuId suId, SpId spId));

EXTERN S16 smBindUehToSztSap    ARGS((Void));
EXTERN S16 smBindUehToNhuSap    ARGS((Void));
EXTERN S16 smBindUehToCtfSap    ARGS((Void));
EXTERN S16 smBindUehToRgrSap    ARGS((Void));
EXTERN S16 smBuildUehAlarmCntrl   ARGS((Void));
EXTERN S16 smBuildUehS1InitCntrl   ARGS((Void));
EXTERN S16 smBuildUehCellCntrl   ARGS((Void));
EXTERN S16 smBuildUehDbgCntrl   ARGS((LogConfig *));

/* NH(LTE RRC) Build Functions */
EXTERN S16 smBuildNhGenCfg     ARGS((Void));
EXTERN S16 smBuildNhProtCfg    ARGS((Void));
EXTERN S16 smBuildNhUsapCfg    ARGS((Void));
EXTERN S16 smBuildNhKwuLsapCfg ARGS((Void));
EXTERN S16 smBuildNhCkwLsapCfg ARGS((Void));
EXTERN S16 smBuildNhCrgLsapCfg ARGS((Void));
EXTERN S16 smBuildNhCpjLsapCfg ARGS((Void));
EXTERN S16 smBuildNhPjuLsapCfg ARGS((Void));
EXTERN S16 smBuildNhLsapCfg    ARGS((U8 elemId));
EXTERN S16 smBindNhToKwuSap    ARGS((Void));
EXTERN S16 smBindNhToCkwSap    ARGS((Void));
EXTERN S16 smBindNhToCrgSap    ARGS((Void));
EXTERN S16 smBindNhToCpjSap    ARGS((Void));
EXTERN S16 smBindNhToPjuSap    ARGS((Void));
EXTERN S16 smBindNhToLsap      ARGS((U8 elemId));
EXTERN S16 smBuildNhAlarmCntrl   ARGS((Void));
EXTERN S16 smBuildNhDbgCntrl   ARGS((LogConfig *));

/* PJ(LTE PDCP) Build Functions */
EXTERN S16 smBuildPjGenCfg     ARGS((Void));
EXTERN S16 smBuildPjCpjSapCfg  ARGS((Void));
EXTERN S16 smBuildPjPjuSapCfg  ARGS((SpId spId));

/* KW(LTE RLC) Build Functions */
EXTERN S16 smBuildKwGenCfg     ARGS((Void));
EXTERN S16 smBuildKwUsapCfg    ARGS((U8 elemId, U8 ent));
EXTERN S16 smBuildCkwUsapCfg   ARGS((Void));
EXTERN S16 smBuildKwuUsapCfg   ARGS((U8 ent));
EXTERN S16 smBuildKwRguLsapCfg ARGS((Void));
EXTERN S16 smBindKwToRguSap    ARGS((Void));
EXTERN S16 smBuildKwDbgCntrl   ARGS((LogConfig *));
EXTERN S16 smBuildYsDbgCntrl   ARGS((LogConfig *));

/* RG(LTE MAC) Build Functions */
EXTERN S16 smBuildRgGenCfg     ARGS((Void));
EXTERN S16 smBuildRgUsapCfg    ARGS((U8 elemId));
EXTERN S16 smBuildCrgUsapCfg   ARGS((Void));
EXTERN S16 smBuildRguUsapCfg   ARGS((Void));
EXTERN S16 smBuildRgSchInsCfg   ARGS((Void));
EXTERN S16 smBuildRgTfuLsapCfg ARGS((Void));
EXTERN S16 smBindRgToTfuSap    ARGS((Void));
EXTERN S16 smBindRgSchToTfuSap ARGS((Void));
EXTERN S16 smBuildRgGenCntrl   ARGS((Void));
EXTERN S16 smBuildRgDbgCntrl   ARGS((LogConfig *));
/*Pwav interface*/
EXTERN S16 sendMsgToFrm ARGS((MsgId msgId, ModuleId srcModId));
EXTERN void uehSendToBsm ARGS((MsgId msgId, U16 len, U8 *data));
PUBLIC S16 sendCntrlCfmToBsm (MsgId msgId, ModuleId srcModId);
PUBLIC void uehRecvNhTrcCntrlCfm( NhMngmt *cfm );
PUBLIC void uehRecvSzTrcCntrlCfm( SzMngmt *cfm );
PUBLIC void uehRecvRgTrcCntrlCfm( RgMngmt *cfm );
PUBLIC void uehRecvMacTrcInd( Buffer *dstMbuf );
PUBLIC void uehRecvS1apTrcInd( SzMngmt *trc );
PUBLIC void updateCntForCntrlCfm(Elmnt elmnt);






PUBLIC S16 udhActvInit
(
  Ent    entity,    /* Entity   */
  Inst   inst,      /* Instance */
  Region region,    /* Region   */
  Reason reason     /* reason   */
);

PUBLIC S16 smUdhInit (
  SSTskId    sysTskId
);


PUBLIC S16 smUdhSapsCfgReq
(
  Pst       *pst,
  CmSapCfg  *cmSapCfg
);
PUBLIC S16 smUdhSapsBindReq
(
  Pst           *pst,
  CmSapBindReq  *udhGtpSapBindReq
);

PUBLIC Void uehSmUdhCfg(Void);
PUBLIC S16 smUdhSapsCfgReq(Pst *post,CmSapCfg  *cmSapCfg);
PUBLIC S16 smBuildUdhGtpSapCfg(Void);
PUBLIC S16 smBuildUdhPdcpSapCfg(Void);
PUBLIC S16 smBuildUdhUehSapCfg(Void);
PUBLIC S16 smUdhGtpSapBindReq(Void);
PUBLIC S16 smUdhPdcpSapBindReq(Void);



PUBLIC U8 ueFsm(
  UehUeCb     *ueCb,
  EventType    ueEvt,
  void        *pdu
  );

void uehRecordUeEvnt( const UehUeCb *ueCb, EventType ueEvt , void *sdu,U8 evtVector);
void fillConnReqDetail(U8 ueEvt,U16 ueIdx,U16 crnti);
//void uehSetState( UehUeCb *ueCb, U8 newState );
#define uehSetState( ueCb, newState ) \
{\
  logState("crnti=%d|%d -> %d (%s)", ueCb->crnti, ueCb->ueState, newState, #newState);\
  ueCb->ueState = newState;\
}
const char *uehGetEventName(UINT8 event);
// const S8 *uehGetEventName(U8 event);
void  *allocUeHistoryElement(Void);
void  uehFillEventDetail(EventType ueEvt,UeEventHistory *ueEventHistory,void *sdu,const UehUeCb *ueCb);
TBOOL isEventToBeAddedToHistory(EventType ueEvt,const UehUeCb *ueCb);
TBOOL isUeHistoryTableFull(UehUeHisTbl *ueHisTbl);

EXTERN S16 UehHdlEgtpTunelCfm ARGS(
(
Pst         *pst,
SuId        suId,
CmLteEgtpTunCfm *egtpTunCfm
));

/*Timer related Functio*/

EXTERN S16 uehRegInitTmr ARGS((Void));
EXTERN S16 uehRegInitDatTmr ARGS((Void));
EXTERN Void uehStartTmr(UehTimerId timerId,PcmTimer *tmr,PTR cb,U32 userId);
EXTERN Void uehStopTmr(UehTimerId timerId,PcmTimer *tmr,PTR cb,U32 userId);
//EXTERN Void uehStopTmr ARGS(( PTR cb, S16 tmrEvnt));
EXTERN S16 uehActvTmr ARGS (( void));
EXTERN U16 uehMsgTypeToIndex(U8 Type, U8 msgCat);

PUBLIC S16 uehProcDlNasInS1Con ARGS ((UehUeCb *ueCb, S1apPdu *pdu));
PUBLIC S16 uehProcDlNasMsg ARGS ((UehUeCb *ueCb, S1apPdu *pdu));
PUBLIC S16 uehProcInitCtxtSetup(UehUeCb* ueCb, InitialContextSetup *initialContextSetup);
PUBLIC S16 uehProcInitCtxtSetupInUeCntd(UehUeCb *ueCb, S1apPdu *pdu);
void uehSndInitCtxtSetupFail( UehS1ConCb *s1ConCb, UINT16 causeType, UINT32 causeVal);
void uehSndUeCtxtModFail( UehUeCb *ueCb, UINT16 causeType, UINT32 causeVal);
void uehStoreInitialCntxtInfo( UehUeCb *ueCb, InitialContextSetup *initialContextSetup);
PUBLIC S16 uehProcInitCtxtSetupInS1Con(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehProcUeCtxRelCmd ARGS ((UehUeCb *ueCb, S1apPdu *pdu));
PUBLIC S16 uehProcS1ErrInd(UehUeCb *ueCb,  S1apPdu  *pdu);
PUBLIC S16 uehProcLocReport(UehUeCb *ueCb, S1apPdu *pdu);
void uehProcUeHistoryCfgReq(Buffer *mbuf);
void uehHandleTraceStart( UehUeCb *ueCb, TraceActivation *traceActivation);
PUBLIC S16 uehProcTraceStart(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehProcDeactivateTrace(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehProcUdhUeInactvInd(UehUeCb *ueCb, void *sdu);
PUBLIC S16 uehProcUdhUlInactivityInd(UehUeCb  *ueCb, void     *sdu);
PUBLIC S16 uehProcUeRspTmrExpiryForRlf(UehUeCb *ueCb);
EXTERN S16 UehLiUdhUlInactvInd ARGS((Pst *pst, UdhUehUlInactiveInd *udhUehUlInactiveInd));
PUBLIC S16 uehValidateEnbAndMmeUeS1apIds(U32 mmeUeS1apId,U32 enbUeS1apId,U16 curCrnti);
EXTERN S16 uehProcRrcCountCfm ARGS((UehUeCb *ueCb, NhuDRBCountInfoLst *nhuDRBCountInfoLst));
EXTERN S16 uehProcRrcCounterChkRsp ARGS((UehUeCb *ueCb, void *sdu ));
PUBLIC S16 uehSndUdhDelUeReq( UehUeCb *ueCb);
PUBLIC S16 uehSndUdhDelRabReq( UehUeCb *ueCb);
PUBLIC S16 uehProcUdhAddRabCfm(UehUeCb *ueCb, UdhUehAddRabCfm *udhUehAddRabCfm);
PUBLIC S16 uehProcERabSetupReq(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehProcERabModifyReq(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehProcERabReleaseReq(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehProcUeCtxModReq(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehUtlSztUeCntxtRel ARGS ((UehS1ConCb *s1Con, S1apPdu  **pdu));
EXTERN S16 uehProcMmeRelIndTmrExp (UehUeCb *ueCb);
EXTERN S16 uehProcUdhRabDelCfm(UehUeCb *ueCb,UdhUehDelRabCfm  *delRabCfm);
EXTERN S16 uehProcUdhUeDelCfm( UehUeCb *ueCb, UdhUehDelUeCfm  *delUeCfm);
void fillUeEventStatus(MsgUehBsmActiveUeStatusRsp *actUeSts,UehUeCb *ueCb);
TBOOL uehProcTgtToSrcTprntContDecCfm( UehUeCb   *ueCb, void *sdu);
EXTERN S16 uehFillCounterCheckMsg( PTR *evnt, NhuCounterCheck *counterCheck, NhuDRBCountInfoLst *nhuDRBCountInfoLst);
EXTERN S16 uehProcInternalReconfTmrExpiry( UehUeCb *ueCb);
EXTERN S16 UehLiUdhBndReq ( Pst *pst, SuId suId, SpId spId);
EXTERN S16 UehLiUdhAddRabReq ( Pst *pst, SuId suId, UehUdhAddRabReq *addRabReq   /* UDH Add RAB structure */);
EXTERN S16 UehLiUdhDelRabReq ( Pst *pst, SuId suId,UehUdhDelRabReq  *delRabReq   /* UDH Del RAB structure */);
EXTERN S16 UehLiEgtEguLclTnlMgmtReq ( Pst *pst, SuId suId, EgtUEvnt *egtUEvnt);
EXTERN S16 UehLiRgrCfgReq ( Pst* pst, SpId spId, RgrCfgTransId transId, RgrCfgReqInfo * cfgReqInfo);
EXTERN RgrUeDsrTransMax rgrUeSrCfgMapping(EnumNhuSchedulingRqstConfigsetupdsr_TransMax dsrTransMax);
EXTERN S16 UehLiUdhDelUeReq ( Pst *pst,SuId suId,UehUdhDelUeReq    *delUeReq);
EXTERN S16 sendUehMsgToPmh (Event event, MsgLen len, U8 *data);
//S1apMgrCb *getS1MgrCb( UINT32 peerId);
/* Paging */
PUBLIC S16 uehLiSztPagingHandler ARGS (( S1apPdu  *pdu));
PUBLIC S16 uehFillAndAddPagingInfo ARGS (( S1apPdu *s1apPdu));
UINT16 uehConvertTimeAlignmentTimerEnumToVal(TimeAlignmentTimer  timeAlignmentTimerEnum);
UINT16 uehConvertPeriodicBsrEnumToVal(PeriodicBsrTimer  periodicBsrTimerEnum);
UINT16 uehConvertRetxBsrTimerEnumToVal(RetxBsrTimer  retxBsrTimerEnum);
PUBLIC S16 UehLiEgtEguLclTnlMgmtReq (Pst *pst, SuId suId, EgtUEvnt *egtUEvnt);
EXTERN S16 UehLiNhuBndReq ARGS((Pst *pst, SuId suId, SpId spId));
EXTERN S16 UehLiNhuUbndReq ARGS(( Pst *pst, SpId spId, Reason reason ));
EXTERN S16 UehLiNhuDatReq  ARGS(( Pst *pst, SpId spId, NhuDatReqSdus *nhuDatReqSdus ));
EXTERN S16 UehLiNhuDatRsp  ARGS(( Pst *pst, SpId spId, NhuDatRspSdus *nhuDatRspSdus ));
EXTERN S16 UehLiNhuCfgReq  ARGS(( Pst *pst, SpId spId, NhuCfgReqSdus *nhuCfgReqSdus ));
EXTERN S16 UehLiNhuCnclUeReq  ARGS(( Pst *pst, SpId spId, NhuCellId cellId,NhuCrnti crnti ));
void uehClearCallInRrcConnectingState(UehUeCb *ueCb);
EXTERN S16 UehLiNhuCellCfgReq  ARGS(( Pst *pst, SpId spId, NhuCellCfgReqSdus *nhuCellCfgReqSdus ));
EXTERN S16 UehLiNhuDeleteTransReq  ARGS(( Pst *pst, SpId spId, NhuCellId cellId, NhuCrnti ueId, U32 transId )); 
EXTERN S16 UehLiNhuDecodeReq ARGS(( Pst     *pst, SuId     suId, NhuDecodeSduReq *nhuDecodeSduReq));
EXTERN S16 UehLiNhuCountReq ARGS(( Pst     *pst, SuId     suId, NhuHdr *countHdr));
EXTERN S16 UehLiNhuEncodeReq ARGS(( Pst     *pst, SuId     suId, NhuEncReqSdus *nhuEncReqSdus));
EXTERN S16 UehLiNhuPdcpSduStaReq ARGS((Pst     *pst, SuId    suId, NhuHdr  nhuHdr));

PUBLIC S16 UehLiRgrBndReq ARGS((Pst *pst, SuId suId, SpId spId));

#ifdef RGR_RRM_TICK
EXTERN S16 uehSibHdlTmrInd ARGS((RgrTtiIndInfo *ttiInd ));
PUBLIC S16 UehLiRgrTtiInd ARGS((Pst *pst, SuId suId, RgrTtiIndInfo *ttiInd));
#endif
PUBLIC S16 UehLiCtfBndReq ARGS((Pst *pst, SuId suId, SpId spId));
PUBLIC S16 UehLiSztBndReq ARGS((Pst *pst, SuId suId, SpId spId));
PUBLIC S16 UehLiSztUbndReq ARGS((Pst *pst, SpId spId,Reason reason));

PUBLIC S16 UehLiSztUDatReq ARGS((Pst *pst,SpId spId,SztUDatEvnt *uDatEvnt));
PUBLIC S16 UehLiSztRelRsp ARGS((Pst *pst,SpId spId,SztRelRsp *relRsp));

EXTERN S16 UehLiSztBndReq       ARGS((Pst *pst, SuId suId, SpId spId));
EXTERN S16 UehLiSztUbndReq      ARGS((Pst *pst, SpId spId, Reason reason));
EXTERN S16 UehLiSztBndCfm       ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 UehLiSztStaInd       ARGS((Pst *pst, SuId suId, SztStaInd *sztSta));
EXTERN S16 UehLiSztFlcInd       ARGS((Pst *pst, SuId suId, SztFlcInd *flcInd));
EXTERN S16 UehLiSztErrInd       ARGS((Pst *pst, SuId suId, SztLclErrInd *sztErrInd));
EXTERN S16 UehLiSztUDatReq      ARGS((Pst *pst, SpId spId, SztUDatEvnt *uDatEvnt));
EXTERN S16 UehLiSztUDatInd      ARGS((Pst *pst, SuId suId, SztUDatEvnt *uDatEvnt));
EXTERN S16 UehLiSztConReq       ARGS((Pst *pst, SpId spId, SztConReq *conReq));
EXTERN S16 UehLiSztConInd       ARGS((Pst *pst, SuId suId, SztConInd *conInd));
EXTERN S16 UehLiSztConRsp       ARGS((Pst *pst, SpId spId, SztConRsp *conRsp));
EXTERN S16 UehLiSztConCfm       ARGS((Pst *pst, SuId suId, SztConCfm *conCfm));
EXTERN S16 UehLiSztRelReq       ARGS((Pst *pst, SpId spId, SztRelReq *relReq));
EXTERN S16 UehLiSztRelInd       ARGS((Pst *pst, SuId suId, SztRelInd *relInd));
EXTERN S16 UehLiSztRelRsp       ARGS((Pst *pst, SpId spId, SztRelRsp *relRsp));
EXTERN S16 UehLiSztRelCfm       ARGS((Pst *pst, SuId suId, SztRelCfm *relCfm));
EXTERN S16 UehLiSztDatReq       ARGS((Pst *pst, SpId spId, SztDatEvntReq *datEvnt));
PUBLIC S16 UehLiSztErrInd( Pst            *pst, SuId           suId, SztLclErrInd   *sztLclErrInd);
EXTERN S16 UehLiSztDatInd       ARGS((Pst *pst, SuId suId, SztDatEvntInd *datEvnt));
EXTERN S16 UehLiSztEncDecReq    ARGS((Pst *pst, SpId spId, SztEncDecEvnt *sztEncDecEvnt));
EXTERN S16 UehLiSztEncDecCfm    ARGS((Pst *pst, SuId suId, SztEncDecEvnt *sztEncDecEvnt,SztReqStatus *status));
EXTERN S16 UehLiSztAudCfm       ARGS((Pst *pst, SuId suId, SztAudEvnt *audEvnt));
EXTERN S16 TfLiCtfBndCfm ARGS(( Pst       *pst,         SuId      suId,        U8        status      ));
EXTERN S16 TfLiCtfCfgCfm ARGS(( Pst *pst,             SuId suId,           CtfCfgTransId transId, U8 status            ));
EXTERN S16 TfLiCtfUeIdChgCfm ARGS(( Pst       *pst,            SuId       suId, CtfCfgTransId transId, CtfUeInfo *ueInfo, U8 status));
EXTERN S16 UehLiCtfBndReq ARGS(( Pst* pst, SuId suId, SpId spId)); 
EXTERN S16 UehLiCtfUbndReq ARGS(( Pst* pst, SpId spId, Reason reason));
EXTERN S16 UehLiCtfCfgReq ARGS(( Pst *pst,                   SpId spId,                  CtfCfgTransId transId,      CtfCfgReqInfo *cfgReqInfo   )); 
EXTERN S16 UehLiCtfUeIdChgReq ARGS(( Pst* pst, SpId spId, CtfCfgTransId transId, CtfUeInfo * ueInfo, CtfUeInfo * newUeInfo));
/**/
/* ueh_cell.c */
EXTERN S16 uehFillSysInfMsg ARGS((NhuDatReqSdus *nhuDatReqEvntSdu, U8 msgCategory, U8 msgType));
EXTERN S16 uehEncSibPdu ARGS((NhuDatReqSdus *nhuDatReqEvntSdu, Buffer **encBuf));
EXTERN S16 uehFillCellRbCfg ARGS((NhuRbCfgList  **rbCfgList));
EXTERN S16 uehSndNhuCellCfgReq ARGS((U16 cellId));
EXTERN S16 uehSndCtfCellCfgReq ARGS((U16 cellId));
EXTERN S16 uehHdlRgrCellCfgReq ARGS((U16 cellId));
EXTERN S16 uehHdlRgrCellReCfgReq ARGS((U16 cellId));
EXTERN S16 uehSndRgrSiMsg ARGS((U8 siIndex,U8 siIdx,U16 cellId));
EXTERN S16 uehSndRgrSiCfgReq(U16  cellId);
EXTERN S16 uehSndRgrSiMibCfgReq(U16 cellId);
EXTERN S16 uehSndRgrSiSib1CfgReq(U16 cellId);
EXTERN S16 uehSndRgrSiSib2CfgReq(U8 siIdx,U16 cellId);
EXTERN S16 uehSndRgrSiSib3CfgReq(U8 siIdx,U16 cellId);
EXTERN S16 uehSndRgrSiSib4CfgReq(U8 siIdx,U16 cellId);
EXTERN S16 uehSndRgrSiSib5CfgReq(U8 siIdx,U16 cellId);
EXTERN S16 uehFillRgrDlHqCfg ARGS((RgrDlHqCfg *cfg));
EXTERN S16 uehFillRgrRntiCfg ARGS((RgrRntiCfg *cfg));
EXTERN S16 uehFillRgrCfiCfg ARGS((RgrCfiCfg *cfg));
EXTERN S16 uehFillRgrUlTrgCqiCfg ARGS((RgrUlTrgCqiCfg *cfg));
EXTERN S16 uehFillRgrDlCmnCodeRateCfg ARGS((RgrDlCmnCodeRateCfg *cfg));
EXTERN S16 uehFillRgrPuschSubBandCfg ARGS((RgrPuschSubBandCfg *cfg));
EXTERN S16 uehFillRgrUlCmnCodeRateCfg ARGS((RgrUlCmnCodeRateCfg *cfg));
EXTERN S16 uehFillRgrDlfsCfg ARGS((RgrDlfsCfg *cfg));
EXTERN S16 uehFillRgrBwCfg ARGS((RgrBwCfg *cfg));
EXTERN S16 uehFillRgrPhichCfg ARGS((RgrPhichCfg *cfg));
EXTERN S16 uehFillRgrPucchCfg ARGS((RgrPucchCfg *cfg));
EXTERN S16 uehFillRgrSrsCfg ARGS((RgrSrsCfg *cfg));
EXTERN S16 uehFillRgrSiCfg ARGS((RgrSiCfg *cfg));
EXTERN S16 uehFillRgrPuschCfg ARGS((RgrPuschCfg *cfg));
EXTERN S16 uehFillRgrPreambleSetCfg ARGS((RgrPreambleSetCfg *cfg));
//EXTERN void uehCfgGlobalVarInit (UehCfgTable *RrmDftCfg);
#ifdef LTE_TDD
EXTERN S16 uehFillRgrTddPrachRscInfo ARGS((RgrTddPrachRscInfo *cfg));
#endif 
EXTERN S16 uehFillRgrRachCfg ARGS((RgrRachCfg *cfg));
EXTERN S16 uehFillRgrCellDlPfs ARGS((RgrCellDlPfs *cfg));
EXTERN S16 uehFillRgrCellUlPfs ARGS((RgrCellUlPfs *cfg));
EXTERN S16 uehFillRgrCmnLcCfg ARGS((RgrCellCfg *cfg));
EXTERN S16 uehFillRgrUlPwrCfg ARGS((RgrUlPwrCfg *cfg));
/* ueh_sib.c */
#ifdef RGR_RRM_TICK
EXTERN S16 uehSibBldSIB5Msg ARGS((NhuDatReqSdus *nhDatReqEvntSdu));
EXTERN S16 uehSibHdlTmrInd ARGS((RgrTtiIndInfo *tmngInfo));
EXTERN S16 UehLiRgrTtiInd ARGS((Pst *pst, SuId suId, RgrTtiIndInfo *ttiInd));
EXTERN S16 uehHdlTmrInd ARGS((RgrTtiIndInfo *tmngInfo));
#endif

/* ueh_li_hdl */
EXTERN S16 uehHdlNhuDatInd ARGS((NhuDatIndSdus *datIndSdu));
EXTERN S16 freeUeCb(UehUeCb *ueCb);
void  uehClearUeCtxInNodeB(UehUeCb    *ueCb);
#ifdef SI_NEW
EXTERN S16 uehSibBldPdu ARGS((U8 msgCategory, U8 msgType, Buffer **encBuf));
EXTERN S16 uehLiRgrSiCfgReq ARGS((Pst* pst, SpId spId, RgrCfgTransId transId, RgrSiCfgReqInfo* cfgReqInfo));
#endif

void uehCanclUeSndRrcConRej( UehUeCb *Cb);
void uehStartRrcConnClear( UehUeCb    *ueCb);
PUBLIC S16 uehProcRrcDatCfmInUeCtxRel(UehUeCb *ueCb);
PUBLIC S16 uehProcRrcDatCfmInUeCapOngoing(UehUeCb *ueCb, NhuDatCfmSdus  *nhDatCfmSdus);
PUBLIC S16 uehProcRrcDatCfmInRecOngoing(UehUeCb *ueCb, NhuDatCfmSdus  *nhDatCfmSdus);
PUBLIC S16 uehProcCtxRelTmrExpiry(UehUeCb *ueCb);
PUBLIC S16 uehProcSrb1TmrExpiry(UehUeCb *ueCb);
PUBLIC S16 uehProcUeCrntiRetTmrExpiry(UehUeCb *ueCb);
PUBLIC S16 uehProcUeRspTmrExpiryAftrS1Conctd(UehUeCb *ueCb);
PUBLIC S16 uehProcUeRspTmrExpiryInRrcCon(UehUeCb *ueCb);
PUBLIC S16 uehProcRrcConRelL2AckTmrExpiry(UehUeCb *ueCb);
PUBLIC S16 uehProcS1LinkDownInS1Con(UehUeCb *ueCb);
PUBLIC S16 uehProcEgtpTunnelCfmInRelOngoing(UehUeCb *cb, EgtUEvnt *eguMsg);
PUBLIC S16 uehProcUdhRabDelCfm(UehUeCb *ueCb, UdhUehDelRabCfm  *delRabCfm);
PUBLIC S16 uehProcS1LinkDownInUeCntd(UehUeCb *ueCb);
TBOOL uehProcSrcToTgtTprntContEncCfm(UehUeCb *ueCb, SztEncDecEvnt *encDecCfm);
TBOOL uehTriggerToSendS1apHandoverRequired(UehUeCb *ueCb);
TBOOL uehProcHovrPrepInfoEncCfm(UehUeCb *ueCb, NhuEncCfmSdus *nhuEncCfmSdus);
PUBLIC S16 uehProcHandoverCmd(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehProcHandoverPrepFailed(UehUeCb *ueCb, S1apPdu *pdu);
PUBLIC S16 uehProcOpStopOrS1LinkDown(UehUeCb *ueCb);
PUBLIC S16 uehProcUeCapEnqInfo(UehUeCb *ueCb, NhuDatIndSdus *datIndSdu);
PUBLIC S16 uehProcUeCapEncCfm(UehUeCb *ueCb, NhuEncCfmSdus *nhuEncCfmSdus);
PUBLIC S16 uehSndS1UeCapInfoIndReq(UehUeCb *ueCb);
PUBLIC S16 uehProcUeCapDecCfm(UehUeCb *ueCb, NhuDecodeSduCfm *datIndSdu);
PUBLIC S16 uehSndUeCapEnqiry(UehUeCb *ueCb);
PUBLIC S16 uehSndUeRadioAccCapInfoDecReq(UehUeCb *ueCb);
PUBLIC S16 uehSndUeCapInfoDecodeReq( UehUeCb *Cb);
PUBLIC S16 uehSndUeEutraCapDecodeReq( UehUeCb *ueCb);
PUBLIC S16 uehSndEgtpDelInitCtxtSetupFail( UehUeCb       *ueCb, UINT16        causeType, UINT32        causeVal);
PUBLIC S16 uehSndNhuDecodeReq( NhuDecodeSduReq  *evt);
PUBLIC S16 uehProcUeRspTmrExpiryInRecOngoing(UehUeCb *ueCb);
PUBLIC S16 uehProcS1ClearInRrcCon(UehUeCb *ueCb);
PUBLIC S16 uehProcAdmissionReqSndEgtpCreateReq(UehUeCb *ueCb);
PUBLIC S16 uehSndUdhAddRabReq(UehUeCb *ueCb);
PUBLIC S16 uehSndUENhuDelCfgTransReq(NhuCellId cellId, NhuCrnti ueId, U32 transId);
EXTERN S16 uehSndCmnNhuDatReq ARGS((U8 msgCategory, U8 msgType, CmLteTimingInfo tmnInf));
PUBLIC S16 uehProcRrcConReEstblReq (UehUeCb *ueCb, NhuDatIndSdus *nhuSdu);
PUBLIC S16 uehSndUeCapInfoEncodeReq(UehUeCb *Cb);
PUBLIC S16 uehSndUeRadAccCapInfoEncodeReq(UehUeCb *ueCb);
EXTERN S16 uehSndUENhuDatReq ARGS((NhuDatReqSdus *evt));
EXTERN S16 uehHdlNhuDatRsp ARGS((NhuDatRspSdus *nhuDatRspEvntSdu));
EXTERN S16 uehSndRrcConRej ARGS((UehUeCb * ueCb ));
EXTERN S16 uehSndRrcConSetup ARGS((UehUeCb * ueCb));
EXTERN S16 uehSndRrcConReEstblRej ARGS((UehUeCb *ueCb,NhuHdr hdr,NhuCrnti tmpCrnti ));
EXTERN S16 uehProcRrcConReq ARGS((UehUeCb * ueCb, NhuDatIndSdus *datIndSdu));
EXTERN S16 uehSndRrcRecfgReq ARGS((UehUeCb *ueCb));
EXTERN S16 uehSndRrcDlInfoTfr ARGS((UehUeCb *ueCb, U16    msgLen, U8     *nasPdu));
EXTERN S16 uehSndRrcConRel ARGS((UehUeCb        *ueCb));
EXTERN S16 uehSndRrcSecModeCmd ARGS((UehUeCb  *ueCb));
EXTERN S16 uehFetchSecInfo ARGS((UehUeCb *ueCb, InitialContextSetup *initialContextSetup));
EXTERN S16 uehSndRrcConReEst ARGS(( NhuDatIndSdus *datIndSdu));
EXTERN TBOOL isUehUeProcFailed ARGS((UehUeCb  *ueCb, ProceId procId,TBOOL condition));
EXTERN S16 uehProcRrcConReCfgCfm ARGS(( UehUeCb *ueCb, NhuCfgCfmSdus *nhuCfgCfmSdus));
EXTERN S16 uehProcRrcConSetupCfgCfm ARGS((UehUeCb *ueCb, NhuCfgCfmSdus *nhuCfgCfmSdus));
EXTERN S16 uehProcSecCfgCfm ARGS((UehUeCb *cb, NhuCfgCfmSdus *sdu));
EXTERN S16 uehProcRrcConSetCmp ARGS((UehUeCb *ueCb, NhuDatIndSdus *datIndSdu));
EXTERN S16 uehProcUlNasTrans ARGS((UehUeCb *ueCb, NhuDatIndSdus *datIndSdu));
PUBLIC S16 uehProcEgtpTunnelCfmInRecOngoing(UehUeCb *ueCb, EgtUEvnt *eguMsg);
PUBLIC S16 uehSndS1InitCntxtSetupRsp( UehUeCb *ueCb);
PUBLIC S16 uehSndS1UeCntxtRlsReq(UehUeCb *ueCb, S1Cause *cause);
PUBLIC S16 uehSndRgrCfgForRrcReCfg(UehUeCb *ueCb); 
EXTERN S16 uehProcOpStopInUeCntd ARGS((UehUeCb *ueCb));
PUBLIC S16 uehProcCallConnectedRrcErrInd(UehUeCb *cb, NhuErrIndSdus *nhErrIndSdus);
PUBLIC S16 uehProcRelOngoingRrcErrInd(UehUeCb *cb, NhuErrIndSdus *nhErrIndSdus);
PUBLIC S16 uehProcConnectedRrcErrInd(UehUeCb *cb, NhuErrIndSdus *nhErrIndSdus);
PUBLIC S16 uehProcRecOngoingRrcErrInd(UehUeCb *ueCb, NhuErrIndSdus *errIndSdu);
PUBLIC S16 uehProcSmcOngoingRrcErrInd(UehUeCb *ueCb, NhuErrIndSdus *nhErrIndSdus);
PUBLIC S16 uehProcUeCapOngoingRrcErrInd(UehUeCb *ueCb, NhuErrIndSdus *nhErrIndSdus);
PUBLIC S16 uehProcEgtpErrIndInCallCntd(UehUeCb *ueCb, EgtUEvnt *sdu);
PUBLIC S16 uehProcUeCapabilityInfo(UehUeCb *ueCb);
EXTERN S16 uehProcOpStopInS1Con ARGS((UehUeCb *ueCb));
EXTERN S16 uehProcRrcSecModCmp ARGS((UehUeCb *ueCb, void *sdu));
EXTERN S16 uehProcRrcSecModFail ARGS((UehUeCb *ueCb, void *sdu));
EXTERN S16 uehProcRrcReCfgCmp ARGS((UehUeCb *ueCb, NhuDatIndSdus   *datIndSdu));
EXTERN S16 uehSndCtfUeRls ARGS((UehUeCb *ueCb));
EXTERN S16 uehSndCtfUeCfg ARGS((UehUeCb *ueCb));
EXTERN S16 uehSndRgrDedLgChCfg ARGS((UehUeCb *ueCb));
EXTERN S16 uehSndRgrUeRls ARGS((UehUeCb *ueCb));
EXTERN S16 uehSndRgrUeRlsOnRrcReEstabFail ARGS((U16    crnti));
EXTERN S16 uehProcRgrUeReCfgCfm(UehUeCb *ueCb, UeRgrCfgCfm *ueRgrCfgCfm);
#ifdef FT_ERAB_SETUP_REL
EXTERN S16 veSndRrcRecfgReq ARGS((VeUeCb *ueCb, U8 s1Event));
EXTERN S16 veFillRrcRecfgInitCntxtSetup ARGS ((VeUeCb  *ueCb,
                                               NhuDatReqSdus *nhDatReqEvntSdu,
                                               SztInitCntxtSetupRqst *s1apMsg));
EXTERN S16 veFillRrcRecfgERABSetReq ARGS ((VeUeCb   *ueCb, NhuDatReqSdus  *nhDatReqEvntSdu,
                                           SztE_RABSetupRqst   *s1apMsg));
EXTERN S16 veFillRrcRecfgERABModReq ARGS ((VeUeCb   *ueCb,NhuDatReqSdus  *nhDatReqEvntSdu,
                                           SztE_RABMdfyRqst   *s1apMsg));
EXTERN S16 veFillRrcRecfgERABRelCom ARGS ((VeUeCb    *ueCb,NhuDatReqSdus  *nhDatReqEvntSdu,
                                           SztE_RABRlsCmmd   *s1apMsg));
#endif
EXTERN S16 uehSndRgrLchCfg ARGS(( UehUeCb *ueCb, UehUeRAB *rabInfo));
EXTERN S16 uehSndRgrLcgCfg ARGS(( UehUeCb *ueCb, U8 lcId, U8 grpId));
void uehFillRgrUeTxModeCfg ARGS(( RgrUeTxModeCfg *ueCfg));
void uehFillRgrUeDlCqiCfg ARGS(( RgrUeDlCqiCfg *ueCfg, UehUeCb *ueCb,RgrTxMode txMode));
void uehFillRgrUeSrCfg ARGS(( RgrUeSrCfg *ueCfg, UehUeCb *ueCb));
void uehFillRgrUeUlHqCfg ARGS(( RgrUeUlHqCfg *ueCfg));
void uehFillRgrUeUlPwrCfg ARGS(( RgrUeUlPwrCfg *ueCfg));
void uehFillRgrUeQosCfg ARGS(( RgrUeQosCfg *ueCfg));
void uehFillRgrUeTaTmrCfg ARGS(( RgrUeTaTmrCfg *ueCfg));
#ifdef RGR_V1
void uehFillRgrUeBsrTmrCfg ARGS (( RgrUeBsrTmrCfg *ueCfg));
#endif
#ifdef RGR_CQI_REPT
void  uehFillRgrUeNumCqiRept(RgrUeCqiReptCfg *ueCfg);
#endif

void uehFillRgrUeDrxCfg ARGS(( RgrUeDrxCfg *ueCfg));
void uehFillRgrUeAckNackRepCfg ARGS(( RgrUeAckNackRepCfg *ueCfg));
void uehFillRgrUeMeasGapCfg ARGS(( RgrUeMeasGapCfg *ueCfg));
void uehFillRgrUeCapCfg ARGS(( RgrUeCapCfg *ueCfg));
void uehFillRgrCodeBookRstCfg ARGS(( RgrCodeBookRstCfg *ueCfg));

#ifdef LTE_TDD
void uehFillRgrTddAckNackMode ARGS(( RgrTddAckNackMode *ueCfg));
#endif /* LTE_TDD */

EXTERN S16 uehSndRgrUeCfgReq ARGS(( UehUeCb *ueCb, RgrCfgReqInfo *cfgInfo));
RgrCfgReqInfo     *uehCreateAndFillRgrUeReCfgReq( UehUeCb        *ueCb, U16            ueRecfgTypes);
PUBLIC S16 uehSndRgrUeReCfgReq ( UehUeCb *ueCb, RgrCfgReqInfo     *cfgInfo);
EXTERN S16 UehLiEgtEguStaCfm ARGS(( Pst *pst, SuId suId, EgtUEvnt *eguMsg));
EXTERN S16 UehLiEgtEguLclTnlMgmtCfm ARGS(( Pst *pst, SuId suId, EgtUEvnt *eguMsg));
EXTERN S16 UehLiEgtEguUStaInd ARGS(( Pst *pst, SuId suId, EgtUEvnt *eguMsg));
EXTERN S16 UehLiEgtEguErrInd ARGS(( Pst *pst, SuId suId, EgtUEvnt *eguMsg));
EXTERN S16 UehLiPjuBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 UehLiPjuDatCfm ARGS(( Pst  *pst, SuId  suId, CmLtePdcpId *pdcpId, PjuDatCfmInfo  *datCfm));
EXTERN S16 UehLiPjuStaInd ARGS(( Pst *pst, SuId suId, CmLtePdcpId *pdcpId, PjuStaIndInfo  *staInd, Buffer         *mBuf));
EXTERN S16 UehLiPjuDatFwdInd ARGS(( Pst *pst, SuId suId, CmLtePdcpId *pdcpId, PjuDatFwdIndInfo *datFwdInd));

#ifdef FT_INACTIVITY_TMR
EXTERN S16 veInactivityDetected ARGS ((VeEuUeCb    *euUeCb));
#endif

/* ueh_bldmsg.c */
EXTERN S16 uehGetPMICfgIdx ARGS((TknU32 *tkn));
EXTERN S16 uehGetSRCfgIdx ARGS((TknU8 *tkn));
EXTERN S16 uehFillRRC_ConSetup ARGS((NhuDatRspSdus *nhuDatRspSdu,  UehUeCb *ueCb));
void  uehFillRRC_ConReject ARGS((NhuDatRspSdus *nhuDatRspSdu));
EXTERN S16 uehFillRRC_DlInfTr ARGS((NhuDatReqSdus *nhuDatReqSdu,U16 pduLen,U8 *nasPdu, U8 pdcpId));
void uehFillRRC_SecModeCmd ARGS((NhuDatReqSdus *nhuDatReqSdu, UehUeCb *ueCb));
void uehFillRRC_ConRls ARGS(( NhuDatReqSdus *nhuDatReqSdu,UehUeCb *ueCb));
void uehFillNhuRrcConReCfgn ARGS((UehUeCb *ueCb,NhuDatReqSdus *nhDatReqEvntSdu ));
PUBLIC S16 uehFillAndSndUlPwrReconf(UehUeCb  *ueCb);
PUBLIC S16 uehFillNhuUeCapEnquiry ARGS((UehUeCb *ueCb, NhuDatReqSdus  *nhDatReqEvntSdu));
//EXTERN S16 uehDecodeS1PagingMsg ARGS ((S1apPdu       *s1apPdu, UehPagingInfo *pagingInfo ));
EXTERN S16 uehFillAndAddPagingRecord ARGS((S1PagingInfo *pagingInfo));
PUBLIC S16 uehInitPendingPagingList( void );
PUBLIC S16 uehFreePendingPagingList( void );
U8 getPoValFromIsAndNs( U8   is);
PUBLIC S16 uehFillRRCPagingMsg ( NhuDatReqSdus    *datReqSdus[UEH_MAX_I_S], U16              pagSfn);
PUBLIC S16 uehAddPagingRecord ( S1PagingInfo     *pagingInfo, U16               pagSchSfn, U16               tValue, U8                i_s);
PUBLIC S16 uehFillPageRecordInPageOccupation ( NhuDatReqSdus    *datReqSdus, NhuPag           *page, UehPagingRecord  *pendPagUesLstPtr, UINT16            pagingRecIndex);
void uehFillPagHdr ( NhuDatReqSdus   *pagMsg, U16              pagSfn, U8               po);
EXTERN S16 uehFillRRC_ReCfg_ERABSetupLst ARGS((Ptr  evnt, UehUeCb *ueCb, NhuRRCConRecfgn_r8_IEs *reCfgIes));
EXTERN S16 uehFillNhuSRBAddModLst ARGS((Ptr evnt, NhuRadioResourceConfigDedicated *radioResCfg, U8  srbId));
EXTERN S16 uehFillNhuDRBAddModLst ARGS (( Ptr evnt, NhuRadioResourceConfigDedicated *radioResCfg, UehUeCb *ueCb));
EXTERN S16 uehFillUeRadioAccCapContPdu ARGS((NhuEncReqSdus *nhuEncReqSdus, NhuUE_CapRAT_ContLst  *ue_RadioAccessCapInfo, UehUeCb *ueCb));
void uehFillNhuMACMainCfg ARGS((Ptr  evnt, NhuRadioResourceConfigDedicated *radioResCfg, UehUeCb *ueCb));
void uehFillRadResPhyCfgDed ARGS((Ptr evnt, NhuRadioResourceConfigDedicated *radioResCfg, UehUeCb *ueCb));
EXTERN S16 uehFillUeAddRbCfg ARGS((NhuRbCfgList  **rbCfgList,UehUeCb        *ueCb,U8            msgType));
EXTERN S16 uehFillNhuUeCfgInfo ARGS((Ptr           evnt, NhuUeCfgInfo  *ueCfg, UehUeCb *ueCb,U8 msgType));
EXTERN S16 uehFillPdcpCfg ARGS((NhuPdcpCfg  *pdcpCfg, U8 nhuRlcMode));
EXTERN S16 uehFillTrChCfg ARGS((NhuTrChCfg    *trChCfg));
EXTERN S16 uehFillSecCfg ARGS((NhuSecurityCfg     *secCfg, UehUeCb  *ueCb));
EXTERN S16 uehFillUlLogChCfg ARGS((NhuUlLogChMap *ulLogChMap,U8 rbId));
EXTERN S16 uehFillUlRlcModeInfo ARGS((NhuUlRlcMode  *ulRlcMode,U8  rabId));
EXTERN S16 uehFillDlRlcModeInfo ARGS((NhuDlRlcMode  *dlRlcMode,U8  rabId, U8 ueCategory));
void uehFillNhuEvntHdr ARGS((NhuHdr      *nhuHdr, U16 cellId,U16 crnti, U8 msgType));

typedef S16 (* UehSztLIMFSM)    ARGS((UehS1ConCb *s1ConCb,S1apPdu *pdu));

EXTERN S16 uehSzFillTknU32 ARGS((TknU32 *encParam,U32 val));
EXTERN S16 uehSzFillTknU8 ARGS((TknU8 *encParam,U8 val));
EXTERN S16 uehSzFillTknU16 ARGS((TknU16 *encParam,U16 val));
EXTERN S16 uehCpyCmTptAddr ARGS((CmTptAddr *dst, CmTptAddr *src));
EXTERN S16 uehSztUtilsGetMsgIdx ARGS((U8 *msgIdx,S1apPdu *pdu));
EXTERN S16 uehSzFillTknStr4 ARGS ((TknStr4 *ptr, U8 len, U8 val));
EXTERN S16 uehSzFill2TknStr4 ARGS ((TknStr4 *ptr, U8 len, U8 *val));
EXTERN S16 uehSzFillTknBitStr4 ARGS ((TknStr4 *ptr, U8 len, U8 *val));
EXTERN S16 uehSzFillTknStrOSXL ARGS ((TknStrOSXL *ptr, U16 len, Data *val, S1apPdu  **pdu));
EXTERN S16 uehSzFillTknStrOSXL1 ARGS ((TknStrOSXL *ptr, U16 len, U32 val, S1apPdu  **pdu));
EXTERN S16 uehSzFillTknBStr32 ARGS ((TknBStr32  *ptr, U16  len, U32  val));

EXTERN S16 UehLiNhuBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 UehLiNhuDatInd ARGS(( Pst *pst, SuId suId, NhuDatIndSdus *nhuDatIndSdus));
EXTERN S16 UehLiNhuDatCfm ARGS(( Pst *pst, SuId suId, NhuDatCfmSdus *nhDatCfmSdus));
EXTERN S16 UehLiNhuStaInd ARGS(( Pst *pst, SuId suId, NhuStaIndSdus *nhStaIndSdus));
EXTERN S16 UehLiNhuErrInd ARGS(( Pst *pst, SuId suId, NhuErrIndSdus *nhErrIndSdus));
EXTERN S16 UehLiNhuCfgCfm ARGS(( Pst *pst, SuId suId, NhuCfgCfmSdus *nhuCfgCfmSdus));
EXTERN S16 UehLiNhuCellCfgCfm ARGS(( Pst *pst, SuId suId, NhuCellCfgCfmSdus *nhuCellCfgCfmSdus));
EXTERN S16 UehLiNhuDecodeCfm ARGS(( Pst *pst, SuId suId, NhuDecodeSduCfm *nhuDecodeSduCfm));
EXTERN S16 UehLiNhuCountCfm ARGS(( Pst *pst, SuId suId, NhuDRBCountInfoLst *nhuDRBCountInfoLst));
EXTERN S16 UehLiNhuEncodeCfm ARGS(( Pst *pst, SuId suId, NhuEncCfmSdus *nhuEncCfmSdus));
EXTERN S16 UehLiNhuPdcpSduStaCfm ARGS(( Pst *pst, SuId suId, NhuPdcpSduStaCfm *nhuPdcpSduStaCfm));
EXTERN S16 UehLiUdhUeInactvInd ARGS((Pst *pst, UdhUehUeInactiveInd *udhUehInactiveUeInd));
EXTERN S16 UehLiUdhAddRabCfm ARGS((Pst *pst, UdhUehAddRabCfm *udhUehAddRabCfm));
EXTERN S16 UehLiUdhDelRabCfm ARGS((Pst *pst, UdhUehDelRabCfm *delRabCfm));
EXTERN S16 UehLiUdhDelUeCfm ARGS((Pst *pst, UdhUehDelUeCfm *delUeCfm));
EXTERN S16 UehLiUdhBndCfm ARGS((Pst *pst, SuId suId, U8 status));

EXTERN S16 uehProcCellRgrCfgCfm ARGS((U8 status));
EXTERN S16 uehProcRecOngoingRgrCfgCfm ARGS((UehUeCb *ueCb, UeRgrCfgCfm *ueRgrCfgCfm));
PUBLIC S16 uehProcRrcConRgrCfgCfm ARGS((UehUeCb *ueCb, UeRgrCfgCfm *ueRgrCfgCfm));
PUBLIC S16 uehProcRelOngoingRgrCfgCfm (UehUeCb *ueCb, UeRgrCfgCfm *ueRgrCfgCfm);
PUBLIC S16 uehProcCtfCfgCfm  ARGS((UehUeCb *ueCb, UeCtfCfgCfm *ueCtfCfgCfm));
EXTERN S16 uehProcRgrLchLcgCfgCfm (UehUeCb *ueCb, UeRgrCfgCfm *ueRgrCfgCfm);
EXTERN S16 uehProcRgrUeCfgCfm (UehUeCb *ueCb, UeRgrCfgCfm *ueRgrCfgCfm);
EXTERN S16 uehProcRrcIdleRrcErrInd ARGS((UehUeCb *ueCb, NhuErrIndSdus *nhErrIndSdus));
EXTERN S16 uehProcRrcConRrcErrInd ARGS((UehUeCb *ueCb, NhuErrIndSdus *nhErrIndSdus));
EXTERN S16 uehProcS1ConRrcErrInd ARGS((UehUeCb *ueCb, NhuErrIndSdus *nhErrIndSdus));

EXTERN S16 UehLiRgrBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 UehLiRgrCfgCfm ARGS(( Pst *pst, SuId suId, RgrCfgTransId  transId,U8 status));
EXTERN S16 UehLiCtfBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 UehLiCtfCfgCfm ARGS(( Pst *pst, SuId suId, CtfCfgTransId transId,U8 status));
EXTERN S16 UehLiCtfUeIdChgCfm ARGS(( Pst *pst, SuId suId, CtfCfgTransId transId,CtfUeInfo *ueInfo,U8 status));
EXTERN S16 uehLiCtfDatRsp(Pst *pst,SuId suId,CtfCfgTransId transId,CtfKdfCfmInfo *datRspSdu,U8 status); 

EXTERN S16 uehGenerateCfm ARGS((Pst *pst, Header *hdr, U8 prim, UehMngmt *cfm));
EXTERN S16 uehSndEgtpComDed ARGS((U8 egtpCom, UehS1ConCb *s1apConCb));
PUBLIC S16 uehSndEgtpCom(U8 egtpTunOpt,UehUeCb *ueCb);
EXTERN Void uehSendLmAlarm ARGS((U16 category, U16 event, U16 cause));
EXTERN S16 uehSzGetIE ARGS(( SztS1AP_PDU *pdu, U32 id, TknU8 **ie));
EXTERN void uehInitUeSrbInfo ARGS(( UehUeRABInfo *srbCfg, U8 rabId));
EXTERN S16 uehFillRRC_ConReEstblReject ( NhuDatRspSdus *nhuDatRspSdu);
#ifdef UEH_PERF_DL_DATA
EXTERN Void uehPerfTmrExp ARGS((PTR pCb, S16 tmrEvnt));
#endif
//EXTERN void uehInitTmrDescTable(void);




#ifdef UEH_PERF_MEAS

PUBLIC S16 uehPerfLIMRrcConSetCmpHndlr ARGS((NhuDatIndSdus *datIndSdu));

PUBLIC S16 uehPerfSndSecModeCmnd ARGS((UehUeCb *ueCb));

PUBLIC S16 uehPerfFillRRC_SecModeCmd ARGS ((NhuDatReqSdus *nhuDatReqSdu, UehUeCb
*ueCb));

PUBLIC S16 uehPerfFillUeCfgInfo ARGS((UehUeCb        *ueCb));

#endif  /* UEH_PERF_MEAS */


PUBLIC S16 uehSndBindCtrlRgToTfuSap(U32 action);
EXTERN void uehProcActiveUeStatusReq(Buffer *mBuf);
EXTERN void uehProcModStateReq(Buffer *mBuf);
EXTERN void uehProcActiveRabStatusReq(Buffer *mBuf);
EXTERN void uehProcUeEventHistoryStatusReq(Buffer *mBuf);
EXTERN void uehFillUeEventHistoryRsp(MsgUehBsmUeEventHistoryRsp *ueEventHisRsp);
EXTERN void uehLoglatestEvts(MsgUehBsmUeEventHistoryRsp *ueEventHisRsp);
EXTERN void fillProcDetail(U8 procEvent,U8 checkEvent,MsgUehBsmUeEventHistoryRsp *ueEventHisRsp,U16 ueIdx);
EXTERN void validateUeIdx(MsgUehBsmUeEventHistoryRsp *ueEventHisRsp,U8 idx);
EXTERN S16 uehCrmInitializePucchResTables ARGS((void));
EXTERN TBOOL uehCrmAddUe ARGS ((S16 ueId, AcParams *fillme));
EXTERN S16 uehCrmDeleteUe ARGS ((S16 ueId));
EXTERN void UehPrepRgrCfgReq(UehUeCb * ueCb,	 RgrCfgReqInfo *cfgInfo);
EXTERN S16 UehPrepRgrReCfgReq(UehUeCb * ueCb,	 RgrCfgReqInfo *cfgInfo);

//RRM  Changes
EXTERN void uehProcRrmCellCounter(Buffer *mBuf);

#ifdef LTE_L2_MEAS
UehL2MeasProfileCfg  *uehGetL2mProfile(ProfileId profileId, UehL2MeasCtx  *uehL2MeasCtx);    
UehL2MeasProfileCfg *uehGetEmptyL2mProfile(UehL2MeasCtx *uehL2MeasCtx);    
void uehClearL2MeasCtx(void);
void uehClearL2mProfile(UehL2MeasProfileCfg  *profileCfg);
U32 getCtxSize(U32 identity);
void uehConfigureProfile(UehL2MeasCtx *uehL2MeasCtx, 
                         UehL2MeasProfileCfg  *profileCfg, 
                         MsgPmhUehCounterStartReq   *counterStartReq);
void uehConfigureCounter(UehL2MeasCtx *uehL2MeasCtx, 
                         UehL2MeasProfileCfg  *profileCfg, 
                         CounterId counterId);
void uehDumpProfile(void);
U8 getQciId(CounterId counterId);
void uehStartProfile(UehL2MeasProfileCfg  *profileCfg);
TBOOL uehVerifyL2mCfm( UehL2MeasProfileCfg *profile, U32 identity, U32 transId);
void uehSamplingTimerExpiry(UehL2MeasProfileCfg *profile);

void getTimeStamp(char  *dataAndTimeStampField);

PUBLIC S16 uehMacL2MeasReq(UehL2MeasProfileCfg  *profileCfg, CounterLink  *counter);
PUBLIC S16 uehRlcL2MeasReq(UehL2MeasProfileCfg  *profileCfg, CounterLink  *counter);
PUBLIC S16 uehPdcpL2MeasReq(UehL2MeasProfileCfg  *profileCfg, CounterLink  *counter);

PUBLIC S16 uehLrgSchL2MeasReq (LrgSchMeasReqInfo  *measInfo, Buffer *mBuf);
S16 uehKwL2MeasReqInfo (KwL2MeasReqInfo *param, Buffer *mBuf );
PUBLIC S16 uehLkwL2MeasReq( KwL2MeasReqEvt *measReqEvt,Buffer *mBuf);
PUBLIC S16 uehLpjL2MeasReq(PjL2MeasReqEvt *measReqEvt, Buffer *mBuf);
PUBLIC S16 uehPjL2MeasReqInfo ( PjL2MeasReqInfo *param, Buffer *mBuf);

PUBLIC S16 uehSndMsgToSm (IN EventType uniqueEventType, void *msg, Pst *pst);
void UehPrepLrgL2MeasReq( LrgSchMeasReqInfo     *measCfgInfo, 
                          UehL2MeasProfileCfg   *profileCfg, 
                          CounterLink           *counter);
void UehPrepKwL2MeasReqEvt (KwL2MeasReqEvt      *measReqEvt,
                            UehL2MeasProfileCfg *profileCfg, 
                            CounterLink         *counter);
void UehPrepPjL2MeasReqEvt (PjL2MeasReqEvt      *measReqEvt,
                            UehL2MeasProfileCfg *profileCfg, 
                            CounterLink         *counter);

PUBLIC U16 getMeasPeriod(SamplingInterval   interval);

PUBLIC S16 uehMacL2MeasCfm( Pst *pst, LrgSchMeasCfmInfo  *measInfo);
PUBLIC S16 uehRlcL2MeasCfm( Pst *pst, KwL2MeasCfmEvt  *measCfmEvt);
PUBLIC S16 uehPdcpL2MeasCfm( Pst *pst, PjL2MeasCfmEvt  *measCfmEvt);

void uehL2mSamplingUpdate(UehL2MeasProfileCfg  *profile, TBOOL statusSucc);
void uehBuildAndSendCounterReport(UehL2MeasProfileCfg *profile);
void uehSendCounterStopInd(UehL2MeasProfileCfg *profile, MeasurementStopCause cause);
#endif /*LTE_L2_MEAS*/

/* Time to string function for call Trace time prints */
PUBLIC S8 UehStrTime (char *timestr);

PUBLIC S8 UehFlushTrace(void);
PUBLIC S8 UehSaveTrace (char *trc);
NhuTPollReTx uehGetPollRetxForOamVal(AmRlcTPollRetransmit AmPlRtx);
NhuPollPdu uehGetPollPduForOamVal(AmRlcPollPDU AmPlPdu);
RgrPwrAlpha uehGetAlphaForOamVal(Alpha alpha);
NhuRlcPollByte uehGetPollByteForOamVal(AmRlcPollByte AmPlbyte );
NhuMaxRetxThres uehGetMaxRetxThreForOamVal(AmRlcMaxRetxThreshold MaxRetxThrsld );
NhuTReOrdering uehGetTReordForOamVal(AmRlcTReordering AmTreord);
NhuTStatusProh uehGetStatProhForOamVal (AmRlcTStatusProhibit AmStatProh );
NhuPdcpDiscardTmr uehGetDiscTmrForOamVal (DiscardTimer DisTmr);
NhuMaxHarqTx uehGetMaxHargTxForOamVal(MaxUlHarqTx MxHarqTx);

PUBLIC S8 UehInitSaveQ(UehUeCb *uecb);
PUBLIC S8 UehSaveInput(int event, void *cb, void *sdu);
PUBLIC UecmSaveQNode UehPopSaveQHead(UehUeCb  *ueCb);


/* SRS */
void uehCrmAllocateSrs( UehUeCb *ueCb);
void uehCrmDeallocateSrs( UINT16      sfIndx, UINT16      combIndx, UINT16      csIndx);


PUBLIC S16 sendUehMsgToFrm( Ent     dstEnt, Event   event, MsgLen     len, U8     *data);
PUBLIC S16 sendUehMsgToBsm( Event   event, MsgLen     len, U8     *data);
PUBLIC S16 sendSmMsgToBsm( Event   event, MsgLen     len, U8     *data);
PUBLIC S16 sendSmMsgToLth( Event   event, MsgLen     len, U8     *data);
PUBLIC S16 sendUehMsgToPrm( Event   event, MsgLen     len, U8     *data);
void sendUehStartIndToPrm(void);
void sendUehHelloAckToPrm(Buffer   *mBuf);
void procRcvdUehExitMsg(void);
PUBLIC S16 sendCemMsgToBsm( Event event, MsgLen     len, U8     *data);
PUBLIC S16 sendS1mMsgToBsm( IN Event   event, IN MsgLen     len, IO U8     *data);
void s1mProcS1mCfgReq( Buffer *mBuf);

PUBLIC S16 uehSndMsgOut ( IN  Ent            dstEnt, IN  EventType         uniqueEventType, IO  UehUeCb           *ueCb, IO  void              *transId, FR  void              *msg);
PUBLIC S16 uehSndMsgToUdh ( IN  EventType uniqueEventType, void *msg  );
PUBLIC S16 uehSndMsgToPhy ( IN EventType uniqueEventType, IO  void     *transId, void         *msg);
PUBLIC S16 uehSndMsgToSch ( IN EventType uniqueEventType, IO void      *transId, void         *msg );
PUBLIC S16 uehSndMsgToS1 ( IN  EventType         uniqueEventType,IO UehUeCb               *ueCb, void                  *msg); 
PUBLIC S16 uehSndMsgToRrc ( IN  EventType         uniqueEventType, IO UehUeCb               *ueCb, void                  *msg);

void uehSendS1apErrorInd(U16 crnti,UINT32 errCause);

//void uehFillAndSndUlPwrReconf( UehUeCb  *ueCb );
PUBLIC Void uehStopAllUeTmrs(UehUeCb *ueCb);

char *uehGetUeStateName( UeState ueState );
char * getS1Cause (const UehUeCb *ueCb);
void fillEventDetail(const char *format, ...);
PUBLIC S16 UehLiSztAbortCfm (Pst *pst,SuId suId,SztAbortS1 *abortS1);
PUBLIC void UehLiSztUAbortReq(Pst *pst,SpId spId, SztAbortS1 *s1Abort);
PUBLIC S16 uehLiCtfDatReq(Pst *pst,SpId spId,U16 crnti,CtfKdfReqInfo *ctfKdfReqInfo);
PUBLIC S16 uehSndCtfDatReq ( CtfKdfReqInfo *evt ,U16 crnti);
PUBLIC S16 uehFillCtfEvntHdr(CtfHdr *ctfHdr, U16 cellId, U16  crnti);
void fillReconfgCount(U8 ueEvt,U8 isIncoming,U16 ueIdx);
S16 createAndSendSztLocalRelReq(UConnId suConnId);


#ifdef LTE_L2_MEAS
PUBLIC S16 smPdcpFwdL2MeasCfm
    (
    Pst     *pst, 
    Buffer  *mBuf
    );
PUBLIC S16 smRlcFwdL2MeasCfm
    (
    Pst     *pst,
    Buffer  *mBuf
    );
PUBLIC S16 smMacFwdL2MeasCfm
    (
    Pst     *pst,
    Buffer  *mBuf
    );
PUBLIC S16 cmUnpkLrgSchL2MeasReq (LrgSchL2MeasReq func,Pst *pst,Buffer *mBuf);
PUBLIC S16 cmUnpkLkwL2MeasReq (LkwL2MeasReq func,Pst  *pst,  Buffer *mBuf);
#endif
//PUBLIC S16 cmUnpkLpjL2MeasReq ( LpjL2MeasReq func,Pst *pst,Buffer *mBuf);

void uehFillRgrPuschDedCfg(
        RgrUePuschDedCfg *puschDedCfg
          );
U32 getProcIdByEntityId(
        Ent entityId
        );
//inline const UINT8* errorCodeStr(IN BsErrorValue enumNum);
ModuleId sfiGetModIdFromEntityId(
  Ent entityId
  );
void stopCounterCollection(void);
#ifdef LTE_L2_MEAS
EXTERN S16 cmUnpkLrgSchL2MeasCfm ARGS(( LrgSchL2MeasCfm func,Pst  *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLkwL2MeasCfm ARGS
((
LkwL2MeasCfm func,
Pst *pst,
Buffer *mBuf
));
//EXTERN S16 cmUnpkLpjL2MeasCfm ARGS(( LpjL2MeasCfm func,Pst  *pst,Buffer *mBuf));
#endif


void uehProcCounterStartReq(
    Buffer *mBuf
      );
void uehProcCounterStopReq(
    Buffer *mBuf
      );
void readFromFileUehConfigTable(
  IN UINT8                   *fileToRead,
  IO UehConfig               *ptrUehConfig,
  IO UINT8                   *error
        );
void readFromFileEgtpConfigTable(
  IN UINT8                   *fileToRead,
  IO EgtpConfig              *ptrEgtpConfig,
  IO UINT8                   *error
        );
void readFromFileSctpConfigTable(
  IN UINT8                   *fileToRead,
  IO SctpConfig              *ptrSctpConfig,
  IO UINT8                   *error
        );
void readFromFileS1apConfigTable(
  IN UINT8                   *fileToRead,
  IO S1apConfig              *ptrS1apConfig,
  IO UINT8                   *error
        );
int PW_PALInitLibrary(void);
S16 ipcInit(
    S16 selfProcId,
    S16 lowProcId,
    S16 highProcId);
S16 regDriverTask(S16 driverInst,S16 destProcId);

void uehDeallocateResources(
  FR UehUeCb    *ueCb
);

PUBLIC S16 uehSendUeCtxRelReq(
  UehUeCb    *ueCb,
  S1Cause    cause
);

void cemProcOpstopCmd(Buffer   *mbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
