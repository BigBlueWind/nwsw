/**
 * @file    sfi.c
 * @brief This module is the SSI<-->FRM Interface.
 *          It converts FRM-Msg to (pst, mBuf) and vice-versa
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 */

#include "pst_events.h"
#include "msg_ids.h"
#include "bs_modules.h"
#include "sfi.h"
#include "libconfig.h"
#include "ueh.h"
#include "log.h"
#include "msg_names.h"
/* SFI Context */
SfiCb                   g_sfiCb;

/*Macros*/
#define getSfiCb()      (&g_sfiCb)
#define OFFSET(structName, structElement)  (unsigned int)(&(((structName *)(0))->structElement))

#if 0
#define SFI_FREEMBUF(_mBuf)\
  do{\
      if (_mBuf NOTEQ NULLP) {\
        (Void)SPutMsg((_mBuf));\
        (_mBuf) = NULLP;\
      }\
  }while(0)
#endif

/** FUNCTIONS **/
inline MsgId getMsgIdBySrcEntityAndEventId(
    Ent src,
    Event eventId
    )
{
  /* see sfiInitMsgIdByEventAndSrcEntity to understand '- PWAV_START_EVENT' */
  return getSfiCb()->msgIdFromEventAndEntityArray[src][eventId - PWAV_START_EVENT];
}

/**
 * This function sends start Query to respective Module
 * Every module is waiting for messages on message queue
 * We enquire the status of modules by sending start query,
 * Module responds with START_IND towards BSM
 */
void sfiSendStartQueryToEnt(
    Ent entity
    )
{
  Pst           sendPst;
  Buffer        *mBuf;
  SfiCb         *sfiCb = getSfiCb();

  if (SGetMsg(sfiCb->init.region, sfiCb->init.pool, &mBuf) NOTEQ ROK)
  {
    logError("sfiSendStartQueryToEnt(): Allocating Memory for event EVTSTARTQ Failed to entity=%d", entity);
    return;
  }

  cmMemset((U8 *)(&sendPst), 0, sizeof(Pst));
  sendPst.dstEnt    = entity;
  sendPst.dstProcId = getProcIdByEntityId(entity);

  sendPst.srcEnt    = ENT_PRM;
  sendPst.srcProcId = getProcIdByEntityId(sendPst.srcEnt);

  sendPst.dstInst  = 0;
  sendPst.srcInst  = 0;
  sendPst.event    = EVT_START_QUERY;

  sendPst.pool     = sfiCb->init.pool;
  sendPst.selector = SM_SELECTOR_LC;
  sendPst.region   = sfiCb->init.region;

  SPstTsk(&sendPst, mBuf);
}

/**
 * This function sends start Query to all the modules
 */
void sfiSendStartQueryToAllModules(void)
{
  /* Modules belongs to cp exe */
  sfiSendStartQueryToEnt(ENTNH);
  sfiSendStartQueryToEnt(ENTUEH);
  sfiSendStartQueryToEnt(ENTSZ);
  sfiSendStartQueryToEnt(ENTSB);
  /* TODO: Add when supported
  sfiSendStartQueryToEnt(ENTS1M);
  sfiSendStartQueryToEnt(ENTCEH);
  */

  /* Modules belongs to gtp exe */
  sfiSendStartQueryToEnt(ENTEG);
  sfiSendStartQueryToEnt(ENTHI);
  sfiSendStartQueryToEnt(ENTUDH);

  /* Modules belongs to lowerarm */
  sfiSendStartQueryToEnt(ENTKW);
  sfiSendStartQueryToEnt(ENTPJ);
  sfiSendStartQueryToEnt(ENTRG);

}
#if 0
/* This function opens imc.cfg and fills the context in cfg */
void openImcConfig(
    OUT config_t *cfg
    )
{
  char *rootDir = NULL;
  char filename[CONFIG_FILE_LENGTH];

  rootDir = msArgv[2];
  if ( rootDir EQ NULL )
  {
    fprintf( stderr, "Environment variable --rootDir is not set !");
    exit(EXIT_FAILURE);
  }

  sprintf( filename,"%s/config/imc.cfg", rootDir );

  config_init(cfg);
 // logInfo("%s", "sfiInitCtx(): Reading imc.cfg file");
  if(! config_read_file(cfg, filename))
  {
    fprintf(stderr, "%s:%d - %s\n",
        config_error_file(cfg),
        config_error_line(cfg),
        config_error_text(cfg));

    logError("%s", "openImcConfig(): Incorrect config file path");
    config_destroy(cfg);
    exit(EXIT_FAILURE);
  }
}
#endif
/**
 * This function Reads the config file, and initialises SFI CallBlock
 */
