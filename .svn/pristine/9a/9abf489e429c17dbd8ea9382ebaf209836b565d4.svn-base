/**
 * @file    ipc.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @details
 * There are 2 types of message structures:
 * a) FRM Messages - these messages go between 2 modules using libfrm
 * b) SSI Messages - these are the ones exchanged between CCPU Modules
 *                 - When 2 CCPU modules, also called TAPA Tasks, are in different executables, SSI messages are "relayed".
 *                 - This relaying is done by the module and thus it is called SSR (SSI Relay)
 * Provides IPC using UDP
 * communication between 3 modules cp.exe , gtp.exe and radioup.exe is done using this IPC.
 *
 * @author Manu , Ritesh
 *
 */

#include "pst_events.h"
#include "libconfig.h"
#include "log.h"
#include "cmn_types.h"     /* FileName128, ImcMethod */
#include "ccpu_cmn.h"
#include "ueh.h"
#include "libconfig.h"
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm_lib.x"
#include "ipc.h"
#include "imc_unix.h"
#include "imc_udp.h"
#include "sfi.h"
#include "errno.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "bs_config.h"
#include "sendToOam.h"
#include <sys/ioctl.h>
#include "mt_4gmx.x"


//#define MAX_PORTS 3

//#define CONFIG_FILE_LENGTH       100

#define SOCK_FLAG_NOBLOCK         4

#define IPC_FREEMBUF(_mBuf)\
  do{\
      if (_mBuf != NULLP) {\
        (Void)SPutMsg((_mBuf));\
        (_mBuf) = NULLP;\
      }\
  }while(0)

#if 0
/** Framework Message Structure
  * NOTE: Duplicated from libfrm so that those files dont have to be included.
  * Inclusion of libfrm can conflict with SSI.
  */

typedef struct s_FrmMsg /* Duplicated from frm */
{
  U32    msgId;
  U32    src;
  U32    dest;
  U32    via;
  U16    len; /* len of data in bytes */
  /* user is only required to fill data */
  U8    data[0]; /* ignore the compilation warning */
} __attribute__((__packed__)) FrmMsg; /*Need to pack it*/

#endif

/** The following types of communication can happen:.
 *
 *  a) FRM-FRM
 *      The FRM-FRM interface works directly over UDP and UNIX SOCK.
 *
 *  b) SSI-SSI
 *      SSI-SSI across executables is NOT SUPPORTED. This is done using SSI-Relay.
 *      The relay itself goes over UDP or UNIX SOCK.

 *  c) FRM-SSI
 *      FRM-SSI require a conversion layer.
 *      This is done in SM at present.
 *      SM maps a FRM-Message to a CCPU-Event and vice-versa.
 *
 */
  






/** Globals **/
SfiCb          g_sfiCb;
ModIpcCtx      g_modIpcCtx;

/**  ******** Functions **/

/**
 * @details This function returns the global context of a process
 */
ModIpcCtx *getModuleCtx()
{
  return (&g_modIpcCtx);
}
/**
 * @param fd  FD of the Interface
 * @return Is Interface is Configured or not *
 */

int isInterfaceConfigured(int fd)
{
   if(fd EQ 0)
      return 0;
   else
      return 1;
}

/**
 * @param       procId                self processor Id
 * @details
 * gets module entity from self processor ID
 */
S16 getModEntityfrmseflProc(S16 procId)
{
	switch (procId)
	{
	case CP_PROC_ID: return MODID_UEH;
	case GTP_PROC_ID: return MODID_UDH;
	case RADIOUP_PROC_ID: return MODID_RLC;
	default:
          logError( "%s: Unknown procId Received:%d", __FUNCTION__, procId );
           return MODID_NONE;
	}
}

/**
 * @param       entity          Module Entity
 *
 */
TBOOL isFrmTypeEntity(Ent entity)
{
	switch (entity)
	{
      case ENT_BSM:
      case ENT_PRM:
      case ENT_PMH:
      case ENT_LTH:
    	  return TRUE;
      default:
    	  return FALSE;
	}
}


