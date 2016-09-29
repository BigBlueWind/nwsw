//-------------------------------------------------------------------------------------------
/** @file icpu2.h
 *
 * @brief This file contains declaration of I-CPU externed interface used 
 *        to send/receive API commands between x4 and x2 ARMs
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
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

#ifndef _I_CPU_2_H_
#define _I_CPU_2_H_

#include "4gmx.h"
#include "appids.h"
#include "hal.h"
/****************************************************************************
*                The module error codes                                     *
****************************************************************************/

#define ICPU_RC_OK                  MX_OK
#define ICPU_RC_PTR_ERROR           DEF_USRRC(APP_MODULE, ICPU2, 1)
#define ICPU_RC_PTR_ALIGN_ERROR     DEF_USRRC(APP_MODULE, ICPU2, 2)
#define ICPU_RC_INIT_ERROR          DEF_USRRC(APP_MODULE, ICPU2, 3)
#define ICPU_RC_STORAGE_OVERWRITE   DEF_USRRC(APP_MODULE, ICPU2, 4)

#define ICPU_VERSION            0x0100

#define ICPU_BLOCK_TX_SIZE      (16*1024)
#define ICPU_BLOCK_RX_SIZE      (16*1024)
#define ICPU_BLOCK_TX_NUM       (256)
#define ICPU_BLOCK_RX_NUM       (256)

#define ICPU_QUEUE_NUM          6

#define ICPU_TX_FREE_QUEUE      0   // This queue contain indexes of free blocks in TX partition
#define ICPU_TX_HI_QUEUE        1   // Indexes of HI priority messages
#define ICPU_TX_REG_QUEUE       2   // Indexes of regular priority messages
#define ICPU_RX_FREE_QUEUE      3   // This queue contain indexes of free blocks in RX partition
#define ICPU_RX_HI_QUEUE        4   // Indexes of HI priority messages
#define ICPU_RX_REG_QUEUE       5   // Indexes of regular priority messages

typedef struct _I_CPU_QUEUE_
{
    VUINT32  *  Ptr;        // The pointer to the queue storage with indexes of accessible blocks (size is ICPU_BLOCK_TX_NUM)
    VUINT32     Put;        // Put index 
    VUINT32     Get;        // Get index
    VUINT32     Num;        // The number of elements in the queue 
    
    VUINT32     StatPut;    // Statistics of putting elements
    VUINT32     StatGet;    // Statistics of getting elements

}ICPUQ;


/** @brief This type of data describes the base objects 
            of I-CPU communication

*/
typedef struct _I_CPU_OBJ_CTX_
{
    UINT32      Version;                // The version of I-CPU 
    UINT32      Status;                 // Bit#0 = 1 : the I-CPU is initialized 
    UINT32      TxIrqID;                // The ID of interrupt to notify remote side
    UINT32      RxIrqID;                // The ID of interrupt to notify this side

    // ************************************
    // The parameters of TX/RX arrays
    // where messages can be located
    // ************************************

    PUINT8      TxMsgStoragePtr;        // The pointer to the array where TX blocks are located
    PUINT8      RxMsgStoragePtr;        // The pointer to the array where RX blocks are located
    UINT32      TxBlocksNum;            // The number of blocks in TX array
    UINT32      TxBlockSize;            // The size of block in bytes of TX array
    UINT32      RxBlocksNum;            // The number of blocks in RX array
    UINT32      RxBlockSize;            // The size of block in bytes of RX array

    UINT32      TxQNum;                 // The number of TX-Queues (to be auto-detected in runtime on Linux)
    UINT32      RxQNum;                 // The number of RX-Queues (to be auto-detected in runtime on Linux)

    // ************************************
    //        TX/RX directions 
    // ************************************
    ICPUQ       Queues[ICPU_QUEUE_NUM]; 
    
}ICPU_OBJ_CTX;


/** @brief This function initializes I-CPU objects

    @param pStorage [in]  - the pointer to the storage where I-CPU objects will be located

    @return [MXRC] an error code */

MXRC ICpuInit (void * pStorage);

/** @brief This function is designed to allocate the new block in I-CPU storage 
           to be used to send the data 

    @return [LPVOID] the pointer or NULL */
           
LPVOID ICpuLockTxData (void);

