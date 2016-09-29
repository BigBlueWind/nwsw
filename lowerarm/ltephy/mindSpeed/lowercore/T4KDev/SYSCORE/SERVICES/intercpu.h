//-------------------------------------------------------------------------------------------
/** @file intercpu.h
 *
 * @brief This file contains declaration of inter-cpu API used in application
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
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

#ifndef _INTER_CPU_H_
#define _INTER_CPU_H_

#include "4gmx.h"
#include "appids.h"
#include "hal.h"
#include "icpu2.h"
/****************************************************************************
*                The module error codes                                     *
****************************************************************************/

#define IC_RC_OK                MX_OK
#define IC_RC_ALREADY_USED      DEF_USRRC(APP_MODULE, ICMGR, 1)
#define IC_RC_CHAN_ID_ERROR     DEF_USRRC(APP_MODULE, ICMGR, 2)
#define IC_RC_CLIENT_ERROR      DEF_USRRC(APP_MODULE, ICMGR, 3)
#define IC_RC_UNEXP_API_END     DEF_USRRC(APP_MODULE, ICMGR, 4)
#define IC_RC_NOT_MEMORY        DEF_USRRC(APP_MODULE, ICMGR, 5)
#define IC_RC_SEND_ERROR	    DEF_USRRC(APP_MODULE, ICMGR, 6)
#define IC_RC_NON_SUPPORTED	    DEF_USRRC(APP_MODULE, ICMGR, 7)
#define IC_RC_NOT_INITED        DEF_USRRC(APP_MODULE, ICMGR, 8)
#define IC_RC_CPU_ID_ERROR      DEF_USRRC(APP_MODULE, ICMGR, 9)
#define IC_RC_PROC_ERROR        DEF_USRRC(APP_MODULE, ICMGR, 10)
#define IC_RC_THR_CREATE_ERROR  DEF_USRRC(APP_MODULE, ICMGR, 11)
#define IC_RC_ICPU_IS_REQUIRED  DEF_USRRC(APP_MODULE, ICMGR, 12)

#define INTER_CPU_MEM_ADDRESS        0

#define INTER_CPU_CTRL_MAX_IRQ       (HAL_IRQ_INTER_ARM_7 + 1)

#define INTER_CPU_SVSR_INTERRUPT_SRC HAL_IRQ_INTER_ARM_0
#define INTER_CPU_SVSR_INTERRUPT_DST HAL_IRQ_INTER_ARM_1

#define ICPU_LOCK_TX_TIMEOUT        (150 * 50) // 50 us
#define ICPU_LOCK_RX_TIMEOUT        (150 * 50) // 50 us

/** @brief The list of inter-cpu channels supported by the system */

typedef enum _IC_CHANNEL_IDs_
{
    IC_CHAN_SVSR = 0,     // The parts and queues to send/receive
                          // data to/from the upper ARM (supervisor path)

    IC_CHAN_SVSR_HBI = 1, // This path is used to send/receive the commands to/from
                          // the host by using HBI interface

    IC_CHAN_SVSR_EX_ICPU=2, // The extended I-CPU interface 

    IC_CHAN_NUM           // The number of objects
}ICChannelID;

/** @brief This type describes opened inter-cpu channel and used
           for receiving and sending data from/to remoute server */

typedef struct _IC_CHANNEL_CTX_
{
    HANDLE  hHandler;       // The dispatcher of incomming data
    LPVOID  HandParam;      // The parameter of dispatcher

    HANDLE  hRxQueue;       // The RX queue of RX partition indexes
    HANDLE  hRxPart;        // The RX partition, it contains incom. data
    HANDLE  hTxQueue;       // The TX queue
    HANDLE  hTxPart;        // The TX partition it contains outgoing data

    ICChannelID ChanID;     // The id of inter-cpu channel
    UINT32      DstIrqID;   // The id of Irq signal assigned to the IC obj
    UINT32      SrcIrqID;   // The id of Irq signal assigned to the IC obj
}ICChannelCtx;

/** @brief  The type of data handler */

typedef UINT32 (*ICHandler) (ICChannelCtx * channelCtx);

/** @brief The type of inter-cpu object descriptors (queue + partiton) */

typedef struct _INTERCPU_OBJ_
{
    ICQUEUE queue;
    ICPART part;

} InterCpuObj;

/** @brief The structure of memory used for inter-cpu communication */

typedef struct _ICMEMMAP_
{
    InterCpuObj ToUpArmCtrlObj;     // It is used for sends data to the up ARM
    InterCpuObj FromUpArmCtrlObj;   // For receiving data from the up ARM

} SICMemMap;


