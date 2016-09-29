//-------------------------------------------------------------------------------------------
/** @file supervisor.h
 *
 * @brief The declaration of supervisor's APIs and types
 * @author Mindspeed Technologies
 * @version $Revision: 1.58 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _SUPERVISOR_H_
#define _SUPERVISOR_H_

#include "4gmx.h"
#include "intercpu.h"
#include "appids.h"
#include "apimgr.h"
#include "larmmsgid.h"
#include "svsrlogger.h"
/******************************************************************************
*                The list of module error codes                               *
*******************************************************************************/

#define RC_SVSR_OK              MX_OK
#define RC_SVSR_NOT_MEMORY      DEF_USRRC(APP_MODULE, SUPERVISOR, 1)
#define RC_SVSR_NOT_INIT        DEF_USRRC(APP_MODULE, SUPERVISOR, 2)
#define RC_SVSR_ALLOC_ERROR     DEF_USRRC(APP_MODULE, SUPERVISOR, 3)
#define RC_SVSR_INSTANCE_ERROR  DEF_USRRC(APP_MODULE, SUPERVISOR, 4)
#define RC_SVSR_API_PROTO_ERROR DEF_USRRC(APP_MODULE, SUPERVISOR, 5)
#define RC_SVSR_DONOT_SEND_RESP DEF_USRRC(APP_MODULE, SUPERVISOR, 6)
#define RC_SVSR_API_DONOT_SUPP  DEF_USRRC(APP_MODULE, SUPERVISOR, 7)
#define RC_SVSR_UNKNOWN_INST    DEF_USRRC(APP_MODULE, SUPERVISOR, 8)
#define RC_SVSR_UNKNOWN_API_VER DEF_USRRC(APP_MODULE, SUPERVISOR, 9)
#define RC_SVSR_ALREADY_USED    DEF_USRRC(APP_MODULE, SUPERVISOR, 10)
#define RC_SVSR_UNREG_TYPE      DEF_USRRC(APP_MODULE, SUPERVISOR, 11)
#define RC_SVSR_PARAM_ERROR     DEF_USRRC(APP_MODULE, SUPERVISOR, 12)
#define RC_SVSR_THR_NUM_ERR     DEF_USRRC(APP_MODULE, SUPERVISOR, 14)
#define RC_SVSR_EVN_NUM_ERR     DEF_USRRC(APP_MODULE, SUPERVISOR, 15)
#define RC_SVSR_TMR_NUM_ERR     DEF_USRRC(APP_MODULE, SUPERVISOR, 16)
#define RC_SVSR_LST_NUM_ERR     DEF_USRRC(APP_MODULE, SUPERVISOR, 17)
#define RC_SVSR_QUEUE_NUM_ERR   DEF_USRRC(APP_MODULE, SUPERVISOR, 18)
#define RC_SVSR_PART_NUM_ERR    DEF_USRRC(APP_MODULE, SUPERVISOR, 19)
#define RC_SVSR_HEAP_NUM_ERR    DEF_USRRC(APP_MODULE, SUPERVISOR, 20)
#define RC_SVSR_RHEAP_NUM_ERR   DEF_USRRC(APP_MODULE, SUPERVISOR, 21)
#define RC_SVSR_QUEUE_SIZE_ERR  DEF_USRRC(APP_MODULE, SUPERVISOR, 22)
#define RC_SVSR_HEAP_SIZE_ERR   DEF_USRRC(APP_MODULE, SUPERVISOR, 23)
#define RC_SVSR_RHEAP_SIZE_ERR  DEF_USRRC(APP_MODULE, SUPERVISOR, 24)
#define RC_SVSR_PART_SIZE_ERR   DEF_USRRC(APP_MODULE, SUPERVISOR, 25)
#define RC_SVSR_INDEX_ERR       DEF_USRRC(APP_MODULE, SUPERVISOR, 26)
#define RC_SVSR_COREID_ERR      DEF_USRRC(APP_MODULE, SUPERVISOR, 27)
#define RC_SVSR_ICORE_OVR       DEF_USRRC(APP_MODULE, SUPERVISOR, 28)
#define RC_SVSR_CPUID_ERROR     DEF_USRRC(APP_MODULE, SUPERVISOR, 29)
#define RC_SVSR_SCTP_W_ERR      DEF_USRRC(APP_MODULE, SUPERVISOR, 30)
#define RC_SVSR_SCTP_R_ERR      DEF_USRRC(APP_MODULE, SUPERVISOR, 31)
#define RC_SVSR_ICPU_W_ERR      DEF_USRRC(APP_MODULE, SUPERVISOR, 32)
#define RC_SVSR_ICPU_R_ERR      DEF_USRRC(APP_MODULE, SUPERVISOR, 33)
#define RC_SVSR_IS_NOT_IMP      DEF_USRRC(APP_MODULE, SUPERVISOR, 34)
#define RC_SVSR_SIZE_ERROR      DEF_USRRC(APP_MODULE, SUPERVISOR, 35)
#define RC_SVSR_ICORE_ALLOC_ERR DEF_USRRC(APP_MODULE, SUPERVISOR, 36)
#define RC_SVSR_ICORE_MAKE_ERR  DEF_USRRC(APP_MODULE, SUPERVISOR, 37)
#define RC_SVSR_ICORE_SHARE_ERR DEF_USRRC(APP_MODULE, SUPERVISOR, 38)
#define RC_SVSR_NOT_ICORE_MEM   DEF_USRRC(APP_MODULE, SUPERVISOR, 39)
#define RC_SVSR_TRACE_ID_ERROR  DEF_USRRC(APP_MODULE, SUPERVISOR, 40)