/**
 * @param       entity          Module Entity
 * @details
 * logs the event
 */
 void logEvent(char *printMe, Pst *pst, MsgLen len)
{
    logMsgOut("event=%d src=%d dst=%d dstProcId=%d len=%d", pst->event, pst->srcEnt, pst->dstEnt, pst->dstProcId, len); 
}

/**
 * @param       printMe         string passed on
 * @param       pst             Post structure
 * @param       mBuf            CCPU buffer structure
 * @details
 * logs the particular event and posts the message to SSI
 *
 */
S16 postMsg(char *printMe, Pst *pst, Buffer *mBuf)
{
  S16 msgLen;

  SFndLenMsg(mBuf, &(msgLen));

  if (ROK == SPstTsk(pst, mBuf))
  {
	logEvent(printMe, pst, msgLen);
	return ROK;
  }
  logError("%s", "SPstTsk() failed. MSG DROPPED");
  logEvent("Could not send", pst, msgLen);
  IPC_FREEMBUF(mBuf);
  return RFAILED;
}

/**
 * @param       cfg         config Structure
 * @details
 * This function opens imc.cfg and fills the context in cfg *
 * TODO: Remove from here - there should be a common function used by ALL layers.
 */
void openLibConfigFile(
  IN U8        *fileNameWithoutPath, 
  IO config_t  *cfg)
{
  char *rootDir = msArgv[2];
  FileName      filename;
  int           numWritten;

  if ( rootDir EQ NULL )
  {
    fprintf( stderr, "Configuration ERROR: msArgv[2] is NULL. This could be because rootDir is not set! Exiting.");
    logError("%s", "Configuration ERROR: msArgv[2] is NULL. This could be because rootDir is not set! Exiting.");
    exit(EXIT_FAILURE);
  }

  numWritten = snprintf((S8 *) filename, sizeof(filename), "%s/config/%s", rootDir, fileNameWithoutPath );
  if (!(numWritten < sizeof(filename)))
  {
    fprintf( stderr, "File Read ERROR: FILE NAME IS TOO LONG. Could not read %s as %s/config/%s exceeds max-file-name length=%d. Exiting",
          filename, rootDir, filename, sizeof(filename));
    logError( "File Read ERROR: FILE NAME IS TOO LONG. Could not read %s as %s/config/%s exceeds max-file-name-length=%d. Exiting",
          fileNameWithoutPath, rootDir, fileNameWithoutPath, sizeof(filename));
    exit(EXIT_FAILURE);
  }

  config_init(cfg);
  logInfo("Reading %s",filename);
  if(! config_read_file(cfg, (const S8 *) filename))
  {
    fprintf(stderr, "%s:%d - %s\n",
        config_error_file(cfg),
        config_error_line(cfg),
        config_error_text(cfg));

    logError("%s failed to read %s. Exiting", __FUNCTION__, fileNameWithoutPath);
    config_destroy(cfg);
    exit(EXIT_FAILURE);
  }
  /* otherwise return with cfg set to a handle */
}

/**
 * @param       udpIpcInfoTable         structure consisting IP address, udpPort and procId
 * @details
 * This function Reads the config file, and initialises udp Ipc Context
 */
