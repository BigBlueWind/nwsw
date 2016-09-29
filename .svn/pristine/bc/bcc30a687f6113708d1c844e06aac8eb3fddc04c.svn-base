/** @file rrciface.h
 *
 * @brief Interface to LTE RRC layer (L3)
 * @author Mindspeed Technologies
 * @version $Revision: 1.18 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef RRCIFACE_H_
#define RRCIFACE_H_

#include "resultcodes.h"
#include "osal.h"

#define TX_FIX_EN
#define TX_FIX_SIZE                         3422//1419


#define ERROR_RRC_INIT                      0x1
#define ERROR_RRC_UNKNOWN_API               0x2
#define ERROR_RRC_UNKNOWN_MSGTYPE           0x3
#define ERROR_RRC_REQUEST                   0x4

//RRC APIs which added as TLV in the RRC APIs in apidefs.h attached

#define	RR_CONFIG_COMMON                    0x0001
#define	RRCONFIG_DEDICATED                  0x0002
#define	SRB_TOADD_MOD                       0x0003
#define	DRB_TOADD_MOD                       0x0004
#define	DRB_TORELEASE_LIST                  0x0005
#define	RLC_CONFIG                          0x0006
#define	PDCP_CONFIG                         0x0007
#define	LC_CONFIG                           0x0008
#define	MAC_MAIN_CONFIG                     0x0009
#define	PHY_CONFIG_DEDICATED                0x000A

#define	PDSCH_CONFIG_DEDICATED              0x000B
#define	PUSCH_CONFIG_DEDICATED              0x000C
#define	PUCCH_CONFIG_DEDICATED              0x000D
#define	RACH_CONFIG_DEDICATED               0x001E
#define	TPC_PDCCH_CONFIG                    0x000F

#define	UL_POWER_CONTROL_DEDICATED          0x0010
#define	CQI_REPORT_CONFIG                   0x0011
#define	SOUNDING_RS_UL_CONFIG_DEDICATED     0x0012
#define	ANTENNA_INFO_DEDICATED              0x0013
#define	SCHEDULING_REQ_CONFIG               0x0014
#define	MEAS_CONFIG                         0x0015
#define	MOBILITY_CONTROL_INFO               0x001A
#define	CARRIER_FREQ_EUTRA                  0x001B
#define	CARRIER_BANDWIDTH_EUTRA             0x001C
#define	ADDITIONAL_SPECTRUM_EMISION         0x001D
#define	DEDICATED_INFO_NAS                  0x001E
#define	SECURITY_CONFIG_HO                  0x001F

// SYSTEM_CTRL_REQ

#define SYSTEM_CTRL_REQ_COMMON              0xF008//0x0100
#define SYSTEM_CTRL_REQ_REQUEST             0x404b//0xF009//0200
#define CELL_CONF_REQ                       0xF00A//0x0201
#define RADIOBEARER_LIST                    0xf017
#define RADIOBEARER_TYPE                    0xf133
#define RADIOBEARER_ID                      0x4004
#define RADIOBEARER_CONFIG                  0x40ac
#define RADIOBEARER_CONFIG_INFO             0xf136
#define LOGICAL_CHANNEL_ID                  0xf14e
#define MAC_CONFIG                          0xf14f
#define MAC_LOGICAL_CHANNEL_CONFIG          0xf150
#define MAC_TESTMODE_CONFIG                 0x40b8
#define MAC_TESTMODE_CONFIG_NONE            0x40B7
#define MAC_TESTMODE_INFO                   0xf154
#define MAC_TEST_DLLOG_CH_ID                0x40c8
#define MAC_TEST_SCH_NoHEADER_MANIP_TYPE    0xf157
#define PDCP_CONFIG_TYPE                    0x40ae
#define RLC_CONFIG_TYPE                     0xf140

// MAC-PHY

extern OSAL_SEMAPHORE MacToPhySema;
extern OSAL_SEMAPHORE LteMacUlSema;

extern OSAL_MEMARRAY RRC_MAC_MsgArray;

RESULTCODE RrcInit(void* phy_out, void* phy_in);
RESULTCODE RrcDone(void);
RESULTCODE RrcDispatch(U32 size, PTR msg);

U32 MS_Close(void);
RESULTCODE SvsrUeMmi(PTR msg);
U32 RRC_Ctx_Init(HANDLE pHeap);
void RRC_Destroy(void);
void RrcConnectionRequest(void);
void RrcConnectionComplete(void);
void RrcConnectionReconfigComplete(void);
void RrcConnectionRelesase(void);
void RrcDlInfoTransfer(void);
void RrcUlInfoTransfer(void);
RESULTCODE RrcSendToMac(void* pParam, U16 ParamLen);

#endif /* RRCIFACE_H_ */
