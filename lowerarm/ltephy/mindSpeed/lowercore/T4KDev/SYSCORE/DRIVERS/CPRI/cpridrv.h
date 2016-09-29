//-------------------------------------------------------------------------------------------
/** @file cpridrv.h
 *
 * @brief CPRI driver definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.44 $
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

#ifndef __CPRIDRV_H__
#define __CPRIDRV_H__

#include "4gmx.h"
#include "hal.h"
#include "appids.h"
#include "apimgr.h"
#include "devinfo.h"
#include "mdmafectypes.h"

/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define CPRI_RC_OK            MX_OK
#define CPRI_RC_SFP_INIT            DEF_USRRC(APP_DRIVER, CPRI_DRV, 1)
#define CPRI_RC_INVALID_SERDES      DEF_USRRC(APP_DRIVER, CPRI_DRV, 2)
#define CPRI_RC_ALLOC_ERROR         DEF_USRRC(APP_DRIVER, CPRI_DRV, 3)
#define CPRI_RC_CONNECT_ERROR       DEF_USRRC(APP_DRIVER, CPRI_DRV, 4)

//#define CPRI_TRACE
#define CPRI_SYNC_LOCAL_TIMER

#define NUM_OF_SUPPORTED_ANTENNAS 4

#define CPRI_RX_START_DELAY_COUNTER_RRH     2500

#define CPRI_RX_START_DELAY_COUNTER     8500   /*PWAV*/ /* 100 – original value*/

#define CPRI_IRQ_TICKS_MEASUREMENT

#define CPRI_UP_DELAY               20000 //*PWAV requires longer */  /* 1000 – original value*/

#define CPRI_DEV_NUM        6

#define CPRIRATE_614        1
#define CPRIRATE_1228       2
#define CPRIRATE_2457       4
#define CPRIRATE_3072       5
#define CPRIRATE_4915       8
#define CPRIRATE_6144       10

#define CPRI_CHIP_SRATE     3840
#define CPRI_3_84_MSPS      CPRI_CHIP_SRATE
#define CPRI_7_68_MSPS      (2*CPRI_CHIP_SRATE)
#define CPRI_15_36_MSPS     (4*CPRI_CHIP_SRATE)
#define CPRI_30_72_MSPS     (8*CPRI_CHIP_SRATE)

#define CPRI_FAST_CM_PTR    194
#define CPRI_PWAV_CM_SEL    208

#define CPDMA_RXSRESET      (1 << 1)
#define CPDMA_TXSRESET      (1 << 1)

#define CPDMA_BASEADDR(n)               (RAD_APB_CPDMA_BASEADDR + (n * 0x01000))
#define RADIP_BASEADDR(n)               (CPRI_BASEADDR  + (n * 0x20000))

#define I2CSRDS2SFP_0_1_ADDR            0x1  //Serdes 2 SFP connection 0 1
#define I2CSRDS2SFP_2_3_ADDR            0x2  //Serdes 2 SFP connection 2 3
#define I2CSRDS1SFP_0_1_ADDR            0x3  //Serdes 1 SFP connection 0 1

enum {                      // CPRI rate
    HDLC_RATE_0 = 0,        // Any rate
    HDLC_RATE_240,          // Any rate
    HDLC_RATE_480,          // Any rate
    HDLC_RATE_960,          // >= 1228.8 Mpbs
    HDLC_RATE_1920,         // >= 2457.6 Mpbs
    HDLC_RATE_2400,         // 3072 Mpbs
    HDLC_RATE_3840,         // 3072 Mpbs
    HDLC_RATE_4800
};

#define CPRI_MAX_ETH_FRAME_SIZE (1536)

#define CPRI_ETHC1_LEND         (1<<1)
#define CPRI_ETHC1_IE           (1<<10)
#define CPRI_ETHC1_RXIE         (1<<11)
#define CPRI_ETHC1_TXIE         (1<<12)
#define CPRI_ETHC1_RXRIE        (1<<13)
#define CPRI_ETHC1_RXREIE       (1<<15)
#define CPRI_ETHC1_RXRBIE       (1<<16)
#define CPRI_ETHC1_TXRBIE       (1<<19)
#define CPRI_ETHC1_TXAIE        (1<<18)