#if 0
void sfiInitCtx(
  IO SfiCb  *sfiCb
  )
{
  config_t          cfg;
  config_setting_t *setting;
  U16               count;
  U16               i=0;
  config_setting_t *table;
  const char       *moduleName, *ipAddress;
  ModuleId          modId;
  int               port;

  memset(sfiCb, 0, sizeof(SfiCb));
  sfiCb->init.region = DFLT_REGION;
  sfiCb->init.pool   = DFLT_POOL;
  sfiCb->dbgMask     = 0xFF;

  /* Read the IPAddress and Ports of FRM Modules */


  openImcConfig(&cfg);
  setting = config_lookup(&cfg, "module_cfg_table");
  if(setting NOTEQ NULL)
  {
    count = config_setting_length(setting);
    for (i=0; i< count; i++)
    {
      table = config_setting_get_elem(setting, i);
      config_setting_lookup_string(table, "moduleName", &moduleName );
      config_setting_lookup_string(table, "ipAddress", &ipAddress);
      config_setting_lookup_int(table, "port", &port);

      for(modId = 0 ; modId < MODID_MAX ; modId++)
      {
        if(strcmp(getModuleName(modId), moduleName) EQ 0)
        {
          sfiCb->frmModAddrByModId[modId].port  = port;
          cmInetAddr(ipAddress, &(sfiCb->frmModAddrByModId[modId].address) );
          sfiCb->frmModAddrByModId[modId].address = CM_INET_HTON_U32(sfiCb->frmModAddrByModId[modId].address);
          break;
        }
      }
    }
  } /* if */
  config_destroy(&cfg);
  sfiInitMsgIdByEventAndSrcEntity(sfiCb);
}
#endif

/**
 * This function de-initializes the socket interface to the FRM
 */
#if 0
void sfiDeInit(void)
{
   SfiCb *sfiCb = getSfiCb();

   logInfo("%s", "sfiDeInit(): Shutting down the UDP IPC");

   /*-- clear interrupt pending flag --*/
   SSetIntPend((U16)1, FALSE);

   /* close the socket */
   (Void) cmInetClose(&sfiCb->sockFd);

   CM_INET_SET_INV_SOCK_FD(&sfiCb->sockFd);

   logInfo("%s", "sfiDeInit(): UDP IPC Shutdown Done");

   RETVOID;
}
#endif
/*
 * This function returns the CCPU Event for a given message Id
 */