#define SVSR_CLIENT_NUM                     (SVSR_INST_NUM)  // The maximum number of API command handlers

#define SVSR_ICORE_NUM                      (4)
#define SVSR_MSG_NUM                        (120)
#define SVSR_MSG_PART_SMALL_BLOCK_SIZE      (256)
#define SVSR_MSG_PART_MIDDLE_BLOCK_SIZE     (512)
#define SVSR_MSG_PART_BIG_BLOCK_SIZE        (7*1024)    // Mainly designed to contain PHY cached messages
#define SVSR_MSG_PART_BLOCK_SIZE            (30*1024)
#define SVSR_MSG_PART_BLOCK_NUM             (SVSR_MSG_NUM)

#define SVSR_RESP_INST_ERROR                1

#define SVSR_INVALID_INSTANCE               ((UINT16)0x0)
#define SVSR_INSTANCE_ID                    ((UINT16)0x1)
#define PHYSTUB_INSTANCE_ID                 ((UINT16)0x17)

#define MAKE_LARM_INST(x)                   ((x) | (0x8000))
#define GET_LARM_INST(x)                    ((x) & ~(0x8000))

#define UARM_INSTID_SUPERVISOR              1
#define UARM_INSTID_HOST                    2
#define UARM_INSTID_DEFMAC                  3
#define UARM_INSTID_DEF_MSMAC               4
#define UARM_INSTID_DEF_UNMM               10

#define IS_API(api, function, type)         ((api)->MessageID == (function) && (api)->Type == (type))
#define IS_CFG_API(api, function)           ((api)->MessageID == (function) && (api)->Type == (API_TYPE_CFG_PARAM))
#define IS_CFG_RESP_API(api, function)      ((api)->MessageID == (function) && (api)->Type == (API_TYPE_CFG_RESP))
#define IS_QRY_API(api, function)           ((api)->MessageID == (function) && (api)->Type == (API_TYPE_QRY_PARAM))
#define IS_QRY_RESP_API(api, function)      ((api)->MessageID == (function) && (api)->Type == (API_TYPE_QRY_RESP))

#define SVSR_MSG_OPT_DEFAULT                (0 << 0)
#define SVSR_MSG_OPT_DEALLOC                (1 << 0)
#define SVSR_MSG_OPT_SAVE                   (1 << 1)
#define SVSR_MSG_OPT_HIGH_PROC              (1 << 2)    // The destination handler thread will be putted at the beginning of scheduled list
/******************************************************************************
*                The list API parameters                                      *
*******************************************************************************/
// SVSR_QUERY_INST_ID command

#define SVSR_PID_MOD_TYPE                   2
#define SVSR_PID_MOD_INDEX                  3
#define SVSR_PID_MOD_INST_ID                4

#define SVSR_DUMP_RX_API                    (1 << 0)
#define SVSR_DUMP_TX_API                    (1 << 1)