typedef struct _IC_CTX_
{
    UINT32          bInited;
    UINT32          BaseAddr;
    UINT32          nInfID;
    UINT32          nSrcIrqID;
    UINT32          nDstIrqID;
    UINT32          nMasterCpuID;
    ICHandler       Proc;
    ICChannelCtx *  CpuCtx [CPU_NUM];

}ICCTX, *PICCTX;

/****************************************************************************
*                Module interface functions                                 *
****************************************************************************/

/** @brief The function inits inter-cpu manager, this function should be called
           before usage of any API declared in this header.

 @return [MXRC] error code of initialization */

MXRC ICInit(void);

/** @brief This function returns the total size of I-CPU objects

    @param BaseAddr [in] - the base address of I-CPU space

    @return [UINT32] the total size of I-CPU objects */

UINT32 ICGetICpuSize (UINT32 BaseAddr);

/** @brief The function returns pointers to the handler parameter
    @param pCtx [in] - the pointer to the channel context
    @return [LPVOID] the pointer to the handler parameter */

INLINE LPVOID ICGetHandParam (LPVOID pCtx)
{
    _ASSERT(pCtx != NULL);

    return ((ICChannelCtx *)pCtx)->HandParam;
}

/** @brief The function opens specified by the id inter-cpu channel and
           creates a thread for processing incomming data.

    @param nCommID[in] - the id of inter-cpu channel
    @param hand   [in] - the pointer to function, this function will be
                         scheduled for execution every time when new data
                         be precent in opened channel. It can be NULL.
    @param pParam [in] - the pointer to handler parameter

    @param pChannel[out]-the descriptor of opened channel

    @return [MXRC] error code */

MXRC ICOpenChannel (ICChannelID nCommID, ICHandler hand, LPVOID pParam,
                  ICChannelCtx ** pChannel);

/** @bried The function removes allocated system resource
           and disables inter-cpu interrupt that was assigned for
           opened channel.

    @param pChannel [in] - the descriptor of opened channel

    @return [MXRC] error code */

MXRC ICCloseChannel (ICChannelCtx * pChannel);

/** @brief The function returns size of communication channel block used for
           data sending/receiving. In case of non message-oriented comm. chan.,
           the size of block will be 0xFFFFFFFF bytes.

    @param pChannel [in] - the context of communication channel

    @return [UINT32] size of block*/

UINT32 ICGetSendBlockSize (ICChannelCtx * pChannel);

/** @brief The function returns number of communication channel blocks used to
           send/receive data. In case of non message-oriented comm. chan.,
           the number is equal to 0.

    @param pChannel [in] - the context of communication channel

    @return [UINT32] size of block*/

UINT32 ICGetRecvBlockCount (ICChannelCtx * pChannel);

/** @brief The function returns size of communication channel block used for
           data sending/receiving. In case of non message-oriented comm. chan.,
           the size of block will be 0xFFFFFFFF bytes.

    @param pChannel [in] - the context of communication channel

    @return [UINT32] size of block*/

UINT32 ICGetRecvBlockSize (ICChannelCtx * pChannel);


/** @brief The function sends API command to the upper ARM by using
           inter-cpu communication objects.

    @param IntID[in] - the ID of inter-cpu communication interface
    @param pData[in] - the pointer to the API command
    @param nSize[in] - the size of data pointer by the pData argument

    @return [MXRC] error code */

MXRC ICWriteData (ICChannelCtx * pChannel, LPVOID pData, UINT32 nSize);

/** @brief The function reads data stored in the inter-cpu channel

    @param pChannel [in]  - the channel descriptor
    @param pData    [out] - the received data
    @param pnBufSize[in/out] - in: the buffer length
                              out: the size of received data

    @return [MXRC] error code */

MXRC ICReadData (ICChannelCtx * pChannel, LPVOID pData, PUINT32 pnBufSize);

/**@brief The function returns a pointer to the received data, the data
          are stored in communication channel and should be deallocated
          by calling free function.

   @param pChannel [in] - the context of communication channel

   @return [LPVOID] the pointer to the receive data */

LPVOID ICLockRxData (ICChannelCtx * pChannel);

/**@brief The function frees locked received data.

   @param pChannel [in] - the context of communication channel
   @param pData    [in] - the return value of ICLockRxData function

   @return [LPVOID] the pointer to the receive data */

#ifdef SYS_TRACER_ENABLED
    MXRC ICFreeRxData (ICChannelCtx * pChannel, LPVOID pData);
    MXRC ICFreeRxDataLR (ICChannelCtx * pChannel, LPVOID pData, UINT32 lr);
#else
    MXRC ICFreeRxData (ICChannelCtx * pChannel, LPVOID pData);
#endif


/**@brief The function returns a pointer to the allocated block in send partition.

   @param pChannel [in] - the context of communication channel

   @return [LPVOID] the pointer to the allocated block */