#define CPRI_ETHTXSTAT_RDY      (1<<1)
#define CPRI_ETHTXSTAT_ABORT    (1<<1)
#define CPRI_ETHTXSTAT_RDY_BLK  (1<<2)

#define CPRI_ETHRXSTAT_RDY      (1<<0)
#define CPRI_ETHRXSTAT_EOF      (1<<1)
#define CPRI_ETHRXSTAT_ABORT    (1<<2)
#define CPRI_ETHRXSTAT_RDY_END  (1<<5)
#define CPRI_ETHRXSTAT_RDY_BLK  (1<<6)

#define GET_MAP_AC(CpriRate, SamplingRate, SampleWidth) 1//(8*(CpriRate)*15/2/((SamplingRate)/CPRI_CHIP_SRATE)/(SampleWidth))

#define CPRI_IXIA           0
#define CPRI_RADIOCOMP      1
#define CPRI_EMULATOR       2
#define CPRI_MIMO_2AC       3   // The MIMO is configured like 2 AC (2x2)

typedef struct tCPRIREGS
{
    VUINT32 CPRI_INTR;
    VUINT32 CPRI_STATUS;
    VUINT32 CPRI_CONFIG;
    VUINT32 CPRI_CTRL_INDEX;
    VUINT32 CPRI_RX_CTRL;
    VUINT32 CPRI_TX_CTRL;
    VUINT32 RES0;
    VUINT32 CPRI_LCV;
    VUINT32 CPRI_BFN;
    VUINT32 RES1[2];
    VUINT32 CPRI_HW_RESET;
    VUINT32 CPRI_PHY_LOOP;
    VUINT32 RES2[2];
    VUINT32 CPRI_CM_CONFIG;
    VUINT32 CPRI_CM_STATUS;
    VUINT32 CPRI_RX_DELAY_CTRL;
    VUINT32 CPRI_RX_DELAY;
    VUINT32 CPRI_ROUND_DELAY;
    VUINT32 CPRI_EX_DELAY_CONFIG;
    VUINT32 CPRI_EX_DELAY_STATUS;
    VUINT32 RES3[10];
    VUINT32 CPRI_SERDES_CONFIG;
    VUINT32 RES4[15];
    VUINT32 CPRI_MAP_CONFIG;
    VUINT32 CPRI_MAP_CNT_CONFIG;
    VUINT32 RES5[2];
    VUINT32 CPRI_MAP_TBL_CONFIG;
    VUINT32 RES6;
    VUINT32 CPRI_MAP_TBL_INDEX;
    VUINT32 CPRI_MAP_TBL_RX;
    VUINT32 CPRI_MAP_TBL_TX;
    VUINT32 PRI_MAP_OFFSET_RX;
    VUINT32 CPRI_MAP_OFFSET_TX;
    VUINT32 CPRI_START_OFFSET_RX;
    VUINT32 CPRI_START_OFFSET_TX;
    VUINT32 CPRI_MAP_RX_READY_THR;
    VUINT32 CPRI_MAP_TX_READY_THR;
    VUINT32 CPRI_MAP_TX_START_THR;
    VUINT32 CPRI_IQ_RX_BUF_STATUS;
    VUINT32 RES8[7];
    VUINT32 CPRI_IQ_TX_BUF_STATUS;
    VUINT32 RES9[7];
    VUINT32 CPRI_PRBS_CONFIG;
    VUINT32 RES10[3];
    VUINT32 CPRI_PRBS_STATUS;
    VUINT32 RES11[11];
    VUINT32 CPRI_IQ_RX_BUF_CONTROL;
    VUINT32 RES12[7];
    VUINT32 CPRI_IQ_TX_BUF_CONTROL;
    VUINT32 RES13[407];
    VUINT32 ETH_RX_STATUS;
    VUINT32 ETH_TX_STATUS;
    VUINT32 ETH_CONFIG_1;
    VUINT32 ETH_CONFIG_2;
    VUINT32 ETH_RX_CONTROL;
    VUINT32 ETH_RX_DATA;
    VUINT32 ETH_RX_DATA_WAIT;
    VUINT32 ETH_TX_CONTROL;
    VUINT32 ETH_TX_DATA;
    VUINT32 ETH_TX_DATA_WAIT;
    VUINT32 ETH_RX_EX_STATUS;
    VUINT32 ETH_ADDR_MSB;
    VUINT32 ETH_ADDR_LSB;
    VUINT32 ETH_HASH_TABLE;
    VUINT32 RES14[3];
    VUINT32 ETH_FWD_CONFIG;
    VUINT32 ETH_CNT_RX_FRAME;
    VUINT32 ETH_CNT_TX_FRAME;
    VUINT32 RES15[236];
    VUINT32 HDLC_RX_STATUS;
    VUINT32 HDLC_TX_STATUS;
    VUINT32 HDLC_CONFIG;
    VUINT32 HDLC_CONFIG_2;
    VUINT32 HDLC_RX_CONTROL;
    VUINT32 HDLC_RX_DATA;
    VUINT32 HDLC_RX_DATA_WAIT;
    VUINT32 HDLC_TX_CONTROL;
    VUINT32 HDLC_TX_DATA;
    VUINT32 HDLC_TX_DATA_WAIT;
    VUINT32 HDLC_RX_EX_STATUS;
    VUINT32 RES16[6];
    VUINT32 HDLC_FWD_CONFIG;
    VUINT32 HDLC_CNT_RX_FRAME;
    VUINT32 HDLC_CNT_TX_FRAME;
} CPRIREGS, *PCPRIREGS;