void readProcConfig(
                ItfIpcCtx      ipcProcInfoTable[]
  )
{
  config_t          cfg;
  config_setting_t  *setting;
  U16               count;
  U16               i=0;
  config_setting_t  *table;
  const char        *processName, *unixSockFilePath;
  S8                *ipAddress;
  S16                imcMethod;
  S16                procId;
  int                ssrPort = 0;
  U8                 numConfigured = 0;

  U8               *configFile = (U8 *)"imc.cfg";

  /* Read the IPAddress and Ports of SSI & FRM Modules */

  openLibConfigFile(configFile, &cfg);
  setting = config_lookup(&cfg, "ssr_cfg_table");
  if(setting != NULL)
  {
    count = config_setting_length(setting);
    numConfigured = 0;
    for (i=0; i< count; i++)
    {
      table = config_setting_get_elem(setting, i);
      config_setting_lookup_int    (table,  "procId",            (int *)&procId );
      config_setting_lookup_string (table,  "processName",       &processName );
      config_setting_lookup_int    (table,  "imcMethod",         (int *)&imcMethod );
      config_setting_lookup_string (table,  "unixSockFilePath",  &unixSockFilePath);
      config_setting_lookup_string (table,  "ipAddress",         (const S8 **)&ipAddress);
      config_setting_lookup_int    (table,  "ssrPort",           (int *)&ssrPort);

      if ((procId >= MIN_PROC_ID) && (procId <= MAX_PROC_ID))
      {
    	  cmInetAddr( ipAddress, &(ipcProcInfoTable[procId].udpIpcCfg.ipAddr) );
    	  ipcProcInfoTable[procId].udpIpcCfg.ipAddr = CM_INET_HTON_U32(ipcProcInfoTable[procId].udpIpcCfg.ipAddr);
    	  ipcProcInfoTable[procId].udpIpcCfg.udpPort  = ssrPort;
    	  ipcProcInfoTable[procId].imcMethod = imcMethod;
    	  strncpy((S8 *)(ipcProcInfoTable[procId].unixSockCfg.unixSockFilePath), unixSockFilePath, UNIX_SOCK_PATH_MAX);
    	  numConfigured++;
      }
      else
      {
    	  logError("Unexpected procId %d in %s. Exiting", procId, configFile);
    	  exit(EXIT_FAILURE);
      }
    } /* for */
  } /* if */
  config_destroy(&cfg);
}


/**
 * @param       frmModAddrByModId         structure consisting ip address and port
 * @details
 * This function reads the imc config file and copies ipaddress and port into its know context
 */

/** TODO: Move this to a common place - there should be just one function; it should be used by all */
void readIpcImcCfg(
                  ItfIpcCtx ipcFrmInfoTable[]
  )
{
  config_t          cfg;
  config_setting_t *setting;
  U16               count;
  U16               i=0;
  config_setting_t *table;
  const char       *moduleName, *unixSockFilePath;
  S8               *ipAddress;
  S16               imcMethod;
  ModuleId          modId;
  int               udpPort = 0;
  U8               *configFile = (U8 *)"imc.cfg";
    
    /* Read the IPAddress and Ports of FRM Modules */
  openLibConfigFile(configFile, &cfg);
  setting = config_lookup(&cfg, "module_cfg_table"); /* TODO: imc_cfg_table */
  if(setting != NULL)
  {
    count = config_setting_length(setting);
    for (i=0; i< count; i++)
    {
      table = config_setting_get_elem(setting, i);
      config_setting_lookup_string(table, "moduleName", &moduleName );
      config_setting_lookup_int   (table, "imcMethod",  (int *)&imcMethod  );
      config_setting_lookup_string (table,"unixSockFilePath",  &unixSockFilePath);
      config_setting_lookup_string(table, "ipAddress", (const S8 **)&ipAddress);
      config_setting_lookup_int(table,    "udpPort", (int *)&udpPort);

      for(modId = 0 ; modId < MODID_MAX ; modId++)
      {
        if(strcmp((const S8 *)getModuleName(modId), moduleName) EQ 0)
        {
          ipcFrmInfoTable[modId].imcMethod  = imcMethod;
          strncpy( (S8 *)&(ipcFrmInfoTable[modId].unixSockCfg.unixSockFilePath), \
                  unixSockFilePath, UNIX_SOCK_PATH_MAX);
          ipcFrmInfoTable[modId].udpIpcCfg.udpPort  = udpPort;
          cmInetAddr( ipAddress, &(ipcFrmInfoTable[modId].udpIpcCfg.ipAddr) );
          ipcFrmInfoTable[modId].udpIpcCfg.ipAddr = CM_INET_HTON_U32(ipcFrmInfoTable[modId].udpIpcCfg.ipAddr);
          break;
        }
      }
    }
  } /* if */
  config_destroy(&cfg);
}


/**
 * @param       fd         File Descriptor
 * @details
 * This function Converts flat buffer into mBuf format
 */