#define SVSR_LOGGER_RX                      (1 << 0)
#define SVSR_LOGGER_TX                      (1 << 1)


#define SVSR_I_CPU_TX                       (1)
#define SVSR_I_CPU_RX                       (2)

#define SVSR_SCTP_DEFHOST                   "192.168.0.1"
#define SVSR_SCTP_DEFPORT                   32

/** @brief The set of API handler error codes

    APIHEC - API handler error code */

typedef enum _APIHEC_
{
    APIHEC_OK       = 0,                // The API command was processed correctly and the
                                        // supervisor should send response for processed API
                                        // command (the response is prepared by the API
                                        // handler )

    APIHEC_DROP_RESP= 1,                // The response should be dropped. The API handler
                                        // will allocate response and send response later if
                                        // it be required

    APIHEC_KEEP_API_DROP_RESP = 2,      // The I-CPU command will be deallocated by API handler
                                        // and response will be dropped

}APIHEC;

typedef enum _COMM_TYPE_
{
    COMM_TYPE_ICPU = 0,                 //I CPU communication type, now used as default
    COMM_TYPE_DIRECT_SCTP = 1           //Direct sctp transport type
}COMM_TYPE;

/** @brief The type of API command handler */

typedef APIHEC (*APIHandler) (LPVOID pClientCtx,  // The handle context
                            ApiHeader * pApi,   // Received API command
                            ApiHeader * pResp); // The response for API command

typedef MXRC (*MsgHandler) (LPVOID pClientCtx,  // The handle context
                            LPVOID pMsg);       // The message


typedef MXRC (*ApiLoggerProc) (LPVOID pClientCtx, ApiHeader * pApi);

typedef struct _API_HANDLER_CTX_
{
    LPVOID              ClientCtx;  // The handler context
    APIHandler          Handler;    // The API command handler
    UINT32              ModTypeID;  // see: DrvModIDs type of appids.h file
    ApiHeader *         AsmApiCmd;  // The pointer to currently assembled API command
    UINT32              AsmOffs;    // The buffer offset used to build commands
    UINT32              CpuID;      // The API command handler CPU ID
    ApiLoggerProc       TxApi;      // The TX API logger, it can be null, if not null, this function is called to log trace
    ApiLoggerProc       RxApi;      // The RX API logger, it can be null, if not null, this function is called to log trace

}ApiHandlerCtx;


typedef struct _MSG_HANDLER_CTX_
{
    LPVOID              ClientCtx;  // The handler context
    MsgHandler          Handler;    // The MSG command handler
    ApiLoggerProc       TxApi;      // The TX API logger, it can be null, if not null, this function is called to log trace
    ApiLoggerProc       RxApi;      // The RX API logger, it can be null, if not null, this function is called to log trace

}MsgHandlerCtx;

typedef struct _MSG_HANDLER_DATA_
{
    struct _MSG_HANDLER_DATA_ * Next;
    LPVOID MsgPtr;
    UINT16 MsgID;
    UINT16 MsgOpt;
    UINT32 TimeStamp;
    UINT32 Prior;
    
}MsgHandlerData;

/** @brief The critical system parameters used by the upper layers */

typedef struct _SPRV_SYSINFO_
{
    UINT32  nMaxCommBlockSize;  // The maximum size of communication block
    UINT32  nMaxApiParamSize;   // The maximum size of API (in bytes)

}SvsrSysInfo;


typedef struct _SVSR_API_STAT_
{
    UINT32      RxMsgNum;   // The number of received API commands for this core
    UINT32      TxMsgNum;   // The number of sent API commands for this core 
    UINT32      RxTime;     // The time of the latest received API command 
    UINT32      TxTime;     // The time of the latest sent API command 
    UINT32      RxErrNum;   // The number of message received with error (unknown API, etc)

    ApiHeader   RxICpuApi;  // The last received API command 
    ApiHeader   TxICpuApi;  // The last transmitted API command 

}SvsrApiStat;

typedef struct _SVSR_MSG_STAT_
{
    UINT32      RxMsgNum;   // The number of received message  for this core
    UINT32      TxMsgNum;   // The number of sent message for this core 
}SvsrMsgStat;

/** @brief The data context of the supervisor manager */