typedef struct tCPDMAREGS
{
    VUINT32 MEMSEG;
    VUINT32 RX_DMA_ENA;
    VUINT32 TX_DMA_ENA;
    VUINT32 DMA_CTRL;
    VUINT32 RX_INT_STAT;
    VUINT32 RX_INT_ENA;
    VUINT32 TX_INT_STAT;
    VUINT32 TX_INT_ENA;
    VUINT32 BLK_SIZE;
    VUINT32 INT_SIZE;
    VUINT32 IQ_SAMPLE_SIZE;
    VUINT32 LOOPBACK_SIZE;
    VUINT32 RX_AC_ENA;
    VUINT32 TX_AC_ENA;
    VUINT32 EXT_COMM;
    VUINT32 EXT_TRANSFER;
    VUINT32 RX_AC_PTR[8];
    VUINT32 TX_AC_PTR[8];
    VUINT32 RX_AC_BASE[8];
    VUINT32 TX_AC_BASE[8];
    VUINT32 RX_AC_PROG_CHK[8];
    VUINT32 TX_AC_PROG_CHK[8];
} CPDMAREGS, *PCPDMAREGS;

typedef struct tCPRIETHBDESC
{
    PUINT8      BPtr;
    UINT32      BCtrl;
} CPRIETHBDESC, *PCPRIETHBDESC;

#define NUM_CPRI_ETH_TX_BUF     4   // this size must be 2**n
#define NUM_CPRI_ETH_RX_BUF     4   // this size must be 2**n
#define CPRI_ETH_RX_BUF_DONE   ((UINT32)1 << 31)


#define CPRI_LOG_CTRL_TX0_ENABLED           (1 << 0)    // TX#0 log is enabled by default
#define CPRI_LOG_CTRL_TX1_ENABLED           (1 << 1)    // TX#1 log is enabled by default
#define CPRI_LOG_CTRL_RX0_ENABLED           (1 << 2)    // RX#0 log is enabled by default
#define CPRI_LOG_CTRL_RX1_ENABLED           (1 << 3)    // RX#1 log is enabled by default
#define CPRI_LOG_CTRL_TX_RR                 (1 << 4)    // RoundRobin log model
#define CPRI_LOG_CTRL_RX_RR                 (1 << 5)    // RoundRobin log model
#define CPRI_LOG_CTRL_MASK                  (CPRI_LOG_CTRL_TX0_ENABLED|CPRI_LOG_CTRL_TX1_ENABLED| \
                                             CPRI_LOG_CTRL_RX0_ENABLED|CPRI_LOG_CTRL_RX1_ENABLED| \
                                             CPRI_LOG_CTRL_TX_RR|CPRI_LOG_CTRL_RX_RR)

#define PAR_CPRI_GPIO_TXIRQ_ENA         10
#define PAR_CPRI_GPIO_TXIRQ_PORT        11
#define PAR_CPRI_GPIO_TXIRQ_TOGGLE_DELAYS        12

#define PAR_CPRI_GPIO_RXIRQ_ENA         20
#define PAR_CPRI_GPIO_RXIRQ_PORT        21
#define PAR_CPRI_GPIO_RXIRQ_TOGGLE_DELAYS         22