Buffer * recvIntoSSIMsgBuffer(int fd)
{

  char      *localbuf      = NULLP;
  char       smallBuf[80];
  S16        ret           = 0;
  int        numBytesRcvd  = 0;
  Buffer    *mBuf          = NULL;
  ModIpcCtx *modCtx        = getModuleCtx();
  TBOOL      isAllocated   = FALSE;

  ioctl(fd, FIONREAD, &numBytesRcvd);

  if (numBytesRcvd <= 0)
  {
    return NULLP;
  }

  if (numBytesRcvd < 80) localbuf = &smallBuf[0];
  else 
  {
    UEH_ALLOC(&localbuf, numBytesRcvd);
    isAllocated = TRUE;
  }

  numBytesRcvd = recv(fd, localbuf, numBytesRcvd, SOCK_FLAG_NOBLOCK);
  
  if (numBytesRcvd <= 0)
  {
    logError("ioctl said %d bytes in buff but recv() failed errno=%d", numBytesRcvd, errno);
    goto error;
  }

  UEH_ALLOCMBUF(modCtx->memInfo.region, modCtx->memInfo.pool, &mBuf);
  if (!mBuf)
  {
    logError("SGetMsg() failed. MSG rx on fd=%d of len=%d DROPPED (reg=%d, pool=%d)", fd, numBytesRcvd, modCtx->memInfo.region, modCtx->memInfo.pool);
    goto error;
  }
  else
  {
    ret = SAddPstMsgMult(localbuf, numBytesRcvd, mBuf);
  }
  if (isAllocated) UEH_FREE(localbuf, numBytesRcvd);
  return mBuf;

  error:
  if(isAllocated) UEH_FREE(localbuf, numBytesRcvd);
  return NULL;
}


/**
 * @details
 * This function receives messages of SSR_ITF_TYPE and FRM_INT_TYPE on fds, converts them to PST and posts to SSI.
 */
void recvIpcMsgFromOutsideAndPostWithin(void)
{
    /* call the handler */
    Buffer     *mBuf;
    Pst         sendPst;
    ModIpcCtx  *modCtx    = getModuleCtx();
    U16         ssrRecvFd = modCtx->selfIpcCtx[SSR_ITF_TYPE].recvFd;
    U16         frmRecvFd = modCtx->selfIpcCtx[FRM_ITF_TYPE].recvFd;
    fd_set      readFdSet;

    /* Is any of the fds set? */
    {
       struct timeval tv = { .tv_sec = 0, .tv_usec = 3000 };
       FD_ZERO(&readFdSet);
       FD_SET(ssrRecvFd, &readFdSet);
       FD_SET(frmRecvFd, &readFdSet);
       if(select (20, &readFdSet, NULL, NULL, &tv) <= 0) return;
    }

    if(FD_ISSET(ssrRecvFd, &readFdSet)) //ssrReisInterfaceConfigured(ssrRecvFd))
    {
       while((mBuf = recvIntoSSIMsgBuffer(ssrRecvFd)) != NULL)
       {
          SRemPreMsgMult((Data*)&sendPst,  sizeof(Pst), mBuf); /* Cuts out Pst and send mBuf */

          postMsg("SSR", &sendPst, mBuf);

       }
    }

    if(FD_ISSET(frmRecvFd, &readFdSet))//isInterfaceConfigured(frmRecvFd))
    {
       FrmMsg frmMsg;
       while((mBuf = recvIntoSSIMsgBuffer(frmRecvFd)) != NULL)
       {

          memset(&sendPst, 0, sizeof(Pst));

          SRemPreMsgMult((Data*)&frmMsg, (MsgLen) sizeof(FrmMsg), mBuf); /* Remove PWAV FRM Header */
          logMsgIn("FRM | %s | len=%d | src=%d | dst=%d", getMsgName(frmMsg.msgId), frmMsg.len, frmMsg.src, frmMsg.dest);
          sendPst.dstEnt       = sfiGetEntityFromModId(frmMsg.dest);
          sendPst.dstProcId    = getProcIdByEntityId(sendPst.dstEnt);
          sendPst.srcEnt       = sfiGetEntityFromModId(frmMsg.src);
          sendPst.srcProcId    = getProcIdByEntityId(sendPst.srcEnt);
          sendPst.dstInst      = 0; /* TODO: Why hard-code */
          sendPst.srcInst      = 0;
          sendPst.event        = getEventFromMsgId(frmMsg.msgId);
          sendPst.pool         = modCtx->memInfo.pool;
          sendPst.region       = modCtx->memInfo.region;
          sendPst.selector     = SM_SELECTOR_LC;

          postMsg(NULL, &sendPst, mBuf);
       }
    }
}

