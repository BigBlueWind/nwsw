//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL dataint.h
 *
 * @brief Communication data interface, it's developed to unify
*       different interfaces like CPRI/PCIe/sRIO
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

#ifndef _DATA_INT_H_
#define _DATA_INT_H_

#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "cpridrv.h"
#include "evmfpga.h"

// The list of error codes

#define RC_DI_PARAM_ERROR       DEF_USRRC(APP_MODULE, DATAINT, 1)
#define RC_DI_UNKNOWN_INT       DEF_USRRC(APP_MODULE, DATAINT, 2)
#define RC_DI_UNKNOWN_OPT       DEF_USRRC(APP_MODULE, DATAINT, 3)
#define RC_DI_NOT_OPENED        DEF_USRRC(APP_MODULE, DATAINT, 4)
#define RC_DI_UNKNOWN_CMD       DEF_USRRC(APP_MODULE, DATAINT, 5)
#define RC_DI_NOT_SUPPORTED     DEF_USRRC(APP_MODULE, DATAINT, 6)
#define RC_DI_MASK_IS_DISABLED  DEF_USRRC(APP_MODULE, DATAINT, 7)

// The list of communication interfaces supported by the system

#define DI_CPRI             0
#define DI_CPRI_EMULATOR    1
#define DI_PCIE             4
#define DI_SRIO             5

// Configuration options

#define DI_LOG_TX0_LOG_ENABLED              (1 << 0)    // TX#0 log is enabled by default
#define DI_LOG_TX1_LOG_ENABLED              (1 << 1)    // TX#1 log is enabled by default
#define DI_LOG_RX0_LOG_ENABLED              (1 << 2)    // RX#0 log is enabled by default
#define DI_LOG_RX1_LOG_ENABLED              (1 << 3)    // RX#1 log is enabled by default
#define DI_LOG_TX_LOG_RR                    (1 << 4)    // RoundRobin log model
#define DI_LOG_RX_LOG_RR                    (1 << 5)    // RoundRobin log model
#define DI_LOG_OPT_MASK                     (DI_LOG_TX0_LOG_ENABLED|DI_LOG_TX1_LOG_ENABLED|DI_LOG_RX0_LOG_ENABLED|DI_LOG_RX1_LOG_ENABLED|DI_LOG_TX_LOG_RR|DI_LOG_RX_LOG_RR)
#define DI_LOG_TX_LOG_RESET                 (1 << 30)   // Just to reset the pointers, this is command bit, it's not an option
#define DI_LOG_RX_LOG_RESET                 ((UINT32)1 << 31)   // Just to reset the pointers, this is command bit, it's not an option

#define DI_DRV_CTRL_USER_PARAMS             (1 << 0)    // the default drivers parameters should be overwritten with specified in the cfg structure

#define DI_DRV_CTRL_CPRI_IXIA               (1 << 1)    // The default CPRI parameters for IXIA
#define DI_DRV_CTRL_CPRI_RADIOCOMP          (1 << 2)    // CPRI parameters for radiocomp
#define DI_DRV_CTRL_CPRI_SFP_ENABLED        (1 << 3)    // To enable SFP
#define DI_DRV_CTRL_CPRI_OPTICAL_LOOPBACK   (1 << 4)
#define DI_DRV_CTRL_CPRI_20MHZ_1AC          (1 << 5)
#define DI_DRV_CTRL_CPRI_10MHZ_MIMO_2AC     (1 << 6)


#define DI_DRV_CTRL_ADI_PCIE_L1             (1 << 0)    // PCIe - 1 lane
#define DI_DRV_CTRL_ADI_PCIE_L2             (1 << 1)    // PCIe - 2 lanes
#define DI_DRV_CTRL_ADI_PCIE_L4             (1 << 2)    // PCIe - 4 lanes

#define DI_DRV_CTRL_HW_LB                   (1 << 30)   // To set HW loopback

#define DI_INFO_TX_IRQ_NUM          1
#define DI_INFO_RX_IRQ_NUM          2
#define DI_INFO_TX_LOG_SIZE         3
#define DI_INFO_RX_LOG_SIZE         4
#define DI_INFO_TX_MAX_IRQ_TIME     5   // The time between two interrupts in ticks
#define DI_INFO_RX_MAX_IRQ_TIME     6   // The time between two interrupts in ticks


/*******************************************************
*              DI API commands                         *
*******************************************************/
#define DI_AD9361_CTRL              1   // AD9xxx initialization
#define DI_DEALY_CTRL               3   // CPRI delay configuration
#define DI_LOG_MASK                 4   // LOG mask configuration, used for CPRI and for AD9xxx
#define DI_SET_EVMFPGA_INT_PROF     5   // To select interrupt profile for evmfpga code
#define DI_GET_CPRI_REGS            6   //get CPRI registers status


typedef LPVOID DIHANDLE;

typedef union _DI_DRV_CFG_
{
    CPRICONFIG      CpriCfg;

}DiDrvCfg, *PDiDrvCfg;


/** @brief The configuration parameters used with function DiOpen */