// ID of configuration parameters
#define PAR_CPRI_CONF_NUM_CPRI_DEV         22
#define PAR_CPRI_CONF_SAMPLING_RATE        23
#define PAR_CPRI_CONF_SAMPLE_WIDTH         24
#define PAR_CPRI_CONF_REC_MAC_ADDR0        25
#define PAR_CPRI_CONF_REC_MAC_ADDR1        26
#define PAR_CPRI_CONF_REC_MAC_ADDR2        27
#define PAR_CPRI_CONF_REC_MAC_ADDR3        28
#define PAR_CPRI_CONF_REC_MAC_ADDR4        29
#define PAR_CPRI_CONF_REC_MAC_ADDR5        30
#define PAR_CPRI_CONF_RE_MAC_ADDR5         31
#define PAR_CPRI_CONF_REC_IP_ADDR          32
#define PAR_CPRI_CONF_RE_IP_ADDR           33
#define PAR_CPRI_CONF_ETH_ENABLE           34
#define PAR_CPRI_CONF_ETH_LITTLE_ENDIAN    35
#define PAR_CPRI_CONF_ETH_FCS_ENABLE       36
#define PAR_CPRI_CONF_ETH_RATE             37
#define PAR_CPRI_CONF_HDLC_ENABLE          38
#define PAR_CPRI_CONF_MAX_AC_NUM           39
#define PAR_CPRI_CONF_CPRI_RATE            40

#define CPRI_ANTENNA_MAX 4

#define CPRI_USE_DEFAULT                    0
#define CPRI_USE_ANTENNA_0              1
#define CPRI_USE_ANTENNA_1              2

#define CPRI_GET_RX_USAGE(x)  ((x >> 28) & 0x3)
#define CPRI_GET_TX_USAGE(x)  ((x >> 30) & 0x3)
#define CPRI_GET_RATE(x)  (x & 0xFFFFFFF)

typedef struct tCPRICTX
{
    PCPRIREGS       pCpriRegs;
    PCPDMAREGS      pCpdmaRegs;
    PUINT8          pTxDmaBuf;
    PUINT8          pRxDmaBuf;

    UINT32          LogCtrl;        // Enable/Disable log, log options ...
    UINT32          RxLogFreezeNum; // to save the space in the log and skip some
                                    // number of irrelevant SF at the beginning we need to use this filed,
                                    // it can be set to 0
    UINT32          nRXAntenas;
    PUINT8          pRxLogBuf[CPRI_ANTENNA_MAX];      // RX log pointer
    UINT32          RxLogBufSize;
    UINT32          RxLogBufPos[CPRI_ANTENNA_MAX];
    UINT32          RxLogBufReset[CPRI_ANTENNA_MAX];
    UINT32          RxLogBufTime[CPRI_ANTENNA_MAX];
    UINT32          RxLogBufRoudupPos;  // This value is used to initialize RX log
                                        // possition for EMULATOR mode on the round-up step
                                        // we need to play this game for the PHY code,
                                        // by default this value is set to 0
    UINT32          TxLogFreezeNum; // to save the space in the log and skip some
                                    // number of irrelevant SF at the beginning we need to use this filed,
                                    // it can be set to 0
    UINT32          nTXAntenas;
    PUINT8          pTxLogBuf[CPRI_ANTENNA_MAX];      // TX log pointer
    UINT32          TxLogBufSize;
    UINT32          TxLogBufReset[CPRI_ANTENNA_MAX];
    UINT32          TxLogBufTime[CPRI_ANTENNA_MAX];
    UINT32          TxLogBufPos[CPRI_ANTENNA_MAX];
    PUINT8          pEmulTxBuf[CPRI_ANTENNA_MAX];
    PUINT8          pEmulRxBuf[CPRI_ANTENNA_MAX];
    UINT32          MaxTxIrqPeriod;
    UINT32          TxIrqTick;
    UINT32          MaxRxIrqPeriod;
    UINT32          RxIrqTick;

    UINT32          TxPlayBufSize;
    UINT32          TxPlayBufPos;
    UINT32          TxBufOffset;
    UINT32          RxBufOffset;
    UINT32          DmaIntSize;

    UINT32          TxLogIrqStart;
    UINT32          RxLogIrqStart;
    UINT32          TxStopCount;
    UINT32          RxStopCount;
    UINT32          TxLogCount;
    UINT32          RxLogCount;
    UINT32          TxLogEnable;
    UINT32          RxLogEnable;
    PDMAFDESC       pMdmaFDesc; // points to Tx and Rx MDMA FDesc
    UINT32          NumAxC;
    UINT32          CpriMapConf;
    UINT32          RecIpAddr;
    UINT32          ReIpAddr;
    UINT8           RecMacAddr[6];
    UINT8           ReMacAddr[6];
    UINT8           EthEnable;
    UINT8           HdlcEnable;
    UINT8           TxEthBufPut;
    UINT8           TxEthBufGet;
    UINT8           RxEthBufPut;
    UINT8           RxEthBufGet;
    UINT8           DevID;
    UINT8           CpriConfType;
    CPRIETHBDESC   *TxEthBDesc;
    CPRIETHBDESC   *RxEthBDesc;
    void            (*pRxDMACallBack)(LPVOID pRxPtr, UINT32 nChannID, PUINT8 pRxData, UINT32 nSize);
    void            (*pTxDMACallBack)(LPVOID pTxPtr, UINT32 nChannID, PUINT8 pTxData, UINT32 nSize);
    void            *RxPtr;
    void            *TxPtr;
    UINT32          TxIrqNum;
    UINT32          RxIrqNum;
    UINT32          TxIrqGpioEna;
    UINT32          TxIrqGpioPort;
    UINT32          RxIrqGpioEna;
    UINT32          RxIrqGpioPort;
    UINT32          TxIrqGpioToggleDelays[2];
    UINT32          RxIrqGpioToggleDelays[2];
    UINT32          TxIrqGpioToggleCntr;
    UINT32          RxIrqGpioToggleCntr;
    UINT32          TxIrqGpioToggleDelayIdx;
    UINT32          RxIrqGpioToggleDelayIdx;
    UINT32          UseRxAntenna;
    UINT32          UseTxAntenna;
    UINT32          UpDelay;
    UINT32          RxStartDelayCounter;
} CPRICTX, *PCPRICTX;