typedef struct _SVSR_CTX_
{
    ICChannelID    CommChannelID;  // The communication channel ID
    ICChannelCtx * CommChannel;    // The communication channel context
    UINT32         CommBlockSize;  // The communication block data size
    UINT32         ApiWayTest;     // The unique ID used in API route testing

    ApiHandlerCtx  clients [SVSR_CLIENT_NUM]; // Supervisor clients contexts
    MsgHandlerCtx  msghdlr [SVSR_CLIENT_NUM]; // Supervisor message/signal handlers

    // ========= I-CORE communication ====
    HANDLE         hMsgHandler;    // Message handler
    MsgHandlerData MsgStorage [SVSR_MSG_NUM];
    MsgHandlerData*MsgFirst;
    MsgHandlerData*MsgLast;
    SYSFPART       MsgPart;
    VUINT32        MsgSync;
    // ===================================

    // ========= I-SIGNAL communication====
    HANDLE         hSigHandler;    // Signal handler
    MsgHandlerData SigStorage [SVSR_MSG_NUM];
    MsgHandlerData*SigFirst;
    MsgHandlerData*SigLast;
    SYSFPART       SigPart;
    VUINT32        SigSync;
    // ===================================

    PUINT32        IMsgQStorage;// Queue storage
    UINT32         IMsgPut;
    UINT32         IMsgGet;
    UINT32         IMsgSync;
    UINT32         IMsgCount;

    HANDLE         IMsgH;       // The message handler
    UINT32         CurMsgTs;    // Current message timestamp
    UINT32         MsgProcFlags;

    SvsrApiStat    ICpuStat;    // I-CPU statistics 
    SvsrMsgStat    ICoreStat;   // I-Core statistics     

}SupervisorCtx, *PSupervisorCtx;

typedef struct _ICORE_PART_CFG_
{
    UINT32  Cached;     // Flag: 1 - cached, 0 - non-cached
    UINT32  BlockNum;   // The block number for this partitoons
    UINT32  BlockSize;  // The size of block in bytes
    LPVOID  StorPtr;    // The pointer to the storage
    HANDLE  PartHandle; // The handle of partition

}ICORE_PART_CFG;

UINT32 SvsrGetCommBlockSize (void);
#ifdef PWAV
extern SupervisorCtx SvsrCtx [CPU_NUM];
#endif
SvsrSysInfo SvsrGetSysInfo (void);

/** @brif The function initializes a supervisor manager and opens inter-cpu
          communication channel used for interaction with upper ARM.

    @param id      [in] - the ID of communication channel (see:ICChannelID type)
    @return [MXRC] error code */

MXRC SvsrInit (void);

/** @brief  The function sends to the upper-arm the supervisor
            ready message. This message contains error code of system initialization

    @param ErrorCode [in] - the error code of system initialization

    @return [MXRC] operation error code */

MXRC SvsrSendReady (UINT32 ErrorCode);

/** @brief The function registers new API handler on supervisor layer. The
           API commands with specified instance ID will be routed to API handler

    @param nInstID    [in] - the API handler instance ID
    @param hand       [in] - the pointer to the function of API commands handler
    @param pClientData[in] - the pointer to the handler context data


    @return [MXRC] error code */

MXRC SvsrRegClient (UINT32 nInstID, APIHandler hand, LPVOID pClientData);

/** @brief This function removes API handler specified by the instance ID. The
           API commands sent to this instance ID will be rejected and host be
           notified of response with supervisor error code.

    @return nInstID [in] - the Instance ID allocated by calling SvsrRegClient

    @return [MXRC] error code */

MXRC SvsrRemClient (UINT32 nInstID);

/** @brief This function sets logger function for the specified message handler
           if logger function is not NULL pointer, this function will be called to
           log the TX or RX API command.

    @param nInstID    [in] - the instance handler ID
    @param nCtrl      [in] - contrl mask, see: SVSR_LOGGER_xxx
    @param logger     [in] - the pointer to the logger function, it can be null to stop logging

    @return [MXRC] an error code */

MXRC SvsrSetClientLogger (UINT32 nInstID, UINT32 nCtrl, ApiLoggerProc logger);