#ifdef SYS_TRACER_ENABLED
    LPVOID ICLockTxData (ICChannelCtx * pChannel);
    LPVOID ICLockTxDataLR (ICChannelCtx * pChannel, UINT32 lr);
#else
    LPVOID ICLockTxData (ICChannelCtx * pChannel);
#endif

/** @brief The function returns a pointer to the allocated data of TX storage 
    @param pChannel [in] - the pointer to the communication channel 

    @return [LPVOID] the pointer to the allocated block */

#ifdef SYS_TRACER_ENABLED
    LPVOID ICAllocTxData (ICChannelCtx * pChannel);
    LPVOID ICAllocTxDataLR (ICChannelCtx * pChannel, UINT32 lr);
#else
    LPVOID ICAllocTxData (ICChannelCtx * pChannel);
#endif

/** @brief The function returns a pointer to the allocated data of RX storage 
    @param pChannel [in] - the pointer to the communication channel 

    @return [LPVOID] the pointer to the allocated block */

LPVOID ICAllocRxData (ICChannelCtx * pChannel);

/**@brief The function returns a pointer to the allocated block in send partition.

   @param pChannel [in] - the context of communication channel
   @param pData    [in] - the data allocated by the ICLockTxData function
   @param bNotify  [in] - notify/don't notify  the remote side

   @return [LPVOID] the pointer to the allocated block

   *  \ingroup group_lte_hal

   */

MXRC ICSendHiTxData (ICChannelCtx * pChannel, LPVOID pData, BOOL bNotify);

/**@brief The function returns a pointer to the allocated block in send partition.

   @param pChannel [in] - the context of communication channel
   @param pData    [in] - the data allocated by the ICLockTxData function
   @param bNotify  [in] - notify/don't notify  the remote side

   @return [LPVOID] the pointer to the allocated block */

MXRC ICSendTxData (ICChannelCtx * pChannel, LPVOID pData, BOOL bNotify);

/** @brief This function puts data to the incoming channel and generates event to 
            the local system (to own processor) to emulate remote I-CPU sender

    @param pChannel [in] - the pointer to the channel context
    @param pData    [in] - the pointer to the input data 
    @param bNotify  [in] - the flag of event generating

    @return [MXRC] an error code */

MXRC ICAddRxMessage (ICChannelCtx * pChannel, LPVOID pData, BOOL bNotify);

/** @brief The function frees allocated TX block of communication channel

    @param pChannel [in] - the context of communication channel
    @param pData    [in] - the allocated by the ICLockTxData function block

    @return [MXRC] error code */

#ifdef SYS_TRACER_ENABLED
    MXRC ICFreeTxData (ICChannelCtx * pChannel, LPVOID pData);
    MXRC ICFreeTxDataLR (ICChannelCtx * pChannel, LPVOID pData, UINT32 lr);
#else
    MXRC ICFreeTxData (ICChannelCtx * pChannel, LPVOID pData);
#endif

/** @brief This function returns a number of accessible TX blocks in
           communication channel

    @param pChannel [in] - the descriptor of communication channel

    @return [UINT32]  number of block*/

UINT32 ICGetTxFreeBlocks (ICChannelCtx * pChannel);

/** @brief This function returns a number of accessible RX blocks in
           communication channel

    @param pChannel [in] - the descriptor of communication channel

    @return [UINT32]  number of block*/

UINT32 ICGetRecvFreeBlocks (ICChannelCtx * pChannel);

UINT32 ICGetRXPartStorageAddr (UINT32 BaseAddr, UINT32 icpu_id);
UINT32 ICGetRXPartStorageSize (UINT32 BaseAddr, UINT32 icpu_id);
UINT32 ICGetTXPartStorageAddr (UINT32 BaseAddr, UINT32 icpu_id);
UINT32 ICGetTXPartStorageSize (UINT32 BaseAddr, UINT32 icpu_id);

UINT32 ICIsTxMemory (ICChannelCtx * pChannel, LPVOID pData);
UINT32 ICIsRxMemory (ICChannelCtx * pChannel, LPVOID pData);

MXRC GetICMemObjects(SICMemMap * pInfo);
MXRC GetExtICMemObjects(ICPU_OBJ_CTX * pInfo);
UINT32 GetICpuRegionSize(UINT32 BaseAddr, UINT32 icpu_id);

/** @brief This function initializes the I-CPU IRQ handler on the specified core 

    @param nCpuID [in] - the CPU id

    @return [MXRC] an error code */

MXRC ICSelectMasterCpu (UINT32 nCpuID);

#endif /*_INTER_CPU_H_*/

#ifdef __cplusplus
}
#endif