/** @brief This function is designed to free allocated TX block

    @param Ptr [in] - the pointer to allocated message 

    @return [MXRC] an error code*/
    
MXRC ICpuFreeTxData (LPVOID Ptr);

/** @brief This function is designed to send the message to remote side

    @param Ptr      [in] - the pointer to the message block
    @param nSignal  [in] - to generate the IRQ signal or not

    @return [MXRC] an error code*/

MXRC ICpuSendTxData (LPVOID Ptr, UINT32 nSignal);

/** @brief This function is designed to send the message to remote side
           with HIGH priority

    @param Ptr      [in] - the pointer to the message block
    @param nSignal  [in] - to generate the IRQ signal or not

    @return [MXRC] an error code*/
    
MXRC ICpuSendHiTxData (LPVOID Ptr, UINT32 nSignal);


/** @brief This function is designed to receive the message from remote side

    @NOTE The high priority messages are received first

    @return [LPVOID] the pointer to the received message */
    
LPVOID ICpuLockRxData (void);


/** @brief This function is designed to free RX message

    @param Ptr [in] -  the pointer to the received message 

    @return [MXRC] an error code
    */
    
MXRC ICpuFreeRxData (LPVOID Ptr);

/**@brief This function returns the total number of TX blocks that 
          can be allocated in TX partition(array)

   @return [UINT32] number of TX blocks */

UINT32 ICpuGetTxBlockNum (void);

/**@brief This function returns the size of TX block in bytes 

   @return [UINT32] the size of TX block in bytes*/

UINT32 ICpuGetTxBlockSize(void);

/**@brief This function returns the total number of RX blocks that 
          can be allocated in RX partition(array)

   @return [UINT32] number of TX blocks */

UINT32 ICpuGetRxBlockNum (void);

/**@brief This function returns the size of TX block in bytes 

   @return [UINT32] the size of TX block in bytes*/
   
UINT32 ICpuGetRxBlockSize(void);

/** @brief This function returns the number of free TX blocks that 
           can be really allocated at this moment. This function is not
           the same to ICpuGetTxBlockNum()

    @return [UINT32]  accessible number of I-CPU blocks */

UINT32 ICpuGetTxFreeBlock(void);

/** @brief This function returns the number of free RX blocks that 
           can be really allocated at this moment. This function is not
           the same to ICpuGetRxBlockNum()

    @return [UINT32]  accessible number of I-CPU blocks */

UINT32 ICpuGetRxFreeBlock(void);

/** @brief Diagnostics function used to allocate memory in RX partiton 

    @return [LPVOID] the pointer to RX storage block */

LPVOID ICpuAllocRxData(void);

/** @brief Diagnostics function used to emulate sending command from remote side 

    @param pData  [in] - the pointer to the data
    @param bIrqSet[in] - to set IRQ signal or not

    @return [MXRC] an error code */

MXRC IcpuAddToRxQueue(LPVOID pData, UINT32 bIrqSet);

/** @brief This function checks is block from TX storage or not

    @param pData [in] - the pointer to the data

    @return [BOOL] result */

BOOL ICpuIsTxData (LPVOID pData);

/** @brief This function checks is block from RX storage or not

    @param pData [in] - the pointer to the data

    @return [BOOL] result */

BOOL ICpuIsRxData(LPVOID pData);

/** @brief This function returns the total size in bytes of I-CPU storage 
            it includes: 
               - size of context
               - size of queues 
               - size of partitions

    @return [UINT32] the total size of I-CPU in bytes */
               
UINT32 ICpuGetTotalStorageSize(void);

/** @brief This function returns a pointer to the RX storage 
    @param base[in] - the base address of I-CPU storage 
    @return [UINT32] the base address */

UINT32 ICpuGetRxStorageBase(UINT32 base);

/** @brief This function returns a pointer to the TX storage 
    @param base[in] - the base address of I-CPU storage 
    @return [UINT32] the base address */

UINT32 ICpuGetTxStorageBase(UINT32 base);

/** @brief This function returns the size of RX storage in bytes
    @return [UINT32] the base address */

UINT32 ICpuGetRxStorageSize(void);

/** @brief This function returns the size of TX storage in bytes
    @return [UINT32] the base address */

UINT32 ICpuGetTxStorageSize(void);

#endif /*_I_CPU_2_H_*/

#ifdef __cplusplus
}
#endif