/** @brief This function sends API command to the upper ARM by using inter-cpu
           communication channel. If API command length is more than size of
           supported block, the command will be automatically divided to the
           smaller blocks and be sent to the host by using protocol of sending
           large commands.

    @param pApi     [in] - the pointer to the API commamnd data

    @return [MXRC] error code of operation */

MXRC SvsrSendAPI (ApiHeader * pApi);

MXRC SvsrSendAPIEx (ApiHeader * pApi, BOOL notify);


/** @brief This function sends API command to the upper ARM by using inter-cpu
           communication channel.  The function is used to send linked list API only!

    @param pApi     [in] - the pointer to the API commamnd data

    @return [MXRC] error code of operation */

MXRC SvsrSendAPILinkedList (ApiHeader * pApi);

/** @brief This function sends API command to the upper ARM with HI-priority 
           by using inter-cpu communication channel. 
           It's possible to use this API only in case of extended I-CPU 

    @param pApi     [in] - the pointer to the API commamnd data

    @return [MXRC] error code of operation */

MXRC SvsrSendHiAPI (ApiHeader * pApi);

/** @brief The function returns pointer to the client context specified by the
           instance ID.

    @param nInstID [in] - the instance ID

    @return [ApiHandlerCtx*] the context or NULL*/

ApiHandlerCtx * SvsrGetClientCtx (UINT32 nInstID);


/** @brief This function sends a large data block to the host as extended API
           command with a protocol of sending extended API commands by dividing
           it to the set of fragments

    @param nInstID   [in] - the instance ID
    @param nMessID   [in] - the message ID
    @param nType     [in] - command type
    @param pData     [in] - the pointer to the API command data
    @param nDataSize [in] - the size of command payload in bytes

    @return [MXRC] error code of operation */

MXRC SvsrSendExtApi (UINT16 nDstInstID, UINT16 nSrcInstID, UINT16 nMessID,
                    UINT16 nType, LPVOID pData, UINT32 nDataSize);


/** @brief This function is designed to store current size of data objects (heaps, partitions, queues ... )
 *         and should be called first before usage of SvsrCheckMemObjs
 *  */

MXRC SvsrStoreSysObjs (void);

/** @brief This function is designed to check current size of data objects (heaps, partitions, queues ... )
 *         with prestored values (see: SvsrStoreMemObjs)
 *
 *  @param nSkipICpu [in] - if 1, the function takes account of API commands (API and response) located in I-CPU space, 0 - will not
 *  @param pResp [out] - the pointer to the response, pepares with this function to be sent to the host with
 *                        detailed information about problem
 *
 *  @return MXRC
 *
 * */

MXRC SvsrCheckSysObjs (UINT32 nSkipICpu, ApiHeader * pResp);

/** @brief This function sets dump mask and enables/disables dumping of TX/RX API commands
    @param nMask [in] see SVSR_DUMP_xxx macro

    @return [MXRC] */
MXRC SvsrSetDumpMask (UINT32 nMask);

/** @brief This function is designed to free API command located in I-CPU storage
           and should be used for this API commands only

    @param pApi [in] - the pointer to the API command located in I-CPU storage

    @return [MXRC] an error code */

#ifdef SYS_TRACER_ENABLED
    MXRC SvsrFreeApi (ApiHeader * pApi);
    MXRC SvsrFreeApiLR (ApiHeader * pApi, UINT32 lr);
#else
    MXRC SvsrFreeApi (ApiHeader * pApi);
#endif

/** @brief The function allocates new one block in I-CPU partition
           and returns a pointer to this block, this data can be used
           with functions: SvsrFreeTxApi or SvsrSendTxApi to free or to send data
           in case of sending, the destination point is responsible to free data

    @return  [ApiHeader *] the pointer to the API command or NULL */

#ifdef SYS_TRACER_ENABLED
    ApiHeader * SvsrAllocICApi(void);
    ApiHeader * SvsrAllocICApiLR(UINT32 lr);
#else
    ApiHeader * SvsrAllocICApi(void);
#endif

/** @brief This function allocates memory in the I-CPU partition specified 
            with ID (RX I-CPU or TX I-CPU)

    @param  type[in] - the type of I-CPU memory, see: SVSR_I_CPU_RX or SVSR_I_CPU_TX

    @return [ApiHeader *] the pointer to the memory casted like API header */

ApiHeader * SvsrAllocICApiEx(UINT32 type);