typedef struct tCPRIDRVCTX
{
    CPRICTX     CpriCtx[CPRI_DEV_NUM];
    UINT32      CpriRate;
    UINT32      DevEnaMask;
    UINT32      CpuID;
    UINT32      EmulTickTime;
    UINT32      DiUsed;
} CPRIDRVCTX, *PCPRIDRVCTX;

typedef struct tCPRICONFIG
{
    UINT32      DiUsed;
    UINT32      NumCpriDev;
    UINT32      CpriRate;
    UINT32      SamplingRate;
    UINT32      RecIpAddr;
    UINT32      ReIpAddr;
    UINT8       SampleWidth;
    UINT8       RecMacAddr[6];
    UINT8       ReMacAddr[6];
    UINT8       EthEnable;
    UINT8       EthFcsEnable;
    UINT8       EthLittleEndian;
    UINT8       EthRate;
    UINT8       HdlcEnable;
    UINT8       CpriConfType;
    UINT32      MaxAcNum;
    UINT32      (*pCPRICreateCallBack)(PCPRIDRVCTX pCpriDrvCtx);
    UINT32      EmulTickTime;
    UINT32      TxIrqGpioEna;
    UINT32      TxIrqGpioPort;
    UINT32      TxIrqGpioToggleDelays;
    UINT32      RxIrqGpioEna;
    UINT32      RxIrqGpioPort;
    UINT32      RxIrqGpioToggleDelays;
    UINT32      UseRxAntenna;
    UINT32      UseTxAntenna;
    UINT32      UpDelay;
    UINT32      RxStartDelayCounter;
} CPRICONFIG, *PCPRICONFIG;

#define CPRI_TX_LOG_ENABLED(pCpriCtx) ((pCpriCtx)->TxLogEnable)
#define CPRI_TX_LOG_ENABLE(pCpriCtx) (((pCpriCtx)->TxLogEnable = 1))
#define CPRI_TX_LOG_DISABLE(pCpriCtx) (((pCpriCtx)->TxLogEnable = 0))

#define CPRI_RX_LOG_ENABLED(pCpriCtx) ((pCpriCtx)->RxLogEnable)
#define CPRI_RX_LOG_ENABLE(pCpriCtx) (((pCpriCtx)->RxLogEnable = 1))
#define CPRI_RX_LOG_DISABLE(pCpriCtx) (((pCpriCtx)->RxLogEnable = 0))