U32 getEventFromMsgId(
    MsgId msgId
  )
{
  switch(msgId)
  {
    case MSG_PRM_SCTP_HELLO:
    case MSG_PRM_EGTP_HELLO:
    case MSG_PRM_TUCL_HELLO:
    case MSG_PRM_MAC_HELLO:
    case MSG_PRM_PDCP_HELLO:
    case MSG_PRM_RLC_HELLO:
    case MSG_PRM_RRC_HELLO:
    case MSG_PRM_S1AP_HELLO:
    case MSG_PRM_UEH_HELLO:
    case MSG_PRM_UDH_HELLO:
      return EVT_HELLO;

    case MSG_PRM_RRC_EXIT_CMD:
    case MSG_PRM_S1AP_EXIT_CMD:
    case MSG_PRM_PDCP_EXIT_CMD:
    case MSG_PRM_RLC_EXIT_CMD:
    case MSG_PRM_MAC_EXIT_CMD:
    case MSG_PRM_EGTP_EXIT_CMD:
    case MSG_PRM_SCTP_EXIT_CMD:
    case MSG_PRM_TUCL_EXIT_CMD:
    case MSG_PRM_UEH_EXIT_CMD:
    case MSG_PRM_UDH_EXIT_CMD:
      return EVT_EXIT;

    case  MSG_BSM_S1M_OPSTOP_REQ:
      return EVT_BSM_S1M_OPSTOP_CMD;

    case  MSG_BSM_CEM_OPSTOP_REQ:
      return EVT_BSM_CEM_OPSTOP_CMD;

    case  MSG_BSM_UDH_OPSTOP_REQ:
      return EVT_BSM_UDH_OPSTOP_CMD;

    case  MSG_BSM_S1M_OPSTART_REQ:
      return EVT_BSM_S1M_OPSTART;

    case  MSG_BSM_CEM_OPSTART_REQ:
      return EVT_BSM_CEM_OPSTART;

    case  MSG_BSM_UDH_OPSTART_REQ:
      return EVT_BSM_UDH_OPSTART;

    case MSG_BSM_SM_SCTP_INIT_CONFIG_REQ:
      return EVT_BSM_SM_SCTP_CFG_REQ;

    case MSG_BSM_SM_EGTP_INIT_CONFIG_REQ:
      return EVT_BSM_SM_EGTP_CFG_REQ;

    case MSG_BSM_SM_S1AP_INIT_CONFIG_REQ:
      return EVT_BSM_SM_S1AP_CFG_REQ;

    case MSG_BSM_SM_SET_LOG_CFG:
      return EVT_BSM_SM_SET_LOG_CFG;

    case   MSG_BSM_UDH_CONFIG_REQ:
      return EVT_BSM_UDH_CFG_REQ;

    case MSG_BSM_S1M_CONFIG_REQ:
      return EVT_BSM_S1M_CFG_REQ;

    case MSG_BSM_CEM_ENB_CONFIG_REQ:
      return EVT_BSM_CEM_CFG_REQ;

    case MSG_BSM_UEH_GET_ACTIVE_UE_STATUS_REQ:
      return EVT_BSM_UEH_ACTIVE_UE_STATUS_REQ;

    case MSG_BSM_UDH_GET_ACTIVE_RAB_STATUS_REQ:
      return EVT_BSM_UDH_ACTIVE_RAB_STATUS_REQ;

    case MSG_BSM_UDH_GET_RAB_HISTORY_REQ:
    return EVT_BSM_UDH_RAB_HISTORY_REQ;

    case MSG_BSM_UEH_GET_UE_EVENT_HISTORY_REQ:
      return EVT_BSM_UEH_UE_EVENT_HISTORY_REQ;

    case MSG_BSM_UEH_UEEVENTHISTORYLOG_CONFIG_REQ:
      return EVT_BSM_UEH_UEEVENTHISTORYLOG_CONFIG_REQ;

    case MSG_LTH_SM_TRACE_START_REQ:
      return EVT_LTH_SM_TRC_START_REQ;

    case MSG_LTH_SM_TRACE_STOP_REQ:
      return EVT_LTH_SM_TRC_STOP_REQ;

    case MSG_PMH_SM_PDCP_GEN_STAT_REQ:
      return EVT_PMH_SM_PDCP_GEN_STAT_REQ;

    case MSG_PMH_SM_PDCP_UPLANE_STAT_REQ:
    	return EVT_PMH_SM_PDCP_UPLANE_STAT_REQ;

    case MSG_PMH_SM_SCTP_GEN_STAT_REQ:
        return EVT_PMH_SM_SCTP_GEN_STAT_REQ;

    case MSG_PMH_SM_EGTP_GEN_STAT_REQ:
        return EVT_PMH_SM_GTP_GEN_STAT_REQ;

    case MSG_PMH_SM_RLC_GEN_STAT_REQ:
        return EVT_PMH_SM_RLC_GEN_STAT_REQ;

    case MSG_PMH_SM_RRC_GEN_STAT_REQ:
      return EVT_PMH_SM_RRC_GEN_STAT_REQ;

    case MSG_PMH_SM_RRC_CELL_STAT_REQ:
        return EVT_PMH_SM_RRC_CELL_STAT_REQ;

    case MSG_PMH_SM_MAC_GEN_STAT_REQ:
      return EVT_PMH_SM_MAC_GEN_STAT_REQ;

    case MSG_PMH_SM_MAC_RLCSAP_STAT_REQ:
      return EVT_PMH_SM_MAC_RLCSAP_STAT_REQ;

    case MSG_PMH_SM_MAC_PHYSAP_STAT_REQ:
        return EVT_PMH_SM_MAC_PHYSAP_STAT_REQ;

    case MSG_PMH_SM_S1AP_GEN_STAT_REQ:
      return EVT_PMH_SM_S1AP_GEN_STAT_REQ;

    case MSG_PMH_SM_S1AP_PEER_NODE_STAT_REQ:
	return EVT_PMH_SM_S1AP_PEER_NODE_STAT_REQ;
    
    case MSG_BSM_UEH_MOD_STATE_REQ:
      return EVT_BSM_UEH_MOD_STATE_REQ;
    
    case MSG_BSM_UDH_MOD_STATE_REQ:
      return EVT_BSM_UDH_MOD_STATE_REQ;

	// RRM Changes
    case MSG_PMH_UEH_RRM_STAT_REQ:
    {
      return EVT_PMH_UEH_RRM_CELL_STAT_REQ;
    }
    case MSG_PMH_UEH_COUNTER_START_REQ:
      return EVT_PMH_UEH_COUNTER_START_REQ;
    case MSG_PMH_UEH_COUNTER_STOP_REQ:
      return EVT_PMH_UEH_COUNTER_STOP_REQ;
    
    case MSG_PMH_UDH_DSTATS_REQ:
      return EVT_PMH_UDH_DSTAT_REQ;

    //Done
    case MSG_PMH_TFU_DSTATS_CFG_REQ:
      return EVT_PMH_TFU_DSTATS_CFG_REQ;
    /* BS-1799 cell barring procedure*/
    case MSG_BSM_CEM_CELL_BAR_REQ:
      return EVT_BSM_CEM_CELL_BAR_REQ;

    case MSG_BSM_CEM_CELL_UNBAR_REQ:
      return EVT_BSM_CEM_CELL_UNBAR_REQ; 
    default:
      logError("sfiGetEventFromMsgId(): Unknown MsgId Received:%d", msgId);
      return 0;
  } /* End of Switch */
}