/** @brief The function frees allocated in I-CPU partition block
           It can be used only for the data allocated with function SvsrAllocTxApi

    @return  [MXRC] an error code */

MXRC SvsrFreeICApi (ApiHeader * pApi);

/** @brief The function sends data, if used ICPU, it can be used only for the data
           allocated with function SvsrAllocTxApi ONLY!!!

    @return  [MXRC] an error code */

MXRC SvsrSendICApi (ApiHeader * pApi);
MXRC SvsrSendICApiEx (ApiHeader * pApi, BOOL notify);


/** @brief The function sends data, if used ICPU, it can be used only for the data
           allocated with function SvsrAllocTxApi ONLY!!!
           Don't modify sending API.

    @return  [MXRC] an error code
    */

MXRC SvsrSendApiSimpy (ApiHeader * pApi);

/** @brief This function registers message handler

    @param nMsgID       [in] - message handler ID
    @param hdlr         [in] - handler
    @param pHandleData  [in] - handler data

    @return [MXRC] an error code */

MXRC SvsrRegMsgHandler (UINT32 nMsgID, MsgHandler hdlr, LPVOID pHandleData);

/** @brief This function registers message handler on the remote core

    @param nCoreID      [in] - the core ID
    @param nMsgID       [in] - message ID
    @param hdlr         [in] - handler
    @param pHandleData  [in] - handler data

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrRegMsgHandlerEx (UINT32 nCoreID, UINT32 nMsgID, MsgHandler hdlr, LPVOID pHandleData);

/** @brief This function return message handler
           registered for this message ID

    @param nMsgID [in] - the message ID
    @param pObj   [out]- the pointer to the message handler structure

    @return [MsgHandlerCtx] message handler */

MXRC SvsrGetMsgHandler (UINT32 nMsgID, MsgHandlerCtx * pObj);

/** @brief This function installs CORE message handler

    @param nMsgID [in] - the message ID
    @param pObj [in] - the pointer to the handler context

    @return [MXRC] error code */

MXRC SvsrSetMsgHandler (UINT32 nMsgID, MsgHandlerCtx * pObj);


/** @brief This function sends message to the
           remote core supervisor

    @param nCoreID [in] - the core ID [0 ... CPU_NUMS)
    @param nID     [in] - massage handler ID
    @param pMsg    [in] - the pointer to the message
    @param opt     [in] - message option (automatic deallocation, etc), see: SVSR_MSG_OPT_xxx

    @NOTE:
         Sender and receiver are responsible to allocate and free
         memory, used like a message storage, also it can be just a pointer
         to a global buffer

    @return [MXRC] an error code */

MXRC SvsrSendMsg (UINT32 nCoreID, UINT32 nID, LPVOID pMsg, UINT32 opt);

/** @brief This function sends signal to the
           remote core supervisor

    @param nCoreID [in] - the core ID [0 ... CPU_NUMS)
    @param nID     [in] - massage handler ID
    @param pMsg    [in] - the pointer to the message
    @param opt     [in] - message option (automatic deallocation, etc), see: SVSR_MSG_OPT_xxx

    @NOTE:
         Sender and receiver are responsible to allocate and free
         memory, used like a message storage, also it can be just a pointer
         to a global buffer

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrSendSignal (UINT32 nCoreID, UINT32 nID, LPVOID pMsg, UINT32 opt);

/** @brief This function sends message to the
           remote core supervisor

    @param nCoreID [in] - the core ID [0 ... CPU_NUMS)
    @param nID     [in] - massage handler ID
    @param pMsg    [in] - the pointer to the message
    @param MsgPrior[in] - the message priority, 0 - highest priority
    @param opt     [in] - message option (automatic deallocation, etc), see: SVSR_MSG_OPT_xxx

    @NOTE:
         Sender and receiver are responsible to allocate and free
         memory, used like a message storage, also it can be just a pointer
         to a global buffer

    @return [MXRC] an error code */

MXRC SvsrSendMsgEx (UINT32 nCoreID, UINT32 nID, LPVOID pMsg, UINT32 MsgPrior, UINT32 opt);


/** @brief This function is designed to process I-CORE messages in polling mode
           and mainly designed to be used in blocking thread(s)

    @return [MXRC] an error code */

MXRC SvsrProcICoreMsg (void);