/**
 * @param 
 * @details
 * This function sends mBuf to peer through udp socket/unix socket etc..
 */

S16 ipcSend(
  Pst       *pst,
  Buffer    *mBuf
)
{
  U8             *sendBuf = NULLP;
  ItfIpcCtx      *destAddr;   /* remote relay addr/port */
  U8             inst;
  S16            retVal    = ROK;
  FrmMsg         frmMsg;     /* FRM Message to be filled */
  MsgLen         len, numCopied, hdrLen;
  S32            numSent;
  struct in_addr inAddr;
  ModIpcCtx     *modCtx    = getModuleCtx();
  int            sendFd;


  cmUnpkInst(&inst, mBuf); // This will reduce mBuf len by sizeof(inst)
  SFndLenMsg(mBuf, &len);

  if (isFrmTypeEntity(pst->dstEnt))
  {
    /**
    *  mBuf when it was received : inst,      dataByte1, dataByte2, ...
    *  mBuf after cmUnpkInst     : dataByte1, dataByte2, ...
    *  mBuf to be sent to frm    : msgId,     src,       dest, via, len, dataByte1, dataByte2, ...
    */

    frmMsg.msgId = getMsgIdBySrcEntityAndEventId(pst->srcEnt, pst->event);
    frmMsg.src   = sfiGetModIdFromEntityId(pst->srcEnt);
    frmMsg.dest  = sfiGetModIdFromEntityId(pst->dstEnt);
    frmMsg.via   = 0; /*Unused*/
    frmMsg.len   = len;

    /**
    * frmMsg.data:
    *        The data bytes are already inside the mBuf
    *        frmMsg.data is an array of 0 bytes.
    *        So sizeof(frmMsg) is same as sizeof (msgId, src, dest, via, len).
    */

    hdrLen = sizeof(FrmMsg);
    UEH_ALLOC(&sendBuf, hdrLen + len);
    if(!sendBuf) {
      logError("mem alloc failed. requested=%d bytes", (hdrLen + len));
      IPC_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
    }
    memcpy(sendBuf, &frmMsg, hdrLen);
    if(len > 0) SCpyMsgFix(mBuf, 0, len, sendBuf+hdrLen, &numCopied);

    destAddr = &(modCtx->frmModAddrByModId[frmMsg.dest]);
    sendFd   = modCtx->selfIpcCtx[FRM_ITF_TYPE].sendFd;
  }
  else
  {
    /* createSsrMsg() */
    hdrLen = sizeof(Pst);
    UEH_ALLOC(&sendBuf, hdrLen + len);
    //sendBuf = (U8 *)malloc(hdrLen + len);
    if(!sendBuf) {
      logError("alloc failed. requested=%d bytes", hdrLen + len);
      IPC_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
    }
    memcpy(sendBuf, pst, hdrLen);
    if(len > 0) SCpyMsgFix(mBuf, 0, len, sendBuf+hdrLen, &numCopied);

    destAddr = &(modCtx->peerAddressByProcessorId[pst->dstProcId]);
    sendFd   = modCtx->selfIpcCtx[SSR_ITF_TYPE].sendFd;
  }


  if (destAddr->imcMethod EQ UNIX_SOCK)
  {
    numSent = sendUnixSockMsg(sendFd,
                              destAddr->unixSockCfg.unixSockFilePath,
                              sendBuf,
                              hdrLen + len);
    if ((pst->event > PWAV_START_EVENT) && (pst->event < EVT_EGTP_LTH_TRC_IND)) /* To minimize Unknown events logging */
       logMsgOut("Sent UNIXSOCK fd=%d DstPort=%d event=%d src=%d dst=%d dstProcId=%d bytes=%d", sendFd, destAddr->udpIpcCfg.udpPort, pst->event, pst->srcEnt, pst->dstEnt, pst->dstProcId, numSent);
  }
  else if (destAddr->imcMethod EQ UDP_SOCK)
  {
    inAddr.s_addr = ntohl(destAddr->udpIpcCfg.ipAddr);
    numSent = sendUdpMsg(sendFd,
                         (U8 *)inet_ntoa(inAddr),
                         destAddr->udpIpcCfg.udpPort,
                         sendBuf,
                         hdrLen + len);

    if ((pst->event > PWAV_START_EVENT) && (pst->event < EVT_EGTP_LTH_TRC_IND)) /* To minimize Unknown events logging */
       logMsgOut("Sent UDP fd=%d DstPort=%d event=%d src=%d dst=%d dstProcId=%d bytes=%d", sendFd, destAddr->udpIpcCfg.udpPort, pst->event, pst->srcEnt, pst->dstEnt, pst->dstProcId, numSent);
  }
  else
  {
    logError("Send failed, event=%d src=%d dst=%d dstProcId=%d len=%d", pst->event, pst->srcEnt, pst->dstEnt, pst->dstProcId, len);
  }

  retVal = ROK;

  /* After sending the message to PROCESS, free the message buffer */
  UEH_FREE(sendBuf, hdrLen + len);
  IPC_FREEMBUF(mBuf);
  RETVALUE(retVal);
}