#define CPRI_GET_TXDMA_ADDR(bufsize) (CRAM_BASEADDR + DevInfoGetSize(MEMID_CRAM) - (ROUND(bufsize, 32)))

extern CPRICONFIG CpriConfigByHost; // Configuration structure for CPRI configuring from host
extern BOOL bCpriConfigByHost;      // The flag which informs that a host has configured CPRI
extern BOOL bSftCtrlEnabled;
extern UINT32 SfpSrdsID;
extern UINT32 SfpMask;

void CpriSrds1Init(UINT32 CpriRate, UINT32 loopback, UINT32 prbs);
void CPRIDrvSetCtrlWord(PCPRIREGS pCpriRegs, UINT32 Index, UINT32 Value);
UINT32 CPRIDrvGetCtrlWord(PCPRIREGS pCpriRegs, UINT32 Index);
UINT32 SrdsSfpCtrl(UINT32 SrdsId, UINT32 SfpEnableMask);
MXRC CPRIDrvShutdown(void);

/** @brief This function just stops device and does not free any resources
           like allocated memory. It's needed to save allocated resources for
           the futher debugging and do not touch potentially corrupted heaps

    @return [MXRC] an error code */

MXRC CPRIDrvGenShedding(void);

/** @brief This function is designed to set CPRI HW loopback (TX->RX)

    @param nDev [in] - the CPRI dev ID [0..5]
    @param bLB  [in] - LB mode: 1 - set LB, 0 - reset LB

    @return [MXRC] an error code */

MXRC CPRIDrvSetLB (UINT32 nDev, BOOL bLB);

MXRC CPRIDrvInit(PCPRICONFIG pCpriConfig);
MXRC CPRIDrvShutdown(void);
void CPRIDrvTxLogEnable(UINT32 DevID);
void CPRIDrvTxLogDisable(UINT32 DevID);
void CPRIDrvRxLogEnable(UINT32 DevID);
void CPRIDrvRxLogDisable(UINT32 DevID);
UINT32 CPRIDrvGetTxIntCount(UINT32 DevID);
UINT32 CPRIDrvGetRxIntCount(UINT32 DevID);
UINT32 CPRIDrvWaitForLink(PCPRIREGS pCpriRegs, UINT32 timeout);
void CPRIDrvEthTxFrame(UINT32 DevID, LPVOID pTxEthFrame, UINT32 TxFrameSize);
BOOL CPRIDrvEthRxReady(UINT32 DevID);
UINT32 CPRIDrvEthRxFrame(UINT32 DevID, LPVOID pRxFrame);
void CPRIDrvEnableDma(UINT32 DevID);
void CPRIDrvDisableDma(UINT32 DevID);
void CPRIDrvPauseDma(UINT32 DevID);
void CPRIDrvResumeDma(UINT32 DevID);
void CPRIDrvSetReMac(PCPRIREGS pCpriRegs, UINT8 *pMac);
void CPRIDrvSetReIp(PCPRIREGS pCpriRegs, UINT32 ReIpAddr);
void CPRIDrvSetRecIp(PCPRIREGS pCpriRegs, UINT32 ReIpAddr);
PCPRICTX GetCPRICtx(UINT32 DevID);
PCPRIDRVCTX CPRIDrvGetCtx (void);

LPVOID CPRIDrvGetTxBuffer (UINT32 nDevID);
LPVOID CPRIDrvGetRxBuffer (UINT32 nDevID);
void CPRIDrvCfgProc(PCPRICONFIG CpriCfg);
MXRC CPRIDrvCfgApi(ApiHeader *pApi, ApiHeader *pResp);
UINT32 GetCPRIDmaSize(UINT32 DevID);
UINT32 CPRIDrvGetRxLogStatus(UINT32 DevID);
UINT32 CPRIDrvGetTxLogStatus(UINT32 DevID);

UINT32 CPRIDrvGetTxLogSize(UINT32 DevID, UINT32 AnId);
UINT32 CPRIDrvGetRxLogSize(UINT32 DevID, UINT32 AnId);
CPRIREGS * CpriGetRegs (CPRICONFIG *pCfg, UINT32 nDevID);
void CpriMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif /* __CPRIDRV_H__ */

#ifdef __cplusplus
}
#endif