/*
 * This function returns PWAV Module Id from CCPU Entity Id
 */
ModuleId sfiGetModIdFromEntityId(
    Ent entityId
    )
{
  ModuleId modId;

  switch(entityId)
  {
    case ENTNH: modId = MODID_RRC;  break;
    case ENTSZ: modId = MODID_S1AP; break;
    case ENTSB: modId = MODID_SCTP; break;
    case ENTEG: modId = MODID_EGTP; break;
    case ENTHI: modId = MODID_TUCL; break;
    case ENTRG: modId = MODID_MAC;  break;
    case ENTKW: modId = MODID_RLC;  break;
    case ENTPJ: modId = MODID_PDCP; break;
    case ENTSM: modId = MODID_SM; break;
    case ENTTF: modId = MODID_TFU; break;   
 
      /* PWAV Modules */
    case ENTUEH: modId = MODID_UEH; break;
    case ENT_BSM: modId = MODID_BSM; break;
    case ENTUDH: modId = MODID_UDH; break;
    case ENTS1M: modId = MODID_S1M; break;
    case ENTCEM: modId = MODID_CEM; break;
    case ENT_PRM: modId = MODID_PRM; break;
    case ENT_PMH: modId = MODID_PMH; break;
    case ENT_LTH: modId = MODID_LTH; break;
    default:
      logError( "sfiGetModIdFromEntityId(): Unknown EntityId Received:%d", entityId );
      modId = MODID_NONE;
      break;
  } /* End of Switch */

  return modId;
}
/*
 * This function returns CCPU Entity Id from PWAV Module Id
 */
