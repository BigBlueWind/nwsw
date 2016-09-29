//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL iqphone.h
 *
 * @brief IQ player/recorder service, it's developed to be used with CPRI driver
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

#ifndef _IQPHONE_H_
#define _IQPHONE_H_


#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "cpridrv.h"

#define RC_IQPHONE_ALLOC_ERROR      DEF_USRRC(APP_MODULE, IQPHONE, 1)
#define RC_IQPHONE_CPRI_MODE_ERROR  DEF_USRRC(APP_MODULE, IQPHONE, 2)
#define RC_IQPHONE_UNSUP_MODE       DEF_USRRC(APP_MODULE, IQPHONE, 3)
#define RC_IQPHONE_TX_SIZE_ERROR    DEF_USRRC(APP_MODULE, IQPHONE, 4)
#define RC_IQPHONE_RX_SIZE_ERROR    DEF_USRRC(APP_MODULE, IQPHONE, 5)
#define RC_IQPHONE_NOT_INITED       DEF_USRRC(APP_MODULE, IQPHONE, 6)

#define IQPHONE_CTRL_TX_ENABLE      (1 << 4)
#define IQPHONE_CTRL_RX_ENABLE      (1 << 5)
#define IQPHONE_CTRL_TX_RR          (1 << 6)
#define IQPHONE_CTRL_RX_RR          (1 << 7)

#define IQPHONE_STAT_MDMA_TX        (1 << 0)
#define IQPHONE_STAT_MDMA_RX        (1 << 1)

#define IQPHONE_COM_CPRI            (0)

typedef struct _IQ_PHONE_INIT_
{
    UINT32      nMode;      // CPRI mode, TX/RX LOG modes
    UINT32      TxSkipNum;  // The number of TX interrupts that should be skipped
    UINT32      RxSkipNum;  // The number of RX interrupts that should be skipped
    UINT32      TxLogSize;  // The TX log size in bytes
    UINT32      RxLogSize;  // The RX log size in bytes
    UINT32      TxLogLim;   // The number of blocks sent to CPRI/...
    UINT32      RxLogLim;   // The number of blocks that can be written to log

}IQPhoneInitData;

typedef struct _IQ_PHONE_CTX_
{
    UINT32  bCtxInit;       // This flag is set when context was allocated, global variables are set...
    UINT32  bModuleInit;    // This flag is set when host has configured in/out buffers
    UINT32  Mode;
    UINT32  Status;
    UINT32  DevID;

    PUINT8  TxLogPtr;
    UINT32  TxLogOffs;
    UINT32  TxLogSize;

    PUINT8  RxLogPtr;
    UINT32  RxLogOffs;
    UINT32  RxLogSize;

    UINT32  TxSendNum;
    UINT32  RxRecvNum;

    UINT32  TxSkipNum;  // The number of TX interrupts that should be skipped
    UINT32  RxSkipNum;  // The number of RX interrupts that should be skipped

    UINT32  TxLogLim;   // The number of blocks sent to CPRI/...
    UINT32  RxLogLim;   // The number of blocks that can be written to log

}IQPhoneCtx;

MXRC IQPhoneInit (void);

/** @brief This function is designed to initialize CPRI interface
           to play and record data

    @param pCfgData [in] - the configuration data

    @return [MXRC] an error code */

MXRC IQPhoneInitCpri (IQPhoneInitData * pCfgData);

/** @brief This function starts player/logger

    @param nInterface[in] - the player/recorder communication interface, see: IQPHONE_COM_xxx
    @param pCfgData  [in] - the pointer to the player/recorder configuration parameters

    @return [MXRC] an error code */

MXRC IQPhoneStart (UINT32 nInterface, IQPhoneInitData * pCfgData);

/** @brief This function stops player/logger

    @param nMode    [in] - the player/recorder id,
                           see: IQPHONE_CTRL_TX_ENABLE/IQPHONE_CTRL_RX_ENABLE

    @return [MXRC] an error code */

MXRC IQPhoneStop (UINT32 nCtrl);

#endif //_IQPHONE_H_

#ifdef __cplusplus
}
#endif