/**
 * @param       sockFd         socket Fd structure
 * @param       addr	       Ipaddress and port
 * @param       readFdSet      Read Fd set
 * @details
 * This function opens the socket and binds with the corresponding port
 * Set the socket for the corresponding Fd set
 */
S16 openAndBindUdpSockFrm(int *sockSendFd, int *sockRecvFd, CmInetAddr *addr, CmInetFdSet *readFdSet)
{
  logInfo("Opening Sender UDP Socket on Port = %d", addr->port);
  if(FALSE EQ openUdpSocket(addr->port + 1, TRUE /*FALSE*/, sockSendFd))
  {
    RETVALUE(RFAILED);
  }
  logInfo("Opening Receiver UDP Socket on Port = %d", addr->port);
  if (FALSE EQ openUdpSocket(addr->port, TRUE, sockRecvFd))
  {
    RETVALUE(RFAILED);
  }
  FD_SET(*sockRecvFd, readFdSet);
  RETVALUE(ROK);
}

S16 openAndBindUnixSockFrm(int *sockSendFd, int *sockRecvFd, S8 *unixSockFilePath , CmInetFdSet *readFdSet)
{
  if(FALSE EQ openUnixSocket((U8 *)unixSockFilePath, FALSE, sockSendFd))
  {
    RETVALUE(RFAILED);
  }
  logInfo("Opening UNIX Domain Socket on Port = %s", unixSockFilePath );
  if (FALSE EQ openUnixSocket((U8 *)unixSockFilePath, TRUE, sockRecvFd))
  {
    RETVALUE(RFAILED);
  }
  FD_SET(*sockRecvFd, readFdSet);
  RETVALUE(ROK);
}



/**
 * @param       selfProcId     Self Processor ID
 * @param       lowProcId	   Lowest Range Processor ID
 * @param       highProcId     Highest Range Processor ID
 * @details
 * It opens the processor config and stores the data into its own context
 * This function initialises the socket interface towards PEER PROCESS and registers driver task
 */
S16 ipcInit(
		S16 selfProcId,
		S16 lowProcId,
		S16 highProcId)