typedef struct _DI_CFG_
{
    UINT32 LogCtrl;   // The common parameters
    UINT32 DrvCtrl;   // Driver specific control parameters
    UINT32 EmulTickTime;  // The cpri emulator ticks

    void (*TxCb) (LPVOID pTxCbPtr, UINT32 nChannID, PUINT8 pTxBuf, UINT32 nSize);
    void (*RxCb) (LPVOID pRxCbPtr, UINT32 nChannID, PUINT8 pRxBuf, UINT32 nSize);

    LPVOID TxCbPtr;
    LPVOID RxCbPtr;

    UINT32 nTxAntenas;
    UINT32 nRxAntenas;
    PUINT8 TxLog[NUM_OF_SUPPORTED_ANTENNAS];
    PUINT8 RxLog[NUM_OF_SUPPORTED_ANTENNAS];
    UINT32 RxLogSize;
    UINT32 TxLogSize;
    UINT32 RxLogRoudupPos;
    UINT32 TxLogFreezeNum;
    UINT32 RxLogFreezeNum;

    DiDrvCfg DrvCfg; // Specific driver parameters

    UINT32 UseRxAntenna;
    UINT32 UseTxAntenna;
    UINT32 UpDelay;
    UINT32 RxStartDelayCounter;
}DiConfig, * PDiConfig;

/** @brief The module context */

typedef struct _DI_CTX_
{
    UINT32 IntID;
    UINT32 Opened;

    UINT32 HostLogMaskEnabled;  // The flag to use log mask provided by the host and to ignore provided by the phy
    UINT32 HostLogMaskValue;    // The log mask value provided by the host
    UINT32 EvmFpgaIntProfID;    // The interrupt signals ID

    UINT32 LogCtrl;   // The common parameters
    UINT32 DrvCtrl;   // Driver specific control parameters

    void (*TxCb) (LPVOID pTxCbPtr, UINT32 nChannID, PUINT8 pTxBuf, UINT32 nSize);
    void (*RxCb) (LPVOID pRxCbPtr, UINT32 nChannID, PUINT8 pRxBuf, UINT32 nSize);

    LPVOID TxCbPtr;
    LPVOID RxCbPtr;

    UINT32 nTxAntenas;
    UINT32 nRxAntenas;
    PUINT8 TxLog[NUM_OF_SUPPORTED_ANTENNAS];
    PUINT8 RxLog[NUM_OF_SUPPORTED_ANTENNAS];
    UINT32 TxLogSize;
    UINT32 RxLogSize;
    UINT32 RxLogRoundupPos;
    UINT32 TxLogFreezeNum;
    UINT32 RxLogFreezeNum;
    UINT32 UpDelay;
    UINT32 RxStartDelayCounter;

}DiCtx, *PDiCtx;

/** @brief This funtion initaizes DataInterface module and it's called by
           the service manager

    @return [MXRC] an error code */

MXRC DiInit (void);

/** @brief This function opens communication interface and returns interface handle

    @NOTE This function should be called first before usage of any other API of this service

    @param nDiID        [in] - the interface ID, see: DI_xxx of this file
    @param pCfg         [in] - the common configuration parameters
    @param pOutHandle   [out]- the interface handle

    @return [MXRC] an error code */

MXRC DiOpen (UINT32 nDiID, PDiConfig pCfg, DIHANDLE * pOutHandle);

/** @brief This function shutdowns communication interface

    @NOTE This function should be called first before usage of any other API of this service

    @return [MXRC] an error code */

MXRC DiShutdown (void);

/** @brief This function closes communication interface

    @param h [in]- the interface handle (see: DiOpen)

    @return [MXRC] an error code */

MXRC DiClose (DIHANDLE h);


MXRC DiPause(DIHANDLE h);
MXRC DiResume(DIHANDLE h);

/** @brief This function returns information from data interface driver

    @param h        [in] - the channel handle, see: DiOpen (...)
    @param nChID    [in] - the channel ID (can be treated like antenna)
    @param DiInfoID [in] - the parameter ID (see: DI_INFO_xxx)

    @return [UINT32] value */

UINT32 DiGetInfo (DIHANDLE h, UINT32 nChID, UINT32 DiInfoID);

/** @brief This function returns pointer to the current TX buffer, where data can be putted

    @param h        [in] - the DI handle
    @param nChID    [in] - the channel ID

    @return [LPVOID] pointer to the TX buffer */

LPVOID DiGetTxBuf (DIHANDLE h, UINT32 nChID);

/** @brief This function returns pointer to the current RX buffer, from where data can be read

    @param h        [in] - the DI handle
    @param nChID    [in] - the channel ID

    @return [LPVOID] pointer to the RX buffer */

LPVOID DiGetRxBuf (DIHANDLE h, UINT32 nChID);

/** @brief This function is designed to set ADI/CPRI HW loopback (TX->RX)

    @param h    [in] - the DI handle (see: DiOpen(...))
    @param bLB  [in] - LB mode: 1 - set LB, 0 - reset LB

    @return [MXRC] an error code */

MXRC DiSetLB (DIHANDLE h, BOOL bLB);

/** @brief This function enables/disables LOGs for selected device

    @param h        [in] - the DI handle (please see: DiOpen(...))
    @param nChID    [in] - the channel ID (actually antenna ID)
    @param nLogCtrl [in] - the LOG bit mask: see DI_LOG_xxx

    @return [MXRC] an error code */

MXRC DiLogControl (DIHANDLE h, UINT32 nChID, UINT32 nLogCtrl);

/** @brief This function returns log mask forced set by the host
    @return [UINT32] log mask*/

UINT32 DiGetForcedLogMask (void);

/** @brief This function returns status is forced log mask enabled or not
    @return [UINT32] status*/

UINT32 DiGetIsForcedLogMask (void);

/** @brief This function sets log mask that will be applied to data interface
    @param Enabled [in] - enable/disable log mask
    @param nMask   [in] - the log mask value

    @return [MXRC] an error code */

MXRC DiSetForcedLogMask (UINT32 Enabled, UINT32 nMask);

MXRC DiTest (UINT32 nMode);

#endif // _DATA_INT_H_

#ifdef __cplusplus
}
#endif

