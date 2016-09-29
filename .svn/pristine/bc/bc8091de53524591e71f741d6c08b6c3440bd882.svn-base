#ifndef __SRDSDRV_H__
#define __SRDSDRV_H__

/*****************************************************************************
 *                                                                            *
 *       Copyright 2010 by Mindspeed Technologies, Inc.                       *
 *       All Rights Reserved                                                  *
 *                                                                            *
 *       Mindspeed Technologies, Inc.                                         *
 *       4000 MACARTHUR BLVD - EAST TOWER                                     *
 *       NEWPORT BEACH, CA 92660                                              *
 *                                                                            *
 ******************************************************************************
 *       srdsdrv.c                                                            *
 *                                                                            *
 *.      SerDes driver                                                        *
 *                                                                            *
 *****************************************************************************/

#include "config.h"
#include "4gmx.h"
#include "appids.h"

#define RC_SRDES_HW_ID_ERROR    DEF_USRRC(APP_DRIVER, SERDES_DRV, 1)
#define RC_SERDES_DEV_TIMEOUT   DEF_USRRC(APP_DRIVER, SERDES_DRV, 2)

// CPRI rates
#define SRDSRATE_1228       2
#define SRDSRATE_2457       4
#define SRDSRATE_3072       5
#define SRDSRATE_4915       8
#define SRDSRATE_6144       10

#define SRDSMODE_PCIE       0x4
#define SRDSMODE_SRIO       0x9
#define SRDSMODE_CPRI       0xB
#define SRDSMODE_PRBS       0xC

// SRIO and PCIe 1 rate
#define SRDSRATE_2500       0
// PCIe 2 rate
#define SRDSRATE_5000       1




void SerdesGemWriteReg(UINT32 SrdsAddr, UINT32 Val);
UINT32 SerdesGemReadReg(UINT32 SrdsAddr);
//void  SerdesGemInit(UINT32 SrdsRate);
void  SerdesGemInit(void);
void  Serdes0Init(UINT32 SrdsRate, UINT32 SrdsMode);
//void  Serdes1Init(UINT32 SrdsRate);
UINT32 Serdes1IsInited(UINT32 CpriRate, UINT32 nLbMode);
void Serdes1Init(UINT32 CpriRate, UINT32 SrdsMode, UINT32 loopback, UINT32 prbs);
//void  Serdes2Init(UINT32 SrdsRate);
void  Serdes2Init(UINT32 SrdsRate, UINT32 SrdsMode);
UINT32 SrdsSfpCtrl(UINT32 SrdsId, UINT32 SfpEnableMask);
void SysDelay(UINT32 ms);
void SrdsMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif /*__SRDSDRV_H__*/