/** @brief This function initializes partition used for I-COREs communication

    @param nElmNum    [in] - the number of blocks in partition
    @param nBlockSize [in] - the block size

    @NOTE:  This function should be called from CORE-0 ONLY!!!

    @return [MXRC ] an error code */

MXRC SvsrInitMsg (UINT32 nElmNum, UINT32 nBlockSize);

/** @brief This function initializes partition used for I-SIGNALS communication

    @param nElmNum    [in] - the number of blocks in partition
    @param nBlockSize [in] - the block size

    @NOTE:  This function should be called from CORE-0 ONLY!!!

    @return [MXRC ] an error code
    \ingroup group_lte_services
    */

MXRC SvsrInitSig (UINT32 nElmNum, UINT32 nBlockSize);

/** @brief This function allocates partition block in cacheable memory
           and returns pointers to this block or NULL.

    @NOTE: If block was used with SvsrSendMsg function, it should not be deallocated
           the system will deallocate it automatically

    @return [ApiHeader *] the pointer to an allocated block */

#ifdef SYS_TRACER_ENABLED
    ApiHeader * SvsrAllocMsg (void);
    ApiHeader * SvsrAllocMsgLR (UINT32 lr);
#else
    ApiHeader * SvsrAllocMsg (void);
#endif

#ifdef SYS_TRACER_ENABLED    
    ApiHeader * SvsrAllocMsgEx (UINT32 size);
    ApiHeader * SvsrAllocMsgExLR (UINT32 size, UINT32 lr);
#else
    ApiHeader * SvsrAllocMsgEx (UINT32 size);
#endif

/** @brief This function frees block manually, it should be used
           only in case when block was not send to remote core

    @return [MXRC] an error code */

MXRC SvsrFreeMsg (LPVOID pMsg);

/** @brief This function frees block manually from any msg partition, it should be used
           only in case when block was not send to remote core

    @return [MXRC] an error code */

#ifdef SYS_TRACER_ENABLED
    MXRC SvsrFreeMsgEx (LPVOID pMsg);
    MXRC SvsrFreeMsgExLR (LPVOID pMsg, UINT32 lr);
#else
    MXRC SvsrFreeMsgEx (LPVOID pMsg);
#endif

/** @brief This function sets logger function for the specified message handler
           if logger function is not NULL pointer, this function will be called to
           log the TX or RX API command.

    @param nMsgHdlrID [in] - the message handler ID
    @param nCtrl      [in] - contrl mask, see: SVSR_LOGGER_xxx
    @param logger     [in] - the pointer to the logger function, it can be null to stop logging

    @return [MXRC] an error code */

MXRC SvsrSetMsgLogger (UINT32 nMsgHdlrID, UINT32 nCtrl, ApiLoggerProc logger);

/** @brief This function return the timestamp for the current message,
            this function should be called from message handler, in
            other case timestamp is meaningless

    @return [UINT32] the message timestamp (when message was putted to the I-CORE queue) */

UINT32 SvsrGetMsgTimestamp (void);

/** @brief This function is designed to automatically find out what 
           the communication isnterface is used (I-CORE or I-CPU) and to call 
           interface function to free already allocated data 

    @param pData [in] - the pointer to the block of memory 

    @return [UINT32] an error code */
           
#ifdef SYS_TRACER_ENABLED
    MXRC SvsrFreeIMem (LPVOID pData);
    MXRC SvsrFreeIMemLR (LPVOID pData, UINT32 lr);
#else
    MXRC SvsrFreeIMem (LPVOID pData);
#endif

/** @brief This function returns a number of I-CORE blocks that 
    can be allocated by the application at this moment

    @return [UINT32] the number of available I-CORE blocks*/
    
UINT32 SvsrGetICoreNum (void);

/** @brief This function returns a number of I-CORE blocks that
    can be allocated by the application at this moment

    @param block_size[in] - the size of block in bytes

    @return [UINT32] the number of available I-CORE blocks
    \ingroup group_lte_services
    */

UINT32 SvsrGetICoreNumEx (UINT32 block_size);

/** @brief This function puts MSPD I-CPU commands to the RX partition 
            just to emulate remote I-CPU sender and generates interrupt 
            to start processing this command 

    @param pApi [in] - the pointer to the API command allocated in RX I-CPU partition!!!

    @return [MXRC] an error code */