Ent sfiGetEntityFromModId(
    ModuleId modId
    )
{
  Ent ent;

  switch(modId)
  {
    case MODID_PRM: ent = ENT_PRM; break;
    case MODID_UEH: ent = ENTUEH; break;
    case MODID_S1M: ent = ENTUEH; break; /* Presently S1M and CEH are in UEH */
    case MODID_CEM: ent = ENTUEH; break;
    case MODID_UDH: ent = ENTUDH; break;
    case MODID_BSM: ent = ENT_BSM; break;
    case MODID_LTH: ent = ENT_LTH; break;
    case MODID_PMH: ent = ENT_PMH; break;

    case MODID_SM:   ent = ENTSM; break;
    case MODID_RRC:  ent = ENTNH; break;
    case MODID_S1AP: ent = ENTSZ; break;
    case MODID_SCTP: ent = ENTSB; break;
    case MODID_EGTP: ent = ENTEG; break;
    case MODID_TUCL: ent = ENTHI; break;
    case MODID_MAC:  ent = ENTRG; break;
    case MODID_RLC:  ent = ENTKW; break;
    case MODID_PDCP: ent = ENTPJ; break;
    case MODID_TFU: ent = ENTYS; break;

    default:
      logError( "sfiGetEntityFromModId(): Unknown ModuleId Received:%d", modId );
      ent = ENTNC;
      break;
  } /* End of Switch */

  return ent;
}
#if 0
/**
 * This function is invoked by CCPU Framework when a message is received at UEH socket ID from PWAV framework
 */
S16 sfiRecvFrmFrm(
    struct UProc *uProc /* unused */
  )
{
  S16                  retVal;
  CmInetAddr           remAddr;
  CmInetMemInfo        info;        /* buffer allocation info */
  Buffer               *mBuf;
  MsgLen               numBytes;    /* length to read */
  Pst                  sendPst;
  S16                  setFds = 0;
  SfiMsg               msg;
  SfiCb                *sfiCb = getSfiCb();

  info.pool = sfiCb->init.pool;
  info.region = sfiCb->init.region;

  cmMemset((U8 *)(&sendPst), 0, sizeof(Pst));
  /* call select() correctly, depending on mode,
  * When a sockFd is set then call recvMsg
  * Process the received message
  */
  retVal = cmInetSelect(&sfiCb->readFdSet, NULLP, NULLP, &setFds);
  if (setFds EQ 0)
  {
    return RFAILED;
  }

  numBytes=CM_INET_READ_ANY;
  retVal = cmInetRecvMsg(&sfiCb->sockFd, &remAddr, &info, &mBuf, &numBytes, CM_INET_NO_FLAG);

  if (retVal NOTEQ ROK)
  {
    logError("Message reception failed, retVal=%d", retVal);
    return RFAILED;
  }

  SRemPreMsgMult((Data*)&msg, (MsgLen) sizeof(SfiMsg), mBuf); /* Remove PWAV FRM Header */

  logMsgIn("Msg:%s, of len:%d from src:%d, to dst:%d", getMsgName(msg.msgId), msg.len, msg.src, msg.dest);

  sendPst.dstEnt       = sfiGetEntityFromModId(msg.dest);
  sendPst.dstProcId    = getProcIdByEntityId(sendPst.dstEnt);
  sendPst.srcEnt       = sfiGetEntityFromModId(msg.src);
  sendPst.srcProcId    = getProcIdByEntityId(sendPst.srcEnt);
  sendPst.dstInst      = 0;
  sendPst.srcInst      = 0;
  sendPst.event        = getEventFromMsgId(msg.msgId);
  if( MSG_UNDEFINED EQ sendPst.event )
  {
    logError("Error, No Mapping event Defined, Dropping the received message:%d",msg.msgId );
    return 0;
  }

  sendPst.pool         = info.pool;
  sendPst.region       = info.region;
  sendPst.selector     = SM_SELECTOR_LC;

  logInfo("Sending: Event:%s, from src:%d, to dst:%d", getEventName(sendPst.event), sendPst.srcEnt, sendPst.dstEnt );

  return(SPstTsk(&sendPst, mBuf));
}
#endif
U32 getProcIdByEntityId(
  Ent entityId
  )
{
  switch(entityId)
  {
  case ENTUEH: return SM_UEH_PROC;
  case ENT_BSM: return SM_BSM_PROC;
  case ENT_PRM: return SM_PRM_PROC;
  case ENT_PMH: return SM_PMH_PROC;
  case ENT_LTH: return SM_LTH_PROC;

  case ENTS1M: return SM_S1M_PROC;
  case ENTCEM: return SM_CEM_PROC;
  case ENTUDH: return SM_UDH_PROC;

  case ENTSM: return SM_SM_PROC;
  case ENTHI: return SM_HI_PROC;
  case ENTSB: return SM_SB_PROC;
  case ENTSZ: return SM_SZ_PROC;
  case ENTEG: return SM_EG_PROC;
  case ENTRG: return SM_RG_PROC;
  case ENTPJ: return SM_PJ_PROC;
  case ENTKW: return SM_KW_PROC;
  case ENTNH: return SM_NH_PROC;
  case ENTYS: return SM_YS_PROC;
  default:
    logError("unknown entitiyId=%d, no proc info provided", entityId);
    return 0;
  }
}

