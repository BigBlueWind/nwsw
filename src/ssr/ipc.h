/**
 * @file    ipc.h
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @details
 *          This file contains ipc function prototypes
 *
 * @author Manu Sharma
 * @author Girish Vakati
 *
 */

#ifndef IPC_H
#define IPC_H

/** There are 2 types of message structures:
 *
 *  a) FRM Messages
 *      These Messages Go between 2 modules using libfrm
 *
 *  b) SSI Messages -
 *      These are the Messages exchanged between CCPU Modules
 *
 * ImcItfType - This type indicates the type of interface, and thus the structure of messages expected on that interface.
 * ImcMethod  - This Type indicates the type of method available for transport of messages across executables.
 *
 * NOTE: Interface Type (ImcItfType) and Message Transport Method (ImcMethod) are independent.
 */ 
typedef enum e_ImcItfType{ 
  SSR_ITF_TYPE = 0, 
  FRM_ITF_TYPE,
  MAX_ITF_TYPE
} ImcItfType;

/**
 * NOTE: In principle there can be multiple interfaces of a 'type'.
 *       For example, in principle, there can be 2 interfaces of FRM_ITF_TYPE in a module.
 *       However, till date this need has not arisen.
 *       Hence, MAX_INTERFACES is same as MAX_INTERFACE_TYPES.
 *       If this assumption changes any day, make appropriate changes here.
 */

#define MAX_INTERFACES MAX_ITF_TYPE

/** User Defined parameters of a UDP SOCKET */
typedef struct s_UdpIpcCfg
{
  CmInetIpAddr    ipAddr;
  U16             udpPort;
} UdpIpcCfg;

/** User Defined parameters of a UNIX SOCKET */
typedef struct s_UnixSockCfg
{
  FileName     unixSockFilePath;
} UnixSockCfg;



/** Stores the IPC Context of an Interface.
  * An interface can use only one IMC Method.
  * However, 'union' has NOT been used as they are more difficult to debug using GDB.
  */
typedef struct s_ItfIpcCtx
{
  ImcMethod     imcMethod; /** If UNIX_SOCK then unixSockCfg is applicable; if UDP_SOCK, then udpIpcCfg is applicable */
  UdpIpcCfg     udpIpcCfg;
  UnixSockCfg   unixSockCfg;
  int           sendFd;
  int           recvFd;
} ItfIpcCtx;

/** Stores the IPC Context of a MODULE.
  * A module has multiple interfaces.
  */

typedef struct s_ModIpcCtx
{
  CmInetMemInfo       memInfo;       /* Used by CCPU Modules */
  U32                 dbgMask;       /* Used by CCPU Modules */
  CmInetFdSet         readFdSet;     /* Used for select() */
  U32                 selectTimeout;

  /** How am I reachable:
   *  selfIpcCtx[SSR_ITF_TYPE] contains the context for listening to SSR messages.   
   *  selfIpcCtx[FRM_ITF_TYPE] contains the context for listening to FRM messages.   
   */
   ItfIpcCtx selfIpcCtx[MAX_INTERFACES]; 

  /** Destination info -- How to reach peers/others ?
   *    There are 2 types of peers:
   *            CCPU TAPA Tasks and
   *            Framework-based modules
   *
   *    In case of CCPU TAPA Tasks, SSR is used.
   *       SSR-entity in 'sender' (called SSR-src), sends to SSR-entity in 'destination' processor (called SSR-dst).
   *
   *    The flow is as under:
   *        SSI: SPstTsk(pst, buffer)
   *           if (isOnDifferentProc(pst->dst))
   *              task = findDriverTask(pst->dst->dstProcId)
   *              task->sendFunction(pst, buffer);
   *        DriverTask:
   *           if(pst->dstEntId is CCPU_TYPE_ENT)
   *              sendUsingSsr(pst, buffer);
   *           else if (pst->dstEntId is FRM_TYPE_ENT)
   *              sendUsingFrm(pst, buffer);
   *
   *  SSR-src finds the destination address based on pst->dstProcId, (not dstEntId, as it is sending to SSR-dst).
   *  SSR-dst receives the incoming message as does SPstTsk() as usual.
   *  uses dst->procId to find the address of the peer.
   *  For every processorId, the 
   *  In case of FRM, the method of sending is determined by modId
   */

  ItfIpcCtx peerAddressByProcessorId[MAX_PROC_ID]; /* peers */
  ItfIpcCtx frmModAddrByModId[MODID_MAX];

} ModIpcCtx;


ModIpcCtx *getModuleCtx(void);
int isInterfaceConfigured(int fd);
S16 getModEntityfrmseflProc(S16 procId);
TBOOL isFrmTypeEntity(Ent entity);
void logEvent(char *printMe, Pst *pst, MsgLen len);
S16 postMsg(char *printMe, Pst *pst, Buffer *mBuf);

void openLibConfigFile( IN U8  *fileNameWithoutPath, IO config_t  *cfg);
void readProcConfig(ItfIpcCtx      ipcProcInfoTable[] );
void readIpcImcCfg( ItfIpcCtx ipcFrmInfoTable[] );

Buffer * recvIntoSSIMsgBuffer(int fd);
void recvIpcMsgFromOutsideAndPostWithin(void);

S16 ipcSend( Pst  *pst, Buffer *mBuf);
S16 openAndBindUdpSockFrm(int *sockSendFd, int *sockRecvFd, CmInetAddr *addr, CmInetFdSet *readFdSet);
S16 openAndBindUnixSockFrm(int *sockSendFd, int *sockRecvFd, S8 *unixSockFilePath , CmInetFdSet *readFdSet);
S16 ipcInit( S16 selfProcId, S16 lowProcId, S16 highProcId);


S16 regDriverTask(S16 driverInst, S16 destProcId);
S16 regDriverLwrArmTask(S16 driverInst,S16 destProcId);
S16 IpcInit(
                S16 selfProcId,
                S16 lowProcId,
                S16 highProcId);
S16 regDriverLwrArmTask(S16 driverInst,S16 destProcId);
#endif