MXRC SvsrPutICpuApiToRx (ApiHeader * pApi);

/**
 * TODO add doxygen description
 */

MXRC SvsrChangeTransport(COMM_TYPE type, char *sctp_host, unsigned int sctp_port);

/** @brief This function return I-CPU statistics for the specified core

    @param CpuID [in] - the CPU ID
    @param pStat [out]- the pointer to the statistics object 

    @return [MXRC] an error code */

MXRC SvsrGetICpuStatistics (UINT32 CpuID, SvsrApiStat * pStat);

/** @brief This function return I-CORE statistics for the specified core

    @param CpuID [in] - the CPU ID
    @param pStat [out]- the pointer to the statistics object

    @return [MXRC] an error code

    \ingroup group_lte_services

    */
MXRC SvsrGetICoreStatistics (UINT32 CpuID, SvsrMsgStat * pStat);

/** @brief This function returns a number of accessible TX blocks in
           communication channel

    @return [UINT32]  number of block

    *  \ingroup group_lte_hal
    */
UINT32 SvsrGetFreeBlocksICTxPart(void);

/** @brief This function returns a number of accessible RX blocks in
           communication channel

    @return [UINT32]  number of block
    *  \ingroup group_lte_hal

    */
UINT32 SvsrGetFreeBlocksICRxPart(void);

/** @brief The function packs specified heap, the packet data represents current heap state 
           and maybe used for further analyzing 

    @param hHeap    [in] - the heap handle
    @param id       [in] - the heap id, just used to identify heap
    @param pStor    [out]- the storage, where heap info can be stored 
    @param nStorSize[in] - the storage size in bytes

    @return [MXRC] an error code 

    \ingroup group_lte_services
    
    */
#ifdef MX_HEAP_LEAKAGE_CONTROL           
MXRC SvsrPackHeap (HANDLE hHeap, UINT32 id, LPVOID pStor, UINT32 nStorSize);
#endif

/** @brief This function selects I-CPU master CPU, master CPU is the CPU 
           defined to process I-CPU IRQ signals and call API handlers or route 
           API commands to the destination cores 

    @param nMasterCpu [in] - the master CPU, limited with CPU_NUM

    @return [MXRC] an error code of operation */

MXRC SvsrSelectICpuMaster (UINT32 nMasterCpu);

/** @brief This function is designed to enable/disable API diagnostics,
           API diagnostics is designed to send API commands to the host 
           in specified period of time and to check API mechanism integrity 

    @param nTTI         [in] - to enable/disable diagnostics, TTI>0 to enable|0-disable
    @param nNumPacket   [in] - the number of packets or (-1) till stop API
    @param nListSize    [in] - the number of API commands in the list send to the host

    @return [MXRC] an error code of operation*/

MXRC SvsrEnableApiDiag (UINT32 nTTI, UINT32 nNumPacket, UINT32 nListSize, UINT16 DestID, UINT32 TestType);

#ifdef SYS_TRACER_ENABLED
/** @brief This function attaches supervisor communication object to the OS tracer
           Later it will be possible to trace objects by using Sys-tracer (tracer.c/h)

    @param nTrID    [in] - the communication object ID see: [TID_I_CPU_TX  ... TID_I_CORE_3]
    @param pTraceH  [out]- the trace handler 

    @return [LPVOID] the trace storage or NULL if error */

LPVOID SvsrEnableTrace (UINT32 nTrID, HANDLE * pTraceH);

/** @brief To detach the tracer and to stop tracing, the storage has to be manually 
           deallocated by using a pointer returned by SvsrEnableTrace and SvsrFreeTraceStorage

    @param nTrID [in] - the tracer ID, see [TID_I_CPU_TX  ... TID_I_CORE_3]

    @return [MXRC] an error code */
           
MXRC SvsrDisableTrace(UINT32 nTrID);

/** @brief To deallocate trace storage allocated by SvsrEnableTrace 

    @NOTE!  It has t be used only after SvsrDisableTrace function

    @param p [in] - the pointer returned by SvsrEnableTrace
    */

void SvsrFreeTraceStorage (void * p);

#endif

#endif /*_SUPERVISOR_H_*/

#ifdef __cplusplus
}
#endif