/**
 * This function sends the message from CCPU SSI to PWAV FRM
 * The flow is as under:
 * 1. TAPA-Task sends mBuf to SFI
 * 2. SFI removes the 'inst' field as PWAV-FRM does not use it.
 *    This is done using cmUnpkInst(&inst, mBuf)
 * 3. SFI then adds FRM Message Header to top of the received mBuf.
 *    In other words, it creates a PWAV-FRM message inside mBuf.
 *    This is done because the socket-send function from CCPU expects mBuf as input.
 * 4. SFI sends it to the PWAV module using cmInetSendMsg(mBuf(containing frmMsg))
 */
#if 0
S16 sfiSendToFrm(
  IN   Pst       *pst,
  FR   Buffer    *mBuf
  )
{
  MsgLen         bytesSent;  /* stores the length of the message posted */
  CmInetMemInfo  info;       /* memory pool/region info*/
  CmInetAddr     destAddr;   /* remote relay addr/port */
  SfiMsg         frmMsg;
  SfiCb         *sfiCb = getSfiCb();
  U8             inst;
  S16            retVal;
  Buffer        *tmpBuf;

  if (SGetMsg(pst->region, pst->pool, &tmpBuf) NOTEQ ROK)
  {
    logError("%s", "Error in allocating Message");
  }

  logInfo("Received Event:%s, from src:%d, to dst:%d", getEventName(pst->event), pst->srcEnt, pst->dstEnt );
  cmUnpkInst(&inst, mBuf);

  /* mBuf when it was received: inst, dataByte1, dataByte2, ...
   * mBuf after cmUnpkInst: dataByte1, dataByte2, ...
   * mBuf to be sent to frm: msgId, src, dest, via, len, dataByte1, dataByte2, ...
   */
  frmMsg.msgId = getMsgIdBySrcEntityAndEventId(pst->srcEnt, pst->event);
  frmMsg.src   = sfiGetModIdFromEntityId(pst->srcEnt);
  frmMsg.dest  = sfiGetModIdFromEntityId(pst->dstEnt);
  frmMsg.via   = 0; /*Unused*/
  SFndLenMsg(mBuf, &(bytesSent) ); /* CCPU Message Length is 4 bytes, our FRM length is 2 Bytes */
  frmMsg.len   = bytesSent;

  logError("Message received in sfi [%s]", getMsgName(frmMsg.msgId));

//		  MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP

  /* frmMsg.data:
   * The data bytes are already inside the mBuf
   * frmMsg.data is an array of 0 bytes.
   * So sizeof(frmMsg) is same as sizeof (msgId, src, dest, via, len).
   * Hence below, we pass sizeof(frmMsg) as the number of bytes to be added to top of mBuf.
   *   SAddPreMsgMult((Data*)&data, sizeof(SfiMsg), mBuf); don't work
   *   So we create a temporary message and do the change
   */
  SAddPstMsgMult((Data *)&frmMsg, sizeof(SfiMsg), tmpBuf);
  SCatMsg(tmpBuf, mBuf, M1M2);

  /* SFI has modified the received mBuf.
   * Hence region and pool information should be from the original sender, not SFI.
   */
  info.region = pst->region;
  info.pool   = pst->pool;

  destAddr    = sfiCb->frmModAddrByModId[frmMsg.dest];

  retVal = cmInetSendMsg(&sfiCb->sockFd, &destAddr, &info, tmpBuf, &bytesSent, CM_INET_NO_FLAG);
  if (retVal NOTEQ ROK)
  {
    logError("Failed to send Msg:%s to port:%d, ipAddress:%s", getMsgName(frmMsg.msgId), destAddr.port,inet_ntoa( ntohl(destAddr.address)));
    SFI_FREEMBUF(mBuf);
    SFI_FREEMBUF(tmpBuf);
    RETVALUE(RFAILED);
  }

  logMsgOut("Msg:%s sent to port:%d, ipAddress:%s of Size:%d ", getMsgName(frmMsg.msgId), destAddr.port, inet_ntoa(ntohl(destAddr.address)), bytesSent );

  /* After sending the message to PWAV framework, free the message buffer */
  SFI_FREEMBUF(mBuf);
  SFI_FREEMBUF(tmpBuf);
  RETVALUE(ROK);
}
#endif
#if 0
/**
 * This function initialises the socket interface towards PWAV Framework and registers driver task
*/
S16 sfiInit(void)
{
  S16                ret;
  CmInetAddr         uehAddr;    /* local relay addr/port */
  SfiCb             *sfiCb = getSfiCb();

  logInfo("%s", "sfiInit(): Initializing IPC");

  /* Initialise the Mapping from CCPU Events to PWAV MessageID
  * Read the imc.cfg file,
  * Create a socket, bind it to the address of UEH
  */
  sfiInitCtx(sfiCb);

  ret = cmInetSocket(CM_INET_DGRAM, &sfiCb->sockFd, 0);
  if (ret NOTEQ ROK)
  {
    logError("%s", "sfiInit(): listening server create socket failure");
    RETVALUE(RFAILED);
  }

  uehAddr = sfiCb->frmModAddrByModId[MODID_UEH];
  ret = cmInetBind( &sfiCb->sockFd, &uehAddr);
  if (ret NOTEQ ROK)
  {
    logError("sfiInit(): listening server bind failure on port=%d", uehAddr.port );
    RETVALUE(RFAILED);
  }

  /* set the server fd in the read fd set --*/
  CM_INET_FD_SET( &sfiCb->sockFd, &sfiCb->readFdSet);
  logInfo("sfiInit(): Server Opened successfully at port (%u)", uehAddr.port );

  /* Register the driver task */
  ret = SRegDrvrTsk(    1 /* channel instance, not used*/,
                        OAM_PROC_ID /* low */,
                        OAM_PROC_ID /* high */,
                        sfiSendToFrm,
                (ISTsk) sfiRecvFrmFrm);

  SSetIntPend((U16)1, TRUE);
  if (ret NOTEQ ROK)
  {
    logError("sfiInit(): Driver Task registration Failed, ret=%d", ret);
    RETVALUE(RFAILED);
  }

  logInfo("%s", "sfiInit(): SFI INIT Done");

  /* Now send start query to all the modules,
   * Modules will respond by sending MSG_XXX_PRM_START_IND
   */
  sfiSendStartQueryToAllModules();

  RETVALUE(ROK);
}
#endif

