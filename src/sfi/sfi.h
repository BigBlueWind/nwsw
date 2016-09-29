/**
 * @file    sfi.h
 * @brief CCPU has a System Services Interface (SSI).
 *          PWAV has its own mechanism for Inter-Module Communication (called FRM).
 *          SFI interfaces SSI, FRM messages.
 *          Hence it is called SSI-FRM-Interface (SFI)
 * @details Copyright Powerwave Technologies, Inc., 2010
 */

#ifndef SFI_H
#define SFI_H
#include "ccpu_cmn.h"
#include "cmn_types.h"
#include "pst_events.h"
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "libconfig.h"
#include "start_indication.h"

#define CONFIG_FILE_LENGTH       100


/* Types */
typedef struct s_SfiMsg /* Duplicated from frm */
{
  U32    msgId;
  U32    src;
  U32    dest;
  U32    via;
  U16    len; /* len of data in bytes */
  /* user is only required to fill data */
  U8    data[]; /* Changed from data[0] to data[] to avoid warning when compiling with c99*/
} __attribute__((__packed__)) SfiMsg; /*Need to pack it*/

typedef SfiMsg FrmMsg;

/** SFI Context */
typedef struct s_sfiCb
{
  TskInit         init;                    /* task initialization info     */
  U32             dbgMask;
  CmInetAddr      frmModAddrByModId[MODID_MAX];
  CmInetFd        sockFd;
  CmInetFdSet     readFdSet;
  MsgId           msgIdFromEventAndEntityArray[255][NUM_PWAV_EVENTS]; /* ENT x EVENT */
  /*TODO: How many entities are there? */
}SfiCb;

extern SfiCb      g_sfiCb;

/*Macros*/
#define getSfiCb()      (&g_sfiCb)
#define OFFSET(structName, structElement)  (unsigned int)(&(((structName *)(0))->structElement))
#if 0
#define SFI_FREEMBUF(_mBuf)\
  do{\
      if (_mBuf != NULLP) {\
        (Void)SPutMsg((_mBuf));\
        (_mBuf) = NULLP;\
      }\
  }while(0)
#endif
/*** Function Prototypes ************/
MsgId getMsgIdBySrcEntityAndEventId(
    Ent src,
    Event eventId
    );
void sfiInitMsgIdByEventAndSrcEntity(
    SfiCb *sfiCb
    );
void sfiSendStartQueryToEnt(
    Ent entity
    );
void sfiSendStartQueryToAllModules(void);
char* getEventName(U32 evtId);
//void sfiDeInit(void);
U32 getEventFromMsgId(
    MsgId msgId
  );
ModuleId sfiGetModIdFromEntityId(
    Ent entityId
    );
Ent sfiGetEntityFromModId(
    ModuleId modId
    );
U32 getProcIdByEntityId(
  Ent entityId
  );

#endif /*end of SFI_H*/

