/** @file svsr.h
 *
 * @brief Supervisor message handling
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef SVSR_H_
#define SVSR_H_

#include "resultcodes.h"

//
// Limits
//
#define SVSR_MAXMACINST             6
#define SVSR_MAXPHYINST             6

//
// All command handlers must follow this definition
//
typedef RESULTCODE (*CmdHandler)(PTR);

//
// API handler mapping
//
typedef struct CmdMap
{
    U16 msgID;
    CmdHandler cmdHandler;
} CMD_MAP;

//
// Dynamic Instance IDs
//
typedef struct InstMap
{
    U16 instanceID;
    U16 instanceType;
} INST_MAP;

typedef struct _CevaDump_
{
    U32  CevaID;         // It's used in the response 
    U8*  pDumpBuf;       // The pointer to the dump buffer 
    U32  DumpBufSize;    // The original buffer size 
    U32  DumpSize;       // The size of data placed by CEVA
}CevaDumpCtx;

typedef struct _SvsrApiMsgStats_
{
    U32 CtrlMsgTotalCount;

    U32 CtrlMsgToHostToUsCount;

    U32 CtrlMsgToSvsrCount;
    U32 CtrlMsgToHostCount;
    U32 CtrlMsgToHostGenCount;
    U32 CtrlMsgToMacCount;
    U32 CtrlMsgToAuxCount;
    U32 CtrlMsgToAuxCount1;
    U32 CtrlMsgToAuxCount2;
    U32 CtrlMsgToRrcCount;
    U32 CtrlMsgToOtherCount;
    U32 CtrlMsgToOtherCount1;
    U32 CtrlMsgToOtherCount2;
    U32 CtrlMsgToOtherCountErr1;
    U32 CtrlMsgToOtherCountErr2;

    U32 CtrlMsgToIcpuCount;
    U32 CtrlMsgToIcpuWeCount;
    U32 CtrlMsgToIcpuWeAuxCount;
    U32 CtrlMsgToIcpuWeCountErr1;
    U32 CtrlMsgToIcpuLarmCount;
    U32 CtrlMsgToIcpuAuxCount;
    U32 CtrlMsgToIcpuHostCount;
    U32 CtrlMsgToIcpuHostHbiCount;
    U32 CtrlMsgToIcpuHostCount1;
    U32 CtrlMsgToIcpuHostCount2;
    U32 CtrlMsgToIcpuHostCountErr;
    U32 CtrlMsgToIcpuOtherCount;
    U32 CtrlMsgToIcpuOtherCount1;
    U32 CtrlMsgToIcpuOtherCountErr;

    U32 CtrlMsgToLocalCount;

    U32 CtrlMsgToHostRxSduCount1;
    U32 CtrlMsgToHostRxSduCount2;
    U32 CtrlMsgToHostRxSduCount3;
    U32 CtrlMsgToHostRxSduCountErr1;
    U32 CtrlMsgToHostRxSduCountErr2;
    U32 CtrlMsgToHostRxSduCount4;

    U32 CtrlMsgToHostRxSduCount5;
    U32 CtrlMsgToHostRxSduCount6;
    U32 CtrlMsgToHostRxSduCountErr3;

    U32 SctpTxMsgCount;
    U32 SctpTxMsgCount1;
    U32 SctpTxMsgCountErr1;
    U32 SctpTxMsgCountErr2;
    U32 numPackQueue;

}SVSR_APIMSG_STATS;

extern SVSR_APIMSG_STATS SvsrApiMsgStats;

RESULTCODE SvsrInit(void);
RESULTCODE SvsrDone(void);
EXPORT RESULTCODE SvsrDispatch(U32 size, PTR msg);

// Send indication to HOST
RESULTCODE SvsrReady(void);

//
// Command handlers
//

RESULTCODE SvsrCommand(U16 srcID, U16 msgID, U32 paramCount, ...);

// SYS range
RESULTCODE SvsrLoadLarm(PTR msg);
RESULTCODE SvsrLoadDsp(PTR msg);
RESULTCODE SvsrLoadUarm(PTR msg);
RESULTCODE SvsrReset(PTR msg);
RESULTCODE SvsrUnload(PTR msg);
RESULTCODE SvsrApiMsgStatsHandler(PTR msg);

// MAC range
RESULTCODE SvsrCreateMac(PTR msg);
RESULTCODE SvsrCreatePhy(PTR msg);
RESULTCODE SvsrDestroyMac(PTR msg);
RESULTCODE SvsrDestroyPhy(PTR msg);
RESULTCODE SvsrPhyTick(PTR msg);

//RRC range
RESULTCODE SvsrCreateRrc(PTR msg);
RESULTCODE SvsrDestroyRrc(PTR msg);

RESULTCODE SvsrLarmHDiagCevaDump(PTR msg);

#endif /* SVSR_H_ */