{
  CmInetAddr        procAddr;    /* local relay addr/port */
  U8                Entity;
  ModIpcCtx        *modCtx = getModuleCtx();

  clearBytes(modCtx, sizeof(ModIpcCtx));
  clearBytes(&g_sfiCb, sizeof(SfiCb));

  modCtx->selectTimeout = 2; /* ms */
  logInfo("%s Initializing on selfProcessorId = %d", __FUNCTION__, selfProcId);
  memset(&(modCtx->peerAddressByProcessorId), 0, sizeof(modCtx->peerAddressByProcessorId));
  readProcConfig(modCtx->peerAddressByProcessorId);
  readIpcImcCfg(modCtx->frmModAddrByModId);
  sfiInitMsgIdByEventAndSrcEntity(&g_sfiCb);
  Entity = getModEntityfrmseflProc(selfProcId);
  
  logInfo("Entity = %d FilePath = %s", Entity, modCtx->peerAddressByProcessorId[Entity].unixSockCfg.unixSockFilePath);

  /* Fill selfInfo for SSR Interface: modCtx->peerAddressByProcessorId[selfProcId]; */

  modCtx->selfIpcCtx[SSR_ITF_TYPE].imcMethod         = modCtx->peerAddressByProcessorId[selfProcId].imcMethod;
  modCtx->selfIpcCtx[SSR_ITF_TYPE].udpIpcCfg.ipAddr  = modCtx->peerAddressByProcessorId[selfProcId].udpIpcCfg.ipAddr;
  modCtx->selfIpcCtx[SSR_ITF_TYPE].udpIpcCfg.udpPort = modCtx->peerAddressByProcessorId[selfProcId].udpIpcCfg.udpPort;
  strncpy((S8 *)(modCtx->selfIpcCtx[SSR_ITF_TYPE].unixSockCfg.unixSockFilePath),
          (const S8 *)(modCtx->peerAddressByProcessorId[selfProcId].unixSockCfg.unixSockFilePath),
          UNIX_SOCK_PATH_MAX);

  /* Fill selfInfo for FRM Interface: modCtx->frmModAddrByModId[Entity]; */

  modCtx->selfIpcCtx[FRM_ITF_TYPE].imcMethod         = modCtx->frmModAddrByModId[Entity].imcMethod;
  modCtx->selfIpcCtx[FRM_ITF_TYPE].udpIpcCfg.ipAddr  = modCtx->frmModAddrByModId[Entity].udpIpcCfg.ipAddr;
  modCtx->selfIpcCtx[FRM_ITF_TYPE].udpIpcCfg.udpPort = modCtx->frmModAddrByModId[Entity].udpIpcCfg.udpPort;
  strncpy((S8 *)(modCtx->selfIpcCtx[FRM_ITF_TYPE].unixSockCfg.unixSockFilePath),
          (const S8 *)(modCtx->frmModAddrByModId[Entity].unixSockCfg.unixSockFilePath),
          UNIX_SOCK_PATH_MAX);

  /* create socket for the process [SSR Interface] */
  if (modCtx->selfIpcCtx[SSR_ITF_TYPE].imcMethod EQ UDP_SOCK &&
      modCtx->selfIpcCtx[SSR_ITF_TYPE].udpIpcCfg.udpPort NOTEQ 0)
  {
    procAddr.port     = modCtx->selfIpcCtx[SSR_ITF_TYPE].udpIpcCfg.udpPort;
    procAddr.address  = modCtx->selfIpcCtx[SSR_ITF_TYPE].udpIpcCfg.ipAddr;
    if(ROK EQ openAndBindUdpSockFrm(&(modCtx->selfIpcCtx[SSR_ITF_TYPE].sendFd),
                                    &(modCtx->selfIpcCtx[SSR_ITF_TYPE].recvFd),
                                    &procAddr,
                                    &(modCtx->readFdSet)
                                   ))
    {
        logInfo("%s SSR Port %d OK",__FUNCTION__, procAddr.port);
    }
    else
    {
        RETVALUE(RFAILED);
    }
  }
  else if (modCtx->selfIpcCtx[SSR_ITF_TYPE].imcMethod EQ UNIX_SOCK &&
           strlen((const S8 *)modCtx->selfIpcCtx[SSR_ITF_TYPE].unixSockCfg.unixSockFilePath))
  {

     if(ROK EQ openAndBindUnixSockFrm(&(modCtx->selfIpcCtx[SSR_ITF_TYPE].sendFd),
                                     &(modCtx->selfIpcCtx[SSR_ITF_TYPE].recvFd),
                                     (S8 *)&(modCtx->selfIpcCtx[SSR_ITF_TYPE].unixSockCfg.unixSockFilePath[0]),
                                     &(modCtx->readFdSet)
                                     ))
     {
        logInfo("%s SSR File %s  OK",__FUNCTION__, modCtx->selfIpcCtx[SSR_ITF_TYPE].unixSockCfg.unixSockFilePath);
     }
     else
     {
         RETVALUE(RFAILED);
     }
  }

  /* create socket for the process [FRM Interface] */
  if (modCtx->selfIpcCtx[FRM_ITF_TYPE].imcMethod EQ UDP_SOCK &&
      modCtx->selfIpcCtx[FRM_ITF_TYPE].udpIpcCfg.udpPort NOTEQ 0)
  {
    procAddr.port     = modCtx->selfIpcCtx[FRM_ITF_TYPE].udpIpcCfg.udpPort;
    procAddr.address  = modCtx->selfIpcCtx[FRM_ITF_TYPE].udpIpcCfg.ipAddr;
    if(ROK EQ openAndBindUdpSockFrm(&(modCtx->selfIpcCtx[FRM_ITF_TYPE].sendFd),
                                    &(modCtx->selfIpcCtx[FRM_ITF_TYPE].recvFd),
                                    &procAddr,
                                    &(modCtx->readFdSet)
                                   ))
    {
        logInfo("%s FRM Port %d OK",__FUNCTION__, procAddr.port);
    }
    else
    {
        RETVALUE(RFAILED);
    }
  }
  else if (modCtx->selfIpcCtx[FRM_ITF_TYPE].imcMethod EQ UNIX_SOCK &&
           strlen((const S8 *)(modCtx->selfIpcCtx[FRM_ITF_TYPE].unixSockCfg.unixSockFilePath)))
  {

     if(ROK EQ openAndBindUnixSockFrm(&(modCtx->selfIpcCtx[FRM_ITF_TYPE].sendFd),
                                     &(modCtx->selfIpcCtx[FRM_ITF_TYPE].recvFd),
                                     (S8 *)&(modCtx->selfIpcCtx[FRM_ITF_TYPE].unixSockCfg.unixSockFilePath[0]),
                                     &(modCtx->readFdSet)
                                     ))
     {
        logInfo("%s SSR File %s  OK",__FUNCTION__, modCtx->selfIpcCtx[FRM_ITF_TYPE].unixSockCfg.unixSockFilePath);
     }
     else
     {
       RETVALUE(RFAILED);
     }
  }
  logInfo("%s: Driver Init Done on selfProcId=%d", __FUNCTION__, selfProcId);
  RETVALUE(ROK);
}

