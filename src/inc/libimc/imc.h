/**
 * @file    imc.h
 * @brief   Inter Module Communication
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author   Lakshmi Narayan Madala
 *
 *  */

#ifndef IMC_H
#define IMC_H

#include "bs_config.h"     /* global configuration */
#include "imc_module.h"    /* ModuleId */
#include "msg_ids.h"       /* MsgId  */
#include "basic_types.h"   /* UINT16, UINT8  */
#include "cmn_types.h"     /* ImcMethod */

/* Types */
typedef struct s_Msg
{
  MsgId    msgId;
  ModuleId src;
  ModuleId dest;
  ModuleId via;
  UINT16   len;             /* len of data in bytes */
  UINT8    data[0];         /* user is only required to fill data */
} __attribute__((__packed__))Msg;

typedef struct s_ModAddr
{
  ModuleId  modId;
  UINT8     ipv4[LEN_IPV4_ADDR];
  UINT16    udpPort;
  SINT32    unixSendFd;
  SINT32    udpSendFd;
  SINT32    recvFd;
  ImcMethod imcMethod;
  char      unixSockFilePath[UNIX_SOCK_PATH_MAX];
} ModAddr;

/* ************************************************************
 * The state variables of "Inter Module Communication provider" module
 */

/* Msg Counters changes started */
typedef enum e_MsgRcvErrCause
{
  ERR_NUM_OF_BYTES_MISMATCH,
  ERR_INVLD_MSG_DESC_RCVD,
  ERR_MISCELLANEOUS,
  MAX_ERROR_CAUSES
} MsgRcvErrCause ;

typedef struct s_SndMsgCntrsCtx
{
  UINT16 msgSentCntr[MSG_ID_MAX]; /* Counters for successfully sent messages */
  UINT16 msgSentErrCntr[MSG_ID_MAX];/*Counters for failed for sending  messages */
} SndMsgCountersCtx;

typedef struct s_RcvMsgCntrsCtx
{
  UINT16 msgRcvdCntr[MSG_ID_MAX]; /*Counters for successfully received messages */
  UINT16  msgRcvdErrCntr[MAX_ERROR_CAUSES];/*Counters for failed receive messages */
} RcvMsgCountersCtx;
 
extern TBOOL msgCntrsRunning ;
/*Message Counter Changes Ended */

typedef struct s_ImcContext
{
  Count   numMsgInst[MSG_ID_MAX]; /* How many have been created and yet not freed. */
  ModAddr modAddrTab[MODID_MAX];
  SndMsgCountersCtx sndMsgCountersCtx;/* Context to handle send message counters*/
  RcvMsgCountersCtx rcvMsgCountersCtx; /* Context to handle receive message counters*/
  UINT16  maxMsgSize;
} ImcContext;

/* Global */
extern ImcContext g_ImcContext;
#define getGlobalImcContext()      &(g_ImcContext)
#define getSendMsgCntrContext()    &(g_ImcContext.sndMsgCountersCtx)
#define getRcvMsgCntrContext()     &(g_ImcContext.rcvMsgCountersCtx)
#define getMaxMsgSize()              g_ImcContext.maxMsgSize
#define setMaxMsgSize(val)           g_ImcContext.maxMsgSize = (val)
#define getTotalMsgSize(frmMsgPtr)   sizeof(Msg)+(frmMsgPtr)->len
#define isErrorCauseIdValid(errorCauseId) (errorCauseId < MAX_ERROR_CAUSES)

/* Internal Functions */
Msg *createMsgInst(
  IN MsgId  msgId,
  IN UINT8  *data,
  IN UINT16 dataLen
);

void freeMsgIn(
    FR Msg       *msgIn
    );

void freeMsgInst(
  Msg* msg
);

void sendMsgInst(
    Msg *msg
    );

/** ************
 *  API
 ***************/
#if 0
void initImcContext(void);
#endif

#define createMsg(msgId) \
  createMsgInst(msgId, NULL, 0)

#define sendMsg(msg) \
  sendMsgInst(msg), msg = NULL

#define freeMsg(msg) \
  freeMsgInst(msg), msg = NULL


#define getMsgLen(buf) ((Msg*)buf)->len + sizeof(Msg)

Msg *recvMsg(
  IN ModuleId modId,
  IO UINT16   *ec
  );

TBOOL registerForImc(
  IN ModuleId modId
);

void deregisterForImc(
  IN ModuleId modId
);

TBOOL createAndSendMsg(
  IN MsgId  msgId,
  IN UINT8 *data,
  IN UINT16 len
);

/* This is required by tput_tester */
ModAddr *getModAddr(
  IN ModuleId modId
  );

inline void incMsgInst(MsgId msgId);

/* Message Counters changes started */

TBOOL startMsgCntrs(void
);

TBOOL stopMsgCntrs(void
);

TBOOL logSendMsgCntrs(void
);

TBOOL logSendMsgErrCntrs(void
);

TBOOL logRcvdMsgCntrs(void
);

TBOOL logRcvdMsgErrCntrs(void
);

TBOOL logMsgCntrForMsgId(MsgId
);

TBOOL logRcvdMsgErrCntrForErrCause(MsgRcvErrCause
);

TBOOL resetSendMsgCntrs(void
);

TBOOL resetSendMsgErrCntrs(void
);

TBOOL resetRcvdMsgCntrs(void
);

TBOOL resetRcvdMsgErrCntrs(void
);

TBOOL resetMsgCntrForMsgId(MsgId
); 

TBOOL resetRcvdMsgErrCntrForErrCause(MsgRcvErrCause
);

/* Message Counter Changes Ended */

#endif /* IMC_H */