/**
 * @param       driverInst     Driver Channel Instance
 * @param       destProcId     Destination Processor ID
 * @details
 * Registers the particular driver task
 */
S16 regDriverTask(S16 driverInst,S16 destProcId)
{
  /* Register the driver task */
  S16 ret = SRegDrvrTsk(
		    driverInst,
		    destProcId,  /* low */
	            destProcId,  /* high */
          (ActvTsk) ipcSend,
            (ISTsk) recvIpcMsgFromOutsideAndPostWithin);

  if (ret != ROK)
  {
    logError("%s Driver Task %d for dstProc %d registration Failed, ret=%d", 
              __FUNCTION__, driverInst, destProcId, ret);
    RETVALUE(RFAILED);
  }

  SSetIntPend((U16) driverInst, TRUE);
  logInfo("%s: Driver Instance %d registration for destProcId=%d OK", __FUNCTION__, driverInst, destProcId);
  RETVALUE(ROK);
}
#ifndef PWAV_L2SIM 
#ifdef SS_4GMX_UCORE
/**
 * @param       driverInst     Driver Channel Instance
 * @param       destProcId     Destination Processor ID
 * @details
 * Registers the particular driver task
 */
S16 regDriverLwrArmTask(S16 driverInst,S16 destProcId)
{
  /* Register the driver task */
  S16 ret = SRegDrvrTsk(driverInst,
                         destProcId,  /* low */
                         destProcId,  /* high */
                         uarmPstTsk,
                         (ISTsk) uarmIsTsk);

  if (ret != ROK)
  {
    logError("%s Driver Task %d for dstProc %d registration Failed, ret=%d",
              __FUNCTION__, driverInst, destProcId, ret);
    RETVALUE(RFAILED);
  }

  //SSetIntPend((U16) driverInst, TRUE);
  logInfo("%s: Driver Instance %d registration for destProcId=%d OK", __FUNCTION__, driverInst, destProcId);
  RETVALUE(ROK);
}
#endif /* SS_4GMX_UCORE */
#endif  
